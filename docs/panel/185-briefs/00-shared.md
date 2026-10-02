# Panel 185, the shared brief: five questions, one sitting

Convened 2026-10-02 by the coordinator on the author's answer of 2026-10-01
(*2a*, `docs/records/log/2026-10-01-2224-six-answers-panel-184-ratified-panel-185-convened-the-openers-message-reworded-the-blind-seats-command.md`),
the fifth question added by the author's condition on panel 184's (1b)
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`
§ Author's verdict). **The frozen tree is the trunk at `03e70520`**
(`git log -1` read clean, `git status` empty, at 00:41 on 2026-10-02). Every
figure below names the command that produced it on that tree; a figure taken
from a record is marked *recorded*, with its source, and is a seat's to
re-measure, never a premise. **Repaired at 01:15 on the completeness
critic's first pass** (`docs/panel/185-reports/completeness-critic-briefs.md`,
its § G, every item taken; the version it read is
`00-shared-before-the-critic.md`). The evidence is in the tree, under
`docs/panel/185-briefs/probes/q1` to `q5`, copied from the session's
scratchpad and from the critic's probes.

## How every seat works
- **Your own directory**, `<scratchpad>/185-<seat>/`, a copy of the frozen tree
  (`git archive 03e70520 | tar -x`), where `<scratchpad>` is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
  Build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes` (seconds); a compiler from `selfhost/` is
  `./heroes build selfhost/main.hero -o heroes-next`, about a minute. Never use
  the repository's `./heroes`, never build or run in another seat's directory,
  never edit the repository but your report. A program built from outside the
  tree needs `HEROES_RUNTIME=<your copy>/runtime`. The probes below are read
  from the repository and copied into your directory to run.
- **Your report is a file you write as you go**,
  `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/185-reports/<seat>.md`,
  so a stalled seat leaves what it had. Its headings, per question you judge:
  `verdict` (approve, object or veto, per route), `section`, `cost` (lines of
  compiler, spec tokens, programs moved, each measured), `prediction`
  (falsifiable, with when it is checkable), `condition` (what would change your
  verdict). English, no em dashes.
- **The resolution the sitting adopts is the most robust and complete one,
  never the cheapest and never a compromise** (CLAUDE.md § 4 and
  § Precedence); say which route that is in your judgement and what a
  conservative one would be.
- **No paid run**, except the blind seat's one session, named in its brief.
  One you find worth running goes in your report with its size.
- **Long commands** in the background, polled with short calls; never more
  than three processes at once: three repair lanes work on this machine, and a
  Linux container may be running (`docker ps` before you start one).

## The five questions

