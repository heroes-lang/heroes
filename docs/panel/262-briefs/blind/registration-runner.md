# Panel 262's blind runner: what changes against panel 261's, by diff, registered before any session

Written 2026-11-27 from 03:34 by the run's session (its brief: `.claude/worktrees/rebel-scratch/coord/blind262-brief.md`),
**before any session, the free preflight included**, as `blind/registration.md` § The area, the runner and every session
asks: *"adapted again by whoever starts this run, every change written as a `diff -u` into a `registration-runner.md`
beside this file before any session"*. The run is the author's authorisation of 2026-11-26 about 21:45 (*ok a 25
dollars*, the registration's head), **a ceiling of 25.00 USD for the whole run**, on the registration as the
coordinator amended it before any session (its last section, `registration.md` sha256 `84cd466e...a302`).

## The runner it starts from

**Panel 261's, as its run ran it**: the files of its area, `.claude/worktrees/261-llm-ergonomist/` (`run.sh`,
`preflight.sh`, `tools/`), which are panel 256's runner as panels 257, 259 and 261 adapted it (261's
`tools/registration-runner.md` holds each diff against panel 256's bytes and panel 259's; its two changes after its
sessions ran, `make_verdict.py` reading an unreadable `run.json` as no success and `spent.py` counting `cut/*/run.json`,
are in those files). Copied to `.claude/worktrees/blind262-scratch/runner-261/` before any change; their hashes, read
then (`shasum -a 256`):

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
faea28d3aa308fbfbee70a4cfd381007338e920619c35e105741a317906dee09  tools/make_verdict.py
d6fe89f5c79e6d938577e1352829788b4251fcc66c81bd5e896e924d095de63d  tools/read_runs.py
a1519e78f9154349e9d1c58183bfcf0f390fd3a6cba9cf795f472b2682711b1d  tools/reads.py
41aae8419d682a6d110d276998070924cb5fb259ed3993e234a3847e7a99d5c1  tools/scan.py
0f46d1878d433bc5d027ca431d57a0f4a21228ee0157d0c767caae03eaccab28  tools/spent.py
```

**Unchanged** (`cmp` equal to panel 261's): `after.py`, `brief-verdict.md`, `check_area.py`, `read_runs.py`,
`reads.py`, `scan.py` (each panel 256's bytes too) and `spent.py` (panel 261's, counting `cut/*/run.json`).

## What changes, in words

- **The area**: `.claude/worktrees/262-llm-ergonomist/`, drawn 03:34, the top of an empty git repository of its own
  (`git init`, `.gitignore` `*`, local user `reader` <`reader@invalid`>, no commit; `git check-ignore` names it ignored
  by this repository), `home/` and `tmp/` inside; `arms/` the three pinned arms (`arms-9b88d3e84/`'s bytes),
  `tasks/` the two tasks, `programs/` the 35 registered programs (never `h13.hero.registered`).
- **The folders** (`make_sessions.py`, `check_folders.py`): **three arms**, `now`, `adopted`, `rival`; **two tasks**,
  t1 and t2; five replicates: 30 folders, each holding exactly `brief.md` (`blind/brief-session.md`), `task.md`,
  `spec.md` (one arm, never named) and `programs/`: **t1's 28 files** (`prelude.h`, `sw.h`, `h1` to `h13`, `.h` and
  `.hero`) or **t2's 8** (`co.h`, `sw.h`, `esc.h`, `th.h`, `n1.hero` to `n4.hero`), every file at its registered hash
  (the last line per path of `blind/inputs.sha256`; the arms by the amendment's hashes). The labels: panel 256's draw,
  `secrets.SystemRandom().sample` over the letters standing in none of `now`, `adopted` and `rival` (14 letters),
  written to `key.txt` alone and printed nowhere.
- **The order, randomised by a seed written down before the first session** (the brief's step 2): **`SEED =
  136237622`**, drawn at 03:34:39 by `secrets.randbits(31)`; the 30 graded sessions are the `(task, arm, replicate)`
  triples sorted, shuffled by `random.Random(136237622)`, then named by their labels; **the verdict last**. Made at
  03:39: `order.txt` sha256 `c3480f86...db6b`, `key.txt` `faaac0cb...be6c` (630 files under `s/`, `check_folders.py`
  exit 0). Panel 256's fixed sequence reversed on even replicates is gone.
- **The caps and the ceiling** (`run.sh`, `preflight.sh`): **1.00 USD each t1 session, 0.60 each t2, 1.50 the
  verdict, margin 0.10, ceiling 25.00** (the registered ceiling the author's 25.00); the runner stops before any
  session where `spent + cap + 0.10` would pass 25.00, the verdict last, so the 30 graded run in every case (at their
  caps 24.00; the last starts at most at 23.40 + 0.60 + 0.10 = 24.10) and the verdict whenever the thirty spent 23.40
  or less. **The per-session limit 1,800 s** (261's 900), t1's thirteen programs being twice 261's input; a session
  past it is killed by its pid and counts as a stop, as before.
- **The warden and the registration pinned** (`run.sh`): `WARDEN` is `docs/panel/262-reports/spec-warden.md` in the
  main checkout (`d490152b...5c88`, the hash the synthesis read; panel262's copy the same bytes), its section `##
  Predictions registered before the blind run` **`4c47b7ad51bd7d8d205470fab85720ca077bef839cc49174006087e87e5a2bd0`**
  (28 lines, `awk` as `run.sh` reads it); `REGISTRATION_SHA` the amended registration's, `84cd466e...a302`, its copy
  `tools/registration.md`.
- **The session command: panel 261's, flag for flag**, which is panel 256's and the skill's § Procedure with `--bare`
  and `--model claude-opus-5-5`: `claude -p "<the registered prompt>" --bare --restricted --safe-mode
  --strict-mcp-config --model claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <cap> --output-format
  json < /dev/null`, under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area and
  `GIT_CEILING_DIRECTORIES` above it, the key from the trunk's `.env` (its length alone printed). The prompt is panel
  261's, unchanged.
- **One check added after the free preflight** (`run.sh`): the preflight's transcript's `api-request-shape` entry
  names the tools the session is offered; **the runner stops before any paid session unless they are `Read` alone**,
  the registration's assumption (panel 259's 31 of 31 sessions could not write `report.md`; panel 261's 33
  transcripts, read by this session at 03:30, name `Read` alone under 2.1.323). `claude --version` reads **`2.1.326
  (Claude Code)`** on the day (03:23), where the registration carried `2.1.323`.
- **`preflight.sh`**: the constants it checks (1.00, 0.60, 1.50, 0.10, 25.00; the 30 graded at their caps plus the
  margin under the ceiling); the three arms by hash, `now` against `git show 9b88d3e84:spec/heroes-spec.md`,
  `adopted` and `rival` each `patch` of its synthesis diff over `now`, `cmp` by hash; `spent.py` pinned at panel
  261's bytes, `after.py` and `reads.py` at panel 256's.
- **The verdict** (`make_verdict.py`, `brief-verdict-reports.md`): `verdict/` holds `spec-A.md` (`now`), `spec-B.md`
  (`adopted`), **`spec-C.md` (`rival`)**, `changes.diff` (A to B) and **`changes-C.diff`** (A to C), `task.md` t1
  then t2, `programs/` both tasks' 35 files (as panel 259's verdict held its t1's), and `reports/<label folder>.md`
  per successful graded session, never an arm's name; `brief.md` is panel 256's `brief-verdict.md` unchanged with
  the appendix, which says the third version, that the tasks ask what programs do rather than for a program, and
  lists the thirty reports. The registration's words: *"holding the arms' specs, the diffs, the tasks and the
  reports, never a key"*.
- **The key, the grades, the labels and D5** are fixed before any session in **`tools/key-grades.tsv`** (sha256
  `a3613cf8...78a8`, listed below, and whole below): per program the key's verdict and lines (one verdict on both graded
  platforms under `9b88d3e84`'s stage 2, plain and `--sanitize`: the amendment's rule 3), the words an `ABORTS` must
  name (the refusal list), its class under each arm (the synthesis's table, `h13` kept), and the rule that reads it
  (D1 `h1`, `h3`, `h13`, `n1`, `n2`, `n3`; D2 `h7`, `h9`, `h10`, `h11`; D3 `h2`, `h5`, `h6`, `h8`; D4 `h4`, `n4`; D5
  `h12`). The grades are § The grades with the synthesis's three label rules, graded after the run by the run's
  session, which is not blind (`key.txt` read for grading, as at panel 261); the two readers of the labels are the
  coordinator's, after the run, from packets this session prepares.

## The diffs, each against panel 261's file

### run.sh

```diff
--- 261/run.sh
+++ run.sh
@@ -1,10 +1,11 @@
 #!/bin/bash
-# Panel 261's blind seat (panel 256's runner as panels 257 and 259 adapted it, adapted again for panel 261: see
-# tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
-# time, never two, each from its own folder: the 30 graded ones (t1, t2, t3 under `now` and `adopted`, five each)
-# capped at 0.60 USD each, the verdict at 1.20, and stops before the sum plus the next session's cap plus a 0.10
-# margin would pass 20.00 USD, the author's ceiling for this run (2026-11-26, *ok a 20 dollars*, recorded in
-# docs/panel/261-briefs/llm-ergonomist.md § The size, the spend and the cap).
+# Panel 262's blind seat (panel 256's runner as panels 257, 259 and 261 adapted it, adapted again for panel 262: see
+# tools/registration-runner.md, docs/panel/262-briefs/blind/registration-runner.md, for every difference and its
+# diff). Runs the 31 sessions order.txt names, one at a time, never two, each from its own folder: the 30 graded ones
+# (t1 and t2 under `now`, `adopted` and `rival`, five each) capped at 1.00 USD each t1 session and 0.60 each t2, the
+# verdict at 1.50, and stops before the sum plus the next session's cap plus a 0.10 margin would pass 25.00 USD, the
+# author's ceiling for this run (2026-11-26 about 21:45, *ok a 25 dollars*, recorded at the head of
+# docs/panel/262-briefs/blind/registration.md).
 # A folder already holding run.json is skipped and its cost counted, so a stopped run resumes.
 #
 # NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
@@ -36,8 +37,12 @@
 # skipped. A session still running after $SESSION_LIMIT seconds is killed by its pid (never by name) and counts as
 # a stop. SIGINT and SIGTERM to the runner kill the session in flight and remove the lock.
 #
+# And, since panel 262 (the registration's own question, panel 259's 31 of 31 sessions without a Write tool): the
+# free preflight's transcript names the tools the session is offered (its api-request-shape entry), and the runner
+# stops before any paid session unless they are `Read` alone, the registration's assumption on the day.
+#
 # Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation:
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-llm-ergonomist/run.sh
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-llm-ergonomist/run.sh
 set -u
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
@@ -48,18 +53,19 @@
     *) echo "usage: run.sh [--preflight-only]"; exit 2 ;;
 esac
 
-CAP=0.60
-CAP_VERDICT=1.20
+CAP_T1=1.00
+CAP_T2=0.60
+CAP_VERDICT=1.50
 MARGIN=0.10
-CEILING=20.00
-REGISTERED_CEILING=20.00
-SESSION_LIMIT=900
+CEILING=25.00
+REGISTERED_CEILING=25.00
+SESSION_LIMIT=1800
 ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
-WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/261-reports/spec-warden.md
-WARDEN_SECTION_SHA=98a3455be021372392dc4e6bd0629d4a9d982a94f5d499cb7ccfdf6dd62bcc7f
-# The registration as the synthesis completed it (the arms' hashes, the codes, the caps), pinned here.
+WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/262-reports/spec-warden.md
+WARDEN_SECTION_SHA=4c47b7ad51bd7d8d205470fab85720ca077bef839cc49174006087e87e5a2bd0
+# The registration as the synthesis completed it and the coordinator amended it before any session, pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=9e241beea319d55dac31cae89f62a1aa2f98ff006372bc60814ea4fd0c34e285
+REGISTRATION_SHA=84cd466ecc7acca271afc0e3d79ba24dbab86692056a9552decbdaae4395a302
 HEADING='## Predictions registered before the blind run'
 CLI_TMP="/private/tmp/claude-$(id -u)"
 REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
@@ -157,7 +163,7 @@
     mkdir -p "$HERE/preflight"
     cp tools/brief-session.md "$HERE/preflight/brief.md"
     ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-before.txt" 2>&1
-    session "$HERE/preflight" "$CAP" "$REFUSED_KEY"
+    session "$HERE/preflight" "$CAP_T1" "$REFUSED_KEY"
     ls -la "$CLI_TMP" > "$HERE/preflight/cli-tmp-after.txt" 2>&1
     diff "$HERE/preflight/cli-tmp-before.txt" "$HERE/preflight/cli-tmp-after.txt" > "$HERE/preflight/cli-tmp-diff.txt"
     log "preflight: $CLI_TMP listing before and after differ in $(grep -c '^[<>]' "$HERE/preflight/cli-tmp-diff.txt") lines (other sessions on this Mac write there too; preflight/cli-tmp-diff.txt)"
@@ -179,6 +185,24 @@
     *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
     *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
 esac
+# The tools the preflight session was offered (its transcript's api-request-shape): Read alone, as the registration assumes.
+pre_tools=$(python3 -I -c '
+import glob, json, sys
+names = set()
+for p in glob.glob("home/.claude/projects/*-preflight/*.jsonl"):
+    for line in open(p, errors="replace"):
+        try:
+            e = json.loads(line)
+        except ValueError:
+            continue
+        if e.get("type") == "api-request-shape":
+            names.update(t.get("name") for t in e.get("shape", {}).get("tools", []))
+print(",".join(sorted(n for n in names if n)) or "none read")')
+log "preflight: the tools offered: $pre_tools"
+if [ "$pre_tools" != "Read" ]; then
+    echo "the preflight session was offered $pre_tools, not Read alone: the registration assumes no Write tool; nothing more run"
+    exit 2
+fi
 if [ "$PREFLIGHT_ONLY" = 1 ]; then
     log "preflight only: stopped before the key is read; no paid session run"
     exit 0
@@ -195,7 +219,12 @@
 passed_registered=0
 while read -r folder <&3; do
     [ -n "$folder" ] || continue
-    if [ "$folder" = verdict ]; then d="$HERE/verdict"; cap=$CAP_VERDICT; else d="$HERE/s/$folder"; cap=$CAP; fi
+    case "$folder" in
+        verdict) d="$HERE/verdict"; cap=$CAP_VERDICT ;;
+        t1-*) d="$HERE/s/$folder"; cap=$CAP_T1 ;;
+        t2-*) d="$HERE/s/$folder"; cap=$CAP_T2 ;;
+        *) log "$folder names no task: runner stopped"; STOPPED=3; break ;;
+    esac
     if [ -f "$d/run.json" ]; then
         echo "$folder already run, its cost counted"
         continue
```

### preflight.sh

```diff
--- 261/preflight.sh
+++ preflight.sh
@@ -1,5 +1,5 @@
 #!/bin/bash
-# Panel 261's preflight (panel 259's, its arms, tasks, folders and arm check changed for panel 261): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
+# Panel 262's preflight (panel 261's, its arms, tasks, folders, caps, ceiling and arm checks changed for panel 262): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
 #   bash preflight.sh            the static checks alone: no `claude -p`, no API call, no key read except its LENGTH
 #   bash preflight.sh --session  the static checks, then `bash run.sh --preflight-only`: the one free session of
 #                                run.sh (panel 256's), whose key is a string the API refuses (cost 0 measured at
@@ -21,13 +21,14 @@
 
 chk "run.sh parses (bash -n)" 'bash -n run.sh'
 chk "no run.lock stands" '[ ! -e run.lock ]'
-CAP=$(const CAP); CAPV=$(const CAP_VERDICT); MARGIN=$(const MARGIN); CEILING=$(const CEILING)
-echo "      constants of run.sh: cap $CAP, verdict cap $CAPV, margin $MARGIN, ceiling $CEILING (registered ceiling $(const REGISTERED_CEILING), the author's), per-session limit $(const SESSION_LIMIT) s"
-chk "caps are the registered 0.60 and 1.20, margin 0.10" '[ "$CAP" = 0.60 ] && [ "$CAPV" = 1.20 ] && [ "$MARGIN" = 0.10 ]'
-chk "ceiling is 20.00, the author's for the whole run" '[ "$CEILING" = 20.00 ]'
-WORST=$(python3 -I -c "print(f'{30*$CAP + $CAPV:.2f}')")
-echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 7 to 12, an inference, unrun"
-chk "the worst case stays under the ceiling, margin included (every session can run)" "python3 -I -c 'import sys; sys.exit(0 if $WORST + $MARGIN <= $CEILING else 1)'"
+CAP1=$(const CAP_T1); CAP2=$(const CAP_T2); CAPV=$(const CAP_VERDICT); MARGIN=$(const MARGIN); CEILING=$(const CEILING)
+echo "      constants of run.sh: t1 cap $CAP1, t2 cap $CAP2, verdict cap $CAPV, margin $MARGIN, ceiling $CEILING (registered ceiling $(const REGISTERED_CEILING), the author's), per-session limit $(const SESSION_LIMIT) s"
+chk "caps are the registered 1.00 (t1), 0.60 (t2) and 1.50 (verdict), margin 0.10" '[ "$CAP1" = 1.00 ] && [ "$CAP2" = 0.60 ] && [ "$CAPV" = 1.50 ] && [ "$MARGIN" = 0.10 ]'
+chk "ceiling is 25.00, the author's for the whole run" '[ "$CEILING" = 25.00 ]'
+GRADED=$(python3 -I -c "print(f'{15*$CAP1 + 15*$CAP2:.2f}')")
+WORST=$(python3 -I -c "print(f'{15*$CAP1 + 15*$CAP2 + $CAPV:.2f}')")
+echo "      worst case, every session to its cap: the 30 graded $GRADED USD, with the verdict $WORST; the verdict runs only if the thirty spent at most $(python3 -I -c "print(f'{$CEILING - $CAPV - $MARGIN:.2f}')"); the registration's estimate about 13 to 18, an inference, unrun"
+chk "the 30 graded sessions at their caps stay under the ceiling, margin included (every graded session can run)" "python3 -I -c 'import sys; sys.exit(0 if $GRADED + $MARGIN <= $CEILING else 1)'"
 
 # the runner's own text: every flag of the registered command, no tool beyond Read and Write
 for f in '--bare' '--restricted' '--safe-mode' '--strict-mcp-config' '--model claude-opus-5-5' '--tools "Read,Write"' '--allowedTools "Read,Write"' \
@@ -46,15 +47,20 @@
 done < tools/inputs.sha256
 REG_SHA=$(grep -E '^REGISTRATION_SHA=' run.sh | cut -d= -f2)
 chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
-chk "every arm's full sha256 stands in the registration's own table" \
-    'for a in now adopted; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
-for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 10c2a8f93476d39b' 'brief-verdict.md 52d37bb04ac8ab20'; do
+chk "every arm's full sha256 stands in the registration (its amendment before any session)" \
+    'for a in now adopted rival; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
+for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'brief-verdict.md 52d37bb04ac8ab20' 'after.py e5b8bd741da31af5' 'reads.py a1519e78f9154349'; do
     tf=${pair% *}; tp=${pair#* }
     chk "tools/$tf is panel 256's registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
 done
-chk "tools/synthesis-adopted.diff is the synthesis's diff (202d8a03a086...)" '[ "$(sha tools/synthesis-adopted.diff)" = 202d8a03a086281da72f4da493534f55df4d30442e687f47fbbcf37858044582 ]'
-chk "the adopted arm is the frozen spec with tools/synthesis-adopted.diff applied by patch (six hunks, the synthesis's text)" \
-    'mkdir -p check && rm -f check/adopted-applied.md && patch -s -o check/adopted-applied.md arms/spec-now.md tools/synthesis-adopted.diff && [ "$(sha check/adopted-applied.md)" = "$(sha arms/spec-adopted.md)" ]'
+chk "tools/spent.py is panel 261's (0f46d1878d433bc5..., it counts cut/*/run.json too)" '[ "$(sha tools/spent.py | cut -c1-16)" = 0f46d1878d433bc5 ]'
+chk "tools/synthesis-adopted.diff is the synthesis's diff (909fb9085d92...)" '[ "$(sha tools/synthesis-adopted.diff)" = 909fb9085d92c8cd08e6f1b7fe8de672945fc84bfe8c94b238cb59879a90a665 ]'
+chk "tools/synthesis-rival.diff is the synthesis's diff (6bb97f71aee8...)" '[ "$(sha tools/synthesis-rival.diff)" = 6bb97f71aee879e749cc562d3ba1aaafcfbbba4032a917d237b3178d022e059d ]'
+chk "arms/spec-now.md is main's spec at the pinned 9b88d3e84 (736b534c852e...)" '[ "$(sha arms/spec-now.md)" = 736b534c852e39aff9bdde0b7e6008a836b25f74b4110b017cce145bd178efd3 ] && [ "$(git -C /Users/joseph/Temp/heroes/heroes-lang show 9b88d3e84:spec/heroes-spec.md | shasum -a 256 | cut -c1-64)" = "$(sha arms/spec-now.md)" ]'
+for a in adopted rival; do
+    chk "the $a arm is the pinned spec with tools/synthesis-$a.diff applied by patch" \
+        'mkdir -p check && rm -f check/$a-applied.md && patch -s -o check/$a-applied.md arms/spec-now.md tools/synthesis-$a.diff && [ "$(sha check/$a-applied.md)" = "$(sha arms/spec-$a.md)" ]'
+done
 chk "tools/registration-runner.md (the runner's diffs against panel 256's bytes) exists" '[ -s tools/registration-runner.md ]'
 
 # the warden's registered predictions, pinned
