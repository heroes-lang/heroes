# Panel 165 — compiler-engineer

**verdict**: veto

**section**: design.md §1.7 (core plus elaboration — *"anything in the core must be
implemented in the type checker AND the lowering AND the backend"*), Part 5 (the
seven constructs), §1.12 (robustness), §4.19 (the FFI vocabulary and its
guarantee). CLAUDE.md § 2 (Principle 0) for the need question.

Every number below came from a command run in a scratchpad copy of the tree
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 3.40`), on
2026-09-19. The commands are given.

---

## The sentence, priced clause by clause

Panel 164: *"It needs no new expression, no lend, no built-in and no position
rule — one widening of §4.19's parameter list, with ordinary type identity doing
the matching. It is the only route of the seven where the compiler CHECKS the
extent instead of trusting the author or the callee."*

Three clauses are true. Three are false. The last one is **inverted**.

### TRUE — "no new expression, no built-in"

`i8[8]` already parses in a parameter position and the formatter already round-trips it:

```
$ ./heroes fmt t/fmt.hero          # extern "r6.h" / function arr_len(s: i8[8]) -> i64
extern "r6.h"
    function arr_len(s: i8[8]) -> i64
```

Byte-identical to the input. So `.claude/rules/diagnostics-and-goldens.md`'s
new-surface-form walk — formatter, dumps, `mutate`, the two highlighters — is
**not** owed. That is a real saving over routes 3 and 4 and panel 164 is right
about it. The refusal is the checker's, not the parser's.

### TRUE — "ordinary type identity doing the matching"

Extents are part of type identity, and both directions fire:

```
$ ./heroes check t/ident.hero
error[fixed_array_length]: `i8[8]` holds exactly 8, and this literal has 4 …
$ ./heroes check t/ident2.hero
error[type_mismatch]: expected `i8[4]`, found `i8[8]`
```

`.fixed(element, length)` is interned structurally (`selfhost/check/table.hero`,
539 lines; `table.intern(@c.out.types, .fixed(element: f32_id, length: 4))` at
`selfhost/check/ffi.hero:357`), so no new comparison is needed. Panel 164 is
right here too.

### FALSE — "no position rule"

`crosses_the_boundary` (`selfhost/check/ffi.hero:76-92`) is **one predicate
serving four positions**: the result (`:98`), the parameter (`:107`), the
`extern constant` (`selfhost/check/ffi.hero:159`), and recursively the types a
callback carries (`:84-88`). Run, three programs, three positions, one code:

```
$ ./heroes check t/r6res.hero     # function mk() -> i8[8]
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern`'s result …
$ ./heroes check t/r6c.hero       # constant K: i8[8]
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern constant` …
$ ./heroes check t/r6mut.hero     # function fill(@s: i8[8]) -> i64
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern`'s parameter …
```

C forbids a function returning an array (C11 §6.7.6.3p1), so the result must
stay refused, and an `extern constant` of array type is meaningless on this
side. A rule that opens one position and holds three is **a position rule by
name** — `arguments_may_stand` / `result_may_stand` (`selfhost/check/ffi.hero:262`,
`:281`) are the two that already exist, and the module's own doc says why they
are asked separately: *"a vocabulary rule cannot be the one that says where a
callback may stand."* Route 6 needs exactly the same separation for `.fixed`.
It is small — ~12-18 lines — but the sentence says it is not needed at all, and
it is.

Third position the sentence does not answer: `@s: i8[8]`. C's `void f(char b[8])`
and `void f(char (*b)[8])` are different functions; the `@` form is the second.
The sentence covers neither.

### FALSE — "one widening of §4.19's parameter list"

It is at least two widenings in two modules, and the second is the one nobody
named. `fixed_only_in_a_group` (`selfhost/check/ffi_sweep.hero:34-76`, 93
code_lines) is a **file-wide sweep** whose legal set is built only from group
records' fields (`:37-43`). A route 6 parameter's type node is unclaimed, so it
is refused there no matter what `crosses_the_boundary` says. Measured on an
ordinary function, where the shared brief measured only a local binding:

```
$ ./heroes check t/arg3.hero      # function take(x: i8[8]) -> i64
error[fixed_outside_a_group]: `i8[8]` is a C array member, so it belongs to a
  `record` inside an `extern` group … at t/arg3.hero:6:18
```

So `claim()` (`selfhost/check/ffi_sweep.hero:84-99`) must additionally claim an
**extern** function's parameter type nodes and no other function's.
`ast.FunctionDecl.is_extern` (`selfhost/ast.hero:446`) makes that possible; it
is ~12-20 lines. Two modules, not one.

