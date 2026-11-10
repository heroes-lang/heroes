# Panel 242, the compiler-engineer's brief

Read `00-shared.md` beside this file first. Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/242-compiler-engineer` (detached at
1edabb957); build your compiler there from the seed and nowhere else. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p242/docs/panel/242-reports/compiler-engineer.md`.
Panel 241's evidence (`docs/panel/241-reports/evidence.tar.xz`, its note beside it) holds route 1's
diff and the hook mirror's patch of that sitting's compiler-engineer; use them, re-run what you cite.

## Question 1 is yours, built

- **Route (A) scoped to replace**: the compiler takes `core` 0.2.0 as a catalogue entry and moves
  every call whose contract is `core/strings`' onto it, deleting what that leaves dead
  (`cli/strings`' `starts_with`, `ends_with`, `holds` where no caller remains, and every live hand
  copy of those contracts outside `cli/strings`; count them again at 1edabb957, since the b28 round
  deleted four). Build it over two stages with its fixpoint, run the compiler's own tests, and
  report: lines and functions deleted, modules that need an `as` because they keep `cli/strings`
  for helpers `core` has no contract for, the seed's `#line`s naming the placed folder, and the
  cost of every `heroes check selfhost/main.hero` in instructions retired (`/usr/bin/time -l`,
  alternated, five each). The hook mirror: where the guard refuses a net run, the patch is
  241's; the runs it blocks stay unrun, and you say which.
- **Route (C)**: a copy of `core`'s files as the compiler's own modules, held to the index by a
  check of the net rather than a fingerprint. Say what the copy needs to compile (241 measured the
  plain copy refused) and what the check would be (which suite, about how many lines), and whether
  it is (A) without the fingerprint.
- **What (B) costs** if the answer is no: the hand helpers stay; count what M-core-packages'
  acceptance condition and feature 2315 would then have to say.

## Questions 2 and 4 from the compiler's side

- Route (b) of question 2: what of `cli/process` an `os` package would hold, what stays the
  compiler's, and how many of the 96 modules move; route (c): what the compiler's index reading
  gains over running `curl`, and what the seed's build line becomes.
- Question 4: the compiler's `path`-shaped helpers (defect 2406 lists some): how many, which
  contract each has (Go's `path/filepath` or `path`), and so which package the compiler would take
  first, counted as you counted question 1.

## Your verdict

Per question: approve a route, object or veto, with the cost in lines and seconds. Veto on
soundness only, naming the program or compiler fact that would make the route wrong.

## Added on the critic's first pass (`../242-reports/critic-briefs.md`)

- **Question 1 (i) from the compiler's side**: read panel 234's R1 veto of B5 (the canonical tree
  compiled into the compiler for programs, `docs/panel/234-reports/compiler-engineer.md` and the
  sitting's R1) and say whether the compiler depending on a package falls inside it, with the
  compiler fact that decides it.
- **Question 1 (ii), counted as question 1**: `crypto/sha256` against `selfhost/cli/sha256.hero`
  (292 lines) and the two C doors: what a Heroes-alone package would delete, what the fingerprint
  check would then be, and whether a compiler can verify the placed tree of the package that
  implements its verification (a bootstrap of trust: say what the seed and the first fetch would
  check, and against what).
- **Route (D)**: what admitting `path/filepath` through the net (`tests/harness/`) or the hooks'
  rewrite (issue 2082) would cost, and whether `.claude/hooks/provenance.py` follows a catalogue
  entry of a program that is not the compiler (read it; do not patch the trunk's).
- **Costs in instructions retired**, not seconds: the ffi-pragmatist builds on this Mac at the same
  time. Seconds are planning figures.
