# Panel 184's R1 landed: `{{` and `}}` each write one brace, and a lone `}` is an error, in its two stages

2026-10-03 at 00:24 by the clock (`date`), lane fbrace records the landing of
panel 184's R1, ratified 2026-10-01
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`),
which defect 173 held. It landed in the two stages R1 names, panel 121's
bootstrap hazard: `f6fdda79` (2026-10-02 23:36), the compiler learns `}}`
with no literal of its own changing meaning, 0 of the 380 tracked `selfhost/`
files holding `}}` or a lone `}` in an `f` literal's text, counted by the
compiler's own lexer; `11b40220` (23:42), the seed regenerated from that tree
and its fixpoint verified by `cmp`; and the stage after it, in which no `f`
literal of the compiler's own or the harness's holds a brace of text at all, so
nothing of theirs needed the escape. R8's sentence is in spec § 2 since
`8fc6e206`.

## The decision

| | |
|---|---|
| date | 2026-10-02 |
| decision | R1 as ratified: in an `f` literal `{{` and `}}` each write one brace, and a `}` alone in the text is `lone_brace`, at the brace, with two guesses, `}}` and its deletion; the fixes that write into an `f` literal's text double `}` as well as `{` (an escape's repairs, `\}` now read as the brace `}}`, and `\(n)`'s rewrite); the walk a new escape owes every tool, each held by a test: `fmt`, the four dumps and `mutate` over `surface-fixtures/braces173/`, `heroes probe` over it at exit 0, the TextMate grammar's first `f"…"` rule held to the lexer by `spec/colouring`, the site highlighter's comment; 3 literals in 2 run goldens and the gallery's `{{like this}` moved |
| reason | the one silent wrong output of the sitting's shapes no refusal reached: `f"{{x}}"` printed `{x}}` and `f"a}b"` printed `a}b`, both at exit 0; R1's prediction, the ffi-pragmatist's, scored by `run/fixedbugs-173-a-regular-expression-gets-the-bytes-its-author-wrote`: POSIX `regcomp` receives `^[0-9]{3}-[0-9]{4}$`, 19 bytes, where it received `^[0-9]{3}}-[0-9]{4}}$`, 21 bytes, its two matches inverted |
| design.md § | §4.17 |
| panel | 184 |

## What it leaves open

- The round's gate: the full net, the compiler's own tests, the census of
  `check` trunk against batch, and the formatter's probe by hand before the
  push, `selfhost/cli/probe.hero` having moved; the site's build before the
  push, `site/src/lib/highlight.ts` having moved.
- Whether `lone_brace` joins `diag.is_thesis_rule`: the sitting did not rule
  it, and without the refusal a `}` alone still has a meaning, text, as it had
  before R1. Left out, a question for the coordinator rather than a choice
  made here.
- The regex case is skipped on Windows, which has no `regex.h`, as every such
  case is; its Linux legs are unrun in the lane.
