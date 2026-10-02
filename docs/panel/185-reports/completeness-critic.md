# Panel 185, the completeness critic's second pass: the five reports

Written 2026-10-02, from 02:26, after the coordinator's resume. I read
`00-shared.md` as repaired, the four seat briefs as repaired, and the five
reports in `docs/panel/185-reports/`: the compiler-engineer's, the
ffi-pragmatist's, the spec-warden's, the historian's and the blind seat's.
Everything below was run in my own directory,
`<scratchpad>/185-critic/`, a copy of `03e70520` with its compiler built from
the seed. No seat's directory was read, built in or run in. Panel 184's
`<scratchpad>/184-compiler-engineer/tree2` was read, not run, to take its R4
prototype as a patch. No paid run was made.

Status: complete. § F lists what the synthesis must not get wrong, most
important first.

## A. The finding the coordinator asked me to verify: R4 as prototyped breaks `build`

**Reproduced independently.** Panel 184's R4 prototype holds its routes behind
one constant, and it is pinned to R4's route: `constant ROUTE: str` / `"2d"`
(`tree2/selfhost/check/leaves.hero:18-19`). I took it as a patch,
`diff -ruN corpus/selfhost tree2/selfhost` (405 lines), and applied it to a
fresh `git archive 03e70520` in `185-critic/r4/tree`.

Nine hunks fail, as the compiler-engineer found. Eight of them are defect 139's
repair, which landed on the trunk after panel 184's tree:
- the `jumps` field of `join.Branch` and its tests;
- the if/else branches;
- the statement `match`'s `all_jump`;
- the `jumped` flag in `arms`.

The other is R4's own: the `block` hunk that tracks `left` and refuses after
it, applied by hand. So is the `broke` field of `Checker` (the one rejected
hunk of `state.hero`). The compiler was built from my seed compiler: real
90.48, user 80.84, `r4/heroes-r4`.

| probe (`probes/q3/`) | `check` | `build` |
|---|---|---|
| c1 `0 => exit(code: 3)` as a value arm | 0 | **2** |
| c2 `0 => assert false` | 0 | **2** |
| c3 a value block ending in `while true` | 0 | **2** |
| c6 a value block ending in `exit(code: 3)` | 0 | **2** |
| c4, a statement after `return` (control) | 1, `unreachable_statement` | 1 |
| panel 184's `after-return.hero` (control) | 1 | 1 |
| c5, a statement after a `match` every arm of which returns | 0 | 0 |
| a55 | 1, `no_value` | 1 |

The message for each of the four: *internal error: compiling the generated C
failed: ... variable 'h2_r0' is used uninitialized whenever 'if' condition is
true [-Werror,-Wsometimes-uninitialized]*. That is a check-clean program that
the compiler then blames on itself, exit 2, the class of `.claude/rules/c-boundary.md`.

**What I did not verify**: the compiler-engineer's 16-line seal. It was built
and run in that seat's directory, and the `emission` suite was not run on it
by anyone (the engineer says so).

**What the finding is, and what it is not.** R4's ratified text speaks only of
`missing_return`: *`missing_return` is satisfied by a call to `exit(code:)`, by
`assert false` and by a `while true` with no `break` of its own*. The
prototype widens `Outcome.jumps` wherever those statements stand, so it also
reaches value positions. The landing therefore has two choices, and the
synthesis must make one of them in writing:
- **keep the widening at a function's end**, and c1, c2, c3 and c6 stay refused
  as today;
- **let it reach value arms**, which is the spec-warden's *arms wide* text and
  the historian's *(3a) with R4's predicate, the three path-enders included*.
  Then the seal lands with R4, or c1, c2, c3 and c6 build at exit 2.

Either way, "a defect in R4 as prototyped" is to be filed before R4 lands, as
the engineer asks.

## B. Contradictions between seats, and which side is checkable

### B1. Q1, (1a) and (1a′)

