# Panel 259, the compiler-engineer's report

Written as I go, from 2026-11-25 08:21 by this Mac's clock (`date`). My copy is
`.claude/worktrees/259-compiler-engineer` at `3ec775b66` (`git rev-parse HEAD`); long output in
`.claude/worktrees/259-compiler-engineer-scratch/`. The seat was stopped by the usage limit at about 08:30 and resumed
at 09:17 on the coordinator's word. **Ran** and **read** are said apart on every line.

## 0. The compilers (ran)

In my copy, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 74.29`, `user 23.72`, `sys 1.23`, the
machine loaded: indicative only), then `./heroes build selfhost/main.hero -o heroes-next` (`real 610.87`, `user
240.33`, `sys 55.40`, the same caveat), exit 0; `./heroes-next --version` prints `heroes 0.6.0`, `sources
117362877b136f67`, the brief's key (§ 3). Log: `259-compiler-engineer-scratch/build.log`.

## Verdict

**Veto, on soundness, of R1's healing by address** (design.md §1.12): under a switch the runtime does not see onto
memory of the thread's stack, or a library that copies stacks, it hands a live value's room or region to another
frame, and the program prints another frame's bytes (11 for 7, 1001 for 8, 7 for 10) or dies by SIGSEGV with nothing
said, on this Mac and on Linux arm64 at `3ec775b66`. **The route I would adopt is E with B and the owner's proof
(`rt-ebo6`), and I built it**: no entry judged gone by its frame's address; entries given back only on a seen jump or
a seen `makecontext`, each proven by its frame on the abandoned chain and its own pointer in the abandoned memory.
Every carved, copying, heap and lend shape runs right under it; its price is a leak until the room's gibibyte for
every abandonment the runtime does not see (`asm_jump`, door 6, Windows), and +958 instructions a lend and about
+1,940 a 32 KB take in the prototype's asides. One spec sentence is owed, for that price. Rule 6's false words on a
carved coroutine's escape (`escape_carved`, both platforms) are a separate defect no route of the room touches, and so
is `unseen_carved`'s Linux refusal (rule 6 at the `dlsym` door, the `dlsym-make` lane's), which stays under the route.


## 1. The mechanism, traced

**This Mac (ran, 09:20 to 09:30).** A copy of the runtime with five `fprintf`s (`259-compiler-engineer-scratch/rt-trace/`,
its diff `rt-trace.diff`, 49 lines: the thunk's entry in `hero_callback_heal`, each take and each give of `frame.c`, each
entry `hero_frame_heal_below` gives back, each region `hero_lend_heal_inside` gives back), handed to stage 2 by
`HEROES_RUNTIME=`. `own_carved` (`out-own_carved-trace.txt`, lines 1 to 64 quoted by `grep -n`):

```
1:TRACE thunk frame=0x16dac22d0 held=0                                   body's thunk, under ow_host
2:TRACE take own=0x16dac22b0 at=0x7a0080c020 bytes=262144                body's `mine`
3:TRACE take own=0x16dac22b0 at=0x7a0084c020 bytes=262144
28:TRACE thunk frame=0x16db022e0 held=2                                  coroutine 0's thunk, 0x40030 ABOVE body
29:TRACE heal-room victim=0x16dac22b0 at=0x7a0084c020 asker=0x16db022e1   body's two takes given back
30:TRACE heal-room victim=0x16dac22b0 at=0x7a0080c020 asker=0x16db022e1
59:TRACE thunk frame=0x16db422e0 held=4                                  coroutine 1's thunk
60-63: coroutine 0's four takes given back the same way
64:TRACE take own=0x16db422c0 at=0x7a0080c020 bytes=262144                coroutine 1's first take: body's bytes
```

So on this Mac **the thunk's entry gives `body`'s room away** (`callback.c:262` → `frame.c:182`), not a take; the
first coroutine's takes land in another chunk, and **coroutine 1's first take is handed `body`'s own address**, which
its `e5(11)` fills: `body holds 11`, twice. The abort that follows is coroutine 0's `run` giving `0x7a009d0020` back
with no mark left (`top=0x0`: coroutine 1's thunk healed it), so `hero_frame_give` finds none, asks the aside, and
`hero_frame_out_of_order` says *compiler bug* (`frame.c:231-232`, `:144-146` of `frame_aside.c`). `below` is the same
mechanism one level down: coroutine 1's thunk gives back coroutine 0's four takes, coroutine 1's inner takes reuse
`0x74d340c020`, coroutine 0's `v`, and coroutine 0's give finds no mark (`out-below-trace.txt`). Linux arm64 is
traced in § 1b below, in my container.


**§ 1b, Linux arm64 (ran, my container, at `3ec775b66`, 08:42 to 08:45 UTC).** The same traced copy (`lx/rt-trace/`),
`lx/trace/out-own_carved.txt`: `TRACE thunk frame=0xffffdfd37bd0 held=0` (`body`'s), then `TRACE thunk
frame=0xffffdfd77bd0 held=2` (coroutine 0's, 0x40000 above) and **`TRACE heal-room victim=0xffffdfd37ba0` twice**,
`body`'s two takes, then coroutine 1's thunk giving back coroutine 0's four: **the thunk's entry, as on this Mac**.
The run ends by SIGSEGV after the coroutines' gives with nothing of the program's printed (91 trace lines): there the
room's chunks are blocks glibc maps and a free unmaps (an inference from glibc's threshold, not traced), and the
`--sanitize` cell names the read, `own_carved.hero:81`, `body`'s print (the repair's, carried). `below` the same:
coroutine 1's thunk gives back coroutine 0's four takes, then SIGSEGV. `lend_carved`: coroutine 0's thunk gives back
`body`'s two regions, and the program prints `body holds 1001 lent 30 20 31 21` and the same false message as on this
Mac, exit 134.

**The lend regions (ran).** My shape `lend_carved.hero` with `lendown.h` (in `programs/compiler-engineer/`): `body`
holds `mine: i64 @= n0` (a local lent in place for its whole call) bumped by C to 8, and lends `b0` and `b1` to
`ow_start_lent`, which writes `buf[0]`, starts a coroutine on a quarter of `ow_host`'s array (an own switch while the
lend is open) and writes `buf[1]` once it is back; each coroutine lends its own local and a buffer across its own
yield. `lend_heap.hero` is the same on `malloc`'s stacks. Three runs a cell, this Mac, at `3ec775b66`:

| shape | plain | `--sanitize` |
|---|---|---|
| `lend_heap` | exit 0, 3 of 3, 133 bytes: `body holds 8 lent 10 20 11 21`, ..., `body holds 8 coroutines 2 1002`, `lent heap: 8` (right) | exit 0, 3 of 3, the same bytes (`cmp`) |
| `lend_carved` | **exit 134, 3 of 3**, 347 bytes, equal by `cmp`: **`body holds 1001 lent 30 20 31 21`** (8 and 10 are right), then `panic: a buffer lent to C was given back at an address its region does not hold: what held the address in this function's frame was written over, by C writing past a field or an element it was lent, which is the binding's word (§ 13), or by a fault of this compiler`, `at lend_carved.hero:12:5, in lend_carved.run` | **exit 134, 3 of 3**, 1,882 to 1,887 bytes (addresses differ): `AddressSanitizer: BUS on unknown address ... WRITE`, `#0 ... in ow_start_lent lendown.h:9`, `AddressSanitizer can not provide additional info.` |

