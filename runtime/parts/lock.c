/* parts/lock.c: a lock one run of the compiler holds beside a folder it works
 * in (defect 2453; panel 246's R1, ratified 2026-11-12; design.md §1.12).
 *
 * WHAT IT ANSWERS THAT A PROCESS NUMBER CANNOT: whether the run that made a
 * folder is still running. `heroes add`, `heroes fetch` and every build that
 * asks git where the program stands work in folders of their own,
 * `heroes.packages/.fetching-<pid>-<n>/` (`selfhost/fetch/staging.hero`). A
 * run killed with `kill -9` leaves its folder, and the number in its name may
 * since name another process, the system giving a dead run's number to a
 * later one: measured by the sitting, a later run of that number removed the
 * dead run's folder and the only copy of the author's edit in it, at exit 0.
 * A lock the system releases when its holder dies, however it dies, says what
 * the number cannot: a later run that takes a folder's lock knows its run is
 * dead (`selfhost/fetch/leftover.hero`), and one that cannot leaves it.
 *
 * THE FILE STANDS BESIDE THE FOLDER, `<folder>.lock`, taken before the folder
 * is made and given back after it is removed: the one layout the sitting
 * measured with no live run's folder lost, no birth refused and no dead folder
 * left (the ffi-pragmatist's `proto.c`, 13,191 runs on this Mac, and Linux
 * arm64). A lock inside the folder leaves a window between `mkdir` and the
 * lock, in which a recovering run takes a live folder or leaves a dead one.
 *
 * POSIX: `flock(LOCK_EX | LOCK_NB)` on a descriptor opened `O_CLOEXEC`, so a
 * program this process starts holds it only between its `fork` and its
 * `exec`. Never `fcntl`'s record locks: they are the process's, so a second
 * open of the file in the same process takes the lock again, and closing any
 * descriptor of the file loses it (measured by the same seat on APFS, FAT16,
 * FAT32 and exFAT, where `flock` held in both).
 *
 * WINDOWS: `LockFileEx` over the file's first byte, exclusive and failing at
 * once, on a handle no child inherits (its security attributes NULL), which
 * the system releases when the handle closes, a process ended by
 * `TerminateProcess` included. The file stays, as it does on POSIX, so on
 * every platform a folder a dead run left has its lock file beside it, and a
 * folder with none is one no lock proves dead, which a compiler older than
 * this file made and which is never undone. The seat's other arm, a share-0
 * open the system deletes on close, leaves a dead run's folder with no lock
 * file: the same as a folder an older compiler left, which may hold an edit
 * no ledger names. Microsoft's page for `LockFileEx` says a dead process's
 * locks are released after a time that "depends upon available system
 * resources": until then a later run reads the lock as held and leaves the
 * folder to the next, the safe side.
 *
 * THE IDENTITY CHECK: once locked, the file the descriptor holds must be the
 * one the path still names (device and inode on POSIX, volume and index on
 * Windows). A recovering run that takes a lock file in the instant between
 * its owner's open and its lock, finds no folder beside it and removes it,
 * leaves the owner locking a file no name reaches: the owner then reads the
 * name as another's and takes the next. Without the check this Mac lost a live
 * run's folder 65 to 76 times in 8 s of the seat's load; Linux arm64 lost none,
 * and the check costs a `stat`.
 *
 * A FILE SYSTEM THAT KEEPS NO LOCK, `flock` or `LockFileEx` refused outright:
 * the take answers HERO_OS_UNSUPPORTED, a file it made removed again, and the
 * run works with no lock file, so its folder is never recovered, as before
 * this file existed (the lockless fallback panel 246's R1 owed).
 *
 * EVERY TAKE HANDS BACK A BLOCK, whatever its status, which the caller gives
 * back through `hero_lock_drop` once: a group declares it as a handle whose
 * life its marks track, so a run that forgets one is told when it ends.
 * Nothing is kept between calls but the block the caller holds, so the
 * runtime suite's Rule 3 has no object here to judge. */

#if defined(_WIN32)
struct hero_lock {
    /* INVALID_HANDLE_VALUE where the take holds no lock. */
    HANDLE handle;
    /* The path as the take was given it, for the removal at the drop. */
    char *path;
};
#else
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

struct hero_lock {
    /* -1 where the take holds no lock. */
    int fd;
    /* The path as the take was given it, for the removal at the drop. */
    char *path;
};
#endif

/* A block holding no lock yet, its path copied. Through the single point's
 * own `malloc` (`parts/alloc.c`), weighed by neither counter: the handle marks
 * count its life, and a block outlives the call that made it. */
