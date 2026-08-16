# Panel 082 — the note that told the author not to try

**Date**: 2026-08-16 · **Session type**: full panel, five judges, five briefs ·
**Status**: `provisional — author ratification pending`

**Numbered 082 and briefed as 081**, for the reason `docs/panel/081` records: a
parallel session took 080 while both sittings were in flight.

## The brief was wrong, and a judge measured it rather than arguing with it

The coordinator's brief stated, as the sitting's central fact, that `sort` inside
a generic **cannot** be checked by the frontend: *"the instantiation … is visible
only after monomorphisation, which runs in the IR, and `heroes check` is parse →
resolve → `types::check` and stops. **So** there is no third place to put it that
`check` reaches."*

That is false, and it is false in CLAUDE.md §1's exact named shape — **an
inference presented as a measurement**, with the tell in the connective. The
pipeline was read; the conclusion was not run.

`crates/heroes/src/types/apply.rs:139-143` writes
`checked.instantiations.insert(span.start, resolved_args)` **inside
`types::check`**. The compiler-engineer ran parse → resolve → check on the
brief's own program, with no IR anywhere, and printed the table: `9:9 -> [P]` —
the checker already knows `A = P`, **keyed by the call-site span, which is the one
line the author can edit**. For a generic calling a generic, both edges are
present. The fourth place exists, and it has precedent beside it in the same
function: `map_keys::check` and `ffi_decls::fixed_only_in_a_group` are already
file-wide passes run after `decls::file`.

Worse for the brief: **option (a) as written cannot catch its own example.**
`mono::run` pushes exactly one diagnostic class — polymorphic recursion. The
`unsupported[builtin]` row is `emit/gate.rs:129`, reached only after `own::run`.

Two shipping module docs carry the same false claim and outlive this sitting
unless it says so: `crates/heroes/src/types/ordering.rs:70-76` and
`crates/heroes/src/emit/builtins.rs:156-162`.

## Verdict table

| Judge | Verdict | Rests on | Cost / delta | Prediction | Condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **veto (a)** · approve **(b)** and **(d)** · **object (c)** | §1.1, §1.7 | (a) **57 lines**, `check.rs` 263→308 which **fails `layout.rs:105`**, **2.2×** wall clock on `selfhost/`; (b) **~90–110 lines**, all in `types/`; (d) **50/−16 across 4 files** | if (b)+(d) lands: ceiling test passes, diagnostic is `unordered_element` **at the call**, `g2.hero:9:9`, not `5:10` | a program where `build` reports the row and `Checked::instantiations` holds no concrete unordered argument. Named what it searched: direct calls, UFCS, function values, generics through generics, nested containers |
| **ffi-pragmatist** | **object** to (d)'s **scoping** · approve (a) | §1.11, §4.19, §1.12 | (a) **29 lines**, 3 files, **0 in `runtime/`**, ABI unmoved at 14, **1.29×**, `check` stays clang-free | a `const`-qualified pointer member golden is exit 2 today and stays exit 2 under every option, because none touches `emit/ffi_field.rs` | (d) must cover `pointer_element`, not only `builtin`; if (a) lands, the ruling must state the §4.16 hole and say *check accepts ⇒ **build** succeeds*, never *⇒ run succeeds* |
| **llm-ergonomist** (spec only) | **veto** | locality | — | ≥70% write `sort(xs)` in the generic body; Task 2 Q2 splits near evenly on which line the error names; ≤20% believe the note | lifts on one added sentence, either direction: refuse the construct early, **or** admit constraints so the signature carries the fact |
| **spec-warden** | **object** on the accounting | §1.6, §1.0, §12 | the briefed "zero spec tokens" is **false**: `spec:173` said `sort` takes *"a number or `str`"* while `bool` had been ordered since 06:06 | the honest floor is **+3/+4**, and `grep -rnE '^function [a-z_]+<' selfhost/` returns zero at M-selfhost-port close | the `bool` clause lands, or (c) is a decision to leave a false sentence in the prompt |
| **historian** | approve **(c)+(d)**, with a reservation against justifying (c) by Rust | CLAUDE.md §8, §12, §1 | documentary | the false note is **already plural** — `grep` returns more than one row, at least one likewise value-dependent | flips to (a) if `check` with mono measures under ~10% slower — because then RFC 3477's cost premise, (c)'s only historical cover, is falsified at this scale |