- **The historian** approves both, reading (1a′) as the more robust, on GHC's
  `capi` and bindgen's `--wrap-static-fns`, and objects to (1b).
- **The ffi-pragmatist** vetoes both, and so does the spec-warden (Principle 0
  and §1.12).
- **The compiler-engineer** objects to both.

**The checkable side is the ffi-pragmatist's and the engineer's, and I re-ran
it.**

- **Every wrong parameter type compiles through a (1a′) wrapper**, with the
  compiler's own flags (`selfhost/cli/flags.hero:91-109`). `static inline
  int64_t w(T a0) { return WEXITSTATUS(a0); }` gives 0 errors for `T` =
  `int32_t`, `int64_t`, `uint8_t`, `uint32_t`, `double` and `void *`
  (`probe/q1/c2/run.sh`).
- **A system header's macro hides the conversion.** The same wrapper over
  `htonl` gives 0 errors for `int32_t` and `int64_t` as written, and 2 each
  once preprocessed first (`clang -E -P`). The same `int64_t` handed to a
  typed function outside any macro gives 3 errors.
- **The `u8` declaration reads out of bounds.** The wrapper typed `uint8_t`,
  built with `-fsanitize=address,undefined`, exits **134**: *AddressSanitizer:
  stack-buffer-overflow ... READ of size 4 ... in hero_m_WEXITSTATUS*. Built
  without the sanitizer, it exits 0 and prints **`240`**, where
  `WEXITSTATUS(1)` is `0`.

**Wording the synthesis must get right.** The coordinator's message calls this
"a measured stack overflow". It is an out-of-bounds READ of a 1-byte argument,
`_W_INT(w) (*(int *)&(w))` at `sys/wait.h:131`. Without the sanitizer it is a
silent wrong answer at exit 0. It is not stack exhaustion.

**Why the historian's reading falls.** Its premise is *"Where the declaration
is the source, the C compiler compiling a call with it is the check, and it is
the whole check"* (its Q1, point 2). The ffi-pragmatist measured that premise
false for system-header macros on macOS, and I reproduced it.

Its other claim, *"the wrapper is a real function, so the panel 092 probe
applies to it unchanged"*, is answered by the ffi-pragmatist's matrix. The
wrapper's parameter types are the declaration itself, so the probe holds the
declaration against itself (ffi report, *(1a′)*, its *experiment* line).

Its precedent shows that other FFIs accept this hole. It does not show the
hole is safe. And its *robust* means reach, where CLAUDE.md § Precedence puts
§1.12's no-memory-corruption at rank 3, above every other good. The record of
precedent is worth carrying into the synthesis as what (1b) costs users (the
Linux kernel's helper directory, bindgen #753). It does not lift a soundness
veto.

### B2. Q1, what (1b)'s note writes into the author's shim

- **The compiler-engineer** says of its prototype: *"its note's shim template
  has one parameter whatever the arity; the landing writes the declared
  parameters"*.
- **The ffi-pragmatist** says: *"It must not draft the C types from the
  declaration: that reproduces (1a′) in the author's file, and the `u8`
  overflow with it"*.

**The ffi-pragmatist's side is checkable, and it is the `u8` run above.** A
shim drafted from a wrong `status: u8` is exactly the wrapper that read out of
bounds. Once the author pastes it, the probe holds the declaration against a
shim that matches it by construction. The note must give the C types from the
macro's documentation, or a placeholder the author fills. It must never give
the declared types.

### B3. Q1 and defect 145, which question splits the readings

- **The compiler-engineer** puts my first pass's `(void)sizeof(NAME);` into
  the same unit as the macro question.
- **The ffi-pragmatist** recommends `typedef NAME hero_q;`.

**The ffi-pragmatist's side is checkable, and I re-ran it** against
`fixedbugs-145-types.h`. The `typedef` form gives three distinct answers:
- `anon_s` and `opaque_t` compile, so each is a type name;
- `only_tag` gives *must use 'struct' tag*, so it is a real tag;
- `only_tagg` and the function name `use_anon_s` give *unknown type name*.

By the ffi-pragmatist's table, `sizeof` compiles on a function's name, so it
would call a function a typedef. It also cannot tell a real tag from a
misspelled one. My first pass's query was the weaker one, and the synthesis
should take the `typedef` form.

### B4. Q2, (2a) against (2c), and what the blind reading says

- **The spec-warden** approves (2c) in its split form at -20 vendored tokens,
  on Principle 0: *"a loop on the arm's line has no decision naming it and no
  compiler line using it"*.
- **The compiler-engineer** approves (2a), and so does the historian, who
  calls (2c) *the conservative route*.
- **The blind reading approves both A1 and A3** and objects only to A2. *"None
  of the three makes a line's meaning or whether it compiles depend on anything
  but the arm's own line."* Its predictions are A1 at most 3 per cent and A3 at
  most 5 per cent first-attempt errors. **So the blind reading does not choose
  between (2a) and (2c)**, and the synthesis must not cite it for either over
  the other. The coordinator's message says it approves (2a); it approves both.

**What is checkable:**
- (2a) moves 0 programs and 0 compiler lines (4 more if `_ = e` is admitted).
- (2c) moves 1 tracked line, `tests/golden/run/fixedbugs-139-value-arms-that-jump-or-give-a-value.hero:100`.
  It costs 13 compiler lines and a new diagnostic, `loop_on_arm_line`, which
  is a new diagnostic class (CLAUDE.md § 4).
- Both split forms are removals: (2a) -11 and (2c) -20 vendored.
- No seat measured a loop on an arm's line as a mistake. The engineer's
  probes found none, and the blind seat predicts at most 3 per cent errors.

**What weakens the spec-warden's premise.** Its own correction of 02:05 says
*"no decision names a loop on the arm's line, and none refuses it"*. But
design.md §4.7 (`:1230`, panel 014) says *one statement*, spec § 5's
`Statement` lists `While | For`, and spec § 8's prose (`:227`) says *one
statement*. So the loop on the arm's line is in the ratified design by the
class it belongs to. Principle 0 governs what *enters*. (2c) is a new refusal
of a form that is in, and it owes a measured mistake class. That is the
spec-warden's own condition, and nobody measured it.

The historian's Python condition (an `else` after a compound head on an arm's
line read with another owner) is also checkable. I opened the one such line in
the compiler, `selfhost/keywords.hero:249-252`, `.swap fix => if ...` /
`else`. The `else` sits at the arms' column and has one possible owner. The
two other `=> if ` hits are a comment (`selfhost/print/fmt.hero:713`) and a
golden's deliberate parse error (`fixedbugs-131-a-failed-head-over-its-body.hero:55`).

### B5. Q2, `_ = e` on an arm's line

- **The compiler-engineer** admits it, 4 lines, and every neighbour then gets
  its block form's message.
- **The historian** reads `_ = e` as no binding (Rust, Go).
- **The spec-warden's** exact prose for both split routes refuses it: *"inline
  one statement that is not a declaration or an `=`, `_ = 0` included"*. It
  keeps `_ = 0` a line of a block, as spec § 8 says.

All three agree that today's message (*`_` would be bound*) is false by spec
§ 5. I re-ran `probes/q2/s06-discard.hero` in the first pass. **The disagreement
is a choice, and the synthesis must make spec text and compiler say the same
thing**: the CE's (2a) build and the warden's (2a) text disagree on `_ = e`,
and that disagreement is defect 147's own class.

### B6. Q3, does R4 reach `c5`?

- **The spec-warden** says *"If R4 is to reach `c5` (R4 states its refusal
  reads defect 139's every-branch predicate)"*, prices every (3a) text merged
  into R4 by redefining *jump*, and predicts *"exits 1 on `c5`"*.
- **The historian** says *"(3a)'s leaves on every path should be R4's
  predicate, and if it is, R4 reaches `c5`"*.
- **The compiler-engineer** says *"R4 does not reach c5: its refusal reads
  only the three jump words"*.

**The engineer's side is the record's, and it is checkable twice.**

- **Panel 184 chose (2d), which leaves `c5` legal, on purpose.** Its
  compiler-engineer's route (2d) is *"after-if-else-returns, after a `match`
  or `while true` that leaves | exit 0, as today"*
  (`docs/panel/184-reports/compiler-engineer.md:614-640`). The refusal after
  an all-leaving `match` belongs to route (2a), which panel 184 refused: 142
  statements in 51 files. Its *"(2d)'s refusal reads the same predicate"*
  (`docs/panel/184-...md:193`) follows *"The `match` defect is repaired
  first"*. It is not a statement that R4 refuses after an all-leaving
  `match`, and R4's own text says *after `return`, `break` or `continue`*.
- **On `03e70520`, measured here.** I rebuilt the same R4 tree with `ROUTE
  "2a"` (`r4/heroes-2a`, real 109.00). Under (2a), `heroes check --brief
  selfhost/main.hero` goes from exit 0 to **exit 1, with 133
  `unreachable_statement` refusals in 62 files of the compiler**. In 131 of
  them the statement that leaves before the refused one is a statement `match`
  whose every arm leaves, the spec-warden's merged text's case; in 2 it is a
  `while true`. `tests/harness/main.hero` goes to exit 1 too, and `c5` exits
  1. Under R4 itself (`r4/heroes-r4`), `selfhost/main.hero` stays at exit 0.

**So the spec-warden's merged (3a) texts, as written, are not (3a).** They are
(3a) plus panel 184's refused (2a)-narrow. Adopted, they stop the compiler
checking itself on about 131 statements. That holds for the narrow and the
*arms wide* forms, and for any text in which an all-leaving `if` or `match`
"is a jump" for R4's *a statement after a jump*. The landing needs a (3a)
sentence that says the arm, or the block, leaves, without making R4 refuse
after it. It is not priced yet.

Blind candidate B1's *"... leaves as well, and counts as a jump"* is near the
same edge. It was read on a spec copy with no R4 sentence in it (see § E2).

### B7. Q4, (4a) against the two-clause route

- **The historian** approves (4a), lane 135c's three clauses.
- **The compiler-engineer** approves (4a′), (4a)'s string clause with (4b)
  for integers and characters. The spec-warden approves the same two clauses.
- **The ffi-pragmatist** has no objection between (4a) and (4b) and leans to
  (4b) for integers.

**Checkable, and I re-ran it.** `t7` is `- 1 => "one"` over `_ => "many"` with
`n = 1`. It takes the certain fix `-1` and prints `many`. No neighbour opens
with a spaced `-`, so (4a)'s integer clause keeps the fix `certain`. The
spec-warden's one-arm probe is the same shape. **(4a) is falsified as a rule of
certainty.**

(4a′) is a route the seats built, not one the brief listed. The synthesis
should give it its own label, not adopt it under (4a)'s.

### B8. Q4, whether spec § 8's `Pattern` stops listing `[ "-" ] string`

- **The compiler-engineer** says it should.
- **The spec-warden** objects, at 0 tokens: the parser accepts `-"a"` and the
  checker refuses it by type.

**Checkable**: `heroes parse` exits 0 on `probes/q4/s5c_bulleted_strings_applied.hero`,
re-run here. So the production matches the parser today. Tightening it means
changing the parser too, or the spec would claim a parse refusal the parser
does not make, which is defect 147's shape again. The synthesis has to pick
one of those two, and either way production and parser must agree.

### B9. Q5, three seats and three routes

- **The blind reading** approves C1 (5a) and objects to C2 (5d) and C3 (5c).
- **The compiler-engineer** approves (5b) amended, at 0 false alarms, 0 files
  moved and 2 true sites. It measures (5a) at 32 false alarms with 22 files
  stopped, `selfhost/main.hero` 0 to 1, and (5d) at 27 false and the same 22.
- **The spec-warden** approves (5b) short at +38 over R1 and (5c) as
  conservative.
- **The historian** approves (5c): every bare-brace check in the record
  retreated over false positives, and none ships as an error.

What is checkable, and what is not comparable:

1. **The blind reading never saw (5b)**, on the brief's choice, so it says
   nothing for or against (5b). Its argument against C2 is *"It leaves
   `{a + b}`, `{x > 10}` ... silently printed"*. That is an argument about
   (5d)'s shape. It does not carry to (5b), whose prototype counts any hole
   with at least one name, every name bound. By that stated rule, `{total >
   10}` with `total` bound is caught. That is an inference from the rule as
   the engineer states it, not run by me.
2. **Its C1 approval turns on a reading it flagged as open**: does *"would be
   a hole"* mean any `{...}`, or one whose contents parse? The engineer's (5a)
   prototype is the parsing reading, so its 32 false alarms are that
   reading's. The broad reading is unmeasured, and its count can only be
   higher.
3. **(5a) buys no measured reach over (5b).** Panel 184 recorded that both
   catch the same 17 silent `forget-f` mutants, *"under both (1a) and (1b)
   they are the rule's class, so their capture is true by construction"*. So
   those 17 measure neither rule's reach, and panel 184 adds that how often a
   model forgets the `f` is unmeasured. In the tracked tree both catch the
   same 2 true sites, by the engineer's census. I re-ran the file and the two sites are real: on
   `03e70520`, `docs/panel/184-briefs/blind/task1.hero` gets `unused_binding`
   at 6:5 and nothing at all for line 14. The difference between (5a) and (5b)
   is therefore 32 false alarms against 0, and locality.
4. **The historian's warning is checkable against Heroes' own corpus and holds
   there so far.** Its precedents' false positives are *"strings that something
   else fills later, where the name is in scope by construction"*. The only
   template program tracked, `examples/template/main.hero`, holds 20 such
   literals, and the engineer's census says (5b) spares all 20, since no
   binding there shares a placeholder's name. One program is a small sample,
   and a template whose placeholder names a local in scope is the shape that
   would break (5b). The synthesis should carry it as (5b)'s falsifier.

**The author's condition is not met by this sitting, and the synthesis must
say so.** Panel 184's § Author's verdict sent *(1b)'s wording* back to a
sitting after its blind reading objected. The spec-warden priced new short
wordings of (5b) (+38). The CE built (5b). No blind reading of any (5b)
wording was made. Adopting (5b) re-proposes a route whose only blind reading
objected (on locality of compiling, an objection rather than a veto). Whether
the author's ruling on §1.3 (*meaning, not legality*) disposes of that
objection is the author's to say, not the synthesis's. See § E1.

## C. Claims asserted and not measured by the command that settles them

1. **The seal of R4 has no `emission` run, and (3x) with R4 and the seal was
   never built as one tree.** `walk.hero` *"1884, 14 over"* its decided 1870
   is the compiler-engineer's arithmetic on measured parts. So is the
   *"expect it to move no blessed emission"*, which the engineer marks as an
   inference, unrun.
2. **No route was run on Windows by anyone, and Linux x86-64 only by the
   ffi-pragmatist, for today's compiler and (1d)'s shim.** The engineer's
   (1b) prediction includes *"reads `ffi_macro_name` ... on Linux x86-64"*,
   unrun. `.claude/rules/platforms.md` and CLAUDE.md § Verification say a
   repair at the C boundary is run on its platform at once. (1b) and (1d) are
   C-boundary changes, so Windows and Linux arm64 are owed before the landing
   is called done.
3. **The compiler-engineer's (5a) cost is "12 of its literals", and its own
   table lists 11 under `selfhost/`**:
   - `emit/assert_spelling.hero` 188, 192, 277 and 278;
   - `emit/body.hero:182`;
   - `escape_readings.hero` 306 (two) and 307;
   - `lex_interp.hero:149`;
   - `lexer.hero:452`;
   - `next_line.hero:314`.

   One of the two is wrong, and the number is cited as a Principle 0 cost.
4. **The ffi-pragmatist's "reads three bytes beyond `t1`" without the
   sanitizer is marked an inference.** I measured its consequence: exit 0,
   prints `240` for `1`.
5. **The (5b) false-alarm count of 0 is over the tracked tree, 1,530 files,
   one template program.** It is a measurement of this corpus, not of the
   class (B9, item 4).
6. **The historian's `assert false` condition** (*"a Heroes `assert` can never
   be switched off"*) **is unrun by that seat.** I searched for a switch:
   `NDEBUG`, `noassert`, `no-assert`, *assertions off* and `disable.*assert`
   over `selfhost/cli/`, `runtime/` and design.md, and found none. So the
   condition looks met, but as a search, not a proof.
7. **The blind reading's rates are predictions**, each with a stated
   falsifier, and none was scored. The coordinator should not let a synthesis
   sentence quote one as a measurement.

## D. What is missing

1. **Four defects found by this sitting are filed nowhere yet**:
   - R4's widened predicate reaching value positions with no seal (§ A);
   - a C object declared as an `extern function` is exit 2, *called object
     type 'int' is not a function or function pointer* (the ffi-pragmatist's).
     I reproduced it with `function errno() -> i32` in `extern "errno.h"`,
     `build` exit 2;
   - `declaration_in_arm`'s *`_` would be bound*, false by spec § 5 (three
     seats);
   - `ffi_unknown_name`'s *declares no* for glibc's statement macro `FD_ZERO`
     (the ffi-pragmatist's).

   None is this sitting's route, and each needs a number before the synthesis
   cites it.
2. **(3b) keeps a split that is measured, not argued.** On `03e70520` a value
   arm block written `if j == 0` / `return 1` / then `return 2` checks clean
   and prints `1 2 20` (`probe/second/bcp3.hero`). The same flow written with
   `else` is refused (a69). The blind seat flagged the first as unclear under
   B2 (its choice point 3); the compiler accepts it today. That is a reason
   for (3a) no seat stated.
3. **(1b)'s mechanism must keep round 1 first.** The ffi-pragmatist measured
   `curl/curl.h`'s `curl_easy_setopt` as both a macro and a function on this
   Mac; I reproduced the macro with `#ifdef`. An `#ifdef` asked before the
   parenthesised probe would move it to the weaker form, and `memset` under
   `_FORTIFY_SOURCE` with it. The engineer's prototype asks only after a
   refused round, and the synthesis must write that ordering into the
   resolution, not leave it to the landing.
