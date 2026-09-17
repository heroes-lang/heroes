# Panel 160 — spec-warden report

Sitting of 2026-09-17, at M-check-completeness, on defect 050. Every number
below was produced by a command run this session in a scratchpad copy of the
tree (`cp -r`, `rm -rf target build`, seed built with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0). Drafts
were applied to the copy's `spec/heroes-spec.md`, priced with
`./heroes measure spec/heroes-spec.md` (vendored) AND
`./heroes measure spec/heroes-spec.md --refresh` (real, `claude-opus-5`), and
reverted with `git checkout -- spec/heroes-spec.md` (exit 0 every time). The
copy's spec is clean at the end (`git status --short spec/` empty).

## Summary

- **verdict**: **approve**, for one package and no other: **option E written
  as a clause in § 6's `.is_err()` row (E3), paid for by two named removals
  (Ra + Re)**. Not a veto: no draft comes within 2160 real tokens of the
  ceiling. **Object** to the two wordings in the briefs that write `i64??`
  (D1a, D2a), at any price. **Object** to landing the peel sentence alone
  (option D).
- **section**: design.md §1.6 (ceiling **10240**, `claude-opus-5`,
  `POST /v1/messages/count_tokens`, grepped at design.md:255-256 this
  sitting; unconditional payment rule, design.md:305-314), §1.2 (cost
  formula), §1.0 (burden of proof); CLAUDE.md § 12 (spec beats compiler).
- **spec_token_delta**: base measured 2026-09-17 in the copy: `claude-legacy`
  **5879** · `cl100k_base` **6004** · real **7998** · digest
  `7a8fb4400c7ed444` — identical to the brief's base. Recommended package
  E3+Ra+Re lands at **5993 vendored / 7986 real, net −11 / −12**. Full table
  below, every draft on both instruments.
- **removal**: **Re**, § 8's *`break` and `continue` exist.* (**−10 vendored /
  −12 real**), stated in the `Statement` production (§ 5), the `Inline`
  production (§ 8) and § 8's jump bullet; and **Ra**, § 10's *and `for k in
  sort(keys(m))` walks them in order* (**−18 / −21**), which is the compiled
  fence nine lines above plus § 11's `sort` direction (panel 159). Either one
  alone pays; both together make the document smaller than it was.
- **needed_for_self_hosting**: **no**. `selfhost/` has 0 generic declarations,
  0 calls of the library `find`, 0 `{K: V?}` types outside one comment, and
  its 12 reader sites on `[i64?]` elements are single-level because an array
  index yields `T` (probe below). Options A and E break nothing the bootstrap
  compiles, as far as grep can see; the type-aware count is UNRUN.
- **argument**: The compiler does not need this; the thesis does. Measured:
  `m["a"].is_err()` on a `{str: i64?}` holding a stored failure checks 0, runs
  0 and prints `false`; `xs.find(is_bad).is_err()` over a `[i64?]` does the
  same. A plausible program compiles and answers the wrong question, which is
  CLAUDE.md's first sentence falsified and §1.2's most expensive failure: no
  round trip, a wrong answer. Option D spends tokens and changes no behaviour,
  so it buys nothing §1.2 can count. E3 states the refusal in the one row a
  reader consults, needs no `T??` spelling, costs +17/+21 alone, is cheaper
  than A's sentence (+31/+36), and does not refuse `.must().must()`, a correct
  two-level read. Two true duplicates pay for it with change to spare.
- **prediction**: instrument `heroes measure spec/heroes-spec.md --refresh`,
  existing today, scored in the commit that lands this sitting's resolution.
  Landing E3+Ra+Re exactly as priced reads **7986 real** tokens and **5993
  cl100k / 5868 claude-legacy** tokens; E3+Ra reads **7998 / 6003**; E3+Re
  reads **8007 / 6011**. Any other number means the landed text is not the
  text priced. Second, under §1.2, instrument the `check` `run` `emission`
  `corpus` suites: E's refusal turns **0** existing programs red across
  `examples/`, `selfhost/`, `tests/golden/`, the new golden written for it
  excepted; one existing file going red falsifies it.