**Q1. A macro-only C name in an `extern` group** (defect 143).
`probes/q1/macro.hero`, `function WEXITSTATUS(status: i32) -> i64` in `extern
"sys/wait.h"`: `build` exit 1, *error[ffi_unknown_name]: `sys/wait.h` declares
no `WEXITSTATUS`* (run at 00:41), built at `selfhost/emit/ffi_declared.hero:159`.
`check` exits 0 on it: the `extern` probe runs at `build`, `--emit-c`
included, never at `check`. The header defines it as a macro. design.md
promises macros reachable in four places: `:565` (§1.11), `:645` (§3.1),
`:2265-2266` and `:2401-2402` (§4.19); the spec names no macro (`grep -n -i
macro spec/heroes-spec.md` is empty). The probe line is
`selfhost/emit/extern_probe.hero:171`, `(void)(name)(args)`, which panel 092
chose so that a `_FORTIFY_SOURCE` macro cannot hide a wrong signature,
recording the macro-only case as its known cost. A second clang round already
exists for a handle's `struct` tag, `selfhost/cli/assemble.hero:4-24` (defect
037, panel 152 R1), paid only by a program that needs it. On macOS (the
critic, `sys/wait.h:131`): `WEXITSTATUS((int32_t)0)` fails, *cannot take the
address of an rvalue*; `WEXITSTATUS((int32_t){0})` compiles; `(WEXITSTATUS)(...)`
and `(htonl)(...)` are *undeclared identifier*. **The population**: `git
ls-files '*.hero' | xargs grep -l '^extern '` names 412 files; `build
--emit-c` on each (`probes/q1/extern-emit.tsv`, by `probes/q1/emit-one.sh`):
206 emit and 206 do not, and five of the refused are `ffi_unknown_name`, none
a macro (`_IO_FILE`, `_popen`, `_pclose`, `_iobuf`, two misspellings, a local
`describe`): those must stay refused under any route. Routes:
- **(1a)** a second clang round for a name the first round reports undeclared
  and `#ifdef` finds defined: its result probed through the call form with an
  lvalue zero per argument (`(T){0}`), its parameters stated unchecked in spec
  § 13, as panel 092 stated the pointee gap (lane emit's recommendation);
- **(1a')** the compiler writes the wrapper itself from the declared
  signature, `static inline <ret> hero_m_<name>(<C types> a0, ...) { return
  <name>(a0, ...); }`, which checks what (1a) checks (the critic's C2);
- **(1b)** a macro-only name refused, the message saying it is a macro and
  naming a repair a program can use today, with design.md's four sentences
  corrected;
- **(1d)** the repair (1b) would name, measured working on `03e70520`: a
  header of the program's own, `probes/q1/shim/wait_shim.h`, holding `static
  inline int hero_wexitstatus(int status) { return WEXITSTATUS(status); }`,
  bound by `extern "wait_shim.h"`: `heroes run` prints 1 for 256, and declared
  `status: i64` it is refused, `ffi_parameter_type` (`probes/q1/shim/`).
  **`heroes cc` is not built** (`./heroes help`: *Commands not built yet: lsp ·
  cc*), and design.md §4.19 (`:2381-2383`) records shim compilation as
  undecided (panel 036);
- **(1c)** a mark on the line, for instance `macro function`, so the unchecked
  parameters are visible where the reader stands: a surface change, listed so
  the option set holds it (the critic's C3).

**Defect 145, beside it**: where clang shows no spelling of the type, the note
gives two true readings
(`tests/golden/unsupported/fixedbugs-145-a-typedef-with-nothing-else-said-of-it.hero`,
`-a-misspelled-tag`). One extra query splits them (the critic, against
`fixedbugs-145-types.h`: `(void)sizeof(anon_s);` compiles, a typedef name;
`(void)sizeof(only_tagg);` is *undeclared identifier*). Whether it rides (1a)'s
round is a question, not a premise.

**Q2. A one-line `match` arm** (defect 147). spec § 8 (`:242-243`): `Inline =
( Expression | "return" [ Expression ] | "break" | "continue" | "assert"
Expression ) NEWLINE`. Its own prose (`:227`) and design.md §4.7 (`:1230`): *An
arm's body is one statement, inline, or an indented block*. **The parser
already reads a whole `Statement` on the arm's line**, then refuses a
declaration (`selfhost/grammar_expr.hero:1165-1209`, the `arm` function;
`selfhost/parse/arm_body.hero:25`, `reject_declaration`). On `03e70520`:
`.blue => n @ 5` and `.red => while n < 3` over an indented body check, build
and run, printing 5 and 3 (`probes/q2/a54-statement-arm-mutation.hero`,
`a76-inline-while-arm.hero`); the critic found `while`, `for`, `if` and `match`
heads after `=>` all check, build and run (`probes/q2/s07` to `s10`), and in the
tracked files (`grep` counts, not the parser's) a mutation after `=>` on 704
lines of 107 files, 676 of them in `selfhost/`, 600 with no `"`
(`probes/q2/mutation-arms.txt`), and `=> if ` on 3 lines, `=> match ` on 7,
`=> while ` on 1. Routes: **(2a)** the production says what the prose, the
design and the parser say, a statement on the arm's line, a binding excepted:
a spec change alone, moving no program; **(2b)** the parser refuses what the
production does not list: it would refuse the compiler's own source on about
600 lines; **(2c)** the production gains the mutation, and a `while` or a
`for` after `=>` is refused, written as the arm's block (one tracked line
moves, `tests/golden/run/fixedbugs-139-value-arms-that-jump-or-give-a-value.hero:100`).
**Beside it, a question**: `.blue => _ = 0` is refused with *`_` would be
bound where nothing can read it* (`probes/q2/s06-discard.hero`), while spec § 5
says `_` binds nothing and § 8 calls `_ = 0` the arm that does nothing:
whether (2a)'s *a binding excepted* takes `_ = e`, and whether that message is
true.

**Q3. A value block whose last statement leaves on every path.** `x = match k`
with an arm whose block ends in a `match` or an `if` whose every branch
`return`s: `check` exit 1, `no_value` at the inner statement, *this `match`
produces no value* (`probes/q3/a55-value-arm-block-inner-returns.hero`,
`a73-value-arm-block-ends-match-stmt-then-nothing.hero`) or *this `if`
produces no value* (`a69-value-arm-block-if-returns.hero`), each exit 1 at
00:41. The same refusal, the critic found, for an outer value `if` whose
branch ends in a returning `match` (`probes/q3/b1-outer-if.hero`), the inner
`match` as the arm's inline head (`b2`), `return match k` (`b3`), two levels
deep (`b5`), `break` and `continue` in a loop (`b6`), an inline `if` head
(`b8`). design.md §4.7 (`:1236-1255`): a jump is admissible as an arm body, and
*a `match` all of whose arms jump produces no value and is legal only in
statement position* (panel 017 R1). **Panel 184's R4 (ratified, not landed)
counts `exit(code:)`, `assert false` and a `while true` with no `break` as
ending a path**; as value arms on `03e70520`, `0 => exit(code: 3)` draws
`bound_unit` and a `type_mismatch` placed on the OTHER arm
(`probes/q3/c1-exit-arm.hero`, `c6-block-exit-last.hero`), and `assert false`
or a `while true` block draw *this branch ends on a statement* (`c2`, `c3`);
and a `print` after a statement `match` whose every arm returns checks clean
(`c5`). Routes: **(3a)** a block whose last statement leaves on every path
leaves, so its arm is a jumping arm; **(3b)** the refusal stands and its
message names the repair. **The question the routes owe**: is (3a)'s *leaves
on every path* R4's predicate, its three path-enders included, and does R4
then reach `c5`?

**Q4. The certain `-` in a pattern.** design.md §4.15 (`:1997-2000`): a `-` set
apart from its literal at an arm's start is refused, and the fix, *the `-`
written against its literal, is `certain`, because in a pattern a `-` can only
be its literal's sign* (defect 123; panel 180). Lane 135c's probes, re-run at
00:41 and by the critic (`probes/q4/`): arms written as a bulleted list, `- 1
=>`, `- 2 =>`, `- 3 =>` (`s5a_bulleted_ints.hero`), take the certain fix and
the applied program builds and prints `many`, where the meant program prints
`two`; bulleted strings take the fix and are refused anew, `-"a"`,
`bad_operand` twice (`s5c`); bulleted characters apply into `-'a'`, which
checks clean, a character literal being an integer (spec `:46`), and print 3
where 2 is meant (`s5d`); one signed arm among plain ones prints `minus one`
either way (`s5b`); and `- 1 | - 2 => "small"` over `_ => "many"` flags only the
head (`t6-bulleted-pipe.hero`). Beside it, spec § 8's `Pattern` (`:244`) admits
`[ "-" ] string`, which the checker refuses. Routes: **(4a)** lane 135c's rule
in its three clauses (`<scratchpad>/lane-135c/next/first-pass.md:17-20`):
before a string the `-` signs nothing, so the fix deletes it, `certain`;
before a character both readings compile, so both fixes are guesses; before
an integer the sign is certain only where no neighbouring arm opens with a
spaced `-`; with the question whether a spaced `-` after a `|` is such a
neighbour; **(4b)** both fixes always guesses; **(4c)** the premise stands and
bulleted arms are not a mistake worth a reading (no seat measures how often a
model writes them; the critic sized a run for it, not made, § E of its
report).

**Q5. The forgotten `f`.** Panel 184 ratified (2026-10-01) R1 (`{{` and `}}`
write one brace in an `f` literal, a lone `}` an error) and R2 (`unused_binding`
naming the literal that holds the name); **neither has landed**: on
`03e70520` `probes/q5/brace.hero` prints `5`, `{x}}`, `{x}`, `f"{x}}"` prints
`5}` (`r1-lone-close.hero`), and `name = "Ada"` beside `print("hello {name}")`
gives the plain `unused_binding` with no note (`r2-unused.hero`). design.md
§1.3's locality test, ruled by the author, speaks of meaning, not legality.
Panel 184's (1b), a plain literal refused where a brace hole names a bound
binding, landed only if its blind second reading approved the wording; the
reading objected (*That non-locality affects compiling, not meaning, so it is
an objection, not a veto*,
`docs/panel/184-reports/llm-ergonomist-task1-second-reading.md:152-153`) and
approved (1a), a plain literal holding any hole-shaped `{...}` refused, which
panel 184's synthesis records (recorded, its compiler-engineer's census of
2026-09-30, not re-run here) at *34 false alarms and 0 true ones, and 22 files
stop checking, the compiler among them*; the compiler's own tests hold plain
literals with a name-shaped hole (`selfhost/lexer.hero:452`) and pin today's
`{x}}` (`selfhost/lex_interp.hero:149`). **Already refused by the record**:
panel 121 refused an active brace in a literal without a prefix (design.md
Part 7 item 7, `:3068-3076`), and design.md `:3545` holds *this language has no
warning level: a diagnostic is exit 1 or nothing* (panel 119). Routes:
**(5a)** panel 184's (1a); **(5b)** its (1b), amended; **(5c)** no refusal,
R2's note alone (built by nobody yet); **(5d)** a route judged by the
literal's own text alone, for instance a hole whose text is a name or names
joined by `.` and calls on them, its false alarms measured; **(5e)** a route
the seats find, meeting the two refusals above knowingly.

## The tree's sizes, run at 00:41 on `03e70520`
1,530 tracked `.hero` files (`git ls-files '*.hero' | wc -l`); 412 hold a line
beginning `extern `. `./heroes measure spec/heroes-spec.md`: 6,838 on the
vendored maximum, 9,060 `real` (recorded by the instrument, `claude-opus-5`,
2026-09-28), against a ceiling of 10,240, of which the FFI floor mortgages 60
(panel 030 R3): what is measured against the ceiling is 9,120, a headroom of
1,120, and Q1's § 13 sentence is an FFI sentence. The real count is re-taken by
`heroes measure --refresh`, a paid call this sitting does not make.

## The seats, and who reads what
- **compiler-engineer** (`compiler-engineer.md`): all five; builds the routes
  it judges in its copy.
- **ffi-pragmatist** (`ffi-pragmatist.md`): Q1 and defect 145; Q4 where a C
  habit is the reading.
- **spec-warden** (`spec-warden.md`): every spec and design.md sentence each
  route writes, priced on the vendored instrument.
- **historian** (`historian.md`): precedent for each question, by web search.
- **llm-ergonomist** (`llm-ergonomist.md`): one fresh session outside the
  repository, three markers over Q2, Q3 and Q5, its brief in `blind/`.
- **completeness critic** (`completeness-critic.md`): these briefs first,
  done; then the reports, before the synthesis.