@@ -64,7 +70,7 @@
 
 # the folders, the order, the area
 chk "order.txt and key.txt stand" '[ -s order.txt ] && [ -s key.txt ]'
-chk "check_folders: 31 in order, 30 folders holding brief.md, spec.md and task.md alone at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
+chk "check_folders: 31 in order, 30 folders holding brief.md, spec.md, task.md and their task's programs alone at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
 chk "check_area: the area is the top of its own empty git repository, reader <reader@invalid>, HOME and TMPDIR inside" 'python3 -I tools/check_area.py'
 chk "git log in the area says no commit" 'git log 2>&1 | grep -q "does not have any commits yet"'
 chk "no CLAUDE.md, AGENTS.md, MEMORY.md or memory folder in the area (git's own folder left out)" \
@@ -72,7 +78,7 @@
 chk "no CLAUDE.md or memory under home/ (the CLI's own home for the sessions)" \
     '[ -z "$(find home \( -name CLAUDE.md -o -name MEMORY.md -o -type d -name memory \) -print 2>/dev/null | head -1)" ]'
 echo "      spent so far, by tools/spent.py over the run.json files: $(python3 -I tools/spent.py) USD"
-chk "the ceiling still leaves room for the next session" "python3 -I -c 'import sys; sys.exit(0 if $(python3 -I tools/spent.py) + $CAP + $MARGIN <= $CEILING else 1)'"
+chk "the ceiling still leaves room for the next session" "python3 -I -c 'import sys; sys.exit(0 if $(python3 -I tools/spent.py) + $CAP1 + $MARGIN <= $CEILING else 1)'"
 echo "      CLAUDE.md or AGENTS.md ABOVE the area (they cannot be absent: the area stands inside the repository's root):"
 d=$(dirname "$HERE"); while [ "$d" != / ]; do for f in CLAUDE.md AGENTS.md; do [ -e "$d/$f" ] && echo "        $d/$f"; done; d=$(dirname "$d"); done
 echo "      what keeps them out is --bare (no CLAUDE.md discovery), the area being the top of its own repository, HOME inside the area; the PROOF is the free preflight's scan: claude-md-or-memory=no"
