# Panel 248, llm-ergonomist: the blind seat, prepared and unrun

Written 2026-11-14 for the coordinator, who runs the sessions and grades them. **This seat is a paid
run, the author's every time** (`.claude/skills/panel/SKILL.md:288-295`; `.claude/rules/verification.md`
§ The batch: *"Four kinds stay the author's every time: a push or any outward-facing act, a panel's
ratification, a paid run, and a destructive operation"*). **It is unrun until the author's yes**: the
folders are made, the sessions named, the cap and the ceiling proposed below, and no session has run
(`find .claude/worktrees/p248-scratch/s -type f | wc -l`: 36, `brief.md heroes-spec.md task.md` in each
of the twelve, no `run.json` or `run.json.part`). Without the yes the synthesis records the seat as
unrun. **Repaired before any session on the critic's second pass**
(`docs/panel/248-reports/critic-briefs.md` § Second pass, its items 1, 2, 9 and 15): the runner, arm d's
card, and what a session sees and is checked for; the text before stands beside this file as
`llm-ergonomist.before-second-pass.md`, the first folders' map and order as
`248-llm-ergonomist/s-map.before-second-pass.txt` and `s-order.before-second-pass.txt`, the tools
before as `tools/blind-one.before-second-pass.sh` and `tools/cards.before-second-pass.py`. Everything in § The seat's
brief and § The tasks is copied verbatim into the session folders and carries none of the project's
names; everything else is for the coordinator.

## What it measures

Whether a reader holding only the specification and the `net` package's card writes, on the first try,
**t4**, a server that ends itself on a request (question 4), and **t5**, a server that closes a
connection that sends nothing (question 5); with today's card and with the candidate face. Each
session's folder holds the base's specification (`heroes-spec.md`, the same in all twelve) and a
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

