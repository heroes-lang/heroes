# Panel 158 — completeness critic

Not a sixth judge. **No verdict.** This report names what the five seats did not
produce. Every number below was produced by a command run in this session
against the working tree with the committed seed binary `./heroes`; anything
unrun says so in its own words.

---

## 1. The hinge: the ergonomist and the warden, and the split is decidable

**The disagreement, quoted.**

The llm-ergonomist, reading only `spec/heroes-spec.md`:

> § 10: "`m[k]` is a `V?` with code `missing_key`". Substituting `V = i64?`
> mechanically gives `i64??`. § 3, the production: `Type = Prefix { "[" integer
> "]" } [ "?" ] .` … So a type carries **at most one** `?`. `i64??` is not a type
> this language has. **Both cannot be true.**

The spec-warden, grepping the same document:

> So the document says, deliberately and in four places: **the value exists, the
> spelling does not.** Those are consistent. There is no false sentence, so R1
> option 4's premise fails.

**Ruling: the warden wins the formal question, the ergonomist wins the question
that decides the sitting, and the warden's evidentiary claim is false as
measured.**

**(a) On formal consistency the warden is right.** § 10:287 is a rule about the
type an *expression has*; § 3:71 is a production over types a program *writes*.
A type system that computes types with no concrete syntax is not inconsistent,
and the historian's own item 3 is the shipped precedent for it — Rust's
`{integer}` is printed in diagnostics and rejected by the grammar, deliberately,
PR 35080. The ergonomist's step that does not follow is the last one: § 3
establishes that `i64??` is not a type this language **writes**, not that it is
not a type this language **has**.

**(b) On "the document says so four times" the warden is wrong, and it is
checkable.** Run against the document, this session:

```
grep -niE "cannot be written|no spelling|unwritable|not writable|cannot write|no written" spec/heroes-spec.md
→ 0 hits

grep -c '??' spec/heroes-spec.md        → 2
grep -n  '??' spec/heroes-spec.md       → 201: … | "true" | "false" | "nullptr" | "???" | "fail"
                                          324: `???` is a valid expression anywhere…
```

Both hits are `???`, the hole. The token `??` appears nowhere in the document.

The warden's four rows are not four statements of the distinction. Three of them
state the **value** half and none mentions a spelling — § 10:287 (`m[k]` is a
`V?`), § 9:258-262 (generics, no constraints), § 5:128-131 ("Both read the
OUTERMOST type, so a `[T?]`, a record holding one, or a type parameter that
arrived fallible is still dropped"). One states the **spelling** half and does
not mention that a computed type may exceed it — § 3:71. **The document states
the two halves and states the relation between them zero times.**

A reader reaches "the value exists, the spelling does not" only by performing the
inference the warden performed. That is the same inference the ergonomist
performed ("§ 10 is a semantic rule, § 3's production is syntax, so the compiler
computes a type the syntax cannot write") and then declined to trust, because the
document licenses neither it nor its negation. The tell is in the warden's own
row 4: *"`[ ]` is **optional**, **so** at most one `?`: the written form `i64??`
is underivable"* — CL-018's connective, in the sentence that carries the claim.

**(c) What it does to the sitting.** It removes the headline of the warden's own
R1. The warden refuses option 4 on the ground that *"'qualify the spec' has
nothing to correct, and any qualification restates the production"*, citing
ledger row 5863, which refused a draft *"for saying it twice."* Measured, the
document says it **zero** times, so a qualification is a first statement and not
a second. The warden's price table already carries the honest number: row 5, *"the
cheapest honest merge, § 3's type row **or** § 10's map sentence — **+9**
vendored."* **The +0 resolution is not available; the warden's own +9 row is what
the warden's argument supports.** Every other number in that report survives, and
so does the §1.6 payment, because the named removal (`check/table.hero:66-67`)
is unaffected.

It rescues nothing else. The ergonomist's veto on repair 2 rests on locality
(§ 5's "inference is local only" plus § 3's "No implicit conversions"), not on
the contradiction, so it stands untouched.

---

## 2. Three fifth options, and a sixth nobody named

**They are not the same option in three hats. They fall on two sides of the
language question, and two of them are additive rather than alternative.**

**Side A — change no language.** The warden's fifth (document unchanged, delete
the false invariant, add one clause to the message) and the compiler seat's R1
("option 4, leave it and qualify the spec, **plus the comment defect**") are
**one option disagreeing about one thing**: whether the spec gets a sentence.
Item 1 settles that disagreement — the sentence is a first statement, at the
warden's own +9, not +0.

**Side B — two different missing halves of option 3, and they are additive.**

- The **ffi seat's** half: option 3 plus moving the `pointer_element` refusal
  from the emitter to the checker. Its stated condition — *"if the sitting
  adopts option 3 **without** the checker half of my fifth option, my verdict
  stays `object` rather than becoming `approve`"*.
- The **compiler seat's** half: the `???` lexer re-split. Its veto is explicitly
  bounded — *"The veto does not reach option 3 **with the lexer half priced and
  landed** — it reaches the version that deletes eight lines and calls the class
  closed."*

These answer different failures. The ffi half fixes `check` 0 / `build` 1 on a
written `cstr??`; the lexer half fixes `fmt` exiting 2 with *"this is a compiler
bug"* on depth three. **Nobody scored option 3 with both.** Read off the five
reports rather than guessed, that version carries **zero vetoes** (compiler seat
withdraws by its own condition 1, ergonomist already approves 3, ffi seat becomes
`approve`), **one objection** (the warden's, on Principle 0 and §1.2 — untouched
by either half), and the historian's advisory precedent behind it (Swift has
always been able to write `T??`, and `case let value??:` is legal Swift). **The
sitting produced a zero-veto resolution and did not put it on the ballot.**

