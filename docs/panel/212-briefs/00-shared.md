# Panel 212, the shared brief: the verb that turns an issue file into a GitHub issue

Written 2026-10-11 on the tree frozen at `09060ea3` (`git -C <tree> log -1 --format=%h`),
`git status` clean. **Revised the same day after the completeness critic's first pass**
(`docs/panel/212-reports/critic-briefs.md`), which found 14 disagreements between this
brief and the tree; the corrections are folded in and marked *(critic)*. **Every number
below was produced by the command beside it; none is carried.** A sentence that says
something does not exist says what was searched. A figure you cannot reproduce with its
command is a finding: report it.

## The decision this sitting serves

The author asked on 2026-10-11, in the coordinator's conversation (no record of it exists
yet; the sitting's decision issue will carry it), to **close M-issue-files** (row 83 of the
chain, `docs/roadmap/milestones/M-issue-files.md`), whose last big item is the transcription
of the project's issue files into GitHub issues. Binding instructions the author already gave,
**which this brief repeats because the sitting must honour them** (*critic*, `issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`):

1. **The transcription is the last act of the work**, after the labels, the milestones and
   this verb's sitting; each of those is an outward act the author asks for when it comes.
2. **Each issue carries its own days**, filed and settled, *"as GitHub's own dates where an
   endpoint can set them and in the body's first line always"*; the author wrote that
   *"I would not want it all to look like today's date; do it very carefully"*.
3. **A closed issue carries a closing comment** (`Fixed by` or `Settled by`), dated where it can be.
4. **Whether GitHub's issue import endpoint still sets `created_at` and `closed_at` is a
   standing question to the endpoint, asked before the first issue is made**, and the rule in
   `.claude/rules/records.md`, *a transcription cannot back-date GitHub's own dates*, is
   amended if the answer is yes. **That is route R4 below, and it is the author's, not a
   curiosity.**

The author's *7a* of 2026-10-05 says this sitting is convened when the author decides to
transcribe. CLAUDE.md § 10 says any new capability is a `heroes` subcommand or flag, never a
second binary, never a script, never a Makefile; § 4 sends the tool's surface to a panel.

## The proposal (what the coordinator would put to the author; R1 below)

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

The rule it implements is `.claude/rules/records.md` § *The issues*, the paragraph opening
*What an issue becomes on GitHub* (`grep -n 'What an issue becomes on GitHub' .claude/rules/records.md`
reads line 255 at this HEAD), as amended on 2026-10-11 by M-issue-files steps 9 to 11. **The
rule has gaps that the first drafts of this brief handed the seats as settled** (D1 to D7
below): rule on them, do not assume them.

## Facts, each with the command that produced it

**F1. What there is to transcribe** (`for k in decision defect task feature; do t=$(grep -rl "^kind: $k$" issues/ | wc -l); o=$(grep -rl "^kind: $k$" issues/ | xargs grep -l '^- \[ \] ' | wc -l); echo $k $t $o; done`):
decision 1207 (0 open), defect 664 (0 open), task 96 (35 open), feature 47 (27 open);
`find issues -name '*.md' ! -name README.md | wc -l` is **2014**, **1952 closed** and **62 open**.
`github:` reads `none` on every card (`grep -rlE '^github: https' issues/ | wc -l` is 0).

**F2. Size** (`python3 -I` over `os.path.getsize` of the 2014 files): largest 54,818 bytes
(`issues/2026-09/28/2026-09-28-2138-the-predictions-of-m-agreed-retention-scored.md`),
**median 2,844** *(critic: the first draft said 2,846)*, mean 3,128, about 6 MB in all; **none
over 65,536 bytes**, one over 30,000. GitHub's documented body limit is **unverified here**
(historian).

