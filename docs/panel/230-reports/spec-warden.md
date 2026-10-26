# Panel 230, the spec-warden's report

Written as it goes, from the first finding (the shared brief's rule). Worktree
`.claude/worktrees/230-spec-warden`, detached at `595d92d88`, clean (`git status --short` printed
nothing). Scratch `.claude/worktrees/230-spec-warden-scratch/`. Read before anything else:
`00-shared.md`, `spec-warden.md`, `critic-briefs.md` (40 findings). `blind/README.md` read last, as
asked.

## 1. The ceiling, read today, and where this milestone stands against the block

- **design.md §1.6, by grep today** (`grep -n '1\.6' docs/design.md`, the section at `:253`):
  *16384 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`*. Unchanged
  from my brief's number. The payment rule is unconditional at every level (`:317-326`, panel 123
  R7): every amendment owes a named removal or a registered prediction naming an instrument that
  exists on the day of registration, and its milestone (`:328-339`, panel 046).
- **The consolidation block's sentence** (`:292-293`): *the rows from M-buildable-structs to
  M-compatibility-promise ... spend at most **1,500** real tokens before the thesis harness has run,
  and an addition past them waits for it; and the next raise rests on what that harness measures a
  token of specification to buy*.
- **M-panic-location is inside that span**: `grep -n` over `ROADMAP.md` puts M-buildable-structs at
  row 66 (`:191`), M-thesis-harness at row 70, *done 2026-10-24* (`:195`), M-panic-location at row
  72, OPEN (`:197`), M-compatibility-promise at row 74, scheduled (`:199`).
- **§1.2's price of one correction, as measured** (design.md `:389-392`, panel 226's R12): metric 2
  first ran on 2026-10-24 (measurement 052), *the rewrite rate read 0.198 on Sonnet 5.5 strict and
  0.147 control, the round-trip's median 3,619 tokens*. My standing question 3 says *500 to 2000*;
  the measured median is 3,619, so every rewrite-rate argument below is priced at 3,619, read from
  the document today, not recalled.

## 2. The consolidation block (brief item 4): how much stands spent, and whether it still binds

**Spent, read forward** (`awk 'NR>=226 && NR<=253' docs/measurements/010-spec-budget-ledger.md`):
the raise's row (7655, 2026-10-14) starts the block at **10,048** real; panel 215's row reads 10,108
(+60), panel 217's *238 of the consolidation block's 1,500 spent* (10,286), panel 227's (row 115)
*251* (10,299), and row 116, the claim, **10,297**, *-2 ... no addition, so no payment is owed*,
with no block figure. The ledger's two stated figures equal both the net since 10,048 and the sum of
the additions (every delta before row 116 was positive), so its method cannot be read off it: **249
spent** if the block is net, **251** if it is gross. Which one is an inference either way. It moves
no verdict of this sitting: 1,249 or 1,251 stand unspent, against drafts of tens of tokens (§ 4).

**Whether it still binds.** design.md `:292-293` binds the 1,500 *before the thesis harness has
run*, *an addition past them waits for it*. The harness has run: M-thesis-harness, row 70, *done
2026-10-24* (`ROADMAP.md:195`), measurement 052, which is the held-out run AND the ablation the
same sentence names, *the spec without its grammar rules as one of Part 11's variant contexts*
(052 § 4: 44 of 60 green at turn 1 with the productions, 44 without; 51 and 53 at turn 3). So
**by its own words the cap's condition has passed**: it bound additions made before the run, and
"waits for it" has nothing left to wait for. Nothing written since says so or says otherwise:
`grep` of `1,500` and `consolidation block` over `ROADMAP.md`, journals 069 and 070, the
milestone's page and `tests/harness/suite_spec.hero` finds only the pre-run wording
(`suite_spec.hero:125`, `ROADMAP.md:23`). **My reading, and it is a reading, not a ruling**: the
1,500 no longer binds this sitting; what binds is the ceiling (16384, 6,087 free and 6,027 beside
the FFI floor, row 116), the payment rule, unconditional at every level, and §1.0's burden.
The block's lapse should be written down by the author, since a rule nobody retires keeps being
cited as live (it is in `suite_spec.hero`'s comment today). **And the next raise's rule has its
first number, which argues against growth, not for it**: the one ablation run, of the productions,
bought +0 points at turn 1 and read 2 better without them at turn 3. A token of specification has
not yet been measured to buy anything on these tasks, so headroom is not an argument for any
sentence below (§1.6 `:394-397`, *a spec that grows to fill the budget because it can has failed
§1.2*).

**One more thing the run measured that this sitting's §1.2 arithmetic needs, and one it did not.**
Measured: the rewrite rate (Sonnet 5.5 strict 0.198, 23 correction turns over 120 samples; median
correction 3,619 tokens; 052 § 9). Not measured anywhere in 050 to 052 (`grep -n -iE
'\babort(s|ed)?\b|\bcrash'` over the three finds one line, 051's *a program stopped loudly
(`abort`)*): **how many correction turns followed an `abort` or a `crash` outcome**, which is the
whole population a place line or a rendered side can act on. Its count is in 052's
`verdicts.jsonl` (outcomes), which I did not open (held-out material, the coordinator's to count by
decision 4a's rule: numbers alone). Until it is counted, every §1.2 claim for § 6 and § 12 is
priced on a base rate nobody has read.

## 3. The instrument, and today's baseline (brief item 1)

My compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build
selfhost/main.hero -o heroes-next`, exit 0; `./heroes-next --version` prints `heroes 0.5.0` and
`sources 413da33e7969760a`, the shared brief's key. No clock was run.

**The unchanged file, re-measured today**: `./heroes-next measure spec/heroes-spec.md` prints
legacy **7717**, `cl100k_base` **7854**, maximum 7854; then `set -a; . <root>/.env; set +a` (key
length 108, value not printed) and `./heroes-next measure spec/heroes-spec.md --refresh` prints
`SPEC_REAL_TOKENS` **10297**, `claude-opus-5`, taken `2026-10-26`, digest `5bb452355391e3b6`. So
the brief's 10,297 holds today, and every delta below is against my own baseline of this sitting.

**How each draft was priced**: `make_drafts.py` in my scratch builds each draft from the unchanged
spec by exact replacement, asserting every anchor matches exactly once; the draft `P` it builds is
**byte-identical** to the blind seat's `spec-P.md` (`cmp`, sha256 `aa11f940...`, the critic's
digest), which checks the anchors. Then for each draft: `cp` it over `spec/heroes-spec.md` in my
worktree, `heroes-next measure` (vendored) and `heroes-next measure ... --refresh` (real), `git
checkout -- spec/heroes-spec.md`. 36 drafts, 36 `--refresh` calls, the token-counting endpoint
alone; `git status --short` empty afterwards. Every number below is **real**, the binding one.