4. **Landing chores no route sentence mentions:**
   - (4a′) and (4a) fail the compiler's own test `selfhost/parse.hero:383`
     (992 of 993), which pins the certain fix on t7's shape;
   - the defect-123 golden's `.fixed` becomes an `.applied`, by hand
     (`tests/golden/check/` forbids regeneration);
   - R2's placement, `resolve/state.hero` `report_unused`, sits at 326 of a
     decided 330;
   - (5b)'s fix must double a text `}` once R1 lands (the engineer's);
   - R4 rewrites `fixedbugs-139-a-value-arm-that-ends-on-a-statement`, whose
     four `.blue => assert false` value arms stop being refused.
5. **No `errno` route.** By the ffi-pragmatist, (1d) is the only route that
   reaches `errno`, `stdin` or `optarg` at all. Whether (1b)'s note should
   offer it there is the question that seat corrected itself into, against
   design.md §4.19 `:2318-2325` (panel 038). It is still open.

## E. The questions the sitting should have asked and did not

1. **Q5: a blind reading of a NEW (5b) wording.** The author's condition sent
   *the wording* back. The brief excluded (5b) from the blind seat *"since its
   own blind reading objected"*, so this sitting holds a priced wording (+38)
   and a built prototype, and still no reading of the words. Its size, if the
   coordinator wants it: one fresh session, the blind seat's command, capped
   at 3 USD. This sitting's session cost 0.61 USD by the CLI's report. Not run.
