# Panel 019 — the IR, and where the sugar dies

Date: 2026-08-04. Trigger: M4, lowering (design.md Part 10 step 6, Part 5).
CLAUDE.md §4 makes architecture — backend, IR, tool surface — a panel path, and
this session decides all three at once: the representation every later pass reads,
the pass that erases Part 5's sugar table, and the `heroes build` verb. Five
judges, differentiated inputs; the ffi-pragmatist compiled fourteen C programs,
the llm-ergonomist saw only `spec/heroes-spec.md` and label-stripped dumps.

## Proposal (as put to the judges)

1. **One pass, not two.** No desugared tree: lowering erases Part 5's sugar
   (`if`, `for x in xs`, `?`, `.must()`/`.default()`/`.is_err()`, UFCS, named
   arguments, record construction, `.case`, `assert`) on the way into the IR.
2. **Slots, not SSA.** Locals, parameters and `@` cells are numbered slots
   (`load`/`store`); temporaries are single-assignment by construction; **no phi
   nodes**. A branching *expression*'s value is a synthetic slot written by each arm.
3. **Three-address, typed.** At most two operands and one destination; every value
   carries a `TyId`; one terminator per block (`jump`, `branch`, `return`,
   `unreachable`).
4. **`heroes build <file>`** is M4's subcommand: no flag → it lowers and prints
   nothing (exit 0 = it lowered); `--dump-ir` prints the text IR on stdout.
5. **Generic bodies lower polymorphically** (`Ty::Generic(i)` survives into the
   IR); monomorphisation waits for M6. design.md §4.12's "textual substitution on
   the AST before type checking" is already false — M3 checks generic bodies
   polymorphically.
6. **`test` lowers as a zero-argument function, `assert` as branch + `panic`** at
   M4; only the *runner* waits for M6.
7. **The dump is a deliberate artifact**: deterministic, one instruction per line,
   source names on slots, synthetic slots named `i.1`.

## Verdicts

| Judge | 1 one pass | 2 slots | 3 three-address | 4 `build` | 5 polymorphic | 6 `test`/`assert` | 7 the dump |
|---|---|---|---|---|---|---|---|
| compiler-engineer | approve, **saves ~450** | approve, **saves ~400–550** | **condition**: false as written | approve (+55) | **VETO** — of the unamended spec | approve, **incomplete** | approve (+200) |
| llm-ergonomist | — | approve (Variant 1 decisively) | — | — | — | — | **condition ×4**, + conditional veto on block arguments |
| spec-warden | condition ×3 | approve + mortgage | **condition**: enumerate the exceptions | **OBJECT** — scoped veto on the silent no-flag form | approve (0 tokens) | condition ×2 | condition ×4 |
| ffi-pragmatist | approve | condition (E3) | **OBJECT** — five omissions, three ladder steps unwritable | approve | approve | approve | condition (source order) |
| historian (advisory) | support-with-condition | **support — the decisive row** | support | — | support | — | support-with-condition |

Measured, by the judges given the instruments:

- **spec cost: 2139 → 2139, delta 0**, verified point by point (`heroes measure
  spec/heroes-spec.md`). The spec mentions neither an IR nor the tool, so no point
  here *can* cost spec tokens — which is why the warden went looking for the cost
  elsewhere and found it in five documents that stop being true. Side finding: the
  recorded 2136 is stale (head is **2139**; panel 018's sweep cost **+3 measured**
  against a table that sold shape F as −4 on v0, so the "deltas transfer to v1"
  claim is falsified by 7 tokens), and `heroes measure` vendors two of panel 011's
  three tokenisers — o200k was the highest of the three on v0 (2050 vs 2048).
- **the tree, baseline: 12,472 non-test lines** (`types/` 3,857 · `syntax/` 2,557
  · `resolve/` 1,479 · `printer/` 1,454 · `lexer/` 983), 5,386 more in tests.
  `ir/` is costed at ~2,100 non-test lines over 13 files.
- **slots vs SSA, costed**: phi construction ~200 + phi elimination ~80 + edge
  ownership ~150 + liveness ~120 = **~550 lines avoided**, and CLAUDE.md §7 hoists
  every local to the prologue anyway, so phi *destruction* in the emitter would be
  reinventing slots.
- **fourteen C programs written and compiled** (`-std=c11 -Wall
  -Werror=return-type -Werror=uninitialized -fno-strict-aliasing`), goto-shape,
  against the real headers. Four of them decide points 3 and 7.

