# Panel 235: the completeness critic on the historian's survey brief

Read 2026-11-07 in lane `pm-opening`, HEAD `b32ac8632`, the brief untracked.
No verdict on the design. Each *(X: 0)* below is `grep -c X` on the brief.

## Framing facts, checked

1. **L15-16** *"the paragraph opening Opened 2026-11-07"*: `awk 'NR>=59&&NR<=64'`
   shows that paragraph is lines 59-63 and holds none of the position; the
   position, criteria and questions are lines 65-165. `git show
   main:docs/roadmap/milestones/M-package-manager.md | grep -c "Opened
   2026-11-07"` reads 0, on `pm-opening` 1. The brief's paths are relative and
   the seat's Read and Write take absolute ones: rooted in the main checkout,
   on `main`, it finds no such text.
2. **L34-36**, the spec: the bullet is lines 27-30, its last sentence wrapped
   over 29-30; *one file is one module* is line 22. Holds. The same bullet's
   `use syntax/decl as sd`, an in-file rename the language has, is unmentioned
   (`as`: 0).
3. **L18-32** against page lines 65-113: holds as paraphrase. Dropped: *a bare
   name against the index* (95), *a local folder while a package is developed*
   (157), and the measured criteria (107-113), among them *zero if it depends on
   nothing* and *mistakes from Go's, Cargo's, npm's and Python's habits*.
4. **L41**: `docs/panel/235-reports/` exists (`ls -la`). Holds.

## World facts stated as true; make them questions

5. L27 *"as `pip install` reads PyPI"*.
6. L49-50, C's include forms *"as the precedent for angle brackets"*: what the
   C standard says each form searches, and whether that is a mine-versus-fetched
   precedent, is a finding, not a premise.
7. L53-54, the rename mechanisms, Go's `replace` above all: a project-local name
   in source, or a redirect behind an unchanged import path?
8. L72-75: crates.io's index history, PEP 503 and 691, Go's proxy and
   checksum database.
9. L83-84 puts ZON and `Package.swift` in one kind; data or a program the tool
   runs is to be established.
10. L85-86 pairs `1.10` and `no` as if YAML 1.1 and 1.2 differ on both: ask each
    value under each version's schemas apart, and which version their parsers
    implement.

## Leading

11. L67 *"For Go in particular"*: Go's history in depth, Cargo's not, while the
    sitting chooses between them.
12. L49-50: C is the bracket's only named precedent, while
    `docs/panel/234-reports/historian.md` (read 2026-11-05) holds verified rows
    on Nim's `pkg/` (230-245), Node's `node:` (310-318) and Go's dot in the
    first path element (88-91) (`234`, `Nim`, `pkg/`, `node:`: 0).
13. L85-87: only YAML's pitfalls are seeded.

## Missing

14. **The inventory a whole copy is ruled on**: every directive and key of
    `go.mod`, `go.sum`, `Cargo.toml`, `Cargo.lock` in the newest release, which
    release and its date (`release`: 0), with any line declaring a language
    version or edition (`edition`, `toolchain`: 0), since CLAUDE.md § 2 (line
    137) refuses *"a declared version"* before 1.0.0.
15. How an import line shows own versus fetched, and what happens when a
    dependency and a local module, or two dependencies, offer one name (page
    139-140, panel 101's R3).
16. Whether reading the manifest or building a dependency runs its code
    (`execut`: 0).
17. Git sources: references accepted, how an untagged commit gets a version,
    whether the lock holds a content hash or only a commit id, what a
    fingerprint does with line endings, file modes, symlinks and case, and
    incidents where one broke with the source unchanged (`incident`: 0).
18. The exact error when manifest and lock disagree or a hash mismatches, and
    whether it names its fix (`error`: 0); the page's mistakes list needs it.
19. Whether a project with no dependencies needs the file; whether the manager
    shipped with the language's 1.0.
20. The verbs (add, fetch, update, verify) and whether they rewrite the file
    keeping comments and order; `internal/`-style privacy; git as a program or
    a library (page 162-163).
21. Rows: Goose (page 46-57, read 2026-10-25; panel 234's historian found
    another Goose quotation of that day no longer matching its file, line 258),
    Nimble, npm itself (`package-lock`: 0), pip's `requirements.txt`.
    Candidates, unverified here: Gradle's version catalogs (an alias file of
    that very name), Bundler, Maven Central, indexes kept as git repositories
    (opam, Julia's General, Bazel's registry).
22. Table 2: incidents that changed each index's rules; systems that began
    git-only and added an index, and what changed in their files.
23. The rulings the page reopens, `docs/panel/056-what-a-project-file-would-carry.md`
    and `099-where-a-use-path-starts.md`, so both sides' precedents are
    gathered (`056`, `099`: 0).

## Misreadings

24. L44 calls the systems *columns*, L91 says *"Every row is `verified`, with
    the URL"*: one URL for seven claims invites the excess records.md measured
    (33 of 90 lines, line 129). Ask a URL and day per cell (`cell`: 0).
25. `.claude/agents/historian.md` demands `verdict: approve | object` (41) and
    lets a claim stand *unverified* (18); the brief says no verdict, and cut.
    Say which wins.
26. No order of work for a run that stops: Go and Cargo first.
27. A cell may become a `## Prior art` line, which says nothing of Heroes: no
    cell relates a finding to the position, and each names the tool's release.

## The three repairs

1. **Point at the text** (1, 12): absolute lane paths, commit `b32ac8632`,
   page lines 59-165, panel 234's rows re-read, never carried.
2. **Ask for the inventory** (14), dated, version-declaring lines included.
3. **Neutralise and atomise** (5-13, 24-25): examples become questions, every
   probe is asked of every row, one URL per cell, and the agent definition's
   verdict and *unverified* are overridden in words.
