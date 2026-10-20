# Panel 226, the completeness critic's second pass: what the five reports leave out

Written 2026-10-20, as it went, after the five seats' reports and before the synthesis was written
into the tree. Brief: `docs/panel/226-briefs/critic-second.md`. Copy: `<scratchpad>/226-critic/`,
the first pass's `git archive` of `a0a8d1249`, judged with its stage 2 compiler `./heroes-next`
(`sources 7792ecfe50a531e8`). No paid run, no network call, no credential printed; nothing built or
run in another seat's folder. Taken as read, per the coordinator's note: the ffi-pragmatist's
report is complete (`object`, no veto, the `sysctl(KERN_PROCARGS2)` hole), and R1 was built in
the coordinator's copy during the sitting. No verdict here (`/panel` § 3b).

Every number below is one a command of mine printed, with the command beside it, or is marked
*unrun* in my own words.


**What I ran, once, so the sections below can cite it.** Copy at `a0a8d1249`, stage 2 `./heroes-next`
(`sources 7792ecfe50a531e8`), work files in `<scratchpad>/226-critic-work/`; the coordinator's two
evidence tars, `steps-2-to-9.tar` (10:16) and `step1-build-permissive.tar` (10:03), read from
`thesis-drafts/coordinator-work/` and extracted into my scratchpad, never into the lane. Read, not
run: the five reports, the six briefs, my first pass and the draft at 10:27.

## 1. A route nobody listed

- **R1.** Three. (i) **The rule that assigns a row to *silent* or *loud* is not written.** The table
  gives 25 codes a silent meaning, `non_exhaustive`/`needs_wildcard` an abort and
  `endless_recursion` a stack panic; nobody named the principle (*silent wherever the permissive
  language named is silent; loud only where no silent meaning exists*, or its opposite). The choice
  moves a control sample between R3's classes 2 and 3 **and changes the control arm's turn-2
  feedback** (a `panic:` line against observed-against-expected), so it moves `green@3`(control) and
  the primary: the critic brief's first warning, a control whose definition decides the result. The
  unlisted route is the uniform one, a *dynamic-check control* in which every dropped rule becomes a
  run-time panic at its point of violation; it would be better only if the thesis were about the
  TIME of refusal (compile against run) rather than refusal against none, so I expect the sitting to
  keep the mixed table, but with the rule written in its header. (ii) **A fourth arm, strict with the
  feedback *it did not compile* and no message**: the historian's condition (1) (report line 459)
  presupposes that comparison and no arm measures it; brief minus silent is the one-liner's value,
  silent minus control the bare refusal's. (iii) My first pass's warning-control (the 27 diagnostics
  shown as warnings, the binary built) is in `00-shared.md` R1 as *the critic's route* and the
  draft's R1 neither takes it nor says why not.
- **R2.** Three. (i) **A primary on the silent class.** The proposal's own sentence says *the thesis
  is measured on the third* class and the primary measures convergence; nobody named a primary of
  the shape *share of samples whose last turn is `compiled_wrong`, strict minus control*, nor the
  turn-1 one, *of the programs the control runs silent-wrong, the share strict refuses*, which is
  deterministic given the shared generation (no model variance past turn 1). Better if the thesis is
  design.md §1.12's (a wrong program refused rather than run) rather than *the model converges
  faster*; the draft should say which and why `green@3` carries it. (ii) **The smallest effect of
  interest**: the three outcomes turn on a threshold of 0 (draft lines 370 to 372), so a 1 pp
  difference with a tight interval *confirms*; the brief's own orientation is 10 pp. Name the margin
  or say 0 is deliberate. (iii) **Lazy forking.** `thesis_run.hero:5-9` (steps tar) forks every arm
  at turn 2 whatever the turn-1 outcome, so where turn 1 compiled and failed the example all three
  arms receive an identical prompt and three independent samples: a discordant pair can be pure
  sampling noise. Forking at the first turn where the arms' prompts differ (full against brief at
  the first refusal; strict against control at the first program strict refuses and control runs)
  makes every discordant pair the arms' doing, and costs one request where today three are sent.
  It is better exactly because the test is McNemar on discordant pairs; nothing in R2, R5 or R7
  names it.
