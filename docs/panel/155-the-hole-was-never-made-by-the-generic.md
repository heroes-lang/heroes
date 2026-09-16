# Panel 155 — the hole was never made by the generic

2026-09-16, at M-check-completeness. Full panel, five seats, plus the
completeness critic. **Resolution provisional — author ratification pending.**

The sitting was convened to decide whether a generic CALL should be refused when
the types it binds make the generic's body illegal. It adopted a different
answer, because the seats measured that the thing the question blamed is not what
makes the hole.

## The proposal, verbatim as put to the seats

> Should a generic CALL be refused when its bindings make the generic's body
> illegal — closing `float_map_key` and `ffi_partial_operation` through a generic
> — given that doing so deletes a program the repository documents as "must keep
> working"?

The shape proposed was panel 082 R3's, ruled 2026-08-16 and never built: record
per `(decl, type-parameter index)` the obligations a generic's body incurs,
propagate through generic-to-generic calls by a fixpoint, and check each
obligation against the concrete type at the call site.

The four questions put were R1 (is the call an error), R2 (what happens to
`count([1.5, 2.5])` and the runtime guard behind it), R3 (what does spec § 13's
promise mean through a generic), R4 (does the answer differ between the two
rules).

## The verdict table

| seat | R1 | R2 | R3 | R4 | veto |
|---|---|---|---|---|---|
| **compiler-engineer** | **veto** as one question; approve for `partial` alone on the split | object | approve: the spec is qualified, not the compiler | object — split it | **cast** on R1 as one question |
| **llm-ergonomist** | object — direction right, spelling wrong | — | the spec does not contain the rule at all | — | not cast; **armed** on transitive propagation |
| **spec-warden** | object as one question; split it | object — the blast radius is two goldens, not one | the compiler is wrong, +0 tokens | object — split, decisively | not cast; would cast on an undifferentiated R1 |
| **ffi-pragmatist** | approve for `partial`, object to the stated reason | approve — padding is clean | the compiler has the bug, two routes wide | object — the halves have the SAME structure | not cast; **armed** on the emitter panic |
| **historian** | advisory object, narrow | — | — | — | none (advisory seat) |

Costs and deltas. `ffi_partial_operation` adopted: **+0 spec tokens**, because
spec § 13 already promises it unconditionally. Refused: **+10 to +14** cl100k to
qualify the sentence. The call-site pass: a measured floor of **284 code lines
plus 22 of wiring**, against panel 082's estimate of *90-110*, and unfinished —
the prototype did not pass `heroes check`. Spec stands at **5989** vendored,
**7974** real, ceiling 10240; § 1.6 does not decide this sitting either way and
saying otherwise would dress a small payment as an argument.

## What the seats measured, and what it settles

**The `partial` abort is complete, and *consistency not safety* survives.** Seven
programs against `sys/stat.h` with `record FileStat tag stat partial` — direct,
record field, array element, map value, variant payload, `FileStat?`, and through
a holder used as a map key — are all `check` 0, `build` 0, run **134**, all
`panic: …_FileStat_eq: a partial record has no structural equality`, identical
under `--sanitize`. The reason is architectural and was read out of the emitted
C: the refusal lives inside the partial type's generated `_eq`/`_hash`, the one
function every route must call, and `grep memcmp runtime/parts/*.c` returns two
hits, both in `str.c` — there is no byte-wise shortcut for any aggregate.

**The padding question is clean.** `sizeof(struct stat)` 144, `offsetof(st_size)`
96, so 136 unnamed bytes and a hole at 28..31. The emitter's own construction
line lifted into C, the frame dirtied with `0xAA`, compiled `-O0` and `-O2`:
**136 of 136 bytes zero**, hole zero, struct-assignment `memcmp` 0. Panel 061's
*124 of 124* reproduces as 136 of 136.

**THE FINDING: the hole is not made by a generic.** Straight-line code, no
generic anywhere, a chain of nested records with the offending type at the
bottom:

| depth | `ffi_partial_operation` | `float_map_key` |
|---|---|---|
| 1 to 15 | `check` **1** | `check` **1** |
| 16 | `check` **1** | `check` **0**, no diagnostic |
| 17, 18, 20, 24 | `check` **0**, `build` 0, run **134** | `check` **0**, no diagnostic |

Both walks abandon at their `depth > 16` give-up, and both comments call that
absence *"the safe direction"* — safe only because a runtime guard stands behind
it. **The sitting asked whether the generic is the hole. It should have asked
whether the static rule is total, and where it stops.** Both walks answer that in
their own comments.

