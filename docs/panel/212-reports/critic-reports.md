# Panel 212, the completeness critic's second pass

No verdict. Written 2026-10-11 on the tree frozen at `09060ea3f` after reading the five reports, the revised
brief and the first-pass report. Every result below was produced by the command beside it in this session.
My own files: a detached worktree `.claude/worktrees/scratch-panel-212/critic2/` (compiler built from the seed there,
6.96 s; a scratch program written and deleted there), and outputs under the session scratchpad `.../scratchpad/c2/`.
Requests to GitHub: GET reads, plus one GraphQL **query** (a POST at HTTP level, read-only in effect, the same kind the
ffi-pragmatist made; its cost was 1 point of the graphql resource). Nothing was written to GitHub. Three WebFetch reads of
GitHub docs and two of public issue pages. `gh auth status` was run with the token masked by `gh` itself.

## 0. Something the synthesis must carry first

The ffi-pragmatist's report opens by disclosing that it **printed the author's GitHub token into its tool output** (a
`gho_` token, from `gh auth token` run without redirection). Its recommendation is to tell the author and rotate
(`gh auth refresh`, or log out and in). Nothing in the other four reports mentions it. It is a leaked secret by the
project's own rule (guard_bash.py: *a secret in a transcript is a leaked secret*), so it belongs in the synthesis and in
the decision issue, and the rotation is the author's act. Also: `gh auth token` answers any process of the same user
(its own finding), which matters for every route that asks for the token.

## 1. Contradictions between seats, and which side a command settles

