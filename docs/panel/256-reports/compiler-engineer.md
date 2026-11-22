# Panel 256, the compiler-engineer

Written 2026-11-22 from 22:11, as it went. Every command ran in my copy,
`.claude/worktrees/p256-scratch/compiler-engineer/` (`git log -1 --format=%H` printed
`8aca3cc432a6007daaf416ecf3b1b8564472fa09`, `git status --short` printed nothing), or read the briefs
folder. My agent file and CLAUDE.md § Precedence were read in the copy (the frozen ones bind); the agent
file is the one my session loaded, word for word in its body; § Precedence carries the 2026-11-22
amendment.

`uptime` at the start: load averages 21.50 13.18 8.27. `pmset -g assertions`: `PreventUserIdleSystemSleep 1`.

## 0. The compiler

(in progress: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` then `./heroes build
selfhost/main.hero -o heroes-next`, started 22:12)
- `clang ... -o heroes` then `./heroes build selfhost/main.hero -o heroes-next`: exit 0 (`build-stage.log`:
  `wrote heroes-next`); `./heroes-next --version`: `heroes 0.6.0` / `sources 031aaabb358218fd`, the
  coordinator's. Kept as `heroes-frozen`. (The first clang wrote its temporary object to the system's
  TMPDIR; every later command ran with `TMPDIR=<copy>/ce256/tmp`.) No duration is given: load 21 to 47
  (`uptime`) the whole time.
- **The probes** (`cp` of `256-briefs/probes/q2/` to `ce256/q2-frozen/`, `bash run.sh ../../heroes-frozen`):
  `diff` against `coordinator-run.txt`, pids and numbers of four digits or more masked, prints only the
  coordinator's two header lines. **I read what the coordinator reads**, every row.

## 1. Reading the mechanism (Q2 items 1, 7b), at the frozen commit

- `heroes check` runs the checker alone (`selfhost/cli/check.hero:65`, `checker.check`), never lowering or
  `mono` (`mono.run` is called from `cli/compile.hero:211`, `emit.hero:333` and tests only: `grep -rn
  "mono\.run"`). **So any rule placed after `mono` is silent under `heroes check`**, as polymorphic
  recursion is today. That alone moves my build into the checker.
- **Panel 155's obstacle (7b) is a recording, not a map.** Each of the three sites that write
  `instantiations[span.end]` holds the callee's declaration in a local at that line: `decl` at
  `check/walk.hero:1790` (the same `decl` handed to `synth_argument` above it), `decl` at
  `function_value.hero:211`, `decl` at `waiting_names.hero:126`. One more table keyed the same way,
  `instantiated[span.end] = [callee, span.start]`, answers *which body's needs apply at this call*: 1 line
  per site, 6 in `state.hero` (field, comment, initialiser). The *caller* is the generic declaration whose
  text holds the call: `ast.Decl.span` runs from the keyword to the body's end
  (`parse/tails.hero:177`, `finish @ body.must().span`), so it is a containment test over the generic
  declarations, 5 lines (`owner`). 155's 150 lines were a node-to-declaration map for every node; this
  needs it only for the calls and the rule sites.
- **The IR keeps the send's `@`**: `emit/threads.hero:41-46,60-65`, a `.inout_arg` for `m.send(@v)`, a
  `.value_arg` for `m.send(v)`, and `sent_of` is `NULL` for a type holding no handle
  (`emit/crossing.hero:68-70`). And `ir/mono.hero` already holds each instance's declaration and its
  settled letters (`Instance.decl`, `bound`, `:46-49`), generic-calls-generic substituted (`:104-121`).
- The class (item 1), by reading `check/handed.hero:44`, `check/crossing.hero:107`,
  `emit/key_pointer.hero:51-53`, `check/thread_ops.hero:99-108,175-192`: five rules walk what a type holds
  and answer nothing for a letter: (a) the send's `@` (`marked`), (b) a handle into `spawn`
  (`handle_into_spawn`), (c) a `ptr`/`cstr` crossing by `spawn`'s argument or answer (`crosses`), (d) a
  written `(mailbox T)`/`(thread T)` whose element reaches a `ptr` (`crossing.judge`), (e) a unit mailbox
  (`crossing.judge`). The sixth, a queue holding itself (`holds_itself`), **cannot be reached through a
  letter**: it needs `T` to contain `(mailbox T)`, an infinite type no binding can be (reasoned, not run).
  Beyond threads, `e2` (a `partial` key through a letter) is panel 155's guarded class (critic 4), the
  run-time `hero_panic` designed by panel 061: I leave it filed, not closed by this resolution.

## 2. The route built: gi in the checker, as needs carried through calls (Q2 items 2, 3, 7b)

**Design** (`selfhost/check/instances.hero`, new, 399 lines by `wc -l`, 328 code lines by the
write hook's count of `suite_layout`'s unit: past the 300 ceiling, so a landing splits it, the
messages from the walk): every rule (a) to (e) that a generic body reaches through a letter becomes a
**need** of its declaration, `(decl, rule, letters, origin span)`. After every body is typed (beside
`moved` and `endless`, `checker.hero`), every recorded instantiation asks its callee's needs of the types
it gives the letters: a type that breaks a need is told **at that call**, the body's file and line in the
message; a type naming the **caller's** letters hands the need to the caller. Needs are deduplicated by
`(decl, rule, letters)`, a finite set, so the fixpoint ends under polymorphic recursion (`f<T>` calling
`f<[T]>`: `[T]` reaches the letter `T` again, an old key). The send's `@` under gi is the one disjunctive
rule: *some* letter reached must hold a handle, so its need carries the letter set, not one letter.
The body's own refusal moves: `thread_ops.marked` returns early where the element reaches a letter and
holds nothing concretely (5 lines), so `p1`'s `hand<T>` is no longer refused in the body.

**Where it lands** (`git diff --stat` in my copy): `check/state.hero` +6, `check/walk.hero` +1,
`check/function_value.hero` +1, `check/waiting_names.hero` +1, `check/thread_ops.hero` +5,
`checker.hero` +5, and `check/instances.hero` 399. **No lowering, IR, emitter or runtime line.** Erased
in the frontend: by design.md Part 5's test it is a checker rule, not a construct the backend sees.

**Built**: `./heroes-frozen check selfhost/main.hero` exit 0 with the rule (after one error of mine, an
unused parameter); `./heroes-frozen build selfhost/main.hero -o heroes-gi` exit 0, `--version`
`sources 350f0deb647ad93e` (its `real 257.88` against `user 167.13` + `sys 41.41`: a waiting run, no
duration).

### 2.1 What gi answers, every probe (`ce256/q2-gi/run2.sh ../../heroes-gi`, outputs `gi-probes.txt`, `gi-programs.txt`)

| probe | frozen (`heroes-frozen`) | gi (`heroes-gi`) |
|---|---|---|
| `p1_defect` | 1, `marker_mismatch` in `hand<T>`'s body | 1, at `main`'s call of `relay`: *"this call gives `T` the type `Boxed`, which holds a C handle, and `relay` sends a value of it without `@` at p1_defect.hero:4"*; `hand(m, @b)` accepted |
| `p2_relay_only` | 0, 0, 0 | **1**, `marker_mismatch` at the call, body line 4 |
| `p3`, `p3b` (spawn of `Boxed`) | 0, 0, 0 / 134 | **1**, `handle_into_spawn` at the call, body line 4 |
| `p4_ptr_generic` | 1 (written type) | 1, the written type **and** a second message at `relay`'s call (two messages for one mistake) |
| `p5`, `p5c` (`(mailbox T)` at `Pointed`) | 0, 0, 0 | **1**, `thread_crossing` at the call, body line 3; twice (two `(mailbox T)` written in `fresh`) |
| `p6b_unit_generic` | 0, **build 134** (the compiler's panic) | **1**, `empty_mailbox` at the call, body lines 1 and 2 (twice) |
| `p7_spawn_ptr_generic` | 0, 0, 0 | **1**, `thread_crossing` at the call, body line 4 |
| `p8_answer_ptr_generic` | 0, 0, 0 | **1**, `thread_crossing` at the call, body line 4 |
| controls `c3`, `c6`, `c7`, `p9`, `e1` | 1 each | 1 each, the same first line |
| `e2_partial_key_generic` | 0, 0, 134 | 0, 0, 134 (left as panel 155 ruled it) |

**The six t2 programs** (copied with `db.hero` into `ce256/q2-gi/`): `prog1` 0, 0, run 0 *"false 7"*; `prog2`
**0, 0, run 0** *"false true"*; `prog3` 1 at the call (*"`T` the type `Db`, which holds a C handle, and
`relay` sends a value of it without `@` at prog3.hero:4"*); `prog4` 1 at the call (*"`T` the type `i64`,
which holds no C handle and no thread, and `hand` sends a value of it with `@` at prog4.hero:2"*);
`prog5` 1, `handle_into_spawn` at the call; `prog6` 0, 0, run 0 *"42"*. The frozen compiler's answers
(the coordinator's `coordinator-run.txt`, re-read): 1 runs, 2 refused in the body, 3 runs to 134, 4 refused
in the body, 5 runs to 134, 6 runs. **gi turns both run-time aborts (3, 5) into compile errors at the
call, accepts 2, and keeps 4 refused.**

**The shapes beside** (mine, `ce256/q2-gi/n*.hero`): `n1` a generic calling a generic (`outer<A>` calls
`inner<T>` which sends `v` bare), at `Boxed`: **1** at `main`'s call, body line 4, **but the message names
`outer` as the sender, where the send is `inner`'s**: a false sentence in my prototype's wording (the line
is right; the function should be the origin's declaration, `owner(origin)`), which a landing owes;
`n5` the same with `@` all the way down: 0, 0, run 0 *"false true"*. `n2` the disjunctive `@`
(`hand<A, B>` sending `@v: {A: B}`): `{str: Boxed}` accepted, `{str: i64}` refused at its call (the
message names `A` alone, where the need is *A or B*). `n3` polymorphic recursion (`deep<T>` calling
`deep<[T]>`) with a bare send, at `Boxed`: **check terminates**, 1 at the call, body line 5 (the
frozen compiler: check 0; `mono` would refuse the recursion at build). `n4` a generic function **value**
(`f: (function(Pointed) -> i64) = keep`, `keep<T>` writing `(mailbox T)`): 1 at the value's
instantiation, body line 4 (frozen: 0).

**The census** (`heroes-gi check`): `examples-rebel/certs/main.hero` 0, `examples-rebel/iothub/main.hero`
0, `tests/golden/run/threads-handles-in-lists-maps-and-generic-functions.hero` 0,
`tests/golden/run/fixedbugs-2682-ok-of-a-unit-a-thread-answered.hero` 0. **gi refuses none of the four.**
The awk over all 852 generics (`'^function [a-z_0-9]+<'` then a body line naming `(mailbox|thread)` of a
capitalised word, `.send(`, `spawn(`, `.wait()` or `mailbox()`) prints the same four and no fifth.

### 2.2 Item 7d: is gi what § 11 already says? **Yes, by my build.**

Run on the six programs, gi's verdicts are the frozen § 11 sentence applied to each instance: a handle
crosses only through `m.send(@v)` (programs 3 and 5 refused, 2 accepted), and § 9's marker rule at the
instance (an `@` at a call whose parameter is not written) refuses 4. Nothing gi refuses is a program
the frozen text accepts when its types are settled, and nothing it accepts is one the text refuses: the
only text gi needs is **where** the error is told (at the call, the body's line named), which is
§4.17's, not a new rule. `gc` and `gr` change what a program may write; gi does not.

## 3. Q1 item 8: `m.number()`, its runtime half built

**Built** in a copy of the runtime (`ce256/q1/runtime/`, `cp -R runtime`; the tree's `runtime/` untouched,
since the compiler build compiles it): `hero_mail_number(m)` (the slot plus one in the high half, the
life in the low, the slot taken the first time it is asked), `hero_mail_ready(number)` (1 while a value
waits, the queue is closed or answers `stopped`; 0 while a receive would wait; -1 once the number names
nothing), `hero_mail_watch(number, ring, ctx)` (the layer's watch, `ring` NULL to take it away, which
returns once no ring is in flight), and the ring in `hero_mail_send` after `hero_mail_drop(&m->lock)`
(the sender's copy keeps the queue), in `hero_mail_close` after its drop, and in `hero_mail_hear_stop`
under the list's lock alone. The slot is cleared in `hero_mail_decref` under the list's lock. **115 added
lines in `parts/mail.c`, 4 in `heroes_runtime.h`** (`diff -u ... | grep -c '^+[^+]'`).

**The C test** (`ce256/q1/test_ring.c`, `clang -std=c11 -Wall -Wextra -I runtime test_ring.c
runtime/runtime.c`, run under `timeout -s KILL 20`, exit 0): the "layer" is a pipe the ring writes one
byte down, as an `eventfd` or `EVFILT_USER` would be rung. Its output, whole: `number 4294967297, again
4294967297` / `ready before any send: 0` / `watch: 0` / **`poll woke: 1 (revents 1), rings 1`** (a second
thread's send rang the watch and woke the main thread's `poll`) / `send answered 0` / `ready with a value
queued: 1` / `receive 0, got 42` / `ready once drained: 0` / `ready once closed: 1, rings 2` / `watch taken
away: 0` / `ready once the queue has gone: -1, watch: -1` / `a new queue in the same slot: 4294967298, the
old number now: -1`. Under `-fsanitize=thread`: exit 0, `grep -c "WARNING: ThreadSanitizer"` 0 (one run,
one send: thin evidence, not a stress).

**The language half, priced from the code, unbuilt**: `inventory.hero` `OPERATIONS` +1 (`:198-204`) and
`arity_of` +1 (`:299-302`); `check/thread_types.hero` `operates` +1 (`:36-39`); `check/thread_ops.hero`
`operation` +3 (a `number` taking nothing, answering `i64`, `:127-149`); `emit/threads.hero` `writes` +1
and one branch, `hero_mail_number(m)`, about 4 (`:30-31`, `:52-90`); `operation_names` refuses a program's
`number` taking a mailbox first with no change (critic 29: none of the tree's 55 does). About 10 lines of
compiler and 119 of runtime. **Spec § 11's boundary row holds as written**: the mailbox never reaches C;
an `i64` does, and C cannot reach a value with it (the table answers a queue's state, never a value).

**What I measured against R6 and the lock order**: the ring is the runtime calling the layer's code
through a pointer, so R6's *words* (`mail.c:44-46`) do not hold and its *measured reason* (linkage: a
`print(1)` program links no `hero_net`) does: the runtime names no layer symbol (the pointer is the
layer's, registered at run time). The lock order holds by construction: no ring runs under a queue's lock;
the stop's ring runs under the list's lock, which `hero_mail_watch` also takes, so a watch taken away
cannot free a context a stop's ring is reading. What it costs the layer: a ring may take **no lock a
`hero_mail_watch` caller holds**, which an `eventfd` write, a `kevent(EVFILT_USER, NOTE_TRIGGER)` and a
`sendto` on Windows each satisfy (read, not run here).

## 4. The other routes, built behind one switch (Q2 items 4, 5, 7c, 9)

**One more build**, `heroes-sw` (`sources 5cafe2523b320e66`, exit 0, `real 271.82` against `user 172.01` +
`sys 42.61`, waiting): the route read from the environment (`gi`, `gc`, `gr`, `off`) and the emitter's
guard (7c) in `emit/crossing_bodies.hero` (+12 lines, `body`'s `put`: a type reaching a `ptr` or a
`cstr` writes `hero_panic` and copies nothing). **Two mistakes of mine in that edit, found by reading
before any verdict was taken from it**: my rename turned the variable's name into `P256_route()`, and
`cli/environment.env` answers `ok("")` for an unset variable (`environment.hero:41-50`), so with nothing
set the build ran a hybrid (the bare-send need alone: `prog4` passed) and its guard is always on. That
first gr run is discarded; every run below sets the literal name, `env "P256_route()=<route>"`, and
`gi` through `heroes-sw` reproduced `heroes-gi`'s answers on the nine programs I ran both ways.

### 4.1 gr, refused in the body (item 5)

The six t2 programs: **all six refused**, 1 to 6, each in the body (`prog1`: *"a `(mailbox T)` or a
`(thread T)` names no type parameter in a generic function"* at 1:22, then the send at 2:12). The four
live generics: **all four refused** (`certs/demo/stage.hero:138:14`, `iothub/demo/world.hero:245:9`,
the golden's `relay<A>` twice at 13:25 and 14:5, `fixedbugs-2682`'s `run<T>` at 11:9). Over the 852,
the awk of § 2.1 finds no fifth body gr reaches. **And item 9**: `q9_add_mailbox.hero`, a generic
`add_mailbox<T>(@set: Set, m: (mailbox T)) -> i64?`, is **refused by gr** at 6:39 (*"names no type
parameter"*), and accepted by gi and by the frozen compiler (0, 0, run 0 *"1 2"*). **So under gr,
P1's `poll.add_mailbox<T>` cannot be written in Rebel at all**: gr and Q1's face are incompatible.

### 4.2 gc, and what its `@` means for a letter whose instance holds no handle (item 4)

`env "P256_route()=gc"` over the six: `prog1` **refused** in the body (*"is sent `m.send(@v)` in a generic
function"*: a program the frozen text accepts), `prog2` 0, 0, run 0 *"false true"*, `prog3` refused in
the body, `prog4` **0, 0, run 0 *"false 7"***, `prog5` refused at the call (spawn needs as gi), `prog6`
0, 0, 42. The census: gc refuses the golden's `relay<A>` alone (14:5), none of certs, iothub, 2682.

**Item 4, measured** (`g4i_read_after_at_i64.hero`, `g4_read_after_at.hero`: `hand<T>(m, @v: T) -> T`
sends `m.send(@v)` then returns `v`): at `T = i64`, **check 0, build 0, run 0 printing `false 7 7 7`**:
the send succeeded, and `v` read after it, and `n` in `main`, still hold 7. The emitter writes
`hero_mail_send(t1, h_0put_1b9a87, NULL, (void *)&(*ph1_v), &hero_here)` (`--emit-c`, line 135): the
`sent` argument is `NULL` for a type that holds no handle (`emit/crossing.hero:68-70`), so **nothing is
emptied**. At `T = Boxed` the same body, the same read: **run 134**, *"a C handle reached the argument `f`
of `fclose` through a value `m.send(@v)` emptied"*. So gc's sentence *"which empties it whatever that type
holds"* is false of the build for every type without a handle, and one source line reads `7` in one
instance and aborts in another. **Can the lives walk decide it in the body once?** Not as the walk stands:
`check/moved.hero` ends a life at `wait` alone (`moved_calls.hero:36-39`) and reads an `@` argument as a
write that keeps it alive (`moved.hero:38`); and a send that fails `closed` or `stopped` leaves `v` *"as it
was"* (spec § 11), so a rule *"dead after `m.send(@v)` on every path"* refuses the retry a program may
write on the failure path. Making gc true needs either that over-refusal or an emptied `i64`, a value the
language has no word for. **gc is the route with something that can be wrong at run time.**

### 4.3 The emitter's guard after `mono` (item 7c)

`env "P256_route()=off"` (no checker rule) with the guard: `p7`, `p8`, `p5`, `p5c`, `n4` **check 0, build
0, run 134** *"a value reaching a ptr or a cstr crosses between threads (spec section 11)"*, the abort in
`h_0put_1541e50a`, the put, before a byte is copied (`p8`'s inside `h_0run_...`, the thread's answer);
`p3` (a handle, not a `ptr`) 0, 0, 0 *"4"* as before; `prog1` *"false 7"* and `prog6` *"42"*
unchanged. **12 lines, one file, no new pass**: the emitter already walks every instance's crossing types
after `mono` (`emit/crossing.hero:60,77`). It closes `p7` and `p8` **at run time**, never at `check`, and
a single-threaded `p5`/`p5c` aborts too (its mailbox put runs on one thread), which is the § 11 rule
(*"a `(mailbox T)` whose `T` holds a `ptr`"*) enforced late. **As the only route it is weaker than gi**
(an abort where a compile error is owed, in an instance a test may never run); **behind gi it is exactly
the guard panel 155's veto asked of any give-up**, and the cheapest line in this sitting.

### 4.4 After `mono` as a pass, and a written bound (item 6)

- **After `mono`, priced and refused as the primary route.** Priced from the code: `ir/mono.hero` holds
  `Instance.decl` and `bound` per instance (`:46-49`), the IR keeps the send's `@` (`emit/threads.hero:41-46`),
  so a walk of each instance's runs is a new module of about the size of my `instances.hero` less its
  fixpoint (mono's queue is the fixpoint), called from `mono.run`, told at exit 1 as polymorphic
  recursion is. **Refused because `heroes check` never runs `mono`** (`cli/check.hero:65`): `check` would
  accept what `build` refuses, in the loop the thesis is measured on; and mono names an instance by the
  first call that reached it (`Pending.span`), never every call. Its seam is worth 12 lines as the guard
  (4.3), not a pass.
- **A written bound** (`T: Send`): **refused, on measurement.** It is a core construct by Part 5's test (a
  new form in the grammar, the formatter, the probe's reader, `mutate` and both colourers, by
  `.claude/rules/diagnostics-and-goldens.md`'s list), and every generic calling a generic must repeat it
  (Rust's viral bound), where my build **infers** the same need and carries it through the call
  (`n1`, `n3`, `n5`). Nothing a bound would refuse is accepted by gi: every probe of § 2.1 is refused
  without one. What would change it: a program whose need, inferred three calls deep, is told at a call
  an author cannot connect to the body's line; the cheaper answer to that is `heroes doc` printing the
  inferred need, not a form.

### 4.5 Panel 155's veto condition asked of my build

Where my build gives up: (1) a recorded instantiation with no `instantiated` entry is skipped
(`instances.hero`, `if !c.out.instantiated[end].is_err()`); all three recording sites write both tables
on the same line, so it is unreachable by construction, and a landing makes it an assertion; (2) a
binding that is the error type (a program already refused) is skipped; (3) `e2` is not this rule's
(panel 061's `hero_panic` guards it). **What stands behind a give-up**: for a `ptr`/`cstr`, the 12-line
emitter guard (4.3); for a handle, the runtime's holder and consumed-address checks (`p3b`, the critic's
`k3d`, `g4c`: run 134 each). So gi plus the guard meets 155's condition; gi alone meets it for handles
and not for a `ptr`.

### 4.6 What my prototype owes before it could land (found by running it)

- **A false sentence** (`n1`): the message names the called function (`outer`) as the one that sends,
  where the send is `inner`'s; the line is right. It must name the origin's declaration.
- **Two messages for one mistake** (`p5`, `p5c`, `p6b`: one per `(mailbox T)` written in the body; `p4`:
  the written type and the call). Deduplicate by (call, rule, letter).
- **The disjunctive `@`** (`n2`) names one letter of two.
- **The ceiling**: `instances.hero` is 328 code lines (the write hook's count), past 300: the needs'
  collection and the messages split from the fixpoint.
- **Unrun**: the compiler's own tests, the golden forms and the census of `check` with the rule; the
  brief's cases were run, nothing wider (§ 6 says what was).

## 5. Q1: is a number the right door? (item 8, the comparison)

What I measured decides less than I would like, and I say which half is read:

- **The number** (built in the runtime, § 3): 119 lines of runtime, about 10 of compiler (priced), no new
  type, no new syntax, the dotted-operation machinery reused (`inventory.operation_of`,
  `thread_types.operates`, `emit/threads.writes`). The mailbox never reaches C, so spec § 11's boundary
  row stands word for word; C holds a name that answers `-1` once its queue has gone (measured), the
  shape panel 246 gave a socket. **The one cost no measurement here removes**: the ring is layer code
  called from the runtime, whose lock discipline is the layer's to keep (a ring may take no lock a
  `hero_mail_watch` caller holds).
- **A descriptor the mailbox hands out** (`fs/watch.hero:298-330`'s shape, critic 12): no callback, R6's
  words kept, level-triggered by construction on POSIX (a byte written when the queue goes from empty,
  read when it empties). **Read, not built**: on Windows the set is a `WSAPoll` array, so the descriptor
  must be a loopback socket, which puts Winsock calls into `parts/mail.c`, and R6's *measured* reason
  (a `print(1)` program links no network code, `254-...md:416`) is the one this would break. The number
  plus a watch keeps the waking mechanism in the layer, which is where Winsock already is.
- **A mailbox handed to C under a mark**: refused on the ceiling and the boundary. It relaxes the § 11 row
  for one form and gives C a counted reference it must give back, a new retention contract at the C
  boundary (`.claude/rules/c-boundary.md`'s territory) for what an `i64` already does.
- **The pump** and **a language-level wait over mailboxes**: the pump needs no language change but a
  thread per set and every event queued twice (the forwarder's price, critic 11), and it puts the user
  interface's wait inside `receive`, which `ui.wait` (the system's own loop) cannot be; a `select` over
  mailboxes waits on no socket. Neither is one wait for everything.
- **The one wait in the runtime** (Go's poller): the readiness layer's 1,531 lines (`hero_poll.c`, `wc -l`)
  move into what every program links; refused on the ceiling (§1.1) before any other argument.

**I would build the number with the watch**, and land the runtime half first, since it is what the C
test exercised; the compiler half is about ten lines across five files I read and did not build.

## 6. Item 7, the clocks (frozen against gi)

`ce256/clock.sh`: alternated pairs, the order inside a pair alternating, every run's `real`, `user`,
`sys` and the 1-minute load kept (`ce256/clocks.txt`); the floor first, the frozen compiler against a
byte copy of itself (`heroes-frozen2`), then frozen against `heroes-gi`, on `check selfhost/main.hero`
and on `check examples-rebel/certs/main.hero` (a Rebel program that instantiates `start<S>` four times and
reaches 555 of `packages`' generics through `rebel`).

## 7. Panel 155's conditions, asked of this build (`docs/panel/155-the-hole-was-never-made-by-the-generic.md:147-158, 201-212`)

- **155's R1 grounds, one by one.** *A second span-keyed table*: mine, `instantiated`, is keyed by the very
  key `instantiations` uses and written on the same line (`walk.hero:1790` and the two others), so it
  inherits that table's soundness argument (`state.hero:70-85`, the call's END) and adds no new kind of key;
  it could as well be the callee carried inside `instantiations`' own value. *Duplicating
  `map_keys.refuse`*: none; the needs ask `handed.held_handle` and `key_pointer.reaches_pointer` themselves,
  and the one new walk, `letters_of` (about 20 lines), asks a different question (which letters a type
  holds). *A node-to-declaration map*: replaced by the callee recorded where the checker already holds it
  and a containment test (§ 1). *A `depth > 16` give-up*: none; the needs are a finite set and `n3`
  (polymorphic recursion) terminates.
- **155's withdrawal condition for this seat**: *"a prototype passing `heroes check` at ≤150 total code
  lines with the net green and no golden rewritten; or a demonstration that this closes a §1.12
  robustness class"*. My prototype is about 347 lines (19 of wiring, 328 of module) and the net is unrun:
  **the first clause is not met.** The second is the ffi-pragmatist's to run (whether `p7`/`p8`'s `ptr`
  crossing corrupts memory; the brief marks it unrun). Under CLAUDE.md § Precedence as amended
  2026-11-22, the ceiling yields to the most robust resolution; I record the 347 lines as its price.
- **155's llm-ergonomist veto**: *"a constraint propagates transitively through nested generic calls
  without the diagnostic naming the full chain"*. **My build trips it**: `n1`'s message names the call in
  `main` and the body line, not `outer -> inner`, and names `outer` as the sender. A landing carries the
  chain in the need (each hand-over to a caller appends the call), about 10 lines, and the message prints
  it. **Until that is built, gi as I built it is vetoable by that seat's standing condition.**
  **Built afterwards, § 9** (`heroes-gi2`): `n1` and the three-deep `n6` name the chain
  (*"reached through `outer` -> `mid` -> `inner`"*) and the function that sends; the condition is met by
  a build, at the cost of the 446 lines counted in § 10.

**What the clocks gave: nothing admissible.** The first four runs, the floor on `check selfhost/main.hero`:
`real 19.46 / user 16.50 / sys 0.27` (load 29.02), `25.33 / 17.02 / 0.31` (27.79), `34.06 / 17.38 /
0.35` (51.10), `40.15 / 17.58 / 0.40` (73.48). **Every one has `real` above `user + sys`, so every one
is discarded** (CLAUDE.md § Verification), and the load rose to 82 and then 114 (`uptime`, other seats'
builds), so I stopped the script (`pkill`, then `pgrep -f` of my copy: 0 processes standing) rather than
spend half an hour on runs the rule throws away. **Item 7 is unrun on this machine today.** What can be
said without a clock, read from the code and not measured: on `selfhost/main.hero`, which declares one
generic (`fetch/verb_place.hero:95`, `unit_of<T>`, no thread operation), the rule is one linear pass over
the expressions and the written types and returns before it sorts any instantiation (`needs.len() ==
0`); on a Rebel program it adds a sort of the instantiation keys and a fixpoint whose size is the
number of needs times the calls to their declarations. The clock owed: `heroes run
tests/harness/latency.hero -- compare heroes-frozen heroes-gi` on a still machine, and the same pairs on
`check examples-rebel/certs/main.hero`.

## 8. The census: what gi refuses that the frozen compiler accepts (`ce256/census.sh`)

Every tracked golden case naming `(mailbox `, `(thread ` or `spawn(` (`git ls-files 'tests/golden/*.hero' |
xargs grep -l -E ...`: 98 files) and every `examples-rebel/*/main.hero` (42), checked by both compilers,
the exit code and the multiset of error codes compared. **The 98 golden cases: 74 accepted by both, 24
refused by both with the same codes and counts, 0 moved** (`census-golden.txt`). **The 42 Rebel
programs** (`examples-rebel/*/main.hero`, every one, each reaching `packages/rebel`): **42 accepted by
both, 0 moved** (`census-out-part1.txt`, 9, and `census-rebel.txt`, 33, re-counted against the list with
`comm`). Run with `heroes-gi`; `heroes-gi2` differs from it in its messages and its switches, not in what
it refuses, which § 9's runs of the same probes show. **So over 140 programs, gi refuses nothing the
frozen compiler accepts**; what it refuses is the probes' and the t2 programs' (§ 2.1), each a program
the frozen compiler built and ran to an abort, a compiler panic or an unchecked `ptr` crossing.

## 9. The repaired build, `heroes-gi2` (§ 4.6's three faults and the switch's two)

Edits in my copy: each need carries the declarations it was handed through (`via`), the message names the
origin's function and the chain; one message per (call, rule, letter); the disjunctive `@` names every
letter; `P256_ROUTE` read by its name, an unset or empty one meaning `gi`; the guard on only for
`P256_GUARD=1`. `./heroes-gi check selfhost/main.hero` exit 0 (after one error of mine, a local shadowing
a function's name); `./heroes-gi build selfhost/main.hero -o heroes-gi2` exit 0, `sources
0e8d15027f0726af` (`real 262.43` against `user 97.66` + `sys 3.79`: waiting). `instances.hero` is now 446
lines by `wc -l`, 326 neither blank nor comment (my `awk`, not `suite_layout`'s unit).

Run with no variable set (`ce256/q2-gi/gi2/`, `out-<probe>.txt`):

- `n1`: *"this call gives `A` the type `Boxed`, which holds a C handle, and `inner` sends a value of it
  without `@` at n1_nested_generic.hero:4, reached through `outer` -> `inner`"*: **the false sentence is
  gone and the chain is named**, which is panel 155's llm-ergonomist condition met by a build.
- `n6` (three deep, the letter wrapped in a list on the way: `outer<A>` calls `mid` with `[v]`, so `B = A`;
  `mid` calls `inner` with `v: [B]`, so `T = [B]`; `inner` sends `v: T` bare): 1 error, *"... `inner` sends a value of it without
  `@` at n6_three_deep.hero:4, reached through `outer` -> `mid` -> `inner`"*.
- `n2`: *"this call gives `A` and `B` the type `str` and `i64`, which holds no C handle and no thread, and
  `hand` sends a value of it with `@`"* (true; its grammar wants a plural).
- `p5`, `p5c`, `p6b`: **one** error each now. `p4` still two: the written `(mailbox kinds.Pointed)` in
  `main` and the call that gives `relay` that type (two messages for one mistake, `adjacent` in kind).
- `n3` 1, `n4` 1, `n5` 0; `prog1` 0, `prog2` 0, `prog3` 1, `prog4` 1, `prog5` 1, `prog6` 0; `q9` 0: as
  `heroes-gi`.
- The guard's switch: `p7` built with `P256_ROUTE=off P256_GUARD=1`: build 0, run **134**, the put's
  panic; with `P256_GUARD=0`: build 0, run 0 printing *"1"*. `pgrep -f` of the build folder afterwards: 0.

## 10. Verdict

- `verdict`: **object** (to P2 as worded, `gc`; and to `gr`). No veto: neither adds a core construct; the
  one form I would veto, a written bound, nobody proposed as the resolution (§ 4.4).
- `section`: design.md §1.7 and Part 5 (a rule erased in the frontend, not a construct the lowering and
  the backend must carry), §4.12 (*"No constraints. No `where`, no bounds"*), §1.12 through CLAUDE.md §
  Precedence as amended 2026-11-22 (the ceiling yields to the most robust resolution).
- `implementation_cost`: **gi**, checker only: `selfhost/check/instances.hero` new, 446 lines by `wc -l`
  with the chain (326 neither blank nor comment by my `awk`; the write hook counted 328 code lines before
  the chain, past the 300 ceiling: a landing splits it),
  `check/state.hero` +6, `check/walk.hero` +1, `check/function_value.hero` +1,
  `check/waiting_names.hero` +1, `check/thread_ops.hero` +5, `checker.hero` +5. **The guard**,
  `emit/crossing_bodies.hero` +12 (prototype's switch included). **Q1's number**: `runtime/parts/mail.c`
  +115, `runtime/heroes_runtime.h` +4 (built, C test), about 10 compiler lines across `inventory.hero`,
  `check/thread_types.hero`, `check/thread_ops.hero`, `emit/threads.hero` (priced, unbuilt).
  **gc** would be gi's spawn and type needs plus a body rule, and, to make its `@` true for a type with
  no handle, a lives-walk change in `check/moved_walk.hero` I did not build. **gr** is the body refusals
  alone, about 60 lines of the same module.
- `needed_for_self_hosting`: **no**. `selfhost/` declares one generic (`fetch/verb_place.hero:95`,
  `unit_of<T>`, no thread operation) and starts threads through an `extern`
  (`cli/link.hero:240`), never `spawn` or a mailbox.
- `argument`: gi is what § 11 already says, judged where the types exist: built, it refuses every probe
  at `check` (handle and `ptr` crossings, the unit mailbox's compiler panic), turns `prog3`'s and `prog5`'s
  run-time aborts into errors at the call, accepts `prog2`, and moves none of 140 programs (98 golden, 42 Rebel).
  gc's `@` empties nothing at `i64` (`7` read after, measured) while aborting at `Boxed`: one line, two
  meanings, and it refuses the frozen-legal `relay<A>`. gr refuses all four live generics and makes
  `poll.add_mailbox<T>` unwritable. The 12-line emitter guard is the run-time floor panel 155 asked for.
- `prediction`: if gi lands, the census of `check` over the tracked tree at the landing's batch gate
  (M-rebel-library) moves **zero** programs outside the cases added for defect 2695, and
  `check/instances.hero` with its split stays **under 500 lines** by `wc -l` at that commit.
- `condition`: I move to gc if a build makes a read after a letter's `m.send(@v)` a compile error in the
  body without refusing the retry on a failed send; to gr if a `poll.add_mailbox<T>` is writable under it;
  away from gi if, on a still machine, `heroes run tests/harness/latency.hero -- compare` reads gi slower
  past the floor of effect on `check` of a Rebel program, or a program gi accepts crosses a handle or a
  `ptr` unguarded at run time.

## 11. One line per question

- **Q2 item 1, the class**: five rules (send's `@`, handle into `spawn`, `ptr` by `spawn` or answer, written
  `(mailbox T)`/`(thread T)` reaching `ptr`, unit mailbox) closed; holds-itself unreachable through a letter;
  `e2` left to panel 155's guard. *Would change it*: a probe of a sixth rule a letter reaches.
- **Q2 item 2, the written-type hole**: judged as a need of the generic that writes the type, at every call
  (`p5c` refused at `main`'s call). *Would change it*: a mailbox type inferred in a non-generic body from
  no generic and no written type.
- **Q2 item 3, gc and gi built**: gi adopted; gc refused. *Would change it*: gc's `@` made true for `i64`
  without refusing the failure-path retry.
- **Q2 item 4, gc's `@` with no handle**: emptied nothing, `7` read after, `Boxed` aborts. *Would change
  it*: a lives-walk rule that decides it once in the body, built and run on `g4`.
- **Q2 item 5, gr**: refuses all four live generics and item 9's face. *Would change it*: a census where gr
  refuses nothing the tree needs.
- **Q2 item 6, after `mono` and a bound**: after-`mono` refused as primary (`check` never runs it), kept as
  the 12-line guard; a bound refused as a core form with no robustness gain over inferred needs. *Would
  change it*: `heroes check` made to run `mono`, measured cheap.
- **Q2 item 7, cost**: unrun (every run waited, load 29 to 114). *Would change it*: `latency.hero compare`
  past the floor of effect on a still machine.
- **Q2 item 7b, 155's obstacle**: the callee recorded at the three lines that already hold it, one table on
  `instantiations`' own key. *Would change it*: an instantiation recorded anywhere else.
- **Q2 item 7c, the emitter**: the guard closes `p7`/`p8` at run time in 12 lines; adopt it **behind** gi.
  *Would change it*: the ffi-pragmatist measuring that a `ptr` crossing corrupts nothing.
- **Q2 item 7d**: gi is a clarification of § 11 (a sentence saying where the error is told). *Would change
  it*: a program gi refuses that the frozen text, read per instance, accepts.
- **Q1 item 8, `m.number()`**: the number with the layer's watch; runtime half built and rung. *Would change
  it*: a sanitizer report from `hero_mail_watch(NULL)` racing sends under stress on Linux or Windows.
- **Q1 item 9**: `add_mailbox<T>(@set, m: (mailbox T)) -> i64?` checks and runs under gi and the frozen
  compiler, refused under gr. *Would change it*: nothing short of adopting gr.
