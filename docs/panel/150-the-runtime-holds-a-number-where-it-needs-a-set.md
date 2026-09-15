# Panel 150 — the runtime holds a number where it needs a set

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 037 and 038, without asking,
under CLAUDE.md § 4's *ask once per milestone then convene again*. **Full panel**,
five seats plus the completeness critic. Briefs at `docs/panel/150-briefs/`,
reports at `docs/panel/150-reports/`, all written before any seat started.

## The proposal, verbatim

> **Q1, defect 037.** A `ptr` producer leaks at exit 0. When must a `ptr`
> producer be marked?
>
> **Q2, defect 038.** A fixed array of handles behind ONE mark, released element
> by element, aborts at 134 against a message naming two causes that do not fit.
> What does a whole-value mark owe when the release is per element?

## The verdict table

| seat | Q1 | Q2 |
|---|---|---|
| **llm-ergonomist** | approve, three conditions | **VETO** |
| **historian** | approve (advisory), three conditions | approve *refuse the caller's loop* |
| **spec-warden** | **object** — the rule is right, the ORDER is wrong | **object** — no spec sentence; fix the message |
| **compiler-engineer** | **REFUSE WITH VETO** on the type-keyed rule | **REFUSE WITH VETO** on per-element; refuse the refusal; adopt the message |
| **ffi-pragmatist** | **object** — the ABI is untouched | **VETO** on *refuse the element release* |

## What the sitting measured, and every seat contributed one that mattered

**The mark on a `ptr` cannot be read.** `malloc(size: u64) -> ptr acquires
sqlite3_notafunction` beside `free(p: ptr consumes)` **checks at exit 0** — the
spec-warden. `consumed_types` keys on a declaration index and a `ptr` has no
declaration, so `releaser_reads` has nothing to compare — the compiler-engineer.

**And obeying the wrong word corrupts.** `fopen(…) -> ptr acquires free`,
released with `free`: check exit 0, **`AddressSanitizer: attempting free on
address which was not malloc()-ed`** — the compiler-engineer. Two
non-interchangeable releasers in `netdb.h`, swapped: check 0, run 0, and under
the sanitizer **`SEGV` inside `freeaddrinfo`** — the ffi-pragmatist. *Coordinator
confirmed both.*

**The `ptr` probe silences the check §4.19 exists to perform.** One emission, one
type changed: the `ptr` form writes `(void *)a3`, the handle form writes
`addrinfo * *`. The cast is what makes the cross-wire invisible.

**The per-element mark is refuted by construction.** Four `@` out-parameters each
`acquires`, against a header filling two of four: `panic: 2 C handle(s) never
given back`. **It turns defect 038's `-3` into a `+2`** — the compiler-engineer.

**And the instrument is pointed the wrong way.** The program that releases four
elements and corrupts nothing **aborts at 134**; the one that releases elements
AND the composite, a real double free, **exits 133 with zero bytes on both
streams** — the ffi-pragmatist.

**A class nobody had filed.** A producer that fails returns NULL and `acquires`
**counts the NULL**, so a program handling the failure correctly aborts —
the ffi-pragmatist. *Coordinator confirmed on a handle type, so it is not
`ptr`-specific.*

## What the completeness critic found, and it dissolved four of the five verdicts

Report at `docs/panel/150-reports/critic.md`.

**A. The two routes nobody listed are ONE route, and each seat had half.**
`grep` over the two reports shows neither seat saw the other's. Both reproduce.

- **`record Blob tag void` is capped at ONE per program.** Two of them are
  `error[duplicate_tag]: one tag, one type`. **Defect 037's own header needs it
  twice**, so under that route the briefed program is **unwritable**. And the ffi
  seat had measured the other half without knowing: *"`netdb.h` declares 19
  `struct X *`-returning entry points and zero `void *` ones"* — **it covers zero
  of nineteen.**
- **`record AI tag addrinfo` checks at 0 and clang refuses the build**, 18 times
  *must use 'struct' tag*. The ffi seat's hand-edit was a **one-token
  substitution**, and the whole gap is that the grammar holds one identifier where
  C writes two.
