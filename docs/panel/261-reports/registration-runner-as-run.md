# Panel 261's blind runner: what changes against panel 256's, registered before any session

Written 2026-11-26 by the sitting's repair (from the critic's first reading, `docs/panel/261-reports/critic-briefs.md`
§ 4 item 7: the file was owed and did not exist), **before any session is drawn and before any session runs; none is
drawn and none runs**, as `blind/registration.md` § Every session's folder asks: the session brief's diff against panel
256's committed `brief-session.md`, whole, and what the run's session changes in the runner's other files. **The run
is the coordinator's**, after the synthesis, on the author's authorisation of 2026-11-26 (*ok a 20 dollars*): a
ceiling of **20.00 USD** for **31 sessions** (30 graded, t1, t2 and t3 under `now` and `adopted`, five each, and one
verdict), **0.60 USD** a graded session, **1.20 USD** the verdict, margin **0.10** (`llm-ergonomist.md` § The size, the
spend and the cap).

**The runner is panel 256's** (`docs/panel/256-briefs/blind/`, 23 entries by `ls`) **adapted as panels 257 and 259
adapted it.** The nearest model is panel 259's, `docs/panel/259-briefs/blind/registration-runner.md` (sha256
`6a7f993c287dc5217a5a968dfe8e3733804e4d5a5d54b9b58d24b568bffe6d98`, `shasum -a 256`, the repair), which holds a `diff -u`
of each file it changed against panel 256's committed bytes, so panel 259's runner can be rebuilt by applying those
diffs with `patch`, as panel 259 rebuilt panel 257's (unrun by the repair: the run's session rebuilds it and says
whether each applied clean). Panel 260 committed no runner (`ls docs/panel/260-briefs/blind/`:
`reference-t1`, `registration.md`, `task-t1.md`).

## What changes, in words

- **`brief-session.md`**: written by the repair, whole, as `blind/brief-session.md` (1,757 bytes, sha256
  `e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222`); its diff against panel 256's committed bytes is
  below. Panel 256's opening and method stand, a write task's (*"write the code, don't opine"*); the spec is named
  `spec.md` (panel 257's name); there is no `programs/` line, a write task holding none (as panel 260's t1); `answer`
  asks the program `main.hero` whole in one fenced block, then `prints`, `compiles` and `parameters`, word for word as
  `blind/registration.md` asks; panel 256's `prediction` field leaves, its content being `prints` and `compiles` under
  `answer`; `choice_points` and `context` are panel 256's words, which panel 257's and 259's keep unchanged.
