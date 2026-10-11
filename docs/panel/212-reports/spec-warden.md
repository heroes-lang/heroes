# Panel 212, spec-warden report

Written 2026-10-11 in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-212/spec-warden`, a detached
worktree of `09060ea3f` (`git log -1 --format=%h` reads `09060ea3f`), with a compiler built there from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.3 s). No `--refresh`, no key, no request that
changes GitHub, no `gh` call at all. Every number names its command; what I could not run says *unrun*.
One file of mine is left in that worktree and nowhere else: `tests/harness/zz_r11.hero` (R11's proof, below).

## The structure the agent definition asks for

- `verdict`: **object** to R1 as proposed (a fourteenth verb `issues` with `render`, `plan`, `push`). **Not
  provisional on the two budgets**, whose delta is zero by the absence of a sentence; **provisional on the help text's
  figure**, which is a candidate text priced on the vendored tables only. No veto on the budget. **I hold a veto on
  Principle 0's burden and I convert to it** if the resolution claims the verb *enters by the stopping rule* or by
  Principle 0, which both fail below; it stands only as an author decision of the capability, the way `probe` did.
- `section`: design.md §1.6 (budget, delta zero), §1.2 (no program token moves), §3.5 (*neither does a Makefile, a script,
  or a separate ... tool*), CLAUDE.md §2 and §10 with `.claude/rules/cli-surface.md` (the stopping rule, *no fourth input
  class*), CLAUDE.md § Hard stops (outward acts asked for every time).
- `spec_token_delta`: **0 / 0, measured as recorded, not re-priced.** `./heroes measure spec/heroes-spec.md` (offline,
  exit 0): claude-legacy 7515, cl100k_base 7650, maximum 7650, **real 10044 (claude-opus-5, 2026-10-11)**, headroom 196
  against 10240, 136 once the FFI floor's 60 is deducted; no STALE line, so the file's digest still matches the pin.
  `./heroes measure CLAUDE.md`: legacy 6071, cl100k 6174, **real 8167 (2026-09-29)**, ceiling 12288. The proposal has
  no spec sentence and, as I find below, needs no CLAUDE.md one. A changed document could only be re-priced by
  `--refresh`: **unrun, not allowed here.** The one token figure it does move is the help text (item 3 of the
  questions): **+83/+88 vendored for three bare subverb lines, about +100 to +130 with flags**, on a help that is 80
  lines and 1245 vendored today (`./heroes --help > f; ./heroes measure f`), a text nothing budgets (panel 016's watch
  list: *the help text has no budget*).
- `removal`: **nothing from the spec, and that is not a problem: no spec token is bought.** What should come out is the
  duplicate. `tests/harness/cards.hero` (763 lines) and `credit.hero` (640) already read a card in Heroes; a verb in
  `selfhost/` cannot import them (the brief's F7/question 12), so it would be a second reader of the same cards. R1
  buys a copy of 1,403 lines' worth of reading, and the record says *a drift nobody notices*. If R1 lands, the removal is
  the harness's copy, which is a refactor nobody proposed.
- `needed_for_self_hosting`: **no.** The compiler compiles itself without it (`git grep -n -E 'issues/' -- selfhost/cli selfhost/main.hero`
  finds only comment citations of issue files, no module that reads the folder).
- `argument`: below, 120 words.
- `prediction`: below.
- `condition`: below.

## Argument (under 120 words)

No stopping-rule clause is met: fixpoint, harness and Part 11 need no such verb. Panel 179 said the rule *was not stretched to cover* `probe`; the author decided that capability. The author's 7a sends only *the verb that renders an issue file* to a sitting, so `plan` and `push`, which write to a third party, have no decision behind them, and would run behind the pre-approved `Bash(./heroes *)` where `git push` is asked. `render` is a harness instrument, as `credit.hero` shows without a verb. R11 runs today: a program that `use`s `cards` read an issue and fed a child its file on stdin, exit 0. R1 adds a table row and a site push for an act done once.

## Prediction (falsifiable, instrument exists today)

1. **If R1 lands**: `./heroes --help | wc -l` reads **at least 86** (80 today) and `./heroes measure` of the saved help
   reads a vendored maximum of **at least 1330** (1245 today); `git grep -n -c -iE 'thirteen|tredici' -- site/` still
   lists the two start pages but both now say fourteen/quattordici, and `npm run build` in `site/` goes red between
   the table row and the page edit (the claims check, `claims.ts:448, 450`). **If R11 is chosen**: the help stays 80 lines
   and 1245, `grep -c 'Command(' selfhost/cli/table.hero` stays **13**, and the site is untouched.
2. Spec real stays **10044** and CLAUDE.md's pin stays **6145** (vendored max 6174, drift 29, window 50) either way,
   scored by `heroes measure` on the landing commit.
3. §1.2: the rewrite rate of Heroes programs does not move under either route, **0 change**, because no program can contain
   the verb; so the verb pays for no tokens under §1.2 and owes its keep to Principle 0 or to the author.

## Condition

My verdict becomes **approve** for `issues render` alone, as a verb, if the author decides that capability in words
(the way 2026-09-26 decided `probe`) **and** the resolution rules `plan`/`push` out of the binary or into R11, **and**
`.claude/settings.json` and `guard_bash.py` gain the outward-act refusal below in the same commit. It becomes **veto**
if the synthesis says the stopping rule or Principle 0 admits the verb, or if a push that publishes the site
(`deploy-site.yml`) goes out with `fourteen` before the author's ratification is recorded. It stays **object** for R1 as
proposed on any other reading. What would make my refusal of the verb wrong is in § 3.

---

# The brief's four questions

## 1. The spec

Offline `heroes measure` prints the recorded row and exits 0; the numbers above are its output. The proposal names no
spec sentence (`grep -n -iE 'subcommand|heroes (check|build|run|fmt|test|measure)' spec/heroes-spec.md` finds only two
table rules at lines 62 and 165, which are `|---|---|`), so **the committed spec's real row does not move**. Said
exactly as the brief asks: a *changed* spec could only be priced by `--refresh`, unrun.

## 2. The contract

`CLAUDE.md` would need **no sentence**. Its § 3 and `.claude/rules/cli-surface.md` already name the sitting; § 10 says
*any new capability is a `heroes` subcommand or flag*; `.claude/rules/records.md` § The issues says *until a `heroes`
verb renders it* and is where the amendment goes (that file loads on touch and is judged by neither document's gate).
What a landing would amend instead: `records.md`'s paragraph (*What an issue becomes on GitHub*), and
`cli-surface.md` (the sentence panel 179 added for `probe`, because this verb's reading of the stopping rule needs one too).

**The headroom, if CLAUDE.md did need a sentence**: the contract is judged by drift, not digest
(`selfhost/cli/measure.hero:138-150`). Pinned `CONTRACT_TOKENS` is 6145 (`tests/harness/suite_spec.hero:182`), the
vendored maximum reads 6174 today, drift 29 against `DELTA_GATE` **50** (`suite_spec.hero:189`). So **21 vendored
tokens** of CLAUDE.md can be added before `measure` reports STALE and the `contract` check goes red until a paid
`--refresh`. Any sentence longer than about a line and a half is therefore a refresh, which this sitting cannot run. The
real figure for the current text is not measured (8167 was taken 2026-09-29); by the 8167/6145 ratio it would read near
8205, an inference.

**The finding the brief did not ask for, and it is a hard stop.** `.claude/settings.json` allows `Bash(./heroes *)`
(line 4 to 28 of `permissions.allow`) and asks only `Bash(git push*)`; it also allows `Bash(/usr/bin/time -p ./heroes *)`,
`Bash(./heroes-next *)`, `Bash(xargs -P 8 ./heroes *)` and **`Bash(caffeinate -i *)`**. CLAUDE.md § Hard stops makes
*anything outward-facing* asked for every time and says the settings enforce it (CL-041: until 2026-09-07 the hard stops
were prose and the local settings pre-approved the git ones). Today a hand transcription goes through `gh`, which no
allow line covers, so each call is a prompt. **A verb `heroes issues push` would run 5,918 writes behind a prefix that is
pre-approved**: the verb removes exactly the friction that was the confirmation. R11's `./heroes run <path>.hero` is the
same prefix. So any route needs, in the commit that lands it, an `ask` line for the push form **for every wrapper
prefix in the allow list** and a rule in `.claude/hooks/guard_bash.py`, which reads wrappers (`env`, `time`, `caffeinate`,
`timeout`, `-c`) as the shell runs them (`verification.md` § Layer 1). **Unrun: whether the permission system lets
`caffeinate -i *` swallow an inner `ask`**, I cannot test it here; the inference is that it can. The one network
precedent in the compiler, `heroes measure --refresh`, reads and prints constants; it writes to nobody, so it set no
precedent for a verb that writes to a third party.

## 3. Principle 0 and § 10's stopping rule, honestly

The 13 verbs (`grep -c 'Command(' selfhost/cli/table.hero` reads 13, lines 150 to 162), and which suite types each
(`git grep -l -E '"<verb>"' -- tests/harness`, a literal search, so it can miss a verb built from a variable):

| verb | serves | typed by the harness | note |
|---|---|---|---|
| `lex` | the language (pipeline stage) | `suite_spec.hero` | |
| `parse` | the language | not found by my grep | a literal-search miss, unrun otherwise |
| `check` | the language | 8 modules | |
| `build` | the language | 14 modules | |
| `run` | the language | 7 modules | |
| `test` | the language | 3 suites | |
| `fmt` | the language | `suite_canonical`, `suite_special`, `shell` | |
| `doctor` | the person running programs (toolchain) | `shell.hero` | |
| `measure` | the thesis (§1.6 forcing function) | `suite_spec.hero:541, 571, 602, 623` | judges the spec and the contract |
| `mutate` | the thesis (Part 11 metric 3) | `shell.hero:1958, 2003` | |
| `grammar` | the language's own tables | `suite_grammar.hero:59` | |
| `probe` | the project (the formatter's soundness) | `suite_probe.hero`, `shell.hero` | **by the author's decision of 2026-09-26** |
| `this` | the language's prose | not found by my grep | |

So **eleven or twelve of thirteen are typed by the harness or serve the thesis directly, and the one that is neither is
`probe`, admitted by an author decision that the rule says it was not stretched to cover.** `issues` would be the first
verb whose subject is the project's tracker and the first that **writes to a third party**.

**The arguments that admitted the second kind, quoted.** Panel 016 (`docs/panel/016-command-surface.md`, § What changed
the proposal, item 6): *the trigger for a new verb was a proven overload, never a new capability*; its historian's
sources for holding the line were a namespace for the long tail, an external extension point, and removal and
re-adding. `cli-surface.md`: *A capability enters the surface only if the fixpoint invocation, the golden harness or the
design.md Part 11 harness must type it, or it has a measured Part 11 effect*, and on `probe`: *the capability entered by
the author's decision of 2026-09-26, which the stopping rule was not stretched to cover*. Panel 179 (resolution item 1):
*The admission is the author's decision of 2026-09-26, stated as that and not stretched from the stopping rule*, with
*the author decided the capability, the sitting its artifact class*. The critic's finding about my brief holds
(`git grep -l 'heroes measure'` is broad); I found no sitting that admitted `measure` by the rule either, only the rule
being written around the verbs that existed on 2026-08-04.

**Does this verb stand on the same ground? Not as proposed.** (a) Clause 1 of the stopping rule: nothing in the fixpoint,
the golden harness or Part 11 must type `issues`. The brief's route to clause 2 (make `render` a `records` check so the
write hook refuses a card the rule cannot render) is **a need the sitting would create by deciding it**, and it is also
met without a verb: `credit.hero` is a 640-line module run by `records/prior-art` and by the write hook, with no verb
(`records.md`, *the executor is `records/prior-art` (`tests/harness/credit.hero`)*). (b) The author's decision: the author's
7a of 2026-10-05 reads *The verb that renders an issue file goes to a sitting when the author decides to transcribe*
(`issues/2026-10/05/2026-10-05-0027-the-author-answers-1c-2a-3a-4a-5a-6b-7a.md`, item 7); the 2026-10-06 instruction says
*it is fine to send them to the issues*. Neither decides that a verb **pushes**. Whether *the verb that renders* means
`render` alone is the coordinator's to ask; I read it so. (c) **No fourth input class** (`cli-surface.md`): `render <file>`
takes an argv file, which the three classes allow. `plan [path...]` walks a folder of cards and reads GitHub, and
`push` reads a token and writes the network; that is an input class the rule's three shapes do not list, and panel 056's
three return conditions (one key, no string shared with a `.hero`, one binding `package` cannot build) concern a
build file and are not met, which `cli-surface.md` calls the only amendment path. (d) §3.5's *a separate ... tool*
refuses a tool beside the binary; it does not say whether a project tracker belongs inside it.

**What would have to be true for the refusal route to be right (R11, with R3 and R6 behind it), and the program or
compiler fact that would make my refusal wrong.** I ran the first half. `tests/harness/zz_r11.hero` in my worktree:

```
use cards
use shell
... issue = cards.issue_of(path: path, text: text.must())
... ran = shell.run_fed(scratch: ..., program: "wc", words: ["wc", "-c"], stdin_path: path)
```

`./heroes run tests/harness/zz_r11.hero -- issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`
exits 0 and prints `kind=decision filed=2026-10-06 open=false bytes=2874` then `child exit=0 out=    2969`, and
`wc -c` of the file reads 2969: **a Heroes program can read a card with the harness's own reader and spawn a child with the
whole file on its standard input**. That answers the brief's F7 objection (*`process.run` hard-codes the standard-input
path to empty*) for this route: `shell.run_fed` takes a stdin path (`tests/harness/shell.hero:144`), so `gh api --input -`
is reachable without touching `selfhost/cli/process.hero`. Unrun: that the spawn reaches the network, a write I may not do,
and which directory such a program lives in (a second `main` beside `tests/harness/main.hero` is a layout and `records`
question I did not test). **My refusal is wrong if** (i) the author decides the capability in words, as for `probe`;
(ii) a Heroes program run by `heroes run` cannot do something the verb can, and the candidate is **the secret**: a spawned
`curl` with the token in neither argv nor a file (`--variable`, `--expand-header`, as `selfhost/cli/refresh.hero:81` does)
may not be reachable from a program that `use`s only the harness's `shell`, whose `run_fed` takes words and a path; that is
**unrun**; (iii) the transcription becomes a standing flow, files authoritative for good (`records.md`: *an issue that
disagrees with its file is the one corrected*), and the author wants it as a command a stranger can type; then the
question is not whether but where the renderer lives, and a renderer in the compiler is the only place `heroes` the
binary can offer it to a clone without the harness. I would then read it as a verb of the `measure` kind and not the
`probe` kind. (iv) A measured Part 11 effect, say that GitHub issues raise a Heroes-writing model's task success; I know of none.

## 4. The site

`git grep -n -iE 'thirteen|tredici' -- site/` lists **22 lines in 12 files** (`git grep -n -c` per file: README 4,
style.css 1, it/log 2, it/start 1, it/why/not 1, log 2, start 1, why/not 1, claims.ts 4, figures.ts 1, nods.ts 3, spec-page.ts 1).
**Only two are the verb count**: `site/src/html/start.html:100` (`One command, thirteen verbs`) and
`site/src/html/it/start.html:100` (`Un comando, tredici verbi`); the other 20 are *thirteen numbers over thirteen doors*,
the borrow checker's thirteen of seventeen functions, CSS widths, and word tables. A grep for `thirteen` alone finds 11
and **misses the Italian start page**, which is the brief's point; `tredici` is the Italian word. `claims.ts:325, 328`
already hold `fourteen` and `quattordici`, so no table edit. Edits for a fourteenth verb: **two heading lines and two
verb lists, four lines in two files**: `start.html:100` and its list (lines 102 to 106, ending `<code>grammar</code>,
<code>this</code>.`), `it/start.html:100` and its list; the check requires `<code>issues</code>` on both
(`claims.ts:491` `NAME_EVERY_VERB`, `:607-611`), and the count word on both (`:448, :450`, and the unambiguous-noun
rows `:690-691`). The site's build is the only judge and **no suite of the net runs it** (`verification.md`, the 2026-09-28
deploy that was refused): `npm run build` in `site/` before the push. Unrun here.

**What a push that publishes the site costs this milestone.** `deploy-site.yml`'s `paths` are `site/**`, `examples/**`,
`selfhost/keywords.hero`, `spec/heroes-spec.md` and itself; the verb's landing touches `site/` and so the push publishes.
At this HEAD the trunk has **0 commits to push** (`git log --oneline origin/main..main | wc -l` reads 0, and 0 of them
under `site spec examples`), so the verb's would be **one site commit travelling alone**, one deploy, whose job the workflow
describes as under a minute and free in a public repository (its header, unrun). The three real costs are elsewhere: (1)
**sequence**: the author asked the transcription to be *the very last act*, and the verb's landing, the full net (the
batch gate), the push and the CI's four legs (`ci.yml:97-100` ignores only `site/**` and two workflows) must all precede
it, so the push that publishes `fourteen verbs` goes out **before the first issue exists and before the author's
ratification of a provisional resolution is recorded** (CLAUDE.md § 4), which publishes a count the author may overturn;
the site edit must therefore be the last commit of that push and never earlier. (2) **The commit that writes 2,014
`github:` addresses runs the whole four-leg CI** (the brief's question 10). (3) **A stranger's view**: the start page's
sentence *All the tools are available through heroes* would list a verb that writes to this project's own tracker, the
only one a person writing programs never needs.

## What I found that is outside the four questions

1. **The brief cites `tests/harness/argv.hero:333, 393`; that file does not exist.** `git ls-files | grep -E 'argv\.hero$'`
   gives only `selfhost/cli/argv.hero`. The verbs a fourteenth would touch in the compiler are `selfhost/cli/table.hero`
   (tag enum at line 28 to 30, the `Command` row, the flags), `selfhost/cli/argv.hero:123` (operand handling),
   `selfhost/main.hero:200, 250, 288` (dispatch and two verdict tables), plus a new module and its tests; the seed is then
   regenerated at the batch's close. I did not find the lines the brief means.
2. **The brief's F7 says a spawned program cannot be fed standard input.** True of `selfhost/cli/process.hero`, false of the
   harness's `shell.run_fed` (shown above), which matters because R11 and R1 differ here.
3. **The help is a reader's cost under §1.2 that nothing counts.** The three-line candidate I priced
   (`/private/tmp/claude-501/.../scratchpad/sw/cand.txt`, 83/88 vendored) is my text, not a landing's; the existing `probe`
   block of four lines (name, summary, two flags) reads 97/100.
4. **A verb ships in every user's binary.** `heroes` is 12,645,496 bytes here (`ls -la heroes`); binary size is not a goal and I
   do not weigh it, but a repository-specific writer (`heroes-lang/heroes` hard-coded) in the user's compiler is the one
   feature whose audience is one maintainer. A parameter for the repository is a second feature.

## Commands, so each number is reproducible

```
./heroes measure spec/heroes-spec.md          # real 10044, 2026-10-11, headroom 196
./heroes measure CLAUDE.md                    # real 8167, 2026-09-29; vendored max 6174
sed -n 180,190p tests/harness/suite_spec.hero # CONTRACT_TOKENS 6145, DELTA_GATE 50
sed -n 138,150p selfhost/cli/measure.hero     # drift window
grep -c 'Command(' selfhost/cli/table.hero    # 13
./heroes --help > f; ./heroes measure f       # 80 lines, vendored max 1245
git grep -n -iE 'thirteen|tredici' -- site/   # 22 lines; the two verb counts are start.html:100 and it/start.html:100
git log --oneline origin/main..main | wc -l   # 0
sed -n 1,48p .claude/settings.json            # allow ./heroes *, ask git push* only
./heroes run tests/harness/zz_r11.hero -- <issue file>   # R11: reads a card, feeds a child its file
```
