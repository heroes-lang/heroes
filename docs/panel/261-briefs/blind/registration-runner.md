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
