# Panel 226, shared brief: how the thesis is measured the first time, metrics 2 and 4 of Part 11

**Convened 2026-10-20** at the opening of M-thesis-harness (row 70), by the author's answers of
2026-10-19 in the session that planned it, meant as *decide the open points first, then the panel,
and the panel may overturn*, and by issue 2079
(https://github.com/heroes-lang/heroes/issues/2079), filed 2026-10-14 at the chain's re-ordering,
which asks for a sitting on design.md Part 11 before the instrument is built. The session is
unattended (the author's instruction of 2026-10-20, meant as *start the loop and run until the step
is done*), so every open point below took its recommended answer (CL-002) and this sitting is what
may overturn it; the author's verdict is a reading. **Lane: full panel**, five seats and a
completeness critic before and after them: the proposal amends design.md Part 11, a part of the
design, and the llm-ergonomist's experiment is the one measurement no other seat can make.

**The tree**: the lane `thesis` at `a0a8d1249` (`git log --oneline -1`, run 2026-10-20), the trunk
`main` at `4d459093d` plus step 0 (the ROADMAP's row 70 OPEN and the milestone page's § Opened
2026-10-20); `git status --short` empty. Every compiling seat has its own copy, made by `git archive
HEAD | tar -x` into `<scratchpad>/226-<seat>/` (12,801 files each, `find -type f | wc -l`), and
builds its own compiler there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (7.93 s
real on this Mac today), then `./heroes build selfhost/main.hero -o heroes-next` for stage 2, which
is the compiler a gate judges with. No seat reads, builds or runs in another's directory, prints a
credential, or starts a paid run its brief does not name.

## What exists, measured 2026-10-20 (the command beside each number)

- **The thesis's two halves.** Metric 3 runs (`heroes mutate`; measurement 024: 19,286 of 20,614
  mutants killed under `check`, 16,765 under `--permissive`, over 120 programs). Metric 2 has never
  run (`docs/measurements/007:24-25` says so of metric 2 alone; issue 816) and metric 4 has no record
  in `docs/measurements/` (`grep -rl 'turns-to-green' docs/measurements/` prints nothing).
- **`--permissive` is a flag of `check` alone**: one line in `selfhost/cli/table.hero`, line 94,
  inside `check_flags` (lines 90 to 97) (`grep -n permissive selfhost/cli/table.hero`). `build`,
  `run` and `test` do not take it. Nothing has ever built or run a permissive binary: the metric
  filters diagnostics in process (`selfhost/mutate/score.hero:147-153`), the recovery plan spawns
  `check --permissive --json` (`selfhost/mutate/judged.hero:70`).
