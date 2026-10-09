---
kind: defect
area: emit
milestone: none
filed: 2026-10-08
commit: 79285686dd133f2051fbfeb2c6770e3a1187d1e9
github: none
---

- [ ] **508 — a function whose every path calls itself runs for ever at `-O2`, where the spec says recursion too deep aborts** | `function forever(n: i64) -> i64` returning `forever(n)`, called from `main`, built at `-O2`: exit 0; its run is killed by `timeout 10` at exit 124 with nothing printed (run by the coordinator before 07:48 on 2026-10-08 on the trunk's compiler, `<scratchpad>/batch15/serve/forever.hero`); clang turns the self-call into a jump; `heroes run` builds at `-O2` by default (`selfhost/cli/verbs.hero`), and the spec says *Recursion too deep aborts* (`spec/heroes-spec.md:280`); found by panel 199's completeness critic, with mutual recursion, a self-call through a function value and a UFCS wrapper hanging the same way | the emitted C at `-O2` · `spec/heroes-spec.md:280` · defect 457 · panel 199 · **class: blocking**

    **Origin:** filed by the coordinator at 07:48 on 2026-10-08 from panel 199's completeness critic's first pass (finding 1), reproduced before filing. Its route (`-fno-optimize-sibling-calls` or its equal, the spec naming the level, or a rule refusing the shape) is panel 199's to choose.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): the spec's sentence false at the default level of `heroes run`, a program that hangs where it is promised an abort.

    Repaired at `79285686`, 2026-10-09 (lane land199, panel 199's R2, ratified with *unbounded recursion is an abort*), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `-fno-optimize-sibling-calls` is a word of `flags.flags()` at every level, since at `-O0` the objects are byte-identical with it (Apple clang 21): `ping` and `pong`, a self-call through a function value and a self-call behind a helper that may exit ran to exit 124 at `-O2` and abort 134 now, on this Mac (clang 21.0.0) and in the Linux arm64 container (clang 22.1.8); a tail recursion ten million deep aborts at `-O2` too and a hundred thousand still prints there (the surface row `deeptail`); the compiler built at `-O2` checks +0.11% to +0.32% in instructions. Owed (panel 199's R2): the Windows box's clang 23.1.1, offline on 2026-10-09, and the CI's clang 18.1.3 and 20.1.8.
