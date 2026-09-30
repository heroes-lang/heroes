# Panel 183, shared brief: where an unclosed opener's reach ends

Written 2026-09-30 between 04:55 (the measurement) and 05:02 by the coordinator,
for every seat.
Every number and path below names the command that produced it, run while this
brief was written. **The tree the seats copy is commit `171e8c45`**, the trunk
when the briefs went out (`git log -1` read then); the trunk's working tree
moves during the sitting (a lane's merge is due), so no seat reads it:
each copies the commit with `git -C /Users/joseph/Temp/heroes/heroes-lang
archive 171e8c45 | tar -x -C <its own directory>`, then `rm -rf build`, then
builds its own compiler from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (a few seconds).

**The lane is the full panel** (`.claude/skills/panel/SKILL.md` § Two lanes):
the question is what the lexer does with a mistake, a diagnostic's placement,
and a sentence of design.md Part 4. Seats: compiler-engineer, llm-ergonomist,
spec-warden, historian; the completeness critic read these briefs before any
seat started (`docs/panel/183-reports/completeness-critic-briefs.md`, whose
repairs are in them) and reads the reports after. The ffi-pragmatist does not
sit. **Whether that loses a C-facing half is a question, not a premise**: the
critic ran an `extern` group whose first member lost its `)`
(`probes/extern_member_unclosed.hero`) and found that batch 2 changed what
the author of a binding sees (the members at column 4 are in neither
question's word list); the compiler-engineer answers whether a member line
should end a reach, and the sitting widens if the answer touches a binding.

## Why the sitting is owed

design.md §4.15 (`grep -n "reported at end of file" docs/design/design.md`,
line 1944), panel 007's bullet *Continuation lines: inside brackets only*:

> An unclosed opener is a compile error reported at end of file, citing the
> opener — without that diagnostic one missing `)` would silently swallow the
> rest of the file's layout.

On 2026-09-30 the recovery cluster's second batch landed `41807577` (*Defect
130: a declaration opening its line at column 0 names the brackets a mistake
left open, so a stray closer below hides neither the opener nor the
declaration's mistakes*), on the trunk since the fast-forward to `e5cc73eb`
at 04:46 (`git reflog`). Since then an unclosed opener is reported where a line at column 0
opens a declaration, and that line is read as the declaration; so the
sentence's *at end of file* is no longer true of every program. **Where the
diagnostic is printed did not move**: both compilers print `unclosed_bracket`
at the opener's own position (Task 1 below: `10:10` before and after), so
what the rule changed is how far the opener's reach extends and where it is
found closed, which is what the sentence's *at end of file* describes. **The
coordinator let the rule land on a measurement that nothing that compiles
changes, without the sitting CLAUDE.md § 4 asks before a change to what the
lexer DOES and to design.md Part 4.** This sitting is that one, owed after
the fact: it judges the landed rule on its merits, it can refuse it, and it
is not a retro-record, since the decision was the coordinator's and not the
author's.

## The two questions

**(a)** Ratify, amend or refuse the rule as landed: *the reach of an unclosed
opener ends at the first line at column 0 whose first token is `record`,
`variant`, `constant`, `use`, `extern`, `test`, or `function` followed by a
name; the lexer reports every opener still open there, `unclosed_bracket`,
citing the opener, and lays the line out at depth zero.* And the words that
replace §4.15's sentence.

**(b)** Whether the reach ALSO ends at a line inside brackets whose first token
is `if`, `while`, `for`, `match`, `return`, `assert` or `else`, at any column.
Lane recovery-b2's report raised it: within one body, a stray closer below an
unclosed opener, with no declaration between, pairs with it, so the opener is
never reported and the lines between are read inside the bracket. Its four
reproducers are in this sitting's records, `docs/panel/183-briefs/probes/`
(`u2_p_stray_inside_body.hero`, `u2_q_body_then_stray.hero`,
`u4_k_next_mistake.hero`, `u5_i_stray_in_body.hero`); on the trunk's compiler
`u2_p` prints one `expected_separator` at `3:14` and `u5_i` one
`expected_params_close` at `2:5`, and neither reports the opener.

**And (a) is two things that the question judges together**: the lexer's rule,
and the parser's use of the offsets the lexer hands it (`closes`), which batch
2's later commits added. The critic measured that `41807577` alone, before
those commits, prints `e5cc73eb`'s output on both tasks below and
`c85bccb8`'s on nested openers and on the `extern` group; the
compiler-engineer separates the two with that commit's compiler (its
`selfhost/` built by the `c85bccb8` seed's compiler, about a minute).

## Measured for this brief

**Where words open lines inside brackets.** Command: `python3
docs/panel/183-briefs/probes/reach.py <frozen lexer> <judge> <repo> <out>`,
with `<frozen lexer>`
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/instrument/tree/heroes`,
`<judge>` the trunk's compiler, `<repo>` the repository, `<out>` a scratch
folder for the fences; the lexer is the `c85bccb8` snapshot's compiler (the
critic found it byte-identical to a `c85bccb8` seed build), which PREDATES the rule
(so the measurement does not depend on what it measures), over `git ls-files
'*.hero'` at `171e8c45` and the spec's fenced blocks; depth counted from the
bracket tokens, floored at zero:

- 1358 tracked `.hero` files and 8 spec fences, 915,946 tokens.
- The first tokens of a line at column 0 inside brackets: `}` 33, `function`
  44, `constant` 2, `record` 3, `variant` 3, `test` 1, `use` 1, `extern` 2,
  `)` 11.
- **55** such lines open with a declaration word (`function` counted only
  before a name), **every one in six of the seven cases batch 2 added**, which
  are wrong on purpose: `tests/golden/check/fixedbugs-130-a-declaration-past-a-bracket-never-closed`,
  `fixedbugs-130-a-line-going-on-below-a-bracket-never-closed`,
  `fixedbugs-131-a-brace-habit-never-closed-is-told-once`,
  `fixedbugs-131-a-group-a-call-or-an-index-the-lexer-found-open`,
  `fixedbugs-131-a-head-that-left-a-bracket-open` and
  `fixedbugs-131-a-literal-that-never-closed-takes-its-line` (the seventh,
  `fixedbugs-130-an-orphan-under-a-whole-arm-holding-its-own-mistake`, holds
  none).
- **34** lines inside brackets, in **12** files, open with `if`, `while`,
  `for`, `match`, `return`, `assert` or `else`: eleven `tests/golden/check/`
  cases and `tests/golden/surface-fixtures/brackets180/shape073.hero`.
- **All 15 files named above exit 1** under `heroes check --brief` with the
  trunk's compiler (built from `e5cc73eb`'s seed, which `171e8c45` carries
  unchanged); the script prints a verdict for the 12 statement files, and the
  coordinator's loop over all 15 (`check --brief` on each, exit counted) read
  15 at exit 1.

**What that does not show**: the tracked tree is not every program. Whether
a program that compiles can place one of these words at the head of a line
inside brackets is a question about the grammar, not the corpus, and it is
the compiler-engineer's first task.

## The two programs, both compilers run

`c85bccb8` is the compiler before batch 2, `e5cc73eb` after all of it (the
rule and five later commits); each ran
`heroes check <file>` in the coordinator's scratchpad
(`scratchpad/p183/ab/`). The llm-ergonomist gets these programs and the
outputs label-stripped in its own brief.

*Task 1*, a declaration below an unclosed `[` (two mistakes: the `]` left out
on line 10, the operand left out on line 14):

```
function total(xs: [i64]) -> i64
    t: i64 @ 0

    for x in xs
        t @ t + x

    return t

