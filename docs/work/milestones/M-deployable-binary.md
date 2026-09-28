# M-deployable-binary — what the machine that RUNS a program needs


**Scheduled 2026-09-10 by author decision**, from § What production-ready means
row 4, which had no owner at all: M-install-channels answers *how do I get the
compiler*, and nothing in this file answered *what does the machine running my
binary need*.

**What it delivers**, three measurements and one decision. `otool`/`ldd` on a
built binary on **each** platform, written into `docs/ref/environment/`. Whether a
binary built against the CI image's glibc runs on an older server, and what it
says when it does not. Whether a Linux binary can be **static**, which is the
difference between a `scratch` image and a distro image — `design.md:708-710`
already records that macOS has no static libc, so one flag cannot answer for
three platforms. And the decision: **`heroes build` defaults to `-O0` while
`run` defaults to `-O2`**, so the artifact somebody ships is the slow one unless
they know to ask. A changed default, a `--release` alias or one sentence, and
§10's stopping rule judges a flag here like any other — but not silence.

**What warrants it is one measurement and one silence.** On this Mac a Heroes
binary links `/usr/lib/libSystem.B.dylib` and nothing else, so it carries no
sidecar; the same question on Linux and on Windows is **unrun**.

**Why here.** Before M-install-channels: a channel that installs a compiler whose
output nobody knows how to ship is half a channel. A row of its own and not that
row's bullet, because they are two deliverables — the compiler, and the user's
program — which is §14's own test.

**What it does not deliver**: any outward act. **Soundness lane**, unless the
`-O` decision takes a flag.

*******************************************************************************
**OPEN: 2**

- [ ] **M-deployable-binary** | what a built program needs at run time on each platform, and which `-O` it ships with | `selfhost/cli/table.hero` · `selfhost/cli/verbs.hero:105` · `design.md:708-710` · `docs/ref/environment/`

    **Origin:** author decision 2026-09-10, § What production-ready means row 4,
    which had no owner at all.

    **One half is measured and it is good**: on this Mac, 2026-09-10, `otool -L`
    on the self-hosted compiler names `/usr/lib/libSystem.B.dylib` and nothing
    else, so a Heroes binary carries no sidecar. **The other half is unrun and is
    written as unrun**: the same question on Linux and on Windows.

    **Three measurements and one decision.** `otool`/`ldd` on a built binary on
    each platform, written into `docs/ref/environment/`. Whether a binary built against
    the CI image's glibc runs on an older server, and what it says when it does
    not. Whether a Linux binary can be static, which is the difference between a
    `scratch` image and a distro image — knowing `design.md:708-710` already
    records that macOS has no static libc, so one flag cannot answer for three
    platforms. And the decision: **`heroes build` defaults to `-O0` while `heroes
    run` defaults to `-O2`**, so the artifact somebody ships is the slow one unless
    they know to ask. A changed default, a `--release` alias, or one sentence — and
    §10's stopping rule judges a flag here like any other. What it may not be is
    silence.

- [ ] **M-deployable-binary** | panel 182's deferred routes, each back only on its own condition: the consuming store (b), the initialising stores (f), a value declared where it is defined (g), MemorySanitizer as a Linux instrument leg, and one slot for the synthetic slots of mutually exclusive arms | `docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md` · `docs/panel/182-reports/completeness-critic.md` · `selfhost/emit/`

    **Origin:** panel 182, 2026-09-28, deferred and not refused; ratified by
    the author the same evening, and scheduled here by the coordinator under
    CLAUDE.md § 3's delegated default, because this is the one scheduled
    milestone whose deliverable is what a built program costs when it runs,
    and that is what the routes buy. The author may move it.

    **The conditions, as the sitting wrote them.** (b) and (f) come back when
    the compiler itself crosses a leak gate (its runs report live blocks at
    exit: the critic's differential probe, or a runtime mode in which
    `hero_exit` checks) **and** (b)'s fault injection is shown to fire; their
    gain over the landed route is 6 to 9 points of user time, and their
    failure modes, a double release, a leak and for (f)'s consumed-store half
    a use after free, have no instrument today that sees them in the one
    program with 12,897 synthetic slots. (g), the critic's unlisted route,
    removes the value prologue outright and its gain is unrun. MemorySanitizer
    was measured working on the Linux arm64 image by the critic, and becomes a
    leg once the cache keys let it build its own objects, which defect 122's
    repair made them do (the C compiler and its flags in every key, closed
    2026-09-28). Coalescing rests on a premise nobody ran.

    **What opening this item owes first**: re-measure each gain on the
    compiler of that day, since 6 to 9 points is a measurement of
    2026-09-28's, and say which condition is met, in the item, before any
    route is built.

*******************************************************************************
