# Panel 184, spec-warden's report

Written as it goes, 2026-09-30 from 17:03. Seat's tree: `git archive a294a6ff`
into `<sw>` =
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-spec-warden/`,
`rm -rf build`, compiler built from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 9.72`).

## 0. The ceiling, grepped today

`grep -n '^### 1.6' <sw>/docs/design/design.md` gives line 253; lines 255-256
read: *must fit in 10240 tokens, measured by `claude-opus-5` through
`POST /v1/messages/count_tokens`*. The payment rule is unconditional (lines
305-314): every amendment owes a named removal or a registered prediction
naming an instrument that exists today.

## 0.1 What this seat can and cannot measure

`<sw>/selfhost/cli/measure.hero:106-116` prints the `real` row only for the
path `spec/heroes-spec.md` and only when the file's digest equals
`pinned.SPEC_DIGEST` (`<sw>/selfhost/measure/pinned.hero:63-64`); otherwise it
prints STALE and withholds. The only route to a new `real` count is
`--refresh`, which calls `count_tokens` over the network, and a paid run is
forbidden to this seat. So **every `real` AFTER number in this report is
unrun**; what is measured is the vendored pair before and after, and the
`real` delta is an inference from them, marked as one wherever it appears.

(Sections 0.x are the measurements in the order they were taken; sections
1 to 5 answer the brief's four tasks; section 6 holds the verdicts.)

## 0.2 The spec before, re-measured in this seat's copy

`cd <sw> && ./heroes measure spec/heroes-spec.md`, exit 0, 17:05:
`claude-legacy 6716`, `cl100k_base 6838`, `maximum 6838`, `spread 122`,
`real 9060` (claude-opus-5, 2026-09-28), *Headroom: 1180 against the 10240
ceiling, but the FFI floor mortgages 60 of it, so what is measured against the
ceiling is 9120*. The coordinator's reading reproduces exactly.

## 0.3 How a vendored delta maps to a real one: measured on the ledger, and not a factor

A Python pass over `<sw>/docs/measurements/010-spec-budget-ledger.md`
extracting every row that states both `**+v** vendored and **+r** on the
reader's instrument`: 34 rows. Of the 28 with v >= 9 and both positive,
r/v runs **1.03 to 1.60, median 1.31**, pooled 1.401 (3952 over 2820). Below
v = 5 the sign itself is unreliable: row 4474 is +4 vendored and -1 real, row
5997 is +4 and -2. The ledger's own appendix of 2026-09-09 forbids converting
a row with a factor, so every real figure this report derives from a vendored
delta is written as the band **v x 1.03 to v x 1.60**, an inference, and
never as a point.

## 0.4 What the lexer does with braces today (run, not read)

`<sw>/probe/braces.hero`, `./heroes check` exit 0, `HEROES_RUNTIME=<sw>/runtime
./heroes run` exit 0:

| source | prints |
|---|---|
| `f"a}b"` | `a}b` |
| `f"{{x}}"` | `{x}}` |
| `f"{{x}"` | `{x}` |
| `f"{{"` | `{` |
| `"{x}"` | `{x}` |
| `"{{x}}"` | `{{x}}` |

So in an `f` literal `{{` is the only brace escape and **a `}` is always
text**; `}}` writes two. **Finding F1, against the shared brief's own wording
of route (1a)**: *braces meant as text in such a place are written
`{{`...`}}` in an `f` literal* is false against today's lexer, in the silent
direction: `f"{{count}} rows"` checks at exit 0 and prints `{count}} rows`. A
reader moving a plain `"{count} rows"` to that spelling gets one stray brace
and no message. The true spelling is `f"{{count} rows"`. Either (1a)'s sentence
says so, or (1a) also makes `}}` write one `}`, which changes what every
existing `f` literal holding `}}` outside a hole prints, and that sentence must
then say it too (counted in section 1 below).

## 0.5 Panel 107, read in full in `<sw>`

Its refusal 3 (`:246-247`): *a depth number in the spec, refused: it would be
false on the day it landed, for a legal program and nearly for this compiler
itself*. Its refusal 2 (`:245`): `main` never runs on a created thread, because
it breaks `examples/sdl/` (macOS gives video to the real main thread only).
Its warden's flip condition for a number (`:53`): *only if a compiler-enforced
minimum exists with a 1,000-frame golden green in all three configurations on
all three platforms*. `Recursion too deep aborts` itself came later, from panel
115 (ledger row 3824, net -6), as a property with no number.

## 0.6 F1 has a history, and the brief's wording repeated it

Panel 121's proposal (`<sw>/docs/panel/121-the-brace-was-already-taken.md:28`)
read *`{{` and `}}` write one brace*. The sentence that landed (ledger row
4203) says only *`{{` writes one brace*, and
`tests/golden/run/interpolation-holes-and-braces.hero:13` pins the difference:
`f"{n + 1} and {{braces}} and {word.len()}"` prints `4 and {braces}} and 5`
(its `.expected`, read). I searched `docs/records/log/`, `docs/records/journal/`
and `docs/panel/` for `}}` and found no ruling that a `}}` in the text writes
two braces. **Journal 029** (`<sw>/docs/records/journal/029-corpus-coverage.md:86-89`)
had already recorded the habit once, in an example's template engine:
*`{{name}}`, what a writer types for a literal `{name}`*, and *Python's and
Rust's format strings double both*. Run in `<sw>/probe/`: Python 3.14.7 prints
`{x}` for `f"{{x}}"` and refuses `f"a}b"` (*SyntaxError: f-string: single '}'
is not allowed*); rustc 1.90.0 prints `{x}` for `println!("{{x}}")`, refuses
`println!("a}b")` (*invalid format string: unmatched `}` found*), and prints
`7 rows` for `println!("{x} rows")`, a literal with no prefix that
interpolates, which is the habit route (1) is about. Heroes prints the stray
`}`, at exit 0. The coordinator's own draft of
route (1a) made the same slip.

## 0.7 What (1a) would refuse in the tracked tree: 32 literals, all false alarms

Method: `<sw>/probe/lits.py` lexes every string literal of the 1,389 tracked
`.hero` files at `a294a6ff` (one line each, `#` comments skipped, escapes
honoured, holes scanned with nested brackets and literals skipped); every plain
literal holding a `{` (375) is turned into `f` + that literal, and those in
which a `{` would then open a hole (170) are each written as
`function main()` / `_ = <it>` and handed to `./heroes parse`, which parses
without resolving names (probe: `f"{zz.len()} {q}"` exits 0, `f"{ return x; }"`
exits 1). **32 exit 0**: the plain literals that (1a), read as *would parse as
an `f` literal with a hole*, refuses. They are in **8 files**:

- **`selfhost/`, 12**: the C emitter's zero initialiser `{0}` six times
  (`emit/body.hero:182` `" = {0}"`, `emit/extern_record.hero:175`,
  `emit/assert_spelling.hero:188`, `:192`, `:277`, `:278`), and six test
  strings holding Heroes source (`lexer.hero:427`, `escape_readings.hero:306`
  twice and `:307`, `lex_interp.hero:148`, `next_line.hero:261`);
- **`examples/template/main.hero`, 20**: a template engine's own inputs,
  `"Hello, {name}."` and its siblings.

**None of the 32 is a forgotten `f`.** So (1a) as the brief words it is panel
121's Principle 0 regression again at a smaller size: the compiler stops
compiling itself until 12 of its literals in 7 modules are rewritten, and the
one example that exists to handle `{name}` text must spell every template
`f"Hello, {{name}."`. Under (1b) the six `{0}` hinge on a word the route has not
chosen: *only where the hole's names are bound* is vacuously true of a hole
with no names.

## 0.8 Question 3, measured: the half of a limit a reader relies on moves with the stack and with the compiler

`depth.py` copied from the briefs directory into `<sw>/probe/`; `./heroes
check` and `HEROES_RUNTIME=<sw>/runtime ./heroes build`, each in a subshell
under `ulimit -s L` (lowering the soft limit, as panel 107 did). Exit codes:

| stack (KB) | 16 nested calls | 32 | 64 | 16 nested `if` | 32 | 64 |
|---|---|---|---|---|---|---|
| 8176 (this Mac's default) | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| 4096 | 0 / 0 | 0 / 0 | **134 / 134** | 0 / 0 | 0 / 0 | 0 / 0 |
| 2048 | 0 / 0 | **134 / 134** | 134 / 134 | 0 / 0 | 0 / 0 | **134 / 134** |
| 1280 | **134 / 134** | 134 / 134 | | 0 / 0 (20: 0 / **134**) | | |
| 1024 | 134 / 134 | 134 / 134 | 134 / 134 | 0 / **134** | 134 / 134 | 134 / 134 |

(cells are `check / build`; at 1280, 12 and 13 nested calls are 0 / 0, 14
is 0 / **134**, 15 is 134 / 134.) The panics
name `checktable.ty_key` (64 calls at 4096), `checkwalk.fallible_constructor`
(16 calls at 1024) and `irflatten.expr` (16 `if` built at 1024).

**The compiler checking itself** (`./heroes check selfhost/main.hero`): exit 0
at 8176, 2048, 1792, 1536 and 1280; **exit 134 at 1024 and at 896**
(`panic: stack exhausted in checkwalk.synth`). Panel 107 measured it at
**exit 0 at 896 KB** on 2026-09-04 (`:104-107`). So the stack this compiler
needs for its own source has grown past 1 MB in 26 days, and nobody set out to
move it.

**The deepest nesting the tracked tree holds today**, `<sw>/probe/nest.py`
(open blocks by indentation where no bracket is open, plus open brackets, plus
open holes, at every token; 1,383 files): **20**
(`tests/golden/check/fixedbugs-a-nesting-bound-was-a-silence.hero:152`, a
17-deep constructor chain), then 16, 14; in `selfhost/` at most **14**
(`selfhost/emit/ffi_lend.hero:97`). That golden checks at exit 1 as expected
at 8176, 2048 and 1280: a constructor with named arguments costs less stack
per level than `g(g(...))`, so the depth that aborts is a property of the
shape as well as of the machine.

## 0.9 Question 2, what each predicate would refuse in the tracked tree (line scanners, an approximation)

`<sw>/probe/afterjump.py`: a line starting `return`, `break` or `continue`
followed at the same indentation by a statement: **1** hit,
`tests/golden/check/continuation-outside-brackets-in-other-words.hero:28`, whose
next line is `+ 1  #~ expected_expression`, an error case and not a statement.
So **(2b) refuses nothing that is tracked**, by this scan.
`<sw>/probe/after.py`, the three shapes the wide (2a) adds:

- a statement after `exit(...)` in its block: **1**,
  `tests/golden/run/exit-status.hero:20` (`print("after")`);
- after a `while true` with no `break` of its own: **4**, three of them the
  `return` today's `missing_return` demands: `selfhost/number.hero:191`
  (`return run`), `tests/harness/strings.hero:242`,
  `examples/interpreter/run/eval.hero:125`, and
  **`selfhost/emit/unread.hero:150-171`**, read: the loop leaves only through
  `return read`, and after it stand `empty: {i64: bool} = {}` and
  `return empty`, two statements no path reaches, written because the rule
  demands them;
- after `assert false`: 0.

So the wide (2a) edits **5 tracked sites, 2 of them in the compiler**, and it
turns a spelling today's compiler REQUIRES (`exit-then-return.hero`, exit 0,
re-run here) into one it refuses.

Re-run on this seat's compiler, all as the brief says: the five
`after-*.hero` programs `check` exit 0; `exit(code: 3)`, `assert false` and a
`while true` as the last statement of an `i64` function each cost
`missing_return` at 1:23, exit 1; `exit(code: 3)` then `return 0` exits 0.

## 1. The sentence each route needs, written and measured

Every sentence below was applied to its own copy of the spec
(`<sw>/drafts/<name>.md`, made by `<sw>/drafts/make.py` and `make2.py`, each
anchor asserted to match exactly once) and measured with `./heroes measure`,
which on a path that is not the spec's prints the two vendored counts and no
verdict. Base: legacy 6716, cl100k 6838 (identical at the spec's own path and
at `drafts/base.md`). **The real column is the inference of 0.3**, 9060 +
vendored delta x 1.03 to 1.60; **no real AFTER count was taken** (0.1).

| route | the sentence, and where | legacy | cl100k | vendored delta | real, INFERRED |
|---|---|---|---|---|---|
| (1a), smallest | § 2, *A literal without the `f` is unchanged* becomes *A literal without the `f` may not hold what would be a hole.* | 6723 | 6846 | **+8** | 9068 to 9073 |
| (1a), true for the reader it refuses | the same, and *`{{` writes one brace* becomes *`{{` and `}}` write one brace* (F1-lang, below) | 6727 | 6850 | **+12** | 9072 to 9079 |
| (1a), keeping *is unchanged* | *A literal without the `f` is unchanged, and may not hold what would be a hole.* | 6727 | 6850 | +12 | 9072 to 9079 |
| (1a) with today's `}` stated | the line above plus *and a `}` is text: `f"{{n}"` writes `{n}`* | 6745 | 6868 | +30 | 9091 to 9108 |
| (1b) | *..., and may not hold what would be a hole naming only what is bound there.* | 6733 | 6856 | **+18** | 9079 to 9089 |
| (1b), unambiguous | *... a hole whose every name is bound there, a parameter or a declaration.* | 6740 | 6864 | +26 | 9087 to 9102 |
| (1c), (1d) | **none** | | | 0 | 9060 |
| F1-lang | *`{{` and `}}` write one brace.* | 6720 | 6842 | **+4** | 9064 to 9066 (at +4 the ledger has seen the real sign flip) |
| F1-strict | *`{{` and `}}` write one brace, and a lone `}` is an error.* | 6730 | 6853 | +15 | 9075 to 9084 |
| F1-spec (today's rule stated) | *`{{` writes one brace and a `}` is text: `f"{{n}"` writes `{n}`.* | 6735 | 6858 | +20 | 9081 to 9092 |
| (2b) | § 8, after the `Loops:` line: *A statement after a jump, in its block, is a compile error.* | 6732 | 6854 | **+16** | 9076 to 9086 |
| (2b), the brief's words | *A statement after `return`, `break` or `continue` in its block is a compile error.* | 6739 | 6861 | +23 | 9084 to 9097 |
| (2a), narrow: today's predicate shared | *A statement after a jump, or after an `if`/`else` or `match` whose every branch ends in one, is a compile error.* | 6750 | 6872 | **+34** | 9095 to 9114 |
| (2a), wide | *A statement after a jump, `exit(code:)`, `assert false`, a `while true` with no `break` of its own, or an `if`/`else` or `match` whose every branch leaves, is a compile error.* | 6772 | 6894 | **+56** | 9118 to 9150 |
| (2a), wide, the `return` rule stated too | the line above plus *A function with a `->` must `return` on every path that reaches its end.* | 6792 | 6914 | +76 | 9138 to 9182 |
| (2a), the `return` half alone | *A function with a `->` must `return` on every path that reaches its end, and no path goes past* (the list) | 6789 | 6911 | +73 | 9135 to 9177 |
| (2c) | **none** | | | 0 | 9060 |
| (2c), today's `missing_return` stated truthfully | *A function with a `->` must `return` on every path that reaches its end, and `exit(code:)`, `assert false` and `while true` do not end a path.* | 6759 | 6881 | +43 | 9104 to 9129 |
| (3a) | § 1, a bullet after the layout bullet: *- Brackets, blocks and holes nest at most 64 deep.* (merged into :270 instead: the same +15) | 6730 | 6853 | **+15** | 9075 to 9084 |
| (3a), saying what it leaves | the line above plus *; a long chain of operators or calls may still exhaust the compiler's stack.* | 6746 | 6870 | +32 | 9093 to 9111 |
| (3c) | *- A source nests at most 64 deep, and each bracket, block, hole, operator and call is one level.* | 6743 | 6866 | **+28** | 9089 to 9105 |
| (3c), chains built flat | *... is a level, and a chain of one operator or of calls is one.* | 6754 | 6877 | +39 | 9100 to 9122 |
| (3b) | **none** | | | 0 | 9060 |

**What each smallest sentence rests on, and why a route with none loses a
reader nothing:**

- **(1a)'s +8 is not enough on its own.** A reader it refuses rewrites the
  literal as an `f` literal with its braces escaped, and the escape a reader
  types is `{{..}}` (journal 029 measured the habit; Python and Rust both
  honour it, run above). Today that prints a stray `}` at exit 0 (0.4). So
  (1a) is true for its own reader only with F1 in the same amendment: **+12**
  with F1-lang, +30 with today's rule written out.
- **(1b)'s word *bound* is narrower in this spec than the route means**:
  `spec:131` says *An unused binding or parameter*, so a parameter is not a
  binding here, and a function or a constant is neither. The unambiguous form
  costs +26. Both forms refuse `{0}`, by vacuous truth, which is the six C
  initialisers of 0.7; to spare them the sentence must say *names something
  bound there*, and costs more.
- **(1c) needs no sentence**: what compiles and what it prints do not move,
  and the spec states no compile diagnostic's words (the brief's own grep
  prints only `missing_key`, a failure code of a `T?`). A reader of the spec
  is right before and after.
- **(2a)'s `return` half needs no words once its refusal half is written**: a
  `return` after a statement that always leaves is refused by that sentence,
  so no reader can believe it owed. What neither half says is `missing_return`
  itself, which the spec has never stated; stating it is +20, and that
  sentence is true only under (2a) (under (2b) and (2c) the true statement is
  the +43 one, which writes the wart into the document).
- **(2b) is cheapest because § 8 already defines *jump*** (`spec:228-229`,
  *A jump (`return`, `break`, `continue`)*): +16 against the brief's
  three-word list at +23.
- **(3a)'s +15 is true of what it refuses and silent about what it does not
  count**: a 250-term `+` chain opens one bracket and aborts (the brief's
  table). The +32 form says so, and what it says is a property with no number,
  the shape panel 115 used for `Recursion too deep aborts`.
- **(3b) needs no sentence**: the spec says nothing about depth, and under
  (3b) nothing about depth is true. Today the silence is the compiler's bug
  under CLAUDE.md § 12 (a program the spec permits does not compile);
  under (3b) the silence becomes exact.

## 2. What pays (design.md §1.6, panel 012 as amended by 046)

**No route breaches the ceiling.** The largest combination priced, (1a) with
today's rule written out (+30), the wide (2a) in its longest wording (+83,
`2a-both`) and (3c) with flat chains (+39), is +152 vendored: **at most 9303
real by the band's upper end (9060 + 152 x 1.60), 9363 with the FFI floor's
60, against 10240.** That is an inference, and the margin (877) is far wider
than any ratio the ledger has seen could close. So no veto of mine is a budget
veto.

