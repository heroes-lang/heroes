---
paths:
  - "selfhost/**"
  - "tests/**"
  - "examples/**"
  - "spec/**"
  - "docs/**"
  - "issues/**"
  - ".claude/**"
---

# Which suites judge a change, and what may run beside them

Home of CLAUDE.md § Verification's two additions of 2026-09-09 (author
instruction). What each rule cost to learn is in `docs/records/contract/case-law.md`,
cited as `CL-NNN`. § Verification keeps the rules; this file keeps the map and
the reasoning, because a map is long and a rule is short.

## The map is not written here. The command that produces it is

**A list of which suite reads which directory is a premise about the world and
it expires in silence** (`.claude/rules/module-shape.md`). So the map below is
today's ANSWER and the command above it is the instrument. Re-run the command
rather than trusting the table:

```sh
# the suite names the net registers, which are what `-- <compiler> <name>` takes
# (one word it prints, `harness`, is not a suite; the count is the command's)
grep -oE '"[a-z_]+"' tests/harness/main.hero | sort -u

# what each suite file walks, from its own constants — the literal paths, and
# the bare group names of a suite that builds `"tests/golden/" + group`
for f in tests/harness/suite_*.hero; do
  printf "%-14s " "$(basename $f .hero | sed 's/^suite_//')"
  { grep -ohE '"(tests/golden/[a-z-]+|tests/emission|examples|selfhost|seed/heroes\.c|spec/[a-z-]+\.md|docs/[a-z/]+|issues)"' "$f"
    if grep -q '"tests/golden/" +' "$f"; then
      for d in $(ls tests/golden); do
        grep -qE "^[[:space:]]*\"$d\"[[:space:]]*$" "$f" && echo "\"tests/golden/$d\""
      done
    fi
  } | sort -u | tr '\n' ' '; echo
done
```

**`golden` is not one of the twenty**, measured 2026-09-09, and finding out
why corrected the map: `suite_golden.hero` runs as **four FORMS**, and their
names are the selectors — `check`, `ir`, `emit`, `unsupported`
(`tests/harness/main.hero`, the `forms` list in `main`). So `-- <compiler> golden` selects nothing
and prints no line, and a suite name that selects nothing is a green run that
tested nothing. That is the failure this paragraph exists to prevent, and it
happened here first. **Five forms since 2026-10-04**: `permissive`, `check`'s
question asked of the control arm, `check --permissive` (defect 211). **Six
the same day**: `full`, `check` with no `--brief` over `tests/golden/full/`,
which pins a diagnostic's notes and excerpts (defect 289).

## What gates what, measured 2026-09-09

| touched | the suites that judge it |
|---|---|
| `selfhost/**` | `canonical` `layout` `order` `records`, **plus the compiler's own tests** |
| `selfhost/emit/**`, `selfhost/ir/**` (they move `tests/emission/**`, `tests/golden/emit/**` and `seed/heroes.c`) | **`emission`** **`emit`** `determinism` **`wholes`** `descriptors`, plus everything `selfhost/**` already gets |
| `selfhost/print/**`, `selfhost/lexer.hero`, `selfhost/parse/**` | **`probe`** `surface`, plus everything `selfhost/**` already gets, and the run by hand below before a push |
| `tests/golden/check/**` | **`check`** `annotations` `canonical` `fixes` |
| `tests/golden/fixedbugs/**` | `annotations` `canonical` **`emission`** |
| `tests/golden/unsupported/**` | **`unsupported`** `annotations` `canonical` |
| `tests/golden/permissive/**` | **`permissive`** `annotations` `canonical` |
| `tests/golden/full/**` | **`full`** `annotations` `canonical` |
| `tests/golden/run/**` | `canonical` `determinism` **`emission`** `lines` `run` `warnings` |
| `tests/golden/emit/**` | **`emit`** `canonical` `determinism` **`emission`** `warnings` |
| `tests/golden/ir/**` | **`ir`** `canonical` `determinism` **`emission`** |
| `tests/golden/surface-fixtures/**` | `annotations` `fixes` **`probe`** |
| `examples/**` | `canonical` `corpus` `emission` `warnings` |
| `spec/heroes-spec.md` | `spec` `special` **`grammar`** `unseen` |
| `selfhost/keywords.hero`, `selfhost/operators.hero`, `selfhost/grammar_expr.hero`'s `binary_op` | **`grammar`**, plus everything `selfhost/**` already gets |
| `docs/**`, `issues/**`, `CLAUDE.md`, `.claude/**` | `records` `unseen` |
| `.claude/hooks/**` | **the hooks' own tests**, `python3 -I -m unittest discover -s .claude/hooks -t .claude/hooks`, plus what `.claude/**` already gets |
| `tests/harness/**` | **the net's own tests**, `heroes test tests/harness/main.hero` |
| a file `site/src/lib/claims.ts` names at its top (`selfhost/cli/table.hero`, `selfhost/cli/doctor.hero`, `selfhost/parse/decl.hero`, `tests/harness/suite_spec.hero`, `.claude/agents/`, `.github/workflows/ci.yml`, and the rest it lists), or `site/**` | **the site's build**, `npm run build` in `site/`, before the push |

