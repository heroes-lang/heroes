# Panel 260's blind runner: its differences from panel 256's, by diff

Written 2026-11-26 by the blind run's session, **before any session**, as `blind/registration.md` § Every session's
folder and `llm-ergonomist.md` § The size, the spend and the cap ask: the runner is panel 256's
(`docs/panel/256-briefs/blind/`) as panel 257 adapted it and panel 259 used it (rebuilt by panel 259 from 257's diffs,
its files read from `docs/panel/259-reports/llm-ergonomist-blind-answers.tar.xz`; panel 257's brief-session.md rebuilt
here again from its own diff in `docs/panel/257-reports/llm-ergonomist-blind-answers.tar.xz` and `cmp`-equal to 259's
copy), adapted for panel 260. Each changed file has a `diff -u` against panel 256's committed bytes below, then one
against panel 259's file, which says what panel 260 changed on top.

**Unchanged by hash** (`cmp` equal to panel 256's committed bytes): `scan.py`, `check_area.py`, `read_runs.py`,
`spent.py`, `brief-verdict.md`, `after.py`, `reads.py`. **Unchanged from panel 259's bytes**: `brief-verdict-reports.md`.
**Copied**: `synthesis-adopted.diff` and `synthesis-rival.diff` (`docs/panel/260-reports/`'s, read by `preflight.sh`'s
arm check), `registration.md` (`blind/registration.md` with the section *Written 2026-11-26, after the landing*, its
sha256 pinned in `run.sh`), `inputs.sha256` (the three arms, both tasks and t2's 20 program files, every hash the
registration's).

## What changed, in words

- **`brief-session.md`**: panel 257's bytes with the two lines `blind/registration.md` § Every session's folder
  rewrites, the `programs/` line and the `answer` field, word for word; its change 1, `spec.md`, is 257's own. No other
  line differs from 257's (the diff against 259's copy, below, is those two hunks alone).
- **`run.sh`**: panel 259's, flag for flag (`--bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5`, `--tools "Read,Write" --allowedTools "Read,Write"`, the same `--disallowedTools`, `--max-budget-usd
  "$2"`, `< /dev/null`, `env -i`, `HOME`, `TMPDIR`, `CLAUDE_CODE_TMPDIR`, `GIT_CEILING_DIRECTORIES`), the caps 0.60 and
  1.20, the margin 0.10 and **the ceiling 20.00, the author's for the whole run**; `WARDEN` is
  `docs/panel/260-reports/spec-warden.md`, its predictions section pinned (`818747c9…`); `REGISTRATION_SHA` this
  registration copy's; **the session brief pinned by its sha256 (`ca8e2077…`), so a brief differing by any other
  line refuses the start**, as § Every session's folder asks and panel 259's runner did not check; comments.
- **`make_sessions.py`**: the programs a folder holds: t1 none; t2 the six folders `programs/p1/` to `p6/`, 20 files,
  nested. The arms, the label pool, the draw and the order: panel 257's.
- **`check_folders.py`**: a comment; its code is panel 257's, which reads nested `programs/` paths already.
- **`make_verdict.py`**: panel 259's, its programs t2's 20 files (t1 has none) and its comment.
- **`preflight.sh`**: the arm check applies each registered diff to `now` with `patch -p1` and `cmp`s the result with
  the arm (this sitting's diffs touch two lines, 401 and 508, where 259's replaced one); the estimate it prints; comments.

## diff of brief-session.md, against panel 256's bytes

```diff
--- 256/brief-session.md
+++ 260/brief-session.md
@@ -1,31 +1,31 @@
 # Your brief
 
 You judge one thing: whether the language's specification, read with no other file, lets a language
-model predict what a program does when it is built and run, and write the program its author meant,
-on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**
+model predict what a program does when it is built and run. Your verdict is an experiment, not an
+opinion: **answer from the specification, don't opine.**
 
 ## Your inputs, and nothing else
 
 The files in this directory are your whole input. Read no other file, and do not ask for one.
 
-- `heroes-spec.md`: the language's specification, the whole language.
+- `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
+- `programs/`: where the task names a folder of that name, the files it holds; a task that names none
+  has none.
 
 ## Your method
 
-Read the task and do what it asks. Write every file you write whole, with its path. For every place
+Read the task and do what it asks, each item in its order and under its own name. For every place
 where the specification left you a choice, say the choice you made and what the other would produce:
 a program the compiler refuses, a program that stops with an error when run, a program that never
 ends, or a program that runs and gives a different answer than intended.
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, each question answered in its order; every file whole, under its
-  path.
+- `answer`: what the task asks, each job or question in its order; every file whole, under its
+  path; and each job's `prediction` and `confidence` where the task asks them.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
-- `prediction`: whether the compiler accepts the program, every line the program writes, in order,
-  and how it ends: normally, by an abort and what the abort says, or not at all.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
   this directory reached your context, and which.
 
```

## diff of brief-session.md, against panel 259's

```diff
--- 259/brief-session.md
+++ 260/brief-session.md
@@ -10,7 +10,8 @@
 
 - `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
-- `programs/`: the C files the task names.
+- `programs/`: where the task names a folder of that name, the files it holds; a task that names none
+  has none.
 
 ## Your method
 
@@ -21,8 +22,8 @@
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, each question answered in its order, each program or design under
-  its own name.
+- `answer`: what the task asks, each job or question in its order; every file whole, under its
+  path; and each job's `prediction` and `confidence` where the task asks them.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
```

## diff of run.sh, against panel 256's bytes

```diff
--- 256/run.sh
+++ 260/run.sh
@@ -1,68 +1,84 @@
 #!/bin/bash
-# Panel 256's blind seat (panel 254's runner, its caps, paths and gates changed by asserted edits): runs the
-# sessions order.txt names, one at a time, never two, each from its own folder, every graded session capped
-# at 1.00 USD and the verdict at 1.50, and stops before the sum would pass 19.50 USD, under the author's
-# 20 USD for this seat (2026-11-22, relayed by the coordinator of M-rebel-library). A folder already
-# holding run.json is skipped and its cost counted, so a stopped run resumes.
+# Panel 260's blind seat (panel 256's runner as panel 257 adapted it and panel 259 used it, adapted again for panel
+# 260: see tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
+# time, never two, each from its own folder: the 30 graded ones capped at 0.60 USD each, the verdict at 1.20, and
+# stops before the sum plus the next session's cap plus a 0.10 margin would pass 20.00 USD, the author's ceiling for
+# this run (the answer *ok a*, relayed at 10:37 on 2026-11-25, recorded in docs/panel/260-briefs/llm-ergonomist.md;
+# registration.md's cap section).
+# A folder already holding run.json is skipped and its cost counted, so a stopped run resumes.
 #
 # NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
-# finding 4: that sitting's were written 13 seconds after its runner started). This script refuses to
-# start unless docs/panel/256-reports/spec-warden.md, in the lane-p256 worktree, holds the heading
+# finding 4). This script refuses to start unless $WARDEN holds the heading
 #   ## Predictions registered before the blind run
-# with at least one non-empty line under it before the next `## ` heading; it COPIES that report, as it
-# stands when the runner starts, to warden-at-start.md beside run.log (a later start keeps the first copy
-# and writes warden-at-resume-<time>.md), and logs the copy's sha256 (panel 253's critic, finding 6).
+# with at least one non-empty line under it before the next `## ` heading, and that section's sha256 is the one
+# pinned below (the section as it stood when this area was prepared, 2026-11-26); it COPIES the report, as it
+# stands when the runner starts, to warden-at-start.md beside run.log (a later start keeps the first copy and writes
+# warden-at-resume-<time>.md), and logs the copy's sha256 (panel 253's critic, finding 6).
 #
-# Panel 251's runner leaked three ways (its critic's finding 2, defect 2589), and panel 253's critic found
-# a fourth way out (its finding 5: panel 251's session t4-f-2 left cache-break-state-<its id>.json in
-# /private/tmp/claude-501/). Every session here takes:
+# Every session takes (panel 251's leaks, defect 2589, and panel 253's fourth way out):
 #   - standard input from /dev/null, so no session reads order.txt from the loop;
-#   - the area as the top of an empty git repository of its own (.gitignore `*`, user reader
-#     <reader@invalid>, no commit), so the status the CLI renders names no commit and no person;
+#   - the area as the top of an empty git repository of its own (.gitignore `*`, user reader <reader@invalid>,
+#     no commit), so the status the CLI renders names no commit and no person;
 #   - HOME as the area's home/, so the CLI's transcripts land there, never under ~/.claude/projects/;
-#   - TMPDIR and CLAUDE_CODE_TMPDIR as the area's tmp/. WHETHER THESE TWO VARIABLES MOVE THE CLI'S
-#     cache-break-state FILE INTO THE AREA IS UNRUN: no session was run to see it. So the free preflight
-#     below lists /private/tmp/claude-<uid>/ before and after a session that makes no billed call, and
-#     after every session the runner looks there for a file naming that session's id, stopping at the first.
+#   - TMPDIR and CLAUDE_CODE_TMPDIR as the area's tmp/; after every session the runner looks under
+#     /private/tmp/claude-<uid>/ for a file naming that session's id, and stops at the first.
+# And, panel 256's `--bare`, which is what keeps the repository's CLAUDE.md (above this area) out of a session.
 #
-# THE FREE PREFLIGHT: one session in preflight/, with the real prompt and every flag of a graded session,
-# whose key is a string the API refuses (the coordinator's measurement of 2026-11-19, coord/blindtest/,
-# made its probes so, each run.json `total_cost_usd: 0`). It must cost 0, leave no file naming its id
-# under /private/tmp/claude-<uid>/, and its transcript must read clean by tools/scan.py (the real prompt
-# alone, a git status naming `reader` and no commit, no CLAUDE.md, no memory); the runner stops otherwise.
-# It is also the scan's first negative control on a transcript holding the real prompt.
+# THE FREE PREFLIGHT: one session in preflight/, with the real prompt and every flag of a graded session, whose
+# key is a string the API refuses (panel 256's measurement: run.json `total_cost_usd: 0`, `terminal_reason:
+# api_error`). It must cost 0, leave no file naming its id under /private/tmp/claude-<uid>/, and its transcript
+# must read clean by tools/scan.py (the real prompt alone, a git status naming `reader` and no commit, no
+# CLAUDE.md, no memory); the runner stops otherwise. It runs once: a preflight/run.json already there is kept.
+# `bash run.sh --preflight-only` stops right after it, BEFORE THE KEY IS READ.
 #
-# Panel 253's runner written 2026-11-19 by its brief writer; adapted for panel 254 and again for panel 256 on
-# 2026-11-22 by their coordinators (panel 256: the caps, the ceiling, the warden's path, the registration's
-# gate). Started by the coordinator of M-rebel-library alone (RUN.md):
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/run.sh
+# A session that is not a `success` (subtype success, is_error false, terminal_reason completed) stops the runner
+# (exit 3), its folder kept as it ended and its cost counted; running run.sh again continues after it, that folder
+# skipped. A session still running after $SESSION_LIMIT seconds is killed by its pid (never by name) and counts as
+# a stop. SIGINT and SIGTERM to the runner kill the session in flight and remove the lock.
+#
+# Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation (panel 260):
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-llm-ergonomist/run.sh
 set -u
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
+PREFLIGHT_ONLY=0
+case "${1:-}" in
+    "") ;;
+    --preflight-only) PREFLIGHT_ONLY=1 ;;
+    *) echo "usage: run.sh [--preflight-only]"; exit 2 ;;
+esac
 