**No named removal was found.** Searched: §§ 1, 2, 8 and 9 for a fact stated
twice or made false by one of the routes. The only candidate, *A literal
without the `f` is unchanged*, is already spent inside (1a)'s +8. § 13 is
dense, but its clauses are other sittings' paid rows, and ledger row 2745
records why funding one sitting from another's live clause was refused. So
every priced sentence is paid by a registered prediction, or it waits.

**Are the instrument's counts a Part 11 effect?** Yes in shape: design.md
Part 11 metric 3 (`:3931-3933`) is *mutation operators as data, applied
mechanically to the golden corpus; per-operator kill rate*, and 17 of 25
`forget-f` and 6 of 150 `over-indent` SILENT is exactly that. Two limits,
both measured:

1. **The instrument is not in the repository.** `git ls-tree -r --name-only
   a294a6ff` lists no `instrument/` and no `recovery.py`; the files are plain
   Python in the session scratchpad under `/private/tmp`, and that directory is
   not a git repository (`git rev-parse` fails there). The repository's own
   metric-3 instrument, `heroes mutate`, has 16 operators and neither of these
   two (`<sw>/selfhost/mutate/ops.hero:31-46`). design.md §1.6 admits a
   prediction as payment only if its instrument exists on the day of
   registration. This one exists today; nothing keeps it until the milestone
   that scores it. So a prediction that pays names a compile of committed
   files (the mutants landed as golden cases, which `heroes check` scores) or
   `heroes mutate` once it carries the operator. Panel 183 named
   `recovery.py` in its own gate, but its spec delta was 0, so nothing was
   paid with it; here it would be.
