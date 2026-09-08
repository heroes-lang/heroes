# Panel 118 — `_ =` stops swallowing a failure, and the valve nobody had to invent

**Sat** 2026-09-08 · **lane** full panel, five judges · **milestone** M-discard-refusal
(`docs/ROADMAP.md` row 39) · **verdict** provisional — author ratification pending

Convened because both halves are language (CLAUDE.md §4): a clause at `spec:98-99`
and a diagnostic class. The author's verdict on the *rule* was already given
(`/decide` answer `3a`, 2026-09-03, `DESIGN-LOG.md:541`); what the sitting was for
is the shape.

## The proposal, verbatim as it went out

> `_ = e` where `e`'s type is a `T?` becomes a compile error: the one place a
> failure can be dropped without the type system noticing stops being silent.
> Today `_ = risky(0 - 1)` compiles and the program exits 0 having swallowed the
> error. Panel 003 made a non-`()` expression alone on a line an error and named
> `_ = e` as the deliberate discard; this closes the hole that discard left. The
> spec clause goes at `spec:98-99`. The diagnostic is a new code with a `certain`
> fix where one exists and a `guess` otherwise.

Four questions were put: (1) what the escape hatch is for a deliberate
best-effort discard; (2) whether the rule reaches a type parameter; (3) the
diagnostic's code and when its fix is `certain`; (4) whether the code joins
`is_thesis_rule` so `check --permissive` drops it.

## What was measured before the briefs went out

Method, the record's own: strip `^( *)_ = ` from every discard in a copy of the
tree, then read the type the compiler itself names in `discarded_value`
(`heroes check --json`, whose output is on **stderr** by the contract at
`.claude/rules/cli-surface.md`). Deduplicated by file:line.

| tree | discard sites | of which fallible |
|---|---|---|
| `selfhost/` | 238 | **53** |
| `tests/harness/` | 14 | **6** |
| `examples/` | 47 | **1** |
| **total** | **299** | **60** |

Reproduced independently by the compiler seat and by the FFI seat. The record's
2026-09-03 figures were 269 sites and 61 fallible; `examples/` has grown 18 → 47
and one `selfhost/` site moved. **36 of the 60 sit inside `test` blocks** and 24
are production code; of those 24, **10 sit in a function that is already
fallible**, so a `?` is their repair and it improves them.

Spec budget on the day: **3903** of a hard 4096, with `FFI_FLOOR` mortgaging 60,
so `tests/harness/suite_spec.hero:203` goes red at 4036 and the real room is
**132** tokens. `selfhost/check/walk.hero` measures **1695** code lines against a
**1700** DECIDED entry: five lines, in the one file every new diagnostic reaches.

## Verdicts

| seat | verdict | section | cost / delta | condition |
|---|---|---|---|---|
| compiler-engineer | **yes**, object on a new built-in, **veto** on refusal at `mono` | §1.7, Part 5; §1.3 against a callee summary | ~50 lines, 4 frontend files, 0 IR shapes, 0 Part 5 rows | no `certain` fix; de-certify `discarded_value`; close the generic hole at the body; test the type before the name text |
| llm-ergonomist | **yes with condition**, three clauses or it objects | the thesis, read from `spec/` alone | — | the rule reads the discarded expression's static type; a named hatch; the generic limit stated |
| spec-warden | **yes with condition** | §1.6, §1.2, §1.4; Principle 0 | W4g **+15**, W4h +20, W1 +39, W2 +50, W3 **+59** | land ≤ +20 by its own reckoning; de-certify `discarded_value`; `.must()` never `certain`; re-measure at the landing commit |
| ffi-pragmatist | **yes with condition**, objects without a hatch | §4.19 (`()` is a type, not a value), on §1.11 | ABI untouched; 0 of 17 fallible FFI discards are on `extern` declarations | a one-line drop whose result is `()`, or an equivalent |
| historian | **approve**, advisory | precedent | — | evidence Zig relaxed its rule, or a valve rate above ~25% in a real corpus, would flip it |

