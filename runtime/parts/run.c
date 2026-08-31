/* parts/run.c — running a program by argument list, instead of by writing a
 * sentence for a shell to parse.
 *
 * WHAT THIS REPLACES. The compiler used to build a command LINE as a string and
 * hand it to `system()`, quoting every filename on the way with a hand-written
 * POSIX quoter. Panel 098 measured what that costs: the same argument
 * `a b 'c' $HOME` arrives as 14 bytes through an argument list and as 20 —
 * `a b c /Users/joseph`, three words, quotes eaten, the variable expanded —
 * through a shell with the quoter removed. The quoter was a parser defence
 * maintained by hand across 189 call sites. An argument list deletes the parser.
 *
 * WHY A BUILDER AND NOT ONE CALL. `hero_run(program, argv: [str])` cannot be
 * declared: `[str]` is `error[ffi_type]`, and packing the arguments into one
 * NUL-separated string is `error[unknown_escape]`. There is no single Heroes
 * value that can carry an argument list, so the list is pushed one word at a
 * time and this file holds it between the calls (panel 098 conditions 1 and 2).
 * The buffers are `hero_alloc`, which `parts/alloc.c` has counted since
 * 2026-08-30 — so a word that is never released is a panic at exit rather than
 * a leak nothing reports.
 *
 * "NEVER STARTED" IS ITS OWN ANSWER, AND THAT IS THE POINT. `system()` answers
 * 127 both for a program that does not exist and for one that ran and exited
 * 127, and a program killed by a signal decodes to `WEXITSTATUS = 0`, which
 * reads as success. That collision is why `heroes build` on a machine with no
 * clang used to say *"internal error: the runtime did not compile"* — the
 * compiler blaming itself for the machine. Here the child reports a failed
 * `execvp` through a close-on-exec pipe, so the parent knows the difference
 * without guessing, and a signal comes back as 128 + the signal.
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#endif

/* The argument list being built. Bounded rather than grown: the longest line
 * this compiler writes is a clang invocation, and 256 words is an order of
 * magnitude above it. Overflow is a panic, not a truncation. */
#define HERO_RUN_MAX_ARGS 256

static char *hero_run_words[HERO_RUN_MAX_ARGS + 1];
static int64_t hero_run_count = 0;

/* Drop every word. Safe to call twice, and called before every build. */
void hero_run_reset(void) {
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        hero_release(hero_run_words[i]);
        hero_run_words[i] = NULL;
    }
    hero_run_count = 0;
}

/* One word, copied. The copy is this file's, because the `str` it came from may
 * be gone before `hero_run_go` is called.
 *
 * A NUL inside the word is a panic rather than a truncation: a `str`'s `len` is
 * authoritative (spec:52) and `execvp` reads to the first NUL, so the two
 * disagree exactly when an argument would silently become a shorter one. */
void hero_run_arg(HeroStr word) {
    if (hero_run_count >= HERO_RUN_MAX_ARGS) {
        hero_panic("a command line reached 256 arguments");
    }
    size_t length = (size_t)hero_str_len(word);
    if (strlen(word.ptr) != length) {
        hero_panic("an argument contains a NUL byte");
    }
    char *copy = hero_alloc(length + 1);
    memcpy(copy, word.ptr, length);
    copy[length] = '\0';
    hero_run_words[hero_run_count] = copy;
    hero_run_count += 1;
}

/* Where a stream goes when the caller wants it thrown away. The word is spelled
 * here and nowhere else: `selfhost/` never writes `/dev/null`, which is a
 * platform word panel 097 keeps out of the compiler (condition 7). A caller
 * that wants to discard asks `hero_run_discard_path()` for the local spelling
 * and passes it back as an ordinary path.
 *
 * **An empty path means INHERIT, and that is the safer default of the two.**
 * `heroes run` hands the program the terminal it was called from: its output is
 * the point of the command, and a stream that vanishes by default would be a
 * silence nobody asked for. Discarding is the exception and says so at the call
 * site. */
#if defined(_WIN32)
#define HERO_RUN_DISCARD "NUL"
#else
#define HERO_RUN_DISCARD "/dev/null"
#endif

HeroStr hero_run_discard_path(void) {
    return hero_str_from_bytes(HERO_RUN_DISCARD, (int64_t)strlen(HERO_RUN_DISCARD));
}

static int hero_run_inherits(const char *path) {
    return path == NULL || path[0] == '\0';
}