**And `check` and `build` already AGREE at 0 on both remaining faces.** The
milestone is named *what `heroes check` accepts, `heroes build` compiles*. Face 1
was the only true `check` 0 / `build` 1 gap and it was closed on the morning of
this sitting, by a **body** refusal. Faces 2 and 3 are not check/build gaps at
all; the divergence is at **run**. The milestone file said so the whole time and
nobody had read it that way.

**The runtime guard, tested past the bound for the first time.** At depth 16 a
`nan` key still aborts 134 — the guard is a structural self-comparison, so
nesting does not evade it. And `+0.0` against `-0.0` gives `==` true and
`len(m)` 1 at exit 0.

## The precedent, and it runs against the proposal

Stroustrup, P0557r1 (2017-01-31), read first-hand, on his own design:

> The lack of well-specified interfaces led to the spectacularly bad error
> messages we saw over the years.

> The requirements of `sort` on its argument type are implicit ("hidden") in its
> function body. … The error message for `sort(d)` will appear only when the
> template is instantiated, and that may be long after the point of call.

That is the proposal's design, diagnosed by its author thirty years after
shipping, and the example is literally `sort`. Go's type-parameters proposal
rejects the mechanism by name — *"We don't want to derive the constraints from
whatever `Stringify` happens to do… a minor change to `Stringify` might change
the constraints"* — which is exactly what an obligation store derived from a body
does. Zig, the closest living relative, has had the constraint request open since
2018 and **closed `not_planned` in 2023**, still argued after closure.

**The precedent for what Heroes does today comes from Go**, which loosened a
compile-time refusal into a run-time panic for generic map keys in **1.20**. So
the record's claim that Heroes is the only language making this a run-time event
does not survive as stated; narrowed to floats it survives unfalsified.

**A fourth design the framing omitted**: Standard ML's equality type variables —
infer the constraint from the body, publish it in the **signature**, refuse at
the call against the signature, with no constraint syntax written and no
per-instantiation re-check. The critic then measured that only its cheap half is
available here: Heroes compiles whole-program, so the inference is computed by
exactly the `(decl, param)` store that costs 306 lines, and *rendering* the
constraint collides with design.md:1721 and spec § 9's *"no constraints, always
inferred, never written"*.

## THE ROUTE NOBODY LISTED, and it was already written down

design.md:1721 does not stop at *No constraints. No `where`, no bounds.* It
continues:

> **If an operation on `T` is needed, pass it as a parameter.**

No seat cited it. It is design (1) — the body refusal — which is the shape that
closed this milestone's first face, and it is what the shipped `sort` diagnostic
already recommends in words: *take the comparison as a parameter instead, `less:
(function(candidate: A, best: A) -> bool)`*.

**Measured at the synthesis, and it is what the split now rests on.** Of the
**50** generic functions in this repository — 17 in `examples/`, 33 in
`tests/golden/`, **0** in `selfhost/` — exactly **two** use `==` in the body, and
**neither compares two values of a bare type parameter**: `first_or<B>` compares
`xs.len() == 0`, which is `i64`, and `same<K>` compares two `{K: i64}` maps,
which *hold* a type parameter without being one. So a body refusal of `A == A`
**deletes zero programs in this tree**, while the same rule aimed at a type
parameter used as a map key deletes two goldens.

## The resolution adopted — provisional, author ratification pending

**R1. The call-site obligation pass is REFUSED**, and on soundness rather than on
price. The compiler seat's veto is uncontested and its ground is a standing rule
the sitting did not know it was breaking: `instantiations` does not record who
was called, so the pass needs a second span-keyed table, against
`selfhost/check/state.hero:78-85` — *"Any future table keyed by a span belongs on
the first side of that line."* It also duplicates `map_keys.refuse` and
`ops.hero:47` into a third file, the drift `map_keys.hero`'s own header exists to
prevent; it needs a node-to-declaration map the compiler does not have
(`grep -rn "decl_of\|owner_decl\|decl_for\|node_decl" selfhost/` is empty), which
is 150 of its 284 lines; and its own walk needs a `depth > 16` give-up **with no
runtime guard behind it**, which is the defect 035 shape reproduced in the
repair.

**R2. `ffi_partial_operation` through a generic is closed in the BODY**, by the
language's own written answer: `==` on two values of a bare type parameter is a
compile error, and a generic that needs equality takes it as a parameter. Zero
programs in this tree are deleted, measured. This is panel 084 R1's shape, the
`sort` note's own recommendation, and it keeps the diagnostic derivable from the
line plus its signature, which is the condition the llm-ergonomist's veto is
armed on. **spec § 13 then becomes true as written and costs +0 tokens.**

