# Panel 228, the historian's brief

Read `00-shared.md` in this folder first. You build nothing and read no worktree; you search the web,
and every precedent you give is **verified**, its source named, read and dated, or marked
`unverified` in those words.

**The question**: how other tools hand a *typed hole* (a place the program leaves unfinished on
purpose, where the compiler says what type belongs and what is in scope) to a machine, and how they
version the machine-readable answer of a checker when it gains a member. Precedents worth looking at,
none of them a premise: GHC's typed holes (`_` and `_name`, "valid hole fits") and its JSON
diagnostics (`-fdiagnostics-as-json`, the release that added it and whether a hole is a diagnostic
there and with which severity), `-fdefer-typed-holes`; Idris 2's IDE protocol and its
`:metavariables`; Agda's interaction goals and `--interaction-json`; Lean 4's `sorry` and `?_` and
how its language server shows a goal; whether rustc's `--error-format=json` and cargo's message format
add fields without a version, and whether `cargo metadata` carries a version number (panel 193's
historian named `cargo metadata`); the Language Server Protocol's `PublishDiagnostics` against a
separate request for goals; SARIF's versioning. Any precedent nobody listed is welcome.

**What you are asked**, for each route of the shared brief (A, A', B, C, D, E): the precedent it would
follow and the one it would break; whether *a hole is not a diagnostic* has a precedent that kept it
out of the diagnostics list and what that tool's consumers do; and whether a precedent caps the
suggestions it hands a machine, and how.

**Output** as `.claude/agents/historian.md` asks. **Write
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-reports/historian.md`**
as you go. No credential, no paid run.