The trace (`bin-lend_carved-trace`): coroutine 0's thunk entry gives back **both** of `body`'s regions,
`TRACE heal-lend victim=0x16b6de250 base=0x104b38000` (the buffer `b0`, its lend still open in `ow_start_lent`) and
`base=0x104b30000` (`mine`), and coroutine 0's own lends then take the same depths, so the same mappings. **What C reads
and writes when `lend.c:439`'s heal gives a live region away: another frame's bytes, with no fault.** The plain build
gives the region back to the next lend at its depth (`hero_lend_push`, `lend.c:524-535`, whose audit finds the sum
unmoved, since nothing wrote between the heal and the reuse), so coroutine 0's `n` is placed on `mine` and its buffer on
`b0`: **two wrong values printed**, then a **false message** naming the binding's word and § 13 for what is a runtime
fault, told by `hero_lend_misplaced` (`lend.c:670`) at a coroutine's give of its buffer after `ow_yield_lent`
(`lend_carved.hero:12`), which no longer finds its region where its frame put it. **No
guard page is involved**: a region's guard is past its end, and every access here is inside a room. Under `--sanitize`
the region given back is sealed into the quarantine (`lend.c:573-591`), so C's `buf[1] = 20 + k` after the switch is a
**fault** at the C line, and AddressSanitizer, which owns the signal there, can say nothing of its cause. So the brief's
three: **plain, a wrong byte and then a false message; `--sanitize`, a fault with no cause named.** Whether the regions'
silent give (`lend.c:472`'s `return`) ran before that message, the trace does not say (it prints no line there).


## 2. Every judgment by order

**Enumerated** by `grep -n 'hero_frame_is_below\|hero_stack_lane(\|hero_sig_known_range\|hero_rebind_known_range\|__builtin_frame_address\|HERO_CALLER_FRAME()' runtime/parts/*.c runtime/*.h` at the base, then by reading each hit's function and the signal rules it feeds (`sigentries.c:258-446`, `sigguard.c:495-690`, `rebind_jump.c:62-213`, `sigjump_save.c:40-110`). Every comparison of a frame's address with another's, and every judgment of which stack an address is on, is a row. *Lies* means: an own switch onto a stack carved from the thread's own makes the judgment answer wrong.

| # | judgment | where | lies under an own switch onto a carved stack? | what the program gets | ran or read |
|---|---|---|---|---|---|
| J1 | the room's heal: every take of a frame below the asker given back | `frame.c:181-185`, asked by each take (`:199`) and the thunk's entry (`callback.c:262`) | **yes**: the coroutine's thunk lies above `body` | a live value's room handed to another frame: `own_carved` `body holds 11`; `coval_carved` (mine) **the coroutine's own value read as 7 for 10**; then *compiler bug* (Mac) or SIGSEGV (Linux) | **ran**, traced (§ 1) |
| J2 | the room's give: the takes above a giver given back where each is of a frame below it, else *compiler bug* | `frame.c:224-240` | **yes, both ways**: a live frame below the coroutine (on the thread's stack, under the array) is judged gone, and a coroutine's take above a host's give is judged out of order | `give_carved` (mine): the coroutine's give frees `held`'s two live takes (`top=0x0` at `held`'s give), then `panic: internal error: a large value's room was given back out of order (compiler bug)`, a false message; the values printed right in this run by luck of the reuse | **ran**, traced |
| J3 | the regions' heal | `lend.c:439-446`, asked by each lend (`:526`) and the thunk's entry (`:449-455`) | **yes** | `lend_carved` (mine): `body holds 1001` for 8, `b0[0]` 30 for 10, then the false *binding's word (§ 13)* message; `--sanitize`, a BUS fault in C | **ran**, traced (§ 1) |
| J4 | the regions' give: silent return where an entry above is not below the giver | `lend.c:462-481` | yes, both ways, as J2 | the silent return leaves the depth, and the give then says *given back at an address its region does not hold* (`lend.c:670`, a local's `:980`), false; or a live region below a coroutine is retired | read; J3's run ends at that message |
| J5 | the asides' heal within one stack tag | `frame_aside.c:66-79`, `lend_aside.c:86-96` | yes, where the own switch is onto memory carved from a stack `makecontext` named (an own-switch library run inside a `makecontext` coroutine) | as J1 and J3, inside that coroutine | read, unrun |
| J6 | which stack a frame is on | `stack_lane.c:88-149`, asked at `frame.c:198`, `lend.c:555`, `callback.c:261` | **yes**: it says *the thread's own* of every unseen carved stack (its own comment, `:51-55`) | the premise J1 to J4 rest on | read; its answer is J1 to J4's runs |
| J7 | `hero_frame_is_below`: an address and the measured range | `callback.c:213-229` | yes | the test J1 to J4 ask | read |
| J8 | rule 4, the range a jump's target lies on | `sigentries.c:258-272` (Darwin `rebind_entries.c`) | yes: a carved coroutine's target is *the thread's own* | feeds J9 and J10 | read |
| J9 | rules 4 and 5, a handler's entry left where a jump's target lies at or above it on its range | `sigentries.c:345-365` | yes, toward the unsafe side: a C handler that own-switches into a coroutine carved above it, which then jumps inside itself, has its entry dropped while it runs; the reverse jump keeps an entry a handler has left | the first: a callback run inside a signal handler with no refusal (async-signal-unsafe); the second: STANDING's words, false | read, unrun (it needs a handler that own-switches) |
| J10 | **rule 6, a seen jump landing below the jumping frame on one range** | `sigentries.c:379-384`, Darwin `hero_rebind_reenter` | **yes, on a seen jump**: a coroutine on a carved stack that leaves for good by a `longjmp` to its host's save point, which lies under the array, *lands below* on the thread's one range | `escape_carved` (mine): **exit 134**, `panic: escapecarved.twice was called after C jumped into stack frames the thread had already left (a handler left by a jump and jumped back into, ...)`, words about a signal handler the program does not have; **`escape_heap`, its twin on `malloc`'s stacks, exit 0, `escaped: -1 then 42`**. 2705's and 2755's class, **on this Mac**, and no route of the room touches it | **ran**, this Mac |
| J11 | rule 7 and HELD: a jump onto a range `makecontext` was given, held a coroutine's where route A holds contexts | `sigentries.c:345-365` (`life`, the assignment at `:362`), `sigguard.c:557-612`, `:669` | **no**: it judges only ranges `makecontext` named, whatever the frames' order; an unseen carved stack has no range, so HELD cannot fire on it | its words are true of what it judges (§ HELD below) | read |
| J12 | the thunk's walk for the delivery's own frame (the CALLED test) | `sigguard.c:672-688` | it misses, never lies: a coroutine's chain ends at its first record (`own.h`'s `x29 = 0`), so a delivery below is not found, and STANDING's entry still refuses in true words | none wrong | read |
| J13 | the abort's walk of its callers | `calls.c:723` | no judgment of liveness: it names callers until the chain ends | `called from ... in own_carved.run`, true | ran (every abort above prints it) |
| J14 | route A's records of a save (`hero_jmp_below`, `hero_rebind_below`) | `sigjump_record.c:140`, `:268`; `rebind_record.c:175`, `:260` | they record a frame below the caller's stack pointer and are matched by buffer, not by order among stacks | none found | read |

**The answer to "could any judgment refuse `own_carved` and `below`, in true words, on every platform?"**: no judgment of today's runtime can, because none sees the switch, and rule 6 (J10) needs a seen switch. **But a fact exists that differs** (§ 3): the frame-record chain from the asker. And the runs the brief called *least likely to move*: a C `longjmp` inside a coroutine on a carved stack (`jumpco_own`, mine) is corrupted like `own_carved` at the base (*compiler bug* on this Mac, SIGSEGV on Linux) and right under the route; its sibling, a coroutine on a carved stack leaving for good by a `longjmp` to its host (`escape_carved`), **moved where I did not expect**: rule 6 refuses it on this Mac as on Linux, since `own.h` calls no `makecontext` and the Mac knows no range either; a signal delivered while a coroutine runs on a carved stack (`sig_carved`, mine) is corrupted at the base and right under the route. `unseen_carved`'s Darwin run I did not trace (it runs right here).

**HERO_SIGNAL_HELD** (step 179a's arm, `sigguard.c:607-612`, its kind `HERO_SIG_HELD` from `unsure == 4`, `sigentries.c:362` and `:440`): its order judgment is J11's, made only on a range `makecontext` named, so an own switch onto a carved stack **does not make it lie**: such a stack has no range. Its words, *"onto a stack `makecontext` was given, which this runtime holds may still be a coroutine's, since on this system it cannot read where a context switch lands or see a coroutine end"*, are true wherever route A holds contexts, which is the only place `unsure` becomes 4 (`hero_sig_range_life` returns `HERO_SIG_RANGE_HELD` only for a range `hero_sig_known_stack` was given `held`, `sigentries.c:136-150` and `:215-220`; `sigjump_make.c:33` passes `hero_ctx_recording()`, true only where route A holds contexts; read by `grep -n`). The one correct program it refuses that I can name: a handler's jump onto memory that was a carved `makecontext` range whose host has since returned, which route A holds a coroutine's for ever; its words say *may still be*, so they stay true of it. **I would ratify HELD and its words**, the cost (a correct program refused where route A holds a range for ever) named beside it. Read, not run: HELD needs `HERO_JMP_FORCE_UNCALIBRATED` or a platform where route A holds contexts, and I did not build that runtime.


## 3. Route A, detection

**The facts the runtime can read when a heal would fire, each against the programs it must tell apart** (ran, this
Mac, at `3ec775b66`). The jumps R1 heals rightly: `room_order` (panel 258's, the C library's `longjmp`, seen),
**`asm_jump` (mine: `asmjump.h`, a save and a jump in the header's own assembly, AArch64 and x86-64, seen by no hook;
`room_order`'s body looped 5,000 times)** and `lendjump`; against `own_carved`, `below` and `lend_carved`.

| fact | R1's jumps | the carved shapes | differs? |
|---|---|---|---|
| the asking frame, the stack pointer | above the victim, on the thread's measured stack | the same | **no** (it is the address the heal already asks) |
| the marks (frame, entry) | an entry of a frame below the asker | the same | **no** |
| `hero_callback_thread.inside`, `.held` (`callback.c:91-96`) | `inside` counts the abandoned callback: 1 at the next thunk | `inside` counts `body`'s live callback: 1 at the coroutine's thunk | **no**: the count cannot tell abandoned from suspended (`callback.c:31-34` says so of itself) |
| the jump hook's records (`sigjump_record.c`, Darwin's `rebind_record.c`) | a decoded jump crossed the victim's frame for `room_order`; **none for `asm_jump`** | none | **for seen jumps only**: that is route B's fact, and it misses `asm_jump` |
| **the chain of frame records from the heal** (probe `rt-probe/`, its diff `rt-probe.diff`, printing the first eight heals) | climbs 8 steps (`room_order`, `asm_jump`) or 9 (`lendjump`) to the thread's outermost record, `below-hi=5808`, ended by 0 | climbs 5 (`own_carved`, `below`) or 6 (`lend_carved`) steps to **the coroutine's first record** (`own.h`'s `x29 = 0`), `below-hi` 531,936 to 794,080, ended by 0 | **yes, all six**, the unseen jump included |

So **a detector exists**: *the asker's chain does not climb to the thread's own outermost record*. It fires at
coroutine 0's thunk entry, **before** `body holds` is printed (the trace of § 1: the first heal is at line 28 of
`out-own_carved-trace.txt`, the first print after line 64). Its words could be true (*"... called on a stack C
switched to by code of its own, carved from the thread's, which this runtime does not see; a value held across that
switch would have been given to another frame"*). **But route A refuses `own_carved`, `below`, `lend_carved`,
`coval_carved`, `give_carved` and `copy_carved`, six correct programs that route E runs right** (§ the table), and its
fact is fooled where route C's is (§ 5): a refusal that a route of the same cost turns into a right answer is the
conservative resolution, not the robust one.


## 4. Route B, healing only on a seen event, and the owner's proof

**Built** in three steps, each a copy of `runtime/` handed by `HEROES_RUNTIME=` (diffs against the base in
`259-compiler-engineer-scratch/`): `rt-eb` (B on E), `rt-ebo` and `rt-ebo2` (the owner's proof, then its cost), and
`rt-ebo3` (the `makecontext` range), `rt-ebo5` (below), and **`rt-ebo6`, the route I would adopt** (`rt-ebo6.diff`,
copied to `programs/compiler-engineer/runtime-rt-ebo6.diff`: 288 lines added, 7 removed, in `callback.c`, `frame.c`,
`lend.c`, `frame_aside.c`, `lend_aside.c`, `sigjump_save.c`, `sigjump_make.c`, `rebind_jump.c`). **`rt-ebo6` is
`rt-ebo5` with the hook's buffer freed when a thread leaves** (`hero_frame_thread_leave`): Linux arm64's `--sanitize`
cell of the golden of coroutines on a `spawn` thread exited 1, 3 of 3, `LeakSanitizer: detected memory leaks`, 65,544
bytes from `hero_heal_prepare` (this Mac's sanitizer checks no leaks, so only Linux saw it); with the buffer freed, both
`spawn` programs pass plain and `--sanitize`, 3 of 3, on this Mac and in the arm64 image (the table). **`rt-ebo5` is `rt-ebo3` with
`no_sanitize("address")` on its two scans**: the Mac's `--sanitize` cells of `rt-ebo3` failed six jump shapes
(`room_order`, `lendjump`, `jumpco`, `jumpco_own`, `copy_jump`, the golden of a `longjmp` inside a coroutine), each
`AddressSanitizer: stack-buffer-overflow`, where the base passes: the hook reads the abandoned frames whole, redzones
included, on purpose; exempted, the three re-run (`room_order`, `lendjump`, `copy_jump`) exit 0 under `--sanitize`.
A plain build cannot tell the two apart, so `rt-ebo3`'s plain cells and costs stand for `rt-ebo5`. On top of E (§ 6):

1. **A seen jump of the setjmp family** (`hero_jmp_jump`, Linux; `hero_rebind_jump`, Darwin; only where the target was
   decoded, route C) calls `hero_heal_jump(t)`, which **only reads**, since it may run inside a signal handler: it
   climbs the jumper's frame records from its own, each step up, inside one stack it can bound (the thread's measured
   stack, or a range `makecontext` named), and only where the climb reaches `t` does it note the records below `t`
   (the frames the jump abandons, by name, not an interval) and copy the words of those frames, callee-saved
   registers spilled first by `__builtin_unwind_init`, into a buffer the thread allocated earlier, where no handler runs. Plain
   stores, no lock, no allocation.
2. **The next take, give, lend or thunk's entry of that thread settles**: under the asides' locks, it gives back each
   entry whose taking frame is a noted record **and whose own pointer is one of the copied words** (the owner's proof:
   the function that took an entry keeps its pointer until its give, so the abandoned image holds it; a coroutine
   copied out, whose frames held the same addresses, does not).
3. **A seen `makecontext`** (`hero_sig_made_context`, `hero_rebind_made_context`) names a range as a stack that starts
   over: every entry of a frame inside it whose pointer is still a word of the range's used memory is given back (read
   from four pages under the lowest such frame, never from the range's first page, where a guard may lie).

**Its residues, read, not run**: the prototype pools the noted frames and copied words of every jump between two
settles, so with a library that copies stacks, two jumps and no take, give, lend or thunk between them, and a pointer
to one coroutine's entry held in the other's abandoned frames, an entry could be matched by one jump's frame and the
other's words; a landing keeps them per jump (a count each), which closes it. An entry whose owner kept its pointer
only in a register no frame spilled is not proven, and leaks; `__builtin_unwind_init` spills every callee-saved one.

**What each step bought, measured (this Mac, at `3ec775b66`, one run each unless said):**

| program | E alone (`rt-e`) | E+B by frame (`rt-eb`) | + owner's proof (`rt-ebo2`) | + `makecontext` (`rt-ebo3`) |
|---|---|---|---|---|
| `room_order` | exit 0 | exit 0, 3.67 MB resident | exit 0, 3.77 MB | exit 0 (cells) |
| `lendjump` (2756's leak) | exit 0 | exit 0, 3.96 MB | exit 0, 3.98 MB | exit 0 (cells) |
| `jumpco` | **exit 134, *stack exhausted*** | (not run) | exit 0, 6.98 MB | exit 0, 7.00 MB plain build, 6.67 MB `-O2` |
| `copy_jump` (§ 8) | exit 0, 3 of 3, right | **exit 134, 3 of 3, false *compiler bug*** | **exit 0, right** | exit 0, right, both levels |
| golden *a coroutine left for good* | **exit 134, *stack exhausted*** | (not run) | **exit 134, *stack exhausted*** | **exit 0, golden's lines, 7.82 MB plain, 7.42 MB `-O2`** |
| `asm_jump` (unseen) | exit 134 | exit 134, 982 MB resident | exit 134 | exit 134 (§ price) |

**Its price, on § 4.3's doors** (ran where marked): every abandonment B does not see leaks the abandoned entries until
the room's gibibyte, then aborts with **`panic: stack exhausted in asm_jump.body: the room large values take off the
stack is full, 1 GiB for the whole program`** (ran, `asm_jump`: exit 0 at the base, 3.6 MB resident; exit 134 under
E+B after about 3,700 of its 5,000 jumps, 982 MB to 1,060 MB resident). Words true of the room, silent on the cause:
**the correct programs that run on `main` and stop under B are the loops of an unseen abandonment over a callback
holding values past 4 KiB**: door 6 on x86-64 (`__builtin_longjmp`; the ffi-pragmatist's cell, unrun by me), a jump in
assembly (`asm_jump`, run), a C++ exception through the frames (door 13), a handler returning to a `getcontext` point
(door 12), a coroutine left for good on a stack never named again (door 8, `quit_fresh`, which aborts **at the base
too**: draft 2's defect), and **every jump on Windows**, which has no hook (read, `.claude/rules/platforms.md`; unrun,
no box). Doors 9 and 10 end the thread: under E its entries are asides of the process, not the thread's chunks that
`hero_frame_thread_leave` frees, so they stay held, a leak (read, `frame.c:257-270` frees the thread's room alone). The lend regions leak mappings with no gibibyte (read: `lend_aside.c` keeps a lent record until its
give; an unbounded count, an inference from the code). **A gate stays silent on a leak; I found no false message
added by B** (the cells), and **the abort's words should gain the gate's clause** (`HERO_CALLBACKS_LEFT`,
`callback.c:133-135`: *N callback(s) of this program never returned: C jumped over their frames*) where
`hero_callbacks_unfinished()` is above zero, so the cause is said: a one-line change, unbuilt.

**`__builtin_longjmp`**: clang refuses `__builtin_setjmp` on both of my platforms (the repair's run, carried); **its
x86-64 cell is the ffi-pragmatist's**, and my prototype runtime is at
`.claude/worktrees/259-compiler-engineer-scratch/rt-ebo6/` for it to read, never to edit: under it the door is an
unseen jump, so the prediction is `asm_jump`'s (a leak until the abort), **unrun by me**.


## 5. Route C, an identity no switch fools

**Built as a probe, not as a route** (`rt-probe`, § 3). The fact is the chain of frame records; **its cost where it
walks** is the climb, 5 to 9 steps in every shape above (three loads and a compare a step), asked only where a heal
would fire: negligible on the path where nothing heals, which never asks it. **What fools it, each by its shape:**

- **a library that copies stacks** (§ 8): my `copy.h` gives each coroutine a first record of 0, so its chain ends
  there and C's gate would heal nothing on it (read from the probe's rule; I did not run C's gate on it); but where a
  copying library starts a coroutine by calling it at the current stack position (greenlet's design, read, not run),
  the chain climbs into the thread's own records, C judges it the thread's, and R1 healing under C's gate gives a
  copied-out coroutine's value away, two coroutines' chains being equal word for word where their code is. **E
  survives both, by construction; C survives the first only**;
- **a first frame a library links to its resumer's on purpose**: inside the thread's stack the link goes *down* (the
  resumer lies under the array), which the climb's rule *each step up* stops, so C is not fooled there (read, from
  `sigguard.c:686`'s own rule, which I copied); on a heap stack the link goes up into the thread's records, but such a
  frame is off the measured stack and judged by `stack_lane.c` before C is asked. **Ran, this Mac**: `link.h` (mine) is
  `own.h` with each first record linked to its starter's frame (`fr[10]`, `fr[5]` on x86-64), `link_carved.hero` is
  `own_carved` on it; the probe's climb stops after 5 steps at the downward link (`ended-by=0x16f5d6070`, the starter's
  frame), so C is not fooled; R1 prints `body holds 11` and aborts, 3 of 3; the route prints `body holds 7 coroutines
  30 32`, 3 of 3;
- **a C frame with no frame record** (x86-64 `-fomit-frame-pointer`, the ffi-pragmatist's image): the climb stops
  early, so C answers *not the thread's*: no heal, a leak, never a wrong value (read);
- **Windows x86-64** keeps no frame records (`callback.c:170-176`): C does not exist there.

**The stack pointer of the extern call that is still open** does not separate `below` (critic, F21): every frame of
`below` lies inside the open call; I did not build it, since the chain separates `below` (§ 3's table) and the copying
shape defeats both. So **C is a detector (route A's), not an identity a route can rest a heal on**.


**The hybrid I considered and did not build**: R1's room kept for the takes whose frame's chain climbs to the thread's
outermost record, the asides for every other. Its hot path would be cheaper than E's, but it must climb at every take
to choose the lane (or keep a memo whose own soundness is a new premise), it is fooled where C is (greenlet's design,
read), and it does not exist on Windows x86-64. Robustness ranks above its saving (CLAUDE.md § Precedence), so it is
the cheaper road, recorded, not the one I would take.

## 6. Route E, no healing by address

**Built** (`rt-e`, 43 lines of diff: every take of the room and every lend through the existing asides, tagged 0, so
`frame_aside.c:99`'s `stack == 0` and `lend_aside.c:105` and `:149`'s `stack != 0` tests never judge one gone; the thunk's entry judges nothing).
The asides already give back **in any order and on any thread** (`frame_aside.c:140-153`, `lend_aside.c:135-160`), so
**no order can be wrong**: R1's false *compiler bug* (2752) cannot occur. **Every carved, heap, lend and copying shape
is right under E** (the table). **What then repairs 2752's and 2756's leaks**: nothing, under E alone: the
golden of a C `longjmp` over a callback inside a coroutine and the golden of a coroutine left for good both abort with
*stack exhausted* (ran, the Mac's E cells, `golden=DIFFERS`), and so do `jumpco` and `jumpco_own`. **That is why B
(§ 4) is part of the route**, and B with the owner's proof is sound under every switch I could write.

**Its cost** (ran, `shapes/cost/cost.sh`, this Mac, four alternating rounds, instructions retired, the mean of rounds
2 to 4, the first discarded; `259-compiler-engineer-scratch/cost-O2-ebo3.txt` and `cost-O0-ebo3.txt`), the base
runtime (a) against `rt-ebo3` (b), whose plain build is `rt-ebo5`'s:

| program | level | base | route | per unit |
|---|---|---|---|---|
| `cost_cb`, 20,000,000 callbacks | `-O2` | 4,071,339,861 | 4,091,293,501 | **+1.0 instruction a callback, +0.49%** |
| | `-O0` | 10,083,246,462 | 10,121,228,386 | +1.9 a callback, +0.38% |
| `cost_lend`, 2,000,000 lends | `-O2` | 5,551,565,517 | 7,468,458,260 | **+958 a lend, +34.5%** (the aside's lock and its scan of a slab of 64 records, `lend_aside.c:102-133`) |
| | `-O0` | 9,915,162,321 | 13,560,796,888 | +1,823 a lend, +36.8% |
| `cost_room`, 300,000 calls of `one`, 3 takes of 32 KB a call (counted by the traced runtime) | `-O2` | 5,434,930,518 | 7,181,277,769 | **+5,821 a call, about +1,940 a take, +32.1%** (a `malloc` and a `free` of 32 KB and the aside's header, the lock, the list) |
| | `-O0` | 10,117,455,262 | 11,891,541,304 | +5,914 a call, +17.5% |

A first build, `rt-ebo`, read three thread-locals at each thunk's entry and cost **+90 instructions a callback** at
`-O2` (`cost-O2.txt`); folding B's flag into `hero_callback_thread`, which the thunk already writes, paid for it.

The lane carried **+550 at 32 KB** for its aside take from `f767e0d4e`'s body; mine is a different program and build,
so the two are not a contradiction, and mine is the one measured here. **This is the prototype's cost, not the
route's floor**: both asides lock one process-wide mutex and the lend scans a slab, where a per-thread free list of
blocks and regions (a give on another thread pushes to that thread's list) removes the `malloc` and the scan without
changing what is judged. Unbuilt, so unmeasured; I state no number for it. **The compiler itself takes no large value
and lends `@` locals in 23 functions** (`grep -c` of `seed/heroes.c`: `hero_frame_take(` 1 and `hero_frame_give(` 1,
both string literals its emitter writes; `hero_lend_local(` 25 hits), so its own build pays a few hundred lends at
most: **an inference from the counts, the build not timed** (the machine read load 36 to 90 all morning).


## 7. Route D from the runtime's side

**Read, not built.** An entry point a binding calls at each switch (`hero_stack_entered(low, bytes)` in `arm-t`;
`grep -rn hero_stack_entered runtime selfhost spec` prints nothing) would have to name the range as `makecontext`'s
wrappers name it (`hero_sig_known_stack`, `sigentries.c:136-150`; `hero_rebind_known_stack`, `rebind_entries.c:73-90`)
and count `hero_stacks_made` (`callback.c:197`); a program that never calls it pays nothing new (the lane's quick test,
`stack_lane.c:143-146`, already reads that count). **What it would buy under the route I adopt: nothing a program needs**:
E judges no entry by its stack, so a told switch changes no answer; it would only let B heal on more events, which the
owner's proof already does safely. **The sanitizer's call** (`__sanitizer_start_switch_fiber`) reaches `--sanitize`
builds alone (§ 6 D of the shared brief, by `grep`; my reading agrees): a runtime that learned switches from it would
judge differently in the two builds, which is a defect by itself. **A binding that does not tell gets, under D over
R1, today's wrong values**; under E, the right ones. So **D is unneeded, and as a new form (`hero_os.h` is a stability
promise, decision 2228, carried) it would cost a spec sentence for nothing**.


## 8. The route nobody listed, and the copying shape

**A library that copies stacks (ran, this Mac, at `3ec775b66`).** `copy.h` (mine): two coroutines run in turn on
**one** region of the thread's own stack, an array in `cp_host`'s frame; switching a coroutine in copies its saved
image back to the region at the addresses it had, after copying the one there out to the heap; `own.h`'s assembly
switches, always through the host, whose frames lie under the region. `copy_carved.hero`: `body` holds `e5(7)`, two
coroutines each hold a value across a yield. `copy_jump.hero`: both coroutines run the same function, so their frames
coincide; coroutine 0 holds `v` across a yield in `step`, coroutine 1 enters `step` at the **same address** and
`longjmp`s over it (the C library's, a jump the hook sees). Three runs a cell, plain:

| shape | R1 (base) | route E (`rt-e`) | route E+B (`rt-eb`) |
|---|---|---|---|
| `copy_carved` | exit 134: `panic: internal error: a large value's room was given back out of order (compiler bug)` | **exit 0**: `body holds 7 coroutines 30 32 (7 30 32 are right)` | **exit 0**, the same |
| `copy_jump` | exit 134, 3 of 3: the same *compiler bug*, `called from ... in copy_jump.step` | **exit 0, 3 of 3**: `coroutine 0 answers 10099 (10099 is right), coroutine 1 jumped: 7` | **exit 134, 3 of 3**: the same *compiler bug*, after B gave back coroutine 0's live `v`, whose frame address coroutine 1's abandoned `step` shared |

So **which route survives the copying shape: E, and E with B once B carries the owner's proof** (`rt-ebo2` onward,
`copy_jump` exit 0 and right, § 4; `rt-ebo5` 3 of 3 plain and `--sanitize`, the table). R1 fails it at the first
thunk; B naming abandoned frames by their address alone fails it on any seen jump inside one coroutine while another
holds a value in a frame at the same address. **No address and no frame record can tell two copied coroutines apart**
(their chains are equal word for word where their code is), so B needed an identity of the *entry*, not of the frame:
its own pointer, present in the abandoned image or not.


## 9. What the emitter owes

**Nothing.** The route is the runtime's alone: no file of `selfhost/` moves, so `selfhost/emit/large_value.hero` (its
takes and gives), `selfhost/emit/callback_thunk.hero` (`hero_callback_entered`, `hero_callback_left`) and the prologue
emit what they emit today. **Ran**: `heroes-next build <p>.hero --emit-c` with the base runtime and with
`HEROES_RUNTIME=rt-ebo3`, for `own_carved`, `room_order` and `lend_carved`: `cmp` differs (at the provenance line),
and the two emissions with `grep -v 'hero_program_sources(void) {'` are **identical**, three of three
(`259-compiler-engineer-scratch/emitc/`). **§ 9b, the compiler's own emission (ran, 09:58 to 10:23)**:
`./heroes-next build selfhost/main.hero --emit-c`, once with the base runtime and once with `HEROES_RUNTIME=rt-ebo3`:
both exit 0, both 40,230,893 bytes, `cmp` differs at line 362,317, and with `grep -v 'hero_program_sources(void) {'`
the two are **identical** (`emitc/compiler.log`), which is `.claude/rules/verification.md` § The formatter's probe's
exception. **The owner's proof rests on a fact
the emitter already gives**: the function that took an entry keeps its pointer until its give (the give is the
function's one exit, `frame.c:13-16`, `lend.c` § A LOCAL LENT TO C), so the abandoned image holds it; an emitter that
ever kept only a derived pointer would make B leak, never give a live entry away.


## 10. The spec sentence, and the arms

**Yes, one sentence is owed, and it is about the price, not the switch.** Under the route I would adopt a value is
never handed to another frame, so the switch needs no sentence (R1 owed none either; §1.12 already promises it). What a
reader **cannot predict** is `asm_jump`: a correct loop of a C jump this runtime does not see, over a callback holding
a value past 4 KiB, runs at the base and **aborts after about 3,700 of its 5,000 jumps** under the route, with *stack
exhausted* (ran); and on Windows every jump is unseen (read). The spec says only *"running out of stack aborts"*
(line 331). A draft, for the warden to place and count (beside line 331, or beside § 13's lease sentence, lines
473 to 474): *"What a function held when C left its frame by a jump or a switch this runtime does not see (code of its
own, a C++ exception, any jump on Windows) stays held until the program ends, and a program that holds 1 GiB of such
values aborts, saying so."* Unmeasured by me (no `measure` run of it; the warden counts).

**The arms, clause by clause** (true: the program and its cell; false: the program that falsifies it; untested: why).
Every cell is this Mac at `3ec775b66` unless it says Linux arm64.

- **`arm-p`**, *"A value a function holds keeps it across every switch of stacks C makes, by `swapcontext` or by code
  of its own, wherever the stack lies."*
  - **at the base: false**: `own_carved` (`body holds 11` for 7), `coval_carved` (`coroutine 7` for 10), `lend_carved`
    (`body holds 1001` for 8), and on Linux arm64 `copy_carved`, `copy_jump`, `coval_carved` exit 139 with nothing
    printed, a use-after-free under `--sanitize`;
  - **under the route: true** for every shape I ran: by code of its own on the heap (`own_heap`), carved from the
    thread's stack (`own_carved`, `coval_carved`, `give_carved`, `lend_carved`), inside an open extern call (`below`),
    copied in and out of one region (`copy_carved`, `copy_jump`); by `swapcontext` on a range `makecontext` named
    (`carved`, the eight goldens), and through `dlsym` (`unseen_carved`, this Mac; on Linux it is refused by rule 6
    before it ends, as at the base, a refusal and not a value lost); the cells are § the table;
  - **untested**: Windows (no box in this sitting), Linux x86-64 (the ffi-pragmatist's image), a switch made inside a
    signal handler (panel 257's `sched`), Boost's own switch (this Mac only; the ffi-pragmatist's binding).
- **`arm-r`**, *"A callback C runs on a stack C switched to by code of its own, not by `swapcontext` or `setcontext`,
  inside the stack of the thread that called C, may abort, saying the switch was not seen;"*
  - **false at the base, in its words**: `own_carved` aborts saying *compiler bug* (this Mac) or SIGSEGV (Linux),
    after printing a wrong value the clause does not admit; no message of the runtime says a switch *was not seen*
    (`grep -rn 'was not seen' runtime/parts` prints nothing); and `escape_carved`, in its scope, is refused in
    REENTERED's words on both platforms;
  - **under the route: true only vacuously** (*may*): nothing in its scope aborts but `escape_carved`, in other words.
  - *"a stack from `malloc` or `mmap`, or one given to `makecontext`, is followed."*: **false on Linux at the base**
    for a `makecontext` reached by `dlsym(RTLD_NEXT, ...)` (`unseen_carved`, the repair's cell, carried; the
    `dlsym-make` lane's door); under the route nothing needs following, so the clause promises nothing a program sees.
- **`arm-t`**, *"C that switches stacks by code of its own tells the program before it runs a callback there, by
  `hero_stack_entered(low: ptr, bytes: i64) -> ()` (`hero_os.h`);"*: **untested**: the function exists nowhere
  (`grep -rn hero_stack_entered runtime selfhost spec` prints nothing). *"a callback C runs ... on a stack the program
  was not told of may abort, saying so."*: **false at the base** as `arm-r`'s first clause; **moot under the route**,
  where a binding that tells nothing runs right (`own_carved`).

So **the arm that describes the route is `arm-p`, and the sentence the route owes is none of the three**: it is the
price's (above).


## The table of routes against shapes

Every cell three runs, plain and `--sanitize`, exit codes and the first line that matters; *R1* is the runtime at
`3ec775b66`; *E* is `rt-e`; *E+B+owner* is `rt-ebo3` for this Mac's plain cells (its plain build is `rt-ebo5`'s),
`rt-ebo5` for this Mac's `--sanitize` cells and every Linux cell, and `rt-ebo6` (which differs only in freeing the
hook's buffer when a thread leaves) for the two `spawn` programs, the only cells where that matters. Mac is this Mac (Darwin arm64); Linux is my `heroes-linux-arm64` container, compilers built inside it
from a `git archive` of the base (`sources 05186f2b38e77a97`, `117362877b136f67`), **every Linux cell at `3ec775b66`**.
Logs: `259-compiler-engineer-scratch/cells-mac-*.log`, `lx/cells-*.log`; outputs beside them.

| shape | route | Mac plain | Mac `--sanitize` | Linux plain | Linux `--sanitize` |
|---|---|---|---|---|---|
| `own_carved` | R1 | 134 ×3, `body holds 11`, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3, nothing printed (the repair's, same base) | 1 ×3, heap-use-after-free (the repair's) |
| | E | **0 ×3, `body holds 7`, right** | **0 ×3, right** | (not run) | (not run) |
| | E+B+owner | **0 ×3, right** | **0 ×3, right** | **0 ×3, `body holds 7`** | **0 ×3** |
| `below` | R1 | 134 ×3, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3 (the repair's) | 1 ×3 (the repair's) |
| | E | **0 ×3, `below: 3032`** | **0 ×3** | | |
| | E+B+owner | **0 ×3, `below: 3032`** | **0 ×3** | **0 ×3, `below: 3032`** | **0 ×3** |
| `lend_carved` (mine) | R1 | 134 ×3, **`body holds 1001 lent 30 20 31 21`**, then the false *binding's word (§ 13)* | 134 ×3, BUS in C (`lendown.h:9`) | 134 ×3, the same lines | 1 ×3, `AddressSanitizer: SEGV` |
| | E, E+B+owner | **0 ×3, `body holds 8 lent 10 20 11 21`** | **0 ×3** | **0 ×3, `body holds 8 lent 10 20 11 21`** | **0 ×3** |
| `coval_carved` (mine) | R1 | 134 ×3, **`coroutine 7`** (10 is right), *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3, nothing printed | 1 ×3, heap-use-after-free |
| | E, E+B+owner | **0 ×3, `body holds 7 other 107 coroutine 10`** | **0 ×3** | **0 ×3, `coroutine 10`** | **0 ×3** |
| `give_carved` (mine) | R1 | 134 ×3, values right by luck, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3 | 1 ×3, heap-use-after-free |
| | E, E+B+owner | **0 ×3, right** | **0 ×3** | **0 ×3, right** | **0 ×3** |
| `copy_carved` (mine) | R1 | 134 ×3, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3, nothing printed | 1 ×3, heap-use-after-free |
| | E, E+B+owner | **0 ×3, `body holds 7 coroutines 30 32`** | **0 ×3** | **0 ×3, `30 32`** | **0 ×3** |
| `copy_jump` (mine) | R1 | 134 ×3, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3 | 1 ×3, heap-use-after-free |
| | E | **0 ×3, `10099`** | **0 ×3** | | |
| | E+B by frame (`rt-eb`) | **134 ×3, *compiler bug* after B freed a live value** | (not run) | | |
| | E+B+owner | **0 ×3, `10099`** | **0 ×3** (`rt-ebo5`; `rt-ebo3`: 134, stack-buffer-overflow in the hook) | **0 ×3, `10099`** | **0 ×3** |
| `sig_carved` (mine, a signal delivered on a carved coroutine) | R1 | 134 ×3, *compiler bug* | 134 ×3, heap-use-after-free | (not run) | (not run) |
| | E+B+owner | **0 ×3, `body holds 7 coroutines 1005 1105`** | **0 ×3** | (not run) | (not run) |
| `jumpco_own` (mine, `jumpco` on `own.h`'s carved stacks) | R1 | 134 ×3, *compiler bug* | 134 ×3, heap-use-after-free | 139 ×3 | 1 ×3, heap-use-after-free |
| | E | 134 ×3, *stack exhausted* | 134 ×3, the same | | |
| | E+B+owner | **0 ×3, `10 35021 body holds 7`** | **0 ×3** (`rt-ebo5`) | **0 ×3, `10 35021 body holds 7`** | **0 ×3** |
| `escape_carved` (mine) | every route | 134 ×3, REENTERED's words, false | 134 ×3, the same | 134 ×3, the same (R1) | 134 ×3 (R1) |
| `escape_heap` (mine) | every route | 0 ×3, `escaped: -1 then 42` | 0 ×3 | 0 ×3 (R1) | 0 ×3 (R1) |
| `asm_jump` (mine, an unseen jump R1 heals) | R1 | 0 ×3, `jumped, sum 35000` | 0 ×3 | 0 ×3 | 0 ×3 |
| | E, E+B+owner | **134 ×3, *stack exhausted in asm_jump.body*** (the price) | 134 ×3 | 134 ×3, *stack exhausted* (the price) | 134 ×3 |
| `own_heap`, `carved`, `two`, `three`, `lend`, `spawn`, `migrate`, `quit`, `unseen`, `unseen_carved`, `ph` | R1 | 0 ×3 each (`two`, `three`, `unseen` exit 134 under `--sanitize`: stack-buffer-overflow, at the base already) | | the repair's cells: 0 each but `unseen_carved` 134 (REENTERED, `dlsym` door) | |
| | E+B+owner | the same exit codes and first lines as the base's, eleven of eleven (`unseen_carved` `body holds 7` on this Mac) | as at the base: 0 ×3 each but `two`, `three`, `unseen`, 134 ×3 with the base's own stack-buffer-overflow | `own_heap`, `carved`, `three`, `lend` 0 ×3; `spawn` 0 ×3; `unseen_carved` 134 ×3, REENTERED's words, as at the base (rule 6 and the `dlsym` door); `two`, `migrate`, `quit`, `unseen`, `ph` not run | the same, but `spawn`: exit 1 ×3 under `rt-ebo5`, LeakSanitizer, the hook's buffer; **under `rt-ebo6` 0 ×3, right** |
| `quit_fresh` | R1 | 134 ×3, *stack exhausted* (draft 2) | 134 ×3 | 134 (the repair's) | |
| | E+B+owner | 134 ×3, *stack exhausted*, unchanged | 134 ×3 | 134 ×3, *stack exhausted*, as at the base | 134 ×3 |
| `jumpco` | R1 | 0 ×3 | 0 ×3 | 0 (the repair's) | |
| | E | 134 ×3, *stack exhausted* | | | |
| | E+B+owner | **0 ×3, `10 35021`** | 0 ×3 (`rt-ebo5`) | **0 ×3, `10 35021`** | 0 ×3 |
| panel 258's six (`alarm`, `handlejump`, `lendjump`, `lendok`, `pexit`, `room_order`) | R1 | `alarm` 134 (allocator refusal), `handlejump` 134 (handle gate), `pexit` 134 (thread ended), the others 0: R1's own results | the same | | |
| | E+B+owner | **the same six results**: five equal to the base's by `cmp`, `handlejump` differing only in the address its message names, which differs between the base's own runs | the base's six results (`alarm`, `handlejump`, `pexit` 134; the others 0) | the base's six results (`alarm`, `handlejump`, `pexit` 134 ×3; the others 0 ×3) | the same |
| the eight coroutine goldens | R1 | 8 of 8 `golden=equal` | 8 of 8 | | |
| | E | **6 of 8**: the two jump goldens *stack exhausted* | 6 of 8 | | |
| | E+B+owner | **8 of 8 `golden=equal`** | **8 of 8 `golden=equal`** | **8 of 8 `golden=equal`** | 7 of 8 under `rt-ebo5` (the `spawn` golden's leak); **8 of 8 with `rt-ebo6`**, its `spawn` golden 0 ×3, `golden=equal` |


**The harness's own verdict** (ran, this Mac, `HEROES_RUNTIME=rt-ebo3 ./heroes-next run tests/harness/main.hero --
./heroes-next run coroutine`, 10:01 to 10:28, `259-compiler-engineer-scratch/harness-run-coroutine-ebo3.txt`): **`run
(only coroutine): 50 passed, 1 failed`**, the 51 cases the brief counts by `grep -E 'coroutine'`, the 2702 to 2745
signal cases among them, each built and run plain and `--sanitize` by the form. The one failure is
`fixedbugs-a-c-longjmp-over-a-callback-inside-a-coroutine-gives-its-room-back at --sanitize`, *tripped a sanitiser*,
`stack-buffer-overflow ... in hero_heal_jump frame_aside.c:210`: the instrumented hook, which `rt-ebo5` exempts. That
case alone, run again by the same form with `rt-ebo5` (`-- ./heroes-next run
a-c-longjmp-over-a-callback-inside-a-coroutine`): **`1 passed, 0 failed`, exit 0** (`harness-run-one-ebo5.txt`).
So **the 51 pass on this Mac under the route**: 50 by `rt-ebo3`, whose plain build and whose other sanitized cells
an exemption from instrumentation cannot turn red, and the 51st by `rt-ebo5`. This Mac's sanitizer checks no leaks,
so the `spawn` golden's leak (§ 4) is Linux's alone, and `rt-ebo6` repairs it there.

## What a veto would compel; the conservative resolution; the prediction

**I veto, on soundness, the premise R1 heals on** (panel 258's R1, issue 2758, *"ratify R1 and build it"*; step 181a's
`stack_lane.c:51-55`): *a frame inside the thread's measured stack and outside every range the runtime knows is the
thread's own, and an entry of a frame below the asker is gone*. It is false of every switch the runtime does not see
onto memory of the thread's stack, and of every library that copies stacks, and what the program then gets is
**another frame's bytes in its values** (§ 1, § 2, § 8: `body holds 11` for 7, `body holds 1001` for 8, `coroutine 7`
for 10, ran, this Mac; exit 139 with nothing printed and a use-after-free under `--sanitize`, Linux arm64), which
design.md §1.12 forbids in its first sentence (*"must not segfault and must not corrupt memory"*) and in its third
consequence (*"The compiler and the runtime **check rather than assume**"*, *"provenance is a premise about the
world"*). **The veto compels a written answer from the author**: either the runtime stops judging an entry gone by its
frame's address (route E, built), or the record says why R1's premise stands against §1.12, with the programs above
named as the price. It yields, as the brief says, to the most robust resolution, which is the one I would adopt.

**The resolution I would adopt: E with B and the owner's proof (`rt-ebo6`), built.** No entry of the room or of the
lend regions is ever judged gone by its frame's address: each is a block or a region of its own, given back by its own
give in any order and on any thread (E). An entry is given back otherwise only on an event the runtime sees and whose
abandonment it can prove: a decoded jump of the setjmp family, whose jumper's chain of frame records reaches the
target, for the entries of the frames below the target on that chain whose own pointer is in the abandoned image; and
a `makecontext` that names a range anew, for the entries of frames inside it whose pointer is still in its memory (B).
Everything else an abandonment leaves stays held until the room's gibibyte, then aborts with *stack exhausted*, words
that should gain the gate's clause naming the callbacks that never returned.

**The conservative resolution would have been route A**: keep R1, and where a heal would fire and the asker's chain of
frame records does not climb to the thread's outermost record (§ 3's probe), refuse with true words instead of healing.
It costs nothing on the hot paths, it fires before any wrong value, and **it refuses six correct programs I ran**
(`own_carved`, `below`, `lend_carved`, `coval_carved`, `give_carved`, `copy_carved`) that the route I adopt runs
right, and a library that starts its coroutines at the current stack position and copies them (greenlet's design,
read) climbs to the thread's record and is not refused but corrupted.

**Two findings to file, by their class, whatever the sitting adopts** (I file nothing; `gh` reads only):
1. **`escape_carved`**: a correct coroutine on a carved stack that leaves for good by a `longjmp` to its host is
   refused by rule 6 in REENTERED's words, about a signal handler the program does not have, **on this Mac and on
   Linux arm64 at `3ec775b66`** (exit 134, 3 of 3 each); its heap twin runs. `blocking` by § Bounded discovery (a false
   message, a correct program refused), 2705's and 2755's class. No route of the room touches it. The fact that would
   repair it is § 3's: a jumper whose chain does not climb to the thread's outermost record is on another stack, so
   its landing below is no re-entry. Unbuilt: my cheaper gate, *no handler ever stood on the thread*, is false on this
   Mac, where the runtime pushes two entries of its own at start (`TRACE push serial=1 unsure=0`, ran), so I dropped it.
2. **`lend_carved`'s message**: at the base the regions' heal hands a live local to another frame and the program is
   then told *"by C writing past a field or an element it was lent, which is the binding's word (§ 13), or by a fault
   of this compiler"*, false of a correct binding (ran, both platforms). The route removes it; R1 kept, it is a defect.

**The prediction**: landed as `rt-ebo6`, the shapes `own_carved`, `below`, `lend_carved`, `coval_carved`,
`give_carved`, `copy_carved` and `copy_jump`, added to `tests/golden/run/` with this report's expected lines, pass the
`run` form on darwin-arm64 and linux-arm64, and **`cost_cb` at `-O2` stays within +1% of the base's instructions
retired** (+0.49% measured here); **falsified** if any of those cases fails or `cost_cb` moves past +1%, checkable at
the gate of the lane that lands the route, at M-rebel-library's close or the milestone that takes it.


## The verdict structure

- `verdict`: **veto** (of R1's healing by address, on soundness; it yields to the resolution above, which I built)
- `section`: design.md §1.12 (*"must not segfault and must not corrupt memory"*; *"check rather than assume"*); §1.1
  and Part 5 for the cost: the route adds no construct, so no core and no sugar, and lands in the runtime alone
- `implementation_cost`: **runtime only**, the prototype `rt-ebo6` +288 −7 lines (`diff`): `runtime/parts/callback.c`
  277 → 383, `frame_aside.c` 153 → 310 (past 300: a landing moves B into a part of its own), `lend_aside.c` 214 → 234,
  `frame.c` 270 → 276, `lend.c` 1,148 → 1,150, `sigjump_save.c` 255 → 256, `sigjump_make.c` 80 → 81, `rebind_jump.c`
  287 → 289 (`wc -l`); **0 lines of `selfhost/`** (`emit/large_value.hero` 242, `emit/callback_thunk.hero` 429
  unchanged; emitted C identical bar `hero_program_sources`, ran on three programs). R1's thread-room paths and
  `stack_lane.c` (165) stay in the prototype, dead; deleting them is unmeasured. Hot paths at `-O2`: +1.0 instruction a
  callback, +958 a lend, about +1,940 a 32 KB take (`cost.sh`, ran)
- `needed_for_self_hosting`: **no** (the compiler takes no large value: `grep -c 'hero_frame_take(' seed/heroes.c`
  prints 1, a string literal of its emitter)
- `argument`: R1 heals by the order of addresses on one stack, and nothing the runtime can read separates a frame a
  jump abandoned from a frame an unseen switch suspended above it, or a copied-out coroutine whose frames shared the
  address: the chain of frame records separates the first two and fails the third. A route that never judges by
  address cannot hand a live value away under any switch; what it cannot prove abandoned it keeps, a leak with a
  bounded, told end. The owner's proof (the entry's own pointer in the abandoned image) is what made B survive the
  copying shape, where B by frame failed it. Robustness outranks the measured cost (CLAUDE.md § Precedence).
- `prediction`: landed as `rt-ebo6`, `own_carved`, `below`, `lend_carved`, `coval_carved`, `give_carved`,
  `copy_carved` and `copy_jump` added to `tests/golden/run/` pass the `run` form on darwin-arm64 and linux-arm64, and
  `cost_cb` at `-O2` stays within +1% of the base (+0.49% here); falsified by any of them failing, at the gate of the
  lane that lands it
- `condition`: a program in which the owner's proof gives a live entry back (then B goes and E stays alone, with its
  leaks); a fact that tells an unseen abandonment from an unseen suspension on every switch, copying included (then R1's
  healing could return for unseen jumps); or a golden of the tree the route breaks that no repair inside it mends


## One line per question

1. **Mechanism**: the coroutine's thunk entry (`callback.c:262` → `frame.c:182`; `lend.c:452` → `:440`) gives back
   `body`'s room and regions, traced on this Mac and in the arm64 image; the regions then give C another frame's bytes
   with no fault, and the program prints wrong values and a false message (plain) or faults (`--sanitize`). *Would change
   it*: a trace where a take, not the thunk, gives first.
2. **Judgments**: fourteen, J1 to J14; J1 to J7 and J10 lie, J10 (rule 6) on a seen jump, **on this Mac too**
   (`escape_carved`); HELD (J11) does not lie and its words are true: ratify it. *Would change it*: a program on which
   HELD fires in words false of it.
3. **Route A**: a detector exists, the frame-record chain from the heal (5 or 6 steps to the coroutine's first record
   against 8 or 9 to the thread's), and it fires before any wrong value; it refuses six correct programs and greenlet's
   copying fools it. *Would change it*: a route that runs those six right being unbuildable.
4. **Route B**: built; by frame it fails the copying shape, with the owner's proof and the `makecontext` event it keeps
   2752's and 2756's repair, 8 of 8 goldens and panel 258's six; its price is `asm_jump` (*stack exhausted*), door 6,
   doors 8, 12, 13 and Windows. *Would change it*: a program where the owner's proof frees a live entry.
5. **Route C**: a detector, not an identity: copying defeats it, Windows has no frame records, and it costs 5 to 9 loads
   where it walks. *Would change it*: a shape where the chain tells two copied coroutines apart.
6. **Route E**: built; sound under every switch I could write, +958 instructions a lend and about +1,940 a 32 KB take in
   the prototype's asides at `-O2`; alone it breaks two goldens, so it needs B. *Would change it*: a per-thread aside
   measured, which would lower the price, not the verdict.
7. **Route D**: unneeded under E; the sanitizer's call reaches `--sanitize` builds alone. *Would change it*: a switch E
   cannot keep a value across.
8. **Unlisted**: the copying shape, which only E (and B with the owner's proof) survives; G5's cookie is fooled by it
   toward giving back (the running coroutine's frame overwrites the copied-out one's slot), and G1's *retire, do not
   reuse* keeps the value but reclaims nothing a heal judged, which is E's leak with R1's judgments kept (both read,
   unrun). *Would change it*: a copying library that keeps its
   coroutines' frames at different addresses (then it is not copying).
9. **Emitter**: owes nothing; emitted C identical bar `hero_program_sources` (three programs ran; the compiler's own:
   § 9b). *Would change it*: an emitter that keeps only a derived pointer to an entry (B would then leak).
10. **Spec**: one sentence, for the price (a value C's unseen abandonment left stays held, the program aborting past
    1 GiB); `arm-p` is true under the route and false at the base, `arm-r` and `arm-t` describe aborts that do not
    happen. *Would change it*: the warden counting the gate's words as enough.

