# SCHEDULED — work that already has a milestone

Neither a question nor a decision: work with a **home**, waiting for it. `/step`
reads this when it opens the milestone an item names — its § 1 Orient says so,
and that line is the whole mechanism. Nothing here needs the author, and nothing
here should be done early: sooner means against a smaller corpus, a smaller
compiler, or a measurement not yet taken.

**EVERY ITEM'S FIRST FIELD NAMES A MILESTONE THAT HAS A ROW IN `docs/ROADMAP.md`
§ The chain AND IS NOT CLOSED** (author instruction 2026-09-04). A milestone
name, or a name plus a step — never a waiting condition, never a description of
a milestone, never a `grep`. If no open row does the work, one is created and
the author places the new id. A closed home and no home read exactly alike in
this list, which is why the rule is about the first field and not about
intentions.

**Only open items live here.** The moment one is done it is ticked with what
closed it and moved to `docs/work/DONE.md`, the record.

**The shape.** One line per item, then the body indented four spaces, opening
with `**Origin:**` and its date. Nothing lives outside the two banners —
`records/lists` is the executor of that.

Format: `- [ ] **M-<milestone>** | <what, in one line> | <where to look>`

**THE ORDER IS § THE CHAIN'S** (author instruction 2026-09-10), milestone by
milestone in the order the ROADMAP will take them, stable inside a milestone,
and an *opening sitting* item first where one exists because no step of that
milestone lands before it. **The file had no visible order until then**: 42 items
ran across a dozen milestones in the sequence they happened to be filed, so a
reader opening a milestone could not see its items together. The order carries no
priority — `/step` reads this file by the name in the first field, not by
position.

**Every item was verified against the repository on 2026-09-10**, on the same
instruction, and the constraint was **verify, never repair**: an item found still
open stays open and is not worked. What that sweep found is written into the
items themselves, each under a dated `**Re-verified 2026-09-10:**` paragraph, and
the headline is worth keeping here: **of 42 items, none had been done** — but
five rested on a sentence that is now false, and most of the rest carried
pointers that had drifted, because `design.md` has grown by 150 to 290 lines
since they were written and CLAUDE.md's §§7-11 became one-line pointers into
`.claude/rules/`. **A line-number citation is exactly what `records/citations`
cannot judge**, since it reads a path claim, so those had been passing every run.
Three pairs became one item each, in each case because the two named the same
file and the same panel and one action closes both; where two items merely share
a *blocker* rather than a deliverable they stay two, with the dependency named.

*******************************************************************************
**OPEN: 52**

- [ ] **M-reflection-verdict** | panel 117's sixth option: give `assert` both sides for an aggregate by walking to the first DIFFERING LEAF and naming the field, instead of rendering the value | `docs/panel/117-both-sides-of-an-assert-when-a-side-is-an-aggregate.md` · `selfhost/emit/structural.hero` · `runtime/parts/array.c`

    **Origin:** panel 117, 2026-09-07, the sitting's own finding rather than
    any brief's. The historian read Zig's `std.testing.expectEqual` from source:
    it never renders the aggregate, it switches on the type and recurses, and
    prints two values only on the scalar arm. Its doc comment says the point out
    loud, that it shows exactly how the two are not equal.

    **Why this route and not the four the brief carried.** It answers every
    objection the sitting raised by construction, not by compromise. No
    iteration order is promised, so the map measurement that killed every
    rendering option stops being an obstacle. The walk is `eq`'s, which panel
    076 already gave a worklist and no bound for a reason that makes it sound,
    so no new depth question opens. Nothing is rendered, so the third invariant
    clause *equal values render equal* is never owed and
    `selfhost/emit/structural.hero`'s seven remaining lines under §11's ceiling
    are not spent. `HeroDesc` and `HERO_RUNTIME_ABI` are untouched, so the FFI
    seat's veto is satisfied without argument. And it is not a second copy of
    what the language is, so the historian's D precedent — an assert-only
    renderer that broke at the link boundary and still has open defects seven
    years on — does not bind.

    **Three things owed BEFORE it lands, because it is unrun.** A compiled
    prototype, since no seat was asked about it and the cost is an estimate. The
    map case run against the two `==`-equal maps the sitting measured, to show
    the walk reports a differing key without promising an order. And the depth
    case run to the frame count panel 076 measured, to show `eq`'s worklist
    carries it.

    **And §9 owes a `fixedbugs` case per shape the day it lands**, because the
    provoking case is a witness and not the class: an array, a map, a record, an
    optional, a variant, and a nested one of each. `tests/harness/suite_special.hero`'s
    aggregate row goes red that day by design, and unpinning it is part of the
    landing.

    **Re-verified 2026-09-10: STILL OPEN, and its own estimate is exact today.**
    `tests/harness/suite_special.hero:260-269` still pins today's behaviour on
    purpose and names panel 117 in the unpin instruction; `grep -rn differing
    --include='*.hero' selfhost runtime` is empty, so no walk exists. And the
    *"seven remaining lines under §11's ceiling"* re-measures to exactly that:
    `selfhost/emit/structural.hero` is **293** code lines against a 300 ceiling, in
    `suite_layout.hero`'s own unit and not `wc -l`.

- [ ] **M-reflection-verdict** | the witnesses, re-measured at the opening, plus the author's annotation question | `examples/json/` · `design.md` Part 6 · `docs/panel/018`

    **Origin:** author instruction 2026-09-03, the author's own example;
    scheduled `DESIGN-LOG.md:539`. A third question is the author's own,
    2026-09-06.

    **Compile-time derivation over a record's fields is refused by nothing in
    the record, and the corpus writes it by hand.** Measured 2026-09-03:
    `examples/json/` is **671** lines (`main.hero` 64, `parse.hero` 416,
    `value.hero` 191) rendering and parsing a `variant` by hand; `grep -rhoE
    '^function (to_str|render|to_json|from_json|show|format)_?[a-z]*'
    examples/` finds **20** hand-written renderers (`render` 5, `shown` 4, `show`
    4, `rendered` 4, `to_str_of`, `show_levels`, `render_or` one each); the
    emitter already generates `eq` and `hash` by walking fields (CLAUDE.md §7).
    Run-time reflection is the Ruby row's (`design.md:2395`), and its refusal
    owes a Part 6 row with a falsifier, since today it is citable only from a
    reasoning note (`docs/panel/039-comptime-and-part-6.md` § Appended
    2026-09-04, row C4) and a panel aside (`docs/panel/018:99`). The sitting
    sits before M-core-packages step 2, `encoding/json`, whose Go original is
    reflection-based.

    **And a third question since 2026-09-06, the author's** (*"simply tags one
    can put on some parts of the code, which the compiler can use — so system
    tags, or other tags that could be defined by the user … I see some keywords
    that look exactly like decorators, such as `owned fclose`"*): (iii) a
    general annotation mechanism, user-defined tags on declarations, ruled on
    with a Part 6 row and its falsifier. Its only refusal today is
    `docs/panel/018-top-level-declaration-shape.md:97-100`, a
    rejected-along-the-way bullet no DESIGN-LOG row names. The brief carries
    panel 114 R7's closed-set clause and its Rust witness
    (`docs/panel/114-the-question-was-not-which-platform.md:150-156`,
    `:244-252`), `design.md:217`'s *"meaning lives elsewhere"*,
    `design.md:1953`'s prefix-`@name` refusal, and the honest argument for it,
    CLAUDE.md §9's bill paid once. Measured 2026-09-07: `decorator` appears
    three times across design.md, `spec/`, `docs/panel/`, `DESIGN-LOG.md`,
    `docs/work/`, `docs/ROADMAP.md` and CLAUDE.md, the prior's refusal in
    `spec/reserved-words.md:30` and FastAPI twice, and in no sitting; spec
    headroom **225**. The system marks the compiler already reads — six
    contextual words in `selfhost/parse/` — are M-core-packages' question (vii),
    and a Python decorator is M-closures-verdict's; both named, neither judged
    here — and since 2026-09-08 that half is **answered**: closures are refused,
    so a decorator in Python's sense is not buildable and this sitting's question
    (iii) is about tags alone.

    **Where to look also:** `design.md` Part 6 (`:2395`, `:2406-2428`), `:217`,
    `:1953` · `docs/panel/039` § Appended 2026-09-04 (row C4) ·
    `docs/work/DONE.md`, the 2026-09-04 entry for the packages session ·
    `selfhost/parse/members.hero:81-94`.
    **Why it matters:** a refusal that lives in a footnote is a question that
    will be asked again — and on 2026-09-06 it was.

    **Re-verified 2026-09-10: STILL OPEN, three counts STALE.** Hand-written
    renderers are **31**, not 20 (`shown` 13, `show` 5, `render` 5, `rendered` 4,
    four singletons). Spec headroom is **706**, not 225 — the ceiling moved to 6144
    on 2026-09-09. And `decorator` now returns **10** hits over the same document
    set, because the 2026-09-07 rows that RECORD the *"three times"* measurement now
    count themselves, which is the shape a count over a record always takes.
    `examples/json/` is still **671** lines exactly. Two pointers moved:
    `design.md:2395` is `:2489` and `:1953` is `:1988`. And `encoding/json` is
    M-core-packages **step 4** since the 2026-09-10 reorder, not step 2.

- [ ] **M-deferral-ledger** | the list the ledger opens with | `design.md` Part 7, Part 8 warts 15–16, Part 9 · `docs/panel/008-escape-sequences.md`

    **Origin:** author instruction 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **Seven Part 7 items and two watch-list entries have no milestone and no
    verdict since the fixpoint made Part 7 admissible (2026-08-18).** Measured
    2026-09-03 against design.md: item 5 `alias` (`:2457`), 6 doctests
    (`:2467`), 8 traits (`:2469`), 9 variant constructors as values (`:2472`),
    11 `raw` (`:2502`, Part 9), 14 visibility (`:2562`), 16 symmetric variant
    syntax (`:2585`); raw string literals (Part 8 wart 15, `design.md:2673`;
    panel 008's finding, on the panel watch list until it was retired
    2026-09-04) and printing without a trailing newline (wart 16, `:2668`).

    Each gets one sitting, in this order, and one of three dated verdicts:
    enters, refused with its falsifier (CLAUDE.md §12), or deferred with a
    return condition written as a falsifiable claim. Not on this list, because
    they have homes: items 1 and 12 (**M-closures-verdict, both refused
    2026-09-08** — 1 to Part 6, 12 to the unplaced paragraph, so neither is on
    Part 7 for this ledger to date), 7
    (M-interpolation-verdict), 10 and conditional compilation
    (M-core-packages' sitting, questions v and vi), 13 (M-isolated-threads), 15
    (M-qbe-backend).

    **Why it matters:** a deferral with no date is a promise, and Part 7's
    preamble says it is not one.

    **Re-verified 2026-09-10: STILL OPEN, and every line number in the list has
    moved.** design.md is 3612 lines now, so Part 7's items read: 5 `alias`
    **:2649**, 6 doctests **:2659**, 8 traits **:2672**, 9 variant constructors
    **:2675**, 11 the `raw` module **:2705**, 14 visibility **:2790**, 16 symmetric
    variants **:2813**; wart 15 is **:2893** and wart 16 **:2896**. None has gained a
    verdict or a milestone, so the ledger's nine are unchanged in substance. **One
    body sentence is behind the tree**: it lists item 7 as homed at
    M-interpolation-verdict, and `design.md:2660` now reads *"ENTERS, ruled
    2026-09-09"* with `m-interpolated-strings` a placed tag.

- [ ] **M-deferral-ledger** | the ledger widens from Part 7 to Part 8's warts, and coverage gets the answer it already has | `design.md` Part 8 warts 5, 8, 11 · `docs/panel/034` · `selfhost/mutate/`

    **Origin:** author decision 2026-09-10, § What production-ready means.

    **Why the same milestone and not another**: the ledger's verdict vocabulary
    already fits — enters, refused with a falsifier, or deferred with a return
    condition — and its own list already reaches into Part 8 for warts **15** and
    **16**. Three more warts have no owner at all: **5**, errors are codes and
    strings rather than types; **8**, `+` on `str` is quadratic and so is the
    `[str]` built to avoid it; **11**, no user-extensible iteration, which is
    Part 7 item 8's shadow. **Wart 5 was already ruled to STAY a wart** (panel 034,
    *"the answer is a `constant`, not a feature"*), which is the ledger's third
    verdict and shows the widening costs no new machinery.

    **And coverage, which a production reader will ask for.** Line coverage does
    not exist and `heroes mutate` is a stronger instrument than it — a mutant the
    compiler fails to catch is a hole in the language, where a covered line is only
    a line that ran. **The answer belongs in Part 6's shape**, a refusal naming the
    program that would make it wrong, rather than in a silence a team reads as an
    omission. Its falsifier is available: a defect class `mutate` cannot reach
    because no operator produces it, which a covered-line report would have
    named.

- [ ] **M-cleanup-verdict** | the sitting, and the count it is handed rather than guesses | `design.md` Part 6, Part 7 · `spec/heroes-spec.md:241-246` · `selfhost/emit/types.hero:6`

    **Origin:** author decision 2026-09-10, out of the session that wrote
    `docs/ROADMAP.md` § What production-ready means. The row's own section carries
    why it exists; this item is what the sitting needs in hand.

    **The silence is measured and the vocabulary is named**, so the sitting starts
    from a fact rather than an impression: `defer`, `RAII`, `scope guard`,
    `scope-bound`, `destructor`, `finally` and `cleanup on`, over `design.md`,
    `spec/` and all 125 sittings, return **nothing about a construct**. The two
    hits are `design.md:1525`'s *"C++ RAII with references"*, which is a row about
    the ownership model, and the emitter's internal `h_T_drop`.

    **What is owed at the opening, and it does not exist yet**: how many
    acquire-and-release pairs stand in `selfhost/` and under `examples/`, and how
    many early returns and `?` operators sit between an acquire and its release.
    That count is the argument in both directions, and neither direction may be
    argued without it. The two obligations to count are `owned <C function>`
    (2026-09-07) and `x: cstr @ s.lease()` with `end_lease(@x)` (2026-09-09), and
    `spec:246` is what makes a miss loud: a lease nobody ends aborts when `main`
    returns, saying how many.

    **What the sitting may not do**: take M-deferral-ledger's Part 7 items, which
    are that milestone's, or decide a spelling before it has decided whether a form
    enters at all. Full five seats, because a form that enters has surface.

