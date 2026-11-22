# Panel 254, compiler-engineer

Read `00-shared.md` first, whole (repaired after the completeness critic's first reading,
`docs/panel/254-reports/critic-briefs.md`; its findings 11, 28, 29, 30, 34, 35 and 37 are in this brief). You judge the ceiling (design.md §1.1, §1.7, Part 5): what route B
costs in `selfhost/` and `runtime/`, whether it is core or sugar, and **whether it is sound**, measured
in your copy. Your veto is a refusal on soundness (a program that corrupts memory, a race the sanitizers
read); a cost is reported, not vetoed (`00-shared.md` § 2).

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/compiler-engineer/`,
a detached worktree at `89f155330`. Build its compiler as `00-shared.md` § 7 says, `export TMPDIR` inside
it, and write your work under an untracked `p254/` folder there. **Your report**:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/docs/panel/254-reports/compiler-engineer.md`,
written as you go. Never the Rust bootstrap at `m-bootstrap-archive`.

## Q1. What the language half costs, counted again on this tree (R3's reason 3)

1. Add one case for `(thread T)` and one for `(mailbox T)` to the checker's `Ty` in your copy
   (`selfhost/check/table.hero`; panel 251's probe was one `thread_ty { result: i64 }` case), then
   `./heroes-next check selfhost/main.hero`: count the `error[non_exhaustive]` lines and the files they
   stand in, for each case alone and for both. Panel 251 read **288 in 109 files** for one case; R3
   would reopen at **under 30**. Name the files by area (`check/`, `emit/`, `ir/`, `cli/`, ...).
2. The parser arm: `thread` and `mailbox` read as contextual words after a type's `(`, two `TypeKind`
   cases (251: `parse/type.hero` +12, `ast.hero` +6, **24 matches in 12 files**); count again.
3. **Operations written only after a dot** (S13): panel 251 measured that every method-form built-in of
   today takes its name everywhere (`error[builtin_name_taken]` for `function lease`, `cstr`,
   `validated`, `slice`, `must`, `default`, `is_err`), so *"route M needs a kind of name the compiler does
   not have today, one resolved by its receiver's type alone"*. Price that kind of name: where it would
   live (`selfhost/resolve/`, `selfhost/check/builtins.hero`), how many lines, what a program declaring
   `function send(...)` and calling `m.send(v)` then resolves to, and whether `x.f(y)` UFCS on a
   mailbox is refused or resolved to the program's `f`.
4. **The built-ins** `spawn`, `spawn_sized`, `mailbox`: a call typed by its context (`m: (mailbox Job)
   = mailbox()`), a generic built-in over the function's answer type; what the checker has today for
   either (`selfhost/check/builtins.hero`), counted.
5. **The refusals S10 and S11 need**: a `ptr` or `cstr`, or a value holding one, crossing; a handle
   crossing without `@`. Where each is refused (checker, lowering), and which diagnostic class each is
   (a new class is a spec-and-diagnostic matter: name it for the sitting).
6. **The doors after M (S14)**: what the compiler's own `selfhost/main.hero:88-113` becomes, and
   whether a program binding `hero_thread_spawn` must be refused, may stay, or is left unnamed; what
   each choice moves (**69 files declare a door, 73 name one**, six of them under `selfhost/` and one in
   `tests/harness/`, `00-shared.md` § 5 item 10): count the compiler's and the net's own.
7. **The walk a new surface form owes** (CLAUDE.md § 9, `.claude/rules/diagnostics-and-goldens.md`):
   the formatter first and its self-check, then `heroes grammar`, the editors' grammar (`editors/`), the
   site's colouring by `heroes lex`; count what each moves for `(thread T)`, `(mailbox T)`, `spawn(f, @x)`
   and `m.send(@v)` (the critic's finding 37: Q1.2 counts the formatter's `match` statements, not this
   walk).
8. **`spawn(f, @x)`** (S11 as repaired, the critic's finding 30): what the parser, the checker's `@`
   rules and the lowering need for an `@` argument to a built-in, and what a spawner's copy of a handle
   is after it.

**If the time allows** (you saw what panel 177's seats did): build the parser arm, the two `Ty` cases
and enough of the checker and emitter that a program with `spawn`, `t.wait()`, `mailbox()`,
`m.send(v)`, `m.receive(within_ms:)` and `m.close()` builds and runs. If it does not, Q2's emulation
stands in, named as such, and the compiler half is reported as counted, not built.

## Q2. The runtime half, built, and 7.13's heaps (R3's reason 4)

1. **A queue in the runtime** (`runtime/parts/`, in your copy): S5 to S8 as the proposal writes them,
   and the alternatives `00-shared.md` § 5 lists (a handle's life; a bounded queue; drain against
   stop), built enough to measure which is the more robust. Its waits on a condition variable; its
   wake on the stop (Q2.4); every object it keeps between calls `_Thread_local`, `_Atomic` or named in
   `SHARED_BY_DECISION` (`tests/harness/suite_runtime.hero:227`): run that suite on your copy, base and
   prototype, and give both count lines (defect 2588 says it is red at base on the platform layer;
   what moves is what counts).
2. **The deep copy**: a value crosses whole. Build it the way you judge most robust, and say what it
   does to the descriptor's *"five members and gains no sixth"* (`runtime/parts/desc.c:19`; design.md
   §4.20; panel 027): a function per type generated by the emitter beside `perfn.hero`'s, a sixth
   member, or a flat encoding into one runtime block and a rebuild on the other side. **7.13**: say,
   measured, which heap each block of a crossing value is allocated and freed in, on which thread, and
   whether every value is *"born and dies on one thread"* as `spawn_str.c:14-27` makes a `str`. Panel
   251's critic found its shallow crossing was 7.13's declined cross-heap refcount and its deep copy
   freed on one thread what another allocated.
3. **Stress, in C and under the sanitizers** (`00-shared.md` § 6 for TSan): 8 senders and 1 receiver,
   1 sender and 8 receivers, 64 threads, 100,000 values each, values of every shape (a `str` of 0 bytes
   and of 1 MiB, a nested array, a map, a record holding a map of arrays, a `(mailbox U)` inside a
   value); **a mailbox sent into its own queue inside a record holding `[(mailbox R)]`, and two mailboxes
   each inside the other's queue** (`00-shared.md` § 5 question 11, the critic's finding 29): what each
   life (S5's count of copies, a handle's life, another) leaks, tells, or frees, and what the leak gate
   then says at `main`'s end (a false *"compiler bug"* is a false message, CLAUDE.md § 8); a close racing a waiting `receive` and a racing `send`; a stop racing both; the last copy
   dropped on a thread other than the one that made it. 20 runs plain, 5 under ASan + UBSan, 5 under
   TSan, at least; every report counted, and the runs that hang counted (under `timeout`).
4. **The stop** (S7, S12): today's flag and its wake are `hero_net.c`'s (the platform layer); a
   runtime `receive` answering `stopped` must reach them. Build the route you judge robust (the flag
   moved into the runtime with `hero_net.c` reading it, or the runtime calling the member, or another),
   and measure what each program then links (`nm` of a `print(1)` program before and after).
5. **The runtime's thread, lease and ended-handle machinery under concurrent use** (the original
   question): a handle acquired on one thread and consumed on another; a handle's life moving through a
   mailbox (S11), with `m.send(@v)` emptying `v` as `transfers` does (`runtime/parts/alloc.c`'s dead
   values, `hero_handle_eq_at` at line 1123); a lease taken on one thread (`runtime/parts/lend.c:143-186`
   keeps its regions `_Thread_local`); panel 251's R2 gate at `main`'s end with a worker blocked in
   `receive` (`hero_spawn_check_joined`, `runtime/parts/spawn.c:739`, called from `alloc.c:314`); the 256-slot table
   (`runtime/parts/spawn.c:109`, `HERO_SPAWN_SLOTS 256`) against S1's `limit`. TSan and ASan, counted.

