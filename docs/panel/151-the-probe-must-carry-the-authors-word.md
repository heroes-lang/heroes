# Panel 151 — the probe must carry the author's word

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 037, in the **SOUNDNESS LANE**:
two seats, because the route under test appeared to change no surface, no
diagnostic and no spec token. Briefs are the defect entry itself; reports at
`docs/panel/151-reports/`.

**Both seats refused the lane.** That is the sitting's first finding and it is
recorded before the verdict, because a lane chosen wrong is the failure the
skill's own §Two lanes exists to prevent.

## The proposal, verbatim

> **Route B.** The compiler already asks clang what a header's type text is, so
> the correct spelling of a `tag` is reachable and the author need not say it
> twice. `handles.c_spelling` writing `tag_text + " *"` from the author's word
> alone is the one line an answer would replace.

**It was the coordinator's, written into defect 037 an hour before the sitting.**

## Verdict

| seat | verdict |
|---|---|
| **compiler-engineer** | **REFUSE WITH VETO** |
| **ffi-pragmatist** | **REFUSE WITH VETO** as briefed; **adopt with condition** on a narrowing it compiled |

## Route B's premise was false three ways, and the compiler seat proved it on the compiler compiled to C

**`handles.c_spelling` is DEAD CODE.** Zero callers. Proved where a call graph
cannot lie: `grep -c "h_handles_c_spelling" seed/heroes.c` reads **2**, a forward
declaration and a definition. The live line is `selfhost/emit/ctype.hero:139`.
*Coordinator confirmed.*

**The existing clang dump never asks about a handle.** `cli/pointee.hero`'s
`numeric_out` returns absent for `.named`, and a handle record IS `.named`. The
dump the compiler actually wrote for `freeaddrinfo(ai: AI consumes)` contains
`addrinfo` **zero** times.

**And the emitter runs strictly before the ask**: `cli/produce.hero:60` emits the
whole artifact, `pointee.check` is at `:180`. **`heroes check` never invokes clang
at all**, so Route B would do nothing for defect 037's headline symptom.

## The veto both seats reached independently: Route B deletes the check it was meant to serve

**The emitter's probe IS §4.19's instrument.** It works by writing **the author's
word** and letting clang disagree with the header. Route B writes **the header's
own text**, so the probe can no longer disagree with the thing it exists to
check.

Compiled by the ffi seat, two units, one header:

```c
static void probe_today(sqlite3_stmt * a0) { (void)(sqlite3_close)(a0); }
  → error: incompatible pointer types … [-Werror]

static void probe_routeb(sqlite3 * a0)     { (void)(sqlite3_close)(a0); }
  → clang exit 0, zero diagnostics
```

**And the wrong tag is caught today, by name.** *Coordinator's measurement,
2026-09-15*: `record Db tag sqlite3_stmt` passed to `sqlite3_open` and
`sqlite3_close` produces **two** `error[ffi_parameter_type]`, each naming the
header's own `struct sqlite3 *`. **Route B as briefed makes it zero.**

The seat's answer to the sitting's own uncomfortable question — *if the author
writes `tag addrinfo` and clang says `struct addrinfo *`, is that a service or a
lie?* — is therefore **measurable rather than a matter of taste: it is a lie, and
the message that disappears is the evidence.**

## Two premises that earlier sittings argued from and neither ran

**`struct CURL` is not an error. It is a fabricated type accepted quietly.**
`typedef void CURL;` in this Mac's SDK makes `struct CURL` an incomplete type
that converts through `void *`. Panel 150 and `selfhost/handles.hero`'s own
module doc both reason from *"a `struct <tag>` spelling reaches one of the three
handle types in this tree"*, meaning it would break. *Coordinator confirmed*:
it compiles, with a `-Wvisibility` warning, not a refusal.

**And the ffi seat corrected its own panel-150 sentence.** `netdb.h` declares
**18** struct-returning entry points, not 19: its earlier grep counted
`getrpcbynumber` twice, once per `__LP64__` branch. It also found its panel-150
claim does not generalise — for `examples/curl` the cross-wire check is silenced
**by the header**, under any spelling, because `CURL *` IS `void *`.

## The counts this sitting corrected

| claimed | measured |
|---|---|
| `examples/` declares 16 handle types (coordinator's brief) | **5** |
| shipped handle declarations in all trees | **22** |
| distinct tags in shipping code | **7**, and clang's answer matches the emitted text for all seven |

**So Route B's ABI breakage is zero** — which is its one real strength, and not
enough to save it.

## Resolution — provisional, author ratification pending

**R1 — Route B is REFUSED**, with two vetoes, on the ground both seats reached
separately: a probe generated from the header cannot check the binding against
the header. design.md §1.11 clause 3 is the sentence it inverts — *"a wrong type
in an `extern` is a compile error, which is this project's thesis applied to the
boundary."*

**R2 — THE NARROWING BOTH SEATS CONVERGE ON, and the ffi seat compiled it: ask
clang ONE BIT, never the type name.** *Does the author's word need `struct` in
front of it?* The author's word stays the identity, so every refusal survives:

```c
#include <netdb.h>
addrinfo *hero_tagprobe;     /* exit != 0  → emit `struct addrinfo *` */

#include <curl/curl.h>
CURL *hero_tagprobe;         /* exit 0     → emit `CURL *` */
```

Compiled by the seat: the wrong tag is **still refused**, and a tag that names
nothing is **still refused**, loudly. It needs no declarator parser, no
return-type extraction, and it has an answer for a handle that appears in no
signature at all. Cost **0.02 s per tag**, cacheable under the existing key.

**R3 — THE LANE WAS WRONG AND THIS SITTING CANNOT RATIFY R2.** Both seats say so
for different reasons, which is what makes it the finding rather than one seat's
scruple. The compiler seat: Route B's irreducible content is a **stage reorder of
the build pipeline**, and two seats should not ratify that. The ffi seat: the
narrowing amends **design.md:2192**, *"The tag is written verbatim"*, and §4.19's
*"no external tool, no libclang"* — **Part 4 sentences a two-seat lane cannot
touch.** No `spec/heroes-spec.md` token moves under either form, so the lane was
right about the spec and wrong about design.md.

**A FULL PANEL IS QUEUED on R2's narrowing.** Defect 037 stays open until it
sits, and the entry now carries the narrowing so no later session re-derives it.

**What conservative would have been** (CL-040): leave defect 037 open with no
route named. Refused, because both seats compiled a narrowing that works and
recording it costs nothing.

## Author's verdict

**PENDING.** Queued in `docs/work/DECIDE.md` as `panel 151`.

**What a yes settles**: that Route B is refused for inverting §4.19's instrument,
and that the narrowing — one bit, the namespace qualifier, never the type name —
is what the full sitting is convened on.

**What it does not settle**: the narrowing itself, which amends design.md Part 4
and is owed five seats.

## Predictions to score

- **compiler-engineer**: if Route B is ever the route, the closing commit adds
  **more than 150 lines under `selfhost/`** and touches `cli/compile.hero`.
- **ffi-pragmatist**: `record Db tag sqlite3_stmt` produces **2**
  `error[ffi_parameter_type]` today, **0** under Route B as briefed, and **2**
  under the narrowing. **The first number was run at the sitting and read 2.**
