# Panel 042 — the shape of the integer widths

**Convened** 2026-08-12 for `M-sized-integers`, on the author's ratification of
panel 041. **Lane: full panel.** **Status: provisional — author ratification
pending on the four questions the sitting could not close.**

Panel 041 vetoed sized integers and the author overturned that veto the same day,
asking for *"un sistema di tipi fatto bene… molto simile a Rust"*. **The direction
was therefore not before this panel.** Four rulings were fixed going in — `i64`
the real name with `int` a transparent alias; both annotation and suffix, the
suffix mandatory where context does not supply the type; conversions returning
`T?`; overflow aborting at every width — and the judges were told to judge the
consequences, never the rulings.

Three of the four rulings turned out to have consequences the rulings did not
settle. That is the sitting.

---

## The verdict table

| judge | verdict | on what |
|---|---|---|
| compiler-engineer | **VETO** | Q3 — the alias is unimplementable as stated under §4.15 |
| llm-ergonomist | **VETO** | the draft as a whole; would approve the literal-syntax half alone |
| spec-warden | object | the wording, at 42 tokens more than a version saying three more rules |
| ffi-pragmatist | object | the `+` in the assertion macro, and closing Q6 with eight fixed widths |
| historian (advisory) | object | Q3 only; approves Q1, Q4, Q5 and Q2's option (c) |

Both vetoes are about **shape**, not about the widths. Neither judge disputes that
the widths should exist; both say the draft cannot be built as written.

---

## The four things that decide the milestone

### 1. The alias cannot be transparent *and* satisfy §4.15 — and the fix is not the one proposed

**The compiler-engineer's veto, verified on this tree.** There are two type
renderers. `types/render.rs:27` maps a `TyId` to a fixed string (`Ty::Int =>
"int"`). `printer/types.rs:20` does `out.push_str(src.slice(node.span))` — it
echoes **source bytes**. They meet inside one diagnostic at `types/holes.rs:124`,
so a `???` hole would print `fields of Point: x: int` above `expected: i64`.
`expects i64, found int` is constructible in one message with no new code.

**The historian corrects where the fault lies, with sources.** That message comes
from *comparing* types by spelling, not from *printing* the spelling. Two
strategies are shipped and only one worked:

- **clang prints both** — `'pid_t' (aka 'int')` — comparing on the canonical type
  and treating the sugar as display-only. Fifteen years, no filed complaint.
- **Go erased eagerly and reversed it.** `go/types` before 1.22 unaliased exactly
  the way the compiler-engineer proposed, and Go's own type-checker maintainer
  filed the bug: *"an error message related to the type `A` will not report `A`
  but `some_type`"* ([golang/go#63223](https://github.com/golang/go/issues/63223)).
  Repairing it spanned Go 1.22–1.24 plus an ecosystem audit. Go's residual leak —
  `%#v` printing `[]byte` for a `[]uint8` — was filed in 2016 and is **ten years
  unfixed, deferred to Go 2**.

So the rule is **canonicalise in the comparison, print what the author wrote**,
which also spares `types/lower.rs` the context parameter its 87 lines and 17 call
sites would otherwise need.

**But §4.15's half of the veto survives that**, and it is the open question: if
`int` and `i64` are both writable, `heroes fmt` must emit one of them. The
compiler-engineer names two options and says §4.15 admits no third. **There is a
third, and no judge proposed it**, because each was looking at one instrument:

| option | `fmt` emits | costs |
|---|---|---|
| (a) `int` → `i64` | `i64` | rewrites 545 sites in 168 files; throws away the 545-token saving the spec-warden measured |
| (b) **`i64` → `int`** | **`int`** | `i64` stays writable and readable, `fmt` canonicalises it away — **exactly the shape `M-literal-bases` shipped this morning for hex digit case** |
| (c) `i64` is not a name | `int` | the eight are `i8 i16 i32 int u8 u16 u32 u64`; uniform-looking scheme lost |

