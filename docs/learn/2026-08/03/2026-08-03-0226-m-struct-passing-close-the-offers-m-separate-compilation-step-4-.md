- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 4 (the exact conversion that still loses) | `pow(x: f32, y: f64)` printed a fifth-digit-wrong answer at exit 0, yet the conversion f32→double at the call is EXACT — not one bit changes. Where was the precision lost, and why does this diagnostic's sentence ("the precision the header expects is lost before the call") differ from the integer one ("C would convert the value in silence")?

    **Where to look:** tests/golden/fixedbugs/ffi-float-parameters.hero · selfhost/emit/ffi_narrowed.hero::PROMOTED
    **Why it matters:** the defect is upstream of the conversion clang sees, which is why -Wconversion — the flag that watches value-changing conversions — is measured blind to it