function main()
    xs = [1, 2, 3
    print(total(xs))

function report(n: i64)
    print(n +)
```

`c85bccb8`: `unclosed_bracket` at 10:10, `expected_expression` at 13:1
(*found `function`*), `expected_expression` at 14:14; exit 1.
`e5cc73eb`: `unclosed_bracket` at 10:10, `expected_expression` at 14:14;
exit 1.

*Task 2*, one body (three mistakes: the `)` left out on line 5, one `)` too
many on line 10, the operand left out on line 11):

```
function double(n: i64) -> i64
    return n * 2

function main()
    total = double(3

    if total > 2
        print(total)

    print(double(total)))
    print(total +)
```

Both compilers: `expected_args_close` at 7:5 (*expected `)`, or `,` and
another argument, found `if`*), `expected_expression` at 11:18; exit 1. The
extra `)` on line 10 is said by neither. Under (b), the prediction, which the
compiler-engineer replaces with a run of its own prototype: `unclosed_bracket`
at 5:19, `expected_end_of_line` at 10:25 (the code a stray `)` gets today,
measured on `print(1))`), `expected_expression` at 11:18.

## Where the compiler does it (at `171e8c45`)

`grep -n` for each name:

- `selfhost/layout.hero:177`, `close_brackets`; `selfhost/next_line.hero:220`,
  `opens_a_declaration`; `selfhost/lexer.hero`, the `closes` field of
  `LexOutput` (lines 46, 119, 124, 131, 150); `selfhost/state.hero:95`, the
  lexer state's `closes`;
- `selfhost/parse/unclosed.hero:47`, `never_closed`, which the parser asks;
- two predicates of the same family, found by the critic:
  `selfhost/parse/headless.hero:185`, a second `opens_a_declaration`, over
  token kinds, with another word list (`function`, `constant`, `record`,
  `variant`), for a block below a failed line; and
  `selfhost/next_line.hero:240`, `starts_afresh`, called at
  `selfhost/open_line.hero:209` for panel 181's depth-zero rule, whose list
  holds `break` and `continue` and not `if` or `match`;
- `selfhost/lexer.hero:215` and `:220` also build a `LexOutput` with `closes`;
- `wc -l`: `layout.hero` 304, `next_line.hero` 289, `parse/unclosed.hero` 328,
  `lexer.hero` 712 (the ceiling is `tests/harness/suite_layout.hero`'s measure,
  not `wc -l`).

## What a resolution owes

The design.md sentence, and a `docs/records/log/` entry; the spec's sentence
if the spec-warden finds one owed (`grep -n -i "unclosed\|never closed" spec/heroes-spec.md`
returned nothing at `171e8c45`; its nearest sentence is line 36's, a string
is one line); for (b), if adopted, its golden cases and the
census of every tracked file in both arms. The resolution adopted is the most
robust and complete one, never the cheapest and never a compromise (CLAUDE.md
§ 4), provisional until the author reads it.
