# Panel 155 — completeness critic

Not a sixth judge. **No verdict on R1–R4.** This report names what the five
seats and the coordinator's own briefs left out, and where it could settle a
question cheaply it ran the command rather than filing the gap.

Everything marked **measured** below was run in this session, 2026-09-16, on a
seed compiler built into the scratchpad — `clang -I runtime seed/heroes.c
runtime/runtime.c -o <scratch>/heroes`, `real 3.45`, exit 0. The repository
working tree was not modified except by this file. Exit codes captured directly,
never through a pipe. Everything else is marked **unrun** and carries the command
that would settle it.

---

## 1. Routes nobody listed, and two of them are now priced

### 1a. The historian's fourth design (SML inferred equality type variables) — available only in its expensive half

The route: infer the constraint from the body, **publish it in the signature**,
refuse at the call against the signature, no constraint syntax written by the
programmer, no per-instantiation re-check.

**No seat priced it.** The historian found it and was instructed not to
recommend; the other four never saw it (it landed at 10:44, after every other
report). Measured facts that decide its availability here:

- **The cheap-sounding half is the expensive half already priced.** Heroes
  compiles whole-program from one root; there is no separate compilation and no
  signature artifact. So "infer the constraint and attach it to the signature" is
  computed by exactly the store panel 082 R3 proposes — `(decl, param)` →
  obligations, plus a fixpoint. The compiler seat's measured floor for that store
  is **284 + 22 = 306 code lines and it does not yet compile**. SML changes the
  *presentation* of the constraint, not its computation. The saving the historian
  names (no per-instantiation re-check) is a saving Heroes does not have to make,
  because nobody proposed re-checking bodies per instantiation either.
- **The half that would actually differ collides with a ratified refusal.**
  Publishing the constraint means rendering it where a reader reads it.
  design.md:1721 (measured, `grep -n "No constraints" docs/design/design.md`):
  *"**No constraints.** No `where`, no bounds."* spec:258 (measured): *"Generics:
  on functions only, no constraints, always inferred, never written."* A rendered
  inferred constraint is a new surface form, which owes the whole walk in
  `.claude/rules/diagnostics-and-goldens.md` § A new surface form — formatter
  round trip, every `--dump-<stage>` printer, `heroes mutate`, both highlighters —
  and its own panel, because it changes spec § 9.
- **It covers one half of the question only.** SML's `''a` is about equality.
  There is no SML analogue for "may this type be a map key" — that is a library
  property there, not a language one. So the fourth design speaks to
  `ffi_partial_operation` and is silent on `float_map_key`.

**Verdict on availability: available, at the compiler seat's measured cost plus a
surface change design.md and the spec both currently refuse.** It is not the
cheap route it reads as. What would settle the remaining question — whether the
constraint must be rendered at all — is a panel question about spec:258, not a
measurement.

### 1b. The compiler seat's fifth route (make the runtime abort name the call site) — "zero surface cost" is false, measured

The seat offered it unpriced and marked it UNRUN. It is now priced at the
emitter, and the price is not zero.

- **There is no location channel into a panic.** `runtime/heroes_runtime.h:39`:
  `_Noreturn void hero_panic(const char *msg);` — one `const char *`, no file, no
  line, no span. Measured: `grep -rniE "hero_panic_at|__LINE__|call_site|callsite"
  runtime/ selfhost/emit/` returns **zero hits**.
- **The abort does not live at a call site, it lives in a per-TYPE function.**
  The ffi seat printed it: `h_realstat_FileStat_eq` is one generated function for
  the type, reached from every comparison of that type anywhere in the program.
  Naming *the* call site requires threading a location argument into every
  generated `_eq`/`_hash` call — every array compare, every map insert, every
  nested field compare — or a dynamic call-context in the runtime. That is a
  pervasive emitter and runtime change touching the exact two lines
  (`selfhost/emit/structural.hero:57` and `:218`) over which the ffi seat says it
  will cast its veto if they are touched on the wrong premise.
- The seat's own supporting inference — *"it touches `runtime/` and no checker
  pass, so it cannot breach a `selfhost/` ceiling"* — is an inference, and the
  measurement above says the change is at least half in `selfhost/emit/`.

**Still worth keeping on the table**, because a cheaper variant exists that
nobody named: not *the* call site, but **the monomorphic instance name**, which
is already in the mangled function name and already printed
(`h_hrecord_FileStat_eq: …`). That names the type, not the call. Whether a reader
can get from the mangled name to the call is unrun; the command is to write one
program with two call sites of the same generic and read the panic text.

