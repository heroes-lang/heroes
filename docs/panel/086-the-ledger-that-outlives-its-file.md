# 086 — The ledger that outlives its file

**Convened** 2026-08-19, M-bootstrap-archive step 6 (the last row of the
milestone). **Three seats**, and the lane is stated below with what it gave up.

## The proposal, verbatim

> `crates/` moves to `archive/bootstrap-rs/` this milestone: the Rust bootstrap
> compiler and its 4,719 lines of Rust tests leave the live tree, making
> design.md:82's claim true ("no third language anywhere"). Panel 085 R4/R5 ruled
> the move and its order; five things that die with the bootstrap already have
> successors, landed and green this session.
>
> What is not settled: `crates/heroes/src/measure/gate.rs` carries more than its
> checks — its doc comment is **the SPEC_TOKENS ledger**, 38 rows, one per spec
> amendment, each naming what the amendment cost and what paid for it. Two
> design.md Part 1 sentences name that file as the ledger's LIVE home:
>
> - **:322** — "The live figures are the `SPEC_TOKENS` ledger in
>   `crates/heroes/src/measure/gate.rs`, which moves in the same commit as the
>   spec; read them there."
> - **:349** — "the ledger in `crates/heroes/src/measure/gate.rs` carries them,
>   moves in the same commit as the spec, and a figure that lives in one place
>   cannot die in another."
>
> After the move that file sits in a directory panel 085 R4 describes as never
> maintained again, so both sentences become false as written.
>
> **A.** The 38 rows move verbatim into `tests/harness/suite_spec.hero`'s module
> doc, beside the two numbers that enforce them.
> **B.** The 38 rows move verbatim to `docs/measurements/010-spec-budget-ledger.md`,
> a dated record; design.md points there for the history and at
> `suite_spec.hero` for the figure.
> **C.** Nothing moves; design.md's citations become
> `archive/bootstrap-rs/heroes/src/measure/gate.rs`.
> **D.** Something this sitting finds.

**Addendum sent mid-sitting** (coordinator, on CLAUDE.md §1's fourth rule — the
provoking case is a witness, not the class): design.md cites a path inside
`crates/` in **four** places, not two — :322, :349, **:357** (a dated measurement)
and **:1349** ("the inventory is `grep -rn \"// ORDER:\" crates/`", a live
*instruction*) — and CLAUDE.md §4 (:112) names `crates/heroes/src/{lexer,syntax,types}/`
as the paths whose behaviour is panel-gated. Two questions were added: does the
verdict extend to the class, and **is there a mechanism that makes a dead citation
loud?**

## The lane, and what it gave up

Neither of `/panel`'s two lanes fits: the proposal has **no spec token, no
diagnostic, no surface and no C boundary**, so the llm-ergonomist (which reads
only the spec) and the ffi-pragmatist (which compiles the binding C) had no
differentiated input to be judges *about*. The three seats with input were run:
**compiler-engineer** (files and line counts), **spec-warden** (§1.6 is its
section, and it is the seat that vetoed the archive over this very artifact at
panel 085) and **historian** (precedent). What the lane gave up is the reader's
half — and the proposal has no reader-facing half to give up.

## Verdicts

| seat | verdict | rests on | measured cost | condition | veto |
|---|---|---|---|---|---|
| compiler-engineer | **B + a lock** · vetoes **C** · objects to **A** on size | design.md:349 with CLAUDE.md §11, §12 | the ledger is **48,996 bytes**, median row 1,027, **max row 3,379** — 81% of `gate.rs`'s bytes. A gives `suite_spec.hero` either 421 lines with a 3,379-byte comment line (4× the file's current maximum) or ~1,083 lines wrapped to the harness's own 70–83 byte comment width. The lock is **~35–50 lines** and the helpers exist | withdraws the objection to A if the ledger is re-wrapped to ≤83 bytes/line *and* the file lands ≤600 lines; withdraws the veto on C the day `archive/bootstrap-rs` is proven still built in CI | **veto on C** |
| spec-warden | **D = B + one check** | §1.6 (design.md:253, *"must fit in 4096 tokens, measured"*), design.md:349 quoted in full, CLAUDE.md §11's test relaxation | **the ledger measures 12,757 tokens — 3.6× the spec it guards**; spec 3512 / 4096, headroom 584, delta 0 this sitting; registry 916 | approves on (i) the ledger-agreement check landing with B, (ii) :322/:349 repointed, (iii) :1349 repointed to `selfhost/` | no |
| historian | **B**, moderately strongly, **plus a forwarding pointer** where the ledger used to live | precedent, six projects verified with URLs | — | would read A as viable given one live project whose enforced constant and full amendment ledger share a file and survived a move of it | advisory |

### What the three agree on, independently

1. **B is the home.** Three seats, three routes: the engineer on bytes, the warden
   on tokens and on §14's protection (`docs/measurements/` is the only candidate
   directory that is both on §14's never-rewritten list **and** already outside
   `suite_records.hero`'s walk), the historian on five of six precedents splitting
   figure from history.
2. **C is the documented failure mode.** The engineer vetoes it because
   `gate.rs`'s compile-time `assert!(SPEC_TOKENS < 4096)` stops being compiled in
   the same commit that deletes `cargo test` — one live number, one fossil, and
   *"design.md:322 says read them there"*. The historian supplies the precedent
   with a name: Linux built **`make refcheckdocs`** (`scripts/documentation-file-ref-check`)
   because live docs repeatedly cited moved paths, after a 27-patch *"docs: Fix
   more broken references"* series in 2018.
