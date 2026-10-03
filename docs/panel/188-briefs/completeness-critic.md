# Panel 188, the completeness critic's brief

You are not a judge and give no verdict. Two passes (`/panel` § 3b and § 3c):

## First pass, now, before any seat is launched

Read every file of `docs/panel/188-briefs/` (`00-shared.md`, `00-facts.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`, `blind/`), and for **every framing fact**
(a number, a path, a line number, a count, a quoted sentence, a negative
sentence such as *nothing else judges* or *no sentence says*, and every word
that claims a search was done) run the command that settles it, in your own
copy `<scratchpad>/188-critic/` made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 826ddc2f | tar -x -C <copy>`,
your compiler built inside it from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`). F1 and F6 may be re-run from
`<scratchpad>/188-facts/` (copy them; never edit that folder).
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.

Then name what is MISSING: a route nobody listed, a question the sitting
should ask and does not, a seat handed a framing fact it cannot check, and a
fact that leans the briefs toward one route. Write
`docs/panel/188-reports/completeness-critic-briefs.md`: per fact, verified
(the command and its output) or not (what you ran and what it said); then the
missing items. The coordinator repairs the briefs from it before launching
the seats.

Rules: no paid run of any kind (no `claude -p`, no API call, no `heroes
measure --refresh`); never build or run inside the trunk, another seat's copy
or a lane's worktree; at most three processes at once; English, no em dashes.
The Windows box is offline; what needs it is unrun.

## Second pass, later

Over the seats' reports, when the coordinator sends you them.
