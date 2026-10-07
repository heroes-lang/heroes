---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **447 — the CI's Windows leg carries no SDL3, so the SDL3 event case runs on Linux and Darwin only** | defect 213 added SDL3 to the Linux legs; the Windows runner image has vcpkg but no pkg-config, so the route would be `vcpkg install sdl3 pkgconf`, and the `package` path has never run on Windows; MSYS2's SDL3 is probably the wrong build for clang's MSVC target (lane b14-box's inference, unrun) | `.github/workflows/ci.yml`, the Windows job · defect 213 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*for you to decide* 1, the lane's recommendation: its own item, measured on the box before any `ci.yml` change).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage of one platform.