2. **A kill rate is not a frequency.** 25 of 25 caught under (1a) is true by
   construction. It says nothing about how often a model forgets the `f`, nor
   how often it writes `{name}` meaning text, and §1.2 needs both. What the
   committed tree says: **0 forgotten `f`s and 32 hole-shaped plain literals
   meant as text** (0.7).

**The predictions I would accept as payment**, each naming an instrument in
the repository today:

- **(1a) with F1-lang, +12**: at the landing commit, the 25 `forget-f`
  mutants committed as golden cases each give their new code at the literal's
  own line under `heroes check --brief` and `--permissive` (25 of 25, 0
  SILENT); the census of 0.7, re-run with `heroes parse`, reads exactly the 32
  named before migration and 0 after; `interpolation-holes-and-braces` prints
  `4 and {braces} and 5`; and `heroes measure` reads real **≤ 9079**. It is
  checkable at the landing. It pays for the tokens and proves nothing about
  §1.2: only a first-try measurement does that (below).
- **(2b), +16**: the two `over-indent` mutants under a `return`
  (`selfhost/cursor.hero:237` at `a294a6ff`, `tests/harness/cases.hero:45`),
  committed as golden cases, are refused at the moved statement; the other
  four stay SILENT, since the route does not reach them; the census of every
  tracked `.hero` file gains **0** refusals (0.9); real **≤ 9086**.