### 1c. The second route the coordinator handed out unpriced is now CLOSED, not open

The compiler seat marked *"a warning is not available"* as a question rather than
a premise, correctly, because its search rested on its own vocabulary. **Settled
this session, measured**: `selfhost/diag.hero:21-26` —

```
variant Kind
    error_kind
    unsupported_kind
```

Two arms. `unsupported_kind`'s own comment is *"The program is fine and THIS
COMPILER is unfinished"*, which is not a warning and never could be reused as
one. `grep -rni "severity" selfhost/` returns **3** hits, all in
`selfhost/emit/ffi_site.hero`, all about parsing **clang's** output. There is no
warning severity in this compiler and adding one is a new diagnostic class, which
is itself a § 4 panel trigger. **The route is closed; the sitting may stop
carrying it.**

### 1d. The sixth route nobody named, and it is the language's own written answer

design.md:1721, the same line the spec-warden quoted for a different purpose,
does not stop at *"No constraints. No `where`, no bounds."* It continues:

> **If an operation on `T` is needed, pass it as a parameter.**

That is a ruled answer to precisely this question, in the design document, and
**no seat cited it as a route**. Applied to the `==` half:

```
function same<A>(x: A, y: A, eq: (A, A) -> bool) -> bool
    return eq(x, y)
```

