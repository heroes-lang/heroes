- [ ] **M-module-namespace step 3** | **A diagnostic in a non-root module named the root file and a line nobody could find in it.** `render_line` and `render` each printed `src.name` and `line_col` directly — the same answer while there was one file, a *false* one the moment there were several. Task: say why `render` needs **two** line numbers now, which one is printed and which one slices the text, and why the repair was one function on `Source` rather than two fixed call sites

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`locate`) · archive/bootstrap-rs/heroes/src/diagnostics/render.rs
    **Why it matters:** well-formed and wrong is the failure mode a green suite cannot see