- **`tag void` gives up exactly the §4.19 check**: an 8-byte `malloc` handed to
  `freeaddrinfo` **builds clean with zero clang diagnostics**, because `void *`
  converts to anything. Under `struct addrinfo` the same cross-wire is a hard
  `-Werror`.

**The composed sentence nobody priced: a `tag` names a C TYPE NAME**, not an
identifier.

*Coordinator confirmed the cap: two `tag void` records are refused.*

**B. The second spec falsehood is this sitting's business, and D1 is half the
repair.** `consumes` occurs only in `CParam` under `Extern`, so *any `extern`* is
sound — and `tests/golden/check/unmarked-handle-producer.hero` **already says
exactly that in its own header comment**; the spec is the only document still
saying *group*. **But the demand crosses MODULE boundaries**, which D1's text does
not say, **and the diagnostic names no location for the arming declaration** — a
§4.17 defect nobody filed. **This dissolves the historian's Q1 approval**, whose
condition rested on *the blast-radius-of-one-file is what an audited region
buys*. **It is not a region**, and the one-file number was the coordinator's brief
counting a conjunction the checker never evaluates.

**C. The ergonomist refuted itself**, and not by 149's licenses-versus-causes
pattern: it wrote the cause down and scored it as somebody else's. **Its condition
1 is already unmet on handles** — `_ = slot_open(…)` checks at 0 and aborts at
134, so no static refusal exists. Route A satisfies all three conditions **by
construction**, and it was never shown it.

**D. The narrowed refusal's premise is FALSE.** The ffi seat offered to withdraw
its veto for a refusal narrowed to *elements AND composite*, on the ground that
elements-only is safe. The critic built elements-only with **four interior
pointers into one block**: **133, zero bytes.** **Elements-only corrupts.** The
seat had measured a property of the header it wrote. And `raylib.h`'s
`UnloadFontData(glyphs, glyphCount)` is a **whole-array release in one call**, so
the seat cited as its counterexample the rule it exemplifies.

**And the repair all five seats converge on is only ever readable on the one
program that is memory-safe.**

**E. Two defects, not one, and one of them is a LEAK.** `acquires` counts a failed
call at **both** positions; `borrows` does not; and **`slot_close(nullptr)`
discharges any obligation**, so a program holding a real leaked handle plus one
null release **exits 0 in silence**. **Three of five real producer call sites are
exposed**, and `examples/sqlite/main.hero` is measured: it prints its fallback and
is aborted at 134. That file **already says in writing** why the corpus is silent,
*"a fact about its SQL and not a defence"*, and no brief or seat cited it.

*Coordinator confirmed the silent exit 0.*

## The question the sitting did not ask, and the brief foreclosed it

**Every finding above is one fact: the runtime holds a NUMBER where it needs a
SET.**

`runtime/parts/alloc.c:174-178` already decides this, in writing — *"a different
instrument, not a better sentence"* — **with no cost in the cost argument**,
against CLAUDE.md § Precedence's *"a guard that closes a corruption class lands,
with its cost measured and reported rather than argued."* The message landed on
2026-09-14 and has been rewritten twice since; the route all five seats converge
on would be **the third rewrite in two days**.

**And the coordinator's shared brief foreclosed the question in its own words**:
*"Nothing has to be built; something has to be refused."*

Two further brief defects the critic names. **The llm-ergonomist reads neither
the shared brief nor any sitting by charter**, so the brief's *read these three
sittings* is unsatisfiable for one seat in five and the brief does not say so.
And 149's citation failure did not recur: 147 is cited by three seats, 148 by
three, 149 by four.

## Resolution — provisional, author ratification pending

Adopted under CLAUDE.md § 4: the most robust and complete resolution, never the
cheapest and never a compromise. **It is not what any seat proposed**, because
the critic falsified the premise four of them shared.

**R1 — THE COUNTER BECOMES A SET.** `hero_live_handles` stops being one
`_Atomic int64_t` and becomes the set of live handle addresses. This is the
instrument `alloc.c` named and priced at nothing, and CLAUDE.md § Precedence rank
3 carries it: it closes a **silent corruption class**, and a guard that does lands
with its cost measured rather than argued. What it settles, each measured broken
today:

