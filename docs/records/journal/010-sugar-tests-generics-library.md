# 010 — M6: sugar, tests, generics, library

## Goal

Finish the language. Everything in design.md §1.0's closure list that is not
modules or the three FFI conveniences, and the acceptance criterion written into
the ROADMAP since M0: **the appendix calculator compiles, runs, its tests pass**.

Seven steps and three panels: `.must()` carrying its failure · `m[k] @ v` and
`keys(m)` · the five Tier-1 built-ins that had no C · the library, and `range`
with a body · function values as C function pointers · generics by
monomorphisation · `heroes test`.

It closes with that criterion met, and with `heroes mutate` at 97% / 81% over
839 mutants.

## What surprised

**Three panels ran, and in every one the deciding evidence was compiled rather
than argued.** Panel 027 refused a sixth field on the runtime's per-type
descriptor because C11 zero-fills a short initialiser list: every existing
descriptor would have carried a NULL silently, under this project's own flag set,
and the call is a segmentation fault with no type name. Panel 028 refused a
pre-built library because its only generic shape is type-erased — and the judge
compiled that too, passing a `str`-typed function to map an `[int]`: zero
diagnostics under every warning clang has, then a crash. Panel 029 refused a
readable mangled suffix by finding two legal programs that spell the same symbol.
None of the three is an aesthetic judgement and none could have been reached by
reading.

**A pass that has already decided is worse than one that declines to.** The
ROADMAP said monomorphisation runs before the ownership pass because ownership
"cannot decide" with type parameters. It can, and it did: `is_refcounted` answers
*false* for a type parameter, which is right for `int` and a leak for `str`, so
the ownership pass had committed the wrong answer into every generic body in the
tree. The dump showed it — a store with no incref beside an array with one. The
ordering was right for a reason nobody had written down.

**Two judges disagreed about one sentence, and both were measuring correctly.**
The spec's phrase saying which built-ins are written in Heroes buys the compiler
nothing — the tier field is read nowhere, and a Tier-1 name passes as a function
value exactly like a Tier-2 one. It buys the *reader* three things, and one of
them turned out to be load-bearing: "these obey the ordinary rules" is precisely
what makes the mandatory-label rule apply to `range`. The removal was measured,
priced, and then declined, on the other judge's transcript rather than on a
preference.

**The spec contradicted itself for eleven weeks and implementing one function
found it.** `range(a, b)` positional, mandatory labels on same-typed parameters,
`range` written in Heroes: pick any two. Nobody could see it because `range` had
no body — a built-in escapes the label rule, and a declaration does not. The
resolution was to obey the rule, which turns the universal habit `range(0, n)`
into a compile error carrying a machine-applicable fix.

**Landing a feature can make an existing defect reachable.** `fold`'s callback
order and direction were undecidable from the spec, and the wrong guess is silent
wherever the accumulator and the element share a type — `sum`, `concat`, every
string builder. Four well-typed programs, one source text, four outputs, zero
diagnostics. It had never been observable because `fold` had no body. The step
that gave it one is the step that had to carry the fix, and the spec sentence
cost nine tokens.

**The panel's insurance found the fire.** Panel 028 asked for a guard that no
diagnostic may point inside the library, on the historian's prediction that
diagnostic provenance would be the first defect there. It fired on its first
exposure to library code with locals of its own, twice: the library's `fold` has
a local `total`, and a user with a top-level `total` was told their own
declaration was shadowed by a line they cannot open.

**§4.12's claim about size is true and partial.** Generics were supposed to make
the compiler smaller, and `types/builtins.rs` did lose 92 lines when the six
higher-order built-ins became declarations — those lines were literally the
"seven special cases in the type checker, each with hand-written rules" the
document names. They do not pay for the pass, and the honest arithmetic is that
the milestone added more than it removed.

**And the stopping rule refused something the plan scheduled.** `outline` and
`explain` are on the ROADMAP under M6 and neither is admissible: not in the
fixpoint invocation, not needed by any harness, and with no measured effect
because the measurement has not been taken. Panel 016 wrote that rule for exactly
this case. A rule that only ever agrees with the plan is not a rule.

## What broke and why

**The map shipped without copy-on-write, and without a move.** `n = m` then
`m["b"] @ 2` changed `n` too — exit 0, ASan clean, leak counter zero, a green
harness on a program violating "no aliasing exists anywhere". And `m[k] @ v` with
a refcounted value leaked one block per insert, because the ownership pass hands
the value over and the primitive copied it instead. Neither was visible to
`run/maps.hero`, which uses `{str: int}`: an int's copy is an assignment, so the
double incref is invisible unless the *value* owns a block, and the file never
aliased a map before mutating it. The cheaper spelling of a container test passes
while testing neither rule.

**A mandate the runtime never implemented.** design.md says slicing that lands
mid-sequence is an error; `slice("caffè", from: 0, to: 5)` exited 0 and printed a
corrupt byte. Found by a judge pricing a spec sentence, not by a test.

**The gate never asked what a container held.** `[()]`, `[ptr]` and `{str: ptr}`
checked clean, built binaries, and aborted saying "this is a compiler bug" — for
three milestones. The generated C said what was wrong and nobody read it:
`hero_unreachable(); /* not an array */`.

**An error type with no diagnostic is poison.** A built-in used as a value
answered `error_ty()` and reported nothing, and the comment beside it claimed the
case was unreachable. `map(xs, to_str)` type-checked at exit 0 and produced C
that does not compile. Found by the closure-list audit on its first program.

