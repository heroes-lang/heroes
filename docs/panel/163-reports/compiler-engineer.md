# Panel 163 — compiler-engineer

Every number below came from a command run on 2026-09-18 in a copy of the tree
at `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/work`,
seeded with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Nothing was rebuilt from `selfhost/` and `archive/bootstrap-rs/` was not read.

- `verdict`: **veto**
- `section`: design.md **§1.7 Core plus elaboration** (line 410) and **Part 5**
  (line 2532) — *"Anything in the core must be implemented in the type checker
  and the lowering and the backend"*; with CLAUDE.md § Precedence rank 3
  (robustness, design.md §1.12) carrying the route-3 half.
- `needed_for_self_hosting`: **no**. `grep -rnE ": *[iuf][0-9]+\[[0-9]+\]" selfhost/ --include='*.hero'`
  returns **8** lines, every one a short field; the compiler compiles itself
  without any of the four routes. Principle 0 puts all four on the thesis
  argument, not on compiler need.

## What is vetoed, and what is not

Vetoed: **route 1** (a call typed by context) and **route 3** (an uninitialised
`@` out-parameter). Both add a core construct under §1.7 and both are unsound on
a measurement below. **Route 2** (a zero default) draws an objection, not a veto:
it is sound, and it is four to six times the cost the brief was told.
**Route 4 runs today** and needs neither.

## Question 2 first, because the sitting was told something false

Panel 162's compiler seat is reported as saying `selfhost/emit/assert_spelling.hero:170`'s
`zero_of` already emits `(Color){0}`, and that `selfhost/check/walk.hero:616` is
the single wall. **I verified both. The first is true as a string and false as a
mechanism; the second is not the wall.**

`grep -rn "zero_of" selfhost/` returns **10 hits: one definition, eight unit
asserts inside its own file, and exactly ONE non-test caller** —
`selfhost/emit/extern_assert.hero:175`, which pushes the string into a list that
line 182 folds into `_Static_assert(...)`. That file's own module doc,
`selfhost/emit/assert_spelling.hero:1-9`, states what it is:

> How a type is spelled inside an assertion that is **NEVER EVALUATED** …
> `_Generic`'s controlling expression is unevaluated (C11 6.5.1.1p3), which is
> what makes the whole mechanism free — and why **nothing here may have a side
> effect or a cost**.

So `(Color){0}` is a type-probe argument inside a `_Static_assert`. It is never a
runtime value: it never reaches `selfhost/emit/body.hero`'s prologue (`:115`),
never reaches `selfhost/ir/own.hero`'s exit sweep (`:270`), never reaches
`selfhost/emit/construct.hero`. There is no edge from `zero_of` to any of them.
**The route from that function to a zero default is zero lines long because it
does not exist.**

Two more corrections in the same paragraph:

- `zero_of`'s own `.fixed` arm, `assert_spelling.hero:237`, returns `(void *)0`.
  It does not zero a fixed array at all.
- `walk.hero:616` is `fixed_array_length`, and it fires only when a literal IS
  written. The wall for OMITTING the field is `missing_fields`,
  `selfhost/check/walk.hero:2036` inside `check_named_fields` (`:2027`). Run:

  ```
  error[missing_fields]: `Utsname` is built with every field, named: sysname: — all of them, always
  ```

  That is §4.9's *field names are always mandatory at construction* (design.md
  line 1372), a different rule in a different function from the one the sitting
  was pointed at.

**Route 2's real price**, per file, with current line counts from `wc -l`:

| module | lines today | what route 2 adds |
|---|---|---|
| `selfhost/check/walk.hero` | 2292 | `check_named_fields` (`:2027`) is **shared by `construct_record` (`:1851`) and `case_construct` (`:1877`)**, and its label loop is positional — `arg[i]` must be `field[i]`. A gap-tolerant loop plus a shape test so a `.case` is not relaxed too: ~40–60 |
| `selfhost/data_errors.hero` | — (`:56`) | `missing_fields` changes meaning. That is a **diagnostic class**, which CLAUDE.md § 4 makes a panel question of its own |
| `selfhost/ir/flatten.hero` | 1301 | two `record_shape` producers, `:517` and `:666`, both fed by `lower_args`, which reads the **surface** `[ast.Arg]`. An omitted field has no AST node, so the zero must be synthesised in the IR: ~25–40 |
| `selfhost/emit/storageless.hero` | 148 | a third producer answering `"{0}"`. Its doc already says *"A third would want this module rather than another arm"*, so the seam exists: ~10 |
| `selfhost/emit/construct.hero` | 179 | compound-literal arity, via `emit/aggregate.hero` |
| `selfhost/ir/verify.hero` | 506 | the construct-arity invariant |

