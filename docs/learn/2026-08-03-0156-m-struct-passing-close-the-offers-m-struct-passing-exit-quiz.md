- [ ] **M-struct-passing close — the offers** | M-struct-passing, exit quiz | Four questions with numbers as answers: how many rustc errors did `Ty::F64` → `Ty::Float(FloatKind)` produce, and what does that number *mean*; why does the field probe take the field's **address**; why does `HERO_RET_F32` accept one C type where `HERO_RET_F64` accepts three; and which of `{int32_t; float}` / `{float; float}` against `{int32_t; int32_t}` actually breaks on arm64

    **Where to look:** docs/panel/060 · docs/records/journal/018
    **Why it matters:** the last one is the question the panel's own brief got wrong
