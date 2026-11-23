# Panel 256, the shared brief: one wait for everything a program waits on, and a send inside a generic function

Written 2026-11-22 from 21:20 by the sitting's coordinator, **repaired from 22:10 after the completeness
critic's first reading** (`docs/panel/256-reports/critic-briefs.md`, 29 findings; each repair names its
finding as *(critic N)*). **Read that report too**: its findings 5, 6, 9, 11 to 17 and 26 are measurements
a seat may build on, each with its command. **Every number, path and count below was
produced by a command run while this file was written, and the command is named beside it**; a sentence
that could not be run says *unrun* or is put as a question. Read this file whole, then your own brief,
then begin. **Your report is written as you go**, never only at the end, at
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p256/docs/panel/256-reports/<seat>.md`.

## 1. The sitting

- **The frozen commit**: `8aca3cc432a6007daaf416ecf3b1b8564472fa09` of `rebel-round`, in the lane
  `lane-p256` (`git -C .claude/worktrees/lane-p256 log -1 --format=%H`; `git status --short` printed 0
  lines). It holds panel 254's mailbox (`runtime/parts/mail.c`, 836 lines; `selfhost/check/thread_ops.hero`,
  245) and the repairs of defects 2692 and 2690 (`git log --oneline -12`: `6c324e660` merged lane-crossing,
  defect 2692; `b36db185f` merged lane-signals, feature 2690). **Nothing in the tree moves until the
  synthesis is written**; the coordinator's own copy is `.claude/worktrees/p256-scratch/coord/`.
- **Who convened it, and the author's word**: the coordinator of M-rebel-library, 2026-11-22, relaying the
  author: a **full panel** (both questions touch spec § 11 and the checker), seats differentiated by input,
  the completeness critic reading these briefs before any seat and the reports after; **the blind seat's
  paid run authorised by the author up to 20 USD** (2026-11-22, meant as *I authorise up to 20 dollars*),
  prepared by this sitting and **started by the coordinator of M-rebel-library, never by a seat**.
- **The rule that decides**: CLAUDE.md § Precedence, read at the frozen commit: robustness (rank 3) beats
  elegance, token cost, ergonomics, compiler size and speed; and the author's instruction of 2026-11-22
  committed at `691531908`, *"the most robust solution now always wins, a new form included and at the
  cost of the spec's size: Principle 0, the spec's budget (design.md §1.6) and the spec-warden's vetoes
  yield to it, and the ceiling is raised to fit"* (`git show 691531908 -- CLAUDE.md`). **A soundness veto
  still binds** (a program that corrupts memory, a race the sanitizers read). A budget or Principle 0
  objection is recorded as a cost, measured.
- **Which contract binds you** *(critic 1)*: the main checkout stands on `main` at `17c50eba6`, which does
  not hold `691531908`, and its `.claude/agents/spec-warden.md`, `ffi-pragmatist.md`, `CLAUDE.md` and
  `.claude/skills/panel/SKILL.md` differ from the frozen commit's (`git diff HEAD 8aca3cc43 --stat` there).
  Your session may have loaded `main`'s. **The frozen commit's files bind this sitting**: read your own
  agent file and CLAUDE.md § Precedence in your copy. The frozen `spec-warden.md` says at its vetoes:
  *"**Neither veto holds against the most robust resolution** (CLAUDE.md § Precedence, the author's
  instruction of 2026-11-22): there you report its cost, measured, and the ceiling is raised to fit it."*
- **The seats**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory,
  web-verified), and the llm-ergonomist (the blind seat: fresh `claude -p` sessions from an area of its own,
  prepared here, run later by the coordinator of M-rebel-library; `256-briefs/llm-ergonomist.md`).

## 2. What binds every seat

- **Your own copy**: a detached worktree of the frozen commit at
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/<seat>/` (made for you; `git -C
  <it> log -1 --format=%H` must print the frozen commit). **Build your own compiler there from the seed**,
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then stage 2, `./heroes build
  selfhost/main.hero -o heroes-next`: **the seed does not know a mailbox** (`grep -c mailbox seed/heroes.c`
  prints 0), so every probe of a thread or a mailbox needs stage 2. **The machine is loaded**: the
  coordinator's stage 2 took `real 327.02` against `user 215.08` + `sys 53.35` at a load near 28
  (`uptime`), a run that was waiting, so its duration is no measurement; expect minutes, not seconds. Its
  `--version` reads `heroes 0.6.0` / `sources 031aaabb358218fd`; yours must read the same.