**~120–180 lines across five or six modules, in the checker AND the lowering AND
the backend.** By §1.7 that is a core construct, not sugar, and the brief was
told it was a checker rule.

## Question 1: route 1, and the brief's premise is false

The shared brief says *"A call typed by context does not exist here"* and counts
**6** context-typed sites, *"all of them literal forms"*. Measured in
`selfhost/check/walk.hero`'s `check` (`:379`), the expression-kind dispatch has
**7** context-consuming arms: `.array` `:469`, `.map` `:490`, `.variant_case`
`:505`, **`.call` `:508`**, `.if_expr` `:540`, `.match_expr` `:551`, **`.method`
`:579`**. Two of them are calls. `.call` passes `expected: ok(expected)` into
`dispatch_call` (`:1282`); `.method` passes `expected: ok(expected)` into
`method`. The comments beside them name panel 105 and defect 006 —
*"This line was `synth`, which drops `expected`, and the whole of defect 006 was
that one fall."* **The checking direction for a call exists and has for two
milestones.**

What does not exist is `expected` reaching a **built-in**: `builtin_dispatch`
(`walk.hero:1338`) has no `expected` parameter. `repeat` is a built-in
(`selfhost/inventory.hero:87`) and today is `repeat(s: str, n: u64) -> str`
(`selfhost/check/builtins.hero:256-267`). Threading it: ~15 lines in walk.hero,
~25 in builtins.hero. **Cheap.**

**The lowering is where I veto.** `selfhost/emit/ctype.hero` (511 lines) answers
*"C has no assignable array"*, and **a C function cannot return an array at
all**. `selfhost/emit/storageless.hero:86` refuses `.call` explicitly as *not a
producer of a fixed value*. So a `repeat(0, 256)` typed `i8[256]` must be
**erased at compile time** into a braced literal — which requires evaluating `n`
at compile time. `grep -rn "const_eval\|constant_fold" selfhost/ --include='*.hero'`
returns **nothing**: there is no compile-time evaluator in this compiler. Route 1
therefore needs one, or a new *"this argument must be an integer literal"*
argument rule — a concept the language does not have, in a built-in whose count
today is an ordinary `u64` value. That is a core construct hiding inside a
widening, and it is the third time this half has been priced as a widening.

## Question 3: route 3, and the hole is one no instrument here can see

Cost, before the hole:

- **A new surface form.** `spec § 5`'s `Statement = ident ":" Type ( "@" | "=" )
  Expression NEWLINE` gains a form with no expression. That triggers the whole
  walk in `.claude/rules/diagnostics-and-goldens.md` § A new surface form:
  `selfhost/print/fmt.hero` (**1528** lines) and its round trip,
  `selfhost/print/dump.hero` (**255**), `heroes mutate`, `suite_grammar`,
  `editors/vscode/syntaxes/heroes.tmLanguage.json` and
  `site/src/lib/highlight.ts`.
- **A definite-assignment pass over SLOTS.** `selfhost/ir/values.hero` (**256**
  lines) does dominance over **values**, and its doc says why it must:
  *"every temporary is hoisted to the C function's prologue and nothing is
  initialised there, so a value read in a block its definition does not dominate
  is a read of an uninitialised C local."* A slot-level version is a second pass
  of comparable size (~150–250 lines) plus a new diagnostic class.

**The hole.** `selfhost/emit/body.hero:145` and `:192` emit `" = {0}"` **only**
when `layout.is_refcounted`. An extern record is POD (spec § 13 allows a field to
be a number, `bool`, `ptr`, `cstr`, a record of the group, or a fixed array of
one — never a `str`), so it gets **no initialiser**. A `uname()` that fails
leaves 256 indeterminate bytes, and panel 162's own landed
`f.validated_bytes()` then reads them. Nothing here sees that:
`storageless.hero`'s doc says *"ASan cannot see an intra-object overflow in a C
struct at all"*, and `.claude/rules/c-boundary.md` says `--sanitize` is
`address,undefined` — **no leg runs MemorySanitizer**. Route 3 creates a class of
indeterminate read that **no instrument in this repository can detect**, and it
turns panel 021's loud zero-initialiser into a quiet one for exactly the types
the FFI touches. CLAUDE.md § Precedence rank 3: robustness is a goal of the
language and beats ergonomics. **Refusal, not a price.**

## Question 4: the route nobody listed WORKS TODAY, and I ran it

`./heroes run t/e.hero` printed **`Darwin`**. The program:

```
extern "shim.h"
    record Utsname tag utsname partial
        sysname: i8[256]
    function uname(@u: Utsname) -> i32
    function hero_utsname_zero() -> Utsname

