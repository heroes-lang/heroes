---
name: step
description: Run one development step of the Heroes compiler autonomously — implement, test, queue what the author owes nobody (/learn) and what the compiler is waiting on (/decide), commit. Chains steps without stopping; use for every implementation step, including in unattended loops.
---

# /step [<milestone> <step>] — the development protocol

The assistant implements; the author's learning is real but **post-hoc**: it
happens in `/learn`, on the author's clock, fed by the queue this skill
writes. Never stop mid-step to ask the author anything. (Learn-first mode —
closed questions before implementing — only when the author explicitly asks
for it before a step; no other ceremony attaches to it.)

## 1. Orient
`git log --oneline -10` · the newest files in `docs/log/` · `docs/ROADMAP.md` status · read
`spec/heroes-spec.md` in full. If no step was named, take the next one from
the ROADMAP.

**Opening a milestone means opening its own file, `docs/work/milestones/<its name>.md`,
which carries its reasoning and the open items that name it.** That list is work with a home — a measurement to run, a
paragraph owed, a row to price — put aside precisely because doing it earlier
would do it against a smaller corpus or a compiler that had not grown into the
question yet. There is no `/scheduled` skill and there should not be one: a third
verb that reads a list `/step` already has to open answers no question the two
existing ones do not (CLAUDE.md §10's stopping rule, applied to the skills). What
the list needs is not a caller of its own, it is *this line* — without it the
file is write-only, which is the failure mode of every list nobody is obliged to
open.

**Every item that gets done in the step is ticked with what closed it and MOVED
to a NEW FILE in `docs/done/` before the commit** (never a line appended to
`docs/work/DONE.md`, which has been the map since 2026-09-12). A live list holds open items only;
the record holds everything else. That rule existed in three documents and in no
skill until 2026-08-26, and by then `docs/work/DECIDE.md` held 138 ticked items
and zero open ones while the record had not been written to in eight days. The
same applies to an item this step *finds* rather than finishes: it is filed as
`- [ ]` in the list that matches what it asks — `SCHEDULED.md` if it names a
milestone, `DECIDE.md` if it names a default the compiler is running on,
**`DEFECTS.md` if it is BROKEN** (a crash, a wrong answer at exit 0, a silence
where a message is owed), `docs/work/learn/LEARN.md` if it only asks what is true —
and **never as a bare bullet or a `## ` section**, which is a notation no count
in this project can see. **`DEFECTS.md` was missing from this list until
2026-09-07**, four days after the author instituted it, so no skill read or
wrote the fourth list and an open defect was invisible in `/where`'s own status
report: CLAUDE.md § 3's story about a rule with no executor, told about § 3's
own list (`docs/contract/case-law.md` CL-044).

**The shape of a filed item, since 2026-09-07:** one line of three fields,
`- [ ] **<first field>** | <what, in one line> | <where to look>`, then an
optional body indented four spaces opening with `**Origin:**` and its date, and
the whole region fenced by two lines of asterisks (`docs/work/DECIDE.md` carries
the rule and `/decide` § 6 the reasoning). The first field is what the
instrument reads: a milestone in `SCHEDULED.md`, `panel NNN` in `DECIDE.md`, the
number in `DEFECTS.md`, the origin in `docs/work/learn/LEARN.md`. **When an item is
added, the `**OPEN: N**` line under the banner moves with it** — `records/lists`
compares that number to the items it counts, so the count cannot drift the way
one in a second document does. **A line that will carry a path into a body needs its
date on that same physical line** — `records/citations` reads one line at a
time, and re-wrapping a paragraph moved five dates off their paths the day this
shape landed, which the check caught before the commit. `records/lists` is what
fails when any of it is not done.

## 2. Implement
- If the step touches a panel path (CLAUDE.md § 4), run `/panel` first, and
  asynchronously: adopt **the provisional resolution CLAUDE.md § 4 requires**,
  the most robust and complete one and never the cheapest, then queue the
  ratification. This line said *the conservative default* until 2026-09-07
  (`docs/contract/case-law.md` CL-040). **Ask once per milestone**, then convene
  without asking again.
- Write the code and the golden cases. Mark the milestone's 5 adversarial
  cases `# UNVERIFIED — pending debrief`; label bulk regression cases.
- **Format at the moment of writing**, `heroes fmt <file> --in-place`. Two of
  the four failures that stopped a full net on 2026-09-06 were a file written
  and not formatted, which costs nothing here and thirteen minutes there
  (`docs/contract/case-law.md` CL-063). A `PostToolUse` hook notices it too.
- Run the tests. **The gate for a SUB-STEP is the named suites, one at a time,
  plus the compiler's own tests and the net's own tests**; the full net runs
  once before a push and not at every commit (CLAUDE.md § Verification, CL-063).
  Naming a suite runs only that one, which is what makes a fast loop possible:
  `-- <compiler> records`. The three suites since **M-bootstrap-archive**
  (2026-08-19), with their counts in `docs/ROADMAP.md` § Where we are and
  nowhere else:
  - `heroes test selfhost/main.hero` — the compiler's own;
  - `heroes run tests/harness/main.hero -- <compiler>` — the net, which carries
    the double-emit determinism diff, the ASan `run/` goldens, the blessed
    emissions and the file ceiling inside it;
  - `heroes test tests/harness/main.hero` — **the net's own tests**, the one
    that goes red when an instrument's pinned number stops matching what the
    instrument reads.
  **And run the suite you did not expect to move, after the last edit rather
  than after the last interesting one** (CL-054): *my change cannot have touched
  that* is an inference, so it is either run or written down as a guess.

  **This line said "the two" until 2026-09-02 and the third one was red for six
  commits.** `9599d97` re-baselined `suite_layout`'s ceiling table and left two
  asserts reading the old numbers — and that commit's own diff *states both new
  numbers in prose*, two screens above the asserts. `DESIGN-LOG` had already
  recorded the identical failure on 2026-08-31, *"a suite that had never been
  run"*, 83 of 90 while the other two were green, and recorded the lesson without
  giving it a caller. The command existed the whole time, in
  `docs/ROADMAP.md`'s verify block, which nothing obliges anybody to open. A rule
  written in one document and performed by nothing is CLAUDE.md §3's own story
  about `DECIDE.md`, and this is that story in the test suites.

  `cargo test` and `cargo clippy` were this line until the archive; the Rust they
  ran is `archive/bootstrap-rs/` and nothing builds it. Naming a suite runs only
  that one, which is what makes a fast loop possible: `-- <compiler> records`.

## 3. On failure
Diagnose and fix autonomously. Record symptom → cause → fix in the milestone
journal, and queue the **raw symptom only** (golden diff, clang error, panic)
plus the fixing commit's hash — so the author can hypothesise in `/learn`
before reading the fix.

## 4. Queue comprehension — per new concept, not per step
When a step introduces a new concept (first tokens, first tree, first types,
first blocks, first C…), append 2–4 closed-form items (a count, a choice
among structures, an output value) to the END of `docs/work/learn/LEARN.md`, which
appends at the bottom since 2026-09-12 (it appended at line 31 before, so every
filing touched one line):
`- [ ] <origin> | <question / task> | <where to look> | <why it matters>`
Plumbing steps — CLI, harness, refactors, bulk cases — add nothing.

## 5. Close
**A lane does not close a milestone.** One milestone, one file, one worktree is
the shape since 2026-09-12 (`.claude/rules/records.md` § Working in lanes); a
close re-measures the whole tree for § Where we are, and two lanes cannot both
be right about it. Closing is an act on the trunk, and closes accumulate into a
train.

Every step ends with a commit: `M-<name> step <k>: <what>`. The naming algorithm
for `<name>` is CLAUDE.md §14 — its only home.

Milestone close — the checklist (this is its only copy):
- goldens pass (ASan-clean where applicable); determinism diff empty (from M-scalars-run on);
- journal `docs/journal/NNN-<slug>.md`, 3 sections: **goal** · **what
  surprised** (impersonal — shapes and rules, never scores) · **what broke
  and why**;
- one story beat, **a new file in `docs/book/beats/`** named
  `YYYY-MM-DD-HHMM-<slug>.md` and holding the one line
  `date | milestone | the beat`. Never a line appended to `docs/book/beats.md`,
  which has been the MAP since 2026-09-12: it keeps the line count the record
  had so that a `beats.md:NNN` citation still resolves, and `records/rotated`
  goes red on a line added to it;
- one `docs/log/` entry per decision made, a new file named
  `YYYY-MM-DD-HHMM-<slug>.md` and holding the line
  `date | decision | reason | design.md § | panel`. `DESIGN-LOG.md` has been the
  MAP since 2026-09-12 and is never appended to;
- **score every prediction whose milestone this is**, and lapse the ones you
  cannot: `grep -n "$(git describe --tags --abbrev=0)\|<this milestone>" docs/work/milestones/
  docs/measurements/010-spec-budget-ledger.md docs/panel/*.md`. (It named
  `crates/heroes/src/measure/gate.rs` until 2026-08-23 — doubly dead: the tree is
  archived, and panel 086 moved the ledger's 38 rows out of that doc comment into
  the record above, where `tests/harness/suite_spec.hero` locks them to
  `SPEC_TOKENS`.) A prediction is scored, or
  it is marked `lapsed` in the ledger row that spent it and the clause it bought
  goes back to `docs/work/DECIDE.md` to be re-argued under the removal branch — **never
  renewed with a new milestone name** (panel 046 R2). This bullet exists because
  the mechanism failed without it: panel 036's *"Score at M-ffi-ladder close"*
  survived that milestone's close untouched;
- **WRITE** the milestone's `/learn` offers into `docs/work/learn/LEARN.md` as items
  (walkthrough, golden ratification, mutation drill, exit-quiz), and **do not
  put them to the author**. `/learn` is never convened by the assistant, not at
  a milestone close and not as a suggestion at the end of a step; this bullet
  and that rule used different verbs for the same act until 2026-09-07
  (`docs/contract/case-law.md` CL-044 is the neighbouring shape, and the rule is
  CLAUDE.md § 3);
- append the closing block — the status paragraph and the milestone's chain entry —
  to `docs/journal/NNN-<slug>.md` § *What landed, and what carried forward*, and
  leave `docs/ROADMAP.md` § Where we are at **≤15 lines**. The ROADMAP says what is
  *next*; a closed milestone's record is its journal. (It reached 935 lines before
  this rule existed, growing ~66 per close, and the reader met 512 lines about the
  past before the first line about the future.)
- update `docs/ROADMAP.md` § Where we are and § The chain — the closing row takes
  its date, tag and journal link, the next row becomes `**OPEN**`, and the counts
  in the summary table are **re-measured, never carried** (CLAUDE.md §1); then the
  milestone's own section under § The milestones, one by one keeps only the
  reasoning a future milestone has to honour, the rest going to the journal;
- **tag `m-<name>` LOCALLY, and stop there.** This bullet said *push
  `--follow-tags`* until 2026-09-07, and that command pushes the branch as well
  as its tags: the site lives on this branch and
  `.github/workflows/deploy-site.yml` publishes it on any push touching `site/`,
  `examples/`, `spec/heroes-spec.md` or `selfhost/keywords.hero`, so a close
  that followed this checklist to the letter published the site without asking. The push is asked for, with how many site
  commits would travel, and it carries the tag when the author says yes
  (CLAUDE.md § Hard stops, `docs/contract/case-law.md` CL-042);
- site build log: only when the author asks (`site/README.md`).
