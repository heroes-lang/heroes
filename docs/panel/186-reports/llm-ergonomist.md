# Panel 186, the llm-ergonomist's reading, run as a fresh blind session

Run by the coordinator 2026-10-02 from 11:09 to 11:11, from `<scratchpad>/186-llm-ergonomist/`, outside the repository and outside any git tree, with no `CLAUDE.md` in or above it; its inputs `spec.md` (the spec at `779139d0` with one marker, `docs/panel/186-briefs/blind/spec-marker.diff`), `sa.h` and `brief.md` (both in `docs/panel/186-briefs/blind/`). Command: `.claude/skills/panel/SKILL.md` § 2's, with `--model claude-opus-5-5`, `claude` 2.1.285. `run.json`: subtype `success`, 6 turns, 150.9 s, total cost 0.479 USD (the cap 3). Its `context` section reports nothing but the harness's system instructions; the reading stands. The report follows verbatim.

# verdict

- **L: veto.** What line 2 prints depends on the order the constructor's two overlapping writes reach C. The spec never states that order, and the line, the signature and the header do not settle it. This is a dependence of **meaning**.
- **M: approve.** Every line's meaning follows from the line and the header. Whether `==` compiles depends on the header having a union, which is the named header, so this is allowed.
- **N: object.** Whether `s.i` or `s.f` compiles depends on which member the group's `record` chose, not on the header. This is a dependence of **compiling**, not of meaning. N also forces a bit-pattern workaround or a wrong member read.

# experiment

