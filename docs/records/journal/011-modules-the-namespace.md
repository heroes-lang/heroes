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

## What landed, and what carried forward

Moved verbatim from `docs/ROADMAP.md` on 2026-08-12, when the ROADMAP became a
file about what is next (CLAUDE.md §14). The identifiers are the ones this
milestone was built under.

**Status: M8a closed 2026-08-12, tag `m8a` — a program is many files, and the
calculator is four of them.

    $ heroes test examples/calculator/main.hero
    7 tests, all passed

`use geom` reads `geom.hero` beside the file that names it; every cross-module name
is written qualified; the whole program is still **one `.c`**. The acceptance held on
all three counts — `examples/calculator/` is `main` → `eval` → `parse` → `lex`, its
stdout is byte-identical to the single file's, and no golden's emitted C changed
shape. Three of §1.0's four remaining rows are still open (**file I/O · `args()` ·
`exit(code)`**, whose route M7 decides); the fourth, **modules**, closes here.

**Two panels, and both were decided by counting rather than by arguing.** Panel 031
landed the spec at **+71** (2363 → 2434), and eleven of its thirteen resolutions cost
zero tokens because `use` was made to *bind* — the spec's own verb — so "shadowing is
a compile error" and "an unused binding is a compile error" already covered a local
named after a module and a `use` nothing reads. Its wording was fixed by a judge that
found the proposal's own example **illegal as the thing it demonstrated**:
`geom.dist2(a, b)` has two same-typed parameters, so labels are mandatory, and that
text is legal only as UFCS.

**Panel 032 was convened by author instruction to add subdirectories, and refused
them on a count.** A path buys **zero** `use` lines — 452 either way, because a path
changes how a target is spelled and never whether a `use` is needed — and it forces
the *same* renames, since 40 of the port's 119 files sit in 15 last-part collision
groups the proposal's own clause refuses. Nim's compiler runs 171 files in one
directory. "A directory is a module" was struck by three vetoes from three
independent grounds, including the only **silent wrong program** in this project's
panel record. The port starts flat; the decision is queued for ratification, and if
overruled the form is fixed at +26.

**Five defects, and three of them were found by running rather than by testing.** A
diagnostic in a non-root module named the root file and a line nobody could find in
it (the repair, `Source::locate`, then earned itself twice more). `geom.Point(x: 3,
y: 4)` is a construction and not a call, and routing it through the call path put
`hero_unreachable(t1, t2)` in the generated C. And two shipped defects were caught by
a judge compiling something else: the mangler took the raw `use` name, so two legal
modules produced one C symbol and **exit 2 saying the compiler is wrong**; and every
non-root module's `#line` named the root file, which would have handed §4.19's
guarantee to a file that does not contain the declaration. Each has a case named
after it.

**A measurement was wrong since M6 and repairing it lowered the number.** `heroes
mutate` built a bare `Source` while the CLI has attached the library since M6, so the
metric scored a frontend nobody runs. Corrected: **96% / 79%**, and the M6 line below
records the old pair as what it was.

414 crate tests (was 379 at M8a step 1), 13 golden harnesses, 32 CLI surface. Spec at
**2434** of 4096, 1662 of headroom, now pinned by a test: panel 024's delta gate
landed as `measure::gate`, so a spec change is red until the commit that made it true
writes the new number down. `heroes mutate` runs 1148 mutants, up from 839 — the split
calculator is corpus.

**After the close, same day, panels 033 and 034 and sweep 001**: 427 crate tests,
37 CLI surface, 95 `run/` cases, and an **eleventh mutation operator**. `typo-code` slips a character in an
error code and reads **25 mutants, 0 caught** — the only zero in the table — which
moves the corpus-wide pair to **93% / 78% over 1173** with the killed counts
*unchanged* at 1087 and 903: nothing got worse, twenty-five mistakes the harness
could not see entered the denominator (`docs/measurements/004-error-codes.md`).
**Three live defects, all found by judges compiling something else** — and then,
by author instruction, those three were **generalised into hypotheses and hunted
recursively**: four hunts, one rule (compile and run; a defect you only reasoned
about is not a finding), **twenty defects, all fixed**. Then the rule those
twenty produced — *a narrowing asks the value, never the world* (CLAUDE.md §11)
— was turned on the compiler and found **twenty more**, of which nineteen were
repaired and one was cleared by testing it (`docs/defects/001-the-post-m8a-sweep.md`).

They came in three shapes, and the shapes are the finding. **The scope widened
silently**: a flag or a set that meant *this file* when a `Source` held one file
and means *this program* since M8a — four of them, including §4.16's hole
exemption in two different passes. **A position was assembled by hand**: eight
callers building a location from `line_col` and `src.name` under a doc comment
that has said *"there is one function and no caller assembles the triple itself"*
since M8a — and the sharpened lesson is that `locate` can only protect the
location it is **asked** for, so a line number formatted into a `String` note is
outside every guard the compiler has, which is why M8a's own sweep walked past
ten of them. **And the emitter asked what the program *mentioned* where it needed
what a declaration *is*** — D3's shape, nine times, one repair retiring three.

Nine of the twenty refused or miscompiled an **ordinary** program: `record Box {
v: int? }` was exit 2, a map inside a record aborted on `==` while its `hash`
worked, `heroes build` on a file with no `main` blamed the compiler, and `check
--apply` panicked at 101 on any multi-module program. **No golden moved** — the
corpus was compiling none of these shapes, which is the same sentence D3 wrote
with ninety `run/` cases and the reason each fix ships with a case named after it.
The one expectation that changed was a sentence M8a had made false:
`declared_twice` said *"one file is one program"*.

Eighteen panel sessions were ratified the same day, 013 through 034, each
recording **what kind** of ratification it was.

**Runnable:** `heroes test examples/calculator/main.hero` · `heroes run
examples/calculator/main.hero` · `heroes check examples/calculator/eval.hero
--dump-scopes` (the `uses` section: every module the file names and every qualified
name it may write, with the file and line each comes from) · `heroes build
examples/calculator/main.hero --emit-c` (four files in the `#line` directives).

### M8a — Modules: the namespace, not the build architecture
`use`, **always-qualified** cross-module names (never a glob import: `x.f(y)` is
UFCS for `f(x, y)`, and an unqualified import makes `f` resolvable only from
another file's import list — panel 030 R4), one module string per declaration
through the mangler (`h_<module>_<name>`, `emit/mangle.rs` has anticipated it
since M5a), N source files in one `Source`, and **the library becomes an ordinary
module** — which deletes `is_library`, `library_line_of`, `LIBRARY_MODULE`,
`writer::LIBRARY_FILE` and `emit/builtins.rs`'s 35-line reachability walk.
**It emits one whole-program `.c`, exactly as today.** Principle 0's closure list
says *modules*; it never says translation units, and CLAUDE.md §13 forbids compile
time as a justification (panel 030 R1). Panel 029 R5b therefore does **not** need
answering here: whole-program monomorphisation still sees every instance, which
matters because six of the library's seven functions are generic.
**Acceptance:** ✅ 2026-08-12 — `examples/calculator/`, four modules,
`main` → `eval` → `parse` → `lex`, seven tests from three of them, stdout
byte-identical to the single file's, and no golden's emitted C changed shape.
**Subdirectories were refused** at panel 032, on a count: a path buys zero `use`
lines and forces the same renames. The port starts flat.
**Runnable:** `heroes build` and `heroes test` on the split calculator ·
`--dump-scopes` showing qualified names.


