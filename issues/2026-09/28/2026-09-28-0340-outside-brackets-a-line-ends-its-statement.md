---
kind: decision
area: none
milestone: none
filed: 2026-09-28
commit: 405caf90891a1daad7ddba2a132b45319fb76e5a
github: none
---

# Outside brackets a line ends its statement, in both directions

2026-09-28 | at bracket depth zero a line ends its statement: a line whose last
token cannot end one, and a line whose first token can only go on with the line
above (a binary operator, a spaced `-`, a postfix `(` `[` `?` `::` after a
finished line or a closed block), are refused by one lexer diagnostic,
`continuation_outside_brackets`, at the break, one diagnostic per break, the
parser handed the joined line; the join is a `certain` fix where the next line
cannot stand alone and a `guess` where it can; an `error` token ends a
depth-zero line; the code joins the thesis list; the spec gains *"Outside
brackets a line ends its statement: it may not end where the statement cannot,
and the next line may not go on with it, so a long expression, a condition
included, breaks inside parentheses."* and § 1 reads *"a condition needs no
parentheses"*, +62 real | the compiler admitted at the same margin, in every
depth-zero context, what design.md §4.15 and the author's ratification of
panel 007 exclude (defect 116; 82 of 83 shapes compiled), while refusing the
deeper line, a combination the historian found in no language; the critic
found the class in the other direction too, a postfix after a block applied
to its value (defect 119) and a `certain` fix that drops a subtracted term
(defect 120), and `fmt` exiting 2 on the shapes (defect 118); a careful
reader of the spec predicted the refusal on every fragment the compiler
accepted; the longer spec text is the author's instruction of the same night
| design.md §4.15, §4.17, §1.7, Part 11 | **panel 181**, four seats, a
critic and a second blind reading, provisional
