# Panel 259, the ffi-pragmatist: each draft clause against the cells, and the repaired clauses

Written 2026-11-25 by the ffi-pragmatist seat, in its copy at `3ec775b66`. Each clause is marked **true** (the
program and its cell), **false** (the program that falsifies it) or **untested** (why). The cells are this report's
(`ffi-pragmatist.md`, outputs in `.claude/worktrees/259-ffi-pragmatist-scratch/{mac,x86,arm}/runs/`) unless they
name the repair's (`259-briefs/runs/`). "Mac" is Darwin arm64, stage 2 built in this copy; every Linux cell names its
image and base.

## arm-r

> A callback C runs on a stack C switched to by code of its own, not by `swapcontext` or `setcontext`, inside the
> stack of the thread that called C, may abort, saying the switch was not seen; a stack from `malloc` or `mmap`, or
> one given to `makecontext`, is followed.

| clause | mark | the program and its cell |
|---|---|---|
| r1 *a callback ... on a stack C switched to by code of its own ... inside the stack of the thread ... may abort, saying the switch was not seen* | **false today**; **untested** as route A's promise | `bfc_host` (Boost.Context 1.92, Mac): `body holds 11` twice, a wrong value, **before** the abort, whose words are *"internal error: ... (compiler bug)"*, not *"the switch was not seen"*; `own_carved`, `cpo_host` the same; Linux arm64 at `3ec775b66`, `own_carved` exit 139 with nothing printed (the repair's). No runtime here detects the switch, so the promise has no cell |
| r2 the scope *not by `swapcontext` or `setcontext`* | **false** as the class's edge | `cpu_heap` (Mac): switched only by `swapcontext` (counter `swap 7`, `made 2`), corrupts (null-pointer panic in `hero_frame_aside_unlink` or the *compiler bug* panic, 3 of 3); `unseen_carved` on Linux arm64 at `3ec775b66` switches by the runtime's `swapcontext` and aborts in `HERO_SIGNAL_REENTERED`'s words (the repair's) |
| r3 *a stack from `malloc` or `mmap` ... is followed* | **false** | `cpu_heap` (`malloc`, Mac): corrupts; `cpo_heap_told` (`malloc`, Mac): crashes. **True** only for a stack one coroutine owns: `bfc_heap`, `own_heap`, `cpo_heap` untold (Mac, 3 of 3, plain and `--sanitize`) |
| r4 *or one given to `makecontext`, is followed* | **false** | `cpu_heap`: every stack given to `makecontext` (counter `made 2`), corrupts. **True** for `carved` (`made 2`, right) and `aj_carved` (OpenSSL's fibers on carved stacks through `ASYNC_set_mem_functions`, Mac, 3 of 3 plain and `--sanitize`). On Linux the `RTLD_NEXT` and C-library-handle doors leave a `makecontext` unseen (the repair's arm64 cells at `3ec775b66`) |

## arm-t

> C that switches stacks by code of its own tells the program before it runs a callback there, by
> `hero_stack_entered(low: ptr, bytes: i64) -> ()` (`hero_os.h`); a callback C runs, inside the stack of the thread
> that called C, on a stack the program was not told of may abort, saying so.

| clause | mark | the program and its cell |
|---|---|---|
| t1 *C ... tells the program before it runs a callback there* (a binding can, and it suffices) | **true for a stack one coroutine owns**, **false as sufficient** | `bfc_host_told`, `bfc_below_told` (Mac): right, 3 of 3, plain and `--sanitize`, with one call in the binding's header. `cpo_host_told` (a copying library, told): the *compiler bug* panic, 3 of 3. `cpo_heap_told`: **the tell turns a program that runs right untold (`cpo_heap`) into a crash** (null-pointer panic in `hero_frame_aside_unlink`, 3 of 3; `heap-use-after-free WRITE of size 262144` under `--sanitize`) |
| t2 *a callback ... on a stack the program was not told of may abort, saying so* | **false today**; **untested** as a promise | as r1: wrong value first, then words that name a compiler bug; no runtime detects the untold stack |
| t3 the signature `low: ptr, bytes: i64` | **true that a binding can call it**; **false that a range is the fact needed** | the prototype `p259_stack_entered(void *, size_t)` compiled and ran (`bfc.h`); a range names two coroutines that occupy the same addresses in turn as one stack (`cpo_*_told`, `cpu_heap`) |

## arm-p

> A value a function holds keeps it across every switch of stacks C makes, by `swapcontext` or by code of its own,
> wherever the stack lies.

