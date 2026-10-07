/* parts/write.c: the program's `write_file`, whole or not at all (design.md
 * §1.12; defect 438).
 *
 * WHY THIS EXISTS. `write_file` was `fopen(path, "wb")` and then the bytes,
 * so the file was emptied before the first byte of the new text reached it: a
 * reader beside it could find it empty or cut, and a process killed in between
 * left it so, the old text gone. Measured 2026-10-07 on this Mac at `dad2da47`:
 * a child under a 4096-byte ceiling on what it may write (RLIMIT_FSIZE through
 * `hero_run_limit_writes`, the kernel's nearest thing to a full disk) wrote
 * 1 MB over "OLD" and left 4096 bytes of the new text, the old lost; and a
 * 64 MB write killed with SIGKILL the instant the file stopped being "OLD" left
 * it at 0 bytes in 2 runs of 6.
 *
 * WHAT IT DOES NOW. The new text is written under a private name beside the
 * file, flushed to the device, and renamed over it: `parts/replace.c`'s
 * `hero_file_stage` and `hero_fs_replace`, the route the compiler has taken
 * for the author's own files since defect 136. The name holds the old text or
 * the new, never a part of either; a write that fails half-way answers
 * HERO_OS_FAILED with the file as it was and no private name left; a process
 * killed before the rename leaves the file as it was and its private name
 * beside it. A name that is not there yet is made the same way, so a reader
 * never sees it half-written either.
 *
 * WHERE A RENAME CANNOT, TODAY'S WRITE. A rename puts a NEW file at the name,
 * and a file is more than its bytes. So wherever the new file could not be
 * everything the old one was, the write goes in place exactly as before
 * (`hero_file_write_in_place`), and so does every refusal it made:
 *
 *   - what is not a regular file: a device, a FIFO, `/dev/stdout` (a measured
 *     use, design.md's wart about `print`), and a directory, refused as before;
 *   - a file with a second name, which would go on holding the old text;
 *   - a file this process could not open for writing (`hero_fs_writable`),
 *     which `fopen` refuses as before, rather than a rename replacing what its
 *     owner protected: a rename asks only the directory;
 *   - a file whose owner, group, mode, flags, ACL or extended attributes the
 *     new file could not be given (the stage's own steps); a directory that
 *     takes no new name (no write permission there, a private name past what a
 *     path may hold); a rename refused (a sticky directory, a Darwin ACL that
 *     denies the file's deletion);
 *   - on Windows, any file already there. The stage there carries the
 *     attributes and not the owner, the explicit ACL entries or the named
 *     streams (a downloaded file's mark of the web is one), and no call this
 *     runtime links can ask a file whether it has them, so only a name that is
 *     not taken yet goes through a private name there.
 *
 * A symbolic link is followed to the file its chain names (`hero_fs_landing`),
 * and THAT file is replaced, the link staying a link; the kernel's own answer
 * for the path must be that very file (device and inode), or the write goes in
 * place, which is what keeps a link the kernel resolves itself, `/dev/stdout`
 * through `/proc` on Linux, out of a reading by text. A link whose chain names
 * nothing yet makes that name whole.
 *
 * WHAT A FAILED STAGE DECIDES. A failure to write the private copy or to flush
 * it (a full device, EFBIG past a ceiling, an I/O error), or a create refused
 * for want of room, is the write's failure: the in-place write would meet the
 * same wall after emptying the file. Every other refusal is a shape a rename
 * cannot serve, and the write goes in place.
 *
 * THE PRICE. The new text needs room beside the old until the rename, so a
 * device with room for the new text and not for both refuses where the
 * in-place write fitted; and a write costs a stage, a flush of the file and of
 * its directory, and a rename, where it was an open, a write and a close
 * (lane b14-runtime's counts are in defect 438's issue).
 *
 * The runtime still does nothing to a path (`parts/os.c`): the private name is
 * the path the program gave, its last part cut at a character to
 * HERO_WRITE_NAME_KEPT bytes, with this process's id and a number no earlier
 * call has had after it, and the rename's target is the path itself, or the
 * file its links name. */

