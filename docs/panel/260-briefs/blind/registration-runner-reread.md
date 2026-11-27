# Panel 260's re-read of the narrowed sentence: the runner's differences from the first run's, by diff, registered before any session

Written 2026-11-27 from 08:28 by the re-read's session (its brief: `.claude/worktrees/rebel-scratch/coord/reread260-brief.md`),
**before any session, the free preflight included**, as `registration.md` § Registered 2026-11-26: the narrowed text,
re-read asks: *"run as registered by whoever starts it, its runner's changes written as a `diff -u` beside this file
before any session"*. The run is the author's authorisation of 2026-11-27 at about 08:25 (the answer *2a*, the
registration's last paragraph), **a ceiling of 12.00 USD for the whole run**, 20 sessions, no verdict session.

## The runner it starts from

**The first run's, as it ran**: `run.sh`, `preflight.sh` and `tools/` of
`docs/panel/260-reports/llm-ergonomist-blind-answers.tar.xz` on `main` (`tar -xJf`, read 08:29), every file at the hash
`registration-runner.md` § The hashes of the runner as registered gives, `preflight.sh` and `tools/spent.py` at the
hashes its § Amended 2026-11-26 after the network outage gives (`166300f1...`, `0d09532b...`), and each `cmp`-equal to
the first run's area, `.claude/worktrees/260-llm-ergonomist/`. Copied to `.claude/worktrees/reread260-scratch/runner-first/`
before any change; every diff below is against those bytes.

**Unchanged** (`cmp` equal to the first run's): `tools/after.py`, `tools/brief-session.md` (sha256 `ca8e2077...8beb`, 274
words, pinned in `run.sh` as before), `tools/check_area.py`, `tools/reads.py`, `tools/scan.py`, `tools/spent.py` (the
amended one, counting `cut/*/run.json`), `tools/synthesis-adopted.diff`. **Left out of the area**, there being no
verdict and no `rival` in the re-read: `tools/make_verdict.py`, `tools/brief-verdict.md`, `tools/brief-verdict-reports.md`,
`tools/synthesis-rival.diff`.

## What changes, in words

