/* parts/replace.c: replacing a file whose name the AUTHOR gave, whole or not
 * at all (design.md §1.12; defect 136).
 *
 * WHY THIS EXISTS. `heroes fmt --in-place` and `heroes check --apply
 * --in-place` wrote the author's source through `hero_file_write`, which is
 * `fopen(path, "wb")`: the file is emptied before a byte of the new text is
 * written. Measured 2026-09-30 on a 2 MB HFS+ image: `fmt` left 0 bytes of a
 * 62,706-byte program with 8 KB free, `check --apply` left 57,344 of the 59,312
 * bytes it meant, cut inside `return`; a SIGKILL the instant the file changed
 * left 0 bytes in 6 runs of 6, and one 40 to 100 microseconds later left
 * 1,204,224 of a 1,206,709-byte text. So the new text is written under a private name beside the
 * file, flushed to the device, and only then renamed over it: the name holds the
 * old text or the new one, never a part of either (the same kills after it: 25
 * of 25 left the old text or the new, and a kill before the rename leaves the
 * private name beside it).
 *
 * WHAT A RENAME OWES THE FILE IT REPLACES. The in-place write kept everything a
 * file is besides its bytes, because it wrote into the same file; a rename puts a
 * new one there. Measured the same day, before this part existed, the in-place
 * write kept every one of these: the mode (444, 600, 640, 755), a group other than
 * its directory's, both names of a hard link, a symbolic link and a chain of two,
 * an extended attribute, an ACL, and the file's flags (`hidden` and `nodump` on
 * Darwin, `nodump` and `noatime` on Linux). So each is carried to the new file,
 * or the rewrite is refused before anything is written:
 *
 *   - a symbolic link is followed to the file it names, and THAT file is
 *     replaced: the link stays a link (`hero_fs_landing`);
 *   - a file that could not be opened for writing is refused, as `fopen`
 *     refused it: a read-only mode, an immutable flag, an ACL that denies
 *     writing (`hero_fs_writable`). A rename needs only the DIRECTORY to be
 *     writable, so without the question it would replace what the author
 *     protected, which is exactly what `--emit-c -o` did from defect 134's
 *     repair until this one, measured;
 *   - the owner, the group, the permission bits, the flags, the ACL and the
 *     extended attributes are given to the new file before it takes the name
 *     (`hero_file_stage`), and a new file that cannot have them is removed with
 *     the old one untouched. The flags were missing from this part's first
 *     version, which lost them on both platforms (measured);
 *   - a new file that could not take the name is removed whatever it carries: a
 *     Darwin ACL denying the file's deletion refuses the rename, and carried to
 *     the copy it refused the copy's own removal too, which stayed beside the
 *     author's file until `hero_file_unstage` (measured);
 *   - a file with more than one name cannot be carried at all, since the other
 *     names would go on holding the old text; `hero_fs_links` is how the caller
 *     sees one and refuses it.
 *
 * WHAT IS NOT HERE is the policy: which questions are asked of which file, and
 * the words said when one is refused, are `selfhost/cli/publish.hero`'s. This
 * part answers and does not decide.
 *
 * THE WINDOWS ARM, MEASURED ON THE BOX on 2026-09-30 (lane 136, at `9cc3a295`):
 * the compiler's own tests 947, all passed, and `surface`, `fixes` and `probe`
 * at 0 failed. `fmt --in-place` on a 16 MB NTFS volume left the file byte for
 * byte at 8, 32 and 64 KB free (ERROR_DISK_FULL, 112), where the write before
 * this part left it at 0 bytes. A read-only file and one another process holds
 * open for reading alone are refused untouched (5 and 32); a hidden, a system
 * and a hidden system file are rewritten with their attributes, where `fopen`
 * refused all three; a second name is refused; a link, to a source or as an
 * `-o`, is written through and stays a link; `-o NUL` is written into. It
 * carries the file's attributes but not its owner, its explicit ACL entries or
 * its alternate data streams: a new file there takes its owner and the inherited
 * entries of its directory, which no case there measured. No kill landed inside
 * the write there: of 13 per compiler, a Git Bash watcher's came after the write
 * and the fixed delays before or after it.
 */

#include <errno.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <copyfile.h>
#include <membership.h>
#include <sys/acl.h>
#elif defined(__linux__)
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#endif
#endif

