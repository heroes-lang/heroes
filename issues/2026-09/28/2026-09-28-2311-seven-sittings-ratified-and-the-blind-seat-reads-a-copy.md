---
kind: decision
area: none
milestone: none
filed: 2026-09-28
commit: cea6e9d0dabe7f36776d30ee877cda6742017b64
github: none
---

# Seven sittings ratified, and the blind seat reads a copy

2026-09-28, evening, `/decide` on the author's answer, with no milestone open.

## The decision

| | |
|---|---|
| date | 2026-09-28 |
| decision | **panels 175, 176, 177, 179, 180, 181 and 182 are ratified in one act**, every open item of `docs/work/DECIDE.md`, each with the recommendation its item carried; the list returns to **zero** |
| reason | the author, in these words: *attacchiamo i due difetti apetti e ratifca tutte le decide*, meant as: let us attack the two open defects, and ratify every decision on the list |
| design.md § | §4.15, §1.11, §1.12, Part 5 |
| panel | 175, 176, 177, 179, 180, 181, 182 |

## How it is recorded

**As a reading**, CLAUDE.md § 4's default, which the author asked on
2026-09-21 to be taken for granted; not `by delegation`. The record of the
seven items is
`docs/records/done/2026-09-28-2311-seven-sittings-ratified-in-one-act-and-the-blind-seat-reads-a-copy.md`,
and each sitting's `## Author's verdict` opens with its own paragraph, the
`Pending` stub kept beneath it.

## What the yes applied, and what it found already landed

Every provisional resolution had landed during M-agreed-retention, verified
against the tree at `a6eab736` before its verdict was written: the defects the
four FFI sittings named, 075, 077, 078, 079, 080, 081, 082, 088 and 090, are
all in `docs/records/done/`; `contract_differs` and `ffi_owned_const_cell` have
their goldens; `heroes probe` is the thirteenth verb and the reader holds rule
(d); `spaced_minus_element` and `continuation_outside_brackets` are both on
`diag.is_thesis_rule`. **Three things were not landed, because they waited on
the yes, and they are applied in the same commit**:

- **Panel 175 item 7**: the llm-ergonomist reads a copy of the specification
  outside the repository, in its own scratchpad directory, and reports whether
  any project rule reached its context. Reading `spec/heroes-spec.md` in place
  loads `.claude/rules/spec-shape.md` by its `paths:`, and that file carries
  the token counts the seat is never given (`.claude/skills/panel/SKILL.md`,
  `.claude/agents/llm-ergonomist.md`). The briefs of panel 181's second reading
  already handed the seat a copy outside the tree; the rule had not said so.
- **Panel 180's arm**: design.md §4.15's bullet on a spaced `-` named a
  literal's elements and called both fixes `guess`, while the compiler refuses
  the same shape opening a `match` arm with a `certain` fix; measured on the
  trunk's compiler before the sentence was written.
- **Panel 182's deferrals**, deferred and not refused: routes (b), (f) and the
  critic's (g), MemorySanitizer as a Linux leg, and coalescing. **Scheduled by
  the coordinator** under CLAUDE.md § 3's delegated default as one open item of
  `docs/work/milestones/M-deployable-binary.md`, the scheduled milestone whose
  deliverable is what a built program costs when it runs; the author may move
  it. Before this, the deferrals lived only in the `DECIDE.md` item, which the
  ratification removes, so without a live home they would have gone silent.

**What stays open**, named rather than bundled: the defects the evening is
repairing, 129 (item 3 of panel 181 failing on a statement broken at more than
one line end) and 130 and 131, in their own lanes; and the registered
predictions of these sittings whose milestones have not come, each at its row.
