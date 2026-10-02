# Panel 185, the shared brief: five questions, one sitting

Convened 2026-10-02 by the coordinator on the author's answer of 2026-10-01
(*2a*, `docs/records/log/2026-10-01-2224-six-answers-panel-184-ratified-panel-185-convened-the-openers-message-reworded-the-blind-seats-command.md`),
the fifth question added by the author's condition on panel 184's (1b)
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`
§ Author's verdict). **The frozen tree is the trunk at `03e70520`**
(`git log -1` read clean, `git status` empty, at 00:41 on 2026-10-02). Every
figure below names the command that produced it on that tree, the coordinator
running it while writing; a figure taken from a record is marked *recorded*,
with its source, and is a seat's to re-measure, never a premise.

## How every seat works
- **Your own directory**, `<scratchpad>/185-<seat>/`, a copy of the frozen tree
  (`git archive 03e70520 | tar -x`), where `<scratchpad>` is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
  Build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes` (seconds); a compiler from `selfhost/` is
  `./heroes build selfhost/main.hero -o heroes-next`, about a minute. Never use
  the repository's `./heroes`, never build or run in another seat's directory,
  never edit the repository. A program built from outside the tree needs
  `HEROES_RUNTIME=<your copy>/runtime`.
- **Your report is a file you write as you go**,
  `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/185-reports/<seat>.md`,
  so a stalled seat leaves what it had. Its headings, per question you judge:
  `verdict` (approve, object or veto, per route), `section` (the design.md or
  spec section it touches), `cost` (lines of compiler, spec tokens, programs
  moved, each measured), `prediction` (falsifiable, with when it is
  checkable), `condition` (what would change your verdict). English, no em
  dashes.
- **The resolution the sitting adopts is the most robust and complete one,
  never the cheapest and never a compromise** (CLAUDE.md § 4 and
  § Precedence); say which route that is in your judgement and what a
  conservative one would be.
- **No paid run**: no `claude -p` session, API call or cloud run, except the
  ones this sitting's briefs name (the blind seat's one session, below). One
  you find worth running goes in your report with its size.
- **Long commands** run in the background, polled with short calls; one
  blocking call of ten minutes kills a session. Never more than three
  processes at once: three repair lanes work on this machine.

## The five questions