- **Never** read, build or run in another seat's folder, in `lane-p256`, or in the main checkout; never
  write outside your copy and your report; never a file outside the repository's root (the scratch is
  inside it, under `.claude/worktrees/`, which git ignores).
- **No paid run**: no `claude -p` session, no API call, no cloud run of your own, **and the Windows box is
  not started by anyone in this sitting** *(critic 2)*: `.claude/rules/platforms.md` at the frozen commit
  lets a session power it on (`docs/platforms/windows/power-on.py`), and it is paid by the hour; this
  sitting's convener forbids a paid run started by the coordinator or a seat. Windows' halves are priced
  from the code and Microsoft's pages, marked *unrun*, and owed to the box or the CI's Windows leg. One worth running goes in
  your report with its size, for the coordinator. The spec-warden's `measure --refresh` is the one
  exception, bounded in its brief.
- **No credential printed**: `gh auth status` if you must know; never `gh auth token`, never a key's value
  (`${#ANTHROPIC_API_KEY}` prints its length). **No GitHub write**: `gh issue view` and `gh issue list`,
  read only.
- **A program that may not end** (a server, a loop, a reader of a pipe) is built with `heroes build` and
  its BINARY run under `timeout -s KILL <seconds>`, never `timeout heroes run`; its output goes to
  `/dev/null` or through `head -c 65536` into a file; after a timeout, `pgrep -f <your copy>/` and stop
  what stands (`.claude/rules/verification.md` § A run that may not end; a probe of 2026-10-06 wrote 146 GB).
- **Every claim in your report is a command's**, named beside it; a negative sentence says what was
  searched and where (CLAUDE.md § RUN IT). A route you think nobody listed is the most valuable thing you
  can bring: list it, and price it if you can.

## 3. Question 1: everything a program waits on, in one wait

**The three items, each read with `gh issue view <n> --repo heroes-lang/heroes --json title,body`
(read 2026-11-22 21:00; no comments on any):**

- **Feature 2693** (open, `kind:feature`, filed 2026-11-22): panel 254 *"left 'a mailbox in a `poll` set
  (chat's and notes' loops)' unpriced and unadopted"*; `poll.ledger`'s `poll/wake` stands `pending`, its
  note resting on spec § 13's old rule that a thread is handed one `i64`, which panel 254 removed. Its
  *Origin*: chat and notes serve every connection on one thread's `poll` loop; moving plain requests to
  worker threads *"needs that thread to wait on its sockets and on its mailbox at once.
  `m.receive(within_ms:)` waits on the mailbox alone and `poll.wait` on the set alone."* Its own question:
  `poll.add_waker(set) -> Waker?` rung by `poll.wake(w)` (`EVFILT_USER`, an `eventfd`, a loopback socket on
  Windows), *"or the language's own answer to panel 254's open question"*.
