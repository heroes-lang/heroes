# Panel 075 — the branch nobody took

**Convened** 2026-08-16, M-selfhost-port, from `docs/debrief/DECIDE.md:340` —
panel 069's llm-ergonomist found it **from the spec alone, before it knew the
question**, which that sitting called the strongest evidence this project's
instruments produce. Full lane, five seats.

## What was asked

With a `nan`, `<` and `>=` are **both false**, so an `if/else` takes the `else`
arm in silence. IEEE requires it and every language agrees, so the compiler is
not wrong — the specification says nothing, and the reader's model (*one of `<`
and `>=` must be true*) is one every programmer holds.

Candidates: **1** say it in the document · **2** an ordered comparison with a
`nan` **aborts** · **3** an `is_nan` built-in · **4** nothing.

## What the coordinator measured before the briefs went out

```
n < a  : false      n >= a : false      branch: else      exit 0
```

And the fact that decides the sitting's shape: **this language already aborts
when a `nan` reaches an ordering operation, in two places, with the reason
written down** — `runtime/parts/sort.c:51,57` (*"sort of an f64 array containing
nan"*) and `runtime/parts/map.c:85`, with panel 069 R4 making a float map key a
compile error. The bare `<` operator is the one place the decision was never
applied.

## The finding, and it did not need candidate 2 to be found

Measured after the ergonomist's verdict, by running it:

```
smallest([nan, 1.0, 2.0])  ->  nan
smallest([1.0, nan, 2.0])  ->  1.0
```

**The same six numbers, the same program, two answers, decided by which line the
`nan` sits on.** The ffi-pragmatist reached the same class from the other side by
compiling two De Morgan-equivalent validators — `x >= 0.0 && x <= 1.0` and
`!(x < 0.0 || x > 1.0)` — which **disagree on a `nan` at exit 0**: one rejects it,
one admits it into a 0..1 range.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | approve 1 (amended) · **approve 2** · **veto 3** · object 4 | Instrumented `emit::operator::binary` and counted the whole repository rather than a sample: **941 ordered comparisons across 267 `.hero` files, exactly 2 with a float operand**, both in one golden, both on **raylib struct fields an `extern` function returned**. Candidate 2 measured at **+23 lines in ONE file** (`emit/operator.rs` 263 → 286), `--dump-ir` **byte-identical** across all 78 run goldens — so it is sugar-side, the checker never sees it — 566/566, ABI unmoved. **Vetoes `is_nan`**: +19 lines across **four** modules, two mandatory spec-enforcement tests, and a name reserved in every scope forever, for something `x != x` already spells |
| **ffi-pragmatist** | approve 1 · **approve 2** · object 3 · object 4 · no veto | Compiled real bindings and found where the `nan` comes from: **21 of 29** probed C expressions return one, and `strtod`/`atof` **must** accept the three characters `nan` (C99 7.22.1.3p3) — *"a nan enters through data, not through an author mistake"*. Neither `raylib.h` nor `sqlite3.h` says the word once. **And §1.12 does reach, through the boundary**: `(int)nan` is `0` on arm64 and **`-2147483648` on x86_64**, both compiled here, so a library indexing a table by a bound float reads `table[-2147483648]` — UBSan flags it, ASan does not stop it, and Heroes cannot see it because the cast is in the callee. Cost at `-O2`: **5 instructions against 3** (clang lowers `x != x` to arm64's unordered flag), **zero guards emitted** across the 34-file corpus, five FFI examples green |
| **llm-ergonomist** (spec-only, blind) | approve 1 (amended) · approve 2 **bundled with 3** · approve 3 · **veto 4** | Disclosed its own contamination (a single read returned Part 2) and submitted the half that survives it: **the document admits two mutually inconsistent readings and both compile.** The spec defines `==` on `nan` and prints `!=` beside it with no separate definition, teaching that **`!=` is `!(==)`**; a reader transfers that to the ordering operators and concludes `>=` is `!(<)`. Under that faithful reading **`grade(nan)` returns `"pass"`** — a nan passes the exam. It refused to veto candidate 2 on locality and said why: *"Candidate 2 is strictly more local than candidate 4"* — today that `<` means *compare, and on one input take a branch the document never states* |
| **spec-warden** | object 1 alone · **approve 2 at +14, conditionally** · **veto 3** · object 4 | Priced everything and **corrected the brief's economy**: candidate 2 needs **two rows, not one** — the operator table and `sort`'s own — because *"an enumeration is how a spec makes a silent claim about what it left out"*, which is row `3405`'s own lesson. And the payment may **not** name `heroes mutate`: `mutate/mod.rs:56-60` decides Killed/Survived from `resolve`+`check` and **never runs the mutant**, so no operator it could grow has an arm for a runtime abort — panel 046 R1's exact failure. The instrument with an arm is the golden **run** harness, which has had one since `abort-sort-nan` |
| **historian** (advisory) | **approve 2+3** · object 1 alone | Fifteen systems, sourced. **Documentation alone has a fifteen-year failure record**: Java's javadoc has said exactly the right sentence since 1.2 and *"Comparison method violates its general contract!"* is still ubiquitous; **Python refused the fix twice** (bpo-12286 2011, bpo-36095 closed *not a bug* 2019) and `sorted` still returns garbage. **Rust 1.81 (2024-09-05) made `sort` panic** on a broken comparator and **kept it** — six real crates broke within weeks, which is bugs *found*. **And the one thing that contradicts this sitting**: not one verified system aborts on a bare relational operator by design. **Zig's #11234 proposed exactly candidate 2 and was closed**, superseded by #23173's `real` types that cannot store a `nan` at all; **Erlang has no `nan`** (`0.0/0.0` raises `badarith`, decades, no reversal); and real production geometry (`tavianator`, ray/box) *depends* on `<` being quietly false |

## The disagreements, both of them real

1. **Does §1.12 reach?** The compiler-engineer says no — §1.12 is memory safety, and a silently-taken `else` is a wrong answer, not a corruption — and warns that *"a record citing §1.12 for a non-memory-safety property will be quoted back to license something §1.12 never granted."* The ffi-pragmatist says yes **at the boundary**, and compiled the case: a bound float that a C callee casts to `int` indexes memory, and the index differs by architecture. **Both are right and the record keeps both**: §1.2 and §4.14 carry the argument inside Heroes; §1.12 carries it across §4.19's boundary, where the pragmatist measured it.
2. **`is_nan`.** Two vetoes against two approvals. The resolution below takes neither seat's answer whole.

## The resolution — provisional, author ratification pending

1. **Candidate 4 is refused** on the ergonomist's veto: *"the document's first line is 'This document is the whole language'"*, and CLAUDE.md §12 asks a refusal to name the program that would make it wrong — `smallest([nan, …])` is that program and it was run.
2. **Candidate 2 lands**: `<`, `<=`, `>`, `>=` with an `f32`/`f64` operand abort when either side is a `nan`. `==` and `!=` are **untouched**, so every `== 0.0` divide guard and every null test keeps working, and NumPy's one recorded reversal — a warning removed from `maximum`/`minimum` because nan propagation *was* the answer — does not reach it.
3. **The claim that it removes `sort.c`'s special case is struck from the record.** Three seats falsified it independently, two of them by deleting the guards and running: `sort` returned `1.0 2.0 3.0 5.0 nan 4.0` at exit 0, an arbitrary permutation. `hero_cmp_f64` is runtime C reached through a function pointer; **the emitter never writes that comparison**. The coordinator's brief asserted it as a saving and it is not one. It follows that this creates **one rule at two sites in two languages**, and the compiler-engineer's price for approving is the test that fires when that premise dies (CLAUDE.md §11).
4. **Candidate 3 is refused, and what the two approving seats asked for is delivered instead.** Both vetoes rest on the same ground — `x != x` already works, and a built-in costs four modules and a name reserved in every scope forever. Both approvals rest on the reader having **no documented way to ask**. So the spec **points at `x != x`**, at zero new names: *"and `nan` equals nothing, itself included, so `x != x` asks whether it is one."*
5. **Spec: +27 measured, 3440 → 3467**, in three places and not one — the operator table (*"a number only, and a `nan` aborts"*), `sort`'s built-in row (*"a number or `str`; a `nan` aborts"*), and the `==` clause gaining the test. The warden's two-row rule is honoured and the third is what replaces the vetoed built-in.
6. **The historian's candidate 5 — abort where the `nan` is born — is filed, not adopted.** It has the better precedent (Erlang, Zig's live proposal) and it is unpriced, it interacts with `spec:61`'s *"too large is `inf`"*, and it is a different mechanism. The coordinator measured the inconsistency that makes it live: **integer division by zero aborts here and `1.0 / 0.0` returns `inf` at exit 0**. Its own sitting.

**What a veto at ratification would compel**: 23 lines in one emitter file and three spec edits; reverting is a revert, and the `--dump-ir` diff being empty means no golden was regenerated to accommodate it.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | `heroes mutate examples/calculator` reports **rates identical to today's** — 653 mutants, 615 killed (95%), 529 permissive (82%) — because metric 3 scores what the *checker* catches and `examples/` has 0 float ordered comparisons. **If any per-operator rate moves, the core/sugar classification was wrong** | the closing milestone |
| ffi-pragmatist | **no rung of §4.19's ladder needs a shim, an `is_nan`, or a one-character source edit**: the five FFI examples emit **zero** nan guards, and SQLite step 3 cannot produce one at all (`sqlite3_column_double` returns NULL-as-0.0, measured on 3.51.0) | M-ffi-ladder |
| spec-warden | `cargo test` passes at **≥566** with **zero** `.expected` files changed other than the new abort cases, and **0** float ordered-comparison sites in the corpus rewritten to keep working | M-selfhost-probe |
| historian | the first abort that fires on a program its author believes correct **arrives through the C boundary**, not from Heroes arithmetic; and the abort fires **zero** times across the closure list to M-selfhost-fixpoint | M-ffi-ladder |
| llm-ergonomist | under the amended sentence alone, readers' *descriptions* become right (≥90%) while their **programs do not** (<25% gain) — documentation moves the description, not the code | M-program-corpus |

## Conditions on the record

- **compiler-engineer**: flips to `object` if a prototype produces a non-empty
  `--dump-ir` diff, or if `emit/operator.rs` lands above 300 lines without a
  split. Its veto on `is_nan` lifts if **a blind spec-only seat fails to reach
  `x != x`** in ≥1 of 3 tasks — *"that is the evidence I want and it does not
  exist yet"*.
- **ffi-pragmatist**: flips if a **library** is named whose documented protocol
  requires a `nan` to flow through `<`. **Vetoes the `x != x` spelling if this
  project ever adopts `-ffast-math`/`-Ofast`** — measured: under `-Ofast` the
  guard is deleted and `nan < 1.0` returns 1, while `isnan()` survives (Apple's
  `math.h:156-158` says why). Vetoes `is_nan` if it reaches the runtime as an
  entry point, which would move `HERO_RUNTIME_ABI` 14 → 15 for a two-line
  function.
- **spec-warden**: approval is void if the payment names `heroes mutate`, or if
  the sitting insists the `sort` guards come out.
- **llm-ergonomist**: downgrades candidate 2 to `object` if the amended sentence
  alone is measured to produce order-independent programs at ≥50%.
- **historian**: flips to `object` on **any** verified case of a language that
  made an ordered comparison abort by default and then removed it. It searched
  and found none; NumPy's is a warning on reductions.

## Author's verdict

*Pending.*
