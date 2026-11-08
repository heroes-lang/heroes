# Panel 238, the spec-warden's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/238-spec-warden/`, detached at the briefs' commit; build your compiler there in two
stages as `00-shared.md` says (you need it for `heroes measure`). Your mandate is design.md §1.6 and
§1.2 and Principle 0's burden of proof; reach §1.6 by grep and read what it says today. You sit on
one question, C's last part: **whether the specification moves**. The briefs said it does not (F16);
the critic's first pass made it a question (`238-reports/critic-briefs.md`, findings 17, 24, 28).

**Vendored tables only**: `heroes measure` without `--refresh`, no `claude -p`, no API call. The
real row it prints is the one pinned by the last refresh; a draft's real cost is therefore unrun, and
you say so and give the vendored delta. Measured while this brief was written, `./heroes-next measure
spec/heroes-spec.md` in `p238`: `claude-legacy 8188`, `cl100k_base 8335`, `real 10915` (*claude-opus-5,
2026-11-08*), headroom 5469 against 16384.

## What the sitting needs from you

1. **Does the rule need a line of the specification at all?** design.md §4.15 says the formatter
   *removes all style decisions from the model*; F24 says the tree was written with no order rule and
   635 of 1,068 files with a `use` are unsorted. Whether a model must know the order, given that
   `heroes build` does not require canonical form (try it: an unsorted program builds) and `fmt`
   writes the order, or whether stating it buys a measured effect (Part 11). If you find a line worth
   its tokens, draft it and price it on the vendored tables, with its removal or its registered
   prediction (panel 012's payment rule).
2. **The comment meanings** (F8): spec section 1, lines 49-50, *a comment directly above a
   declaration documents it; `##` is a section heading*. Under the rule a `use` line can move away
   from a comment or out of a `##` section (`selfhost/cli/response_file.hero:142-145`). Does any
   sentence of the specification become false or ambiguous under the compiler-engineer's rule (read
   its report as it lands in its worktree, or reason from `00-shared.md` § A's routes and say which
   you assumed), and what is the smallest repair?
3. **The premise in the specification**: line 131 *"Declaration order never matters but a group's"*
   and line 376 *"reads a module's headers in the order its groups are written"*; F19 to F22 show the
   written order of `use` lines reaching diagnostics, the one-file emission and `--emit-c`'s verdict.
   Does the specification say, or imply, anything about the order of `use` lines today (a silence is
   often a ruling: grep `docs/panel/` and search the issues, `gh issue list --repo heroes-lang/heroes
   --search 'use order' --state all`), and if the compiler is made order-free (`00-shared.md` § A),
   does a sentence owe the reader that fact?

Your report: `docs/panel/238-reports/spec-warden.md` in your worktree, written as you go, ending as
`00-shared.md` § Rules says, in the structure of `.claude/agents/spec-warden.md`.
