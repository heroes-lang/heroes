# Panel 184, brief for the spec-warden

Read `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/00-shared.md`
first: the sitting, its three questions, and every measurement they rest on,
with the commands. (Repaired on the completeness critic's first pass, 16:51
onward: `docs/panel/184-reports/completeness-critic-briefs.md`; its routes are
now (1a) to (1d), (2a) as ONE predicate shared with `missing_return`, (2b),
(2c), and (3a) to (3d).)

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-spec-warden/`:
`git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff | tar -x -C <it>`,
`rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Never build or run in the repository's working tree or another seat's
directory; in the working tree, read only this sitting's briefs directory,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/`.

**The spec, measured by the coordinator at 16:10** with the trunk's
`./heroes` at `a294a6ff`: `./heroes measure spec/heroes-spec.md`
prints `real 9060` (claude-opus-5, 2026-09-28, *the binding number*),
`maximum 6838`, and *Headroom: 1180 against the 10240 ceiling, but the FFI
floor mortgages 60 of it (panel 030 R3), so what is measured against the
ceiling is 9120 and the check goes red at 10240.* Re-measure it in your copy
before you use it.

## Your tasks, in order

1. **The sentence each route needs, written and measured.** For each of
   00-shared.md's routes that changes what a reader must know, (1a), (1b),
   (1c), (2a) (both of its halves: the statement refused, and the `return`
   no longer demanded after a statement that always leaves), (2b), (3a),
   (3b), (3c), write the smallest sentence of
   `spec/heroes-spec.md` that makes a reader of the spec alone right about it,
   at the place it belongs (§ 2 for question 1, whose last sentence *A literal
   without the `f` is unchanged* some routes falsify; § 8 for question 2; for
   question 3, beside `spec/heroes-spec.md:270`, *Recursion too deep aborts*,
   or in § 1), apply it to a copy of the spec, and give `heroes measure`'s
   `real` before and after. A route that needs no sentence says why a reader
   loses nothing.
2. **What pays** (panel 012, design.md §1.6): for each priced sentence, a
   named removal of equal size or a pre-registered falsifiable prediction.
   00-shared.md's instrument counts (17 of 25 `forget-f` mutants and 6 of 150
   `over-indent` mutants silent, 2 of those 6 after a `return`) are the
   measured argument on offer; say whether each is a design.md Part 11 effect
   or an argument you would accept, and what a prediction scored at the next
   instrument run would have to say.
3. **Question 3 and panel 107.** Panel 107 refused a depth number in the spec
   for a program's recursion at run time, because it *would be false on the
   day it landed, for a legal program and nearly for this compiler itself*
   (`docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md`, § The
   resolution). Say whether a limit on a source's nesting, counted by the
   parser and the same on every platform by construction, is that kind of
   number or another, and whether it belongs in the spec at all, given that
   under (3a) it decides which programs compile.
4. **Principle 0** (CLAUDE.md § 2): for each route, whether it enters v1 as
   compiler need or as a measured thesis effect, or waits.

Your verdict per question: approve, object or veto each route; your veto is a
budget breach, and it names the sentence and its count. Write your report to
`docs/panel/184-reports/spec-warden.md` in the repository AS YOU GO (the one
file you write there). No paid run (`claude -p`, an API call, a cloud run).
English, no em dashes.