| today | with a set |
|---|---|
| a null acquisition is counted, so a correct failure path aborts | a null is not an address; nothing is recorded |
| `slot_close(nullptr)` discharges a real obligation, exit 0 in silence | it discharges nothing |
| a double release and an unmarked producer are indistinguishable | the set knows the address was already gone |
| the element release and the composite release are indistinguishable | the set knows which addresses it holds |
| the message names a closed list of causes, rewritten three times in two days | it names the address and what happened to it |

**R2 — a `tag` names a C TYPE NAME.** The grammar holds one identifier where C
writes two, which is the whole reason defect 037's briefed case has no spelling.
With it, `record AI tag struct addrinfo` closes that case with **no new
diagnostic**: `unmarked_handle_producer` already demands the mark,
`unread_releaser` already reads it, and `type_mismatch` plus clang's own
`-Werror=incompatible-pointer-types` already refuse the cross-wire. **It is the
only route measured to INCREASE what clang checks.**

**R3 — the mark is refused on a `ptr`.** Not widened to it. `acquires` on a `ptr`
is a word the compiler cannot read, design.md Part 6's *a tag nobody reads is a
comment that looks like a guarantee*, and obeying a wrong one was measured to
corrupt twice. With R2 landed, the author has a spelling for every real case, so
the refusal costs no program.

**R4 — the spec is corrected on both counts it is false about**, the warden's D1
at **+7 real**, widened by the critic's finding that the demand crosses module
boundaries.

**What conservative would have been, recorded so the author can choose it**
(CL-040): fix the runtime message only, as all five seats converged on. It is
nine lines in one file and zero spec tokens, and it is **refused here** because
the critic measured that the message is only ever readable on the one program
that is memory-safe, while the two that corrupt exit 133 saying nothing at all.

## What this sitting produced besides its resolution

| defect | what it is |
|---|---|
| **039** | `acquires` counts a failed producer's NULL, so a correct failure path aborts at 134 — and three of five real producer sites are exposed |
| **040** | releasing a null discharges a real obligation, so a leaked handle plus one null release exits 0 in silence |
| **041** | `unmarked_handle_producer` names no location for the declaration that armed it, across a module boundary (§4.17) |

## Author's verdict

**RATIFIED 2026-09-15**, the resolution as adopted: the robust one and not the
conservative one. The alternative recorded under CL-040 — rewrite the runtime
message and nothing else — is declined.

**Two of the four resolutions had already landed when the yes was given**, and
the verdict is therefore partly on what SHIPPED and partly on what is owed. That
distinction is the useful half and it is written out rather than left to a
reader.

**LANDED, and measured before and after.** R1, the counter as a set of live
handle addresses. It closed defects 038, 039 and 040 together, because all three
were one fact. The correct failure path went from abort 134 to exit 0; a leaked
handle plus one null release went from exit 0 in silence to an abort naming the
address; a fixed array released element by element went from `+3` and an abort to
exit 0, because the program was correct all along.

**OWED, and named so it is not lost.** R3, the mark refused on a `ptr`, is
**unlanded** — and panels 151 and 152 changed what it should look like, since the
author now has no alternative spelling for a struct C names with two words, so
refusing `ptr` today would leave `getaddrinfo` unbindable rather than badly
bound. It waits on defect 037's remaining half. R4, the specification corrected
on both counts it is false about, is **unlanded** at +7 real, and panel 152's
spec-warden re-ran all three probes and confirmed the document is still false at
HEAD.

**R2 is superseded rather than owed.** *A `tag` names a C type NAME* was adopted
here without a spelling; panels 151 and 152 then refused two spellings on
measurement and adopted a third. The chain is the record.

**What a yes settles**: that the instrument becomes a set rather than a number,
that a `tag` may name what C actually writes, that the mark is refused where it
cannot be read, and the spec correction.

**What it does not settle**: whether the per-element release is ever legal, which
R1 makes DIAGNOSABLE without deciding, and which no seat could settle because the
one shipped example either seat offered turned out to be a whole-array release.
