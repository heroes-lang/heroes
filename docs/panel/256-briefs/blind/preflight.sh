#!/bin/bash
# PREFLIGHT ONLY: run.sh cut before the line that reads the key, so no billed session can run from it
# (written 2026-11-22 by panel 256's coordinator, as panel 254's was). Below, run.sh as it stands:
# Panel 256's blind seat (panel 254's runner, its caps, paths and gates changed by asserted edits): runs the
# sessions order.txt names, one at a time, never two, each from its own folder, every graded session capped
# at 1.00 USD and the verdict at 1.50, and stops before the sum would pass 19.50 USD, under the author's
# 20 USD for this seat (2026-11-22, relayed by the coordinator of M-rebel-library). A folder already
# holding run.json is skipped and its cost counted, so a stopped run resumes.
#
# NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
# finding 4: that sitting's were written 13 seconds after its runner started). This script refuses to
# start unless docs/panel/256-reports/spec-warden.md, in the lane-p256 worktree, holds the heading
#   ## Predictions registered before the blind run
# with at least one non-empty line under it before the next `## ` heading; it COPIES that report, as it
# stands when the runner starts, to warden-at-start.md beside run.log (a later start keeps the first copy
# and writes warden-at-resume-<time>.md), and logs the copy's sha256 (panel 253's critic, finding 6).
#
# Panel 251's runner leaked three ways (its critic's finding 2, defect 2589), and panel 253's critic found
# a fourth way out (its finding 5: panel 251's session t4-f-2 left cache-break-state-<its id>.json in
# /private/tmp/claude-501/). Every session here takes:
#   - standard input from /dev/null, so no session reads order.txt from the loop;
#   - the area as the top of an empty git repository of its own (.gitignore `*`, user reader
#     <reader@invalid>, no commit), so the status the CLI renders names no commit and no person;
#   - HOME as the area's home/, so the CLI's transcripts land there, never under ~/.claude/projects/;
#   - TMPDIR and CLAUDE_CODE_TMPDIR as the area's tmp/. WHETHER THESE TWO VARIABLES MOVE THE CLI'S
#     cache-break-state FILE INTO THE AREA IS UNRUN: no session was run to see it. So the free preflight
#     below lists /private/tmp/claude-<uid>/ before and after a session that makes no billed call, and
#     after every session the runner looks there for a file naming that session's id, stopping at the first.
#
# THE FREE PREFLIGHT: one session in preflight/, with the real prompt and every flag of a graded session,
# whose key is a string the API refuses (the coordinator's measurement of 2026-11-19, coord/blindtest/,
# made its probes so, each run.json `total_cost_usd: 0`). It must cost 0, leave no file naming its id
# under /private/tmp/claude-<uid>/, and its transcript must read clean by tools/scan.py (the real prompt
# alone, a git status naming `reader` and no commit, no CLAUDE.md, no memory); the runner stops otherwise.
# It is also the scan's first negative control on a transcript holding the real prompt.
#
# Panel 253's runner written 2026-11-19 by its brief writer; adapted for panel 254 and again for panel 256 on
# 2026-11-22 by their coordinators (panel 256: the caps, the ceiling, the warden's path, the registration's
# gate). Started by the coordinator of M-rebel-library alone (RUN.md):
#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/run.sh
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
cd "$HERE" || exit 2

CAP=1.00
CAP_VERDICT=1.50
MARGIN=0.10
CEILING=19.50
ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
WARDEN=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/spec-warden.md
# The coordinator's registration (the predictions and the arms' hashes), its sha256 written into RUN.md and here.
REGISTRATION="$HERE/tools/registration.md"
REGISTRATION_SHA=fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92
HEADING='## Predictions registered before the blind run'
CLI_TMP="/private/tmp/claude-$(id -u)"
REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
PROMPT="Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English."
log() { echo "$(date '+%Y-%m-%d %H:%M:%S') $*" | tee -a "$HERE/run.log"; }

# One runner at a time.
if ! mkdir "$HERE/run.lock" 2>/dev/null; then
    echo "another runner holds $HERE/run.lock: nothing run"
    exit 2
fi
trap 'rmdir "$HERE/run.lock"' EXIT

# The spec-warden's predictions first: the heading, a non-empty line under it, the report copied and hashed.
if ! grep -q -x "$HEADING" "$WARDEN" 2>/dev/null; then
    echo "the spec-warden's report holds no registered predictions yet ($WARDEN): nothing run"
    exit 2
fi
if ! awk -v h="$HEADING" '$0 == h { on = 1; next } on && /^## / { exit } on && /[^[:space:]]/ { found = 1 } END { exit found ? 0 : 1 }' "$WARDEN"; then
    echo "the heading stands in $WARDEN with nothing under it: nothing run"
    exit 2