- **(2a) narrow, +34**: the same two refused, and the if/else probe of the
  brief (`after-if-else-returns.hero`) refused too; `missing_return` changes
  no exit code over the tracked tree; real ≤ 9114.
- **(2a) wide, +56**: its measured reach is the same two mutants. What it adds
  (a forgotten `break` before a statement; the defensive `return` after
  `while true` that it now refuses) has **no operator**: none of the
  instrument's thirteen (`brace-*`, `bracket-open`, `extra-closer`,
  `forget-f`, `indent-tab`, `missing-comma`, `missing-operand*`,
  `over-indent`, `wrong-closer`, by name search of its source) and none of
  `heroes mutate`'s sixteen. A prediction about that reach is registered as
  an observation and pays nothing (design.md §1.6, panel 046).
- **(3a), (3c)**: the one honest prediction is *at `ulimit -s 4096` a 64-deep
  call nest checks at exit 0*, and it is **false today** (0.8). Nothing
  payable exists for them without a change to the stack.

## 3. Question 3 and panel 107: is a parser-counted limit that kind of number?

**It is two numbers, and they are of two kinds.**

- **The refusal half** (*one level more than N is refused*) is a property of
  the text, counted by the parser, and the same on every platform by
  construction. That half is **not** panel 107's kind: nothing about the
  machine enters it.
