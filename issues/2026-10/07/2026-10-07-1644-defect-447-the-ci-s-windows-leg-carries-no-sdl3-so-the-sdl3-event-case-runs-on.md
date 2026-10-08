---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: 864df5a27695ad49a0798b0493bf2a9f48448b91
github: none
---

- [ ] **447 — the CI's Windows leg carries no SDL3, so the SDL3 event case runs on Linux and Darwin only** | defect 213 added SDL3 to the Linux legs; the Windows runner image has vcpkg but no pkg-config, so the route would be `vcpkg install sdl3 pkgconf`, and the `package` path has never run on Windows; MSYS2's SDL3 is probably the wrong build for clang's MSVC target (lane b14-box's inference, unrun) | `.github/workflows/ci.yml`, the Windows job · defect 213 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*for you to decide* 1, the lane's recommendation: its own item, measured on the box before any `ci.yml` change).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage of one platform.

    **2026-10-08, lane b15-box**: Repaired at `864df5a2`, gated by its case, the emission it moved and the records, where **the Windows leg of the CI must run first** (the step has not run on a runner) and the net is owed at the batch's close. Measured on the Windows box (Windows 10.0.26100, clang 23.1.1 driving lld-link, vcpkg tool 2026-09-26): `vcpkg install sdl3:x64-windows-release pkgconf:x64-windows-release` took 1.7 minutes and gave SDL3 3.4.18 and pkgconf 3.0.3; the `package` path ran there for the first time, the compiler's `pkg-config` found by copying vcpkg's `pkgconf.exe` under that name beside its DLL, answering `-IC:/... -LC:/... -lSDL3`, which the allow-list passed, `SDL3.dll` found through PATH and its absence a load error at run time; the case as it stood was `ffi_field_type`, SDL_KeyboardEvent's `type` and `scancode` being `int` on Windows and `unsigned int` on macOS and Linux (C measured on all three), so its third union member is SDL_CommonEvent, and the case and `examples/sdl/` printed their `.expected` on the box at -O0, -O2, --sanitize and --sanitize -O0, the case also on this Mac and in the Linux arm64 container; `ci.yml`'s Windows job gains the step, its body the script that ran on the box with the runner's variables simulated, and the one emission the case moves is re-blessed (1,002 and 0, read in full). Not run: the step on a runner; this lane's own compiler on the box (the coordinator's batch-14 `heroes.exe` built and ran them, and no file of `selfhost/` moved for this item); the runner's MSVC `link.exe`, the box linking with lld-link; and MSYS2's SDL3, which the item guessed to be the wrong build for clang's MSVC target and nothing measured.
