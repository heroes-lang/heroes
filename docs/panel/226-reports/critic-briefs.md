# Panel 226, the completeness critic's first pass: the briefs against the tree

Written 2026-10-20, before any seat was launched. Copy: `<scratchpad>/226-critic/`, a `git archive`
of `a0a8d1249` (12,801 files by `find . -type f | wc -l`, no `.git`). Compiler built there from the
seed in **7.78 s** real (`heroes 0.4.0`, `sources 5f45d6514b22d650`); stage 2 built in **79.59 s**
real (user 61.40, sys 13.07; `heroes-next` says `sources 7792ecfe50a531e8`, so the two binaries
carry different keys). Read: `critic.md`, `00-shared.md`, the four seat briefs, `blind/` (which
appeared at 02:13, after my first listing at 02:12), `/panel` § 2 and § 3c. No network call was
made; prices and terms are checked only against the document the brief cites (the `claude-api`
skill, loaded locally). Line numbers below are the copy's, which is the lane's HEAD.

## 1. `00-shared.md`

- **verified** 12,801 files per copy; seed build "7.93 s" (mine 7.78); `git status --short` was
  empty of tracked changes, but the briefs themselves are untracked (`?? docs/panel/226-briefs/`
  at my start) and `/panel` § 2 says they are committed with the sitting.
