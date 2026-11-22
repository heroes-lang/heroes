# Panel 255, the completeness critic, third reading: round two's reports

Written as I go, 2026-11-22/23, on `docs/panel/255-briefs/round2.md` and the two round-two reports
(`compiler-engineer-r2.md`, Linux; `ffi-pragmatist-r2.md`, Darwin). No verdict on the route. Every run
in my copy `.claude/worktrees/p255-scratch/critic/` (detached at `a75179641`), `TMPDIR` inside it; the
seats' files copied there before any run, never run in place. Findings numbered, each with its command
and result.

## How I read and what I ran

Read whole: `255-briefs/round2.md` (156 lines), `compiler-engineer-r2.md` (602 lines) and
`ffi-pragmatist-r2.md` (730 lines), by `sed` ranges. Copied into my copy, `p255/r3/` (`cp`, md5 checked
against each report's): the Darwin seat's six runtimes' `rebind.c`/`sigguard.c` over the tree's
`runtime/` (`rt-mac/{today,C4,C4r,C4f1,C4f3,A2679}`, `md5` `4d39ad4a`, `6b4fcb0b`, `2730f161`,
`3e8a9a34`, `0b0ffcf2`, `f872835c`, each the report's), his `sh/` (`fsh/`), the engineer's three routes
(`ce-routes/{C4,C4fail,C2679}`, four files each, `md5sum` as his) and `shapes/` (`csh/`). `./heroes --version` in my copy reads
`sources 61416c0c33ecc5a9`, the tree's key (the engineer's *today*). Darwin: my copy's stage 2 with
`HEROES_RUNTIME` naming each column (`p255/r3/run-mac.sh`, 3 runs a cell, plain and `--sanitize`, logs
`p255/r3/logs/mac-<column>-<set>.log`). Linux arm64: one container `critic255r3`
(`heroes-linux-arm64:latest`, `--network none`, my copy read-only), stage 1 and stage 2 of the tree, each
column's runtime a `HEROES_RUNTIME` folder (`p255/r3/lin.sh`, logs `p255/r3/out-linux/lin-<column>-<set>.log`).
My own shapes are `p255/r3/mysh/cr.h` and its `.hero` files: **GR** (`gr_<way>_<onstack>`, G resumed after
a seen move below it), **CV** (`cv_<how>_<onstack>_<stack>_<unseen>`, A′ split into the two ways the
engineer's `apu_*` differ from his `ap_*`), **CU** (`cu_<how>_<onstack>`, the Darwin seat's `au_*` with the
handler on the alternate stack too), **AX** (`ax_<onstack>_<stack>_<unseen>`, the Darwin seat's A″ in every
orientation) and, in `mysh/ub.h`, **UB** (`ub_<deep>_<onstack>`, the tree's inline jump out of a handler with
a shallow or deep callback). Two variants of the engineer's C4 are mine, each one line: `ce-routes/C4f3`
(contexts forced onto A too, finding 8) and `ce-routes/C4np` (no overwrite proof, finding 13); they are
instruments, not routes proposed.

## Findings

### 1. The two tables diffed row by row: on the 62 rows both name, the C4s split on G's three own-stack forms alone

*Command*: the Linux table (`compiler-engineer-r2.md` lines 246 to 317) and the Darwin table
(`ffi-pragmatist-r2.md` lines 347 to 445) cut by `sed` into `p255/r3/cmp/{lin,mac}.md`, each cell normalised
to run / S / U / K / crash per mode by `p255/r3/cmp/cmp.py` (`python3 -I`), the engineer's names mapped to the
Darwin seat's where the shape is the same (`n`=`nn`, `fp`=`fp1`, `fpp`=`fp2`, `g_1_0`=`g1_0`, `g_1_1`=`g1_1`,
`g_2_0`=`g2`, `g_3_0`=`g3`, read in `r2new.h` and `xn.h`: the same form and orientation each).
*Result*: 68 Linux rows, 90 Darwin rows, **62 shared**. Column *C4* differs on **three**: `g_1_0`/`g1_0`,
`g_2_0`/`g2`, `g_3_0`/`g3` (Linux runs, Darwin 134 S), and on `f`, `fp`, `fpp` under `--sanitize` only
(Linux exit 1, LeakSanitizer on the shapes' own `calloc`, `xf.h:25`; Darwin's AddressSanitizer reports no
leak), which is a sanitizer's difference and not a route's. Linux's *C4 forced* against Darwin's *forced,
the `setjmp` family* (`C4f1`, the same instrument, below) differs on those G rows (Linux runs, Darwin S) and
on `x_fake` under `--sanitize` (Linux 134 U, Darwin runs). Every other shared row reads the same in both C4
columns and both forced-key columns.
Rows written on one platform only: Darwin's `as_0`, `as_1`, `cs_0`, `cs_1`, `fp3`, `rr_0`, `rr_1`, `rr_2`,
`sp_0`, `sn`, the `t_*` (Linux's twins are its `x_*`); Linux's `au_*_1` (the handler on the alternate
stack; Darwin's `au_1`, `au_2` are `nr_unseen_switch` with `nr_on(..., 0)`, the own stack only,
`r2new.h:158`), `g_2_1`, `g_3_1`, `apu_*`. Each is ported and run on the other platform below.

### 2. The Linux C4 runs the Darwin seat's R inside the signal: two MUST BE REFUSED rows the Darwin C4 refuses, failed on Linux

*Command*: the Darwin seat's `rr.h` and `rr_{0,1,2}.hero`, copied unchanged (`p255/r3/fsh/`), built in
`critic255r3` by the tree's stage 2 (`sources 61416c0c33ecc5a9`) against each column's runtime, the
engineer's files byte for byte (`md5sum`: C4 `sigjump.c` `6c349919`, `siginstall.c` `fd3606f0`, `sigguard.c`
`893d1703`, `siglookup.c` `ba422681`, as his `routes/C4/`), plain and `--sanitize`, 3 runs each
(`p255/r3/out-linux/lin-<column>-f.log`).
*Result* (Linux arm64, every column the same):

