# Panel 240: shared brief

**Sitting** 240, `retro-record`, a reduced sitting by the author's instruction of
2026-11-09 (meant as: *fix it, without a sitting or with a reduced one, before
the tag*). Seats: compiler-engineer, spec-warden, historian; the completeness
critic reads these briefs before any seat starts and the reports after. No
llm-ergonomist and **no paid run of any kind**: no `claude -p`, no API call, no
`heroes measure --refresh` (the `real` row is not refreshed; the vendored count
is the instrument and every verdict that rests on it says so).

**The base**: commit `429464c94012e0691dfa091e0b38070c5c9550f0` of the branch
`b27-round`, tree `9f5ae3df8732db56336f726265a199019e4ad8cd`
(`git rev-parse '429464c94^{tree}'`, run 2026-11-09 21:35). Each building seat
has its own detached worktree at that commit:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-<seat>/`. Build your
compiler inside it from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, a few seconds; stage 2 by `./heroes build
selfhost/main.hero -o heroes-next`, about a minute). Never use another tree's
compiler, never read or build in another seat's worktree, never commit, never
push. Write your report as you go to
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/reports/<seat>.md` (the coordinator copies every report into `docs/panel/240-reports/` at the synthesis, since the round's tree is read by its gate until then).
No seat prints a credential (`gh auth status` if you must know, never
`gh auth token`).

## The decision taken (the author's, 2026-11-09, about 21:30)

Asked whether the root folder `packages/` (panel 238's R10, the repository's
static index) was wrongly named because "in Heroes it is called a module", the
author was shown the spec's words and answered: *fix it, one name for one
thing*. The coordinator's reading, which the author accepted as the
recommendation:

- **module** = one `.hero` file (spec § 1, line 22: "One file is one module");
- **package** = the code others publish, which the program's catalogue
  `heroes.toml` fetches (spec § 1, lines 31 to 37: `use <json>/parse` reaches
  "the module `parse` of the package `json`"); so `packages/` (the index) and
  `heroes.packages/` (where `heroes fetch` places trees) keep their names;
- **library** = C code a build links (`link "m"`, spec § 13).

**The collision**: spec § 13, lines 479 to 482, uses *package* for something
else, a C library found through `pkg-config`:

    A group may name a **package** instead of a library: `extern "raylib.h" package "raylib"`
    asks the system where its headers and libraries are and what else it needs.

        Extern = "extern" string [ ( "link" | "package" ) string ] NEWLINE

That mark came from panel 050 (`docs/panel/050-the-package-clause.md`,
retro-record, 2026-08-14). Panel 235 named the collision and did not rule it
(`docs/panel/235-dependencies-named-by-an-alias-in-angle-brackets-that-the-programs-catalogue-resolves-gos-modules-copied-whole-a-toml-catalogue-listing-every-package-and-no-fetched-code-run-at-build.md`,
§ The word *package*, line 509: "the collision with § 13 is not ruled"), and
journal 075 carries it forward.

## The proposal (what the sitting rules)

Rename § 13's mark so that *package* means one thing. The coordinator's
candidate is `pkgconfig`:

    A group may name what `pkg-config` knows a library by: `extern "raylib.h" pkgconfig "raylib"`
    asks the system where its headers and libraries are and what else it needs.

        Extern = "extern" string [ ( "link" | "pkgconfig" ) string ] NEWLINE

with the diagnostic codes of that sense renamed to match, no alias and no old
spelling kept (CLAUDE.md § 2: before `1.0.0` no form enters to keep an older
program compiling). **The name is the open question**: `pkgconfig`, `system`,
`library`, or one nobody listed. Each seat says which, and why, from its input.

## Measured for this brief (2026-11-09, at the base)

- `git grep -lE '^\s*extern "[^"]*" package "' -- '*.hero' | wc -l` = **70**
  files; the same pattern counting lines (`-h`, `wc -l`) = **87**.
- `git grep -hoE '"(ffi_package|package_[a-z_]+|[a-z_]+_package[a-z_]*)"' --
  'selfhost/*.hero' | sort | uniq -c`: `package_unmarked` 7, `package_reads_outside`
  6, `package_comparison` 4, `ffi_package` 4, `package_wants_a_slash` 2,
  `package_wants_a_name` 2, `package_wants_a_module` 2, `package_name_quoted` 2,
  `package_mark_unclosed` 2, `package_mark_holds_a_path` 2, `package_mark_apart` 2,
  `unknown_package_module` 1, `package_unread` 1. **Which of these are of the C
  sense and which of the fetched sense is a question, not a premise**:
  `selfhost/head_names.hero:426` shows `package_comparison` said of `"zlib=1"`
  under `what: "package"`, so that one is of the C sense; the rest were not read.
- `git grep -lE 'ffi_package|package_comparison' -- tests/ | wc -l` = **26** files.
- `selfhost/` files naming `"package"`, `.package` or `package:`
  (`git grep -lE '"package"|\.package\b|package:' -- 'selfhost/*.hero'`): 20, of both
  senses, among them five whose own names say *package* in the C sense
  (`cli/package_answer.hero`, `cli/package_macros.hero`, `cli/package_missing.hero`,
  `cli/package_stand_in.hero`, `emit/header_packages.hero`), a question for the
  compiler-engineer.
- Files outside `selfhost/` that re-print or colour a program and hold the word
  (`git grep -lnw package -- editors/ site/src/lib/ selfhost/print/ selfhost/probe/
  selfhost/mutate*`): `editors/vscode/syntaxes/heroes.tmLanguage.json`,
  `editors/vscode/extension.js`, `site/src/lib/highlight.ts`,
  `site/src/lib/examples-model.ts`, `site/src/lib/examples-render.ts`,
  `site/src/lib/i18n.ts`, `selfhost/print/fmt.hero`, `selfhost/print/dump.hero`,
  `selfhost/probe/judge.hero`, `selfhost/mutate/op_decls.hero`, and the README
  and manifests of `editors/`.
- Pages with `package "x"` (`git grep -lE 'package "[a-z]' -- site/src docs/design.md
  .claude/rules`): `site/src/html/docs/the-c-boundary.html` and its Italian twin.
- The spec today (`./heroes measure spec/heroes-spec.md` at the round, 2026-11-09 21:03):
  **8325** on the vendored maximum, **10,900** real (claude-opus-5, 2026-11-08,
  not refreshed tonight).

## What every seat answers

1. The name, from your input, with the reason; `pkgconfig`, `system`, `library`,
   or another.
2. Your verdict on the rename as a whole: adopt, adopt with a condition, or a
   veto with the soundness reason a veto needs.
3. One falsifiable prediction, with the milestone at which it becomes checkable.
4. What you could not measure, said as a question.