## What changed the proposal

**1. An unmangled Heroes `open` silently replaces libc's.** The ffi-pragmatist
compiled two translation units where a Heroes top-level `function open` reaches C
unmangled: clang says nothing, and the program prints `7` instead of a file
descriptor. `open` is not among the 23 names `resolve/builtins.rs` reserves, so
this is legal Heroes today. The IR therefore records **linkage on every callee and
every definition** — `Heroes(decl) | Extern(decl) | Builtin(index) |
Indirect(value)` — which the compiler-engineer asked for independently, from the
other side (a later pass must never re-derive what `Resolved::uses` already knew).
This is design.md §4.19's own named killer class, reproduced.

**2. "At most two operands and one destination" is false in three places at
once.** `print` is variadic, `fold` and `slice` are 3-ary, a record construction is
n-ary — and `dst = call print(x)` is a *hard* clang error (`assigning to 'int64_t'
from incompatible type 'void'`), while raylib is almost entirely void-returning.
So the invariant is restated: no instruction contains a nested expression, and the
operand count is variable in exactly three forms (call, construct, print). The
operand run copies `Params { start, len }` and the canonicalising `runs` map that
`types/table.rs` already has.

**3. The store's direction was a coin flip, and LLVM had already tossed it the
other way.** The ergonomist read `store total, t0` correctly *by namespace luck* —
`total` was in the `slots:` line and `t0` was not — and said so: LLVM is
value-first (`store %v, ptr %p`), which is the nearest neighbour in any model's
training data. A probe settles it: `t0: int @ 0` is legal Heroes, so a source
binding named `t0` makes the dump ambiguous on input the author may write. Both
halves are fixed by notation rather than by care: **the destination is always left
of `=` or `<-`**, and temps and synthetic slots carry `$`, a character the language
does not contain.

**4. Three blocks in the sample have hidden exits, and the ergonomist missed them
on first read — which is the datum.** The spec says out-of-bounds aborts, overflow
aborts, division by zero aborts. So `index` and int `add` are not ordinary
instructions, and "a pass written against my reading is wrong and compiles". Every
abort-capable instruction now carries a **trailing `!`** in the dump, which also
distinguishes int `add!` from `f64` `add` — the one place the mark is a type
distinction rather than a warning.

**5. Block arguments lost a blind A/B, and the reason generalises.** Given two
label-stripped dumps of `abs`, the ergonomist answered both correctly and then
counted the cost: Variant 1 (slots) one re-read and one guess; Variant 2
(`^bb3(%3: i64)`, LLVM/MLIR spelling) three and three. The decisive one is not
verbosity: `%3`'s meaning *is* the set of jumps elsewhere in the function, so the
line cannot be read as a statement at all. Slots make that scan optional; block
arguments make it mandatory. `icmp slt` and `.i64` were noted as spending the most
expensive notation on distinctions Heroes cannot express — it has one integer type
and it is signed.

**6. The historian's decisive row points the same way, from the opposite
institution.** LLVM — the project with the strongest possible interest in SSA —
tells frontend authors not to build it: "we strongly recommend that you use this
technique [alloca + load/store] … unless there is an extremely good reason not
to", noting "clang uses this technique for local mutable variables". QBE: "phi
instructions are NOT necessary when writing a frontend to QBE". rustc MIR chose
non-SSA in RFC 1211 (2015-07-14) for diagnostics, borrowck and debuginfo — the
motives that match this project's — and has not migrated in eleven years, adding
an SSA *analysis* over the non-SSA IR instead. Every verified migration (GCC
4.0.0, 2005-04-20; Go 1.7, 2016-08-15) bought generated-code quality, which
CLAUDE.md §13 renounces. **No project was found that regretted a non-SSA IR for
correctness or diagnostic reasons.**

**7. GHC is not precedent for a desugared tree — it is precedent for a *check*.**
The historian's reading: Haskell's `HsSyn` is re-parameterised through parse,
rename and typecheck and desugared once into Core, and the payoff GHC states is
Core Lint — "an 100% independent check on the type inference engine". Rust's four
trees were each added late, per check (HIR for `for`-loop desugaring, THIR for
exhaustiveness and unsafety). So: keep the single pass, and buy the check
separately. **An IR verifier lands with the IR.**

