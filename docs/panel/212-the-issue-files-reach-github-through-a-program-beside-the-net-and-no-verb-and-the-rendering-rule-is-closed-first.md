# Panel 212: the issue files reach GitHub through a program beside the net and no verb, and the rendering rule is closed first

**Convened and written 2026-10-11, on the tree frozen at `09060ea3`.** The author decided that day to
transcribe (the *7a* of 2026-10-05: the sitting on the verb is convened when the author decides to
transcribe) and asked to close M-issue-files. **Full panel**: the tool's surface is architecture
(CLAUDE.md § 4). Seats: compiler-engineer, ffi-pragmatist, historian, spec-warden, llm-ergonomist, and a
completeness critic twice. **Resolution: provisional, author ratification pending.**
Briefs: `docs/panel/212-briefs/` (shared brief, five seats, the critic, five samples, two scripts).
Reports: `docs/panel/212-reports/` (the five seats, `critic-briefs.md` and `critic-reports.md`, and
`llm-ergonomist-run1-unread.md`, a first run that could not open its inputs and is kept as it is).

## The proposal, verbatim

```
heroes issues render <file>          print the title, labels, milestone, body and closing comment the rule
                                     gives that file, as one JSON object; reads one file, writes nothing,
                                     needs no network and no account
heroes issues plan [<path>...]       render every issue file in name order and say what `push` would do
                                     against what GitHub already holds; reads GitHub, writes nothing
heroes issues push [<path>...]       create, in name order, each issue GitHub does not hold, comment and
                                     close the closed ones, and write the new address into each created
                                     file's card (`github:`); stop at the first refusal; a rerun resumes
```

With the author's four standing instructions of 2026-10-06 (`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`):
the transcription is the last act; each issue carries its own days, *as GitHub's own dates where an
endpoint can set them and in the body's first line always*; a closed issue carries a closing comment;
and whether GitHub's import endpoint still sets `created_at` and `closed_at` is asked of the endpoint
before the first issue is made.

## What the sitting found before it judged

The sitting is where the proposal stopped being the proposal. In order of what each changed:

1. **The completeness critic's first pass over the briefs found 14 disagreements with the tree**, and
   among them the sentence that would have sent the engineer to rebuild what exists (*selfhost has no JSON
   writer*: `selfhost/cli/json_text.hero` and `selfhost/mutate/json_fields.hero` write it), the network
   precedent named wrongly (`refresh.hero` spawns `curl`; `count_tokens.hero` is the offline half), a
   median title length of 71 that was 101, and a copy method (`cp -r` of a worktree) that would have shared
   the lane's index with every seat. The briefs were repaired and the seats launched on the repaired ones.
2. **The rendering rule has seven gaps** that the first samples had hidden (D1 to D7 of `00-shared.md`).
   The `Defect NNN:` prefix would be doubled on 107 of the 109 defect cards that already have it; 47 defect
   cards have no number (41, 45 and 47 are three counts of three readings, none a fact about the cards);
   `closed` has no source; two `self` cards do not resolve by the rule's command; and 670 cards have no
   title by the rule as written. **The rule was handed to the seats as settled; the sitting finds it is
   not.**
3. **The seats built, and measured.** The engineer built `render` as a verb (775 code lines in 7 modules,
   2014 of 2014 cards rendered, none refused) **and as a Heroes program under `tools/`**, byte-identical
   on all 2014 (`cmp`, the critic re-ran it). The critic composed the two things the route depends on
   (a 54,818-byte standard input through `shell.run_fed`, and `gh api -i` read back); their composition
   is the one thing not run.
4. **The historian read GitHub's limits**, and the sitting's arithmetic changed: **80 content-generating
   requests a minute and 500 an hour**, serial requests at least a second apart, `retry-after` obeyed
   (the sitting re-read the pages): **5,918 requests are about 11.8 hours, not minutes**. The Import API is
   an unofficial preview that sets the dates and does not notify, reported working in February 2025 and
   attested in no 2026 source.