-CAP=1.00
-CAP_VERDICT=1.50
+CAP=0.60
+CAP_VERDICT=1.20
 MARGIN=0.10
-CEILING=19.50
+CEILING=20.00
+REGISTERED_CEILING=20.00
+SESSION_LIMIT=900
 ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
-WARDEN=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/spec-warden.md
-# The coordinator's registration (the predictions and the arms' hashes), its sha256 written into RUN.md and here.
+WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/260-reports/spec-warden.md
+WARDEN_SECTION_SHA=818747c952c3fddf1966862747e0549d245bd9e2fa2b73b91ff47755f77dd7ea
+# The coordinator's registration (the arms' hashes, the cap and the predictions), pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92
+REGISTRATION_SHA=369e4fa1e2b4a70e892c172cae9d10b4c704a0fa334ccd918f3c6647d3e9e672
 HEADING='## Predictions registered before the blind run'
 CLI_TMP="/private/tmp/claude-$(id -u)"
 REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
 PROMPT="Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English."
 log() { echo "$(date '+%Y-%m-%d %H:%M:%S') $*" | tee -a "$HERE/run.log"; }
+CHILD=""
 
-# One runner at a time.
+# One runner at a time; the lock names its pid.
 if ! mkdir "$HERE/run.lock" 2>/dev/null; then
-    echo "another runner holds $HERE/run.lock: nothing run"
+    echo "run.lock stands (pid $(cat "$HERE/run.lock/pid" 2>/dev/null || echo unknown)): another runner, or one killed hard; if none is alive, remove run.lock by hand: nothing run"
     exit 2
 fi
-trap 'rmdir "$HERE/run.lock"' EXIT
+echo $$ > "$HERE/run.lock/pid"
+cleanup() { [ -n "$CHILD" ] && kill "$CHILD" 2>/dev/null; rm -f "$HERE/run.lock/pid"; rmdir "$HERE/run.lock" 2>/dev/null; }
+trap cleanup EXIT
+trap 'log "signal received: runner stopping"; exit 3' INT TERM HUP
 
-# The spec-warden's predictions first: the heading, a non-empty line under it, the report copied and hashed.
+# The spec-warden's predictions first: the heading, a non-empty line under it, the section pinned, the report copied.
+section_sha() { awk -v h="$HEADING" '$0 == h { on = 1; print; next } on && /^## / { exit } on { print }' "$1" | shasum -a 256 | cut -c1-64; }
 if ! grep -q -x "$HEADING" "$WARDEN" 2>/dev/null; then
     echo "the spec-warden's report holds no registered predictions yet ($WARDEN): nothing run"
     exit 2
@@ -71,39 +87,52 @@
     echo "the heading stands in $WARDEN with nothing under it: nothing run"
     exit 2
 fi
+if [ "$(section_sha "$WARDEN")" != "$WARDEN_SECTION_SHA" ]; then
+    echo "the warden's registered predictions are not the section pinned in run.sh ($WARDEN_SECTION_SHA): nothing run"
+    exit 2
+fi
 if [ -f "$HERE/warden-at-start.md" ]; then
     copy="$HERE/warden-at-resume-$(date '+%Y%m%d-%H%M%S').md"
 else
     copy="$HERE/warden-at-start.md"
 fi
 cp "$WARDEN" "$copy" || { echo "could not copy $WARDEN: nothing run"; exit 2; }
-log "spec-warden report copied to $(basename "$copy"), sha256 $(shasum -a 256 "$copy" | cut -c1-64)"
+log "spec-warden report copied to $(basename "$copy"), sha256 $(shasum -a 256 "$copy" | cut -c1-64), predictions section $WARDEN_SECTION_SHA"
 
-# The coordinator's registration second: its hash the one RUN.md names, copied beside run.log.
+# The coordinator's registration second: its hash the one pinned here, copied beside run.log.
 if [ "$(shasum -a 256 "$REGISTRATION" 2>/dev/null | cut -c1-64)" != "$REGISTRATION_SHA" ]; then
-    echo "tools/registration.md is not the registered file (RUN.md names $REGISTRATION_SHA): nothing run"
+    echo "tools/registration.md is not the registered file (run.sh pins $REGISTRATION_SHA): nothing run"
     exit 2
 fi
 [ -f "$HERE/registration-at-start.md" ] || cp "$REGISTRATION" "$HERE/registration-at-start.md"
 log "registration $REGISTRATION_SHA checked and copied"
 
-# Before any spend: the order, the key, every folder not yet run holding its registered files alone,
-# and the repairs.
+# The session brief third: panel 257's bytes with the two lines blind/registration.md § Every session's folder rewrites
+# and no other (registration-runner.md shows its diff); a brief that differs refuses the start.
+BRIEF_SHA=ca8e20776dfdb6253ccf101e30f9e829ec2401d3d184ba2d95c6d389bd058beb
+if [ "$(shasum -a 256 tools/brief-session.md 2>/dev/null | cut -c1-64)" != "$BRIEF_SHA" ]; then
+    echo "tools/brief-session.md is not the registered brief (run.sh pins $BRIEF_SHA): nothing run"
+    exit 2
+fi
+log "brief-session.md $BRIEF_SHA checked"
+
+# Before any spend: the order, the key, every folder not yet run holding its registered files alone, and the repairs.
 [ -f order.txt ] && [ -f key.txt ] || { echo "order.txt or key.txt missing: nothing run"; exit 2; }
 mkdir -p "$HERE/tmp"
 python3 -I tools/check_folders.py || { echo "nothing run"; exit 2; }
 python3 -I tools/check_area.py || { echo "nothing run"; exit 2; }
 
-# The machine held awake for the runner's life (.claude/rules/verification.md § A long run holds the
-# machine awake); a second assertion costs nothing where Amphetamine already holds one.
-caffeinate -i -w $$ &
+# The machine held awake for the runner's life (.claude/rules/verification.md § A long run holds the machine
+# awake); a second assertion costs nothing where Amphetamine already holds one.
+caffeinate -i -w $$ > /dev/null 2>&1 < /dev/null &
 claude --version | tee -a "$HERE/run.log"
 
-# One session from folder $1 with cap $2 and key $3; its exit status is the CLI's.
+# One session from folder $1 with cap $2 and key $3; its exit status is the CLI's. The session is the subshell's
+# own process (exec), so the watchdog and the cleanup kill it by pid.
 session() {
     (
         cd "$1" || exit 2
-        env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
+        exec env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
             TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp" \
             GIT_CEILING_DIRECTORIES="$(dirname "$HERE")" ANTHROPIC_API_KEY="$3" \
           claude -p "$PROMPT" \
@@ -111,15 +140,28 @@
             --tools "Read,Write" --allowedTools "Read,Write" \
             --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
             --max-budget-usd "$2" --output-format json < /dev/null > run.json 2> run.err
-    )
+    ) &
+    CHILD=$!
+    # the watchdog polls once a second and keeps no stdout, so no orphaned `sleep` holds a pipe open (`run.sh | tee`)
+    ( i=0; while [ "$i" -lt "$SESSION_LIMIT" ]; do sleep 1; i=$((i + 1)); kill -0 "$CHILD" 2>/dev/null || exit 0; done
+      kill "$CHILD" 2>/dev/null ) > /dev/null 2>&1 < /dev/null &
+    local watch=$!
+    wait "$CHILD"
+    local status=$?
+    kill "$watch" 2>/dev/null
+    wait "$watch" 2>/dev/null
+    CHILD=""
+    return $status
 }
 # The session id a run.json names, or nothing.
 session_id() { python3 -I -c "import json,sys; print(json.load(open(sys.argv[1])).get('session_id',''))" "$1/run.json" 2>/dev/null; }
