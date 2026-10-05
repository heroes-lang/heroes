- [ ] **M-module-namespace step 5** | **Two modules, two `Point`s, two C structs.** `h_two_Point` and `h_geom_Point` in one translation unit, from `record Point` in each file. Question: what did the emitter take the module from before this step, and construct the two-module program that would have produced `error: redefinition` — then say why no existing golden caught it

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/ctype.rs · archive/bootstrap-rs/heroes/src/emit/decls.rs
    **Why it matters:** every single-file program has one module and it is the root's, so the whole corpus was blind to this by construction
