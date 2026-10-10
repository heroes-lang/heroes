/* parts/dir.c — listing a directory, and removing a tree.
 *
 * WHY THESE EXIST. The test harness reached the filesystem the way the compiler
 * used to: `find dir -maxdepth 1 -type f -print0` and `rm -rf` through a shell.
 * Both are POSIX utilities, so both die on `cmd.exe`, and panel 097's condition
 * 7 says the harness ships in the same milestone as the compiler or the
 * milestone does not claim Windows. These two calls are what that costs.
 *
 * THE SHAPE IS THE ARGUMENT BUILDER'S, FOR THE SAME REASON. A `[str]` cannot
 * cross the FFI boundary, so a directory listing cannot be returned as one:
 * `hero_dir_scan` fills a list held here and answers how many names it found,
 * and `hero_dir_at` hands them back one at a time. `parts/run.c` explains the
 * measurement behind that; this file only follows it.
 *
 * The names are RELATIVE to the directory scanned, and `.` and `..` are never
 * among them. A recursive scan yields paths joined with `/`, which Windows
 * accepts in every filesystem call (`parts/os.c:104`), so no caller has to know
 * which separator it is standing on — panel 057 refused `\` as a separator
 * everywhere and this does not reopen it.
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

/* The listing being built. It GROWS, and the first version of this file did not
 * — it held a fixed 8192 names, with a comment borrowed from `parts/run.c`
 * explaining that the bound was an order of magnitude above what was needed.
 * That reasoning is true about an argument list, which has tens of entries, and
 * false about a directory: this project's own `build/` holds **37,240** files,
 * measured, so the scan filled up and answered "I could not read it" — and three
 * suites failed with a message about a program that had emitted nothing.
 *
 * A premise that was true where it was written and false where it was copied,
 * which is CLAUDE.md §11's exact shape. There is no right number to guess for a
 * directory, so there is no number. */
static _Thread_local char **hero_dir_names = NULL;
static _Thread_local int64_t hero_dir_count = 0;
static _Thread_local int64_t hero_dir_room = 0;

/* Every name and every block of names every thread's listing holds, so the
 * exit check can tell a listing the program never released from a runtime
 * call that kept what it borrowed (defect 573). */
static _Atomic int64_t hero_dir_names_held = 0;
static _Atomic int64_t hero_dir_blocks_held = 0;

static int64_t hero_dir_names_held_at_exit(void) {
    return atomic_load(&hero_dir_names_held);
}

static int64_t hero_dir_blocks_held_at_exit(void) {
    return atomic_load(&hero_dir_blocks_held);
}

/* What a scan is looking for. */
#define HERO_DIR_FILES 0
#define HERO_DIR_DIRECTORIES 1

static void hero_dir_reset(void) {
    for (int64_t i = 0; i < hero_dir_count; i += 1) {
        hero_release(hero_dir_names[i]);
        hero_dir_names[i] = NULL;
        atomic_fetch_sub(&hero_dir_names_held, 1);
    }
    hero_dir_count = 0;
}

/* Room for one more name, doubling. Answers 0 only when the allocator itself
 * gave up, which `hero_alloc` turns into a panic before it can get here. */
static int hero_dir_make_room(void) {
    if (hero_dir_count < hero_dir_room) return 1;
    int64_t wanted = hero_dir_room == 0 ? 256 : hero_dir_room * 2;
    char **grown = hero_alloc((size_t)wanted * sizeof(char *));
    atomic_fetch_add(&hero_dir_blocks_held, 1);
    for (int64_t i = 0; i < hero_dir_count; i += 1) grown[i] = hero_dir_names[i];
    if (hero_dir_names != NULL) {
        hero_release(hero_dir_names);
        atomic_fetch_sub(&hero_dir_blocks_held, 1);
    }
    hero_dir_names = grown;
    hero_dir_room = wanted;
    return 1;
}

static int hero_dir_keep(const char *name) {
    if (name[0] != '.') return 1;
    if (name[1] == '\0') return 0;
    if (name[1] == '.' && name[2] == '\0') return 0;
    return 1;
}

/* One name, remembered as `prefix + name` so a recursive scan answers paths a
 * caller can open. Answers 0 when the list is full. */
static int hero_dir_remember(const char *prefix, const char *name) {
    if (!hero_dir_make_room()) return 0;
    size_t lead = strlen(prefix);
    size_t tail = strlen(name);
    char *joined = hero_alloc(lead + tail + 1);
    memcpy(joined, prefix, lead);
    memcpy(joined + lead, name, tail);
    joined[lead + tail] = '\0';
    hero_dir_names[hero_dir_count] = joined;
    hero_dir_count += 1;
    atomic_fetch_add(&hero_dir_names_held, 1);
    return 1;
}

static int hero_dir_walk(const char *root, const char *prefix, int64_t want,
                         int64_t recursive, int follow);

