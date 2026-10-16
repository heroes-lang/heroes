# Panel 217, the spec-warden

Written 2026-10-16 from about 22:05, as the work went. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.
My copy is `<scratchpad>/217-spec-warden/`, `git worktree add --detach … f57af53da` (HEAD
`f57af53dae65fedb66d8915311738b8bb05dd38e`), `rm -rf build`, then the compiler from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 8.81 user 8.30 sys 0.23`).
`m-g-at` was copied over `spec/heroes-spec.md` there before anything was drafted or measured:
`shasum -a 256` printed `af7fc4d6…` for the copy's file before and `ac4ff3e263629118…` after,
which is `m-g-at`'s.

## 0. The ceiling, read today rather than carried

- **design.md §1.6, grepped at 22:08**: the ceiling is **16384** on `claude-opus-5` through
  `POST /v1/messages/count_tokens`, raised from 10240 by author decision 2026-10-14
  (`e9eaa8431`), **with a second rule this seat's file does not carry**: *the rows from
  M-buildable-structs to M-compatibility-promise, the consolidation block, spend at most **1,500**
  real tokens before the thesis harness has run, and an addition past them waits for it.* The
  payment rule (a named removal or a registered prediction naming an instrument that exists
  today) is unchanged and unconditional.
- **The seat's own file is a stale carrier again**: `.claude/agents/spec-warden.md` lines 8, 12 and
  44 say **10240** (`grep -n '10240\|16384'`), last touched `da28f000d` on 2026-09-29, two weeks
  before the raise. It also lacks the 1,500 rule. Held to that file, this sitting would read 10,108
  as 132 under a ceiling it is 6,216 under, and would not know the block budget exists. Not mine to
  edit here (one file written in the trunk); reported for the coordinator.
- **What the block has spent, by the ledger**: the trunk's `docs/measurements/010-spec-budget-ledger.md`
  ends at row 228, the raise, 10,048 real. The one row since is R3's (`m-g-at`), uncommitted, in
  `<scratchpad>/step9-aside/…/010-spec-budget-ledger.md` line 229: **+60 real** (10,048 to 10,108).
  `git log e9eaa8431^..main -- spec/heroes-spec.md` prints only the raise. So the block has spent
  **60 of 1,500** if R3 lands as that row says, and this sitting's addition is the second spend.
- `tests/harness/suite_spec.hero`: `CEILING` 16384 (line 156), `FFI_FLOOR` 60 (line 286),
  `DELTA_GATE` 50 vendored tokens (line 211), `SPEC_TOKENS` 7655 vendored (line 109).

## 1. THE F1 DRAFT EXISTS (22:19)

**`<scratchpad>/217-spec-warden/heroes-spec-f1.md`**, sha256
**`8106db1e4fb9aced1d5be768f45c56e71b777aae438e6dd4f063a7e8ff700c50`**, **10,227 real** on
`claude-opus-5` (`heroes measure spec/heroes-spec.md --refresh` in my copy at 22:18:42 with this
file as `spec/heroes-spec.md`), against `m-g-at`'s **10,108** re-measured at 22:07:33: **+119 real**;
vendored 7,798 cl100k / 7,662 legacy, against 7,703 / 7,568 (+95 / +94). A backup with the same
sha256 is at `<scratchpad>/217-spec-warden-work/heroes-spec-f1.md`. With it in place, `spec` reads
27 passed and 4 failed, the four being the count pins any unpasted draft fails (`budget`,
`spendable`, `real`, `ledger`), and `grammar` reads 9 passed, 0 failed.

**Spelling: the same as the prototype's**, `c: Counter lent`, the word after the type, the call
unmarked. What the prototype must also match, or the readers are refused for following the text:

1. **The production refuses `@` and `lent` together on a Heroes parameter**:
   `Param = "@" ident ":" Type | ident ":" Type [ "lent" ] .` (`CParam` is unchanged: it already
   admits `lent` without `@`).
2. **A `lent` argument and an `@` argument of one call reaching one place are refused**, as two
   `@` arguments are today (`aliased_mutable_arguments`, probes `alias_same` and `alias_overlap`
   below, both refused at `f57af53da`).
3. **UFCS applies to a first parameter declared `lent`**: the text exempts only `@`, so
   `l.peek()` is legal by it.
4. **A group's record declared `lent` without `@` on a C function is a pointer to const**, the
   place, which C reads and never writes. The prose says *pointer to const* and not
   `` `const T *` ``, because `const` is a refused foreign word (`selfhost/foreign_words.hero:148`)
   and `spec/rejected` reads every code span: measured, the same draft with `` `const T *` `` reads
   `FAIL spec/rejected … const`, 26 passed and 5 failed.
5. **The text says *read-only*, nothing more**: by it, copying out of a `lent` parameter, handing it
   to a by-value parameter or returning it is a read and is legal, a lock-holding record's copy
   being R2's refusal by type. The compiler-engineer's brief refuses *copied* and *returned*
   outright; if the prototype does, either the prototype relaxes or the text owes a clause. Not a
   difference task C can meet.

The example is the specification's own `Lex` (`peek(l: Lex lent)` beside `advance(@l: Lex)`), **never
task C's names**, so that the arm measures whether readers reach for the form, not whether they copy
an example (the critic's A2).

6. **A function with a `lent` parameter used as a value** (`xs.map(peek)`) is not ruled by the
   text: *top-level functions are values* (§ 9) then applies. Today a function with an `@`
   parameter is refused as one (probe `at_fn_value`, `error[mutable_parameter_as_value]`: *no
   function type carries an `@` (§ 9)*). Since a read-only lend is observably a copy once (2)
   holds, an adapter passing the argument's address is sound; if the prototype refuses instead,
   the text owes that clause. Task C does not meet it.

**The diff against `m-g-at`, whole** (`diff m-g-at heroes-spec-f1.md`):

```
124c124
<     Param       = [ "@" ] ident ":" Type .
---
>     Param       = "@" ident ":" Type | ident ":" Type [ "lent" ] .
272,274c272,276
<   `function advance(@l: Lex)` … `advance(@l)`. Semantics: the callee works on
<   the caller's place itself. UFCS does
<   not apply when the first parameter is `@`.
---
>   `function advance(@l: Lex)` … `advance(@l)`; a read-only one is marked
>   `lent`, in the signature alone, where one with neither mark would be a copy:
>   `function peek(l: Lex lent)` … `peek(l)`. Either way the callee works on the
>   caller's place itself, which no other `@` argument of the call may reach.
>   UFCS does not apply when the first parameter is `@`.
415c417,419
< it is handed unless declared `lent`, and a lend reaches only one so declared.
---
> it is handed unless declared `lent`, and a lend reaches only one so declared; a
> group's record declared `lent` without `@` is a pointer to const, the place C
> reads and never writes.
```

## 2. The draft against the prototype (22:33, `heroes-f1` sha256 `e2127c87e9a3…`, run from my own probe folder)

Fifteen probes in `<scratchpad>/217-spec-warden-work/probes/f1/`, each built with
`<scratchpad>/217-compiler-engineer/heroes-f1` and its binary run under `timeout 10`, every program's
`build/` in my folder. **Twelve agree with the text**: `lent` and `@` overlapping in one call refused,
whole and by field (`aliased_mutable_arguments`, p2, p2b); UFCS on a first `lent` parameter accepted
(p3, prints 6); a write, an onward `@` lend and an `@` at the call refused (`not_mutable` p4, p5,
p14; `marker_mismatch` p6); a copy out, a return, a by-value hand-off, a temporary argument and an
immutable binding's lend accepted (p7, p8, p9, p10, p12). The prototype allows the copy and the
return, so point 5 above is settled the text's way. **Three do not agree**:

- **p1, `function f(@p: Pt lent)` on a Heroes function builds** (exit 0, prints 4); the draft's
  production refuses it. I hold the production: `@` and `lent` on one Heroes parameter says *mutable*
  and *read-only* at once, and the C position, where it means *written, not kept*, is the only one
  that needs both. The prototype owes the refusal at landing. Harmless to the arm: a reader who
  writes it builds and is tabulated `@c`.
- **p11, a `lent` function as a value is refused**, `error[lent_parameter_as_value]`, as an `@`
  one is. The draft is silent, so *top-level functions are values* reads as allowing it. **The text
  owes the clause at landing**, and it owes it for `@` already: § 9 does not say what
  `mutable_parameter_as_value` cites it for. Priced below; not in the arm's draft, which task C does
  not reach and whose sha256 is already published.
- **p13 and p15, a C function that WRITES bound `lent` without `@` is accepted, and gives a wrong
  value at exit 0.** p15: `pt.h` with `static inline void pt_bump(struct pt *p) { p->x += 1; }`,
  bound `function pt_bump(p: Pt lent)`; `q = Pt(x: 1)`, an immutable binding, then `pt_bump(q)`:
  prints `1`, `2`, `2`. A `=` binding changed with no `@` anywhere (§ 5: *`=` binds once,
  forever*). The emitted probe is `hero_ffi_probe_…_pt_bump(struct pt * a0)`, no `const`, so clang
  is never asked. p13 binds `counter_lock(c: Counter lent)` against `int counter_lock(struct counter
  *c)` and builds: the lock is taken through a lend the text calls read-only. **This is the draft's
  promise unchecked, CLAUDE.md § Precedence rank 3 (a wrong program accepted, `blocking`)**, and the
  ffi-pragmatist's veto line. The repair is the pointee probe's: a record lent without `@` is probed
  as `const struct pt *`, so passing it where C takes `struct pt *` is clang's
  `-Wincompatible-pointer-types-discards-qualifiers` (unrun on the prototype; a question for the
  compiler-engineer and the ffi-pragmatist). **For the arm**: a reader who binds `counter_lock`,
  `counter_unlock` or `counter_init` as `c: Counter lent` builds and prints `6`, `4`, `5` under
  `heroes-f1`, so the person's table must also mark any non-const `counter.h` function bound without
  `@`, as a wrong binding the judge passes.

*Corrected 2026-10-16, read from `date` at 22:23: § 2's heading says 22:33, a time I took from the
compiler-engineer's "READY at 22:31" rather than from the clock. The probes ran between 22:21 and
22:22 (`stat` of `p1_at_and_lent.bin` and `p15_c_writes_lent.bin`), and `heroes-f1` and `key-f1.hero`
were written at 22:20 and 22:21 (`stat`), so that report's 22:31 is ahead of this machine's clock as
well.*

**The landing's price, the function-value clause added** (`<scratchpad>/217-spec-warden-work/drafts/f1-landing.md`,
§ 9's *Top-level functions are values, but not one with an `@` or `lent` parameter:
`xs.fold(0, add)`.*): **10,246 real** (`--refresh` at 22:23:24), **+19** over the arm's draft and
**+138** over `m-g-at`; vendored 7,815 cl100k / 7,678 legacy.

**p15 is a known class with its instrument in the tree**: `selfhost/emit/ffi_read_only.hero` (defect
411, 2026-10-06) asks every non-`@` `cstr` parameter `__builtin_types_compatible_p(P,
__typeof__(*(P)0) const *)` and refuses a writable pointee as `ffi_writable_parameter`, after
`SHA256_Final(md: cstr lent)` wrote 32 bytes through a shared `str` at exit 0. The prototype does not
ask it of a record lent without `@`. Extending that question is the repair the C half owes.

## 3. The other prices, each measured alone (`--refresh`, my copy, 22:12 to 22:20)

| draft | what it is | real | over `m-g-at` |
|---|---|---|---|
| `m-g-at` | the base | 10,108 | 0 |
| only the production | `Param = "@" ident ":" Type \| ident ":" Type [ "lent" ] .` | 10,120 | +12 |
| only § 9 (B2's wording) | the `lent` clause, the unmarked call, the aliasing clause | 10,168 | +60 |
| only § 13 | *a group's record declared `lent` without `@` is a pointer to const …* | 10,142 | +34 |
| A | precise production, § 9 rewritten (*A parameter marked `@` … is mutable*) | 10,216 | +108 |
| A-loose | A with `Param = [ "@" ] ident ":" Type [ "lent" ] .` | 10,212 | +104 |
| B / B2 | the § 9 clause merged into the sentence already there | 10,210 / 10,214 | +102 / +106 |
| **C1w, the arm's draft** | B2 plus *where one with neither mark would be a copy* | **10,227** | **+119** |
| C1w + function values | the landing: *but not one with an `@` or `lent` parameter* | 10,246 | +138 |
| F0s | no form; § 9 says `@` also hands a read-only function an uncopyable record | 10,142 | +34 |

The three edits add exactly (12 + 60 + 34 = 106 = B2). **C1w over B2 is +13 for the sentence's one
claim about what `lent` is FOR**: B2 sorts parameters into mutable and read-only, and a by-value
parameter is read-only too (`not_mutable`), so B2 would recruit `lent` for every unwritten
parameter; C1w says the difference is the copy, which is task C's whole question. CLAUDE.md
§ Precedence (2026-09-28: *of the spec's own tokens, always the most robust and solid route*) takes
the 13.

**F0s is false as written**: `doubled(@p: Pt)` returning `p.x * 2` is refused `never_rebound`
(probe `at_read_only`, my compiler at `f57af53da`), so *`@` also hands a function that only reads
it* sends a reader to that refusal whenever the helper reads a field itself. A true version must
say the read reaches C, or relax panel 210's rule. It also writes R2's lock rule into the
specification, which panel 215 ruled against (*no sentence of the specification: the message
carries the fix*). **The conservative route has no simple true sentence; it is F0 and R2's note.**

## 4. The verdicts

### F1, the arm's draft (C1w): **approve, provisional**: on the arm's predictions, on p15's repair, and on p1 and p11 reconciled

- **section**: design.md §1.6 (the payment rule and the consolidation block's 1,500), §1.2, §1.3,
  Principle 0 (CLAUDE.md § 2), §1.12 for p15.
- **spec_token_delta**: **+119 real** (10,108 to 10,227) for the arm; **+138 real** (to 10,246) at
  landing with the function-value clause; vendored +95 / +112 (cl100k), over `DELTA_GATE`'s 50, so
  the commit body names the payment. Under the ceiling: 10,246 + 60 (FFI floor) = 10,306 of 16,384.
  **The block**: R3's +60 and this +138 are 198 of the 1,500 the consolidation block may spend
  before the thesis harness runs.
- **removal**: nothing, and that is a problem. The one word it drops is m-g-at's *Semantics:*.
  No sentence becomes redundant: the out-parameter sentence, `.ptr()`'s *lent whole with `@`* and
  § 5's `never_rebound` clause all stand. **The payment is the registered prediction below**,
  scored by instruments that exist tonight: `heroes-f1`, the arm's launcher and the person's
  table.
- **needed_for_self_hosting**: **no**, measured: `selfhost/`'s 12 extern files declare 58
  functions and no group record (`<scratchpad>/217-spec-warden-work/selfhost-externs.txt`), so no
  parameter of the compiler takes a record by place.
- **argument**: The thesis is already met for the copy: R2 refuses all ten first programs. What F1
  buys is §1.2's other factor and §1.3's honesty. Arm R2 measured F0's price: ten first tries
  refused, ten second tries correct, all ten `doubled(@c)`, a call line that says the counter
  changes when it does not. F1 is additive (both halves are refused today, probes
  `f1_heroes_half` `expected_params_close`, `f1_c_half` `lent_shape`). With aliasing refused, a
  read-only lend is observably a copy, so the unmarked call keeps locality. It costs 119 tokens on
  every prompt. Against task C's round (500 to 2,000), it pays where at least 6% to 24% of sessions
  need it, a share nobody has measured. The ffi-pragmatist's census gives a reach: 11 of 188 `@`
  extern parameters point at const.
- **prediction** (registered; scored by the coordinator's table of arm F1 tonight, `heroes-f1`
  building each `c.hero`): of 10 readers, **(a) at least 8 build and print 6, 4, 5; (b) at least 8
  declare `doubled`'s parameter `lent` without `@`**, the half F1 can fail; (c) at most 1 copies a
  lock, read from the source; (d) at least 8 bind `counter_value` as `c: Counter lent`; (e) at most 1
  binds `counter_init`, `counter_lock` or `counter_unlock` without `@`, which `heroes-f1` passes
  (p13). **Token prediction**: the landing measures **10,246 real** by `heroes measure
  spec/heroes-spec.md --refresh` if `m-g-at` lands first byte for byte and the clause is taken,
  10,227 if not.
- **condition**: **veto** if (a) or (b) misses: the only payment is that prediction, so a miss
  leaves +119 unpaid under §1.6, F0 and R2's note stand, and the form is re-argued.
  **Veto** if the form lands without p15's repair: *a pointer to const, the place C reads and never
  writes* would be a promise the compiler does not keep, a wrong value at exit 0. **Object** until
  the compiler refuses `@` with `lent` on a Heroes parameter (p1; spec beats compiler, CLAUDE.md
  § 12) and the text carries the function-value clause (p11). **Approve** outright on (a), (b), p15
  repaired and p1 and p11 reconciled.

### The other forms

| form | verdict | real | the reason, on what |
|---|---|---|---|
| F0, nothing (R2's note says `@c`) | **object**, and it is the conservative route the record keeps | +0 | arm R2: 10 of 10 first tries refused, 10 of 10 second tries correct, 10 of 10 `doubled(@c)`; §1.2 one round per program, §1.3 a call line claiming a change |
| F0s, a § 9 sentence and no form | **veto** | +34 | false as written (`never_rebound`, probe `at_read_only`), and writes R2's rule into the spec against panel 215's ruling |
| F2, by-value passed by place, no word | **object** | +0 (Heroes half), unbuilt | reaches no C: lending to `const struct counter *` still needs `@`, so task C's copy or `@c` stays (a reading of the grammar, unrun); if it changes no program's meaning it is an emission choice and needs no sitting |
| F3 as listed | nothing to rule | — | already the language (the critic: `written_nowhere.hero` line 7, the kit's key builds) |
| U5, an `@` parameter only read admitted | **object** | unpriced | undoes panel 210's `never_rebound`, whose one bug-shaped case is a dropped write (`fixedbugs-135`), and makes the signature that claims a change the official route |
| U1, a new word (`in`, `view`, …) | **object** | unpriced, at least F1's | a word every tool learns (the `named` check, the formatter, the editors) to separate *kept* from *read-only* on a C parameter. No binding in the tree is shown to need that separation: the census's 11 const `@` parameters are not yet read for whether C keeps them |
| U2, a mark at the call | **object** | unpriced | §1.5, terse where you use: with aliasing refused, the lend is observably a copy, so the mark tells the line nothing |
| U3, by type (an uncopyable record passed by place) | **object** | unpriced | §1.3: a C parameter's C type would depend on what the record's header struct holds, and R2's rule would enter the spec |
| U4, a lend method at the call to C | **object** | unpriced | reaches C and not the Heroes helper, so task C's helper still copies or claims a change |
| U6, the message alone | **approve as part of whichever form lands** | +0 | under F1, R2's note and `never_rebound`'s fix must say `lent`, not `@c` (a diagnostic class, the compiler-engineer's) |

## 5. For the coordinator, outside this seat's question

- **`.claude/agents/spec-warden.md` carries 10240 and no block budget** (§ 0). The file's own rule
  says to grep design.md first, and this sitting did; the next seat that trusts its number vetoes
  at 10,240 a document that has 6,000 tokens of room.
- **The arm's table owes two columns the judge cannot see**: a non-const `counter.h` function bound
  without `@` (p13, passed by `heroes-f1`), and `@c: Counter lent` on the Heroes helper (p1, passed
  by `heroes-f1`, refused by the text).
- **`aliased_mutable_arguments`, `never_rebound` and `not_mutable` still say *copy-out***, against
  `m-g-at`'s § 9 and this draft's (probes `alias_same`, `at_read_only`): R3's landing owes them,
  as panel 215's verification paragraph lists.

*Corrected 2026-10-16 at 22:26: § 2 counts *fifteen probes*; there were sixteen, p1 to p14 with p2b
in one batch and p15 written after them to show p13's consequence. Twelve agree with the text; p1,
p11 and p13 (with p15) do not.*

**Closed at 22:26**: my worktree removed (`git worktree remove --force`; `git worktree list` names it
no more), and `heroes-spec-f1.md` put back at `<scratchpad>/217-spec-warden/heroes-spec-f1.md`, sha256
`8106db1e…700c50` again, from the backup in `<scratchpad>/217-spec-warden-work/`, where the drafts,
the probes and the suite outputs stay. Every count above is a `--refresh` on `claude-opus-5`, one
token count per draft (eleven in all, 22:07 to 22:23), and no model run.
