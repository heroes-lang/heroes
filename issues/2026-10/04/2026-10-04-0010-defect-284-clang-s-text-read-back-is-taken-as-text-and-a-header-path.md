---
kind: defect
area: emit
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **284 — clang's text read back is taken as text, and a header path holding a byte that is not UTF-8 is unmeasured where clang prints it raw** | clang 21 escapes a byte that is not UTF-8 as `<E9>` in `#warning`, `#pragma message` and `#error`, its stderr UTF-8 each time (panel 189's critic, this Mac, 2026-10-04); what remains is a header under a directory whose name holds such a byte, which only Linux can make, named in a `#warning`: what clang prints there and what the compiler does with it is unrun (the critic's command: such a header built by `heroes build` on the trunk's compiler and on 227's route, in the Linux arm64 image) | the readers of clang's text (`selfhost/emit/clang_name.hero`, the build's warnings file) · panel 189's Q6, which listed this shape · **class: improvement**

    **Origin:** panel 189's critic, second pass (§ Q6, *unmeasured by every seat*); filed by the sitting's R12.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on Linux; no program measured wrong.