Separately, the compiler seat named a fourth fifth that the shared brief does not
list: refuse where the value is **produced** — a `nests_fallible` predicate
called at generic instantiation and at the `m[k]` / `ok()` rules, ~55 code lines
across 4 files, the only route closing all three doors without the lexer, and
**it breaks programs that run today** (its own `find` and `wrap` cases, both exit
0). Marked UNRUN by that seat and still unrun.

### The sixth: put the parentheses in `Prefix`, not the surgery in the lexer

Both the compiler seat's veto and the warden's § 4 rest on one measured collision
— `print/types.hero:25` renders `.fallible` as `render_type(inner) + "?"`, so
depth three renders `i64???`, which the lexer max-munches as the hole. Both treat
that collision as intrinsic to option 3. **It is not. It is intrinsic to
rendering `?` adjacent to `?`.** The ergonomist recorded the missing piece
without seeing its use:

> `Prefix = ident [ "." ident ] | "[" Type "]" | "{" Type ":" Type "}" | "(" ")"
> | "(" "function" TypeArgs "->" Type ")"` has **no `"(" Type ")"` alternative**,
> so parenthesising is not an escape either: `(i8?)?` is not a type.

Add that one alternative and the collision cannot occur at any depth: `(i64?)?`,
`((i64?)?)?` — no two `?` ever adjacent, **no lexer change at all**, no
context-sensitive lexing (the one structure the compiler seat says a
recursive-descent compiler should never grow), and `fmt` round-trips by rendering
a fallible-of-fallible parenthesised. It closes the third door the compiler seat
proved option 3 leaves open, in the grammar rather than the token layer, which is
where §1.7's arrow points.

What it costs, unpriced here and owed before anyone recommends it: `(i64)?`
becomes writable too, so `fmt` acquires a canonicalisation duty (strip redundant
parens) and the canonical-form check must judge it; § 3's production grows one
alternative, which the warden's instrument would price; and `(` after a type's
start needs one token of lookahead against the existing `"(" ")"` and
`"(" "function"` alternatives. **UNRUN.** The command that settles the grammar
half is a patch to `selfhost/parse/type.hero`'s `Prefix` plus
`selfhost/print/types.hero:25`, then `./heroes build selfhost/main.hero -o
heroes-next` and `heroes fmt` over a depth-three file.

A seventh, weaker, also unscored: the compiler seat **measured** that
`x: i64?? ?` with a space parses at exit 0 under its patch, and named the spec
sentence it would need — *"`T??` is legal, `T???` must be written `T?? ?`"* —
calling it *"true and unwritable"*. Significant whitespace inside a type is worse
than parentheses on every axis (invisible in source, fragile under `fmt`, and it
adds a special case rather than removing one), but it is a third route to the
same door and the sitting should record why it is refused rather than leave it
unmentioned.

