# The blind seat: what a model does with what a failure says

**Redesigned 2026-10-26, before any session ran**, on the critic's first pass
(`docs/panel/230-reports/critic-briefs.md`, findings 14 to 21, 28 to 30 and 39), and **re-asked of
the author**: four long sessions, one per family and variant, could show no difference by this
file's own rule, so the author's yes of that day changed the shape to **84 short sessions within the
same 12 USD**. The first design's files were never run; the digests the critic recorded of them
(finding 30) are that design's.

**What the seat measures**: whether the proposal's text changes what a model does when a program has
failed (family P) and what a model writes when a program must fail (family M). Each cell is read by
**three sessions, one task each**; the two variants are label-stripped: **T**, today's specification
(`git show 595d92d88:spec/heroes-spec.md`) and, in family P, today's stderr; **P**, the proposal's
specification (T with `00-shared.md`'s three sentences, each asserted to match once) and, in family
P, the stderr the proposal would print.

## Family P, *fix what stopped*: 8 programs × 2 variants × 3 sessions = 48, on Haiku 5.5

`brief-p.md` as `brief.md`, `tasks-p/<item>-<T|P>.md` as `task.md`. Each program stops with an abort
that more than one site of it could have raised, and holds one planted mistake; shown: what it
should print, what it printed, its stderr, its status (134). **The two variants of an item differ
in lines added beneath the stderr's first line and nothing else** (`diff tasks-p/<item>-T.md
tasks-p/<item>-P.md`: one added line each, the place, and three for `rects` and `roster`, the place
and the two sides; no line removed).

| item | the abort | sites that could raise it | the planted mistake | P's place |
|---|---|---|---|---|
| `report` | index | five index reads | a loop bound, `<=` for `<` | `report.hero:13:44, in report.windows` |
| `basket` | `.must()` | two `.must()` | a key misspelt in the caller | `basket.hero:18:12, in basket.line_total` |
| `rects` | `assert` on records | one | a field built by the wrong helper | `rects.hero:35:16`, and both sides |
| `checksum` | overflow | six arithmetic operators | the modulus after the loop | `checksum.hero:11:13, in checksum.checksum` |
| `histogram` | index | three index sites, a write among them | one bin too few, upstream of the read | `histogram.hero:23:18, in histogram.tally` |
| `shipping` | `.must()` | two `.must()` | a key built with `c-` for `c` | `shipping.hero:28:16, in shipping.main` |
| `roster` | `assert` on `[Person]` | one | a stable sort broken by `>=` | `roster.hero:39:12`, and both sides |
| `excerpt` | slice through a character | two slices | a cut not moved to a character's start | `excerpt.hero:15:12, in excerpt.preview` |

**The place's convention**, the proposal's: the column is the start of the expression whose
evaluation stopped the program, measured in the emitted C where a line holds more than one (in
`histogram` line 23 the read `out[b]` at column 18 is checked before the write at column 9); a
`.must()` that fails in a callee names the callee (`basket`, where the mistake is in `main`). The
§ 6 sentence of P's specification names *the file, line and function* and P's stderr writes a column
too (finding 38); the session reads both, and so does the seat's result.

**Why Haiku 5.5**: measurements 053 and 054 read 27 of 27 and 60 of 60 at the first try (finding
18), so a strong model would likely read the same in both variants whatever the text; the smallest
model the trial pins (`tests/thesis/held-out/prediction.md`) is the likeliest to differ, an
inference. The programs, their T stderr (the lane's `heroes-next`, `sources 413da33e7969760a`), the
hand-written P stderr and the reference fixes (each run: `report`, `basket`, `rects`, `checksum`,
`histogram`, `shipping` and `roster` change one line, `excerpt` adds a loop of three) are in
`.claude/worktrees/panic-location-scratch/blind230/items/`.

## Family M, *say that it failed*: 6 tasks × 2 variants × 3 sessions = 36, on Sonnet 5.5

`brief-m.md` as `brief.md`, `tasks-m/<task>.md` as `task.md`, the same text in both variants; only
the specification differs.

| task | the statuses | what the error cases judge |
|---|---|---|
| `sum` | stated: *a status other than 0*, nothing on stdout | 5 error cases (a bad line, no argument, **two arguments**, an unreadable file, `+5`) |
| `capital` | **not stated**: *an error, and the program reports it* | 2 (`Germany`, `italy`) |
| `port` | **not stated** | 3 (no `port`, a line without `=`, an unreadable file) |
| `words` | **not stated** | 1 (an unreadable file) |
| `date` | **not stated** | 4 (`2023-02-29`, `2023-3-1`, `2023-13-01`, `1900-02-29`) |
| `tally` | stated exactly: **1** for a bad line, **2** for the arguments or the file | 3 |

**The default**, issue 1001's subject, is read on the four tasks that state no status: **10 error
cases a session, 30 a variant**. An error case of a default task passes when the status is not 0
and something was written on stdout or stderr (*reports it*); `tally` reads whether a writer of M1's
one status still gives the 2 the task asks (finding 27). The cases are
`.claude/worktrees/panic-location-scratch/blind230/m/cases/` (33), run by `m/run-cases.sh`: **a run
past 10 s (`timeout`'s 124) fails as a HANG** and is counted apart (finding 17); a case asking a
status other than 0 that ends at 0 is counted **SILENT**. The coordinator's references pass every
case on today's compiler (8, 4, 5, 4, 7 and 5 of their cases) and four written as M1 (`main -> ()?`,
`?`, `return fail(...)`) pass every case of theirs on the prototype; four mutants are each refused
for their reason (a `sum` taking the first of two arguments, SILENT; a `capital` exiting 1 and saying
nothing, *nothing reported*; one printing and ending at 0, SILENT; one looping, HANG).

**The P variant's compiler** is the coordinator's prototype, `.claude/worktrees/230-coordinator/heroes-m1`
(`sources 4e17d16b5df3eede`), built from `595d92d88` with two files changed (`git diff --stat`:
`selfhost/check/decls.hero` 6 lines, `selfhost/emit/c_main.hero` 21; the diff's sha256 `319cff23...`):
`main_returns` accepts `()?`, and the C `main` writes `error: <code>: <msg>` on stderr and exits 1
when `main` returns a `fail`. It never merges; the seats' prototypes are theirs. Today's programs
build and pass on it as on the lane's compiler (the six references, 33 of 33), and `main -> i64` is
still refused.

## The run

`fill.py` draws the cells' order (`random.Random(230).shuffle`) and fills the lane's `build/b230/1` to
`84` (git ignores `/build`), each with `heroes-spec.md`, `brief.md` and `task.md` alone; the key,
folder to family, task, variant and model, goes to
`.claude/worktrees/panic-location-scratch/blind230/sealed/key.tsv`, which `run_one.sh` reads for the
model and `score.py` never opens. `run_one.sh <n>` runs the session **from inside its folder** (so
`--restricted` confines its reads there) and writes its transcript to `build/b230-out/<n>.jsonl`
beside the folders, never inside one:

```
cd build/b230/<n> && GIT_CEILING_DIRECTORIES=<lane>/build/b230 claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <lane>/build/b230/<n>. You read its files with the Read tool, by absolute path." \
  --model <claude-haiku-5-5 | claude-sonnet-5-5> --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.14 --output-format stream-json --verbose > <lane>/build/b230-out/<n>.jsonl 2> ...
```

**The budget**: 84 × 0.14 = 11.76 USD at most, inside the author's 12. **The first session of each
model runs alone and its cost is read before the others start**; one the bound stops is scored as no
answer and counted, never re-run; if the first session of a model is stopped by the bound, the run
stops there and the author is asked. The key is the trunk's `ANTHROPIC_API_KEY` from `.env`, as
panels 227 and 228 used it.

## Scored before the key is read, by `score.py`, written before the run

- **The context**: a session whose tool calls are anything but `Read` of a path inside its own
  folder is void and counted apart; each session's `context` section is read by the coordinator
  before the key is joined, and void only where it names a project file, rule, memory or commit (a
  `context` naming the system prompt is not a failure, panel 228's amendment).
- **Family P**, per session: **fixed** when the answer's last fenced block holding `function main`,
  written as `<item>.hero` and built by the lane's `heroes-next`, prints exactly the reference output
  and exits 0 within 10 s; **located** when the text before the answer's first fence holds,
  whitespace collapsed, the planted line or an accepted alternative, fixed here: `report` `while i +
  2 <= xs.len()`; `basket` `line_total(prices, item: "chese", count: 2)`; `rects` `y1: lower(a:
  r.y0, b: r.y1)`; `checksum` `h @ h * 31 + v` or `return h % MODULUS`; `histogram` `return top /
  width` or `while i < bin_count(top: top, width: width)`; `shipping` `return "c-" + id.to_str()` or
  a `c["c<N>"] @` line; `roster` `while j > 0 && out[j - 1].age >= out[j].age`; `excerpt` `return
  title.slice(from: 0, to: limit) + "..."`. **Where P's place points elsewhere than the mistake**
  (`basket`, `histogram`), *located* reads whether the place misleads; where it points at it
  (`report`, `checksum`, `excerpt`), whether it helps; the table says which.
- **Family M**, per session: the last fenced block holding `function main`, written as `main.hero`,
  built **by both compilers**, and its task's cases run on each; after the key is joined, a T
  session is read on the lane's `heroes-next` and a P session on `heroes-m1`. **passed** when every
  case passes; **silent** the count of error cases that ended at 0; **hang** the count past 10 s;
  and **`declares_fallible_main`**, a `main -> ()?` in the program: in a T session it is refused
  `main_returns` and counted as no case passed, and it scores **panel 159's llm-ergonomist's
  prediction**, *≥1 in 3 first attempts at a fallible `main` writes `-> ()?`* (fallible: the 18 T
  sessions, every task able to fail).
- **Recorded beside**, never scored: each session's cost (`total_cost_usd`), its output, and its
  `choice_points` section, quoted whole in the seat's report.

**What counts, written before the run**: per family, P against T, each pooled over its cells.
**Family P**, 24 sessions a variant: a variant helps or hurts on *fixed* or *located* where it stands
**4 or more of 24** from the other; a smaller difference is *no difference this sample can show*.
**Family M**, 18 sessions a variant: on *silent* over the 30 default error cases, a difference of
**5 or more**; on *passed*, **3 or more of 18**; `tally`'s wrong statuses read apart. **A veto on the
seat's behalf** is recorded only where P stands 4 or more of 24 below T on *fixed*, or 5 or more
SILENT above T, or 3 or more of 18 below T on *passed*. **What the seat cannot say**, recorded before
the run (finding 28): it reads the proposal against today, never one route against another (no item
separates S1 from S2, L1 from L4, or M1 from M5), and in family P the specification and the stderr
change together, so a difference belongs to the whole proposal.

## The instruments, fixed by their digests before the first session

`shasum -a 256`, read 2026-10-26, in `.claude/worktrees/panic-location-scratch/blind230/`:

```
a84f1e8c5810d18b1400cf6efa9b4c369e9bb0c28d7f80723ef5fe305436a316  fill.py
49bde22825d8612547b5b4cd9816629b1892739d7128cf45dce86e3b5339e8d1  run_one.sh
4d3fa8b08219407eaac8acb505ac9a4315a280e0102365ebc72517965dc969eb  score.py
7e4517cd8f6de0e1049ca1b95f08caaef577be4b644467af7b687463cfd905f9  m/run-cases.sh
5b69e6a8c6b327d4f47fd0c7ad327b489a93403d1a430dc2df486da5d3c2f3a1  spec-T.md
aa11f940102ed438e68d9217c736fd969fa8880972e4fbc947ae59c600e0ff5e  spec-P.md
4c72ba12308391f07c405ac7c96493d4c13700e311420db4ced129ca9a998813  m/cases (find -type f | sort | xargs shasum | shasum)
aa33637135d38c6a24f7f01b8a6e16aa55162e3e26ca09407d64610d3817a67b  items/ref/*.out (ls | sort | xargs shasum | shasum)
319cff2326abdc251664edfc84faf3b86be647e0a31f2bb621d307a18b5385ef  git -C .claude/worktrees/230-coordinator diff
```

`score.py` was run once before this list was taken, on three made-up sessions in a folder since
removed (a right `report` fix, *fixed* 1 *located* 1; a wrong one, 0 and 0; a `capital` written as
M1, refused `main_returns` on today's compiler and 4 of 4 on the prototype); every file goes into the
sitting's evidence at the synthesis, and a change after the first session is written beneath this
section with its date.

**The seat's report** is the experiment's result, written by the coordinator in
`docs/panel/230-reports/llm-ergonomist.md` from `scores.tsv` joined with the key, every number with
its command.
