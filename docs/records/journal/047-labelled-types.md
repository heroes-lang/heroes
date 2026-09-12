# 047 — M-labelled-types

Closed 2026-09-11. One sitting, panel 129, five seats, judging a thing that had
already been built — the route panel 128 priced, vetoed on cost, and scheduled
with the measurement that would lift its own veto.

## Goal

Repair defect 026 at the author's instruction, given in five words the day panel
128 closed. §4.9 names the arguments of a call when two parameters share a type,
and the rule reads a DECLARATION's names; a function type had none at any of its
sites, so `f: (function(str, str) -> str) @ report` was called positionally and
handed its two arguments over the wrong way round at exit 0.

## What surprised

**The blast radius was a tenth of the estimate, and the reason is one line of the
checker.** Panel 128's compiler seat priced the route at eleven modules by Part
5's mechanical test. The tree says otherwise: `.function_ty` is CONSTRUCTED at one
site, and its 192 other mentions are match arms binding the payload whole. A
variant case that gains a field costs its constructors, not its readers.

**Generics were free, and not by luck.** `check/generics.hero`'s `bind` descends a
function type structurally — arity, then parameter by parameter, then the result —
and never compares two whole interned ids. So the moment names entered the type's
identity, every generic higher-order call in the tree kept working without a line
written for it.

**The names had to be read where the source still existed.** `check/lower.hero`
turns a written type into an interned id and has the tree, the resolver's answers
and the table — and no source text, which is what a span needs. Its thirty-seven
call sites made a signature change the wrong repair, so the resolver, the last
pass holding the source, records the names per type-arena node and the lowering
reads them back as text.

**The C moved in exactly one way, and the way it moved was a name.** Nine of 441
emission traces changed, and each is byte-identical once three things are removed:
the synthesized `h_0fn_<hash>` names, the function-pointer typedef lines and the
`#line` rows. The hash is taken over the type as RENDERED, so a named type gets a
new name and a program with one gains a typedef. The canonical key was then made
to omit the names' share when a type has none, so a type nobody named keeps the
name it always had — without that, every function typedef in every program would
have been renamed to announce that nothing had been added.

**The headline metric does not move, and the record says so first.** `heroes
mutate --operator swap-args examples` reads 1733 of 1910 killed, 96%, on the seed
compiler and on the new one, to the mutant, because `examples/` holds no function
type with two parameters of one type. What moved is a corpus written for the
shape: four programs, six mutants, **one killed before and six after**.

**Three of five seats sent the work back, and the panel earned its keep.** This is
the shape worth keeping from the sitting. The warden refused a sentence whose
reading was false against a running program and handed back a wording eleven
tokens cheaper that said more. The ffi seat wrote a zlib binding the new rule
broke and would not lift its object until `_` stopped being a name. The compiler
seat could not break the design and vetoed anyway, because the tree did not
compile and because the number it had been given was `grep`'s rather than the
repository's.

## What broke and why

- **The build was broken for sixteen minutes and I did not notice.** A new
  function `label_key` shadowed a local of that name in the same file, and the
  compiler said so; I read a failed build as a passing one and went on measuring
  with a binary from before the change. Panel 129's compiler seat found it with
  `stat`, compared the binary's time against the file's, and named it CL-054 with
  the coordinator in the chair. Every row of the brief it could not reproduce is
  in the sitting's file.
- **The line count was `grep`'s and not the instrument's.** `.claude/rules/module-shape.md`
  says to ask the instrument; I asked the shell and reported +178. `code_lines`
  in `tests/harness/suite_layout.hero` counts every non-blank line outside a test
  block, comments included, and says **432**. The condition that was supposed to
  lift a veto was *about 150*.
- **A wildcard parameter is what C callbacks do for a living.** zlib's `free_func`
  ignores `opaque` in most implementations and reads it in the ones that pool, so
  one header typedef takes both `plain_free(_: ptr, address: ptr)` and
  `pooled_free(opaque: ptr, address: ptr)`. The first ran before and was refused
  after. `_` contributes no name now, and `table.fits` is an id comparison in
  every case but that one.
- **Names were mandatory and not required to distinguish.** The compiler seat
  attacked the shapes beside the repair and found `(function(a: str, a: str) ->
  str)` accepted, with the swap inside it accepted too. One rule closes it and its
  `_` twin.
- **The call site asked for more labels than a declaration does.** The ergonomist
  read the document cold and could not tell which rule it was reading, because the
  text's one example has two parameters and the two readings coincide there. They
  are one rule now: a name is written at the positions that share a type and
  nowhere else, in the type and at the call alike.
- **Three ceilings moved, and the table and the assert moved together.**
  `check/walk.hero` 1751 to 1842, `ast.hero` 484 to 491, and `check/table.hero`
  past 300 for the first time at 371.

## What landed, and what carried forward

**The rule.** A function type names the parameters that share a type with another
and no others; the names are part of the type's identity; a declared function used
as a value carries its own names on the same condition; a call through such a
value is checked exactly as a call to a declaration is. Four diagnostics carry it:
`needs_parameter_names`, `named_parameter_in_function_type` with its `certain`
fix, `repeated_parameter_name`, and `label_on_function_value` narrowed to a
position that has no name.

**The number.** `heroes mutate --operator swap-args`, four programs of calls
through function values, before and after:

| | before | after |
|---|---|---|
| mutants | 6 | 6 |
| killed by `heroes check` | 1, **17%** | 6, **100%** |
| the same operator over `examples/` | 1733 of 1910, 96% | **identical to the mutant** |

**The document.** 5663 real tokens, **+39**, digest `0d61cc71040884f6`; § 3's
function-type row goes back to what it said before panel 128 and § 9 carries the
whole rule. Ledger row 66 says what paid: **nothing**.

**The price, in the repository's own unit.** **432 lines** with comments counted,
240 without, against a condition of about 150. It is in the sitting, in the
ledger row and in `docs/work/DECIDE.md`, because the seat that set the condition
vetoed the absence of that number rather than its size.

**What carried forward.** A callback's ROLES can still be inverted at its
definition where the type's parameters are distinct letters: `fold` handed a
function that reads its two the other way round prints `cba` for `abc` at exit 0.
design.md §4.9 excludes monomorphised types explicitly and gives its reason, and
the compiler seat gave a second, independent ground, so it is a question about
that criterion in `docs/work/DECIDE.md` rather than an open defect. And nothing in
the language can see a Heroes signature and a C header read backwards together:
the front end keeps no parameter name for a typedef at all.

### The chain entry

| 48 | **M-labelled-types** | done 2026-09-11 | `m-labelled-types` | [047](journal/047-labelled-types.md) | defect 026 repaired: a function type names the parameters that can be confused, the names are part of its identity, and the classic inversion through a function value goes from 17% to 100% — at 432 lines against a condition of about 150, which is the sitting's own headline · **§1.2** |