- **The thesis rules**: 27 codes in `selfhost/diag.hero`'s `is_thesis_rule`, lines 192 to 221
  (`sed -n 192,221p selfhost/diag.hero | grep -oE '"[a-z_]+"' | wc -l`), `non_exhaustive` the
  last, added 2026-10-16 (issue 2078, the author's *4a*); a third category stays refused under
  `--permissive` (lines 181 to 191, panel 192's R4). Witness cases: 21 `.hero` under
  `tests/golden/permissive/` (`ls | wc -l`).
- **`check` speaks one stage at a time**: `spoken`, `selfhost/cli/check.hero:265-275`, returns the
  first stage with diagnostics among parse, modules, names, types.
- **The hidden-case format does not exist**: a run golden holds stdout, then an optional
  `!sanitizer: <needle>` line and an optional ending, `!panic: <message>` or `!exit: <code>`
  (`tests/harness/expectation.hero:12-30`; the critic counted 129 `!panic:`, 9 `!exit:`, 40
  `!sanitizer:` and 37 files with two such lines); no case carries arguments or input files
  (`suite_run.hero:183,190`).
- **The harness's watchdog is fixed**: `hero_run_limit(seconds: WATCHDOG_SECONDS)` at
  `tests/harness/shell.hero:285`, the constant at line 751, asserted 600 at line 1287; `run_limited`
  (line 162) adds a byte cap and keeps the 600 s. The runtime's limit kills the child's process
  group, `kill(-child, SIGTERM)` at `runtime/parts/run.c:1556`, `SIGKILL` at 1569, and answers 124
  at 1600 (POSIX; 1400 on Windows). **The coordinator drafted `shell.run_bounded(scratch, program,
  words, seconds, bytes, stdin_path)` in a copy of its own during this sitting**, `spawned` taking
  the seconds; it is not in the frozen tree and lands after the synthesis; the seats judge the
  design, not its presence.
- **The spec**: 26,601 bytes, 470 lines, sha256 `212264059c985c2e…` (`wc`, `shasum -a 256`);
  **10,294 tokens on `claude-haiku-5-5`, `claude-sonnet-5-5`, `claude-opus-5-5` and
  `claude-fable-5-1`, 10,292 on `claude-opus-5`** (`POST /v1/messages/count_tokens`, the spec as
  one user message, run 2026-10-20 from the trunk with the key of `.env`). **The pinned count is
  stale**: `heroes measure` prints `STALE`, the ledger's last real count being 10,286 on
  `claude-opus-5` (2026-10-18) for an earlier digest, so today's 10,292 is +6 on a spec that moved
  since and the ledger owes a refresh. 46 lines open a production (`grep -cE '^    [A-Za-z]+ += '
  spec/heroes-spec.md`). The frozen prompt template
  `docs/metrics/first-try.md` is 573 bytes, sha256 `b7f06aab65cd9790…`, with `{{SPEC}}` and
  `{{TASK}}`.
- **The frozen recovery plan** (`tests/golden/recovery/`): 14,209 singles, 15,737 pairs, 14,209
  base rows, 1,356 corpus messages (`wc -l`); 4,120 singles sit on `tests/golden/run/` files
  (`grep -c '"tests/golden/run/'`), whose `.expected` stand in the live tree. No word of
  `mutate --recovery` writes a mutant's text into its records (`selfhost/cli/mutate_recovery.hero:16-23`);
  during a replay `judged.plant` writes each mutant into the mirror and `uproot` removes it
  (`judged.hero:62`), so a mutant is the hunks of a single applied to `corpus/<file>.frozen`
  (`selfhost/mutate/hunks.hero:1-26`), which the instrument applies itself.
- **Full-form diagnostics pinned**: 39 `.expected` under `tests/golden/full/` (`ls | wc -l`).
- **Tools on this Mac**: curl 8.7.1 (`--variable` and `--expand-header` need 8.3, which is how
  `selfhost/cli/refresh.hero:59-83` keeps `ANTHROPIC_API_KEY` out of argv and of every file);
  OpenSSL 4.0.3 (brew) and LibreSSL 3.3.6 (`/usr/bin/openssl`), both with `-pbkdf2`, `-pass` and
  `-a` (`openssl enc -help`, run 2026-10-19); `/usr/bin/sandbox-exec` present; `claude` 2.1.292;
  `codex` absent (`command -v`). The harness has 77 modules, 21 of them `issue_*`.
- **What a Heroes program can do without `extern`** (spec § 11): `read_file`, `write_file`, `args`,
  `args_checked`, `exit`, `print`; no stdin, no clock, no spawn. The harness spawns through an
  `extern "hero_os.h"` group of its own in each module that spawns, as panel 033's R5 asks: five
  today, `shell.hero:38`, `cases.hero:23`, `issue_pace.hero:31`, `issues.hero:89`,
  `suite_cache.hero:89` (`grep -n 'extern "hero_os.h"' tests/harness/*.hero`); and a program under
  `tests/harness/` cannot `use` anything under `selfhost/` (spec § 1).
- **Prices** (the `claude-api` skill's table, cached 2026-10-06, a document and not a measurement;
  the author's reading of Anthropic's pricing page on 2026-10-14 gave the same for Haiku and Opus):
  Haiku 5.5 $0.10 / $0.50 per MTok in / out for prompts up to 100K tokens ($0.50 / $2.50 beyond),
  Sonnet 5.5 $2 / $10, Opus 5.5 $4 / $20, Fable 5.1 $10 / $50; cache reads $0.20 per MTok on Sonnet
  5.5 and Opus 5.5 (0.1× and 0.05×), $0.25 on Fable 5.1 (0.025×), Haiku's own not confirmed; cache
  writes 1.25× with a five-minute TTL. **Thinking is billed as output on every one of them and cannot
  be switched off on Opus 5.5, Sonnet 5.5 (except `between_tools`) or Fable 5.1**; effort defaults:
  `medium` on Opus 5.5 and Haiku 5.5, `high` on Sonnet 5.5; `temperature` rejected or non-default on
  all four (the same skill). The models' training cutoff is June 2026 by the milestone page's
  sentence of 2026-10-14 (`docs/roadmap/milestones/M-thesis-harness.md:60`), a statement no
  provider's document in the tree supports: the historian verifies it. The repository is public
  since 2026-09-08 (M-open-repository).
- **Terms of use, read 2026-10-19 in two web searches and to be verified by the historian**:
  OpenAI's API does not train on inputs by default (help.openai.com/en/articles/5722486); ChatGPT
  consumer accounts train unless *Improve the model for everyone* is off, and Codex has a separate
  *Include environments* setting (same page); Codex's non-interactive `codex exec` with *Sign in with
  ChatGPT* uses the plan's included usage (help.openai.com/en/articles/11369540,
  developers.openai.com/codex/noninteractive); Anthropic's consumer plans train by default since
  2025-09-28 unless opted out, business and API tiers do not (secondary sources only; the official
  pages were not in the results).
- **A blind reading of the plan** (an LLM-evaluation reader given the plan alone, 2026-10-19, in
  the coordinator's session; its report is in that session's transcript and not in the tree) found
  23 confounds, 18 fragile claims and 25 underspecified points; the ones the plan took are in the
  milestone page § Opened 2026-10-20 and below as the defaults.

## The proposal, as design.md Part 11 would read (the diff the sitting rules on)

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

The two gradings of today's item 2 stay as the turn-1 columns; the one-turn, spec-only context of
today's text becomes the turn-1 row of every conversation. The control arm's paragraph gains: *the
control arm builds and runs*.

## The questions, each with the default taken and what the other route would be

- **R1. The control arm builds.** `build --permissive` (and `run`, `test` with it), with a runtime
  meaning for each of the 27 codes, pinned in a table each row of which says what the most common
  permissive language does, and an abort of its own message, since `hero_unreachable`'s says *this
  is a compiler bug, please report it* (`runtime/parts/panic.c:46`), which a permissive abort is
  not (the critic's finding) (a `match` missing a case aborts at the unnamed case; `_` is a catch-all;
  missing, wrong or needed labels read by position; shadowing: the inner binding wins; a discarded
  value falls; an unused binding is nothing; `continuation_outside_brackets` as the control parse
  already reads it; `marker_mismatch`, `ufcs_on_mutable` and `declaration_in_arm` need the sitting's
  sentence). **Default: yes, for the held-out run; the judge-only control, declared biased, for the
  trial** (the trial measures discordance, tokens and format leakage while the flag is built).
  Other routes: the control measured only on programs both arms accept; **a control that keeps the
  27 diagnostics as warnings the model sees and still builds** (the critic's route), which measures
  a third thing, the rules' presence without their refusal.
- **R2. The primary.** As the proposal states. Other routes: `green@1` (metric 2 as written, which
  the strict arm can only tie or lose); mean turns (censored in the control where the thesis bites).
- **R3. Three failure classes**, the thesis on the silent one; an abort with `panic:` is loud.
  The edges the critic named: a correct stdout with a non-zero exit, and stderr noise at exit 0, are
  both `compiled_wrong` by `expectation.unmet`'s own reading (the ending is part of the expectation,
  and a silent ending is silent on stderr too); a run-time `timeout` is its own class. Other: two
  classes with the abort among the silent.
- **R4. Three arms**, `--brief` the third, so the message's value and the rules' value separate.
  Other: two.
- **R5. Symmetric feedback** as the proposal states; a fixed sentence per non-compiler bucket
  (`no_program`, `format_slip`, `truncated`, `refused_extern`, `hole`, `timeout` on the visible
  example) in the frozen template; and two buckets outside the denominator, counted and never
  resampled in silence: `api_refusal` (`stop_reason: refusal`, HTTP 200) and `transport_error`
  (429, 529, a `curl` failure, a timeout: retried as `thesis_pace` says, excluded past the retries). Other: the compiler's message alone, the control told nothing on a program that compiles
  (the plan's first version; a blind reader found it wins by construction).
- **R6. Contexts and contamination**: `spec`, `none` reported as *knowledge of the language* with the
  rule of three, variants by path (the spec without its productions, panel 133's prediction 3; with
  one worked diagnostic, issue 1005) on ten tasks with Haiku and Sonnet at the held-out run; a suite
  GUID and a per-task GUID in a fixed comment of the template, a `canary-probe` word that asks each
  model to complete the prefix before a run, *exact completion = suite burned*; the Heroes-only
  spellings grepped in code blocks alone and for non-word tokens; declared cutoffs in provenance.
- **R7. Models and sampling**: the four Anthropic models, the provider's default effort per model
  written out (the blind reader: *medium* is not the same effort on different models; *equal* as an
  exploratory variant); thinking on, blocks kept in the transcripts; `temperature`/`top_p` written
  where accepted else recorded as default; `max_tokens` at least 32,000, revised on the trial's
  output distribution; no `fallbacks`; `created_at` and the echoed `model` recorded; a retired model
  stays in the published sentence with its date. **The forks are proven**: the three arms continue
  one turn-1 transcript holding thinking blocks, and the trial sends the first fork under
  `prefix_mismatch_behavior: "error"` (beta `thinking-binding-controls-2026-08-01`) so that an
  arm silently running with dropped thinking is impossible rather than unnoticed (the critic's R7).
- **R8. Tasks and n**: 40 + 20 reserve, 3 samples; n confirmed from the trial's discordance and ICC
  (orientation: with 120 pairs 10pp are 12 net pairs, McNemar p ≈ 0.023 at 18 against 6 discordant,
  ≈ 0.065 at 24 against 12; three correlated samples at ICC 0.5 count as 60 pairs; the critic
  re-ran the arithmetic); Haiku's 100K price tier and the five-minute cache TTL in the budget; the task rules (one `main.hero`, no
  `use`, no `extern`, inputs by `args()` and at most three files, exact stdout and exit code, 4 to 8
  hidden cases, English prose with no Heroes spelling and no construct named, a reference solution of
  25 to 80 lines, a hand mutant per tempted mistake and the case that kills it), a difficulty target
  from the trial, a blind second reader, pre-registered adjudication (ambiguous → next reserve;
  missing sample → excluded from all arms; `build_failed` → excluded, refiled).
- **R9. Transports and terms.** A held-out task on no consumer transport until its terms document no
  training; the `harness` rows on the assistant's tasks. **The author's own question: is the thesis
  measured the API's number with the harness rows in a table of their own (A, the coordinator's
  recommendation, Part 11 unchanged in its protocol), or the harness's alone, `claude -p` against
  `codex exec` (B, fair between two harnesses but not Part 11's frozen single prompt, and it spends
  the plans' quota)?** The sitting decides; the author ratifies by reading.
- **R10. The sealed archive, the author's decision; two questions the author left to the sitting.**
  (i) **When does it open**, the key committed and the tasks readable in git's history: at the
  series' close, when the suite is burned or retired, the key meanwhile given on request
  (recommended); at M-publication-gate; at the first `v*` release after the first held-out run.
  (ii) **Does the key also stand in a GitHub Actions secret** (an environment with a required
  reviewer) for a `thesis-attest` job that decrypts in the runner and prints counts and hashes alone,
  on a failure the task's sha alone (recommended), or does the key stay local and the attestation by
  hand? Beside them: three archives and three keys, the ciphertext's sha256 beside it (CBC does not
  authenticate), the `-iter` count written, the licence named, the delivery register.
- **R11. The sandbox** the model's program runs in: on this Mac `sandbox-exec` denying everything
  but execution and writes inside the sample's scratch, no network, `ulimit`, an emptied
  environment, the grader refusing to start outside it; on Linux `bwrap` or `unshare`. The identity,
  filesystem and network are the question.
- **R12. Part 11's text**: the diff above, and what of today's text (20 × 5, Wilson, single-turn) is
  kept as history under it.
- **R13. The predictions**: panel 007's two (void as written: the spec states the continuation rule
  at lines 11 to 19; the warden's count, zero depth-0 illegal breaks, stays scoreable), panel 133's
  3 and 4 and issue 1005 (the variant contexts), issue 35's bucket (`compiled_wrong`, named).

## What is to be measured, not argued

Each seat's brief names its own; the critic's first pass checked these framing facts with commands
in its copy before any seat started (`docs/panel/226-reports/critic-briefs.md`, 218 lines; every
repair it asked is in this text). A seat that builds builds in its own copy **and judges with stage
2**, `./heroes build selfhost/main.hero -o heroes-next` from its seed-built compiler: the two carry
different `sources` keys (`5f45d6514b22d650` the seed's, `7792ecfe50a531e8` the tree's) and the
hook in front of a gate compares the tree's with the judge's. Two paid calls and no other: the
llm-ergonomist's blind session (at most 4 USD) and the ffi-pragmatist's five-cent probe of
`claude -p`, each named in its brief.
