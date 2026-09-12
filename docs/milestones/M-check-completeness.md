# M-check-completeness — what `heroes check` accepts, `heroes build` compiles


**Scheduled, no warrant**, and the row says so on purpose. Principle 0 holds this
one: panel 082 R3 ruled the direction in 2026-08-16 and three seats measured that
nothing on the closure list needs it. What put a row under it on 2026-09-04 is
that its work had been parked on a `grep` for nineteen days with no milestone a
reader could open.

**The promise that is false today.** `heroes check` accepts `first([P(x: 1)])` at
exit 0 and `heroes build` refuses it, and the same gap has three more faces, all
measured: a map built inside `function tally<K>(k: K)` and keyed at `f64` by the
call is `check` 0 and **runs**, where `m: {f64: i64}` written down is exit 1;
`spec:221` promises a compile error for comparing a `partial` group record *"for
it and for any value holding it"*, and through `function same<A>(x: A, y: A)` it
is `check` 0, `build` 0, **run 134**; and a module that is nothing but an
`extern` group is `heroes check` **exit 0 with zero output**, because `check`
never runs clang — that fourth one is M-package-manager's, where a distributable
binding makes it load-bearing.

**Why none of them can be fixed in the body.** Panel 084 R1's shape worked
because `sort`'s domain is restricted, so refusing it on `[A]` deleted nothing. A
map keyed on `K` is legal at `str` and every integer, and `==` is legal on almost
everything, so a refusal inside the generic's body would delete working programs.
The concrete type arrives at **the call**, and that is the only line the author
can edit — which is also why the IR route provably cannot carry the diagnostic:
monomorphisation copies the template's span and discards the call's, while the
checker already keys instantiations **by the call-site span**.

**Refused, with its measurement** (082 R3): running `mono` inside `check` — 57
lines, the file past §11's ceiling, **1.3–2.2× on every keystroke**, a
`--permissive` flag contaminating Part 11's control arm, and a §4.16 hole turning
every `???` file into exit 1. Running clang inside `check` for the fourth face is
the same trade at a larger price.

**The trigger stays a `grep`, not a date**: `grep -rnE '^function [a-z_]+<'
selfhost/` returned zero generics whose body calls `sort(` when it was last
measured, and the day it returns one this is §1.0 compiler-need at any price.
**What the milestone does not lift** is the llm-ergonomist's veto: the generic
body's line stays undecidable from the line plus its signature, and lifting that
needs constraints on generics, which is the author's trade.

*******************************************************************************
**OPEN: 4**

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

- [ ] **M-check-completeness** | three rules a blind reader guessed at, and one of the guesses compiles: `sort`'s direction, `xs[i] @ v`, and whether `main` may be fallible | `docs/panel/126-the-document-nobody-had-tidied.md` · `spec § 10 Strings, arrays, maps` · `spec § 11 Built-ins`

    **Origin:** panel 126's ergonomist seat, 2026-09-11, out of three tasks
    written twice each. **`sort`'s direction is the one that matters**: neither
    version of the specification says ascending, both say only *walks them in
    order*, and a tie-break written on the wrong assumption compiles and prints
    a silently different answer. That is the single silent-error risk the seat
    found in six programs, and it is one word of the document. **`xs[i] @ v` is
    given nowhere** while `m[k] @ v` is given, so every sort the seat wrote
    carried a map of taken keys instead of swapping; measure first whether the
    compiler accepts it, because the seat could not. **Whether `main` may be
    `-> ()?`** decides whether `?` is usable in the one function every program
    has, and the document says a file holds `function main()` and nothing more.
    Each is a spec sentence, so each is the panel's; this row is the home
    because that milestone already asks what `heroes check` accepts.

*******************************************************************************
