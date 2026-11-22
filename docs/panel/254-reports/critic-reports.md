# Panel 254, the completeness critic's second reading: the reports, before the synthesis

Written 2026-11-22 from 03:40 to 04:01 (`date`) by the completeness critic, 24 findings (1 to 23 and 6b). No verdict on the route: what is
missing, which contradictions between seats a command settles (settled here where it could be), and
what the synthesis must settle. Read first: `CLAUDE.md` (§ RUN IT, § Hard stops, § Precedence as
amended at `691531908`), `254-briefs/critic.md`, `00-shared.md`, `arms/spec-m.diff`, my first reading
`critic-briefs.md`, and the four reports `spec-warden.md`, `historian.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, each whole. The blind seat's area was read only (no `run.sh`, no `claude -p`,
nothing written there).

Commands ran in my copy `.claude/worktrees/p254-scratch/critic/` (detached at `89f155330`, my files
under its untracked `p254/`, `TMPDIR=<copy>/build/tmp`); a seat's program was re-run only from a copy of
its files laid into my `p254/` (its sha256 compared with the seat's first), never inside its folder.
Load at 03:43: `uptime` 8.93 14.08 24.47 on 8 cores, so no figure below is a speed claim. Each finding
is tagged **confirmed** (re-run, the seat's result reproduced), **contradiction** (two seats, and what
settles it), **missing** (a case, route or question no report covers), **false** (a sentence of a
report or brief, with the right value), or **unchecked** (and why).

## Findings

### A. S11: the handle hand-over and the ffi-pragmatist's veto (the coordinator's point 2)

**1. confirmed: the copy-then-send case, without and with the repair.** Copied
`ffi-pragmatist/p254/c/copy_then_send.c` and `copy_then_send_held.c` into `p254/s11/` (sha256 equal to
the seat's, `8b7ccf9c80c903e9...` and `940d0bbe9411a7da...`); built my copy's frozen `runtime/runtime.c`
(`rt-base.o`) and a tree `p254/s11/rt/` that is my frozen `runtime/` with only `parts/alloc.c` replaced
by the seat's `p254/q11/rt/parts/alloc.c` (`diff` of the two `alloc.c`: 49 lines added, 0 removed; the
seat's `q11/rt/` also carries route A's files, which I did not take). `clang -O1 -g`, `-lsqlite3`, ten
runs each, `timeout 60`:

| binary | mode | exits (10 runs) |
|---|---|---|
| frozen runtime | `copy` (the sender uses a copy kept from before `m.send(@db)`) | `0 139 139 139 138 139 139 139 138 139`: **9 of 10 killed**, 1 silent |
| frozen runtime | `cell` (the emptied cell) | 134 x 10, *"a dead C handle reached sqlite3_exec: a call marked `consumes` or `transfers` ended ..."* |
| holder check | `copy` | 134 x 10, *"a C handle reached sqlite3_exec on a thread that does not hold its life: it was sent to another thread, and this is a copy kept from before the send"* |
| holder check | `cell` | 134 x 10, the dead-value message |
| holder check | `moved` (the receiver alone) | 0 x 10 |

The seat's 8 of 10 killed reads 9 of 10 here; its 10 of 10 stopped and 10 of 10 clean reproduce
exactly. **The veto's measurement stands**: S11 as written, and the compiler-engineer's `handles.c`
emission, let one SQLite connection reach C on two threads. And the cell's message still names
*"`consumes` or `transfers`"* for a value a send emptied, the compiler-engineer's blocking-class
message (its Q2.5), reproduced on the frozen runtime.

**2. The repair attacked at the shapes beside it** (`p254/s11/held_shapes.c`, the same emulation as the
seat's: `hero_handle_acquired_at`, `hero_handle_alive_at` before each use, a send =
`hero_handle_hand_over` + the cell emptied, a receive = `hero_handle_take_over`; built on the holder
runtime; each shape run 3 times, every run the same):

| shape | what it emulates | result | reading |
|---|---|---|---|
| `spawn` | `spawn(f, @h)`, hand-over before the thread starts, take-over in its thunk; the spawner then uses a copy | **134**, stopped by name | **covered**, if the emitter calls the pair at `spawn` |
| `spawnforgot` | the thunk does not take over | 134, the spawned thread itself stopped | an emitter that forgets the take-over is **loud**, never silent |
| `wait` | a worker acquires, answers the handle; the waiter takes over | 0, both used it | **covered**, if `wait` takes over |
| `waitforgot` | the waiter does not take over | 134 | loud |
| `deadthread` | A acquires and is joined; B, a new thread, uses the handle with no receive | `pthread_equal(A, B) = 1` 3 of 3, and **134**, stopped | the repair's runtime number (`atomic_fetch_add` on a 64-bit counter, `_Thread_local`) **does not inherit `hero_ipc`'s reuse**: Darwin gave B A's `pthread_t` and the check still told them apart |
| `sendback` | A keeps a copy, sends `@h` to B, B uses it and sends it back, A takes over, then uses **the copy kept from before the first send** | **0, 3 of 3: not stopped** | memory-sound (one thread at a time), but **the seat's proposed sentence promises more than its check does**: *"A copy of it kept from before then is not the receiving thread's: reaching C through it aborts"* reads as aborting here, and nothing aborts |
| `record` | a value holding two handles, the walk handing over only one; the sender keeps the other in a list | **0, not stopped** | the check is exactly as complete as the per-type walk that calls it: a handle the walk misses is unprotected **in silence** |

So: **spawn and wait are covered by the same two calls** (unbuilt in the emitter; the seat says so,
its § Unrun); **a handle in a record or a list is covered only if one walk visits every handle a value
holds, on both sides** (missing on the receiving side is loud, missing on the sending side leaves the
in-transit window open: an inference from the code, the race not run); **a handle sent back makes every
old copy on the original thread valid again**, which is sound and is not what the sentence says; **the
thread-number reuse is closed** for the new check. And **the message is false in three of the four
loud shapes**: `deadthread`, `spawnforgot` and `waitforgot` are told *"it was sent to another thread, and
this is a copy kept from before the send"* when nothing was sent or no copy was kept (CLAUDE.md § 8,
blocking-class by the compiler-engineer's own reading of the two messages it found). The text owed is
by thread, not by copy: *"reaching C through it on a thread that does not hold it aborts, naming the
call"*.

**3. confirmed, and adjacent: `hero_ipc`'s lock holder is fooled by a reused `pthread_t`.** Copied
`ffi-pragmatist/p254/c/ipclock_alive.c` (lock name changed to `p254-critic-alive`), linked against my
frozen `hero_ipc.o hero_stream.o hero_stream_net.o hero_time.o hero_net.o hero_poll.o runtime.o`; three
runs of each mode: `joined: release_on_B=0 kind=0 same_pthread_t=1` 3 of 3, `alive: release_on_B=-1
kind=14 same_pthread_t=0` 3 of 3. The holder test is `pthread_equal(slot->holder, pthread_self())`
(`runtime/platform/hero_ipc.c:2897-2903`, read), and `hero_ipc_held_here` (`:2907-2916`) asks it at
every acquire, so **a new thread can also be told it already holds a lock its dead predecessor took**
(read, not run). Not a memory fault (the seat: `flock` is the open description's); a defect of today's
tree, **unfiled**: the coordinator owes it an `adjacent` defect issue, with this program and the
repair's own answer (a runtime thread number, never a `pthread_t`; on Windows `GetCurrentThreadId()` is
reused too, read at `:2899`).

**4. confirmed, as far as two cases reach: the holder check breaks no handle-and-thread program I ran.**
`HEROES_RUNTIME=<copy>/p254/s11/rt lane-p254/heroes-next build` of two goldens that hold handles on
threads, and the same build on the frozen runtime: `fixedbugs-two-threads-over-one-allocator-give-back-their-own-handles`
and `the-platform-layer-hands-a-connection-to-a-thread-as-a-number`, each exit 0 both ways, stdout equal
(`diff -q`), the first equal to its `.expected`; `strings` finds the holder message in the two held
binaries and not in the two frozen ones, so the held runtime was the one linked. **And the `runtime`
suite over it**: `p254/proto-held/`, a root of symlinks to my copy whose `runtime` is `p254/s11/rt/`
(the compiler-engineer's method, which read its prototype's part, 9 and 1), `timeout 500
heroes-next run tests/harness/main.hero -- heroes-next runtime`: **`runtime: 10 passed, 0 failed`**,
exit 0 (`p254/suite-runtime-held.out`). The patch's two new objects are `_Atomic` and `_Thread_local`,
so the suite's rule 3 passes them by their words; the run shows nothing else in it trips the suite.
**Unchecked**: the full net on the holder runtime (56 files under `tests/golden/run/` name
`hero_thread_spawn`, `git grep -l ... | wc -l`; not run for time). The seat ran neither either.

**5. missing: two orders the repair owes and no report states** (read in the seat's `alloc.c`, not
run). (a) **With the compiler-engineer's and the historian's S6** (*"a send that fails `closed` leaves
`v` as it was"*), the hand-over must be made only once the send has succeeded: the prototype's
`hero_handle_hand_over` is a separate call that marks the life `HERO_HELD_IN_TRANSIT`, so an emitter
that calls it before the queue answers `closed` leaves the sender holding a `v` that its own next C
call refuses (the holder is no thread). The compiler-engineer built the cell emptied on success only;
nobody built the two together. (b) `hero_handle_retained_at`'s branch for a second reference to a live
address (`hero_handle_set[i].n++`, the seat's `alloc.c:1427`) moves no holder, so a `retains` result
taken on a thread that does not hold the life is refused at its first use. Sound (the safe
direction), but unmeasured and unsaid.

### B. S7 and S12: the stop against a drain (the coordinator's point 1)

**6. contradiction, checked: the drain strands nothing and tells wrong answers where a reply mailbox
is reused; the stop-wins strands and told none.** Copied the compiler-engineer's three built binaries
`gateway-m0`, `-m1`, `-m2` and `gateway.c` into `p254/s7/` and ran each with N 8, M 32, R 200, no pause,
the stop at 20 ms, every combination of its two switches (`argv[8]` FRESH, a reply mailbox per request;
`argv[9]` FORGET, the owner's queue never closed), three runs each, `timeout 20` (`p254/s7/runs.txt`;
`pgrep` empty after):

| behaviour | reply mailbox | served | applied (of 6,400) | applied but told an error | **told a count never applied** | told another's count | forgotten close |
|---|---|---|---|---|---|---|---|
| 0, stop wins (S7) | reused | 8 of 32, 6 of 6 runs | 724 to 1,182 | 0, 0, 2, 2, 2, 0 | **0 in 6 runs** | 0 | ends (3 s, the clients' own wait) |
| 0 | fresh | 8 of 32 | 142 to 819 | 0 to 6 | 0 | 0 | ends |
| 1, drain | reused | 32 of 32 | 523 to 1,379 | 1 to 7 | **1, 1, 4, 2, 7, 6: 6 of 6 runs** | 1 (once), a count told twice once | ends |
| 1 | fresh | 32 of 32 | 200 to 1,517 | 0 to 4 | 0 | 0 | ends |
| 2, the stop never ends a mailbox's wait | either | 32 of 32 | **6,400** | 0 | 0 | 0 | **hangs: 6 of 6 killed at 20 s, no line** |

So the compiler-engineer's table is right as far as it goes, and three things it does not say change
how the synthesis must read it. **(i)** The drain's *"32 of 32 served"* means every queued connection
reached a worker; **4,555 to 5,549 of the 6,400 requests per run were then told an error** (the seat's three
`stopfresh.txt` runs, 5,177 to 5,549, and three of my first loop, 4,555 to 5,523, whose lines were
printed to the terminal and not kept: that loop ran under zsh, which split no argument, so it ran only
FRESH 0 or 1 with FORGET 0 whatever its labels said, and the table above is the second loop's), since every
reply wait on an empty reply mailbox answers `stopped` at once. **(ii)** **Under the drain a reused
reply mailbox tells a client a count for a request the owner never applied, in 6 of 6 runs**, and the
stop-wins told none in its 6 (the stale reply is read by the next request once the stop no longer
overtakes it); the seat's report says so for a reuse *"cascade"* (`stopmodes*.txt`, 0 to 24,473) and
its S7 text names it (*"a value sent after a receive stopped waiting is the next receive's"*), but its
table's *"client told another's count: 0"* is the fresh-mailbox column only. A silent wrong value is
`blocking` by `.claude/rules/verification.md` § Bounded discovery; under the drain it rests on the
program's discipline (a reply mailbox per request), which no sentence can enforce. **(iii)**
**Behaviour 2 is the only exact one** (6,400 applied, every client told its count, 0 errors) and the
only one that hangs on a forgotten close. Neither the drain nor the stop-wins is better on every axis;
the choice is *stranded handles told loudly and clients left waiting* (0) against *every queued value
delivered and a stale reply read silently on reuse* (1). **Two routes nobody measured**: a mailbox that,
once it has answered `stopped`, answers `stopped` for ever (no stale read, and still a drain of what
was queued before), and behaviour 2 with a deadlock report (Go's *"all goroutines are asleep"*: every
thread of the program waiting in `receive(-1)` or `wait` aborts, naming them), which would turn the
hang into a loud end; each was **a question, unbuilt**, when this finding was written, and the first
is measured in finding 6b.

**6b. measured: a route nobody listed keeps the drain and removes the stale read.** In
`p254/s7/m3/`, copies of the compiler-engineer's `mail.c`, `mail.h`, `mailrt.c` and `gateway.c`, three
lines added to `mail.c` (asserted to match once): a field `gave_stop`, and **behaviour 3**, a drain
whose mailbox, once it has answered `stopped`, answers `stopped` for ever (a value arriving after that
is dropped with the queue, never read). Built with `clang -O2 -DHERO_MAIL_STOP=<1|3>` against my
frozen runtime (`gw-1`, `gw-3`; `cmp`: they differ). Same arguments as finding 6, three runs per case
(`runs-m3.txt`, `served-m3.txt`):

| behaviour | reply mailbox | told a count never applied | told another's count | applied but told an error | served |
|---|---|---|---|---|---|
| 1, drain (rebuilt from the seat's source) | reused | **4, 8, 4, 0, 3, 4** | 3 (once), a count told twice twice | 0 to 8 | 32 of 32 |
| 1 | fresh | 0 | 0 | 0, 2, **674**, 5, 5, 0 | 32 of 32 |
| **3, drain, then stopped for ever** | reused | **0 in 6 runs** | **0** | 0 to 39 | 32 of 32 (3 of 3) |
| 3 | fresh | 0 in 6 runs | 0 | 0 to 6 | 32 of 32 |

So **behaviour 3 strands no queued connection (as 1) and told no wrong count with a reused reply
mailbox (as 0)**, in 12 runs, forgotten close or not (each ended, `pgrep` empty). What it keeps of
both: *"applied but told an error"* stands in every behaviour that ends a wait at a stop (0, 1 and 3;
674 in one drain run with fresh mailboxes), which is the unknown outcome of any request in flight at a
stop, not a false value, and the text should say a `stopped` receive does not say whether the request
was applied. **Twelve plain runs, then six under TSan and six under ASan+UBSan** (`gw-3-thre`, `gw-3-addr`, `-O1 -g
-fsanitize=...`, the queue and the gateway both instrumented; FRESH 0 and 1, three each;
`p254/s7/m3/san-m3.txt`): **0 reports, 32 of 32 served, 0 told a count never applied, 0 told another's
count, exit 0, 12 of 12** (no TSan control of mine; the compiler-engineer's and the ffi-pragmatist's
`control_race.c` show this Mac's TSan reads a race). An emulation on one Mac: a route for the
compiler-engineer to measure as it measured 0 to 2 (its stress topologies, Linux), not a result; its spec text would read *"...;
`stopped` once a stop is asked and it is empty, and from then on"*. The timeout hazard (a reply arriving
after a `timeout`, read by the next request, the compiler-engineer's 72 and 76) is untouched by any
behaviour of the stop.
One cost it adds, read and not run: a handle sent into a mailbox that has already answered `stopped`
is never received, so it is told at `main`'s end as one nobody consumed; the consistent text makes such
a send fail (`stopped`, `v` left as it was, as S6's `closed`), which the emulation did not build.

**7. settled by reading: the stop's wake is not called from a signal handler.** The compiler-engineer's
one line sits at the end of `hero_net_nudge_all` (`runtime/platform/hero_net.c:877-882`, *"Under the
lock"*), which is called at `:1232`, `:1387` and `:1548` on ordinary threads; the handler itself uses
only `write`, `sigaction` and `kill` (`:101`). So `hero_mail_hear_stop` taking the queues' list lock is
not a signal-safety hazard. The lock order it makes, **the layer's lock, then the queues' list lock,
then a queue's lock**, is safe only while no queue operation ever takes the layer's lock: true of the
prototype (it never calls the layer) and **a rule the landing must keep, written nowhere** (inference
from the code, not a run). The layer already has a hearers' list for its members
(`hero_net_tell_hearers`, `:874`), which the runtime cannot use without calling the layer: the
seat's direct call is the only shape that keeps the direction.

**8. settled among three seats: the order of `closed` and `stopped` under the drain.** The
spec-warden's W-i (*"`stopped`, before all else"*) orders a closed, empty mailbox under a stop; it was
written for the stop-wins text, and it reads behaviour 0. The compiler-engineer's drain text (*"on an
empty mailbox it fails `closed` once it is closed, `stopped` once a stop is asked ..., and `timeout`
else"*) and the ffi-pragmatist's (*"closed and empty is `closed`"*) agree with each other: `closed`
first. Nothing measured decides between W-i and that order except finding 6's table; a closed, empty
mailbox under a stop is not a shape either gateway reached (the seat's `stopfresh.txt` lines read
`closed 0` in all nine).

### C. W-j against W-j' (the coordinator's point 3)

**9. W-j' is checkable only as a rule that depends on a call elsewhere; a third route, local, was not
listed.** Probes on the frozen stage 3 (`p254/wj/`):

- `at`: `function serve(@b: Block)` whose body calls `free(b)`, called `serve(@b)`: **builds, prints
  `served`, exit 0**. An `@` parameter that ends the handle is today's form.
- `val`: the same `serve` handed as a value: **`error[mutable_parameter_as_value]`**, *"no function
  type carries an `@` (§ 9), so `serve` is only called by name"*.
- `dual`: `function serve(b: Block)` (no `@`) calling `free(b)`: **`error[consumed_borrowed_handle]` at
  the body's own line**, its note: *"Write the parameter `@b` in the signature of `serve` and `@` before
  its argument at every call of `serve`, which says on both lines that the value does not survive"*.

Today the body of a function is judged once, by its own signature (spec § 9 and § 13, *"passing one the
function borrowed is an error: mark the parameter `@`"*). **W-j'** (*"a spawned function's parameter owns
what `spawn(f, @x)` handed it"*) makes `free(b)` in `dual` legal or not according to whether some
`spawn` elsewhere names `serve`, and a direct call `serve(b)` of the same function then either ends the
caller's `b` with no `@` at the call (what § 13 refuses today) or must be refused by a rule read off
another file. That is checkable (the compiler sees the whole program), but **non-local**, the class the
llm-ergonomist's veto exists for, and that seat has not run; the compiler-engineer's *"about 20 lines,
keyed on the call being `spawn`'s"* does not say which of the two it does with the direct call.
**W-j** costs a mailbox of one per hand-over (a reader writes `m: (mailbox Conn) = mailbox()`,
`spawn(serve, m)`, `m.send(@c)`), and breaks nothing measured. **The route nobody listed, W-j''**:
`spawn(serve, @c)` with `function serve(@c: Conn)`, spawn naming its function as a call names it (`@`
in the signature and at the argument, as `serve(@c)` is written today); the body is judged by today's
rule, locally, and a direct call is today's `serve(@c)`. What it would cost: `spawn`'s first argument
read as a function's name where its parameter is `@` (§ 9 says no function value carries one), and a
sentence for what the thread's cell holds when `serve` returns (a handle still in it is told at
`main`'s end by the set, finding 2's `spawnforgot` shape); **unpriced, a question for the
compiler-engineer**. The ffi-pragmatist is indifferent among the three provided `spawn` and `wait`
move the holder (its S11 cell), which finding 2 shows the same two calls do.

### D. Where the queue lives (the coordinator's point 4)

**10. no contradiction left between the two seats; one sentence the proposal lacks.** The two seats agree
in their reports: the ffi-pragmatist withdrew its own first recommendation (a platform member) for the
compiler-engineer's shape (*"his keeps the dependency's direction for every program, and I take it"*).
Their figures agree where they overlap: a `print` program **310,832** bytes (compiler-engineer) and
**310,856** (ffi-pragmatist), both with **0** `hero_net` symbols; `runtime.o` linked whole into every
program (ffi-pragmatist, `nm`: `_hero_thread_spawn` in a `print` program). **Neither shape makes a
program that names no mailbox link `hero_net.o`**: under the compiler-engineer's, the queue is the
runtime's (+4,080 bytes, +25 symbols, every program, measured by that seat) and the layer calls it;
under route A's member, nothing is linked until a program names it. What the brief's question 7 did not
ask: under the compiler-engineer's shape a program that links no layer member can never be stopped, so
its `receive` never answers `stopped`, which is the layer's own rule (a stop is asked through the
layer) and is said nowhere in the proposal's text. **Not re-measured here** (the figures are the two
seats'; the 24 bytes between their `print` binaries are two builds of one source, not examined).

### E. The cycle, W-f and the gate (the coordinator's point 5)

**11. W-f is decidable, and at declarations; its walk misses one constructor.** Generics are on
functions only (spec line 318, *"Generics: on functions only"*), compiled by monomorphisation (design.md
`:1984`), and polymorphic recursion is refused (probe `p254/wf/`, `grow<T>` calling `grow([x], ...)`:
**`heroes check` exit 0, `heroes build` exit 1 `error[polymorphic_recursion]`**), so a program has
finitely many concrete types. A cycle of queues needs a mailbox type that is a part of its own element
type, which no type built of `[...]`, `{...}`, `?` and `(mailbox ...)` alone can be (it would be
infinite): it passes through a record's or a variant's declaration, and records are not generic. So
**the cycle half of W-f is decided at declarations, in the checker**, with no instantiation walk; the
`ptr`/`cstr` half (S10) needs the type arguments the checker infers at each call of a generic, which it
has. And today's precedent warns against putting it in the IR: a refusal only `build` makes is one
`heroes check` passes (the probe above). **Missing from W-f's text**: *"in a field, an element, a
payload or another mailbox's `T`"* names no `(thread T)`. A `record R { back: (mailbox (thread R)) }`
puts a thread's handle in a queue whose last copy the thread's answer holds; S3's gate tells it when
`main` returns (a thread nobody waits for), so it is loud at the end and **silent while a server runs**,
the per-occurrence leak W-f exists to close (inference from the types, unbuilt). The walk owes a
thread's answer type.

**12. missing: what W-f refuses that a program of the address-passing shape needs, and the route that
keeps it.** The hub W-f names can split (a `Command` carrying `(mailbox Event)`); a **network of peers
whose message carries another peer's address of the same message type** (Erlang's pids in messages,
a ring set up by `next: (mailbox Msg)` sent on the same mailbox) cannot be split, since the address of
a peer that accepts `Msg` must travel inside `Msg`. The spec-warden's L4 predicts 0 of the four
applications refused; no seat built or searched a program of the peer shape (a question, not a
premise). **The route the historian named as a known remedy and no seat priced: split ends** (Rust's
`mpsc`; the historian's reading in Q7, *"Rust separates the ends and discards the queue when the
reading end goes"*, on its Q5 pages): a copyable send end and a receive end that is a handle; the receive end's close or end discards
the queued values, which breaks every cycle through send ends, and a forgotten receive end is told at
`main`'s end as a handle (251's `m0` telling). It is not cycle-free either: a receive end sent into its
own queue is a handle nobody can reach, told at `main`'s end and silent while a server runs, so a
narrower refusal (no receive end inside its own element type) would still be owed (inference from the
types, unbuilt). Its price is a third
type constructor (one `Ty` case is 289 `match` sites, each further case one arm more at each,
the compiler-engineer's Q1.1) and spec tokens nobody measured. The compiler-engineer's *"the handle
life does not avoid the count"* is true of the queue object's memory and silent on the leak, which split
ends close.

### F. The false premises the seats reported (the coordinator's point 6)

**13. the `runtime` suite at base: stale, and its issue open.** The compiler-engineer read `runtime: 10
passed, 0 failed` at `89f155330`. `gh issue view 2588` (read-only, 2026-11-22): **OPEN**, `class:blocking`,
filed at `3a5ee2691` with `8 passed, 2 failed`. `git log --oneline 3a5ee2691..89f155330`:
`ee336f2f4 M-rebel-library step 54a: the runtime suite green on the platform layer (defect 2588)`,
merged by `06a2ecfbb`, and `804b72763 step 87a: the runtime suite green again`. So the premise was
true when the issue was filed and false at the frozen commit, and the issue awaits its round's close
(nothing for this sitting; the coordinator may say so on the issue when the round closes).

**14. the 256-thread panic: confirmed by reading, run by the compiler-engineer.**
`runtime/parts/spawn.c:520-523`: `hero_panic("too many threads running at once — this runtime holds
256, ...")`, `HERO_SPAWN_SLOTS 256` (`:109`); the compiler-engineer's `p254/q25/table.c` ran 257 threads
to exit 134. The spec-warden and the historian adopted S1 as written; the compiler-engineer's and the
ffi-pragmatist's S1 texts both cover the table. Settled by the run.

**15. the poll set: false on Darwin, unrun elsewhere, and question 8 left unpriced by every seat.**
`runtime/platform/hero_tls.c:15-26` (read): Darwin's TLS arm makes its own socket on Network.framework's
queue, so the stream stands on no descriptor; Windows and Linux run TLS over a socket stream, which
`hero_poll.h` takes (read, not run here, as the ffi-pragmatist says). So the premise is false on Darwin
for TLS (the ffi-pragmatist's `HERO_POLL_UNSUPPORTED`, kind 5) and for an `ipc` channel everywhere it
stands on the member's table. **What no seat priced: a mailbox in a `poll` set** (`00-shared.md` § 5
question 8). Chat's and notes' loops wait with `poll` on Linux and Windows; under every route
measured they must either give each connection a thread or proxy a mailbox through a socket. The
ffi-pragmatist's *"for a TLS server on Darwin the question does not arise"* is true of TLS on Darwin
and does not answer the plain-socket loops.

**16. the IPC lock's reused `pthread_t`**: finding 3 (reproduced 3 of 3); owed as an `adjacent` defect.

### G. The blind seat not having run (the coordinator's point 7)

**17. the blind run, when it runs, measures the text of 02:04, and one registered row inverts under the
amendment three seats want.** Read only: the area's `arms/` hold `spec-m.md`, `spec-m0.md`,
`spec-now.md` at the sha256 of `arms/arms.sha256` (`b008c307...`, `1dc1446d...`, `ffd687d6...`); `run.log`
ends *"preflight only: stopped before the key is read; no paid session run"* (02:31:31);
`warden-at-start.md` sha256 `fd1418d769bd62b2...`, as `run.log` records, and its registered predictions
section is identical to the one in `spec-warden.md` now (`diff` of the two sections empty). So W1 to
W11 are registered and unscored, D1 to D3 unmeasured, the llm-ergonomist's veto on non-local constructs
unexercised. Three consequences for the resolution: **(a)** t2's registered row for program 3 is
*`receive: stopped`* (`llm-ergonomist.md:107`), the stop-wins answer; under the drain the compiler-engineer
and the ffi-pragmatist adopt, the same program prints **`got queued`**, so a run on arm `m` grades a
reader right for reading a rule the resolution would not adopt, and the spec-warden's W5 (p3 4 of 5)
scores that rule. **(b)** Arm `m` holds none of the amendments the seats measured (S1's table, S6's
*"left as it was"*, S11's holder, W-a to W-j): the run measures the proposal, not the resolution.
**(c)** W-j', if adopted, is exactly the non-local shape the blind seat exists to test, and W-h's
divergence (`m.send(j)` against `send(m, j)`) is a locality question too. **What the record must say**:
the resolution is provisional on the blind seat as well as on the author; the registered run, when the
author's word starts it, scores W1 to W11 on the arms as registered (their hashes above), and **a fourth
arm, the adopted text, owes its own registered predictions before any session of it runs** (else a
reading of the adopted text is one nobody predicted); program 3's row is re-registered for that arm.

### H. Claims asserted and not measured, and smaller contradictions

**18. the cost of S6's *"`v` left as it was"* in the checker: priced by nobody.** After `m.send(@v)`,
`v` is empty on the `.ok` path and whole on the `.err` path; the checker must type that (the extern
`-> i32 when 0` precedent the compiler-engineer cites is a mark on a C result, not a built-in's `T?`),
and a program that writes `_ = m.send(@v)` leaves a `v` whose state no line shows. Unpriced; the
compiler-engineer's line table has no row for it.

**19. S14's sequence: the seed names the doors.** `grep -c hero_thread_spawn seed/heroes.c`: **12**
(`:84-85`, `_Static_assert`s on `hero_thread_spawn_sized` and `hero_thread_join`). The
compiler-engineer's *"the runtime stops exporting them (made `static`)"* breaks CLAUDE.md § Commands'
first line (clang on the seed and the runtime) until a seed that names no door exists; the doors can
leave `hero_os.h` only after (1) a seed refresh that knows `spawn_sized`, (2) `selfhost/main.hero`
moved to it, (3) a second seed refresh that names no door, and (4) every one of the 69 files declaring
a door moved (`00-shared.md` § 5 item 10). The spec-warden states (1) and (2), the ffi-pragmatist keeps
the doors until callers move; nobody states (3). No runtime file outside `parts/thread.c`, `spawn.c`,
`spawn_str.c` and the headers names them (`grep -rln`), so nothing internal is lost.

**20. the compiler-engineer's *"24 `Conn` handles ... told at `main`'s end"* is an inference in that
seat's emulation**, whose connections cross as `i64` descriptors (`gateway.c`'s `conns` mailbox takes
`hero_cross_take_i64`); the ffi-pragmatist measured the telling on route A (`p254/left/`, *"1 C handle(s)
never given back ... in hero_stream_number"*), so the inference rests on a measurement of the set, not of
B's queue.

**21. the ffi-pragmatist's veto measurement, 8 of 10, read 9 of 10 here** (finding 1): the same class,
one run apart; nothing turns on it.

**22. missing: four questions the sitting did not ask, each a route or a sentence no report priced.**
(a) **A send that neither waits nor grows**: S6's unbounded queue against a waiting bounded one was the
brief's question 3, and the issue's own sketch was *"a bounded send"*; a third shape, a bound per
mailbox whose `send` fails `full` at once (`v` left as it was), admits neither the waiting send's
deadlock (the compiler-engineer's inference, unrun) nor unbounded growth; nobody built or priced it,
and nobody measured the unbounded queue's memory under a producer faster than its consumer. (b) **A
wait with a limit**: every hang the reports name (a worker in `receive(-1)` on a queue nobody closes,
waited by `main`; behaviour 2 with a forgotten close) is a `t.wait()` that cannot end; S3 gives `wait`
no `within_ms:`, and no seat asked whether it should. (c) **A reply that cannot be read twice**: the
stale-reply class (a reply arriving after `timeout` or `stopped`, read by the next request) is the
reuse of a reply mailbox; the compiler-engineer's text names it, the language could make it impossible
(a one-shot reply, or a send-and-wait operation that makes its reply mailbox itself, Erlang's
`gen_server:call` with its unique reference), and no seat priced that. (d) **A deadlock report**: Go's
runtime aborts when every goroutine is asleep; with any behaviour of the stop, a program whose every
thread waits in `receive(-1)` or `wait` hangs in silence under every behaviour measured (finding 6's
forgotten close under behaviour 2; any forgotten close with no stop asked); unpriced.

**23. checked: the holder check covers every C crossing the emitter already guards, by its own
rule.** `selfhost/emit/handle_before.hero` (read, its head comment): *"EVERY CROSSING OF A HANDLE INTO C
IS ASKED FIRST ... Whatever the parameter's mark, none, `consumes`, `transfers`, `retains` or `borrows`,
a handle the argument reaches is asked `hero_handle_alive` before anything else"*, the one exception a
cell a handle comes out through. So the ffi-pragmatist's *"it covers every path to C because the emitter
writes `hero_handle_alive_at` before every C call with a handle argument"* holds by the emitter's own
rule (read, `:167` writes the call), for a handle reached inside a record or a list too, since the
emitter uses `check/reaches`. What it cannot see is a C library that keeps a handle and calls it from
a thread of its own, which is the C side's, and the callback guard already refuses Heroes code on a
thread the runtime did not start (`selfhost/emit/callback_guard.hero`, head comment).

## Not checked, and why

- **Sanitizers on my own runs**: the holder shapes (finding 2) and the gateway re-runs of finding 6
  ran plain only, for time (behaviour 3 ran under TSan and ASan+UBSan, finding 6b); the seats' ASan, UBSan and TSan counts were not re-run.
- **The in-transit window** (a hand-over forgotten on the sending side, finding 2): read from the code,
  the race not run.
- **The compiler half of anything**: W-j'', split ends, S6's flow typing of `v`, W-f's walk through a
  thread's answer: priced by nobody, built by nobody, here included.
- **The spec text's cost** of every amendment above: a real count needs a paid refresh, not mine; the
  spec-warden's `w2` is unrefreshed too.
- **Linux arm64 and Windows**: no Docker asked, no box; every Windows arm in the reports is written and
  unrun.
- **The seats' own instruments**: the compiler-engineer's 289 and 313 counts, its 120 stress runs, the
  ffi-pragmatist's TLS, socket, SQLite and route A runs, the historian's pages and the spec-warden's
  four refreshes were read, not re-run; only finding 1's and finding 6's programs were.
- **The full net and the compiler's own tests on the holder runtime**: not run; the `runtime` suite on
  it read 10 and 0 (finding 4).

## What the synthesis must settle

Most important first.

1. **S11, the soundness veto: confirmed, and the repair's text owed by thread, not by copy.** Re-run:
   9 of 10 killed without the holder check, 10 of 10 stopped by name with it, 10 of 10 clean moves
   (finding 1). The resolution adopts the runtime's holder check by a runtime thread number (never a
   `pthread_t`, finding 2's `deadthread`), made by the per-type crossing walk for **every** handle a value
   holds, on both sides, at `send`/`receive`, at `spawn`/the thread's start and at the thread's
   answer/`wait`, the hand-over made only once a send has succeeded (finding 5); and it says what the
   check is: **a thread check** (a handle sent back makes old copies on its first thread valid again,
   finding 2's `sendback`). Text: *"... ; reaching C through it on a thread that does not hold it
   aborts, naming the call."* Five runtime messages owed, blocking-class: the holder message in the
   three shapes where it is false (finding 2), the dead value after a send naming *consumes or
   transfers*, a handle dropped with a queue not naming the mailbox (the compiler-engineer's Q2.5).
2. **S7 and S12: choose on finding 6's whole table, not on *"served"*.** Stop-wins strands 24 of 32
   (loud); the drain strands none and **tells wrong counts in silence where a reply mailbox is reused**
   (6 of 6 runs); behaviour 2 is exact and hangs on a forgotten close; **behaviour 3, unlisted (drain,
   then `stopped` for ever), stranded none and told no wrong count in 12 plain runs of the emulation and
   12 more under TSan and ASan+UBSan with 0 reports** (finding 6b). By the measurements the most robust
   text is behaviour 3's, **owed the compiler-engineer's stress topologies and a Linux run before the
   lane is held to it**; failing those, the drain with
   the compiler-engineer's stale-reply sentence. Either way: `closed` before `stopped` (finding 8), a
   `stopped` receive says nothing of whether a request was applied, and the runtime never calls
   `hero_net.h`, the layer calling the queues (finding 7's lock order written down as a rule).
3. **W-j, W-j' or W-j''** (finding 9). W-j' is checkable only as a rule on a function's body that
   depends on a `spawn` elsewhere, the non-local class the unrun blind seat exists to judge; if adopted,
   the text must say what a direct call of the same function does. W-j'' (`spawn(serve, @c)` with
   `function serve(@c: Conn)`) is local and unpriced; W-j costs a mailbox of one. The ffi-pragmatist is
   indifferent provided `spawn` and `wait` move the holder.
4. **W-f and the cycle**: decided at declarations in the checker (finding 11); its walk owes a
   thread's answer type; the compiler-engineer's gate at `main`'s end beside it; the refused peer shape
   (finding 12) recorded, and **split ends recorded as the unpriced route that needs no refusal**, so the
   author can choose it.
5. **The blind seat** (finding 17): the resolution is provisional on it; t2 program 3's registered row
   is the stop-wins answer and inverts under any draining behaviour; the registered run scores W1 to
   W11 on the registered arms, and the adopted text is a fourth arm whose predictions are registered
   before any session of it runs.
6. **S6**: *"`v` then left as it was"* (two seats and the historian against one that did not price it);
   the checker's typing of `v` after `m.send(@v)` priced by nobody (finding 18).
7. **S1**: `limit` covers the runtime's table of 256 (the 257th `spawn` exits 134 today, finding 14).
8. **S14's sequence** (finding 19): the seed names the doors 12 times, so they leave `hero_os.h` and
   the runtime's exports only after two seed refreshes and every one of the 69 declaring files moved.
9. **The adjacent defect** (finding 3): `hero_ipc`'s holder compared by `pthread_equal` after its thread
   ended, reproduced 3 of 3; filed by the coordinator.
10. **What the route leaves out**, said so in the resolution rather than found later: a mailbox in a
    `poll` set (finding 15), a non-waiting bounded send, a `wait` with a limit, a one-shot reply and a
    deadlock report (finding 22).

### The rows, where the seats' texts differ, and the text their measurements support

| row | where they differ | the text the measurements support as most robust |
|---|---|---|
| S1 | warden and historian as written; compiler-engineer and ffi-pragmatist amend | *"`spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`, `T` being `f`'s answer, failing `limit` where the system or the runtime has no thread to give, its message saying why;"* (257th thread: exit 134 today) |
| S2 | none | as written; it reads S1's `limit` through *"does the same"* |
| S3 | historian as written; three seats W-a | § 3: *"a running thread whose function answers a `T`; a handle (section 13) with no null, which its `wait` ends"*; `wait` moves the holder (S11) |
| S4 | none who spoke | W-e: *"`mailbox()` gives a new, empty `(mailbox T)`, typed as an empty `[]` is (section 10)."* |
| S5 | historian objects (advisory); three seats adopt a value with W-b, W-f | a value with a count of copies; § 3 W-b (*"... but a mailbox's queue"*); W-f's clause **with a thread's answer added to its walk** (finding 11); the runtime's gate at `main`'s end; split ends recorded as the alternative (finding 12) |
| S6 | warden as written; compiler-engineer, ffi-pragmatist, historian amend | *"`m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed, `v` then left as it was;"* (under behaviour 3, also failing `stopped` once the mailbox has answered `stopped`) |
| S7 | warden: stop-wins, `stopped` before all else; compiler-engineer and ffi-pragmatist: the drain, `closed` first | behaviour 3 once measured: *"... on an empty mailbox it fails `closed` once it is closed, `stopped` once a stop is asked (section 13) and from then on, and `timeout` else; a value that arrives after a `timeout` is the next receive's"*; the drain's text (the compiler-engineer's) if behaviour 3 is not measured |
| S8 | none | as written |
| S9 | historian excepted function values; ffi-pragmatist adds a boundary clause | as written, function values included (`heroes_runtime.h:659-661`, a bare C pointer, read), plus *"A `(thread T)` and a `(mailbox T)` never reach C: no `extern` signature, callback parameter or group record field holds one."* |
| S10 | where the refusal lives | as written, refused by the checker (W-f's *"anything else is a compile error"*), at declarations and at the type arguments the checker infers |
| S11 | ffi-pragmatist veto; warden W-j; compiler-engineer W-j' | the ffi-pragmatist's amendment phrased by thread (item 1 above) plus the runtime's holder check; the hand-over form W-j, W-j' or W-j'' (item 3) |
| S12 | warden as written; compiler-engineer and ffi-pragmatist amend with S7 | the compiler-engineer's: *"... and a waiting call of that layer, or a mailbox's `receive` that finds it empty, answer stopped;"*, the runtime never calling the layer |
| S13 | none of substance | W-c, W-d, W-g, W-h; `wait`, `send`, `receive`, `close` never in the built-in table (the ffi-pragmatist's `builtin_name_taken` on `exit`) |
| S14 | compiler-engineer *"made `static`"*; ffi-pragmatist *"until every caller moves"*; warden *"in the same landing"* | removed from the text; the doors leave `hero_os.h` after the four steps of finding 19 |
