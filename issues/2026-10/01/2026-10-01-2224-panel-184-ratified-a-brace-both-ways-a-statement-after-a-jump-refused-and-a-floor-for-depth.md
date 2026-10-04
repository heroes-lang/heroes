---
kind: decision
area: design
milestone: none
filed: 2026-10-01
commit: 606700e262e9d18483ee44ba7e05cdec2dbafd35
github: none
---

# Panel 184 ratified: a brace both ways, a statement after a jump refused, and a floor for depth

2026-10-01, evening, `/decide` on the author's answer. The item below was
verified at `b39ae3d5` before its verdict was written: `f"{{x}}"` printed
`{x}}`, a `print` after a `return` checked at 0, and the census of every
batch of the day found the compiler aborting on the sitting's two deep blind
files.

**The author's words**: *1a 2a 3b 4a 5a 6a*, the evening's answer to six
recommendations the coordinator put to them at 22:06 with their reasons and
measurements, each item verified against the trunk at `b39ae3d5` first.
**Recorded as a reading**, CLAUDE.md § 4's default; not `by delegation`. The
other answers are in the decision log entry of the same evening.

- [x] **panel 184** | ratify, amend or overturn R1 to R8: the brace written both ways in an `f` literal and a lone `}` refused; `unused_binding` pointing at the literal that holds its name; no refusal of a forgotten `f` yet, with the question whether design.md §1.3's locality test reaches a rule of legality; a statement after a jump refused and the return rule stated; the compiler's passes on a thread of its own stack; a floor, not a ceiling, for depth | `docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md` § The resolution · **ratified 2026-10-01**

    Until it is answered, the compiler goes on as today: `f"{{x}}"` prints
    `{x}}`, a forgotten `f` compiles and prints its braces (17 of 25 sites),
    a statement after a jump is silent, and a deep source aborts the compiler
    at a depth the machine decides. Two blind readings that would settle R3
    and R4's last question are sized and not run (the critic's § E, items 6
    and 7, one session each, a 3 USD cap).

    **Verdict, 2026-10-01: ratified, meant as *1a*.** R1 to R8 stand as the
    sitting states them. R3's question is ruled: design.md §1.3's locality test
    speaks of what a program means, not of what is legal, so route (1b) as
    amended lands after R1, on the condition that the blind reading of task 1
    with its amended wording (the critic's § E item 6) approves the wording;
    the two readings, one session each, capped at 3 USD each, are authorized
    and run once the author has updated the `claude` command (item 6 of the
    same answer). The landing is a lane after the round's gates, in the
    order the sitting's `## Author's verdict` now names.
