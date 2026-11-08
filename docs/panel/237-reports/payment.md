# Panel 237, the payment of R6: R4 run again on the landed specification

Written 2026-11-08 by lane p237-land-spec of batch 25, from the coordinator's run of that afternoon.
Every number below was recounted from the run's own files in the lane's session, by the command or
file named beside it; the archive `payment-evidence.tar.xz`, beside this file, holds those files.

## What was registered, and when

R6 of the synthesis, committed at `665ffb09b` (2026-11-08 13:50) and ratified by the author's reading
at `5d9e832b2` (13:58), before any landing and before any session below: *with R6 and R7 landed, R4
run again on the landed specification (the same three models, 72 sessions) binds no close bare under
fence B, and reads `lent_shape` as the first error of at most 19 of its first tries, half of today's
38*; *unrun or falsified, B's +4 and R7's tokens are paid by the removal the spec-warden priced, and a
call returning nothing takes no clause in § 13's `when` paragraph (−10 vendored), counted on the real
tokeniser then*. The author's verdict allowed the run within 10 USD beyond the sitting's cap. Fence B
and R7's sentence landed at `7ee862fcf` (ledger row 8335, 10,915 real).

## How it was run

- **The runner**: `pay237.py`, its words `prep`, `run`, `grade` and `sum`, in the coordinator's
  scratch folder `.claude/worktrees/p237-scratch/blind237/` (inside the repository's root, ignored by
  git), its folder `pay/`.
- **The inputs**: `task-reader-4.md`, R4's task as the sitting ran it (each session's `task.md` has
  its sha256, `727043266bc9176d…`), and the specification as landed at `7ee862fcf`
  (`pay/heroes-spec.md`, sha256 `944901ae64a22fff21e6df2d4edeaff676ff32c7bedd24084e84ea9065419edc`,
  the same as `git show 7ee862fcf:spec/heroes-spec.md`); all 72 sessions' copies of it hash the same,
  so the archive holds it once.
- **The sessions**: `claude-sonnet-5-5`, `claude-opus-5-5` and `claude-haiku-5-5`, 24 each, named
  `R4-L-<s|o|h><k>`, in the order `random.Random(2376)` draws, four at a time; the blind flags of the
  sitting's runner (`--bare --restricted --safe-mode --strict-mcp-config`, `Read` and `Write` alone),
  git blind (`GIT_CEILING_DIRECTORIES`), the key read from `.env` by the runner and neither printed nor
  stored; first turn only, each capped at 0.45 USD, no new session once the reported sum passed 10.00.
  By the files' times the folders were made at 16:08:53 and the last session ended at 16:24:13.
- **The grade**: each answer's last fenced block built by `pay/heroes-pay`, the round's stage 2
  (`heroes-pay --version`: `sources 62dba14251e6eafb`), under `timeout 120`, and its binary run under
  `timeout 20` with the sitting's `shim.dylib`, which writes the SQLite connections still open at exit
  into `connections.txt`; the classes are the sitting's (`237-briefs/blind/README.md` § Grading).
  `grade.out` and `graded/table.tsv` were written at 16:24:42.

## The result

`pay/grade.out`, recounted from `pay/graded/table.tsv`:

| model | sessions | correct | silent | loud | refused | failed | close `when 0` | close bare |
|---|---|---|---|---|---|---|---|---|
| Sonnet 5.5 | 24 | 0 | 0 | 0 | 24 | 0 | 24 | 0 |
| Opus 5.5 | 24 | 20 | 0 | 0 | 4 | 0 | 24 | 0 |
| Haiku 5.5 | 24 | 12 | 1 | 0 | 11 | 0 | 23 | 1 |
| all | 72 | 32 | 1 | 0 | 39 | 0 | 71 | 1 |

- **The first errors of the 39 refused**: `lent_shape` 37 (Sonnet 24, Haiku 10, Opus 3; 34 on a
  handle parameter, 3 on a `ptr` that declares no extent), `needs_label` 1, `expected_args_close` 1.
  All 24 of Sonnet's programs write `sqlite3_prepare_v2(db: Db lent, …`, the handle `lent` that R7's
  sentence now says is no record lent read-only, and that defect 2347's `certain` fix deletes.
- **The one bare close**: `R4-L-h17`, silent: it printed `5` and `0` and exited 0 with one connection
  open, defect 2322's report, under the fence that binds the close `when 0`.

## The verdict

**Falsified, both halves.** The runner's own lines: *bare closes 1 (held when 0); lent_shape first 37
of 72 (held when at most 19)*, *VERDICT: falsified*. At the sitting, R4 under fence B read 0 bare
closes of 18 and `lent_shape` first in 38 of 72 over the four arms; here 1 of 72, and 37 of 72.

## What paid

The removal branch R6 registered: § 13's `when` paragraph loses *and a call returning nothing takes no
clause*, the sentence ending at *`true` or `false` for a `bool`*. The rule stays enforced, by the
checker and in the grammar: the `Member` production admits `when` only after `->` and a type, and
`-> () when 0` is refused `when_on_void` (`selfhost/check/marks.hero`), whose message says to drop the
clause. Priced at its landing, one `--refresh` at 16:31: **−10 vendored** on both tables (8335 to
8325, 8188 to 8178) and **10,915 to 10,900 real** on `claude-opus-5` (−15), digest
`43a8cf64676f5a99`, so the specification stands 4 real tokens under its count before panel 237
landed (10,904). The ledger's row for the removal and its note on row 8335's payment are in
`docs/measurements/010-spec-budget-ledger.md`.

## What `--apply` makes of the refused, descriptive only

Not part of the prediction or its verdict, read after it: `pay/applied/summary.txt`, each of the 39
refused programs given `heroes check --apply --in-place` by the same compiler, then built and run with
the shim. 29 become correct (exit 0, no connection open); 10 stay refused: `lent_shape` 4 (each on a
`ptr` that declares no extent, which keeps no fix), `cannot_infer` 4, `expected_args_close` 1,
`marker_mismatch` 1.

## The cost

72 of 72 sessions answered (`is_error` false in each `run.json`, none retried), **4.5208 USD** by the
CLI's own reports (Sonnet 1.1535, Opus 3.2099, Haiku 0.1574; `python3 -I pay237.py sum` gives the
same total), inside the author's 10 USD; and one `--refresh`, three `count_tokens` requests, whose
price no command here reports.

## The archive

`payment-evidence.tar.xz`: **607 entries**, 38,896 bytes (457,718 bytes of text unpacked): `pay237.py`;
`pay/heroes-spec.md`, `pay/grade.out` and `pay/run.log`; every session's `task.md`, `run.json` and
`run.err` (216, the 72 `run.err` empty); `pay/graded/table.tsv` and each graded program's `main.hero`,
`build.out`, `run.out` and `connections.txt`; `pay/applied/summary.txt` and each applied program's
`main.hero`, `apply.out`, `build.out`, `run.out` and `connections.txt`. Text only: no binary, no
compiler, no `prog`, no `notes.db`, no `build/` cache. Before it was written its files were searched
for a key-shaped string (`sk-ant-`, and every run of 40 or more key characters: a commit hash and the
specification's two catalogue checksums, nothing else), for the key itself (no file holds it), and
for an e-mail address or the author's mail domain (none).