Option (b) satisfies every constraint at once and contradicts ruling 1's letter:
it makes `int` the canonical spelling, not `i64`. **This is the author's to
settle** — the panel's provisional default is (b), on the spec-warden's measured
ground that `int` is cheaper in the spec (**+21** to rewrite) *and* in every
program (**545 occurrences across 168 `.hero` files**, 3.2 per file, for zero
semantics).

### 2. Eight variants make ruling 4 silently false — one variant does not

**Measured by probe, both shapes added to the tree and `cargo check --all-targets`
run, tree restored.**

| shape | rustc errors | silent sites left |
|---|---|---|
| **A** — eight unit variants `Ty::I8 … Ty::U64` | **2** | **18** |
| **B** — one `Ty::Int(IntKind)` | **20** | **0 voluntary** |

The 18 are not stylistic. Named, with what each miscompiles:

- **`emit/ops.rs:208`** — `let integral = operands == Ty::Int;`. Under shape A a
  `u8` makes that `false`, falls to the generic path at `:292`, and emits
  `t0 = l + r;` with **no `__builtin_add_overflow`**. **Ruling 4 becomes silently
  false and the program exits 0.** Under shape B it is `error[E0308]`.
- `emit/ops.rs:264-278` — the shift guard hardcodes `> 63` and casts `(uint64_t)`;
  `u8 << 9` becomes C UB, which CLAUDE.md §7 forbids outright.
- `emit/ops.rs:237` — `INT64_MIN && INT64_C(-1)`; `i8`'s `-128 / -1` unguarded.
- `emit/externs.rs:215` `_ => None` — an `extern -> u8` gets **no `_Static_assert`
  at all**, voiding §4.19 silently.
- `emit/ctype.rs:323` `_ => Some("HeroValue")` — a `u8` local declared `HeroValue`.
- `emit/descriptors.rs:55` `_ => None` — `[u8]` has no descriptor, and
  `runtime/parts/sort.c:64` dispatches by pointer identity on `&hero_desc_int`.

Arm arithmetic: **A ≈ 112 arm-edits of which rustc demands 16; B = 20 forced, 0
voluntary, and 8 arms in exactly 4 dispatch tables** (`render.rs`, `ctype.rs`,
`ops.rs`, `externs.rs`). Shape B is adopted provisionally, and CLAUDE.md §11's
loud-fallback rule is what decides it rather than taste.

**Cost, measured by module: ~450–550 non-test compiler lines plus ~120 runtime C.**
The compiler-engineer's own words: *"It fits one person. I am not vetoing on
volume, and a veto there would be dishonest."*

### 3. Ruling 4 leaves the motivating case exactly where it was

**The llm-ergonomist's veto ground, and it is the sitting's hardest finding.**
Given the eight types and told overflow aborts at every width, it wrote FNV-1a —
the hash this project has *already* met — and the line

```
h @ h * 1099511628211u64    # compiles; aborts on byte 1 of every input
```

FNV-1a **is** multiplication modulo 2^64; the wraparound is the algorithm, not an
accident. The judge then wrote the limb workaround to size the escape hatch: ~14
lines and six masking decisions, because `lo + (cross << 32)` also aborts, so
every intermediate must be masked first — and **the workaround converts a runtime
abort into a silently wrong hash**. Neither branch is a first-try-correct program.

This matters because it is not hypothetical. `DESIGN-LOG.md`, 2026-08-12: the
mangler's typehash *was* FNV-1a and had to be redesigned to a polynomial modulo
2^31 − 1 because Heroes could not express it — the single measured instance, in
this project's whole history, of unsigned actually being needed. **Ruling 4 leaves
it unexpressible.** A third option was on the author's own menu and was not taken:
abort everywhere *plus* explicit `wrapping_mul`/`wrapping_add`.

### 4. What the FFI actually needs is not eight fixed widths