## 4. The prices, each sentence alone and together (real, `claude-opus-5`; baseline 10,297)

| draft | the words (changed part) | real | Δ real | Δ vendored max |
|---|---|---|---|---|
| **§ 1** `1P`, as drafted | *produces nothing or a `()?`: a `fail` it returns is written on stderr, and the program exits 1.* | 10,328 | **+31** | +24 |
| § 1 `1a`, cheapest M1 measured | *produces nothing or a `()?`, whose `fail` is written on stderr, exiting 1.* | 10,324 | +27 | +19 |
| § 1 `1b` | *...or a `()?`; a `fail` it returns is written on stderr and the program exits 1.* | 10,327 | +30 | +23 |
| § 1 `1M2` | *produces nothing or an `i64`, the program's exit status.* | 10,310 | +13 | +11 |
| § 1 `1M4` | `1P` plus *with where it was made* | 10,333 | +36 | +29 |
| § 1 `1M6` | *produces nothing or an `i64?`: `ok(n)` exits with status `n`, and a `fail` is written on stderr and exits 1.* | 10,339 | +42 | +33 |
| § 1 `1M7` | *produces nothing; an `.err` arm in it that does not end the program with `exit(code:)` is a compile error.* | 10,329 | +32 | +27 |
| **§ 6** `6P`, as drafted | *saying why and where: the file, line and function;* | 10,307 | **+10** | +10 |
| § 6 `6col` | *...the file, line, column and function;* | 10,309 | +12 | +12 |
| § 6 `6own`, narrowed to L1's reach | *saying why, and where if an operation of the program raised it;* | 10,311 | +14 | +12 |
| § 6 `6can` | *saying why and, where it can, where;* | 10,305 | +8 | +8 |
| § 6 `6walk`, names the walk | `6P` plus *or for a fault the functions running* | 10,317 | +20 | +18 |
| § 6 zero | today's sentence | 10,297 | **0** | 0 |
| **§ 12** `12P`, as drafted | *both sides of a comparison, written as this document writes a value.* | 10,286 | **−11** | −7 |
| § 12 `12min` | *both sides of a comparison.* | 10,276 | **−21** | −16 |
| § 12 `12bare` | *shows the source expression and both sides.* | 10,270 | −27 | −20 |
| § 12 `12cut` | `12P` plus *cut where long* | 10,291 | −6 | −3 |
| § 12 `12S2` | *both sides of a comparison, an array or a map by its length.* | 10,286 | −11 | −6 |
| § 12 `12S3` | *shows the source expression, and where the two sides of a comparison first differ.* | 10,282 | −15 | −11 |
| § 12 `12gone` | the `assert` failure sentence deleted | 10,251 | −46 | −34 |
| **§ 13** `13i` (owed under M1; the check runs, its abort wins) | *aborts when `main` returns, a `fail` included, saying how many.* | 10,304 | **+7** | +6 |
| § 13 `13ii` (the leak unreported) | *aborts when `main` returns without a `fail`, saying how many.* | 10,303 | +6 | +4 |
| **§ 11** `11u8` (owed, defect 2224) | `` `exit(code: u8)` (ends the program) `` | 10,297 | **0** | 0 |
| § 11 `11abort` | *(ends the program; a code outside 0 to 255 aborts)* | 10,312 | +15 | +12 |
| § 11 `11clamp` | *(ends the program; a code outside 0 to 255 exits 1)* | 10,313 | +16 | +14 |
| § 11 `11bits` | *(ends the program, the shell seeing its low eight bits)* | 10,311 | +14 | +9 |
| M5 `M5e` | *`eprint(...)` (the same, on stderr)* in the `Built-ins:` sentence | 10,315 | +18 | +13 |
| M5 `M5x` | `` `exit(code: i64, msg: str)` (ends the program, writing `msg` on stderr) `` | 10,314 | +17 | +12 |

