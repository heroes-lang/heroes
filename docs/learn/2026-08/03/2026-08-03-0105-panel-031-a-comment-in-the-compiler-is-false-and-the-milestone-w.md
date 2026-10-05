- [ ] **panel 031** | **A comment in the compiler is false and the milestone would have inherited it.** `mangle.rs` says "at M-module-namespace a module is a declared name rather than a file stem, so the shape stops being a stem-sanitising question at all" — but `module_of` strips `_`, so modules `geo_m` and `geom` are one component, and clang answers `redefinition of 'h_geom_Point'` on a legal program, which CLAUDE.md §7 makes exit 2. Task: say why the fix is a Heroes diagnostic naming both files rather than a ban on `_` in a module name

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mangle.rs · docs/panel/031 R10
    **Why it matters:** five of this compiler's own file names carry `_`