| clause | mark | the program and its cell |
|---|---|---|
| p1 the promise | **false today** | `own_carved`, `below`, `bfc_host`, `bfc_below`, `cpo_host`, `cpo_host_told` (Mac, 3 of 3); `cpu_heap` (Mac, every switch seen); Linux arm64 at `3ec775b66`: `own_carved` and `below` exit 139 (the repair's) |
| p1 where it holds today | true | `bfc_heap`, `own_heap`, `cpo_heap`, `carved`, `aj_carved`, `unseen_carved` (Mac) |

## The repaired clauses

**The class, repaired** (every draft names it by the switch; the cells name it by the stack): a callback's large value
is lost **where two coroutines' frames can stand at the same address or in one range this runtime orders by address**:
(a) stacks carved from the thread's own that no `makecontext` the runtime wraps named, and (b) one region several
coroutines occupy in turn, by any switch, named or not. A stack that one coroutine owns from its first switch to its
end is followed, by any switch, wherever it lies, **once the runtime knows its range or cannot name it at all**.

**Repaired arm-r** (a description of a refusal; its abort half has no runtime to run against):

> A callback C runs on a stack carved from the thread's own by code of C's own, or on a stack another coroutine
> also runs on in turn, may abort, saying so; a callback on a stack only it runs on, from `malloc`, `mmap` or
> `makecontext`, keeps its values.

**Repaired arm-t**: no range clause survives the copying cells. A tell, if one is kept, carries **which coroutine
runs now**, at every switch, and a binding that does not call it loses nothing that runs today:

> C that runs several coroutines on its own switch may say which one runs, at each switch, by
> `hero_coroutine_now(id: i64) -> ()` (`hero_os.h`); a callback C runs is never given another's value, told or not.

**Repaired arm-p** is arm-p itself: it is the sentence §1.12 asks for, and it is false today, so it is owed **by a
runtime change**, never by a sentence alone.

## The repaired claims, run

Run on this Mac at `3ec775b66`, outputs named above (Linux rows in the report, by base):

- *"a callback on a stack only it runs on, from `malloc`, `mmap` or `makecontext`, keeps its values"*: `bfc_heap`
  (malloc, own switch), `own_heap` (malloc, own switch), `carved` and `aj_carved` (`makecontext` on carved memory, the
  first `aj2.h`): **right, 3 of 3 each**. `cpo_heap` is two coroutines on one `malloc` region, untold, and right: it
  sits in the clause's *may abort* half and keeps its values only because a stack nobody named is never judged
  (`frame_aside.c:99`), which the tell undoes (`cpo_heap_told`). The `mmap` word: **untested here** (no cell maps a stack).
- *"a callback ... on a stack another coroutine also runs on in turn, may abort, saying so"*: **false today on every
  cell** (`cpu_heap`, `cpo_host`, `cpo_*_told` print or abort in words that name a compiler bug or a null handle);
  it is a promise only a new runtime could keep, and none is built here.
- *"a callback C runs is never given another's value, told or not"* (repaired arm-t's last clause, arm-p's
  promise): **false today** (the six cells of p1).

## The repaired arm-t, run (this Mac at `3ec775b66`, `cells-id.sh`, `cells-01.sh`, `make-ident-runtime.sh`)

The tell *"which coroutine runs"*, prototyped (a runtime copy keyed by a thread-local the binding sets around each
switch; R1 untouched): **`bfc_host_id`, `cpo_host_id` (greenlet's design), `cpo_heap_id`, `cpu_heap_id` and
`bfc_host_id_01` are right, 3 of 3 each**, plain and `--sanitize` (but `cpu_heap_id-san`, ASan's own limit with a
copied stack). So the repaired clause's first half, *"C ... may say which one runs, at each switch"*, is **true** as
a binding's act and as a repair, **for the bindings that say it**.

Its last clause, *"a callback C runs is never given another's value, told or not"*: **false** under that prototype for
the untold (`bfc_host_01` and `cpo_host_01` against the identity runtime, untold: `body holds 11`, 3 of 3). It holds
only under a runtime that keeps no room by a frame's address for an untold coroutine; the no-heal copy keeps the
value (`body holds 7`) and then aborts on the room's order (*compiler bug*, 3 of 3), so **no runtime built here makes
that clause true**: it stays the promise route E owes, untested until a route-E runtime exists.

## Linux x86-64 at `3ec775b66` (emulated; the report's § Question 7), against the same clauses

- **r1** false: `own_carved` exit 139, 3 of 3, nothing printed; `cpo_host` the same.
- **r2** false as the class's edge: `cpu_heap` (counted `made 2 swap 7`, every switch seen) exit 139, 3 of 3;
  `unseen_carved` (counted `made 0 swap 3`) exit 134 in `HERO_SIGNAL_REENTERED`'s words (the critic's F18, re-run at
  this base: the same 823 bytes as the repair's arm64 cell, `cmp`).
- **r3** false: `cpu_heap` (`malloc`) segfaults; `cpo_heap_told` (`malloc`, told) segfaults. True for `cpo_heap`
  untold and `aj_heap`.
- **r4** false: `cpu_heap` (given to `makecontext`); and the door is wider than the drafts and the lane's report say:
  `dlvsym` by all three handles answers the C library's `makecontext` (`dl3` `state 1`). True: `aj_carved` (OpenSSL
  3.5.7's fibers carved through its allocator, `made 2 swap 8`), exit 0, 3 of 3.
- **t1** false as sufficient: `cpo_heap_told` and `cpo_host_told` segfault after `body holds 7` twice, 3 of 3 each.
- **p1** false: `own_carved`, `below`, `cpu_heap`, `cpo_host` segfault.

## Linux arm64 at `3ec775b66` (native; the report's § Linux arm64)

Every mark above holds there: r1 (`own_carved`, `cpo_host` 139), r2 (`cpu_heap` 139 with `made 2 swap 7`;
`unseen_carved` 134 with `made 0 swap 3`), r3 (`cpu_heap`, `cpo_heap_told` 139; `cpo_heap` and `aj_heap` right), r4
(`cpu_heap`; `dlvsym` unseen by all three handles, `dl3` `state 1`; `aj_carved` right), t1 (`cpo_heap_told`,
`cpo_host_told` 139), p1 (`own_carved`, `below`, `cpu_heap`, `cpo_host` 139). The repaired arm-t's told half:
`cpo_host_id`, `cpo_heap_id`, `cpu_heap_id` right, 3 of 3, plain and `--sanitize`, and `cpo_host_id_01` right; its
untold half: `cpo_host_01` 139 today, and with no heal `body holds 7` then the room's order abort. And one fact for
the runtime's own reading of the identity: `cpu_heap` right with no tell under `make-ctxid-runtime.sh`, 3 of 3.
