# Panel 217, the completeness critic, pass 1: the briefs checked

Written 2026-10-16 from about 21:50, in my own copy `<scratchpad>/217-critic/` (`git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach … f57af53da`, then `rm -rf build`),
with two compilers built there: `heroes` from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 8.99 user 8.50 sys 0.24`) and `heroes-r2` from it (`./heroes
build selfhost/main.hero -o heroes-r2`, `real 75.54 user 58.24 sys 12.99`). The readers' files were
copied from the coordinator's `readers-r2/` into `<scratchpad>/217-critic-work/` before anything
ran, and every probe below lives in `<scratchpad>/217-critic-work/probes/` and `…/copies/`. No paid
run, no API call, no credential printed. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.

**In short**: every number and path in the briefs checks out but two. **F3 is false as a new form:
the relaxation it describes is already the language** (§ 1, the forms). The spec-warden's
*2026-10-16 21:30* refresh could not be re-run by me and rests on one uncommitted ledger row. The
larger risks are in the arms: the judge scores the kit's own F0 answer, `doubled(@c)`, as correct,
so neither prediction can show the difference the sitting is about unless a person tabulates each
program's signature (§ 2).

## 1. The framing facts, one by one

### 00-shared.md

- **`f57af53da` is `f57af53dae65fedb66d8915311738b8bb05dd38e`, "M-buildable-structs step 7 … refused
  at build, ffi_lock_copied"**: verified, `git rev-parse f57af53da` and `git log -1 --format='%H %s'
  f57af53da`. `git log --oneline main..f57af53da` prints that one commit and `f57af53da..main`
  prints nothing: the lane is the trunk's head `6e2690b50` plus step 7 alone. `git diff --stat main
  f57af53da -- seed/` prints nothing.
- **The lane is `lane-land215`**: verified, `git worktree list` prints
  `…/.claude/worktrees/lane-land215 f57af53da [lane-land215]`.
- **`m-g-at`'s sha256 begins `ac4ff3e2`**: verified, `shasum -a 256 …/heroes-spec-m-g-at.md` prints
  `ac4ff3e263629118b7f63c9bf058f7b2b5b8f3825051b4601f0094f5893cfbd3`.
- **`Param`, `m-g-at` line 124**: verified, `grep -n 'Param       ='` prints `124:    Param       =
  [ "@" ] ident ":" Type .`
- **`lent` belongs to a C function's parameter alone (line 462)**: verified for the grammar. `grep
  -n lent` over the file prints lines 366, 369, 373, 412, 415 and 462, and 462 (`CParam`) is the only
  production holding it.
- **`lent` means *C does not keep it* (line 415)**: verified, the sentence running 414 to 415: *"a
  parameter, `@` or not, is taken to keep what it is handed unless declared `lent`"*.
- **`m-g-at`'s § 9 says *the callee works on the caller's place itself*, and the copy at `f57af53da`
  still has the old § 9 sentence**: verified, but **incomplete**. `diff <copy>/spec/heroes-spec.md
  m-g-at` prints 18 changed lines in three hunks: § 9 (272-273, *copy in, copy out*), and two in § 13
  (`rest: zero` moved after `partial`, the union sentence rewritten, `m-g-at` 383-391). The copy's
  spec is the trunk's byte for byte: sha256 `af7fc4d6…` for both (`git show main:spec/heroes-spec.md
  | shasum -a 256`). See § 2, item A6.
- **There is no way to write a Heroes function that takes such a record's place without declaring
  that it changes it**: verified on the compiler, for every route I tried. `function doubled(c:
  Counter lent)` is a parse error, ``error[expected_params_close]: expected `)`, or `,` and another
  parameter, found a name (`lent`)``. `.ptr()` on a `Counter` field is refused, ``error[bad_operand]:
  `ptr` takes a fixed run of bytes — `i8[N]` or `u8[N]`, found `Counter` ``. A by-value parameter lent
  onward with `@` is refused, `error[not_mutable]`. (Probes `heroes_lent_param`, `ptr_field_cell`,
  `byval_at_to_c`, each built with `heroes-r2`.) What I searched: `Param`, `CParam`, `.ptr()`,
  `.cstr()`, generic parameters. A route nobody searched may still exist.
- **Panel 215's re-run: ten readers of `m-g-at` all printed the right answer, all ten wrote
  `doubled(c: Counter)` by value and copied the counter**: verified. `scores.tsv` reads `c=ok` and
  `byval=h_c_doubled` on all ten rows. `grep -E '^function doubled' r*/c.hero.txt` prints `function
  doubled(c: Counter) -> i64` ten times, and each has exactly one line `<name> @= c`.
- **The re-run report read seven of ten notes (r01, r03, r04, r05, r06, r07, r09); tonight's grep
  finds four (r03, r05, r06, r08)**: the grep is reproduced, with the brief's own warning confirmed.
  The brief does not print its command. Mine, `grep -l -i -E '@c.*(chang|modif|mutat)|(chang|modif|mutat).*@c'
  r*/notes.md`, prints exactly r03, r05, r06, r08. Two neighbouring patterns print 2 (whole words
  only: r03, r06) and 8 (any `@` beside *chang/modif/mutat/writ*). **By reading** (`awk '/doubled/'`
  on each note, six lines), **nine of ten name `doubled(@c)` as the other choice and say it would mark
  the counter changed or writable**: r01 (*"marks the counter as writable at every call site"*), r02
  (*"the signature would allow writes"*), r03, r04 (*"allows mutation"*), r05, r06, r07, r08 (*"would
  allow writes"*) and r10 (*"the signature would then allow writes"*). r09 says it only by implication
  (*"to leave the counter unchanged, `doubled` takes the counter without `@`"*). So both 215's *seven*
  and tonight's *four* undercount; the reading is nine, ten with r09. It is still a reading, not a
  count.
- **With the trunk's compiler 10 of 10 build at exit 0 and print 6, 4, 5; with the lane's at R2, 10
  of 10 are refused, each with 4 to 7 `ffi_lock_copied` (r02 7, r05 5, the rest 4)**: verified by
  re-running, not only by reading the coordinator's files. My seed compiler (the trunk's seed, which
  is the lane's: `grep -c ffi_lock_copied seed/heroes.c` prints `0`) builds all ten at exit 0 and
  each prints `6 4 5`. `heroes-r2` refuses all ten at exit 1, with `grep -c
  'error\[ffi_lock_copied\]'` printing 4, 7, 4, 4, 5, 4, 4, 4, 4, 4 and no other error code. My ten
  message files are byte-identical to the coordinator's `build-r2.txt` (`cmp`, ten times silent), and
  the ten programs are byte-identical to `docs/panel/215-reports/llm-ergonomist-work-2/r*/c.hero.txt`.
  One caution: I used the seed compiler, not the trunk's `./heroes` binary, so *the trunk's compiler*
  is verified as *the trunk's seed*.
- **r01's four refusals, the `guess` fix on the parameter only, and the note**: verified by reading
  `build-r2.txt`. The brief quotes the note's first half; it goes on *"; a field is read in place,
  `c.n`"*, which matters to arm R2 (§ 2, A3). `grep -c 'fix (guess)'` prints 1 in each of the ten
  files.
- **The kit's key writes `doubled(@c: Counter) -> i64`, `return counter_value(@c) * 2`, every C
  function `@c: Counter lent`, `counter_value`'s header `const struct counter *`**: verified, `cat -n
  key/c.hero` (lines 4 to 7, 17, 18) and `counter.h`. With `heroes-r2` the key builds at exit 0 and
  prints `6 4 5`.
- **Issue 2098, task of M-buildable-structs; issue 2070, panel 215, *3a***: verified, `gh issue view
  2098 --json …` prints `kind:task`, milestone `M-buildable-structs`, OPEN, *"Convene a sitting on a
  read-only lend …"*; 2070 is CLOSED, *"Panel 215: …"*. Panel 215's file says *3a* at its
  ratification paragraph.
- **The prediction text** (*at least 8 of 10 correct on task C's second try; at most 2 of 10 copy
  the lock first try*): verified, line 217 of the 215 file, under `## Predictions to score` (line
  209). The brief's path `docs/panel/215-…-reads.md` is that file (its slug ends `…what-c-reads`).
