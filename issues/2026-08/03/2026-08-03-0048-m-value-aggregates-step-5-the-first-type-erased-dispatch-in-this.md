- [ ] **M-value-aggregates step 5** | The first **type-erased dispatch** in this project: the runtime works on an array through `HeroDesc`'s five function pointers, and the emitter's own calls stay typed. Count: for `[Point]` where `Point` holds a `str`, how many functions does one `hero_array_decref` reach, and which of them is the one the compiler generated versus the one it wrote by hand?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/perfn.rs (descriptors) · runtime/runtime.c (hero_array_decref) · tests/golden/emit/aggregates.expected
    **Why it matters:** it is the one place the "clang type-checks every call" property is deliberately given up, once per type
