# Panel 248, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-14 for the coordinator, who runs the sessions and grades them. **This seat is a paid
run, the author's every time** (`.claude/skills/panel/SKILL.md:288-295`; `.claude/rules/verification.md`
§ The batch: *"Four kinds stay the author's every time: a push or any outward-facing act, a panel's
ratification, a paid run, and a destructive operation"*). **The author approved this sitting's paid runs
at 10:30 on 2026-11-14, a ceiling of 25 USD in all**, the blind seat and the spec-warden's `heroes
measure --refresh` calls together (given to the coordinator and relayed to this brief's writer; recorded
as the coordinator's message gives it). **No session has run** when this was written
(`find .claude/worktrees/p248-scratch/s -name 'run.json*' | wc -l`: 0); the coordinator starts them by
the command lines in § How the coordinator starts it.

**Its history, each text kept beside this file**: the first registration, twelve sessions at a proposed
5 USD (`llm-ergonomist.before-second-pass.md`); repaired before any session on the critic's second pass
(`docs/panel/248-reports/critic-briefs.md` § Second pass, its items 1, 2, 9 and 15: the runner, arm d's
card, what a session sees and is checked for; `llm-ergonomist.before-widening.md`); then **widened on
the author's approval to ten sessions a cell, forty in all**, the runner's stop at 16.00 USD (this
text). The tools before each change stand in `248-llm-ergonomist/tools/` as `*.before-second-pass.*` and
`*.before-widening.*`, the maps and orders as `s-map.before-*.txt` and `s-order.before-*.txt`.
Everything in § The seat's brief and § The tasks is copied verbatim into the session folders and
carries none of the project's names; everything else is for the coordinator.

## What it measures

Whether a reader holding only the specification and the `net` package's card writes, on the first try,
**t4**, a server that ends itself on a request (question 4), and **t5**, a server that closes a
connection that sends nothing (question 5); with today's card and with the candidate face. Each
session's folder holds the base's specification (`heroes-spec.md`, the same in all forty) and a
`task.md` carrying the task and the two cards; **the arm is the card, label-stripped**: nothing in a
folder says which face it holds.

| arm | `net`'s card in `task.md` | sha256, 16 | `net/http`'s card |
|---|---|---|---|
| x | core 0.3.0's, verbatim (`packages/core/0.3.0/files/net.md`, `cmp`: equal) | `f5aa1e67f34dd188` | 0.3.0's verbatim, `37f68b5d0eabcd6d` |
| q | x with two lines after `catch_stop`'s (below) | `10019ee4c190f74b` | the same |
| d | x with one line after `write`'s and `timed_out`'s line widened (below) | `f2d1e86f7cc08cbb` | the same |

The two hashes of 0.3.0's cards are the published list's own (`packages/core/0.3.0/list`: `f5aa1e67...
net.md`, `37f68b5d... net/http.md`). The faces are made by
`.claude/worktrees/p248-scratch/248-llm-ergonomist/tools/cards.py` (`05b972ec6f97186f`), each insertion
asserted to land exactly once; `diff` of each against x prints exactly these lines:

- **q** (question 4's candidate spelling):
  - `` - `ask_stop() -> ()?`, called `net.ask_stop()`: Asks the program to stop, from any thread, as Ctrl-C does once `catch_stop` was called: from then every call here that would wait fails `stopped`, on every thread. ``
  - `` - `stop_asked() -> bool`, called `net.stop_asked()`: Whether the program was asked to stop, by Ctrl-C, SIGTERM or `ask_stop`. ``
- **d** (question 5's candidate spelling):
  - `` - `set_deadline(c: Conn, ms: i64) -> ()?`, called `net.set_deadline(c, ms)`: From this call on, a `read` or `write` on `c` that waits more than `ms` milliseconds fails `timed_out`; 0 takes the limit away. ``
  - `timed_out`'s line becomes `` - `timed_out`, `net.TIMED_OUT`: The peer did not answer in time, or a wait passed the limit `set_deadline` gave. ``

  **Go's name is gone from d's line** (the critic's second pass, item 2): the first version ended *"Go's
  `Conn.SetDeadline`, counted from each call that waits"*, while Go's page says the opposite of the
  last clause (`curl -s -L -o briefs/fix/web/go-net.html https://pkg.go.dev/net`, 509,912 bytes, read
  2026-11-14: *"A deadline is an absolute time after which I/O operations fail instead of blocking. The
  deadline applies to all future and pending I/O, not just the immediately following call to Read or
  Write."*), so a reader who knows Go was told one mechanism by its name and another by its words. The
  line now describes the candidate alone, a limit counted from each call that waits; the other lines of
  the card keep their own references to Go, which are core 0.3.0's (`grep -c -i "go's"`: 8 in x, 8 in d).