**R3. `float_map_key` through a generic WAITS**, under Principle 0. No compiler
need (`selfhost/` has zero generics), no spec sentence behind it — the document
constrains `K` nowhere, which the llm-ergonomist found from the spec alone and
the spec-warden confirmed from the tree — and no measured Part 11 effect. The
body rule that closes R2 would delete `tests/golden/run/abort-map-key-nan.hero`
and `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero`, the second of
which was **already rewritten once** to route through a generic, so this would
close the route that rewrite moved to.

**R4. The real defect is filed, and it is not about generics.** Both static walks
abandon past a nesting bound and pass programs with no generic in them: `check` 0
at depth 16 for `float_map_key` and at depth 17 for `ffi_partial_operation`. A
silence where a message is owed is a defect by this project's own definition, and
it goes to `docs/work/DEFECTS.md` with the reproducers above. **The spec-warden's
third asymmetry was false and this is why**: both walks state the same give-up in
the same words, licensed by the same guard.

**R5. What the specification owes, and it is the ergonomist's condition.** The
document must state at its own home — § 10, where maps live — which types may key
a map. Today a reader is given exactly two map-key refusals, both in § 13, and
naming two closes the list. Until that sentence exists the compiler refuses
`m: {f64: i64}` on a rule the language never states, which is CLAUDE.md § 12
pointing the other way from usual: here the spec's silence is the bug. Priced at
+10 to +20 vendored by the warden. **This is R5 and not part of R3**: the
sentence is owed whether or not the generic route is ever closed.

**What the conservative resolution would have been**, recorded so the author can
take it: adopt the call-site pass for `ffi_partial_operation` alone, as three
seats were willing to. It is rejected here because it buys the same behaviour as
R2 at 284 lines instead of a body rule, against a soundness veto, and against
four language communities' measured experience of that exact mechanism.

## What a veto would compel

The compiler seat withdraws its veto on any one of: a prototype passing `heroes
check` at ≤150 total code lines with the net green and no golden rewritten; a
demonstration that this closes a §1.12 robustness class rather than a consistency
one; or the split. The ffi seat casts its veto the moment a resolution touches
`selfhost/emit/structural.hero:57` or `:218` on the ground that the static rule
now covers it — it does not, depth 17 is measured — and moves to object if the
call-site refusal lands without the depth hole filed as a defect, which R4 files.
The llm-ergonomist vetoes if a constraint propagates transitively through nested
generic calls without the diagnostic naming the full chain; R2 has no
propagation, so the trigger is not reached.

## Author's verdict

**RATIFIED 2026-09-16, as adopted — BY DELEGATION AND NOT BY READING, and that
distinction is the record's and not a formality.** The author's instruction, in
full: *continue until the step is finished with zero defects and zero open
decisions — ratify the panels for me — and at the end push everything and check
that CI is green on all three systems.* So the yes below is the assistant's
judgement exercised under an authority the author handed over, on a sitting the
author has not read. Recording it as *the author ratified* would credit them
with a reading that did not happen (`.claude/rules/records.md` § And the record
says whose idea it was), and nobody would check it.

**What that means for anyone reading this later**: the resolution stands and the
work built on it is legitimate, but this ratification carries none of the
independent weight a read one does. The conservative resolution below is
untaken rather than rejected, and it stays available at no cost — R2 landed in
`selfhost/check/ops.hero` as a body rule, so reversing it is a deletion and not
an unwinding.

Queued as `panel 155` in `docs/work/DECIDE.md` on 2026-09-16 and closed the same
day; the item's record entry carries the same distinction.

**What a yes would settle.** That a body refusal is this language's answer to an
operation a type parameter may not support, because `design.md:1721` already says
so and the shipped `sort` diagnostic already recommends it in words — so the
question is not which mechanism to invent but whether the rule already written is
applied consistently. That `spec § 13` becomes true as written at +0 spec tokens.
That `float_map_key` through a generic stays open, and is not a defect but a
position: the runtime guard is the rule there, and the specification does not yet
say otherwise. That the depth-bound hole is the thing worth repairing, and it is
defect 046.

**What a yes would NOT settle**, stated so the tick is not read as more than it
is. It does not lift the llm-ergonomist's standing veto ground: `spec § 10` still
owes a sentence naming which types may key a map, and until it exists the compiler
refuses `m: {f64: i64}` on a rule the language never states — that is R5 and it is
owed whichever way R1 goes. It does not decide whether constraints on generics
ever enter; nothing here proposes them and the fourth design the historian found,
Standard ML's inferred equality variables, was measured as available only in its
expensive half. And it does not touch `selfhost/emit/structural.hero` — the ffi
seat's veto is armed there, and depth 16 is measured proof the static rule does
not cover what that panic covers.

