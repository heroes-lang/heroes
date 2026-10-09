# Panel 201, historian's brief

Read `00-shared.md` beside this file first. Web search is your instrument;
every precedent sourced and dated, and an unsourced one said to be.

- **Q1 (488)**: how languages with inferred generics treat a generic function
  passed where a generic parameter of function type is expected: Rust
  (`fn app<A>(f: fn(A) -> A, x: A)` called with a generic `ident`), Swift,
  TypeScript (contextually typed generic function arguments, its higher-order
  function type inference of 3.4), OCaml and Haskell (let-polymorphism), Go's
  type parameters (inference from function arguments, 1.21's changes); which
  refuse and why, and the errors models are known to make there.
- **Q2 (467)**: how other languages state where `main` lives (Go's `package
  main`, Rust's `src/main.rs` and `fn main`, Zig, Nim), and whether any
  documents a reader adding `main` to a library file as a known mistake.
- **Q3 (520)**: compilers that warn or refuse mutual or indirect unbounded
  recursion (rustc's `unconditional_recursion` and its open issue on mutual
  recursion, clang's `-Winfinite-recursion`, GCC, Swift), and their false
  positives; what they say of recursion through a function value.

Report: verdict per question (advisory), the precedents with their sources
and dates, the conditions that would turn your reading.
