# Panel 221: a record constant is a static object, so C asks that its value be constant, and a call or an object is refused as the scalar is

**Convened 2026-10-17 from 12:00, synthesised the same day from 13:25**, on defect 2116
(https://github.com/heroes-lang/heroes/issues/2116, `blocking`), which reaches the hole panel 063's
condition 1 wrote down as a known cost. **Seats**: compiler-engineer and ffi-pragmatist. No route
adopted adds surface, a diagnostic class or a spec token. **Resolution: provisional, author
ratification pending.** Brief: `docs/panel/221-briefs/00-shared.md`. Reports:
`docs/panel/221-reports/compiler-engineer.md`, `docs/panel/221-reports/ffi-pragmatist.md`.

## The proposal, verbatim (the shared brief's question)

> A group record constant whose header value reads something that is not constant builds at exit
> 0, and two reads of it differ, where the same read as a scalar constant is refused
> `ffi_not_constant`. Panel 063 left the question unasked of a struct, measured:
> `__builtin_constant_p` answers 0 for every struct, so asking it would refuse raylib's 26
> `CLITERAL(Color)` macros. Does the compiler now refuse such a record, and how, without refusing a
> record C builds from constants?

The dump route (K) was the coordinator's; S, the static local, both seats' (the ffi-pragmatist
calls it Sl); F (`__builtin_constant_p` per field), memoising the first read and keeping the hole
were the seats' to weigh.

## What was measured

- **S, both seats, built**: the accessor's local becomes `static <struct> hero_constant_value =
  NAME;`, C's rule for an object of static storage (C11 § 6.7.9 p4) asking a constant
  initialiser, and clang's *not a compile-time constant* told `ffi_not_constant`, the scalar's
  class and words. The compiler-engineer: on 74 shapes it agrees with the scalar twin's
  `__builtin_constant_p` on all 27 leaf pairs, on Apple clang 21, Debian 22, Ubuntu 18 and the
  Windows box's clang 23 (clang 18 differs only on `__builtin_abs(-3)`, record and scalar alike);
  of 44 record shapes that all built and 8 that printed two values from two reads, S refuses 20 and
  keeps 24, the same at `-O2`. The ffi-pragmatist: 58 shapes alike on three clangs at both levels;
  raylib's 26 `CLITERAL(Color)` and Darwin's `PTHREAD_*_INITIALIZER`s build and run with
  byte-identical output; no brace list or compound literal of the real headers refused (82, 58, 55
  and 120 over the four legs); refused only objects and calls (74 on the Mac, 111 on Debian, 72 on
  Ubuntu, 1,920 on Windows).
- **The price is Windows'**: 1,834 extern `const GUID` objects, and 86 macros naming one, bind as
  constants today and S refuses them, as a scalar `extern const` object is already refused
  (§4.19: a value, not an object); with the program's own `#define INITGUID`, 91 become values S
  keeps, and the rest are reached by the helper function the message proposes, which binds and
  runs (the ffi-pragmatist). No tracked program binds one.
- **K, the dump route, objected by both**: classifying leaves by clang's node kinds disagrees with
  the scalar on 8 of 27 leaves even refined, accepts `*k_holder.p` (a mutable global through a
  const pointer) and keeps `__builtin_readcyclecounter()`, whose two reads differed; agreeing would
  need a constant evaluator of the compiler's own, panel 218's R5 veto.
- **F, per field**: more code, never asks a `partial` record's unnamed bytes, gains nothing
  measured. **Memoising the first read** gives a record and its scalar twin two rules. **Keeping
  the hole** leaves a `blocking` defect open.
- **The ffi-pragmatist's condition**: `{ 1 / 0, 2 }` must keep panel 218's undefined words, never
  be told as no constant. Measured by the coordinator on S as the seat built it: told
  `ffi_not_constant` with a note naming *an object, a call or a statement expression*, false of
  `1 / 0`. Repaired in the lane: the six warnings the value check makes errors for a number are
  errors in a record's accessor too (`emit/undefined_words.hero`), and where clang says at the
  constant's line that the value is undefined, that is told and the no-constant line is not; then
  `{ 1 / 0, 2 }` reads *division by zero is undefined*, the `#define` quoted, *C leaves this value
  undefined*, one diagnostic.
- **Found beside the sitting**: a record whose value holds a compound literal's address,
  `{ "a", (int32_t[]){ 5 } }`, returned a pointer into the accessor's dead frame (`1` at `-O0`,
  `1000` at `-O2`, `stack-use-after-return`), filed as defect 2121 and refused by S; C2 blind to
  `array_filler` (panel 220's file, R2); the lane's 2109 golden pinning a build key (panel 220's
  file); the lane's message for a probe that does not link naming the header's code where the value
  reads an object a library defines (repaired with this landing).

## Verdicts

| seat | verdict | section | condition |
|---|---|---|---|
| compiler-engineer | **adopt S**; object K (veto if it grows into a C constant evaluator), F, memoising, keeping the hole | §1.1, §4.19, Part 5 | `run` and `unsupported` each move one case, `emission` 8 files and 1 retired, nothing else; the `lldb` suite and a static local's padding unrun by the seat |
| ffi-pragmatist | **approve Sl**; object S as a separate probe line, F, K; **veto** `__builtin_constant_p` of a whole struct | §4.19, §1.11, §1.12; C11 6.7.9p4 | object if the refusal is told `ffi_constant_type` with a false note, if `{ 1 / 0, 2 }` loses panel 218's words, or if `PT_CALL`'s move rewrites the golden in place; veto any version reaching the call site or the layout |

## Resolution (provisional: author ratification pending)

**R1, S**: a record constant's accessor declares its value `static`, so C asks that it be
constant, and the refusal is `ffi_not_constant`, the scalar's class, its note offering the
function a value computed in C is reached through, the value declared first since C takes a brace
list only as an initialiser. Panel 063's condition 1 is closed: no `__builtin_constant_p` is asked
of a struct.

**R2, the undefined words first**: a record whose value C leaves undefined is told so, the six
warnings of the value check made errors in its accessor, the no-constant line beside them not
told.

**R3, the goldens moved, not rewritten**: `run/fixedbugs-094-...-still-builds` loses `PT_CALL` with
a dated correction beneath its head, the shape moved to
`unsupported/fixedbugs-2116-a-record-constant-whose-value-is-no-constant-is-refused`;
`unsupported/fixedbugs-2075-a-record-whose-leaf-calls-...` moves to `ffi_not_constant`, said
beneath its head with the date.

### The question to the author

**On Windows, 1,834 `extern const GUID` objects bind as constants today, and S refuses them as a
scalar object is refused: ratify?** **Recommended: yes**, because a constant names one value and
an object is read where it lives, which is §4.19's rule for every scalar already; a program
reaches such an object through a function of its own, which binds and runs, or makes it a value
with its own `#define INITGUID`. The other answer keeps record objects as constants on every
platform, a rule the scalar does not have, and leaves defect 2121's dead frame to another repair.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | the repair at most +60/-30 lines in `selfhost/emit/`; `run` and `unsupported` each move one case and `emission` 8 files and 1 retired, no other golden | the lane's gate | the batch gate |
| ffi-pragmatist | raylib's 26, Darwin's six and glibc's three `PTHREAD_*` initialisers, and every brace list and compound literal of the four populations build with no shim and no message on every CI leg | the CI's legs, `pop.py`'s lists | the batch's CI |

## Process notes

- **The coordinator's route was refused by both seats, on measurement**, and the condition one of
  them set was met only after the coordinator ran it on the seat's own build.
- **No historian sat**, so no `## Prior art` is owed.

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2123, naming the sitting as `panel 221`, with its one question and the recommendation.

**Corrected 13:03, by `date`**: the head's *synthesised ... from 13:25* was written without reading the clock; `date` printed 13:02 after this file was committed.
