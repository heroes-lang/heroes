# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/records/done/`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **panel 184** | ratify, amend or overturn R1 to R8: the brace written both ways in an `f` literal and a lone `}` refused; `unused_binding` pointing at the literal that holds its name; no refusal of a forgotten `f` yet, with the question whether design.md §1.3's locality test reaches a rule of legality; a statement after a jump refused and the return rule stated; the compiler's passes on a thread of its own stack; a floor, not a ceiling, for depth | `docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md` § The resolution

    Until it is answered, the compiler goes on as today: `f"{{x}}"` prints
    `{x}}`, a forgotten `f` compiles and prints its braces (17 of 25 sites),
    a statement after a jump is silent, and a deep source aborts the compiler
    at a depth the machine decides. Two blind readings that would settle R3
    and R4's last question are sized and not run (the critic's § E, items 6
    and 7, one session each, a 3 USD cap).

- [ ] **panel 183** | R3 could not be made true: accept its shapes as the rule's known cost, reword the opener's message, or take a narrower R3 | `docs/panel/183-the-reach-of-an-unclosed-opener-ends-at-a-line-no-bracket-can-hold.md` § Author's verdict, the landing · `tests/golden/check/panel-183-a-statement-inside-a-bracket-closed-below-is-the-rules-known-cost.hero`

    Until it is answered, the compiler goes on as landed: a statement word
    at or left of its statement's margin, inside a bracket the author
    closes below it, makes the opener read *never closed* and costs 2 to 5
    messages where 1 stood before R1 (measured on `3cc3b553`, 2026-10-01).
    Three routes: (a) accept the cost as pinned; (b) reword only, the
    opener's message naming the line and the word where its reach ended,
    a sentence that would be true in Task 2 and in the shapes alike, the
    count unchanged; (c) a narrower R3, for instance *the closer that
    pairs lies inside the statement the word begins*, which would tell
    `g5`, `g6`, `g7` and `b_if_col0` apart from Task 2, whose closer stands
    in a later statement, and not `m1`. (b) and (c) are unrun and have no
    prototype.

    **Recommendation: (b), and (c) prototyped in the recovery lane before
    any sitting sees it.** The reason: the harm measured is a false
    sentence, *never closed* of a bracket the author closed, which
    design.md §4.17 makes a defect whatever its frequency, and (b) removes
    it with no rule changed; the extra messages are the rule's cost, and
    the only measurements of their reach are 0 of 1,394 tracked files and
    0 of 13,594 single mistakes, so (c) is worth a prototype, not yet a
    sitting. Conservative is (a).

*******************************************************************************
