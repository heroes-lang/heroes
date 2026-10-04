2026-09-12 · **Two defects found beside a sitting and repaired at their classes,
and one of them is a rule that had already been decided and not followed**
(defects 027 and 028, both from the ffi-pragmatist's boundary experiments at
panels 131 and 132, both reproduced by the coordinator before filing).
**027**: `cstr` emits `const char *` and a header's member is often plain
`char *` — and `selfhost/emit/extern_field.hero` names `struct passwd`'s
`pw_name`, `pw_dir` and `pw_shell` in its own comment, because **panel 089
widened the per-field `_Static_assert` to accept both spellings** on the ground
that *"const on the pointee restricts WRITES, and the language offers none
through a `cstr`"*. The declaration was legal, the assertion passed, and the
compound literal was then refused by this project's own
`-Werror=incompatible-pointer-types-discards-qualifiers` at **exit 2**, as
`internal error: compiling the generated C failed`. **A binding the compiler had
deliberately admitted could be declared and not built**, and the message accused
the compiler's internals and named no repair. The repair casts a `cstr` value to
`__typeof__` of the member, foreign records and `cstr` fields only, in the same
form the assertion above it already uses. **028**: `emit/synth.hero::collect`
returned at once for `.named`, so it never walked a record's fields, and a
function type reachable only through a field reached no typedef —
`ctype.func_of`'s assertion, whose comment states the premise in its own words,
found it false and aborted at **exit 134** with no file, no line and no code,
after `heroes check` had passed the program at exit 0. It survived only while
nothing CALLED through the field, which is why every program that holds a
callback in a record and also calls it was fine. `collect` now walks fields and
case fields under a second `seen` set, the set being second because `into` IS the
typedef list, and existing at all because a record may hold itself through `[T]`
or `{K: V}`. **Both repaired at the class**: four shapes for 028 — direct,
nested, through an array, in a variant's case — and both proved in both
directions on this Mac, old **2** and **134** against new **0**. **And the 027
case says what it does NOT guard**: no suite clangs a `fixedbugs/` case, so the
file guards the emission and the build is proved by the run recorded in the
commit — stated in the case rather than implied, because a case that looked like
a compile guard and was not would be worse than none. `docs/work/DEFECTS.md`
returns to OPEN: 0 | CLAUDE.md §8, §12; design.md §4.19; `.claude/rules/c-boundary.md` | — (089, 131, 132)
