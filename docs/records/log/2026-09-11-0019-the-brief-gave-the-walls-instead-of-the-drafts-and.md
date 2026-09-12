2026-09-11 · **The brief gave the walls instead of the drafts, and two seats
invented the same repair while two others invented a different one** (panel 132,
step 2 of M-reflection-verdict, full five seats; `provisional — author
ratification pending`). Panel 131 had refused to rule compile-time derivation
because its own brief framed it as rendering Heroes' construction call; this
sitting asked the corrected question and offered four shapes **with their
objections already written**, saying that a fifth was the most valuable thing a
seat could produce and that *no shape survives* was a verdict. **Shape E, reached
twice independently**: the llm-ergonomist holding only the specification wrote
`Room::width`, the compiler-engineer holding the whole compiler wrote
`Point.width`, and the second built it — **+59 code lines**, **zero** in
`selfhost/emit/`, `runtime/`, the lexer, the parser, ownership and mono; 631
compiler tests and 109 goldens green; `heroes fmt` and double emit
byte-identical; `heroes check selfhost/main.hero` **14.41 s to 13.90 s** on a
still machine; `DECIDED.len()` still 18; **+36 vendored, ≈ +46 real** spec.
The grammar already parses `Point.x` and today refuses it, so the slot was free,
and `ir/flatten.hero` erases the form to an interned literal, which makes it
**sugar and not core** by §1.7's own test. **Shape B-prime, reached twice
independently too**: the ffi-pragmatist found that **a record cannot be generic**,
so a record of handlers fixes its accumulator and every byte must buffer in a
Heroes string before reaching C — §1.11's level-3 glue inverted — moved the
handlers into parameters and compiled a walk driving both a Heroes string and a
live `FILE *`, field-name literals going **18 to 0**; and the historian, reading
Ada, wrote that the precedent transfers *"only if the per-scalar writers are
passed as ordinary function arguments"*. **The negative claim is false and it is
thirty years old**: Ada shipped compiler-derived per-component serialisation with
no macro, no compile-time execution, no trait and no separate generator, twice,
`T'Write` in ISO 1995 and `T'Put_Image` in ISO 2022, the mechanism being an
attribute — and GNAT's own warning comes with it, that the derived format is
*"deliberately not documented"*, so a Heroes walk feeding a package must fix field
order or a golden pins what the compiler never promised. **The briefed shape is
refused on four vetoes with four distinct compiled grounds**, and the seat that
guards the document voted to land **nothing**, on 1-of-56 renderers saving 0
lines and on the `constant` idiom already giving
`error[unknown_name] … did you mean K_WIDTH?` at zero spec tokens — refused for
one measured reason, that **a constant is not bound to the field**, so one whose
body reads `"widht"` compiles and ships. **Two withdrawals by the historian of its
own panel 131 claims**, both recorded: Erlang's `record_info` does not transfer,
and its quadratic-derivation warning attaches to the type-level representation
rather than to flat per-field emission, which is an argument against the candidate
this sitting also refused. **What is NOT closed and is said rather than implied**:
the rot, which only the enumerating shape closes, scheduled with what it owes
because what was compiled is a **simulation of what the compiler would generate
and not the generation** — the distinction that cost panel 117 four days earlier.
**And a mutation operator lands with E and is what pays for it under §1.6**, two
seats converging without knowing of each other: `selfhost/mutate/ops.hero` holds
14 operators and **not one typos a string literal**, so the net is blind to the
exact mistake this milestone exists to kill — 0% caught on bare strings, 100% on
`Record.field`. **Panel 117's clause *until this language renders one* goes
false-by-standing** and its removal is queued separately, since it reverses the
author's wording choice of 2026-09-07. **Defect 028 filed beside the sitting** and
reproduced before filing: a record field of function type never called through
passes `heroes check` at exit 0 and aborts `heroes build` at exit 134 with
`assert failed: !found.is_err()`, no file, no line, no code | design.md §1.6,
§1.7, §1.11, §1.12, Part 11; CLAUDE.md §2, §9, §10 | — (033, 046, 117, 123, 131
are what it inherits or corrects)
