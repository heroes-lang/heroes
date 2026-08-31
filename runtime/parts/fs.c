/* parts/fs.c — the filesystem operations, and the one place that knows what
 * machine it is on.
 *
 * WHY THESE ARE HERE AND NOT IN `selfhost/`. The compiler used to create a
 * directory by writing `mkdir -p 'build/x'` and handing it to `system()`. On
 * `cmd.exe` that creates a directory called `-p`, which is how the Windows CI
 * leg died at `heroes doctor` from 2026-08-24 until this file existed. The
 * obvious repair — bind `mkdir` from `sys/stat.h` and `_mkdir` from `direct.h`
 * — was taken to panel 097 and vetoed there, because it cannot be written:
 * `direct.h` is `ffi_missing_header` off Windows, this language has no `#if` by
 * design, and `struct stat`'s `st_mode` is two bytes on Darwin against four on
 * glibc, so no `record` spelling passes both (measured, cross-compiled). The
 * platform arm belongs to the runtime — DESIGN-LOG:282's rule, and the same one
 * `parts/os.c:21` already follows for `_setmode`.
 *
 * WHAT EACH ONE OWES THAT THE SHELL WAS PROVIDING SILENTLY, measured by panel
 * 097's two seats and re-run here:
 *   - `mkdir -p` succeeds on a directory that already exists, and creates
 *     intermediate ones. `mkdir` answers -1/EEXIST for both — and EEXIST is
 *     also what it answers for a *file* sitting where the directory should be,
 *     so errno cannot tell the two apart and this file asks `is_directory`
 *     instead (panel 097 condition 12).
 *   - `rm -f` succeeds on a file that is not there. `remove` answers -1/ENOENT.
 *   - `mv -f` replaces its target and is atomic on POSIX. `rename` is too;
 *     Windows' is not, and that is written down below rather than smoothed over.
 */

#if defined(_WIN32)
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif

/* Is there a directory at this path? The question `test -d` asked, and the one
 * `mkdir`'s errno cannot answer. */
int64_t hero_fs_is_directory(const char *path) {
#if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return 0;
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0 ? 1 : 0;
#else
    struct stat info;
    if (stat(path, &info) != 0) return 0;
    return S_ISDIR(info.st_mode) ? 1 : 0;
#endif
}

/* Is there anything at all at this path? `test -e`. */
int64_t hero_fs_exists(const char *path) {
#if defined(_WIN32)
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES ? 1 : 0;
#else
    struct stat info;
    return stat(path, &info) == 0 ? 1 : 0;
#endif
}

/* One directory, its parents assumed. HERO_OS_OK if it exists as a directory
 * afterwards, whoever made it. */
static int64_t hero_fs_mkdir_one(const char *path) {
#if defined(_WIN32)
    if (_mkdir(path) == 0) return HERO_OS_OK;
#else
    if (mkdir(path, 0777) == 0) return HERO_OS_OK;
#endif
    /* It failed. EEXIST is the same answer for a directory that is already
     * there — which is success — and for a FILE sitting where the directory
     * should be, which is not. Ask the filesystem rather than errno. */
    if (hero_fs_is_directory(path)) return HERO_OS_OK;
    return HERO_OS_FAILED;
}

/* `mkdir -p`: every component, and an existing directory is success. The
 * separator is `/` on both platforms — Windows accepts it in every filesystem
 * call, which `parts/os.c:104` already records — so no path is rewritten here
 * and panel 057's refusal of `\` as a separator is untouched. */
int64_t hero_fs_mkdir_all(const char *path) {
    size_t length = strlen(path);
    if (length == 0) return HERO_OS_FAILED;
    if (length >= HERO_FS_PATH_MAX) return HERO_OS_FAILED;

    char work[HERO_FS_PATH_MAX];
    memcpy(work, path, length);
    work[length] = '\0';

    for (size_t i = 1; i < length; i += 1) {
        if (work[i] != '/') continue;
        work[i] = '\0';
        if (hero_fs_mkdir_one(work) != HERO_OS_OK) return HERO_OS_FAILED;
        work[i] = '/';
    }
    return hero_fs_mkdir_one(work);
}