**Program under L** (no sentence). Section 13 says a group's record is "the header's struct: all its fields". Section 9 has no default values, so a construction names every field. In C11, `i` and `f` are members of `SA`.

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
    print(make_sa().i)
    b = SA(kind: 2, i: 0, f: 1.5, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    print(make_sa() == make_sa())
```

Every part can be expressed under L.

**What it prints under L** (step 2), reading only the spec and C's union rule:

1. `12`. Settled: the record is the header's struct, `make_sa` wrote `i` last, and reading `i` reads those bytes.
2. NOT settled. The constructor writes both `i` and `f`, which share 4 bytes. The spec says nothing about the order the fields reach C: argument order, declaration order, or something else.
   - If `f` is written last, it prints `1.5`.
   - If `i` is written last, it prints the bytes of `int32_t 0` read as a `float`, which is `0.0`.
   - A third reading is that the compiler refuses two fields at one offset. Nothing states that either; under it, the program does not compile.
3. `4`. Settled: `x` does not overlap the union.
4. `true`. Structural `==` compares `kind`, `i`, `f` and `x`. Both values hold kind 1, i 12 and x 3. In both, `f` is the same bit pattern: 12 read as a float is the denormal 12 * 2^-149, about 1.7e-44, not a nan. So every field is equal. The spec settles this line only because this bit pattern happens not to be a nan. If `i`'s bits were a nan, the same line would print `false` for two identical structs.

**Program under M**

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
    print(make_sa().i)
    b = SA(kind: 2, f: 1.5, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    p = make_sa()
    q = make_sa()
    print(p.kind == q.kind && p.i == q.i && p.x == q.x)
```

M cannot express `make_sa() == make_sa()`, because comparing an `SA` is a compile error. The closest program compares the fields one by one, using one union member. It prints `12`, `1.5`, `4`, `true`.

**Program under N** (the record names exactly one of `i` and `f`)

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
    print(make_sa().i)
    # 0x3fc0_0000 is the bit pattern of the float 1.5
    b = SA(kind: 2, i: 0x3fc0_0000, x: 4)
    print(sa_f(b))
    print(sa_x(b))
    p = make_sa()
    q = make_sa()
    print(p.kind == q.kind && p.i == q.i && p.x == q.x)
```

Under N, one record cannot both read `i` and build with `f`: the program picks one member for the whole file. This program names `i` and gets 1.5 by writing that float's bit pattern into `i`. So "builds an SA with f 1.5" is not expressed as written. N also cannot express `make_sa() == make_sa()` (compile error); the closest program is the field-wise comparison. It prints `12`, `1.5`, `4`, `true`.

# choice_points

**L**
- **Which union members the record lists.** Chosen: both `i` and `f`.
  - Listing only one member without `partial` is presumably refused by clang's field check (compile error).
  - `record SA partial` with `kind, i, x` cannot build with `f`.
  - `record SA partial` with `kind, f, x` has to read `make_sa().f`, which compiles and silently prints `1.7e-44`, not `12`.
  - Either partial form makes `==` a compile error.
- **The value given to `i` in the constructor.** Chosen: `0`. The spec does not say which write wins, so the program compiles and line 2 prints `1.5` or `0.0`. Any other value of `i` gives that value's bits read as a float. Writing `i: 0x3fc0_0000` would print `1.5` under either order, but nothing in the spec suggests doing so.
- **Argument order, `i:` before `f:` or after.** Chosen: declaration order. If writes followed argument order, swapping the two flips line 2 between `1.5` and the reinterpreted `i`.
- **Field order in the record.** Chosen: header order. The spec does not say whether another order is refused, or whether it changes the write order.
- **`sa_x` result `i32` or `i64`** (a result may be wider). Chosen: `i32`. `i64` compiles and prints the same.
- **No `link`**, since the functions are `static inline`. Adding `link "sa"` would fail at link time, so no binary.
- **Parameter name `s`** in the extern functions. Free: any name compiles the same.

**M**
- **Which member to name in the constructor.** Chosen: `f: 1.5`.
  - Naming `i: n` instead compiles, and `sa_f` prints the float read from `n`'s bits (silently wrong).
  - Naming both, or neither, is a compile error.
- **Listing only one member in the record:** compile error, because under M a record names every member.
- **Equality.** `make_sa() == make_sa()` is a compile error. Chosen: field-wise comparison using `i`.
  - Comparing `f` instead prints `true` here, but `false` for a nan bit pattern.
  - Leaving out the union member prints `true` here, but is not the struct's equality in general.
- **`sa_x` result width and `link`:** as under L.

**N**
- **Which member the record names.** Chosen: `i`. If the record names `f` instead:
  - `print(make_sa().i)` is a compile error.
  - The natural fix, `print(make_sa().f)`, compiles and silently prints `1.7e-44`.
  - The build with `f: 1.5` then works.
- **How to build with 1.5 while the record names `i`.** Chosen: the bit pattern `0x3fc0_0000`.
  - Writing `i: 1` or any other guessed value compiles, and `sa_f` prints a denormal or some other wrong float (silently wrong).
  - Writing `f: 1.5` is a compile error.
- **Two modules**, each with its own `extern "sa.h"` and its own `record SA` (one naming `i`, one naming `f`), would express both parts as written. The spec neither allows nor forbids two records binding one struct in different modules; if refused, it is a compile error. Within one file, two `record SA` declarations bind one name twice, which is a compile error.
- **Equality:** as under M.
- **`sa_x` result width and `link`:** as under L.

# argument

Under L, `SA(kind: 2, i: 0, f: 1.5, x: 4)` has to name both overlapping members, and the spec never says which write survives. So line 2's output cannot be told from the line, the signature or the header. That is a dependence of meaning, and L is where a silently wrong program is likeliest. M settles construction by requiring exactly one member, so a wrong naming is a compile error. It makes `==` an explicit compile error, which a model answers with a field-wise comparison. Its one silent risk is putting a number in `i`. N makes whether `s.i` compiles depend on the group's record declaration, not the header. It forces either bit-pattern tricks or a wrong member read that compiles and prints 1.7e-44.

# prediction

Setup: 100 one-turn attempts by a current frontier model given `spec.md`, `sa.h` and this task. Success means the program compiles and prints `12`, `1.5`, `4` and `true`, with the equality written however the candidate allows.

- **L:** at least 30% silently wrong, mostly line 2 printing the reinterpreted `i`. How many depends on the compiler's unstated write order: at most 70% success if `f` happens to win, and at most 10% that get `1.5` under every order.
- **M:** at least 75% one-turn success; at most 5% silently wrong; the rest compile errors (most often a direct `==`).
- **N:** at most 30% one-turn success; 10 to 25% silently wrong (reading `.f` from `make_sa`, or a guessed `i`); the rest compile errors.

# condition

- **L:** I would move from veto to object if the spec fixed the write order for overlapping fields (for example, "the last named argument wins") or refused naming two members of one union. Either change makes the meaning visible on the line.
- **M:** I would object if trials showed more than 10% silently wrong programs, for example from building through `i` with a decimal value.
- **N:** I would move to approve only if the header alone settled which member a record names, for example "the first member". In that case compiling would no longer depend on the record declaration. Trials would also need to show one-turn success near M's.

# context

Besides `brief.md`, `spec.md` and `sa.h`, my context held only the harness's system instructions: environment details (working directory, platform, date, model identity) and an account email address supplied as user context. I did not use any of these in the analysis. I read no other file and used no tool except reading and writing files in this directory.