- **The area**: `.claude/worktrees/260-reread-llm-ergonomist/`, drawn at 08:35 as the first run's was: the top of an
  empty git repository of its own (`git init -b main`, `.gitignore` `*`, local user `reader` <`reader@invalid`>, no
  commit; this repository's `.gitignore:34`, `.claude/worktrees/`, ignores it), `home/` and `tmp/` inside; `arms/` the
  two arms, `tasks/` the two tasks, `programs/` t2's 20 files.
- **The arms** (`registration.md` § Registered 2026-11-26: the narrowed text, re-read, its table): `arms/spec-adopted.md`,
  `c3885b81b`'s `spec/heroes-spec.md` with `docs/panel/260-reports/synthesis-adopted.diff` applied by `patch -p1`
  (`70b8e497...f1a7`, 34,855 bytes, the first run's `adopted` byte for byte); `arms/spec-narrowed.md`, that with its line
  401 replaced by the line step 208b wrote into § 13 (`spec/heroes-spec.md` line 411 at `750d2540d` and at `9f5c59e6b`,
  `cmp`-equal, 910 bytes, sha256 `21af541d...`; `tools/narrowed-line.txt`): `6874087d...74cc`, 35,051 bytes, `diff`
  printing one `<` and one `>` (`401c401`); the line's two sentences before *" A group names its header"*, with a
  newline, are the registered narrowed text alone, 834 bytes, `cdd29e79...624d`, and its tail after them is
  `adopted`'s byte for byte. `tools/spec-now.md` (`c3885b81b`'s spec, `c915f0f2...2bee`) is kept for the arm check
  alone and is in no session's folder.
- **The folders** (`make_sessions.py`, `check_folders.py`): **two arms**, `adopted` and `narrowed`; **two tasks**, t1 and
  t2; five replicates: **20 folders**, each holding exactly `brief.md`, `task.md`, `spec.md` (one arm, never named) and,
  for t2, `programs/`, its 20 files nested; every file at its registered hash (`tools/inputs.sha256`, whole below).
  **The draw**: panel 256's, `secrets.SystemRandom().sample` over a pool of letters, written to `key.txt` alone and
  printed nowhere; the pool leaves out the letters of `adopted` and `narrowed` and, a change, **the first run's three
  labels `q`, `f` and `c`**, so that no folder of the re-read bears the name of a first-run folder of another arm in the
  records (14 letters). **The order**: panel 256's, the four cells in a fixed sequence, reversed on the even replicates;
  no verdict. Made at 08:35:42: `order.txt` `4708a527...bdcb`, `key.txt` `4178bfd9...093c`, 260 files under `s/`,
  `check_folders.py` exit 0. The draw (`key.txt`'s first line) is read by this session only for grading, after the 20.
- **The caps and the ceiling** (`run.sh`, `preflight.sh`): **0.60 USD each session, margin 0.10, ceiling 12.00**, the
  registration's, the registered ceiling the same; the runner stops before any session where `spent + 0.60 + 0.10`
  would pass 12.00, so **the 20th session starts only where the 19 before it spent 11.30 or less**. The first run's
  static check *"the worst case stays under the ceiling, margin included"* cannot hold at the registered numbers
  (20 x 0.60 + 0.10 = 12.10 > 12.00), so `preflight.sh` checks the registration's own arithmetic instead, *"20 sessions
  at the cap"* within the ceiling, and prints the 11.30 above; the stop rule is the first run's, unchanged. No verdict
  cap: `CAP_VERDICT` is gone, and `run.sh` stops on any folder of `order.txt` that is not a `t1-` or `t2-` session.
- **The registration pinned** (`run.sh`): `REGISTRATION_SHA` is `registration.md` as it stands on `main` at `9f5c59e6b`
  (its re-read section and the authorisation), `f9fcd8a8...f8a7`, copied to `tools/registration.md`. **The warden's
  pin is the first run's**, `818747c9...7dd7ea` (the section reads so on `main` today): the re-read registers no
  prediction, and the check is kept as the runner has always made it.
- **The session command: the first run's, flag for flag**: `claude -p "<the registered prompt>" --bare --restricted
  --safe-mode --strict-mcp-config --model claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write"
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 0.60
  --output-format json < /dev/null`, under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area and
  `GIT_CEILING_DIRECTORIES` above it, the key from the trunk's `.env` (its length alone printed); the prompt unchanged;
  the per-session limit 900 s unchanged. **`claude --version` reads `2.1.326 (Claude Code)`** on the day (08:36:07),
  where the first run read `2.1.323`: the registration reads `adopted` again the same day for exactly this, *"so that a
  change of the model's or the runner's between the two days is not read as the text's"*.
- **One record added after the free preflight** (`run.sh`): the tools the preflight session was offered, read from its
  transcript's `api-request-shape` entry as panel 262's runner reads them, **logged, not a stop**: the first run's
  sessions were offered `Read` alone and gave their reports as their final answers (`result.md`), and the scoring reads
  `report.md` where one exists, else `result.md`, as it did.
- **`read_runs.py`** reads `verdict/run.json` only where it exists (it read it unconditionally, and there is none).
- **`preflight.sh`**: the constants above; the arms checked by a new `tools/check_arms.py` (whole below) instead of the
  first run's `patch` loop over `adopted` and `rival`; the tool pins (`read_runs.py` at its new bytes, `after.py` and
  `reads.py` added, `brief-verdict.md` gone); 20 folders. **And a repair, found here**: the first run's two checks that
  run `for ...; do ... || exit 1; done` inside the string `chk` hands to `eval` end `preflight.sh` itself at exit 1, with
  no `FAIL` line and no later check, when they fail (run 08:33: `bash -c` of that shape prints nothing and exits 1);
  fail closed but silent. The one such check kept runs in a subshell, `( ... )`, so a failure prints `FAIL` and the
  checks after it still run.
- **The scoring, fixed here before any answer exists** (`.claude/worktrees/reread260-scratch/score/`, the first run's
  `score/` files from the same archive): `t1_extract.py`, `t2_read.py`, `t1_job.sh` and `t1_built.py` unchanged;
  `t1_declared.py` reads `narrowed` by `adopted`'s rows, as § The narrowed text, re-read says (*"W2 of t1,
  declared-right by § The grader's reading's own rows (nothing, each file)"*, W1, W3 and W4 *"graded the same way"*), and
  its layer headers are both arms' (`hero_<word>.h` but `hero_os.h` and `hero_compiler.h`); run over the first run's
  own `score/t1/` it gives the first run's 40 `adopted` and `now` rows of `t1-declared.tsv` exactly (`diff` empty,
  08:36). t2's key is the first run's `key.sh` unchanged (`80c144b4...`), run on the day by today's `main` build on this
  Mac and in the Linux arm64 image, before any paid session (the report says what it read).

## The diffs, each against the first run's file

### run.sh

```diff
--- first/run.sh
+++ reread/run.sh
@@ -1,10 +1,11 @@
 #!/bin/bash
-# Panel 260's blind seat (panel 256's runner as panel 257 adapted it and panel 259 used it, adapted again for panel
-# 260: see tools/registration-runner.md for every difference and its diff). Runs the 31 sessions order.txt names, one at a
-# time, never two, each from its own folder: the 30 graded ones capped at 0.60 USD each, the verdict at 1.20, and
-# stops before the sum plus the next session's cap plus a 0.10 margin would pass 20.00 USD, the author's ceiling for
-# this run (the answer *ok a*, relayed at 10:37 on 2026-11-25, recorded in docs/panel/260-briefs/llm-ergonomist.md;
-# registration.md's cap section).
+# Panel 260's re-read of the narrowed sentence (the first run's runner, panel 256's as panels 257 and 259 adapted it
+# and panel 260 ran it on 2026-11-26, adapted again for the re-read: see tools/registration-runner.md, which is
+# docs/panel/260-briefs/blind/registration-runner-reread.md, for every difference and its diff). Runs the 20 sessions
+# order.txt names (t1 and t2 under `adopted` and `narrowed`, five each; no verdict session), one at a time, never two,
+# each from its own folder, each capped at 0.60 USD, and stops before the sum plus the next session's cap plus a 0.10
+# margin would pass 12.00 USD, the author's ceiling for this run (the answer *2a*, 2026-11-27 about 08:25, recorded at
+# the end of docs/panel/260-briefs/blind/registration.md § Registered 2026-11-26: the narrowed text, re-read).
 # A folder already holding run.json is skipped and its cost counted, so a stopped run resumes.
 #
 # NOT TO BE STARTED BEFORE THE SPEC-WARDEN'S REPORT HOLDS ITS REGISTERED PREDICTIONS (panel 251's critic,
@@ -36,8 +37,12 @@
 # skipped. A session still running after $SESSION_LIMIT seconds is killed by its pid (never by name) and counts as
 # a stop. SIGINT and SIGTERM to the runner kill the session in flight and remove the lock.
 #
-# Started by the blind-run session the coordinator of M-rebel-library launched on the author's authorisation (panel 260):
-#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-llm-ergonomist/run.sh
+# And, as panel 262's runner asks (there a stop, here a record): the free preflight's transcript names the tools the
+# session is offered (its api-request-shape entry), and the runner logs them before any paid session; the first run's
+# 31 sessions were offered Read alone and gave their reports as their final answers (result.md).
+#
+# Started by the re-read's session the coordinator of M-rebel-library launched on the author's authorisation:
+#   bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-reread-llm-ergonomist/run.sh
 set -u
 HERE="$(cd "$(dirname "$0")" && pwd)"
 cd "$HERE" || exit 2
@@ -49,17 +54,16 @@
 esac
 
 CAP=0.60
-CAP_VERDICT=1.20
 MARGIN=0.10
-CEILING=20.00
-REGISTERED_CEILING=20.00
+CEILING=12.00
+REGISTERED_CEILING=12.00
 SESSION_LIMIT=900
 ENV_FILE=/Users/joseph/Temp/heroes/heroes-lang/.env
 WARDEN=/Users/joseph/Temp/heroes/heroes-lang/docs/panel/260-reports/spec-warden.md
 WARDEN_SECTION_SHA=818747c952c3fddf1966862747e0549d245bd9e2fa2b73b91ff47755f77dd7ea
-# The coordinator's registration (the arms' hashes, the cap and the predictions), pinned here.
+# The registration as it stands on main at 9f5c59e6b (its re-read section and the author's authorisation), pinned here.
 REGISTRATION="$HERE/tools/registration.md"
-REGISTRATION_SHA=369e4fa1e2b4a70e892c172cae9d10b4c704a0fa334ccd918f3c6647d3e9e672
+REGISTRATION_SHA=f9fcd8a8a1808fdd71c6451315c572260ad1223f9617754aa0859ac8373ff8a7
 HEADING='## Predictions registered before the blind run'
 CLI_TMP="/private/tmp/claude-$(id -u)"
 REFUSED_KEY="sk-ant-api03-refused-by-design-panel-preflight"
@@ -188,6 +192,20 @@
     *"prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no"*) ;;
     *) echo "the preflight's transcript does not read clean: nothing more run"; exit 2 ;;
 esac
