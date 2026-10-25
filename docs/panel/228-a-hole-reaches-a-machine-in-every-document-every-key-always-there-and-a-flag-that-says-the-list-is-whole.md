# Panel 228: a hole reaches a machine in every document, every key always there, and a flag that says the list is whole

**Sat 2026-10-25** in M-rechecked-issues, on feature 2091 and defect 2158, by the author's yes of that
day, asked once for the milestone (CLAUDE.md § 4). **Full panel**: the compiler-engineer, the
spec-warden, the historian, the ffi-pragmatist (seated after the critic's first pass) and the
llm-ergonomist as a blind experiment, with a completeness critic before the seats and after them. The
briefs are `docs/panel/228-briefs/`, the reports `docs/panel/228-reports/`. **Resolution provisional —
author ratification pending**, its decision issue named below.

## The proposal, verbatim

`heroes check --json` on a program whose stages are quiet and which holds `???` writes, in the
document it writes to stderr today (`"schema": 2`, `"kind": "diagnostics"`), a member `"holes"` after
`"diagnostics"`: one object per hole, in source order, carrying what the text report carries and
nothing else, each list at the text's cap and in its order. [...] Exit 0 is kept; `"schema"` stays 2;
under `--json` the text report is not printed to stdout, the document being the answer. A program with
no hole writes `"holes": []`. (`00-shared.md` § The proposal, with its keys.)

## The verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object A; approve A'** in a landing order; object B, C, D; E as a fallback only. No veto | §1.7, Part 5 untouched | +190 code_lines over five files for one value two renderers read; `check selfhost/main.hero` 6.81 s before and after, the document 0.01 s on 800 holes | at M-rechecked-issues' tag, (a) the commits closing 2203, 2204 and 2205 touch neither renderer; (b) the one closing 2203 adds a module under `selfhost/resolve/` or raises a ceiling | the member from the one value the text reads; after 2203, 2204, 2205; `more_functions`; the text kept unless a reader is shown it breaks |
| spec-warden | **object A; approve A'**; object B; **veto C** (Principle 0); approve D; E as a first landing only | §1.6, §1.2, Principle 0 | **0 spec tokens on every route** (`grep -n -- '--json'` empty); the cap stated in the spec priced at **+10 real** (`measure --refresh`, reverted) | at the landing, the spec's diff empty and the ledger row 0; the probe hole reads `… and 9 more` under its functions, `… and 3 more` in scope after 2203 | A' with stdout untouched, after the three repairs, every capped list counted, holes beside errors as § 12 reads, §4.16 corrected and not the spec |
| historian (advisory) | **approve** a `"holes"` member beside `"diagnostics"` (A, A', D in shape); **object C** | prior art | none | none | none; three routes nobody listed (`--format-version`, the text inside the document, a minor number) |
| ffi-pragmatist | **object A, A', B, C, E; approve D only** under six conditions. No veto | §1.11, §4.17 | 20 C11 consumers, json-c 0.19 and cJSON 1.7.12, 0 warnings | on D landed, both idiomatic consumers end with no signal on the twelve holed golden programs and refuse every answer of `9a38f1b26` at exit 2 | `"holes"` in every document at every exit; every key on every hole, no string absent or null; a count beside every capped list; every type a Type the file writes, or `""`; after 2203, 2204, 2205; §4.17 says when the number moves |
| llm-ergonomist (blind) | **no shape helps or hurts that the sample can show; no veto** | §1.1 | 80 sessions on Haiku 5.5, **0.244 USD** | the same 80 tasks re-run on the landed document, every cell 8 of 10 or more on the second score | none |

**The blind experiment** (`228-reports/llm-ergonomist.md`): with the answer the only channel a hole's
facts reach the model by, the four shapes (today's text, A, B and C) each led Haiku 5.5 to every hole's
place and type, 40 of 40, and to a program that checks and prints the right value, 40 of 40. `finished`
read 10, 10, 10 and 10 in family F and 9 (T), 10 (A), 10 (B) and 7 (C) in W; every one of the four
wrong answers names the hole and calls the program it returns finished. The critic's second pass
(finding 6) adds that the W tasks' intents paraphrase the right function's documentation line and that
C stands exactly 3 of 10 below A and B, the threshold, though not below T: the experiment ranks
nothing, and shows no shape hiding a hole from a model.

## Where the seats disagree, and which side the world takes

- **A against A'.** The proposal emptied stdout under `--json`. The compiler-engineer measured that
  this buys 0 lines and 0 readers in the net and leaves `--json --dump-scopes` announcing a hole it no
  longer shows; the spec-warden that it changes *what* is printed, not *how*. Nobody showed a reader the
  text breaks. **A'**, the text kept.
- **`"holes": []` beside an error.** The spec-warden asked for holes beside errors *as § 12 reads*.
  The compiler-engineer built it and measured it false: on a call of the wrong arity the checker never
  reaches the hole, so the document said `"holes": []` of a program holding one (`arity.hero`); the
  critic reproduced it with that seat's binary (second pass, finding 2). **The checkable side is the
  compiler-engineer's**: beside an error the list cannot be promised whole.
- **The number: 2 or 3.** The ffi-pragmatist measured route A crash json-c's idiomatic reader (exit
  139) and read a holed file as finished in cJSON, in silence, on every answer older than the landing,
  and approved D because its number lets a reader refuse such an answer. The compiler-engineer measured
  D's number protect no reader that the member's absence faults, at eleven pins and two published
  pages. **The critic's second pass ended the disagreement by measuring a route nobody listed** (finding
  4): a flag that is true only when the list is whole, which both libraries read as false where it is
  absent, so an older answer is refused at exit 2 with no crash, no silent *finished*, no schema bump and
  no false list beside an error. And it found the crash the ffi-pragmatist measured on older answers on
  **A' itself**, on today's compiler (finding 1): a statement hole carries no `"expected"`, and the
  idiomatic `strcmp` on its NULL faults on 4 of 11 answers, `s7`'s true statement hole among them. So the
  number was never the protection; **every key always there** is.

## The resolution, provisional — author ratification pending

**R1. Every document carries the holes and says whether the list is whole.** Every document `heroes
check --json` writes, the `"kind": "diagnostics"` document on stderr and the `"kind": "applied"`
document `--apply --json` writes on stdout, carries `"holes"`, an array, and `"holes_complete"`, a
boolean. `"holes_complete"` is `true` exactly when every stage of the check ran quiet, and then
`"holes"` is every hole of the program in source order; otherwise it is `false` and `"holes"` is `[]`,
since beside an error the type stage may not reach a hole. `"schema"` stays 2. A reader acts on
`"holes"` only where `"holes_complete"` is `true`, which a document from before the landing never says.

**R2. Every hole carries every key, and no key is ever null.** `"file"`, `"line"`, `"col"`;
`"position"`, `"value"` or `"statement"`; `"expected"` and `"returns"`, strings, `""` where there is
none; `"in_scope"`, objects of `"name"`, `"type"` and `"mutable"`; `"more_in_scope"`, a count;
`"fields"`, objects of `"name"` and `"type"`; `"cases"`, strings; `"functions"`, objects of `"call"`
and `"doc"`, `""` where a function has no documentation line; `"more_functions"`, a count. **Every
type string is a type the hole's file can write**, qualified as it qualifies it (defect 2205's repair),
or `""` where the checker does not know it, never `?`.