/* Each call below clears `hero_fs_why_code` on the way in and sets it where it
 * fails; `parts/os.c` defines it. */

/* A path the runtime found, handed to Heroes as a `str`, or "" with EILSEQ
 * where its bytes are not UTF-8, which `hero_str_from_bytes` would otherwise
 * abort on: a link's target is whatever bytes somebody wrote into it. */
static HeroStr hero_fs_found_path(const char *path) {
    int64_t length = (int64_t)strlen(path);
    if (!hero_utf8_valid(path, length)) {
        hero_fs_why_code = (int64_t)EILSEQ;
        return hero_str_from_bytes("", 0);
    }
    return hero_str_from_bytes(path, length);
}

/* `hero_fs_is_surrogate`, whether a path is a link on Windows, lives in
 * parts/fs.c since 2026-10-05, where `hero_dir_remove_tree` asks it too
 * (defect 345). */

/* The file a write to `path` lands in: `path` itself, or, where `path` is a
 * symbolic link, the file its chain of links finally names, which need not
 * exist yet, so a dangling link is written through and creates what it names, as
 * `fopen` did. A relative target is read from the directory holding the link.
 * "" with `hero_fs_why` set where the chain cannot be followed: forty links deep
 * (ELOOP, the limit Linux itself stops at), or longer than HERO_FS_PATH_MAX.
 *
 * Only the LAST part of the path is followed. A link among the directories on
 * the way is the kernel's to follow, and it follows it the same way for the
 * private name beside the landing, so the rename stays inside one directory and
 * therefore on one volume. */