5. **Two things the sitting was not asked and has to carry.** The ffi-pragmatist **printed the author's
   GitHub OAuth token** in a tool output (it says so in the first lines of its report; no file of the
   repository, any lane or any scratch folder holds it: searched for the shape of `gho_`, `ghp_` and
   `github_pat_`, 0 files). **And `.claude/settings.local.json` pre-approves `Bash(gh api
   repos/heroes-lang/heroes/*)`, `gh api` with a `-f` field defaults to POST, and `guard_bash.py` does not
   name `gh` at all**: today a GitHub write from an assistant's shell would not ask.

## Verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to R1, no veto | design.md §1.1, §1.7, Part 5, §3.5 | render alone 775 code lines in 7 modules, +10 edited lines, +12,507 lines of C in the seed (+0.94%); `selfhost/main.hero` is 300 code lines the suite's way, so any added line is red and a seam is the remedy; plan and push 400 to 500 more (inference) | if the verb lands, 1,150 to 1,400 code lines in at least 9 modules under `selfhost/cli/`, `main.hero` needing a seam in the same change | approve `render` as a verb if it is also a permanent `records` check; approve `plan` and `push` in `selfhost/` only with a life after this transcription; drop the objection to the surface if it takes flags (`issues [path] [--plan\|--push]`) |
| ffi-pragmatist | **approve** spawning `gh api`; **veto** libcurl in the compiler binary; `curl` an acceptable second with an objection to its token handling | design.md §1.11, §4.19; spec §13 | route A costs the boundary nothing: `hero_run_go` takes a standard-input path, so `--input -` needs a wrapper parameter and no C; libcurl needs six modules (the variadic `curl_easy_setopt`), `-lcurl` on the bootstrap line, and the Windows leg has no libcurl (`ci.yml:439, 469`) | under A, 0 lines change in `runtime/` and the seed's link line; falsified if the first implementation edits either | withdraw approval of A if the machine has no `gh`, or if `-i`, `--jq` or `--input -` change across `gh` versions (2.102.0 measured) |
| historian (advisory) | **object** to three premises, not to the verb | the briefs' F9, R4 and F4 | the transcription is 7.9 to 11.8 hours at GitHub's documented limits; an import route would be about 4 | a 403 without `Retry-After` is documented as possible; two community threads report one after 100 to 150 creations | a GitHub page showing the import endpoint removed leaves the rule standing; a live write showing the dates are set amends it |
| spec-warden | **object** to R1; **veto** reserved on Principle 0's burden | CLAUDE.md §2, §10; design.md §1.2, §1.6 | spec 0 tokens (real 10044, recorded, not re-priced); CLAUDE.md 0 (it could take 21 vendored tokens before STALE); the help text about +100 vendored, budgeted by nobody | a verb makes `./heroes --help` at least 86 lines (80 today); R11 leaves `Command(` at 13 and the site untouched | approve `render` alone as a verb if the author decides the capability in words and `plan` and `push` leave the binary; veto if the sitting says the stopping rule or Principle 0 admits the verb; veto if a push saying *fourteen verbs* precedes the author's ratification |
| llm-ergonomist | **object** (its own reading void by its definition) | the thesis, as a reader | 13 of 20 cells clean, 5 with a caveat, 2 not found (the problems of samples 02 and 05) | 18 of 20 clean after its changes 1 to 5 (not re-run) | none stated |

The ergonomist **was not blind**: `CLAUDE.md`, the memory index, `.claude/rules/records.md` and
`.claude/rules/verification.md` reached it as system context, and `records.md` describes the rendering
under test. Its first run read no sample (the brief named a folder and the seat cannot list one); the
second read five samples made by a **Python sketch**, not by the engineer's renderer, and is one reader.
What it can claim is narrow: two of five samples had a title a stranger cannot use (a path, a log row), and
the headings of the sketch put a defect's substance under *Where to look*.

## Disagreements, stated plainly

1. **A verb, or a program run by `heroes run`.** The compiler-engineer and the spec-warden both object to
   the verb and name the program; the ffi-pragmatist is indifferent to the placement (it approves the
   transport). Nobody defends R1 as proposed. **What is not settled by any command is a reading**: whether
   a Heroes program run by `heroes run` is a *script* in CLAUDE.md § 10 and design.md § 3.5. The net itself
   is `heroes run tests/harness/main.hero`; `.claude/rules/cli-surface.md` line 25 says *nothing if two
   existing invocations already compose to it*. It is the author's reading to ratify.