**If the author takes the conservative resolution instead** — the call-site pass
for `ffi_partial_operation` alone — then R2 is withdrawn, the compiler seat's veto
must be answered rather than avoided, and its condition names the price: a
prototype passing `heroes check` at ≤150 total code lines with the net green and
no golden rewritten, against 284 measured.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | a built call-site pass reads **≥284** code lines in the layout unit and needs its own `DECIDED` row | if ever built; `-- ./heroes layout` |
| compiler-engineer | `-- ./heroes run` goes red on **exactly two** files if the float half lands | M-check-completeness close |
| spec-warden | the `partial` half alone leaves `measure` at **5863/5989 unchanged**, digest matching, `spec` green with no `--refresh` | this milestone, at R2's landing |
| spec-warden | refusing both requires qualifying the spec at **+10 to +14** cl100k | this milestone |
| ffi-pragmatist | a depth-17 nest passes `heroes check` at **0** and aborts **134**; it cannot live in `tests/golden/check/`, having no diagnostic to snapshot | R4's defect, at its repair |
| ffi-pragmatist | `examples/ledger/db/sqlite.hero` and `examples/sqlite/main.hero` need no edit; `corpus` scores it | this milestone |
| llm-ergonomist | a § 10 clause naming legal key types raises first-try rate on the direct form by **≥20 points**; no instrument for this exists in the tree today | M-thesis-harness |
| historian | if design (2) is ever adopted, ≥1 golden diagnostic will have its primary span in a file other than the one being compiled | the milestone after any such adoption |

## Process, recorded against this sitting

**Three of five seats were killed by the watchdog at 600 seconds** with no
output — the compiler, ffi and historian seats — and all three were resumed or
relaunched. Panel 087 recorded four of five for the same cause. The seats that
measure run long silent commands, and that is now twice.

**The coordinator damaged the instrument and it is recorded in the coordinator's
own voice.** When resuming the two stalled seats, the coordinator passed them the
findings of the two that had finished. Their verdicts are therefore **not
independent** of the spec-warden's. By report mtime the order was
llm-ergonomist 10:00, spec-warden 10:04, compiler-engineer 10:41,
ffi-pragmatist 10:41, historian 10:44. So the split is **two independent readings
plus two informed concurrences**, not four of five: the llm-ergonomist reached it
first, reading only the spec, and never saw the warden.

Clearly the resumed seats' own, at full weight: the compiler seat's ceiling
correction — which runs against its own argument — the absent node-to-declaration
map, the two-producer architecture, and the second span-keyed table that is its
actual veto ground; the ffi seat's seven routes, the `memcmp` count, the padding
measurement, `any_equal<A>`, and above all the depth-17 hole, which nothing it
was told could have produced. Contaminated: the compiler seat accepted the
two-golden count without re-measuring, so the two seats' agreement on it is one
reading and not two.

**The process rule this sitting paid for: a resumed seat is given its own brief
again, never another seat's answers.**

**Four numbers in the coordinator's own briefs were carried from documents rather
than measured**, which is CL-017 against the seat that wrote them:

- *`check/walk.hero` is 1708 against a ceiling of exactly 1708* — measured
  **1861 against 1870**, nine lines of headroom. It cut FOR the proposal, and the
  compiler seat refused to stand on an objection it had disproved.
- *17 generic functions across `examples/` and the golden trees* — **17 in
  `examples/` and 33 in `tests/golden/`**, 50. Four seats judged blast radius on
  it.
- *`walk.hero:2225` already walks `keys(c.out.instantiations)`*, offered as proof
  the seam was not speculative — it is **inside a `test` block**. The private
  brief carried the qualifier; the shared one dropped it.
- *the IR route **provably** cannot carry the diagnostic* — traced to `.rs` paths
  in `archive/bootstrap-rs/`, the tree the brief's own rules forbid reading. The
  conclusion survives re-measurement against `selfhost/ir/mono.hero:252`, but the
  word was recalled, not run, and it is the sentence that excluded a route.

**One route the brief offered is closed, not open**: there is no warning
severity. `selfhost/diag.hero:21-26`'s `variant Kind` has exactly two arms, and
`grep -rni severity selfhost/` returns three hits, all parsing clang's output.
The compiler seat filed it as a question rather than a premise, which was
correct, and this settles it.