**R3. The text stays on stdout.** Under `--json` the hole's report is still printed to stdout as it is
today. The text and the document are two renderings of one value the report builds once, so a repair
lands in the value and both renderings follow.

**R4. Every list the cap cuts says how many it cut**, in both renderings: the text's functions gain
`… and N more` as its names in scope have, and the document carries `"more_in_scope"` and
`"more_functions"`. The cap stays five (design.md §4.16).

**R5. The order of landing.** The value first, then the repairs of defects 2203, 2204 and 2205 in it,
then the member. Defect 2203 was repaired during the sitting (1d66ece88); 2204 and 2205 were widened
that day by the critic and the compiler-engineer, and the member does not land while either is open.

**R6. design.md §4.16's sentence about the cap is corrected, and the specification is untouched.**
No revision of the spec stated the cap, by the critic's searches (first pass, finding 19); the cap is
§4.16's own, and the correction says so and that every list it cuts is counted. 0 spec tokens.

**R7. design.md §4.17's paragraph about the document says when `"schema"` moves**: when a member's
meaning changes or a member leaves; a member added is added with what its absence means, and
`"holes"` with `"holes_complete"`, whose absence a reader takes as `false`.

**R8. Route C is refused**, by the spec-warden's veto: a hole as a diagnostic contradicts spec § 12's
*It is not an error* at exit 1 and the exit contract at exit 0, and moves the thesis instrument's
`hole` outcome into `compile_error`, inside the measure the thesis is judged on. The veto lifts only
with a measured Part 11 effect that pays for amending § 12.

**R9. Not adopted, and why.** B: a second document on a stdout that `--apply --json` and `--json
--dump-scopes` already write to, and a reader of stderr alone still blind (2158). E: a count answers
2158 and not 2091, which asks what goes at the hole. **D, the conservative alternative, recorded for
the author to choose**: R1 to R7 under `"schema": 3`, the ffi-pragmatist's route, at eleven pins and two
published pages; it protects no reader R1's flag does not, measured.

## What the sitting found beside its question

- **Three defects in what the report says**, all `blocking`: 2203 (repaired at 1d66ece88), 2204
  (widened to seven of fourteen quiet programs: an index, a `while` condition, a `print` argument, an
  `f"..."` hole, a `for`'s array, a branch's value, an unannotated binding's later use), 2205 (widened to
  a second renderer, a record's fields and a function's parameters).
