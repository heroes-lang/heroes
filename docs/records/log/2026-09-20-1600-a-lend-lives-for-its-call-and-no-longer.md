# A lend lives for its call and no longer

2026-09-20. M-declared-extents step 15, landing panel 169's resolution item 1.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **`spec § 13` states the lend's extent**: *A lend lives for its call and no longer: C keeping the pointer reads bytes the program may have changed or freed since, and nothing checks it.* Appended to the `@`-name sentence it qualifies |
| reason | it is the one sentence the document did not contain, found independently by the spec-warden reading the repository and by the llm-ergonomist reading nothing but this file. Every route on panel 169's ballot presupposed it, and the ergonomist wrote the caller-side rule under both readings and could not tell which compiles |
| design.md § | §1.6, §4.19, §1.2 |
| panel | 169, provisional |

## Which reading was taken, and by whom

**`a lend lives for its call`**, taken by the coordinator under CLAUDE.md § 3's
rule that a whole milestone asked for in one `/step` decides its delegated
questions with the recommended resolution as the default and says once which way
it went. The author's ratification is queued as `panel 169`, with the
recommendation and its cost written into the item.

**Three measurements carry it.** It is what `examples/gallery/13-lease.hero:3`
already teaches, so the document catches up with the teaching material rather
than inventing a rule. It over-refuses **nothing**: the two sound nine-line
programs panel 169's completeness critic wrote are legal under it. And it is the
only reading under which the ffi seat's veto on withdrawing the field lend stays
consistent with the rule the same sitting adopted, because the refill loop is
that veto's own idiom.

**What it costs, said rather than buried.** Under this reading defects 066 and
068 are one class again and a caller-side rule closes neither. What closes them
is R5 — the author allocates, C frees with the function the author names — plus a
mark on the parameter that retains. An unmarked retaining parameter stays a
silent wrong answer, which none of the ten ecosystems the historian surveyed does
better.

## Two drafts, priced on the binding instrument, and the dearer dropped

`--refresh` on `claude-opus-5`, applied to the real path and re-measured each
time, which is the only route to the number design.md §1.6 names:

| draft | real | Δ |
|---|---|---|
| baseline | 8154 | — |
| *A lend lives for the call it stands in and no longer: a C function that keeps the pointer reads bytes the program may have changed or freed, and nothing here checks that* | 8209 | **+55** |
| **adopted**, fifteen words shorter for the same two facts | **8201** | **+47** |

**+33 vendored**, against a `DELTA_GATE` of 50 which is measured in vendored
tokens. Digest re-pinned `249990ca1b6b2f66` to `6c3a27eb1b8ac830`, in both places
in one commit. **2039 free** by the tool's own headroom line, 1979 net of the FFI
floor.

## What paid

Five registered falsifiable predictions, one per seat, in panel 169's file, each
naming an instrument that exists today and the milestone it is scored at. No
named removal was spent.

## And an arithmetic slip in the ledger, corrected in this row rather than above it

The row this one follows states **1986 free and 1926 net** at 8154 real. The
tool's own headroom line reads 2086 and 2026 at that figure. The older row is a
record and stays as it was written; this row states its own numbers from the
tool's output rather than from arithmetic, which is what the discrepancy argues
for.