3. **The missing mechanism is the same one, named twice.** *Nothing today checks
   that the ledger's newest row and the enforced constant agree* — the coupling
   has only ever been adjacency at `gate.rs:138/139`, which is a habit. The
   warden's base rate for what habits do: `RESERVED_WORDS_TOKENS` drifted **+85
   across eight milestones** with no commit naming a delta, and row `3510` stood
   false for **6h46m**.

### Disagreements, unsmoothed

- **How far :349 reaches.** The coordinator's brief read *"a figure that lives in
  one place cannot die in another"* as forbidding any second home for the number.
  The warden **narrowed it, quoting the subject**: *"**No count is restated in
  this paragraph** on purpose"* — the sentence is about design.md's own prose, so
  what it forbids is an **unchecked, undated** copy. The general rule the warden
  extracted, and this synthesis adopts: **a number may live in a second place only
  if it is checked or dated.** A ledger row is dated; a pin is checked; design.md
  prose would be neither.
- **A is not merely bigger, it is a different kind of file.** The engineer and the
  warden reject A on different measurements of the same object (48,996 bytes /
  12,757 tokens) and the warden adds the argument that matters: CLAUDE.md §11's
  ~300-line relaxation for tests rests on *"a case is read one at a time and a
  case is self-contained"*, and a 12,757-token module doc is not a case — it is a
  wall in front of seven checks.
- **One of the coordinator's numbers was wrong and both seats caught it.** The
  brief estimated A at "roughly 560 lines"; it is 421–1,083 depending on wrapping.
  The engineer also reported `suite_spec.hero` at 313 lines; **measured at
  synthesis time it is 383** (`wc -l`), which strengthens its own objection rather
  than weakening it.

## Resolution — `provisional — author ratification pending`

Adopted, as the most conservative reading of three converging verdicts:

**R1. The ledger moves verbatim to `docs/measurements/010-spec-budget-ledger.md`.**
Every number and every panel citation survives the move; moving verbatim is not
rewriting (CLAUDE.md §14), and the destination is the one candidate under §14's
protection.

**R2. The lock lands with it, or R1 is not done.** `tests/harness/suite_spec.hero`
gains `LEDGER_ROWS` and one check: the record's row count equals `LEDGER_ROWS`,
**and** its newest row's figure equals `SPEC_TOKENS`, **and** that equals what
`heroes measure` reports for the spec. Three edits must agree; any two
disagreeing is red, with a failure injection under it (CLAUDE.md §9). Target:
`spec: 8 passed, 0 failed`.

**R3. design.md:322 and :349 point at both halves** — the record for the history,
`tests/harness/suite_spec.hero` for the figure — and say which is which.

**R4. design.md:1349's instruction is repointed to `selfhost/`, and the gap it
exposes is recorded rather than papered over.** Measured by two seats
independently: `# ORDER:` is **5 files / 6 occurrences** in `selfhost/` against
**10 files / 13** in `crates/`, and four marks in the bootstrap
(`ir/print.rs`, `emit/{descriptor_set,typedefs_generated,typedefs}.rs`) have **no
marked counterpart in the port**. Repointing alone therefore under-reports, so the
seven-marker gap goes to `SCHEDULED.md` with these numbers.

**R5. A forwarding pointer stays where the ledger was** (the historian's
precedent: CPython's `Misc/stable_abi.toml` header names `PC/python3dll.c` and
`PC/python3.def` as its own previous homes). One sentence in the archived
`gate.rs` naming the new home and the milestone.

**R6. design.md:357 and CLAUDE.md:352 are history and stay as written** — dated
measurements, true when taken. **CLAUDE.md:112 is a live pointer and is the
author's**: §4's trigger list is the contract, so a proposed wording goes to
`DECIDE.md` rather than being rewritten here.

**R7. The dead-citation check lands, in its anchored form.** The engineer measured
both rules: the naive one (any backticked path-shaped token) is **145
occurrences, 84 distinct, 44 missing — 52% false positives, unkeepable**; the
**anchored** rule (the token starts at a repository root directory, no `*`, `<`,
`>`, `{`, `}`) is **47 occurrences, 34 distinct, 2 misses**, and both misses are
directory prefixes without a slug (`docs/panel/042`, `docs/panel/040`), which a
prefix-match rule accepts with no allow-list. Home: `suite_records.hero`, ~45
lines. Its honest limit, stated by the seat that measured it: it catches four of
the six citations in this sitting's class — CLAUDE.md:112's brace form and
:1349's quoted `grep` are outside the rule.

**What a veto would compel.** The engineer's veto binds C only, and C is not
adopted. If the author overturns R1 in favour of A, the engineer's condition is
mechanical: re-wrap the ledger to ≤83 bytes per line and keep `suite_spec.hero`
≤600 lines. If the author overturns R2, the warden's verdict flips to **veto** on
the move commit, and the warden's alternative is not "trust the habit" — it is
that the archive does not happen until the check exists.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at M-bootstrap-archive close the anchored scan reports **exactly one** dead citation (design.md:357, the dated one, allow-listed) and `suite_spec.hero` is **≤450 lines with no line >800 bytes**; under A instead, that file exceeds 420 lines and carries a comment line >3,000 bytes | this milestone's close (`wc`, `awk`, the harness) |
| spec-warden | with R2: `heroes run tests/harness/main.hero` reports **`spec: 8 passed, 0 failed`**, and corrupting the newest ledger row gives **7 passed / 1 failed at exit 1**. Without R2: the newest row and `SPEC_TOKENS` disagree **within three spec amendments** | the second milestone close after this one |
| historian | the next spec amendment after the archive appends its row to whichever file holds the enforced constant and its test, **not** to the archived ledger — falsified if the row lands in `archive/bootstrap-rs/` | the next spec-amending commit (`git log --stat`) |

## Author's verdict

*(pending — `docs/debrief/DECIDE.md`)*
