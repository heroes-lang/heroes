# Defect 098 closed: an end names the life it was announced for, so two threads over one allocator give back their own handles

2026-09-26, M-agreed-retention step 16, in lane `bcfc342b`, merged `e79fa17f`.
Found by the skeptic seat over the landing of panel 177's items 1 and 2.

- [x] **098 — two threads that acquire and release handles from one C allocator abort a correct program** | the live set records an end after C returns, so between C's release and the runtime's record the other thread can be handed the same address, the runtime overwrites the live entry with the new life, and the first thread's record then deletes the new one: the second thread's correct release is a stray | `runtime/parts/alloc.c` (`hero_handle_acquired` on a live entry with an end pending, `hero_handle_ended`) · **closed 2026-09-26**

    **Origin:** the skeptic seat over the landing of panel 177's items 1 and 2
    (the poison and the dead set), 2026-09-25, which measured it on the
    runtime before that landing too; reproduced by the coordinator the same day
    on the trunk's compiler at `9e17d471`, so it is older than the landing.
    Panel 177's completeness critic had listed threads as unmeasured for both
    of its routes.

    **The reproducer.** Two threads spawned with `hero_thread_spawn`, each
    200,000 times `n = sn_new()`, `sn_value(n: n)`, `sn_free(n: n)`, over ONE
    C pool with a free list under a `pthread_mutex_t`, which is what every
    real allocator is; `sn_new() -> S acquires sn_free`, `sn_free(n: S
    consumes)`. `check` 0; `run` **134, ten of ten, on Darwin arm64**, *1 C
    handle(s) given back that were never taken*; and ten of ten on Linux x86-64 and on Linux arm64, the same line. Windows unrun.

    **Why it is a defect.** A correct program is refused, and the class is
    every program that hands handles to C from more than one thread over one
    allocator, which is the ordinary case for a server. The runtime's own
    pending count (defect 084) already knows an end is in flight when the
    second thread's acquisition arrives: an acquisition of a live address with
    an end pending is C having released it inside that call and handed it out
    again, so it is one more reference, not a replacement, and the end in
    flight then leaves the new life standing. The repair makes the set say
    that, and pins it with the reproducer as a golden over the shared pool.

## The repair

**An address is not an identity, so an end now names a life.** Two repairs
that settle the ends by the order they land were weighed first and refused. The
filing's own, *an acquisition of a live address with an end pending is one more
reference*, leaves the old life's place unpoisoned when its end lands. A count
of the old life's ends in flight, built in the lane and refused before its
commit, poisons the wrong place in the same race: a new life holding two
references (`retains`) whose own end landed before the old one's would lose
its name while that name still held a reference. The repair that landed is
exact instead:

- **Every life has a number no other life at any address ever had**, issued
  under the set's lock at each acquisition and at each reference that begins a
  life.
- **The announcement answers it and the end hands it back.**
  `hero_handle_ending` and `hero_handle_transferring` return the number; the
  emitted C keeps it in a local of the call's own block, `hero_life_<position>_<index>`,
  one per handle the argument reaches; `hero_handle_ended` and
  `hero_handle_kept` take it. An end whose life is no longer the set's, because
  C released the address inside the call and handed it out again, ends the life
  it was announced for, poisons that place, and leaves the new life standing,
  neither counted down nor remembered as dead. Runtime ABI 25 to **26**.
- **A reference taken by a call that ends nothing**, at an address whose every
  reference is being given back by other calls still inside C, begins a new
  life rather than joining the old one: C made a new object there, or kept the
  old one alive, and either way one life is owed to the reference once the ends
  land. `hero_handle_retained` is told whether its call also ends a handle
  (`ends_here`), since only then can an end in flight be the same call's; the
  releasers are compared on every other join.
- **The defence in depth stays loud**: a handle that reaches an end with life
  number zero was never announced, an emitter fault, and is reported as the
  stray it would have been.

## What stays as it was

A copy given back after C handed its address out again is still the new life's
to the runtime, panel 177's item 4: `limit-a-copy-given-back-after-c-reused-its-address-is-not-caught`
pins the release through such a copy at exit 0, the shape defect 077's record
measured and left unpinned. And a call that ends a handle AND takes a reference
at an address another thread is giving back joins the old life, since the
runtime cannot tell that end from its own; it is the one interleaving this
repair does not make exact, and it is argued, not run.

## The measurements

| program | before (trunk `7dc73925`, runtime ABI 25) | after (lane `bcfc342b`, ABI 26) |
|---|---|---|
| the two-thread reproducer, 200,000 rounds a thread over one pool | run **134**, five of five on Darwin arm64, the stray's message three times and the dead set's twice; ten of ten on three platforms when filed | **0**, ten of ten on Darwin arm64, `a 200000` and `b 200001` |
| `run/fixedbugs-two-threads-over-one-allocator-give-back-their-own-handles` | — | 0, plain and `--sanitize`, on Darwin arm64, Linux x86-64, Linux arm64 and the Windows box |
| `run/fixedbugs-a-callback-handed-the-address-a-release-freed-begins-a-new-life`, its first half: a node C hands a callback inside the release, taken back after it | run **134** at the correct give-back, the stray's message | 0 |
| the same golden's second half: a reference, `retains sn_put`, taken at that address by a call that ends nothing | run **134**, a wrong join, `sn_free\|sn_free_then` against `sn_put` | 0 |
| `run/limit-a-copy-given-back-after-c-reused-its-address-is-not-caught` | — | 0, as panel 177's item 4 states; a later read of the new cell stops before C at 134 |

Gate in the lane: the compiler's 721 tests; the full net at 2510 passed and 3
failed, all three this repair's own (a file past the `layout` ceiling, the life
counter missing from `suite_runtime`'s shared-by-decision list, that suite's
floor) and green after them; the net's own 167; the seed regenerated and the
fixpoint held. Every run golden, 206, plain and with `--sanitize`, on Linux
x86-64, Linux arm64 and the Windows box: **1236 runs**, every exit and stdout as
its golden says, 28 skipped where the machine lacks raylib or `netdb.h`, as the
harness skips them. On the trunk after the merge `e79fa17f`: the index equal
to the lane's tree, the compiler's 721 tests, the reproducer at exit 0.
