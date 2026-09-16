# Panel 156 — compiler-engineer report

Seat: the ceiling (design.md §1.1, §1.7, Part 5). Veto on soundness.
Every number below was run on this Mac (darwin arm64, 2026-09-16) in a
scratchpad copy of the tree. The repository working tree was not modified
except for this file.

## R2 — THE HINGE IS SETTLED, AND IT IS NONE OF THE FOUR CANDIDATES

The arm64 frame walk is **not** broken. `hero_stack_regs` extracts the right
register, `hero_stack_lo`/`hi` are right, `dladdr` answers, the chain walks to
its end. Instrumented copy of `runtime/parts/stack.c`, fixture at -O0, stderr:

    DBG pc=0x1045427a8   fp=0x16b8c6520
    DBG lo=0x16b0cc000   hi=0x16b8c8000   lo_minus_window=0x16afcc000
    DBG pc_sname=node_value
    DBG iter=0  fp=0x16b8c6520  next_fp=0x16b8c6550  ret=0x1045427e4  ret_sname=main
    DBG iter=1  fp=0x16b8c6550  next_fp=0x16b8c6bb0  ret=0x1802004e4  ret_sname=start
    DBG iter=2  fp=0x16b8c6bb0  next_fp=0x0         ret=0x0          break: ret==0

