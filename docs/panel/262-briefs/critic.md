# Panel 262, the completeness critic

Two readings, each its own report under `docs/panel/262-reports/`, written in your own copy and copied to the trunk's
same path when you finish or stop. You give no verdict on any candidate. Your copy is a detached worktree the
coordinator makes, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-critic` (or the path the coordinator's
prompt names), at the base the briefs name: **`aded91388` for the first reading** (done, `critic-briefs.md`), **`87003cb42`
for the second** (the repair moved the sitting there, through `c18a9a9a3`, `00-shared.md`'s head); build its compiler from the seed and stage
2 there (`00-shared.md` § 6; stage 2 should read `sources 6ce3f9e116d22e3a` at
`87003cb42`, `7205f5b93a1fc8f1` at `aded91388`). **You hold this Mac and both Linux images** (`heroes-linux-arm64:latest`,
`338a1044ac90`, and `heroes-linux:latest`, `6f78598a4eba`, run with `docker run --pull never`; the arm64 image has no
`/usr/bin/time`), one container at a time, `--rm`, named `p262-critic`, removed when idle. **No paid run is yours to
start** (the blind seat's run is not authorised, `llm-ergonomist.md`) **and no credential yours to print**; no GitHub
write (`gh` reads only); no file outside the repository's root (`TMPDIR` inside your copy; no `/tmp`); no `pkill`;
never read, build or run inside a seat's copy, the blind seat's area, or **lane sig-family's worktree and scratch**,
which are repairing the runtime this sitting describes.

## First reading: the briefs, before any seat starts (`critic-briefs.md`, done 2026-11-26 15:07 to 15:46)

Read `00-shared.md`, the five seats' briefs (`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`), `blind/registration.md`, `blind/task-t1.md`, `task-t2.md`,
`blind/brief-session.md`, `blind/programs/`, `blind/key-mac/`, this file, and `ground/`; then the sources they cite
(spec § 13 lines 401 to 428 and § 6, § 9 line 331; `runtime/parts/sigguard.c` whole, `sigentries.c:1-140`,
`sigjump.c:1-82`, `thread.c:120-144`; `tests/harness/spec_refusals.hero` whole; `selfhost/emit/callback_thunk.hero:395-429`;
`docs/panel/257-*.md` whole and `257-reports/spec-x1.txt`, `spec-x2.txt`; `docs/panel/259-*.md:120-160` and
`:290-323`; `259-reports/llm-ergonomist-blind.md` whole; issues 2706, 2848, 2850, 2851, 2852, 2853 and 2855 with `gh
issue view`; `.claude/worktrees/rebel-scratch/coord/sig-family-brief.md`). Then:

- **re-run every framing fact by command**: each number, path, line, count and quotation; the clerk's narrowed `run`
  cases and probes (`ground/probes.sh <your stage 2> <out>` from your copy's root) **and on Linux arm64**, which the
  clerk did not run; the census (`ground/census.sh`); the counts of § 3.7; the blind key (`blind/key.sh`) on both
  platforms; and **the lane's state at the moment you read** (`git log --oneline main..lane-sig-family`, `git log
  --oneline -15 main`): the clerk read it between 14:13 and 14:31, and steps may have landed since;
- **search or question every negative sentence and every claim about the option set**; **read the blind registration**
  for what it measures; **say which seat was handed a fact it cannot check**; **name the route or the shape nobody
  listed**, and **the reading nobody named**;
- end with `## What the coordinator must repair before any seat starts`, most important first.

## Second reading: the reports, before the synthesis (`critic-reports.md`)

You are a **fresh agent** in this reading, launched after the seats; this file is then your brief again. Read the
briefs as repaired (`00-shared.md`'s head says what the repair did and where), your first reading, then every report
under `docs/panel/262-reports/` and the programs the seats kept under `programs/<seat>/`. **Check the repair of the
briefs against your first reading's last section, item by item**, and against its C1 to C28: each repair the briefs
say they applied, whether it was applied, and **whether a number the repair wrote is the command's** (the repair's
outputs are under `ground/`, `blind/key-mac-repair/`, `blind/labels-259/` and `ground/linux-arm64/`; its scratch is
`.claude/worktrees/panel262-repair-scratch/`). **Ask `main` again** (`git log --oneline 87003cb42..main -- runtime
spec`): a lane's merge after the base changes the ground, and a seat that wrote a sentence on the base alone where
`main` had moved owes its second column. Name what is missing: a candidate nobody built, a clause asserted and not run
with the program that settles it, a contradiction between seats and which side is checkable, a repair that is itself
wrong. **Run each warden text's promises** against the compiler-engineer's and the ffi-pragmatist's programs on the
platform they did not use, **and against the falsifiers of `00-shared.md` question 3** (2705, the own-stack
coroutine, panel 258's jump refusal, route A, the thread), and **three shapes of your own** beside the sentence's
exits. Then: does the text the seats would adopt promise only what runs, on every platform, route and thread it
claims, and with the lane's repairs at the state the landing will meet them? Does it leave neither 259's reader nor
257's reader, nor a reader of the list as exhaustive, wrong on the blind programs, by your own reading of the text
against the key? Do the spec-warden's registered predictions read the registration's rules as repaired (D2's fixed
set, D5, the labels)? **Grade the labels of panel 259's 48 archived wrong answers yourself, apart**
(`blind/labels-259/units.txt`; the answers in `docs/panel/259-reports/llm-ergonomist-blind-answers.tar.xz`, unpacked in
your scratch, never in the tree), without reading `graderA.tsv` first, and report your agreement with grader A per
label: the second reader the repair could not give. Does every precedent the historian marks `verified` quote its
page? End with `## What the synthesis must settle`, most important first.
