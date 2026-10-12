# 063 — M-issue-files: every item one issue, transcribed with its own days, and then on GitHub alone

## Goal

The author's instruction of 2026-10-04, meant as: *every defect, every
decision and everything in the record should already have the structure to be
copied one file to one GitHub issue, keeping their sequence and the commit that
settled each*. The milestone gave every item a file of `issues/` with a card
saying what GitHub needs (steps 1 to 8), then, by the author's instruction of
2026-10-07, the credit where somebody outside the project got there first and a
defect's number kept as its name (steps 9 to 12); panel 212, ratified
2026-10-11, made the transcription a program of the harness, which imported
issues 1 to 2,029 in the files' order with their own days (steps 13 to 18). The
author's answers of 2026-10-12 then chose GitHub alone: the program writes and
reads the issues, the rules, the skills and the checks were rewritten for it,
and `issues/` left the tree (steps 19 to 23).

## What surprised

- **GitHub keeps an issue's own days when it is imported.** The issue import
  endpoint, an unofficial preview, sets `created_at` and `closed_at` from the
  request (measurement 041), so the dates the author asked to keep are
  GitHub's own and not only a line in the body. The REST route has no such
  field, which is why every later correction reports a day and never writes
  one.
- **The body GitHub stores is not always the body it was sent.** A literal
  `\u0001` or `\u009b` in a card came back in caret notation, on the import
  route and on the REST route alike (defect 675); the Markdown oracle, which
  renders and writes nothing, does not show it. Only reading every import
  back found it, and the repair writes such a backslash as an entity inside an
  HTML `<code>` element.
- **The earliest commit of a card is the one that opened it.** Panel 212's D4
  named a closed card's `self` commit as its file's first, which is the
  commit that filed it open: wrong for 25 of 75 such cards. The commit that
  wrote `commit: self` is the one that settled it, asked with git's pickaxe.
- **A sentence about a range is a citation to an instrument that reads
  ranges.** A comment of the check that holds the code's defect numbers to
  GitHub wrote the old series as `defects 001 to 677`, and the check read it
  as 677 citations: the 42 it reported missing were exactly the numbers that
  were never issued.
- **The first defect named by its GitHub number was the compiler's.** Two
  harness modules each declared a record `Held` and both called one generic
  function; `check` accepted the program and clang refused its C, the two
  instances sharing one name (defect 2030). Panel 093 had settled the same
  collision for the names of synthetic types and never asked it of the
  functions instantiated at them; every shape beside it, a list, a map, two
  type parameters and a function type, collided the same way.
- **Nothing let a writer read an issue before it was public.** `render` read
  only the files of `issues/`, and `file` opened an issue at once; `draft`
  now prints what `file` would open, and its first use moved a run's result
  back beside its reproducer.

## What broke and why

- **GitHub failed some imports on its own internal error.** Each was read
  back first, no issue having been made, and the import asked again, three
  times at most (step 15).
- **A restart of the session killed what it had started.** The gate and two
  lanes' agents stopped with it; the gate is now started in a session of its
  own, and an agent is resumed with its transcript.
- **A truncated name resolved into the wrong tree.** A living file cited
  `docs/records/log/2026-09-13-1400-…`, and two trees, the log and the record
  of closed items, held an entry of that minute; the rewrite chose the second.
  Checking every replaced path against the map of 2,029 found it and git
  named the tree each came from.
- **The write hook judged files halfway through a change made in several
  edits**, refusing names the next edit defined; each refusal was the
  hook's moment, not the file's, and the file checked whole afterwards.

## What landed, and what carried forward

**The milestone's items, closed on GitHub**: the renderer (issue 1655,
`c98f14b1`), the labels and the milestones (1656, `d67f95ae`), the
transcription (1657, `6461f3e6`), `kind:learn` gone (1689, `d67f95ae`), the
thanks page's group (2014, `cddad7fe`), where an issue lives (2018,
`2d4e8b95`), and defect 2030 (`92e08d82`, its walk's `# ORDER:` mark again at
`38d9eceb`). The decisions are issues: the author's answers (2023, 2027), the
long run that asks the machine whether something holds it awake (2031), `draft`
(2032), and a sitting with no ratification issue linked to its own file (2033).

**The second pass ran that evening** (panel 212's R9 item 6, the word `link`,
step 24, on the author's yes): 2,030 issues read, 1,602 bodies written with
6,676 links, each read back as it was sent, from 20:51 to 22:11 with no write
refused; a second run linked the one issue filed after the first had read the
list (2033, two links), and a dry run after it would write nothing. 41 names
stay text, nothing answering them. GitHub's rendering of all 2,033 issues,
read as HTML afterwards, holds no `issue-link` and no `user-mention` anchor.
Every body as it stood was saved before the pass ran.

**The gate, 2026-10-12 20:27 to 20:57, on the compiler built from the
regenerated seed, the fixpoint verified byte for byte** (`seed/heroes.c`
1,333,811 lines): the compiler's own tests 1620 passed, 0 failed; the net's
own 377 passed, 0 failed; the whole net, four suites at a time and `cache`
alone, 8,319 passed: emission 1241, annotations 1064, fixes 996, check 699,
wholes 601, descriptors 601, warnings 584, determinism 545, lines 507, run 506,
surface 424, unsupported 248, corpus 70, full 38, spec 31, ir 29, probe 27,
records 26, permissive 21, emit 12, special 10, grammar 9, runtime 8, cache 7,
layout 6, unseen 3, units 3, canonical 2, and `order` 1 passed and 2 failed,
the mark the repair had lost. With it restored and the seed regenerated for
its three lines, the last run, at `9ccf7d41`, read order 3, layout 6, canonical
2, wholes 601, descriptors 601, records 26, unseen 3 and spec 31, all 0
failed, the compiler's own 1620, the net's own 377 and the issue program's 154
passed; the site's build checked its 38 claims over 188 pages. **Linux arm64**,
`emission` whole in its container with the round's compiler: 1229 passed and 0
failed before and after a bless, every one of the 600 blessed files equal to
the Mac's, the two cases this Mac skips holding no generic instance.

**Status at the close**, for `docs/ROADMAP.md` § Where we are: the compiler
618 modules and 161,393 lines of Heroes, the seed 1,333,811 lines of C; the
spec 7655 vendored and 10,050 real against 10240; the contract 6186 vendored
and 8167 real, the real row refreshed 2026-09-29; sittings 210, journals 64
numbered files, milestone pages 54, measurements 40, beats 131, questions 469
in `docs/learn/` of which 444 open; the issues are GitHub's, 2,033 of them
when this was written.

**Carried**: the GitHub milestone `M-issue-files` closed, a milestone's write
staying the author's every time; the organisation's issue field `Defect`
(R6 item 5), which no run relies on, the author's to create if wanted. After
the push, as the optimistic chain says: the formatter's probe, a comment of
`selfhost/print/fmt.hero` having moved (step 20); no census and no R2, since
nothing the compiler refuses and no recovery file moved.

**Corrected 2026-10-12**, the same evening: the formatter's probe carried above is
not owed for this push, by the author's word, meant as *is it really essential?
are we wasting three hours on a comment?*, then *skip it and write it into the
rule*. The comment's emitted C is byte-identical, 52,126,543 bytes either way with
`cmp` silent, and `.claude/rules/verification.md` § The formatter's probe carries
the exception; the probe M-inferred-cell's changes owed stays owed.

**The chain entry**: row 83, `m-issue-files`, closed 2026-10-12, this
journal.
