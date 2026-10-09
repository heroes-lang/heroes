---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 61624d0f2cf948cbc865148a5bd08d822a55b659
github: none
---

- [ ] **538 — `heroes test` tells two groups binding one C name with different types that one header does not compile** | two modules binding `a.h` and `b.h`, each defining a `static inline twice` of another type: `check` and `build` exit 0 and the program prints 6 and 8, while `heroes test` exits 1 with *`b.h`, the header this group names, does not compile*, and with the `use` lines swapped blames `a.h`; each header compiles alone; panel 200's route B for defect 453 would make `--emit-c` say the same; what `check`, `build` and `test` should say of such a program is a diagnostic class | `selfhost/emit/` and `selfhost/cli/whose.hero` (`ffi_header_refused`) · panel 200 R1 · defect 453 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the header blamed chosen by the order of the `use` lines.

    Repaired at `61624d0f2cf948cbc865148a5bd08d822a55b659`, 2026-10-09, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where clang's note under the first error points into another group's header, `ffi_header_refused` now tells both headers, both lines clang reports, the other group's place and that *these cannot be compiled in one unit*, on the group whose header sorts first by bytes, the same words in either order (`cli/headers_together.hero`); panel 200's two-module program under `heroes test` says the same in both `use` orders, where the base blamed `b.h` and then `a.h`. No new class: what `check`, `build` and `test` should say of two groups binding one C name with different types stays panel 200 R1's question for a full sitting. Cases `unsupported/fixedbugs-538-*`, one module's two groups in both orders, red on the base, annotated: unsupported 2 and 0, annotations 2 and 0, the readers of clang's quoted line (fixedbugs-360, macro, fixedbugs-143) 32 and 0, the compiler's own tests 1,532 passed. Beside it, the same cause unrepaired: where the conflicting definition sits in a header another group's header includes, clang's note names that file, which no group names, and the old message stands in one order (the other order is told of both); telling it order-free needs the include graph.
