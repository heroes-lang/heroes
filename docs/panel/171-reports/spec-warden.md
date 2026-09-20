# Panel 171: spec-warden

**Every number below was produced by a command run in this session, 2026-09-20,
Darwin arm64, in a COPY of the tree at `b6e26fcc` under my own scratchpad
(`.../scratchpad/spec-warden-171/tree`), with a compiler built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`).** Every draft
was applied to `spec/heroes-spec.md` on the real path in that copy, measured
with `./heroes measure spec/heroes-spec.md --refresh` after sourcing the copy's
`.env` (key length printed, 108; value never), and reverted; `git status
--short spec/` in the copy was empty after every run. The main tree's spec
re-measures **8201 real / 6159 cl100k** and `git status` there shows only the
two untracked `docs/panel/171-*` directories.

Ceiling re-grepped rather than taken from any brief: `docs/design/design.md:255`,
**10240, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`**.

---

- **verdict**: **approve**. Measured, not provisional.
- **section**: design.md §1.6 (payment rule, unconditional), §1.0 (the closure
  list, first bullet), §1.12 (*it does not suspend Principle 0 ... what §1.12
  decides is the shape of a form already admitted*), §1.2.
- **spec_token_delta**: **measured**. Baseline **8201 real / 6159 vendored**.
  Cheapest complete draft (M2, one-token word) **8214 real / 6168 vendored**:
  **+13 real, +9 vendored**, under `DELTA_GATE` 50. Net of the 60-token FFI
  floor: 8274 against 10240, **1966 free**. Full table in § 1.
- **removal**: the clause the flip falsifies, *and nothing checks it* (**−8
  real / −5 vendored** alone), together with the reason that was owed only while
  nothing checked, *C keeping the pointer reads bytes the program may have
  changed or freed since*. Spent inside M2, whose prose is **+3 real** net over
  today's sentence; the unpaid **+10** is the `CParam` slot and the example, paid
  by P1 and P2 below. Panel 170's removal (the tag clause) is **no longer
  spendable**: see § 5.
- **needed_for_self_hosting**: **yes, under the ruling.** Nine extern functions
  in `selfhost/` carry thirteen `cstr` parameters that receive lends at twelve
  lines; without the word the flipped checker refuses `selfhost/cli/process.hero`
  and `selfhost/emit/literal.hero`, and the compiler cannot compile itself.
- **argument** (118 words): The flip makes today's § 13 sentence false in its
  last clause and redundant in its middle one, so the merged rewrite is nearly
  self-funding: the production and the example cost +10, the prose +3. Merging
  beat appending by 72% here because it deletes two clauses; at panel 170 it
  lost because it added qualifiers. The word enters on the closure list, measured
  from the tree: thirteen `cstr` parameters in nine `selfhost/` declarations,
  every lend for the duration of its call. That discharges §1.0 for the word's
  entry but not for its shape, which §1.12 says still owes proof, and has it
  measured: own slot, negative polarity, one token. §1.6's payment is
  unconditional and the closure list buys no exemption; the removal and two
  predictions pay.
- **prediction**: P1, P2, P3 in § 7, each with an instrument that exists today.
- **condition**: § 8.

---

## 1. Every draft, priced on the instrument that judges it

Word `reads` unless stated. Each row: applied to the real path in the copy,
`--refresh`, reverted. Vendored figures from `./heroes measure` offline over the
same text in a second pass this session; real figures from `--refresh`.