### FALSE, and this is the veto — "no lend"

The argument has to arrive in C as something, and there are exactly three
candidates. Two are already ruled out by this repository's own record and the
third **is** the lend.

- **By value.** C has no array parameter passed by value. Not available.
- **By the address of a copy.** `selfhost/emit/field_lend.hero:40-48` is the
  record of that attempt, in the module's own doc: *"`fixed_text` gave
  `(void *)(t13.nsap)` — the address of a COPY. Reading through it is correct by
  accident… Writing through it is lost in silence: the probe filled eight bytes,
  C returned, and the program's record was unchanged, at exit 0 with every suite
  green."*
- **By decay from the place.** This is the only one that works, and it is
  `lent_place` (`selfhost/emit/field_lend.hero:54-85`) — the producer walk that
  follows a `.load` back to its cell and a `.field` one step off it. That
  function, plus the rule that the argument must BE a place, **is the lend**.
  `check/lending.hero` (250 code_lines) and `check/lend_types.hero` (138) exist
  to police exactly it.

Route 6 does not avoid the lend. It makes the lend **implicit** — the same
automatic decay panel 164's own table shows **four seats vetoing and the fifth
objecting** as R1, and the same the historian found eight of nine C-binding
languages refuse. The difference between R1 and route 6 is one author-written
number on the parameter. The crossing is identical.

### INVERTED — "the only route where the compiler CHECKS the extent"

This is the claim route 6's whole marginal value rests on, and it is backwards.

A **field's** extent is checked against the real header, by clang, today:

```
$ ./heroes build t/mis.hero -o /tmp/mis    # header: int8_t name[4]; Heroes: name: i8[8]
error[ffi_field_type]: `Slot.name` is not `i8[8]` in `mis.h` — clang read the
  header's struct and the field disagrees
```

A **parameter's** extent cannot be, and the reason is C's, not this compiler's:

```
$ cat /tmp/dp.c
void f(int8_t b[8]);
void g(int8_t *b);
_Static_assert(__builtin_types_compatible_p(__typeof__(f), __typeof__(g)), "not compatible");
$ clang -std=gnu11 -c /tmp/dp.c -o /tmp/dp.o && echo OK
OK
```

clang says the two are **the same type**. C11 §6.7.6.3p7 adjusts `T a[N]` to
`T *a`, so there is no extent in the header for any probe to read — the shared
brief measured the same thing from the AST and did not carry the conclusion.

So route 6 does not add a check. It compares an author-written number on the
parameter to an author-written number on the field, and calls the agreement of
two author claims a verification. Route 3 has the identical property with `n: 8`
written at the call — the only difference is that route 3's number is **visible
at the crossing** and route 6's is hidden behind type identity. And route 6 then
uses that unverifiable number to size a **write** into a Heroes record, because
50 of 141 pointer parameters in the census are non-`const`. That is §1.12, which
CLAUDE.md § Precedence puts at rank 3, above compiler size and ergonomics.

There is a second robustness loss, stated by the emitter's own module doc
(`selfhost/emit/storageless.hero:10-15`): the field read keeps the subscript on
an **array-typed lvalue** on purpose, because *"ASan cannot see an intra-object
overflow in a C struct at all"* and UBSan's array-bounds arm *"sees it ONLY
while the subscript sits on something whose C type is still T[N]."* A decayed
parameter is pointer arithmetic outside any array-typed lvalue, so `--sanitize`
goes blind for everything C does through it. Route 3 has this too; route 3 makes
the author write `.ptr()` and `n:` to ask for it.

---

## A defect found while pricing this, and it is route 6's failure mode

`heroes check` exits 0 and `heroes build` exits 2 on this program:

```hero
extern "r6b.h"
    record Slot tag slot
        name: i8[8]
        id: i32

function main()
    t: Slot @ Slot(name: [72, 105, 0, 0, 0, 0, 0, 0], id: 1)
    u: Slot @ Slot(name: [72, 105, 0, 0, 0, 0, 0, 0], id: 2)
    print(to_str(t.name == u.name))
```

```
$ ./heroes check t/eq.hero ; echo $?
0
$ ./heroes build t/eq.hero --emit-c -o /tmp/eq.c ; echo $?
internal error: compiling the generated C failed:
t/eq.hero:9:11: error: use of undeclared identifier 't24'
    9 |     t27 = t24 == t26;
2
```

The compiler blames itself for a `.hero` file. `check_fixed_flow`
(`selfhost/emit/gate.hero:356-382`) guards `.store` and `.construct`; its
fallthrough at `:380` lists `.binary` and `.call` as bare returns. Its own
comment says the enumeration was built after *"four other shapes went to clang
unrefused and clang blamed the compiler, every one heroes check exit 0, measured
2026-08-16"* — this is a **fifth**, still standing. `tests/golden/unsupported/fixed-array-flow.hero`
carries three rows and needs a fourth. Not in `docs/work/DEFECTS.md` (`**OPEN: 0**`).

