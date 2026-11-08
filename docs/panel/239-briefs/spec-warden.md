# Panel 239, the spec-warden's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/239-spec-warden/`, detached at the sitting's briefs commit on `p239`; build your
compiler there as `00-shared.md` says (you need it for `heroes measure` and to run the cases). You
judge the indicator (design.md §1.2's cost formula, §1.6's budget) and Principle 0's burden of proof
(CLAUDE.md § 2), with veto power on a budget breach.

## The counts

`./heroes-next measure spec/heroes-spec.md` at `0a62bb10c`, run while this brief was written:
`claude-legacy 8178`, `cl100k_base 8325`, `real 10900` (`claude-opus-5`, 2026-11-08, pinned by the
last refresh), headroom 5484. **The vendored tables only**: price every draft by applying it to a
copy and running `heroes measure` on that copy's path, or as `.claude/rules/spec-shape.md` § How a
change to the document is made says for the vendored rows. **No `--refresh`**: it is a paid request
the author has not allowed for this sitting; where a real count would decide, say so and size it.

## What the sitting needs from you

1. **What the specification says today about each defect.** § 13 at line 375 (*"A group names its
   header, and `link` a library when the symbols need one; C reads a module's headers in the order
   its groups are written ... clang checks every result type, constant and record field against that
   header"*): whether a reader of it can predict `sig_sorted`'s crash, `dup_*`'s two values,
   `pw_first`'s refusal and `pack/pk_first`'s wrong value (`00-shared.md`), and which of them the
   document's own words call a correct program. **Read line 131 first**, *"Declaration order never
   matters but a group's (section 13); mutual recursion needs no forward declarations."*, and § 1's
   `use` lines (22 to 31): whether a `use` line is a declaration in that sentence's sense decides
   whether the document already promises that two `use` lines' order changes no program's meaning,
   which `pw_first`, `dup_sorted` and `sig_sorted` break today. § 13 runs from line 373 to 490. A
   silence in the spec is often a ruling: grep
   `docs/panel/` and search the issues for one before you call it a gap.
2. **Whether either repair owes a sentence.** A refusal is no new form (CLAUDE.md § Precedence,
   ranks 3 and 4, panel 215: robustness decides that a measured class closes, Principle 0 how), but
   it changes which programs build, and a reader who cannot predict it writes it. Draft the shortest
   true sentence for each, and the one that says nothing at all, priced on both vendored tables;
   say what each buys under §1.6's payment rule (a named removal, or a registered prediction naming
   an instrument that exists). Panel 238's R7 names a draft of its spec-warden's stating 2363's refusal, priced
   there and not adopted (`238-reports/spec-warden.md`): price it again here, at this commit, since
   the words of the refusal are this sitting's.
3. **design.md §4.19** (`docs/design.md`, the section from line 2582) and §4.17 (line 2320): the
   sentences each repair owes the design, drafted and placed; and `.claude/rules/c-boundary.md`
   § *A clang failure that the author's own extern caused*, whose list has twelve members and whose
   rule is that *"a symbol nobody declared stays exit 2"*: whether 2363's refusal is a thirteenth
   member, a widening of one, or neither.
4. **The words of both refusals**, as a reader of the specification meets them: whether each
   message names only what the specification names (a group, its header, its `link`, a module) or
   leans on a word the document never uses (`grep -ci` over `spec/heroes-spec.md` prints 0 for
   *archive*, *link line*, *translation unit*, *unit* and *linker*, 2 for *library*), and what
   that costs a reader who has only the specification.

Your report: `docs/panel/239-reports/spec-warden.md` in your worktree, written as you go, ending as
`00-shared.md` § Rules says, with your prediction registered before you read any other seat's report.
