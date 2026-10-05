- [ ] **Moved from `DECIDE.md` 2026-08-12 — `/decide` step 1** | M-literal-bases step 1 | `leading_zero` carries **two** fixes and neither is `certain`, while `base_prefix_case` carries one that is. Question: state the rule that decides which, in one sentence, without using the word "confidence". Then say what would have gone wrong had `0700`'s fix been tagged `certain` — be specific about what `heroes check --in-place` would have done to a file

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/number.rs · tests/golden/check/leading-zero.hero · CLAUDE.md §8
    **Why it matters:** a machine-applicable fix that picks one of two readings is the defect wearing a repair's clothes