## Disagreements, unsmoothed — and the one the coordinator settled by measuring

**The two seats that compile disagree about (a)'s price by a factor of two**: 57
lines and 2.2× against 29 lines and 1.29×. They built different things — the
compiler-engineer exposed `gate::refuse` and hit the file-size ceiling; the
ffi-pragmatist wired lower→mono→gate more narrowly. Both numbers are real and
neither was taken on trust. The disagreement does not need settling, because both
prices lose to (b) on the compiler-engineer's measurement and to the §4.16 hole
both of them found independently.

**They also disagree about how many rows the false note reaches, and that one is
decidable.** The compiler-engineer built `[ptr]` and reported `pointer_element`
keeps the note *"verbatim and correctly"*. The ffi-pragmatist built the same thing
against real SQLite 3.51.0 and reported the opposite. **Measured by the
coordinator, and the ffi-pragmatist is right**: `stmts: [ptr] @ []` is
`unsupported[pointer_element]` with *"no change to this file will fix this"* — and
the same file with `record Stmt { p: ptr, sql: str }` and `stmts: [Stmt]` runs at
**exit 0**. A change to the file fixes it. The note is false on the FFI's own row,
which is §4.19 ladder step 3's actual shape — a collection of SQLite statement
handles — and therefore the row an author reaches before they ever reach `sort`
in a generic.

**Two vetoes point in opposite directions.** The compiler-engineer vetoes (a) on
the ceiling. The llm-ergonomist vetoes the **status quo** on locality: nothing on
the line `sort(xs)` and nothing in `function smallest<T>(xs: [T]) -> T` decides
whether that line is legal, because the answer lives at call sites in other
functions and possibly other modules. Its own words, and they are the sharpest
sentence of the sitting: *"'fixing' the reported line means deleting the
function."* So (c) — leave it alone — is vetoed too, and the sitting has no
do-nothing option.

## What is measured and not in dispute

- `first([3, 1, 2])` runs and prints 1. Refusing the generic **body** deletes a
  working program; no seat proposed it.
- `heroes check g2.hero` exits **0**; `heroes build` exits **1** and its note is
  false — changing the file to `first([2, 1])` builds and runs.
- **Not compiler-need, three seats independently**: `selfhost/` declares **zero**
  generic functions, its `sort` calls are all `sort(keys(m))` at `str`, and zero
  of the thirteen `// ORDER:` marks sits in generic code.
- The `sort.c` nan guards are **still needed**: the ffi-pragmatist deleted both,
  rebuilt, and fed it a nan libm produced (`sqrt(-1.0)`) — **four different
  answers at exit 0**, one of them not even sorted among the non-nans. Neither (a)
  nor (b) makes them redundant, and both make them *harder to defend*, because
  *check accepts ⇒ build succeeds* will be misread as *⇒ run is right*.
- **`heroes check` stays clang-free** under (a): a binding to a header that is not
  installed still exits 0. That was the ffi seat's stated fear and it does not
  happen.
- **Option (a) breaks §4.16**, found independently by both compiling seats:
  `gate_ops.rs:81` turns `???` into `unsupported[hole]`, so every program with a
  hole becomes exit 1 against `spec:192-194`.

## Resolution — provisional, author ratification pending

**R1. (d) lands now, widened to the rule rather than the row.** The note moves off
the `Diagnostic::unsupported` **constructor** and becomes a per-row decision.
Measured scope: 7 note sites over 5 codes; `unsupported_operand` and
`pointer_element` are the two that lie; the others keep the note verbatim and
correctly. Shipping (d) for `sort` alone would be *"a repair without its adjacent
shapes"* — CLAUDE.md §1, by name, and the FFI's row is the one authors hit first.

