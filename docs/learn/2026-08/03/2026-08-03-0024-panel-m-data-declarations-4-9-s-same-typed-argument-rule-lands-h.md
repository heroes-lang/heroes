- [ ] **panel? M-data-declarations** | §4.9's same-typed-argument rule lands hardest on FFI, where C's numeric APIs cluster same-typed parameters: `pow(base:, exponent:)` and `hypotenuse(a:, b:)` now need labels at every call site. Visible in `examples/gallery/08-ffi.hero`. Does the rule cross the `extern` boundary, or does a bound C signature get an exemption?

    **Where to look:** examples/gallery/08-ffi.hero, design.md §4.9, §4.19
    **Why it matters:** the rule is one of the thesis's headline mechanisms and this is the first place its price is concrete
