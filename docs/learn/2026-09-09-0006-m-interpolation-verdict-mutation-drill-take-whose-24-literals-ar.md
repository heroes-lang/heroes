- [ ] **M-interpolation-verdict, mutation drill** | Take `examples/template/main.hero`, whose 24 literals are the only hole-shaped braces in the tree, and say what would happen to each under an ungated brace, then under the `f` gate; count the loud failures and the silent ones | `examples/template/main.hero` · `docs/measurements/022` § The brace migration | 312 literals hold a brace, 211 in the compiler itself, and the gate is what made a syntax change not a two-stage bootstrap

    **Origin:** M-interpolation-verdict close, 2026-09-09.