## The disagreement, and the measurement that ended it

Two seats wanted a new built-in and two priced obstacles to it. Both sides were
arguing under a different branch of Principle 0 and both were right inside their
own branch.

**For a named hatch.** The ergonomist ran the experiment on itself: blocked at
`_ = declare(name)`, it went to § Failure's table and wrote
`_ = declare(name).default(0)` — which compiles, invents a sentinel with no
meaning, and is a worse program than the one it was refused. Its other reachable
repair was `.must()`, which on `state.declare` would abort the compiler on any
input program that merely contains a name error. The historian added that
`_ = f().is_err()` is **not auditable**, since grep cannot tell it from a
legitimate predicate test, and that design.md:1117 already calls `.is_err()`
*"the weakest member, first to cut"* — making it the sanctioned idiom would weld
it in.

**Against.** The warden measured `.ignore()` at **+59**, the dearest of every
candidate, and argued it re-blesses the defect class: the cheapest repair to the
new error becomes `.ignore()`, reproducing today's survivors eight characters
longer. The engineer measured the harder obstacle by running the instrument
rather than reasoning: `selfhost/check/builtins.hero` is at **374 of a DECIDED
374**, and one added constant produces

```
selfhost/check/builtins.hero: 376 lines of code, past the 374 it measured
when this check was written — a file already over the ceiling may not grow further
```

so the file where every built-in's type rule lives cannot take a new built-in at
all. Beyond it: `selfhost/inventory.hero:131` pins `entries[12].name == "is_err"`
over a table whose order is identity; `fallible_operation` returns a slot id and
`()` has no slot (panel 017 C); a Part 5 sugar row is taxed one `--dump-ir`
golden; and the DECIDED raise is a second sitting.

**Then the premise both sides shared turned out to be false.** The engineer's
*"the existing vocabulary reaches all 60 sites"* and the warden's *"both check at
exit 0"* were measured with `heroes check`. The FFI seat measured a different
gate, and the coordinator reproduced it on the real tree with the shipped
compiler:

```
_ = risky(1).is_err()        -> warning: variable 't6' set but not used [-Wunused-but-set-variable]
_ = unit_risky(1).is_err()   -> warning: variable 't6' set but not used
unit_risky(1).must()         -> no warning
```

So the one-line hatch everybody had assumed emits a C warning **at every payload
type**, and `tests/harness/suite_warnings.hero:24` says of itself *"THERE IS NO
EXEMPTION"*. The FFI seat priced the corpus consequence: rewriting two
`examples/ledger/db/sqlite.hero` wrappers to the `()?` that same file already
uses for `run`, `reset` and `bind_*`, then applying `.is_err()` at 15 sites,
takes the `warnings` suite from **162 passed / 0 failed to 161 / 1**. The engineer
withdrew that half of its own verdict.

The option set, all of it run on the real tree on 2026-09-08:

| deliberate drop | lines | C warning | aborts | legal in `main` / a `test` |
|---|---|---|---|---|
| `match` with **block** arms | **5** | none | no | yes |
| `_ = f().is_err()` | 1 | **one** | no | yes |
| `_ = f().is_err().to_str()` | 1 | none | no | yes |
| `.must()` | 1 | none | **yes** | yes |
| `f()?` | 1 | none | no | **no** — `try_in_infallible` |
| `.default(v)` | 1 | none | no | only where the payload is constructible, never `()?` |

The inline `match` arm `.ok _ => _ = 0` is refused with `declaration_in_arm`,
whose own message says *"or open a block"* — so the `match` route costs five
lines and not the three the FFI seat estimated. `.default(())` is
`expected_expression`, a parse error on §4.19's own ground. `.must()` is not a
neutral choice: the FFI seat measured `sqlite3_close` answering `SQLITE_BUSY`,
which today prints the program's own reason and `exit(3)` and under `.must()`
becomes `panic` and **exit 134**, the program's exit code and message lost.