/* Is `path` a link itself, asked of the link and never of what it names: a
 * symbolic link on POSIX (`lstat`), a symbolic link or a junction on Windows
 * (`hero_fs_is_surrogate`, parts/fs.c). */
static int hero_dir_is_link(const char *path) {
#if defined(_WIN32)
    return hero_fs_is_surrogate(path);
#else
    struct stat info;
    return lstat(path, &info) == 0 && S_ISLNK(info.st_mode);
#endif
}

static int64_t hero_dir_scan_walk(const char *root, int64_t want, int64_t recursive, int follow);

/* Everything `root` holds, of the kind asked for. Returns how many names, or
 * -1 when the directory could not be read at all — which the shell's
 * `2>/dev/null` used to hide. A link to a directory is walked into, as every
 * listing here always has: a walk reads what its tree names. */
int64_t hero_dir_scan(const char *root, int64_t want, int64_t recursive) {
    return hero_dir_scan_walk(root, want, recursive, 1);
}

/* The scan, `follow` saying whether a link to a directory is walked into or
 * listed as the one name it is (`hero_dir_remove_tree`, defect 345). */
static int64_t hero_dir_scan_walk(const char *root, int64_t want, int64_t recursive, int follow) {
    hero_dir_reset();
    if (!hero_dir_walk(root, "", want, recursive, follow)) {
        /* **-1 means the runtime holds NOTHING**, array included, and that is an
         * invariant rather than a tidiness: a caller that gets -1 returns its
         * own failure, and it has no listing to release. Five call sites in
         * `tests/harness/shell.hero` are written exactly that way. This used to
         * be `hero_dir_reset()`, which frees the names and keeps the block that
         * holds them — one live scratch buffer, and the leak gate said so. */
        hero_dir_release();
        return -1;
    }
    return hero_dir_count;
}

/* Let the listing go. A caller reads the names it wants and then calls this:
 * the names are `hero_alloc`, so a listing nobody releases is a leak — and
 * since 2026-08-30 that is a panic at exit rather than a silence, which is how
 * this function came to exist at all. The scan itself resets before it fills,
 * so the only listing that can survive is the last one. */
void hero_dir_release(void) {
    hero_dir_reset();
    if (hero_dir_names != NULL) {
        hero_release(hero_dir_names);
        atomic_fetch_sub(&hero_dir_blocks_held, 1);
        hero_dir_names = NULL;
        hero_dir_room = 0;
    }
}

/* The name at that index, as an owned `str`. Out of range is a panic, which is
 * what an out-of-range index does everywhere else in this language (§4.9).
 * So is a name that is not UTF-8, which `hero_str_from_bytes` refuses: the
 * compiler's own walk asks `hero_dir_at_shown` below instead (defect 239). */
HeroStr hero_dir_at(int64_t index) {
    if (index < 0 || index >= hero_dir_count) {
        hero_panic("a directory listing was read out of range");
    }
    return hero_str_from_bytes(hero_dir_names[index], (int64_t)strlen(hero_dir_names[index]));
}

/* The name at that index as the shown read gives a file (defect 239):
 * HERO_OS_OK and the name where it is UTF-8; HERO_OS_NOT_TEXT, the marks and
 * the shown name where it is not, which a `str` can hold and a message can
 * name, where `hero_dir_at` panicked, `probe` and `mutate` exit 134 over a
 * folder holding `caf<E9>.hero` on Linux and exit 127 over one holding
 * `café.hero` on Windows, whose `FindFirstFileA` answers in the ANSI code
 * page. One converter, `hero_bytes_shown` (parts/os.c), for a file, a value
 * and a name. Out of range is the panic `hero_dir_at` gives. */
HeroStr hero_dir_at_shown(int64_t index, int64_t *status, int64_t *marks) {
    if (index < 0 || index >= hero_dir_count) {
        hero_panic("a directory listing was read out of range");
    }
    const char *name = hero_dir_names[index];
    return hero_bytes_shown(name, (int64_t)strlen(name), status, marks);
}

/* Delete a path and everything under it. `rm -rf`: a path that is not there is
 * success, because every caller of this asks for the end state and not for the
 * work.
 *
 * **A LINK IS REMOVED AS THE LINK, and never followed** (defect 345), as `rm
 * -rf` removes one: the walk below lists a link to a directory as the one
 * name it is, and a path that is itself a link goes alone. Until 2026-10-05
 * the walk entered such a link and deleted the files it named, outside the
 * tree asked for, and answered success (measured on this Mac: a tree holding
 * `link -> ../A` took `A/keep.txt` with it). */