**These spellings are candidates, written before any seat built a route.** The compiler-engineer's and
the ffi-pragmatist's briefs name them; **a session is graded only against a face that was built**, so
if the door the seats build is spelled otherwise, or behaves otherwise than its line says, the
coordinator changes `cards.py`, empties `s/` (nothing having run in it) and runs `sessions.py` again,
and records the change here, with the old text kept beside this file, **before any session runs**. A
spec arm (the spec-warden's q4m or q4a, `spec-warden.md`) would be a registration of its own, before its
sessions, and a paid run of its own.

## The sessions, their folders, their order and their bound

**40 sessions**: t4 under x and q, t5 under x and d, **ten each**, every one capped at
`--max-budget-usd 0.40`. **The folders' names say nothing of the seat, the task or the arm**: each is
six random hexadecimal digits under **`.claude/worktrees/p248-scratch/s/`**, not under
`248-llm-ergonomist/`, because a session's working folder is in what it is shown, and panel 247 kept its
sessions under `p247-scratch/s/` for that reason (`docs/panel/247-briefs/llm-ergonomist.md:52-57`, on
panel 246's open question, *"a name saying less than `246-llm-ergonomist/` is a question for the next
sheet"*). The seat's own files are under `.claude/worktrees/p248-scratch/248-llm-ergonomist/`: `src/`
(the brief, the tasks, the cards), `tools/` (the scripts), `s-map.txt` and `s-order.txt`, never inside a
session's folder.

**A session's working path still names the sitting** (the critic's second pass, item 9): it is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p248-scratch/s/<hex>`, so a session can read
`heroes-lang`, a worktree and `p248-scratch` in it, never the arm or the task, which no path and no file
of the folder names. Panel 247's sessions saw the same shape of path, and one of its forty `context`
answers named it (`.claude/worktrees/p247-scratch/context.tsv`, session `597f7e`, the critic's
reading: *"names the folder's path (a `heroes-lang` worktree, `.claude/worktrees/p247-scratch`)"*).

**Both folders are inside the repository's root**, which CLAUDE.md § Hard stops requires (rank 1) where
`SKILL.md:133-134` asks a copy outside it, as panel 247's ninety-six sessions ran. Walking up from a
session folder to `/`, what stands above every one (the critic's item 15, re-run): the repository's
**`CLAUDE.md`** (`/Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md`, the only one; no `CLAUDE.local.md`,
no `~/.claude/CLAUDE.md`), the repository's **`.claude/`** with **`.claude/settings.json`**,
**`.claude/settings.local.json`** and **`.claude/hooks/`** (`ls -d`), and the user's `~/.claude/`.
`--safe-mode` disables `CLAUDE.md`, skills and hooks and `--restricted` ignores the user, project and
local settings (`SKILL.md:171-175`); **whether they do so for this CLI is unrun**, since asking would be a
session. **So each `context` answer is read for those by name**: any trace of `CLAUDE.md` or its rules,
of the settings (a permission, a language, an added directory) or of a hook's output (a refusal, an
injected line) reaching the session; a `yes` naming any of them voids that session until it is re-run,
and the path alone (above) is recorded and does not void it. Every folder is git-ignored (`git
check-ignore -v .claude/worktrees/p248-scratch/s`: `.gitignore:34:.claude/worktrees/`).

**The folders, made again for the widening**: the twelve of the second pass, in which nothing had run
(`find s -name 'run.json*'`: 0; 36 files, every one `brief.md`, `task.md` or `heroes-spec.md`), were
emptied, and `tools/sessions.py` (`c3e0e2de463d9f17`) run once, drawing forty new names. **Each folder
re-checked**: `ls -A` printed `brief.md heroes-spec.md task.md` in all forty, `find s -type f | wc -l`
120; each `heroes-spec.md` `cmp`-equal to `git show df7612ad3:spec/heroes-spec.md` and each `brief.md`
to `.claude/worktrees/p247-scratch/s/027658/brief.md` (no folder differed); the map's three hashes
matched each folder's files in 40 rows of 40 (`python3 -I` over `s-map.txt`); `grep -i -o -w -h -E` over
the forty `brief.md` and forty `task.md` for *panel, sitting, seat, arm, coordinator, ergonomist, llm,
blind, candidate, face, grade, rubric, critic, warden, historian, defect, decision, milestone, batch,
lane, worktree, scratch, p248, p247, 248, 247, 246, t4, t5, q4m, q4a, q5d*, a milestone's `M-` name,
*claude, anthropic, opus, CLAUDE*, the author's names and *heroes-lang* printed nothing; `SetDeadline`
stands in none.

**The order**: ten rounds, round k carrying replicate k, each round the four cells in an order drawn at
random (`random.SystemRandom`) so that **no two consecutive sessions share a cell** (a draw whose first
cell repeats the previous round's last is drawn again): counted over `s-map.txt`, ten sessions a cell
and 0 consecutive pairs of one cell; a stop leaves every cell within one round of the others.
The cells in run order: `t4-q t5-d t4-x t5-x | t4-x t4-q t5-d t5-x | t5-d t4-q t4-x t5-x | t4-x t5-d
t5-x t4-q | t4-x t4-q t5-x t5-d | t5-x t4-x t5-d t4-q | t5-d t4-q t5-x t4-x | t5-x t4-q t5-d t4-x | t5-d
t5-x t4-x t4-q | t5-d t4-q t4-x t5-x`.

**The map, the coordinator's alone**: `.claude/worktrees/p248-scratch/248-llm-ergonomist/s-map.txt`
(`7b208983b6b0e6c9`; each row: order, session, folder, then the first 16 of `shasum -a 256` of
`heroes-spec.md`, `brief.md` and `task.md`), the order in `s-order.txt` (`9c5c0f13d48ea1e7`), both written by
`sessions.py`, never inside a session's folder. The four `task.md` hashes, one per cell: t4-x
`c831407013e92305`, t4-q `5e2de4fe299c71ef`, t5-x `f17b4264a32a2fd8`, t5-d `01cd81a9a476ec53`; every
`heroes-spec.md` `163219f2fa69fe82` (the base's) and every `brief.md` `1d0b6a53cddcb0a0` (panel 247's):

| order | session | folder under `s/` | `heroes-spec.md` | `brief.md` | `task.md` |
|---|---|---|---|---|---|
| 1 | t4-q-1 | `99ff9a` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 2 | t5-d-1 | `07b6a2` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 3 | t4-x-1 | `517cd7` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 4 | t5-x-1 | `25026a` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 5 | t4-x-2 | `3f493c` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 6 | t4-q-2 | `d9b285` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 7 | t5-d-2 | `2626a9` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 8 | t5-x-2 | `2d7c2e` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 9 | t5-d-3 | `6cbc48` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 10 | t4-q-3 | `50fc3b` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 11 | t4-x-3 | `f271c2` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 12 | t5-x-3 | `7c4ab7` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 13 | t4-x-4 | `966533` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 14 | t5-d-4 | `6d4a98` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 15 | t5-x-4 | `456dc0` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 16 | t4-q-4 | `8edaeb` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 17 | t4-x-5 | `99a7f7` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 18 | t4-q-5 | `7c12b3` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 19 | t5-x-5 | `2b31db` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 20 | t5-d-5 | `55898d` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 21 | t5-x-6 | `de1015` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 22 | t4-x-6 | `f4ef20` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 23 | t5-d-6 | `797fd8` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 24 | t4-q-6 | `513116` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 25 | t5-d-7 | `839425` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 26 | t4-q-7 | `55fad3` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 27 | t5-x-7 | `92968d` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 28 | t4-x-7 | `e8f07a` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 29 | t5-x-8 | `4015eb` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 30 | t4-q-8 | `b5ab5a` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 31 | t5-d-8 | `34fc17` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 32 | t4-x-8 | `a5f7cd` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 33 | t5-d-9 | `b4e50b` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 34 | t5-x-9 | `0be3d3` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 35 | t4-x-9 | `42faf2` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 36 | t4-q-9 | `57aaeb` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 37 | t5-d-10 | `238ae7` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 38 | t4-q-10 | `291589` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 39 | t4-x-10 | `fb741e` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 40 | t5-x-10 | `5f2ec7` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |

**The command**, panel 247's (`docs/panel/247-briefs/llm-ergonomist.md:116-122`), with one change, the
file it writes while it runs, run from each session's folder by `tools/blind-one.sh`
(`9d66fa0dfc97bc3a`, `bash -n`: parses), the environment holding only these:

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 0.40 --output-format json > run.json.part 2> run.err
mv -f run.json.part run.json          # in blind-one.sh, once the command returns
```

**The runner, and when it stops starting sessions** (`tools/blind-one.sh`). A session writes
`run.json.part` and renames it `run.json` when the command returns (the critic's second pass, item 1),
so a `run.json` is an ended session's alone. Before each start it reads every ended `run.json` under
`s/` and starts the session only while: **every ended one has a readable cost** (else *"STOP before
<hex>: N ended run.json without a readable cost"*); **no ended one is an error other than its own
budget**, `is_error` with a `subtype` other than `error_max_budget_usd`, which is how a refused model or
an API error would end (else *"STOP before <hex>: N ended session(s) in error other than its budget"*;
panel 247's ninety-six `run.json` read 95 `success` and 1 `error_max_budget_usd`, by `python3 -I` over
`p247-scratch/s/*/run.json`, so a session that reached its cap does not stop the rest); and **the sum
of their `total_cost_usd` is at most 16.00 USD** (`BOUND`, else *"STOP before <hex>: sum <s> above
16.00"*). A folder whose `run.json` stands is skipped, so a later invocation resumes; a `.part` a killed
run left is overwritten. `BLIND_S` and `BLIND_BOUND` exist for the simulation alone.

**Proved by a stub simulation, never a real session**
(`.claude/worktrees/p248-scratch/briefs/fix/runner-sim/`): `bin/claude`, a local stub that logs its
call, sleeps 2 to 8 s (`$((2 + RANDOM % 7))`) and prints a `run.json` with the cost in `cost.txt`, or
for a folder named in `bad.txt` an error, a budget's end or output that is not JSON; the PATH given to
the runner `runner-sim/bin:/opt/homebrew/opt/python/bin:/usr/bin:/bin`, on which the only `claude` is the
stub (`env -i PATH=... bash -c 'command -v claude'` printed the stub's path; the real one,
`/opt/homebrew/bin/claude`, is not on it), the key empty; each run over a fresh copy of the forty
folders by `xargs -n 1 -P 4 bash tools/blind-one.sh < s-order.txt`, as `blind-run.sh` runs it:

- **`sim-40.log`, forty stubs at 0.45 USD each, the runner's own bound 16.00**, the fifth folder
  (`3f493c`) ending at its budget (`error_max_budget_usd`): four `START`s at 10:37:25, then each next
  one as one ended, at most four in flight in the log's order (`python3 -I` over the log), the budget's
  end not stopping the rest (*"END 3f493c 10:37:31 exit 1 cost 0.450000 subtype error_max_budget_usd"*,
  the next `START` after it); **39 `START`s**, the last at a sum of ended sessions of 15.75 (*"START
  fb741e 10:38:08 sum of ended sessions 15.750000"*), then *"STOP before 5f2ec7: sum 16.200000 above
  16.00"*, the four in flight ending after; *"stub calls: 39, run.json: 39, .part left: 0"*, 17.55 USD in
  all, under 16.00 plus four sessions' 0.45.
- **`sim-40e.log`, forty stubs at 0.20, the second folder (`07b6a2`) ending in error
  (`error_during_execution`)**: *"END 07b6a2 10:38:38 exit 1 cost 0.000000 subtype
  error_during_execution"*, then 31 *"STOP before <hex>: 1 ended session(s) in error other than its
  budget"*, the three in flight ending after; *"stub calls: 9, run.json: 9, .part left: 0"*.
- The second pass's three runs over twelve folders (`sim-A.log`, `sim-B.log`, `sim-C.log`) stand beside
  them, made with the runner before its error rule and widening (`tools/blind-one.before-widening.sh`).

`pgrep -f runner-sim` found nothing after each.

**The bound, inside the author's 25 USD.** The forty caps alone come to 16.00 USD (`python3 -I -c
'print(40*0.40)'`). But **the CLI can pass its cap by one request**: panel 247's t4-m2-3 ended at
0.4960 USD on a 0.40 cap (`docs/panel/247-*.md:423-424`). With the runner's stop at 16.00 of ended
sessions and four in flight, **the worst case is 16.00 plus four sessions at 0.496, 17.984 USD**
(`python3 -I -c 'print(16.00 + 4*0.496)'`): **an assumption, not a bound**, since it holds only while no
session passes 0.496, the highest per-session cost of panel 247's ninety-six (each from 0.1465 to
0.4960, a mean of 0.2189, 21.0131 in all, by `python3 -I` over `p247-scratch/s/*/run.json`). That leaves
7.016 USD of the 25 (`python3 -I -c 'print(25 - 17.984)'`) for the spec-warden's `--refresh` calls and a
first-session retry. At 247's mean the forty would cost about 8.76 USD (`python3 -I -c
'print(40*0.2189)'`): an inference about these, not a measurement.

## How the coordinator starts it

**Before starting**: (1) **whether the cards' spellings are the ones that will be graded**: § What it
measures says a session is graded only against a face that was built, and the compiler-engineer and the
ffi-pragmatist are building the doors now; a session run on a spelling the seats then change is graded
against nothing, so the coordinator either waits for their spellings and re-makes the folders, or
starts now and records that the candidate spellings are the ones graded. (2) **The machine held
awake** (`.claude/rules/verification.md` § A long run holds the machine awake): `pmset -g assertions`
read `PreventUserIdleSystemSleep 1` at 10:39; if it reads 0, prefix each command below with
`caffeinate -i`. (3) `ls .claude/worktrees/p248-scratch/s/*/run.json*` prints nothing.

**Step 1, the first session alone, the model check**: `claude --version` here prints `2.1.311 (Claude
Code)`; `SKILL.md:185-186` records that 2.1.274 refused `claude-opus-5-5`, and whether 2.1.311 accepts it
is unrun. From the repository's root:

```
cd /Users/joseph/Temp/heroes/heroes-lang
bash .claude/worktrees/p248-scratch/248-llm-ergonomist/tools/blind-first.sh >> .claude/worktrees/p248-scratch/248-llm-ergonomist/blind-runner.log 2>&1
tail -3 .claude/worktrees/p248-scratch/248-llm-ergonomist/blind-runner.log
```

`tools/blind-first.sh` (`043501da5b99512e`, `bash -n`: parses) loads the key from the trunk's `.env`
(`set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`), prints its length alone and the CLI's
version, and runs `blind-one.sh` on the first folder of `s-order.txt` (`99ff9a`, t4-q-1). Read its `END`
line: `subtype success` and a cost go on to step 2; a refused model or an API error shows as another
`subtype` (or an unreadable cost) and an error in `s/99ff9a/run.err`, and **no other session starts**
(the runner's error rule would refuse every later start anyway). Then the model is the coordinator's
decision, recorded here before any further session (`SKILL.md:185-186`: *"or the seat runs on
another"*); the retry runs inside the 7.016 USD left, after moving `s/99ff9a/run.json` aside, never
deleting it.

**Step 2, the rest, four at a time**, from the repository's root:

```
cd /Users/joseph/Temp/heroes/heroes-lang
bash .claude/worktrees/p248-scratch/248-llm-ergonomist/tools/blind-run.sh >> .claude/worktrees/p248-scratch/248-llm-ergonomist/blind-runner.log 2>&1
```

`tools/blind-run.sh` (`c26efdb5a92901e4`, `bash -n`: parses) loads the key the same way and runs `xargs -n
1 -P 4 bash tools/blind-one.sh < s-order.txt`, which skips `99ff9a` (its `run.json` stands) and starts
the other thirty-nine in order as slots free, until the stop rules above; it ends with a `DONE` line, the
number of `run.json` and their sum. The log, `248-llm-ergonomist/blind-runner.log`, holds every `START`,
`END`, `SKIP` and `STOP` line of both steps; it is the coordinator's, read from outside the session
folders. Should a step be interrupted, the same command resumes it.

**The reports**: take `result` from `run.json` where `report.md` is missing, and say which. The
coordinator copies them into `docs/panel/248-reports/llm-ergonomist.md` under a header saying how they
were run, each session's cost and every `context` answer, read as § The sessions says.

## Grading, proposed, registered before any session

**Grading needs the built doors**: arm x is graded with the base's compiler and core 0.3.0; arms q and
d with the compiler and runtime of the seat whose door the card spells, and a core version carrying that
face, each pinned by `HEROES_RUNTIME` and named here by its hashes before any session, as panel 247's
three setups were (`247-briefs/llm-ergonomist.md:150-158`). Every answer's files are written from its
report into a fresh folder under `.claude/worktrees/p248-scratch/grade/<folder>/`, built three times,
and every binary run bounded as 00-shared.md's table says (the server under `timeout -s KILL 60`, each
client under `curl --max-time 5`, the client side under `timeout -s KILL 90`, every stream kept to 65,536
bytes), every Darwin death read in its crash report. Beside its class, each answer's **mechanism** is
recorded: for t4, the door, its own process sent SIGTERM through a C binding of `kill` and `getpid`,
`exit` from a handler's thread, or other; for t5, the deadline, a watchdog thread, a C binding of
`setsockopt`, or none.

- **t4**, run twice: the server started on a free port, then `GET /hello` twice and `GET /quit`, each by
  `curl --max-time 5`: **right** if both `/hello` answer 200 `hello, world`, `/quit` answers 200 `bye`,
  and the program prints `served 3 requests` and exits 0 within 5 s of the `/quit` answer, its leak check
  saying nothing; else **refused** (build exit 1), **stopped** (killed, or an error), **silent** (another
  output or exit code) or **hung** (killed by the bound).
- **t5**, run twice: the server started; three connections opened that send nothing (a probe of the
  coordinator's, each connection with its own 10 s limit), the moment each is closed by the server
  read; meanwhile `GET /hello` by `curl --max-time 5`; then SIGTERM: **right** if each quiet connection is
  closed between 1.5 and 4 s after it opened, `/hello` is answered within 2 s while they are open, and
  SIGTERM ends the program with `stopped` and exit 0 within 2 s; else the same four wrong classes.
- **The references, run before any session**, as panel 247's critic asked of its t2 (`247-briefs/
  llm-ergonomist.md:185-195`): the coordinator writes one right answer per task and face that has a
  built door, and one for x where today's card allows one (t4 by its own SIGTERM; t5 has none the
  coordinator knows of, which the spec-warden's question 4 measures), and grades each five times; a task
  whose reference fails its own rubric is reshaped before any session.
- **The decision rule, registered before any session**: a face is measured to help if its arm gives at
  least **three** more `right` answers than x over the **ten** sessions of its task, both graded under
  the setup that has the door. **Its reason**: panel 247's rule asked three more of eight to twelve
  sessions (`247-briefs/llm-ergonomist.md:196-202`, `354-357`), on a difference no single changed answer
  per task could produce; ten sessions a cell keeps that threshold. The rule of the twelve-session
  design (two more of three) stands in `llm-ergonomist.before-widening.md`. The coordinator may change
  the rule before the first session, never after.

---

## The seat's brief (`brief.md`, verbatim)

Panel 247's seat brief, byte for byte (`1d0b6a53cddcb0a0`, extracted from
`docs/panel/247-briefs/llm-ergonomist.md`, its § The seat's brief):

```markdown
# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model write the program its author meant on the first try. Your verdict is an experiment, not an
opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write whole, with its path. For every place
where the specification left you a choice, say the choice you made and what the other would produce:
a program the compiler refuses, a program that stops with an error when run, or a program that runs
and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks, and every file, whole, each under its path.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `prediction`: what each program prints when built and run, and whether you expect the compiler to
  accept it.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
```

## The tasks (`task.md`, verbatim, one per session)

Each `task.md` is the task's text (`src/task-t4.md`, `2dea9e954ed2892b`; `src/task-t5.md`,
`640cfcc85ca0b2a2`), then a line `## The package's documentation`, then the arm's `net` card, then
`net/http`'s card, joined by `sessions.py`.

**t4**

```markdown
# Task

Write a program, `main.hero`: an HTTP server built on the package `core`, whose two modules
`<core>/net` and `<core>/net/http` are documented below, with the package already fetched for the
program. It takes one argument, a port, and listens on 127.0.0.1 at that port. It serves each
connection on a thread of its own. It answers `GET /hello` with status 200 and the body
`hello, world` followed by a line feed, `GET /quit` with status 200 and the body `bye` followed by a
line feed, and every other request with status 404. Once it has answered a `GET /quit`, it accepts no
more connections, lets every connection it holds end, prints `served N requests` with N the number of
requests it answered, and ends with exit code 0. Give every file, whole, with its path, and say
whether the compiler accepts it when built with `heroes build main.hero` and what it prints when a
client sends `GET /hello` twice and then `GET /quit`.
```

**t5**

```markdown
# Task

Write a program, `main.hero`: an HTTP server built on the package `core`, whose two modules
`<core>/net` and `<core>/net/http` are documented below, with the package already fetched for the
program. It takes one argument, a port, and listens on 127.0.0.1 at that port. It serves each
connection on a thread of its own and answers `GET /hello` with status 200 and the body
`hello, world` followed by a line feed, and every other request with status 404. A client that
connects and then sends nothing must not hold the server: the server closes any connection on which
it has waited 2 seconds without receiving a byte, and keeps answering other clients meanwhile. It runs
until Ctrl-C or SIGTERM, then prints `stopped` and ends with exit code 0. Give every file, whole, with
its path, and say whether the compiler accepts it when built with `heroes build main.hero` and what
happens to a client that connects and sends nothing.
```

## Questions this registration leaves open

- Whether `claude` 2.1.311 accepts `--model claude-opus-5-5` (step 1, run alone, says).
- Whether the spellings on the cards survive the seats' builds (§ How the coordinator starts it, before
  starting, item 1).
- Whether `--safe-mode` and `--restricted` keep `CLAUDE.md`, the settings and the hooks from a session
  (each `context` answer is read for them).

## Added 2026-11-14 at about 10:45, by the coordinator

The candidate spellings in the cards are the ones graded: the seats were still building their doors when the author approved the run, and panel 247 graded candidate variants the same way. Where the seats adopt other spellings, the synthesis says so and reads this seat's result as about the candidates.

## Added 2026-11-14 at about 10:48, by the coordinator

The first session (99ff9a, t4-q-1) ended `success` at 0.294230 USD with no `report.md`: under `--restricted`, claude 2.1.311 gave it `Read` alone (its own words: "the only tool I have is Read"), and it put the whole report in its answer, as the seat brief provides; the report is read from `run.json`'s `result` for every session. The flags are not changed for the other 39, so all forty run alike.
