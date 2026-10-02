# Panel 185, the completeness critic's first pass: the briefs against the frozen tree

Written 2026-10-02, from 00:48. Every command below ran in
`<scratchpad>/185-critic/` (`<scratchpad>` is the one `00-shared.md` names). That
directory is a copy of `03e70520`: I extracted `git archive 03e70520 | tar -x` into a
second directory and ran `diff -rq` against this one, exit 0, no difference. The
compiler was built inside it from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, real 5.34 s, `heroes 0.2.0`). I did not use the
repository's `./heroes`. Nothing was built or run in another seat's directory: the
evidence files the briefs cite were copied into `185-critic/probe/` and re-run there.
`git ls-files` was read from the repository, whose HEAD is `03e70520`, with the index
clean apart from the untracked `docs/panel/185-briefs/`. The probes I wrote are in
`185-critic/probe/q1` to `q5` and `probe/blind`.

No paid run was made.

Status: complete. Section G lists the repairs, most important first.

## A. Framing facts found false or misleading, each with its command

**A1. (1b)'s repair, "a C shim compiled by `heroes cc`", names a command that does
not exist.** `./heroes help` ends with *Commands not built yet: lsp · cc*
(`selfhost/cli/help.hero:47`). design.md §4.19 (`:2381-2383`) says how a shim is
compiled is **undecided**: *panel 036 deferred both candidates, `heroes cc` and a
`compile "shim.c"` clause*. So the shared brief's *the repair design.md §1.11 already
names* hands every seat a repair that a program cannot use on `03e70520`. A message
built to that wording would point the author at a command the binary says it does
not have.

What does work today, measured in `probe/q1/shim/`, is a header of the program's
own holding `static inline int hero_wexitstatus(int status) { return
WEXITSTATUS(status); }`, bound by `extern "wait_shim.h"` with `function
hero_wexitstatus(status: i32) -> i64`. `heroes run` prints `1` (for 256) and exits
0, and it needs no `--include` when the header sits beside the source. Declaring
`status: i64` instead gives `build` exit 1, *error[ffi_parameter_type]: `status` of
`hero_wexitstatus` is declared wider than the header's `int`*. So this route keeps
the full parameter check, which (1a) gives up. See C1.

**A2. (2b) would refuse the compiler's own source at least 600 times, and no brief
says so.** I grepped the 1,530 tracked files for a mutation after `=>`
(`=> <place> @ `, `probe/q2/mutation-arms.txt`): 704 lines in 107 files, 676 of
them in `selfhost/`. Of those 676, 600 hold no `"`, so they are not text inside a
literal. Grep is not the parser, so this is a bound rather than the census, but the
order of magnitude is not in doubt. Today's parser takes them all: an inline arm
body is `statement(@c, @a, text)` followed by `arm_body.reject_declaration`
(`selfhost/grammar_expr.hero:1194-1206`). The shared brief presents (2b) as an equal
route, and the blind seat's A2 is (2b). The sitting needs to know that (2b) breaks
the fixpoint unless about 600 lines of `selfhost/` are rewritten. That is also a
Principle 0 fact for (2a) and (2c): the compiler needs the mutation arm, 600 times
over. Panel 014's own enumeration of the inline statement includes *a mutation*
(`docs/panel/014-match-arm-body.md:133`).

**A3. (2c) says one thing in the shared brief and another to the blind seat.** The
shared brief's (2c) refuses *a `while` or any head that opens a block* after `=>`.
Blind candidate A3, mapped to (2c) in `llm-ergonomist.md`, refuses only *a `while`
or a `for`*. The difference matters because `Primary` includes `If | Match`
(`spec/heroes-spec.md:208-212`). Today's `Inline = ( Expression | ...)` therefore
already admits an `if` or a `match` head on the arm's line, and the tracked corpus
uses both:
- `selfhost/keywords.hero:249`, `.swap fix => if fix.word == "function"`;
- `tests/golden/ir/nested-match.hero`, `.dot => match c`;
- `tests/golden/run/fixedbugs-139-value-arms-that-jump-or-give-a-value.hero:36`
  and `:100`, `.red => match d` and `.red => while n < 3`;
- `tests/golden/check/fixedbugs-131-arms-at-the-margin-of-their-match.hero:78`.

