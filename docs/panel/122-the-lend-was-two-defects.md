# Panel 122 — the lend was two defects

Sitting of 2026-09-09, M-cstr-lifetime. **Full panel, five seats**: the change
carries a diagnostic class, a spec sentence and semantics.

**Convened by the author, in one word — *"chiama il panel"*** — after the
coordinator put a design fork to them that rested on a measurement of its own
that was wrong. What came back was not the ballot. Two vetoes were dissolved by
a reading nobody had proposed, two clauses the ballot lacked were measured free,
and **the defect that opened the milestone turned out to be two defects**, of
which the rule under discussion closes one.

## Proposal (as put to the judges)

Defect 022: `return ("heap-" + n.to_str()).cstr()` builds at exit 0 with zero
diagnostics and hands C a pointer into freed memory — `strlen` answers 0 where
17 is right, and `--sanitize` reports heap-use-after-free freed by
`hero_release_block`. `return "static".cstr()` is sound, because
`HERO_STR_STATIC` gives a literal static storage and an immortal refcount. The
two spellings are one brace apart. `spec:253` is the only sentence in the
project about the lend's lifetime: *"`s.cstr()` lends a `str` to C to read for
that call"*.

Three candidates went out:

1. a lend may only be an argument of an **extern** call;
2. a lend may be an argument of **any** call, plus a type rule that a `cstr`
   may not be returned, bound, or held in an aggregate;
3. **escape-only**: refuse a lend only where it leaves the function.

Candidate 1 was written, wired and uncommitted in the working tree
(`selfhost/check/lending.hero`, a `cstr_escapes` diagnostic, one call in
`selfhost/checker.hero`), so every seat could build it.

## The reading that dissolved two vetoes

**The spec-warden vetoed candidate 2 and the ffi-pragmatist vetoed its
clauses, for the same reason from opposite directions**: as a rule about the
TYPE `cstr` it cannot tell a lend from a pointer C owns. The warden compiled the
program that proves it, and it runs:

```
c = getenv(name: name.cstr())
return c.validated().default("unset")
```

The ffi seat added the shapes that ship: `tests/golden/run/abort-null-cstr-into-c.hero:27`
binds a `cstr`; `tests/golden/fixedbugs/ffi-out-parameter-guard.hero:45-46` puts
one in an `@` cell to pin panel 053's guard placement; `tests/golden/ir/owned-release.hero:26`
declares `-> cstr owned free`. A rule that refused those *while asserting a
lifetime fact false of them* is, in the warden's words, a blur worse than
silence.

**The compiler-engineer supplied the reading that answers both: word every
clause as a rule on the POSITION of the `.cstr()` expression, never on the type.**
Then `owned`, a C-returned `cstr`, the `@` cell and the binding are all
untouched by construction, because the sweep asks the resolver for the `cstr`
**builtin** and never looks at a type. It built that version: blast radius **0
files of 638**, and the compiler's own **591 tests all passed**.

## What each seat found that the ballot did not contain

**The two-hop laundering, and it defeats candidates 2 and 3.** Both compiling
seats built it independently:

```
function pass(c: cstr) -> cstr
    return c

function outer(n: i64) -> cstr
    return pass(c: ("heap-" + n.to_str()).cstr())
```

The lend is an argument of a call, so a position rule blesses it; what leaves
the frame is a **call result**, so no rule about lends sees it. Stock `heroes
check` is silent; `--sanitize` reports heap-use-after-free, READ of size 7.

**And the clause that closes it costs nothing.** The engineer grepped every
`-> cstr` in `selfhost/`, `examples/` and `tests/`: **every one is inside an
`extern` group**. So *a function outside an extern group may not answer `cstr`*
is free, it is a signature rule rather than a flow analysis, and it has no
`written_types` hole — a generic body cannot produce one, measured:
`function escape<T>(...) -> T` returning a lend is `error[type_mismatch]`
before monomorphisation.

**A second free clause, and the note that contradicts it.** A `cstr` record
field outside a group is legal today and dangling:

```
record Holder
    c: cstr
function held(n: i64) -> Holder
    return Holder(c: ("heap-" + n.to_str()).cstr())
```

Silent at `check`, heap-use-after-free under the sanitizer. The engineer
enumerated every `: cstr` field outside an extern group in the tree: **none**,
so the clause is free. **And `selfhost/emit/gate.hero:172` currently
recommends exactly that shape** — *"hold the pointer in a `record` and make an
array of that instead"* — so shipping the clause turns a diagnostic's advice
into a compile error, and that note and its test at `:455` move in the same
commit.

