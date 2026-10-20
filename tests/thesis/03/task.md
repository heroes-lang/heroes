# Task 3: bracket depth

The program receives one argument: a string. Three kinds of brackets may appear in it, `()`, `[]`
and `{}`; every other character is ignored. The brackets are balanced when every opening bracket is
closed by a bracket of the same kind, in the right order, and nothing is left open.

If the brackets are balanced, print the greatest nesting depth reached, as a number, and exit with
status 0; a string with no bracket prints `0`. If they are not balanced, print `unbalanced` and exit
with status 1.

Example. The argument `a(b[c]{d})` prints `2`; the argument `(]` prints `unbalanced`.

canary heroes-task:e572092da9bba53299296496541445da