**8. "Textual substitution on the AST before type checking" has no success
story.** rustc: "no monomorphized MIR is ever created"; monomorphisation is "the
first step in the backend". Swift, in its compiler lead's words: "there is no such
thing as an instantiation-time error … in stark contrast to the instantiation
model of C++ templates". And C++ itself is not textual substitution — two-phase
name lookup reached MSVC only in Visual Studio 2017 15.3, after roughly two
decades of conformance debt. The warden's finding is the reason this matters here
rather than in the abstract: **the milestone's own witness, design.md's appendix,
contains generic `apply`/`reduce`**, and monomorphisation is M6 — so refusing
generic bodies leaves M4 with nothing to prove itself against.

**9. `heroes build` that exits 0 having produced nothing is panel 016's ledger
regrowing its first entry.** The warden quotes that panel's watch list verbatim:
"`check` is not `build` … a model runs `heroes check` and reports success for a
program that has no backend yet." A silent no-flag `build` promotes the misreading
into the tool's behaviour, one milestone before the fix was scheduled — and
`cli.rs` currently tells the user, in two places, that `build` lands at M5.

**10. `assert` lowered without its operands would be written twice.** The
appendix holds six `test` blocks and ~25 `assert`s, some inside `match` arms; the
spec (line 157) and §4.18 both require the failure to show the source expression
**and both sides**. The spans have existed since M2. Lowering "branch + panic"
without them guarantees the same function is rewritten at M6 — §1.2's cost formula
turned on the compiler's own source.

## Disagreements, unsmoothed

- **The engineer vetoes point 5; the warden and the historian approve it.** The
  disagreement is not about the engineering — the engineer says so outright ("I am
  vetoing the *unamended spec*, not the engineering") — but about which artifact
  is normative. CLAUDE.md §12 settles it in the engineer's favour: spec beats
  compiler, so the compiler contradicting design.md §4.12 *is* a bug until §4.12
  is amended. The veto therefore lifts by amendment rather than by argument, and
  the engineer's real fear is separate and stands: ownership cannot decide whether
  a `Generic(0)` slot needs a `decref`, so the pass order must be written down.
- **The engineer says a desugared tree would cost ~450 lines; the warden says
  fusing the passes makes three documents false.** Both are right, and the
  resolution pays the warden's price in amendments rather than the engineer's in
  lines. The warden's third condition is the one with teeth: with no desugared
  tree to inspect, **the only evidence Part 5's table is honoured is a
  `--dump-ir` golden per row.**
- **The ergonomist and the engineer disagree about verbosity without knowing
  it.** The engineer's own condition is that if the appendix's dump exceeds 12 IR
  lines per source line, "the load/store verbosity has defeated the deliberate
  artifact claim"; the ergonomist accepted the format *because* every line is
  readable standalone. The measurement at M4 close arbitrates.
- **The ffi-pragmatist wants `-Wconditional-uninitialized` added; nobody else
  raised it.** It found a `ptr` slot stored only inside a loop, read after it:
  clang is silent under CLAUDE.md §7's flag set and the program segfaults. The
  flag is measured clean on the runtime and all four spikes. It is a §7 amendment,
  so it is queued for M5a rather than taken here.

## Resolution — provisional, author ratification pending

The most conservative resolution that lifts both vetoes. Nothing here waits for
the author: work proceeds on this default and the verdict is appended when given.

1. **Point 1 — accepted, one pass.** design.md Part 5's closing line ("gone
   *before* the lowering stage sees the tree"), §1.7's "one function in the
   frontend", and the ROADMAP's "erased in the frontend" are amended to say what
   happens: the sugar is erased **on the way into the IR**, and the IR is the
   evidence. **An IR verifier ships with the IR** (types agree, exactly one
   terminator per block, every slot declared, no fallthrough, no `Ty::Generic`
   past monomorphisation) and runs in tests — GHC's payoff without GHC's tree.
   **One `--dump-ir` golden per live sugar row.** The queued author exercise
   ("hand-desugar three constructs") is re-specified against IR text.
2. **Point 2 — accepted, slots, no phi, unanimous.** The mortgage is recorded: the
   post-fixpoint QBE backend consumes the slot form via QBE's documented non-SSA
   path, and if it ever needs phi insertion, the historian's prediction dies and
   this decision reopens. The ergonomist's **conditional veto stands against block
   arguments or phi in any form**, liftable only by measurement.
3. **Point 3 — amended, and this is where the panel changed the design.** The
   invariant is restated: **no instruction contains a nested expression**; operand
   counts are variable in exactly three forms (call, construct, print), held in a
   run arena. And the IR carries five things the proposal omitted:
   **linkage** on every callee and definition; **at most one destination** on a
   call (void calls have none); **a place** — slot plus field/index path — as a
   store's destination (§4.8's own `l.pos @ l.pos + 1`); **construction** as an
   instruction, not an expansion; an explicit **`cast`** for `str` → `cstr`, so
   §4.3's no-implicit-conversions rule stays visible in the dump. Abort-capable
   instructions are marked `!`. Variadic call sites are marked, because that is
   the one boundary clang verifies nothing at.
