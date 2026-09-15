# Panel 154 — the class was never argument-shaped, and one flag closed it

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 045, without asking, under
CLAUDE.md § 4's *ask once per milestone then convene again*: panels 150 to 153
convened under the same clause the same day. **Full panel**, five seats plus the
completeness critic. Briefs at `docs/panel/154-briefs/`, reports at
`docs/panel/154-reports/`, all five briefs written before any seat started and
**the working tree frozen from the briefs to this synthesis** — which panel 153
promised and broke, and which this sitting kept.

## The question

> **A Heroes program hands a handle holding `nullptr` to a C function that
> dereferences it. What stops it?**

The brief offered five routes and invited a sixth: **A** guard every handle
argument; **B** guard by default with a word to permit null; **C** no guard
unless a word demands one; **D** no new word, the existing marks decide;
**E** refuse at compile time where the argument is written `nullptr`.

## The verdict table

| seat | A | B | C | D | E |
|---|---|---|---|---|---|
| **compiler-engineer** | **adopt** | object | **veto** | **veto** | **veto** as the resolution |
| **ffi-pragmatist** | object | **adopt** | refuse | **veto** | object |
| **spec-warden** | object | **adopt** | **veto** | **veto** | **veto** |
| **historian** (advisory) | object | **adopt** | object | object strongly | adopt as an addition |
| **llm-ergonomist** | object | **adopt** | object | object | refuse |

**Four of five converged on B.** Route D was vetoed twice and its premise
falsified by measurement; C and E were vetoed as resolutions.

## What the seats measured, and each contributed one that mattered

**§1.12 had already decided the default, and no seat had quoted it before
today.** The ffi-pragmatist and the compiler-engineer found the same paragraph
independently: *"they check at the boundary even where the value is known to be
good … because the alternative is a rule that has to reason about where a value
came from, and provenance is a premise about the world. That is one predictable
branch per FFI argument, and it is paid deliberately."* **Routes C and D are
provenance rules.** The design document refuses them in advance.

**The census nobody had taken.** The ffi-pragmatist dumped this Mac's SDK
through clang's own AST and counted pointer parameters by header family — 129
functions in the `stdlib.h` family with 134 pointer parameters, 86 in `stdio.h`'s
with 125 — and checked the documented NULL contract for each that matters. That
is the number route A's cost rests on and it was a premise until this sitting.

**The guard costs nothing.** The compiler-engineer wrote route A's patch rather
than estimating it — 4 files, 31 insertions, 6 deletions, `ops.hero` 258 → 272
code lines against a ceiling of 300 — and timed the guard at 10^8 iterations:
medians identical at 0.37 s, the guarded run inside the unguarded run's own
spread.

**A correction to the brief, from the seat that measured it.**
`guard_cstr_arguments` **does not exist**: the function is `guard_arguments`,
`selfhost/emit/ops.hero:244`. The stale name is in `.claude/rules/c-boundary.md`,
in defect 045's own entry and in both briefs — I propagated it from the rule
file into the sitting's framing.

**And a reader's own account of why the shape misleads.** The llm-ergonomist,
reading only the binding: *"`borrows` told me about ownership and I read it as
also telling me about presence — the library keeps one, so there is one. The only
mark on the result line talks about lifetime, which primes a reader to believe
lifetime is the only question a result has."*

## What the completeness critic found, and it is the resolution

Report at `docs/panel/154-reports/critic.md`.

**THE ROUTE NOBODY LISTED IS ONE STRING IN THE FLAG LIST.** Five seats searched
the Heroes call site and the brief's five options are five ways to legislate an
**argument**. Nobody compiled the emitted C any other way. The historian named
`-fno-delete-null-pointer-checks` as *"route G, cheap hardening, insufficient
alone"* and marked it unrun; the critic ran it, on the panel's own witnesses,
under the project's own flags:

