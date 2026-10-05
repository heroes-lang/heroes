- [ ] **M-generics-library step 4** | **Three places hand a program back and all three had to learn about the library**: `check --apply`, `fmt`, and `fmt --in-place`. Count: what would each have written into the author's file, once per invocation, without `user_text()`? Then say why the *comments* needed filtering separately from the declarations

    **Where to look:** archive/bootstrap-rs/heroes-cli/src/commands/check.rs · archive/bootstrap-rs/heroes/src/printer/fmt.rs
    **Why it matters:** a formatter that appends a library to the file it formats is a fixpoint that never closes
