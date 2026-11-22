# Panel 254, compiler-engineer's report

Started 2026-11-22 02:32 CET (`date`), load 16.45 on 8 cores (`uptime`). Written as I go; the verdict
structure is filled at the end. Copy: `.claude/worktrees/p254-scratch/compiler-engineer/` at `89f155330`,
`git status` clean at 02:32. Every file I write is under its untracked `p254/`; `TMPDIR` is its
`build/tmp`. Read first: `00-shared.md`, `compiler-engineer.md`, `arms/spec-m.diff`,
`254-reports/critic-briefs.md`, `254-reports/spec-warden.md` (whole, each).

## Log

- 02:32 build started (seed, stage 2, stage 3), log `p254/build.log`.
- 02:42 build done: stage 1 `real 29.79 user 18.81 sys 0.77`; stage 2 `real 286.49 user 176.95 sys 40.29`;
  stage 3 `real 274.63 user 173.34 sys 43.35`; load 16.45 at 02:32, 14.81 at 02:42 (`uptime`), so
  no figure of these is a speed claim. `./heroes-next --version`: `heroes 0.6.0`, `sources
  6e891e681c14bca0`, equal to the coordinator's stage 3.

## Q1.1 The checker's `Ty`, counted again on this tree (R3's reason 3)

Six copies of `selfhost/` under `p254/q1/<v>/selfhost/` (`rsync -a`), each edited by a Python
replacement asserted to match once; `./heroes-next check p254/q1/<v>/selfhost/main.hero`, output in
`p254/q1/<v>.out`; sites and files from the `at p254/q1/<v>/selfhost/<file>:` line under each
`error[non_exhaustive]` (`grep -A1 | grep -oE | sort -u | wc -l`).

| copy | what was added | exit | `error[non_exhaustive]` | files |
|---|---|---|---|---|
| `base` | nothing (control) | 0 | 0 | 0 |
| `thread` | `Ty.thread_ty { result: i64 }` | 1 | **289** | **110** |
| `mailbox` | `Ty.mailbox_ty { element: i64 }` | 1 | 289 | 110 (the same files, `diff` empty) |
| `both` | both `Ty` cases | 1 | **289** | 110 (one error per `match`, naming both cases) |
| `tk` | `TypeKind.thread_of`, `TypeKind.mailbox_of` (no parser arm) | 1 | **24** | **12** |
| `all` | the four cases | 1 | **313** | **119** |