The grep counts are `=> if ` on 3 lines, `=> match ` on 7, `=> while ` on 1 and
`=> for ` on 0. On the frozen compiler I probed each head after `=>`
(`probe/q2/s07` to `s10`): `while`, `for`, `if` and `match` all check, build and
run. The two texts must name one route. Under the shared brief's wording, (2c) also
moves the compiler's own source.

**A4. The Q2 code pointer is the wrong module.** The compiler-engineer brief gives
*Q2: `selfhost/parse/arm_line.hero` (the arm's line)*. That module's header (lines
1-24) says it handles only *a `match` arm whose pattern opens with a `-` set apart
from its literal*, which is Q4's code and not Q2's. The inline body is parsed in
`arm` at `selfhost/grammar_expr.hero:1165-1209`, and the declaration refusal is
`selfhost/parse/arm_body.hero:25` (`reject_declaration`). The pointer was probably
inherited from defect 147's entry (`docs/work/DEFECTS.md:770`), which names the same
file. A consequence the briefs do not draw: because the parser already reads a
whole `Statement`, (2a) is a change to the spec alone and moves no program, while
(2b) and (2c) are parser changes.

**A5. `32e9dd18` is not defect 139's repair.** `git show --stat 32e9dd18` shows
*The batch of lane flow closes: the seed regenerated once ...*. It touches
`seed/heroes.c` and four emission files and no `selfhost/` module.
`git log -S'every_branch_jumps'` names the repair commits: `201b99af` (*Defect 139,
the value half*) and `3a25b640` (*a match statement leaves its block only when
every arm does*).

**A6. Q1's census instrument cannot see Q1.** The compiler-engineer is asked for *a
census of `check --brief` and its exit over the 1,530 tracked files*. On
`probe/q1/macro.hero`, `check --brief` exits 0 and `build --emit-c` exits 1 with
`ffi_unknown_name`: the extern probe runs at build time, and `--emit-c` does run
it. For Q1, a census over `check` would read zero movement whatever the route does.
Q1's census has to be `build --emit-c` over the files that hold an `extern` line.

**A7. "412 files hold an `extern` line" is not "every binding that builds today".**
The ffi-pragmatist brief asks *whether the emitted C of every binding that builds
today stays byte-identical (412 tracked `.hero` files hold an `extern` line ...)*.
The 412 is correct (`git ls-files '*.hero' | xargs grep -l '^extern ' | wc -l` gives
412). Of them, 311 are under `tests/golden/`, 72 under `docs/panel/`, 5 under
`selfhost/`, and most of the goldens are refusal cases. I ran `build --emit-c` on
each of the 412 (`probe/emit-one.sh`, `xargs -P 3`, `probe/extern-emit.tsv`):
**206 exit 0 and 206 exit 1** on this Mac. Of the 206 that emit, 140 are under
`tests/golden/`, 41 under `docs/panel/`, 21 under `examples/`, 2 under
`tests/harness/` and 2 under `selfhost/cli/`. Five are refused with
`ffi_unknown_name`, and **none of the five is a macro-only name**:
- `_IO_FILE`, glibc's spelling (`docs/panel/175-briefs/popen_linux.hero`);
- `_popen`, `_pclose` and `_iobuf`, Windows names (`popen_windows.hero`);
- the misspellings `sqlite3_openn` and `addrinfoo` (the two `fixedbugs` goldens);
- `describe` in a local `err.h` (`tests/golden/ir/owned-cell.hero`).

That gives a falsifiable baseline for (1a): those five must stay refused.

**A8. The scratchpad evidence goes away with the session.** The framing facts of
Q1 to Q5 rest on files under the session scratchpad:
- `p184/ffi-side/macro.hero`;
- `p185/inline/a54`, `a55`, `a69`, `a73` and `a76`;
- `lane-emit/pass1/143/`;
- `lane-135c/next/P5/`;
- `decide-1001/brace.hero`;
- `lane-flow/first-pass.md`;
- `184-compiler-engineer/`.

`.claude/skills/panel/SKILL.md` § 2 moved briefs onto disk *because the scratchpad
this line named until then is session-specific and goes away with the session*.
Panel 184 kept its probes in the tree (`docs/panel/184-briefs/probes/`), and panel
185's briefs do not. No later reader can open the programs these questions are
about.

## B. Facts stated without their source, or marked wrongly

**B1. The "6 to 7 levels of nesting" figure** (compiler-engineer brief, *lane flow
measured on 2026-10-01 that a field added to `arms` cost 6 to 7 levels of nesting
until it was reshaped*) is not marked *recorded* and names no source. I found it at
`<scratchpad>/lane-flow/first-pass.md:106-109`: *A first draft lost 6 and 7 levels
(frames: arms +1152 bytes, expression_statement +1200, clang -fstack-usage on the
emitted C); the arm rule moved out of `arms`*. It is in no tracked record. The
nearest tracked text is the body of `201b99af`, which gives the depths before and
after the reshaping (*`if` 192 and 192, `match` 161 and 161, value `if` 123 and 123,
calls 95 and 97*) and not the 6 or the 7.

**B2. *a locality of compiling* is printed as a quotation and is not one.** The
second reading's words are *That non-locality affects compiling, not meaning, so it
is an objection, not a veto*
(`docs/panel/184-reports/llm-ergonomist-task1-second-reading.md:152-153`).

**B3. The second round for defect 145 is an inference.** The shared brief says *the
same second round would let defect 145's note name the one reading*. (1a)'s second
round is triggered by `ffi_unknown_name` and asks `#ifdef`. Defect 145's goldens are
`ffi_unknown_tag` and need a different question. I measured that one exists: against
`tests/golden/unsupported/fixedbugs-145-types.h`, `(void)sizeof(anon_s);` compiles
(a typedef name) and `(void)sizeof(only_tagg);` is *use of undeclared identifier*.
One extra clang query therefore splits the two goldens. Whether it is "the same"
round is a design question for the seats, not a premise.

**B4. Panel 184's census is quoted without its other half.** The brief carries *34
false alarms and 0 true ones* and leaves out the same table cell's *22 files stop
checking, the compiler among them*
(`docs/panel/184-a-brace-...md:54`). The spec-warden's cell reads *the compiler
stops compiling itself*. That is the fact about (5a) a seat most needs, and the
blind seat's C1 is (5a). It also reaches (5d). The compiler's own tests hold plain
literals with a name-shaped hole, for example `selfhost/lexer.hero:452`,
`lex_one("s = f\"a {n} b\"\n")`, and `selfhost/lex_interp.hero:149` pins today's
`{x}}` (`assert piece_text("f\"{{x}}\"") == "{x}}"`), which R1 must change.

**B5. R2 has not landed either, and (5c) is "R2's note alone".**
`probe/q5/r2-unused.hero` (`name = "Ada"`, `print("hello {name}")`) gives *`name`
is bound and never read — remove the binding, or read it*, with no note naming the
literal. The brief says R1 has not landed and is silent on R2. (5c) is a route
whose only content is unbuilt.

**B6. The compiler-engineer is pointed at panel 184's binaries.** `heroes-1a` and
`heroes-1b2` sit in `<scratchpad>/184-compiler-engineer/`, built on 2026-09-30. Two
rules bear on that. Running them means running inside another seat's directory. And
a prototype built on an older tree is the stale-compiler case of
`.claude/rules/verification.md` § *The compiler that judges is a build artifact*.
The brief says *re-measure on `03e70520`*. It should also say to re-apply the
prototypes' changes on `03e70520` in the seat's own copy, not run the old binaries
against the new corpus.

**B7. Smaller items.**
- a69's message is *this `if` produces no value*, not the `match` wording the brief
  quotes for all three cases.
- The spec-warden's instrument also prints *the FFI floor mortgages 60 of it (panel
  030 R3), so what is measured against the ceiling is 9120*. The headroom that
  judges is therefore 1,120, not 1,180, and Q1's § 13 sentence is an FFI sentence.
