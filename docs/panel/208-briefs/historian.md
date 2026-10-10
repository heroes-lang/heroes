# Panel 208, historian (advisory, no veto)

Read `00-shared.md` beside this file first. You have no build to run; your
instrument is web search, and **unsourced precedent is inadmissible**: every
date, version, issue number and quotation you give carries its URL, fetched in
this session.

The question: when a language that refuses to have warnings (or treats them as
errors) meets a deprecated API, what has it done, what did it cost, and what
did it change later? Find, with sources:

- **Zig**: `@deprecated()` and `-fallow-deprecated` (the version that added
  them, the proposal, what users said, and whether deprecation of a C
  function imported through `@cImport` is seen at all);
- **Go**: no compiler warnings by design; how `// Deprecated:` is surfaced
  (vet, staticcheck, gopls), and whether cgo passes C's
  `-Wdeprecated-declarations` through or silences it;
- **Rust**: `#[deprecated]` as a lint, `deny(warnings)` in CI and its
  well-known breakage when a dependency deprecates; bindgen's handling of C's
  deprecated attribute;
- **Swift**: `@available(*, deprecated)` warnings against `unavailable`
  errors, and how C headers' availability attributes cross into Swift;
- **Nim, Odin, C3, Hare, V** where they say anything (a language with no
  answer is a datum too);
- **C compilers in `-Werror` builds**: what projects do about a system
  header's deprecation (Apple's `sprintf` deprecation in macOS 13's SDK broke
  `-Werror` builds; find the record).

Write your report to `docs/panel/208-reports/historian.md` as you go: for each
precedent the decision, its date and source, what it cost its users, and
whether it was softened later; then which of the shared brief's routes
(R), (S), (M), (F), (N) the record supports and which it warns against, and one
prediction someone can score.
