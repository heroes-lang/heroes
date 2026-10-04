---
kind: decision
area: none
milestone: none
filed: 2026-09-25
commit: 517b8e25692a4cf0075c04fd306e5017fd428cd4
github: none
---

# An end a call announces is pending until the call returns

2026-09-25 | the live set counts, per address, the ends a call has announced
before C runs (`hero_handle_ending`, `hero_handle_transferring`) and not yet
made (`hero_handle_ended`) or withdrawn (`hero_handle_kept`, when a `when`
clause or a NULL handle result says the call failed); a call announcing more
ends than the address holds references is stopped before C, with a message
naming the shape and the shim | panel 176 ruled defect 084 a message and a
shim, and ABI 24's first shape read the set before C and mutated it after, so
`SSL_set_bio(s, b, b)` passed both checks, the library freed twice, and the
runtime spoke afterwards (the landing review, 2026-09-24); a reference-counted
object at two consuming positions of one call holds two references and runs |
design.md §1.12, robustness, and Part 7's runtime | panels 176 and 177, their
landing