**The route nobody had listed.** The warning is not the language's, it is the
emitter's: `selfhost/emit/unread.hero:38-46` puts `.binary` in the group that may
not lose its destination, and the module doc gives the reason — *"Index/Cast/
Unary/Binary can ABORT and their emitters print the check WITH the assignment"*.
Sound for arithmetic, for `/` and `%`, for the shifts, and for `< <= > >=` on a
float. **Not** for `eq` and `ne` on a scalar, which print an operator and nothing
else. Sent back to the compiler seat as a question rather than settled by
symmetry, it returned the enumeration the code believes, read from
`selfhost/emit/operator.hero:61-216` over `ir.BinaryOp`'s 16 cases, and corrected
the coordinator's list three times: the orderings abort **only on `f64`**, so the
safe set is per (operator, operand type) and not per operator; `&&` and `||` are
not `ir.BinaryOp` cases at all, since they lower to branches; `!` is a `.unary`,
whose sibling `neg` does abort. And it named an abort the coordinator had not
thought of: `.eq` on an **aggregate** prints a runtime call, and two of those
carry checks — `hero_array_require` at `runtime/parts/array.c:238-239`, and a
`partial` record's generated `_eq`, which is a deliberate `hero_panic` at
`selfhost/emit/structural.hero:53-56` whose own comment says *"'should never' is
exactly the premise that goes in the loud direction"*.

## Resolution — provisional, author ratification pending

The most robust and complete resolution, not the cheapest and not a compromise
(CLAUDE.md §4, `docs/contract/case-law.md` CL-040). The author's standing
instruction of 2026-09-08, *in doubt prefer robustness to token economy*, is
applied where it bites: at R1's three positions and at R6's wording.

**R1 — the rule closes three positions, not one.** `_ = e` where `e`'s static
type is a `T?`; a `_` **parameter** whose declared type is a `T?`; and `_ = x`
where `x`'s type is a type parameter. The second and third are holes of the
identical class and both were found by running, not by reading: the compiler seat
measured `function swallow(_: i64?)` with `swallow(risky(0 - 1))` checking and
running at exit 0 with the error gone, and the coordinator reproduced it. Both
cost **zero migration sites** — the tree holds no real `_` parameter (the single
grep hit is a comment in `tests/harness/suite_layout.hero:166`) and none of its 17
generic declarations contains a discard, `selfhost/library_source.hero` included.
Stating a refusal beats stating a gap, which is the warden's phrase for it.

**R2 — the refusal reads the static type of the discarded expression.** So
`_ = f()?` stays legal, and so does `_ = f().is_err()`. This is the ergonomist's
first clause and it is load-bearing: without it the rule is undecidable from the
sentence, and a reader's wrong guess costs either a spurious error on a common
correct line or an accepted drop.

**R3 — no fix is `certain`, and the reason is measured.** The same line's three
repairs give three different answers:

```
_ = risky(0 - 1)?            -> 1
_ = risky(0 - 1).is_err()    -> 9
_ = risky(0 - 1).must()      -> panic: .must() on an error
```

`?` inserts a control-flow edge, so it is not meaning-preserving;
`.is_err()` is meaning-preserving and preserves the bug, so certifying it would
make `--apply` automate defeating the rule; `.must()` aborts. A rule that exists
to make a decision visible must not ship a repair that takes the decision. Three
`guess` fixes, each naming its consequence, and `.must()`'s names the abort.

**R4 — `discarded_value` is de-certified on a fallible type in the same
commit.** This is the third half of the proposal and it was in nobody's owed
list. `selfhost/value_errors.hero:131` gives that diagnostic a `.certain` fix
whose replacement is `_ = `, and `heroes check --apply --in-place` on a bare
fallible statement was run and writes exactly

