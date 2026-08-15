# Panel 065 — the inventory lives in the marks, never in prose

**Lane: soundness** (compiler-engineer, ffi-pragmatist). Convened 2026-08-15.
**Provisional — author ratification pending.**

**What the lane gave up**: no llm-ergonomist, no spec-warden, no historian. The
proposal changes no surface, no diagnostic text and no spec token — it corrects a
false factual sentence in design.md §4.9's port note and adds code comments — so
there is no reader-facing half for the other three seats to be differentiated
about. The spec-warden's number is knowable without a seat: `spec/heroes-spec.md`
is untouched, so the count stays 3374. What was genuinely given up is a
historian's check on whether other self-hosting compilers marked their
order-sensitive walks; nothing in the resolution leans on precedent.

## The proposal, verbatim

> Amend design.md §4.9's port note. The OLD text (design.md:1329-1335) claims the
> Rust bootstrap "iterates a map in exactly one place (`types/holes.rs`)" — false
> today by a read-only sweep: 22 iteration sites over `BTreeMap`/`BTreeSet` in
> non-test code, 13 of them direct walks whose order reaches an artifact (3 into
> the emitted C, the rest into diagnostic text and `--dump` output), 3 internal.
> `types/holes.rs` itself regained a map walk (`nearby`, holes.rs:171) after the
> SCHEDULED item recording the correction was verified — the fact rotted twice in
> three days, which is the argument for marks over prose.
>
> NEW: the note states the discipline (every ordering-sensitive walk carries
> `// ORDER:` at the write site; each becomes an explicit `sort` in the Heroes
> port, or the M-selfhost-fixpoint diff breaks), names the artifact classes, and
> carries the count only as a dated snapshot. Plus: the 13 `// ORDER:` markers
> land (code comments only — no code change, no spec change, no new diagnostic).

## Verdict table

| judge | verdict | section | measured cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | approve-with-conditions | design.md §4.9 under §1.1/§1.7; CLAUDE.md §7, §9, §11 | 13 one-line comments across 10 files + ~3 net design.md lines; zero code change; largest touched file 295 lines, all under ~300. Verified the 13/3 split site by site, swept the whole collection census (**16 direct iteration sites**, zero `HashMap` anywhere, zero existing `ORDER:` marks), and reproduced the tie-break empirically: `x: i64[2][3] @ …` gives two `fixed_outside_a_group` **both at 2:8**, inner `i64[2]` first — map (node-id) order surviving the stable span sort at `types/mod.rs:171` | Once the tie-break golden exists, at **M-selfhost-port** the ported `fixed_only_in_a_group` passes it *only* by sorting offenders on the map key: a span-only sort emits the outer diagnostic first and the golden diff shows the swapped pair at 2:8 | (1) inventory command scoped to code: `grep -rn "// ORDER:" crates/`, so the design.md sentence is not its own fourteenth hit; (2) markers name the **sort key**, mandatory at `ffi_decls.rs` and `cycles.rs`, where span order under-determines the output; (3) `names_in`'s single marker names artifact classes, never a consumer count; (4) a golden pinning the tie-break — today **no golden exercises it** |
| **ffi-pragmatist** | approve-with-conditions | design.md §1.11/§4.19 (verified untouched); §4.9; Part 5 step 17 | Zero at the boundary: zero runtime lines, `HERO_RUNTIME_ABI` stays 14, no marked site in `runtime/`. Live double-emit over a probe exercising all three emit sites: diff empty, full build runs. **Finding: the double-emit test pins *stability*, not *order*** — both emissions come from the same binary, so a deterministic reorder passes it. The order itself is pinned only by `golden_emit_cases_produce_their_c`, and its five cases pin `_desc` order **but neither the per-option function order nor the `fn{N}` typedef numbering — today those two orders have no checked-in pin at all**; `typedefs_generated.rs:71-72`'s comment overstates what the determinism test covers, §11's expiring-premise shape. The `fn{N}` suffix is the walk position, so an unsorted walk **renames symbols file-wide** | At **M-selfhost-port**, the first ported emitter file that walks a Heroes map without `sort` fails a *golden emit* diff naming the case, before **M-selfhost-fixpoint** ever runs — falsified if the first ordering defect instead surfaces as a raw B.c/C.c diff, which would mean the golden net was not re-pointed at the ported binary | (1) the note must not say "the fixpoint diff breaks" uniformly over all 13: emitted-C walks break the fixpoint diff; diagnostic/dump walks break the golden diff **only when the harness runs under the ported binary**, which M-selfhost-port must therefore do; (2) one `tests/golden/emit` case with ≥2 `T?` options and two function-typed parameter shapes, and the overstating comment corrected to cite it; (3) no bare count "13" in prose — omit it or give it a test |

