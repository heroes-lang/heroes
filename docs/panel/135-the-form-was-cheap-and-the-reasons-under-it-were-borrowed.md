# Panel 135 — `alias`: the form was cheap, the reasons under it were borrowed, and the boundary asked for the other door

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 1 · **status**
`provisional — author ratification pending`

**Lane: full five seats.** The form has surface (a sixth top-level declaration),
spec text (one sentence and one production in § 4) and a diagnostic rule (an error
writes the alias with what it stands for), so every seat's input differs: the
ergonomist read two specifications blind, the warden measured eleven drafts, the
engineer priced two routes in a copy of the tree, the FFI seat compiled the C a
binding would need, and the historian went to the sources design.md item 5 cites.
The gate CLAUDE.md § 4 asks once per milestone was the `/step` that opened this
milestone: its own file says *the steps are the sittings*, so the yes to the step is
the yes to the sitting, and this line is where a reader who disagrees finds it.

## The item, as design.md Part 7 carries it

Item 5 (`docs/design/design.md:2705-2714`, written 2026-08-04): *`alias` — `Env =
alias` / `{str: i64}`. Deferred on the strongest possible ground: reversibility. It
is the only purely additive thing in the language … Precedent: Go shipped in 2012
and added transparent type aliases in 1.9, in 2017. Five years, nobody died. When
added, make it transparent (not a new type, fully interchangeable) and make errors
expand the alias (`expects: Env = {str: i64}`) … Call it `alias`, not `type` …
Keeping `type` free also leaves the door open for v2 distinct types.*

The ledger this milestone delivers gives every such item one of three dated
verdicts: **enters** (its own milestone), **refused** (a Part 6 row with the program
or compiler fact that would make it wrong, CLAUDE.md § 12), or **deferred again with
a return condition** written as a falsifiable claim (`docs/work/milestones/M-deferral-ledger.md`).

## The proposal, as it went out

A spec diff to § 4, Top-level declarations:

```
 Every top-level line starts with its kind. A `constant`'s name takes `: type`;
 a `function`'s parameter list attaches to its name; `record` and `variant`
-declare types, so nothing follows their name.
+declare types, so nothing follows their name; `alias Env = {str: i64}` names a
+type it does not create: `Env` and `{str: i64}` are one type everywhere, and an
+error writes the alias with what it stands for, `Env = {str: i64}`.
…
     Declaration = "constant" ident ":" Type Block
+                | "alias" ident "=" Type NEWLINE
                 | "function" ident [ Generics ] Params [ "->" Type ] Block
```

The `Extern` group's `Member` production (§ 13) is not extended: an alias is a
top-level declaration outside any group. The spelling is `alias Env = …` and not
item 5's own `Env = alias …`, because § 4's first sentence, *every top-level line
starts with its kind*, is a rule the form has to obey rather than a habit.

## What the coordinator measured before the briefs went out, 2026-09-13

| fact | number | how |
|---|---|---|
| spec today | **5655** vendored · **7531** real (`claude-opus-5`, pinned 2026-09-12) · ceiling 8192 · FFI floor mortgages 60 → 7591 stands against it | `./heroes measure spec/heroes-spec.md` |
| the diff above | **5723** vendored, **+68** · real *unrun* (a draft has no pin, and `--refresh` is never run in a sitting) | `./heroes measure` on the draft |
| a shorter wording (*names a type without creating one: the two spellings are one type, and an error writes both*) | **5699**, **+44** · real *unrun* | same |
| `alias` as an identifier in `.hero` code | **0** hits in `selfhost/`, `examples/`, `tests/golden/` (15 files carry the word in comments and test titles only); **not reserved** in `selfhost/keywords.hero` | `grep -rn '\balias\b' --include='*.hero'` |
| function types spelled in `selfhost/` | **68**; the most repeated `(function(A) -> B)` **9×**, `(function(i64) -> i64)` 6× | `grep -ohE '\(function\(…' selfhost` |
| map types spelled in `selfhost/` | `{i64: bool}` **77×** (6 tokens, at most 8 in one file), `{i64: i64}` 25×, `{str: str}` 18×, `{str: i64}` 16×, `{str: {str: bool}}` 8× | same |
| nested arrays | `[[i64]]` **10×** | same |
| the longest repeated spelling in `examples/` | `(function(value.Request) -> value.Frame?)`, 43 characters, 12 tokens, **6×** in `examples/interpreter/run/expr.hero` and once in `run/eval.hero` (a seventh hit is a comment) | same |
| exhaustive matches a sixth `DeclKind` would extend | **130** arms naming `.test_decl` in **52** files; the engineer confirmed the count and added **31** `TypeRef` arms in 15 files | `grep -rn '\.test_decl' selfhost` |
| the compiler compiles itself with no alias | yes — so the form is not on the §1.0 closure list | the fixpoint, 2026-08-18 |

