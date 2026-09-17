# Panel 161 — compiler-engineer

Every number below was produced by a command run on 2026-09-17 in a copy of the
tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/tree`,
built with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
(`3.08 s` real). The command is named beside each number. Nothing here is copied
from the shared brief, from a milestone file or from an earlier sitting.
`archive/bootstrap-rs/` was deleted from the copy before any measurement.

- `verdict`: **veto**, scoped — see § The veto. Route 5 is the resolution I
  recommend; routes 1 and 2 are refused on soundness.
- `section`: design.md **§1.7** (core plus elaboration: "anything in the core
  must be implemented in the type checker *and* the lowering *and* the
  backend"), with **§1.12** ("the boundary is complete: any C library must be
  bindable") deciding between the admissible ones and **Part 5**'s core/sugar
  table as the instrument.
- `implementation_cost`: route 1 breaks **11 exhaustive `match`es across 6
  files**, measured; route 5 is **~60–90 lines across 5 emit modules**, no
  checker, no lexer, no IR, no runtime; route 5b is **one string** in
  `selfhost/cli/flags.hero`. Files and commands below.
- `needed_for_self_hosting`: **no**. `grep -rn '^extern "' selfhost/` returns
  four groups (`cli/process.hero:37`, `:49`, `cli/io.hero:34`,
  `emit/literal.hero:41`) and none declares an `i8`/`u8` field of any shape, so
  no plain-`char` binding is on the closure list. Principle 0 therefore gives
  routes 1 and 2 no entry ticket, and route 5 needs none: it puts no form into
  the language.
- `argument`: (≤120 words, § The argument)
- `prediction`: § The prediction, two of them, checkable at M-arm-platform
  close on the `heroes-linux-arm64` image.
- `condition`: § What would change my verdict.

---

## The finding that reorders the sitting: the sign is asserted in EIGHT places, not one

The brief prices route 3 as "a one-line change at `extern_field.hero:154` and
the corresponding scalar path". **Measured false.** I emitted the golden
program's C and compiled it with the compiler's own flag list plus
`-funsigned-char`, which is what an unsigned-`char` ABI looks like to clang:

```
./heroes build tests/golden/run/ffi-a-char-array-member.hero --emit-c \
    --include tests/golden/run > /tmp/charmember.c
clang <the 15 flags of selfhost/cli/flags.hero:53-69> -funsigned-char \
    -I runtime -I tests/golden/run /tmp/charmember.c runtime/runtime.c -o /tmp/cm
```

Six errors, not one:

| count | error |
|---|---|
| 1 | `static assertion failed … heroes-ffi-field Tag name` — the `_Generic` row |
| 4 | `implicit conversion changes signedness: 'int8_t' to 'char' [-Werror,-Wsign-conversion]` — the record CONSTRUCTION |
| 1 | `implicit conversion changes signedness: 'char' to 'int8_t'` — the READ `t.name[0]` |

The same run with `-fsigned-char` gives 0 errors and prints `68` / `68 7`.

A second program (`take(c: i8)` and `give() -> i8` against a header's plain
`char`, `--emit-c` then the same two compiles) adds two more mechanisms: the
`HERO_RET_I8` static assertion fails, and the probe's and the real call's
`int8_t → char` conversions are two more `-Wsign-conversion` errors.

So the sign of the header's `char` is asserted in eight places, enumerated by
grepping the four mechanisms and then by reading the errors above (that is where
this list came from — CL-057):

| # | position | file:line | lines in file |
|---|---|---|---|
| 1 | array-element field | `selfhost/emit/extern_field.hero:152-155` | 363 |
| 2 | scalar field | `selfhost/emit/extern_field.hero:226-234`, `:260-261` | 363 |
| 3 | result | `selfhost/emit/extern_assert.hero:70-78` (`HERO_C_UNSIGNED`, `HERO_RET_I8/U8`) | 238 |
| 4 | parameter, and every ordinary conversion | `selfhost/cli/flags.hero:67` `-Werror=sign-conversion` | 196 |
| 5 | construction write | `selfhost/emit/aggregate.hero:213`, `written_as` `:238-255` | 398 |
| 6 | fixed-array literal elements | `selfhost/emit/storageless.hero:73` | 148 |
| 7 | field and index reads | `selfhost/emit/container.hero:116`, `aggregate.hero:285` | 393 |
| 8 | `@` pointee width **and sign** | `selfhost/cli/pointee.hero` (310) + `selfhost/emit/ffi_pointee.hero` (342) | — |

(`wc -l` on those files, same session.) **No route named in the brief touches
more than two of the eight.** Any resolution that accepts `i8`/`u8` against a
plain `char` and stops at the assertion leaves positions 5, 6 and 7 failing at
clang, on a line whose `#line` points at an ordinary statement rather than at an
`extern` declaration — which is `ffi_narrowed`'s gate, so the likely arrival is
exit 2, *internal error: compiling the generated C failed*.

