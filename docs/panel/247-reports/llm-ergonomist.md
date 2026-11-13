# Panel 247, llm-ergonomist: the blind seat's forty sessions

Run by the coordinator's agent on 2026-11-13 from 10:25:17 to 10:40:16, four at a time, under the
design registered in `docs/panel/247-briefs/llm-ergonomist.md` before any session (its text before
the extension kept as `llm-ergonomist.before-extension.md`): the author's yes to a paid run of up to
25 USD, given that morning (*può spendere fino a 25*), widened the registered twelve sessions to five
arms (x the base, r **hrr**, e **he**, m **hm6**, s **hs**) times the two tasks times four sessions,
and rescaled the decision rule before any session to *an arm's sentence enters only if it gives at
least three more `right` answers than arm x over its eight sessions, graded under the route its
sentence describes*. Each session is one `claude -p` with the brief's flags (`--bare --restricted
--safe-mode --strict-mcp-config`, `Read,Write` alone, `--max-budget-usd 0.40`), run from a folder of
six random hexadecimal digits under `.claude/worktrees/p247-scratch/s/` holding `brief.md`,
`task.md` and the arm's `heroes-spec.md`, the key loaded from the trunk's `.env` and never printed.
No session wrote `report.md`; each report is `run.json`'s `result`, as at panel 246. **Total spent:
9.5693 USD**, summed from the forty `run.json` files (`total_cost_usd`); the runner's 22 USD bound was
never approached.

**Grading**: each answer's files copied into `p247-scratch/grade/`, built and run under three
setups, every Darwin death read in its crash report: **today**, the base with D (`sources
560c95206c908459`); **C**, the compiler-engineer's `heroes-c2` with its runtime (`sources
1f83fdf86f0d2855`); **E**, that compiler with the runtime's `HERO_SIGNAL_DEFER` set to 1, route F (E's
delivery with C's detection), there being no safe-point runtime built, so arm s's verdict under E is
provisional. Arm m's specification is the base with hm6's one replacement, `3335350f71b16ce0`,
byte-identical to the spec-warden's draft (`cmp`), 8349 / 8499 / 8499 vendored (+9).

**Context**: 40 of 40 answered `no`. Three name the working directory (one the
`.claude/worktrees/p247-scratch` path, two infer macOS from `/Users/`); none is voided, as panel 246
handled its two.

Written into this file by the coordinator from the agent's report (the harness refused the agent a
report file of its own), with `p247-scratch/tools/assemble.py`.

## Every session, in the registered order

| order | session | folder | cost (USD) | turns | report from | context | handler | today | C | E |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | t1-x-1 | `3830e4` | 0.2873 | 4 | `result` | `no` | heroes:allocates | refused | refused | refused |
| 2 | t1-r-1 | `98c872` | 0.1821 | 4 | `result` | `no` | c-header | right | right | right |
| 3 | t1-e-1 | `707099` | 0.2365 | 4 | `result` | `no` | heroes:allocates | right | stopped | right |
| 4 | t1-m-1 | `fcce6e` | 0.2085 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 5 | t1-s-1 | `14aeb3` | 0.2210 | 4 | `result` | `no` | heroes:allocates | right | stopped | right |
| 6 | t2-x-1 | `3b5cc9` | 0.2761 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 7 | t2-r-1 | `304e74` | 0.2051 | 4 | `result` | `no` | c-header | right | right | right |
| 8 | t2-e-1 | `ba6229` | 0.2880 | 4 | `result` | `no` | heroes:allocates | stopped | stopped | right |
| 9 | t2-m-1 | `a19d3a` | 0.3012 | 5 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 10 | t2-s-1 | `8b4305` | 0.2121 | 4 | `result` | `no` | heroes:formats | stopped | stopped | right |
| 11 | t2-s-2 | `47763b` | 0.2203 | 4 | `result` | `no` | heroes:formats | stopped | stopped | right |
| 12 | t2-m-2 | `36e5b3` | 0.2946 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 13 | t2-e-2 | `daaef5` | 0.2675 | 4 | `result` | `no` | heroes:allocates | stopped | stopped | right |
| 14 | t2-r-2 | `e46eba` | 0.2068 | 4 | `result` | `no` | c-header | right | right | right |
| 15 | t2-x-2 | `e63d43` | 0.2784 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 16 | t1-s-2 | `f302f2` | 0.2269 | 4 | `result` | `no` | heroes:allocates | right | stopped | right |
| 17 | t1-m-2 | `3b3338` | 0.2629 | 4 | `result` | `no`; infers macOS from the path `/Users/...`, not voided | blocked-and-asked | refused | refused | refused |
| 18 | t1-e-2 | `597f7e` | 0.2474 | 4 | `result` | `no`; names the folder's path (a `heroes-lang` worktree, `.claude/worktrees/p247-scratch`), not voided | heroes:allocates | refused | refused | refused |
| 19 | t1-r-2 | `a4d252` | 0.1803 | 4 | `result` | `no` | c-header | right | right | right |
| 20 | t1-x-2 | `b91072` | 0.2576 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 21 | t1-x-3 | `38180e` | 0.2366 | 4 | `result` | `no` | blocked-and-asked | right | right | right |
| 22 | t1-r-3 | `2e62c9` | 0.1748 | 4 | `result` | `no` | c-header | right | right | right |
| 23 | t1-e-3 | `87efae` | 0.2774 | 4 | `result` | `no` | heroes:allocates | refused | refused | refused |
| 24 | t1-m-3 | `8be6f8` | 0.2275 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 25 | t1-s-3 | `2dde04` | 0.1922 | 4 | `result` | `no` | heroes:allocates | right | stopped | right |
| 26 | t2-x-3 | `1bf6d1` | 0.3564 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 27 | t2-r-3 | `056a0c` | 0.2029 | 4 | `result` | `no` | c-header | right | right | right |
| 28 | t2-e-3 | `0d036a` | 0.2564 | 4 | `result` | `no` | heroes:allocates | stopped | stopped | right |
| 29 | t2-m-3 | `ff7f62` | 0.2629 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 30 | t2-s-3 | `5c1359` | 0.2168 | 4 | `result` | `no` | heroes:formats | stopped | stopped | right |
| 31 | t2-s-4 | `86745c` | 0.2072 | 4 | `result` | `no` | heroes:formats | stopped | stopped | right |
| 32 | t2-m-4 | `41fe36` | 0.2736 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 33 | t2-e-4 | `313ff5` | 0.2561 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 34 | t2-r-4 | `ec3f30` | 0.1963 | 4 | `result` | `no` | c-header | right | right | right |
| 35 | t2-x-4 | `d9a2db` | 0.2767 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 36 | t1-s-4 | `3fb6c6` | 0.2055 | 4 | `result` | `no` | heroes:allocates | right | stopped | right |
| 37 | t1-m-4 | `51d552` | 0.2295 | 4 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 38 | t1-e-4 | `027658` | 0.2464 | 5 | `result` | `no` | blocked-and-asked | refused | refused | refused |
| 39 | t1-r-4 | `86b0cd` | 0.1872 | 4 | `result` | `no` | c-header | right | right | right |
| 40 | t1-x-4 | `216c39` | 0.2262 | 4 | `result` | `no`; infers macOS from the path `/Users/...`, not voided | blocked-and-asked | refused | refused | refused |

**Total spent**, summed from the 40 `run.json` files' `total_cost_usd` (`python3 -I`): **9.5693 USD**, inside the author's 25.

## Right answers per arm, out of eight (t1 and t2, four each), under each setup

| arm | sentence | today | C | E |
|---|---|---|---|---|
| x | none (the base) | 1 | 1 | 1 |
| r | hrr | 8 | 8 | 8 |
| e | he | 1 | 0 | 4 |
| m | hm6 | 0 | 0 | 0 |
| s | hs | 4 | 0 | 8 |

## The registered rule's verdict per arm

An arm's sentence enters only if it gives at least three more `right` answers than arm x over its eight sessions, both graded under the route its sentence describes (r and m under C, e and s under E, s marked provisional).

| arm | route's setup | arm's right | x's right | difference | verdict |
|---|---|---|---|---|---|
| r | C | 8 | 1 | +7 | **enters** |
| e | E | 4 | 1 | +3 | **enters** |
| m | C | 0 | 1 | -1 | does not enter |
| s | E | 8 | 1 | +7 | **enters** (provisional: graded under E, not S) |

## Findings for the synthesis

1. **`sigemptyset` stops the compiler with `internal error: compiling the generated C failed`**, exit 2, in 7 answers (x 3, m 4): it is a macro on Darwin (`signal.h:128`), not a function. An exit 2 where the author could have been told: a `blocking` defect, filed by the coordinator.
2. **`record sigset_t partial` with no fields** is refused `error[empty_record]` in 7 answers (x 2, m 3, e 2), and the fix it suggests, a handle, does not fit `sigset_t`; whether the specification offers a way to hold a `sigset_t` is a question for the sitting.
3. **The class has another member D does not see**: a handler that lends a buffer to `write` re-enters the lend tracking. Under today two runs died with a false message, *"C wrote to a buffer lent to it after the call it was lent to had returned"* (t2-e-1 run 2, beside D's stop; t2-e-2 run 1, alone).
4. **C's refusal printed twice** in 3 runs of the 159 graded (t2-e-1 runs 1 and 3, t2-e-2 run 3): a second signal arrived while the first refusal ran.
5. **Crash reports of the graded binaries**: today 21 SIGABRT, 19 in `hero_alloc_reentered` (D's stop) and 2 in `hero_panic` through the lend audit; C 31 SIGABRT, all in `hero_signal_refuse`; E none. No lock SIGKILL and no SIGTRAP among them.
6. **Other build refusals**: e 2 `ffi_return_type` (`signal` declared returning nothing), x 1 `ffi_constant_type` (`POLLIN` as `i16`), x 1 `line_end_before_continuation`, m 1 `lent_shape`; 19 of 40 answers refused at build in all. Arm m's zero measures the compiler's handling of signal sets as much as hm6: its eight readers blocked the signals and asked `sigpending`, and none wrote a Heroes handler.


## The forty reports, verbatim

### t1-x-1 (`3830e4`, from `result`)

````markdown
I only have a file-reading tool, so I can't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The program makes a pipe and moves its write end to a fixed file descriptor, `STOP_FD` (100). A handler written in Heroes is installed for `SIGINT` and `SIGTERM`. The handler only writes one byte to that descriptor. This is the "self-pipe" method, chosen because Heroes has no mutable globals for a handler to set.

Each step builds `step N` and adds it to the list. It then sleeps one second with C's `sleep`, going back to sleep if a signal cut the sleep short, and prints the text. Only after that does it check the pipe, using `poll` with a timeout of 0, so a signal never ends a step partway through. When a byte is waiting, the loop stops. The program then prints `stopped after N steps`, with N the length of the list, and `main` returns normally, which gives exit code 0.

Answer to the task's question: I expect `heroes build main.hero` to accept the program. Sent `kill -INT` about 2.5 seconds after it starts, it prints `step 1`, `step 2`, `step 3`, then `stopped after 3 steps`, and exits with code 0. The `prediction` section explains the timing.

### `main.hero`

```
# Runs numbered steps until SIGINT or SIGTERM, then reports how many it printed.

extern "unistd.h"
    function sleep(seconds: u32) -> u32
    function pipe(@fds: [i32] counted_by 2 lent) -> i32
    function dup2(fildes: i32, fildes2: i32) -> i32
    function write(fd: i32, buf: cstr lent, nbyte: u64) -> i64

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "poll.h"
    constant POLLIN: i16
    record PollFd tag pollfd
        fd: i32
        events: i16
        revents: i16
    function poll(@fds: PollFd, nfds: u32, timeout: i32) -> i32

# The descriptor the signal handler writes to; the write end of the stop pipe.
constant STOP_FD: i32
    100

# Signal handler: only records that a stop was asked for.
function on_signal(_: i32)
    _ = write(STOP_FD, "!".cstr(), 1)

function main()
    fds: [i32] @= [0, 0]
    r = pipe(@fds)
    assert r == 0
    d = dup2(fildes: fds[1], fildes2: STOP_FD)
    assert d == STOP_FD
    _ = signal(SIGINT, on_signal)
    _ = signal(SIGTERM, on_signal)
    texts: [str] @= []
    n @= 0
    while true
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        print(text)
        p: PollFd @= PollFd(fd: fds[0], events: POLLIN, revents: 0)
        if poll(@p, 1, 0) > 0
            break
    print(f"stopped after {texts.len()} steps")
```

No other files are needed. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **How the handler tells `main` a stop was asked for.** The spec has no mutable globals and no closures, so a handler can't set a flag. I used a pipe moved to a fixed descriptor, plus `poll` with a timeout of 0.
   - **Blocking the signals with `sigprocmask` and checking `sigpending`:** this needs `sigset_t` as a record. It is a struct on Linux but an integer on macOS, so the compiler would refuse it on at least one platform.
   - **`setenv`/`getenv` as the flag:** this would probably run and give the right answer, but `setenv` is not safe to call from a signal handler.

2. **Return type of `signal`.** C returns a function pointer. I declared it as `-> ptr`, because the spec says a callback is never a result.
   - **Leaving out `->`:** the header check might refuse that declaration, or might accept it. If `ptr` is refused against a function-pointer result, that is a compile error too. This is my main risk of the compiler refusing the program.

3. **`write`'s buffer.** I declared it `buf: cstr lent` and passed `"!".cstr()`, although C says `const void *`.
   - **`@buf: [u8] counted_by nbyte lent`:** this needs a cell inside the handler. If the header check refuses `cstr` against `const void *`, the compiler refuses the program.

4. **Restarting a cut-short `sleep`.** If a signal arrives during the sleep, I go back to sleep for the seconds `sleep` reports as left, so the step still waits about a second.
   - **One `sleep(1)` whose result is discarded:** step 3 prints at about 2.5 s instead of about 3 s. The output text is the same.

5. **When the stop is checked.** I check after the print, at the end of the step, so the step in hand always finishes.
   - **Checking at the start of each step:** the same answer, but an extra step could start after the signal.
   - **Breaking straight after the sleep:** the program would end in the middle of a step, with N one less than the steps begun. That gives a different answer than intended.

6. **Exit code 0.** `main` simply returns.
   - **`exit(code: 0)`:** the spec refuses a code written as a number, so that is a compile error.

7. **`dup2`'s two `i32` parameters.** Named arguments are mandatory when two parameters share a type, and I wrote `fildes:` and `fildes2:`.
   - **Positional arguments:** a compile error.

8. **The `pollfd` struct.** It is declared with `tag pollfd`, because the header writes the name after `struct`.
   - **Without `tag`:** clang would not find a type named `PollFd`, so the compiler refuses it.

9. **`POLLIN` declared as `i16`,** to match the `events` field.
   - **As `i64`:** a type error when building the record, so the compiler refuses it.

10. **Unused handler parameter written as `_`.**
    - **A named parameter:** a compile error, because it is never used.

11. **Where `poll` reads the count from.** I passed one record as a plain `@` out-parameter, with `nfds: u32`. That converts exactly to either `unsigned int` (macOS) or `unsigned long` (Linux).
    - **`[PollFd] counted_by nfds`:** this would also work.
    - **`nfds: u64`:** the compiler refuses it on macOS.

12. **Running a Heroes function as a signal handler.** The spec doesn't say whether the runtime allows this; I assumed it does.
    - **If it doesn't:** the program could stop with an error or misbehave when the signal arrives.

## prediction

I expect the compiler to accept the program. The main risks are choice points 2 and 3, and on platforms where `POLLIN`'s macro type is checked strictly, 9.

Sent `kill -INT` about 2.5 seconds after it starts, it prints:

```
step 1
step 2
step 3
stopped after 3 steps
```

and exits with code 0.

Timing:
- `step 1` prints at about 1 s and `step 2` at about 2 s.
- The signal arrives during step 3's sleep. That step finishes and `step 3` prints at about 2.5–3.5 s, depending on how `sleep` rounds the time it reports as left.
- The final line follows. `kill -TERM` and Ctrl-C give the same result.

## context

Beyond `brief.md`, `task.md` and `heroes-spec.md` in this directory, the only other text that reached me was the system prompt and the user's message. The system prompt gave the agent role, the working directory path and the date. No other rules, memories or documents reached my context, and I read no other file.
````

### t1-r-1 (`98c872`, from `result`)

````markdown
I have no tool that writes files, so I could not create `report.md`. As the brief says to do in that case, the whole report follows as my answer.

# report.md

## answer

The task asks for a program that runs steps until it is told to stop, finishes the step it is on, prints a summary and exits with 0.

**What happens:** I expect `heroes build main.hero` to accept the program and build a binary. I can't be fully sure, because the spec doesn't say exactly how a header the program ships gets included (see choice points 1 and 2). Sent `kill -INT` about 2.5 seconds after it starts, the program prints:

```
step 1
step 2
step 3
stopped after 3 steps
```

and ends with exit code 0.

The program needs two files. The spec (section 13) says that when a Heroes function is given to `signal`, "stops the program the first time a signal calls it… write the handler in C, in a header the program ships". So the handler lives in a C header next to `main.hero`. The handler only sets a flag, and the Heroes loop checks that flag between steps. A signal therefore cuts short at most the `sleep` of the current step. That step still prints its text, and then the loop ends.

### `stop.h`

```c
#ifndef STOP_H
#define STOP_H

#include <signal.h>

/* Set by the handler, read by the program between steps. */
static volatile sig_atomic_t stop_flag = 0;

static void stop_handler(int sig)
{
    (void)sig;
    stop_flag = 1;
}

/* Installs the handler for SIGINT (Ctrl-C, kill -INT) and SIGTERM (kill -TERM).
   Returns 0 on success, -1 if either could not be installed. */
static inline int stop_install(void)
{
    if (signal(SIGINT, stop_handler) == SIG_ERR) {
        return -1;
    }
    if (signal(SIGTERM, stop_handler) == SIG_ERR) {
        return -1;
    }
    return 0;
}

/* 1 once a stop was asked for, else 0. */
static inline int stop_requested(void)
{
    return stop_flag != 0;
}

#endif
```

### `main.hero`

```
extern "stop.h"
    function stop_install() -> i32
    function stop_requested() -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Runs steps until SIGINT or SIGTERM arrives; the step in hand always finishes.
function main()
    assert stop_install() == 0
    done: [str] @= []
    n @= 0
    while stop_requested() == 0
        n @ n + 1
        text = f"step {n}"
        done @ done.push(text)
        _ = sleep(1)
        print(text)
    print(f"stopped after {done.len()} steps")
```

## choice_points

1. **Where the handler goes.** The spec says a Heroes function given to `signal` "stops the program the first time a signal calls it", so I wrote the handler in C, in `stop.h`. If I had instead declared `signal` in an extern group and passed it a Heroes function, the program would build. But the first Ctrl-C or `kill` would end it at once: no finished step, no "stopped after" line, and a nonzero exit. It would run and give the wrong answer.
2. **How the header defines its functions.** I wrote them `static` / `static inline` in the header, on the reading that the generated C includes the header and uses its declarations ("clang checks every result type … against that header"). The other option was ordinary non-static definitions. That works if the compiler writes its own `extern` prototypes. But it gives duplicate-symbol link errors if the header ends up in more than one translation unit. In the other direction, if the compiler does write its own prototypes, my `static` choice clashes with them and the build is refused. The spec doesn't settle this.
3. **Where the header is found.** I assumed `extern "stop.h"` finds the file next to `main.hero`. If the include path doesn't cover that directory, the build is refused.
4. **`signal()` instead of `sigaction()`.** `signal()` needs no feature-test macro. `sigaction` would need `_POSIX_C_SOURCE`, and the spec makes such macros depend on header order ("one defining `_GNU_SOURCE` first"); getting that wrong could get the build refused. With BSD-style `signal` (glibc, macOS) the handler stays installed and calls get restarted. `sleep` is still cut short, and the output doesn't change either way.
5. **The `sleep` declaration.** C's `unsigned int` maps to `u32` at the header's width, as the spec requires for parameters. I also gave the result as `u32`. Declaring the parameter `i64` would be refused, since parameters must match C's width and sign.
6. **The time left after an interrupted `sleep`.** I throw it away (`_ = sleep(1)`), so step 3 prints right when the signal arrives, at about 2.5 s. The alternative was to keep sleeping until the full second had passed. The printed lines would be the same, only about 0.5 s later. I read "finishes the step in hand" as "the step's print still happens".
7. **Checking that installation worked.** I used `assert stop_install() == 0` rather than `exit(code: 1)`. The spec says a code "written as a number is refused", which could mean a literal `1` doesn't compile. If that reading is right, `exit(code: 1)` would be refused. The assert simply aborts if installation fails.
8. **Ending with exit code 0.** `main` just returns. I avoided `exit(code: 0)` for the same reason as point 7: it might be refused.
9. **Where the stop check sits.** I check the flag at the top of each loop. Checking straight after `sleep` and leaving before `print` would run, but give a wrong answer: it would drop the "step 3" line, which breaks "finishes the step in hand".
10. **Where N in the summary comes from.** I use `done.len()`. This equals both `n` and the number of steps printed, so using `n` would print the same thing. I chose `done.len()` partly because it reads the list, so the list is used.
11. **The `int` results.** `stop_install` and `stop_requested` return `i32`, matching C's `int`. The spec also allows a wider result, such as `i64`; that would compile and print the same.

## prediction

- **Does it compile?** I expect `heroes build main.hero` to accept it. The main risks are choice points 2 and 3, which the spec leaves open.
- **Sent `kill -INT` at about 2.5 s:** step 1 prints at about 1 s and step 2 at about 2 s. Step 3's `sleep` starts at about 2 s and is cut short by the signal at 2.5 s. Step 3 then prints, the loop's check sees the flag, and the program prints the summary and returns from `main` with exit code 0:

```
step 1
step 2
step 3
stopped after 3 steps
```

`kill -TERM` gives the same result. Ctrl-C at a terminal also does, though the terminal may echo `^C` first.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, my context held only the agent system prompt: that I'm a Claude agent built on the Claude Agent SDK, the working directory and date, and the tool definitions. I saw no other project instruction, rule, contract, memory or document, and I read no other file.
````

### t1-e-1 (`707099`, from `result`)

````markdown
I couldn't create `report.md` because I can only read files here, so the whole report is below. I haven't built or run the program; everything in `prediction` is what I expect from reading the spec.

# Report

## answer

The task asks for `main.hero`: a loop of steps that stops cleanly on SIGINT or SIGTERM. It also asks whether `heroes build main.hero` accepts the program and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**How it works:**
- **Catching the signal.** The program registers a handler for SIGINT and SIGTERM with C's `signal`. The spec says such a handler "runs after the signal, on a thread of the runtime's own, never inside it."
- **Telling the loop to stop.** Heroes has no mutable globals, and only an `i64` can cross between threads. So the handler records the stop request in the process environment with `setenv`, and the main loop checks it with `getenv` before each step.
- **Finishing the step.** A step always runs to the end: build the text, add it to the list, wait one second, print. If the signal cuts `sleep` short, the program sleeps for the time that was left.
- **The final count.** N in `stopped after N steps` is the length of the list. Every step adds one text and prints it, so this equals the number of steps printed.
- **Exit code.** `main` returns normally, which ends with exit code 0.

### `main.hero`
```
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Name of the environment variable that records a stop request.
constant STOP_VAR: str
    "HEROES_STOP_REQUESTED"

# Runs on the runtime's own thread after SIGINT or SIGTERM: records the request.
function on_stop(_: i32)
    _ = setenv(name: STOP_VAR.cstr(), value: "1".cstr(), overwrite: 1)

# Whether a stop has been requested.
function stop_requested() -> bool
    return !getenv(STOP_VAR.cstr()).validated().is_err()

# Waits one whole second, even if a signal cuts sleep short.
function wait_one_second()
    left @= sleep(1)
    while left > 0
        left @ sleep(left)

function main()
    _ = unsetenv(STOP_VAR.cstr())
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    while !stop_requested()
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        wait_one_second()
        print(text)
    print(f"stopped after {texts.len()} steps")
```

This is the only file. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **How the handler reaches the main loop.** The spec gives no shared mutable state: there are no mutable globals, a handler can't capture anything, and only an `i64` crosses threads.
   - **My choice:** the handler sets an environment variable with `setenv`, and the loop polls it with `getenv`.
   - **Writing a file with `write_file` and polling with `read_file`:** would also work, but leaves a file behind.
   - **A pipe with assumed file descriptor numbers:** might stop with an error or never stop.
   - **Blocking the signal with `sigprocmask` and checking `sigpending`:** needs `sigset_t` as a `partial` record. The runtime's own thread handling may then stop the signal from ever showing as pending, so the program would never stop.
   - **Risk in my choice:** POSIX does not promise that `setenv` on one thread and `getenv` on another are safe together. In practice it works.

2. **The result type of `signal`.** In C it returns `void (*)(int)`, and the spec says a callback can only be a parameter, never a result.
   - **My choice:** `-> ptr`, discarded with `_ =`.
   - **No `->` at all (C `void`):** clang's result-type check would probably refuse it.
   - **A function type as the result:** refused by the rule above.
   - **Risk:** the compiler might also refuse `ptr` against a function pointer. The spec doesn't say.

3. **Types of the constants and parameters.**
   - **My choice:** `SIGINT` and `SIGTERM` as `i32`, the handler's parameter as `i32` (C `int`), and `sleep` as `u32 -> u32` (C `unsigned int`).
   - **`i64` constants:** the spec allows a wider type, but every call would then need `to_i32(...).must()`. Passing the `i64` directly would be refused.
   - **`i64` for a parameter:** refused, because a parameter must match the header's width.

4. **What happens when the signal interrupts `sleep`.**
   - **My choice:** sleep again for the seconds `sleep` reports as left, so each step really waits about one second.
   - **A single `sleep(1)`:** step 3 would end at about 2.5 s. The handler thread might not have set the variable yet, so the loop could start step 4. The output would then be `step 4` and `stopped after 4 steps`, which is a different answer.

5. **Where the stop check sits.**
   - **My choice:** in the `while` condition, so it runs only between steps.
   - **A check inside the step (for example right after `sleep`):** that step would end without printing, and the count would be wrong.

6. **How the program ends with code 0.**
   - **My choice:** `main` simply returns.
   - **`exit(code: 0)`:** the compiler refuses it, because an exit code "written as a number is refused".

7. **Which number goes into `stopped after N steps`.**
   - **My choice:** `texts.len()`. This also reads the list, so the program counts as using it.
   - **`n`:** gives the same number. With `n`, the list is still read by its own `push`, so it would probably not count as unused.

8. **A stale variable from outside.**
   - **My choice:** `unsetenv` at startup, so a `HEROES_STOP_REQUESTED` already in the environment can't stop the program early.
   - **Leaving it out:** if the variable was already set, the program would print `stopped after 0 steps` immediately.

9. **Lend vs lease for C strings.**
   - **My choice:** `.cstr()`, which lends the string for one call only, with `cstr lent` parameters.
   - **`.lease()`:** each lease would have to be ended with `end_lease`. Without that, the program stops with an error when `main` returns.

10. **The handler's unused parameter.**
    - **My choice:** `_: i32`.
    - **A named parameter:** refused by the compiler as unused.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it. The least certain part is the `-> ptr` result of `signal` (choice point 2).
- **Output with `kill -INT` at about 2.5 s:**

  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```

  - `step 1` prints at about 1 s and `step 2` at about 2 s.
  - Step 3 is mid-sleep when the signal arrives. If the signal cuts `sleep` short, the program sleeps the rest, so step 3 finishes at about 3 s.
  - The stop check then sees the request, the program prints the last line and ends with exit code 0.
- **Timing risk:** glibc's `sleep` may round the half second left down to 0. Step 3 would then end at about 2.5 s, racing the handler thread, and the output could instead end with `step 4` and `stopped after 4 steps`.
- **Ctrl-C at the terminal:** the terminal may also echo `^C`.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document did, apart from the general system setup (working directory and date).
````

### t1-m-1 (`fcce6e`, from `result`)

````markdown
I couldn't save `report.md` because I have no tool that writes files, so the whole report is below as the brief allows. I haven't built or run the program, so whether the compiler accepts it and what it prints are predictions.

# report.md

## answer

The task asks for `main.hero`, plus two answers: does `heroes build main.hero` accept it, and what does it print when sent `kill -INT` about 2.5 seconds after it starts?

**How it works.** The specification forbids a Heroes function from being a signal handler (section 13: "A callback is a parameter, never a result or a signal handler"). So the program cannot catch the signal with a handler. Instead it **blocks** SIGINT and SIGTERM with `sigprocmask`, using the C functions from `signal.h`. A blocked signal neither ends the process nor interrupts `sleep`; it just stays pending. After each full step, the program asks `sigpending` whether either signal has arrived. If one has, it leaves the loop and prints the summary. `main` then returns normally, which gives exit code 0. It can't call `exit(code: 0)`, because section 11 refuses an exit code written as a number.

The space for a `sigset_t` comes from `malloc` and is passed as a `ptr`. Section 13 exempts "what a `ptr` points at" from the width check, so the program never has to declare the layout of `sigset_t`, which differs between platforms. 1024 bytes is more than any platform needs (Linux needs 128, macOS 4).

There is only one file, and no `heroes.toml`, because the program uses no package.

### `main.hero`
```
# Prints `step N` once a second until SIGINT or SIGTERM arrives, never stopping
# in the middle of a step, then prints how many steps it printed and ends with 0.

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function sigprocmask(how: i32, set: ptr, oldset: ptr) -> i32
    function sigpending(set: ptr) -> i32
    function sigismember(set: ptr, signo: i32) -> i32

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Bytes reserved for one C `sigset_t`; larger than any platform's.
constant SIGSET_BYTES: u64
    1024

function main()
    stop_signals = malloc(SIGSET_BYTES)
    assert stop_signals != nullptr
    emptied = sigemptyset(stop_signals)
    assert emptied == 0
    added_int = sigaddset(stop_signals, SIGINT)
    assert added_int == 0
    added_term = sigaddset(stop_signals, SIGTERM)
    assert added_term == 0
    blocked = sigprocmask(how: SIG_BLOCK, set: stop_signals, oldset: nullptr)
    assert blocked == 0
    pending = malloc(SIGSET_BYTES)
    assert pending != nullptr
    texts: [str] @= []
    n @= 0
    while true
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
        asked = sigpending(pending)
        assert asked == 0
        if sigismember(pending, SIGINT) == 1 || sigismember(pending, SIGTERM) == 1
            break
    print(f"stopped after {texts.len()} steps")
    free(stop_signals)
    free(pending)
```

## choice_points

1. **Catching the signal.**
   - **My choice:** block SIGINT and SIGTERM and check `sigpending` after each step.
   - **The other:** pass a Heroes function to `signal` or `sigaction` as a handler. Section 13 forbids that, so the compiler refuses it.
   - **Doing nothing:** Ctrl-C would kill the process mid-step with status 130, not 0, and nothing after the last step would print. That runs but gives the wrong answer.
2. **Declaring `sigset_t`.**
   - **My choice:** `malloc`'d memory passed as `ptr`.
   - **The other:** declare `record SigSet tag sigset_t` with fields. Its layout differs by platform (a struct of `unsigned long[16]` on glibc, a `__uint32_t` on macOS), so it would be refused on one platform or the other.
   - **A fieldless tagged record:** that is a handle, meaning a pointer to `sigset_t`, which `malloc` cannot produce safely. It would most likely be refused.
3. **Width of the constants.**
   - **My choice:** declared `i32`, so they pass straight to `int` parameters.
   - **The other:** `i64` constants are allowed, since a result may be wider, but each call would then need `to_i32(SIGINT).must()`. Passing them without the conversion is refused.
