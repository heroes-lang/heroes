- [ ] **M-core-packages** | `compile "gfx.c"` waits for a witness, not for an argument | `docs/panel/114` § R6, R7 · `selfhost/cli/libraries.hero` · `examples/sdl/main.hero`

    **Origin:** settled out of `docs/work/DECIDE.md` on 2026-09-06 by author
    instruction, after `docs/panel/114` R6 closed the thin half the same day.

    **The thin case is answered and needs no language form**: one header the
    package ships with `#ifdef` inside and `static inline` wrappers, measured on
    macOS and on the Windows box, one `.hero` source, exit 0 on both, and
    `--emit-c` carrying zero platform words. SDL is that case and
    `examples/sdl/main.hero` binds it today with no platform word anywhere in
    the file; the `link` half is `package`, which panel 050 said *subsumes the
    platform axis panel 049 refused while putting no machine's name in any
    program*.

    **The thick case is refused by Principle 0 and not by a judgement about
    `compile`**: a wrapper with hundreds of lines of implementation wants a `.c`
    file the compiler builds, nothing in the closure list calls one, and no Part
    11 metric moves — so it waits, regardless of elegance (CLAUDE.md §2).
    **What is owed here is a WITNESS, not a decision**: a package in this
    milestone's own work whose C half is too large for a header of `static
    inline` functions. If it appears, panel 036's deferral reopens with its
    terms already priced — a group clause (`extern "gfx.h" compile "gfx.c"`) was
    panel 036 P2's own veto, so the package-level shape is the one that has
    never been judged, and panel 114 R7 has already recorded what a
    declaration-level form would cost. **If it does not appear in a milestone
    about packages that compose, that is the answer** and the deferral closes
    for good.

    **Where to look also:** `docs/panel/036-the-ffi-ladder.md:275` ·
    `docs/panel/050-*.md`.
    **Why it matters:** the one question a Heroes user asks that has a good
    answer for small libraries and no answer for large ones, and it has been
    waiting for a witness since August rather than for an argument.

    **Re-verified 2026-09-10: STILL OPEN, correctly waiting, and today's commit
    sharpened it.** `grep -rn 'compile "' selfhost/ spec/ design.md` returns one hit,
    `design.md:2152`, the record of panel 036's deferral; no surface form exists and
    `examples/sdl/main.hero` still carries zero platform words. **The whole tree now
    holds exactly one C file under `examples/`** — `examples/gallery/13-lease.h`,
    seven lines of `static inline` wrappers, landed 2026-09-10 with `7965174d`. That
    is the **thin** case again, which is panel 114 R6's own answer, so the deferral
    still has no thick witness and the wait is doing its job.