+# Whether a run.json is a success: subtype success, is_error false, terminal_reason completed.
+is_success() { python3 -I -c "import json,sys; j=json.load(open(sys.argv[1])); sys.exit(0 if j.get('subtype')=='success' and j.get('is_error') is False and j.get('terminal_reason')=='completed' else 1)" "$1/run.json" 2>/dev/null; }
 # Whether a file naming session id $1 stands under the CLI's temporary folder outside the area.
 left_outside() { [ -n "$1" ] && [ -d "$CLI_TMP" ] && [ -n "$(find "$CLI_TMP" -maxdepth 4 -name "*$1*" 2>/dev/null | head -1)" ]; }
 
-# THE FREE PREFLIGHT (above): a key the API refuses, so no billed call; its folder holds a copy of the
-# session brief alone, and it runs once (a preflight/run.json already there is not run again).
+# THE FREE PREFLIGHT (above): a key the API refuses, so no billed call; its folder holds a copy of the session
+# brief alone, and it runs once (a preflight/run.json already there is not run again).
 if [ ! -f "$HERE/preflight/run.json" ]; then
     mkdir -p "$HERE/preflight"
     cp tools/brief-session.md "$HERE/preflight/brief.md"
@@ -146,6 +188,10 @@
     *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
     *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
 esac
+if [ "$PREFLIGHT_ONLY" = 1 ]; then
+    log "preflight only: stopped before the key is read; no paid session run"
+    exit 0
+fi
 
 # The key from .env, its length alone printed, never its value.
 set -a
@@ -154,7 +200,9 @@
 echo "ANTHROPIC_API_KEY length ${#ANTHROPIC_API_KEY}"
 [ "${#ANTHROPIC_API_KEY}" -gt 0 ] || { echo "no key: nothing run"; exit 2; }
 
-while read -r folder; do
+STOPPED=0
+passed_registered=0
+while read -r folder <&3; do
     [ -n "$folder" ] || continue
     if [ "$folder" = verdict ]; then d="$HERE/verdict"; cap=$CAP_VERDICT; else d="$HERE/s/$folder"; cap=$CAP; fi
     if [ -f "$d/run.json" ]; then
@@ -164,8 +212,20 @@
     spent=$(python3 -I tools/spent.py)
     if ! python3 -I -c "import sys; sys.exit(0 if $spent + $cap + $MARGIN <= $CEILING else 1)"; then
         log "stopped before $folder: spent $spent, and $cap + $MARGIN more would pass $CEILING"
+        STOPPED=3
         break
     fi
+    if [ "$passed_registered" = 0 ] && python3 -I -c "import sys; sys.exit(0 if $spent > $REGISTERED_CEILING else 1)"; then
+        log "note: spent $spent passes the registered ceiling $REGISTERED_CEILING (stop at $CEILING)"
+        passed_registered=1
+    fi
+    if [ "$folder" = verdict ] && [ ! -d "$d" ]; then
+        python3 -I tools/make_verdict.py 2>&1 | tee -a "$HERE/run.log"
+        [ -d "$d" ] || { log "verdict/ not built: runner stopped"; STOPPED=3; break; }
+    fi
+    if [ "$folder" = verdict ]; then
+        python3 -I tools/check_folders.py --verdict || { log "verdict/ is not what was built: runner stopped"; STOPPED=3; break; }
+    fi
     log "$folder (spent $spent, cap $cap)"
     session "$d" "$cap" "$ANTHROPIC_API_KEY"
     status=$?
@@ -174,8 +234,15 @@
     sid=$(session_id "$d")
     if left_outside "$sid"; then
         log "$folder left a file naming its session under $CLI_TMP, outside the repository's root: runner stopped"
+        STOPPED=3
         break
     fi
-done < order.txt
+    if ! is_success "$d"; then
+        log "$folder is not a success (exit $status): runner stopped; its run.json is kept and its cost counted"
+        STOPPED=3
+        break
+    fi
+done 3< order.txt
 log "spent $(python3 -I tools/spent.py) USD in all"
 python3 -I tools/scan.py home/.claude/projects | tee -a "$HERE/run.log"
+exit $STOPPED
```

## diff of run.sh, against panel 259's

```diff
--- 259/run.sh
+++ 260/run.sh
@@ -1,16 +1,17 @@
 #!/bin/bash
-# Panel 259's blind seat (panel 256's runner as panel 257 adapted it, adapted again for panel 259: see
-# tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
+# Panel 260's blind seat (panel 256's runner as panel 257 adapted it and panel 259 used it, adapted again for panel
+# 260: see tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
 # time, never two, each from its own folder: the 30 graded ones capped at 0.60 USD each, the verdict at 1.20, and
 # stops before the sum plus the next session's cap plus a 0.10 margin would pass 20.00 USD, the author's ceiling for
-# this run (2026-11-25 09:25, recorded in docs/panel/259-briefs/llm-ergonomist.md; registration.md's cap section).
+# this run (the answer *ok a*, relayed at 10:37 on 2026-11-25, recorded in docs/panel/260-briefs/llm-ergonomist.md;
+# registration.md's cap section).
 # A folder already holding run.json is skipped and its cost counted, so a stopped run resumes.
 #
 # NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
 # finding 4). This script refuses to start unless $WARDEN holds the heading
 #   ## Predictions registered before the blind run
 # with at least one non-empty line under it before the next `## ` heading, and that section's sha256 is the one
-# pinned below (the section as it stood when this area was prepared, 2026-11-25); it COPIES the report, as it
+# pinned below (the section as it stood when this area was prepared, 2026-11-26); it COPIES the report, as it
 # stands when the runner starts, to warden-at-start.md beside run.log (a later start keeps the first copy and writes
 # warden-at-resume-<time>.md), and logs the copy's sha256 (panel 253's critic, finding 6).
 #
@@ -35,8 +36,8 @@
 # skipped. A session still running after $SESSION_LIMIT seconds is killed by its pid (never by name) and counts as
 # a stop. SIGINT and SIGTERM to the runner kill the session in flight and remove the lock.
 #
-# Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation:
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-llm-ergonomist/run.sh
+# Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation (panel 260):
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-llm-ergonomist/run.sh
 set -u
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
@@ -54,11 +55,11 @@
 REGISTERED_CEILING=20.00
 SESSION_LIMIT=900
 ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
-WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/259-reports/spec-warden.md
-WARDEN_SECTION_SHA=5be9f7b3c83152f3eb5fd0089da5962fcbde959a97b1d6cf9a5020bb0f2fa82a
+WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/260-reports/spec-warden.md
+WARDEN_SECTION_SHA=818747c952c3fddf1966862747e0549d245bd9e2fa2b73b91ff47755f77dd7ea
 # The coordinator's registration (the arms' hashes, the cap and the predictions), pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=4d68580993c98f9c2d1e1396632a0a6e52e12a1140286884eee2384a86b0bc44
+REGISTRATION_SHA=369e4fa1e2b4a70e892c172cae9d10b4c704a0fa334ccd918f3c6647d3e9e672
 HEADING='## Predictions registered before the blind run'
 CLI_TMP="/private/tmp/claude-$(id -u)"
 REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
@@ -106,6 +107,15 @@
 [ -f "$HERE/registration-at-start.md" ] || cp "$REGISTRATION" "$HERE/registration-at-start.md"
 log "registration $REGISTRATION_SHA checked and copied"
 
+# The session brief third: panel 257's bytes with the two lines blind/registration.md § Every session's folder rewrites
+# and no other (registration-runner.md shows its diff); a brief that differs refuses the start.
+BRIEF_SHA=ca8e20776dfdb6253ccf101e30f9e829ec2401d3d184ba2d95c6d389bd058beb
+if [ "$(shasum -a 256 tools/brief-session.md 2>/dev/null | cut -c1-64)" != "$BRIEF_SHA" ]; then
+    echo "tools/brief-session.md is not the registered brief (run.sh pins $BRIEF_SHA): nothing run"
+    exit 2
+fi
+log "brief-session.md $BRIEF_SHA checked"
+
 # Before any spend: the order, the key, every folder not yet run holding its registered files alone, and the repairs.
 [ -f order.txt ] && [ -f key.txt ] || { echo "order.txt or key.txt missing: nothing run"; exit 2; }
 mkdir -p "$HERE/tmp"
```

## diff of preflight.sh, against panel 256's bytes

```diff
--- 256/preflight.sh
+++ 260/preflight.sh
@@ -1,153 +1,115 @@
 #!/bin/bash