/* Is `path` newer than `reference`? The question `find -newer` asked, and the
 * one a build cache asks constantly: did this object get rebuilt after that
 * marker was dropped.
 *
 * -1 when either path cannot be stat'd, so a caller can tell "older" from
 * "not there" — which `find -newer` could not, because a missing file simply
 * did not appear in its output. */
int64_t hero_fs_newer_than(const char *path, const char *reference) {
#if defined(_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA a;
    WIN32_FILE_ATTRIBUTE_DATA b;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &a)) return -1;
    if (!GetFileAttributesExA(reference, GetFileExInfoStandard, &b)) return -1;
    return CompareFileTime(&a.ftLastWriteTime, &b.ftLastWriteTime) > 0 ? 1 : 0;
#else
    struct stat mine;
    struct stat theirs;
    if (stat(path, &mine) != 0) return -1;
    if (stat(reference, &theirs) != 0) return -1;
    if (mine.st_mtime != theirs.st_mtime) return mine.st_mtime > theirs.st_mtime ? 1 : 0;
    /* Same second: the sub-second field decides, where the platform has one.
     * A build can produce two files inside one second and often does. */
#if defined(__APPLE__)
    return mine.st_mtimespec.tv_nsec > theirs.st_mtimespec.tv_nsec ? 1 : 0;
#elif defined(st_mtime)
    return mine.st_mtim.tv_nsec > theirs.st_mtim.tv_nsec ? 1 : 0;
#else
    return 0;
#endif
#endif
}

/* `rm -f`: gone afterwards, and a path that was never there is success.
 *
 * **WINDOWS RETRIES, because a file there can be busy for reasons that are
 * nobody's bug and last milliseconds.** A just-exited .exe keeps its image
 * mapped for a beat after the process is gone, and Defender opens every fresh
 * binary the moment it appears — so a delete that raced either one answered
 * "cannot remove" for a file that was deletable forty milliseconds later.
 * Measured 2026-08-31 on the net's own suites: five checks red as `cannot lay
 * the program out under build/harness` and `cannot remove build/tu-…`, on
 * paths a retry-by-hand then removed cleanly. Ten tries over ~1.3 s is far
 * past both causes; a file still held after that is genuinely held, and the
 * caller hears it. POSIX never retries: a busy file there deletes anyway
 * (the name goes, the inode lingers), so the loop would be dead code. */
int64_t hero_fs_remove(const char *path) {
    if (remove(path) == 0) return HERO_OS_OK;
    if (!hero_fs_exists(path)) return HERO_OS_OK;
#if defined(_WIN32)
    for (int wait_ms = 5; wait_ms <= 320; wait_ms *= 2) {
        Sleep((DWORD)wait_ms);
        if (remove(path) == 0) return HERO_OS_OK;
        if (!hero_fs_exists(path)) return HERO_OS_OK;
    }
#endif
    return HERO_OS_FAILED;
}

/* `mv -f`: the target is replaced, and on POSIX no reader ever sees it absent.
 *
 * THE WINDOWS ARM IS NOT ATOMIC AND THAT IS A LOSS, WRITTEN DOWN RATHER THAN
 * DISCOVERED (panel 097 condition 11). C's `rename` refuses an existing target
 * on Windows — C11 7.21.4.2 leaves it implementation-defined — so the arm below
 * asks `MoveFileEx` for the replacement instead. Microsoft documents
 * MOVEFILE_REPLACE_EXISTING as atomic when both paths are on one volume, which
 * is the only case this compiler produces: the staged file is written beside its
 * destination in the same build directory. **NOT VERIFIED on Windows** — nobody
 * on this project has the machine, and the first tag run that goes green there
 * is what turns this paragraph from a claim into a measurement.
 *
 * It matters because of what calls it: `cli_toolchain.hero` publishes a cache
 * artifact by renaming it into place *after* its dependencies are recorded, so
 * that an object on disk is never one the cache cannot justify serving. A
 * remove-then-rename would open a window where the object is simply absent. */
int64_t hero_fs_rename(const char *from, const char *to) {
#if defined(_WIN32)
    if (MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING)) return HERO_OS_OK;
    return HERO_OS_FAILED;
#else
    if (rename(from, to) == 0) return HERO_OS_OK;
    return HERO_OS_FAILED;
#endif
}