- [ ] **M-check-completeness** | the sortable obligation and the two written-type rules, in the one pass that closes all three | `docs/panel/082` R3 · `selfhost/check/walk.hero` · `selfhost/check/ordering.hero` · `selfhost/check/map_keys.hero` · `selfhost/check/partial.hero`

    **Merged 2026-09-10** from two items, by author instruction, and the merge is
    the second item's own sentence rather than a judgement: it said *"whose pass is
    what closes both of these rules"*. One pass in the checker closes the sortable
    obligation and both rules that fire on a written type and not on an inferred
    one, so one item holds them. **Both bodies are kept whole below**, each under
    the summary it arrived with, because a merge that summarises loses the
    measurements the sitting is owed.

    **Re-verified 2026-09-10: STILL OPEN, and three pointers moved.** The trigger
    is unchanged — `grep -rnE '^function [a-z_]+<' selfhost/` is **0** — and
    `selfhost/check_sortable.hero` still does not exist (checked 2026-09-10, and
    the date is on this line because the path is a claim about a file nobody has
    written). `is_refusable` still
    stands alone at `selfhost/check/ordering.hero:45` with no recording sibling,
    and `grep -rn current_decl selfhost/` is empty. Both bypasses are still in the
    code: `check/map_keys.hero:132` gives up on a type parameter while the written
    case fires at `:90`, and `check/partial.hero:84` puts `.generic` in the give-up
    arm. **The flat filenames in the first body are dead** — `check_walk.hero`,
    `check_ordering.hero`, `check_state.hero` and `ir_mono.hero` are now
    `selfhost/check/walk.hero`, `check/ordering.hero`, `check/state.hero` and
    `ir/mono.hero`, which M-selfhost-nesting moved on 2026-09-02. **And the pass
    has nowhere to go without a decision**: `check/walk.hero` measures **1708**
    code lines against a DECIDED ceiling of exactly **1708**, so the ~90-110 line
    pass needs its own module or a ceiling raise, argued. The `partial` promise is
    `spec/heroes-spec.md:258`, not `spec:221`. The behavioural claims were
    deliberately not re-run: they need a source file and a build.

    **The first item, as it stood.** the sortable obligation moves into the checker

    **Origin:** panel 082 R3, ratified 2026-08-16. Its home since 2026-09-04,
    when every item here was checked against the chain: this one had named a
    `grep` and no milestone since 2026-08-16, and a waiting condition is not a
    home. The trigger itself is unchanged and is restated below, where it
    belongs.

    **A pass of its own, not built.** The sitting's own estimate, 2026-08-16, is
    ~90–110 lines at `selfhost/check_sortable.hero` (2026-08-16), which names a
    file this repository does not have yet.
    Today `heroes check` accepts `first([P(x: 1)])` at exit 0 and `heroes build`
    refuses it, which is the one shape where *check accepts ⇒ build succeeds* is
    still false. Option (a) — running `mono` inside `check` — is **refused**,
    measured: 57 lines, `check.rs` past §11's ceiling, 1.3–2.2× on every
    keystroke, `--permissive` contaminating Part 11's control arm, a `heroes
    mutate`/`heroes check` split scoring two languages, and a §4.16 hole that
    turns every `???` file into exit 1. (b) is the direction because it is the
    only one that puts the diagnostic on **the call the author can edit**: the
    IR route provably cannot, since `ir/mono_subst.rs` copies the template's
    span and `mono.rs` discards the call's, while `types/apply.rs:139` already
    keys instantiations **by the call-site span**. Shape: `Checker` gains
    `current_decl`, `apply.rs` records the callee, `ordering.rs` gains a
    recording sibling to `is_refusable`, and one fixpoint over `(decl, param)`
    pairs — finite, so termination is free.

    **The `.rs` names above are the BOOTSTRAP's, kept as the sitting measured
    them** (2026-08-16, three days before `crates/` became
    `archive/bootstrap-rs/`): the live counterparts are `check_walk.hero`,
    `check_ordering.hero`, `check_state.hero` and `ir_mono.hero`, and whoever
    builds this re-measures against those rather than trusting the mapping. The
    call-site span was `crates/heroes/src/types/apply.rs:139` until 2026-08-19,
    when the bootstrap was archived.

    **The trigger is a grep, not a date**: `grep -rnE '^function [a-z_]+<'
    selfhost/` returns zero today, and the day it returns a generic whose body
    contains `sort(`, this is §1.0 compiler-need at any price. Until then
    Principle 0 holds it — nothing on the closure list needs it, measured by
    three seats. **Not carried by this item**: the llm-ergonomist's veto, which
    (b) does not lift — it leaves the generic body's line undecidable from the
    line plus its signature, and lifting that needs constraints on generics,
    which is the author's trade and lives in `docs/panel/082` § What a veto
    would compel.

    **Why it matters:** the sitting ruled the direction and Principle 0 ruled
    the date, so this is work with a home rather than a question.

    **The second item, as it stood.** two rules that fire on a written type and not on an inferred one

    **Origin:** panels 082 R3 and 084, decided 2026-08-16. Its home since
    2026-09-04 for the same reason as the item above, whose pass is what closes
    both of these rules — they share its milestone.

    `float_map_key`: `m: {f64: i64}` is exit 1 written down, and a map built
    **inside** `function tally<K>(k: K)` as `m: {K: i64} @ {}` keyed at `f64` by
    the call is `check` 0 and **runs**. `ffi_partial_operation`: `spec:221`
    promises a compile error for comparing a `partial` group record *"for it and
    for any value holding it"*, and through `function same<A>(x: A, y: A)` with
    `x == y` it is `check` 0, `build` 0, **run 134**.

    **Both are consistency and not safety, and that is measured rather than
    assumed**: a real `nan` key through the generic gives `panic: a map key that
    is not equal to itself (nan)` and the `partial` case gives panel 061's own
    deliberate `hero_panic`, both clean stops, both identical under
    `--sanitize`. §12 says the compiler is wrong; §1.12 says it may wait.
    **Neither can take panel 084 R1's shape** — that worked because `sort`'s
    domain is restricted, so refusing it on `[A]` deleted nothing; a map keyed
    on `K` is legal at `str` and every integer, and `==` is legal on almost
    everything, so refusing either in the body would delete working programs.
    The concrete type arrives at the **call**, which is what R3's pass reads.

    **Where to look also:** `spec:221`.
    **Why it matters:** one rule, two answers, depending on whether a generic
    stands in the middle.

- [ ] **M-check-completeness** | nothing watches the TEXT of a `guess` fix, and one shipped a name the language had withdrawn | `tests/harness/suite_fixes.hero` · `tests/harness/suite_golden.hero:160-171` · `selfhost/value_errors.hero:32`

    **Origin:** found 2026-09-08 while writing defect 019's golden case at M-closures-verdict step 3 — the case would not pin the thing the defect was. **Filed in `DECIDE.md` first and moved here the same day**: there is no question to answer, since nobody would argue that a diagnostic should recommend a name the language does not have. It is work, and work with a milestone belongs here.

    **What is unwatched, measured**: `.expected` compares the stderr of
    `check --brief`, which carries one line per diagnostic and **no `fix (…)`
    line**; `suite_fixes` tests only `certain` fixes, through `.fixed`, because
    §8 makes only those machine-applicable. So the four cases written at that
    step pin every message and not one fix title, and `mixed_arithmetic`
    recommended `fit_<width>(x)` for as long as the family had been gone — the
    only reason it was noticed is that a reader ran the compiler's own advice.
    A `guess` fix is *more* likely to be read by a human than a `certain` one,
    which is applied without being read.

    **Why this milestone.** A `certain` fix that does not compile and a `guess`
    fix that names nothing are one family: the compiler's own advice must be
    sound, which is this row's subject applied to the repair rather than to the
    program.

    **The shape, and the reason it is not a one-liner**: assert that every
    backticked identifier in a fix title is a name the language has. The
    difficulty is telling a name from a fragment — titles carry `` `from: ` ``
    and `` `to_i64(x)` `` alike — so a hasty rule either misses the class or
    cries wolf, and the alternative of pinning fix text in `.expected` makes
    every golden churn whenever a wording improves, which is why the compact
    form exists.
    **Why it matters:** the compiler telling a reader to write something that
    does not exist is the same failure as a diagnostic that lies, and this is
    the one kind no instrument here can see.

    **Re-verified 2026-09-10: STILL OPEN, and its central claim is exact.** Of
    the **104** `tests/golden/check/*.expected` files, **zero** carry a `fix (…)`
    line; the only `.expected` in the tree that does is
    `tests/golden/unsupported/ffi-writable-parameter.expected:8`. So no snapshot
    watches a fix's text. **One half landed since**: the `fit_` defect is repaired
    (`selfhost/value_errors.hero:51-55` names `to_f64(x)` / `to_<width>(x)`) with its
    golden, `tests/golden/check/fixedbugs-a-guess-fix-names-a-family-that-exists.hero`,
    and defect 023 added two exact-title assertions for `mixed_arithmetic` alone
    (`:453-466`). The instrument is still missing.

- [ ] **M-check-completeness** | a doubly-fallible value the checker tracks and the syntax cannot write | `selfhost/parse/type.hero:55` · `docs/panel/110` · `selfhost/library_source.hero:92`

    **Origin:** `docs/panel/110`, 2026-09-04: the sitting that refused the `T??`
    spec sentence docketed the question the refusal rests on. This item said
    *row 42* until 2026-09-05, and inserting `M-c-callbacks` at 34 made it 43,
    which is exactly why CLAUDE.md §14 puts the order in the chain table and
    nowhere else; the number is dropped rather than corrected — and
    `docs/ROADMAP.md:363` records it as *"the only one of the four with **no
    warrant**"*, which this item is.

    **So `heroes check` accepts programs whose types have no spelling.**
    Measured 2026-09-04: `m: {str: i64?}` is accepted and `v = m["a"]` then
    `v.must().must()` **runs and prints 1**; `function take(x: i64??)` is
    `error[nested_fallible]`; `function take(x: i64?)` handed that value is
    `error[bad_operand]`, so the checker knows the real type; and the compiler
    **prints a type it refuses to parse** — `error[type_mismatch]: expected
    i64?, found i64??` — which is §4.17's own failure, a message that cannot be
    acted on. The producer is not a contrivance: the §1.11 built-in `find<A>(xs:
    [A], f) -> A?` over `[i64?]` yields it and prints 1, and a **generic**
    signature names it and nests without bound (`wrap<T>(x: T) -> T?` twice
    gives `i64???`). `spec:157` promises `m[k]` returns `V?` unqualified.

    **Four repairs were priced at the sitting and one is vetoed on soundness**:
    refusing `{K: V?}` at the declaration (~30 lines, breaks 0 programs, closes
    one of three doors); **flattening `m[k]` — VETOED**, because it collapses
    *key absent* and *key present, value failed*, so a stored `fail("parse", …)`
    becomes indistinguishable from a missing key and the program takes the wrong
    branch at exit 0; making written `T??` legal and deleting the parse refusal,
    which is the only option that closes all three doors and the only one §1.7
    favours since it **removes** a special case, at the cost of a catch the
    thesis may want; or leaving it and qualifying the spec.

    **What this milestone's sitting must measure first, none of it run yet**:
    `--dump-ir` on the `find`-over-`[i64?]` program, to see whether the nested
    fallible has a stable representation under monomorphisation; whether `?`
    peels the outer level in the **emitted C** as well as on the checked path;
    whether `hero_runtime_check_leaks()` is clean on a nested fallible carrying
    a `str`; and the full list of library functions that bind a parameter from
    an argument and return it fallible — **one of six measured** (`find`,
    `library_source.hero:92`). Also owed here and cheap: `nested_fallible` is
    **absent** from `is_thesis_rule` (`selfhost/diag.hero:88-103`), so
    `--permissive` does not drop a refusal the checker computes past happily,
    which is a question about what Part 11's control arm is.

    **Where to look also:** `selfhost/diag.hero:88-103` · `spec:157` ·
    `tests/golden/check/a-fallible-type-is-never-written-fallible-twice.hero`.
    **Why it matters:** this row's own sentence is *what `heroes check` accepts,
    `heroes build` compiles — through a generic too*, and it fails in both
    directions on the same day: `function f(_: ())` was check 0 and build 2
    until defect 012 was repaired, and this one is check 0 with a type no
    signature can hold.

    **Re-verified 2026-09-10: STILL OPEN on the code, UNSETTLED on the run.**
    Every code claim holds: `selfhost/parse/type.hero:51-58` refuses the **written**
    form only, `selfhost/library_source.hero:92` is the `find` signature as quoted,
    `nested_fallible` is still absent from `is_thesis_rule`
    (`selfhost/diag.hero:88-104`), and no refusal of `{K: V?}` at a declaration
    exists anywhere. **The runtime half was deliberately not re-run** — it needs a
    source file and a build — so *"`v.must().must()` runs"* stands as the item wrote
    it. Two pointers moved: the `m[k]` promise is `spec/heroes-spec.md:165-166`, not
    `spec:157`, and the *no warrant* sentence is `docs/ROADMAP.md:419`. **Cite the
    milestone by NAME rather than by row**: that row was 48 and is **50** since the
    2026-09-10 insert, which is exactly why §14 keys by name.

- [ ] **M-arm-platform** | the fourth leg: an arm64 image, a matrix entry, and the `char` prediction scored | `docs/environment/linux/` · `.github/workflows/ci.yml` § matrix · `.claude/rules/platforms.md`

    **Origin:** author decision 2026-09-10, § What production-ready means row 3.

    **What it delivers**, in the order the platforms rule asks: an arm64 Linux
    image beside the x86-64 one under `docs/environment/linux/`, built from its own
    `Dockerfile`; the seed built from C alone there; the compiler's **618** tests
    and the harness's own **126** passing there; and a fourth CI matrix entry, so
    the leg is a judge and not a hunting instrument (`.claude/rules/platforms.md`
    § A platform fact is run on a platform).

    **The prediction is registered here so it can be scored.** Plain `char` is
    unsigned on the ARM ABI and signed on x86-64, and `spec:228` declares a
    parameter and a field at *the header's own width and sign*, so either the leg
    finds a divergence in the corpus's **20** `extern` programs of 55, or it finds
    none and the FFI's width rules are stronger than three legs could show.
    Structure padding and `va_list` are the next two shapes to attack, in that
    order.

    **Why it is cheap, measured before the row was written**: the existing image
    runs x86-64 **under Rosetta** on an arm64 Mac, so the new one is the native
    instrument and not the dearer one.

    **What it may not become**: a `--target` flag. `DESIGN-LOG.md:539` refused
    cross-compilation on 2026-09-03 and this row obeys that refusal rather than
    bending it — a real machine, measured, exactly as the rule asks.

- [ ] **M-core-packages** | the opening sitting, full five seats, and no step lands before it | `design.md` §1.11, §4.15, §4.19, Part 6 · `docs/panel/028`, `032`, `036`, `039`, `049`, `097`, `099`

    **Origin:** author instruction 2026-09-03, out of the reasoning session
    recorded in `DESIGN-LOG.md:537`; the sixth question added by the author the
    same night. Put ahead of M-package-manager by the reorder late on
    2026-09-03, `DESIGN-LOG.md:539`.

    **Six questions, each measured before it is asked.** (i) The §1.11 boundary
    for a package written in Heroes alone: the ROADMAP's own test — *a binding
    is verified by clang against the header it names, and a standard library is
    verified by whoever wrote it* — puts a pure-Heroes `strings` on the second
    side, and the answer owes a falsifier (CLAUDE.md §12). (ii) Where packages
    live and how `use` reaches them: measured, `heroes test
    pkg/strings/strings.hero` runs a leaf's tests alone, but a module that
    `use`s a sibling package compiles only from the program's root (`use` cannot
    climb, panel 099 R1), so `heroes fetch` places a tree under the root and
    each tree wants a root-level driver — panel 032 R6 made concrete, with panel
    028 R3 keeping anything from being searched at run time. (iii) The byte
    buffer: a `ptr` of known length already becomes a `str` through
    `hero_str_from_bytes` (10 MB in 0.32 s), but `[u8]` is `error[ffi_type]` at
    check (`selfhost/check/ffi.hero:45`); three routes — per-byte runtime
    entries (soundness lane, ABI +1), a `[u8]` result admitted for a group over
    `heroes_runtime.h` (a diagnostic and a §4.19 sentence change, full lane), or
    a built-in (the route panel 036 refused for `read_file`). (iv) `net` as a
    runtime part: `sockaddr` differs between Darwin and glibc —
    `error[ffi_field_type]` on the other platform, measured both ways — and
    Windows is winsock; panel 097's `struct stat` shape, touching §1.11's row
    *Sockets: libc*. (v) Part 7 item 10 widened to a typedef whose width **or
    sign** differs by platform: `clockid_t` is `u32` on Darwin and `i32` on
    glibc, so `clock_gettime` has no single spelling and `timespec_get` is the
    portable clock. (vi) **Conditional compilation** — five shapes: C's textual
    `#if`; a compile-time keyword in the body (Nim `when`, D `version`, Odin
    `when`, Swift `#if`), whose inactive branch is not type-checked on this
    machine; Rust's `cfg` attributes, likewise; one file per platform (Go
    `net_linux.go`, Odin `_linux.odin`, Hare `+linux`), every file a whole
    module checked on its platform and no word in the body; and nothing in the
    language (Ada, Oberon), which is where Heroes stands.

    The record to hand the seats: panel 049 refused the platform axis with a
    veto (*"a platform question belongs where it is a measurable fact about the
    machine"*), panel 097 put the arm in `runtime/parts/`, panel 039 left
    comptime unplaced, §4.15 makes a textual difference semantic, and CI `cmp`s
    `seed/heroes.c` against what the compiler emits on every leg
    (`.github/workflows/ci.yml:515-520`), so host-dependent emission breaks an
    instrument. The limit to name: the runtime is the only C a package can add
    to (panel 036 P2 vetoed `compile "shim.c"`), so shape five serves the
    project's packages and nobody else's. **Not on the list**: the callback
    boundary — measured, no package needs it; it is M-isolated-threads' item
    below.

    **Where to look also:** `design.md` Part 7 item 10 ·
    `selfhost/check/ffi.hero:45` · `selfhost/modules.hero:125` ·
    `runtime/heroes_runtime.h`.
    **Why it matters:** a server cannot be distributed in Heroes today, and
    every reason is a compiler fact rather than a missing library.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held** (`docs/panel/` ends
    at 125), **and it gained two questions.** `docs/ROADMAP.md` § M-core-packages now
    carries **(viii)** whether a group's header is the authority for its own
    declarations or the module's header set is — the compiler's own
    `selfhost/cli/process.hero:49-53` declares three `hero_os.h` functions under
    `extern "stdlib.h"` and compiles, because a TU includes every group's header
    (`selfhost/emit/unit.hero:32`) — and **(ix)** how thick a wrapper over a C group
    is, the author's question of 2026-09-10, whose already-settled half is panel 033
    R5: the wrapper is mandatory, not a matter of taste. **Two pointers moved**: the
    `[u8]` refusal is `selfhost/check/ffi.hero:89`, not `:45`, and the seed `cmp` is
    `.github/workflows/ci.yml:689-693`, not `:515-520` — the item's *"on every leg"*
    is correct, since that step sits in the single matrix job with `fail-fast:
    false`.

- [ ] **M-core-packages** | four repairs to design.md that ride its opening sitting, §4.19's `#include` sentence among them | `design.md` §1.11, §3.5, §4.19, §4.20, Part 7 item 4 · `selfhost/emit/externs.hero:69-70` · `docs/panel/056`, `091`

    **Merged 2026-09-10** from two items, by author instruction: both are repairs
    to design.md, both ride the same opening sitting, and one editing pass closes
    all four. **Both bodies are kept whole below.**

    **Re-verified 2026-09-10: STILL OPEN, all four owed, and every pointer in the
    second body has moved.** `grep -n "declaration order" design.md` is **0**, so
    the `#include` invariant is still written only in code, at
    `selfhost/emit/externs.hero:69-70` (the item said `:72`, which is now the
    `hero_os.h` seed line of that same doc comment). `args_checked` appears **0**
    times in design.md and `validated` only outside §1.11 and §4.20, so both are
    still omitted; they are `spec/heroes-spec.md:204-205`, not `spec:186-190`.
    *"no aliases, no package hierarchy"* still stands at `design.md:2641`, and
    §3.5 still holds no paragraph naming what would return a project file.
    **Three pointers**: §1.11's Tier-2 list is `design.md:493`, §4.20 opens at
    `:2266`, Part 7 item 4 is `:2641`. **The second body's own evidence sentence is
    now false**: *"`grep -n unplaced design.md` hits only `:2407`"* — it hits
    `:2566`, `:2584`, `:2617` and `:2712`, and `:2407` is not among them.
    **And the `#include` sentence has to say more than it did**: the list now
    carries two unconditional seeds, `math.h` and `hero_os.h`
    (`selfhost/emit/externs.hero:76`), so *the order groups are declared in is the
    order of the includes* is true only after those two.

    **A FIFTH repair was found while verifying and it belongs here**, same class as
    the second: `design.md:806` (§4.1) still says *"there are no aliases and no
    wildcard"*, which `spec/heroes-spec.md:11` contradicts — `use syntax/decl as
    sd` has bound an alias since M-package-layout closed on 2026-09-02. The
    sentence to repair is design.md's, since §12 gives the spec precedence.

    **The first item, as it stood.** §4.19 owes one sentence: a group's `#include` order is load-bearing

    **Origin:** panel 091, the ffi-pragmatist's explicit *not covered by
    design.md*. Its home since 2026-09-07, when M-declared-freer closed without
    taking it. It named that milestone from 2026-09-04 on the ground that panel
    109 amends §4.19 and this is a §4.19 sentence — true, and the sitting ruled
    on ownership and never on the `#include` list, so the sentence stayed
    unwritten. It rides M-core-packages' opening sitting instead, which touches
    §1.11, §4.19 and §4.15 by its own six questions, and where a later item
    already parks three other design.md repairs for the same reason. The lesson
    is this file's own: *the next sitting that touches X* is a waiting condition
    and not a home, and naming a milestone did not fix that — what fixes it is
    naming a sitting whose AGENDA contains the question.

    Measured: `<jpeglib.h>` alone is 8 errors (`unknown type name 'size_t'`);
    `<stdio.h>` first, then clean. So the order in which groups are declared
    **is** the order of the `#include` list (`emit_externs.headers`, "in
    declaration order"), and it is the author's only lever on it. design.md
    §4.19 does not say so, which means nothing stops a later pass from
    reordering or thinning that list — and the sitting that would do it would be
    reasoning from a document that never mentioned the constraint. The sentence
    is owed whether or not anything is ever pruned; it is CLAUDE.md §11's
    expiring premise before it expires. Amending design.md Parts 1-11 is a panel
    path (CLAUDE.md §4), so this rides the next sitting that touches emission
    rather than convening one.

    **Where to look also:** `docs/panel/091` § What the ffi-pragmatist compiled.
    **Why it matters:** an invariant nobody wrote down is one somebody will
    optimise away.

    **The second item, as it stood.** three repairs to design.md that ride its opening sitting

    **Origin:** found 2026-09-03 reading forward (CLAUDE.md §1). They ride the
    opening sitting because that sitting touches §1.11 anyway.

    **(a)** §1.11's Tier-2 list (`design.md:474-478`) and §4.20's inventory
    (`:2273-2276`) omit `validated` and `args_checked`, landed 2026-08-24
    (`DESIGN-LOG:408`, `:413`); the spec has them (`spec:186-190`) and
    `suite_spec` polices the spec, not design.md. **(b)** Part 7 item 4
    (`design.md:2447-2449`) still reads *"no aliases, no package hierarchy"*
    after M-package-layout landed `use syntax/decl as sd` on 2026-09-02; `grep
    -n 'panel 099\|panel 100\|panel 101' design.md` is 0. **(c)** Panel 056
    deliverable B — *"a greppable paragraph in design.md §3.5"* naming what
    would return a project file — was ratified 2026-08-15 and never written:
    `grep -n unplaced design.md` hits only `:2407`, the comptime paragraph.

    All three are design.md Parts 1–11, so none is written without a sitting
    (CLAUDE.md §4); deliverable D, the same conditions in the ROADMAP's
    M-package-manager entry, was discharged 2026-09-03.

    **Why it matters:** a document that omits what shipped briefs the next
    sitting wrong.

- [ ] **M-core-packages** step 0 | four repairs before the first package, each a `fixedbugs` case, the macro-only probe among them | `selfhost/ir/place_store.hero:100` · `selfhost/cli/pointee.hero` · `selfhost/emit/extern_probe.hero:164-165` · `docs/panel/092`, `103`

    **Merged 2026-09-10** from two items, by author instruction, and the second
    named the first as its own witness: *"the macro-only item panel 092 filed above
    gains its witness"*. Same milestone, same step, same sitting, same file — four
    repairs and four `fixedbugs` cases, in one item. **Both bodies are kept whole
    below.**

    **Re-verified 2026-09-10: all four STILL OPEN, and no case exists for any of
    them.** Of **116** `fixedbugs-*` files under `tests/golden/`, none matches
    push, pointee, `htons` or `setsockopt`, and `grep -rln "htons\|htonl\|isascii"
    tests/ examples/` is empty. The `place_store` pointer is exact —
    `selfhost/ir/place_store.hero:100` still chooses `.push_owned` for a bound
    value — and `selfhost/cli/pointee.hero` (310 lines) still has no `const void`
    arm. The macro-only defect is stated in the emitter's own comment at
    `selfhost/emit/extern_probe.hero:164-165`: *"`(htonl)(a0)` is `use of
    undeclared identifier`, and so is `(&htonl)`"* — the item's `:153` lands inside
    that same block. **Two numbers were deliberately not re-run**: the 14.78 s
    against 0.00 s pair is a build timing (CL-025), and *"44 distinct C functions
    are probed by the corpus"* needs a build to reproduce. The *"none is
    macro-only"* half re-measures cheaply and still holds.

    **The first item, as it stood.** a macro-only C function has no golden case, and the parenthesised probe breaks it

    **Origin:** panel 092, the compiler-engineer's own condition 3. Its home
    since 2026-09-04: this item said *the next milestone that touches the FFI
    probe or the golden corpus* and named none, and step 0's repair (c) is
    already this item's own witness — `htons` on Darwin is defined only as a
    macro and the parenthesised probe reports it as *declares no `htons`*, which
    is false.

    `(htonl)(a0)` is `use of undeclared identifier 'htonl'` — and so is
    `(&htonl)`, so the alternative spelling does not save it. §4.19 promises
    *"Macros and `inline` functions are reachable"*, and the repair that closed
    the `_FORTIFY_SOURCE` hole narrows that promise for a name the header
    defines **only** as a macro. **Measured: 44 distinct C functions are probed
    by the corpus and none is macro-only**, so nothing fires today — which is
    exactly why it needs a case rather than a fix. The case is one `extern`
    group over a macro-only name (`htonl`, `isascii`, `major`/`minor` from
    `sys/types.h`), and the decision it forces is whether the emitter falls back
    to the unparenthesised form when the parenthesised one fails to compile —
    which cannot be asked of clang in one pass.

    **Where to look also:** `docs/panel/092` § Where the seats disagreed.
    **Why it matters:** a promise in the document with no case in the harness is
    a promise nobody will notice breaking.

    **The second item, as it stood.** three repairs before the first package, each a `fixedbugs` case

    **Origin:** found by the session's probes, 2026-09-03, on the Mac and in the
    Linux image.

    **(a)** `xs @ xs.push(c.to_u8().must())` copies the whole array on every
    push: 100,000 pushes in **14.78 s** against **0.00 s** when the value is
    bound first (`v = c.to_u8().must()` then `xs @ xs.push(v)`), `heroes
    build`'s default optimisation, this Mac; `selfhost/ir/place_store.hero:100`
    chooses `.push_owned` for a bound value and not for an inline `.must()`, so
    the runtime's copying `push` runs. Every byte-oriented package writes
    exactly this shape. **(b)** `@value: i32` against a `const void *` pointee
    (`setsockopt`) is `internal error: checking what the extern out-parameters
    point at failed` at exit 2 on both platforms — panel 103's pointee assertion
    writes `_Static_assert(sizeof(const void) == …)`, a legal author declaration
    blamed on the compiler; `SO_REUSEADDR` is what a restarted server needs.
    **(c)** `htons` on Darwin is defined only as a macro (`sys/_endian.h`, the
    prototype under `#if defined(lint)`), and the parenthesised probe's failure
    is reported as `ffi_unknown_name`, *"declares no `htons`"*, which is false —
    the macro-only item panel 092 filed above gains its witness; glibc declares
    the function beside the macro, so the same `.hero` runs on Linux and prints
    36895.

    **Where to look also:** `selfhost/emit/extern_probe.hero:153`.
    **Why it matters:** a package built on a quadratic `push` and an
    out-parameter the compiler cannot describe fails on its first real input.

- [ ] **M-core-packages** step 1 | `strings` is the first package because the corpus already wrote it | `examples/` · `tests/harness/strings.hero` · `docs/panel/057`, `097`

    **Origin:** measured 2026-09-03.

    **Four helpers copied by hand across `examples/`, sharable by nothing**:
    `function split_lines(` in 8 files, `is_space` in 8, `trimmed` in 7,
    `index_of` in 4, and `tests/harness/strings.hero` carries `trimmed`,
    `is_space`, `lines`, `contains`, `starts_with` and `ends_with` once more.
    `use` cannot reach a module outside the program's root
    (`selfhost/modules.hero:125`), no search path exists by ruling (panel 028
    R3), and the two doors into the language are shut: panel 097 condition 5
    closes `selfhost/library_source.hero`, panel 057 refused `path_join` as a
    built-in on Principle 0. A package is the only home, and the copies are the
    measurement of what it must hold before anyone invents it.

    **Where to look also:** `grep -rl '^function split_lines(' examples` ·
    `spec` § Built-ins.
    **Why it matters:** the corpus has already written the package, seven or
    eight times, without a name.

    **Re-verified 2026-09-10: STILL OPEN, and every count is UNDERSTATED.** With
    the item's own command: `split_lines` **9** files (said 8), `is_space` **9**
    (said 8), `trimmed` **7** and `index_of` **4** (both exact). And
    `tests/harness/strings.hero` carries **14** functions, not the six listed —
    `ends_with, without_suffix, split_on, lines, base_name, trimmed, is_space,
    contains, starts_with, without_prefix, to_number, index_of, split_text, words`.
    **`strconv` joined this step's package table on 2026-09-10** and `to_number`
    above is the copy that proves why.

- [ ] **M-core-packages** | a C symbol spelled with a Heroes keyword cannot be bound at all | `selfhost/keywords.hero` · `docs/panel/094` R2, R3 · `design.md` §4.19

    **Origin:** panel 013's ffi-pragmatist, pre-existing and option-independent;
    carried on the panel watch list from 2026-08-03 until it was retired
    2026-09-04. Its home since 2026-09-07, when M-declared-freer closed without
    reaching it — and the reason is worth keeping, because it is the second time
    this item has outlived its address. Panel 109 opened §4.19's reserved
    annotation vocabulary and `owned` is the first word in it, which is why the
    item was homed there; what the sitting did NOT do is rule on a second
    contextual word, and Principle 0 gave it no reason to. **M-core-packages is
    where a reason appears**: a package that binds a real library is the first
    thing in this repository that can meet a C symbol spelled with a Heroes
    keyword, and that milestone's opening sitting already has §1.11 and §4.19 on
    its agenda. Its previous home read: the panel watch list deferred it to
    §4.19's deferred annotation vocabulary at the milestone now named
    M-ffi-ladder, an address that expired when that milestone closed 2026-08-12,
    and panel 109 is what opened that vocabulary — `design.md:2120-2125`
    reserved it, *"Reserve a keyword"*, and `owned <C function>` is the first
    word in it.

    **And §4.19 has no way to say another name for it.** Measured at panel 013
    over the macOS SDK and homebrew headers, in declarator or field position:
    `function` occurs **571** times, `func` 29, `assert` 11, `test` 5, `match`
    3 — and `selfhost/keywords.hero` reserves every one of them, so `extern
    function function(…)` has no spelling. **Re-measure the five counts at the
    opening rather than trusting these**: the headers on this machine have moved
    twice since.

    **What is NOT the answer, decided and on the record**: panel 094 refused a
    rename clause (ratified 2026-08-26) — but on a premise this case never
    reached, that *"one C symbol carries one arity"*, and its file contains
    **zero** occurrences of `reserved`, `collide` or `registry`, so the
    collision was never priced. R3 of that sitting binds the spelling if it ever
    lands: **`tag`, not `= "cname"`**, which is already the word an `extern`
    record uses for the C tag (`spec:231`) and is contextual, so it costs no
    keyword. The cheap question for the sitting is whether `owned`'s arrival
    makes a second contextual word in the same position free, or whether
    Principle 0 still holds this one out — nothing on the closure list binds a
    colliding symbol, and a binding nobody can write never appears in a corpus,
    so no trigger can ever fire for it.

    **And the same sitting names the shape rule the family already obeys**
    (author question 2026-09-06, M-core-packages' question (vii) in
    `docs/ROADMAP.md`, split from M-reflection-verdict's (iii) because the
    second-contextual-word question is here): six contextual words today —
    `owned`, `tag`, `partial`, `link`, `package`, `as` — each after the thing it
    modifies, each in one position, each carrying a check, none in
    `selfhost/keywords.hero`'s table of 21 (measured 2026-09-07 from
    `selfhost/parse/`); the rule stated by panels 094 R3, 109 and 114 R7
    separately and written as a rule nowhere; whether design.md §4.19 names it,
    so that a `tag` for a colliding symbol, the buffer case M-declared-freer
    queued and panel 003's discardable mark follow it without a fourth
    re-derivation.

    **Where to look also:** `docs/panel/013-function-type-marker.md:179-184` ·
    `docs/panel/114-the-question-was-not-which-platform.md:244-252` ·
    `design.md` §4.19, `:2120-2125` (`:2139-2144` today) ·
    `selfhost/parse/members.hero:81-94` · `spec:231`.
    **Why it matters:** §1.11 says everything comes from C, and this is the one
    class of C name the language cannot reach — the hole is invisible because
    the program that would find it cannot be written.

    **Re-verified 2026-09-10: STILL OPEN, and both of its measured negatives
    hold exactly.** `selfhost/keywords.hero:35-57` is the closed table at **21**
    words and none of the six contextual words is in it; over `docs/panel/094`,
    `reserved`, `collide` and `registry` are still **0**, **0** and **0**. Two
    pointers moved: design.md's *"Reserve a keyword"* is `:2178`, and the `tag`
    sentence is `spec/heroes-spec.md:254-255` where the item says `spec:231`, which is
    now a fence. **The five SDK counts were deliberately not re-run**: they need a
    declarator scan of the macOS SDK plus the Homebrew headers, which the item itself
    asks for at the opening.

- [ ] **M-core-packages** | copy-on-write's `refcount == 1` is a test and then a mutate, and nobody could race it | `runtime/parts/cow.c:44`, `:78`, `:83` · `runtime/parts/map-write.c:127` · `docs/panel/113`

    **Origin:** `docs/panel/113`, 2026-09-06; the finding is both compiling
    seats', independently, in separate checkouts, and the ffi seat's positive
    control is what makes their silence admissible. **Re-homed a second time
    2026-09-06, at M-thread-stacks' close**: that milestone ran threads and did
    not meet the return condition either, so the item moves on rather than
    expiring with the milestone that failed to close it. Its home is now the
    next milestone whose own work runs many threads for a real reason — a web
    server is one connection per thread — because this is a WATCH item and a
    watch needs traffic, not a schedule.

    **The sitting was convened on this and re-aimed itself.**
    `runtime/parts/cow.c:44`, `:78`, `:83` and `runtime/parts/map-write.c:127`
    read `refcount == 1` and then mutate; step 3 made the read whole and did not
    make the pair single. **What no seat could do is race it.** With
    `parts/thread.c`'s guard patched down in their own copies: 8 threads and
    240,000 mutations of nested `[[str]]` and `{str: i64}` under ASan and
    ThreadSanitizer, **exit 0, zero warnings**; and 4 foreign threads with
    100,000 concurrent mutating touches of one shared header, same result, for a
    `[i64]`, a `{str: [i64]}` and a `str`. **The instrument was proved live in
    the same session**: a hand-made race on `hero_array_push_owned` is `data
    race … cow.c:80`, exit 134, 3 races, length 246821 instead of 400000.

    **The reason is the grammar, not luck**: a callback parameter arrives
    BORROWED and the emitted body increfs before it can store (`t1 = h0_a;
    hero_array_incref(t1); h1_ys = t1;`), and the two ways round that are closed
    — `@` inside a function type is `error[expected_type]`, and writing a
    callback parameter is `error[not_mutable]`. **Option A, a
    compare-and-exchange from 1 to a busy sentinel, is deferred and NOT on
    cost**: measured cheap (0.37 s against 0.36 s over 40M stores) and unsound
    as scoped, because the sentinel must be held across the CALLER's mutation —
    `hero_array_set` does `drop(place); memcpy(...)` after `unshare` returns —
    so a panic inside `drop` would strand a block busy forever. **Option B is
    vetoed by both compiling seats**: 400,000 stores at 24.50 s against
    40,000,000 at 0.36 s, O(n²) and not a percentage.

    **THE RETURN CONDITION, and it is the whole item now**: a program that
    corrupts memory through those four sites **with every reference counted** —
    that is, without C releasing a reference it still lends. Produce it and A
    lands with the critical section widened to cover the caller. The searches
    that failed are named in the sitting rather than hidden, and what was NOT
    tried is named too: `sort` through a function pointer, the `eq` and `hash`
    descriptor walks, and the drop-list drain under contention.

    **Where to look also:** `docs/panel/113` § What did NOT reproduce.
    **Why it matters:** a window nobody can reach is not a repair anybody should
    ship, and the sitting's own measurements are what say so.

    **Re-verified 2026-09-10: STILL OPEN, and every citation is exact.**
    `grep -rn 'refcount == 1' runtime/parts/*.c` returns exactly the four sites the
    item names, `cow.c:44`, `:78`, `:83` and `map-write.c:127`, and `cow.c:38-43`
    still carries the sitting's note that until this lands, `parts/thread.c`'s guard
    is what keeps any other thread out. The return condition — a corrupting program
    with every reference counted — is unmet, so the watch stands. Its 240,000-mutation
    figures need threads under TSan and were not re-run.

- [ ] **M-core-packages** | the loopback HTTP server, one connection per thread | `design.md` Part 7 item 13 · `runtime/parts/thread.c` · `docs/panel/111`

    **Origin:** carried out of the callback-boundary item that closed at
    M-c-callbacks step 0, 2026-09-05. **Re-homed 2026-09-06**: the threads it
    needs landed at M-isolated-threads and the ten example programs are the
    witness Part 7 item 13 was owed; what is still missing is SOCKETS, which is
    that milestone's question (iv) — `sockaddr` differs between Darwin and glibc
    and Windows is winsock.

    **The permission that blocked it is gone and the witness was never
    written.** A loopback HTTP server answered `curl` from Heroes on 2026-09-03,
    single-threaded; the version with one connection per thread is what
    design.md Part 7 item 13 exists for, and it could not be written at all
    while a function value could not cross the FFI. It can now —
    `pthread_create` binds, runs and joins, measured 2026-09-05 — and what it
    will meet is the four corruption classes panel 111 built, which is precisely
    why it belongs at the milestone that makes them unreachable rather than at
    the one that opened the door. **Note what the guard does to it today**:
    every worker stops by name at its entry, so the witness is not runnable
    until isolation lands, and that is the correct state rather than a
    regression.

    **And the spelling is not portable, measured 2026-09-05 on both machines**:
    `pthread_t` is an opaque pointer on Darwin and an `unsigned long` of 8 bytes
    on glibc, so the same binding is `@thread: ptr` here and `@thread: u64`
    there — both compile and run on their own machine, neither compiles on the
    other, and the compiler says which with a `guess` fix on each. That is
    M-core-packages' platform-typedef question (item (v), `clockid_t`) arriving
    in a second place, and this milestone meets it first.

    **Why it matters:** a concurrency milestone with a model and no program
    decides nothing, and this is the program.

    **Re-verified 2026-09-10: STILL OPEN in substance, one sentence now FALSE.**
    Still open: no server program exists and nothing in the tree binds `socket`,
    `bind` or `listen`. **The false sentence is the guard's**: *"every worker stops by
    name at its entry, so the witness is not runnable until isolation lands."*
    Isolation landed at M-isolated-threads on 2026-09-06, and
    `runtime/parts/spawn.c:192-193` has `hero_spawn_enter` call `hero_thread_claim()`,
    so a Heroes-spawned worker is home and the guard never fires on it — today it
    refuses only a thread **C** made itself (`runtime/parts/thread.c:10-68`), which is
    the hole it was written for. `examples/threads/main.expected` shows eight threads
    answering. So the witness IS runnable now, and that is the item becoming cheaper
    rather than staler.

- [ ] **M-core-packages** | `compile "gfx.c"` waits for a witness, not for an argument | `docs/panel/114` § R6, R7 · `selfhost/cli/libraries.hero` · `examples/sdl/main.hero`

    **Origin:** settled out of `docs/work/DECIDE.md` on 2026-09-06 by author
    instruction, after `docs/panel/114` R6 closed the thin half the same day.

    **The thin case is answered and needs no language form**: one header the
    package ships with `#ifdef` inside and `static inline` wrappers, measured on
    macOS and on the Windows box, one `.hero` source, exit 0 on both, and
    `--emit-c` carrying zero platform words. SDL is that case and
    `examples/sdl/main.hero` binds it today with no platform word anywhere in
    the file; the `link` half is `package`, which panel 050 said *subsumes the
    platform axis panel 049 refused while putting no machine's name in any
    program*.

    **The thick case is refused by Principle 0 and not by a judgement about
    `compile`**: a wrapper with hundreds of lines of implementation wants a `.c`
    file the compiler builds, nothing in the closure list calls one, and no Part
    11 metric moves — so it waits, regardless of elegance (CLAUDE.md §2).
    **What is owed here is a WITNESS, not a decision**: a package in this
    milestone's own work whose C half is too large for a header of `static
    inline` functions. If it appears, panel 036's deferral reopens with its
    terms already priced — a group clause (`extern "gfx.h" compile "gfx.c"`) was
    panel 036 P2's own veto, so the package-level shape is the one that has
    never been judged, and panel 114 R7 has already recorded what a
    declaration-level form would cost. **If it does not appear in a milestone
    about packages that compose, that is the answer** and the deferral closes
    for good.

    **Where to look also:** `docs/panel/036-the-ffi-ladder.md:275` ·
    `docs/panel/050-*.md`.
    **Why it matters:** the one question a Heroes user asks that has a good
    answer for small libraries and no answer for large ones, and it has been
    waiting for a witness since August rather than for an argument.

    **Re-verified 2026-09-10: STILL OPEN, correctly waiting, and today's commit
    sharpened it.** `grep -rn 'compile "' selfhost/ spec/ design.md` returns one hit,
    `design.md:2152`, the record of panel 036's deferral; no surface form exists and
    `examples/sdl/main.hero` still carries zero platform words. **The whole tree now
    holds exactly one C file under `examples/`** — `examples/gallery/13-lease.h`,
    seven lines of `static inline` wrappers, landed 2026-09-10 with `7965174d`. That
    is the **thin** case again, which is panel 114 R6's own answer, so the deferral
    still has no thick witness and the wait is doing its job.

- [ ] **M-core-packages** step 1 | `strconv`, because six programs write the same digit loop by hand | `selfhost/check/builtins.hero:73-95` · `examples/json/`, `calculator/`, `ini/`, `spreadsheet/`, `csv/`, `interpreter/` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    the first of its four silences. The package table gained a `strconv` row the
    same day.

    **What is missing and how it is known**: `to_i64` does not take a `str`
    (`selfhost/check/builtins.hero:73-95` — an integer or a float, and a `str`
    argument is a diagnostic), so **every program whose input is text builds its
    numbers digit by digit**: `json`, `calculator`, `ini`, `spreadsheet`, `csv`,
    `interpreter`. `tests/harness/strings.hero` carries a `to_number` of its own,
    which is the seventh copy. Go's tree has `strconv` for exactly this.

    **Width and precision belong here too, and only here for now.** There is no
    route to a padded integer or two decimal places, and the compiler hand-writes
    four padders (`cli/measure.hero:305`, `cli/doctor.hero:161`,
    `print/fmt.hero:145`). A `f64` to two places is integer arithmetic and a point,
    so it is this package's work. **It becomes a question about the language only
    if this package cannot do it** — a format spec inside an `f"…"` hole is the
    shape it would take, and `f"…"` landing on 2026-09-09 is what makes it
    thinkable — and that is a return condition rather than a plan.

    **It adds no built-in and no language form**: panel 097 condition 5 keeps
    `selfhost/library_source.hero` closed, and this is ordinary Heroes.

- [ ] **M-core-packages** step 6 | a service that stops cleanly is not writable, and the server step is where that stops being theoretical | `spec/heroes-spec.md:47` · `selfhost/check/ffi.hero:262-268` · `runtime/parts/thread.c:10-68`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    its third silence.

    **What is true today, measured.** A signal handler is *declarable*: a callback
    may stand as a **parameter** (`selfhost/check/ffi.hero:262-268`), never as a
    result. But **there are no mutable globals** (`spec:47`), so a handler has no
    way to record that it fired — it can only call `exit`, which is not a graceful
    stop. So `SIGINT` on a server that should drain its connection and close its
    database is not writable in Heroes, in any spelling, and nothing in the record
    says so.

    **Why it is homed on the HTTP server step and not on the opening sitting**: it
    binds nothing until there is a server to stop, and the step that writes one is
    where the answer is cheap or the wall is real. **What it must not quietly
    become** is a mutable global: that is Part 6, permanently. The shapes to price
    are a runtime part that owns the flag and answers a Heroes call, and the
    server's own loop asking between connections.

- [ ] **M-core-packages** step 6 | the long run: every program in the net exits, and a service is the one that stays up | `tests/harness/suite_corpus.hero` § configurations · `runtime/parts/alloc.c` § `hero_runtime_check_leaks` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means. The
    finding is structural rather than a defect: `main.expected` is the shape of the
    whole net, so **every instrument here watches a program that starts, prints and
    stops**.

    **What that makes invisible** is exactly what production meets first: a
    refcount that drifts by one per request, a descriptor never closed, memory that
    grows one connection at a time. None of it is reachable by a corpus of
    one-shot programs, and this milestone's own acceptance says why — a listening
    server does not fit `main.expected`, so its loopback case is server and client
    in one process with deterministic output.

    **Two cheap additions on top of that case, not a new programme**: drive the
    loopback server for N requests rather than two, and read the allocation count
    at the end; and assert `hero_runtime_check_leaks()` after the long run rather
    than after the short one. If the count is flat across N, the class is shut for
    the shape the corpus can see. **What it is not**: an endurance suite, a soak
    farm, or anything that goes red at random — CLAUDE.md's own warning about an
    instrument nobody trusts applies here first.

- [ ] **M-core-packages** step 6 | only an `i64` crosses into a thread, and the spec never says the word | `runtime/hero_os.h:251-256` · `selfhost/check/ffi.hero:76-92` · `design.md` Part 7.13 · `examples/threads/main.hero`

    **Origin:** author decision 2026-09-10, § What production-ready means row 6.

    **Two measurements, and the second is the one nobody had written down.**
    Concurrency arrives as `extern "hero_os.h"` with `hero_thread_spawn`,
    `hero_thread_join` and `hero_thread_limit`, where **only an `i64` crosses in
    each direction** and the checker enforces it — a `[T]` or `{K: V}` inside the
    callback's signature is `error[ffi_type]`
    (`selfhost/check/ffi.hero:76-92`). That is Part 7.13's data-parallelism rung
    exactly as designed, and `examples/threads/main.hero` says so in its own first
    paragraph: *"it is the model"*. **And `spec/heroes-spec.md` never says thread,
    concurrency, spawn or parallel** — zero occurrences — so a program written from
    the spec alone, which is the one reader this language exists for, cannot use a
    thread at all.

    **Why it is the server step's question.** `docs/ROADMAP.md` § M-web-framework
    describes *"a `Request` record … exactly the message the separate-heap model
    wants, small and copied once"*, and **no program can write that today**. One
    connection per thread needs only the descriptor, which is an `i64`, so the
    server step is where it is found out whether the rung is enough — and if it is,
    that is the answer and the spec is what changes, not the runtime.

    **What it may not do**: widen the model on an appetite. Panel 111 measured what
    an unguarded `str` across 32 threads costs — `heap-use-after-free` in nine ASan
    runs of ten — and `runtime/parts/cow.c`'s test-then-mutate is still this
    milestone's own open watch item.

- [ ] **M-core-packages** | `heroes test` cannot run one test, and the binary already can | `selfhost/cli/verbs.hero:104-163` · `selfhost/cli/table.hero` · `CLAUDE.md` §10

    **Origin:** author decision 2026-09-10, § What production-ready means. Homed
    here because this is the milestone that multiplies both the packages and their
    `test` blocks; it is small enough to ride any commit that touches the CLI
    table.

    **What is true today**: there are **545** `test` blocks in the corpus and no
    way to run one. `run_test` (`selfhost/cli/verbs.hero:104-163`) compiles once
    and then loops over every title, handing the binary an **index** — so the
    binary can already run test *N* and no flag exposes it. Tests are collected
    across every `use`d module, so a run cannot even be scoped to one file.

    **What it owes**: §10's stopping-rule argument, like any other flag. The
    argument available is that the harness itself will need it once a package's
    tests are a corpus of their own, which is the same shape that admitted
    `--operator` for `heroes mutate`. **A flag, never a verb.**

- [ ] **M-core-packages** | `heroes build --help` is an error at exit 2, which is a stranger's first minute | `selfhost/cli/table.hero:139-176` · `selfhost/cli/argv.hero:78-81` · `selfhost/cli/help.hero:22-46`

    **Origin:** author decision 2026-09-10, § What production-ready means.

    **Measured**: `./heroes build --help` answers ``error: `build` does not accept
    `--help` — it accepts --dump-ir, --emit-c, -o, --include, --library,
    --sanitize, -O0, -O2`` and exits **2**, the code reserved for *the tool could
    not run*. The message is good and the verdict is wrong: asking a subcommand for
    its help is not a failure, and every tool a production user has ever run
    answers it.

    **Where the answer already lives**: `--help` and `-h` are global words handled
    outside the flag table (`selfhost/cli/argv.hero:78-81`), and
    `selfhost/cli/help.hero:22-46` already prints per-command text from the one
    table that both parses argv and prints help, so the two cannot disagree. What
    is owed is that a subcommand's `--help` reaches that printer instead of the
    unknown-flag arm, and the exit code that goes with it — **0**, since the tool
    did what it was asked.

- [ ] **M-package-manager** | a bindings module is invisible to `heroes check` | `selfhost/cli/check.hero` · `docs/panel/091` · `docs/panel/082` R3

    **Origin:** panel 091, found by the ffi-pragmatist unasked. Load-bearing
    from M-separate-compilation onward.

    `heroes check badbind.hero` on a module that is nothing but an `extern`
    group is **exit 0 with zero output** — `check` never runs clang, so nothing
    verifies the group — while `heroes build` on a caller that reaches one of
    its two declarations reports `error[ffi_return_type]` **on the bindings
    module's own line**. This is the same shape as panel 082 R3's *check accepts
    ⇒ build succeeds* gap, one department over, and it becomes load-bearing
    exactly here: separate compilation is what makes a pure bindings module a
    normal thing to write, and M-package-manager is what makes it a thing you
    **distribute**. Whoever fixes it should read 082 R3 first — that item
    refused running `mono` inside `check` on a measured cost, and running clang
    inside `check` is the same trade at a larger price.

    **Why it matters:** a check that accepts everything is not a check, and a
    distributable binding is the one artifact whose whole value is that somebody
    verified it.

    **Re-verified 2026-09-10: STILL OPEN, and the mechanism is unchanged.**
    `selfhost/cli/check.hero` reaches no clang and no probe — the only `probe` in it
    is an unrelated local counter at `:234-241` — while clang is reached on the build
    path alone, `selfhost/cli/produce.hero:57` and `:295`. No fixture exists: the
    `badbind` name appears in `docs/panel/091`'s brief and in this list, nowhere
    else.

- [ ] **M-doc-generator** | the stopping rule is the opening's first question | `CLAUDE.md` §10 · `docs/measurements/003-closure-list-audit.md:82-100`

    **Origin:** author instruction 2026-09-03, scheduled against the
    recommendation, `DESIGN-LOG.md:539`.

    **`heroes doc` fails CLAUDE.md §10's stopping rule today, and the precedent
    that says so refused two verbs this file had scheduled.**
    `docs/measurements/003` rider 3: `outline` and `explain` were in the ROADMAP
    under what is now M-generics-library and neither entered, because the
    fixpoint invocation, the golden harness and the Part 11 harness type none of
    them and no Part 11 effect was measured. `heroes doc` appears in the record
    once, as Part 6's promise (`design.md:2400`, the literate-source row:
    *"generates the document instead — one direction only"*). The two routes
    that admit it are named in the ROADMAP entry: the site or the guide
    consuming its output for the packages' API, or Part 6's promise as a
    §1-derived argument. A sitting convened without answering this first is a
    sitting convened on a settled question.

    **Where to look also:** `design.md:2400` · `spec:14-15`.
    **Why it matters:** a place in the table is not a warrant, and the ROADMAP
    already says so.

    **Re-verified 2026-09-10: STILL OPEN.** No `doc` verb exists in
    `selfhost/cli/`, and `docs/measurements/003`'s rider 3 still reads *"Neither is
    admissible"* verbatim. One pointer moved: Part 6's literate-source promise is
    `design.md:2494`, not `:2400`.

- [ ] **M-panic-location** | what a panic says today, so the "before" is on the record | `runtime/parts/panic.c:21-25` · `runtime/parts/stack.c:202-213`, `:292`

    **Origin:** measured 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **`hero_panic` flushes stdout, prints `panic: <msg>` and calls `abort()`,
    and no abort in the language names a `.hero` file, line or function**
    (`runtime/parts/panic.c:21-25`; the out-of-range index, overflow, `.must()`
    and the character-splitting slice all funnel through it). The one exception
    is the stack guard, which walks back with `dladdr` to the first Heroes frame
    on POSIX (`runtime/parts/stack.c:202-213`) and on Windows names the failure
    and not the function until dbghelp is measured (`stack.c:292`). The
    generated C carries `#line` (CLAUDE.md §7), so `__FILE__`/`__LINE__` at each
    aborting runtime call already resolve to the `.hero` position.

    Owed: the location passed to every abort, `HERO_RUNTIME_ABI` +1 with the
    two-phase edit (`seed/README.md`), one `fixedbugs` case per abort class, the
    three platforms before the commit, and the corpus leg's time before and
    after — the location is passed, never computed.

    **Where to look also:** `seed/README.md` · `CLAUDE.md` §7, §9.
    **Why it matters:** a program that stops without saying where is §1.12's
    goal met halfway.

    **Re-verified 2026-09-10: STILL OPEN, and the behaviour is identical.**
    `runtime/parts/panic.c:21-25` is still `fflush(stdout)`, `fprintf(stderr,
    "panic: %s\n", msg)`, `abort()`, and `hero_panic` still takes only a message, so
    nothing passes a location; `HERO_RUNTIME_ABI` is **22**. **Two pointers moved
    inside one file**: the `dladdr` walk is `hero_stack_blame` at
    `runtime/parts/stack.c:237-249` (`:202-213` is now `hero_stack_regs`), and the
    Windows note is at `:426-433`, not `:292`.

- [ ] **M-panic-location** | the Windows quoting round trip has no test, and the comment claimed one | `runtime/parts/run.c` § quoted · `tests/harness/suite_records.hero` § citations

    **Origin:** measured 2026-09-06, found by the citation check the same day it
    learned to read the compiler's own comments — a claimed test is a citation
    like any other, and this is the class that check exists for, one level up
    from a path that merely moved. Its home since 2026-09-07: M-declared-freer
    closed without touching `runtime/parts/run.c`, which was always the item's
    real condition rather than that milestone's name. M-panic-location is the
    next milestone whose work IS the runtime — every abort gains a location,
    `HERO_RUNTIME_ABI` moves with the two-phase edit, and its own scheduling
    item names the Windows half by file, `runtime/parts/stack.c:292`, where the
    platform names the failure and not the function. A test about Windows
    quoting rides a milestone that is already on the Windows box; it rides a
    step rather than convening anything.

    `runtime/parts/run.c`'s `quoted` builds the Windows command line — a
    backslash before a quote is doubled, `C:\dir\` at the end of a quoted word
    needs `C:\dir\\` — and it REPLACED a function that had a test,
    `cli_shell.hero`'s `sq`. Its comment claimed a test of its own until
    2026-09-06 — `tests/golden/run/win-quote-round-trip.hero` (2026-09-06),
    *"asserts the round trip on the words that break naive implementations"*.
    Measured 2026-09-06: `git log --all --diff-filter=A --` over
    `tests/golden/run/win-quote-round-trip.hero` (2026-09-06) finds **zero**
    commits, and `ls tests/golden/run/ | grep -ci quote` is **0**. The
    file was never written; the comment was corrected to say what is true, which
    is that the test is owed.

    **Where to look also:** the Windows box,
    `docs/environment/windows/WINDOWS-MACHINE.md`.
    **Why it matters:** a replacement that loses its predecessor's test is a
    regression nobody can see, and the comment that says otherwise is what stops
    anybody looking.

    **Re-verified 2026-09-10: STILL OPEN.** Its own command still answers
    zero: `ls tests/golden/run/ | grep -ci quote` is **0**, and `grep -rn win_quote
    tests/` finds nothing. The comment half IS repaired — `runtime/parts/run.c:145-153`
    now says in the file itself that the function *"HAS NONE OF ITS OWN"* test and
    that the round trip is owed to this list, and adds a *"NOT VERIFIED on a real
    Windows CRT"* line; `hero_run_win_quote` is at `:154`. The test is still
    owed.

- [ ] **M-typed-inspection** | the mechanism is the opening sitting's first question | `CLAUDE.md` §10 · `selfhost/cli/table.hero` § run_flags · `docs/ROADMAP.md` § M-vscode-extension

    **Origin:** measured 2026-09-06, the cheap route RUN rather than argued.
    §10's stopping rule is asked before it.

    **Thirty lines of lldb Python turned an opaque array into its elements with
    no compiler change and no runtime change.** It read `len` out of the header,
    resolved the `elem` descriptor pointer to the symbol `hero_desc_str`, found
    the C type `HeroStr` and printed `"ada"` and `"grace"`; `nm` shows a user
    type links as `_h_desc_Room_desc`, so **the descriptor's own symbol name is
    the type name `HeroDesc` does not carry**, which is `runtime/parts/sort.c`'s
    pointer-identity trick one level up. So a `name` field on `HeroDesc` is
    refused before it is proposed: `HERO_RUNTIME_ABI` +1 to buy a string the
    linker already holds.

    What the sitting must rule: whether CLAUDE.md §10's *"never a script"*
    forbids a formatter the BUILD emits and nobody types (Rust ships
    `rust-lldb`, a wrapper script, which is exactly what §10 refuses); whether
    the surface is `heroes run --debug` (a flag: same input, same question,
    different how, and at `-O0` because `run` defaults to `-O2` and the locals
    are gone there), a new verb (which §10 admits only with a proven overload),
    or **nothing** (the conservative reading, refused on §12: two invocations
    cannot guarantee the formatter and the binary came from one build, and a
    stale formatter shows the wrong variable's name at exit 0).

    Not this milestone's: calling a generated `to_str` inside the stopped
    process, which allocates on that process's heap on whatever thread lldb
    picks, after M-isolated-threads gave every thread its own.

    **Where to look also:** `design.md:594`.
    **Why it matters:** the mechanism decides whether the compiler changes at
    all, and three of the four routes leave it untouched.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held, no surface landed.**
    `grep -rn '"--debug"' selfhost/` is empty and `selfhost/cli/table.hero:90-97`'s
    `run_flags` are unchanged. One pointer moved: *"No typed variable inspection in
    v1"* is `design.md:613`, not `:594`.

- [ ] **M-typed-inspection** | what a stopped program shows today, so the "before" is on the record | `selfhost/emit/mangle.hero` · `selfhost/emit/body.hero` § prologue · `runtime/heroes_runtime.h`

    **Origin:** measured 2026-09-06 on this Mac, with lldb in batch mode over a
    hand-written program carrying one of each shape.

    **Half the promise already works and four things are broken, and nobody had
    run it.** Works: a breakpoint on a `.hero` line resolves and is hit with the
    source line printed, `bt` names Heroes frames at `.hero:line`
    (`h_dbg_total(...) at dbg.hero:19`), a local carries the author's own
    spelling behind an index (`h3_base = 7`), a `str` shows its text, a record
    shows its fields (`h0_p = (f_x = 3, f_y = 4)`).

    Broken: `p p` is `error: use of undeclared identifier 'p'`, so the author
    must know the mangling (`selfhost/emit/mangle.hero` § slot) and lldb's
    expression parser is C++; a `[T]` and a `{K: V}` are an opaque
    `HeroArrayHeader *`; a `T?` prints **both** arms including a garbage `err`
    half, under a hashed type name (`h_0opt_e201354`); and `frame variable`
    dumps **139** locals in one blessed emission
    (`tests/emission/run-adversarial-aggregate-overwrite.c`, 111 named and 28
    temporaries) against **247** in `syn/expr.hero::compared`, because §7 hoists
    every local to the prologue and `frame variable` has no name filter.

    Owed at the milestone: this table re-run as the "after", and the census that
    splits those 139 into temporaries, `$`-synthetic slots and real bindings,
    which is what decides whether the last step needs a slot table at all.

    **Where to look also:** `selfhost/ir/containers.hero` § SlotKind ·
    `runtime/heroes_runtime.h` § HeroArrayHeader, HeroDesc.
    **Why it matters:** the compiler is a 55,050-line Heroes program and the
    person learning from it cannot see a value in it.

    **Re-verified 2026-09-10: STILL OPEN, one number STALE, one UNSETTLED.**
    The compiler is **57,120** lines of Heroes, not 55,050 — which is what
    `docs/ROADMAP.md` says today, so the old figure survives only in
    `docs/journal/036-declared-freer.md:156`, where a record keeps what it measured.
    The **139 locals** split into 111 named and 28 temporaries is an lldb
    `frame variable` figure and is **UNSETTLED** without running lldb; declaration-line
    proxies give 103 to 151 depending on the pattern, which is why the census is this
    item's own step. And `syn/expr.hero::compared` is
    `examples/interpreter/syn/expr.hero:64`, not a compiler module — the item says so
    in full further down, and the short form is what misleads.

- [ ] **M-typed-inspection** | the claim with no live test, and the harness has no lldb row | `.github/workflows/ci.yml` · `tests/harness/main.hero` · `docs/panel/085`

    **Origin:** measured 2026-09-06.

    **`design.md:616` and `docs/ROADMAP.md:452` both assert that a golden runs
    lldb in batch mode and asserts a breakpoint on a `.hero` line is hit, and
    nothing has run it since 2026-08-19.** The golden is
    `archive/bootstrap-rs/heroes-cli/tests/golden.rs`, which M-bootstrap-archive
    left where nothing builds it; `.github/workflows/ci.yml` installs lldb on
    the Linux leg for it, describes it as executing in four comments, and runs
    no cargo at all; `tests/harness/` has no lldb suite.

    Owed: one harness suite driving lldb through `shell.run`'s argv list and
    watchdog, carrying the three guards the archived test bought with failures
    (lldb wrote nothing on either stream · the breakpoint is pending with no
    locations · the file, the line and `stop reason`) plus the **stepping** half
    that never had a test, and **its own falsifier run once by hand and quoted
    in the commit body** — `-g` deleted from `selfhost/cli/flags.hero`, the
    suite must go red. It also settles `docs/panel/085` B2's own condition,
    which said the lldb class *"was not tried"*. Windows is stated rather than
    silent: `lldb.exe` exits `0xC0000135` before running a command, so what that
    leg gets is a `heroes doctor` row naming the absence.

    **Where to look also:** `CLAUDE.md` §9.
    **Why it matters:** a debugger suite that passes without DWARF is a
    decoration, and a promise with no instrument is how this one went eighteen
    days unnoticed.

    **Re-verified 2026-09-10: STILL OPEN, and half of its cited claim never
    existed.** Still open: `tests/harness/` holds 20 suites and no lldb suite, and CI
    still installs lldb on the Linux leg (`.github/workflows/ci.yml:271`) for a test
    it never runs. **The false half**: *"`design.md:616` and `docs/ROADMAP.md:452`
    both assert that a golden runs lldb in batch mode."* `grep -n "batch mode"
    design.md` is **empty** — design.md never claimed it, its lldb sentences being
    `:614`, `:635-636`, `:664` and `:2886` — and the ROADMAP's claim is at **`:500`**.
    The same stale pair stood in `docs/ROADMAP.md:1737` and in
    `tests/harness/suite_records.hero`, and **the second was repaired on 2026-09-10**
    in the commit that carries this verification.

- [ ] **M-generated-programs** | the catalogue of shapes, enumerated from the world | `tests/golden/fixedbugs/` · `docs/work/DONE.md`, the defect entries

    **Origin:** scheduled with the chain row, 2026-09-06, out of the author's
    instruction that the hunt look at the bugs found in other similar
    compilers. At its first step, and this item is a starting point rather than
    the list.

    **What one search found the day the row entered.** Csmith (Utah) generates
    random C programs and found hundreds of latent defects in GCC and LLVM by
    differential testing; YARPGen (Intel) generates programs free of undefined
    behaviour and reported **more than 220** bugs to GCC, LLVM and the Intel
    compiler, its own stated contribution being *generation policies* for
    diversity; a survey of compiler fuzzing exists (arXiv 2306.06884) and an
    OOPSLA'19 study asks how much the bugs found this way matter in practice;
    Zig carries an issue titled *Compiler crashes found with fuzzing*.

    **The nearest corpus is this repository, and it was measured rather than
    recalled**: **fourteen** defect entries in `docs/work/DONE.md`, **76**
    regression cases named after one (49 with the `fixedbugs-` prefix under
    `check/`, `run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`), and
    **63** points in `selfhost/` where the compiler declares a case impossible.

    **What is owed**: a `docs/measurements/` file naming each shape with its
    source and marking what was read and what was not, because CLAUDE.md §1 says
    an enumeration carries where it came from — and the two shapes the author
    named, the C boundary and depth, enter it with a number beside them rather
    than as an impression.

    **Where to look also:** `https://dl.acm.org/doi/10.1145/3428264` ·
    `https://arxiv.org/pdf/2306.06884` ·
    `https://github.com/ziglang/zig/issues/10121`.
    **Why it matters:** a generator aimed at the shapes one session can think of
    measures that session, and the whole point of the milestone is to be aimed
    at the world.

    **Re-verified 2026-09-10: STILL OPEN, and every count it states is now
    LOW.** `docs/work/DONE.md` carries **19** numbered defect entries, not fourteen —
    18 distinct ids, since 014 was issued twice and the next is 025
    (`docs/work/DEFECTS.md:20-26`). Regression cases are **58** `fixedbugs-*.hero`
    under `tests/golden/` (17 check, 35 run, 6 unsupported) against the item's 49, and
    **28** in `tests/golden/fixedbugs/` against 27: **86** in total, not 76. **And the
    63 is UNSETTLED**: no command reproduces it. Over `selfhost/**/*.hero`,
    `impossible` is 9, `cannot happen` 4, `internal error` 27 and `unreachable`
    **119**, so the catalogue's own first act is to define what it counts before it
    counts it.

- [ ] **M-generated-programs** | the generator computes the answer while it builds the program | `tests/harness/suite_corpus.hero` § configurations · `examples/montecarlo/main.hero`

    **Origin:** scheduled with the chain row, 2026-09-06, the author taking all
    five oracles over the two narrower options offered. The net's three
    configurations are the differential arm.

    **The two moves, so the milestone does not re-derive them.** Generation goes
    from the TYPE and never from the text — start at *an expression of type
    `i64` is needed* and descend among the forms that type admits — so the
    program type-checks by construction and a refusal from `heroes check` is
    itself a defect. And every node carries its value as it is built, so the
    generator writes the program and its `main.expected` together and needs no
    second compiler as judge; that is Csmith's checksum trick, and it is what
    makes the silent class visible at all.

    **Heroes has no undefined behaviour, which changes the job**: overflow,
    division by zero and an index out of range all abort by design, so there is
    nothing to steer around the way YARPGen must for C — there is a clean arm
    whose values are safe by construction, and a smaller declared arm whose
    expected result IS the abort and its message.

    **What already exists and must be reused rather than rebuilt**: the three
    configurations are `tests/harness/suite_corpus.hero::configurations()`,
    measured 2026-09-06 as `["-O0", "-O2", "--sanitize"]`, and their
    disagreement is the only differential oracle there is until M-qbe-backend
    gives the tree a second backend; a deterministic pseudo-random stream driven
    by an integer seed is in `examples/montecarlo/main.hero`, with a test saying
    why a seed must reproduce.

    **Where to look also:** `docs/ROADMAP.md` § M-generated-programs.
    **Why it matters:** exit 0 with a wrong number is the class nobody can write
    a golden case for in advance, and an oracle the generator carries is the
    only kind that scales with the programs.

    **Re-verified 2026-09-10: STILL OPEN, and both premises hold exactly.**
    `tests/harness/suite_corpus.hero:304-305` is still
    `function configurations() -> [str]` returning `["-O0", "-O2", "--sanitize"]`, and
    `examples/montecarlo/main.hero:41` still carries the seeded stream with its two
    tests at `:85-89`. No generator exists: the net registers 20 suites and none is
    generative.

- [ ] **M-generated-programs** | the reducer starts from `mutate/sites.hero`, and only reduced witnesses enter the net | `selfhost/mutate/sites.hero` · `tests/harness/main.hero` · `CLAUDE.md` §10

    **Origin:** scheduled with the chain row, 2026-09-06, the author placing the
    generator in the harness rather than behind a new verb.

    **A generated crasher is unreadable, and an unreadable crasher is not
    repaired.** The reduction is the ordinary one — remove a declaration, a
    function, a branch, re-run, keep the removal if the symptom survives — and
    its primitives are in the tree already: `selfhost/mutate/sites.hero` was
    split out on 2026-08-19 as *the primitives every mutation operator is built
    out of: a text edit, a span, and the four questions about a tree node that
    decide whether a site is one*.

    **What must NOT happen to the net.** The long hunt stays outside it: a suite
    that generates at random goes red at random, and CLAUDE.md § Commands
    already carries what this project pays for an instrument nobody trusts. What
    enters is the committed corpus of reduced witnesses, deterministic, run like
    any other case, with the free-running hunt on tags and by hand. **And the
    tool question is settled before it is asked**: a Heroes program under
    `tests/harness/` run by `heroes run` proposes no verb, so §10's stopping
    rule is untouched — if the harness route is ever measured impossible, THAT
    is the sitting, not the convenience.

    **Why it matters:** the difference between a fuzzer that finds bugs and one
    that files noise is the reducer, and the difference between a net people
    trust and one they ignore is determinism.

    **Re-verified 2026-09-10: STILL OPEN.** `selfhost/mutate/sites.hero` is
    **149** lines and there is no reducer and no generated-corpus suite. CLAUDE.md
    §10's *"never a second binary, never a script, never a Makefile"* is unchanged,
    so the premise the item rests its shape on is intact.

- [ ] **M-generated-programs** | `--sanitize` runs at `-O2`, where clang deletes the leaks LeakSanitizer exists to find | `tests/harness/suite_corpus.hero` § configurations · `CLAUDE.md` §8

    **Origin:** measured 2026-09-07 at M-declared-freer step 5, in the Linux
    container, while building a positive control for the `owned` release. Its
    home because that milestone's whole job is aiming an instrument at the
    compiler, and this is a measured hole in one of the instruments it will lean
    on.

    `tests/harness/suite_corpus.hero`'s `configurations()` is `["-O0", "-O2",
    "--sanitize"]`, and `--sanitize` alone reaches `heroes run`, whose default
    is `-O2` (`heroes --help`). Measured on one program, 200 unfreed allocations
    through a `static inline` header function: at **`-O0`** the binary is **exit
    1** with `LeakSanitizer: detected memory leaks, 4200 byte(s) in 200
    object(s)` naming the `.hero` line; at **`-O2`** it is **exit 0 with nothing
    said**, because the allocation is visible to the optimiser and its only use
    is a null test, so LLVM removes it. **The instrument was proved live in the
    same session** — a plain C `malloc` with the pointer dropped is reported at
    both levels — so this is the optimiser and not a broken sanitizer.

    **How narrow it is, stated rather than left to be found**: a call into a
    real library cannot be elided, which is why `examples/ledger/`'s 40-byte
    leak WAS caught by CI in this exact configuration on 2026-09-04. What is
    invisible is a leak through C the optimiser can see through — a header of
    `static inline` wrappers, which panel 114 R6 made the answer for a package's
    thin C half, so the class is about to get larger rather than smaller.

    **Two further false negatives found the same day and worth carrying**,
    because both cost a wrong conclusion before they were understood: a pointer
    still held in a live local is **reachable** and not leaked, and
    LeakSanitizer scans the stack **conservatively**, so a stale pointer in a
    dead frame is reachable too — a single-allocation leak in a Heroes program
    is therefore usually invisible, because CLAUDE.md §7 hoists every local to
    the prologue.

    **Recommended: a fourth configuration, `-O0 --sanitize`, for the programs
    that declare an `extern` only** — which is CLAUDE.md §8's own narrowing, 8
    programs of 44, so the cost is small and it lands where the class actually
    lives.

    **Where to look also:** `tests/harness/suite_run.hero` ·
    `docs/environment/linux/LINUX-MACHINE.md` · `docs/panel/114` R6.
    **Why it matters:** the leg this project names as its judge for a C leak is
    the one leg whose optimisation level can delete the evidence.

    **Re-verified 2026-09-10: STILL OPEN in substance, one citation FALSE.**
    The substance holds: `configurations()` is still the same three and there is no
    fourth `-O0 --sanitize` arm, and `--sanitize` alone still means `-O2` for `run`.
    **The false citation is CLAUDE.md §8's**: `grep -n sanitize CLAUDE.md` returns
    **nothing at all**, and §8 is Error discipline. That narrowing is **CL-055**, and
    it lives in `.claude/rules/platforms.md` and `.claude/rules/c-boundary.md`, where
    it carries **no program count**; re-measured, programs that declare an `extern`
    are **20 of 55** directories, not 8 of 44. **This file was not touched on
    2026-09-10** (last commit `f0d95197`, 2026-09-06): that day's sanitizer work
    landed on a golden which was then removed, and
    `docs/measurements/025` measures the same instrument from the other side — on
    Linux, `run … --sanitize` reported a 29-byte leak where `build --sanitize` plus
    running the binary was silent.

- [ ] **M-thesis-harness** | should the spec exhibit one worked diagnostic, when today it exhibits zero | `spec/heroes-spec.md` · `design.md` Part 11 · `docs/measurements/007-two-predictions-collected.md:24-25`

    **Origin:** the llm-ergonomist, panel 118, 2026-09-08, the other half of the
    same appetite. `/decide` 2026-09-08 split the two and refused to settle this
    one by a date: it is **decided by an instrument**, which is what CLAUDE.md §2
    asks of anything entering on the thesis rather than on compiler need.

    **The claim, in the seat's own terms**: a language whose thesis is that every
    plausible mistake is a compile error shows its reader **zero** compile errors,
    so a model cannot predict what it is about to be told. That is a falsifiable
    claim about metric 2 — first-try completion under a spec-only context — and
    metric 2 has never run.

    **Why the instrument and not a date.** A worked diagnostic is dearer than the
    **40** tokens free after M-closures-verdict, so it cannot be bought on an
    appetite; Principle 0 admits it on a **measured design.md Part 11 effect**,
    and this milestone is what produces one. The sitting that buys or refuses it
    is therefore the one that can read the number, and a refusal then owes a
    Part 6 row carrying its falsifier (CLAUDE.md §12).

    **What it has to beat, in the same currency.** Panel 120 measured one
    silent-class fact at **+30** where four signature drafts cost +63 to +86 and
    bought nothing for the class the sitting was convened about. The exhibit's own
    price is owed measured rather than estimated before it is compared with
    whatever clause the same budget could buy instead.

    **Why it matters:** the one thing this spec never shows is the thing the whole
    language is for, and whether that costs a reader anything is a measurement
    nobody has taken.

    **Re-verified 2026-09-10: STILL OPEN in substance, and one sentence is now
    FALSE.** The substance holds — `grep -c 'error\[' spec/heroes-spec.md` is **0**,
    so the spec still shows a reader no diagnostic at all. What is false is the
    pricing: *"dearer than the 40 tokens free"* rests on a headroom figure
    `docs/measurements/010-spec-budget-ledger.md` **voids in its own closing
    section**, this one by name. Measured here instead, from
    `tests/harness/suite_spec.hero`: ceiling **6144**, `REAL_TOKENS` **5378**,
    `FFI_FLOOR` **60**, so **706** free — not 40. The comparison against panel 120's
    `+30` is in the same voided currency and needs the new instrument before the
    sitting reads it.

- [ ] **M-thesis-harness** | what it depends on, and the author's decision on the seed tasks | `design.md` Part 11 · `docs/measurements/007` · `docs/panel/011`

    **Origin:** author instruction 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **Metrics 2 and 4 have never run** (`docs/measurements/007:24-25`;
    M-publication-gate's checklist), and the instrument that runs them is a
    Heroes program over HTTPS, so it waits for M-core-packages' `net/http`
    client (libcurl, step 11) and reads its API key from the environment, one of
    §10's three input classes. Part 11's protocol binds it whole: spec-only
    context, single turn, hashed frozen prompts, two gradings, Wilson intervals,
    one non-Anthropic model, provenance (spec sha, compiler sha, model id,
    prompt sha, suite sha). **Author decision 2026-09-03**: a small seed of
    tasks written by the author at this milestone's opening, the bulk still at
    M-guide-book as decided 2026-08-24 (the M-guide-book item above stands).
    Corpus material never enters the held-out set (CLAUDE.md §9).

    **Where to look also:** `design.md` Part 11 (`:2823-2860`) ·
    `harness/tasks/README.md`.
    **Why it matters:** a thesis with one of two factors audited is an opinion
    with a decimal point, which §12 forbids.

    **Re-verified 2026-09-10: STILL OPEN, and it points at a contradiction the
    ROADMAP now has with itself.** `harness/tasks/README.md` still says *"Status: 0
    tasks"* and still keys the bulk to M-guide-book. Part 11 is `design.md:3114-3166`,
    not `:2823-2860`. **And the dependency's number moved**: the `net/http` client is
    M-core-packages **step 5** after the 2026-09-10 reorder, while
    `docs/ROADMAP.md` § M-thesis-harness still says *"step 11"* — which is now the
    byte buffer. Whoever opens either milestone repairs that sentence to name the
    package rather than a number, on §14's own rule.

- [ ] **M-thesis-harness** | panel 007's two predictions contradict each other and nothing has scored them | `docs/panel/007-terminator-enders.md:72-85` · `design.md:1800-1809` · `docs/panel/046` R1

    **Origin:** panel 007's two registered predictions, deferred as *"007-bis"*
    on 2026-08-03 and never scored. Its home since 2026-09-04, when the panel
    watch list was retired: both predictions are scored against a metric-2 run
    and nothing else can score them.

    The ergonomist's: tasks forcing a depth-0 expression break lose **≥25pp**
    first-try under a spec that says nothing. The spec-warden's: **zero**
    baseline completions contain a depth-0 illegal break.
    `docs/panel/007-terminator-enders.md:82-85` says in its own words that
    *"these two cannot both hold"*.

    **What has been settled meanwhile, so the sitting is not asked the wrong
    question**: the *form* is refused and now enforced — `design.md:1800-1804`,
    and `grammar_expr.hero`'s `ends_the_expression` refuses a continuation at
    bracket depth zero after a defect that had it compiling and printing at exit
    0 (panel 095, ratified). What is open is only whether the **spec owes the
    ~28-word layout sentence**, and panel 007 deferred exactly that as one
    package with Nim's explicit continuator set. **The scoring rule is panel 046
    R1's**: an outstanding prediction is *re-decided, never renewed* — scored,
    or marked `lapsed` with its clause re-argued under the removal branch.
    `design.md:1807-1809` still carries the deferral.

    **Where to look also:** `docs/measurements/007`, which scored panel 046's
    six rows and not these two.
    **Why it matters:** a prediction nobody can score is a deferral wearing a
    measurement's clothes, and this pair has worn it for a month.

    **Re-verified 2026-09-10: STILL OPEN, and unscoreable for the reason it
    says.** `docs/panel/007:82-85` still carries both predictions and its own
    *"(these two cannot both hold)"*, and `selfhost/grammar_expr.hero:160` still
    defines `ends_the_expression`. Metric 2 has still never run, so neither
    prediction can be scored. **Both design.md pointers moved**: the
    statement-position sentence is `:1853` and the deferred trailing-operator
    continuation `:1861`; `:1800-1809` is now about literal patterns and
    `&&`/`||`.

- [ ] **M-thesis-harness** | the compiles-but-wrong-output bucket the harness never named | `docs/panel/009-spec-budget-2000.md:100-103` · `design.md:2851-2853` · `selfhost/mutate/score.hero`

    **Origin:** panel 009's llm-ergonomist, as a condition on its own vote
    2026-08-04. Its home since 2026-09-04, when the panel watch list was
    retired; the condition is the seat's own and it is a condition on the
    instrument, so it belongs to the milestone that builds it.

    **The harness must report a compiles-but-wrong-output bucket, separate from
    the compile-error bucket, and today nothing names one.** The seat's words:
    without it *"the prediction is untestable and the raise unjustified on this
    axis"*, and its scoring line adds *"only if the compiles-but-wrong bucket
    exists"*. **What exists instead, measured**: Part 11 states two gradings,
    *compile* rate and *tests-pass* rate (`design.md:2851-2853`) — which yields
    the bucket by subtraction and never names it, and `git log -S` dates that
    text to 2026-08-03, the day **before** the sitting, so it is not an answer
    to the condition. `selfhost/mutate/score.hero`'s `survived` is a
    compiles-but-presumed-wrong class over mechanical mutants, which is **metric
    3 and not this**: it never compares program output. The golden runner does
    compare output, so the capability is in the tree and the reporting class is
    not.

    **Also owed and cheap**: the sibling condition from the same vote — that
    Part 11 report *class-weighted* numbers — survives in **one** place in this
    repository, the sitting itself, which is the shape §11 calls a premise that
    expires in silence.

    **Where to look also:** `docs/panel/009-spec-budget-2000.md:23` ·
    `tests/harness/suite_run.hero`.
    **Why it matters:** the thesis's central claim is that this language deletes
    SILENT errors, and the instrument that would show it has no column for them.

    **Re-verified 2026-09-10: STILL OPEN, and its own one-place count is
    confirmed.** `class-weighted` survives in exactly **one** place, `docs/panel/009`,
    and *"compiles-but-wrong"* appears nowhere in any instrument — only in that
    sitting, in `docs/panel/010:118`, in the narration and in this item.
    `selfhost/mutate/score.hero`'s `.survived` is still over mechanical mutants
    alone. One pointer moved: Part 11's two-gradings sentence is
    `design.md:3136-3138`, not `:2851-2853`.

- [ ] **M-lsp-server** | the per-module build is slower, and the frontend is where the prize was | `docs/panel/093` · `selfhost/emit/unit.hero` · `selfhost/cli/units.hero`

    **Origin:** M-separate-compilation step 6, measured 2026-08-26. Its home
    since 2026-09-03, `DESIGN-LOG.md:539`: a server that re-checks on every save
    cannot wait for `heroes check` on the compiler's own source, so the
    incremental frontend is the tool's precondition rather than a build
    optimisation. This item was M-separate-compilation step 6, open, and it was
    the step's remaining question.

    **The per-module build is SLOWER than the fused one, and the reason is that
    the frontend is 83% of a build.** Measured with one binary on the compiler's
    own source, after the step's four repairs: fused `--emit-c` + clang + link
    **≈123 s** (frontend 88 s + fused emission 30 s + clang **4.3 s** on 788,406
    lines + link); per-module cold **261 s**, per-module fully warm — nothing
    edited — **250 s**. The cache works and is not the problem: the TU objects
    are reused (`ls -lT` shows the cold run's mtimes, 162 cache directories
    after two identical builds rather than 314), and what it saves is the
    **4.3 s** of clang. What it costs is 157 per-TU emissions where the fused
    path prints once. **So there is no invocation where per-module wins today**,
    which collides with the author's rule that a change must not slow the
    compiler down — and the collision is architectural rather than a defect:
    panel 093 R1 keeps the frontend whole-program, so a warm rebuild re-parses
    and re-checks all 158 modules whatever the cache holds. Two directions, and
    the decision is the author's: make the per-TU emission cheaper (it prints
    471k lines against the fused 788k, at 2.5x the cost per line — the per-TU
    setup), or make the FRONTEND incremental, which is the only one that changes
    the ratio.

    **RE-MEASURED 2026-08-26 at step 7 close, and the pair above is NOT
    REPRODUCIBLE on today's tree.** Four timings inside fifteen minutes, one
    machine, one source tree, with the pre-step-7 compiler rebuilt from the
    committed seed as the control: per-module **cold 142.29 s** before /
    **145.53 s** after, per-module **warm 133.08 s** before / **132.68 s**
    after. The fused path measured the same day is **≈124 s** (`--emit-c`
    120.45 s + one clang line over the 792,357-line seed 3.65 s). So the 261 s /
    250 s above is 1.9x today's number for the same command and the same binary
    lineage, and **no cause is asserted because none was measured** — the old
    pair keeps its date (CLAUDE.md §14) and this one is beside it, because the
    decision this item asks for was about to be taken on numbers almost twice
    too large. On the new numbers: per-module warm 132.7 s against fused 124 s,
    so per-module still does not win — but the gap is **8.7 s, not 127 s**,
    which is a different question. Step 7's own contribution is measured and
    small: +3.2 s cold for the header freshness check, nothing at all warm, and
    the `runtime_text` hoist it also landed (157 shell `cat` calls and 25 MB of
    hashing removed per build) does **not** show in wall clock at all.

    **ANSWERED 2026-08-26 by the author: make the FRONTEND incremental** — and
    then the premise the answer rested on stopped being true the same day, which
    is why the item is rewritten rather than actioned. Step 9 took `heroes
    check` on the compiler's own source from **88.0 s to about 8** by removing
    four instances of one quadratic; no architecture changed. Re-measured after
    it, one machine, one tree, new compiler: per-module **cold 57.93 s**,
    **warm 45.21 / 46.23 s**; fused **emit 40.41 s + one clang line 3.71 s =
    44.1 s**. **So the frontend is now ~18% of a build where this item measured
    83%, and the per-module/fused gap is ~1 s where it was 8.7** — the prize an
    incremental frontend could win is at most the 8 s the frontend now costs,
    and the 83% figure that made it the obvious direction is gone. **What
    dominates instead is the per-TU EMISSION, and that is architectural rather
    than a defect**: panel 093 R4 puts the emitted text IN the cache key, so a
    warm build must emit all 157 TUs to learn that it may reuse their objects.
    The cache can never skip the work that computes its own key. That is the
    shape the next sitting on this should argue about, with these numbers; an
    incremental frontend is now the smaller half. The item stays here rather
    than moving to `DECIDE.md` because what it asks is which milestone does it,
    not what should be true — answered 2026-09-03: M-lsp-server, for the
    frontend half an editor feels; the emission half, architectural by panel 093
    R4, stays measured here as the trigger.

    **Why it matters:** separate compilation was priced in LINES by the sitting
    and never in seconds, and the seconds say the payoff is behind a
    whole-program frontend.

    **Re-verified 2026-09-10: STILL OPEN.** No incremental frontend exists;
    `grep -rln incremental selfhost/` hits a comment about `starts_of`
    (`selfhost/source.hero:116`) and MSVC's incremental **linker**
    (`selfhost/cli/flags.hero:126-130`). **Its numbers were deliberately not
    re-run**: every one is a wall-clock build, and CL-025 forbids a clock while
    anything else moves, so they stand as the three dated triples the item already
    carries.

- [ ] **M-vscode-extension** | the TextMate grammar is behind the language, and nothing judges either highlighter | `editors/vscode/syntaxes/heroes.tmLanguage.json` · `site/src/lib/highlight.ts` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form

    **Origin:** author instruction 2026-09-10 — a step will be needed at some
    point to improve the syntax colouring, in VS Code and in editors generally
    but on the site too, because introducing new things means the colouring has
    to know about them. That is CL-036's walk asked about a tool the walk did not
    name. The rule was widened the same day; this is the repair it now demands.

    **Three gaps, measured, and the asymmetry is the finding.**
    `site/src/lib/highlight.ts` reads a string with holes as
    `selfhost/lex_interp.hero` reads it and cites that module by name.
    `editors/vscode/syntaxes/heroes.tmLanguage.json` has **no `f"…"` rule**, so a
    hole is painted as ordinary string text; admits **four escapes** where
    `spec:76` gives five in a string, so a legal `\r` is painted
    `invalid.illegal` and a correct program shows as an error; and knows **none**
    of `owned`, `tag`, `partial`, `link`, `package`, `as`. One of the two kept up
    because somebody remembered.

    **The check is the durable half**, and its seam exists: `suite_spec.hero:49`
    already reads `spec/reserved-words.md`, so a row that compares each
    highlighter's word list against `selfhost/keywords.hero`'s **21** keywords and
    `selfhost/inventory.hero`'s **39** built-ins has somewhere to live. **Nothing
    judged either file before** — `grep tmLanguage tests/harness/` was empty, and
    `suite_records.hero` reads `editors/vscode/icons` for the SVG-path rule alone —
    which is how the grammar rotted in silence.

    **Not filed as a defect, deliberately** (author decision the same day): the
    list would go from 0 to 1 and block the next tag, and the repair touches no
    compiler line and no spec token, so it can ride any commit.

- [ ] **M-qbe-backend** | every local is hoisted, so a recursive frame is sized by the whole body | `CLAUDE.md` §7 · `examples/interpreter/syn/expr.hero` · `selfhost/emit/`

    **Origin:** measured 2026-09-03 at M-corpus-depth step 5, while measuring
    defect 007 in `docs/work/DONE.md`. Its home since 2026-09-04: it had named
    no milestone, only *the next panel that touches the emitter or the IR*. That
    milestone's own warrant is the one this question needs — a second backend is
    what turns *the IR is target-agnostic* from an assertion into a measurement,
    and a slot's lifetime is precisely where the C emitter's convention stops
    being the only possible answer. Architecture, so it still rides a sitting
    rather than convening one (CLAUDE.md §4), and an earlier sitting that
    touches emission may take it.

    **That is what sets the recursion ceiling of every program in this
    language.** Measured with `heroes build --emit-c` over
    `examples/interpreter/`: **247 hoisted locals in the prologue of
    `syn/expr.hero`'s `compared`**, 151 in `primary`, several of them whole
    `Token` and `Expr` variants sized by their largest case, plus two `@`
    parameters copied in by value at every level (§4.8). Under `--sanitize` that
    chain measured **11,728 bytes between `bp` and `sp` for one frame** (ASan
    pads stack variables, so read it as an upper bound). The consequence is a
    ceiling a reader would not expect: this program's nesting limit is **120
    under `--sanitize`, 190 at `-O0`, 340 at `-O2`**, and the compiler's own
    parser gives up at **440**.

    CLAUDE.md §7 states the hoist as a rule with a reason (a `void t0;` is a
    hard error, one `goto`+label per block, all locals in the prologue) and the
    question this item asks is narrower than repealing it: **a temporary live
    inside one basic block does not need a slot for the whole function.** What a
    sitting has to price: whether scoping temporaries per block breaks the
    `#line` discipline or the double-emit determinism test, what it does to the
    seed's line count, and whether clang's own optimiser already does it at
    `-O2` — which the 190-against-340 pair suggests and does not prove. **Not
    urgent and not a defect**: the programs in this repository work. It is filed
    because the number is measured and the next sitting that touches emission
    should argue from it rather than discover it. **And half of the measurement
    that sitting needs is scheduled before it** (added 2026-09-07):
    M-typed-inspection's second step, scheduled 2026-09-06, is a census of
    exactly these hoisted locals — how many are `t<N>` temporaries, how many
    `$`-synthetic slots the lowering invented, how many bindings the author
    wrote — so the sitting reads that census before pricing per-block scoping
    rather than counting again.

    **Where to look also:** defect 007 in `docs/work/DONE.md` ·
    `docs/ROADMAP.md` § M-typed-inspection.
    **Why it matters:** a recursion ceiling nobody chose is a limit set by an
    implementation detail.

    **Re-verified 2026-09-10: STILL OPEN in substance, STALE PREMISE on its
    headline number, and that is the finding.** The **247 and 151** hoisted locals do
    not reproduce. Read off the cached emitted C for that program
    (`build/336f076635d558f1/main.c`, stamped `heroes 0.2.0`),
    `h_synexpr_compared`'s prologue is **119** declarations and
    `h_synexpr_primary`'s **122**; two older cached artifacts agree at about 122. So
    neither the number nor the 247/151 **asymmetry** survives. **UNSETTLED which of
    three things happened** — the source changed, the emitter improved, or the
    original count used a different rule — and settling it needs
    `heroes build --emit-c`, which was deliberately not run here (CL-025, and it
    writes into `build/`). The sitting re-measures it as its first act rather than
    inheriting either number. One pointer moved: the hoist rule left CLAUDE.md §7,
    which is a one-line pointer now, for `.claude/rules/generated-c.md:33-35`.

- [ ] **M-qbe-backend** | the QBE the sitting will meet is not the one panel 001 rejected | `design.md` §3.2 · `DESIGN-LOG.md:6`, `:8` · `selfhost/emit/extern_probe.hero`

    **Origin:** measured 2026-09-06 from this Mac, after the author asked
    whether the backend should move to QBE at the end of the chain; `qbe` is not
    installed here, so what c9x.me says is READ and not run. Its home is that
    milestone because every fact below is about the tool it builds on, and the
    sitting that opens it is the reader; nothing here convenes anything.

    **Two of design.md §3.2's sentences about it have expired.**
    `https://c9x.me/compile/releases.html`, read 2026-09-06: release **1.3
    (2026-06-01)** says *"Windows ABI support"*, and release **1.2
    (2024-02-16)** added *"new experimental `dbgfile` and `dbgloc`
    directives"*, which is line-level debug information. `design.md:702-704`
    says QBE has *"no DWARF"*; the site's front page still lists amd64 (linux
    and osx), arm64 and riscv64 with no Windows target, so the two pages
    disagree about Windows; and the IL document (`doc/il.html`) does not describe
    the two directives yet. **So the sitting's first step is to install QBE 1.3
    and RUN it** — `arm64_apple` on this Mac, the Windows target on the box
    (`docs/environment/windows/WINDOWS-MACHINE.md`) — before a line of emitter
    is written or a sentence of §3.2 is amended: a platform fact that has not
    been run on that platform is an inference (CLAUDE.md §1).

    **What did not expire is the reason the answer to the author's question is
    *a second backend, never a replacement*.** The IL has no way to include a C
    header or to check a declared signature against one (read the same day), and
    the verification of every `extern` in this compiler is clang reading the
    header. Counted in `selfhost/emit/` on 2026-09-07: **34** `_Static_assert`,
    **35** `_Generic`, **10** `__builtin_classify_type`, **7** `__typeof__`,
    **4** `__builtin_types_compatible_p`, **48** `#line`, **5**
    `__builtin_*_overflow`. **20 of 54** program directories under `examples/`
    (those holding a `main.hero` or `whole.hero`) declare an `extern`, and each
    would lose the header check. The runtime (**751** lines in `runtime.c` and
    the header, **4911** in `parts/`) and the seed (**747,095**) are C and still
    want a C compiler, and QBE itself emits assembly for `cc out.s` — so QBE
    adds a dependency and removes none. The `--sanitize` configuration in
    `tests/harness/suite_corpus.hero::configurations()` is clang's; QBE has no
    sanitizer. **And clang's DWARF has two consumers now, not one**:
    M-typed-inspection, scheduled 2026-09-06, is built on `-g`, on `#line` and
    on lldb reading the C types, so a second backend must say what a stopped
    program shows under it, or say plainly that it shows nothing. **Where the
    milestone sits is unchanged and the author's**: after the tools and before
    the books, for the 2026-09-03 reason that the IR should have stopped moving
    first (`docs/ROADMAP.md` § Who scheduled what).

    **Where to look also:** `design.md` §3.2 (`:698-707`) · `docs/ROADMAP.md`
    § M-qbe-backend, § M-typed-inspection ·
    `tests/harness/suite_corpus.hero::configurations` ·
    `https://c9x.me/compile/releases.html`.
    **Why it matters:** a sitting that argues from a 2026-08-03 description of a
    tool that has shipped two releases since is a sitting convened on a premise
    that expired in silence.

    **Re-verified 2026-09-10: STILL OPEN, six of the seven emitter counts
    exact.** In `selfhost/emit/`: `_Static_assert` **34**, `_Generic` **35**,
    `__builtin_classify_type` **10**, `__typeof__` **7**,
    `__builtin_types_compatible_p` **4**, `#line` **48** — all as written — and
    `__builtin_*_overflow` is **6**, not 5. Both unamended design.md sentences are
    still there (`:723` *"no DWARF"*, `:721` *"no headers"*). **Four numbers moved**:
    §3.2's QBE bullet is `design.md:717-726` not `:698-707`; `runtime.c` plus its
    header is **776** lines not 751; `runtime/parts/*.c` is **5013** not 4911; and
    `seed/heroes.c` is **773,509** not 747,095. Externs are **20 of 55** program
    directories, the numerator unchanged. **And one stale citation found beside it**:
    `design.md:718` calls the QBE backend *Part 7 item 14*, where item 14 is
    declaration visibility and QBE is item **15** (`design.md:2807`) — the same slip
    the ROADMAP's own cells carried until 2026-09-03.