- **About 8 s and about 75 s**: verified, 8.99 and 75.54 `real` above.
- **The seed at `f57af53da` predates R2**: verified, `ffi_lock_copied` appears 0 times in
  `seed/heroes.c` and in `selfhost/check/lock_copies.hero` and `selfhost/resolve/writes.hero`.

### The forms, F0 to F3

- **F2's *the copy stays* (a reading, unrun)**: the half that today's compiler can answer agrees.
  `return counter_value(@c) * 2` on a by-value `c` is refused ``error[not_mutable]: `c` is a
  parameter without `@`, so it cannot be written — mark it `@c` …`` (probe `byval_at_to_c`). F2 itself
  is unbuilt, so the claim stays a reading.
- **F3 is FALSE as a new form.** F3 is *"panel 210's refusal of an `@` parameter nothing writes,
  relaxed where the only use is an `@` argument"*. That relaxation is already the language at
  `f57af53da`:
  - `selfhost/resolve/written_nowhere.hero` line 7 says it in its own words: *"What counts as a write
    is the resolver's: `@` to it, a field or an element; an `@` argument; …"*;
  - the kit's key, whose `doubled(@c)` only lends `@c` to C, builds at exit 0 under `heroes-r2`;
  - probe `at_lent_to_heroes_at` (`outer(@c)` whose only use is `inner(@c)`, which only lends to C)
    builds at exit 0;
  - only an `@` parameter that is merely READ is refused: probe `at_read_only` (`return c.n * 2`) is
    `error[never_rebound]`.

  So F3 as written equals F0. The form the brief probably means is a relaxation for an `@` parameter
  only **read**, which is listed nowhere (§ 3, U5).