- **The acceptance half** (*everything up to N compiles*) is the half a reader
  relies on and the only reason to write the number down, and it **is**
  panel 107's kind, for one program instead of all of them: the compiler's
  frames times the stack it was given. Measured in 0.8: 64 nested calls check
  at 8176 KB and abort at 4096 KB; at 1280 KB, where the compiler still checks
  its own source, 16 nested calls abort `check` and 20 nested `if` abort
  `build`; the shape moves it (a 17-deep constructor chain with named
  arguments checks at 1280 KB); and **the compiler moves it**: the stack it
  needs for its own source was under 896 KB on 2026-09-04 (panel 107:104-107)
  and is over 1024 KB today, in 26 days, with nobody aiming at it.

So an N small enough to be true at every stack on which the compiler itself
works is **13 at most, for calls, at 1280 KB** (measured: 13 nested calls
build, 14 abort `build`, 15 abort `check`), and that is below what the tracked
tree already holds (20, and **14 in `selfhost/`**): it would refuse the
compiler's own source. An N that spares the tree is false at the compiler's own floor. **Either
way the sentence is false on the day it lands, for a legal program, at a stack
where this compiler runs**: panel 107's refusal 3 (`:246-247`), reason for
reason, moved from the program to the compiler.

**One mechanism would make the acceptance half uniform too**: the compiler
running its passes on a thread whose stack it chooses (the third of (3b)'s
list). Panel 107 refused `main` on a created thread for programs (`:245`),
because macOS gives video to the process's real main thread and
`examples/sdl/` broke (`:160-164`). The compiler binds no such library, so
whether that refusal reaches the compiler's own passes is a question for the
compiler and FFI seats; this seat did not run it. If it does not, a chosen
stack moves every abort depth out by the same factor on every platform, and
then (3a)'s N can be true. But then (3a) is buying only the conversion of a
residual abort into a diagnostic, and section 4 asks whether that enters the
language at all.