- [ ] **M-guide-book** | metric 2's held-out tasks, written by the author | `harness/tasks/README.md` · `docs/panel/011`

    **Origin:** author decision 2026-08-24, `/decide` answer `8b`. This item
    replaces both the 2026-08-11 original and the 2026-08-18 promotion to
    `DECIDE.md`, which had made three copies of one question.

    They must be author-written or they measure the assistant's priors rather
    than the thesis, and `n` scales with the paced protocol rather than 15 at
    once. Measured 2026-08-24: `harness/tasks/` holds **only `README.md`** —
    zero of the 15 — so nothing has been quietly accumulating, and metric 3 is
    unaffected (it has run on every commit since panel 011). **Why this
    milestone owns it**: M-guide-book's own work *is* the author writing
    programs in Heroes, so the held-out set comes out of work already happening
    instead of competing with the compiler for the same hours. Two deferrals of
    this item have already made their stretches unrecoverable, which is why the
    home is named rather than left as "later".

    **Where to look also:** `docs/debrief/DECIDE.md` (2026-08-18, the closed
    promotion; that directory is retired).
    **Why it matters:** metric 2 validity, and the thesis keeps two measured
    halves instead of one.

    **Re-verified 2026-09-10: STILL OPEN, nothing accumulated.**
    `harness/tasks/` holds one file, its own README, which says *"Status: 0 tasks"*.
    One clarification for whoever opens it: that README's target is **20** frozen
    tasks of which **15** must be author-written, so this item's 15 is the held-out
    subset and not the whole set.