---

## Route 1 — a ninth integer type: CORE, and measured

`IntKind` is a variant, so the compiler counts its own blast radius. I added a
ninth case to the copy and asked the seed compiler:

```
# in the scratchpad copy, ninth case `ichar` inserted into selfhost/widths.hero
./heroes check selfhost/main.hero --brief
```

**11 `error[non_exhaustive]`, in 6 files** (the same command on the unmodified
copy is clean, `17.68 s` total by `time`; the modified run was not timed):

```
selfhost/widths.hero:41, :56, :63, :93, :105, :119   (6)
selfhost/emit/builtins.hero:258
selfhost/emit/assert_spelling.hero:84
selfhost/emit/descriptors.hero:40
selfhost/emit/literal.hero:59
selfhost/emit/ops.hero:200
```

That is the floor, not the cost. It excludes everything the checker accepts
silently: `selfhost/resolve/vocab.hero`'s primitive table (a surface name),
`selfhost/check/table.hero` and `check/builtins.hero`, `emit/convert.hero`'s
conversion matrix (`int_signed` is consulted at `:137`, `:142`, `:143`),
`emit/ctype.hero`, and the runtime — `emit/descriptors.hero:42` answers
`&hero_desc_i8`, so a ninth width needs a ninth `HeroDesc` in
`runtime/parts/desc.c` (207 lines, `hero_desc_i8` at `:130`, `hero_desc_u8` at
`:154`), a declaration in `runtime/heroes_runtime.h:350-353`, and a comparison
row in `runtime/parts/sort.c:135-138`. Sixteen descriptors exist today
(`grep -rho "hero_desc_[a-z0-9]*" runtime/ | sort -u`).

**And the sign is not representable.** `selfhost/widths.hero:61-64`:

```
function int_signed(kind: IntKind) -> bool
```

no target in hand, and `int_c_type` (`:91-100`) must return a fixed-width C
name for the emitter. A kind whose sign is the target's makes both functions
target-parameterised, and every caller with them — 25 call sites of the width
helpers outside `widths.hero` across 12 files
(`grep -rn 'int_kinds()\|int_signed(\|int_bits(\|int_name(\|int_c_type(\|int_contains(\|int_fits(' selfhost/ --include='*.hero' | grep -v widths | wc -l`).

**The soundness objection, measured.** A type whose sign is the target's makes
the *value a program prints* target-dependent. I compiled the emitted C of one
program declaring `name: i8[4]` and of the same program declaring `name: u8[4]`
(the emitter's own output, with the u8 variant transcribed by the five edits the
emitter would make), each under `-fsigned-char` and `-funsigned-char`, with
route 5's relaxation applied:

| declared | signed plain `char` | unsigned plain `char` |
|---|---|---|
| `i8[4]`, byte 0xC8 | prints `-56` | prints `-56` |
| `u8[4]`, byte 0xC8 | prints `200` | prints `200` |

