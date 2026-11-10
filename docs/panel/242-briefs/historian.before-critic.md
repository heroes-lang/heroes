# Panel 242, the historian's brief

Read `00-shared.md` beside this file first. Your report, written as you go:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p242/docs/panel/242-reports/historian.md`.
Every line you mark `verified` carries the URL and the day you read it; unsourced precedent is
inadmissible.

## Question 1's precedent

For each, say whether the language's own compiler or toolchain depends on packages of its own
ecosystem, how they reach it (vendored in the tree, fetched at build, pinned by a lock), how the
bootstrap or seed handles them, and what its history had to correct:

- Go: `src/cmd/vendor`, `src/cmd/go.mod` and the `golang.org/x` modules the toolchain carries;
- Rust: rustc's dependency on crates of crates.io, `Cargo.lock` in `rust-lang/rust`, vendoring for
  offline and distribution builds, and the bootstrap;
- Cargo itself, built with crates of the registry it serves;
- Zig, Swift (the compiler and swift-syntax through SwiftPM), Gleam, and any other you find.

And whether any of them states a rule like design.md:570-571's (*no package is compiled into the
compiler*), or the opposite.

## Questions 2 and 3, briefly

A package manager's transport: which of Go, Cargo, pip/uv, npm run `git` and `curl` as programs
and which bind libcurl or a TLS library, and what moved them.

## Your verdict

Advisory, per question, with the precedents it rests on, each `verified` or marked unverified.
