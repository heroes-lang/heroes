# 001 — M1: the lexer

Milestone M1 · 2026-08-03/04 · steps 1–4 · first entry under the inverted
protocol (implement first, comprehension in `docs/debrief/QUEUE.md`).

## 1. Goal

Source text → tokens, the whole specced lexical surface: 18 keywords, all
operators and punctuation, int/float/str/char literals, comments retained
as tokens, rigid indentation (spaces only, exactly 4 per level,
Indent/Dedent), Go-style terminator insertion (design.md §4.15), reserved
foreign words with prescribed errors (spec/reserved-words.md), error
recovery that never stops the stream. Surface: `heroes lex <file> [--json]`.
Modules born: `source/` (owned text + byte-offset spans), `diagnostics/`
(codes + `certain|guess` fixes from day one), `lexer/`. 23 crate-internal
snapshot tests; 519+ lines of lexer.

## 2. What surprised — shapes and rules

- **A rule can cite its precedent and still not match it.** §4.15 said
  "(Go's rule)" but the transcribed ender list had dropped Go's
  `break`/`continue` and postfix operators. Implementing by the letter made
  the gap visible in one day; panel 007 completed the list (`break`,
  `continue`, `???`, postfix `?` — the analogue of Go's `++`/`--`) and
  specified continuation as brackets-only, after the compiler-engineer
  showed the naive "no terminator → next line continues" clause would have
  silently killed every `record`/`variant`/`else` body. The ffi judge
  showed postfix `?` as ender is *required*, not just safe: a fallible
  extern (`-> int?`) would otherwise swallow the next declaration.
- **Layout state is three small numbers.** The whole layout mechanism is:
  an indent level, a stack of unclosed opener spans (its length = bracket
  depth; inside brackets indentation is not structural), and the kind of
  the line's last significant token (decides the terminator). Everything
  else is dispatch.
- **Escapes don't exist and nothing forced the question until now.**
  design.md never mentions `\n`; the appendix never uses one. The letter is
  implementable (backslash = ordinary byte) but the self-hosted lexer will
  need the tab *character* without writing `9` — the exact magic-number
  class §4.3's char literals were introduced to kill. On record in
  OPEN-QUESTIONS for a panel before M6.
- **The thesis became executable in ~30 lines.** `let x = 5` now fails
  with "bind with `=`: `x = 5`" and `while` carries a machine-applicable
  `Certain` fix to `for`. First feature where "every plausible LLM mistake
  is a compile error" is code, not prose.

## 3. What broke and why

Nothing red reached a commit; two course-corrections mid-step, on record:
- The first lexer draft stored `text: &[u8]` inside the lexer struct — a
  reference in a struct, exactly what the Cyclone rule forbids. Rewritten
  before the first commit: `LexState` owns only offsets and output; the
  source enters every function as a parameter (the `@` shape).
- The first continuation proposal (depth-0 "no terminator → continue") was
  wrong and would have shipped without the panel's differentiated inputs:
  the engineer falsified it against the spec's own block headers in one
  pass. The fix cost ~30 lines; the bug would have cost every indented
  body in the language.

Open at close of M1 (tracked in `docs/debrief/QUEUE.md`): milestone debrief
offers (walkthrough, adversarial ratification, mutation drill, exit-quiz),
the escape-sequence panel (before M6), 007-bis predictions (at the
baseline). TextMate grammar (ROADMAP bonus) deferred. Verified at close:
23/23 snapshots, clippy clean, `heroes lex` live on examples/first.hero.
