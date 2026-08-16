# Panel 081 — the two silences, and what was under one of them

**Date**: 2026-08-16 · **Session type**: full panel, five judges, five briefs ·
**Status**: `provisional — author ratification pending`

**Numbered 081 and briefed as 080.** While this sitting's judges were measuring,
another session landed `docs/panel/080` (`heroes this`, author instruction
*"inseriscila nel linguaggio allora"*, commits `f2c0dc9` and `3b9ef97`). The
number was taken from under a sitting in flight; the briefs say 080 and this file
says 081, and the record says why rather than being tidied. Two of five judges
independently reported the working tree as unfrozen during their runs, which is
how it was caught — **the freeze rule works, and it now has a failure mode nobody
had listed: a second session with the same author's credentials.**

## The proposal, verbatim

Two clauses, offered together because they looked like the same rule seen twice.
Both were found by an llm-ergonomist seat reading only the spec (panels 076, 077).

**Clause A** — `spec:177-179`:

```diff
-exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`.
+exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`; a `bool` prints `true` or `false`.
```

**Clause B** — `spec:219-221`:

```diff
-array of one: `i32[4]`, never a `[T]`.
+array of one: `i32[4]`, written as an array literal but never a `[T]`.
```

Measured with `heroes measure`, cl100k_base binding, re-run by three seats and
agreeing to the token: baseline **3506**, +A **3518** (+12), +B **3512** (+6),
both **3524** (+18). Ceiling 4096, headroom 590 → 572.

## Verdict table

| Judge | Verdict | Rests on | Cost / delta | Prediction | Condition |
|---|---|---|---|---|---|
| **llm-ergonomist** (blind A/B, two variants, label-stripped) | **approve** adding · **object** to the status quo | §1.4 | — | ≥25% of beta readers produce stdout that is not `true\n`; 100% of those compile cleanly | Withdraws if fresh readers converge on `true` anyway, **or** if `print` on a `bool` were a compile error — in which case it would argue for a *third* variant saying so |
| **compiler-engineer** | **object** — A approve, B object | §1.7, §1.6, §1.4, CLAUDE.md §7 | **zero compiler lines** both; 3506/3518/3512/3524 confirmed | B: at M-ffi-ladder close the three broken shapes are still exit 2, **or** `storageless.rs`+`inst.rs` ≥ 434 lines (today 374) | B → approve when the shapes compile with a golden each, **or** at a wording true of the whole surface |
| **spec-warden** | **object** on the package | §1.6, §1.4, panel 035 R4 | found and measured a **−10** removal (`spec:177`) → A at net **+2** | removal+A reads exactly 3508; `harness/tasks/README.md` says **0 tasks**, so no reader prediction is admissible payment | A → approve with the removal riding in the same commit. **B → veto** if adopted while `reserved: a.reserved` is exit 2 |
| **ffi-pragmatist** | **approve both** | §1.11, §4.19, §1.12 | bound `termios`, `sqlite3_snapshot`, raylib `VrDeviceInfo` for real, all exit 0 | a `char[N]` golden exits 1 today on all eight widths and exits 0 printing `68` after its repair | The synthesis must **not** adopt B on the brief's "the wrong guess is loud" reasoning — that premise is false |
| **historian** | **approve** A as written · approve B's **intent**, object to its **wording** | §1.1, §1.4, §1.2 | documentary | three-arm reader study: prose alone leaves a wrong-delimiter attempt that *showing* the literal does not | For B: a documented case where a permissive **sentence** without the **form** ended an elimination misreading. Found none |

## Disagreements, unsmoothed

**The two clauses are not one rule, and four seats said so from four directions.**
The brief offered them as a pair; every seat split them. A is a **silence over a
mistake that compiles**; B is a sentence about a surface. §1.4 funds the first and
panel 035 R4 refuses the second — that is the general rule for this class, and it
already existed.