@@ -95,7 +101,8 @@
         d=verdict; cap=$CAPV
         [ -d verdict ] && extra="" || extra=" (built after the 30 by tools/make_verdict.py, reports of the successful sessions only)"
     else
-        d="s/$name"; cap=$CAP; extra=""
+        d="s/$name"; extra=""
+        case "$name" in t1-*) cap=$CAP1 ;; *) cap=$CAP2 ;; esac
     fi
     if [ -f "$d/run.json" ]; then st="already run"; else st="to run"; fi
     if [ -d "$d" ]; then fc=$(find "$d" -type f | wc -l | tr -d ' '); else fc=0; fi
```

### make_sessions.py

```diff
--- 261/tools/make_sessions.py
+++ tools/make_sessions.py
@@ -1,53 +1,64 @@
-# Panel 261's blind seat (panel 256's make_sessions.py as panels 257 and 259 changed it; for 261 the arms, the tasks
-# and the folders' files changed, docs/panel/261-briefs/blind/registration-runner.md): draws the arms' labels, then
-# makes one session folder per task, arm and replicate (t1, t2 and t3, `now` and `adopted`, 5 each: 30 folders), each
-# holding brief.md, task.md and spec.md and nothing else (a write task holds no programs/), and writes key.txt and
-# order.txt beside s/, never inside it. THE DRAW: the two labels are sampled from a pool of letters with panel 256's
-# method, secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place
-# the draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
-# the six cells in a fixed sequence, reversed on the even replicates, the verdict last.
-# The verdict's folder is NOT made here: it is built after the 30 by tools/make_verdict.py.
+# Panel 262's blind seat (panel 261's make_sessions.py, changed for panel 262 as
+# docs/panel/262-briefs/blind/registration-runner.md registers: three arms, two tasks, programs/, the order by a seed):
+# draws the arms' labels, then makes one session folder per task, arm and replicate (t1 and t2, `now`, `adopted` and
+# `rival`, 5 each: 30 folders), each holding brief.md, task.md, spec.md and programs/ (t1's 28 files or t2's 8) and
+# nothing else, and writes key.txt and order.txt beside s/, never inside it. THE DRAW: the three labels are sampled
+# from a pool of letters with panel 256's method, secrets.SystemRandom (the operating system's entropy, no seed, not
+# reproducible); key.txt is the only place the draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS
+# they name. THE ORDER (panel 262's brief, step 2): the 30 graded sessions in an order randomised by random.Random(SEED)
+# over the cells sorted by (task, arm, replicate), SEED written in the runner's registration before the first session;
+# the verdict last. The verdict's folder is NOT made here: it is built after the 30 by tools/make_verdict.py.
 # Refuses to run over an existing s/, verdict/, key.txt or order.txt.
 # usage (from the area): python3 -I tools/make_sessions.py
