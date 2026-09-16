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

**Corrected 2026-09-16, at this milestone's opening: the first face is CLOSED,
and the sentence above is false as it stands.** Measured on a compiler built
from the seed, `first([Point(x: 1, y: 2)])` through a `function first<A>(xs:
[A])` whose body sorts is `check` **1** and `build` **1**, not `check` 0 —
panel 084 R1's body refusal closed it, by the very route the next paragraph
names as the one that worked. The other three faces were re-run the same day and
each holds exactly as written: the map through `tally<K>` is `check` 0, `build` 0,
**run 0 printing 1** where `m: {f64: i64}` written down is `check` 1; and
`same<A>(x: a, y: b)` over a `partial` group record is `check` 0, `build` 0,
**run 134** — `panic: a partial record has no structural equality` — where `a ==
b` written down is `check` 1. **What was found in the closing rather than in the
gap**: the refusal that closed the first face had no golden case, only a unit
test asserting `is_refusable` on a `.generic` type id, which is a fact about a
table entry and not about a program.
`tests/golden/check/sort-through-a-type-parameter.hero` is that case, and it
pins both spellings — `sort(xs)` and `xs.sort()`.

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
**OPEN: 2**

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

    **THE FOUR MEASUREMENTS THIS ITEM ASKED FOR ARE RUN, 2026-09-16, and the
    sitting has no precondition left.**

    1. **The nested fallible has a stable representation under monomorphisation.**
       `--dump-ir` on the `find`-over-`[i64?]` program names it in full:
       `slots xs: [i64?] · found: i64?? · $f0: i64?? · $own7: i64??`, and
       `$t8: i64?? = call heroes find($t6, $t7)`. So the IR prints a type the
       parser refuses, exactly as the diagnostic does — the type is real
       everywhere except in the syntax.
    2. **`?` peels the outer level in the emitted C as well**: a two-level
       program emits at exit 0 and the C carries 26 fallible/tag references, so
       nothing is lost between the checked path and the artifact.
    3. **`hero_runtime_check_leaks()` is CLEAN on a nested fallible carrying a
       `str`**: `{str: str?}` with an allocating payload, built and run, exit 0.
       So the shape is not leaking and the question is consistency, not safety.
    4. **The library list is COMPLETE and it is one function, not six.** Of the
       six generics in `selfhost/library_source.hero` — `map`, `filter`, `fold`,
       `find`, `any`, `all` — only `find<A>(xs: [A], f) -> A?` WRAPS what it
       bound: `map` and `filter` return `[B]` and `[A]`, `fold` returns `B`
       bare, `any` and `all` return `bool`. The item recorded *one of six
       measured*; all six are measured now and the one was already the whole
       list. A user generic still nests without bound, which is unchanged.

    **What is left is the sitting**, and both routes were re-run the same day to
    confirm the ground under it: `m: {str: i64?}` with `v.must().must()` is
    `check` 0, `build` 0, run 0 printing 1, and so is the `find` route.

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

    **ALL THREE ARE MEASURED, 2026-09-16, and every one has an answer the
    document does not give.** The seat could not run them; its only input is the
    specification, which is the point of that seat and the reason these stayed
    open.

    | the question | the compiler, measured | the document |
    |---|---|---|
    | is `sort` ascending? | **yes** — `1,2,3`; `apple,fig,pear`; `false,true` | says only *walks them in order* |
    | is `xs[i] @ v` accepted? | **yes**, exit 0, prints `99` | gives `m[k] @ v` and nothing for an array |
    | may `main` be `-> ()?` | **no**: `error[main_returns]`, *"a program reports failure by what it prints, not by what it returns"* | says a file holds `function main()` and no more |

    **So none of the three is a compiler defect and all three are silences.**
    The sitting's question is therefore narrower than the item first framed it:
    not *what should the language do* but *what does the document owe*, with the
    behaviour already settled. `sort`'s direction is the one that matters, as
    the seat said — a tie-break written on the wrong assumption compiles and
    prints a silently different answer, and it costs one word.

*******************************************************************************