**The literal hole, which would have bitten a type-shaped rule.**
`xs = [("heap-" + x).cstr()]` writes no type node, so a rule reading
`c.out.written_types` would have panel 061's exact hole; only `heroes build`
refuses it today, from the emitter. The `a.exprs` sweep catches it because it
asks the value. **So the rule stays in `check/lending.hero` and does not move
to where types are judged.**

**The §4.19 conflict never existed.** The coordinator reported that candidate 1
refuses a shape §4.19 mandates. The warden located it correctly: §4.19
prescribes a Heroes **route** out of a module and says nothing about the
wrapper's parameter type, and the project ships both spellings —
`tests/golden/surface-fixtures/externroute/bind.hero` takes a `str` and lends
inside (no diagnostic under candidate 1), while
`tests/golden/surface-fixtures/twoarity/as_text.hero` takes a `cstr` and makes
the caller lend (three diagnostics). The ffi seat ran the repair: re-type the
wrapper parameters to `str`, move the lend inside, **exit 0, prints `n = 42`**.

**And candidate 1's two diagnostics point at each other.** Its note says *"write
`.cstr()` directly in the `extern` call that reads it"*; doing so gives
`error[extern_across_modules]`, whose note says *"write a Heroes function beside
the `extern` and call that"*. A reader following either arrives at the other.

**What actually keeps a lend alive, measured from the emitted C.** One
synthetic owner slot **per expression site**, released at function exit **or
when its site re-executes, whichever comes first**: in a loop body the emitted C
carries `t17 = h4_own4; h4_own4 = t10; hero_str_decref(t17);` inside the block,
so iteration *n+1* frees iteration *n*'s bytes. Function exit is an upper bound
and not the guarantee, which falsifies candidate 3's premise — a rule resting on
it asks the world (`.claude/rules/module-shape.md`), and this premise is already
dead inside a loop.

## The class no rule closes, and it is why the defect splits

**Both compiling seats built the same program from opposite ends.** Every lend
is an argument of an extern call — the position all three candidates bless — and
the C side keeps the pointer:

```
/* stash.c */  static const char *held;
               void stash_put(const char *s) { held = s; }   /* keep, do not copy */
```
```
while at < n
    stash_put(s: ("row-" + at.to_str() + "-payload").cstr())
    at @ at + 1
```

Accepted at exit 0 with zero diagnostics by **both** candidate compilers, prints
`0` where 14 is the answer, and `--sanitize` reports heap-use-after-free freed by
the slot rebind. The ffi seat reached the identical class through SQLite's
`sqlite3_bind_text(..., destructor: nullptr)`, which is SQLITE_STATIC — *"the
string is static, keep it"* — inside a loop with `sqlite3_step` after it:
`matched rows: 0` where 1 is right, freed at a `main.c` line with **no `.hero`
position at all**. The discriminator is the call's **fifth argument**, not the
position of the string expression, and no position rule can see it.

**The project already knew, and wrote it down as a rule for a human.**
`examples/ledger/db/sqlite.hero:305-309` ships that exact shape with this
comment above it: *"a caller must step before it drops the string, and every
caller here does, in the next line. Written down because it is the one place a
correct-looking rearrangement would be a use-after-free."*

