---
kind: decision
area: none
milestone: none
filed: 2026-08-12
commit: 486c75647ea9f87269d2d38803561b8476c6306f
github: none
---

- [x] Covered | panel 021 | **DONE 2026-08-12 (author: unify — the level becomes a flag).** `-O0` and `-O2` are flags on `build`, `run` and `test`; each verb keeps its default (`build` -O0, `run` -O2, `test` -O0) and the flag overrides it. Spelled attached, as clang spells it, so the flag a reader already knows is the flag that works. **Two levels and no others**, which is §10's stopping rule applied to a flag rather than a verb: the `run/` golden harness types `-O0` and `-O2` — every case runs at both — and nothing types `-O1`, `-O3` or `-Os`, so `-O1` is a strictness error naming what is accepted. Both levels at once is refused rather than resolved by precedence, the same reading `build` gives `--dump-ir --emit-c`. The level was already in the cache key, so the two builds were always two directories; what changed is that `build` can now be *asked* for the level the harness runs at. Was: Optimisation level is chosen by **verb** (`build` = -O0, `run` = -O2) and sanitising by **flag** | crates/heroes-cli/src/commands/compile.rs (`level_from`) · crates/heroes-cli/src/cli/table.rs | a surface that grew by accident is the thing the stopping rule exists to prevent