-# PREFLIGHT ONLY: run.sh cut before the line that reads the key, so no billed session can run from it
-# (written 2026-11-22 by panel 256's coordinator, as panel 254's was). Below, run.sh as it stands:
-# Panel 256's blind seat (panel 254's runner, its caps, paths and gates changed by asserted edits): runs the
-# sessions order.txt names, one at a time, never two, each from its own folder, every graded session capped
-# at 1.00 USD and the verdict at 1.50, and stops before the sum would pass 19.50 USD, under the author's
-# 20 USD for this seat (2026-11-22, relayed by the coordinator of M-rebel-library). A folder already
-# holding run.json is skipped and its cost counted, so a stopped run resumes.
-#
-# NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
-# finding 4: that sitting's were written 13 seconds after its runner started). This script refuses to
-# start unless docs/panel/256-reports/spec-warden.md, in the lane-p256 worktree, holds the heading
-#   ## Predictions registered before the blind run
-# with at least one non-empty line under it before the next `## ` heading; it COPIES that report, as it
-# stands when the runner starts, to warden-at-start.md beside run.log (a later start keeps the first copy
-# and writes warden-at-resume-<time>.md), and logs the copy's sha256 (panel 253's critic, finding 6).
-#
-# Panel 251's runner leaked three ways (its critic's finding 2, defect 2589), and panel 253's critic found
-# a fourth way out (its finding 5: panel 251's session t4-f-2 left cache-break-state-<its id>.json in
-# /private/tmp/claude-501/). Every session here takes:
-#   - standard input from /dev/null, so no session reads order.txt from the loop;
-#   - the area as the top of an empty git repository of its own (.gitignore `*`, user reader
-#     <reader@invalid>, no commit), so the status the CLI renders names no commit and no person;
-#   - HOME as the area's home/, so the CLI's transcripts land there, never under ~/.claude/projects/;
-#   - TMPDIR and CLAUDE_CODE_TMPDIR as the area's tmp/. WHETHER THESE TWO VARIABLES MOVE THE CLI'S
-#     cache-break-state FILE INTO THE AREA IS UNRUN: no session was run to see it. So the free preflight
-#     below lists /private/tmp/claude-<uid>/ before and after a session that makes no billed call, and
-#     after every session the runner looks there for a file naming that session's id, stopping at the first.
-#
-# THE FREE PREFLIGHT: one session in preflight/, with the real prompt and every flag of a graded session,
-# whose key is a string the API refuses (the coordinator's measurement of 2026-11-19, coord/blindtest/,
-# made its probes so, each run.json `total_cost_usd: 0`). It must cost 0, leave no file naming its id
-# under /private/tmp/claude-<uid>/, and its transcript must read clean by tools/scan.py (the real prompt
-# alone, a git status naming `reader` and no commit, no CLAUDE.md, no memory); the runner stops otherwise.
-# It is also the scan's first negative control on a transcript holding the real prompt.
-#
-# Panel 253's runner written 2026-11-19 by its brief writer; adapted for panel 254 and again for panel 256 on
-# 2026-11-22 by their coordinators (panel 256: the caps, the ceiling, the warden's path, the registration's
-# gate). Started by the coordinator of M-rebel-library alone (RUN.md):
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/run.sh
+# Panel 260's preflight (panel 257's as panel 259 changed it, its arm check and pins changed for panel 260): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
+#   bash preflight.sh            the static checks alone: no `claude -p`, no API call, no key read except its LENGTH
+#   bash preflight.sh --session  the static checks, then `bash run.sh --preflight-only`: the one free session of
+#                                run.sh (panel 256's), whose key is a string the API refuses (cost 0 measured at
+#                                panel 256: run.json total_cost_usd 0, terminal_reason api_error), its transcript
+#                                then read by tools/scan.py; the real key is never read in that mode.
+# Prints one line per check, `ok` or `FAIL`, then a dry listing of what run.sh would run (folders, caps, file counts:
+# NEVER an arm, and never a byte count, which would tell one), and exits 0 only if every check is ok. Reads no file outside the area but the .env's key LENGTH
+# (never its value), the warden's report and, when present, the repository's own copies of the registered inputs.
 set -u
+MODE="${1:-}"
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
+fail=0
+ok() { echo "ok    $*"; }
+no() { echo "FAIL  $*"; fail=1; }
+chk() { if eval "$2" >/dev/null 2>&1; then ok "$1"; else no "$1"; fi; }
+const() { grep -E "^$1=" run.sh | head -1 | cut -d= -f2-; }
+sha() { shasum -a 256 "$1" | cut -c1-64; }
 
-CAP=1.00
-CAP_VERDICT=1.50
-MARGIN=0.10
-CEILING=19.50
-ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
-WARDEN=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/spec-warden.md
-# The coordinator's registration (the predictions and the arms' hashes), its sha256 written into RUN.md and here.
-REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92
-HEADING='## Predictions registered before the blind run'
-CLI_TMP="/private/tmp/claude-$(id -u)"
-REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
-PROMPT="Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English."
-log() { echo "$(date '+%Y-%m-%d %H:%M:%S') $*" | tee -a "$HERE/run.log"; }
+chk "run.sh parses (bash -n)" 'bash -n run.sh'
+chk "no run.lock stands" '[ ! -e run.lock ]'
+CAP=$(const CAP); CAPV=$(const CAP_VERDICT); MARGIN=$(const MARGIN); CEILING=$(const CEILING)
+echo "      constants of run.sh: cap $CAP, verdict cap $CAPV, margin $MARGIN, ceiling $CEILING (registered ceiling $(const REGISTERED_CEILING), the author's), per-session limit $(const SESSION_LIMIT) s"
+chk "caps are the registered 0.60 and 1.20, margin 0.10" '[ "$CAP" = 0.60 ] && [ "$CAPV" = 1.20 ] && [ "$MARGIN" = 0.10 ]'
+chk "ceiling is 20.00, the author's for the whole run" '[ "$CEILING" = 20.00 ]'
+WORST=$(python3 -I -c "print(f'{30*$CAP + $CAPV:.2f}')")
+echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 6 to 8, an inference, unrun"
+chk "the worst case stays under the ceiling, margin included (every session can run)" "python3 -I -c 'import sys; sys.exit(0 if $WORST + $MARGIN <= $CEILING else 1)'"
 
-# One runner at a time.
-if ! mkdir "$HERE/run.lock" 2>/dev/null; then
-    echo "another runner holds $HERE/run.lock: nothing run"
-    exit 2
-fi
-trap 'rmdir "$HERE/run.lock"' EXIT
+# the runner's own text: every flag of the registered command, no tool beyond Read and Write
+for f in '--bare' '--restricted' '--safe-mode' '--strict-mcp-config' '--model claude-opus-5-5' '--tools "Read,Write"' '--allowedTools "Read,Write"' \
+         '--max-budget-usd "$2"' '--output-format json' '< /dev/null' 'HOME="$HERE/home"' \
+         'TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp"' 'GIT_CEILING_DIRECTORIES=' 'env -i'; do
+    chk "run.sh's session command has: $f" 'grep -qF -- "$f" run.sh'
+done
+chk "--disallowedTools names Bash, WebFetch, WebSearch, Glob, Grep, Edit, Task, Agent, ListAgents, SendMessage" \
+    'grep -qF -- "--disallowedTools \"Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage\"" run.sh'
+chk "no pkill, no killall in run.sh" '! grep -nE "^[^#]*(pkill|killall)" run.sh'
 