The byte is the invariant and **the DECLARATION decides the value**. A ninth
type whose sign is the target's would print `200` on Debian arm64 and `-56` on
this Mac for one source file — it reintroduces, at the value level, exactly the
platform-dependence it was minted to abolish. That is the Rust `c_char` hazard,
and it is worse than what we have.

## Route 2 — a spelling legal only inside `extern`

**Lexing costs nothing and that is not the question.** The contextual words are
not keywords: they are `.ident` tokens compared by text at the parse site —
`selfhost/parse/tails.hero:218` (`tag`), `:240` (`partial`),
`selfhost/parse/members.hero:132` (`owned`), `selfhost/parse/group.hero:167-182`
(`link`, `package`). A ninth costs one comparison there.

The cost is what the word DENOTES. If it denotes a type, it needs everything
route 1 needs (a `Prim` in `resolve/vocab.hero`, a type-table entry, a
descriptor, conversions, a spec paragraph) **plus** a scoping rule the other
contextual words do not have, because a type escapes its group: a field of that
type is read somewhere else in the program.

**The sub-question the brief says decides the route answers it — against the
route.** A field bound this way has to be read into some Heroes type, and the
table above shows that once the read converts to a declared fixed-width type the
member's own sign is *unobservable*. So the new spelling buys nothing that
relaxing the assertion does not buy, and it costs a type. `t.name[0]` would
either yield the ninth type (and then every arithmetic and print site needs a
ninth arm, and the value is target-dependent again) or yield `i8`/`u8` by a
conversion the author writes — which is the current spelling with extra
ceremony.

## Route 3 — accept `i8` or `u8` against a plain `char`, everywhere

Cost as written: 2 lines (`extern_field.hero:154`, `:230-233`). Cost as
required: positions 1-7 of the table above, because of the five
`-Wsign-conversion` errors measured. **Its stated soundness cost is wrong on its
own terms** — the brief says it "permits a silent sign confusion: 200 read back
as −56", and the measurement above shows a `u8` field reads 200 on both ABIs and
an `i8` field reads −56 on both. The confusion the brief fears is not what this
route buys.

What route 3 does cost is fidelity where C is definite. It would accept `i8[4]`
against a header's `unsigned char[4]` and `u8[4]` against `signed char[4]`, both
of which are the author misreading a header that states its sign. Those two stay
refused under route 5 (measured below); under route 3 they pass. design.md §1.1:
comprehension decides, and a mistake that stops being a compile error is the
thesis's own currency.

## Route 4 — refuse

design.md §1.12 states the standard this has to meet: *"the boundary is
complete: any C library must be bindable, because a library Heroes cannot reach
is C code the author has to keep writing by hand"*. A refusal is held to Part
6's rule — name the program fact that would make it wrong — and the fact already
exists in the repository: `tests/golden/run/ffi-a-char-array-member.hero` binds
`char[4]` and runs green on three legs. Refusing would un-land M-complete-structs
on the fourth leg. `ffi_field_type` could carry the sentence (the mapper is
`selfhost/emit/ffi_field.hero:30-55`, 233 lines) but there is no repair to name:
the note would have to say *there is no spelling*, which §4.17 forbids
(`ffi_flexible_array_member` exists precisely because a message naming a repair
that does not exist was judged worse than none).

---

## Route 5 — the one nobody listed: change WHAT IS ASSERTED, and it is sound

**The `_Generic` rows are the compiler's choice, not C's.** Today every one of
the twelve rows asks width AND sign. For eleven of them the sign is a fact the
*header* states. For plain `char` it is a fact the *ABI* states, so asking it
demands of the author a spelling that does not exist. The repair is to ask, of
the plain-`char` row alone, the question C actually leaves open:

```
char (*)[N]: (sizeof(char) == sizeof(<elem>))          # the sign conjunct dropped, this row only
```

and, in the other direction, to make the emitted conversions explicit so the
declared Heroes type governs the value.

**Measured, five compiles, all on this Mac:**