**Why it is this panel's business**: `.binary` and `.call` are the two unguarded
arms, and route 6's whole mechanism is to put a fixed value in a `.call`
argument. It would open the other one on purpose.

---

## implementation_cost

Measured with the instrument that judges, `code_lines` in
`tests/harness/suite_layout.hero:525-540` — not `wc -l`, per
`.claude/rules/module-shape.md` (*"ask the instrument, not the shell"*).
`./heroes run tests/harness/main.hero -- ./heroes layout` → `2 passed, 0 failed`.

| module | code_lines | ceiling | headroom | what route 6 needs there |
|---|---|---|---|---|
| `selfhost/check/ffi.hero` | **275** | 300 (§11) | 25 | split `.fixed` out of `crosses_the_boundary:89` for the parameter position only, holding the result, the constant and the callback. ~12-18 |
| `selfhost/check/ffi_sweep.hero` | 93 | 300 | 207 | `claim()` claims an extern function's parameter type nodes. ~12-20 |
| a place rule (new, or inside `check/lend_types.hero` 138 / `check/lending.hero` 250) | — | 300 | — | the argument must be a field of a named binding. Route 3 paid 388 lines across two modules for this same question. ~40-80 |
| `selfhost/lend_errors.hero` 176 or `selfhost/ffi_errors.hero` 177 | — | 300 | — | ≥2 constructors with the notes §4.17 demands. ~30-50 |
| **`selfhost/emit/gate.hero`** | **360** | DECIDED **365** | **5** | `check_fixed_flow`'s `.call` arm, opened conditionally. ~10-20 → **a DECIDED raise** |
| `selfhost/emit/ffi_call.hero` 106 + `selfhost/emit/field_lend.hero` 100 | — | 300 | — | render the argument as a PLACE. `lent_place` exists; ~15-30 of wiring |
| `tests/harness/suite_layout.hero` | — | — | — | `assert DECIDED.len() == 18` at `:694`, or a raised number |
| `spec/heroes-spec.md` | real **8106** | 10240 | **2134** | one § 13 sentence; panel 164 priced the decay wording at **+57**. **No budget veto, and I say so plainly.** |

**~120–190 selfhost lines, in 6 to 8 modules, plus at least one DECIDED raise.**

Two things this is **not**. `emit/ctype.hero` (395 against a DECIDED 395, zero
headroom) is probably **not** touched: `emit/body.hero:186` skips a temporary
whose `c_type` fails, so the route goes through a storageless place renderer
rather than through `c_type:380`'s `"C has no assignable array"`. And the
checker side genuinely fits — 275 + 18 = 293, under 300. Panel 164 was right
that the frontend half is cheap.

**My brief's own premise is false and the instrument says so.** It states
*"`check/ffi.hero` is 384 lines, already above `.claude/rules/module-shape.md`'s
~300 threshold."* By `wc -l`, yes. By `code_lines`, the metric
`suite_layout.hero` actually judges with, it is **275**, it is not a `DECIDED`
row, and the suite is green. `.claude/rules/module-shape.md` names this exact
mistake: *"by `wc -l`, 71 of 189 `.hero` files under `selfhost/` pass 300 while
the suite is green."*

### Scoring panel 164's engineer, as the brief asked

Predicted for route 3: *"~90-150 lines, **none in a zero-headroom file**."*
Route 3 is `f.ptr()`, and it shipped at `ef7b013b` (defect 061), after
M-readable-bytes closed — not at M-readable-bytes, which shipped route 4.

```
$ git show --numstat --format='' ef7b013b -- selfhost/ | awk '{a+=$1;d+=$2} END {print a,d}'
547 218
$ git show --stat --format='' ef7b013b -- selfhost/ | tail -1
 13 files changed, 547 insertions(+), 218 deletions(-)
```

**547 insertions, +329 net, 13 files, three new modules** (`check/lend_types.hero`
138, `lend_errors.hero` 176, `emit/field_lend.hero` 108). Against a predicted
ceiling of 150: falsified by **2.2x** on net and **3.6x** on insertions. And the
second half:

```
$ git show --numstat --format='' ef7b013b -- selfhost/check/builtins.hero
2	2	selfhost/check/builtins.hero
```

`check/builtins.hero` measures **378** against a `DECIDED` **378** — zero
headroom. Route 3 touched it and survived only because the edit was net zero.
`suite_layout.hero` was not in that commit at all.