The Heroes frame is not in the chain. **Why: on AArch64 a leaf function saves
no frame record.** `otool -tvV -p _node_value`:

    10000a79c  sub sp, sp, #0x10
    10000a7a0  str x0, [sp, #0x8]
    10000a7a4  ldr x8, [sp, #0x8]
    10000a7a8  ldr x0, [x8]        <- the fault
    10000a7ac  add sp, sp, #0x10
    10000a7b0  ret

No `stp x29, x30`. At the fault x29 still points at **h_main_main's** frame,
whose saved return address is in C's `main` (`nm -n`: `_h_main_main` T a6f0,
`_node_value` t a79c, `_main` T a7b4; ret 0x…a7e4 is inside `_main`). So the
walk asks *who called h_main_main*, never *who is h_main_main*. On x86-64 the
`call` instruction pushes the return address unconditionally, so the same walk
lands one frame lower and finds `h_main_main`. **x86-64 is right by accident of
ISA, not by design, and the same walk on Linux arm64 would fail identically.**
The missing datum is the link register x30.

Verdict R2: **defect**, and it is a two-platform defect, not a macOS one.

## The repair, built and run: +7 net lines, all in the runtime

`runtime/parts/stack.c` (622 lines) only. `hero_stack_regs` gains a fourth
out-parameter `lr` — `uc->uc_mcontext->__ss.__lr` on Darwin arm64,
`uc->uc_mcontext.regs[30]` on Linux arm64, zero on both x86-64 arms and on the
unknown arm; `hero_stack_blame` gains the parameter and probes
`dladdr(lr - 1)` once before the loop. **13 lines added, 6 removed, net +7**,
counted by `diff`. Zero lines in `selfhost/` (62,134 lines of `.hero`).

Result on the fixture: exit 134, stdout `7`, stderr
`… at offset 0x0, called from main.main`. macOS now says what Linux says.

Attacked at the adjacent shapes, all run:
- **non-leaf faulting C function** (a `node_value` that calls a helper first,
  so x30 is saved and clobbered): `called from main.main`. Correct. lr then
  points inside the faulting function itself, which `hero_stack_is_heroes`
  rejects, and the ordinary loop answers.
- **stack exhaustion unaffected**: `surface-fixtures/deep` still
  `panic: stack exhausted in main.down`, `wideframe` still `in main.descend`
  at both -O0 and -O2, exit 134.
- **-O2 on the nullread fixture gives a THIRD answer on this same Mac**:
  `called from main`. `h_main_main` is inlined into C's `main`, so no Heroes
  symbol exists to name. The harness row already pins -O0, which is what makes
  it pass; **no blame name may be pinned without its -O level.**

## R1 — the Heroes caller, and the "both names" route is not free

The Heroes caller is right: it is the author's own line, and the handler's own
comment already says so (`runtime/parts/stack.c:451-455`). `node_value` was
never an answer — `hero_stack_blame` returns it as `first`, the fallback for
*the walk found nothing*, so the green leg has been asserting the failure mode.

**The route nobody priced — naming both — is priced here and it is NOT free.**
Its C half rests on `dladdr` resolving a **local** symbol: `_node_value` is `t`
in `nm -n`, not `T`, and Darwin's `dladdr` returned it anyway. Whether glibc's
does is **UNRUN** — glibc documents searching the dynamic symbol table, where a
`static inline` function does not appear. If it does not, "both" yields two
names on macOS and one on Linux, which is the opposite of dissolving R1.
Cost if it is symmetric: ~+6 further lines in `hero_stack_blame` (return two
pointers) plus the message. I object to adopting it before that is run.

## R3 — keep what was printed, by buffering, not by flushing

**fflush on the abort path is not safe and the deadlock is concrete here.** The
handler is entered synchronously on the faulting thread; a stack exhaustion can
occur *inside* a stdio call, and that thread then holds stdout's lock. A hang in
CI is worse than a lost line, and §1.12's promise is about crashing and
corruption, not about hanging — the document does not cover a hang, and I say so
rather than invent a rationale.

**The repository already decided this shape once.** `runtime/parts/os.c:63-72`,
`hero_err_unbuffered`, in its own words: *"`_IONBF` rather than an `fflush` at
each of the six abort sites: a call that has to be remembered at every exit is a
call that will be forgotten at the seventh. This runs once, before a program's
first line."* It is called from `hero_args_set` (os.c:82). The stdout repair is
**one line in that function**, `runtime/parts/os.c` (293 lines).

Measured, 200,000 `print` calls, stdout redirected to a file, `/usr/bin/time -p`
`real`, three runs each, steady state:

| stdout mode | real | vs default |
|---|---|---|
| default (fully buffered) | 0.01 s | — |
| `_IOLBF` | 0.23 s | ~15x |
| `_IONBF` | 0.43 s | ~29x |

Output byte-identical in all three (`cmp`).

**Recommend `_IOLBF`.** `print` always ends with `hero_print_end()` →
`putchar('\n')` (`runtime/parts/panic.c:34`), so a line-buffered stream loses
nothing that `print` wrote, at half `_IONBF`'s price. `_IONBF` buys only
protection for a partial line no `print` can produce.

## R4 — a defect; the arm can be written, only the measurement waits

Read from source, not run: `runtime/parts/stack.c:584-585` tests
`ExceptionCode == EXCEPTION_ACCESS_VIOLATION && ExceptionAddress == 0`, and
`ExceptionAddress` is the PC. Nothing reads `ExceptionInformation[1]`. The third
arm mirroring the POSIX side is ~8 lines in the same file. I hold no box, so the
confirmation is the ffi seat's. Note Windows has no frame walk at all
(stack.c:562), so it can deliver the *panic line* but not the caller name.

## R5 — the red stands, and the pin is not what the brief says it is

**There is no `.expected` file.** `find tests/golden/surface-fixtures -name
"*.expected"` returns nothing. The pin is one row,
`tests/harness/suite_surface.hero:283`. `tests/harness/**` is **not** in
`.claude/rules/records.md` § A record is never rewritten — that list names
`tests/golden/`. So editing the row is an ordinary edit, `UPDATE_GOLDEN=1` is
irrelevant to it, and the hard stop is not engaged.

Recommendation: **do not weaken the row; land the repair.** macOS+Linux go green
today for +8 lines total (7 in stack.c, 1 in os.c) plus the row's
`err_has("called from node_value")` → `err_has("called from main.main")`, in one
commit. Windows stays red until R4, which is honest: it is the platform where
§1.12's named condition is live.

## Core or sugar (§1.7)

**Neither — it is not a language construct at all.** §1.7's test asks whether the
type checker, the lowering and the backend must all handle a new form. Here none
of the three changes: 0 lines in the lexer, 0 in the checker, 0 in descriptors,
0 in ownership, 0 in the emitter, 0 in `selfhost/`'s 62,134 lines. All 8 lines
land in `runtime/` (6,466 lines). The ceiling is untouched and Pascal-P4's scale
is not in play.

**Are the three arms diverging in a way that will keep costing?** Yes, and this
sitting is the evidence: the POSIX arm has a frame walk and Windows has none;
the walk's correctness silently depended on an ISA property nobody wrote down.
The cheap countermeasure is a sentence in stack.c naming the assumption
(*the callee's frame record exists*) and which platform pays when it does not.

## Left UNRUN, with the command that would settle each

1. **glibc `dladdr` on a `static` function.** Settles whether "both names" is
   symmetric. `docker run` the Linux image, build the fixture, and print
   `first` — or simply `dladdr` a static function in a 10-line C probe.
2. **The repair on Linux x86-64 and on Linux arm64.** The x86-64 leg should be
   unchanged (lr is 0 there); the arm64 leg should change from whatever it says
   today to `main.main`. `heroes run tests/golden/surface-fixtures/nullread/main.hero -O0`
   in the container from `docs/ref/environment/linux/Dockerfile`.
3. **`_IOLBF` actually saving the `7` on glibc.** macOS flushes on abort anyway,
   so my measurement cannot show the fix working — only that it costs 0.22 s per
   200k prints.
4. **The full net, and the compiler's own tests.** Forbidden by this sitting's
   time budget. `./heroes run tests/harness/main.hero -- ./heroes surface` is the
   narrow one; `.claude/rules/verification.md` says `annotations` and `fixes`
   also read `tests/golden/surface-fixtures/**`.
5. **Windows anything.**

## Verdict

**object** — not veto. No core construct is proposed and the ceiling is not
breached (8 lines of runtime C, 0 of compiler). I object to two things: adopting
"both names" before UNRUN item 1, and pinning any blame name without its -O
level, since -O2 already gives a third answer on this Mac.

**Prediction.** If the +7-line `lr` seed and the one-line `_IOLBF` land and the
row at `tests/harness/suite_surface.hero:283` is changed to
`err_has("called from main.main")`, then at the M-check-completeness close
`git diff --stat` for R1+R2+R3 shows **zero lines under `selfhost/`** and fewer
than 20 lines under `runtime/`, and the `surface` suite is green on macOS and
Linux and still red on Windows.

**Condition.** A glibc run showing `dladdr` resolving a `static` function's
name would make "both names" symmetric and I would then support it, at ~+6
further lines in `hero_stack_blame`. A measured program where the `lr` probe
names a Heroes function that is *not* an ancestor of the fault would sink the
repair and I would veto it.
