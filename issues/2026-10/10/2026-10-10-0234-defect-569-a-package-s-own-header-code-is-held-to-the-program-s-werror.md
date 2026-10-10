---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **569 — a package's own header code is held to the program's `-Werror`, refusing GMP and libavutil on this Mac** | panel 204's ffi-pragmatist and its critic: `gmp.h` (line 1882) and FFmpeg's `libavutil` are refused on this Mac by `-Werror=sign-conversion` firing inside the libraries' own inline code, before any binding is checked (`p/gmpmac/g.hero`, `av`), on today's compiler and on panel 204's prototype; a header of the program's own with a diagnostic pragma around the include builds and prints `6`; the policy is panel 198's ruling, which hands a package's `-isystem <dir>` on as `-I<dir>`, and panel 204 R2's `-Werror=macro-redefined` widens the same policy, refusing Expect's `EXP_ABORT` with libjpeg's `JPEG_LIB_VERSION` in both orders | the compile flags `selfhost/cli/flags.hero` and the package's include directories, `selfhost/cli/` (panel 198's ruling); panel 204 R5, for a sitting of its own · **class: blocking**

    **Origin:** filed by the coordinator at 02:34 on 2026-10-10 from panel 204 (`docs/panel/204-c-reads-a-module-s-headers-in-the-order-its-groups-are-written-and-the-spec-says-so.md`, R5), the seats' measurements, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused.