**12 sessions**: t4 under x and q, t5 under x and d, three each. **The folders' names say nothing of
the seat, the task or the arm**: each is six random hexadecimal digits under
**`.claude/worktrees/p248-scratch/s/`**, not under `248-llm-ergonomist/`, because a session's working
folder is in what it is shown, and panel 247 kept its sessions under `p247-scratch/s/` for that reason
(`docs/panel/247-briefs/llm-ergonomist.md:52-57`, on panel 246's open question, *"a name saying less
than `246-llm-ergonomist/` is a question for the next sheet"*). The seat's own files are under
`.claude/worktrees/p248-scratch/248-llm-ergonomist/`: `src/` (the brief, the tasks, the cards), `tools/`
(the scripts), `s-map.txt` and `s-order.txt`, never inside a session's folder.

**A session's working path still names the sitting** (the critic's second pass, item 9): it is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p248-scratch/s/<hex>`, so a session can read
`heroes-lang`, a worktree and `p248-scratch` in it, never the arm or the task, which no path and no file
of the folder names. Panel 247's sessions saw the same shape of path, and one of its forty `context`
answers named it (`.claude/worktrees/p247-scratch/context.tsv`, session `597f7e`, the critic's
reading: *"names the folder's path (a `heroes-lang` worktree, `.claude/worktrees/p247-scratch`)"*).
**No word of the project's names is in a folder's files**: `grep -i -o -w -h -E` over the twelve
`brief.md` and twelve `task.md` for *panel, sitting, seat, arm, coordinator, ergonomist, llm, blind,
candidate, face, grade, rubric, critic, warden, historian, defect, decision, milestone, batch, lane,
worktree, scratch, p248, p247, 248, 247, 246, t4, t5, q4m, q4a, q5d*, a milestone's `M-` name, *claude,
anthropic, opus, CLAUDE*, the author's names and *heroes-lang* printed nothing; what they hold is the
language's own names (`Heroes`, `heroes`, `.hero`, `hero_net` in the card) and `127.0.0.1`.

**Both folders are inside the repository's root**, which CLAUDE.md § Hard stops requires (rank 1) where
`SKILL.md:133-134` asks a copy outside it, as panel 247's ninety-six sessions ran. Walking up from
`s/972740` to `/`, what stands above every session folder (the critic's item 15, re-run): the
repository's **`CLAUDE.md`** (`/Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md`, the only one; no
`CLAUDE.local.md`, no `~/.claude/CLAUDE.md`), the repository's **`.claude/`** with
**`.claude/settings.json`**, **`.claude/settings.local.json`** and **`.claude/hooks/`** (`ls -d`), and the
user's `~/.claude/`. `--safe-mode` disables `CLAUDE.md`, skills and hooks and `--restricted` ignores the
user, project and local settings (`SKILL.md:171-175`); **whether they do so for this CLI is unrun**,
since asking would be a session. **So each `context` answer is read for those by name**: any trace of
`CLAUDE.md` or its rules, of the settings (a permission, a language, an added directory) or of a hook's
output (a refusal, an injected line) reaching the session; a `yes` naming any of them voids that session
until it is re-run, and the path alone (above) is recorded and does not void it. Every folder is
git-ignored (`git check-ignore -v .claude/worktrees/p248-scratch/s`: `.gitignore:34:.claude/worktrees/`).

**The map, the coordinator's alone**, written by `tools/sessions.py` (`cae1b8bcdcdb9510`) to
`248-llm-ergonomist/s-map.txt` and the order to `s-order.txt`, every folder's spec and brief hash
asserted (each `shasum -a 256`, first 16). **The folders were made again on the second pass**, after
arm d's card changed: the first twelve, in which nothing had run (`find s -name 'run.json*'`: 0; 36
files, every one `brief.md`, `task.md` or `heroes-spec.md`), were emptied and `sessions.py` run again,
which draws new names; the first map and order stand beside the new as `*.before-second-pass.txt`.
Each new folder re-checked: `ls -A` printed `brief.md heroes-spec.md task.md` twelve times, `find s -type
f | wc -l` 36, each `heroes-spec.md` `cmp`-equal to `git show df7612ad3:spec/heroes-spec.md` and each
`brief.md` to `.claude/worktrees/p247-scratch/s/027658/brief.md`, and each `task.md`'s hash the table's:

| order | session | folder under `s/` | `heroes-spec.md` | `brief.md` | `task.md` |
|---|---|---|---|---|---|
| 1 | t4-x-1 | `972740` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 2 | t4-q-1 | `a77262` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 3 | t5-x-1 | `312cf1` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 4 | t5-d-1 | `7dbbca` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 5 | t5-d-2 | `2266da` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |
| 6 | t5-x-2 | `dfbae4` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 7 | t4-q-2 | `e03cf5` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 8 | t4-x-2 | `a6d5db` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 9 | t4-x-3 | `1b8002` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `c831407013e92305` |
| 10 | t4-q-3 | `96cd98` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `5e2de4fe299c71ef` |
| 11 | t5-x-3 | `e5fc7b` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `f17b4264a32a2fd8` |
| 12 | t5-d-3 | `acd173` | `163219f2fa69fe82` | `1d0b6a53cddcb0a0` | `01cd81a9a476ec53` |

`163219f2fa69fe82` is the base's `spec/heroes-spec.md` (`spec-warden.md`); `1d0b6a53cddcb0a0` is panel
247's seat brief byte for byte. Only arm d's `task.md` hash moved (`ee131c6a56d90238` before); x's and
q's are as they were. `grep -c` over the new folders: `set_deadline` 2 lines in d's `task.md` and 0 in
x's, `ask_stop` 2 in q's and 0 in x's, `SetDeadline` 0 in every one. **The order**: three rounds over
the four cells, round 1 `t4-x t4-q t5-x t5-d` with replicate 1, round 2 the same cells reversed with
replicate 2, round 3 forward with replicate 3, so a stop leaves every cell represented.

**The command**, panel 247's (`docs/panel/247-briefs/llm-ergonomist.md:116-122`), with one change, the
file it writes while it runs, run from each session's folder by `tools/blind-one.sh`
(`9fa8b055a0077e3a`, `bash -n`: parses), the environment holding only these:

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

**The runner, repaired** (the critic's second pass, item 1). The first `blind-one.sh` wrote `> run.json`
from the moment a session started and stopped on any `run.json` it could not read, so every session in
flight was an unreadable file: the critic's dry run with stubs of 2 to 8 s printed four `START`s, then
eight *"STOP before <hex>: 3 unreadable run.json"* at the first `END`. Panel 247's runner had no such
line (`p247-scratch/tools/blind-one.sh:19-21`). **Now a session writes `run.json.part` and renames it
`run.json` when the command returns**, so a `run.json` is an ended session's alone: the sum counts ended
sessions, an ended `run.json` with no readable cost stops the runner (*"STOP before <hex>: N ended
run.json without a readable cost"*), a folder whose `run.json` stands is skipped, and a `.part` a killed
run left is overwritten. `S` and `BOUND` may be set from `BLIND_S` and `BLIND_BOUND`, for the simulation
alone.

**Proved by a stub simulation, never a real session**
(`.claude/worktrees/p248-scratch/briefs/fix/runner-sim/`): `bin/claude`, a local stub that logs its call,
sleeps 2 to 8 s (`$((2 + RANDOM % 7))`) and prints a `run.json` with the cost in `cost.txt` (or output that
is not JSON for a folder named in `bad.txt`); the PATH given to the runner
`runner-sim/bin:/opt/homebrew/opt/python/bin:/usr/bin:/bin`, which holds no other `claude` (`env -i
PATH=... bash -c 'command -v claude'` printed the stub's path; the real one is
`/opt/homebrew/bin/claude`, not on it), the key empty; each run over a fresh copy of the twelve folders
by `xargs -n 1 -P 4 bash tools/blind-one.sh < s-order.txt`, as `blind-run.sh` runs it:

- **`sim-A.log`, 0.20 USD a session, the bound 3.00**: twelve `START`s, four in flight at a time, each
  next one starting as one ended (*"END 972740 10:22:54 ... START 2266da 10:22:54 sum of ended sessions
  0.200000"*), twelve `END`s at exit 0, the last at 10:23:06; *"stub calls: 12, run.json: 12, .part left:
  0"*.
- **`sim-B.log`, 0.40 USD a session, the bound 3.00 (the stop rule)**: ten `START`s, every one at a sum
  of ended sessions of at most 2.40 (*"START 96cd98 10:23:13 sum of ended sessions 2.400000"*), then
  *"STOP before e5fc7b: sum 3.200000 above 3.00"* and *"STOP before acd173: sum 3.200000 above 3.00"*,
  the four in flight ending after; *"stub calls: 10, run.json: 10, .part left: 0"*, so 4.00 in all, under
  3.00 plus four sessions' 0.40.
- **`sim-C.log`, an ended session without a readable cost**: the stub's non-JSON for `312cf1`, *"END
  312cf1 10:23:38 exit 1 cost unreadable (JSONDecodeError)"*, then six *"STOP before <hex>: 1 ended
  run.json without a readable cost"*, the two in flight ending after; *"stub calls: 6, run.json: 6, .part
  left: 0"*.

`pgrep -f runner-sim` found nothing after.

**The CLI**: `claude --version` here prints `2.1.311 (Claude Code)`; `SKILL.md:185-186` records that
2.1.274 refused `claude-opus-5-5`, and whether 2.1.311 accepts it is unrun (asking would be a session).
So **the first session runs alone** (`bash tools/blind-one.sh 972740`, its `END` line read: a refused
model shows as a `run.json` with no report and an error in `run.err`, or as the unreadable cost that stops
the runner), and only then `tools/blind-run.sh` (`19dc1235a03fb6ad`, `bash -n`: parses), which skips a
folder whose `run.json` stands and runs the rest four at a time in `s-order.txt`'s order.

**The key**: loaded in the runner's own shell by `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env;
set +a`, its length alone printed (`${#ANTHROPIC_API_KEY}`), never its value, passed to each session
through `env -i` as above. The coordinator loaded no key for these briefs.

**The bound, proposed: a ceiling of 5 USD for the seat.** Each session is capped at **0.40 USD**
(`--max-budget-usd`), so the twelve are bounded at 4.80 USD by their caps alone (`python3 -I -c
'print(12*0.40)'`). But **the CLI can pass its cap by one request**: panel 247's t4-m2-3 ended at 0.4960
USD on a 0.40 cap (`docs/panel/247-*.md:423-424`). So the runner also sums `total_cost_usd` over every
ended `run.json` under `s/` and starts a session only while that sum is at most **3.00 USD** (`BOUND`
in `blind-one.sh`); with four in flight the total is at most 3.00 plus four sessions' costs: **4.984
USD** if no session costs more than 0.496 (`python3 -I -c 'print(3.00 + 4*0.496)'`), the largest of panel
247's ninety-six, **an assumption, not a bound** (the critic's second pass, item 10). Panel 247's
ninety-six `run.json` (`.claude/worktrees/p247-scratch/s/*/run.json`, summed by `python3 -I`): 21.0131
USD, each from 0.1465 to 0.4960, a mean of 0.2189, so twelve at that mean would cost about 2.63 USD
(`python3 -I -c 'print(12*0.2189)'`): an inference about these, not a measurement. **The ceiling is the
author's to give or change**; a lower one lowers `BOUND` in the same proportion before any session.

**The reports**: take `result` from `run.json` where `report.md` is missing, and say which. The
coordinator copies them into `docs/panel/248-reports/llm-ergonomist.md` under a header saying how they
were run, each session's cost and every `context` answer, read as the paragraph above says.

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
- **The decision rule, proposed**: a face is measured to help if its arm gives at least **two** more
  `right` answers than x over the **three** sessions of its task, both graded under the setup that has
  the door. **Its reason**: with three sessions a cell, a difference of one is what one changed answer
  makes, so the face counts only on a difference one answer cannot make. The coordinator may change the
  rule before the first session, never after.

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

- Whether `claude` 2.1.311 accepts `--model claude-opus-5-5` (the first session, run alone, says).
- Whether three sessions a cell can separate a face from x at all, where panel 247 ran four to twelve a
  cell: a wider design is the author's, at a wider ceiling.
- Whether the spellings above survive the seats' builds (above: the folders are re-made if not).