1. The three refusals the golden case says must not relax, asked of the relaxed
   assertion under both ABIs (`clang -std=gnu11 -f{signed,unsigned}-char
   -DSHAPE=n refuse.c`): `i16[4]` against `char[4]` **refused** (width);
   `i8[8]` **refused** (length); `i8[2]` **refused** (length). Two shapes beside
   them, which nobody asked for and which decide whether the widening is
   scoped: `i8[4]` against `unsigned char[4]` **refused**, `u8[4]` against
   `signed char[4]` **refused**. Only the plain-`char` row widens.
2. The golden program with the row relaxed, an element cast
   `(__typeof__(((Tag *)0)->name[0]))` on the write and a cast to the declared
   width on the read: compiles clean and prints the same thing under
   `-fsigned-char` and `-funsigned-char` (`-56` for `i8`, `200` for `u8`).

**Cost, by file, all `wc -l` this session:**

| file | lines | what changes | estimate |
|---|---|---|---|
| `selfhost/emit/extern_field.hero` | 363 | the row loop `:152-155` special-cases `char`; the scalar branch `:226-234` gains an "or the member is plain `char`" disjunct over `unsignedness_of` | 12-18 |
| `selfhost/emit/extern_assert.hero` | 238 | `HERO_RET_I8/U8` `:72,:75` gain the same disjunct | 4-6 |
| `selfhost/emit/aggregate.hero` | 398 | `written_as` `:238-255` widens from `cstr`-only to an 8-bit int — **the precedent is exactly this, defect 027 / panel 089**, same function, same `__typeof__` form | 10-15 |
| `selfhost/emit/storageless.hero` | 148 | the braced literal `:73` needs the holder and member threaded in so each element carries the cast — the one fiddly part | 20-30 |
| `selfhost/emit/container.hero` | 393 | the index read `:116` casts to the declared element's C type | 5 |
| tests + goldens | — | one `run` case per ABI, and the four annotations the golden already carries | 20-30 |

**~60-90 lines, five emit modules. Zero lexer, zero parser, zero resolver, zero
checker, zero IR, zero descriptor pass, zero ownership pass, zero runtime, zero
new diagnostic class, zero spec grammar.** By Part 5's test this is not even
sugar: nothing enters the language, and `--dump-ir` is unchanged. Pascal-P4 is
~4000 lines; this is inside the noise of one module.

**What it does not fix**: position 4 (a plain-`char` PARAMETER) and position 8
(a plain-`char` `@` pointee). There is no cast available at a probe's call that
keeps the probe honest for every other type, so those stay refused. The scale
says that is the right scope: the brief's own AST walk counts **1** plain-`char`
parameter and **0** results against **23** array fields.

## Route 5b — the cheaper one, recorded rather than recommended (CL-040)

**The compiler chooses the sign.** Every clang line this compiler writes goes
through `flags.flags()` (`selfhost/cli/toolchain.hero:203`, `:266`, `:290`;
`selfhost/cli/units.hero:142`, `:229`). Adding one string, `-fsigned-char`, to
`selfhost/cli/flags.hero:53-69` makes plain `char` signed in every translation
unit this compiler produces, on all four legs, and `i8` becomes the portable
answer by construction.

