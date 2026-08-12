# Open panel sessions — awaiting the author's decision

**None.** All four sessions opened by panel 000 were decided by the author
on 2026-08-03 (all on the recommended resolution) and confirmed by full
five-judge sessions the same day:

- 002 — `ok(x)` constructor and `fail`'s typing → `002-ok-constructor.md`
- 003 — statement-position rule → `003-statement-position.md`
- 005 — Principle 0 formal adoption → `005-principle-0.md`
- 006 — map iteration order and `print`'s contract → `006-map-order-print.md`

Decisions are applied to design.md (the living document). **Spec v0 stays
frozen** until the pre-amendment baseline run; the v1 amendment texts are
recorded in each session file, and the spec-warden's package condition binds
them: v1 lands together with the cut of the spec's 53-word header comment,
or it breaches the 1500-token budget.

## Watch list (not open sessions — conditions and gaps on the record)

- **§1.6's scope for `spec/reserved-words.md` is undecided** (spec-warden,
  013): 831 measured tokens saying which words are legal — semantics, and a
  diagnostic class — currently uncounted by `heroes measure`, whose default
  is `heroes-spec.md` alone. True headroom is 952 or 121 depending on the
  answer. Bigger than anything panel 013 decided.
- **`heroes measure` cannot reproduce panel 011's o200k number** (2050): only
  two tables are vendored, so the binding maximum reports 2048. Immaterial to
  every verdict so far; on the record so the instrument stays honest.
- **The foreign-word registry collides with C identifiers that bindings must
  name** (ffi-pragmatist, 013; pre-existing): in SDK headers, declarator or
  field position, `function` occurs 571 times, `func` 29, `assert` 11,
  `test` 5, `match` 3 — and §4.19 has no alias syntax. Belongs to §4.19's
  deferred annotation vocabulary at M7.
- **Closures (v1.5) break the C-ABI compatibility of function values**
  (ffi-pragmatist, 013): a capturing closure is a record plus a pointer, so
  the type system must distinguish capture-free at the boundary.
- **Newline-separated parameter lists**: §4.9 gives newline separation to
  multi-line *literals*; nothing says whether a signature may use it. The M2
  parser requires `,` (the narrower reading) and reports the newline form.
  Decide when `heroes fmt` picks the canonical long-signature shape (M2
  step 4) — a panel path, since it is surface syntax.
- **`T??` in the surface**: §4.6 refuses the level ambiguity but never says
  whether the *written* form is rejected. The M2 parser rejects it with
  `nested_fallible` (conservative: a rejection can be relaxed, an acceptance
  cannot be withdrawn).

- `()` is used by the 003 rule but never defined in the spec's type table;
  owes a row if the baseline shows confusion (spec-warden, 003).
- The M6 closure-list audit is scoped by three riders: mechanical,
  spec-coverage with named cuts, tier assignment for file I/O / `args()` /
  `exit(code)` (panel 005).
- Ordering-sensitive `BTreeMap` walks in the Rust bootstrap get `// ORDER:`
  markers as they are written; each becomes an explicit sort in the port
  (compiler-engineer, 006).
- Canonical `f64` rendering: deterministic, locale-independent; exact
  algorithm fixed at M5b with its goldens (ffi-pragmatist, 006).
- ~~CLAUDE.md's "Nim ≈150k lines"~~ — resolved by removal (2026-08-04): the
  claim was dropped in CLAUDE.md's single-sourcing rewrite; the repo no
  longer asserts a line count for Nim anywhere (historian, 005, satisfied).
- ~~Terminator ender-list gap (M1.1)~~ — **resolved by panel 007** (author
  ratified 2026-08-03): ender list completed, continuation inside brackets
  only, unclosed opener = EOF error.
- **Depth-0 trailing-operator continuation + the spec-v1 layout sentence
  (007-bis, keyed to the baseline run):** deferred as one package. Score at
  the baseline: ergonomist predicts ≥25pp first-try loss on depth-0
  expression breaks with a silent spec; spec-warden predicts zero baseline
  completions contain such a break. If the warden is wrong, reconvene with
  Nim's explicit continuator set + the sentence (+ a named spec removal and
  a real tokenizer count).
- ~~Escape sequences do not exist (M1.3)~~ — **resolved by panel 008**
  (author ratified 2026-08-04): five escapes split by context, backslash
  reserved, set frozen.
- **Raw string literals (no panel yet, no urgency):** panel 008's
  implementation found that `"C:\temp"` cannot be made loud — `\t` is legal,
  so the path silently becomes `C:<TAB>emp`. Every C-style-escape language
  carries this; the standard remedy is raw strings (Go backquotes, Rust
  `r#"…"#`, Swift SE-0200), which are v2 material at best. Recorded so it is
  not rediscovered as a bug.
- ~~Repeated `@` arguments are a silent divergence~~ — **resolved by panel
  010** (adopted provisionally 2026-08-04, ratification queued): two `@`
  arguments sharing a root binding are a compile error. Original statement: `shift(a @ n, b @ n)` — the
  same variable passed twice as a mutable argument — compiles under both
  readings of §4.8 and yields *different answers* (copy-in/copy-out:
  last write wins; reference semantics: the writes see each other). Every
  mainstream language installs the reference model, so a model will guess
  wrong silently. Proposed fix is not spec prose but a compile error on
  repeated `@` arguments.
- **The harness needs a compiles-but-wrong-output bucket** separate from the
  compile-error bucket (ergonomist, 009): without it the thesis's central
  claim — that silent errors are what the design eliminates — has no
  instrument, and panels 008/009's predictions are untestable.
- **The spec never says what a `constant`'s body may contain** (reasoning 003).
  `spec/heroes-spec.md` shows a single literal (`constant MAX_DEPTH: int` / `64`)
  and every `constant` in the corpus is one. The compiler accepts an arbitrary
  block: arithmetic, a call to a user function, an `if`/`else`, a multi-statement
  body, a `str` concatenation and an array literal all compile and run, verified
  at `2ac0403`. No golden pins the difference, so a reader with only the spec
  cannot know the form is general — panel 035's category exactly, and the
  measurement of the sentence is owed before anyone decides whether the
  generality is intended or accidental.
- **Part 6 names `Macros` and is silent on compile-time evaluation** (reasoning
  003). The mechanism appears once in the entire record, at design.md:1821, as a
  remark about Zig. Recorded here so the silence is a known gap rather than an
  invitation; the decision to convene panel **039** on it is in
  `docs/debrief/DECIDE.md`.
