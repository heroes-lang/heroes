# Panel 236, the completeness critic's first pass: the briefs

Measured 2026-11-08 at `5bd4cf3e4` with my own build (`sources 2fa3d81374af3684`); programs and
outputs in `.claude/worktrees/236-critic-scratch/`. Frames: `flags()`'s words plus
`-Wframe-larger-than=65536`; runs: `timeout 60`, `head -c 65536`.

## 1. The proposal is the shape panel 233's ffi seat vetoed, and no brief says so

`233-reports/ffi-pragmatist.md` § 2 (R7) and § 7: veto (a) on *a thunk or a function value taking the
out-parameter convention*, lifted only by one C signature for a function, its value and its thunk,
or a `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk), <typedef>))` per thunk. The
proposal's indirect call is that shape. Panel 233 escaped it by excluding program records from
callback types; group records are not excluded: `cb/cbparam.hero`, `cb/cbresult.hero` (a callback
returning the 128 MiB `Held`) and `cb/cbnamed.hero` (4,800 named bytes) emit at exit 0. § 2's
`extval` holds on the base: `extval/extval.hero` returns the extern `held_make` as a
`(function(i64) -> Held)` (`t4 = held_make;`, `make`'s typedef) and hands that value to C through
`hero_callback_of`. Under the proposal, a call through it passes `hero_out` to a C
function. Unasked: that adapter, what `hero_callback_of` maps it to, the per-thunk assertion. The ffi
brief sends its seat to § 3b, the compile cost, "above all".

## 2. `grpret` cannot measure part (b)

Which call aborts, unmeasured in the brief: each, alone, at both levels.

| program | `go`'s frame, `-O0` / `-O2` | run, both levels |
|---|---|---|
| `grpret` | 268,435,584 / 134,217,808 | `stack exhausted in grpret.go`, 134 |
| `grpnoext`: `make`'s result read, no extern | 134,217,840 / 134,217,792 | the same |
| `grpextonly`: the extern call alone | 134,217,856 / 134,217,776 | the same |

`grpextonly` is the shape of defect 2253's three cases, which expect `!panic: stack exhausted`, and
frame 2 keeps that copy: `grpret` aborts whatever the route; its witness is `grpnoext`. Lane
b23-emit's *runs at `-O2`* fails for the returned shape at 128 MiB. A generic instance,
`same(x: Held(...))` (`grp/grpgen.hero`), aborts alike (268,435,600 / 134,217,792); no brief lists it.

## 3. The room's 1 GiB is a second bound nobody is asked to read

`grp/grpopt.hero`, `make(n) -> Held?`, which the base already returns through `hero_out`: no frame
over 64 KiB, and at both levels `panic: stack exhausted in grpopt.go: the room large values take off
the stack is full, 1 GiB for the whole program`, 134: `go`'s prologue takes 9 regions of at least
128 MiB. A witness past every leg's stack (Windows links 64 MiB, `fixedbugs-2253-held.h`) fits only
below 1 GiB over its live regions (arithmetic). No brief asks the room's peak. Unfiled.

## 4. Part (a) is outside today's set, and construction in place is missing

The chains' records are fully named; `large_types` (`large_size.hero:165-166`) maps a group record
only when it is or holds a `partial` one, so today's set moves no byte of `gn12` or `ga12`. Question 1
should say so. Of panel 233's four parts, each measured necessary, these records have the room (R4)
and the proposal adds two; construction in place, `construct.hero:137`, stays `own_declaration`
only. No brief states what bound closes part (a); panel 233's was 8 billion instructions, 100 MB.

## 5. Framing facts re-run

- Line counts, cited lines, the empty `git diff`, the key, the `grpret`/`grpwrap` table: as stated.
- `--emit-c`: 4,250,402 and 51,911 bytes, the brief's plus 17 and 16 times 25, the counts of the
  folder's path in each.
- `gn12` 12,475,836,488 instructions, 164,790,800 bytes; `ga12` 10,156,816,432, 155,517,528: within
  0.4 %.
- `target.c` fails on `x86_64-pc-windows-msvc` alone; a program record in a callback is `ffi_type`;
  `ssh win` answers.
- Defect 2278 was closed by panel 233's landing (`82a9dc630`), not batch 23.
- *Route A* (2026-11-07) has no `kind:decision` issue among the fifteen newest; the defect's Class
  line records it.

## 6. Questions the briefs do not ask

- `--sanitize` at both levels on every shape (defect 2316 was silent without it); `wholes`, `lldb`,
  `determinism`, `descriptors`, `emit` on the route: 233's landing found `wholes` red after its
  sitting.
- Panel 215's R1: zero padding where C reads a record that crossed through `hero_out` or a pointer.
- Question 4's *told at build* and *refused* leave this lane (a diagnostic, a spec change); the ffi
  seat is not told.
- Who runs the 7 GB chains, when: two seats, batch 24's lanes, 24 GiB.
- The count's premise, *a lower bound of `sizeof`*: `union/unioncount.hero`, a group record naming
  both members of a 2,400-byte union, is counted 4,800, so its 2,408-byte holder takes `hero_out`
  today. A named-field rule for group records moves these too.

## 7. Routes nobody listed

- **R7b as comparator**: function values keep C's convention, only calls by name take `hero_out`
  (233's approved shape). It needs a C-convention adapter per address-taken function and none for
  an extern; calls through values keep aborting.
- **Refuse a converted group record in a callback type**: no tracked program hands C one (awk over
  extern blocks of `git ls-files '*.hero'`, then `git grep '(function('`: seven callback types name
  a group record, all handles or 16-byte records). Removes the C-side adapter, not § 1's; a spec
  change, so the full panel.
- **A temporary copied straight into a slot written in the slot's region**, if the IR allows it:
  what § 3 needs.

## 8. Steering

§ 3b *above all* (§ 1); `grpret` as the measure (§ 2); one route, no comparator (§ 7); the `-O0`
framing (§ 2).