-import hashlib, os, secrets, sys
+import hashlib, os, random, secrets, sys
 assert os.path.isfile('tools/make_sessions.py'), 'run from the blind area'
 for p in ('s', 'verdict', 'key.txt', 'order.txt'):
     if os.path.exists(p):
         sys.exit(f'{p} exists: nothing made')
+SEED = 136237622  # written in docs/panel/262-briefs/blind/registration-runner.md before the first session
 REPLICATES = 5
-ARMS = ['now', 'adopted']
+ARMS = ['now', 'adopted', 'rival']
 SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
-TASKS = ['t1', 't2', 't3']
-# no letter that stands in an arm's name (now, adopted), so a label says nothing of its arm
-POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted')]
+TASKS = ['t1', 't2']
+# t1's 28 files and t2's 8, as blind/registration.md § Where the programs come from and the tasks name them
+T1_PROGRAMS = ['prelude.h', 'sw.h'] + [f'h{i}.{x}' for i in range(1, 14) for x in ('h', 'hero')]
+T2_PROGRAMS = ['co.h', 'sw.h', 'esc.h', 'th.h'] + [f'n{i}.hero' for i in range(1, 5)]
+PROGRAMS = {'t1': T1_PROGRAMS, 't2': T2_PROGRAMS}
+assert len(T1_PROGRAMS) == 28 and len(T2_PROGRAMS) == 8
+# no letter that stands in an arm's name (now, adopted, rival), so a label says nothing of its arm
+POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set(''.join(ARMS))]
 def sha(b): return hashlib.sha256(b).hexdigest()
 def read(p): return open(p, 'rb').read()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
 for f, h in reg.items():
     assert sha(read(f)) == h, f'{f} is not the registered file'