+# The tools the preflight session was offered (its transcript's api-request-shape), logged; the first run's were Read alone.
+pre_tools=$(python3 -I -c '
+import glob, json
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
 if [ "$PREFLIGHT_ONLY" = 1 ]; then
     log "preflight only: stopped before the key is read; no paid session run"
     exit 0
@@ -204,7 +222,10 @@
 passed_registered=0
 while read -r folder <&3; do
     [ -n "$folder" ] || continue
-    if [ "$folder" = verdict ]; then d="$HERE/verdict"; cap=$CAP_VERDICT; else d="$HERE/s/$folder"; cap=$CAP; fi
+    case "$folder" in
+        t1-*|t2-*) d="$HERE/s/$folder"; cap=$CAP ;;
+        *) log "$folder is no graded session of the re-read (no verdict session is registered): runner stopped"; STOPPED=3; break ;;
+    esac
     if [ -f "$d/run.json" ]; then
         echo "$folder already run, its cost counted"
         continue
@@ -219,13 +240,6 @@
         log "note: spent $spent passes the registered ceiling $REGISTERED_CEILING (stop at $CEILING)"
         passed_registered=1
     fi
-    if [ "$folder" = verdict ] && [ ! -d "$d" ]; then
-        python3 -I tools/make_verdict.py 2>&1 | tee -a "$HERE/run.log"
-        [ -d "$d" ] || { log "verdict/ not built: runner stopped"; STOPPED=3; break; }
-    fi
-    if [ "$folder" = verdict ]; then
-        python3 -I tools/check_folders.py --verdict || { log "verdict/ is not what was built: runner stopped"; STOPPED=3; break; }
-    fi
     log "$folder (spent $spent, cap $cap)"
     session "$d" "$cap" "$ANTHROPIC_API_KEY"
     status=$?
```

### preflight.sh

```diff
--- first/preflight.sh
+++ reread/preflight.sh
@@ -1,5 +1,5 @@
 #!/bin/bash