**Together** (real): the proposal `P` (`1P`+`6P`+`12P`) **10,327, +30** (the brief's *+27, a lower
bound* was the vendored figure; the real is 3 more); `P`+`13i`+`11u8` **+37**; `P`+`13i`+`11abort`
**+52**; `1P`+`13i` +38; `1P`+`13i`+`11u8` +38; `12min`+`11u8` **−21**; `1P`+`13i`+`12min`+`11u8`
**+17**; `1P`+`13i`+`6P`+`12min`+`11u8` **+27**; `M5e`+`12min`+`11u8` **−3**. The deltas add (the
sum of the parts equals the whole to the token in every combination measured), so any other
package is the sum of its rows.

**Every package is two orders of magnitude inside the ceiling** (the largest, +52, against
6,027 free beside the FFI floor, a factor of 116) and inside the block's 1,249 whatever the block's status (§ 2).
**No budget veto exists in this sitting.** What decides each sentence is the payment rule and
§1.0's burden, below.

### The route nobody listed for defect 2224, measured at 0 tokens: `exit(code: u8)`

`11u8` changes one word of § 11 and costs **0** real tokens (10,297 both). It needs no new
sentence, because § 2 already says *a literal must fit its type* and § 11 already says `to_i8` ...
`to_u64` *give a `T?`, because the number may not fit*. Run on my compiler with a user function of
that shape, `function quit(code: u8)` (`230-spec-warden-scratch/u8probe/`): `quit(code: 256)` is
`error[int_out_of_range]: 256 does not fit a u8`; `quit(code: -1)` the same; `quit(code: n)` with
`n` an `i64` is `error[type_mismatch]: expected u8, found i64`, its note offering `to_u8(x)` with
`.must()` (aborts) or `.default(v)`; `quit(code: to_u8(n).default(1))` built and ran, status 255
with no argument and 1 with one. And the before, on the same compiler: `exit(code: 256)` checks
clean, builds, and exits **0** (defect 2224). So the silent success becomes **a compile error for a
literal and an answered `T?` for a computed code**, which is the thesis's own move, and a reader
pays nothing to learn it. What it costs is not spec tokens: `exit` is library Heroes
(`selfhost/library_source.hero:267-268`, `function exit(code: i64)` calling `hero_exit(code)`), so
the library's signature, the compiler's 14 calls in `selfhost/main.hero`, and every program that
passes an `i64` (`examples/`: one `exit(f.code)`) move; the compiler-engineer's to count. It is
**the most robust route on the list and the cheapest**, and, subject to W5 (§ 9: the §1.2 risk
that a computed `i64` code becomes a correction turn), I recommend it over `11abort`,
`11clamp` (both +15 or +16, both a run-time answer to a mistake the type can refuse) and `11bits`
(+14, which writes defect 2224's silent success into the language as a rule; refused on §1.12).

### A second batch, closing the readings found in the first (same instrument, spec clean after)

| draft | the words | real | Δ real |
|---|---|---|---|
| § 13 `13either` | *aborts when `main` returns, failing or not, saying how many.* | 10,302 | **+5** |
| § 13 `13even` | *aborts when `main` returns, even with a `fail`, saying how many.* | 10,305 | +8 |
| § 12 `12Pptr` | `12P` plus *a pointer as its address* | 10,294 | −3 |
| § 12 `12Pfull` | `12P` plus *a pointer as its address, cut where long* | 10,299 | +2 |
| § 1 `1Pord` | `1P` plus *unless an abort ends it first* | 10,338 | +41 |

**The cheapest closure of finding 36 is `13either`, +5**, in § 13 where the leak check lives,
cheaper than saying it in § 1 (`1Pord`, +10 over `1P`). `13i` (+7) reads two ways: *a `fail`
included* can attach to *how many*.

## 5. Every sentence's other readings (brief item 5)

**§ 1, `1P`** (*a `fail` it returns is written on stderr, and the program exits 1*):
- with a lease or a handle live, § 13 says the return aborts (134) and § 1 says it exits 1: two
  rules, no order (the critic's finding 36, measured by them at 134 on today's compiler). Closed by
  `13either`, +5, the check running first and its abort winning, which keeps the leak report on the
  failing path (`13ii`, +6, drops it there: refused on §1.12, a resource fault told nowhere);
- `?` leaving `main` needs no word: § 6's table already sends the error to the caller *whose return
  type must be fallible*, which `()?` is;
- *written on stderr* says nothing of the form (`error: <code>: <msg>`) or of a line feed inside the
  message (defect 2225): output a reader of the run sees, not a rule a program is written against,
  so no words; the printer is the compiler's to make unforgeable;
- `main_returns`' message (`selfhost/value_errors.hero:168`, *a program reports failure by what it
  prints, not by what it returns*) becomes false under M1: compiler work, not spec tokens.