**Does it belong in the spec?** Under (3a) or (3c) it must: they decide which
programs compile, and CLAUDE.md § 12 makes a compiler that refuses what the
spec permits the one with the bug. Under (3b) nothing belongs there, and the
spec's silence, today the compiler's bug, becomes exact. So *whether it belongs
in the spec* is the same question as *whether (3a) enters the language*.

**Today's abort fails §1.12's own test.** design.md:605-607: *what would make
this wrong: an abort that stops a program at a place that tells the reader
nothing, where the crash would have named the cause. Then the check is in the
wrong place.* `panic: stack exhausted in checkwalk.synth` names a function of
the compiler and no line of the source, and exit 134 is outside
`.claude/rules/cli-surface.md:41`'s 0, 1 and 2. So (3d) stands on nothing I can
cite.

**A route the brief did not list, (3e)**: the exhaustion told as the TOOL's
failure. Exit 2 (*the tool could not run*, `cli-surface.md:41`) with the
source position of the opener or link where the stack ran out, the stack size
it had, and the remedy; no language rule, **0 spec tokens**, panel 107's *no
number* kept, and §1.12's falsifier answered because the place is named. Its
feasibility is unrun here: the guard is the runtime's
(`runtime/parts/stack.c`), shared by every Heroes program, whose own abort is
134 by `spec:270`, so the compiler would need its own route (a check of the
remaining stack before it descends, or a handler that knows it is the
compiler). That is the compiler and FFI seats' to measure.

## 3.1 Today's `missing_return` predicate, run, because the narrow (2a) sentence claims to share it

`<sw>/probe/jump/`, `./heroes check --brief`, an `i64` function whose body is
the shape alone: a `match` on a variant whose every arm is `return` **exit 0**;
a `match` on an `i64` with `_`, every arm `return`, **exit 0**; an `if` /
`else if` / `else` chain every branch of which returns **exit 0**; the same
chain with no `else`, **exit 1** (`missing_return` at 1:23); the brief's
`after-if-else-returns.hero` exit 0. So *after an `if`/`else` or `match` whose
every branch ends in one* is today's predicate, with *`if`/`else`* read as a
chain that ends in `else`.

## 4. Principle 0 (CLAUDE.md § 2, design.md §1.0), route by route

design.md:590-591 settles the one argument several routes would lean on:
*§1.12 does not suspend Principle 0 ... "it would be safer" is not an entry
ticket any more than "it would be elegant" is.*

