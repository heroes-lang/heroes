- [ ] **M-agreed-retention golden ratification** | `base: i64 = 100`, `fee: i64 = 7`, then `total = base` on one line and `- fee` on the next at the same margin, then `print(total)`. Before defect 120's repair the second line was refused as `discarded_value`, whose fix, tagged `certain`, wrote `_ = - fee`; `check --apply` then `run` printed 100. **Before reading: the old fix compiled. Which rule of this repository says it was never `certain`, and what does the new golden's `.fixed` file hold instead?** | `tests/golden/check/fixedbugs-a-discard-that-drops-a-term-meant-to-be-subtracted.hero`, `.expected`, `.fixed` · `.claude/rules/diagnostics-and-goldens.md` § Errors are a deliverable

    **Where to look after answering:** *a `certain` fix repairs the defect
    the diagnostic names; a fix that leaves the defect standing is a `guess`,
    however well it compiles.* The defect here is the broken subtraction,
    and `_ = - fee` keeps it: 100 where 93 was meant. Panel 181 ruled that
    outside brackets a line ends its statement, so a line beginning with a
    `-` set apart from its operand is refused as the break it is, once, as
    `continuation_outside_brackets`, and the `.fixed` file holds
    `total = base - fee`. The 2026-09-08 reading of that rule had called this
    very site one that obeys it by construction; it was a reading and not a
    run, and the note under it says so.

    **To ratify:** read the three files and say whether the join is certain
    on this shape. Then look at defect 129, the same
    join on a line holding `-` alone, where two certain fixes applied together
    write a program `check` refuses, and say what a `certain` fix owes when a
    second fix touches the same lines.