4. **The `sleep` signature.** I declared `seconds: u32 -> u32`, matching `unsigned int`. A signed type or a different width would be refused. The result is `u32`, so it must be discarded with `_ =`; leaving it unused is refused.
5. **Named arguments to `sigprocmask`.** Its two `ptr` parameters share a type, so names are required. Leaving them out is refused.
6. **Using the list.** Writing to a cell is not a use, so I read `texts` through `texts.len()` in the final message. If the program never read it, `texts` would be an unused binding and the compiler would refuse it. Using `n` in the message instead would also be correct, but only if `texts` were read somewhere else.
7. **Checking the C results.**
   - **My choice:** bind each result to a name, then `assert` the name. The calls run whether or not asserts are kept.
   - **The other:** put the call inside the `assert`. If a build dropped asserts (the spec doesn't say whether one does), the mask would never be set. Ctrl-C would then kill the process with status 130. That runs but gives the wrong answer.
8. **Ending with code 0.** I let `main` return. `exit(code: 0)` is refused, because the code is written as a number.
9. **When the check happens.** I check after the print, so a stop always lets the current step finish. Checking before building the next step amounts to the same thing. Checking before the print would end mid-step, which is the wrong answer.
10. **Comparing `sigismember` with `== 1`.** I used `== 1`. Using `!= 0` would also treat its error value of −1 as "asked to stop", which here makes no difference.
11. **Order of the `extern` groups.** No group needs another's names, so the order doesn't matter. I added no `_GNU_SOURCE` and no `link`, assuming libc is linked by default. If it weren't, linking would fail.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it.
- **Output with `kill -INT` at about 2.5 s:**
  1. `step 1` at about 1 s.
  2. `step 2` at about 2 s.
  3. The signal arrives during the third `sleep`. It is blocked, so the sleep is not interrupted, and `step 3` prints at about 3 s.
  4. The `sigpending` check then sees SIGINT, so the program prints `stopped after 3 steps` and exits with code 0.
  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```
- **Ctrl-C or `kill -TERM`:** both behave the same way, each finishing its current step.

## context

Only this directory's `brief.md`, `task.md` and `heroes-spec.md` reached my context, plus the working-directory path and the date. No other instruction, rule, contract, memory or document did.
````

### t1-s-1 (`14aeb3`, from `result`)

````markdown
I couldn't create `report.md` because I only have a file-reading tool here, so the whole report follows as my answer, as the brief allows when the file can't be written.

# report.md

## answer

**What the task asks.** A program, `main.hero`, that runs numbered steps until it is asked to stop. Each step builds `step N`, adds it to a list, sleeps one second through C's `sleep` and prints the text. Ctrl-C, `kill -INT` or `kill -TERM` makes it finish the current step, print `stopped after N steps` and exit with code 0. The task also asks whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 seconds after starting.

**How it works.** The language has no mutable globals, so the signal handler can't set a flag that the program owns. The specification guarantees that a function given to `signal` "runs after the signal, never inside it: on the thread the signal interrupted, once that thread is back in the program's own code." That makes it safe for the handler to call ordinary C functions. The handler sets an environment variable with `setenv`, which is state that C holds, and the loop checks it with `getenv` before each step. A step that has started always runs to the end.

The program uses no package, so it has no `heroes.toml`. It is one file:

### `main.hero`

```
# C's signal installer and the two stop signals.
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

# C's sleep: waits whole seconds, gives back the seconds left unslept.
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# The process environment, used as the stop flag the signal handler sets.
extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

# Runs after SIGINT or SIGTERM, outside the signal itself: records the request to stop.
function on_stop(_: i32)
    _ = setenv(name: "HEROES_STEPS_STOP".cstr(), value: "1".cstr(), overwrite: 1)

# Whether a stop has been requested since the program started.
function stop_requested() -> bool
    return getenv("HEROES_STEPS_STOP".cstr()) != nullptr

# Waits one whole second, sleeping again for whatever a signal cut short.
function wait_one_second()
    left @= sleep(1)
    while left > 0
        left @ sleep(left)

function main()
    _ = unsetenv("HEROES_STEPS_STOP".cstr())
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    while !stop_requested()
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        wait_one_second()
        print(text)
    print(f"stopped after {n} steps")
```

## choice_points

1. **How the handler tells the loop to stop.** The specification has no mutable globals and gives no way for the handler to reach a local cell.
   - **My choice:** use the environment through `setenv` and `getenv`.
   - **Alternative: a pipe.** The handler writes to a pipe whose descriptor number is guessed (3 or 4), and the loop polls it. The program runs, but it is fragile: if the guessed descriptor is wrong, it never stops cleanly.
   - **Alternative: blocking the signals.** Block them with `sigprocmask` and poll `sigpending`. This needs `sigset_t`, which is a struct on Linux and an integer on macOS, so the declaration would be refused by clang on one platform or the other.
   - **Alternative: a loop with no stop check.** The default action kills the program mid-step with exit code 130 and no final line. That is the wrong answer.

2. **The result type of `signal`.** C's `signal` returns a function pointer, and the specification says a callback is never a result.
   - **My choice:** `-> ptr`, thrown away with `_ =`. Both sides are pointers, and "a result may be wider than C's".
   - **Alternative: no `->` at all.** C's result would then be declared `void`. clang checks result types, so this risks being refused.
   - **Alternative: a function-typed result.** The specification forbids it, so the compiler refuses it.
   - **Risk:** if clang won't treat a function pointer as `ptr`, the build is refused at this line.

3. **The handler's parameter.** Its name is `_`, because an unused parameter is a compile error. A named parameter that is never read would be refused.

4. **The type of the C declarations.** `SIGINT`/`SIGTERM` are `i32` and the handler takes `i32`, because C says `int`. `sleep` takes and returns `u32`, because C says `unsigned int`.
   - Declaring the parameters `i64` would be refused, because a parameter must match C's width and sign.
   - Declaring the constants `i64` would make `signal(SIGINT, …)` fail to type-check, because there are no implicit conversions.

5. **Labels on `setenv`.** Its two `cstr` parameters share a type, so I name every argument. Leaving the labels off would be refused, because named arguments are mandatory when two parameters share a type.

6. **What "finishes the step in hand" means for the sleep.** A signal cuts `sleep` short.
   - **My choice:** sleep again for whatever was left, so the full wait completes before the step prints.
   - **Alternative: a single `sleep(1)`.** It prints the step as soon as the signal lands. The output is the same, only sooner.
   - **Side effect of my choice:** `sleep` rounds the time left, so the re-sleep can wait up to a whole extra second, or none. The stop line can arrive at about 2.5 s or about 3.5 s.

7. **Ending with code 0.** The specification says `exit` refuses a code "written as a number". I read that as possibly refusing `exit(code: 0)` outright, though it may only mean out-of-range literals.
   - **My choice:** let `main` return normally, which by convention exits 0. The specification never states that exit code, so this is an assumption.
   - **Alternative: `exit(code: 0)`.** It is either accepted with the same result or refused at compile time.

8. **Clearing the flag at start-up.** I call `unsetenv` first. Without it, a variable left in the inherited environment would make the program print `stopped after 0 steps` at once.

9. **The count in the last line.** I use the counter `n`, the number of steps started. Every step that starts also prints, so it equals the number printed. `texts.len()` would give the same number, but the specification doesn't say what type `len` returns.

10. **Whether `texts` counts as used.** It is only written to, through `texts @ texts.push(text)`. I rely on the read inside `push` counting as a use. If the compiler doesn't count it, it refuses the program as having an unused binding, and the fix would be to print `texts.len()` at the end.

## prediction

I expect the compiler to accept the program, with some risk at choice point 2 (`signal` declared `-> ptr`) and a small risk at choice point 10.

Expected timeline when sent `kill -INT` at about 2.5 s:

- **t = 1 s:** prints `step 1`.
- **t = 2 s:** prints `step 2`.
- **About 2.5 s:** the signal cuts short step 3's sleep. The handler runs once the program is back in its own code and sets the flag.
- **About 3 s:** the re-sleep finishes. It could be about 2.5 s or 3.5 s because of `sleep`'s rounding. The program prints `step 3`.
- **Then:** the loop's check sees the flag, the program prints the last line and returns from `main`.

Output:

```
step 1
step 2
step 3
stopped after 3 steps
```

The exit code is 0. `kill -TERM` and Ctrl-C give the same result.

## context

Only the files in this directory reached my context: `brief.md`, `task.md` and `heroes-spec.md`. Apart from those I had only the general system setup: the working directory, the date, and the fact that I am an agent with a file-reading tool. No other rule, contract, memory or project document reached my context.
````

### t2-x-1 (`3b5cc9`, from `result`)

````markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief says to do in that case, the whole report follows.

# report.md

## answer

The task: `main.hero` builds the texts `item 1` … `item 1000000` into a fresh list twenty times, then prints `built 20000000 items`. It counts the SIGUSR1 signals it receives during the work, prints `received M signals`, never crashes from a signal, and exits with code 0.

The specification has no signal feature, so the program reaches C through the FFI (section 13). It avoids a C signal handler entirely: a callback can't safely change a counter, because there are no mutable globals and a handler can't return state. Instead:

1. **Block the signals first.** `main` blocks SIGUSR1 and SIGUSR2 in its own thread with `pthread_sigmask`. Only after that does it start a second thread with `hero_thread_spawn`, so that thread inherits the block. With the signal blocked, SIGUSR1's default action (killing the process) can never happen. Signals sent at any rate just become pending.
2. **Count in a second thread.** That thread loops on `sigwait` and counts every SIGUSR1 handed to it.
3. **Stop the counter.** When the twenty rounds are done, `main` sends itself SIGUSR2 with `kill(getpid(), SIGUSR2)`. On Linux, `sigwait` hands back the lower-numbered SIGUSR1 (10) before SIGUSR2 (12), so a SIGUSR1 that is already pending gets counted before the stop.
4. **Collect the count.** The counting thread returns its count, which `main` reads with `hero_thread_join`.
5. **Ignore late signals.** A SIGUSR1 that arrives after that stays pending and blocked, and the process exits normally with code 0.

### `main.hero`

```
extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signo: i32) -> i32
    function pthread_sigmask(how: i32, set: sigset_t lent, old: ptr) -> i32
    function sigwait(set: sigset_t lent, @sig: i32) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "unistd.h"
    function getpid() -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The set of the two signals this program waits for: SIGUSR1, which it
# counts, and SIGUSR2, which it sends itself to stop the counting.
function signal_set() -> sigset_t
    set @= sigset_t(rest: zero)
    _ = sigemptyset(@set)
    _ = sigaddset(@set, signo: SIGUSR1)
    _ = sigaddset(@set, signo: SIGUSR2)
    return set

# Runs in its own thread: takes each SIGUSR1 as it arrives and counts it,
# until SIGUSR2 arrives; answers the count.
function count_signals(_: i64) -> i64
    set = signal_set()
    count @= 0
    while true
        sig: i32 @= 0
        if sigwait(set, @sig) != 0
            continue
        if sig == SIGUSR2
            return count
        if sig == SIGUSR1
            count @ count + 1

function main()
    set = signal_set()
    blocked = pthread_sigmask(how: SIG_BLOCK, set: set, old: nullptr)
    if blocked != 0
        print("cannot block SIGUSR1")
        exit(code: 1)
    waiter = hero_thread_spawn(body: count_signals, arg: 0)
    total @= 0
    round @= 0
    while round < 20
        xs: [str] @= []
        i @= 1
        while i <= 1000000
            xs @ xs.push(f"item {i}")
            total @ total + 1
            i @ i + 1
        round @ round + 1
    _ = kill(pid: getpid(), sig: SIGUSR2)
    received = hero_thread_join(handle: waiter)
    print(f"built {total} items")
    print(f"received {received} signals")
```

No other file is needed. No `heroes.toml` is needed either, because the program uses no package.

## choice_points

1. **How to catch the signal: block it and use `sigwait` in a thread, or install a handler.**
   - Chosen: `pthread_sigmask` plus `sigwait`.
   - Alternative: `signal(SIGUSR1, handler)`. `signal` returns a function pointer, and the spec says a callback is never a result, so that declaration is refused. Declaring the result as `ptr` instead is likely refused too, since clang checks result types.
   - Alternative: `sigaction`. Its handler field is a function pointer, and a record field may not be a function value, so that is refused.
   - Even if a handler compiled, it has no place to count (no mutable globals). It would also run Heroes code inside a signal handler, which risks a crash at run time.

2. **How to stop the counting thread: self-sent SIGUSR2.**
   - Nothing but one `i64` crosses between threads, so there is no shared flag.
   - Alternative: count SIGUSR1 itself as the stop signal. The thread would stop at the first external signal, giving the wrong answer.
   - Alternative: `sigtimedwait` polling. It still has no way to learn that `main` has finished.
   - Risk: an outside process that sends SIGUSR2 would stop the count early. The task only promises SIGUSR1.

3. **Exact counting versus coalescing.** Standard signals merge while pending, so M counts *deliveries*. This can be less than the number of `kill -USR1` calls when they come fast. The spec offers no way around this; real-time signals would need the senders to change.

4. **When to block: first statement of `main`.** A SIGUSR1 that arrives before `main` runs (during runtime start-up) still has its default action and kills the process. Nothing in the spec lets me close that window.

5. **The spawned thread inherits the mask.** I assume `hero_thread_spawn` uses pthreads, so the new thread inherits the block. If the runtime has some other thread with SIGUSR1 unblocked, a signal sent to it would kill the process (exit code 138).

6. **How to declare `sigset_t`: `record sigset_t partial` with no fields, built with `rest: zero`.**
   - This is Linux/glibc: there `sigset_t` is a typedef of an unnamed struct, so it gets the same name and no `tag`.
   - On macOS, `sigset_t` is an integer, so the compiler refuses this record.
   - Adding `tag` would also be refused: a record with a tag and no fields is a handle (a pointer), and anyway the struct has no tag.

7. **`oldset` declared as `ptr` and passed `nullptr`.** The spec lets a `ptr` stand for any pointer. The alternative, `@old: sigset_t`, also works, but it needs an extra cell that serves no purpose.

8. **Constants declared `i32`, matching C's `int` exactly.** Declaring them `i64` would force `to_i32(...).must()` at every call. Passing an `i64` straight to an `i32` parameter is refused.

9. **Named arguments.**
   - `kill` has two `i32` parameters, so `pid:` and `sig:` are mandatory. Leaving them off is refused.
   - Elsewhere I wrote names for clarity, except `sigaddset(@set, signo: …)` and `sigwait(set, @sig)`, where every parameter has a different type.

10. **Counting `total` by hand instead of using `xs.len()`.** The spec never gives the result type of `len`. If it is `u64`, then `total + xs.len()` with an `i64` `total` is refused. My counter is an `i64` and avoids that question.

11. **`while` loops instead of `for _ in range(...)`.** It's unclear whether `_` may be a `for` variable (the grammar says `ident`). A named round variable that is never read would be an unused binding and refused. `while` also avoids building a list of a million `i64` each round.

12. **Ignoring the results of `sigemptyset`, `sigaddset` and `kill` with `_ =`.** They are not fallible, so `_` is allowed. If I bound them to names and never read them, the compiler would refuse them as unused.

13. **`sigwait` failing returns to the loop (`continue`).** On Linux it does not fail with this set. If it did fail without that check, the program would read whatever value `sig` held.

14. **Print order: both lines after `join`.** The task's output order is unchanged. Printing `built` before stopping the counter would also work, but signals arriving during that print would then be counted. That counts as "while it works" either way.

15. **Group order: `signal.h`, then `unistd.h`, then `hero_os.h`.** None needs another's names, and none needs `_GNU_SOURCE`, so any order works.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it on Linux (glibc). It would refuse it on macOS, because of the `sigset_t` record.
- **Output when run:**
  ```
  built 20000000 items
  received M signals
  ```
  - M is the number of SIGUSR1 deliveries during the run.
  - It is 0 if none are sent.
  - It is at most the number sent; it can be less when signals come faster than the counting thread takes them.
- **Exit code:** 0, however many SIGUSR1 come, as long as they arrive after the first line of `main`. One sent during runtime start-up, before the block, would end the process (choice point 4).

## context

- The only files I read were `brief.md`, `task.md` and `heroes-spec.md` in this directory.
- My system prompt identified me as an agent built on the Claude Agent SDK and gave the working directory path and the date. It carried no language rules.
- No other instruction, contract, memory or document about Heroes reached my context.
````

### t2-r-1 (`304e74`, from `result`)

````markdown
I couldn't save `report.md` because I have no tool that writes files. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

The program has two files: `main.hero` and a C header, `sigcount.h`, placed beside it. The specification (section 13) says a function handed to `signal` stops the program the first time a signal calls it, and that the handler must be written "in C, in a header the program ships". So the header installs a SIGUSR1 handler with `sigaction` and adds to a lock-free atomic counter. The Heroes code declares the header's two functions in an `extern` group, installs the handler first, does the work and then reads the counter.

### `sigcount.h`
```c
#ifndef SIGCOUNT_H
#define SIGCOUNT_H

#include <signal.h>
#include <stdatomic.h>
#include <string.h>

_Static_assert(ATOMIC_LLONG_LOCK_FREE == 2,
               "sigcount needs a lock-free long long to be async-signal-safe");

static atomic_llong sigcount_value = 0;

static void sigcount_handler(int signo)
{
    (void)signo;
    atomic_fetch_add_explicit(&sigcount_value, 1, memory_order_relaxed);
}

/* Installs the SIGUSR1 handler. Returns 0 on success, -1 on failure. */
static int sigcount_install(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sigcount_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    return sigaction(SIGUSR1, &sa, NULL);
}

/* How many SIGUSR1 signals the handler has run for so far. */
static long long sigcount_count(void)
{
    return atomic_load_explicit(&sigcount_value, memory_order_relaxed);
}

#endif
```

### `main.hero`
```
extern "sigcount.h"
    function sigcount_install() -> i32
    function sigcount_count() -> i64

function main()
    status = sigcount_install()
    if status != 0
        print("could not install the SIGUSR1 handler")
        exit(code: 1)
    built @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            built @ built + 1
            i @ i + 1
        round @ round + 1
    received = sigcount_count()
    print(f"built {built} items")
    print(f"received {received} signals")
```

There is no `heroes.toml`, because the program uses no package (section 1).

## choice_points

1. **Where the handler lives.** I wrote it in C in a shipped header, as section 13 says. The other choice is to pass a Heroes function to `signal` through FFI. That compiles, but the program stops at the first SIGUSR1, so it fails the task.
2. **`sigaction` instead of C's `signal()`.** In strict ISO mode, glibc's `signal()` resets the handler to the default after the first delivery. The second signal would then kill the process: it stops with an error when run. `sigaction` keeps the handler installed. `SA_RESTART` keeps system calls, such as the runtime's writes for `print`, from failing with EINTR while signals arrive. Without it, output could break or the program could crash.
3. **Feature-test macros.** I don't define `_POSIX_C_SOURCE` in the header. I assume clang's default `gnu17` mode, where glibc shows `sigaction` and `SA_RESTART`, and macOS always shows them. If the compiler builds with strict `-std=c11` on Linux, `struct sigaction` and `SA_RESTART` are hidden and the compiler refuses the program. Defining the macro in the header would be too late if the generated C includes `<signal.h>` first. It could also change what the runtime's own headers see.
4. **Counter type.** I used a lock-free `atomic_llong`, checked by a `_Static_assert`. The other choice, `volatile sig_atomic_t`, may be 32 bits wide. It works for any realistic number of signals, but in theory it could overflow, and the answer would be wrong.
5. **Header functions are `static` in the header.** The other choice is a separate `.c` file plus `link`. The specification only describes shipping a header, so that route risks a link failure, which means the compiler refuses the program. My choice has its own risk: if the compiler included the header in more than one C translation unit, each copy would have its own counter. That is unlikely for a single module.
6. **Header path.** I wrote `extern "sigcount.h"` and assume the header next to `main.hero` is found. The specification doesn't say which include directories are searched. If the file's directory isn't searched, the compiler refuses the program.
7. **Result widths.** `sigcount_install` returns C `int`, declared `i32`. `sigcount_count` returns `long long`, declared `i64`. Section 13 says a result may be wider than C's, so `i64` for the first would also be accepted.
8. **Loops.** I used `while` loops with counters, not `for _ in range(...)`. The grammar's `for` takes an `ident`, and the specification never says `_` can be a loop variable. A named loop variable that I don't use would be an unused binding, which the compiler refuses. `range(from: 1, to: 1000001)` would also build an extra million-element array each round.
9. **Counting items myself.** I keep a `built` counter instead of adding `items.len()`. The specification never gives `len`'s result type. If it were `u64`, adding it to an `i64` total would be refused, since there are no implicit conversions. `items` is still used, because `items.push(...)` reads it, and it is re-bound, so it isn't an unused cell.
10. **A fresh list each round.** `items: [str] @= []` is declared inside the outer loop, so every round starts with an empty list and the previous one is freed. Declaring it once outside would keep growing to 20 million strings. The printed line would be the same, but memory use would be very large.
11. **When the count is read.** I read it right after the work and before printing, which matches "while it works". Reading it after the first `print` would sometimes give a slightly larger number. The handler stays installed, so signals during printing still can't kill the program.
12. **If installing fails.** The program prints a message and exits with code 1. The task requires exit code 0, but continuing without a handler would let the first signal kill the process.
13. **"Signals it receives".** POSIX merges standard signals that arrive while one is already pending. M is therefore the number of handler runs, which can be fewer than the number of `kill -USR1` calls. Only real-time signals queue, and the task names SIGUSR1.
14. **The startup window.** A SIGUSR1 that arrives before `main` calls `sigcount_install` gets the default action and ends the process. An `__attribute__((constructor))` in the header would make that window shorter but can't remove it. I kept the explicit call.

## prediction

- I expect `heroes build main.hero` to accept the program. Choice points 3 and 6 are the risks.
- With no signals, it prints:
  ```
  built 20000000 items
  received 0 signals
  ```
- With signals, the second line is `received M signals`. M is the number of SIGUSR1 deliveries during the work. It can be lower than the number sent, because pending signals merge.
- It exits with code 0, and it doesn't crash however fast signals arrive once the handler is installed.

## context

My context held my own system prompt: I'm a Claude agent in that working directory, with the date and tool-use rules. It also held the user's message telling me to follow `brief.md`. The only files I read were `brief.md`, `task.md` and `heroes-spec.md` in this directory. No other instruction, rule, contract, memory or document reached my context.
````

### t2-e-1 (`ba6229`, from `result`)

````markdown
I couldn't save `report.md` because I only have a file-reading tool, so the whole report is below as the brief allows. I have not compiled or run the program; the prediction section is my expectation only.

# report.md

## answer

The task asks for `main.hero`. Twenty times it builds the texts `item 1` … `item 1000000` into a fresh list, then prints `built 20000000 items`. While it works it counts the SIGUSR1 signals it receives, then prints `received M signals`. It must never crash and must exit with code 0.

How it works:
- **Catching the signal.** SIGUSR1 is caught through `signal` from `signal.h`. The spec says a function given to `signal` runs on a runtime thread of its own, never inside the signal itself.
- **Counting without globals.** The language has no mutable globals, so the handler cannot keep a count itself. Instead it writes one byte into a pipe whose write end has been moved to a fixed file descriptor, the constant `SIG_FD` (100).
- **Reading the count.** A reader thread started with `hero_thread_spawn` counts the bytes until the pipe closes. `main` closes the pipe when the work is done, and `hero_thread_join` gives back the count.

File `main.hero`:

```
# Builds twenty lists of a million texts and counts the SIGUSR1 signals
# that arrive meanwhile.

extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "unistd.h"
    function pipe(@fds: [i32] counted_by 2 lent) -> i32
    function dup2(oldfd: i32, newfd: i32) -> i32
    function close(fd: i32) -> i32
    function read(fd: i32, @buf: [u8] counted_by count lent, count: u64) -> i64
    function write(fd: i32, @buf: [u8] counted_by count lent, count: u64) -> i64

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The descriptor the signal handler writes one byte to, per signal.
constant SIG_FD: i32
    100

# Runs on the runtime's signal thread, once per SIGUSR1 delivered.
function on_usr1(_: i32)
    buf: [u8] @= [1]
    _ = write(SIG_FD, @buf, 1)

# Counts the bytes read from `fd` until every write end is closed.
function count_bytes(fd: i64) -> i64
    rfd = to_i32(fd).must()
    buf: [u8] @= []
    total @= 0
    while true
        n = read(rfd, @buf, 4096)
        if n == 0
            break
        if n > 0
            total @ total + n
    _ = close(rfd)
    return total

# The texts `item 1` to `item n`, in a fresh list.
function build(n: i64) -> [str]
    items: [str] @= []
    i @= 1
    while i <= n
        items @ items.push(f"item {i}")
        i @ i + 1
    return items

function main()
    _ = signal(SIGUSR1, on_usr1)
    fds: [i32] @= [0, 0]
    rc = pipe(@fds)
    assert rc == 0
    moved = dup2(oldfd: fds[1], newfd: SIG_FD)
    assert moved == SIG_FD
    _ = close(fds[1])
    reader = hero_thread_spawn(body: count_bytes, arg: to_i64(fds[0]).must())
    total @= 0
    round @= 0
    while round < 20
        items = build(1000000)
        total @ total + items.len()
        round @ round + 1
    print(f"built {total} items")
    _ = close(SIG_FD)
    received = hero_thread_join(reader)
    print(f"received {received} signals")
```

## choice_points

1. **Return type of `signal`.** C returns `sighandler_t`, a function pointer, and the spec forbids a callback as a result. I declared `-> ptr` and discard it with `_ =`.
   - Other choice: no `->` at all, so `signal` is a `()` call standing alone. If the result-type check against the header requires a matching or wider result, that `()` would be refused by the compiler.
   - My `ptr` may also be refused if clang will not match `void *` against a function pointer. That would be a compile error, and the spec gives no third option.

2. **Counting without globals.** I used a pipe on a fixed descriptor, read by a joined thread.
   - Other choice: append to a file with `read_file`/`write_file` from the handler. That runs, but each append copies the whole file, which gets quadratic under a flood of signals, and the path has to be fixed too.
   - Other choice: block SIGUSR1 and wait for it on a `sigwait` thread. If any runtime thread leaves SIGUSR1 unblocked, the signal's default action can kill the process (a stop with an error at run time). `sigset_t` also differs between platforms (a struct on Linux, an integer on macOS), so the declaration may not compile.

3. **The value of `SIG_FD`.** I chose 100. Another number works the same unless the runtime is already using that descriptor, in which case `dup2` would close it and the program would misbehave at run time.

4. **Order of setup.** `signal` is installed first, then the pipe. A signal arriving in the few microseconds before `dup2` hits a closed descriptor: the write fails with `EBADF` and that signal goes uncounted. The other order would leave SIGUSR1 at its default action (terminate the process) for longer, risking a crash.

5. **`@` on `write`'s buffer.** C's buffer is `const void *`. I followed the spec's `@md: [u8] counted_by N lent` form. Leaving out the `@` may be refused by the compiler, or it may be equally fine; the spec does not say.

6. **No retry when the handler's `write` fails.** If it is interrupted (`EINTR`), that signal is lost and the count comes out lower. A retry loop would spin forever after `main` closes `SIG_FD`, because every later write fails with `EBADF`.

7. **A failing `read` in the reader thread.** A negative result is treated as "try again". Breaking out instead would undercount if an interrupted `read` ends the loop early.

8. **Loop counters.** I used `while` with a cell for the twenty rounds, not `for r in range(...)`. A `for` variable that is never read is an unused binding, which the compiler refuses, and the grammar's `for` takes an `ident`, so `_` may not be allowed there.

9. **The type of `len()`.** I assumed it gives an `i64`. If it gives a `u64`, adding it to `total` mixes types, which the compiler refuses.

10. **Reading the pipe with an empty array.** `buf` starts as `[]`, and I rely on `counted_by count` padding it with zeros up to 4096 elements. If the padding does not happen, reaching past the array aborts at run time.

11. **`assert` in ordinary code.** I used `assert rc == 0` and `assert moved == SIG_FD` in `main`. The other choice is an `if` followed by `exit(code: 1)`, which would break the exit-code-0 requirement in that failure case anyway.

## prediction

I expect the compiler to accept it with `heroes build main.hero`. The main risks are the result type of `signal` (choice point 1) and the type of `len()` (choice point 9).

When run, it prints:

```
built 20000000 items
received M signals
```

and exits with code 0.

M is the number of times the handler actually runs. Standard signals merge while one is pending, so a fast sender can send more signals than M; M counts the ones delivered. With no sender, M is 0.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context, plus the system prompt (working directory and date). No other instruction, rule, contract, memory or document did.
````

### t2-m-1 (`a19d3a`, from `result`)

````markdown
I couldn't create `report.md` because I have no tool that writes files, so the whole report is below, as the brief allows.

# Report

## answer

The task asks for a Heroes program, `main.hero`, that builds `item 1` to `item 1000000` into a fresh list twenty times and prints `built 20000000 items`. It must also count the SIGUSR1 signals that arrive while it works, print `received M signals`, never crash or stop early, and exit with code 0.

The specification (§13) says a Heroes function may never be a signal handler or be called by one. It also gives no way to point C at a C handler. So the program cannot catch the signal; it has to block it and collect it.

- **Block first.** The first thing `main` does is block SIGUSR1 with `sigprocmask`. After that, a SIGUSR1 can no longer end the process. It only becomes "pending".
- **Collect while working.** Every 100 items, and once more after the last round, the program checks whether SIGUSR1 is pending (`sigpending` and `sigismember`). If it is, `sigwait` takes it off the pending list and the count goes up by one.
- **Signals after the last check.** Any that arrive later stay blocked and pending, and the process still exits normally with code 0.
- **Libraries.** Everything comes from libc's `signal.h`, so no `link` line and no `heroes.toml` are needed.

There is one file.

### `main.hero`

```
# SIGUSR1 handling through libc: block the signal, then collect it by polling.
extern "signal.h"
    constant SIGUSR1: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signum: i32) -> i32
    function sigprocmask(how: i32, set: sigset_t lent, @oldset: sigset_t) -> i32
    function sigpending(@set: sigset_t) -> i32
    function sigismember(set: sigset_t lent, signum: i32) -> i32
    function sigwait(set: sigset_t lent, @sig: i32) -> i32

# Takes every SIGUSR1 now pending and says how many it took.
function drain(set: sigset_t lent) -> i64
    pending: sigset_t @= sigset_t(rest: zero)
    got: i32 @= 0
    n @= 0
    while true
        _ = sigpending(@pending)
        if sigismember(pending, SIGUSR1) != 1
            return n
        _ = sigwait(set, @got)
        if got == SIGUSR1
            n @ n + 1

function main()
    set: sigset_t @= sigset_t(rest: zero)
    old: sigset_t @= sigset_t(rest: zero)
    _ = sigemptyset(@set)
    _ = sigaddset(@set, SIGUSR1)
    rc = sigprocmask(how: SIG_BLOCK, set: set, oldset: @old)
    assert rc == 0
    received @= 0
    total @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            if i % 100 == 0
                received @ received + drain(set)
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    received @ received + drain(set)
    print(f"built {total} items")
    print(f"received {received} signals")
```

## choice_points

1. **How to count signals.** §13 forbids a Heroes callback as a signal handler. So I blocked the signal and polled for it.
   - Using `signal`/`sigaction` with a Heroes function would be refused by the compiler.
   - Ignoring the signal (`SIG_IGN`) would run, but always print `received 0 signals`, which is the wrong answer.
   - Doing nothing would let the first SIGUSR1 kill the process, a run-time stop.
2. **Which platform's `sigset_t`.** I wrote `record sigset_t partial` with no fields. That matches glibc on Linux, where `sigset_t` is a typedef of an unnamed struct. On macOS `sigset_t` is a `__uint32_t`, not a struct, so I expect the compiler (clang) to refuse it there. The other option was `sigset_t: u32`-style integer parameters, which works on macOS but is refused on Linux. The specification gives no way to cover both. I chose Linux.
3. **Header and library.** I used libc's `signal.h` with no `link`. The specification says `hero_os.h` covers "threads, processes and files" but lists no signal functions in it. Naming `hero_os.h` would probably be refused for missing names.
4. **Types of the constants.** I declared `SIGUSR1` and `SIG_BLOCK` as `i32`, the width of C's `int`. The specification's example declares an `int` constant as `i64`, which may also be accepted. But then every call would need `SIGUSR1.to_i32().must()`. Passing an `i64` constant straight to an `i32` parameter would be refused, because there are no implicit conversions.
5. **Pointer parameters.** For `const sigset_t *` I used `lent` without `@` (§13: a pointer to const). For `sigset_t *` and `int *` out-parameters I used `@`.
   - Declaring a const pointer without `lent` would be read as passing the struct by value, or as C keeping the pointer, and refused against the header.
   - Passing `nullptr` for `oldset` is impossible with an `@` record parameter, so I pass a dummy cell, `old`. It is only written. I relied on the rule that "a write through an @ parameter" counts as a use. If the compiler disagreed, it would refuse the program for an unused binding.
6. **Named arguments.** `sigprocmask` has two `sigset_t` parameters, so I named every argument (`how:`, `set:`, `oldset: @old`). Leaving them positional would be refused.
7. **How to collect.** I used `sigpending` + `sigismember` + `sigwait`. The alternative was `sigtimedwait` with a zero timeout, which needs `siginfo_t` and `timespec` records. That is more surface for the compiler to refuse, but would count the same.
8. **How often to check: every 100 items.** Ordinary signals are not queued: while one SIGUSR1 is pending, more of them merge into it.
   - Checking less often counts fewer of a fast flood. The program still runs, but M is lower.
   - Checking every item costs about 40 million extra system calls, which is slower but counts a little more.
   - No choice gives an exact count of the `kill` calls sent. Even a C handler loses signals that merge this way.
9. **Results of the C calls.** I discard most results with `_ =` (they are `i32`, not fallible, so `_` is allowed). I check `sigprocmask` with `assert`. Writing such a call without `_ =` would be refused.
10. **Loops.** I used `while` with counters rather than `for x in range(...)`. `range` would build a million-element `i64` array each round. Both run and give the same output; `while` uses less memory.
11. **Type of `len()`.** The specification never states it. I assumed `i64`, since `total` is an `i64` that starts from the literal `0`. If `len` returns `u64`, then `total + items.len()` would be refused, and the line would need `total` typed as `u64` or a `to_i64(...).must()`.
12. **Output format.** I used f-strings (`f"built {total} items"`). `print("built ", total, " items")` would print the same.
13. **Remaining risks I cannot rule out.**
    - A SIGUSR1 that arrives before `main` blocks it, during runtime start-up, still kills the process.
    - If the Heroes runtime runs threads of its own, they don't block SIGUSR1, so a signal sent to one of them would kill the process. The specification says nothing about runtime threads.

## prediction

- **Linux, glibc, x86-64:** I expect `heroes build main.hero` to accept the program. Run, it prints:
  ```
  built 20000000 items
  received M signals
  ```
  and exits with code 0.
  - M is the number of SIGUSR1 signals collected: 0 if none were sent.
  - If signals are sent slowly (further apart than the time to build 100 items), M equals the number sent.
  - Under a fast flood, M is lower than the number sent, because pending signals merge.
- **macOS:** I expect the compiler to refuse `record sigset_t partial`, because `sigset_t` there is an integer typedef, not a struct.
- **Least certain parts:**
  - the record with no fields and `partial`, and building it with `rest: zero` alone;
  - the `i64` result of `len()`;
  - that the write-only `old` cell counts as used.

## context

No instruction, rule, contract, memory or document reached my context besides `brief.md`, `task.md` and `heroes-spec.md` from this directory. The only exception is the agent's system prompt: tool use, the working directory and the date. I also tried to read `report.md` to check whether it already existed. It does not.
````

### t2-s-1 (`8b4305`, from `result`)

````markdown
I only have a file-reading tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for `main.hero`. Twenty times, it builds the texts `item 1` to `item 1000000` into a fresh list. It then prints `built 20000000 items` and `received M signals`, where M is the number of SIGUSR1 signals it received while working. It must exit with code 0.

**How the design follows from the spec.** Each step below is forced by a rule in the spec:
- **Catching the signal:** the only way is the FFI. §13 says a function given to `signal` "runs after the signal, never inside it", on the interrupted thread, "once that thread is back in the program's own code". So the handler is ordinary Heroes code and can safely call C.
- **Where the count lives:** Heroes has "no mutable globals" (§4) and "no anonymous functions" (§9). A callback must be a top-level function value, so it can't hold a counter of its own.
- **The solution:** the count lives in the process environment. The handler reads it with `getenv`, adds 1, and writes it back with `setenv`. `main` reads it once at the end.

**File: `main.hero`**
```
# Counts SIGUSR1 signals while building twenty lists of a million texts.

extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "stdlib.h"
    function getenv(name: cstr lent) -> cstr
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32

# Name of the environment variable that holds the signal count.
constant COUNT_VAR: str
    "HEROES_SIGUSR1_COUNT"

# Value of one decimal digit; the count only ever holds digits.
function digit_value(c: str) -> i64
    return match c
        "1" => 1
        "2" => 2
        "3" => 3
        "4" => 4
        "5" => 5
        "6" => 6
        "7" => 7
        "8" => 8
        "9" => 9
        _ => 0

# Reads a non-negative decimal number.
function parse_count(text: str) -> i64
    n @= 0
    for c in text.chars()
        n @ n * 10 + digit_value(c)
    return n

# The count stored so far; 0 when the variable is unset.
function read_count() -> i64
    name = COUNT_VAR
    text = getenv(name.cstr()).validated().default("0")
    return parse_count(text)

# Runs after each SIGUSR1, in the program's own code, never inside the signal.
function on_usr1(_: i32)
    count = read_count() + 1
    name = COUNT_VAR
    value = to_str(count)
    _ = setenv(name: name.cstr(), value: value.cstr(), overwrite: 1)

