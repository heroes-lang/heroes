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

**SCORED 2026-09-17, and the prediction is half right — the interesting half
being the false one.** The divergence is real and the leg found it in its first
hour: `char` is unsigned on Linux arm64 and signed on Linux x86-64, measured on
two containers one axis apart. **What is false is the stated cause.** *"Unsigned
on the ARM ABI"* is not what the world does: the generic AAPCS says unsigned and
Debian arm64 follows it, while **Apple's own arm64 ABI deviates and declares
`char` signed** — so this Mac, an arm64 machine, is a signed-`char` platform and
all three legs before today were signed. The divergence belongs to the
PLATFORM's ABI, not to the architecture, and a fourth leg that had been chosen
for its architecture alone could have been an arm64 Darwin and shown nothing.
The record is
`docs/records/log/2026-09-17-2317-the-fourth-leg-found-the-third-char.md`.

**What it cost is defects 058 and 059**, and the corpus was not where it showed.
The full net on the new machine reads **1825 passed, 3 failed**, with `corpus`
at **53 passed, 0 failed** — so **none of the 20 `extern` programs of 55 binds a
plain-`char` member**, and the row's own guess about where to look was wrong
alongside its guess about why. The three failures are one program in three
suites:
`tests/golden/run/ffi-a-char-array-member.hero`, a golden case written at
M-complete-structs whose own comment carries the premise *"`char` is signed
here"* — measured on one machine, true on three legs, false on the fourth.

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
**OPEN: 0**

*******************************************************************************