2. **Where the program lives.** The engineer built it under `tools/issues/` (which today holds C spikes
   only) with **copied** helper modules (`bytes.hero`, `json_text.hero`: 146 lines, drift guarded by a
   `cmp` check of about 10 lines); the spec-warden built it in the harness with `cards` and `shell`, which
   `tools/` cannot import (`shell.hero` is 2,234 lines). Two placements, not two answers.
3. **The ffi-pragmatist's *no JSON builder exists* against the engineer's `json_fields.written`.** Settled by
   the critic's command: the engineer is right, and the ffi-pragmatist's Route C cost is overstated.
4. **The ergonomist's changes against the author's words.** *Give them a structure like the issue files'*
   (2026-10-06) and the rule that a Prior art line says only what its page says (33 of 90 proposed lines had
   said more): its changes 1, 2, 3, 6 and 7 conflict with one or the other; its change 5 is moot (the
   placeholder is the sketch's); the ergonomist flags its own change 6 and offers to withdraw it.
5. **The counts of numberless defects** (41, 45, 47) differ by reading, not by error. Not cited as a fact.

## Resolution (provisional: author ratification pending)

**The most robust and complete route, the one CLAUDE.md § 4 requires; what the conservative one would have
been is below.**

**R1. No fourteenth verb. The pure half and the outward half are programs of the harness, beside the net.**
`render` is a set of modules in `tests/harness/` next to `cards.hero` (which already reads cards in Heroes),
reused by the records suite. The outward half, `plan` and `push`, is a second root of the same directory,
`tests/harness/issues.hero`, run as `heroes run tests/harness/issues.hero -- <render|plan|push> ...`. This
keeps the compiler binary free of any code that can write to a third party; adds nothing to the seed, the
help, the command table or the site; shares `cards.hero` and `shell.hero` where the `tools/` placement
copied them; and is removed, when the transcription is done, by deleting the modules the program alone
uses. **The reading of § 10 is the author's.**

**R2. `render` is held by the `records` suite** as `records/rendered`: every card renders to a title, labels
and a body under the limits GitHub documents, and the check refuses a card the rule cannot render. Cost,
measured by the seats: the suite grows by about 60% (render of 2014 cards 11.86 s against the suite's 20
to 27 s); accepted, since an unrendered rule is the executor gap CL-080 closed for every other rule here.
It is also the dry run: `render` and `plan` are the only rehearsals (no staging repository, below).

**R3. The seven gaps of the rule are closed in `.claude/rules/records.md` before the tool is written**, each
as a deterministic reading held by R2:
- **D1, the title**: a `# ` heading, else a defect's bold first field with its `NNN — ` stripped, else the
  item's second field, else (a single-field item) its first field, else (an old log row) its second field;
  emphasis markers (`**`) removed (832 titles carry them); **a title that opens with a code span holding a
  path takes the card's slug words instead**; cut at a word boundary to at most 250 characters with the full
  title as the body's first line when cut (GitHub's 256 is a community figure, unverified, and three titles
  reach 257 at a cut of 256); `Defect NNN: ` added **only where the title does not already open with it**;
  a defect with no number gets no prefix and no field value.
- **D2**: the item line splits on ` | ` outside code spans.
- **D3, `closed`**: the day the body's `Closed YYYY-MM-DD` line gives; else the author date of the commit the
  card names; else the card's `filed`; `plan` prints which source each used.
- **D4, `self`**: the earliest commit of `git log --follow --format=%H -- <file>`, where `--diff-filter=A`
  answers 72 of 74 and fails the two cards that entered by merge.
- **D5**: `area: none` and a defect with no class get **no label**.
- **D6**: the headings of the kind's form in `.github/ISSUE_TEMPLATE/`; `**Origin:**` and `**Class:**`
  paragraphs become headings; **a hash written in backticks that names a commit is written bare**, since
  GitHub links a bare hash and not one inside a code span.