- **R9.** One. **Route C: the `harness` transport under API-key authentication.** The
  ffi-pragmatist read in `claude --help` that under `--bare` the credential is `ANTHROPIC_API_KEY`
  or `apiKeyHelper` alone (report lines 297 to 299); the historian verified that Codex takes
  `CODEX_API_KEY` for a single run (line 359) and that API data falls under the Commercial Terms
  (lines 327 to 334). A `claude -p --bare` or `codex exec` run with an API key is API-billed, under
  terms that document no training, with no consumer automated-access clause in the way, so the
  `harness` rows could run on held-out tasks and measure the scaffold against the frozen prompt. It
  is better if the author's question is about the harness's scaffolding and not about the plan's
  quota; the draft's R9 refuses B for the terms and never asks whether the transport can be moved
  under the terms it accepts. Unrun: whether 2.1.292 answers at all under `--bare` on a non-tty
  (the hang's cause, the ffi-pragmatist's own question).
- **R10.** Three. (i) **One key per task.** R6 says *a per-task GUID completed exactly = that task
  burned, replaced from the reserve*, and R10 seals forty tasks under one key: a burned task cannot
  be published without opening the other thirty-nine. A per-task key (one archive member per task,
  or a key derived from the master and the task's number) lets the burned task open alone; nobody
  named it. (ii) **A commitment to the key now**: the sha256 of the key committed at the freeze
  beside the ciphertext's, so the key committed at the series' close is provably the one that
  existed, and *re-encrypted later* is excluded by a line, not by trust. (iii) For the second
  question, between the Actions secret and the key kept local: **a signed attestation** (a git-signed
  tag over `hashes.txt` and the counts) that a stranger can verify without the key leaving the Mac.
  It satisfies the author's two reasons only in part (a signature is a claim, a CI run is a run), so
  the sitting's choice stands, but the route exists and the draft says it does not.

## 2. A claim asserted and not measured

By file and line; *holds*, *measured here* or *unrun* is my own finding.

- **compiler-engineer.md:185-190**, the harness's limit reaching a test binary *by one hop of
  forwarding, within 2 s (unrun)*. **Measured here**: `spin.hero` with a `test` that loops for ever,
  `./heroes-next test spin.hero` started in its own session by `python3 -I forward2.py`; the test
  binary `build/479e995e767ef492/spin 0` ran at 99.5% CPU in a process group of its own (pid 46910,
  pgid 46910), its parent `heroes-next` (pid 46901, pgid 46901); `os.killpg(46901, SIGTERM)`, the
  watchdog's `kill(-child, SIGTERM)`, and the test binary was **gone in 0.0 s**, the parent exiting
  −15, nothing left for the cleanup to kill. The reading holds and is faster than its grace. The
  draft's line 81 to 82 says *by reading, unrun*: it can say measured.
- **compiler-engineer.md:79** (row 10), `bound_unit`'s cost *unrun how many* through `mono`.
  **Measured in the step-1 tar**: `diff -u` of its 26 `selfhost/` and 4 `runtime/` files against my
  copy, lines opening `+`/`-` and not `++`/`--`: **+504 / −132 over 30 files**; `ir/flatten.hero`
  +129/−24, `emit/guarded_names.hero` +54/−51, `resolve/state.hero` +46/−4, `emit/abort.hero` +46,
  `cli/compile.hero` +34/−17, `runtime/parts/panic.c` +31, `ir/lower.hero` +24/−1, the rest under
  16 each; **no file under `mono` is in the tar** (`find selfhost -path '*mono*'` prints nothing),
  and the hunk's own comment reads *a slot of `()` is one `mono` refuses, so the call ran and
  nothing* [is stored]. So `bound_unit` as built costs about five lines of `ir/flatten.hero` and no
  `mono` line; § 3 says what that does to R1's words.
- **compiler-engineer.md:155-160**, *identical, 27,890,840 bytes*. The `cmp` is a measurement; the
  size is a question. **Measured here**: `./heroes-next build selfhost/main.hero --emit-c` at
  `a0a8d1249` writes **26,694,926 bytes** to stdout (`real 50.69 user 47.21`); the draft's line
  219 says the coordinator's patched stage 2 emits 26,907,438 (+212,512 for +504/−132 lines). The
  compiler-engineer's is 1,195,914 above the base for +79/−39 lines, and the 57,144 `#line`
  directives name `"main.c"` and relative `selfhost/` paths (`grep '^#line' | awk | uniq -c`), so a
  copy's path cannot explain it. Which emission that was is a question for the seat; the fixpoint
  claim stands on `cmp` either way.
- **historian.md**, the `verified in part` lines (10 marks; `grep -c 'verified in part'`): 75-82
  SWE-bench Verified (the 93 annotators, 1,699 screened and the discard fraction `unverified`);
  138-149 the canary hit (one comment, `unverified` as a measurement); 230-235 Wilson's text;
  260-267 Hanley's text; 335-342 and 343-349 OpenAI's two help pages (search snippets); 368-380
  OpenAI's Terms (a 2023 archive and a snippet, the current text `unverified`). Two reach the draft
  as if whole: **R8's prior-art line** *Reached first by SWE-bench Verified (... the discard rate to
  size a reserve for)* rests on figures the historian marked `unverified`, and
  `.claude/rules/records.md` § And the record says whose idea it was admits only a `verified`
  precedent; and **lines 127-130 and 302-303**, *both providers' consumer terms forbid automated
  access outside the API*, hold for Anthropic's (§ 3, `verified`) and for OpenAI's only on the 2023
  archived wording. The `codex exec` page R9 cites is `verified`, so R9's line stands; the sentence
  about OpenAI's consumer terms should carry *verified in part*.