-# Panel 260's preflight (panel 257's as panel 259 changed it, its arm check and pins changed for panel 260): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
+# Panel 260's re-read's preflight (the first run's, its arms, count, caps, ceiling and arm checks changed for the re-read): everything that can be asked of the area before a paid session, and it SPENDS NOTHING.
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
-echo "      worst case, every session to its cap: $WORST USD (31 sessions); the registration's estimate about 6 to 8, an inference, unrun"
-chk "the worst case stays under the ceiling, margin included (every session can run)" "python3 -I -c 'import sys; sys.exit(0 if $WORST + $MARGIN <= $CEILING else 1)'"
+CAP=$(const CAP); MARGIN=$(const MARGIN); CEILING=$(const CEILING)
+echo "      constants of run.sh: cap $CAP, margin $MARGIN, ceiling $CEILING (registered ceiling $(const REGISTERED_CEILING), the author's), per-session limit $(const SESSION_LIMIT) s"
+chk "the cap is the registered 0.60 a session, margin 0.10" '[ "$CAP" = 0.60 ] && [ "$MARGIN" = 0.10 ]'
+chk "ceiling is 12.00, the author's for the whole run" '[ "$CEILING" = 12.00 ] && [ "$(const REGISTERED_CEILING)" = 12.00 ]'
+WORST=$(python3 -I -c "print(f'{20*$CAP:.2f}')")
+LAST=$(python3 -I -c "print(f'{$CEILING - $CAP - $MARGIN:.2f}')")
+echo "      worst case, every session to its cap: $WORST USD (20 sessions, the registration's 20 at the cap); a session starts only where spent + $CAP + $MARGIN <= $CEILING, so the 20th starts only where the 19 before it spent $LAST or less; the registration's estimate about 4.1, an inference, unrun"
+chk "the 20 sessions at their caps do not pass the ceiling (the registration's arithmetic: 20 sessions at the cap)" "python3 -I -c 'import sys; sys.exit(0 if $WORST <= $CEILING else 1)'"
 
 # the runner's own text: every flag of the registered command, no tool beyond Read and Write
 for f in '--bare' '--restricted' '--safe-mode' '--strict-mcp-config' '--model claude-opus-5-5' '--tools "Read,Write"' '--allowedTools "Read,Write"' \
@@ -47,14 +48,14 @@
 REG_SHA=$(grep -E '^REGISTRATION_SHA=' run.sh | cut -d= -f2)
 chk "tools/registration.md is the file run.sh pins" '[ "$(sha tools/registration.md)" = "$REG_SHA" ]'
 chk "every arm's full sha256 stands in the registration's own table" \
