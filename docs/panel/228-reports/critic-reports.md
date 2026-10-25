# Panel 228, the completeness critic's second pass: the reports

Read 2026-10-25 (the clock in CET since the night's change, `+0100`) over every file of
`docs/panel/228-reports/` (`compiler-engineer.md`, `spec-warden.md`, `historian.md`,
`ffi-pragmatist.md`, `llm-ergonomist.md`, `blind/key.txt`, `blind/scores.json`, `blind/score.py`,
`blind/run_one.sh`) and the repaired briefs (`00-shared.md` rewritten, `blind/README.md` the second
design, `blind/variants.py`, `blind/variants/*.md`, the four seats' briefs). Every command was run in
`.claude/worktrees/228-critic` (detached at `9a38f1b26`, its own `heroes-next`, `sources
451802aa66692eb3`) unless it names another folder; a seat's file was read where it stands and copied
here before anything ran on it. Written as it goes.

## Findings

### 1. The adopted A' document crashes the idiomatic C reader on the newest compiler: the A'/D quarrel is not about the number

**The fact.** The compiler-engineer's adopted build (A') writes a statement hole with no `"expected"`
key and no `"fields"` key, and a type it does not know as `null`: its own answer for `s3`,
`228-compiler-engineer/ce228/a1/hole_s3.err`, reads `"position": "statement"`, `{"name": "v",
"type": null, ...}`, and no `"expected"`. The ffi-pragmatist measured that shape, a key present on
one hole and absent on the next, as json-c's idiom crashing (its § 3 item 4, § 10), on its own
prototype. Nobody ran the ffi-pragmatist's consumers on the compiler-engineer's documents.

**The command.** `a_naive.c`, `a_schema.c`, `a_getex.c`, `a_lax.c` with `common.h` and `fill.h`
copied from `228-ffi-pragmatist/fp228/consumers/`, the eleven `.err` of
`228-compiler-engineer/ce228/a1/` copied beside them (`critic228/second/cross/`); each consumer
`clang -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -I/opt/homebrew/include ...
-ljson-c`, exit 0, no warning; then each consumer on each document:

```
document      a_naive   a_schema  a_getex   a_lax
hole_both     1         1         1         1
hole_main     3         3         3         3
hole_s1..s2   3         3         3         3
hole_s3       139       139       3         3
hole_s4       3         3         3         3
hole_s5       139       139       3         3
hole_s6       139       139       3         3
hole_s7       139       139       3         3
w1_main       3         3         3         3
w2_main       3         3         3         3
```

**What it means.** The idiomatic read crashes on 4 of the 11 answers of the route two seats approve,
**on the newest compiler, at schema 2, with no older `heroes` anywhere**: every hole in statement
position. `s7` (`if a > 1` / `???`) is a true statement hole, not defect 2204's, so no repair of
2203, 2204 or 2205 removes it. The compiler-engineer's verdict on A' and its condition say nothing of
it; the spec-warden approves A' on parity alone. So the seats' disagreement *A' against D* is two
questions the routes tie together: (a) the **number** (2 or 3), which protects a consumer from an
**older** answer, and (b) **the shape of every hole and every document** (every key always present,
no `null`, the member in every document), which protects it from the **newest** one. The
ffi-pragmatist's conditions 1, 2 and 4 are (b), and they bind A' as built exactly as they bind D.

**What the synthesis must answer.** (b) apart from (a): whether every hole carries every key
(`"expected"` as `""` and `"position"` on every hole, `"fields"` and `"cases"` always arrays, a type
never `null`), ruled for whichever number it adopts. A' as built does not meet it.

### 2. `"holes": []` beside an error: the compiler-engineer's side is the checkable one, and reproduces

**The disagreement.** The spec-warden (§ 6) recommends *the compiler reports every hole the type stage
reached, beside the errors, in both renderings*, the member absent only where the type stage did not
run. The compiler-engineer built that (step 2) and did not adopt it: beside an error the list is
incomplete, so the member is written only where every stage was quiet. The ffi-pragmatist's condition
1 asks for the member **in every document at every exit**, and its own measured safe shape, `S-both`,
is `A-both` with `"holes": []` added (its § 7): `both.hero` holds a hole at 3:14, so the document it
measured as *reads right* says, of a program with a hole, that it has none. Its consumers read
`"diagnostics"` first and never reached that claim.

**The command.** `heroes-228b` (the compiler-engineer's step 2, `sources 507385db0d10a397`) and its
probes copied into `critic228/second/beside/`, then `heroes-228b check --json <f>` and the document
parsed by `python3 -I`:

```
arity.hero   (n = pair(???), pair takes two)    exit 1  diagnostics ['wrong_arity']   holes []
stage4.hero  (z = "a" - 1 / w: i64 = z + ???)    exit 1  diagnostics ['bad_operand']   holes [(3, 18)]
both.hero    (a: i64 = "x" / b: i64 = ???)       exit 1  diagnostics ['type_mismatch'] holes [(3, 14)]
```

`arity.hero` holds `???` at 5:14 and the type stage ran: *every hole the type stage reached* is `[]`,
which a reader cannot tell from *no hole*. So the spec-warden's repair, written as a list, is a false
answer beside an error, the class the spec-warden itself calls `blocking`.

**And the quiet path is complete, over the shapes tried.** `critic228/second/positions/count.sh`
compares, per program, the `???` written (`grep -o -F '???'`), the holes `check` reports (`grep -c
'^hole at'`) and `build`'s `no binary: N hole(s)`: 14 programs that check at exit 0 (a hole in
`print(...)`, an index, a field's record, a receiver, `fail(code: ...)`, a `match` arm, a `match`
scrutinee, a map value, two in an array, a `while` condition, two operands, a concatenation, an
`assert`, an `@` re-binding), **14 of 14 equal**, 16 holes written, 16 reported, 16 counted by
`build`; plus holes in a `test` block, an uncalled function, a generic function and an uncalled
function of another module (`critic228/second/*.hero`), each reported. A negative over these
shapes only.

**What the synthesis must answer**, since no seat proposed it: a member that is **never absent and
never a false list**. Either the list is written only where it is complete and a second member says
so, present on every document, or the member's absence is ruled to mean *unknown* and every reader is
told to refuse it. The ffi-pragmatist measured the second as the idiom's crash; the first is
finding 4.

### 3. Defect 2204 is wider than filed, at exit 0, on the path every seat approves

**The fact.** A hole whose value is used is told `position: statement — any type, or none` at more
value positions than 2204 lists (a `for`'s array, an `if`/`match` branch, `x = ???`), and a value of
the wrong type is refused at each:

```
xs[???]          told any type   xs["a"]           error[type_mismatch]: expected `i64`, found `str`
while ???        told any type   while "a"         error[not_bool]: a `while` condition must be `bool`
print(???)       told any type   print(P(x: 1))    error[bad_operand]: `print` takes any integer, a float, `bool` or `str`
f"n is {???}"    told any type   f"p is {p}"       error[hole_not_renderable]: ... this one's type is `P`
```

(`critic228/second/positions/p02_index`, `p10_while_cond`, `p01_print` and their `r*` twins;
`critic228/second/fhole`, `frecord`, `funit`.) Also told *statement* at exit 0, wrong-typed values
untried: a receiver (`???.len()`), a `match` scrutinee, a map value, an `assert`'s `==` operand. Of
the 14 quiet programs above, **7 tell at least one value hole *statement***.

**What it changes.** The compiler-engineer prices 2204's repair at the `for`, `born`, `if` and
`match` sites of `check/walk.hero` (0 lines of room); the class reaches the index, the `while`
and `assert` conditions, the built-in calls and the interpolation, each a `synth` with no
expectation, so the repair is larger than priced and question 5's wait is longer. And a document
carrying `"position": "statement"` today is false on half the sampled programs. The coordinator
widens 2204 (`edit`, it is open) or files the rows; this pass files nothing.

### 4. A route nobody listed, measured: a positive completeness flag, whose absence both libraries read as *not known*

**The fact.** Panel 193's advice, *check `"schema"` first*, fails under A because the number does not
move (ffi-pragmatist § 3 item 2). A member whose **absence** is the safe answer does move: an older
`heroes` writes no such member, and both libraries' idiomatic reads return false for an absent
boolean. So a document carrying `"holes"` **always**, plus `"holes_complete": true` only where every
stage was quiet (the list then complete, finding 2's 14 of 14) and `false` beside an error (the list
then the holes the type stage reached), answers the three seats' conditions at once: never absent
(the ffi-pragmatist's 1), never a false complete list (the compiler-engineer's `arity.hero`), the
holes beside errors reported (the spec-warden's § 6), at schema 2, with nothing moved on the
`"applied"` kind or on today's readers (the compiler-engineer's price of D).

**The command** (`critic228/second/flag/`): four documents written by `docs.sh` (today's, a quiet
holed one, a quiet clean one, one beside an error with `"holes": []` and `"holes_complete": false`);
`flag_jsonc.c` reads in json-c 0.19's idiom, `json_object_get_boolean(json_object_object_get(doc,
"holes_complete"))` then `json_object_array_length(json_object_object_get(doc, "holes"))` only if
true; `flag_cjson.c` the same in cJSON 1.7.12's idiom (`cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive
(...))`, the copy `cjson/` taken from `228-ffi-pragmatist/fp228/cjson/`, `cJSON.c` digest
`06885ac9e22ca587`, whose `cJSON_IsTrue` returns false on NULL, `cJSON.c:2785-2790`). Both `clang
-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion`, exit 0, no warning:

```
today          json-c: complete=0 errors=0 refused: the holes are not known   exit 2 | cJSON: the same, exit 2
quiet_holed    json-c: complete=1 errors=0 holes=1                             exit 3 | cJSON: exit 3
quiet_clean    json-c: complete=1 errors=0 holes=0                             exit 0 | cJSON: exit 0
beside_error   json-c: complete=0 errors=1 refused: the holes are not known   exit 1 | cJSON: exit 1
```

json-c's header does not say what `json_object_get_boolean` does with NULL (`json_object.h:691-703`);
the run says 0.

**What would make it right, and wrong.** Right where every reader is told to read the flag before
the list, which, unlike *check the schema first*, works on an older answer; and the flag's polarity
must be positive. LSP's `isIncomplete` (the historian, § 3) is the opposite polarity, and an absent
`isIncomplete` would read as *complete*, the silent misreading. Wrong for a reader that reads
`"holes"` without the flag: on an older answer json-c still crashes there (`probe2.c`), as under A.
And it is one more member and one more rule, which the spec-warden's parity test (the document
renders the text's answer, nothing more) would have to admit, since the text says nothing about
completeness. Unbuilt in the compiler; the documents are hand-written.

**What the synthesis must answer.** This route against D's number (the ffi-pragmatist's) and against
absence (the compiler-engineer's and spec-warden's), on the three measured readers' outcomes.

### 5. Every reader of the document in the tree is a Heroes program, and none was measured: there, an older answer is read as *finished* in silence or aborts loudly, never a segfault

**The fact.** The ffi-pragmatist measured C readers (json-c 0.19 and cJSON 1.7.12), and its case for
D rests on them. design.md §1.11's row for JSON (`docs/design.md:551`) reads *write it in Heroes (a
JSON value is literally a `variant` with payload — a good fit) or bind cJSON*: Heroes first. Every
reader the shared brief lists is a Heroes program (`selfhost/mutate/json_read.hero` and
`json_fields.hero`, `tests/harness/json_read.hero` and `json_fields.hero` under the grader and the
suites), and in the tree's idiom an absent member is a `T?` the compiler makes the reader decide on:
`selfhost/mutate/judged.hero:107` decides with `.default(no_fixes)`, an absent array read as empty.

**The command** (`critic228/second/heroesreader/`): `tests/harness/json_read.hero` and
`json_fields.hero` copied beside two readers of mine, `lax.hero` (an absent `"holes"` read as no
hole, the `.default` choice) and `strict.hero` (`json_fields.field(of: doc, name: "holes").must()`),
each `heroes-next fmt --in-place` then `heroes-next build`, exit 0; run on today's real answer for
`hole/main.hero` (`heroes-next check --json`, exit 0) and on the compiler-engineer's A' answer for the
same program (`ce228/a1/hole_main.err`, copied):

```
lax    on today's answer   exit 0    finished
lax    on A''s answer      exit 0    holes: 1
strict on today's answer   exit 134  panic: .must() on an error: missing: no field `holes`
strict on A''s answer      exit 0    holes: 1
```

**What it means.** Across the two ways design.md tells a programmer to read JSON, Heroes and cJSON,
an older answer under A or A' is read **as finished, in silence** by the idiom that defaults (the
ffi-pragmatist's `cj_naive`, exit 0; `lax` here), and refused loudly only by a reader that chose
`.must()` (an abort naming the field, 134, the language's own *loud*). json-c's segfault is the one
library of the three where the failure is a crash. So the class the synthesis weighs is mostly
**defect 2158's own silent *finished***, reached again by every reader newer than the `heroes` it
runs, and the json-c crash is its loudest case, not its usual one. Either way it is a version gap
that a number (D) or a positive flag (finding 4) closes and that A and A' leave open; the
compiler-engineer's *D's number protects no consumer the member's absence faults* is true of a
document from a stage that spoke and silent on an older `heroes`, which is the case D is for.

### 6. The blind result reproduces, sits at its ceiling again, and is one session from a veto on C

**Reproduced.** `scores.json` joined to `key.txt` by `python3 -I` (`task` agreeing on all 80):
F 10/10/10/10 on both scores for T, A, B, C; W `finished` T 9, A 10, B 10, C 7, the program 10 in
every cell; 0 voids; 0.244 USD, the dearest session 0.00398; the wrong `finished` are folders 19, 43,
58 (C) and 25 (T). The scorer judged W's programs with the lane's `heroes`, whose `--version` reads
`sources 451802aa66692eb3`, the stage 2 the README names. The second design repaired the first
pass's findings 4 to 10 (the answer is the only channel in F; `lib.hero` unseen in W; C run;
`--no-session-persistence`, and `ls ~/.claude/projects | grep -c build-b` reads 0, run again here).

**What it still cannot tell apart.**
- **The W tasks name their answer.** w1's intent, *a cube whose edges all measure `edge`*, is the
  needed maker's documentation line, *Builds a cube whose edges all measure `side`*; w2's, *as an
  amount in euros*, is *An amount in euros.* (`rechecked-issues-scratch/blind228b/w1/lib.hero:6`,
  `w2/lib.hero:9`). A model matching words finds the maker in any shape that carries the line, so
  the program score is at its ceiling, 40 of 40, in each family.
- **The cap was present and could not fail.** w2's lib has seven makers (`grep -n '^function'`:
  `dollars`, `euros`, `francs`, `pounds`, `rupees`, `yen`, `zero`, then `shown`); the needed one is
  the second, inside the five every shape shows. *The cap read* (README) is true; *the cap tested*
  would need the needed maker among the two cut, which is the case `more_functions` exists for.
- **C against A and B.** The registered rule compares each shape with T alone: C's 7 against T's 9
  is 2, *no difference*. Against A and B, both 10, C stands exactly 3 below, the README's own
  threshold; three of the four misses are C's. The coordinator's reading, *as it stands* read as *as
  I return it*, is recorded beside the count as it should be, and it applies to T's one miss too.
  So the synthesis may write *no veto under the registered rule*, and should not write that the
  blind seat cleared C: under that rule C's clearance rests on folder 25, T's one miss.

**What the synthesis should say of it.** All four shapes carry a hole's facts to the smallest pinned
model when the answer is the only channel; the experiment ranks none of them, and it says nothing of
the cap, the counts, the keys the landing will write (A and B carried `"doc": null` and no
`more_functions`; the compiler-engineer's build writes `""` and `more_functions`; the ffi-pragmatist
asks `""` and `"position"` everywhere) or of any program reading the document.

### 7. Who reads an older answer is a fact of the tree, not a hypothesis, and no seat asked it

**The facts.** The ffi-pragmatist's verdict on A turns on *a measurement that no C reader of `check
--json` exists or will run against a `heroes` older than the landing* (its last paragraph); the
compiler-engineer's objection to D is that its number protects nobody. Neither asked which readers
in the tree run a compiler other than the one they were built with:
- **`heroes mutate`, panel 187's R2, every round**: `selfhost/mutate/judged.hero:1-14`, *a mutant
  ... judged by a compiler run as a child*, *two runs of two compilers*, *the normal arm is `check
  --json`*; R2 compares the round's compiler against the trunk's (`.claude/rules/verification.md`
  § Bounded discovery). On the landing round the trunk's compiler writes no `"holes"`, read by the
  new reader, whose idiom for an absent array is `.default(...)` (`judged.hero:107`, finding 5);
- **the thesis instrument**: every word takes `--compiler <path>` (`tests/harness/thesis_args.hero:21`),
  and the pre-registration pins a compiler by its key (`tests/thesis/held-out/prediction.md:123-125`);
  a grader taught to read `"holes"` (2158's purpose) and run with a pinned older compiler meets the
  older answer.
- **The two readers that do not**: the assistant writing packages *through `heroes check --json`*
  (`docs/roadmap/milestones/M-lsp-server.md:19-21`, the roadmap's one named reader), which reads
  whatever the `heroes` it runs writes, and `heroes lsp`, a verb of the same binary.

And the historian's route (i), unrun by that seat, is measured: `heroes-next check --json
--format-version 3 main.hero` exits **2**, *`check` does not accept `--format-version` — it accepts
--dump-scopes, --json, --brief, --permissive, --apply, --in-place*, 0 bytes on stdout. An older
`heroes` refuses a consumer-chosen format loudly, so that route works on every compiler built today;
it is a new flag, which the stopping rule must admit (the spec-warden's § 7 found no harness that
must type the member, and a flag is a capability).

**What the synthesis must answer.** Which readers it protects, named: the in-tree Heroes readers that
run another compiler (R2 every round, the thesis instrument), the assistant, or an external C
program; the number (D), the flag (finding 4) and `--format-version` protect the first and the
last, and nothing is needed for the assistant and `heroes lsp`.

### 8. Defect 2205 has a second renderer, which its issue does not name

**The fact.** 2205's issue names `selfhost/check/render.hero` `render_ty` under *Where to look*. The
compiler-engineer found (its question 2) that a record's field types and a function's parameters
are printed from the declaring file's syntax by another renderer (`print/types.render_type`), bare
for the same reason. Reproduced with today's compiler on its probe (copied to
`critic228/second/q2205/`): `heroes-next check main.hero` prints `expected type: Box`, `e: Edge`,
`b: Box`, **`fields of Box: edge: Edge, depth: i64`** and **`lib.grow(b: Box)`**, exit 0; pasting
the field's type as the report writes it, `lib.Box(edge: Edge(length: 1), depth: 2)`, is
`error[needs_qualifying]: `Edge` is declared in module `lib` ...` at 4:32, exit 1, and `lib.Edge(...)`
checks at exit 0.

**What follows.** 2205's *Where to look* gains `print/types.render_type` (the issue is open, so
`edit`), and question 5's wait counts six sites in two renderers, as the compiler-engineer priced
them, not one.

### 9. *Hole* names two things in this compiler, and the document would hold both

**The fact.** Spec § 2 calls an interpolation's `{...}` a *hole* (`spec/heroes-spec.md:54`, `:56`),
§ 12 calls `???` one. Three diagnostic codes use the word for the interpolation:
`hole_without_f` (`selfhost/plain_holes.hero:158`, a thesis rule, `selfhost/diag.hero:271`),
`hole_not_renderable` (`selfhost/check/interp.hero:43`) and `expected_hole_close`
(`selfhost/grammar_expr.hero:480`). Route C's code `hole` would stand beside `hole_without_f` in one
`"diagnostics"` list, and a reader matching codes by prefix takes both; route A's member `"holes"`
and the flag of finding 4 would stand, beside an error, next to a diagnostic about the other kind of
hole. No brief or seat raised the name.

**What the synthesis must answer.** The member's name and its one meaning, said in the place a
reader learns the document (design.md §4.17's paragraph, `llms.txt`): `"holes"` as spec § 12's,
never § 2's.

### 10. The spec-warden's unrun claim is measured: the thesis instrument tells a model *refused* and withholds the report

**The command.** An answer made from `tests/thesis/01/reference.hero` with its first `return ""`
turned into `return ???`, fenced as a model answers (`critic228/second/thesis/answer.md`, by
`python3 -I`), graded offline, no API call: `./heroes-next run tests/harness/thesis.hero -- grade
critic228/second/thesis/answer.md --task tests/thesis/01 --compiler ./heroes-next`, exit 0, printing
`hole 0/9` and the message `no binary: 1 hole in build/thesis-78528/answer.hero`. (The reference
unchanged reads `cases_pass 9/9`.) That message is what `thesis_run.hero:675-678` puts in the
template's `refused` block, whose words are *The compiler refused the program:*
(`docs/metrics/first-try.md:32-38`).

**What it means.** The spec-warden's § 3, *no route of this sitting changes a byte a model in the
thesis instrument reads*, holds, now measured on the grader's side (the prompt's assembly read, not
run). Two things follow that no seat drew. (a) The stopping rule's *measured Part 11 effect* cannot
be met by any route **as the instrument stands**: that is a fact about the instrument, not about
holes, and a route that put the report (or this document) in the `refused` block would be the one
way a model in the project's own measurement meets it. (b) The instrument tells a model that a
program with a hole was *refused*, which § 12 says it is not, and hands it none of what § 12 says
the compiler reports. Whether a model in the trial ever wrote a hole is a question: no measurement
of 050 to 052 names the `hole` outcome (`grep -n -i hole`), and their `verdicts.jsonl` are no
longer on disk (`find build .claude/worktrees -maxdepth 4 -name verdicts.jsonl` prints nothing).

**What the synthesis must answer**, or file: whether the instrument's feedback for `hole` carries the
report, which is the only route by which this sitting's work could ever be measured on Part 11.

### 11. Smaller points, each checked

- **The pins of D** (compiler-engineer): `git grep -n -F 'schema\": 2' -- selfhost tests/harness`
  and `git grep -n 'schema 2'` print the four compiler tests, the four net rows, the grader's own
  test input (`thesis_grade.hero:306`, not a pin, as it says), the help (`cli/table.hero:92`) and
  both errors pages; with the constant and design.md's `:2316`, the *eleven pins and two pages* it
  counts. Reproduced.
- **design.md names cJSON second** (ffi-pragmatist § 3b): `docs/design.md:551` reads *write it in
  Heroes ... or bind cJSON*; the seat's citation is right, and finding 5 measures the first half.
- **json-c's README** (ffi-pragmatist § 3): `/opt/homebrew/Cellar/json-c/0.19/README.md:397-398`
  names `json_object_object_get()` for a child, and `grep -c json_object_object_get_ex` prints 0.
  Reproduced.
- **The adopted build's `"type": null`** stands against the ffi-pragmatist's condition 4 (*never
  `null`*) and its § 7 measurement (NULL into `strcmp`); the compiler-engineer chose `null` to answer
  the first pass's finding 3 (`?` reads as the fallible suffix). The two answers to that finding
  disagree, `null` against `""`, and the synthesis picks one.
- **The quiet path's holes are all reported** over sixteen holes in fourteen shapes (finding 2), so
  the compiler-engineer's member written only on the quiet path is complete there; the false lists
  are all beside errors.
- **Defect 2206** (a C function named with a keyword) is a parser message of the `extern` group and
  touches nothing this sitting rules; it does not enter question 5's wait.
- **The lane moved**: `git log -1` in the lane reads `379a9a44a` (two commits past `9a38f1b26`,
  defect 2190 and M-rechecked-issues step 1), and `git diff --stat 9a38f1b26 379a9a44a -- selfhost
  runtime seed` prints nothing (the two commits move ten files, the hooks and the harness's records
  among them), so every line number of `selfhost/` the reports cite holds at the lane's head.

## The questions the synthesis must answer that no seat answered

1. **The shape of every hole and every document, ruled apart from the number** (finding 1). A' as
   built crashes the idiomatic json-c reader on 4 of its own 11 answers at schema 2; the
   ffi-pragmatist's conditions 1, 2 and 4 bind whatever number is adopted. Which: every key on every
   hole (`"position"` always, `"expected"` a string, `"fields"`/`"cases"` always arrays), and a type
   the checker does not know as `""` (ffi-pragmatist) or `null` (compiler-engineer)?
2. **What a document says of holes when a stage spoke** (findings 2 and 4). Absent (compiler-engineer,
   spec-warden), present in every document (ffi-pragmatist, whose own `S-both` is a false `[]`), or
   present with a positive completeness flag (finding 4, measured safe in both libraries' idioms on
   an older answer, at schema 2). `"holes": []` beside an error is false (`arity.hero`, reproduced);
   on the quiet path the list is complete over every shape tried.
3. **Which readers the version rule protects, by name** (findings 5 and 7). R2 (`heroes mutate`)
   reads an older compiler's answer every round, in Heroes, where the idiom reads absence as
   *finished* in silence; the assistant and `heroes lsp` never meet an older answer; an external C
   reader is the case the ffi-pragmatist measured. The number, the flag and `--format-version` (an
   older `heroes` refuses it at exit 2, measured) each protect the first and the last.
4. **The order of landing, with the defects as they now stand** (findings 3 and 8): 2204 told at an
   index, a `while` condition, a `print` argument and an interpolation, beyond its filed three;
   2205 with a second renderer; question 5's wait is for repairs larger than the reports priced. The
   coordinator widens both open issues.
5. **Parity against the extra members.** The spec-warden admits the document as *the completion of
   what `--json` already promises* only under parity (the document renders the text's answer, nothing
   more); the compiler-engineer adds `more_functions`, the ffi-pragmatist `"position"` on every hole,
   finding 4 a flag. Each needs the text to say the same (the spec-warden's own ruling already asks
   the text to count every cut list), or parity is given up and the stopping rule asked again.
6. **The thesis instrument's feedback for `hole`** (finding 10): it tells a model *refused* and
   withholds the report, so no route can be measured on Part 11 as it stands. To rule or to file.
7. **The member's name** (finding 9): one meaning of *hole*, said where a reader learns the document.

**The blind seat** (finding 6) settles that all four shapes carry a hole's facts to Haiku 5.5 when
the answer is the only channel; it ranks none, and its registered *no veto* on C rests on T's one
miss.

## A disclosure

In the first pass, the compiler of this worktree was built by a background command of my session,
whose output the harness wrote under `/private/tmp/claude-501/.../tasks/` (the build's two
`time -p` blocks and two `--version` lines); that is outside the repository's root, and I did not
choose the path. Nothing else of either pass was written outside `.claude/worktrees/228-critic/` and
the two report files.
