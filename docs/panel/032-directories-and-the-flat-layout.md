# Panel 032 — Directories, and the count that says a flat layout is the answer

**Convened** 2026-08-12, mid-M8a, by author instruction ("voglio risolvere adesso
il tema delle cartelle... se no il self host parte molto disordinato").
**Trigger** surface syntax and semantics, `spec/**` (CLAUDE.md §4).
**Status** `provisional — author ratification pending`.

## The proposal, verbatim

> Panel 031 landed modules two hours earlier: `use geom` reads `geom.hero`
> **beside** the importing file, always qualified, no nesting — and its own
> record recommended deciding subdirectories at M10, with packages, because the
> escape valve is additive so waiting costs nothing. The author overruled the
> deferral: the port (M8b) would otherwise start with ~40 files in one
> directory, against a Rust tree of **121 non-test files across 13
> directories**.
>
> **D** (+38, 2472) — `- A module in a subdirectory is named by its path: `use
> syntax/decl`, which still binds `decl`. Two modules whose last parts match is
> an error.`
> **A** (+40, 2474) — the same with `use "syntax/decl"` (Nim's escape valve).
> **C** (+50, 2484) — `- A directory is a module too: if `syntax.hero` is absent
> and `syntax/` is there, `use syntax` binds every `.hero` in it, and those
> files see each other unqualified.` (Go's model.)
> **S** (+0) — stay flat.

The ergonomist received the four as `v-0` … `v-3`, label-stripped, in no
meaningful order, with `v-0` (today's language) among them and unmarked.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **veto C · object D/A · approve S** | **D and A buy exactly zero `use` lines**: 452 either way, because a path changes only how a target is *spelled*. And they force **24 renames** over the port's own tree |
| llm-ergonomist | **veto v-2 (C) · approve v-1 (D)** with condition | C is **the only silent wrong program** it could build from any of the four documents — and it is triggered by *adding a file*, not by editing one |
| spec-warden | **veto all three · approve S, "not yet"** | Principle 0's burden is not met, and the killer is empirical: **40 of 119 files sit in 15 last-part collision groups**, so D renames 34% of the tree it exists to tidy |
| ffi-pragmatist | **veto C · approve D** with condition | compiled it: basename components collide **22-way on `mod`** over 169 paths, whole-path concatenation collides **zero** times — and under C a `ui/` directory holding an ncurses and a raylib binding **does not compile**, 8 errors inside `raylib.h`, from a library the program never calls |
| historian (advisory) | **support D · oppose C** | **Nim's compiler is 171 `.nim` files in one directory**, with the same last-component rule, and has been for years. TypeScript spent 2019–2023 *removing* C's model and measured a 46% smaller package for it |

## C is dead, and four seats killed it independently

Three vetoes and one opposition, from four different inputs, and none of them
was asked about the others' ground.

The **ergonomist** built the program the bullet invites and it is the only
silent wrong answer in this panel's whole record. C's wording says "*if
`syntax.hero` is absent* and `syntax/` is there" — so the meaning of `use
syntax` in every file of a 40-file program depends on whether a file that no
line mentions exists. Create `syntax.hero` later — as a facade, a generated
file, an unfinished rename — and `syntax.show` silently rebinds from
`syntax/tree.hero`'s `show` to the new one. Both type-check. The program prints
something different. It also could not establish that `use lex` from inside
`syntax/scan.hero` is legal at all, since the only import rule says *beside
this file* — under which C **cannot express a 40-file compiler**.

The **ffi-pragmatist** compiled the FFI consequence, which is its seat's own:
under C the translation unit's header set is chosen by *directory membership*
rather than by what the program uses, so an unreferenced `.hero` drags its
header and link flags in. Two bindings in one directory — ncurses and raylib,
the same row of §1.11's table — produce 8 errors inside `raylib.h` under C and
exit 0 under D.

The **compiler-engineer** priced what C actually buys — 165 fewer `use` lines —
and found it buys them by deleting "always qualified", which panel 030 R4
adopted and panel 031 ratified, and by adding a **directory-enumeration
primitive that is not on the closure list**.

The **historian** found the natural experiment already run: C is TypeScript's
namespaces, and TypeScript's own team spent four years removing them, citing
implicit cycles and useless incremental builds, and published a 46% package-size
and 10–25% build-time win for the migration.

And the **spec-warden** noted that C's conditional clause repeats panel 023's
falsified form exactly — a rule smuggled into a subordinate clause, which this
project's own DESIGN-LOG calls "the highest-risk text in this document".

## A is dominated by D, and the reason is one measurement

Nim needs quotes because a path component may not be an identifier
(`import "gfx/3d/somemodule"` — `3d` is not one). **Heroes' syntax is
ASCII-only and every component would be an identifier**, so the quotes are dead
syntax. The ergonomist then measured what they cost: writing the same six-file
program under A, it produced `use token` in one file and `use "lex/token"` in
another — **one module in two lexical classes, so no single textual search finds
every importer**. Under D both are identifiers.

## The disagreement that decides the panel: D against S

Two seats for D, two for S, one advisory for D — and the seats do not disagree
about any fact. They disagree about what the facts mean.

**The facts, all counted rather than argued:**

- **D buys zero `use` lines.** 288 cross-directory references plus 164
  intra-directory ones; a path changes the spelling of a target, never whether
  a `use` is needed. 452 lines flat, 452 lines nested.
- **D forces renames on the same files a flat layout would.** 40 of 119 files
  fall in 15 last-part collision groups (`decls` ×5, `types` ×5, `exprs` ×4,
  `stmts` and `builtins` ×3 …), and D's own error clause refuses every one. The
  repair is `types/typedecls.hero`, qualified `typedecls.foo()` — which is flat
  naming with directories painted on.
- **The pressure is smaller than it feels.** design.md sizes the port at 5–8k
  lines; at CLAUDE.md §11's 300-line cap that is ~27 files, at 200 lines/file
  ~40. Nim's compiler has run at **171 files in one directory** for years, under
  the same last-component rule.
- **Hierarchy arrives late without incident.** Go's compiler was 60,000 lines of
  C before it was split into packages, *after* self-hosting; today
  `cmd/compile/internal` is 47 directories. The historian searched for a case
  where adding hierarchy late broke shipped programs and found none.
- **Waiting is free and acting is not.** `module_path_has_no_parts` already
  refuses every `/` and `.` in a `use` line, so the path form is purely
  additive later. Landing it now also pre-empts M10, where packages produce the
  case that actually decides D against A.

**Against that, the two seats that want D now** are answering a different
question: not *is it needed* but *is it better to read*. The ergonomist ranked
the four documents on one test — from a call site alone, in a 40-file program,
can you find the defining file? — and D won it outright, because the last part
is a program-wide unique key *by rule*, and the directory tells you the
subsystem for free. That is §1.3's locality, and it is the thesis's own
currency.

## Resolution — `provisional — author ratification pending`

**R1 — C is struck, and not deferred.** Three vetoes on three independent
grounds, none liftable by wording: it makes a call's meaning depend on the
filesystem, it makes a `use` line's meaning depend on a file no line mentions,
and it chooses a translation unit's headers by directory membership. It is also
the only candidate that forecloses the others.

**R2 — A is struck.** Dominated by D: its quotes are dead syntax under an
ASCII-only grammar, and they cost one module two lexical classes.

**R3 — S is adopted, provisionally: the port starts flat.** The most
conservative resolution, and the one the counts support — D buys zero `use`
lines, forces the same renames, and costs ~100 lines and 26–38 spec tokens for
a tidier `ls`. Principle 0's burden is not met: subdirectories are not on
§1.0's closure list, no Part 11 effect has been measured, and "the port would be
tidier" is the shape measurement 003 rider 3 refused when this project's own
ROADMAP scheduled `outline` and `explain`.

**R4 — if the author overrules, the form is fixed and it is not D as tabled.**
The warden beat the wording by 12 tokens and the ergonomist supplied the clause
that makes it unambiguous. The landing form is **+26 measured (2434 → 2460)**:

```
- A `use` may be a path: `use syntax/decl` binds `decl`. Last parts are unique.
```

with the ergonomist's condition folded in as the sentence that decides
root-relative against file-relative — it predicted ≥50% of `use` lines written
from a subdirectory to a peer directory would take the root-relative spelling,
and D as tabled does not say which is right.

**R5 — the C component is the whole path, not the last part.** The
ffi-pragmatist compiled both over the port's 169 real paths: last-part
components collide **22-way on `mod` alone**; segments sanitised and
concatenated collide **zero** times. This holds whether or not D ever lands, and
it is the shape `module_names_collide` must compare.

**R6 — `use` is scoped to *the program*, said in the spec.** The historian's
condition: OCaml's flat module namespace is survivable within one program and
unfixable across libraries, which is why Dune's wrapped libraries exist. Heroes
has no unit above the file yet, so the uniqueness rule must not inherit a global
scope by silence when M10 adds one.

## Two live defects, compiled rather than argued — and both are M8a's

Neither depends on this panel's outcome. Both are in what shipped this evening.

**D1 — panel 031 R10 does not fire, and the case it was written for is exit 2
on a legal program.** `module_names_collide` compares module *names*; the
collision is in the *concatenation*. Reproduced with the real compiler, on the
exact pair `mangle.rs`'s own doc names:

```
$ heroes build print.hero        # module `print`, function `inst_value_name`
                                 # module `print_inst`, function `value_name`
internal error: compiling the generated C failed:
  error: redefinition of 'h_print_inst_value_name'
exit 2
```

The mangler's doc says sanitising the module to `[A-Za-z0-9]` closes exactly
this, and it is right — but `FileEntry.module` carries the **raw `use` name**,
which may contain `_`, and the emitter hands that to the mangler. The component
that reaches C has to be `module_of` of the module, and R10 has to compare
components rather than names.

**D2 — every non-root module's `#line` names the root file.** In the four-module
calculator, `main.hero` is 78 lines, the emitted `#line` values reach 410, and
`lex.hero`, `parse.hero` and `eval.hero` appear **zero times** in the emitted C.
At M7 that delivers §4.19's guarantee — a wrong `extern` caught by clang against
the real header — to a file that does not contain the declaration. It is the
same class as step 3's diagnostic defect, in the one renderer that was not
routed through `Source::locate`.

**D3, recorded for M7 and not for here** (ffi-pragmatist, candidate-independent):
`heroes build` reports a clang failure as `internal error … exit 2`, and §4.19's
whole design routes the *author's* FFI mistakes through clang. A wrong `extern`
must arrive as exit 1 with clang's text forwarded, or the thesis at the boundary
ships under the banner "the compiler is wrong".

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | at M8b close, `find selfhost -name '*.hero'` ≤ 45 and ≥95% of stems already globally unique; under D the nested `use` count differs from flat by <5%, and ≥20 files carry a disambiguating prefix anyway | M8b |
| llm-ergonomist | under C, ≥50% of models writing a file in a subdirectory that depends on a peer directory emit a bare `use lex` whose legality the document does not establish | harness run |
| llm-ergonomist | under D **as tabled**, ≥50% of `use` lines from a subdirectory to a peer take the root-relative spelling; if the compiler is file-relative, a 6-file 3-directory program first-try-compiles ~0% of the time | harness run |
| spec-warden | if D lands as tabled, the port renames **≥40 of 119 files** to satisfy last-part uniqueness — the identical work a flat layout does at +0 tokens. Falsified if M8b lands with ≤5 renames | M8b |
| ffi-pragmatist | at M7, under whole-path components, `db/sqlite.hero` completes §4.19's ladder step 3 with no shim and no change to the emitted C's shape — compiled, linked and run today. Falsifier: any binding whose `extern` C name varies with its directory | M7 |
| historian | porting 121 files across 13 directories, D's duplicate-last-part error fires **at least once**, and **every instance is resolved by renaming a file, never by needing an alias**. If the port needs `use x/y as z`, D was the wrong candidate | M8b |

## What a veto would compel

C's three are not liftable by wording and the resolution strikes it. The
warden's veto on D and A lifts on any one of: a metric-1 run where nested beats
flat on multi-file tasks; a demonstrated flat-layout blocker the compiler or
CLAUDE.md §11 actually refuses; or **the author overruling on the record** — in
which case R4's +26 form is what lands, never D at +38.

## The author's instruction, and what this panel says back to it

The instruction was *decide now, or self-hosting starts disordered*. The panel
answers the second half rather than the first: **it will not start disordered,
because directories do not tidy it.** The 452 `use` lines are the same either
way, 40 of the 119 files need renaming under both, and the compiler whose
layout this is modelled on runs 171 files in one directory today. What
directories buy is the reader's answer to "where does this name live", which is
real and is §1.3's own currency — and which is also exactly what a flat
`syntax_decls.hero` buys, at +0 tokens and +0 lines.
