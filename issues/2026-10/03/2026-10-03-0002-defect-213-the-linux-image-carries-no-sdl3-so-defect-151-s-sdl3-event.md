---
kind: defect
area: none
milestone: none
filed: 2026-10-03
commit: 091f60af8a8b50d6c4ad8bc6c035c41f5f833ae4
github: none
---

- [ ] **213 — the Linux image carries no SDL3, so defect 151's SDL3 event runs on this Mac alone** | `run/ffi-a-construction-polls-an-sdl3-event` in the arm64 container: *the package sdl3 is not installed on this machine*, skipped; the CI's Linux jobs and the Windows leg skip it too, by their totals | the `heroes-linux-arm64` image and the CI's install list · defect 151's closed record · **class: improvement**

    **Origin:** the coordinator's closings agent, 2026-10-03, and the author's answer *4a*.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage of one platform.

    **2026-10-07, lane b14-box**: Repaired at `091f60af`, gated by its cases in the new image and in `ubuntu:24.04`, and by `records` and `unseen`, no file of the compiler moving; the net is owed at the batch's close. Debian 13's `libsdl3-dev` 3.2.10+ds-1 joins both Linux Dockerfiles, the arm64 image rebuilt beside the old as `heroes-linux-arm64:sdl3-b14`, where the case and `examples/sdl/` gave their `.expected` at `-O0`, `-O2` and `--sanitize`, both told *not installed* in the old image; the CI's two Linux legs, Ubuntu 24.04 with no SDL3 package, build 3.2.10 from its source at commit `877399b2`, those lines run in `ubuntu:24.04` on arm64 to the same three readings, the x86-64 leg unrun; the Windows job, which has no `pkg-config`, is left as it was, the case's C reading its `.expected` on the Windows box against SDL's own 3.2.10 release.
