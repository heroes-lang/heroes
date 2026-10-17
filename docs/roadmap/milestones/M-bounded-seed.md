# M-bounded-seed — the seed bounded in shape and in rhythm

**Opened 2026-10-14, at row 67, by the author's instruction**, the first row of
the chain re-ordered that day, consolidation first
(`docs/roadmap/scheduling.md` says why each row moved). No warrant is claimed;
what it buys is measured below and in
`docs/measurements/045-the-seed-is-seventy-percent-of-the-history-and-a-rewrite-that-keeps-it-at-the-releases-clones-in-seventy-one-mebibytes.md`.

**What was measured the day it opened.** The seed is 70% of the repository's
history: 1,222 versions of `seed/heroes.c`, 478 of the 677 MiB of blobs in the
pack, and a fresh clone of 580 MiB where the tree alone compresses to about 36.
It is regenerated at every batch's close and on every lane that touches
`selfhost/`: 62 regenerations in the two weeks to 2026-10-14, and 39 of the
356 merges of the four weeks before carried a seed settled by rebuilding. At
`612e686d8` it is 51,688,146 bytes and 1,400,548 lines, and 48% of its bytes
are `#line` directives naming a `.hero` file (`selfhost/emit/c_text.hero`'s
`line_directive`); between `v0.3.0` and `v0.4.0` 62% of the lines that changed
were `#line` lines. Nobody reads a seed diff, and the commit that brought the
seed in said so (*152 commits is 152 seed reviews*, `d86aa5d8c`). Clang took
17.98 s on the seed of `5876e9153` and 7.86 s on the seed of `612e686d8`
(`/usr/bin/time -p`, `real` against `user` 17.20 and 7.60, so neither run was
waiting), against the 3.4 s `seed/README.md` measured in August.

**What it delivers, in three parts.**

1. **The shape.** The seed is the one emission nobody steps through in a
   debugger, so its `#line` directives buy nothing and cost half of it. An
   emission profile writes the seed without them (or with the line number
   alone, which C11 § 6.10.4 allows), and, if a measurement says it pays, a
   bootstrap root without `mutate`, `probe` and `measure`. It is a sitting:
   CLAUDE.md § 10's rule refuses `--no-line` as a flag
   (`.claude/rules/cli-surface.md` § What this rule refuses today), and the
   profile is a question about the C the compiler writes (design.md §3.1;
   panels 020 and 200). The target is a number: a seed under 10 MB.
2. **The rhythm.** The compiler is always built in two stages, clang on the
   seed and then `heroes build selfhost/main.hero` with what clang made, which
   every lane already does; the fixpoint is checked one generation further.
   The seed is refreshed only when that two-stage build goes red, and at each
   release, whose `VERSION` it carries. A new form enters `selfhost/` only
   after the refresh whose seed knows it. The rules this moves change in the
   same commit as the policy: `.claude/rules/verification.md` § The batch,
   `.github/workflows/ci.yml`, the hook that judges a compiler older than the
   seed, `docs/roadmap/verify.md` and `seed/README.md`.
3. **The history, rewritten once.** The seed kept only at the four releases
   and at the head; every older commit kept under `refs/archive/*`, so every
   hash the record cites still opens; the 69 tags re-pointed under their own
   names; the hashes the tree pins remapped through `git filter-repo`'s
   commit map. Rehearsed on local copies the day this row opened (measurement
   045): a fresh clone from 580 MiB to 71 MiB, the seeds at the kept commits
   byte-identical, one cited commit of 525 not remappable (`2067ec875`, which
   GitHub already does not hold), and the records suite at 25 and 2 on the
   rewritten copy, then 26 and 0 once the 56 hashes pinned in 27 files were
   remapped. **Its conditions**: proved first on a throwaway repository that
   GitHub serves `refs/archive/*`, an old `Record:` permalink and a release on
   a re-pointed tag; every commit-map pair differing in `seed/heroes.c` alone;
   a sample of intermediate commits built in two stages, so that `git bisect`
   stays usable; `--prune-empty never`; announced in the README and in its
   decision issue, as git's own documentation asks of anyone who re-tags
   (`git help tag`, § On Re-tagging); and the promise that `main` and the
   published tags are never rewritten again written into
   M-compatibility-promise the same day. The throwaway repository, the
   force-push and the reset of the shared checkout are each asked for.

**Why now.** On 2026-10-14 the repository had 1 star, 0 forks, 0 watchers and
11 unique visitors in fourteen days, and no install channel pins a release
tarball's checksum yet; once one does, a moved tag breaks every install. And
the thesis harness writes the compiler's hash into every run's provenance, so
the hashes should stop moving before its first run.

