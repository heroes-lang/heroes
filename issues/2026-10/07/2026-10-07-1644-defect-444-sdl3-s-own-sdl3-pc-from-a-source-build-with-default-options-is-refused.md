---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: f9852b6e382a6c467241e220a908346443d5a5b1
github: none
---

- [ ] **444 — SDL3's own `sdl3.pc`, from a source build with default options, is refused with `ffi_package` for `-Wl,--enable-new-dtags`** | SDL 3.2.10 built from source with default options into `/usr/local` writes `-Wl,--enable-new-dtags` into `sdl3.pc`, and a program binding `package "sdl3"` is refused `ffi_package` (lane b14-box's measurement, reproducer `<scratchpad>/batch14/box/ci/ci-step2.sh`, its output `ci-step2-out.txt`, not re-run by the coordinator); defect 213's CI step avoids it with `-DSDL_RPATH=OFF` | the link flags a package's pkg-config answer may carry (`selfhost/cli/`) · defect 213 · **class: systemic**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-box's final report (*found beside* 1; the lane read `adjacent`).

    **Class: systemic**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, `blocking` by the letter, whose repair widens the flags a package may hand the link: a ruling no rule reaches, so a sitting's (CLAUDE.md § 4).

    Repaired at `f9852b6e`, 2026-10-09 (lane b16-land198, panel 198's R1, R3 and R6, ratified by the author that day), with `a0cf7880` (the note) and `29b7aa93` (the CI), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and this item closes after the CI's legs (R8). A package's `-pthread` reaches the compile and every link but Windows', `-isystem <dir>`, joined or as the next word, is handed on as `-I<dir>`, `-Wl,--enable-new-dtags` and `-Wl,--export-dynamic` are accepted and not sent to the linker, and `-Wl,-rpath` is not sent to lld-link; `--disable-new-dtags`, the position-dependent words (libpsx's whole-archive bracket, the one Debian 13 package still refused), `-W...`, `-Xlinker` and a comma-tunnelled word stay refused. The refusal's note names the list's five tables word for word, held to them by a compiler test, its reason clause true of every refused word. The CI builds SDL3 with `SDL_RPATH` at its default. Cases: six compiler tests and two `absence.hero` cases; the compiler's own tests 1520 passed, the net's own tests 321 passed (the base 1512 and 318).