**Two of my own, both caught by checks written before the code.** The
monomorphisation pass instantiated a generic call inside a generic body verbatim,
carrying a type parameter into the copy — the phase check said so on the first
run. And the emitter named function typedefs off the interned arena rather than
off the program, so three golden files changed for programs that use no function
value at all.

**And a lesson about the tools rather than the compiler.** Two debugging detours
in this milestone were caused by reading a *stale* artifact: the build cache is
keyed on the compiler's version string, not on its code, so changing the emitter
does not invalidate it. Both times the symptom was a fix that appeared not to
work. It is worth a line in the record because the next person to meet it will
lose the same twenty minutes.

## What landed, and what carried forward

Moved verbatim from `docs/ROADMAP.md` on 2026-08-12, when the ROADMAP became a
file about what is next (CLAUDE.md §14). The identifiers are the ones this
milestone was built under.

**M6 closed 2026-08-11, tag `m6` — the language is finished, and the
calculator's tests pass.

    $ heroes test examples/calculator.hero
    7 tests, all passed

That is the acceptance criterion this file has carried since M0. Nine of design.md
§1.0's fourteen language rows and **every** library row are done; the four that are
not are **modules** (M8a, the namespace) and **file I/O · `args()` · `exit(code)`**,
whose route — `extern`, shim, or built-in — M7 decides, because panel 030 R3
refuted by compilation the audit's assumption that they are plain `extern`s.
`docs/measurements/003-closure-list-audit.md` is Principle 0's checkpoint, run
under panel 005's three riders and answered as a program rather than an argument:
`tests/golden/run/closure-list.hero` exercises every entry and runs clean.

**The order of what remains was revised by panel 030** — modules, then the FFI,
then a probe, then the port and the fixpoint, with separate compilation and
everything in Part 7 after it. The table is below under *What is left*, and the
milestone identifiers are **not** chronological: read the order from the table.

**Seven steps.** `.must()` carrying its failure · `m[k] @ v` and `keys(m)` (panel
026) · the five Tier-1 built-ins that had no C (panel 027) · the library, embedded
and compiled with every program, with `range` (panel 028) · function values as bare
C function pointers · generics by monomorphisation before the ownership pass (panel
029) · `heroes test`, one process per test.

**Three panels, and in every one the deciding evidence was compiled rather than
argued.** A sixth field on the descriptor ABI was vetoed because C11 zero-fills a
short initialiser list, so every existing descriptor would carry a NULL silently
under this project's own flags. A pre-built library was vetoed because its only
generic shape is type-erased — measured: zero diagnostics under `-Weverything`, then
SEGV. A readable mangled suffix was vetoed by two legal programs that spell one
symbol.

**Six live defects found, five of them reachable for milestones.** The map shipped
with no copy-on-write (`n = m` then `m["b"] @ 2` changed `n`, exit 0, ASan clean) and
leaked one block per refcounted insert; `slice("caffè", from: 0, to: 5)` exited 0
with a corrupt byte against a design.md mandate; `[()]`, `[ptr]` and `{str: ptr}`
built binaries that aborted saying "this is a compiler bug"; a built-in used as a
value produced C that does not compile; and the spec had contradicted itself since
v0 about `range`. Each has a case named after it.

Spec at **2363** of 4096, 1733 of headroom. `heroes mutate` 97% / 81% over 839
mutants — up from 96% / 79%, without this milestone adding a diagnostic: the labels
`range` now requires make a whole class of mutation catchable. **That pair is not a
rate this compiler ever had**: `mutate` was scoring a `Source` with no library
attached while every CLI invocation attaches one, and M8a made the two the same
pipeline. The honest figure for the same corpus is 96% / 79%.

**Runnable:** `heroes test examples/calculator.hero` · `heroes run
tests/golden/run/closure-list.hero` · `heroes run tests/golden/run/generics.hero
--sanitize` · `heroes run tests/golden/run/library-higher-order.hero` · `heroes build
tests/golden/run/generics.hero --emit-c` (the instances, with their types in a
comment above each).

**What the audit refused, and it is the first time the stopping rule has refused
something this file scheduled:** `outline` and `explain` are not admissible. Neither
is in the fixpoint invocation, neither is needed by a harness, and there is no
measured Part 11 effect because the measurement has not been taken. They wait for
the harness run.


### M6 — Sugar, tests, generics, library ✅
`T?` operators, function values (C function pointers), generics by
monomorphisation, `test`/`assert` with source text, `outline`, `explain`.
Library in Heroes: map/filter/fold/find/any/all/range — `join` is Tier 1, in
the C runtime with its `Builder` (§1.11, §4.20; this line used to say
otherwise, and the ffi-pragmatist found the contradiction in panel 015).
**Acceptance: the appendix calculator (restored to `examples/`) compiles,
runs, its tests pass.** ✅ 2026-08-11 — `examples/calculator.hero`, seven tests.
Principle 0 checkpoint: ✅ `docs/measurements/003-closure-list-audit.md`, under
panel 005's three riders. It assigns file I/O / `args()` / `exit(code)` **plain
`extern`s rather than Tier 1** — they are C functions with C signatures, the case
§4.19 exists to serve — and refuses `outline`/`explain` under §10's stopping rule
until the harness measures them. **Still owed: the first *tests-pass* rates, both
arms** — the harness has not run since M5b, which is what leaves four panel
predictions unscored and the tier phrase's −6 removal unresolved.