#if defined(_WIN32)
/* THE QUOTING DOES NOT DISAPPEAR ON WINDOWS, IT MOVES HERE.
 *
 * `CreateProcess` takes one string and the child's C runtime parses it back
 * into words, so an argument list has to be re-flattened. The rules are the
 * CRT's, and they are not the shell's: a word is wrapped in double quotes when
 * it holds a space, a tab or a quote; a `"` inside becomes `\"`; and every
 * backslash immediately before a quote — the closing one included — is doubled,
 * which is why `C:\dir\` at the end of a quoted word needs `C:\dir\\`.
 *
 * This function replaces one that had a test (`cli_shell.hero`'s `sq`), so it
 * ships with one of its own: `tests/golden/run/win-quote-round-trip.hero`
 * asserts the round trip on the words that break naive implementations.
 * **NOT VERIFIED on a real Windows CRT** — the round trip is checked against
 * this project's own parser on the platforms it can run on, and the first green
 * Windows tag run is what closes that gap. */
static void hero_run_win_quote(const char *word, char *out, size_t *at) {
    size_t needs_quotes = 0;
    for (const char *p = word; *p != '\0'; p += 1) {
        if (*p == ' ' || *p == '\t' || *p == '"') needs_quotes = 1;
    }
    if (word[0] == '\0') needs_quotes = 1;

    if (!needs_quotes) {
        for (const char *p = word; *p != '\0'; p += 1) out[(*at)++] = *p;
        return;
    }

    out[(*at)++] = '"';
    for (const char *p = word; *p != '\0'; ) {
        size_t slashes = 0;
        while (*p == '\\') { slashes += 1; p += 1; }
        if (*p == '\0') {
            /* Trailing backslashes precede the closing quote: double them, or
             * the CRT reads the quote as escaped and the word never ends. */
            for (size_t i = 0; i < slashes * 2; i += 1) out[(*at)++] = '\\';
            break;
        }
        if (*p == '"') {
            for (size_t i = 0; i < slashes * 2 + 1; i += 1) out[(*at)++] = '\\';
            out[(*at)++] = '"';
        } else {
            for (size_t i = 0; i < slashes; i += 1) out[(*at)++] = '\\';
            out[(*at)++] = *p;
        }
        p += 1;
    }
    out[(*at)++] = '"';
}

/* Every pushed word, flattened into the one string CreateProcess wants. Owned,
 * and the caller decrefs it. Worst case per word is 2n+3 bytes — every byte a
 * backslash before a quote, plus the wrapping quotes and the separator. */
static HeroStr hero_run_win_command_line(void) {
    size_t room = 1;
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        room += strlen(hero_run_words[i]) * 2 + 3;
    }
    char *line = hero_alloc(room);
    size_t at = 0;
    for (int64_t i = 0; i < hero_run_count; i += 1) {
        if (i > 0) line[at++] = ' ';
        hero_run_win_quote(hero_run_words[i], line, &at);
    }
    line[at] = '\0';
    HeroStr built = hero_str_from_bytes(line, (int64_t)at);
    hero_release(line);
    return built;
}
#endif

#if !defined(_WIN32)
/* The child half of the POSIX arm: redirect, exec, and if the exec fails say so
 * down the pipe before dying. Never returns. */
static void hero_run_child(const char *program, int report,
                           const char *out_path, const char *err_path) {
    /* An empty path leaves the stream alone, so the child writes to whatever
     * this process was writing to. */
    if (!hero_run_inherits(out_path)) {
        int out = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out < 0) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
        dup2(out, 1);
        if (out > 2) close(out);
    }
    if (!hero_run_inherits(err_path)) {
        int err = open(err_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (err < 0) {
            int failure = errno;
            ssize_t ignored = write(report, &failure, sizeof failure);
            (void)ignored;
            _exit(126);
        }
        dup2(err, 2);
        if (err > 2) close(err);
    }

    execvp(program, hero_run_words);

    /* Only reachable when the exec failed. The pipe is close-on-exec, so the
     * parent seeing bytes here means "never started" and seeing none means the
     * program ran — which is the distinction `system()` cannot make. */
    int failure = errno;
    ssize_t ignored = write(report, &failure, sizeof failure);
    (void)ignored;
    _exit(127);
}
#endif

/* Run the program with the words pushed so far, `hero_run_words[0]` included as
 * argv[0] by convention. Returns the exit code; `*status` is HERO_OS_OK when the
 * program ran at all, HERO_OS_NOT_FOUND when it could not be started, and
 * HERO_OS_FAILED when this runtime could not try. A caller reads `*status`
 * first and the code second (panel 098 condition 3).
 *
 * The word list is left in place: `hero_run_reset` is the caller's to call, and
 * the leak gate says so at exit if it forgets. */
