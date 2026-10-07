---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **444 — SDL3's own `sdl3.pc`, from a source build with default options, is refused with `ffi_package` for `-Wl,--enable-new-dtags`** | SDL 3.2.10 built from source with default options into `/usr/local` writes `-Wl,--enable-new-dtags` into `sdl3.pc`, and a program binding `package "sdl3"` is refused `ffi_package` (lane b14-box's measurement, reproducer `<scratchpad>/batch14/box/ci/ci-step2.sh`, its output `ci-step2-out.txt`, not re-run by the coordinator); defect 213's CI step avoids it with `-DSDL_RPATH=OFF` | the link flags a package's pkg-config answer may carry (`selfhost/cli/`) · defect 213 · **class: systemic**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 1; the lane read `adjacent`).

    **Class: systemic**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, `blocking` by the letter, whose repair widens the flags a package may hand the link: a ruling no rule reaches, so a sitting's (CLAUDE.md § 4).