- **historian.md:384-400**, the cutoff. The models overview is `verified`; *no listed model's
  training data can hold the public repository* is the historian's inference from the provider's
  declaration, and it names its own unrun half: whether any Heroes text was public before
  2026-09-08 by another route (the site's first publication). The draft's R6 (lines 412 to 415)
  carries the conclusion without either caveat. Unrun here (`git tag --list 'site-v*'
  --sort=creatordate` in the lane would date the site's first publication; my copy has no `.git`).
- **llm-ergonomist.md:308-312**, 45/30, 50/30, 70/60: predictions, labelled so in the report and
  the draft, their role. Its **25 choice points** (draft line 157): **measured here**, 25 numbered
  items under `## choice_points` before `### Prose`, 9, 11 and 5 by task (`awk`). The header's *24
  turns, 218 s, 0.78 USD* (lines 8 to 9) is the coordinator's reading of `run.json` in that seat's
  folder: unrun here, by the brief.
- **The draft's § What was measured**: *exit 153* (line 514) **holds**: `/bin/sh -c 'ulimit -f
  4096; head -c 10000000 /dev/zero > big.bin'` printed `Filesize limit exceeded: 25`, exit 153, the
  file 4,194,304 bytes. *Invalid argument* (line 516) **holds** for `-v`, `-d` and `-m` from
  `/bin/sh` (*cannot modify limit: Invalid argument*, three times); `/bin/zsh` says *setrlimit
  failed: invalid argument* for `-v` and `-d` and *bad option* for `-m`. *39 tests* (line 194)
  **measured here**: the steps tar's `tests/` overlaid on my copy, `./heroes-next test
  tests/harness/thesis_sandbox.hero`, **39 tests, all passed**, `real 29.86`. *92 tests* (line 167):
  **92 tests, 7 failed** here (`real 35.37`), every one of the seven about the template (*the frozen
  template renders in both layouts*, *the feedback blocks are the template's four ... a template
  missing one is refused*, *the canary context*, *the feedback is the template's block*, *a dry run
  ... counts one request*, *a live run on the scripted transport*, *a run's provenance names the
  template and the layout apart*): the amended `docs/metrics/first-try.md` is in neither tar
  (`ls steps/docs/metrics` prints nothing) and my copy holds the frozen one, `b7f06aab…` with one
  `---` line; so the 92 stand only with a file the evidence does not carry. *4,529 lines, 16
  modules* (line 166): **16** `thesis*.hero` in the tar, **4,547** lines by `wc -l`, a drift of 18
  between the tar at 10:16 and the draft at 10:27. *445 KB*, *4 canaries* (lines 186-187): **unrun
  here** (a `seal` under a throwaway key would settle them; the 4 is `LABEL`'s line plus three
  `task.md` by `thesis_seal.hero:166-188`, read). *26 cases* (line 225): **26** `.hero` under
  `tests/golden/control/` in the step-1 tar, their run unrun here, my stage 2 having no `build
  --permissive`. *20,650* (line 175) says *estimated*. *703, 22, 40, 30, 6, 1738, 414* (lines 216
  to 229) and the three timings (231) are the coordinator's in its copy, unrun here.
- **ffi-pragmatist.md:297-303**: the cause of the two-hour hang is the seat's own *question,
  unrun*, and the draft's line 305 reads it as the seat *agreeing from the other side* with the
  historian's terms: § 3 below.

## 3. Contradictions between seats, and which is checkable

1. **`bound_unit`.** The compiler-engineer: its lines through `mono` *unrun*. The draft's R1 (lines
   340 to 342): *the binding holds `()`, its slot carried through `mono` and the emitter, its cost
   measured in the landing step*. The draft's own § What was measured (lines 204 to 205): *`x =
   greet()` binding nothing (`hi`, `1`)*. **Checked in the step-1 tar** (§ 2): what was built is
   *the call runs and nothing is bound*, `mono` untouched, and the cost is in the tar today, not in
   a landing step. R1's sentence describes a route that was not built. The same section says *the
   module is split at a named seam ... never raised in silence* (lines 345 to 348) while § What was
   measured (220 to 224) raised three `DECIDED` rows and added two with their measures: built is
   *raised with the measure written*, not *split*; checkable by `diff` of the tar's
   `tests/harness/suite_layout.hero` against the tree's, not run here. The tar is what is right about
   the tar; R1's words should match it, or say the split is owed at the landing.
2. **`claude -p` and the terms.** The historian (lines 299 to 309, 357 to 364): Anthropic's § 3
   forbids automated access *except via an API key or where we otherwise explicitly permit it*;
   OpenAI's `codex exec` page says *do not use this workflow for public or open-source repositories*.
   The ffi-pragmatist's report **does not say under which authentication its probe ran**; it reads
   from `--help` that `--bare` takes `ANTHROPIC_API_KEY` or `apiKeyHelper` alone and infers the
   child had **no credential** with `HOME` on an empty folder, a login wait being *the most plausible
   cause* of the hang (lines 297 to 302). So its probe was most likely no use of the consumer
   service at all, and nothing about the terms follows from it; the draft's line 305 (*agrees from
   the other side: its own probe hung two hours*) joins two unrelated facts. The sitting's one
   completed `claude -p` run is the llm-ergonomist's, stopped by *You've hit your session limit,
   resets 4:30am Europe/Rome* (its report, lines 9 to 11): that message's shape is the
   subscription's window, not the API's 429, so the blind seat ran as automated use of the consumer
   service on the assistant's tasks, the very use R9 says is undocumented as permitted (it allows
   it on the assistant's tasks without saying why that side of the line is different). Checkable by
   `run.json` in that seat's folder or by the account's auth status; **unrun here**, the brief
   forbidding both the folder and any `claude` call. Which is right: the historian on the terms; the
   draft on neither sentence as written.
3. **The spec's count.** The spec-warden (lines 66 to 74, 104 to 106): wrapper 6 on `claude-opus-5`
   and 8 on the four live ids, content 10,286 on all five; the brief: 10,294 on four, 10,292 on
   one. **No contradiction**: 10,294 − 8 = 10,286 = 10,292 − 6, and the pinned `REAL_TOKENS` is the
   wrapper-exclusive number. Checked offline here: the seed-built `./heroes measure
   spec/heroes-spec.md` ends *No verdict is given against a stale count* and `./heroes-next measure`
   prints *Headroom: 6098 against the 16384 ceiling ... 10346*, so `STALE` was the seed's reading and
   the brief's *+6 on a spec that moved* was the false sentence, which the draft's § Process notes
   already corrects. The count itself is `count_tokens`', unrun here (no network by the brief).
4. **Found beside them: the denominator.** R5 (lines 403 to 405): `api_refusal` and
   `transport_error` *outside the denominator*. The instrument in the steps tar:
   `thesis_report.hero:117` `n = g.verdicts.len()`, the last outcome `api_refusal` falling into
   `other` at line 141 and **inside `n`**, the Wilson interval at line 149 over that `n`;
   `api_refusal` is a turn outcome at `thesis_run.hero:290`; and **`transport_error` is in no
   thesis module** (`grep transport_error tests/harness/thesis*.hero`: nothing): `thesis_pace.hero:46,54`
   fail with code `transport` past the retries, and `thesis_run.hero:316` propagates it with `?`.
   Which is right: R5 is the rule; the instrument does not yet do it. Checked by reading the tar.
5. **Found beside them: the digests.** Lines 187 to 188, *`tasks --sealed` read the opened copy with
   the same three task digests and suite digest as the clear copy*; `thesis_seal.hero:181-188` writes
   `canary heroes-task: <hex>` as the last line of every `task.md` of the suite it seals, and
   `thesis_tasks.hero:16` digests `task.md` whole as *what the model sees*. **Measured here**: `tasks
   tests/thesis` on the tar's suite prints task 01 `db7ef756…` and the suite `077bc0b8…` (its `LABEL`
   already carrying a suite canary, the line ending `canary`); one canary line appended to 01's
   `task.md` in a copy prints `bae233ff…` and suite `03808c4e…`. Both sentences are true only if *the
   clear copy* already held the canaries; say so, and compute `hashes.txt` after the canaries are
   written and never before.
6. **Found beside them: the three emission sizes** (§ 2, the compiler-engineer's line 159 against
   the draft's 219 and my base): a question to the compiler-engineer, not a finding.

## 4. The question the sitting should have asked and did not

Weighed, with the brief's five candidates first:

- **The trial's judge-only control leaking into the held-out pre-registration: yes, and this is the
  one I would put to a sixth seat.** `thesis_grade.hero:21-25` (steps tar) says it in the
  instrument's own words: *today the control arm judges and cannot build: a program it accepts is
  built strict, so a control verdict past `check` is biased towards the strict compiler's refusals
  at build*. So in the trial a program `check --permissive` accepts and strict refuses becomes
  `build_failed`, which R8 (lines 449 to 450) **excludes and refiles as a compiler defect**. That
  cell, *control runs it, strict refuses it*, is the discordant cell the thesis bites in, and the
  trial cannot produce it; yet R8 (436 to 438) takes **n from the trial's measured discordance and
  intra-task correlation**, and R2 writes the inconclusive rule, the difficulty target and
  `max_tokens` from the same trial. The question: *which pre-registered quantities may be read from
  a trial whose control did not build, and which must wait?* My reading, for the coordinator to
  take or leave: format leakage, the token and output distributions, `max_tokens`, pacing and the
  forks' proof may be read now; the discordance, n and the inconclusive rule wait for `build
  --permissive`, or are read from strict against brief as a floor and said to be one; and the
  trial's `build_failed` must be bucketed as *strict refused at build* and never filed as a defect.
- **Whether a model's program can detect the sandbox**: answered by the ffi-pragmatist (a no-`extern`
  program sees it only as a refused read, which costs it the hidden cases); rejected as asked.
- **The per-task canary line in `task.md`**: not noise, a cue. § 3 item 5 measured that the line is
  in the prose the model reads. *canary* is the word BIG-bench's sentence made a known marker of
  *you are being evaluated*, and the historian's BrowseComp precedent (lines 168 to 188) is eval
  awareness changing a model's behaviour. Under route A nothing trains on the prompt, so a GUID in
  the prompt detects nothing the opened repository's GUID would not; strip it from what is sent (a
  `CANARY` file beside `task.md`, the prose digest over the prose) or say why it stays. And the
  draft uses *canary* for two things, the `none` context (line 36, `thesis_prompt.hero:23`) and the
  GUIDs: name them apart. The second question I would ask.
- **`green@3`'s denominator**: answered in words by R5 and R8 and not by the instrument (§ 3 item
  4). One thing to add: a transport loss correlates with transcript length, which correlates with
  the arm, so *excluded from every arm* (R8) is the right rule and *not green* would bias against the
  arm with more turns; keep R8's rule and build it.
- **The `brief` arm's feedback on a program that compiles and fails the example**: the same text as
  the strict arm's, by R5. That does **not** make the two arms differ at turn 1 alone: turn 1 is one
  shared generation, and the arms differ at **every turn whose program is refused** (the full against
  the one-line message) and coincide at every turn whose program compiles, so the first difference
  can fall at any turn. What follows: the full-against-brief contrast is diluted by the share of
  conversations with no refusal at all, and should be reported over the pairs holding at least one
  refusal as well as over all; and lazy forking (§ 1, R2) is what keeps the coinciding turns from
  adding noise to it.

## 5. Repairs asked of the draft, one line each, by its section

- **§ What was measured, the compiler-engineer bullet (lines 81-82)**: replace *by reading, unrun*
  with the measurement above (the test binary gone in 0.0 s after the group SIGTERM).
- **§ What was measured, the coordinator bullet (166-167)**: re-count the lines at the landing (the
  tar says 4,547) and say the 92 pass only with the amended template, which must travel with the
  modules (the evidence tars carry no `docs/metrics/first-try.md`).
- **§ What was measured (187-188)**: *the same digests as the clear copy* only where the clear copy
  already carries the canaries; `hashes.txt` computed after `seal` writes them.
- **§ What was measured (219) and the compiler-engineer's 159**: ask the seat which emission was
  27,890,840 bytes; the base is 26,694,926 and the paths are relative.
- **§ Disagreements (301-306)**: strike *agrees from the other side*; the probe's hang has an unrun
  cause (most plausibly no credential under `--bare`), and the one subscription-authenticated run of
  the sitting is the blind seat's.
- **R1 (340-342)**: write `bound_unit` as built, *the call runs and nothing is bound, no `mono`
  line*, and the cost as the tar has it (+504/−132 over 30 files whole, `flatten` +129/−24); strike
  *measured in the landing step*.
- **R1 (345-348)**: say *raised with the measure written in `DECIDED`* where that is what was built,
  or say the split at a named seam is owed at the landing; not both.
- **R1 (table header)**: write the rule that assigns a row to a silent meaning or an abort, and say
  that it moves the control's turn-2 feedback.
- **R1 (closing)**: name the warning-control and the fourth *it did not compile* arm as routes not
  taken, with the reason.
- **R2 (365-372)**: name the smallest effect of interest or say the threshold 0 is deliberate; say
  why the primary is `green@3` and not the silent class the proposal's sentence names; add the
  turn-1 deterministic reading as an exploratory row.
- **R2 / R5 / R7**: fork at the first turn where the arms' prompts differ, not at turn 2 by rule;
  say what it saves and that the pairs become the arms' own.
- **R4**: report full-against-brief over the pairs holding at least one refusal beside the whole.
- **R5 (403-405)**: the two buckets exist in the instrument's vocabulary and outside `n`
  (`thesis_report.hero:117,141`), and `transport_error` is named where `thesis_pace`'s `transport`
  failure lands.
- **R6 (411-415)**: *by the provider's declaration*, and the site's first publication date read
  from `site-v*` tags before *never saw Heroes* is written.
- **R6 (417-422)**: the per-task GUID out of the prose sent to the model, or the reason it stays;
  *canary* for the GUIDs and another word for the `none` context.
- **R8 (436-438, 449-450)**: n and the inconclusive rule not from a judge-only trial; the trial's
  `build_failed` bucketed as *strict refused at build*, never filed as a compiler defect.
- **R8 (prior art)**: SWE-bench Verified is `verified in part`; keep the line only on what the
  historian marked `verified` (the subset exists, human-validated), drop *the discard rate*.
- **R9 (454-461)**: name route C, the `harness` transport under an API key, and say whether it is
  taken; say why the blind seat's subscription run on the assistant's tasks is on the permitted
  side of the line R9 draws, or stop drawing it there.
- **R9 (prior art) and § What was measured (127-130)**: OpenAI's consumer terms *verified in part*
  (a 2023 archive and a snippet); the `codex exec` page carries the line alone.
- **R10 (474-481)**: one key per task, or say why one key for forty is kept against R6's own
  replacement rule; a sha256 of the key committed at the freeze beside the ciphertext's.
- **R10 (482-489)**: name the signed-attestation route between the secret and the local run, and
  why the secret is chosen over it.
- **§ Predictions to score**: add the two things this pass measured and the draft can score at the
  landing, the 92 and the digests after `seal`.

The file is written: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/docs/panel/226-reports/critic.md`.
