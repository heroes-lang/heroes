# Panel 254, llm-ergonomist (the blind seat): prepared, its free preflight clean, its paid sessions not run

Written 2026-11-22 at 04:05 by the sitting's coordinator, who prepares and runs this seat (SKILL.md § 2).
Every number below is a command's, named beside it.

## Where it stands

- **The area** `.claude/worktrees/p254-scratch/254-llm-ergonomist/`, an empty git repository of its own
  (`tools/check_area.py` exit 0), its arms at the hashes of `docs/panel/254-briefs/arms/arms.sha256`
  (`now` `ffd687d6...`, `m` `b008c307...`, `m0` `1dc1446d...`).
- **The draw** (`tools/make_sessions.py`, 2026-11-22 at about 02:30): `now=q m=y m0=p` (`key.txt`'s first
  line); **26 sessions** in `order.txt` (`wc -l`), 25 graded folders under `s/` and the verdict's;
  `tools/check_folders.py` exit 0.
- **The spec-warden's registered predictions** were copied by the runner's gate at 02:31:28 to
  `warden-at-start.md`, sha256 `fd1418d769bd62b229d85114bab6ecac070b33f5ff8db23de681cfec678aee6b`
  (`run.log`; `shasum -a 256` read again at 04:03): W1 to W11 on the blind counts, registered at 02:11.
- **The free preflight** (`preflight.sh`, `run.sh` cut before the line that reads the key, so no billed
  session can run from it; written for this purpose): **cost 0**, the session `580fc549-...` answered
  `api_error` on the refused key, `subtype` `success`, `is_error` true, `terminal_reason` `api_error`;
  `/private/tmp/claude-501/` listed before and after, **0 lines differ**; `tools/scan.py`: *"prompt-alone=yes
  git-status=reader, no commit claude-md-or-memory=no"*; `tools/reads.py`: 1 transcript, 0 calls, 0
  paths outside its folder.
- **Spent: 0.000000 USD** (`tools/spent.py`). **No paid session ran.**

## Why the paid sessions did not run

The paid run's allowance, up to 20 USD for the sitting, reached this coordinator only as the relay of
the M-rebel-library coordinator, twice (*"the author allowed it on 2026-11-22"*). A paid run is the
author's to allow (`.claude/rules/verification.md` § The batch: *"Four kinds stay the author's every
time: ... a paid run"*), and this coordinator, a session the author does not speak to, does not treat an
agent's relay of the author's word as the author's consent. So it prepared everything the run needs and
stopped before the key. **The run starts with one command, from the session that holds the author's
word**:

```
bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/254-llm-ergonomist/run.sh
```

It skips the preflight (already run), re-checks the area and the folders, copies the warden's report
again to a `warden-at-resume-<time>.md` beside the first copy (only `warden-at-start.md`'s predictions
score, as registered), reads the key and prints its length alone, runs the 26 sessions one at a time
with `--max-budget-usd 3` each, and stops before any session that would take the spend past 20 USD.
After it: `tools/scan.py`, `tools/reads.py`, every session's `run.json` read for `subtype`, `is_error`,
`terminal_reason`, `total_cost_usd` and `modelUsage`, and the grades by the rows of
`docs/panel/254-briefs/llm-ergonomist.md`, written here under a heading of their own.

## What the registered run measures, and what it no longer can

The completeness critic's second reading (`critic-reports.md` finding 17), checked here by reading:

- **The arms are the proposal's**, sha256 above: the run measures `spec-m.md`, the text put to the seats,
  not the sitting's resolution (`254-reports/resolution/spec-r.diff`), which holds the amendments the
  seats measured (S1's table, S6's failed send, the stop's drain, S11 by thread, W-a to W-j).
- **t2's program 3 row** (`receive: stopped`, the proposal's stop that wins over queued values) is the
  proposal's answer; under the resolution's stop (a drain, then `stopped` for ever) the same program
  prints `got queued`. Graded on arm `m`, the row scores a reading of the proposal's rule, as registered.
- **A fourth arm, the resolution's text, owes its own registered predictions before any session of it
  runs**, and program 3's row re-registered for it (`got queued`). Not drawn: adding cells after the
  draw would change the 26-session design the spec-warden predicted. Owed with the run.
- W-h's divergence and the hand-over's form (S11's W-j, adopted, against W-j' and W-j'') are locality
  questions this seat exists to read; none is in the registered tasks.