6. **The Windows arm** (`00-shared.md` § 5 question 13): write the queue's wait and its wake on the
   stop for Windows beside the POSIX one (`SRWLOCK`, as `runtime/platform/hero_tls.c:159-200` does for
   its own locks, and a condition variable); its run is owed to the CI's legs or the box.
7. **The routes beside the proposal** (`00-shared.md` § 5, *Routes beside the proposal*): price 7.13's
   own *"one queue per thread"* and its first rung, plain data parallelism (any value across `spawn` and
   `wait`, no mailbox), against the proposal, from your Q1 and Q2 numbers.

## Q3. The gateway-shaped program, route B (`00-shared.md` § 8)

Yours to build and run, both correctness and speed, as § 8 defines them: by the route as far as you
built it in Q1, else its emitted C written by hand as the emitter would write it against your Q2
runtime, **named as an emulation**. The one-thread baseline is the same program with N = 1, or the
program as `examples-rebel/registry/server/loop.hero` shapes it.

## Q4. The rows, and R3

For each row S1 to S14: adopt, amend (the exact spec text), or refuse, each with what you ran. Then R3's
four reasons as you measured them: the cost (Q1's counts against 288/109 and the 30 R3 named), 7.13
(Q2.2), and **whether you read the author's instruction of 2026-11-22 as the ruling on the need
(Principle 0)**, in one line. Your verdict on A as the conservative route, from what you read and ran.

## Your output

`.claude/agents/compiler-engineer.md`'s structure, and `00-shared.md` § 10. Every number with its
command; every file you built named by its path in your copy, so the critic can re-run it.