---

## 3. The shared brief against the tree

**Already corrected twice by seats, and both corrections hold.**

- *"a message that cannot be acted on"* — **falsified**, independently by the
  warden and the ffi seat. Source confirmed this session:
  `selfhost/check/walk.hero:674` attaches
  `fix (guess): `.must()` — abort on the error case, giving `…``.
- The four-measurement table is the coordinator's own, and rows 1-3 have since
  been re-measured by seats and **confirmed** — the ffi seat re-ran row 3 and
  extended it to twelve shapes on two platforms with LeakSanitizer proved live by
  a deliberate control leak; the compiler seat re-ran row 1 and read `i64???` at
  27 IR sites.

**Row 4, checked here: the content is true and the citation is wrong.**
`grep -nE "^function [a-z_]+<" selfhost/library_source.hero` reads **0**, because
that library is Heroes source embedded as string literals. Reading the region
directly (`sed -n '70,125p'`): `map<A,B> -> [B]`, `filter<A> -> [A]`,
`fold<A,B> -> B`, `find<A> -> A?`, `any<A> -> bool`, `all<A> -> bool`. Six, one
wraps. **Row 4 is true.** Note that the warden's `needed_for_self_hosting: no`
rests on the same grep shape over `selfhost/`, so it is measuring string content
rather than declarations; the conclusion is almost certainly still right (the
compiler seat's independent `??`-in-type-position count reads 0), but the
instrument is not what it claims.

What does **not** follow from row 4 is the sentence it is used to carry. "The
surface through the library is one function" is true; "so the surface is small"
is not, and both the compiler seat and the ffi seat reached `T???` through a
**user** generic (`wrap<T>` twice, `Res???` via a handle), which is not among the
six and is bounded by nothing. CL-018, again in the brief's own connective.

### "So this is CONSISTENCY and not safety" — half measured, and the dichotomy is missing its third box

**The memory half is now measured, and it is the strongest thing in the sitting.**
The ffi seat put twelve shapes across macOS arm64 and a Linux container through
it — `str??` with allocating payloads at both levels, `Res??` and `Res???`
handles with `acquires` obligations, `find` over `[Res?]`, real `-lsqlite3` — with
LSan proved live by a control leak, and got the right answer every time, including
the two engineered `134`s. **§1.12 is not reached.** That is no longer the
coordinator's assertion.

**The behaviour half is unmeasured, and the brief's two boxes do not contain the
thesis's own failure mode.** A plausible mistake that COMPILES and yields a wrong
ANSWER is neither a §1.12 violation nor a cosmetic inconsistency. The ergonomist
named the shape from the specification alone and had no compiler to run it. **I
ran it, `./heroes check`, this session:**

```
# {str: i64?} holding a stored fail; the line reads as "did the value fail?"
function main()
    m: {str: i64?} @ {}
    m["a"] @ fail(code: "parse", msg: "no")
    if m["b"].is_err()
        print("err")
→ exit 0

# the same shape through the find door, no map in sight
function is_ok(x: i64?) -> bool
    return !x.is_err()
function main()
    xs: [i64?] = [ok(1), fail(code: "parse", msg: "no")]
    hit = find(xs, is_ok)
    if hit.is_err()
        print("absent")
→ exit 0
```

Both lines test the **outer** layer per § 5's "Both read the OUTERMOST type", and
both read as a test of the stored failure. Two doors, exit 0, today, before any
repair. **The brief's framing is right about memory and wrong as a framing**, and
R4 — answered "yes, decisively" by three of five seats — was answered against a
two-box dichotomy whose third box has a measured occupant.

Two consequences the sitting did not draw. **Option 1 is the only listed option
that makes the measured silent program a compile error** (it refuses the
declaration), which is a stronger position than any seat gave it — the ergonomist
ranked it 3rd as "misleadingly incomplete" and the ffi seat objected that it
"protects 0 programs". It protects the one program measured silent. And **option
3 does not close it**: making the type writable leaves `m["b"].is_err()` compiling
and still meaning the outer layer. The silent class is orthogonal to the whole
option set except option 1, which closes one of its two doors.

### One thing the ergonomist asked for and nobody ran, now run

Its hesitation 2 and prediction 5: *"Compile `m["b"].default(0)` against a
`{str: i8?}`. If it compiles today, the status quo already has a second silent
class and the sentence is owed regardless of which repair lands."* Run:

```
    m: {str: i8?} @ {}
    m["b"] @ to_i8(7)
    print(m["b"].default(0))
→ exit 1
error[bad_operand]: `print` takes any integer, a float, `bool` or `str`, found `i8?`
error[type_mismatch]: expected `i8?`, found `i64`
```

**Loud, twice.** The ergonomist's worry is falsified and the report is
strengthened by it: it wrote "four of five hesitations fail loudly today"; five of
five do.

---

## 4. The question the sitting should have asked

### (a) Does the `???` collision exist today, before any repair?

**Measured: no, and I looked for the one route that would make it live.** § 8 of
the contract makes only `certain` fixes machine-applicable, so the collision could
exist today only if some machine-applicable fix built its replacement text from a
rendered type. Enumerated every `certainty: .certain` replacement under
`selfhost/`: `""`, `"'\\'"`, `": @"`, `"::"`, `"@"`, `"\\" + escaped`, `"\r"`,
`"_ = "`, `"_"`, `"r"`, `expected + ": "`, `fix.word`, `inner`, `lowered`,
`name + " tag " + name`, `near[0]`, `text.slice(from: 0, …)`. The two I could not
read off the line I opened: `lowered` is a numeric-literal prefix
(`selfhost/number.hero:80`), `inner` is a `use`-line name
(`selfhost/parse/use_line.hero:251`). **None is built from `render_type`.** The
one fix that does embed a rendered type — `walk.hero:674`'s `.must()` — puts it in
the **title** and builds its replacement from `source.span_text(s, span)`, and is
tagged `.guess`. So the compiler cannot today write `i64??` back into a source
file. The collision is created by the patch, exactly as the compiler seat said.

The question behind it, which nobody asked in either direction: the hazard is a
property of one token. `???` is the only multi-character token in the language
built by repeating a single-character token that is simultaneously a postfix
operator and a type suffix. **The class has one member** — which is why option 3
looks like it needs lexer surgery, and why the grammar-level escape in item 2
dissolves it rather than paying for it.

### (b) Is anything else resting on `check/table.hero:66-67`?

Four sites carry the invariant in any wording, and the compiler seat's
characterisation of one of them is wrong:

| site | status |
|---|---|
| `selfhost/parse/type.hero:55`, `:403` | the refusal and its test — live and correct |
| `selfhost/check/table.hero:66-67` | *"`T?` — never nested: `T??` is rejected by the parser, so this node's argument is never itself a fallible."* — **FALSE**, measured three ways this sitting |
| `selfhost/check/builtins.hero:17` | *"`T??` is the one nesting Heroes' parser refuses"*, inside a port note about Rust's `Option<Option<TyId>>` — **TRUE**; the parser does refuse the written form, and this sentence claims nothing about what the checker can build |
| `docs/design/design.md:2836-2837` | *"it retires in silence the parser's refusal of `T??` that `selfhost/check/table.hero:66-67` relies on, unless the checker re-earns it"* — inherits the false comment into the `alias` costing |

**One comment is false, not two.** The compiler seat wrote *"Both are already
false today"* and predicted *"`grep -c nested_fallible selfhost/` reads **2** at
that close"* — that denominator counts the string, not the invariant, and the
invariant has four sites in three documents. The design.md inheritance is the one
nobody booked: a correction there is append-only under CLAUDE.md § 14.

**And the enumeration nobody performed.** The invariant being false makes one
question owed that the sitting replaced with a comment fix: **is any consumer of
the `.fallible` node non-recursive?** The node is touched in more than twenty
files under `selfhost/` — `parse/type` 9, `emit/convert` 5, `ir/print` 4,
`ir/flatten` 4, `emit/gate` 4, `ir/mono` 3, `emit/synth` 3, `ir/layout` 2,
`ir/phases` 2, and singletons in `resolve/types`, `ir/owned_release`,
`ir/questions`, `ir/place_store`, `ir/inout`, `print/types`, `emit/types`,
`emit/members`, `emit/extern_record`, `mutate/handles`, `cli/pointee`. The ffi
seat proved the **release** path recursive by reading the emitted C and the
compiler seat proved both **renderers** recursive. That is three of twenty-odd.
The command is `grep -rn "\.fallible" selfhost/ --include='*.hero'` followed by
opening each arm and asking whether it peels once or recurses. Cheap, unrun, and
it is the shape `.claude/rules/module-shape.md` names: a premise about the world
that expired in silence, corrected in the comment that stated it and not in the
code that may have believed it.

---

## 5. The historian's corpus argument

**Nobody has performed the Swift-equivalent count.** Two seats performed adjacent
counts and neither is it:

- ffi seat: map-of-fallible **declarations** across `examples/`, `selfhost/`,
  `tests/` = 4 occurrences, all in `tests/golden/check/`, zero elsewhere.
- compiler seat: `??` in `selfhost/` = 7 lines, zero in type position.

SE-0230's count was a different question: of the sites that **produce** a nesting,
how many **use** the distinction the flatten destroys — 613 producers, "zero cases
… distinguish between the error case and the nil-as-a-value case." The historian's
own condition 1 asks for exactly it and records it undone.

**It is cheap, and I ran the producer half.**

```sh
# door 1 — a map whose value type is fallible
grep -rlnE '\{[A-Za-z0-9_.]+ *: *[A-Za-z0-9_.\[\]]+\?\}' --include='*.hero' examples/ selfhost/ tests/
→ tests/golden/check/a-fallible-type-is-never-written-fallible-twice.hero
  tests/golden/check/no-size-through-fallible.hero
```

**Two files in the whole tree, both written to probe this very refusal.** The
remaining two doors are one command each and then a read:

```sh
# door 2 — find/map/fold over an array whose element type is fallible
grep -rnE '\[[A-Za-z0-9_.]+\?\]' --include='*.hero' examples/ selfhost/ tests/
# door 3 — a user generic instantiated at a fallible
grep -rnE '^function [a-z_]+<' --include='*.hero' examples/ selfhost/ tests/
# then, for each hit, the USE half: does any branch distinguish `missing_key`
# (or `not_found`) from a code stored in the payload?
grep -n 'is_err\|\.err \|missing_key\|not_found' <each hit>
```

With a producer set this small the use half is minutes, not a suite.

**And the sitting should say what the answer will mean, because it is not what
Swift's meant.** Swift found zero uses among **613** producers: the distinction
was real, reachable and unused, so the collapse was safe. Heroes will find zero
uses among **two** producers, both of which are probe tests. That is not evidence
the distinction is unused; it is evidence the corpus has nothing to say. So the
count does **not** license repair 2, and the historian is right that Swift's
precedent does not reach the container flatten in any case. What it *is* is
Principle 0 evidence in numeric form — the warden's *"nothing may ENTER on that
footing"* with a number attached — and it cuts equally against option 1, which the
ffi seat already priced as protecting 0 programs, and against the urgency of
option 3, which `selfhost/` needs at 0 sites.

**One procedural thing to settle before anyone runs it.** The ergonomist
pre-declared the measurement inadmissible: *"A demonstration that no program in
the corpus ever needs the distinction does not move me; I judge what a reader can
write, not what a corpus happens to contain."* That is defensible for a **veto** —
a corpus cannot show a hazard unreachable — but it fences off the one instrument
the historian's whole precedent runs on, and CLAUDE.md § 12 says measurement beats
opinion **including the panel's**. The synthesis should say which way § 12 points
here rather than leave both standing.

---

## What this report ran

`grep` over `spec/heroes-spec.md`, `selfhost/`, `docs/design/design.md`,
`examples/`, `tests/`; `sed` reads of `spec/heroes-spec.md` (§§ 1, 3, 5, 9, 10,
11), `selfhost/library_source.hero:70-125`, `selfhost/check/table.hero:60-80`,
`selfhost/check/builtins.hero:10-25`, `docs/design/design.md:2830-2842`; and four
`./heroes check` runs on probe programs in the scratchpad, exit codes captured
directly. No build, no suite, no rebuild from `selfhost/`.

**UNRUN and named:** the parenthesised-`Prefix` sixth option (grammar patch plus
`print/types.hero:25`, then a `selfhost/` build and a `fmt` round-trip); the
`.fallible` consumer enumeration; doors 2 and 3 of the corpus count; and the
spec-token price of the +9 merge, which needs `heroes measure` against a tree
carrying the draft.
