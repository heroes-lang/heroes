# Panel 252, the completeness critic's second reading: the reports against the world

Read 2026-11-19 from 17:34 CET (`date`), load 3.09 (`uptime`), after the four reports stood in
`docs/panel/252-reports/` (`compiler-engineer.md` 17:01, `historian.md` 17:32, `spec-warden.md` 16:21,
`llm-ergonomist.md` 16:45, `ls -la`). Every command ran in my own copy,
`.claude/worktrees/rebel-scratch/252-critic/tree/` (a `git archive` of `181555e9a`, its stage 2 printing
`sources 23c7d2a626b29fd5`, the frozen key), or read a seat's folder without building in it. No paid run,
no `claude -p`, no API call, no credential printed, no GitHub write, no sub-agent, no commit; one build or
run of mine at a time; no wall-clock measurement. The transcripts under the blind area's `home/` were not
read. Temporary files under `.claude/worktrees/rebel-scratch/252-critic/`.

Kinds: **wrong** (a seat's or the coordinator's fact the command contradicts), **unrun** (a claim with no
command behind it, or a negative not searched), **route** (an option nobody listed), **rule** (a rule of
`SKILL.md` or CLAUDE.md the reading does not keep), **contradiction** (two seats, which side is checkable,
and the check), **question** (what the sitting should have asked).

Written as it goes; findings numbered in the order found.

## Findings

### 1. The blind seat's cells grade as reported, the builds included; the grader's own ring test is blind and must not be cited (wrong, small; the count stands on other commands)

**What holds, each by command.** The 51 `result.md` stand whole in the report's appendix (`python3 -I`,
each file's text searched in `llm-ergonomist.md`: 51 of 51). The runner's log: `warden-at-start.md` copied
at 16:08:22 *"before any paid session"* (`run.log:1`), and its `## Registered predictions` section is
byte-identical to the report's today (W1 to W12 in both, `python3 -I` comparing the two sections), so the
predictions scored are the ones registered; `preflight held`, `spent 7.081615 USD in all`, `scan exit 0 (51
transcripts read, 0 failed a check; failures by kind: {})` (`grep` of `run.log`; `scan.txt`'s last two
lines, the temporary folder *"129 file(s) modified since 1795100903, 0 naming a session or the area"*). I did
not read the transcripts under `home/`; the scan is the instrument that did, and its controls are the
writer's (`252-briefs/llm-ergonomist.md:86-114`). **No session is voided** stands on that scan and on the
51 `context` answers, every one *no* in substance (read in the appendix).

**Task 2 graded right, and its `now` regrade is a built fact.** Task 2's program (cut from `task-t2.md`)
under my frozen stage 2: `error[cannot_infer]` at `main.hero:7:26`, exit 1. Under a compiler I rebuilt from
the compiler-engineer's nine changed files (`diff -rq` of its `selfhost/` against my frozen copy, the files
copied into `252-critic/tree-all/`, stage 2 built there): **`sources 679e4c547fb98958`, its `ce-all2`'s key**,
and the program builds and prints `2`, `1`. So the five `now` readers predict exactly what Q2-R's compiler
does. **The three `q2-c` answers that say *"line 12"*** (`t2-g-2`, `t2-g-4`, `t2-g-5`) count the lines of
`task.md`, where the second `print` stands at line 12 and the first at 11 (`cat -n task-t2.md`); each quotes
`print(nodes({"leaf": []}))` whole, calls the first line accepted in the same numbering, and commits to
*refused* in its `## prediction`, the *"if the other reading held"* line beside it. The registered row,
*"naming line 7 ... or the `[]` in it"* (`252-briefs/llm-ergonomist.md:254`), admits the quote, so 5 of 5 is the
row's own reading; **by the line number alone the cell reads 2 of 5 and Q2-C's rule does not fire**. The
synthesis states which it used; finding 5 says why neither count can rank Q2-C above Q2-R.

**The ring test of `coord/252-grade/grade.py` can never fire.** Its line is `any(re.match(r'^use
shapes\\b', l) ...)`: in a raw string `\\b` is a backslash and a `b`, so the pattern asks for the text
`use shapes\b`. Run: `re.match(r"^use shapes\\b", "use shapes")` is `False`, the corrected `\b` `True`
(Python 3.15.2, the one on `PATH`). So `grade-out.txt`'s *"area.hero uses shapes (the ring): False"*, ten
times, is the pattern's constant answer. **The count stands on two other commands**: `grep '^use '` of each
extracted file shows all ten faces `use shapes/internal/area` and **no `area.hero` holding any `use` line**,
each private function taking `(w: i64, h: i64)` (four named `of`, six `rect_area`); and the frozen builds, 10
of 10 printing `26`, could not have passed over a ring. The script goes into the evidence archive as it
stands, so the synthesis cites the grep, never that line, and the correction is written beneath it.

