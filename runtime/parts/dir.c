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
static char **hero_dir_names = NULL;
static int64_t hero_dir_count = 0;
static int64_t hero_dir_room = 0;

/* What a scan is looking for. */
#define HERO_DIR_FILES 0
#define HERO_DIR_DIRECTORIES 1

static void hero_dir_reset(void) {
    for (int64_t i = 0; i < hero_dir_count; i += 1) {
        hero_release(hero_dir_names[i]);
        hero_dir_names[i] = NULL;
    }
    hero_dir_count = 0;
}

/* Room for one more name, doubling. Answers 0 only when the allocator itself
 * gave up, which `hero_alloc` turns into a panic before it can get here. */
static int hero_dir_make_room(void) {
    if (hero_dir_count < hero_dir_room) return 1;
    int64_t wanted = hero_dir_room == 0 ? 256 : hero_dir_room * 2;
    char **grown = hero_alloc((size_t)wanted * sizeof(char *));
    for (int64_t i = 0; i < hero_dir_count; i += 1) grown[i] = hero_dir_names[i];
    if (hero_dir_names != NULL) hero_release(hero_dir_names);
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
    return 1;
}

static int hero_dir_walk(const char *root, const char *prefix, int64_t want,
                         int64_t recursive);

/* Everything `root` holds, of the kind asked for. Returns how many names, or
 * -1 when the directory could not be read at all — which the shell's
 * `2>/dev/null` used to hide. */
int64_t hero_dir_scan(const char *root, int64_t want, int64_t recursive) {
    hero_dir_reset();
    if (!hero_dir_walk(root, "", want, recursive)) {
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
        hero_dir_names = NULL;
        hero_dir_room = 0;
    }
}

/* The name at that index, as an owned `str`. Out of range is a panic, which is
 * what an out-of-range index does everywhere else in this language (§4.9). */
HeroStr hero_dir_at(int64_t index) {
    if (index < 0 || index >= hero_dir_count) {
        hero_panic("a directory listing was read out of range");
    }
    return hero_str_from_bytes(hero_dir_names[index], (int64_t)strlen(hero_dir_names[index]));
}

/* Delete a path and everything under it. `rm -rf`: a path that is not there is
 * success, because every caller of this asks for the end state and not for the
 * work. */
int64_t hero_dir_remove_tree(const char *path) {
    if (!hero_fs_exists(path)) return HERO_OS_OK;
    if (!hero_fs_is_directory(path)) return hero_fs_remove(path);

    /* The listing is taken WHOLE before anything is deleted. Walking a
     * directory while removing from it is unspecified in POSIX and wrong on
     * Windows, and the bug it makes is the worst kind: it deletes some of the
     * entries and reports success. */
    int64_t found = hero_dir_scan(path, HERO_DIR_FILES, 1);
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
    int64_t dirs = hero_dir_scan(path, HERO_DIR_DIRECTORIES, 1);
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
 * could not be opened or the listing filled up. */
static int hero_dir_walk(const char *root, const char *prefix, int64_t want,
                         int64_t recursive) {
    size_t room = strlen(root) + 2 + HERO_FS_PATH_MAX;
    char *child = hero_alloc(room);
    char *deeper = hero_alloc(room);
    int ok = 1;

#if defined(_WIN32)
    char *pattern = hero_alloc(room);
    if (snprintf(pattern, room, "%s\\*", root) < 0) { ok = 0; }
    WIN32_FIND_DATAA found;
    HANDLE search = ok ? FindFirstFileA(pattern, &found) : INVALID_HANDLE_VALUE;
    hero_release(pattern);
    if (search == INVALID_HANDLE_VALUE) {
        hero_release(child);
        hero_release(deeper);
        return 0;
    }
    do {
        const char *name = found.cFileName;
        if (!hero_dir_keep(name)) continue;
        int is_dir = (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
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
        int is_dir = hero_fs_is_directory(child) != 0;
#endif
        int wanted = is_dir ? (want == HERO_DIR_DIRECTORIES)
                            : (want == HERO_DIR_FILES);
        if (wanted && !hero_dir_remember(prefix, name)) { ok = 0; break; }

        if (is_dir && recursive) {
            if (snprintf(child, room, "%s/%s", root, name) < 0) { ok = 0; break; }
            if (snprintf(deeper, room, "%s%s/", prefix, name) < 0) { ok = 0; break; }
            if (!hero_dir_walk(child, deeper, want, recursive)) { ok = 0; break; }
        }
#if defined(_WIN32)
    } while (FindNextFileA(search, &found));
    FindClose(search);
#else
    }
    closedir(open_dir);
#endif
    hero_release(child);
    hero_release(deeper);
    return ok;
}