## The paragraph the port author will rely on (ffi-pragmatist, adopted verbatim)

The fixpoint compares B.c (produced by A, the Rust bootstrap, whose every walk is
`BTreeMap` — *sorted* order by construction) against C.c (produced by B, a Heroes
binary, whose maps iterate in fixed-seed *hash* order). Both producers are
internally deterministic; determinism is not what breaks. What breaks is that the
two compared files come from two implementations whose deterministic orders are
**different functions of the same data** — so an unsorted walk whose order
reaches the emitted C makes `diff B.c C.c` non-empty wherever hash order departs
from sorted order. Two precisions. First, this is not guaranteed per-site: a
small key set can hash into sorted order by accident, which is why the rule must
be a `sort` at the write site, never a test that the orders agree. Second, the
A-vs-B seam is the **only** place the fixpoint can see it — once B.c = C.c, all
later generations are identical binaries. And the fixpoint diff sees **only**
walks whose order reaches the port's own emitted C: the diagnostic/dump walks
produce artifacts the fixpoint never compares. Their net is the goldens re-run
under the ported binary — M-selfhost-port already names the goldens as the net,
and the harness currently hardcodes `CARGO_BIN_EXE_heroes`, so re-pointing it is
part of that milestone's work, now stated rather than implied.

## Disagreement, stated plainly

**The dated count.** The compiler-engineer holds that "snapshot 2026-08-15
(panel 065): thirteen" is admissible — it claims only what was measured on a
date, in §14's measurement vocabulary, and it does real work on the record: it
refutes "exactly one place" with the scale of the falseness. The ffi-pragmatist
holds the stricter line: no count in prose at all unless a test fires when it
diverges (CLAUDE.md §7's dead-eleven precedent). The resolution keeps the dated,
panel-cited snapshot **and makes the grep the only live inventory**; if the
author sides with the pragmatist at ratification, the strike is one
parenthetical. Both judges agree the *live* claim must be the marks.

## The resolution — provisional, author ratification pending

All seven conditions adopted; the most conservative reading where they touch:

1. design.md §4.9's port note rewritten: discipline + sort-key-bearing marks +
   scoped grep as the live inventory + the count only as a dated snapshot + the
   **two break classes stated separately** (fixpoint diff for emitted-C walks;
   golden diff under the ported binary for diagnostic/dump walks).
2. The 13 `// ORDER:` markers land at the write sites, each naming the sort key
   and the observer. One marker at `names_in` (resolved.rs:148) covers its six
   consumers: the write site is the only line that touches the map, the port's
   `names_in` owns the one sort, and six consumer-side marks would claim six
   sorts where one is owed.
3. `typedefs_generated.rs:71-72`'s comment corrected: the determinism test pins
   stability; the emit golden pins the order.
4. Two new goldens: `tests/golden/check/` — the nested-fixed-array tie-break
   (two diagnostics at one position, inner first); `tests/golden/emit/` — a case
   with two `T?` option types and two function-typed parameter shapes, pinning
   the per-option function order and the `fn{N}` numbering for the first time.
5. Nothing else moves: no code change, no spec token, no runtime line, no ABI.

**What a veto at ratification would compel**: reverting the design.md sentence to
its (false) previous text is not on the table — §11 makes a demonstrably false
normative sentence a defect. A veto could only reshape the wording or strike the
marks; the falseness of the OLD text stands measured either way.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | ported `fixed_only_in_a_group` passes the tie-break golden only by sorting on the map key; a span-only sort swaps the pair at 2:8 | M-selfhost-port |
| ffi-pragmatist | the first ordering defect in the port surfaces as a golden-emit diff naming its case, never as a raw B.c/C.c fixpoint diff | M-selfhost-port, scored no later than M-selfhost-fixpoint |

## Process notes

Both judges built in copies with `target`/`build` removed (panel 056's rule);
the working tree was frozen from briefs out to this synthesis and shows no
edits. The ffi-pragmatist reported `HERO_RUNTIME_ABI` at **14** — CLAUDE.md §7's
"kept the number at 10" is panel 037's dated record, not a live claim, and needs
no correction. The sweep that fed the briefs was itself wrong once before it
was verified: the SCHEDULED item (verified 2026-08-12) said `types/holes.rs`
walks only vectors, and `holes.rs:171` walks `resolved.top` today — the fact
this panel exists to stop prose from asserting had already rotted inside the
item that scheduled the panel.