**The ffi-pragmatist measured the corpus rather than arguing about it.** A clang
AST walk over `sqlite3.h`, `curl.h` and `raylib.h`: **1151 function declarations,
and zero returning an 8- or 16-bit scalar.** `int` 320, enums 39, and the 21 that
actually hurt are `ssize_t` 8, `time_t` 7, `unsigned long` 4, `size_t` 1, `off_t`
1 — every one of them **C-width, not Heroes-width**.

The mapping table compiles clean (8 positive rows, 9 negative, against real
headers, under §7's full flag set), and the width companion assertion is
load-bearing: without it **i386 and wasm32 accept a 32-bit `size_t` as `u64`**.
But those are precisely the two rows that fire on ILP32 — so `u64` binds `size_t`
only under an LP64 premise **the `.hero` source cannot write**, Heroes having no
conditional compilation. `c_size_t` — one association, no width claim, no
companion — is clean on all five targets, and clean as an out-parameter where
`u64` produces `incompatible pointer types` (on Darwin `size_t` is `unsigned long`
and `uint64_t` is `unsigned long long`: different types, same width).

Part 7 item 10, as amended by panel 041 this morning, already says this — *"a
Heroes-width `u64` wearing a C-width name is not"* what §4.19 asked for — and
every compile run today **confirms** the sentence rather than overturning it.

**Panel 041's finding 1 is overturned, conditionally.** `SIZE_MAX` and
`CURLAUTH_ANY` are named **and read correctly** under `u64` —
`18446744073709551615` and `18446744073709551599`, compiled and run. But through
the runtime as it stands they read `-1` and `-17`, because `hero_print_int` takes
`int64_t` and `print` is monomorphic (panel 006). **`u64` needs
`hero_print_uint`/`hero_uint_to_str`, and that is `HERO_RUNTIME_ABI` 10 → 11.**

---

## A live defect in the shipped compiler, found by the sitting and reproduced twice

`emit/externs.rs:130` writes `_Generic(+(c), …)`. **`+` on a pointer is a hard
clang error**, so an `extern` whose header returns a pointer and whose Heroes
declaration says otherwise never produces the marker string `emit/ffi.rs` looks
for — and CLAUDE.md §7's named exception cannot fire:

```
$ heroes run ptrbug.hero          # extern getenv(name: cstr) -> int
internal error: compiling the generated C failed:
  error: invalid argument type 'char *' to unary expression
```

`getenv` returning `char *` declared as `int` is an ordinary author mistake, and
the compiler answers ***the compiler is wrong***. It should be exit 1 and an
`ffi_return_type` on the `.hero` line — which is exactly what panel 036 built the
mechanism to do.

**And the premise that put the `+` there is false today, verified independently
in this sitting.** `emit/externs.rs:117-123` records that without `+` the macro
*"refused every enum-returning C function in existence"*. Compiled just now on
Apple clang 21, against a synthetic enum **and against the real `CURLcode`**:

```
enum with +   : 1        CURLcode without + : 1
enum without +: 1
```

The repair at M-ffi-ladder was two changes made together — the `+`, and widening
the accepted set to include `unsigned int` and its narrower siblings. **The second
alone was sufficient**, because `_Generic` on an enum selects its compatible
integer type, which the widened set now contains. The `+` has been carrying a
justification for work it does not do, and breaking pointer returns while it does.

This is the **third** premise-written-as-justification the sitting found, after
`HERO_RET_INT`'s *"exactly what fits in Heroes' `int64_t`"* (dies the moment a
second width exists) and `printer/types.rs:3`'s *"there is exactly one correct
spelling of every type, so this renderer has no options"* (dies with ruling 1).
CLAUDE.md §11 owes each of them a falsifiable claim and a test that fires when it
dies; the sitting produced the test for the third one and it is `p42-e2-enum.c`.

