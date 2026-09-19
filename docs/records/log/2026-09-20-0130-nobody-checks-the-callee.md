# Nobody checks the callee

2026-09-20. Panel 167, at M-declared-extents, on defect 066 — the lend's
lifetime — which panel 166 filed and declined to price.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **route A is adopted** — a field lease that COPIES, spelled as a second use of `.lease()` so it costs no new surface, allocated with a trailing header so the pointer C receives is the allocation base, emitted `const` from a non-`@` root — **with two type rules widened from `cstr` to `ptr`**, without which the critic measured it is not sound. Route C refused on two vetoes, route D on one, route B not adopted on a ceiling |
| reason | no language in a ten-ecosystem survey enforces foreign retention statically, because the callee is not in the type system; what survived everywhere is a lend, a lease with an owed release, and a run-time instrument. Heroes has the first and — measured against a real `.dylib` — **not the third**: `--sanitize` prints zero reports and the correct value, so it masks the bug rather than catching it |
| design.md § | §1.12, §4.19 (including its third reserved case, unbuilt), §1.6 |
| panel | 167, provisional — author ratification pending |
| ratification | queued as `panel 167` in `docs/work/DECIDE.md` |

## Three things the sitting found that no document in it knew

**The lease we were about to copy is itself open.** A lease pointer copied into
an `extern` group's record survives `end_lease` and reads freed bytes: `check`
exit 0, `heap-use-after-free` under the sanitizer. Clause 1 permits a lease name
as an argument of a call and a record constructor is a call. Filed as **defect
067**, and it is repaired before route A extends the mechanism it lives in.

**What actually holds the `cstr` lease sound is two rules `ptr` does not have** —
no Heroes function answers one, no record outside a group holds one. Both check
clean for `ptr` today. So *the cheap spelling* and *the sound spelling* of route
A are not the same spelling, which is the axis no seat measured.

**Defect 066 is two defects.** The second — a live record rewritten while C holds
its field's address — is a wrong answer at exit 0 that **no sanitizer can ever
see**, because nothing is freed and no frame dies. Filed as **defect 068**.

## What was refused, and on what

Route C, writing the hole into the specification, is refused on two vetoes: one
on locality, because the `extern` declaration is byte-identical whether C retains
or not, so the fact lives in a C comment; one on the instrument, because there is
none. It was the cheapest thing on the table at **+13 real**, measured to the
digest, and a veto is a refusal rather than a price.

Route D, withdrawing the lend, is refused on a veto measured at 28 `const void *`
declarations in `sqlite3.h` and 7 in `raylib.h`. **The veto was measured on a
tree without route A**, where nothing but a lend reaches such a parameter; once
the lease lands it reaches them too. That is named in the resolution as the next
sitting's question rather than left to be rediscovered.

Route B, the binding author's mark, is not adopted on a ceiling: three files sit
at 298/300, 524/525 and 1174/1175, two code lines turn two of them red, and
**panel 166's own `counted_by` — landed the same afternoon — touched all three.**
Its stated completeness ground was falsified by the critic, who compiled the
route out the engineer said did not exist.

## And the sitting corrected its own coordinator twice

The shared brief said the sanitizer catches the escape. It catches it only
because the coordinator's retainer was a `static inline` in a header; behind a
real shared library it is silent and prints the correct value. And the brief's
file-size table ranked the files the lend already lives in rather than the files
a mark would enter — which is why one seat found the only ceiling veto in the
sitting and four did not.
