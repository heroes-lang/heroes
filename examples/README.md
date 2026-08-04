# examples/

- `gallery/` — twelve programs written to be **read**: what the language looks
  like when it is used rather than tested. One theme each, in reading order,
  opening with the smallest program worth compiling and ending with the shapes
  the middle end needed:

  | file | what it shows |
  |------|---------------|
  | `00-first.hero`     | the compiler's first program — M5a's target, and spike 01 is its hand-written C shape |
  | `01-points.hero`    | records, named construction, UFCS |
  | `02-tokens.hero`    | a variant with payloads, and exhaustive `match` |
  | `03-fallible.hero`  | `T?`, `fail`/`ok`, `?`, `.must()`, `.default()` |
  | `04-loops.hero`     | `while cond`, `for x in xs`, cells, `break`/`continue` |
  | `05-mutation.hero`  | `@` parameters: copy in, copy out, no aliasing |
  | `06-generics.hero`  | generics on functions, and functions as values |
  | `07-strings.hero`   | bytes versus characters, `slice`, `join` |
  | `08-ffi.hero`       | `extern`: everything else comes from C |
  | `09-holes.hero`     | `???`, and working skeleton-first |
  | `10-maps.hero`      | `{K: V}`, and `m[k]` returning a `V?` — absence as a type |
  | `11-trees.hero`     | a recursive variant: the array is the only indirection |

  Every one of them is held to six properties by the test suite, because an
  example nobody runs is an example that rots: it **parses** clean
  (`printer::gallery`), it **resolves** clean (`resolve::tests::gallery`), it
  **type-checks** clean (`types::tests::acceptance`), it **lowers and verifies**
  (`ir::tests::acceptance`), it is in **canonical form** byte for byte — so no
  example teaches a layout `heroes fmt` would undo — and formatting **preserves
  its tree**. The gallery is therefore also the widest regression surface the
  compiler has, and `heroes mutate` uses it as the corpus it makes mistakes in.

- The **calculator** — the acceptance program — lives in design.md's appendix
  ("A complete example program"), its single source. The copy that sat here
  was 70% duplicate and drifted (pruned 2026-08-03). It returns as a real
  file at M6, generated from the appendix, when `heroes test` can run it.
