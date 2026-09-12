# M-arm-platform — the fourth real machine


**Scheduled 2026-09-10 by author decision**, from § What production-ready means
row 3.

**What it delivers.** arm64 Linux as a measured platform: the seed built from C
alone, the compiler's own tests and the harness's own passing there, and a fourth
CI matrix entry, so the leg is a judge and not a hunting instrument
(`.claude/rules/platforms.md`). The three legs today are Linux x86-64, Darwin
arm64 and Windows x86-64.

**What warrants it.** The architecture most production containers run on has
never compiled this compiler, and the prediction is registered here so it can be
scored: plain `char` is **unsigned** on the ARM ABI and **signed** on x86-64,
while `spec:228` declares a parameter and a field at *the header's own width and
sign*. So either the leg finds a divergence in the corpus's 20 `extern` programs
of 55, or the FFI's width rules are stronger than three legs could show.
Structure padding and `va_list` are the next two shapes.

**It is not the cross-compilation `DESIGN-LOG.md:539` refused.** That refusal's
own ground — *"the three platforms are measured on real machines by rule"* —
argues for a fourth real machine and against a `--target` flag. And it is cheap:
the existing image runs x86-64 **under Rosetta** on an arm64 Mac, so the new one
is the native instrument.

**Why here.** Before M-core-packages, which declares bindings against roughly
eighteen C headers: a leg arriving after them never saw them written. The cheaper
position was before M-install-channels, at the price of every binding owing a
re-measurement; the author took the robust one (CL-040).

**What it does not deliver**: a target flag, a prebuilt binary, or a fourth
platform in M-online-compiler's sense. § The names carries why the id is neither
`M-fourth-platform` nor `M-arm-linux`.

*******************************************************************************
**OPEN: 1**

- [ ] **M-arm-platform** | the fourth leg: an arm64 image, a matrix entry, and the `char` prediction scored | `docs/environment/linux/` · `.github/workflows/ci.yml` § matrix · `.claude/rules/platforms.md`

    **Origin:** author decision 2026-09-10, § What production-ready means row 3.

    **What it delivers**, in the order the platforms rule asks: an arm64 Linux
    image beside the x86-64 one under `docs/environment/linux/`, built from its own
    `Dockerfile`; the seed built from C alone there; the compiler's **618** tests
    and the harness's own **126** passing there; and a fourth CI matrix entry, so
    the leg is a judge and not a hunting instrument (`.claude/rules/platforms.md`
    § A platform fact is run on a platform).

    **The prediction is registered here so it can be scored.** Plain `char` is
    unsigned on the ARM ABI and signed on x86-64, and `spec:228` declares a
    parameter and a field at *the header's own width and sign*, so either the leg
    finds a divergence in the corpus's **20** `extern` programs of 55, or it finds
    none and the FFI's width rules are stronger than three legs could show.
    Structure padding and `va_list` are the next two shapes to attack, in that
    order.

    **Why it is cheap, measured before the row was written**: the existing image
    runs x86-64 **under Rosetta** on an arm64 Mac, so the new one is the native
    instrument and not the dearer one.

    **What it may not become**: a `--target` flag. `DESIGN-LOG.md:539` refused
    cross-compilation on 2026-09-03 and this row obeys that refusal rather than
    bending it — a real machine, measured, exactly as the rule asks.

*******************************************************************************
