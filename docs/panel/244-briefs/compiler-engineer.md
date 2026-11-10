# Panel 244, compiler-engineer

Read `00-shared.md` first. Your worktree `.claude/worktrees/244-compiler-engineer`. Run each rule
the compiler holds of a package, with a program and a catalogue in your scratch folder, and quote
what each prints: `internal/` from a program and from a package, in the program's own tree; a
placed tree with one byte changed, at `build`, `check` and `run`; `heroes fetch` and `heroes add`
in their three forms (index over `file://`, git over a `file://` repository, `--path`); `use <core>`
alone; `use <core>/nothere`. Say, for each, whether the specification's present text lets a
reader predict it, and whether the message alone carries the rule. Name any rule the compiler holds
that `00-shared.md` does not list.

## After the critic's first pass

Run the critic's shapes (`p4` to `p21`) yourself and add: `internal` as an element (`lib/internal.hero`,
a root `internal/`), a `path` entry with and without a version, a package's own catalogue holding a
`path`, two entries naming one package, R3's two aliases for two packages (unrun by the critic).

## Q-stub, added by the author (`00-shared.md` § A question added by the author)

Price the verb: where it lives in `selfhost/cli/` (the table, its operand, its flags), what it
writes for a program and for a package, built as a prototype in your worktree and run (`heroes
<verb> program <name>` and `package <name>`, then `heroes build` / `heroes test` on what it wrote,
and the package reached from a program by `heroes add --path`), what it refuses (an existing
folder, a name no `use` writes, a name that is a keyword), the lines it adds, and what moves with
the count of verbs (`table.hero:263`, the site's `claims.ts` verb count, the CLI goldens).
