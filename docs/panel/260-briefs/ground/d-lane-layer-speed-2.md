---
kind: defect
area: cli
milestone: M-rebel-library
filed: 2026-11-25
commit: none
github: none
---

- [ ] **A program binding `hero_ui.h` does not link on Linux arm64 at `-O0` or `-O2`: the GTK arm's parts call `nextafter`, and nothing puts `-lm` on the link** | In `heroes-linux-arm64:latest` (Debian clang 22.1.8), the compiler of `main` at 849bb174a built from its emitted C builds a four-line program binding `hero_ui_native_here` from `hero_ui.h` at exit 2, *internal error: linking failed*, `/usr/bin/ld: build/layer-….a(hero_ui_parts_gtk.o): in function hero_ui_parts_gtk_act: hero_ui_parts_gtk.c:1260: undefined reference to nextafter`, then *the linker refused the objects clang compiled*; the same program under `--sanitize` links and runs (`native: true`), the sanitizers' runtime bringing libm, and `print(1)` builds at exit 0. The seed of 0521cd903 and this lane's compiler of `0097b3f26` exit 2 the same way at `-O0` and `-O2`. libm is on a link only where a program writes `link "m"` (`libraries.in_the_c_runtime`), and a member of the platform layer cannot | `runtime/platform/hero_ui_parts_gtk.c:1260` (`nextafter`; `hero_ui_parts_win32.c:464` calls it too, where the C runtime holds it), `selfhost/cli/link.hero` and `selfhost/cli/supplied_kept.hero` (the needs a member records: a library a taken member needs reaches the link line, or the member calls nothing outside the C library the link already carries) · **class: blocking**

    **Origin:** 2026-11-25, lane-layer-speed, timing a program reaching the UI in the Linux arm64 container as its brief asks: every compiler measured exited 2 on it at `-O0` and `-O2` and built it under `--sanitize`. Reproducer, in the container: `printf 'extern "hero_ui.h"\n    function hero_ui_native_here() -> i64\n\nfunction main()\n    print(f"native: {hero_ui_native_here() >= 0}")\n' > ui.hero && heroes build ui.hero -o bin`.

    **Class: blocking**, 2026-11-25: a correct program refused at exit 2, *internal error*, on a platform of the CI.