- **D7**: a card's second and later `- [ ]` lines are plain list lines, so GitHub shows no task progress.
- **Bodies**: a bare `#NNN` and an `@word` are written inside a code span (10 of the 32 `#NNN` would link to our
  own issues of that number the moment it exists; 9 of 11 short `@words` are real accounts); a link to another
  project's issue or pull request is written with `redirect.github.com`, which GitHub documents as the way to
  avoid the backlink, **an effect the sitting has not seen run** (R6). **The oracle** is `POST /markdown`
  (mode `gfm`, context `heroes-lang/heroes`), which returns HTML and writes nothing: no rendered body may
  contain an issue-link or a mention. It is a POST that changes nothing, and the author's yes is asked for it.

**R4. Transport: spawn `gh api -i --input -`**, the body on standard input through `shell.run_fed`, the
status line and `Retry-After` read back from `-i`, every call with `--hostname github.com`. **Not libcurl**
(the ffi-pragmatist's veto: six modules, `-lcurl` on the bootstrap line, no libcurl on the Windows leg),
**not `curl`** (the token). **The program never reads the token**: `gh` holds it; `gh auth token` is never
run. `gh`'s exit code is 1 for every HTTP failure, so the program reads the status line and never the code.

**R5. Pacing and resumption.** Serial; at least one second between mutating requests; on a 403 or 429 the
`retry-after` seconds, else `x-ratelimit-reset`, else one minute, growing; **the tree has no sleep and no
clock**, so a wait spawns `sleep`, and the program runs on the Mac (Windows ships none: stated, not
solved). **State lives in the cards**: the address is written into a card's `github:` line immediately after
its creation and before the next request; a run killed between a creation and that write is reconciled by
**listing recent issues** (a list, not a search: the search qualifier lags and the historian found its
reliability unverified) and matching the `Record:` permalink. **The permalink's pin** is a commit already on
`origin/main`, given as `--pin <hash>` and recorded in the run's state file; a rerun must give the same pin;
the push that carries it precedes the first issue. The run works in its own worktree under
`.claude/worktrees/`, so the shared checkout is not dirty for hours. Order: the cards' (`filed`, then name).
**A closed issue is created, commented and closed: three requests (5,918 in all, the author's instruction of
2026-10-06 stands)**; every closed issue closes `completed`. A close is unstated as a content-generating
request by GitHub's pages, so 3,966 is the lower bound and 11.8 hours the upper.

**R6. Before the first issue, in this order, each an outward act with the author's yes:**
1. **Rotate the token** (revoke it at github.com/settings/applications, `gh auth login`), with the scopes
   the next steps need (`delete_repo` for the experiment, `admin:org` for the field).
2. **The experiment, in two throwaway private repositories under the author's user account, one issue
   each, deleted when read**: (a) the import endpoint with `created_at`, `closed_at`, a comment date, a
   label and a milestone that exist, polled to `imported`, then read back with the issue, its comments
   **and its timeline events**; (b) `redirect.github.com` against a plain URL, a second repository's
   timeline read for a backlink. If (a) answers yes, **the rule is amended as the author asked** and the
   import becomes the route for dates, one request per issue, awaited one at a time so that numbers keep
   their order; if no, the body's first line carries the days and the REST route stands.
3. **The milestones** the author's *4a* named, one per row of the chain (84 today), closed where the row is
   closed; the label `kind:learn` removed.
4. **`PUT /repos/heroes-lang/heroes/interaction-limits`** with `collaborators_only` for the run, so that a
   stranger's issue cannot take a number between two of ours, lifted after (the critic's route). `plan`
   refuses to push unless it reads one in force, or `--accept-gaps` is given.
5. **The issue field `Defect` is set only if it exists when the run starts** (the create request takes it at
   no cost); it is no longer a precondition: the title opens `Defect NNN:`, the import cannot set it, and
   `field.defect:` search is not a guard the run relies on (R5). CL-083's field stays valid, as optional.

