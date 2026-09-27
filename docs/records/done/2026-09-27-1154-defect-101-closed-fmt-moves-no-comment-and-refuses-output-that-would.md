# Defect 101 closed: `fmt` moves no comment the generators can reach, and refuses output that would

2026-09-27, M-agreed-retention step 17, in lane g (`832478b3` to `574711c3`),
merged `316d974f`. Found by the skeptic seat over the repair of defects 096,
099 and 100. This record holds what the four share: the rounds, the guard, the
generators that judged them, and the gate. Defects 096, 099 and 100 cite it.

- [x] **101 — `fmt` moves a comment silently at three more places: off a block-opening line, out of a bracket at a body's end, and away from a blank line it adds between match arms** | a trailing comment on the line that opens a block (`if x > 0  # note`) is printed on the block's first line; a comment trailing the last element of a bracketed value at the end of a body leaves the function and becomes a column-0 remark above the next declaration; and a comment between two `match` arms gains a blank line under it the source does not have. Each at `fmt` exit 0 with the tree the same, so the self-check cannot see it | `selfhost/print/fmt.hero` (the block openers, the bracketed-value printer, the match-arm walk) · **closed 2026-09-27**

    **Origin:** the skeptic seat over the repair of defects 096, 099 and 100,
    2026-09-25, beside its reproducers; reproduced by the coordinator the same
    day on the trunk's compiler at `3b40c60d`, so it is older than that
    repair.

    **The reproducers.** `if x > 0  # only the positive case` over a block:
    `fmt` 0, the comment printed as the block's first line. `return [` then
    `1,` then `2  # the last element` then `]` as a body's last statement:
    `fmt` 0, the elements printed one per line and the comment at column 0
    under the function, above `function main()`, a blank line under it. A
    `# the other values` between the two arms of `return match x`: `fmt` 0, a
    blank line added under the comment.

    **Why it is a defect.** The first two move a comment away from the code
    the author wrote it beside, silently, which defect 095's record calls the
    worse form of a formatter's mistake; the third breaks the rule defect
    096's repair wrote down, that a blank line is printed only where the
    source has one. None changes the tree, so no instrument sees any of them.
    The repair keeps a trailing comment on the line it trails, a comment
    inside a bracket inside the bracket, and prints a blank line between arms
    only where the source has one.

## The repair, as filed

- A comment trailing a line that opens a block, `else` included, stays on
  that line.
- A comment inside a bracket stays inside it, on its element's line.
- `match` arms gain no blank line the source does not have.
- And one shape beside them that the trunk moved at exit 0: a comment on the
  `(` line of a signature written across lines keeps the signature one
  parameter per line.

## The guard, which is why the class is closed and not only the witnesses

Beside the parse, fixpoint and tree checks `fmt` already made on its own
output, it now compares **where each comment sits** in its input and its
output, and refuses at exit 2 with the file untouched when one moved
(`print/anchors.hero`, called from `refuse_output_the_formatter_broke` in
`selfhost/cli/syntax_cmds.hero`): *`fmt` would move the comment on line N of
`<path>` away from the code it was written beside*. For every comment it
compares two things, both asked of the two files and never of the walk that
printed the second:

