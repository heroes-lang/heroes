---
kind: defect
area: compiler
milestone: none
filed: 2026-09-27
commit: b02855a922d08319f4be4097d49505d24b6c3eb8
github: none
---

# Defect 108 closed: a comment inside a releaser set is part of no name

2026-09-27, M-agreed-retention step 23, in lane B (`ec483127`), merged `335b6895`, with
defect 107. Found by panel 180's completeness critic beside the
compiler-engineer's `rel_pipe` shape.

- [x] **108 — a comment inside an `extern` member's releaser set becomes part of the next releaser's name** | `acquires h_close |  # either one` / `h_close_v2` fails `heroes check` with `unread_releaser` for `#eitheroneh_close_v2`: `releasers` splits the set's source text and drops only whitespace, and the same reading feeds the checker, the emitter and `fmt`; without the comment the program runs | `selfhost/handles.hero:191` (`releasers`) · **closed 2026-09-27**

    **Origin:** panel 180's completeness critic, 2026-09-27, reproducer in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/critic/probes/releaser/`,
    beside the compiler-engineer's `rel_pipe` shape; loud, not corrupting, as
    far as the critic ran; re-run by the coordinator on the trunk's compiler
    at `29ed5601`: `heroes check` exit 1 with that message, and the control
    without the comment runs (*ended by h_close_v2*). The neighbour of panel
    176's releaser set.

    **Why it is a defect.** A comment changes a program's meaning, here into
    a refusal naming a function nobody wrote; a comment must be inert
    wherever the grammar lets it stand.

## Where a comment can stand in a set, and what each reader did with it

The parser (`selfhost/parse/marks.hero`, `named_marker`) reads a set as a
name and then `| name` pairs, with the cursor, which passes over comments,
and hands on one span from the first name to the last. A line may end
inside that span only where the lexer plants no terminator, which is after
a `|` (`layout.is_line_ender`); after the word, or after a name before a
`|`, the break is refused with or without a comment (`expected_releaser`,
`expected_params_close`), and a result's set has no bracket to break in. So
the places are: a comment trailing a `|`, one on a line of its own after a
`|`, two of them, one whose text holds a `|`, and, outside the span, one
after the set's last name before the list's `,` or `)`, trailing it or on a
line of its own. The span's text was read by one function,
`handles.releasers`, dropping blanks, and by `releaser_spans` for the
carets, splitting on blanks too. Its readers, enumerated from
`grep -rn "handles.releas" selfhost/`:

- `check/releasers.hero` (`releaser_reads`): each name looked up; the
  comment's text joined the next name, `unread_releaser`;
- `check/contracts.hero` (`set_key`): the contract two modules'
  declarations of one C function are compared by; a commented set in one
  and the same set in the other were `contract_differs`, naming
  `#eitheroneh_close_v2`;
- `emit/handle_text.hero` and `emit/handle_before.hero`: the set the
  runtime is handed, `a|b` as a C string, behind the checker's refusal;
- `print/marks.hero` (`named_suffix`), for `fmt` and `dump`: the set
  spelled on one line, the comment inside it swallowing the rest of the
  line, so `fmt`'s output did not parse (`expected_releaser`) and it
  refused at exit 2.

## The repair

- **One scanner** (`handles.releaser_spans`): a `#` ends a name and the
  scanner passes to the line's end, a `|` in the comment included;
  `releasers` reads the names off those spans, so the list the checker
  indexes and the carets it indexes them with come from one pass and
  cannot disagree in length, where the two readers used to split
  differently. Every reader above goes through these two functions and is
  unchanged.
- **`fmt` keeps the comment where it is written** (`print/headline.hero`):
  a parameter's words come from one list in the grammar's order,
  `signature.parameter_words`, each carrying its set as a span
  (`marks.Word`), and a set with a comment inside is printed through the
  same `breaks.lead` a parameter's type takes, broken after the `|` the
  comment follows. A parameter's last line is its last token's now, a set
  across lines included, so the comment trailing that line stays on it,
  where the trunk refused `h_close |` over `h_close_v2  # c` over `)`.
- **The guard sees a comment inside a set** (`print/insets.hero`, new):
  `anchors` leaves every mark out of the tokens it compares, because `fmt`
  reorders the words, so a comment moved past a set's last name had no
  compared token to cross and would have passed at exit 0. Each comment
  inside a set is compared by the word whose set holds it and the set's
  tokens before it, which a reordering of the words keeps and a move
  within the set changes; read from the tree's spans, so a releaser named
  like a word is still a name. `anchors.hero`'s header says so, and its
  list of what the guard cannot see reads *a mark outside a set* now.

## The neighbours, and what each does now

Measured with the lane's compiler at `ec483127` against the trunk's at
`f39836a6`, each program beside its control, the same program with its
comments removed, `check` compared by exit and diagnostic codes, 75 pairs
generated over the twelve set shapes below, on `acquires` (an `@`
out-parameter first and last in its list, and beside a parameter marked
`lent`), `transfers`, `retains`, and two sets in one signature, plus a
result's set and a CRLF copy:

| shape | trunk `check` | lane `check` = control | trunk `fmt` | lane `fmt` |
|---|---|---|---|---|
| comment trailing a `|` | 1, `unread_releaser` | yes | 2 | 0 |
| on its own line after a `|` | 1 | yes | 2 | 0 |
| both | 1 | yes | 2 | 0 |
| after each of two `|` | 1 | yes | 2 | 0 |
| own line, then trailing a second `|` | 1 | yes | 2 | 0 |
| a `|` inside the comment | 1 | yes | 2 | 0 |
| trailing the last name before `)` | 0 | yes | 0 | 0 |
| on its own line before `)` | 0 | yes | 0 | 0 |
| a set across lines, the comment trailing its last name | 0 | yes | 2 | 0 |
| a set across lines, no comment | 0 | yes | 0 | 0 |
| after the word (`acquires  # c` / `a`) | 1, the grammar's | yes | | |
| before a `|` (`a  # c` / `| b`) | 1, the grammar's | yes | | |
| a result's set continued at the member's own margin, `|  # c` | 1, `unread_releaser` | yes | 2 | 2, *cannot keep the comment* |
| the reproducer's CRLF copy | 1 | yes | 2 | 0 |

Every `acquires` pair whose control runs ran with the same output as its
control (14 of 14), which is the emitter's reading: a set handed to the
runtime with the comment's text in it would abort at `h_close_v2`. Every
lane exit 0 was judged by the second skeptic seat's reader (`rd.py`): no
comment moved, and each output a fixpoint with the same tree. Beside them,
by hand:

- **Two modules declaring one C function**, one with a comment inside its
  set (`releaser108/commented.hero`, `contract.hero`): the trunk
  `contract_differs`, the lane `check` 0 and `run` *one contract*.
- **The words out of the grammar's order**, `x: H retains a |  # c` over
  `b lent`: `fmt` prints `lent` before `retains`, the comment still after
  `a |`, and the guard, by the set place, passes it; the reader, which
  keeps the marks in its stream, flags `lent` crossing the comment. That
  is the words' reordering, which panel 177's landing made canonical, not
  a move, and it is a fourth documented class of the reader's flags beside
  defect 101's three.
- **Two sets of one parameter, each with a comment, written against the
  grammar's order** (`retains … |  # one` over `… transfers … |  # two`):
  printed in the grammar's order the two comments would change order, and
  `fmt` refuses at exit 2 (*cannot keep the comment*); the spec lets a
  value take one of the five words, so no program `check` accepts has it.
- **A result's set continued at the member's own margin**: no bracket
  holds that line, and a line continued at a statement's margin is
  design.md §4.15's deferred depth-zero continuation, which the lexer
  accepts (reported with defect 107). `check` treats the comment as inert;
  `fmt` refuses at exit 2 as the trunk did, the file untouched.

## The pins

`tests/golden/surface-fixtures/releaser108/` (README, `rel.h`, `sets.hero`,
`commented.hero`, `contract.hero`) and five rows of
`tests/harness/suite_surface.hero`: `check`, `run` and `fmt` of `sets.hero`,
`check` and `run` of `contract.hero`. The compiler's own tests: `handles`'
*a comment inside a releaser set is part of no name* (eight texts, the
spans of the reproducer's two names), `anchors`' *a comment moved inside a
releaser set is seen* and `insets`' own.

## The measurements

Lane g's generators (`judge3.py` with `rd.py` and `rd2.py`, copied into the
lane's scratch), run by the coordinator on the lane's final compiler under the
memory guard with two workers, 2026-09-27: the continuation-column family
over the 47 fixtures of `comments101/`, `comments107/` and `releaser108/`,
2112 variants, 1557 at exit 0, 555 that do not parse, 0 refused; the
bracket-break family over the 32 new fixtures and the sitting's reproducers,
2547 variants, 1749 at exit 0, 798 that do not parse, 0 refused; the 32
files themselves, 30 at exit 0 and 2 that do not parse. The readers' flags
are the documented classes only: 72 of the owner model's, each the
ascending-run rule (panel 179's ruling c); 25 of the alignment's, each a
comment kept on the same code while parentheses only one file has were
dropped or reduced around it (ruling b), read by hand on four variants and
classified by the tokens crossed for all 25. One printing seen on the way
is the trunk's as well: a comment after `+` in a call's argument is printed
inside a pair of `fmt`'s own parentheses.

## The gate

In the lane, on the final source (the coordinator's, after the lane's
agent stopped at the account's weekly limit): the seed regenerated from it,
the compiler rebuilt, emitted again and compared, `cmp` silent (sha256
`5cda930d88f790f9`); the compiler's 764 tests and the net's own 171; layout
4, surface 177, canonical 2, annotations 191, fixes 25, check 150, records
24 and lines 207, each 0 failed. The agent's own gate, on the source before
its last edit (a line of `print/headline.hero` `layout/concat` flagged,
since spelled in pieces), read run 206, emission 628, determinism 236,
corpus 55 and warnings 267, 0 failed, and the full net 2576 and 0.

On the trunk after the merge, `335b6895`: the one real conflict was the
surface suite's count, lane A's 102 -> 121 and this lane's 102 -> 112 on
the same base, joined as 131 with both histories; the seed emitted again by
the trunk's compiler, fixpoint by `cmp` (`d2751877386f8382`); the
compiler's 785 tests, the net's own 172; the full net **2752 passed and 0
failed** (1464.51 s real, 833.51 user). The Windows box on the merged
tree, sent twice truncated and a third time whole by its checksum: the compiler's 785 tests, surface 306, canonical 2,
annotations 199, check 153, fixes 26, layout 4, lines 205, run 204, grammar
9 and spec 20, each 0 failed.