fi
if [ -f "$HERE/warden-at-start.md" ]; then
    copy="$HERE/warden-at-resume-$(date '+%Y%m%d-%H%M%S').md"
else
    copy="$HERE/warden-at-start.md"
fi
cp "$WARDEN" "$copy" || { echo "could not copy $WARDEN: nothing run"; exit 2; }
log "spec-warden report copied to $(basename "$copy"), sha256 $(shasum -a 256 "$copy" | cut -c1-64)"

# The coordinator's registration second: its hash the one RUN.md names, copied beside run.log.
if [ "$(shasum -a 256 "$REGISTRATION" 2>/dev/null | cut -c1-64)" != "$REGISTRATION_SHA" ]; then
    echo "tools/registration.md is not the registered file (RUN.md names $REGISTRATION_SHA): nothing run"
    exit 2
fi
[ -f "$HERE/registration-at-start.md" ] || cp "$REGISTRATION" "$HERE/registration-at-start.md"
log "registration $REGISTRATION_SHA checked and copied"

# Before any spend: the order, the key, every folder not yet run holding its registered files alone,
# and the repairs.
[ -f order.txt ] && [ -f key.txt ] || { echo "order.txt or key.txt missing: nothing run"; exit 2; }
mkdir -p "$HERE/tmp"
python3 -I tools/check_folders.py || { echo "nothing run"; exit 2; }
python3 -I tools/check_area.py || { echo "nothing run"; exit 2; }

# The machine held awake for the runner's life (.claude/rules/verification.md § A long run holds the
# machine awake); a second assertion costs nothing where Amphetamine already holds one.
caffeinate -i -w $$ &
claude --version | tee -a "$HERE/run.log"

# One session from folder $1 with cap $2 and key $3; its exit status is the CLI's.
session() {
    (
        cd "$1" || exit 2
        env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
            TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp" \
            GIT_CEILING_DIRECTORIES="$(dirname "$HERE")" ANTHROPIC_API_KEY="$3" \
          claude -p "$PROMPT" \
            --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
            --tools "Read,Write" --allowedTools "Read,Write" \
            --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
            --max-budget-usd "$2" --output-format json < /dev/null > run.json 2> run.err
    )
}
# The session id a run.json names, or nothing.
session_id() { python3 -I -c "import json,sys; print(json.load(open(sys.argv[1])).get('session_id',''))" "$1/run.json" 2>/dev/null; }
# Whether a file naming session id $1 stands under the CLI's temporary folder outside the area.
left_outside() { [ -n "$1" ] && [ -d "$CLI_TMP" ] && [ -n "$(find "$CLI_TMP" -maxdepth 4 -name "*$1*" 2>/dev/null | head -1)" ]; }

# THE FREE PREFLIGHT (above): a key the API refuses, so no billed call; its folder holds a copy of the
# session brief alone, and it runs once (a preflight/run.json already there is not run again).
if [ ! -f "$HERE/preflight/run.json" ]; then
    mkdir -p "$HERE/preflight"
    cp tools/brief-session.md "$HERE/preflight/brief.md"
    ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-before.txt" 2>&1
    session "$HERE/preflight" "$CAP" "$REFUSED_KEY"
    ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-after.txt" 2>&1
    diff "$HERE/preflight/cli-tmp-before.txt" "$HERE/preflight/cli-tmp-after.txt" > "$HERE/preflight/cli-tmp-diff.txt"
    log "preflight: $CLI_TMP listing before and after differ in $(grep -c '^[<>]' "$HERE/preflight/cli-tmp-diff.txt") lines (other sessions on this Mac write there too; preflight/cli-tmp-diff.txt)"
fi
pre_cost=$(python3 -I -c "import json,sys; print(json.load(open(sys.argv[1])).get('total_cost_usd', 'unreadable'))" "$HERE/preflight/run.json" 2>/dev/null)
pre_id=$(session_id "$HERE/preflight")
log "preflight: cost $pre_cost, session $pre_id"
if [ "$pre_cost" != "0" ] && [ "$pre_cost" != "0.0" ]; then
    echo "the preflight cost $pre_cost, not 0: nothing more run"
    exit 2
fi
if left_outside "$pre_id"; then
    log "preflight: a file naming its session stands under $CLI_TMP: the variables do not keep the CLI inside the area; nothing more run"
    exit 2
fi
pre_scan=$(python3 -I tools/scan.py home/.claude/projects | grep -- '-preflight ')
log "preflight scan: ${pre_scan:-no transcript found}"
case "$pre_scan" in
    *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
    *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
esac

log "preflight only: stopped before the key is read; no paid session run"
exit 0
