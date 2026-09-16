# Panel 157 — the artifact of a build that never happened

2026-09-16, at M-check-completeness, on **defect 048**. **Soundness lane**:
compiler-engineer and ffi-pragmatist. **Resolution provisional — ratified the
same day by the author's standing delegation, recorded at the foot.**

The sitting was convened on an artifact bug: `heroes build --emit-c` writes C
that clang refuses, for any `record <Name> tag <name>` binding. It found a second
half nobody had asked about, and that half is a **§1.12 memory corruption** the
repository already ships five cases of.

## The lane, and what it gave up

`--emit-c` is a tool surface with **no spec token** — the specification is the
language and does not mention the flag — and no diagnostic class of its own. So
the llm-ergonomist would have read a document that is silent here and the
spec-warden measured a delta of zero. **What the lane gives up**: no verdict on
how a reader experiences the artifact, and no precedent. Neither seat's absence
is felt in what follows, but the sitting says so rather than leaving a reader to
assume five seats sat.

## The question that decides the option set, SETTLED

Four header shapes, each compiled twice under the real flag list from
`selfhost/cli/flags.hero`, by the ffi-pragmatist:

| header | `struct node *` | `node *` |
|---|---|---|
| A `typedef struct { int64_t v; } node;` (ANONYMOUS) | **error** | ok |
| B `typedef struct node node;` | ok | ok |
| C `struct node { int64_t v; };` | ok | **error** |
| D `typedef struct node_s { … } node;` | **error** | ok |

Shape A under `struct`: *incompatible pointer types initializing `struct node *`
with an expression of type `node *`*. Shape C bare: *must use 'struct' tag to
refer to type 'node'*.

**Shape A is real and bindable today** — the seat wrote the header and the
binding, built it (exit 0, prints `7`) and compiled the `--emit-c` artifact
(exit 0, prints `7`). So **no fixed spelling can be correct and the round is
unavoidable.**

The compiler-engineer independently killed the one clang-free route the brief had
offered: a typedef shim at the top of the artifact **fails on shape A** with
*typedef redefinition with different types*. It is always-qualify wearing a hat.
**Clang is the only oracle**, and the only clang-free alternative left is a C
declaration parser inside Heroes — which is what that seat's veto is aimed at,
held and not cast.

Two corrections the sitting owes its own record. A **declaration** proves nothing:
shape A accepts `struct node *p;` at exit 0 under default flags, so panel 152 was
right and was about declarations; the round survives because it compiles **uses**,
and only because `-Werror=incompatible-pointer-types` is in the flag list. And the
round does **not** fire needlessly on shape B, because clang accepts `node *`
there and there is no refusal to read.

## THE SILENT HALF, which no question asked

`selfhost/cli/pointee.hero` has exactly one call site,
`selfhost/cli/assemble.hero:62`, **inside the round**. `cli/compile.hero:266`
returns before it. So `--emit-c` never runs the pointee check either.

```
extern "widen.h"                 # static inline void fill(uint64_t *n);
    function fill(@n: i32)       # eight bytes into a four-byte slot
```

| | |
|---|---|
| `build` | exit 1, `error[ffi_parameter_type]` |
| `--emit-c` | exit 0 |
| clang on that artifact, full flag list | **exit 0, 0 errors, 0 warnings** |
| running it | printed `2863311530` — `0xAAAAAAAA` written over the `guard: i64 @ 123456` in the adjacent stack slot |

**That is design.md §1.12 verbatim**, and the mechanism is bitter: the emitted
line is `fill((void *)&h0_n);`, and `cli/pointee.hero`'s own doc says that cast
exists so a **correct** binding compiles clean under
`-Werror=incompatible-pointer-types`. Under `--emit-c` the same cast launders a
**wrong** one past the only flag that would have seen it.

**And it is already in the repository.** Of 32 `tests/golden/fixedbugs/` cases,
**25** emit at exit 0 while `build` refuses them. Compiled, **20 are loud and 5
are silent**: `ffi-missing-link`, `ffi-pointee-opaque`, `ffi-pointee-sign`,
`ffi-pointee-void`, `ffi-pointee-width`.

**So R1's framing was too weak.** *"Emit C that clang accepts"* is provably
insufficient — five committed cases satisfy it and are still unsound.

## And the instrument gap is larger than the defect

The compiler-engineer measured the blessed set: of **240** emissions, **28** fail
`-fsyntax-only` and **3** fail with *must use 'struct' tag* — all three `run/`
programs that are **green today**:
`tests/emission/run-fixedbugs-a-tag-that-needs-struct.c`,
`run-fixedbugs-a-tag-names-a-handle-and-a-record.c`,
`run-fixedbugs-getaddrinfo-is-bindable.c`. **Defect 048 is already committed and
blessed**, because `emission` compares bytes and a byte comparison cannot see
that the bytes do not compile.

## The resolution adopted

