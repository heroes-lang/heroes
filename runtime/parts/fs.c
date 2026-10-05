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

#include <errno.h>
#if defined(_WIN32)
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#endif
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

/* Each call below that can fail clears `hero_fs_why_code` on the way in and
 * sets it where it fails; `parts/os.c` defines it and says why it exists. */

#if defined(_WIN32)
/* A path's attributes, asked WIDE (parts/codepage.c), and so are the removal
 * below and the two questions it asks: `hero_dir_remove_tree` hands them the
 * names its listing carried back, and a name holding a surrogate standing
 * alone is not UTF-8 there, so a narrow door read it as another and answered
 * *not there* for a file that was (panel 191's R2). Every UTF-8 name is the
 * narrow door's own answer under the code page this runtime carries.
 * INVALID_FILE_ATTRIBUTES where nothing is there or the bytes name nothing. */
static DWORD hero_fs_attributes(const char *path) {
    wchar_t *wide = hero_win_wide(path, NULL);
    if (wide == NULL) return INVALID_FILE_ATTRIBUTES;
    DWORD attrs = GetFileAttributesW(wide);
    hero_release(wide);
    return attrs;
}
#endif

#if defined(_WIN32)
/* Is `path` a symbolic link or a junction (a reparse point that NAMES another
 * file) rather than a file some filter keeps a reparse point on, as a cloud
 * placeholder is? `FindFirstFileW` reports the tag in `dwReserved0`. */