| witness at `-O2` | today | with the flag |
|---|---|---|
| the fixture, two calls | exit 133, both streams empty — the correct `7` lost too | exit 134, stdout `7`, the named panic |
| one call only | **exit 0, prints `8372224`**, five runs of five identical | exit 134, named |
| null behind a non-null handle | exit 133, both streams empty | exit 134, named |
| a null **`ptr`** argument | **exit 0, prints `8372224`** | exit 134, named |
| a field 1 MiB past null | exit 139, silent, **also at `-O0`** | exit 139, still silent |

**It guards the dereference and not the argument, so it refuses nothing.**
`free(NULL)`, `sqlite3_close(NULL)` and the shipped
`getaddrinfo(hints: nullptr)` golden all stay legal — and that last one is why
no route on the table could ever be widened to `ptr`: it would refuse a case
already in the tree.

**What it costs, measured with the machine still**: twenty `--emit-c` runs of
`examples/ledger/`, median `real` 2.87 s with the flag and 2.87 s without; 10^9
iterations of a chain walk carrying three redundant null checks the optimiser
would otherwise delete, `real` 1.04 s both; the seed +16,528 bytes, 0.38%; the
emitted C **byte-identical**; the compiler's own 653 tests green either way.

**Three sentences of my brief were false, and the third was a defect in a repair
that had landed an hour earlier.**

1. *"`-O2` prints a value for a node that does not exist, exit 0"* — false of the
   fixture it cited, which is exit 133 with empty streams, and true of a shape
   the brief never cited. **The shape decides and neither general statement
   survives.**
2. *"No runtime guard can ever reach it"* — false twice. The trap is a
   deliverable signal (SIGTRAP, caught in a handler, measured), and one flag
   returns the fault to the `SIGSEGV` path the runtime already owns.
3. *"A field's offset is smaller than a page in every struct a header can lay
   out"* — the premise under the window my own repair had shipped. Falsified by
   three lines of header, and then bounded properly: 275 records laid out from 21
   headers of this SDK, exactly one over a page, `_opaque_pthread_t` at 8192
   bytes. **`.claude/rules/module-shape.md` forbids resting a narrowing on a
   premise about the world**, and that is what it was.

**And the `--sanitize` row understated what already ships by a full exit code**:
that leg names all four witnesses with file and line, exit 134, including the
1 MiB case.

**Two seats' corpus censuses disagreed and the critic counted it itself**: 73
handle parameters in `.hero` files, 35 unmarked, 26 `consumes`, 11 `acquires`
through an `@` out-parameter, none in `selfhost/`. The compiler-engineer's side
is the checkable one; the spec-warden's 41 is low by nearly half and was
load-bearing in two of its arguments.

## Resolution — provisional, author ratification pending

Adopted under CLAUDE.md § 4: the most robust and complete resolution, never the
cheapest and never a compromise. **It is not what the seats converged on**, and
the critic's own sentence is why: *"here the cheapest in tokens and the most
complete are the same route, which is the tell that the frame, not the price,
decided."*

**R1 — `-fno-delete-null-pointer-checks` JOINS THE FLAG LIST.** LANDED. It
closes every wrong-answer-at-exit-0 shape the sitting could measure, including
the two no route on the table reached — a null `ptr` and a null behind a non-null
handle — at zero surface, zero spec tokens, zero new words and zero refusals of
legal C. `selfhost/cli/flags.hero` carries the reason and the measurements beside
the string, and its own test now pins fifteen flags rather than fourteen.

**R2 — THE NULL WINDOW BECOMES THE PLATFORM'S OWN FLOOR.** LANDED.
`runtime/parts/stack.c`'s `addr < 4096` becomes `HERO_NULL_WINDOW`, which is
Darwin's measured `__PAGEZERO` of 4 GiB on this platform and 64 KiB elsewhere,
and the message now prints the faulting offset so a reader can tell a null plus
a field from a small wild pointer. Measured after: the 1 MiB field says
`at offset 0x100000, called from big_value` at `-O0` and `-O2` both, where it
died at 139 in silence before. **Panel 104's wild store still re-raises**,
because the stack lives near the top of the address space. The 64 KiB half is
marked as the inference it is, per `.claude/rules/platforms.md`: a platform fact
is run on a platform, and the Linux and Windows floors were not.

