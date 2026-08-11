# M8a — Modules: the namespace

## Goal

The first entry in panel 030's revised chain, and the first of the closure
list's four remaining rows: **modules as a namespace, and only the namespace.**
`use geom` reads `geom.hero` beside the file that names it; every cross-module
name is written qualified; the whole program is still emitted as **one `.c`**,
because §1.0's list says *modules* and never says translation units, and one
`.c` per module with prototypes across them and a per-module cache is a build
architecture priced at 2–3× and deferred to M9.

The acceptance was fixed before the milestone opened: `examples/calculator.hero`
split across modules, `heroes test` still green, and no golden's emitted C
changing shape. All three hold — `examples/calculator/` is four files,
`main` → `eval` → `parse` → `lex`, seven tests from three of them, and the split
program's stdout is byte-identical to the single file's.

Six steps and two panels. `use` as a word (031 landed the spec at +71); `Source`
as N files behind a table; discovery and the module graph; qualified names;
the mangler taking each declaration's own module; the acceptance.

## What surprised

**The architecture built for one special case was the general case.** M5b
appended the library to the user's file and carried one boundary offset,
`library_at`. M8a needed N files, and the panel priced the two candidates by
counting rather than by preferring: **39 `Span` construction sites across 14
files and 271 functions taking `&Source`**. Concatenating N files behind a
per-file table changes *zero* of both; a file id inside `Span` changes all 39,
pushes `Span` past 8 bytes, and gives `Span::to` a same-file assertion with
nothing useful to do when it fails. The cheap change and the correct one were
the same change, and neither was anybody's taste.

**Four language rules arrived with no new specification, because of one verb.**
The panel wrote `use geom` *binds* `geom` — the spec's own word for `=` — and
two sentences already in the document then answered two questions nobody had to
write: "shadowing is a compile error" covers a local named after a module, "an
unused binding is a compile error" covers a `use` nothing reads. A module in
value position and a module named twice needed messages but no rules. Eleven of
panel 031's thirteen resolutions cost zero tokens, and the cheapest amendment in
this project's record turned out to be a verb.

**One predicate had been answering two questions since M5b.** `is_library` was
asked "may a diagnostic point here", "may a `#line` claim the author's file" and
"which module does the mangler use" — the same answer while there were two
files. With four it splits: the dumps and `fmt` want `is_root`, because a
`--dump-<stage>` answers *what does the compiler know about the file I named*;
emission and `heroes test` want `is_library`, because a module's `test` blocks
are its own and a run that took only the root's would drop the rest in silence.
The distinction was invisible for three milestones and is load-bearing in the
acceptance.

**A panel prediction landed on a line count.** The compiler-engineer wrote that
at M8a `resolve/exprs.rs` (279 lines then) and `resolve/errors.rs` (286) would
both cross 300 and force a split under CLAUDE.md §11. They reached 368 and 418.

**And the panel convened to add directories concluded that directories do not
help.** The author asked for them mid-milestone, on the ground that
self-hosting would otherwise start disordered. Four seats counted instead of
arguing: a path buys **zero** `use` lines — 288 cross-directory references plus
164 intra-directory ones, 452 either way, because a path changes how a target is
spelled and never whether a `use` is needed — and it forces the **same renames**,
because 40 of the port's 119 files sit in 15 last-part collision groups that the
proposal's own uniqueness clause refuses. Nim's compiler has run 171 files in
one directory for years under the same rule. The instruction was *decide now*;
the answer was that the thing being avoided does not happen.

## What broke and why

**A diagnostic in a non-root module named the root file, and a line nobody could
find in it.** Both renderers assembled `src.name` + `line_col` themselves —
identical answers while there was one file, false the moment there were three.
Found by running a cycle case and reading the output, not by a test. The repair
is one `Source::locate`, and it earned itself twice more before the milestone
closed: the `--dump-scopes` listing reintroduced the same mistake in its first
draft, and the emitter's `#line` had been making it all along.

**`geom.Point(x: 3, y: 4)` is a construction, not a call.** The first qualified
branch routed it through the call path and the generated C read
`t3 = hero_unreachable(t1, t2)`; clang produced seventeen errors. The qualified
form is an ordinary name that says where it lives, so it needs every branch the
ordinary one has — and `call` had already made that split for unqualified names.

**A newline nobody typed moved the end of the file.** Normalising every file to
end in `\n` so files cannot fuse pushed EOF from `1:6` to `2:1` on a file
written without one. Two lexer tests, about a language with no modules, caught
it. The separator is now closed *before* the next file, so the last file keeps
exactly what was written.

**A measurement had been wrong since M6, and repairing it made the number
worse.** `heroes mutate` built a bare `Source` while the CLI has attached the
library since M6 — so the metric was scoring a frontend nobody runs. Both now go
through the same loader, and the score moves from 97%/81% to **96%/79%** over
the same 839 mutants. The lower pair is the compiler's. What differs is real:
with the library present `xs.map(f)` resolves to a declaration and is checked
through its generic signature; without it the same call reaches the built-in
table and meets a stricter shape rule, so some mutants were being killed by a
rule that does not fire in the compiler users run.

**Two defects shipped and were caught by a judge compiling something else.** The
mangler received the raw `use` name, so modules `print` and `print_inst` with
functions `inst_value_name` and `value_name` both emitted
`h_print_inst_value_name` — exit 2, *the compiler is wrong*, on a legal program,
and the exact pair `mangle.rs`'s own doc names as the reason the component is
sanitised. Panel 031 R10's diagnostic did not fire because it compares module
*names* and the collision is in the concatenation: correct, and comparing the
wrong two strings. Alongside it, every non-root module's `#line` named the root
file, which would have handed §4.19's guarantee — clang checking an `extern`
against the real header — to a file that does not contain the declaration.

**One prediction was right about the outcome and wrong about the mechanism.**
Panel 030 said the library becoming an ordinary module would delete
`is_library`, `library_line_of`, `LIBRARY_MODULE` and `writer::LIBRARY_FILE`;
panel 031 withdrew that on a grep, and it was correct — the file table alone
deletes nothing. Then `writer::LIBRARY_FILE` and `Writer.source` died anyway,
three commits later, when the two `#line` entry points collapsed into one. The
deletion came from the rule, not from the data.
