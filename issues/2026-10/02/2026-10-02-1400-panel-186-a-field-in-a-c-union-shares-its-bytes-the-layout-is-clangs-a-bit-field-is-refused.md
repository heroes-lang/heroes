# Panel 186: a field in a C union shares its bytes, the layout is clang's, a bit-field is refused

2026-10-02, at 14:00, the synthesis of panel 186, sat from 11:08 on the
trunk frozen at `779139d0`, a full panel; provisional, the author's verdict
pending.

## The decision

| | |
|---|---|
| date | 2026-10-02 |
| decision | provisional: R1, the header's layout read from clang as the compiler-engineer built it (a C-judged screen, the dump only for flagged records and only for names, every verdict a C assertion, a control record, macro-reached fields asked by name), its dump bounded for a deep nest by the landing; R2, a record naming two fields that share bytes refused at `build`; R3, one comparison rule for a struct-held union and a union type, refused unless the union's field is an integer, pointer or array of them as wide as the union (panel 077's any-arity rule for a union type changed, two goldens moving; conservative keeps it); R4, completeness by layout, replacing the positional probe, a misspelt field told first; R5, a bit-field refused at `build`; R6, spec § 13's R_build_cover text with B1's clause; R7, the construction-arity form as the robust completion, its home the author's question; R8, the routes not adopted; R9, defect 157 filed; R10, where each is written |
| reason | the one route that builds, censused over 447 files and read on five clangs; no clang reports a designated omission in C; size-covering measured unsound and bit-for-bit covering sound; two blind readings, today's text vetoed by both |
| design.md § | §1.7, §1.11, §1.12, §4.19 |
| panel | 186 |

## What it leaves open

`docs/work/DECIDE.md`'s item `panel 186`: ratify, amend or overturn, and
R7's home, (a) recommended. The landing of R1 to R6 is a lane at the C
boundary, after the item is answered. Filed beside the sitting: defect 157.
