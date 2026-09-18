# Panel 163 — spec-warden

- `verdict`: **veto** on routes 1, 2 and 3 as spec amendments · **approve** route 4
- `section`: design.md §1.6 (payment rule), §1.2 (cost formula), §1.0 / CLAUDE.md § 2 (Principle 0), §1.12 + CLAUDE.md § Precedence rank 3 (robustness), Part 6 / Part 7 (where a refusal lives)
- `spec_token_delta`: **measured**, `claude-opus-5` through `POST /v1/messages/count_tokens`, 2026-09-18, in a scratchpad copy with a seed built from C. Baseline **8030** real / 6032 vendored, digest `199b5f0a66d147d1`, ceiling **10240**.
- `removal`: **nothing — and that is a problem** (for routes 1–3). Route 4 owes nothing because it adds nothing.
- `needed_for_self_hosting`: **no**
- `prediction`: below
- `condition`: below

## 1. Every draft, priced on the instrument that judges the document

Each draft was applied to `spec/heroes-spec.md` **in a scratchpad copy**, measured with
`. ./.env && ./heroes measure spec/heroes-spec.md --refresh`, and reverted. The repository
working tree was never touched. No number below is vendored and none is an estimate.

| draft | what it is | real | Δ real | digest |
|---|---|---|---|---|
| baseline | today's document | **8030** | — | `199b5f0a66d147d1` |
| **D1a** | route 1, § 13 only: *"…or with `repeat(x, n)` at its own length"* | **8048** | **+18** | `7949e8363e9b5538` |
| **D1b** | route 1, **honestly stated**: D1a **+** § 11's `repeat(s, n)`→`repeat(x, n)` **+** § 11 prose saying which type it answers **+** § 9's *no overloading* qualified | **8090** | **+60** | `4ec282cd0f4c93b4` |
| **D2** | route 2, zero default: § 13 sentence **+** § 9's *no default values* qualified | **8062** | **+32** | `2e5fa38f316f44e8` |
| **D3** | route 3, uninitialised out-parameter: § 5 sentence **+** a `Statement` production alternative | **8097** | **+67** | `2f5cb19271199682` |
| **D4** | refuse / defer | **8030** | **0** | unchanged |

**No budget veto.** The worst draft lands at 8097 against 10240, 2143 free. My veto is
Principle 0's, not the ceiling's.

### The +18 is the price of a sentence that leaves two others false

Panel 162 priced route 1 at **+18** and D1a reproduces that number exactly. **It is not the
price of the rule.** Measured this session, from the compiler rather than from the brief:

- `repeat` is `repeat(s: str, n: u64) -> str` — `selfhost/check/builtins.hero:256-267`.
  `repeat(0, 4)` is `error[bad_operand]: `repeat` takes `str` and `u64`, found `i64``, run.
- `spec § 9` line 251 says **"no overloading"**, flat.
- `spec § 11` line 293 writes the built-in as `repeat(s, n)` — `s` for `str`.

So a `repeat` that answers `i8[256]` in one context and `str` in another **is** overloading, and
route 1 does not "bend nothing in the document": it falsifies § 9 and § 11 at +18 and costs
**+60** to state truthfully. The shared brief's premise is wrong, and this is the eleventh
correction to a coordinator's brief in seven sittings. Panel 159's row is the precedent: **the
false sentence is cheaper than the true one**, and §1.6's pressure runs the wrong way exactly
where the document is least able to catch it.

## 2. What the refusal costs, RUN rather than argued

The shared brief says route 4 "owes an answer" for `uname()`. Here it is, compiled and run in the
copy on this Mac, exit 0:

```
extern "sys/utsname.h"
    record Utsname tag utsname partial
        sysname: i8[256]
    function uname(@u: Utsname) -> i32

function zero_utsname() -> Utsname
    return Utsname(sysname: [0, 0, … 256 of them …])

function main() -> ()
    u: Utsname @ zero_utsname()
    rc = uname(@u)
    assert rc == 0
    match u.sysname.validated_bytes()
        .ok v => print(v)          # prints Darwin
        .err _ => print("not text")
```

