# The defect register leaves the list

2026-09-16, author instruction: the live lists and the ROADMAP carry a very
short preamble, the open counts and the table, and no other story. This is the
story that was moved out, kept verbatim because a record is not rewritten
(CLAUDE.md §14).

## What it was

`docs/work/DEFECTS.md`'s preamble had grown to **28 lines and 982 words**, and
**one paragraph of it was a single sentence of 4551 bytes** recording which
sitting, seat or sweep issued each defect number since 2026-09-08. Everything in
it is true and most of it is hard-won — which sitting found a defect while
briefed on another, which critic filed one out of an option nobody had listed,
which one the author opened by asking a question. It stood in front of every
reader who opened the file to find out what is broken today.

## Two things the move measured, and both argue for it

**It stated the next number twice, with different values.** `grep -o 'the next
number to issue is \*\*[0-9]*\*\*'` returns **037** and **052**, in that order,
inside the one sentence. The paragraph had been appended to for weeks and the
earlier statement was never corrected, because nothing reads it: `records/numbering`
computes the next number as one above the highest issued across the file and
`docs/records/done/`, and its own comment says why — *a sentence in a preamble is
shared state, and two sessions reading the same trunk take the same number and
merge cleanly*.

**It pointed three times at a file that has been a map since 2026-09-12.**
`docs/work/DONE.md` was rotated into `docs/records/done/` that day; the preamble
went on naming it as where a repaired defect moves (two hits here, one more in
`docs/work/DECIDE.md`). A pointer nobody follows does not rot loudly.

## And the check that was supposed to keep the file short could not see it

`list_offences` in `tests/harness/suite_records.hero` policed the region
**between** the banners: it refuses a ticked item there, prose there, an item
outside them. It said nothing about what sits **above** the opening banner,
which is where all 982 words were. The instrument built after `DEFECTS.md` grew
2753 bytes of prose about five already-repaired defects was watching the half of
the file that was not the problem. It gains a preamble ceiling in the same
commit as this move, which is what makes the author's instruction a rule rather
than a tidy-up.

## The register itself, verbatim as it stood

**Numbers are never reused, and 014 was issued twice** — `docs/work/DONE.md`
carries a Windows-diagnostic defect and an FFI-boundary defect both numbered
014, filed a day apart. A record is not rewritten (CLAUDE.md §14), so the
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **052** — **050** and **051** were issued on 2026-09-16 by panel 158, 050 by its completeness critic while auditing the sitting's own framing and 051 by two seats independently, which makes 051 the second here that a COMMENT produced rather than a program; **049** was issued on 2026-09-16, and it is the first here that a REPAIR OF ANOTHER DEFECT created: defect 048's tag half worked and made a blessed artifact depend on the machine that produced it, which CI measured on two legs of one push; **048** was issued on 2026-09-16 by panel 156's ffi-pragmatist, at its own boundary and unasked: the sitting was about a crash message and nobody had put `--emit-c` to it, which makes it the second here a seat found while briefed on something else; **046** and **047** were issued on 2026-09-16 at M-check-completeness step 2, and they are unlike each other in a way worth keeping: 046 came from panel 155's ffi-pragmatist attacking the shapes NEXT to the one the sitting was convened about, and it falsified one of the three grounds that sitting's split rested on; **047 is the first here the AUTHOR opened by asking a question** — why is CI broken — and it is a platform fact measured on one machine and shipped for three, which is the rule `.claude/rules/platforms.md` exists to state; **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

