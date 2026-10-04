- [ ] **panel 105** | **What does the verifier refuse that the checker accepted?** Run `heroes build --dump-ir` on `tests/golden/run/fixedbugs-an-empty-result-bound-by-its-context.hero` and count the `function empty_of` lines. Then predict: if the checker had left `A` unbound, how many would there be, and which check in `selfhost/ir/phases.hero` would fire first — `no_generic_survives` or `no_error_survives`?

    **Where to look:** selfhost/ir/phases.hero, the golden
    **Why it matters:** the difference between a type parameter that survived and a type nobody resolved is the difference between two bugs with the same symptom