4. **Point 4 — amended.** `build` is a subcommand: the fixpoint invocation types
   it (`build selfhost/heroes.hero --emit-c -o`), so CLAUDE.md §10 is satisfied by
   the verb. The **no-flag form is not silent**: it lowers, verifies, writes one
   line to stderr naming what happened and what does not exist yet, leaves stdout
   untouched, exits 0. `cli.rs`'s `retired()` and help footer are corrected in the
   same commit.
5. **Point 5 — accepted, veto lifted by amendment.** design.md §4.12's "textual
   substitution on the AST before type checking" and Part 5's `generics` row are
   amended in the same commit as the code; the **pass-order invariant is
   written down**: monomorphisation is an IR→IR pass that runs *before* the
   ownership pass and the emitter, and both assert no `Ty::Generic` survives. This
   inverts nothing in the ROADMAP's order but makes M6's obligation explicit, and
   M4's generic-body goldens are **pre-registered as rewritten at M6** so the churn
   is not read as a regression.
6. **Point 6 — accepted with the warden's condition.** `test` lowers as a
   zero-argument function; `assert` lowers to a branch plus a runtime call
   carrying **the source text and, when the asserted expression is a comparison,
   both operand values**. Part 5's `test` row is amended. M5a pins that emitted C
   excludes test functions unless invoked through `heroes test`. The runtime's
   panic signature is decided here, not at M6.
7. **Point 7 — accepted with every condition, because they are all one
   character or one line.** The dump's grammar is written down; it is
   **deterministic and golden-tested, explicitly not version-stable** (LLVM's own
   stance on `.ll`: "there are no specific promises"); a double-print diff test
   lands with it, the cheap analogue of §7's double-emit test. Notation:
   destination always left of `=` or `<-`; `$` on temporaries and synthetic slots
   (`$t0`, `$i0`), a character no Heroes program contains; `branch $t -> bb2 else
   bb4`, polarity in the text; `preds` on every block; `!` on abort-capable
   instructions; headers and blocks in **source order**, never sorted. **CI greps
   every `check/*.expected` and every runtime abort string for `$`**: a hit fails,
   which is what makes R1 tested rather than intended.

**R1 adopted as a rule of its own** (the ergonomist's second question, and it
turned out to be already-written spec): every exit-0/exit-1 diagnostic speaks in
the syntax the author wrote; **IR vocabulary appears only in `--dump-ir` and in
exit-2 internal errors**. The spec already forbade R2 for `assert` (line 157) and
`???` (line 160). R2's measured cost is not verbosity: for the sample program it
leaves two reachable repairs, `-> str?` (correct) and `.must()` (compiles, and
silently converts propagation into a runtime abort) — the exact failure this
language exists to prevent.

