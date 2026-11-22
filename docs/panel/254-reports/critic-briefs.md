# Panel 254, the completeness critic's first reading: the briefs, before any seat

Written 2026-11-22 from 01:45 to about 02:10 by the completeness critic. No verdict on the proposal;
each finding names the command run and its result. Commands ran in the critic's copy
`.claude/worktrees/p254-scratch/critic/` (detached at `89f155330`, `git status` clean at 01:45; my files
under its untracked `p254/`) unless named otherwise; quick checks used the coordinator's stage 3,
`lane-p254/heroes-next` (`--version`: `heroes 0.6.0`, `sources 6e891e681c14bca0`), read-only, run from my
copy with `TMPDIR` inside it. The briefs were read from `lane-p254/docs/panel/254-briefs/` as they stood
at 01:44. Nothing in the blind area was run but its two read-only checks; no paid run, no API call.

Each finding is tagged: **verified**, **false** (with the right value), **unverifiable by command**,
**ambiguous**, or **missing** (a question, route or owner the briefs lack).

## Findings

### A. The framing facts, fact by fact

1. **The arms reproduce (verified).** `python3 -I .../arms/make_arms.py spec/heroes-spec.md p254/arms`
   printed `now ffd687d6...2d33 510 lines`, `m 47248ce1...fd73 516 lines`, `m0 1dc1446d...ccee 516 lines`;
   `shasum -a 256` of the three equals `arms/arms.sha256` line for line; `cmp` of the regenerated
   `spec-m.diff` and `spec-m0.diff` against the briefs' copies: identical; `wc -l`: 84 and 75. The blind
   area's `arms/spec-*.md` hash to the same three values (`shasum -a 256`), and its `tools/arms.sha256`
   is the same file.

2. **The frozen commit and the rule at `691531908` (verified).** `git log --oneline 89f155330..rebel-round`:
   one commit, `691531908`. `git diff --stat 89f155330 691531908`: the four files the brief names,
   13 insertions, 8 deletions. `git show 691531908`: CLAUDE.md § Precedence's added sentence and
   `.claude/agents/spec-warden.md`'s *"Neither veto holds against the most robust resolution ..."* read
   word for word as `00-shared.md` § 2 and `spec-warden.md` quote them; the commit body's *"design.md §1.6
   keeps its text until a sitting reconciles it"* also. `git worktree list`: `lane-mailbox` at
   `691531908`, `lane-p254` and the four seat copies at `89f155330`.

3. **Feature 2647 (verified, with two things the brief leaves out).** `gh issue view 2647 --repo
   heroes-lang/heroes`: the quoted sentence is its title, open, milestone M-rebel-library. Two things the
   brief does not carry: (a) the issue's own sketch of a repair is **a channel of texts with a bounded
   send** (*"with a bounded send and a receive waiting by a `time.Duration` and failing `stopped`"*), so
   S6's unbounded queue departs from the filer's sketch, which `00-shared.md` § 5 question 3 should name;
   (b) it names a **fifth consumer**, the gateway demo's readiness protocol over a loopback TCP port
   (`demo/notify.hero`), which the table of four applications omits. It also records that `ipc.hero`'s
   channel by name was searched and judged not to carry values between threads of one process; see
   finding 27 for why that is a question, not a premise.

4. **The four applications (verified).** `sed -n` of `gateway/core/state.hero:7-11`,
   `registry/server/loop.hero:1-12`, `chat/server/serving.hero:1-3`, `chat/server/hashing.hero:1-12`,
   `notes/http/server.hero:1-3`: each quotation in `00-shared.md` § 3 stands at the line given.

5. **design.md Part 7.13 (verified, the range one paragraph short).** `grep -n`: item 13 opens at
   `docs/design.md:4118`; *"isolated per-thread heaps, copying at the boundaries"* at 4140; *"A message is
   any Heroes value"*, *"The transport is a mailbox — one queue per thread; the sender copies into the
   receiver's heap and appends to the tail"*, *"Two copy regimes"*, the cost rule, *"do not build a
   scheduler"* and the refc-binaries sentence (ending 4180) all read as quoted. **The item runs to 4189**,
   not 4180: its last paragraph, *"Net effect on v1: two rules, both already true, both now invariants"*
   (one allocation point; refcount operations behind a narrow, never-inlined boundary), is the invariant a
   deep copy and a queue's count must keep, and the cited range leaves it out. Give the seats
   `4118-4189`.

