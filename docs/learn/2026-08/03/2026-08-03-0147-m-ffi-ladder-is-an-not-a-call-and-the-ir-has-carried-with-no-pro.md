- [ ] **M-ffi-ladder** | `s.cstr()` is an `Op::Cast`, not a call, and the IR has carried `CastKind::StrToCstr` with **no producer** since M-ir-lowering. Say why the conversion is a cast rather than a built-in call — the answer is one line of `ir/inst.rs`'s own table — and then say what makes the borrow free, and what makes it good only for the duration of the call

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/calls.rs · emit/inst.rs · design.md §4.20
    **Why it matters:** the highest-return decision in the string design, collected three milestones later