**F3. Titles, run over every card** (`python3 -I docs/panel/212-briefs/scripts/titles.py` from
the repository root; *the first draft's script was unsaved and its median wrong* (critic)).
The rule as written: a `# ` heading, else a defect's bold first field, else the item's second
field. Its output at this HEAD:

- title source: item second field 523, old log row 642, defect item 555, none 17,
  **single-field item 11** (an item with no second field), heading 266 (sum 2014);
- **670 cards have no title by the rule as written**: 642 old log rows, 17 with none of the
  shapes, 11 single-field items *(critic: the first draft said 659)*;
- taking a log row's second field and a single-field item's first (the coordinator's reading,
  the rule being silent): **1986 titled, 1941 distinct, and 27 titles shared by 72 cards**
  (`Covered` 8 times, `panel 062` 6, `panel 021` 4). **A search by title cannot be what stops a
  double**; the `Record:` permalink is unique to a file;
- length: of the 1344 cards titled by the rule as written, **210 exceed 256 characters**,
  **median 101** *(critic)*, maximum 3,529, 324 exceed 150; with the log rows titled, **816 of
  1986 exceed 256**. GitHub's title limit is **unverified here**;
- quality: of the 523 titled by an item's second field, 32 start with a backtick, **19 of those
  with a path**, and 92 contain a `docs/`, `issues/`, `selfhost/` or `tests/` path (sample 02:
  the title is a path).

**F4. What GitHub reads inside a body** (a Python script, outside code spans and fences; the
critic re-ran it): **21 cards contain a bare `#NNN`** (32 occurrences); **10 of the 32 are
numbers at or below 2,100** (`#1`, `#3`, `#55`, `#119`, `#229`, ...), which link to **this
repository's own issue** of that number the moment it exists, writing a *mentioned* event on
it; the other 22 (`#84611`...) stay text until the repository reaches them *(critic)*.
**8 cards contain an `@word`** (15 occurrences, 13 words: `@out`, `@c`, `@x`, `@xs`, `@p`,
`@m`, `@s`, `@buf`, `@h`, `@d`, `@rpath`, **`@executable_path`, `@loader_path`**) which GitHub
reads as a mention of an account of that name if one exists (the critic found 9 of 11 of the
short ones are real accounts). **10 cards contain `owner/repo#NNN`** (`ziglang/zig#9194`,
`rust-lang/rust#84611`, `golang/go#50285`, `nim-lang/Nim#18326`...) and **25 `https://github.com/.../issues|pull/N`
links sit in 11 cards, besides the Prior art lines of the 46 decision issues**: each creates a
**cross-reference event on another project's issue**, visible to its maintainers. Closing
keywords (`fixes #N`) in a body: **0 cards** *(critic, `grep -rliE '(fix(es|ed)?|close[sd]?|resolve[sd]?):? [^ ]*#[0-9]' issues`)*.

**F5. The account and the repository** (`gh auth status`, `gh api repos/heroes-lang/heroes --jq .permissions`,
`gh api user/memberships/orgs/heroes-lang`, `gh api rate_limit`, `gh label list`,
`gh api repos/heroes-lang/heroes/milestones?state=all`, `gh api orgs/heroes-lang/issue-fields`,
`gh api orgs/heroes-lang/issue-types`): logged in as the author's account, token scopes
`admin:public_key gist read:org repo user` (**no `admin:org`**, which creating an organisation
issue field needs); permissions admin, maintain, push; organisation role admin; **0 watchers,
1 star, 0 forks**; **45 labels** (GitHub's 9 defaults and 36 of the project's), **0 milestones,
0 issues, 0 pull requests**; the organisation holds the four default issue fields (Priority,
Effort, Start date, Target date) **and the native issue types Task, Bug and Feature** *(critic)*;
REST core limit 5000 an hour, search 30 a minute. `gh --version` 2.102.0; `curl-config --version`
libcurl 8.7.1; the repository has rulesets *main is not rewritten* and *release and milestone tags
are immovable*.

**F6. The command's surface today** (`grep -c 'Command(' selfhost/cli/table.hero`): **13 verbs**
(`lex parse check build run test fmt doctor mutate probe measure grammar this`). The English start
page says *One command, thirteen verbs* (`site/src/html/start.html:100`) and **the Italian edition
says *tredici*** (`site/src/html/it/start.html:100`) *(critic)*; `site/src/lib/claims.ts` counts
the `Command(` rows and refuses the site's build when a page disagrees, **and also requires each
start page to name every verb as `<code>verb</code>`** (`claims.ts:447-450, 488-491, 607-611, 690-691`),
so a fourteenth verb needs the count word and `<code>issues</code>` on both pages. **A push that
touches `site/` publishes the site** (`.github/workflows/deploy-site.yml`). The net's own tests
list the verbs too (`tests/harness/argv.hero:333, 393`; `main.hero`'s dispatch).

**F7. How the compiler reaches the machine** *(corrected by the critic)*:
- `selfhost/cli/process.hero`: a program is named and its arguments pushed one word at a time,
  `Ran.started` distinguishing a program that did not start from one that exited non-zero;
  **`process.run` hard-codes the standard-input path to empty** (`process.hero:96-103`), so a
  spawned program cannot be fed standard input through it; `gh api --input <file>` is reachable,
  `--input -` is not without changing that module.
- **The compiler already spawns `curl`**: `selfhost/cli/refresh.hero:81-82` (`heroes measure
  --refresh`) runs `process.run(program: "curl", ...)` with the body in a scratch file and the key
  in neither argv nor a file (`--variable`, `--expand-header`), curl 8.3.0 or newer. That is the
  one network precedent; `selfhost/cli/count_tokens.hero:24` says it is *"the half that needs no
  network"*.
- **JSON**: `selfhost/cli/json_text.hero` `json_string` (lines 75-100) is the compiler's JSON
  string escape (quote, backslash, the C0 controls, DEL and C1 controls as `\u00XX`, bytes above
  ASCII passed through), used by `check --json`, `lex --dump-tokens --json` and
  `count_tokens.hero:46`. **What is missing is an object and array builder**, requests being built
  by string concatenation.
- No `gh`, GitHub client or `api.github.com` anywhere in `selfhost runtime tests/harness .claude`
  (`git grep -n -E '"gh"|gh api|gh issue|api\.github\.com'`).

**F8. What a rendered issue is, at present**: nothing renders one. The rule is followed by hand.
Five samples, regenerated by `docs/panel/212-briefs/scripts/render_proto.py` (a **sketch**, not
the rule: its readings are in its docstring), are in `docs/panel/212-briefs/samples/`: 01 a closed
defect, 02 a decision with `## Prior art` (its title is a path: the rule's second-field fallback),
03 a closed task with `## What was done`, 04 four defects in one card, 05 an old log row.

**F9. An inference, labelled as one**: one closed issue costs three requests (create, comment,
close), one open issue one, so 1952 x 3 + 62 = **5,918 requests** before a second pass, against
the REST core limit of 5,000 an hour and GitHub's secondary limits on content creation, which
the historian reads and this brief does not assert.

## The rule's own gaps (D1 to D7), found by running it over the 2014 cards *(critic)*

- **D1. The `Defect NNN:` prefix doubles**: 109 defect cards have a `# ` heading and **107 of
  those already open with `Defect`**, so the rule gives `Defect 064: Defect 064 closes...`;
  **41 defect cards have no number at all** (no `defect-NNN-` in the name, no `NNN` first field;
  the critic counted 45) and their titles are `Covered` (8), `panel 062` (6), `The record` (2)...
  There is no number for the title or for the field `Defect`.
- **D2. Fields split on ` | `, and 20 of 1,219 item lines hold ` | ` inside a code span.**
- **D3. `closed YYYY-MM-DD` has no source**: the card has no `closed` field. The author date of
  the commit the card names is **not** the day the body gives (sample 01 of the first draft
  printed 2026-10-03; its body says *Closed 2026-10-04*); one closed card has `commit: none`.
- **D4. `self` by `git log --diff-filter=A`**: of 74 cards with `commit: self`, 72 give one hash
  and **2 give none** (they entered by merge: `issues/2026-09/24/2026-09-24-0002-defect-093-...`,
  `issues/2026-09/25/2026-09-25-1106-panel-178-ratify-the-rest-is-zero-where-it-is-written.md`).
- **D5. Labels**: 297 cards have `area: none` and 180 defects carry no class; no label exists
  for them.
- **D6. Headings**: a decision's form is *The question / The default today / Where to look / Origin /
  Prior art / Recommendation / Verdict* (`.github/ISSUE_TEMPLATE/decision.md`); the rule says
  `**Origin:**` and `**Class:**` become headings of those names. A hash the cards write in backticks
  (`` `24cb4fd4` ``) **is not linked by GitHub inside a code span**, while the rule says the section
  ends with the hash *since GitHub links a hash*.
- **D7. Task lists**: 20 cards hold more than one `- [ ]` or `- [x]` line (59 lines); GitHub renders
  them as a tracked task list with a progress count.

## The routes, enumerated from the world

Ask of each: what would have to be true for it to be wrong, and which route nobody listed would
exist if it were.

- **R1, the proposal**: a verb `issues` with `render`, `plan`, `push`.
- **R2, a flag of an existing verb**: none of the 13 verbs reads or writes `issues/` (searched:
  `grep -n -iE 'record|issue|\.md|document' selfhost/cli/table.hero` and the `Command(` summaries);
  `measure` reads `CLAUDE.md` and `spec/`, and `check --apply --in-place` is the existing form of a
  verb that rewrites files. A question, not a settled no.
- **R3, render only**, the author feeding `gh api` in a loop: a script by § 10's own words.
- **R4, GitHub's Issue Import API** (a preview taking `created_at`, `closed_at` and comments): **the
  author's standing question (above, item 4).** A read still answers by name: `gh api -X GET
  repos/heroes-lang/heroes/import/issues/1` returns a 404 whose `documentation_url` is
  `https://gist.github.com/jonmagic/5282384165e0f86ef105#check-status-of-issue-import` (the critic;
  the route family exists, an inference from that); who may call it and what it does to dates is
  **unrun**. **Only a write settles it** (a throwaway private repository, one issue, then deletion),
  which is an outward act the author has not yet allowed; the sitting says whether to ask.
- **R5, a CI workflow** that creates the issues on push: a second implementation of the rule outside
  the compiler.
- **R6, no GitHub issues**: the files are the issues. The author has asked for the issues.
- **R7, spawn `curl` as `refresh.hero` already does** *(critic)*: body in a scratch file, token by
  `--variable`; no `gh`, no libcurl binding; the token needs a source (`gh auth token`).
- **R8, GraphQL mutations** (`createIssue`, `addComment`, `closeIssue`, several aliased in one
  request): could cut the 5,918 requests; takes `labelIds`, `milestoneId`, `issueTypeId`,
  `parentIssueId` and no `createdAt`; its secondary-limit cost is **unverified** *(critic)*.
- **R9, `gh issue create` itself** (`--body-file -`, `--label`, `--milestone`, `--type`, `--parent`):
  the native **issue types** Task, Bug and Feature exist in the organisation and `kind:*` labels are
  the proposal's choice; **sub-issues** (`--parent`) would suit a card that bundles defects (sample
  04). Nobody has asked why labels and not types *(critic)*.
- **R10, `gh-iff`**, a `gh` extension "to batch import issues from a CSV file" (the critic found it
  by `gh extension search`; unread): what would have to be true for it to be wrong is that it carries
  no per-issue comments or closes.
- **R11, a Heroes program run with `heroes run`**, no new verb: the net is exactly that
  (`heroes run tests/harness/main.hero`), and `tools/` exists. § 10's *never a script* is about a
  second binary, a shell file or a Makefile; the stopping rule says *nothing if two existing
  invocations already compose to it* (`.claude/rules/cli-surface.md`, line 18).
- **R12, GitHub's own importers or an export archive, a staging repository, slices of the run**
  (the 62 open first, a day at a time), **Projects v2 draft items** (not issues: no number, no
  comments), **git-based tracking** (`git-bug`): listed by the critic, each a sentence in the
  report if the seat can say why it is or is not a route.

## The questions

1. **Surface.** R1 against R2 to R12; the name, the subverbs, whether `plan` and `render` are one
   (`--dry-run`); what a pushed run needs in the way of an explicit yes, **given that no existing
   verb has a confirmation flag**; `.claude/rules/cli-surface.md` fixes *the artifact on stdout,
   diagnostics on stderr; exit 0 clean, 1 the input has diagnostics, 2 the tool could not run*,
   *`--json` says how to print, never what*, and says there is **no fourth input class** (lines 40-47):
   is `issues/` one, and what do *GitHub refused* and *an issue differs* exit with?
2. **Transport.** R1's own: spawn `gh` (no standard input through `process.run`), spawn `curl` (R7,
   exists), libcurl across the C boundary (design.md §1.11: there is no standard library, everything
   comes from C, *critic: not CLAUDE.md*), GraphQL; where the token comes from and what shows it to
   another process.
3. **Titles** (D1, D3 and F3): a rule for the 670 with none, for the shared 72 and the 816 over the
   limit, and a number-less defect, that is deterministic and keeps the number a reader searches for.
4. **Bodies.** What the verb does to a bare `#NNN`, an `@word`, an `owner/repo#N` and a URL to
   another project's issue (F4: **the cross-reference events on other projects' timelines are a side
   effect on third parties**, and the Prior art lines exist to credit those projects), a hash in
   backticks (D6), a ` | ` in a code span (D2) and the 54,818-byte card, without altering what a
   reader reads.
