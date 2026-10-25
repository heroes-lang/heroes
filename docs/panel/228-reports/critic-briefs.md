# Panel 228, the completeness critic's first pass: the briefs

Read 2026-10-25 over every file of `docs/panel/228-briefs/` (`00-shared.md`, `compiler-engineer.md`,
`spec-warden.md`, `historian.md`, `blind/README.md`, `blind/brief.md`, `blind/variants/t{1,2,3}-{T,A,B}.md`).
Every command below was run in `.claude/worktrees/228-critic`, detached at
`9a38f1b26923bc241d1d5b211c88da0746117375`, unless it names another folder. Written as it goes; the
findings are numbered in the order they were found, and the summary at the end orders them by what they
change.

## What reproduced, and needs no change

Run in this worktree with its own compilers: `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes` (`real 8.21`), then `./heroes build selfhost/main.hero -o heroes-next` (`real 87.09`, `user
64.96`, `sys 14.72`), `./heroes-next --version` printing `sources 451802aa66692eb3`, the briefs' key.
The programs of `rechecked-issues-scratch/hole/` and `blind228/` were copied to `critic228/` here.

- `git log -1` is `9a38f1b26923bc241d1d5b211c88da0746117375`; `git diff --stat 76e98124e HEAD --
  selfhost runtime seed` prints nothing.
- `wc -l`: `check.hero` 449, `check_json.hero` 181, `holes.hero` 306. `check.hero:77-79` is the
  `--json` document, `:86-87` the report under the §4.16 comment (`:84`); `holes.hero:28-29` is
  `SUGGESTIONS` 5; `:100` is `if local.name.start <= at`; `:54-58` is the statement line, taken
  when `table.get(c.types, hole.expected)` is `.error` (`:48-52`); `in_scope` caps with `… and N
  more` (`:113-119`), `nearby` caps (`:185-186`), `fields_of` does not. `check_json.hero:38-39` is
  `SCHEMA` 2 under the comment the brief quotes.
- `check --json main.hero`, `check --brief main.hero`, `build main.hero`, `build --json main.hero`
  and `check --json both.hero` print, byte for byte in substance, what the shared brief says
  (exit 0 / 0 / 1 with `no binary: 1 hole in main.hero` / 2 with *`build` does not accept
  `--json`* / 1 with one `type_mismatch` and an empty stdout).
- design.md §4.16 (`docs/design.md:2215`) holds both quoted sentences; spec § 12
  (`spec/heroes-spec.md:348-350`) holds *a program with holes type-checks everything else but
  produces no binary* and *the compiler reports what belongs there (the expected type, what is in
  scope)*.
- `.claude/rules/cli-surface.md:18-20` and `:40-43` hold the stopping rule and the contract as
  quoted.
- `./heroes-next measure spec/heroes-spec.md`: `claude-legacy 7717`, `cl100k_base 7854`, `real
  10297 claude-opus-5, 2026-10-24`, headroom 6087 against 16384, the FFI floor 60: the
  spec-warden's brief exactly.