- [ ] **M-guide-book** | a long signature has no continuation, and a printed page cannot hold one | `selfhost/print/fmt.hero:77`, `:524-550` · `selfhost/parse/members.hero:60-104` · `docs/panel/110`

    **Origin:** `docs/panel/110`'s historian, 2026-09-04, as the one
    recommendation the sitting docketed rather than refused. Scheduled at *row
    53* until 2026-09-05, 54 after the insert, and dropped for §14's reason: a
    reorder moves a number and never a name. This is the milestone whose pages
    are 72 to 80 columns wide.

    The form is legal today — all three multi-line shapes `check` and `run` at
    exit 0 — and `heroes fmt` **joins them onto one line** with no width test,
    deliberately, because panel 007 *"leaves nowhere else to break it, and a
    formatter that invents a continuation invents one the language does not
    have"*. Measured 2026-09-04: **167 of 1444 (11.6%)** signatures in
    `selfhost/` exceed the formatter's 120 columns, against **3 of 850 (0.4%)**
    in `examples/`, and the longest is **235 columns**.

    **Why this milestone and not sooner**: Principle 0 refuses it now, because
    nothing is blocked and ordinary programs barely have the shape — but a book
    page is 72 to 80 columns, so either the book invents a continuation the
    language does not have, which is §9's *formatter lies quietest* failure one
    level out, or those signatures are renamed shorter, or the language gains
    the break. M-doc-generator and M-lsp-server hit the same wall earlier, in
    hover text and generated docs.

    **The precedented shape, and it is convergent**: the author's **trailing
    comma steers the layout** — `zig fmt` puts a parameter list on one line
    unless a trailing comma is present, in which case one per line, and Black's
    *magic trailing comma* (20.8b0) does the same for brackets; two independent
    formatters reached it to avoid a width-driven algorithm guessing. It
    preserves design.md §4.15 because the trailing comma **is** the textual
    difference, it is canonical, and the round trip is stable — and Heroes
    already requires the comma, so the token is not new. `spec:174`'s
    newline-separated multi-line **literals** are the same instinct applied one
    construct over. **The seat's prediction to score at this milestone**: the
    11.6% figure **rises** rather than falls, because every new generic
    parameter and every `@` parameter lengthens a signature and nothing shortens
    one.

    **Where to look also:** `spec:174` · `docs/panel/095`.
    **Why it matters:** the formatter is the one tool whose silence makes every
    diff untrustworthy (§9), and the book is where a shape it cannot print stops
    being invisible.

    **Re-verified 2026-09-10: STILL OPEN, both percentages re-measured, and the
    seat's prediction is AMBIGUOUS.** `selfhost/` is now **180 of 1560 signatures
    over 120 columns (11.5%)** against the item's 167 of 1444 (11.6%), `examples/` is
    **3 of 932 (0.32%)** against 3 of 850, and the longest is still **235**.
    `selfhost/print/fmt.hero:77-78` is still `WIDTH` `120` and `:524-555` still joins
    a signature onto one line with no width test. **The registered prediction says
    the 11.6% figure RISES**: the absolute count rose 167 to 180 and the rate is flat
    to slightly down, 11.6% to 11.54%, so which of the two the seat meant decides
    whether it is met. **Resolve that before the sitting reads it as scored**, which
    is a question and not a task.