- `ffi_unknown_name`, the message (1b) rewrites, is built at
  `selfhost/emit/ffi_declared.hero:159`, which no brief names.
- `selfhost/cli/assemble.hero:4-24` already runs a second clang round for a
  handle's `struct` tag (defect 037, panel 152 R1, *a program whose tags need no
  qualifier never reaches the second round and pays nothing*). That is the
  mechanism (1a) would extend, and its precedent belongs in Q1's framing.

## C. Routes the questions admit that nobody listed

**C1 (Q1). A macro-only name refused, with the message naming a header of the
program's own that wraps the macro in a `static inline` function.** It was measured
working on `03e70520` (A1). Every parameter stays checked, because the wrapper has a
declaration, and it needs no command that is not built. It differs from (1b) in the
repair it names and in being usable today.

**C2 (Q1). The compiler writes that wrapper itself from the declared signature**,
`static inline <ret> hero_m_<name>(<declared C types> a0, ...) { return
<name>(a0, ...); }`. This gives the macro lvalue arguments as (1a)'s `(T){0}` does,
and it checks exactly what (1a) checks. It is a variant of (1a) for the
compiler-engineer and the ffi-pragmatist to price against it.

**C3 (Q1). A mark on the line, for instance `macro function WEXITSTATUS(...)`**, so
that the unchecked parameters are visible where the reader stands rather than
stated once in § 13. This is a surface change and would need a panel. I list it so
that a seat weighing (1a)'s locality has it in the option set. It is a question,
not a recommendation.