**The brief's own reasoning on B was falsified, by the seat that approved B.** The
brief argued B is cheap because its wrong guess is *loud* (three measured
diagnostics). The ffi-pragmatist showed the wrong guess panel 077 actually made
was *"therefore `partial`"* — and that program **exits 0** with fields silently
absent. So B's misreading is **silent**, and if B is ever adopted it must be
adopted for that reason. Adopting it on the brief's reasoning would read §1.4
backwards for the next clause that deserves it.

**The historian's sharpest claim was falsified by the coordinator.** It inferred
that the `partial` route hands C uninitialised memory and asked for one `heroes
run` to settle it. Run: `record Widget partial` naming two of three fields
compiles, runs at exit 0, and C reads **0** from the undeclared `reserved` even
with the stack deliberately dirtied and under `--sanitize`. The mechanism, checked
rather than the outcome: the emitter writes `t15 = (Widget){.x = t13, .y = t14};`,
a C11 compound literal, and **§6.7.9p21 initialises the unnamed members as if
static** — zero, guaranteed by the standard rather than by luck. The `partial`
route is **memory-safe**. It is *semantically* silent, which is the
ffi-pragmatist's separate and surviving point.

## What the sitting actually found: the surface under clause B

Clause B was offered as a phrasing question. Three seats went and used the
surface, and it is broken in **four positions**. Complete inventory, every row run
by the coordinator against a real header this session:

| what you write | `heroes check` | build / run |
|---|---|---|
| `a.reserved[0]` — read by index | 0 | **0, works** |
| `r = a.reserved` — bind it to a name | **0** | **exit 2**, `use of undeclared identifier 'h1_r'` |
| `reserved: a.reserved` — copy it into another record | **0** | **exit 2**, `incompatible pointer to integer conversion` |
| `w.reserved @ [...]` — store into it | **0** | **exit 2**, `use of undeclared identifier 't21'` |
| `[a.reserved]` — put it in an array | **0** | **exit 134**, `hero_unreachable` |
| `print(a.reserved)` | 1 | correctly refused |

Three found by the compiler-engineer, two of those reproduced independently by the
spec-warden, the fourth (`[a.reserved]`) by the coordinator while attacking the
adjacent shapes. **Every one is `heroes check` exit 0.** CLAUDE.md §7 says exit 2
means the compiler is wrong; here the compiler says so about a program the author
wrote and cannot see the fault in.

Cause, one for all four: a `Ty::Fixed` has no C storage, so `emit/storageless.rs`
renders it inline where it is used. Its own doc names *"two producers … a literal
and a field read"* — and the field read only survives in the one position the
emitter reaches. `types/ffi_decls.rs`'s walk reads `written_types`, and all four
broken shapes carry an **inferred** fixed type that no `TypeId` node spells, so
the walk cannot see them. CLAUDE.md §11's shape: a narrowing resting on what was
written rather than on the value in hand.

**And one rung lower, the ffi-pragmatist found the hole that matters most.**
`char[N]` cannot be declared at **any** of Heroes' eight widths, because
`emit/extern_field.rs` asks class+width+sign for a scalar `Ty::Int` (panel 064)
and `_Generic` **type identity** for a `Ty::Fixed` — and C's `char` is a third
type distinct from `signed char` and `unsigned char`. From a clang AST walk over
eleven POSIX headers plus raylib, SDL3, sqlite3, zlib and curl: **26 of 81
fixed-array struct fields (32%) are `char[N]`**, and `uname()` is unreachable at
any price. §1.12: *complete, because a library Heroes cannot bind is a library the
author must leave C code around for.* The repair is one assertion, nine cases,
three real headers, clang exit 0 — built and compiled by that seat.

## Resolution — provisional, author ratification pending

The most conservative reading of five verdicts:

**R1. Clause A is adopted**, at +12 against the spec-warden's measured **−10**
removal, net **+2** → 3508. It is *exactly* true: the compiler-engineer attacked
eleven rendering shapes (direct, `to_str`, both `assert` sides, UFCS, through a
generic, concatenation, map value, array element, and a C `_Bool` memset to 7) and
found no shape in this language where a `bool` prints `1`; 27 of 81 `run/`
goldens already pin it. The blind seat got `true` from **no sentence** at
confidence 5/10 and nearly wrote `1`. §1.4 exactly: silent, and it compiles.

**R2. Clause B is NOT adopted, and is not refused either — it is unripe.** One
veto stands against adopting it while the surface it describes crashes the
compiler in four positions. The clause returns when R3 lands, and its **wording is
chosen after the repair rather than before**, because two seats showed the offered
wording is true only of the corner that survives.

**R3. The four shapes are the repair this sitting owes**, and they rank above both
clauses: exit 2 and exit 134 on `heroes check`-clean programs is §1.12 and
CLAUDE.md §7, not a documentation question. Conservative default: **refuse them at
exit 1 on the author's line**, naming the spelling that works. Lowering them
properly is the widening, is not compiler-need (`selfhost/` has zero fixed-array
fields, measured by two seats), and is available at a price nobody has taken.

**R4. `char[N]` is queued with its repair already built**, not folded in here: it
is a §1.11/§4.19 completeness defect rather than a spec-wording one, and the
ffi-pragmatist filed it rather than trading it for its approval.

**R5. The general rule for this class is panel 035 R4, refined and now stated:** a
silence is bought only where there is no mistake to be loud about — **and
"loud" means exit 1 with a message the author can act on.** Loud at **exit 2 with
the compiler's own name on it** is not a covered mistake, it is a defect, and it
is paid in compiler lines and never in spec tokens. That third case is what these
two clauses exposed and what R4 had never had to consider.

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| llm-ergonomist | ≥25% of status-quo readers produce stdout that is not `true\n`; the A−B delta is entirely in the **silent** column, zero in the loud column | metric 2, which `harness/tasks/README.md` says has **0 tasks** — so registered as an observation, paying nothing (panel 046 R1) |
| compiler-engineer | at M-ffi-ladder close the three shapes are still exit 2, **or** `storageless.rs`+`inst.rs` ≥ 434 lines (374 today) | M-ffi-ladder close |
| spec-warden | removal+A reads exactly **3508** cl100k / 3435 legacy, `cargo test` green, `print-contract.expected` unchanged | this sitting's commit |
| spec-warden | a golden with `reserved: a.reserved` is **exit 2** today and must read exit 0 or exit 1 by M-selfhost-port close | M-selfhost-port close |
| ffi-pragmatist | a `char[N]` golden (`utsname.sysname`) is exit 1 on all eight widths today; after the built repair it exits 0 printing `68` with every existing ffi golden green | whenever R4 lands |
| historian | three-arm study: prose alone leaves a wrong-delimiter attempt (`{0,0,0,0}`) that showing the literal does not | metric 2 |

**And one finding that outranks the sitting.** Panel 024's historian found no
study varying a specification's length against correctness of programs written in
it. Still true for *specifications* — but four studies since vary **specification
detail in context** against **code correctness** (arXiv 2508.03678, 2604.24712,
2602.15228, 2510.26130, all verified this session). Their joint result is that
detail-vs-correctness is **non-monotone** and that *structural redundancy* is what
buys robustness. That is design.md §1.4 arriving from outside this project, and it
says the token count is not the quantity that predicts the objective. It does not
move the 4096 ceiling; it belongs on `DECIDE.md`.

## Process notes

Two seats reported the tree unfrozen; the coordinator confirmed the commits were
the author's own from a parallel session and **reverted nothing**. The near-miss
is on the record because the reflex was to revert: an unrecognised commit inside a
frozen window looks exactly like a judge that ignored its brief, and the only
thing that separated them was `git log --format=%an` and reading the panel file
the commit carried.
