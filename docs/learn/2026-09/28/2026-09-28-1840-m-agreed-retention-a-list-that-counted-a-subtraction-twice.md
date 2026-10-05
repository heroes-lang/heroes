- [ ] **M-agreed-retention walkthrough** | `base = 10`, `qty = 3`, `discount = 5`, then `totals = [base * qty` on one line and `- discount]` on the next. Before defect 106's repair this was `check` 0 and ran at exit 0. **Before reading: how many elements did `totals` hold, which of the two lines' ends decided it, and what is the one mark in the text that says which reading the author meant?** | `tests/golden/check/fixedbugs-a-list-split-a-subtraction-broken-before-its-minus.hero` · `selfhost/parse/list_line.hero` (`spaced_minus_element`) · spec § 0

    **Where to look after answering:** two, `30` and `-5`. The lexer ended
    the first line after `qty`, which can end a line, and inside a list a
    line end with no `,` is the separator, so the second line began a new
    element and `- discount` was read as a negative one. The break is PEP 8's
    and Black's, so a reader coming from Python writes it on purpose. The
    only evidence of intent is the gap: `-discount` touching its operand is a
    negative element, `- discount` set apart is a subtraction broken before
    its operator, and the repair refuses the second in every context a line
    end separates (a list, a map's key, a map's value), with both readings
    offered as `guess` fixes, since both parse.

    **The question to carry away.** Panel 180 found this while measuring a
    sentence of the specification, *elsewhere it may fall between any two
    tokens*, that was false in the other direction (defect 104). Say why an
    exit-0 wrong answer outranks a refusal of a correct program in this
    language's order of harms, and where CLAUDE.md puts that order.