**R3 — ROUTE A IS ADOPTED AND NOT LANDED HERE, and the reason is the one thing
it buys.** The flag closes the class; route A's guard buys the **blame line**.
Measured: at `-O0` the panic says *called from `node_value`*, and under the flag
at `-O2` the same program says *called from main*, because inlining flattened
the frame. That is a real diagnostic cost and it is the honest argument for the
guard — not the class, which the guard does not close, since it cannot be widened
to `ptr` without refusing a shipped golden. The compiler-engineer's patch exists,
is formatted, and passes `canonical`, `layout` and `order`; it is **not
compiled**, by its own statement, and it lands on its own clock.

**R4 — ROUTE B's WORD WAITS FOR A SECOND WITNESS.** Four seats adopted it and it
is not refused: it is **deferred on the evidence rule the compiler-engineer
stated and I am holding the sitting to** — *two is a class and one is a
witness*. Exactly one shipped program would be refused by route A today, and
until a second exists the fourth parameter word buys nothing the flag has not
already bought: +36 real spec tokens in the warden's best encoding,
`selfhost/ast.hero` four lines from its ceiling, the re-printing walk in the
formatter, the dump, `mutate` and the syntax self-check, a new refusal that
`.claude/rules/verification.md` sends to five golden suites, and a new meaning
for the 35 unmarked handle parameters already in the tree.

**R5 — ROUTES C, D AND E ARE REFUSED**, C and D with the vetoes cast against
them. D's premise — that a `consumes` parameter accepts NULL and an unmarked one
does not — was falsified against vendor documentation by two seats
independently, and §1.12 refuses it in advance as a provenance rule. E is
inverted, in the ergonomist's words: it forbids the correct null and permits the
dangerous one.

**R6 — WHAT STAYS OPEN, named rather than closed by silence.** A hand-written
struct whose touched field sits past the platform floor on Linux or Windows,
where the floor is the inference of R2 rather than a measurement. No record in
this SDK is past it. The falsifier is a program that reads one and dies at 139
in silence, and what closes it is each floor read on its own platform.

**What conservative would have been, recorded so the author can choose it**
(CL-040): land route B as the four seats converged, with `accepts_null` on the
parameter and its spec sentence, and close 045 on that. **Refused here** because
the critic measured that B leaves three shapes open — including two wrong
answers at exit 0 — while the flag closes them, and because a word a reader must
learn is the most expensive thing on this table under §1.2 and bought the least.

## What this sitting produced besides its resolution

No new defect. Three corrections to records that had already landed: the stale
`guard_cstr_arguments` in `.claude/rules/c-boundary.md` and in defect 045's
entry, the `-O2` row of that entry, and the window premise in
`runtime/parts/stack.c`.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| **critic** | the flag costs nothing: medians `real` 2.87 s against 2.87 s on twenty compiler runs, 1.04 s against 1.04 s on 10^9 iterations | `/usr/bin/time -p`, machine still | **scored in its own report**, and re-run at this landing |
| **historian** | clang's `_Nonnull` annotations cover a small fraction of the SDK, so a route reading them degrades to the default | a count under the SDK | **SCORED, held by a factor of ten**: 66 of 3120 headers, 2% |
| **compiler-engineer** | route A's guard is not foldable and fires at every level | build the witness at `-O0` and `-O2` | **scored**: exit 134 named at both, under the flag |
| **ffi-pragmatist** | zero `static inline` definitions in the headers it counted | `grep` for the spelling | **SCORED, MISSED**: 33 SDK headers carry 4061 always-inline definitions; the negative claim rested on the spelling searched for (CL-018) |
| **spec-warden** | R4, shortening § 13's `acquires sqlite3_finalize`, is −9 real | `heroes measure --refresh` | **SCORED, held**: 7965 real, digest `70ca3e663b0ae136` |

## Author's verdict

*Pending. Queued in `docs/work/DECIDE.md` as `panel 154`.*