| # | Claim A | Claim B | Settling command | Result |
|---|---|---|---|---|
| 1 | ffi: a Heroes program can pass stdin through the runtime's `hero_run_go`; `process.run` hard-codes an empty path | spec-warden: `shell.run_fed` takes a stdin path (`tests/harness/shell.hero:144`) | `sed -n 85,135p selfhost/cli/process.hero`; `sed -n 125,175p tests/harness/shell.hero`; `grep -n hero_run_go runtime/hero_os.h` | **Not a contradiction, three layers.** `selfhost/cli/process.hero:118` has `in_path: "".cstr()` (the compiler's wrapper); `runtime/hero_os.h:369` and `runtime/parts/run.c:1025` take a path, empty meaning inherit; `tests/harness/shell.hero:239` `spawned` passes one through. All three true. **What matters for R11 and neither said:** `shell.hero` is **2,234 lines** and lives in `tests/harness/`; a program under `tools/` cannot `use shell` (compiler-engineer measured that `use` resolves only from the entry file's directory). The spec-warden's proof sits **inside** `tests/harness/`; the compiler-engineer's R11 sits in `tools/issues/` with its own 36-line spawn. These are two different R11s and the sitting must not treat the spec-warden's proof as proof of the compiler-engineer's placement. |
| 2 | compiler-engineer: `tools/` holds only C spikes, yet its report builds `tools/issues/` | (none) | `git ls-files tools`; `ls` of the engineer's worktree | **Consistent.** At the frozen HEAD `tools/` is `tools/spike/*.c` only (8 files, no `.hero`). `tools/issues/` exists only in the engineer's untracked worktree files (`git status` there: `?? tools/issues/`). `tools/` is a known area (`tests/harness/cards.hero:607` `"tools/ tools"`), so no `AREAS` change. |
| 3 | compiler-engineer: `selfhost/mutate/json_fields.hero` writes JSON objects, `json_read.hero` reads them | ffi: only a reader exists (`json_read.hero`), **the builder is missing** (sections 3 and 6.2) | `sed -n 130,200p selfhost/mutate/json_fields.hero`; `sed -n 20,40p selfhost/mutate/json_read.hero` | **The compiler-engineer is right, the ffi-pragmatist wrong.** `json_fields.hero:135-190` has `whole`, `string`, `strings`, `wholes`, `written(j)` and `write_into`, which writes `.fields(names:, values:)` with every string through `json_text.json_string`; its test (line 218 on) round-trips an object. The ffi's Route C cost ("the JSON body must be built by the verb, no builder exists") is overstated by whatever a builder would have cost (0 lines). Both seats agree Route A needs no builder. |
| 4 | compiler-engineer: `argv.hero` is `selfhost/cli/argv.hero`; the brief's `tests/harness/argv.hero:333, 393` does not exist | spec-warden says the same; the **revised** shared brief still prints `tests/harness/argv.hero:333, 393` | `ls tests/harness/argv.hero` (no such file); `sed -n 333p selfhost/cli/argv.hero` | **Both seats right; the revised brief still carries the error** (the first pass missed it). Lines 333 and 393 of `selfhost/cli/argv.hero` are the two test strings listing the verbs. Also the brief's `cli-surface.md` "line 18" is line 25 (`sed -n 25p`), as the engineer said. |
| 5 | ffi finding 6.4: Heroes has no string-to-integer parse (its probe called `atoll`) | the tree | `grep -n 'function to_number' tests/harness/strings.hero`; `grep -n 'function whole_of' selfhost/mutate/json_fields.hero` | **Overstated.** `tests/harness/strings.hero:185` `to_number(text) -> i64?` and `json_fields.whole_of` (overflow-safe) exist; what is true is that `str.to_i64()` as a method does not. The harness one is reachable only from `tests/harness`, the other from `selfhost/mutate`. |
| 6 | ffi: `hero_run_limit` is bound only in `selfhost/mutate/child.hero:26` | tree | `grep -rn hero_run_limit selfhost tests/harness` | True of `selfhost/`; it is **also** bound in `tests/harness/shell.hero:41` and called there (`:285`) with `WATCHDOG_SECONDS` = 600 (`:751, :1287`), **not a parameter** ("it is what `run` means"). A paced push that sleeps through a `Retry-After` of more than 600 s cannot do it in one harness-spawned child. |
| 7 | spec-warden: "today a hand transcription goes through `gh`, which no allow line covers, so each call is a prompt" | `.claude/settings.local.json` on this machine (gitignored, not in the lane tree; read in the trunk) | `cat /Users/joseph/Temp/heroes/heroes-lang/.claude/settings.local.json` | **Wrong on this machine.** The local file allows `Bash(gh api repos/heroes-lang/heroes/*)`, `Bash(gh api --allow-escape-sequences repos/heroes-lang/heroes/*)` and `Bash(gh run *)`. `gh api` with any `-f` field **defaults to POST** (`gh api --help`: *adding request parameters will automatically switch the request method to POST*), so `gh api repos/heroes-lang/heroes/issues -f title=x` matches an allow line and would run unasked. The shared `.claude/settings.json` (tracked) has no `gh` line either way. The spec-warden's conclusion (a verb removes the confirmation) is right a fortiori: the confirmation is already absent for the REST form. |
| 8 | compiler-engineer's title and number counts (47 numberless defects; 1979 distinct titles, 26 shared by 61 cards) | the brief's (41; 1941 distinct; 27 shared by 72); first pass's 45 | `python3 -I` over `scratchpad/ce/all5.ndjson` | The engineer's numbers reproduce from its own output (664 defects, 47 with an empty `defect_field`; 1979 distinct, 26 shared, 61 cards). **They are counts of a reading, not of the rule**: the three figures for "numberless defects" differ because the rule is silent (D1). Do not cite one as a fact about the cards. |

Candidates in the brief that held: the engineer's `tools/` claim, the `.cstr()` empty path, `cli-surface.md` line 25,
the two start pages at line 100 (`git grep -n -i -E 'thirteen|tredici' -- site/` is 22 lines in 12 files), the ffi's
`gh -i --jq` output shape (reproduced on a real GET: 30 header lines, blank line, `.full_name`), and `git log origin/main..main` = 0.

## 2. Claims asserted and not (fully) measured

**2.1 "R11 output is `cmp`-identical to the verb on all 2014 files."** `cmp scratchpad/ce/all5.ndjson scratchpad/ce/tool.ndjson`
prints nothing (identical, 2014 lines each, 7,814,398 bytes), and the six render modules in `tools/issues/cli/` are
`cmp`-identical to `selfhost/cli/issue_*.hero` (checked file by file). So the command the report gives
(`heroes run tools/issues/main.hero -- issues > tool.ndjson`, then `cmp` against the verb) **would show what it claims**, and
it does. **What it does not show:** it compares the same render code reached by two entry points. It proves the
`heroes run` route can host the module and the externs. It says nothing about whether any rendering is right: no
rendered card has been compared with a hand-written issue (there is no oracle, the engineer says so), and the readers of
the bytes have not seen a real rendering (the ergonomist read a Python sketch).

**2.2 "The 14th verb takes `selfhost/main.hero` to 302 code lines and the layout suite red."** Counted the suite's way:
`code_lines` (`tests/harness/suite_layout.hero:966`: blank lines out, test blocks out, comment lines **in**), re-implemented
in `python3 -I`, gives **300** for `selfhost/main.hero` at HEAD, which is the ceiling (`CEILING` 300, the test is
`lines > limit`, line 750). `./heroes run tests/harness/main.hero -- ./heroes layout selfhost/main.hero` at HEAD:
`4 passed, 0 failed`. A `use` line plus one dispatch arm is +2, so any single added code line is red; 302 is
right. Is a row or a seam the normal remedy? `DECIDED` has **19 rows**, each a file the language refuses to split
(a mutually recursive knot; the table's own text: *where the LANGUAGE forbids the seam*), and `assert DECIDED.len() == 19`
must be edited by hand for any addition (`suite_layout.hero:1139`). `main.hero` is not a knot; it has
already shed its dispatch into `cli/verbs`, `cli/syntax_cmds` and `cli/check` modules. So **a seam is the rule's remedy
and a row would be the first of its kind that is not a knot**. The cost was never put as a number: the seam is code moved
out of `main.hero` in the same commit as the verb. (Unrun by me: the engineer's own 302 reading; my reading is the
baseline plus the arithmetic.)

**2.3 "A pre-approved `Bash(./heroes *)` would let a push verb run without confirmation."** Read exactly:
- `.claude/settings.json` (tracked): `allow` has `Bash(./heroes *)`, `Bash(/usr/bin/time -p ./heroes *)`,
  `Bash(./heroes-next *)`, `Bash(xargs -P 8 ./heroes *)` and `Bash(caffeinate -i *)`; `ask` has **only** `Bash(git push*)`;
  `deny` has the `git add -A`/`commit -a`/`stash`/force-push family. No `gh`, `curl`, or `issues` entry anywhere.
- `.claude/hooks/guard_bash.py` (818 lines): refuses `git add -A` and kin, `git stash`, force push, `UPDATE_GOLDEN=`,
  discards (`discards.py`), reads of `.env`, expansion of two secrets, `--no-line`, and **one network rule**: a client
  (`curl wget http https xh httpie python python3 node deno`) naming `api.anthropic.com` with anything but
  `/v1/messages/count_tokens`. It says of itself *Nothing here touches the tree or the network*. It has **no rule about
  `gh`, `api.github.com`, or any `heroes` verb**, and it sees only the text of the command the assistant types, never
  what a compiled `heroes` spawns inside.
- So the spec-warden's reading holds: `./heroes issues push` is allowed by prefix and the hook has no opinion. What I did
  **not** run, and the spec-warden said it did not run either: whether `ask` outranks `allow` for the same command in
  this Claude Code version, and whether `caffeinate -i *` or `/usr/bin/time -p ./heroes *` swallows an inner `ask`. Both
  are one command each in a scratch session. The report's remedy (an `ask` line per wrapper prefix) therefore stands
  unproven; the stronger remedy a hook can give is a refusal, but a hook cannot tell consent from habit.

**2.4 The historian's rate-limit figures against its pages** (re-read today): `docs.github.com/en/rest/using-the-rest-api/rate-limits-for-the-rest-api`
says "no more than 80 content-generating requests per minute" and "no more than 500 content-generating requests per
hour", "Some endpoints have lower content creation limits", a violation "will receive a `403` or `429`", wait for
`retry-after` if present, wait until `x-ratelimit-reset` if `x-ratelimit-remaining` is 0, otherwise "at least one minute",
then exponentially more; "may result in the banning of your integration". **All verified.** The page does **not** define
"content-generating" (I asked for any mention of issues, comments or edits: none). The create-issue page and the
create-comment page each say "This endpoint triggers notifications." and "Creating content too quickly using this endpoint
may result in secondary rate limiting." (re-read); the update-issue (PATCH, the close) section says neither. So: creation
and comment count by the pages' own wording; a close is **unstated**. The arithmetic: 3,966 / 500 = 7.93 h and
5,918 / 500 = 11.84 h are correct as bounds, and the historian labelled them inference. What it also found and the
synthesis must not drop: two community threads report a 403 after about **100 to 150 issues** created in batches, with no
`Retry-After` and `X-RateLimit-Remaining` high, so the documented 500 an hour may be **optimistic**, and GitHub states no
duration for a block sent without a header. The rule the page gives for that case is "wait at least one minute".
The historian's Prior art lines 4 and 5 rest on fetch summaries; I re-read the two pages: Swift
`swiftlang/swift#58382` title is `[SR-16123] Non-escaping closure ...` and a **"Previous ID" table holds SR-16123**, but
my read gives the body as opening with *Issue Description:* then code, so "the body begins with a table" is **not
confirmed** and the line must be re-read before it is filed; CPython `python/cpython#59000` shows a BPO field linking
`bugs.python.org/issue14795` and "opened on May 13, 2012" (confirmed). The autolink page confirms
"By default, references generate a backlink", the "Avoiding backlinks to linked references" heading and the
`redirect.github.com` sentence, and says **nothing** about code spans.

**2.5 The ffi-pragmatist's `gh` statements.** `-i` exposes headers: reproduced on a real GET and on a real GraphQL query
(status line, 22 headers, blank line, body; `X-Ratelimit-Resource: graphql`). `Retry-After` visibility: **unrun on a real
limit** (the ffi said so; the shape came from its fake). A POST 301 followed as GET: measured only against its fake; it is
the Go HTTP client's standard rule, an inference for GitHub. `GH_HOST`: measured against the fake only. `gh api --help`
confirms `--hostname` (default github.com), `--input -`, `-i`, and that parameters switch the method to POST.

**2.6 Numbers nobody measured that the sitting needs, measured now** (from `scratchpad/ce/all5.ndjson`, the engineer's 2014
renderings, parsed with `python3 -I`):
- **Largest rendered body: 54,459 code points** (`2026-09-28-2138-the-predictions-of-m-agreed-retention-scored.md`),
  **0 of 2014 over 65,536**. The historian left this open ("I did not measure the rendered one").
- **Titles: the verb's cap lets 3 reach 257 characters** (cut at 256 plus ` ...`), so if GitHub's 256-character title
  limit (a community figure, unverified) is real, 3 creates would be refused. **833 titles are cut**, **969 exceed 150
  characters**, **832 contain `**`** (**747 start with `**`**) and 204 contain a repository path. The historian quotes
  GitHub as not ignoring Markdown in a title. So a title is, for 41% of cards, a paragraph with bold markers. The ergonomist's
  complaint about sample 02 is not a sample artifact: it is the rule's second-field fallback at scale, and nobody costed it
  (the engineer's `## Title in full` section is its patch).
- All 34 labels the renderings use already exist on GitHub (`gh label list`, 45 labels); `area:probe` and `kind:learn`
  are the two project labels unused. **0 milestones** exist; the cards name 63 `M-` values, and 1,816 of 2014 cards say
  `milestone: none`.
- `gh api repos/heroes-lang/heroes/import/issues/1` still answers 404 (reproduced). The token's scopes are
  `admin:public_key gist read:org repo user` (`gh auth status`): no `delete_repo`, no `admin:org`.

## 3. The adopted route nobody built

If the sitting follows the seats: the compiler-engineer and the spec-warden both object to R1 as proposed; both point to R11
(a Heroes program run with `heroes run`) for `render`; the ffi-pragmatist approves spawning `gh api` but states its
predictions for the **verb** (it edits `selfhost/cli/process.hero`); the historian wants the push to sleep rather than stop.
The route that follows is: a Heroes program under `tools/` (or `tests/harness/`), `render` also held as a `records` check,
`plan` and `push` spawning `gh api -i ... --input -` through a stdin-feeding spawn, sleeping on `Retry-After`.

**Built and run, per piece:** render over 2014 files (engineer; two entry points); a program that reads a card with the
harness reader and feeds a child its file (spec-warden, `wc`); a Heroes probe binding `hero_run_go` that drove `gh` through
12 scenarios against a loopback fake, 54,818 bytes delivered via stdin and via `--input <file>` (ffi); a pure-state-machine
or file-appending recording fake for a transport (engineer, tiny tests); `heroes run tools/issues/main.hero`'s tests, 11 in 1.25 s.

**The two things the route depends on, and what I ran:** (a) `run_fed` feeding a 54,818-byte body, (b) the output of
`gh api -i` read back. In `critic2` a scratch program using the harness's `shell.run_fed` ran `wc -c` over the 54,818-byte card
as stdin (`out=   54818`, code 0), and `gh api graphql -i --input -` with a one-line query file as stdin against the **real**
GitHub: `started=true code=0`, 1,297 bytes of output captured and printed with its status line, headers, blank line and JSON
(login masked). So stdin reaches real `gh` through `run_fed` and the `-i` output reads back as text. **Still unrun: one
call that does both** (a 54,818-byte POST body through `run_fed` into `gh api -X POST ... --input -`), which needs a server
that accepts it (the ffi's fake, not run by me); the pieces are measured, the composition is not. Both facts hold only for
`tests/harness`'s `shell.hero`; the engineer's `tools/issues/` has its own spawn and no `in:` parameter was built or tested
there.

**Unbuilt, by name (nobody built any of it):** `plan` (the diff against what GitHub holds); `push` (create, comment, close);
the writer of `github:` into a card; the response reader for `gh -i` (status line, `Retry-After`, `x-ratelimit-*` parsed);
**pacing and sleeping: the tree has no sleep and no clock** (`grep -i 'sleep\|nanosleep\|clock_gettime'` over `runtime/*.h`
and `selfhost/` finds nothing; the one precedent spawns `date`, `selfhost/cli/refresh.hero:123`), so a wait must spawn a
program (`sleep`), which Windows does not ship; a resumable state machine; the second pass of `defect NNN` links; the
issue-field `Defect`; any test of `records/render`; the `ask` rule. The engineer's estimate for the push half is 400 to 500
lines, labelled inference. The shell-side parts of `render` also touch git (about 1,500 spawns, `sys` 4.38 s of 11.86 s).

## 4. Questions not asked

**(a) Resumption over hours.** At 500 content requests an hour the transcription is 8 to 12 hours of wall clock (historian),
more if the community's lower observed ceilings are real. Not asked: *where does the state live* (the cards' `github:` lines in
a working tree that other sessions share, uncommitted for the whole run, while the hard stops allow a commit only by
pathspec); *who watches* (CLAUDE.md § 3 asks an update every three minutes and a wakeup of at most three: roughly 200 to 240
recaps); *a closed laptop* (the process suspends mid-request; GitHub may already hold the issue; `caffeinate -i` is in the
allow list and `pmset` in the local one, so the author already runs long jobs this way); *the permalink pin* (the `Record:`
link carries `git rev-parse HEAD`; if another session commits during 10 hours the same card renders a different body on
rerun, and **the pinned commit must already be pushed or every link 404s**, so the push precedes the first issue; at HEAD
`git log origin/main..09060ea3` is 0); *a stranger in between* (see (g)).

**(b) Created already closed.** Checked: `gh issue create --help` has no state or closed flag; the REST create-issue page lists
`title, body, milestone, labels, assignees, issue_field_values, type, parent_issue_id` and **no `state`, `closed` or
`state_reason`** (state and `state_reason` exist only on the PATCH). So via REST every closed issue is create, close, and a
comment if the author's closing comment is kept: 3 requests (5,918 total) or, folding the closing text into the body, 2
(3,966 total, 1,952 saved). Only the unofficial import takes `closed`. Not asked: **which `state_reason`** (`completed` or
`not_planned`) a closed decision takes (1,207 decisions; a declined one closed as completed says the opposite); whether
a close counts against the 500 an hour (unstated by both pages, so 3,966 is the lower bound and the saving is unmeasured).

**(c) The import endpoint, the live test, the scope.** Reproduced: a read of `.../import/issues/1` is a 404 and the token has
no `delete_repo` (historian right). What a test must contain (historian's list stands) plus what it left out: one issue
with `created_at`, `closed_at` and `closed: true` in 2026-09; a comment with its own `created_at`; one label and one
milestone that exist (the import fails otherwise); poll to `imported`; then read back the issue, its comments **and its
timeline events** (the historian's 2019 report says `labeled` events read "now"). Not stated: (i) **ordering**, since the
gist says numbers are handed out when processing completes, so an asynchronous import can give numbers in completion
order and break the order promise unless each import is awaited before the next; (ii) the `Defect` issue field and the native
type are **not** settable by import (the 2025 discussion says types are applied after import), so each issue costs at least
one more request; (iii) whether an import counts against the 500 an hour; (iv) the test repository must be in the author's
**user** account and, with no `delete_repo`, **cannot be deleted by this token**, so the leftover is one private repository
holding one issue; the options are to leave or archive it, or to raise the scope (the author's act), and no one has put that to
the author as a choice. (v) The test does not answer the *organisation* "Not Found" reports unless run in an organisation repository.

**(d) Order of the tag and the push if a 14th verb is added.** The author's instruction (`issues/2026-10/06/...1249...`) makes
the transcription the last act and says M-issue-files cannot close while the acts on GitHub wait. A 14th verb adds a `site/`
change (two start pages, four lines) whose push **publishes the site** (`deploy-site.yml`), and the `Record:` pin needs the
commit pushed first. So the order forced is: verb, records amendment and site edit committed; `npm run build` in `site/` (unrun
by anyone: `node_modules` absent in the engineer's tree); the asked-for push (site goes live saying *fourteen*, before the
author's ratification and before any issue exists); the transcription; the commit writing 2,014 `github:` lines (full four-leg CI,
`ci.yml:97-100`); only then the tag over a clean list. The milestone page's close clause still reads "the three issues below
it ... and the tag placed over a list clean of blocking and systemic defects, which today it is not" while the brief's F1 says 0
open defects: one of the two is stale. Not asked: whether the transcription commit may sit **after** the tag the author's
sentence implies (it says the transcription closes the milestone, so the tag must follow it, and the push that carries the tag
is a second asked push). With R11 instead of a verb none of the site steps exist and the push carries no site commit.

**(e) `render` as a records check.** The `records` suite measured here: `records: 30 passed, 0 failed`, `real 19.89 user 19.94
sys 2.34` in a fresh worktree (the brief says 27 s; the number moves). The engineer's whole-tree render is 11.86 s, so a check
that renders every card adds about **60%** to the suite; narrowed per card (the write hook's way, `fmt_check.py:223-251`
runs the suite narrowed to the file) the engineer's loop gives about 12.7 ms of work per card plus a compile of the tool.
Would the hook then refuse a card nobody can render? **At this HEAD it refuses none** (engineer: 0 of 2014 refused; refusals
are only "does not open with its card", "no title can be read", unreadable path). So the check would guard future cards only
against those three conditions, not against the unrenderable-in-practice ones measured above (3 titles at 257, 832 titles with
bold markers, 47 numberless defects). To make it mean something the rule needs a refusal for each, which is a rule change
(a panel question) and not a check.

**(f) The ergonomist's six ranked changes against two binding rules.** The author's words: *give them a structure like the issue
files'*; the records rule: a Prior art line says only what its page states and nothing of what Heroes did.
- Change 1 (a fixed Problem / Resolution / Commit block before everything) **adds a structure no issue file has**; the files
  hold the template headings of `.github/ISSUE_TEMPLATE/`. Conflicts with the author's sentence unless the author accepts it.
- Change 2 (rename headings by contents) and 7 (collapse project-internal text into `<details>`) **depart from the file's own
  headings**; change 7 also hides sections the rule says end with the hash.
- Change 3 (titles by symptom, never a path) **conflicts with the rule's title order** (heading, bold first field, second
  field); the fix for 02 (the sitting's own title) is not a source the rule names.
- Change 6 (a verb that matches the line; say what Heroes adopted) **conflicts directly** with *each line says what the cited
  page itself states, and nothing of what Heroes did about it* (`.claude/rules/records.md` § Prior art; 33 of 90 lines had
  failed exactly this). The ergonomist flags this itself and offers to withdraw it.
- Change 5 (the `<the commit the run is pinned to>` placeholder) **is moot**: it is the Python sketch's placeholder; the
  engineer's verb emits a real 40-hex permalink (`Record: https://github.com/heroes-lang/heroes/blob/09060ea3fb01.../issues/...`).
- Its whole reading is **void by its own agent definition**: its report says `records.md` and `verification.md` reached it
  as system context, and its marks on template headings and the Prior art rule "rest on knowledge of the repository". Its
  second run answered five **sketch** samples (the first-pass critic had warned the sketch tests the prototype, not the rule),
  from **one** reader. Its prediction (18 of 20 cells clean after changes 1 to 5) was not re-run. No stranger reader has seen
  a rendering of the engineer's verb.

**(g) Routes and questions nobody listed.**
- **Hold strangers off during the run**: `PUT /repos/heroes-lang/heroes/interaction-limits` (docs page re-read: `limit`
  `collaborators_only`, `expiry` from `one_day` to `six_months`; it restricts who may comment, **open issues** and create pull
  requests). `GET` on it answers `{}` now (none set). It would settle the brief's question 5 (a stranger's issue taking a number
  between ours) without a lock in the verb; it is an outward write and the author's call, and no seat or brief names it.
- **Cheapest settling of the code-span question** (`#NNN` and `@word` in backticks, hash in backticks): the Markdown render
  endpoint (`POST /markdown`, mode `gfm`, context `heroes-lang/heroes`) returns HTML and writes nothing; I did not run it
  because the brief allows only GET. It answers what the autolink page does not (the page says nothing about code spans),
  and settles the engineer's "one fact I take from GitHub's behaviour, not run here" and the historian's D6 `unverified`.
  It cannot settle whether `redirect.github.com` writes no cross-reference event; only a real write can.
- Whether the closer should be the author's account: every issue and comment is authored by `giuseppearici`; GitHub's
  abuse language ("banning of your integration") is about the token holder.
- A `Defect` field needs `admin:org` (a token refresh) **and** its values cannot come from the import route: nobody asked
  whether the field is still wanted given the title already opens `Defect NNN:` and the search guard on it is unproven
  (`field.defect:` shows on GitHub's page; the only caveats are from a third-party mirror, unverified).