int64_t hero_dir_remove_tree(const char *path) {
    if (hero_dir_is_link(path)) return hero_fs_remove(path);
    if (!hero_fs_exists(path)) return HERO_OS_OK;
    if (!hero_fs_is_directory(path)) return hero_fs_remove(path);

    /* The listing is taken WHOLE before anything is deleted. Walking a
     * directory while removing from it is unspecified in POSIX and wrong on
     * Windows, and the bug it makes is the worst kind: it deletes some of the
     * entries and reports success. */
    int64_t found = hero_dir_scan_walk(path, HERO_DIR_FILES, 1, 0);
    if (found < 0) return HERO_OS_FAILED;   /* a failed scan holds nothing */

    size_t room = strlen(path) + 2 + HERO_FS_PATH_MAX;
    char *full = hero_alloc(room);
    int64_t failed = 0;

    for (int64_t i = 0; i < found; i += 1) {
        int written = snprintf(full, room, "%s/%s", path, hero_dir_names[i]);
        if (written < 0 || (size_t)written >= room) { failed = 1; continue; }
        if (hero_fs_remove(full) != HERO_OS_OK) failed = 1;
    }

    /* Directories, deepest first: the scan yields parents before children, so
     * the reverse order empties from the bottom. */
    int64_t dirs = hero_dir_scan_walk(path, HERO_DIR_DIRECTORIES, 1, 0);
    if (dirs < 0) { hero_release(full); return HERO_OS_FAILED; }

    for (int64_t i = dirs - 1; i >= 0; i -= 1) {
        int written = snprintf(full, room, "%s/%s", path, hero_dir_names[i]);
        if (written < 0 || (size_t)written >= room) { failed = 1; continue; }
        if (hero_fs_remove(full) != HERO_OS_OK) failed = 1;
    }
    hero_release(full);

    /* **`hero_dir_release`, not `hero_dir_reset`.** Reset frees the names and
     * keeps the block that holds them, so this function leaked exactly one
     * scratch buffer per call — invisible to a caller, and caught by the leak
     * gate this milestone added hours earlier. Nothing here is read after this
     * point, so the listing goes whole. */
    hero_dir_release();

    if (hero_fs_remove(path) != HERO_OS_OK) failed = 1;
    return failed ? HERO_OS_FAILED : HERO_OS_OK;
}

/* One directory level, recursing where asked. Answers 0 when the directory
 * could not be opened or the listing filled up. Where `follow` is 0 a link is
 * one entry of the files, whatever it names, and is never entered. */
static int hero_dir_walk(const char *root, const char *prefix, int64_t want,
                         int64_t recursive, int follow) {
    size_t room = strlen(root) + 2 + HERO_FS_PATH_MAX;
    char *child = hero_alloc(room);
    char *deeper = hero_alloc(room);
    int ok = 1;

#if defined(_WIN32)
    /* Wide, and a name carried back by `hero_win_name_bytes` (parts/codepage.c
     * says why the narrow listing cannot be kept). */
    wchar_t *pattern = hero_win_wide(root, L"\\*");
    WIN32_FIND_DATAW found;
    HANDLE search = pattern != NULL ? FindFirstFileW(pattern, &found) : INVALID_HANDLE_VALUE;
    if (pattern != NULL) hero_release(pattern);
    if (search == INVALID_HANDLE_VALUE) {
        hero_release(child);
        hero_release(deeper);
        return 0;
    }
    char held[3 * MAX_PATH + 1];
    do {
        if (hero_win_name_bytes(found.cFileName, held, sizeof held) < 0) { ok = 0; break; }
        const char *name = held;
        if (!hero_dir_keep(name)) continue;
        int is_dir = (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (!follow && (found.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0 &&
            IsReparseTagNameSurrogate(found.dwReserved0))
            is_dir = 0;
#else
    DIR *open_dir = opendir(root);
    if (open_dir == NULL) {
        hero_release(child);
        hero_release(deeper);
        return 0;
    }
    struct dirent *entry;
    while ((entry = readdir(open_dir)) != NULL) {
        const char *name = entry->d_name;
        if (!hero_dir_keep(name)) continue;
        if (snprintf(child, room, "%s/%s", root, name) < 0) { ok = 0; break; }
        int is_dir = (follow || !hero_dir_is_link(child)) && hero_fs_is_directory(child) != 0;
#endif
        int wanted = is_dir ? (want == HERO_DIR_DIRECTORIES)
                            : (want == HERO_DIR_FILES);
        if (wanted && !hero_dir_remember(prefix, name)) { ok = 0; break; }

        if (is_dir && recursive) {
            if (snprintf(child, room, "%s/%s", root, name) < 0) { ok = 0; break; }
            if (snprintf(deeper, room, "%s%s/", prefix, name) < 0) { ok = 0; break; }
            if (!hero_dir_walk(child, deeper, want, recursive, follow)) { ok = 0; break; }
        }
#if defined(_WIN32)
    } while (FindNextFileW(search, &found));
    /* The end of a listing is ERROR_NO_MORE_FILES; any other reason is a
     * listing cut short, said as a failure and never as the whole. */
    if (ok && GetLastError() != ERROR_NO_MORE_FILES) ok = 0;
    FindClose(search);
#else
    }
    closedir(open_dir);
#endif
    hero_release(child);
    hero_release(deeper);
    return ok;
}