function main()
    name = COUNT_VAR
    zero = "0"
    _ = setenv(name: name.cstr(), value: zero.cstr(), overwrite: 1)
    _ = signal(SIGUSR1, on_usr1)
    total @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        for i in range(from: 1, to: 1_000_001)
            items @ items.push(f"item {i}")
            total @ total + 1
        round @ round + 1
    print(f"built {total} items")
    print(f"received {read_count()} signals")
```

No other files are needed. The program uses no package, so there is no `heroes.toml` (§1).

## choice_points

1. **Where the count is stored.** I used the environment through `getenv`/`setenv`. The spec gives a handler no state of its own: there are no mutable globals and no closures.
   - Alternative A, a pipe written to at a fixed descriptor: on Linux the pipe fills at about 64 KiB of unread bytes. After that the handler either blocks forever, so the program hangs, or with `O_NONBLOCK` drops counts, so the answer is wrong.
   - Alternative B, a lengthening string such as "x" + previous: correct, but the total work grows with the square of the signal count.
   - Trying to keep a Heroes-side counter: the compiler refuses it.

2. **Using `signal` rather than `sigaction`.** The spec names `signal` explicitly. `sigaction` would need a union-bearing `record ... partial` and `rest: zero`, which are more places to go wrong.

3. **The result type of `signal`.** C returns a function pointer, but the spec says "a callback is a parameter, never a result". So I declared the result as `-> ptr` and discarded it with `_ =`.
   - If clang's check against the header rejects `ptr` for `void (*)(int)`, the compiler refuses the program. The spec doesn't settle this. It's my main uncertainty.
   - Declaring a function-typed result is explicitly refused.

4. **`SIGUSR1` as `i32` rather than `i64`.** It matches `signal`'s `int` parameter. With `i64`, passing it as `sig: i32` would be refused (there are no implicit conversions), unless I also changed the parameter to `i64`. That would also be refused, because a parameter must have C's width.

5. **The callback's parameter is named `_`.** A named, unused parameter is a compile error (§5).

6. **Counting with my own `i64` `total` rather than `items.len()`.** The spec doesn't give the result type of `len`. If it were `u64`, `total + items.len()` would be refused. For the same reason, I loop over `text.chars()` and match strings instead of indexing `text[i]` with an `i64`.

7. **A `while` round counter rather than `for _ in range(...)`.** The grammar writes `for ident`, and it's unclear whether `_` is allowed there. A named but unused loop variable is a compile error.

8. **`setenv` arguments are named.** `name` and `value` share the type `cstr`, so naming them is mandatory (§9). Positional arguments would be refused.

9. **Handling a null `getenv`.** I used `.validated().default("0")`. Using `.must()` would stop the program with an error if the variable were ever unset. That can't happen here, because `main` sets it to "0" first.

10. **The handler is never uninstalled.** Signals arriving after the final read still run the handler harmlessly. Restoring the default handler would let a late signal kill the process, giving a non-zero exit. Ignoring the signal would need the `SIG_IGN` macro as a function value, which the FFI can't express.

11. **Startup race (it can't be removed).** A SIGUSR1 that arrives before `signal(...)` runs gets the default action and kills the process. Any language has this.

12. **Merged signals.** POSIX merges standard signals that are still pending, and the spec doesn't say whether the deferred callback runs once per signal or once per delivery. I count callback runs. That equals the signals the process received, which can be fewer than the number sent.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it. The one real risk is choice point 3, the `-> ptr` result of `signal` checked against `signal.h`. If that is refused, the compiler rejects the program.
- **Output:** a run with no signals prints
  ```
  built 20000000 items
  received 0 signals
  ```
- **With signals:** if signals are sent with `kill -USR1 <pid>` during the run, the second line is `received M signals`. M is the number of SIGUSR1 deliveries the process saw. It is at most the number sent, because pending signals merge.
- **Exit code:** 0. The program doesn't crash or stop early under any signal rate after the handler is installed.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. There was also the system framing that I am a Claude agent working in this directory. No other instruction, rule, contract, memory or document reached it.
````

### t2-s-2 (`47763b`, from `result`)

````markdown
I have no tool that writes files, so I could not create `report.md`. The whole report follows here instead, as the brief allows.

# report.md

## answer

The task asks for `main.hero`. The program installs a SIGUSR1 handler first. It then builds a fresh list of the texts `item 1` … `item 1000000` twenty times and prints `built 20000000 items` followed by `received M signals`. The language has no mutable globals and no closures, so a top-level handler cannot reach any variable of `main`. The count is therefore kept in state that C holds: the process environment variable `HEROES_SIGUSR1_COUNT`, read with `getenv` and written with `setenv`. The specification says a function given to `signal` runs "after the signal, never inside it … once that thread is back in the program's own code". That means the handler can safely use ordinary Heroes code and libc calls.

### `main.hero`

```
# Counts SIGUSR1 signals while building twenty lists of a million texts.

extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "stdlib.h"
    function getenv(name: cstr lent) -> cstr
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32

# The environment variable that holds the running count.
constant COUNT_VAR: str
    "HEROES_SIGUSR1_COUNT"

# Reads a decimal count written by `to_str`; anything empty reads as 0.
function parse_count(text: str) -> i64
    n @= 0
    i @= 0
    while i < text.len()
        n @ n * 10 + (to_i64(text[i]).must() - '0')
        i @ i + 1
    return n

# The count stored so far.
function current_count() -> i64
    seen = getenv(name: COUNT_VAR.cstr()).validated().default("0")
    return parse_count(seen)

# Runs after each SIGUSR1, outside the signal itself.
function on_usr1(_: i32)
    next = current_count() + 1
    _ = setenv(name: COUNT_VAR.cstr(), value: to_str(next).cstr(), overwrite: 1)

function main()
    _ = setenv(name: COUNT_VAR.cstr(), value: "0".cstr(), overwrite: 1)
    _ = signal(sig: SIGUSR1, handler: on_usr1)
    total @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    print(f"built {total} items")
    count = current_count()
    print(f"received {count} signals")
```

Build with `heroes build main.hero`. No other file is needed, and no `heroes.toml` is needed because the program uses no package.

## choice_points

1. **Where the count lives.** The spec has no mutable globals and no anonymous functions or closures, and a callback cannot take an `@` parameter. So the handler cannot reach a cell in `main`. I chose state held by C: the environment, through `getenv` and `setenv`.
   - **Writing to a file instead** would also work, but it is slower and needs a file path.
   - **Using a cell in `main`** is impossible: the compiler refuses it, because the handler has nothing in scope to name.
   - **Appending one character per signal and measuring the length at the end** would avoid parsing. But each signal then copies the whole string, so the cost grows with the square of the count. A heavy signal storm would make the run much slower; it would not change the answer.
2. **What `signal` returns.** C returns `void (*)(int)`, and the spec says a callback is never a result. I declared `-> ptr` and threw it away with `_ =`.
   - **Leaving out `->`** (a `void` result) would most likely be refused, because clang checks every result type against the header.
   - **`-> ptr` itself** could also be refused if the checker will not treat a function pointer as `void *`. This is the main risk of a compile error.
3. **`sigaction` instead of `signal`.** I used `signal`, because it is the only signal function the spec describes. Declaring the `struct sigaction` group (with unions and `partial`) would be fragile. If the platform's `signal` reset the handler to its default after one signal (System V style), a second signal would kill the program. I rely on the spec's runtime wording and on BSD/glibc behaviour, which keeps the handler installed.
4. **The type of `SIGUSR1`.** I declared it `i32`, matching C's `int`, so it passes straight to `sig: i32`.
   - **Declaring it `i64`**, as the spec's `SQLITE_OK` example does, would need `to_i32(SIGUSR1).must()` at the call. Passing it as-is would be refused for mixing widths.
5. **Marking the C string parameters `lent`.** `getenv` and `setenv` take `cstr lent`, because `.cstr()` is a lend, and a lend only reaches a parameter declared `lent`. Without `lent`, the compiler refuses the program.
6. **Named arguments for `setenv`.** It has two `cstr` parameters, so naming them is mandatory. Leaving out the names would be refused.
7. **The handler's unused parameter.** I named it `_`. Giving it a name and not using it is a compile error.
8. **Where the variable starts.** `main` sets it to `"0"` before installing the handler, and every read also falls back with `.default("0")`.
   - **Without the fallback**, a `.must()` on a null `getenv` result would stop the program with an error (`null_cstr`).
9. **Installing the handler first.** If a SIGUSR1 arrived before installation, the default action would terminate the process. Installing it later would widen that window.
10. **Loops.** I used `while` with counters instead of `for _ in range(...)`.
    - **`for _ in`** might be refused, because the grammar's `For` takes an `ident` and `_` may not count as one.
    - **`for round in range(...)`** with the name unused would be refused as an unused binding.
    - `range(from: 1, to: 1000001)` would also build a million-element array on each round.
11. **Types of `len` and `s[i]`.** I assumed `len()` gives an `i64`. If it gives a `u64`, then `i < text.len()` and `total + items.len()` are refused for mixed types, and the fix is `to_i64(...).must()`. `s[i]` is a `u8` per the spec, which I convert with `to_i64(...).must()` and compare against `'0'` as an `i64`.
12. **The handler running again inside itself.** If a deferred signal could run the handler between its read and its write, one increment would be lost. The program would still run and stay well-behaved, but M would be one too low. The spec gives no atomic operation and does not say how the runtime delivers deferred signals, so I accepted this risk.
13. **Signals that run together.** POSIX merges several pending SIGUSR1 into one, and the spec does not say whether the runtime queues them. M counts handler runs, which can be fewer than the number of `kill` calls. Neither the task nor the spec says how to count merged signals.

## prediction

I expect the compiler to accept the program, with one main risk: the `-> ptr` result on `signal` (choice point 2). The second risk is if `len()` returns `u64` (choice point 11).

When built and run with no signals sent, it prints:

```
built 20000000 items
received 0 signals
```

It exits with code 0. If N SIGUSR1 signals are sent with `kill -USR1` during the run, the second line is `received M signals`. M equals N when signals are spaced apart. It can be lower when signals arrive faster than the handler runs (choice points 12 and 13). The program does not crash in either case.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document about Heroes did. The only other input was the general system note that I am a Claude agent working in this directory.
````

### t2-m-2 (`36e5b3`, from `result`)

````markdown
I only have a Read tool, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for `main.hero`. It builds the texts `item 1` to `item 1000000` into a fresh list twenty times, then prints `built 20000000 items`. It counts every SIGUSR1 it receives while it works and prints `received M signals` after that line. It must not crash or stop early, and it must exit with code 0.

**What the spec allows.** Section 13 says a callback is "never a result or a signal handler, nor called by one". So the program can't install a handler. Instead it **blocks** SIGUSR1 with `sigprocmask`, which means a SIGUSR1 can no longer kill the process. It then checks for a waiting SIGUSR1 every 1000 items with `sigpending` and `sigismember`, and takes each one with `sigwait`, which returns at once. It does one last check after the twenty rounds. A SIGUSR1 that arrives after that last check stays waiting while blocked and does no harm, so the program still ends with exit code 0.

Each signal set is a 256-byte block from `malloc`, passed as a `ptr`. I did this because section 13 accepts a `ptr` against any C pointer ("except … what a `ptr` points at"), and `sigset_t` is a struct on glibc but a `u32` on macOS.

### `main.hero`

```
# Builds twenty rounds of a million texts and counts the SIGUSR1 signals
# that arrive meanwhile, by blocking the signal and draining it by polling.

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

extern "signal.h"
    constant SIG_BLOCK: i32
    constant SIGUSR1: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function sigprocmask(how: i32, set: ptr, oldset: ptr) -> i32
    function sigpending(set: ptr) -> i32
    function sigismember(set: ptr, signo: i32) -> i32
    function sigwait(set: ptr, @sig: i32) -> i32

# Room for one sigset_t on every platform (glibc: 128 bytes, macOS: 4).
constant SIGSET_BYTES: u64
    256

constant ROUNDS: i64
    20

constant ITEMS: i64
    1000000

constant POLL_EVERY: i64
    1000

# Takes every SIGUSR1 now pending and says how many it took.
function drain(set: ptr, probe: ptr) -> i64
    n @= 0
    going @= true
    while going
        rc = sigpending(probe)
        assert rc == 0
        if sigismember(probe, SIGUSR1) == 1
            got: i32 @= 0
            w = sigwait(set, @got)
            assert w == 0
            assert got == SIGUSR1
            n @ n + 1
        else
            going @ false
    return n

function main()
    set = malloc(SIGSET_BYTES)
    probe = malloc(SIGSET_BYTES)
    assert set != nullptr
    assert probe != nullptr
    e1 = sigemptyset(set)
    assert e1 == 0
    e2 = sigemptyset(probe)
    assert e2 == 0
    a1 = sigaddset(set, SIGUSR1)
    assert a1 == 0
    m1 = sigprocmask(how: SIG_BLOCK, set: set, oldset: nullptr)
    assert m1 == 0
    total @= 0
    received @= 0
    round @= 0
    while round < ROUNDS
        items: [str] @= []
        i @= 1
        while i <= ITEMS
            items @ items.push(f"item {i}")
            if i % POLL_EVERY == 0
                received @ received + drain(set: set, probe: probe)
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    received @ received + drain(set: set, probe: probe)
    print(f"built {total} items")
    print(f"received {received} signals")
    free(set)
    free(probe)
```

## choice_points

1. **How to catch the signal.** The spec forbids a callback as a signal handler. I chose to block SIGUSR1 and drain it by polling.
   - Alternative: install a handler through `signal`/`sigaction`. The compiler refuses it.
   - Alternative: set SIGUSR1 to SIG_IGN. That runs, but it always prints `received 0 signals`, which is the wrong answer.
   - Alternative: leave SIGUSR1 alone. The first `kill -USR1` kills the process, so it stops with an error.

2. **How to represent `sigset_t`.** I used a `ptr` to a `malloc`'d block.
   - Alternative: `record sigset_t partial`. This could work on glibc, where it is a struct. On macOS `sigset_t` is a `u32`, so the compiler would refuse it.
   - **Risk with my choice:** macOS `<signal.h>` also defines `sigemptyset`, `sigaddset` and `sigismember` as macros that dereference the pointer. If the generated C hands them an untyped `void *`, the compiler would refuse it on macOS. On Linux/glibc these are real functions.

3. **One process-wide mask call vs a per-thread one.** I used `sigprocmask`.
   - `pthread_sigmask` would behave the same if the program has a single thread.
   - If the Heroes runtime runs its own threads with SIGUSR1 unblocked, the signal may be delivered to one of them and kill the process. The spec says nothing about runtime threads.
   - There is also a short window at start-up, before the first `sigprocmask`, where a SIGUSR1 kills the process. No program written in the spec can close it.

4. **Polling in the main loop vs a waiter thread.** I chose polling every 1000 items.
   - A thread spawned with `hero_thread_spawn`, sitting in `sigwait` and stopped by a self-sent SIGUSR2, would notice signals sooner. It needs more machinery and a second signal.
   - Polling after every item would be more precise but costs about 20M extra system calls.
   - Polling less often merges more signals into one (see the next point).

5. **What M means.** Standard signals don't queue: several SIGUSR1s sent between two drains reach the process as one. I count deliveries, meaning each time a SIGUSR1 was waiting when I checked.
   - Under a fast flood, M is smaller than the number of `kill` calls. No POSIX program can count those exactly with SIGUSR1.
   - Reading the task as "M equals the number sent" gives a different answer than intended under bursts, and no program can meet that reading.

6. **Ways to make an unused or omitted value impossible to get wrong:**
   - **`got`**: I read it in an `assert` rather than relying on "a write through an `@` parameter is a use". If that rule were read the other way, the compiler would refuse the program.
   - **The loops**: I used `while` counters, not `for round in range(...)` with an unused loop variable, which the compiler refuses. Writing `_` as the `for` variable may not parse, since the grammar asks for an `ident`.
   - **Error handling**: I used `assert` instead of `exit(code: 1)`. The spec says a code "written as a number is refused", so the compiler might refuse `exit(code: 1)`. An assert failure would end the program with an error, but only if a libc call fails, which shouldn't happen.

7. **Named arguments.** `sigprocmask` has two `ptr` parameters, and so does `drain`, so both calls name their arguments. Leaving the names out is refused by the compiler.

8. **Types I assumed:**
   - **Constants as `i32`:** `SIG_BLOCK` and `SIGUSR1` are declared `i32`, to match C's `int` and pass straight to `i32` parameters. Declaring them `i64` would be accepted as a constant ("a result may be wider"), but the compiler would refuse it when passed to an `i32` parameter.
   - **`items.len()` as `i64`:** I assumed it returns `i64`, so `total @= 0` (an `i64`) adds up. If it returned `u64`, the compiler would refuse the program for mixing types.

9. **`malloc` size and the `free` calls.** I used 256 bytes, which covers every platform's `sigset_t`. The `ptr` from `malloc` is not a handle, so `acquires free` can't be written and I call `free` myself.

## prediction

- **Build:** I expect `heroes build main.hero` to accept the program on Linux/glibc. On macOS the compiler might refuse it if the macro forms of `sigemptyset`, `sigaddset` and `sigismember` are used (choice point 2).
- **With no signals sent**, it prints:
  ```
  built 20000000 items
  received 0 signals
  ```
- **With signals sent** while it works, the first line is the same and the second is `received M signals`. M is the number of SIGUSR1 deliveries the kernel recorded. It equals the number of `kill -USR1` calls when they are spaced out, and is fewer under bursts, since signals sent close together merge into one.
- **Exit code:** 0 in both cases. A flood of signals can't kill it once the mask is set. The only remaining risks are a signal in the start-up window, or a runtime thread that doesn't have SIGUSR1 blocked.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The only other things were the system and environment information: the working-directory path and the date. No other instruction, rule, contract, memory or document did.
````

### t2-e-2 (`daaef5`, from `result`)

````markdown
I can't create files: the only tool I have reads files. As the brief allows, here is the whole report as my answer. The file was not written; save the text below as `report.md`.

---

# report.md

## answer

The task asks for `main.hero`. It builds the texts `item 1` … `item 1000000` into a fresh list twenty times, then prints `built 20000000 items`. It counts every SIGUSR1 it receives while it works and prints `received M signals`. It must exit with code 0. No package is used, so there is no `heroes.toml`, and the program is one file.

**The problem.** The language has no mutable globals and no closures. A handler given to `signal` (spec §13) receives only the signal number, so the program can't simply add 1 to a counter.

**How it counts.**
- The handler is a top-level function. For each signal it writes one byte into a pipe.
- The pipe's write end is copied onto a fixed descriptor number, the constant `SIGNAL_FD`. That is how the handler finds it without any shared state.
- A thread started with `hero_thread_spawn` reads the pipe all the time and counts the bytes. Because it keeps draining the pipe, the pipe never fills and the handler never blocks.
- When the work is done, `main` closes the write end. The reader then sees end-of-file and returns its count through `hero_thread_join`.

### `main.hero`

```
# Builds twenty lists of a million texts while counting the SIGUSR1 signals
# the process receives, then reports both.

extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "unistd.h"
    function pipe(@fds: [i32] counted_by 2 lent) -> i32
    function dup2(fildes: i32, fildes2: i32) -> i32
    function close(fd: i32) -> i32
    function write(fd: i32, @buf: [u8] counted_by nbyte lent, nbyte: u64) -> i64
    function read(fd: i32, @buf: [u8] counted_by nbyte lent, nbyte: u64) -> i64

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The descriptor the signal handler writes one byte to per signal.
constant SIGNAL_FD: i32
    200

constant ROUNDS: i64
    20

constant ITEMS: i64
    1000000

# Runs on the runtime's signal thread, once per delivered SIGUSR1.
function on_usr1(_: i32)
    b: [u8] @= [1]
    _ = write(SIGNAL_FD, @b, 1)

# Thread body: counts bytes on the pipe's read end until end of file.
function count_signals(fd: i64) -> i64
    rfd = to_i32(fd).must()
    total @= 0
    buf: [u8] @= []
    while true
        n = read(rfd, @buf, 4096)
        if n == 0
            break
        if n > 0
            total @ total + n
    return total

function main()
    _ = signal(SIGUSR1, on_usr1)
    fds: [i32] @= []
    assert pipe(@fds) == 0
    rfd = fds[0]
    wfd = fds[1]
    assert dup2(fildes: wfd, fildes2: SIGNAL_FD) == SIGNAL_FD
    _ = close(wfd)
    reader = hero_thread_spawn(count_signals, to_i64(rfd).must())
    built @= 0
    round @= 0
    while round < ROUNDS
        items: [str] @= []
        i @= 1
        while i <= ITEMS
            items @ items.push(f"item {i}")
            i @ i + 1
        built @ built + items.len()
        round @ round + 1
    print(f"built {built} items")
    _ = close(SIGNAL_FD)
    received = hero_thread_join(reader)
    _ = close(rfd)
    print(f"received {received} signals")
```

## choice_points

1. **Install a `signal` handler, or block the signal and wait for it.** I used `signal`, because §13 says its function runs on a runtime thread and never inside the signal. The other way blocks SIGUSR1 with `sigprocmask` and has a thread wait in `sigwait`. Runtime threads that started before `main` would still have SIGUSR1 unblocked, and its default action kills the process. That version would stop with an error when run.

2. **How the handler reaches the counter.** I used a pipe whose write end sits on the fixed descriptor `SIGNAL_FD` = 200, with a reader thread counting bytes. The alternatives:
   - A module-level counter is a compile error, because there are no mutable globals.
   - A counter file at a fixed path would run, but it leaves a file behind.
   - A pipe with no reader until the end would fill up (16–64 KiB) under a flood of signals. The handler would block, or with a non-blocking write it would drop counts, giving a wrong M.

3. **Result type of `signal`.** I declared it as `ptr`. C returns a function pointer, and §13 says a callback is never a result. If clang refuses `ptr` against `void (*)(int)`, the compiler refuses the program. The fallback would be `sigaction`.

4. **Type of the `SIGUSR1` constant.** I used `i32` so it passes straight to `sig: i32`. Declaring it `i64` would also be accepted, but each use would need `to_i32(...).must()`.

5. **Passing the byte buffer to `write`.** I passed it as `@buf: [u8] counted_by nbyte lent`, the only array form the spec shows. If a `lent` without `@` is the form meant for `const void *`, the `@` version might still be accepted. Using `cstr` for `const void *` might be refused by the width check, so the compiler might refuse it.

6. **The read buffer starts empty.** §13 says C is handed N elements, "the array's own then zeros", so an empty array becomes 4096 bytes. If that reading is wrong, the program might stop with an error when run (out-of-bounds abort).

7. **Errors from `read`.** A negative result (for example an interrupted call, EINTR) loops again; only 0 (end of file) stops the loop. If negative results also stopped the loop, an interrupted read would end the count early and give a smaller M.

8. **Type of `items.len()`.** I assumed `i64`; §6's example compares `xs.len() == 0`. If it is `u64`, the line `built @ built + items.len()` mixes widths and the compiler refuses it. The fix would be `to_i64(items.len()).must()`.

9. **Unused handler parameter.** I named it `_`. Naming it `sig` without reading it is a compile error.

10. **Loop counters.** I used `while` with cells. `for r in range(...)` would make `r` an unused binding, which is a compile error, unless `_` is allowed as a loop variable, which the grammar (`ident`) leaves unclear.

11. **Arguments to `dup2`.** Its two `i32` parameters share a type, so I named them at the call. Positional arguments are a compile error there.

12. **Declaring the thread functions.** I declared `hero_thread_spawn` and `hero_thread_join` in an `extern "hero_os.h"` group with no `link`. If they were built-ins, the declarations might be refused as duplicates. Leaving them out, when they are not built-ins, is a compile error.

13. **Order of setup.** The handler is installed before the pipe exists. A signal that arrives in that gap writes to a closed descriptor: the count is lost, but nothing crashes. Installing the handler later would let an early signal's default action kill the process.

14. **Signals at the very end.** A signal handled after `main` closes `SIGNAL_FD` is not counted, and its write just fails (EBADF), which is harmless. Counting stops at the end of the work, which matches "while it works".

15. **Fixed descriptor 200.** If something already used descriptor 200, `dup2` would close it silently and the program would misbehave. Nothing else here opens files, so I accept that risk.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program. The main risks are choice points 3, 5 and 8.
- **With no signals sent**, it prints:
  ```
  built 20000000 items
  received 0 signals
  ```
  and exits with code 0.
- **With signals sent:** the second line shows M, the number of SIGUSR1 the process actually received. Standard signals that arrive while one is already pending are merged by the kernel, so M can be lower than the number of `kill -USR1` calls. Under a flood the program does not crash or stop early, and it still exits with code 0.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. My system prompt was there too: a general agent setup giving the working directory and date, with no Heroes-specific rules. No other instructions, memories or documents about this language were present.
````

### t2-r-2 (`e46eba`, from `result`)

````markdown
I only have a file-reading tool, so I could not write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for a program that builds `item 1` … `item 1000000` into a fresh list twenty times, then prints `built 20000000 items`. It must count the SIGUSR1 signals that arrive while it works and print `received M signals`. It must not crash under any number of signals, and it must exit with code 0.

Section 13 of the spec says: "A function given to `signal` stops the program the first time a signal calls it: no code of the program runs inside a signal handler, so write the handler in C, in a header the program ships." So the program is two files. One is a C header that holds the handler and an atomic counter. The other is `main.hero`, which declares that header as an `extern` group.

### `sigcount.h`

```c
#ifndef HEROES_SIGCOUNT_H
#define HEROES_SIGCOUNT_H

/* sigaction needs POSIX visibility under a strict -std=c11. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdatomic.h>

_Static_assert(ATOMIC_LLONG_LOCK_FREE == 2,
               "the signal counter must be lock-free to be touched in a handler");

/* How many times the SIGUSR1 handler has run. */
static atomic_llong sigcount_received;

/* The handler: only a lock-free atomic increment, which is async-signal-safe. */
static void sigcount_on_usr1(int signo)
{
    (void)signo;
    atomic_fetch_add_explicit(&sigcount_received, 1, memory_order_relaxed);
}

/* Installs the handler; 0 on success, -1 on failure (as sigaction). */
static int sigcount_install(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sigcount_on_usr1;
    sigemptyset(&sa.sa_mask);
    /* SA_RESTART: interrupted system calls (the runtime's write, read...) resume
       instead of failing with EINTR. SIGUSR1 itself is blocked while its
       handler runs (no SA_NODEFER), so the handler never nests. */
    sa.sa_flags = SA_RESTART;
    return sigaction(SIGUSR1, &sa, NULL);
}

/* The count so far. */
static int64_t sigcount_read(void)
{
    return (int64_t)atomic_load_explicit(&sigcount_received, memory_order_relaxed);
}

#endif
```

### `main.hero`

```
# The C side: a SIGUSR1 handler that only counts, shipped in sigcount.h.
extern "sigcount.h"
    function sigcount_install() -> i64
    function sigcount_read() -> i64

# Builds "item 1" .. "item 1000000" into a fresh list; returns how many it pushed.
function build_round() -> i64
    items: [str] @= []
    pushed @= 0
    for i in range(from: 1, to: 1000001)
        items @ items.push(f"item {i}")
        pushed @ pushed + 1
    return pushed

function main()
    assert sigcount_install() == 0
    total @= 0
    rounds @= 0
    while rounds < 20
        total @ total + build_round()
        rounds @ rounds + 1
    received = sigcount_read()
    print(f"built {total} items")
    print(f"received {received} signals")
```

Built with `heroes build main.hero`, with `sigcount.h` in the same directory.

## choice_points

1. **Where the handler lives.** I wrote it in C, in a shipped header (`sigcount.h`), as section 13 requires. The alternative was to pass a Heroes function to `signal`/`sigaction` through an extern. The spec says that program stops the first time a signal calls the handler, so it would compile and then die at the first SIGUSR1, with no `received` line and a non-zero exit.
2. **The handler's body.** It only does a lock-free `atomic_fetch_add`. The alternative was a `volatile sig_atomic_t` with `++`. That is a race if the runtime has other threads that can take the signal, and a 32-bit `int` could overflow. In the worst case the program runs and prints a lower count than intended.
3. **`sigaction` with `SA_RESTART`, instead of `signal()`.** With `signal()` (System V semantics on some libcs) or without `SA_RESTART`, the handler may reset to the default action after one delivery, or a runtime `write` may fail with EINTR. The second SIGUSR1 would then kill the process, or `print` would fail: the program stops with an error when run.
4. **The handler stays installed after the work, and I read the count before printing.** The alternative was to restore the default action after the loop. Then a signal that arrives during the `print`s or at exit would kill the process (SIGUSR1's default is to terminate), which is an error when run. "While it works" is the count I take, at the point the loop ends. Signals that arrive later are absorbed but not reported.
5. **The result types are wider than C's.** I declared `sigcount_install` as `-> i64` against C's `int`, and `sigcount_read` as `-> i64` against `int64_t`. The spec allows this ("a result may be wider than C's"). The alternative, `-> i32`, would also be accepted. It would need `== 0` with an `i32` literal, which works the same.
6. **Reacting to a failed install.** I wrote `assert sigcount_install() == 0`, which aborts. The alternative was `exit(code: 1)`. The rule "one written as a number is refused" is ambiguous: it could mean any literal code is refused. On that reading the compiler would refuse the program, so I avoided `exit` entirely.
7. **The loop count.** I used a `while` with a counter cell instead of `for _ in range(...)`. The `For` production asks for an `ident`, and the spec does not say whether `_` is allowed there. A named but unused loop variable would be refused as "an unused binding" (section 5).
8. **Counting items.** I counted pushes in an `i64` cell instead of summing `items.len()`. The spec never says the result type of `len` (`i64` or `u64`). If it is `u64`, `total + items.len()` mixes types and the compiler refuses it (section 7: "never mixed").
9. **A fresh list per round.** It is a local cell in `build_round`, created on each call. The alternative was one cell in `main` reset with `items @ []`. That works too. It builds a fresh list either way.
10. **The header's search path.** I assumed `extern "sigcount.h"` resolves beside `main.hero`, and that a group with no `link` is valid (the grammar marks `link` optional). If the header is searched only on system paths, the compiler (clang) refuses the program with "file not found".
11. **`_POSIX_C_SOURCE`.** I defined it, guarded, before the includes. If the generated C already included `<signal.h>` without it under a strict `-std=c11`, `struct sigaction` would be incomplete and the build would fail. On macOS and on glibc's default gnu mode it compiles either way.
12. **`static` C definitions in the header.** This assumes one translation unit includes the header. If the generated code included it in several, each would get its own counter. The handler would increment one and `sigcount_read` might read another, so the program would run and report 0.

## prediction

I expect the compiler to accept both files with `heroes build main.hero`. The main risk points are choices 10 and 11.

When run, it prints exactly:

```
built 20000000 items
received M signals
```

It exits with code 0. M is the number of times the handler ran during the build.

With no signals sent, M is 0. While signals arrive, every SIGUSR1 that is delivered is counted. Standard signals are not queued, so several sent while one is still pending merge into one delivery, and M can be lower than the number of `kill` calls. M is never higher, and no burst can crash the program.

One unavoidable gap remains: a SIGUSR1 sent before `main` installs the handler, in the first moments of process start-up, still gets the default action. That signal would terminate the program.

## context

No instruction, rule, contract, memory or document other than the files in this directory reached my context. The only inputs were `brief.md`, `task.md` and `heroes-spec.md`. My context also held the agent's system prompt (the tool list, the working directory and the date), which said nothing about Heroes.
````

### t2-x-2 (`e63d43`, from `result`)

````markdown
I couldn't save `report.md` because I can only read files here, so the whole report is below. I haven't built or run the program: the compile and output claims in `prediction` are my expectations, not results.

# report.md

## answer

The specification has nothing for signals, and no mutable global that a signal handler written in Heroes could count into. So the program reaches C's `<signal.h>` and `<pthread.h>` through `extern` groups, and uses the thread functions from `hero_os.h` that section 13 names.

How it works:
1. `main` first blocks SIGUSR1 and SIGUSR2 with `pthread_sigmask`. From then on neither signal can stop or kill the process.
2. It starts a thread with `hero_thread_spawn`. The thread inherits that mask and loops on `sigwait`: each SIGUSR1 adds one to a counter, and SIGUSR2 makes it return the count.
3. `main` does the twenty rounds and prints the `built` line.
4. It sends itself SIGUSR2 with `kill(getpid(), …)` and collects the count with `hero_thread_join`, the thread's single `i64` answer.
5. It prints the count and returns normally, so the exit code is 0. A SIGUSR1 that arrives after the listener has stopped stays pending and blocked, so it never takes effect.

### `main.hero`
```
extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    function sigemptyset(@set: u32 lent) -> i32
    function sigaddset(@set: u32 lent, signo: i32) -> i32
    function sigwait(@set: u32 lent, @sig: i32 lent) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "pthread.h"
    function pthread_sigmask(how: i32, @set: u32 lent, @oset: u32 lent) -> i32

extern "unistd.h"
    function getpid() -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The set holding SIGUSR1 (counted) and SIGUSR2 (the order to stop).
function wait_set() -> u32
    set: u32 @= 0
    _ = sigemptyset(@set)
    _ = sigaddset(@set, SIGUSR1)
    _ = sigaddset(@set, SIGUSR2)
    return set

# Runs on its own thread: counts SIGUSR1 until SIGUSR2 arrives.
function listen(_: i64) -> i64
    set @= wait_set()
    count @= 0
    while true
        sig: i32 @= 0
        _ = sigwait(@set, @sig)
        if sig == SIGUSR2
            return count
        if sig == SIGUSR1
            count @ count + 1

function main()
    mask @= wait_set()
    old: u32 @= 0
    _ = pthread_sigmask(how: SIG_BLOCK, set: @mask, oset: @old)
    listener = hero_thread_spawn(body: listen, arg: 0)
    total @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    print(f"built {total} items")
    _ = kill(pid: getpid(), sig: SIGUSR2)
    received = hero_thread_join(handle: listener)
    print(f"received {received} signals")
