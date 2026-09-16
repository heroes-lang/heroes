# Panel 158 — compiler-engineer report

- **verdict**: `veto` on option 3 as scoped in the shared brief; `object` to
  option 1; recommend **option 4 plus one defect**.
- **section**: design.md §1.7 (core plus elaboration — *"does it remove a special
  case"*), design.md §4.15 as enforced by `.claude/rules/diagnostics-and-goldens.md`
  § *A new surface form lands in every tool that reads the language*,
  CLAUDE.md § Precedence rank 3 (robustness).

## What I ran (everything below is this session's, in a scratch copy)

Copy at `<scratch>/t`, `target`/`build` removed. Seed built
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` — **real 3.14 s**.
Patched compiler built once from `selfhost/`, **real 68.02 s** (one failed
checker-stage build before it at 17.41 s, a syntax error in my own test).

### The measurement that decides the sitting

**`???` is a reserved token of the language** — `spec/heroes-spec.md:201`
lists it in the literal production and `spec:324` says *"`???` is a valid
expression anywhere"*. It is the typed hole, and the lexer max-munches it.

I deleted the refusal at `selfhost/parse/type.hero:51-58` and replaced the
single application of `?` with a loop (the honest form of option 3: the current
code consumes the extra `?` and applies `.fallible` **once**, so deleting the
error alone would silently drop levels). Rebuilt. Measured:

| written | patched compiler |
|---|---|
| `x: i64?` | exit 0 |
| `x: i64??` | **exit 0 — the door opens** |
| `x: i64???` | **exit 1**, `error[expected_params_close] … found `???`` — the *hole* token |
| `x: i64????` | exit 1, same message |
| `x: i64?? ?` (space) | **exit 0** |

And the killer, run not reasoned:

```
$ ./heroes-next fmt <scratch>/p/g.hero     # g.hero holds `v: i64?? ? @ ok(ok(ok(7)))`
error: `fmt` produced source that does not parse: …:2:11: error[expected_binding_symbol]:
       expected `@` to declare a mutable, or `=` to bind, found `???`
error: this is a compiler bug — `…/g.hero` was NOT changed
exit=2
```

`selfhost/print/types.hero:25` renders `.fallible` as `render_type(inner) + "?"`,
so a three-level written type renders `i64???`, which the lexer then reads as a
hole. **Option 3 as scoped makes the formatter emit a program the parser
rejects**, and the formatter's own self-check says *this is a compiler bug*. On
the seed the same file is exit 1 at parse time, so the breakage is created by the
patch and does not exist today.

So **option 3 closes one door and leaves the third one open behind a diagnostic
that never mentions fallibility** — it is a partial closure, exactly the charge
the brief levels at option 1, and it is a §4.15 canonical-form breach on top.

### The doors, each one run by me on the seed

| route | check / build / run |
|---|---|
| `m: {str: i64?}`, `m["a"].must().must()` | 0 / 0 / 0, prints `1` |
| `find(xs: [i64?], p)` then `.must().must()` | 0 / 0 / 0, prints `1` |
| `wrap<T>(x: T) -> T?` applied three times | 0 / 0 / 0, prints `1`; `--dump-ir` names `i64???` at **27** sites |
| `function take(x: i64??)` | `error[nested_fallible]`, exit 1 |

### Option 3's line delta, measured by `git diff`

`selfhost/parse/type.hero` 535 → 533 lines. **17 removed, 14 added**; the
code half is **-10 lines** (the 17-line `?` block becomes 7), the test half
**+8**. That is the whole of §1.7's subtraction: **ten lines out of 62,132**
across 213 `.hero` files under `selfhost/` — 0.016%.

Against it, the collateral I can name with a path:

- `tests/golden/check/a-fallible-type-is-never-written-fallible-twice.hero`
  and `.expected` — seed exit 1 with 3 diagnostics; **patched exit 0 with no
  output**. 3 `#~ nested_fallible` annotations become orphans and a 3-line
  `.expected` becomes empty. `tests/golden/` is append-only
  (`.claude/rules/records.md` § *A record is never rewritten*), so this is a
  record edit, not a test edit. Suites `check` and `annotations` go red.
- `seed/heroes.c:1275` carries `HERO_STR_STATIC(hero_str_8fb9f99, "nested_fallible")`.
  The blessed seed must be regenerated in the same commit.
- `selfhost/check/table.hero:66-67` and `selfhost/check/builtins.hero:17` are
  comments asserting the invariant. **Both are already false today** — see the
  defect below.
- `spec/heroes-spec.md` would then owe a sentence, and the only honest one is
  *"`T??` is legal, `T???` must be written `T?? ?`"*. Panel 110 vetoed a `T??`
  spec sentence twice for being false; this one is true and unwritable.

**Closing the third door properly needs the lexer.** Either `???` splits by
context — the parser telling the lexer what position it is in, which is the one
structure a recursive-descent compiler should never grow — or the type parser
re-splits a `.hole` token into three `.question` spans, ~12-15 lines of token
surgery in `selfhost/parse/type.hero` plus span arithmetic so the caret lands on
the right character. That **adds** a special case. §1.7's arrow points the other
way, and option 3 is being recommended *because* of that arrow.

### R2, `is_thesis_rule`

Read `selfhost/diag.hero:87-104`: **15 codes**, `nested_fallible` is not among
them, and every one of the 15 is a rule about a *program the checker could
otherwise compile*. `nested_fallible` is a **parser** refusal: `--permissive`
cannot drop it, because dropping it leaves no type node to check. Part 11's
control arm is *the same compiler with §1 switched off*, and a form the front
end cannot build is not in §1's scope. **Not a defect. The list is right.**

### R3, one renderer or two

Two, and I read both. `selfhost/check/render.hero:47` renders the **checked**
type (`+ "?"`, recursive) and feeds the diagnostics, `ir/print.hero` (7 call
sites, lines 139-243) and `ir/mono.hero:353`. `selfhost/print/types.hero:25`
renders the **written** type and feeds `heroes fmt` (`print/fmt.hero:61 use
print/types`) and `--dump-ast`. A repair to what is *printed* has two places; a
repair to what is *parsed* has one. Both already print `??` and `???`
correctly — which is why the formatter breaks under option 3 rather than
truncating.

### The defect this sitting should book whatever R1 decides

`selfhost/check/table.hero:66-67` reads:

```
    # `T?` — never nested: `T??` is rejected by the parser, so this
    # node's argument is never itself a fallible.
```

Measured false three ways in this session (map, `find`, `wrap`), at exit 0.
`selfhost/check/builtins.hero:17` repeats it. This is
`.claude/rules/module-shape.md` § *A narrowing asks the value, never the world*
in its pure form: a premise about the world that expired in silence while the
comment goes on reading as correct. design.md:2836 still cites those two lines
as a live invariant a future `alias` feature would retire. **The correction is
owed under option 1, 3 and 4 alike, and it is two comment lines plus the test
that fires when the premise dies.**

### The fifth option, priced as a counted estimate (UNRUN)

Refuse where the value is **produced**, not where a type is written: a
`nests_fallible(types, id) -> bool` predicate in `selfhost/check/table.hero`
(~8 lines) called at generic instantiation in `selfhost/check/generics.hero`
(17 `.fallible` sites today) and at the `m[k]` and `ok()` rules in
`selfhost/check/builtins.hero` (19 sites), plus a diagnostic in
`selfhost/value_errors.hero`. **~55 code lines across 4 files.** It is the only
route that closes all three doors without touching the lexer. Its price is real
and it is not a parser price: it **breaks programs that run today** — my `find`
and `wrap` cases, both exit 0 — and it puts a refusal inside generic
instantiation, where §4.12's generics are deliberately unconstrained. I have not
compiled it. The command that would settle it is a patch to those four files and
`./heroes build selfhost/main.hero -o heroes-next`.

## Verdicts

- **R1.** **Option 4** (leave it, qualify the spec), **plus the comment defect
  above**. Option 3 is vetoed as scoped. Option 1 is objected to. Option 5 is
  the only complete closure and it is not affordable at this milestone under
  Principle 0 — nothing on the closure list needs it, and the compiler itself
  writes `??` at **0** sites in 62,132 lines.
- **R2.** Not a defect; `is_thesis_rule` is correct to omit a parser refusal.
- **R3.** Printing `i64??` is **acceptable and should stay**. The diagnostic's
  job (§4.17) is to name the type the checker has. Printing `i64?` instead
  would be a lie, and the two renderers already agree. What the message owes is
  a `note:` saying the type has no written spelling — one line in
  `selfhost/check/render.hero`'s callers, not a renderer change.
- **R4.** Yes, decisively. Consistency without a safety case is exactly the
  footing panel 155 R3 left `float_map_key` waiting on, and the ceiling argument
  that funded option 3 (§1.7 subtraction) is **-10 lines** while its measured
  collateral is a formatter that emits unparseable source.

## Veto

**Cast**, on option 3 as scoped in the shared brief. Ground: it lands a written
form whose canonical rendering does not re-parse (design.md §4.15, run above,
`fmt` exit 2 and the compiler's own words *"this is a compiler bug"*), and the
shared brief's claim that it *"closes all three doors"* is false as measured.
The veto does not reach option 3 **with the lexer half priced and landed** — it
reaches the version that deletes eight lines and calls the class closed.

## needed_for_self_hosting

**no.** `grep -rn '??' selfhost --include='*.hero' | grep -v '???'` reads **7**
lines in 62,132, and I opened all seven: six are comments or a test name, the
seventh is the string literal `"i64??"` inside `parse/type.hero:401`'s own test.
**Zero in type position.** The compiler compiles itself today with the refusal
in place.

## Prediction

At the **M-check-completeness close**, with option 3 landed as scoped: running
`./heroes fmt` over a `.hero` file containing a three-level written fallible
exits **non-zero with the string `this is a compiler bug`**, and
`heroes run tests/harness/main.hero -- ./heroes check annotations` reports at
least **3** orphaned `#~ nested_fallible` annotations. If option 4 is adopted,
`grep -c nested_fallible selfhost/` reads **2** at that close (it reads 2 today)
and `selfhost/check/table.hero:66-67` no longer asserts that a fallible's
payload is never fallible.

## Condition — what changes my verdict

Any one of:

1. A priced, compiled `.hole` re-split in `selfhost/parse/type.hero` that makes
   `x: i64???` check at exit 0 **and** `heroes fmt` round-trip it, with the
   added line count named. Then option 3 closes the class and I withdraw.
2. A measured Part 11 effect: a first-try rate on a task involving `m[k]` over
   `{K: V?}` that is worse under the current refusal than under option 3.
3. A program on the §1.0 closure list that cannot be written without a written
   `T??`. Today there are 0 in `selfhost/`.

## UNRUN

- The patched compiler's **own tests**: `./heroes-next test selfhost/main.hero`.
- Whether the patched compiler reaches a **fixpoint** (compiles itself byte for
  byte): `./heroes-next build selfhost/main.hero -o heroes-next2`.
- The golden suites under the patch:
  `./heroes run tests/harness/main.hero -- ./heroes-next check` and
  `… -- ./heroes-next annotations`. I predict both red on the one golden file
  named above; I did not run them (the brief forbids the net).
- Option 5 compiled. Its figures are counted estimates by analogy, marked above.
