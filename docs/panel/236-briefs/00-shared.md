# Panel 236, shared brief: a group record off the frame, between Heroes functions and handed to C

**Convened 2026-11-08 on the soundness lane** (`.claude/skills/panel/SKILL.md` § Two lanes): the
proposal changes how the emitted C carries a value, so the seats are the two who compile,
`compiler-engineer` and `ffi-pragmatist`, with the completeness critic, who read these briefs before
any seat started (`236-reports/critic-briefs.md`) and reads the reports after
(`236-reports/critic-reports.md`). **A route that needs a new diagnostic, a refusal or spec text
leaves this lane**: a seat that finds one the most robust says so, with the program that shows it,
and the coordinator convenes the full panel for that part; this sitting rules the emitted C alone.
What the lane gives up is said in the synthesis. Convened on **defect 2286**
(https://github.com/heroes-lang/heroes/issues/2286), classed `systemic` by the author's decision of
2026-11-07 (route A), recorded in the defect's Class line (no decision issue was opened for it),
which names the proposal and sends it to this sitting after panel 235; the author ratifies the
sitting's resolution. `proposal.md` is the proposal in ten lines.

**Repaired on the critic's first pass, 2026-11-08** (`236-reports/critic-briefs.md`): the base's
history corrected (2278 was closed by panel 233's landing); panel 233's veto (a) and the shapes that
meet it added; `grpret` replaced as the witness of part (b) by `grpnoext`, with the critic's
measurements of each call alone; the room's 1 GiB bound, part (a)'s place outside today's set, the
missing construction in place, the union's double count, the suites the route owes and the comparator
routes added; the steering toward one route and one section removed. The critic's programs are in
`probe/`, beside this file.

## The base, frozen

Every seat measures **`main` at `5bd4cf3e4`** (`git log -1 --format=%H` in each seat's worktree:
`5bd4cf3e4ee1eead81577817e95dc25b3304df20`), whose compiler reads `sources 2fa3d81374af3684`
(`./heroes-next --version`; the coordinator's stage 2 and the critic's own build agree, and `git
diff --stat eef9919ab 5bd4cf3e4 -- selfhost runtime seed` prints nothing). It holds panel 233's
route as landed, which closed defect 2278 (`82a9dc630`), and batch 23's repairs beside it (defects
2316 and 2317).

## Your worktree, your compiler, and the bounds

- Each seat works in **its own worktree**, `.claude/worktrees/236-<seat>/` (detached at the base),
  and its scratch beside it, `.claude/worktrees/236-<seat>-scratch/`; never in another seat's, never
  in the repository's root, never in a lane of batch 24 (`b24-*`, `pm-*`, `core-b1`), and no file
  written outside `/Users/joseph/Temp/heroes/heroes-lang` (CLAUDE.md § Hard stops). The sitting's
  convention since panels 234 and 235: worktrees under `.claude/worktrees/` instead of the
  scratchpad the skill names, because CLAUDE.md forbids a file outside the repository's root.
- Build your own compiler in your worktree, never another's: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`, then `./heroes build selfhost/main.hero -o heroes-next` (under a
  minute), and again after every edit of `selfhost/` or `runtime/`.
- **Measure cost in instructions**, `/usr/bin/time -l` (its *instructions retired* line), never in
  seconds: four lanes of batch 24 run on this Mac beside the sitting.
- **A program that may not end** is built and its binary run under `timeout 60`, its output through
  `head -c 65536` into a file; never `timeout heroes run`.