5. **Dates and order** (author's items 2 and 4): R4 or not; what `closed` means (D3); the order is
   the cards' (`filed`, then name) and **a stranger's issue takes a number in between** (records.md,
   *Order*): does the run lock the repository, accept the gap, or stop?
6. **Idempotence and resumption.** A run killed between a creation and the commit that writes
   `github:` must create no second issue: `field.defect:NNN` for a defect, the `Record:` permalink
   (**not the title**, F3) for the rest; *which commit is the permalink pinned to* (it changes if
   `main` moves between a run and its rerun); pacing against the limits and `Retry-After`; what a
   rerun does with an issue **a stranger edited, commented on or closed**.
7. **Created closed, or created then closed?** 3 requests and 2 timeline events per closed issue, and
   1,952 issues open for an instant (the Import API takes `closed`); and the notification volume of
   2,014 creations (0 watchers, 1 org member; the author's own settings unread).
8. **What the verb creates besides issues**: the milestones (**84 rows in the chain, 63 `M-` names
   on the cards, 54 pages under `docs/roadmap/milestones/`**: which set; *critic: the first draft said
   83*), the field `Defect` (needs `admin:org`), the removal of `kind:learn`, the issue types. All
   outward acts the author asks for each time; a GitHub milestone must exist before an issue names it.
9. **The second pass.** Every `defect NNN` a body names becomes a link once all issues exist
   (records.md); what it does for the number issued twice (`014`) and the cards with no number.
10. **Who writes `github:`.** The verb writes the card, the coordinator commits (pathspec commit, hard
    stops): between a creation and that commit the tree is dirty and a killed run leaves issues no
    commit records. The one commit that writes 2,014 addresses runs the **full four-leg CI**
    (`.github/workflows/ci.yml:97-100`: only `site/**` and two workflows are ignored).
11. **What is irreversible, and the rehearsal.** `gh issue delete` exists and leaves a number gap
    (unrun whether numbers are reused); the comments on other projects' issues stay; a staging
    repository is an outward act; so **a dry run proving the rendering without GitHub** is `render`
    and `plan` alone: what is its oracle, given the rule is silent in D1 to D7, and should `render`
    also be a check the `records` suite runs so that the write hook refuses a card the rule cannot
    render.
12. **Where it lives and what it costs**: the module's place (~300 lines a module,
    `.claude/rules/module-shape.md`); `tests/harness/cards.hero` (763 lines) already reads cards in
    Heroes while the compiler's modules cannot import from the harness: share or copy, and who
    notices a drift; a test seam (a transport that can be a recording); and what a fourteenth verb
    costs the site (F6).
13. **A sync after the fact.** Cards are corrected where they stand and GitHub copies text at creation:
    is there ever an `update`/`sync` subverb, and does that decide whether `render` stays pure?
14. **The thesis.** CLAUDE.md § 2: a form enters v1 if the compiler needs it or it provably serves the
    thesis. `measure`, `mutate` and `probe` are the precedents, and `.claude/rules/cli-surface.md`
    says panel 179 ruled `probe` by an author decision *"which the stopping rule was not stretched
    to cover"*; the panels are `docs/panel/016-command-surface.md` and
    `docs/panel/179-the-formatter-s-probe-is-a-verb-and-its-oracle-is-ruled-before-it-lands.md`.

## What binds every seat

- **Your own worktree of the frozen tree**, made with
  `git -C /Users/joseph/Temp/heroes/heroes-lang worktree add --detach /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-212/<seat> 09060ea3`
  *(critic: a `cp -r` of the lane shares the lane's HEAD and index through its `.git` pointer
  file)*, and **your own compiler built inside it from the seed**
  (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.1 s measured by the critic).
  Never the trunk's `./heroes`, never another seat's directory. Nothing outside this repository's
  root is written (CLAUDE.md § Hard stops). Remove nothing: the coordinator removes the worktrees.
- **No request that changes GitHub, no `gh` call but a read** (`gh api` GET, `gh auth status`,
  `gh repo view`, `gh extension search`), **no `gh issue create`, no paid run, no write to the
  import endpoint.** This brief names none.
- Your report goes to `docs/panel/212-reports/<seat>.md` **in the lane tree
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212`**, written as you go. A
  claim that is a number carries its command; a negative claim says what was searched; an inference
  says it is one.
- The coordinator does not touch the lane tree's `docs/panel/212-briefs/` between the moment these
  briefs go out and the synthesis.