- `site/src/llms.txt:61-66`, `site/src/lib/claims.ts:86-90` (`jsonSchema`) and `:536-542` (the
  errors pages' *schema N, written to stderr*) are what the brief says; `ROADMAP.md:204` is row 84,
  M-lsp-server, and `docs/roadmap/milestones/M-lsp-server.md:20` says *the people who arrive with*
  `heroes check --json`.
- `grep -rl 'hole at' tests/golden` prints six files (but see finding 6).
- Issues 2203 and 2204 are open, `class:blocking`, filed 2026-10-25, with the shapes the brief
  names (`gh issue view`).
- Blind: `shasum -a 256` of each `blind228/src/t{1,2,3}.hero` equals the `sha256` in its variants;
  `src/t{1,2,3}.out` less the lines defect 2203 adds (t1 `q: Point` in the first hole and `d: i64`
  in the second, t2 `book: Item`) is the T variant's stdout; the references `ref/t{1,2,3}.hero`
  check at exit 0 and, built and run, print `25`, `40`, `ada!` `bob!`; the real stderr document is
  multi-line as the variants show it; `/build` is `.gitignore:7` and `git check-ignore -v
  build/w228/1/task.md` names it.

## Findings

### 1. A third false name in the report, with its own cause: the names of a block that has closed

**The fact.** `in_scope` lists every local of the declaration the hole is in whose name starts
before the hole (`holes.hero:85-88`, whose comment rests on the premise *a local cannot be read
outside the declaration that binds it*, and `:97-100`). A local bound inside an `if` body or a
`for` body, and a `for`'s own variable, are therefore listed at a hole written after that block has
closed, and the resolver refuses each at that column. Defect 2203 lists none of these shapes (its
issue lists `d: i64 = ???`, `@=`, `a + ???`, `P(x: ???)`, `for v in ???`, `m = match`, `d = if`,
each the hole's own binding) and its cause, as the shared brief gives it, is `:100`'s `<=`, which
these shapes do not need.

**The commands** (`critic228/scope/`, `../../heroes-next check <file>`):

```
ifblock.hero   a = 1 / if a > 0 / inner = 2 / print(inner) / b: i64 = ???   ->  in scope: a: i64, inner: i64, b: i64   exit 0
forvar.hero    for k in [1, 2] / print(k) / b: i64 = ???                    ->  in scope: k: i64, b: i64               exit 0
forbody.hero   for k in [1, 2] / t = k * 2 / print(t) / b: i64 = ???        ->  in scope: k: i64, t: i64, b: i64       exit 0
ifblock_used.hero  ... b: i64 = inner   ->  error[unknown_name]: nothing named `inner` is in scope     exit 1
forvar_used.hero   ... b: i64 = k       ->  error[unknown_name]: nothing named `k` is in scope — did you mean `ok`?   exit 1
```

**What the briefs should say.** The shared brief's *Two defects* bullet and question 5 name a third
cause beside 2203 and 2204: the report models a function's scope, not a block's. It changes the
compiler-engineer's question 2 (whether the checker knows, at a hole, which block-locals are still
live, so the one value both renderings read is right) and question 5's premise (*two* defects).
The blind variants are not touched: no hole in t1, t2 or t3 stands after a closed block (t3's hole
is inside its `for`). The coordinator files it (`blocking` by 2203's own reasoning, a name the
resolver refuses at that column); this pass files nothing.

### 2. A fourth shape of defect 2204: an unannotated binding's hole whose value is used

**The fact.** `x = ???` is told `position: statement — any type, or none` though its value is
bound and used; a value of the wrong type written there is refused. Defect 2204 lists a `for`'s
array, a `match` arm and an `if` branch, not a binding with no annotation.

**The commands** (`critic228/scope/`):

```
untyped_used.hero    x = ??? / y: i64 = x / print(y)   ->  hole at 2:9, position: statement — any type, or none; in scope: x: ?   exit 0
untyped_filled.hero  x = "s" / y: i64 = x / print(y)   ->  error[type_mismatch]: expected `i64`, found `str` at 3:14            exit 1
```

**What the briefs should say.** The 2204 bullet names this shape too, or the coordinator adds it to
2204 as its depth-one row (it has 2204's cause, `:48-58` taking `.error` for *nothing expected*).

### 3. A legitimate name whose type renders `?`, which question 3 does not weigh

**The fact.** A name bound to an unannotated hole is really in scope at a later hole, and the report
renders its type `?` (`untyped.hero`: `x = ??? / y: i64 = ??? / ...` prints, at the second hole,
`in scope: x: ?` and `y: i64`; `y` is defect 2203's, `x` is not). In Heroes `?` is also the
fallible suffix (`T?`, spec § 6), so a document carrying `"type": "?"` hands a machine a string that
reads as a malformed fallible type.

**What the brief should say.** Question 3 (*a string as Heroes writes the type, or a structure*)
names this case: what a type the checker does not know is, in each rendering.

### 4. The blind experiment cannot tell the shapes apart: the program it shows answers every score

**The fact.** Each `task.md` shows the program whole, `???` in plain sight, and the folder's spec
says at § 12 that `???` is a hole and that a program with holes produces no binary. Everything the
compiler's answer adds is also in the program shown: t1's `Point` fields and `dist2`, t2's `Item`
fields, t3's `shout`. So:

- `finished: no` is read from the program, whatever shape the answer has;
- every hole can be filled without reading the answer (`d: i64 = 25` passes t1's three scores);
- route B's one stated weakness, *a reader that reads stderr alone* (shared brief, route B), cannot
  show, since every variant hands the model both streams;
- the veto rule (README, last paragraph) fires only if a shape makes a model take the program for
  finished, or write one the compiler refuses, *more often than today's*: at three a cell and a
  score at its ceiling for every shape, that is one stray answer. **And no rule says what a benefit
  would look like**, so a null result is written *no veto*, which a synthesis can read as support for
  route A. That leans toward the proposal.

What the variants never exercise, by `grep` over `variants/*.md`: no `"mutable": true`, no
`"cases"`, `"more_in_scope": 0` on all eight scope lists, no function with a doc line, `position`
only in t3's three. So questions 2 (the cap) and 3 (the types) get no reading either.

**Commands**: reading `variants/t{1,2,3}-T.md` against `spec/heroes-spec.md:348-350`; `grep -l
'"mutable": true' variants/*.md` and `grep -l '"cases"' variants/*.md` print nothing; `grep -h
'"more_in_scope"' variants/*.md | sort | uniq -c` prints `8 "more_in_scope": 0,`.

**What the README should say**, or what the experiment should become. Tasks where the answer is the
only channel, which is defect 2158's situation: (a) the compiler's answer for a file the task does
not show, or shows only in part, asking whether it is finished and what goes at each place; (b) a
two-module program whose record, cases and functions that return the hole's type live in the module
the task does not show; (c) a hole with more than five names in scope and a type with more than five
makers, so the cap's rendering is read; (d) a variant type and a cell. And a rule written before
the run for what counts as a shape helping, not only hurting.

### 5. The JSON shapes are the coordinator's keys, and one clause of the text is in T alone

**The facts.**
- `"mutable"`, `"more_in_scope"`, `"position": "statement"` and `functions` as strings are named by
  `gen/variants.py:28-43`, not by the shared brief, which says *whether it is a cell* and *how many
  more the cap left out* and names no key. The compiler-engineer is asked to build the route it would
  adopt (its question 5); if its document's keys differ, the blind seat read a document no compiler
  emits.
- T's statement line is `position: statement — any type, or none` (`variants.py:17`, the compiler's
  own, `holes.hero:58`); A and B carry `"position": "statement"` (`variants.py:29`). *Any type, or
  none* is in T alone. (It is also defect 2204's false clause, so its absence may help A and B on a
  2204 shape; t3's hole is a true statement, so not here.)
- `variants.py:7-12` is a hand-written table, not a parse of `src/*.out`. Every row agrees with
  `src/*.out` less the 2203 lines (checked above), so the README's *by construction* holds as a
  transcription; it should say *transcribed*.

**What the README should say.** Either run the sessions on the document the compiler-engineer's
build emits, or record the keys as the proposal's and say the compiler-engineer builds to them; and
render T's clause in A and B (a `"note"`), or drop the claim *the information is the same*.

### 6. Route C is left out of the experiment, and it is the shape most likely to move a model

**The fact.** The README measures T, A and B. D differs from A by one digit, which a model reading
the answer would not act on. C puts each hole in `"diagnostics"`, where a reader looks for what is
wrong, and either keeps exit 0 with a non-empty list or moves to exit 1: of the four routes it is the
one whose shape most plausibly changes what a model does (an inference; unrun).

**What the README should say.** A fourth shape, C, or one sentence saying why the seat need not
read it.

### 7. The sample is a third of what the budget buys, and the per-session bound does not bound the sitting

**The facts.** Panel 227's six counting sessions on the same model, each with a whole copy of the
specification, cost 0.0566 USD together (`docs/panel/227-reports/llm-ergonomist.md:18-19`), about
0.0094 USD a session. At that price 27 sessions cost about 0.25 USD, so the author's 3 USD buys
roughly ten sessions a cell rather than three: an inference, since this sitting's tasks are shorter
than panel 227's two. And `--max-budget-usd 1` per session lets 27 sessions spend up to 27 USD against
the sitting's 3 USD.

**What the README should say.** A per-session bound of 3 / 27 = 0.11 USD (or 3 / N for N sessions),
so the sitting's ceiling holds by construction; the number of sessions a cell chosen from the price
panel 227 measured; and what is done if the budget ends mid-run (the cells left unequal, or the run
abandoned).

### 8. The command writes a transcript per session outside the repository's root

**The fact.** Panel 227's counting run, under `--bare`, left one transcript per folder under
`~/.claude/projects/`: `ls -la -T ~/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang-build-w-2/`
prints `cdee9d41-4071-47e1-bf4f-71bd0568d651.jsonl`, 157,371 bytes, `Oct 21 12:45:52 2026`, and
`build-w-1` to `-6` each hold one or more. CLAUDE.md § Hard stops: *no file is written outside this
repository's root*. `claude --help` (2.1.308) lists `--no-session-persistence`: *sessions will not
be saved to disk and cannot be resumed (only works with --print)*. The README's command omits it.

Two things beside it. The `context` answer is the model's own report, and `--output-format json`
keeps the final result only, so nothing the coordinator keeps shows which paths a session read;
`key.txt` is to stand beside the README in the lane, a path a session's unconfined `Read` could
open. And the system prompt hands each session its absolute path,
`.../.claude/worktrees/rechecked-issues/build/w228/<N>`, which names the milestone and the sitting
(panel 227's was `build/w/<N>`); no shape is named by it.

**What the README should say.** Add `--no-session-persistence`; keep each session's tool calls
inside its folder (`--output-format stream-json`, *realtime streaming* by the help text, where
`json` is a *single result*; whether it carries every tool call, and needs `--verbose` under `-p`,
is unrun here) and check that every
`Read` named a path inside the folder before an answer is scored.

### 9. The scoring is done by the coordinator who holds the key, and its extraction rule is unwritten

**The fact.** The README says the coordinator writes the verdict from the 27 answers and does not
say the answers are scored before `key.txt` is read, nor which fenced block is *the program*
(`brief.md` asks for *one* fenced block; `tests/harness/thesis_grade.hero:29-30` pre-registers *the
LAST fenced block that holds `function main`*).

**What the README should say.** The extraction rule, pre-registered, and the scoring (check strict,
build, run, compare) done by a command over the 27 folders before the key is opened.

### 10. *The three the trial pins* are four

**The fact.** `tests/thesis/held-out/prediction.md:104` and `:123-124` pin `claude-haiku-5-5`,
`claude-sonnet-5-5`, `claude-opus-5-5` and `claude-fable-5-1`. Haiku 5.5 is the smallest of them;
the README's *the smallest of the three the trial pins* should say four. Panel 227 chose Haiku on a
measured reason (9 of 30 turn-1 answers, its README); this README's reason, *the likeliest to miss
a hole a shape hides*, is an inference and should say so.

### 11. The readers list mixes writers and mentions, and its vocabulary misses real readers

**The facts.** The brief's grep, `git grep -l -e 'check --json' -e '"schema": 2' -e check_json`
with records and sittings left out, prints 24 paths. Of those the brief lists:
- **the two golden cases are not readers**: their one match is a comment
  (`tests/golden/check/fixedbugs-use-refusal-eats-the-next-line.hero:13`,
  `tests/golden/full/fixedbugs-271-a-fix-away-from-its-message-s-line-says-where.hero:7`), and `grep
  -c -F '???'` prints 0 for both. The shared brief does not name them, and the compiler-engineer's
  question 4 says *the two golden cases the shared brief names*;
- `selfhost/cli/certain.hero` and `selfhost/cli/apply_answer.hero` write the `--apply` document
  (`apply_answer.hero:50` builds it with `check_json.head_of`); they read none.

An argument list spells `"--json"` apart from `check`, which the grep cannot see. `git grep -l -e
'"--json"' -- selfhost tests/harness site tools editors` prints 15 files; those that run `check`
with it and read the answer:
- `tests/harness/apply_tree.hero:91-92`: `check <holes-json>/holes.hero --apply --in-place --json`
  pins the applied document (`"rounds": 0`, `"not_applied": []`) on a file holding a hole, and
  `--apply --in-place` without `--json` pins *a clean file with a hole prints nothing at all*.
  These are question 4's own witnesses;
- `tests/harness/fix_names.hero:97-99`: `check <case> --json` over every `tests/golden/check/`
  case, reading each `"title"` from stderr; six of those cases hold `???` (finding 12);
- `tests/harness/suite_surface.hero:315`: `check <path> --apply --in-place --json`;
- `tests/harness/thesis_sha256.hero:14`, which reads the `read` digests.

**What the brief should say.** Three lists (writers; readers, each with the stream it reads;
mentions), the two golden cases named or dropped, and `apply_tree.hero:91-92` under question 4.

### 12. *Six files* hold `hole at`, but two are cases; twelve golden programs hold a hole

**The fact.** Of the six files `grep -rl 'hole at' tests/golden` prints, four are the recovery
corpus (`tests/golden/recovery/plan-singles.jsonl` and three `corpus/tests/harness/*.frozen`); the
live cases are `tests/golden/ir/fixedbugs-231-a-hole-leaves-its-function-unmerged.expected` and
`tests/golden/check/discarded-failure.hero`. The programs a route can move are those holding a
hole: `grep -rl -F '???' tests/golden --include='*.hero' | grep -v recovery` prints twelve, by
directory `check` 6, `ir` 1, `run` 1, `surface-fixtures` 3, `unsupported` 1.

**What the compiler-engineer's question 4 should say.** Both counts, so what a route moves is read
from the programs and not from one rendering of the report.

### 13. Route C's cost to the Part 11 instrument is not named

**The fact.** `tests/harness/thesis_grade.hero:151-160` runs `check --json` first and grades any exit
other than 0 and 2 as `compile_error`, with the codes of `"diagnostics"`, before it builds; a hole
reaches the `hole` outcome only through `build`'s `no binary:` (`:169-171`), and `hole` stands
outside the denominator (`:15-16`). Under route C at exit 1, a hole is graded `compile_error` with
the code `hole`, inside the measure the thesis is judged on, unless the grader changes in the same
landing.

**What the shared brief should say**, under C: that cost, beside the exit contract.

### 14. Route B lands on a stdout that already carries an artifact in two flag combinations

**The facts** (`critic228/hole/`):
- `check --apply --json main.hero`: exit 0, stdout the `"kind": "applied"` document, **stderr
  empty**: today, under `--apply --json`, no stream says the program holds a hole;
- `check --json --dump-scopes main.hero`: exit 0, stdout the scope dump, then `holes in `main`:
  unused bindings are not reported there`, then the hole's text; stderr the diagnostics document.
  `selfhost/cli/check_flags.hero:53` and `:55` assert both combinations are accepted.

Route B writes a second JSON document into a stdout that already holds one (`--apply --json`) or
holds text (`--dump-scopes`), and no RFC 8259 reader takes either whole.

**What the shared brief should say.** Under B, both combinations; under question 4, today's fact
that `--apply --json` writes nothing to stderr, so the hole is told nowhere.

### 15. The issues the sitting is convened on are quoted nowhere, and one of them leans on the stopping rule

**The facts** (`gh issue view 2091`, `gh issue view 2158`):
- defect 2158 is `class:adjacent`, and its Class paragraph says *the instrument that found it has
  another route* (`build`'s `no binary:`). That is the stopping rule's input, *the Part 11 harness
  must type it*: by the defect's own words it does not have to;
- feature 2091's Origin says *`--json` says how to print, never what, so this needs no new verb*:
  the issue already answers the brief's open question (*a capability or the completion of one*)
  one way, and the brief should quote it so the spec-warden judges that reading, not a blank;
- 2158's Class paragraph cites *spec § 9* for *a `???` is a program that type-checks and makes no
  binary*; the sentence is § 12 (`spec/heroes-spec.md:339`, `:348-350`; § 9 is *Functions and
  calls*, `:257`). The issue is open, so `edit` corrects it.

**What the shared brief should say.** Each issue's kind and class, the two sentences above, and the
correction owed to 2158.

### 16. Panel 193 is quoted short of its own reason for schema 2, which is this project's precedent for route D

**The facts.** `docs/panel/193-...md:95` reads *the historian: additive fields keep 1, as `cargo
metadata` does* (the brief: *as cargo*). The sitting's reason for 2 is at `:69-74`: two documents
would have said `"schema": 1`, and *a 19-line json-c consumer that checks `"schema": 1` crashes at
exit 139 on the first where today it refuses loudly*; R3's conservative alternative, `:135-137`, was
*schema 1 and the keys added under it, as `cargo metadata` adds them*. The shared brief says only
*the sitting chose 2*.

The failure panel 193 measured runs in one direction: a consumer written for a new member,
meeting an answer without it under the same number, read the absence as a value.
`docs/panel/193-reports/ffi-pragmatist.md:235-242`: on json-c 0.19, the idiomatic
`json_object_get_int64(json_object_object_get(..., "offset"))` gave *`start 0 end 0` for every fix*
on an answer that had no `offset`, the header saying *null is equivalent to 0*. The same direction
here: a consumer written for `"holes"`, meeting an older `heroes` whose schema-2 document has none,
does not find it, and only a number it checks (route D) turns that into a refusal.

**Measured here, so it is no longer a question**: `critic228/jsonc/probe2.c`, json-c 0.19 (`brew
list --versions json-c`), `clang -std=c11 -Wall -Wextra -Wpedantic -I/opt/homebrew/include probe2.c
-L/opt/homebrew/lib -ljson-c` (exit 0, one warning, an unused `argv`). On today's answer
`{"schema": 2, "kind": "diagnostics", "read": [], "diagnostics": []}` it prints `get_ex says
ABSENT`, `idiomatic get -> NULL`, then `json_object_array_length(json_object_object_get(doc,
"holes"))` **faults, exit 139**; on route A's answer with one hole it prints `array_length -> 1`,
exit 0. So under route A, a consumer written for `"holes"` with the idiomatic read crashes on every
`heroes` older than the landing, at the exit panel 193 cited for choosing 2; under D it can refuse
by the number. (A consumer that asks `json_object_object_get_ex` first does not crash; panel 193
counted the idiomatic read, and so does this probe.) No seat is asked this: it is the
ffi-pragmatist's kind of question, the seat the shared brief leaves out because *a C consumer of the
document was panel 193's question, and its answer, schema 2, stands*; that answer was about a
different change.

**What the brief should say.** Quote `:95` whole and R3's reason and conservative alternative,
state this probe under route D, and either seat the ffi-pragmatist for D or give the
compiler-engineer the question *what a consumer of schema 2 written for `"holes"` does on an older
answer*.

### 17. The historian's brief states, as a fact, the precedent that favours A over D

**The fact.** *rustc's `--error-format=json` and cargo's message format, which add fields without
bumping a version* is written as a fact inside a list the same sentence calls *none of them a
premise*. Unverified here (the critic reads no web page in this pass); panel 193's historian named
`cargo metadata`, not cargo's message format.

**What the brief should say.** The same precedents as questions: *whether rustc's and cargo's JSON
add fields without a version, and whether `cargo metadata` carries a version number*.

### 18. The proposal's members leave out a line the text report carries

**The facts.** `holes.hero:217-218` writes, under each function that returns the type, its first
documentation line, and the module's own test asserts it (`:279`, `origin()\n      # Builds the
origin.`). The shared brief's route A says *carrying what the text report carries today and nothing
else* and lists `functions` with no documentation. And the text's cap differs by list: `in_scope`
says `… and N more` (`:118-119`), `nearby` stops at five and says nothing (`:185-186`); question 2's
*kept at 5 per list with the count left out* reads as if the count were left out of both.

**What the brief should say.** `functions` with each one's documentation line (or a ruling that the
document drops it), and question 2 with the asymmetry stated.

### 19. Question 6's open half is settled by the spec's history

**The facts.** The specification has stood at `spec/heroes-spec.md` since its first commit:
`git log --follow --diff-filter=AR --name-status -- spec/heroes-spec.md` prints one `A`, `d10fbec74`
(2026-08-03), never a rename. Its v0 holes paragraph is today's word for word (`git show
d10fbec74:spec/heroes-spec.md`, lines 155-157). `git log -S` over the spec finds no commit for
`suggestion`, `deterministic order`, `nearby`, `capped`, `at most 5`, `at most five` or
`SUGGESTIONS`; `-G'(\?\?\?|[Hh]ole).*([0-9]|five)'` finds one commit, `0f331b39e`, whose changed
lines hold no hole.

**What the brief should say.** *No revision of the specification has stated the cap, by the
searches above* (naming them, since the claim rests on their vocabulary), so design.md §4.16's *and
the cap is stated in the spec* has been false since the spec's first commit, and question 6 is a
repair to rule, not a history to find.

### 20. The spec-warden's grep prints a line

**The fact.** `grep -n json spec/heroes-spec.md` prints `448:it has ended as if given back.
`retains json_object_put` after a result`; case-insensitive it also finds `:353` (*JSON,
databases*). The claim *`--json` appears nowhere* holds: `grep -n -- '--json' spec/heroes-spec.md`
prints nothing, exit 1.

**What the brief should say.** The second command.

### 21. Routes nobody listed

- **E, a count and not the report**: the stderr document gains `"holes": <n>` (or the list of
  places alone, `file`, `line`, `col`), the text report staying on stdout. It answers defect 2158 as
  filed (*a reader of the JSON cannot see the hole*) with one member, no second rendering of the
  report, no cap or type question, and none of 2203's, 2204's or finding 1's false content in a
  document a machine trusts. Right if the document's readers need to know *that* a file is
  unfinished (the grader buckets, an agent re-reads the text); wrong if they need *what goes
  there*, which feature 2091 asks for (*an agent reading `--json` gets none of it*).
- **A', the document and the text both**: route A's member, the text report kept on stdout under
  `--json`. The shared brief's A drops the text (*the document being the answer*) and no route
  keeps both, though dropping it changes stdout for every reader of `check --json` on a holed file
  today (`--json --dump-scopes` included, finding 14).
- **The order of landing**: the routes all carry today's report; none is *the document after the
  repairs of 2203, 2204 and finding 1*, which question 5 asks as a wait and not as a route.

### 22. Small: the lane's state as the brief describes it

`stat -f '%SB'` gives `docs/panel/228-briefs` born at 02:37:40, so at 02:41 `git status
--porcelain` printed at least `?? docs/panel/228-briefs/` (an inference from the birth time); now
the lane also prints ` M .claude/hooks/guard_bash.py`, ` M tests/harness/suite_records.hero` and
`?? .claude/hooks/tagged.py`. *Files no seat reads (the hooks, the skills, the issues' comments)*
omits `tests/harness/suite_records.hero`; harmless, since each seat reads its own worktree, and the
sentence should say that instead of listing.

## Summary, ordered by what each finding changes

**22 findings.** Every one carries its command above; the programs and the probe are in
`.claude/worktrees/228-critic/critic228/` (`scope/`, `hole/`, `blind/`, `jsonc/`).

**The blind seat's experiment, before any session is paid for**:
- 4: the tasks show the program whole, so every score is read off the program and none off the
  shape; route B's weakness cannot show; a null result reads as *no veto*. The tasks need the
  compiler's answer to be the only channel.
- 8: the command writes a transcript per session outside the repository's root (panel 227's did);
  `--no-session-persistence` exists. A hard stop, so it binds before the run.
- 7: `--max-budget-usd 1` × 27 does not bound the sitting's 3 USD; the measured price buys about
  three times the samples.
- 5, 6, 9, 10: the JSON keys are the coordinator's, T alone carries *any type, or none*, route C is
  not read, the scoring and extraction are not pre-registered, *three* models are four.

**The compiler-engineer**:
- 1 and 2: a third false-name cause (a closed block's names) and a fourth shape of 2204 (`x =
  ???`), both with a name or a type the checker refuses at that column; question 2 and question 5
  change.
- 16: measured, json-c 0.19's idiomatic read of an absent `"holes"` faults (exit 139), so route A
  crashes such a consumer on every older `heroes`; that is route D's case and no seat is asked it.
- 11, 12, 13, 14: `apply_tree.hero:91-92` is question 4's witness and no brief names it; the *two
  golden cases* read nothing and are named nowhere; route C moves the thesis grader's `hole`
  outcome into `compile_error`; route B writes into a stdout that already holds an artifact.
- 3, 18, 21: the type `?`; the documentation line the members leave out; routes E (a count) and A'
  (both renderings).

**The spec-warden**: 15 (2158 is `adjacent` and says the instrument has another route; 2091 says
*no new verb*: both are the stopping rule's inputs), 19 (no revision of the spec ever stated the
cap, so question 6 is a repair), 20 (the grep to cite).

**The historian**: 17 (a premise written as a fact, leaning A over D) and 16 (*`cargo metadata`*,
not *cargo*).

**The coordinator**, outside the sitting: file finding 1 and add finding 2 to 2204, correct 2158's
*§ 9* to § 12, and 22 (the lane's state).
