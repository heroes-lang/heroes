---
kind: defect
area: cli
milestone: M-rebel-library
filed: 2026-11-25
commit: none
github: none
---

- [ ] **A header one member of the platform layer reads stops every program's build, one that reaches no such member included** | In the Linux arm64 container, a `/usr/include/cups/cups.h` written after a build of the clock program (`tests/golden/run/the-platform-layer-clock-never-goes-back.hero`), which reaches `hero_time` alone, makes its next build exit 2, *internal error: the platform layer did not compile*, *compiling the platform layer's hero_printing failed*, and so does a build with no `build/`: every build compiles every member of the archive (panel 234's R6), and `hero_printing.c` includes CUPS's header wherever `__has_include(<cups/cups.h>)` finds one, holding its own copies of CUPS's records to that header by `_Static_assert` | `runtime/platform/hero_printing.c` (the CUPS probe at its line 1865), `selfhost/cli/layer.hero` (`compiled`), `selfhost/cli/layer_reach.hero` (`for_build`): a member that does not compile on this machine is told only to a program that reaches it, or the archive is built of what compiles and a link that takes a member left out is refused naming it · **class: adjacent**

    **Origin:** 2026-11-25, lane-layer-speed, measuring route (b) of `coord/layer-speed-routes.md` in `heroes-linux-arm64:latest` with the compiler of `0097b3f26` built from its emitted C by clang 22.1.8: a cold build of the clock program at exit 0, then `mkdir -p /usr/include/cups && printf '#error a cups.h installed after the build\n' > /usr/include/cups/cups.h`, then the same build warm and in a fresh folder, both exit 2 with clang's words naming `/usr/include/cups/cups.h:1:2`. Measured only with a planted header; whether a real `libcups2-dev` whose records differ from the layer's copies trips the same `_Static_assert`s is unrun, a question rather than a premise.

    **Class: adjacent**, 2026-11-25: found beside the layer's cost; a correct program refused, but shown only with a header planted to refuse, so not yet a measured `blocking`. Route (a) of `coord/layer-speed-routes.md`, an archive of the members a program reaches, would confine it to programs that reach printing.