```

No other files are needed. There is no `heroes.toml`, because the program uses no package.

## choice_points

1. **How to count signals.** The specification has no signal facility and no mutable globals. A Heroes handler passed to `signal`/`sigaction` would have nowhere to keep a count, and it is unknown whether such a handler is safe to run inside a signal.
   - **Chose:** block the signals, then count them with `sigwait` on a thread whose `i64` result is the count.
   - **Other choice (handler through `signal`):** the compiler refuses it (nothing to count into), or, if it compiles, it may crash when run because the handler isn't safe inside a signal.
2. **Platform layout of `sigset_t`.** I assumed macOS, where `sigset_t` is `__uint32_t`, so the parameters are `@set: u32`.
   - **On Linux:** `sigset_t` is a struct and SIGUSR1 is 10, not 30. clang would refuse the `u32` declarations, so the compiler refuses the program there. Linux needs a group `record` for `__sigset_t` instead.
3. **`lent` on the `@` pointer parameters.** The specification says a parameter is "taken to keep what it is handed unless declared `lent`".
   - **Chose:** `@set: u32 lent` and similar.
   - **Without `lent`:** I expect the compiler refuses passing local cells that C would be taken to keep.
4. **How to stop the counting thread.**
   - **Chose:** send SIGUSR2 to the process with `kill(getpid(), …)`. That needs no `pthread_t`, which can't cross into a Heroes thread.
   - **Other ways:** a polling flag is impossible (no shared mutable state). Not stopping the thread at all means `hero_thread_join` never returns, so the program hangs and never prints line 2.
5. **Blocking with `pthread_sigmask`, not `sigprocmask`.**
   - **Chose:** `pthread_sigmask` before `hero_thread_spawn`, so the new thread inherits the mask.
   - **With `sigprocmask`:** it is unspecified in a multithreaded process. Any thread left with SIGUSR1 unblocked would get the default action, and the process dies when run.
6. **Polling `sigpending` in the work loop instead of a thread.**
   - **Not chosen.** It runs, but because SIGUSR1s merge, it misses every signal sent between two polls, so it gives a lower count than intended.
7. **Return value of `sigwait`.**
   - **Chose:** discard it and test `sig` (which starts at 0), so an error isn't counted and doesn't abort.
   - **With `assert rc == 0`:** the program would stop with an error if `sigwait` ever failed (it can't for a valid set).
8. **Named arguments.** They are required where two parameters share a type: `pthread_sigmask` (two `u32` sets) and `kill` (two `i32`).
   - **Other choice:** leaving them positional there is refused by the compiler. For `sigaddset` and `sigwait` I left them positional, because their parameter types differ.
9. **`_` as the thread body's parameter.**
   - **Chose:** `listen(_: i64)`, since the argument is not used.
   - **With a named, unused parameter:** compile error.
10. **Loops written with `while` and counters, not `for _ in range(...)`.**
    - **Reason:** it is unclear whether the `For` production accepts `_`, and a named loop variable left unused is a compile error.
11. **The width of `getpid`'s result and of the constants.**
    - **Chose:** `getpid() -> i32` for `pid_t`, and the constants as `i32`, so they type-check against the `i32` parameters.
    - **Other choice:** constants declared `i64` don't match the `i32` parameters and are refused (no implicit conversions).
12. **`items.len()`'s type.** The specification doesn't state it.
    - **Chose:** assume it is `i64`, matching `total`.
    - **If it isn't:** the addition is refused.

## prediction

- **Compiler:** I expect it accepts the program on macOS (moderate confidence, given choice points 2, 3 and 11). I expect it refuses it on Linux because of `sigset_t`.
- **Output when run:**
  ```
  built 20000000 items
  received M signals
  ```
  - With no signals sent, M is 0.
  - Otherwise M is the number of SIGUSR1s actually delivered while the listener was waiting. That is at most the number sent, because SIGUSR1s that arrive while one is already pending merge into one.
  - Exit code is 0.
- **Remaining crash window:** a SIGUSR1 that arrives before `main` runs `pthread_sigmask` still gets the default action (the process terminates). No Heroes code runs before `main`, so this can't be avoided.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The harness system prompt (working directory and date) is the only other thing present; it carried no rules about Heroes. No other document, memory or project rule was read.
````

### t1-s-2 (`f302f2`, from `result`)

````markdown
I can only read files here, so I could not create `report.md`. As the brief allows, the whole report is below.

---

# report.md

## answer

**Task:** write `main.hero`. It runs numbered steps until asked to stop. Each step builds `step N`, adds it to a list, waits one second with C's `sleep` and prints the text. On Ctrl-C, `kill -INT` or `kill -TERM` it finishes the current step, prints `stopped after N steps` and exits with code 0. The task also asks whether `heroes build main.hero` accepts it, and what it prints when sent `kill -INT` about 2.5 s after it starts.

**How it stops (design).** The spec has no mutable globals and no closures. A handler given to `signal` takes only an `i32`, so it cannot change a variable in `main`. The spec does say a `signal` handler "runs after the signal, never inside it: on the thread the signal interrupted, once that thread is back in the program's own code." So the handler can safely make ordinary C calls. It records the stop in C-owned state: it sets an environment variable with `setenv`. The loop checks that variable with `getenv` before each step. A step is never cut short: the check happens only between steps. If a signal cuts `sleep` short, the step sleeps again for the time that was left.

**File: `main.hero`**
```
extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

# The environment variable that carries "stop was asked for" from the handler to the loop.
constant STOP_VAR: str
    "HEROES_STEP_LOOP_STOP"

# Runs on the main thread after SIGINT or SIGTERM arrives; it only records the request.
function on_stop(_: i32)
    _ = setenv(name: STOP_VAR.cstr(), value: "1".cstr(), overwrite: 1)

function stop_requested() -> bool
    return getenv(name: STOP_VAR.cstr()) != nullptr

function main()
    _ = unsetenv(name: STOP_VAR.cstr())
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    while !stop_requested()
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        print(text)
    print(f"stopped after {n} steps")
```

No other files are needed. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **Getting the stop request from the handler to the loop.** The spec allows no mutable globals and no closures, and the handler's type is `(function(i32) -> ())`.
   - **My choice:** the handler sets an environment variable in C (`setenv`), and the loop reads it with `getenv`.
   - **Other option, handler calls `exit` itself:** the handler would run right after `sleep` returns, before `print`. The program would end in the middle of a step without printing `step 3` or the summary. Because `exit(code: 0)` is refused (see 6), it could not even exit with 0. Result: the compiler refuses it, or it gives a different answer.
   - **Other option, check `sleep`'s return value instead:** a signal that arrives outside `sleep` (during `print`) would be missed, so the program keeps running. Result: a different answer.
   - **Other option, `sigprocmask` plus `sigpending`:** this needs `sigset_t` declared as a group record. That type is a struct on glibc but a `u32` on macOS, so the program would probably be refused on one platform.

2. **The result type of `signal`.** C returns `void (*)(int)`, and the spec says "a callback is a parameter, never a result."
   - **My choice:** `-> ptr`, discarded with `_ =`.
   - **Other option, leave out the result:** clang would see `void` against a function pointer, and the compiler would refuse it. Writing a function type as the result is refused outright.
   - **Risk:** if clang's check also refuses `ptr` for a function-pointer result, this declaration is the one line the compiler refuses.

3. **Signal constants as `i32`.** I declared `SIGINT` and `SIGTERM` as `i32` to match `signal`'s `int` parameter, because the spec allows no implicit conversions.
   - **Other option, `i64`:** passing them to an `i32` parameter would be refused.

4. **`cstr` parameters marked `lent`.** I passed `STOP_VAR.cstr()` and `"1".cstr()` to parameters marked `lent`, because "a lend reaches only one so declared."
   - **Other option, no `lent`:** the compiler would refuse the call.
   - **Named arguments:** `setenv` has two `cstr` parameters, so its call must name them; `name:` and `value:` are mandatory. Without them, the compiler refuses the call.

5. **Handler parameter named `_`.** An unused parameter is a compile error.
   - **Other option, `sig: i32` left unused:** refused.

6. **Ending with code 0.** `main` simply returns.
   - **Other option, `exit(code: 0)`:** "one written as a number is refused," so the compiler would refuse it.

7. **Finishing a cut-short sleep.** When a signal interrupts `sleep`, it returns the seconds left, and I sleep again until that is 0. This is my reading of "finishes the step in hand."
   - **Other option, sleep once:** same output, but `step 3` appears at about 2.5 s instead of about 3–3.5 s.

8. **When the deferred handler runs.** The spec says only "once that thread is back in the program's own code."
   - **My assumption:** it runs right after `sleep` returns, and in any case before the next `stop_requested()` check. Then the count is 3.
   - **If it runs later:** if the runtime runs the handler only at some later safe point, after the loop condition, one more step runs. The program would print `step 4` and `stopped after 4 steps`, which is a different answer.

9. **`texts` counts as used.** The cell is read only by `texts.push(text)` in its own re-binding. The spec says "a read is a use," so I relied on that.
   - **Other option:** if the compiler judged `texts` unused, it would refuse the program. The task needs the list, so I kept it.

10. **A leftover stop variable.** At start-up I call `unsetenv` so that a `HEROES_STEP_LOOP_STOP` inherited from the parent shell cannot stop the program at once.
    - **Without it:** in that environment the program prints only `stopped after 0 steps`.

11. **Literal types.** `sleep(1)` and `left > 0` take their type `u32` from context; `n @= 0` is `i64`. No choice was needed beyond following section 2.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program. The main risk is the `-> ptr` result on `signal` (choice point 2); a smaller one is the unused-cell rule for `texts` (choice point 9).
- **Output with `kill -INT` at about 2.5 s:**
  - About 1.0 s: `step 1` is printed.
  - About 2.0 s: `step 2` is printed.
  - About 2.5 s: the signal interrupts the third `sleep`, and the handler sets the stop variable.
  - The rest of step 3's second is slept, then `step 3` is printed.
  - The loop check now sees the stop request, so the summary line is printed and `main` returns.
```
step 1
step 2
step 3
stopped after 3 steps
```
- **Exit code:** 0. `kill -TERM` and Ctrl-C behave the same way.

## context

The only inputs that reached my context were `brief.md`, `task.md` and `heroes-spec.md` in this directory. The only other instructions were the agent harness's system prompt: the tool list, the working directory and the date. No other rule, contract, memory or project document was read or shown to me.
````

### t1-m-2 (`3b3338`, from `result`)

````markdown
I only have a Read tool, so I couldn't create `report.md`. Following the brief's fallback, the whole report is below.

# report.md

## answer

**What the task asks:** write `main.hero`. It runs numbered steps (build `step N`, add it to a list, call C's `sleep(1)`, print). When it gets SIGINT or SIGTERM, it finishes the current step, prints `stopped after N steps` and exits with code 0. The task also asks whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 s after it starts.

**Approach.** The spec rules out the usual approach. A callback is "never a result or a signal handler, nor called by one". There are no mutable globals, and threads share nothing but one `i64` in and one out. So the program installs no handler. Instead it:
- blocks SIGINT and SIGTERM with `sigprocmask` before the first step;
- after each step, asks `sigpending` whether either signal is waiting.

A blocked signal can't kill the process or cut short `sleep`, so a step can never be interrupted. The signals are never unblocked, so when `main` returns the pending signal is thrown away and the exit code is 0.

The program is written for macOS, the platform this is being run on. There, `sigset_t` is a `__uint32_t`, so a set is one `u32` element behind an `@` parameter.

### `main.hero`
```
# Signal-mask calls from C. sigset_t is a __uint32_t on macOS, so a set is one u32.
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    function sigemptyset(@set: u32 lent) -> i32
    function sigaddset(@set: u32 lent, signo: i32) -> i32
    function sigprocmask(how: i32, @set: u32 lent, @oset: u32 lent) -> i32
    function sigpending(@set: u32 lent) -> i32
    function sigismember(@set: u32 lent, signo: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

function main()
    mask: u32 @= 0
    assert sigemptyset(@mask) == 0
    assert sigaddset(@mask, SIGINT) == 0
    assert sigaddset(@mask, SIGTERM) == 0
    old: u32 @= 0
    assert sigprocmask(how: SIG_BLOCK, set: @mask, oset: @old) == 0
    texts: [str] @= []
    while true
        text = f"step {texts.len() + 1}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
        pending: u32 @= 0
        assert sigpending(@pending) == 0
        if sigismember(@pending, SIGINT) == 1 || sigismember(@pending, SIGTERM) == 1
            break
    print(f"stopped after {texts.len()} steps")
```

No other file is needed. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **How the signal is noticed.** I chose blocking plus polling with `sigpending`. The alternatives:
   - A handler passed to `signal`/`sigaction`: the spec forbids this, so the compiler refuses it.
   - A thread running `sigwait`: it can't tell the main loop anything without a join, which blocks.
   - Guessing at signal functions in `hero_os.h`: the spec names none, so any guessed name would be refused.
   - Not blocking at all: Ctrl-C or `kill` would kill the process mid-step with exit code 130 or 143, and it would never print "stopped after".
2. **Leaving the signals blocked at the end.** If the program unblocked them before returning, the pending SIGINT would be delivered and kill it (exit 130, possibly before the final line is flushed). Leaving them blocked gives exit code 0.
3. **The type of `sigset_t`.** I used `u32`, which fits macOS (`__uint32_t`). On Linux it is a 128-byte struct, so this file would be refused there. The Linux version would be `record sigset_t partial` (no fields) with `@set: sigset_t lent`, and cells built as `sigset_t(rest: zero)`. On macOS that version would be refused, because clang would find a scalar where a struct is expected. The spec gives no way to write this portably: a byte array only stands in where the header says `void *`.
4. **`lent` on the `@set` parameters.** I added it because C doesn't keep the pointer. Without it, the spec says C "is taken to keep what it is handed". The compiler might still accept that, or it might refuse or restrict later moves of the cell. The answer printed would not change.
5. **`@` on the read-only input sets** (`sigprocmask`'s `set`, `sigismember`'s `set`). The spec only clearly says a pointer is passed when a parameter has `@`. A plain `set: u32 lent` might be passed by value, which would be a type mismatch against `const sigset_t *`, so the compiler would refuse it.
6. **Named arguments on `sigprocmask`.** `set` and `oset` share a type, so names are required. Leaving them out is a compile error.
7. **Constants declared as `i32`.** If I declared them as `i64`, passing them to `int` parameters would be an implicit width conversion, which the compiler refuses.
8. **Checking C results with `assert` rather than `exit`.** `exit` with a numeric literal code is refused, and `_ =` would silently ignore a failed setup. With `assert`, a failure aborts loudly instead of running a loop that can't be stopped cleanly.
9. **Where the check happens.** I check after the print, so the step that was running when the signal arrived is finished and printed. Checking before building the next step is the same thing. Checking before the print would drop the last line and give a count one too low.
10. **Counting with `texts.len()` instead of a separate counter.** Both give the same answer. The list also had to be read somewhere, or it would be an unused binding error.

## prediction

**Does it compile?** On macOS, I expect `heroes build main.hero` to accept it. On Linux it would be refused because of the `sigset_t` type (choice point 3).

**What it prints with `kill -INT` at about 2.5 s:** the signal arrives during step 3's `sleep` and waits as pending. Step 3 finishes at about 3 s and is printed, then the check finds the signal:
```
step 1
step 2
step 3
stopped after 3 steps
```
- **Exit code:** 0.
- **`kill -TERM`:** the same output.
- **Ctrl-C at the terminal:** the same output, except the terminal may echo `^C` in front of the next line.

## context

Only the files in this directory reached me: `brief.md`, `task.md` and `heroes-spec.md`. I also saw the system setup: a working directory path under `/Users/...`, which is how I know this is macOS (choice point 3), and today's date. No other instructions, rules, memory or documents came in. What I know about POSIX and macOS signal headers is general knowledge, not from any file.
````

### t1-e-2 (`597f7e`, from `result`)

````markdown
I only have a Read tool, so I couldn't write `report.md`. As the brief says for that case, the whole report follows as my answer.

# report.md

## answer

**The task:** write `main.hero`. It runs numbered steps (`step N`), adding each text to a list, sleeping one second through C's `sleep`, and printing it. On Ctrl-C, `kill -INT` or `kill -TERM` it finishes the step in hand, prints `stopped after N steps` and exits with code 0. The task also asks whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**How the program stops (it uses no packages, so there is no `heroes.toml`):**
- The spec gives a function handed to `signal` no way to share state with `main`. There are no mutable globals and no closures, and only one `i64` crosses between threads.
- So the stop flag is kept on the C side, in an environment variable. `main` sets `HEROES_STOP=0` before installing the handlers.
- The handler, which runs on the runtime's own thread, sets it to `1`.
- After each step has been printed, the loop reads it with `getenv`.

**Short answers to the two questions:**
- I expect the compiler to accept it.
- I expect it to print `step 1`, `step 2`, `step 3`, `stopped after 3 steps` and exit with code 0.

### `main.hero`
```
# Runs numbered steps until asked to stop (SIGINT or SIGTERM),
# finishing the step in hand before it reports and ends.

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ()))

extern "stdlib.h"
    function getenv(name: cstr lent) -> cstr
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# The environment variable that carries the stop request from the
# signal handler's thread to the main loop.
constant STOP_VAR: str
    "HEROES_STOP"

# Runs on the runtime's own thread after SIGINT or SIGTERM arrives.
function on_stop(_: i32)
    _ = setenv(name: STOP_VAR.cstr(), value: "1".cstr(), overwrite: 1)

# Whether a stop has been asked for since the program started.
function stop_requested() -> bool
    match getenv(STOP_VAR.cstr()).validated()
        .ok v => return v == "1"
        .err _ => return false

# Waits one whole second, sleeping again for whatever an interruption left.
function wait_one_second()
    left @= sleep(1)
    while left > 0
        left @ sleep(left)

function main()
    assert setenv(name: STOP_VAR.cstr(), value: "0".cstr(), overwrite: 1) == 0
    signal(SIGINT, on_stop)
    signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    stopping @= false
    while !stopping
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        wait_one_second()
        print(text)
        stopping @ stop_requested()
    print(f"stopped after {texts.len()} steps")
```

## choice_points

1. **How the handler tells the loop to stop.** The spec says a `signal` handler "runs after the signal, on a thread of the runtime's own". It gives no shared mutable state: no globals, no closures, and a thread carries only one `i64`.
   - **My choice:** an environment variable, written with `setenv` and read with `getenv`. It is set to `"0"` up front, so the handler only overwrites an existing entry and doesn't grow `environ` while `getenv` is reading it.
   - **Other options:**
     - A file flag through `write_file` would run, but could pick up a stale file left by an earlier run.
     - A self-pipe needs fd numbers the handler can't be told.
     - `sigprocmask` plus `sigpending` needs `sigset_t`. That is an integer on macOS and a struct on Linux, so the declaration would be refused on one platform or the other.
     - Calling `exit` from the handler would cut the step short and can't know N. That runs but gives the wrong answer.
   - **Remaining risk:** `setenv` and `getenv` run on different threads. That is a race in theory, but harmless with overwrite-in-place on glibc.

2. **The result type of `signal`.** C returns a function pointer, and the spec says a callback is "a parameter, never a result".
   - **My choice:** declare no result. The spec's own example, `atexit(f: (function() -> ()))`, also leaves out C's `int` result.
   - **Other options:** `-> (function(i32) -> ())` is refused by the rule above. `-> ptr` may be refused by clang's result-type check (function pointer against `void *`), and it would also have to be used or dropped with `_ =`.

3. **The handler's parameter.** A named parameter that isn't used is a compile error, so I wrote `_: i32`. It is `i32` because C's parameter is `int`, and callback parameters follow the width rule. An `i64` there would be refused.

4. **Constant types.** I declared `SIGINT` and `SIGTERM` as `i32`, matching C's `int` and the `sig: i32` parameter. As `i64` the constant itself would be allowed, but passing it to `sig: i32` would be refused, since there are no implicit conversions.

5. **Exit code 0.** I let `main` return normally. `exit(code: 0)` is refused, because "one written as a number is refused".

6. **Where the stop flag is checked.** I check it once per step, after the print, so a step is never cut short.
   - If I checked before the step instead, the output would be the same, but the loop would test the flag before doing anything.
   - If I checked right after `sleep`, a signal arriving during the sleep would skip that step's print. That runs, but prints one step fewer than it did.

7. **When `sleep` is cut short by the signal.** I keep sleeping for the seconds it reports as left.
   - If I called `sleep(1)` just once, the step would end at about 2.5 s. The flag could then be read before the runtime thread has set it, giving `step 4` and `stopped after 4 steps`. That runs and gives a different answer.
   - The re-sleep also keeps each step at one whole second.

8. **Counting N.** I use `texts.len()`, which equals the number printed, because a stop happens only between whole steps. This also counts as a read of `texts`, so the list can't be rejected as written but never used. Using `n` would print the same.

9. **Declaring `setenv`.** It has two `cstr lent` parameters, so every call site names its arguments. Without `lent`, a `.cstr()` lend couldn't reach the parameter and the program would be refused. The `int` results are declared `i32`, which compiles.

10. **Checking `setenv` at startup.** I used `assert ... == 0` instead of handling the error. The alternative, printing and returning, only differs if `setenv` fails.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program. The least certain points are leaving the result off `signal` (choice 2) and clang accepting `setenv` from `stdlib.h` without a feature macro. That is fine with glibc's and macOS's defaults.
- **Run with `kill -INT` at about 2.5 s:** steps finish at about 1 s, 2 s and 3 s. The signal arrives during step 3's sleep, the handler sets the flag, and step 3 still sleeps out its full second. The check after printing `step 3` then sees the flag. Output, then exit code 0:
```
step 1
step 2
step 3
stopped after 3 steps
```
- `kill -TERM` or Ctrl-C at the same moment should give the same output.

## context

- **Inputs used:** only `brief.md`, `task.md` and `heroes-spec.md` from this directory. I read no other file.
- **What else reached my context:** the system's agent framing, which gave the working directory and the date (2026-11-13). The working-directory path names a git worktree of a "heroes-lang" project (`.claude/worktrees/p247-scratch`), but nothing from that project's files reached me. No other instructions, rules, contracts, memories or documents were present.
````

### t1-r-2 (`a4d252`, from `result`)

````markdown
I only have a file-reading tool here, so I could not write `report.md`. As the brief allows, the whole report follows as my answer.

# report.md

## answer

The task needs two files. One is `main.hero`. The other is a C header, `stop.h`, which holds the signal handler. The spec (§13) says no program code runs inside a signal handler, and a Heroes function given to `signal` "stops the program the first time a signal calls it". So the handler has to be written in C, "in a header the program ships". The handler only sets a flag. `main` checks that flag at the top of each step, so a signal never cuts a step short: the program finishes the current step, then stops.

**Does it compile?** I expect `heroes build main.hero` to accept it.

**What it prints when sent `kill -INT` about 2.5 seconds after it starts:**
```
step 1
step 2
step 3
stopped after 3 steps
```
It then ends with exit code 0.

### `stop.h`
```c
#ifndef HERO_STOP_H
#define HERO_STOP_H

#include <signal.h>

/* Set by the handler, read by the program between steps. */
static volatile sig_atomic_t stop_flag = 0;

static void stop_on_signal(int sig)
{
    (void)sig;
    stop_flag = 1;
}

/* Returns 0 when both SIGINT and SIGTERM are caught, -1 otherwise. */
static int stop_install(void)
{
    if (signal(SIGINT, stop_on_signal) == SIG_ERR) {
        return -1;
    }
    if (signal(SIGTERM, stop_on_signal) == SIG_ERR) {
        return -1;
    }
    return 0;
}

/* Returns 1 once a stop was asked for, 0 before. */
static int stop_requested(void)
{
    return stop_flag != 0;
}

#endif
```

### `main.hero`
```
extern "stop.h"
    function stop_install() -> i32
    function stop_requested() -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

constant EXIT_SETUP_FAILED: i64
    1

# Runs numbered steps until SIGINT or SIGTERM, never stopping inside a step.
function main()
    if stop_install() != 0
        print("cannot catch SIGINT and SIGTERM")
        exit(code: EXIT_SETUP_FAILED)
    texts: [str] @= []
    n @= 0
    while stop_requested() == 0
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
    print(f"stopped after {texts.len()} steps")
