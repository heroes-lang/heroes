# `fmt` refuses output that moves a comment, as it refuses output that changes the tree

2026-09-27 | beside the parse, fixpoint and tree checks it makes on its own
output, `fmt` compares for every comment its block, by the one rule the
printer places by (`print/owners.hero`), and the tokens it keeps on either
side, in order, asked of the input and the output and never of the walk that
printed the second; a comment that moved is refused at exit 2 with the file
untouched, *`fmt` would move the comment on line N … away from the code it
was written beside*, inside the existing refusal and with no diagnostic class
added | defects 099 and 101 were comment moves at exit 0 with the tree the
same, which no check `fmt` made could see, and a move is the worse of a
formatter's two mistakes (defect 095's record); the rounds of lane g after it
landed, each attacked by a seat that had not written them, found most shapes
as refusals rather than silent moves, and the moves it missed (six in the
third round, `k01` inside `-> ()` in the fourth) became its unit tests; its
blind spots are written in its header | design.md §4.15 | none; panel 179
(provisional) rules the probe that will
judge by this guard and an independent reader together
