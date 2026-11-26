# Panel 261, the ffi-pragmatist's brief

Read `00-shared.md` first, whole, then the critic's first reading (`docs/panel/261-reports/critic-briefs.md`), **on
which these briefs were repaired** before any seat started. **Your agent file is the base's** (`.claude/agents/ffi-pragmatist.md` of
your copy); design.md §4.19 and Part 8 item 19 are reached by `grep -n`. **Your veto on ABI breakage and soundness
stands** under the author's amendment of 2026-11-22.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-ffi-pragmatist`, a detached worktree at the
base `df92f9281` (`00-shared.md` names it). Build there from the seed, then stage 2 (`00-shared.md` § 8; `--version`
should read `sources 4dcf3a9f20257573`, the repair's stage 2 at the base; say if it does not). **You hold this Mac and the two Linux images**, `heroes-linux-arm64:latest` and `heroes-linux:latest`
(`docker images` lists both, about 01:14), one container at a time, `--rm`, named `p261-ffi`, removed when idle, run with
`docker run --pull never`. **No paid run, no credential printed, `gh` reads only, nothing outside the repository's root,
no `pkill`, no commit.** Report as you go to `docs/panel/261-reports/ffi-pragmatist.md` in your copy; C and programs
under `docs/panel/261-reports/programs/ffi-pragmatist/`.

## The C boundary today, re-read before you cite it

- **§ 13 lines 427 and 428**: *"A callback is a **parameter**, never a result or a signal handler, nor called by one;
  its parameters follow the same rule and `()` is `void`"*; and the frames C leaves (panel 259, landed at `168d3d6c7`);
  § 13's first line now names the platform layer's `layer` word (panel 260, step 197d, at the base), which touches no
  callback.
- **No binding in the tree needs the spelling question 7 asks** (the critic's § 5, re-run by the repair at the base,
  `ground/callbacks.sh`, its output `ground/callbacks.txt`): every function-typed parameter inside a `group` or
  `extern` block of `rebel/library`, `selfhost/`, `examples/` and `rebel/examples/` is a thread body, **18**, every one
  `(function(i64) -> i64)`, `(function(x: i64) -> i64)` or `(function(str) -> str)`, the `body` of a
  `hero_thread_spawn` function (2 in Rebel's library, `threads/hero_os.hero:22`, `:37`). **So your verdict on the C
  half rests on the bindings you write** (item 1 below), on Part 8's `sqlite3.h` counts (carried, which you re-run) and
  on panel 217's example; say so in the verdict. And those 18 are functions C calls on a thread of its own: a mode
  admitted in an `extern` callback's type and handed to `hero_thread_spawn` would hand one thread's place to another
  (the critic's reading, unrun), so questions 7 and 8 meet there (`00-shared.md` § 7 (p)).
- **design.md Part 8 item 19** (lines 4582 to 4640 at the base, panel 112, 2026-09-05; **carried**, never re-run here): over
  `sqlite3.h`'s 106 distinct callback signatures, 153 unspellable parameter occurrences, **102** a pointer to a struct
  the header declares (**+64** callbacks a spelling would unlock), 26 a pointer to a scalar, 18 a pointer to a pointer,
  4 a `const` pointee; *"17 of 106 bind today"*; and **a fifth cause, a grammar refusal**: *"`(function(i32, @out: i32)
  -> i32)` is `expected_function_type_params_close`. An out-parameter inside a callback is **26** occurrences in
  `sqlite3.h`"* (lines 4606 to 4608). **That code is stale at the base** (the critic's § 2 item 7, re-run by the
  repair): the out-parameter is **`error[expected_type]: expected a type, found `@``** at the `@`, exit 1 (probe `o1`),
  the code of `p1`; `expected_function_type_params_close` is `lent`'s refusal (`p4`). Its last lines name **panel 112's R6**, the route a mode in a callback's type must take: *"a qualifier
  field on the function type that already exists ... rather than a new `Ty` case"*. **Re-run the census** on the
  header this Mac has (`/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sqlite3.h`, present: `ls`;
  `grep -c '(\*'` reads 217 lines holding a pointer declarator, which is not the 106 signatures) and say what the
  proposal unlocks of it, `@` and `lent` apart.
- **Panel 217's ffi-pragmatist** (`docs/panel/217-reports/ffi-pragmatist.md:186-196`, read): a C callback receiving
  `const struct pt *` *"has no spelling today"*; `(function(p: Pt) -> i64)` is `error[ffi_callback_type]`; the C an
  F1 reaching a function type's parameter would emit, `int64_t h_one(const struct pt *)`, *"passes to that callback
  type and prints `42`, and is refused against `int64_t (*)(struct pt *)`, incompatible function pointer types"*.
- **The thunk**: `selfhost/emit/callback_thunk.hero` (429 lines; its comment, lines 1 to 30: a function value's
  address is what crosses, and a **thunk** with the function's own signature clears the handles a callback receives
  and checks its result); `selfhost/emit/callback_guard.hero` (207); `runtime/parts/callback.c` (111: the count of
  callbacks entered and not left, a frame a C jump abandoned).
- **An `@` slot in C** is a pointer: `int64_t h_f7_bump(h_f7_App *ph0_a, int64_t h1_x);`, while a value's typedef has
  no pointer slot today, `typedef int64_t (*h_0fn_32a9823f)(h_f7_App, int64_t);` (`00-shared.md` § 3.4, `f7`); a
  `lent` slot is the same pointer with the slot marked `read_only` in the IR (`selfhost/ir/read_only.hero`); a
  group's record lent without `@` crosses as `const T *` (§ 13 line 468: *"a pointer to const, the place C reads and
  never writes"*).
- **What `@` promises** (design.md §4.8 lines 1620 and 1621): *"`@` parameters never alias, so a direct-pointer
  lowering and `restrict` stay legal"*. C calling a callback may hand one pointer twice.
- **What the emitter already writes** (the critic's § 5, re-read by the repair): a library function's value twin
  **already carries a pointer slot for an `@` parameter**, `hero_value_h_library_dist(int64_t h0_a, int64_t *ph1_b)`
  (`selfhost/emit/signature.hero:326-342`'s test, line 342), and **a large group record through a function value takes
  C's convention through a twin** (panel 236's R3; `signature.hero:99-102`, `selfhost/emit/large_value.hero:82`
  `value_large`): two value-call conventions a mode slot meets.

## What you write, compile and run

1. **A real binding whose callback takes a pointer to a header's struct**, both halves: **`nftw`**, `int nftw(const
   char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int)` (`ftw.h:59-60` on this Mac,
   `grep -n`), whose callback holds a `const` pointee **and** a non-`const` one (design.md Part 8 item 19 names it as
   the case `const` alone does not reach); one sqlite3 callback with an out-parameter (an `sqlite3_exec` row callback,
   or one of the 26 the census names); and one of your choice on Linux. Write each as the proposal would let it be
   written (`(function(path: cstr, sb: Stat lent, flag: i32, @ftw: Ftw) -> i32)` or as the route the seats adopt
   spells it), **emit its C, compile it with the flags the compiler uses** (read them in `selfhost/cli/flags.hero`), run
   it on this Mac and in both images, and say for each whether clang accepts the value's type against the header's
   prototype (identical types in a function-pointer type, Part 8 item 19's *"inside a function-pointer type the types
   must be identical"*).
2. **What `@` and `lent` mean at the boundary, measured**: C handing one pointer to two `@` slots of one callback
   (does the emitted C keep `restrict` legal, or must the thunk compare the two?); C writing through a `lent` slot's
   pointer (the IR verifier sees only Heroes; does a `const` reach the typedef?); a callback receiving `@ftw` and
   keeping nothing of it past its return (§ 13 line 410: C *"may keep [a place] only while the place does not move"*;
   here the place is C's); a **handle** record reached through an `@` slot (the thunk's clear, `callback_thunk.hero`'s
   danger, lines 4 to 14); a group record of the program's own past 4,096 bytes (defect 2337's shape, `gh issue view
   2337`). For each: refused (by which code, at which line), or built and run on the three legs, with
   `--sanitize` where it reads memory.
3. **The C of a Heroes value with an `@` slot**, no `extern` involved: the typedef with the pointer, the call
   `fs[0](&h0_a, ...)`, a library function's value twin (`selfhost/emit/signature.hero:326-342`'s test: the twin drops
   `const HeroPlace *hero_caller`), on the three legs; and **byte-identity**: the C of a program that uses no mode in a
   function type is the same bytes under the edited compiler (the compiler-engineer builds one; if none exists when you
   start, say so and judge the C by hand).
4. **Threads**: `spawn(f, x)` with `f` taking `@` or `lent` (`runtime/parts/thread.c`, 144 lines; § 11 line 383): what
   the runtime would be handed, and why it is refused or meaningful.
5. **The shape beside each** (CLAUDE.md § RUN IT): a callback with an `@` slot to a scalar (`@n: i64` against `int64_t
   *`), to a pointer (`char **`), to a fixed array, and a callback returning through an `@` slot what it was handed.

## What you judge

`verdict` per route (`00-shared.md` § 6.2 and § 7, and § 1 item 4's question: a mode in an `extern` callback's type,
refused or meaningful), with `section` (design.md §1.11, §4.19, Part 8 item 19), `cost` (the C, the thunk, the
runtime), `prediction` and `condition`, and the binding C every verdict rests on, compiled or shown failing.
