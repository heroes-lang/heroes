# Debrief queue — comprehension owed, never blocking

Development appends; `/debrief` sessions tick off. Checked items stay (the
queue is also the record). Format:

`- [ ] <origin> | <question / task> | <where to look> | <why it matters>`

## Open

- [x] M0 | Explain-it-back: what does spike 04 decide, and why does it exist before any compiler code? | tools/spike/04-variant.c · docs/journal/000-setup.md §5 | it fixed the runtime container + descriptor ABI that M5c will implement
- [x] M0 | Milestone offers: superseded by M1's drill (same lexer, live symptom); exit-quiz declined for now | docs/journal/000-setup.md | optional, on request — see /debrief
- [x] M0 | Decide: retro-tag `m0`? → tagged 2026-08-04 at 02c4ca1 (M0 closed before the tagging habit) | git log 02c4ca1 | tags are the chronology the book and /where rely on
- [ ] harness | Metric 2 minimum viable protocol (panel 011): local open-weights model, or 2 pre-registered tasks per milestone. NOT an API-key question | docs/panel/011-measurement-without-an-api-key.md | ~16 samples by v1 beats zero; the pre-amendment number is still unrecoverable later
- [ ] harness | Held-out tasks must be author-written (assistant-written tasks would measure the assistant's priors); n now scales with the paced protocol, not 15 at once | harness/tasks/README.md | metric 2 validity
- [ ] panel 011 | Ratify: budget as hard measured ceiling 2000 (max over two vendored tokenisers), metric 3 primary-but-never-pooled with three arms, metric 2 via local model or paced sampling | docs/panel/011-measurement-without-an-api-key.md § Resolution | the repo is IN BREACH today at 2050 — this decides what happens next
- [ ] tooling | Implement `heroes measure` offline: two vendored BPE tables (sha256-pinned), ~180 lines, zero deps; port debt at M8c needs file I/O + maps in Heroes | panel 011 | the rule 'measured-only' is unenforceable until this exists
- [ ] pre-M3b | Paper exercise: hand-check five expressions in two columns (⇐ checking / ⇒ synthesis) | glossary entry to be born from this session | M3b is the conceptual cliff; the exercise should precede it
- [ ] pre-M4 | Hand-desugar `for x in xs`, `?`, and one UFCS chain from the appendix calculator | design.md Part 5 | the desugarer lands in M4
- [x] M1.1 | First tokens. How many tokens does the line `print((2 + 3) * 4)` produce, counting neither layout tokens nor the terminator — 8, 10, or 12? | crates/heroes/src/lexer/tests.rs (first_hero_end_to_end) | reading a token dump is reading what the parser will see
- [x] M1.1 | `for` and `print`: one is a keyword token, one a plain identifier. Which is which, and why does the parser care about the difference? | spec/reserved-words.md § keywords | keywords carry structure; identifiers are just names
- [x] M1.1 | A line ends with `+`; the next starts a new expression. Does the lexer emit a terminator after the `+` — yes or no? And after a `)`? | crates/heroes/src/lexer/layout.rs (is_line_ender, design.md §4.15) | this one rule is how Heroes lives without semicolons
- [x] M1.1 | An 8-space indent follows a 0-space line: one Indent token, two Indent tokens, or an error? | lexer/tests/adversarial.rs | rigid indentation is the layout bet — errors, never interpretation
- [x] M1.1 | Ratify the 5 adversarial lexer cases (tab in margin, 5-space indent, 2-level jump, tab mid-line, single `&`) — say what each guards against | crates/heroes/src/lexer/tests/adversarial.rs, marked UNVERIFIED | they become check/ goldens when the runner lands
- [x] M1.1 | Design gap found: §4.15's terminator ender-list omits `break`/`continue`/`???` — panel session needed before M2 | resolved: docs/panel/007-terminator-enders.md, author ratified live 2026-08-03 | the parser cannot land on an ambiguous line-ending rule
- [x] M1.3 | How would a Heroes program express a real newline character? (When asked: it couldn't) | resolved by panel 008 → the M1.5 items below | the self-hosted lexer needs '\t' without magic numbers
- [ ] M1.4 | Write `let x = 5` in a .hero file and run `heroes lex` on it: what comes out, and what travels attached to the diagnostic? | spec/reserved-words.md · lexer tests (certain_fix_travels_with_the_diagnostic) | the thesis made executable: the likeliest LLM mistake fails loudly with the repair pre-written
- [x] M1 close | Milestone debrief offers (walkthrough + adversarial ratification + mutation drill done; exit-quiz declined): lexer walkthrough (16 token kinds end to end) · ratify the 5 adversarial cases · mutation drill · exit-quiz (re-implement `is_line_ender` or `char_lit` on a throwaway branch) | /debrief | all optional, author's call; doing it by hand first is the lesson
- [x] M1.5 | `"a\nb"` used to compile printing four characters. Now it prints two lines. Which half of panel 008 did that — the five escapes, or reserving the backslash? | docs/panel/008-escape-sequences.md · lexer/escape.rs | the reserved backslash is the half that does the thesis work
- [x] M1.5 | `"C:\temp"` still compiles silently as `C:<TAB>emp`. Why can no escape set fix that, and what would? | lexer/tests/literals.rs (the_residual_windows_path_trap_is_on_the_record) | knowing the limits of a fix is part of owning it
- [x] panel 009 | Ratify the budget governance adopted provisionally: measured-only, soft 1500/hard 2000, the +500 as a pre-allocated purse, spend only where a wrong guess is silent | docs/panel/009-spec-budget-2000.md § Resolution | it is the rule that keeps 2000 from becoming 2500
- [ ] panel 010 | Ratify (or overturn) the repeated-`@` rule adopted provisionally: two `@` arguments sharing a root binding are a compile error, over-rejecting distinct-index pairs by design | docs/panel/010-repeated-mutable-arguments.md § Resolution | three judges converged; Ada reached the same rule in 2012 after 33 years of the alternative

## Covered

- 2026-08-04 — M0/M1 closed out: spike 04 walkthrough written into journal 000 §5, mutation drill run and recorded in journal 001 §4 (the diagnostic lied — the bug was in the bookkeeping it reads), `m0` retro-tagged. Exit-quiz declined; it stays available on request.

- 2026-08-04 — Ratified: the 5 adversarial lexer cases (rationale now on each case; the UNVERIFIED marker is gone) and panel 009's budget governance in full.
- 2026-08-04 — M1 concepts (first tokens, keyword vs identifier, terminator insertion, indentation recovery) and the two escape questions. One divergence, distilled into `docs/glossary/001-expressible-vs-loud.md`: making something *expressible* and making a mistake *loud* are separate jobs with separate prices, and the panel voted them separately.
