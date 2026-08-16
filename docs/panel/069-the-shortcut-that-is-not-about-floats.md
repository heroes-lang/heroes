# Panel 069 — the shortcut that is not about floats

**Convened** 2026-08-16, M-selfhost-port, from `DECIDE.md`'s panel-062 item
(*"`nan == nan` is `false`, and `spec:71` says `==` is structural equality on
every value"*). Full lane, five seats.

**It was convened about floats and it is not about floats.** Every seat that
compiled said so independently, and the compiler-engineer proved it with a
program containing no float at all.

## What was asked

`spec/heroes-spec.md:70-72`: *"`==` is structural equality on every value —
ints, strings, records, variants, arrays, maps, recursively; a map's insertion
order does not affect it. A `ptr` or `cstr` compares as an address."*

`spec/heroes-spec.md:73-74`, two lines below: *"Every value behaves as an
independent copy: after `b = a`, mutating `b` never changes `a`. **No aliasing
exists anywhere.**"*

Measured by the coordinator before the briefs went out, all exit 0:

```
n = 0.0 / 0.0
a = [n]
b = a          # spec:73 calls this an independent copy
c = [n]        # built the same way, identical contents
a == b   ->  TRUE
a == c   ->  FALSE
```

Cause: `runtime/parts/array.c:112`, `if (a == b) return true;`, with the same
line at `runtime/parts/map.c:201`. Copy-on-write makes `b = a` share a pointer.

Candidates: **1** repair the spec · **2** delete the identity fast path ·
**3** refuse `==` on any float-reaching type · **4** something else.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | **veto 3** · approve 1, 2 | **It proved the defect with no float in the program.** With panel 061's partial-record hole simulated, `xs == ys` (COW-shared) returns **`true`, exit 0**, while `xs == zs` (distinct, equal contents) gives `panic: a partial record has no structural equality`, **exit 134**. *"The identity check silently defeats the loud fallback panel 061 installed on purpose."* Option 2 is **two deleted lines, zero compiler lines**, ABI unchanged at 14, **566 tests green before and after with zero goldens touched**, and the golden harness **3% faster**. Option 3 is ~230 lines across checker, descriptor pass and emitter — §1.7's three-way cost verbatim — and **19 sites across 7 files**, ten of them `examples/spreadsheet/formula.hero`'s entire test suite |
| **ffi-pragmatist** | **veto 3** · object 1, 2 · approve 4 | **Compiled the boundary and found something worse than nan.** Real libc, real `<time.h>`: two records holding `cstr`s pointing at **different bytes** compare `true`, and `[a] == [b]` compares `true` — `a.text` is `"first"`, `b.text` is `"second"`. *"`spec:73`'s 'No aliasing exists anywhere' is false at the boundary, measured."* On option 3: it breaks `examples/raylib/main.hero:143`'s `assert mid.x == 5.0`, `examples/spreadsheet/formula.hero:114`'s `if divisor == 0.0` — **a divide guard** — and SQLite's only check on a `double`; and the hand-written replacement `p.x == q.x && p.y == q.y` **compiles, is equally wrong, and its degenerate form forgets a field**, which the generated `_eq` cannot |
| **spec-warden** | approve wording **1e** at **+6** · object to any spec text for (B) · **veto 3** | **Two artifacts, two answers, and it found a second hole of the same kind.** (A) `spec:70` is wrong. (B) `spec:73` is the **compiler's** bug at **zero spec tokens**. And **function values compare by address** (`emit/structural.rs:118`, panel 029) while the spec names only `ptr` and `cstr` — so naming floats alone would have been *"the fifth repeat"*. Its wording deletes the enumeration *"ints, strings, records, variants, arrays, maps"*, **measured at 10 tokens**, which pays for 16 tokens of repair at a net **+6** — and the enumeration is *"the mechanism that manufactured both holes"* |
| **llm-ergonomist** (spec-only, blind) | object to 1 alone · approve 2 (**mandatory**) · object 3 · approve 4 | **All three programs it wrote before knowing the question contained the mistake, and two would not have been noticed.** Its verdict on the cheapest option: *"an uninformed reader writes nothing; a misinformed reader writes `assert xs == zs` and ships it"* — **option 1 alone is a net negative**. It showed `:70` and `:73` **jointly entail a false theorem** a reader can derive. It found a hole nobody had raised: with a nan, `n < a` and `n >= a` are **both false**, so `if/else` silently takes the else branch. And it noted the map-key abort **contradicts a stated return type** — `m[k]` is documented to return `V?` with `missing_key` |
| **historian** (advisory) | object to 1, 2, 3 alone · approve **4** | **Swift ships this exact bug.** COW arrays, `_buffer.identity` fast path, [SR-6181 / swift#48733](https://github.com/swiftlang/swift/issues/48733) opened **2017-10-18**, **still open** — and swift-foundation **added the same fast path to `Data` in PR #1322, merged 2025-05-29**. **No language has ever removed a container-level identity fast path**; NumPy removed a *per-element* one over 1.9→1.13, naming NaN. The two defences elsewhere are **speed** — which CLAUDE.md §13 forbids as a reason — and **termination on cyclic values**, Python's, which needs cycles. On map keys: **Rust and Zig refuse statically** (`K: Eq + Hash`; `@compileError` in `autoHash`), Go's runtime-refusal proposal [#20660](https://github.com/golang/go/issues/20660) has been **open since 2017-06-13**, and Go **added the `clear` builtin** to route around its own NaN keys |

## Where the seats disagree

**Almost nowhere, and that is itself the finding.** Five seats with five different
inputs — one that read only the spec, one that only searched the record, three
that compiled — converged on the same shape without seeing each other's work.

The one real disagreement is **how much of option 4 is owed now**:

- the ffi-pragmatist wants a **runtime reflexivity guard** as well, prototyped
  and measured green (`panic: a value compared with == that is not equal to
  itself (nan)`), and says the guard belongs **at the `==` operator site in the
  emitter, not inside the generated `_eq`** — because `runtime/parts/map.c:85`
  deliberately calls `key->eq(key, key)` *expecting* `false`, so a guard inside
  `_eq` would abort inside the very probe written to detect the condition;
- the compiler-engineer wants **option 2 alone plus the spec repair**, on §1.7's
  criterion: option 2 *removes* a special case, and every addition must earn its
  way past Principle 0;
- the ergonomist and the historian want the **map-key abort turned into a
  compile error**, which is Rust's and Zig's answer and needs no traits, because
  a map's key type is known at compile time.

## Three findings nobody asked for

1. **The `cstr` hole is bigger than the nan hole** (ffi-pragmatist). Two values
   pointing at different bytes compare `true`, and it reaches containers. It is
   `spec:72` working as documented and `spec:73` being false anyway.
2. **A dead test in the corpus** (ffi-pragmatist).
   `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero:31` writes
   `m[0.0 - 0.0] @ 2` and calls it *"the `-0.0` control"* — but `0.0 - 0.0` is
   `+0.0`, verified by the coordinator (`0.0 * (0.0 - 1.0)` is the one that
   produces `-0.0`). **The control has never controlled anything.** What it
   claimed to prove is nonetheless true: `hero_hash_f64` collapses the sign, and
   the seat measured `hash(-0.0) == hash(0.0)` at scalar, record-field and
   fixed-array level.
3. **`nan` ordering is silent** (ergonomist, from the spec alone; confirmed by
   the coordinator by running it). `n < a` **false**, `n >= a` **false**, and the
   `else` branch runs with no diagnostic.

## The §1.12 crash, filed regardless of which option wins

**The brief told every seat that no option here is a memory-safety question.
That was wrong, and the compiler-engineer said so.** `_eq`, `hero_array_eq` and
the drop chain recurse without bound over a runtime-deep
`record Node { children: [Node] }`.

Coordinator's own measurement, on **today's unmodified compiler**, the binary run
directly rather than through `heroes run`:

```
depth 150000, -O0, two distinct chains compared:   exit 139   (SIGSEGV, silent)
```

No diagnostic, no abort message, no line. **This is design.md §1.12 violated by a
program the author is entitled to write**, it predates every option here, and
**none of the four closes it**. It is queued as its own item. The fast path does
not cause it — it only hides it when the two operands happen to share a pointer,
which is the same sentence as the rest of this sitting.

## The resolution — provisional, author ratification pending

1. **Option 3 is refused on two vetoes**, both measured rather than argued: 19
   sites across 7 files including a divide guard, and a hand-written replacement
   that compiles and is equally wrong while being able to forget a field. It does
   not return without a program the closure list needs that option 2 makes wrong
   (today: **0 of 20** `selfhost/` files contain a float-reaching `==`).
2. **Option 2 lands**: `runtime/parts/array.c:112` and `runtime/parts/map.c:201`
   are deleted, each replaced by a comment stating what was removed and why the
   two defences do not apply here. Zero compiler lines, ABI stays 14.
3. **The spec is repaired at both false sentences**, in the warden's measured
   wording, and the repair **deletes the enumeration** rather than extending it —
   because the enumeration is what manufactured both holes, and a repair that
   names floats and leaves function values open would be the fifth of its kind.
   **+6 net** against a **−10 named removal**, 3399 → 3405.
4. **The float map key becomes a compile error**, not a runtime abort. Two
   precedents in this language's own machinery class, no traits required, and it
   removes Heroes' unique position as the only language that makes this a runtime
   event. Its diagnostic must answer the generic case in the same sentence — the
   ergonomist's condition, and it vetoes without it.
5. **The runtime reflexivity guard is refused today and left available**, priced
   at the ffi-pragmatist's prototype. With (2) and (4) landed, the remaining
   silent case is a bare `p == q` on a float-bearing record, which is one
   diagnostic away and is where the disagreement above actually lives.
6. **Three items are filed rather than fixed here**: the unbounded `_eq`
   recursion (§1.12, SIGSEGV, predates this sitting), the `cstr`
   different-bytes-compare-equal hole, and `nan` ordering's silent `else`.

**What a veto at ratification would compel**: reverting (2) is two lines; the
spec repair at (3) stands regardless, because both sentences are false today and
§12 does not care which option wins.

## Two process findings, and both are about the instrument

**The `spec-warden` agent definition carries a stale ceiling.**
`.claude/agents/spec-warden.md:8,9,28` says the budget is **3000 tokens since
panel 012**; design.md §1.6 says **4096 since panel 024** (author decision
2026-08-10). The seat found it, reached design.md by grep as CLAUDE.md §1
requires, and said so: *"Had I held the 3000 line I would have vetoed the
unmodified spec at 3399."* **This is worse than a wrong brief** — a brief
poisons one sitting, a wrong agent definition poisons every future one, silently.

*One correction of attribution, made in the seat's favour on the substance and
against it on the aim*: the seat filed this as *"a fourth wrong premise from the
convening seat"*. The defect is real and is repaired in the same commit as this
file. The attribution is not: this sitting's brief stated 4096, which is correct,
and the 3000 is in the seat's own definition, which no brief writes.

**`toolchain.rs:78` finds the runtime at `./runtime` relative to cwd**, and
`./build` likewise (compiler-engineer). A judge working from a copy but with cwd
in the real repository silently used the **unpatched** runtime — its first
"option 2 does not work" result was that, not a finding — and wrote cache
entries into the real `build/`. This is panel 056's `rm -rf target build` trap
wearing a second face: the copy is not enough, **the cwd must move too**. It
belongs in the next sitting's brief and in `/panel`'s own step 3. No source file
was touched; `git status --porcelain` returns 0 lines, verified by the seat twice
and by the coordinator at synthesis time.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | option 2 lands with **0 added lines under `crates/heroes/src/`**, `HERO_RUNTIME_ABI` still **14**, suite **566 green**, golden harness within **±5% of 120.82s** | M-selfhost-port close |
| compiler-engineer | if option 3 landed instead, its checker file exceeds **120 lines** and **≥15 of the 19** measured sites need rewriting | any attempt at 3 |
| ffi-pragmatist | under option 4, `examples/raylib/main.hero` needs **no shim and no edit**, `heroes test` stays green on `examples/{raylib,sqlite,spreadsheet,gallery}`, and **no `tests/golden/run/*.expected` changes** | M-selfhost-port · again at fixpoint |
| spec-warden | at the landing commit `heroes measure` reads exactly **3334 / 3405** and `SPEC_TOKENS = 3405`; deleting the two runtime lines leaves `cargo test` green with **zero** existing goldens changed | the landing commit · re-scored at M-selfhost-fixpoint |
| llm-ergonomist | on 20 tasks comparing composite values holding an `f64`, **option 1 adopted alone** produces **no fewer** confident-wrong programs than the status quo, and **≥3 of 20** carry an assertion the compiler silently contradicts on the `b = a` path | next Part 11 harness run |
| llm-ergonomist | option 2 + the sentence: silent-divergence **below 15%**, first-try compile-error rate **+≤3 points**. Option 3 additionally: silent **~0%**, compile-error rate **+20–35 points** | next Part 11 harness run |
| historian | deleting the fast path produces **no compile-time regression distinguishable from noise** in the fixpoint self-compile | M-selfhost-fixpoint |

**Scored in this sitting.** The ergonomist predicted the golden corpus contains
**zero** cases covering the defect. **Half falsified**: three goldens name `nan`
(`abort-map-key-nan`, `abort-sort-nan`,
`fixedbugs-a-map-key-that-is-not-itself`), so the value is covered and covered
deliberately. The specific half holds — **no golden covers copy-versus-rebuild**,
which is the defect this sitting was convened about. The seat could not have
known: its own rules forbid it reading the repository, and it said so.

The historian's condition 1 — *"cycles are the single fact that most changes the
answer and it is one grep away"* — was resolved by the coordinator during the
sitting and independently by the compiler-engineer. **Cycles are not
constructible**: `n.children @ push(n.children, n)` produces
`len(n.children[0].children) == 0`, and mutating the parent afterwards leaves the
child at its old value, so the child is a snapshot rather than a loop.
design.md:2855 states the principle normatively — *"if values are never aliased,
reference cycles cannot be constructed… that is not a compromise, it is the
reason the whole design is small."* **Python's termination defence therefore does
not transfer, and speed is the fast path's only remaining defence in this
language** — which is the one CLAUDE.md §13 forbids as a reason.

## Author's verdict

*Pending.*