**And design.md already reserved the answer.** §4.19: *"A binding annotation
vocabulary will eventually be needed… Three cases to cover: a pointer you must
free (and with which function), **a borrowed pointer you must not touch**, and a
buffer that C takes ownership of. Reserve a keyword."* Case 1 is `owned`,
landed at panel 109. **The lend is case 2, verbatim**, and every candidate in
this ballot is a position rule standing in for a fact that belongs at the
declaration.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to 1 · ship 2 read **syntactically** · **veto** the provenance reading | §1.12 via rank 3; §1.7, Part 5 | +129 to ~+150 code lines, one new module, **zero** DECIDED ceilings breached, frontend only; `check` 12.46 s → 12.62 s, **+1.3%** | at the close, a program whose every lend is an extern argument and whose C side retains the pointer is still accepted, and `--sanitize` still reports use-after-free | someone shows twoarity's three refused sites are unsound; it expects no such report |
| ffi-pragmatist | **object** · **veto** candidate 2's type clauses · ship 1's position rule with three corrections | §4.19's reserved keyword, §4.20 | ABI **unmoved**, no new entry point, checker-only; 37 files hold a lend, candidate 1 refuses **1** | `x7` (SQLITE_STATIC in a loop) is accepted at `check` and use-after-free under `--sanitize` at the milestone's close | the fixture re-typed rather than the rule bent; the note rewritten; **defect 022 split** |
| spec-warden | **object** · candidate 1 in a **merged rewrite** at **+20 real tokens** · would **veto** candidate 2 | §1.6, §1.2, CLAUDE.md §12 | merged **+20**, C1 appended +24, C3 +38, **C2 +57 and RED today** at 4104 | with the merged wording, `measure` reads 4010 vendored and 5114 real; the fixture repair is ≤6 lines across 3 files and 0 spec tokens | §1.6 carries 6144 with its tokeniser and date and both `CEILING` pins follow; the module doc corrected; the note's falsehood removed |
| llm-ergonomist | **veto** candidate 1 on locality · approve 2 **with "returned" added** · object to 3 | locality | 10 silences found, 2 able to change a program quietly | ≥30% of first-try programs declare a non-extern function whose signature mentions `cstr`; candidate 1's first-try rate is ≥25 points below | withdraws the veto only if models never write the wrapper **and** the spec says an extern is callable only from its declaring module |
| historian | **approve** 3, advisory; candidate 1 is the one shape the sources reject | precedent | 30 sourced precedents; 6 claims it could not source, each named | if candidate 2 is adopted as written, *"may not be bound"* is the first clause relaxed within two milestones | a shipped language that forbids binding a borrowed C pointer and held it for a release cycle |

## Disagreements, unsmoothed

**The ergonomist and both compiling seats cannot both be satisfied by candidate
1.** The ergonomist vetoes it because `cio.say(...)` and `cio.printf(...)` are
the same line whose legality is decided in another file; the engineer objects
because it refuses three **sound** sites in a fixture §4.19's route prescribes.
Those are the same fact seen from two chairs, and the resolution takes it as
decisive: the extern discriminator goes.

**The historian's central precedent, read carefully, cuts the other way from its
own verdict.** Rust planned `temporary_cstring_as_ptr` deny-by-default and
shipped it warn, and the recorded objection is that
`func(CString::new(..).unwrap().as_ptr())` is *"in typical situations a false
positive"* — that is the ARGUMENT position, the one every candidate here
permits. On the position discriminator Rust agrees with this sitting. Where the
historian's null result stands, and it is what removes candidate 1: **no
language uses which KIND of function is being called as a lifetime criterion.**

**The warden's +20 merged wording was measured against candidate 1's rule and
the resolution takes candidate 2's**, so the sentence changes and its price must
be re-measured before it lands. The warden's reclaiming argument survives the
change: a positional rule implies the duration, so *"to read for that call"* is
paid twice in any appended draft.

**The coordinator was wrong four times in this milestone and every one was
caught by a seat.** The corpus count was reported as 67 of 67 arguments of an
extern call; it is 66 of 67, and two of the four re-reads were not externs at
all — `bind.system(...)` is the **negative** fixture
`tests/golden/surface-fixtures/externroute/wrong.hero`, and `as_text.say(...)`
is a Heroes wrapper. The "1 of 638" blast radius is right about files and
undercounts **sites by three**. The recommended rule had the two-hop hole. And
the conflict was located at §4.19 when it was at one fixture's parameter type.

## Resolution — PROVISIONAL, author ratification pending

The most robust and complete resolution, not the cheapest and not a compromise
(CLAUDE.md § 4).

**R1. Every clause is a rule on the POSITION of the `.cstr()` expression, never
on the type `cstr`.** This is what makes the rule compatible with `owned`, with
a C-returned `cstr`, with a `cstr` in an `@` cell and with a binding — all of
which ship and run — and it is why both type-shaped vetoes fall.

**R2. A lend may stand only as an argument of a call**, extern or Heroes. The
extern discriminator is dropped: it has no precedent, it decides a line's
legality in another file, and it refuses three sound sites in the route §4.19
prescribes.

**R3. A function outside an `extern` group may not answer `cstr`.** Measured
free — every `-> cstr` in the tree is already inside a group — and it closes the
two-hop laundering R2 alone blesses.