- [ ] **M-guide-book** | the held-out tasks must never be committed, now that the repository is public | `harness/tasks/README.md` · `design.md` Part 11

    **Origin:** M-open-repository, 2026-09-08, and nobody had written it down.

    Metric 2 is held out: 15 of the 20 tasks must be author-written, and the
    whole point is that the model reading them has not seen them. **A public
    repository makes committing them the act that burns them** — not only for a
    scraped training set, but for anybody who reads the file before taking the
    test. Nothing is burned today, because `harness/tasks/` holds **0 tasks**
    and the five assistant drafts were pruned on 2026-08-03.

    Owed at the moment the first task is written: a home outside this
    repository, and a line in `harness/tasks/README.md` naming it. What can be
    committed is the grading and the provenance, which say nothing about the
    task's content.

    **Why it matters:** a measurement whose instrument is public measures
    something else.

    **Re-verified 2026-09-10: STILL OPEN, and its factual premise is true
    today.** `harness/tasks/` holds one file and **zero** tasks, so nothing is burned
    yet, which is exactly the window the item exists to use. What is owed is still
    absent: `grep -in "outside this repository" harness/tasks/README.md` finds
    nothing, so no home outside this repository is named anywhere.

- [ ] **M-deployable-binary** | what a built program needs at run time on each platform, and which `-O` it ships with | `selfhost/cli/table.hero` · `selfhost/cli/verbs.hero:105` · `design.md:708-710` · `docs/environment/`

    **Origin:** author decision 2026-09-10, § What production-ready means row 4,
    which had no owner at all.

    **One half is measured and it is good**: on this Mac, 2026-09-10, `otool -L`
    on the self-hosted compiler names `/usr/lib/libSystem.B.dylib` and nothing
    else, so a Heroes binary carries no sidecar. **The other half is unrun and is
    written as unrun**: the same question on Linux and on Windows.

    **Three measurements and one decision.** `otool`/`ldd` on a built binary on
    each platform, written into `docs/environment/`. Whether a binary built against
    the CI image's glibc runs on an older server, and what it says when it does
    not. Whether a Linux binary can be static, which is the difference between a
    `scratch` image and a distro image — knowing `design.md:708-710` already
    records that macOS has no static libc, so one flag cannot answer for three
    platforms. And the decision: **`heroes build` defaults to `-O0` while `heroes
    run` defaults to `-O2`**, so the artifact somebody ships is the slow one unless
    they know to ask. A changed default, a `--release` alias, or one sentence — and
    §10's stopping rule judges a flag here like any other. What it may not be is
    silence.