| route | compiler need | thesis, measured | so |
|---|---|---|---|
| (1a) | **no**, and worse: 12 of the compiler's own literals are refused (0.7), so it stops compiling itself until they are rewritten | metric-3 shape: `forget-f` SILENT 17 of 25 to 0 of 25, by construction; the cost side is 32 of 32 false alarms in the committed tree and unmeasured in fresh writing | enters only on a thesis effect half measured: **waits** for the other half |
| (1b) | no | the same detection; §1.3 argues against it (legality set by bindings elsewhere) | **waits** |
| (1c) | not a form: a message | ends advice that, followed, leaves a silent program (6 of 25; the coordinator's task-1 run) | not gated by Principle 0; enters as a diagnostic repair (§4.17) |
| F1-lang | no | removes a silent difference on the Python and Rust habit, measured as a difference (0.4, the goldens), unmeasured as a frequency | enters with (1a), which is untrue for its own reader without it; alone, a thesis argument I accept at +4 with the prediction below |
| (2b) | no | **measured: 2 of 150 `over-indent` mutants, SILENT to refused** | enters on that |
| (2a) narrow | no | the same 2, and one predicate for both rules | enters on that, plus the robustness of one predicate |
| (2a) wide | no; 2 of the compiler's own sites must change (0.9) | the same 2; the rest has no operator | its extra **waits** |
| (3a), (3c) | **no**: the compiler checks itself with no limit (exit 0 at 8176 KB) and nests at most 14 | **none**: no operator plants depth, no metric-2 task needs it; §1.12 is not a ticket | **burden unmet** |
| (3b), (3e) | not forms: a compiler repair (CLAUDE.md § 12) and a tool-surface one (`cli-surface.md:41`) | | not gated |

## 5. The run worth making, not this seat's to start

The binding AFTER count of whatever the sitting adopts: `heroes measure
spec/heroes-spec.md --refresh` on the adopted text, **3 `count_tokens`
requests** (two offset probes and the document, `selfhost/cli/refresh.hero:143-207`).
`--refresh` refuses every path but the spec's (`pinned.hero:106-111`), so it
runs in the landing's own tree. No verdict below turns on it: the margin is
877 tokens at the band's upper end (section 2).

## 6. Verdicts, and the fields the seat owes

Every `spec_token_delta` below is MEASURED on the two vendored tables; every
real AFTER figure is an INFERENCE from the ledger's band (0.3), so each verdict
is **provisional on the real count**, and none of them turns on it.

### Question 1: a literal whose `f` was forgotten

- `verdict`: **(1a) object · (1b) object · (1c) approve · (1d) object** as
  the whole answer · **F1-lang approve** (the shape beside the question, not on
  the ballot) · provisional on the real count.
- `section`: design.md §1.2 (`:190-204`), §1.3 (`:206-217`), §1.0
  (`:112-126`), §1.6 (`:305-327`).
- `spec_token_delta` / `cost`: base 6716 / 6838 vendored, 9060 real (pinned,
  digest matching). (1a) **+8**, **+12** with F1-lang (real inferred 9072 to
  9079); (1b) **+18**, +26 unambiguous; (1c) **0**; F1-lang **+4**.
- `removal`: nothing found in §§ 1, 2, 8, 9, and that is a problem for (1a)
  and (1b); (1c) owes none.
- `needed_for_self_hosting`: **no**; (1a) as worded breaks 12 of the
  compiler's own literals.
- `argument`: (1a) refuses 32 literals in the committed tree and none is a
  forgotten `f`: six are the emitter's C `{0}`, six the compiler's test
  strings, twenty a template engine's inputs. The compiler stops compiling
  itself until twelve are rewritten, panel 121 R2's ground again. Its escape,
  as the brief words it, is false: `f"{{count}}"` prints `{count}}` at exit 0,
  where Python and Rust print `{count}` (both run). 25 of 25 caught is
  construction, not frequency; §1.2 needs both sides and fresh writing has
  measured neither. (1b) sets legality by bindings elsewhere (§1.3). (1c)
  costs nothing and ends advice that, followed, leaves a silent program.
- `prediction`: at (1c)'s landing, the 6 `forget-f` mutants `unused_binding`
  reports today are told at the literal with the `f` as the fix, 0 of the 25
  change exit code, the 17 stay SILENT, and `heroes measure` reads real 9060
  with the digest unchanged. With F1-lang: `interpolation-holes-and-braces`
  prints `4 and {braces} and 5`, exactly 3 tracked literals change output, in
  2 golden files (`interpolation-holes-and-braces.hero:13` and
  `fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero:25` and
  `:26`, whose `.expected` lines 8 and 9 become `{é} 7 {€}` and `é{7}é`,
  Python's answers), and real reads **≤ 9066**. Checkable at the landing commit.
- `condition`: (1a) moves to approve if the llm-ergonomist's fresh
  transcripts show forgotten `f`s outnumbering hole-shaped text literals, with
  F1-lang in the same amendment and the 12 compiler literals rewritten in the
  same commit. I would move (1c) to object if its message fired on any literal
  a program means as text; by the brief's definition it cannot, and the landing
  census should show 0.

### Question 2: a statement after a jump

- `verdict`: **(2b) approve · (2a) narrow approve**, preferred if the panel
  wants one predicate for both rules · **(2a) wide object** · **(2c) object**
  as the whole answer · provisional on the real count.
- `section`: design.md §1.6 (`:316-327`, what a prediction must name), §1.0,
  §1.2.
- `spec_token_delta` / `cost`: (2b) **+16** (real inferred 9076 to 9086);
  (2a) narrow **+34** (9095 to 9114); (2a) wide **+56** (9118 to 9150), +76
  with the `return` rule stated; (2c) 0.
- `removal`: nothing found, and that is a problem only for the wide (2a)'s
  extra 22, which no instrument can score.
- `needed_for_self_hosting`: **no**.
- `argument`: Every jump route reaches the same measured thing: 2 of the 6
  silent `over-indent` mutants, both under a `return`. (2b) says it in +16,
  because § 8 already defines a jump, and refuses nothing tracked. The narrow
  (2a) shares today's `missing_return` predicate (run: a `match`, or an
  `if`/`else` chain, every branch returning) and adds the brief's if/else
  program for 18 more: one predicate, two rules. The wide (2a) spends 22 more
  on a reach no operator plants, turns the spelling the compiler requires after
  `exit` into a refusal, and edits five tracked sites, two in the compiler.
  Right direction, unpaid price.
- `prediction`: at the landing of (2b) or the narrow (2a), the two mutants
  committed as golden cases (`selfhost/cursor.hero:237` at `a294a6ff`,
  `tests/harness/cases.hero:45`) are refused at the moved statement, the other
  four stay SILENT, the census of tracked files gains **0** refusals under
  (2b), and real reads **≤ 9086** ((2b)) or **≤ 9114** (narrow). Checkable at
  the landing commit.
- `condition`: the wide (2a) moves to approve with an operator in `heroes
  mutate` that plants its reach (a forgotten `break` before a statement, or the
  defensive `return` after `while true`) and a prediction scored on it, or with
  a named removal of 22. (2b) moves to object if the landing census finds a
  tracked program it refuses.

### Question 3: how deep a source may nest

- `verdict`: **(3a) veto · (3c) veto**, on Principle 0 and **not** on the
  budget · **(3b) approve · (3e) approve** as the zero-token fallback if
  (3b) cannot land in v1 · **(3d) object** · provisional on the real count.
- `section`: design.md §1.0 with §1.12 `:590-591` (safety is not an entry
  ticket) and `:605-607` (an abort that names no place is in the wrong place);
  panel 107's refusal 3; CLAUDE.md § 12.
- `spec_token_delta` / `cost`: (3a) **+15** (real inferred 9075 to 9084),
  +32 saying what it leaves; (3c) **+28**, +39 with flat chains; (3b), (3e)
  **0**.
- `removal`: none owed by (3b) or (3e); none found for (3a) or (3c).
- `needed_for_self_hosting`: **no**, measured: the compiler checks itself
  with no limit, and nests at most 14.
- `argument`: A parser-counted limit is uniform only in what it refuses. What
  a reader relies on, that everything under N compiles, is panel 107's number
  again: 64 nested calls check at 8176 KB and abort at 4096; at 1280 KB, where
  the compiler still checks itself, 14 abort `build`; and that floor moved from
  under 896 KB to over 1024 KB in 26 days. An N true at the compiler's own
  floor (13) refuses the compiler's own source (14). No compiler need, no
  Part 11 metric, and §1.12 is not a ticket. (3b) needs no sentence; (3e),
  exit 2 naming the place, needs none either.
- `prediction`: if (3a) lands with N = 64 and the stack unchanged,
  `(ulimit -s 4096; heroes check <64 nested calls>)` exits **134**, not 0:
  falsifiable today, and it reads 134 now (0.8). If (3b) lands, the brief's
  table programs at 10,000 exit 0 or 1 under `check` at 8176 KB and at
  1280 KB, and the spec's real count stays 9060. Checkable at the landing.
- `condition`: I lift the veto if a measured compiler need or a Part 11
  effect appears, or if the compiler seat measures (3b) and (3e) both
  infeasible for v1; then I object instead, and require the sentence to state
  only what is uniform (the refusal) with the compiler choosing its own stack
  so the acceptance half holds on every platform.

### The one finding I would not want the synthesis to miss

**F1, the closing brace, found beside question 1 and on nobody's ballot.**
Today `f"{{x}}"` prints `{x}}` at exit 0; Python 3.14.7 and rustc 1.90.0 print
`{x}` (run); the project's own golden pins the stray brace
(`interpolation-holes-and-braces.expected`: `4 and {braces}} and 5`); panel
121's proposal had `}}` write one brace and no ruling since says otherwise;
journal 029 recorded the habit once already; and the shared brief's own (1a)
wording made the slip. Every route that sends a reader to escape braces sends
them into it, silently. F1-lang, *`{{` and `}}` write one brace*, is +4
vendored and changes the output of 3 tracked literals, all in goldens.
Second: the stack the compiler needs for its own source rose past 1 MB since
panel 107 measured 896 KB, which is the whole case against a depth number.
