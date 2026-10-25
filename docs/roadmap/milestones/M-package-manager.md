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

**A comparison read on 2026-10-25, and one answer for panel 056's open
question**, how a fetched package is fixed to a version without a per-project
file. Goose (github.com/aardappel/goose, [goose/docs/design/packages.md](https://github.com/aardappel/goose/blob/master/docs/design/packages.md) § 1 and
§ 4.3) pins it in the root file's own import line, `import x from
"owner/repo@v1";`, a git source at a tag or a commit, and the same line in any
other file is a *requirement* the root must satisfy, refused with the line to
add: "No lockfile, no version ranges, no registry", one version of a package
per program. A `use` here already resolves from the compiled file's directory
(spec § 1), so the shape fits this language's rule that the root decides;
whether the source belongs in the `use` line is a question of surface, so this
row's sitting's. Beside it, Goose keeps one cache for the machine, where this
compiler keeps `build/` per project.