- **condition**: I move to **object** if a type-aware count finds one reader
  applied to a nested fallible in `selfhost/` (then the count is the price and
  must be in the ledger row), if the sitting lands any wording carrying
  `i64??`, or if any addition lands without a named removal measured in its
  commit. I move to **veto** only if `--refresh` reads above **10240**, which
  no draft here approaches (worst case 8077).

## 1. The ceiling, by grep, today

`grep -n "^### 1.6" docs/design/design.md` → line 253. Lines 255-256: *must fit
in 10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`*. The payment rule is unconditional
(design.md:311-314). `DELTA_GATE` is **50**, VENDORED
(`selfhost/measure/judged.hero:156-157`). The FFI floor is **60**, printed by
`heroes measure`.

## 2. Base, measured in the copy (2026-09-17)

```
claude-legacy 5879 · cl100k_base 6004 · real 7998 (claude-opus-5)
digest 7a8fb4400c7ed444 · ceiling 10240 · 2242 free · 2182 net of the FFI floor
```
`${#ANTHROPIC_API_KEY}` = 108 after `set -a; . ./.env; set +a`; `--refresh`
exit 0 on every call.

## 3. Every draft, both instruments

Δ against 6004 vendored (`cl100k_base`) and 7998 real. "Landed" is the
absolute count of the edited document.

| # | draft (where) | vendored Δ | real Δ | landed v / r | ratio r/v |
|---|---|---|---|---|---|
| D1a | 158's ergonomist sentence verbatim, after § 6's abort line: *`T` may itself be fallible — `m[k]` on a `{str: i64?}` is an `i64??` — and every operation below peels one.* | **+41** | **+46** | 6045 / 8044 | 1.12 |
| D1b | same rule without the spelling: *Each operation below peels one level: a `T?` whose `T` is fallible hands back that `T`.* | +27 | +33 | 6031 / 8031 | 1.22 |
| D1c | minimal: *Each operation below peels one level.* | **+9** | **+11** | 6013 / 8009 | 1.22 |
| D2a | option E, the brief's wording appended to D1a (*… `match` names both levels, and `.is_err()`, asking only the outer one, is refused on such a value.*) | **+68** | **+79** | 6072 / 8077 | 1.16 |
| D2b | option E, compact sentence, no spelling | +43 | +52 | 6047 / 8050 | 1.21 |
| E1 | option E sentence with the why | +36 | +43 | 6040 / 8041 | 1.19 |
| E2 | option E sentence, rule only: *On a `T?` whose `T` is fallible, `.is_err()` is refused; `match` names both levels.* | +30 | +36 | 6034 / 8034 | 1.20 |
| **E3** | option E in the table row: `| `.is_err()` | boolean test; refused where `T` is itself fallible: `match` names both levels |` | **+17** | **+21** | 6021 / 8019 | 1.24 |
| D3 | option A sentence: *A `T?` whose `T` is fallible is read by `match` alone; the other four operations below are refused on it.* | +31 | +36 | 6035 / 8034 | 1.16 |
| D4a | option B, § 3 row: `| `{K: V}` | map; `V` is never a `T?` |` | +11 | +11 | 6015 / 8009 | 1.00 |
| D4b | option B, § 10 clause: *…with code `missing_key`, and `V` is never itself a `T?`;* | +12 | +14 | 6016 / 8012 | 1.17 |

Removals, measured alone:

| # | removal | vendored Δ | real Δ | landed v / r | true duplicate? |
|---|---|---|---|---|---|
| **Ra** | § 10: *, and `for k in sort(keys(m))` walks them in order* | **−18** | **−21** | 5986 / 7977 | yes: the compiled fence at § 10 shows that exact line, and § 11 gives `sort`'s direction since panel 159 |
| **Re** | § 8: *`break` and `continue` exist.* | **−10** | **−12** | 5994 / 7986 | yes: `Statement` (§ 5) and `Inline` (§ 8) list both, and § 8's bullet names them as jumps |
| Rc | § 12: *; ordinary builds ignore them* | −5 | −11 | 5999 / 7987 | not offered: whether `heroes build` really ignores a `test` block is UNRUN |
| Rg | § 3: *No aliasing exists among the values this language owns.* | −11 | −15 | 5993 / 7983 | no: it names the concept the previous sentence describes and is the hinge to the `ptr` sentence — content |

Packages, measured as ONE landed text (tokenisation is not additive; these
are the numbers a landing must reproduce):

| package | landed vendored | landed real | net v / r |
|---|---|---|---|
| **E3 + Ra + Re** | **5993** | **7986** | **−11 / −12** |
| E3 + Ra | 6003 | 7998 | −1 / +0 |
| E3 + Re | 6011 | 8007 | +7 / +9 |
| E2 + Ra + Re | 6006 | 8001 | +2 / +3 |
| E2 + Ra | 6016 | 8013 | +12 / +15 |
| D3 + Ra + Re | 6007 | 8001 | +3 / +3 |
| D3 + Ra | 6017 | 8013 | +13 / +15 |
| D1c + Re | 6003 | 7997 | −1 / −1 |

The brief's *§ 7 `x != x`* clause: still content. `grep -i "is_nan\|isnan"
spec/heroes-spec.md selfhost/library_source.hero` exits 1, so `x != x` is the
only nan test the language offers and the clause is its home.

## 4. The four questions

**Q1, as measured.** Above. The vendored instrument understated every draft
(ratios 1.00 to 1.24, below panel 159's 1.60 but never below 1.00), which is
why every row carries the real.

**Q2, what it displaces.** Ra and Re, both true duplicates measured alone and
as a package. §1.6's rule is unconditional; every draft here owes one, and the
recommended package overpays so the document ends smaller.

**Q3, §1.2.** The defect is a silent wrong answer, measured twice this session
(§ 5 below). A refusal converts it into one 500–2000-token round trip with a
diagnostic that names the fix; the silence costs a debugging session. D1c
spends +11 real and leaves the silence, so it reduces the rewrite rate by
nothing countable. E3 spends +21 real for a compile error at the exact reader
that loses the level. A (D3) spends +36 and also refuses `.must().must()`,
which is a correct program — §1.2 calls a construct that raises error
probability a net loss; A does it to a correct read.

**Q4, Principle 0.** Compiler need: none (§ 6 below). Thesis: the shared
brief's measurement and mine agree that a plausible program compiles and
answers the wrong question, which is the Part-1-derived argument §1.0's second
branch accepts. E passes on that branch; D does not, because it changes no
behaviour.

## 5. Probes, run in the copy (`./heroes check` then `./heroes run`, exit codes direct)

| probe | check | run | output | what it settles |
|---|---|---|---|---|
| `xs: [i64?] @ [ok(1)]` then `print(xs[0].must() + 1)` | 0 | 0 | `2` | an array index on `[T?]` yields ONE level, so `selfhost/check/generics.hero`'s 12 `bindings[…].is_err()/.must()` sites are not nested readers |
| `m: {str: i64?}`, `m["a"] @ fail(…)`, `print(m["a"].is_err())` | 0 | 0 | `false` | defect 050 reproduced: the stored failure is invisible |
| `xs: [i64?]` with a failing element, `print(xs.find(is_bad).is_err())` | 0 | 0 | `false` | the SECOND door: `find` over `[T?]` is silent the same way, so option B (the map alone) closes half |
| `x: i64?? @ ok(ok(1))` | **1** | — | `error[nested_fallible]` | `i64??` is a spelling the compiler refuses, so D1a and D2a teach a form that does not compile |

## 6. Principle 0's corpus count, from the world

- `grep -rnE '^function [a-z_]+<' selfhost/ | grep -v library_source` → **0**.
- `grep -rn "find(" selfhost/ | grep -v library_source` → 3 hits, **0 calls of
  the library `find`**: `emit/container.hero:288` is the C string
  `hero_map_find(`, `check/builtins.hero:407,417` are `rfind`. **Brief
  correction**: the shared brief counts these as three `find(` sites.
- `{K: V?}` in `selfhost/` → **1**, `check/table.hero:67`, a comment.
- `examples/`: `{K: V?}` **0**; `[T?]` **2** hits, both comments in
  `examples/argv/main.hero` (`args_checked` used 11 times there, `find(` 0
  times); all **6** `find(` calls are in `examples/readings/main.hero` over
  `good = all_week.filter(answered)`, a `[Reading]`; the one generic returning
  a wrapped parameter, `wanted<A>(…) -> A?` (`interpreter/run/value.hero:150`),
  is called 6 times inside match arms. No grep finds a nested reader among the
  72 `].is_err(`/`].must(`/`].default(` sites. **A type-aware count is UNRUN**:
  a grep cannot see types, which is the brief's own caveat.
- `tests/golden/`: `{K: V?}` in **2** files. `no-size-through-fallible.hero`
  holds it in a comment. `a-fallible-type-is-never-written-fallible-twice.hero:27,31`
  applies `.must().must()` inside functions whose declared type is already
  refused at `nested_fallible` (lines 26, 30, `.expected` shows those three
  errors and no other), so they are not checker-visible nested readers. I
  wrote "A refuses a repository program" mid-session on that grep; it was an
  inference and is withdrawn here.

## 7. R2 and R4, from this seat

- **R2.** None of the honest wordings priced needs a written `T??`: E3 says
  *where `T` is itself fallible*. The specification does not depend on 158's
  R1 landing. The diagnostic's text is the compiler seat's; the spec's is
  spelling-free by construction, and the two brief wordings that are not
  (D1a, D2a) should not land at any price, because `heroes check` refuses the
  form they show (probe 4).
- **R4.** The document owes the refusal one clause, in the `.is_err()` row.
  It does not owe 158's peel sentence: D1c documents a behaviour the compiler
  would still let go wrong. E3 and D1c together are **UNRUN as a package**
  (estimate from the singles, +26 / +32; an estimate, not a price).
- **Option B needs a sentence**, answering the brief's fourth question: § 3's
  production `Prefix = … "{" Type ":" Type "}"` derives `{str: i64?}` and the
  type row constrains `V` nowhere, so a refusal unwritten there is a compiler
  bug under CLAUDE.md § 12, and the `grammar` suite cross-checks § 3. Priced at
  +11/+11 (row) or +12/+14 (§ 10). It buys half of what E3 buys (probe 3) for
  half the price, and contradicts § 5's *a `[T?]`, a record holding one, or a
  type parameter that arrived fallible* by singling out the map.

## 8. The ledger

- Rows today: **78** (`grep -cE "^\| *[0-9]+ \|"`), newest at line 158, panel
  159's +15 / +24.
- **My panel 158 prediction lapses, unscorable as written.** It read *with the
  resolution below, `heroes measure spec/heroes-spec.md --refresh` reads 7974
  real / 5989 vendored and digest `21a9dc541cfa2fa8` at the milestone close,
  unchanged*. Panel 159 moved the base to 7998 / 6004 / `7a8fb4400c7ed444`
  with three sentences unrelated to nesting, so the numbers cannot be scored
  against their base. Mark it `lapsed`, not falsified. The half that is still
  checkable held: `grep -n "peel\|nested\|itself fallible\|outer"
  spec/heroes-spec.md` returns one line (44, `nested brackets`, about
  f-strings) and none about fallibles, so nothing entered on this subject.
- The package landing here owes its own row: landed vendored, landed real,
  digest, the two removals measured in that commit, and the paste into
  `tests/harness/suite_spec.hero` and `selfhost/measure/pinned.hero`.

## 9. Brief corrections found this sitting

1. `find(` in `selfhost/` outside the library: **0 library calls**, not 3 sites
   (§ 6).
2. The ergonomist's *~26 tokens* for the R3 sentence: **+41 vendored / +46
   real** as landed in § 6.
3. 158's *+9 vendored* cheapest merge: reproduced (D1c, +9 / **+11 real**), so
   that one stands.

## 10. UNRUN

- A type-aware count of readers applied to a nested fallible anywhere in the
  tree (needs the checker; grep cannot see types).
- E3 + D1c as one landed text.
- Whether `heroes build` ignores a `test` block (would decide Rc).
- Any compiler-side line count for E's refusal.
