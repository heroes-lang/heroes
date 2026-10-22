# Panel 226: metrics 2 and 4 measured by one instrument, three arms forked from one generation, the control arm built and run, and the held-out tasks sealed in the tree

**Convened 2026-10-20 at the opening of M-thesis-harness (row 70), synthesised the same day**, on
issue 2079 (https://github.com/heroes-lang/heroes/issues/2079, filed 2026-10-14 at the chain's
re-ordering, which asks for a sitting on design.md Part 11 before the instrument is built), by the
author's answers of 2026-10-19 in the session that planned the milestone, meant as *decide the open
points first, then the panel, and the panel may overturn*, and by the author's instruction of
2026-10-20, meant as *start the loop and run until the step is done*, so that every open point took
its recommended answer (CL-002) and this sitting is what may overturn it. **Lane: full panel**, the
five seats and a completeness critic before them and after them: the proposal amends Part 11, a
part of the design, and the llm-ergonomist's blind experiment is the one measurement no other seat
can make. **Resolution: provisional, author ratification pending.** Briefs:
`docs/panel/226-briefs/`, repaired after the critic's first pass
(`docs/panel/226-reports/critic-briefs.md`). Reports: `docs/panel/226-reports/compiler-engineer.md`,
`ffi-pragmatist.md`, `spec-warden.md`, `historian.md`, `llm-ergonomist.md`, `critic.md`, and the
coordinator's measurements below, each number with the command that produced it in the report that
holds it. Evidence: `docs/panel/226-reports/evidence.tar.xz` and `evidence.md` beside it. **The
ratification is queued** as the open decision https://github.com/heroes-lang/heroes/issues/2159,
`Panel 226:`, its `## Prior art` the eighteen precedents below, each re-read on 2026-10-20.

## The proposal, verbatim (the shared brief's diff to design.md Part 11)

> **2. First-try rate and 4. Turns-to-green, one instrument.** Spec → model → program → graded by
> black-box hidden cases (arguments and input files in, stdout and exit code out). One generation
> per (task, sample), judged by three arms: **strict** (`check`, the full message), **strict-brief**
> (`check --brief`) and **control** (`check --permissive`, built and run with `build --permissive`,
> each of the 27 dropped rules given a runtime meaning pinned in a table). From turn 2 the
> conversations fork from that generation, append-only, with the same feedback in every arm: the
> compiler's message on a refusal; the visible example's observed-against-expected on a program that
> compiles; the hidden cases never. Capped at 5 turns; non-convergence counted. Three failure
> classes: refused at compile time, failed loudly at run time (a panic), **silent** (exit 0, wrong
> output); the thesis is measured on the third. **One primary, pre-registered at the freeze**:
> `green@3`(strict) − `green@3`(control) on Sonnet 5.5 in the `spec` context, a cluster bootstrap over
> tasks and McNemar on the discordant pairs, three outcomes named before the run (confirmed,
> falsified, inconclusive); everything else exploratory. `green@k` is *solved by turn k*, not Chen
> et al.'s pass@k, which is reported apart on the turn-1 samples. n from the trial's measured
> discordance and intra-task correlation; 40 author-written held-out tasks plus 20 in reserve, 3
> samples each, 10 assistant-written tasks in clear for the trial. Contexts: `spec`, `none` (the
> canary), variants by path. Models: Haiku 5.5, Sonnet 5.5, Opus 5.5, Fable 5.1 as ceiling, the
> provider's default effort each, pinned ids with `created_at` and declared cutoff; a non-Anthropic
> model through its API when a key exists; `claude -p` and `codex exec` as a `harness` transport in a
> table of their own, never on a held-out task until their terms document that prompts do not train.
> The held-out tasks live in this repository sealed (`openssl enc -aes-256-cbc -pbkdf2`, three
> archives, three keys), with a README, a canary and a probing protocol. Provenance per run: spec
> sha, compiler sha and `sources` key, prompt sha, suite sha, model, effort, parameters, cutoff,
> harness versions, per request the id, timestamp and usage.

The thirteen questions R1 to R13 and the default each took are `docs/panel/226-briefs/00-shared.md`
§ The questions; the two the author left to the sitting in so many words are R9 (which number is
*the thesis measured*) and R10 (when the archive opens, and whether its key stands in a GitHub
Actions secret).

## What was measured