-    'for a in now adopted rival; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done'
-for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py d6fe89f5c79e6d93' 'spent.py 0d09532b49d7c848' 'brief-verdict.md 52d37bb04ac8ab20'; do
+    '( for a in adopted narrowed; do grep -qF "$(grep "arms/spec-$a.md" tools/inputs.sha256 | cut -d" " -f2)" tools/registration.md || exit 1; done )'
+for pair in 'scan.py 41aae8419d682a6d' 'check_area.py 083089c60775136c' 'read_runs.py 5603d2b82a208951' 'spent.py 0d09532b49d7c848' 'after.py e5b8bd741da31af5' 'reads.py a1519e78f9154349'; do
     tf=${pair% *}; tp=${pair#* }
     chk "tools/$tf is the registered bytes ($tp...)" '[ "$(sha tools/$tf | cut -c1-16)" = "$tp" ]'
 done
-chk "the arms are the frozen spec with each registered diff applied by patch -p1 (adopted: the synthesis's R6 text and production; rival: w1)" \
-    'for a in adopted rival; do rm -rf tmp/arm-check && mkdir -p tmp/arm-check/spec && cp arms/spec-now.md tmp/arm-check/spec/heroes-spec.md && (cd tmp/arm-check && patch -p1 --quiet < ../../tools/synthesis-$a.diff) && cmp -s tmp/arm-check/spec/heroes-spec.md arms/spec-$a.md || exit 1; done; rm -rf tmp/arm-check'
-chk "tools/registration-runner.md (the runner's diffs against panel 256's bytes) exists" '[ -s tools/registration-runner.md ]'
+chk "the arms rebuilt (tools/check_arms.py): adopted is c3885b81b's spec with the synthesis's diff by patch -p1; narrowed is adopted with line 401 replaced by step 208b's line, that line alone differing, its two sentences the registered 834 bytes" \
+    'python3 -I tools/check_arms.py'
+chk "tools/registration-runner.md (the re-read's runner diffs against the first run's bytes) exists" '[ -s tools/registration-runner.md ]'
 
 # the warden's registered predictions, pinned
 W=$(grep -E '^WARDEN=' run.sh | cut -d= -f2); WS=$(grep -E '^WARDEN_SECTION_SHA=' run.sh | cut -d= -f2)
@@ -63,7 +64,7 @@
 
 # the folders, the order, the area
 chk "order.txt and key.txt stand" '[ -s order.txt ] && [ -s key.txt ]'
-chk "check_folders: 31 in order, 30 folders at their registered hashes, 5 per task and arm, none stray" 'python3 -I tools/check_folders.py'
+chk "check_folders: 20 in order, 20 folders at their registered hashes, 5 per task and arm, no verdict, none stray" 'python3 -I tools/check_folders.py'
 chk "check_area: the area is the top of its own empty git repository, reader <reader@invalid>, HOME and TMPDIR inside" 'python3 -I tools/check_area.py'
 chk "git log in the area says no commit" 'git log 2>&1 | grep -q "does not have any commits yet"'
 chk "no CLAUDE.md, AGENTS.md, MEMORY.md or memory folder in the area (git's own folder left out)" \
@@ -90,12 +91,7 @@
 while read -r name; do
     [ -n "$name" ] || continue
     n=$((n + 1))
-    if [ "$name" = verdict ]; then
-        d=verdict; cap=$CAPV
-        [ -d verdict ] && extra="" || extra=" (built after the 30 by tools/make_verdict.py, reports of the successful sessions only)"
-    else
-        d="s/$name"; cap=$CAP; extra=""
-    fi
+    d="s/$name"; cap=$CAP; extra=""
     if [ -f "$d/run.json" ]; then st="already run"; else st="to run"; fi
     if [ -d "$d" ]; then fc=$(find "$d" -type f | wc -l | tr -d ' '); else fc=0; fi
     printf '  %2d  %-9s cap %s  %-11s %2s files%s\n' "$n" "$name" "$cap" "$st" "$fc" "$extra"
```

### tools/check_folders.py

```diff
--- first/tools/check_folders.py
+++ reread/tools/check_folders.py
@@ -1,12 +1,13 @@
-# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used unchanged in code by panels 259 and 260): order.txt names 31 folders (the 30 graded
-# ones, 5 per task and arm, then `verdict` last), each in key.txt; every folder not yet run holds exactly its
+# Before any spend (panel 256's check_folders.py, as changed for panel 257 and used in code by panels 259 and 260, its arms,
+# count and order changed for panel 260's re-read): order.txt names 20 folders (5 per task and arm, `adopted` and
+# `narrowed`, no verdict), each in key.txt; every folder not yet run holds exactly its
 # registered files, nested ones included (`programs/`), at their registered hashes, and each of those hashes is
 # the one tools/inputs.sha256 registers (the arm's spec, the task, the programs) or tools/brief-session.md's;
 # no folder of s/ stands outside order.txt. The verdict's folder is built after the 30, so it is not asked for
 # before then (`--verdict` asks only of it, after tools/make_verdict.py).
 # usage: python3 -I tools/check_folders.py [--verdict]
 import hashlib, os, sys
-ARMS = ('now', 'adopted', 'rival')
+ARMS = ('adopted', 'narrowed')
 def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
 key = {}
@@ -17,10 +18,10 @@
     key[name] = (arm, dict(f.split('=', 1) for f in files))
 order = [l.strip() for l in open('order.txt') if l.strip()]
 bad = []
-if len(order) != 31 or len(set(order)) != 31 or order[-1] != 'verdict':
-    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, the last {order[-1:]}: not 31 with verdict last')
+if len(order) != 20 or len(set(order)) != 20 or 'verdict' in order:
+    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct: not 20, or a verdict among them')
 cells = {}
-for name in order[:-1]:
+for name in order:
     if name not in key:
         continue
     task = name.split('-')[0]
```

### tools/make_sessions.py

```diff
--- first/tools/make_sessions.py
+++ reread/tools/make_sessions.py
@@ -1,13 +1,12 @@
-# Panel 260's blind seat (panel 256's make_sessions.py as panel 257 changed it and panel 259 used it; for 260 only the
-# programs each task's folder holds changed: t1 none, t2 the six program folders p1 to p6 under programs/, nested):
-# draws the arms' labels, then makes one session
-# folder per task, arm and replicate (t1 and t2, `now`, `adopted`, `rival`, 5 each: 30 folders), each holding
+# Panel 260's re-read of the narrowed sentence (panel 260's first run's make_sessions.py, its arms changed to the two
+# the re-read registers): draws the arms' labels, then makes one session
+# folder per task, arm and replicate (t1 and t2, `adopted`, `narrowed`, 5 each: 20 folders), each holding
 # brief.md, task.md, spec.md and, for t2, programs/, and nothing else, and writes key.txt and order.txt beside s/, never
-# inside it. THE DRAW: the three labels are sampled from a pool of letters with panel 256's method,
+# inside it. THE DRAW: the two labels are sampled from a pool of letters with panel 256's method,
 # secrets.SystemRandom (the operating system's entropy, no seed, not reproducible); key.txt is the only place the
-# draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The order is panel 256's:
-# the six cells in a fixed sequence, reversed on the even replicates, the verdict last.
-# The verdict's folder is NOT made here: it is built after the 30 by tools/make_verdict.py.
+# draw is written, and this script PRINTS NEITHER THE LABELS NOR THE ARMS they name. The pool leaves out the letters
+# of the arms' names and the first run's three labels (q, f, c), so no folder of the re-read bears a first run's name.
+# The order is panel 256's: the four cells in a fixed sequence, reversed on the even replicates. No verdict session.
 # Refuses to run over an existing s/, verdict/, key.txt or order.txt.
 # usage (from the area): python3 -I tools/make_sessions.py
 import hashlib, os, secrets, sys
@@ -16,14 +15,15 @@
     if os.path.exists(p):
         sys.exit(f'{p} exists: nothing made')
 REPLICATES = 5
-ARMS = ['now', 'adopted', 'rival']
+ARMS = ['adopted', 'narrowed']
 SPEC = {a: f'arms/spec-{a}.md' for a in ARMS}
 TASKS = ['t1', 't2']
 T2_PROGRAMS = sorted(os.path.relpath(os.path.join(r, x), 'programs') for r, _, fs in os.walk('programs') for x in fs)
 assert len(T2_PROGRAMS) == 20, f'programs/ holds {len(T2_PROGRAMS)} files, not the 20 registered'
 PROGRAMS = {'t1': [], 't2': T2_PROGRAMS}
-# no letter that stands in an arm's name (now, adopted, rival), so a label says nothing of its arm
-POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('now' + 'adopted' + 'rival')]
+# no letter that stands in an arm's name (adopted, narrowed), so a label says nothing of its arm, and none of the first
+# run's labels (q, f, c), so a folder's name says which run it was
+POOL = [c for c in 'abcdefghijklmnopqrstuvwxyz' if c not in set('adopted' + 'narrowed' + 'qfc')]
 def sha(b): return hashlib.sha256(b).hexdigest()
 def read(p): return open(p, 'rb').read()
 reg = {l.split()[0]: l.split()[1] for l in open('tools/inputs.sha256') if l.strip()}