| | today | **C4** | C4 forced | C, 2679 alone | Darwin C4 (the seat's, re-run here) | Darwin C4r |
|---|---|---|---|---|---|---|
| `rr_0` (sigsetjmp in the handler, out, jumped back in) | `answer ran for 80`, 139 / ASan SEGV | **the same** | the same | the same | 134 U | 134 U |
| `rr_1` (getcontext spelling) | `answer ran for 81`, 139 / ASan SEGV | **the same** | the same | the same | 134 U | 134 U |
| `rr_2` (the jump back from a heap coroutine) | `answer ran for 82`, 139 / ASan stack-overflow | **the same** | the same | the same | **ran 82**, then 134 / ran 82, exit 0 | **ran 82**, exit 0 |

Raw line, `lin-C4-f.log`: `C4 rr_0 plain 3 exit=139 | answer ran for 80|` and `C4 rr_0 sanitize 1 exit=1 |
answer ran for 80| | ==10968==ERROR: AddressSanitizer: SEGV`. **The Heroes function runs inside the resumed
handler under the Linux C4, then the process faults** (the handler returning through frames the runner's
jump wrote over). The Darwin seat wrote R as MUST BE REFUSED (`rr.h`'s header comment) and closed `rr_0` and
`rr_1` on Darwin by a rule the Linux C4 does not carry (*a jump that lands BELOW the jumping frame on the
range that frame stands on lands in frames already unwound*, `ffi-pragmatist-r2.md` "Two repairs", 22:14;
`rt-mac/C4r/parts/rebind.c`, `hero_rebind_reenter`). By reading `ce-routes/C4/siginstall.c`
`hero_sig_jumped` (lines 392 to 422): a seen jump with no entry standing does nothing (`own->depth == 0`
returns at once), so the jump back is not seen as a landing at all. **So rule 8 fails on `rr_0` and `rr_1`
(Darwin refuses, Linux runs inside the signal) and the Linux C4 fails two MUST BE REFUSED rows**; shared
with Linux today (no row moves toward running against today), not closed. `rr_2` is a residue of BOTH C4s,
shared with today on both platforms. The engineer's report never names R (it was written on Darwin only);
his verdict's own condition (*"a context case of the landing running inside the signal"*) is met by it.

### 3. The Darwin seat's other new shapes on Linux: six more rule-8 splits beyond G, four of them correct programs Darwin refuses and Linux runs

*Command*: as finding 2, every new shape of the Darwin seat (`p255/r3/fsh/`), and the same shapes re-run on
this Mac under its today, C4 and C4r (`p255/r3/logs/mac-<column>-f.log`); the cells by `p255/r3/cells.py`
into `p255/r3/cmp/fsh-both.md`. G's alternate-stack forms 2 and 3, which Darwin did not write, added as
`g2_1`, `g3_1` (the seat's own `nr_g` with `onstack: 1`).
*Result* (R*n* the program ran, exit 0; S, U, K a refusal's words; each cell both modes unless split):

| shape (its MUST) | Linux today | **Linux C4** | Linux C4 forced | Linux 2679 alone | Darwin today | **Darwin C4** | Darwin C4r |
|---|---|---|---|---|---|---|---|
| `as_0` (correct, by c_*'s reasoning) | R51 | **R51** | R51 | R51 | R51 | **S** | S |
| `cs_1` (correct, c_* by `setcontext`, alternate stack) | R90 | **R90** | R90 | R90 | K | **S** | S |
| `fp3` F‴ (correct) | R43 | **R43** (`--sanitize` exit 1: LSan, the shape's own `calloc`) | the same | the same | R43 | **S** | S |
| `sp_0` (correct, MUST RUN) | R95 | **R95** | R95 | R95 | R95 | **S** | R95 |
| `g1_0`, `g2`, `g3` (G, own stack) | R61, R62, R63 | **run** | run | R61, R62, **S** | R61, R62, U | **S** | S |
| `g1_1`, `g2_1`, `g3_1` (G, alternate stack) | R61, -, - | **S** (g1_1) | S | R61 | R61, R62, U | **S** | S |
| `as_1` (MUST BE REFUSED) | K | S | S | K | K | U | U |
| `ap_*`, `au_*`, `nn` (MUST BE REFUSED) | `ap_1_*`, `ap_2_0`, `au_1`, `nn` run | **all refused** | refused | as today | refused or `nn` runs | refused | refused |
| `cs_0`, `fp1`, `fp2` (MUST RUN) | run | run | run | run | run | run | run |
| `sn` | 134 / ASan, a NULL context jumped through by glibc (the seat's note: Darwin-only as written) | the same | the same | the same | R72 (seat) | R72 (seat) | - |

**On the union of both seats' shapes the two C4s now disagree on nine rows, not three**: G's own-stack forms
(three), `as_0`, `cs_1`, `fp3`, `sp_0` (correct programs Darwin's C4 refuses and Linux's runs; `sp_0` agrees
with C4r), and `rr_0`, `rr_1` (finding 2, the other way). **The safe direction is not one side's**: on `rr_*`
it is Darwin's; on `as_0`, `cs_1`, `fp3` the Linux C4 runs correct programs Darwin refuses, and whether its
reason for running them (the cross-stack leave of `hero_sig_cross_left`, a carved range judged by which side
lies above) also runs a MUST BE REFUSED twin is finding 4's question.

### 4. A MUST BE REFUSED shape the Linux C4 runs inside the signal where today REFUSES it: the A″ spelling with an unseen save

*Command*: my `ax_<onstack>_<stack>_<unseen>` (`p255/r3/mysh/cr.h`, `ax_run`): the Darwin seat's A″ (`as_1`:
a coroutine saves a buffer and swaps back; the handler saves its own context, `siglongjmp`s INTO the
coroutine; the coroutine `setcontext`s back into the handler; the handler, running again inside its signal,
calls the program) in every orientation, the handler's save made by `getcontext` (unseen 0) or by glibc's own
`getcontext` reached by `dlvsym(RTLD_NEXT, "getcontext", "GLIBC_2.17")` (unseen 1, the engineer's `apu_*`
technique). MUST BE REFUSED, every one. Built and run as finding 2 (`p255/r3/out-linux/lin-<column>-m.log`).
*Result* (Linux arm64, both modes, 3 of 3):

| `ax_` handler stack / coroutine stack / save | today | **C4** | C4 forced | C, 2679 alone |
|---|---|---|---|---|
| `0_0_0` own / an array of the own stack / seen | R53 | S | S | R53 |
| `0_0_1` own / own array / **unseen** | **R53** | **R53** | R53 | R53 |
| `0_2_0` own / heap / seen | S | S | S | S |
| `0_2_1` own / heap / **unseen** | **134 U** | **R53: runs inside the signal** | R53 | 134 S |
| `1_0_0`, `1_2_0` alternate / seen | K | S | S | K |
| `1_0_1`, `1_2_1` alternate / unseen | K | K | K | K |

Raw, `lin-C4-m.log`: `C4 ax_0_2_1 plain 3 exit=0 | answer ran for 53|ax_0_2_1: 530|`; `lin-today-m.log`:
`today ax_0_2_1 plain 3 exit=134 |  | U`. **`ax_0_2_1` is a row that moves toward running a Heroes function
inside a signal against today, under the Linux C4 and its forced column**, which the Linux C4's verdict says
never happens (*"moves no program toward running inside a signal"*, `compiler-engineer-r2.md` § 7, lines 534 and
535; his *C, 2679 alone* verdict says the same of its own column and holds: it refuses the row). By
reading `hero_sig_cross_left` (`ce-routes/C4/siginstall.c:363`): an entry on the own stack is left by a jump
to a known range that is not carved from it (the heap coroutine), unless marked resumable, and the mark
needs a SEEN `getcontext` (`hero_sig_context_saved`, called from the `getcontext` wrapper only,
`sigjump.c:649`). Today refuses it in U words (its record of the coroutine's buffer); C, 2679 alone (today's
`siginstall.c` with C's decode) refuses it, in S words. `0_0_1` runs under every Linux column: a residue shared with today, the
`siglongjmp` spelling of the engineer's own `apu_*` residue (which he names as open, § 3.6). Darwin's half is
finding 7.

### 5. The overwrite proof that runs G's own-stack forms on Linux is not a proof: G resumed after it runs a Heroes function inside the signal

*Command*: my `gr_<way>_<onstack>` (`p255/r3/mysh/cr.h`, `gr_run`): G's form 1 (the handler `swapcontext`s to
the runner's pre-signal context), the signal raised 16 KiB below the runner so nothing the runner does next
writes over the handler's frames; then ONE seen move 64 KiB below them, through an `alloca` written only at its
bottom (way 0: a `sigsetjmp`/`siglongjmp` pair on a static buffer; way 1: the program called, G's own
callback; way 2: nothing, the control); then the runner `setcontext`s into the handler's own swap; the handler,
running again inside its signal, calls the program (`answer ran for 70 + way`), then leaves by `siglongjmp`.
That last call is MUST BE REFUSED by the reasoning that makes `rr_*` so. Both platforms, every column.
*Result* (both modes, 3 of 3):

| `gr_` | Linux today | **Linux C4** | Linux C4 forced | Linux 2679 alone | Darwin today | **Darwin C4** | Darwin C4r | Darwin A |
|---|---|---|---|---|---|---|---|---|
| `0_0` a seen jump below, own stack | R70 | **R70: inside the signal** | R70 | R70 | R70 | S | S | U |
| `1_0` G's callback below, own stack | R69, R71 | **R69, then R71 inside the signal** | the same | the same | R69, R71 | S | S | U |
| `2_0` no move below (control) | R72 | **S** | S | R72 | R72 | S | S | U |
| `*_1` the handler on the alternate stack | K (`1_1`: R69 then K) | S | S | K | K | S | S | U |

Raw, `lin-C4-m.log`: `C4 gr_0_0 plain 3 exit=0 | answer ran for 70|gr_0_0: 700|` and `C4 gr_2_0 plain 3
exit=134 |  | S`. **The control and the shape differ by the seen move below alone**: it fires
`hero_sig_overwritten` (`ce-routes/C4/siginstall.c:348`, called from `hero_sig_jumped`, line 403, at every
seen jump's target), which removes the suspended handler on the premise *"whatever runs at `at` grew there
through the handler's frames: they are overwritten and it can never be resumed"*. A frame below is not a write
over the frames between (an `alloca`, a large local, a callee's untouched red zone); the handler's frames are
intact and its swap's context resumes it. **So the rule that buys `g_1_0`, `g_2_0`, `g_3_0` on Linux also
runs `gr_0_0` and `gr_1_0` inside the signal; Darwin's C4, which has no such rule, refuses every G form and
every `gr`.** Not new against today (Linux and Darwin today run both); not closed by the Linux C4; closed by
the Darwin C4. The safe direction on G is Darwin's, as the engineer's own fallback says (*"drop the overwrite
proof: `g_*_0` refused too, and nothing else of the union moves"*, § 4: by reading, unrun by him; the Darwin
column is that design measured).

### 6. The engineer's residue `apu_*` is the unseen save alone; his description of it is not his code's

*Command*: `grep -n 'char stack' csh/xn2.h csh/xn.h`; then my `cv_<how>_<onstack>_<stack>_<unseen>`
(`cr.h`, `cv_run`): A′ with the handler's save seen or unseen and the coroutine's stack 0 an array of the
runner's live frame, 1 a static array, 2 the heap, every combination (24 programs), both platforms.
*Result*: `xn2.h:29` declares `_Alignas(16) static char stack[...]`: **`apu_*_0`'s coroutine stack is a
static array**, not the *"own-stack array"* his table says (`compiler-engineer-r2.md:350`); `xn.h:33`
(`ap_*`) is the own-stack array. Measured on Linux (`lin-<column>-m.log`): **every unseen combination with
the coroutine calling (how 1) runs inside the signal under the Linux C4, whatever the stack** (`cv_1_*_*_1`:
R31 ×6), and the handler-calls form on the own stack too (`cv_2_0_*_1`: R32 ×3); every SEEN combination is
refused, S (12 of 12). Today runs 18 of the 24 (all but the six alternate-stack how-2 rows, K): the nine
unseen ones C4 runs, and the nine seen ones C4 refuses; so the residue is the unseen save alone and is shared
with today, as he says; his `apu_*_0` would read the same with an own-stack array
(`cv_1_0_0_1` R31). On Darwin every `cv` is refused under today (U), C4 and C4r (S), 24 of 24, both modes
(`mac-<column>-m.log`): Darwin's rule 6 holds every entry off a known range at a `setcontext` into one, and
needs no save to be seen. **So rule 8 fails on A′ with an unseen save: Linux runs it inside the signal,
Darwin refuses it**; the two rule-6s are different rules (Linux: *held only if a `getcontext` was seen
inside the handler*; Darwin: *held on every `setcontext` into a coroutine's range*), and the Darwin one is
the one that closes the residue, at the price of `cs_1` (finding 3).

### 7. Finding 4's shapes on Darwin, and the engineer's by-reading cost of an unseen exit, measured on both

*Command*: `ax_*` and my `ub_<deep>_<onstack>` (`p255/r3/mysh/ub.h`: the tree's own inline jump out of a
handler, `fixedbugs-2522-an-inline-jump-out-of-a-handler.h`'s machine part copied unchanged, then the program
called SHALLOW, from the runner's frame above the handler's, or deep, 256 KiB below as the tree's case; the
handler on the own or the alternate stack) on this Mac under today, C4, C4r and A
(`p255/r3/logs/mac-<column>-m2.log`, `mac-A2679-m.log`) and in `critic255r3` (`lin-<column>-m.log`).
*Result*, `ax_*` (MUST BE REFUSED), both modes, 3 of 3:

| `ax_` | Darwin today | Darwin A (2679 alone) | **Darwin C4**, C4r | Linux today | **Linux C4** |
|---|---|---|---|---|---|
| `0_0_0`, `0_0_1` own / own array | **R53** | **R53** | **S** | R53, R53 | S, **R53** |
| `0_2_0`, `0_2_1` own / heap | **R53** | **R53** | **S** | S, U | S, **R53** |
| `1_*` alternate stack | K | K | U | K | S or K |

So **today's Darwin runs all four own-stack A″ inside the signal** (a residue neither seat's table has; the
seat's `as_1` is the alternate-stack orientation, K today) and **Darwin's C4 closes all four**; Darwin's
2679-alone (A) keeps them as today. The Linux C4 closes one, keeps one, and opens one (finding 4). Rule 8
fails on `ax_0_0_1` and `ax_0_2_1` (Linux runs, Darwin refuses); Darwin's is the safe direction.

`ub_*` (correct: the handler is left for good, by a jump no function of the C library makes):

| `ub_` | Linux today | **Linux C4** | Linux C4 forced, 2679 alone | Darwin today | **Darwin C4**, C4r, A |
|---|---|---|---|---|---|
| `0_0` shallow, own stack | **R81** | **S** | S, R81 | S | S |
| `0_1` shallow, alternate stack | **R81** | **S** | S, R81 | S | S |
| `1_0` deep, own stack (the tree's case) | S | S | S, S | S | S |
| `1_1` deep, alternate stack | **R82** | **S** | S, R82 | S | S |

**The engineer's by-reading cost is real on Linux and absent on Darwin**: the Linux C4 refuses three correct
programs today's Linux runs (`ub_0_0`, `ub_0_1`, `ub_1_1`, in S words that say *"or C left the handler in a way
this runtime does not follow (an inline `__builtin_longjmp` ...)"*, true), because it removed the callback's
prune by its own frame (rule 5); today's Darwin already refuses all four, so both C4s agree (rule 8 holds) and
the cost is Linux's against its own today. Neither seat ran it; neither table lists it.

### 8. The two forced columns are different instruments; forced the same way, Linux fails `b_*` exactly as Darwin does, and Darwin's context word is a public field

*Command*: (a) `grep -n FORCE_UNCALIBRATED -A3 ce-routes/C4fail/sigjump.c`: the engineer's define sets
`cal.key_ok = false` only (`C4/sigjump.c:431` to `433`, the define itself at `C4fail/sigjump.c:76`), so his *C4 forced* is `decoding: 2` (contexts still by
their field), the instrument of Darwin's `C4f1`, not of Darwin's *forced (both)* `C4f3`. (b) I built Linux's
`C4f3`: his `C4fail` with one line more, `cal.ctx_ok = false;` (`p255/r3/ce-routes/C4f3/sigjump.c`, `diff`: one
line), which reaches the context A path his code carries (`hero_ctx_recording`, `C4/sigjump.c:596`) and no define
of his reaches; run in `critic255r3` (`lin-C4f3-c.log`, `lin-C4f3-f.log`). (c) Darwin's context word: a C
program printing `offsetof` on this Mac (`p255/r3/off/off.c`), and for x86-64 the offset read from a compile
error's type (`off86.c`, `clang -arch x86_64 -c`).
*Result*: (b) the accessor prints `decoding: 0` both modes; **`b_0`, `b_1` refused, 134 U, both modes, 3 of
3** (raw: `C4f3 b_0 plain 3 exit=134 |  | U`), as the Darwin seat's `C4f3` (his table: 134 U); `e`, `x_heap0`
refused U; `c_*`, `cj`, `d_*`, `ns_*`, `gcb`, `f` run; every MUST BE REFUSED row of the set refused (`a_*` S,
`ap_*` U, `c2_*` S, `n` S, `x_unseen0`, `x_unseen`, `x_partial` U); G's own-stack forms still run (`g_1_0`
R19). **So the forced column's one MUST RUN failure (`b_*`) is not Darwin's: it is route A's for contexts on
both platforms**, and the Linux table reads it as held only because its define never puts contexts on A.
(c) `arch arm64 sizeof(ucontext_t) 880 uc_mcsize-at 40 uc_mcontext-at 48 mcontext_data-at 64`,
`ss.sp-in-mcontext 264 => byte 0x148 of the context; uc_mcontext points at +0x40`: **the Darwin seat's
`HERO_REBIND_CTX_SP 0x148` is `ucontext_t.__mcontext_data.__ss.__sp`, a field of the SDK's public struct**,
as glibc's `uc_mcontext.sp` is on Linux (and its size word `0x28` is `uc_mcsize`); on x86-64 the same public
field (`__ss.__rsp`) is byte **128** (`char (*)[128]`). The Darwin seat calls Darwin x86-64 *"A for both by
construction"* and the column that fails `b_*`: for the context family that is a choice (no x86-64 context
calibration was written), not the platform's. **A route nobody built**: Darwin x86-64's contexts read and
verified by that public field, as Linux's are, which would keep `b_*` running there. Whether libsystem's
x86-64 `setcontext` loads its stack pointer from that field is **unrun** (no x86-64 libsystem to disassemble
on this Mac, no Rosetta). No CI leg runs Darwin x86-64 (`ci.yml:373`: Linux x86-64, Linux arm64, Darwin
arm64, Windows x86-64), so whatever it does is read by no leg.

### 9. Every row where the two C4s disagree, over the union of both seats' shapes and mine: 22, and which way is safe

*Command*: `p255/r3/judge.py` (`python3 -I`) over every log of this reading (`cells.py`'s cells; a row *runs*
where a cell shows R*n* or a call that ran before a fault), Linux C4 against Darwin C4 and C4r.
*Result*: 22 rows split (the rows of findings 2 to 7), and the direction of each:

| rows | MUST | Linux C4 | Darwin C4 (C4r) | the safe direction |
|---|---|---|---|---|
| `rr_0`, `rr_1` | REFUSED | **runs inside the signal** | refused U | Darwin's |
| `gr_0_0`, `gr_1_0` (its second call) | REFUSED | **runs inside the signal** | refused S | Darwin's |
| `ax_0_0_1`, `ax_0_2_1` | REFUSED | **runs inside the signal** (`0_2_1` against today) | refused S | Darwin's |
| `cv_*` with an unseen save, 9 rows | REFUSED | **runs inside the signal** (as today) | refused S | Darwin's |
| `g1_0`, `g2`, `g3` (G, own stack) | the author's | runs (by the proof of finding 5) | refused S | Darwin's, unless the author rules G runs; then no design here runs it without `gr` |
| `as_0`, `cs_1`, `fp3` | RUN (correct) | runs | **refused S** | Linux's is correct; Darwin's is a told refusal |
| `sp_0` | RUN (correct) | runs | refused S (**C4r runs**) | agree under C4r |

**15 of the 22 are MUST BE REFUSED rows the Linux C4 runs and the Darwin C4 refuses; none goes the other
way.** Rule 8 is not met by the two C4s as built, and on every MUST BE REFUSED split it is the Linux half that
fails: its rule 6 (held only where a save was seen), its overwrite proof, and the absence of Darwin's landing
rule. The three correct programs the Darwin C4 refuses and Linux runs (`as_0`, `cs_1`, `fp3`) are the price of
the rules that close those 15 on Darwin (finding 6's rule 6; the target judged on a coroutine first, its
report's rule 4).

### 10. Both seats' MUST claims hold on my re-run of their decisive rows; C4r and C4 differ on `sp_0` alone

*Command*: the seat's runtimes and shapes copied (`md5` as his), built by my copy's stage 2, 3 runs a cell
(`p255/r3/logs/mac-{today,C4,C4r}-f.log`, 43 shapes).
*Result*: every MUST BE REFUSED row he lists refused (`a_*` S, `ap_*` S, `au_1`, `au_2` S, `nn` S, `c2_*` S,
`rr_0`, `rr_1` U, `as_1` U, `x_unseen0`, `t_unseen0` S), every MUST RUN row run (`b_*` R3, `c_*`, `cj` R9,
`d_*` R4, `f` R6, `ns_*` R7, `gcb`, `fp1` R41, `fp2` R42, `cs_0` R90), both modes, under C4 and C4r alike;
`sp_0` S under C4 and R95 under C4r; `rr_2` runs under both (C4: *ran 82* then 134 plain, exit 0 sanitized;
C4r: exit 0 both), his named residue, reproduced. Today's verdicts read as his table. Not re-run: the case
sets, the `runtime` suite, libpng, the `x_*` and `jo_*` rows, `ij*`.
**The engineer's decisive rows hold too** (`lin-<column>-c.log`, his `shapes/` copied, his runtimes
`md5`-checked, 44 programs): the accessor `decoding: 3` (C4), `2` (forced), `3` (2679 alone), today
`ffi_missing_link`; `a_*`, `ap_*`, `au_*`, `n`, `c2_*`, `x_unseen`, `x_unseen0`, `x_partial` refused S under
C4; `b_*`, `c_*`, `cj`, `d_*`, `e`, `f`, `fp`, `fpp`, `ns_*`, `gcb` run (the three `f` shapes exit 1 under
`--sanitize` after the whole output: LeakSanitizer, as he says); `g_*_0` run, `g_*_1` S; `apu_1_0`, `apu_1_1`,
`apu_2_0` run inside the signal under C4 as under today, `apu_2_1` K: his residue, reproduced. His table
reads as measured; what it lacks is the shapes it never ran (findings 2 to 7).

### 11. *C, 2679 alone*: on neither platform does it move a row toward running inside a signal, over every shape of this reading

*Command*: `judge.py`'s second part: every row where Linux's C2679 or Darwin's A runs a program its own
platform's today refuses.
*Result*: **none on either platform**, over the Darwin seat's new shapes, my `gr`, `ax`, `cv`, `cu`, `ub` and
the decisive union rows. Linux's C2679 refuses rows today runs (`g3` S, as the engineer's `g_3_0`; `cv_1_1_1_*`,
`cv_2_0_1_*`, `cv_2_0_2_*` S; `ax_0_2_1` U becomes S); Darwin's A refuses `gr_*` (U) where today runs
`gr_0_0`, `gr_1_0`, `gr_2_0`, and keeps today's `ax_0_*` (R53, four rows), `nn`, `rr_*`, `t_unseen0`,
`x_unseen0` running. The Linux C4 and its forced column are the only columns of this reading that move a row
toward running against today (`ax_0_2_1`, finding 4); the Darwin C4 and C4r move none.

### 12. The realistic failed calibration on Darwin (the `setjmp` family alone on A) runs all four own-stack A″ inside the signal

*Command*: the Darwin seat's `C4f1` (`HERO_REBIND_FORCE_UNCALIBRATED 1`, `md5` `3e8a9a34`, *"a libsystem that
changes its munge, contexts still decoded"*), `C4f3` (both families), on my shapes
(`p255/r3/logs/mac-C4f{1,3}-m.log`); Linux's forced column as finding 4.
*Result* (both modes, 3 of 3; raw: `C4f1 ax_0_2_1 plain 3 exit=0 | answer ran for 53|`):

| | Darwin C4 | **Darwin forced, `setjmp` family** | Darwin forced, both | Linux forced (key) | Linux forced, both (mine) |
|---|---|---|---|---|---|
| `ax_0_0_0`, `ax_0_2_0` (seen save) | S | **R53** | U | S | U |
| `ax_0_0_1`, `ax_0_2_1` (unseen save) | S | **R53** | U | **R53** | **R53** |
| `ax_1_*` | U | U | U | S or K | U or K |
| `gr_*` | S | S | U | `gr_0_0`, `gr_1_0` **run** | `gr_0_0`, `gr_1_0` **run** |
| `cv_*` (8 of 24 run here) | S | S | U | the unseen ones run | - |

The seat's sentence *"With the setjmp family alone forced ... the whole forced column holds, `b_*`
included"* is true of his union and false of A″: under the failure he calls realistic, route A follows the
handler's `siglongjmp` INTO the coroutine by its record and the coroutine's `setcontext` back into the
handler is not caught, as today (today runs all four, finding 7). So **the brief's rule 3 (*"Rules 4 to 7 bind
the A path too"*) is not met on Darwin for A″**, and the forced columns of the two platforms disagree on
`ax_0_0_0`, `ax_0_2_0` (Darwin runs, Linux refuses) and `gr_*` (Linux runs, Darwin refuses). Not new against
today on either platform.

### 13. The engineer's fallback, built: the Linux C4 without its overwrite proof refuses every G and every `gr`, and moves nothing else run

*Command*: `p255/r3/ce-routes/C4np/`: his C4 with one line, `if (1) return;` at the top of
`hero_sig_overwritten` (`diff`: one line); run in a second container `critic255r3b` (my first container's
state committed to a temporary image, my copy mounted read-only), 38 programs, both modes, 3 runs
(`p255/r3/out-linux/lin-C4np-{c,f,m}.log`).
*Result*: `g_1_0`, `g_2_0`, `g_3_0`, `g1_0`, `g2`, `g3` **refused S** (C4: run); `gr_0_0`, `gr_1_0` **refused S**
(C4: run inside the signal); every other row as under C4: `a_*`, `ap_*`, `au_1_*`, `n`, `c2_*`, `x_unseen0`,
`x_partial` refused; `b_*`, `c_*`, `cj`, `d_*`, `e`, `f`, `fp`, `fpp`, `fp1` to `fp3`, `ns_*`, `gcb`, `cs_0`,
`cs_1`, `sp_0`, `as_0` run; `rr_0` still runs inside the signal (finding 2 is another rule). **His sentence
(*"drop the overwrite proof: `g_*_0` refused too, and nothing else of the union moves"*, by reading) holds on
every row run here**, and it is Darwin's G verdict: with it, G reads the same on both platforms (all refused)
and `gr` closes on Linux.

### 14. What this reading did not run, said

Every cell of this reading is in `p255/r3/cmp/all.md` (`cells.py` over every log; on Linux the names `ap_*`,
`b_*`, `c_*` and their kin were run from both seats' folders and their cells are merged there, both
refused or both run under every C4 column). **Unrun**: x86-64 on either platform (the new shapes' switch is
arm64 assembly; `gr`, `ax`, `cv`, `ub` are portable C and were not built for x86-64); Ubuntu's glibc 2.43; the
case sets and the `runtime` suite under any column (both seats ran them; nothing here moves them); libpng;
`ij*`; Darwin's `cv` under A and the forced columns beyond 8 of 24; the Linux C4 with Darwin's landing rule or
Darwin's rule 6 (the repairs findings 2, 4 and 6 point at: no route of mine, the seats' to build); whether
libsystem's x86-64 `setcontext` reads `__ss.__rsp` at byte 128 (finding 8). Load on this Mac was 11 to 136
through the runs (other sessions' work beside mine): no timing here is a resolution's ground. Containers: two
started (`critic255r3`, `critic255r3b`) and one temporary image (`critic255r3-img:tmp`), all removed
(`docker ps -a` and `docker images` list none of them); `lane-ui-gtk-box` untouched.

## What the synthesis must settle

1. **Rule 8 is not met, and on every MUST BE REFUSED split it is the Linux half that runs the program inside
   the signal** (finding 9: 22 rows split, 15 of them MUST BE REFUSED rows the Linux C4 runs and the Darwin C4
   refuses, none the other way). The Linux C4's approval rests on a union that lacked R, `gr`, A″ and the
   unseen `cv`; its own stated condition (*"a context case ... running inside the signal"*) is met. Before any
   route is adopted the Linux half must carry, built and run on Linux, Darwin's three rules or their
   equivalents: the landing rule (a jump landing below the jumping frame on its own range, finding 2), rule 6
   as Darwin reads it (every `setcontext` or jump into a coroutine's range holds the standing handlers, with no
   save needing to be seen, findings 4 and 6), and no overwrite proof (finding 5, built as `C4np` in finding
   13). A route that does not build is not adopted (`round2.md`).
2. **The Linux C4 moves a MUST BE REFUSED row toward running inside a signal against today**: `ax_0_2_1`,
   today 134 U, C4 and its forced column `answer ran for 53` (finding 4). The Linux C4's verdict says no program moves
   that way (§ 7); the synthesis cannot carry that sentence for C4.
3. **G, put to the author with what it costs**: on Linux G's own-stack forms run only through the overwrite
   proof, which also runs `gr_0_0` and `gr_1_0` inside the signal (finding 5); without it G is refused on
   both platforms and nothing else run moves (finding 13). The author's question is therefore *"G refused on
   both platforms"* against *"G run, and `gr` with it"*, not G by stack.
4. **The correct programs each design refuses that today runs**, for the author as ruling B was: Darwin's C4
   (and C4r) `as_0`, `fp3`, `g1_0`, `g1_1`, `g2`, `g2_1`, and `cs_1` (refused today by K too); C4 alone `sp_0`
   (C4r runs it, as Linux does); the Linux C4 `g_*_1`, and **`ub_0_0`, `ub_0_1`, `ub_1_1`** (a callback after a
   handler left by an inline jump, which today's Linux runs: finding 7, named by the engineer by reading,
   now measured); under a failed calibration A's listed costs, plus `b_*` and `cs_0` where contexts fail too
   (on both platforms, finding 8). If the Linux half takes Darwin's rules (item 1), Linux pays `as_0`, `cs_1`,
   `fp3` too: unrun, the synthesis's to measure.
5. **The residues that stay, each shared with today**: `rr_2` under both C4s (R across ranges; the Darwin
   seat's per-range *bottom*, unbuilt); the A″ residue under Darwin's realistic failed calibration
   (`C4f1`: all four own-stack `ax_0_*` run inside the signal, finding 12, so rule 3's *"Rules 4 to 7 bind the
   A path too"* is not met there); on Linux, until item 1 lands, `rr_0`, `rr_1`, `gr_0_0`, `gr_1_0`,
   `ax_0_0_1` and the nine unseen `cv`. The context defect the coordinator files must list them as its open
   rows, and their shapes (`rr.h`, `cr.h`) as its goldens on both platforms.
6. **The forced column is one instrument on both platforms only if the define reaches both families on
   both**: the Linux define forces the key only; forced on contexts too, Linux refuses `b_*` exactly as Darwin
   (finding 8). And Darwin's context word is the public `__mcontext_data.__ss.__sp` (0x148 on arm64; byte 128
   on x86-64), so Darwin x86-64's *"A for both by construction"* is a choice for its context family: a context
   decode by that field there (as Linux's) is a route nobody built, and no CI leg reads Darwin x86-64.
7. ***C, 2679 alone* moves no row toward running inside a signal on either platform** over every shape of this
   reading (finding 11): Linux's C can land first as measured; Darwin's A refuses `b_*` and eleven other
   correct programs today runs, and the Darwin seat objects to it. Which platform lands what first is the
   synthesis's, on these columns.
8. **The goldens owed**: the engineer's golden list (§ 8) carries none of the shapes that split the two C4s
   (`rr_*`, `as_*`, `cs_*`, `fp3`, `sp_0`, `gr_*`, `ax_*`, the unseen `cv`, `ub_*`); the Darwin seat's carries
   `rr_*`, `as_*`, `cs_*`, `fp3` and not `sp_0`; neither carries `gr_*`, `ax_*`, the unseen `cv` or `ub_*`,
   which no seat wrote. And the two seats' same names are
   different programs (`ap_*`, `au_*`, G's forms: `xn.h` against `r2new.h`). The landing's union needs ONE
   header per shape, run on every leg under one `.expected`, which is rule 8 made executable.