```

## choice_points

1. **Where the handler lives.** I wrote it in C, in `stop.h`, and the program only reads a flag.
   - The other choice is to pass a Heroes function to `signal`. The spec says that stops the program at the first signal: it would end mid-step and never print `stopped after N steps`.

2. **Where `extern "stop.h"` looks for the header.** I assumed it finds `stop.h` beside `main.hero`, the same way `use` starts at the compiled file's directory. The spec does not say.
   - If the header must sit somewhere else or be named another way, the compiler refuses the program because the header is missing.

3. **Function bodies inside the header.** I made them `static`.
   - Without `static`, the build could fail at link time with duplicate symbols if the generated C includes the header in more than one translation unit.
   - If the toolchain instead expects the symbols to come from a library, the build could fail at link time with missing symbols. The spec's "a header the program ships" suggests the code belongs in the header.

4. **`signal()` or `sigaction()`.** I used `signal()`, which is ISO C and needs no feature macro.
   - `sigaction` might need `_POSIX_C_SOURCE` or `_GNU_SOURCE` defined first (§13 says such a definition must come first). Getting that wrong means the compiler refuses the program.
   - At run time both behave the same here, because `sleep` is not restarted after a signal under either.

5. **C types as written in Heroes.**
   - `int` becomes `i32` and `unsigned int` becomes `u32`, at the exact width and sign, as §13 requires for parameters.
   - Results could be wider than C's, but I kept them exact.
   - Declaring the flag result as `bool`, or `sleep`'s parameter as `i64`, would be refused.

6. **The `sleep` result.** I threw it away with `_ = sleep(1)`. It is a plain `u32`, not a fallible type, so `_ =` is allowed.
   - Leaving it as a bare line would be refused, because a line that computes a value must use it.
   - When the signal arrives during step 3, `sleep` returns early, at about 2.5 s instead of 3 s. The step still finishes and prints.
   - I could have looped to sleep out the remaining time. The output would be the same, just about 0.5 s later.

7. **How the program exits with code 0.** `main` simply returns; it does not call `exit(code: 0)`. §11 says an exit code "written as a number is refused", so `exit(code: 0)` would be a compile error.
   - For the setup-failure path I used a named constant, which I read as not "written as a number". If that reading is wrong, the compiler refuses `exit(code: EXIT_SETUP_FAILED)`. The fix would be to print and return instead.

8. **Reading the list.** I used `texts.len()` as N in the final message. That counts the steps printed, because every step that is pushed is also printed.
   - It also guarantees `texts` is read outside its own re-binding. If a self-read inside `texts @ texts.push(...)` did not count as a use, a program that never read `texts` elsewhere would be refused as unused.

9. **Where the stop check sits.** I check the flag only at the top of the loop.
   - Checking after `sleep` and breaking there would skip `print`. Step 3 would never print, and the program would report "stopped after 2 steps" while it had pushed 3: a wrong answer.

10. **Calling `sleep`.** I wrote `sleep(1)` without the argument name. With only one parameter, the name is optional. Writing `sleep(seconds: 1)` would also be accepted.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program, with `stop.h` beside it and no `heroes.toml`, since it uses no package.
- **Timing:** steps print at about 1 s, 2 s and 2.5 s.
  - At 2.5 s, step 3's `sleep` is interrupted by SIGINT.
  - The handler sets the flag and `sleep` returns early.
  - The program prints `step 3`, sees the flag at the next loop check, and leaves the loop.
- **Output and exit:** it prints the four lines shown under `answer`, then exits with code 0.
  - Ctrl-C does the same, though the terminal may also echo `^C`.
  - `kill -TERM` gives the same output.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other file was read. I received no other project instructions, rules, contracts, memory or documents, beyond the general system prompt that describes my tools and working directory.
````

### t1-x-2 (`b91072`, from `result`)

````markdown
I couldn't write `report.md` because I can only read files here, so the whole report follows as the brief allows. I haven't built or run anything, so every outcome below is a prediction.

# report.md

## answer

**What the program does.** At startup it blocks SIGINT and SIGTERM with `sigprocmask`. While they are blocked, a Ctrl-C, `kill -INT` or `kill -TERM` cannot end the program. The signal just waits as pending. Each step builds `step N`, adds it to the list, sleeps one second with C's `sleep` and prints the text. Only then does the program check with `sigpending`/`sigismember` whether either signal is waiting. If one is, it prints `stopped after N steps` and returns from `main`, so it ends with exit code 0. A blocked signal still pending when the process ends is thrown away, so it never kills the program.

**Expected result.** I expect the compiler to accept it on Linux (glibc). On macOS it may be refused when the generated C is compiled; see choice point 4. When sent `kill -INT` about 2.5 s after it starts, it prints:

```
step 1
step 2
step 3
stopped after 3 steps
```

and exits with code 0 at about t = 3 s.

### `main.hero`

```
# Signal handling: block SIGINT and SIGTERM, then poll for them between steps.
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function sigprocmask(how: i32, set: ptr, oset: ptr) -> i32
    function sigpending(set: ptr) -> i32
    function sigismember(set: ptr, signo: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

# Storage for one C `sigset_t`; 128 bytes covers glibc (128) and macOS (4).
constant SIGSET_BYTES: u64
    128

# Whether SIGINT or SIGTERM arrived while blocked; `scratch` holds one sigset_t.
function stop_requested(scratch: ptr) -> bool
    assert sigpending(set: scratch) == 0
    return (sigismember(set: scratch, signo: SIGINT) == 1
        || sigismember(set: scratch, signo: SIGTERM) == 1)

function main()
    blocked = malloc(size: SIGSET_BYTES)
    pending = malloc(size: SIGSET_BYTES)
    assert blocked != nullptr
    assert pending != nullptr
    assert sigemptyset(set: blocked) == 0
    assert sigaddset(set: blocked, signo: SIGINT) == 0
    assert sigaddset(set: blocked, signo: SIGTERM) == 0
    assert sigprocmask(how: SIG_BLOCK, set: blocked, oset: nullptr) == 0
    texts: [str] @= []
    n @= 0
    while true
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(seconds: 1)
        print(text)
        if stop_requested(pending)
            break
    print(f"stopped after {texts.len()} steps")
    free(p: blocked)
    free(p: pending)
```

No `heroes.toml` is needed because the program uses no package.

## choice_points

1. **How to catch the signal: block it and check for it, or install a handler.** I chose to block it and check after each step. A handler installed with `signal(sig, handler: (function(i32) -> ()))` has three problems:
   - C's `signal` returns a function pointer, and the spec says a callback is "never a result". The declaration would be refused, or need an unverified `ptr` result.
   - Heroes has no mutable globals, so the handler can't set a flag that `main` reads.
   - The one workaround, checking whether `sleep` returns a nonzero remainder, misses a signal that arrives while the program is printing or building text. The program would run on and give the wrong result.
   
   Not installing anything would let the default action kill the process mid-step: no "stopped after" line and exit status 130 or 143.
2. **How to give C a `sigset_t`.** I chose a `ptr` to `malloc`'d memory, because the spec doesn't check what a `ptr` points at. The other option, a group `record sigset_t`, is refused on macOS, where `sigset_t` is a `__uint32_t` and not a struct. On glibc it would have to name the private `__val` field, which ties the program to one platform.
3. **The buffer size.** I chose 128 bytes. A smaller size, such as 4, makes glibc write past the buffer, which is undefined behaviour at run time.
4. **Platform risk I can't remove.** On macOS, `sigemptyset`, `sigaddset` and `sigismember` are also macros that dereference their argument. Passed a `void *`, the generated C would likely be refused at the clang step. I expect Linux to accept it. With these inputs I couldn't find a portable alternative.
5. **The width of C results.** I declared `int` results as `i32` and the constants as `i32`. This matches the header, and the constants pass straight into the `i32` parameters. Declaring the constants as `i64` would be refused at those calls, because Heroes has no implicit width conversion.
6. **What to do with C results.** I chose `assert … == 0`. Writing `_ = …` would also compile but would silently ignore a failure. The spec doesn't say whether asserts outside tests always run. If they were compiled out, the calls inside them would disappear, the signals would never be blocked, and the program would die on the signal.
7. **Named arguments.** `sigprocmask` has two `ptr` parameters, so the names are required (`set:`, `oset:`). Leaving them out is refused. I also wrote names on the other calls, which the grammar allows.
8. **When to check for the signal.** I check after printing, which is the end of the step. Checking before the sleep would start a new step after the signal and print one more `step` line before stopping, which is the wrong result.
9. **Never unblocking the signals.** I never unblock them, and I don't call `sigwait`. Unblocking them before ending would deliver the pending signal and kill the process with a signal status instead of 0.
10. **How to end.** I return normally from `main`, which gives code 0. The other way, `exit(code: 0)`, would also work.
11. **`free` with no `->`.** C's `void` result maps to `()`. Declaring a result type would likely be refused by clang's check.
12. **What N is.** I print `texts.len()`, the number of texts built, which here equals the number printed. Printing `n` would give the same answer.
13. **The long `return` expression.** I wrapped it in parentheses so it can break across lines. Without them, a line break outside brackets is refused.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program on Linux/glibc. On macOS it may be refused when compiling C (choice point 4).
- **Running with `kill -INT` at about 2.5 s:** it prints `step 1` at 1 s and `step 2` at 2 s. The signal arrives during the third sleep and is held as pending. It prints `step 3` at 3 s, sees the pending SIGINT, prints `stopped after 3 steps`, and exits with code 0.
- **Ctrl-C or `kill -TERM`:** both behave the same way.

## context

Apart from this directory's `brief.md`, `task.md` and `heroes-spec.md`, the only extra input was the agent's system setup (working directory, date and tool definitions). No other rules, memory or documents reached my context.
````

### t1-x-3 (`38180e`, from `result`)

````markdown
I couldn't write `report.md`: the only tool I have here reads files and can't create them. As the brief allows, the whole report follows as my answer.

# report.md

## answer

**Does the compiler accept it?** I expect `heroes build main.hero` to accept the program on macOS. I'm less sure of this than of anything else in the report (see `prediction`). On Linux I expect it to be refused.

**What it prints when sent `kill -INT` about 2.5 seconds after it starts:**
```
step 1
step 2
step 3
stopped after 3 steps
```
It then exits with code 0.

**How it works.** The specification doesn't mention signals, and the language gives a C signal handler no way to tell the program anything. There are no mutable globals, and a callback can only take and return numbers. So the program doesn't install a handler. It blocks SIGINT and SIGTERM with C's `sigprocmask`, and before each step it asks C's `sigpending` whether either one has arrived. A blocked signal can't kill the process or interrupt `sleep`. The step in progress always finishes; the program then stops at the next check, prints the summary, and returns from `main`.

No other file is needed. The program uses no package, so there is no `heroes.toml`.

### `main.hero`
```
# Waits one second, then hands back the seconds left unslept.
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Signal masking: SIGINT and SIGTERM are blocked, never handled, and
# polled between steps, so a step is never cut short.
# This targets macOS, where sigset_t is a 32-bit unsigned integer and
# signal s is the bit 1 << (s - 1).
extern "signal.h"
    constant SIGINT: i64
    constant SIGTERM: i64
    constant SIG_BLOCK: i32
    function sigprocmask(how: i32, @set: u32 lent, @oset: u32 lent) -> i32
    function sigpending(@set: u32 lent) -> i32

# Whether SIGINT or SIGTERM has arrived (and waits, blocked).
function stop_requested(mask: i64) -> bool
    pending: u32 @= 0
    rc = sigpending(@pending)
    assert rc == 0
    return (to_i64(pending).must() & mask) != 0

function main()
    mask = (1 << (SIGINT - 1)) | (1 << (SIGTERM - 1))
    blocked: u32 @= to_u32(mask).must()
    previous: u32 @= 0
    rc = sigprocmask(how: SIG_BLOCK, set: @blocked, oset: @previous)
    assert rc == 0
    texts: [str] @= []
    n @= 0
    while !stop_requested(mask)
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(seconds: 1)
        print(text)
    print(f"stopped after {n} steps")
```

## choice_points

1. **How to catch the stop request.**
   - **What I chose:** block SIGINT and SIGTERM with `sigprocmask`, and check `sigpending` before each step.
   - **The other choice:** install a handler with `signal(sig, handler)`. Three things stop it:
     - The specification says a callback can't be a result, and `signal` returns one. The compiler would refuse that declaration.
     - A handler can't set anything the program can read, because there are no mutable globals.
     - Even if it could, a handler would cut `sleep` short, so the step in progress would end early. That runs but gives a different answer than intended.

2. **What type stands for `sigset_t`.**
   - **What I chose:** `u32`, which is what `sigset_t` is on macOS (`__uint32_t`), passed as `@set: u32` (one element, same width and sign).
   - **The other choice:** a group `record sigset_t partial`, which Linux would need because its `sigset_t` is a struct. On macOS the compiler would refuse it, because there `sigset_t` isn't a struct.
   - **Consequence:** my version is refused on Linux, where the header's type doesn't match `u32`.

3. **How to build the signal mask.**
   - **What I chose:** compute the bits myself, `1 << (signo - 1)` (macOS's `__sigbits`). The bitwise operators work only on `i64`, so I convert with `to_u32(...).must()`.
   - **The other choice:** `sigemptyset` / `sigaddset` / `sigismember`. On macOS these are also macros, so declaring them in an `extern` group risks the compiler refusing them when clang checks against the header. Building the bits myself avoids that.
   - **Consequence:** the hard-coded bit layout is wrong on any platform with a different layout. There, signals wouldn't be blocked and Ctrl-C would kill the program mid-step: it runs but gives a different answer.

4. **How to declare the set parameters.**
   - **What I chose:** `@set: u32 lent` and `@oset: u32 lent`, because C writes through these pointers but doesn't keep them.
   - **The other choice:** leaving out `lent` means the parameter "keeps what it is handed". Lending a local cell to it might be refused.
   - **What I couldn't do:** pass `nullptr` for `oset`, because an `@` parameter needs a real place. That's why the program has the extra `previous` cell.

5. **The types of the constants.**
   - **What I chose:** `SIG_BLOCK: i32`, to match the C `int` parameter `how`. `SIGINT` and `SIGTERM` are `i64` (wider is allowed) so they can go straight into the `i64` bit arithmetic.
   - **The other choice:** declaring `SIG_BLOCK` as `i64` gives a type mismatch at the call, which the compiler refuses.

6. **How the program ends with code 0.**
   - **What I chose:** let `main` return normally.
   - **The other choice:** `exit(code: 0)` is refused, because the specification refuses an exit code written as a number.

7. **Where the stop check sits.**
   - **What I chose:** the loop condition, so the check happens between steps.
   - **The other choice:** checking only after `print`, at the end of a step, comes to the same moment.
   - **A choice that would be wrong:** checking before `sleep`. The program would end mid-step: text built and stored but never printed, so it gives a different count.

8. **Where N in "stopped after N steps" comes from.**
   - **What I chose:** the counter `n`, which equals the number of texts printed.
   - **The other choice:** `texts.len()` gives the same value, but the specification doesn't say what type `len` returns.

9. **Results that need checking.**
   - **What I chose:** the `sigprocmask` and `sigpending` results go through `assert rc == 0`, and the `sleep` result is thrown away with `_ =`.
   - **The other choice:** leaving a result unbound is a compile error.

10. **Whether `@` arguments count as use.**
    - **How I read it:** "an `@` argument counting", and a write through an `@` parameter counts as a use. So `blocked`, `previous` and `pending`, which are only handed to C with `@`, are neither "unused" nor "never re-bound".
    - **If I've read it wrong:** the compiler refuses the program.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program on macOS. The least certain points are:
  - the `@set: u32 lent` declarations against parameters C declares as `const sigset_t *` and `sigset_t *`;
  - whether constants from an `extern` group can be used directly as `i64` / `i32` values.

  On Linux I expect it to be refused, because `sigset_t` is a struct there (choice point 2).
- **Run, with `kill -INT` at about 2.5 s:** steps 1 and 2 print at about 1 s and 2 s. The signal arrives during the sleep of step 3 and is held, because it's blocked. Step 3 finishes and prints at about 3 s. The next check finds the signal and ends the loop:
  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```
  The exit code is 0.
- **Ctrl-C and `kill -TERM`:** both behave the same way. The terminal may echo `^C`.
- **Small gaps:**
  - A signal that arrives in the first instant, before `sigprocmask` runs, kills the program with the default action.
  - A signal that arrives during a step is acted on when that step ends, so stopping can take up to one second.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other project instruction, rule, contract, memory or document did. I used general knowledge of C's POSIX headers (`signal.h` on macOS and Linux, `unistd.h`), which comes from training, not from a file.
````

### t1-r-3 (`2e62c9`, from `result`)

````markdown
I couldn't save `report.md` because I can only read files here, so the whole report is below. I didn't build or run anything; the compiler result and the output are predictions.

# report.md

## answer

The task asks for `main.hero`. It must run numbered steps until it gets SIGINT or SIGTERM, then finish the step it is in, print `stopped after N steps` and exit with code 0. It also asks whether `heroes build main.hero` accepts the program, and what it prints when sent `kill -INT` about 2.5 seconds after starting.

Section 13 says the handler can't be written in Heroes: "a function given to `signal` stops the program the first time a signal calls it … write the handler in C, in a header the program ships." So the program ships a C header, `stop.h`. Its handler only sets a flag. The Heroes loop checks that flag only between steps, so a step is never cut off.

### `stop.h`
```c
#ifndef STOP_H
#define STOP_H

#include <signal.h>

/* Set by the handler; read by the program between steps. */
static volatile sig_atomic_t stop_flag = 0;

static void stop_handler(int sig)
{
    (void)sig;
    stop_flag = 1;
}

/* Route Ctrl-C (SIGINT) and kill -TERM (SIGTERM) to the flag. */
static void stop_install(void)
{
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);
}

/* Nonzero once a stop was asked for. */
static int stop_requested(void)
{
    return stop_flag != 0;
}

#endif
```

### `main.hero`
```
extern "stop.h"
    function stop_install()
    function stop_requested() -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Runs numbered steps until SIGINT or SIGTERM, never stopping inside a step.
function main()
    stop_install()
    texts: [str] @= []
    n @= 0
    while stop_requested() == 0
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
    print(f"stopped after {n} steps")
```

There is no `heroes.toml`, because the program uses no package.

## choice_points

1. **Where the handler lives.**
   - **Choice:** a C handler in the shipped `stop.h`, which only sets a `sig_atomic_t` flag.
   - **Other option:** declaring `signal` in an `extern` and passing it a Heroes function.
   - **What that would produce:** the spec says the program stops the first time the signal arrives. It would end mid-step, without the final line and without exit code 0. Wrong answer at run time.
2. **Static vs non-static definitions in the header.**
   - **Choice:** `static`.
   - **Other option:** non-static functions and variable.
   - **What that would produce:** if the compiler includes the header in more than one C file, the link fails with duplicate symbols, so the build is refused. My `static` version has the opposite risk: if the generated calls are in a different C file from the one that includes the header, they can't be linked. The spec doesn't say how the generated C is split into files.
3. **`signal()` vs `sigaction()`.**
   - **Choice:** `signal()`, which is plain ISO C and is declared under any compiler mode.
   - **Other option:** `sigaction`.
   - **What that would produce:** under a strict `-std=c11` it may be undeclared without a `_POSIX_C_SOURCE` define first, so the build would be refused. The spec doesn't say what compiler flags it uses.
4. **Result types of the C functions.**
   - **Choice:** I declared `-> i32` for `int` and `-> u32` for `unsigned int`, and left `-> ` off the `void` function.
   - **Other option:** any other width or sign.
   - **What that would produce:** a result may be wider than C's (for example `i64`) and still pass. A narrower result or the wrong sign is refused by the compiler's header check.
5. **Discarding `sleep`'s result with `_ =`.**
   - **Choice:** `_ = sleep(1)`.
   - **Other option:** binding it to a name that is never read, or writing the call as a bare line.
   - **What that would produce:** the compiler refuses both (an unused binding, and a line that computes a value without using it).
6. **When `sleep` is cut short.**
   - **Choice:** one `sleep(1)` call per step. A signal during the sleep makes it return early with the time left, and the step then prints its text.
   - **Other option:** looping on the time left to make the full second.
   - **What that would produce:** the same output, but `step 3` would come out at about 3.0 s instead of about 2.5 s.
7. **Where the flag is checked.**
   - **Choice:** at the top of the loop only, so a step that has started always finishes.
   - **Other option:** also checking after the sleep.
   - **What that would produce:** the program would end mid-step and print one step fewer (`stopped after 2 steps`), which is the wrong answer.
8. **Where the step count comes from.**
   - **Choice:** the counter `n`.
   - **Other option:** `texts.len()`.
   - **What that would produce:** the same number, since every step that is built gets printed. The spec doesn't give `len`'s result type, though.
9. **Exit code 0.**
   - **Choice:** `main` simply returns.
   - **Other option:** `exit(code: 0)`.
   - **What that would produce:** section 11 says "a code … written as a number is refused," so the compiler refuses it. The spec doesn't explicitly say that returning from `main` gives exit code 0; I assumed it does.
10. **How the header is named.**
    - **Choice:** `extern "stop.h"`, a quoted name next to `main.hero`.
    - **Risk:** the spec doesn't say where shipped headers are searched for. If the program's directory isn't on the include path, the build is refused.
11. **Order of the `extern` groups.** The two headers don't depend on each other and neither defines `_GNU_SOURCE`, so either order works.

## prediction

- **Build:** I expect `heroes build main.hero` to accept the program, with the risks noted in choice points 2 and 10.
- **Run, with `kill -INT` at about 2.5 s:**
  1. Steps 1 and 2 print at about 1 s and 2 s.
  2. The signal arrives during step 3's sleep, so `sleep` returns early.
  3. Step 3 prints, the loop sees the flag and stops.

  It prints:
  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```
- **Exit code:** 0. The output is the same for `kill -TERM` or Ctrl-C.

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only things in my context were the agent's standard system setup and the user's request to follow the brief. No other rule, memory or document about this language reached me.
````

### t1-e-3 (`87efae`, from `result`)

````markdown
I couldn't create `report.md` because I can only read files here, not write them, so the whole report is below as the brief allows.

# report.md

## answer

The task asks for `main.hero`, a loop that only stops between steps when it gets SIGINT or SIGTERM. It also asks whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**How the signal reaches the loop:** the spec says a function given to `signal` runs on one of the runtime's own threads after the signal arrives. But the spec has no mutable globals, and threads can share only an `i64`. So the handler and the loop share a flag through C instead: an environment variable. The handler sets it with `setenv` and the loop checks it with `getenv` before each step. Because the check only happens between steps, a step that has started always finishes.

The program is one file and needs no `heroes.toml`, since it uses no package.

### `main.hero`

```
extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ()))

constant EXIT_OK: i64
    0

# True once a stop signal's handler has run.
function stop_requested() -> bool
    return getenv(name: "HEROES_MAIN_STOP_REQUESTED".cstr()) != nullptr

# Runs on the runtime's own thread after SIGINT or SIGTERM; only records the request.
function on_stop(_: i32)
    _ = setenv(name: "HEROES_MAIN_STOP_REQUESTED".cstr(), value: "1".cstr(), overwrite: 1)

function main()
    _ = unsetenv(name: "HEROES_MAIN_STOP_REQUESTED".cstr())
    signal(sig: SIGINT, handler: on_stop)
    signal(sig: SIGTERM, handler: on_stop)
    texts: [str] @= []
    step @= 0
    while !stop_requested()
        step @ step + 1
        text = f"step {step}"
        texts @ texts.push(text)
        _ = sleep(seconds: 1)
        print(text)
    print(f"stopped after {texts.len()} steps")
    exit(code: EXIT_OK)
```

## choice_points

1. **How the handler tells the loop to stop.** The spec gives no shared mutable state: no globals, no closures, and only an `i64` can cross between threads.
   - **My choice:** a C environment variable, set with `setenv` and read with `getenv`.
   - **Blocking the signals instead** (`sigprocmask` + `sigpending`) would mean writing out `sigset_t`. That type is a `u32` on macOS and a struct on Linux, and its helper functions are macros on macOS, so the compiler would likely refuse it. Also, the runtime has its own threads, so a signal could land on a thread that doesn't block it, and the default action would kill the process mid-step.
   - **Using a pipe or a file instead** would need a fixed file descriptor or would leave a file on disk.
   - **Risk of my choice:** POSIX doesn't promise that `getenv` is safe while another thread runs `setenv`. In rare cases the program could crash while running.
2. **Clearing the variable at startup.** I call `unsetenv` first. Without it, if the variable were already set in the environment the program started from, it would print `stopped after 0 steps` straight away.
3. **`signal`'s return value.** C's `signal` returns a function pointer, and the spec says a callback is never a result. Following the spec's own `atexit` example, which leaves out C's `int` result, I left out the result. If I had declared `-> ptr`, I expect the compiler would refuse it, since clang checks result types against the header.
4. **Integer widths.**
   - `SIGINT` and `SIGTERM` are declared as `i32`, matching C's `int`. Declaring them `i64` would need `to_i32(...).must()` at each call, or the compiler would refuse the program for a width mismatch.
   - `sleep(seconds: u32) -> u32` follows the header's `unsigned int`. A parameter declared `i64` would be refused.
5. **The handler's unused parameter is named `_`.** If I had named it, the compiler would refuse it as an unused parameter.
6. **Which parameters are `lent`.** Every `cstr` parameter is `lent`, because `.cstr()` only reaches a `lent` parameter. Without it, the compiler would refuse the program.
7. **How the program ends.** It calls `exit(code: EXIT_OK)` with a named constant.
   - `exit(code: 0)` would be refused, because the spec rejects a code written as a number.
   - Just returning from `main` probably also gives exit code 0, but the spec never says so.
8. **When the stop check happens.** It runs before each step, at the top of the loop. Checking in the middle of a step would break the rule that a step in progress must finish.
9. **Where N comes from.** N is `texts.len()`, which equals the number of steps printed. A separate counter would give the same answer.
10. **Unused results.** `sleep`'s, `setenv`'s and `unsetenv`'s return values are discarded with `_ =`. Leaving out `_ =` would be refused, because the spec doesn't allow a line to drop a computed value. `signal` has no result, so its line stands alone.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it. The main uncertainty is whether leaving out `signal`'s function-pointer result is allowed (choice point 3).
- **Output with `kill -INT` at about 2.5 seconds:**
  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```
  Then it exits with code 0. The signal arrives during step 3's sleep, and the handler sets the flag on the runtime's thread. Step 3 finishes and prints at about 3.0 seconds, and the next check stops the loop.
- **Less likely outcome:** if the signal cuts `sleep` short and the main thread checks the flag before the handler thread has run, it prints `step 4` as well and then `stopped after 4 steps`. That would still be exit code 0.

## context

No instruction, rule, contract, memory or document reached my context other than `brief.md`, `task.md` and `heroes-spec.md`. The only other context was the general agent system prompt (working directory and date).
````

### t1-m-3 (`8be6f8`, from `result`)

````markdown
I only have a file-reading tool here, so I couldn't write `report.md`. As the brief allows, the whole report is below.

---

# report.md

## answer

The program blocks SIGINT and SIGTERM with `sigprocmask`, so a stop request is held back instead of ending the program. After each step it checks `sigpending` to see whether a stop was asked for. The specification forbids signal handlers ("A callback is a parameter, never a result or a signal handler"), so this is the only way it leaves open. Because the signals are blocked, they also can't cut `sleep` short, so the step in hand always finishes.

### `main.hero`

```
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signo: i32) -> i32
    function sigprocmask(how: i32, set: sigset_t lent, @oldset: sigset_t) -> i32
    function sigpending(@set: sigset_t) -> i32
    function sigismember(set: sigset_t lent, signo: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Exit code when the stop signals cannot be blocked.
constant FAILED: i64
    1

# The set holding the two signals that ask the program to stop.
function stop_set() -> sigset_t
    set: sigset_t @= sigset_t(rest: zero)
    _ = sigemptyset(@set)
    _ = sigaddset(@set, SIGINT)
    _ = sigaddset(@set, SIGTERM)
    return set

# Whether SIGINT or SIGTERM has arrived (held pending, since both are blocked).
function stop_requested() -> bool
    pending: sigset_t @= sigset_t(rest: zero)
    _ = sigemptyset(@pending)
    _ = sigpending(@pending)
    return sigismember(pending, SIGINT) == 1 || sigismember(pending, SIGTERM) == 1

function main()
    stops = stop_set()
    old: sigset_t @= sigset_t(rest: zero)
    if sigprocmask(how: SIG_BLOCK, set: stops, oldset: @old) != 0
        print("could not block SIGINT and SIGTERM")
        exit(code: FAILED)
    texts: [str] @= []
    n: i64 @= 0
    while true
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
        if stop_requested()
            break
    print(f"stopped after {texts.len()} steps")
```

There is no `heroes.toml`, because the program uses no package (§1).

## choice_points

1. **How to catch the signal.** The spec forbids a callback as a signal handler, so I block the signals and poll for them with `sigpending`.
   - Other choice: declaring `signal(sig: i32, f: (function(i32) -> ()))` breaks a stated rule, so the compiler would refuse it.
   - Doing nothing: Ctrl-C or kill would end the program mid-step with no "stopped" line and a nonzero, signal-caused exit status. That runs but gives the wrong answer.
2. **Declaring `sigset_t`.** I used `record sigset_t partial` with no fields and built it with `rest: zero`.
   - On glibc this matches: `sigset_t` is a typedef of an untagged struct, so the record keeps the same name and has no `tag`.
   - On macOS `sigset_t` is `__uint32_t`, not a struct. There I expect the compiler to refuse the record. Declaring it with a tag (`tag __sigset_t`) would be refused on glibc, because that name is a typedef, not a struct tag.
   - Leaving out `partial` would need glibc's private field `__val: u64[16]`. That would compile on glibc only, and it ties the code to an internal field.
3. **`@` out-parameters without `lent`.** I followed the `sqlite3_open` example. The spec says a non-`lent` parameter "is taken to keep" what it gets.
   - If the compiler asked for `lent` here, it would refuse the program. Adding `@set: sigset_t lent` is the other reading. I can't tell which one the compiler requires, so this is a real ambiguity in the spec.
4. **Named arguments for `sigprocmask`.** `set` and `oldset` have the same type, so naming them is mandatory (§9). I named all three arguments.
   - Calling it by position would be refused.
   - I assumed `lent` and `@` don't make the types different. If they did, naming the arguments is still allowed.
5. **Declared widths.** Constants and `int` results are `i32`, and `sleep` takes `u32` (`unsigned int`).
   - Using `u64` or `i64` for the parameter would be refused (§13).
   - The constants could also be `i64`, because "a result may be wider." Then every call would need `to_i32(...).must()`.
6. **When to check for a stop.** I check after each step has printed.
   - Checking only at the start of a step also works.
   - Checking during the sleep isn't possible, and isn't needed, because blocking keeps `sleep` from being interrupted.
   - Side effect: a signal that arrives in the microseconds between the check and the next step's start makes the program run one more full step. It still never stops mid-step.
7. **Leaving the pending signal undrained.** The program exits with the signal still blocked and pending, and the kernel discards it.
   - Draining it with `sigwait` would add a cell and a call but print the same thing.
   - If the runtime unblocked signals before `exit`, which the spec doesn't say it does, the process would die from the signal. The exit code would be 130 or 143 instead of 0.
8. **Exit code 0.** `main` returns normally, which I take to mean exit code 0.
   - `exit(code: 0)` is a code "written as a number", which the spec says is refused. That's why the failure path uses the named constant `FAILED`.
9. **Counting steps.** N in "stopped after N steps" is `texts.len()`. The step text uses the separate cell `n`. Both equal the number of steps printed, so the output is the same either way.
10. **Ignoring C return codes.** I discard the results of `sigemptyset`, `sigaddset`, `sigpending` and `sleep` with `_ =`, which works because they are plain `i32`/`u32`, not fallible. Only `sigprocmask` is checked. Checking all of them would print the same thing on a normal run.

## prediction

**Compiler:** I expect `heroes build main.hero` to accept it on Linux/glibc. On macOS I expect it to refuse the `sigset_t` record (choice point 2). The open risk on any platform is the `lent` question for `@` out-parameters (choice point 3).

**Output** when sent `kill -INT` about 2.5 seconds after starting:

- At about 1 s the first sleep ends and it prints `step 1`; no signal is pending.
- At about 2 s it prints `step 2`; no signal is pending.
- At 2.5 s SIGINT arrives during step 3's sleep. It stays pending, and `sleep` is not interrupted.
- At about 3 s it prints `step 3`, sees the pending signal, and leaves the loop.

```
step 1
step 2
step 3
stopped after 3 steps
```

It ends with exit code 0. Ctrl-C at the terminal and `kill -TERM` behave the same way.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document did, apart from the general tool and environment setup (working directory, date, and the single `Read` tool). That setup carried nothing about Heroes. My knowledge of POSIX signals, glibc and macOS headers comes from training, not from any file.
````

### t1-s-3 (`2dde04`, from `result`)

````markdown
I only have a file-reading tool here, so I could not write `report.md`. As the brief says for that case, the whole report follows.

# report.md

## answer

The task asks for a program, `main.hero`, that runs numbered steps until it gets SIGINT or SIGTERM, finishes the step in progress, prints `stopped after N steps` and exits with code 0. It also asks whether `heroes build main.hero` accepts the program and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**How it is built.** The specification has no mutable globals. A function given to `signal` is a top-level function of one `i32`, and it cannot write to any variable in `main`. So the stop request is passed through state that C holds: the process environment. The signal function calls `setenv("HEROES_STOP", "1", 1)`. The loop checks that variable with `getenv` only at the start of each step. That way a signal can never cut a step short. The specification says a function given to `signal` runs after the signal, "on the thread the signal interrupted, once that thread is back in the program's own code". So it sets the variable somewhere inside the current step, and the step still finishes.

### `main.hero`

```
# Prints "step N" once a second until SIGINT or SIGTERM, finishing the step in hand.

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

# Runs after SIGINT or SIGTERM, back in the program's own code: records the request.
function on_stop(_: i32)
    _ = setenv(name: "HEROES_STOP".cstr(), value: "1".cstr(), overwrite: 1)

# Whether a stop has been requested since the program started.
function stop_requested() -> bool
    return getenv("HEROES_STOP".cstr()).validated().default("") == "1"

function main()
    _ = unsetenv("HEROES_STOP".cstr())
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    while !stop_requested()
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
    print(f"stopped after {n} steps")
```

No other files are needed. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **How the signal function tells the loop to stop.** The specification has no mutable globals, and the signal function takes only an `i32`, so it cannot reach a variable in `main`.
   - **My choice:** an environment variable, written with `setenv` and read with `getenv`.
   - **Option: a file written with `write_file`.** This would also work, but it leaves a file on disk and can fail.
   - **Option: block the signals and poll them with `sigprocmask`/`sigpending`.** `sigset_t` is a plain integer on macOS but a struct on Linux. The record declaration would be refused on one platform or the other.
   - **Option: call `exit` inside the signal function.** The program would stop without printing the last step or the summary, which is the wrong output. Also, `exit(code: 0)` is refused at compile time because the code is written as a number.

2. **The result type of `signal`.** C returns a function pointer, and the specification says a callback can never be a result.
   - **My choice:** `-> ptr`, thrown away with `_ =`.
   - **Option: no `->` at all.** clang might refuse this as a result-type mismatch (`void` against a function pointer).
   - **Risk:** the spec does not say that `ptr` matches a function-pointer result. If the compiler requires an exact match, both forms are refused. This is the step most likely to fail at compile time.

3. **The types of `SIGINT`/`SIGTERM` and of the signal function's argument.**
   - **My choice:** `i32`, matching C's `int`. This lets the constants be passed straight to `sig: i32`.
   - **Option: `i64`, as in the spec's `SQLITE_OK` example.** Passing them to `signal` would be refused because the types are mixed.
   - **Option: an `i64` argument in the function type.** It would not match C's `void (*)(int)` and would be refused.

4. **The signal function's unused parameter.**
   - **My choice:** `_: i32`.
   - **Option: a named parameter such as `sig: i32` that is never read.** This is a compile error for an unused parameter.

5. **Where the stop check happens.**
   - **My choice:** in the `while` condition, so only between steps. Every step that starts is finished and printed.
   - **Option: also check between `sleep` and `print`.** The program would stop with a step built and added to the list but not printed, against "finishes the step in hand".

6. **The N in the summary.**
   - **My choice:** the counter `n`. Its value is the number of steps printed, because every step that starts prints.
   - **Option: `texts.len()`.** It would give the same number. The spec does not state the type `len` returns, so I avoided it inside the `f`-string.
   - **Unused cell:** `texts` is still used, because `texts.push(text)` reads it. Without that read, the compiler would refuse it as a cell nothing uses.

7. **Exit code 0.**
   - **My choice:** let `main` return normally.
   - **Option: `exit(code: 0)`.** It is refused, because the spec rejects an exit code written as a number.

8. **Clearing the variable at startup.**
   - **My choice:** call `unsetenv` first.
   - **Option: skip it.** If `HEROES_STOP=1` were already in the environment, the program would print only `stopped after 0 steps`. That is a different answer, not a crash.

9. **Labels on `setenv`'s arguments.** `name` and `value` are both `cstr`, so named arguments are required; I gave all three labels. Calling it positionally would be refused.

10. **A signal during `sleep`.** The program installs a real handler, so `sleep` returns early with EINTR and the signal function runs right after it. The step then prints early, about 2.5 seconds in rather than 3. If the step must wait a full second, I would have to loop on `sleep`'s return value. I kept a single `sleep(1)`, as the task says.

## prediction

**Compiler.** I expect `heroes build main.hero` to accept the program and produce a binary. The main risk is the `-> ptr` result declared for `signal` (choice point 2). If that is refused, the error will be on that line.

**Output when sent `kill -INT` at about 2.5 seconds.**

- Step 1 sleeps from 0 to 1 s and prints `step 1`.
- Step 2 sleeps from 1 to 2 s and prints `step 2`.
- Step 3 starts its sleep at 2 s. SIGINT arrives at about 2.5 s and cuts the sleep short. The signal function sets the variable, then `step 3` prints. The loop check sees the stop request, so the program prints the summary and exits with code 0.

```
step 1
step 2
step 3
stopped after 3 steps
```

`kill -TERM` or Ctrl-C at the same moment gives the same output.

## context

The only inputs that reached me were `brief.md`, `task.md` and `heroes-spec.md` from this directory, plus the system prompt (Claude agent, the working-directory path and the date). No other instructions, rules, contracts, memories or documents were in my context.
````

### t2-x-3 (`1bf6d1`, from `result`)

````markdown
I couldn't create `report.md` because the only tool I have here is Read. As the brief instructs for that case, the whole report follows as my final answer.

---

# report.md

## answer

The task asks for `main.hero`. Twenty times over, it builds the texts `item 1` … `item 1000000` into a fresh list, then prints `built 20000000 items`. While it works, it counts the SIGUSR1 signals it receives and then prints `received M signals`. It must not crash or stop early, and it must end with exit code 0.

The specification has no signal facility of its own. Section 13 says anything beyond the document comes from C libraries, and that threads come from `hero_os.h` (`hero_thread_spawn` / `hero_thread_join`). The language has no mutable globals and no closures, so a C signal handler written in Heroes would have nowhere to keep a count. The design is therefore:

1. `main` first blocks SIGUSR1 and SIGUSR2 with `pthread_sigmask`. Signals sent to the process then become pending instead of killing it with their default action.
2. `main` starts a thread running `count_signals`. That thread loops on `sigwait`, counting each SIGUSR1. When it receives SIGUSR2 it returns its count, which is the thread's one `i64` result.
3. `main` does the work. Then it sends SIGUSR2 to its own process with `kill(getpid(), SIGUSR2)`, joins the thread to get the count, and prints both lines. Returning from `main` gives exit code 0.

There is one file.

### `main.hero`

```
# Builds twenty lists of a million texts, counting the SIGUSR1 signals
# that arrive meanwhile.
#
# SIGUSR1 and SIGUSR2 are blocked before any work starts, so a signal
# never runs its default action (ending the process); a counting thread
# takes them one by one with `sigwait`. SIGUSR2, sent by the program to
# itself when the work is done, tells the counting thread to stop.

extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signum: i32) -> i32
    function pthread_sigmask(how: i32, set: sigset_t lent, oldset: ptr) -> i32
    function sigwait(set: sigset_t lent, @sig: i32) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "unistd.h"
    function getpid() -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

## Signals

# The set holding SIGUSR1 and SIGUSR2.
function signal_set() -> sigset_t
    set @= sigset_t(rest: zero)
    _ = sigemptyset(@set)
    _ = sigaddset(@set, SIGUSR1)
    _ = sigaddset(@set, SIGUSR2)
    return set

# Blocks SIGUSR1 and SIGUSR2 in the calling thread; threads it starts
# afterwards inherit the mask.
function block_signals()
    set = signal_set()
    _ = pthread_sigmask(SIG_BLOCK, set, nullptr)

# Body of the counting thread: counts SIGUSR1 until SIGUSR2 arrives,
# and answers the count.
function count_signals(_: i64) -> i64
    block_signals()
    set = signal_set()
    count @= 0
    sig: i32 @= 0
    while true
        rc = sigwait(set, @sig)
        if rc != 0
            continue
        if sig == SIGUSR2
            return count
        if sig == SIGUSR1
            count @ count + 1

## Work

function main()
    block_signals()
    counter = hero_thread_spawn(count_signals, 0)
    total @= 0
    for _ in range(from: 0, to: 20)
        items: [str] @= []
        i @= 1
        while i <= 1_000_000
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
    _ = kill(pid: getpid(), sig: SIGUSR2)
    received = hero_thread_join(counter)
    print(f"built {total} items")
    print(f"received {received} signals")
```

## choice_points

1. **Where the signals are received.** I chose to block the signals and collect them with `sigwait` on a thread from `hero_os.h`. The other option was a C handler installed with `signal(SIGUSR1, on_usr1)` and written as a Heroes callback. That handler could count nothing: there are no mutable globals and no closures, and a handler has no `@` parameter. It would have to talk to C, for example writing a byte to a pipe at a fixed fd. That pipe fills under a flood of signals, after which the handler blocks or the count is wrong, and the spec says nothing about whether a Heroes function is safe to run as a handler. Result: a program that runs and gives a different answer, or hangs.
2. **Blocking in `main` before the thread is spawned.** If only the thread blocked SIGUSR1, a signal sent to the process could still go to the main thread, where the default action ends the process. Result: a program that stops early and does not exit with 0.
3. **How `sigset_t` is declared.** I used `record sigset_t partial`, with no fields and built with `rest: zero`. I chose this because glibc defines `sigset_t` as a typedef of an untagged struct, and `partial` keeps C's size. One alternative was declaring it as a handle with `tag`. That would make it a pointer, which is the wrong type for C. Result: refused by the compiler. Another alternative was an `@set: u32` out-parameter. That matches macOS, where `sigset_t` is `__uint32_t`, but it is refused on Linux. **This is a platform choice: I targeted Linux/glibc.** On macOS the partial record over a scalar typedef may be refused.
4. **`sigwait`/`pthread_sigmask` take `set: sigset_t lent`.** The spec says a group's record declared `lent` without `@` is a pointer to const, which matches `const sigset_t *`. Without `lent` the set would be passed by value. Result: refused, because it disagrees with the header.
5. **`oldset: ptr` with `nullptr`.** I relied on "except … what a `ptr` points at" to let a `ptr` stand for `sigset_t *`. The alternative was `@old: sigset_t` with an extra cell. That should also compile, but it needs a cell whose value is never read.
6. **The `sigemptyset`/`sigaddset` out-parameters are `@` without `lent`.** C does not keep the place, so either form should be accepted. Adding `lent` would not change anything.
7. **Constants declared `i32`.** I declared them `i32`, C's `int`, so they can be passed to `i32` parameters without conversion. Declaring them `i64`, which is allowed because a constant may be wider, would force `to_i32(...).must()` at every use. Without that conversion the program is refused for mixing widths.
8. **Named arguments for `kill`.** Both of its parameters are `i32`, so the spec makes names mandatory: `kill(pid:, sig:)`. Positional arguments would be refused.
9. **Stopping the counter by sending SIGUSR2 to the process.** I used `kill(getpid(), …)`. `raise(SIGUSR2)` would aim the signal at the main thread, where it stays blocked forever, and the counter thread would never wake. Result: `hero_thread_join` hangs and the program never ends.
10. **Signals that pile up.** Standard signals do not queue, so several SIGUSR1 sent while one is already pending merge into one. M counts the signals **received**, as the task says, and can be smaller than the number sent. The counter thread spends almost all its time inside `sigwait`, so few are lost. Linux `sigwait` takes the lowest-numbered pending signal first, so a SIGUSR1 that is pending when SIGUSR2 arrives is still counted. A polling design in the main loop would lose many more and report a smaller M.
11. **When `rc != 0`, the thread does `continue` rather than `return`.** `sigwait` cannot fail with a valid set, apart from EINTR on some systems, which `continue` handles. Returning instead would leave the SIGUSR2 unread and give a count that is too small.
12. **`items.len()` is assumed to be `i64`.** The spec never gives the result type of `len`. If it were `u64`, `total + items.len()` mixes `i64` and `u64`. Result: refused. Adding `total @ total + 1` inside the loop would avoid the question.
13. **`_` as the loop variable and as a parameter.** I wrote `for _ in range(...)` and `count_signals(_: i64)`, relying on "`_` names anything you do not use". A named variable would be refused as unused.
14. **`while` for the inner loop.** I used a `while` loop instead of `for i in range(from: 1, to: 1_000_001)`. Both give the same output; the `for` form builds an extra array of a million `i64` each round.
15. **The `hero_os.h` group has no `link`.** The spec says these functions are "linked where a group names one". Adding a `link` for some other library would fail to link.
16. **No feature macros.** I assumed the generated C compiles with POSIX declarations visible. Under a strict `-std=c11`, glibc hides `kill`, `sigwait` and `pthread_sigmask`. The spec mentions `_GNU_SOURCE` but gives no syntax for defining it. Result if the assumption is wrong: refused by clang.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program on Linux/glibc. The main risks are points 3, 5, 12 and 16. On macOS, point 3 (`sigset_t` is a scalar there) is the most likely reason for a refusal.
- **Run:** after a few seconds of work (20 million formatted strings), it prints
  ```
  built 20000000 items
  received M signals
  ```
  where M is the number of SIGUSR1 signals taken during the run. M is 0 if none were sent. It can be less than the number sent, because pending standard signals merge. Exit code 0.
