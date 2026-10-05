- [ ] **M-value-aggregates step 1** | Count: the corpus invariant runs over **66** accepted programs and only **14** aggregate declarations. Question: why so few, and which of `heroes mutate`'s ten operators could ever produce a containment cycle? (The answer decides whether `no_size` can appear in Metric 3 at all.)

    **Where to look:** archive/bootstrap-rs/heroes/src/types/tests/sizes.rs · harness/mutations/operators.md
    **Why it matters:** an invariant over a corpus with no instances is a rule nobody is following
