# Defect 104 closed: a line inside brackets breaks by how it ends, the compiler breaks it one way everywhere, and the spec says so

2026-09-27, M-agreed-retention step 21, in lane `74203a64` (a detached worktree
at the trunk's `2b1a1f24`), merged `49197fc9`. Found by panel 179's completeness
critic measuring which generated variants do not parse; reproduced by the
coordinator on the trunk at `fa813325`; ruled by panel 180 (provisional,
author ratification pending), whose resolution R1, R3, R4, R5 and R6 this
record closes. Defect 106, the silent class the sitting found beside it,
closes in the same commit and has its own record.

- [x] **104 — the spec says a NEWLINE inside brackets may fall between any two tokens, and the compiler refuses a break before an operator, a `:` or a `,`** | spec § 0 ends *Inside `(` `[` `{` a NEWLINE never ends a statement: where it separates, a production writes it; elsewhere it may fall between any two tokens*, but the lexer plants a terminator after a line-ending token inside brackets too (design.md §4.15, *terminators, which are inserted unchanged everywhere, brackets included*, which §4.9's one-element-per-line literals rely on), so `x = (1` then `+ 2)` is `expected_group_close`, while `x = (1 +` then `2)` checks clean; a reader who follows the spec writes a program the compiler refuses | `spec/heroes-spec.md:11-12` · `docs/design/design.md:1939-1946` · `selfhost/grammar_expr.hero:161` (`ends_the_expression`) · **closed 2026-09-27**

    **Origin:** panel 179's completeness critic, 2026-09-26, measuring which
    of the seats' generated variants fail to parse; filed by the coordinator
    at `605946f9` after reproducing `x = (1` over `+ 2)` at exit 1 and
    `x = (1 +` over `2)` at exit 0 on the trunk at `fa813325`.

    **Why the document was the false party, and why the compiler moved
    too.** design.md §4.15 and the lexer agree that a terminator is planted
    after a line-ending token at any depth (panel 007), so the sentence was
    false the day it entered at `e497646a`. Panel 180 measured that the
    compiler's own rule was not the uniform one either: of the
    compiler-engineer's 117 break shapes the trunk accepted 68, under Go's
    last-token rule with seven exceptions, three of them accidents no
    production needed (an index's `]`, every type's closer and a function
    type's `,`, and a line end before a literal's `,`). The sitting made the
    compiler uniform (R1), the refusal say why (R3), and the sentence true
    (R4), and vetoed route (c), making the old sentence true, because it
    turned `print(f(a` over `-1))` from a refusal into a sum at exit 0.

## The repair

- **A line end may stand before every closing bracket and before every `,`
  (R1)**, and in a list a line end and the `,` after it are one separator.
  `selfhost/grammar_expr.hero` skips the line ends before an index's `]` and
  reads the line end before a literal's `,` (`separator`);
  `selfhost/parse/type.hero` skips them before `[T]`'s `]`, `{K: V}`'s `}`, a
  function type's `)`, its parameters' `,`, and `T[n]`'s `]`. The 14 shapes of
  the three accidental exceptions move to accepted and nothing moves the other
  way (the map below).
- **A line end before what would have gone on with the line above names
  itself (R3)**, `line_end_before_continuation`, unprototyped at the sitting
  and designed here: after a line that ends where a line may end, a binary
  operator, `.`, `::`, `?`, `(`, `[`, or the `:` or `->` a production writes
  there, in a group, an index, a call's arguments, a literal (before a token
  no element begins with), a map entry's `:`, a type's brackets, a function
  type's parameters and `->`, and a signature's parameters and their releaser
  sets. The message: *a line end stands before `+`: the line above ends with a
  number (`1`), which keeps its line end, and inside brackets a line end may
  stand only before a closing bracket or a `,`; to break the line here, end
  the line above with `+`*. The fix moves the token to the end of the line
  above, keeping every comment where it was: `certain` where going on is the
  only reading, and in a call's arguments before `-`, `.`, `(` or `[`, which
  can also begin an argument, two `guess` fixes, the move and *a new argument:
  write the `,`*. What is left of the bracket is passed over to its closer, so
  a break costs one diagnostic where the trunk gave two in 6 of the map's
  refused shapes. New modules `selfhost/parse/line_end.hero` (247 code lines)
  and, for the literal, `selfhost/parse/list_line.hero` (147, shared with
  defect 106's refusal), out of the knot.
- **The spec says what is true (R4).** The opening paragraph's sentence is
  the sitting's adopted text, the llm-ergonomist's second reading of the
  critic's T1, and § 1's string clause is *strings any but a raw carriage
  return or line end: a string is one line*; design.md §4.15 gains the ruling
  on a break BEFORE a token inside brackets, R1 to R3 with panel 180's number.
  +101 vendored and +138 real (6693 to 6794, 8861 to 8999 on
  `claude-opus-5`, digest `072437a576dbb74c`), the sitting's own priced
  figures, with the ledger row in `docs/measurements/010`.
- **The instrument that keeps the sentence true (R5).** The
  compiler-engineer's 117 shapes are `tests/golden/surface-fixtures/brackets180/`,
  each a `surface` row running `heroes parse` whose exit the fixture's header
  derives from the adopted sentence's clauses; the 36 refused ones carry their
  diagnostics as marks, which `annotations` compares, and the 81 accepted ones
  have no `main`, so that sweep, which checks a program whole, does not build
  them.
- **Two comments R1 made false are true again (R6)**:
  `selfhost/print/breaks.hero`'s `restored` and the header of
  `tests/golden/surface-fixtures/comments101/indexparens.hero`, with the
  `surface` row that quotes it.

## What it does not change

- **The lexer.** Every line end is planted where it was.
- **The refusals that are not about brackets**, now stated: a string and an
  `f"..."` hole are one line (`unterminated_string`), and `if` and `match`
  carry a block no bracket can hold (`missing_body`, `missing_match_arms`).
- **A name or a mark word after a line end** (`(p: ptr` over
  `counted_by n, …)`, `counted_by` over `n`, `cstr` over `lent`): refused as
  before, with the message they had, since a name can begin the next
  parameter and R3 names only a token that cannot.
- **`fmt`.** No printer change: its output parses under the new rule, the
  `canonical` suite and every `fmt` row green.

## The measurements

The compiler-engineer's map (`cases.py`, `run.py`) on the lane's final
compiler against the trunk's `trunk.tsv`: 81 accepted and 36 refused against
68 and 49; the 14 shapes of R1 to accepted (an index's `]` ×3, a Place's `]` ×2, a
literal's `,` ×2, `[T]`'s `]` ×2, `{K: V}`'s `}`, `T[n]`'s `]`, a function type's `)` ×2 and its
parameters' `,`), one to refused (row 55, defect 106's); every exit equal
to the prototype's `bii.tsv`; 25 refused shapes with a new first code, 23 of
them R3's.

Gate in the lane, the compiler from its regenerated seed: the compiler's 760
tests; the net's own 168; grammar 9, spec 20, special 10, surface 284,
annotations 195, fixes 26, check 153, canonical 2, layout 3, order 3, records
24, lines 207, run 206, emission 628, determinism 236, corpus 55, warnings
267, each 0 failed; the full net 2687 passed and 3 failed, the three `run` cases at `--sanitize` whose goldens expect AddressSanitizer's `bad-free` summary (c-frees-a-lease-and-the-runtime-names-it, -on-a-later-call, -through-a-callback): the report was cut after its first line, on a run that read real 2245 s against 810 user while another lane ran lldb and docker. All three pass in the named `run` suite before and after it, 206 and 0 both times, and the first alone prints the summary; they are runtime FFI cases no parser change reaches, and the cause is not established. The seed regenerated
(`aaac1fd48d3a2fe9`), and the fixpoint held. `heroes check
selfhost/main.hero`, alternating: the trunk 26.69 and 26.61 s user, the lane
26.43 and 26.46, real within 0.6 s of user plus sys in all four.

On the trunk after the merge, `49197fc9`: lane 104's compiler emitted the
merged source and the seed that emission builds emits the same bytes again
(sha256 `dcdd65fe0416a689`); the compiler's 765 tests, the net's own 171;
the full net **2691 passed and 0 failed**, the three sanitiser cases among
the passes, so the lane's three are not reproduced. Then `70db9f6c`,
`spaced_minus_element` on the thesis list with the author's instruction in
CLAUDE.md. The platforms, measured after the merge's commit rather than
before it, which is the order `.claude/rules/platforms.md` asks and was
not kept here: Linux arm64 on the trunk at `70db9f6c`, the compiler's 765
tests, surface 284, canonical 2, annotations 196, check 153, fixes 26,
layout 4, lines 203, run 202, grammar 9, spec 20, every one 0 failed; the
Windows box on the same tree, the compiler's 765, surface 277, canonical 2,
annotations 196, check 153, fixes 26, layout 4, lines 201, grammar 9 and
spec 20 at 0 failed, and run 199 and 1, defect 115's case, repaired in the
lane merged next (`f08b192d`).
