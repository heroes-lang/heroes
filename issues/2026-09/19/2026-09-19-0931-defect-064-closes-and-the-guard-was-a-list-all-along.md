# Defect 064 closes, and the guard was a list all along

2026-09-19, M-declared-extents. Found by panel 165's compiler-engineer while
pricing route 6, and the class widened by that sitting's completeness critic.

- [x] **064 — `==` on two fixed byte fields is accepted, then the compiler blames itself** | `check` exited 0 and `build` exited with `internal error: use of undeclared identifier`, so a program the checker admitted could not be emitted | `selfhost/emit/gate.hero`'s `check_fixed_flow`

## What it did

```
print(to_str(t.name == u.name))          # two i8[8] fields

heroes check  -> exit 0
heroes build  -> internal error: compiling the generated C failed:
                 error: use of undeclared identifier 't6'
                    10 |     t9 = t6 == t8;
```

**The class, measured before the repair**, which is the rule that a class is not
a class until its exceptions have been looked for:

| shape | before |
|---|---|
| `t.name == u.name` | check 0, **build 2** |
| `t.name != u.name` | check 0, **build 2** |
| `t == u`, the whole record | works |
| `t.id == u.id`, a scalar field | works |
| `t.name[0] == u.name[0]`, an element | works |

So the class is every `.binary` whose operand is a fixed array, and nothing
wider. The whole record works because the record's **generated `eq` walks
elementwise** — the capability is in the emitter; what had no path to it was a
direct comparison of two fields.

## The cause, and it was already written down

`selfhost/emit/gate.hero`'s own paragraph, from panel 081 R3:

> *Where a fixed array is allowed to FLOW — C's rule: a `T[N]` has no storage of
> its own, so it works in exactly the two positions that consume `fixed_text` (a
> subscript base, a construction argument). **Four other shapes went to clang
> unrefused and clang blamed the compiler, every one `heroes check` exit 0**,
> measured 2026-08-16.*

`check_fixed_flow` is therefore a **list**, built after four such shapes shipped,
and `.binary` was the fifth. It sat in the arm that returns without looking:

```
.lit | .load | .push_owned | .unary | .binary | .cast | .call => return
```

## The repair

`.binary` leaves that arm and gets a row, with the code the guard already has:

```
.binary bin =>
    if is_fixed_value(c, f, value: bin.left) || is_fixed_value(c, f, value: bin.right)
        note(@found, code: "fixed_flow", what: "a fixed array as an operand of an operator", span: span)
```

**No panel was convened, and that is the reason rather than the omission:** the
code, the diagnostic kind and the rule are panel 081 R3's, and the guard's own
documentation says the list grows as shapes are found. Nothing about the language
moved — `unsupported` is panel 020's second word, *the program is fine and this
compiler is unfinished*, so the program that was accepted-then-crashed is now
refused with a message and an exit of **1** rather than the compiler blaming
itself at 2.

**`.call` was NOT guarded beside it**, though the critic named the two as
siblings. Measured: a fixed array reaching a call is how `f.ptr()` and
`f.validated_bytes()` work, so a blanket row there would refuse the two lends
this language ships. What holds the rest of that position shut is `ffi_type` and
`fixed_outside_a_group`. The reason lives in the golden's header rather than in
the gate, because the gate is at its `DECIDED` ceiling and the golden has none.

## What it reports now

```
unsupported[fixed_flow]: a fixed array as an operand of an operator is not emitted yet
  at tests/golden/unsupported/fixed-array-flow.hero:63:12
     |
  63 |     same = a.reserved == b.reserved  #~ fixed_flow
     |            ^^^^^^^^^^^^^^^^^^^^^^^^
  note: write a fixed array out where its record is built, and compare the whole
        record or one element at a time — C has neither array assignment nor
        array comparison
```

**The note gained its second half with this repair**, because one code now keys
two shapes and the sentence named only the first. Both routes were RUN before
they were written, which is what that function's own paragraph asks: `t == u`
prints `false` at exit 0 and `t.name[0] == u.name[0]` prints `true` at exit 0.

## The case

`tests/golden/unsupported/fixed-array-flow.hero` gains a fourth row rather than a
new file, because it is the same class and that file's whole subject is the
positions a fixed array may not flow through. Its header said **three** and now
says four. The annotation `#~ fixed_flow` is in the source as well as the
snapshot, which is the witness a regenerator cannot invent.

## What it cost, and the ceiling it hit

`selfhost/emit/gate.hero` was at **360** code lines against a `DECIDED` **365**.
The first draft of the repair carried eighteen lines of comment and put the file
at **385**; `layout` refused it in those words — *a file already over the ceiling
may not grow further*. The comments were cut to two lines and the reasoning moved
to the golden's header, landing the file at **≤365** with `layout` green. Panel
165's compiler-engineer predicted this exact collision — *"`emit/gate.hero` at 360
vs `DECIDED` 365, five lines of headroom, and it must change"* — and the
prediction is scored **correct**.

## Gates

`unsupported` 15, `annotations` 166, `canonical` 2, `layout` 2, `order` 3,
`records` 24, `check` 127, `run` 127, `emission` 462, `determinism` 156 — every
one green on a compiler rebuilt from `selfhost/`. The wide set was run because
`.claude/rules/verification.md` says a change to what the compiler REFUSES is
judged by every golden tree and not by the `selfhost/**` row.