**C4 (Q4). The rule as lane 135c wrote it, which (4a) does not state.** Lane 135c's
note (`<scratchpad>/lane-135c/next/first-pass.md:17-20`) has three clauses:
- before a string, the `-` signs nothing, so the fix deletes it, `certain`;
- before a character, both readings compile, so both fixes are guesses;
- before an integer, the sign is certain only where no neighbouring arm opens with
  a spaced `-`.

The brief's (4a) keeps only the third clause. The re-runs show why the other two
matter. On s5c, today's certain fix writes `-"a"`, which `check` then refuses
(`bad_operand`, twice). On s5d, bulleted characters apply into `-'a'`: since *a
character literal is an integer* (`spec/heroes-spec.md:46`), the program checks
clean and prints `3` where `2` was meant. Neither shape is in the brief.

**C5 (Q4). A shape that defeats (4a) as stated.** In `probe/q4/t6-bulleted-pipe.hero`,
the arm is `- 1 | - 2 => "small"` over `_ => "many"`. Only the head `- 1` is
flagged, because a `-` after `|` is not at a line's head, and the arm is the only
one that opens with a spaced `-`. So (4a) keeps the fix certain. The applied program
prints `many` where `small` was meant. Whether a spaced `-` after a `|` counts as a
neighbour is a question for (4a)'s wording.

**C6 (Q5). Two routes already refused by the record, which a seat may otherwise
propose as (5e).** Panel 121 refused an active brace in a literal without a prefix
(design.md Part 7 item 7, `:3068-3076`: *211 literals in 39 of the compiler's own
modules already hold a `{`, which is what gated the form behind a letter*). design.md
`:3545` says *this language has no warning level: a diagnostic is exit 1 or
nothing* (panel 119). The brief should name both, so that a seat reopening either
does so knowingly.

## D. Premises handed to seats where a question was owed

- **D1.** (1b)'s repair through `heroes cc` (A1).
- **D2.** (2b) and (2c) as costless alternatives to (2a) (A2, A3). The parser
  already implements (2a), so the routes are not symmetric.
- **D3.** (1b) corrects *§1.11's sentence*. design.md promises macro reachability
  in four places: `:565` (§1.11), `:645` (§3.1, *reaches macros, `inline`
  functions and `#define` constants*), `:2265-2266` (§4.19, *Macros and `inline`
  functions are reachable*) and `:2401-2402` (§4.19, shims *not for every macro*).
  The spec mentions no macro at all (`grep -n -i macro spec/heroes-spec.md` is
  empty). The spec-warden prices only spec text and design.md carries no budget,
  but the sentence count (1b) owes is four, not one.
- **D4. Q3's cases name one shape, and its neighbours fail the same way.** On
  `03e70520` each of these draws `no_value`, *every branch jumps*:
  - an outer value `if` whose branch ends in a returning `match` (`probe/q3/b1`);
  - the inner `match` written as the arm's inline head (`b2`);
  - `return match k` (`b3`);
  - two levels deep (`b5`);
  - `break` and `continue` inside a loop (`b6`);
  - an inline `if` head (`b8`).

  The adopted route should be built and run on these, not on a55, a69 and a73
  alone.