- **Owed by the run's session, before any session, each with a `diff -u` against panel 256's committed bytes and
  against panel 259's rebuilt file, written below this list as panel 259's file holds them**:
  - **`make_sessions.py`**: `ARMS = ['now', 'adopted']` (two arms, no `rival`, `llm-ergonomist.md` § The arms);
    `TASKS = ['t1', 't2', 't3']`; **no `programs/`** in any folder, so each holds `brief.md`, `task.md` and `spec.md`
    and nothing else (30 folders); the label pool the letters standing in neither `now` nor `adopted`; the draw and
    the order panel 256's (the six cells in a fixed sequence, reversed on the even replicates, the verdict last).
  - **`run.sh`**: the caps 0.60 and 1.20, the margin 0.10, **the ceiling 20.00** and the registered ceiling 20.00 (the
    stop is `spent + cap + 0.10 > 20.00`; the verdict runs only if the 30 spent at most 18.70); `WARDEN` is
    `docs/panel/261-reports/spec-warden.md` on `main`, its section `## Predictions registered before the blind run`
    pinned by sha256 as the run starts; `REGISTRATION_SHA` is the copy of `blind/registration.md` as the synthesis
    completes it; **the session command panel 259's, flag for flag**, which is panel 256's (`--bare --restricted
    --safe-mode --strict-mcp-config --model claude-opus-5-5`, `--tools "Read,Write" --allowedTools "Read,Write"`, the
    same `--disallowedTools`, `--max-budget-usd "$2"`, `< /dev/null`, `HOME`, `TMPDIR`, `CLAUDE_CODE_TMPDIR`,
    `GIT_CEILING_DIRECTORIES`, `env -i`; panel 256's `run.sh:106-113`, `sed -n`, the repair).
  - **`preflight.sh`**: the constants it checks (0.60, 1.20, 0.10, 20.00); its arm check reading
    `tools/synthesis-adopted.diff` alone, the adopted text's diff the synthesis writes (no rival arm).
  - **`check_folders.py`**: a graded folder holds exactly `brief.md`, `task.md` and `spec.md`.
  - **`make_verdict.py`** (panel 259's, new there): `verdict/` holds `spec-A.md` (`now`), `spec-B.md` (`adopted`),
    their unified diff, `task.md` the three tasks, t1 then t2 then t3, separated by a line `---`, and
    `reports/<folder>.md` for each successful graded session, named by its label, never its arm; no `programs/`.
  - **`brief-verdict-reports.md`** (panel 259's, new there): its words say **three tasks** and **two versions** of the
    specification (panel 259's said two tasks and three versions), and list the thirty reports' names.
  - **`inputs.sha256`**: the two arms, the three tasks and `tools/brief-session.md`, by the hashes of
    `blind/registration.md` § The inputs, the adopted arm's as the synthesis writes it.
  - **`registration.md`**: a copy of `blind/registration.md` as the synthesis completes it, its sha256 pinned in
    `run.sh`; `synthesis-adopted.diff`: a copy of the adopted text's diff.
- **Expected unchanged by hash** (`cmp` equal to panel 256's committed bytes, the run's session to say so):
  `scan.py`, `check_area.py`, `read_runs.py`, `spent.py`, `brief-verdict.md`, `after.py`, `reads.py`, as panel 259's
  were.

## diff of brief-session.md, against panel 256's bytes

Panel 256's `brief-session.md`: 1,642 bytes, sha256
`724e8e0d656bb7b0daa51c4422337b2b993f44b6d8499d980fa7801a7652690b` (`wc -c`, `shasum -a 256`, the repair). `diff -u
256/brief-session.md blind/brief-session.md`, run by the repair:

```diff
--- 256/brief-session.md
+++ blind/brief-session.md
@@ -8,7 +8,7 @@
 
 The files in this directory are your whole input. Read no other file, and do not ask for one.
 
-- `heroes-spec.md`: the language's specification, the whole language.
+- `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
 
 ## Your method
@@ -20,12 +20,14 @@
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, each question answered in its order; every file whole, under its
-  path.
+- `answer`: what the task asks, in this order and under these names:
+  - `main.hero`: the program, whole, in one fenced code block, and no other code block in `answer`;
+  - `prints`: every line you expect the program to print, in order;
+  - `compiles`: *yes* or *no*, and if no, which line and why;
+  - `parameters`: for each function the program binds, each of its parameters and why it is written
+    so.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
-- `prediction`: whether the compiler accepts the program, every line the program writes, in order,
-  and how it ends: normally, by an abort and what the abort says, or not at all.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
   this directory reached your context, and which.
 
```

**Read for a leak** (`grep -n -i 'heroes\|panel\|seat\|lent\|@'` of `blind/brief-session.md`, the repair: nothing):
the brief names no project, seat, arm, mode or defect; `main.hero` names the extension, as `task-t1.md` to
`task-t3.md` already do (the critic's § 4).

## The hashes, as registered by the repair

```
724e8e0d656bb7b0daa51c4422337b2b993f44b6d8499d980fa7801a7652690b  docs/panel/256-briefs/blind/brief-session.md
e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222  docs/panel/261-briefs/blind/brief-session.md
6a7f993c287dc5217a5a968dfe8e3733804e4d5a5d54b9b58d24b568bffe6d98  docs/panel/259-briefs/blind/registration-runner.md
```

The run's session appends the hashes of the runner as it registers it, as panel 259's file ends.

---

# Appended by the run's session, 2026-11-26 13:34 local, before any session (none drawn into a run, none run)

**Where this stands.** The file above is `docs/panel/261-briefs/blind/registration-runner.md` byte for byte (sha256
`54ca33175241e1b3c2d2fe0058647b46d43721b22560e761b6f99eca2366be84`, its first 7,996 bytes here). It asks the run's session
to write the runner's diffs *below this list*; the coordinator's brief to this session forbids *an edit of a registered
file*, so the registered file in the tree stands untouched and its continuation is this copy, the area's
`tools/registration-runner.md`, which the answers' archive carries.

**Panel 259's runner, rebuilt.** Panel 259's archive (`docs/panel/259-reports/llm-ergonomist-blind-answers.tar.xz`,
unpacked into `.claude/worktrees/261-llm-ergonomist-scratch/a259/`) holds its whole runner. Its
`tools/registration-runner.md` (sha256 `6a7f993c...6d98`, `cmp` equal to `docs/panel/259-briefs/blind/registration-runner.md`)
holds five diffs against panel 256's committed bytes; each was extracted and applied by `patch` to
`docs/panel/256-briefs/blind/`'s bytes: **all five applied clean** (exit 0), and each rebuilt file is **`cmp` equal**
to the archive's (`run.sh`, `preflight.sh`, `check_folders.py`, `make_sessions.py`, `brief-session.md`). Panel
259's `make_verdict.py`, `brief-verdict-reports.md` and `inputs.sha256` (new there) are taken from its archive.

**Unchanged by hash** (`cmp` equal to panel 256's committed bytes): `scan.py`, `check_area.py`, `read_runs.py`,
`spent.py`, `brief-verdict.md`, `after.py`, `reads.py`, as panel 259's were.

**Copies**: `tools/registration.md` is `docs/panel/261-briefs/blind/registration.md` as the synthesis completed it
(sha256 `9e241bee...e285`, pinned in `run.sh`); `tools/synthesis-adopted.diff` is
`docs/panel/261-reports/synthesis-adopted.diff` (`202d8a03...4582`); `tools/brief-session.md` is
`docs/panel/261-briefs/blind/brief-session.md` (`e8fee1a4...0222`). The arms: `arms/spec-now.md` is `git show
df92f9281:spec/heroes-spec.md` (`3df14254...d93f`), `arms/spec-adopted.md` is
`docs/panel/261-reports/synthesis-adopted.txt` (`e9b0c819...758c`), equal to the first with the diff applied by
`patch`; the tasks are `blind/task-t1.md` to `task-t3.md`. Every one by the registration's hash.

**The warden's predictions**: `docs/panel/261-reports/spec-warden.md` on `main` (sha256 `1a7cf8e8...0f43`, the hash
the synthesis read), its section `## Predictions registered before the blind run` sha256
`98a3455be021372392dc4e6bd0629d4a9d982a94f5d499cb7ccfdf6dd62bcc7f`, pinned in `run.sh`.

## diff of run.sh, against panel 256's bytes

```diff
--- 256/run.sh
+++ run.sh
@@ -1,68 +1,84 @@
 #!/bin/bash
-# Panel 256's blind seat (panel 254's runner, its caps, paths and gates changed by asserted edits): runs the
-# sessions order.txt names, one at a time, never two, each from its own folder, every graded session capped
-# at 1.00 USD and the verdict at 1.50, and stops before the sum would pass 19.50 USD, under the author's
-# 20 USD for this seat (2026-11-22, relayed by the coordinator of M-rebel-library). A folder already
-# holding run.json is skipped and its cost counted, so a stopped run resumes.
+# Panel 261's blind seat (panel 256's runner as panels 257 and 259 adapted it, adapted again for panel 261: see
+# tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
+# time, never two, each from its own folder: the 30 graded ones (t1, t2, t3 under `now` and `adopted`, five each)
+# capped at 0.60 USD each, the verdict at 1.20, and stops before the sum plus the next session's cap plus a 0.10
+# margin would pass 20.00 USD, the author's ceiling for this run (2026-11-26, *ok a 20 dollars*, recorded in
+# docs/panel/261-briefs/llm-ergonomist.md § The size, the spend and the cap).
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
+# Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation:
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-llm-ergonomist/run.sh
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
+WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/261-reports/spec-warden.md
+WARDEN_SECTION_SHA=98a3455be021372392dc4e6bd0629d4a9d982a94f5d499cb7ccfdf6dd62bcc7f
+# The registration as the synthesis completed it (the arms' hashes, the codes, the caps), pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92
+REGISTRATION_SHA=9e241beea319d55dac31cae89f62a1aa2f98ff006372bc60814ea4fd0c34e285
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
@@ -71,39 +87,43 @@
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
@@ -111,15 +131,28 @@
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
@@ -146,6 +179,10 @@
     *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
     *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
 esac
+if [ "$PREFLIGHT_ONLY" = 1 ]; then
+    log "preflight only: stopped before the key is read; no paid session run"
+    exit 0
+fi
 
 # The key from .env, its length alone printed, never its value.
 set -a
@@ -154,7 +191,9 @@
 echo "ANTHROPIC_API_KEY length ${#ANTHROPIC_API_KEY}"
 [ "${#ANTHROPIC_API_KEY}" -gt 0 ] || { echo "no key: nothing run"; exit 2; }
 
-while read -r folder; do
+STOPPED=0
+passed_registered=0
+while read -r folder <&3; do
     [ -n "$folder" ] || continue
     if [ "$folder" = verdict ]; then d="$HERE/verdict"; cap=$CAP_VERDICT; else d="$HERE/s/$folder"; cap=$CAP; fi
     if [ -f "$d/run.json" ]; then
@@ -164,8 +203,20 @@
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
@@ -174,8 +225,15 @@
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

## diff of run.sh, against panel 259's (rebuilt)

```diff
--- 259/run.sh
+++ run.sh
@@ -1,16 +1,17 @@
 #!/bin/bash
-# Panel 259's blind seat (panel 256's runner as panel 257 adapted it, adapted again for panel 259: see
+# Panel 261's blind seat (panel 256's runner as panels 257 and 259 adapted it, adapted again for panel 261: see
 # tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
-# time, never two, each from its own folder: the 30 graded ones capped at 0.60 USD each, the verdict at 1.20, and
-# stops before the sum plus the next session's cap plus a 0.10 margin would pass 20.00 USD, the author's ceiling for
-# this run (2026-11-25 09:25, recorded in docs/panel/259-briefs/llm-ergonomist.md; registration.md's cap section).
+# time, never two, each from its own folder: the 30 graded ones (t1, t2, t3 under `now` and `adopted`, five each)
+# capped at 0.60 USD each, the verdict at 1.20, and stops before the sum plus the next session's cap plus a 0.10
+# margin would pass 20.00 USD, the author's ceiling for this run (2026-11-26, *ok a 20 dollars*, recorded in
+# docs/panel/261-briefs/llm-ergonomist.md § The size, the spend and the cap).
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
@@ -36,7 +37,7 @@
 # a stop. SIGINT and SIGTERM to the runner kill the session in flight and remove the lock.
 #
 # Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation:
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-llm-ergonomist/run.sh
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-llm-ergonomist/run.sh
 set -u
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
@@ -54,11 +55,11 @@
 REGISTERED_CEILING=20.00
 SESSION_LIMIT=900
 ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
-WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/259-reports/spec-warden.md
-WARDEN_SECTION_SHA=5be9f7b3c83152f3eb5fd0089da5962fcbde959a97b1d6cf9a5020bb0f2fa82a
-# The coordinator's registration (the arms' hashes, the cap and the predictions), pinned here.
+WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/261-reports/spec-warden.md
+WARDEN_SECTION_SHA=98a3455be021372392dc4e6bd0629d4a9d982a94f5d499cb7ccfdf6dd62bcc7f
+# The registration as the synthesis completed it (the arms' hashes, the codes, the caps), pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=4d68580993c98f9c2d1e1396632a0a6e52e12a1140286884eee2384a86b0bc44
+REGISTRATION_SHA=9e241beea319d55dac31cae89f62a1aa2f98ff006372bc60814ea4fd0c34e285
 HEADING='## Predictions registered before the blind run'
 CLI_TMP="/private/tmp/claude-$(id -u)"
 REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
```

## diff of preflight.sh, against panel 256's bytes

```diff
--- 256/preflight.sh
+++ preflight.sh
@@ -1,153 +1,116 @@
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
+# Panel 261's preflight (panel 259's, its arms, tasks, folders and arm check changed for panel 261): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
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
+echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 7 to 12, an inference, unrun"
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
+# the registered inputs, by hash: the arms, the tasks, the session brief, and the registration itself
+while read -r f h; do
+    [ -n "$f" ] || continue
+    chk "$f is the registered file ($(echo "$h" | cut -c1-12))" '[ "$(sha "$f")" = "$h" ]'
+done < tools/inputs.sha256
+REG_SHA=$(grep -E '^REGISTRATION_SHA=' run.sh | cut -d= -f2)
+chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
+chk "every arm's full sha256 stands in the registration's own table" \
+    'for a in now adopted; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
+for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 10c2a8f93476d39b' 'brief-verdict.md 52d37bb04ac8ab20'; do
+    tf=${pair% *}; tp=${pair#* }
+    chk "tools/$tf is panel 256's registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
+done
+chk "tools/synthesis-adopted.diff is the synthesis's diff (202d8a03a086...)" '[ "$(sha tools/synthesis-adopted.diff)" = 202d8a03a086281da72f4da493534f55df4d30442e687f47fbbcf37858044582 ]'
+chk "the adopted arm is the frozen spec with tools/synthesis-adopted.diff applied by patch (six hunks, the synthesis's text)" \
+    'mkdir -p check && rm -f check/adopted-applied.md && patch -s -o check/adopted-applied.md arms/spec-now.md tools/synthesis-adopted.diff && [ "$(sha check/adopted-applied.md)" = "$(sha arms/spec-adopted.md)" ]'
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
+chk "check_folders: 31 in order, 30 folders holding brief.md, spec.md and task.md alone at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
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

## diff of preflight.sh, against panel 259's (rebuilt)

```diff
--- 259/preflight.sh
+++ preflight.sh
@@ -1,5 +1,5 @@
 #!/bin/bash
-# Panel 259's preflight (panel 257's, its constants, arms and diffs changed for panel 259): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
+# Panel 261's preflight (panel 259's, its arms, tasks, folders and arm check changed for panel 261): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
 #   bash preflight.sh            the static checks alone: no `claude -p`, no API call, no key read except its LENGTH
 #   bash preflight.sh --session  the static checks, then `bash run.sh --preflight-only`: the one free session of
 #                                run.sh (panel 256's), whose key is a string the API refuses (cost 0 measured at
@@ -26,7 +26,7 @@
 chk "caps are the registered 0.60 and 1.20, margin 0.10" '[ "$CAP" = 0.60 ] && [ "$CAPV" = 1.20 ] && [ "$MARGIN" = 0.10 ]'
 chk "ceiling is 20.00, the author's for the whole run" '[ "$CEILING" = 20.00 ]'
 WORST=$(python3 -I -c "print(f'{30*$CAP + $CAPV:.2f}')")
-echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate 6.3 to 8.5, an inference, unrun"
+echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 7 to 12, an inference, unrun"
 chk "the worst case stays under the ceiling, margin included (every session can run)" "python3 -I -c 'import sys; sys.exit(0 if $WORST + $MARGIN <= $CEILING else 1)'"
 
 # the runner's own text: every flag of the registered command, no tool beyond Read and Write
@@ -39,7 +39,7 @@
     'grep -qF -- "--disallowedTools \"Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage\"" run.sh'
 chk "no pkill, no killall in run.sh" '! grep -nE "^[^#]*(pkill|killall)" run.sh'
 
-# the registered inputs, by hash: the arms, the tasks, the programs, and the registration itself
+# the registered inputs, by hash: the arms, the tasks, the session brief, and the registration itself
 while read -r f h; do
     [ -n "$f" ] || continue
     chk "$f is the registered file ($(echo "$h" | cut -c1-12))" '[ "$(sha "$f")" = "$h" ]'
@@ -47,21 +47,14 @@
 REG_SHA=$(grep -E '^REGISTRATION_SHA=' run.sh | cut -d= -f2)
 chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
 chk "every arm's full sha256 stands in the registration's own table" \
-    'for a in now adopted rival; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
+    'for a in now adopted; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
 for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 10c2a8f93476d39b' 'brief-verdict.md 52d37bb04ac8ab20'; do
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
+chk "tools/synthesis-adopted.diff is the synthesis's diff (202d8a03a086...)" '[ "$(sha tools/synthesis-adopted.diff)" = 202d8a03a086281da72f4da493534f55df4d30442e687f47fbbcf37858044582 ]'
+chk "the adopted arm is the frozen spec with tools/synthesis-adopted.diff applied by patch (six hunks, the synthesis's text)" \
+    'mkdir -p check && rm -f check/adopted-applied.md && patch -s -o check/adopted-applied.md arms/spec-now.md tools/synthesis-adopted.diff && [ "$(sha check/adopted-applied.md)" = "$(sha arms/spec-adopted.md)" ]'
 chk "tools/registration-runner.md (the runner's diffs against panel 256's bytes) exists" '[ -s tools/registration-runner.md ]'
 
 # the warden's registered predictions, pinned
@@ -71,7 +64,7 @@
 
 # the folders, the order, the area
 chk "order.txt and key.txt stand" '[ -s order.txt ] && [ -s key.txt ]'
-chk "check_folders: 31 in order, 30 folders at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
+chk "check_folders: 31 in order, 30 folders holding brief.md, spec.md and task.md alone at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
 chk "check_area: the area is the top of its own empty git repository, reader <reader@invalid>, HOME and TMPDIR inside" 'python3 -I tools/check_area.py'
 chk "git log in the area says no commit" 'git log 2>&1 | grep -q "does not have any commits yet"'
 chk "no CLAUDE.md, AGENTS.md, MEMORY.md or memory folder in the area (git's own folder left out)" \
```

## diff of tools/check_folders.py, against panel 256's bytes

```diff
--- 256/check_folders.py
+++ tools/check_folders.py
@@ -1,28 +1,73 @@
-# Before any spend (panel 256's, from panel 254's): order.txt names 46 folders, each in key.txt, and every folder not yet run holds
-# exactly its registered files at their registered hashes. usage: python3 -I tools/check_folders.py
+# Before any spend (panel 256's check_folders.py, as changed for panel 257, used by panel 259, and changed for panel
+# 261: two arms, three tasks, no programs/): order.txt names 31 folders (the 30 graded ones, 5 per task and arm, then
+# `verdict` last), each in key.txt; every graded folder not yet run holds exactly `brief.md`, `task.md` and `spec.md`,
+# at their registered hashes, and each of those hashes is the one tools/inputs.sha256 registers (the arm's spec, the
+# task) or tools/brief-session.md's; no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
+# before then (`--verdict` asks only of it, after tools/make_verdict.py).
+# usage: python3 -I tools/check_folders.py [--verdict]
 import hashlib, os, sys
+ARMS = ('now', 'adopted')
+GRADED_FILES = ['brief.md', 'spec.md', 'task.md']
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
+want = {(t, a): 5 for t in ('t1', 't2', 't3') for a in ARMS}
+if cells != want:
+    bad.append(f'the cells are not 5 sessions of each task and arm: {sorted(cells.items())}')
+brief_sha = sha('tools/brief-session.md')
+def registered(name, arm, f):
+    """The hash a file of a graded session's folder must have, from the registration's inputs."""
+    task = name.split('-')[0]
+    if f == 'spec.md': return reg[f'arms/spec-{arm}.md']
+    if f == 'task.md': return reg[f'tasks/task-{task}.md']
+    if f == 'brief.md': return brief_sha
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
+    if name != 'verdict' and have != GRADED_FILES:
+        bad.append(f'{name} does not hold exactly brief.md, spec.md and task.md: {have}'); continue
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

## diff of tools/check_folders.py, against panel 259's (rebuilt)

```diff
--- 259/check_folders.py
+++ tools/check_folders.py
@@ -1,12 +1,13 @@
-# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used unchanged in code by panel 259): order.txt names 31 folders (the 30 graded
-# ones, 5 per task and arm, then `verdict` last), each in key.txt; every folder not yet run holds exactly its
-# registered files, nested ones included (`programs/`), at their registered hashes, and each of those hashes is
-# the one tools/inputs.sha256 registers (the arm's spec, the task, the programs) or tools/brief-session.md's;
-# no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
+# Before any spend (panel 256's check_folders.py, as changed for panel 257, used by panel 259, and changed for panel
+# 261: two arms, three tasks, no programs/): order.txt names 31 folders (the 30 graded ones, 5 per task and arm, then
+# `verdict` last), each in key.txt; every graded folder not yet run holds exactly `brief.md`, `task.md` and `spec.md`,
+# at their registered hashes, and each of those hashes is the one tools/inputs.sha256 registers (the arm's spec, the
+# task) or tools/brief-session.md's; no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
 # before then (`--verdict` asks only of it, after tools/make_verdict.py).
 # usage: python3 -I tools/check_folders.py [--verdict]
 import hashlib, os, sys
-ARMS = ('now', 'adopted', 'rival')
+ARMS = ('now', 'adopted')
+GRADED_FILES = ['brief.md', 'spec.md', 'task.md']
 def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
 key = {}
@@ -25,7 +26,7 @@
         continue
     task = name.split('-')[0]
     cells[(task, key[name][0])] = cells.get((task, key[name][0]), 0) + 1
-want = {(t, a): 5 for t in ('t1', 't2') for a in ARMS}
+want = {(t, a): 5 for t in ('t1', 't2', 't3') for a in ARMS}
 if cells != want:
     bad.append(f'the cells are not 5 sessions of each task and arm: {sorted(cells.items())}')
 brief_sha = sha('tools/brief-session.md')
@@ -35,7 +36,6 @@
     if f == 'spec.md': return reg[f'arms/spec-{arm}.md']
     if f == 'task.md': return reg[f'tasks/task-{task}.md']
     if f == 'brief.md': return brief_sha
-    if f.startswith('programs/'): return reg[f]
     return None
 def listing(d):
     return sorted(os.path.relpath(os.path.join(r, x), d) for r, _, fs in os.walk(d) for x in fs)
@@ -54,6 +54,8 @@
     if name != 'verdict' and arm not in ARMS:
         bad.append(f'{name} names the arm {arm!r}'); continue
     have = listing(d)
+    if name != 'verdict' and have != GRADED_FILES:
+        bad.append(f'{name} does not hold exactly brief.md, spec.md and task.md: {have}'); continue
     if have != sorted(files):
         bad.append(f'{name} holds other files: {have}'); continue
     for f, h in files.items():
```

## diff of tools/make_sessions.py, against panel 256's bytes

```diff
--- 256/make_sessions.py
+++ tools/make_sessions.py
@@ -1,58 +1,55 @@
-# Panel 256's blind seat: draws the labels, then makes one session folder per task, arm and replicate, each
-# holding brief.md, task.md and heroes-spec.md and nothing else, and the verdict session's folder. The key
-# and the order are written beside s/, never inside it. Refuses to run over an existing s/. Written
-# 2026-11-22 by panel 256's coordinator, on panel 254's make_sessions.py.
-# usage (from the blind area): python3 -I tools/make_sessions.py
-import difflib, hashlib, os, secrets, sys
+# Panel 261's blind seat (panel 256's make_sessions.py as panels 257 and 259 changed it; for 261 the arms, the tasks
+# and the folders' files changed, docs/panel/261-briefs/blind/registration-runner.md): draws the arms' labels, then
+# makes one session folder per task, arm and replicate (t1, t2 and t3, `now` and `adopted`, 5 each: 30 folders), each
+# holding brief.md, task.md and spec.md and nothing else (a write task holds no programs/), and writes key.txt and
+# order.txt beside s/, never inside it. THE DRAW: the two labels are sampled from a pool of letters with panel 256's
+# method, secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place
+# the draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
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
+ARMS = ['now', 'adopted']
+SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
+TASKS = ['t1', 't2', 't3']
+# no letter that stands in an arm's name (now, adopted), so a label says nothing of its arm
+POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted')]
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
+            assert sorted(files) == ['brief.md', 'spec.md', 'task.md']
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

## diff of tools/make_sessions.py, against panel 259's (rebuilt)

```diff
--- 259/make_sessions.py
+++ tools/make_sessions.py
@@ -1,10 +1,10 @@
-# Panel 259's blind seat (panel 256's make_sessions.py as panel 257 changed it; for 259 only the programs each task's
-# folder holds changed): draws the arms' labels, then makes one session
-# folder per task, arm and replicate (t1 and t2, `now`, `adopted`, `rival`, 5 each: 30 folders), each holding
-# brief.md, task.md, spec.md and programs/ and nothing else, and writes key.txt and order.txt beside s/, never
-# inside it. THE DRAW: the three labels are sampled from a pool of letters with panel 256's method,
-# secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place the
-# draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
+# Panel 261's blind seat (panel 256's make_sessions.py as panels 257 and 259 changed it; for 261 the arms, the tasks
+# and the folders' files changed, docs/panel/261-briefs/blind/registration-runner.md): draws the arms' labels, then
+# makes one session folder per task, arm and replicate (t1, t2 and t3, `now` and `adopted`, 5 each: 30 folders), each
+# holding brief.md, task.md and spec.md and nothing else (a write task holds no programs/), and writes key.txt and
+# order.txt beside s/, never inside it. THE DRAW: the two labels are sampled from a pool of letters with panel 256's
+# method, secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place
+# the draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
 # the six cells in a fixed sequence, reversed on the even replicates, the verdict last.
 # The verdict's folder is NOT made here: it is built after the 30 by tools/make_verdict.py.
 # Refuses to run over an existing s/, verdict/, key.txt or order.txt.
@@ -15,12 +15,11 @@
     if os.path.exists(p):
         sys.exit(f'{p} exists: nothing made')
 REPLICATES = 5
-ARMS = ['now', 'adopted', 'rival']
+ARMS = ['now', 'adopted']
 SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
-TASKS = ['t1', 't2']
-PROGRAMS = {'t1': ['co.h', 'sw.h', 'jump.h'] + [f'q{i}.hero' for i in range(1, 8)], 't2': ['co.h', 'sw.h']}
-# no letter that stands in an arm's name (now, adopted, rival), so a label says nothing of its arm
-POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted' + 'rival')]
+TASKS = ['t1', 't2', 't3']
+# no letter that stands in an arm's name (now, adopted), so a label says nothing of its arm
+POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted')]
 def sha(b): return hashlib.sha256(b).hexdigest()
 def read(p): return open(p, 'rb').read()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
@@ -37,8 +36,7 @@
             name = f'{task}-{draw[arm]}-{n}'
             d = os.path.join('s', name)
             files = {'brief.md': brief, 'task.md': read(f'tasks/task-{task}.md'), 'spec.md': read(SPEC[arm])}
-            for p in PROGRAMS[task]:
-                files[f'programs/{p}'] = read(f'programs/{p}')
+            assert sorted(files) == ['brief.md', 'spec.md', 'task.md']
             for f, b in files.items():
                 os.makedirs(os.path.dirname(os.path.join(d, f)), exist_ok=True)
                 open(os.path.join(d, f), 'wb').write(b)
```

## diff of tools/brief-session.md, against panel 259's (rebuilt; its diff against panel 256's bytes is the registered one above, `cmp` equal)

```diff
--- 259/brief-session.md
+++ tools/brief-session.md
@@ -1,8 +1,8 @@
 # Your brief
 
 You judge one thing: whether the language's specification, read with no other file, lets a language
-model predict what a program does when it is built and run. Your verdict is an experiment, not an
-opinion: **answer from the specification, don't opine.**
+model predict what a program does when it is built and run, and write the program its author meant,
+on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**
 
 ## Your inputs, and nothing else
 
@@ -10,19 +10,22 @@
 
 - `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
-- `programs/`: the C files the task names.
 
 ## Your method
 
-Read the task and do what it asks, each item in its order and under its own name. For every place
+Read the task and do what it asks. Write every file you write whole, with its path. For every place
 where the specification left you a choice, say the choice you made and what the other would produce:
 a program the compiler refuses, a program that stops with an error when run, a program that never
 ends, or a program that runs and gives a different answer than intended.
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, each question answered in its order, each program or design under
-  its own name.
+- `answer`: what the task asks, in this order and under these names:
+  - `main.hero`: the program, whole, in one fenced code block, and no other code block in `answer`;
+  - `prints`: every line you expect the program to print, in order;
+  - `compiles`: *yes* or *no*, and if no, which line and why;
+  - `parameters`: for each function the program binds, each of its parameters and why it is written
+    so.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
```

## diff of tools/make_verdict.py, against panel 259's (new there, not in panel 256's)

```diff
--- 259/tools/make_verdict.py
+++ tools/make_verdict.py
@@ -1,11 +1,9 @@
-# Panel 259's blind seat: builds the verdict session's folder after the 30 graded sessions, as the registration says
-# (docs/panel/259-briefs/llm-ergonomist.md § The rows: "its folder is built after the 30 and holds the two specs, a
-# diff and the reports, never a key"). Written 2026-11-25 by the blind run's session, before any session ran; panel
-# 257's own make_verdict.py is not in its archive, so this file is new, its shape panel 257's as its report describes
-# it (docs/panel/257-reports/llm-ergonomist-blind.md § The A/B verdict): spec-A.md is `now`, spec-B.md is `adopted`
-# (the resolution's text), changes.diff their unified diff, task.md t1 then t2, programs/ t1's programs, reports/ one
-# file per successful graded session named by its folder (its label, never its arm), report.md where the session wrote
-# one, else the result.md tools/after.py kept. brief.md is tools/brief-verdict.md (panel 256's registered bytes) with
+# Panel 261's blind seat (panel 259's make_verdict.py, changed for panel 261 as
+# docs/panel/261-briefs/blind/registration-runner.md asks: three tasks, two arms, no programs/): builds the verdict
+# session's folder after the 30 graded sessions: spec-A.md is `now`, spec-B.md is `adopted` (the synthesis's text),
+# changes.diff their unified diff, task.md t1 then t2 then t3, separated by a line `---`, reports/ one file per
+# successful graded session named by its folder (its label, never its arm), report.md where the session wrote one,
+# else the result.md tools/after.py kept; no programs/. brief.md is tools/brief-verdict.md (panel 256's registered bytes) with
 # tools/brief-verdict-reports.md appended, then the thirty report names listed, which panel 257's verdict lacked (it
 # could not list reports/ with Read alone and read none of them). Appends the verdict's row to key.txt.
 # Refuses to run over an existing verdict/, or before every graded folder holds a run.json.
@@ -39,12 +37,10 @@
 a_txt, b_txt = read('arms/spec-now.md'), read('arms/spec-adopted.md')
 diff = ''.join(difflib.unified_diff(a_txt.decode().splitlines(True), b_txt.decode().splitlines(True),
                                     fromfile='spec-A.md', tofile='spec-B.md')).encode()
-task = read('tasks/task-t1.md') + b'\n---\n\n' + read('tasks/task-t2.md')
+task = read('tasks/task-t1.md') + b'\n---\n\n' + read('tasks/task-t2.md') + b'\n---\n\n' + read('tasks/task-t3.md')
 names = '\n'.join(f'- `{r}`' for r in sorted(reports)).encode()
 brief = read('tools/brief-verdict.md') + b'\n' + read('tools/brief-verdict-reports.md') + b'\n' + names + b'\n'
 files = {'brief.md': brief, 'spec-A.md': a_txt, 'spec-B.md': b_txt, 'changes.diff': diff, 'task.md': task}
-for p in ('co.h', 'sw.h', 'jump.h') + tuple(f'q{i}.hero' for i in range(1, 8)):
-    files[f'programs/{p}'] = read(f'programs/{p}')
 files.update(reports)
 for f, b in files.items():
     os.makedirs(os.path.dirname(os.path.join('verdict', f)) or 'verdict', exist_ok=True)
```

## diff of tools/brief-verdict-reports.md, against panel 259's (new there, not in panel 256's)

```diff
--- 259/tools/brief-verdict-reports.md
+++ tools/brief-verdict-reports.md
@@ -1,10 +1,9 @@
 ## Also in this directory
 
-- `task.md` holds two tasks, the first then the second after the line `---`; do each under each
-  specification. Its programs are in `programs/`.
-- `reports/`: thirty reports by other readers, each of whom read one of three versions of the
-  specification (two of them are `spec-A.md` and `spec-B.md`; which version a report's reader had is not
-  said) and did one of the two tasks, with no file but that version, the task and its programs. Read
-  every report before you write your verdict, and say in `experiment` where the readers agree with your
-  own answers and where they do not. You cannot list a folder, so the reports' names are given here, all
-  thirty:
+- `task.md` holds three tasks, the first, then the second after a line `---`, then the third after
+  another; do each under each specification.
+- `reports/`: thirty reports by other readers, each of whom read one of the two versions of the
+  specification (`spec-A.md` or `spec-B.md`; which version a report's reader had is not said) and did
+  one of the three tasks, with no file but that version and the task. Read every report before you
+  write your verdict, and say in `experiment` where the readers agree with your own answers and where
+  they do not. You cannot list a folder, so the reports' names are given here, all thirty:
```

## diff of tools/inputs.sha256, against panel 259's (new there, not in panel 256's)

```diff
--- 259/tools/inputs.sha256
+++ tools/inputs.sha256
@@ -1,15 +1,6 @@
-arms/spec-now.md c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee
-arms/spec-adopted.md aaf23b9e7f76472e672a1ce51d6c7efbe00f5e21c2b83d8f5caea44a6af616f3
-arms/spec-rival.md 0461f1d3941433d09f41299f13d56361a4d37eec01351bf742d53cc1edcf4ac1
-tasks/task-t1.md f71c54471a591832e97f563f1bb089d4ae10faccf0cdea07beeb063a4ef952f7
-tasks/task-t2.md 28c1309c762b2cb462568ad775e417543fb528b35bec5d0c37569a971d6be097
-programs/co.h ed909982b316b77d2a8a99d6871d7dc049ad6c82aed642ba602c99b048560c8c
-programs/sw.h e2c84c66eab40ee72cf3c690dd813bc1274678e26b5b30f209571a1daa2ac135
-programs/jump.h 64d3ab4492142e99104a3791c4579381c7582467b0f492048c7e7100fc1ef977
-programs/q1.hero 2dd061aeee342fd956d8eb5dec4d349496a6d36195c5f1e1f2234d79c545d1ce
-programs/q2.hero 3da8917ef51efcbcc745534ec4ee08ef8f1f909ee555eb9a7c84cb54ae8fe41e
-programs/q3.hero 180fac477656effc92111441fa3534a9563708f032241c8b181692143ddd3791
-programs/q4.hero 5b9351effc2758f217c73f46d1ef98652d95cb063449135c2fe80d4e3758ea58
-programs/q5.hero 33d97704951be1ef2f7ae8c5dc56e49d533c475aeae1d77bce5a1ba2fbf70fc1
-programs/q6.hero aad2c4d0b4fbcaeb37ffd99a32d597e89299ca4600e137b75853a7254bbb318c
-programs/q7.hero ebb1c12946a476bbb2b67434380cfee8eab5770c921412e10427b2024716da28
+arms/spec-now.md 3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f
+arms/spec-adopted.md e9b0c819cce1c91ec74e43f2197bdd490239ccacae53af7ca9e943052bb2758c
+tasks/task-t1.md 1695341319fbd053b581589cb0c3330e0f002f642edc6a4dc9f57bb3b521abe8
+tasks/task-t2.md 2b952b79348c1b9b8135c9be3bffb6cef33375086588e849a8ef8c48ca424c45
+tasks/task-t3.md cea2d2b6e5f03a40226d5820aaa56c887fcd5cdc27152bd9b9cf1022904f8cfd
+tools/brief-session.md e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222
```

## The hashes of the runner as registered (`shasum -a 256`, before any session)

```
bdd834f0c69d315d7eebcc2432ff5d2fc47a99198217e0e57085814865f55722  run.sh
67434325c2217221e0d9a31f442025edd78aeb5deebd46278d2b714be8deca28  preflight.sh
e5b8bd741da31af51a2491adf2adc8e8e110133251f7b0f11580ad470ece48aa  tools/after.py
e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222  tools/brief-session.md
f353b9029046d6813c14b63efc32f0f127b505dec8a905b294a105ac4e0a6f89  tools/brief-verdict-reports.md
52d37bb04ac8ab20ae12458f1f7550a3ffaa818d5e48cec88fe90359f8835a36  tools/brief-verdict.md
083089c60775136c80fec0599033d250d93032a9a7b66c4d9ac0914bd530e169  tools/check_area.py
8aac4f85180a5e78e826bfca0dbcc3c4dae9e79ce1b7f95e8e397894a9b9370a  tools/check_folders.py
07c84afb65b38707fc40af92b87987b2b75bc8be74a17061a30da66d0d84f7ed  tools/inputs.sha256
21dd43a4f4b48729ca9363bdec3631b40e6a5bd0991a0866ff9660255311fedc  tools/make_sessions.py
50a55481a4e79ec9c16e77721807169f5ee511e8e105214fa64035ccda309ee8  tools/make_verdict.py
d6fe89f5c79e6d938577e1352829788b4251fcc66c81bd5e896e924d095de63d  tools/read_runs.py
a1519e78f9154349e9d1c58183bfcf0f390fd3a6cba9cf795f472b2682711b1d  tools/reads.py
9e241beea319d55dac31cae89f62a1aa2f98ff006372bc60814ea4fd0c34e285  tools/registration.md
41aae8419d682a6d110d276998070924cb5fb259ed3993e234a3847e7a99d5c1  tools/scan.py
10c2a8f93476d39bbc60349099c23b4d1b15fbffb7e42de938a65673d7003ef0  tools/spent.py
202d8a03a086281da72f4da493534f55df4d30442e687f47fbbcf37858044582  tools/synthesis-adopted.diff
```

---

# Appended 2026-11-26 14:31, after the 30 graded sessions' draw and 29 counted, before t1-l-4's rerun and the verdict

**Two changes to the runner after sessions ran, each for a case the runner did not foresee**, the runner otherwise as
registered above:

1. **t1-l-4 was cut at 14:06:27** by the coordinator's session restarting (`run.log`: *signal received: runner
   stopping*), 4 s into its run: `run.json` and `run.err` empty; its transcript records one request and a cost-state of
   0.003128 USD, a second request in flight unrecorded. The resumed runner (14:08:56) skipped it by its rule, so 29
   sessions were counted. At 14:17:07 `tools/make_verdict.py` stopped the runner with a `JSONDecodeError` on that empty
   `run.json`, where its own rule is to leave a session that is not a success out of the verdict. **Repaired**: an
   unreadable `run.json` is no success (diff 1). No verdict session had run.
2. **The coordinator's instruction after the network outage of about 14:25** (nothing of this run was in flight then):
   *a session cut mid-run counts only if its JSON says success and completed; otherwise it is rerun and its spend (if
   any) still counts toward the ceiling*. So t1-l-4's cut `run.json` and `run.err` moved to
   `cut/t1-l-4-20261126-140627/`, its folder back to its three registered files (`check_folders.py` exit 0), and
   **`tools/spent.py` counts `cut/*/run.json` too** (diff 2), the cut's empty JSON at 0.50 USD by spent.py's own rule
   for an unreadable file: the runner's spent stays 7.853686 USD with the cut counted. `preflight.sh`'s check that
   `spent.py` is panel 256's bytes no longer holds; `run.sh` does not run `preflight.sh`.

## diff 1, tools/make_verdict.py

```diff
--- tools/make_verdict.py (as registered before any session)
+++ tools/make_verdict.py (repaired)
@@ -20,7 +20,10 @@
 if missing:
     sys.exit(f'{len(missing)} graded folders hold no run.json yet: nothing made')
 def success(n):
-    j = json.load(open(os.path.join('s', n, 'run.json')))
+    try:
+        j = json.load(open(os.path.join('s', n, 'run.json')))
+    except (OSError, ValueError):
+        return False  # a run.json that cannot be read (a session cut by a signal) is no success
     return j.get('subtype') == 'success' and j.get('is_error') is False and j.get('terminal_reason') == 'completed'
 reports = {}
 for n in sorted(graded):
```

## diff 2, tools/spent.py

```diff
--- tools/spent.py (panel 256 bytes, as registered)
+++ tools/spent.py (after the cut)
@@ -2,7 +2,8 @@
 # be read counted as 0.50. usage: python3 -I tools/spent.py
 import glob, json
 total = 0.0
-for p in glob.glob('s/*/run.json') + glob.glob('verdict/run.json'):
+# and cut/*/run.json: a session cut mid-run (by a restart or an outage) and rerun keeps its spend counted (panel 261)
+for p in glob.glob('s/*/run.json') + glob.glob('verdict/run.json') + glob.glob('cut/*/run.json'):
     try:
         total += float(json.load(open(p)).get('total_cost_usd'))
     except Exception:
```

```
faea28d3aa308fbfbee70a4cfd381007338e920619c35e105741a317906dee09  tools/make_verdict.py
0f46d1878d433bc5d027ca431d57a0f4a21228ee0157d0c767caae03eaccab28  tools/spent.py
```