**R7. A guard, a blocking defect closed in the same commit as the tool** (filed with this sitting):
`.claude/hooks/guard_bash.py` refuses in the assistant's shell any GitHub write, whatever the permission
files say: `gh api` with a method other than GET (`-X` POST, PATCH, PUT, DELETE, any `-f` or `-F` or `--input`,
each of which implies POST) except the one read-only POST to `/markdown`; `gh issue`, `gh label`, `gh repo`,
`gh release`, `gh milestone` write subcommands; `gh api graphql` carrying `mutation`; **and the tracked
`settings.json` gains an `ask` entry for the program's `push`**. The author's `settings.local.json` is not
edited by the assistant; the guard is what makes its broad `gh api` allow harmless. **A second defect:
a panel seat printed the author's token**; the repair is `/panel`'s brief rule (*a seat that must
authenticate runs `gh auth status`, never `gh auth token`*), which is the author's to give, and a hook line
that refuses `gh auth token` in any shell the assistant types.

**R8. The site is untouched**: no verb, no *fourteen*. The thanks page of M-issue-files step 13 is the
milestone's one site push, made before the transcription and asked for.

**R9. The order that closes M-issue-files**: (1) the two defects, the rule's closed gaps, the render modules,
`records/rendered` and the program's `render` half, in a lane, gated, pushed; (2) the thanks page, pushed on
the author's yes; (3) the author's acts of R6; (4) `plan`, then the transcription run, under `caffeinate -i`,
with the author present, resumable; (5) the commit that writes the 2014 addresses (it runs the full four-leg
CI, about 80 minutes: `ci.yml` ignores only `site/**` and two workflows); (6) **the second pass of
`defect NNN` links as a run of its own, later** (it is a PATCH per body, counted against the same limits,
and the title already carries the number); (7) the close checklist, the local tag, and the push of the tag,
asked.

**R10. Edited issues**: `plan` reports an issue that differs from its file and **never edits it**; there is no
`sync` now, and a later one is a question of its own.

**R11. The contract of the program**: the artifact on stdout, diagnostics on stderr; exit 0 clean, 1 the input
has diagnostics (a card that cannot render), 2 the tool could not run (no `gh`, no network, a request GitHub
refused that pacing does not answer); `push` needs `--yes`, a `--pin` and the interaction limit.

### What the conservative resolution would have been, so the author can choose it

**R1 as a verb**, with the surface that parses (`heroes issues [<path>] [--plan|--push]`, one operand: the
engineer measured that `argv.parse` takes one `Operand` per verb and `Invocation.file` is read at 158 sites);
`selfhost/main.hero` given a seam in the same commit (it is exactly 300 code lines); the two start pages
changed to *fourteen* and *quattordici* with `<code>issues</code>` and **a push that publishes the site before
the transcription and before the ratification**; the seed regenerated (+12,507 lines of C); and the guard of
R7 in the same commit, since `Bash(./heroes *)` is allowed by prefix. Everything else (R2 to R6, R8 to R11)
stands unchanged. **The spec-warden would then veto unless the author decides the capability in words**, as
the author did for `probe`.

### What a veto would compel

The spec-warden's veto is conditional on the verb and does not fire on R1 above. The ffi-pragmatist's veto
on libcurl is honoured by R4. No other seat vetoed.

## Prior art

Panel 212 (`docs/panel/212-the-issue-files-reach-github-through-a-program-beside-the-net-and-no-verb-and-the-rendering-rule-is-closed-first.md`, `docs/panel/212-reports/historian.md`), every link re-read 2026-10-11.
The sitting's decision issue carries the same lines.

