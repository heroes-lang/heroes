# Panel 228, shared brief: a hole's report inside `heroes check --json`

**Convened 2026-10-25** in M-rechecked-issues, on feature 2091 and defect 2158
(https://github.com/heroes-lang/heroes/issues/2091,
https://github.com/heroes-lang/heroes/issues/2158), by the author's yes of that day, asked once for
the milestone (CLAUDE.md § 4). **Full panel**: the five seats and a completeness critic, which read
these briefs before any seat started (`docs/panel/228-reports/critic-briefs.md`, 22 findings) and
reads the reports after. The first draft left the ffi-pragmatist out, *a C consumer of the document
was panel 193's question*; the critic measured that route A crashes such a consumer on every older
compiler (§ Route D below), a question of exactly that seat's kind, so it sits. § Repaired, at the
end, says what the critic's first pass changed.

**The tree**: the lane `rechecked-issues` at `9a38f1b26923bc241d1d5b211c88da0746117375` (`git log
-1`, 2026-10-25 02:41), whose `selfhost/`, `runtime/` and `seed/` are the trunk's `76e98124e` (`git
diff --stat 76e98124e HEAD -- selfhost runtime seed` prints nothing). **Each seat that builds works
in its own detached worktree at that commit**, `.claude/worktrees/228-<seat>/`, made by the
coordinator, builds its own compiler there, reads nothing of the lane but its brief and writes
nothing there but its report, `docs/panel/228-reports/<seat>.md`; the lane moves meanwhile, and no
seat reads it. **No seat writes outside the repository's root** (CLAUDE.md § Hard stops); no seat
reads, builds or runs in another's worktree; **no seat prints a credential** (`gh auth status`,
never `gh auth token`); none starts a paid run: the blind seat's sessions are the coordinator's,
inside the author's 3 USD of 2026-10-25.

## The proposal (route A)

`heroes check --json` on a program whose stages are quiet and which holds `???` writes, in the
document it writes to stderr today (`"schema": 2`, `"kind": "diagnostics"`), a member `"holes"`
after `"diagnostics"`: one object per hole, in source order, carrying what the text report carries
and nothing else, each list at the text's cap and in its order. Its keys, as the blind seat reads
them:

```
{"file": "main.hero", "line": 5, "col": 22,
 "expected": "lib.Box",
 "in_scope": [{"name": "edge", "type": "i64", "mutable": false}], "more_in_scope": 0,
 "fields": [{"name": "width", "type": "i64"}, ...],
 "functions": [{"call": "lib.cube(side: i64)", "doc": "Builds a cube whose edges all measure `side`."}, ...]}
```

`"expected"` gives way, where nothing is expected of the hole, to `"position": "statement"` and,
where the function's result is not `()`, `"returns"`; a variant's `"cases"` stand where a record's
`"fields"` do. Exit 0 is kept; `"schema"` stays 2; under `--json` the text report is not printed to
stdout, the document being the answer. A program with no hole writes `"holes": []`.

## The issues, as filed

- **Feature 2091** (`kind:feature`, `area:cli`): *a hole's report inside `heroes check --json`:
  today the expected type, the names in scope and the candidate functions reach stdout as text
  alone*; its Origin: *`--json` says how to print, never what (`.claude/rules/cli-surface.md`), so
  this needs no new verb*.
- **Defect 2158** (`kind:defect`, **`class:adjacent`**): *a reader of the JSON cannot see the hole*;
  its Class: *a true answer less complete than it could be ... no wrong value, no crash, no false
  message; **the instrument that found it has another route*** (`build`'s `no binary:`). Its § 9 is
  § 12, corrected beneath it on 2026-10-25.

Both are inputs to the stopping rule (§ What is true today, last bullet), and the spec-warden judges
that reading, not a blank.

## What is true today, each with the command that settles it

The programs are in `.claude/worktrees/rechecked-issues-scratch/hole/` and the critic's in
`.claude/worktrees/228-critic/critic228/`, each run with a stage-2 compiler of `9a38f1b26`, `heroes
--version` printing `sources 451802aa66692eb3`, built by `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (8.29 s) and then `./heroes build selfhost/main.hero -o heroes-next`
(82.24 s real in the lane, 87.09 s in the critic's worktree).

- **The document holds no hole.** `check --json main.hero`, a `main` binding `d: i64 = ???` beside
  two `Point`s and `dist2(a: Point, b: Point) -> i64`: exit 0; stderr `{"schema": 2, "kind":
  "diagnostics", "read": [...], "diagnostics": []}`; stdout, as text, `hole at <file>:13:14`,
  `expected type: i64`, `in scope:` and `functions that return it:`. `check --brief` prints the same
  text at exit 0; `build` prints it and `no binary: 1 hole in <file>` at exit 1; `build --json` is
  refused, exit 2.
- **Where it is written**: `selfhost/cli/check.hero:86-87`, after every stage was quiet, under the
  comment *§4.16: holes are output, not diagnostics* (`:84`); the `--json` document at `:77-79`,
  written before it. The report is one string built in `selfhost/check/holes.hero` (`report`,
  `in_scope`, `fields_of`, `nearby`), with `SUGGESTIONS` 5 at `:28-29`. **The cap differs by list**:
  `in_scope` writes `… and N more` (`:113-119`), `nearby` stops at five and says nothing
  (`:185-186`), `fields_of` has no cap; each function's first documentation line is written under it
  (`:217-218`, asserted by the module's test at `:279`). `wc -l`: `check.hero` 449,
  `check_json.hero` 181, `holes.hero` 306.
- **A hole beside an error is reported nowhere.** `both.hero`, `a: i64 = "x"` then `b: i64 =
  ???`: exit 1, one `type_mismatch`, nothing on stdout. And **under `--apply --json` the hole is
  told by no stream**: `check --apply --json main.hero` exits 0 with the `"kind": "applied"` document
  on stdout and stderr empty (critic, finding 14); `tests/harness/apply_tree.hero:91-92` pins that
  document (`"rounds": 0`, `"not_applied": []`) on a file holding a hole, and `--apply --in-place`
  without `--json` pins *a clean file with a hole prints nothing at all*.
- **What the report says is false in three ways, each filed `blocking` on 2026-10-25**, so a
  document carrying today's report carries them:
  - **defect 2203**, widened that day: the names the report lists are not the resolver's at the hole.
    It lists the binding the hole's own initialiser defines (`d: i64 = ???` lists `d`) and the names
    of a block already closed (a name bound in an `if` body, a `for`'s variable and its body's
    names, at a hole after them), each refused `unknown_name` at that column; `in_scope` models a
    local as visible *in the declaration that binds it, from its name on* (`holes.hero:85-88`,
    `:100`);
  - **defect 2204**: a hole whose value is used is told `position: statement — any type, or none`
    when the checker's expected type is `.error` (`holes.hero:48-58`): a `for`'s array, an `if` or
    `match` branch's value, and `x = ???` whose `x` is used later; a value of the wrong type is
    refused at each. Beside it, such an `x` is listed at a later hole with its type rendered `?`,
    which reads as Heroes' fallible suffix (critic, finding 3);
  - **defect 2205**: a type declared in another module is named bare, `expected type: Box`, `fields
    of Box` and `b: Box` in scope where the file must write `lib.Box`, while `lib.cube(side: i64)`
    beside them is qualified; `Box(...)` there is `needs_qualifying` and `b: Box` `unknown_type`.
    The same rendering (`selfhost/check/render.hero` `render_ty`) gives a type mismatch's wording,
    ``expected `Box`, found `i64` ``.
- **The schema**: `selfhost/cli/check_json.hero:38-39`, `SCHEMA` 2, which the help, the page a model
  reads and the errors pages say, each held to it (`site/src/lib/claims.ts:86-90` and `:536-542`).
  **Panel 193's reason for 2**
  (`docs/panel/193-a-fix-says-where-it-applies-and-what-apply-does-with-it-and-every-answer-says-what-it-is.md`):
  its historian, `:95`, *additive fields keep 1, as `cargo metadata` does*; the sitting's reason,
  `:69-74`, that two documents would have said `"schema": 1` and *a 19-line json-c consumer that
  checks `"schema": 1` crashes at exit 139 on the first where today it refuses loudly*; R3's
  conservative alternative, `:135-137`, *schema 1 and the keys added under it, as `cargo metadata`
  adds them*.
- **Who writes, who reads, and who only mentions** the document (`git grep -l -e 'check --json' -e
  '"schema": 2' -e check_json` and `git grep -l -e '"--json"' -- selfhost tests/harness site tools
  editors`, read by the critic, finding 11):
  - writers: `selfhost/cli/check_json.hero`, `selfhost/cli/check.hero`, and the `--apply` document's,
    `selfhost/cli/apply_answer.hero` (`:50`, `check_json.head_of`) and `selfhost/cli/certain.hero`;
  - readers, each with its stream: `selfhost/mutate/json_read.hero` and `judged.hero` (`heroes
    mutate`, RFC 8259 whole, stderr); `tests/harness/fix_names.hero:97-99` (`check <case> --json`
    over every `tests/golden/check/` case, each `"title"` from stderr; six of those cases hold
    `???`); `tests/harness/suite_surface.hero:315` and `tests/harness/apply_tree.hero:91-92` (`--apply
    --in-place --json`, stdout); `tests/harness/thesis_sha256.hero:14` (the `read` digests);
    `tests/harness/thesis_grade.hero:151-171` (`check --json` first, any exit but 0 and 2 graded
    `compile_error` with the codes of `"diagnostics"`, a hole reaching its own outcome, `hole`,
    only through `build`'s `no binary:`, and standing outside the denominator, `:15-16`);
    `site/src/llms.txt:61-66`, the page a model reads;
  - mentions only: the milestone page of M-lsp-server (row 84), whose server is to serve *the people
    who arrive with* `heroes check --json`; two golden cases whose one match is a comment.
- **What a route can move in the goldens**: `grep -rl -F '???' tests/golden --include='*.hero' |
  grep -v recovery` prints twelve programs (`check` 6, `ir` 1, `run` 1, `surface-fixtures` 3,
  `unsupported` 1); `grep -rl 'hole at' tests/golden` prints six files, four of them the recovery
  corpus.
- **The cap's home**: design.md §4.16 says the suggestions are *capped at 5, in deterministic order
  — and the cap is stated in the spec*. **No revision of the specification has stated it**, by the
  critic's searches (finding 19): the spec has stood at `spec/heroes-spec.md` since its first commit,
  `d10fbec74`, its holes paragraph then word for word today's, and `git log -S` over it finds no
  commit for `suggestion`, `deterministic order`, `nearby`, `capped`, `at most 5`, `at most five` or
  `SUGGESTIONS`. A repair to rule, not a history to find; the claim rests on those words.
- **The contract** (`.claude/rules/cli-surface.md` § One command): the artifact on stdout,
  diagnostics on stderr; exit 0 clean, exit 1 the input has diagnostics, exit 2 the tool could not
  run; *`--json` says how to print, never what*. **And the stopping rule**: a capability enters the
  surface only if the fixpoint invocation, the golden harness or the Part 11 harness must type it, or
  it has a measured Part 11 effect. 2158 says the Part 11 instrument has another route; 2091 says no
  new verb is needed.

## The routes the sitting judges

- **A**, the proposal: `"holes"` inside the stderr document, schema 2, stdout empty under `--json`.
- **A'**: A's member, and the text report kept on stdout under `--json`, as today; one answer in two
  renderings, and stdout unchanged for every reader of `check --json` on a holed file (`--json
  --dump-scopes` included).
- **B**: the report, which is check's artifact, as a second document on stdout, `{"schema": 2,
  "kind": "holes", "read": ..., "holes": [...]}`, the stderr document unchanged. A reader of stderr
  alone still takes the file for whole, defect 2158's complaint; and two flag combinations already
  write to that stdout (critic, finding 14): `--apply --json` its `"kind": "applied"` document, and
  `--json --dump-scopes` the scope dump, then `holes in `main`: unused bindings are not reported
  there`, then the hole's text, so B would put two documents, or a document after text, on one
  stream, which no RFC 8259 reader takes whole.
- **C**: each hole an entry of `"diagnostics"`, a code such as `hole`, its report in `"notes"`. It
  makes a hole a diagnostic, which §4.16 says it is not; a non-empty `"diagnostics"` at exit 0
  breaks the exit contract, and exit 1 changes what `check` says of a program the spec calls valid
  and **moves the thesis instrument's `hole` outcome into `compile_error`**, inside the measure the
  thesis is judged on, unless the grader changes in the same landing (critic, finding 13).
- **D**: A's member under `"schema": 3`. **Measured by the critic (finding 16)** with json-c 0.19,
  `critic228/jsonc/probe2.c`: on today's answer, which has no `"holes"`,
  `json_object_array_length(json_object_object_get(doc, "holes"))`, the idiomatic read, **faults,
  exit 139**; on route A's answer it reads 1. So under A a consumer written for `"holes"` crashes on
  every `heroes` older than the landing, the exit panel 193 cited for choosing 2; under D it can
  refuse by the number. A consumer that asks `json_object_object_get_ex` first does not crash.
- **E**: a count, not the report: the stderr document gains `"holes": <n>` (or the places alone,
  file, line and column), the text report staying on stdout. It answers 2158 as filed with one member
  and none of the false content above in a document a machine trusts; it does not answer 2091, which
  asks for *what goes there*.
- A route nobody listed is welcome, with what would make it right.

## The questions inside every route

1. **Holes beside errors**: whether a hole the type stage reached is reported when another stage
   spoke, in the text and in the document; and whether `--apply --json` says it.
2. **The cap** in a machine's document: kept per list as the text keeps it, the asymmetry included
   (`in_scope`'s count written, `nearby`'s not), or a count for every capped list, or every item.
3. **The types**: a string as the file writes the type, qualified by its module as defect 2205's
   repair will write it, or a structure; and what a type the checker does not know is (`?` today).
4. **The `--apply --json` document**: whether `"kind": "applied"` carries the holes of the program
   it hands back, today told by no stream.
5. **The order of landing**: whether the document waits for the repairs of defects 2203, 2204 and
   2205, so a machine never reads a false name, a false position or a spelling the file refuses;
   their repairs are not this sitting's to rule.
6. **The cap's sentence**: design.md §4.16 claims the spec states it, and no revision did; the spec
   states it, priced, or §4.16 is corrected.

## Repaired after the critic's first pass

`docs/panel/228-reports/critic-briefs.md`, 22 findings, every one taken: the ffi-pragmatist seated
(16); the widened 2203, the fourth shape of 2204 and the type `?` (1, 2, 3); defect 2205, found by
the coordinator writing the second blind design; the readers split into writers, readers by stream
and mentions, `apply_tree.hero`, `fix_names.hero`, `suite_surface.hero:315`, `thesis_sha256.hero` and
the grader named, the two golden cases called mentions (11); twelve holed programs beside six
renderings (12); C's cost to the grader (13); B on a stdout already written (14); the issues quoted,
2158's § 9 corrected beneath it (15); panel 193 quoted whole (16); the historian's precedents asked as
questions (17); the functions' documentation lines and the cap's asymmetry (18); question 6 settled
as a repair (19); `grep -- '--json'` (20); routes A' and E (21); the lane described by what the seats
read (22). The blind seat's experiment was redesigned (4 to 10): `blind/README.md`.