**`emit` and `unseen` were missing until 2026-10-06**, both found at batch 12's
close. `tests/golden/emit/` holds emissions kept by hand that an emitter change
moves: the batch's trial gate at `bead6e45` read `emit` 7 and 1 on
`ffi-lent-emits-nothing`, moved by defect 361's guard includes, which its lane
had not run because the `selfhost/emit/**` row named `emission` alone. And
`tests/harness/suite_unseen.hero` (defect 356, panel 192's R12) reads every
file `shell.project_files` names, so an invisible character written into a
document, an issue or the spec is its verdict and no other suite's.

**The site's build was missing from this map until 2026-09-28**, and it cost a
deploy. `heroes probe` became the command's thirteenth verb at M-agreed-retention
step 27, and the start page in both editions still said *twelve verbs* and named
eleven of them plus `this`; the whole net read 3,144 and 0, the push of that
milestone's close went out, and `site/src/lib/claims.ts`, which counts the
`Command(` rows of `selfhost/cli/table.hero` and asks the page to say the same,
refused the deploy. The page that was live stayed live, so nothing false was
published, and nothing true was either. The claims check is the site's
instrument and no suite of the net runs it, so a change to a file it reads goes
through the site's build before the push, as a `selfhost/print/` change goes
through the probe by hand.

**`grammar` was missing from the spec row until 2026-09-16**, found by panel
159's completeness critic while auditing the briefs that sitting was working
from. The row named `spec` and `special`, and `tests/harness/suite_grammar.hero:307`
reads `spec/heroes-spec.md` too — grepped, three suites name the path, not two.
It is load-bearing rather than tidy: that sitting's resolution amends **§ 5**,
the section holding the `Place` and `Param` productions `grammar` cross-checks
against what `heroes grammar` prints, so a session that obeyed this file to the
letter would have landed a § 5 sentence and run two of its three judges. This
file's own warning applies to it word for word — *a suite name that selects
nothing is a green run that tested nothing* — and so does the shape it keeps
finding: **the map is a premise about the world, and it expires in silence.**
The commands at the top of this file are the instrument; the table is only their
last answer.

**`emission` was missing from three rows until 2026-09-24, and so was it from
the command that makes them.** `suite_emission.hero` walks `"tests/golden/" +
group` over a constant list, `emit`, `fixedbugs`, `ir`, `run`, so the grep for
literal paths saw it read `examples` and nothing else, and the table said the
same. What it cost: the lane that closed defect 076 added two `run` goldens,
gated them by this row, and merged them with no blessed emission; the trunk's
`emission` read 486 and 2 until panel 176's compiler-engineer found it. The
coordinator trusted the table instead of running `emission`, which is this
file's first warning in so many words. The command now also prints the bare
group names of any suite that concatenates the golden root, and on that day it
printed the four groups above and changed no other row.

**`wholes` and `descriptors` were invisible to the command until 2026-09-28**,
and so was `tests/emission` under `emission` and `records`: the grep's
alternation named no path outside `tests/golden/`, `examples`, `selfhost`,
`spec/` and `docs/`, so the two suites that read `tests/emission` and
`seed/heroes.c` printed an empty row, which reads as *judges nothing*. Found by
panel 182's lane when it added `wholes`, the coordinator asking where the map
said an emitter change owed it. The command now names both paths, and on that
day, against its old self, it moved four rows and no other:

```
descriptors    "seed/heroes.c" "tests/emission"
emission       "examples" "tests/emission" "tests/golden/emit" "tests/golden/fixedbugs" "tests/golden/ir" "tests/golden/run"
records        ... "docs/work/milestones" "selfhost" "tests/emission"
wholes         "seed/heroes.c" "tests/emission"
```

**`wholes` refuses a write into a part of a temporary that nothing wrote whole
on that path** (panel 182's item 3): since that sitting the prologue gives a
temporary no initialiser, so the bytes such a write leaves are what the stack
held, inside a value the IR calls defined, and the sitting's critic found no
other instrument here that sees them (the verifier, clang, ASan). An emitter or
lowering change is what can introduce one, which is why that row owes it; it
reads files and runs no compiler.

**A NEW CHECKER RULE is not a `selfhost/**` change, and the row above says it
is.** Added 2026-09-14, at the M-marked-acquisition close, and it cost six red
checks in the full net over a milestone that had gated every sub-step. The
`selfhost/**` row names `canonical` `layout` `order` `records` and the
compiler's own tests, and **not one of those compiles a golden program**. So
when step 5 landed `check/acquiring.hero`, which REFUSES a shape that was legal
the day before, a golden case written at step 4 stopped compiling and three
suites went red in silence — `run`, `emission` and `determinism`, all three on
one file, none of them named by the row that sent the change through.

The rule, and it is about the KIND of change rather than the directory:

> **A change to what the checker REFUSES is judged by every golden tree, not by
> the `selfhost/**` row.** Widening a refusal can invalidate any program
> already in the repository, and the programs live in `tests/golden/**` and
> `examples/**`. Run `check` `run` `emission` `determinism` `corpus` as well.

Two things this is not. It is not *run the full net for every compiler change*,
which CL-063 refuses on cost; the trigger is narrow and nameable — a new
diagnostic, or an existing one that fires where it did not. And it is not a
claim that the map was wrong on 2026-09-09: it was right about which suite
reads which DIRECTORY, and this is the case where the directory is not what
decides. That is the same shape `.claude/rules/module-shape.md` names, one
level up: **the map rests on a premise about what a change can reach, and a
refusal reaches further than the file it is written in.**

**`emission` on `tests/golden/fixedbugs/` is the row that cost this file.**
That suite's own comment states its premise: *"those cases are wrong FFI
bindings, and the thing that refuses them is clang, at build time. `--emit-c`
never calls clang, so all fourteen emit — measured."* On 2026-09-09 four cases
entered that directory which the CHECKER refuses, so they emitted nothing and
the suite went red — correctly. **The repair was to move the cases, not to
loosen the suite**: a defect the checker refuses belongs in
`tests/golden/check/` under a `fixedbugs-` prefix, which is where the milestone
before had already put four of its own, and which `emission` excludes by
design. A premise written down is a premise that fails loudly; this one did its
job.

**Corrected 2026-10-05, defect 298**: the premise that comment stated,
*`--emit-c` never calls clang*, is false since 2026-09-16 (defects 048 and
049). `--emit-c` runs the pointee check and the build's rounds, which compile
every unit with clang and do not link, so a binding clang refuses exits 1 with
no C: 30 of the 38 `fixedbugs/` cases, measured 2026-10-05 by lane b11-misc,
and the suite passes such a program with nothing to bless (defect 341 is its
floor). The suite's comments say so since defect 298's repair; the paragraph
above stands as the history of 2026-09-09.

## The formatter's probe, by hand, before a push that touches `selfhost/print/`

Added 2026-09-28 with `heroes probe`, panel 179 item 7. The net holds two of
its forms: the fixtures under every family, one variant in two because the
whole run measured past the sitting's 60 s of user time, and the whole tree
under `multi` alone (`tests/harness/suite_probe.hero`). **What it cannot hold
is the form that found the defects**: a comment at every place one can stand
and every bracket broken after every token, over every file. So:

> **Before a push that touches `selfhost/print/`, run `heroes probe <root>
> --family single` and `--family bracket` over `selfhost`, `tests` and
> `examples`, and `heroes probe` over the fixtures with no stride, and push
> only over exit 0**, or over exit 2 on the failures `suite_probe`'s rows
> name as an open defect's. A failure is a finding filed as a `defect` issue
> or a repair in the same lane, never a reason to skip the run.

Its cost, measured 2026-09-28 on one module and carried to the tree by
arithmetic, so an inference: 25 µs per line of the seed for each variant with
a compiler built at `-O2`, 8.6 variants per line in the two families, and
73.3 million for the sum of the squared line counts of the 1,020 seeds, about
4.4 CPU hours at `-O2` and five times that with the gate's plain build. It is
the reason the run is by hand: build the compiler with `clang -O2` for it and
run the six invocations side by side.

## What may run beside a gate, and what may not

**Free: a pass-or-fail gate.** While a suite is deciding green or red, other
work may proceed — a subagent, a panel's judges, a second worktree — because
nothing another process does changes whether an assertion holds.

**Measured 2026-09-29, and now a rule of the batch gate**: the 26 suites run
six at a time in separate harness processes (`xargs -P 6`, each with its own
`build/harness-<pid>`) agree with the sequential run on every one of the 26
count lines, exit 0 each, 729 s of wall beside two other lanes' work; the
sequential run the same day read `real 13997.37` against `user 1092.31` on a
machine at load 30 and is discarded as a duration (the ratio). So a batch gate
runs its suites in parallel, and its duration is not a number anyone writes
down while it does.

**Two things the first batch gate under this rule found, the same evening.**
`cache` went red in the parallel pass, 4 and 2, *nothing was edited and these
objects were rebuilt: build/tu-.../library.o*: that suite asks the filesystem
which object is newer after a build that touched nothing, and in the shared
`build/` another suite's build had rewritten the library's object, one slot
per compiler, at that moment. It read 6 and 0 alone, before and after. So
**`cache` runs alone, after the parallel pass**, being the one suite whose
question is about the cache rather than about a program; and **a suite red in
the parallel pass is re-run alone before its red is read as a verdict**. A
false red is what a shared cache can produce; a false green it cannot, since
an object read half-written fails to compile or to link, and every other suite
read the same counts in parallel as alone, twice (26 of 26 on 2026-09-29).

**Forbidden: anything while a clock runs.** CL-025 is unchanged and it is the
half that bites: time before and after with `/usr/bin/time -p`, and while a
clock runs the machine stays still. Paid for again on 2026-09-08, when a run
read `real 1870.49` against `user 65.37` — the ratio says it was waiting, so
the number was discarded and the honest re-run read 67 s. **The rule is
therefore: parallel work is free on correctness and forbidden on duration.**

**Forbidden: editing what the running suite reads.** A suite reading the tree
owns the tree until it exits (CL-025). `records` reads `CLAUDE.md`, `.claude/**`
and all of `docs/`, so amending the contract while the net runs is the panel
056 story with the coordinator in the judge's chair. Two routes out, and both
are better than waiting idle:

- **draft in the scratchpad** and apply when the suite exits — no conflict is
  possible, because nothing in the tree moves;
- **detach a git worktree** and work there. A worktree is a separate checkout
  with its own index, which is why it is safer than a second session in this
  one: CL-041 and CL-070 are both about a shared index carrying away another
  session's work, and a worktree has no such index to share.

That second route became a way of working on 2026-09-12, and its rules are
`.claude/rules/records.md` § Working in lanes: one milestone, one file, one
worktree. What made it possible is that the records stopped being monoliths —
a lane now writes files nobody else is writing — and what still binds is this
section's own line: parallel work is free on correctness and forbidden on
duration.

## The compiler that judges is a build artifact, and it can be older than the tree

Added 2026-09-18, at M-declared-extents step 1, and it cost two full-net runs —
**thirty-one minutes for one bit of information**.

`heroes` is `.gitignore:15`. It is therefore the one input to every gate that
`git status` cannot report, and a session that reads a clean tree has been told
nothing about it. On that day `./heroes` was built at 02:08 and HEAD was 18:17,
so the net ran **1862 passed, 23 failed** against a tree whose own ROADMAP said
1891 and 0 — and the tree was right. Rebuilding from the seed took **2.97
seconds** and the same net read 1891 and 0.

**The rule: build the compiler before the gate that judges with it, not after
the gate goes red.** `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
is three seconds against the net's fifteen minutes, so there is no trade to
weigh; and a suite selected by name is judged by the same stale binary just as
silently as the whole net is.

**What makes it hard to see is that the failure does not name itself.** Not one
of the 23 lines said *your compiler is old*. The two `spec` failures said
`STALE: the recorded count is for 6bdb9b497a141864 and this file is
3c065c560426eb07` — the OLD binary carrying the OLD pin and correctly concluding
that the DOCUMENT had moved. A golden failed with `error[unknown_function]: no
function named validated_bytes`, which reads as a missing built-in rather than as
a compiler that predates it. Both diagnostics were true statements by the
program that made them, and both pointed away from the cause. That is CL-078's
shape — *a defect is written down as the finder saw it, and the shapes beside it
are where what it actually is becomes visible* — arriving in the harness rather
than in the language.

## And the order that makes this worth doing

CLAUDE.md § Verification already asks for it and CL-063 is its case: **the named
suites gate a sub-step, one at a time, and the full net runs once before a
push.** The value of the map above is that "the named suites" stops being a
judgement call. Read the map, run those, and keep the thirteen-minute net for
the push — where its cost buys something, because it is the last thing between
the work and a public branch.

**Superseded on 2026-09-29 in its first half**: a repair or a step is gated by
the golden form that holds its cases and by the compiler's own tests, and
the full net runs once per BATCH, sixteen to sixty-four defects since
2026-10-05 (sixteen to thirty-two from 2026-10-03), which is what a push carries
(§ The batch, above; CL-079). The map keeps its other job: it says which form a
change's cases live in, and which suites the batch's census and platforms owe.

## The batch: repairs gated by their cases, one gate for sixteen to sixty-four

**Amended 2026-10-03 by author instruction**, meant as: *here you have to
make batches holding at least about fifteen defects together, because
otherwise it gets too long.* Given on the coordinator's plan for panel 188's
landing, a lane of five and a lane of two; measured that day by the commits'
dates, seven round gates in 27 hours (`e2d59fdb`, 2026-10-02 13:36, to
`da0ff6c9`, 2026-10-03 16:17), the seventh over one defect. Minutes later,
meant as: *make batches of at most twenty defects*; and then, meant as: *you
choose the range, but dare a little: ten to twenty, sixteen to thirty-two.*
The coordinator chose **sixteen to thirty-two**: the open list of that
evening, 47 once its filings were in, clears in two batches at that size and
in three at ten to twenty, each batch paying one seed, one full net, one
census, two platform legs and one push; the price is a red gate's bisect of
five steps instead of four. So **a batch holds sixteen to thirty-two
defects**, in as many lanes as its clusters need (one lane per cluster of
shared files, parallel lanes for disjoint ones), merged into one round and
gated once. **It is filled from the open list**:
`blocking` first, then the `adjacent` and `improvement` items that share a
cluster's files, then the oldest `adjacent`, then `improvement`; so no
`blocking` item waits for others to be found, and when fewer than sixteen are
open the batch takes them all.

**Amended 2026-10-05 by author instruction**, meant as: *seeing how well this
process is going with many defects in a batch, I would take it to a maximum
of sixty-four defects, just because 64 is a nice number in computing.* Given
on batch 12, opened that evening at thirty-two and grown while it ran, and on
the same evening's aim, meant as: *close this whole queue tonight, only the
improvements staying out*. So **a batch holds sixteen to sixty-four defects**,
filled in the order above; the price grows by one step, a red gate's bisect
of six steps at most.

**A batch's own questions take the recommended answer** (the author's *5a*
of 2026-10-04, meant as: *yes, a standing default for your recommended
answers in batches*;
`docs/records/log/2026-10-04-1236-the-author-answers-1a-2a-3a-4a-5a-6b.md`).
A question a batch raises, which the coordinator would otherwise put to the
author, is decided by the coordinator with the resolution it recommends. It
rests on a measurement as any recommendation does (CLAUDE.md § 3), and the
batch's closing commit says once which way each went, so the author can
turn any of them. **Four kinds stay the author's every time**: a push or any
outward-facing act, a panel's ratification, a paid run, and a destructive
operation (CLAUDE.md § Hard stops). It is CLAUDE.md § 3's default for a
milestone asked for in one `/step` (CL-002), applied to the unit the work
now moves in.

**Amended 2026-10-02 by author instruction**, meant as: *keep only the Mac
as the development environment, and the other platforms only as activities
before the final push, Linux included, above all the emulated one; Linux arm64
in Docker rather than the emulated Linux.* Given on the breakdown of lane
recovery-b6's gate that morning, its files' times: 107 minutes, of which the
seed and fixpoint 2, the full net on this Mac 14, the census 2 and **the
emulated Linux x86-64 container 83**; and on a catch rate that container had
not earned (it passed defect 140's variants case at every gate of 2026-10-01
while the CI's x86-64 leg timed out on it, clang 22.1.8 against 18.1.3). So:

- **one gate per ROUND**, not per lane: the round's lanes, together a batch of
  sixteen to sixty-four defects (above), are merged into one tree under
  `.claude/worktrees/`, one merge
  commit each, and that tree is gated once by steps 1 to 4 below; red, the
  lane is found first by the red suite at each merge commit, then the commit
  inside it by bisect;
- **step 5 leaves the gate**: no container at a gate. Before a push, Linux
  arm64 in its Docker container (native on this Mac, 24 and 27 minutes on
  2026-10-02) and the Windows box; Linux x86-64 is the CI's leg after the
  push;
- **a defect at the C boundary** (`runtime/`, `seed/`, `selfhost/emit/ffi*`
  and `extern*`, an `examples/` program with an `extern`) closes only after
  the push's platform legs have run its cases, a case on a header one
  platform lacks judged where the header is (`.claude/rules/platforms.md`,
  the author's *A* of 2026-10-03); every other defect closes at the round's
  gate.

The first round under it, 2026-10-02: four lanes, one gate, `e2d59fdb`, the
full net 4,449 passed and 0 failed. The text below is the rule as it stood
before, kept.

**And per repair, its own cases alone** (author instruction 2026-10-02,
between 16:23 and 16:31 by the clock read before it and the commit after
it, meant as: *the lanes are slow; per repair they must run only
mini tests, and every other check only at the end*). Given on what one
repair of lane land186 ran that afternoon: `check` whole (400 cases),
`unsupported` whole (73), `warnings` whole (289 programs built), the
compiler's 1,023 tests and its own cases; and the lanes' briefs had asked a
census of about 1,700 files after each repair. So a repair is gated by its
compiler rebuilt and its own cases, every form narrowed to them with the
harness's third word, plus the seconds-long checks of the touched file (fmt,
`layout` filtered to it); **the whole forms, the compiler's own tests, the
recovery instrument and the census run once, at the round's gate.** The
per-repair bullet below that says *the golden form ..., whole where the cost
column says it costs under a minute* and *the compiler's own tests* is
superseded by this paragraph; the risk it takes, a repair's damage to
another's cases found only at the gate, is the risk CL-079 already took, and
the gate's bisect by lane and commit is its answer.

Author instruction 2026-09-29, in these words (meant as): *it is unsustainable
to go at the speed we are going; a repair gets its own test, on one platform,
and is queued; the next repair the same; only at the end of a batch of four or
five are they all tested together, on one platform first and on the others
after; there must be one point, and only one, where everything is tested in the
net; two or three repairs tested together can contaminate each other, and that
is a risk we take.* What it cost to learn is CL-079, and this section is the
rule's only home. CL-063 already said the named suites gate a sub-step and the
full net runs once before a push; the practice had eaten the rule. Counted on
the commit bodies of the night of 2026-09-28 to 29, nine and a half hours: 31
commits, 11 merges each with a coordinator's gate, 10 repair commits, 8 gates of
10 to 21 suites **every one reporting 0 failed**, 0 defects closed.

**Per repair or step**, in the lane that holds its batch, in sequence:

- the golden case per shape, `fixedbugs-NNN-<slug>` under `tests/golden/check/` or
  `tests/golden/run/`, annotated in its source;
- **the golden form that holds those cases**, whole where the cost column below
  says it costs under a minute (`check`), filtered to the defect's cases where
  it does not (`run`, `fixes`): `heroes run tests/harness/main.hero --
  <compiler> <form> [<case-substring>]`. The third argument is the harness's
  own, and zero matches exit 2 (a run that selected nothing is a green run that
  tested nothing, this file's first warning). The whole form costs what the
  filtered one does, and it sees at once a contamination of the batch's earlier
  repairs, whose cases live in the same form. CL-074 binds the invocation: the
  gate's own command, never a `diff` by hand;
- the compiler's own tests, `heroes test selfhost/main.hero`: 880 tests, 69 s of
  CPU warm on 2026-09-29 beside another net, 35 s on a still machine (CL-025).
  Not `heroes test <module>`: a nested module resolves its `use` lines from its
  own directory and cannot be tested or checked alone, measured 2026-09-29 on
  `selfhost/measure/pinned.hero`, and 251 of the compiler's 308 modules are
  nested;
- **no seed, no fixpoint, no whole suite, no census, no platform.** The seed is
  regenerated once, at the batch's close, and travels in the closing commit:
  measured over 2026-09-15 to 29, 93 commits touched `seed/heroes.c`, 22.9
  million lines of churn, about 21 whole-file rewrites, and **21 of 58 merges
  carried a seed conflict**, every one settled by rebuilding. A repair commit's
  body says *seed not regenerated, awaiting the batch gate*, in about fifteen
  lines: the class, the cases, the gate line. Its issue file stays `- [ ]` and
  gains one dated body line, *Repaired at `<hash>`, gated by its cases and the
  compiler's own tests; the net is owed at the batch's close*, and its card's
  `commit` takes that hash whole, so that no sentence says closed before the
  net has said it.

**Before the first repair of a cluster, attack the shapes beside all of its
defects in one pass** (CL-061, CL-078): on the night counted above the shapes
were sought after each repair and reopened a round each time, six filings and
widenings for ten repairs. A widening found beside a defect becomes the next
item of the same lane, never a filing on the trunk. **Amended 2026-10-02 by
§ Bounded discovery below**: only a shape with the repair's own cause stays in
the item; every other real defect found beside it is filed apart, `adjacent`.

## Bounded discovery: the classes of a defect, and when a round ends

Author instruction 2026-10-02, between 16:31 and 16:39 by the commits
before and after it, meant as: *D1a D2a D3a*, on a
proposal put with its measurements. **The loop it ends**: the open count was
3 on 2026-09-29 and 10 from 15:55 on 2026-10-02 (defect 158's filing), 65 defects closed in nine
days while the numbers issued went from 082 to 158; defect 130, itself found
beside defect 124 on 2026-09-28, took seven batches (recovery-b1 to b6 and
b8) and its item grew to 264 lines, every batch's first pass adding shapes to
the same defect. Two rules that are each right multiplied: the shapes beside
a repair are attacked (CL-061, CL-078), and every one of them became the
same lane's item and blocked the tag.

**Every defect carries one class**, its file's item ending its
line ` · **class: <name>**`, and its body one line opening
`**Class: <name>**, YYYY-MM-DD` with the reason after the day
(`.claude/rules/records.md` § The lists):

- **`blocking`**: the current work's acceptance fails, or, whatever the
  work, robustness or truth does: a wrong value, a crash, a memory fault, an
  exit 2 where the author can be told, a false message, a `certain` fix that
  writes a program meaning something else, a correct program refused, a
  wrong one accepted, a clang warning on a correct program, a red CI. **This
  class is never deferred** (§ Precedence: robustness).
- **`adjacent`**: real, found beside the work, none of the above (a second
  message for one mistake, a mistake told only after the first is fixed, a
  true message less exact than it could be). Filed with its reproducer and
  never widening the item it was found beside. **It becomes `blocking` when
  two milestone tags have been placed since it was filed**, so the queue
  cannot only grow.
- **`systemic`**: a defect that has taken **three batches** and is still
  open, or one whose remaining rows need a ruling no rule reaches; it stops
  the lanes on it and goes to a sitting (CLAUDE.md § 4) or to the author.
- **`improvement`**: hardening, coverage or a cleaner form nobody needs to
  be right; the backlog.

**The bounds**, one place: the shapes attacked are those beside the
item's reproducer, depth one, never those beside the shapes; a batch repairs
at most two `adjacent` items found inline, in its own files; everything else
found is filed, classed, and left.

**When a round ends**: when its items are repaired and its gate is green,
not when nothing more can be found in the files it touched. **A milestone is
tagged** over zero `blocking` and zero `systemic` items (CLAUDE.md §
Verification), and the author's goal *0 defects* reads as that (the author's
*D3a*). The executor is `records/lists` (one class per item) and
`records/tagged` (only `blocking` and `systemic` count, and an aged
`adjacent` counts as `blocking`).

**A recovery item closes** (panel 187's R1, ratified 2026-10-03, the author's
answer *(a)*) when each of its rows is repaired, filed `adjacent` apart one
item per cause, or pinned as a known cost in a `tests/golden/check/` case whose
header gives its reason and the ruling it rests on; never a row that breaks
design.md §4.17's promise (a false message, a `certain` fix that writes
another meaning, an exit 2), which is `blocking`. A pinned row is not an open
item and does not age, and its golden moving, either way, is read at the gate.
The counts §4.17 names are read at each round whose lanes touch the
recovery's files, the round's compiler against the trunk's over one frozen
corpus of planted mistakes, every moved mutant named in the round's closing
commit and a worse one filed by its class (panel 187's R2); never a ratchet
on totals, and a corpus program's own messages are subtracted before a
mutant is read as worse (the fourth round's reading, 2026-10-03).

**Per batch**, which closes at the first of: its sixteen to sixty-four items
repaired (every open one, when fewer than sixteen are open), before any push,
or when the author asks; never more than sixty-four. A batch
never spans a tag. The gate is what a push already owed, on the tree that will
BE the trunk (merge the trunk into the lane once, gate the merged tree, then
fast-forward the trunk: same tree, no second gate):

1. the seed regenerated, the compiler built from it, the fixpoint by `cmp`;
2. the compiler's own tests, the net's own tests, **the full net**, its counts
   in the closing commit's body; the suites may run six at a time in separate
   processes, `cache` alone after them, and a red suite is re-run alone before
   it is read (§ What may run beside a gate, below); nothing is timed while
   they do;
3. the census of `check` over the tracked `.hero` files, trunk against batch,
   where the batch changed what the compiler refuses (the rule *a new checker
   rule is judged by every golden tree*, done with one instrument), run with
   `xargs -P 8`;
4. the formatter's probe by hand if `selfhost/print/` moved, the site's build if
   a file `claims.ts` reads moved (both above);
5. then the Linux x86-64 container (`.claude/rules/platforms.md`).

**Red**: the batch's commits are linear in its lane, one per defect. `git bisect`
between the batch's base and head, at each step the compiler rebuilt from
`selfhost/` with the base's compiler (`heroes build selfhost/main.hero`, about
30 s warm) and the one red suite; sixteen to sixty-four repairs are four to
six steps. The culprit
alone is redone, with the other repairs' cases in view; where the culprit is
the interaction of two repairs, the later one is redone and the record says so.
**Green**: each repaired defect's issue is ticked where it stands and given its
*The repair* section, nothing moving (`.claude/rules/records.md` § The issues,
since 2026-10-04; it was moved by `git mv` into `docs/records/done/` that day
only), the ROADMAP's count moves with the ticks, and the closing commit carries
the counts.

**CL-054 is discharged at the batch, not per repair**: *run the suite you did
not expect to move* is what the full net is, and a repair defers it knowingly,
in writing. **CL-063's half about the sub-step is superseded by this section**;
its half about the full net before a push stands, and the push is a batch
closed.

## A suite is the last judge, never the first finder

Author instruction 2026-09-29: *prevent the trivial mistakes before any suite
runs; a file over the ceiling, a file that does not parse: check the touched
files and only those, so that the suite never finds what something simpler
could.* Measured on the trunk's compiler the same day, `real` equal to `user`:
`heroes fmt` on a module **0.02 s**, `heroes check selfhost/open_line.hero`
**0.18 s**, `heroes check selfhost/grammar_expr.hero`, the largest module,
**0.48 s**. Five layers, each with an executor, cheapest first:

- **Layer 0, at the moment of writing** (`.claude/hooks/fmt_check.py`, on the
  touched file only): a `.hero` that does not parse puts the compiler's message
  in front (exit 2) instead of waiting for a build; a `tests/harness/` module
  goes through `heroes check <file>`, and a `selfhost/` module through
  `heroes check selfhost/main.hero`, the whole compiler's names and types in
  **5.8 s** (measured 2026-09-29), because a nested module cannot be checked
  alone and a root module checked alone does not see its callers; a `selfhost/` module's
  line ceiling is judged in two stages, the file's non-blank lines first (a
  count that is always at least the instrument's, so under 300 it is silent)
  and the harness's own `layout` filtered to the file when that count passes
  300 or the file is in `DECIDED`; a `tests/golden/` case's `#~` annotations
  are held to its `.expected`. The hook notices and never rewrites (CL-025).
  **The last check was named here from 2026-09-29 and performed by no hook
  until 2026-10-05** (defect 286): since then the hook asks the `annotations`
  suite itself, narrowed to the case, on a write of its `.hero` or its
  `.expected`, and a golden case whose `#~` marks claim diagnostics is no
  longer told it does not parse (defect 272). **Since 2026-10-07 the file is
  judged in the tree it stands in** (defect 254, `.claude/hooks/trees.py`): the
  nearest directory above it holding `seed/heroes.c` and a `.git` entry, so a
  lane's file is judged by the lane's compiler with its paths read from the
  lane's root, whatever the session's directory; a file outside every tree is
  asked only whether it parses and is canonical, by the session's tree's
  compiler; and a tree with no compiler built is told so, its file not judged.
  **And the age of that compiler is asked before a refusal of it is read**
  (defect 384): a compiler built before a source of its tree was written, the
  file written aside, has its refusal told as its age, with the rebuild and
  its words beneath, never as *does not parse*; an older compiler that accepts
  the file says nothing.
- **Layer 1, before a commit** (`.claude/hooks/guard_bash.py`): a harness run
  whose named compiler is older than `seed/heroes.c` or the newest file under
  `selfhost/` or `runtime/` is refused (2026-09-18 cost 31 minutes and two nets
  to this, above), those of the tree the run stands in, found from the
  command's own `cd` since 2026-10-07 (defect 348: a lane's run from a session
  on the trunk was judged against the trunk); a `git commit` on the same command line as a gate is refused
  (two commits went past a red `records` in one week, `f22c8baf`, `efaf5564`,
  because the chain read the pipe's last exit); a gate piped into `head` or
  `tail` is refused (seven `emit` goldens stayed red for two steps behind an
  output cut at forty lines); and a `git commit` whose index holds a `.hero`
  file that does not parse, is not canonical or is over its ceiling is
  refused, the index being the one of the tree the commit runs in, `git -C` or
  the command's `cd`, since 2026-10-07 (defect 287: a lane's commit was judged
  by the trunk's index), and a refusal by a compiler older than that tree told
  as its age; a golden case whose `#~` marks claim diagnostics is judged by
  its marks, not by `fmt`, every such case of a commit asked of `annotations`
  in one run bounded at 45 s, which gives no opinion past it (defect 334,
  `.claude/hooks/marks.py`, which the write-time hook asks through too). The
  hard stops' commit rule is read where it was blind until
  2026-10-07 (defect 403): a `--` followed by no path, by expansions alone or
  by the whole tree is refused as a commit with no `--` is; a merge, a
  cherry-pick or a revert concluded with the whole index, `--continue` or a
  bare commit while it stands, is refused when the index holds a file it did
  not bring; and a command behind an assignment, a wrapper (`env`, `time`,
  `caffeinate`, `timeout`) or a shell's `-c` is read as the shell runs it.
  And a commit's, a merge's or a tag's message, `-m` or `-F` and the heredoc
  `-F -` reads, is refused where it holds a character the `unseen` suite
  refuses in a document, named by its code point, line and column, the list
  being that suite's `REFUSED`, read from it (defect 378).
- **Layer 2, per repair**: the form that holds its cases and the compiler's own
  tests.
- **Layer 3, per batch**: the seed, the fixpoint, the full net, the census.
- **Layer 4, before a push**: Linux arm64, Windows, the probe, the site's build.

What the record says the suites found, 2026-09-04 to 29, every case: a file
written and not formatted (twice), a blank line in the harness, a `DECIDED`
table at 17 against an assert at 16, an item moved and a sitting no longer
cited, a stale binary, seven `emit` goldens behind a cut output, `warnings` 204
and 1 written as 204 and 0, two commits past a red `records`. Every one is a
layer 0 or layer 1 catch. Of the **59 defects** closed between 2026-09-15 and
29, **a suite found two** (074 by CI's Windows leg, 115 by `run` on the Windows
box) and 57 were found by a person, a seat or a probe.

## The map's cost column

The rule above says *under a minute* and *filtered otherwise*, so the cost of a
suite is a measurement and not a memory. The command, machine still (CL-025),
compiler warm, and its answer beneath with the date:

```sh
for s in $(grep -oE 'only == "[a-z_]+"' tests/harness/main.hero | grep -oE '[a-z_]+"' | tr -d '"'; echo check ir emit unsupported); do
  printf "%-14s" "$s"; /usr/bin/time -p ./heroes run tests/harness/main.hero -- ./heroes "$s" 2>&1 >/dev/null | grep -E '^(real|user)' | tr '\n' ' '; echo
done
```

**2026-09-29: the column is owed, unrun, at the first quiet hour**; two other
sessions' gates and builds ran on this machine all day, and a number taken
beside them is discarded by the ratio. What stands: the whole net read
`real 948.31` on 2026-09-18
(`docs/records/log/2026-09-18-2327-the-number-had-a-platform-in-it.md`), `run` 65 s
on 2026-09-28 (journal 061), and three rule files gave it three durations, 13,
15 and 20 minutes, which is CL-064 on the number that decides how a session is
planned. This section is that number's one home from today.

## A long run holds the machine awake

Measured 2026-09-29 with `pmset -g custom`: this Mac sleeps after **one minute**
idle, on battery and on power alike, and a third-party assertion (Amphetamine)
is what has kept it awake. Journal 061 records the Mac asleep from 10:27 to
13:57 under panel 177 and every lane stalled under a closed lid, and two
wakeups that did not fire. So a gate, a sitting or a loop that expects to run
longer than a minute holds its own assertion: `caffeinate -i <command>`, or
`caffeinate -i -w <pid>` beside a run already started. It costs nothing and
depends on no application.

## A run that may not end is bounded in time and in bytes

Measured 2026-10-06 to 07. Panel 196's completeness critic probed libuv with
a program that prints its loop's handles without end, under `timeout 60
heroes run`. The timeout ended `heroes`, and the program `heroes run` had
started kept running, its parent now `launchd`: the runtime gives a child its
own process group (`runtime/parts/run.c:810`, so the terminal's Ctrl-C reaches
the program in front), and `heroes` forwards no signal when it is itself
killed (defect 425). It wrote **146 GB** into one file of the scratchpad (the
process had run 44 minutes when `ps` read it), the data volume filled at about
23:47 by the lanes' reports, every lane, seat and shell stopped on `ENOSPC`
(even `df`, the harness writing its own output first), and the author found
it with `ps`, killed it (the file's last write 00:04) and removed it before
00:09. The coordinator's
brief had asked for the probe and bounded neither its output nor how it is
stopped. Reproduced at 00:09 with a program of four lines: `timeout 3 heroes
run` exits 124 and the program is still running, its parent 1.

So, until defect 425 is repaired and after:

- **a program that may not end** (a probe of an endless loop, a server, a
  reader of a pipe nobody closes) is built with `heroes build` and its BINARY
  run under `timeout`, never `timeout heroes run`;
- **its output goes to `/dev/null`, or through `head -c <bytes>` into a
  file**, never into a file nothing bounds;
- **a brief that asks a seat or a lane for such a probe names both bounds**, the
  time and the bytes, as it names a paid run's budget;
- **after a timeout the built binary is looked for** (`pgrep -f build/`) and
  stopped, before the run's verdict is written down.
