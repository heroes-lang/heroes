---
kind: decision
area: none
milestone: none
filed: 2026-10-01
commit: 9d1c209d1cef5a7f643d8913117b73aecbe3594a
github: none
---

# Seven rulings the recovery batches applied, written where they can be read

Written 2026-10-01 at 00:26, late, on the audit of defects 130 to 133 run on
the afternoon of 2026-09-30 (`scratchpad/audit-130-133/`, the auditor's report
in the coordinator's session): the rulings the audit leaned on existed only in
the scratchpad item files the coordinator wrote for the lanes,
`lane-recovery-b2-items.md` to `-b4-items.md`. A decision nobody can open is not
a record (CLAUDE.md § 14, per decision a log entry), so each is written here
with its words and its reason.

## The decision

| | |
|---|---|
| date | 2026-09-30: the item files that carry them were written at 02:04 (batch 2), 04:52 (batch 3) and 09:40 (batch 4), by their files' own times |
| decision | seven readings of design.md §4.17 (one mistake costs one message, at the mistake, none hidden) for the recovery cluster, defects 130 to 133, taken by the coordinator as a lane's item list's default (CLAUDE.md § 3) |
| reason | each below |
| design.md § | §4.17, §4.15 |
| panel | none for 1 to 5 and 7; 6 went to panel 184 |

1. **A braced block's lines are judged as they will be once the braces are
   gone** (batch 2's B1): one level, exactly 4 spaces, deeper than the head's
   line, so a margin, a `,` or a `)` the unbraced program would refuse is told
   in the same run, once per block, with the code the unbraced program gets;
   the braces' own message is unchanged. **Not yet landed**: the audit found 12
   shapes of item 130 still told only once the braces go (its rows 130-17a,
   17b, 21a, 21c, 27, 30b, H-001 to H-003, B1a, B1b and B4), and no item list
   after batch 2 carried the ruling; batch 5 takes it.
2. **`record {` with no name** (batch 2's B2): the one token is told once, and
   its words say both what is missing and where the fields go (`record Point`,
   its fields on the lines below it, one level deeper); the same for `variant {`
   and every head that takes a name before a block. **Not yet landed**: the
   audit's rows B2a and B2b; batch 5 takes it.
3. **The Allman `empty_record` and `empty_variant` are not a defect**: the code
   names what the parser sees, a head whose indented block is empty because the
   `{` stands at the head's column, as `missing_body` does for `function
   main()` over `{`, and the message names the brace (*never in braces (found
   {)*). No new code.
4. **A braced `if`/`else` chain costs one `missing_body` per braced block**,
   each block's braces being that block's mistake, as two braced functions are
   two; the recovery instrument's `brace-else-chain` EXTRA is not a defect.
5. **One indentation habit is one mistake** (batch 4's C4, moved to batch 5):
   told once per run of consecutive lines that carry it, at the run's first
   line, with the code each line gets today and words that name the run's
   extent; the fix re-indents the whole run and is `certain` only where the
   run's levels map one to one onto multiples of four (a two-space run whose
   every margin is even; a run of tabs alone), a `guess` otherwise; a line in
   the run that also holds another mistake still has that mistake told.
6. **`forget-f` and `over-indent` SILENT are the language's reading today**
   (spec § 2's *a literal without the `f` is unchanged*; nothing in the spec on
   a statement after a jump), so not a recovery defect: they went to panel 184,
   which sat on 2026-09-30 and waits on the author.
7. **A certain fix is judged by what `check --apply` writes**, and a later
   stage's first message after it (`unknown_name`, `type_mismatch`, an unused
   binding) is read as the next stage speaking, not as the fix's lie, since
   `check` runs a stage only when the one before said nothing
   (`docs/records/log/2026-08-04-0026-runs-three-stages-in-order-lex-parse-resolve-check.md`).
   This is how the audit and the lanes' reports read the new codes after an
   apply; the recovery instrument itself counts every new code as
   `APPLY-NEW` (its judge's `flags_of`, in the scratchpad, 2026-09-30), so a
   row it flags is read by hand against this ruling, never waved through.

## How it is recorded

As the coordinator's, not the author's: rulings taken as a lane's default,
each open to the author's reversal; the author is told in the next report
that they exist and where.