- [ ] **M-microcontroller-verdict** | the ruling on a program running on a microcontroller under an RTOS, RISC-V first, and the two runtime facts a 32-bit build refuses today | `runtime/heroes_runtime.h:122` · `runtime/parts/stack.c` · `DESIGN-LOG.md:539` · `docs/measurements/026-the-two-facts-a-32-bit-target-refuses.md` · `.claude/rules/platforms.md`

    **Origin:** author question 2026-09-10, whether supporting a microcontroller
    such as the ESP32, or a program under an RTOS, would be worth a step, since
    the language compiles without a garbage collector. The row, its shape (a
    verdict, not a target), its position and the chip family were the
    assistant's recommendations, and the author accepted all of them the same
    day. No board exists yet; the author is ordering one.

    **What the record said, read before anything was proposed**: nothing about a
    device, measured (zero hits for microcontroller, RTOS, bare metal,
    freestanding, newlib or ILP32 in design.md, `spec/`, `docs/` and the site in
    that sense); Part 2's *not a systems language*; `DESIGN-LOG.md:539`'s
    *cross-compilation considered and not entered, the three platforms are
    measured on real machines by rule*, restated at M-arm-platform as *never a
    `--target` flag*; panel 049's veto of a platform axis, and panel 114's R1 (no
    conditional compilation) and R2 (a header the program ships beside itself);
    `spec:228`, which at 32 bits makes `size_t` a `u32`.

    **What was measured on this Mac, 2026-09-10**, in the file named above: the
    runtime asks the C library for **73** functions, fifteen of them `pthread`,
    eight for processes, seven for signals and the stack guard; under
    `--target=riscv32-unknown-elf` and `arm-none-eabi` it refuses at
    `heroes_runtime.h:122`, a 64-bit atomic that must be lock-free because a
    string literal's block is read-only, and asks for `locale.h` and `math.h`,
    which a freestanding probe has no copy of; the smallest gallery program emits
    189 lines with `main(argc, argv)` and three includes; `stack.c` delivers
    `spec:167` through two branches, POSIX and Windows, and neither exists on
    FreeRTOS; a hello at `-O2` is 89,160 bytes stripped here and links libSystem
    alone. No Espressif toolchain is installed, Homebrew's QEMU knows no `esp32`
    machine, and Apple clang has no RISC-V backend, so everything about newlib,
    GCC and the device itself is **unrun**.

    **Decided ahead** (author, 2026-09-10, on recommendation): RISC-V chips first
    (ESP32-C3, C6), because upstream clang and GCC carry the target and Apple
    clang already type-checks for it, while Xtensa lives only in Espressif's
    fork; ESP-IDF over FreeRTOS rather than bare metal, because Part 2 is met to
    the letter when the registers are Espressif's C, and every `str`, `[T]` and
    `{K: V}` needs `malloc`.

    **The first step is a measuring session and not the sitting**: install
    ESP-IDF, run the `--emit-c` output and the runtime through
    `riscv32-esp-elf-gcc -fsyntax-only`, and check the 73 names against newlib,
    so that the sitting is handed answers. The seven questions it is handed are
    in the ROADMAP's own section for this row.

    **What it may not become by this row alone**: a `--target` flag
    (`DESIGN-LOG.md:539`), a standard library for the device (§1.11), a form in
    the language (panel 114 R1).

