# Panel 243, spec-warden

Read `00-shared.md` first, whole. Your worktree: `.claude/worktrees/243-spec-warden` (detached at
`89db84347`); `heroes measure` is your instrument, its vendored rows only (`measure --refresh`
is a paid run and not named here, so your verdict on the `real` row is provisional and says so).

Measured at the base by the coordinator: `heroes measure spec/heroes-spec.md` prints
claude-legacy 8167, cl100k_base 8314, and `real 10898 claude-opus-5, 2026-11-09` (the pin the
tree holds); re-run it yourself.

You judge what each route costs the specification and design.md in words and tokens, and
whether a route needs a spec sentence at all:
- Q1: does spec § 13 (`spec/heroes-spec.md:373` on) say anything a reader would apply past `...`?
  Write the sentence each route would need, if any, measure its tokens on the vendored tables,
  and the design.md wording of route (C), priced the same way.
- Q2 and Q3: does the specification mention a header standing in, `pkg-config`, or a sibling
  include (`grep -n -i` with the words you choose, named)? Write design.md §4.19's amended
  sentence for each question's recommended route, with its token cost.
- For every route, list the plausible mistakes a reader of the specification alone would make,
  and which error the route gives each.

Report: the sentences, their counts, the commands, your verdict.

## After the critic's first pass

Read `00-shared.md` § After the critic's first pass. Price the R10 exception (design.md
3900-3910) each Q1 route needs, and say whether a route needs a spec § 13 sentence; if one does,
say so plainly, since the synthesis then queues the blind seat's paid run for the author.