**Considered and refused, each with its reason.** A repository for the seed
alone: it moves the growth rather than removing it, and a pin between two
repositories is a second place where the truth lives, while
`git clone --filter=blob:none` already gives a stranger what it would.
A new repository for the rewritten history: the 2,068 issues would take new
numbers, and a defect's number is its name for ever (CL-083). A seed
downloaded at bootstrap, the shape Rust's `src/stage0` and Go's
`GOROOT_BOOTSTRAP` take: `seed/README.md` refuses it already, *the shape of
CVE-2024-3094*. A seed at every milestone tag: 35 milestones closed in
September, so the history would grow by tens of MiB a month again.

**Named for the deliverable**, the seed bounded. Not `M-smaller-seed`, a
direction a later emitter may reverse; not `M-release-seed`, half the policy;
not `M-light-clone`, the effect on a stranger rather than what the tree gains.

**Corrected 2026-10-14, the same evening, by the run on the tree to rewrite**
(measurement 045 § 7). Three sentences above are wrong and stand as they were
written. *The seed is 70% of the repository's history: 1,222 versions*: the
1,222 counted every line its command printed, commits, tags and trees beside
the blobs; the seed's versions were 294 at `5876e9153` and are 301 at
`c4532d741`, and they are 87% of the pack a clone receives, where the 70% read
the local repository's own pack. *A fresh clone of 580 MiB* was a clone of the
local repository; a clone from GitHub was 616.61 MiB. And the condition *a
sample of intermediate commits built in two stages, so that `git bisect` stays
usable* is not met by keeping the seed at the releases alone: 2,279 of the
3,619 commits would carry a seed whose runtime interface their own tree no
longer has, which clang refuses at its first lines, 37 of the 66 tagged commits
would carry none, and `seed/README.md`'s recovery chain (*every milestone tag
from `m-selfhost-fixpoint` on carries its own `seed/heroes.c`*, § If the seed
is already broken) would hold at five tags of 69. No choice of kept seeds keeps
every commit buildable from a default clone, because the language moves under
an older compiler as well as the runtime does: keeping the seed wherever the
runtime's interface changed, six of the 32 tags measured that carry an older
seed and two of ten commits drawn at random still stop at the second stage,
and each of the eight builds as it was. The commits as they were, under
`refs/archive/`, keep every one. Which seeds the rewrite keeps goes back to the
author, with four variants measured.

**Decided 2026-10-15, by the author: the rewrite keeps the seed at every tag**
(variant A of measurement 045 § 7), the coordinator's recommendation and its
reason put beside the four variants measured. Every tagged commit that carries
a seed keeps its own, so every published tag stays the tree it was, byte for
byte, and builds as it did, and `seed/README.md`'s recovery chain, *every
milestone tag from `m-selfhost-fixpoint` on carries its own `seed/heroes.c`*,
stays true of the past. The rehearsal's pack weighs 124.88 MiB against B's
72.95; a clone from GitHub is weighed after the push. **One rule for the past
and the future, as the author asked on 2026-10-14**: every tag builds from its
own tree. In the past it does by the seed it carries; from part 2's rhythm on
(issue 2087) it does by the two-stage build the CI checks at every push, and
that issue owes the recovery chain's sentence its new shape when it lands. A
commit between two tags is built or bisected under `refs/archive/`, which keeps
every commit with the seed it carried: part 3's condition on bisect is
answered so, and its other conditions stand. **And a copy of the repository as
it was, kept aside** (the author's instruction of 2026-10-15, meant as: *if you
have a copy of the old repository somewhere, keep it aside, to compare that
`main` has the same latest files*): a bundle of every ref and a mirror of
GitHub, under `.claude/worktrees/backup-before-rewrite/`, made again just
before the force-push; `main`'s files are compared with it after the rewrite
and after the push, the remapped hashes being the one difference allowed.

**Decided 2026-10-17, by the author: the target is amended** (panel 219, ratified that morning,
https://github.com/heroes-lang/heroes/issues/2118, the coordinator's recommendation and its
reason put beside the other answer). Part 1's *a seed under 10 MB* is reached by no emission
profile: the sitting measured it as a floor, every copy, inline bounds check, `#line`,
indentation and dead function removed leaving 18,831,319 bytes, identifier text being 69.7% of
what remains. The shape this milestone delivers is panel 219's s4, the compiler's own emission
with a re-anchoring `#line` joined to its anchor and a file name written where it changes, for
every program: the seed about 25.6 MB, its line table unchanged at every address, and its
history 60% of the seed as written's per regeneration; part 2's rhythm bounds how often that is
paid. What a smaller seed would need changes every program's C (copy propagation in the
lowering, bounds checks out of line, the identifiers' mass), each a sitting and an ABI question
of its own, and is no part of this row.