That is the calibration for panel 164's route 6 sentence: **the same sitting's
last one-widening claim shipped at 3.6x its predicted ceiling, and it was made
before anybody had opened the emitter.**

---

## needed_for_self_hosting

**no.** Every fixed-array declaration in the compiler's own tree is inside a test
string:

```
$ grep -rnE ':\s*[iuf][0-9]+\[[0-9]+\]' --include='*.hero' selfhost/ examples/ | wc -l
13
```

Twelve are `selfhost/**` test fixtures; the thirteenth is
`examples/raylib/main.hero:101`. The compiler declares **zero** fixed-array
fields and calls **zero** extern functions with an array parameter. The shared
brief's 172/1 count reproduces on my ruler
(`grep -rhn '^    function [a-zA-Z_]' … | sort -u | wc -l` → 172). Principle 0
is unmet, and the thesis half is unmet too: `./heroes run r3.hero` already
prints `2`, so the program route 6 would serve **runs today**.

On `.claude/rules/verification.md`, since the brief asked: the rule bites, but
only on half of route 6. The vocabulary widening invalidates no program in
`tests/golden/**` or `examples/**`. The **place rule** and the `check_fixed_flow`
`.call` guard are new refusals, and those are judged by `unsupported`,
`annotations`, `canonical` and `check`. A seat claiming the full
`run`/`emission`/`determinism`/`corpus` set here would be asserting a gate it
does not need.

---

## argument

Route 6 is core by §1.7's own test, not a widening. It needs the type checker (a
position rule `crosses_the_boundary` cannot express, plus a place rule), the
lowering (`check_fixed_flow`'s `.call` arm, in a file with five lines of
headroom) and the backend (a place renderer, because the address of a copy loses
every write at exit 0 — `emit/field_lend.hero:40-48`). Its stated value is
inverted: a field's extent is checked against the header by clang today; a
parameter's cannot be, because `__builtin_types_compatible_p` says `int8_t[8]`
and `int8_t *` are one type. Route 6 removes a clang-verified extent and
substitutes an unverifiable one, then writes through it. That is the decay four
seats vetoed at panel 164, wearing a number.

(117 words, counted)

---

## prediction

**If route 6 is adopted, then at the close of the milestone that lands it:**

1. `git diff --numstat <milestone-base>..<milestone-tag> -- selfhost/` will show
   **more than 300 inserted lines**; and
2. `tests/harness/suite_layout.hero` will have changed — a raised `DECIDED`
   number, or a new row, or `assert DECIDED.len() == 18` at `:694` becoming 19.

**Falsified if** route 6 lands in **under 300** selfhost insertions **and**
`git diff` on `tests/harness/suite_layout.hero` over that milestone is empty.

Grounded on the neighbouring shipped route: route 3, the same panel, the same
"one widening" framing, predicted at 90-150 and measured at 547 insertions
across 13 files with three new modules.

**Second, cheaper, checkable now:** the defect above
(`t.name == u.name` → `heroes check` 0, `heroes build` 2) is real and unfiled. I
predict a fourth row in `tests/golden/unsupported/fixed-array-flow.hero` and a
`.binary` arm in `check_fixed_flow` before any route-6 work compiles, because
route 6 opens the sibling arm. Falsified if route 6 lands with
`selfhost/emit/gate.hero:380` still listing `.binary` as a bare return.

---

## condition

Three things, any one of which moves me from veto to object, and the first two
together move me to approve.

1. **A probe that checks a route-6 parameter's extent against the header.** I
   measured that clang calls `void f(int8_t[8])` and `void f(int8_t *)` the same
   type. If somebody runs something that reads the extent back out of the
   header — the `-ast-dump` source range the shared brief's census stage two
   already uses, applied at build time rather than at survey time — then route 6
   becomes the only route where the compiler checks the extent, the sentence
   becomes true, and the §1.12 objection falls. **This is the measurement the
   sitting is actually missing**, and it is a day's work with an instrument that
   already exists in `scratchpad/extents/census.py`.

2. **A Principle 0 need.** One binding on the closure list, or in
   `examples/`, that route 3 cannot express. `getcwd(@buf: ptr, size: u64)` is
   expressible today and runs.

3. **A measurement of what route 6 would CATCH.** The shared brief names this as
   the number it most lacks and I could not produce it either. If somebody
   measures a class of wrong-field bugs that route 6's type identity refuses and
   route 3's `n:` admits, that is a §1.12 argument *for* it rather than against,
   and I would want to see it before the sitting closes.

What would **not** move me: a smaller line count. The cost is not the objection;
the unverifiable extent is.