HeroStr hero_fs_landing(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (!hero_fs_is_surrogate(path)) return hero_fs_found_path(path);
    HANDLE h = CreateFileA(path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                           OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        hero_fs_why_code = (int64_t)GetLastError();
        return hero_str_from_bytes("", 0);
    }
    /* WIDE, and carried back as the listing carries a name (parts/codepage.c):
     * the narrow door answered a target holding a surrogate standing alone
     * as U+FFFD under the UTF-8 code page, valid text naming a file that is
     * not there, which the write below then made beside the link (defect
     * 238). As bytes it is not UTF-8, and `hero_fs_found_path` refuses it as
     * Linux's arm refuses a target whose bytes are not. */
    wchar_t final[HERO_FS_PATH_MAX];
    DWORD got = GetFinalPathNameByHandleW(h, final, (DWORD)HERO_FS_PATH_MAX, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
    if (got == 0 || got >= (DWORD)HERO_FS_PATH_MAX) {
        hero_fs_why_code = got == 0 ? (int64_t)GetLastError() : (int64_t)ERROR_FILENAME_EXCED_RANGE;
        CloseHandle(h);
        return hero_str_from_bytes("", 0);
    }
    CloseHandle(h);
    /* `\\?\C:\dir\name` for a path on a drive: the prefix goes, so the name
     * reads as the author writes one. A UNC or volume path keeps it. */
    const wchar_t *at = final;
    if (got > 6 && wcsncmp(final, L"\\\\?\\", 4) == 0 && final[5] == L':') at = final + 4;
    int64_t length = 0;
    char *bytes = hero_win_bytes(at, &length);
    HeroStr found = hero_fs_found_path(bytes);
    hero_release(bytes);
    return found;
#else
    char current[HERO_FS_PATH_MAX];
    size_t length = strlen(path);
    if (length >= sizeof current) {
        hero_fs_why_code = (int64_t)ENAMETOOLONG;
        return hero_str_from_bytes("", 0);
    }
    memcpy(current, path, length + 1);
    for (int hops = 0; hops < 40; hops += 1) {
        struct stat info;
        /* Not there, or not a link: this is where the write lands. A path
         * that cannot be asked about at all lands here too, and the write
         * itself then says why. */
        if (lstat(current, &info) != 0 || !S_ISLNK(info.st_mode)) return hero_fs_found_path(current);
        char target[HERO_FS_PATH_MAX];
        ssize_t got = readlink(current, target, sizeof target - 1);
        if (got < 0) {
            hero_fs_why_code = (int64_t)errno;
            return hero_str_from_bytes("", 0);
        }
        if ((size_t)got >= sizeof target - 1) {
            hero_fs_why_code = (int64_t)ENAMETOOLONG;
            return hero_str_from_bytes("", 0);
        }
        target[got] = '\0';
        if (target[0] == '/') {
            memcpy(current, target, (size_t)got + 1);
            continue;
        }
        char *slash = strrchr(current, '/');
        size_t keep = slash == NULL ? 0 : (size_t)(slash - current) + 1;
        if (keep + (size_t)got >= sizeof current) {
            hero_fs_why_code = (int64_t)ENAMETOOLONG;
            return hero_str_from_bytes("", 0);
        }
        memcpy(current + keep, target, (size_t)got + 1);
    }
    hero_fs_why_code = (int64_t)ELOOP;
    return hero_str_from_bytes("", 0);
#endif
}

/* What is at `path`: the LINK itself where it is one, as `lstat` answers.
 * HERO_FS_OTHER is what is neither a file, a directory nor a link: a device, a
 * FIFO, a socket, on Windows `NUL`. HERO_FS_UNKNOWN when the question itself
 * failed, with `hero_fs_why` saying why. */
int64_t hero_fs_kind(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (hero_fs_is_surrogate(path)) return HERO_FS_LINK;
    /* Opened for no access at all, which asks nothing of the file's own
     * permissions: a handle's type is what tells `NUL` from a file, where the
     * attributes do not. */
    HANDLE h = CreateFileA(path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                           OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        DWORD e = GetLastError();
        if (e == ERROR_FILE_NOT_FOUND || e == ERROR_PATH_NOT_FOUND) return HERO_FS_ABSENT;
        hero_fs_why_code = (int64_t)e;
        return HERO_FS_UNKNOWN;
    }
    DWORD type = GetFileType(h);
    BY_HANDLE_FILE_INFORMATION info;
    BOOL known = type == FILE_TYPE_DISK && GetFileInformationByHandle(h, &info);
    CloseHandle(h);
    if (type != FILE_TYPE_DISK) return HERO_FS_OTHER;
    if (!known) {
        hero_fs_why_code = (int64_t)GetLastError();
        return HERO_FS_UNKNOWN;
    }
    return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? HERO_FS_DIRECTORY : HERO_FS_FILE;
#else
    struct stat info;
    if (lstat(path, &info) != 0) {
        if (errno == ENOENT) return HERO_FS_ABSENT;
        hero_fs_why_code = (int64_t)errno;
        return HERO_FS_UNKNOWN;
    }
    if (S_ISLNK(info.st_mode)) return HERO_FS_LINK;
    if (S_ISREG(info.st_mode)) return HERO_FS_FILE;
    if (S_ISDIR(info.st_mode)) return HERO_FS_DIRECTORY;
    return HERO_FS_OTHER;
#endif
}

/* How many names the file at `path` has, following a link: 1 for an ordinary
 * file, more for one with hard links. -1 where it cannot be asked. */
int64_t hero_fs_links(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    HANDLE h = CreateFileA(path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                           OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        hero_fs_why_code = (int64_t)GetLastError();
        return -1;
    }
    BY_HANDLE_FILE_INFORMATION info;
    BOOL known = GetFileInformationByHandle(h, &info);
    if (!known) hero_fs_why_code = (int64_t)GetLastError();
    CloseHandle(h);
    return known ? (int64_t)info.nNumberOfLinks : -1;
#else
    struct stat info;
    if (stat(path, &info) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return -1;
    }
    return (int64_t)info.st_nlink;
#endif
}

/* Could the file at `path` be opened for writing (the question `fopen(path,
 * "wb")` asked of it) without emptying it? 1 or 0, and `hero_fs_why` says why
 * not. ETXTBSY is a yes: it says a program is running from the file, which a
 * rename replaces without touching the running image, and it is what a binary
 * rebuilt under its own `-o` meets on Linux. */
int64_t hero_fs_writable(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    HANDLE h = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        hero_fs_why_code = (int64_t)GetLastError();
        return 0;
    }
    CloseHandle(h);
    return 1;
#else
    int fd = open(path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        if (errno == ETXTBSY) return 1;
        hero_fs_why_code = (int64_t)errno;
        return 0;
    }
    close(fd);
    return 1;
#endif
}

