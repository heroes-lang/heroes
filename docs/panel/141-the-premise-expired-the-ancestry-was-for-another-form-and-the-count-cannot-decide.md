# Panel 141 — declaration visibility: the premise expired, the ancestry was for another form, and the count the direction was left to cannot decide

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 6 · **status**
`provisional — author ratification pending`

**Lane: full five seats plus a completeness critic.** The gate CLAUDE.md § 4 asks
once per milestone was given at step 1.

## The item, which is the best-documented in the list

`private` on a top-level declaration, hiding it from every other module. **Costed
and deferred at panel 033** (2026-08-12), where five judges produced no vote to
adopt: ~80 non-test lines, **zero** in the IR, the ownership pass, the emitter
and the mangler, *"because one whole-program translation unit gives visibility no
linkage consequence"*, and the landing form fixed at **+18 measured** spec tokens.
Principle 0 unmet, and its offered § 1 argument refuted by the compiler. Then the
sentence this sitting exists for: **"Direction unresolved and left to a count."**

## Three things the sitting falsified, and the first is the item's own hinge

**1. The decisive premise expired two weeks after it was written** (engineer).
*"One whole-program translation unit gives visibility no linkage consequence"* was
true on 2026-08-12. **M-separate-compilation landed 2026-08-26.** Measured today:
`selfhost/emit.hero:201` emits **157 translation units**, `emit/decls.hero:7` a
prototype per definition, and a never-called helper gets **external linkage**. So
visibility now has exactly the consequence the item says it cannot have.

**And the benefit is already bought without it**: reachability pruning
(`emit.hero:222-224`) means a neighbouring unit carries **zero** prototypes for
the unused helper, with no annotation. What `private` would add is `static` —
performance, a stated non-goal.

**2. The ancestry is for a different form than the one costed** (historian, six
languages verified with sources). All six default to hidden — **and none of them
marks hiding**. Modula-2 uses a separate export list in a separate file; Oberon
and Nim an asterisk **on the declaration, positive**; Zig `pub`, positive; Erlang
a module attribute list; Go the **initial capital, in the name**. *"Item 14's
costed form — a `private` keyword on a default-visible language — has **zero**
ancestry in the set panel 033 cited. Its actual ancestry is Kotlin and C's
`static`."* **Both reversal citations are also wrong as stated**: Rust RFC 0001 is
about **struct fields**, not declaration visibility across modules, and the claim
survives only with a different citation (RFC 0026, which removed `priv`).

**3. The count cannot decide, and "42% to 52%" was never one instrument run
twice** (critic). Three methods ran today: the engineer's **925 private-able of
1868, 49.5%**; the FFI seat's **942 of 1885, 50%**; the warden's 923. The
coordinator's own 967/52% **did not follow the 60 `use … as` lines**, worth 42 to
44 declarations, and is corrected here. And panel 033's 42% was *"a different
method on a pre-port tree a third the size"* — a Rust compiler — so the trend is
not a trend. **The run that would make it one, and nobody did it**: today's script
over a worktree at `m-selfhost-probe`, one method, two trees.

## Verdicts

| seat | verdict | what it measured |
|---|---|---|
| `compiler-engineer` | **object**, and recommends **REFUSED with a Part 6 row**, *"not deferred again — deferring on a premise the compiler has falsified is what this milestone exists to end"*; says plainly that its veto does not fire and does not reach for it | the expired premise above. Cost re-priced against a measured precedent rather than estimated: `partial` is the identical shape, a keyword modifier carried as a bool, and costs **100 lines across 24 files**. Opt-in `private` ≈ **90–110 lines in 9–10 files**, so panel 033's ~80 holds within 25%. **Default-private costs the same compiler lines — the gate inverts, it does not grow — plus a migration the item never priced: ~1181 declarations to mark, and a two-stage seed bootstrap**, since `seed/heroes.c` is a checked-in 796,427-line artifact that must first learn to parse `export` and then be regenerated over the marked tree. **The item prices the two directions as one; that is its second error.** And panel 033's refutation: conclusion sound, **evidence sentence wrong** — an uncalled top-level function gives `heroes check` exit **0, silently**, so the unused rule does *not* reach the top level today |
| `llm-ergonomist` | **A adopt-with-condition, B object**; neither vetoed | counted three versions of one two-module program: 286 characters today, **294 under opt-in (+2.8%)**, **300 under default-private (+4.9%)**. And **it corrected the coordinator's brief**: the specification does **not** say an error carries everything needed to fix the program without opening another file — that sentence is design.md's §4.17, which this seat is forbidden to read, and it said so rather than judging against a premise it could not see |
| `spec-warden` | rules for **default-private** | re-measured both directions against today's re-shaped document; its `real` figures are a scaling it labels an inference, which the critic says could have been a measurement |
| `ffi-pragmatist` | rules for **default-private** | re-measured panel 033's binding claim on today's tree, and added the layer panel 033 never asked about: of 47 declarations in `examples/ledger/db/sqlite.hero`, **26 are extern members whose visibility is already fixed** by `extern_across_modules`, leaving **14 `export` against 33 `private`** — the only discriminator runnable today, and nobody adopted it |
| `historian` | **object** (advisory) — not to the item, but *"to the sitting adopting `private` as the landing form while citing an ancestry in which no language uses it, and to letting the count decide a direction it cannot decide"* | the six-language table above, each with a source and a date |

## The route nobody listed, and it is already in the compiler