6. **M-isolated-threads, `cow.c`, `suite_runtime.hero:227` (verified).** `wc -l`: 59. `grep -n
   'refcount == 1' runtime/parts/cow.c`: lines 44, 88, 93. The words *"a test and then a mutate that no
   memory order can close"* are the milestone page's (`M-isolated-threads.md:49-50`), not `cow.c`'s;
   the brief's sentence can be read either way, harmless. `sed -n 220,232p tests/harness/
   suite_runtime.hero`: line 227 is `constant SHARED_BY_DECISION: [str]`.

7. **Panel 246's R6 lines (verified).** `grep -n`: `hero_net_number` at `hero_net.c:2400-2402`,
   `hero_net_of` closing at 2457 (`sed -n`), so 2400-2453 is within four lines; `net.hero`'s
   `accept_number` at 265 and `conn_of` at 276, their comments from 258. Rebel's only numbered hand-over
   is `accept_number`/`conn_of` (`grep -n -E '^function [a-z_]*_number\(|...'` over the package: no other
   `_number` hand-over), as the ffi-pragmatist's Q1.2 says.

8. **Panel 251's file (verified, with one false citation).** `wc -l`: 344. `grep -n '^#'`: Q3 opens at 65
   and its proposal runs to about 136; R3 is `267-283`; `docs/panel/251-reports/compiler-engineer.md:166-167`
   reads *"288 `error[non_exhaustive]` in 109 files"*; `:170-172` the parser arm, *"`parse/type.hero` +12
   and `ast.hero` +6 ... 24 matches on `TypeKind` non-exhaustive in 12 files"*; the author's verdict,
   ratified 2026-11-19, decision 2590 (lines 336-344). **False: "Its critic's finding 6"** (`00-shared.md`
   § 4). The two mailboxes disagreeing on a send after the close is **finding 8** of
   `docs/panel/251-reports/critic-reports.md` (`### 8.` at line 342); "6" is its rank in that report's
   summary list (line 635). Finding 6 there is about Q2's hang. Repair the citation.

9. **W4's location (false as a pointer).** `spec-warden.md` cites W4's condition at
   `docs/panel/251-...md:270-272`; those lines only name it (*"its stated condition met (W4 false for both
   spellings)"*). W4 is stated at `docs/panel/251-reports/spec-warden.md:92` (*"`now` right ≤ 2 of 5;
   `proposal-b` right ≥ now + 3 and `proposal-m` right ..."*) and its veto condition at `:217-222`
   (`grep -n 'W4'`). Give the spec-warden both lines.

