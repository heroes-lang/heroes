# 068 — M-witnessed-seed: every seed since the archive descends from the one the Rust bootstrap emits

## Goal

The milestone the author scheduled on 2026-10-14, before the archived Rust
bootstrap left the tree: the seed witnessed by a second implementation, the
half of diverse double-compiling this repository could still run. Four items,
in its page's order: the bootstrap rebuilt with its pinned `rustc` and its
comparison at `m-bootstrap-archive` run again; a CI leg compiling the seed with
gcc as well as clang; the chain walked from that root to today's seed; and
only then the bootstrap out of the tree, its living citations pointing at its
tag. One lane, `lane-witnessed-seed`, opened on 2026-10-18 by the author's
instruction of the night before, *if you finish this step, start the next
one*, and closed that evening.

## What surprised

**The archive's own compiler could not cross the first link in a working
day.** Asked to build `e4a475cdd`, the first stage the walk needed, the
compiler clang makes of the tag's seed was still in its emitter after 7 h 26
min, 1,877 of 2,087 samples of its stack in `memmove`: the quadratic push of
the August compilers, which measurement 011 found and 045 § 7 met at three
tags. The Rust bootstrap emitted the same sources in 0.83 s. So the witness
crossed the first link itself, which is the stronger shape of the method: the
chain is rooted in the second implementation, not in a seed it vouched for.

