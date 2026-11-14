# Panel 248, the spec-warden's report

Started 2026-11-14 10:20 CET. Written as I go; a section marked *pending* is not yet measured.

## Where the brief is wrong, first

- **Errata applied** (the critic's second pass, `critic-briefs.md` § Second pass, items 3 to 7 and 13):
  q5d, q5j and q3a are spelled by my brief alone (`hero_net_set_deadline`, `hero_thread_ready`,
  `hero_run_dir` occur in no other brief: `grep -c` over `248-briefs/*.md`, run 10:24, 0 in every
  file but `spec-warden*.md`), so I price them as **candidate spellings, not routes**; q5j is not the
  join 00-shared.md describes, so I price both shapes; my brief's spec grep printed nothing (item 5);
  `real` is the pin `SPEC_REAL_TOKENS 11285` of 2026-11-13, not a count made today (item 6).

## The ceiling, read today

`grep -n '^### 1.6' docs/design.md` printed 253 at the base `df7612ad3`; its first sentence: *"must fit
in 16384 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*. The payment
rule (§1.6, its paragraph from line 320): *"Every amendment owes a named removal or a registered
prediction naming an instrument that exists today ... at every level, with no threshold to be under."*

## The counts, measured in my own tree

My tree: `git archive df7612ad3` into `.claude/worktrees/p248-scratch/248-spec-warden/tree`, the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0) and stage 2 (`./heroes build
selfhost/main.hero -o heroes-next`, exit 0), `./heroes-next --version`: `heroes 0.6.0`, **`sources
4c9a6a9800095eef`**, the base. `shasum -a 256 spec/heroes-spec.md` `163219f2fa69fe82...`, `wc -l -c` 492
and 29,187. Load average 287 at 10:21 (`uptime`), so no duration here is a measurement.

**The base**, `./heroes-next measure spec/heroes-spec.md`, exit 0: claude-legacy **8470**, cl100k_base
**8621**, maximum 8621, spread 151, real **11285** (the pin of 2026-11-13, no `STALE`), *"Headroom: 5099
against the 16384 ceiling ... the FFI floor mortgages 60 of it (panel 030 R3), so what is measured
against the ceiling is 11345"*. Byte for byte the brief's.

**The drafts**, rebuilt by `248-spec-warden/drafts.py` (`python3 -I drafts.py tree/spec/heroes-spec.md
price`): each anchor asserted to occur exactly once (`str.count` printed 1 for each), each draft one
line changed against the base (`diff | grep -c '^[<>]'` 2 for each). Each measured `./heroes-next
measure ../price/<d>.md`, exit 0, **no `--refresh`**; the files and outputs are in `price/`.