/* How a write reaches its file. */
#define HERO_WRITE_IN_PLACE 0
#define HERO_WRITE_CREATE 1
#define HERO_WRITE_REPLACE 2

/* How many private names one write asks for before it writes in place: a name
 * is taken only by a file a killed process left with this process's id. */
#define HERO_WRITE_TRIES 4

/* How much of the last part of a path its private name keeps: 200 bytes, and
 * the suffix is at most 44 (two numbers of at most 19 digits, `.`, `-`,
 * `.tmp`), so a part is never past 244, under the 255 APFS, ext4 and NTFS
 * allow; `selfhost/cli/publish.hero`'s NAME_KEPT, for the same reason. */
#define HERO_WRITE_NAME_KEPT 200

/* The write as it was until defect 438, kept whole for the shapes a rename
 * cannot serve. */
static int64_t hero_file_write_in_place(const char *path, HeroStr text) {
    hero_fs_why_code = 0;
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    int64_t len = hero_str_len(text);
    if (len > 0) {
        size_t put = fwrite(hero_str_cstr(text), 1, (size_t)len, file);
        if (put != (size_t)len) {
            hero_fs_why_code = (int64_t)errno;
            fclose(file);
            return HERO_OS_FAILED;
        }
    }
    /* A full disk is often reported here and not at `fwrite`: the bytes sat in
     * the stream's buffer until the close tried to write them. */
    if (fclose(file) != 0) {
        hero_fs_why_code = (int64_t)errno;
        return HERO_OS_FAILED;
    }
    return HERO_OS_OK;
}

/* `path` into `out`, HERO_FS_PATH_MAX bytes; 0 where it does not fit. */
static int hero_write_copy(const char *path, char *out) {
    size_t length = strlen(path);
    if (length == 0 || length >= HERO_FS_PATH_MAX) return 0;
    memcpy(out, path, length + 1);
    return 1;
}

/* Where `path`'s write lands, into `landing`, and the file there followed
 * through a link: the landing's own name copied, 0 where it cannot be. */
static int hero_write_landing(const char *path, char *landing) {
    HeroStr found = hero_fs_landing(path);
    int copied = found.len > 0 && hero_write_copy(found.ptr, landing);
    hero_str_decref(found);
    return copied;
}

#if defined(_WIN32)
static int hero_write_route(const char *path, char *landing) {
    int64_t kind = hero_fs_kind(path);
    if (kind == HERO_FS_ABSENT) return hero_write_copy(path, landing) ? HERO_WRITE_CREATE : HERO_WRITE_IN_PLACE;
    if (kind != HERO_FS_LINK || !hero_write_landing(path, landing)) return HERO_WRITE_IN_PLACE;
    return hero_fs_kind(landing) == HERO_FS_ABSENT ? HERO_WRITE_CREATE : HERO_WRITE_IN_PLACE;
}
#else
static int hero_write_route(const char *path, char *landing) {
    struct stat here;
    if (lstat(path, &here) != 0) {
        if (errno != ENOENT) return HERO_WRITE_IN_PLACE;
        return hero_write_copy(path, landing) ? HERO_WRITE_CREATE : HERO_WRITE_IN_PLACE;
    }
    struct stat file = here;
    if (S_ISLNK(here.st_mode)) {
        if (!hero_write_landing(path, landing)) return HERO_WRITE_IN_PLACE;
        struct stat through;
        if (stat(path, &through) != 0) {
            /* The chain names nothing yet, the one case a link is written
             * through to a name that is not there. */
            if (errno != ENOENT) return HERO_WRITE_IN_PLACE;
            struct stat at;
            if (lstat(landing, &at) == 0 || errno != ENOENT) return HERO_WRITE_IN_PLACE;
            return HERO_WRITE_CREATE;
        }
        if (lstat(landing, &file) != 0 || file.st_dev != through.st_dev || file.st_ino != through.st_ino) {
            return HERO_WRITE_IN_PLACE;
        }
    } else if (!hero_write_copy(path, landing)) {
        return HERO_WRITE_IN_PLACE;
    }
    if (!S_ISREG(file.st_mode) || file.st_nlink != 1) return HERO_WRITE_IN_PLACE;
    return hero_fs_writable(landing) ? HERO_WRITE_REPLACE : HERO_WRITE_IN_PLACE;
}
#endif

