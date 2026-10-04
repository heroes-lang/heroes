---
kind: defect
area: print
milestone: none
filed: 2026-09-28
commit: 3f4ac02c316be882d8d5b7f66e5c9518eb9a370d
github: none
---

# Defect 121 closed: a comma at the start of its own line with a comment after it keeps its line, and `fmt` formats the file

2026-09-28, M-agreed-retention step 27, in the probe lane (`e8ed8732`), merged
`49e50f34`. Found by `heroes probe` on its first run over the formatter's
fixtures, the bracket-break family; repaired in the same lane, and filed here
at its repair so the register holds what the probe found.

- [x] **121 — `fmt` refuses at exit 2 a comma the author wrote at the start of its own line with a comment after it** | the probe's bracket break after the `1` of `[1,  # c` makes a line that begins with the comma and carries the comment, with nothing between the value and it; `heroes fmt` refused it at exit 2, *this is a compiler bug*, in a list, a map, a call, a record construction, a method call and a signature, 12 variants over `comments101/brackets.hero`, `conditions.hero` and `parts.hero` | `selfhost/print/margins.hero` (`commented_comma`) · `selfhost/print/headline.hero` · **closed 2026-09-28**

    **Origin:** the probe lane's agent, 2026-09-28, the probe's first run
    over the formatter's fixture directories, reported to the coordinator at
    04:17 and in its commit.

    **Why it is a defect.** A verb that says *this is a compiler bug* is one;
    and the line is one the parser accepts, since panel 180 lets a NEWLINE
    stand before a `,`.

## The repair

`margins.commented_comma` keeps such a comma's line in `brackets.element` and
in a signature's parameters (`print/headline.hero`), the list's and the map's
included. The shapes beside it, measured by the lane at exit 0 and a
fixpoint: two such commas, nested lists, CRLF, a comment on the next line, a
function type, an extern member. The fixture
`tests/golden/surface-fixtures/comments101/commacomment.hero`, with a README
paragraph, and a `surface` row pinning `fmt`'s output byte for byte; the
trunk's `fmt` before the merge exits 2 on it.

## The gate

In the lane, with the trunk at `345f167b` merged in: the seed regenerated,
fixpoint by `cmp`; the compiler's 798 tests, the net's own 174; canonical 2,
layout 4, order 3, records 24, surface 325, fixes 26, annotations 199, check
153, emission 636, determinism 240 and the new `probe` 24, each 0 failed.
Linux arm64 on the lane's tree (`e8ed8732`, the `heroes-linux-arm64` image): the
compiler's 798 tests and surface 325, canonical 2, annotations 199, check 153,
fixes 26, layout 4, order 3, lines 207, run 206, emission 624, grammar 9, spec
20 and probe 24, each 0 failed. The Windows box on the same tree, one archive
whose sha256 matched on both sides (`50e1ab6dfa572350`), seed
`6d4f23053439a965`: the compiler's 798 tests and surface 318, canonical 2,
annotations 199, check 153, fixes 26, layout 4, order 3, lines 205, run 204,
emission 602, grammar 9, spec 20 and probe 24, each 0 failed.
