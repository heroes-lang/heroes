# Panel 240: compiler-engineer

Read `00-shared.md` beside this file first. Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-compiler-engineer/`,
detached at the base. Build there from the seed; nowhere else.

**Your question is the cost, measured by building it.** Implement the rename in
your worktree, far enough that the number is real, with the coordinator's
candidate `pkgconfig` unless your reading finds a better name, and report:

1. **Which of the thirteen codes listed in `00-shared.md` are of the C sense**
   (a `pkg-config` lookup) and which of the fetched sense (a Heroes package),
   each read at its definition with the file and line. Propose the new name of
   each C-sense code.
2. **Every place the mark is read, printed or coloured**: the lexer's contextual
   marks (`selfhost/keywords.hero`), the parser, the AST's field, the formatter
   (`selfhost/print/fmt.hero`), the dumps, `heroes mutate`, the probe's reader,
   the `Fix` replacements, the TextMate grammar and `site/src/lib/highlight.ts`
   and `lexed.ts` (`.claude/rules/diagnostics-and-goldens.md` § A new surface
   form lands in every tool that reads the language, the walk this rename owes).
   Count the files and lines your prototype changed (`git diff --stat`).
3. **Whether internal names should follow**: the five `selfhost/` files whose
   names say *package* in the C sense, and the identifiers inside them. Measure
   what renaming them costs and say whether a reader of `selfhost/` would meet
   *package* in two senses if they stay.
4. **The goldens**: the 26 files under `tests/` holding `ffi_package` or
   `package_comparison`. `tests/golden/` is append-only by
   `.claude/rules/records.md` § A record is never rewritten, and
   `UPDATE_GOLDEN=1` does not exist. Find how earlier code renames were landed
   (`git log -S` on a renamed code is one way) and say which route this one
   takes, with the precedent's commit.
5. **Run it**: your prototype's stage 2 against the base's on
   `./heroes run tests/harness/main.hero -- ./heroes-next check full run unsupported emission`
   or whichever forms hold the cases you touched, and the compiler's own tests;
   report the counts. A route that does not build is not adopted.
6. A program written with the old spelling, `extern "raylib.h" package "raylib"`,
   after the rename: what does it get? The answer the sitting wants is an error
   that names the new spelling with a `certain` fix (design.md §4.17); say
   whether that is a refusal the language owes (CLAUDE.md § 2 allows telling a
   `0.x` program, never keeping its spelling) and what it costs.

Cite files and line counts. Never use `crates/`. Do not commit, do not push.
