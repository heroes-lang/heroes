# Panel 184, brief for the completeness critic

You run twice and give no verdict. **You start no paid run**: no `claude -p`,
no API call, no cloud run (author instruction 2026-09-30, after panel 183's
critic started 80 sessions of its own). If a paid run would settle something,
say so in your report with its size, and the coordinator decides.

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-critic/`:
`git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff | tar -x -C <it>`,
`rm -rf build`, and your own compiler built from its seed. Never read, build
or run in the repository's working tree or another seat's directory. The
coordinator's measurements are in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p184/`
(read them; re-derive from your own copy, never by re-running the
coordinator's commands alone), and the recovery instrument is
`.../scratchpad/instrument/tool/recovery.py`, which you may run against your
own compiler into your own directory.

**First, before any seat starts**: read `00-shared.md` and the seats' briefs
(`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`, and `blind/brief-task1.md` to
`brief-task3.md` with the programs and outputs beside them, all in
`docs/panel/184-briefs/`), and return every framing fact you could not verify
with a command of your own: a number, a path, a line, a count, a date, a
quoted output, and every negative sentence (*nothing*, *no one of the 25*,
*no ruling*, *every other shape*), each with the command you ran and what it
printed. Check in particular 00-shared.md's nesting table cell by cell, the
instrument's counts and the reading of the 6 silent `over-indent` sites, the
line numbers it cites, and whether a blind brief leaks which variant is
today's or a project name. Write this pass to
`docs/panel/184-reports/completeness-critic-briefs.md`, and stop: the
coordinator repairs the briefs before the seats start.

**Second, after the seats**: read the briefs and the reports, and name what is
MISSING: a route nobody listed; a claim asserted and not measured, with the
command that would settle it; a contradiction between seats, and which side is
checkable and how; a framing fact a seat took on trust; and the question the
sitting should have asked and did not. Write it to
`docs/panel/184-reports/completeness-critic.md`. English, no em dashes.