The body no longer compares, so there is no obligation to compute, no fixpoint,
no `(decl, param)` store, no second span-keyed table, and no call-site pass. The
refusal moves **into the body** — design (1), the shape panel 084 R1 already used
successfully for `sort` — and the diagnostic's `Fix` points at the documented
repair rather than at a dead end. The llm-ergonomist reached the same structural
place independently from the spec alone ("the language has already taught me its
rule … the operation refuses the type parameter, in the body, locally") without
having design.md to cite.

**What this route costs, and it must be stated rather than hidden**: it deletes
`same<A>` as written, and it deletes the ffi seat's compiled `any_equal<A>` —
the same deletion the call-site rule makes, one line higher. The difference is
that the body refusal offers a repair the design document already ratifies, and
it costs nothing in the checker's architecture.

**And it covers one half only.** There is no "pass it as a parameter" for *keying
a map on `K`*: the hazard is in the type, not in an operation the caller could
supply. So the sixth route splits the question **along the same line the seats
split it, for a different reason** — and on the half it covers it is cheaper than
anything the sitting priced.

**Unrun, and it is the highest-value unrun item in this sitting**: what
`ffi_partial_operation` in the body deletes across the corpus. The command is a
body refusal prototyped against `check` `run` `emission` `determinism` `corpus`.
The measurement in § 3 below says the answer is very probably **zero files**.

---

## 2. Claims asserted and not measured, with the command that settles each

### In the coordinator's own briefs — these are the serious ones, because five seats built on them

| # | claim | status | command |
|---|---|---|---|
| A | shared brief: *"**17** generic functions exist across `examples/` and the golden trees"* | **FALSE, measured** | `for d in examples tests/golden; do grep -rnE '^function [a-z_]+<' $d \| wc -l; done` → **17** and **33**. The brief's enumeration omits two thirds of the corpus it claims to enumerate. CL-057: the list is a measurement too, and four seats used this one to judge blast radius. |
| B | shared brief: *"`walk.hero:2225` already walks `keys(c.out.instantiations)`"* — offered as proof "the seam exists and is not speculative" | **misleading, measured** | `sed -n '2220,2232p' selfhost/check/walk.hero`. Line 2225 is inside `test "a generic call binds, substitutes, and is recorded by its span"`, and the walk counts instantiations to assert `recorded == 1`. The compiler-engineer's private brief carried the qualifier *"(inside a test, but it is the shape)"*; **the shared brief dropped it**, so four of five seats read a test assertion as a production seam. |
| C | shared brief: *"The IR route **provably** cannot carry the diagnostic: monomorphisation copies the template's span and discards the call's."* | **true of the live compiler, but its provenance is a forbidden tree** | The milestone file it was copied from cites `ir/mono_subst.rs`, `mono.rs` and `types/apply.rs:139` — `archive/bootstrap-rs/`, which the shared brief's own process rules call a compiler that does not ship and forbids reading. Re-measured here against the live tree: `selfhost/ir/mono.hero:252` reads `span: template.span`, so the conclusion survives. The word *provably* did not survive; it was recalled, not run. This is the CL-017 shape, and it is load-bearing: it is the sentence that excluded one of the three routes. |
| D | CE brief: *"measures **1708** code lines against a DECIDED ceiling of exactly **1708**"* | **FALSE**, caught by the compiler seat | `./heroes run tests/harness/main.hero -- ./heroes layout`; the table reads 1870 and the file measures 1861. Already recorded; named here only because it is the second stale number in the same brief set, and B, C and D share one cause: the briefs were assembled from documents rather than from the tree. |
| E | shared brief / CE brief: the pass is *"~90-110 lines"* | **stale and about a different pass** | The milestone file dates that estimate 2026-08-16 and attaches it to `selfhost/check_sortable.hero`, a file that does not exist and a rule that has since been closed by a body refusal. Measured replacement: **306 and unfinished** (compiler seat). |
| F | shared brief: *"Both rules were measured at panel 084 as **consistency, not safety**"* | **half settled this sitting** | The ffi seat attacked it for `partial` across seven routes and it held. **Nobody attacked it for `float_map_key`.** Partially settled here: see § 4b. |

### In the reports

| # | claim | status | command |
|---|---|---|---|
| G | spec-warden and compiler-engineer: *"exactly two goldens go red"* | **unrun by both** | `./heroes run tests/harness/main.hero -- ./heroes` plus `check` `run` `emission` `determinism` `corpus`. Both seats state the prediction; neither ran a prototype that could produce it. The compiler seat's prototype does not compile. |
| H | compiler-engineer: the fixpoint terminates, bound `\|pairs\| * 2` | **by construction, self-flagged UNRUN** | `./heroes check selfhost/main.hero` on a prototype that compiles. Honest flagging; recorded so the synthesis does not promote it. |
| I | spec-warden: real-token Δ *"estimated at 1.0x to 1.35x"* | **estimate, self-flagged, and it cannot flip the verdict** — 2239 tokens of headroom against a one-sentence edit. Correctly reasoned. |
| J | llm-ergonomist: predictions 1–4 (first-try compile rate moves of ≥10 and ≥20 points, repair rate ≥80%) | **no instrument exists in this tree** | The nearest is `heroes mutate` / metric 3 (`docs/measurements/001-metric-3.md`), which measures the **silent-error rate** on a mutated corpus — a different quantity. The seat's numbers are the largest claims in the sitting and nothing here can score them. Naming the instrument was owed and is missing. |
| K | historian: Rust's *"`f64` is `PartialEq` but not `Eq`"* | **unsourced this session, self-flagged**; and Codeberg ziglang#32099 named as a live lead and not opened. Both correctly marked. |
| L | ffi-pragmatist: *"a call-site check would inherit the depth-16 bound"* | **self-flagged as a reading, not a measurement**. It is the load-bearing step of that seat's strongest finding; § 4a below makes it very much harder to doubt. |

---

## 3. The spec-warden / ffi-pragmatist contradiction: there is none, and the reader must be told why it looks like one

**They are compatible, and both measurements are correct.** I reproduced the
disjointness a third time with a third command
(`comm -12` over `grep -rlE '^function [a-z_]+<' examples tests/golden` and
`grep -rl partial examples tests/golden`): **24 generic-declaring files, 22
partial-mentioning files, intersection empty.** Three independent commands, one
answer.

The two statements are about different universes:

- the warden measured **this repository today** — no file both declares a generic
  and declares a `partial` record, so the `partial` half breaks nothing that
  exists;
- the ffi seat wrote and compiled **a program that does not exist in this
  repository** — `any_equal<A>` over `[FileStat]`, `check` 0, `build` 0, prints
  `false` correctly for the one-element call, aborts 134 on the two-element
  call — so the `partial` half deletes a program shape a binding author would
  plausibly write.

Both true. Nothing to reconcile.

**What must go in the record, because it is not what either seat says.** The
warden offered three measured asymmetries as the ground for splitting R1. The
ffi seat's `any_equal` **removes one of the three**: asymmetry #2 ("deleted
programs: partial zero, float two") is not a difference between the two rules, it
is a difference in **what this repository happens to contain**. A corpus gap is
not a structural property, and it expires the moment somebody writes the program
the ffi seat already compiled. The ffi seat says this in its own words and votes
for the split anyway, "on that ground and on no other" — i.e. on a ground it has
just shown to be contingent.

Asymmetry #3 ("guard dependency: float's bound leans on its guard, `partial`'s
guard has nothing leaning on it") is **falsified by measurement**, and by both
files' own comments. `selfhost/check/partial.hero:44-51` states its `depth > 16`
give-up and calls absence *"the safe direction here"* — safe because the
generated `hero_panic` stands behind it, which is the ffi seat's depth-17
receipt. `selfhost/check/map_keys.hero:115-119` states the identical bound for
the identical reason. **Both walks lean on their guard, in writing, in the same
words.**

So of the three legs the split stands on, **one is contingent on the corpus and
one is false**. The leg that survives is the first and it is the strongest one in
the sitting: `spec:353-354` promises the `partial` behaviour unconditionally and
the spec constrains `K` nowhere. A reader six months from now must be told that
**panel 155's split rests on the spec sentence and on nothing else** — not on a
deletion count, not on a guard asymmetry — because the other two legs will read
as load-bearing and are not.

---

## 4. The question the sitting should have asked

### 4a. The framing is aimed at the wrong thing, and it is now measured on BOTH halves

The ffi seat found the hole at nesting depth 17 with no generic anywhere. **The
float twin has the same hole, and it opens one level shallower.** Measured this
session, straight-line programs, no generic, a chain of `record L0 { f: L1 }`
down to `f: f64`, used as a written map-key annotation:

| nesting | `check` | code |
|---|---|---|
| 1, 2, 8, 15 | **1** | `error[float_map_key]` |
| **16**, 17, 18, 24 | **0** | *(no diagnostic)* |

`float_map_key`'s static rule gives up at **depth 16** in straight-line code.
`ffi_partial_operation`'s gives up at **depth 17**. Neither hole is made by a
generic. **Both halves of this sitting are about a rule the checker abandons by
written design, and the generic is one of at least two routes into the same
abandonment.**

The sitting asked *"is the generic the hole?"* The question it should have asked
is **"is the static rule total, and where does it stop?"** — to which the answer
is written in both walks' own comments, in the same sentence, and is *no, at
depth 16-17, deliberately, because a runtime guard stands behind it*. Closing the
generic route adds one more early report and moves the completeness of neither
rule. Any resolution advertised as *now every route is a compile error* would be
false on the day it lands, in **two** rules and not one, and the ffi seat is
right to say it would move from approve to object over exactly that.

### 4b. The half-measured corollary, run here because it was cheap

The shared brief's § "The second-order cost" and the warden's robustness
objection both rest on *"the runtime guard stands behind the `depth > 16`
bound"*. That claim was never tested past the bound. Measured this session at
nesting depth 16, no generic:

- a **nan** key: `check` 0, `build` 0, run **134**, `panic: a map key that is not
  equal to itself (nan)` — **the guard fires**, because it is a structural
  self-comparison and not a float-aware test, so nesting does not evade it;
- **+0.0 versus -0.0** through the same 16-deep key: `p == n` prints `true` and
  `len(m)` prints **1**, exit **0** — the map agrees with `==`, so this shape
  produces no wrong answer either.

**So the guard claim holds where it was tested, and `float_map_key` remains
consistency and not safety on the two shapes I could reach.** Recorded because
the warden's robustness objection — the one that outranks its token argument and
would have drawn its veto on an undifferentiated R1 — now has one measurement
under it instead of none. Still unrun, and it is what would overturn this: the
other float hazards past the bound, `f32` narrowing and a float inside a variant
case rather than a record field. The command is the same generator with those two
shapes.

### 4c. The milestone's own name does not describe either face, and the face that fits it was never put to the panel

The compiler seat observed that `check` and `build` already agree at 0 for both
remaining faces. The milestone file settles why, and no seat read it out:
`docs/work/milestones/M-check-completeness.md:9-31` lists **four** faces of
*"check accepts ⇒ build succeeds"*:

1. `first([P(x: 1)])` — **CLOSED**, corrected in that same file on the morning of
   this sitting, by a **body** refusal (panel 084 R1). It was the one true
   `check` 0 / `build` 1 face.
2. the map through `tally<K>` — `check` 0, `build` 0, run 0.
3. `same<A>` over a `partial` record — `check` 0, `build` 0, run 134.
4. a module that is nothing but an `extern` group — `check` exit 0 with zero
   output, because **`check` never runs clang**. The file assigns this one to
   M-package-manager.

So: the only face that was ever a check/build gap is closed; the only remaining
face that still is one belongs to another milestone; and **this sitting spent
five seats on the two faces where `check` and `build` agree**. The question the
sitting should have asked is *which of the four faces is still a check/build
gap*, and the answer was in the milestone file the whole time. The face that
closed did so by the route this sitting's brief argues against — a refusal in the
body — which is § 1d's route, and the brief's reason for excluding it (*"a body
refusal for these two deletes programs that run"*) is measured and true for
`float_map_key` and **unmeasured for `ffi_partial_operation`**, where § 3's
disjointness says the deletion count across the corpus is very probably zero.

---

## 5. The instrument damage, and it must be recorded

**What happened.** Three of five seats stalled at the 600 s watchdog with no
output. When the coordinator resumed the compiler-engineer and the
ffi-pragmatist, it passed them the findings of the seats that had already
finished. File mtimes give the order: **llm-ergonomist 10:00:40, spec-warden
10:04:00, compiler-engineer 10:41:00, ffi-pragmatist 10:41:33, historian
10:44:33.**

**What it costs.** The panel's design is differentiated inputs producing
independent verdicts; agreement is evidence only to the degree the seats could
not have copied it. Both resumed seats cite the spec-warden by name. Their
agreement with the warden is therefore **not five independent readings, and not
even three**. Concretely, the sitting's headline — *four of five seats want the
question split* — is really: **one seat proposed the split, two seats who had
been told about it concurred, and one seat reached the same line independently.**

**Which findings are clearly the seats' own** — they could not have come from
what they were told, and they carry full weight:

*Compiler-engineer, independent:*
- the ceiling correction, 1861 against 1870, with an awk replica validated
  against two rows it reproduces exactly — and it runs **against** its own
  argument, which is the strongest evidence of independence in the sitting;
- **there is no node-to-declaration map** in `selfhost/` (`grep -rn
  "decl_of\|owner_decl\|decl_for\|node_decl"` empty), so the store's key cannot
  be computed without two new walks;
- the two rules live in two architectures — `map_keys` is a whole-program arena
  sweep, `ops.hero:47` is inside the expression walk — so there are two
  producers;
- `instantiations` records type arguments only and not the callee, so a second
  span-keyed table is required against `state.hero:78-85`'s written rule. **This
  is the seat's veto ground and it is entirely its own.**
- the 306-line measured floor, and the honest statement that the prototype does
  not compile.

*Ffi-pragmatist, independent:*
- the seven measured routes to the abort and the emitted C that explains why
  there is no wrong answer at exit 0;
- `grep memcmp runtime/parts/*.c` → two hits, both in `str.c`, so no aggregate
  has a byte-wise shortcut;
- the padding measurement — 136 unnamed bytes, zero nonzero at `-O0` and `-O2`;
- **the depth-17 hole with no generic in it**, which is the single most important
  finding in the sitting and which nothing it was told could have produced;
- `any_equal<A>` compiled and run.

*Contaminated, and to be weighted down:*
- the compiler-engineer's *"I accept the spec-warden's finding that the count of
  dead goldens is two"* — **accepted, not re-measured**, and both seats then
  predict "exactly two files go red" as if it were two readings. It is one.
- the compiler-engineer's condition 3, which names the warden's split explicitly.
- the ffi-pragmatist's disjointness number is the good case and should be
  recorded as such: *"I confirm the spec-warden's zero, measured independently
  with a different command."* That is re-measurement, not concurrence, and it
  keeps full weight. (Third confirmation in § 3 above.)

**The one genuinely independent corroboration of the split** is the
**llm-ergonomist**, which finished first, read only `spec/heroes-spec.md` and its
own brief, and reached the same line from the other side: decisive for the
proposal on the `==` case, "closer to a wash, and may be negative" on the map
case. It never saw the warden's report. That is the corroboration the synthesis
should lean on, and it comes with a condition the warden does not state — that
the spec state the map-key rule at its home in § 10 — without which that seat
objects regardless of where the check fires.

**Recommended record.** The synthesis should say, in its own voice, that panel
155's compiler-engineer and ffi-pragmatist verdicts were produced with the
spec-warden's findings in hand after a watchdog stall, name which of their
findings are independent (the lists above), and count the split as **two
independent readings plus two informed concurrences**, not four. And the process
rule this sitting paid for: **a resumed seat is given its own brief again, not
another seat's answers** — a stall costs a re-run, and telling the seat what the
panel already thinks costs the instrument.
