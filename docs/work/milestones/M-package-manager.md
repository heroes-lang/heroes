# M-package-manager — packages, and what stands in for a standard library


**Scheduled, no warrant.** Not a decision to take later: design.md:772 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is
**M-separate-compilation**, not M-module-namespace: without separate compilation,
installing a package means recompiling the world on every build. **Since
2026-08-25 there is a second one in front of it** — M-package-layout, which rules
on how a fetched package's modules are named and reached; `heroes add` cannot
place files it has no spelling for. **And since 2026-09-03 the packages themselves
stand in front of it**: M-core-packages, whose step 13 — `heroes fetch`
under the root, the root-level driver and the `heroes check` item panel 091 filed
here — moved into this milestone, because one distributes what exists. **One
question its sitting owes, found the same night**: pinning what `fetch` brings —
Go's `go.sum`, Cargo's lock file — is a per-project file, and CLAUDE.md §10 admits
no fourth input class; the sitting says how a fetched package is fixed to a
version without one, or names which of panel 056's three return conditions it
meets.

**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it. **What this milestone does not deliver is the packages**:
those are M-core-packages, the row before it since 2026-09-03 — and the sentence just above,
*verified by clang, or by whoever wrote it*, is the first question that
milestone's opening sitting has to answer for a package written in Heroes alone.

**Panel 056's return conditions live here, as that sitting ruled** (`docs/panel/056`
§ Resolution, deliverable D, ratified 2026-08-15 — and written into this entry only
on 2026-09-03, by a reasoning session that read the panel forward and found the
deliverable undischarged). A per-project file is refused (CLAUDE.md §10: there is
no fourth input class), and **three conditions, all met, return the question to a
panel**: *one key, not two* — a `prefix <dir>` from which `-I` and `-L` are both
derived, so the skew is unrepresentable; *no string in the file may also appear in
a `.hero` file*, so deleting the file never changes which symbols are linked, only
whether the build succeeds; and *one named binding that `package "<name>"` and one
command line cannot build* — a set that was empty on 2026-08-15. Meeting fewer
does not.

*******************************************************************************
**OPEN: 2**

- [ ] **M-package-manager** | a bindings module is invisible to `heroes check` | `selfhost/cli/check.hero` · `docs/panel/091` · `docs/panel/082` R3

    **Origin:** panel 091, found by the ffi-pragmatist unasked. Load-bearing
    from M-separate-compilation onward.

    `heroes check badbind.hero` on a module that is nothing but an `extern`
    group is **exit 0 with zero output** — `check` never runs clang, so nothing
    verifies the group — while `heroes build` on a caller that reaches one of
    its two declarations reports `error[ffi_return_type]` **on the bindings
    module's own line**. This is the same shape as panel 082 R3's *check accepts
    ⇒ build succeeds* gap, one department over, and it becomes load-bearing
    exactly here: separate compilation is what makes a pure bindings module a
    normal thing to write, and M-package-manager is what makes it a thing you
    **distribute**. Whoever fixes it should read 082 R3 first — that item
    refused running `mono` inside `check` on a measured cost, and running clang
    inside `check` is the same trade at a larger price.

    **Why it matters:** a check that accepts everything is not a check, and a
    distributable binding is the one artifact whose whole value is that somebody
    verified it.

    **Re-verified 2026-09-10: STILL OPEN, and the mechanism is unchanged.**
    `selfhost/cli/check.hero` reaches no clang and no probe — the only `probe` in it
    is an unrelated local counter at `:234-241` — while clang is reached on the build
    path alone, `selfhost/cli/produce.hero:57` and `:295`. No fixture exists: the
    `badbind` name appears in `docs/panel/091`'s brief and in this list, nowhere
    else.

- [ ] **M-package-manager** | nothing in the tree compiles or links what `--emit-c` wrote, so a defect in the artifact is invisible to every suite | `tests/harness/suite_emission.hero` · `docs/panel/157-the-artifact-of-a-build-that-never-happened.md` R4

    **Origin:** panel 157 R4, adopted 2026-09-16 and deliberately not built —
    defect 048's repair closed the two holes that sitting measured and left the
    instrument that would have FOUND them.

    **What is missing, stated as the assertion rather than as a gap**: the same
    program built both ways must give the same exit code and the same stdout,
    and it must **LINK** and not merely compile. Each half of that sentence was
    paid for. Compiling the artifact catches 20 of the 25 `fixedbugs/` cases the
    build refuses; it catches **none** of the five that were silent, because
    clang accepted their C — that is what the exit-code comparison is for. And
    linking is what `ffi-missing-link` needs: a library nobody named is a linker
    failure and no check that stops earlier can see it, which is why that one
    case still emits at 0 after defect 048's repair.

    **Why this milestone.** `emission` compares BYTES, and a byte comparison
    cannot see that the bytes do not compile: of 240 blessed emissions measured
    2026-09-16, **28** failed `-fsyntax-only` and **3** failed with *must use
    'struct' tag* while green in the suite. Defect 048 was already committed and
    blessed. The cost of the missing instrument is a whole class of defect that
    ships, and it becomes load-bearing exactly here, where a binding is
    something you **distribute** rather than something you build once.

    **The cost, priced at the sitting**: +20 to +30 lines in
    `tests/harness/suite_emission.hero`. What was NOT priced is the wall clock —
    building 240 programs twice, with a link each, is a different order from
    reading a blessed file, and whoever builds it should measure that first and
    say whether the leg belongs in `emission` or behind the push.

    **Why it matters:** the suite that exists to compare emissions cannot tell
    a good emission from one that will not build, and it said so for a year in
    its own comment without anybody hearing it.

*******************************************************************************