**Task 3, task 4 and task 5 as graded**: in finding 2 (the ceilings) and finding 3 (task 3's rubric).

### 2. The three ceilings: t1 and t5 measured the readers' route around the shape, t4 measured the trap met and turned by the spec, and none measured the half of the hazard that is silent (question; changes how the rules that read zero may be cited)

**Task 1** (`tools/task-t1.md:8-9`) asks for *"the area of one rectangle ... computed here, by a function that
`total` calls"*. Nothing asks the private function to take or give a `Rect`, so two integers are the
shortest program, and all ten took them: `of(w: i64, h: i64)` four times (the reference's own non-ring
spelling, `252-briefs/llm-ergonomist.md:158`) and `rect_area(w: i64, h: i64)` six (finding 1's grep). **The
rule read the route around the refusal, not the refusal**: a reader who never needs the face's type in the
private module never meets `module_cycle`, whatever the spec says. Q1-C's cell is no better informed: its
sentence names the case and the readers were already off it. *"Neither fires, so panel 031's R6 ruling
stands on this reading"* (`252-reports/llm-ergonomist.md:51-52`) is true of the rule and says nothing of
the refusal. **A task that leads into the shape**: the private module must construct or return the face's
type (*"`shapes/internal/grow.hero` makes a rectangle `k` times larger; `shapes.grow(r: Rect, k: i64) ->
Rect` calls it"*), where two integers in and two out is two calls and a reader reaches for `shapes.Rect`;
graded by the frozen stage 2 and the probe as task 1 was. Unrun, and a paid run (1.33 USD for ten
sessions at this sitting's t1 median of 0.1327, its `run.json` costs by `python3 -I`; t4's 1.28, t5's 1.22,
t3's 1.66): the coordinator's to decide.

**Task 4** (`task-t4.md:5`) ends *"The first line it prints is `u 3`"*. **All five `now` readers met the
trap and turned it by reading**: each `## choice_points` names `word[0]` (or `w[0]`) as a `u8` that would
print `117 3` and rejects it (`sed -n '/^## choice_points/,/^## prediction/p'` of each `result.md`, `grep -i
u8`: `t4-y-1` *"`w[0]` gives a `u8`, so it would print `117` instead of `u`"*, `t4-y-2`, `-3`, `-4` (*"Writing
`f"{word[0]}"` would have the same problem"*), `-5`). Nine of ten used `slice(from: 0, to: 1)`, `t4-y-3` a
character kept from `chars()` (the grader's files, `grep -n`). So *"the trap does not meet readers"*
(`252-reports/llm-ergonomist.md:62`) is the wrong name for the count: **the trap met 5 of 5, and today's
§ 10 sentence plus a printed expected line turned all five**. That bears on Q4 one way only: it says
nothing of a program whose output nobody states. **Every one of the compiler-engineer's four Rebel traps
stands in error text** (read at each line: `backend.hero:155` *"the server sent a message of kind
{head[0]}"*, `tzif.hero:100` and `punycode.hero:284` inside a `fail(...)` message, `session.hero:168` in the
text naming an unknown message's kind, *"a message of kind {o.kind}"*), the one place no expected output is
ever shown. **A task that leads into the shape**: a
function that refuses a string holding a byte that is not a digit, with a message naming that byte, its
expected message not printed in the task. Unrun, paid, the coordinator's.

**Task 5**: one `now` reader of five wrote `1e-9`; the nine written-out tolerances are all exactly
`0.000000001` (extracted from each answer's code, `Decimal` equal to `1e-9`; no reader used `_`). **But the
task could not have seen the silent half**: its checks are `0.1 + 0.2` against `0.3`, which differ by
`5.55e-17`, and `1.0` against `1.001`, by `1.0e-3` (`python3 -I`), so any tolerance between those prints
`true`, `false`; a dropped or added zero is graded right. The rule measured the loud half (the refusal met,
1 of 5) and nothing of the hazard `00-shared.md:430-435` brings, a written-out literal one zero off. **A
task that leads into it**: checks whose verdict flips at one decade (`near(1.0, 1.0 + 5e-10)` true and
`near(1.0, 1.0 + 5e-9)` false, written as sums of short literals), or a tolerance of `1e-15` or a constant
of twenty digits, where writing out is long. Unrun, paid, the coordinator's.

**What this changes**: the rules that read zero (Q1's two, Q4's *"meets readers"*, Q5's three) may be cited
as *"the task let readers avoid the shape"* (t1, t5) or *"readers met it and the spec turned them"* (t4),
**never as evidence that the hazard is absent**; and W1, W6, W8, W9, W10 scored *no* are scored on tasks
that could not have fired them as worded. The synthesis cannot rest Q1-R2's, Q4-R's or Q5-R's rejection
on these counts.

### 3. Task 3: both arms right by build as well as by text, and the hazard the `now` readers wrote is four silent wrong programs out of five, measured (unrun until now; a question the sitting should have asked)

**The `q3-r` answers build, monomorphised by hand.** `252-critic/tools/mono_t3.py` rewrites each `q3-r`
answer with `T` made `i64` (its `main` cut before the `Ring<str>` half) and again with `T` made `str` (the
`i64` half cut), `Ring<…>` made `Ring`, `<T>` struck from each `function`: **10 of 10 halves build under my
frozen stage 2, run 3 of 3 with exit 0, and print `3 4 5` and `green blue`** (`work2/t3mono/`). So every
`@` lent, every `[]` given a type, every label, the field lent as `@r.items`, and the order printed are the
language's today; only the type former, and a `[]` typed through it from `Ring<i64>`, are new and unbuilt.
**R1, outside the type former, and R4 hold by build for the five `q3-r` answers** (R2 and R3 by reading,
as graded), and the rubric's grade of 5 of 5 stands on more than the text.

**The hazard, run.** To each `now` answer's `main` I appended one call handing the `i64` ring's state the
`str` ring's list, then a loop printing the `str` ring by its own reader (`work2/t3mix/<session>/main.hero`):
**5 of 5 build** under the frozen stage 2; **4 of 5 then exit 0 printing `green x blue`**, a ring of two
holding three, stderr empty (`t3-y-1` to `t3-y-4`); **`t3-y-5` aborts**, *"panic: array index out of range
at main.hero:16:9, in main.ring_add"*, exit 134. So *"5 of `now`'s 5 hold an API that accepts a mismatched
pair"* (`252-reports/llm-ergonomist.md:59-61`) is now a measurement: **four silent wrong programs and one
abort, from one plausible slip**, the same class as the compiler-engineer's `cache`, `indexed` and
`ordered` (finding 9). The `q3-r` answers cannot be written that way: `add(@r: Ring<T>, item: T)` takes no
list.

**Why the rule read 5 against 5**: the rubric's four rows (`252-briefs/llm-ergonomist.md:255`) are all
satisfiable under `now`, and the hazard was registered *"recorded beside it, not a gate"* (`:270-272`). The
counted rule measured whether readers can write a working ring under each arm (both can), **not whether the
program they write can be misused** (only `now`'s can). So *"Q3-R does not serve the thesis"* is a ceiling,
not an absence of effect. **The question the sitting should have asked**: does design.md Part 11 count a
reader who writes an API whose misuse compiles and runs wrong? If yes, the measured effect is 5 of 5
against 0 of 5, a difference of 5 by the shape of the API, and Principle 0's *"a measured design.md Part 11
effect"* (CLAUDE.md § 2) is on the table for Q3-R; if no, say so in the synthesis, since W5's *"does not
fire"* is the only blind evidence the spec-warden's veto rests on.

### 4. Q1: the compiler-engineer's side is checkable and holds one shape further, a ring between two packages; the historian's entanglement reason is no compiler fact, and R6's own record is corrected at its source (contradiction, checked; changes what the synthesis may cite)

**What a command settles.** With the CE's routes rebuilt in my copy (`cr-all`, `sources 679e4c547fb98958`,
finding 1), **the one ring nobody built**: packages `alpha` and `beta` beside a program, each with its own
`heroes.toml` naming the other by `path` (spec § 1, *"Inside a package ... its brackets name entries of the
package's own catalogue"*), `alpha/a.hero` using `<beta>/b` and `beta/b.hero` naming `a.Count`
(`work2/pkgring/`). Frozen stage 2: `error[module_cycle]: ... <alpha>/a uses <beta>/b uses <alpha>/a`, exit 1.
`cr-all`: **`check` exit 0, `build` exit 0, the binary prints `41`; an edit to `beta` rebuilt warm prints
`61`, restored `41`**; each compile under `timeout -s KILL 60`, `pgrep -fl` of the folder empty after. The
compiler's own notes say a fetched package's local `path` is refused, so **a ring through packages fetched by
`version` or `git` is unrun**. So the historian's condition *"toward Q1-R2: the compiler-engineer's untried
two-package ring failing to place or build"* (`252-reports/historian.md:273-274`) is not met for local
packages, and the CE's census of what rests on acyclicity (`252-reports/compiler-engineer.md:57-69`) now
covers the shape it named as its own gap.

**What no command settles, and the record it corrects.** Panel 031's R6 (`docs/panel/031-*.md:193-198`,
read) gives two reasons: the cache's topological order and *"Go's own design paper records the same trade
and the cost it names (incremental builds)"*. The paper's source (`gh api
repos/golang/website/contents/_content/talks/2012/splash.article`, a GET, lines 336-343) names incremental
builds and then: *"More important, when allowed, in our experience such imports end up entangling huge
swaths of the source tree into large subpieces"*. Go issue 31833 holds Griesemer's *"We have deliberately
chosen to disallow import cycles both for human understanding and implementation simplicity"* and the
proposer's unanswered *"Is there a plan to solve the bloating package problem?"*; issue 30247 holds
*"Import cycles can be convenient but their cost can be catastrophic"* (`gh issue view ... --comments`, both
read). **So the historian's correction of R6 is right at the source**, and the CE's *"its reason is gone"*
retires one of R6's two reasons, the one it named; the paper's other reason is a judgement about how
programs grow, which no census of the compiler reaches. **The measured side of that judgement points the
other way in this tree**: the refusal is what made the knots, the compiler's `flatten.hero` (1,414 lines)
and `grammar_expr.hero` (1,767) by their own comments, and Rebel's mirrors, twice-written codes and event
lists (`00-shared.md:167-172`'s table; the CE's `wc -l` of them, 6,850 lines, an upper bound). The synthesis may adopt Q1-R on the compiler facts and must
write the entanglement reason down as weighed, not as answered. Q1-R2 (the shape Go declined in 31833) is
the listed route nearest the line the historian reads Rust, Java and Swift drawing at the unit that ships
together, a ring inside a face and its `internal/` and none beyond; it serves 32 of the brief's 34
witnesses (all but `time`/`fs` and `websocket`, `00-shared.md:160-165`), and the spec-warden's objection to
it is a sentence's price, not a measurement.

**One asymmetry nobody weighed**: R6 calls the refusal *"the reversible direction"*. Before `1.0.0`,
CLAUDE.md § 2 makes tightening as legal as relaxing (*"A change that refuses a `0.x` program is told, never
undone for it"*), so the asymmetry R6 bought holds from `1.0.0` alone; the question is whether the language
reaches `1.0.0` with rings allowed, and nothing in this sitting decides it later more cheaply.

### 5. Q2: neither blind count can rank Q2-C above Q2-R, the built Q2-R leaves two shapes, and § 10's sentence is the price Q2-R's *"0 tokens"* leaves out (contradiction, checked; unrun price)

**Each arm's readers are right against their own compiler.** Under the frozen compiler task 2 is refused at
line 7; the five `q2-c` readers predict that. Under the CE's Q2-R (`cr-all`, built in my copy) task 2 prints
`2`, `1`; the five `now` readers predict that (finding 1). So *"Q2-C's sentence carries"* measures that
readers follow a sentence describing the compiler, and *"the spec reads as Q2-R"* that today's spec already
describes the repaired one: **5 against 5, each right on its own route**. The count does not choose between
the routes; Principle 0 and CLAUDE.md § 12 do, and the spec-warden's veto of Q2-C (+21 real, an
order-dependence written down) is untouched by it. The historian's precedent (rustc's first-element typing
removed as *"accidental"*, PR 140283, `252-reports/historian.md:340-363`) is advisory and points the same
way.

**What the built Q2-R covers, re-run** (the CE's 37 programs copied to `work2/q2/`, `check` under my frozen
stage 2 and `cr-all`): all 37 read as the CE's table, 31 accepted under `cr-all`; `e`, `i`, `x06`, `x17`
stay refused rightly; **`x07` (`b: u8 = 7` then `[1, b]`, `error[type_mismatch]: expected i64, found u8`,
its reverse `x08` accepted) and `x14` (`count({name(): []})`, `cannot_infer` with the same false *"no other
argument and no context says what `K` is"*) stay refused**. The spec-warden's premise 3 (*"Q2-R's repair as written would leave
`[[], ["a"]]` refused"*) is true of the brief's words and **false of the built route**: `x01`, `x03` and
`x05`, the plain literals refused for their order, build under `cr-all`. So the route the synthesis may
adopt is the CE's built one, the literal's lead plus its typed parts, with `x07` and `x14` named as open
and `x14`'s message still false; *"one verdict in either order"* holds for every pair the 37 measure but
`x07` against `x08`, which is not every literal.
The CE's premise 4 holds too: `fixedbugs-402-*.hero:57` blesses `fs = [ident, twice]  #~ cannot_infer`, its
header naming the shape *"first in an array literal"*, and `[twice, ident]` builds under the frozen
compiler (`work2/q2b/r3`, exit 0; `r4`, exit 1); both build under `cr-all`. A pinned golden moves under Q2-R,
read at the gate, not a ruling against it (the header cites panel 105 for another shape).

**The price nobody counted.** Spec § 10 (`spec/heroes-spec.md:320-321`): *"a container literal ... an
empty one needs an annotation: `xs: [i64] = []`"*. The frozen compiler already accepts an unannotated `[]`
in `{"a": ["b"], "b": []}`, and Q2-R accepts it in seven more of the brief's programs. **All five `now`
readers of task 2 weighed that sentence** (`grep -c -i annotation` of each `result.md`: 1 to 3) and read it as
*"only when nothing gives the literal a type"* (`t2-y-1`'s choice point 1). So the readers resolve it the way
Q2-R needs, measured; and the sentence is false as worded under either compiler, which is panel 035's
*"worst possible spend"*. The spec-warden raised it as *"a question for the synthesis, not a price"*
(`252-reports/spec-warden.md:208-209`); I would put it as a price: Q2-R at 0 tokens leaves a sentence the
repair makes falser, and a few words (*"an empty one that nothing else types needs an annotation"*) are owed
or the choice not to pay them written down. Unpriced, unrefreshed.

### 6. Q3: the frozen tree's count is 288 in 109, the compiler cannot show a need either way, and the spec-warden's veto rests on a rule that read a ceiling (wrong, small; contradiction, checked)

**The count.** The CE's experiment copies (`ce/exp/q3/`, `ce/exp/q1a/`) were cut from its route-modified
`selfhost/`: `diff -r` against my frozen copy shows `check/byte_text.hero` and the Q2-R files in both.
Its two one-case changes applied alone to a fresh copy of the **frozen** `selfhost/` (`work2/exp-q3/`,
`exp-q1a/`), `heroes-next check selfhost/main.hero`: **Q3's `applied` case, 288 `non_exhaustive` in 109
files** (no other error); **Q1-A's `alias_decl`, 322 in 172**, the CE's figure. So *"290 in 111"* counts two
errors in two files of the CE's own routes, and its Q3 prediction (*"touches at least 111 files"*,
`252-reports/compiler-engineer.md:325-326`) is 109 on the frozen tree, exactly panel 251's carried figure for
`(thread T)`.

**Compiler need.** `selfhost/` holds one generic function in all (`grep -rE '^function \w+<' selfhost`:
`fetch/verb_place.hero:95`, `unit_of<T>`). A compiler written with no generic code cannot show a generic
container's need, so the CE's *"no"* is true and says little; the brief's second shape, *"a shape and a list
passed side by side"* (`252-briefs/compiler-engineer.md:126-128`), went uncounted, and on this evidence
would read zero for the same reason. **Principle 0's ticket for Q3-R can come only from Part 11.**

**The three seats.** CE *object* (cost, no need), spec-warden *veto unless task 3's rule fires*, historian
*approve as the direction* (Go's 43651 *"we also want generic types"*, Dart, Go 1.27's generic methods). The
checkable part is the blind evidence the veto names, and finding 3 shows it: both arms 5 of 5 by a rubric
the hazard never entered, and the hazard itself 5 of 5 compile, 4 of 5 silent. **So the synthesis may keep
Q3-C and the veto on Principle 0's burden unmet, but not cite W5's *"does not fire"* as evidence the form
buys nothing**: the rule could not fire on a task where both arms write a working ring. The registered
alternative is a task where the hazard bites (two rings of one type, interleaved, so a mismatched pair
changes the output), its rule written before it runs; unrun, paid, the coordinator's.

**Q3-C's mechanism.** The CE's slot hash (an `[i64]` of each slot's hash, O(1) per touched slot) is
*"unbuilt"* and needs *"a hash of any `K` the package can call"*; it would close the package's four false
promises and none of the five readers' rings, which hold no hash. Q3-C repairs `containers`; it does not
close the class the readers wrote.

### 7. Q4: robustness does not reach a byte's number, so the refusal stands on Principle 0's ticket alone; the census's *"4"* is one reader's floor, and its share hangs on its denominator (contradiction, checked; unrun)

**Rank.** The spec-warden approves the refusal as *"robustness, rank 3"* (`252-reports/spec-warden.md:239`);
the CE says *"§1.12 does not reach it"*. design.md §1.12 (`docs/design.md:621-629`, read): *"A Heroes
program must not segfault and must not corrupt memory"*. Panel 215, whose words *"robustness decides THAT
a measured class of wrong programs closes"* the spec-warden cites, closed a lock copied by value and padding
leaked to C (`docs/panel/215-*.md:110-116`, `:176-184`). **The checkable side is the CE's**: printing `117` is
a defined value, neither a fault nor a corruption. So the refusal, and the spec-warden's Q3 sentence *"by §
Precedence it closes now"* for the same reason, cannot rank at 3; Q4-R and Q4-R2 rest on Principle 0's
ticket (*"a measured design.md Part 11 effect"*) or on the thesis line of CLAUDE.md's first paragraph, never
on § Precedence's robustness.

**The census, re-read.** `ce/out/q4-sites-src.txt` holds 226 sites: 63 in Rebel's packages, 6 in Rebel's
drivers (colour channels, number meant), 157 in goldens (`cut -d: -f1 | sort | uniq -c`); the census diff
moves 90 drivers, 42 `run` roots and 14 `check` cases (`grep -c` by directory), as reported. I read all 69
Rebel sites. **By the CE's own criterion, a text protocol's kind byte is a letter** (it counts
`postgres/internal/backend.hero:155`, *"a message of kind {head[0]}"*): **`redis/internal/resp.hero:229`**
is the same shape, *"the reply holds the byte {kind} at byte {at}, which begins no RESP type"*, its code
comparing `kind` with `'*'`, `'%'`, `'~'`, `'|'`, `'>'` nine lines above (`:220`, read) and with `'$'` at
`:187`; and
**`qr/internal/bits.hero:94`** writes the byte's number beside the alphabet it failed, *"holds byte
{data[at]} ... which is not one of \"{ALPHANUMERIC}\""*. That makes **6 of 69** by the CE's criterion
(8.7%). Nine more stand in text formats under messages that say *"byte"* (`acme.hero:603`,
`asn1/internal/reals.hero:143`, `times.hero:31`, `:75`, `texts.hero:72`, `mail/internal/transfer.hero:159`,
`pdf/internal/parse.hero:371`, `qr/internal/bits.hero:85`, `archive/internal/tar_header.hero:148`): the
number agrees with the word *"byte"*, and the character is what a reader of the message wants; **up to 15 of
69 (22%)** on that reading. The goldens' 157 are the compiler's own tests of what a byte prints, chosen to
print numbers. **The CE's condition, *"a census where the character-meant share exceeds 10%"*, reads 1.8%
over the whole census, 6 to 9% over Rebel by the CE's criterion, and up to 22% over Rebel by the wider
one**; Rebel is the only corpus written by model agents, which is the thesis's population. A second reader
moves the verdict's own threshold; the synthesis may not rest Q4-C on *"1.8%"* as a property of the trap.

**The class's words.** The spec-warden's *"no new class is owed"* rests on reusing `hole_not_renderable` and
`bad_operand`. Run: a record in a hole prints *"`to_str` takes an integer, a float, `bool` or `str` — this
one's type is `R`, which has no text"* (`work2/q5/hole.hero`); of a `u8`, an integer `to_str` takes and
renders, those words are false, and `bad_operand`'s *"`print` takes any integer"* likewise. So either a new
class (the CE's) or both classes reworded: the CE's reason holds, and the spec-warden's choice is a reword,
not a free reuse.

**Where the traps stand** (finding 2): all six of finding 7's stand in error or description text, each
inside an `f"..."` hole. A route nobody listed is narrower than Q4-R and of another kind: refuse a `u8` in a
hole **of an `f"..."` literal only**, leaving `print`. The census says what it buys: Rebel's 63 package
sites are 63 holes of 63 and its 6 driver sites 6 `print` arguments (`grep -c 'f"'` of each line), so it
would spare the six drivers and refuse the other 57 number prints of Rebel; named, priced, not recommended.

### 8. Q5: § 11 already says *"too large is `inf`"*, so the range refusal is a ruling with a sentence to price, not a defect *"owed either way"*; and the silent zero is today's too (wrong framing; unrun price)

**Reproduced under my frozen stage 2** (`work2/q5/`): a float literal of 401 digits builds and prints
`inf`, exit 0; an `f32` of 41 digits, `inf`; **and a literal of `0.` and 400 zeros and a `1` builds and prints
`0.0 true` against `0.0`, exit 0**, the underflow side the CE measured only under Q5-R (`1e-400`). `print(inf)`
is `error[unknown_name]`, `to_f64("1e-06")` `error[bad_operand]`, and `x / x`, `-1.0 / x` print `nan -inf`:
the spec-warden's premise 2 holds, and no route makes `inf` or `nan` readable.

**What the seats did not weigh**: spec § 11, `spec/heroes-spec.md:356-357`, *"Nothing fails to fit a
float: too large is `inf`, and `to_f32` rounds"*. It stands in the conversions' paragraph; whether it binds
a literal is a reading, and on the reading a reader of the spec would take, the 401-digit literal does what
the spec says. So the CE's *"a defect ... `blocking` ... a wrong value"* and *"the refusal is owed either
way"* (`252-reports/compiler-engineer.md:354-361`, `:387-388`) are a ruling this sitting would make, a new
refusal and a diagnostic class, which (i) scopes or amends § 11's sentence and (ii) owes its own words in
§ 2. **Neither is priced**: the spec-warden's `w5-r-strtod-u` (+56 vendored) names no range refusal, and the
CE's *"about +20 lines"* is compiler lines alone. The historian's Java precedent (JLS § 3.10.2, both
directions refused) is the shape to price. **The synthesis may adopt the range refusal only with its
sentence priced and § 11:356 reconciled**; filed as a defect against today's compiler, it must cite § 11:356
and say which reading it takes.

**The contradiction over Q5-R.** The CE and the historian approve Q5-R; the spec-warden approves it only
*"if W9's rule fires, else Q5-C's form"*, and W9 read *no* (5 against 4). By finding 2, W9's task could not
fire it: four readers in five wrote the tolerance out and none was graded on a value a zero would change.
So the spec-warden's own registered condition, read literally, routes to `w5-c-u`; read for what it
measured, it is silent. The synthesis must say which, since it decides whether `1e-06`, which `print` writes
today, becomes a literal.

### 9. The defects the seats name: each reproduced by its seat's program in my copy, each the frozen tree's, none filed (checked)

The branch holds one commit past the freeze, `f4e3594d5`, touching `docs/panel/251-reports/evidence.md` and
`evidence.tar.xz` alone (`git log --oneline 181555e9a..HEAD`, `git diff --name-only`); `origin/main` has no
commit under `selfhost/` or `spec/` since the merge base (`git log 181555e9a..origin/main -- selfhost spec`
prints nothing) and holds no `packages/rebel/` at all. Issues searched, reads only (`gh issue list --state
all --search` for *float literal infinity*, *literal inf*, *atof*, *float literal overflow*, *literal rounds
to zero*, *deque item list*, *containers promise*, *pop_front*, *module_cycle note*, *byte as number*): none
names any of these; the nearest, defect 2276 (an `f32` literal read through a double), is closed and another
shape.

| defect | seat | reproduced here | whose tree |
|---|---|---|---|
| `deque`'s promise to abort on a list not its own | brief-writer | `taken 10`, exit 0, 3 of 3 each, at my first reading (`critic-briefs.md:199-200`) | the branch's (Rebel) |
| `cache`, `indexed`, `ordered`: the same promise, and a key written into the other map | compiler-engineer | its `ce_*_mix2.hero` copied into my `examples-rebel/`, built by my frozen stage 2: `2 2 1 20 true` (`cache`, `indexed`: the second map holds `"q"` worth 20, never put there), `2 2 one seven true` (`ordered`), exit 0, empty stderr, 3 of 3 each; `ce_cache_mix.hero` aborts exit 134, *"it is not the list the structure was built over"*, 3 of 3 | the branch's |
| a float literal past range is `inf`, in silence | compiler-engineer | 401 digits, `inf`; an `f32` of 41, `inf`; **and 400 zeros then `1`, `0.0`, equal to `0.0`**, each exit 0 (`work2/q5/`) | the frozen tree's and `main`'s; whether it is a defect turns on § 11:356 (finding 8) |
| four Rebel holes writing a letter's byte as its number | compiler-engineer | the four lines read in place; two more of the same shape (finding 7) | the branch's |
| `module_cycle`'s note sends a face into `private_module` | brief-writer | `bw/q1/third/` copied: `main.hero` checks, exit 0; `main2.hero` `error[private_module]`, exit 1; `bw/q1/cycle/` `error[module_cycle]` with the note | the frozen tree's and `main`'s; a message's words, no class |

The note is incomplete rather than wrong: the brief's own `bw/q1/public/` (a public module beside the face)
follows it and builds. Under Q1-R the note leaves with its class; under Q1-C its words owe the public third
module.

### 10. Every premise a seat called false in its brief, judged (checked; two of fourteen are wrong or only half right)

| # | seat's premise | verdict | the command |
|---|---|---|---|
| CE 1 | there is no `heroes doc` | **right** | `grep -c 'Command(' selfhost/cli/table.hero` 16; `heroes-next doc`: *"unknown command `doc`"* |
| CE 2 | `module_cycle`'s comments are 22 lines in 20 files, not eight | **right** | the CE's `grep -rn -i -E` outside `modules.hero`: 22, 20; `suite_layout.hero` `:18`, `:75`, `:251` |
| CE 3 | the census is 1,924 roots, not 1,852 | **a definition, not an error** | the brief's 1,852 counts files holding `function main(` (`grep -rl`: `run` 837 = 811 + 26, `check` 783 = 687 + 96); the CE's adds the 69 top-level `check` cases holding none (`grep -L`). Both right by their commands; the CE's is a superset and moves nothing |
| CE 4 | an order-dependence inside one literal is pinned already | **right** | `fixedbugs-402-*.hero:57`, its header's shape *"first in an array literal"*; `work2/q2b/r3` exit 0, `r4` exit 1 (finding 5) |
| CE 5 | Q2-R as worded lifts refusals inside `ok(…)` | **right** | its first census (`ce/out/census-diff.txt`): `cannot_infer` gone in 544, 551, 552 |
| CE 6 | `cache`, `indexed`, `ordered`'s promises are false | **right** | finding 9 |
| CE 7 | a float literal past range is a silent `inf` today | **right as a measurement; its classification is a reading** | finding 8, § 11:356 |
| CE 8 | three of four Rebel traps read a `[u8]` | **right** | `backend.hero:155` `head[0]`, `session.hero:168` `o.kind`, `tzif.hero:100` `data[...]`; `punycode.hero:284` `s[at]` alone indexes a `str` |
| SW 1 | `q1-c`'s sentence is false | **right** | `work2/sib/`: `geom/internal/calc` uses `geom`, `check` exit 0, build, prints `25` |
| SW 2 | Q5-R does not make `print`'s output read back in the language | **right** | finding 8's runs |
| SW 3 | Q2-R's repair as written leaves plain literals refused | **half right**: true of the brief's words, false of the built route | finding 5 (`x01`, `x03`, `x05` under `cr-all`) |
| SW 4 | Q4-R owes no new class | **wrong as stated**: reuse needs both classes' words changed | finding 7 |
| H 1 | design.md §4.12's Go note is not Go's reason | **right** | `gh api repos/golang/go/contents/doc/go_faq.html?ref=go1.12`: *"Polymorphic programming did not seem essential ... and so was left out for simplicity"*, the containers and the empty interface a *"Meanwhile"*; design.md:2022-2023 says *"precisely because the three built-in containers covered most needs"* |
| H 2 | Python 3's `"use"[0]` is a string; `bytes` gives the `int` | **right** | `python3 -I -c 'print(repr("use"[0]), repr(b"use"[0]), f"{b"use"[0]}")'`: `'u' 117 117` (Python 3.15.2) |

The historian's premises 3 to 5 (Go 1.27's generic methods, R6's incomplete citation, JSON's frame) I checked
only where a contradiction leaned on them: R6's, at the paper's source (finding 4). Go 1.27 is unread here.

### 11. A task's reference holding the shape does not show that the task leads a reader into it (rule; for `SKILL.md` § 2's blind-seat bullet, the coordinator's to propose)

Panel 251's critic found a task whose input could not show the shape, and this sitting's answer was that
*"every task here was run first as a reference holding the shape"* (`00-shared.md:553-555`). The references
held it: `bw/ref/t1` builds the ring and is refused, `t4` prints `117 3`, `t5` writes `1e-9`. **Three of five
tasks still let the shortest correct program avoid it** (finding 2): t1 asks for a function `total` calls
and never for one over `Rect`; t4 prints the expected first line; t5's checks pass any tolerance within
thirteen decades. The reference was the writer's program, written to hold the shape; the readers wrote
the program the task text made shortest. **The check that would have caught it before 7.08 USD was spent,
at no cost**: write the shortest program the task's text admits, and ask whether it holds the shape and
whether the grading tells a wrong value of it from a right one. Run on these five task files by reading
them (`cat -n tools/task-t*.md`), it flags t1, t4 and t5, and passes t2 and t3; t3 then needs the second
question, since its rubric cannot see the hazard (finding 3).

### 12. Smaller points, each with its command

- **The CE's *"149 roots move"*** (`252-reports/compiler-engineer.md:408`) is its first census's count
  (`ce/out/census-diff.txt:1`, *"roots compared: 1802 moved: 149"*); `ce-all2`'s, which the report
  describes, reads **146** (`census-diff2.txt:1`; 90 drivers, 42 `run`, 14 `check`, `grep -c` by directory),
  and 0 of the 122 directory cases.
- **The spec-warden's removal list** names *"the eight `module_cycle` comments"*
  (`252-reports/spec-warden.md:160-162`); there are 22 lines in 20 files and three in `suite_layout.hero`
  (finding 10, CE 2). Q1-R's correction is those 25 lines and design.md §4.1's clause.
- **The spec-warden approved Q1-R *"on the compiler-engineer's census"*** at 16:21, forty minutes before that
  census existed (the CE's report closed 17:01, `date` in each). The census now stands and holds, one shape
  further (finding 4); the condition is met, and the synthesis says it was met after.
- **No blind session could write `report.md`**: `ls s/*/report.md` finds none, and 43 of 51 `result.md` say
  in their own words that the session had no write tool (`grep -l -i -E` for *"only have a file-reading
  tool"*, *"can't write"*, *"couldn't write"* and kin), under `run.sh:55-56`'s `--tools "Read,Write"
  --allowedTools "Read,Write"` with `--bare --restricted`. `brief-session.md:32`'s fallback (*"give the whole
  report as your final answer instead"*) caught all 51, so no grade moved, as panel 251's 51 did. Why `Write`
  is absent is unmeasured, and finding it costs a paid session: a question for the coordinator, since a task
  that asks for several files depends on the fallback's single answer.
- **The historian's PDF** outside the root (`252-reports/historian.md:19-23`) is gone: `test -e` of the named
  path prints *absent*.
- **Design.md §4.12's Go note** (`docs/design.md:2022-2023`) is not what Go's FAQ says (finding 10, H 1), and
  its *"~90% of real generic use here too"* cites nothing (`grep -n '90%' docs/design.md` prints it at
  `:2023` and one other line, `:1974`, advanced generics as *"90% of the implementation cost"*, which names
  no source either). A correction beneath, dated, if the synthesis leans on §4.12's rationale; Q3's ruling stands on
  Principle 0 and needs neither.
- **The spending**: 7.081615 USD of the author's 10 (`run.log`), the refreshes counted 0 by the page the
  brief quotes; no other paid run by any seat that I found in their reports.
- **Not re-run here**: the CE's whole census (I re-checked six roots of it, three moved and three not, each
  as its diff says), its 15,988 character literals, the spec-warden's thirteen refreshes and its `spec` and
  `grammar` suite runs over the arms, design.md:290-293's consolidation block against `ROADMAP.md`, and the
  historian's pages beyond the five I read through `gh` and the four facts I ran.

## What held, so the synthesis need not re-pay for it

Each by its command, in my copy or by reading the seats' files: the CE's routes on disk are what it reported
from (rebuilt, `sources 679e4c547fb98958`); its twelve Q1 cases' verdicts as tabled (read, not re-run, but the
two-package ring added and run); its Q2 table, 37 programs, every verdict; its Q4 site list's counts and
the census diff's 146 moves by directory (90, 42, 14), and six roots re-checked under both compilers each as
the diff says; Q1-A's 322 in 172; the `heroes doc` absence and the 22 comment lines; `fixedbugs-402:57`; the
first census's lifted `ok(…)` refusals; the four `containers` promises false; the silent `inf`; the
spec-warden's sibling module, its `inf`/`nan`/`to_f64` runs and its record-in-a-hole message; the blind
run's predictions registered whole before the first paid session, its scan (51 read, 0 failed), its spend
(7.081615 USD), its 51 `context` answers, its 51 answers in the appendix, and every grade of every cell
(t1 and t4 and t5 by the builds in `grade-out.txt` and my greps, t2 by reading all ten, t3 by building the
`q3-r` answers monomorphised); the historian's Go quotes for Q1 at their sources through `gh`, the Go FAQ at
`go1.12`, and Python's index by running it.

## The findings that change what the synthesis may adopt

1. **The rules that read zero measured the readers' route, not the hazard** (finding 2, finding 11): t1's
   task never asks the private module for a `Rect`, t4's prints the expected `u 3` and all five `now`
   readers name `word[0]` as a `u8` and turn away, t5's checks pass any tolerance within thirteen decades.
   W1, W6, W8, W9 and W10's *no*, and *"R6's ruling stands on this reading"*, may not be cited as evidence
   the hazards are absent; the rejections of Q1-R2, Q4-R and Q5-R cannot rest on them.
2. **Q3's rule read a ceiling, and the hazard it recorded is four silent wrong programs of five** (finding 3,
   finding 6): the `now` answers compile a mismatched call 5 of 5 and four exit 0 printing a two-item ring
   holding three. Q3-C and the veto may stand on Principle 0's burden; W5 may not be cited as the form
   buying nothing; Q3-C closes `containers`' four promises and none of the readers' rings.
3. **Q1**: the compiler needs no acyclicity one shape further, a ring between two packages built, rebuilt
   warm and run under the CE's Q1-R; R6's recorded reason was Go's lesser one (verified at the paper's
   source and in issues 31833 and 30247). Q1-R may be adopted on the compiler facts with the entanglement
   reason written as weighed; a ring through fetched packages is unrun (finding 4).
4. **Q2**: neither count ranks Q2-C over Q2-R (each arm's readers right against its own compiler, Q2-R's
   built); the adoptable Q2-R is the CE's built route, `x07` and `x14` named open; § 10's *"an empty one needs
   an annotation"* is a price Q2-R's *"0 tokens"* leaves out; Q2-C's *"carries"* is 5 by quote, 2 by line
   number (finding 1, finding 5).
5. **Q4**: rank 3 does not reach a byte's number (§1.12's text; panel 215's class), so the refusal needs
   Principle 0's ticket; the CE's *"4 of 226"* is one reader's floor, 6 of Rebel's 69 by its own criterion
   and up to 15, its 10% condition crossing or not by corpus and reader; reusing `hole_not_renderable` needs
   its words changed (finding 7).
6. **Q5**: § 11:356 says *"too large is `inf`"*, so the range refusal is a ruling, unpriced in spec tokens,
   not a defect *"owed either way"*; the silent zero is today's too; the spec-warden's condition for Q5-R
   rests on W9, which read a ceiling (finding 8).
7. **The frozen tree's Q3 price is 288 in 109**, not 290 in 111 (finding 6), and the CE's second census moved
   146 roots, not 149 (finding 12).

## The rest

Finding 9 (the five defects reproduced, each the frozen tree's, `main`'s for the two compiler ones, none
filed), finding 10 (fourteen premises judged: ten right, one a definition, one half right, one right as a
measurement with its classification a reading, one wrong as stated), finding 12 (the spec-warden's
eight comments are 25 lines; its Q1-R approval met after it was written; no blind session had `Write`,
43 of 51 saying so; the PDF gone; design.md §4.12's Go note and its two unsourced *"90%"*; the spending;
what I did not re-run).

**Tally**: twelve numbered findings. **wrong** 4 (1's grader test, 6's 290/111, 12's 149, 10's SW 4);
**unrun** 4 (3's hazard and monomorphised builds, now run; 5's § 10 price; 7's second reading of the census;
8's range sentence); **route** 2 (2's three leading tasks; 7's hole-only refusal); **rule** 1 (11);
**contradiction** 5 (4, 5, 6, 7, 8, each checked on its checkable side); **question** 3 (3's Part 11 reading
of a misusable API, 4's `1.0.0` asymmetry, 12's missing `Write`).

Finished 2026-11-19 at 18:08 CET (`date`), begun at 17:34. One stage 2 built
(`252-critic/tree-all/cr-all`), no other build of the compiler; every program run under `timeout -s KILL`,
`pgrep` of my folders empty after; no file written outside `.claude/worktrees/rebel-scratch/252-critic/` but
this report; no paid run, no credential printed, no GitHub write, no sub-agent, no commit; the transcripts
under `home/` not read.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