```
    _ = write_file(path: "build/probe4.txt", text: "hi")
```

which the new rule refuses. Found independently by the coordinator, the warden and
the FFI seat. It owes a `tests/golden/check/` `.fixed` case: the compiler seat
measured that `grep -l "_ = "` over all **7** `.fixed` files on disk returns
nothing, so no instrument watches that path today, which is panel 071's failure
repeated.

**R5 — the deliberate drop is `_ = f().is_err()`, and the narrow emitter
relaxation is what makes it clean.** A `.binary` may lose its destination when
the operator is `eq`, `ne`, `bit_and`, `bit_or` or `bit_xor` **and** the operand
type is a scalar. Not the orderings, which panic on a `nan`. Not aggregates,
whose `_eq` helpers carry the two checks named above. Measured: ~14 lines in
`selfhost/emit/unread.hero`, which is at **183** code lines against `CEILING`
300 and has **no** DECIDED entry, so it is the one file in that neighbourhood
with room; `selfhost/emit/inst.hero` stays at exactly **350 of a DECIDED 350**
because `c` and `f` are already in scope at the call site, so its line is
**edited and not added**; `selfhost/ir.hero` stays at **310 of 310**, untouched.
`vanishes_when_unread` already returns `true` for `.binary` and needs zero new
lines, only the same type-awareness. **No golden's bytes move**, established
three ways: no golden discards a comparison, `tests/golden/` holds no blessed C,
and `unread.hero`'s callers all run after lowering so `--dump-ir` cannot change.
The emitted C for `_ = f().is_err()` becomes the call plus its ownership traffic,
byte for byte what `_ = f()` emits: four dead assignments and four declarations
disappear. **This is a defect repair inside this sitting's subject and not
architecture** — nothing changes what a program means, and `unread.hero:10-16`
records this as a recurring class and names this repair, with
`tests/harness/suite_warnings.hero:18-22` recording it a second time. It owes its
own commit, a `DESIGN-LOG.md` line and a `tests/golden/run/fixedbugs-` case.

**R6 — the spec clause names all three positions and says what counts**, at a
measured **+49** against 132 tokens of room, leaving 83. The warden's own W4g
would have cost +15 and covered one position; robustness over token economy is
the author's standing instruction and the difference is 34 tokens. Wording, to be
re-measured at the landing commit:

> A `_` never drops a `T?` — not `_ = e`, not a `_` parameter, not a type
> parameter — but the type discarded is what counts, so `_ = f()?` is fine.
> Answer the error.

**R7 — what the rule does not reach is pinned by goldens, not by spec tokens.**
A fallible wrapped in a container or a record survives: `_ = [f()]` and
`_ = R(field: f())` have types `[T?]` and a record, neither of which is a `T?`.
This is the historian's prediction and it is the wrapping evasion every precedent
in its table has — Rust's `#[must_use]` is evaded by `(f(),)`. It is an evasion
and not a mistake, so it buys no spec sentence; it gets two `tests/golden/check/`
cases so the behaviour is pinned and a later session does not close it by
accident. The compiler seat's second measured hole, a fallible stashed in a
container and never inspected (`xs: [i64?] = [risky(0 - 1)]` with only `len()`
read, exit 0), is a **different class**: it needs reachability, not a type
judgment, and it is filed rather than closed.

**R8 — the new code is a thesis rule.** It joins `is_thesis_rule`'s list in
`selfhost/diag.hero:87-103`, beside `discarded_value` and `bound_unit`, so
`check --permissive` drops it and stays the control arm design.md Part 11 needs.

**R9 — `_ = m[k]` refused is right, not collateral damage.** Zero such sites
exist in the tree; a map read is pure, so discarding it is a dead line; and
`.must()`, `.default()`, `?` and `if !m[k].is_err()` all remain.

### What conservative would have been, recorded so the author can choose it

