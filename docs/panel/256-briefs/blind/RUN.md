# Panel 256, the blind seat's paid run: what to start, and where

**For the coordinator of M-rebel-library, who starts it on the author's word of 2026-11-22 (up to 20 USD).
This sitting's coordinator did not start it, and no seat did.**

## The folder

`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/`

The top of an empty git repository of its own (`.gitignore` `*`, local user `reader` <`reader@invalid`>, no
commit), inside this repository's root and ignored by it; `home/` and `tmp/` beside the 46 session folders
`s/<task>-<label>-<n>/` and `verdict/`, each holding only its registered files (`tools/check_folders.py`
exit 0, `tools/check_area.py` exit 0, `git status --short` 0 lines, all read 2026-11-22 23:03).

## The command

```
bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/run.sh
```

One runner at a time (`run.lock`); it holds the machine awake itself (`caffeinate -i -w $$`); a stopped run
resumes, a folder holding `run.json` skipped and its cost counted. **Expect 46 sessions, one at a time, about
40 to 60 minutes** (panel 254's 26 took 35 minutes, `09:27:34` to `10:03:16`), and **8.5 to 16.0 USD**, an
inference from panel 254's measured costs; the runner stops before any session whose cap (1.00 USD, the
verdict 1.50) and a 0.10 margin would take the sum past **19.50 USD**.

## What it checks before it spends (each refuses the run where it fails)

1. The spec-warden's predictions stand in `lane-p256/docs/panel/256-reports/spec-warden.md` under `## Predictions
   registered before the blind run`; the report is copied and hashed as the runner starts (the first copy was
   made by the preflight, `warden-at-start.md`, sha256 `2bd2ae1ccdc6be5e...`; a later start writes
   `warden-at-resume-<time>.md`).
2. `tools/registration.md`'s sha256 is **`fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92`**,
   written into `run.sh`; it is copied beside `run.log`.
3. `tools/check_folders.py` (46 folders at their hashes) and `tools/check_area.py`.
4. **The free preflight, already run** at 23:02:41 by this sitting's coordinator (`bash preflight.sh`, the
   runner cut before the key is read): a key the API refuses, **cost 0**, no file naming its session under
   `/private/tmp/claude-501/` (the listing before and after differs in 0 lines), and the scan clean:
   `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no` (`run.log`). Claude Code 2.1.323.
   The runner reads the preflight's `run.json` again and stops if it is not clean.
5. The key from the main checkout's `.env`, its length alone printed.

**One correction before any paid session, said here because the registration's hash moved**: the first
registration (`cb686925...`) carried three clock times its coordinator had not read (23:20, 23:25, 23:35);
they were removed from it and from the blind brief at 23:03, its predictions and every other line unchanged,
and it was registered again as above (`run.log`'s line of 23:03). The draw was made between the two
(`key.txt`), so the corrected file was written after the draw; nothing in it but the time stamp moved.

## After the run (no cost)

```
cd /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist
python3 -I tools/scan.py | tail -3
python3 -I tools/reads.py | tail -2
python3 -I tools/read_runs.py .
```

Then hand the folder back to panel 256's coordinator to score: the rows, the rules and what the run does to
the resolution are registered in `docs/panel/256-briefs/llm-ergonomist.md`, the predictions in
`registration.md` and the spec-warden's report, and the report goes to
`docs/panel/256-reports/llm-ergonomist-blind.md`. **The draw is not blind to the scorer**: it was printed into
the coordinator's transcript by `make_sessions.py`, as panel 254's was; the registered rows are what keep
the grading honest.
