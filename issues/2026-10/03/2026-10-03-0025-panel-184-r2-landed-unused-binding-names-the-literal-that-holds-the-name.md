---
kind: decision
area: none
milestone: none
filed: 2026-10-03
commit: ffaf4cec4ca7f3ff4a182b579e9947010e5aaf4f
github: none
---

# Panel 184's R2 landed: `unused_binding` names the plain literal that holds the name in braces

2026-10-03 at 00:25 by the clock (`date`), lane fbrace records the landing of
panel 184's R2, ratified 2026-10-01
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`),
at `492d35a7` (2026-10-02 23:59). Measured first, it had not landed: `count =
3` over `print("{count} rows")` was told *`count` is bound and never read —
remove the binding, or read it*, no literal named and no fix, and panel 185's
own probe `docs/panel/185-briefs/probes/q5/r2-unused.hero` read the same. No
list held it, so it is no defect's: the coordinator files it.

## The decision

| | |
|---|---|
| date | 2026-10-02 |
| decision | R2 as ratified: `unused_binding` on a name a plain literal of the same function holds in braces gains a note quoting the literal, and, where the binding reaches the literal, the literal written with an `f` as a guess, a loop variable's or a payload's certain rename to `_` becoming a guess beside it (two readings); where it does not reach it, the note alone. A hole is what the compiler's own lexer and parser read once an `f` is written before the quote (`selfhost/plain_holes.hero`, the panel 184 engineer's prototype); a pattern's literal is never read for it, no `f` spelling a pattern, nor the library's |
| reason | the message pointed away from the mistake: six of the instrument's 25 `forget-f` sites cost only it (the sitting's count); rustc has shipped the same note since 1.65 (the historian's). 0 spec tokens |
| design.md § | §4.17 |
| panel | 184 |

## What it leaves open

- Once panel 185's R7 landed after it (`a5fc53de`), a hole naming only what is
  in scope is refused and its names count as read, so R2 speaks where R7 does
  not: a hole that names something bound nowhere as well, and a literal the
  binding does not reach. Its case,
  `tests/golden/check/panel-184-r2-a-name-a-plain-literal-holds-in-braces-is-told-so.hero`,
  holds those shapes, which stand after R7; `"{count} rows"` itself is R7's.
- The round's gate, as for every repair of the lane.
