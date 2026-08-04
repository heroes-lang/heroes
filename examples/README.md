# examples/

- `first.hero` — the compiler's first program (M5a target; spike 01 is its
  hand-written C shape).
- `gallery/` — nine programs written to be **read**: what the language looks
  like when it is used rather than tested. One theme each, in reading order:

  | file | what it shows |
  |------|---------------|
  | `01-points.hero`    | records, named construction, UFCS |
  | `02-tokens.hero`    | a variant with payloads, and exhaustive `match` |
  | `03-fallible.hero`  | `T?`, `fail`/`ok`, `?`, `.must()`, `.default()` |
  | `04-loops.hero`     | `for cond`, `for x in xs`, cells, `break`/`continue` |
  | `05-mutation.hero`  | `@` parameters: copy in, copy out, no aliasing |
  | `06-generics.hero`  | generics on functions, and functions as values |
  | `07-strings.hero`   | bytes versus characters, `slice`, `join` |
  | `08-ffi.hero`       | `extern`: everything else comes from C |
  | `09-holes.hero`     | `???`, and working skeleton-first |

  Every one of them is held to four properties by the test suite, because an
  example nobody runs is an example that rots: it **parses** clean
  (`printer::gallery`), it **resolves** clean (`resolve::tests::gallery`), it is
  in **canonical form** byte for byte — so no example teaches a layout
  `heroes fmt` would undo — and formatting **preserves its tree**. The gallery
  is therefore also the widest regression surface the frontend has.

- The **calculator** — the acceptance program — lives in design.md's appendix
  ("A complete example program"), its single source. The copy that sat here
  was 70% duplicate and drifted (pruned 2026-08-03). It returns as a real
  file at M6, generated from the appendix, when `heroes test` can run it.