- [ ] **M-install-channels** | the channels, now that a release tag exists for them to pin | `site/src/html/index.html:135-137` · `seed/README.md` · `docs/environment/`

    **Origin:** measured 2026-09-03, re-measured 2026-09-07; scheduled
    `DESIGN-LOG.md:539`.

    **`heroes --version` printed `heroes 0.0.1` and every tag was a milestone's
    name** on 2026-09-03 (`git tag --list --sort=creatordate`: `m0` …
    `m-robustness-guards`; CLAUDE.md §14 makes the id the tag), so nothing a
    formula could pin existed. **The version scheme was decided on 2026-09-07,
    ahead of this row** (CLAUDE.md §14 § Release tags; the `DESIGN-LOG.md` row of
    that date): `vX.Y.Z` tags, `v0.1.0` first, so the thing a formula pins is
    the release tag's source archive and its checksum, and the version question
    is no longer owed here.

    Installing today is `git clone` plus `clang -I runtime seed/heroes.c
    runtime/runtime.c -o heroes` (`site/src/html/index.html:135-137`), which is
    why every channel — a Homebrew tap, a winget or scoop manifest, a Nix flake,
    a Docker image from the `Dockerfile` in `docs/environment/linux/` — builds
    from the seed and none ships a binary: `heroes` without clang compiles
    nothing. Owed here: each channel installed and `heroes doctor` run on its
    platform, all in private (a local tap, `brew install
    --build-from-source`), each pinned to a `v*` tag; what `1.0.0` promises is
    still M-publication-gate's compatibility paragraph, written together with
    that number; the outward act is the gate's.

    **Where to look also:** `CLAUDE.md` §10, §14.
    **Why it matters:** a language nobody can install with one line is a
    language whose one clang line nobody will type.

    **Re-verified 2026-09-10: STILL OPEN, and its enabling fact has advanced.**
    No channel exists anywhere: no formula, no flake, no manifest, and the only
    `Dockerfile` in the tree is `docs/environment/linux/`'s development box, which is
    not a shipped image. What HAS advanced is what a formula can pin — `v0.1.0` and
    **`v0.2.0`** are tagged, and emitted C now stamps `Generated by heroes 0.2.0`
    (`tests/emission/run-ffi-constant.c:1`), so the item's *"`heroes 0.0.1`"*
    sentence is history rather than a premise. One pointer moved: the install line is
    `site/src/html/index.html:125-127`, not `:135-137`.