**§ 6, `6P`** (*saying why and where: the file, line and function*):
- **false by construction for the third kind under L1 alone**: 25 lines that print `panic:` from
  fault handlers, the exit-time checks and the lease reports (the critic's finding 4, measured by
  them, not re-run by me). A reader told *recursion too deep aborts* (§ 9) and *a lease nobody ends
  ... aborts when `main` returns* (§ 13) expects a file and a line for both, and neither has a call
  site to pass one;
- the narrowings keep an exception each: `6own` (*where if an operation of the program raised it*)
  is false for the stack guard and for a program's own call of an `extern` function its header
  declares as a pointer while it holds none (`runtime/parts/stack.c:677`), both operations of the
  program caught in a fault handler; `6walk` (*or for a fault the functions running*) is false on Windows,
  which has no walk (`stack.c:850-857`, the brief's citation), and at exit, where no frame stands;
  only `6can` (*where it can*, +8) and zero are true under every route, and `6can` tells a reader
  nothing they can act on;
- *the file, line and function* where the place line also writes a column (finding 38): not false,
  incomplete; `6col` names it for +2;
- *the file* of an abort inside the library's own Heroes (`map`, `find`) names a file the reader
  cannot open: the compiler-engineer's question 2, with words owed only if the sentence is written.

**§ 12, `12P`** (*both sides of a comparison, written as this document writes a value*):
- a `str` holding a quote or a line feed: the document writes `"a\"b"` and `"\n"`, so the clause
  promises the spec's escapes, which closes defect 2225 for sides and decides finding 9's two
  notations **for sides only** (`\u{1b}`, not panel 192's `<U+001B>`), leaving `.must()`'s line,
  which § 6's *saying why* governs, on the other notation;
- **a `ptr`, a `cstr`, a function value, a handle**: the document writes no value of these types
  but `nullptr`, so the clause is undefined there, and a reader can take a `cstr` side to show its
  text where S1 writes an address;
- **past S1's bounds** (16 elements, 1,024 bytes, depth 6): the document writes values whole, the
  renderer cuts them;
- a union's member and a `partial` record's unnamed fields have no written form at all;
- a map: the document gives keys *in no order* (§ 10), so a reader cannot derive S1's sorted order;
  not false, and the order is output;
- *of a comparison*: the IR carries sides for a top-level comparison only
  (`selfhost/ir/flatten.hero:937-953`, `questions.comparison_op`), so `assert a == b && c == d`
  shows none; today's *both sides* reads the same way, no change.
Closing the undefined ones costs **+13 over `12P`** (`12Pfull`, +2 net against today), for a
promise about output a program is never written against.

**§ 12, `12min`** (*both sides of a comparison*): no notation promised, so none of the readings
above arises; **true under S1, loose under S2** (an array by its length is a side shown in short)
**and S3** (one differing field of each side), and **false under S4**, so it lands with the renderer
or not at all. Two comments quote the older form of this sentence as the spec's: `runtime/parts/failure.c:203`,
*"An `assert` failure shows the source expression and both sides."*, and `selfhost/emit/abort.hero:91`
(`git grep -n 'shows the source expression'`); `12min` makes them nearly true again, and today
they quote a sentence the spec no longer holds (defect 051's class, a comment asserting what is not
so; reported here, not filed).

**§ 13, `13either`**: *failing or not* attaches to *`main` returns*; no other reading found.

**§ 11, `11u8`**: none found. A reader learns the range from the type, § 2 says a literal must fit
it, and § 11 says `to_u8` gives a `T?`.

## 6. Principle 0 (brief item 3), and what reopening panel 159 owes

**The compiler's need, measured** (`grep -n 'exit(code' selfhost/main.hero`, 14 lines, comments
excluded): `exit(code: 2)` five times, `exit(code: 0)` twice, `cli_table.exit_code(...)` (an `i64`,
`selfhost/cli/table.hero:220`) six times, and `exit(code: hero_thread_join(handle: handle))` once.
And, counted over the tree's programs (`grep -rnE '(^|[^a-z_.])exit\('` over `examples`,
`tests/thesis`, `selfhost`, comments and strings excluded): **191 calls, 182 with a literal code**;
the 9 computed are the compiler's 8 and `examples/interpreter/main.hero:63`'s `exit(f.code)`; the
task suite's 148 are all literals.

- **M1**: the compiler gives statuses 0, 2 and computed ones and writes its errors through
  `hero_write_err`, so it would not use a `main` that exits 1. **Compiler-need: no. Ticket: the
  thesis alone**, and the thesis is unmeasured: Part 11's tasks pin every status in prose and forbid
  `extern`, and its grader ignores stderr (`docs/metrics/tasks.md:55-69`, the critic's finding 26),
  so the held-out run cannot see a default; 052 published no `main_returns` count. **Neither, so it
  waits**, unless this sitting's own measurement (family M, § 7) shows the effect.
- **M5** (`eprint`, +18, or `exit(code:, msg:)`, +17): the compiler would use it, but it has a
  working route today, a binding of the runtime's own header, so it does not NEED it, unless the
  sitting rules that binding `hero_os.h` from a program is an accident to close (the critic's
  finding 12, the ffi-pragmatist's question). Then M5 is compiler-need, and `M5e` with `12min` and
  `11u8` is **−3 net**. Otherwise it waits: no instrument reads stderr, and family M does not
  separate M5 from M1.
- **M6** (+42): not needed (`exit(code:)` serves 2 and 3 today), no measurement, the dearest § 1
  wording. Waits.
- **M7** (+32 for the cheapest wording I could make true): a refusal that would refuse a correct
  program, a `main` whose `.err` arm recovers (a default port, a fallback file), which
  `.claude/rules/verification.md` classes `blocking`. Waits until a rule exists that refuses only
  the measured class.
- **M2** (+13, the cheapest § 1 change): a `-> i64` inherits defect 2224 (a returned 256 is a
  success); not needed; no variant measures it.

**What a reopening owes.** Panel 159's R3 is a ratified sentence, but not a design.md Part 6 row
(`grep -n -iE 'main_returns|exit status|exit code' docs/design.md` finds no row refusing a result
from `main`), so CLAUDE.md §12's *the program or compiler fact that would make it wrong* is owed in
substance, not by form: a measured class of programs that the refusal makes worse, and an answer to
159's conditional veto (*accepting `main() -> ()?` without a specified exit code*), which M1's
*exits 1* gives. **What changed since 2026-09-16 is enough to reopen and not enough to reverse.**
The author never read 159 (its foot: *by delegation and not by reading*), which is reason enough
to put it back on the table. But each change the shared brief lists is evidence about something
else: the harness ran, and it cannot see a default; defect 2224 is about `exit`'s range and is
answered by § 11 whatever `main` does; issue 1001's move is administrative; and **M1 leaves the
program 1001 measured exactly where it is**, a `main` that matches a failure, prints it and exits
0 (the critic's finding 11). The evidence that could carry a reversal is family M: its
`declares_fallible_main` scores 159's own prediction (*≥1 in 3 first attempts at a fallible `main`
writes `-> ()?`*), never scored in 40 days (`grep -rln main_returns docs/measurements` finds
nothing), and its SILENT and passed counts are the only reading of the default anywhere.

## 7. What pays for each (brief item 2), and what of the blind design can serve

**§1.2's break-evens**, every one priced at 052's measured median correction of 3,619 tokens: a
sentence of Δ tokens pays on the formula when it saves a correction in more than Δ / 3,619 of first
attempts. `1P`+`13either` (+36): **1.0%**. `6P` (+10): 0.28%. `11abort` over `11u8` (+15): 0.41%.
`M5e` (+18): 0.50%. (In tokens, §1.2's unit; a cached specification token is cheaper in money,
which lowers every break-even and changes no ordering.)

| sentence | Δ real | what pays | standing |
|---|---|---|---|
| § 12 `12min` | −21 | itself, a removal (the clause any rendering route makes false) | paid; frees 21 |
| § 12 `12P` | −11 | itself | paid; frees 11, and adds the promise § 5 finds undefined |
| § 11 `11u8` | 0 | nothing owed | subject to W5 below |
| § 11 `11abort` | +15 | its tokens by § 12's removal; its burden by §1.12 (defect 2224 is a silent success) | the fallback if W5 fails |
| § 6 `6P` | +10 | **nothing that exists can**: family P changes the spec and the stderr together, and Part 11 has no arm that varies an abort's text (the critic's findings 28 and 32); a removal could pay its tokens and still leave §1.0's burden unmet | not payable |
| § 1 `1P` + § 13 `13either` | +36 | **family M's measurement**, as panel 217's sitting measurement paid row 7846, with § 12's −21 as a named removal for 21 of the 36 | pending this sitting's run |

**What of `blind/README.md` (read last, sha256 `0aa2745d...`, the 84-session design) can be a
payment:**

- **Family M can pay for § 1, and nothing else in this sitting can.** Its variants differ in the
  specification alone (*the same text in both variants; only the specification differs*), its
  instruments are fixed by digest before the first session (`score.py` `4d3fa8b0...`,
  `m/run-cases.sh` `7e4517cd...`, `spec-P.md` `aa11f940...`, the prototype's diff `319cff23...`),
  it is scored before the key is read, and its thresholds are written before the run. That is an
  instrument that exists on the day of registration (§1.6 `:328-339`), scored at this milestone.
  Its three readings, and what each would pay:
  - **`declares_fallible_main`** in the 18 T sessions scores panel 159's prediction and is the
    sharpest test of §1.2: **2 or more of 18** puts the one-sided 97.5% lower bound of the rate at
    1.38% (Clopper-Pearson, computed with `python3 -I` by bisection on the binomial tail: 0.14% for 1 of 18, 1.38% for 2, 3.58% for 3), above M1's
    1.0% break-even, so M1 pays on the formula; 1 of 18 bounds it at 0.14% and pays nothing. One
    caution: in a one-shot session a refused `main` passes no case, where the real loop pays one
    correction for it, so **`passed` overstates M1's effect** and should not carry the payment
    alone;
  - **SILENT over the 30 default error cases**, 5 or more fewer under P (the README's own
    threshold): the reading of issue 1001's default, and a payment for § 1 if met;
  - `tally`'s wrong statuses: a cost reading of M1 (the critic's finding 27), not a payment.
- **Family P cannot pay for a sentence**, by the README's own words (*in family P the
  specification and the stderr change together, so a difference belongs to the whole proposal*).
  A difference there is evidence for the MECHANISM (L1 and S1, which cost no spec tokens under the
  zero route), and in the honest direction it is evidence that the stderr, not the sentence, does
  the work. **To price `6P` it would need a third variant, today's specification with the
  proposal's stderr**; 24 more Haiku sessions at the README's 0.14 USD bound are 3.36 USD at most,
  past the author's 12 (11.76 committed), so either the author widens the budget or the zero route
  stands. I recommend the zero route rather than the spend.
- **Nothing in the blind design separates M1 from M5, L1 from L4, or S1 from S2** (its *What the
  seat cannot say*), so no route against another is payable from it.

## 8. The verdicts, one per sentence

### § 1, what `main` may produce (M1, the proposal)

- `verdict`: **veto** (Principle 0's burden unmet today), lifted by the condition below. Counts
  measured, not provisional.
- `section`: CLAUDE.md § 2 (Principle 0); design.md §1.6 `:317-339` (the payment rule and its standard) and
  `:394-397` (*a spec that grows ... because it can*); §1.2 (`:389-392`, the measured rewrite rate).
- `spec_token_delta`: **10,297 → 10,328, +31** as drafted (`1P`); +27 at the cheapest M1 wording
  (`1a`); **+36 with the § 13 clause it owes** (`13either`); by `heroes-next measure
  spec/heroes-spec.md --refresh`, applied and reverted.
- `removal`: none of its own; § 12's −21 (`12min`) can stand as a named removal for 21 of its 36,
  which pays tokens and not the burden.
- `needed_for_self_hosting`: **no** (the compiler gives 0, 2 and computed statuses and writes
  stderr through `hero_write_err`; this report's section 6).
- `argument`: M1's ticket is the thesis alone: the compiler ends with statuses 0, 2 and computed
  ones and writes stderr through a binding, so it would not use a `main` that exits 1. No
  instrument has measured the thesis effect: Part 11's tasks state every status and its grader
  ignores stderr, and 052 published no `main_returns` count. It reopens panel 159's R3, never read
  by the author; what changed since is evidence about `exit`, not about `main`'s result, and M1
  leaves issue 1001's measured program exiting 0. On §1.2's arithmetic its +36 pay only if more
  than 1.0% of first attempts are refused `main_returns` (36 / 3,619). Family M can show that in
  this sitting; until it does, M1 waits.
- `prediction`: W2, W3 and W4 below.
- `condition`: any one of: family M's T sessions declare `main -> ()?` in **2 or more of 18**;
  family M's SILENT over the 30 default cases is **5 or more lower** under P; 052's strict-arm
  turn-1 verdicts carry `main_returns` in **5 or more of 480**. Then approve `1P` with `13either`
  in the same commit (+36), paid by that measurement and § 12's removal.

### § 6, where an abort says it stopped

- `verdict`: **veto `6P` as drafted** (false by construction under L1 alone, and no instrument can
  meet §1.0's burden for it); **object** to every other wording (`6col` +12, `6own` +14,
  `6walk` +20, `6can` +8); **approve zero tokens**, today's sentence kept.
- `section`: design.md §1.6 (payment rule) with CLAUDE.md § 2's burden; CLAUDE.md §12 (*spec beats
  compiler*, so a false sentence makes the compiler wrong); panel 159's hinge (*manufacturing a
  defect that does not exist*).
- `spec_token_delta`: **10,297 → 10,307, +10** as drafted; 0 for the route I approve.
- `removal`: nothing, and the zero route owes nothing.
- `needed_for_self_hosting`: no (the mechanism may be §1.12's; the sentence is nobody's need).
- `argument`: The place is output the reader of a run sees, and it names itself (`at
  report.hero:13:44, in report.windows`); no program is written against it, and *saying why*
  stays true under every route. As drafted the sentence is false by construction for the third
  kind under L1 alone, and every narrowing keeps an exception: stack exhaustion and a call through
  an `extern` pointer that holds none are operations of the program caught in a fault handler, and
  Windows has no walk. No instrument can pay for it: family P changes the specification and the
  stderr together, and no arm of Part 11 varies an abort's text. Writing a sentence the compiler
  is known to contradict is panel 159's hinge.
- `prediction`: registered as an observation, which pays nothing: if family P reads P 4 or more of
  24 above T on *located*, the effect is the stderr's, which costs no spec token.
- `condition`: a mechanism under which *the file, line and function* holds for every abort § 6
  defines, on every platform, **and** a family P variant with today's specification and the
  proposal's stderr showing the sentence's own effect at the README's threshold.

### § 12, what a failed `assert` shows

- `verdict`: **approve the removal that matches the route taken**: `12min` (−21) with S1, `12S2`
  (−11) with S2, `12S3` (−15) with S3, each the cheapest wording measured that is true of its
  route; **object to `12P`'s notation clause**; with S4, today's sentence stays and nothing is
  owed.
- `section`: design.md §1.6 (a removal owes no payment) and §1.2.
- `spec_token_delta`: `12P` **10,297 → 10,286, −11**; `12min` **→ 10,276, −21**; `12Pfull` (the
  clause closed) → 10,299, +2.
- `removal`: *where `print` takes them; where a side is an aggregate it shows the expression
  alone*, which any rendering route makes false.
- `needed_for_self_hosting`: no.
- `argument`: Any route that renders an aggregate makes today's clause false, so the sentence must
  change, and its cheapest true form shrinks the document by 21. The proposal's *written as this
  document writes a value* buys a writer nothing, since no program is written against an assert's
  report, and promises what the compiler would not do: the document writes no `ptr`, `cstr`,
  function value or handle, S1 cuts past 16 elements, 1,024 bytes and depth 6, and a union's member
  has no written form. Closing those costs 13 more. Quoting a `str` side is defect 2225's repair, a
  false message under §4.17, and needs no sentence.
- `prediction`: W1.
- `condition`: if the sitting insists on promising the notation, `12Pfull` (+2 net) and never
  `12P`; if it keeps S4, no change.

### § 13, owed by M1 (finding 36)

- `verdict`: **approve `13either` (+5) if and only if M1 lands**, in the same commit; object to
  `13i` (+7, two readings) and to `13ii` (+6, the leak report dropped on the failing path, §1.12).
- `spec_token_delta`: 10,297 → 10,302, +5. `removal`: paid with § 1. `needed_for_self_hosting`:
  no.
- `argument`: Two sentences that disagree about one program are worse than the silence either
  replaces. `13either` is the cheapest wording measured and the robust order: the check runs on
  every return of `main` and its abort wins, so a lease left live on a failing path is still
  reported.
- `condition`: M1 not taken, nothing owed.

### § 11, `exit(code:)`'s range (defect 2224)

- `verdict`: **approve `11u8`** (0) on W5; **object** to `11abort` (+15) and `11clamp` (+16) as
  run-time answers to what the type refuses at compile time (`11abort` is the fallback if W5
  fails); **object** to `11bits` (+14), which writes 2224's silent success into the language as a
  rule (§1.12; I would join a soundness veto on it).
- `section`: design.md §1.6, §1.12; spec § 3's *no implicit conversions, widths included*.
- `spec_token_delta`: **10,297 → 10,297, 0**.
- `removal`: none needed.
- `needed_for_self_hosting`: **yes**, the range of a built-in the compiler calls 14 times.
- `argument`: Defect 2224 is issue 1001's own class, a failure told to the shell as success,
  reached through the route every M keeps, so § 11 must say what a code outside 0 to 255 does. The
  type says it for nothing: one word, 0 real tokens, and today's compiler already refuses `256` and
  `-1` for a `u8` and an unconverted `i64`, its note naming `to_u8` (run). It is § 3's rule
  applied to the one built-in that breaks it. Its risk is §1.2's: a computed `i64` code becomes a
  correction turn. In the tree, 182 of 191 calls pass a literal, and all 148 in the task suite do.
- `prediction`: W5, and W6.
- `condition`: W5 failing (3 or more of 480 answers pass a computed code) moves me to `11abort`,
  paid by § 12's removal; or the compiler-engineer measuring a migration the tree cannot make
  mechanically.

### The routes with no sentence in the proposal

- **M5** (`M5e` +18, `M5x` +17): **object**, it waits; it becomes compiler-need, and then
  **approve `M5e`**, if the sitting rules that a program's binding of `hero_os.h` is an accident to
  close (`M5e`+`12min`+`11u8` is −3 net).
- **M2** (+13), **M4** (+36), **M6** (+42), **M7** (+32): **object**: none is needed, none is
  measured, and M7 would refuse a correct program.

## 9. Predictions registered (falsifiable, each with its instrument and when it is scored)

- **W1, tokens.** Instrument `heroes measure spec/heroes-spec.md --refresh`; scored at
  M-panic-location's landing step. Against today's 10,297 the landing reads the sum of its rows in
  § 4 (the deltas added to the token in all 9 combinations measured): `12min`+`11u8` **10,276**;
  with `1P`+`13either` **10,312**; the proposal with what it owes, `P`+`13either`+`11u8`,
  **10,332**. If another row moves the spec first, the same deltas against that base.
- **W2, family M, `declares_fallible_main`.** Instrument the blind seat's `score.py` (sha256
  `4d3fa8b0...`), tallied when the key is joined; scored at this sitting, M-panic-location. **At
  most 1 of the 18 T sessions** declares `main -> ()?` (panel 159's llm-ergonomist registered at
  least 6). Falsified at 2: then M1 clears §1.2's break-even and my § 1 veto lifts.
- **W3, family M, SILENT and passed.** Same instrument and milestone. **P's SILENT count over the
  30 default error cases within 4 of T's**, and P's *passed* within 2 of 18 of T's beyond what W2's
  refusals account for.
- **W4, the held-out run's `main_returns`.** Instrument: 052's `verdicts.jsonl`
  (`build/thesis-runs/c24a5b81`), the diagnostic codes of the strict arm at turn 1, counted by the
  coordinator with numbers alone (decision 4a); scored at this sitting's synthesis. **At most 4 of
  480** (under 1.0%).
- **W5, computed exit codes.** The same programs, counted the same way: **at most 2 of 480** turn-1
  answers pass a non-literal code to `exit(` (the 0.41% break-even between `11u8` and `11abort`).
- **W6, the refusal, if `11u8` lands.** Instrument `heroes check`, at the landing step: with
  `exit(code: u8)`, `exit(code: 256)` is refused `int_out_of_range` and `exit(code: n)` with `n` an
  `i64` `type_mismatch`, each with its golden case annotated, and the compiler's own 14 calls
  build.

**The count owed and not predicted**: how many of 052's 23 strict correction turns (Sonnet 5.5)
followed an `abort` or `crash` outcome, the whole population a place line or a rendered side can
act on in the instrument. It pays nothing; without it, §1.2 cannot price the mechanism at all.

## 10. Corrections to the briefs, measured here

- The shared brief's *the draft reads 7881 vendored, 7743 legacy: +27, a lower bound*: the real
  count is **10,327, +30** (§ 4).
- The block: *251 spent* is the gross figure; net it is 249 (§ 2). Whether it binds is a reading I
  give, and the author's to write down.
- My own standing brief's *one correction round-trip costs 500–2000 tokens* is superseded by
  design.md's measured median, **3,619** (`:389-392`); every break-even here uses it.

## 11. Disclosures, written last

- **The blind design I read** is sha256 `0aa2745d...`, which is exactly the pre-registration
  committed in `811e84d39` (`git show 811e84d39:docs/panel/230-briefs/blind/README.md | shasum -a
  256`). Since then (12:30) the working copy adds a dated note: `run_one.sh` gained `< /dev/null`
  after session 1, and `shipping.hero`'s header comment shows its mistake in both variants. Neither
  touches family M's instruments (`score.py`, `m/run-cases.sh`, `spec-T.md`, `spec-P.md`), so § 7's
  reading of what can pay stands. The `shipping` flaw weakens family P further, which pays for no
  sentence anyway.
- **W2 and W3 were written at about 12:37, after the blind run had begun** (that note says
  sessions 1 and 4 onward had run), **without my reading any session's folder, transcript, output
  or score**; I did not open `build/b230*` and do not know where the coordinator's run stands. The
  pre-registration that binds is the README's, committed before the first session: SILENT 5 or more
  of 30, *passed* 3 or more of 18, and panel 159's *at least 1 in 3* for `declares_fallible_main`.
  My *2 of 18* is a §1.2 reading of that same tally (the break-even, § 7), and its timing is
  disclosed here so the synthesis can weigh it as such. W1, W4, W5 and W6 name counts nobody has
  read (the landing's, and 052's codes and programs, which I did not open).
- **Not re-run by me and cited as others' measurements**: the 25 third-kind lines (the critic's
  finding 4), the 134 of a `main` returning with a lease live (finding 36), `handled` exiting 0
  (finding 11), Windows' missing walk (`stack.c:850-857`, the shared brief). Everything else in
  this report was run in this session, in my worktree at `595d92d88`, with its command beside it.
- **Paid runs**: none. **42** `--refresh` calls to the token-counting endpoint, the one the brief
  names: 1 for the baseline, 36 for the first batch, 5 for the second (`wc -l` of `prices.tsv`
  reads 41, and `drafts/` holds 41 files).
- **Files**: this report alone in the lane. Scratch:
  `.claude/worktrees/230-spec-warden-scratch/` (`make_drafts.py`, `make_drafts2.py`, `drafts/`, 41
  drafts; `prices.tsv`; `u8probe/`; the two build logs; `exit-calls.txt`). My worktree's
  `spec/heroes-spec.md` reverted after every pricing, `git status --short` empty at the end.
