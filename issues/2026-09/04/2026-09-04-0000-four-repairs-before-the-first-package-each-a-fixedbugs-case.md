- [ ] **M-core-packages** step 0 | four repairs before the first package, each a `fixedbugs` case, the macro-only probe among them | `selfhost/ir/place_store.hero:100` · `selfhost/cli/pointee.hero` · `selfhost/emit/extern_probe.hero:164-165` · `docs/panel/092`, `103`

    **Merged 2026-09-10** from two items, by author instruction, and the second
    named the first as its own witness: *"the macro-only item panel 092 filed above
    gains its witness"*. Same milestone, same step, same sitting, same file — four
    repairs and four `fixedbugs` cases, in one item. **Both bodies are kept whole
    below.**

    **Re-verified 2026-09-10: all four STILL OPEN, and no case exists for any of
    them.** Of **116** `fixedbugs-*` files under `tests/golden/`, none matches
    push, pointee, `htons` or `setsockopt`, and `grep -rln "htons\|htonl\|isascii"
    tests/ examples/` is empty. The `place_store` pointer is exact —
    `selfhost/ir/place_store.hero:100` still chooses `.push_owned` for a bound
    value — and `selfhost/cli/pointee.hero` (310 lines) still has no `const void`
    arm. The macro-only defect is stated in the emitter's own comment at
    `selfhost/emit/extern_probe.hero:164-165`: *"`(htonl)(a0)` is `use of
    undeclared identifier`, and so is `(&htonl)`"* — the item's `:153` lands inside
    that same block. **Two numbers were deliberately not re-run**: the 14.78 s
    against 0.00 s pair is a build timing (CL-025), and *"44 distinct C functions
    are probed by the corpus"* needs a build to reproduce. The *"none is
    macro-only"* half re-measures cheaply and still holds.

    **The first item, as it stood.** a macro-only C function has no golden case, and the parenthesised probe breaks it

    **Origin:** panel 092, the compiler-engineer's own condition 3. Its home
    since 2026-09-04: this item said *the next milestone that touches the FFI
    probe or the golden corpus* and named none, and step 0's repair (c) is
    already this item's own witness — `htons` on Darwin is defined only as a
    macro and the parenthesised probe reports it as *declares no `htons`*, which
    is false.

    `(htonl)(a0)` is `use of undeclared identifier 'htonl'` — and so is
    `(&htonl)`, so the alternative spelling does not save it. §4.19 promises
    *"Macros and `inline` functions are reachable"*, and the repair that closed
    the `_FORTIFY_SOURCE` hole narrows that promise for a name the header
    defines **only** as a macro. **Measured: 44 distinct C functions are probed
    by the corpus and none is macro-only**, so nothing fires today — which is
    exactly why it needs a case rather than a fix. The case is one `extern`
    group over a macro-only name (`htonl`, `isascii`, `major`/`minor` from
    `sys/types.h`), and the decision it forces is whether the emitter falls back
    to the unparenthesised form when the parenthesised one fails to compile —
    which cannot be asked of clang in one pass.

    **Where to look also:** `docs/panel/092` § Where the seats disagreed.
    **Why it matters:** a promise in the document with no case in the harness is
    a promise nobody will notice breaking.

    **The second item, as it stood.** three repairs before the first package, each a `fixedbugs` case

    **Origin:** found by the session's probes, 2026-09-03, on the Mac and in the
    Linux image.

    **(a)** `xs @ xs.push(c.to_u8().must())` copies the whole array on every
    push: 100,000 pushes in **14.78 s** against **0.00 s** when the value is
    bound first (`v = c.to_u8().must()` then `xs @ xs.push(v)`), `heroes
    build`'s default optimisation, this Mac; `selfhost/ir/place_store.hero:100`
    chooses `.push_owned` for a bound value and not for an inline `.must()`, so
    the runtime's copying `push` runs. Every byte-oriented package writes
    exactly this shape. **(b)** `@value: i32` against a `const void *` pointee
    (`setsockopt`) is `internal error: checking what the extern out-parameters
    point at failed` at exit 2 on both platforms — panel 103's pointee assertion
    writes `_Static_assert(sizeof(const void) == …)`, a legal author declaration
    blamed on the compiler; `SO_REUSEADDR` is what a restarted server needs.
    **(c)** `htons` on Darwin is defined only as a macro (`sys/_endian.h`, the
    prototype under `#if defined(lint)`), and the parenthesised probe's failure
    is reported as `ffi_unknown_name`, *"declares no `htons`"*, which is false —
    the macro-only item panel 092 filed above gains its witness; glibc declares
    the function beside the macro, so the same `.hero` runs on Linux and prints
    36895.

    **Where to look also:** `selfhost/emit/extern_probe.hero:153`.
    **Why it matters:** a package built on a quadratic `push` and an
    out-parameter the compiler cannot describe fails on its first real input.