#if !defined(_WIN32)
/* The new file's bytes and metadata on the device before its name moves: a
 * rename that reached the disk ahead of the data it names is a file of zeros
 * after a power cut, on the filesystems that delay allocation. Darwin's plain
 * `fsync` leaves the drive free to reorder, so the barrier is asked for there,
 * and a filesystem that does not know it (a network one) gets `fsync`. A
 * filesystem that cannot flush at all (EINVAL, ENOTSUP) has nothing this can
 * ask of it, and the write goes on without the promise. NOT RUN: a power cut is
 * not something this Mac can be asked for; the calls are what POSIX and
 * Darwin's `fcntl(2)` document for this order. */
static int hero_stage_flush(int fd) {
#if defined(__APPLE__) && defined(F_BARRIERFSYNC)
    if (fcntl(fd, F_BARRIERFSYNC) == 0) return 0;
#endif
    if (fsync(fd) == 0) return 0;
    return errno == EINVAL || errno == ENOTSUP ? 0 : -1;
}

/* The ACL and the extended attributes of `like`, given to the new file. */
static int64_t hero_stage_attributes(int fd, const char *like) {
#if defined(__APPLE__)
    int from = open(like, O_RDONLY | O_CLOEXEC);
    if (from < 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_ATTRIBUTES;
    }
    int copied = fcopyfile(from, fd, NULL, COPYFILE_ACL | COPYFILE_XATTR);
    if (copied != 0) hero_fs_why_code = (int64_t)errno;
    close(from);
    return copied == 0 ? HERO_STAGE_DONE : HERO_STAGE_ATTRIBUTES;
#elif defined(__linux__)
    /* Linux keeps a POSIX ACL as the extended attribute
     * `system.posix_acl_access`, so copying every attribute copies it. The
     * `security.` namespace is the security module's (SELinux, Smack), which
     * labels a new file itself: a label it will not let this process set is
     * skipped rather than refused, so that a policy cannot make every rewrite
     * impossible. Every other attribute is carried or the rewrite is refused. */
    int from = open(like, O_RDONLY | O_CLOEXEC);
    if (from < 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_ATTRIBUTES;
    }
    int64_t step = HERO_STAGE_DONE;
    ssize_t room = flistxattr(from, NULL, 0);
    if (room < 0) {
        /* A filesystem with no attributes has none to carry. */
        if (errno != ENOTSUP) {
            hero_fs_why_code = (int64_t)errno;
            step = HERO_STAGE_ATTRIBUTES;
        }
        close(from);
        return step;
    }
    char *names = room > 0 ? hero_alloc((size_t)room) : NULL;
    ssize_t listed = room > 0 ? flistxattr(from, names, (size_t)room) : 0;
    if (listed < 0) {
        hero_fs_why_code = (int64_t)errno;
        step = HERO_STAGE_ATTRIBUTES;
        listed = 0;
    }
    for (ssize_t at = 0; at < listed && step == HERO_STAGE_DONE;) {
        const char *name = names + at;
        size_t name_length = strlen(name);
        at += (ssize_t)name_length + 1;
        ssize_t size = fgetxattr(from, name, NULL, 0);
        if (size < 0) {
            hero_fs_why_code = (int64_t)errno;
            step = HERO_STAGE_ATTRIBUTES;
            break;
        }
        char *value = hero_alloc(size > 0 ? (size_t)size : 1);
        ssize_t read_back = size > 0 ? fgetxattr(from, name, value, (size_t)size) : 0;
        if (read_back < 0 || fsetxattr(fd, name, value, (size_t)(read_back < 0 ? 0 : read_back), 0) != 0) {
            if (strncmp(name, "security.", 9) != 0) {
                hero_fs_why_code = (int64_t)errno;
                step = HERO_STAGE_ATTRIBUTES;
            }
        }
        hero_release(value);
    }
    if (names != NULL) hero_release(names);
    close(from);
    return step;
#else
    (void)fd;
    (void)like;
    return HERO_STAGE_DONE;
#endif
}

/* The file's own flags, given to the new file: measured 2026-09-30 on this
 * Mac, `chflags hidden` and `chflags nodump` both survived the in-place write
 * and both were lost by this part's first version, which carried everything
 * else. Carried are the flags an owner sets that say how the file is TREATED,
 * and no flag that says how its bytes are STORED, which is the filesystem's to
 * decide for a new file: Darwin's `compressed` given to a file written without
 * compression would read as garbage. Immutable and append-only are not among
 * them because such a file was refused before a byte was written
 * (`hero_fs_writable`: `open` for writing answers EPERM on both). A filesystem
 * that keeps no flags has none to carry; one that keeps them and refuses them
 * to the new file is a refusal like any other. */