function main()
    u: Utsname @ hero_utsname_zero()
    _ = uname(@u)
    print(u.sysname.validated_bytes().must())
```

beside a 9-line `shim.h` that `#include <sys/utsname.h>` and returns a memset
struct. **Zero language change, zero compiler change, zero spec tokens.** The
frontend accepts a group function returning a record with a 256-element array
field — I also checked it in isolation with `heroes check`, clean.

Measured sizes: `wc -c` gives **281** bytes of `.hero` plus **214** of `.h` =
**495**, against **1011** bytes for the full-literal program. The brief's 803 is
confirmed by regeneration; and `heroes fmt --in-place` **does not leave it
alone** — it reshapes the call and the canonical form is still one **785**-character
line.

Two guards I checked before recommending it, because a route that corrupts is
worse than none:

- **Lying about the length is refused.** Declaring `sysname: i8[8]` against the
  header's `char[256]` gives `error[ffi_field_type]: Utsname.sysname is not
  i8[8] in sys/utsname.h — clang read the header's struct and the field
  disagrees`. The corruption route is already shut.
- **An empty record is refused both ways.** `record Utsname partial` with no
  fields gives `error[empty_record]`; a `tag` with no fields is a handle, which
  the brief already names.

What route 4 costs is honest and belongs in the synthesis: a shim header **is**
C code the author leaves around, which is exactly what CL-028's *complete* FFI
instruction dislikes. But that is an ergonomics cost, and § Precedence lists
ergonomics among the things robustness and compiler size outrank, not above them.

## What I would write

**Nothing in `selfhost/`.** One row in design.md Part 6 refusing a zero default
and a context-typed call, naming the fact that would make the refusal wrong (a
library whose only producer of a long-array struct is an out-parameter *and*
whose header cannot be wrapped — macro-only, or the struct type itself
unnameable), and one sentence in `spec § 13` or an `examples/` case showing the
producer-function shape. If the sitting wants the ergonomics anyway, **route 2 is
the only sound one of the three**, and it must be priced at 120–180 lines across
five modules and a diagnostic-class change, not as a checker rule.

- `argument`: The mechanism panel 162's seat found is a `_Static_assert` probe
  with one caller (`extern_assert.hero:175`) in a context C11 never evaluates; it
  has no edge to the prologue, the ownership sweep or `construct.hero`, so route 2
  is 120–180 lines across checker, lowering and backend — §1.7's definition of
  core. Route 1 needs a compile-time evaluator that `grep` shows does not exist,
  because C cannot return an array. Route 3 creates indeterminate reads that
  neither ASan nor UBSan sees and no leg runs MSan. **Route 4 ran and printed
  `Darwin` with zero compiler lines.** Principle 0 gives none of the three a
  compiler need: 8 fixed fields in `selfhost/`, all short.
- `prediction`: If the sitting adopts route 1, 2 or 3, then at the
  **M-readable-bytes** tag `git diff --stat <tag~N>..<tag> -- selfhost/` will show
  **≥ 100 inserted lines touching ≥ 4 modules**, and at least one case under
  `tests/golden/check/` will need rewriting because `missing_fields` or
  `fixed_array_length` changed meaning. If it adopts route 4, that same command
  shows **0 lines under `selfhost/`** and `heroes measure spec/heroes-spec.md
  --refresh` moves by **at most +8 real** from 8030.
- `condition`: I withdraw the veto on route 1 if somebody lands, in a branch, a
  `repeat` against an expected `T[N]` whose count is not a literal and shows what
  `--dump-ir` and the emitted C contain — because that is the case I claim cannot
  be written. I withdraw it on route 3 if a leg is added that runs
  MemorySanitizer over a program with an exempted `@` binding, so the
  indeterminate read is visible to an instrument rather than to an argument. I
  soften the objection on route 2 if the omission is confined by a test that
  `case_construct` is unreachable from the relaxed `check_named_fields` and the
  measured diff comes in under 60 lines.