**R1. `--emit-c` owes the artifact of a build that passed** — not merely C that
clang accepts. The five silent cases are why the weaker wording is refused. What
settles it is not prose: `--emit-c` is how `seed/heroes.c` is made
(`seed/README.md:85`) and CI re-emits and compares it on every push
(`.github/workflows/ci.yml:692`), so the flag is load-bearing for the bootstrap.
The compiler seat notes, correctly, that **design.md:717 is ambiguous** — *"stop
at the C and read it"* reads as either an output or a dump — and that the
unambiguous sentence is `.claude/rules/cli-surface.md`'s *"an output, not a
dump"*. The shared brief overstated their agreement and this records the
correction.

**R2. Two changes, different in kind, and the difference is the whole design.**

- **The pointee check runs on the `--emit-c` path**, and it is a REFUSAL: a
  program `build` rejects gets no artifact. It is the compiler's own diagnostic,
  already written, and it moves to where both paths reach it. This is the §1.12
  half and it outranks everything else in this sitting (CLAUDE.md § Precedence
  rank 3).
- **The tag round becomes an ADVISORY probe**, gated on the program declaring a
  tagged handle: emit, `-fsyntax-only`, harvest `needs_struct`, **discard the
  verdict**, re-emit, write. It can never turn a success into a failure, and on a
  machine lacking the header it degrades to today's output. Measured at
  `real 0.74` on the 820,861-line seed, and zero when the gate is shut.

**Rejected: `--emit-c` exits 2 when a tagged binding is present.** It is the
cheapest option and it removes the capability on exactly the day the flag must
work or there is no seed.

**R3. What happens to `suite_emission.hero`'s fourteen.** The tag half keeps
emitting, because the probe's verdict is discarded. The five pointee cases stop
emitting, **correctly** — they are programs the build refuses for a §1.12 reason,
and an artifact for them is the defect. Either way the suite's stated reason at
`suite_emission.hero:70` — *"`--emit-c` never calls clang"* — becomes **false**
and must be rewritten rather than left standing: the premise's substance survives
for the tag half and its sentence does not. Re-blessing uses `UPDATE_EMISSION=1`
(`:128`, `:263`), never the forbidden `UPDATE_GOLDEN=1`.

**R4. The instrument, and it lands whichever way R1 had gone.** Compiling the
artifact catches 20 of the 25 and none of the five. The assertion that catches
the five is the ffi seat's: **the same program built both ways must give the same
exit code and the same stdout, and it must LINK and not merely compile** —
without the link, `ffi-missing-link` stays invisible. Its home is `emission`.

## Implementation cost, measured

Under **40 net compiler lines**, all in `selfhost/cli/`: `compile.hero` −10 (the
`--emit-c` write and halt move out of it), `produce.hero` +25 to +35 (the gated
probe), `assemble.hero` and `emit/ffi_tag.hero` **0** — the build's round is
untouched and `needs_struct` is reused verbatim. `suite_emission.hero` +20 to
+30 for R4's leg. No lexer, checker, descriptor, ownership or IR change.

## Predictions to score

| seat | prediction | scored by |
|---|---|---|
| compiler-engineer | the compiler grows **<40 net lines**, all under `selfhost/cli/`; `emit/ffi_tag.hero` still 311 lines | `git diff --stat` at the repair's close |
| compiler-engineer | **exactly 3** files in `tests/emission/` change bytes, the three named; the case count does not fall | `emission` after re-blessing |
| compiler-engineer | `heroes build selfhost/main.hero --emit-c` stays within 1 s of pre-repair | `/usr/bin/time -p` |
| ffi-pragmatist | SQLite is shape B: the round runs once and `struct_tags` stays empty, so §4.19's step 3 needs no shim | `corpus` |
| ffi-pragmatist | raylib's `Vector2` is shape A and becomes **unbindable** under a fixed-`struct` rule | measured already; scores the refused option |

## What a veto would compel

The compiler seat's veto is held and aimed at exactly one thing: **a C
declaration parser inside Heroes** to learn tags without clang. It breaches §1.7
and §1.1 and would be cast on sight. The ffi seat moves to veto if the resolution
is *always write `struct X *`*, which shape A measures as making raylib
unbindable. It withdraws its R1 objection only if a `pointee.check` call site
exists on the `--emit-c` path that it missed — it measured one hit.

## Author's verdict

**RATIFIED 2026-09-16, as adopted — BY DELEGATION AND NOT BY READING**, under the
author's standing instruction of that day: *continue until the step is finished
with zero defects and zero open decisions — ratify the panels for me — and at the
end push everything and check that CI is green on all three systems.* The yes is
the assistant's judgement under an authority the author handed over, on a sitting
the author has not read; recording it otherwise would credit them with a reading
that did not happen.

**What the yes settles.** That `--emit-c` owes the artifact of a build that
passed, and that the bar is not *clang accepts it*. That the tag probe is
advisory and the pointee check is a refusal. That no fixed spelling is correct,
measured on four header shapes. That `emission` gains a leg which builds both
ways and compares exit code and stdout, and links.

**What it does NOT settle, and this is the honest half.** **The repair is not
built.** This sitting priced it and did not land it, and defect 048 is widened
rather than closed: it was filed as an artifact bug and it is now also a §1.12
corruption with five committed witnesses. That is a milestone's work and not a
step's, and it is recorded as such rather than rushed at the end of one.
