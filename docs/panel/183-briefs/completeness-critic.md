# Panel 183, brief for the completeness critic

You run twice and give no verdict.

**First, before any seat starts**: read `00-shared.md` and the four seats'
briefs (`compiler-engineer.md`, `llm-ergonomist.md`, `spec-warden.md`,
`historian.md`, all in `docs/panel/183-briefs/`), and return every framing
fact you could not verify with a command of your own: a number, a path, a line,
a count, a date, a quoted output, and every negative sentence (*nothing*,
*never*, *every one*, *all 15*), each with the command you ran and what it
printed. Run what you can in your own directory,
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/183-critic/`,
made with `git -C /Users/joseph/Temp/heroes/heroes-lang archive 171e8c45 |
tar -x -C <it>`, `rm -rf build`, and your own seed-built compiler; the
coordinator's measuring script is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p183/reach.py`
and its programs and outputs are in `.../scratchpad/p183/ab/`; re-derive
00-shared.md's counts (1358 files, 8 fences, 915,946 tokens, 55 declaration
lines, 34 statement lines in 12 files, 15 files at exit 1) with a method of
your own, not by re-running the script alone. The frozen lexer 00-shared.md
names is `.../scratchpad/instrument/tree/heroes`. Write this pass to
`docs/panel/183-reports/completeness-critic-briefs.md`, and stop: the
coordinator repairs the briefs before the seats start.

**Second, after the seats**: read the briefs and the four reports, and name
what is MISSING: a route nobody listed; a claim asserted and not measured,
with the command that would settle it; a contradiction between seats, and
which side is checkable and how; a framing fact a seat took on trust; and the
question the sitting should have asked and did not, including whether the
ffi-pragmatist's absence lost a C-facing half. Write it to
`docs/panel/183-reports/completeness-critic.md`. English, no em dashes.