**The rewrite of 2026-10-16 changed what a link is.** Variant A wrote each
tag's own seed at the tagged commit and left the seeds between out. So a seed
regenerated one commit before a close stands on the close (link 20's is what
`476a2692e`'s sources emit, seven commits of `selfhost/` before
`m-stated-grammar`), and the dropped seeds were the bridges across every
change that taught the compiler something and used it at once: `validated`
defined and called in one commit, the whole tree rewritten to `@=` in the
commit that taught the parser `@=`, `hero_program_sources` written by the
emitter and read for `--version`, and a cap the runtime raised when the
compiler had already passed it. Every crossing had a procedure the repository
had written down before (the ABI number put back, `bfeab994f`; the scaffold of
panel 222's R3) or the order its developers had kept by hand, the definition
before the use, and the walk replayed them. 28 of the 39 links needed none.

**Halving needs the history's shape.** Over the topological order, halving
took a stage on one lane, then on the other, and back, since a range past the
last stage alone does not exclude the lane it left; along the trunk's first
parents each commit holds every one before it, and buildability falls once.
And a merge whose lane teaches a form and uses it eighteen minutes later
(`18d8e18b8`, `\u{...}`) is a cycle seen from the trunk and a step seen from
the lane, whose own commit `6934f73fc` was the stage.

**A tree that leaves wants its archive, not its date.** Taking the bootstrap
out turned 243 citations dead, and the check's own message offered the cheap
repair, a date on each line, which exempts the line for good. The check
already held the better one, built for `issues/` six days before: a citation
of a folder that left is followed to the commit that holds it. Generalised to
two trees, it judges the same 243 at the bootstrap's tag, where a misspelt one
still fails, and none of the lines had to change for the check's sake.

**The formatter's probe over `tests` cannot end.** Finishing M-readonly-lend's
post-push probe found `fixedbugs-259`'s 84,027 `bracket` variants more than 61
CPU hours, the whole run about 262: defect 2139.

## What broke and why

- **The walk's own scripts, each found by its effect.** A compiler paired with
  its own runtime alone could not build a commit declaring a runtime function
  its runtime predates; the rule became the ABI's, the target's runtime where
  the ABIs agree. The halving's range past the last stage ping-ponged across
  lanes. And a helper returned text where a list was meant: the lane descent
  read a hash's second character as a commit, and the search for a seed's
  source, meant to try the commits that say the seed was regenerated first,
  never matched one; 048 says the order was the history's alone.
- **A gate that ran no suite.** `xargs -I` on macOS cannot build a command
  past 255 bytes, so item 4's first gate ran the two test sets, no suite, and
  wrote its done; read in the files, not the exit, and run again.
- **A stop that took one job too many.** A pattern meant for the probe's jobs
  over `tests` also matched one of the milestone's own files,
  `tests/harness/suite_spec.hero`, which was run again alone.
- **Files outside the repository's root.** The walk's logs and compilers lived
  in the session's scratch folder until the evening, against the hard stop;
  they moved under `.claude/worktrees/`, and what 048 rests on into its archive.
- **The first rewrite of the citations** cut 23 brace forms at their commas
  and wrote three possessives no reader could parse; every changed line was
  read, the files restored from `HEAD`, and the rewrite done again.
- **The second rewrite left `docs/learn/` out**, a living folder its listing
  of the living lines had skipped. `records/citations` found it: 197 of the
  243 dead citations were there, on 144 lines, which step 6's body counts as
  *197 lines*. A third pass rewrote the 120 lines that spelled the directory,
  keeping each path whole so a reader hands it to `git show` as written.
- **A path inside a revision, broken since 2026-08-19.** Reading those 120
  lines found a question asking for `git show
  944ee94^:archive/bootstrap-rs/heroes/src/ir/exprs.rs`, which fails: the move
  of that day had rewritten `crates/` inside a revision that holds `crates/`.
  The question's three such paths name `crates/` again, each asked of
  `944ee94^` with `git cat-file -e`, and no other living line holds one.
- **The guard refused, rightly**: a `git commit` with no pathspec, in a
  throwaway repository too; paths after `--` given as expansions; `head`
  beside a run of the net.

## What landed, and what carried forward

**The parts**, one commit each: the row opened, its page saying what it
starts from (`acfa47b6d`); item 1, the archived bootstrap rebuilt with its
pinned `rustc` and agreeing with the seed at its tag on 142 of 142 programs
(`229a6454b`); item 2, the CI's step that builds the seed with gcc and asks
its compiler for the C the compiler clang builds emits, green on the two
Linux legs of run 39106276951 (`85fea83af`); item 3, the walk, measurement
048, every one of the 39 seeds since the archive rebuilt byte for byte from
the one before it, rooted in the bootstrap's emission (`53d65fadf`); the rule
the walk made possible, every refresh rebuilt by the seed before it, its
route in a `Seed-link:` trailer the CI's job `seed-link` takes, the author's
instruction (`2883d3a59`, https://github.com/heroes-lang/heroes/issues/2140);
and item 4, the bootstrap out of the tree, kept at its tag, its living
citations saying so and `records/citations` following them there
(`7ce66022e`, its decision filed after the push, so that the commit it names
is one GitHub can show). Between them, M-readonly-lend's probe ended and
defect 2139 was filed (`0b57692c6`). No golden case was written, so none
waits for ratification, and nothing the compiler does moved, so no mutation
drill is offered; the questions are in `docs/learn/2026-10/18/`.

**Predictions scored at the close**: none. The page claims no warrant, and
the search `/step` gives, its name and `m-readonly-lend` over
`docs/roadmap/milestones/`, the spec's ledger and `docs/panel/`, finds only
M-readonly-lend's page scheduling this row.

**The gate, 2026-10-18, on `7ce66022e`**, the compiler clang built from the
seed, stage 2 and stage 3 emitting the same 26,591,884 bytes by `cmp`, stage
2 judging (`sources 08c492ac05cbb532`), the seed not refreshed: the full net,
31 suites four at a time and `cache` alone after, every one 0 failed: run
539, emission 1311, check 703, warnings 618, annotations 1180, surface 425,
fixes 1003, unsupported 353, corpus 70, determinism 580, lines 540, lldb 14,
probe 27, ir 30, emit 13, permissive 22, full 40, records 26, spec 31,
canonical 2, descriptors 636, grammar 9, order 3, runtime 8, special 10,
unseen 3, wholes 636, seed 1, layout 6, units 3, cache 7; the compiler's own
tests 1730 and the net's own 411, over the same sources. No program's
emitted C moved, the compiler's own identical but for its key and `emission`
1311 and 0, so the Linux arm64 bless the checklist owes a close that moves
the emitted C was not owed. The site's build, 38 claims over 20 pages, exit
0.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler
674 modules and 175,346 lines of Heroes, no line moved by step 6; the seed
unchanged, 263,383 lines, 26,240,524 bytes, 56,190 `#line`s; the spec 7846
vendored and 10,286 real against 16384, unchanged; the contract 8167 real as
refreshed 2026-09-29, step 6's one line moving the vendored count from 6170
to 6173, within `DELTA_GATE`'s 50; sittings 222, journals 69 numbered files,
milestone pages 61, measurements 47 documents, beats 136, questions in
`docs/learn/` 491 of which 466 open. Journal 067's *measurements 47* counted
the folder's entries, two of them archives: 45 documents by this count.

**The tag waits.** `m-witnessed-seed` is not placed on this close. Defects
2136 and 2137, `adjacent`, were filed at 05:01 UTC on 2026-10-18, six
minutes before `m-readonly-lend` was placed at 05:07, so they stood open at
it, and this tag would be their second, where `records/tagged` counts an
`adjacent` item as `blocking`. Asked of GitHub and git before the tag was
placed, rather than placed and read red as M-bounded-seed's was (journal
066). 2136's repair is in the build's rounds; 2137's, by its own body, is a
question for a sitting. The tag goes on the commit that closes both, as
`a3eaae009` did for M-bounded-seed.

**Carried**: defects 2136 and 2137, which hold the tag, 2137 by way of a
sitting; the CI's first run of the new archive on a one-commit checkout,
where `records/citations` should set the bootstrap's citations aside with its
reason, read after the push; defect 2139, the formatter's probe over `tests`
in `bracket`, which cannot end; the census of `check` and panel 187's R2,
not owed, nothing the compiler refuses having moved; the walk's worktrees and
the scratch files 048's archive does not keep, to remove; and the GitHub
milestone `M-witnessed-seed`, for the author to close.

**The chain entry**: row 69, `m-witnessed-seed`, closed 2026-10-18, this
journal.

## Tagged over a clean list, 2026-10-19

**`m-witnessed-seed` is placed on the commit this section is written in**, over
a list asked of GitHub before it was placed: no `blocking` or `systemic`
defect open, and no open decision but panel 225's ratification
(https://github.com/heroes-lang/heroes/issues/2148), the exception the
contract names. What held it, and what closed it, in one batch on the lane
`lane-witnessed-seed`:

- **defects 2136 and 2137**, by panel 225
  (`docs/panel/225-a-value-only-the-probe-sees-is-told-beside-a-refusal-read-from-the-programs-own-unit-preprocessed-whole-then-cut-at-marks-no-header-can-write.md`,
  provisional, author ratification pending): a value only the probe sees is
  told beside a refusal, read from a copy of the program's own unit
  preprocessed whole and then cut at marks no header can write. The route is
  the union of the sitting's two prototypes, which the critic named and the
  compiler-engineer built in a third step; it was run on the Windows box
  before the synthesis adopted it, and landed at `c03e81984`;
- **defect 2145**, found by both seats beside the question: the landed
  repair of 2128 wrote a refused constant's accessor inert before
  preprocessing, and so told a value C defines undefined; retired by the
  same landing;
- **defect 2146**, found while the synthesis was written: the probe's
  `-D main` word, which every header the unit includes read too (`bf75f3ff1`);
- **defect 2147**, found by the compiler-engineer building the union: a kept
  verdict of panel 216's probe and of panel 218's C2 blind to its header's
  edit, a wrong value at exit 0 on a warm build (`295cd8433`);
- **defects 2142 and 2144**, the two red legs of the close's push: six builds
  of one cold program at once on Windows, measured and repaired on the box
  (`aaedffa9d`, `4f81c0aeb`), and the bootstrap's parent directory on a
  one-commit checkout (`df6e22223`).

**What surprised.** The two prototypes the seats built were one route found
twice, cut after the preprocessor rather than before it, and each answered
programs the other did not; the critic's attack on both, a header writing the
mark's own text, was answered by neither. A key alone did not close it: the
compiler-engineer made a header learn the key through defect 2146's word, and
only the count of the marks written held. And the sitting's question found
two defects of the probe every build runs, neither of them the copy's.

**The gate**, on this Mac, stage 2 built from the seed (`sources
7792ecfe50a531e8`): stage 2 and stage 3 emit the compiler's 26,694,926 bytes
alike, so the seed was not refreshed; the compiler's own tests 1738 and the
net's own tests 411, all passed; the full net's 31 suites at 0 failed,
`cache` 8 alone after the rest, `emission` 1315 once the two new `run/`
cases' emissions were blessed; on Linux arm64, in its container with the
same stage 2, `emission` failed those two alone and passed the two cases
this Mac skips, so its bless was not owed (the gate's commit, `17aa3ec38`).

**Carried**: defect 2143, `adjacent` and filed after `m-readonly-lend`, whose
probe half the landing closed and whose other half, a second refused unit's
own clang refusal told one build later, waits; defect 2139, the formatter's
probe over `tests` in `bracket`; and panel 225's ratification. The census of
`check` and panel 187's R2 are not owed: every change of the batch that moves
a verdict is the build's value probe, which `heroes check` does not run (it
exits 0 on a program binding `K_DIV` as `INT_MIN / -1`, which `build`
refuses).
