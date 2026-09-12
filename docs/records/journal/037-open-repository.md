# 037 — M-open-repository: the repository opens

## The goal

The repository stops being private, and every sentence that promised it would
open stops promising. The author asked for it on 2026-09-08, out of order: this
milestone was inserted at chain row 38, ahead of M-discard-refusal, and it is
the mirror of the decision of 2026-09-03 that published the site with the
repository shut. `site/README.md` § Launch order named the price of that
decision at the time, the pages that say *not yet*, and this milestone pays it.

Three things the author asked for by name: the site says the language is in
beta, it says which version, and it says that the code can be read but changes
are not accepted. One of the three turned out not to be a thing a repository
setting can do, which is the first finding below.

## What surprised

**The switch that does not exist.** Forking cannot be disabled on a public
repository. GitHub's policy for it covers private repositories owned by an
organisation, and this one had `allow_forking: false` only because it was
private; there is no equivalent for pull requests either. Apache-2.0, which this
project chose in August for the explicit patent grant, grants the fork in
writing regardless. So the shape of the answer changed: the policy is
**declared** where GitHub reads it out, `CONTRIBUTING.md` and a pull-request
template, `main` is protected rather than the door locked, and the refusal
states what the licence still permits, which is the honest half that a bare *no*
would hide.

**The licence re-check disagreed with the record it was checking.** The
publication gate has carried *"the licence re-check on vendored material,
against the upstream repositories rather than against this project's
recollection"* since August, and `vendor/tokenizers/README.md` said the same
thing about itself. Run: `openai/tiktoken` is MIT as recorded. But
`anthropics/anthropic-tokenizer-typescript`, archived and last pushed
2024-03-04, **states two licences in the same commit** — a `LICENSE` file
carrying the MIT permission text under `Copyright 2023 Anthropic, PBC.`, and a
`package.json` at version 0.0.4 declaring `"license": "Apache-2.0"`. `NOTICE`
said MIT for both. Neither reading costs anything, since both are permissive and
compatible with this repository's own Apache-2.0, but a notice that states one
thing where the source states two is not a notice. Both are named now, each
upstream text is vendored beside the table it covers, and **the disagreement is
recorded rather than resolved**: resolving it would mean deciding on an archived
third party's behalf which of its two documents it meant.

**A document that had already written the rule it was breaking.** The fixpoint
drawing in `docs/assets/` carried three measured numbers under a carve-out
granted on 2026-08-31, on the condition that a reader could re-run them from the
file's comment. Nobody did. By the time the README was swept, two of the three
had moved, and the comment's own sentence was *"a graphic nobody re-measures
should not carry the digit that moves"*. The same shape ran through the README
itself, which held six claims the tree contradicted, including a footer saying
the domain holds one page reading *work in progress* six days after the site
went live.

**The instrument's blind spot was documented and still cost five days.**
`site/src/lib/claims.ts` checks numbers and named things against the tree, and
its header states its own scope: it cannot verify a sentence it does not know
about. More than twenty sentences promising a private repository were exactly
that class, and no test, no claim row and no CI step would have noticed them
going false. The repair is not a cleverer check but a list: thirteen forbidden
phrases, seven English and six Italian.

**And the defect the gate names is narrower and worse than its own sentence.**
Panel 030 queued *"`main` cannot fail, so a program that goes wrong still tells
the shell it succeeded"*. Measured here: a program that receives a `fail`,
matches it and prints it exits **0**; one that calls the built-in `exit(1)`
exits **1**; an out-of-bounds index exits **134**. So `exit(code)` entering the
spec did change the answer, and not the way a reader of the old sentence would
guess. **The language has a way to report failure and no way to oblige it**, and
11 of 54 example programs remember to use it.

## What broke and why

**The first exit-code measurement was wrong, in a way this repository has a
written record of.** `./heroes run x.hero | tail` makes `$?` the exit code of
`tail`, so all three programs read 0 and the conclusion would have been that
nothing had changed since panel 030. That is the trap
`docs/environment/windows/WINDOWS-MACHINE.md` documents at :244-261, written
about a Windows shell and met here on a Mac. The second reading redirected to
files instead and the three answers were 0, 1 and 134.

**The renumbering advice given first was an inference, not a measurement.** The
recommendation put to the author was to append the milestone at the end of the
chain rather than insert it, on the ground that 127 `row NN` citations lived in
append-only records. Measured: nearly all of those are panel rows and
spec-budget ledger rows, different tables entirely. The live citations of a
*chain* row at 38 or above are about a dozen in the records and thirteen inside
`docs/ROADMAP.md`. Two of the thirteen were **already wrong before this
milestone existed** — `M-core-packages` cited at row 39 while it stood at 44 —
which is the argument for the repair that was chosen: the citations inside the
living file became **names**, on the 2026-09-03 precedent that a reorder moves a
number and never a name.

