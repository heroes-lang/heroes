# Panel 180: a line inside brackets breaks by how it ends, and a list refuses a subtraction it would split

2026-09-27, M-agreed-retention, at the trunk `29ed5601`, frozen from the
briefs to this synthesis. **Not a retro-record**: defect 104 was filed as a
false spec sentence to repair at a sitting, and the sitting decides the
repair. Briefs: `docs/panel/180-briefs/`. Reports: `docs/panel/180-reports/`;
the historian's (no write tool), the llm-ergonomist's (the harness refused its
file) and the ergonomist's second reading were written out by the coordinator
from their final messages, verbatim, with a header and the coordinator's
scoring appended.

**Four seats and a critic, not five**, on CLAUDE.md § 4's *choosing only the
seats whose input differs* (CL-023): the ffi-pragmatist's input is the C a
binding needs, and no route reaches C. Written so the author can overrule it.
The llm-ergonomist read twice: the brief's three sentences, then the adopted
one, because the critic found the first reading never saw any wording with
*or a `,`* or with the list refusal in it.

**Four errors in the briefs, all found by seats.**
- *Q states the compiler as it is*: false. The spec-warden's 35 probes found
  Q wrong on 7: a type's closers and a function type's `,` refuse a line end,
  and a signature's and an extern's leading `,` parse. The ergonomist's
  10 of 10 against Q scored the reader, not Q's truth (critic).
- *The shared brief's table names two exceptions*: the compiler-engineer's
  117-shape map holds seven (an index's `]`; every type's closer and a
  function type's `,`; a `,` after a line end in a literal; a mark word read
  as a name; the list's silent split; and two that are not about brackets, an
  `f"..."` hole holding no line end and `if`/`match` never standing in
  brackets).
- *Q and R's line-ender words*: both omitted `true`, `false`, `nullptr`, and
  neither says a mark word such as `counted_by` is a name to the lexer
  (spec-warden, critic).
- *R refuses a line end before `,`*: that would make `heroes fmt`'s own output
  unparsable, since `fmt` prints a `,` on a line of its own after a comment
  (six such lines for `comments101/nextvalue.hero`; spec-warden,
  compiler-engineer).

## The proposal

Defect 104 (`docs/work/DEFECTS.md`): spec § 0 ends *"Inside `(` `[` `{` a
NEWLINE never ends a statement: where it separates, a production writes it;
elsewhere it may fall between any two tokens"*, and the last clause is false:
the lexer plants a terminator after a line-ending token at any bracket depth
(design.md §4.15, panel 007), so `x = (1` / `+ 2)` is `expected_group_close`.
The sitting chooses the sentence, and whether the compiler moves.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve (b)+(ii)**; object to (a) alone, (i), (e); **veto (c)** | §1.7, Part 5 for cost (nothing core, parser only); thesis and §1.4 for the veto | (b) +9 -1 lines, (ii) +29, together +38 -1 in `grammar_expr.hero` and `parse/type.hero`; `grammar_expr.hero` 1080 code lines against its 1085 ceiling; every gate run green | `layout` 3/0 at the close, `grammar_expr.hero` at most 1080 (1056 if the refusal leaves the knot), `parse/type.hero` 285, no existing program edited to keep compiling | the veto lifts for a (c) that keeps a missing comma loud; moves to (e) on a measured break-before-operator rate |
| llm-ergonomist | **approve R** in a shorter form S; object to P; no veto | the thesis, locality | S 37 words against R's 60 | the reader's Q column matches today's compiler 10 of 10 (**held**); `deltas = [` / `1` / `-1` / `]` prints 2 (**held**) | if the compiler keeps Q, adopt Q's short form |
| spec-warden | **object, provisional** to Q and R as worded; approve RbComma under (b) | §1.6, §1.2, §1.4 | RbComma +39 vendored; worst +95; 1319 real free | `real` delta between 1.2 and 1.7 times the vendored one (**held**: 1.37 on the adopted text, 1.32 on T1) | veto (c) without a Part 11 effect |
| historian | **approve (b)**; object to (c) as worded | precedent | | the silent `[a` / `- b]` reads as two elements (**held**, prints 2); Odin accepts a group break and refuses a call's (run by the critic: **held**) | changes on a precedent that reads `- b` as a new element for years with no defect |
| llm-ergonomist, second reading of the converged sentence | **approve**, with a rewording that keeps all twelve answers | the thesis, locality | the rewording +30 real over the critic's T1 | its twelve answers match the (b)+(ii) prototype 12 of 12 (**held**); zero silent misreadings at the next harness run | objects if a fresh reader answers fragments 2, 7 or 11 differently, or if a touching `-x` meant as subtraction adds elements in 1% of multi-line containers |