**Dropping `+` fixes the shipped defect and is what the narrow widths need. Both
point the same way** — which is the only reason this is recorded as a repair
rather than as a second question.

---

## Disagreements, stated plainly

**`s[i]` splits the panel three ways and nobody is obviously right.** The
llm-ergonomist makes it a veto ingredient: with `s[i]` still `int` and character
literals still `int`, **`[u8]` has no producer in the language** and `b == 'a'`
for a `u8` becomes a type error — which is lexer code, the closure list's own
shape. The historian approves `s[i] -> u8` on Go's precedent, unchanged since Go 1
with no sourced regret, Java's regret being semantic rather than about width. The
compiler-engineer says keep `int`, because §4.3:864-868 already decided it when it
deleted `byte`, and reopening it costs a spec clause for nothing. All three are
right about different things, and the deciding fact is the ergonomist's: a byte
array the language cannot construct is a type nobody can use.

**The historian corrects the proposal's own framing of Q4.** *"Rust does the
latter"* understates it — Rust, Swift, Zig, Nim, Kotlin and Go all let a literal
take its neighbour's type, 6-for-1. The single counter-example is **Java, and it
is a *half* adoption**: declaration adopts (`byte b = 42;`) and arithmetic does not
(`b = b + 1;` is *"possible lossy conversion"*). For a language whose thesis is
that every plausible mistake is a compile error, the Java shape is **worse than
either pure choice**, because `b: u8 @ 255` would compile and `b @ b + 1` would
not, for a reason the diagnostic must explain in terms of a promotion lattice §4.3
says does not exist.

**And Q4 is far cheaper than the proposal assumed.** The compiler-engineer found
the proposal's framing backwards: there is *no* `f64` precedent — `exprs.rs:50` is
`ExprKind::Float => f64()`, unconditional, and `1.5 + x` works because there is one
float type, not because machinery ran. The real precedent is `contextual()` at
`exprs.rs:196-203`, which already implements *"one side has no type of its own, so
it takes the other's"* for `.case`, `???` and empty literals. Cost: **one arm in
`contextual()` plus ~15 lines in `expect.rs`**, with `stmts.rs:152` already routing
assignment through `expect::check`. Residual named: `b @ 1 + 2` still fails,
because `Binary` is not a ⇐ form; another ~15 lines. **Total ~30.**

**Ruling 3 is a genuine departure and should be recorded as one.** Every language
with a fallible integer conversion **kept a non-fallible one**, 4-for-4, and none
changed the incumbent's return type: Swift added a *label* (`exactly:`, SE-0080,
revised during review from throwing to failable), Rust added a *name* (`try_into`,
RFC 1542 in 2016, stable 1.34 in 2019, three years, `as` untouched). Heroes
shipping only the fallible form is defensible on §1 grounds and unprecedented in
the sample. Both the compiler-engineer and the historian land on **option (c),
distinct names**, and against (b) — making `to_int` fallible has *zero* precedent
and breaks every program that uses it.

**Rust ran ruling 2's opposite experiment and reverted it.** RFC 0212 restored
integer inference fallback, recording both that removing it *"has not exposed a
single bug"* and that it was *"quite annoying"* — an argument for the default, not
against it. The proposal's reading of ruling 2 turns out to be **Kotlin's rule
exactly**, arrived at independently.

**And one warning nobody had.** Ada 83 → 95 is the closest precedent for one
integer type becoming several, and what broke existing code was not the new types:
it was **literal ambiguity in range and iteration constructs** once a second type
could accept the same literal — `for Char in 'A' .. 'Z'` became illegal once
`Wide_Character` existed, and nobody predicted it. `range` gets audited first.

---

## Provisional resolution

Work proceeds on these; the four in bold are the author's.

1. **Shape B** — one `Ty::Int(IntKind)` variant. Adopted, on the measured 18
   silent sites and on `emit/ops.rs:208` making ruling 4 silently false.
