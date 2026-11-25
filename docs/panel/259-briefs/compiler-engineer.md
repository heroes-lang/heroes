# Panel 259, the compiler-engineer's brief

Read `00-shared.md` first, whole, then the critic's first reading (`docs/panel/259-reports/critic-briefs.md`, written
at the first base `2cf85ea64`; the briefs were repaired from its items 1 to 9 and re-frozen at the base `3ec775b66`,
`00-shared.md` § 2) and the two drafts it names (§ 4.6). Your seat judges **implementation cost, soundness and what the
runtime and the emitted C owe** (design.md §1.1, §1.7, §1.12, Part 5, reached by `grep -n`;
`.claude/rules/generated-c.md`, which moved between the two bases: read it at this one). You have a veto, on soundness,
and it yields to the most robust resolution. Your files are `runtime/` and `selfhost/` (never `crates/`, which is not
in the tree: the Rust bootstrap is the tag `m-bootstrap-archive`). **You measure with a stage 2 compiler** built in
your own copy, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-compiler-engineer`, a worktree of
`3ec775b66`; a runtime you change is a copy of `runtime/` handed to a build by `HEROES_RUNTIME=<copy>` (the variable is
read in `selfhost/main.hero`, `grep -n HEROES_RUNTIME selfhost/main.hero`), never the copy's own `runtime/` edited under
a build another process reads. **You hold this Mac and Linux arm64** (`heroes-linux-arm64:latest`); x86-64 is the
ffi-pragmatist's, and so is every cell that only compiles there (question 4). **Every Linux cell you write names the
base it was run on** (`00-shared.md` § 2: the `dlsym-make` lane will move `unseen_carved`'s and `ph`'s rows).

## What you are asked to settle, in this order

1. **The mechanism, traced.** § 5 of the shared brief reads the code and says the thunk's entry or the coroutine's
   first take gives `body`'s room away; on Linux arm64 the critic's trace puts both coroutines' thunks above `body`'s
   frame (§ 5's last paragraph). Trace it in `shapes/own_carved.hero` with a counter or `lldb` in a copy of the
   runtime, **on this Mac and in the arm64 image**: which call, at which frame addresses, against which mark. Then
   **the lend regions**: write the shape beside it (a frame lends a local or a buffer to C, `lent` or `@`, and holds
   the lend across an own switch onto a carved stack, the coroutine lending too), run it plain and `--sanitize` on
   both platforms, three runs a cell, and say what C reads and writes when `lend.c:439`'s heal gives a live region
   away. **The regions' give does not fail as the room's does** (§ 4.1: `hero_lend_heal_to`, `lend.c:462`, returns
   silently at `:472` where the room's give stops with *compiler bug* at `frame.c:237`), so a lend shape may print
   nothing false and still have lost its region. A region is `mmap`ed with a guard page (`lend.c`, `grep -n
   hero_lend_map`): is the failure a wrong byte, a fault, or a false message?
2. **Every judgment by order, enumerated from the code** (`grep -n` the callers of `hero_frame_is_below`,
   `hero_stack_lane`, and anything else that compares a frame address with another; the signal machinery's rules 4 to 7
   in `runtime/parts/sigentries.c`, which judge a jump's target by the stacks they know). For each: does an own switch
   onto a carved stack make it lie, and what does the program then get? **One already fires, and is traced** (shared
   brief § 5, the critic's trace at `2cf85ea64`, `critic/trace/`, re-run by the repair at this base in the arm64 image
   with the same lines and values, `runs/linux-arm64/out-unseen_carved-trace.txt`): rule 6, `hero_sig_reenter`
   (`sigentries.c:379-384`), judged coroutine 0's **yield**, a `swapcontext` the runtime saw landing below the frame
   making it on the one range it knew, and the refusal was told at coroutine 1's first thunk entry in
   `HERO_SIGNAL_REENTERED`'s words (`sigguard.c:596-600`), 2705's and 2755's class. Trace it on this Mac too, where the
   switch is seen and should name its range (an inference, untraced), then say **whether any judgment could refuse
   `own_carved` and `below`, whose switches the runtime does not see, on every platform and in true words**; rule 6
   cannot, since it needs the switch seen. **And `HERO_SIGNAL_HELD`** (shared brief § 1 item 5: step 179a's arm,
   refusing a jump onto a stack `makecontext` was given that no switch was seen to enter, where contexts fall to route
   A): say whether its order judgment lies under an own switch onto a carved stack, and whether its words are true; the
   sitting ratifies or refuses it. **Run the one you think least likely to move** (a signal delivered while a coroutine
   runs on a carved stack; a C `longjmp` inside a coroutine on a carved stack, `shapes/jumpco.hero`'s shape with
   `own.h`).
3. **Route A, detection.** List every fact the runtime can read at the moment a heal would fire (the asking frame,
   the stack pointer, the marks, the thread's callback count `hero_callback_thread`, `callback.c:91-96`, the jump
   hook's records, `sigjump_record.c`, a frame-record walk). For each, **build the programs it must tell apart**: a C
   jump over a callback that R1 heals rightly (panel 258's `docs/panel/258-briefs/jumps/room_order.hero`, and an
   unseen one in assembly) against `own_carved` **and `below`** (`shapes/below.hero`, the critic's: the coroutines'
   frames inside the open extern call, below `main`, no Heroes value held across the call; this Mac 134 *compiler
   bug*, the arm64 image 139 at this base and the x86-64 one 139 at `2cf85ea64`, a use-after-free under `--sanitize`
   on all three, shared brief § 5), and show whether
   the fact differs between them. A fact that differs is a detector: then the words, and whether it fires **before**
   any wrong value is read (today `own_carved` prints `body holds 11` before it aborts). A fact that never differs: say
   so, with what you tried.
4. **Route B, healing only on a seen jump, over the interval it crosses.** Prototype it in a runtime copy: an entry
   given back only where a jump the hook decoded (`runtime/parts/sigjump_save.c` on Linux, `rebind_jump.c` on Darwin)
   crossed its frame, between the jumping frame and the target's stack pointer, on the stack the target lies on. Run
   panel 258's five reproducers (`docs/panel/258-briefs/jumps/`), the eight coroutine cases of step 181a (`ls
   tests/golden/run | grep -E
   '^fixedbugs-(a-c-longjmp-over-a-callback-inside|a-coroutine-left-for-good|coroutines-|three-coroutines)' | grep -c
   '\.hero$'` prints 8 at this base; `grep -E 'coroutine'` alone selects 51, the 2702 to 2745 signal cases among them,
   which a route that touches the signal machinery owes too), narrowed by the harness's third word, the shapes, and **an
   unseen jump over a callback** in assembly, each three runs, plain and `--sanitize`. **Price B on § 4.3's doors** (the
   shared brief's reading of panel 258's table: 8, 12 and 13 abandon frames with the process running on, 9 and 10 end
   the thread). **`__builtin_longjmp` compiles on x86-64 alone** (the repair, 07:44: clang refuses `__builtin_setjmp`
   for this Mac's target and for `aarch64-linux-gnu`, and writes an object for `x86_64-linux-gnu`), and panel 258 left
   that door **unrun** there: **its run is the ffi-pragmatist's**, who holds x86-64; hand it your prototype runtime's
   path (a copy under your own folder, read only by it) or write the cell *unrun*. **Name every correct program that
   runs on `main` and stops running under it**, and the words it gets; a leak a gate stays silent on is a cost, a false
   message is a defect.
5. **Route C, an identity no switch fools.** Prototype the frame-record walk (from the asking frame to the thread's
   outermost frame, asked only where a heal would fire) and say: its cost on the path where nothing heals (R1's and
   step 181a's instruments are `shapes/cost/`: `cost_cb.hero`, `cost_lend.hero`, `cost_room.hero` with `cost.h`, run
   by **`shapes/cost/cost.sh <your copy> <runtime-a> <runtime-b> <rounds> <level>`**, which copies the three programs
   into `<your copy>/cost-run/`, builds them with `<your copy>/heroes-next` against the two runtimes, and prints one
   line a run, instructions retired by `/usr/bin/time -l`; repointed by the repair from the lane's folders, which the
   sweep takes, and run by it once at this base, one round at `-O2` against two copies of one runtime, exit 0, six
   lines; best of N alternating runs, the first discarded), its cost where it walks, and **what fools it**: a C frame
   with no frame record between (x86-64, the ffi-pragmatist's image), a first frame a library links to the resumer's
   on purpose (write one), a coroutine started on the thread's stack by a library you can name. **The other candidate
   the shared brief names, the stack pointer of the extern call that is still open, is fooled by `below`** (§ 6 C):
   say whether that holds in your runs. Then any other identity you find.
6. **Route E, no healing by address.** Every take of a callback-reachable frame aside: its cost (the lane measured
   **+550 instructions at 32 KB and +1,900 at 256 KB** a take, carried from `f767e0d4e`'s body: re-measure), and
   **what then repairs defects 2752 and 2756** for the jumps R1 heals today. Is E plus B (heal only by a seen jump's
   interval, the asides keyed by stack) sound under every switch?
7. **Route D from the runtime's side.** What an entry point a binding calls at each switch must do so the order is true
   (name the stack as `makecontext`'s ranges are named, `sigentries.c` and `rebind_entries.c`, its count
   `hero_stacks_made`, `callback.c:197`; it would be declared in `runtime/heroes_runtime.h` or `runtime/hero_os.h`, the
   first of which moved at this base, +86 −2, `git diff --numstat 2cf85ea64 3ec775b66`), what it costs a program that
   never calls it, and **whether the runtime can learn a switch from the call a library already makes for
   AddressSanitizer** (`__sanitizer_start_switch_fiber`,
   `tests/golden/run/fixedbugs-coroutines-switched-by-hand.h:45-46`) without breaking `--sanitize`: **that variant
   reaches `--sanitize` builds alone** (shared brief § 6 D, by `grep`: Boost's default backends make no such call, its
   `ucontext` backends make it around a `::swapcontext` the runtime already sees, and the tree's header declares it only
   under `__has_feature(address_sanitizer)`), so a plain build gets nothing from it. Say what a binding that does not
   tell gets under D.
8. **The route nobody listed** (§ 6 G, and the critic's § G): a large value's room owned by its C frame and by no
   per-thread order; a library that copies stacks, where two coroutines' frames hold the same addresses in turn (write
   one: `memcpy` a coroutine's stack segment out and in, on the thread's own stack). Which route survives the copying
   shape?
9. **What the emitter owes** for the route you would adopt (`selfhost/emit/large_value.hero`, which moved at this
   base by one line, `out.push("NULL")` to `out.push("((void *)0)")`; `selfhost/emit/callback_thunk.hero`; the
   prologue), and whether `--emit-c` of the compiler stays byte-identical bar the `hero_program_sources` line
   (`.claude/rules/verification.md` § The formatter's probe states that exception).
10. **Does the route you would adopt need a spec sentence**, and which program a reader could not otherwise predict?
    Say so first if yes. `arms/arm-r`, `arm-t`, `arm-p` are the clerk's drafts (the critic's F18 marks clauses the
    runs already decide); your report marks each clause of each **true** (the program and its cell), **false** (the
    program that falsifies it) or **untested** (why).

**A route that does not build is not adopted** (`.claude/skills/panel/SKILL.md` § 3c): build the one you would adopt
in your copy and run, at least, `own_carved`, **`below`**, `own_heap`, `carved`, `quit_fresh`, `jumpco`, panel 258's
five, and the eight coroutine goldens, on this Mac and in the arm64 image, plain and `--sanitize`. Keep every program
you write, with the script that built and ran it, in `docs/panel/259-reports/programs/compiler-engineer/` of your copy
(sources, never a binary), so the critic can run them on the other platform.

## The report

`docs/panel/259-reports/compiler-engineer.md` of your copy, written as you go: your verdict, the mechanism you would
build and its measured cost, the table of routes against shapes (route, shape, platform and **the base it ran on**,
plain and `--sanitize`, three runs, first line of stderr; `below` beside `own_carved` in every route's rows), the
detector you found or the reading that found none, **what a veto would compel**, what the conservative resolution would
have been, and one falsifiable prediction with the measurement at which it is checkable. Then the verdict structure of
`.claude/agents/compiler-engineer.md`, and one line per numbered question.
