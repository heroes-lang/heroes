# Panel 186, the llm-ergonomist's third reading: O_cover, read blind, its program built on R7's prototype

Run by the coordinator 2026-10-02 from 17:44 to 17:46 by `date`, on the authorization the author gave with their answer *2a* (R7's home (a), prototyped and read blind before the spec takes O_cover), from `<scratchpad>/186-blind-3/`, outside the repository and any git tree, no `CLAUDE.md` in or above it. Inputs: `spec.md`, the trunk's spec at `4e568244` with O_cover in place of `partial`'s three lines and the spec-warden's B1 bit-field clause (`docs/panel/186-briefs/blind-3/spec.diff`), `sa.h` and `brief.md` (in `blind-3/`). One candidate only: the reading measures whether the text leads to a program the R7 compiler accepts. Command: `.claude/skills/panel/SKILL.md` § 2's, `--model claude-opus-5-5`; `run.json`: `success`, 6 turns, 67.6 s, 0.251 USD. Its `context` reports nothing but the harness's system prompt; the reading stands.

**Its program built** (`docs/panel/186-briefs/blind-3/program.hero.txt`, kept as text since today's compiler refuses it), by the coordinator at 17:46 by `date`: on lane land186's R7 prototype (`<scratchpad>/lane-land186/r7/heroes-r7`, its diff `<scratchpad>/lane-land186/r7.diff`) **`build` exit 0, 0 warnings, and it prints `12`, `1.5`, `4`, `true`**, the four meant lines; on the trunk's compiler at `4e568244`, exit 1, `missing_fields` at `SA(kind: 2, f: 1.5, x: 4)`. The report follows verbatim.

# experiment

```
extern "sa.h"
    record SA
        kind: i32
        i: i32
        f: f32
        x: i32
    function make_sa() -> SA
    function sa_f(s: SA) -> f32
    function sa_x(s: SA) -> i32

function main()
    s = make_sa()
    print(s.i)
    t = SA(kind: 2, f: 1.5, x: 4)
    print(sa_f(t))
    print(sa_x(t))
    a = make_sa()
    b = make_sa()
    print(a.kind == b.kind && a.i == b.i && a.x == b.x)
```

Part the specification cannot express as asked: `a == b` on two `SA` values.
Section 13 makes comparing a record "holding a union's field that is not an
integer, pointer or array of them as wide as the union" a compile error, and
this record holds `f: f32` (needed to build with `f: 1.5`). The program
compares field by field instead, over `kind`, `i` and `x`. Since `i` is an
`i32` as wide as the union, this covers every byte of `SA` (no padding), so
it gives the same answer a whole-value equality would.

# choice_points

1. **Which union members the record declares.** Choice: both `i` and `f`
   ("names one or more of each union, reads any"). Other choices: only `i`
   means `SA(kind: 2, f: 1.5, x: 4)` is a compile error (unknown field), but
   `a == b` compiles; only `f` means `s.i` is a compile error and `==` is still
   refused.
2. **Whole-record `==` vs field-wise comparison.** Choice: field-wise. Writing
   `print(a == b)` with `f: f32` declared is a compile error (section 13).
3. **Building through `i` with the bit pattern of 1.5 (`i: 1069547520`) so the
   record can drop `f` and allow `==`.** Not taken: the brief asks for `f` 1.5.
   That route would compile and print the same four lines, but it no longer
   names `f`.
4. **Which union member to name in the constructor.** Choice: `f` only.
   Naming both `i` and `f`, or neither, is a compile error ("built naming
   exactly one").
5. **Field widths.** Choice: `i32` for `kind`, `i`, `x` and `f32` for `f`, as
   the header says. A field such as `kind: i64` is refused (a field is
   declared at the header's own width).
6. **Result widths.** Choice: `sa_x -> i32`. `-> i64` is allowed ("a result
   may be wider than C's") and prints the same `4`. `sa_f -> f64` would also be
   allowed as wider; it prints `1.5` either way since 1.5 is exact.
7. **Field order in the record.** Choice: header order, `kind, i, f, x`. The
   spec does not say whether order must match; another order might be refused
   by the clang check or accepted, not printing differently in either case.
8. **Record name / `tag`.** Choice: `record SA` with no `tag`, because the
   header names it by typedef and has no struct tag. `tag SA` would claim a
   `struct SA` that does not exist, likely a compile error.
9. **No `link` clause.** The functions are `static inline` in the header; I
   assumed no library is needed. Adding `link "sa"` would likely fail at link
   time.
10. **Parameters `s: SA` without `lent`.** `lent` concerns lends of `str`/
    fields, not a by-value struct; adding it to a record parameter is not
    described and may be refused.
11. **Literal `1.5` for an `f32` field.** Relies on "a literal takes the type
    its context asks for"; writing `to_f32(1.5)` would also compile, same output.

# prints

1. `12`: `make_sa` stores 12 in `i`; reading `i`, the member last written, is
   defined in C; an `i32` prints as `12`.
2. `1.5`: `t` is built with `f` set to 1.5; `sa_f` returns `s.f`, the member
   last written; an `f32` 1.5 prints with a point as `1.5`.
3. `4`: `sa_x` returns `x`, which is 4.
4. `true`: both values have `kind` 1, `i` 12, `x` 3, so all three comparisons
   hold.

# argument

A correct program is fairly likely in one turn: section 13 explicitly covers
anonymous unions, reading any member and building with exactly one. The trap
is the equality part: a model writing `print(a == b)` hits a compile error,
which is loud, not silent. The silent-wrong risk is small and lies elsewhere:
declaring `f` as `f64` or a wrong width is refused, but a model might build
through `i` with a wrong bit pattern, or compare only `kind` and `x`, both
compiling and printing plausibly while not testing what was asked. Ambiguity
about field order, `tag` for a typedef'd struct and whether `static inline`
needs no `link` could cost compile failures.

# prediction

Over 100 one-turn attempts: about 60 compile and print `12`, `1.5`, `4`,
`true` with a stated workaround for `==`; about 30 fail to compile (mostly
`a == b` on the record, some constructor or declaration errors); about 5 to
10 compile and are silently wrong (wrong bit pattern via `i`, or an equality
that skips the union).

# context

Only `brief.md`, `spec.md` and `sa.h` from this directory were read. The
harness also supplied a system prompt and an account email address, which
played no part in the program or the report.
