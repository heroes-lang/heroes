# Panel 255, the completeness critic

Not a seat and no verdict on the route (`.claude/skills/panel/SKILL.md` § 3b, § 3c). Two readings, each
its own report under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p255/docs/panel/255-reports/`,
written as you go. **Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p255-scratch/critic/`,
a detached worktree at `a75179641`; everything you run, you run there (`export TMPDIR=<copy>/p255/tmp`),
never in a seat's folder and never in `lane-p255` but for your report. `00-shared.md` § 8 binds you too:
no paid run, no credential printed, no GitHub write, no file outside the repository's root.

## First reading: the briefs, before any seat (`critic-briefs.md`)

Read `docs/panel/255-briefs/` whole in `lane-p255`: `00-shared.md`, `2679-question.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, `repro/`. Re-run by command, in your copy, every framing fact you can: each number,
path, line number and count, and each **negative sentence** (*no case keeps ...*, *not on disk*, *no CI leg
names ...*), which is a claim about a search and is checked as one. Say which facts are carried and whether
a seat is handed one as a premise. Read the option set as CLAUDE.md § RUN IT asks: a route nobody listed,
what would have to be true for it, and a question the sitting should ask and does not. Check that the two
seats' tasks between them build and run every route the resolution could adopt on every platform it would
reach, and name a gap. Docker is free to use (`00-shared.md` § 7); a long container run goes in the
background and is checked every few minutes. Every finding numbered, its command and result beside it,
then `## What the coordinator must repair before any seat starts`, most important first.

## Second reading: the reports, before the synthesis (`critic-reports.md`)

Read the briefs again as repaired, then `compiler-engineer.md` and `ffi-pragmatist.md` under
`255-reports/`. Name what is missing: a route nobody listed or nobody built; a claim asserted and not
measured with the command that settles it; a contradiction between the seats (each builds T on its own C library, and a route's
verdicts on two platforms: compare them) and which is checkable, checked in your copy by re-running a seat's program
from a copy of its files you make yourself, never inside its folder; a case whose verdict a seat reports
moved without the run that shows it; and the question the sitting should have asked. Every finding
numbered, its command and result beside it; end with `## What the synthesis must settle`, most important
first.

## What the first reading did

Run 2026-11-22, its agent's run 1,520 s long and its report last written at 15:26:56 (`stat`), on the briefs as first written;
13 findings in `critic-briefs.md`. Every repair it asked was made before any seat started: `00-shared.md`
names each by *(critic N)*, shapes 7 and 8 were added to `repro/` and run by the coordinator on both
platforms (`repro/mac7.log`, `repro/linux7.log`), and both seat briefs were rewritten around the widened
route set (T, the composition by calibration, F in assembly), one platform per seat with its real
libraries, and the run lists of finding 7.

## Third reading: round two's reports, before the synthesis (`critic-r2-reports.md`)

Added 2026-11-22 when round two (`round2.md`) was convened on the second reading's findings, and its
brief read first by a critic (`critic-round2-brief.md`). Read `round2.md`, then
`compiler-engineer-r2.md` (Linux) and `ffi-pragmatist-r2.md` (Darwin). The design asks ONE set of
verdicts on both platforms (its rule 8): diff the two seats' tables row by row, for every column
(*today*, *C4*, *C4 forced to fail*, *C, 2679 alone*), over the union and each seat's new shapes, and
port and run on the other platform any shape one seat wrote and the other did not, where it decides a
split. Name: every row where the two C4s disagree, and which is the safe direction; every MUST row
either C4 fails; every residue each seat names (a MUST BE REFUSED shape that still runs) and whether it
is shared with today; the correct programs each C4 refuses that today runs (the author's, as ruling B
was); whether *C, 2679 alone* on each platform moves any context row toward running inside a signal
against today. The seats' files may be read and copied into your copy to re-run, never run in place.
End with `## What the synthesis must settle`, most important first.
