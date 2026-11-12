# Panel 246, the llm-ergonomist's blind reading: the thread rung (B1) and the clean stop (B2)

Run 2026-11-12 by the coordinator's hand, on the author's `3a` of 2026-11-12 (a full sitting, the
blind seat its one paid run, a ceiling of 10 USD for the sitting), under the rule registered before
any session in `docs/panel/246-briefs/llm-ergonomist.md`, which holds the brief and the three tasks.
The hand set up the folders, ran the sessions and copied their reports; it graded nothing and read
the reports only for their `context` answers. Grading is the coordinator's, by the registered rule.

## How it was run

- **Twenty `claude -p` sessions**, one per folder of
  `.claude/worktrees/p246-scratch/246-llm-ergonomist/<task>-<arm>-<n>/` (inside the repository's
  root, ignored by git: `git check-ignore -v` names `.gitignore:34:.claude/worktrees/`), each folder
  holding, before its session, `brief.md` (the sheet's brief, verbatim), `task.md` (its task's text,
  verbatim) and `heroes-spec.md` (its arm's specification), and nothing else; the session added
  `run.json` and `run.err`. The brief and the tasks were cut from the sheet's four `markdown` fences
  by a script, 31, 11, 14 and 9 lines, as the sheet shows them.
- **The arms**, built by `arms/build_arms.py` from
  `git -C .claude/worktrees/b33-round show ef50a7478:spec/heroes-spec.md`, each sentence taken from
  `00-shared.md` with its wrapped lines joined, T1, T2, S1 and S2 inserted with one leading space
  after the one `linked where a group names one.`, and arm r's `one written as a number is refused).`
  replaced by `one written as a number is refused) · ` and S3. Their `shasum -a 256`, first sixteen
  digits, each equal to the one the briefs registered, and each file `cmp` equal to the
  coordinator's draft in `p246-scratch/price2/`: x `bcb60736643d7772`, y `41ad6690571bd27b`, z
  `a5f822c1d0f79b03`, p `c1cf6e539b036b7d`, q `ea0c4dc22560d2d7`, r `f4e3f600482a50fb`. Every
  session folder's `heroes-spec.md` was hashed again after the copy and gave its arm's digits.
- **The order**, the registered one: `t1-x-1 t1-y-1 t1-z-1 t2-x-1 t2-y-1 t2-z-1 t3-x-1 t3-p-1
  t3-q-1 t3-r-1`, then the same in reverse with `-2`, `t3-r-2` to `t1-x-2`. Not run: a t3 arm with
  panel 114's R2 sentence, which is not in the twenty.
- **The command**, the sheet's, verbatim, run from each session's folder by
  `246-llm-ergonomist/run_all.sh`, one session at a time, the key loaded in the same shell by
  `. /Users/joseph/Temp/heroes/heroes-lang/.env` and never printed (`${#ANTHROPIC_API_KEY}` printed
  once: 108):

  ```
  env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
      GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
    claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
      --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
      --tools "Read,Write" --allowedTools "Read,Write" \
      --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
      --max-budget-usd 0.45 --output-format json > run.json 2> run.err
  ```

- **The bound**: each session capped at 0.45 USD by `--max-budget-usd`; before each start the
  runner summed `total_cost_usd` over every `run.json` written so far and started the next session
  only while that sum was at most 6.50 USD, stopping on any `run.json` without a readable cost.
  The first session ran alone and its `run.json` was read (subtype, model, turns, denials, the four
  headings present) before the other nineteen were started.
- **The CLI**: `claude --version` read `2.1.311 (Claude Code)` (`/opt/homebrew/bin/claude`), and
  `claude --help` listed every flag of the command. **The model**: `claude-opus-5-5` answered every
  session, and it is the only model `run.json`'s `modelUsage` names in any of the twenty. Every
  session ended `subtype` `success`, `is_error` false, `stop_reason` `end_turn`, with no permission
  denied, 4 turns each but `t2-x-1`'s 5; every `run.err` is empty (0 bytes in all).
- **The machine**: held awake by assertions already standing (`pmset -g assertions`:
  `PreventSystemSleep` 1 by screen sharing, `PreventUserIdleSystemSleep` 1 by powerd), so no
  `caffeinate`. Other sessions loaded the Mac (`uptime` 13.78 at the start, 16.68 at the end), so no
  duration is recorded here as a measurement.
- **What it cost**: 3.8216 USD in all by the CLI's `total_cost_usd` (the runner's sum and the
  assembler's agree, 3.821610), 0.1419 to 0.3258 a session, under the runner's bound of 6.50, the
  caps' 9.00 and the author's 10 for the sitting. The bound never stopped a session. No session
  reached its 0.45 cap.
- **Where the reports came from**: no session wrote `report.md` (`find` over the folders: 0); each
  gave its report as its final message, which the CLI writes as `result` in `run.json`, and that
  `result` is what is copied below, byte for byte, by `246-llm-ergonomist/assemble.py`. Each holds
  the brief's four headings, `answer`, `choice_points`, `prediction` and `context`.

## Context

Every session answered the brief's last question **no**: no instruction, rule, contract, memory or
document other than its folder's three files reached it, each naming at most the CLI's own system
prompt (the agent framing, the working directory, the date, the tools) and the one-line instruction.
Five said what they wrote of C headers or POSIX came from their training. **Two noted the working
directory's path**, which the CLI's system prompt carries: `t1-z-1` named
`p246-scratch/246-llm-ergonomist/t1-z-1`, and `t1-x-2` said the path names `heroes-lang` and
`llm-ergonomist`. No `yes`, so no session is void by the sheet's rule; whether the path is a leak the
coordinator weighs is the coordinator's reading, not this hand's.

## The twenty sessions

In the order they ran. Cost is the CLI's `total_cost_usd`; *context* is the session's own
answer to the brief's last question, as the coordinator's hand read it.

| # | session | task | arm | cost (USD) | turns | `context` answer | report from |
|---|---|---|---|---|---|---|---|
| 1 | t1-x-1 | t1 | x | 0.2376 | 4 | no: the three files, the environment notice and working directory, the one-line instruction | `result` in `run.json` (no `report.md`) |
| 2 | t1-y-1 | t1 | y | 0.1528 | 4 | no: the three files, the harness setup (working directory, date) | `result` in `run.json` (no `report.md`) |
| 3 | t1-z-1 | t1 | z | 0.1614 | 4 | no: the three files, the harness framing; it names the working-directory path, `p246-scratch/246-llm-ergonomist/t1-z-1` | `result` in `run.json` (no `report.md`) |
| 4 | t2-x-1 | t2 | x | 0.3258 | 5 | no: the three files, the system prompt, the instruction; C header knowledge from training | `result` in `run.json` (no `report.md`) |
| 5 | t2-y-1 | t2 | y | 0.1577 | 4 | no: the three files, the harness setup and the instruction | `result` in `run.json` (no `report.md`) |
| 6 | t2-z-1 | t2 | z | 0.1734 | 4 | no: the three files, the system setup (working directory, date) | `result` in `run.json` (no `report.md`) |
| 7 | t3-x-1 | t3 | x | 0.2703 | 4 | no: the three files, the system prompt and the instruction; C and POSIX knowledge from training | `result` in `run.json` (no `report.md`) |
| 8 | t3-p-1 | t3 | p | 0.1645 | 4 | no: the three files, the agent and tool instructions | `result` in `run.json` (no `report.md`) |
| 9 | t3-q-1 | t3 | q | 0.1658 | 4 | no: the three files, the system prompt (working directory, date) | `result` in `run.json` (no `report.md`) |
| 10 | t3-r-1 | t3 | r | 0.1494 | 4 | no: the three files; C knowledge from training | `result` in `run.json` (no `report.md`) |
| 11 | t3-r-2 | t3 | r | 0.1698 | 4 | no: the three files, the system prompt (working directory, date) | `result` in `run.json` (no `report.md`) |
| 12 | t3-q-2 | t3 | q | 0.1596 | 4 | no: the three files, the system prompt (working directory, date) | `result` in `run.json` (no `report.md`) |
| 13 | t3-p-2 | t3 | p | 0.1749 | 4 | no: the three files, the system prompt and working directory | `result` in `run.json` (no `report.md`) |
| 14 | t3-x-2 | t3 | x | 0.2955 | 4 | no: the three files, the system prompt; `signal.h` knowledge from training | `result` in `run.json` (no `report.md`) |
| 15 | t2-z-2 | t2 | z | 0.1563 | 4 | no: the three files, the system prompt and the instruction | `result` in `run.json` (no `report.md`) |
| 16 | t2-y-2 | t2 | y | 0.1562 | 4 | no: the three files, the system prompt (working directory, date, tool) | `result` in `run.json` (no `report.md`) |
| 17 | t2-x-2 | t2 | x | 0.1825 | 4 | no: the three files, the system prompt; `pthread.h` knowledge from training | `result` in `run.json` (no `report.md`) |
| 18 | t1-z-2 | t1 | z | 0.1869 | 4 | no: the three files, the working directory and date | `result` in `run.json` (no `report.md`) |
| 19 | t1-y-2 | t1 | y | 0.1419 | 4 | no: the three files, the standard system instructions | `result` in `run.json` (no `report.md`) |
| 20 | t1-x-2 | t1 | x | 0.2395 | 4 | no: the three files, the system and tool instructions; it notes the working-directory path names `heroes-lang` and `llm-ergonomist` | `result` in `run.json` (no `report.md`) |

**Total**: 3.8216 USD over 20 sessions (3.821610 unrounded), 0.1419 to 0.3258 a session.

## The reports

Each as the session gave it, byte for byte, in the order the sessions ran.

### t1-x-1

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't create `report.md`: the only tool I have here reads files and cannot write them. As the brief allows, the whole report is below.

# report.md

## answer

**Short answer:** I expect the compiler to accept `main.hero` together with the header `bands.h` that I wrote. The program should print `170666346666800000` twice. The specification doesn't support that verdict fully: threads are the one thing the task needs that it never describes.

Section 13 says only that threads come "from the language's `hero_os.h`". It gives no function name, signature or type from that header, and clang checks every declaration against the header. So the specification alone can't tell a model how to make a thread.

I went around this with the FFI rules, which *are* fully specified. I wrote a small C header of my own, `bands.h`, which wraps POSIX `pthread_create` and `pthread_join`. It starts a Heroes callback `(function(i64) -> i64)` on its own thread. All the summing is done in Heroes; the C code only starts threads and waits for them.

**Expected result.** The sum of i² for 0 ≤ i < 800,000 is (n−1)·n·(2n−1)/6 with n = 800,000, which is **170666346666800000**. That is well below the largest `i64` (about 9.22·10¹⁸), so nothing overflows.

### `main.hero`

```
# Sums `i * i` for every `i` in [0, 800,000): first on eight threads, one per
# band of 100,000, then on the program's own thread alone.

extern "bands.h" link "pthread"
    record BandJob tag band_job
    function band_spawn(f: (function(i64) -> i64), arg: i64) -> BandJob acquires band_join
    function band_join(job: BandJob consumes) -> i64

constant BAND_SIZE: i64
    100_000

constant BANDS: i64
    8

# The sum of `i * i` for `i` in [from, to).
function sum_squares(from: i64, to: i64) -> i64
    total @= 0
    for i in range(from: from, to: to)
        total @ total + i * i
    return total

# The sum of band number `band`, run on a thread of its own.
function band_sum(band: i64) -> i64
    return sum_squares(from: band * BAND_SIZE, to: (band + 1) * BAND_SIZE)

function main()
    jobs: [BandJob] @= []
    for band in range(from: 0, to: BANDS)
        jobs @ jobs.push(band_spawn(band_sum, band))
    threaded @= 0
    for job in jobs
        threaded @ threaded + band_join(job)
    print(threaded)
    print(sum_squares(from: 0, to: BANDS * BAND_SIZE))
```

### `bands.h`

```c
#ifndef BANDS_H
#define BANDS_H

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

/* One band's work: the callback, its argument, its result, its thread. */
struct band_job {
    int64_t (*f)(int64_t);
    int64_t arg;
    int64_t result;
    pthread_t thread;
};

static inline void *band_run(void *p) {
    struct band_job *job = p;
    job->result = job->f(job->arg);
    return NULL;
}

/* Starts f(arg) on a new thread; aborts if the thread cannot be made. */
static inline struct band_job *band_spawn(int64_t (*f)(int64_t), int64_t arg) {
    struct band_job *job = malloc(sizeof *job);
    if (job == NULL) {
        abort();
    }
    job->f = f;
    job->arg = arg;
    job->result = 0;
    if (pthread_create(&job->thread, NULL, band_run, job) != 0) {
        abort();
    }
    return job;
}

/* Waits for the thread, frees the job, and returns f(arg). */
static inline int64_t band_join(struct band_job *job) {
    pthread_join(job->thread, NULL);
    int64_t result = job->result;
    free(job);
    return result;
}

#endif
```

Build: `heroes build main.hero`, with `bands.h` in the same directory. The program uses no packages, so there is no `heroes.toml`.

## choice_points

1. **Where the threads come from.** The specification says `hero_os.h` but says nothing about what is in it.
   - *My choice:* my own header wrapping pthreads, declared under the FFI rules.
   - *The other option:* guess an API, for example `extern "hero_os.h"` with `function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join`. Unless the guess matched the real header exactly, the compiler would refuse it (clang checks names and types against the header).
   - **This is the main gap in the specification for this task.**
2. **Calling `pthread_create` directly from Heroes instead of a wrapper.** This doesn't work, for four reasons:
   - The callback would have to be `(function(ptr) -> ptr)`, and Heroes has no way to turn an `i64` into a `ptr` or back. So a thread couldn't learn its band number or return its sum.
   - A lend lives only for its call and reaches only `lent` parameters, but `pthread_create` keeps `arg`.
   - `pthread_t` is a pointer on macOS but `unsigned long` on Linux.
   - Result: the compiler refuses it, or the program can't be written at all. That is why I used a wrapper.
3. **Whether a header beside `main.hero` is found by `extern "bands.h"`.** The specification never says which directories are searched for headers.
   - *My choice:* assume the directory of the compiled file is searched.
   - *The other outcome:* if it isn't, the build fails with "header not found", so the compiler refuses the program.
4. **`link "pthread"` versus no `link`.**
   - On macOS and on glibc 2.34 or later, pthreads are in libc and `-lpthread` still resolves, so the same program is accepted either way.
   - On an older glibc, leaving out `link` would fail at link time.
5. **Calling a Heroes callback from a thread that C created.** The specification doesn't say whether the runtime is thread-safe on such threads (allocation, the array built by `range`, abort reporting).
   - *My choice:* assume it is, since the specification names threads as supported.
   - *If it isn't:* the program could crash or abort while running, or in the worst case print a wrong first line.
6. **Keeping the handles in a `[BandJob]` array and consuming them through the loop's copy `job`.** The specification suggests handles may sit in array elements ("through a field or an array element").
   - It doesn't say whether the stale copy left in `jobs` counts as an unended handle when `main` returns.
   - *If it does:* the program prints both lines and then aborts at exit with "8 handles not consumed".
   - *Safer option:* eight separately named handles, `j0` … `j7`, which the specification clearly supports. They give the same output with no doubt about this.
7. **`band_spawn` names the result `acquires band_join`, and `band_join` takes its parameter with `consumes`.**
   - *Not chosen:* leaving out the marks. That leaves the handle's life untracked. It is probably still accepted, because there would then be no `consumes` anywhere to force the marking rule, and the output is the same.
   - *Not chosen:* adding `when 0` to `band_join`. That is wrong here, because the result is a sum and not a status code. It would make the program wrong whenever a band's sum isn't 0.
8. **Summing with `for i in range(...)` (which builds the whole array) instead of a `while` loop with a counter.**
   - The answer is the same either way; the `range` version uses about 6.4 MB per call.
   - The specification guarantees that `range` excludes `to`. If it included `to`, the sum would be wrong (each band would also count its next band's first number).
9. **Named arguments on `sum_squares(from:, to:)`.** The specification requires names when two parameters share a type. Calling it without names would be refused by the compiler.
10. **Literal types.** `total @= 0` and `threaded @= 0` default to `i64`, which matches `i * i` and `band_join`'s result. Giving either cell another integer type would be refused, because the specification allows no mixing of types.

## prediction

- **Compiler:** I expect it to accept `main.hero` with `bands.h`. This depends on choice points 3 and 6, and on the compiler compiling a header that contains `static inline` functions.
- **Output:**

```
170666346666800000
170666346666800000
```

- **Exit code:** 0.

**Main risks**, most likely first:
- The header isn't found next to the source file, so the compiler refuses the program.
- The handle copies left in `jobs` cause an abort at exit, after both lines have printed.
- The runtime isn't safe on threads that C created, so the program may fail while running.

If the program had been written against a guessed `hero_os.h` API instead, I expect the compiler would refuse it.

## context

Nothing from outside this directory reached me apart from the general system setup:
- the agent-environment notice and the current working directory;
- the user's instruction to read `brief.md`, follow it, and answer in English.

I read only `brief.md`, `task.md` and `heroes-spec.md`. No project rules, memory files or other documents were in my context.
`````

### t1-y-1

From `result` in `run.json` (no `report.md`).

`````markdown
I had no tool for writing files, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for one program, `main.hero`. It adds up `i * i` for `i` from 0 up to (not including) 800,000 twice. The first time, eight threads run at once, each summing one band of 100,000 numbers, and the eight results are added. The second time, the program's own thread does it alone. Each result goes on its own line. I expect `heroes build main.hero` to accept the program and print `170666346666800000` twice.

As a check, the closed form is (n−1)·n·(2n−1)/6 with n = 800,000, which is 799,999 · 800,000 · 1,599,999 / 6 = 170,666,346,666,800,000. That fits in an `i64` (the limit is about 9.22·10^18), and so does every partial sum along the way, so nothing overflows.

### `main.hero`

```
extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

constant BANDS: i64
    8

constant BAND_SIZE: i64
    100_000

# Sum of `i * i` for every `i` from `from` up to and not including `to`.
function squares(from: i64, to: i64) -> i64
    total @= 0
    i @= from
    while i < to
        total @ total + i * i
        i @ i + 1
    return total

# The sum of squares over band `k`, run on a thread of its own.
function band(k: i64) -> i64
    return squares(from: k * BAND_SIZE, to: (k + 1) * BAND_SIZE)

function main()
    handles: [i64] @= []
    for k in range(from: 0, to: BANDS)
        handles @ handles.push(hero_thread_spawn(body: band, arg: k))
    threaded @= 0
    for h in handles
        threaded @ threaded + hero_thread_join(handle: h)
    print(threaded)
    print(squares(from: 0, to: BANDS * BAND_SIZE))
```

No other file is needed. The program uses no package, so it has no `heroes.toml` catalogue (section 1).

## choice_points

1. **How to reach threads.** Section 13 says threads come from the language's `hero_os.h`, "linked where a group names one". I took that to mean they are not built-ins and need an `extern "hero_os.h"` group. I declared the two functions exactly as the spec writes them.
   - *Other choice:* call `hero_thread_spawn` without declaring it. I expect the compiler to refuse that as an unknown name.

2. **`link` on the group.** I left `link` off, reading "linked where a group names one" as the language's own platform layer being linked automatically.
   - *Other choice:* add something like `link "hero_os"`. If no such library exists, the link step would fail and the build would be refused. If it does exist, the result is the same program.

3. **C widths in the declarations.** I used `i64` for everything, as the spec's own signatures do. Section 13 refuses a parameter whose width disagrees with the header.
   - *Other choice:* `i32` or `u64` would be refused if the header says `int64_t`.

4. **No life-cycle marks on the thread handle.** The handle is a plain `i64`, not a handle record, so I added no `acquires`/`consumes`. The spec says those words "mark only a value that reaches a handle… anywhere else they are errors".
   - *Other choice:* adding `acquires hero_thread_join` would be refused by the compiler.

5. **Named arguments to `squares`.** Its two parameters share a type, so names are mandatory at the call site (section 9).
   - *Other choice:* `squares(0, 800000)` would be refused.
   - I also named the arguments of `hero_thread_spawn` and `hero_thread_join`. That isn't required, since `Arg = [ident ":"] …` makes names optional; leaving them off would give the same program.

6. **`while` loop instead of `for i in range(...)` inside `squares`.** Both are legal and give the same answer. `range` would first build an array of 100,000 (or 800,000) `i64` values, which is slower and uses more memory but still prints the same numbers.

7. **Spawning all eight threads before joining any.** The task says the eight threads must run at the same time.
   - *Other choice:* joining each thread right after spawning it would still print the right number, but the threads would run one after another, which breaks the task.

8. **Joining in spawn order and adding as I go.** Addition here is exact and never overflows, so the order doesn't change the answer.

9. **The sum is `i64` without an annotation.** `total @= 0` takes `i64` because that is what an unannotated literal gets (section 2). The value fits, so no overflow abort.
   - *Other choice:* `i32` or `u32` would overflow and stop the program with an error. `u64` would also need `from: u64` and conversions, but it would work.

10. **The empty array needs a type.** I wrote `handles: [i64] @= []`, as section 10 requires.
    - *Other choice:* without `: [i64]`, the compiler would refuse it.

11. **Growing the array.** I used `handles @ handles.push(...)` on a plain name, which grows in place (section 10). The fact that `push` returns the array is inferred from that same example.

12. **Constants.** I used SCREAMING_CASE names with literal bodies; `100_000` puts the `_` between digits, as section 2 allows. Using them directly as `range` bounds and inside `band` is fine.

## prediction

I expect the compiler to accept the program as written. Every binding and parameter is used, every cell is re-bound, and no value is silently dropped. Passing `band` as a value is allowed because it has no `@` or `lent` parameter. The function's `return` covers its path.

When built and run, it prints:

```
170666346666800000
170666346666800000
```

The main risk is points 1–2: I am assuming that an `extern "hero_os.h"` group with no `link` resolves and links the platform's thread functions. If the toolchain wanted a different form, the build would be refused, not produce a wrong answer.

## context

Apart from the system/harness setup (an agent working directory and the date), nothing other than `brief.md`, `task.md` and `heroes-spec.md` reached my context. That means no other instructions, rules, contracts, memories or documents. I read no other file.
`````

### t1-z-1

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the whole report follows as my final answer, as the brief allows.

# report.md

## answer

The task asks for `main.hero`. It adds up `i * i` for `i` from 0 up to but not including 800,000 in two ways. First, eight threads each add up one band of 100,000 numbers at the same time, and the program adds their eight results. Then the program's own thread adds up the whole range alone. Each result goes on its own line.

The specification never lists what `hero_os.h` declares. It only says that threads come from it and that "into a thread and back crosses one `i64` and nothing else". So the three thread declarations in the `extern` group below are my guesses at that header, written in the specification's FFI syntax.

The expected value is (n−1)·n·(2n−1)/6 with n = 800,000: 799,999 · 800,000 · 1,599,999 / 6 = **170666346666800000**. That fits in an `i64`.

### `main.hero`

```
# Threads come from the language's own header; into a thread and back crosses one i64.
extern "hero_os.h"
    record Thread tag hero_thread
    function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join
    function hero_thread_join(t: Thread consumes) -> i64

constant BANDS: i64
    8

constant BAND_SIZE: i64
    100_000

# Sum of i * i for every i from `from` up to and not including `to`.
function range_sum(from: i64, to: i64) -> i64
    total @= 0
    for i in range(from: from, to: to)
        total @ total + i * i
    return total

# The body of one thread: the sum over the band that starts at `start`.
function band_sum(start: i64) -> i64
    return range_sum(from: start, to: start + BAND_SIZE)

function main()
    threads: [Thread] @= []
    for k in range(from: 0, to: BANDS)
        threads @ threads.push(hero_thread_spawn(band_sum, k * BAND_SIZE))
    parallel @= 0
    for t in threads
        parallel @ parallel + hero_thread_join(t)
    print(parallel)
    print(range_sum(from: 0, to: BANDS * BAND_SIZE))
```

No other files are needed. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **What `hero_os.h` declares (names, handle tag, signatures).** The specification says threads come from this header but never lists its contents. I guessed `hero_thread_spawn`, `hero_thread_join` and the tag `hero_thread`.
   - *If the real names or tag differ:* the compiler refuses the program. clang checks every declaration against the header, and an unknown symbol or tag fails.
   - This is the decisive gap. No reading of the specification alone tells me these names.
2. **How a thread's result comes back.** I chose a join that returns the `i64`.
   - *Alternative:* an out-parameter, such as `@result: i64` plus a status result marked `when 0`.
   - *If the header uses the other style:* the compiler refuses the program, because the declaration disagrees with the header.
3. **Thread argument and callback type.** I chose a callback `(function(i64) -> i64)` plus a separate `arg: i64`.
   - *Alternative:* a callback with no argument. Then each band would need its own function, because there are no closures or anonymous functions.
   - *If the header's arity or widths differ:* the compiler refuses the program.
4. **Lifetime marks on the handle.** I marked spawn with `acquires hero_thread_join` and join's handle parameter with `consumes`.
   - *If I left them out and the header's handle needs them:* the program might compile and still be wrong. If the language tracked the lives, an unended handle would abort when `main` returns.
   - *If join were marked `when 0`:* join returns a sum, not a status, so `consumes` would count only when the sum happens to be 0. Handles would leak and abort at exit.
5. **Where the handles are kept.** I push them into `threads: [Thread]` and join each one with `for t in threads`, where `t` is a copy of the element.
   - *Alternative:* index with `threads[k]`.
   - The specification says handing C "a copy made before" a handle was ended aborts. Each handle is ended only once, through its one copy, so I expect no abort. If the language instead tracks the array element itself as unconsumed, the program would abort at the end of `main`. I judge that unlikely.
6. **Starting all threads before joining any.** I spawn all eight, then join them in order, so they run at the same time.
   - *If I joined inside the spawn loop:* it would compile and print the same numbers, but the threads would run one after another, which breaks the task.
7. **Named arguments.** `range_sum(from:, to:)` has two `i64` parameters, so the labels are mandatory. `hero_thread_spawn` has two parameters of different types, so I left its labels out.
   - *If I left out labels on `range_sum` or `range`:* the compiler refuses the program.
8. **Building the loop with `range`.** `range` creates an array of 100,000 or 800,000 `i64`s.
   - *Alternative:* a `while` loop with a counter cell, which uses less memory.
   - Both give the same answer. No overflow is possible, because the largest partial sum is about 1.7e17.
9. **Literals and constants.** `100_000` uses the digit separator the specification allows. `total @= 0` and `parallel @= 0` take the type `i64` by default.
   - *If I gave a different integer type (for example `i32`) anywhere:* the compiler would refuse the program for a width mismatch, or it would abort on overflow when run.
10. **Linking.** I wrote no `link` clause. The specification says the language's own headers are "linked where a group names one".
    - *If a library name is in fact needed:* linking fails, so the build is refused.

## prediction

- **Compiler:** I expect it to **refuse** the program, unless my guesses at `hero_os.h` (choice points 1–3) happen to match the real header. The specification gives no way to know them. Everything outside the `extern` group should type-check by the rules as written.
- **Output, if it compiles with the header as I guessed:**
  ```
  170666346666800000
  170666346666800000
  ```
  The program should exit normally with no abort, because each thread handle is acquired once and consumed once.

## context

Apart from the files in this directory, the only thing that reached my context was the agent harness's system framing. It identifies me as a Claude agent, gives the working directory path (a worktree named `p246-scratch/246-llm-ergonomist/t1-z-1`) and the date, and states the tool-use conventions. No other instruction, rule, contract, memory or project document reached me. I read only `brief.md`, `task.md` and `heroes-spec.md`.
`````

### t2-x-1

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I have no tool that writes files, so the whole report is below as the brief's fallback says.

# report.md

## answer

**What I expect:** the compiler accepts `main.hero` on Linux with glibc. Run, it prints `4`, `2`, `6`, `2` and then `14`, one number per line. On macOS I expect the build to fail, because macOS has no `threads.h`. All of this is a prediction, not a test result.

**Why the program looks like this.** The spec says threads come from "the language's `hero_os.h`" (§13), but it never says what that header declares. Any declaration I wrote against it would be a guess at function names. So I used the C11 standard header `threads.h`, a C library that §13's FFI rules let me declare and clang can check.

Each thread has to get its sentence and hand back a count. The language makes both awkward:
- There are no closures, no anonymous functions and no mutable globals.
- A Heroes program can't look inside a `ptr`.

So I wrote one worker function per sentence. Each one reads its sentence from a constant array. Each returns its count as the thread's `int` result, which `thrd_join` writes into an `@` cell. All four threads start before the first join, so they run at the same time.

### `main.hero`

```
# Word counts of four sentences, each counted on a thread of its own.

extern "threads.h" link "pthread"
    function thrd_create(@thr: u64, func: (function(ptr) -> i32), arg: ptr) -> i32
    function thrd_join(thr: u64, @res: i32) -> i32

constant SENTENCES: [str]
    ["the quick brown fox", "jumps over", "the lazy dog sleeps all day", "and wakes"]

# The words of `s`: runs of characters between single spaces.
function count_words(s: str) -> i32
    if s.len() == 0
        return 0
    n: i32 @= 1
    for i in range(from: 0, to: s.len())
        if s[i] == ' '
            n @ n + 1
    return n

function count_0(_: ptr) -> i32
    return count_words(SENTENCES[0])

function count_1(_: ptr) -> i32
    return count_words(SENTENCES[1])

function count_2(_: ptr) -> i32
    return count_words(SENTENCES[2])

function count_3(_: ptr) -> i32
    return count_words(SENTENCES[3])

function main()
    t0: u64 @= 0
    t1: u64 @= 0
    t2: u64 @= 0
    t3: u64 @= 0
    c0 = thrd_create(@t0, count_0, nullptr)
    assert c0 == 0
    c1 = thrd_create(@t1, count_1, nullptr)
    assert c1 == 0
    c2 = thrd_create(@t2, count_2, nullptr)
    assert c2 == 0
    c3 = thrd_create(@t3, count_3, nullptr)
    assert c3 == 0
    r0: i32 @= 0
    r1: i32 @= 0
    r2: i32 @= 0
    r3: i32 @= 0
    j0 = thrd_join(t0, @r0)
    assert j0 == 0
    j1 = thrd_join(t1, @r1)
    assert j1 == 0
    j2 = thrd_join(t2, @r2)
    assert j2 == 0
    j3 = thrd_join(t3, @r3)
    assert j3 == 0
    print(r0)
    print(r1)
    print(r2)
    print(r3)
    print(r0 + r1 + r2 + r3)
```

There are no other files. The program uses no package, so there is no `heroes.toml`.

## choice_points

1. **Which threading header to use.** The spec says threads come from `hero_os.h` but documents nothing in it. I chose the real C11 `threads.h`.
   - *Other choice:* declare guessed `hero_os.h` functions, such as `hero_thread_spawn`. clang checks every declaration against the header, so a wrong name or signature means **the compiler refuses the program**. Nothing in the spec lets me get the names right on the first try.
   - *Risk of my choice:* a checker might expect `hero_os.h`.
   - This is the main gap in the spec for this task.

2. **`threads.h` versus `pthread.h`.**
   - *Other choice:* pthreads. The thread function returns `void *`, and a Heroes program can't turn an integer into a `ptr`. The count would have no way back to `main` except a workaround such as writing it to a file and parsing it again. Also, `pthread_t` is a pointer on macOS but `unsigned long` on Linux, so one declaration can't fit both.
   - *My choice:* `threads.h`, where the thread function returns `int`, so the count comes back directly through `thrd_join`'s `int *res`.
   - *Cost:* macOS has no `threads.h`, so there **the compiler refuses the program** because the header is missing.

3. **How to declare `thrd_t`.** I wrote `@thr: u64`, which matches glibc's `unsigned long`.
   - On musl, `thrd_t` is a pointer (`struct __pthread *`), so the width/sign check makes **the compiler refuse it**. A handle record such as `record Thrd tag __pthread` would fit musl and fail on glibc.

4. **`link "pthread"`.** I wrote it so that glibc older than 2.34 links. On 2.34 and later, glibc and musl still ship a stub `libpthread`, so it's harmless.
   - *Other choice:* leave it out. That works on new glibc, but on old glibc **the link step fails** with unresolved `thrd_*` symbols.

5. **How each thread gets its sentence.** I used four separate worker functions, each indexing a constant.
   - *Other choice:* one worker that takes the sentence through `arg`, for example as a leased `cstr`. The C type is `void *`, and the callback type would become `int (*)(char *)`. Recent clang treats that mismatch as an error, so **the compiler would likely refuse it**. Even passing `cstr` where C expects `void *` may count as "disagrees".
   - A `ptr` argument can't be read from Heroes at all.

6. **Checking the result with the literal 0.**
   - *Other choice:* declare `constant thrd_success: i64`. That breaks the SCREAMING_CASE rule for constants, while the name has to match C's spelling. Which rule wins is unclear, so it is a possible **compile refusal**.
   - `0` is `thrd_success` in glibc's enum.

7. **Result width.** I declared `-> i32`, matching C's `int`. §13 also allows a wider result such as `i64`. Either should compile.

8. **Sentences as an array constant `[str]`.** §4 says a constant's body "computes over literals and other constants", and the spec gives no container example.
   - If container literals are not allowed there, **the compiler refuses it**.
   - The other choice was four `str` constants, which avoids that risk but is more verbose.

9. **Type of `s.len()`.** The spec never gives the type `len` returns. I assumed `i64`, so it works with `range(from: 0, to: s.len())`, which takes `i64`.
   - If it returns `u64`, the `range` call is **refused** and would need `to_i64(s.len()).must()`.

10. **How words are counted.** I count spaces plus one, and return 0 for an empty string. That matches "a run of characters between single spaces".
    - *Other choice:* counting non-empty runs. That gives the same answer here, because the inputs contain no double spaces.
    - `s[i] == ' '` compares a `u8` with a character literal, which takes the type `u8`.

11. **Starting and joining threads.** I used four explicit cells and a fixed order: create all four, then join all four.
    - *Other choice:* arrays of thread ids and workers with `@ts[i]` element out-parameters. That is more compact, but it leans more on how `@` places and function-value arrays behave.
    - Joining each thread right after creating it would compile and print the same output, but the threads would run one after another, not "at the same time".

12. **Heroes code running on a thread C started.** The spec allows Heroes functions as C callbacks (§13) but says nothing about the runtime being thread-safe.
    - If it isn't (for example a shared allocator or abort state), **the program could stop with an error or crash at run time**. Nothing in the spec lets me rule this out.

13. **`assert` in `main`.** The grammar allows `assert` in any block. If one fails, the program aborts and shows the expression.
    - *Other choice:* drop the results with `_ = thrd_create(...)`. That is allowed because the results are not `T?`, but it would hide failures.

## prediction

- **Linux, glibc ≥ 2.28** (where `thrd_t` is `unsigned long`): I expect `heroes build main.hero` to accept the program. Running it prints:
  ```
  4
  2
  6
  2
  14
  ```
- **macOS:** I expect a refusal because `threads.h` is missing.
- **musl:** I expect a refusal because `thrd_t` is a pointer, not a `u64`.

My confidence that it compiles is moderate. The main risks are choice points 1, 8 and 9.

## context

The only files I read were `brief.md`, `task.md` and `heroes-spec.md` in this directory. Two other things were in my context: the agent-SDK system prompt (working directory, date, tool list) and the user's one-line instruction to follow the brief. No other rule, contract, memory or document reached my context. Everything I said about C headers (`threads.h`, `pthread.h`, glibc, musl, macOS) comes from my general training knowledge, not from any file.
`````

### t2-y-1

From `result` in `run.json` (no `report.md`).

`````markdown
I can't write files: the only tool I have here reads files, so `report.md` was not created. Following the brief's fallback, the whole report is below.

# report.md

## answer

The task asks for `main.hero`. It holds the four sentences, counts each sentence's words on its own thread with all four threads running at once, and prints the four counts in order and then their sum. The program is one file, and it uses no package, so there is no `heroes.toml`.

### `main.hero`

```
# Counts the words of four sentences, one thread per sentence.

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# The sentence with index `i`, in the task's order.
function sentence(i: i64) -> str
    return match i
        0 => "the quick brown fox"
        1 => "jumps over"
        2 => "the lazy dog sleeps all day"
        _ => "and wakes"

# Words are runs of characters between single spaces: one more than the spaces.
function count_words(s: str) -> i64
    if s.len() == 0
        return 0
    n @= 1
    for i in range(from: 0, to: s.len())
        if s[i] == ' '
            n @ n + 1
    return n

# The body of each thread: only one i64 can go in and one come out.
function work(i: i64) -> i64
    return count_words(sentence(i))

function main()
    handles: [i64] @= []
    for i in range(from: 0, to: 4)
        handles @ handles.push(hero_thread_spawn(body: work, arg: i))
    total @= 0
    for h in handles
        c = hero_thread_join(h)
        print(c)
        total @ total + c
    print(total)
```

All four threads are started before the first join, so they run at the same time. Joining in the order of the handles prints the counts in the order of the sentences, whatever order the threads finish in.

## choice_points

1. **How a sentence reaches its thread.** The spec says only one `i64` goes into a thread and one comes out. So the thread gets the sentence's index, and `sentence(i)` returns the text.
   - I used a function with `match` on `i64`, with `_` as the last arm, because the spec allows `_` on `i64`.
   - **Alternative:** a constant `SENTENCES: [str]` whose body is a list literal. The spec says neither yes nor no to array constants. If it is not allowed, the compiler refuses the program; if it is, it runs the same.
   - **Alternative:** a `match` with no `_` arm. The compiler would refuse it, since it cannot be exhaustive on `i64`.
   - **Trade-off of my choice:** any index from 3 upward gives "and wakes". That is harmless here, because only 0 to 3 are ever passed.

2. **Declaring the thread functions.** The spec says threads come from `hero_os.h`, so I wrote an `extern "hero_os.h"` group, copying the two signatures from section 13 exactly.
   - **No `link`:** the spec says the platform layer is linked "where a group names one", meaning the header. Writing `link "hero_os"` with a guessed library name would probably fail at link time, so the compiler would refuse it.
   - **Alternative:** leaving the group out. The two names would then be undefined, and the compiler would refuse the program.

3. **No ownership marks on the thread handle.** The handle is a plain `i64`, not a handle record, so I put no `acquires` or `consumes` on it. The spec says those words on a value that reaches no handle are errors, so adding them would make the compiler refuse it.

4. **Named arguments for `hero_thread_spawn`.** I wrote `body:` and `arg:`. Its two parameters have different types, so the names are optional, and leaving them out would compile the same. For `range`, the names `from:` and `to:` are required because both parameters are `i64`. Leaving them out would make the compiler refuse it.

5. **What `s.len()` returns.** The spec doesn't give its result type. I assumed `i64`, which `range(from: 0, to: s.len())` needs.
   - **If it is `u64`:** the compiler refuses the call to `range`, and I would need `to_i64(s.len()).must()`. That would also work if it is already `i64`, but it is more noise.

6. **How to find a space.** I compared bytes with `s[i] == ' '`. The character literal takes `u8` from the context.
   - **Alternative:** `for c in s.chars()` with `c == " "`. It runs the same.
   - **To avoid:** writing `' '` where an `i64` is expected against `s[i]` without context, or comparing to `32` typed as `i64`. That mixes widths, and the compiler would refuse it.

7. **How a word is counted.** I counted words as spaces + 1, with an empty string counting 0.
   - **Alternative:** counting runs of non-space characters. That gives the same answers here, because the sentences have single spaces and no leading or trailing space.
   - **Without the empty-string guard:** the count would be wrong only for `""`, which never occurs here.

8. **Joining.** I started all four threads, then joined them in order.
   - **Alternative:** spawning and joining each one in turn in a single loop. It prints the same, but the threads would not run at the same time, which breaks the task without any error.

## prediction

I expect `heroes build main.hero` to accept the program, assuming `s.len()` is `i64` and `hero_os.h` needs no `link` (choice points 2 and 5). When run, it prints:

```
4
2
6
2
14
```

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only things in my context were the harness's own system instructions. These were the agent setup, the working directory and the date, plus the user's request to follow `brief.md`. No project rule, contract, memory or other document reached me, and I read no other file.
`````

### t2-z-1

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**The task:** write `main.hero`. It holds the four sentences and counts the words of each on its own thread, with all four threads running at once. It prints the four counts in sentence order, then their sum. The report must also say whether `heroes build main.hero` accepts it and what it prints.

**The main gap:** the spec says threads come from the language's `hero_os.h` header. It says that one `i64` goes into a thread and one comes back. It never names any function, type or signature in `hero_os.h`. So the `extern` group below is my guess at that header. clang checks every declaration in it against the real header, so this group decides whether the program compiles.

Since only one `i64` reaches a thread, each thread gets the sentence's index. The thread function reads the sentence from a constant.

### `main.hero`

```
# The four sentences, in order.
constant SENTENCES: [str]
    ["the quick brown fox", "jumps over", "the lazy dog sleeps all day", "and wakes"]

extern "hero_os.h"
    record Thread tag hero_thread
    function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join
    function hero_thread_join(t: Thread consumes) -> i64

# Counts the words of sentence `index`: a word is a run of characters between single spaces.
function count_words(index: i64) -> i64
    s = SENTENCES[index]
    n @= 1
    for i in range(from: 0, to: s.len())
        if s[i] == ' '
            n @ n + 1
    return n

function main()
    t0 = hero_thread_spawn(count_words, 0)
    t1 = hero_thread_spawn(count_words, 1)
    t2 = hero_thread_spawn(count_words, 2)
    t3 = hero_thread_spawn(count_words, 3)
    c0 = hero_thread_join(t0)
    c1 = hero_thread_join(t1)
    c2 = hero_thread_join(t2)
    c3 = hero_thread_join(t3)
    print(c0)
    print(c1)
    print(c2)
    print(c3)
    print(c0 + c1 + c2 + c3)
```

There is no `heroes.toml`, because the program uses no package.

## choice_points

1. **The `hero_os.h` API: names, types and signatures.** The spec gives none of them. I chose `hero_thread_spawn(f, arg) -> Thread acquires hero_thread_join`, `hero_thread_join(t: Thread consumes) -> i64`, and a handle type with C tag `hero_thread`.
   - **Other choice:** any other name or shape, such as `hero_spawn`, `thread_start` or `hero_os_thread_create`. It could also be a join that returns a status and writes the result to an `@result: i64` out-parameter, or a thread handled as an `i64` id rather than a handle.
   - **Effect:** if the real header differs in any name, tag or width, **the compiler refuses the program**, because clang checks every member against the header. I expect this to be the most likely way the program fails. I can't resolve it from the spec.
2. **How join reports its result and any failure.** I chose a join that returns the thread's `i64` directly, with `consumes` and no `when`.
   - **Other choice:** `-> i64 when 0` with an `@result: i64` out-parameter.
   - **Effect:** if the header is really like that, my version is **refused** (wrong types or number of arguments). Going the other way, if I had used the `when 0` form against a header that returns the value, that would also be refused. And with `when 0`, a failed join would leave the handle unended, so the program would **stop with an error when `main` returns**.
3. **Whether the spawn result needs `acquires`, and which disposer it names.** I chose `acquires hero_thread_join`. The spec says that once any `extern` consumes a handle type, every call that hands one back must say which kind it is.
   - **Effect:** leaving the mark off is **refused**. Naming a disposer other than the function that consumes the handle **aborts before C runs** at join.
4. **Whether the group needs `link`.** I named none. The spec says `hero_os.h` is the language's own header, linked "where a group names one."
   - **Effect:** if a library had to be named, the build would **fail at link time**.
5. **How the sentences reach the threads.** I chose a top-level `constant SENTENCES: [str]` that the thread function indexes.
   - **Other choice:** a function `sentence(index: i64) -> str` with a `match` on the index and a `_` arm. That should compile just as well.
   - **Effect:** if a constant may not have an array type (the spec only says the body "computes over literals and other constants"), my version is **refused**. Passing the string itself into the thread is impossible, since only one `i64` crosses, so that would be **refused**.
6. **Four named handles instead of an array of handles filled in a loop.** I chose four names.
   - **Effect:** the array version should also work. But it hands C a loop-variable copy of each array element, and the spec's rules on copies of handles ("handing C a copy made before") make that riskier. At worst it would **stop with an error when run**. I avoided it.
7. **Passing the callback.** I passed the top-level function `count_words` by name, typed `(function(i64) -> i64)`. That is allowed because it has no `@` or `lent` parameter.
   - **Effect:** a function with a `lent` parameter would be **refused** as a value.
8. **Positional or named arguments for `hero_thread_spawn`.** I used positional ones, which is allowed because the two parameter types differ. Named arguments would give the same program.
9. **How words are counted.** I counted spaces plus one, which matches the task's definition for non-empty sentences with single spaces.
   - **Other choice:** counting runs of non-space characters gives the same answers here. Counting spaces alone would **run and give a different answer** (3, 1, 5, 1, 10).
10. **The character literal against a byte.** `s[i]` is a `u8`, so `' '` takes the type `u8` from its context. Comparing against the string `" "` instead would be **refused** as a type mismatch.
11. **Join order.** I spawn all four threads before joining any, so they run at the same time.
    - **Effect:** joining each thread right after spawning it prints the same output, but the threads would run one after another, which breaks the task's requirement.

## prediction

If the guessed `hero_os.h` declarations match the real header, the compiler accepts the program. Everything else follows the spec as I read it. If they don't match (choice points 1 and 2), it is refused at the `extern` group. I can't rule that out from the spec, and I consider it the most likely outcome after acceptance.

When it builds and runs, it prints:

```
4
2
6
2
14
```

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The only other input was the system setup: the working directory, the date, and the fact that I'm a Claude agent with a file-reading tool. No other instruction, rule, contract, memory or document did.
`````

### t3-x-1

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I have no tool that writes files; I could only read. As the brief says to do in that case, the whole report follows.

# report.md

## answer

**Whether it compiles:** I expect `heroes build main.hero` to accept the program, but I'm not certain. One declaration is the main risk: the type I gave the result of C's `signal` (see choice point 2).

**Output when sent `kill -INT` about 2.5 seconds after start:**
```
tick 1
tick 2
tick 3
stopped after 3 ticks
```
The exit code is 0.

**How it works.** The specification has no mutable globals and no closures, so a signal handler can't set a flag that `main` reads. Instead, the program uses a file descriptor as the flag:
- At start it copies stdout onto a fixed descriptor number, `STOP_FD` (100).
- The handler for SIGINT and SIGTERM just closes `STOP_FD`. Calling `close` is safe inside a signal handler.
- After each step, the loop checks whether `STOP_FD` is still open by trying `dup(STOP_FD)`. If that fails, a stop was asked for. The loop has already finished the step and printed its tick, so it prints the summary and returns from `main`.

**`main.hero`**
```
# Prints `tick N` once a second until SIGINT or SIGTERM, then a summary.

extern "unistd.h"
    function sleep(seconds: u32) -> u32
    function close(fildes: i32) -> i32
    function dup(fildes: i32) -> i32
    function dup2(fildes: i32, fildes2: i32) -> i32

extern "signal.h"
    constant SIGINT: i32
    constant SIGTERM: i32
    function signal(sig: i32, handler: (function(i32) -> ())) -> ptr

# The descriptor whose closing tells the loop to stop.
constant STOP_FD: i32
    100

# Signal handler: closing STOP_FD is the whole message.
function on_stop(_: i32)
    _ = close(STOP_FD)

# True once on_stop has closed STOP_FD.
function stop_requested() -> bool
    probe = dup(STOP_FD)
    if probe < 0
        return true
    _ = close(probe)
    return false

function main()
    _ = dup2(fildes: 1, fildes2: STOP_FD)
    _ = signal(SIGINT, on_stop)
    _ = signal(SIGTERM, on_stop)
    n @= 0
    running @= true
    while running
        _ = sleep(1)
        n @ n + 1
        print(f"tick {n}")
        if stop_requested()
            running @ false
    print(f"stopped after {n} ticks")
```

This is the only file. No `heroes.toml` is needed, because the program uses no package.

## choice_points

1. **How the signal reaches the loop.** The specification has no mutable globals and no closures, so a handler can't set a flag directly. I chose to have the handler close a fixed descriptor and have the loop check it with `dup`. The alternatives:
   - A global flag: the compiler refuses it, since there are no mutable globals.
   - Blocking the signals with `sigprocmask` and checking `sigpending`: this needs `sigset_t` as a group `record`. On macOS `sigset_t` is a `__uint32_t`, not a struct, so it is refused there.
   - A handler that does nothing, with the loop judging by `sleep`'s return value: this compiles and runs, but gives a different answer than intended. A signal that arrives outside `sleep`, or one that `sleep` rounds down to 0 seconds left, is missed, and the program keeps ticking.
2. **The result type of `signal`.** The specification says a callback can't be a result. I declared the result `-> ptr` and discard it with `_ =`. The alternatives:
   - Declaring the real handler type `-> (function(i32) -> ())`: refused.
   - Leaving out `->` (void): the clang header check probably refuses it as a mismatch with `sighandler_t`.

   `ptr` against a function pointer may be refused too. This is the one place I'm unsure the build passes.
3. **The handler's parameter.** I named it `_`. Naming it `sig` and not using it is refused as an unused parameter.
4. **The handler's C type.** I used `(function(i32) -> ())`, meaning `int` → `void`. Declaring the parameter `i64` is refused, because a callback's parameters are held to the header's width.
5. **The type of `sleep`'s parameter.** I used `u32`, matching C's `unsigned int`. `i64` is refused. Its `u32` result has to be used or discarded, so I wrote `_ =`; a bare `sleep(1)` line is refused.
6. **Calling `dup2`.** Its two parameters share a type, so I wrote named arguments (`fildes:`, `fildes2:`). Positional arguments are refused.
7. **How the program ends.** It returns from `main`, which I take to mean exit code 0. `exit(code: 0)` is refused, because the specification refuses an exit code written as a number.
8. **Not finishing the interrupted second.** A signal cuts `sleep` short, and I don't sleep the rest of the second before printing the tick. Re-sleeping on the returned remainder would delay `tick 3` to about 3.5 seconds on macOS (which rounds the remainder up), but the printed output would be the same. I read "finishes the step in hand" as "prints its tick".
9. **When the check runs.** It runs after the tick. Checking before `sleep` would end the program without printing the tick for the step that was interrupted, giving `stopped after 2 ticks`, which is a different answer than intended.
10. **Ignoring the result of `dup2`.** If `dup2` failed, `STOP_FD` would never be open, so `stop_requested` would be true at once and the program would stop after `tick 1`, a different answer. I accepted this because `dup2` onto descriptor 100 doesn't fail under normal limits.
11. **Which descriptor to copy.** I copied stdout (1). If stdout were closed, `dup2` would fail, with the effect described in point 10.
12. **The type of the constants.** I declared `SIGINT` and `SIGTERM` as `i32` so they pass to `sig: i32` without conversion. As `i64` they would be refused at the call, since there are no implicit conversions.
13. **How the text is printed.** I used `print(f"tick {n}")`. `print("tick ", n)` prints the same thing. Writing `"tick {n}"` without the `f` is refused.

## prediction

- **Compiler:** I expect it to accept the program. The main risk is point 2 above: if `-> ptr` isn't allowed against `signal`'s function-pointer result, the build is refused there.
- **Run with `kill -INT` at about 2.5 seconds:** `tick 1` appears at about 1 second and `tick 2` at about 2 seconds. The signal then arrives during the third `sleep`, which returns early, and `tick 3` is printed. The check finds `STOP_FD` closed, and the program prints `stopped after 3 ticks` and exits with code 0.
- **`kill -TERM` and Ctrl-C:** they behave the same way. At a terminal, Ctrl-C also echoes `^C`, which comes from the terminal, not the program.

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only other things that reached my context were:
- the system prompt saying I'm a Claude agent on the Claude Agent SDK;
- the working-directory path and the date;
- the user's instruction to follow the brief.

No other rules, contracts, memory files or documents reached me, and I read no other file. My knowledge of C and POSIX (`signal`, `sleep`, `dup`, `dup2`, `close`, and how `sigset_t` differs between macOS and Linux) comes from what I already know, not from any file.
`````

### t3-p-1

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a Read tool here, so I couldn't create `report.md`. As the brief allows in that case, the whole report follows.

# report.md

## answer

**The task:** write `main.hero`. It runs a loop of one-second steps. Each step calls C's `sleep` and then prints `tick N`. When stopped by Ctrl-C, `kill -INT` or `kill -TERM`, it finishes the current step, prints `stopped after N ticks` and exits with code 0. The task also asks whether the compiler accepts the program and what it prints when sent `kill -INT` about 2.5 s after it starts.

**Files:** only one file is needed. The program uses no package, so it has no `heroes.toml` (§1: "a program that uses no package has no catalogue").

### `main.hero`
```
# C's sleep: unsigned int sleep(unsigned int seconds);
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# The language's stop flag for Ctrl-C, SIGINT and SIGTERM.
extern "hero_os.h"
    function hero_stop_catch()
    function hero_stop_asked() -> i64

function main()
    hero_stop_catch()
    n @= 0
    while hero_stop_asked() == 0
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        n @ n + 1
        print(f"tick {n}")
    print(f"stopped after {n} ticks")
```

**How it works:**
- `hero_stop_catch()` turns the three signals into a flag instead of letting them end the program.
- The loop checks the flag only between steps. A step (sleep, then print) therefore always finishes once it has started.
- The outer loop has a condition, so the spec does not count it as ending a path. `main` returns normally, which gives exit code 0.

## choice_points

1. **Which header declares `hero_stop_catch` and `hero_stop_asked`.**
   - The spec names those two functions in the same sentence as `hero_os.h` (threads, processes, files), `hero_net.h` and `hero_time.h`, but never says which header holds them.
   - **Chosen:** `hero_os.h`, since signals and ending the process are process matters.
   - **Other choice:** if they are actually in `hero_time.h` or `hero_net.h`, clang checks the group against the wrong header and the **compiler refuses** the program (undeclared functions).

2. **Whether that group needs `link`.**
   - The spec says "linked where a group names one", which is unclear.
   - **Chosen:** no `link`, treating `hero_os.h` as part of the language runtime.
   - **Other choice:** if a library is required, the build **fails at link time**. If I had added a `link` with a guessed library name that doesn't exist, the build would also fail.

3. **Result type of `hero_stop_catch`.**
   - The spec gives no `->` for it.
   - **Chosen:** no result, meaning `void`. The call stands alone as a statement, as §5 requires for a `()` line.
   - **Other choice:** if C actually returns `int`, clang's result check **refuses** it. Declaring `-> i32` when it is really `void` would also be refused. If it does return a value, the call would also need `_ =`.

4. **Result type of `hero_stop_asked`.**
   - **Chosen:** `i64`, exactly as the spec writes it. A result may be wider than C's, so this works even if C returns `int`.
   - **Other choice:** `i32` would be refused if the header really says something wider. Comparing with `== 0` works either way.

5. **The `sleep` signature.**
   - **Chosen:** the parameter is `u32`, because C says `unsigned int` and parameters must match width and sign. The result is `u32`.
   - **Other choices:**
     - A `u64` or `i32` parameter: the compiler **refuses** it.
     - No result: `void` against `unsigned int`, which clang's result check **refuses**.
   - The result is used rather than discarded, so no `_ =` is needed.

6. **Positional or named argument for `sleep`.**
   - **Chosen:** positional `sleep(1)`. Named arguments are mandatory only when two parameters share a type, and the literal takes the type `u32` from context.
   - **Other choice:** `sleep(seconds: 1)` is allowed by the grammar and should behave the same.

7. **A sleep cut short by the signal.**
   - On POSIX systems, `sleep` returns early, with the seconds still left, when a caught signal arrives.
   - **Chosen:** sleep again for whatever is left, so the step really waits its full second. Tick 3 then prints at about 3.0 s.
   - **Other choice:** a single `sleep(1)`. Tick 3 prints at about 2.5 s; the printed output is the same.

8. **Where the stop flag is checked.**
   - **Chosen:** only at the top of the loop.
   - **Other choice:** checking between the sleep and the print would end in the middle of step 3. It would print `stopped after 2 ticks`, a **different answer** that breaks "finishes the step in hand".

9. **How the program ends.**
   - **Chosen:** return from `main`, as the spec recommends ("finishes what it holds and returns from `main`"), giving exit 0.
   - **Other choices:**
     - `exit(code: 0)`: refused, because "one written as a number is refused".
     - Leaving out `hero_stop_catch()`: the signal **kills** the process (exit 130 for SIGINT, 143 for SIGTERM) and the final line is never printed.

10. **The counter's type.**
    - **Chosen:** `n @= 0` is inferred as `i64`, which `f"..."` formats as `to_str` does.
    - **Other choice:** declaring `u32` would also work. Mixing it with an `i64` would be refused.

## prediction

**Compiler:** I expect `heroes build main.hero` to accept the program and produce a binary. That depends on choice points 1–3: the header, no link library, and a `void` `hero_stop_catch`. If any of those guesses is wrong, the compiler or linker refuses the program and there is no binary.

**Run, with `kill -INT` sent at about 2.5 s:**
- At 1.0 s it prints `tick 1`.
- At 2.0 s it prints `tick 2`.
- At about 2.5 s the signal sets the flag and cuts the third sleep short. The program sleeps again for the second that `sleep` reports left (it rounds up), so `tick 3` appears at about 3.0–3.5 s.
- The flag is then seen and the loop ends.

Output:
```
tick 1
tick 2
tick 3
stopped after 3 ticks
```

The exit code is 0. `kill -TERM` and Ctrl-C at the same moment give the same output.

## context

Only this directory's `brief.md`, `task.md` and `heroes-spec.md` were read. My context also held the general agent/tool instructions (Claude Agent SDK, working directory, date). No other project file, rule, contract, memory or document about Heroes reached it.
`````

### t3-q-1

From `result` in `run.json` (no `report.md`).

`````markdown
I only had a file-reading tool, so I couldn't create `report.md`. As the brief says to do in that case, here is the whole report.

---

# report.md

## answer

**What the task asks:** a program, `main.hero`, that runs one-second steps until it is asked to stop. Each step calls C's `sleep` and then prints `tick N`. When it gets SIGINT or SIGTERM, it finishes the step in hand, prints `stopped after N ticks` and exits with code 0. The task also asks whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 s after it starts.

Only one file is needed. The program uses no package, so it has no `heroes.toml` (§1).

### `main.hero`

```
# C's sleep, from unistd.h: unsigned int sleep(unsigned int seconds)
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# The language's own stop flag: from its first call, SIGINT and SIGTERM
# no longer end the program; it answers 1 once one has come.
extern "hero_os.h"
    function hero_stop_asked() -> i64

function main()
    n @= 0
    while hero_stop_asked() == 0
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        n @ n + 1
        print(f"tick {n}")
    print(f"stopped after {n} ticks")
```

How it works, with the rules it relies on:
- The `while` condition calls `hero_stop_asked()` before the first step. From that call on, SIGINT and SIGTERM no longer kill the process (§13), so a signal arriving during any step only sets the flag.
- A signal cuts `sleep` short, and `sleep` then returns the seconds it did not sleep. The inner loop sleeps that remainder, so the step really waits its full second.
- The tick is counted and printed only after the wait. The flag is checked only between steps, so a step is never cut in half.
- After the loop, `main` returns normally, which gives exit code 0. The spec says a loop that "asks between its steps finishes what it holds and returns from `main`" (§13).
- Types: `n` is `i64`, because a bare literal defaults to `i64` (§2). `left` is `u32`, taken from `sleep`'s result. In `sleep(1)` and `left > 0` the literals take the `u32` the context asks for.
- Every cell is re-bound (`n @ …`, `left @ …`), and every value is used, so no unused-binding errors (§5).

## choice_points

1. **Which header declares `hero_stop_asked`.** §13 names the function but not its header. It lists "threads, processes and files from … `hero_os.h`" and "sockets and clocks from … `hero_net.h` and `hero_time.h`". Signal handling is about the process, so I chose `hero_os.h`.
   - *If it is declared elsewhere* (say `hero_time.h`), clang finds no declaration in `hero_os.h` and **the compiler refuses the program**. Then only the `extern` line needs changing.

2. **Whether that group needs `link`.** I wrote no `link`. I read "linked where a group names one" as: the language's own headers are linked automatically once a group names them.
   - *If a `link "…"` is actually needed*, the build fails at the link step (**the compiler refuses it**). Adding a `link` with a made-up library name would also be refused.

3. **When to call `hero_stop_asked` for the first time.** I call it in the loop condition, before the first `sleep`, so the handler is in place from the start.
   - *If it were checked only after a step* (for example at the bottom of a `while true` loop), a Ctrl-C during the first second would arrive before the first call. That **ends the program by the signal** without the final line and with a non-zero exit (130 or 143), which is a different answer than intended.

4. **Sleeping again after an interrupted `sleep`.** I re-sleep the returned remainder in `while left > 0`.
   - *The alternative* is to ignore the result (`_ = sleep(1)`). That compiles and prints exactly the same lines. But the interrupted step then waits only about 0.5 s before printing `tick 3`, so the step is cut short in time.
   - **A caveat on timing:** `sleep` returns whole seconds, and whether a 0.5 s remainder comes back as 0 or 1 depends on the C library. So `tick 3` prints at about 2.5 s (remainder 0) or about 3.5 s (remainder 1). The printed text is the same either way.

5. **How to end with code 0.** I return normally from `main`.
   - *The alternative,* `exit(code: 0)`, is **refused by the compiler**: §11 says "one written as a number is refused".

6. **How `sleep` is declared.** The parameter is `u32`, because §13 says a parameter must be declared at the header's own width and sign, and C's is `unsigned int`.
   - *A parameter of `i64`*, or of `u64` (wider than C's 32 bits and not exactly convertible), is **refused**.
   - For the result, I chose `u32`. §13 also allows a result wider than C's, so `u64` would be accepted too, and the literal `0` in `left > 0` would then simply be a `u64`. Same behaviour.

7. **How to format the output.** I used `print(f"tick {n}")`. `print("tick ", n)` gives identical output, since `print` puts no separator between values (§11).
   - *A plain `"tick {n}"` without the `f`* is a **compile error** (§2), because `n` is in scope there.

8. **Naming the argument.** I wrote `sleep(1)` without a label. Named arguments are mandatory only when two parameters share a type (§9), and `sleep` has one parameter. `sleep(seconds: 1)` would also be accepted, so this choice makes no difference.

## prediction

**Compiler:** I expect `heroes build main.hero` to accept the program and produce a binary. That depends on choice points 1 and 2 (`hero_stop_asked` declared in `hero_os.h`, linked automatically). If either is wrong, the build is refused, at checking or at linking.

**Output with `kill -INT` at about 2.5 s:**

```
tick 1
tick 2
tick 3
stopped after 3 ticks
```

- `tick 1` prints at about 1 s and `tick 2` at about 2 s.
- The signal arrives during the third `sleep`, at about 2.5 s. That step finishes: it re-sleeps any reported remainder, then prints `tick 3` at about 2.5 s or 3.5 s (see choice point 4).
- The next condition check sees 1, so the loop ends. The program prints `stopped after 3 ticks` and exits with code 0.
- Ctrl-C or `kill -TERM` at the same moment gives the same result.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached me. No other project file, rule, contract, memory or document did. The one exception is my agent harness's system prompt. It said I am a Claude agent built on the Claude Agent SDK and gave the working directory path and the date. It contained nothing about Heroes.
`````

### t3-r-1

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a file-reading tool, so I couldn't create `report.md`. As the brief says to do in that case, the whole report is below.

---

# report.md

## answer

**Does the compiler accept it?** I expect `heroes build main.hero` to accept the program and build a binary.

**What it prints when sent `kill -INT` about 2.5 seconds after it starts:**
```
tick 1
tick 2
tick 3
stopped after 3 ticks
```
It then ends with exit code 0. The signal arrives during step 3's wait. Step 3 finishes and prints `tick 3`. The loop then sees the stop request and prints the final line.

There is only one file.

### `main.hero`
```
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# One step: wait a full second, even if a signal cuts C's sleep short.
function wait_one_second()
    left @= sleep(seconds: 1)
    while left > 0
        left @ sleep(seconds: left)

function main()
    n @= 0
    while !stop_asked()
        wait_one_second()
        n @ n + 1
        print(f"tick {n}")
    print(f"stopped after {n} ticks")
```

## choice_points

1. **Type of `sleep`'s parameter.** C declares it `unsigned int seconds`. Section 13 says a parameter must have the header's own width and sign, so I chose `u32`. Writing `i64`, `u64` or `i32` gives a program **the compiler refuses**. (`i32` against unsigned is not a conversion C makes exactly.)

2. **Type of `sleep`'s result.** C returns `unsigned int`. I chose `u32`. A wider type such as `u64` is also allowed ("a result may be wider than C's"), and the program would behave the same. Leaving out the `->`, which declares `void`, would fail clang's check against the header: **the compiler refuses**.

3. **Using `sleep`'s result.** I bind it to `left` and loop until it reaches 0.
   - A bare `sleep(seconds: 1)` line computes a value and doesn't use it: **the compiler refuses**.
   - `_ = sleep(seconds: 1)` would compile. A signal interrupts the sleep, so step 3 would then print `tick 3` at about 2.5 s instead of 3.0 s. The output text is the same, only the timing changes.
   - I chose the loop so that a step really waits one second, as the task says.

4. **Named or positional argument.** With only one parameter, the label isn't required. I wrote `seconds:` for clarity. `sleep(1)` would compile the same way, because the literal takes `u32` from the parameter.

5. **The `extern` group without `link`.** `sleep` is in libc, so I named no library. Adding a `link` for a library that doesn't exist would fail when the program links.

6. **Where `stop_asked()` is first called.** It is called in the `while` condition, before the first step. That matters because only its first call turns off the default "end the program" response to a signal. If I asked only after the first sleep, an early SIGINT or SIGTERM would kill the program mid-step: **it stops with an error/signal when run**, prints no final line and returns a non-zero status.

7. **How the program ends with code 0.** It returns from `main`. Writing `exit(code: 0)` gives a program **the compiler refuses**, because section 11 says an exit code written as a number is refused. I am assuming that returning from `main` gives exit code 0. The spec says that is how a `stop_asked` loop ends, but it never says the status is 0.

8. **Output formatting.**
   - I used `f"tick {n}"`. The same text without the `f` would be **refused by the compiler**, because its braces name the in-scope `n`.
   - `print("tick ", n)` would have worked too, since `print` adds no separator.

9. **Type of the counter.** `n @= 0` makes `n` an `i64`, the default for a literal, and `n @ n + 1` keeps that type. No choice affects the output here.

10. **Shape of the loop.** I used `while !stop_asked()` rather than `while true` with `if stop_asked()` / `break`. Both are equivalent, provided the check comes before each step (see point 6).

## prediction

- **Compiler:** I expect it to accept `main.hero`. `sleep` is checked against `unistd.h` as `u32 -> u32`. Every cell is re-bound, there are no unused bindings, and no `T?` is dropped.
- **Run with `kill -INT` at about 2.5 s:**
  - `tick 1` at about 1 s and `tick 2` at about 2 s.
  - At 2.5 s, SIGINT interrupts step 3's `sleep`. If the runtime's handler makes `sleep` return early, the inner loop sleeps the rest (1 s, rounded). Either way, `tick 3` comes at about 3.0–3.5 s.
  - The loop then checks `stop_asked()`, gets true and prints `stopped after 3 ticks`. `main` returns with exit code 0.
- **With `kill -TERM`** the result is the same. **With Ctrl-C** it is the same, except the terminal may also echo `^C`.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. I read no other file and got no project document, rule or memory. My general knowledge of C (POSIX `sleep` returning the seconds it didn't sleep, and how it behaves when a signal interrupts it) came from training, not from any file.
`````

### t3-r-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I only have a file-reading tool here. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

**What the task asks:** write `main.hero`, a program that loops through steps: sleep one second using C's `sleep`, then print `tick N`. When it gets SIGINT or SIGTERM, it finishes the current step, prints `stopped after N ticks` and exits with code 0. The report must also say whether `heroes build main.hero` accepts it and what it prints when sent `kill -INT` about 2.5 s after it starts.

There is one file. It uses no package, so there is no `heroes.toml`.

**`main.hero`**
```
# C's sleep: waits that many seconds, or less when a signal arrives,
# and returns the seconds it did not sleep.
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# Ticks once a second until a stop is asked, finishing the step in hand.
function main()
    ticks @= 0
    while !stop_asked()
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        ticks @ ticks + 1
        print(f"tick {ticks}")
    print(f"stopped after {ticks} ticks")
```

How it works:
- **First `stop_asked()` call:** the `while` condition calls `stop_asked()` before the first step. The spec says that from this first call, a SIGINT or SIGTERM no longer ends the program; it only makes later calls return `true`.
- **When a signal arrives during `sleep`:** C's `sleep` stops early and returns the seconds it did not sleep. The inner loop sleeps that remainder, so the step still waits its full second.
- **End of the step:** the step then prints its tick. The loop condition sees the stop, the program prints the summary and returns from `main`.

## choice_points

1. **The width of `sleep`'s parameter.** I chose `u32`, because C declares `unsigned int`. The spec requires a parameter to match the header's width and sign.
   - With `i32` or `i64`, the compiler refuses the program: the sign or width disagrees, and C does not convert it exactly.
2. **The type of `sleep`'s result.** I chose `u32`.
   - `i64` should also be accepted, because "a result may be wider than C's".
   - A signed result narrower than C's, such as `i32` or `i16`, would be refused when clang checks it against the header.
   - Declaring no result (`()`) would claim `void` against `unsigned int`. I expect clang's check to refuse it.
3. **Using the result.** I read the result as the remainder to sleep.
   - The other option was to discard it with `_ = sleep(1)`, which is allowed because `u32` is not fallible. That compiles and prints the same lines. The only difference is that the interrupted step ends early (tick 3 at about 2.5 s instead of about 3 s), so the step isn't really "finished".
   - Writing `sleep(1)` alone on a line, without using its value, is refused by the compiler: a line that computes a value must use it.
4. **Where `stop_asked()` is first called.** I call it in the loop condition, before the first step.
   - If it were only checked after a step (a do-while shape), a signal arriving before that first call would still kill the program. That means a stop during the first second ends the run with no `stopped` line and a nonzero (signal) status.
   - At 2.5 s both versions behave the same.
5. **How the program ends.** It returns from `main`, as §11 describes.
   - `exit(code: 0)` is refused by the compiler, because §11 says an exit code "written as a number is refused".
   - Note: the spec says the loop "returns from `main`" but never says outright that returning from `main` gives exit code 0. I'm assuming it does.
6. **How the arguments are passed.** I wrote `sleep(1)` with the argument by position.
   - `sleep(seconds: 1)` is allowed by the grammar. Named arguments are only required when two parameters share a type, so it should behave the same.
7. **The `extern` header line.** I wrote it with no `link`, because `sleep` is in libc.
   - Adding `link "c"` would most likely compile and behave the same.
8. **The type of `ticks`.** I let it be inferred: the literal `0` makes it `i64`.
   - Writing `ticks: i64 @= 0` is the same program.
9. **How the lines are printed.** I used an f-string.
   - `print("tick ", ticks)` prints the same text, because `print` adds no separator.
10. **Where the `extern` group goes.** I put it before `main`. Declaration order doesn't matter, so putting it after `main` is the same program.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program and produce a binary.
- **Output with `kill -INT` at about 2.5 s:**
```
tick 1
tick 2
tick 3
stopped after 3 ticks
```
- **Exit code:** 0.
- **Timing:** tick 1 at about 1 s and tick 2 at about 2 s. The signal interrupts the third `sleep` at about 2.5 s. `sleep` returns 0 or 1 depending on how libc rounds the unslept half-second, so tick 3 prints at about 2.5 s or about 3.5 s. The stop line follows right away.
- **Other ways to stop it:** `kill -TERM` gives the same output. Ctrl-C at a terminal does too, except the terminal may also echo `^C`.

## context

No other files reached my context. The only text besides `brief.md`, `task.md` and `heroes-spec.md` was the agent's system prompt: it says I'm a Claude agent built on the Agent SDK and gives the working directory and date. It contains no rules, contracts, memory or documents about Heroes.
`````

### t3-q-2

From `result` in `run.json` (no `report.md`).

`````markdown
I can't write `report.md`: the only tool I have here reads files and cannot create them. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for a program, `main.hero`, that:
- runs steps until it is told to stop;
- in each step, waits one second with C's `sleep` and then prints `tick N`;
- on SIGINT or SIGTERM, finishes the step it is in, prints `stopped after N ticks` and exits with code 0.

It also asks whether `heroes build main.hero` accepts the program and what it prints when sent `kill -INT` about 2.5 s after it starts.

The program is one file. It uses no package, so it has no `heroes.toml`.

### `main.hero`

```
# Ticks once a second until asked to stop, then reports how many ticks it printed.

extern "unistd.h"
    function sleep(seconds: u32) -> u32

extern "hero_os.h"
    function hero_stop_asked() -> i64

function main()
    n @= 0
    while hero_stop_asked() == 0
        left @= sleep(1)
        while left > 0
            left @ sleep(left)
        n @ n + 1
        print(f"tick {n}")
    print(f"stopped after {n} ticks")
```

How it works:
- **First call to `hero_stop_asked()`:** this happens in the loop condition, before step 1. From that call on, SIGINT and SIGTERM no longer end the process; they only make `hero_stop_asked()` return 1 (§13).
- **Checking for a stop:** the loop checks only between steps, so a step is never cut short.
- **A signal during `sleep`:** it can make `sleep` return early with the seconds still left. The inner loop sleeps through what remains, so each step still waits its full second before printing.
- **Exit code:** `main` returns normally, which gives exit code 0. It does not call `exit(code: 0)`, because §11 refuses an exit code written as a number.

## choice_points

1. **Which header declares `hero_stop_asked`.** The spec gives its signature but not its header.
   - **Chosen:** `hero_os.h`, because §13 says "processes" come from that header.
   - **Other choices:** `hero_time.h` or `hero_net.h`. If the function isn't declared there, clang's check of the group fails and **the compiler refuses the program**.
   - **Risk:** if it really lives in one of those, my choice is the one that is refused.

2. **Type of `sleep`'s parameter.** C declares `unsigned int seconds`.
   - **Chosen:** `u32`, the header's exact width and sign.
   - **Other choices:** `i32` or `i64` disagree in sign or width, and C doesn't convert them exactly, so **the compiler refuses the program** (§13).

3. **Type of `sleep`'s result.** C returns `unsigned int`.
   - **Chosen:** `u32`.
   - **Other choices:** `i64` is wider, which §13 allows. It also compiles, but then `left > 0` and `sleep(left)` would need a `to_u32(left).must()` conversion. `i32` or `u16` is narrower or has the wrong sign: **refused**.

4. **What to do with `sleep`'s result.**
   - **Chosen:** bind it and use it to finish the second.
   - **Other choices:**
     - A bare `sleep(1)` line leaves a value unused: **refused** (§5).
     - `_ = sleep(1)` compiles. The output is the same, but tick 3 would print at about 2.5 s instead of 3.0 s, because the signal cuts that step's wait short. That still meets "finishes the step in hand", but the step would not last a full second.

5. **When `hero_stop_asked()` is first called.**
   - **Chosen:** in the loop condition, before step 1 starts.
   - **Other choice:** checking only after each step. A Ctrl-C during step 1 would then still use the default action, killing the process by signal. The **program would end with no `stopped` line and a non-zero status (130 or 143)**.

6. **How the program ends.**
   - **Chosen:** return from `main`.
   - **Other choice:** `exit(code: 0)` is **refused**, because §11 refuses an exit code written as a number.

7. **Type of the counter `n`.**
   - **Chosen:** `n @= 0`, which makes it an `i64` (§2).
   - **Other choice:** `n: u64 @= 0` also compiles and prints the same.

8. **How the lines are formatted.**
   - **Chosen:** `f"tick {n}"`.
   - **Other choices:**
     - `print("tick ", n)` prints the same, since `print` adds no separator.
     - `"tick {n}"` without the `f` is **refused**: braces that would name `n` are an error in a plain literal (§2).

9. **Whether to add `link` to either group.**
   - **Chosen:** none. `sleep` is in libc, and §13 says the platform layer is linked when a group names its header.
   - **Other choice:** adding `link "c"` would probably also be accepted.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program and produce a binary. The main risk is choice 1: if `hero_stop_asked` isn't declared in `hero_os.h`, the program is refused.
- **Run, with `kill -INT` at about 2.5 s:**
  - The first `hero_stop_asked()` returns 0.
  - `tick 1` prints at about 1 s and `tick 2` at about 2 s.
  - At 2.5 s the signal interrupts step 3's `sleep`, which returns early. The inner loop sleeps the rest of the second.
  - `tick 3` prints at about 3 s. The next check returns 1, so the loop ends.
  - Output:
    ```
    tick 1
    tick 2
    tick 3
    stopped after 3 ticks
    ```
  - Exit code: 0.
- **Ctrl-C or `kill -TERM` at the same time:** the output and exit code are the same.

## context

Only the files in this directory reached my context: `brief.md`, `task.md` and `heroes-spec.md`. No other instruction file, project rule, contract, memory or document was loaded. The only other text I had was the system prompt, which gave the working directory and the date.
`````

### t3-p-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't save `report.md` because I only have a file-reading tool here, so the full report is below. Following the brief's fallback, it is my final answer. Nothing was compiled or run; the predictions are reasoned from the spec.

# report.md

## answer

The task asks for a program that prints a tick once a second and stops cleanly when interrupted. It also asks whether the compiler accepts it and what it prints when sent `kill -INT` about 2.5 seconds after it starts.

**Short answer:** I expect `heroes build main.hero` to accept the program with no other files and no `heroes.toml`. When sent `kill -INT` at about 2.5 s, it prints:

```
tick 1
tick 2
tick 3
stopped after 3 ticks
```

It then exits with code 0.

### `main.hero`

```
# sleep from C: unsigned int sleep(unsigned int seconds)
extern "unistd.h"
    function sleep(seconds: u32) -> u32

# The language's own stop flag: Ctrl-C, SIGINT and SIGTERM set it instead of ending the program
extern "hero_os.h"
    function hero_stop_catch()
    function hero_stop_asked() -> i64

function main()
    hero_stop_catch()
    n @= 0
    while hero_stop_asked() == 0
        _ = sleep(1)
        n @ n + 1
        print(f"tick {n}")
    print(f"stopped after {n} ticks")
```

There are no other files. The program uses no package, so it needs no catalogue.

How it works:
- `hero_stop_catch()` runs first, so a stop request sets a flag instead of killing the process.
- The loop checks the flag only between steps. Each step sleeps, counts and prints `tick N`.
- After a stop request, the step in progress finishes and the loop ends. The program then prints `stopped after N ticks` and returns from `main`, which gives exit code 0.

## choice_points

1. **Which header declares `hero_stop_catch` and `hero_stop_asked`.** Section 13 names `hero_os.h` (threads, processes and files), `hero_net.h` (sockets) and `hero_time.h` (clocks). It describes the stop functions in the same sentence but doesn't say which header holds them.
   - **My choice:** `hero_os.h`, since signals belong with processes.
   - **The other choice:** naming `hero_time.h` or `hero_net.h` would make clang's header check reject the program because the symbols wouldn't be declared there.

2. **No `link` clause on the `hero_os.h` group.** The spec says the platform layer is "linked where a group names one", which I read as: naming the header is enough.
   - **My choice:** no `link`.
   - **The other choice:** adding `link "hero_os"` or similar might fail at link time if no such library exists by that name.

3. **What `hero_stop_catch` returns.** The spec writes `hero_stop_catch()` with no result type.
   - **My choice:** no `->`, meaning `void`.
   - **The other choice:** if the real header returns `int`, clang's result-type check would reject `void`, and the fix would be `-> i32` or wider with `_ = hero_stop_catch()`. The reverse error is also possible: declaring a result when C's is `void` would be rejected.

4. **The type of `hero_stop_asked`'s result.** The spec gives `-> i64`, and a result may be wider than C's, so I used `i64`. Since a narrower type would only be safe if it matched C exactly, I kept the spec's type.

5. **The types in `sleep`'s declaration.** C's parameter is `unsigned int`, so it must be declared `u32`.
   - **The other choice:** `i64` or `i32` would be rejected, because a parameter must match C's width and sign.
   - For the result I chose `u32`. `u64` or `i64` would also be accepted, since a result may be wider.

6. **The parameter name `seconds`.** I used C's own name. With a single parameter, named arguments aren't required, so `sleep(1)` works. Another name should make no difference.

7. **The unused result of `sleep`.** Every computed value must be used.
   - **My choice:** `_ = sleep(1)`, which is allowed because the value isn't fallible.
   - **The other choice:** a bare `sleep(1)` would be rejected by the compiler.

8. **Whether a wait cut short by the signal should be resumed.** A signal cuts `sleep` short. I considered calling `sleep` again for the seconds it reports as left, until it returns 0.
   - **My choice:** don't resume. The step finishes by printing its tick right away.
   - **The other choice:** resuming would print the same output, but `tick 3` would come later, at about 3.0 to 3.5 s, because `sleep` reports the time left in whole, rounded seconds. It would also add a cell that could look wrong to a reviewer.

9. **Where to check the flag.** I check it only in the `while` condition, between steps. That makes "finish the step in hand" automatic.
   - **The other choice:** also checking between the `sleep` and the `print` would stop without printing `tick 3`. That ends mid-step and breaks the task.

10. **The type of the counter.** `n @= 0` makes `n` an `i64`, and `f"tick {n}"` prints it the way `to_str` does. No conversion is needed. `n` is changed in the loop, so the "cell nothing re-binds" rule is satisfied.

11. **How the program ends.** I return from `main` instead of calling `exit(code: 0)`. Both give exit code 0. `exit(code: 0)` would also be legal, since `0` isn't refused here; only an exit code written as a number in some other way is refused.

## prediction

- **Compiler:** I expect `heroes build main.hero` to accept the program. The main risks are choice points 1 and 3: which header holds the stop functions and the exact C type of `hero_stop_catch`.
- **Run, `kill -INT` at about 2.5 s:**
  - `tick 1` prints at about 1 s.
  - `tick 2` prints at about 2 s.
  - Step 3 is in its `sleep` when the signal arrives. The handler sets the flag, and on Linux and macOS a caught signal makes `sleep` return early, at about 2.5 s. The step then finishes and prints `tick 3`.
  - The loop condition sees the flag, the program prints `stopped after 3 ticks`, and `main` returns.
  - If the runtime instead blocks the signal so that `sleep` isn't interrupted, `tick 3` prints at 3.0 s. The output is the same either way.

  ```
  tick 1
  tick 2
  tick 3
  stopped after 3 ticks
  ```

  Exit code 0.
- **`kill -TERM` or Ctrl-C:** these behave the same way.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context, plus the general system prompt describing me as a Claude agent and my working directory. I read no other instruction, rule, contract, memory or document.
`````

### t3-x-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the whole report is below as the brief allows.

# Report

## answer

The task asks for `main.hero`. The program keeps a count of steps. Each step calls C's `sleep(1)` and then prints `tick N`. A request to stop (Ctrl-C, `kill -INT` or `kill -TERM`) must not cut a step short: the program finishes the step, prints `stopped after N ticks` and exits with code 0.

**How it works:** The specification has no mutable globals and no anonymous functions. It also gives no way to write C's `signal()` result, which is a function pointer: callbacks may only be parameters, never results. So a signal handler that sets a flag can't be written. Instead, the program blocks SIGINT and SIGTERM with `sigprocmask`. A blocked signal never kills the process and never cuts `sleep` short; it just stays pending. After each tick the program asks `sigpending` whether either signal is waiting. If one is, it leaves the loop, prints the summary and returns from `main`, which gives exit code 0.

No package is used, so there is no `heroes.toml`. The one file:

### `main.hero`
```
# POSIX signal masking: SIGINT and SIGTERM are blocked so they stay pending
# instead of ending the process; the loop polls for them between steps.
extern "signal.h"
    constant SIG_BLOCK: i32
    constant SIGINT: i32
    constant SIGTERM: i32
    record sigset_t partial
    function sigemptyset(@set: sigset_t) -> i32
    function sigaddset(@set: sigset_t, signo: i32) -> i32
    function sigprocmask(how: i32, set: sigset_t lent, @oldset: sigset_t) -> i32
    function sigpending(@set: sigset_t) -> i32
    function sigismember(set: sigset_t lent, signo: i32) -> i32

extern "unistd.h"
    function sleep(seconds: u32) -> u32

# True when SIGINT or SIGTERM is waiting to be delivered.
function stop_requested() -> bool
    pending @= sigset_t(rest: zero)
    assert sigpending(@pending) == 0
    return (sigismember(set: pending, signo: SIGINT) == 1
        || sigismember(set: pending, signo: SIGTERM) == 1)

function main()
    mask @= sigset_t(rest: zero)
    assert sigemptyset(@mask) == 0
    assert sigaddset(@mask, signo: SIGINT) == 0
    assert sigaddset(@mask, signo: SIGTERM) == 0
    old @= sigset_t(rest: zero)
    assert sigprocmask(how: SIG_BLOCK, set: mask, oldset: @old) == 0
    n @= 0
    while true
        _ = sleep(1)
        n @ n + 1
        print(f"tick {n}")
        if stop_requested()
            break
    print(f"stopped after {n} ticks")
```

## choice_points

1. **How to catch the signal: a blocked mask polled with `sigpending` (chosen), or a handler installed with `signal`/`sigaction`.**
   - **Handler with `signal`:** its result is `void (*)(int)`. The spec says a callback is "a parameter, never a result", and a header-checked result can't be written as `ptr`, so the compiler refuses it.
   - **Handler with `sigaction`:** `sa_handler` would be a function-typed field, which the spec doesn't allow, so the compiler refuses it.
   - **Even if a handler were accepted:** it has no mutable global to set, so the loop would never learn about the signal. The default action would then kill the process mid-step (no summary line, exit code 130 or 143), or the signal would be lost and the program would run forever.

2. **Platform for `sigset_t`: Linux/glibc (chosen), where it is a struct, declared `record sigset_t partial` with no fields.**
   - On macOS `sigset_t` is `__uint32_t`. clang's check against the header refuses a record there, so this file does not compile on macOS.
   - A macOS version would declare `@set: u32` (one element, per the out-parameter rule) and `set: u32 lent`. But macOS also defines `sigemptyset`, `sigaddset` and `sigismember` as macros, and the spec doesn't say whether an `extern` function that is also a macro compiles. That version could be refused, or would have to set the bits itself (`1 << (SIGINT-1)`, which is non-portable).
   - The spec mentions `hero_os.h` for processes but doesn't document what it contains, so I couldn't rely on it for signals.

3. **A `partial` record with zero fields (chosen), or naming glibc's field `__val: u64[16]`.**
   - The grammar allows `record ident partial NEWLINE`. If the compiler requires at least one field, it refuses this.
   - Naming `__val` is exact on 64-bit glibc, but on a 32-bit `unsigned long` the field check refuses it.

4. **Building the set with `sigset_t(rest: zero)` and naming no fields (chosen).** The other option, building it with no arguments, is not allowed for a group's record, so the compiler would refuse it.

5. **`@` parameters without `lent` (chosen, like the spec's `@out: Db`).** Adding `lent` would probably also compile. Leaving `@` off `sigemptyset`, `sigaddset` and `sigpending` would mean C writes into a copy: the mask would never contain the signals, so the first Ctrl-C would kill the process mid-step.

6. **Naming all three arguments to `sigprocmask` (chosen).** `set` and `oldset` share the type `sigset_t`, so names are mandatory there. Calling it by position would be refused.

7. **A throwaway `old` cell for `oldset` (chosen), or `nullptr`.** `nullptr` isn't a `sigset_t` place, so the compiler would refuse it. `old` counts as used because it is written through an `@` argument.

8. **Constants typed `i32` (chosen), or `i64`.** With `i64`, passing them to the `int` parameters would need `to_i32(...).must()`. Without the conversion the compiler refuses the call, because there are no implicit conversions.

9. **`sleep(seconds: u32) -> u32` with the result dropped by `_ =` (chosen).** Leaving out `_ =` is refused (a value must be used). Declaring the parameter as `i32` or `u64` is refused (C says `unsigned int`).

10. **Checking for a stop after printing the tick (chosen), or before sleeping.** Checking before the sleep would also be correct, but it can't replace the check after the tick. Without that check, a signal arriving during step N would still produce tick N+1 before the stop, so the program would run an extra step and the count would be one higher.

11. **Ending by `break` and returning from `main` (chosen), or `exit(code: 0)`.** The spec says an `exit` code "written as a number is refused", so `exit(code: 0)` may be a compile error. Returning from `main` gives code 0 and avoids the question.

12. **Checking C results with `assert` (chosen), or with `match`/`exit`.** If a call failed, `assert` aborts the program with a non-zero code. Ignoring the result with `_ =` would let the program carry on with no mask, so a signal would kill it mid-step.

13. **The long `||` condition is wrapped in parentheses**, because outside brackets a line break ends the statement. Without the parentheses the compiler refuses it.

## prediction

- **Build:** on Linux/glibc I expect `heroes build main.hero` to accept the file. The risks are the zero-field `partial` record and whether `assert` is allowed outside tests. On macOS I expect it to be refused, because `sigset_t` is an integer there.
- **Run, with `kill -INT` about 2.5 s after start:** the signal arrives during the third `sleep`. Because it is blocked, it doesn't cut the sleep short. At about 3.0 s the program prints `tick 3`, sees SIGINT pending, prints the summary and exits with code 0:
```
tick 1
tick 2
tick 3
stopped after 3 ticks
```
- **Ctrl-C and `kill -TERM`:** both behave the same way.
- **Edge case:** a signal sent in the microseconds before `sigprocmask` runs would still kill the process.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context, plus the system prompt (the agent role, the working directory and the date). No other instruction, rule, contract, memory or project document reached it. What I said about glibc's and macOS's `<signal.h>` comes from my general knowledge, not from any file.
`````

### t2-z-2

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a Read tool, so I couldn't write `report.md`. As the brief allows, here is the whole report.

# report.md

## answer

The task asks for `main.hero`. It holds four sentences, counts the words of each on its own thread with all four threads running at once, prints the four counts in order and then their sum.

The specification says threads come from the language's `hero_os.h` (section 13). It also says only one `i64` goes into a thread and one comes back. But it never shows that header's functions, types or names. So the `extern` group below is my guess at that header. That guess is the program's biggest risk.

Here is how the program follows the one-`i64` rule. Each thread gets only the sentence's index (an `i64`). It looks the sentence up itself and returns the word count (an `i64`). There are no mutable globals, so no other state crosses.

### `main.hero`

```
extern "hero_os.h"
    record Thread tag hero_thread
    function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join
    function hero_thread_join(t: Thread consumes) -> i64

# The sentence at index `i`, 0 to 3.
function sentence(i: i64) -> str
    sentences = ["the quick brown fox", "jumps over", "the lazy dog sleeps all day", "and wakes"]
    return sentences[i]

# The words of sentence `i`: runs of characters between single spaces.
function count_words(i: i64) -> i64
    s = sentence(i)
    if s.len() == 0
        return 0
    n @= 1
    for k in range(from: 0, to: s.len())
        if s[k] == ' '
            n @ n + 1
    return n

function main()
    t0 = hero_thread_spawn(count_words, 0)
    t1 = hero_thread_spawn(count_words, 1)
    t2 = hero_thread_spawn(count_words, 2)
    t3 = hero_thread_spawn(count_words, 3)
    c0 = hero_thread_join(t0)
    c1 = hero_thread_join(t1)
    c2 = hero_thread_join(t2)
    c3 = hero_thread_join(t3)
    print(c0)
    print(c1)
    print(c2)
    print(c3)
    print(c0 + c1 + c2 + c3)
```

No other file is needed. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **The thread API in `hero_os.h`: names, signatures and handle tag.** The spec says threads come from `hero_os.h` but never names anything in it. I guessed `hero_thread_spawn(f, arg) -> Thread` (a handle with tag `hero_thread`) and `hero_thread_join(t) -> i64`. Clang checks every declaration against the header, so if the real names, tag or types differ, **the compiler refuses the program**. This is the only point where the spec gives no answer, and I expect it to decide the outcome.
2. **How the thread entry is declared.** I made it a callback parameter of type `(function(i64) -> i64)`, which fits "one `i64` in and back". If the header instead takes `ptr` for the entry or argument, or returns the result through an `@` out-parameter, the program is **refused**.
3. **Lifetime marks on the handle.** I wrote `acquires hero_thread_join` on spawn and `consumes` on join, with no `when`. The alternatives:
   - Leaving out `acquires`/`consumes` loses the lifetime tracking, and the compiler may refuse it if other externs consume the type.
   - If the real join returns a status and needs `when 0`, my version would be **refused**, or would misread a status as a count.
   - If I forgot to join one thread, the program would **abort when `main` returns** ("a handle nobody consumes").
4. **Four named handles rather than an array plus a loop.** With `for t in threads` and `hero_thread_join(t)`, I'm not sure whether a copied element counts as "borrowed" or as "a copy made before". That could be **refused** or **abort**. Named bindings avoid the question.
5. **Spawn all four, then join.** Joining each thread right after spawning it would still print the right counts, but the threads would run one after another, which breaks the task's requirement that they run at the same time.
6. **Passing the sentence.** Only an `i64` may cross, so I pass an index and look it up in a local array. Passing the `str` itself would be **refused**. A `constant SENTENCES: [str]` would also work if the spec allows a constant to be an array (it doesn't say). A `match` on `i` would work too.
7. **Counting words.** I count spaces plus one, and return 0 for an empty string. That is correct under the task's rule of single spaces. If the input had doubled spaces, the result would differ from a "non-empty runs" count, but this input has none.
8. **Types of `s.len()` and `s[k] == ' '`.** I assumed `len` returns `i64`, so it fits `range(from:, to:)`. `s[k]` is a `u8`, and the literal `' '` takes `u8` from that context. If `len` returned `u64`, the call would be **refused**, and I would need `to_i64(...)`, which returns an `i64?` and so needs `.must()`.
9. **Named arguments on `range`.** Both of `range`'s parameters are `i64`, so names are required. Writing `range(0, s.len())` would be **refused**. `hero_thread_spawn`'s two parameters have different types, so passing them by position is allowed.

## prediction

If `hero_os.h` really declares what I guessed in points 1 and 2, the compiler accepts `heroes build main.hero`, and running the program prints:

```
4
2
6
2
14
```

But nothing in the spec confirms those names. I expect it is **more likely than not that the compiler refuses the program** at the `extern` group, because a header declaration won't match. Everything below the `extern` group should type-check under the spec as written.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only instructions that reached me were the system prompt and the user's message telling me to follow `brief.md`. No other project files, rules, memory or documents reached me.
`````

### t2-y-2

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a file-reading tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The program holds the four sentences in a constant. `main` starts one thread per sentence and passes each thread the sentence's index, because the spec says only one `i64` can cross into a thread. All four threads start before any of them is waited on, so they run at the same time. Each thread counts the words in its sentence and returns the count. `main` then waits on the threads in sentence order, printing each count as it arrives, and prints the sum last.

The compiler should accept it with `heroes build main.hero`. The program should print `4`, `2`, `6`, `2` and then `14`.

### `main.hero`

```
# The four sentences, in the order their counts are printed.
constant SENTENCES: [str]
    ["the quick brown fox", "jumps over", "the lazy dog sleeps all day", "and wakes"]

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# Words in `s`: runs of characters between single spaces, so one more than its spaces.
function count_words(s: str) -> i64
    count @= 1
    for c in s.chars()
        if c == " "
            count @ count + 1
    return count

# A thread's body: counts the words of the sentence at `index`.
function count_words_at(index: i64) -> i64
    return count_words(SENTENCES[index])

function main()
    handles: [i64] @= []
    for i in range(from: 0, to: SENTENCES.len())
        handles @ handles.push(hero_thread_spawn(body: count_words_at, arg: i))
    total @= 0
    for h in handles
        n = hero_thread_join(handle: h)
        print(n)
        total @ total + n
    print(total)
```

No other file is needed. The program uses no package, so it has no `heroes.toml`.

## choice_points

1. **How each thread gets its sentence.** The spec says only one `i64` goes into a thread. So I pass the sentence's index (`arg: i`) and the thread looks the sentence up itself.
   - The other choice is to try to pass the string. The spawn signature has no place for it, so **the compiler refuses** that program.

2. **Where the sentences live.** I put them in `constant SENTENCES: [str]`, written as an array of string literals. The thread body reads this constant.
   - The spec only shows an `i64` constant. It says a constant's body "computes over literals and other constants", and the type rules allow `[str]`. I also assume a thread may read a constant, since constants are immutable and are not handed into the thread.
   - If either assumption is wrong, **the compiler refuses** the program.
   - The safe alternative is a `match index` with arms `0`, `1`, `2`, `3` and a `_` arm inside `count_words_at`. That needs no constant and gives the same output.

3. **Declaring the thread functions.** I wrote an `extern "hero_os.h"` group with exactly the signatures from section 13, and no `link`. The spec says the language's own header is "linked where a group names one".
   - If `hero_os.h` actually needs a `link` or uses a different C width, **the compiler refuses** the program, because clang checks the declarations against the header.
   - Calling the functions with no `extern` group would also be **refused**, as unknown names.

4. **Starting every thread before waiting on any.** I spawn all four in one loop, then join them in a second loop.
   - Joining each thread right after spawning it would run the threads one after another. Its output would be the same, but it would **not meet the task's "running at the same time"**.

5. **Order of the output.** I join the handles in sentence order and print each count when its join returns. The order is therefore fixed, whichever thread finishes first.
   - Printing from inside the threads would give **output in a random order**, and it could interleave.

6. **How words are counted.** The count is the number of spaces plus one, which follows the task's definition of words separated by single spaces.
   - An empty string would count as 1 and not 0, but none of the four sentences is empty.
   - Counting runs of non-space characters gives the same answer here.

7. **Characters or bytes.** I loop over `s.chars()` and compare with `" "`.
   - Looping over `s[i]` with `' '` would also work, because `s[i]` gives a `u8` and the character literal takes `u8` from the comparison. But it needs `s.len()` to work as `range`'s `to`, which is the same uncertainty as point 8.

8. **What type `len()` returns.** I used `SENTENCES.len()` as `range`'s `to`, which is an `i64`. The spec never states `len`'s return type.
   - If it returns `u64`, **the compiler refuses** the program, because the spec allows no implicit conversions. Writing `to: 4` avoids the risk.

9. **Named arguments.**
   - `range(from:, to:)` must use names, because both parameters are `i64`. Leaving them out is **refused**.
   - In the spawn and join calls, the names are optional because the parameter types differ. With or without them, the program behaves the same.

10. **The array of handles.** I used `handles: [i64] @= []` with the type written, because an empty literal needs one. I grow it with `handles @ handles.push(...)`, the in-place form for a plain name.
    - Leaving out the annotation is **refused**.

11. **`count_words` takes `s: str` as a copy, not `lent`.** Either way is accepted and prints the same. `count_words_at` cannot have a `lent` parameter, because a function used as a value may not have one.

## prediction

I expect the compiler to accept `heroes build main.hero`. The main risks are choice points 2, 3 and 8: a `[str]` constant read inside a thread, the exact declarations `hero_os.h` expects, and `len()` returning `i64`.

When run, it prints:

```
4
2
6
2
14
```

## context

Besides this directory's files (`brief.md`, `task.md` and `heroes-spec.md`), the only thing that reached me was the framing system prompt. It says I am a Claude agent, gives the working directory and date, and lists the file-reading tool. No other instruction, rule, contract, memory or document about Heroes reached me.
`````

### t2-x-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't create `report.md` because I can only read files here, not write them. As the brief allows, the whole report is below.

The main finding: the specification says threads come from the language's own C header `hero_os.h` (section 13), but it never lists any of that header's functions. So the thread part of this program is my guess. The compiler checks every declaration against the real header (via clang), so the program builds only if my guesses match it.

---

# report.md

## answer

The task asks for `main.hero`. It holds the four sentences, counts each one's words on its own thread with all four threads running at once, then prints the four counts in order and their sum. It uses no package, so there is no `heroes.toml`. It is one file:

### `main.hero`
```
# Thread primitives from the language's own OS layer (section 13).
extern "hero_os.h"
    record Thread tag hero_thread
    function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join
    function hero_thread_join(t: Thread consumes, @result: i64) -> i64 when 0

# The four sentences, in order.
constant SENTENCES: [str]
    ["the quick brown fox", "jumps over", "the lazy dog sleeps all day", "and wakes"]

# Words are runs of characters between single spaces.
function count_words(s: str) -> i64
    if s.len() == 0
        return 0
    n @= 1
    for c in s.chars()
        if c == " "
            n @ n + 1
    return n

# Body of one thread: count the words of sentence `i`.
function count_sentence(i: i64) -> i64
    return count_words(SENTENCES[i])

function main()
    t0 = hero_thread_spawn(count_sentence, 0)
    t1 = hero_thread_spawn(count_sentence, 1)
    t2 = hero_thread_spawn(count_sentence, 2)
    t3 = hero_thread_spawn(count_sentence, 3)
    c0 @= 0
    c1 @= 0
    c2 @= 0
    c3 @= 0
    rc0 = hero_thread_join(t0, @c0)
    rc1 = hero_thread_join(t1, @c1)
    rc2 = hero_thread_join(t2, @c2)
    rc3 = hero_thread_join(t3, @c3)
    assert rc0 == 0
    assert rc1 == 0
    assert rc2 == 0
    assert rc3 == 0
    print(c0)
    print(c1)
    print(c2)
    print(c3)
    print(c0 + c1 + c2 + c3)
```

Will `heroes build main.hero` accept it? Only if `hero_os.h` really declares `hero_thread`, `hero_thread_spawn` and `hero_thread_join` with these exact types. The specification doesn't say, so I don't expect this to compile on the first try. If it does build, it prints the five lines listed under `prediction`.

## choice_points

1. **Which thread functions to call.** The specification names `hero_os.h` but none of its functions, types or calling style. I made up a small, simple interface:
   - a handle type `hero_thread *`;
   - `spawn(callback(int64) -> int64, int64)`;
   - `join(handle, int64 *out) -> int64`, returning 0 on success.

   **If the real header differs** (other names, `void *` arguments, a status returned instead of written to an out-parameter, a different handle type), clang's check refuses the program. Any other guess carries the same risk: there was no right choice available from the specification.

2. **`hero_os.h` or `pthread.h`.** I followed the specification's line that threads come from `hero_os.h`. The alternative was the real `pthread.h` API (`void *(*)(void *)` callback, `void *` argument, `void **` result).
   - **With `pthread.h`:** the handle type is platform-dependent (`unsigned long` on Linux, a struct pointer on macOS), so the declaration would be refused on one platform or the other.
   - Getting a per-thread `i64` in and out through `void *` has no clean form here. A lend lasts only for its call, but the thread would outlive `pthread_create`. That gives either a compile-time refusal or behaviour the specification doesn't define at run time.

3. **How each thread learns its sentence.** I pass the index as the callback's `i64` argument and read the sentence from a constant array. Closures and mutable globals don't exist, so the alternatives were four separate functions, one per sentence, or passing the string through C. Four functions would give the same output. Passing a `str` through C needs `cstr`/lease handling and a C signature the specification doesn't give, which most likely means a refusal.

4. **How results come back.** I used an `@` out-parameter on join, with one cell per thread. The other options:
   - **The thread prints its own count:** the lines would come out in a race order, so the program runs but may print the wrong order.
   - **No mutable globals exist:** so threads can't write results into shared state.

5. **Ownership marks on the handle.** I wrote `acquires hero_thread_join` on spawn, and `consumes` with `when 0` on join.
   - **Leaving the marks off:** the program still runs (`Thread` is a handle record).
   - **Marking `acquires` with no matching `consumes`:** a handle nobody consumes aborts when `main` returns.
   - **`consumes` without `when 0`:** this is legal, but then a failed join would still count as ending the handle.

6. **Using the join result.** The call returns a value, so I bind it and `assert` it is 0. Writing the call as a bare statement would be refused (a value must be used), and `_ = …` would be accepted but would hide a failed join.

7. **How words are counted.** I count `" "` characters via `chars()` and add 1, returning 0 for an empty string. Comparing bytes with `s[i] == ' '` over `range(from: 0, to: s.len())` would give the same result. My one doubt about that version: `range` gives `i64`, and the specification never states what type `len()` returns. If it isn't `i64`, the compiler refuses the mixed types. Splitting on single spaces with no doubled spaces makes "spaces + 1" exact for these four sentences.

8. **Writing the constant array.** I made `SENTENCES` a constant written as a `[str]` literal on one line. The specification's constant example is a single integer, and it doesn't clearly say whether an array or string constant is allowed. If it isn't, the compiler refuses it. In that case I'd fall back to a `match` on `i` with string arms and a `_` catch-all, which `i64` allows.

9. **Labelled arguments.** Spawn's two parameters have different types (a function and an `i64`), and so do join's (a `Thread` and an `@i64`), so I left the labels off the calls. If the real header's parameters share a type, the specification requires labels, and the compiler would refuse the calls without them.

10. **Order of the joins.** I start all four threads before joining any of them, so they run at the same time. Joining each thread right after starting it would print the same lines, but the threads would run one after another, which breaks the task.

## prediction

Whether it compiles: probably not on the first try. The language rules themselves (bindings, cells, `@` arguments, the handle's life, nothing unused) are respected, as far as I can check against the specification. The risk is entirely in the `extern "hero_os.h"` block, whose signatures the specification doesn't give. If they match the real header, the build succeeds and the program prints:

```
4
2
6
2
14
```

## context

Nothing reached my context except `brief.md`, `task.md` and `heroes-spec.md` from this directory, and the agent's own system prompt (the working directory and the date). No other instruction, rule, contract, memory or document was used. What I wrote about the real `pthread.h` API comes from general knowledge, not from any file.
`````

### t1-z-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I have no file-writing tool here, so the whole report follows, as the brief allows.

# report.md

## answer

The task asks for `main.hero`. It adds up `i * i` for every `i` from 0 up to but not including 800,000 in two ways, and prints each result on its own line. The first way splits the numbers into eight bands of 100,000, sums each band on its own thread with all eight running at once, and adds the eight results. The second way computes the whole sum on the program's own thread.

The right answer is (n−1)·n·(2n−1)/6 with n = 800,000, which is **170666346666800000**. That fits in an `i64`, and so does every partial sum and every `i * i`.

The specification says threads come from the language's `hero_os.h`, and that "into a thread and back crosses one `i64` and nothing else". It never gives that header's functions, types or names. So the `extern` group below is a guess, made with the FFI rules of section 13.

### `main.hero`

```
# Sum of i*i for every i in 0 up to, not including, 800,000:
# first on eight threads at once, then on the program's own thread alone.

extern "hero_os.h"
    record Thread tag hero_thread
    function hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread acquires hero_thread_join
    function hero_thread_join(t: Thread consumes) -> i64

constant BAND: i64
    100_000

constant BANDS: i64
    8

# The sum of i*i for every i from `from` up to, not including, `to`.
function sum_squares(from: i64, to: i64) -> i64
    total @= 0
    for i in range(from: from, to: to)
        total @ total + i * i
    return total

# The sum of one band of BAND consecutive numbers; a thread's body.
function band_sum(band: i64) -> i64
    start = band * BAND
    return sum_squares(from: start, to: start + BAND)

function main()
    t0 = hero_thread_spawn(band_sum, 0)
    t1 = hero_thread_spawn(band_sum, 1)
    t2 = hero_thread_spawn(band_sum, 2)
    t3 = hero_thread_spawn(band_sum, 3)
    t4 = hero_thread_spawn(band_sum, 4)
    t5 = hero_thread_spawn(band_sum, 5)
    t6 = hero_thread_spawn(band_sum, 6)
    t7 = hero_thread_spawn(band_sum, 7)
    parallel = (hero_thread_join(t0) + hero_thread_join(t1) + hero_thread_join(t2) +
        hero_thread_join(t3) + hero_thread_join(t4) + hero_thread_join(t5) +
        hero_thread_join(t6) + hero_thread_join(t7))
    sequential = sum_squares(from: 0, to: BANDS * BAND)
    print(parallel)
    print(sequential)
```

No other files are needed. The program uses no package, so it has no `heroes.toml` (section 1).

## choice_points

1. **What the thread functions in `hero_os.h` are called and what they look like.** The specification names the header and says one `i64` goes in and one comes back, and nothing more.
   - **My choice:** I wrote `hero_thread_spawn(f: (function(i64) -> i64), arg: i64) -> Thread` and `hero_thread_join(t: Thread consumes) -> i64`.
   - **Alternatives:** the real header may use other names, such as `hero_spawn` or `hero_thread_create`. It may return the result through an out-parameter, such as `@result: i64` with an `-> i64 when 0` status. It may take the argument before the callback.
   - **What the other choices produce:** section 13 says clang checks every declaration against the header. Any difference makes the **compiler refuse** the program. This is the main risk in the program, and the specification gives no way to settle it.

2. **The C name of the thread handle (`tag hero_thread`).**
   - **My choice:** a handle record, `record Thread tag hero_thread`, which section 13 reads as `hero_thread *`.
   - **Alternatives:** the header might use another struct name, a `void *` (`tag void`), or a plain integer id typed `i64` with no handle at all.
   - **What the other choices produce:** a wrong tag is **refused by the compiler**. If the thread really is an integer id, my `consumes`/`acquires` marks are errors, because those words may mark only a value that reaches a handle, so the **compiler refuses** that too.

3. **Lifetime marks on the handle.**
   - **My choice:** `acquires hero_thread_join` on the spawn result and `consumes` on the join parameter. Without them the language could not check that every thread is joined.
   - **The other choice:** dropping both marks would probably still compile. If I kept `consumes` on join but dropped `acquires` on spawn, the rule that every call handing back a consumed handle type must say which it is would make the **compiler refuse** it.

4. **No `link` clause.**
   - **My choice:** I wrote none. Section 13 says these come "from the language's `hero_os.h`", which suggests the language supplies the library itself.
   - **What the other choice produces:** if a library must be named, building **fails at link time**.

5. **Eight named handles instead of an array of handles.**
   - **My choice:** `t0` to `t7`, each consumed exactly once.
   - **The other choice:** storing the handles in a `[Thread]` and joining in a `for` loop. Each loop element is a copy, and a copy stays behind in the array. Section 13 says handing C "a copy made before" can **abort when run**. It also seems to leave live handles behind, which may **abort when `main` returns** ("a handle nobody consumes"). The specification does not say how a handle inside an array is tracked, so I avoided the question.

6. **Named arguments for `sum_squares` and `range`.**
   - **My choice:** both parameters are `i64`, so I wrote `from:` and `to:` at every call, as section 9 requires.
   - **The other choice:** positional arguments are **refused by the compiler**.
   - `hero_thread_spawn`'s two parameters have different types, so positional arguments are allowed there.

7. **A `for` over `range` instead of a `while` loop.**
   - **My choice:** `range` builds a 100,000-element `[i64]` for each band. That costs memory but gives the same answer.
   - **The other choice:** a `while` loop with a counter cell gives the same output with less allocation. Getting the end condition wrong (`<=` instead of `<`) would **run and give a different answer**.

8. **Breaking the long sum across lines.**
   - **My choice:** I wrapped it in parentheses and broke each line after a `+`, following the layout rules at the top of the specification.
   - **The other choice:** breaking it outside parentheses is **refused by the compiler**.

9. **Literal types.**
   - **My choice:** `total @= 0` and the other bare literals default to `i64`, which is what the sum needs.
   - **The other choice:** typing the counter `i32` would make `i * i` **abort when run** from overflow, since 799,999² ≈ 6.4e11 does not fit in an `i32`.

10. **The two constants.** `BAND` and `BANDS` are both used: `BANDS` sets the sequential bound. An unused constant might be treated like an unused binding and **refused by the compiler**, so I made sure both are read.

## prediction

- **Compiler:** it accepts the program only if `hero_os.h` really declares `hero_thread_spawn`, `hero_thread_join` and `struct hero_thread` with the signatures in choice points 1 and 2. The specification doesn't give that header, so I can't promise this; it is the likeliest reason for a refusal. Everything else (layout, named arguments, used bindings, constants, the `for`/`range` loop, the parenthesised continuation) follows the specification as written, and I expect it to type-check.
- **Output, if accepted and run:** two lines, the same number on each.

```
170666346666800000
170666346666800000
```

- **Other outcomes:** if the header differs, `heroes build main.hero` fails with a clang error about the `extern` group and no binary is made. Otherwise, the arithmetic cannot overflow, every thread handle is consumed exactly once, and the program exits normally.

## context

Apart from my working directory and the date, given by the environment, nothing outside this directory reached my context. That means no other instruction, rule, contract, memory or document. I read only `brief.md`, `task.md` and `heroes-spec.md`.
`````

### t1-y-2

From `result` in `run.json` (no `report.md`).

`````markdown
I couldn't write `report.md` because I have no tool that writes files; I can only read them. As the brief allows, here is the whole report.

# report.md

## answer

The task asks for a program `main.hero` that does two things. First, it adds up `i * i` for `i` from 0 to 799,999 using eight threads at once, one per band of 100,000 numbers. Then it computes the same sum on the main thread alone. It prints each result on its own line.

The thread functions come from the language's `hero_os.h` (section 13). I declare them in an `extern` group exactly as the spec gives their signatures. The program uses no package, so it needs no `heroes.toml`. The only file is:

### `main.hero`
```
extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

# How many consecutive numbers one band holds.
constant BAND: i64
    100_000

# How many bands, and so how many threads.
constant BANDS: i64
    8

# The sum of i * i over band `band`: from band * BAND up to, not including, (band + 1) * BAND.
function band_sum(band: i64) -> i64
    start = band * BAND
    stop = start + BAND
    total @= 0
    i @= start
    while i < stop
        total @ total + i * i
        i @ i + 1
    return total

function main()
    handles: [i64] @= []
    for b in range(from: 0, to: BANDS)
        handles @ handles.push(hero_thread_spawn(body: band_sum, arg: b))
    threaded @= 0
    for h in handles
        threaded @ threaded + hero_thread_join(handle: h)
    print(threaded)
    single @= 0
    i @= 0
    while i < BANDS * BAND
        single @ single + i * i
        i @ i + 1
    print(single)
```

All eight threads are started before any is joined, so they run at the same time.

## choice_points

1. **How the thread functions are declared.** I wrote `extern "hero_os.h"` with no `link`. The spec says `hero_os.h` belongs to the language and is "linked where a group names one," so naming the header should be enough.
   - **Other option:** adding `link "hero_os"` or similar. If no library has that name, the compiler or linker would refuse the program.

2. **Types in the extern signatures.** I copied them from section 13: `body: (function(i64) -> i64), arg: i64) -> i64` and `handle: i64 -> i64`. Parameters must match the header's own width and sign.
   - **Other option:** a narrower type such as `i32`, or a handle record like `record Thread tag ...`. Clang's check against the header would refuse it.

3. **Labels on calls.** `range(from:, to:)` must have labels because both parameters are `i64` (section 9). For `hero_thread_spawn` the two parameters have different types, so labels are optional; I wrote them anyway. `hero_thread_join(handle: h)` is also labelled though it needs no label.
   - **Other option:** `range(0, BANDS)` without labels would be refused by the compiler. Leaving labels off the thread calls would still compile.

4. **Passing `band_sum` to a thread.** It is a top-level function with no `@` or `lent` parameter, so it can be used as a value.
   - **Other option:** giving it an `@` or `lent` parameter would make the compiler refuse it.

5. **Starting all threads, then joining.** I start all eight first and keep the handles in an array, then join them.
   - **Other option:** joining each thread right after starting it would compile and give the same number. But the threads would run one after another, which breaks what the task asks.

6. **Building the handles array.** I used `handles @ handles.push(...)` on a plain name, so it grows in place (section 10). The empty array has a type annotation because the spec requires one.
   - **Other option:** `handles @= []` without `: [i64]` would be refused by the compiler.

7. **Loops: `while` instead of `for i in range(...)`.** `range` builds a whole array: 100,000 elements per band and 800,000 on the main thread. A `while` loop avoids that.
   - **Other option:** `for i in range(from: start, to: stop)` would compile and give the same answer, just using more memory.

8. **How the main thread computes its sum.** I wrote a separate direct loop, rather than calling `band_sum` eight times on the main thread.
   - **Other option:** calling `band_sum` eight times would also compile and print the same number. But the two results would then share the same code, so the check would prove less.

9. **Integer types.** All sums and literals are `i64`, the default. The total is 170,666,346,666,800,000, which is below the `i64` limit of about 9.22×10¹⁸. The largest band sum and the largest single term are well within range too, so nothing overflows.
   - **Other option:** a narrower type such as `i32` would overflow, and in Heroes overflow stops the program with an error at run time.

10. **Constants.** The bodies are indented literals, as in section 4; `100_000` uses the allowed digit separator.
    - **Other option:** the spec's wording leaves unclear whether a lowercase constant name is refused, so I used SCREAMING_CASE.

11. **Unused names.** Every name I bind is read: `b`, `h`, `start`, `stop`, `handles`, and each cell is re-bound.
    - **Other option:** binding the result of `hero_thread_join` and never using it would be refused by the compiler.

## prediction

I expect `heroes build main.hero` to accept the program. Running the binary should print:

```
170666346666800000
170666346666800000
```

The check: (n−1)·n·(2n−1)/6 with n = 800,000 is 799,999 · 800,000 · 1,599,999 / 6 = 1,023,998,080,000,800,000 / 6 = 170,666,346,666,800,000.

The main risk is choice point 1. If `hero_os.h` needs an explicit `link`, the build would fail at link time.

## context

The only files that reached my context are `brief.md`, `task.md` and `heroes-spec.md`, all in this directory. Apart from my standard system instructions, no other instruction, rule, contract, memory or document reached me. I read no other file.
`````

### t1-x-2

From `result` in `run.json` (no `report.md`).

`````markdown
I only have a Read tool, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

The task asks for `main.hero`. It sums `i * i` for `0 <= i < 800,000` twice: first as eight bands of 100,000 numbers, each on its own thread, all running at once, with the eight results added; then on the main thread alone. Each sum prints on its own line. Both lines should be **170666346666800000**, which is `(n−1)·n·(2n−1)/6` for `n = 800,000`. That fits in an `i64`, so the language's overflow abort never fires.

The spec says threads come from "the language's `hero_os.h`", but it never says what that header declares: no function names, signatures or types. So I couldn't write a call into `hero_os.h` that I could stand behind. Instead I wrote my own small header, `bands.h`, which wraps POSIX threads. Through the spec's FFI (section 13) it exposes a handle type and two functions, and each thread calls back into a Heroes function to do the band's sum. So the summing itself is written in Heroes. The program is two files.

### `main.hero`

```
# Sum of i * i for 0 <= i < 800,000: first on eight threads, one band of
# 100,000 numbers each, then on the program's own thread alone.

extern "bands.h" link "pthread"
    record Job tag band_job
    function band_start(lo: i64, hi: i64, f: (function(lo: i64, hi: i64) -> i64)) -> Job acquires band_join
    function band_join(job: Job consumes) -> i64

constant BANDS: i64
    8

constant WIDTH: i64
    100_000

# The sum of i * i for lo <= i < hi.
function band_sum(lo: i64, hi: i64) -> i64
    total @= 0
    for i in range(from: lo, to: hi)
        total @ total + i * i
    return total

function main()
    jobs: [Job] @= []
    for b in range(from: 0, to: BANDS)
        lo = b * WIDTH
        jobs @ jobs.push(band_start(lo: lo, hi: lo + WIDTH, f: band_sum))
    threaded @= 0
    for job in jobs
        threaded @ threaded + band_join(job)
    print(threaded)
    print(band_sum(lo: 0, hi: BANDS * WIDTH))
```

### `bands.h`

```c
#ifndef BANDS_H
#define BANDS_H

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t (*band_fn)(int64_t lo, int64_t hi);

typedef struct band_job {
    pthread_t thread;
    band_fn fn;
    int64_t lo;
    int64_t hi;
    int64_t sum;
} band_job;

static void *band_main(void *arg) {
    band_job *job = (band_job *)arg;
    job->sum = job->fn(job->lo, job->hi);
    return NULL;
}

/* Starts a thread computing fn(lo, hi); aborts if it cannot. */
static inline band_job *band_start(int64_t lo, int64_t hi, band_fn fn) {
    band_job *job = (band_job *)malloc(sizeof *job);
    if (job == NULL) abort();
    job->fn = fn;
    job->lo = lo;
    job->hi = hi;
    job->sum = 0;
    if (pthread_create(&job->thread, NULL, band_main, job) != 0) abort();
    return job;
}

/* Waits for the thread, frees the job, and gives back its sum. */
static inline int64_t band_join(band_job *job) {
    if (pthread_join(job->thread, NULL) != 0) abort();
    int64_t sum = job->sum;
    free(job);
    return sum;
}

#endif
```

## choice_points

1. **Where the threads come from.** The spec names `hero_os.h` as the source of threads but gives none of its API.
   - **My choice:** my own header, `bands.h`, wrapping pthreads.
   - **The other option:** guess names such as `hero_thread_spawn` or `hero_thread_join` in `extern "hero_os.h"`. If a guessed name or signature is wrong, the compiler refuses the program, because clang checks every declaration against the header. Nothing in the spec lets a model get this right on the first try, and this is the main finding of the experiment.
   - **Risk of my choice:** the spec may require threads to come only from `hero_os.h`. In that case the compiler or the language's rules could refuse a program that creates threads itself.

2. **How C code gets compiled.** The spec has no way to compile a `.c` file. So I put the C code in `static inline` / `static` functions inside the header. If the compiler only binds to symbols it links, rather than compiling the header into the generated C, these functions don't exist and the build is refused or fails to link. I also assumed `extern "bands.h"` finds the header beside `main.hero`. If it doesn't, the build fails with "header not found".

3. **Linking pthread.** I wrote `link "pthread"`. Leaving it out would also work on macOS and on glibc 2.34 or later. On older glibc it would fail to link.

4. **Getting data into and out of a thread.** I used an opaque handle (`record Job tag band_job`) plus a Heroes callback that C calls with `lo` and `hi`.
   - **Option: `pthread_create` directly with a `ptr` argument.** Heroes has no way to read through a `ptr`, so the band couldn't be passed in and the sum couldn't come back. That design can't be written.
   - **Option: an `@` record element handed to C and kept.** This depends on whether the array's place moves, and a moved place gives undefined or wrong results.

5. **Calling Heroes code on a thread the language didn't create.** I assumed the generated `band_sum` is safe to call at the same time from foreign threads. The spec says nothing about this. If the runtime keeps per-thread state, the program could crash or abort when run.

6. **Callback type with two `i64` parameters.** Two parameters share a type, so the spec makes names mandatory in the function type: `(function(lo: i64, hi: i64) -> i64)`. `band_sum` uses the same names in the same order. A function type without the names, or with them in the other order, would be another type, and the compiler would refuse it.

7. **Resource marks.** I used `acquires band_join` on the result and `consumes` on `band_join`'s parameter, without `when`.
   - **Option: leave the marks out.** This should still compile. The spec only says that a handle nobody consumes aborts at the end of `main`, and it doesn't say whether that applies to handles with no mark.
   - **Option: add `when 0`.** That would be wrong here, because `band_join` returns a sum, not a status. A sum of 0 would then count as "consumed" and anything else as "not consumed", so the program would abort at exit with live handles.

8. **Keeping the handles in a `[Job]` array** and consuming them while iterating, which leaves copies in the array. I took the spec to mean that a handle's life is shared by all its copies. Eight named bindings would avoid that doubt but are clumsy. If the compiler counted the copies left in the array as unconsumed, the program would abort when `main` returns.

9. **`range` versus `while`.** `range(from:, to:)` builds an array of 100,000 numbers per band, plus one of 800,000 for the single-thread sum. A `while` with a counter cell avoids that memory. The result is the same either way.

10. **Integer type.** I used `i64`, the default for literals. `i32` or `u32` would overflow, and overflow aborts at every width, so the program would stop with an error.

11. **No null check on `band_start`'s result.** The C side calls `abort()` instead of returning null. If it returned null without a check in Heroes, `band_join` would dereference null and crash.

## prediction

- **Compiles:** I expect the compiler to accept it, provided three assumptions hold: the header is found beside `main.hero`, `static inline` header functions can be bound, and callbacks into Heroes from foreign threads are allowed. I'm about 50/50 on these, and choice point 2 is the most likely failure. If the intended route was `hero_os.h`, the program might be refused on that ground instead.
- **Output when built and run:**
```
170666346666800000
170666346666800000
```

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached me, plus the general system and tool instructions for this session (the agent framing and tool descriptions). Those carried no Heroes-specific rules. The working-directory path mentions "heroes-lang" and "llm-ergonomist", but I read no file outside this directory and no other document, memory or contract reached me.
`````
