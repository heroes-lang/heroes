- [ ] **M-struct-passing close — the offers** | M-struct-passing, mutation drill | **Three catch-all arms broke on one day.** Given `emit/convert.rs` before the fix — an arm for `Ty::Float(FloatKind::F64)` and a `_ => return` — predict what `to_i64` on an `f32` produces, and at what exit code, *before* reading the answer. Then say which of the project's flags could have caught it and why none did

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/convert.rs · docs/records/journal/018 § What broke and why
    **Why it matters:** the answer turns on panel 021's zero-initialiser being a defence, not an oversight