**R2. (a) is refused**, on the compiler-engineer's veto and on evidence neither
brief nor coordinator had: it costs the file-size ceiling, 1.3–2.2× on every
keystroke, `--permissive` contamination of Part 11's control arm, a
`heroes mutate`/`heroes check` split scoring two different languages, and the
§4.16 hole. Zig's `-fno-emit-bin` is (a) shipping and remains the standing
counter-precedent; it is not refused forever.

**R3. (b) is the direction, and it is queued rather than landed here.** It is the
only option that answers both vetoes' substance: the diagnostic becomes
`unordered_element` at exit 1 **on the call the author can edit**, which is the
line the blind seat said it needed and the line the IR route provably cannot
produce (`mono_subst.rs` copies the template's span; `mono.rs` holds the call span
at line 68 and discards it at 95). Priced at ~90–110 lines in `types/`, bounded
and not built. Not compiler-need, so Principle 0 puts it behind the port.

**R4. (c) is not available.** It carried a veto of its own, and its stated
ground — the brief's — is false. Whatever else is decided, the two module-doc
paragraphs asserting it are wrong today and are corrected with this sitting.

**R5. Two defects are filed rather than traded**, both found by seats answering a
different question: `const`-qualified pointer members (`check` 0, `run` exit 2;
**39 of sqlite3.h's 158 pointer members** are `const`-qualified) — a fifth class
under CLAUDE.md §7's narrowing rule, which `emit/ffi_field.rs` never sees; and
`unsupported[pointer_element]`'s message appending *"is not emitted yet"* to a
phrase that already has a clause.

**R6. The invariant this sitting is really about, stated so it cannot drift**:
*check accepts ⇒ **build** succeeds*. Never *⇒ run succeeds* — `sqrt(-1.0)` is a
legal `f64` that no static pass can see, and `sort.c:51,57` is the standing
counterexample.

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | (b)+(d): ceiling test green, no `types/` file over 300, diagnostic is `unordered_element` at `9:9` | whenever (b) lands |
| compiler-engineer | (a): `layout.rs:105` fails naming `check.rs`, and `heroes check selfhost/lexer.hero` ≥1.8× | only if (a) is revived |
| ffi-pragmatist | `fixedbugs-a-const-qualified-pointer-member.hero` is exit 2 today and after this sitting under every option | M-selfhost-port close |
| ffi-pragmatist | SQLite §4.19 ladder step 3 needs no shim under (a) — measured exit 0 today | M-ffi-ladder |
| historian | the false note is already plural: `grep` returns more than one row, at least one value-dependent | **scored NOW, held**: two rows, `unsupported_operand` and `pointer_element`, the second measured value-dependent by the coordinator |
| llm-ergonomist | ≥70% write `sort(xs)` in the generic body; Q2 splits near evenly; ≤20% believe the note | metric 2 (0 tasks — registered as an observation, paying nothing) |
| spec-warden | `grep -rnE '^function [a-z_]+<' selfhost/` returns zero at M-selfhost-port close | M-selfhost-port close |

## What a veto would compel

The llm-ergonomist's veto is the live one, and it is not answered by (b): (b)
moves the *error* to the call site but does not make the generic body's line
decidable from the line plus its signature. Answering it needs **constraints on
generics** — `function smallest<T: ordered>(…)` — which is a language addition
under Principle 0 with no compiler-need behind it, and which the historian priced
from outside: Swift and Go bought the checker-side guarantee with a **closed
constraint vocabulary**, C++ took until C++20 and Stroustrup names the surviving
obstacle as *legacy migration* (which Heroes does not have), and Nim's Araq opened
the same RFC in 2019 and it is still open. That is the trade the author owns, and
this sitting does not spend it.