/* A private name beside `at` into `out`: 0 where it would not fit. */
static int hero_write_private_name(const char *at, char *out) {
    size_t length = strlen(at);
    size_t cut = length;
#if defined(_WIN32)
    while (cut > 0 && at[cut - 1] != '/' && at[cut - 1] != '\\') cut -= 1;
#else
    while (cut > 0 && at[cut - 1] != '/') cut -= 1;
#endif
    size_t end = length;
    if (length - cut > HERO_WRITE_NAME_KEPT) {
        end = cut + HERO_WRITE_NAME_KEPT;
        /* Back to the first byte of a character: a UTF-8 continuation byte is
         * 0x80 to 0xBF, and a name may not end inside one. */
        while (end > cut && ((unsigned char)at[end] & 0xC0) == 0x80) end -= 1;
    }
    char suffix[64];
    int n = snprintf(suffix, sizeof suffix, ".%lld-%lld.tmp", (long long)hero_os_pid(), (long long)hero_run_serial());
    if (n < 0 || (size_t)n >= sizeof suffix || end + (size_t)n >= HERO_FS_PATH_MAX) return 0;
    memcpy(out, at, end);
    memcpy(out + end, suffix, (size_t)n + 1);
    return 1;
}

/* The stage's reason for a create it refused, where the reason is a name
 * somebody else holds. */
static int hero_write_name_taken(int64_t why) {
#if defined(_WIN32)
    return why == ERROR_FILE_EXISTS || why == ERROR_ALREADY_EXISTS;
#else
    return why == EEXIST;
#endif
}

/* A reason that the device itself refused: no room, no quota, an I/O error.
 * The in-place write would meet it too, after emptying the file. */
static int hero_write_device_refused(int64_t why) {
#if defined(_WIN32)
    return why == ERROR_DISK_FULL || why == ERROR_HANDLE_DISK_FULL;
#else
    return why == ENOSPC || why == EDQUOT || why == EIO;
#endif
}

int64_t hero_file_write(const char *path, HeroStr text) {
    hero_fs_why_code = 0;
    char landing[HERO_FS_PATH_MAX];
    int route = hero_write_route(path, landing);
    if (route == HERO_WRITE_IN_PLACE) return hero_file_write_in_place(path, text);

    /* A file already there gives the new one everything it is besides its
     * bytes; a name not taken yet is made as `fopen` makes one. */
    const char *like = route == HERO_WRITE_REPLACE ? landing : "";
    char staged[HERO_FS_PATH_MAX];
    int64_t step = HERO_STAGE_CREATE;
    for (int tries = 0; tries < HERO_WRITE_TRIES; tries += 1) {
        if (!hero_write_private_name(landing, staged)) return hero_file_write_in_place(path, text);
        step = hero_file_stage(staged, text, like);
        if (step != HERO_STAGE_CREATE || !hero_write_name_taken(hero_fs_why_code)) break;
    }

    if (step == HERO_STAGE_DONE) {
        if (hero_fs_replace(staged, landing) == HERO_OS_OK) return HERO_OS_OK;
        /* Refused, and the file is as it was: the copy goes, and the write is
         * made in place as a rename cannot make it. */
        (void)hero_file_unstage(staged);
        return hero_file_write_in_place(path, text);
    }
    /* The stage removed what it made. A copy that could not be written whole
     * or flushed, or a create the device refused, is the write's failure, its
     * reason kept; anything else is a shape a rename cannot serve. */
    if (step == HERO_STAGE_WRITE || step == HERO_STAGE_FLUSH || hero_write_device_refused(hero_fs_why_code)) {
        return HERO_OS_FAILED;
    }
    return hero_file_write_in_place(path, text);
}

/* The program's door (panel 192): a name holding a NUL names no file, and C
 * would read it only to the NUL, so it is answered and never opened. */
int64_t hero_file_write_str(HeroStr path, HeroStr text) {
    if (hero_name_holds_nul(path)) {
        hero_fs_why_code = (int64_t)EINVAL;
        return HERO_OS_BAD_NAME;
    }
    return hero_file_write(path.ptr, text);
}