**R4. Outside an `extern` group, a `cstr` may not be a record field**, joining
the array, map and `T?` refusals `selfhost/emit/gate.hero` already makes.
Measured free. `emit/gate.hero:172`'s note and its test at `:455` are rewritten
in the same commit, because that note currently prescribes the shape this clause
refuses.

**R5. DEFECT 022 SPLITS, and the milestone may not close as though the class
were shut.** What R2 to R4 close is the lend that **escapes its frame**. What no
position rule can close is the lend the **C side retains past the call**, whose
discriminator is an argument of the C function and not a Heroes position. That
half is re-filed with its own number, carrying both seats' reproducers, against
§4.19's reserved borrow keyword — case 2 of the three the design reserved, of
which `owned` is case 1.

**R6. The spec takes one merged sentence rather than an appended one**, on the
warden's reclaiming argument, re-measured for R2's wording before it lands. The
author has already decided the spec states this refusal (2026-09-09).

**R7. The diagnostic is rewritten**, and three of its sentences are wrong today:
its note prescribes a program `extern_across_modules` refuses; its lifetime
claim, *"released when the function that built it returns"*, is false of all
three sites where it currently fires, which are literals, **and** false in
general, since the slot is released on re-execution; and it cites §4.20, the
runtime, for a promise §4.19 makes. The repair it names is the measured one:
give the wrapper a `str` parameter and lend inside it.

**R8. The sweep gains the engineer's early exit** — five lines asking whether a
file holds a lend at all — so the +1.3% on `heroes check` goes to zero. Rank 5
would have let the cost land; it is free, so it is taken.

**What a veto would compel.** The ergonomist's veto on candidate 1 is honoured
by R2 rather than overridden. The two type-clause vetoes are honoured by R1. The
engineer's veto on the provenance reading is honoured by R1 as well: **177
exhaustive `.cstr` arms in 52 files**, with `emit/inst.hero` at 350 of 350 and
`emit/ctype.hero` at a decided 365, make a second type case a core construct by
Part 5's test and the only version of any candidate that breaches a ceiling.

## Author's verdict

**RATIFIED 2026-09-09** (author instruction, `1a 2a 3a 4a`, meaning all four
sittings as recommended), in full, **R5 included** — which is the part the
recommendation flagged, because ratifying it is agreeing that a milestone closed
over a defect that is still open.

**That reading is the honest one rather than the tidy one, and the split is what
made the tag rule work rather than something the tag rule had to be argued
around.** `M-cstr-lifetime` closed **untagged** on 2026-09-09 because
`docs/work/DEFECTS.md` holds defect **024**, and § Verification says a milestone
is tagged only over a clean list. A tag says the tree is clean; it was not.

**The half that was split off was ruled on the same day.** Panel 124 measured
that no declaration-site mark can express it either, because
`sqlite3.h:4888` puts the retention decision in the FIFTH ARGUMENT of the same
declaration, and adopted a capability instead of a refusal. So R5's claim — that
this is a different class and no position rule can see it — is now confirmed by a
second sitting reading the header directly, and both compiling seats there agreed
independently.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | a program whose every lend is an extern argument and whose C side retains the pointer is accepted at `heroes check` and reports heap-use-after-free under `--sanitize`; falsified if the shipped rule refuses it, or if the closing record names the retained-pointer shape and cites a spec sentence putting it on the programmer | M-cstr-lifetime close |
| compiler-engineer | complete candidate 2 lands `selfhost/check/lending.hero` at ≤140 lines in `suite_layout.hero`'s unit and adds zero `DECIDED` rows | M-cstr-lifetime close |
| ffi-pragmatist | `x7`, SQLITE_STATIC in a loop, is exit 0 with zero diagnostics at `check` and heap-use-after-free under `--sanitize`, so it cannot be annotated and cannot be added as a golden | M-cstr-lifetime close |
| spec-warden | with the merged wording, `heroes measure` reads 4010 vendored, `count_tokens` reads 5114 real, and the fixture repair is ≤6 changed lines across 3 files at 0 spec tokens | M-cstr-lifetime close |
| llm-ergonomist | ≥30% of first-try programs on a print-through-C task declare a non-extern function whose signature mentions `cstr`; and the current silent-wrong-output rate on the helper task is above 50% and goes to 0% under the adopted rule | M-thesis-harness, metric 2 |
| historian | if a *"may not be bound"* clause is ever adopted, it is the first relaxed within two milestones, provoked by `p = s.cstr(); c_foo(p); c_bar(p)` | two milestones after any such adoption |