**A YAML template broke on a colon inside a sentence.** `description: … say
that: a silence in the spec is often the finding` is not a mapping value, and
the issue form failed to parse. Every workflow and template was then run through
a parser rather than read.

**A commit named fourteen paths and carried sixteen, and its own body said
otherwise.** A parallel session held both landing pages open with its copy work
staged, while this milestone's beta notice sat in the same files, so the plan
was to leave them out and say so. The commit body did say so, and it was false:
`git add <paths>` followed by a bare `git commit` commits **the whole staging
area**, and the two staged pages rode along. CLAUDE.md § Hard stops asks for
exactly the opposite — a commit carries only this conversation's files, because
other sessions share this checkout (CL-041) — and **the procedure written
alongside that rule does not achieve it**. The form that does is
`git commit -- <paths>`, now in `.claude/rules/records.md` and in the case law
as CL-070.

The sweep itself cost nothing, since the author was driving both sessions and
the work was theirs either way. What it cost is a log entry that describes its
own contents wrongly, which §12 forbids, caught only by running
`git show --stat` on a commit already made. The correction is underneath it,
dated, rather than an amended history. **The general shape is the finding**: a
rule and the command meant to enforce it are two different things, and the gap
between them stays invisible for as long as the situation the rule exists for
does not occur.

## What landed, and what carried forward

**The repository is public**, verified from outside rather than from the
setting: unauthenticated, `github.com/heroes-lang/heroes` answers 200, and so do
`releases` and the security policy, the two addresses this milestone's own files
point a stranger at and which returned 404 while it was being written. The
author threw the switch; everything before it was making the switch safe to
throw.

**Ten steps, no sitting.** Opening a repository is none of CLAUDE.md §4's five
triggers, and nothing here touched the language: the spec is unmoved at 3903,
the compiler unmoved at 55,054 lines in 189 modules, the runtime ABI unmoved at
21. Three suites green, 588, 1612 and 121. The net's wall clock is discarded and
the ROADMAP says why.

**No predictions were registered against this milestone**, which the close
checked rather than assumed: it convened no panel and spent no spec token, so
there was nothing to score and nothing to lapse.

**What carried forward**, all of it filed with a home rather than left in this
page: the CI cost premise, whose replacement is a measurement on the account
after a real run and not a reading of a documentation page; the trademark
question, restated and explicitly unmoved by the opening; the `main`-cannot-fail
defect, now measured to three exit codes instead of remembered as one sentence;
and the held-out tasks, which a public repository would burn the day they are
written and which nobody had written that down about. `M-publication-gate` keeps
what it always owned, the thesis measured and the compatibility paragraph that
`1.0.0` is, and its entry now records underneath which of its bullets this
milestone banked.

**Two rules came out of the day and both have executors.** `git commit --
<paths>`, because naming paths to `git add` limits nothing and this milestone
proved it on its own record; and quote-aware command splitting in the guard,
because the first rule's executor read a quoted mention as a command on its
first run. CL-070 carries both, and seventeen shapes hold them.

**The thing worth remembering** is smaller than any of that. The site had told
visitors for five days that the code was shut, in sentences written carefully
and checked by nothing, and the instrument that checks this site's prose says in
its own header that it cannot verify a sentence it does not know about. Numbers
were guarded and promises were not. A promise that comes true is a false claim
exactly as a number that moves is, and it fails more quietly.

## Verified from outside, after the tag

Appended 2026-09-08, after the close, because the proof needed a public
repository and the close needed the proof to be possible rather than done.

**A stranger's path, run as a stranger**: a clone into an empty directory with
no credentials in the environment and no global git config, then the first line
of `README.md` and the three under it.

```
git clone https://github.com/heroes-lang/heroes.git      37.8 s, 161 MB
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    2.89 s
./heroes --version                                       heroes 0.2.0
./heroes doctor                                          every row ok
./heroes run examples/gallery/00-first.hero              20, exit 0
```

On a MacBook Air, which is the machine every timing in this repository comes
from. **This is the sentence the site prints turned into a command somebody
ran**: a C compiler is the only thing you need, and until today nobody outside
this laptop could test it.

**The settings, read back rather than remembered**: public · issues on ·
discussions and wiki off · Apache-2.0 · eight topics · secret scanning and push
protection enabled · `main` carries `deletion` and `non_fast_forward` rules, the
protection a free plan grants only a public repository · workflows from a fork
need approval from every external contributor, not only a first-time one.
Forking stays possible, because it cannot be otherwise and because the licence
grants it in writing.