- **verified** metric 3's numbers: `docs/measurements/024…md:47` reads `| **total** | 20614 | 323 |
  19286 (95%) | 16765 (83%) |`, corpus 120. **Partly verified** "metrics 2 and 4 have never run
  (`007:24-25`)": those lines say metric 2's predictions lapsed and metric 2 was scheduled; they do
  not mention metric 4. Issue 816: **could not verify** (GitHub, no network by policy).
- **verified** `--permissive` is one line, `table.hero:94`, inside `check_flags` (90 to 97);
  `build_flags` (98), `run_flags` (108), `test_flags` (116) carry no `permissive`. **verified as a
  negative** "nothing has ever built or run a permissive binary": `grep -rc permissive selfhost/`
  hits 22 files, none under `emit/`, `ir/`, nor `main.hero`, `verbs.hero`, `rounds.hero`; the hits
  are the control parse (`parse.hero`, `parse/*`, `head_*`), `diag.hero`, `cli/check*.hero`,
  `cli/table.hero`, `mutate/*`. `score.hero:147-153` is `reports(diagnostics, permissive)`;
  `judged.hero:70-76` builds `[compiler, "check", "--permissive"?, "--json", mirror/rel]`.
- **verified** 27 codes in `diag.hero:192-221` (`sed -n 192,221p | grep -oE '"[a-z_]+"' | wc -l`
  prints 27), `non_exhaustive` last; the third category's comment is lines 181 to 191; panel 192's
  R4 is at `192-…md:236`. "Added 2026-10-16, issue 2078, the author's 4a": **a document, not git**
  (`design.md:4363` says it; the copy has no history).
- **verified, command imprecise** 21 witness `.hero` under `tests/golden/permissive/`; `ls | wc -l`
  as written prints 42 (21 `.hero` + 21 `.expected`). Same shape for `tests/golden/full/`: 39
  `.expected`, but `ls | wc -l` prints 82 (39 + 39 `.hero` + 4 `.h`).
- **verified** `spoken` at `check.hero:265-273` (275 is `at_stage`'s first line); `said_at`
  (240-259) asks syntax, modules, names, types, `range(from: 1, to: 5)`.
- **false in part** "a run golden holds stdout and at most one ending line, `!panic:` or `!exit:`
  (`expectation.hero:12-17`)": line 26 documents a third ending, `!sanitizer: <needle>` (panel
  173); `tests/golden/run/*.expected` carry 129 `!panic:`, 9 `!exit:`, 40 `!sanitizer:`, and **37
  files carry both `!panic:` and `!sanitizer:`**, so two ending lines exist. **verified** no case
  carries arguments or input: `suite_run.hero:183` spawns `[binary]`, 190 `run item.source`; no
  other `!word:` ending exists.
- **verified** `hero_run_limit(seconds: WATCHDOG_SECONDS)` at `shell.hero:285`, the constant at
  751, `assert WATCHDOG_SECONDS == 600` at 1287. **caveat** "`run_limited` (162) caps bytes alone":
  it calls `spawned` (239), and 285 is inside `spawned`, so every harness spawn, `run_limited`'s
  included, is also bounded at 600 s; bytes is only the parameter it adds.
- **false citation, true claim** "`run.c:293-296`": those lines define `HERO_RUN_WATCHDOG_GRACE_MS`.
  The watchdog's `kill(-child, SIGTERM)` is line 1556, `kill(-child, SIGKILL)` 1569, `return 124`
  at 1600 (POSIX) and 1400 (Windows).
- **verified** spec 470 lines, 26,601 bytes, sha256 `212264059c985c2e0cff…`; 46 production lines;
  template 573 bytes, sha256 `b7f06aab65cd9790483d…`, one `{{SPEC}}` and one `{{TASK}}`.
- **could not verify** 10,294 / 10,292 tokens (network). What the copy says offline:
  `./heroes measure spec/heroes-spec.md` prints vendored 7708 / 7846 and `STALE: the recorded count
  is for 589b7195235b8e5d and this file is 31ee4e9b4a253a37`; the ledger's last real count is
  10,286 on `claude-opus-5` (2026-10-18, row 7846), so the brief's 10,292 is +6 on an unpinned
  spec. The brief should say the pinned count is stale and the refresh is owed.
- **verified** recovery counts 14,209 / 15,737 / 14,209 / 1,356 and 4,120 singles on `run/`.
  **verified with a caveat** "no word of `mutate --recovery` emits a mutant's text": the words
  (`mutate_recovery.hero:16-23`) are `--compiler`, `-o`, `--compare`, `--cut`, and its `print`s are
  counts and errors; but `judged.plant` (`judged.hero:62`, `write_file(at.mirror + "/" + rel,
  text)`) writes every mutant's full text into the mirror during a run and `uproot` removes it, and
  `--cut` writes `cut/dump-N.json`, the corpus's token stream (`kind`/`text`/`line`/`col`). The text
  is transient on disk, not in the records. `hunks.hero:1-26` verified.
- **verified** tools: `curl 8.7.1`; `OpenSSL 4.0.3`; `LibreSSL 3.3.6`; both `enc -help` list `-A
  -a -base64 -iter -pass -pbkdf2`; `/usr/bin/sandbox-exec` present; `claude` 2.1.292; `codex`
  absent; 77 harness modules, 21 `issue_*`.
- **verified** spec § 11 names `print`, `read_file`, `write_file`, `args`, `args_checked`, `exit`
  (lines 315 to 338); `grep -i 'stdin\|clock\|spawn\|\btime\b\|random'` over the spec prints
  nothing. `shell.hero:38` is `extern "hero_os.h"`; **four other harness modules declare their own
  group too** (`cases.hero:23`, `issue_pace.hero:31`, `issues.hero:89`, `suite_cache.hero:89`), so
  the sentence should not read as if `shell.hero` were the one. Spec § 1: `Use = "use" ident { "/"
  ident }` from the compiled file's directory, so `..` cannot be written: verified.
- **documents, labelled as such** prices and terms. Against the skill's own text (cached
  2026-10-06): the four price pairs **verified**; "cache reads about a tenth" **false for two of
  four**: the skill says 0.1× except Fable 5.1 at 0.025× ($0.25) and Opus 5.5 at 0.05× ($0.20);
  "writes 1.25×" **verified** for the 5-minute TTL (2× for 1-hour); thinking rules, effort defaults
  and `temperature` **verified**; Haiku 5.5's prices hold for prompts up to 100K tokens ($0.50 /
  $2.50 beyond), unsaid. "Training cutoff June 2026 by the skill's statement": **could not verify**,
  no cutoff sentence in `SKILL.md` or `shared/models.md`; the Models API exposes `created_at`, no
  cutoff field. Terms: the brief labels them "to be verified by the historian": correct.
- **could not verify** the blind reading's 23 / 18 / 25: `grep -ri confound docs/` hits no file of
  this milestone; the page § Opened carries the points taken, not the counts. Name where they live.
- **verified** R8's arithmetic: exact McNemar two-sided p = 0.0227 at 18 vs 6 (24 discordant),
  0.0652 at 24 vs 12 (36); design effect 1 + 2 × 0.5 = 2, 120 / 2 = 60; 10 pp of 120 = 12.
- **verified** R13: panel 007 keeps two baseline predictions (`007-…md:80-86`, the ergonomist's
  ≥ 25 pp and the warden's zero depth-0 breaks); spec lines 11 to 19 state the continuation rule;
  panel 133's table rows 3 and 4 are the ergonomist's, "the Part 11 experiment"; `2409.19151` is in
  133. Issue 35's `compiled_wrong`: **could not verify**, the word is nowhere in the tree.
- **verified** ROADMAP row 70 `OPEN`; the milestone page § Opened (line 73). **drift between page
  and brief**: the page cites `check.hero:266-275` and `shell.hero:262`; the brief 265-275 and 285;
  285 is right, 262 is a comment inside `spawned`.
- **contradiction** "the budget of the one paid run (the llm-ergonomist's blind session) is in its
  brief and nowhere else": `ffi-pragmatist.md` item 5 authorises a second paid call
  (`--max-budget-usd 0.05`). The skill allows a named, bounded run; the "nowhere else" is false.

## 2. `compiler-engineer.md`

- **verified** `table.hero:90-97`; `check.hero:40-41` (`said @ parsed.control` under
  `--permissive`); `parse.hero:32-39`; `kept_of` 283-291 (281-282 its comment); `compiling`
  `main.hero:267-290`; `cli/rounds.hero` exists; stage 2 "about a minute" read 79.6 s here.
- **verified** `tests/golden/permissive/the-control-arm-reads-a-match-missing-a-case.hero` exists,
  a `Shape` with `.dot` unmatched and a `type_mismatch` witness. `panic.c:39-43` prints `panic: %s`
  to stderr then `abort()` (134); `hero_unreachable` is 45-47 **and its message says "this is a
  compiler bug, please report it"**: a permissive missing-case abort must not reuse it. **cannot
  run** `git diff --stat` in the copy (no `.git`); say `diff -ru` against a second archive, or
  `wc -l`.
- **verified** `check_json.hero:1-17` (schema 2 to stderr); test "holes stay exit 0" at
  `check.hero:295`; `run_test` is `verbs.hero:112-172` and names no limit; the only `selfhost/`
  callers of `hero_run_limit` are `cli/value_run.hero:267` (the value probe) and
  `mutate/child.hero:63`, so "no per-test time limit" holds by grep. `run.c:238-296` is the comment
  on groups and forwarded signals: right place for item 4's question.
- **does not exist** `shell.run_bounded(seconds, bytes, in_path)`: no match in `shell.hero`; the
  milestone page (line 127) says the grader *needs* one. Write it as the function to design, against
  `spawned(scratch, program, words, in_path, write_limit)` at 239 (longer than 239-262).
- **verified** `ISSUE_PROGRAM` at `main.hero:645`, its test 777 to 805; `implementation_cost` is a
  field of the seat's definition (`compiler-engineer.md:39`).

## 3. `ffi-pragmatist.md`

- **verified** `refresh.hero:59-83` (comment 59-72, `asked` from 73, curl words with `--variable
  %KEY --expand-header`). **false** "`reply_of`, line 193": `reply_of` is `issue_wire.hero:207`; 193
  is inside `answer`. Nothing in that file handles `100 Continue` (grep empty), which is the reason
  for `-H "Expect:"`, so the question stands.
- **already answered** "is `-iter` accepted by both?": yes, both `enc -help` list it. Keep the
  cross-decrypt and `ps` questions.
- **could not verify** "the Windows box is on". `platforms.md` § The Windows box says only the
  author starts it and work reaches it by `git push win main:main` or a tar; it names no shell route,
  and the seat's copy has no `.git`, so `git push win` is impossible from it. Give a host and a
  command, or mark the item unrun in advance.
- **verified** `hero_os.h:359-362`; `claude --help` lists `--bare`, `--disallowedTools`,
  `--no-session-persistence`, `--max-budget-usd`; `python3` 3.15 and `nc` present. The paid call
  contradicts `00-shared`'s "nowhere else" (§ 1).

## 4. `spec-warden.md`

- **verified** `guard_bash.py:44` `COUNT_TOKENS`, refusal at 907-910 of any other follower of the
  host for a client command ("a tripwire, not a sandbox", its own words); the template says "Single
  turn. Context = this template + spec + ONE task"; 46 productions; the template's sha.
- **wrong section, paraphrased** "§1.2's sentence that the factor is *no longer audited* (panel
  011, item 6)": the sentence is `design.md:385-387`, inside **§1.6** (253 to 419; §1.2 is 190 to
  205) and reads "stops being an audited one (panel 011)"; "no longer audited" is panel 011 item
  6's wording (`011-…md:98`). §1.2 itself (193) carries the formula and no audit sentence.
- **verified** §1.6's "spend at most **1,500** real tokens before the thesis harness has run"
  (291-293); §1.1 at 174; `spec_token_delta` is a field of the definition.

## 5. `historian.md`

- **verified** `2409.19151` is in panel 133. Every other line is a web claim the seat is asked to
  verify, and its tools (`WebSearch`, `WebFetch`, `Read`, `Write`) reach them. It is told it needs
  no copy of the tree; item 1's "already in panel 133" it cannot re-read without one, harmless.

## 6. `blind/` (the llm-ergonomist)

- **verified** `README.md`: the spec copy's sha equals the tree's; the two feedback programs exist
  under `tests/golden/full/`; `--max-budget-usd 4` is the one paid run's budget, written here.
- **verified** `one/brief.txt` and `one/full.txt` are byte for byte what my compiler prints on the
  renamed program. **false** `two/full.txt`: my compiler prints a 4-space gutter (`    |`, `  4 |`)
  where the file keeps the original line 10's 5-space gutter, and `fix (guess) … (at main.hero:2:6)`
  where the file says `2:5` (the tree's `.expected` says `8:6`). The file was renumbered by hand and
  drifted; regenerate it with `heroes check main.hero`. `two/brief.txt` is identical.
- **mismatch** `brief.md` says "read `program.hero`"; the files are `program.hero.txt`.
- **question** `README.md` is written in the past tense ("the seat ran") and names `/panel`,
  `tests/golden/full/`, `heroes check`; if it sits in the handed folder, `brief.md`'s "your inputs
  are the files in this directory" makes the seat read it, and its `context` answer is tainted.
  Keep it out of the seat's folder, or say it was added afterwards.
- **drift** the field `hesitation_points` follows `.claude/agents/llm-ergonomist.md:47`, while
  `/panel` § 2 (line 182) says the brief asks for `choice_points` and that the "hesitation" wording
  was once stopped by the API's safeguards. The brief's prose asks for choice points; its field
  name does not. Align the definition and the skill, then the brief.
- **verified** no project name leaked into `brief.md` or the tasks (grep for `design.md`,
  `CLAUDE.md`, `selfhost`, `panel`, `CL-0`, `docs/`, `permissive`, `thesis`: only `README.md` hits).

## 7. Routes R1 to R13 leave out, and questions a seat's tools cannot answer

- **R1**: a control that keeps the 27 diagnostics as *warnings the model sees* and still builds is a
  different control from one that drops them in silence; the proposal takes "drop" without naming
  the other. Both measure the rules' value; only the second also measures the message's.
- **R5**: the non-compiler buckets lack the API's own `stop_reason: "refusal"` (HTTP 200, a
  classifier; "refused" is overloaded with the compiler's refusal) and a transport bucket (429, 529,
  SDK retries, a timed-out request). Pre-register both, and whether a sample is retried or excluded.
- **R7**: the three arms fork one turn-1 transcript carrying thinking blocks; the skill says a
  block's signature records the conversation prefix and that an edited prefix is a 400 on accounts
  created on or after 2026-08-31, or a silent `drop_block`. A fork keeps the prefix byte-identical,
  so it should pass, but nothing in the briefs says so or asks the trial to prove it: set
  `thinking.block_binding.prefix_mismatch_behavior: "error"` under `thinking-binding-controls-2026-08-01`
  on the trial, or two arms may silently run without turn 1's thinking.
- **R2**: say whether the primary is one- or two-sided, and name the mixed-effects (task random
  effect) logistic model as the alternative to McNemar plus a cluster bootstrap.
- **R3**: correct stdout with a non-zero exit, correct stdout with stderr noise, and a run-time
  timeout each need a class before the run; `!sanitizer:` shows the expectation format already has
  a third ending the grader must decide on.
- **R8**: Haiku 5.5's price tier above 100K-token prompts and the 5-minute cache TTL (samples that
  share a prefix must start within it) belong in the spec-warden's arithmetic.
- **Which binary**: the seed-built compiler and stage 2 carry different `sources` keys; `00-shared`
  says stage 2 judges, the spec-warden's and ffi-pragmatist's briefs say only "build from the seed".
  A harness run with the seed-built compiler in a copy was not refused by the hook (exit 2, `no
  suite named nosuchsuite`), so nothing stops a seat from judging with the wrong one.
- **Tools**: compiler-engineer's `git diff --stat` (§ 2); ffi-pragmatist's Windows box (§ 3); the
  rest of the questions are within their seats' tools.

## 8. Repairs for the coordinator, before the seats start

1. `00-shared`: three endings in a run golden, 37 cases with two; `run.c` kill lines 1556, 1569,
   1600; `run_limited` is time-bounded too; cache reads 0.05× on Opus 5.5 and 0.025× on Fable 5.1;
   drop "by the skill's statement" on the cutoff or cite where it is; name where the blind
   reading's counts live; say the pinned spec count is stale (10,286 pinned, 10,292 today, +6);
   `007:24-25` speaks of metric 2 alone; four harness modules spawn, not one; strike "nowhere else".
2. `compiler-engineer`: `run_bounded` is to be designed, not read; `hero_unreachable`'s message is
   the wrong one for a permissive abort; `diff -ru` instead of `git diff --stat`.
3. `ffi-pragmatist`: `reply_of` is line 207; `-iter` is answered; give the Windows box a route or
   mark the item unrun; reconcile the five-cent call with `00-shared`.
4. `spec-warden`: the audit sentence is §1.6, `design.md:385-387`, "stops being an audited one".
5. `blind/`: regenerate `two/full.txt` with the compiler; `program.hero` → `program.hero.txt`;
   keep `README.md` out of the handed folder; `choice_points` once the definition and skill agree.
6. Add R5's two buckets, R7's prefix check, R3's three cases and R1's warning-control to the
   question list, and tell every compiling seat which binary judges (stage 2) and why the keys
   differ.
7. Commit `docs/panel/226-briefs/` with the sitting (it is untracked), the repaired text included.