- **Feature 2660** (open, filed 2026-11-21): *"A server cannot stop gracefully past its own accept loop:
  the stop is process-wide and permanent, so nothing may wait after it."* Its three cases: a WebSocket's
  closing handshake (RFC 6455 § 7.1) after SIGTERM, which `websocket.close` cannot wait for because it
  fails `stopped`; a grace for work in flight whose handlers call a database or an API (Go's
  `Server.Shutdown(ctx)`); one server stopped among several, or a server and a client in one process. Its
  own sketch: a stop a program scopes, `os.stop_token()`, which a `poll.Set`, a `net.Listener` or a call
  takes. **The question as relayed to this sitting**: lane-mailbox's analysis said panel 254's R1 to R8 do
  not answer it (carried from the coordinator's question; that analysis is not in the tree).
- **The `ui` package's TH01**: *"a thread's message wakes the waiting user interface"*, verdict `must`,
  Rebel today `pending (ui/mailbox)`. It lives in a file git ignores,
  `.claude/worktrees/rebel-scratch/coord/ui-feature-catalogue.md` (its row at line 377, its section *3.
  Threads (TH01)* at lines 463-474), so its section is copied whole in § 3.4 below. The tracked row is
  `packages/rebel/0.1.0/files/ui.ledger:92-98`: `capability ui/mailbox`, `heroes pending`, Go
  `ui.QueueMain`, wx `wxThreadEvent, wxEvtHandler (QueueEvent, CallAfter)`, note *"until it lands, a
  thread's work is read by the program when a `timer` fires"*.

### 3.1 What waits today, and on what (each read at the frozen commit)

| what waits | where | on what |
|---|---|---|
| `m.receive(within_ms: n)` | `runtime/parts/mail.c` (836 lines) | a `pthread_cond_t` per queue (`mail.c:234-239`, `:456-473`), the queue's own lock; **nothing else can wake it but a send, a close or the stop** |
| `poll.wait(set, limit:)` | `packages/rebel/0.1.0/files/poll.hero:372`, the layer `runtime/platform/hero_poll.c` (1531 lines) | Darwin's and the BSDs' `kqueue`, Linux's `epoll`, any other POSIX and Windows a `poll`/`WSAPoll` over the set's array (`hero_poll.c:62-96`); entries: sockets, streams, files, timers, and since feature 2690 the counted signals (`add_signals`, `poll.hero:290`), **`unsupported` on Windows, "whose wait takes sockets alone"** (`poll.hero:286`) |
| a call of the layer (`net`'s `recv`, `accept`, `send`) | `runtime/platform/hero_net.c` (4650 lines) | a condition variable per waiting call, one poller thread per process (panel 248's R2, `hero_net.c:44-69`) |
| `ui.wait(@app, within:)` | `ui.hero:1647`, the layer `hero_ui_wait` (`runtime/hero_ui.h:331`) | the system's own loop: `MsgWaitForMultipleObjectsEx(0, NULL, ...)` on Win32 (`hero_ui_win32.c:2436`), `g_main_context_iteration(NULL, 1)` on GTK 4 (`hero_ui_gtk.c:2277`), `NSApp.nextEvent(matching:until:...)` on AppKit (`hero_ui_appkit.swift:1280`) |

**The stop, as it is**: `os.catch_stop()` (`os.hero:790`) and `os.ask_stop()` (`os.hero:807`) make *"every
call of the library that waits on the system ... fail `stopped`, on every thread"*, *"for the rest of the
process"*. In the layer: `hero_net_nudge_all` (`hero_net.c:897`) wakes every waiting call, tells the other
members (`hero_net_tell_hearers`), and since panel 254's R6 calls `hero_mail_hear_stop()` (`hero_net.c:904`);
`hero_poll_hear_stop` writes one byte down a pipe on POSIX and one datagram to a loopback socket on Windows
that nobody reads, so every set of the process stays woken (`hero_poll.c:37-42`). **The lock order, one
everywhere**: the layer's lock, then the queues' list lock, then a queue's lock, then the handle set's or
the thread table's (`mail.c:48-51`); a send hands a handle over under the queue's lock (`mail.c:424`,
reaching `hero_handle_hand_over` at `:748`), so a ring outside every runtime lock stands after
`hero_mail_drop` (`:433`) *(critic 8)*. **The
direction**: the layer calls the runtime (`hero_mail_hear_stop`); the runtime never calls the layer (panel
254's R6).

**What a mailbox may not do**: spec § 11, read at the frozen commit: *"a `(thread T)` or a `(mailbox T)`
in an `extern` signature, a callback's parameter or a group's record"* is a compile error. **So no package
can hand a mailbox to C today**, and no `poll.add_mailbox` can be written in Rebel without a change to
the language or a value that names a mailbox without being one (§ 3.3).

### 3.2 What the programs that exist do instead (`git grep`, at the frozen commit)

- **chat**: `examples-rebel/chat/server/readers.hero:11-15`: *"A `poll` set waits on sockets and a mailbox
  waits alone, and nothing joins the two ..., so a worker, after each hand-over, sends one datagram from a
  UDP socket of its own to the loop's doorbell, a UDP socket in the loop's set"*; the loop drains it at
  `chat/server/serving.hero:157-171` (`on_doorbell`). The hashing threads tell the loop the same way.
- **notes**: `examples-rebel/notes/http/workers.hero:11-16`: each worker and the loop *"ring each other on
  two loopback pairs of their own (`net.pair`)"*: one byte on `ring` after each ask wakes the loop, one byte
  on `stop` at the stop wakes the worker (`workers.hero:64-68`).
- `git grep -l -E 'doorbell|net\.pair\(' -- 'examples-rebel/*.hero'` outside `/demo/`: 5 files
  (`chat/server/readers.hero`, `chat/server/serving.hero`, `notes/http/server.hero`,
  `notes/realtime/realtime.hero`, `reach_web.hero`).
- **The stop**: `git grep -c -E 'os\.(catch_stop|ask_stop)\(' -- examples-rebel`: **41 sites in 31 files**;
  `os.count_signal(`: **1** site, `.interrupt` in a REPL (`examples-rebel/shell/interactive/repl.hero:35`);
  `poll.add_signals` **is called by no program**: its five mentions outside `poll`'s files are comments and
  ledger lines *(critic 3)*; `poll.make(`: **21** files; `ui.wait(`: 5 files; `(mailbox `: 148 lines in 49
  files (151 occurrences) *(critic 8)*.
- **Two premises in the tree that panel 254 made false** (read at the frozen commit, for the seat whose
  route touches them): `ui.hero:36-38`, *"An `App` is a value no thread can be handed (a thread takes an
  `i64` or a `str`)"*; `poll.ledger:145`, *"a set crosses to no thread, a thread being handed one `i64`
  (spec § 13)"*. Under spec § 11 today a handle crosses through `m.send(@v)` and is then the receiving
  thread's.
- **Two open blocking defects of the same family**, a wait that is not told of what a library already
  holds: **2688** (`websocket` keeps whole messages in its inbox that a drained socket never reports to
  `poll`) and **2675** (`nats`, `mqtt`, `amqp` leave a message read behind an awaited reply unparsed, so
  *"a poll loop never sees it"*), each read with `gh issue view`. **Whether "one wait" must also answer
  for bytes a library holds above the socket is a question this sitting should rule on or name**, not a
  premise.

### 3.3 The routes the coordinator found, as a proposal, never as premises

**The proposal (P1)**, stated in ten lines, for the seats to attack:

1. **A mailbox is a source a wait takes.** The language gives a mailbox a number that names its queue to C
   (`spec-q1.diff`: `m.number()`, an `i64`, a fifth operation after a dot, which *"keeps no copy and
   reaches no value, and once the queue has gone it names nothing"*), resolved by the runtime's own table,
   slot and life, as panel 246 made a socket cross as a number (`hero_net.c`'s `hero_net_number`). Rebel
   writes `poll.add_mailbox(set, m)` and `ui.add_mailbox(@app, m)`: an entry **told ready while the queue
   holds a value, is closed or answers `stopped`**, level-triggered as every entry is, rung by the send
   itself, so no ring is forgotten and none is lost. The runtime gains a watch the layer registers
   (layer to runtime, R6's direction), whose ring takes no lock of the layer's.
2. **A set joins the user interface's wait**: `ui.add_set(@app, set)`, the set's own descriptor watched by
   the system's loop (a `kqueue` or `epoll` descriptor is itself pollable), `ui.wait` answering an event of
   its key; Windows' set, a `WSAPoll` array with no handle, is the open half.
3. **The stop scoped to a server is that server's own mailbox**, in its set, closed or sent to; **graceful
   shutdown counts SIGTERM** (`os.count_signal(.terminate)` and `poll.add_signals`, both shipped and
   **used by no server in the tree**, *critic 3*) instead
   of catching the stop, so no other wait fails `stopped`; the process stop stays for the program that
   wants it; Windows' `add_signals` made to work by the stop's own loopback mechanism.
4. **Recorded as conservative**: the waker of 2693's question (`poll.add_waker`, `poll.wake`), library
   only, no spec token: a send and a ring, two acts, of which the second can be forgotten or put first.

**P1 item 1's ring is the runtime calling the layer's code through a pointer** *(critic 28)*: linkage is
kept (R6's measured reason, a `print(1)` program links no `hero_net` code), R6's words (*"the runtime never
calls the platform layer"*) are not. Whether R6 is read by its words or by its measurement is for the seats.

**Routes the critic found that nobody listed** *(critic 11, 12, 14)*, each to be priced or refused:
- **the mailbox hands out a descriptor**, as `fs/watch` already does: `packages/rebel/0.1.0/files/fs/watch.hero:298-330`,
  `add_to(set, w)`, a pipe the watch rings on Darwin and Linux and a loopback datagram socket on Windows,
  added with `poll.add_stream`, told readable while an event waits: a C-side queue joined to a set,
  level-triggered, **shipped**, with no callback from the runtime into the layer;
- **a forwarder per mailbox, written in Rebel today**: a thread that receives from `m`, sends on into a
  second mailbox and rings a doorbell the set holds, the library owning the ring's order; a thread per
  watched mailbox (the 257th `spawn` exits 134), each value queued twice, and for a `T` holding a handle a
  send of a letter's value inside a generic, **so it exists only under a Q2 rule that lets a generic hand a
  handle over**;
- **the one wait in the runtime** (Go's network poller under its scheduler), a mailbox an entry by
  construction, R6 no longer a constraint, at the price of where `hero_poll.c`'s 1,531 lines then live;
- **a process stop with a grace**: the stop fails only waits begun before it, or only after a deadline the
  program set.

**What the question holds** *(critic 15)*: beside the four waits of § 3.1 the tree waits on a child's end
(`process.wait`, `wait_or_stop`, `wait_or_signal`; `poll.ledger:154-155` refuses it in a set), `ipc.accept`,
`tls.accept`, `fs/watch.wait` (which joins a set) and a thread's end, `t.wait()`, which no set takes. **They
are named here and are outside the resolution unless a seat measures a program that needs one in the same
wait**; a thread that must be heard ending sends on a mailbox before it ends.

**Routes the coordinator listed and did not choose, for the seats to price or refuse**: the waker (4); a
**stop token** (2660's own sketch, `os.stop_token()`), which every waiting call takes; **the pump**, a
thread of the layer or of Rebel that waits on a set and sends its events into a mailbox, so the program's
one wait is `receive` (Erlang's ports in active mode deliver a socket's data as messages: a question for the
historian); **the inverse UI direction**, a set that takes the user interface's events; **a language-level
wait over several mailboxes** (Go's `select`); **a mailbox handed to C** (the boundary row of spec § 11
relaxed for one form, the queue's count retained by C). **What would have to be true for a route nobody
listed to exist** is a question every seat is asked.

### 3.4 TH01, copied whole from the git-ignored catalogue (lines 463-474, read 2026-11-22 21:05)

> A mailbox's `receive(within_ms:)` blocks its thread (spec § 11), and so does the system's loop inside
> `ui.wait`: the UI thread cannot block in both at once, so the sending thread must wake the loop. What
> each system offers to be woken from another thread: `PostMessageW` (Win32), `MainContext.wakeup` and
> `MainContext.invoke` (GLib), `CFRunLoopWakeUp(_:)` or `postEvent(_:atStart:)` (AppKit). The shape
> this suggests, as a question for the panel: a C call `hero_ui_wake(app_number)` that any thread may
> make, answering 0 or -1 `NOT_OPEN` and never touching a widget, which queues one `WAKE` (coalesced as
> the queue already coalesces); the face's `wait` then takes the mailbox's values with
> `receive(within_ms: 0)` when `WAKE` arrives. The app's number, an `i64`, crosses a thread where the
> `App` handle cannot (ui.hero's header). The alternative that needs no new call, waiting in short
> slices and asking the mailbox between them, costs a wake-up every slice and a slice's latency.

(Its last premise, *"crosses a thread where the `App` handle cannot"*, is the stale header of § 3.2.)

## 4. Question 2: a send inside a generic function

**Defect 2695** (open, `class:blocking`, `area:compiler`, filed 2026-11-22, read with `gh issue view`):
*"a generic body is checked once against its type parameters, and the walk that asks whether a type holds a
handle answers nothing for a parameter, so the rule of spec § 11 is decided for no instance"*; where to
look, `check/thread_ops.hero` (`marked`) and `check/handed.hero` (`.generic => continue`). Its own question:
*"a ruling on how a send whose element is a type parameter is judged: in each instance once its letters
are settled (the checker's instances, or the lowering's), or by a refusal of the letter itself, as
`operation_names` refuses a type parameter first because a thread or a mailbox may fill it"*.

### 4.1 The mechanism, read at the frozen commit

- `selfhost/check/handed.hero` (50 lines), `held_handle`: `.mailbox_ty | .function_ty | .failure | .generic
  | .error => continue` (line 44), so a letter holds no handle.
- `selfhost/check/thread_ops.hero` (245 lines): `marked` (line 175) asks `crossing.held_handle` of the
  mailbox's element and refuses an `@` that disagrees (`marker_mismatch`); `named` (line 57) refuses a
  `spawn` argument that holds a handle (`handle_into_spawn`, line 101-108) and asks `crosses` of the thread
  function's parameter and result (lines 99-100); `crosses` uses `emit/key_pointer.hero`'s
  `reaches_pointer`, whose letter arm is `continue` (`key_pointer.hero:53`).
- `selfhost/check/crossing.hero` (171 lines): `check` judges **only the thread and mailbox types WRITTEN in
  the source** (`a.types`, line 31: never-crosses, a unit mailbox, a queue holding itself); its own letter
  arm `continue` (line 107); `operation_names` refuses a function named for an operation whose first
  parameter is a letter, *"since either may fill it"* (lines 113-145) — the one letter rule there is.
- **Where instances are made**: the checker records each call's bound letters,
  `c.out.instantiations[span.end] @ resolved_args` (`check/walk.hero:1790`; also
  `function_value.hero:211`, `waiting_names.hero:126`); `selfhost/ir/mono.hero` (553 lines) copies a
  generic per declaration and letters after checking, and already refuses one PROGRAM at exit 1 after the
  checker: polymorphic recursion (its head comment, lines 20-28).

### 4.2 The shapes beside it, built and run (`256-briefs/probes/q2/`, `run.sh`, its output `coordinator-run.txt`)

Each probe checked with the coordinator's stage 2 (`sources 031aaabb358218fd`), and each one `check`
accepts built and run once under `timeout -s KILL 20`. **The class is wider than the defect's two lines**:

| probe | the shape | check | build | run |
|---|---|---|---|---|
| `p1_defect` | the defect's own: `m.send(@v)` in `hand<T>` | **1**, `marker_mismatch` *"`send` does not change this argument"* | | |
| `p2_relay_only` | `m.send(v)`, `v: T`, at `T = Boxed` (holds a `FILE *`) | **0** | 0 | 0 |
| `p3_spawn_generic` | `spawn(body, setup)`, `setup: S`, at `S = Boxed` | **0** | 0 | 0 |
| `p3b_spawn_generic_copy_kept` | the same, `main` then closing its own copy | 0 | 0 | **134**, *"a C handle reached the argument `f` of `fclose` at an address a call marked `consumes` or `transfers` ended"* |
| `c3_spawn_nongeneric` | the same spawn, no generic | **1**, `thread_crossing` *"holds a C handle, which crosses ... only through a mailbox"* | | |
| `p5_ptr_written_generic` | `fresh<T>` writes `(mailbox T)`, at `T = Pointed` (holds a `ptr`) | **0** | 0 | 0 |
| `p5c_inferred_nongeneric_send` | `main`, no generic, sends a `Pointed` into the mailbox `fresh` made, its type inferred and never written | **0** | 0 | 0 |
| `p4_ptr_generic` | `main` writes `(mailbox kinds.Pointed)` | **1**, `thread_crossing` *"reaches `ptr`, which never crosses"* | | |
| `p7_spawn_ptr_generic` | `spawn` of a `Pointed` through `start<S>` | **0** | 0 | 0 |
| `c7_spawn_ptr_nongeneric` | the same, no generic | **1**, `thread_crossing` | | |
| `p8_answer_ptr_generic` | a thread answering `Pointed` through `run<T>` | **0** | 0 | 0 |
| `p6b_unit_generic` | `(mailbox T)` at `T = ()` | **0** | **134**, the compiler's own panic *".must() on an error: none: no descriptor for this type at emit/threads.hero:51"* | |
| `c6_unit_nongeneric` | `(mailbox ())` written | **1**, `empty_mailbox` | | |
| `e1_partial_eq_generic` | `==` on two `T`s | **1**, `generic_equality` (panel 155's R2) | | |
| `e2_partial_key_generic` | a `partial` record as a map key through `index<T>` | **0** | 0 | **134**, *"a partial record has no structural equality"* |
| `e4_partial_key_nongeneric` | the same key, no generic | **1**, `ffi_partial_operation` | | |

**Read plainly**: a C handle through a generic is stopped at run time, an abort where a compile error is
owed (`p3b`'s abort is the consumed-address check; the critic's `k3d`, `main` keeping a copy the thread
never closes, is the holder check's witness, *"on a thread that does not hold it"*, exit 134, *critic 5*);
**a `ptr` crosses a thread through a generic with no check at any stage** (`p7` by `spawn`, `p8` as a
thread's answer, both exit 0); `p5` and `p5c` cross no thread (one thread sends and receives) and show the
written-type rule not applied, `p5c` by code with no generic in it *(critic 5)*; a unit mailbox through a
generic crashes the compiler. **`e2` is not new** *(critic 4)*: panel 155 measured that class
(`docs/panel/155-*.md:47-53`, *"check 0, build 0, run 134"*) and ruled its run-time abort a designed guard
(issue 599, panel 061's `hero_panic`), closing `==` on a letter alone; **whether the 2026-11-22 rule reopens
panel 155's ruling on a generic map key is a question, not a premise**. **Whether a `ptr` crossing corrupts memory** (a C object freed by one thread
and read by another) **is unrun**: the probes cross `nullptr`. That is the ffi-pragmatist's first question.

### 4.3 What the tree holds that a rule would reach (`git ls-files '*.hero'` and `awk`, 8,802 files)

Generic functions whose body sends, spawns, waits, receives or makes a mailbox
(`git ls-files '*.hero' | LC_ALL=C xargs awk '...'`, the command in `probes/q2/census-command.txt`):
**four**, `examples-rebel/certs/demo/stage.hero:137` `start<S>` (`spawn(body, setup)`, four calls in
`certs/demo/script.hero` and `days.hero`), `examples-rebel/iothub/demo/world.hero:244` `start_thread<S>`
(`spawn(body, setup)`, four calls in `world.hero:258-274`), `tests/golden/run/threads-handles-in-lists-maps-and-generic-functions.hero:13`
`relay<A>` (`into.send(value)`, two calls at `A = i64`), and
`tests/golden/run/fixedbugs-2682-ok-of-a-unit-a-thread-answered.hero:10` `run<T>` (`spawn(f, 1)`, `t.wait()`,
at `T = ()`). **Their instances, run by the critic** *(critic 6)*: `--emit-c` of `examples-rebel/certs` and
`examples-rebel/iothub`, `grep -c hero_cross_put_handle` 0 in both; `start<Setup>` ×4 and `start_thread<…>`
×3, their structs holding strings, arrays, numbers, an `Instant`, a `str` variant and a `HeroMail *`, no
handle and no `ptr` read (iothub's `Fault` unread); the goldens run at `i64` and `()`. So the four are live
and harmless today; **`gc` as worded refuses the golden `relay<A>`** (`into.send(value)` without `@`), `gr`
refuses all four, `gi` none. The tree declares
**852** generic functions in all (`git ls-files '*.hero' | LC_ALL=C xargs grep -c -E '^function
[a-z_0-9]+<'`, summed), most of them Rebel's.

### 4.4 The precedents in this repository (read each; the lines are `grep -n`'s)

- **Panel 155** (`docs/panel/155-the-hole-was-never-made-by-the-generic.md`), its **R2**: `==` on a bare
  type parameter refused IN THE BODY (`generic_equality`), *"the language's own written answer"*,
  design.md §4.12's *"No constraints. No `where`, no bounds. If an operation on `T` is needed, pass it as a
  parameter"*; and *"what the conservative resolution would have been"*: a call-site pass, priced at **284
  lines**, rejected *"against a soundness veto"* (a nesting give-up with no runtime guard behind it). Its
  **R3** left a float map key through a generic waiting.
- **Defect 056 and measurement 034** (`docs/measurements/034-the-generic-body-that-reads-the-outer-level.md`,
  100 lines): the generic body checked ONCE; two shapes of a repair, *"At the call site, from the recorded
  instantiation. Per `(decl, type parameter)`, record the obligations the body incurs ... That is panel 082
  R3's shape, never built"*, and *"After `mono`, over the monomorphised IR ... a second layer under design.md
  §1.7"*, which panel 160's compiler-engineer said it would veto without a costing. **Both unbuilt, both
  uncosted** at that date.
- **Panel 215's lock copies** (`selfhost/check/lock_copies.hero:306-332`, `to_a_letter`): a lock record
  handed to a bare letter `x: T lent` is judged at the CALL as a copy, *"measured: `apply<T>(x: T lent, ...)`
  ... handed C a lock by value, exit 0"*.
- **`sort` is "never a type parameter"** (spec § 11's built-ins sentence), the shipped refusal by letter.
- **design.md §4.12** (`docs/design.md:1982-2040`): *"Functions only, no constraints, always inferred,
  implemented by monomorphisation"*; Part 6's traits row (`docs/design.md:3639`) refuses a generic
  constrained to types that supply operations.

**And the obstacle panel 155 measured against the call-site route stands** *(critic 9)*: `instantiations`
(`selfhost/check/state.hero:86`, `{i64: [i64]}`, a span's end to its bound types) records no callee, and
`grep -rn "decl_of\|owner_decl\|decl_for\|node_decl" selfhost/check/` prints nothing: the node-to-declaration
map was **150 of panel 155's 284 lines**.

### 4.5 The proposal (P2) and its alternatives, each a spec diff (`256-briefs/arms/`)

`make_arms.py` builds every arm from the frozen spec by asserted edits (`python3 -I make_arms.py
<spec/heroes-spec.md>`, run 21:15, exit 0); hashes in `arms.sha256`; vendored counts by `heroes-next
measure <arm>` from the copy's root (the real counts are the spec-warden's):

| arm | what § 11 gains (one sentence after *"... a callback's parameter or a group's record."*) | vendored max |
|---|---|---|
| `now` | nothing: the frozen spec, sha256 `15804f3f...` | 9,761 |
| `gc` (**P2**) | the rules judged at each call on the types it gives the letters, and **a letter's value always sent `m.send(@v)`**, which empties it whatever the type holds | 9,812 (+51) |
| `gi` | judged at each call, **the `@` included**: a generic that sends a letter's value serves the types that hold a handle (`m.send(@v)`) or those that hold none (`m.send(v)`), refused at a call that gives it the other kind | 9,847 (+86) |
| `gr` | **refused in the body**, panel 155's shape: a letter's value is not sent, handed to `spawn` or answered by a thread, and a `(mailbox T)` or `(thread T)` names no letter | 9,824 (+63) |
| `q1` | Q1's `m.number()` (P1 item 1) and the UFCS sentence's fifth exception | 9,812 (+51) |

**The routes beside them**, for the seats: a written **bound** on a letter (Rust's `T: Send`, Swift's
`Sendable`; refused by design.md §4.12 and Part 6 as they stand, and the 2026-11-22 rule lets the most
robust form enter); **after `mono`** (measurement 034's second shape); the obligation **inferred and shown
in the signature by the formatter or `heroes doc`**; and whatever the class asks of the rules beyond
threads (§ 4.2's `e2`). **Whether the `@` of `gc` can mean "emptied" for a value that holds no handle,
what a read after it does, and what it costs the four functions of § 4.3, is unrun.** **And a route after `mono` that is not a new pass**
*(critic 14)*: the emitter already writes a crossing function per instance type, and for `Pointed` writes
`hero_cross_put_raw(b, p, sizeof(h_kinds_Pointed))` (`--emit-c` of `p7`; `selfhost/emit/crossing.hero:60,77`,
`.ptr | .cstr` copied raw), so it knows after `mono` that an instance holds a `ptr`: a refusal there, or a
run-time guard, is the guard panel 155's veto condition asks to stand behind any give-up.

### 4.6 Is `gi` what the frozen text already says? *(critic 16)*

Spec § 11 at the frozen commit: *"a handle, or a value holding one, crosses only through a mailbox, written
`m.send(@v)` ... Anything else is a compile error"*; nothing there exempts a value whose type a call gave a
letter. **Read as written, the frozen text refuses programs 3 and 5 of `blind/task-t2.md` and accepts program
2**, which is `gi`'s reading of those three; program 4 (`@` on an `i64`) it does not decide. CLAUDE.md § 12:
*"Spec beats compiler: the compiler has the bug."* **So every seat answers: is `gi` a clarification of § 11,
needing at most a sentence saying where the error is told, and only `gc` and `gr` changes to the language?**

### 4.7 Two defects the probes found, with no issue *(critic 18)*

`p6b`: the compiler aborts building a `(mailbox T)` at `T = ()` (`emit/threads.hero:51`, exit 134 of `heroes
build`); `p7` and `p8`: a `ptr` crosses a thread through a generic, checked nowhere. `gh issue list --state
all --search` for their words found neither. **Both are `blocking`** (*"a crash"*, *"a wrong one
accepted"*, `.claude/rules/verification.md` § Bounded discovery). This sitting files nothing on GitHub: their
drafts are written beside the decision's for the coordinator of M-rebel-library, and the resolution says
whether each is closed by it.

## 5. What each seat delivers (its brief says how)

- the compiler-engineer: Q2's mechanism built, every arm it can, with the census; Q1's language half
  (`m.number()`, the runtime's table and watch);
- the ffi-pragmatist: Q1's C built and stressed on this Mac and in the Linux container; Q2's `ptr`
  crossing under the sanitizers;
- the spec-warden: every arm priced, the texts judged, **the blind run's predictions registered**;
- the historian: who did each route before, and what happened;
- the critic: what is missing, twice;
- the blind seat: the arms read by fresh sessions, run later.

Every seat ends its report with its verdict structure (its agent file), and **one line per question:
which route you would adopt, and the one measurement that would change your mind.**
