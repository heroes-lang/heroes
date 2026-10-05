- [ ] **M-module-namespace step 4** | **One `top_in` instead of a flat `get` is where "always qualified" lives.** The top-level table's key went from `name` to `(module, name)`. Question: why does an unqualified name fall back to the *library* module and to no other, and what would break if it fell back to every module instead?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`top_visible`) · docs/panel/031 R5
    **Why it matters:** the library is the one module every file sees without naming it, and that is not an exception to the rule