- [ ] **M-compatibility-promise** | the paragraph, the suite that makes a broken promise red, and the sentence §10 forces | `README.md:117` · `.claude/rules/records.md` § Release tags · `docs/ROADMAP.md` § M-publication-gate

    **Origin:** author decision 2026-09-10, § What production-ready means row 1.
    M-publication-gate owns the paragraph as one bullet of its checklist; this row
    delivers the paragraph **and** the instrument, and discharges that bullet.

    **The suite, and both halves already exist.** The corpus at a release tag,
    recompiled by today's compiler: a program in `examples/` at `v0.2.0` that stops
    compiling on `main` is a broken promise rather than a discovery. And the
    trigger is decided — `git diff vA vB -- spec/heroes-spec.md` moves `Y` and its
    emptiness moves `Z` — so no new verb and no new vocabulary is owed.

    **The hardest sentence, and it is a consequence rather than a choice**: a
    program cannot declare the language version it was written for, because §10
    admits no fourth input class and panel 056 refused a per-project file. So the
    promise is **one-directional**: the compiler must not break old programs, and
    an old program cannot ask for an old compiler. That belongs in the paragraph,
    not in a footnote.

    **And the paragraph says the deployment model out loud**: crash and be
    restarted. Part 6 refuses exceptions permanently, and `.must()` on an error, an
    out-of-range index, an overflow at any width and a division by zero all abort
    with nothing catching them (`spec:166-169`). That is defensible as a model and
    indefensible as a surprise.

- [ ] **M-publication-gate** | no suite builds a gallery file, and ten of the fourteen are named by no test | `examples/gallery/` · `tests/harness/suite_corpus.hero:44` · `examples/README.md`

    **Origin:** measured 2026-09-04 at the close of M-corpus-depth, which is
    also when it got this home: that milestone closed with this half unbuilt
    while the rest of the item went to the record, and `examples/README.md` was
    still filing it at the closed milestone. The gate is where it belongs
    because these are the files the site shows — measured 2026-09-04,
    `site/src/html` carries **160** `data-src` references to 16 files and
    **148** of them point into `gallery/`.

    `examples/gallery/` holds **12** `.hero` files and no `main.hero`, so
    `tests/harness/suite_corpus.hero` cannot reach the directory — its floor is
    deliberately one under the directory count for exactly that reason
    (`suite_corpus.hero:44`). Measured over `tests/` the same day:
    `gallery/00-first.hero` is named **31** times, `09-holes.hero` **5**,
    `01-points.hero` **once**, and the other **nine are named nowhere**. What
    does hold is worth keeping straight rather than overstating: every file
    checks clean, because `heroes mutate examples/gallery` is a `suite_surface`
    row and `mutate` refuses a corpus that does not compile, and every file is
    canonical byte for byte. **What nothing asserts is that a gallery file
    BUILDS** in the three configurations — `00-first.hero` by surface rows and
    `09-holes.hero` to exit 1 on purpose are the two exceptions, and the other
    ten are checked, formatted and never lowered.

    **Two shapes.** (a) Give the gallery a `main.hero` so the corpus suite
    reaches it. (b) Teach the corpus suite a directory with no `main.hero`,
    whose files are built one at a time. **Recommended: (b)** — these files are
    deliberately independent one-form programs, and a `main.hero` would be a
    program written for the harness rather than for a reader, which is the one
    thing this directory is not for.

    **Where to look also:** `examples/README.md` § What holds the gallery.
    **Why it matters:** the ten files a stranger reads first are the ten nothing
    compiles.

    **Re-verified 2026-09-10: STILL OPEN, every count STALE, and one half
    repaired.** `examples/gallery/` holds **14** `.hero` files, not 12
    (`12-interpolation.hero` and `13-lease.hero` joined), and **ten of the fourteen
    are named by no test**, not nine of twelve: `00-first` is named 31 times,
    `01-points` once, `09-holes` four times and `13-lease` five, all five of those in
    `suite_records.hero`, which is a document check and not a build. The site's
    slices are **166** `data-src` references to **18** files, **154** of them into
    `gallery/`, where the item says 160 / 16 / 148. **Already repaired**: the
    misfiling its origin note complains about is gone — `examples/README.md:150-153`
    files this at M-publication-gate and says so — though that README still carries
    the old counts, so it is the same sweep's second half.

- [ ] **M-publication-gate** | `main` still cannot fail, and the measurement says exactly how far `exit(code)` got | `docs/panel/030-the-build-order-revised.md:225` · `spec/heroes-spec.md:190-194` · `examples/`

    **Origin:** M-open-repository, 2026-09-08. The gate's own checklist names
    this as *"one defect that shows up on the second page of any tour"*, queued
    from panel 030, and says it must not be true on the day the examples go up.

    **Measured this session, on a rebuilt compiler**, three shapes rather than
    the one the gate remembered. A program that receives a `fail`, matches it
    and prints it exits **0** — panel 030's sentence, still exactly true. A
    program that calls the built-in `exit(1)` exits **1**, so the escape hatch
    the spec gained is real and works. An out-of-bounds index exits **134**, so
    the guards are unaffected. **11 of the 54 example programs call `exit(`**
    and none of the gallery's 12 do, which is the corpus half of the same
    question: the ones that can fail mostly remember, and nothing makes them.

    So the finding is narrower than *"main cannot fail"* and worse than
    *"solved"*: **the language has a way to report failure and no way to oblige
    it**, and the default for a program that handles its own error is to tell
    the shell it succeeded. Changing what `main` returns is a language change
    and owes a panel; that is why this is filed and not fixed here.

    **Where to look also:** `docs/ROADMAP.md` § M-publication-gate.
    **Why it matters:** a script that calls a Heroes program cannot tell whether
    it worked, which is the one thing an exit code is for.

    **Re-verified 2026-09-10: STILL OPEN, the finding intact, one exit code
    UNPINNED.** `main` still may not declare a result — `selfhost/check/decls.hero:79-81`
    raises `main_returns`, tested at `:218-220` — so *the language has a way to report
    failure and no way to oblige it* stands. `exit(code)` is asserted end to end:
    `tests/golden/run/exit-status.expected` demands `!exit: 3` and
    `tests/harness/expectation.hero:55-56`, `:84-88` make that a demand on the shell
    status. **The abort's 134 is pinned nowhere**: the abort goldens demand only the
    message and a non-zero code (`expectation.hero:89-96`), so *"an out-of-range index
    is 134"* is **UNSETTLED** as a number and settled as non-zero-with-a-message.
    Counts: **11 of 54** program directories call `exit(`, unchanged; the gallery is
    **14** files, not twelve, and still none of them calls it. One pointer moved:
    `exit(code: i64)` is `spec/heroes-spec.md:203`, not `spec:190-194`. **And nothing
    was decided elsewhere**: `docs/work/DECIDE.md` is empty and no sitting on what
    `main` returns has ever been queued.

- [ ] **M-publication-gate** | the trademark question, in the narrow form that applies: the Aladdin Sane bolt | `site/README.md` § Style guide · `docs/assets/`

    **Origin:** the gate's own checklist, restated at M-open-repository
    2026-09-08 because opening the repository did not touch it.

    The name is a common word and worries nobody. The bolt is iconography
    attached to an actively managed estate, it is in the site's banner and in
    this repository's own `docs/assets/`, and the style guide already keeps
    lyrics out, which is the other half of the same care. **Unchanged by the
    repository opening**: the site has carried it publicly since 2026-09-03, so
    the exposure is the same today as yesterday and this stays where it was.

    **Why it matters:** the gate's own words are that it is cheaper to answer
    before publication than after, and publication is now closer.

    **Re-verified 2026-09-10: STILL OPEN and unchanged.** `site/README.md`
    § *One thing the publication gate owns* restates the question and answers
    nothing, and the lyrics rule beside it is unchanged: titles as nods, never
    lyrics. The item's claim that the bolt is in this repository's own assets is
    true — `docs/assets/banner-light.svg` and `banner-dark.svg`, described in
    `docs/assets/README.md`.

*******************************************************************************