#if defined(__APPLE__)
#define HERO_CARRIED_FLAGS (UF_NODUMP | UF_OPAQUE | UF_HIDDEN)

static int64_t hero_stage_flags(int fd, const struct stat *old, const struct stat *now) {
    if ((old->st_flags & HERO_CARRIED_FLAGS) == (now->st_flags & HERO_CARRIED_FLAGS)) return HERO_STAGE_DONE;
    if (fchflags(fd, (now->st_flags & ~(uint32_t)HERO_CARRIED_FLAGS) | (old->st_flags & HERO_CARRIED_FLAGS)) == 0) {
        return HERO_STAGE_DONE;
    }
    hero_fs_why_code = (int64_t)errno;
    return HERO_STAGE_FLAGS;
}
#elif defined(__linux__)
/* `chattr`'s d, A and S: no dump, no access time, synchronous updates. */
#define HERO_CARRIED_FLAGS (FS_NODUMP_FL | FS_NOATIME_FL | FS_SYNC_FL)

static int64_t hero_stage_flags(int fd, const char *like) {
    int from = open(like, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (from < 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_FLAGS;
    }
    int old = 0;
    int asked = ioctl(from, FS_IOC_GETFLAGS, &old);
    int why = errno;
    close(from);
    if (asked != 0) {
        if (why == ENOTTY || why == EOPNOTSUPP || why == EINVAL || why == ENOSYS) return HERO_STAGE_DONE;
        hero_fs_why_code = (int64_t)why;
        return HERO_STAGE_FLAGS;
    }
    int wanted = old & HERO_CARRIED_FLAGS;
    if (wanted == 0) return HERO_STAGE_DONE;
    int now = 0;
    if (ioctl(fd, FS_IOC_GETFLAGS, &now) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_FLAGS;
    }
    if ((now & HERO_CARRIED_FLAGS) == wanted) return HERO_STAGE_DONE;
    int given = (now & ~HERO_CARRIED_FLAGS) | wanted;
    if (ioctl(fd, FS_IOC_SETFLAGS, &given) == 0) return HERO_STAGE_DONE;
    hero_fs_why_code = (int64_t)errno;
    return HERO_STAGE_FLAGS;
}
#endif

/* Everything after the create, on the open file: the bytes, then who owns it
 * and what it permits, then its flags, its ACL and its extended attributes,
 * then the flush. Owner and group first, because a
 * `fchown` may clear the set-id bits the `fchmod` after it restores; and only
 * where they differ, because a user who is not a member of the group a new file
 * inherited cannot even set it to itself. */
static int64_t hero_stage_fill(int fd, HeroStr text, const char *like) {
    const char *bytes = hero_str_cstr(text);
    int64_t left = hero_str_len(text);
    while (left > 0) {
        size_t chunk = left > (int64_t)(1 << 30) ? (size_t)(1 << 30) : (size_t)left;
        ssize_t put = write(fd, bytes, chunk);
        if (put < 0) {
            if (errno == EINTR) continue;
            hero_fs_why_code = (int64_t)errno;
            return HERO_STAGE_WRITE;
        }
        bytes += put;
        left -= (int64_t)put;
    }
    if (like[0] != '\0') {
        struct stat old;
        struct stat now;
        if (stat(like, &old) != 0 || fstat(fd, &now) != 0) {
            hero_fs_why_code = (int64_t)errno;
            return HERO_STAGE_OWNER;
        }
        if ((now.st_uid != old.st_uid || now.st_gid != old.st_gid) && fchown(fd, old.st_uid, old.st_gid) != 0) {
            hero_fs_why_code = (int64_t)errno;
            return HERO_STAGE_OWNER;
        }
        if (fchmod(fd, old.st_mode & 07777) != 0) {
            hero_fs_why_code = (int64_t)errno;
            return HERO_STAGE_MODE;
        }
#if defined(__APPLE__)
        int64_t flagged = hero_stage_flags(fd, &old, &now);
#elif defined(__linux__)
        int64_t flagged = hero_stage_flags(fd, like);
#else
        int64_t flagged = HERO_STAGE_DONE;
#endif
        if (flagged != HERO_STAGE_DONE) return flagged;
        int64_t carried = hero_stage_attributes(fd, like);
        if (carried != HERO_STAGE_DONE) return carried;
    }
    if (hero_stage_flush(fd) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_FLUSH;
    }
    return HERO_STAGE_DONE;
}
#endif

/* `text`, written whole into a NEW file at `staged`, flushed to the device, and
 * given `like`'s owner, group, permission bits, flags, ACL and extended
 * attributes when `like` is not "": the author's file the new text is for. Answers
 * HERO_STAGE_DONE, or the step that failed with `hero_fs_why` saying why.
 *
 * THE NAME MUST NOT EXIST, and that is what makes the name safe to predict: a
 * file, or a link, somebody put at `staged` beforehand is never opened, written
 * or followed: the create answers EEXIST instead (O_EXCL, CREATE_NEW). And
 * because only this function knows whether it made the file, it is the one that
 * removes it when a later step fails: a caller cleaning up after a failed
 * create would remove what somebody else had put there. */
int64_t hero_file_stage(const char *staged, HeroStr text, const char *like) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    HANDLE h = CreateFileA(staged, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        hero_fs_why_code = (int64_t)GetLastError();
        return HERO_STAGE_CREATE;
    }
    int64_t step = HERO_STAGE_DONE;
    const char *bytes = hero_str_cstr(text);
    int64_t left = hero_str_len(text);
    while (left > 0 && step == HERO_STAGE_DONE) {
        DWORD chunk = left > (int64_t)(1 << 30) ? (DWORD)(1 << 30) : (DWORD)left;
        DWORD put = 0;
        if (!WriteFile(h, bytes, chunk, &put, NULL) || put == 0) {
            hero_fs_why_code = (int64_t)GetLastError();
            step = HERO_STAGE_WRITE;
            break;
        }
        bytes += put;
        left -= (int64_t)put;
    }
    if (step == HERO_STAGE_DONE && !FlushFileBuffers(h)) {
        hero_fs_why_code = (int64_t)GetLastError();
        step = HERO_STAGE_FLUSH;
    }
    CloseHandle(h);
    if (step == HERO_STAGE_DONE && like[0] != '\0') {
        /* The attributes a file carries besides its bytes, the ones a caller
         * may set. Read-only is not among them: a read-only file was refused
         * before anything was written. */
        DWORD attrs = GetFileAttributesA(like);
        DWORD kept = attrs == INVALID_FILE_ATTRIBUTES ? 0
                     : attrs & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE |
                                FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
        if (kept != 0 && !SetFileAttributesA(staged, kept)) {
            hero_fs_why_code = (int64_t)GetLastError();
            step = HERO_STAGE_MODE;
        }
    }
    if (step != HERO_STAGE_DONE) {
        int64_t why = hero_fs_why_code;
        DeleteFileA(staged);
        hero_fs_why_code = why;
    }
    return step;
#else
    int fd = open(staged, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0666);
    if (fd < 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_STAGE_CREATE;
    }
    int64_t step = hero_stage_fill(fd, text, like);
    /* A full disk is often reported at the close and not at the write, where
     * the filesystem held the bytes back (the same note as `hero_file_write`). */
    if (close(fd) != 0 && step == HERO_STAGE_DONE) {
        hero_fs_why_code = (int64_t)errno;
        step = HERO_STAGE_WRITE;
    }
    if (step != HERO_STAGE_DONE) {
        int64_t why = hero_fs_why_code;
        unlink(staged);
        hero_fs_why_code = why;
    }
    return step;
#endif
}