- **F1's C half, `counter_value(c: Counter lent)` with no `@`, is refused today**: probe
  `c_lent_noat`, ``error[lent_shape]: `lent` on `c` of `counter_value` is a word nothing reads — `c`
  is not a `cstr` or a `ptr`, and only those two are ever lent``. That message's *"only those two are
  ever lent"* is already too strong today, since `@c: Counter lent` builds. F1 moves `lent_shape`,
  and the compiler-engineer's brief does not name it among the checks it touches. `grep -rln
  lent_shape selfhost` prints `selfhost/ffi_errors.hero`, `selfhost/field_lend_errors.hero` and
  `selfhost/check/lend_extent.hero`, none of them in the brief's list.

### compiler-engineer.md

- Every file it names exists at `f57af53da` (`wc -l`): `selfhost/resolve/written_nowhere.hero` 195,
  `selfhost/check/lock_copies.hero` 271, `selfhost/check/lending.hero` 408,
  `selfhost/check/leasing.hero` 311, `selfhost/print/fmt.hero` 1773, `selfhost/parse/` (a directory,
  60 files). The `expected_params_close` my `lent` probe met is raised in
  `selfhost/parse/members.hero` (`grep -rln`). `.claude/rules/diagnostics-and-goldens.md`
  line 96 is *"## A new surface form lands in every tool that reads the language"*; the brief's
  section name is a shortening and points to the right place. `tests/harness/suite_layout.hero`
  holds `DECIDED` 15 times.
- design.md §4.8 (*Mutable parameters: `@`*, line 1411) and §4.10 (*Value semantics and
  copy-on-write*, line 1666) exist (`grep -n '^#'`).
- The `check`, `run` and `emission` forms are named in `tests/harness/main.hero` (`grep -oE`).
- **Not named by the brief, and touched by F1**: `lent_shape` (above); `not_mutable` and
  `never_rebound`, whose messages still say *"copy in, copy out"* and *"its copy-out hands every
  caller back the value it passed"* (probes `byval_at_to_c` and `at_read_only`), while `m-g-at`'s
  § 9, which the readers read, now says there is no copy. That is R3's landing's business more than
  this sitting's, but the R2 arm's judge prints those words.

### ffi-pragmatist.md

- **The `git grep` lists the files**: it prints **939** files (`… | wc -l`), of which **900 are
  under `tests/golden/`**: 327 `run`, 327 `unsupported`, 142 `check`, 49 `surface-fixtures`, 40
  `fixedbugs`, 5 `full`, 4 `permissive`, 3 `ir`, 2 `lldb`, 1 `emit`. **509 of them (`unsupported`,
  `check`, `fixedbugs`) are programs the compiler is meant to refuse**, and one more is
  `archive/bootstrap-rs/heroes/src/library/source.hero`, which nothing builds (CLAUDE.md § 5). A
  count of *the extern parameters today declared `@x: T` whose header is a pointer to `const`* over
  that list mixes wrong bindings kept on purpose with real ones. The real bindings are 21 under
  `examples/`, 12 under `selfhost/` and 5 under `tests/harness/`; the brief should say which set
  counts.
- **`heroes-linux-arm64`**: exists as a Docker image, `heroes-linux-arm64:latest` and
  `:sdl3-b14` (`docker images`), not as a running container (`docker ps -a` lists one container,
  `distracted_curran`). **`ssh win`**: timed out during this check (`ssh -o BatchMode=yes -o
  ConnectTimeout=5 win 'echo ok'` printed `connect to host apponfly-vps port 22: Operation timed
  out`), so that leg is *unrun* unless it comes back.
- `selfhost/cli/pointee.hero` exists (607 lines). design.md §1.11 (line 477) and §4.19 (line 2436)
  exist.

### historian.md

No framing fact the repository can check; every precedent is the seat's own to source.

### spec-warden.md

- **10,108 real on `claude-opus-5`**: the figure is in the record. `llm-ergonomist-rerun.md` line 18
  gives it, *"taken by the coordinator with `heroes measure spec/heroes-spec.md --refresh` in a
  scratch copy at `7e5c02048`"*, a file committed 2026-10-14 14:40 (`git log`). **The time
  *2026-10-16 21:30* could not be verified**. It is written in one row of an uncommitted ledger,
  `<scratchpad>/step9-aside/docs/measurements/010-spec-budget-ledger.md` line 229 (*"priced on one
  `--refresh` at 21:30 on 2026-10-16: **10,108 real**"*), and in no tracked file. Re-running it is an
  API call my brief forbids: *unrun*. Also, **`7e5c02048` no longer resolves** (`git log -1
  7e5c02048` prints *unknown revision*), probably since the history rewrite of 2026-10-16. A dated
  record cites it, so a reader can no longer rebuild that scratch copy.
- **A coincidence to keep apart**: `./heroes measure spec/heroes-spec.md` at `f57af53da` prints
  `real 10048 claude-opus-5, 2026-10-13` and *"what is measured against the ceiling is 10108"*, which
  is 10,048 plus the FFI floor's 60. `m-g-at`'s own count is also 10,108. So `m-g-at` measured
  against the ceiling is **10,168**, not 10,108. A seat that reads the tool's *10108* as `m-g-at`'s
  starts from the wrong figure.
- **The ceiling 16,384**: verified, `tests/harness/suite_spec.hero` line 157 (`constant CEILING:
  i64` / `16384`), and `measure` prints *"Headroom: 6336 against the 16384 ceiling"*.
- **Lines 124, 271-273, 415, 462**: verified, except that § 9's `@` sentence runs to line 274
  (*"not apply when the first parameter is `@`."*), so it is 271 to 274.
- **`.claude/rules/spec-shape.md` § Where a rule lives**: exists, line 49. design.md §1.2 (line
  190) and §1.6 (line 253) exist. `.env` exists in the trunk (`test -f`); not read.
- **Apply the draft to `spec/heroes-spec.md` in your own copy**: at `f57af53da` that file is not
  `m-g-at` (above). Drafting on it starts from 10,048 and from the old § 9 and § 13. See A6.

### llm-ergonomist.md

- **The launcher's flags**: verified, `run_one.sh` line 4 reads `--bare --restricted
  --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 0.7
  --output-format json`. It **pins no `--model`**.
- **Ten sessions cost 2.2106 USD**: verified, the sum of `total_cost_usd` over the ten `run.json` is
  `2.2106` (`python3 -I`). Each session's `modelUsage` names `claude-opus-5-5` (and
  `claude-haiku-4-5-20251001`). The bound, 20 × 0.70 = 14.00, and the expectation, 2 × 2.2106 ≈
  4.42, are arithmetic. The expectation is an inference, because 215's sessions each wrote three
  programs and tonight's write one, with longer inputs in arm R2. Panel 216 names no paid run
  (`grep -i 'usd\|paid' docs/panel/216-briefs/*.md` finds only its *no paid run* rule), so the
  evening's 10 USD is this sitting's alone, as far as the briefs say.
- **"Each in a folder of its own with `git init`, outside any project context"**: **not what panel
  215 did**. Its `run.log` lists every reader at
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/readers2/rNN`, inside this repository, and
  r05's note quotes that path back (*"the working directory path (a `.claude/worktrees/readers2/r05`
  worktree)"*). See A9.
- **"Five at a time"**: could not be verified for 215. Its `run.log` shows ten exits between `start
  12:41:09` and `end 12:43:52` and nothing about concurrency. For tonight it is a plan.
- **`readers-r2/r<NN>/build-r2.txt`**: exists for all ten, verified above.

## 2. How the two blind arms could mislead

- **A1. The judge scores F0 as correct, so the sitting's difference is invisible to it.**
  `judge.sh`'s `c=ok` means *builds, runs, prints `6\n4\n5`*. The kit's own key is F0,
  `doubled(@c)`, and builds `ok`. So in arm R2, "8 of 10 correct" is met by readers who take R2's
  fix and write a signature claiming a change. In arm F1, "at most 2 of 10 copy" is met just as well
  by 10 of 10 writing `@c` and nobody using the form. **Each arm needs, per program, a person's
  tabulation of `doubled`'s parameter** (by value plus a copy, `@c`, the read-only lend, other) and
  of whether the note says the signature claims a change. **And arm F1's prediction needs a second
  half that F1 can fail**: *at least N of 10 write the read-only lend*. Neither the brief nor 215's
  prediction says either.
- **A2. The task's words decide.** Task C says *"without changing the counter"*. In 215, those
  words are what every reader cites for rejecting `@c` (above, nine of ten). In arm F1 the form is
  defined as *read and never written*, so the task states the form's own definition. A high F1
  score can measure the overlap between the task and the new sentence, not the form's reach. A
  control task without those words (*"a function that gives a counter's value doubled, using
  `counter_value`"*) would show whether readers reach for the form unprompted. In arm R2 the same
  words pull against R2's own fix (`@c`), so the arm measures a conflict between the message and the
  task.
- **A3. R2's messages hand arm R2 the answer, and one of their notes leads to a second refusal.**
  The `guess` fix says *"take it by place, and lend it at each call with `@`"*. The note says
  *"`f(@c)`; a field is read in place, `c.n`"*. A reader who follows the second half, `doubled(@c)`
  with `return c.n * 2`, is refused `error[never_rebound]` (probe `at_read_only`). Panel 210's rule
  refuses the very shape R2's note describes. The judge then prints `build:never_rebound`, which
  reads as the reader's failure and is the compiler's.
- **A4. Not the same reader.** 215's prediction is about *readers … on task C's second try*. Arm R2
  hands a FRESH session another session's program, without that session's notes or reasons. 215's
  `run.json` keeps no `session_id` (its keys are `duration_ms, is_error, modelUsage, num_turns,
  result, subtype, total_cost_usd`), so the original readers cannot be resumed from the record. The
  arm scores the prediction on a different experiment, and the report should say so.
- **A5. A different session shape from the baseline.** 215's readers wrote A2, U and C in one
  session, A2 first. A2 binds `const struct tagged *` as `@t: Tagged lent`, which primes *a const
  pointer is `@`* (r05 and r09 say it, r09: *"against the rule that C pointers are `@` parameters"*).
  Both arms give task C alone. Arm F1's *at most 2 of 10 copy* is then compared with a 10 of 10
  measured under a different priming. Under F1, A2's own binding would change too
  (`tg_kind(t: Tagged lent)`), and nobody tests it.
- **A6. Arm F1 may read a different base specification.** The arm reads *"the spec-warden's draft
  over `m-g-at`"*, while the spec-warden is told to apply the draft to `spec/heroes-spec.md` in its
  copy. At `f57af53da` that is the trunk's spec, which differs from `m-g-at` in § 9 and twice in
  § 13. A draft made there gives the F1 arm a document without R3, and the R2 arm (which reads
  `m-g-at`) a different one.
- **A7. The draft and the judge may disagree.** Arm F1 reads the spec-warden's spelling and is
  judged by the compiler-engineer's prototype, whose spelling is *"the spec-warden's to settle;
  build this one unless …"*. A difference in spelling, in the call's mark, or in what a C parameter
  with no `@` may say (today `lent_shape`) refuses a reader who followed the draft. 215 had a key
  built before the run. **217 names no F1 key**: task C written by the draft's rules and built by the
  prototype before any reader runs.
- **A8. A refusal can hide a copy.** `judge.sh` keeps the first error code only (`grep -m1 -oE
  'error\[…\]'`, line 15). Worse, the compiler itself does not report a copy once the resolver has
  refused anything. Probe `masked_copy` (an `@` parameter only read, plus r01's by-value `doubled`)
  prints `error[never_rebound]` and no `ffi_lock_copied`. "Copies the lock" must be read from each
  program's source, never from the build.
- **A9. The readers ran inside this repository, and the briefs say otherwise.** By `claude --help`
  (2.1.292, local, no session run): `--bare` skips *"CLAUDE.md auto-discovery"*, and `--restricted`
  *"confines the file tools to the working directories (--add-dir included)"*. Whether that
  confinement holds was not run, since running it is paid: *unrun*. If tonight's readers again sit
  under `.claude/worktrees/`, their parent directories hold the F0 key
  (`docs/panel/215-reports/spec-warden-work/q3/kit/key/c.hero`) and this sitting's briefs, which name
  F1 and the expected outcomes. Only that confinement keeps them out, and every reader sees the path
  `…/heroes-lang/.claude/worktrees/…`. Running the readers in the scratchpad, outside the repository,
  costs nothing and removes the question.
- **A10. The model is not pinned.** 215 ran on `claude-opus-5-5` (`modelUsage`). The unchanged
  launcher takes the account's default. Each new `run.json`'s `modelUsage` should be read before
  scoring against 215.
- **A11. `context: yes` will not discriminate.** All ten 215 readers wrote `context: yes`, because
  the harness's own system prompt reached them (`grep -n context r*/notes.md`). The field shows a
  leak only if the arm reads *what* each says, as 215 did.

I also checked the other way a judge could miss a copy: a lock copy the compiler at R2 cannot see. I
built 18 probes with `heroes-r2` (in `…/copies/`). **All 13 that copy a `Counter` and reached the
checker were refused `ffi_lock_copied`**: an immutable binding, a field assignment, a copy of the
whole `Stats`, `Stats` by value, an array literal, `push`, a generic identity, a returned `Counter`,
a variant case, UFCS by value, a map value, and an `if` and a `match` yielding a `Counter`. `==` was
refused `ffi_partial_operation`. Three of my probes were malformed (two variant spellings and an
inline `if`), and one, an f-string of `s.first.n`, copies nothing and built, as it should. I found
no hole in R2 itself.

## 3. Forms of a read-only lend the briefs do not list

The briefs list F0 to F3, from panel 215's question 2, the notes and the grammar. Each form below
comes from reading `m-g-at` § 9 and § 13 and the probes above. None is built, and each is a
question, not a recommendation.

- **U1, a new word instead of a second meaning for `lent`**: `doubled(c: Counter in)`, `read`,
  `view`, `borrowed`, or similar. F1 makes `lent` mean both *C does not keep it* and *nothing writes
  it*. On a C parameter these are two separate facts, and F1 cannot say *C keeps a pointer it only
  reads*: no `lent` means *kept*, and a record with no `@` and no `lent` is a copy. The ffi brief
  raises the case; no form separates the two.
- **U2, F1 with a mark at the call**: the read-only lend marked where it is passed, `doubled(&s.first)`
  or a method like the existing `s.cstr()`. Under F1 `doubled(s.first)` reads exactly like the
  by-value copy that R2 refuses everywhere else, and the reader of the call must open the signature
  to know. The briefs list only the unmarked call.
- **U3, by type**: a record the language refuses to copy (one whose C struct holds a lock) is passed
  by place wherever it is passed by value, with no new word. The parameter is then read-only by
  construction, and a C parameter of that type with no `@` binds `const struct counter *`. It is F2
  restricted to the refused types and extended to C, which F2 as listed is not. Under it, all ten
  readers' signatures stand; their `view @= c` copies stay refused.
- **U4, a lend method at the call to C, with no parameter form**: extend `.ptr()` (today
  byte-arrays only, `bad_operand`) or add a sibling, so that a record field is lent read-only to C.
  § 13 already says *"C writes back through the lend only where the binding is a `@` name"*. This
  covers `counter_value(stats.first…)` from `main`, but not a Heroes helper. It is worth naming
  because the language already has two read-only lends to C (`.cstr()`, `.ptr()`), and none for a
  record.
- **U5, the real F3: an `@` parameter only READ admitted**: `never_rebound` relaxed for an `@`
  parameter that is only read, perhaps only for types that cannot be copied, plus a § 9 sentence
  saying `@` lends the place and the function MAY change it. It is not the same as the F3 listed,
  which is already the language (§ 1).
- **U6, a diagnostic-only form**: R2's note for a parameter the body never writes says what a
  reader can do without claiming a change, or says that `@c` is what lending a place costs. It
  changes a message class and no grammar. The R2 arm measures today's message only.

## 4. Questions the sitting should ask and does not

1. **Which base specification**: `m-g-at`, copied over `spec/heroes-spec.md` before any draft and
   any `measure`? Its count against the ceiling is 10,168 with the floor, not 10,108 (§ 1,
   spec-warden).
2. **Aliasing between a read-only lend and an `@` lend of one place in one call.** Today
   `bump_and_read(a: @p, b: p)`, where the body adds 1 through `a` and returns `b.x`, prints `1` and
   then `p.x` prints `2` (probe `alias`, `heroes-r2`, exit 0). `b` is a copy taken before the call.
   Under F1 with `b: Pt lent` passed by place, the read through `b` sees the write: `2`. So F1
   changes what an existing program shape prints, unless the pair is refused, as two `@` arguments
   are (`aliased_mutable_arguments`, which panel 215 says it ran). Neither the briefs nor the seats'
   questions name it. The same question at the C boundary: `memcpy`-like functions with a `const`
   source and an `@` destination of one place.
3. **UFCS**: § 9 says UFCS does not apply when the first parameter is `@`. Does `s.first.doubled()`
   apply to a read-only first parameter, and does it then read as a copy?
4. **Which types may be lent read-only**: every record, or only those that cannot be copied? `n:
   i64 lent`, a `str`, an array? And may the callee copy OUT of a read-only lend, for a record with
   no lock? The compiler-engineer's brief refuses *"copied"* outright, which for an ordinary record
   is stronger than F1 needs. That should be a decision, not a default.
5. **A `const` pointer that C launders**: `strchr`-like functions return a non-`const` pointer into
   what they were given as `const`. Under F1, may a `lent` argument's place come back as a result?
   The brief says *"returned"* is refused for the Heroes side only.
6. **What R2's message says once the form exists**, and an arm for it. Neither arm tests a reader
   refused under F1 with an F1-aware message. That is the second try F1 would actually produce.
7. **A control arm with the task's words changed** (A2), and the arms' tabulation of every
   program's signature (A1). Both cost nothing but reading, except the control arm, which is one
   more paid arm of up to 7.00 USD (ten sessions at the 0.70 bound) for the coordinator to weigh.
8. **`never_rebound`'s and `not_mutable`'s texts** still teach copy-in and copy-out against
   `m-g-at`'s § 9. Is that the R3 landing's to fix, or does this sitting's form move them anyway?
