# 027 — M-package-layout: a `use` may be a path, and a file may rename what it binds

**Closed 2026-09-02**, tag `m-package-layout`. Three sittings (`docs/panel/099`,
`100`, `101`), fifteen judge verdicts, three steps, and one rule added to
CLAUDE.md by author instruction.

## Goal

Decide what replaces the refusal `module_path_has_no_parts` — *"a module name is
one word"* — and make it work. Panel 032 had ruled the flat layout in on
2026-08-12 and fixed the landing form if the author ever overruled; this
milestone is that overrule, taken as its own deliverable because
M-separate-compilation delivered the build architecture instead
(`DESIGN-LOG:435`).

Three questions had to be answered in order, and each one was a sitting:

1. **Where does a path start?** Panel 099: at the directory of the file you
   compile, the same from every file, with no `..`.
2. **What happens when two paths end in the same word?** Panel 100: a file
   renames one with `as`, and uniqueness is scoped **per file** rather than per
   program.
3. **Should a path bind its last part at all, or the whole path joined?** Panel
   101: the last part — refused the alternative on reversibility, over four
   seats that approved it.

What landed: `use syntax/decl` binds `decl`; `use syntax/decl as sd` binds `sd`;
a file may not bind one name twice; every `use` starts at the directory of the
file you compile. Spec **3592 → 3663**, headroom 433.

## What surprised

**The base was already implemented.** `modules.hero` opened every module at the
compiled file's directory plus its name, and had done since M-module-namespace —
so *"beside the file that names it"* and *"the directory of the file you
compile"* were the same sentence, and only a flat tree made them look like two
rules. Panel 099's compiler-engineer found that before the sitting ruled, which
is why the discovery pass changed zero lines for the feature it exists to serve.

**A rule can be adopted in the morning and priced by its own corpus in the
afternoon, and the corpus wins twice.** Panel 100 was convened because the
compiler's own 169 modules cannot be grouped into directories under
*"last parts are unique"* — 13 to 15 collision groups over 32 to 36 files,
depending on the mapping. Panel 101 was convened because the author asked whether
the whole path joined would bind better, and the answer measured **127 of 127
modules binding exactly the name their call sites already write, zero
collisions**, with the spec **39 tokens shorter**. Neither sitting was foreseen
when the milestone opened; both were forced by measuring the thing that had just
been decided.

**A seat can prove a theorem and then run it.** Panel 101's spec-warden observed
that panel 032 R5 sanitises a module's whole path to `[A-Za-z0-9]`, which erases
`_` and `/` alike — so any collision between joined bindings is already a
collision between C components, and `module_names_collide` has refused every one
since M-module-namespace. The ffi-pragmatist then enumerated **162,165 path
pairs**: zero counterexamples. The author's own worry — `aa_bb.hero` beside
`aa/bb.hero` — is that theorem's most obvious instance and was already exit 1
before this milestone opened.

**Four seats approved a rule and the fifth's argument won.** Panel 101 split 4-1
for the joined binding. What decided it was the one argument no other seat
touched: the last-part rule is **reversible** and the joined rule is not — adding
Idris-style unambiguous-suffix qualification later is additive, while changing the
default rewrites every call site in every program ever written. The historian put
that beside a search that came back empty (no language binds a whole path joined
by a separator that does not survive as structure) and beside the two languages
that do bind whole paths and spent that uniqueness on renaming immediately: Ada
ships `Text_IO renames Ada.Text_IO` in its **obsolescent** annex, and Haskell was
offered implicit last-part aliasing in 2001 and declined.

**The llm-ergonomist's veto of the rule that stands is outstanding**, and
`docs/panel/101` says so on the page rather than in a footnote. Its ground is
§1.3's own test: `state.enter(x)` is not determined by its line plus the
enclosing signature when three modules end in `state`, *"and the wrong
determination compiles"*. Its falsifier is registered — a harness task measuring
a non-zero silent wrong-module rate — and if that measurement ever comes back
non-zero, this milestone's central decision is wrong and the joined rule is the
answer.

## What broke and why

**Six consumers of one new form, landed one at a time, every miss found by an
instrument.** `as` reached the parser and the resolver, then four diagnostic
sites, then the formatter, then the AST dump — and at no point did the compiler
refuse to build. The last two are the interesting ones and they produced the
milestone's own rule (CLAUDE.md §9, by author instruction).

- **`heroes fmt` deleted the alias.** It printed a `use` line as
  `"use " + name`, so `fmt --in-place` turned `use render/scale as near_scale`
  into `use render/scale` and the next build could not find the module. design.md
  §4.15 is what makes this the worst defect available in that file: the canonical
  form exists so that *"any textual difference between two versions is
  SEMANTIC"*, and the formatter was making a semantic difference of its own.
  Found by the corpus, because `examples/shapes/` was formatted and then run.
- **And the guard that watches the formatter was blind in the same place.**
  `heroes fmt` already refuses its own output when that output *"holds a
  different tree"* — and it compares the two trees by **dumping** them, with
  `print_dump`, which had also not learned `as`. Both renderings dropped the
  word, so they agreed, and the guard reported the same tree while the formatter
  was deleting one. One omission, two consumers, and the second was the
  instrument watching the first. A self-check that compares two **renderings**
  can only see what the renderer carries.

