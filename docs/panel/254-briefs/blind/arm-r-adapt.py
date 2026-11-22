# Panel 254's fourth arm (R9): its make_sessions.py, check_folders.py and run.sh, made from the first
# area's by edits each asserted to match exactly once. Written 2026-11-22 by the scoring session.
# usage: python3 -I adapt_r.py <first area> <fourth arm's area>
import os, sys
first, area = sys.argv[1], sys.argv[2]


def edit(text, old, new):
    assert text.count(old) == 1, ('not exactly once', old[:80], text.count(old))
    return text.replace(old, new)


# make_sessions.py: one arm, three cells, the first draw's letters out of the pool, no verdict session.
t = open(os.path.join(first, 'tools/make_sessions.py')).read()
t = edit(t, "# Panel 254's blind seat: draws the labels,",
         "# Panel 254's fourth arm (R9, the resolution's text), adapted 2026-11-22 by the scoring session from the\n"
         "# first area's make_sessions.py by asserted edits: one arm, three cells, no verdict session, the first\n"
         "# draw's letters (q, y, p) out of the pool. The first file's header follows.\n"
         "# Panel 254's blind seat: draws the labels,")
t = edit(t, "ARMS = ['now', 'm', 'm0']", "ARMS = ['r']")
t = edit(t, "    ('t1', 'now', 'task-t1.md'), ('t1', 'm', 'task-t1.md'),\n"
            "    ('t2', 'm', 'task-t2.md'), ('t2', 'm0', 'task-t2.md'),\n"
            "    ('t3', 'm', 'task-t3.md'),\n",
         "    ('t1', 'r', 'task-t1.md'), ('t2', 'r', 'task-t2.md'), ('t3', 'r', 'task-t3.md'),\n")
t = edit(t, "pool = list('cfghjkpqvwxyz')", "pool = list('cfghjkvwxz')")
old_verdict = t[t.index("# the verdict session: the frozen spec"):t.index("# the order:")]
t = edit(t, old_verdict, "# no verdict session for the fourth arm (its registration.md says why)\n")
t = edit(t, "order.append('verdict')\n", "")
open(os.path.join(area, 'tools/make_sessions.py'), 'w').write(t)

# check_folders.py: 15 folders.
t = open(os.path.join(first, 'tools/check_folders.py')).read()
t = edit(t, "# Before any spend: order.txt names 26 folders,",
         "# Panel 254's fourth arm: the first area's check_folders.py with 26 made 15 (2026-11-22).\n"
         "# Before any spend: order.txt names 15 folders,")
t = edit(t, "if len(order) != 26 or len(set(order)) != 26:\n"
            "    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, not 26')",
         "if len(order) != 15 or len(set(order)) != 15:\n"
         "    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, not 15')")
open(os.path.join(area, 'tools/check_folders.py'), 'w').write(t)

# run.sh: the caps, the arm's ceiling, and the gate on the registration.
t = open(os.path.join(first, 'run.sh')).read()
t = edit(t, "#!/bin/bash\n",
         "#!/bin/bash\n"
         "# PANEL 254'S FOURTH ARM (R9, the resolution's text): the first area's run.sh adapted 2026-11-22 by the\n"
         "# scoring session, by asserted edits: each session capped at 0.60 USD, the arm bounded at 5.00 USD (the\n"
         "# author's 20 USD for the seat, 6.445551 spent by the first run, the total never past 11.45), and the\n"
         "# gate reading the arm's own registration.md (R9: its predictions registered before any session of it\n"
         "# runs) instead of the spec-warden's report, copied to registration-at-start.md and hashed in run.log.\n"
         "# Every other line as it ran. The first file's text follows.\n")
t = edit(t, "CAP=3.00\nCAP_VERDICT=3.00\nMARGIN=0.10\nCEILING=20.00\n",
         "CAP=0.60\nCAP_VERDICT=0.60\nMARGIN=0.10\nCEILING=5.00\n")
t = edit(t, "WARDEN=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/docs/panel/254-reports/spec-warden.md\n"
            "HEADING='## Predictions registered before the blind run'\n",
         "WARDEN=\"$HERE/registration.md\"\n"
         "HEADING='## Predictions registered before the fourth arm runs'\n")
t = edit(t, "if [ -f \"$HERE/warden-at-start.md\" ]; then\n"
            "    copy=\"$HERE/warden-at-resume-$(date '+%Y%m%d-%H%M%S').md\"\n"
            "else\n"
            "    copy=\"$HERE/warden-at-start.md\"\n"
            "fi\n",
         "if [ -f \"$HERE/registration-at-start.md\" ]; then\n"
         "    copy=\"$HERE/registration-at-resume-$(date '+%Y%m%d-%H%M%S').md\"\n"
         "else\n"
         "    copy=\"$HERE/registration-at-start.md\"\n"
         "fi\n")
t = edit(t, 'log "spec-warden report copied to', 'log "the fourth arm\'s registration copied to')
open(os.path.join(area, 'run.sh'), 'w').write(t)
print('made: tools/make_sessions.py, tools/check_folders.py, run.sh')