- **its block**, by the one rule the printer places by
  (`print/owners.hero`, defect 099's record);
- **the tokens `fmt` keeps on either side of it**, in order. Left out of those
  tokens, because `fmt` rewrites them by design: commas, `-> ()`, a
  parameter's and a result's marks, a merged group's second head with the
  lines it is on, and parentheses only one of the two files has.

Where the two kept streams part, which a token `fmt` rewrites and the module
does not know would make them, every comment from that point on is refused,
the loud direction. What it cannot see is written in its header: a comment
moved across only a comma, a mark, a `-> ()` or parentheses one file has; one
moved between two lines of one bracket with no token crossed; and the blank
lines around a comment. This is the same exit-2 refusal `fmt` makes for a
changed tree, one more check inside it; no diagnostic class is added.

## The rounds, and who found what

The lane repaired four times after the filing, each round attacked by a seat
that had not written it. Every finding was repaired at its class and pinned
by a fixture.

| round | commit | it repairs what was found by | the finding |
|---|---|---|---|
| second | `832478b3` | the filings: the skeptic seats over 095's and 096's repairs, the parser seat that repaired 096 | the four defects as filed; the guard lands |
| third | `e0e08d18` | the skeptic seat over `832478b3` | 099's own class still open after a `)` alone on its line; six moves the first guard missed at exit 0; a comment trailing `-> ()` refused where the trunk formatted it; and defect 102, filed and closed apart |
| fourth | `83ac68c1` | the second skeptic seat over `e0e08d18` | a comment inside `-> ()` moved into the parameter list at exit 0 (`k01`), because a comment inside the `()` hid it from the guard; 303 of 7,464 bracket-break variants refused |
| fifth | `574711c3` | the third skeptic seat over `83ac68c1`; panel 179's compiler-engineer; the lane's own continuation-column run | no silent move; 575 refusals of the parenthesis generator in five families; 17 refusals of the bracket-break generator ported to Heroes; three variants of one shape (`qualified.hero` at 90) |

The subjects call the first of these the second round because the first was
defect 095's repair, which found 096. Defect 103, the lexer's quadratic
append, was found by the lane's agent timing its own round.

## What it changes in the canonical form

Measured over every `.hero` file under `selfhost/`, `tests/` and `examples/`
on the trunk at `f37b01b3`, 886 files, the trunk's `fmt` against the merged
lane's, 2026-09-27: **843 print the same bytes, 35 carry diagnostics and both
refuse them at exit 1, none that the trunk formats is refused**, and 8 print
differently:

- five, in `tests/golden/check/` and `tests/golden/unsupported/`, where the
  trunk moves a trailing `#~` annotation off a head line (`for i in
  range(...)  #~ unused_binding`) onto the line below and the lane keeps it
  where it is written, which is this defect's first shape;
- three, the `paramcomment095/` fixtures, where a signature printed across
  lines now puts every line after its first eight columns further in than a
  bracket's lines go elsewhere (`margins.deepened`): a member's parameters at
  sixteen and its closing line at twelve, where they were at eight and four,
  so the closing line stands four past the body's column and nothing in the
  head stands at it. At the body's column a continuation read as the body's
  first line, and a `)` at the head's own column as a statement the block
  then hung under (the third seat's `m3` and `sh3`). It applies to every
  block head `fmt` prints across lines to keep a comment inside it (`if`, `else if`,
  `while`, `for`, `match`, a signature, a constant's head), and over the 886
  files these three are the only ones it changes. design.md §4.15 leaves the
  whitespace inside brackets free (*multi-line calls, signatures and literals
  indent freely*) and the spec names no column for it (`grep` for *continu*,
  *column* and *indent*).

Two more rules of the canonical form came from the rounds and are written in
`print/page.hero`'s list. A run of comments is placed wholly in its last
comment's block, which panel 179 ruled the canonical form (provisional,
ruling (c)). And the author's parentheses around a value with a comment
inside them are kept, with every pair of theirs inside them around the same
value, where the fourth round dropped them and broke the line after the
comment: its output of `ys[(a  # c` over `)]` did not parse (the third seat's
`min1`, 571 of its 575 refusals), because the parser skips a line's end
before a group's `)` and not before an index's `]`.

## The measurements

**The reproducers**, `fmt` on each, the trunk's compiler at `f37b01b3` against
the lane merged with it, 2026-09-27:

| program | before | after |
|---|---|---|
| `if x > 0  # only the positive case` over a block | exit 0, the comment MOVED to the block's first line | exit 0, unchanged, a fixpoint |
| `return [` `1,` `2  # the last element` `]` at a body's end | exit 0, the comment MOVED to column 0 under the function | exit 0, the comment on its element's line, the commas dropped as in every list printed one per line, a fixpoint |
| `# the other values` between two `match` arms | exit 0, a blank line ADDED under the comment | exit 0, unchanged |

**The generators**, the three skeptic seats' Python, run on the fifth round's
final source (seed `933f69e7`) by the lane's agent, each variant judged by
`fmt`'s own guard and by two readers written apart from the compiler (an
owner-block model and an all-token alignment), under a memory guard with two
workers:

| set | variants | exit 0 | do not parse | refused |
|---|---|---|---|---|
| the continuation-column generator over `comments101/`, every variant | 1,899 over 39 files | 1,404 | 495 | 0 |
| the three seats' reproducers, panel 179's 17 and the lane's comma shapes | 826 | 746 | 80 | 0 |
| the parenthesis generator over the fixtures, the gallery and the examples | 1,287, 810 and 22,692 | | | 0 |
| the multi-comment generator over the tree | 8,946 | | | 0 |
| a fixed fifth of the bracket-break generator (seed 20260926, md5 of seed, path and name 0 mod 5, `comments101/` and `paramcomment095/` whole) | 36,875 | | | 0 |

The readers' flags are the three documented classes only: an ascending run
placed in its last comment's block, a doc above a second same-head group
moving into the merged group (defect 100's record), and a comment inside the
`()` of a unit result, which `fmt` keeps written where a comment sits in it.
The 495 of the first row do not parse on the trunk either. Three
sets ran on the fifth round's source before its last repair, the comma rule
(`margins.comma_line`), and were not rerun after it: the parenthesis
generator over `selfhost/` (98,540 variants), the single-comment generator
over the tree (468,719) and the continuation-column generator over the tree
(47,923). Their only refusals were the three variants that repair closed.

**The gate on the merge**, the lane at `574711c3` with the trunk at
`f37b01b3` merged in, seed regenerated from the merged source (`045cc769`)
and the fixpoint held by `cmp`: the compiler's own 756 tests; the net's own 168; the full net 2565 passed and 0 failed, every one of its 24 suites at 0 (check 150, ir 24, emit 8, unsupported 15, run 206, annotations 191, determinism 236, emission 628, descriptors 298, cache 6, units 3, fixes 25, lines 207, corpus 55, warnings 267, records 24, surface 167, special 10, spec 20, grammar 9, layout 3, canonical 2, order 3, runtime 8). On the trunk after the merge, `316d974f`: the tree equal to the lane's, and the compiler's 756 tests.

**Other platforms**, the merged tree copied into each: Linux x86-64, Linux arm64 and the Windows box, 2026-09-27: the compiler's 756 tests on each, the CRLF cases among them; surface 167, 167 and 160, canonical 2, annotations 191, check 150, fixes 25, layout 3, lines 203, 203 and 201, every one 0 failed, the smaller counts on Windows the programs that need a library the box does not have.

**Timing**: the trunk's compiler at `f37b01b3` against the merged one, both built from their own seed with the plain line of CLAUDE.md § Commands, alternated, the machine still, over the trunk's own source. `check selfhost/main.hero`: 24.04 and 24.17 s user before, 24.11 and 24.23 after, the same. `fmt selfhost/check/walk.hero`: 0.21 s user before and 0.28 after, twice each. Over 1, 2, 4, 8, 16 and 32 copies of that file, 0.21, 0.44, 0.96, 2.22, 5.53 and 19.12 s before against 0.28, 0.58, 1.25, 2.80, 6.86 and 21.15 after: a constant factor between 1.11 and 1.33, the same growth, and the same bytes printed at 8 copies. The profile of the 8-copy file puts a tenth of the new compiler's samples in `print/owners.hero`, the rule the printer and the guard both ask. This is the guard's price, landed under CLAUDE.md § Precedence's rule for a guard that closes a class, measured and reported rather than argued. The growth itself is older than the lane and is defect 105's.
