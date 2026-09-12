## Verify it yourself

### From a cold checkout, right now

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler, from C alone (~3.5 s)
./heroes run tests/harness/main.hero -- ./heroes             # the net
./heroes test tests/harness/main.hero                        # the net's own tests (121, 2026-09-07)
./heroes test selfhost/main.hero                             # the compiler's own tests
./heroes doctor                                              # the toolchain
```

**Rebuild `./heroes` before trusting it.** On 2026-08-25 the binary in the
repository root was nine hours stale and did not know a built-in that had landed
the evening before; a panel seat nearly filed that as a language defect. The
first line above is 3.5 seconds and removes the whole class.

### End to end, per milestone

In the chain's order. Every one of these is a command somebody can run today.

```sh
heroes doctor                                  # M-day-zero
heroes lex ex.hero --dump-tokens [--json]      # M-token-stream
heroes parse ex.hero --dump-ast                # M-syntax-tree
heroes fmt ex.hero [--in-place]                # M-syntax-tree (the flag was --write until panel 016)
heroes check ex.hero --json [--permissive]     # M-typed-frontend
heroes build ex.hero                           # M-ir-lowering: lowers, verifies, says so
heroes build ex.hero --dump-ir                 # M-ir-lowering (M-strings-ownership: increfs visible)
heroes build ex.hero --emit-c                  # M-scalars-run — and the determinism diff:
heroes build ex.hero --emit-c -o a.c && heroes build ex.hero --emit-c -o b.c && diff a.c b.c
heroes run examples/gallery/00-first.hero      # M-scalars-run: first native binary (-O2)
heroes test examples/calculator/whole.hero     # M-generics-library: acceptance
heroes test examples/calculator/main.hero      # M-module-namespace: the same tests, across modules
heroes run examples/sqlite/main.hero           # M-ffi-ladder: acceptance — open, query, close
heroes test examples/maze/main.hero            # M-program-corpus: one program (the harness runs them
                                               #   all, in three configurations — a directory
                                               #   argument is §10's question, not a given)
heroes mutate                                  # M-program-corpus: the rate over the enlarged corpus
heroes test selfhost/lexer.hero                # M-selfhost-probe: the ported lexer's own tests
# M-selfhost-fixpoint — the fixpoint, on generated C (the first line was
# `cargo run --` until the archive; `seed/README.md` is the live ritual):
heroes build selfhost/main.hero -o A
./A build selfhost/main.hero --emit-c -o B.c && clang … B.c -o B
./B build selfhost/main.hero --emit-c -o C.c && diff B.c C.c
heroes run tests/harness/main.hero -- ./heroes # the net (`cargo build && cargo test` until 2026-08-19)
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    # M-bootstrap-archive: the way in
heroes test selfhost/main.hero                 # the compiler's own tests
```

---