2. **Q3: a reading of (3a) WITH R4's sentence in the spec copy.** The blind
   spec has no R4 sentence (`blind/spec-markers.diff` adds none), so B1 was
   approved without the sentence it must live beside. B6 shows that the
   wording of that pair decides whether the compiler still checks itself.
3. **Q2: is a loop on an arm's line a mistake?** It is (2c)'s whole case and
   the spec-warden's own condition. It is a question for the mutation
   instrument (`heroes mutate`; whether an operator for it exists is unrun
   here) or for a sized generation run. Nobody measured it.
4. **Q1: does (1d) build on Windows?** A `static inline` in a header of the
   program's own, under the Windows box's clang, decides whether the repair
   (1b) names is one every supported platform can use.

## F. What the synthesis must not get wrong, most important first

1. **Q3 must not adopt a text in which R4 reaches `c5`.** That covers the
   spec-warden's merged (3a) sentences, the historian's *"R4 reaches `c5`"*,
   and any wording in which an all-leaving `if` or `match` "is a jump" for
   R4. On `03e70520` that rule refuses 133 statements of the compiler, 131
   after such a `match`, and `selfhost/main.hero` goes to exit 1 (§ B6,
   measured here). Panel 184 chose (2d) to avoid exactly this. (3a)'s sentence
   must leave R4's reach where its ratified text puts it, and it is unpriced.