int64_t hero_run_go(const char *program, const char *out_path,
                    const char *err_path, int64_t *status) {
    if (hero_run_count == 0) {
        *status = HERO_OS_FAILED;
        return -1;
    }
    hero_run_words[hero_run_count] = NULL;

#if defined(_WIN32)
    /* Windows has no argv at the operating-system level: `CreateProcess` takes
     * one command-line STRING which the child's C runtime parses back. So the
     * quoting an argument list deletes on POSIX does not disappear here, it
     * changes algorithm and moves into C — see `hero_run_win_quote`. */
    HeroStr line = hero_run_win_command_line();
    SECURITY_ATTRIBUTES inherit;
    inherit.nLength = sizeof inherit;
    inherit.lpSecurityDescriptor = NULL;
    inherit.bInheritHandle = TRUE;

    HANDLE out = hero_run_inherits(out_path)
        ? GetStdHandle(STD_OUTPUT_HANDLE)
        : CreateFileA(out_path, GENERIC_WRITE, FILE_SHARE_READ, &inherit,
                      CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE err = hero_run_inherits(err_path)
        ? GetStdHandle(STD_ERROR_HANDLE)
        : CreateFileA(err_path, GENERIC_WRITE, FILE_SHARE_READ, &inherit,
                      CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (out == INVALID_HANDLE_VALUE || err == INVALID_HANDLE_VALUE) {
        if (out != INVALID_HANDLE_VALUE && !hero_run_inherits(out_path)) CloseHandle(out);
        if (err != INVALID_HANDLE_VALUE && !hero_run_inherits(err_path)) CloseHandle(err);
        hero_str_decref(line);
        *status = HERO_OS_FAILED;
        return -1;
    }

    STARTUPINFOA startup;
    memset(&startup, 0, sizeof startup);
    startup.cb = sizeof startup;
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startup.hStdOutput = out;
    startup.hStdError = err;

    PROCESS_INFORMATION child;
    memset(&child, 0, sizeof child);

    /* **`NULL` for the application name, and the program is the command
     * line's first word instead.** Passing it as `lpApplicationName` makes
     * Windows look for that exact path: no PATH search and no `.exe` appended,
     * so `clang` is not found on a machine that has `clang.exe` on its PATH —
     * which is precisely how the first Windows run of this file failed, with
     * `heroes doctor` reporting no C toolchain on a runner that ships one.
     * With NULL, the documented search runs: the working directory, the
     * system directories, then PATH, with the executable extensions applied.
     *
     * `hero_run_win_command_line` already puts the program first, because
     * argv[0] is a pushed word like every other. */
    (void)program;
    BOOL started = CreateProcessA(NULL, (char *)line.ptr, NULL, NULL, TRUE,
                                  0, NULL, NULL, &startup, &child);
    if (!hero_run_inherits(out_path)) CloseHandle(out);
    if (!hero_run_inherits(err_path)) CloseHandle(err);
    hero_str_decref(line);
    if (!started) {
        *status = HERO_OS_NOT_FOUND;
        return -1;
    }

    WaitForSingleObject(child.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(child.hProcess, &code);
    CloseHandle(child.hProcess);
    CloseHandle(child.hThread);
    *status = HERO_OS_OK;
    return (int64_t)code;
#else
    int report[2];
    if (pipe(report) != 0) {
        *status = HERO_OS_FAILED;
        return -1;
    }
    if (fcntl(report[1], F_SETFD, FD_CLOEXEC) != 0) {
        close(report[0]);
        close(report[1]);
        *status = HERO_OS_FAILED;
        return -1;
    }

    pid_t child = fork();
    if (child < 0) {
        close(report[0]);
        close(report[1]);
        *status = HERO_OS_FAILED;
        return -1;
    }
    if (child == 0) {
        close(report[0]);
        hero_run_child(program, report[1], out_path, err_path);
    }

    close(report[1]);
    int failure = 0;
    ssize_t got = read(report[0], &failure, sizeof failure);
    close(report[0]);

    int wait_status = 0;
    while (waitpid(child, &wait_status, 0) < 0) {
        if (errno != EINTR) {
            *status = HERO_OS_FAILED;
            return -1;
        }
    }

    if (got == (ssize_t)sizeof failure) {
        *status = HERO_OS_NOT_FOUND;
        return -1;
    }
    *status = HERO_OS_OK;
    if (WIFSIGNALED(wait_status)) return 128 + WTERMSIG(wait_status);
    return WIFEXITED(wait_status) ? WEXITSTATUS(wait_status) : -1;
#endif
}