## The measurement that decided the sitting

**Neither branch of Principle 0 holds, and every seat that measured said so in its
own unit.** Not on the closure list: zero uses. No measured Part 11 effect: metric 3
(`heroes mutate`) is blind to a transparent alias by construction — same type, same
kill rate — and metrics 2 and 4 have no instrument (grep of `selfhost/cli` and
`tests/harness` for a first-try or turns-to-green runner: empty). The token case, run
rather than argued: the warden rewrote `examples/interpreter/run/expr.hero` with one
alias for its six copies of a 12-token function type and measured **3326 → 3278
cl100k, −48, 1.4%** of that file, the strongest case in the tree; the most repeated
function type in `selfhost/` is generic and cannot be aliased; the most repeated
spelling of any kind is six tokens long. design.md §1.0: *neither, and it waits,
regardless of elegance.*

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| `compiler-engineer` | **object**, no veto (no core construct is added: interning makes transparency one arm) — recommends DEFERRED AGAIN | §1.7 (removes no special case, moves nothing to sugar), §1.1 (the tree's DECIDED ceilings), §1.12 | **no prototype compiled** (a change to `selfhost/` is testable only through the 20-minute rebuild the brief forbids), so every line figure is a counted estimate by analogy, marked (est.). The one route the seat would accept, R2a — a sixth `DeclKind` whose name the resolver rewrites to its target so `check/lower.hero` interns one id: **~140–180 code lines (est.)**, of which a new alias-cycle walk ~80 (a new module beside `selfhost/resolve/cycles.hero`, which is at 331/331 and is the constant walk; named `resolve/aliases.hero` by the seat on 2026-09-13, unwritten), `parse/decl.hero` ~16, `check/render.hero` +10, `check/lower.hero` +6; **130 `DeclKind` arms in 52 files and 31 `TypeRef` arms in 15 files edited**; its owed half lands on `print/fmt.hero` (**room 3** of 1150), `ast.hero` (**room 5** of 505) and `check/walk.hero` (**room 11** of 1870). It **deletes nothing** in `selfhost/`. Robustness finding: `T??` is refused by the parser today (`parse/type.hero:55`) and `check/table.hero:66-67` relies on it; `alias M = i64?` then `M?` retires that invariant in silence unless the checker re-earns it | if `alias` enters, at its milestone's close either `fmt.hero`'s DECIDED row reads above 1150, or `selfhost/print/` holds one more module than at `4e9520eb`, or the arm landed uncommented at exactly 1150 — scored by `-- ./heroes layout` | adopt-with-condition on (a) a compound spelling ≥ 30 characters at ≥ 20 sites in one program (today 7 at 43) or (b) a Part 11 run attributing a first-compile difference to a repeated spelling; conditions: route R2a, the cycle walk in the same step as the parser arm and before any `type_uses` write, `nested_fallible` re-established in `check/lower.hero` with its own annotated golden, `fmt.hero` split or raised **before** the alias arm and never in the same commit |
| `ffi-pragmatist` | **adopt-with-condition**, no veto (nothing crosses the boundary, so no veto ground exists) | §1.11, §4.19; spec § 13 `Member`; Part 8 wart 19 | compiled in a copy: a header's `typedef void (*handler_t)(int)` bound through two externs emits **one** typedef today, `h_0fn_634dc79f`, and the alias spelling owes that C **byte for byte** by construction — if the alias resolves to its target's type id. What it buys the boundary is **spelling only**: one written type saved at `qsort`/`bsearch`, zero at `sqlite3_exec`, and **zero** protection against the swapped handle | at the milestone that implements item 5, `tests/golden/run/ffi-callback-c-calls-back.hero` rewritten with `alias Handler = (function() -> ())` emits C that `cmp`s equal to today's, one typedef `h_0fn_294870dd`, one probe line; a second typedef means the alias leaked into `content_key` | the alias resolves to its target's type id in the checker, so `synth.content_key(Handler) == content_key(target)` and neither `table.get` nor `render_ty` on the emitter path ever sees one; the expansion `Env = {str: i64}` comes from the WRITTEN declaration, never a reverse map over ids; objects if `Member` admits `alias` inside a group |
| `llm-ergonomist` | **adopt-with-condition**, no veto | § 4 | Task 1 counted by hand: the type spellings of one program are **98** characters with the alias and **135** without, over three uses; the use site `h: Handler` costs one look-up the error pays back | given `alias Id = i64`, `alias Count = i64`, `f(id: Id, count: Count)`: at least half of first-try programs call `f` positionally (a § 9 error) against fewer than one in five with `f(id: i64, count: i64)`; and under the text WITHOUT the alias at least 1 in 10 first-try programs carries a mismatch between two hand spellings of one function type, under the text with it none | two silences must close in § 4 before *adopt* outright: self-reference (`alias Tree = [Tree]`, which a compiler following the text *"could equally refuse or loop"*) and what an error writes for a chain; its recommended words, 123 characters: *An alias may name another; one that reaches itself is an error, and what an error writes after `=` has no alias left in it* |
| `spec-warden` | **defer**, no veto (budget half provisional: a draft's real count is unrun) | §1.0 `:117-126` burden unmet; §1.3 `:211-217`; §1.5 `:247`; §1.6 `:305-315` payment; §1.2 `:190-204` | eleven drafts measured on cl100k: FULL **+68**, MIN **+44**; the removal that pays is **R1**, § 4's kinds sentence that the `Declaration` production already states, **−43**; R2 *No aliasing exists anywhere* **−6**; with the recursion clause (+7) the payable text W1 is **+26 net**, and R3+R5 take it to **−2**. The strongest case in the tree saves **48 tokens, 1.4%** of one file. The most repeated function type is generic and cannot be aliased. `alias` is **not reserved** | if adopted, W1's real delta lands under `DELTA_GATE` 50 once R1+R2+R3+R5 are taken; registered as an observation that pays nothing: when metric 2 runs, a spec with `alias` beats one without by fewer than 1 task in 20 | moves to adopt-with-condition when a `type_mismatch` golden or a metric 2 run shows a mistyped copy of a repeated spelling, or when item 10's sitting names `alias` as the C-width vocabulary's mechanism; veto only if a landing `--refresh` reads 8192 net of the FFI floor |
| `historian` | **object to the rationale, not the feature** (advisory): DEFERRED AGAIN with the item's text corrected | Pascal ISO 7185 §6.4.1, Modula-2 §6.3, Oberon-2 App. A; Go 16339/18130; Swift `(aka …)` | dates CONFIRM (go1 2012-03-28, go1.9 2017-08-24); the framing is FALSIFIED: Go's general `=>` alias was merged for 1.8 and **reverted 2016-11-04**, its warrant was **gradual code repair**, not brevity; Go printed the expansion in errors for **six years** and its 2024 repair broke controller-tools, mockery and govulncheck; Kotlin's `typealias` is a hard keyword. Wirth's `TYPE A = B` is a synonym under the same keyword as new types, so *call it `alias`, not `type`* departs from Pascal, Modula-2, Oberon, Go, Rust and Haskell. **Swift is the one shipped, default-on precedent for both forms in one message** | the largest count of one structural spelling in `selfhost/` is below 10; above 30 *"Go's warrant has already arrived here and my DEFER is wrong"* — **scored at the sitting: `{i64: bool}` is 77 across the tree, so by its own threshold the prediction FAILS**, with the warden's finer measure beside it: 6 tokens, at most 8 in one file, and whether any commit changed all 77 together is unrun | ENTERS if one structural type spelled identically at N sites was changed at every site in one commit, or a diagnostic golden's line exceeds a budget because a type is repeated; REFUSED if a shipped language removed its transparent synonym — none found |

## Where they disagree, unsmoothed

**Two seats said adopt and three said wait, and they were not disagreeing about the
same thing.** The ergonomist and the FFI seat judged the FORM: is it local, does it
break the boundary. Both found nothing to refuse — every wrong guess the ergonomist
made errors loudly, and nothing an alias does reaches C. The warden, the engineer and
the historian judged the WARRANT: is it needed, is it paid for, are the reasons under
it true. All three found no. Principle 0 is a burden on what enters, so the seats
whose mandate is the burden decide the verdict and the seats whose mandate is the form
decide what the form will look like when the burden is met. That is the split, and
it is not a compromise: the form was not vetoed, the warrant was not found.

**The historian's prediction failed at the sitting, and it stays in the table.** It
set a threshold — above 30 repeated spellings, its own DEFER is wrong — without
reading the count the brief handed it (69–77×). The count is 77. The seat's
argument survives on the warden's finer measure (six tokens, eight per file, a
set-of-integers idiom whose sites would not change together) but its prediction does
not, and a track record is only worth keeping when the misses stay in it.

**The engineer and the warden disagree on the return threshold.** Engineer: ≥ 30
characters at ≥ 20 sites in one program. Warden: ≥ 12 tokens at ≥ 10 sites in one
file. Today's maximum is 6 sites in one file at 12 tokens, so both are unmet; the
resolution below takes the warden's, because tokens are the unit §1.6 judges and
`heroes measure` is the instrument that exists, and it records the engineer's beside
it as the conservative number.

**The engineer's route and the FFI seat's condition agree without having met.** The
FFI seat's condition is that the alias never reaches `content_key`; the engineer's
finding is that interning at `check/lower.hero` makes exactly that the cheap route.
Two seats, one from the boundary and one from the checker, converged on the same
shape.

## The resolution adopted, provisionally

**Item 5 is DEFERRED AGAIN, dated, with a return condition, and its text is corrected
underneath rather than over.** This is the most complete resolution and not the
cheapest: a bare *deferred, v2* is what the ledger exists to end, and ENTERS would set
aside a rank-4 rule (Principle 0) on a token saving measured at 1.4% of one file.

1. **The verdict, written into design.md Part 7 item 5 as an indented paragraph under
   the original** (CLAUDE.md § 14: a record is corrected beneath, never over): DEFERRED
   AGAIN, 2026-09-13, panel 135. Principle 0 unmet on both branches, with the
   measurements above.
2. **Three claims struck, the number kept.** *Five years, nobody died* — Go's `=>` was
   merged and reverted before 1.8, and 1.9's warrant was refactoring, not brevity.
   *The only purely additive thing* — additive for programs, not tools (Go broke ten
   in 2017 and three more in 2024), and a reserved word is not additive; `alias` is
   unreserved today, so the milestone that lands it reserves the word **in the same
   commit** as the spec text. *Call it `alias`, not `type`* — kept as a deliberate
   departure from Pascal, Modula-2, Oberon-2, Go, Rust and Haskell, not as their
   precedent. And the spelling `Env = alias` is corrected to `alias Env = {str: i64}`.
3. **What the sitting settled about the form**, so the sitting that admits it need not
   re-derive it: transparent, and *everywhere* reaches the constructor and `::`; an
   error writes both forms, `Env = {str: i64}`, Swift's `(aka …)` the precedent; an
   alias never names itself, even through another, refused by an iterative walk in
   the resolver before any `type_uses` write and asserted again in `check/lower.hero`
   so a missed cycle aborts with a message and never overflows the C stack (§1.12); a
   chain is legal and the expansion has no alias left in it; no generics; not a
   `Member`; `T??` through an alias is refused in the checker with its own golden,
   because the parser can no longer see it; two canonical spellings of one type
   exist, so §4.15's sentence and `selfhost/print/types.hero:4-6` change with it; the
   route is R2a and the alias never reaches `content_key`; `print/fmt.hero` is split
   or its ceiling raised **before** the arm is written; the payable text is W1 at
   **+26 net**, paid by R1 (−43) and R2 (−6).
4. **The return condition**, falsifiable and naming instruments in this tree — any one
   of three: (a) `heroes measure` on a non-generic type spelling reads **12 or more
   tokens** and `grep -c` finds it **10 or more times in one file** of `selfhost/` or
   `examples/` (today's maximum: 6); (b) a commit in `selfhost/` whose diff changes one
   structural type spelling at **8 or more sites** — Go's own warrant, scored by
   `git log -p`; (c) Part 7 item 10's sitting at M-core-packages finds a header whose
   width vocabulary needs a name, which makes the form compiler-need — and that
   sitting, not this one, decides item 10. Registered as an observation that pays
   nothing under §1.6: when metric 2 first runs, a specification with `alias` beats
   one without by fewer than 1 task in 20 on first-try compile rate.
5. **Defect 029 is filed** beside the sitting (§ Found beside the sitting), and the
   emitter's double typedef is recorded there for the milestone that will meet it.

**What conservative would have been, so the author can choose it**: the same
deferral with the engineer's threshold (≥ 30 characters at ≥ 20 sites) instead of the
warden's, which returns the item later; or a bare *deferred* with no return condition,
which this milestone's own file rules out.

**What a no would compel.** If the author rules ENTERS, the form takes its own
milestone (a form lands in every tool that reads the language, CLAUDE.md § 9) under
the conditions in point 3, and the spec text is W1 with `alias` reserved in the same
commit. If the author rules REFUSED, the Part 6 row rests on §1.3 — *constructs whose
meaning lives elsewhere* — and its falsifier is the return condition above, because
no seat found a shipped language that removed its transparent synonym, so a row
resting on precedent would have nothing to name.

## Found beside the sitting, and reproduced before it was written down

**A swapped opaque handle compiles at zero diagnostics and segfaults.** The FFI seat
took `examples/sqlite/main.hero` and changed one line, `sqlite3_step(statement)` to
`sqlite3_step(db)` (line 74): `sqlite3 *` and `sqlite3_stmt *` are both `ptr`, the
probe is `(void)(sqlite3_step)(a0)` with `a0` a `void *`, and C converts in silence.
The coordinator rebuilt it from source in the seat's copy with the seed compiler:
**build exit 0, no diagnostic; run exit 139**. The unchanged example built with the
same binary runs to `rows: 3` at exit 0. design.md §1.12 says a Heroes program must
not segfault, and `.claude/rules/c-boundary.md` says that goal wins here first. The
record holds no ruling and no wart on this class (grep of design.md, `docs/panel/`
and the log for the shape, 2026-09-13: nothing), so it is filed as **defect 029**
with what is owed: a sitting on the repair, because the repair is a fact about `ptr`
and not about `alias` — the *distinct types* door item 5's own text keeps open, and
the one the FFI seat asked for by name. `heroes mutate`'s `swap-args` cannot produce
this mutant (it swaps label with value).

**The emitter already writes two typedefs for one function type.** With
`sqlite3_exec(…, callback: (function(context: ptr, count: i32, values: ptr, names:
ptr) -> i32), …)` and a `row` whose uniquely-typed parameter renders as `i32` with no
name, the checker calls the two types equal (the call passes) and `synth.content_key`
hashes their renderings apart: `h_0fn_539ea1ea` and `h_0fn_5f8c2b52` in one
translation unit. Legal C11, deterministic, not an ABI fact, and not this sitting's
question — but it is exactly the shape an alias implementation could make worse, and
the FFI seat's condition is written against it. Recorded here for the milestone that
next touches `selfhost/emit/synth.hero`.

**A written premise in the tree that an alias would falsify**, found by the
engineer: `selfhost/check/table.hero:66-67` says `T??` *"is rejected by the parser, so
this node's argument is never itself a fallible"*. True today, and true only because
no name can stand for a fallible type. It is the kind of premise
`.claude/rules/module-shape.md` prices — a fact about the world, not the value — and
the alias sitting that follows owes it the test that fires when it dies.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 135`. The question put: ratify
DEFERRED AGAIN with the return condition in point 4 and the corrections in point 2, or
rule ENTERS or REFUSED with what each compels above.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| historian: the largest count of one structural spelling in `selfhost/` is below 10 | `grep -c` | **scored at the sitting: FAILED**, `{i64: bool}` is 77 |
| warden, measured now: one alias for the six copies in `examples/interpreter/run/expr.hero` saves 48 cl100k tokens, 1.4% | `heroes measure` on the rewritten file | done, 2026-09-13 |
| engineer: if `alias` enters, `fmt.hero`'s DECIDED row exceeds 1150, or `selfhost/print/` gains a module, or the arm lands uncommented at exactly 1150 | `heroes run tests/harness/main.hero -- ./heroes layout` | the alias milestone's close, if any |
| FFI: the callback golden rewritten with `alias Handler` emits C that `cmp`s equal to today's, one typedef `h_0fn_294870dd` | `heroes build --emit-c` and `cmp` | the alias milestone's close, if any |
| warden: W1's real delta lands under `DELTA_GATE` 50 once R1+R2+R3+R5 are taken | `heroes measure spec/heroes-spec.md --refresh` | the alias milestone's landing commit, if any |
| ergonomist: with `alias Id = i64`, `alias Count = i64`, `f(id: Id, count: Count)`, at least half of first-try programs call `f` positionally against fewer than one in five for `f(id: i64, count: i64)` | metric 2's runner, when it exists | M-thesis-harness |
| ergonomist: without the alias, at least 1 in 10 first-try programs carries a mismatch between two hand spellings of one function type; with it, none | metric 2's runner | M-thesis-harness |
| historian: at the close of whichever milestone adds `alias`, no second commit is needed to fix alias display in `fmt`, the IR printer or any re-printer | `git log` over that milestone | the alias milestone's close, if any |
| the return condition's own clock: `heroes measure` on a non-generic spelling ≥ 12 tokens found ≥ 10 times in one file; or one commit changing a structural spelling at ≥ 8 sites | `heroes measure`, `grep -c`, `git log -p` | every later sitting of this ledger re-reads it |

## What the seats could not source or could not run

The engineer compiled no prototype (the 20-minute rebuild the brief forbids), so its
line figures are counted estimates by analogy; the arm counts and the ceilings are
measured. The warden could not run the real tokeniser on a draft (no pin; `--refresh`
is never run in a sitting). The FFI seat did not run the unswapped example in its copy
(the coordinator did: exit 0) and did not probe raylib's `TraceLogCallback`
(`va_list`), nor the swap on the other two platforms. The historian could not verify
Swift's `shouldShowAKA` mechanism name (the test file is verified), Swift's keyword
status, GHC ticket #10547 (fetch blocked), or the Oberon 2013 revision's type-identity
appendix; and its *no shipped language removed its transparent synonym* is a negative
from its own vocabulary, so it stands as a question. Whether a module's interface text
includes type declarations under separate compilation, so that editing an alias's
target invalidates dependents, is unrun.