- **The clause at +15** (`_ = e` refuses a `T?`: answer the error), covering the
  statement only, with the `_` parameter and the type parameter left open and
  unstated. Cheaper by 34 tokens and it leaves two measured holes running at
  exit 0.
- **`match` with block arms as the sanctioned drop**, at five lines per site and
  no emitter change at all. It is warning-free and preserves exit codes; it costs
  five lines and an invented no-op arm per site, and it is what the milestone
  would ship if the author rejects R5.
- **The `ignore` built-in**, which two seats wanted and two priced out. It is not
  refused on judgement: it is blocked by `selfhost/check/builtins.hero` at 374 of
  374, and raising a DECIDED entry is a sitting of its own. If the author wants
  the named, greppable form the ergonomist and the historian argued for — and the
  historian's naming ruling is that `ignore` is the least wrong of the four
  candidates, being Midori's own keyword for this exact job, where Rust's `drop`
  imports destructor timing and Haskell's `void` returns a still-wrapped value —
  the route is that ceiling raise, and it is queued rather than closed.

### The departure, recorded as deliberate

**Hare went the other way seven months ago.** The historian verified that Hare
0.26.0, released 2026-02-13, introduced `_ = os::remove("/some/file")` as *the*
explicit way to ignore an error, deliberately replacing a cast-to-void
workaround, with the stated reason *"This makes the fact that you're ignoring the
error here more explicit."* Heroes is about to make that same expression an
error. Hare is the one shipping language with mandatory error handling, so this is
the sharpest counter-precedent available and it is recorded as a choice rather
than discovered later. Zig ships the rule Heroes is adopting — `_ = f()` on an
error union is refused, with a note suggesting `try`, `catch` or `if` — and has
not retreated from it, though its own 0.10.0 release notes call the sibling
unused-locals rule *"a divisive feature"* whose promised autofix slipped the
release. And **no language was found that made ignoring an error a hard error with
no valve at all**, across seventeen searched; the negative rests on that list.

## Predictions to score

| # | seat | claim | instrument | scored at |
|---|---|---|---|---|
| 1 | spec-warden | `heroes mutate examples --survivors` reports **0** `drop-question` survivors, against 9 of 84 in `docs/measurements/014-mutate-over-thirty-five.md:169`, and the spec reads ≤ 3923 | `heroes mutate --survivors`, `heroes measure` | M-discard-refusal close |
| 2 | spec-warden | ≥ 20 of the repaired sites are `test`-block fixture writes repaired to something that asserts; if more than 6 of the ~60 repairs are `.must()`, §1.2's saving was spent turning a swallow into an abort | the diff | M-discard-refusal close |
| 3 | compiler-engineer | the `warnings` suite reports 162 / 0 with all 60 sites on `_ = f().is_err()`; `emit/unread.hero` ≤ 200 code lines, `emit/inst.hero` exactly 350, `ir.hero` exactly 310 | `warnings`, `layout` | M-discard-refusal close |
| 4 | compiler-engineer | `suite_fixes.hero`'s `LEAST` has risen from 6 to ≥ 7, with a `.fixed` golden whose source carries a fallible-valued bare line | `suite_fixes` | M-discard-refusal close |
| 5 | ffi-pragmatist | step 3 of §4.19's ladder needs no shim: `examples/sqlite/main.hero`'s 4 discards are all `i64`, so the file compiles unchanged. Falsified if any line of `examples/sqlite/` needs editing | the diff | M-discard-refusal close |
| 6 | historian | `_ = [f()]` and `_ = R(field: f())` compile with no diagnostic, because their types are not a `T?`. If both are refused, the rule is not a type judgment on the discarded expression and reaches deeper than any precedent in its table | two `tests/golden/check/` cases | M-discard-refusal close |
| 7 | llm-ergonomist | given the rule and no named hatch, ≥ 40% of repairs for a best-effort discard are `.must()` or `.default(<sentinel>)`, behaviour-changing and compiling | a written trial | when the author next runs one |