| draft | shape | legacy | cl100k | max | vendored delta | bytes |
|---|---|---|---|---|---|---|
| x | the base | 8470 | 8621 | 8621 | 0 | 0 |
| q4m | 404 merged: *"Ctrl-C, SIGTERM and `hero_net_ask_stop() -> i64` only make ..."* | 8484 | 8635 | 8635 | **+14** | +30 |
| q4t | q4m with *"on any thread"* (mine: q4a's thread clause, q4m's place) | 8487 | 8638 | 8638 | **+17** | +44 |
| q4a | 404 appended: *"; `hero_net_ask_stop() -> i64` asks the same stop from any thread"* | 8491 | 8642 | 8642 | **+21** | +65 |
| q5j | 377 merged: `hero_thread_ready` beside a join that still waits | 8493 | 8644 | 8644 | **+23** | +77 |
| q5n | 377 merged: **join itself answering "not yet"** (the critic's item 4), *"gives its answer, or at once `HERO_THREAD_RUNNING` while it runs"* | 8484 | 8636 | 8636 | **+15** | +44 |
| q5o | 377 merged: the same with no sentinel, *"`hero_thread_join(handle: i64, @answer: i64) -> i64` puts its answer in `answer` and gives 1, or gives 0 at once while it runs"* | 8491 | 8644 | 8644 | **+23** | +67 |
| q3a | 377 appended: `hero_run_dir(path: str)` | 8497 | 8648 | 8648 | **+27** | +93 |
| q5d | 377 appended: `hero_net_set_deadline(s, ms: i64) -> i64` | 8512 | 8663 | 8663 | **+42** | +122 |

**Every vendored delta is a lower bound, and every verdict below on a draft is provisional**, in those
words: the real count is the pin's alone, and each draft's real count is one `heroes measure --refresh`,
an API call this project counts as a paid run, the author's to allow. **For scale only, an inference
and never a measurement of these drafts**: panel 247's landing moved vendored +128 and the real pin
+177 (11108 to 11285), a ratio of 1.38; by it every draft here is under +60 real, and all six of the
brief's and mine together (+14 +21 +42 +23 +27 vendored, q4m and q4a counted both) under +180 real,
against 5039 real free after the FFI floor. **No draft, nor all of them, comes near the ceiling: I cast
no budget veto in this sitting.** `DELTA_GATE` is 50 vendored per commit (`suite_spec.hero:217-218`):
q5d alone is +42, and any two drafts landed in one commit with q5d pass it, so such a commit says in
its body what paid for it.

## What a reader of today's spec and card writes (my brief's question 4), run on Darwin arm64

Every program in my tree (`examples/sw-*`, so `heroes.toml`'s relative index resolves) or in
`248-spec-warden/probes/`, built by my stage 2, every binary `sw-`. Bounds as 00-shared.md's table:
servers under `timeout -s KILL 60`, clients `curl --max-time 5` under `timeout -s KILL 90`, probes
under `timeout -s KILL 10` to `60`; the bytes by **`dd bs=1 count=65536`**, because BSD `head -c` held
the server's `listening` line in stdio's buffer until exit, so my first run (`runs/quit-1`) found no
port, sent nothing, and the bound killed the server at 60 s (exit 137): **my probe's fault, discarded,
and said here**. After every run `pgrep -fl <binary>` printed nothing left. Scripts: `q4run.sh`,
`q5run.sh`; outputs under `runs/`.

**Question 4, a server that ends on `GET /quit` with what exists today** (`examples/sw-quit`, the
example server with `answer` sending its own process SIGTERM through C bindings of `kill` and `getpid`,
`signal.h` declaring `int kill(pid_t, int)` and `pid_t` `__int32_t` in this SDK, so `pid: i32`):

- **First build refused**: `error[discarded_value]: this line's value has type i32 and nothing receives
  it`, fix `certain`; one correction round, which the fix's `_ = ...` answers.
- **Run** (`q4run.sh sw-quit quit-2 /hello /quit /hello`): `GET /hello` 200, `GET /quit` 200 `bye`,
  the third request found no server (curl exit 7); the server printed *"stopped: requests answered 2,
  connections 2, failed 0"*, **exit 0, stderr empty**.
- **The leak check speaks in such a build**, so the empty stderr is a clean check: the control
  `examples/sw-leak` (a connection given up by `net.number` and never taken back) printed *"panic: 1 C
  handle(s) never given back ... a socket given up as a number no hero_net_of took back"*, exit 134.
- **So on Darwin the route exists without the door**, at one correction round; it is POSIX's `kill`,
  so what it is on Windows is a question, below.

**What line 404 says of a second signal** (`probes/sw_twice.hero`: `hero_net_catch_stop`, then
`kill(getpid(), SIGTERM)` twice): *"caught 0, asked 0"*, *"after one SIGTERM: asked 1"*, then **exit
143**, the line after the second `kill` never printed. The handler's code says why
(`runtime/platform/hero_net.c:556-577`: *"any other, a second one ..., restores the signal's own end
and sends the signal again"*).

**Question 5, a server and a connection that sends nothing, with what exists today**:

- **The example server as written** (`sw-server`, `q5run.sh sw-server quiet-1 258`): one connection
  opened first that sends nothing, then `GET /hello` one after another: **255 answered 200, the 256th
  got nothing** (curl timed out, http 000) and so did the next; the server sat in `joined` on its
  oldest thread with no message until my SIGTERM at 10:30:05, then *"stopped: requests answered 255,
  connections 256, failed 0"*, exit 0, stderr empty. **The critic's reading (its item 21) holds as
  run: one quiet connection first stops the accept after 255 more, in silence.**
- **The plausible repair a reader writes with today's card** (`examples/sw-watch`: the connection's
  thread takes its number back and reads; a second thread given the same number sleeps 1 s and takes
  it back to close it): *"watchdog: taking back the connection numbered 4611686018427518979: the socket
  is closed, or none the layer holds"*, the watchdog answering 1; the read ended only when its peer
  closed; exit 0. **Refused and told, never silent**, but the card offers no way to end a quiet
  connection's wait from outside its thread: the peer closing or the whole stop are the only ends.

## The real counts, measured (the coordinator's direction of 10:30)

**The coordinator's message of 10:30 relayed that the author approved this sitting's paid runs, ceiling
25 USD in all, and directed me to take the real count of the base and of each draft I price.** I did
not see the author's approval myself; I made the calls on that direction, and say so here. Each from
my tree's root with my stage 2, the key loaded in the same command as `set -a; .
/Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, never printed (`${#ANTHROPIC_API_KEY}` 108).
`--refresh` refuses every path but `spec/heroes-spec.md` (`selfhost/measure/pinned.hero:106-111`), so
each draft was copied to that path in my own tree, refreshed, and the base restored after the loop
(`shasum -a 256`: `163219f2fa69fe82` again). Command, each: `./heroes-next measure spec/heroes-spec.md
--refresh`, exit 0, stderr only the paste note (`grep -v -c '^note: paste these'` 0 for all nine).
Request bodies and answers kept in `248-spec-warden/price/<d>.refresh.d/`.

**Calls made: 9 invocations of `--refresh`** (the base and eight drafts), **27 requests** to
`count_tokens` (each invocation two probes and the document, `selfhost/cli/refresh.hero:152-221`; `ls
price/*.refresh.d/*-answer.json | wc -l` 27). Checked by hand on two: the probes answered 7 (`x`) and
11 (`x x x x x`), so the wrapper is 6; the base's document 11291 - 6 = 11285, q4m's 11309 - 6 = 11303.

| draft | real | **real delta** | vendored delta | digest |
|---|---|---|---|---|
| x (base) | **11285**, taken 2026-11-14 | 0 (the pin of 2026-11-13 again) | 0 | `0ecc0ee44a3ffefe` |
| q4m | 11303 | **+18** | +14 | `737f58894bd8649b` |
| q4t | 11307 | **+22** | +17 | `1983d4f05a57b3c6` |
| q5n | 11308 | **+23** | +15 | `7cb2808159386daf` |
| q4a | 11313 | **+28** | +21 | `59d1f5025d388faf` |
| q5o | 11314 | **+29** | +23 | `17bdfeea339f5606` |
| q5j | 11315 | **+30** | +23 | `14aa9be040453c52` |
| q3a | 11318 | **+33** | +27 | `3a7ff8dc3ad2ca48` |
| q5d | 11341 | **+56** | +42 | `64caed32626f59b7` |

**My verdicts below rest on these real counts and are not provisional on the budget.** The largest,
q5d, leaves 16384 - 11341 - 60 = 4983 free after the FFI floor; the brief's five and my three together
would be under +240 real (the sum of the deltas, each measured alone, 238: an inference, since
sentences in one line need not add exactly). **No budget veto is possible in this sitting.** Every
verdict that follows is on Principle 0 (design.md §1.0) and on the payment rule (§1.6), which do not
relax with headroom.

**The same five programs in Linux arm64** (`248-spec-warden/linux/run.sh`, one container `sw-248-1`,
the base archived in on stdin, the image's `python3` as client since it has no `curl`; `docker exit 0`,
`docker ps` after it listed nothing; log `linux/linux.log`): seed and stage 2 exit 0, **`sources
4c9a6a9800095eef`**; `sw-twice` *"after one SIGTERM: asked 1"* then **exit 143**; `sw-leak` the same
panic, exit 134; `sw-watch` the same refusal, *"the socket is closed, or none the layer holds"*, exit 0;
`sw-quit` `GET /quit` `200 OK`, then *"stopped: requests answered 2, connections 2, failed 0"*, **exit 0,
stderr empty**; `sw-server` with one quiet connection first, **255 answered, the 256th
`TimeoutError`**, exit 0 on SIGTERM. **Every result is Darwin's.**

**Windows, owed** (the box is off): on MinGW, which zig stands for here and never MSVC,
`zig cc -target x86_64-windows-gnu -c` of `kill(getpid(), SIGTERM)` exited 1, *"call to undeclared
function 'kill'"* (`zig/killself.c`); `raise(SIGTERM)` compiled, exit 0. Whether `raise` reaches the
layer's Windows stop is a reading that says no (the stop is a console handler,
`runtime/platform/hero_net.c:513-545`, not a C `SIGTERM` handler), unrun. **So the route that exists
today is POSIX's**: on Windows a program has, by reading, no way to ask its own stop through any door
of the base.

**q4s, priced after the reading below showed it needed** (`price/q4s.md`: 404's stop clause becomes
*"the first of Ctrl-C, SIGTERM and `hero_net_ask_stop() -> i64` only makes `hero_net_stop_asked() ->
i64` answer 1 and a waiting call of that layer answer stopped, and a signal after it ends the
program"*): vendored 8647 (**+26**), `--refresh` exit 0, **real 11318 (+33)**, digest
`7e6dd2874d3c53d5`, base restored (`163219f2fa69fe82`). **Calls made in all: 10 invocations, 30
requests.**

## The readings each verdict rests on

**Line 404 and a door that asks the stop (my brief's question 1).** *"Ctrl-C and SIGTERM only make
`hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped"* has two
readings, and both were run against the base:

- **(A) what the two signals do, and nothing more.** Already incomplete at the base: a second SIGTERM
  **ends the program, exit 143**, on Darwin and Linux arm64 (`sw-twice`). Panel 247 never discussed it
  (`grep -n -i -E 'only make|second (signal|one)'` over `247-*.md` and its reports: the sentence quoted
  twice, nothing on its reading).
- **(B) the whole list of what makes it answer 1.** **True at the base**: the counter is static and
  written in two places only, the Windows console handler (`hero_net.c:524`) and the POSIX handler
  (`:563`), `grep -n hero_net_stop_asks` over the runtime.

**So with the door and no sentence (q4z), line 404 is true under neither reading**: B is falsified by
the door, and A was never whole. The card would then say what the spec denies, and CLAUDE.md § 12 puts
the spec first (*"Spec beats compiler: the compiler has the bug"*): by the project's own precedence the
door would land as a defect. **A sentence is owed the day the door lands, and only then.**

**And the door's design decides which sentence is true.** If `hero_net_ask_stop` adds to the same
counter, a person's **first** Ctrl-C after the program asked takes the handler's other branch
(`hero_net.c:563-574`: *"any other ... sends the signal again"*) and ends the process mid-drain; q4m,
read as (A), is then false for the first signal anyone sends, and the true sentence is q4s. If the door
keeps the program's ask apart, so a later signal still counts as the first, q4m is true as written
under (B) and no worse than the base under (A). That is the compiler-engineer's choice (00-shared.md
§ Question 4, *"whether a Ctrl-C after the program asked its stop ends the process at once ... or counts
as the first"*); the spec follows it, never the reverse.

**Line 377 and the three doors beside it (my brief's question 2).** Line 377 says where threads,
processes, sockets and clocks come from, and names `hero_thread_spawn` and `hero_thread_join` because
they carry a rule a reader cannot see elsewhere (one `i64` each way, *"nothing else crosses"*). Panel
242's R6 admitted `hero_run_env` at 0 tokens on *"processes ... from the language's `hero_os.h`"*
(`242-*.md:176-180`); `hero_thread_limit`, which the example server needs, stands in no spec sentence
either. **A deadline in `hero_net.h`, a child's folder in `hero_os.h` and a ready-check beside a join
that still waits each leave line 377 and line 404 true** (a deadline makes a wait answer timed out, not
stopped), so each is R6's case: the door at 0 tokens, its promise in its header and its card.
**The join that answers "not yet" is not**: it makes *"waits for its answer"* false, and it takes away
what the compiler needs (below).

**`ffi_defined_elsewhere` widened (my brief's question 3).** The spec names signals at lines 403-404
alone (`grep -n -i signal`: 2 lines), and no sentence names what the member refuses (the critic's item
5: `grep -c` 0). A library that defines `sigset` is *"beyond this document"* (line 377's first words),
and route I makes no sentence false: it enforces 403-404's rule on more paths. **So it is a diagnostic's
business** (design.md §4.17, `grep -n '^### 4.17'` 2343: *"Every error carries all the context needed to
fix it"*): 0 spec tokens.

**Question 2's promise** (*"within about 50 ms"*) stands in `hero_net.h` twice and in no spec sentence
(`grep -n -i -E '50 ms|millisecond|within about'` over the spec printed nothing): 0 tokens.

**What the compiler needs** (design.md §1.0's closure list, by `grep -rl --include='*.hero'` over
`selfhost/`): `hero_net_ask_stop`, `hero_net_stop_asked`, `hero_net_catch_stop` 0 files each; `chdir` 0;
`hero_thread_join` **5 files, 4 calls**, and `selfhost/main.hero:112` is *`hero_exit(code:
hero_thread_join(handle: handle))`*: the compiler runs every command on a thread and exits with the
waiting join's answer; `cli/link.hero:353`, `mutate/recovery.hero:163` and `mutate/cut.hero:298` sum
workers' answers.

## Verdicts

Every count below is the real one, measured in this sitting (§ The real counts). The ceiling is design.md
§1.6's 16384, read today; none is near it.

### q4m, line 404 merged (question 4)

- `verdict`: **approve**, conditional on the door landing and on its design (`condition`)
- `section`: design.md §1.6 (payment), §1.0; CLAUDE.md § 12 (spec beats compiler)
- `spec_token_delta`: real **11285 → 11303 (+18)**; vendored 8621 → 8635 (+14)
- `removal`: nothing, and that is a problem I answer with a registered prediction (below), as panels 246
  and 247 paid theirs (ledger rows 245-246: *"Paid by the measured effect Principle 0 asks for, the
  blind seat's registered rules"*)
- `needed_for_self_hosting`: no
- `argument`: With the door and no sentence, line 404 is true under no reading: the whole-list reading
  (true today, the counter written by the two handlers alone) is falsified, and the what-they-do reading
  was never whole (second SIGTERM, exit 143, both platforms). q4m restores the reading that is true
  today at the smallest price of the three, keeps the door beside the signals it joins (§1.3's
  locality), and adds no rule a reader must hold apart. The sentence buys truth, not a thesis effect;
  its payment is the prediction.
- `prediction`: registered here, before any session: **a spec arm q4m for t4** (3 sessions, card q,
  the blind seat's runner, its t4 task unchanged) against the blind seat's own t4-q (spec x, card q).
  Under spec x, **at least 1 of 3** answers ends the server by sending itself a signal (`kill`,
  `getpid`, `raise`) instead of calling `ask_stop`, or names line 404's *"only"* against the card in
  its `choice_points`; under q4m, **0 of 3**. Instrument: `248-llm-ergonomist/`'s runner and its
  grading, which exist today; 3 sessions at the brief's 0.40 USD cap, at most 1.20 USD; scored at this
  sitting's synthesis if run, else at M-core-packages' close before the door lands. Falsified if x
  reads 0 of 3 or q4m reads 1 or more. **It needs the runner repaired first**: the critic's second pass,
  item 1, measured `blind-one.sh` stopping every other session after the first one ends. And: landed
  alone on this base, `--refresh` reads **11303**.
- `condition`: if the door adds to the stop's counter so a later signal ends the process, q4m is false
  for the first Ctrl-C a person sends, and I approve **q4s** (+33) instead and object to q4m; if the
  door does not land, nothing enters.

### q4s, the second-signal rule written (question 4)

- `verdict`: **approve, only under the shared-counter design**; otherwise **object** (it would state a
  rule the door does not keep)
- `section`: design.md §1.6, §1.4; CLAUDE.md § 12
- `spec_token_delta`: real **11285 → 11318 (+33)**; vendored +26
- `removal`: nothing; the same registration as q4m pays, its arm then spelled q4s
- `needed_for_self_hosting`: no
- `argument`: Under a door that shares the counter, *"Ctrl-C ... only make ... answer 1"* is false for
  the first signal after the program asked, and a server draining on its own `ask_stop` is killed by one
  Ctrl-C with no sentence anywhere telling its writer. q4s is the one draft true under both readings for
  that design, and it also closes the base's own gap (the second signal, exit 143 measured). Fifteen
  tokens more than q4m buy a promise the program cannot otherwise learn from the spec.
- `prediction`: landed alone on this base, `--refresh` reads **11318**; the registered arm as q4m's.
- `condition`: the compiler-engineer's door keeps the program's ask apart from the signals' count:
  then q4m, and this one is refused.

### q4t and q4a, the thread clause (question 4)

- `verdict`: **object** to both
- `section`: design.md §1.2, §1.3
- `spec_token_delta`: q4t real **+22**, q4a real **+28**
- `removal`: nothing
- `needed_for_self_hosting`: no
- `argument`: q4a appends the door after `hero_signal_take`'s clause, far from the stop it asks (§1.3),
  and leaves *"Ctrl-C and SIGTERM only make ..."* standing first: under the whole-list reading the line
  then contradicts itself within one sentence. q4t's *"on any thread"* binds ambiguously (to the signals
  as easily as to the door). No sentence of the spec says which thread may call a C door (`grep -n -i
  thread`: line 377 alone, which says what crosses), the card already says *"from any thread"*, and no
  measurement says a reader of the spec needs it.
- `prediction`: in the blind seat's t4-q sessions (card with *"from any thread"*, spec x), **3 of 3**
  call the door from the handler's own thread, not from `main`.
- `condition`: one t4 answer under card q that routes the request back to `main` to call the door
  there (a thread-safety doubt) would make the thread clause a measured need: then q4t's words, placed
  after the door.

### q4z, no sentence (question 4)

- `verdict`: **object** if the door lands; **approve** if it does not
- `section`: CLAUDE.md § 12; design.md §1.6
- `spec_token_delta`: 0
- `removal`: n/a
- `needed_for_self_hosting`: no
- `argument`: Measured above: the door leaves line 404 true under no reading. A spec that a landed door
  makes false is the shape § 12 settles against the door. Panel 246's R4 kept the stop's documentation
  at 0 tokens by the spec-warden's veto, Principle 0 unmet (`246-*.md:351-355`); panel 247's R5 has
  since named the stop's doors in line 404, so a door that changes what line 404 says is no longer
  outside the spec, and 246's precedent does not reach it.
- `prediction`: with the door landed and q4z, the blind seat's t4-q answers record the conflict
  (*"only"* against the card) in at least 1 of 3 `choice_points`.
- `condition`: the door not landing, or a rewording of line 404 that drops *"only"* at no larger price.

### q5j, `hero_thread_ready` beside a join that waits (question 5, the brief's spelling)

- `verdict`: **veto** on the sentence (Principle 0's burden unmet); **approve** the door at 0 spec tokens
- `section`: design.md §1.0; panel 242's R6 (`242-*.md:176-180`)
- `spec_token_delta`: real **+30** for the sentence; 0 for the door
- `removal`: nothing
- `needed_for_self_hosting`: no
- `argument`: The need is measured and it is the door's: one quiet connection first stopped the example
  server after 255 requests, in silence, on Darwin and Linux arm64. But line 377 stays true with a
  ready-check beside a waiting join, exactly R6's case; the sentence carries no rule a reader cannot
  read in `hero_os.h`, as `hero_thread_limit` carries none, and nothing measures what the sentence buys.
- `prediction`: with the door landed at 0 tokens and the example's `served` joining whichever thread is
  ready, `q5run.sh` answers **258 of 258** on Darwin and Linux arm64 (255 today).
- `condition`: a spec arm (spec q5j, the door on no card) measured against spec x on a server-shaped
  task, with a registered rule, giving the arm at least two more right answers.

### q5n, the join itself answering "not yet" (question 5, 00-shared.md's shape, the critic's item 4)

- `verdict`: **veto** (Principle 0: it takes away what the compiler needs, and it disserves the thesis)
- `section`: design.md §1.0, §1.4, §1.2
- `spec_token_delta`: real **+23**
- `removal`: nothing
- `needed_for_self_hosting`: **no, and worse**: the compiler needs the waiting join (4 calls,
  `selfhost/main.hero:112` exits with its answer)
- `argument`: Same signature, new meaning: every existing call, the compiler's four and the example's,
  still compiles and silently reads a sentinel for an answer, §1.4's different-but-valid program. And
  any sentinel is an `i64` a thread may legitimately answer (line 377: it *"gives back one"*), so a
  finished thread can be read as running. This is the thesis inverted: a plausible mistake made silent.
- `prediction`: a runtime whose join answers at once with a sentinel builds the compiler at exit 0, and
  that compiler exits with the sentinel before running any command. Unrun: a heavy build I did not make.
- `condition`: none for this shape; a non-waiting join enters only beside the waiting one (q5j's case).

### q5o, the same without a sentinel, `@answer` (question 5, my variant of the falsifying shape)

- `verdict`: **veto** as a replacement of the waiting join (the compiler's need); as a door beside it,
  0 spec tokens by R6, like q5j
- `section`: design.md §1.0; panel 242's R6
- `spec_token_delta`: real **+29**
- `removal`: nothing
- `needed_for_self_hosting`: no; the compiler needs the waiting form it would replace
- `argument`: It fixes q5n's silence (old calls refused at the build, the right direction) but removes
  the wait the compiler's `main` exits through, which it would then have to poll for. A language keeping
  a waiting join keeps line 377 true, so the non-waiting form is an addition, never this sentence.
- `prediction`: under q5o the compiler's four calls are refused at the build (one argument short), the
  right direction, and each then needs a loop to wait; unrun, a heavy build I did not make.
- `condition`: none as a replacement.

### q5d, `hero_net_set_deadline` (question 5)

- `verdict`: **veto** on the sentence (Principle 0's burden unmet); **object** to its words
- `section`: design.md §1.0, §1.2; panel 246's R4 (`246-*.md:351-355`, the stop's documentation at 0
  spec tokens)
- `spec_token_delta`: real **+56**, the largest of the sitting
- `removal`: nothing
- `needed_for_self_hosting`: no
- `argument`: Line 377 already names `hero_net.h` for sockets and line 404 stays true (a deadline answers
  timed out, not stopped), so the door is R6's case. The blind seat's t5 measures the card's face, never
  this sentence. Its words are unsafe too: `s` has no type, unlike every signature in § 13, and *"last
  gave"* does not say whether the limit is absolute or per wait, while arm d's card names Go's absolute
  `SetDeadline` for a per-wait count (the critic's second pass, item 2): a third description beside two
  that disagree raises the rewrite rate.
- `prediction`: in t5, arm x writes **0 of 3** programs that close a quiet connection (no door exists:
  the watchdog shape is refused, measured).
- `condition`: a spec arm measured against x with a registered rule, after the card's Go reference is
  made true.

### q3a, `hero_run_dir` (question 3)

- `verdict`: **veto** on the sentence (Principle 0's burden unmet); **approve** the door at 0 spec tokens
- `section`: design.md §1.0; panel 242's R6 (`242-*.md:176-180`)
- `spec_token_delta`: real **+33**
- `removal`: nothing
- `needed_for_self_hosting`: no (`chdir` in 0 files of `selfhost/`; the four cases are the net's, and the
  net is not the closure list)
- `argument`: `hero_run_env` entered at 0 tokens on line 377's *"processes ... from the language's
  `hero_os.h`"*, and a child's folder is the same kind of door. The draft's words also mislead: *"else
  in the program's"* reads as the program's own folder, where the default is the parent's current
  folder; and on Windows the program itself is looked for in the parent's folder whatever
  `lpCurrentDirectory` says (00-shared.md's `CreateProcessW` page), a promise for the header.
- `prediction`: none owed; at 0 tokens the spec's count does not move (`--refresh` 11285 after the door).
- `condition`: a program a reader writes from the spec alone that needs to know a child's folder and
  goes wrong without a sentence, measured.

### Question 1 (`ffi_defined_elsewhere` widened) and question 2 (2502)

- `verdict`: **approve** no sentence for either
- `section`: design.md §4.17; §1.6
- `spec_token_delta`: 0
- `removal`: n/a
- `needed_for_self_hosting`: no
- `argument`: No spec sentence names the member or the 50 ms promise, and route I or any wake route
  makes no spec sentence false. A program refused because its library defines an installer the runtime
  defines is a C library's matter, beyond the spec by line 377; what it owes is §4.17's message: the
  name, the library, the runtime's definition, the fix.
- `prediction`: the landing's refusal names all four in one diagnostic, read from its golden.
- `condition`: the widening refusing a program that binds only headers with no library of its own: then
  the spec owes one sentence, priced at the landing.

## The order of preference, for the synthesis

1. **Questions 1, 2 and 3, and question 5's doors: no sentence**, 0 tokens (R6's precedent, line 377 and
   line 404 left true).
2. **Question 4: a sentence only if the door lands, and which one follows the door's design**: q4m (+18
   real) if the program's ask is kept apart from the signals' count; q4s (+33) if the door shares it.
   Paid by the registered spec arm above.
3. **Refused**: q4a, q4t (object); q5j's, q5d's and q3a's sentences (veto, Principle 0); q5n and q5o as
   replacements of the waiting join (veto, the compiler's need).

## Owed, and not run

- **Windows**: whether `raise(SIGTERM)` reaches the layer's stop, and every program above, owed to the
  box or the CI's leg (the box is off; MinGW's missing `kill` is zig's, never MSVC's).
- **q5n's prediction** (a compiler built on a join that answers at once exits with the sentinel): a
  stage 2 build on a changed runtime, not made.
- **The q4m (or q4s) spec arm**: three sessions, at most 1.20 USD, registered above; the runner's
  repair first. I started no session.
- **The real count of the landed text**: each draft here is one line changed on this base; a landing
  that changes other lines too is refreshed at the landing.

Finished 2026-11-14 at 10:42 CET (`date`). My folder holds every file named above; nothing of mine was left
running (`pgrep -fl 248-spec-warden`, and `docker ps --filter name=sw-248`, both empty at the end).
