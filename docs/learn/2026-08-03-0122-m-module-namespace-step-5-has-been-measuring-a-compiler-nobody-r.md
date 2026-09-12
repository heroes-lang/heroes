- [ ] **M-module-namespace step 5** | **`heroes mutate` has been measuring a compiler nobody runs, since M-generics-library.** The CLI attaches the library (`input::read`); `mutate::fate` built a bare `Source::new`. Making them the same pipeline moved the score from 97%/81% to **96%/79%**. Task: say which resolution path differs when the library is present — the hint is that `xs.map(f)` reaches `Ref::Top` in one and `Ref::Builtin` in the other — and then say why the LOWER number is the one to publish

    **Where to look:** archive/bootstrap-rs/heroes/src/mutate/mod.rs · archive/bootstrap-rs/heroes-cli/src/input.rs
    **Why it matters:** the control arm and the real arm have to be the same compiler, or the metric measures the harness
