---
kind: defect
area: emit
milestone: none
filed: 2026-10-04
commit: 49231969ad2b25a9e761bc168f7c835bd00dac41
github: none
---

- [ ] **284 — clang's text read back is taken as text, and a header path holding a byte that is not UTF-8 is unmeasured where clang prints it raw** | clang 21 escapes a byte that is not UTF-8 as `<E9>` in `#warning`, `#pragma message` and `#error`, its stderr UTF-8 each time (panel 189's critic, this Mac, 2026-10-04); what remains is a header under a directory whose name holds such a byte, which only Linux can make, named in a `#warning`: what clang prints there and what the compiler does with it is unrun (the critic's command: such a header built by `heroes build` on the trunk's compiler and on 227's route, in the Linux arm64 image) | the readers of clang's text (`selfhost/emit/clang_name.hero`, the build's warnings file) · panel 189's Q6, which listed this shape · **class: improvement**

    **Origin:** panel 189's critic, second pass (§ Q6, *unmeasured by every seat*); filed by the sitting's R12.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on Linux; no program measured wrong.

    Repaired at `49231969`, 2026-10-05 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The measurement, run in the Linux arm64 image on the trunk's compiler of `ca5fa51e` with Debian clang 22.1.8, a group's header under `/t/caf<0xE9>/` reached through `CPATH`: clang prints the path with the byte as it is, and the build forwards the header's `#warning` with the byte written by its value at exit 0, tells a missing include it holds as `ffi_missing_header` and an extern it refutes as `ffi_return_type`, each at exit 1. No reader changed; clang's words are committed as bytes and a compiler test reads them as the build does.
