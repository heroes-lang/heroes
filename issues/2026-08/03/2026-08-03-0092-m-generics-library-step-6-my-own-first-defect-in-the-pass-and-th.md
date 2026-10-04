- [ ] **M-generics-library step 6** | **My own first defect in the pass, and the verifier caught it.** A generic call *inside* a generic body records the template's own parameter — `wrap(x)` in `depth<T>` records `T`, not `str` — so instantiating from it verbatim carried `Ty::Generic` into the copy. The message was `an instruction still has a type parameter in it`. Task: say why the fix is `apply(callee_args, caller_args)` and not "collect from the template instead of the instance"

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mono.rs (`calls_in`) · archive/bootstrap-rs/heroes/src/ir/phases.rs
    **Why it matters:** the check was written before the pass and fired on its first run, which is the only reason this was five minutes rather than a milestone
