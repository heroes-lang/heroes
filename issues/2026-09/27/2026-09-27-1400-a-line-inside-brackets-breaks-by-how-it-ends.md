# A line inside brackets breaks by how it ends, and a list refuses a subtraction it would split

2026-09-27 | inside `(` `[` `{` a line breaks by how it ends, Go's rule at
every depth, and a NEWLINE may stand before every closing bracket and every
`,` (the compiler made uniform: an index's `]`, every type's closer and a
function type's `,` join the rest); where a NEWLINE separates two elements
with no `,`, a line that begins with a `-` set apart from its operand is
refused with two `guess` fixes; a line end the next token cannot continue is
refused naming the line end, with a fix moving the token up; spec § 0 and the
string sentence in § 1 made true, design.md §4.15 given the ruling, the 117
shapes kept as `surface` rows | the spec said a NEWLINE inside brackets may
fall between any two tokens, which the compiler never did (defect 104), and
the compiler had seven accidental exceptions no production needs; the one
exit-0 wrong answer in the question, `[a` / `- b]` read as two elements, is
the spelling PEP 8 and Black teach (defect 106); making the old sentence true
instead (route c) turned `f(a` / `-1)` from a refusal into a wrong answer
and was vetoed | design.md §4.15, §4.9, §1.4 | **panel 180**, four seats and
a critic, provisional
