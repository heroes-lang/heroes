---
kind: decision
area: none
milestone: none
filed: 2026-09-25
commit: 0a8fd34640b2582bd0768d2ba85e3e982a61bdce
github: none
---

# One C function, one contract

2026-09-25 | two `extern` declarations of one C name in two files must say the
same thing at every position they share at one type, and about a result of
one type, `contract_differs` on the later one; a position declared at two
types is two C parameters (a variadic slot bound twice, panel 094's
`twoarity`) and is not compared; within one module `declared_twice` speaks
first | `keep(s: cstr lent)` beside `keep(s: cstr)` was `check` 0 while the
two disagreed about what C does with the bytes, the shape that broke upstream
Clang's `noescape` on its first day, and measured on every mark at
M-agreed-retention step 1; a C symbol is one identity across every module that
names it, like a tag (`one_tag_one_type`) | design.md §4.19, the FFI; §1.11 |
M-agreed-retention's second item, panel 171's completeness critic; step 12