2. **Q2 takes option (c)**, distinct names: `to_int` and `to_f64` keep aborting,
   the widths get their own fallible names. Two judges, and 4-for-4 precedent.
3. **Q4 takes full adoption** — a literal takes its neighbour's type. 6-for-1, and
   Java's half-adoption is the shape to refuse. ~30 lines.
4. **Q1 as proposed**: `i64` default, suffix names otherwise, forced only where
   the literal exceeds the contextual type. Kotlin's rule. **`range` and every
   iteration position audited first**, per Ada.
5. **Alias handling: canonicalise in the comparison, print what the author wrote.**
   Not erasure at `lower.rs` — that is Go's reversed design.
6. **The `+` comes out of the assertion macros**, as a defect repair with its own
   commit, its own golden, and `p42-e2-enum.c` as the test that fires if the
   enum premise ever comes back.
7. **`c_size_t`/`c_ssize_t` are scheduled as a named successor**, not declared
   closed by the eight — Part 7 item 10's own sentence survived every compile.
8. **AUTHOR: which way does `fmt` canonicalise, `int` or `i64`?** Provisional (b),
   `i64 → int`, on 545 corpus tokens and 21 spec tokens.
9. **AUTHOR: does ruling 4 keep FNV-1a unwritable, or do the wrapping forms come
   too?** Provisional: abort-only, as ruled — which leaves the one measured case
   this project has met still unexpressible.
10. **AUTHOR: `s[i]` — `int` or `u8`?** Provisional `int`, which leaves `[u8]`
    with no producer.
11. **AUTHOR: `HERO_RUNTIME_ABI` 10 → 11 for `hero_print_uint`?** Without it a
    `u64` prints `-1`. Provisional: yes, or `u64` is refused at `print` until it is.

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | If shape A ships, `tests/golden/` holds **zero** cases proving `u8` arithmetic aborts and a hand-written `b: u8 @ 255` / `b @ b + 1` prints `0` at exit 0. Concretely: `cargo check` with `Ty` reverted to shape B must produce **≥18** errors; **<5** means B was adopted and this is void | M-sized-integers close |
| 2 | compiler-engineer | `extern function strlen(s: cstr) -> u64` is still a diagnostic or an unasserted binding, so Part 7 item 10's named falsifier stays unfalsified and the row needs a **second** amendment for `c_int`/`size_t`/`const` | M-selfhost-probe |
| 3 | llm-ergonomist | On byte/bit tasks, **≥50%** of generated programs apply a bitwise or arithmetic operator to a non-`int` integer operand — unlicensed by the Operators block as drafted. Under today's spec that count is **0%**, structurally. Corroborating: first-try rate **≥15 points below** today's, and **≥70%** of FNV attempts type-check and abort at runtime versus ~0% today | first metric-2 run |
| 4 | spec-warden | Integer-width mismatch diagnostics are **≤5%** of first-try failures. Above **15%** the widths are a net §1.2 loss and the context-typed-literal rule is the suspect | first metric-2 run |
| 5 | spec-warden | **≥80%** of integer annotations in non-FFI generated programs still spell `int`. Below that, the alias's measured saving is fictional and the alias should be deleted rather than kept | first metric-2 run |
| 6 | ffi-pragmatist | A golden printing `extern constant SIZE_MAX: u64` shows `18446744073709551615` **only if** the ABI stamp goes 10 → 11 with a `u64` print pair. Closing at 10 with any `u64` reaching `print` makes it print `-1` — measured today | M-sized-integers close |
| 7 | ffi-pragmatist | `extern function strlen(s: cstr) -> u64` needs no shim on arm64-darwin, and the identical source under `-target i386-linux-gnu` raises exactly one `ffi_return_type` — **exit 1, not the exit-2 internal error `getenv` produces today** | M-sized-integers close |
| 8 | historian | If `int` is erased at or before `types/lower.rs`, at least one golden's `.hero` writes `int` while its `.expected` prints `i64` for the same span. Count **>0** falsifies erasure-as-implemented; **0** for two milestones and the Q3 objection is withdrawn | M-sized-integers close |