- **Taken from GitHub**: [Best practices for using the REST API](https://docs.github.com/en/rest/using-the-rest-api/best-practices-for-using-the-rest-api): to avoid exceeding secondary rate limits make requests serially instead of concurrently, and wait at least one second between each request.
- **Taken from GitHub**: [Rate limits for the REST API](https://docs.github.com/en/rest/using-the-rest-api/rate-limits-for-the-rest-api): no more than 80 content-generating requests per minute and 500 per hour; with a retry-after header do not retry until that many seconds have elapsed, otherwise wait at least one minute.
- **Taken from GitHub**: [Autolinked references and URLs](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls): by default references generate a backlink, and redirect.github.com can be used instead of github.com in the URL to avoid it.
- **Reached first by Swift**: [swiftlang/swift#58382](https://github.com/swiftlang/swift/issues/58382): a migrated issue's title opens with its old tracker's key in brackets, [SR-16123], and its body has a Previous ID row holding it.

Not prior art, compared only: CPython's linked BPO field (the sitting does not put the old number in a
body field), Go, the D language and the importers; D's and CPython's mass emails are what the sitting learns
from. The unofficial import API's gist is the historian's candidate and **waits for the experiment of R6**.

## Predictions to score

At M-issue-files' close (`git describe --tags --abbrev=0` and the milestone's row):

- **compiler-engineer**: with the program route chosen, the render modules measure 775 code lines in the
  `layout` suite's unit (measured); with the verb, 1,150 to 1,400 in at least 9 modules and a seam in
  `main.hero`. Checked with `layout` and `git diff --stat`.
- **ffi-pragmatist**: `runtime/` and the seed's link line change by 0 lines; falsified if the first
  implementation edits either.
- **spec-warden**: `grep -c 'Command(' selfhost/cli/table.hero` stays 13; the spec's recorded real row stays
  10044; no *fourteen* on any start page.
- **historian**: the run takes at least 7.9 hours of wall clock; a 403 without `Retry-After` happens before
  500 creations in an hour at least once. Falsified by a run that finishes in under 7 hours with none.
- **llm-ergonomist**: 18 of 20 cells clean after its changes 1 to 5. Not scored: the reading is void.
- **the sitting's own**: `records/rendered` renders 2014 of 2014 cards, and `POST /markdown` finds no issue-link
  and no mention in any of the 2014 bodies.

## Process notes

- **A seat printed a credential.** The brief said not to; nothing enforced it. See R7.
- **The briefs were wrong in 14 places and the seats were launched only after the critic's pass**; the
  revised brief still carried one error the critic's second pass found (`tests/harness/argv.hero`, which
  does not exist: the file is `selfhost/cli/argv.hero`).
- **The first run of the llm-ergonomist read nothing**, because the brief named a folder.
- **The first count of titles over 256 characters (210) counted only the cards that had a title**; with the
  old log rows titled by the coordinator's reading it is 816 of 1986.

## Author's verdict

Pending. The decision issue is
`issues/2026-10/11/2026-10-11-1846-panel-212-ratify-amend-or-overturn-r1-to-r11-a-program-beside-the-net.md`.

**What a yes settles**: R1 to R11, so that the issue files go to GitHub through a program of the harness and
not a verb, the rendering rule's seven gaps are closed by the readings of R3, and the order of R9 is the
order that closes M-issue-files. **What it does not settle**: the acts of R6, which the author gives one by
one (the token's rotation, the experiment in two throwaway repositories, the milestones, the interaction
limit, the oracle that posts to `/markdown`), and the reading of CLAUDE.md § 10 that R1 rests on, which is
the author's to say in words.