- **The thesis instrument shows a model no report** (critic, second pass, finding 10): on a hole it
  feeds the model `build`'s `no binary: 1 hole in …` alone, so no route can show a Part 11 effect as the
  instrument stands; a question for whoever next runs metric 2, not this sitting's.
- **`hole` names two things in this compiler**: a `???` and an `f"..."` literal's braces, three
  diagnostic codes using the second; the member's name is the first's, as spec § 12's title is.
- **Outside the repository's root, written and then moved or removed**: the compiler-engineer's working
  files in the session's scratchpad for about 25 minutes (moved into its worktree), two logs of a seat's
  builds there (moved into the lane's scratch folder by the coordinator), one scratch file of the
  ffi-pragmatist under `/tmp` (removed in the same command), the critic's background build's output
  under the harness's task folder. CLAUDE.md § Hard stops forbids each; each is recorded where it
  happened, in the seat's report and here.

## Prior art

- **Agda's `--interaction-json`** (since 2.6.1, read by the historian on 2026-10-25): one answer carries
  `visibleGoals` and `invisibleGoals` beside `warnings` and `errors`, members of one document, the
  file's gaps never errors. R1's shape: holes beside diagnostics, never among them.
- **rustc's `rendered`, GHC's `rendered` (its JSON schema 1.2) and Lean's `PlainGoal.rendered`**: one
  answer in two renderings, a text and a structure, the same facts. R3's one value.
- **The Language Server Protocol's `isIncomplete`**: a list that says it was cut. R1's
  `"holes_complete"` and R4's counts; rustc's *and 17 others* is R4's text.
- **cargo adds fields without moving `--format-version`**, and notes the release that added each
  (`dep_kinds`, *Added in Cargo 1.40*): R7's rule that a member added comes with its meaning when
  absent.
- **GHC moves its schema's minor number on every added member** (1.0, 1.1, 1.2): D's precedent, a
  major move where GHC's is minor, recorded with D.

## Predictions to score

- **compiler-engineer (a)**, *the commits closing 2203, 2204 and 2205 touch neither renderer, with the
  value landed first*: **lapsed** at 1d66ece88, its premise not taken (2203 was repaired before the
  value was built, in `check/holes.hero`'s `in_scope` and the resolver).
- **compiler-engineer (b)**, *the commit closing 2203 adds a module under `selfhost/resolve/` or
  raises a ceiling of `resolve/walk.hero`, `resolved.hero` or `resolve/state.hero`*: **falsified** at
  1d66ece88, which closed it in `resolve/names.hero`, an existing module, `walk.hero` at 300 and
  `resolved.hero` at 299 by `suite_layout.hero`'s rule, no ceiling raised.
- **spec-warden**, at the member's landing: `git diff 9a38f1b26 <landing> -- spec/heroes-spec.md`
  empty and this sitting's ledger row 0; the warden's probe hole reads `… and 9 more` under its
  functions and `… and 3 more` in scope. Checkable at the landing.
- **ffi-pragmatist**, restated for the adopted route by the coordinator from the critic's measurement:
  a json-c 0.19 and a cJSON 1.7.12 consumer in each library's idiom, acting on `"holes"` only where
  `"holes_complete"` is `true`, end with no signal on every answer of the twelve golden programs that
  hold `???`, and refuse every answer of a compiler built at `9a38f1b26` at exit 2. One exit of 139, or
  one *finished* on a holed program, falsifies it. Checkable at the landing.
- **llm-ergonomist**: the 80 tasks re-run on the landed document score 8 of 10 or more on the second
  score in every family and shape, at 0.035 USD a session at most. Checkable at the landing, a paid run
  asked for then.

## Author's verdict

Pending: the decision issue `Panel 228:`, https://github.com/heroes-lang/heroes/issues/2207, holds the ratification. A yes settles R1 to R9 as the
member's landing and design.md's two corrections; it does not settle the repairs of defects 2204 and
2205, which are not this sitting's to rule, nor the thesis instrument's showing a model no report.

## Process notes

- **The critic read the briefs first, and its 22 findings changed the sitting before a seat ran**:
  the ffi-pragmatist seated, defect 2205 found writing the second blind design, the blind
  experiment redesigned so the answer was the only channel (the first design, `blind/first-design/`,
  was never run), and the blind command repaired to write no transcript outside the root
  (`--no-session-persistence`, measured: no folder under `~/.claude/projects` before or after) and to
  bound the sitting rather than each session.
- **The blind context rule was amended before the run**, after the probe of folder 1: *`context`
  answers `no`* would have voided every session for naming the one-sentence system prompt.
- **The coordinator repaired defect 2203 during the sitting**, in the lane the seats did not read,
  each seat working on its own worktree at `9a38f1b26`; two of the compiler-engineer's predictions are
  scored on that commit above.