**What a veto would compel, on the record.** Engineer's veto on 5, if the
amendment were refused: monomorphise before lowering, which at M4 means M4 has no
witness. Warden's scoped veto on 4, if the silent form shipped: remove the no-flag
form, `build` requires `--dump-ir` until M5a. Ergonomist's conditional veto, if
block arguments are ever adopted: reject the dump format outright, liftable only
by its experiment 3 returning ≤1.2×. Ffi's veto, if the instruction set shipped
closed as listed: §4.19's ladder steps 1, 3 and 4 unwritable.

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | The ownership pass is **under 250 non-test lines** and contains **no dataflow fixpoint** — no `while changed`, no live-in/live-out sets. Over 400 lines or a liveness fixpoint ⇒ slots did not buy what was claimed and SSA reopens before M5c | M5b close |
| compiler-engineer | The appendix's `--dump-ir` stays **under 12 IR lines per source line**; over it, the deliberate-artifact claim has failed and the dump needs an elided rendering | M4 close |
| llm-ergonomist | `heroes build probe-collision.hero --dump-ir` produces at least one line where the same spelling denotes a slot in one operand position and a temp in another, indistinguishably — **unless the sigil ships** | M4 (today, no model needed) |
| llm-ergonomist | On ≥20 dumps, `-> A, B` polarity inverts at **≥15%**; `-> A else B` at **<2%**. Under 5% for the unlabelled form falsifies it | metric 2 |
| llm-ergonomist | Block-argument dumps produce **≥2×** the wrong-or-refused answers of slot dumps on ≥3-predecessor joins, concentrated on wrong-predecessor attribution. ≤1.2× falsifies — and lifts the conditional veto | metric 2 |
| llm-ergonomist | R2 first-try repair rate lands **≥25 points below** R1, and R2 produces compiles-but-changes-semantics repairs in **≥20%** of cases against R1's ≤5% | metric 2 |
| spec-warden | At M4 close `heroes measure` prints **max 2139, delta 0**, and the milestone edits **≥4** design.md/ROADMAP sites plus **2** `cli.rs` strings. Spec movement ⇒ a smuggled surface change and this verdict is void; fewer than 4 design edits ⇒ the compiler now contradicts the design document | M4 close |
| ffi-pragmatist | With linkage + optional destination, **SQLite binds with exactly one shim** (`hero_sqlite_open`, for `sqlite3_open_v2`'s out-parameter — Heroes has no address-of); `_prepare_v2`, `_step`, `_column_int`, `_finalize`, `_close` need **zero** shim lines. Any second shim for a non-out-parameter reason falsifies it | M7, ladder step 3 |
| ffi-pragmatist | **Without** the linkage field, the first Heroes top-level function named `open`, `read`, `write`, `close`, `index`, `error`, `time`, `div`, `remove`, `send` or `link` produces a green `heroes test` and a wrong binary | M8a |
| historian | **No phi nodes and no SSA construction** are needed to reach the M8c fixpoint, and the post-fixpoint QBE backend consumes the slot IR without an SSA-construction pass. Phi or def-use chains entering the IR to make the ownership pass work kills it | M5b, conclusively post-fixpoint |
| historian | **No second tree before v1** — a desugared HIR-equivalent arrives only if a check cannot be written on the AST or the IR (rustc's THIR trigger was exhaustiveness, which M3c already did on the AST) | M6 |

## Watch list

- `heroes measure` vendors two of panel 011's three tokenisers; o200k was the
  binding one on v0 by 2 tokens. Vendor it before any verdict lands within 10
  tokens of a ceiling.
- The ROADMAP records 2136; head measures **2139**. Panel 018's sweep cost +3
  measured against a −4 claim: shape-attributable deltas measured on v0 did *not*
  transfer to v1, and the DESIGN-LOG asserted they would.
- `-Wconditional-uninitialized` and `-Werror=format` for CLAUDE.md §7 at M5a: the
  first closes a measured segfault clang is silent about today, the second turns
  `-Wformat` into the compile error §4.19 promises.
- No diagnostic in this compiler decodes a literal yet (`lexer/escape.rs`'s
  `unescape` is called only by the lexer; nothing parses an `int` outside tests).
  An out-of-range `int` literal is therefore a **new diagnostic class landing in
  M4** — CLAUDE.md §4 makes that a panel trigger of its own.
- `???` in a file that reaches `build`: design.md does not cover it. §4.16 says
  such a file "type-checks everything else but produces no binary"; what `build`
  and `--dump-ir` do with it is undecided and is being decided in M4 by default.
- Non-format variadics (`TextFormat`, `sqlite3_mprintf`) are the one boundary
  where clang verifies nothing — measured, no diagnostic, wrong output.
- No evidence exists, anywhere, that a three-address text IR is readable by
  *people*: every claim in the literature is a designer's assertion. Meta's LLM
  Compiler (546B tokens of LLVM-IR) shows only that a *trained-on* IR is tractable
  to models. Heroes' dump is novel and read zero-shot; metric 2 is the only
  instrument that will ever say.

## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The author's instruction was a blanket one — *"ratifica anche tutto
quello che c'è da ratificare"* — given after reading the session summary, not a
clause-by-clause review of this file. It is recorded that way on purpose: this
project's own rule is that a record must not say more than what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here had been
load-bearing since the day it landed, so this changes the record's status rather
than the compiler's behaviour.

What it does **not** settle: anything this file keys to a measurement that has not
been taken. Those stay open on their own terms, listed in `docs/debrief/QUEUE.md`,
and a blanket yes cannot make a number arrive.