@@ -50,11 +50,10 @@
 for r in range(1, REPLICATES + 1):
     seq = cells if r % 2 == 1 else list(reversed(cells))
     order += [f'{t}-{draw[a]}-{r}' for t, a in seq]
-order.append('verdict')
 head = ('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) +
-        '\n# drawn by tools/make_sessions.py: secrets.SystemRandom().sample over the letters standing in none of the arms\' names; '
-        'the verdict row is appended by tools/make_verdict.py after the 30 sessions\n')
+        '\n# drawn by tools/make_sessions.py: secrets.SystemRandom().sample over the letters standing in none of the arms\' names '
+        'and none of the first run\'s labels (q, f, c); no verdict session\n')
 open('key.txt', 'w').write(head + '\n'.join(rows) + '\n')
 open('order.txt', 'w').write('\n'.join(order) + '\n')
-print(len(order), 'sessions in order.txt (30 graded and the verdict);',
+print(len(order), 'sessions in order.txt (20 graded, no verdict);',
       sum(len(files) for _, _, files in os.walk('s')), 'files under s/; the draw is written to key.txt alone')
```

### tools/read_runs.py

```diff
--- first/tools/read_runs.py
+++ reread/tools/read_runs.py
@@ -5,7 +5,7 @@
 root = sys.argv[1]
 tot = 0.0
 rows = []
-for name in sorted(os.listdir(os.path.join(root, 's'))) + ['verdict']:
+for name in sorted(os.listdir(os.path.join(root, 's'))) + (['verdict'] if os.path.isfile(os.path.join(root, 'verdict', 'run.json')) else []):
     d = os.path.join(root, 's', name) if name != 'verdict' else os.path.join(root, 'verdict')
     j = json.load(open(os.path.join(d, 'run.json')))
     c = j.get('total_cost_usd')
```

### score/t1_declared.py (the scoring, not the runner; the first run's `score/` file)

```diff
--- first/score/t1_declared.py
+++ reread/score/t1_declared.py
@@ -1,9 +1,10 @@
 # Panel 260's blind run, the scoring: the declared-right grade of t1, by blind/registration.md § The grader's reading
-# of each arm, by file. Reads score/t1/<session>/w1..w4/ (t1_extract.py's) and key.txt's arm per session.
-#   - the declaration's words under `adopted` and `rival`: the word `layer` after a group's header string, before
+# of each arm, by file. For the re-read (registration.md § Registered 2026-11-26: the narrowed text, re-read): `narrowed`
+# is read by `adopted`'s rows, its W2 by their *nothing, each file*, as that section says; `now` and `rival` are not run. Reads score/t1/<session>/w1..w4/ (t1_extract.py's) and key.txt's arm per session.
+#   - the declaration's words under `adopted` and `narrowed`: the word `layer` after a group's header string, before
 #     `link` or `pkgconfig` where the group has one; under `now`: none (every cell is *nothing*).
-#   - a layer header: `hero_<word>.h` but `hero_os.h` and `hero_compiler.h` (the arms' "every other `hero_` header it
-#     provides but the compiler's `hero_compiler.h`"); no reader's own header is read here (none was written: printed).
+#   - a layer header: `hero_<word>.h` but `hero_os.h` and `hero_compiler.h` (`adopted`'s "every other `hero_` header it
+#     provides but the compiler's `hero_compiler.h`", `narrowed`'s "... but `hero_os.h` and the compiler's `hero_compiler.h`"); no reader's own header is read here (none was written: printed).
 #   - declared-right on a job: every file the table names holds the declaration where its cell says *declares* and none
 #     where it says *nothing*, and no other file the reader wrote for that job holds one. A `layer` word written
 #     anywhere else on a group's line (after `link`, say) or in a file that is not Heroes counts as holding one, and
@@ -36,7 +37,7 @@
 NAMED = {'w1': ['ticks/clock.hero', 'main.hero', 'heroes.toml', 'ticks/heroes.toml'],
          'w2': ['ident/me.hero', 'main.hero', 'heroes.toml', 'ident/heroes.toml'],
          'w3': ['main.hero', 'heroes.toml'], 'w4': ['main.hero', 'heroes.toml']}
-# the file whose cell says *declares* under adopted and rival
+# the file whose cell says *declares* under adopted and narrowed
 DECLARES = {'w1': 'ticks/clock.hero', 'w2': None, 'w3': 'main.hero', 'w4': 'main.hero'}
 
 print('session\tarm\tjob\tgrade\twhy')
@@ -52,7 +53,7 @@
         why, ok = [], True
         if not files:
             print(f'{name}\t{arm}\t{job}\twrong\tno file for the job'); continue
-        target = DECLARES[job] if arm in ('adopted', 'rival') else None
+        target = DECLARES[job] if arm in ('adopted', 'narrowed') else None
         if target and target not in files:
             ok = False; why.append(f'{target} missing')
         for rel, text in sorted(files.items()):
@@ -64,11 +65,11 @@
                             ok = False; why.append(f'{rel}: group over {header} does not say layer' + (' (the word misplaced)' if word else ''))
                     elif word:
                         # a group of the program's own over a layer header in W1's main.hero declares (the table's row)
-                        if arm in ('adopted', 'rival') and job == 'w1' and rel == 'main.hero' and layerish and placed:
+                        if arm in ('adopted', 'narrowed') and job == 'w1' and rel == 'main.hero' and layerish and placed:
                             why.append(f'{rel}: own group over {header} says layer (the table allows it)')
                             continue
                         ok = False; why.append(f'{rel}: group over {header} says layer')
-                    elif arm in ('adopted', 'rival') and layerish and rel != target:
+                    elif arm in ('adopted', 'narrowed') and layerish and rel != target:
                         if job == 'w1' and rel == 'main.hero':
                             ok = False; why.append(f'{rel}: own group over {header} does not say layer')
                         elif job in ('w3', 'w4'):
```

### tools/check_arms.py, new, whole

```python
# Panel 260's re-read, before any spend: the two arms are what docs/panel/260-briefs/blind/registration.md § Registered
# 2026-11-26: the narrowed text, re-read says they are, rebuilt here from the frozen spec and asked byte for byte.
#   - tools/spec-now.md is c3885b81b's spec/heroes-spec.md (sha256 c915f0f2..., the first run's `now`), never an arm here;
#   - arms/spec-adopted.md is it with tools/synthesis-adopted.diff applied by `patch -p1` (the first run's `adopted`);
#   - arms/spec-narrowed.md is adopted with its line 401 (R6's sentence and the paragraph's tail) replaced by
#     tools/narrowed-line.txt, the line step 208b wrote into spec/heroes-spec.md (line 411 at 750d2540d and at
#     9f5c59e6b), the one line differing; the two sentences of it before " A group names its header", with a newline,
#     are the registered narrowed text alone (834 bytes, sha256 cdd29e79...); and the paragraph's tail after them is
#     adopted's, byte for byte.
# Prints one line per failure and exits 2 on any; prints nothing and exits 0 when all hold.
# usage (from the area): python3 -I tools/check_arms.py
import hashlib, os, shutil, subprocess, sys
NOW_SHA = 'c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee'
ALONE_SHA = 'cdd29e79abe90f07c4bec22de575b1db76262160c7b25bf1255c6145aa43624d'
TAIL = b' A group names its header, and `link`'
def sha(b): return hashlib.sha256(b).hexdigest()
def read(p): return open(p, 'rb').read()
bad = []
now, adopted, narrowed, line = (read('tools/spec-now.md'), read('arms/spec-adopted.md'), read('arms/spec-narrowed.md'),
                                read('tools/narrowed-line.txt'))
if sha(now) != NOW_SHA:
    bad.append('tools/spec-now.md is not c3885b81b\'s spec')
work = os.path.join('tmp', 'arm-check')
shutil.rmtree(work, ignore_errors=True)
os.makedirs(os.path.join(work, 'spec'))
open(os.path.join(work, 'spec', 'heroes-spec.md'), 'wb').write(now)
r = subprocess.run(['patch', '-p1', '--quiet'], cwd=work, stdin=open('tools/synthesis-adopted.diff', 'rb'),
                   capture_output=True)
if r.returncode != 0 or read(os.path.join(work, 'spec', 'heroes-spec.md')) != adopted:
    bad.append('arms/spec-adopted.md is not now with tools/synthesis-adopted.diff applied')
shutil.rmtree(work, ignore_errors=True)
a = adopted.split(b'\n')
n = narrowed.split(b'\n')
if not line.endswith(b'\n') or line.count(b'\n') != 1:
    bad.append('tools/narrowed-line.txt is not one line')
built = b'\n'.join(a[:400] + [line.rstrip(b'\n')] + a[401:])
if built != narrowed:
    bad.append('arms/spec-narrowed.md is not adopted with line 401 replaced by tools/narrowed-line.txt')
if len(a) != len(n) or [i for i in range(min(len(a), len(n))) if a[i] != n[i]] != [400]:
    bad.append('adopted and narrowed do not differ in line 401 alone')
if not a[400].startswith(b'Anything beyond this document') or TAIL not in a[400] or TAIL not in line:
    bad.append('line 401 is not R6\'s sentence and its paragraph in both arms')
else:
    alone = line[:line.index(TAIL)] + b'\n'
    if sha(alone) != ALONE_SHA or len(alone) != 834:
        bad.append(f'the narrowed text alone is {len(alone)} bytes, sha256 {sha(alone)[:12]}, not the registered 834, cdd29e79')
    if a[400][a[400].index(TAIL):] != line.rstrip(b'\n')[line.index(TAIL):]:
        bad.append('the paragraph\'s tail after the two sentences differs between the arms')
for b in bad:
    print(b)
sys.exit(2 if bad else 0)
```

### tools/inputs.sha256, whole (the files `make_sessions.py` asserts and `check_folders.py` reads; `tools/spec-now.md` and `tools/narrowed-line.txt` in no session's folder)

```
arms/spec-adopted.md 70b8e497cea7f490e7884d4b7f5d66ba95866d43b0bf16b44e8acbb7d513f1a7
arms/spec-narrowed.md 6874087d96bde1f310ec59eab39b97473154b83ab500e5bf9fa68c5a300374cc
tasks/task-t1.md 3d71ca51116a47bee0f63d3304710870ef7c4d5a4935333ed7963f1b247b41a2
tasks/task-t2.md 93d91f7266adecfe5965b75c6daedec78fc7baf24454d313c1ef725c4b26c62e
programs/p1/heroes.toml c8efd32b4257679e458ecba3d1d52e81a5b2af553b18fe81e5e4483e945c7f59
programs/p1/main.hero ac72ca78bb89fed4ef96bbbae80af1019b74a67896a1b38f8c26bf3057c08b9e
programs/p1/ticks/clock.hero 67b33882a4c07cc7bf2a2e58331d9529f9417b446e04b32c335063f2237dd158
programs/p2/heroes.toml c8efd32b4257679e458ecba3d1d52e81a5b2af553b18fe81e5e4483e945c7f59
programs/p2/main.hero ac72ca78bb89fed4ef96bbbae80af1019b74a67896a1b38f8c26bf3057c08b9e
programs/p2/ticks/clock.hero f4169b4bb46c21079d343358d3121089080716b684715e80e0d3a53f5ea8e7a5
programs/p3/heroes.toml c8efd32b4257679e458ecba3d1d52e81a5b2af553b18fe81e5e4483e945c7f59
programs/p3/main.hero ac72ca78bb89fed4ef96bbbae80af1019b74a67896a1b38f8c26bf3057c08b9e
programs/p3/ticks/clock.hero f600d34346bf8d0415308313af851e03c0d419d6a821ea604628db0603cbbf38
programs/p3/ticks/raw.hero 84e854f0512859bc052b37a65431059eb36057ea9f3c64ed776ad9b14387c1a2
programs/p4/heroes.toml c8efd32b4257679e458ecba3d1d52e81a5b2af553b18fe81e5e4483e945c7f59
programs/p4/main.hero 06f0b35b099cd76883099e571d5e33a44b8d19c8e709e4890114aa2f7a7edb6e
programs/p4/ticks/clock.hero 67b33882a4c07cc7bf2a2e58331d9529f9417b446e04b32c335063f2237dd158
programs/p5/heroes.toml c8efd32b4257679e458ecba3d1d52e81a5b2af553b18fe81e5e4483e945c7f59
programs/p5/main.hero ac72ca78bb89fed4ef96bbbae80af1019b74a67896a1b38f8c26bf3057c08b9e
programs/p5/ticks/clock.hero f4b432676570103ab71748020ab43088ba5aae21296679c9921c511894eeeac5
programs/p5/ticks/myclock.h 18eaf2110a6f66b865ebf617d72e37661fedb0d6101ed00798e73e9d69a32983
programs/p6/heroes.toml e6ef29f563da1511e592cea27ca1c31f510370474d21693c0b80331d68286a45
programs/p6/main.hero 5a38d6b33de9ea3fa4d6d82044721126decb3aa3f6585ab59f0663ee6db5a277
programs/p6/num/ops.hero ae5ebb881febf97609f813d32a6d2b85d5882f25d2f10a5d000acfd5c5df2793
tools/spec-now.md c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee
tools/narrowed-line.txt 21af541ddd7eb73de431312656e80fedf27d6c6145f20b433bd4a8dadd95b23e
```

## The hashes of the runner as registered (the area, 08:37, before any session; `tools/registration-runner.md` is this file)

```
96de383382ff892cb54fb030f5d4f2f4b54aae58532016c15e10f4e24abf9aec  run.sh
fd357f669544cb38a1dedb1d94ffb7703cfd0ea46af6d695d6fd1e75cc24f692  preflight.sh
e5b8bd741da31af51a2491adf2adc8e8e110133251f7b0f11580ad470ece48aa  tools/after.py
ca8e20776dfdb6253ccf101e30f9e829ec2401d3d184ba2d95c6d389bd058beb  tools/brief-session.md
083089c60775136c80fec0599033d250d93032a9a7b66c4d9ac0914bd530e169  tools/check_area.py
98639241217b9f314bbf4a39b219ad8affd3f5f891d3e3e6f3a456da277f957e  tools/check_arms.py
49b4871fc8604e513de7ac64a3d6115d12775527d930031a4f5c2c2e44b150c8  tools/check_folders.py
9746a6266e48cc34514c23510f95e3eae745e0329d8e8c8261ea20b4ae900ed5  tools/inputs.sha256
19bd0da1bcf6733c7e7471dd09022e9ff8ab2da23f6cac47ca411559eff49c33  tools/make_sessions.py
21af541ddd7eb73de431312656e80fedf27d6c6145f20b433bd4a8dadd95b23e  tools/narrowed-line.txt
5603d2b82a2089519ad98fc9d38203e765ab71c401bc0137b7b215003d7fc376  tools/read_runs.py
a1519e78f9154349e9d1c58183bfcf0f390fd3a6cba9cf795f472b2682711b1d  tools/reads.py
f9fcd8a8a1808fdd71c6451315c572260ad1223f9617754aa0859ac8373ff8a7  tools/registration.md
41aae8419d682a6d110d276998070924cb5fb259ed3993e234a3847e7a99d5c1  tools/scan.py
c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee  tools/spec-now.md
0d09532b49d7c848cb8ad9945a327adc63cc5f758dfd02e946acf10bbfb6139b  tools/spent.py
78b15a71fb699447785a982246efcefae5a871b1b37be432527bd2105de4fc17  tools/synthesis-adopted.diff
4708a527bbbb85799eae3eeaf3f935b183a184279504f6860064c6d7f5e3bdcb  order.txt
4178bfd9f38d1dde6aa3e902e35cdaf2b8103303afc35c11f6a53fd4b884093c  key.txt
```

## Corrected 2026-11-27 at 09:03, after the run

The time *"run 08:33"* in § What changes, in words, for the `bash -c` test of the first run's preflight checks, was
written without a clock read at that moment: the test ran between the clock readings 08:33:57 and 08:35:23. Nothing
else above is changed; the area's copy, `tools/registration-runner.md`, holds the bytes as registered before any
session (sha256 `2a74857dacba86b86a6eb86019af2e0b763490109ee277945752b6618996d554`).