**The compiler-engineer, in its own copy with its own stage 2** (`sources 7792ecfe50a531e8`; the
report's § `implementation_cost`):

- **`build --permissive` is a flag the frontend erases**, prototyped: +79/−39 lines over 11 files,
  `selfhost/cli/compile.hero` at **297 of 300** `code_lines` after it, no module over its ceiling;
  `heroes check selfhost/main.hero` exit 0; the compiler's own tests **1738 passed before and
  after**; stage 2 from the patched tree built (exit 0, `sources 291f5502424ce964`) and **stage 3's
  emission identical to stage 2's, 27,890,840 bytes by `cmp`**: the flag's plumbing is a fixpoint of
  itself and forces no seed refresh (a row of the argv table and a field of a record are forms the
  seed's language has).
- **The 27 codes, 27 witness programs, 23 codes exercised** (rows 15, 19, 21, 23, 24, 25 by
  reading): **17 build and run as the table pins them**. **Six programs pass `check --permissive`
  and die at `build --permissive` with exit 2**: `marker_mismatch` in three shapes (clang:
  *incompatible integer to pointer conversion*, the lowering reading the call site's marker at
  `ir/flatten.hero:613`), `ufcs_on_mutable` (the IR verifier), `declared_twice` (clang:
  *redefinition*), `bound_unit` (`mono`'s verifier, *a type the checker never resolved*). **Three
  run with a meaning their own message denies**: `cell_redeclared` in two shapes and a cell's shadow,
  the resolver reading a write and the lowering storing nothing, so `v: i64 @= 1; v @= 2; print(v)`
  prints `1`. **One reads against every permissive language**: an immutable name's shadow, the
  outer binding winning (`resolve/state.hero:310-311` binds nothing on `fail`). **Two abort with a
  false message**: `non_exhaustive` and `needs_wildcard`, exit 134 with `panic: entered unreachable
  code — this is a compiler bug, please report it` at the unnamed case. §1.12 held throughout:
  nothing reached a running binary with a wrong pointer; clang and the verifier refused first.
- **The grader's spawns**: `check --json` writes schema 2 to stderr and its `code` list buckets a
  refusal (`non_exhaustive`; `needs_label` twice and `wrong_label` twice); **a `???` program passes
  `check --json` at exit 0 with `"diagnostics": []`**, the hole as text on stdout alone, and
  `build` exits 1 with `no binary: 1 hole in …`, so a hole is read from `build`, which is how the
  coordinator's grader reads it (`thesis_grade.hero:151`). `heroes test` has no per-test time
  limit; the harness's limit reaches a test binary by one hop of signal forwarding, **measured by
  the critic's second pass**: a spinning test binary in its own process group gone 0.0 s after the
  group SIGTERM, the parent exiting −15.
- **`run_bounded`**: the bound is `seconds + 5` at worst (`HERO_RUN_WATCHDOG_GRACE_MS` 5000,
  `runtime/parts/run.c:290`); the byte cap is POSIX's alone (`can_limit_writes()` false on
  Windows); a bound over `WATCHDOG_SECONDS` should be refused as zero is. The coordinator took all
  three the same hour (below).

**The spec-warden, in its own copy** (the report's §§ 1 to 6, every count `count_tokens` on the
five live ids):

- **The spec spends 0 tokens**: `spec/heroes-spec.md` is the same file before and after, **10,286
  real tokens on `claude-opus-5`**, which is `REAL_TOKENS` exactly; the brief's *+6* compared the
  wrapper-inclusive answer (10,292) with the wrapper-exclusive pin, and `STALE` is the seed-built
  compiler's reading alone, stage 2 reading current; the five live ids count this spec identically
  on 2026-10-20 (10,286), where panel 123 found 967 between two generations in 2026-09.
- **The prompt priced as the model receives it**: 10,690 content tokens rendered, 10,286 the
  spec, 321 a 209-word task, 83 the template's words and tags; the `system` layout costs 0 more and
  caches a 10,360-token head; the user layout and the `system` layout answer the same
  `input_tokens`, so **the layout moves no token and no count can see it**, which is why the
  prompt's identity must be two shas and not one (§ 2 of the report). The ten shortest full
  diagnostics read a mean of 188 tokens, `--brief` 70. The spec's cache-read share of a Sonnet 5.5
  run's cost is under 10% at 1,500 output tokens and under 4% at 6,000: **output tokens, thinking
  included, are the cost driver, not the spec**.
- **§1.2's formula** (`real cost = program tokens × (1 + rewrite rate)`) gets its two columns for
  the first time, and *program tokens* must be read by `count_tokens` on the extracted program,
  **never from `usage.output_tokens`**, which holds the thinking too; the panel-011 sentence at
  §1.6 lines 385 to 387 comes out **amended, not deleted**, on the day the first run's measurement
  lands; §1.6's *ablation by section* names what the run does not ablate.

**The historian, every precedent read 2026-10-20 with its URL** (the report's § precedents; what
each resolution rests on is § Resolution below, and `verified` means the historian's own reading):

- `green@k` is a turn-k rate under feedback and the literature names such a rate apart from Chen's
  pass@k (Olausson's pass@t, LiveCodeBench's self-repair scenario); with n = 3 turn-1 samples
  pass@1 is estimable and pass@k for k > 3 is not, by the HumanEval repository's own rule.
- **The exact conditional McNemar test is the variant precedent advises against at these counts**
  (Fagerland, Lydersen and Laake 2013: *did not perform well for any of the considered scenarios*);
  the mid-p variant held the nominal level in every one of 9,595 scenarios. The brief's p ≈ 0.023
  at 18 against 6 is the exact binomial's arithmetic.
- **A sealed archive in a public repository holds against a crawler and against a model without
  tools, and against a model with web search only while the key is nowhere public**: GPQA printed
  its key in its README from day one; BrowseComp's key travelled with the data and Claude Opus 4.6
  found the evaluation's source on GitHub, located the key, and decrypted all 1,266 entries
  (Anthropic, 2026-03-06). R10's withheld key is the one difference that holds; a benchmark that
  withheld its key and later committed it was not found with the vocabulary searched, so R10(i)
  would be, as far as that search reached, the first of its shape.
- **Both providers' consumer terms, as read, forbid automated access outside the API** (Anthropic's
  `verified`, OpenAI's `verified in part`, a 2023 archive and a snippet; the `codex exec` page
  `verified`):
  Anthropic's Consumer Terms § 3, *except when you are accessing our Services via an Anthropic API
  Key or where we otherwise explicitly permit it*; OpenAI's `codex exec` page, *do not use this
  workflow for public or open-source repositories*. Whether an unattended `claude -p` loop on a
  subscription is *explicitly permitted* is **unverified**: no page read says so. Corrections to the
  brief: Anthropic's consumer decision deadline reads October 8, 2025 on the page today, not
  September 28; Codex's *Include environments* setting is named by no OpenAI page read.
- **Training data cutoff June 2026 for all four listed models** is stated by Anthropic's models
  overview, so the milestone page's sentence is supported today; and since the repository is public
  from 2026-09-08, after that cutoff, **the `none` context is a true *never saw Heroes* for all
  four, and the canary probe tests the provider's declaration rather than the suite**.
- **Feedback quality is the bottleneck** (Olausson 2023: human feedback in place of GPT-4's own
  raised repaired programs passing from 33.3% to 52.6%); RustAssistant fixed 74% of Stack Overflow
  programs from the compiler's message alone; LiveCodeBench's self-repair feeds *the exception
  message or a failing test case*, which is R5 in one published sentence.

**The llm-ergonomist, blind: the spec, the template and three tasks in a folder outside the tree,
nothing else** (`scratchpad/226-llm-ergonomist/`, a fresh `claude -p` session, 0.78 USD; the
report's § experiment and § choice_points):

- It wrote the three programs without running anything, and **predicted, per task, the share of
  first attempts that compile and that pass the hidden cases**: word counts 45% and 30%, the ledger
  50% and 30%, bracket depth 70% and 60%. The three rules it expects models to break most: no
  implicit conversions with `to_<int>` giving a `T?`; unused bindings and the value-returning line
  that must be bound; mandatory labels where two parameters share a type.
- **Repair 1 (a misspelt cell) is the same repair under the one-line message**; **repair 2 (a `u8`
  into an `i64` cell) differs**: the one-liner points at `b` and the obvious fix converts `b`, the
  full form says why `n` is `i64`, that the fix belongs on line 2, and marks it `guess`; *a model
  reading only the one-liner fixes the line it was pointed at*. So the full form matters when the
  fix lies away from the error's position, and that is what the strict-against-brief arm measures.
- **The risk is in the tasks, not the protocol**: it listed 25 choice points the prose leaves open
  (the argument count, an unreadable file, what a *name* is, the trailing newline, CRLF, `+5`,
  `007`, overflow, sort order with no comparator, `pop` and `split` that do not exist); *those
  measure guessing, not the language*. No sentence of the three tasks names a construct; the
  examples made no difference to its code.

**The coordinator, in a copy of its own during the sitting** (`scratchpad/226-coordinator/`, stage
2 `heroes-next` of `a0a8d1249`; the instrument's modules land in the lane after this file):

- The instrument is 16 modules, **4,547 lines under `tests/harness/thesis*.hero`** in the archive
  saved beside the lane (the critic's count; re-measured at the landing), every one under the
  harness's own tests, which pass with the amended `docs/metrics/first-try.md` beside them (it
  travels in the documents' archive, and a copy extracting the code's alone reads 7 failed, the
  critic found): `thesis.hero` **92 tests passed** (it
  reaches every module), the grader 59, `shell.hero` 37, the report 48, the seal 42 under a
  throwaway key, the sandbox 39. `shell.run_bounded` ends `sleep 30` under a one-second bound
  with **124**, refuses a zero bound and now one over `WATCHDOG_SECONDS`, and refuses a byte cap
  where `can_limit_writes()` is false (the compiler-engineer's three points, taken).
- `dry tests/thesis` grades the three assistant tasks' reference solutions **7/7, 9/9, 9/9 in the
  strict arm and the same in the control arm**, 0 red; `plan tests/thesis --model claude-sonnet-5-5`
  counts **3 requests at turn 1** for three arms (one generation per task shared by the arms) and
  **20,650 estimated input tokens**, sending nothing; `tasks tests/thesis` prints the three task
  digests and the suite's, `1d6a524d…`, label `assistant`.
- The statistics module's witnesses: Wilson at 7 of 15 reads 24.8% to 69.9%, at 0 of 20 an upper
  bound of 16.1%; the exact conditional McNemar reads 0.039 at 10 against 2, 0.023 at 18 against 6,
  0.065 at 24 against 12, 1.0 at 3 against 3; **the mid-p reads 0.022 at 10 against 2 and 0.015 at
  18 against 6** (by hand: 2 × (13 + 33) / 4096 and 2 × (55455 + 67298) / 16777216), 1.0 at 3
  against 3; the Park-Miller generator reads 16807 then 282475249 from seed 1. The report module groups verdicts by (model, arm, context), counts `green@1`,
  `green@2`, `green@3` and `green@cap`, buckets the last turn into the three classes, and pairs a
  strict verdict with its control twin by the key without the arm.
- **The seal, run during the sitting under a throwaway key** exported into the process alone
  (`thesis_seal.hero`; the words `seal`, `canary`, `tasks --sealed`, `dry --sealed`): a copy of the
  assistant's suite took 4 canaries (`LABEL`'s second line and each `task.md`'s last), sealed into
  445 KB of base64 opening `U2FsdGVkX1` with its sha256 beside it; `tasks --sealed` read the opened
  copy with **the same three task digests and suite digest as the clear copy** (the canaries
  already written in both: a canary line moves a task's digest, the critic measured, so
  `hashes.txt` is what `seal` prints after writing them, never a count taken before), `dry
  --arm control` on the opened folder passed 7/7, 9/9, 9/9; a wrong key was refused with openssl's *bad decrypt* at exit
  2, an archive with one byte appended was refused as *moved* before any decrypt, and the scratch
  under `build/` was gone at exit. The round-trip test says `SKIP` where `HEROES_SEAL_TEST_KEY` is
  unset, as `issue_git`'s do without a token, so the lane's gate exports a throwaway key.
- **The sandbox, wired into the grader during the sitting** (`thesis_sandbox.hero`, R11 below):
  its own test builds a probe and runs it under `sandbox-exec` (39 tests passed with the modules
  it reaches), the grader's 59 tests pass with every case run inside it, and `dry tests/thesis`
  passes 7/7, 9/9, 9/9 under it in the strict arm.
- **R1, built in the coordinator's copy during the sitting, on the compiler-engineer's reading**
  (the route adopted below, re-made by hand from its report and not copied): the flag's plumbing
  in eleven files as the prototype had it; then the seven meanings. Measured with the stage 2 of
  the patched tree, over the compiler-engineer's own 28 witness programs: **26 build under
  `build --permissive` and run as the table pins them, none exits 2**; the two that do not are no
  running program (a parse error the control arm keeps, `unexpected_block`, and a hole). The six
  that died at exit 2 run: `bump(v)` to `@n` as the caller's place (prints `2`), `show(@v)` to `n`
  as a value (`2`), `v.bump()` lending `v` (`2`), a second `function f` not lowered (`1`), `x =
  greet()` binding nothing (`hi`, `1`), `bump(1)` to `@n` writing a held temporary (`2`). The three
  silent no-ops store: `v @= 2` on a cell, `v = 2` shadowing a cell, `v: i64 @= 2` after `v: i64
  @= 1` each print `2`. The immutable shadow reads inner then outer, `2` then `1`. The two false
  aborts say **`panic: this match has no arm for .dot`** and **`panic: this match has no arm for
  7`**, exit 134, through two abort kinds of the IR and two runtime functions,
  `hero_panic_unmatched_case(int64_t, HeroStr)` and `hero_panic_unmatched_value(HeroStr)`,
  declared after `hero_panic_assert_sides` and **not moving `HERO_RUNTIME_ABI`** (an added
  declaration does not change the shape of the declarations the stamp guards; the ffi-pragmatist
  is asked to say whether it agrees). **The strict build did not move**: the resolver's shadow
  binding and the lowering's aborts are taken only under a `permissive` flag threaded through
  `resolve.resolve_as` and `lower.lower_as` from `compile.Options` and `check --permissive`, so the
  `check` form reads 703 passed, `permissive` 22, `full` 40, `ir` 30 and `layout` 6, each 0
  failed; `emit` reads 12 of 13 moved **by the `hero_program_sources` line alone**, the key of the
  runtime every program links, which the landing re-blesses as a runtime change always owes. The
  compiler's own tests: 1738 passed. Stage 3 from the patched stage 2 emits the same 26,907,438
  bytes (the compiler-engineer's 27,890,840 and the critic's base 26,694,926 are other trees' and
  stay a question this file records, `#line` names being relative). The ceilings moved with their
  measurements written in `DECIDED`: `ir/flatten.hero` 1150
  to 1250 (measured 1243: the five shapes of an argument through one `one_argument`, which calls
  `place` and `expr` and cannot leave the module), `ir.hero` 315 to 325 (323), `resolve/state.hero`
  330 to 350 (349), and two new rows at 305 for `ir/build.hero` (301) and `cli/compile.hero` (302),
  each one field or one flag over the plain ceiling. **A new golden form, `control`**
  (`tests/harness/suite_control.hero`, `tests/golden/control/`, 26 cases): the strict build must
  refuse the case at exit 1 and its `#~` marks must be the codes `check --brief` names, then
  `build --permissive -o` and the binary at `-O0`, `run --permissive` at `-O2`, `run --permissive
  --sanitize` with `run/`'s sanitiser rules; **26 passed, 0 failed**. The net's own tests with the
  third root: 414 passed. **And the compiler is not slower** (CLAUDE.md § Verification, the machine
  otherwise still, `user` within 0.3 s of `real` on every run): `check selfhost/main.hero` three
  times each, before 6.96, 6.78 and 6.80 s real, after 6.91, 6.71 and 6.73 s.
- The ffi-pragmatist's five-cent `claude -p` probe and the llm-ergonomist's session were the
  sitting's two paid calls, both named in the briefs; the coordinator sent nothing to a model.

**The ffi-pragmatist, in its own copy, its first run stopped at 06:45 inside the one paid probe its
brief allowed** (`claude -p`, five cents, hung two hours; *unrun as a measurement*, in its words, and
not started again), resumed from its report file and finished at 10:25 (the report's §§ 1 to 6):

- **The transport's shape** (§ 1, against a listener of its own on `127.0.0.1:18226`): `curl` with
  `--variable %ANTHROPIC_API_KEY --expand-header` keeps the key out of argv and of every file, the
  body on `--data-binary @<file>`, `-H "Expect:"` so no `100 Continue` precedes the status line.
- **The sealing** (§ 2): OpenSSL 4.0.3 and LibreSSL 3.3.6 **open each other's archive byte for
  byte** at 600,000 iterations with `-a`; **`-A` does not cross** (LibreSSL refuses 4.0.3's wrapped
  input under `-A`, 4.0.3 refuses LibreSSL's one-line `-A` output under `-a`), so `-a` on both
  ends, as the instrument's words have it, and `-A` never; **a tampered archive still decrypts**
  (CBC authenticates nothing), so the sha256 beside it is load-bearing and not a courtesy; `-pass
  env:` leaves the key out of `ps`. Unrun and marked: GNU tar opening bsdtar's archive (no `gtar`
  here), the runner's OpenSSL version.
- **The sandbox** (§ 3), the coordinator's profile taken as written: writes outside refused, the
  home unreadable, **fork denied** (`/bin/sh: fork: Operation not permitted`, so a fork bomb is
  impossible), the chain `sh → env → sandbox-exec → binary` one pid that `timeout` and the group
  kill reach; **`(allow process-exec (literal "<binary>"))` runs the probe and refuses
  `/bin/sleep`**, so `process-exec*` narrows. **And the hole**: a C program calling
  `sysctl({CTL_KERN, KERN_PROCARGS2, <pid>})` inside the profile **reads a same-user process's
  environment, `hits=1`, with `sysctl-read` and `process-info*` removed too**; the grader's process
  holds the seal's key for the whole of a `dry --sealed` or a held-out `run`, so only
  `refused_extern` stands between a model's program and the key. Readable inside as well:
  `/Library/Preferences/SystemConfiguration/preferences.plist` (23,713 bytes),
  `/Library/Keychains/System.keychain`'s bytes, `/etc/hosts`. The allow-list profile and every
  `bwrap` word: unrun.
- **Another identity** (§ 4): `sudo -n` needs a password, `kill -0 1` is refused across uids; a
  dedicated user would hide one process's environment from another (an inference, unrun across
  uids since the seat created no user) at the price of a sudoers rule and a `chown`; the profile
  alone answers R11 once the key is nowhere in the spawning process's environment.
- **The `harness` transport** (§ 5), from the hang itself: a `claude -p` that never returns is a
  sample never judged and a wall the instrument cannot see; it owes `run_bounded`'s seconds, its
  prompt on stdin from a file so EOF is certain, the chain `sh -c 'cd "$1" && HOME="$2" exec claude
  -p …'` so the child is one pid, and **`--tools ""`**, the one word of `claude --help` that
  disables every tool by construction (`--disallowedTools` names commands and `WebFetch*`, no
  `WebSearch`).
- **The C boundary** (§ 6): the two runtime declarations change no shape; the emitted call against
  an older header is clang's *call to undeclared function*, against an older object ld's *Undefined
  symbols*, so skew is a compile error without moving the stamp; the five guarded words stand in
  the list and both guard files. **On the ABI: approve, `HERO_RUNTIME_ABI 30` stays.**


**The completeness critic, second pass, over the five reports and this draft**
(`docs/panel/226-reports/critic.md`, 308 lines, in its own copy; it gives no verdict). What it
measured and the draft takes: **the harness's limit reaches a test binary**, the
compiler-engineer's one *unrun*: a spinning test binary in its own process group was gone 0.0 s
after the group SIGTERM on `heroes-next test`, the parent exiting −15; `bound_unit` as built is
*the call runs and nothing is bound*, `mono` untouched, and the whole R1 patch is +504/−132 lines
over 30 files (`flatten` +129/−24); `exit 153`, the three *Invalid argument*s, the 25 choice
points and the sandbox's 39 tests hold in its copy; the instrument's 16 modules measure 4,547 lines
in the saved archive against the 4,529 this draft said (re-counted at the landing); the thesis
root's 92 tests pass only with the amended `docs/metrics/first-try.md`, which travels in the docs
archive and not the code archive, so a copy that extracts one tar reads 7 failed; **a canary line
appended to a task's prose moves its digest and the suite's**, so `hashes.txt` is computed after
`seal` writes the canaries, never before; **the instrument counted `api_refusal` verdicts inside
`n`** (`thesis_report.hero`) and had no `transport_error` bucket, against R5; the three
whole-compiler emission sizes (the base 26,694,926 bytes in its copy, the coordinator's 26,907,438,
the compiler-engineer's 27,890,840) are not path effects, `#line` names being relative, and the
compiler-engineer's stays a question this file records as one. **Its question for a sixth seat**:
the trial's judge-only control builds strict, so the cell *the control runs it, the strict build
refuses it* lands in `build_failed` and is excluded, yet n and the inconclusive rule were to be
pre-registered from that trial's discordance; and the per-task GUID is an eval-awareness cue in
every held-out prompt. **Routes nobody listed**: the rule assigning a table row to silent or loud,
which moves the control's turn-2 feedback and so the primary; a fourth *it did not compile* arm;
a primary on the silent class the proposal's own sentence names; a smallest effect of interest; a
fork at the first turn where the arms' prompts differ rather than at turn 2 by rule; a route C for
R9, the `harness` transport under an API key; one key per task; a sha256 commitment to the key at
the freeze; a signed attestation between the secret and the local run. **Of its 22 repairs the
draft takes 20**, each in its section below; the two not taken are one key per task (R10 says why)
and a new sitting of the compiler-engineer over the 27,890,840 bytes (recorded as a question).


## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**, no veto | §1.7, §1.1, Part 5, Part 11 lines 4351 to 4367, §1.12 | +79/−39 lines, 11 files, `compile.hero` 297/300; the seven lowering-side meanings on `ir/flatten.hero` (1150/1150), `ir.hero` (315/315), `resolve/state.hero` (329/330); row 27's abort ~25 lines over 6 files | if `build --permissive` lands as a filter with no lowering-side meaning for `marker_mismatch`, `ufcs_on_mutable`, `declared_twice` and `bound_unit`, the control arm's `build_failed` bucket at the first held-out run is non-empty and every member carries one of those four codes under strict `check --json` | approve on (a) the 27 witnesses each running as pinned or refused at `check --permissive` by a code the table names, never exit 2, `compile.hero` under 300 or split at a named seam; or (b) the four exit-2 codes ruled into panel 192's third category and `drop-case`'s control column re-measured |
| ffi-pragmatist | **object**, no veto (on the ABI alone: approve) | §1.11, §1.12, §4.19, Part 11's provenance sentence; design.md names neither a process's environment as a channel nor `sandbox-exec`'s operations | none on the ABI; three narrowings | at the milestone's close, with `HEROES_TASKS_KEY` exported in the grader's process, a C program calling `sysctl(KERN_PROCARGS2)` on the grader's pid inside the profile prints the key's value; with the archive opened by a separate invocation that exits before the first `build`, it prints `hits=0` | approve when the key is never in the environment of the process that spawns a model's program, `process-exec*` becomes `process-exec (literal "<binary>")`, and a `harness` transport is spawned by `run_bounded` with stdin from a file and `--tools ""`; veto if a `harness` transport or an `extern`'d program ever runs under the profile with the key exported |
| spec-warden | **approve** | §1.6, §1.2, §1.1, CLAUDE.md § 2 | spec delta **0**; 10,286 = `REAL_TOKENS`; no ledger row owed | the strict arm's correction round-trip (one full diagnostic fed back plus one whole rewritten program, `count_tokens` on `claude-opus-5`) has a median between 700 and 1,300 tokens over the held-out tasks on Sonnet 5.5; the full diagnostic fed back a mean of 150 to 300 tokens and `--brief` under 100; the spec's cache-read share under 10% of a Sonnet 5.5 run's USD at 1,500 output tokens | object if the landing pools the user layout and the `system` layout under one prompt sha, or keeps *ablation by section* in Part 11 while the run ablates the productions alone, or reads §1.2's program tokens from `usage.output_tokens`; veto if a spec token is spent without a ledger row |
| historian (advisory) | **approve** | design.md's historical appendix | none | none of its own; four conditions written as precedent | changes on: a published measurement where a compiler's diagnostic as feedback gave no gain over *it failed*; a documented case where a never-public key's archive was extracted into a corpus; a model completing a per-task GUID of the sealed suite before the key is committed; a provider page explicitly permitting unattended `claude -p` or `codex exec` on a subscription; Aycock's result failing to transfer |
| llm-ergonomist (blind) | **approve** | the thesis: locality and the message | none | per task, first attempts that compile / pass hidden cases: 45/30, 50/30, 70/60 (%); the three rules broken most: implicit conversions and `to_<int>`'s `T?`, unused bindings and the unbound value, mandatory labels | object if most first attempts that compile and fail hidden cases do so on behaviour the prose leaves open, or if the full-against-brief difference is smaller than run-to-run noise |

## Disagreements, stated plainly

- **The compiler-engineer against the proposal's R1 as written.** The proposal promised a table
  giving each of the 27 dropped rules a runtime meaning and read `build --permissive` as a filter.
  Measured, the filter is the whole answer for 17 codes and no answer for seven: four end in exit 2
  after a clean `check --permissive`, three run with a meaning their own message denies, and one
  more reads against every permissive language. Nobody else measured this; the critic's first pass
  had found only row 27's false abort message. The sitting takes the objection whole (R1 below).
- **The historian against the brief's McNemar.** The brief's arithmetic is the exact conditional
  test's; the precedent the historian verified advises the mid-p variant at these counts. The
  coordinator's `thesis_stats.mcnemar` was the exact conditional alone until the historian's report
  was read; the sitting pre-registers the mid-p, added that hour, and keeps the exact beside it (R2).
- **The historian against route B of R9**, and against any tool-enabled harness near the archive:
  both providers' consumer terms forbid automated access outside the API, the `codex exec` page
  points automation at an API key and away from public repositories, and BrowseComp shows what a
  model with web search does to a sealed set whose key it can find. The ffi-pragmatist's own
  `claude -p` probe hung two hours and never returned, its cause unrun (most plausibly no
  credential under `--bare`, the critic reads), and a `harness` transport owes a bound, stdin from
  a file and `--tools ""`. The one `claude -p` run of the sitting that answered is the blind seat's,
  on the assistant's tasks, which is the permitted side of R9's line; which credential it ran under
  is a question the landing answers from its session's record before the row is cited.
- **The ffi-pragmatist against the coordinator's sandbox and seal as built.** The profile holds
  where it was measured, and still a program inside it reads the grader's environment through
  `sysctl(KERN_PROCARGS2)`, where the seal's key sits for the whole run: a hole one bucket-check
  deep (`refused_extern`). Nobody else had asked what a process can read of another process. The
  sitting takes all three of its narrowings (R10 and R11 below).
- **The llm-ergonomist against the tasks, not the protocol**: its 25 choice points are where it
  expects hidden cases to fail for reasons that are not the language's. The task rules of R8 gain
  the list as a checklist the prose must pin or the cases must cover (R8 below).
- **The spec-warden against a single prompt sha**: a `system` layout and a user layout count the
  same tokens and are not the same prompt. Two shas (R12).
- **No seat vetoed.** The two `object`s name their conditions: the compiler-engineer's (a) is met by
  R1 as built, the ffi-pragmatist's three by R10 and R11 as amended below.

## Resolution (provisional: author ratification pending)

The most robust and complete resolution, never the cheapest (CLAUDE.md § 4, CL-040); where robust
and conservative part, conservative is written beside it so the author can choose it. **Prior art**
is the historian's `verified` precedent each item rests on, re-read by the coordinator on
2026-10-20 at the URL the historian's report gives; *none* where no precedent carries the item.

- **R1. The control arm builds and runs, and every one of the 27 codes has a runtime meaning the
  binary performs, pinned in a table, or a refusal the table names.** `build --permissive`, `run
  --permissive` and `test --permissive` land as the compiler-engineer prototyped them (the filter
  `diag.kept` beside `is_thesis_rule`, `Options.permissive`, `parsed.control` in `compile.to_unit`),
  **and the seven codes the filter does not answer get their lowering-side meaning in the same
  landing**: a cell's `x @= e` and a cell's shadow perform the store (`ir/flatten.hero`'s
  `.declare`, reading `r.rebinds`); an immutable name's shadow binds the inner name (*inner wins*,
  `resolve/state.hero`); `marker_mismatch`: the parameter's declaration decides and the call site's
  marker says nothing, and an argument that is not a place, handed to an `@` parameter, is held
  in a temporary the callee writes and nobody reads, the way a discarded value falls (built and
  measured: no checker line moves, and the strict arm's `marker_mismatch` goldens stay);
  `ufcs_on_mutable`: `x.f()` lends
  `x`'s place as `f(@x)` does (`with_receiver`); `declared_twice`: the first declaration wins and the
  second is not emitted; `bound_unit`: **the call runs and nothing is bound**, the poisoned local
  given no slot, `mono` untouched (built, the critic read it so); `non_exhaustive` and `needs_wildcard`: a third `AbortWhy`
  case and `hero_panic_unmatched` in the runtime, the message **`panic: this match has no arm for
  .dot`**, the case's name as the program spells it, the value through `to_str` for `i64`/`str`
  chains, never *this is a compiler bug*. Where a module stood at its `DECIDED` row
  (`ir/flatten.hero` 1150, `ir.hero` 315, `resolve/state.hero` 329 of 330), **the row was raised
  with its measure written beside it**, as every raise of that table was, the helpers that grew
  `flatten` calling `place` and `expr` that no other module may; `compile.hero` and `ir/build.hero`
  take rows of their own. The whole patch is +504/−132 lines over 30 files (the critic's count). **Built during the sitting** (§ What was measured): the
  seven meanings are taken under a `permissive` flag the resolver and the lowering receive from
  `compile.Options`, so the strict build's words and C are the ones it had, and **the 26 witness
  programs that run enter `tests/golden/control/` as cases of a new golden form, `control`**:
  each refused by the strict build at exit 1 with its `#~` marks held to `check --brief`'s codes,
  then built with `--permissive` and run at `-O0`, `-O2` and under the sanitiser, printing what
  its pinned sentence says or aborting with the message the table names, and `build --permissive`
  exits 2 on none of them, which is the compiler-engineer's condition (a) made a gate. **R1 was built during the sitting, so the trial on the assistant's tasks runs with the control
  arm whole** and no judge-only interim exists: n and the inconclusive rule are read from a trial
  whose control builds and runs, which the critic asked of a judge-only trial and this file no
  longer proposes; a `build_failed` in the trial is bucketed *strict accepted, build refused* and
  read as a compiler defect only where the strict arm's own `build` refused. *Conservative would have been* the
  compiler-engineer's condition (b): the four exit-2 codes ruled into panel 192's third category,
  kept refused under `--permissive`, which measures the thesis on 23 rules and credits the `@` rule
  with nothing, as `non_exhaustive` was credited with nothing before 2026-10-16 (issue 2078). The
  author's *4a* of that day is why the sitting takes the complete table. *Not taken either*: a
  control that keeps the 27 diagnostics as warnings the model sees and still builds (the critic's
  first pass), which measures the rules' presence without their refusal, a third thing; and a
  fourth arm, *it did not compile*, the historian's RustAssistant precedent presupposes, which is
  the strict arm's own turn-1 column and not an arm. *Prior art*: none the
  historian sourced; the meanings cite the permissive language each row names (C, Python, C++
  `T&`, Pascal `var`, Swift `mutating`, Rust's `_`) as the table's own reasons, not as findings.
- **R2. The primary, pre-registered at the freeze**: `green@3`(strict, full message) −
  `green@3`(control) on `claude-sonnet-5-5` in the `spec` context, over the held-out suite, three
  samples per task; **the interval is the cluster bootstrap over tasks** (the task the cluster, the
  three samples never nine independent pairs; 2,000 resamples, the seed in provenance); **the test
  is McNemar's mid-p over the discordant pairs, named as such in the pre-registration, the exact
  conditional p reported beside it** and never the primary's; three outcomes named before the run:
  the bootstrap's lower bound above 0 confirms, its upper bound below 0 falsifies, else
  inconclusive, and the inconclusive outcome's rule is written at the freeze (publish as such,
  extend n with the reserve, or stop); **the smallest effect of interest is 10 percentage points**,
  the number the author's predictions already name, so a confirmed difference under it is reported
  as confirmed and small, never as success. **Why `green@3` and not the silent class**: the
  proposal's sentence measures the thesis on the silent class, and the thesis's claim to a reader
  is that the program a user ends up with works, which `green@3` says under feedback a user would
  have; **the silent rate at turn 3, control minus strict, is the first named secondary**, with its
  own interval, and the two are read together. The turn-1 column is deterministic once the
  generations exist and is reported as an exploratory row of its own. `pass@1` in Chen's sense is reported apart on the turn-1
  samples and no `pass@k` for k > 3. **Olausson's pass@t is reported beside `green@k`** from the
  usage already recorded, tokens-to-green per arm. Everything else is exploratory and says so in
  its heading. The instrument holds both (`thesis_stats.mcnemar` and `mcnemar_midp`, the second
  added during the sitting on the historian's reading) and the report prints the mid-p with the
  exact beside it. *Prior art*: **Taken from** Fagerland, Lydersen and
  Laake 2013 (the mid-p variant); **Taken from** Miller 2024 (the paired, clustered standard error
  and the bootstrap for a complicated estimator); **Taken from** Field and Welsh 2007 (the cluster
  bootstrap's consistency); **Reached first by** Chen et al. 2021 (pass@k's unbiased estimator and
  the refusal to estimate pass@k with fewer samples than k); **Reached first by** Olausson et al.
  2023 (pass@t, the rate at the tokens one would expect to spend).
- **R3. Three failure classes**, the thesis measured on the silent one, **and the rule that
  assigns a table row to one of them**: a row whose pinned meaning is an abort (`non_exhaustive`,
  `needs_wildcard`, `endless_recursion`) is loud, every other row's program runs to exit 0 and is
  silent or correct by what it prints; the rule moves the control's turn-2 feedback (the example's
  observed output against an abort's `panic:` line) and so the primary, which is why it is written
  here and pinned by `tests/golden/control/`'s `.expected` files. The classes: refused at compile time;
  loud at run time, exit 134 with `panic:` on stderr, R1's abort included; **silent**, exit 0 with
  the wrong stdout or ending (`compiled_wrong`, issue 35's bucket named). A correct stdout with a
  wrong exit and stderr noise at exit 0 are both `compiled_wrong` by `expectation.unmet`'s own
  reading; a run-time `timeout` and a `crash` (exit above 128 without `panic:`, a compiler defect
  with its own issue) are their own buckets; `hole` is a `???` `check` accepts and `build` makes no
  binary of, read from `build`'s `no binary:`. *Conservative*: two classes, the abort among the
  silent, which would credit the thesis with every loud abort. *Prior art*: none.
- **R4. Three arms**: strict with the full message, strict with `--brief`, control. Full minus brief
  is the message's value (design.md §4.17); brief minus control is the rules' value. The
  llm-ergonomist's repair 2 is the shape the first difference measures: a fix that lies away from
  the error's position; **full against brief is reported over the pairs holding at least one
  refusal** beside the whole, since a pair that never saw a message cannot tell the two apart (the
  critic). *Prior art*: **Reached first by** Deligiannis et al. 2023, RustAssistant
  (the compiler's message as the whole repair signal, 74% fixed without test output).
- **R5. Symmetric feedback in every arm, the hidden cases never**, and **the fork at the first
  turn whose prompt differs**: the arms share turn 1 by construction, and where two arms' turn-2
  prompts are byte-identical (both compiled and failed the example the same way) they share that
  generation too, forking only where the feedback differs, which saves requests and pairs the arms
  more tightly (the critic's route; the instrument's `run` lands it at the landing, and the pairs
  are then the arms' own by prompt rather than by turn number): on a refusal the arm's
  compiler message (full or brief; the control's refusal is a non-thesis error, the same text); on a
  program that compiles, the visible example (`cases/0`, also among the hidden) run and its
  observed-against-expected returned, a `panic:` included; a fixed sentence per non-compiler bucket
  (`no_program`, `format_slip`, `truncated`, `refused_extern`, `hole`, `timeout` on the visible
  example) in the frozen template, identical across arms; the conversation ends when the visible
  example passes and a hidden case does not, the turn marked `compiled_wrong` or `abort`; two
  buckets outside the denominator, counted and never resampled in silence: `api_refusal`
  (`stop_reason: refusal` at HTTP 200) and `transport_error` (past `thesis_pace`'s retries); **the
  report excludes both from `n` and prints them in a column of their own**, which the instrument
  did not do when the critic read it (`thesis_report.hero` counted `api_refusal` inside `n`, and
  no verdict was named `transport_error`), and does since the landing.
  *Conservative*: the compiler's message alone and the control told nothing on a program that
  compiles, which a blind reader found wins by construction. *Prior art*: **Reached first by**
  Jain et al. 2024, LiveCodeBench (self-repair fed *the exception message or a failing test case*);
  **Taken from** Olausson et al. 2023 (feedback quality as the bottleneck, so the feedback is the
  compiler's fixed text and never a model's).
- **R6. Contexts and contamination**: `spec`; `none`, the task alone with *write it in Heroes*,
  reported as *knowledge of the language* with the rule of three, and **read against the declared
  cutoff**: all four models declare June 2026 and the repository is public from 2026-09-08, so
  `none` is a true *never saw Heroes* today and **the canary probe tests the provider's declaration,
  not the suite**; variants by path at the held-out run on ten tasks with Haiku and Sonnet (the
  spec without its 46 productions, panel 133's prediction 3 and Aycock's transfer test; with one
  worked diagnostic, issue 1005); *never saw Heroes* is written **by the provider's declaration**
  and after the site's first publication date is read from the `site-v*` tags at the landing, so
  the sentence rests on two dates and not one. **Per-task GUIDs, not the suite's alone, and out of
  the prose the model reads** (the critic: a GUID line in every prompt is an eval-awareness cue):
  the canary line stays in `task.md`, the file a leak would carry, and is stripped from the task
  text the instrument renders, so a model sees no canary in a task; the probe asks each prefix
  apart. *Canary* names the GUIDs alone; the `none` context is reported as *knowledge of the
  language*, another word. a suite GUID can be
  learnt from the repository as BIG-bench's can, a per-task GUID exists in one sealed file and
  nowhere else, so its exact completion is dispositive and the suite's is a warning; `canary-probe`
  asks each model to complete each prefix before a run and the README says *a per-task GUID
  completed exactly = that task burned, replaced from the reserve; the suite GUID completed = the
  run stops and the author reads*. Heroes-only spellings grepped in code blocks alone and for
  non-word tokens. *Prior art*: **Taken from** BIG-bench (the canary GUID in every task's README,
  as a filter for training pipelines); **Reached first by** Rein et al. 2023, GPQA (a canary string
  beside a sealed dataset); **Taken from** Jain et al. 2024, LiveCodeBench (reading a model's drop
  against its declared cutoff date); **Taken from** Riddell et al. 2024 (the 42-point gap
  contamination buys on HumanEval, the reason the seal is proportionate).
- **R7. Models and sampling**: `claude-haiku-5-5`, `claude-sonnet-5-5`, `claude-opus-5-5`,
  `claude-fable-5-1` as ceiling, each at the provider's default effort written out (`medium`,
  `high`, `medium`, `high` by the models overview), *equal effort* exploratory; thinking on and its
  blocks kept whole in the transcripts and re-sent whole; `temperature`/`top_p` recorded as default
  where the model refuses them; `max_tokens` 32,000 revised on the trial's output distribution; no
  `fallbacks`; `created_at`, the echoed `model` and the declared cutoff in provenance; **the forks
  proven once at the trial under `prefix_mismatch_behavior: "error"`**, so an arm silently running
  with dropped thinking is impossible rather than unnoticed. *Prior art*: none.
- **R8. Tasks and n**: 40 held out plus 20 in reserve, three samples; n confirmed from the trial's
  measured discordance and intra-task correlation before the freeze (orientation: 120 pairs make
  10pp twelve net pairs, mid-p near the exact's 0.023 at 18 against 6); the task rules as proposed
  (one `main.hero`, no `use`, no `extern`, inputs by `args()` and at most three files, exact stdout
  and exit code, 4 to 8 hidden cases, English prose naming no construct, a reference of 25 to 80
  lines, a hand mutant per tempted mistake and the case that kills it), **and the llm-ergonomist's
  choice points become the task checklist**: the prose pins, or a hidden case covers, the argument
  count and an unreadable input, the trailing newline, CRLF, the character class of a name, signs
  and leading zeros, overflow, the order of ties, and the exit code of every malformed input, so a
  hidden failure is the language's and not a guess's; a difficulty target from the trial
  (`green@1` strict on Sonnet between 0.3 and 0.7 for the median task); a blind second reader before
  the freeze; adjudication pre-registered (ambiguous → the next reserve task in order, by a reader
  who has not seen the arms' verdicts; a sample without an answer past the retries → excluded from
  every arm; `build_failed` → excluded, reported in a table of its own and refiled as a compiler
  defect, **expected empty with R1 built**). *Prior art*: **Taken from** Jimenez et al. 2023,
  SWE-bench (FAIL_TO_PASS grading, solved when every test passes); **Reached first by** SWE-bench
  Verified (a human-validated subset of 500, `verified in part`: its discard rate stands in
  snippets alone and is not cited); **Taken from** Miller 2024 (three samples are a cluster, never nine pairs).
- **R9. The thesis measured is the API's number: route A.** Part 11's protocol stays the frozen
  single prompt through the API; the `harness` rows (`claude -p`, `codex exec`) run on the
  assistant's tasks alone, in a table of their own labelled by transport, **with every tool denied
  and no web access** (`--disallowedTools` whole and `--bare` for `claude -p`; `-s read-only` and
  search off for `codex exec`), and **never on a held-out task** until a provider page documents
  that the transport's prompts do not train *and* that unattended use on that plan is permitted;
  today neither is documented for a subscription, and the Codex automation page points automation
  at an API key and away from a public repository. A non-Anthropic model enters through the
  OpenAI-compatible dialect when a key exists. **Route C, a `harness` transport under an API key**
  (`claude -p` with `ANTHROPIC_API_KEY`, which the Pro/Max page says makes Claude Code use the key
  instead of the subscription), is on the API's terms and may run on held-out tasks as an
  exploratory row measuring the harness's own prompt; it is not the primary, which stays the frozen
  prompt through the API. The blind seat's `claude -p` session ran on the assistant's tasks, the
  permitted side; which credential it ran under is read from its session record at the landing. **A `harness` transport is spawned by
  `run_bounded` with seconds of its own** (under `WATCHDOG_SECONDS`, which `run_bounded` refuses
  to exceed), its prompt on stdin from a file so that EOF is certain, through `sh -c 'cd "$1" &&
  HOME="$2" exec …'` so the child is one pid, and with `--tools ""`, the one word that disables
  every tool by construction: the ffi-pragmatist's own probe hung two hours and was never judged.
  *Route B would have been*: the harness's number as
  the thesis, `claude -p` against `codex exec`, fair between two harnesses and not Part 11's frozen
  prompt, spending the plans' quota, and, as the terms read today, automated access the consumer
  terms do not permit. *Prior art*: **Taken from** Jacovi et al. 2023 (*demand training exclusion
  controls from closed API holders* and refuse to evaluate without them); **Taken from** Anthropic's
  Consumer Terms § 3 and OpenAI's `codex exec` page (automated access outside the API; *do not use
  this workflow for public or open-source repositories*).
- **R10. The sealed archive**, the author's decision, with the two questions the author left to the
  sitting answered: **(i) the archive opens at the close of the measurement series it serves**,
  the key committed and the tasks readable in git's history, **with a mechanical trigger written in
  the README**: *within 30 days of the first `v*` release tagged after the held-out run, or of the
  suite's being declared burned by R6's rule, whichever is first; the author commits the key; a
  task fails to open only while the series is live*; meanwhile the key is given on request, the
  delivery register in the README naming the day and the recipient, with the one condition of not
  republishing it. *Conservative*: at M-publication-gate, which may be months after the series
  closes and keeps a finished test sealed for no reason the README can state. **(ii) The key stands
  in a GitHub Actions secret, inside an environment with a required reviewer, for a `thesis-attest`
  job** that decrypts in the runner, runs `tasks --sealed` against `hashes.txt`, the prose lint and
  `dry --mute` on the reference solutions, prints counts and digests alone, on a failure a task's
  digest and never its text, and is skipped (`if:` on the secret) on a fork; the author's two
  reasons, that a stranger can see the archive is what the README says without reading it, and
  that nothing *visible to humans and not to crawlers* exists without a key. *Conservative*: the key
  local alone and the attestation by hand, which no stranger can see run. Beside them, as
  proposed: three archives and three keys (`HEROES_TASKS_KEY`, `HEROES_RESERVE_KEY`,
  `HEROES_RUNS_KEY`), the ciphertext's sha256 beside each (**load-bearing, not a courtesy: a
  tampered archive still decrypts under CBC**, the ffi-pragmatist measured), `-a` on both ends and
  `-A` never (the two OpenSSLs do not cross under `-A`), the `-iter` count written, the licence
  named, the delivery register; **a sha256 of each key committed at the freeze beside the
  ciphertext's**, so the key committed at the opening is checkable as the one the seal was made
  with (the critic's route); **one key for the suite and one for the reserve, not one per task**,
  since R6's replacement rule burns a task and draws the next from the reserve under its own key,
  and forty keys in the register are forty deliveries to get wrong. *Not taken*: a signed
  attestation by the author in place of the CI secret, which proves the author ran it and lets
  nobody else see it run; the secret in a reviewed environment is chosen because a stranger can
  read the job's log. **And the key is never in the environment of a process that runs
  a model's program** (the ffi-pragmatist's first condition): the archive is opened by its own
  invocation, `unseal`, which decrypts into a folder under `build/` and exits; `dry` and `run`
  read that folder in a shell where no `HEROES_*_KEY` is set, and **the grader refuses to run a
  case while one is**, so the operator cannot forget; `dry --sealed` is refused for the same reason
  and says so; `tasks --sealed`, which runs no program, keeps the key. The CI's `thesis-attest`
  unseals in one step and grades in the next under `env -u`. *Prior art*: **Taken from** Jacovi et
  al. 2023 (a password-protected archive, licensed against derivative distribution); **Reached
  first by** Rein et al. 2023, GPQA (a sealed archive in a public repository, its key in the README
  from day one, which R10 differs from by withholding the key); **Taken from** Wei et al. 2025 and
  Anthropic 2026 on BrowseComp (a key that travels with the data is found by a model with search;
  the key is withheld); **Taken from** Epoch AI 2025 on FrontierMath (the failure was bookkeeping,
  so the register, the licence and the trigger are written down).
- **R11. The sandbox, measured during the sitting** (`thesis_sandbox.hero`): on this Mac the
  model's binary runs under `/usr/bin/sandbox-exec` with a `deny default` profile that allows
  exec, process information, a signal to itself and `sysctl-read` (without which the Heroes
  runtime dies sealing a guard page, *the system refused to seal the page after it*, measured),
  allows reads where the system's libraries live and denies them under `/Users`, `/Volumes` and
  every temporary folder, allows reads and writes in the sample's scratch alone and writes on
  `/dev`, and denies the network; in front of it `ulimit -f 4096` (a 10 MB write died with
  SIGXFSZ, exit 153, measured) and `env -i`. The probe wrote inside, was refused outside and in
  the author's home, read neither, exit 0, the same through a pipe. **No memory ceiling exists on
  this Mac**: `ulimit -v`, `-d` and `-m` answer *Invalid argument* from `/bin/sh` (measured) and
  the runtime has no allocation cap, so memory is bounded by the case's ten-second wall alone here,
  and by RLIMIT_AS inside `bwrap` on Linux, whose words (`--unshare-all`, `--die-with-parent`,
  `--clearenv`, the scratch bound read-write and the system read-only) are **unrun on this Mac**
  and the CI's `thesis-attest` leg judges them. **Narrowed on the ffi-pragmatist's measurements**:
  `(allow process-exec (literal "<binary>"))` in place of `process-exec*`, which runs the binary
  and refuses `/bin/sleep`; fork is already denied; `/Library/Keychains` and `/Library/Preferences`
  denied beside the home. **What the profile cannot hide is another process's environment**
  (`sysctl(KERN_PROCARGS2)` reads it with `sysctl-read` gone), so the secrecy of the key rests on
  R10's rule that no process holding it ever runs a model's program, and not on this profile. **The
  grader refuses to run a case where neither tool exists** (`no_sandbox`) **and where a
  `HEROES_*_KEY` stands in its own environment** (`key_in_environment`). The identity is the
  runner's user, the filesystem the scratch alone, the network none. *Prior art*: none.
- **R12. Part 11's text and the template.** Items 2 and 4 of Part 11 are replaced by the
  proposal's paragraph as it stands above, amended by R1 (the seven meanings and the abort's
  message), R2 (the mid-p and pass@t), R6 (per-task GUIDs, the cutoff reading), R8 (the checklist),
  R9 (route A) and R10 (the trigger and the secret); **today's text stays as history under it**:
  *20 frozen tasks × 5 samples, Wilson intervals, API-only, single-turn, spec-only* (the Wilson
  interval stays for the per-arm columns, Brown, Cai and DasGupta's warrant). **The prompt's
  identity is two shas**: a `template sha` of `docs/metrics/first-try.md` with its placeholders,
  and a `layout sha` of the request body's skeleton (roles, `system`, `cache_control`, the feedback
  placeholders unsubstituted); a run differing in either is a new series. **`docs/metrics/first-try.md`
  is amended into the multi-turn template**, its turn-1 words unchanged, R5's four fixed feedback
  blocks named after a second line of three dashes, the layouts and the two shas in its header;
  the coordinator made the amendment during the sitting on the spec-warden's reading, the
  instrument refuses a template missing a block, and `plan` prints both shas (`template=73c376a1`,
  `layout=cached/185cc0a2` on 2026-10-20); the trial and the held-out run both run on that sha,
  and the metric-2 series that never ran breaks nothing. A live run writes `provenance.json` before
  its first request and refuses a tree git cannot describe or a dirty one. **§1.2's two columns**: *program tokens* by `count_tokens` on the extracted program,
  never `usage.output_tokens`; *rewrite rate* as mean(turns-to-green − 1) over the converged, per
  arm, the non-converged apart. **§1.6's panel-011 sentence is amended, not deleted, on the day the
  first measurement lands**, its shape pre-authorised here: *Metric 2 first ran on YYYY-MM-DD
  (measurement NNN): the rewrite rate read X on Sonnet 5.5 strict and Y control, the round-trip's
  median Z tokens; the formula is audited from that day*; and §1.6's *an ablation by section* is
  amended to name what the run ablates, the productions. *Prior art*: **Taken from** Brown, Cai and
  DasGupta 2001 (the Wilson interval for small n, kept for the columns).
- **R13. The predictions**: panel 007's two are void as written (the spec states the continuation
  rule at lines 11 to 19) and lapse by panel 046's R2, the warden's scoreable count (zero depth-0
  illegal breaks in completions) kept; panel 133's prediction 3 and issue 1005 are scored by R6's
  variant contexts at the held-out run; issue 35's bucket is R3's `compiled_wrong`; the seats'
  predictions below are scored at the trial and at the held-out run.

**What a veto would compel**: none was cast. The compiler-engineer's `object` is met by R1's
complete table (its condition (a)); the spec-warden's three objections are met by R12's two shas,
the amended *ablation* sentence and `count_tokens` on the program.

## Predictions to score

- **compiler-engineer** (at the first held-out run, from its provenance records): if `build
  --permissive` lands as a filter with no lowering-side meaning for `marker_mismatch`,
  `ufcs_on_mutable`, `declared_twice` and `bound_unit`, the control arm's `build_failed` bucket is
  non-empty and every member carries one of those four codes under strict `check --json`;
  falsified if the bucket is empty over 40 tasks × 3 samples or holds a program none of the four
  names. **R1 lands the four meanings, so the prediction's premise is not met and it is scored
  against the trial's judge-only control instead**, where `build --permissive` does not run: the
  bucket is then expected empty by construction, and the prediction is scored *not applicable* with
  that reason, unless a held-out sample reaches `build_failed`, which R1 says none should.
- **spec-warden** (at the first held-out run): the strict arm's correction round-trip has a median
  between 700 and 1,300 tokens on Sonnet 5.5 by `count_tokens` on `claude-opus-5`; the full
  diagnostic fed back a mean of 150 to 300 tokens, `--brief` under 100; the spec's cache-read share
  under 10% of the run's USD at 1,500 output tokens, under 4% at 6,000.
- **llm-ergonomist** (at the trial, on the three tasks it wrote): first attempts that compile and
  that pass the hidden cases, 45/30, 50/30, 70/60 per cent for tasks 1 to 3; the three rules
  broken most, in order; repair 1 identical under the brief message and repair 2 different.
- **ffi-pragmatist** (at the milestone's close, on this Mac): with `HEROES_TASKS_KEY` exported in
  the grader's process, a C program calling `sysctl(KERN_PROCARGS2)` on the grader's pid inside
  `thesis_sandbox.hero`'s profile prints the key's value; with the archive opened by a separate
  invocation that exits before the first `build`, the same program prints `hits=0`. Falsified if
  the profile as written hides it.
- **the critic's two measurables, at the landing**: the thesis root's 92 tests pass in the lane
  with the amended template beside them, and `hashes.txt` for a sealed suite equals what `seal`
  prints after writing the canaries and not what `tasks` printed before.
- **historian**: none of its own; its four conditions are scored as events (a per-task GUID
  completed; a provider page permitting unattended subscription use; a never-public key's archive
  found in a corpus; Aycock's result failing to transfer on the productions variant).

## Author's verdict

Pending: the open decision https://github.com/heroes-lang/heroes/issues/2159 queues it, and the
author's reading is recorded here when given.

**Read by the author on 2026-10-21**, in these words, *ratifico 226 con R8 corretto*, meant as:
*I ratify panel 226, with R8 corrected as the coordinator recommended*. The recommendation, put the
same day in the session that ran the trial: R1 as built, every one of the 27 codes with its meaning
in the control arm; R9's route A, the API's number the thesis; R10's opening within 30 days of the
first `v*` release after the held-out run, or of the suite's being burned, and the key in the
reviewed `thesis-attest` environment; R2 to R7 and R11 to R13 as written; and **R8 corrected**:
n fixed at 40 held-out tasks plus 20 in reserve, 3 samples each, the power declared unknown and
the inconclusive outcome named before the run, because measurement 050 found no discordant pair
of 90 at `green@3` strict against control, so the trial gives no discordance to size n on, which
R8 as written asked of it. design.md Part 11 and the milestone page carry the corrected rule.

**RATIFIED, 2026-10-21**, R1 to R13 as written above with R8 amended as the reading above
records, the author answering in conversation that day. (This line added 2026-10-22: the reading
above was written in a shape `records/verdicts` does not read, `Read by the author`, and the
check reads an answer beneath a pending stub only as `RATIFIED,`, `AMENDED,` or `OVERTURNED,` and
its date.)

## Process notes

- The critic's first pass repaired the briefs in the places `critic-briefs.md` names before any
  seat was launched (`/panel` § 3c). The seats ran in parallel in their own copies; two paid calls,
  both named in the briefs: the llm-ergonomist's blind session (0.78 USD, under its 4 USD budget)
  and the ffi-pragmatist's five-cent `claude -p` probe.
- The account's session limit stopped the four seats once (reset 04:30 Europe/Rome); each was
  resumed from its own report file as § 3c asks, none from another's findings. The
  ffi-pragmatist's first run was stopped by the coordinator at 06:45, two hours inside its
  `claude -p` probe with no return; the resumed seat hit the limit again (reset 09:30) and
  finished at 10:25, from its own file both times.
- The coordinator worked in a copy of its own for the whole sitting (`scratchpad/226-coordinator/`)
  and the lane's tree did not move between the briefs and this file; the instrument's modules land
  in the lane after it, as steps 2 to 9 of the milestone, each gated by the harness's own tests.
- The compiler-engineer's prototype (`scratchpad/226-compiler-engineer/`, originals in
  `226-ce-orig/`, witnesses and log in `226-ce-witness/`) is the route R1 adopts; its diff is in the
  evidence archive and is re-made in the lane at the landing step, never copied blind.
- Two corrections to the shared brief, recorded here as the historian found them: Anthropic's
  consumer decision deadline reads October 8, 2025 on the page today, not September 28; Codex's
  *Include environments* setting is named by no OpenAI page read on 2026-10-20 and stays a
  question. One from the spec-warden: `STALE` was the seed-built compiler's reading, the real
  count being 10,286 unchanged.

## At the landing, 2026-10-20

What this file promised the landing would read or re-measure, each answered by a command run that
day in the lane or against GitHub, and what the landing found that the sitting had not.

- **The site's first publication** (R6 promised it from the `site-v*` tags): **no `site-v*` tag was
  ever made**, so the promise named an instrument that does not exist (`git tag -l 'site-v*'` and
  `git ls-remote --tags origin` both empty; the `release-site.yml` workflow that would read one
  exists and has never fired). The site went out on 2026-08-28 behind a gate that answered every URL
  with a holding page and `x-robots-tag: noindex` (`53f7eb17e`); the gate came down with the deploy
  of **2026-09-02 23:43 UTC** (run 33696350597, whose head commit GitHub records as *site: the gate
  comes down and the pages say the repository is still private*), and the repository went public on
  **2026-09-08**. The sentence *never saw Heroes* rests on those two dates and on the providers'
  declared cutoff of June 2026, which precedes both.
- **The blind seat's credential** (R9): **no API key reached it**. The command that launched it
  (this session's transcript, 2026-10-20 00:34 UTC) passed no key and no `--bare`; the shell it ran
  in holds no `ANTHROPIC_API_KEY`, `ANTHROPIC_AUTH_TOKEN` or `CLAUDE_CODE_OAUTH_TOKEN` (asked by
  name, set or unset, the profile files last changed 2026-10-19 20:41, before the run); no
  `apiKeyHelper` and no stored key are configured. Claude Code on this Mac is logged in through an
  account billed as a **Claude Team subscription**, and the run ended on that subscription's session
  limit (`api_error_status` 429, *You've hit your session limit*), the same limit that stopped the
  four seats at the same hour. So the blind seat ran on the Team subscription, on the assistant's
  tasks, the permitted side of R9's line. **Whether a Team plan's terms put a `claude -p` row on the
  API's side of that line is a question**: no provider page was read for it on 2026-10-20, and the
  trial (step 11) reads one before a harness row is cited.
- **The instrument's size**: 17 modules, **5,724 lines** under `tests/harness/thesis*.hero` in the
  lane (`wc -l`), against the 4,547 the critic counted in the saved archive: the canary probe
  (`thesis_canary.hero`, R6's `canary-probe` word) and the repairs below landed after that count.
- **The critic's first measurable**: the thesis root's tests read **101 passed, no test skipped** in
  the lane, with the amended template beside them and stage 2 named as the compiler the tests spawn
  (`HEROES_COMPILER=./heroes-next`, as the CI's step names its own stage 2): true, at a larger count
  than the 92 predicted, the tests the landing added being counted in it.
- **The critic's second measurable**: as built, `seal` printed the archive's digest and no digest of
  the suite, so *`hashes.txt` equals what `seal` prints* could not be scored. `seal` now prints the
  suite's digests as sealed, its canaries written (`thesis_seal.frozen`), and its test asserts the
  three facts the measurable names: those lines differ from what `tasks` printed before the
  canaries, equal what `tasks` prints of the canaried folder, and equal what `tasks` prints of the
  folder the archive opens into. Scored true on that test.
- **R1 re-made in the lane, never copied blind**: 30 files under `selfhost/` and `runtime/`,
  +450/-82 lines (`ir/flatten.hero` +129/-24), against the prototype's +504/-132 over 30 files. The
  landing's first pass found **26 lines of two FFI programs' emissions moved past the sources key**:
  the runtime's two new prototypes named their parameters `tag`, `names` and `shown`, words the
  guard list did not hold, so `tag` entered it and the two programs whose C names a field `tag`
  gained a `push_macro` guard. The prototypes now name `index` and `text`, words the list already
  keeps; the strict build's emitted C moves by the sources key alone (618 files of `tests/emission/`
  re-blessed with one line moved in each, the sources key, and 0 lines moved past it), which is what
  the step's gate asked (*the C without the flag must not move*).
- **The grader built the control arm strict**: `thesis_grade.graded` ran `build` without the flag in
  every arm, so a program `check --permissive` accepted and the strict build refused ended
  `build_failed` and was excluded, the very programs the thesis is measured on; `dry` could not see
  it, the reference solutions being strict programs. The control arm now builds with `build
  --permissive`, and a test grades two programs the strict build refuses: a dropped rule's
  (`unused_binding`) is judged by its output (`compiled_wrong`), a `match` missing a case aborts
  `panic: this match has no arm for .empty` (`abort`).
- **The report had no flow and counted `build_failed` in n**: the adjudication above says such a
  sample is excluded and reported in a table of its own, and `thesis_report.hero` counted it among
  the samples and wrote no flow at all, though `docs/metrics/tasks.md` names the flow as where every
  exclusion is counted. It now writes the flow, attempted, answered, extracted, compiled, ran,
  green, an outcome the grader does not name counted apart, and lists every `build_failed` sample by
  key outside every denominator; a test checks both on hand-counted numbers. And a kept verdict with
  no turn, which every reader indexes by its last, is refused by its key when read.
- **The first pass's full net**, sequential, read 8,918 passed and 2 failed in 3,217.69 s real (user
  1,733.31, sys 515.38): `records/names`, a comment of `tests/harness/shell.hero` that split
  `M-thesis-harness` across two lines, and `records/citations`, `thesis_sandbox.hero` citing
  `docs/panel/226-…` and the held-out README citing an archive the freeze has not written yet. Each
  was rewritten, the README's command taking a placeholder for the archive rather than a date to
  quiet the check, and the second pass's `records` read 26 and 0.
- **A correction to § Predictions to score**: the compiler-engineer's prediction says it is scored
  against *the trial's judge-only control*, and none exists: R1 was built during the sitting, and
  the resolution's own words are that the trial runs the control arm whole. Its premise, a `build
  --permissive` with no lowering-side meaning for four codes, is not met, so it is scored *not
  applicable* at the first held-out run, unless a sample there reaches `build_failed`, which the
  report now lists by key.
- **The compiler is not slower**: `check selfhost/main.hero`, the trunk's stage 2 on the trunk
  against the lane's on the lane, three runs each alternated on a still machine: real 6.56, 6.56 and
  6.56 s on the trunk's stage 2 (sources 7792ecfe50a531e8) against 6.56, 6.55 and 6.56 s on this one
  (sources 79fe404274caacd0), real equal to user plus sys each time.
- **The fixpoint**: stage 2 and stage 3 emit the same 26,735,934 bytes of `selfhost/main.hero`'s C.
- **The gates**: the compiler's own tests 1738 passed, 0 failed; the net's own tests 414 passed, 0
  failed; the full net 8,920 passed and 0 failed, its 32 suites four at a time, `run` first and
  `cache` alone after; `dry` over the assistant's suite, 3 tasks and 0 red in the strict arm and in
  the control arm.
