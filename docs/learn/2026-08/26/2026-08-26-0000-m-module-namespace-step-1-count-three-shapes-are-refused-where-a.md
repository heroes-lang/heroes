- [ ] **M-module-namespace step 1** | Count: three shapes are refused where a module name belongs — `use "geom"`, `use geom.shapes`, `use shapes/geom`. Exactly one carries a machine-applicable fix. Task: say which, then state the rule that decides it, using `.fixed`'s own contract (CI applies every certain fix and asserts the result checks clean)

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/decl.rs (`use_decl`, `is_module_name`) · tests/golden/check/use-is-quoted.fixed (2026-08-26)
    **Why it matters:** a `Certain` fix that leaves a second syntax error is worse than no fix (the fixture as it stood 2026-08-26)