#if !defined(_WIN32)
/* The directory holding `path`, flushed, so the rename in it outlives a power
 * cut too. Best effort, and deliberately so: the rename has already happened
 * when this runs, and a filesystem that cannot flush a directory must not turn a
 * file that WAS replaced into a report that it was not. */
static void hero_fs_sync_directory_of(const char *path) {
    char directory[HERO_FS_PATH_MAX];
    size_t length = strlen(path);
    if (length >= sizeof directory) return;
    memcpy(directory, path, length + 1);
    char *slash = strrchr(directory, '/');
    if (slash == NULL) {
        directory[0] = '.';
        directory[1] = '\0';
    } else if (slash == directory) {
        directory[1] = '\0';
    } else {
        *slash = '\0';
    }
    int fd = open(directory, O_RDONLY | O_CLOEXEC);
    if (fd < 0) return;
    (void)fsync(fd);
    close(fd);
}
#endif

/* Put `staged` in place of `path`: one atomic rename on POSIX, and on Windows
 * `MoveFileEx` with the replacement and the write-through the Win32
 * documentation gives it (`parts/fs.c`'s `hero_fs_rename` carries the caveat
 * that arm lives with). */
int64_t hero_fs_replace(const char *staged, const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (MoveFileExA(staged, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (rename(staged, path) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    hero_fs_sync_directory_of(path);
    return HERO_OS_OK;
#endif
}

/* The staged file removed, after `hero_fs_replace` could not put it in place;
 * a file that is not there is removed already. Only the caller that staged it
 * calls this, which is what makes removing it safe (`hero_file_stage`'s THE
 * NAME MUST NOT EXIST).
 *
 * A PLAIN `unlink` IS NOT ENOUGH ON DARWIN, measured 2026-09-30. A file whose
 * ACL denies deleting it, `everyone deny delete`, cannot be renamed over
 * (EACCES), and the staged copy carries that same ACL, so its own `unlink`
 * failed too and the copy stayed beside the author's file. The owner may
 * always clear an ACL (`chmod -N`, then `rm`, measured, `writesecurity` denied
 * as well), so where the removal is refused the ACL is cleared and it is asked
 * again. Linux decides a removal by the directory alone, and the Windows arm
 * carries no ACL. */
int64_t hero_file_unstage(const char *staged) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (DeleteFileA(staged) || GetLastError() == ERROR_FILE_NOT_FOUND) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (unlink(staged) == 0 || errno == ENOENT) return HERO_OS_OK;
    int why = errno;
#if defined(__APPLE__)
    if (why == EACCES || why == EPERM) {
        acl_t none = acl_init(0);
        if (none != NULL) {
            int cleared = acl_set_file(staged, ACL_TYPE_EXTENDED, none);
            acl_free(none);
            if (cleared == 0 && unlink(staged) == 0) return HERO_OS_OK;
            if (cleared == 0) why = errno;
        }
    }
#endif
    hero_fs_why_code = (int64_t)why;
    return HERO_OS_FAILED;
#endif
}

/* ---- The four the cases build their files with -----------------------------
 *
 * A read-only file, a file with a mode of its own, a second name, a symbolic
 * link: the shapes the rules above answer, which the compiler's own tests and
 * the net must be able to MAKE on every platform, and which the language cannot
 * make (`chmod` is `sys/stat.h`, whose `mode_t` has two widths, and panel 097's
 * reason keeps the arm here). */

/* The permission bits of the file at `path`, following a link; -1 where there
 * is none. Windows keeps one of them, read-only: 0444 or 0666 there. */
int64_t hero_fs_mode(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        hero_fs_why_code = (int64_t)GetLastError();
        return -1;
    }
    return (attrs & FILE_ATTRIBUTE_READONLY) != 0 ? 0444 : 0666;
#else
    struct stat info;
    if (stat(path, &info) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return -1;
    }
    return (int64_t)(info.st_mode & 07777);
#endif
}

/* `chmod`. On Windows the read-only attribute, set where `mode` grants nobody
 * writing and cleared where it grants somebody. */
int64_t hero_fs_set_mode(const char *path, int64_t mode) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        hero_fs_why_code = (int64_t)GetLastError();
        return HERO_OS_FAILED;
    }
    DWORD wanted = (mode & 0222) == 0 ? (attrs | FILE_ATTRIBUTE_READONLY) : (attrs & ~(DWORD)FILE_ATTRIBUTE_READONLY);
    if (wanted == 0) wanted = FILE_ATTRIBUTE_NORMAL;
    if (SetFileAttributesA(path, wanted)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (chmod(path, (mode_t)(mode & 07777)) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    return HERO_OS_FAILED;
#endif
}

/* A second name, `name`, for the file at `existing`: `link`, `CreateHardLinkA`. */
int64_t hero_fs_link(const char *existing, const char *name) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (CreateHardLinkA(name, existing, NULL)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (link(existing, name) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    return HERO_OS_FAILED;
#endif
}

/* A symbolic link at `name` that reads `target`. Windows grants the call only
 * to an administrator, or to anybody in developer mode where the flag below is
 * passed; its refusal answers like any other. */
#if defined(_WIN32) && !defined(SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)
#define SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE 0x2
#endif
int64_t hero_fs_symlink(const char *target, const char *name) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (CreateSymbolicLinkA(name, target, SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#else
    if (symlink(target, name) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    return HERO_OS_FAILED;
#endif
}

/* An ACL entry denying this process's own user the deletion of `path`: the
 * shape whose staged copy `hero_file_unstage` must still remove. Darwin alone
 * has one a rewrite carries; elsewhere HERO_OS_UNSUPPORTED. */
int64_t hero_fs_deny_delete(const char *path) {
    hero_fs_why_code = 0;
#if defined(__APPLE__)
    uuid_t me;
    if (mbr_uid_to_uuid(getuid(), me) != 0) return HERO_OS_FAILED;
    acl_t acl = acl_get_file(path, ACL_TYPE_EXTENDED);
    if (acl == NULL) acl = acl_init(1);
    if (acl == NULL) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    acl_entry_t entry;
    acl_permset_t permits;
    int made = acl_create_entry(&acl, &entry) == 0 && acl_set_tag_type(entry, ACL_EXTENDED_DENY) == 0 &&
               acl_set_qualifier(entry, me) == 0 && acl_get_permset(entry, &permits) == 0 &&
               acl_clear_perms(permits) == 0 && acl_add_perm(permits, ACL_DELETE) == 0 &&
               acl_set_permset(entry, permits) == 0 && acl_set_file(path, ACL_TYPE_EXTENDED, acl) == 0;
    if (!made) hero_fs_why_code = (int64_t)errno;
    acl_free(acl);
    return made ? HERO_OS_OK : HERO_OS_FAILED;
#else
    (void)path;
    return HERO_OS_UNSUPPORTED;
#endif
}

/* The one flag each platform keeps on a file besides its mode that a case can
 * set and read back: `nodump` on Darwin and Linux, `hidden` on Windows.
 * `hero_fs_flagged` answers 1 or 0, or -1 where the filesystem keeps no flags;
 * `hero_fs_set_flag` answers HERO_OS_UNSUPPORTED there. */
int64_t hero_fs_flagged(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        hero_fs_why_code = (int64_t)GetLastError();
        return -1;
    }
    return (attrs & FILE_ATTRIBUTE_HIDDEN) != 0 ? 1 : 0;
#elif defined(__APPLE__)
    struct stat info;
    if (stat(path, &info) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return -1;
    }
    return (info.st_flags & UF_NODUMP) != 0 ? 1 : 0;
#elif defined(__linux__)
    int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        hero_fs_why_code = (int64_t)errno;
        return -1;
    }
    int flags = 0;
    int asked = ioctl(fd, FS_IOC_GETFLAGS, &flags);
    if (asked != 0) hero_fs_why_code = (int64_t)errno;
    close(fd);
    return asked != 0 ? -1 : (flags & FS_NODUMP_FL) != 0 ? 1 : 0;
#else
    (void)path;
    return -1;
#endif
}

int64_t hero_fs_set_flag(const char *path) {
    hero_fs_why_code = 0;
#if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path);
    if (attrs != INVALID_FILE_ATTRIBUTES && SetFileAttributesA(path, attrs | FILE_ATTRIBUTE_HIDDEN)) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)GetLastError();
    return HERO_OS_FAILED;
#elif defined(__APPLE__)
    struct stat info;
    if (stat(path, &info) == 0 && chflags(path, info.st_flags | UF_NODUMP) == 0) return HERO_OS_OK;
    hero_fs_why_code = (int64_t)errno;
    return HERO_OS_FAILED;
#elif defined(__linux__)
    int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    int flags = 0;
    int64_t answer = HERO_OS_OK;
    if (ioctl(fd, FS_IOC_GETFLAGS, &flags) != 0) {
        answer = errno == ENOTTY || errno == EOPNOTSUPP || errno == EINVAL ? HERO_OS_UNSUPPORTED : HERO_OS_FAILED;
        hero_fs_why_code = (int64_t)errno;
    } else {
        flags |= FS_NODUMP_FL;
        if (ioctl(fd, FS_IOC_SETFLAGS, &flags) != 0) {
            answer = errno == ENOTTY || errno == EOPNOTSUPP || errno == EINVAL ? HERO_OS_UNSUPPORTED : HERO_OS_FAILED;
            hero_fs_why_code = (int64_t)errno;
        }
    }
    close(fd);
    return answer;
#else
    (void)path;
    return HERO_OS_UNSUPPORTED;
#endif
}