- **Signals that arrive at other moments:** one sent after the counter stops stays pending and blocked, so it is harmless and the exit code is still 0. One that arrives before `block_signals()` runs, during runtime start-up, would still end the process. Heroes has no way to block it any earlier.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The only other inputs were the agent framing: a system note that I am an agent on the Claude Agent SDK, the working-directory path and the date, plus the user's one-line instruction. No other rule, contract, memory or document was present, and I read no other file.
````

### t2-r-3 (`056a0c`, from `result`)

````markdown
I can only read files here, so I couldn't create `report.md`. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

The program counts SIGUSR1 with a small C header that it ships. The spec requires this: "A function given to `signal` stops the program the first time a signal calls it: no code of the program runs inside a signal handler, so write the handler in C, in a header the program ships." The Heroes code reaches the header through an `extern` group with no `link`, because the header needs no library.

### `main.hero`
```
# Counts SIGUSR1 while building twenty lists of a million texts.

extern "sigcount.h"
    function sigcount_install() -> i64
    function sigcount_read() -> i64

# Builds `item 1` to `item 1000000` into a fresh list and gives its length.
function build_round() -> i64
    items: [str] @= []
    n @= 1
    while n <= 1_000_000
        items @ items.push(f"item {n}")
        n @ n + 1
    return items.len()

function main()
    if sigcount_install() != 0
        print("cannot install the SIGUSR1 handler")
        exit(code: 1)
    total @= 0
    rounds @= 0
    while rounds < 20
        total @ total + build_round()
        rounds @ rounds + 1
    print(f"built {total} items")
    print(f"received {sigcount_read()} signals")
```

### `sigcount.h`
```c
#ifndef HEROES_SIGCOUNT_H
#define HEROES_SIGCOUNT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <signal.h>
#include <stdatomic.h>
#include <string.h>

/* Lock-free on every 64-bit target, so safe to touch from a handler,
   and safe if two threads of the process each take one signal at once. */
static _Atomic long long sigcount_received = 0;

static void sigcount_on_usr1(int signo)
{
    (void)signo;
    atomic_fetch_add_explicit(&sigcount_received, 1, memory_order_relaxed);
}

/* 0 on success, -1 on failure. SA_RESTART restarts the program's own
   system calls (writes, allocations) instead of failing them with EINTR. */
static int sigcount_install(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_handler = sigcount_on_usr1;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    return sigaction(SIGUSR1, &action, NULL);
}

static long long sigcount_read(void)
{
    return atomic_load_explicit(&sigcount_received, memory_order_relaxed);
}

#endif
```

The build command is `heroes build main.hero`, with `sigcount.h` in the same directory. No `heroes.toml` is needed because no package is used.

## choice_points

1. **Handler in Heroes or in C.** I chose C, as the spec says. If I had declared `signal` or `sigaction` in an extern group and passed it a Heroes function, the program would compile and then stop at the first SIGUSR1, which fails the task.
2. **`signal()` or `sigaction()` in the header.** I chose `sigaction` with `SA_RESTART`. `signal()` on glibc, in a strict-ISO build, uses System V semantics: the handler resets to default after one delivery, so the second SIGUSR1 would kill the process. Without `SA_RESTART`, a signal could interrupt the runtime's `write` during `print` with EINTR and fail the run.
3. **Defining `_GNU_SOURCE` in the header.** I define it, guarded. Without it, a strict `-std=c11` build on glibc hides `struct sigaction`, and clang would refuse the program. The spec allows a group header that defines `_GNU_SOURCE` if it comes first; this module has only one group. If the compiler's generated C includes a system header before mine, the define has no effect, but the guard prevents any redefinition error.
4. **The counter's type.** I chose `_Atomic long long`. `volatile sig_atomic_t` with `++` would also work if only one thread ever takes the signal. If the runtime has several threads, two handlers could run at once and lose counts, giving a smaller M. `sig_atomic_t` is also only 32 bits.
5. **Result widths.** I declared both results as `i64` because the spec allows "a result may be wider than C's". Declaring `sigcount_install` as `i32` would also compile; then the `0` in `!= 0` takes type `i32`. Declaring a narrower type than C's would be refused.
6. **The header name.** I wrote `extern "sigcount.h"` and assume the compiler finds it beside `main.hero`. If it only searches system paths, the build fails because the header is missing.
7. **Loop form.** I used `while` loops with counters. `for i in range(from: 1, to: 1_000_001)` would also work, but it allocates a 1M-element array each round. `for _ in range(...)` for the 20 rounds is risky: the grammar says `for ident`, and it is not clear that `_` is allowed there. If it isn't, the compiler refuses the program. Using a named loop variable that is never read would be refused as an unused binding.
8. **Install failure.** I print a message and call `exit(code: 1)`. The spec text "one written as a number is refused" could also be read as forbidding any literal code. Under that reading the compiler refuses this line, and the fix is to remove the `if` block. An `assert` would abort instead.
9. **Building the text.** I used `f"item {n}"`. `"item " + to_str(n)` gives the same result.
10. **What counts as a signal.** M is the number of handler runs. The kernel merges a SIGUSR1 that arrives while another is still pending, so M can be lower than the number of `kill` calls. No user-space design can change that for standard signals.
11. **When the count is read.** I read it after the 20 rounds, inside the second `print`. Signals arriving between the two prints are counted; signals after the read are handled and ignored, and the process still exits 0.

## prediction

- **Compiler:** I expect it to accept the program: no holes, no unused bindings, every cell is re-bound, and the extern results are as wide as or wider than C's. The main risks are points 3, 6 and 8 above.
- **Output with no signals:**
  ```
  built 20000000 items
  received 0 signals
  ```
- **Output under `kill -USR1`:** the same first line, then `received M signals`, where M is the number of deliveries (point 10). Exit code 0.
- **Unavoidable gap:** a SIGUSR1 that arrives before `sigcount_install` runs, at process start, gets the default action and ends the process.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context, plus the agent's system prompt (working directory and date; it is a generic SDK prompt with no language rules). No other instruction, rule, contract, memory or document reached me, and I read no other file.
````

### t2-e-3 (`0d036a`, from `result`)

````markdown
I only have a Read tool here, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for `main.hero`. It builds the texts `item 1` to `item 1000000` into a fresh list twenty times. While it does that, it counts the SIGUSR1 signals it receives. Then it prints `built 20000000 items`, then `received M signals`, and exits with code 0.

How it works:
- The specification has no mutable globals, so a signal handler can't add to a counter in the program.
- Instead, the count lives in a pipe. The handler writes one byte into the pipe each time it runs.
- To let the handler find the pipe without a global, the write end is moved to a fixed descriptor number, the constant `WRITE_FD`.
- A thread started with `hero_thread_spawn` reads the pipe all the time and adds up the bytes, so the pipe never fills.
- When the work is done, `main` closes the write end. The counting thread then reaches end-of-file and returns its total through `hero_thread_join`.
- The handler stays installed until the program ends. A late SIGUSR1 therefore never meets the default action, which would kill the process. A late write goes to a closed descriptor and fails harmlessly, because the read end stays open (so there is no SIGPIPE).

### `main.hero`

```
extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "unistd.h"
    function pipe(@fds: [i32] counted_by 2 lent) -> i32
    function dup2(fildes: i32, fildes2: i32) -> i32
    function close(fd: i32) -> i32
    function read(fd: i32, @buf: [u8] counted_by nbyte lent, nbyte: u64) -> i64
    function write(fd: i32, @buf: [u8] counted_by nbyte lent, nbyte: u64) -> i64

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The descriptor the pipe's write end is moved to, so the handler can reach it
# without a global.
constant WRITE_FD: i32
    100

# Runs on the runtime's signal thread, once per SIGUSR1 delivered: one byte per signal.
function on_usr1(_: i32)
    one: [u8] @= [1]
    _ = write(WRITE_FD, @one, 1)

# Counting thread: adds up the bytes read from the pipe until every write end is closed.
function count_bytes(fd: i64) -> i64
    rfd = to_i32(fd).must()
    total @= 0
    buf: [u8] @= []
    while true
        n = read(rfd, @buf, 4096)
        if n > 0
            total @ total + n
        else if n == 0
            return total
        # n < 0 (for example EINTR): try again

function main()
    fds: [i32] @= []
    assert pipe(@fds) == 0
    assert dup2(fildes: fds[1], fildes2: WRITE_FD) == WRITE_FD
    _ = close(fds[1])
    counter = hero_thread_spawn(body: count_bytes, arg: to_i64(fds[0]).must())
    _ = signal(SIGUSR1, on_usr1)

    total @= 0
    for _ in range(from: 0, to: 20)
        items: [str] @= []
        for i in range(from: 1, to: 1000001)
            items @ items.push(f"item {i}")
        total @ total + items.len()
    print(f"built {total} items")

    _ = close(WRITE_FD)
    received = hero_thread_join(counter)
    print(f"received {received} signals")
```

No other file is needed. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **No global state, so where to keep the count.**
   - Choice: a pipe at a fixed descriptor number (100), plus a counting thread.
   - Alternative A: draining the pipe only at the end. A flood of signals would fill the pipe buffer (about 16–64 KiB). The handler's `write` would then block the runtime's signal thread for good, and later signals would be lost or would stall: the program runs but gives the wrong answer.
   - Alternative B: a named semaphore. On macOS `sem_getvalue` doesn't work, so the program runs but gives the wrong answer there.
   - Risk in my choice: if the runtime already uses descriptor 100, `dup2` silently closes it and the program misbehaves while running.

2. **The result type of `signal`.**
   - Choice: `-> ptr`.
   - C returns a function pointer, and the specification says a callback is never a result. Clang might reject `ptr` against `sighandler_t`, so the compiler might refuse the program.
   - Alternative: leaving out `->` (meaning `void`). This could equally be refused by the header check.
   - This is the choice point I'm least sure of.

3. **Marking `write`'s buffer with `@`.**
   - Choice: `@buf: [u8] counted_by nbyte lent`, following the specification's only example of `counted_by` on an array.
   - The specification defines a `lent` without `@` (a pointer to const) only for records. If it allows that for arrays too, my version still compiles; I just declare more access than C uses.
   - If arrays don't allow it, writing `buf: [u8] counted_by nbyte lent` would get the program refused.

4. **The C callback type.**
   - Choice: `(function(i32) -> ())`, since C's handler takes an `int`.
   - Writing `i64` would get the program refused, because callback parameters must match the header's width.

5. **The type of `SIGUSR1`.**
   - Choice: `i32`, from the header rather than the number. The value is 30 on macOS and 10 on Linux.
   - Hard-coding a number would run, but on the other platform it would install a handler for the wrong signal. SIGUSR1 would then kill the process.

6. **The loop variable `_` in `for _ in range(...)`.**
   - The grammar writes `ident` there, and `_` "names anything you do not use".
   - A named variable that isn't used would get the program refused as an unused binding.

7. **What `len()` returns.**
   - Choice: I assumed `i64`, so `total + items.len()` typechecks.
   - If it returns `u64`, the program is refused for mixing types. The fix would be `total: u64 @= 0`.

8. **A negative return from `read`.**
   - Choice: retry, treating it as EINTR.
   - Alternative: returning the total there. The program would run and could undercount.

9. **Startup failures.**
   - Choice: `assert` for `pipe` and `dup2`, because the specification says `exit` with a literal code is refused.
   - If they fail, the program stops with an error. That is acceptable, since it can't work without the pipe.

10. **When the handler is installed and removed.**
    - It is installed before the work starts and never removed.
    - Resetting to the default afterwards would let a late SIGUSR1 kill the process, making it stop with an error.
    - A SIGUSR1 that arrives before installation still uses the default action and kills the process. Nothing in the specification can stop that.

11. **Whether every signal is counted.**
    - The count is the handler runs the runtime delivers. Standard POSIX signals that arrive together while one is pending merge into one.
    - So M is the number of signals *received*, which can be fewer than the number of `kill`s sent. The specification doesn't say whether the runtime merges them further.

## prediction

I expect the compiler to accept the program. The doubtful points are the result type of `signal` (choice point 2), the type of `len()` (choice point 7) and the `@` on `write`'s buffer (choice point 3).

It prints:
```
built 20000000 items
received M signals
```
- M is the number of SIGUSR1 deliveries during the work. It is `received 0 signals` if none are sent.
- The exit code is 0.
- It doesn't crash however fast the signals come, because the counting thread keeps the pipe drained.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached me. No other project file, rule, contract or memory did. The one other input was my general system setup: I'm an agent built on the Claude Agent SDK with a file-reading tool, and the working-directory path suggests macOS.
````

### t2-m-3 (`ff7f62`, from `result`)

````markdown
I can't write `report.md` because I only have a file-reading tool, so the whole report follows here, as the brief asks.

# report.md

## answer

**What the task asks:** a program, `main.hero`, that runs 20 rounds. Each round builds the texts `item 1` … `item 1000000` into a fresh list. When the rounds are done it prints `built 20000000 items`, then `received M signals`, where M counts the SIGUSR1 signals that arrived while it worked. It must survive any stream of SIGUSR1 and exit with code 0.

**Why it is built this way:** section 13 of the specification says a callback is "never a result or a signal handler, nor called by one". That rules out a SIGUSR1 handler written in Heroes, and it also rules out C's default action for SIGUSR1, which kills the process. So the program works like this:

1. The first thing `main` does is block SIGUSR1 and SIGUSR2 (with `pthread_sigmask`).
2. It then starts a thread with `hero_thread_spawn`. The thread inherits that block, waits for signals in a loop with `sigwait`, and counts each SIGUSR1.
3. When the work is done, `main` sends SIGUSR2 to its own process to tell the thread to stop.
4. `hero_thread_join` returns the count. A thread can return only one `i64`, which is exactly the count.

There is only one file.

### `main.hero`
```
extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signo: i32) -> i32
    function pthread_sigmask(how: i32, set: sigset_t lent, oset: ptr) -> i32
    function sigwait(set: sigset_t lent, @sig: i32) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "unistd.h"
    function getpid() -> i32

constant ROUNDS: i64
    20

constant ITEMS: i64
    1000000

# The set {SIGUSR1, SIGUSR2}: SIGUSR1 is counted, SIGUSR2 tells the counter to stop.
function usr_signals() -> sigset_t
    set: sigset_t @= sigset_t(rest: zero)
    assert sigemptyset(@set) == 0
    assert sigaddset(@set, SIGUSR1) == 0
    assert sigaddset(@set, SIGUSR2) == 0
    return set

# Runs on its own thread, which inherits main's blocked mask: takes every
# SIGUSR1 synchronously and counts it, until main sends SIGUSR2.
function count_signals(_: i64) -> i64
    set = usr_signals()
    count @= 0
    while true
        sig: i32 @= 0
        assert sigwait(set, @sig) == 0
        if sig == SIGUSR2
            return count
        count @ count + 1

function main()
    blocked = usr_signals()
    assert pthread_sigmask(SIG_BLOCK, blocked, nullptr) == 0
    counter = hero_thread_spawn(body: count_signals, arg: 0)
    built @= 0
    for _ in range(from: 0, to: ROUNDS)
        items: [str] @= []
        for i in range(from: 1, to: ITEMS + 1)
            items @ items.push(f"item {i}")
        built @ built + items.len()
    assert kill(getpid(), SIGUSR2) == 0
    received = hero_thread_join(counter)
    print(f"built {built} items")
    print(f"received {received} signals")
```

## choice_points

