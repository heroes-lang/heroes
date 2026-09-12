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
**OPEN: 1**

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

*******************************************************************************