-# The spec-warden's predictions first: the heading, a non-empty line under it, the report copied and hashed.
-if ! grep -q -x "$HEADING" "$WARDEN" 2>/dev/null; then
-    echo "the spec-warden's report holds no registered predictions yet ($WARDEN): nothing run"
-    exit 2
-fi
-if ! awk -v h="$HEADING" '$0 == h { on = 1; next } on && /^## / { exit } on && /[^[:space:]]/ { found = 1 } END { exit found ? 0 : 1 }' "$WARDEN"; then
-    echo "the heading stands in $WARDEN with nothing under it: nothing run"
-    exit 2
-fi
-if [ -f "$HERE/warden-at-start.md" ]; then
-    copy="$HERE/warden-at-resume-$(date '+%Y%m%d-%H%M%S').md"
-else
-    copy="$HERE/warden-at-start.md"
-fi
-cp "$WARDEN" "$copy" || { echo "could not copy $WARDEN: nothing run"; exit 2; }
-log "spec-warden report copied to $(basename "$copy"), sha256 $(shasum -a 256 "$copy" | cut -c1-64)"
+# the registered inputs, by hash: the arms, the tasks, the programs, and the registration itself
+while read -r f h; do
+    [ -n "$f" ] || continue
+    chk "$f is the registered file ($(echo "$h" | cut -c1-12))" '[ "$(sha "$f")" = "$h" ]'
+done < tools/inputs.sha256
+REG_SHA=$(grep -E '^REGISTRATION_SHA=' run.sh | cut -d= -f2)
+chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
+chk "every arm's full sha256 stands in the registration's own table" \
+    'for a in now adopted rival; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
+for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 10c2a8f93476d39b' 'brief-verdict.md 52d37bb04ac8ab20'; do
+    tf=${pair% *}; tp=${pair#* }
+    chk "tools/$tf is panel 256's registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
+done
+chk "the arms are the frozen spec with each registered diff applied by patch -p1 (adopted: the synthesis's R6 text and production; rival: w1)" \
+    'for a in adopted rival; do rm -rf tmp/arm-check && mkdir -p tmp/arm-check/spec && cp arms/spec-now.md tmp/arm-check/spec/heroes-spec.md && (cd tmp/arm-check && patch -p1 --quiet < ../../tools/synthesis-$a.diff) && cmp -s tmp/arm-check/spec/heroes-spec.md arms/spec-$a.md || exit 1; done; rm -rf tmp/arm-check'
+chk "tools/registration-runner.md (the runner's diffs against panel 256's bytes) exists" '[ -s tools/registration-runner.md ]'
 
-# The coordinator's registration second: its hash the one RUN.md names, copied beside run.log.
-if [ "$(shasum -a 256 "$REGISTRATION" 2>/dev/null | cut -c1-64)" != "$REGISTRATION_SHA" ]; then
-    echo "tools/registration.md is not the registered file (RUN.md names $REGISTRATION_SHA): nothing run"
-    exit 2
-fi
-[ -f "$HERE/registration-at-start.md" ] || cp "$REGISTRATION" "$HERE/registration-at-start.md"
-log "registration $REGISTRATION_SHA checked and copied"
+# the warden's registered predictions, pinned
+W=$(grep -E '^WARDEN=' run.sh | cut -d= -f2); WS=$(grep -E '^WARDEN_SECTION_SHA=' run.sh | cut -d= -f2)
+chk "the warden's report stands, with its heading and a non-empty section" 'grep -q -x "## Predictions registered before the blind run" "$W"'
+chk "the warden's section is the one run.sh pins" '[ "$(awk "\$0 == \"## Predictions registered before the blind run\" { on = 1; print; next } on && /^## / { exit } on { print }" "$W" | shasum -a 256 | cut -c1-64)" = "$WS" ]'
 
-# Before any spend: the order, the key, every folder not yet run holding its registered files alone,
-# and the repairs.
-[ -f order.txt ] && [ -f key.txt ] || { echo "order.txt or key.txt missing: nothing run"; exit 2; }
-mkdir -p "$HERE/tmp"
-python3 -I tools/check_folders.py || { echo "nothing run"; exit 2; }
-python3 -I tools/check_area.py || { echo "nothing run"; exit 2; }
+# the folders, the order, the area
+chk "order.txt and key.txt stand" '[ -s order.txt ] && [ -s key.txt ]'
+chk "check_folders: 31 in order, 30 folders at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
+chk "check_area: the area is the top of its own empty git repository, reader <reader@invalid>, HOME and TMPDIR inside" 'python3 -I tools/check_area.py'
+chk "git log in the area says no commit" 'git log 2>&1 | grep -q "does not have any commits yet"'
+chk "no CLAUDE.md, AGENTS.md, MEMORY.md or memory folder in the area (git's own folder left out)" \
+    '[ -z "$(find . -path ./.git -prune -o \( -name CLAUDE.md -o -name AGENTS.md -o -name MEMORY.md -o -name CLAUDE.local.md -o -type d -name memory \) -print | head -1)" ]'
+chk "no CLAUDE.md or memory under home/ (the CLI's own home for the sessions)" \
+    '[ -z "$(find home \( -name CLAUDE.md -o -name MEMORY.md -o -type d -name memory \) -print 2>/dev/null | head -1)" ]'
+echo "      spent so far, by tools/spent.py over the run.json files: $(python3 -I tools/spent.py) USD"
+chk "the ceiling still leaves room for the next session" "python3 -I -c 'import sys; sys.exit(0 if $(python3 -I tools/spent.py) + $CAP + $MARGIN <= $CEILING else 1)'"
+echo "      CLAUDE.md or AGENTS.md ABOVE the area (they cannot be absent: the area stands inside the repository's root):"
+d=$(dirname "$HERE"); while [ "$d" != / ]; do for f in CLAUDE.md AGENTS.md; do [ -e "$d/$f" ] && echo "        $d/$f"; done; d=$(dirname "$d"); done
+echo "      what keeps them out is --bare (no CLAUDE.md discovery), the area being the top of its own repository, HOME inside the area; the PROOF is the free preflight's scan: claude-md-or-memory=no"
 
-# The machine held awake for the runner's life (.claude/rules/verification.md § A long run holds the
-# machine awake); a second assertion costs nothing where Amphetamine already holds one.
-caffeinate -i -w $$ &
-claude --version | tee -a "$HERE/run.log"
+# the tools: the CLI, its flags, the key's length
+chk "claude is on PATH ($(claude --version 2>&1 | head -1))" 'command -v claude'
+for f in --bare --restricted --safe-mode --strict-mcp-config --max-budget-usd --tools --allowedTools --disallowedTools --output-format --model; do
+    chk "claude --help lists $f" 'claude --help 2>&1 | grep -q -- "$f"'
+done
+chk "the .env stands and holds a non-empty ANTHROPIC_API_KEY (its length alone is read, in a subshell)" \
+    'n=$( ( . "$(grep -E "^ENV_FILE=" run.sh | cut -d= -f2)"; echo ${#ANTHROPIC_API_KEY} ) ); [ "$n" -gt 0 ]'
 
-# One session from folder $1 with cap $2 and key $3; its exit status is the CLI's.
-session() {
-    (
-        cd "$1" || exit 2
-        env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
-            TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp" \
-            GIT_CEILING_DIRECTORIES="$(dirname "$HERE")" ANTHROPIC_API_KEY="$3" \
-          claude -p "$PROMPT" \
-            --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
-            --tools "Read,Write" --allowedTools "Read,Write" \
-            --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
-            --max-budget-usd "$2" --output-format json < /dev/null > run.json 2> run.err
-    )
-}
-# The session id a run.json names, or nothing.
-session_id() { python3 -I -c "import json,sys; print(json.load(open(sys.argv[1])).get('session_id',''))" "$1/run.json" 2>/dev/null; }
-# Whether a file naming session id $1 stands under the CLI's temporary folder outside the area.
-left_outside() { [ -n "$1" ] && [ -d "$CLI_TMP" ] && [ -n "$(find "$CLI_TMP" -maxdepth 4 -name "*$1*" 2>/dev/null | head -1)" ]; }
+echo
+echo "dry listing of what run.sh would run, in order (a folder already holding run.json is skipped; no arm is shown):"
+n=0
+while read -r name; do
+    [ -n "$name" ] || continue
+    n=$((n + 1))
+    if [ "$name" = verdict ]; then
+        d=verdict; cap=$CAPV
+        [ -d verdict ] && extra="" || extra=" (built after the 30 by tools/make_verdict.py, reports of the successful sessions only)"
+    else
+        d="s/$name"; cap=$CAP; extra=""
+    fi
+    if [ -f "$d/run.json" ]; then st="already run"; else st="to run"; fi
+    if [ -d "$d" ]; then fc=$(find "$d" -type f | wc -l | tr -d ' '); else fc=0; fi
+    printf '  %2d  %-9s cap %s  %-11s %2s files%s\n' "$n" "$name" "$cap" "$st" "$fc" "$extra"
+done < order.txt
+echo "  the runner stops before any session where spent + its cap + $MARGIN would pass $CEILING USD, stops at the first session that is not a success, and logs to run.log"
 
-# THE FREE PREFLIGHT (above): a key the API refuses, so no billed call; its folder holds a copy of the
-# session brief alone, and it runs once (a preflight/run.json already there is not run again).
-if [ ! -f "$HERE/preflight/run.json" ]; then
-    mkdir -p "$HERE/preflight"
-    cp tools/brief-session.md "$HERE/preflight/brief.md"
-    ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-before.txt" 2>&1
-    session "$HERE/preflight" "$CAP" "$REFUSED_KEY"
-    ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-after.txt" 2>&1
-    diff "$HERE/preflight/cli-tmp-before.txt" "$HERE/preflight/cli-tmp-after.txt" > "$HERE/preflight/cli-tmp-diff.txt"
-    log "preflight: $CLI_TMP listing before and after differ in $(grep -c '^[<>]' "$HERE/preflight/cli-tmp-diff.txt") lines (other sessions on this Mac write there too; preflight/cli-tmp-diff.txt)"
+if [ "$MODE" = --session ]; then
+    echo
+    if [ "$fail" != 0 ]; then echo "a static check failed: the free session is not run"; exit 2; fi
+    echo "--session: the free preflight session (a key the API refuses, cost 0 expected), by run.sh --preflight-only"
+    bash run.sh --preflight-only; rc=$?
+    echo "run.sh --preflight-only exit $rc; run.log's last lines say the cost, the session id and the scan"
+    exit $rc
 fi
-pre_cost=$(python3 -I -c "import json,sys; print(json.load(open(sys.argv[1])).get('total_cost_usd', 'unreadable'))" "$HERE/preflight/run.json" 2>/dev/null)
-pre_id=$(session_id "$HERE/preflight")
-log "preflight: cost $pre_cost, session $pre_id"
-if [ "$pre_cost" != "0" ] && [ "$pre_cost" != "0.0" ]; then
-    echo "the preflight cost $pre_cost, not 0: nothing more run"
-    exit 2
-fi
-if left_outside "$pre_id"; then
-    log "preflight: a file naming its session stands under $CLI_TMP: the variables do not keep the CLI inside the area; nothing more run"
-    exit 2
-fi
-pre_scan=$(python3 -I tools/scan.py home/.claude/projects | grep -- '-preflight ')
-log "preflight scan: ${pre_scan:-no transcript found}"
-case "$pre_scan" in
-    *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
-    *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
-esac
-
-log "preflight only: stopped before the key is read; no paid session run"
-exit 0
+echo
+if [ "$fail" = 0 ]; then echo "preflight (static): every check ok; nothing was spent"; else echo "preflight (static): FAILED, see the FAIL lines; nothing was spent"; fi
+exit $fail
```

## diff of preflight.sh, against panel 259's

```diff
--- 259/preflight.sh
+++ 260/preflight.sh
@@ -1,5 +1,5 @@
 #!/bin/bash
-# Panel 259's preflight (panel 257's, its constants, arms and diffs changed for panel 259): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
+# Panel 260's preflight (panel 257's as panel 259 changed it, its arm check and pins changed for panel 260): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
 #   bash preflight.sh            the static checks alone: no `claude -p`, no API call, no key read except its LENGTH
 #   bash preflight.sh --session  the static checks, then `bash run.sh --preflight-only`: the one free session of
 #                                run.sh (panel 256's), whose key is a string the API refuses (cost 0 measured at
@@ -26,7 +26,7 @@
 chk "caps are the registered 0.60 and 1.20, margin 0.10" '[ "$CAP" = 0.60 ] && [ "$CAPV" = 1.20 ] && [ "$MARGIN" = 0.10 ]'
 chk "ceiling is 20.00, the author's for the whole run" '[ "$CEILING" = 20.00 ]'
 WORST=$(python3 -I -c "print(f'{30*$CAP + $CAPV:.2f}')")
-echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate 6.3 to 8.5, an inference, unrun"
+echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 6 to 8, an inference, unrun"
 chk "the worst case stays under the ceiling, margin included (every session can run)" "python3 -I -c 'import sys; sys.exit(0 if $WORST + $MARGIN <= $CEILING else 1)'"
 
 # the runner's own text: every flag of the registered command, no tool beyond Read and Write
@@ -52,16 +52,8 @@
     tf=${pair% *}; tp=${pair#* }
     chk "tools/$tf is panel 256's registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
 done
-chk "the arms are the frozen spec plus the registered one-line diffs (adopted: the synthesis's sentence, rival: w1 before it)" 'python3 -I - <<E
-import hashlib
-now = open("arms/spec-now.md").read().split("\n")
-def apply(diff):
-    plus = [l[1:] for l in open(diff).read().split("\n") if l.startswith("+") and not l.startswith("+++")]
-    assert len(plus) == 1 and now[427].startswith("and \`()\` is \`void\`")
-    return "\n".join(now[:427] + plus + now[428:])
-for arm, diff in (("adopted", "tools/synthesis-adopted.diff"), ("rival", "tools/synthesis-rival.diff")):
-    assert hashlib.sha256(apply(diff).encode()).hexdigest() == hashlib.sha256(open("arms/spec-%s.md" % arm, "rb").read()).hexdigest(), arm
-E'
+chk "the arms are the frozen spec with each registered diff applied by patch -p1 (adopted: the synthesis's R6 text and production; rival: w1)" \
+    'for a in adopted rival; do rm -rf tmp/arm-check && mkdir -p tmp/arm-check/spec && cp arms/spec-now.md tmp/arm-check/spec/heroes-spec.md && (cd tmp/arm-check && patch -p1 --quiet < ../../tools/synthesis-$a.diff) && cmp -s tmp/arm-check/spec/heroes-spec.md arms/spec-$a.md || exit 1; done; rm -rf tmp/arm-check'
 chk "tools/registration-runner.md (the runner's diffs against panel 256's bytes) exists" '[ -s tools/registration-runner.md ]'
 
 # the warden's registered predictions, pinned
```

## diff of check_folders.py, against panel 256's bytes

```diff
--- 256/check_folders.py
+++ 260/check_folders.py
@@ -1,28 +1,71 @@
-# Before any spend (panel 256's, from panel 254's): order.txt names 46 folders, each in key.txt, and every folder not yet run holds
-# exactly its registered files at their registered hashes. usage: python3 -I tools/check_folders.py
+# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used unchanged in code by panels 259 and 260): order.txt names 31 folders (the 30 graded
+# ones, 5 per task and arm, then `verdict` last), each in key.txt; every folder not yet run holds exactly its
+# registered files, nested ones included (`programs/`), at their registered hashes, and each of those hashes is
+# the one tools/inputs.sha256 registers (the arm's spec, the task, the programs) or tools/brief-session.md's;
+# no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
+# before then (`--verdict` asks only of it, after tools/make_verdict.py).
+# usage: python3 -I tools/check_folders.py [--verdict]
 import hashlib, os, sys
+ARMS = ('now', 'adopted', 'rival')
+def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()
+reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
 key = {}
 for line in open('key.txt'):
     if line.startswith('#') or not line.strip():
         continue
     name, arm, *files = line.split()
-    key[name] = dict(f.split('=', 1) for f in files)
+    key[name] = (arm, dict(f.split('=', 1) for f in files))
 order = [l.strip() for l in open('order.txt') if l.strip()]
 bad = []
-if len(order) != 46 or len(set(order)) != 46:
-    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, not 46')
+if len(order) != 31 or len(set(order)) != 31 or order[-1] != 'verdict':
+    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, the last {order[-1:]}: not 31 with verdict last')
+cells = {}
+for name in order[:-1]:
+    if name not in key:
+        continue
+    task = name.split('-')[0]
+    cells[(task, key[name][0])] = cells.get((task, key[name][0]), 0) + 1
+want = {(t, a): 5 for t in ('t1', 't2') for a in ARMS}
+if cells != want:
+    bad.append(f'the cells are not 5 sessions of each task and arm: {sorted(cells.items())}')
+brief_sha = sha('tools/brief-session.md')
+def registered(name, arm, f):
+    """The hash a file of a graded session's folder must have, from the registration's inputs."""
+    task = name.split('-')[0]
+    if f == 'spec.md': return reg[f'arms/spec-{arm}.md']
+    if f == 'task.md': return reg[f'tasks/task-{task}.md']
+    if f == 'brief.md': return brief_sha
+    if f.startswith('programs/'): return reg[f]
+    return None
+def listing(d):
+    return sorted(os.path.relpath(os.path.join(r, x), d) for r, _, fs in os.walk(d) for x in fs)
+only_verdict = '--verdict' in sys.argv
 for name in order:
+    if only_verdict != (name == 'verdict'):
+        continue
     d = 'verdict' if name == 'verdict' else os.path.join('s', name)
+    if name == 'verdict' and not os.path.isdir(d):
+        continue  # built after the 30
     if name not in key:
         bad.append(f'{name} not in key.txt'); continue
     if os.path.exists(os.path.join(d, 'run.json')):
         continue
-    have = sorted(os.listdir(d)) if os.path.isdir(d) else []
-    if have != sorted(key[name]):
+    arm, files = key[name]
+    if name != 'verdict' and arm not in ARMS:
+        bad.append(f'{name} names the arm {arm!r}'); continue
+    have = listing(d)
+    if have != sorted(files):
         bad.append(f'{name} holds other files: {have}'); continue
-    for f, h in key[name].items():
-        if hashlib.sha256(open(os.path.join(d, f), 'rb').read()).hexdigest() != h:
+    for f, h in files.items():
+        if sha(os.path.join(d, f)) != h:
             bad.append(f'{name}/{f} is not the registered file')
+        r = registered(name, arm, f) if name != 'verdict' else None
+        if name != 'verdict' and r != h:
+            bad.append(f'{name}/{f} is registered as another hash than key.txt holds')
+if not only_verdict and os.path.isdir('s'):
+    for x in sorted(os.listdir('s')):
+        if x not in order:
+            bad.append(f's/{x} is not in order.txt')
 for b in bad:
     print(b)
 sys.exit(2 if bad else 0)
```

## diff of check_folders.py, against panel 259's

```diff
--- 259/check_folders.py
+++ 260/check_folders.py
@@ -1,4 +1,4 @@
-# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used unchanged in code by panel 259): order.txt names 31 folders (the 30 graded
+# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used unchanged in code by panels 259 and 260): order.txt names 31 folders (the 30 graded
 # ones, 5 per task and arm, then `verdict` last), each in key.txt; every folder not yet run holds exactly its
 # registered files, nested ones included (`programs/`), at their registered hashes, and each of those hashes is
 # the one tools/inputs.sha256 registers (the arm's spec, the task, the programs) or tools/brief-session.md's;
```

## diff of make_sessions.py, against panel 256's bytes

```diff
--- 256/make_sessions.py
+++ 260/make_sessions.py
@@ -1,58 +1,60 @@
-# Panel 256's blind seat: draws the labels, then makes one session folder per task, arm and replicate, each
-# holding brief.md, task.md and heroes-spec.md and nothing else, and the verdict session's folder. The key
-# and the order are written beside s/, never inside it. Refuses to run over an existing s/. Written
-# 2026-11-22 by panel 256's coordinator, on panel 254's make_sessions.py.
-# usage (from the blind area): python3 -I tools/make_sessions.py
-import difflib, hashlib, os, secrets, sys
+# Panel 260's blind seat (panel 256's make_sessions.py as panel 257 changed it and panel 259 used it; for 260 only the
+# programs each task's folder holds changed: t1 none, t2 the six program folders p1 to p6 under programs/, nested):
+# draws the arms' labels, then makes one session
+# folder per task, arm and replicate (t1 and t2, `now`, `adopted`, `rival`, 5 each: 30 folders), each holding
+# brief.md, task.md, spec.md and, for t2, programs/, and nothing else, and writes key.txt and order.txt beside s/, never
+# inside it. THE DRAW: the three labels are sampled from a pool of letters with panel 256's method,
+# secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place the
+# draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
+# the six cells in a fixed sequence, reversed on the even replicates, the verdict last.
+# The verdict's folder is NOT made here: it is built after the 30 by tools/make_verdict.py.
+# Refuses to run over an existing s/, verdict/, key.txt or order.txt.
+# usage (from the area): python3 -I tools/make_sessions.py
+import hashlib, os, secrets, sys
 assert os.path.isfile('tools/make_sessions.py'), 'run from the blind area'
-if os.path.exists('s') or os.path.exists('verdict') or os.path.exists('key.txt'):
-    sys.exit('s/, verdict/ or key.txt exists: nothing made')
+for p in ('s', 'verdict', 'key.txt', 'order.txt'):
+    if os.path.exists(p):
+        sys.exit(f'{p} exists: nothing made')
 REPLICATES = 5
-SPECS = {'now': 'arms/spec-now.md', 'gc': 'arms/arm-gc.md', 'gi': 'arms/arm-gi.md', 'gr': 'arms/arm-gr.md'}
-ARMS = ['now', 'gc', 'gi', 'gr', 'w', 'm']
-CELLS = [  # (task, arm, spec file, task file)
-    ('t1', 'w', 'now', 'task-t1-w.md'), ('t1', 'm', 'now', 'task-t1-m.md'),
-    ('t2', 'now', 'now', 'task-t2.md'), ('t2', 'gc', 'gc', 'task-t2.md'),
-    ('t2', 'gi', 'gi', 'task-t2.md'), ('t2', 'gr', 'gr', 'task-t2.md'),
-    ('t3', 'gc', 'gc', 'task-t3.md'), ('t3', 'gi', 'gi', 'task-t3.md'), ('t3', 'gr', 'gr', 'task-t3.md'),
-]
+ARMS = ['now', 'adopted', 'rival']
+SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
+TASKS = ['t1', 't2']
+T2_PROGRAMS = sorted(os.path.relpath(os.path.join(r, x), 'programs') for r, _, fs in os.walk('programs') for x in fs)
+assert len(T2_PROGRAMS) == 20, f'programs/ holds {len(T2_PROGRAMS)} files, not the 20 registered'
+PROGRAMS = {'t1': [], 't2': T2_PROGRAMS}
+# no letter that stands in an arm's name (now, adopted, rival), so a label says nothing of its arm
+POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted' + 'rival')]
 def sha(b): return hashlib.sha256(b).hexdigest()
 def read(p): return open(p, 'rb').read()
-reg = dict(l.split() for l in open('arms/arms.sha256'))  # the arms' hashes, registered before the draw
-for a, f in SPECS.items():
-    assert sha(read(f)) == reg[os.path.basename(f)], a
-pool = list('bdfhjkpqsvxyz')  # no letter of `now`, `gc`, `gi`, `gr`, `w`, `m`
-labels = secrets.SystemRandom().sample(pool, len(ARMS))
+reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
+for f, h in reg.items():
+    assert sha(read(f)) == h, f'{f} is not the registered file'
+labels = secrets.SystemRandom().sample(POOL, len(ARMS))
 draw = dict(zip(ARMS, labels))
-print('draw', ' '.join(f'{a}->{draw[a]}' for a in ARMS))
 brief = read('tools/brief-session.md')
 rows, cells = [], []
-for task, arm, spec, tfile in CELLS:
-    for n in range(1, REPLICATES + 1):
-        name = f'{task}-{draw[arm]}-{n}'
-        d = os.path.join('s', name)
-        os.makedirs(d)
-        files = {'brief.md': brief, 'task.md': read(f'tools/{tfile}'), 'heroes-spec.md': read(SPECS[spec])}
-        for f, b in files.items():
-            open(os.path.join(d, f), 'wb').write(b)
-        assert sorted(os.listdir(d)) == sorted(files)
-        rows.append(f'{name} {arm} ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(files.items())))
-    cells.append((task, draw[arm]))
-# the verdict session: the frozen spec and the resolution's arm, their diff, task t3; never graded
-os.makedirs('verdict')
-a_txt, b_txt = read(SPECS['now']), read(SPECS['gi'])
-diff = ''.join(difflib.unified_diff(a_txt.decode().splitlines(True), b_txt.decode().splitlines(True),
-                                    fromfile='spec-A.md', tofile='spec-B.md')).encode()
-vf = {'brief.md': read('tools/brief-verdict.md'), 'task.md': read('tools/task-t3.md'),
-      'spec-A.md': a_txt, 'spec-B.md': b_txt, 'changes.diff': diff}
-for f, b in vf.items():
-    open(os.path.join('verdict', f), 'wb').write(b)
-rows.append('verdict verdict ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(vf.items())))
+for task in TASKS:
+    for arm in ARMS:
+        cells.append((task, arm))
+        for n in range(1, REPLICATES + 1):
+            name = f'{task}-{draw[arm]}-{n}'
+            d = os.path.join('s', name)
+            files = {'brief.md': brief, 'task.md': read(f'tasks/task-{task}.md'), 'spec.md': read(SPEC[arm])}
+            for p in PROGRAMS[task]:
+                files[f'programs/{p}'] = read(f'programs/{p}')
+            for f, b in files.items():
+                os.makedirs(os.path.dirname(os.path.join(d, f)), exist_ok=True)
+                open(os.path.join(d, f), 'wb').write(b)
+            rows.append(f'{name} {arm} ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(files.items())))
 order = []
 for r in range(1, REPLICATES + 1):
     seq = cells if r % 2 == 1 else list(reversed(cells))
-    order += [f'{t}-{l}-{r}' for t, l in seq]
+    order += [f'{t}-{draw[a]}-{r}' for t, a in seq]
 order.append('verdict')
-open('key.txt', 'w').write('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) + '\n' + '\n'.join(rows) + '\n')
+head = ('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) +
+        '\n# drawn by tools/make_sessions.py: secrets.SystemRandom().sample over the letters standing in none of the arms\' names; '
+        'the verdict row is appended by tools/make_verdict.py after the 30 sessions\n')
+open('key.txt', 'w').write(head + '\n'.join(rows) + '\n')
 open('order.txt', 'w').write('\n'.join(order) + '\n')
-print(len(order), 'sessions in order.txt;', sum(len(os.listdir(os.path.join('s', x))) for x in os.listdir('s')), 'files under s/')
+print(len(order), 'sessions in order.txt (30 graded and the verdict);',
+      sum(len(files) for _, _, files in os.walk('s')), 'files under s/; the draw is written to key.txt alone')
```

## diff of make_sessions.py, against panel 259's

```diff
--- 259/make_sessions.py
+++ 260/make_sessions.py
@@ -1,7 +1,8 @@
-# Panel 259's blind seat (panel 256's make_sessions.py as panel 257 changed it; for 259 only the programs each task's
-# folder holds changed): draws the arms' labels, then makes one session
+# Panel 260's blind seat (panel 256's make_sessions.py as panel 257 changed it and panel 259 used it; for 260 only the
+# programs each task's folder holds changed: t1 none, t2 the six program folders p1 to p6 under programs/, nested):
+# draws the arms' labels, then makes one session
 # folder per task, arm and replicate (t1 and t2, `now`, `adopted`, `rival`, 5 each: 30 folders), each holding
-# brief.md, task.md, spec.md and programs/ and nothing else, and writes key.txt and order.txt beside s/, never
+# brief.md, task.md, spec.md and, for t2, programs/, and nothing else, and writes key.txt and order.txt beside s/, never
 # inside it. THE DRAW: the three labels are sampled from a pool of letters with panel 256's method,
 # secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place the
 # draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
@@ -18,7 +19,9 @@
 ARMS = ['now', 'adopted', 'rival']
 SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
 TASKS = ['t1', 't2']
-PROGRAMS = {'t1': ['co.h', 'sw.h', 'jump.h'] + [f'q{i}.hero' for i in range(1, 8)], 't2': ['co.h', 'sw.h']}
+T2_PROGRAMS = sorted(os.path.relpath(os.path.join(r, x), 'programs') for r, _, fs in os.walk('programs') for x in fs)
+assert len(T2_PROGRAMS) == 20, f'programs/ holds {len(T2_PROGRAMS)} files, not the 20 registered'
+PROGRAMS = {'t1': [], 't2': T2_PROGRAMS}
 # no letter that stands in an arm's name (now, adopted, rival), so a label says nothing of its arm
 POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted' + 'rival')]
 def sha(b): return hashlib.sha256(b).hexdigest()
```

## diff of make_verdict.py, against panel 259's (new in panel 259, not panel 256's)

```diff
--- 259/tools/make_verdict.py
+++ 260/tools/make_verdict.py
@@ -1,14 +1,10 @@
-# Panel 259's blind seat: builds the verdict session's folder after the 30 graded sessions, as the registration says
-# (docs/panel/259-briefs/llm-ergonomist.md § The rows: "its folder is built after the 30 and holds the two specs, a
-# diff and the reports, never a key"). Written 2026-11-25 by the blind run's session, before any session ran; panel
-# 257's own make_verdict.py is not in its archive, so this file is new, its shape panel 257's as its report describes
-# it (docs/panel/257-reports/llm-ergonomist-blind.md § The A/B verdict): spec-A.md is `now`, spec-B.md is `adopted`
-# (the resolution's text), changes.diff their unified diff, task.md t1 then t2, programs/ t1's programs, reports/ one
-# file per successful graded session named by its folder (its label, never its arm), report.md where the session wrote
-# one, else the result.md tools/after.py kept. brief.md is tools/brief-verdict.md (panel 256's registered bytes) with
-# tools/brief-verdict-reports.md appended, then the thirty report names listed, which panel 257's verdict lacked (it
-# could not list reports/ with Read alone and read none of them). Appends the verdict's row to key.txt.
-# Refuses to run over an existing verdict/, or before every graded folder holds a run.json.
+# Panel 260's blind seat: builds the verdict session's folder after the 30 graded sessions (panel 259's make_verdict.py,
+# its programs changed): spec-A.md is `now`, spec-B.md is `adopted` (the resolution's text), changes.diff their unified
+# diff, task.md t1 then t2, programs/ t2's six program folders (t1 has none), reports/ one file per successful graded
+# session named by its folder (its label, never its arm), report.md where the session wrote one, else the result.md
+# tools/after.py kept. brief.md is tools/brief-verdict.md (panel 256's registered bytes) with
+# tools/brief-verdict-reports.md appended (panel 259's bytes), then the report names listed. Appends the verdict's row
+# to key.txt. Refuses to run over an existing verdict/, or before every graded folder holds a run.json.
 # usage (from the area): python3 -I tools/make_verdict.py
 import difflib, hashlib, json, os, sys
 assert os.path.isfile('tools/make_verdict.py'), 'run from the blind area'
@@ -43,7 +39,7 @@
 names = '\n'.join(f'- `{r}`' for r in sorted(reports)).encode()
 brief = read('tools/brief-verdict.md') + b'\n' + read('tools/brief-verdict-reports.md') + b'\n' + names + b'\n'
 files = {'brief.md': brief, 'spec-A.md': a_txt, 'spec-B.md': b_txt, 'changes.diff': diff, 'task.md': task}
-for p in ('co.h', 'sw.h', 'jump.h') + tuple(f'q{i}.hero' for i in range(1, 8)):
+for p in sorted(os.path.relpath(os.path.join(r, x), 'programs') for r, _, fs in os.walk('programs') for x in fs):
     files[f'programs/{p}'] = read(f'programs/{p}')
 files.update(reports)
 for f, b in files.items():
```

## The hashes of the runner as registered

```
8119291170f0b671e8d722a7f9b452ff3ecbc44335d968c5903f1e663ed49642  run.sh
322004f04d64d2d0fc8e8dd752c6bd876593d0bf58f8247fcd8f6eb51ab13f6a  preflight.sh
e5b8bd741da31af51a2491adf2adc8e8e110133251f7b0f11580ad470ece48aa  tools/after.py
ca8e20776dfdb6253ccf101e30f9e829ec2401d3d184ba2d95c6d389bd058beb  tools/brief-session.md
641a1d0c4c60e92440a994f2111067e27ba0de99184d5cb56b31b522eebf92e6  tools/brief-verdict-reports.md
52d37bb04ac8ab20ae12458f1f7550a3ffaa818d5e48cec88fe90359f8835a36  tools/brief-verdict.md
083089c60775136c80fec0599033d250d93032a9a7b66c4d9ac0914bd530e169  tools/check_area.py
19a83d9e889cdb42962f68df64bc79dc7ddf42f1a558e3710d1f6c8330fd2429  tools/check_folders.py
5b100383c4f85e098fd0b883f0032cb285e57ed66e154006d51f22436fefc9e6  tools/inputs.sha256
b291c255ac2cdb5d1788b77bf7391118cc7343d748464b5bb68a92491b4fc10d  tools/make_sessions.py
09b39467e61fccb26e9250784d4c394d10294807b72941a98866366b9d677b7f  tools/make_verdict.py
d6fe89f5c79e6d938577e1352829788b4251fcc66c81bd5e896e924d095de63d  tools/read_runs.py
a1519e78f9154349e9d1c58183bfcf0f390fd3a6cba9cf795f472b2682711b1d  tools/reads.py
369e4fa1e2b4a70e892c172cae9d10b4c704a0fa334ccd918f3c6647d3e9e672  tools/registration.md
41aae8419d682a6d110d276998070924cb5fb259ed3993e234a3847e7a99d5c1  tools/scan.py
10c2a8f93476d39bbc60349099c23b4d1b15fbffb7e42de938a65673d7003ef0  tools/spent.py
78b15a71fb699447785a982246efcefae5a871b1b37be432527bd2105de4fc17  tools/synthesis-adopted.diff
af4718ccae74d423f9a733ecf767bbed21a9c20de237e47cd1bc7844ed6f83c8  tools/synthesis-rival.diff
```

## Amended 2026-11-26 after the network outage of about 14:25, before the rerun

The runner started at 14:15:53. `t1-q-1` counted (`success`, `is_error` false, `completed`, 0.209227 USD); `t1-f-1`
was cut by a network outage the coordinator reported at about 14:25 (back at 14:30): its `run.json` reads `subtype`
`success`, **`is_error` true, `terminal_reason` `api_error`**, 0.090017 USD, so it does not count, and the runner
stopped itself at 14:26:51 as it is written to (*"a session that is not a `success` ... stops the runner"*). A counted
session is never run again; a cut one is run again and its spend still counts toward the ceiling (the coordinator's
instruction, 14:30). So, before the rerun: the cut attempt's `run.json`, `run.err` and `result.md` moved from
`s/t1-f-1/` to `cut/t1-f-1-attempt1/` (its folder then holding its three registered files alone, which
`check_folders.py` asks again), and **`spent.py` sums `cut/*/run.json` too**, so the ceiling's arithmetic keeps every
cent spent (0.299244 USD read by it after the move, the same as before it). `preflight.sh` pins the new `spent.py`.
The cut attempt's answer is never graded and never reaches the verdict (`make_verdict.py` reads `s/` alone).

### diff of tools/spent.py, against panel 256's bytes

```diff
--- 256/tools/spent.py
+++ 260/tools/spent.py
@@ -1,8 +1,10 @@
 # The spend so far: the sum of total_cost_usd over every run.json under s/ and verdict/, one that cannot
 # be read counted as 0.50. usage: python3 -I tools/spent.py
+# Panel 260, amended 2026-11-26 after the network outage of about 14:25: also every run.json under cut/, the
+# attempts a cut stopped short of a success, moved there so their sessions run again, their spend still counted.
 import glob, json
 total = 0.0
-for p in glob.glob('s/*/run.json') + glob.glob('verdict/run.json'):
+for p in glob.glob('s/*/run.json') + glob.glob('verdict/run.json') + glob.glob('cut/*/run.json'):
     try:
         total += float(json.load(open(p)).get('total_cost_usd'))
     except Exception:
```

### diff of preflight.sh, against the bytes registered above

```diff
--- 260-registered/preflight.sh
+++ 260/preflight.sh
@@ -48,9 +48,9 @@
 chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
 chk "every arm's full sha256 stands in the registration's own table" \
     'for a in now adopted rival; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
-for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 10c2a8f93476d39b' 'brief-verdict.md 52d37bb04ac8ab20'; do
+for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 0d09532b49d7c848' 'brief-verdict.md 52d37bb04ac8ab20'; do
     tf=${pair% *}; tp=${pair#* }
-    chk "tools/$tf is panel 256's registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
+    chk "tools/$tf is the registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
 done
 chk "the arms are the frozen spec with each registered diff applied by patch -p1 (adopted: the synthesis's R6 text and production; rival: w1)" \
     'for a in adopted rival; do rm -rf tmp/arm-check && mkdir -p tmp/arm-check/spec && cp arms/spec-now.md tmp/arm-check/spec/heroes-spec.md && (cd tmp/arm-check && patch -p1 --quiet < ../../tools/synthesis-$a.diff) && cmp -s tmp/arm-check/spec/heroes-spec.md arms/spec-$a.md || exit 1; done; rm -rf tmp/arm-check'
```

### The hashes changed

```
166300f142ae86d2c2186a962f885de794010a08b970669f910b615d811a0a74  preflight.sh
0d09532b49d7c848cb8ad9945a327adc63cc5f758dfd02e946acf10bbfb6139b  tools/spent.py
```