## What the sitting measured

**Today's compiler** (the trunk at `29ed5601`, `heroes parse` and `heroes
run`):
- 68 of the compiler-engineer's 117 break shapes are accepted; the rule it
  implements is Go's last-token rule plus a line end allowed before most
  closers and before a call's `,`, with seven exceptions (above). The critic
  re-ran all 117 on its own builds, row for row identical.
- **A silent class, the one exit-0 wrong answer in the question**: inside a
  list whose elements a NEWLINE separates, a line that begins with `-` is a
  new element. `xs = [a` / `- b]`, `[base * qty` / `- discount]` and
  `{1: 10` / `- 2: 20}` all run with one element too many (coordinator,
  historian, compiler-engineer, critic). PEP 8 and Black break *before* a
  binary operator (historian, with sources), so this is a plausible mistake
  that compiles, which the thesis forbids. `heroes fmt` then prints the
  second line as `-b`, erasing the space that was the only evidence of the
  intent (compiler-engineer).
- The census: 1447 NEWLINE-separated element lines in 68 files under
  `selfhost/`, `tests/`, `examples/`, **none** beginning with `-`, 6 with `[`
  (matrix rows), 1 with `(` (a fixture) (compiler-engineer).

**The routes, prototyped by the compiler-engineer, each in its own copy:**
- (a) the spec lists the exceptions, the compiler stays: 0 lines; it preserves
  accidents no production needs.
- (b) the compiler made uniform: a line end before every closer and before
  every `,`, one separator where it meets a `,` in a list; +9 -1 lines; moves
  exactly the 14 refused shapes of three exceptions and nothing else; no
  program's meaning changes (critic: `[1` / `, 2]` is 2 elements,
  `[1` / `2` / `, 3]` is 3, doubled separators stay loud).
- (c) the spec's sentence made true: +55 -5 lines, two modules past 300, 11
  new `fmt` refusals, and `print(f(a` / `-1))` prints 4 at exit 0 where today
  it is an error: **vetoed**.
- (e) a route no brief named, crossing a line end in single-expression
  brackets and before tokens no element can begin: +121 -5, `layout` red on
  three files, 12 new `fmt` refusals.
- (i) refuse `-`, `(`, `[` opening an element line: breaks the 7 lines of the
  census and the `-1` column.
- (ii) refuse `- `, a minus set apart from its operand, opening an element
  line after a NEWLINE that separates with no comma: +29 lines, moves one
  shape, keeps `-1` columns and `fmt`'s output legal (the critic could not
  make `fmt` print a shape (ii) refuses).

**The prices of the sentence**, `heroes measure spec/heroes-spec.md`, the
vendored maximum and the `real` row by `--refresh`, run by the coordinator on
copies (the refresh prints and writes nothing): today 6693 and **8861**; the
brief's Q 6758 and 8948; R 6740 and 8926; the critic's T1 6762 and 8956; T1
with *at any column* 6767 and 8961; **the adopted text, T1 with the column
clause and the string sentence below, 6775 and 8969, +108 real**; and the
ergonomist's second-reading rewording with the same string sentence, **6794
and 8999, +138 real, the text adopted**, against 1379 of headroom (1319 after
the FFI floor).

## The resolution: `provisional, author ratification pending`

The most robust and complete resolution the sitting measured, CLAUDE.md § 4
and § Precedence. It lands in one lane, as one step, with defect 104 and the
defect this sitting found (106).

**R1. The compiler is uniform (route b).** Inside `(` `[` `{` a NEWLINE may
stand before every closing bracket, an index's `]` and every type's closer
included, and before every `,`, where in a list it and the `,` are one
separator. Nothing the compiler accepts today is refused by R1.

**R2. A list refuses a subtraction it would split (route ii).** Where a
NEWLINE separates two elements with no `,`, the next line may not begin with
a `-` set apart from its operand (by a space, a comment or a line end). The
landing, beyond the compiler-engineer's prototype, owes what the critic
found:
- the rule is keyed on *a NEWLINE that separates without a `,`*: the first
  element and a line after a written `,` are untouched;
- the message is true of each shape it fires on, the sign followed by a
  comment and the sign alone on its line included;
- two fixes, both `guess` (the author meant one of two things and only the
  space says which, the precedent being `selfhost/number.hero:106-109`):
  join the line to the one above as a subtraction, keeping any comment that
  trailed it; or write the `-` against its operand as a negative element;
  each fix, applied, gives a program the rule accepts;
- a new diagnostic class: its golden, its source annotation and its `fixes`
  row (CLAUDE.md § 9).
What stays: `[a` / `-b]` unspaced remains two elements. No style the
historian found spells a continuation that way, and Swift, which keys the
same choice on the same space, reads it as a sign (critic, run). A stronger
rule would refuse the legitimate `-1` column or need `fmt` to print commas.

**R3. The refusal says why and what to do (route f, the critic's).** A line
inside brackets that ends where a line may end, followed by a token that
cannot begin what comes next there (a binary operator, `.`, `:`, `?`), is
refused with a message naming the line end as the cause, not *expected `)`*,
and a fix that moves the token to the end of the line above: `certain` where
that is the only reading (a group, an index, a type), `guess` where a missing
`,` is the other (a call's arguments, a parameter list). **Unprototyped**: no
seat built it, so the landing prices it and the lane's commit says what it
cost; the critic ran Go 1.27.1, which says *unexpected newline, expected )*,
as the nearest precedent for naming the line end.

**R4. The spec says what is true**, and design.md says what it rules. The
sentence at `spec/heroes-spec.md:11-12` becomes the llm-ergonomist's second
reading of the critic's T1, which gives the same twelve answers and removes
four places a reader can misparse (a nested *or* that swallows the list, the
antecedent of *carries one*, *stands only* read as *counts only*, *word* read
as *identifier*), and states the idiom a writer needs:

> Inside `(` `[` `{` a NEWLINE never ends a statement. A line there keeps its
> NEWLINE when it ends with a literal, `?`, `???`, a closing bracket, or a name
> or keyword other than `function` and `fail`; that NEWLINE may stand only
> before a closing bracket or a `,`, or where a production writes it, and any
> other line goes on below, at any column, so a long expression breaks after
> an operator. Where a NEWLINE separates without a `,`, the next line may not
> begin with a `-` that does not touch its operand.

*A name* covers the mark words, which are names to the lexer; *keyword*
covers `true`, `false`, `nullptr`, `return`, `break` and `continue`. Its one
imprecision, which T1 shared (critic): it says `if`, `match` and the other
keywords keep a NEWLINE where the lexer plants none, and no program that
parses puts one of them at a line end inside brackets, so a reader predicts
a refusal exactly where the compiler refuses. The critic's T1 (+108 real
with the string sentence) is the cheaper alternative, recorded so the author
can choose it; the reading paid 30 tokens for four misreadings and the
idiom, and the author's instruction of 2026-09-27 is not to economise on
tokens where robustness gains. And the shape beside it, found by the critic
and run by the coordinator: spec line 28 says strings may hold anything *"but
a raw carriage return"*, while a raw line feed ends a string too
(`unterminated_string`, *strings are single-line*). That sentence becomes
*strings any but a raw carriage return or line end: a string is one line*.
design.md §4.15 gains the ruling on a break BEFORE a token inside brackets,
which it never had (compiler-engineer, critic): R1 to R3 with this sitting's
number.

**R5. The instrument, so the sentence cannot drift again.** The
compiler-engineer's 117 shapes become fixtures under `tests/golden/`, each
with the exit code the adopted sentence predicts and each line citing § 0,
run as `surface` rows (spec-warden, critic): a sentence and the parser that
disagree then go red on the day they part, which is what nothing did on
2026-09-12 when the false clause entered at `e497646a`.

**R6. Two comments become false under R1** and are corrected in the same
lane: `selfhost/print/breaks.hero:119-121` and the header of
`tests/golden/surface-fixtures/comments101/indexparens.hero`, which
`tests/harness/suite_surface.hero:347` quotes (critic). The compiler-engineer's
prediction is read as *no existing program edited to keep compiling*; a
comment made true is not a program edited.

**What a veto would compel.** The compiler-engineer's veto binds route (c)
only, and the resolution does not take it.

**What conservative would have been**, recorded so the author can choose it:
route (a), the spec listing today's exceptions, 0 compiler lines, sentence Q
corrected to be true (+95 vendored by the warden's count), and the silent
list class left for a later milestone. Refused because it writes accidents
into the one document a reader trusts and leaves an exit-0 wrong answer
standing.

**Found beside the question, filed rather than ruled:**
- the silent list class as **defect 106**, closed by R2;
- **defect 107**: `heroes fmt` refuses at exit 2 a comment after `.` or `::`
  where the only bracket is the author's parentheses, which it drops (the
  compiler-engineer's reproducers, five more members found by the critic);
  the neighbour lane g's fifth round did not reach;
- **defect 108**: a comment inside an `extern` member's releaser set becomes
  part of the next releaser's name (`selfhost/handles.hero:191`,
  `releasers`), so `acquires h_close |  # either one` / `h_close_v2` fails
  `heroes check` with `unread_releaser` for `#eitheroneh_close_v2` (critic).

## Predictions to score

- **compiler-engineer**, at M-agreed-retention's close: `layout` 3/0,
  `grammar_expr.hero` at most 1080 code lines (1056 if the refusal leaves the
  knot), `parse/type.hero` at 285, each within 2; no existing program edited
  to keep compiling.
- **spec-warden**: the `real` delta between 1.2 and 1.7 times the vendored
  delta. Scored at this synthesis: the adopted text +138 real against +101
  vendored, 1.37; T1 +108 against +82, 1.32: **held** on both.
- **llm-ergonomist, second reading**: P1 **held**, 12 of 12 against the
  prototype; P2, zero silent misreadings at breaks and the first-try rate
  moving at most 5 points, at the next harness run that generates such
  programs.
- **llm-ergonomist**: prediction 3, zero silent misparses at breaks inside
  brackets and most loud refusals from a leading operator or a trailing
  comma, at the next harness run that generates such programs (metric 2 is
  frozen until it can run; the critic): **unscorable until then**.
- **historian**: P1 **held** (prints 2); P2 held for Odin and Go (run by the
  critic).

## Author's verdict

Pending. Queued as `panel 180` in `docs/work/DECIDE.md`. Work proceeds on the
provisional resolution: one lane lands R1 to R6 with defects 104 and 106.

## A question the sitting did not ask, ruled by the coordinator, 2026-09-27

The lane that landed R2 (`74203a64`) asked whether `spaced_minus_element`
belongs on the thesis list `check --permissive` drops, the control arm of
design.md Part 11. It does, by the list's own definition: without the rule
the program still has a meaning, the two elements the rule refuses. Landed on
the trunk with the author's instruction of the same day in CLAUDE.md; the
log entry is `docs/records/log/`'s *a split subtraction is a thesis rule*.
The author's ratification of this sitting covers it.