static hero_lock *hero_lock_block(const char *path) {
    size_t n = strlen(path);
    hero_lock *lock = hero_malloc_raw(sizeof *lock);
#if defined(_WIN32)
    lock->handle = INVALID_HANDLE_VALUE;
#else
    lock->fd = -1;
#endif
    lock->path = hero_malloc_raw(n + 1);
    memcpy(lock->path, path, n + 1);
    return lock;
}

#if defined(_WIN32)
/* Why `CreateFileW` refused the lock file. ACCESS_DENIED is two things: a
 * file of that name its holder removed while holding it, which waits for its
 * last handle to close (a live run's, so held), or a folder this user may
 * not write, where nothing stands: the file's attributes tell them apart. */
static int64_t hero_lock_unopened(const wchar_t *wide, DWORD failure) {
    if (failure == ERROR_FILE_EXISTS || failure == ERROR_ALREADY_EXISTS || failure == ERROR_SHARING_VIOLATION) {
        return HERO_LOCK_HELD;
    }
    if (failure == ERROR_FILE_NOT_FOUND || failure == ERROR_PATH_NOT_FOUND) return HERO_OS_NOT_FOUND;
    if (failure == ERROR_ACCESS_DENIED) {
        DWORD attributes = GetFileAttributesW(wide);
        DWORD asked = attributes == INVALID_FILE_ATTRIBUTES ? GetLastError() : ERROR_SUCCESS;
        if (asked != ERROR_FILE_NOT_FOUND && asked != ERROR_PATH_NOT_FOUND) return HERO_LOCK_HELD;
    }
    hero_fs_why_code = (int64_t)failure;
    return HERO_OS_FAILED;
}

/* Whether the handle `held` locks the file the path `wide` names: a plain file
 * (no link, no folder) with the same volume and the same index. */
static int hero_lock_same(HANDLE held, const wchar_t *wide) {
    BY_HANDLE_FILE_INFORMATION mine;
    BY_HANDLE_FILE_INFORMATION named;
    if (!GetFileInformationByHandle(held, &mine)) return 0;
    if ((mine.dwFileAttributes & (FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_DIRECTORY)) != 0) return 0;
    HANDLE again = CreateFileW(wide, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
                               OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, NULL);
    if (again == INVALID_HANDLE_VALUE) return 0;
    BOOL known = GetFileInformationByHandle(again, &named);
    (void)CloseHandle(again);
    return known && mine.dwVolumeSerialNumber == named.dwVolumeSerialNumber &&
           mine.nFileIndexHigh == named.nFileIndexHigh && mine.nFileIndexLow == named.nFileIndexLow;
}
#endif

hero_lock *hero_lock_take(const char *path, int64_t fresh, int64_t *status) {
    hero_lock *lock = hero_lock_block(path);
    hero_fs_why_code = 0;
#if defined(_WIN32)
    wchar_t *wide = hero_win_wide(path, NULL);
    if (wide == NULL) {
        hero_fs_why_code = -(int64_t)ERROR_NO_UNICODE_TRANSLATION;
        *status = HERO_OS_FAILED;
        return lock;
    }
    /* Shared for reading, writing and deleting: the lock is the byte-range
     * lock below, never the open, so a recovering run may open a live run's
     * file to ask it, and a holder may remove the file while it holds it. The
     * file itself, never a link at its name (`O_NOFOLLOW`'s part). */
    HANDLE h = CreateFileW(wide, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           NULL, fresh != 0 ? CREATE_NEW : OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        *status = hero_lock_unopened(wide, GetLastError());
        hero_release(wide);
        return lock;
    }
    OVERLAPPED at;
    memset(&at, 0, sizeof at);
    if (!LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 1, 0, &at)) {
        DWORD failure = GetLastError();
        (void)CloseHandle(h);
        if (failure == ERROR_LOCK_VIOLATION || failure == ERROR_IO_PENDING) {
            *status = HERO_LOCK_HELD;
        } else if (failure == ERROR_NOT_SUPPORTED || failure == ERROR_INVALID_FUNCTION) {
            /* No lock on this volume: the file a fresh take made holds none. */
            if (fresh != 0) (void)DeleteFileW(wide);
            *status = HERO_OS_UNSUPPORTED;
        } else {
            hero_fs_why_code = (int64_t)failure;
            *status = HERO_OS_FAILED;
        }
        hero_release(wide);
        return lock;
    }
    if (!hero_lock_same(h, wide)) {
        (void)UnlockFileEx(h, 0, 1, 0, &at);
        (void)CloseHandle(h);
        hero_release(wide);
        *status = HERO_LOCK_HELD;
        return lock;
    }
    hero_release(wide);
    lock->handle = h;
    *status = HERO_OS_OK;
    return lock;