**Q1. A macro-only C name in an `extern` group** (defect 143; and defect 145's
case where clang shows no spelling of the type). `function WEXITSTATUS(status:
i32) -> i64` in `extern "sys/wait.h"`: `build` exit 1, *error[ffi_unknown_name]:
`sys/wait.h` declares no `WEXITSTATUS`* (run by the coordinator at 00:41 on
`03e70520`, `<scratchpad>/p184/ffi-side/macro.hero`). The header defines it as
a macro. design.md §1.11 (`docs/design/design.md:565`): *Macros, `inline`
functions and `#define` constants are now reachable directly*. The probe line
is `selfhost/emit/extern_probe.hero:171`, `(void)(name)(args)`, which panel
092 (`docs/panel/092-the-check-that-a-macro-was-hiding.md`) chose so that a
`_FORTIFY_SOURCE` macro cannot hide a wrong signature, recording the
macro-only case as its known cost. Lane emit's measurements (recorded, its
report of 2026-10-01, `<scratchpad>/lane-emit/pass1/143/`): the parenthesised
call checks every name with a declaration and cannot reach a macro-only one;
the plain call reaches it and checks no parameter width; `#ifdef` tells
neither apart; on macOS `WEXITSTATUS((int32_t)0)` fails because `_W_INT` takes
its argument's address. Routes:
- **(1a)** a second clang round for a name the first round reports undeclared
  and `#ifdef` finds defined: its result probed through the call form with an
  lvalue zero per argument (`(T){0}`), its parameters stated unchecked in spec
  § 13, as panel 092 stated the pointee gap (lane emit's recommendation);
- **(1b)** a macro-only name refused, with a message that says it is a macro
  and names the repair design.md §1.11 already names (a C shim compiled by
  `heroes cc`), and §1.11's sentence corrected;
- **(1c)** a route the seats find.
The same second round would let defect 145's note name the one reading where
today it gives two true ones (`tests/golden/unsupported/fixedbugs-145-a-typedef-with-nothing-else-said-of-it`).

**Q2. A one-line `match` arm** (defect 147). spec § 8
(`spec/heroes-spec.md:242-243`): `Inline = ( Expression | "return" [
Expression ] | "break" | "continue" | "assert" Expression ) NEWLINE`.
design.md §4.7 (`:1230`): *An arm's body is one statement, inline, or an
indented block* (panel 014). On `03e70520`, `.blue => n @ 5` and `.red =>
while n < 3` over an indented body both check, build and run, printing 5 and 3
(`<scratchpad>/p185/inline/a54-statement-arm-mutation.hero`,
`a76-inline-while-arm.hero`, run at 00:41). And the spec disagrees with
itself: its prose, `spec/heroes-spec.md:227`, says *An arm's body is one
statement, inline, or an indented block*, as design.md does, and only the
production is narrower. Routes: **(2a)** the production
says what the design says, a statement on the arm's line, a declaration
excepted (design.md §4.7: *Declarations stay out of inline arm bodies*);
**(2b)** the parser refuses what the production does not list; **(2c)** the
production gains exactly the one-line statements (a mutation), and a `while`
or any head that opens a block is refused after `=>`.

**Q3. A value block whose last statement leaves on every path.** `x = match
k` with an arm whose block ends in a `match` or an `if` whose every branch
`return`s: `check` exit 1, *error[no_value]: this `match` produces no value —
every branch jumps, so there is nothing to bind*, at the inner statement
(`<scratchpad>/p185/inline/a55-value-arm-block-inner-returns.hero`,
`a69-value-arm-block-if-returns.hero`,
`a73-value-arm-block-ends-match-stmt-then-nothing.hero`, each exit 1 at 00:41).
design.md §4.7 (`:1236-1255`): a jump is admissible as an arm body, and *a
`match` all of whose arms jump produces no value and is legal only in
statement position* (panel 017 R1). Routes: **(3a)** a block whose last
statement leaves on every path leaves, so its arm is a jumping arm; **(3b)**
the refusal stands and its message names the repair (the `return` written in
the arm itself).

**Q4. The certain `-` in a pattern.** design.md §4.15 (`:1997-2000`): a `-` set
apart from its literal at an arm's start is refused, and the fix, *the `-`
written against its literal, is `certain`, because in a pattern a `-` can only
be its literal's sign* (defect 123; panel 180's ratification). Lane 135c's
probes (`<scratchpad>/lane-135c/next/P5/`), re-run at 00:41: arms written as a
bulleted list, `- 1 =>`, `- 2 =>`, `- 3 =>` (`s5a_bulleted_ints.hero`), take
the certain fix and the applied program builds and prints `many`, where the
meant program (`s5a_bulleted_ints_meant.hero`) prints `two`; one signed arm
among plain ones (`s5b`) prints `minus one` either way. Beside it, spec § 8's
`Pattern = ... | [ "-" ] ( integer | string | character )` (`:244`) admits a
`-` before a string, which the checker refuses (lane 135c's `s5c`). Routes:
**(4a)** the joined `-` is certain only where no other arm of the same
`match` opens with a spaced `-`, a guess otherwise, with the bullet's removal
offered beside it (lane 135c's rule); **(4b)** both fixes always guesses;
**(4c)** the premise stands and bulleted arms are not a mistake worth a
reading.

**Q5. The forgotten `f`.** Panel 184 ratified (2026-10-01): R1 (`{{` and `}}`
write one brace in an `f` literal, a lone `}` an error) and R2 (`unused_binding`
naming the literal that holds the name); design.md §1.3's locality test,
ruled by the author, speaks of meaning, not legality. Its (1b), a plain
literal refused where a brace hole names a bound binding, landed only if its
blind second reading approved the wording: it objected
(`docs/panel/184-reports/llm-ergonomist-task1-second-reading.md`, *a locality
of compiling*), and approved (1a), a plain literal holding any hole-shaped
`{...}` refused, which panel 184's synthesis records at 34 false alarms and 0
true ones over the tracked files (recorded, its compiler-engineer's census on
the tree of 2026-09-30, not re-run for this brief). On `03e70520`, a program
printing `f"{{x}}"` and `"{x}"` builds and prints `{x}}` and `{x}`
(`<scratchpad>/decide-1001/brace.hero`, run by the coordinator at 00:44), so
R1 has not landed and a forgotten `f` prints its braces.
Routes: **(5a)** panel 184's (1a); **(5b)** its (1b), amended; **(5c)** no
refusal, R2's note alone; **(5d)** a route judged by the literal's own text
alone, for instance a hole whose text is a name or a call or member chain on
names, its false alarms measured; **(5e)** a route the seats find.

## The tree's sizes, run at 00:41 on `03e70520`
`git ls-files '*.hero' | wc -l`: 1,530 tracked `.hero` files;
`git ls-files '*.hero' | xargs grep -l '^extern '`: 412 of them hold a line
beginning `extern `. `./heroes measure spec/heroes-spec.md`: 6,838 on the
vendored maximum, 9,060 `real` (recorded by the instrument, `claude-opus-5`,
2026-09-28; the real count is re-taken by `heroes measure --refresh`, a paid
call this brief does not name), against a ceiling of 10,240.

## The seats, and who reads what
- **compiler-engineer** (`185-briefs/compiler-engineer.md`): all five; builds
  the routes it judges in its copy.
- **ffi-pragmatist** (`ffi-pragmatist.md`): Q1 and defect 145; Q4's
  certainty where a C habit is the reading.
- **spec-warden** (`spec-warden.md`): every spec and design.md sentence each
  route writes, priced on the vendored instrument.
- **historian** (`historian.md`): precedent for each question, by web search.
- **llm-ergonomist** (`llm-ergonomist.md`): one fresh session outside the
  repository, four tasks over Q2 to Q5, its brief in `185-briefs/blind/`.
- **completeness critic** (`completeness-critic.md`): these briefs first,
  before any seat; then the reports, before the synthesis.