+for t in TASKS:
+    for p in PROGRAMS[t]:
+        assert f'programs/{p}' in reg, f'programs/{p} is not registered'
 labels = secrets.SystemRandom().sample(POOL, len(ARMS))
 draw = dict(zip(ARMS, labels))
 brief = read('tools/brief-session.md')
-rows, cells = [], []
+rows, units = [], []
 for task in TASKS:
     for arm in ARMS:
-        cells.append((task, arm))
         for n in range(1, REPLICATES + 1):
+            units.append((task, arm, n))
             name = f'{task}-{draw[arm]}-{n}'
             d = os.path.join('s', name)
             files = {'brief.md': brief, 'task.md': read(f'tasks/task-{task}.md'), 'spec.md': read(SPEC[arm])}
-            assert sorted(files) == ['brief.md', 'spec.md', 'task.md']
+            for p in PROGRAMS[task]:
+                files[f'programs/{p}'] = read(f'programs/{p}')
+            assert len(files) == 3 + len(PROGRAMS[task])
             for f, b in files.items():
                 os.makedirs(os.path.dirname(os.path.join(d, f)), exist_ok=True)
                 open(os.path.join(d, f), 'wb').write(b)
             rows.append(f'{name} {arm} ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(files.items())))
-order = []
-for r in range(1, REPLICATES + 1):
-    seq = cells if r % 2 == 1 else list(reversed(cells))
-    order += [f'{t}-{draw[a]}-{r}' for t, a in seq]
-order.append('verdict')
+seq = sorted(units)
+random.Random(SEED).shuffle(seq)
+order = [f'{t}-{draw[a]}-{n}' for t, a, n in seq] + ['verdict']
 head = ('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) +
         '\n# drawn by tools/make_sessions.py: secrets.SystemRandom().sample over the letters standing in none of the arms\' names; '
+        f'the order random.Random({SEED}).shuffle of the 30 (task, arm, replicate) sorted, the verdict last; '
         'the verdict row is appended by tools/make_verdict.py after the 30 sessions\n')
 open('key.txt', 'w').write(head + '\n'.join(rows) + '\n')
 open('order.txt', 'w').write('\n'.join(order) + '\n')
```

### check_folders.py

```diff
--- 261/tools/check_folders.py
+++ tools/check_folders.py
@@ -1,13 +1,19 @@
-# Before any spend (panel 256's check_folders.py, as changed for panel 257, used by panel 259, and changed for panel
-# 261: two arms, three tasks, no programs/): order.txt names 31 folders (the 30 graded ones, 5 per task and arm, then
-# `verdict` last), each in key.txt; every graded folder not yet run holds exactly `brief.md`, `task.md` and `spec.md`,
-# at their registered hashes, and each of those hashes is the one tools/inputs.sha256 registers (the arm's spec, the
-# task) or tools/brief-session.md's; no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
-# before then (`--verdict` asks only of it, after tools/make_verdict.py).
+# Before any spend (panel 256's check_folders.py, as changed for panels 257, 259 and 261, and changed for panel 262:
+# three arms, two tasks, programs/): order.txt names 31 folders (the 30 graded ones, 5 per task and arm, then `verdict`
+# last), each in key.txt; every graded folder not yet run holds exactly `brief.md`, `task.md`, `spec.md` and its
+# task's programs under `programs/` (t1's 28 files, t2's 8), at their registered hashes, and each of those hashes is
+# the one tools/inputs.sha256 registers (the arm's spec, the task, the program) or tools/brief-session.md's; no folder
+# of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for before then
+# (`--verdict` asks only of it, after tools/make_verdict.py).
 # usage: python3 -I tools/check_folders.py [--verdict]
 import hashlib, os, sys
-ARMS = ('now', 'adopted')
-GRADED_FILES = ['brief.md', 'spec.md', 'task.md']
+ARMS = ('now', 'adopted', 'rival')
+TASKS = ('t1', 't2')
+T1_PROGRAMS = ['prelude.h', 'sw.h'] + [f'h{i}.{x}' for i in range(1, 14) for x in ('h', 'hero')]
+T2_PROGRAMS = ['co.h', 'sw.h', 'esc.h', 'th.h'] + [f'n{i}.hero' for i in range(1, 5)]
+PROGRAMS = {'t1': T1_PROGRAMS, 't2': T2_PROGRAMS}
+def graded_files(task):
+    return sorted(['brief.md', 'spec.md', 'task.md'] + [f'programs/{p}' for p in PROGRAMS[task]])
 def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
 key = {}
@@ -26,7 +32,7 @@
         continue
     task = name.split('-')[0]
     cells[(task, key[name][0])] = cells.get((task, key[name][0]), 0) + 1
-want = {(t, a): 5 for t in ('t1', 't2', 't3') for a in ARMS}
+want = {(t, a): 5 for t in TASKS for a in ARMS}
 if cells != want:
     bad.append(f'the cells are not 5 sessions of each task and arm: {sorted(cells.items())}')
 brief_sha = sha('tools/brief-session.md')
@@ -36,6 +42,7 @@
     if f == 'spec.md': return reg[f'arms/spec-{arm}.md']
     if f == 'task.md': return reg[f'tasks/task-{task}.md']
     if f == 'brief.md': return brief_sha
+    if f.startswith('programs/'): return reg.get(f)
     return None
 def listing(d):
     return sorted(os.path.relpath(os.path.join(r, x), d) for r, _, fs in os.walk(d) for x in fs)
@@ -54,8 +61,8 @@
     if name != 'verdict' and arm not in ARMS:
         bad.append(f'{name} names the arm {arm!r}'); continue
     have = listing(d)
-    if name != 'verdict' and have != GRADED_FILES:
-        bad.append(f'{name} does not hold exactly brief.md, spec.md and task.md: {have}'); continue
+    if name != 'verdict' and have != graded_files(name.split('-')[0]):
+        bad.append(f'{name} does not hold exactly brief.md, spec.md, task.md and its task\'s programs: {have}'); continue
     if have != sorted(files):
         bad.append(f'{name} holds other files: {have}'); continue
     for f, h in files.items():
```

### make_verdict.py

```diff
--- 261/tools/make_verdict.py
+++ tools/make_verdict.py
@@ -1,11 +1,13 @@
-# Panel 261's blind seat (panel 259's make_verdict.py, changed for panel 261 as
-# docs/panel/261-briefs/blind/registration-runner.md asks: three tasks, two arms, no programs/): builds the verdict
-# session's folder after the 30 graded sessions: spec-A.md is `now`, spec-B.md is `adopted` (the synthesis's text),
-# changes.diff their unified diff, task.md t1 then t2 then t3, separated by a line `---`, reports/ one file per
-# successful graded session named by its folder (its label, never its arm), report.md where the session wrote one,
-# else the result.md tools/after.py kept; no programs/. brief.md is tools/brief-verdict.md (panel 256's registered bytes) with
-# tools/brief-verdict-reports.md appended, then the thirty report names listed, which panel 257's verdict lacked (it
-# could not list reports/ with Read alone and read none of them). Appends the verdict's row to key.txt.
+# Panel 262's blind seat (panel 261's make_verdict.py, changed for panel 262 as
+# docs/panel/262-briefs/blind/registration-runner.md registers: three arms, two tasks, programs/): builds the verdict
+# session's folder after the 30 graded sessions, as blind/registration.md § The size says ("its folder built after the
+# 30, holding the arms' specs, the diffs, the tasks and the reports, never a key"): spec-A.md is `now`, spec-B.md is
+# `adopted` (the resolution's text), spec-C.md is `rival` (the second-ranked text), changes.diff the unified diff of A
+# to B and changes-C.diff of A to C, task.md t1 then t2, separated by a line `---`, programs/ both tasks' programs (as
+# panel 259's verdict held its t1's), reports/ one file per successful graded session named by its folder (its label,
+# never its arm), report.md where the session wrote one, else the result.md tools/after.py kept. brief.md is
+# tools/brief-verdict.md (panel 256's registered bytes) with tools/brief-verdict-reports.md appended, then the thirty
+# report names listed. Appends the verdict's row to key.txt.
 # Refuses to run over an existing verdict/, or before every graded folder holds a run.json.
 # usage (from the area): python3 -I tools/make_verdict.py
 import difflib, hashlib, json, os, sys
@@ -37,13 +39,20 @@
             break
     else:
         print(f'{n} left neither report.md nor result.md: nothing of it in the verdict')
-a_txt, b_txt = read('arms/spec-now.md'), read('arms/spec-adopted.md')
-diff = ''.join(difflib.unified_diff(a_txt.decode().splitlines(True), b_txt.decode().splitlines(True),
-                                    fromfile='spec-A.md', tofile='spec-B.md')).encode()
-task = read('tasks/task-t1.md') + b'\n---\n\n' + read('tasks/task-t2.md') + b'\n---\n\n' + read('tasks/task-t3.md')
+a_txt, b_txt, c_txt = read('arms/spec-now.md'), read('arms/spec-adopted.md'), read('arms/spec-rival.md')
+def udiff(x, y, to):
+    return ''.join(difflib.unified_diff(x.decode().splitlines(True), y.decode().splitlines(True),
+                                        fromfile='spec-A.md', tofile=to)).encode()
+task = read('tasks/task-t1.md') + b'\n---\n\n' + read('tasks/task-t2.md')
 names = '\n'.join(f'- `{r}`' for r in sorted(reports)).encode()
 brief = read('tools/brief-verdict.md') + b'\n' + read('tools/brief-verdict-reports.md') + b'\n' + names + b'\n'
-files = {'brief.md': brief, 'spec-A.md': a_txt, 'spec-B.md': b_txt, 'changes.diff': diff, 'task.md': task}
+files = {'brief.md': brief, 'spec-A.md': a_txt, 'spec-B.md': b_txt, 'spec-C.md': c_txt,
+         'changes.diff': udiff(a_txt, b_txt, 'spec-B.md'), 'changes-C.diff': udiff(a_txt, c_txt, 'spec-C.md'),
+         'task.md': task}
+T1_PROGRAMS = ['prelude.h', 'sw.h'] + [f'h{i}.{x}' for i in range(1, 14) for x in ('h', 'hero')]
+T2_PROGRAMS = ['co.h', 'esc.h', 'th.h'] + [f'n{i}.hero' for i in range(1, 5)]
+for p in T1_PROGRAMS + T2_PROGRAMS:
+    files[f'programs/{p}'] = read(f'programs/{p}')
 files.update(reports)
 for f, b in files.items():
     os.makedirs(os.path.dirname(os.path.join('verdict', f)) or 'verdict', exist_ok=True)
```

### brief-verdict-reports.md

```diff
--- 261/tools/brief-verdict-reports.md
+++ tools/brief-verdict-reports.md
@@ -1,9 +1,14 @@
 ## Also in this directory
 
-- `task.md` holds three tasks, the first, then the second after a line `---`, then the third after
-  another; do each under each specification.
-- `reports/`: thirty reports by other readers, each of whom read one of the two versions of the
-  specification (`spec-A.md` or `spec-B.md`; which version a report's reader had is not said) and did
-  one of the three tasks, with no file but that version and the task. Read every report before you
-  write your verdict, and say in `experiment` where the readers agree with your own answers and where
-  they do not. You cannot list a folder, so the reports' names are given here, all thirty:
+- `spec-C.md`: a third version of the specification, the whole language, which differs from `spec-A.md`
+  only where `changes-C.diff` shows. Do the tasks under it too, and say in `verdict` also whether a model
+  would predict these programs better from `spec-B.md` or from `spec-C.md`, and why.
+- `task.md` holds two tasks, the first then the second after the line `---`; each asks what programs
+  do when they run, not for a program to be written: under each specification, answer what each task
+  asks, and in `experiment` give those answers in place of a program. Their programs are in `programs/`.
+- `reports/`: thirty reports by other readers, each of whom read one of the three versions of the
+  specification (`spec-A.md`, `spec-B.md` or `spec-C.md`; which version a report's reader had is not
+  said) and did one of the two tasks, with no file but that version, the task and its programs. Read
+  every report before you write your verdict, and say in `experiment` where the readers agree with your
+  own answers and where they do not. You cannot list a folder, so the reports' names are given here, all
+  thirty:
```

### inputs.sha256

```diff
--- 261/tools/inputs.sha256
+++ tools/inputs.sha256
@@ -1,6 +1,41 @@
-arms/spec-now.md 3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f
-arms/spec-adopted.md e9b0c819cce1c91ec74e43f2197bdd490239ccacae53af7ca9e943052bb2758c
-tasks/task-t1.md 1695341319fbd053b581589cb0c3330e0f002f642edc6a4dc9f57bb3b521abe8
-tasks/task-t2.md 2b952b79348c1b9b8135c9be3bffb6cef33375086588e849a8ef8c48ca424c45
-tasks/task-t3.md cea2d2b6e5f03a40226d5820aaa56c887fcd5cdc27152bd9b9cf1022904f8cfd
-tools/brief-session.md e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222
+arms/spec-now.md 736b534c852e39aff9bdde0b7e6008a836b25f74b4110b017cce145bd178efd3
+arms/spec-adopted.md d66dfd307384d3fa7b28cc72e757cc8d9e34680f260a92eaa5732c5870267b17
+arms/spec-rival.md ecfb9931f987e29b25d72c15edb52796ff940cd4a3936b49070704c250214d7c
+tasks/task-t1.md fdbd7d624585c125452b9fad979808318d5fbeaffc858f7eb4738738d37a0578
+tasks/task-t2.md 617de0368efb223cfb5907b52d005b1954153f72f89d695eee346f82cca1e00c
+tools/brief-session.md 01855da511c5b0da778b0794175646721ec88e767920380f4b853844bc8c5fc8
+programs/co.h ed909982b316b77d2a8a99d6871d7dc049ad6c82aed642ba602c99b048560c8c
+programs/esc.h 9a3b08b7fe1ca43150858c6515766decb5685b7f2b2ffded853c430345efd673
+programs/h1.h 9cadabccde1baaad3dfa8173cda2656294b9740377ee84ba514d493b2eae5388
+programs/h1.hero 9d51131a164bfaaca5fe0a818b7badc15e1fc09a36d57dcf543fbb40de091965
+programs/h10.h 04c23df04dab8658cbbb94cdc30a96a860ab25a4433f356e94be416e4ae3df3f
+programs/h10.hero eab794d76acd64816b8c9a92f15b56f2dfc732e2841c77466685298b46616965
+programs/h11.h fc13204482faf33310054f57fb9eb388abcc652e540385dc84714d0f7e0e95b3
+programs/h11.hero 30cd1d6cfcd4a5ddf79096311b3db64aa7a7a17508dffc0c2cb618960aa55adb
+programs/h12.h 86f5b5e61d67cd822a241121b8025c6e60f88290f2c8d615f970c59413106882
+programs/h12.hero ee9a56be09c4a931ae9f55474e9e735832aefe0d4101a9494ac8bf004078f6a2
+programs/h13.h 54e00d252abad5ee629b9338c144ccc572b2a394943104efa5df9f7572f61d4c
+programs/h13.hero 06be86280a074048acbfff1880468051d8096b36d1dff33a3a436b25b533b42e
+programs/h2.h 3e07d6f97b067e8e909696c82a9bd4e1fd55351b168ac4d24edd97545d0065f2
+programs/h2.hero bd8e1259f958e8fb3d0d03aacf8edc2a80803e5be88baea3477cd34f288a1e36
+programs/h3.h f1a08b31e9d15ddb1ba66013fce918691a7b4a752d5f1dd217cf1cacf9625e0e
+programs/h3.hero e844fc0880c0620e61f8fcd7174df7bdcd123dfcbe0050d9f93bcf4e852a46d2
+programs/h4.h 60001822e49272bb81360cad33d0700d9e908dee942a96d34527773bd99f0bc1
+programs/h4.hero f52d7326775c8984daee57bc79dc9bb98d90cce3c1f972d6cc98eae705190b0a
+programs/h5.h 4b72842a94b5a54b78fe14071c0c6e5a19000e899d8722583f360b1b24cb90cc
+programs/h5.hero 6d59c7285553ca56a23ceedf2cc3f50959a6fcc588ef37473831fadca66eb8a1
+programs/h6.h 79c5a0f346f1ca81b121e4cdc4cb26e109ab31e75d7b193e22906ab181e11da3
+programs/h6.hero d07a027747649a370cba4a2119f01113dd045458ba40a6b539cf7757fcc56ac3
+programs/h7.h b39c491c1ee6bfa7c9d8697ed6d6b665d4348a1babfe36f42dfe38ea25d5132a
+programs/h7.hero cc0f1008bc46c168ba985a1ad88844c43fb5079b49d8c70152d208e150128ba3
+programs/h8.h f1aedc3e308b2c1c7e8af6b372659cda3e3c2c902fd3021a119cf0d94f14d1ad
+programs/h8.hero 69dd7278e0e0a13561c43aa2755965a099934c6fab121d5459396de0c3743d94
+programs/h9.h 2614217c1c37662721b6d514377625251eae866aa8dcb98ea17476806e102a4d
+programs/h9.hero 51ff92dc650df777a6bcf4888f5825c833fdd990c6d2c878bfcfeec4ed51752e
+programs/n1.hero a613731fa1636387264eae1ce85dfec4e53ec8ef599ed3d850928307412d5731
+programs/n2.hero 9e7f45f07f37e6dc1a6d68c0ddf50a283a4591d124d4378cbf301637abf3cb70
+programs/n3.hero 62f3c22493f2358998e441d8c42c6c26db4e01b8e6837112657a76ba49c01e8b
+programs/n4.hero ddb1b133f260bba09405e547db540eb64333b161713beb9b6e409ecf50f2a599
+programs/prelude.h 95d7d7d6618ae288cb943352a9ecc39fa18bb787492569ae8f761f37e8a1f9eb
+programs/sw.h e2c84c66eab40ee72cf3c690dd813bc1274678e26b5b30f209571a1daa2ac135
+programs/th.h d92d1489b42efc8f2f6d19b836f207c07652f2c70d1383007131bf3b57ba9bdc
```

### brief-session.md

```diff
--- 261/tools/brief-session.md
+++ tools/brief-session.md
@@ -1,8 +1,8 @@
 # Your brief
 
 You judge one thing: whether the language's specification, read with no other file, lets a language
-model predict what a program does when it is built and run, and write the program its author meant,
-on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**
+model predict what a program does when it is built and run. Your verdict is an experiment, not an
+opinion: **predict from the text, and say where the text decided and where you did.**
 
 ## Your inputs, and nothing else
 
@@ -10,22 +10,21 @@
 
 - `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
+- `programs/`: the programs and headers the task names.
 
 ## Your method
 
-Read the task and do what it asks. Write every file you write whole, with its path. For every place
-where the specification left you a choice, say the choice you made and what the other would produce:
-a program the compiler refuses, a program that stops with an error when run, a program that never
-ends, or a program that runs and gives a different answer than intended.
+Read the task and do what it asks, program by program. For every place where the specification left
+you a choice, say the choice you made and what the other would produce: a program that runs, a
+program that stops with an error when run, or a program the text does not decide.
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, in this order and under these names:
-  - `main.hero`: the program, whole, in one fenced code block, and no other code block in `answer`;
-  - `prints`: every line you expect the program to print, in order;
-  - `compiles`: *yes* or *no*, and if no, which line and why;
-  - `parameters`: for each function the program binds, each of its parameters and why it is written
-    so.
+- `answer`: for each program the task names, in its order, under the program's name:
+  - `verdict`: the task's word, and the lines it prints where the task asks for them;
+  - `abort`: the sentence the abort says and what you would change, or `none`;
+  - `confidence`: 1 to 5;
+  - `sections`: the numbered sections of the specification your verdict rests on.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
```

### brief-session.md, against panel 256's committed bytes

```diff
--- 256/brief-session.md
+++ tools/brief-session.md
@@ -1,31 +1,32 @@
 # Your brief
 
 You judge one thing: whether the language's specification, read with no other file, lets a language
-model predict what a program does when it is built and run, and write the program its author meant,
-on the first try. Your verdict is an experiment, not an opinion: **write the code, don't opine.**
+model predict what a program does when it is built and run. Your verdict is an experiment, not an
+opinion: **predict from the text, and say where the text decided and where you did.**
 
 ## Your inputs, and nothing else
 
 The files in this directory are your whole input. Read no other file, and do not ask for one.
 
-- `heroes-spec.md`: the language's specification, the whole language.
+- `spec.md`: the language's specification, the whole language.
 - `task.md`: one task.
+- `programs/`: the programs and headers the task names.
 
 ## Your method
 
-Read the task and do what it asks. Write every file you write whole, with its path. For every place
-where the specification left you a choice, say the choice you made and what the other would produce:
-a program the compiler refuses, a program that stops with an error when run, a program that never
-ends, or a program that runs and gives a different answer than intended.
+Read the task and do what it asks, program by program. For every place where the specification left
+you a choice, say the choice you made and what the other would produce: a program that runs, a
+program that stops with an error when run, or a program the text does not decide.
 
 ## Your report, `report.md` in this directory, in exactly this structure
 
-- `answer`: what the task asks, each question answered in its order; every file whole, under its
-  path.
+- `answer`: for each program the task names, in its order, under the program's name:
+  - `verdict`: the task's word, and the lines it prints where the task asks for them;
+  - `abort`: the sentence the abort says and what you would change, or `none`;
+  - `confidence`: 1 to 5;
+  - `sections`: the numbered sections of the specification your verdict rests on.
 - `choice_points`: every place the specification left you a choice, the choice you made, and what
   the other would produce.
-- `prediction`: whether the compiler accepts the program, every line the program writes, in order,
-  and how it ends: normally, by an abort and what the abort says, or not at all.
 - `context`: whether any instruction, rule, contract, memory or document other than the files in
   this directory reached your context, and which.
 
```

### tools/key-grades.tsv, new, whole

```
# Panel 262's key and classes, fixed before any session (docs/panel/262-briefs/blind/registration.md: § The key, §
# The grades, § What the synthesis added's classification and refusal list, and § Amended before any session's key on
# both platforms under 9b88d3e84's stage 2, sources 7c7c978a4dfeddf3). The key is the plain build's, one verdict on
# both graded platforms (this Mac, Linux arm64), plain and --sanitize alike, for every program. Columns: task,
# program, key verdict, key lines (|-separated; for ABORTS the lines before the abort), words an ABORTS must name, the
# class under now, adopted and rival, the decision rule that reads it. A class `may` makes NOT PROMISED text-right;
# `line427-no-text-right` has no text-right; `promised`, `silent` and `aborts` none either.
task	program	key	lines	words	now	adopted	rival	rule
t1	h1	RUNS	answer ran for 7|result 70	-	silent	promised	silent	D1
t1	h2	ABORTS	-	handler	may	may	may	D3
t1	h3	RUNS	answer ran for 7|result 70	-	silent	promised	silent	D1
t1	h4	ABORTS	-	handler	line427-no-text-right	may	line427-no-text-right	D4
t1	h5	ABORTS	-	handler	may	may	may	D3
t1	h6	ABORTS	-	handler	may	may	may	D3
t1	h7	RUNS	answer ran for 7|result 70	-	may	promised	may	D2
t1	h8	ABORTS	-	handler	may	may	may	D3
t1	h9	RUNS	left on the thread: 1|answer ran for 7|result 70	-	may	promised	promised	D2
t1	h10	RUNS	answer ran for 7|result 70	-	may	promised	may	D2
t1	h11	RUNS	answer ran for 7|result 70	-	may	may	may	D2
t1	h12	ABORTS	-	handler	silent	may	may	D5
t1	h13	RUNS	watch 0|answer ran for 7|result 70|counted 1	-	silent	promised	promised	D1
t2	n1	RUNS	answer ran for 7|result 70	-	silent	promised	promised	D1
t2	n2	RUNS	left: -1|answer ran for 7|result 70	-	silent	promised	promised	D1
t2	n3	RUNS	left: -1|answer ran for 7|result 70	-	silent	promised	promised	D1
t2	n4	ABORTS	-	thread	silent	aborts	silent	D4
```

## The hashes of the runner as registered (the area, 03:40, before any session)

```
6919f6e7a90d12ee4c1d200d2b42fc07f6134f243d3bd93778e859fd66154d49  run.sh
617a9ffc62d4807d0ccfac66d301a7fe9368fcd1d8f2922171df7c14185cbe34  preflight.sh
e5b8bd741da31af51a2491adf2adc8e8e110133251f7b0f11580ad470ece48aa  tools/after.py
01855da511c5b0da778b0794175646721ec88e767920380f4b853844bc8c5fc8  tools/brief-session.md
1a93433adf1aca1b005741d34a50bd760bce723df12cc69eddaf0596da0c412a  tools/brief-verdict-reports.md
52d37bb04ac8ab20ae12458f1f7550a3ffaa818d5e48cec88fe90359f8835a36  tools/brief-verdict.md
083089c60775136c80fec0599033d250d93032a9a7b66c4d9ac0914bd530e169  tools/check_area.py
3722161f522bbd1c75af5db33a341538c1ca9567f6a6086a9139e1a7b9e98613  tools/check_folders.py
b6e80879b6bbd8f08fbc2ddb0d387c9ae7d41ce353d26a7ab6784056b11b5ba7  tools/inputs.sha256
a3613cf8293f13fa21e6bf2573b26ceb18070fc143ad867899846044bca378a8  tools/key-grades.tsv
f317ef7b7fe49b7f1a0f7d2bc01e9c0a8464e5927dc9965c03e5bff25f368416  tools/make_sessions.py
97f7a1df881d1fc9cd80a4b32fbb3d7c1b476cd8e192e71912694f80c4370cb7  tools/make_verdict.py
d6fe89f5c79e6d938577e1352829788b4251fcc66c81bd5e896e924d095de63d  tools/read_runs.py
a1519e78f9154349e9d1c58183bfcf0f390fd3a6cba9cf795f472b2682711b1d  tools/reads.py
84cd466ecc7acca271afc0e3d79ba24dbab86692056a9552decbdaae4395a302  tools/registration.md
41aae8419d682a6d110d276998070924cb5fb259ed3993e234a3847e7a99d5c1  tools/scan.py
0f46d1878d433bc5d027ca431d57a0f4a21228ee0157d0c767caae03eaccab28  tools/spent.py
909fb9085d92c8cd08e6f1b7fe8de672945fc84bfe8c94b238cb59879a90a665  tools/synthesis-adopted.diff
6bb97f71aee879e749cc562d3ba1aaafcfbbba4032a917d237b3178d022e059d  tools/synthesis-rival.diff
c3480f8663cfa9bc9de1547c78102a703393627d35b6256043c80fc037acdb6b  order.txt
faaac0cbb99e5c985baa25b8ae4e2e692036013e4cf58831dbe0d5331c08be6c  key.txt
```

`tools/registration-runner.md` is this file, copied into the area after it was written; `key.txt` holds the draw
and is read for grading only after the run.
