# Panel 254, the shared brief: values that cross threads whole, the language route (feature 2647)

Written 2026-11-22 from 01:40 by the sitting's coordinator, **repaired after the completeness critic's
first reading** (`docs/panel/254-reports/critic-briefs.md`, 37 findings; each repair names its finding). **Every number, path and count below was
produced by a command run while this file was written, and the command is named beside it**; where a
sentence could not be run it says *unrun* or is put as a question. Read this file whole, then your own
brief, then begin. **Your report is written as you go**, never only at the end, at
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/docs/panel/254-reports/<seat>.md`.

## 1. The sitting, and why it is a full panel

- **The frozen commit**: `89f155330d3da43b429f6ad8f8c6c730b171a35d` of `rebel-round`
  (`git -C .claude/worktrees/lane-p254 log -1 --format=%H`), the lane `lane-p254` holding it. One commit
  stands on `rebel-round` after it, `691531908`, which moves four files and nothing a seat measures but
  the contract (`git diff --stat 89f155330 691531908`: `.claude/agents/spec-warden.md`, `CLAUDE.md`,
  `selfhost/measure/pinned.hero`, `tests/harness/suite_spec.hero`); its rule is § 2 below, read from it.
- **The author's instructions, all of 2026-11-22, each relayed by the coordinator of M-rebel-library**,
  in the order given and meant as:
  1. *fix feature 2647: in Heroes a thread receives one value when it starts and cannot exchange values
     while it runs, so gateway serves one connection at a time*;
  2. *if it is needed, the specification can be touched too* (both routes asked, the most robust and
     complete adopted, never the smallest);
  3. *go directly to the most robust solution, and spending up to $20 of tokens is fine*: so this sitting
     is a **full panel on the LANGUAGE route (B)**, a `(mailbox T)` any Heroes value crosses whole as a
     copy, with `(thread T)`, the handle hand-over and the spec sentences they need, starting from panel
     251's own Q3 text, **spelling M** (operations after a dot; spelling B stays vetoed by the
     ffi-pragmatist's measurement of panel 251 unless a seat shows that veto no longer holds). **The
     library route (A) is recorded as what the conservative route would have been.** The blind seat's
     paid runs are allowed by the author up to **20 USD in all**;
  4. **the contract rule at `691531908`** (§ 2).
- **The seats**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory,
  web-verified) and the llm-ergonomist (the blind seat, `claude -p` sessions run by the coordinator from
  an area of its own, § 9); the completeness critic reads these briefs before any seat starts and the
  reports after.
- **A separate implementation lane, `lane-mailbox`, builds the route in parallel** from panel 251's
  proposal and merges only after this sitting's resolution, which must therefore state **the exact spec
  text and semantics** the lane is held to. Its worktree stands at `691531908`
  (`git -C .claude/worktrees/lane-mailbox log -1`). **No seat reads, builds in or cites `lane-mailbox`**:
  this sitting measures the route from the frozen commit, and that lane is judged by what it rules.

## 2. The rule that changed while this sitting was convened (`git show 691531908`)

CLAUDE.md § Precedence, as committed at `691531908`, the author's words of 2026-11-22:

> **Amended by the author 2026-11-22: the most robust solution now always wins**, a new form included and
> at the cost of the spec's size: Principle 0, the spec's budget (design.md §1.6) and the spec-warden's
> vetoes yield to it, and the ceiling is raised to fit.

And `.claude/agents/spec-warden.md` at its vetoes: *"**Neither veto holds against the most robust
resolution** (CLAUDE.md § Precedence, the author's instruction of 2026-11-22): there you report its cost,
measured, and the ceiling is raised to fit it."* The commit body adds that design.md §1.6 keeps its text
until a sitting reconciles it and that the contract outranks it where they disagree.

**What still binds**: a **soundness veto**, a program that corrupts memory or a race the sanitizers
read, is robustness itself (rank 3) and stands as a refusal. A budget or a Principle 0 objection is
recorded as a cost, measured, and does not stop the most robust resolution.

## 3. The question: feature 2647, and the four programs that need it

`gh issue view 2647 --repo heroes-lang/heroes` (read 2026-11-22 00:58): *"`threads` gives a running
thread no way to send or receive a value, so a server cannot serve its connections on several threads
around one shared cache, rate limiter or metrics registry"*; filed 2026-11-21 writing
`examples-rebel/gateway/`. The four applications that hit it on 2026-11-21, each saying so in its own
module comment (`sed -n` of each, read 2026-11-22):

| application | where it says so | what it holds on one thread |
|---|---|---|
| gateway | `examples-rebel/gateway/core/state.hero:7-11` | response cache, rate limiters, breakers, metrics: *"the gateway answers its clients' connections one at a time on that thread"* |
| registry | `examples-rebel/registry/server/loop.hero:1-12` | *"a SQLite connection and an index in memory, lives on one thread, and the language lets no handler carry it to another"* |
| chat | `examples-rebel/chat/server/serving.hero:1`, `chat/server/hashing.hero:1-12` | one poll loop; a password hash on a thread of its own that reports *"its job's number in one datagram to a loopback socket the loop waits on"* |
| notes | `examples-rebel/notes/http/server.hero:1-3` | one thread waiting with `poll` on every connection |

**And two things the issue says that the table above did not** (the critic's finding 3): a **fifth
consumer**, the gateway demo's readiness protocol over a loopback TCP port (`examples-rebel/gateway/demo/
notify.hero`), and **the filer's own sketch of a repair: *"a channel of texts ... with a bounded send and a
receive waiting by a `time.Duration` and failing `stopped`"*** (the issue's Notes), which S6's unbounded
queue departs from (§ 5 question 3). The issue also records `ipc.hero`'s channel as searched and judged
not to carry a value between threads of one process; the critic ran it and it does (§ 5, route A0).

## 4. The history this sitting stands on (read each yourself; the lines are `grep -n`'s)

- **design.md Part 7 item 13**, `docs/design.md:4118-4189`: *"isolated per-thread heaps, copying at the
  boundaries"*; *"A message is any Heroes value"*; *"The transport is a mailbox — one queue per thread;
  the sender copies into the receiver's heap and appends to the tail"*; *"Two copy regimes"*; the cost
  rule; *"do not build a scheduler"*; Erlang's refc binaries as *"a cross-heap refcount ... a trade to
  make consciously if the copies ever hurt"* (ending at 4180); and its last paragraph (4181-4189), *"Net
  effect on v1: two rules, both already true, both now invariants"*: one allocation point, and refcount
  operations behind a narrow, never-inlined boundary, which a deep copy and a queue's count must keep.
- **M-isolated-threads**, `docs/roadmap/milestones/M-isolated-threads.md` (59 lines, `wc -l`): the
  refcount atomic since its step 3; `runtime/parts/cow.c`'s `if (refcount == 1)` *"a test and then a
  mutate that no memory order can close"*; `tests/harness/suite_runtime.hero`'s rule 3 (objects kept
  between calls are `_Thread_local`, `_Atomic` or in `SHARED_BY_DECISION`, its line 227).
- **Panel 246's R6**: a socket crosses as a number, `hero_net_number` / `hero_net_of`
  (`runtime/platform/hero_net.c:2400-2453`), the number's life in the program's handle set owed to
  `hero_net_of`; Rebel's `net.accept_number` / `net.conn_of` (`packages/rebel/0.1.0/files/net.hero:258-281`).
- **Panel 251** (`docs/panel/251-a-handlers-jump-...-a-whole-value-crossing-waits.md`, 344 lines), its
  **Q3** (lines 65-130: spellings B and M, the shared text, the names taken) and **R3** (lines 267-283),
  **not adopted** for four reasons: (1) the spec-warden's veto, its condition (W4) met; (2) Principle 0
  unpaid; (3) the cost, one new `Ty` case leaving **288** `match` statements non-exhaustive in **109**
  files (`docs/panel/251-reports/compiler-engineer.md:166-167`, reproduced by its critic); (4) design.md
  Part 7.13, which both crossings built then departed from (the shallow one is 7.13's declined cross-heap
  refcount; the deep one freed on one thread what another allocated). **What would reopen it**
  (R3's own words): a reading registered before it runs, task 2 with a separator in a document; a
  `(thread T)` that leaves under 30 `match` statements incomplete, or a program on the closure list that
  needs a record across a thread; a ruling on which of 7.13's heaps a crossing keeps; and answers to what
  the text left unsaid (a send and a waiting `receive` at the close, queued messages, a handle in a list,
  `spawn` failing). Its critic's finding 8 (`docs/panel/251-reports/critic-reports.md:342`; *6* is its rank in that
  report's summary): the two mailboxes built then **disagreed on a send after the close** (one aborted, one answered a status) and both gave `receive` a third answer, `closed`, that the
  text lacked. Ratified by the author 2026-11-19 (decision 2590).
- **R3's four reasons are re-measured here, each by its instrument** (the coordinator's instruction of
  2026-11-22): (1) the spec-warden's veto, now yielding to § 2's rule, measured as a cost by `heroes
  measure spec/heroes-spec.md` on the amended text; (2) Principle 0, **the author's instruction of
  2026-11-22 being the ruling on the need**, recorded with its date, and each seat says whether it reads
  it so; (3) the cost, the `match` statements a `(thread T)` and a `(mailbox T)` leave incomplete, counted
  again on this tree; (4) Part 7.13's isolated heaps, which heaps a crossing keeps.

## 5. The proposal: panel 251's spelling M, made complete

**As a spec diff**: `docs/panel/254-briefs/arms/spec-m.diff` (93 lines, `wc -l`), made from the frozen
`spec/heroes-spec.md` by `docs/panel/254-briefs/arms/make_arms.py`, every edit asserted to match exactly
once (first run 2026-11-22 before 01:38; **repaired after the critic's findings 17, 30, 31 and 33** and
run again at 02:04, exit 0; the text before is `make_arms-v1.py`'s); the whole amended spec is that
script's `spec-m.md`, sha256 `b008c3078855228f...` (`arms/arms.sha256`), **9,488** vendored against
`now`'s 9,022 (+466; `heroes-next measure`, the coordinator's stage 3). Panel 251's own text ported to today's spec is `spec-m0.diff`
(75 lines), sha256 `1dc1446d4fc99a03...`. What `spec-m.diff` changes:

- **§ 3**: two rows, `(thread T)` *"a running thread whose function answers a `T`; a handle (section 13)
  its wait ends"* and `(mailbox T)` *"a queue of `T`s, which every copy of it reaches (section 11)"*;
  the address sentence *"A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS ... two copies reach one
  foreign thing, or one queue, ... what it reaches"*; `Prefix` gains `"(" ( "thread" | "mailbox" ) Type ")"`.
- **§ 7**: *"A `ptr`, a `cstr`, a `(thread T)`, a `(mailbox T)` and a function value compare as an
  address"* (the critic's finding 33a).
- **§ 9**: *"`x.f(y)` is sugar for `f(x, y)` (UFCS), except a thread's `wait` and a mailbox's `send`,
  `receive` and `close` (section 11), which are written only after a dot. There are no other methods,
  ..."* (panel 251's spec-warden found its M text left § 9's UFCS sentence contradicting itself; the
  first repair here read two ways, the critic's finding 33e; this one names its four exceptions, so UFCS
  stands for every other name: for the spec-warden to judge).
- **§ 11**: `spawn` · `spawn_sized` · `mailbox` join the built-ins; one paragraph, *"Threads, also
  provided: ..."*, and one line naming what is taken and what stays free.
- **§ 13**: *"threads, "* and the thread doors' sentence leave the first paragraph; the stop sentence
  gains *"or a mailbox's `receive`"*; the lease sentence reads *"a thread nobody waits for"*.

**The semantics, one row each, which the implementation lane will be held to** (the proposal's; every
seat attacks them, and the resolution states the rows it adopts):

| row | the proposal | panel 251's M text (`m0`) |
|---|---|---|
| S1 `spawn(f, x)` | runs `f(x)` on a thread of its own; gives `(thread T)?`, **failing `limit`** where the system refuses | gives `(thread T)`; failing unsaid |
| S2 `spawn_sized(f, x, bytes: n)` | **does the same** (`(thread T)?`, failing `limit`) on at least `n` bytes of stack; **`n` below 0 aborts** | unsaid |
| S3 `t.wait()` | waits, gives the answer, and **ends the thread's life as a call marked `consumes` ends a handle's** (§ 13: refused where every path has ended it, else an abort; the critic's finding 17); a thread nobody waits for aborts when `main` returns, saying how many | waits and gives |
| S4 `mailbox()` | a new, empty `(mailbox T)`, **its type written where it is bound**, `m: (mailbox Job) = mailbox()` (§ 5's *"inference is local only"*, the critic's finding 33d) | begins a mailbox's life *"as a handle's"* |
| S5 life | **a value, not a handle**: every copy reaches one queue, which goes with its last copy, the values still in it dropped with it, **a handle among them told when `main` returns as one nobody consumes** (the critic's finding 31); **nothing is told at `main`'s end for a mailbox itself** | a handle: one nobody closes aborts at `main`'s end; a copy used after the close (unsaid) |
| S6 `m.send(v)` | adds a copy of `v`, **never waits** (unbounded, 7.13's *"appends to the tail"*); `()?`, **failing `closed`** once closed | *"adds to it"*; its type and a send after the close unsaid |
| S7 `m.receive(within_ms: n)` | the oldest value; waits at most `n` ms, **`-1` no limit, below that an abort**; fails `timeout`; **`closed` once closed and empty** (so a closed mailbox is drained, oldest first); **`stopped` once a stop is asked, even with values in it** (as `hero_poll.h`'s wait: the stop wins) | fails `timeout` or `stopped`; `closed`, and the order of stop and values, unsaid |
| S8 `m.close()` | closes it for every copy; **closing again does nothing**; a `receive` waiting at the close ends `closed` once the queue is empty | ends its life *"as a handle's"* |
| S9 what crosses | **every value whole, as a copy**, through `spawn`, `wait` and a mailbox: numbers, `bool`, `str`, arrays, maps, records, variants, `T?`, function values, a `(mailbox U)` (a copy reaching the same queue, so a reply mailbox travels inside a request) | *"Every value crosses whole, as a copy"* |
| S10 never crosses | **a `ptr` or a `cstr`, or a value holding one**: refused (where: the checker, the proposal says; a seat says what it measured) | *"a `ptr` or `cstr` never crosses"* |
| S11 a handle | **a handle, or a value holding one (a `(thread U)` among them), crosses only written with `@`, `spawn(f, @x)` or `m.send(@v)`, which empty it, its life going with it, or as the answer `wait` gives** (the critic's finding 30: the first text let `spawn(f, x)` leave the spawner a live copy, two threads in one handle); a handle in a list goes as the list's elements do | *"another handle only into `spawn` or a send, its life ending in the sender"* |
| S12 the stop | § 13's stop: after `hero_net_catch_stop()` and a signal, or `hero_net_ask_stop()`, a waiting `receive` answers `stopped` | same, unsaid where it lives |
| S13 names | `spawn`, `spawn_sized` and `mailbox` taken; `wait`, `send`, `receive`, `close` written only after a dot and free for the program | the same |
| S14 the doors | the `hero_thread_*` doors leave the spec's text; whether a program may still bind them is **a question** for the compiler-engineer and the spec-warden | the same question, unanswered by 251 |

**Questions the proposal leaves open, each for the seats to settle by measurement** (the coordinator's
list, not a claim that it is the whole set; the critic says what is missing):

1. S5 against a handle's life (251's text): which is the more robust, measured; what each tells at
   `main`'s end; what a forgotten close costs a program under each (a worker blocked in `receive`
   forever, which `main` may then wait for and hang).
2. S7's stop against a drain: what a server's shutdown loses under each, measured on the gateway shape.
3. S6's unbounded queue against a bounded one with a send that waits: a deadlock each admits, memory
   each admits.
4. **How a value crosses whole**: the descriptor has *"five members and gains no sixth"*
   (`runtime/parts/desc.c:19`, design.md §4.20, panel 027), and its `copy` is today's shared copy
   (`desc.c`'s scalars; `selfhost/emit/perfn.hero` generates the rest); a deep copy needs a function per
   type, a sixth member, or a flat encoding into one runtime block rebuilt on the other side (the
   coordinator's list, the compiler-engineer's to widen; the critic's finding 34). **And 7.13's heaps**: `runtime/parts/spawn_str.c:14-27` already makes each
   `str` *"born and dies on one thread"* (bytes copied into a block of the runtime's on one thread, made
   a `str` on the other); which of the two crossings panel 251 built, or a third, keeps that for every
   value.
5. **`t.wait()` on a binding made with `=`**: whether consuming a handle needs a cell, what today's
   compiler does with `x = acquire()` then a `consumes` call, and what the text must say.
6. **The handle hand-over, S11**: which handles may cross at all (a TLS stream of
   `runtime/platform/hero_tls.c`, a socket, a SQLite connection, a poll set, whose header says *"ONE CALL
   AT A TIME ON A SET"*), what the runtime's handle set (`runtime/parts/alloc.c:1155-1260`, one entry per
   address, process-wide under one lock) and its dead values do when a life moves between threads, and
   what is told at `main`'s end for a handle left inside a mailbox.
7. **Where the stop lives**: today the flag and its wake are `hero_net.c`'s, a member of the platform
   layer linked only where called (`selfhost/cli/layer.hero:1-23`); a built-in `receive` that answers
   `stopped` is the runtime's, which every program links. What links where, measured (on Windows the
   member links `ws2_32`, `hero_net.c`'s top).
8. **A mailbox in a `poll` set** (chat's and notes' loops wait with `poll`, `runtime/hero_poll.h:132-185`
   takes sockets, streams and timers): needed for the route to be complete, and at what cost.
9. **The names**: `spawn`, `spawn_sized` and `mailbox` are declared today by `packages/rebel/0.1.0/files/
   threads.hero:61`, `:77` and `mail/internal/address.hero:141` (`git grep -n -E '^function
   (spawn|spawn_sized|mailbox)(<[^>]*>)?\(' -- '*.hero' ':!**/heroes.packages/**'`: 3 lines); the names
   that stay free under M are declared top-level `wait` 10, `send` 26, `receive` 13, `close` 84 times
   (same command per name, summed with `awk`), and **30 `extern` members are named `close`**, 0 `send`,
   0 `wait`, 0 `receive` (`git grep -h -E '^ +function <name>\('`); **26 of the 30 are copies inside
   `docs/panel/178-reports/`**, a sitting's evidence, so **4 live bindings** of C's `close(2)`, 3 under
   `tests/golden/check/` and 1 under `tests/golden/run/` (the critic's finding 10). No local, parameter or
   `for` variable is named `spawn`, `spawn_sized` or `mailbox` (the critic's `git grep`; the 6 lines it
   found for `mailbox` are a field of `dns.hero`'s SOA case, `:822`, and a built-in's name as a field is
   accepted today).
10. **The doors after M (S14)**: **73 tracked `.hero` files name `hero_thread_spawn`** (`git grep -l -E
    'hero_thread_spawn' -- '*.hero' ':!**/heroes.packages/**' | wc -l`), 53 of them under
    `tests/golden`, and **69 declare a member** (`git grep -l -E '^ +function hero_thread_spawn'`; the
    four that only name it: `examples-rebel/wordbands/main.hero`, `selfhost/emit/guarded_names.hero` and
    two `tests/golden/run/fixedbugs-` cases, 169 and 2492, the critic's finding 11); six `selfhost/` files
    and `tests/harness/` are among those that bind them, so a refusal reaches the compiler and the net; the compiler's own `selfhost/main.hero:88-113` runs every command on a thread of
    `hero_thread_spawn_sized`; Rebel's `threads` binds them (`threads/hero_os.hero`). 13 tracked files
    `use <rebel>/threads` (`git grep -l`).

11. **A queue that goes with its last copy is a count of copies held by several threads**, the
    *"cross-heap refcount ... the one thing this design gets to avoid"* of 7.13 (`design.md:4176-4180`),
    applied to the queue; **and S9 lets a mailbox travel inside a value, so a record holding
    `[(mailbox R)]` sent into its own mailbox puts a copy of it inside its own queue**: once the program's
    copies are gone the queue is never given back, and S5 tells nothing for a mailbox, so the leak is
    silent, and design.md §4.10's *"Reference cycles become impossible, so refcounting no longer leaks"*
    (`design.md:1827-1830`) becomes false (the critic's finding 29, a soundness question). Build it, and
    say which life (S5, a handle's, or another) closes it.
12. **The stop is process-wide and permanent**: `runtime/hero_net.h:193-199`, *"from this call, on every
    thread, a call of this layer that would wait answers ... `HERO_NET_STOPPED`"*. Under S7 every
    `receive` in the process then answers `stopped` for the rest of its life, **a reply already queued in a
    worker's reply mailbox included**, while S6's `send` has no `stopped` answer (the critic's finding 32).
    Question 2 is asked with this premise, by the in-flight request and its reply mailbox.
13. **The Windows arm** (the critic's finding 35): the runtime is on every link line and Windows has no
    pthreads (`tests/harness/suite_runtime.hero:230`); the queue's wait and its wake on the stop there
    are **written** by the compiler-engineer (route B) and the ffi-pragmatist (route A), and **owed** to
    the CI's legs or the box to run.

**Routes beside the proposal, each for a seat to price or to say why not** (the critic's findings 27, 28):

- **A0, today's tree, no new C and no spec token: Rebel's `ipc` channel by name between threads of one
  process.** `runtime/hero_ipc.h:5-55`: *"a local channel by name"* that *"IS A STREAM"*, every wait
  bounded and ending at the stop. **Run by the critic** (`p254-scratch/critic/p254/a0/main.hero`): task t3's
  exact shape, the main thread keeping the counts and four `threads.spawn` workers each connecting by
  name and exchanging 100 requests and replies, **5 of 5 runs plain and 3 of 3 under `--sanitize` print
  `red: 402`, `green: 400`, `blue: 398`, `largest: 402`, exit 0**. Not settled: a `poll` set serving the
  channels together, a keeper on a thread of its own, TSan; and a TLS connection still cannot cross. The
  most conservative route; the ffi-pragmatist prices its gateway shape beside A's.
- **design.md 7.13's own mailbox**, *"one queue per thread"*, and its **first rung, plain data
  parallelism**, any value copied at `spawn` and `wait` with no mailbox (panel 251 named both, *"not
  proposed"*, `docs/panel/251-...md:127-130`): the compiler-engineer prices them beside the proposal; the
  first needs a way for a worker to reach a queue it does not own, which is the aliasing question again
  (a question, not a premise).
- **Spelling B with names nothing declares** (panel 251's critic, *"a spelling nobody priced"*): the
  spec-warden and the ffi-pragmatist say whether such names exist and what they would cost.
- **Shared state by lock** (Go's `sync.Mutex`, Rust's `Arc<Mutex<T>>`, both named by the issue): refused by
  design.md §4.10 and 7.13's isolation, and not priced here.

**What the conservative route would have been (A)**, the library route as first briefed, no spec token:
a mailbox of `str`, `[u8]` and `i64` in a platform member (`runtime/platform/hero_mail.c`, its own
header), its handle's number crossing at `spawn` as `accept_number`'s does, bound by Rebel's `threads`;
and any stream of the layer, a TLS connection among them, handed whole by a number
(`hero_stream_number` / `hero_stream_of` beside `hero_stream.c`'s table, whose `ops->close` already runs
*"on the thread that leaves the stream last"*, `runtime/hero_stream.h:255-259`). Its handle would take
tag **15, the last free of four bits** (`runtime/platform/hero_handle_tag.h:81`). A program would
serialise every richer value into text. The seats price it enough that the synthesis can record it.

## 6. The machine, and what may run

- **Load**: `uptime` read **87.29** at 00:55 and **4.96** at 01:38 on 8 cores (`sysctl -n hw.ncpu`):
  other agents share this Mac. **A clock is read only on a still machine, and its `real` against `user +
  sys` is written beside it** (CLAUDE.md § Verification); a speed result rests on a load whose effect
  the machine's load cannot fake (§ 8's latency-bound shape), and a CPU-bound figure is reported with
  the load and the ratio, or not at all.
- **Docker did not answer** at 00:55 (`docker info`: *failed to connect to the docker API at
  unix:///Users/joseph/.docker/run/docker.sock*). Linux arm64 rows are **unrun** unless `docker info`
  answers when you ask; **never start Docker yourself**. Unrun rows are owed to the CI's legs.
- **Held awake**: `pmset -g assertions` named Amphetamine's `PreventUserIdleSystemSleep` at 00:55.
- **Toolchain**: `/usr/bin/clang`, Apple clang 21.0.0, arm64-apple-darwin27.2.0 (`clang --version`).
  `heroes build --sanitize` adds `-fsanitize=address,undefined` (`selfhost/cli/flags.hero:253-255`);
  **no `heroes` flag gives ThreadSanitizer**: build the program's C and the runtime by hand with
  `-O1 -g -fsanitize=thread`, as panel 251's critic did (`docs/panel/251-reports/critic-reports.md:307-309`).

## 7. Where each seat works, and the rules every seat obeys

- **Your copy** is a detached worktree of the frozen commit made by the coordinator, at
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/<seat>/` (`compiler-engineer`,
  `ffi-pragmatist`, `spec-warden`; the critic's is `critic`), each `git status` clean at `89f155330` when
  made (00:59; the spec-warden's at about 01:35). **No seat reads, builds or runs inside another seat's copy, `lane-mailbox`, or
  `rebel-round`'s worktree.** The historian needs none.
- **Build your own compiler in it** (the lane rules of 2026-11-21, `.claude/worktrees/rebel-scratch/coord/
  lane-rules.md`): `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build
  selfhost/main.hero -o heroes-next`, then `mv heroes-next heroes`, then `./heroes build
  selfhost/main.hero -o heroes-next` again; judge with that `heroes-next`. Measured in the coordinator's
  lane from 01:36 to 01:42 (`/usr/bin/time -p`, `build/p254-build.log`): stage 1 `real 12.44` `user 12.00`, stage
  2 `real 176.28` `user 129.51` `sys 36.08`, stage 3 `real 163.68` `user 119.91` `sys 33.07`; stage 3's
  `--version` second line `sources 6e891e681c14bca0`.
- **Every file you write stays inside the repository's root** (CLAUDE.md § Hard stops): in your copy, in
  an untracked folder of your own (`p254/`), never `/tmp` and never the session's scratchpad. **`export
  TMPDIR=<your copy>/build/tmp`** (made with `mkdir -p`) in every shell that runs clang or `heroes`.
- **One build or harness run of yours at a time**; a program that may not end is built and its binary run
  under `timeout`, its output to `/dev/null` or through `head -c` (`.claude/rules/verification.md` § A run
  that may not end); after a timeout, `pgrep -f` your binary and stop it.
- **No paid run** of any kind (no `claude -p`, no API call), but the spec-warden's refreshes its brief
  names. **No credential printed**: `gh auth status` if you must, never `gh auth token`. **No GitHub
  write, no push, no commit** (the coordinator commits the sitting), and no edit to `lane-p254` but your
  own report file.
- **Your time**: write your report as you go; at about three hours from your start, close what runs and
  write what is unrun as owed. A seat the watchdog stops is relaunched from its own report file.
- **A negative sentence is run or written as a question** (CLAUDE.md § RUN IT): *X cannot be done* names
  what was searched.

## 8. The gateway-shaped program, the measurement both compiling seats owe

**The shape**: one **owner** thread holds a cache (a map) and a SQLite connection (Rebel's
`sql/sqlite` binds `sqlite3.h`); **N connection threads** each serve connections handed to them by the
accepting thread, and ask the owner by message for every read and write of the state; the owner answers
each by a reply mailbox. **Correctness**: every request's answer right, the owner's final state equal to
the sum of the requests (one counter per key, incremented once per request, checked in SQLite at the
end), under ASan + UBSan (`--sanitize`) and under ThreadSanitizer (by hand, § 6), 20 runs plain and 5
under each sanitizer at least, every report counted. **Speed against one thread**, latency-bound so the
machine's load cannot make it: M clients each holding its connection open for a fixed idle time per
request (e.g. 20 ms of sleep in the client between bytes), so one thread serves them in about their sum
and N threads in about their maximum; report N, M, the requests, wall time each way, the load
(`uptime`) before and after, and `real` against `user + sys`.

**Written both ways**: (B) as the proposal spells it, built as far as the route can be built in your
copy, and where the compiler half cannot be built, its emitted C written by hand as the emitter would
write it, against a runtime half you built, **the emulation named as such**; (A) over a platform-layer
mailbox of text, built. The compiler-engineer owns (B)'s program, the ffi-pragmatist (A)'s and the TLS
hand-over in both.

## 9. The blind seat, run by the coordinator (`docs/panel/254-briefs/llm-ergonomist.md`)

26 paid sessions from the area `.claude/worktrees/p254-scratch/254-llm-ergonomist/` (an empty git
repository of its own; `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside it; `< /dev/null`; a free
preflight; each session capped at 3 USD and the run stopped before it would pass 20 USD in all), on
three tasks: t1 (four documents counted on threads, the record carried back; arms `now` and `m`), t2
(five programs at the mailbox's edges, predicted; arms `m` and `m0`), t3 (a keeper thread and four
workers exchanging values while they run; arm `m`), and one verdict session. **The runner starts only
after the spec-warden's report holds its registered predictions** (its brief, § FIRST).

## 10. What every report holds

Your agent definition's structure, plus: a **verdict on route B** (approve, object, or veto with the
program or measurement that compels it; a veto is a refusal, not a price, and since § 2 only a soundness
veto binds against the most robust resolution) and **on each row S1 to S14** (adopt as written, amend to
a stated text, or refuse, each with its measurement); **the exact text you would put in the spec** where
you amend a row; **your verdict on A as the conservative route**; every command with its output's count
lines; **predictions** that a later run can score, each with when it becomes checkable; and **every
premise of this brief you found false**, with the command that showed it.