## What the sitting found that nobody asked it for

- **`heroes measure` overstates the spec's usable room by 60 tokens.**
  `selfhost/cli/measure.hero:157` prints `CEILING - highest` and knows nothing of
  `FFI_FLOOR`, while `tests/harness/suite_spec.hero:203` goes red at
  `SPEC_TOKENS + FFI_FLOOR >= CEILING`. Two instruments in one repository report
  different room for one document, and the one a session reaches for first is the
  looser. Filed as a defect.
- **`docs/work/SCHEDULED.md`'s census is stale**: 269 discard sites and 18 in
  `examples/`, against 299 and 47 today. Corrected with the item.
- **design.md does not say whether a `certain` fix may preserve meaning while
  preserving the bug.** `.claude/rules/diagnostics-and-goldens.md:18` and panel
  071 both address a fix that *changes* meaning; `.is_err()` is the inverse and is
  unaddressed. Ruled against here on §1.2, and the gap is named rather than
  papered over. Filed.
- **design.md does not say whether a runtime `hero_*_require` null rejection is a
  program abort or a compiler tripwire.** Part 5 calls `ptr == NULL` *"the one
  non-value that every runtime entry point rejects"* without saying whose bug it
  catches. R5 excludes aggregates on the assumption that it is a check worth
  keeping; if the author rules those rejections compiler-internal, the exclusion
  can be dropped and the relaxation becomes type-independent, two lines cheaper.
  Filed.
- **The spec exhibits no diagnostic text anywhere**, which the ergonomist
  reported as an appetite rather than a request: a language whose thesis is that
  every plausible mistake is a compile error shows its reader zero compile
  errors. And **`.must()`'s abort is described in three words** with no
  consequence attached, which is what made it the attractive repair in that
  seat's own trial. Both filed.

## Author's verdict

**Pending**, queued as `- [ ] **panel 118**` in `docs/work/DECIDE.md`. Work
proceeds on the resolution above; the verdict is appended here when given, with
follow-up work if it is overturned.

What a `yes` would settle: that the rule closes three positions and not one
(R1), that the refusal reads the discarded expression's own static type (R2),
that no repair ships as `certain` (R3), that `discarded_value` is de-certified on
a fallible in the same commit (R4), that the deliberate drop stays
`_ = f().is_err()` and is paid for by the narrow emitter relaxation rather than
by a new built-in (R5), and that the clause spends 49 tokens naming all three
positions rather than 15 naming one (R6).

What a `yes` would **not** settle: the `ignore` built-in, which two seats argued
for and which is blocked by a ceiling raise that is a sitting of its own; the
three findings filed beside the ratification in `docs/work/DECIDE.md`; and the
class the compiler seat measured and this rule does not reach, a fallible stashed
in a container and never inspected, which needs reachability rather than a type
judgment.

What a veto would compel: the ergonomist's veto fires only on a **non-local**
diagnostic, so R1's third position must stay a refusal at the generic body and
never a ruling at the call or in `mono` — the compiler seat holds the same veto
from the other side, on the ground that a frontend refusal must not buy an IR
field. The FFI seat objects, and does not veto, where no one-line drop exists;
R5 is what answers it, so overturning R5 revives that objection and the fallback
is `match` with block arms at five lines a site.

## Process notes

The tree was frozen from the moment the briefs went out until this file was
written, the follow-up to the compiler seat included. Every seat built in
`/private/tmp/.../scratchpad`, none wrote to the repository, and the two
deliberate red runs that priced `builtins.hero` and `walk.hero` were reverted in
the copy. Another session committed during the sitting, moving HEAD from
`2103c2cb` to `8ffb6a4a`; nothing of that session's work is in this sitting's
commit.

Two seats measured the wrong gate and one withdrew a verdict when shown the
right one. That is the differentiated-input design working, not failing: five
seats reading the same brief would have agreed at `heroes check` and shipped a
hatch that fails the net.