- **D5. Q3 and R4 share a predicate nobody has written down.** R4 says that
  `exit(code:)`, `assert false` and `while true` with no `break` end a path. As a
  value arm on `03e70520`:
  - `0 => exit(code: 3)` draws `bound_unit` and a `type_mismatch` placed on the
    *other* arm, `_ => 20` (`probe/q3/c1`, `c6`);
  - `0 => assert false` and a `while true` block draw *this branch ends on a
    statement* (`c2`, `c3`).

  Whether (3a)'s *leaves on every path* includes R4's three path-enders is unasked.
  Beside it, `c5`: a `print(9)` after a statement `match` whose every arm
  `return`s checks clean, exit 0. Under (3a) that `print` follows a block that
  leaves, and R4 as worded (*after `return`, `break` or `continue`*) does not reach
  it. The compiler-engineer's R4 question should name c1 to c6.
- **D6. Q4's (4c) asks whether bulleted arms are a mistake worth a reading, and no
  seat measures it.** The blind seat is excluded from Q4, and the historian is asked
  for a study "if any". The spec's line 16 is reader-facing (*the next line may not
  begin with a `-` that does not touch its operand*), and whether a model writes
  `- 1 =>` at all is a generation rate. See E.
- **D7. What the blind reading of C adds.** C1 is panel 184's M word for word and
  C3 is its L (`docs/panel/184-briefs/blind/brief-task1.md:14-22`). Both have been
  read blind once already, M approved and L objected. Only C2, route (5d), is new.
  (5b), which is (1b) amended, is not shown because its earlier blind reading
  objected. The llm-ergonomist brief states neither fact, and the synthesis needs
  both to weigh the reading.
- **D8. `_ = 0` on an arm's line.** Spec § 8 says *an arm that does nothing is a
  block holding `_ = 0`* (`:230-231`), and blind task A asks for an arm that does
  nothing. Inline, `.blue => _ = 0` is refused with *`_` would be bound where
  nothing can read it* (`probe/q2/s06-discard.hero`), while spec § 5 says `_`
  *binds nothing*. Whether (2a)'s *a declaration excepted* covers `_ = e` is a
  choice the blind seat will meet, and the message's wording looks like a defect
  beside Q2. Neither is in the briefs.

## E. A paid run worth considering, not run

For D6: one fresh session per task, by the blind seat's command, asked to write
three `match`es over small integer and character keys from a prose list, with no
mention of signs. The measure is how many arms open with `- `. A rate is
informative from about 20 tasks, at the 3 USD cap each, so at most 60 USD.
Panel 184's two second readings cost 0.33 and 0.40 USD by the CLI's own report
(their report headers). Twenty such sessions would then come to about 8 USD, but
that is an inference from two sessions, not a measurement. I note the run only;
the coordinator decides.

## F. Verified as written

These checks ran and agree with the briefs:
- every repository path the briefs name exists (18 paths, `test -e`);
- `extern_probe.hero:171` is the probe line `(void)(name)(args)`;
- panel 092 chose the parenthesised callee against `_FORTIFY_SOURCE` and recorded
  the macro-only cost (`:52`, `:106-110`, `:254-258`), and stated the pointee gap
  (its resolution, item 3);
- design.md `:565`, `:1230`, `:1236-1255` and `:1997-2000` read as quoted;
- spec `:227`, `:242-243` and `:244` read as quoted;
- `macro.hero`: `build` exit 1, `ffi_unknown_name`;
- a54 prints 5 and a76 prints 3;
- a55, a69 and a73 each exit 1 with `no_value`;
- s5a applies and prints `many` where the meant program prints `two`, and s5b
  prints `minus one` both ways;
- `brace.hero` prints `5`, `{x}}`, `{x}`;
- a lone `}` in an `f` literal prints as text (`f"{x}}"` prints `5}`), so R1 has
  not landed;
- a `print` after a `return` checks at 0, and `grep -n 'after a jump'
  spec/heroes-spec.md` is empty, so R4 has not landed;