2. **R4's widening reaching value positions is a decision, not a detail.** As
   prototyped, c1, c2, c3 and c6 check clean and build at exit 2, reproduced
   here (§ A). Either R4 stays at a function's end, or it lands with a seal,
   and the seal's `emission` run is owed.
3. **Q1: (1a) and (1a′) carry a soundness veto that rests on a reproduced
   out-of-bounds read**, ASan exit 134, and `240` printed for `0` without the
   sanitizer. It is not "a stack overflow", and precedent does not lift it
   (§ B1). (1b) with (1d) has three seats' approval.
4. **(1b)'s note must not write the declared C types into the shim** (§ B2).
   The engineer's landing plan says it will; the ffi-pragmatist's measurement
   says that hands the author (1a′)'s hole.
5. **Q5: the author's condition on (5b) stays unmet.** No blind reading of any
   (5b) wording exists. The blind approval of C1 says nothing about (5b), and
   (5a)'s cost (32 false alarms, the compiler stopped) buys no reach (5b) lacks
   (§ B9). The synthesis puts this to the author, with the reading's size.
6. **Q2: the blind reading approves A1 and A3 alike**, so it is cited for
   neither over the other. (2c) is a new diagnostic class with no measured
   mistake behind it, against a form the ratified design already admits (§ B4).
   Whatever the route, spec text and compiler must agree on `_ = e` (§ B5).
7. **Q4: (4a) is falsified by t7, reproduced here.** What three seats approve
   is (4a′), a route of their own, and it should be labelled as one (§ B7).
   `Pattern` and the parser must agree, whichever way (§ B8).
8. **Defect 145's query is `typedef NAME hero_q;`, not `sizeof`** (§ B3).
9. **The unrun items stay marked unrun in the synthesis**: Windows and Linux
   arm64 for Q1, the seal's `emission`, the 1884-line arithmetic, the blind
   rates, and the 11-or-12 count (§ C).
10. **File the four defects of § D1** before the synthesis cites them.