- **The 1 MiB chains need about 7 GB at their peak** (panel 233's ffi seat: 6.98 GB) on a Mac of
  24 GiB shared with four lanes: **the compiler-engineer alone runs them**, the base's and the
  route's one after the other, never two at once; the ffi seat runs none.
- **No paid run** of any kind, no `claude -p`, no API call. **No credential printed**: `gh auth
  status` if you must know, never `gh auth token`. **No write to GitHub.**
- Write your report into `docs/panel/236-reports/<seat>.md` in your worktree **as you go**, not only
  at the end, so a stopped seat leaves what it had; the coordinator copies it into the sitting's lane.

## The programs

Panel 233's reproducers are in its evidence archive; extract them into your scratch with

```
tar -xJf docs/panel/233-reports/evidence.tar.xz -C <your scratch> \
  233-compiler-engineer/ce/critic2/grp 233-ffi-pragmatist/ffi/three/f2 \
  233-ffi-pragmatist/ffi/three/gen2.sh 233-critic/critic/grp
```

The critic's, written at the first pass, are in `.claude/worktrees/p236/docs/panel/236-briefs/probe/`
(copy them into your scratch; they name their headers beside them): `grp/grpnoext.hero` (a Heroes
function returns defect 2253's 128 MiB `partial` record `Held`, read in Heroes, no `extern` call),
`grp/grpextonly.hero` (the `extern` by-value call alone), `grp/grpret.hero`, `grp/grpgen.hero` (a
generic instance `same(x: Held(...))`), `grp/grpopt.hero` (`make(n) -> Held?`), `grp/grpfix.hero` (a
fixed array of `Held` returned), `grp/grpwrap.hero`; `cb/cbparam.hero`, `cb/cbresult.hero` and
`cb/cbnamed.hero` (a callback type taking the 128 MiB record, returning it, and taking a group record
of 4,800 named bytes); `extval/extval.hero` (an `extern` returning `Held` stored as a
`(function(i64) -> Held)` beside a Heroes function and handed to C); `union/unioncount.hero`;
`neg/progrec.hero` (a program record in a callback type, refused).

## What was measured, 2026-11-08, with the commands

Built with the base's compiler, `./heroes-next build <file> -O0|-O2 -o <bin>`, run as `timeout 60
<bin>`; the frames read with `selfhost/cli/flags.hero`'s `flags()` words and
`-Wframe-larger-than=65536` (the critic):

| program | `go`'s frame, `-O0` / `-O2` | run, both levels |
|---|---|---|
| `grpret` | 268,435,584 / 134,217,808 | `before`, `panic: stack exhausted in grpret.go`, exit 134 |
| `grpnoext`: `make`'s result read, no `extern` | 134,217,840 / 134,217,792 | the same |
| `grpextonly`: the `extern` call alone | 134,217,856 / 134,217,776 | the same |
| `grpgen` | 268,435,600 / 134,217,792 | the same |
| `grpopt`, already through `hero_out` | no frame over 64 KiB | `panic: stack exhausted in grpopt.go: the room large values take off the stack is full, 1 GiB for the whole program`, exit 134 |
| `grpwrap` | | `before`, `13`, exit 0 |

So **`grpret` aborts whatever the route**: `grpextonly` is the shape of defect 2253's three cases,
which expect `panic: stack exhausted`, C's ABI putting the by-value argument on the caller's frame.
**Part (b)'s witness is `grpnoext`** (and `grpgen`, `grpfix`). And `grpopt` shows a second bound:
`go`'s prologue takes 9 regions of at least 128 MiB (the critic), so a 128 MiB value through
`hero_out` meets the room's 1 GiB before the stack.

The chains, `f2/gn12` (named halves) and `f2/ga12` (array halves), 64 KiB each: each program's C
written by `--emit-c` and compiled alone with `/usr/bin/time -l clang -fintegrated-cc1 -O2
-gline-tables-only @flags.rsp -I runtime -I <program's folder> -c`, `flags.rsp` the nineteen words of
`flags()`, one per line: `gn12` 12,427,643,548 instructions and 164,839,976 bytes at its peak, `ga12`
10,115,706,929 and 155,828,800 (the coordinator); the critic's own run read 12,475,836,488 and
10,156,816,432, within 0.4 %. **Part (a) is outside today's set**: the chains' records are fully
named, and `large_types` (`selfhost/emit/large_size.hero:165-166`) marks a group record only when it
is or holds a `partial` one, so today's set moves no byte of either; and **construction in place**
(`selfhost/emit/construct.hero:137`) applies to a record of the program's own (`own_declaration`)
alone. Panel 233's bound for its own chain was *each unit under 8 billion instructions and 100 MB at
`-O2`* (its compiler-engineer's prediction). **Not run for this brief**: the 1 MiB chains, which panel
233's ffi seat read at 446.9 billion instructions and 6.98 GB for the array chain and lane b23-emit at
451.6 billion and 6.99 GB on 2026-11-07.

**The count's premise is false for a union** (the critic, `union/unioncount.hero`): a group record
naming both members of a 2,400-byte union is counted 4,800 bytes, so its 2,408-byte holder takes
`hero_out` today; `large_value.hero` calls the count *a lower bound of `sizeof`*.

## Panel 233's veto (a), which the proposal meets

Panel 233's ffi-pragmatist (`233-reports/ffi-pragmatist.md` § 2, *R7*, and § 7) measured three ways a
function type can carry the out-parameter convention, over `bigabi.hero` and `extval.hero`:
**R7a**, the function type's C spelling follows the new convention, refused by clang at the probe and
the call; **R7b**, *function values and thunks keep C's convention*, a value adapter being the
function's address and only a direct call by name using the out-parameter, accepted with the
boundary's ABI identical; **R7c**, the thunk keeping the function's own signature, accepted by clang
in silence and **exit 134 at both levels**, C writing the result through an integer argument. *The
probe checks the function type's typedef against the header, and nothing checks the thunk against the
typedef*; the gap closes with one `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk),
<typedef>))` per thunk, and *R7 keeps the boundary only in R7b's shape, with that assertion*. Panel
233 escaped the question by refusing a program record inside a callback type (R3). **A group record is
not refused there**: `cbparam`, `cbresult` and `cbnamed` emit at exit 0 on the base, and `extval` stores
an `extern` of C's convention (`held_make`) and a Heroes function in one function type, a value C then
receives through `hero_callback_of`. So the proposal's *an indirect call through a function value does
the same* meets veto (a) wherever such a value may hold an `extern`.

## Where to look in the compiler, counted on the base (`wc -l`)

- `selfhost/emit/large_value.hero` (351 lines): panel 233's route; `returned_from_room` (line 114,
  its comment from line 104) is how a group record kept off the frame returns today, by C's
  convention, copied out of a region; its first test (line 339) holds the refusal of a program record
  in a callback type.
- `selfhost/emit/large_size.hero` (203 lines): which types are large and how they are counted
  (`bytes_of` line 66, `holds_partial` line 84, `large_types` from line 156, `group_named` line 171).
- `selfhost/emit/construct.hero`, line 137: construction in place, own records only.
- `selfhost/emit/callback_thunk.hero` (278 lines) and `selfhost/emit/callback_guard.hero` (207
  lines): what C calls when a Heroes function is handed to it, and `hero_callback_of`.
- `selfhost/ffi_errors.hero:39`, `ffi_callback_carries`: the refusal of a program record in a
  callback type.
- `runtime/parts/frame.c`: the room off the frame (panel 233's R1 and R5).

## The record to read

Panel 233's file, `docs/panel/233-a-record-past-4096-bytes-lives-off-the-frame-returned-into-its-callers-slot-and-built-in-place-so-clang-never-splits-it-and-no-frame-outgrows-the-stack.md`,
§ The resolution adopted (R1 to R6), § What it does not close and § Scored at the landing (where
`wholes` refused the route's own emissions after the sitting); `docs/panel/233-reports/ffi-pragmatist.md`
§ 2 and § 7 (veto (a)) and § 3b; `docs/panel/233-reports/critic-reports.md` § 2; defect 2286's issue
and its comment of 2026-11-07; defects 2253, 2278, 2316 and 2317.

## Frames no seat argues past

1. **Robustness ranks above elegance, cost and speed** (CLAUDE.md § Precedence, rank 3): a Heroes
   program must not segfault and must not corrupt memory. A correct program that aborts on stack
   exhaustion keeps spec § 9's promise, *running out of stack aborts*, and is still a program that
   does not run.
2. **Every `extern` call and every callback keeps C's ABI byte for byte** (panel 233's R3), and a
   thunk is checked against its typedef where its signature could differ (veto (a)).
3. **The emitted C is the same on every target** where it can be (panel 233's choice of a count by
   named fields), because the `emission` suite blesses one file per program.
4. **Programs with no such value get byte-identical C**, but for the line `hero_program_sources`
   (panel 233's R6; panel 222's R4).
5. **A route that does not build is not adopted** (the skill § 3c): each seat builds what it
   proposes in its own worktree and runs the shapes it names before the synthesis.

## The questions

1. Which group records the convention reaches: today's off-frame set (`partial`, or holding one by
   value), named fields past `LARGE` (4,096 bytes), or another rule; what each moves in the emission
   corpus; whether it holds on the four CI targets; and what the union's double count does to it.
2. Between Heroes functions: whether a function returning such a record, taking one by value, having
   its result discarded, a generic instance, an option and a fixed array of one, takes `hero_out` and
   the pointer parameter, measured at both levels on the base and on the route
   (`-Wframe-larger-than=65536` and the run), with `grpnoext`, `grpgen` and `grpfix` as witnesses.
3. **Function values and callbacks, under veto (a)**: the proposal as written (values take the
   out-parameter, an `extern` stored as a value given an adapter, a C-convention adapter where a
   Heroes function reaches C) against **R7b** (values and thunks keep C's convention, only calls by
   name take `hero_out`, calls through values keep paying the by-value return); for each, the C of
   `extval`, `cbparam`, `cbresult` and `cbnamed`, the per-thunk assertion, the adapter's own frame,
   and whether C's view of every function is byte for byte the base's.
4. An `extern`'s own by-value parameter or result of such a record (`grpextonly`): C's ABI puts the
   copy on the caller's frame whatever the route does; what a program meets today, and, if the most
   robust answer is a refusal or a message, that it leaves this lane (above).
5. The room: the peak a route takes for one value (`grpopt`'s 9 regions), whether a temporary copied
   straight into a slot can be written in the slot's region, and what bound a witness of 128 MiB then
   fits under on each leg (Windows links a 64 MiB stack, `fixedbugs-2253-held.h`).
6. Part (a): whether the route reaches the chains (outside today's set), what construction in place
   for group records adds, and what clang reads before and after for `gn12`, `ga12` and the 1 MiB
   chains, against panel 233's form of bound (*each unit under 8 billion instructions and 100 MB*).
7. What the landing owes: its cases; `--sanitize` at both levels on every shape; `wholes`, `lldb`,
   `determinism`, `descriptors`, `emit` and the `emission` re-bless; panel 215's R1 (the padding is
   zero where C reads a record that crossed through `hero_out` or a pointer); and what each CI leg runs.