Measured: the **unpatched** emitted C of both probe programs, compiled with
`-funsigned-char -fsigned-char` (an unsigned-`char` target overridden by the
compiler's own flag), gives **0 errors** and runs correctly — `68` / `68 7`, and
`-56` / `-56`. One string closes all eight positions.

Two things I ran before recommending against it, and one I could not run:

- the runtime does not depend on plain `char`'s sign where it matters: the UTF-8
  walk casts to `unsigned char` (`runtime/parts/text.c:28`), and
  `grep -rnE 'char[^*]*<[^=]*0|>= *0x80|& *0x80' runtime/` returns nothing;
- the AST-dump unit has its own flag list (`selfhost/cli/pointee.hero:186-197`)
  and would not get the flag, but it asks only for type TEXT; the verdict unit
  goes through `toolchain.run_clang` (`pointee.hero:237`) and would;
- **unrun**: whether any header this project binds has `static inline` code
  whose behaviour depends on plain `char`'s sign. That is route 5b's real
  hazard — the inline copy would be compiled signed while the shipped library's
  own copy was compiled unsigned, a silent value divergence in code the author
  did not write. It is a question, not a premise, and the instrument is a clang
  AST walk over the nine headers looking for `char` in a comparison inside an
  inline body.

Robustness is why I put 5 above 5b (CLAUDE.md § Precedence rank 3,
`.claude/rules/c-boundary.md`): route 5 changes only what this compiler emits and
leaves every header compiled exactly as its platform intends; route 5b reaches
into third-party code. The seed is also built without `flags()` — CLAUDE.md
§ Commands' line has no `-fsigned-char` — so 5b would have the compiler and the
programs it builds disagree about plain `char`, which is harmless today by the
grep above and is one more premise to keep alive.

---

## The veto

**I refuse routes 1 and 2**, and the refusal is not a price: a ninth integer
type, or any type spelling whose sign is the target's, makes one source file
print `200` on one leg and `-56` on another — measured above. That is not a
portability repair, it is the same defect moved from compile time to run time,
where design.md §1.1 says comprehension decides and CLAUDE.md § Precedence rank 3
says robustness wins. The ceiling argument is secondary and still holds: 11
compile errors, 6 modules, a ninth runtime descriptor and a spec paragraph, for
a form the compiler's own four `extern` groups do not need.

I do not veto routes 3, 4 or 5. Route 3 is priced wrong and scoped wrong; route
4 is refuted by a program already in the tree; route 5 is what I recommend.

## The argument (≤120 words)

C's plain `char` has no sign the header states, so the assertion demanding one
asks the author for a spelling that cannot exist. The fix is to stop asking that
question of that row — not to mint a type. Measured: with the plain-`char` row
asking width only and the boundary conversions cast, a `u8` field reads 200 on
both ABIs and an `i8` field reads −56 on both; the byte is the invariant and the
declaration decides the value. A ninth type whose sign is the target's does the
opposite, printing different numbers per leg for one source file. Route 5 costs
~60-90 lines in five emit modules, no core construct, no runtime, no spec
grammar. Routes 1 and 2 cost 11 broken matches and the property they were minted
to buy.

## The prediction

Both checkable at **M-arm-platform close**, in the `heroes-linux-arm64` image.

1. **If the resolution relaxes only the assertion** (`extern_field.hero:154` and
   the scalar path) and adds no cast at the construction and read sites, then
   `./heroes build tests/golden/run/ffi-a-char-array-member.hero --include
   tests/golden/run` on the arm64 leg fails with **five**
   `implicit conversion changes signedness` errors — four at the construction,
   one at the read — and the compiler reports **exit 2**, not `ffi_field_type`.
   Falsified if it builds, or if any diagnostic class catches those lines.
2. **If route 5b is taken instead** (one string in `flags.hero`), the arm64 net
   goes from `1825 passed, 3 failed` to `1828 passed, 0 failed` **and no other
   suite moves**. Falsified if any suite other than `run`, `determinism` and
   `emission` changes its count — that would mean the flag reaches further than
   the class it was added for.

## What would change my verdict

- **On the veto**: a measured program in which a ninth target-signed type makes
  a Heroes program's observable output *more* stable across the four legs than
  the declared-type-governs rule does. I could not construct one; if a seat can,
  the soundness half of the veto falls and only the ceiling half remains.
- **On route 5**: evidence that the element cast at
  `selfhost/emit/storageless.hero:73` cannot be threaded without a second
  traversal — if that cost passes ~150 lines or forces a new IR field, the
  balance against 5b's one string changes.
- **On route 5b**: the unrun header walk. If no bound header has sign-dependent
  inline code, 5b is one string against ~80 lines and the author should be shown
  that trade explicitly rather than having it decided here.
- **On both**: a Windows measurement. The fourth leg is unrun in the brief, and
  MSVC-targeting clang has plain `char` signed; if the Windows box says
  otherwise, every route above is priced against three data points and not four.