Three measurements the sitting did not have:

- **A `constant` cannot hold it** — `error[constant_body]: a body may not contain a call`. The
  diagnostic names the repair itself: *"for a value that is computed, write a `function`"*. So the
  once-per-platform value is a **function**, not a constant, and it works today.
- **`partial` DOES help, just not where the brief looked.** It does not relax the length check on a
  *declared* field — true. It lets the program **declare only the fields it reads**, so the long
  literal is written once instead of five times, and the record keeps C's size so `uname` fills all
  of it. Whole program: **903 tokens** vendored (*lower bound*, `--refresh` refuses non-spec paths),
  against **4052** for the five-field version. The brief's "`partial` is not an escape" is true of
  one sentence and false of the wall.
- **The mistake is caught, exactly.** 255 zeros instead of 256 is
  `error[fixed_array_length]: `i8[256]` holds exactly 256, and this literal has 255`. A plausible
  mistake is a compile error with the count in it. That is the thesis working, not failing.

## 3. §1.2, with the break-even measured

`real cost = program tokens × (1 + rewrite rate)`. The same program written with `repeat(0, 256)`
measures **142** vendored (*lower bound*) against **903**: a saving of **761** program tokens,
**in a program that binds a long-array struct**.

Route 1 charges **+60 real on every generation**, because the spec is the prompt. The saving lands
only on programs that bind such a struct. Break-even on tokens alone, before any rewrite term:
**761 / 60 ≈ 1 program in 13** must bind a fixed array longer than 8. Measured share today:

```
grep -rnE "(i8|u8|i16|u16|i32|u32|i64|u64|f32|f64)\[[0-9]{2,}\]" selfhost examples tests --include='*.hero'
→ 2 hits, both inside COMMENTS. The longest fixed array DECLARED anywhere in the tree is i8[8].
```

Zero of thirteen, not one. Route 1 is a net loss under §1.2 by a factor the corpus sets, and the
rewrite term does not rescue it: the literal's failure mode is a compile error that names the
number, so it converges, while route 1 **adds** a plausible mistake — `xs = repeat(0, 4)` now
reads as building an array and answers whatever the context happens to ask for.

## 4. Principle 0, and which rule each route spends

**The compiler does not need this**: it self-hosts, and no fixed array longer than `i8[8]` is
declared anywhere in the tree (measured above). So every route must enter on a measured Part 11
effect. **None was offered by any of them.** The burden of proof is unmet and that is my veto.

Where each rule would live, and what it spends:

- **Route 1** — two homes, which is the failure panel 162's critic caught. The built-in's type is
  § 11's, the fixed-array construction is § 13's, and the exemption from *no overloading* is § 9's.
  D1b touches all three. `.claude/rules/spec-shape.md`: *every rule has exactly one home*.
- **Route 2** — spends design.md §4.9 and `spec § 9`'s *no default values*, which is one rule with
  two homes already. +32 in the spec, plus a Part 4 amendment in design.md that this budget does not
  see. Cheapest of the three and the only memory-safe one. If the panel lands anything, land this.
- **Route 3** — **refused on robustness, which outranks tokens** (CLAUDE.md § Precedence rank 3,
  design.md §1.12). `spec § 5`'s four words are what make *read of an uninitialised value* a
  non-class here. The tree already records what the exception costs:
  `selfhost/ir/inout.hero:92-105` initialises a C out-cell to null **"and that is robustness rather
  than tidiness"**, because a C function may never write it — and, measured 2026-09-07, with the
  store deleted **the program prints the same four lines at `-O0` and under
  `-fsanitize=address,undefined`**, because the indeterminate local happened to be zero. Route 3
  generalises exactly that hazard to any `@` declaration, and no sanitizer on this Mac sees it.
  `selfhost/ir/values.hero:1-12` says what the checker has instead of definite assignment: a
  dominance invariant in the **IR**, whose whole purpose is that a missed one is reported as a
  compiler bug rather than in clang's voice inside the author's `.hero` file. Route 3 asks the
  checker for an analysis it does not have, to buy the most expensive of the three drafts.