10. **The names (verified; one nuance on the 30).** In my copy: `git grep -n -E '^function
    (spawn|spawn_sized|mailbox)(<[^>]*>)?\(' -- '*.hero' ':!**/heroes.packages/**'`: 3 lines,
    `mail/internal/address.hero:141`, `threads.hero:61`, `threads.hero:77`; per name with `git grep -c` and
    `awk`: `wait` 10, `send` 26, `receive` 13, `close` 84; `git grep -h -E '^ +function <name>\('`: `close`
    30, `send` 0, `wait` 0, `receive` 0. **But 26 of the 30 `extern close` members are copies under
    `docs/panel/178-reports/`** (a sitting's evidence), 3 under `tests/golden/check/`, 1 under
    `tests/golden/run/` (`git grep -l ... | awk -F/ ... | uniq -c`), as panel 251's critic already found
    (its finding 10's row). The live bindings of C's `close(2)` are 4. Say so to the ffi-pragmatist, whose
    Q2.1 re-measures a veto on this count. **The 3 is the whole of the collisions**: a local, a parameter or
    a `for` variable named `spawn`, `spawn_sized` or `mailbox`, 0 by `git grep` of `name =`, `name @=`,
    `name:` and `for name in`; the 6 lines it finds for `mailbox` are the field of `dns.hero`'s SOA case
    (`:822`) and its constructions, and a built-in's name as a field is accepted today (`p254/t2p5/f.hero`,
    fields `len` and `exit`, `heroes-next check` exit 0).

11. **"73 tracked `.hero` files bind `hero_thread_spawn`" (false as worded).** The brief's command
    counts files that *mention* the name: 73. `git grep -l -E '^ +function hero_thread_spawn'`: **69**
    declare a member; the four that only mention it are `examples-rebel/wordbands/main.hero`,
    `selfhost/emit/guarded_names.hero`, and two `tests/golden/run/fixedbugs-` cases (169, 2492). The 53
    under `tests/golden` is of the 73. `selfhost/main.hero:88-113` binds `hero_thread_spawn_sized` and
    `hero_thread_join` (verified), and **six `selfhost/` files and `tests/harness/` bind the doors**: S14's
    refusal would reach the compiler and the net themselves, which the compiler-engineer's Q1.6 should
    count. 13 files `use <rebel>/threads` (verified, listed by `git grep -l`).

12. **The runtime and platform lines (verified).** Each by `sed -n` or `grep -n` in my copy:
    `desc.c:19` *"The struct has five members and gains no sixth"*; `spawn_str.c:14-27` (*"Each `str` is
    born and dies on one thread"*, at 25-26); `hero_poll.h:54-57` *"ONE CALL AT A TIME ON A SET ... no
    number names a set"* and `:61-70` the stop ending a wait *"even with entries ready"*;
    `hero_stream.h:256-260` (`close` *"on the thread that leaves the stream last"*);
    `hero_handle_tag.h:81` *"15 is free, the last of four bits"*; `alloc.c:1123` `hero_handle_eq_at`,
    `:314` `hero_spawn_check_joined()`, `:1155` `hero_handle_acquired_at` under `hero_handle_lock`;
    `spawn.c:109` `HERO_SPAWN_SLOTS 256`, `:739` `hero_spawn_check_joined`; `lend.c:143-186` all
    `_Thread_local`; `selfhost/cli/layer.hero:1-23`; `hero_net.c:149-151` `ws2_32`;
    `selfhost/cli/flags.hero:253-255`; `selfhost/cli/runtime_key.hero:55-63`; `hero_tls.c` 5,629 lines,
    THE APPLE ARM at 15-26; `tls.hero:379` `accept`; `net.hero:143-149` `Conn`; `hero_net.c:520`
    `hero_net_find`; `hero_layer_alloc.h:21-24`. **No `heroes` flag gives ThreadSanitizer**: `grep -rn
    fsanitize selfhost/cli/` finds `address,undefined` and the value probe's checks only. Panel 251's
    TSan lines, `critic-reports.md:307-309` (verified).

13. **The vendored measurements (verified).** `heroes-next measure` on my regenerated arms: `now`
    8863/9022 (max **9022**), `m` 9251/9422 (**9422**, +400), `m0` 9040/9208 (**9208**, +186); on
    `spec/heroes-spec.md`: **real 11788** (`claude-opus-5, 2026-11-20`), *"Headroom: 4596 against the
    16384 ceiling — but the FFI floor mortgages 60 of it"*. **A question on the instrument, not a
    falsehood**: the real count is `claude-opus-5`'s tokeniser (`selfhost/measure/pinned.hero:58`, `:85`),
    and the blind readers run `claude-opus-5-5` (`run.sh:98`). CLAUDE.md measures *"on the READER's
    tokeniser"*; whether the two models share one tokeniser is unrun here (it needs an API call); the
    spec-warden should say which model its four refreshes count with, and why that is the reader's.

14. **`--refresh` refuses every path but the two judged documents (verified by reading, unrun).**
    `selfhost/measure/pinned.hero:138-148`, the test *"the refresh refuses every file no ceiling judges,
    on the path and not on the key"*, asserts exit 2 for `design.md` and `ROADMAP.md`. Not run: it would
    be an API path.

15. **The machine (verified now; the earlier readings unverifiable).** `clang --version`: Apple clang
    21.0.0, `arm64-apple-darwin27.2.0`; `sysctl -n hw.ncpu` 8; `pmset -g assertions`: Amphetamine's
    `PreventUserIdleSystemSleep` held at 01:56. The load readings of 00:55 and 01:38 and Docker's answer
    at 00:55 are past observations no command re-reads (`uptime` read 4.83 at 01:56). The build log
    `lane-p254/build/p254-build.log` holds the three `real`/`user`/`sys` triples and `sources
    6e891e681c14bca0` as § 7 quotes them. `rebel-scratch/coord/lane-rules.md` exists.

### B. The blind seat (`llm-ergonomist.md`, `blind/`, the area)

16. **t2 program 5's registered row contradicts the arm it is graded by (wrong row).** The row says
    *accepted*, `sent`, `freed 1`, then *an abort at `free(b)`*. But `free(b)` follows `m.send(@b)` on
    every path, and the arm's § 11 says that send *"empties `v`, its life going with it"*, while § 13
    (unchanged, `spec/heroes-spec.md:481`) says *"Giving one back ends it: reading it where every path
    has ended it is a compile error"*. The nearest program today's compiler builds settles which way the
    compiler reads that sentence: `p254/t2p5/c.hero` (`free(b)` twice, straight line) and `d.hero` (a
    `transfers` call, then `free(b)`): `heroes-next check` refuses both, **`error[handle_used_after_end]:
    b was given to ... which ended its life, and this reads it again`**, with the note *"only where the
    life ended on every path to the read: an end on one branch, in an earlier turn of a loop, under
    `when`, through a copy or inside a helper is the runtime's to stop"*. So a reader citing § 13 says
    *refused*, and a third reading (*"empties"* leaves a null `Block`, and `free` of a null handle ends
    normally) is also open. **Repair before the draw**: either register the row as *refused, reading `b`
    where every path has ended it* (and grade *abort* wrong), or move the second `free(b)` to a place the
    note gives the runtime (behind a branch the checker cannot decide, or through a copy taken before the
    send) so that *abort* is the only reading; and register what an *"emptied"* binding is.

17. **The same sentence makes S3's text incomplete.** *"A thread waited for twice aborts"*: on the
    measured reading of § 13 above, `t.wait()` twice in a straight line is a compile error, and the abort
    is the runtime's for a copy, a loop or a branch. The proposal's sentence states one of the two; the
    spec-warden should weigh *"a thread waited for twice is refused where every path has waited, else
    aborts"* or the reference to § 13's rule.

18. **t2 program 3's row rests on one reading of an ambiguous sentence (ambiguous row).** The arm's § 13
    sentence reads *"after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and
    `hero_net_ask_stop() -> ()` only make ... a mailbox's `receive`, answer stopped"*. Program 3 calls
    `hero_net_ask_stop()` with no `hero_net_catch_stop()`; a reader who takes *"after
    `hero_net_catch_stop()`"* to govern all three predicts `got queued`. The runtime's contract says the ask
    alone suffices (`runtime/hero_net.h:193-208`: *"from this call, on every thread ... It cannot fail"*),
    but the session reads only the spec. **Repair**: call `hero_net_catch_stop()` first in program 3 (one
    line, binding it in the same group), so the row holds on either reading.

19. **The other four t2 rows hold on the arm's words (checked by reading against the spec).** Program 1:
    `timeout` on an empty open mailbox, `closed` on the send after the close, the two queued values
    received after the close (*"the values still in it are received"*), `closed` once closed and empty, the
    second close doing nothing. Program 2: `while true` with no `break` ends a path (`spec:278`), so
    `total` is accepted; `total 12`. Program 4: `_ =` discards a value that is not a `T?` (`spec:175-181`);
    the analogue today, `_ = malloc(16)` with `acquires free` (`p254/t2p5/e.hero`), is accepted and aborts
    at `main`'s end naming *"1 C handle(s) never given back"*, the shape of the row's ending. `print`
    writes its values with no separator (`spec:363`), as every row assumes. Note for the grader: program 1
    and program 2 also depend on *"the values still in it are received"* and *"closed once it is closed and
    empty"*, which `m0` does not say, so `m0`'s low counts there measure the sentences, as D2 intends.

20. **t1's expected lines (verified) and one literal written wrong.** A reference in Python
    (`python3 -I`, `str.split(" ")`, leave-out `the a to`): the task's own four lines `a 4,4`, `b 3,3`,
    `c 4,3`, `d 1,1`; S1 `4,4`; S2 `3,3`; S3 `8,7`; S4's text is unchanged, so `4,4` under the new name.
    All as registered. **But S3's table writes `"q"` unescaped while saying "(Heroes escapes)"**: in a
    Heroes string the quotes must be `\"q\"` (`spec:84-87`, seven escapes; `\u{1e}` and `\u{1f}` are
    legal, lowercase and no leading zero). Write the literal exactly as the grader will paste it.

21. **t3's reference (verified).** `python3 -I` over the task's rule: `red 402`, `green 400`, `blue 398`,
    largest 402. The largest reply is deterministic whatever the interleaving (counts only grow, and the
    last addition to `red` is answered with 402), so row (6) is sound.

22. **D1 compares two different instruments (missing).** `t1 now` is graded by a compiler and five runs
    plus four substitutions; `t1 m` by a reader's rubric of seven criteria with no compiler. Every slip the
    compiler refuses and a reader does not check (an unused binding, a cell nothing re-binds, a missing
    `@`, a name refused, `error[never_rebound]` as `p254/t2p5/a.hero` drew) counts against `now` alone, so
    `E1` is biased toward `m` by construction, before any session runs. **Register before the draw** one
    of: grade `now`'s answers by the same seven criteria too and report `E1` both ways; or build `m`'s
    answers by a stated mechanical translation; and say which `E1` D1 reads.

23. **The voiding rules lack two cases SKILL.md and the runner already see.** (a) SKILL.md § 2 (amended
    at `cb2ad6d7d`, read at `lane-p254/.claude/skills/panel/SKILL.md:219-221`): *"a scan that read nothing
    being no clean reading"*. `run.sh` enforces that for the preflight only (`grep -- '-preflight '`); the
    report's rules void a session the scan reads with a leak and say nothing of a session whose transcript
    the scan does not find. (b) `tools/after.py` logs each session's `modelUsage`, and no rule voids a
    session served by another model than `claude-opus-5-5` (SKILL.md: *"2.1.274 refused
    `claude-opus-5-5`"*). Add both to § Voiding before the draw.

24. **"Letters that name no arm" (false).** `tools/make_sessions.py`: `pool = list('cfghjkmpqvwxyz')`,
    which holds `m`, the name of an arm. A draw giving `m` to `m0` or `now` writes folders `t2-m-<n>` that
    mislead the graders, and the brief's sentence is false of the script as written. Drop `m` from the pool
    (and any other letter an arm or a task is named by).

25. **What no check of the area looks for: a read outside the session's folder (missing).** The area's
    top holds `arms/spec-now.md`, `spec-m.md`, `spec-m0.md` by name, `tools/` (the tasks, the draw
    script) and, after the draw, `key.txt`. SKILL.md's reading of 2026-10-01 says `--restricted`
    *"confines the file tools to the folder"*; the preflight does not test it, and `tools/scan.py` counts
    the prompt, a git status and `CLAUDE.md`/memory only, never a `Read` whose path is outside
    `s/<label>/`. Unrun whether today's CLI keeps that confinement. Cheap repair: the scan also flags any
    tool call naming a path outside the session's folder, and the report voids it.

26. **The area's set-up conforms (verified).** `python3 -I tools/check_area.py` from inside the area: exit
    0; `bash -n run.sh`: exit 0. `diff` of panel 253's `run.sh` (extracted from
    `docs/panel/253-reports/evidence.tar.xz` into my `p254/ev253/`) against the area's: exactly the four
    changes the brief names (header, `CAP=3.00` and `CAP_VERDICT=3.00`, `CEILING=20.00`, `WARDEN`);
    `scan.py` and `check_area.py` identical to 253's (`cmp`); `brief-session.md` identical to a 253
    session's `brief.md` in that archive; every file of the briefs' `blind/` identical to the area's
    `tools/` copy (`cmp`). The area: its own empty repository (`git status`: *No commits yet*),
    `.gitignore` `*`, local user `reader` `reader@invalid`, `home/` and `tmp/` present; `run.sh` gives
    every session `env -i`, `HOME`, `TMPDIR`, `CLAUDE_CODE_TMPDIR` inside the area, `< /dev/null`, the
    warden gate (heading plus a non-empty line, copied and hashed), a free preflight with a refused key,
    `--max-budget-usd 3`, and stops before `spent + 3.00 + 0.10 > 20.00`. No `s/`, `key.txt` or
    `order.txt` yet (not drawn). The brief's spend inference *"about 3.5 to 10 USD"*: the cited rates
    give 3.4 to 5.7 USD for 26 sessions (`26 × 0.1322`; `25 × 0.2112 + 0.3772`); the upper 10 is not their
    arithmetic and should say what it adds (t1 and t3 write whole programs).

### C. Negative sentences and the option set

27. **A route nobody listed: today's tree, with no new C (missing; now run, and it works for t3's
    shape).** `runtime/hero_ipc.h:5-55` (the channel at 43, the bounded waits at 49): a platform member
    already gives *"a local channel by name"*, which *"IS A STREAM"*, every wait bounded and ending at the
    stop, and `hero_poll.h` takes a stream into a set. Issue 2647 says `ipc.hero`'s face was searched and
    *"none carries a value between running threads of one process"*. **Run, in my copy**:
    `p254/a0/main.hero` (its `heroes.toml` naming the copy's Rebel), t3's exact task over Rebel's `ipc`:
    the main thread listens on `c.sock` and keeps the counts, four `threads.spawn` workers each connect by
    name, send 100 two-byte requests with `ipc.write_message` and wait for each reply with
    `ipc.read_message`, and answer their largest reply; `heroes-next build` exit 0; **5 runs of 5 plain
    and 3 of 3 under `--sanitize` print `red: 402`, `green: 400`, `blue: 398`, `largest: 402` and exit 0**,
    each under `timeout -s KILL 30` (60 sanitized), `pgrep` empty after. So a running thread exchanges
    values with another, in one process, on the frozen tree, with no new C and no spec token: the issue's
    sentence is false of `ipc`'s channel as a transport (it is true that no *value* crosses whole; values
    go as bytes). What this probe does not settle: the keeper here is `main` and serves the four channels
    one after another (a `poll` set over them would serve them together, unrun); a TLS connection still
    cannot cross (2647's other half). **Route A (a new member `hero_mail.c` taking the last tag, 15) must
    be priced against this route A0**, which costs a socket file and serialisation and nothing in the
    runtime; the ffi-pragmatist's Q3 should build A0's gateway shape beside A's, and the synthesis should
    record A0 as what the most conservative route would have been.

28. **Routes panel 251 named and this sitting drops (missing).** `docs/panel/251-...md:127-130`
    names, *"not proposed"*, (i) Part 7.13's first rung, plain data parallelism, any value copied at
    `spawn` and `wait` with no mailbox (R3's reopening condition *"a program on the closure list that
    needs a record across a thread"* is about exactly that half); and (ii) **7.13's own mailbox, *"a queue
    per thread reached through its `(thread T)` and received inside the body, which needs no aliasing
    value at all"***, which that sitting's compiler-engineer was to price beside the proposal. It keeps
    § 3's *"No aliasing exists among the values this language owns"* true and forms no cycle (finding 29).
    And 251's critic (finding 11, `critic-reports.md:463`, `:488`) left *"a spelling nobody priced"*: B's operations under names no
    declaration or member uses. None is in `00-shared.md` § 5; each is a route whose absence the record
    must explain, or a seat must price. The shared-state routes the issue names (Go's `sync.Mutex`, Rust's
    `Arc<Mutex<T>>`) are refused by design.md §4.10 and 7.13; the brief should say so in one line so the
    synthesis does not have to.

29. **S5 with S9 admits a reference cycle and is itself a cross-thread refcount (missing; a soundness
    question).** S5 gives the queue back *"with its last copy"*: a count of copies held by several
    threads, which is the *"cross-heap refcount — which is an atomic counter, i.e. the one thing this
    design gets to avoid"* of 7.13 (`design.md:4176-4180`), applied to the queue. And S9 lets a
    `(mailbox U)` travel inside a value. A record holding `[(mailbox R)]` is allowed by § 3 (*"it may
    contain itself only through `[T]` or `{K: V}"*), so `m.send(R(boxes: [m]))` puts a copy of `m` inside
    its own queue: once the program drops its own copies, the last copy is in the queue, which is never
    given back, and **S5 tells nothing at `main`'s end for a mailbox**, so the leak is silent (and any
    handle queued with it is then told as a handle never given back, blaming the program for the
    runtime's cycle). design.md §4.10's second gift, *"Reference cycles become impossible, so refcounting
    no longer leaks"* (`design.md:1827-1830`), becomes false. Question 1 (S5 against a handle's life) must
    weigh this; the compiler-engineer can build it in Q2.3's stress; and design.md §4.10 joins the
    sentences the resolution amends.

30. **S11 for `spawn` is unsound as written (missing; a soundness question).** The arm moves a handle's
    life only for *"a `send` written `m.send(@v)`, which empties `v`"*; through `spawn(f, x)` a handle
    *"crosses"* with no `@` and no word on what the spawner keeps. So `spawn(serve, db)` leaves the
    spawner a live copy of `db` while the thread uses it: two threads in one SQLite connection, or one
    poll set (*"ONE CALL AT A TIME ON A SET"*), at once. Panel 251's text said *"its life ending in the
    sender"* for both; the proposal kept it for the send alone. The ffi-pragmatist's Q1 and the
    compiler-engineer's Q1.5 should be asked this case by name (`spawn(f, @x)`, or the spawner's copy
    ended).

31. **S5 and S8 against a handle inside a queued value (contradiction).** *"The values still in it are
    received, or given back with its last copy"*: the runtime cannot give back a handle (only the
    program's `consumes` call does, `spec:473-480`), so a queued handle at the queue's end is either told at
    `main`'s end as never given back (and *"given back"* is false of it) or silently lost. Panel 251's
    critic named the same gap (its finding 8(a), last row). `00-shared.md` question 6 asks what is told;
    the row text itself should be repaired either way.

32. **The stop is process-wide and sticky; the brief does not tell the seats (missing).**
    `runtime/hero_net.h:193-199`: *"from this call, on every thread, a call of this layer that would wait
    answers ... `HERO_NET_STOPPED`"*, and a second call changes nothing. Under S7 every `receive` in the
    process then answers `stopped` for the rest of its life, **a reply mailbox whose reply is already
    queued included**, while S6's `send` has no `stopped` answer and keeps queueing into mailboxes nobody
    can drain. Question 2 (stop against drain) should state this premise and ask for the in-flight request
    and the reply mailbox by name.

33. **Text gaps in `spec-m.diff` beside § 9 and § 3 (missing for the spec-warden).** (a) § 7's
    *"A `ptr`, a `cstr` and a function value compare as an address"* (`spec:231-232`) is not amended, so
    `==` on a `(mailbox T)` reads as structural equality of a queue; and a mailbox as a map key is
    unsaid. (b) § 13 defines a handle as *"a group's record with a `tag` and no fields"*
    (`spec:442-447`); `(thread T)` is called *"a handle (section 13)"* but is no group record, so
    `==` on it, a map key of it, and its null are unsaid. (c) `spawn_sized`'s result type and failure are
    still unsaid (*"asks a stack of at least `n` bytes"*), the defect panel 251's spec-warden and critic
    (finding 8(c)) both named. (d) *"`T` taken from where it is used"* against § 4's *"Signatures are
    always explicit; inference is local only"* (`spec:172`): every t2 program annotates `m: (mailbox i64)
    = mailbox()`, so the blind seat never tests the unannotated form. (e) the § 9 sentence *"`x.f(y)` is
    sugar for `f(x, y)` (UFCS), but on a `(thread T)` or a `(mailbox T)`, whose operations ... are written
    only after a dot"* can be read as *UFCS is off for those receivers* (so `m.drain()` calling the
    program's `drain(m)` is refused) or as *UFCS stands and the four names are added*; the compiler-engineer's
    Q1.3 asks the resolution, and the spec-warden should be asked the sentence's meaning by this example.

34. **The deep copy's option set is stated two ways (inconsistent).** `00-shared.md` § 5 question 4:
    *"a deep copy needs a function per type or a sixth member"*; `compiler-engineer.md` Q2.2 adds a third,
    a flat encoding into one block rebuilt on the other side. Say the set once, and as the
    compiler-engineer's to widen, not as a premise.

### D. Who owns what

35. **The Windows arm of the queue is nobody's (missing).** The runtime's queue (route B) or member
    (route A) waits on a condition variable; the runtime is on every program's link line, and Windows has
    no pthreads (`tests/harness/suite_runtime.hero:230`, in `SHARED_BY_DECISION`: *"`pthread.h` is not found
    under clang targeting MSVC"*). `00-shared.md` § 6 marks only Linux unrun. Name the Windows arm (its
    wait, its wake on the stop, `ws2_32` if the stop's member is linked) as owed to the CI's legs or the
    box, and give it to the compiler-engineer (B) and the ffi-pragmatist (A) to write, if not to run.

36. **The spec-warden's predictions need a file its brief does not name.** It must predict per t2
    program *"from this brief and `llm-ergonomist.md`"*, which names the programs' rows but not their
    text; the text is `docs/panel/254-briefs/blind/task-t2.md`. Name the path in its § FIRST.

37. **Measured by two seats, by design, and nothing unowned among the main measurements.** The stop's
    link (`nm`) is asked of both the compiler-engineer (Q2.4) and the ffi-pragmatist (Q2.3): fine as a
    cross-check, but say so. The TLS hand-over is the ffi-pragmatist's in both routes; gateway B the
    compiler-engineer's, A the ffi-pragmatist's: verified by reading both briefs. Unowned: the cycle of
    finding 29, the `spawn` hand-over of finding 30, route A0 of finding 27, and the walk a new surface
    form owes the tools that re-print a program (`.claude/rules/diagnostics-and-goldens.md`, CLAUDE.md
    § 9: the formatter first, then `heroes grammar`, the editors' grammar, the site's colouring by
    `heroes lex`); the compiler-engineer's Q1.2 counts the formatter's `match` statements, not that walk.

## Not checked, and why

- Whether `claude-opus-5` and `claude-opus-5-5` share a tokeniser (finding 13): needs an API call.
- Whether `--restricted` still confines `Read` to the session's folder (finding 25): needs a session.
- Route A0 with a `poll` set serving the channels together, with a keeper on a thread of its own, and
  under TSan (finding 27): not built for time; owed to the ffi-pragmatist beside route A.
- Docker, and the load at 00:55 and 01:38: past readings.
- No compiler built in my copy: the coordinator's stage 3 sufficed for every check above.

## What the coordinator must repair before launching the seats

1. **t2 program 5's row** (finding 16): `free(b)` after `m.send(@b)` on every path is a compile error by
   § 13's own sentence and by today's compiler's `handle_used_after_end`; re-register the row as refused,
   or move the second `free` where only the runtime can stop it, and register what *"empties"* means.
   And program 3 (finding 18): call `hero_net_catch_stop()` before `hero_net_ask_stop()`.
2. **D1's asymmetric instruments** (finding 22): register, before the draw, how `t1 now` and `t1 m` are
   graded by one rule, and which `E1` D1 reads.
3. **The soundness questions the briefs do not ask** (findings 29, 30, 31, 32): the cycle and the
   cross-thread count S5 implies, against 7.13 and design.md §4.10; a handle through `spawn` without
   `@`; a handle in a queued value at the queue's end; the stop's being process-wide and sticky. Put each
   to the compiler-engineer and the ffi-pragmatist by name, and to the spec-warden as text.
4. **The routes not listed** (findings 27, 28): today's `ipc` channel by name (A0), which already
   carries t3's exchange between running threads, 5 of 5 plain and 3 of 3 under ASan + UBSan
   (`p254/a0/`), so route A is priced against it; 7.13's per-thread mailbox and its first rung; and the
   unpriced B spelling, each priced or excluded with a reason in the shared brief.
5. **The text gaps for the spec-warden** (findings 17, 33): S3's double wait against § 13's compile
   error; § 7's `==` for a mailbox; `(thread T)` as a handle § 13 does not define; `spawn_sized`'s result;
   *"from where it is used"*; the UFCS sentence's two readings.
6. **The blind seat's rules and draw** (findings 20, 23, 24, 25): S3's literal with `\"q\"`; void a
   session with no transcript found and one served by another model; drop `m` from the label pool; a scan
   flag for any read outside the session's folder.
7. **False or loose citations** (findings 5, 8, 9, 10, 11, 3): 7.13 is `4118-4189`; 251's critic's
   mailbox finding is 8, not 6; W4 is at `251-reports/spec-warden.md:92` and `:217-222`; 26 of the 30
   `extern close` are panel 178's evidence; 69 files declare the doors (73 mention them), and the
   compiler and the net are among them; the issue's own bounded-send sketch and its fifth consumer.
8. **Owners** (findings 35, 36): the Windows arm of the queue; the path of `task-t2.md` in the
   spec-warden's § FIRST.