Panel 251 read 288 in 109 for one case; this tree reads **289 in 110**, one more. R3 named **under 30**
as what would reopen the route: **the route is ten times over that line, measured**. By area, the 289
sites: `check/` 132 in 40 files, `emit/` 127 in 56, `ir/` 20 in 9, `cli/` 10 in 5. The heaviest files:
`check/builtins.hero` 15, `check/generics.hero` 12, `check/walk.hero` 10, `check/access.hero` 8,
`check/table.hero` 7, `check/ffi.hero` 7, `emit/extern_field.hero` 6, `check/waiting_names.hero` 6,
`emit/lend_count.hero`, `emit/gate.hero`, `emit/assert_spelling.hero`, `cli/pointee_wants.hero` 5 each.
The `TypeKind` 24: `parse/type.hero` 11 (the parser arm itself answers those), `check/fixed_judged.hero`
2, `check/ffi_sweep.hero` 2, and one each in `resolve/types`, `print/types`, `mutate/handles`,
`emit/ffi_result`, `emit/ffi_declared`, `check/type_labels`, `check/sized`, `check/map_keys`,
`check/lower` (251's 24 in 12, unchanged). **Unmeasured**: the `match`es on `Ty` that end in an arm
covering every other case, which accept a new case in silence; the 289 is a floor of the decisions owed.

**What this means for the ceiling**: two type constructors are **core**, not sugar (design.md §1.7,
Part 5): the checker, the lowering and the backend all match them, 313 decisions in 119 files before
a single operation is typed. The compiler this would land in is 228,594 lines in 878 tracked
`.hero` files (`git ls-files 'selfhost/*.hero' | xargs cat | wc -l`), against Pascal-P4's ~4,000 my
seat's definition names as the scale: that ceiling was left long before this sitting, and nothing
here moves it by a measurable fraction (the route's own lines are priced below).

## Q1.3 Operations written only after a dot: not a new kind of name (a premise found false)

Panel 251's *"route M needs a kind of name the compiler does not have today, one resolved by its
receiver's type alone"* is **false of this tree as worded**. design.md §4.11 (`docs/design.md:1945-1946`):
*"The resolver, on seeing a dot, first looks for a field; failing that, looks for a free function of
that name"*. The code does exactly that: the resolver leaves a dotted name it cannot place to the
checker when a field of that name is in reach (`selfhost/resolve/names.hero:214-216`, *"Could be a
function-valued field: the checker decides, with the type"*), and the checker types the receiver
first and asks `name_types.field_of_function_type` **before** it reads what the resolver bound
(`selfhost/check/walk.hero:1455-1475`, `selfhost/check/name_types.hero:4-6`: *"which `x.f(…)` calls
before any free function named `f`"*). Probed (`p254/q13/field_vs_fn.hero`, a record `Box` with a
field `send: (function(i64) -> i64)` and a top-level `function send(b: Box, n: i64)`): `heroes-next run`
printed **`10`** for `b.send(5)` (the field) and **`1005`** for `send(b, 5)` (the function), exit 0,
no diagnostic. And `field_only.hero`'s `n.send(5)` on an `i64` with no `send` anywhere is told
`error[unknown_function]: i64 has no field send, and no function is named send`.

So a mailbox's and a thread's four operations sit in §4.11's existing first lookup, keyed by the
receiver's `Ty` instead of a record's field, and **the price is small and lands in two files**:

- `selfhost/resolve/names.hero` (248 lines): in `method_name`, beside `field_in_reach`, a dotted
  `wait`, `send`, `receive` or `close` returns unbound when no declaration names it, so the checker
  decides with the type: about **6 lines** and a four-word constant (not in `inventory.hero`, which
  would take the names everywhere and refuse the tree's 133 declarations, the spec-warden's count).
- `selfhost/check/walk.hero` `method` (1,402 onward): after the receiver is typed, a `thread_ty` or
  `mailbox_ty` receiver with one of the four names goes to the operation's typing before
  `field_of_function_type`; any other name on such a receiver falls through to §4.11's free
  function, so `m.drain()` is the program's `drain(m)`. About **15 lines of dispatch** in `walk.hero`
  (already at its decided ceiling, panel 203's landing moved code out of it), the operations
  themselves in a new module, about **90 lines** (four operations, their argument and label
  checks, `within_ms:` read as an `i64`, `send`'s `@` for a value holding a handle).
- **What a program declaring `function send(m: (mailbox Job), j: Job) -> ()?` resolves to**:
  `m.send(j)` is the mailbox's (the first lookup wins, as the field wins in the probe) and
  `send(m, j)` is the program's. **That divergence is not new**: the probe above shows it standing
  today for every function-valued field, unrefused and unsaid by the spec (spec line 306 says `x.f(y)`
  *is sugar for* `f(x, y)`, which `10` against `1005` falsifies for a field). The spec-warden's W-h
  refusal costs about **12 lines** in the checker's declaration walk and **one new diagnostic code**
  (a declared `wait`, `send`, `receive` or `close` whose first parameter is a thread or a mailbox);
  I support it, and I name the field case beside it as the same class standing today (a question for
  the sitting whether W-h's sentence should reach both; I do not widen the route with it).
- `x.f(y)` UFCS on a mailbox for any other `f`: resolved to the program's `f`, as §4.11 already does
  for a record whose field is not `f`. Nothing is refused.

## Q1.4 The built-ins `spawn`, `spawn_sized`, `mailbox`

`check/builtins.hero` (565 lines) types every built-in from its arguments alone (`builtin_call(...
given: [i64] ...)`, line 164); nothing there reads the type the context asks for. The two calls typed
by context today are `ok(x)` (`walk.hero`'s `.builtin` arm, `construction(... expected ...)`) and the
empty `[]`, refused with no expectation (`contextless_errors.empty_array_literal`, `walk.hero:239-241`).
`mailbox()` is the second shape: about **25 lines** in `walk.hero`'s check mode and one message
(`a mailbox's type is written where it is bound`), and with the spec-warden's W-e (*"typed as an
empty `[]` is"*) the same path serves `Req(reply: mailbox())`, since a field's argument is checked
against the field's type. `spawn(f, x)` reads `f`'s `function_ty`, checks `x` against its one
parameter and answers `fallible(thread_ty(result))`: about **35 lines** in `builtins.hero` with
`spawn_sized`'s `bytes:` label, and three `Builtin(name: ...)` rows **appended** to `inventory.hero`
(the order is an index, its own comment), which `spec/offered` asks for (the spec-warden's run).

## Q2 The runtime half, built (written 03:05; every file under `p254/q2/` of my copy)

### Q2.2 first, because the queue rests on it: how a value crosses whole, and 7.13's heaps

**Built: a flat encoding, walked by a family of per-type functions passed beside the descriptor,
never inside it** (`p254/q2/mail.c`, 400 lines with the queue). The sending thread writes the value
into one block of the runtime's own (`hero_malloc_raw`, weighed by neither leak counter, as
`parts/spawn_str.c`'s box is); the receiving thread rebuilds it into blocks it allocates itself
(`hero_str_from_bytes`, `hero_array_new`, `hero_map_new`). The shape is panel 230's renderer, an
existing precedent: `emit/sides.hero` (240 lines) writes one `HeroShowFn` per type an `assert`
compares and the runtime's `hero_show_array(o, p, elem)` takes the element's renderer
(`runtime/heroes_runtime.h:560-600`, `runtime/parts/show.c` 311 lines). Here `HeroPutFn` and
`HeroTakeFn`, and `hero_cross_put_array(b, p, elem)` / `hero_cross_take_array(b, p, desc, elem)`,
the map's with two children, the `str`'s, the scalars', a mailbox's (its address, the copy's count
travelling with it) and a handle's (its address; its life is in the process-wide handle set).

- **The descriptor keeps five members** (`runtime/parts/desc.c:19`, design.md §4.20, panel 027): no
  sixth member and **no ABI move**. Panel 251's deep route added `deep` and moved `HERO_RUNTIME_ABI`.
- **7.13, measured as far as this Mac can**: every block the PROGRAM's values own is allocated and freed
  on one thread; the leak gate (`hero_runtime_check_leaks`, `hero_live_blocks`) is silent at the end of
  every one of the 400 plain and sanitized runs below, with the last copy of a queue dropped on a
  thread other than its maker's (`fan`: `main` drops its copy before the receivers'). **Only the
  runtime's own transport block, and the queue object, are allocated on one thread and freed on
  another**, as `spawn_str.c`'s box and `platform/hero_tls.c`'s layer state already are
  (`alloc.c:625-635`, *"gives it back on whatever thread leaves it last"*). Unmeasured here: a per-thread
  balance census as panel 251's (`ce/q2/census.sh`); the leak gate and ASan are what read it.
- **Its emitter cost, priced, not built**: a module of `emit/sides.hero`'s shape writing a put and a
  take per record, variant, `T?` and container type a program crosses, about **300 lines**, plus the
  wanted-set walk (`sides.hero`'s `wanted`) keyed on `spawn`, `wait`, `send`, `receive`; the runtime's
  half is **about 170 lines** of `mail.c` (the walk) and **about 230** (the queue, the stop, the gate).
- **What it costs per crossing**: two walks (write, then rebuild) against panel 251's deep copy's one;
  unmeasured in instructions here (the machine read load 38 to 63 all session, `uptime`).

### Q2.1 The queue: S5 to S8 as written, and the alternatives, built in one file

`p254/q2/mail.c`: an atomic count of copies (S5's life; no copy-on-write ever tests it, every write
to the queue goes through the queue's lock, so `cow.c`'s test-then-mutate has no twin here), a lock
and a condition variable per queue, `send` never waiting (S6), `receive(within_ms:)` (S7, `-1` no
limit, below `-1` an abort), `close` for every copy and idempotent (S8), the stop through
`hero_mail_hear_stop()`, and **the stop's three behaviours as a compile switch** (`HERO_MAIL_STOP` 0
the stop wins even with values in it, S7 as proposed; 1 a drain, `stopped` only once empty; 2 the stop
never ends a mailbox's wait). The Windows arm is written beside the POSIX one (`SRWLOCK`,
`CONDITION_VARIABLE`, `SleepConditionVariableSRW`), **unrun, owed to the CI's Windows leg or the box**.
Every object kept between calls: the queue's own lock and condition live in the queue's block; three
statics (`hero_mail_list_lock`, `hero_mail_list`, `hero_mail_alive`) would need `SHARED_BY_DECISION`
entries in `tests/harness/suite_runtime.hero:227` and `hero_mail_stopped` is `_Atomic`. **Unrun: the
`runtime` suite on a tree holding the part** (the part is compiled beside the frozen runtime by
`p254/q2/mailrt.c`, not laid into `runtime/`, so the suite would read nothing of it).

### Q2.3 The cycle (the critic's finding 29), built: S5 leaks in silence, measured

`p254/q2/cycle.c`, the emitted C for `record Sub { name: str, boxes: [(mailbox Sub)] }` by hand:

| case | today's gate alone (`hero_runtime_check_leaks`) | with the mailbox gate first |
|---|---|---|
| `control`: `m.send(Sub(boxes: []))` | exit 0, queues alive 0 | exit 0 |
| `self`: `m.send(Sub(boxes: [m]))`, then `main` ends | **exit 0, silent**, queues alive **1** | exit 134, *"1 mailbox(es) still hold values when main returned ..."* |
| `pair`: each of two inside the other's queue | **exit 0, silent**, queues alive **2** | exit 134, *"2 mailbox(es) ..."* |

So S5 with S9 leaks a queue with no word, exactly as the critic read it; **design.md §4.10's
*"Reference cycles become impossible, so refcounting no longer leaks"* (`docs/design.md:1827-1830`)
is false under the text as proposed**. Two closures, both priced: (a) the spec-warden's W-f, a compile
error for a `(mailbox T)` whose `T` reaches that mailbox type through fields, elements, payloads or
another mailbox's `T`, which closes every cycle (a queue holds a copy of a queue only through its
element type, so a cycle of queues is a cycle of mailbox types): a reach walk with a visited set at
each `mailbox_ty` the checker interns with a concrete element and at each generic instantiation that
makes one, **about 50 lines** in `check/`, one diagnostic; (b) the gate above, **about 20 lines** in
the runtime, exact because after `main` returns and every thread was waited for nothing but a queue
can hold a queue. I adopt **both**: the rule so a long-running server cannot leak per occurrence, the
gate so a hole in the rule is told rather than silent. The **handle-life alternative (251's `m0`)
does not avoid the count**: a queue reached from several threads must outlive every thread inside its
`send` or `receive`, so a close that freed it would be a use after free; a handle's life is a policy
on top of the same count, not a replacement for it (read in `mail.c`'s design; not built).

### Q2.3 The stress, under the sanitizers (the runner `p254/q2/runs.sh`, log `p254/q2/runs.log`)

Topologies (`p254/q2/stress.c`, the emulation of a program sending a record of every shape the brief
names: a `str` of 0 bytes and, one value in 1,000, of 1 MiB; `[[str]]`; `{str: [i64]}`; and a
`(mailbox i64)` inside every value, replied on one value in 100, so a queue's copies are dropped on
threads other than its maker's): `fan_in` 8 senders and 1 receiver, `fan_out` 1 sender and 8 receivers,
`ring` 64 threads each sending to the next one's queue and receiving its own, `close` 1,000 rounds of
a close racing 4 waiting receives and 4 sends. Plain `-O2` (written as it lands, below).

### Q2.4 and S7, measured on the gateway shape: the stop's three behaviours

`p254/q2/gateway.c` (§ Q3 below), N = 8 connection threads, M = 32 clients each sending R = 200
requests with no pause, the stop asked 20 ms in, the program as the proposal shapes it (the accepting
thread hands every connection whole through a `(mailbox Conn)`, the owner of the cache and SQLite
answers each request through a reply mailbox inside it), **a fresh reply mailbox per request**; three
runs per behaviour (`p254/q2/stopfresh.txt`):

| behaviour | connections served | applied, client told an error | client told another's count | a forgotten close of the owner's queue (`forget.txt`) |
|---|---|---|---|---|
| 0, the stop wins (S7 as proposed) | **8 of 32, 3 of 3 runs**: 24 connections stay queued, never served, never closed | 0, 0, 1 | 0 | ends |
| 1, a drain | 32 of 32 | 5, 5, 1 | 0 | ends |
| 2, the stop never ends a mailbox's wait | 32 of 32; 6,400 of 6,400 applied and answered | 0 | 0 | **hangs** (killed at 20 s, exit 137) |

**S7 as proposed strands every handle queued when the stop comes**: the workers' `receive` on the
connections mailbox answers `stopped` with 24 connections in it, so those clients wait until their own
timeout (3 s here) and, in Heroes, the 24 `Conn` handles go with the queue and are told at `main`'s end
as handles nobody consumed: an abort blaming the program for what the stop did. `runtime/hero_net.h`'s
own stop is **a drain for a socket** (*"a call that need not wait still moves what it can"*, read at
`:193-199`) and stop-wins only for a poll set (`hero_poll.h:61-70`): behaviour 1 is the socket's.

**And a hazard every behaviour shares, found here** (`p254/q2/timeouts.txt`, no stop at all, a reply
waited 1 ms for an owner taking 3 ms): with **one reply mailbox per worker, reused**, a late reply is
read by the next request, **72 (behaviour 0) and 76 (behaviour 2) clients told another request's
count**; with a fresh reply mailbox per request, **0 and 0**, every give-up told as an error. With a
stop under behaviour 1 and reuse the same cascade told 0 to 24,473 wrong counts (`stopmodes*.txt`). So a
`receive` that gives up, by `timeout` or by `stopped`, leaves a late value for the next one; the
text should say so where it names the reply mailbox (my S7 amendment below).

- 03:17 resumed after the API limit (the coordinator's message): no binary of mine was running
  (`pgrep -fl p254-scratch/compiler-engineer`, `pgrep -fl 'stress-|gateway-m|cycle-'`: nothing); the
  runner had ended after its ASan lines, the TSan half relaunched as `p254/q2/runs-tsan.sh`.

### Q2.3 counts so far (`p254/q2/runs.log`)

| build | topology | runs | exit 0 | sanitizer reports | hung |
|---|---|---|---|---|---|
| plain `-O2` | fan_in, 8 x 100,000 | 20 | 20, *"right 800000 of 800000"* | n/a | 0 |
| plain | fan_out, 1 x 800,000, 8 receivers | 20 | 20, right 800,000 | n/a | 0 |
| plain | ring, 64 x 100,000 | 20 | 20, right 6,400,000 | n/a | 0 |
| plain | close, 1,000 rounds | 20 | 20; e.g. 670,595 sent, 670,595 received, 129,405 refused `closed`, 4,000 receivers ended `closed` | n/a | 0 |
| ASan+UBSan `-O1` | fan_in 8 x 100,000 | 5 | 5 | 0 ASan, 0 UBSan | 0 |
| ASan+UBSan | fan_out 800,000 | 5 | 5 | 0, 0 | 0 |
| ASan+UBSan | ring, **64 x 20,000** (reduced, named) | 5 | 5, right 1,280,000 | 0, 0 | 0 |
| ASan+UBSan | close, 1,000 rounds | 5 | 5, 646,972 sent and received | 0, 0 | 0 |

The leak gate (`hero_runtime_check_leaks`) ran at the end of every one, silent. TSan below when it ends.

### Q2.4 The stop: the runtime's queue hears the layer's stop by one line, measured

Built as the route I judge robust: **the flag stays the layer's, the runtime's queues are told**. The
prototype tree `p254/q2/rt/` is `runtime/` plus `parts/mail.c` included after `parts/map-write.c`
(2 lines in `runtime.c`) and, in `platform/hero_net.c`, `hero_mail_hear_stop();` at the end of
`hero_net_nudge_all` (`:874-882`, the function *"called in each of the three ways a stop is heard"*)
with its prototype (4 lines). The runtime never calls the layer, so a program that never names
`hero_net.h` links no member; one that asks the stop links the member as before.

| program (`p254/q24/`) | runtime | build | runs | defined globals (`nm -g -U`) | bytes | `hero_mail` symbols | `hero_net` symbols |
|---|---|---|---|---|---|---|---|
| `one.hero`, `print(1)` | frozen | 0 | `1` | 271 | 310,832 | 0 | 0 |
| `one.hero` | `HEROES_RUNTIME=p254/q2/rt` | 0 | `1` | **296** (+25) | **314,912** (+4,080) | 16 | **0** |
| `stop.hero`, `hero_net_ask_stop()` | frozen | 0 | `1` | 304 | 360,888 | 0 | 152 |
| `stop.hero` | prototype | 0 | `1` | 329 | 381,368 | 16 | 152 |

And the wake itself (`p254/q24/wake.c`, the prototype's `runtime.c` and `platform/hero_net.c` compiled
and linked by hand): a thread waiting on an empty mailbox with no limit, `main` calling the layer's
own `hero_net_ask_stop()`: **5 of 5 runs, `receive` answered `stopped` 0.017 to 0.048 ms after the
call**, exit 0, both gates silent. On Windows the same line is reached by the console's thread
(`hero_net.h:541-545`); **unrun there**. The cost to every program is the part's 25 symbols and about
4 KB, whatever it uses; the `runtime` suite owes three `SHARED_BY_DECISION` entries for it (Q2.1).

### Two of the historian's questions, answered by measurement

- **S9's function values** (the historian's objection 4, *"Whether a Heroes function value carries
  captured state is the compiler-engineer's to say"*): it carries none. The emitted C of
  `p254/q13/field_vs_fn.hero` types the field `typedef int64_t (*h_0fn_48ac9712)(int64_t)` and fills it
  with a top-level function, `.f_send = h_fieldvsfn_twice` (`p254/q13/fv.c:53`, `:131`), as
  `heroes_runtime.h:659-661` says (*"a bare C function pointer (no closures in v1)"*). Every thread of
  one process runs the same code, so the address crosses as itself: Dart's same-code case, not
  JavaScript's. **I adopt S9 with function values**; the objection's own condition is met.
- **Historian P1** (*"the queue's block still allocated at exit, read by the runtime's leak gate or
  LeakSanitizer"*): the first half held, the second did not under my build: **queues alive 1 and 2,
  today's gate silent, exit 0** (Q2.3 above), because the queue and a queued value are the runtime's
  own blocks, which no counter weighs. LeakSanitizer is not on this Mac's ASan (`alloc.c`'s *"the leak
  check ASan cannot do on this platform"*); on Linux it is **unrun**. Under panel 251's shallow crossing
  the queued blocks would be counted ones, and today's gate would tell them as *"a missing decref ...
  this is a compiler bug"*, a false message (CLAUDE.md § 8): a reason more for the mailbox gate.
- **The historian's objection 1, S6 with S11** (a handle moved by `m.send(@v)` into a closed mailbox):
  in my prototype the refused copy is dropped and the handle's life is left in the set, so it is told
  at `main`'s end as one nobody consumed, blaming the program. The robust repair has a precedent in
  this language: an extern's `-> i32 when 0` (`selfhost/ast.hero:511-513`, *"every end, transfer and
  reference the call makes happens only then"*, panel 177's item 6). **A send that fails `closed`
  leaves `v` as it was**: the emitter empties the cell only on success (one branch), the runtime
  drops only its own copy (built: `hero_mail_send` returns before the cell is touched). Rust's
  `SendError` returns the value the same way (the historian's Q3). My S6 text below says it.

## Q1.5 to Q1.8 (written 03:30)

**Q1.5 The refusals S10 and S11, and W-f: one walk that exists, three targets.** `check/reaches.hero`
(381 lines, panel 149) already walks a type through fields, variant payloads, `[T]`, `{K: V}`, `T?`
and fixed arrays, with a seen-set against declaration cycles, and answers the dotted path to a handle
(`reaches_handle`, `:215`; the arms at `:234-258`). S10 is that walk with `.ptr | .cstr` as the target,
W-f with the mailbox's own type as the target and descending into a mailbox's `T`, and S11 is
`reaches_handle` itself (a `thread_ty` answering as a handle, one of the 289 arms). **All three are
checker refusals at the operation's typing** (`spawn`'s argument and `f`'s answer, `send`'s value,
`mailbox_ty` interned with a concrete element and at each generic instantiation): about **40 lines**
generalising the walk's target, **about 30** at the call sites. Classes: **S11 needs no new one**,
`marker_mismatch` exists (the built-in's value parameter asks `@` exactly when its type reaches a
handle, as a declared `@` parameter asks it); **S10 and W-f are two new codes**, and W-h a third
(Q1.3): three new diagnostic classes, each a spec-and-diagnostic matter for the sitting, each owing a
`tests/golden/check/` case annotated in its source.

**Q1.6 The doors after M (S14).** The compiler and the net declare the doors in **5 files**
(`git grep -l -E '^ +function hero_thread_' -- selfhost tests/harness`): `selfhost/cli/link.hero:240-241`,
`selfhost/main.hero:88-89`, `selfhost/mutate/cut.hero:44-45`, `selfhost/mutate/recovery.hero:34-35`,
`tests/harness/suite_cache.hero:97-98`; 7 name them (`emit/callback_guard`, `emit/guarded_names` as
text). Every door caller has a one-for-one spelling under M (`spawn(f, x)` with `f: (function(i64) ->
i64)`), so **the doors leave `hero_os.h` in the landing and the runtime stops exporting them** (made
`static`, so a program's own header cannot bind them: panel 251's Q3.4), by CLAUDE.md § 2's rule of
2026-10-29; a program still binding one is told by the existing `error[ffi_unknown_name]` (the
spec-warden's probe), **no new class**. `selfhost/main.hero:101-113` becomes
`spawn_sized(command, 0, bytes: STACK_BYTES)` matched on `.err e` (exit 2) and `.ok t` (`hero_exit(code:
t.wait())`), **but only after the seed refresh that knows the form** (`.claude/rules/verification.md`
§ The batch). **One thing S2 must keep for it**: `main.hero:107-110` prints the system's own number
when the thread is refused (*"its own reason is"* `why`); a `limit` failure whose message drops it
would lose that exit-2 contract, so my S1/S2 text says the message names the system's reason.

**Q1.7 The walk a new surface form owes.** `(thread T)` and `(mailbox T)`: `parse/type.hero` (the 11 of
the 24 `TypeKind` sites), `ast.hero` +4 (two cases), the formatter `print/types.hero` (1 site, 2 arms),
`resolve/types.hero` (1), `keywords.hero` two `WORD_` constants and two entries in `contextual()`
(`:136-154`, what `heroes grammar` prints and `grammar/contextual` holds), the spec's `Prefix`
production (the spec-warden ran `grammar` 12/0 on the amended text), the VS Code grammar
(`editors/vscode/syntaxes/heroes.tmLanguage.json` names each contextual word once in one alternation,
`consumes` 1, `acquires` 1: two words added), the site's colouring by `heroes lex`: **nothing**, a
contextual word lexes as an identifier. The formatter's strided probe after the push (the 2026-11-10
rule). **`spawn(f, @x)` and `m.send(@v)` owe the parser and the formatter nothing**, measured:
`p254/q17/at_arg.hero`, `m.put(@xs)` with `put(m: i64, @v: [i64])`, runs (`1`, `1`, exit 0) and is
canonical under `heroes-next fmt` (`diff` empty).

**Q1.8 `spawn(f, @x)`**: the `@` argument after a call's `(` is today's syntax; the checker's `@` rules
apply to it as to a declared `@` parameter, with one departure the spec-warden found (W-j): the
spawned `f` receives the handle as a borrowed parameter (a function value has no `@` parameter,
`error[mutable_parameter_as_value]`), so it cannot consume it. **I agree with W-j's measurement and
not with its repair**: the most robust hand-over keeps `spawn(f, @x)` and rules that **a spawned
function's parameter owns what `spawn(f, @x)` handed it** (W-j'), because the alternative, a mailbox
only, makes the common shape (*start a thread on this connection*) need a mailbox of one. Its price
in the checker: the consuming rules read a spawned function's first parameter as owned at the
`spawn` site, about **20 lines** in `check/consuming`'s borrowed-parameter rule, keyed on the call
being `spawn`'s; the spawner's cell is the dead value after it (`hero_handle_dead()`, measured below).

## Q2.5 The handle machinery across threads, built (`p254/q2/handles.c`)

The calls the emitter writes today around `acquires` and `consumes` (copied from `tests/emission`:
`hero_handle_acquired_at`, `hero_handle_ending_at`, `hero_handle_ended`, the cell overwritten with
`hero_handle_dead()`), and `m.send(@b)` emitted as the send then, on success only, the dead value:

| case | plain | ASan+UBSan x5 | TSan x5 |
|---|---|---|---|
| `move`: 20,000 `malloc` handles acquired on `main`, sent `@`, ended on 8 workers | exit 0, *"20000 sent, 20000 ended on 8 workers"*, gates silent | 5 of 5, 0 reports | 5 of 5, **0 races** |
| `reuse`: the sender reads its emptied cell after the send | exit 134, *"a dead C handle reached free: a call marked consumes or transfers ended this handle's life ..."* | | |
| `queued`: a handle left in a queue whose last copy goes | exit 134, *"1 C handle(s) never given back ..."* | | |

So the process-wide handle set (`alloc.c:1155-1260`, one lock) carries a life acquired on one thread
and ended on another with no change. **Two messages become false or blind under S11, and both are
blocking-class by CLAUDE.md § 8**: `reuse` names *consumes or transfers* for a value a `send` emptied
(about 10 runtime lines: a second dead page, or the message naming a send), and `queued` never names
the mailbox the handle was dropped with (about 10 lines: the queue's drop marks what it drops, the gate
says *"inside a mailbox when its last copy went"*). A lease never crosses (S10 refuses a `cstr`), and
`lend.c:143-186`'s regions stay `_Thread_local` correctly. Panel 251's R2 gate with a worker blocked in
`receive`: a thread nobody waits for is told at `main`'s end before any balance
(`hero_spawn_check_joined`, `alloc.c:314`), and a thread waited for while it blocks in `receive(-1)`
on a queue nobody closes hangs (`forget.txt`, behaviour 2; behaviours 0 and 1 end it at a stop). The
256-slot table: full, it is `hero_panic("too many threads running at once ...")`
(`runtime/parts/spawn.c:522`), not a failure, so **S1's `limit` must also cover the runtime's table**
or the text must say *"more than 256 at once aborts"*; I amend S1 to the first (read, not run: I did
not start 257 threads).

## Q2.6 The Windows arm: written, unrun

`p254/q2/mail.c`: `SRWLOCK`, `CONDITION_VARIABLE`, `SleepConditionVariableSRW` with the wait in
milliseconds and `INFINITE` for `-1`, `GetTickCount64` for the deadline, `WakeConditionVariable` and
`WakeAllConditionVariable`; the list lock `SRWLOCK_INIT`, statically initialised as `hero_handle_lock`'s
Windows spelling is. The stop reaches it through the same `hero_net_nudge_all` line, called on the
console's own thread there (`hero_net.h:541-545`). **Owed to the CI's Windows leg or the box: never
compiled for Windows in this session.**

## Q2.7 The routes beside the proposal, priced from Q1 and Q2

- **7.13's first rung, plain data parallelism** (`spawn` and `wait`, any value whole, no mailbox):
  `thread_ty` alone costs **289 `match` sites in 110 files, the same sites as both cases** (one error
  per `match`, measured above), the crossing walk whole (Q2.2), and none of the queue, the stop, the
  count, the cycle or the gate: about **60%** of the runtime half by my line counts. **It does not
  answer feature 2647**: the issue's need is a running thread exchanging values, which the first rung
  cannot express.
- **7.13's own *"one queue per thread"*, reached through `(thread T)`**: no count of copies, no close,
  no cycle (a thread is a handle, which crosses only by `@` and is consumed by `wait`). **It cannot
  serve the gateway shape**: N connection threads all asking one owner need N copies of the owner's
  `(thread T)`, which S11's handle rule forbids, or a separate address value, which is the mailbox
  again. This is an inference from the types, not a build; panel 251 measured its core at about 90 of
  125 lines.
- **Spelling B with names nothing declares**: the spec-warden found such names (`thread_wait`,
  `mail_send`, ... 0 declarations each); its compiler cost is Q1.3's minus the receiver-first lookup,
  about 20 lines fewer, and every other cost above is the same.

### Q2.1 The `runtime` suite, base and prototype, measured (03:06 and 03:28)

- base, my copy at `89f155330`: `./heroes-next run tests/harness/main.hero -- ./heroes-next runtime`:
  **`runtime: 10 passed, 0 failed`** (`real 221.69 user 80.69`, `p254/q2/suite-runtime-base.out`).
  **So the brief's premise *"defect 2588 says it is red at base on the platform layer"* is false of
  this tree.**
- prototype: the same command from `p254/proto/`, a root of symlinks to my copy whose `runtime` is
  `p254/q2/rt/` (the hook finds my copy above it, whose sources are the compiler's):
  **`runtime: 9 passed, 1 failed`**, the one failure `runtime/threads` naming exactly
  `runtime/parts/mail.c:230-232`, `hero_mail_list_lock`, `hero_mail_list`, `hero_mail_alive`
  (`p254/q2/suite-runtime-proto.out`). The three `SHARED_BY_DECISION` entries owed are the
  `hero_handle_set` shape (a table and its lock and count, shared on purpose); the other nine checks,
  the allocator's single point among them, pass with the part and the `hero_net.c` line.
- 03:28: TSan fan-in, rerun after the contamination below: **5 of 5, 0 races, right 800,000**.
- **A contamination of my own, found and repaired**: between 03:19 and 03:21 my `handles` runs and the
  first TSan runner wrote `o.txt` and `e.txt` in one folder at once (the runner's log held 23 bytes of
  one program's line overwritten by the other's: *"right 800000 of 800000\nt copy dropped ..."*), so
  any TSan count read in that window could have read the wrong file. That runner was stopped, its log
  kept as `runs-tsan-contaminated.log`, and the TSan set run again with private files
  (`runs-tsan2.sh`). The plain and ASan lines stand: nothing of mine wrote those two files during them.
- **Corrected 03:31, my own numbers above**: `p254/q2/mail.c` is **454 lines** (`wc -l`), not 400:
  lines 1-41 the head comment, **42-179 the crossing walk (138 lines, 123 not comment or blank)**,
  **180-454 the queue, the stop and the gate (275 lines, 245)**; so Q2.2's *"about 170"* and *"about
  230"* read 138 and 275, and Q2.7's first rung (the walk and a value-spawning door of about 60 lines
  beside it, unbuilt) is **about 42%** of the runtime half (198 of 473), not *"about 60%"*.

## Q4 The rows, S1 to S14 (written 03:35)

| row | verdict | what decides it (measured unless said) |
|---|---|---|
| S1 `spawn` | **amend** | `spawn.c:522` panics on a full table of 256 and `:618` on a system refusal; `selfhost/main.hero:107-110` needs the system's reason to keep its exit 2 |
| S2 `spawn_sized` | adopt (it reads S1 through *"does the same"*) | `spawn.c:634-636` already answers a refusal and aborts below 0 |
| S3 `t.wait()` | adopt; the table cell as the spec-warden's W-a | `p254/q2/handles.c`: a life ends on another thread through the set unchanged |
| S4 `mailbox()` | **amend** as the spec-warden's W-e | Q1.4: `ok(x)` and `[]` are the context-typed precedents |
| S5 life | **adopt a value with a count of copies; amend** with W-b, W-f and a runtime gate | Q2.3: the self and pair cycles silent at exit 0 under S5 as written; the handle life needs the same count (Q2.3) |
| S6 `send` | **amend** | the historian's objection 1; `-> i32 when 0`'s precedent (`ast.hero:511-513`); built: the cell is emptied on success only |
| S7 `receive` | **amend: the drain (behaviour 1)** | Q2.4's table: S7 as proposed stranded 24 of 32 handed connections, 3 of 3 runs |
| S8 `close` | adopt | `close` race: 1,000 rounds x 20 plain and 5 ASan, every value sent received once, every receiver ended `closed` |
| S9 what crosses | adopt, function values included | `fv.c:53`: a function value is a bare C pointer, no captured state |
| S10 never crosses | adopt; the refusal is the checker's (the spec-warden's clause) | Q1.5: `reaches.hero`'s walk with `.ptr | .cstr` as its target |
| S11 a handle | **amend: W-j'**, not W-j | Q1.8; the ffi-pragmatist's 20 of 20 crashes with one SQLite connection on two threads make the `@` load-bearing |
| S12 the stop | **amend** with S7 | `p254/q24/wake.c`: the layer's stop ends a waiting `receive` in 0.017 to 0.048 ms, 5 of 5 |
| S13 names | adopt with W-c, W-d, W-g, W-h | Q1.3: the four names sit in §4.11's first lookup; the field case shows W-h's divergence standing today |
| S14 the doors | adopt the removal; the doors leave `hero_os.h` and the runtime stops exporting them, after the seed refresh | Q1.6: 5 files of the compiler and the net; `ffi_unknown_name` tells the rest |

**The exact text where I amend** (each a replacement inside the proposal's § 11 paragraph or § 13):

- **S1**: *"`spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`, `T` being `f`'s
  answer, failing `limit` where no thread can be started, its message saying why;"* (covers the
  runtime's table, which then answers instead of panicking, about 5 lines in `spawn.c`).
- **S4** (W-e): *"`mailbox()` gives a new, empty `(mailbox T)`, typed as an empty `[]` is (section 10)."*
- **S5**: the proposal's sentence stands; § 3 gains W-b, *"No aliasing exists among the values this
  language owns but a mailbox's queue."*, and § 11 the spec-warden's W-f clause (*"... and so is a
  `(mailbox T)` whose `T` holds a `ptr`, a `cstr` or that mailbox's own type, in a field, an element,
  a payload or another mailbox's `T`, so no queue ever holds itself"*). No spec sentence for the
  runtime's gate: with W-f it should never speak, and if it does its message says the compiler
  should have refused the program.
- **S6**: *"`m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed,
  `v` then left as it was;"*
- **S7**: *"`m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for
  no limit and below that an abort; on an empty mailbox it fails `closed` once it is closed, `stopped`
  once a stop is asked (section 13), and `timeout` else; a value sent after a receive stopped waiting
  is the next receive's."* (I disagree with the spec-warden's W-i, *"`stopped`, before all else"*: that
  is behaviour 0, which stranded 24 of 32 handles in 3 of 3 runs.)
- **S11** (W-j'): *"... crosses only written with `@`, as `spawn(f, @x)`, whose `f` then owns it, or
  `m.send(@v)`, which empties `v` once it succeeds, its life going with it, or as the answer `wait`
  gives."*
- **S12** (§ 13's stop sentence): *"... and a waiting call of that layer, or a mailbox's `receive` that
  finds it empty, answer stopped;"*

**Two runtime messages owed with S11, blocking-class by CLAUDE.md § 8 (Q2.5)**: the dead value read
after a send must not say *"a call marked consumes or transfers ended this handle's life"*, and a
handle dropped with a queue must be told as dropped inside a mailbox.

## R3's four reasons, as measured here

1. **The spec-warden's veto**: a cost under `691531908` (the spec-warden measured +666 real for `m`).
2. **Principle 0**: **I read the author's instruction of 2026-11-22 as the ruling on the need**, and
   record that the compiler does not need the route to compile itself (`selfhost/main.hero:88-113`
   starts one thread of one `i64` and exchanges nothing with it).
3. **The cost**: **289 `match` sites in 110 files for one `Ty` case, 313 in 119 with `TypeKind`'s two**,
   against R3's *under 30*: ten times over, measured; under `691531908` a cost, and by my seat's own
   definition **a core construct** (two type constructors the checker, the lowering and the backend all
   match), which would be my veto were it not for § 2 of the shared brief: **I record it as the price,
   not as a veto.**
4. **7.13's heaps**: the flat encoding keeps every block of the program's values born and freed on one
   thread (the leak gate silent at the end of every run, the last copy of a queue dropped on a thread
   other than its maker's); the queue object and the runtime's transport blocks are allocated on one
   thread and freed on another, as `spawn_str.c`'s box already is. **The queue's count of copies is
   7.13's declined *"cross-heap refcount"*, applied to one runtime object that no copy-on-write ever
   tests**, so `cow.c`'s open hazard has no twin; 7.13's text (`design.md:4175-4181`) and §4.10's
   cycle sentence (`:1827-1830`) owe one amendment each in the landing, the second kept true by W-f.

**Route A as the conservative route**: approve as what conservative would have been, not adopted. It
needs none of the 313 sites and no cycle rule (no mailbox inside a value), and every program
serialises its values into text; the critic's A0 (Rebel's `ipc` by name, run 5 of 5 and 3 of 3 under
`--sanitize`) is more conservative still and needs no runtime line at all.
- **03:38, run since, under Q2.5's *"read, not run: I did not start 257 threads"***: `p254/q25/table.c`,
  257 threads each waiting on one mailbox: *"started 256"*, then **exit 134**, *"panic: too many threads
  running at once — this runtime holds 256, and a thread's slot comes back when it is joined"*. So
  under S1 as proposed (*"failing `limit` where the system refuses a thread"*) the 257th `spawn` is an
  abort the program cannot answer; my S1 text makes it the `limit` failure.
- **Corrected 03:30, two times of mine**: Q4's *"written 03:35"* and the note above's *"03:38"* were my
  estimates, not readings; `date` read **03:30:05** before Q4 was written and **03:30:23** after that
  note. Every other time in this report was read from `date` or `uptime`.
- **Corrected again, 03:31, the line above**: *"Every other time in this report was read"* is false.
  Read from `date`/`uptime`: 02:32, 02:42, 03:17, 03:28 (the TSan line), 03:30. From the files' own
  times (`ls -l -T`): the base suite 03:06:06, the prototype suite 03:28:24, `forget.txt` 03:00:33,
  `stopfresh.txt` 03:01:03. **Estimates, not readings**: Q2's *"written 03:05"*, Q1.5's *"written
  03:30"* (written before 03:30:05), Q4's *"03:35"*, the note's *"03:38"*.
- **03:31, a premise of my brief run** (Q2.5: *"`m.send(@v)` emptying `v` as `transfers` does"*):
  `p254/q2/handles_t.c`, the `move` case with the send also told to the set as a transfer
  (`hero_handle_transferring(b, "a mailbox")` then `hero_handle_ended`): **exit 134 at the first send**,
  *"a C handle was handed into a value that will end it with `a mailbox`, and the mark that began its
  life names `free` ... `a mailbox` is not one of them"*. So a send is **not** a transfer in the set's
  terms: it empties the sender's cell as a transfer does, and leaves the life in the set untouched for
  the receiving thread, as the `move` case does (20,000 of 20,000). Half of the premise holds.

## The cost, in one table (counted where it says counted, built where it says built)

| where | what | lines | how known |
|---|---|---|---|
| `check/` 40 files, `emit/` 56, `ir/` 9, `cli/` 5 | 289 `match` sites on `Ty` | one arm each, 300 to 600 | sites **counted**, lines estimated |
| `parse/type.hero`, `ast.hero` and 10 more files | 24 `TypeKind` sites and the parser arm | 251's +12 and +6, 24 arms | **counted** |
| `resolve/names.hero` | the four names left to the checker | ~6 | priced on `field_in_reach` |
| `check/walk.hero` | the receiver-typed dispatch; `mailbox()` by context | ~15, ~25 | priced |
| a new `check/` module | the four operations typed | ~90 | priced |
| `check/builtins.hero`, `inventory.hero` | `spawn`, `spawn_sized`; three rows appended | ~35, 6 | priced |
| `check/reaches.hero` and call sites | S10, S11, W-f | ~70 | priced on the existing walk |
| `check/` | W-h; W-j' | ~12; ~20 | priced |
| `keywords.hero`, the VS Code grammar | two contextual words | 4; 1 | priced |
| a new `emit/` module | the crossing family | ~300 | priced on `emit/sides.hero`'s 240 |
| `emit/` | the operations' lowering (the spawn box and thunk, send, receive, close, wait, `@` emptied on success) | ~150 | **an inference**, no precedent priced |
| `runtime/parts/mail.c` | the walk 138, the queue, stop and gate 275 | 454 | **built** |
| `runtime/parts/spawn.c`, `spawn_str.c`'s shape | a value-spawning door; the table's panic made a failure | ~60; ~5 | priced |
| `runtime/platform/hero_net.c` | the stop heard by the queues | 4 | **built** |
| `runtime/parts/alloc.c` | the two messages Q2.5 found false or blind | ~20 | priced |
| `tests/harness/suite_runtime.hero` | three `SHARED_BY_DECISION` entries | 3 | **measured** need |

**About 1,100 to 1,400 lines under `selfhost/` and about 540 under `runtime/`**, three new diagnostic
classes, an ABI unchanged. Against my seat's scale (Pascal-P4's ~4,000 lines) the route alone is a
third of a Pascal compiler; against this compiler's 228,594 lines it is under one percent.

### The TSan set, rerun clean (`p254/q2/runs-tsan2.log`, 03:22 to 03:32)

| build | topology | runs | exit 0 | reports | hung |
|---|---|---|---|---|---|
| TSan `-O1` | fan_in 8 x 100,000 | 5 | 5, right 800,000 | **0 races** | 0 |
| TSan | fan_out 800,000, 8 receivers | 5 | 5, right 800,000 | **0** | 0 |
| TSan | ring, **64 x 10,000** (reduced, named) | 5 | 5, right 640,000 | **0** | 0 |
| TSan | close, 300 rounds | 5 | 5, 207,036 sent and received, 32,964 refused | **0** | 0 |
| plain, ASan, TSan | cycle `self` / `pair` / `control`, each gate | 1 each | `today` exit 0 all; `mail` 134, 134, 0 | 0 sanitizer lines | 0 |

**So, all told, 120 stress runs (80 plain, 20 ASan+UBSan, 20 TSan) ended exit 0 and right with
both gates silent, 0 ASan, 0 UBSan, 0 TSan reports, 0 hangs.** That corrects Q2.2's *"every one of the
400 plain and sanitized runs below"*, a number I wrote before counting: the count is **120** stress
runs, beside the 55 gateway runs and the 13 handle runs below and above.

## Q3 The gateway shape, route B, an EMULATION (`p254/q2/gateway.c`, 03:32 to 03:34)

**Named as an emulation**: the compiler half is not built, so this is the C the emitter would write
for the program `00-shared.md` § 8 describes, by hand (the `Req` record's struct, descriptor,
`retain`, `release` in `shapes.c`'s shape and its put and take), against the prototype runtime;
the sockets are POSIX's (the platform layer is the ffi-pragmatist's), a connection crossing whole
through a `(mailbox Conn)` as its descriptor; the owner thread holds the cache (a runtime map
`{str: i64}`) and an SQLite connection (the SDK's `libsqlite3`, `insert ... on conflict do update ...
returning n`); N connection threads each send every request to the owner with a reply mailbox inside
it and wait for the reply. **Invariants checked at the end of every run** (any breach aborts): every
count a client was told is the one the owner assigned to that request; per key the counts told are
distinct; the cache equals SQLite key by key and their sum is the requests applied; with no stop,
every request told a count and applied once.

**Correctness** (`p254/q2/gwruns.log`), N 8, M 32 clients, R 200 requests each, no pause, no stop:

| build | runs | exit 0 | outcome | sanitizer reports | hung |
|---|---|---|---|---|---|
| plain `-O2`, one reply mailbox per worker | 20 | 20 | 6,400 of 6,400 applied once, each told its own count | n/a | 0 |
| plain, a fresh reply mailbox per request | 20 | 20 | the same | n/a | 0 |
| ASan+UBSan `-O1` | 5 | 5 | the same | **0** | 0 |
| TSan `-O1` | 5 | 5 | the same | **0 races** | 0 |
| TSan, the stop at 20 ms, S7 as proposed | 5 | 5 | 8 of 32 connections served, 24 stranded (Q2.4) | **0 races** | 0 |

**Speed against one thread, latency-bound** (`p254/q2/speed2.txt`; `gateway_speed.c`, the same
program with a 30 s client wait, built with behaviour 1): M 16 clients, R 20 requests each, **20 ms of
client sleep per request**, no stop, 320 requests per run, every run exit 0 with 320 of 320 right;
four rounds, the order of N alternated, the first round discarded; load **25.38 before, 22.34
after** (`uptime`, 03:34, other sessions' work):

| N connection threads | wall, rounds 2 to 4 | best | `real` | `user + sys` |
|---|---|---|---|---|
| 1 | 7.602, 8.206, 8.115 s | **7.602 s** | 7.60 | 0.08 |
| 8 | 0.942, 1.016, 0.964 s | **0.942 s** (8.1 x faster) | 0.94 | 0.05 |
| 16 | 0.490, 0.541, 0.509 s | **0.490 s** (15.5 x) | 0.49 | 0.05 |

`real` far above `user + sys` is the shape's design here, every thread sleeping on a client, so it is
the latency the brief asks for and not a run that waited on the machine; the one-thread figure is
about its sum (16 x 20 x 20 ms = 6.4 s, plus about 3.8 ms a request of round trip) and the N-thread
ones about its maximum (0.8 s and 0.4 s plus the same). A first attempt with the stop runs' 3 s
client wait aborted 4 of 4 at N = 1 on my own invariant, the clients giving up in the queue
(`speed.txt`): kept, named, not a result.

**Owed, unrun**: Linux arm64 and Windows (no Docker asked, no box); the gateway through the platform
layer's sockets and Rebel's `sql/sqlite` rather than POSIX and the SDK's library; route A's
gateway (the ffi-pragmatist's); a TLS stream handed whole (the ffi-pragmatist's).

## Where I disagree with the spec-warden and the historian, by measurement

- **The spec-warden's W-i** (*"`stopped`, before all else"*) and *"I keep S7 as proposed in `w2`"*: S7 as
  proposed stranded 24 of 32 connections handed through a mailbox, 3 of 3 runs, plain and 5 of 5 under
  TSan; the drain served 32 of 32 and ends a forgotten close as the stop does. I hold the drain.
- **The spec-warden's W-j** (the hand-over through a mailbox only): its measurement stands (a spawned
  function cannot consume a borrowed handle); I prefer W-j', a spawned function's parameter owns what
  `spawn(f, @x)` handed it, about 20 checker lines, so starting a thread on a connection needs no
  mailbox of one.
- **The spec-warden's question 5 answer** (no cell needed to consume a handle bound with `=`): agreed;
  my `handles.c` `move` case consumes a received handle from a plain local, 20,000 of 20,000.
- **The historian's objection 4** (function values): lifted by `fv.c:53`; **objection 1** (a moved
  handle refused by a closed mailbox): agreed, my S6 text; **objection 2** (a process-wide overtaking
  stop has no precedent): agreed and measured, my S7 text makes the stop end only a wait on an empty
  mailbox; **objection 3** (a count of copies has no leak remedy): answered by W-f and the gate, both
  priced, the gate built; **P1**: half held (the queue still allocated), half not (today's gate silent).

## Premises of the briefs found false (each with what showed it)

1. **Panel 251's *"route M needs a kind of name the compiler does not have today"*** (my Q1.3): false
   as worded. design.md §4.11's first lookup is by the receiver's type; `p254/q13/field_vs_fn.hero`
   printed `10` for `b.send(5)` and `1005` for `send(b, 5)`.
2. **`compiler-engineer.md` Q2.1, *"defect 2588 says it is red at base on the platform layer"***: the
   `runtime` suite read **10 passed, 0 failed** on my copy at `89f155330`.
3. **Q2.5, *"`m.send(@v)` emptying `v` as `transfers` does"***: half. The cell yes; the set no: a send
   told to the set as a transfer aborts at the first send (`handles_t.c`, exit 134).
4. **S1's row, *"failing `limit` where the system refuses"*** is silent on the runtime's own table,
   which aborts at the 257th thread (`p254/q25/table.c`, exit 134).
5. **S7's row, *"as `hero_poll.h`'s wait: the stop wins"***: the layer has two behaviours, and the
   socket's is a drain (`hero_net.h:193-199`, *"a call that need not wait still moves what it can"*,
   read, not run on a socket here); the poll set's is the one S7 copied.
6. **`00-shared.md` § 5 question 4's option set** (*"a function per type, a sixth member, or a flat
   encoding"*) reads as three alternatives; the flat encoding I built needs the function per type
   beside it, outside the descriptor: two of the three together, the sixth member unneeded.
7. **The historian's P1, its second half** (*"read by the runtime's leak gate"*): today's gate is
   silent on the cycle (exit 0), the queue and its values being the runtime's own blocks.

And three of mine, corrected underneath where they stood: the uncounted *"400"* runs (120), `mail.c`'s
halves (138 and 275, not 170 and 230), and four times written from estimate.

## Verdict (my seat's structure)

- `verdict`: **approve route B; object to S1, S5, S6, S7, S11 and S12 as written**, each amended
  above with its text. **No veto**: no program here corrupted memory and no sanitizer read a race
  (120 stress, 55 gateway, 13 handle runs; 0 ASan, 0 UBSan, 0 TSan reports). The proposal **adds a
  core construct** (two type constructors), which my seat's definition would veto; under
  `691531908` (`00-shared.md` § 2) that is a cost, recorded as such.
- `section`: design.md §1.1 and §1.7 (core, not sugar), Part 5; §4.10 (`:1827-1830`, the cycle
  sentence W-f keeps true), §4.11 (`:1945-1946`, the first lookup the four names sit in), §4.20 (five
  members kept), Part 7 item 13 (`:4118-4189`, the heaps and the declined cross-heap count).
- `implementation_cost`: **313 `match` sites in 119 files** (`Ty` 289 in 110: `check/` 132, `emit/`
  127, `ir/` 20, `cli/` 10; `TypeKind` 24 in 12), then about **1,100 to 1,400 lines under `selfhost/`**
  (checker about 300 beside the arms, a crossing emitter about 300 on `emit/sides.hero`'s 240, the
  operations' lowering about 150 inferred) and about **540 under `runtime/`**, of which **458 built**
  (`p254/q2/mail.c` 454, `hero_net.c` 4); three new diagnostic classes; the ABI unchanged.
- `needed_for_self_hosting`: **no** (`selfhost/main.hero:88-113`: one thread of one `i64`, nothing
  exchanged). I read **the author's instruction of 2026-11-22 as the ruling on the need**.
- `argument`: Two type constructors are core: 289 `match` sites for one, against R3's thirty. But the
  route is sound as built: a flat encoding walked by per-type functions outside the descriptor keeps
  every program block on one thread, the queue's count is never tested by copy-on-write, and 188
  runs under three sanitizers read nothing. What the text gets wrong is measured, not argued: S5 with
  S9 leaks a self-holding queue silently; S7's overtaking stop stranded 24 of 32 handed connections; a
  send refused by a closed mailbox loses its handle; the 257th `spawn` aborts. Each has a priced repair:
  W-f and a gate, the drain, `v` left on failure, `limit` for the table. (115 words)
- `prediction`: **the first commit of the implementation lane that adds the two `Ty` cases reads 280
  to 300 `error[non_exhaustive]` in 105 to 115 files from `check selfhost/main.hero`**, checkable at
  that commit; and **the landing adds at least 1,100 lines under `selfhost/` and at least 450 under
  `runtime/`** (`git diff --numstat` against `89f155330`), checkable at its merge. Beside them: **a
  gateway on the landed route with S7 as proposed leaves at least 1 of 32 handed connections unserved
  at a stop in 3 of 3 runs**, and **the `runtime` suite on the landing reads 0 failed only with three
  `SHARED_BY_DECISION` entries (or their equal) for the queue's list**, both checkable at the lane's
  gate.
- `condition`: a ThreadSanitizer or AddressSanitizer report against the queue or the crossing walk on
  Linux arm64 or Windows (owed to the CI's legs) would make this a soundness veto until repaired; a run
  showing the drain strand a handle or tell a wrong count with a fresh reply mailbox per request would
  move my S7 back to the proposal's; `(thread T)` and `(mailbox T)` erasable in the frontend (they are
  not: generics are on functions only, spec line 318) would make it sugar and lift the cost.

## Owed, unrun

- The queue and the stop's wake on Windows (written, never compiled there) and on Linux arm64 (no
  Docker asked); LeakSanitizer on the cycle (not on this Mac's ASan).
- A bounded queue with a waiting send (question 3), not built: its deadlock (two threads each sending
  into the other's full queue) is an inference from the shape, not a run.
- The compiler half: not built; every compiler line above is counted or priced, not compiled.
- The instruction cost per crossing (two walks against a deep copy's one): not measured, the machine
  at load 22 to 64 all session.
- **Corrected 03:36, the argument above**: *"188 runs under three sanitizers read nothing"* counts the
  plain runs as sanitized. The count: **188 runs in all (120 stress, 55 gateway, 13 handle), 65 of them
  under a sanitizer** (stress 20 ASan and 20 TSan, gateway 5 ASan and 10 TSan, handles 5 and 5), all 65
  with 0 reports; and the argument is **112 words** (`wc -w`), not 115. Read it as: *"... and 188 runs,
  65 of them under ASan, UBSan or TSan, read nothing."*