#else
    int fd;
    do {
        fd = open(path, O_RDWR | O_CLOEXEC | O_NOFOLLOW | (fresh != 0 ? O_CREAT | O_EXCL : 0), 0600);
    } while (fd < 0 && errno == EINTR);
    if (fd < 0) {
        int failure = errno;
        if (failure == EEXIST) {
            *status = HERO_LOCK_HELD;
        } else if (failure == ENOENT || failure == ENOTDIR) {
            *status = HERO_OS_NOT_FOUND;
        } else {
            hero_fs_why_code = (int64_t)failure;
            *status = HERO_OS_FAILED;
        }
        return lock;
    }
    int locked;
    do {
        locked = flock(fd, LOCK_EX | LOCK_NB);
    } while (locked != 0 && errno == EINTR);
    if (locked != 0) {
        int failure = errno;
        if (failure == EWOULDBLOCK || failure == EAGAIN) {
            /* Held, a fresh file among them: a recovering run took it in the
             * instant before this lock and removes it itself. */
            *status = HERO_LOCK_HELD;
        } else if (failure == ENOTSUP || failure == EOPNOTSUPP || failure == ENOLCK || failure == EINVAL) {
            /* No lock on this file system: the file a fresh take made holds
             * none, and no run can take it to prove this one dead. */
            if (fresh != 0) (void)unlink(path);
            *status = HERO_OS_UNSUPPORTED;
        } else {
            if (fresh != 0) (void)unlink(path);
            hero_fs_why_code = (int64_t)failure;
            *status = HERO_OS_FAILED;
        }
        (void)close(fd);
        return lock;
    }
    /* The file locked must be the one the path still names, the name itself
     * and never a link put there since. */
    struct stat held;
    struct stat named;
    if (fstat(fd, &held) != 0 || lstat(path, &named) != 0 || held.st_dev != named.st_dev ||
        held.st_ino != named.st_ino) {
        (void)flock(fd, LOCK_UN);
        (void)close(fd);
        *status = HERO_LOCK_HELD;
        return lock;
    }
    lock->fd = fd;
    *status = HERO_OS_OK;
    return lock;
#endif
}

int64_t hero_lock_drop(hero_lock *lock, int64_t keep) {
    int64_t answer = HERO_OS_OK;
    hero_fs_why_code = 0;
#if defined(_WIN32)
    if (lock->handle != INVALID_HANDLE_VALUE) {
        if (keep == 0) {
            /* Removed while still held: the name goes, or waits marked until
             * this handle closes, which a later open reads as held. */
            wchar_t *wide = hero_win_wide(lock->path, NULL);
            if (wide == NULL) {
                hero_fs_why_code = -(int64_t)ERROR_NO_UNICODE_TRANSLATION;
                answer = HERO_OS_FAILED;
            } else {
                if (!DeleteFileW(wide) && GetLastError() != ERROR_FILE_NOT_FOUND) {
                    hero_fs_why_code = (int64_t)GetLastError();
                    answer = HERO_OS_FAILED;
                }
                hero_release(wide);
            }
        }
        /* Unlocked first, as Microsoft's page asks, so the next run can take
         * a kept file at once; then closed. */
        OVERLAPPED at;
        memset(&at, 0, sizeof at);
        (void)UnlockFileEx(lock->handle, 0, 1, 0, &at);
        if (!CloseHandle(lock->handle)) {
            hero_fs_why_code = (int64_t)GetLastError();
            answer = HERO_OS_FAILED;
        }
    }
#else
    if (lock->fd >= 0) {
        /* Removed while still held, so a run that opens the name after this
         * makes a new file and never shares this one. */
        if (keep == 0 && unlink(lock->path) != 0 && errno != ENOENT) {
            hero_fs_why_code = (int64_t)errno;
            answer = HERO_OS_FAILED;
        }
        /* Unlocked before the close, so a child between its `fork` and its
         * `exec`, which shares the descriptor, keeps no lock. */
        (void)flock(lock->fd, LOCK_UN);
        if (close(lock->fd) != 0) {
            hero_fs_why_code = (int64_t)errno;
            answer = HERO_OS_FAILED;
        }
    }
#endif
    hero_free_raw(lock->path);
    hero_free_raw(lock);
    return answer;
}
