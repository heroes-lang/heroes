# Panel 238, the historian's brief

Read `00-shared.md` and `proposal.md` beside this file first. You are advisory, with no veto. Every
precedent you cite you read on the web while writing, with its URL and the day read, marked
`verified`; a claim you could not read is marked `unverified` and nothing rests on it. Your worktree
is `.claude/worktrees/238-historian/`, where you write your report; you need no compiler.

Your report is the record the sitting's `## Prior art` is written from
(`.claude/rules/records.md` § And the record says whose idea it was), so **each line says only what
the cited page itself states**, in the page's own words where they decide it, and nothing of what
Heroes did about it. A comparison the page does not make, a mechanism it does not describe, or a
date it does not give is cut.

## What the sitting needs from you

1. **Grouping and sorting imports in a canonical formatter** (question A). For each, what the tool
   does by default, read in its documentation or source: whether it sorts, whether it groups, by what
   key (byte order, case-folded, natural), whether a blank line separates groups, what it does with a
   comment above or beside an import, with imports separated by blank lines (each run sorted alone,
   or merged), and with an import that is not at the top of the file.
   - Go: `gofmt` (does it sort within a block of consecutive imports, does it group?) and
     `goimports` (the standard library first, then the rest, with a blank line); `go/ast.SortImports`
     if its documentation says what a comment does.
   - Rust: `rustfmt`'s `reorder_imports` (a stable default?) and `group_imports`
     (`StdExternalCrate`, its stability), and `imports_granularity` if relevant.
   - Python: PEP 8's three import groups and the blank line between them; isort's sections and
     `force_sort_within_sections`; Ruff's `I` rules if they say something isort does not; Black's
     position (it does not sort imports, if its documentation says so).
   - Dart: the `directives_ordering` lint (`dart:` then `package:` then relative, each sorted).
   - Swift (`swift-format`'s `OrderedImports`), Zig (`zig fmt`), Deno (`deno fmt`), Elm
     (`elm-format`), Haskell (`stylish-haskell` or `ormolu`), and any other formatter you find that
     takes a position, for or against sorting.
   - **Any formatter that refuses to reorder imports, and why**: the Heroes formatter refused until
     now (`00-shared.md` F2: *reordering is where a formatter starts deleting the author's
     remarks*). A documented case of a sorting formatter moving or losing a comment, or changing a
     program's meaning by reordering (an import with a side effect, an init order), is the
     strongest objection the sitting could record.
2. **The key** (question A): where a tool documents why it sorts by bytes, case-insensitively, or by
   path segments, and what it does with uppercase letters and digits.
3. **A repository's own static index or vendored registry folder** (question D): where other
   ecosystems keep a local index or a set of packages a repository serves to itself, and the folder
   name each uses: Go's `vendor/` and a `GOPROXY=file://` directory (the Go Modules Reference says a
   `file://` URL can be a module proxy), Cargo's `cargo vendor` and its `directory` or
   `local-registry` source replacement, pip's `--find-links` and PEP 503's simple index served from a
   folder, npm's `file:` dependencies and workspaces' `packages/` convention, Deno's `vendor/`,
   Bazel's `third_party/`. Say what each folder is called and whether the name is a convention or a
   rule a tool enforces.
4. **What would make `packages/` the wrong name later**: an ecosystem where a root `packages/`
   folder means something else by convention (a monorepo's workspace members, for example), read on
   the page that says so.

Your report: `docs/panel/238-reports/historian.md` in your worktree, written as you go, in the
structure of `.claude/agents/historian.md` (verdict, precedents, argument, condition, search), and
ending as `00-shared.md` § Rules says; your verdict is advisory. Under its own heading, **Candidate
lines**: the lines you propose for the decision issue's `## Prior art`, each in one of the two forms
of `.claude/rules/records.md` (*Taken from* or *Reached first by*), each with its URL and a quotation
of the page.