**RATIFIED, 2026-10-11**, R1 to R11 as written above, the author answering in conversation after the
panel's report, meant as: *I revoked the token and logged in again; I approve the panel; I ratify panel 212;
push everything; and the experiment is fine; what do you say about the private repositories?* Recorded as a
reading (CLAUDE.md § 4). **What it settles beyond R1 to R11**: the reading of CLAUDE.md § 10 that R1 rests on, a
Heroes program run with `heroes run` being no script; the token's revocation (R6 item 1, done by the author: the new
token carries the scopes `admin:public_key gist read:org repo`, neither `delete_repo` nor `admin:org`); and the
experiment of R6 item 2, which the author allowed. **What it does not settle**: the other acts of R6, which the author
gives one by one (the milestones, the label's removal, the interaction limit), and the answer to the private
repositories, which is the coordinator's recommendation in the decision issue's verdict. The author may overturn it.

## Appended 2026-10-11: the experiment of R6 answered yes, and what it moves

Run after the ratification above, between 19:11 and 19:14 UTC;
`docs/measurements/041-the-import-endpoint-keeps-an-issues-own-days-and-an-imported-reference-leaves-no-backlink.md`
holds every request and its answer. **The import endpoint set an issue's `created_at`, `updated_at` and
`closed_at`, and its comment's `created_at`, from the request**, in a private repository of the organisation, and
closed it `completed`; awaited one at a time, the numbers kept the requests' order; an imported body made no
backlink in either form of a reference, and a body made by the REST create made one with a plain URL and with
`owner/repo#N`, none with `redirect.github.com` or inside a code span; `POST /markdown` told the same forms apart.

**How it was run, where it differs from R6 item 2's words**: two private repositories as R6 asked, one in the
organisation (`heroes-lang/import-probe`), where the transcription will import, and the target in the author's
account (`giuseppearici/import-probe-target`), as the ratification's decision issue recommended; but six issues in
the first, two imported and four made by REST to answer (b), where that recommendation said one; archived when
read and not deleted, the new token lacking `delete_repo`; the author deleted both, found gone at 19:36 and said
so in conversation.

What it moves, each as R6 item 2 said it would:

- **`.claude/rules/records.md` § The issues is amended as the author asked on 2026-10-06**: the body's first
  line stays on every route, and GitHub's own dates are the card's days where the import sets them.
- **R5**: an issue is one import carrying its days, its labels, its milestone, its closed state and its closing
  comment, read `imported` before the next starts: 2,017 requests (the issue files counted that evening) where the
  REST route took three per closed issue, about 4.0 hours at 500 an hour if an import counts as a
  content-generating request, which is unrun. The run's first request asks the endpoint again; **where it refuses,
  the run stops with exit 2** (R11), and the REST route, which dates every issue the day it runs, is taken only on
  the author's word, since their instruction of 2026-10-06 asked for the issues' own dates. A killed run is
  reconciled as R5 says, by a list of recent issues matched on the `Record:` permalink; the gist's list of the
  imports made since a day is a second witness, unrun.
- **R3's bodies** stand: the import route needs them for nothing, and they keep the REST route safe.
- **R6 item 5** stands: the gist's schema carries no field, so the import cannot set `Defect`.
- **Prior art**: the gist moves from the historian's candidate to the decision issue's fifth line, re-read
  2026-10-11, in the gist's own words.

What it does not settle, measurement 041 § 4: a public repository, the scale, whether an import counts against
the 500 an hour, the endpoint's life after that evening, and a real commit's hash in an imported comment.

## Scored at M-issue-files' close, 2026-10-12

Each by the command named, run that evening on the round's tree.

- **compiler-engineer**: missed by the nearest measure. `layout` judges
  `selfhost/` alone, so its unit was not asked; the ten modules step 13 added
  (`c98f14b1`) hold 1,479 lines that are neither blank, comment nor test,
  `git show` of each read through verification.md's `code_of`, against the
  775 predicted. The verb's route, 1,150 to 1,400, was not built.
- **ffi-pragmatist**: exact. From `cd279804` to `6461f3e6`, steps 13 to 18,
  `git diff` of `runtime/` and of `seed/README.md`'s link line is empty. Three
  comment lines of `runtime/parts/` changed later (step 20, a citation of an
  issue file rewritten as its issue's address), and no code.
- **spec-warden**: two of three exact. `grep -c 'Command(' selfhost/cli/table.hero`
  reads 13, and no start page of either edition says *fourteen*; the spec's
  real row reads 10,050, not 10,044, moved by the six tokens of the subtitle the
  author asked for that day and by nothing of this milestone.
- **historian**: falsified. The six runs that imported 2,028 of the 2,029
  issues took 2.47 hours of wall clock, each log's birth to its last write
  (10:09 to 13:12, and 19:05 to 19:06 for the third pass), and no log holds a
  403 or a `Retry-After`.
- **llm-ergonomist**: not scored, as the sitting said.
- **the sitting's own**: exact, over the count grown since. The last `plan`,
  pinned at `585805bd`, rendered 2,029 of 2,029 files with none refused by the
  rule; and GitHub's own rendering of all 2,030 issues, read as HTML the same
  evening, held no `issue-link` and no `user-mention` anchor, and so did all
  2,033 after R9 item 6's second pass had written its 6,676 links.