## Method note

Full panel. The llm-ergonomist received only two spec variants and never the
repository; it produced the FNV finding and the `[u8]`-has-no-producer finding,
neither of which anyone with the tree in front of them raised. The
compiler-engineer ran both representations through `cargo check` and restored the
tree. The ffi-pragmatist compiled ten C files across five targets and reproduced
the live defect in the shipped compiler. The historian returned **three
corrections to the proposal's own text** and dropped what it could not source.

**Both vetoes were about the shape and neither was about the widths.** That is
worth stating, because the author's ratification of panel 041 is what put the
widths beyond this sitting's reach, and a reader meeting two vetoes in the table
could mistake this for the panel refusing again.


---

## The author's verdict, 2026-08-12 — all four, and one of them goes further than the panel's menu

1. **`int` is deleted.** *"i64 sempre e mai int, togli int."* Not option (a), (b)
   or (c): the alias does not exist. Eight types, one spelling each —
   `i8 i16 i32 i64 u8 u16 u32 u64`. **This dissolves the compiler-engineer's Q3
   veto rather than answering it**: with no alias there is no two-renderer
   problem, no §4.15 question about which spelling `fmt` emits, and
   `types/lower.rs` keeps its 87 lines and its 17 context-free call sites. The
   historian's "canonicalise in the comparison" and Go's #63223 both become moot
   — there is nothing to canonicalise.

   The reason given is the thesis, not a concession: *"non mi importa se `i64`
   sembra strano perché tanto lo useranno gli LLM."* `int` is a word with forty
   years of conflicting widths behind it — 16 bits in C on one platform, 32 on
   another, arbitrary precision in Python — and `i64` is ambiguous to no reader.
   §1.1 makes comprehension the objective and the objective is measured, so the
   spec-warden's **545 `int` occurrences across 168 `.hero` files** are a
   migration cost, not a counter-argument: they are 545 tokens that currently
   carry a number nobody can read off the name.

2. **The wrapping forms come too.** So FNV-1a becomes writable and the
   llm-ergonomist's veto ground is answered rather than overruled. Overflow still
   aborts everywhere by default; `wrapping_mul` and `wrapping_add` are the
   explicit escape, which is the third option from the author's own menu of
   2026-08-12 — the one not taken then.

3. **`s[i]` yields `u8`** — *"u8, o comunque il dato preciso"*, which is the rule
   and not just the answer: **a value gets the type that describes it**, not the
   widest type that holds it. So `[u8]` gains its producer, the ergonomist's
   `b == 'a'` problem moves to character literals, and the historian's Go
   precedent (unchanged since Go 1, no sourced regret) is the one followed.
   Rust's counter-warning stands and is adopted with it: **the index parameter's
   accepted type is not narrowed**, because `as usize` is the most-complained-of
   integer decision in the historian's whole report.

4. **`HERO_RUNTIME_ABI` 10 → 11**, with `hero_print_uint`/`hero_uint_to_str`. So a
   `u64` prints `18446744073709551615` rather than `-1`, and panel 041's finding 1
   is overturned outright rather than conditionally.

**What this leaves the panel's provisional resolution.** Items 1–7 stand as
adopted: shape B, distinct conversion names, full literal adoption, Kotlin's
default rule with `range` audited first, no erasure at `lower.rs` (now moot),
the `+` removed, and `c_size_t` scheduled as a named successor. Items 8–11 are
answered above. **Prediction 5 is retired unscored** — it asked whether ≥80% of
generated annotations would still spell `int`, and `int` will not exist.
Prediction 8 is **moot** for the same reason and retired with it. Predictions 1,
2, 3, 4, 6 and 7 stand.