- on macOS, `WEXITSTATUS((int32_t)0)` fails with *cannot take the address of an
  rvalue* (`_W_INT(w) (*(int *)&(w))`, `sys/wait.h:131`), `(WEXITSTATUS)(...)` and
  `(htonl)(...)` are *undeclared identifier*, and `WEXITSTATUS((int32_t){0})`,
  `WIFEXITED`, `WTERMSIG`, `htonl((uint32_t){0})`, `(isdigit)(...)` and `errno`
  compile;
- 1,530 tracked `.hero` files and 412 holding a line that begins `extern `;
- `measure`: 6,838 maximum, 9,060 real, ceiling 10,240;
- `claude --version` reads 2.1.285;
- the blind folder's `spec.md` differs from the frozen spec by exactly
  `blind/spec-markers.diff` (`cmp` of the two diffs); `brief.md` and the three
  tasks are byte-identical to `blind/`; no `CLAUDE.md`, `.claude` or `.git` sits in
  or above `185-llm-ergonomist/`; the brief and tasks carry none of the project's
  names;
- on the frozen compiler, task-a is refused at 12:23 (`declaration_in_arm`),
  task-b at 4:13 (`no_value`), and task-c prints `total {total}`,
  `{"id": 7, "ok": true}`, `done: true`;
- the `heroes-linux` image exists. A `heroes-linux-arm64` container
  (`naughty_keller`) was running at the time, so the ffi seat's *`docker ps`
  first* is needed.

## G. The repairs I ask for, most important first

1. **Q1, (1b):** strike *a C shim compiled by `heroes cc`*. The command is not
   built and design.md §4.19 records shim compilation as undecided (A1). Name the
   header-only `static inline` wrapper as a measured working repair and add it as
   route (1d) (C1). List the four design.md sentences (1b) must correct, not one
   (D3).
2. **Q2:** state that the parser already implements (2a) (A4), and that (2b) would
   refuse at least 600 code lines of `selfhost/` (A2). Make (2c) one route in both
   briefs: either *`while` and `for`* or *any block head*, with the tracked `if`
   and `match` heads named if it is the second (A3). Repoint the compiler-engineer
   to `grammar_expr.hero:1165-1209` and `parse/arm_body.hero:25` (A4).
3. **Copy the evidence into the tree**, as panel 184 did:
   - `p185/inline/*.hero`;
   - `p184/ffi-side/macro.hero`;
   - `lane-135c/next/P5/*.hero` with `summary.tsv`;
   - `decide-1001/brace.hero`;
   - the lane-flow note.

   Use `docs/panel/185-briefs/probes/` and cite the copies (A8).
4. **Q1's census:** tell the compiler-engineer and the ffi-pragmatist that `check`
   cannot see Q1, and that the population is `build --emit-c` over the 412: 206
   emit today and 5 are `ffi_unknown_name`, none of them a macro (A6, A7).
5. **Q4:** state lane 135c's three-clause rule, not its third clause alone. Add s5c
   (a certain fix that is refused anew) and s5d (characters, silently wrong), and
   put the `- 1 | - 2` shape to (4a)'s authors as a question (C4, C5).
6. **Q3:** name the neighbouring shapes the adopted route must be run on (b1 to b8)
   and R4's path-enders as value arms (c1 to c6). Ask whether (3a)'s predicate is
   R4's (D4, D5).
7. **Q5:** carry panel 184's *22 files stop checking, the compiler among them*
   beside the 34, and say that R2 has not landed either (B4, B5). Name panel 121's
   and panel 119's refusals so that a (5e) proposal meets them (C6). Fix the
   misquotation (B2).
8. **llm-ergonomist brief:** say that C1 and C3 repeat panel 184's M and L, and why
   (5b) is not shown (D7). Decide whether `_ = 0` on an arm's line is part of
   task A's answer space (D8).
9. **Compiler-engineer brief:** mark the "6 to 7 levels" figure *recorded*, cite
   `lane-flow/first-pass.md:106-109`, and write "6 and 7" as the note does (B1).
   Replace `32e9dd18` with `201b99af` and `3a25b640` (A5). Ask for panel 184's
   prototypes to be re-applied on `03e70520` in the seat's own copy (B6).
10. **Smaller items:** the 60-token FFI floor in the spec-warden's numbers;
    `ffi_declared.hero:159` and `assemble.hero:4-24` as Q1 pointers (B7). Treat
    defect 145's second round as a question, with the `sizeof(name)` measurement
    attached (B3).