static int hero_fs_is_surrogate(const char *path) {
    /* Wide, as the directory walk is (parts/codepage.c): a name whose UTF-8
     * passes 260 bytes failed the narrow call and read as no link at all. */
    wchar_t *wide = hero_win_wide(path, NULL);
    if (wide == NULL) return 0;
    WIN32_FIND_DATAW found;
    HANDLE h = FindFirstFileW(wide, &found);
    hero_release(wide);
    if (h == INVALID_HANDLE_VALUE) return 0;
    FindClose(h);
    return (found.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0 &&
           IsReparseTagNameSurrogate(found.dwReserved0);
}
#endif

/* Is there a directory at this path? The question `test -d` asked, and the one
 * `mkdir`'s errno cannot answer. */
int64_t hero_fs_is_directory(const char *path) {
#if defined(_WIN32)
    DWORD attrs = hero_fs_attributes(path);
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
    return hero_fs_attributes(path) != INVALID_FILE_ATTRIBUTES ? 1 : 0;
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
    /* Kept before the question below, which calls `stat` and may move it. */
    int failure = errno;
    /* It failed. EEXIST is the same answer for a directory that is already
     * there — which is success — and for a FILE sitting where the directory
     * should be, which is not. Ask the filesystem rather than errno. */
    if (hero_fs_is_directory(path)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)failure;
    return HERO_OS_FAILED;
}

/* `mkdir -p`: every component, and an existing directory is success. The
 * separator is `/` on both platforms — Windows accepts it in every filesystem
 * call, which `parts/os.c:104` already records — so no path is rewritten here
 * and panel 057's refusal of `\` as a separator is untouched. */
int64_t hero_fs_mkdir_all(const char *path) {
    hero_fs_why_code = 0;
    size_t length = strlen(path);
    if (length == 0) {
        hero_fs_why_code = -(int64_t)ENOENT;
        return HERO_OS_FAILED;
    }
    if (length >= HERO_FS_PATH_MAX) {
        hero_fs_why_code = -(int64_t)ENAMETOOLONG;
        return HERO_OS_FAILED;
    }

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
#if defined(_WIN32)
/* Is there a directory at this wide path, and anything at all: `hero_fs_remove`'s
 * two questions, asked of the name it was handed once widened. */
static int hero_fs_wide_is_directory(const wchar_t *wide) {
    DWORD attrs = GetFileAttributesW(wide);
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

static int hero_fs_wide_exists(const wchar_t *wide) {
    return GetFileAttributesW(wide) != INVALID_FILE_ATTRIBUTES;
}

static int64_t hero_fs_remove_wide(const wchar_t *wide) {
    if (_wremove(wide) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    if (!hero_fs_wide_exists(wide)) {
        hero_fs_why_code = 0;
        return HERO_OS_OK;
    }
    /* **MSVC's `remove()` does not remove a DIRECTORY** — C11 leaves it
     * unspecified and POSIX's does, so the difference hid until a suite
     * deleted a tree it had built earlier in the same run: on a fresh
     * `build/` every earlier remove_tree hit the path-was-never-there exit,
     * and the first delete-then-recreate (`cache/`, `units/`) was the first
     * time a directory unlink actually executed. Measured 2026-08-31: five
     * checks red as `cannot lay the program out`, all of them this line. */
    if (hero_fs_wide_is_directory(wide)) {
        if (RemoveDirectoryW(wide)) {
            hero_fs_why_code = 0;
            return HERO_OS_OK;
        }
        hero_fs_why_code = (int64_t)GetLastError();
    }
    for (int wait_ms = 5; wait_ms <= 320; wait_ms *= 2) {
        Sleep((DWORD)wait_ms);
        int directory = hero_fs_wide_is_directory(wide);
        if (directory ? RemoveDirectoryW(wide) != 0 : _wremove(wide) == 0) {
            hero_fs_why_code = 0;
            return HERO_OS_OK;
        }
        /* The last refusal is the one reported. */
        hero_fs_why_code = directory ? (int64_t)GetLastError() : (int64_t)errno;
        if (!hero_fs_wide_exists(wide)) {
            hero_fs_why_code = 0;
            return HERO_OS_OK;
        }
    }
    return HERO_OS_FAILED;
}
#endif

int64_t hero_fs_remove(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    /* Wide, as `hero_fs_attributes` above says why. Bytes that name nothing
     * are a refusal, never the *not there* that is success here. */
    wchar_t *wide = hero_win_wide(path, NULL);
    if (wide == NULL) {
        hero_fs_why_code = -(int64_t)ERROR_NO_UNICODE_TRANSLATION;
        return HERO_OS_FAILED;
    }
    int64_t removed = hero_fs_remove_wide(wide);
    hero_release(wide);
    return removed;
#else
    if (remove(path) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    if (!hero_fs_exists(path)) {
        hero_fs_why_code = 0;
        return HERO_OS_OK;
    }
    return HERO_OS_FAILED;
#endif
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
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING)) return HERO_OS_OK;
    /* The one this compiler meets: ERROR_ACCESS_DENIED (5) or
     * ERROR_SHARING_VIOLATION (32) where another process holds `to` open or is
     * running it, which POSIX never refuses. `selfhost/cli/publish.hero` is
     * what decides whether that is another build's copy of the same file. */
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (rename(from, to) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    return HERO_OS_FAILED;
#endif
}

/* WHERE THE RUNNING EXECUTABLE IS (defect 277), so the compiler can look for
 * `runtime/` under an ancestor of it, the third place design.md §3.1 and
 * panel 020 rule, for an installed compiler that has no repository above it.
 * The answer is the shown read's (`hero_bytes_shown`, parts/os.c): HERO_OS_OK
 * and the path where it is UTF-8, HERO_OS_NOT_TEXT with marks where it is
 * not, HERO_OS_NOT_FOUND where this machine gives no answer. Each platform's
 * own call, the one place that knows which machine it is on:
 *
 *   - Windows: `GetModuleFileNameW(NULL, ...)`, the path the module was
 *     loaded by, carried back as `hero_win_bytes` writes it (parts/codepage.c):
 *     the narrow door answered a surrogate standing alone as U+FFFD under the
 *     UTF-8 code page, a valid path naming another folder, where these bytes
 *     are not UTF-8 and are named (defect 238); a path at the buffer's bound
 *     is cut, and is no answer.
 *   - macOS: `_NSGetExecutablePath`, then `realpath`, so a compiler reached
 *     through a link answers where it lives, as Linux's answer does.
 *   - Linux and every other POSIX: `/proc/self/exe`, the resolved path; where
 *     `/proc` is not mounted there is no answer, never a guess from `argv[0]`,
 *     which a shell sets to whatever was typed.
 *
 * Fixed buffers of HERO_FS_PATH_MAX, so nothing here allocates outside
 * `parts/alloc.c` (the `runtime` suite's rule 1); `realpath` is given its
 * buffer, which POSIX asks be PATH_MAX, never fewer than these 4096 bytes on
 * the two platforms that run it. */
HeroStr hero_exe_path_shown(int64_t *status, int64_t *marks) {
    *marks = 0;
#if defined(_WIN32)
    wchar_t wide[HERO_FS_PATH_MAX];
    DWORD got = GetModuleFileNameW(NULL, wide, (DWORD)HERO_FS_PATH_MAX);
    if (got == 0 || got >= (DWORD)HERO_FS_PATH_MAX) {
        *status = HERO_OS_NOT_FOUND;
        return hero_str_from_bytes("", 0);
    }
    int64_t length = 0;
    char *bytes = hero_win_bytes(wide, &length);
    HeroStr shown = hero_bytes_shown(bytes, length, status, marks);
    hero_release(bytes);
    return shown;
#else
    char found[HERO_FS_PATH_MAX];
    int64_t length = -1;
#if defined(__APPLE__)
    char raw[HERO_FS_PATH_MAX];
    uint32_t size = (uint32_t)sizeof raw;
    if (_NSGetExecutablePath(raw, &size) == 0 && realpath(raw, found) != NULL) length = (int64_t)strlen(found);
#else
    ssize_t got = readlink("/proc/self/exe", found, sizeof found - 1);
    if (got > 0 && (size_t)got < sizeof found - 1) length = (int64_t)got;
#endif
    if (length < 0) {
        *status = HERO_OS_NOT_FOUND;
        return hero_str_from_bytes("", 0);
    }
    return hero_bytes_shown(found, length, status, marks);
#endif
}
