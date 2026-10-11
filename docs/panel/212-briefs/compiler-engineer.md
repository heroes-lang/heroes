# Panel 212, the compiler-engineer's brief

Read `00-shared.md` first, all of it (it was revised after the critic's pass); then
`.claude/agents/compiler-engineer.md` for your output structure. Your input is the proposal plus
the live compiler: `selfhost/` and `runtime/`, never `archive/bootstrap-rs/`. You have a veto on
implementation cost and on core-versus-sugar (design.md §1.1, §1.7, Part 5).

**Your question: what does the verb cost to build in `selfhost/`, where does it live, and can the
rendering rule be implemented so that two runs on one file give one answer?**

**Build the proposal's `render` in your own worktree and run it on the real files** (sitting rule 3c:
a route that does not build is not adopted). Make the worktree as the shared brief says
(`git worktree add --detach ... 09060ea3`), build the compiler from the seed (about 7 s), add a pure
module under `selfhost/cli/` that takes one issue file's text and returns the title, labels,
milestone, body and closing comment, add the `Command(` row beside `measure`'s
(`selfhost/cli/table.hero`, around line 160), rebuild, and run `heroes issues render <file>` over
**all 2014 files** under `issues/`. The sketch `docs/panel/212-briefs/scripts/render_proto.py` is a
starting point for the rule's readings, nothing more. Report:

1. how many files render without the verb refusing, how many it refuses and why;
2. the rule you had to invent for each of D1 to D7 in the shared brief (the 670 cards with no title,
   the `Defect` prefix that doubles, the number-less defects, the closed day, the two `self` cards
   `git log --diff-filter=A` does not answer, `area: none`, the hashes in backticks, the ` | ` inside
   a code span, the multi-item task lists) and the count of cards each one touches;
3. what you did to a bare `#NNN`, an `@word`, an `owner/repo#N` and a URL to another project's
   issue, and whether a reader of the result loses anything;
4. the module's line count **in the unit `tests/harness/suite_layout.hero` counts** (not `wc -l`;
   module-shape.md), against the ~300 ceiling, and where the seam is if it passes it;
5. the time for the 2014 files, `/usr/bin/time -p`, `real` against `user`;
6. **JSON**: `selfhost/cli/json_text.hero` has `json_string` (00-shared F7); what is missing is an
   object and array builder. Size it, and say whether a test that emits a body holding a quote, a
   backslash, a newline, a tab and a four-byte character needs more than `json_text.hero`'s own;
7. `tests/harness/cards.hero` (763 lines) already reads cards in Heroes, and the compiler's modules
   resolve `use` from their own directory (a nested module cannot be checked alone). **Share that
   reader or copy it?** Measure both, name the drift `records/cards` would not see, and how you would
   guard it;
8. **R11**: a Heroes program under `tools/` or `tests/` run with `heroes run`, no new verb. Build the
   same `render` that way if it is cheaper to say, and report what it would lose (the surface, the
   `heroes doctor` listing, the site's count) and what it would save.

Then the other halves: what a test seam costs (a transport that can be a recording); and what a
fourteenth verb costs the rest of the tree, **counted with commands**: `heroes doctor`, the summary
text in `table.hero`, the site's claims check (F6), and the net's own tests that name the verbs
(`tests/harness/argv.hero:333, 393`, `main.hero`'s dispatch; the critic found at least those).

Do not write to the tree you copied from. Your report is
`docs/panel/212-reports/compiler-engineer.md` in the lane tree, written as you go.