| # | draft | real | Δ real | cl100k | Δ vend |
|---|---|---|---|---|---|
| — | baseline, main tree and copy | 8201 | — | 6159 | — |
| G | `CParam` own slot + the sqlite example's `path: cstr reads`, **no prose** | 8211 | **+10** | 6165 | +6 |
| **M2** | **merged**: *A lend lives for its call and no longer: a parameter is taken to keep what it is handed unless declared `reads`, and a lend reaches only one so declared.* (+ G) | **8214** | **+13** | **6168** | **+9** |
| M1 | merged, with a gloss (*C's word that it keeps no pointer past the call; any other takes a lease*) (+ G) | 8218 | +17 | 6172 | +13 |
| M3 | M2 + *; the rest take a lease* | 8222 | +21 | 6174 | +15 |
| A2 | surgical: keeps the *why*, replaces only *and nothing checks it* with *so a lend reaches only a parameter declared `reads`, which says C keeps none* (+ G) | 8229 | +28 | 6177 | +18 |
| A1 | **appended**: today's sentence minus the false clause, then a new sentence (+ G) | 8248 | **+47** | 6191 | +32 |
| C | delete *, and nothing checks it* **alone**, nothing else | 8193 | **−8** | 6154 | −5 |
| S1 | an INTERIM step-1 text: today's sentence kept + *`lent` after a `cstr` or `ptr` parameter says C keeps no pointer past the call* (+ G) | 8245 | +44 | 6191 | +32 |

**Merging wins, by 72%** (M2 +13 against A1 +47). The brief asked which case this
is: at panel 122 merging won by 36%, at panel 170 it lost by 24%. **The rule
behind both: merging wins when the sentence it joins LOSES a clause, and loses
when it GAINS a qualifier.** Here the flip deletes two clauses, so the merged
form is almost free. It should be quoted that way and not as a law.

**The false clause alone is worth −8 real.** M2 spends it and the *why* clause
too, which is why its prose nets +3. The reason for the default (bytes changed or
freed under C's held pointer) now lives where a reason belongs, design.md §1.12
and §4.19; the prompt carries the rule.

**The drafts pass the document's own instruments.** With M2/`reads` applied in
the copy: `grammar` **7 passed, 0 failed** (the new terminal is a defined,
reachable production); `spec` **16 passed, 4 failed**, the four being `budget`,
`spendable`, `real` and `ledger`, all of them the pinned count, which the landing
commit re-pins in `tests/harness/suite_spec.hero` and `selfhost/measure/pinned.hero`
as `--refresh` prints them.

## 2. The word, priced

M2 with each candidate, three occurrences each (production, example, prose):

| word | real | Δ | cl100k | tokens per occurrence |
|---|---|---|---|---|
| `reads` | 8214 | +13 | 6168 | 1 |
| `lent` | 8214 | +13 | 6169 | 1 |
| `forgets` | 8217 | +16 | 6171 | 2 |
| `noescape` | 8217 | +16 | 6171 | 2 |

**A one-token word saves +3 real over a two-token one**, one per occurrence.
That is the whole price difference between spellings. The difference that
matters is the prose a word needs to be unambiguous, and there I have one
measured objection to `reads`, not a preference:

**`reads` contradicts a sentence the paragraph already carries.** Line 369:
*C writes back through the lend only where the binding is a `@` name.* Under the
flip a `ptr counted_by n` parameter that C WRITES through must also carry the
word, so `buf: ptr counted_by n reads` would be written on a parameter the
document says C writes. Either the word gets a sentence un-saying read-only
(M1's gloss cost +4; a full disclaimer more), or a reader takes it as `const` and
the rewrite rate pays. `lent` asserts nothing about direction and needs no new
vocabulary: the paragraph already uses *lend* seven times, so it is the one
candidate whose meaning the document defines before the word appears. `noescape`
is the historian's precedent and two tokens, and not this language's register.
The choice is the ergonomist's; the price of a gloss is measured here.

## 3. Principle 0, from the tree, and it is the closure list

`grep -rn "\.cstr()" selfhost/` gives 52 raw hits (the brief said 55; three fewer
today). Excluding message text and comments, the lends are:

| file:line | function | lends |
|---|---|---|
| `selfhost/cli/process.hero:83-86` | `hero_run_go` | 4 (`program`, `""`, `out`, `err`) |
| `:114` | `hero_fs_mkdir_all` | 1 |
| `:119` | `hero_fs_exists` | 1 |
| `:122` | `hero_fs_is_directory` | 1 |
| `:125` | `hero_fs_remove` | 1 |
| `:134` | `hero_fs_rename` | 2 |
| `:151` | `hero_dir_scan` | 1 |
| `:166` | `getenv` | 1 |
| `selfhost/emit/literal.hero:144` | `atof` | 1 |

**Twelve lines, thirteen lend expressions, nine functions, thirteen `cstr`
parameters** (declarations at `process.hero:42-53` and `literal.hero:42`). The
brief's twelve against my thirteen turns on `"".cstr()` at `:84`, a literal lend
into `in_path`; it is a lend and needs the mark like the others. `.ptr()` in
`selfhost/`: **zero** real sites; `.lease()`: **zero**.

**I agree: this enters on the closure list (design.md §1.0, first bullet).** Under
the flip every one of the thirteen is refused and the compiler cannot compile
itself. What that changes about the burden, in four parts:

1. **The word's entry owes no Part 11 measurement.** The thesis branch was the
   burden at panels 169 and 170, when `selfhost/` never retained; the ruling
   moved the word to the first bullet. I say plainly that the need is
   **derivative**: the compiler needs the word because the author flipped the
   default, and the flip's own justification is §1.12's, the author's, and not on
   this ballot.
2. **The shape still owes proof, and §1.12 says so in its own words**: *It does
   not suspend Principle 0 ... What §1.12 decides is the shape of a form already
   admitted.* The shape has it, measured: own slot beside `counted_by` (panel 170:
   `atoi(s: cstr consumes)` is `error[unread_mark]`; the slot cost +3 real then
   and is inside G's +10 now), negative polarity (the historian's survey), no
   ending call (the ffi veto), one token (+3 real per extra token, § 2).
3. **§1.6's payment rule is unconditional and the closure list buys no
   exemption.** The removal and the predictions are owed exactly as for a thesis
   form. They are named above.
4. **It fixes the landing order**, which is the shared brief's point: the seed
   must parse the word before `selfhost/` writes it. § 4 says what that means for
   the document.

## 4. What the flip does to the document's own runnable example

`tests/harness/suite_special.hero:330` compiles the first fence after
`## 13. FFI` with `sqlite3_open(path: ":memory:".cstr(), @db)`: **a lend into an
unmarked `path: cstr`**. Under the flip the spec's own example is refused by the
spec's own harness unless line 341 gains the word. So the fence changes, it is
inside G's +10, and it is the reader's first sight of the word, which is a
demonstration cheaper than any gloss (M1's costs +4 more).

Two consequences for the landing, both mine to state because the document is
what they reach:

- **`.claude/rules/spec-shape.md`: a change to a fence is compiled before it is
  written.** The fence can be compiled only by a compiler that parses the word,
  so the spec's edits belong to the commit whose seed does. **The document should
  move once, in the flip's commit, at +13**, with the production, the example and
  the sentence together.
- **An interim step-1 sentence is not worth its price.** S1, the only honest text
  for a commit where the word parses but today's rule still holds, costs **+44
  real** for one unpushed commit's truth, three times the whole resolution. And
  G alone at step 1 (+10, no prose) is a grammar-admitted, prose-undefined form,
  the exact shape I filed at panel 170 § 3(c) about `owned` on an input
  parameter. If step 1 must exist as its own commit, it carries the parser and
  the seed and **no spec text**, and its body says the document catches up in the
  next commit.

## 5. Panel 170's removal is dead, and the document owes a qualifier

I recommended deleting *and two records may not name one tag* (−11 real) because
defect 072 indicted it. **072 closed on 2026-09-20 by NARROWING** (`selfhost/check/decls.hero:314`,
*at `tag void` ONLY*), ratified in `docs/records/done/2026-09-20-1900-...`. The
rule stands for every named tag, so the clause is a live rule and not a removal.
**And the spec was not amended**: `git log -- spec/heroes-spec.md` ends at step
15, and `grep -n void spec/heroes-spec.md` finds only the callback sentence. The
document now over-states a refusal the compiler no longer performs at `void`.
The honest edit is a qualifier, *one tag but `void`*, priced at **+4 real / +3
vendored**. It is not this sitting's and I do not charge it here; it is a
document debt CL-005 names, and the coordinator should file it.

## 6. §1.2: does it pay

Today a lend into a retaining C parameter is 066's and 068's wrong answer at exit
0 or 070's empty stderr at exit 133: **no diagnostic, so the rewrite loop is
unbounded.** Under the flip the same program is a `check` refusal with a `Fix`,
one round trip of 500–2000 tokens at most. The cost side is real and I state it:
every non-retaining declaration, which is the common case (13 of 13 in
`selfhost/`), carries one word, and forgetting it is a compile error. That is a
loud, local, bounded error replacing a silent, unbounded one, which is §1.2's own
instruction: *eliminate errors and only shorten when it's free.* +13 real pays
back on the first avoided incident.

## 7. Predictions, each with an instrument that exists today

**P1 (tokens).** The commit that lands the flip's spec text reads **8214 ± 3
real** on `./heroes measure spec/heroes-spec.md --refresh` with a one-token word
and the M2 sentence (**8217 ± 3** with a two-token word; 8222 for M3, 8229 for
A2, all measured); vendored **≤ 6172**, moved **≤ 13** against `DELTA_GATE` 50;
net of the 60-token floor **≤ 8280**, **≥ 1960 free**. Scored at that commit.

**P2 (closure).** After the flip commit, the word appears **exactly 13 times**
in `selfhost/` extern declarations, on the nine functions in § 3, **zero
`.lease()`** is added to `selfhost/`, and `./heroes build selfhost/main.hero -o
heroes-next` reaches the fixpoint with no other `selfhost/` edit. Instruments:
`grep -c` and the fixpoint build. Falsified by a fourteenth mark or a lease.

**P3 (§1.2, ownership unchanged).** The flip adds **no `.lease()` to
`examples/`**: today's count is **1** (`examples/gallery/13-lease.hero:23`; the
README hit is prose) and it stays 1, so every repair in `examples/` is a word on
a declaration and none changes a program's ownership. Instrument: `grep -rn
"\.lease()" examples/ | wc -l` before and after. Scored at the flip commit.

## 8. Condition

I move to **veto** only if a measured `--refresh` total passes 10240 minus the
60-token floor; nothing measured today is within 1900 of it.

I move to **object** if any of these lands: (i) the spec text in two commits
with the S1 interim, +44 for one commit's truth, when one commit costs +13;
(ii) `reads` without the sentence that un-says read-only, given line 369's
write-back rule (a rewrite-rate cost, § 2); (iii) the sqlite fence edited
without being compiled by the parsing compiler first (spec-shape.md); (iv) M3's
*the rest take a lease* before question 2's admitted set is settled, since it may
be false for `nullptr` and for a `cstr` C handed back.

I mark nothing provisional: every number here was measured this session on the
instrument design.md §1.6 names, and each run is named beside its number.