## 5. Two amendments to one milestone — does it change §1.6's payment rule?

**No, and that cuts against the proposal rather than for it.** §1.6 attaches payment to the
addition, not to the milestone: there is no bundle discount, and panel 162's +46 buys nothing here.
But the **prediction branch is effectively spent on this milestone already**. Panel 162 declined a
removal and paid with registered predictions; §1.6 records that of the six ledger rows bought with a
prediction, **none has ever been collected**, and that an outstanding prediction is *re-decided,
never renewed*. Two unscored rows on one milestone, both due at the same future gate, is the shape
the section names as failing. So my reading is **stricter than the rule requires and I say so**: the
second amendment to one milestone should pay with a **named removal, measured in its own commit**,
or it should not land. I priced no removal and did not find one I would stand behind; the honest
entry is *nothing — and that is a problem*.

## 6. The refusal, held to Part 6's standard — and filed in Part 7, not Part 6

A Part 6 row is permanent and is the most expensive commitment available. This form is refused on
**Principle 0's "it waits"**, which is Part 7's vertex — *postponed rather than refused*. Filing it
in Part 6 would be the cheap move, not the robust one. The falsifier is stated to Part 6's standard
anyway, so it can expire in its turn:

> **What would make this refusal wrong**: three or more programs in `examples/` declaring a fixed
> array longer than 8 — the count is 0 today and the instrument is one `grep`, which exists — **or**
> a Part 11 metric-3 run showing that emitting an N-element zero literal costs **more than six**
> correction round-trips per use (≈6000 program tokens, the point at which +60 spec tokens
> amortise against a 1-in-13 corpus share). Produce either and route 2 returns on merit at +32.

## 7. Deliverables

- `verdict`: **veto** (routes 1, 2, 3) · **approve** route 4, filed as a Part 7 item
- `section`: design.md §1.0/Principle 0, with §1.2, §1.12 and §1.6
- `spec_token_delta`: **measured** — 8030 → 8030 (route 4). Routes priced: D1a 8048 (+18, false as
  written), D1b **8090 (+60)**, D2 **8062 (+32)**, D3 **8097 (+67)**
- `removal`: nothing — and that is a problem
- `needed_for_self_hosting`: no
- `argument` (≤120 words): The compiler self-hosts and the longest fixed array declared in the whole
  tree is `i8[8]`, so Principle 0's first branch is empty and no seat offered a Part 11 measurement
  for the second. The wall is already passable: `partial` plus a one-line helper function compiles
  and prints `Darwin` today, at 903 program tokens against 142 — 761 saved, which needs one program
  in thirteen to bind a long array before +60 real spec tokens break even. The corpus share is zero.
  Route 1 costs +60, not +18; +18 buys a sentence that makes § 9's *no overloading* and § 11's
  `repeat(s, n)` false. Route 3 reopens a hazard no sanitizer on this Mac can see. It waits.
- `prediction`: if nothing lands, `./heroes measure spec/heroes-spec.md --refresh` reads **8030**
  real with digest `199b5f0a66d147d1` at M-readable-bytes close. If route 1 lands as D1b it reads
  **8090 ± 0**; if it lands as D1a at **8048** the document carries two sentences the compiler
  contradicts. At M-selfhost-fixpoint, the `grep` in § 3 above still returns **zero** declared fixed
  arrays longer than 8 in `examples/`.
- `condition`: I withdraw the veto on route 2 (+32, the cheapest and the only memory-safe amendment)
  the day either falsifier in § 6 is measured — three corpus programs, or a metric-3 round-trip cost
  above six per use — **and** a named removal is measured in the same commit. I do not withdraw it
  on route 3 at any token price: that one loses on §1.12, which outranks the budget.

*Measured 2026-09-18 in a scratchpad copy; the repository working tree was not modified. Program
counts are marked* lower bound *because `--refresh` refuses every path but `spec/heroes-spec.md`
and `CLAUDE.md`.*