**A `certain` fix that did not compile, twice, in two different shapes.** §8
makes `certain` machine-applicable and CI asserts an applied fix compiles, so
this class is the one a fix may never be in. First: four sites built a qualified
name from the module instead of from the file's own binding, so a file saying
`use db/sqlite as s` was offered `sqlite.open_db` — and the repair fixed two of
the four, with the compiler's own arity error finding the third and the type path
being the fourth. Second: `module_binding_taken`'s repair was built for a line
with no alias, so on a line that already had one it produced `as state as
ir_state`, which does not parse; it was invisible because that fix is a `guess`
and CI never applies those.

**A refusal that ate the line below it.** `use` is not in panel 007's ender list,
so a use line whose last token is punctuation — `use lex/..`, `use shapes/` —
closes with no terminator. Both new refusals recovered by scanning for one, ran
off the end of the line and swallowed whole declarations: two broken `use` lines
produced **one** diagnostic and zero uses. The repair recovers by LINE, which is
a landmark that exists whatever the last token was, and it was applied to the
older dot refusal too — which had the same bug and never showed it, because
`use geom.shapes` ends in an identifier.

**A comment written the same morning was already false, twice.**
`examples/shapes/main.hero` said *"last parts are unique across a program"* hours
after panel 100 repealed exactly that, and
`tests/golden/check/use-has-a-path.hero` claimed `use a / b` *"is legal"* — read
off the parser rather than run, and it is `error[expected_declaration]` at column
7. Both are corrected in place with the date, because the class matters more than
the case.

**§11's ceiling fired on four files and the answer was different for each.**
Prose was tightened twice; `resolve_vocab.hero` is a real split that
`resolved.hero`'s own port note had been asking for since the type walker
existed; and three ceilings were raised with the reason dated in the table,
following `print_fmt.hero`'s precedent (raised four times, never split, because
the language refuses the only cut available).

## Predictions, scored

**Panel 099's compiler-engineer — two clauses hold, one is false by 4.4×.** It
predicted *"zero changed lines in `selfhost/emit_*.hero` and
`selfhost/cli_units.hero`, and ≤120 added non-test lines total"*. Measured over
the milestone: **zero** files matching `emit_*.hero` changed and
`cli_units.hero` is untouched — both hold, with the honest note that `emit.hero`
(no underscore, outside the glob as written) changed by one line, the
`module_of` repoint. The third clause is **FALSE**: the path work alone added
**417** non-test, non-comment lines against a predicted ≤120, and the same seat
quoted that 417 back at itself in panel 100 while pricing the alias.

**Spec ledger row 3630 — the property holds, the instrument named was the wrong
one.** It predicted *"a `run/` golden holding one `use` path line copied verbatim
into two files at different depths compiles and runs identically from both, and
no golden in the corpus needs a different spelling for the same target"*.
Measured: `use geom/point` appears **three times, character for character**, in
files at two depths, and nothing in `examples/` or `tests/golden/` spells one
target two ways. But it lives in the **corpus** rather than in a `run/` golden,
because `tests/golden/run/` is flat and cannot hold a nested program — a fact
about the harness the row did not know when it named its instrument.

**Panel 100's spec-warden — LAPSED.** It predicted all 169 modules nesting with
zero renames and zero spec tokens, `module_last_parts_collide` firing 0 times and
`measure` still reading 3630. Nothing nested: **zero** files sit below
`selfhost/`, because nesting became its own milestone (chain row 29). Two of its
clauses are also unmeasurable in their own terms now — panel 100 deleted
`module_last_parts_collide`, and `measure` reads 3663 because panel 100's own
sentence landed. Marked lapsed under panel 046 R2 and **not** renewed under the
new milestone's name.

**Panel 100's ffi-pragmatist — LAPSED.** `examples/sqlite/` was not split into
`db/sqlite.hero`; the seat measured the property in its own copy and the
repository never got the case. Its live successor is panel 101's standing
prediction, which is the same claim with a stronger enumeration behind it.

## What landed, and what carried forward

**The chain.** M-package-layout closes as row 28, tag `m-package-layout`. Row 29
is **M-selfhost-nesting** and row 30 **M-corpus-coverage**, both scheduled by
author instruction on the closing day; the site's modules page is row 36's.

**What the next milestone inherits, and it is a choice rather than a task.**
Panel 100 R6 records three roads for nesting `selfhost/`, all priced: keep the
stems (`check/check_state.hero`) and the tree nests with **558** `use` lines
changed and **zero** of the **4065** qualified mentions touched; nest and rename
five files, and the short names arrive with all 4065 rewritten by the nesting
itself; or nest and alias, which is the same 4065 plus 39 `as` clauses the five
renames would have made unnecessary. The choice is the author's and the
milestone is blocked on nothing else.

**Two things are owed and named.** The ergonomist's outstanding veto has a
registered falsifier, and `docs/panel/101` R3 lists the three conditions under
which the joined binding returns. And CLAUDE.md §9's new rule — a surface form
lands in every tool that reads the language — has one instrument behind it so far
(the tree guard's own test) and would be better with a sweep.

**Measured at the close, 2026-09-02**: the compiler **170 modules, 49,994
lines**; the seed **802,624** lines of generated C, fixpoint byte-identical; spec
**3663** of a hard 4096, headroom **433**; runtime ABI **18**; **535** compiler
tests and the net **1200/1200**; 16 example programs; sittings **100**, journals
**27**.
