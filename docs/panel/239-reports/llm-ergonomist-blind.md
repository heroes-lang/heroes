# Panel 239's blind seat: run and graded, 2026-11-09

How it was run: after the author's ratification and allowance of 2026-11-09 (*yes 10 dollars*), by a
session the coordinator briefed with `239-briefs/blind/README.md`, its runner `pay239.py` and every
file it wrote kept under `.claude/worktrees/p239-scratch/blind239/` (ignored by git). The report
below is that session's, copied by the coordinator, who re-read the grader's log of the original
2363 program before filing what it observed (defect 2384). The grading was registered in
`grading.md` before any session, and the grader was proven on hand-written answers before any paid
call.

## What ran

- **Model**: `claude-opus-5-5`; the CLI 2.1.311 accepted it, and every `run.json`'s `modelUsage`
  names it and nothing else.
- **Specification**: `git show 176f604f8:spec/heroes-spec.md`, 28203 bytes, sha256
  `e84250605412ccbb2464adc30e2ac3c9921b566f6fe31079a546fec7cd89bdf9` in all twelve folders,
  byte-identical to the landing round's.
- **Order**: seed 2390, four at a time: `2364-today-1 2364-seat-1 2364-synthesis-1
  2363-synthesis-1`; `2364-synthesis-2 2363-today-1 2363-seat-1 2364-seat-2`; `2363-seat-2
  2363-today-2 2363-synthesis-2 2364-today-2`.
- **Command**: the README's, with `--model claude-opus-5-5` and `--max-budget-usd 0.45`, run from
  each session's folder, the environment holding only `PATH`, `HOME`, `LANG`,
  `GIT_CEILING_DIRECTORIES` and the key from the trunk's `.env`, never printed.
- **Grading compiler**: the landing round's stage 2 at 71a65e0c5 (`sources 2aaec26355432d71`),
  copied and pinned, with its own `runtime/` named by `HEROES_RUNTIME`, without which a copied
  compiler reads the trunk's (`heroes doctor` shows it).

## Spend

The CLI's reported sum after each group: 0.558, 1.079 and **1.5671 USD**; sessions from 0.1158 to
0.1887 USD; none failed, none retried; the 5.40 stop was never neared, the author's 10 never
approached.

## The grader, proven first

Eleven hand-written answers each got the class they should: both original programs refused
(`ffi_defined_elsewhere`, `ffi_record_two_layouts`); for 2363 right by `static`, by dropping
`pick_w.c` and by `nmedit -R`, refused with the `use` lines swapped, silent wrong printing `10 | 30`
and by a constant `1`; for 2364 right by `#undef` at the end of `m.h`, refused by today's guess
`a: i64`, silent wrong printing `10`.

## Results

| session | class | what the answer did |
|---|---|---|
| 2363-seat-1 | right, renamed | `pick_w.c`'s `pick` renamed `w_pick` |
| 2363-seat-2 | right | `static` |
| 2363-synthesis-1 | right | `static` |
| 2363-synthesis-2 | right | `static` |
| 2363-today-1 | right, renamed | renamed `w_pick` |
| 2363-today-2 | right, renamed | renamed `w_pick` |
| 2364-seat-1 | right | `m` no longer names `Rs`; `rs.sum` added; `main` changed |
| 2364-seat-2 | right | the same three files, byte for byte |
| 2364-synthesis-1 | right | `#undef TWO_OTHERS_WIDE` at the end of `m.h` |
| 2364-synthesis-2 | right | the same |
| 2364-today-1 | refused | followed today's `fix (guess)`, `a: i64`, refused by `ffi_record_two_layouts` |
| 2364-today-2 | right | deleted the `#define` from `m.h` |

Right of four: today 3, seat 4, synthesis 4 (a rename counted as wrong, ruled `right, renamed`
before any session since the README does not cover it: today 1, seat 3, synthesis 4). No answer was
silent wrong.

## The decision rule

**The seat's arm does not beat the synthesis's**, 4 against 4 (3 against 4 with renames counted
wrong), so the synthesis's words stand, as R6 adopted. Two cautions: the README's rule says *two or
more of six* and the run gave each arm four sessions, which `grading.md` registered before any
session; and the experiment meets its ceiling, so it cannot separate the seat's arm from the
synthesis's, though it separates today's words on 2364, where one session followed the `guess` fix
into a refusal. The session's reading, not a measurement: the synthesis's sessions took the remedy
their message names (`static`, the one-line `#undef`), the seat's sessions on 2364 restructured three
modules, its message's first remedy.

## Did a project rule, contract or memory reach a session

No sign in any of the twelve: each session's `context` section names only `brief.md`, `task.md`,
`heroes-spec.md` and the generic agent prompt, and a scan of every answer for `CLAUDE.md`,
`design.md`, `panel`, `ROADMAP`, `CL-0`, `selfhost`, `.claude`, `hook`, `skill`, `Principle 0`,
`heroes-lang` and `/Users/` found nothing. Two limits: the working directory reached every session,
and the folder names the README prescribed carry the arm's label, though no answer mentions it; and
no session could write `report.md` (`--restricted` leaves a `-p` session no way to approve a write),
so each gave its report as its final answer, which the brief allows, graded from `run.json`'s
`result`.

## Observations

- The landing's 2363 message names each definition twice, by its absolute path and by
  `lib/liba.a(pick_a.o)`, four notes for two definitions: filed as defect 2384, adjacent, repaired in
  batch 26.
- Neither arm's wording is the landing's for either message; the landing's 2364 message points at
  the reading module's spelling and says the binding is not what to change.
- Unrun: what today's compiler does with `2364-today-1`'s answer.
