# Panel 186, the llm-ergonomist's second reading: the adopted sentences, read blind

Run by the coordinator 2026-10-02 from 13:57 to 13:59, on the critic's second-pass advice (`completeness-critic.md` § 10), from `<scratchpad>/186-blind-2/`, outside the repository and outside any git tree, with no `CLAUDE.md` in or above it; inputs `spec.md` (the spec at `779139d0` with `partial`'s three lines replaced by one marker, `docs/panel/186-briefs/blind-2/spec-marker.diff`), `sa.h` and `brief.md` (both in `blind-2/`). Its candidates: **A** is the critic's O_cover, **B** today's text, **C** the critic's R_build_cover, in that order in the brief. Command: `.claude/skills/panel/SKILL.md` § 2's with `--model claude-opus-5-5`, `claude` 2.1.285. `run.json`: subtype `success`, 5 turns, 120.7 s, total cost 0.381 USD (the cap 3). Its `context` section reports nothing but the harness's environment details; the reading stands. The report follows verbatim.

# report

## verdict

- A: approve
- B: veto
- C: object

## experiment

All three share the group head. `sa.h` has only `static inline` functions, so no `link`. `SA` is a typedef of an untagged struct, so the record takes the name `SA` and no `tag`.

### Under A

Every part can be expressed, but only if the record names `i` and not `f`. A record that names `f` holds a float union member, and `==` on it is a compile error. So the `SA` with `f` 1.5 is built by writing 1.5f's bit pattern into `i`.

```
extern "sa.h"
    record SA
        kind: i32
        i: i32
        x: i32
    function make_sa() -> SA
    function sa_f(s: SA) -> f32
    function sa_x(s: SA) -> i32

function main()
    m = make_sa()
    print(m.i)
    b = SA(kind: 2, i: 0x3fc0_0000, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    p = make_sa()
    q = make_sa()
    print(p == q)
```

The natural program under A has the record name both `i` and `f`, and builds with `b = SA(kind: 2, f: 1.5, x: 4)` (exactly one union member named). Its first three lines compile and are correct. `print(p == q)` is a compile error, so that version cannot express the equality.

### Under B

B says nothing about unions, so "all its fields" means `kind`, `i`, `f`, `x`. Section 9 makes every field name mandatory in a construction, so `i` and `f` must both get a value. I gave `i` the bits of 1.5f, so the bytes come out the same whichever member is written last.

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
    m = make_sa()
    print(m.i)
    b = SA(kind: 2, i: 0x3fc0_0000, f: 1.5, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    p = make_sa()
    q = make_sa()
    print(p == q)
```

### Under C

The program is the same as under A's. A record that names both `i` and `f` "cannot be built" under C. A record that names only `f` cannot read `i` and cannot be compared. So the only record that expresses every part names `i` alone, and the 1.5 again travels as bits.

```
extern "sa.h"
    record SA
        kind: i32
        i: i32
        x: i32
    function make_sa() -> SA
    function sa_f(s: SA) -> f32
    function sa_x(s: SA) -> i32

function main()
    m = make_sa()
    print(m.i)
    b = SA(kind: 2, i: 0x3fc0_0000, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    p = make_sa()
    q = make_sa()
    print(p == q)
```

Under C, the natural program (record names `i` and `f`, builds with `f: 1.5`) is a compile error at the construction. A record naming only `f` can be built with `f: 1.5`, but it cannot read `i` (closest: `print(m.f)` prints the float with 12's bits, `1.7e-44`) and it cannot be compared.

### Step 2: what the B program prints

1. `12`. `make_sa` writes `i` last, and reading `i` gives 12. Settled.
2. `1.5`. Both values in the constructor write the bytes 0x3fc00000, so the result is the same whichever the compiler writes last. Settled for this program only. With the usual `i: 0, f: 1.5`, the spec does not settle it: `1.5` if `f` is written last, `0.0` if `i` is.
3. `4`. Settled.
4. `true`. Field-by-field equality on identical bytes. `f` holds a denormal (12's bits), which is not `nan`, so it equals itself even if denormals are flushed to zero. Settled.

## choice_points

1. **Record name / `tag`.** I chose `record SA` with no tag. Writing a `tag` is refused: the header has no struct tag. All candidates.
2. **Which union members the record names.**
   - A, naming `i` and `f`: compiles. Equality is a compile error.
   - A, naming `f` only: cannot read `i`, and equality is a compile error.
   - B: naming only some fields needs `partial`. Then equality is a compile error, and reading `i` is too if `i` is left out.
   - C, naming both: the construction is a compile error.
   - C, naming `f` only: as for A naming `f` only.
3. **Whether a record naming one member of a union needs `partial` (A, C).** I read "names one or more of each union" as meeting the "all its fields" rule, so no `partial`. If `partial` is required instead, `p == q` is a compile error (loud).
4. **How the 1.5 is built.** I chose the bits in `i`.
   - A: `f: 1.5` with a record naming both is correct, but it gives up equality.
   - B: `i: 0, f: 1.5` compiles and prints `1.5` or `0.0` depending on the unspecified write order. Silent if `0.0`.
   - C: there is no other choice that also keeps the read of `i`.
5. **What `i` gets under B.** I chose 1.5f's bits. Any other value compiles, and line 2 then depends on write order (see 4).
6. **Order of `i` and `f` in the record and at the call site (B).** I followed the header. The spec does not say whether the compiler writes members in declaration order or call-site order. With my bits it makes no difference. With different values, either order compiles and line 2 changes silently.
7. **Result widths.** I chose `f32` and `i32`. `-> f64` or `-> i64` is allowed ("a result may be wider") and prints the same, since 1.5 is exact.
8. **Literal form.** `0x3fc0_0000` or `1069547520`. Both fit `i32` and mean the same.
9. **Equality.** The spec offers only structural `==`. Under A and C, whether it compiles depends on the record's field list. Under B, it depends on `partial`.

## argument

B never says what writing two members of a C union means. Every B program that builds an `SA` must give both `i` and `f`, so its output depends on a write order the spec does not specify. A model choosing `i: 0` gets `1.5` or `0.0` silently. That is a meaning that cannot be told from the line: veto.

A settles the meaning and makes misuse loud: a construction names exactly one member, and comparison over a float member is refused.

C settles the meaning too, but it forbids building any record that names two members. Reading `i` and building with `f` then needs a bit-pattern trick, which costs one-turn success and gains nothing over A.

Under A and C, whether `==` compiles depends on the record declaration's field list (compiling, non-local).

## prediction

Over 20 one-turn attempts by a current frontier model on this task:

- **B:** at least 70% build with an `i` value that is not 1.5f's bits. Under a compiler that writes members in call-site order, at least 30% of all attempts print a wrong second line with no error.
- **A:** at most 5% silent wrong. At most 25% full success; most stop at a compile error on `==`.
- **C:** at most 5% silent wrong. At most 15% full success; most hit a compile error on the construction or the equality.

## condition

I would move B to object if the spec elsewhere fixed the write order for a construction, for example "fields are written in declaration order". I would move it to approve if, in addition, attempts showed fewer than 5% silent wrong programs.

I would move C to approve, and A down to object, if A's "built naming exactly one" produced more silent errors in practice than C's outright refusal. One example: models reading another member of a value built through one, and expecting the value they wrote.

I would move A to object if models frequently failed to compile because they did not know whether a record naming one member needs `partial`.

## context

Only `brief.md`, `spec.md` and `sa.h` from this directory were read. The harness also supplied general environment details (working directory, platform, date) and an account email, none of which bear on the judgement. No other files or tools were used.
