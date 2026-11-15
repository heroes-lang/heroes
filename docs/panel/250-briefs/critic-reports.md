# Panel 250, the completeness critic's second pass, over the reports

*Written with the briefs, before any seat ran; the coordinator re-reads it against the reports that
exist before it is handed on, and says beside it what changed.*

Read every file in `docs/panel/250-briefs/` and `docs/panel/250-reports/` of the worktree
`.claude/worktrees/p250`, whole (the sitting's base is `48f53b3437c6cd46aba5923f4f4780ac7b347283`,
rebased after your first pass; the worktree's own checkout stands at `722ae76bf`): the four seats' reports
(compiler-engineer, ffi-pragmatist, spec-warden, historian) and your own first pass
(`critic-briefs.md`). You give no verdict. Name what is MISSING:

1. **A route nobody listed, or listed and nobody built**: for the unit (W, M, G, H, or another), for the
   sum (F1 to F4, or another), and for the alternatives (R2's ruler counted by modules reached, `core`
   split, trees not committed, the author's *b* and *c*). For the route a seat recommends: was it built
   in that seat's copy and run on the cases it named (`SKILL.md` 3c)? A recommendation over an unbuilt
   route is named as such.
2. **A claim asserted and not measured with the command that settles it**: above all whether R2 is met
   for `core` 0.4.0 under the recommended route (a count of the compiler's tree, placed files counted,
   before and after, in a stated unit); whether any emitted C, `#line` or provenance key moves when the
   same modules are reached from a pruned tree; what each falsifier edit does under the route (which
   module compiles, whether the program builds); which golden of `tests/golden/fetch/` and of the 55
   case folders holding a placed tree moves; and **whether any case pins which files a fetch places**,
   since the `fetch` form prints placed folders' names alone (`tests/harness/suite_fetch.hero:452-456` at
   the base) and a route that changes placement owes such a case.
3. **A contradiction between seats, with which side is checkable**, and the check run where it can be:
   for instance the compiler-engineer's closure against the ffi-pragmatist's C closure on one shape, the
   spec-warden's reading of panel 235's R2 against the historian's page on Go's vendoring, or two seats'
   counts of one tree.
4. **A precedent leaned on that the historian did not mark `verified`**, or a sentence about another
   language's tool written from memory in any report, which `00-shared.md`'s rules make a question.
5. **The reader-facing escape**: whether any report says a route changes what a program, its catalogue,
   a `use` line, a message, the verbs' help or a spec sentence writes, and whether the synthesis would
   adopt such a route that no seat named as reader-facing.
6. **The question the sitting should have asked and did not**, with who could run it and how.

Build a compiler at the base in `.claude/worktrees/p250-scratch/critic/` from the seed if a check needs
one (a `git archive` of `48f53b343`, stage 2 `sources 5bac791363384945`); to rebuild a seat's route, copy its changed files from its report's
diff into a tree of your own, never build or run inside a seat's folder. Work read-only everywhere else;
write only under `.claude/worktrees/p250-scratch/critic/` and your report. No paid run, no `claude -p`,
no API call, no `heroes measure --refresh`, no credential (`gh auth status` at most, never `gh auth
token`), no GitHub write, no file outside the repository's root (a redirection to `/tmp` is one: your first
pass made one, `/tmp/x`, and said so). A suite's or the harness's output goes whole into a file and is
read from it, never through `head` or `tail`, which the guard refuses. A program that may not end runs as a
binary under `timeout -s KILL`, its output through `head -c 65536`; a Docker run as `00-shared.md` §
The platforms says, `docker ps` read after it. The Windows box is off: a Windows question is owed to
the CI's leg.

Write your report to `docs/panel/250-reports/critic-reports.md` in the p250 worktree as you go, and end
it with a numbered list: each item what is missing or contradicted, the command you ran and what it
printed (or the command that would settle it, unrun), and what the synthesis must not adopt without it.

**Re-read by the coordinator on 2026-11-15 at about 08:50, before this pass is handed on.** What
changed since it was written: (1) five reports exist, not four: the four seats', and
`llm-ergonomist.md`, the blind seat the author approved on 2026-11-15 at about 02:03 with a ceiling of
10 USD (24 sessions, `claude -p` from `.claude/worktrees/p250-scratch/s/`, 3.139598 USD by `run.log`,
graded by the rule registered in `250-briefs/llm-ergonomist.md`; the sessions had no write tool, so
their answers were taken from each `run.json`'s `result`, and eighteen of them saw their sibling
folders' names in their prompt, opening none); read it with the others and say whether its grading
followed the registered rule. (2) The sentence above that the worktree's own checkout stands at
`722ae76bf` is false: `.claude/worktrees/p250` stands at `48f53b343`, as three seats said. (3) Every
seat objects to placing only the reached modules and approves whole placement (W); the ffi-pragmatist
vetoes a computed C closure; the compiler-engineer measured that no placement meets R2 for `core`
0.5.0. Item 1 of the list above therefore asks, above all, whether R2's ruler itself (its unit, the
author's *b* and *c* of 2026-11-14) is the question the synthesis must put to the author, and with what
measurement.