1. **How to catch SIGUSR1.** The specification forbids a Heroes callback as a signal handler, so I block the signal and take it with `sigwait` on a thread. The alternatives:
   - Passing a Heroes function to `signal`/`sigaction`: the compiler refuses it.
   - Doing nothing: the first SIGUSR1 ends the process (C's default action), so the program stops with an error.
   - Blocking SIGUSR1 and only checking `sigpending` at the end: it runs, but always reports 0 or 1, a different answer.
2. **What a "received" signal is.** SIGUSR1 is a standard signal, so several sent while one is pending merge into one. I count deliveries (one per `sigwait` return), so M can be less than the number of `kill` calls a sender made. No program, even in C, can count every sender's `kill`. If the author meant "number sent", this gives a different (lower) answer under bursts.
3. **How the counter learns the work is done.** Only one `i64` goes in and one comes out of a thread, so I send SIGUSR2 to the process. The alternatives:
   - Sending SIGUSR1 as the stop signal: it is lost if a SIGUSR1 is already pending, and the program can hang.
   - Using a real-time signal (`SIGRTMIN`): it is a function-call macro on glibc, so the constant is likely refused.
   - With my choice, a SIGUSR2 from an outside process would stop the count early. The task says only SIGUSR1 is sent.
4. **Stop and count racing at the end.** If SIGUSR1 and SIGUSR2 are pending together, `sigwait` may take SIGUSR2 first. That last SIGUSR1 is then not counted. I treat it as arriving after the work ended; another reading would expect a count one higher.
5. **`pthread_sigmask` rather than `sigprocmask`.** `sigprocmask` is unspecified in a threaded process. It would probably still work, but it is not guaranteed.
6. **When the block takes effect.** It is the first statement of `main`. A SIGUSR1 that arrives before that (during start-up) still kills the process; I cannot prevent that from Heroes. If the runtime has its own threads that started before `main` and don't block SIGUSR1, a signal sent to one of them would also kill the process. The specification says nothing about this.
7. **`sigset_t` as `record sigset_t partial` with no fields, built with `rest: zero`.** The specification doesn't say whether a record with no fields and no tag is allowed. On Linux, `sigset_t` is a typedef of a struct, so this should work. On macOS, `sigset_t` is a `__uint32_t` and `sigemptyset`/`sigaddset` are macros, so the compiler probably refuses it there. This program targets Linux/glibc.
8. **Types of the C constants.** I declared them as `i32`, the width C uses. The other choice, `i64`, would be allowed ("a result may be wider"), but then every call taking an `int` would need `to_i32(...).must()`.
9. **`oset: ptr` with `nullptr`.** The alternative is `@oset: sigset_t`, which takes a throwaway cell; that compiles too and gives the same output.
10. **`_` as the loop variable and the thread parameter.** The specification lets `_` name "a parameter, a value". If the grammar's `ident` doesn't include `_`, the compiler refuses it. Using a name instead would be refused as an unused binding.
11. **`assert` on C results outside tests.** I assumed it aborts at run time when false. If asserts were ignored in ordinary builds, the output would be the same, because none of these calls fails.

## prediction

- **Compiler:** I expect it to accept the program on Linux/glibc. It probably refuses it on macOS (see choice point 7).
- **Output:** two lines, then exit code 0:
  ```
  built 20000000 items
  received M signals
  ```
  M is the number of SIGUSR1 deliveries during the run: 0 if none were sent, and possibly less than the number sent, because bursts merge.
- **Crashes:** the program does not crash or stop early however fast SIGUSR1 comes, apart from a signal arriving before `main` blocks it (choice point 6).

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The only other input was the general agent system prompt (working directory, date, tool rules); it contained no rules, memories or documents about Heroes. I used my general knowledge of POSIX signals.
````

### t2-s-3 (`5c1359`, from `result`)

````markdown
I only have a Read tool, so I could not create `report.md`. As the brief allows, the whole report is below.

# Report

## answer

**Task:** write `main.hero`. It builds a fresh list of `"item 1"` … `"item 1000000"` twenty times, then prints `built 20000000 items`. It counts the SIGUSR1 signals it receives while working, prints `received M signals`, and exits with code 0. It must not crash however the signals arrive.

**How it works:** the spec has no mutable globals and no anonymous functions. A signal handler is a top-level function taking one `i32` and returning nothing, so it cannot reach a Heroes variable. The count therefore lives on the C side, in a process environment variable. Section 13 says a function given to `signal` "runs after the signal, never inside it … once that thread is back in the program's own code". That makes it safe for the handler to call `getenv`, `atol` and `setenv`. `main` sets the variable to `"0"` before installing the handler, so the handler always finds it set. One file:

### `main.hero`

```
# Counts SIGUSR1 while building twenty lists of a million texts.

extern "signal.h"
    constant SIGUSR1: i32
    function signal(signum: i32, handler: (function(i32) -> ())) -> ptr

extern "stdlib.h"
    function getenv(name: cstr lent) -> cstr
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function atol(text: cstr lent) -> i64

# The environment variable that holds the count; the C side keeps it,
# since the language has no mutable globals.
constant COUNT_VAR: str
    "HEROES_USR1_COUNT"

# Runs after each delivered SIGUSR1, outside the signal context.
function on_usr1(_: i32)
    current = getenv(COUNT_VAR.cstr())
    n = atol(current)
    text = to_str(n + 1)
    _ = setenv(name: COUNT_VAR.cstr(), value: text.cstr(), overwrite: 1)

function main()
    _ = setenv(name: COUNT_VAR.cstr(), value: "0".cstr(), overwrite: 1)
    _ = signal(SIGUSR1, on_usr1)
    total @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    print(f"built {total} items")
    received = atol(getenv(COUNT_VAR.cstr()))
    print(f"received {received} signals")
```

No `heroes.toml`, because the program uses no package. No other file is needed.

## choice_points

1. **Where the count lives (C's environment):** The language has no mutable globals and no closures, so the count has to be stored outside Heroes.
   - **Alternative:** keep the count in a file using `read_file`/`write_file`. This would run, but it's slow under a flood of signals, leaves a file behind, and could fail.
   - **Alternative:** block SIGUSR1 and poll it with `sigprocmask` plus `sigtimedwait`/`signalfd`. That needs `sigset_t`/`timespec` records and out-parameters, so the compiler is more likely to refuse it. It would also merge signals between polls, giving a lower M.
   - **Alternative:** try to count in a Heroes variable. The compiler refuses this: there is no global, and no closure to capture one.

2. **Using `signal` rather than `sigaction`:** `sigaction` stores the handler in a struct field. A field cannot hold a function, and on glibc `sa_handler` is a union-member macro, so the compiler would refuse it. The spec names `signal` explicitly.

3. **Result type of `signal` declared as `ptr`:** In C it returns `sighandler_t`, a function pointer. The spec forbids a callback as a result and says clang checks result types. `ptr` is my guess at a matching type.
   - **Alternative:** declare no result (`()`). If clang compares `void` with `sighandler_t`, the compiler refuses that.
   - **Risk:** if clang also refuses `void *` against a function-pointer type, my declaration is refused instead. This is the main uncertainty in the program.

4. **Handler parameter type `i32` with no result:** C's `void (*)(int)` becomes `(function(i32) -> ())`, following "same rule, `()` is `void`". Using `i64` would be refused as a width mismatch.

5. **`SIGUSR1` declared as `i32`:** Declared as `i64`, it would no longer match `signum: i32`. The call would then be refused, because there are no implicit conversions.

6. **Setting the variable to `"0"` before installing the handler:** If the handler were installed first, a signal could arrive while `getenv` returns null. `atol(NULL)` would then crash at run time.

7. **Installing the handler first thing in `main`:** Installed any later, an early SIGUSR1 would hit the default action and kill the process. A signal that arrives before `main` starts can't be caught by any program written this way.

8. **Parsing the number with C's `atol`:** The spec gives no string-to-integer built-in. The alternative is a hand-written digit loop over `validated()`, which would work but is longer.

9. **Named arguments for `setenv`:** It has two `cstr lent` parameters that share a type, so the spec makes names mandatory. Positional arguments would be refused.

10. **`while` with counters instead of `for _ in range(...)`:** The `For` production requires an `ident`. `_` may not count as one, and a named loop variable left unused would be a compile error. Also, `range` would allocate a million-element array for the inner loop.

11. **Fresh `items: [str] @= []` each round:** The annotation is required for an empty literal. Growing the list with `items @ items.push(...)` on a plain name grows it in place. Pushing through a field would be quadratic in time but give the same answer.

12. **Discarding with `_ = ...`:** `setenv` and `signal` produce values (`i32`, `ptr`) and are not fallible, so `_ =` is allowed. Leaving them as bare lines would be refused, since a computed value must be used.

13. **What "received" counts:** M counts handler runs. Standard signals that arrive while one is already pending merge into one, and the runtime may merge further before running the handler. So M can be smaller than the number of `kill` calls. Exactly how the runtime defers signals isn't specified.

14. **`COUNT_VAR` as a `str` constant with `.cstr()`:** I assume a `str` constant may be lent with `.cstr()` as an argument. Writing the string literal in each call instead would behave the same.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program. The main risk is the `-> ptr` result on `signal` (point 3). If clang rejects `void *` against `sighandler_t`, the build fails at that declaration.
- **Output:** two lines, then exit code 0:
  ```
  built 20000000 items
  received M signals
  ```
  - With no signals sent, M is `0`.
  - Under `kill -USR1` traffic, M is the number of handler runs. That is at most the number of signals sent, and possibly fewer because pending signals merge.
- **Robustness:** the program doesn't crash under a flood of signals, because the handler runs outside the signal context.

## context

The only inputs that reached my context were `brief.md`, `task.md` and `heroes-spec.md` from this directory, plus the system prompt. That prompt identifies me as a Claude agent and gives the working directory and date. No other instruction, rule, contract, memory or document reached my context.
````

### t2-s-4 (`86745c`, from `result`)

````markdown
I only have a Read tool, so I couldn't create `report.md`. As the brief allows, the whole report is below. I haven't built or run anything, so whether the compiler accepts the program and what it prints are predictions.

# report.md

## answer

The task asks for `main.hero`. It builds the texts `item 1` … `item 1000000` into a fresh list twenty times and prints `built 20000000 items`. While it works it counts the SIGUSR1 signals it receives, then prints `received M signals` and exits with code 0.

The difficult part is the counter. The language has no mutable globals and no closures, and the function given to `signal` only receives the signal number. So the count has to live outside the language. I keep it in C, in the process environment: the handler reads the variable `HERO_SIGUSR1_COUNT` with `getenv`/`atol`, adds one, and writes it back with `setenv`.

This is safe only because the spec (§13) says the function given to `signal` runs after the signal, on the interrupted thread, once that thread is back in the program's own code. In other words it is never an async-signal context. The counter is set to 0 before the handler is installed, and the handler is installed first thing in `main`.

### `main.hero`

```
extern "stdlib.h"
    function getenv(name: cstr lent) -> cstr
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function atol(s: cstr lent) -> i64

extern "signal.h"
    constant SIGUSR1: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

# The environment variable that holds the signal count, kept on the C side.
constant COUNT_VAR: str
    "HERO_SIGUSR1_COUNT"

constant ROUNDS: i64
    20

constant ITEMS_PER_ROUND: i64
    1000000

# Reads the count of SIGUSR1 signals handled so far.
function read_count() -> i64
    value = getenv(COUNT_VAR.cstr())
    if value == nullptr
        return 0
    return atol(value)

# Stores a new count.
function write_count(n: i64)
    text = to_str(n)
    _ = setenv(name: COUNT_VAR.cstr(), value: text.cstr(), overwrite: 1)

# Runs after each SIGUSR1, back in the program's own code.
function on_usr1(_: i32)
    write_count(read_count() + 1)

function main()
    write_count(0)
    _ = signal(SIGUSR1, on_usr1)
    total @= 0
    round @= 0
    while round < ROUNDS
        items: [str] @= []
        i @= 1
        while i <= ITEMS_PER_ROUND
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
        round @ round + 1
    print(f"built {total} items")
    print(f"received {read_count()} signals")
```

Build it with `heroes build main.hero`. It uses no packages, so there is no `heroes.toml`.

## choice_points

1. **Where the counter lives.** I keep it in the C environment (`setenv`/`getenv`/`atol`). The alternatives:
   - **Self-pipe on a fixed descriptor:** the pipe fills after about 64 KiB. It then either blocks (the program hangs) or drops writes (wrong count).
   - **A file written through `write_file`:** this works but does I/O for every signal.
   - **Blocking the signal and polling `sigpending`:** this counts at most one per poll, which gives a wrong, lower answer.
   - **A Heroes-side counter:** impossible. There are no mutable globals and no closures, so the compiler refuses it.
2. **The result type of `signal`.** I declared `-> ptr` for C's `void (*)(int)`.
   - The spec only says "a result may be wider than C's", so the compiler might refuse `ptr` against a function-pointer type.
   - If I declared no result (`()`, which is void), clang would probably refuse it as a mismatched result type.
   - Declaring `(function(i32) -> ())` is refused outright: "a callback is a parameter, never a result".
3. **`signal` rather than `sigaction`.** The spec names `signal` explicitly as the call whose function "runs after the signal". `sigaction` would need a record of its struct, including a union holding the handler, and it isn't clear the deferred-run promise covers it. The likely result would be refusal or a handler run in signal context.
4. **Header choice.** I used `signal.h` and `stdlib.h`. The spec says processes come from `hero_os.h`, but it doesn't list a signal function there. Guessing a `hero_os.h` symbol would risk a compile refusal for an unknown name.
5. **Type of `SIGUSR1`.** I declared `i32`, matching C's `int`, so it passes straight to `sig: i32`. Declaring `i64`:
   - would probably be accepted as a constant;
   - but then `signal(SIGUSR1, …)` is refused for mixing widths;
   - unless it's wrapped as `to_i32(SIGUSR1).must()`.
6. **`atol` vs parsing in Heroes.** There is no str→int built-in, so I used `atol`, assuming `long` is 64 bits (Linux and macOS). On a 32-bit-`long` platform, clang would refuse the `i64` parameter/result check, or the count would wrap above 2³¹ (wrong answer).
7. **`nullptr` check in `read_count`.** Without it, passing a null `getenv` result to `atol` would crash. That can't happen here, because main sets the variable first.
8. **Loops.** I used `while` with counters. `for r in range(...)` with `r` unused is a compile error (unused binding). It's also unclear whether `for _ in` parses, since the production says `ident`.
9. **`_` as the handler's parameter.** A named, unused parameter is a compile error.
10. **Ignoring results.** `_ = signal(...)` and `_ = setenv(...)` are allowed because neither result is a `T?`. Writing the bare call would be refused ("a line that computes a value must use it").
11. **Signal coalescing.** The spec doesn't say whether the runtime queues one handler run per signal or merges signals that are pending together. Standard POSIX signals merge. So M is the number of handler runs, which can be lower than the number of `kill` calls when the signals come in a burst. No choice of mine avoids this, because the kernel merges them. The program never crashes over this.
12. **Signals before `signal(...)` runs.** In the short window before the handler is installed, SIGUSR1's default action kills the process. Installing it first in `main` makes that window as small as possible. The language offers no way to remove the window.

## prediction

- **Compiler:** I expect it to accept the program. The weakest point is the `-> ptr` result declared for `signal` (choice point 2). If that is refused, it's a compile error at that line and nothing else changes.
- **Output with no signals:**
  ```
  built 20000000 items
  received 0 signals
  ```
- **Output with N `kill -USR1` sent:** `received M signals`, where M ≤ N and M = N when the signals are spaced out (see choice point 11). The program doesn't crash however the signals come, and it exits with code 0.

## context

No instruction, rule, contract, memory or document reached my context other than `brief.md`, `task.md` and `heroes-spec.md` in this directory. The only other input was the general system prompt for this agent session: working directory, date, and tool-use conventions.
````

### t2-m-4 (`41fe36`, from `result`)

````markdown
I couldn't write `report.md` because I have no file-writing tool, so the whole report is below as the brief allows.

# report.md

## answer

The task asks for `main.hero`. It builds `item 1` to `item 1000000` into a fresh list twenty times and prints `built 20000000 items`. It also counts the SIGUSR1 signals it receives and prints `received M signals`. It must never crash and must end with exit code 0.

**How it works.** Section 13 says a Heroes function can never be a signal handler ("A callback is a parameter, never a result or a signal handler, nor called by one"). So the program doesn't catch the signal. At the start it blocks SIGUSR1 with `pthread_sigmask`, so the signal can no longer kill the process and instead waits as "pending". After each item it builds, it calls `sigpending`. If SIGUSR1 is pending, it takes it off the queue with `sigwait`, which returns at once, and adds one to the count. SIGUSR1 stays blocked until the process exits, so a signal that arrives late just stays pending and is dropped at exit. The `sigset_t` buffers come from `malloc` as `ptr`, so their size and layout never have to be written in Heroes.

### `main.hero`

```
# Counts SIGUSR1 while building twenty lists of a million texts.

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

extern "signal.h"
    constant SIGUSR1: i32
    constant SIG_BLOCK: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function sigismember(set: ptr, signo: i32) -> i32
    function sigpending(set: ptr) -> i32
    function sigwait(set: ptr, @sig: i32) -> i32
    function pthread_sigmask(how: i32, set: ptr, oldset: ptr) -> i32

# Takes one pending SIGUSR1, if there is one, and says how many it took (0 or 1).
function poll(set: ptr, pending: ptr) -> i64
    assert sigpending(pending) == 0
    if sigismember(pending, SIGUSR1) == 1
        sig: i32 @= 0
        assert sigwait(set, @sig) == 0
        return 1
    return 0

function main()
    set = malloc(256)
    pending = malloc(256)
    assert set != nullptr
    assert pending != nullptr
    assert sigemptyset(set) == 0
    assert sigaddset(set, SIGUSR1) == 0
    assert pthread_sigmask(how: SIG_BLOCK, set: set, oldset: nullptr) == 0
    received @= 0
    built @= 0
    round @= 0
    while round < 20
        xs: [str] @= []
        for i in range(from: 1, to: 1000001)
            xs @ xs.push(f"item {i}")
            built @ built + 1
            received @ received + poll(set: set, pending: pending)
        round @ round + 1
    print(f"built {built} items")
    print(f"received {received} signals")
    free(set)
    free(pending)
```

There are no other files. The program uses no package, so there is no `heroes.toml`. Build it with `heroes build main.hero`.

## choice_points

1. **How to catch the signal.** The spec forbids a Heroes function as a signal handler, and no built-in deals with signals.
   - **Chosen:** block SIGUSR1 and poll for it with `sigpending` and `sigwait`.
   - **Other option:** declare `signal` or `sigaction` with a Heroes callback. The compiler refuses it.
   - **Other option:** don't handle the signal at all. The first `kill -USR1` uses the default action and ends the process (killed by a signal, not exit code 0).

2. **One thread with polling, or a second thread waiting in `sigwait`.**
   - **Chosen:** one thread that polls after every item.
   - **Other option:** a thread started with `hero_thread_spawn` that loops on `sigwait`. Only an `i64` can pass to or from that thread, so main would have to stop it with a second signal, such as sending itself SIGUSR2. If another process also sent SIGUSR2, the thread would stop early and the count would come out too low, even though the program still runs.
   - **Note:** in both designs, signals that arrive while one is already pending merge into one. That is how standard signals work, so M is the number of signals actually delivered, which can be less than the number sent in a flood. A C signal handler would lose them in the same way.

3. **How often to poll.**
   - **Chosen:** after every item. That is 20,000,000 extra `sigpending` system calls, which adds a few seconds of run time but merges the fewest signals.
   - **Other option:** poll every N items. It runs faster but merges more signals, so M comes out lower: a different answer, but no crash.

4. **Memory for the `sigset_t` sets.**
   - **Chosen:** `malloc(256)` as a `ptr`. That is more than the size on glibc (128 bytes) or macOS (4 bytes), and it avoids writing the layout as a group `record`.
   - **Other option:** a group `record sigset_t` with fields. The type is a struct on Linux and a plain `__uint32_t` on macOS, so clang would refuse the record on at least one of them.
   - **Other option:** a `[u8] counted_by` lend. The spec allows that only "where the header says `void *`", so it would be refused.

5. **Declaring `ptr` where C says `sigset_t *`.**
   - **Chosen:** `ptr`, relying on "except … what a `ptr` points at".
   - **Risk:** if that exception doesn't cover these parameters, the compiler refuses the program.
   - **Platform risk:** on macOS, `sigemptyset`, `sigaddset` and `sigismember` may also exist as macros in `<signal.h>`. If clang's check needs a real function, macOS could refuse the program. On Linux/glibc they are real functions.

6. **`pthread_sigmask` or `sigprocmask`.**
   - **Chosen:** `pthread_sigmask`, because the Heroes runtime might have threads.
   - **Other option:** `sigprocmask`. The C standard leaves its behaviour unspecified in a multithreaded process. It works the same in a single-threaded one.
   - **Risk either way:** if the runtime starts its own threads before `main` and they don't block SIGUSR1, a process-directed signal could reach one of them and kill the process. The spec doesn't say whether such threads exist.

7. **The time before the mask is set.** A SIGUSR1 that arrives before `pthread_sigmask` runs (during process start-up) still kills the process. No Heroes code can run before `main`, so I accepted this. The task's phrase "while it works" means after the mask is set.

8. **Integer widths in the declarations.**
   - **Chosen:** `i32` for `int` and for the constants, `u64` for `size_t`.
   - **Other option:** an `i64` parameter, which the spec says would be refused.
   - **Other option:** `constant SIGUSR1: i64`. The spec allows a wider constant, but then every call would need `.to_i32().must()`, and passing it straight to an `i32` parameter would be refused.

9. **The round loop.**
   - **Chosen:** a `while` loop with a `round` cell.
   - **Other option:** `for _ in range(...)`. The grammar says `"for" ident`, and `_` appears to be its own token, so this might be refused.
   - **Other option:** `for r in ...` without using `r`. That is refused as an unused binding.

10. **The `sig` out-cell in `poll`.** It is never read. I'm relying on "a read is a use and a write is not, except through an `@` parameter" to count `@sig` as a use. If that rule were read the other way, the compiler would refuse `sig` as unused.

11. **The item count.**
   - **Chosen:** an `i64` counter, `built`, increased once per push.
   - **Other option:** `xs.len()`. The spec never gives its result type. If it were `u64`, adding it to an `i64` would be refused as mixed widths.

12. **Labelled arguments.** `poll` has two `ptr` parameters and `pthread_sigmask` has two as well, so their calls use names, which the spec requires. Positional calls would be refused. The extern calls with only one `ptr` (`sigismember`, `sigwait`) are positional.

13. **Checking C results.**
   - **Chosen:** `assert … == 0`, which uses each result.
   - **Other option:** a bare call. It would be refused because the line computes a value it doesn't use.
   - **Other option:** `_ = …`. It would be accepted but would ignore errors.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program on Linux/glibc. On macOS there is some risk of refusal (choice point 5).
- **Output:**
  ```
  built 20000000 items
  received M signals
  ```
  - M is the number of distinct times a SIGUSR1 was found pending.
  - With no signals sent, it prints `received 0 signals`.
  - If signals are sent slowly, M equals the number sent. In a fast flood, M is lower, because signals that arrive between two polls merge into one.
- **Exit and stability:** exit code 0. It doesn't crash however fast the signals come, as long as they come after the mask is set (choice points 6 and 7).
- **Run time:** a few seconds longer than without the polling.

## context

Only the files in this directory reached me: `brief.md`, `task.md` and `heroes-spec.md`, plus the user's one-line instruction and my standard agent system prompt. No other project instructions, rules, contracts, memories or documents reached my context.
````

### t2-e-4 (`313ff5`, from `result`)

````markdown
I can only read files here, not write them, so I couldn't create `report.md`. As the brief says for that case, the whole report follows.

# report.md

## answer

The program counts SIGUSR1 itself, without the language's `signal` hook. The handler that `signal` runs can't keep a count, because the language has no mutable globals. So `main` first blocks SIGUSR1 and SIGUSR2. It then starts a thread with `hero_thread_spawn`; that thread waits for the signals with `sigwait` and counts each SIGUSR1. When the twenty rounds are done, `main` sends the process SIGUSR2. The thread sees it, returns its count, and `hero_thread_join` hands the count back to `main`. This uses the one channel the spec gives a thread: one `i64` in, one `i64` out.

### `main.hero`

```
# Counts SIGUSR1 while building twenty lists of a million texts.

extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signum: i32) -> i32
    function pthread_sigmask(how: i32, set: sigset_t lent, oldset: ptr) -> i32
    function sigwait(set: sigset_t lent, @sig: i32) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "unistd.h"
    function getpid() -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The set {SIGUSR1, SIGUSR2}.
function wait_set() -> sigset_t
    set @= sigset_t(rest: zero)
    _ = sigemptyset(@set)
    _ = sigaddset(@set, SIGUSR1)
    _ = sigaddset(@set, SIGUSR2)
    return set

# Runs on its own thread: counts SIGUSR1 until SIGUSR2 arrives.
function count_signals(_: i64) -> i64
    set = wait_set()
    _ = pthread_sigmask(SIG_BLOCK, set, nullptr)
    count @= 0
    while true
        sig: i32 @= 0
        _ = sigwait(set, @sig)
        if sig == SIGUSR2
            return count
        if sig == SIGUSR1
            count @ count + 1

function main()
    set = wait_set()
    _ = pthread_sigmask(SIG_BLOCK, set, nullptr)
    handle = hero_thread_spawn(body: count_signals, arg: 0)
    total @= 0
    rounds @= 0
    while rounds < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            i @ i + 1
        total @ total + items.len()
        rounds @ rounds + 1
    print(f"built {total} items")
    _ = kill(pid: getpid(), sig: SIGUSR2)
    received = hero_thread_join(handle)
    print(f"received {received} signals")
```

`heroes build main.hero`: I expect the compiler to accept it (see the doubts below). It prints `built 20000000 items` and then `received M signals`, and exits with code 0.

## choice_points

1. **How to count: block and `sigwait` on a thread, or the language's `signal` hook.**
   - **Chose:** block the signals and count them with `sigwait` on a spawned thread.
   - **The other way:** a function given to `signal` runs on the runtime's thread. It can't reach a counter, since there are no mutable globals and only the thread's `i64` crosses. A counter kept on the C side (for example, a pipe at a fixed file descriptor) needs fragile setup. A pipe also fills at about 64K signals; then the handler blocks, or it drops counts and the answer is wrong.
   - **Risk of my choice:** the spec doesn't say whether the runtime starts threads of its own before `main`. If one exists and doesn't block SIGUSR1, a signal could reach it and kill the process (SIGUSR1's default action). That would be a stop at run time.

2. **Unix signals don't queue.** Several SIGUSR1s that arrive while one is pending merge into one. M counts the signals delivered, so it can be less than the number of `kill`s sent. No reading of the spec avoids this.

3. **`sigset_t` as `record sigset_t partial` with no fields, built with `sigset_t(rest: zero)`.**
   - **Chose:** a `partial` record with no fields, so its size stays C's. I named it `sigset_t` because glibc's type is a typedef of an untagged struct, so there's no tag to give.
   - **Risks:**
     - On macOS `sigset_t` is a `__uint32_t`, not a struct, so clang would refuse the record.
     - The grammar allows `partial` with no fields, but a construction that names no field, only `rest: zero`, may be refused.
     - The other way, writing the field `__val: u64[16]`, works only on glibc.

4. **`pthread_sigmask`'s `oldset` declared as `ptr` and passed `nullptr`.** The spec allows this because what a `ptr` points at is exempt from the check. Declaring it `@oldset: sigset_t` instead would mean passing a cell that nothing reads, which is an unused-binding compile error.

5. **The constants (`SIGUSR1`, `SIGUSR2`, `SIG_BLOCK`) declared as `i32`, not `i64`.** This lets them go straight to `int` parameters. As `i64`, every use would need `to_i32(...).must()`; without it, the width mismatch is a compile error.

6. **`pid_t` declared as `i32`.** That's right on Linux and macOS. A wider type would be refused, because a parameter must match C's width.

7. **How the counting thread stops: `kill(getpid(), SIGUSR2)` sent to the whole process.**
   - **Chose:** a signal to the whole process. Every thread blocks SIGUSR2, so only the `sigwait` thread takes it.
   - **Using `raise` instead:** the signal would go only to `main`'s own thread, where it's blocked. The counting thread would never see it, `join` would wait forever, and the program would hang.
   - **Ordering:** Linux delivers SIGUSR1 (10) before SIGUSR2 (12), so a SIGUSR1 already pending is counted before the stop.
   - **Named arguments:** `kill(pid:, sig:)` needs them because both parameters are `i32`. Leaving them out is a compile error.

8. **Declaring `hero_thread_spawn` and `hero_thread_join` in an `extern "hero_os.h"` group.** I chose this because the spec says threads come from `hero_os.h`, linked "where a group names one". If the compiler instead provides them already, my group might clash with that and be refused.

9. **Signal blocking at both ends.**
   - `main` blocks the signals before spawning, so the new thread inherits the mask. Blocking after spawning leaves a window where a signal could kill the process.
   - The thread also blocks them itself, as a guard.
   - **Remaining window:** a signal that arrives before the first line of `main` still kills the process. I can't close that from inside the program.

10. **`while` loops, not `for _ in range(...)`.**
    - The grammar wants an `ident` after `for`, and an unused loop name is a compile error. `_` there might be refused.
    - `range` would also build an extra million-element array each round.

11. **`items.len()` assumed to be `i64`.** If it were `u64`, `total @ total + items.len()` would mix types and be a compile error.

12. **Return values of the C calls are ignored with `_ =`.** If one of them failed, the count could be wrong, and a failing `sigwait` would make the thread spin. On Linux these calls don't fail with these arguments.

13. **`count_signals(_: i64)` uses `_` for the unused parameter.** Giving it a name would be an unused-parameter compile error.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it on Linux/glibc, though I'm less sure about points 3 and 8. On macOS I expect it to be refused, because of the `sigset_t` record (point 3).
- **With no signals sent:**
  ```
  built 20000000 items
  received 0 signals
  ```
  Exit code 0.
- **With N SIGUSR1s sent while it works:** the same first line, then `received M signals`, where M is at most N; it's less when signals merged while pending. It doesn't crash and exits with code 0. The exceptions are a signal that arrives before `main` blocks SIGUSR1, or a runtime thread that doesn't block it (point 1).

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, only the agent's system setup reached my context: a note that I'm a Claude agent, the working directory path, the date, and the tool definitions. No other project rule, contract, memory or document reached me.
````

### t2-r-4 (`ec3f30`, from `result`)

````markdown
I only had a Read tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The program counts SIGUSR1 with a signal handler written in C, in a header the program ships. The spec requires this in section 13: "A function given to `signal` stops the program the first time a signal calls it: no code of the program runs inside a signal handler, so write the handler in C, in a header the program ships." The Heroes side has one `extern` group with two functions: one installs the handler and one reads the count. The handler only adds to a lock-free atomic 64-bit counter. It is installed with `SA_RESTART`, so a signal arriving during a system call (for example the `write` behind `print`) restarts the call instead of making it fail with `EINTR`. The compiler should accept both files, and the program prints two lines and exits with code 0 (see `prediction`).

### `main.hero`

```
# Counts the SIGUSR1 signals that arrive during a long piece of work.
# The handler is C, in sigcount.h: no Heroes code runs inside a signal handler.
extern "sigcount.h"
    function sigcount_install() -> i64
    function sigcount_read() -> i64

constant ROUNDS: i64
    20

constant ITEMS_PER_ROUND: i64
    1000000

# Builds `item 1` .. `item ITEMS_PER_ROUND` into a fresh list and gives its length.
function build_round() -> i64
    items: [str] @= []
    for i in range(from: 1, to: ITEMS_PER_ROUND + 1)
        items @ items.push(f"item {i}")
    return items.len()

function main()
    installed = sigcount_install()
    assert installed == 0
    total @= 0
    for _ in range(from: 0, to: ROUNDS)
        total @ total + build_round()
    received = sigcount_read()
    print(f"built {total} items")
    print(f"received {received} signals")
```

### `sigcount.h` (beside `main.hero`)

```c
#ifndef HEROES_SIGCOUNT_H
#define HEROES_SIGCOUNT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <signal.h>
#include <stdint.h>
#include <string.h>

/* Written only by the handler, read only by sigcount_read; lock-free atomics
   are async-signal-safe. */
static int64_t sigcount_received = 0;

static void sigcount_on_usr1(int signo) {
    (void)signo;
    __atomic_fetch_add(&sigcount_received, 1, __ATOMIC_RELAXED);
}

/* Installs the SIGUSR1 handler: 0 on success, -1 on failure. */
static int64_t sigcount_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sigcount_on_usr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    return sigaction(SIGUSR1, &sa, NULL) == 0 ? 0 : -1;
}

/* The number of SIGUSR1 signals handled so far. */
static int64_t sigcount_read(void) {
    return __atomic_load_n(&sigcount_received, __ATOMIC_SEQ_CST);
}

#endif
```

No `heroes.toml` is needed, because the program uses no package.

## choice_points

1. **Where the handler lives.**
   - **Choice:** a C handler in a shipped header, as section 13 says.
   - **Other option:** pass a Heroes function to `signal`/`sigaction` through `extern "signal.h"`.
   - **What that gives:** it compiles, but the first signal stops the program. That is a run-time stop, the exact failure the task forbids.

2. **What the C functions look like.**
   - **Choice:** they are `static` and defined in the header itself. The spec says only "a header the program ships", and the `extern` has no `link`, so the bodies must reach the build through the header.
   - **Other option:** declare them in the header and define them in a separate `.c` file.
   - **What that gives:** the link fails with undefined symbols, because the spec gives no way to compile a loose `.c` file. That is a build refusal.

3. **Where the header is found.**
   - **Choice:** `extern "sigcount.h"`, with the header beside `main.hero`. I assumed the quoted header is searched from the source directory, as C's `#include "..."` is. The spec does not say this.
   - **If that assumption is wrong:** the build is refused with "header not found".

4. **C widths.**
   - **Choice:** I declared the C results as `int64_t`, so that `-> i64` matches exactly.
   - **Other option:** return `int` and declare `-> i32`, or `-> i64` relying on "a result may be wider than C's".
   - **What that gives:** both should also be accepted. A narrower or mismatched parameter, by contrast, would be refused.

5. **Feature macro.**
   - **Choice:** `_GNU_SOURCE` is defined, guarded, at the top of the header. Without it, glibc in strict C mode can hide `struct sigaction` and `SA_RESTART`, and the build is refused.
   - **Other option:** defining `_POSIX_C_SOURCE` instead could hide non-POSIX names the runtime needs. That is also a possible build refusal.
   - **Platforms:** on macOS the macro is harmless.

6. **`SA_RESTART`.**
   - **Choice:** I set it.
   - **What leaving it out gives:** a signal during `print`'s `write` could make it fail with `EINTR`. Depending on the runtime, that could abort or lose output, so the program could stop with an error or print something different.

7. **The counter type.**
   - **Choice:** a 64-bit counter changed with `__atomic` builtins.
   - **Other option:** `volatile sig_atomic_t`, which is an `int`.
   - **What that gives:** it works for any realistic number of signals. In theory it could wrap past 2^31−1 and print a wrong (negative) count.

8. **When the count is taken.**
   - **Choice:** I read it once, right after the loop and before printing. That fixes M to the signals received "while it works".
   - **Other option:** read it inside the second `print`.
   - **What that gives:** signals that arrive between the two prints would also be counted. The answer would differ slightly from what the task means.

9. **Ignoring the loop variable.**
   - **Choice:** `for _ in range(...)`. The `For` grammar names an `ident`, and section 5 says `_` "names anything you do not use".
   - **Other option:** a named loop variable that nothing reads.
   - **What that gives:** a compile error, because an unused binding is refused. If the compiler did not accept `_` there, my version would be refused instead.

10. **Exit code and failure handling.**
    - **Choice:** I never call `exit`, because `main` returning normally gives exit code 0. I check that the handler was installed with `assert installed == 0`.
    - **Why not `exit`:** section 11 says a code "written as a number is refused", so `exit(code: 1)` is a compile error.
    - **Other option:** leave the install result unchecked.
    - **What that gives:** it is refused at compile time, because "a line that computes a value must use it", and `_ =` on an `i64` would only hide a failure.

11. **Named arguments for `range`.**
    - **Choice:** `range(from: 1, to: ITEMS_PER_ROUND + 1)`. The two parameters share the type `i64`, so names are mandatory.
    - **What leaving them out gives:** the compiler refuses it.
    - **Off-by-one:** `to` is excluded, so `to: ITEMS_PER_ROUND` would build only 999,999 items and print `built 19999980 items`.

12. **Growing the list.**
    - **Choice:** `items @ items.push(...)` on a plain local name, which grows in place.
    - **Other option:** hold the list in a record field.
    - **What that gives:** the whole array would be copied on every push. The work becomes quadratic and in practice never finishes.

## prediction

- **Compiler:** I expect it to accept the program with `heroes build main.hero`, given choice points 3 and 9.
- **Output, with no signals sent:**
  ```
  built 20000000 items
  received 0 signals
  ```
- **Output, with signals sent:** the first line is the same and the second is `received M signals`. Exit code is 0 either way.
- **What M counts:** M is the number of SIGUSR1 deliveries the handler ran. While one SIGUSR1 is already pending, more of them merge into it, so M can be less than the number of `kill -USR1` calls under a flood. That is a property of POSIX signals, not of the program.
- **Crashes:** however many signals come, and however fast, the program does not crash.
- **The one remaining window:** a SIGUSR1 sent before `main` has installed the handler, during runtime start-up, still gets the default action and kills the process. No program can close that window.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, the only other input was the agent setup itself: the working-directory path and date in the system prompt, and the tool definitions. No other instruction, rule, memory or document about Heroes reached my context. My knowledge of C/POSIX signals is general background, not from any file.
````

### t2-x-4 (`d9a2db`, from `result`)

````markdown
I couldn't write `report.md` because I only have a file-reading tool, so the whole report is below.

# report.md

## answer

**The approach.** The specification has no signal handling, and nothing in the language can count signals inside a C signal handler, for three reasons:
- There are no mutable globals.
- There are no closures.
- A Heroes callback would run inside the handler, where only async-signal-safe code is allowed.

So the program uses the standard POSIX way to receive signals on a thread of its own, through the FFI (section 13):

1. Before doing anything else, `main` blocks SIGUSR1 and SIGUSR2 with `pthread_sigmask`. A blocked signal is never delivered as a default action, so SIGUSR1 can no longer kill the process. Threads started afterwards inherit the mask.
2. `main` starts a counting thread with `hero_thread_spawn`. That thread loops on `sigwait` and counts each SIGUSR1.
3. `main` does the work. After the twenty rounds it prints the first line, then sends itself SIGUSR2 with `kill(getpid(), SIGUSR2)` to tell the counter to stop.
4. `main` collects the count, which is the thread's `i64` result, with `hero_thread_join`, and prints the second line.

`sigset_t` is laid out differently on Linux (a 128-byte struct) and macOS (a `u32`). To work on both, the set lives in a `malloc`'d 256-byte block passed to C as `ptr`. The specification exempts "what a `ptr` points at" from the width check.

### `main.hero`

```
# Signals: block SIGUSR1/SIGUSR2 process-wide and receive them synchronously.
extern "signal.h"
    constant SIGUSR1: i32
    constant SIGUSR2: i32
    constant SIG_BLOCK: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function pthread_sigmask(how: i32, set: ptr, oldset: ptr) -> i32
    function sigwait(set: ptr, @sig: i32) -> i32
    function kill(pid: i32, sig: i32) -> i32

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

extern "unistd.h"
    function getpid() -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# A fresh signal set holding SIGUSR1 and SIGUSR2; 256 bytes covers every
# platform's `sigset_t`. The caller frees it.
function signal_set() -> ptr
    set = malloc(size: 256)
    assert set != nullptr
    e = sigemptyset(set)
    assert e == 0
    a = sigaddset(set, SIGUSR1)
    assert a == 0
    b = sigaddset(set, SIGUSR2)
    assert b == 0
    return set

# The counting thread: takes SIGUSR1 signals until a SIGUSR2 says stop, and
# answers how many it took.
function count_signals(_: i64) -> i64
    set = signal_set()
    received @= 0
    while true
        sig: i32 @= 0
        r = sigwait(set, @sig)
        assert r == 0
        if sig != SIGUSR1
            break
        received @ received + 1
    free(set)
    return received

function main()
    blocked = signal_set()
    m = pthread_sigmask(how: SIG_BLOCK, set: blocked, oldset: nullptr)
    assert m == 0
    free(blocked)
    counter = hero_thread_spawn(body: count_signals, arg: 0)

    built @= 0
    round @= 0
    while round < 20
        items: [str] @= []
        i @= 1
        while i <= 1000000
            items @ items.push(f"item {i}")
            built @ built + 1
            i @ i + 1
        assert items.len() == 1000000
        round @ round + 1
    print(f"built {built} items")

    k = kill(pid: getpid(), sig: SIGUSR2)
    assert k == 0
    received = hero_thread_join(handle: counter)
    print(f"received {received} signals")
```

There are no other files. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **How to catch signals.** I chose to block the signals and read them with `sigwait` on a thread.
   - **Alternative: install a handler.** `signal(sig, handler)` with a Heroes callback runs into three problems. `signal`'s result is a function pointer, and "a callback is a parameter, never a result", so the compiler would likely refuse the declaration or need a guessed `ptr` result. The handler has nowhere to keep a count, because there are no mutable globals or closures. Running Heroes code inside a handler is not async-signal-safe, so it could crash or deadlock at run time under a flood of signals.
   - **Alternative: install nothing.** The program would stop on the first SIGUSR1, killed by the default action.

2. **How to represent `sigset_t`.** I chose a `malloc`'d `ptr` block.
   - **`record sigset_t` with `__val: u64[16]`.** This matches glibc only. On macOS, where `sigset_t` is `u32`, the compiler refuses it.
   - **`@set: u32`.** The reverse: it fits macOS only, and Linux refuses it. It would also hand C only one element.

3. **Blocking before spawning the thread.** If the thread were spawned first, it would not inherit the mask. A SIGUSR1 could then reach a thread that does not block it, and the default action would kill the process at run time.

4. **How the counter learns to stop.** I chose to send SIGUSR2 to the process.
   - **A shared flag** is impossible: only one `i64` crosses into a thread, and there are no globals.
   - **`pthread_kill`** would need the thread's `pthread_t`, which `hero_thread_spawn` does not give.

5. **The thread body's unused parameter.** I named it `_`. A named parameter that is never used is a compile error.

6. **Loop counters.** I used `while` with counters rather than `for _ in range(...)`. The grammar's `for` takes an `ident`, and the specification doesn't say whether `_` is allowed there. A named loop variable that is never used would be refused.

7. **Counting items.** I used `built @ built + 1` plus `assert items.len() == 1000000`, not `built + items.len()`. The specification never gives `len`'s result type. If it were `u64`, adding it to an `i64` cell would be refused. The literal comparison works with either type.

8. **Checking C results.** Each result is bound to a name and then asserted, rather than written as `assert sigwait(...) == 0`. If a build dropped asserts, the C calls would vanish with them, and the program would block forever or count nothing. The specification doesn't say whether asserts are ever dropped.

9. **Constant widths.** I declared the constants as `i32`, C's `int`. Declaring them `i64` is allowed for a result, but every call site would then need `.to_i32().must()`. Without that, the compiler refuses the call because the widths differ.

10. **Named arguments.** `kill`, `pthread_sigmask` and `hero_thread_spawn` are called with named arguments. For `kill` and `pthread_sigmask` this is required, because two parameters share a type. Positional arguments there would be refused.

11. **Ending the counter loop.** It uses `while true` with `break`. Because it has its own `break`, it doesn't end the path, so the `free` and `return` after it are reachable.

## prediction

I expect the compiler to accept the program with `heroes build main.hero`. The main uncertainty is whether clang's header check accepts `ptr` for the `sigset_t *` parameters. The specification says what a `ptr` points at is exempt, so I expect yes.

When run, it prints:

```
built 20000000 items
received M signals
```

It exits with code 0. M is the number of SIGUSR1 signals delivered while it ran, and 0 if none were sent.

**Limits on the count:**
- **Merged signals:** standard signals don't queue. Several SIGUSR1 sent while one is already pending are merged by the kernel, so M can be less than the number sent.
- **Late signals:** one that arrives at the same moment as the SIGUSR2 at the very end may be left uncounted.
- **Very first instant:** a SIGUSR1 that arrives before `main` reaches `pthread_sigmask` still triggers the default action. No program in this language can close that window.

## context

No instruction, rule, contract, memory or document other than `brief.md`, `task.md` and `heroes-spec.md` in this directory reached my context. The only other inputs were the system prompt, which gave the working directory and date, and the user's request.
````

### t1-s-4 (`3fb6c6`, from `result`)

````markdown
I only have a file-reading tool here, so I couldn't create `report.md`. As the brief allows, the whole report follows.

# report.md

## answer

The task asks for a program, `main.hero`, that loops through steps. Each step builds `step N`, adds it to a list, sleeps one second through C's `sleep`, and prints the text. On SIGINT or SIGTERM it finishes the step in hand, prints `stopped after N steps` and exits with code 0. I also have to say whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 s after it starts.

The program needs a way for the signal handler to tell the main loop to stop. Heroes has no mutable globals and no closures, so that state has to live in C. The handler sets an environment variable with `setenv`. The loop checks it with `getenv` before each step. This is safe because the specification says a function given to `signal` "runs after the signal, never inside it … once that thread is back in the program's own code." A program that uses no package has no catalogue, so there is no `heroes.toml`.

### `main.hero`

```
# Prints `step N` once a second until SIGINT or SIGTERM, then reports how many steps it printed.

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "stdlib.h"
    function setenv(name: cstr lent, value: cstr lent, overwrite: i32) -> i32
    function unsetenv(name: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

# The environment variable through which the signal handler asks the loop to stop.
constant STOP_VAR: str
    "HEROES_STOP_REQUESTED"

# Runs after SIGINT or SIGTERM, outside the signal itself: records that a stop was asked for.
function on_stop(_: i32)
    _ = setenv(name: STOP_VAR.cstr(), value: "1".cstr(), overwrite: 1)

function main()
    _ = unsetenv(STOP_VAR.cstr())
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    texts: [str] @= []
    n @= 0
    while getenv(STOP_VAR.cstr()) == nullptr
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
    print(f"stopped after {texts.len()} steps")
```

## choice_points

1. **How the handler tells the loop to stop.** The specification gives no mutable globals and no closures, so the handler can't set a flag inside the program. I put the flag in the C environment with `setenv` and `getenv`.
   - **Other option: call `exit` in the handler.** The handler runs at the next point the thread is back in program code. That is usually right after the interrupted `sleep` returns and before `print(text)`. So the program would end mid-step without printing `step 3` or the summary. It would run and give a different answer than intended.
   - **Other option: block the signals and poll `sigpending`.** This needs `sigset_t` as a group record. It's a struct on Linux but an integer typedef on macOS, so the compiler probably refuses it on at least one platform.
   - **Other option: a flag file written with `write_file`.** This would work but leaves a file on disk.

2. **Clearing the variable at start.** I call `unsetenv` first. Without it, a `HEROES_STOP_REQUESTED` already set in the environment would make the program print only `stopped after 0 steps`. That runs, but gives a different answer than intended.

3. **The result type of `signal`.** C returns `void (*)(int)`, and the specification says a callback is "a parameter, never a result". I declared the result `ptr`. The specification says clang checks result types and that "a result may be wider than C's", but it doesn't say a function pointer may be read as `ptr`. If it may not, the compiler refuses the program, and the specification offers no other type to write. This is the main risk to compilation.

4. **Type of the signal constants.** I declared `SIGINT` and `SIGTERM` as `i32`, C's `int`, so they pass straight to `sig: i32`. If declared `i64`, the call `signal(SIGINT, …)` is a width mismatch with no implicit conversion, so the compiler refuses it unless I add `.to_i32().must()`.

5. **The handler's parameter.** It is `_: i32`. A named parameter that goes unused is a compile error. The type has to be `i32`, C's `int`; another width is refused.

6. **Named arguments to `setenv`.** Two of its parameters share the type `cstr`, so names are mandatory and I give them. Positional arguments would be refused. Calls with no shared parameter type use positional arguments.

7. **Using the list.** I print the count as `texts.len()` so that `texts` is read. The specification says a write is not a use, and it's unclear whether `texts @ texts.push(text)` counts as reading `texts`. If I never read it otherwise, it might be an unused-binding error. `texts.len()` always equals `n` at that point, because each step pushes before it sleeps and prints. I don't know whether `len` returns `i64` or `u64`, but either prints the same inside the interpolation.

8. **Discarded C results.** The results of `sleep`, `signal`, `setenv` and `unsetenv` are dropped with `_ =`. A bare call line that computes a value is refused.

9. **How the program exits.** `main` simply returns, which gives exit code 0. `exit(code: 0)` would be refused, because the specification refuses an exit code written as a number.

10. **An interrupted `sleep`.** I don't sleep again for the rest of the second, because the task says only that the step must be finished, and printing its text finishes it. Re-sleeping would only delay the final output by up to about 0.5 s, with the same text.

11. **Every `getenv` and `setenv` parameter is `lent`.** The `.cstr()` lend reaches only a parameter declared `lent`. Without the mark, the compiler refuses the lend.

## prediction

**Build:** I expect `heroes build main.hero` to accept the program. The only real risk is the `-> ptr` result of `signal` (choice point 3). If clang's check rejects a function pointer read as `ptr`, the build is refused.

**Run with `kill -INT` at about 2.5 s:**
- **t = 1 s:** step 1 finishes and prints `step 1`.
- **t = 2 s:** step 2 finishes and prints `step 2`.
- **t = 2.5 s:** step 3 is sleeping when SIGINT arrives.
  - `sleep` returns early, and the handler then runs and sets the variable.
  - Step 3 finishes and prints `step 3`.
  - The loop check finds the variable set, so the loop ends.

If `sleep` were not interrupted, the same lines would appear at t = 3 s. The output is:

```
step 1
step 2
step 3
stopped after 3 steps
```

The exit code is 0. `kill -TERM` gives the same result, and so does Ctrl-C.

## context

No instruction, rule, contract, memory or document other than the files in this directory reached my context. I read only `brief.md`, `task.md` and `heroes-spec.md`, plus the system instructions that frame this session: the working directory, the date and the tool description.
````

### t1-m-4 (`51d552`, from `result`)

````markdown
I can only read files here, so I could not write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**What the task asks.** The program must run steps until it is told to stop. If it gets `kill -INT` or `kill -TERM` (or Ctrl-C) partway through a step, it finishes that step, prints `stopped after N steps` and exits with code 0. The task also asks whether `heroes build main.hero` accepts the program, and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**How the program handles the signal.** The specification does not allow a Heroes signal handler: §13 says "A callback is a parameter, never a result or a signal handler, nor called by one". So the program never catches the signal. Instead:

1. At startup, before the first step, it blocks SIGINT and SIGTERM with `sigprocmask`.
2. A blocked signal stays pending. It does not kill the process and does not cut `sleep` short, so the step in hand always finishes.
3. After each step, the program calls `sigpending` to check whether either signal has arrived. If one has, it stops looping, prints the summary and returns from `main` with exit code 0. The pending signal is never unblocked, so it is never delivered.

It targets macOS, where `sigset_t` is a `__uint32_t` bitmask and signal `s` is bit `s - 1`. (The working directory is under `/Users`.)

### `main.hero`
```
# C's signal mask: SIGINT and SIGTERM are blocked, so they wait as pending
# signals instead of ending the program, and each step asks whether one came.
extern "signal.h"
    constant SIGINT: i64
    constant SIGTERM: i64
    constant SIG_BLOCK: i32
    function sigprocmask(how: i32, @set: u32 lent, oset: ptr lent) -> i32
    function sigpending(@set: u32 lent) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Whether any signal in `wanted` (a sigset_t bitmask) is pending.
function stop_requested(wanted: i64) -> bool
    pending: u32 @= 0
    assert sigpending(@pending) == 0
    return (to_i64(pending).must() & wanted) != 0

function main()
    stop_bits = (1 << (SIGINT - 1)) | (1 << (SIGTERM - 1))
    mask: u32 @= to_u32(stop_bits).must()
    assert sigprocmask(SIG_BLOCK, @mask, nullptr) == 0
    done: [str] @= []
    n @= 0
    stopping @= false
    while !stopping
        n @ n + 1
        text = f"step {n}"
        done @ done.push(text)
        _ = sleep(1)
        print(text)
        stopping @ stop_requested(stop_bits)
    print(f"stopped after {done.len()} steps")
```

No other file is needed. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **How to react to the signal.** I blocked the signals and polled for them, because §13 forbids a Heroes callback as a signal handler.
   - Registering a Heroes function with `signal`/`sigaction`: the compiler refuses it.
   - Leaving the default behaviour: the program runs, but `kill -INT` kills it mid-step with no summary and a non-zero status. That is a different answer from the one intended.
   - Ignoring the signal with `SIG_IGN`: the program never stops.
   - A thread running `sigwait`: it could not tell the main loop, because only an `i64` crosses at `join`.

2. **The `sigset_t` representation.** I declared it as a `u32` out-parameter, which is right for macOS.
   - On Linux/glibc, `sigset_t` is a struct. There it would need `record sigset_t` with `__val: u64[16]`, and this file would be refused by clang's check of the header.
   - I also chose not to call `sigemptyset`/`sigaddset`. On macOS these are also macros, and that risks the header check. I build the bitmask myself as `1 << (sig - 1)`, which is correct only for the Darwin layout.

3. **How the `sigset_t *` parameters are declared.**
   - `const sigset_t *set` is declared `@set: u32 lent`, because only an `@` parameter passes a pointer to one number.
   - The old-mask parameter is declared `oset: ptr lent` and given `nullptr`. The other option was a second `@` `u32` cell. That cell would never be read, which may be refused as unused, and it would make two parameters share the type `u32`, which forces named arguments.

4. **Constant widths.** `SIGINT`/`SIGTERM` are declared `i64` so the bit arithmetic can use them directly, which relies on "may be wider" applying to constants. `SIG_BLOCK` is `i32`, because it is passed to an `int` parameter.
   - If a wider constant is not accepted, the compiler refuses `SIGINT: i64`. The fix would be `i32` plus `to_i64(...)`.

5. **Checking the C results.** I used `assert ... == 0`. Discarding them with `_ =` would also compile, but a failure would then go unnoticed.

6. **The source of N in the final message.** I used `done.len()`, the length of the list. It always equals the number of steps printed, because the check happens only after a step has printed. Using `n` would print the same thing.

7. **The loop form.** I used `while !stopping` rather than `while true` with `break`. Both compile, and they print the same output.

8. **How the program ends.** It ends by returning from `main`, which gives exit code 0. I did not use `exit(code: 0)`, because §11 says "one written as a number is refused", so that call might be refused.

9. **Where the check goes.** The check comes after printing. If it came before building the text, a signal that arrived during step k would still make step k print first, but the count logic would change. As placed, every step that starts also finishes.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program on macOS. On Linux, clang's header check should refuse it because of the `sigset_t` mismatch.
- **Run, with `kill -INT` at about 2.5 s:**
  - Step 1 prints at about 1 s and step 2 at about 2 s.
  - The signal arrives during step 3's `sleep`. It stays pending and does not interrupt the sleep.
  - Step 3 prints at about 3 s. The check then sees the pending signal.

  Output:
```
step 1
step 2
step 3
stopped after 3 steps
```
  The exit code is 0. `kill -TERM` and Ctrl-C behave the same way (the terminal may echo `^C`).

## context

Nothing reached my context besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, apart from the standard agent system prompt (the working directory and date). No other instruction, rule, contract, memory or document was present. I used general knowledge of POSIX and macOS's `signal.h`.
````

### t1-e-4 (`027658`, from `result`)

````markdown
I couldn't create `report.md`: the only tool I have here reads files and can't write them. As the brief allows, the whole report is below.

---

# report.md

## answer

**Task:** write `main.hero`. It runs `step N` steps until it is stopped. Each step adds the text to a list, calls C's `sleep(1)` and prints the text. On SIGINT or SIGTERM it finishes the step in hand, prints `stopped after N steps` and exits with code 0. The report also says whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 s after it starts.

**Approach:** the program blocks SIGINT and SIGTERM with `sigprocmask` before the loop, so neither signal can end the process or cut a step short. A signal that arrives while blocked stays pending. After each step has printed its text, the program asks `sigpending` and `sigismember` whether SIGINT or SIGTERM is waiting. If one is, it leaves the loop, prints the count and returns from `main`. That avoids the `signal` callback, whose thread can't share a flag with `main` (see choice point 1).

### `main.hero`

```
# C's signal-mask calls: SIGINT and SIGTERM are blocked, so they wait as
# pending signals until the loop asks for them between steps.
extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signum: i32) -> i32
    function sigprocmask(how: i32, set: sigset_t lent, @oldset: sigset_t) -> i32
    function sigpending(@set: sigset_t) -> i32
    function sigismember(set: sigset_t lent, signum: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Whether SIGINT or SIGTERM arrived and waits, blocked, to be delivered.
function stop_requested() -> bool
    pending @= sigset_t(rest: zero)
    _ = sigpending(@pending)
    return sigismember(pending, SIGINT) == 1 || sigismember(pending, SIGTERM) == 1

function main()
    mask @= sigset_t(rest: zero)
    _ = sigemptyset(@mask)
    _ = sigaddset(@mask, SIGINT)
    _ = sigaddset(@mask, SIGTERM)
    old @= sigset_t(rest: zero)
    _ = sigprocmask(how: SIG_BLOCK, set: mask, oldset: @old)
    texts: [str] @= []
    n @= 0
    stopping @= false
    while !stopping
        n @ n + 1
        text = f"step {n}"
        texts @ texts.push(text)
        _ = sleep(1)
        print(text)
        stopping @ stop_requested()
    print(f"stopped after {texts.len()} steps")
```

No other files are needed. The program uses no package, so there is no `heroes.toml`, and the C calls are all in libc, so no `link` is needed.

## choice_points

1. **How to catch the signal.** Section 13 lets a function be given to `signal`: it runs after the signal, on a thread of the runtime's own. I chose not to use it. That handler is a top-level `(function(i32) -> ())`. There are no mutable globals, and between threads only an `i64` argument and result cross, so the handler can't tell `main`'s loop to stop. The only ways out would be to write a file or a pipe whose descriptor the handler can't be given.
   - **Other choice:** a handler that does nothing or prints. The program would run forever and never print `stopped after N steps`, which is the wrong answer.
   - **Also:** declaring `signal`'s result (`sighandler_t`, a function pointer) is a problem in itself. A callback is "never a result", so the result would have to be `ptr` or left out. The compiler might refuse either.

2. **Blocking the signals instead of handling them.** While SIGINT and SIGTERM are blocked they don't interrupt `sleep`, so each step always waits its full second.
   - **Other choice:** leave them unblocked with no handler. The default action kills the process mid-step with a non-zero status (130 or 143 from a shell), and the final line never prints.
   - **Risk:** if the runtime runs other threads at startup that leave these signals unblocked, the kernel could deliver the signal to one of them and the default action would kill the process. The spec mentions a runtime thread only in connection with `signal`, so I assumed there is none here.

3. **Declaring `sigset_t`.** I wrote `record sigset_t partial`: no tag (it's a typedef, not `struct X`), no fields, and C's size. I build it with `sigset_t(rest: zero)`.
   - **Other choice:** `record SigSet tag sigset_t` with no fields would be a handle, meaning `sigset_t *`. The compiler would refuse it, or C would get the wrong type.
   - **Platform:** this assumes Linux/glibc, where `sigset_t` is a struct. On macOS `sigset_t` is `__uint32_t`, so the record declaration would be refused.

4. **Width of the constants.** I declared them `i32`, to match `int` and pass straight to the `i32` parameters.
   - **Other choice:** `i64` might still be accepted as a constant, but every call would then need `to_i32(...).must()`. Passing an `i64` directly to an `int` parameter is refused.

5. **Named arguments to `sigprocmask`.** `set` and `oldset` are both `sigset_t`, so I treated naming as mandatory and named all three arguments.
   - **Other choice:** a positional call would be refused if sharing a type counts across the `lent` / `@` marks.

6. **Pointer kinds.** `const sigset_t *` became `lent` without `@`, which the spec calls a pointer to const. `sigset_t *` that C writes became an `@` out-parameter.
   - **Other choice:** a plain (copy) parameter where C expects a pointer would be refused by clang's check against the header.

7. **Discarding C's `int` results** with `_ = ...`. They aren't `T?`, so `_` may drop them.
   - **Other choice:** a bare call line would be refused ("a line that computes a value must use it").

8. **Exit code 0.** `main` just returns, which I take to mean status 0.
   - **Other choice:** the spec says a code "written as a number is refused" by `exit`, so `exit(code: 0)` would probably not compile.

9. **When to check for a stop.** I check after the print, so the step in hand always completes and is counted.
   - **Other choice:** checking before the sleep would also finish the step, but checking at the top of the loop is the same thing. Checking between the sleep and the print would end a step without printing it, giving the wrong count.

10. **The count.** I print `texts.len()`. It equals both `n` and the number of steps printed, and it uses the list the task asks for. The cell `texts` is read by `push`, so it isn't unused.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept it, on Linux/glibc. On macOS I expect it to refuse the `sigset_t` record (choice point 3).
- **Output for `kill -INT` at about 2.5 s:** `step 1` prints at about 1 s and `step 2` at about 2 s. The signal arrives during step 3's sleep and stays pending. The sleep finishes, `step 3` prints at about 3 s, the pending SIGINT is found, and the program prints:

```
step 1
step 2
step 3
stopped after 3 steps
```

- **Exit status:** 0.
- **Other cases:** Ctrl-C and `kill -TERM` behave the same way.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document about Heroes did. I also tried to read `report.md` to see whether it already existed; it didn't. My harness's system prompt (identity, working directory, date, tool rules) was present, but it says nothing about Heroes.
````

### t1-r-4 (`86b0cd`, from `result`)

````markdown
I couldn't write `report.md` because I only have a file-reading tool here. As the brief allows, the whole report is below.

# report.md

## answer

**The task:** write `main.hero`, a loop that keeps running. Each step builds `step N`, adds it to a list, calls C's `sleep(1)` and prints the text. On SIGINT or SIGTERM the program finishes the step in hand, prints `stopped after N steps` and exits with code 0. The report must also say whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

The spec (section 13) says a Heroes function given to `signal` stops the program the first time a signal calls it. No program code runs inside a handler, so the handler must be written in C, in a header the program ships. That header is `stop.h`. Its handler only sets a `volatile sig_atomic_t` flag. The Heroes loop reads that flag between steps, so a step is never cut off in the middle.

### `stop.h`
```c
#ifndef STOP_H
#define STOP_H

#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include <signal.h>
#include <string.h>

/* Set by the handler, read by the program between steps. */
static volatile sig_atomic_t stop_flag = 0;

static void stop_on_signal(int sig)
{
    (void)sig;
    stop_flag = 1;
}

/* Installs the handler for SIGINT and SIGTERM. Returns 0 on success, -1 on failure. */
static int stop_install(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = stop_on_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) != 0) {
        return -1;
    }
    if (sigaction(SIGTERM, &sa, NULL) != 0) {
        return -1;
    }
    return 0;
}

/* 1 once a stop was asked for, else 0. */
static int stop_requested(void)
{
    return stop_flag != 0;
}

#endif
```

### `main.hero`
```
extern "stop.h"
    function stop_install() -> i32
    function stop_requested() -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Exit code when the signal handlers cannot be installed.
constant SETUP_FAILED: i64
    1

function main()
    if stop_install() != 0
        print("cannot install signal handlers")
        exit(code: SETUP_FAILED)
    done: [str] @= []
    while stop_requested() == 0
        text = f"step {done.len() + 1}"
        done @ done.push(text)
        _ = sleep(seconds: 1)
        print(text)
    print(f"stopped after {done.len()} steps")
```

There is no `heroes.toml`, because the program uses no package (section 1).

## choice_points

1. **Where the signal handler lives.** I put it in C (`stop.h`), as section 13 says to.
   - **Other choice:** pass a Heroes function to `signal`/`sigaction` through an extern. The compiler would accept it, but the first Ctrl-C would stop the program at once, mid-step, with no `stopped after` line. That's the wrong answer at run time.

2. **How `stop.h` is found.** I assumed `extern "stop.h"` is found beside `main.hero`, the way C finds `#include "stop.h"`. The spec says "a header the program ships" but doesn't say where it's searched for.
   - **If the include path doesn't cover that directory:** the compiler refuses the program because the header isn't found.

3. **Header written with `static` functions, not an external `.c` file plus `link`.** This keeps everything in the one header the spec mentions.
   - **Other choice:** declare the functions in the header and define them in a `.c` file. The spec gives no way to compile and link a shipped `.c` file, so the build would fail at link time.

4. **`sigaction` without `SA_RESTART`, not `signal()`.** This gives the same behaviour on every platform.
   - **Other choice:** `signal()` on some systems resets the handler after the first signal (System V behaviour). A second Ctrl-C would then kill the program mid-step. That only changes the result with repeated signals.

5. **`_POSIX_C_SOURCE` defined only if it isn't already set, and the `stop.h` group written first.** The spec says the groups' headers are read in the order the groups are written. If the compiler builds in strict ISO mode and the runtime included system headers before mine, the macro may come too late and `struct sigaction` would be missing. The compiler would then refuse the program.

6. **The C declarations.**
   - `sleep(seconds: u32) -> u32`: the parameter must be `u32` because C says `unsigned int`. Writing `i64` or `i32` would be refused (section 13).
   - The results are declared as `i32`/`u32`. `i64` would also be accepted, since a result may be wider.
   - I discard the result with `_ = sleep(...)`. Leaving the result unused without `_ =` would be refused (section 5).

7. **Waking early from `sleep`.** A signal cuts `sleep` short. I call it once and ignore the remaining time, so the step ends early, then prints and stops.
   - **Other choice:** loop on the remaining seconds. glibc rounds those up, so step 3 would print at about 3.5 s instead of about 2.5 s. The printed lines are the same either way; only the timing differs.

8. **Exit code 0 comes from `main` returning normally**, not from `exit(code: 0)`. The spec says `exit` refuses a code written as a number. That's also why the failure exit uses the constant `SETUP_FAILED`; writing `exit(code: 1)` would be refused.

9. **Counting with `done.len()`, not a separate counter.** This reads the list, so the `@=` cell `done` is clearly used (section 5 says a write alone is not a use).
   - **Other choice:** a separate `n @=` counter while `done` is only pushed to. The compiler might refuse that as an unused binding.
   - I also assumed `len()` gives a number that `+ 1` and an f-string hole accept.

10. **Where the stop flag is checked: at the top of the loop.** A signal at any point in a step lets that step finish (build, push, sleep, print) before the program ends.
    - **Other choice:** check right after `sleep`. Then step 3 wouldn't print, and the output would end with `stopped after 2 steps`. That's the wrong answer at run time.

11. **Failure branch.** I check `stop_install()` and exit with code 1 if it fails. Ignoring its result with `_ =` would also compile.

## prediction

I expect `heroes build main.hero` to accept both files, as long as `stop.h` is found beside `main.hero` (choice point 2).

The program installs the handlers and starts step 1 at time 0:
- At about 1.0 s it prints `step 1`.
- At about 2.0 s it prints `step 2` and starts step 3.
- At about 2.5 s `kill -INT` arrives. The handler sets the flag and `sleep` returns early. Step 3 prints its text, the loop test sees the flag, and the program prints the final line.

Output:
```
step 1
step 2
step 3
stopped after 3 steps
```
The exit code is 0. `kill -TERM` and Ctrl-C give the same result.

## context

The only inputs that reached my context were `brief.md`, `task.md` and `heroes-spec.md` from this directory. No other file, memory, rule or contract did, apart from the system instructions for this agent session (tool use, working directory, date). I used only general knowledge of C and POSIX: `sigaction`, `sleep` and glibc's behaviour.
````

### t1-x-4 (`216c39`, from `result`)

````markdown
I only had a file-reading tool here, so I couldn't create `report.md`. The whole report is below, as the brief says to do in that case.

# report.md

## answer

**What the task asks:** a loop that never ends by itself. Each step builds `step N`, adds it to a list, calls C's `sleep(1)` and prints the text. A Ctrl-C, `kill -INT` or `kill -TERM` must not cut a step short. The program finishes the current step, prints `stopped after N steps` and exits with code 0.

**How it does that:** Heroes has no mutable globals and no anonymous functions, and the only thing C can pass back is a function value. So a C signal handler has no clean way to set a flag that `main` can read. Instead, the program blocks SIGINT and SIGTERM with `sigprocmask` before the loop. A blocked signal can't interrupt anything, including `sleep`; it just waits as "pending". At the end of each step the program checks with `sigpending` and `sigismember`. If either signal is pending, it leaves the loop, prints the summary and returns from `main`.

C's `sigset_t` differs by platform: a 4-byte integer on macOS, a 128-byte struct on Linux. So it is never declared as a record. It lives in a 128-byte `malloc` block passed around as a `ptr`. The spec allows this: a parameter's declared type must match the header, "except … what a `ptr` points at".

The only file is **`main.hero`**:

```
# Prints "step N" once a second until SIGINT or SIGTERM arrives,
# then finishes the step in hand and reports how many steps it printed.

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    constant SIG_BLOCK: i32
    function sigemptyset(set: ptr) -> i32
    function sigaddset(set: ptr, signo: i32) -> i32
    function sigprocmask(how: i32, set: ptr, oset: ptr) -> i32
    function sigpending(set: ptr) -> i32
    function sigismember(set: ptr, signo: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "stdlib.h"
    function malloc(size: u64) -> ptr
    function free(p: ptr)

# Room for a sigset_t on every platform: 4 bytes on macOS, 128 on Linux.
constant SIGSET_BYTES: u64
    128

function main()
    mask = malloc(SIGSET_BYTES)
    pending = malloc(SIGSET_BYTES)
    _ = sigemptyset(mask)
    _ = sigaddset(mask, SIGINT)
    _ = sigaddset(mask, SIGTERM)
    _ = sigprocmask(how: SIG_BLOCK, set: mask, oset: nullptr)
    steps: [str] @= []
    n @= 0
    stop @= false
    while !stop
        n @ n + 1
        text = f"step {n}"
        steps @ steps.push(text)
        _ = sleep(1)
        print(text)
        _ = sigemptyset(pending)
        _ = sigpending(pending)
        stop @ (sigismember(pending, SIGINT) == 1 || sigismember(pending, SIGTERM) == 1)
    print(f"stopped after {steps.len()} steps")
    free(mask)
    free(pending)
```

**Compiler:** I expect `heroes build main.hero` to accept it. No `heroes.toml` is needed because no package is used.

**Output with `kill -INT` at about 2.5 s:**
```
step 1
step 2
step 3
stopped after 3 steps
```
The exit code is 0.

## choice_points

1. **How the signal reaches the program.**
   - **Chosen:** block both signals and poll for them with `sigpending` once per step.
   - **Other option:** install a handler with `signal(SIGINT, on_stop)`, where `on_stop` is a top-level `(function(i32) -> ())`. The handler can't set anything `main` reads, because there are no globals or closures. It could only use a trick (writing to a fixed file descriptor, `umask`, and so on). Otherwise the program compiles but either ignores the signal or dies in mid-step: a **different answer**. Leaving the default handling in place makes Ctrl-C kill the process at once, with no summary and exit status 130: also a **different answer**.
2. **How `sigset_t` is represented.**
   - **Chosen:** a `ptr` to 128 bytes from `malloc`.
   - **Other option:** declare `record sigset_t partial` and build it with `rest: zero`. On macOS `sigset_t` is not a struct, so clang's header check fails: the **compiler refuses** it. Declaring the parameter as `@set: u32` works on macOS only; on Linux the compiler refuses it, or C writes past the 4 bytes and the program **aborts**.
3. **The buffer size.**
   - **Chosen:** 128 bytes.
   - **Other option:** a smaller block such as 4 bytes works on macOS, but on Linux C writes past it. That corrupts memory and the run **may crash or misbehave**.
4. **The type of the signal constants.**
   - **Chosen:** `i32`, matching C's `int`.
   - **Other option:** `i64`, as in the spec's sqlite example, means every call needs `to_i32(...).must()`. Passing them bare is a type mismatch, so the **compiler refuses** it.
5. **How the program ends.**
   - **Chosen:** return from `main`, which I take to mean exit code 0.
   - **Other option:** `exit(code: 0)`. The spec says a code "written as a number is refused", so the **compiler refuses** it.
6. **Results of C calls.**
   - **Chosen:** each one is discarded with `_ =`.
   - **Other option:** a bare call statement. That leaves a computed value unused, so the **compiler refuses** it.
7. **Argument names for `sigprocmask`.**
   - **Chosen:** all arguments named, because `set` and `oset` share the type `ptr`.
   - **Other option:** positional arguments. Named arguments are mandatory for a same-typed pair, so the **compiler refuses** it.
8. **Where the stop check sits.**
   - **Chosen:** after the print, so the step in hand finishes first.
   - **Other option:** checking before building the text. The step then completes before the check, so this gives the same N.
   - **Third option:** checking right after `sleep`. That would skip printing that step and give a **different answer**: 2 steps printed, the summary still matching those 2.
9. **Clearing `pending` before each `sigpending`.**
   - **Chosen:** call `sigemptyset` first, which is harmless.
   - **Other option:** skip it. `sigpending` fills the whole set anyway, so the result is the same.
10. **Freeing the buffers.**
    - **Chosen:** `free` each of them.
    - **Other option:** leave them. That is the same output; a `ptr` is not a tracked handle, so nothing aborts at exit.
11. **The count in the summary.**
    - **Chosen:** `steps.len()`.
    - **Other option:** `n`. That gives the same value, and `steps` still needs some use or the **compiler refuses** it as unused.

## prediction

- **Compiler:** accepts `main.hero`. I expect the same on macOS and Linux, since every `sigset_t` passes through `ptr`.
- **Normal run:** prints `step 1`, `step 2`, … one per second, without stopping.
- **`kill -INT` at about 2.5 s:** the signal arrives during step 3's `sleep`. Because it is blocked, the sleep finishes at about 3 s. The program then prints `step 3`, finds SIGINT pending and stops. Output:
  ```
  step 1
  step 2
  step 3
  stopped after 3 steps
  ```
  The exit code is 0.
- **`kill -TERM` or Ctrl-C:** behave the same way. With Ctrl-C the terminal also echoes `^C`, which the program does not print.
- **Edge case:** a signal that arrives before `sigprocmask` runs, within microseconds of start, would still kill the process with the default action.

## context

Nothing outside this directory reached my context: no other instructions, rules, contracts, memories or documents. I read only `brief.md`, `task.md` and `heroes-spec.md`. The only other things I had were the general system prompt (an agent with a file-reading tool, the working directory and the date). The directory path `/Users/...` suggests a macOS host, and that shaped choice point 2.
````


---

# The follow-up, twenty-four sessions, graded

Run by the coordinator's agent on 2026-11-13 from 11:30:27 to 11:36:24, four at a time, under the
design registered in `docs/panel/247-briefs/llm-ergonomist.md` § Follow-up before any of its
sessions (the text before it kept as `llm-ergonomist.before-followup.md`). Arms **r** (hrr) and
**g** (hg3), each with § 13's `atexit` example corrected to `-> i32` (defect 2513); tasks t1, t2 and
t3 (two modules); four sessions per cell. **Spend of the follow-up: 4.2886 USD; the seat in all
13.8579 USD**, summed from the 64 `run.json` files under `p247-scratch/s/` (`total_cost_usd`; the
runner's `DONE 64 run.json in the seat, sum 13.857865 USD, unreadable 0`). Graded under one setup,
today with D and G (`p247-scratch/grade/dg/`: the base-with-D compiler `c8a7586f07ffccac`, `sources
560c95206c908459`, and G's runtime, its files' sorted sha256 list `9b47b3debd7b70d5`); grades in
`p247-scratch/grade-fu/<folder>/result.json`, handler kinds in `p247-scratch/handlers-followup.tsv`.
Written into this file by the coordinator from the agent's final message (the harness refused the
agent a report file of its own).

**`right` under today with D and G**, four sessions per cell:

| arm | t1 | t2 | t3 | of 12 |
|---|---|---|---|---|
| r (hrr) | 4 | 2 | 4 | **10** |
| g (hg3) | 4 | 4 | 4 | **12** |

**The registered rule** (g replaces r if g gives at least three more `right` than r over its twelve):
**+2, so r stands.**

**r's two wrong answers are a race in the driver, not the readers'**: both are the first run of a
freshly built t2 binary dying of SIGUSR1's default action (exit -30) after 65 and 167 signals; both
install their handler on `main`'s first line, so the binary had not reached `main` within the
driver's 0.3 s arm. Runs 2 and 3 of the same answers were right (982,714 and 1,022,673 received in
one, 663,152 and 793,960 in the other). No run of the forty died this way. The registered rubric
counts them `stopped`; without the race r reads 12 of 12, and g still does not replace it.

**t3 did not test the shape it was registered to separate**: all four r readers bound their C header
from `stop.hero` alone, `main` calling `stop.hero`'s functions, so one C unit includes the header and
the two-module trap never formed. The trap fails under t3's grading command when `main` binds the
header too (`p247-scratch/coord/t3trap`: SIGKILLed 10 s after SIGTERM, still printing steps); no
reader wrote it. So t3 as worded does not measure whether hrr's remedy survives two modules.

**What the readers wrote** (`handlers-followup.tsv`): **g, 12 of 12 a runtime door**,
`hero_net_catch_stop` and `hero_net_stop_asked` for t1 and t3, `hero_signal_watch` and
`hero_signal_take` for t2; none a Heroes handler and none a C handler. **r, 11 of 12 a C handler
whose flag or counter is `static` in the header**, 1 a C handler whose state is not file-scope
`static`; with the forty's eight r answers, **19 of 20 r answers are the `swp6` shape**. The
follow-up's binaries left no crash report.

**Context**: 24 of 24 answered `no`; two name a working-directory path holding `heroes-lang`
(t2-g-2, `5b0b41`; t3-r-3, `f8abbd`), none voided, as the forty's were handled.

| session | folder | cost | context |
|---|---|---|---|
| t1-r-1 | 7929de | 0.2014 | no |
| t1-g-1 | 711083 | 0.1489 | no |
| t2-r-1 | 2205c9 | 0.2035 | no |
| t2-g-1 | 26ee29 | 0.1700 | no |
| t3-r-1 | fddb9b | 0.1990 | no |
| t3-g-1 | 8c8b52 | 0.1712 | no |
| t3-g-2 | ad8cd3 | 0.1526 | no |
| t3-r-2 | a91333 | 0.1942 | no |
| t2-g-2 | 5b0b41 | 0.1779 | no, path named |
| t2-r-2 | 79d37e | 0.1874 | no |
| t1-g-2 | 449ce2 | 0.1665 | no |
| t1-r-2 | 9f098b | 0.1973 | no |
| t1-r-3 | 0731b8 | 0.1853 | no |
| t1-g-3 | 3cb117 | 0.1473 | no |
| t2-r-3 | e40017 | 0.1989 | no |
| t2-g-3 | bc948e | 0.1570 | no |
| t3-r-3 | f8abbd | 0.1996 | no, path named |
| t3-g-3 | 2a9e2f | 0.1585 | no |
| t3-g-4 | f2dcfe | 0.1610 | no |
| t3-r-4 | c7e795 | 0.1889 | no |
| t2-g-4 | a37010 | 0.1651 | no |
| t2-r-4 | fea4b5 | 0.2004 | no |
| t1-g-4 | 6507d9 | 0.1599 | no |
| t1-r-4 | 4e51a6 | 0.1966 | no |

---

# The second follow-up, twenty-four sessions, graded

Run by the coordinator's agent on 2026-11-13 from about 11:46 to 11:53:15, four at a time, under the
design registered in `docs/panel/247-briefs/llm-ergonomist.md` § Second follow-up before any of its
sessions (the text before it kept as `llm-ergonomist.before-second-followup.md`). Arms **m2** (hm6
merged into the callback sentence, `4527ecbb3481fcda`) and **g** (hg3, `6e0ef87e82c80239`), each
with § 13's `atexit` example corrected; tasks t1, t2 and t3 as before; four sessions per cell.
**Spend of this run: 4.886197 USD; the seat in all 18.744062 USD** over 88 `run.json` files (the
runner's `DONE 88 run.json in the seat, sum 18.744062 USD, unreadable 0`). Graded under today with
D and G (`p247-scratch/grade/dg/`), with defect 2511 modelled by the critic's shim
(`p247-scratch/grade/shim2511/signal.h`, sha256 `62335cefb0bb054d`) and without it; t2's binaries
run once unsignalled first and their storm armed at 1.0 s, as registered. Grades in
`p247-scratch/grade-fu2/`, handler kinds in `p247-scratch/handlers-followup2.tsv`. Written into this
file by the coordinator from the agent's final message.

**`right` under today with D and G**, four sessions per cell:

| arm | t1 | t2 | t3 | of 12 |
|---|---|---|---|---|
| m2 (hm6), 2511 modelled | 1 | 1 | 0 | **2** |
| m2 (hm6), as built | 0 | 0 | 0 | **0** |
| g (hg3), 2511 modelled | 4 | 4 | 4 | **12** |
| g (hg3), as built | 4 | 4 | 4 | **12** |

**The registered rule** (hg3 replaces hm6 if g gives at least three more `right` than m2 over its
twelve, graded with 2511 modelled): **+10, so hg3 replaces hm6.**

**What m2's gap rests on**: ten of its twelve answers were refused `error[empty_record]` at `record
sigset_t partial`, with the shim and without, the shape defect 2514 (filed by the coordinator at
about 11:50, after this run's registration) names; the other two met 2511 and were right with the
shim. **Every m2 reader took hm6's rule**: 12 of 12 blocked the signals and asked `sigpending`
between steps, none wrote a Heroes handler or a C header, none found a runtime door. Whether those
ten would be right with 2514 repaired is a question, unrun (modelling it would change the answers'
text, which the shim does not). **g, 12 of 12 a runtime door**: `hero_net_catch_stop` and
`hero_net_stop_asked` for t1 and t3, `hero_signal_watch` and `hero_signal_take` for t2. Every binary
that built ran right; no death, no crash report; every t2 warm-up run exited 0.

**t3 per arm**: g 4 of 4, the platform layer's stop; m2 0 of 4, all refused `empty_record`.

**Context**: 24 of 24 answered `no`; none names the working directory.

| session | folder | cost | context | as built | 2511 modelled |
|---|---|---|---|---|---|
| t1-m2-1 | df4c46 | 0.2437 | no | refused (`empty_record`) | refused |
| t1-g-1 | 355d5f | 0.1802 | no | right | right |
| t2-m2-1 | a21273 | 0.3107 | no | refused (`empty_record`) | refused |
| t2-g-1 | 03a4c9 | 0.1961 | no | right | right |
| t3-m2-1 | 4592ae | 0.2259 | no | refused (`empty_record`) | refused |
| t3-g-1 | fa070d | 0.1683 | no | right | right |
| t3-g-2 | 73e70f | 0.1468 | no | right | right |
| t3-m2-2 | b8bd1d | 0.2333 | no | refused (`empty_record`) | refused |
| t2-g-2 | c936f3 | 0.1465 | no | right | right |
| t2-m2-2 | f7491a | 0.2579 | no | refused (`empty_record`) | refused |
| t1-g-2 | bad1e0 | 0.1622 | no | right | right |
| t1-m2-2 | 232979 | 0.2470 | no | refused (`internal error`, 2511) | right |
| t1-m2-3 | 16469e | 0.2065 | no | refused (`empty_record`) | refused |
| t1-g-3 | 2977aa | 0.1583 | no | right | right |
| t2-m2-3 | b0e1ea | 0.3016 | no | refused (`internal error`, 2511) | right |
| t2-g-3 | 3e9c45 | 0.1557 | no | right | right |
| t3-m2-3 | cb0bcc | 0.1889 | no | refused (`empty_record`) | refused |
| t3-g-3 | b89dc2 | 0.1750 | no | right | right |
| t3-g-4 | 461f9b | 0.1491 | no | right | right |
| t3-m2-4 | a3828f | 0.2309 | no | refused (`empty_record`) | refused |
| t2-g-4 | 6911ef | 0.1735 | no | right | right |
| t2-m2-4 | c161ec | 0.2731 | no | refused (`empty_record`) | refused |
| t1-g-4 | 69c79d | 0.1484 | no | right | right |
| t1-m2-4 | 02b8d1 | 0.2067 | no | refused (`empty_record`) | refused |

---

# The third follow-up, eight sessions on a server-shaped task, graded

Run by the coordinator's agent on 2026-11-13 from 12:00:41 to about 12:04, four at a time, under the
design registered in `docs/panel/247-briefs/llm-ergonomist.md` § Third follow-up before any of its
sessions (the text before it kept as `llm-ergonomist.before-third-followup.md`; its pipe sentence
corrected under it the same day). Arms m2 (`4527ecbb3481fcda`) and g (`6e0ef87e82c80239`); task
**t4**, a server on a worker thread that must stop on SIGTERM while blocked in `accept` (`task.md`
`2b274fdd28072352`), given the server half of `hero_net.h` and no stop door; the driver
`grade/drivers/t4run.py` (`b7a095db389de439`), run twice per answer. Before any session the
reference with the layer's stop read right 5 of 5 and the trap (a C handler's flag asked between
accepts) wrong 2 of 2, SIGKILLed 10 s after SIGTERM still blocked in `accept`. **Spend of this run:
2.269020 USD; the seat in all 21.013082 USD of the author's 25** over 96 `run.json` files. One
session, t4-m2-3 (`62e9c1`), ended `error_max_budget_usd` at 0.4959816 USD, 0.096 past its 0.40
cap: the CLI passes its cap by one request. Grades in `p247-scratch/grade-fu3/`, handler kinds in
`p247-scratch/handlers-followup3.tsv`. Written into this file by the coordinator from the agent's
final message.

| arm | 2511 modelled | as built |
|---|---|---|
| m2 (hm6) | 0 of 4 | 0 of 4 |
| g (hg3) | **4 of 4** | **4 of 4** |

**The registered rule** (the stop clause of hg3 counts as measured on a server-shaped task if g gives
at least three more `right` than m2 over its four t4 sessions, graded with the shim): **+4, it
counts.** Without `62e9c1`, which gave no answer, g leads 4 of 4 against 0 of 3.

**What each answer did**: g, all four, called `hero_net_catch_stop`; each run read the port, got
`hello`, printed `stopped` and exited 0 at 0.00 s after the signal. m2: two blocked the signals and
asked `sigpending` and were refused `error[empty_record]` (defect 2514's shape); `62e9c1` gave no
`main.hero` and no context section, having hit its budget, graded refused; `f6921d` bound only the
given groups and caught no signal, so SIGTERM's default ended it (exit -15) in both runs. No crash
report was left.

**Context**: the seven that answered said `no`; t4-g-4 (`498004`) notes that the path suggests a
repository named `heroes-lang`, not voided as before; `62e9c1` gave no answer.

| session | folder | cost | context | 2511 modelled | as built |
|---|---|---|---|---|---|
| t4-m2-1 | f96f45 | 0.3133 | no | refused (`empty_record`) | refused |
| t4-g-1 | 0e2f4a | 0.2613 | no | right | right |
| t4-g-2 | 79fcd5 | 0.2280 | no | right | right |
| t4-m2-2 | 4555ac | 0.3152 | no | refused (`empty_record`) | refused |
| t4-m2-3 | 62e9c1 | 0.4960 | none (budget) | refused (no answer) | refused |
| t4-g-3 | 62be7c | 0.2818 | no | right | right |
| t4-g-4 | 498004 | 0.1867 | no, path named | right | right |
| t4-m2-4 | f6921d | 0.1868 | no | stopped (exit -15) | stopped |