**Derive it; never declare it** (critic). The compiler already computes the set and
already owns the linkage word: `selfhost/emit/decls.hero:87` emits
`#define HERO_TU_LOCAL`, empty in a fused unit and **`static` in a per-module
unit** (panel 093 R2), and `selfhost/emit.hero:125`'s `reach_of` already walks
reachability. So the item's **one surviving benefit** — §4.4's unused rule
reaching the top level — ships as an error class with **no keyword, no `Decl`
field, and no change to what `use` binds**:

> *a top-level declaration that no other module names and that nothing in its own
> file uses is a compile error.*

The engineer named that rule in passing — *"the enabler is an unwritten
reachability rule, not visibility"* — and priced neither it nor its diagnostic.
What it does not do is **record intent**, which the reader's seat says is the only
real win of a written marker; so it is named as the form that returns and not
adopted, and the trade is written down for the sitting that takes it.

**And Go's route is unavailable here, measurably rather than arguably**: of 1868
declarations **275 begin with a capital** and they are exactly the 143 records, 86
constants and 46 variants, while the 1593 lowercase are exactly the functions.
**The case bit is 100% spent on kind**, so a Go rule would export every type and
hide every function against a private-able set that is ~89% functions.

## Where they disagree, unsmoothed

**The direction is 2–2 with all four seats claiming to rule** — engineer and
reader for opt-in, warden and FFI seat for default-private. **So this sitting may
not write "the direction is ruled"**, and the critic is right that it would have
to say which measurement beat which. It does not have one.

**The checkable disagreement underneath**: the reader's load-bearing claim is that
under opt-in, omitting the marker is a no-op. The FFI seat's measurement says that
for 26 of 47 declarations in a real binding the unwritten default is a visibility
the resolver already refuses — so omission there is *a declared falsehood rather
than a no-op*. One command settles it and neither seat ran it.

**And one grep decides the engineer against the FFI seat** on whether visibility
has a linkage consequence: both are true, of different emission modes.

## The resolution adopted, provisionally

**Item 14 is REFUSED to Part 6, and the direction is recorded as UNDECIDABLE BY
COUNTING rather than left to a count for a second year.**

1. **The row's falsifier** is the engineer's condition: *M-selfhost-probe
   reporting a real blockage that needs visibility; or a measured Part 11 effect;
   or a `build/tu-*` symbol collision the mangler cannot prevent; or a
   demonstration that `static` on private definitions closes a **robustness** class
   (§1.12) rather than a performance one.* That last outranks compiler size in
   CLAUDE.md § Precedence, and the seat that would pay for it said it would fund
   it at once.
2. **The item's text is struck beneath it**, as this ledger has done four times
   now: the linkage premise expired on 2026-08-26; the ancestry is for a form
   none of the six named languages uses; both reversal citations are wrong as
   cited; and the two directions are priced as one when default-private carries
   ~1181 declarations and a two-stage seed bootstrap that opt-in does not.
3. **"Left to a count" is closed, and closed by saying the count cannot do it.**
   Three methods today read 49.5%, 50% and 50%; panel 033's 42% was another method
   on another tree in another language. **A ratio hovering at half is not a
   tie-breaker, and a second year of waiting for it would produce a fourth number
   near half.** What would decide it is named instead: the FFI seat's per-file
   marker economy on a real multi-module binding — **14 `export` against 33
   `private`** — which is runnable today and which no seat adopted.
4. **The form that returns is the derived rule**, not a keyword, with its one
   known cost written down: it buys the unused-declaration error and does **not**
   record intent.

**What conservative would have been**: defer a third time with the derived rule as
the return condition. The sitting refuses because the item's own hinge is false,
because no seat supports the costed form, and because a Part 6 row carries a
falsifier a reader can check where a third deferral carries a promise.

## Two corrections to the coordinator, recorded rather than quietly fixed

**The brief asserted a sentence the specification does not contain** — that an
error carries everything needed to fix the program without opening another file.
That is design.md §4.17, and the reader's seat is forbidden design.md, so the
brief asked it to judge against a premise it could not see. It said so and judged
anyway, flagging it first.

**And the brief's line citation did not resolve**: `design.md:2988-3005` was item
14 when the brief was drafted and is Part 7 item 13's concurrency text by the time
the historian read it, because this same session amended the document twice above
that point. **A line number in a brief is the citation shape CL-037 says no
instrument can see**, and this sitting is its own example.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 141`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: re-running the count at the fixpoint milestone leaves the private-able share within 45–55% of 1868, and `heroes build selfhost/main.hero` reports zero cross-module name collisions | the count script, `heroes build` | M-selfhost-probe's successor |
| critic: the same script over a worktree at `m-selfhost-probe` gives a number that makes 42%→52% not a trend | `git worktree add`, the script | any later sitting on this item |
| FFI: per-file marker economy on a real binding stays near 14 `export` to 33 `private` | the seat's own script | M-core-packages |
| the falsifier's own clock: a probe blockage, a Part 11 effect, a mangler-proof collision, or a §1.12 class closed by `static` | a sitting that proposes one | every later sitting of this ledger |

## What the seats could not source or could not run

**Neither direction was built**: both are priced by grep, and `heroes build
selfhost/main.hero` over an applied patch is the command this sitting never ran.
The warden's `real` figures are a scaling labelled an inference where a
measurement was available. The reader's six predictions are all scored by a
generation harness whose task corpus is empty — *"Status: 0 tasks"* — so under
§1.6 they pay nothing, and it does not say so. The FFI seat's re-measurement uses
a third denominator and its script never subtracts the extern members its own
finding identifies. The historian could not verify whether item 14 had moved,
its line citation having been invalidated by this session's own edits.
