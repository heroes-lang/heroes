# Panel 177 — compiler-engineer

Read `00-shared.md` first. Your directory is
`<scratchpad>/177-compiler-engineer/`, HEAD `521c5e02`; build the compiler
there from the seed. You judge the ceiling (design.md §1.1, §1.7, Part 5) and
cost, with veto on soundness.

## Pointers, read at HEAD while this brief was written

- `selfhost/check/consuming.hero` — the `consumes` rule, 114 code lines at its
  landing (design.md Part 8 wart 20); its header at `:15-35` states the checker
  has no flow analysis and names the copy it does not reach.
- `selfhost/check/leasing.hero:29` — the same statement about flow.
- `selfhost/check/join.hero` — joins branch TYPES (its header).
- `selfhost/check/acquiring.hero`, 278 lines — `acquires` and `borrows`.
- `selfhost/emit/handle_traffic.hero`, 143 lines — `for_call` `:36` emits
  `hero_handle_acquired` after the call, `before_call` `:101` emits
  `hero_handle_consumed` before it, `emit_each` `:126` walks fields and array
  elements. A NON-consuming handle parameter emits nothing today.
- `runtime/parts/alloc.c`, 610 lines — the live set: statics `:161-169`, one
  mutex, `hero_handle_slot` `:380`, `hero_handle_grow` `:389`,
  `hero_handle_acquired` `:402`; `runtime/heroes_runtime.h:205-206` the two
  entries.
- Panel 176's prototype of the set, `transfers`, `retains`, the call-site rule
  and `contract_differs` is at
  `<scratchpad>/176-compiler-engineer/work/prototype-176-final.diff` (57071
  bytes). You may read and apply it in your own directory; panel 176 adopted it
  **without** the call-site rule and **with** two changes its critic measured:
  the transfer names its releaser, checked against the set (V1c), and `retains`
  keeps the live set instead of replacing it.

## What to settle, by building and running

1. **Question 1's routes M, R, P and S** (00-shared.md). For each: the lines in
   `selfhost/` and `runtime/`, whether the ABI stamp moves, what the emitted C of
   one marked call becomes. **Build the two you judge cheapest far enough to run
   the five reproducers**, and report `check` and `run` for each program before
   and after, three of three. Docker images `heroes-linux` and
   `heroes-linux-arm64` are on this machine; the Linux legs need only
   `docker run --rm -v <your dir>:/src:ro <image> bash -c '…'`.
2. **Every route you build runs over the 510 `.hero` files** under
   `tests/golden/` and `examples/` (`find tests/golden examples -name '*.hero'`
   read 510 at HEAD): which verdicts move, and why each moved.
3. **R's price per call**: a loop of a million non-consuming calls on one live
   handle, before and after, `/usr/bin/time -p`, with nothing else running on
   the machine while the clock runs (other seats may be building: say what
   `real` against `user + sys` reads, and discard a run where `real` is far
   above them).
4. **R and a `borrows` handle.** A handle a `borrows` call hands back is never
   in the set. Write the shape — `sqlite3_next_stmt`'s is the canonical one —
   and say what R does to it, and what it would cost to make R correct there.
5. **P and `==`.** `u1_static.hero` prints `a == b` after `a`'s life ended.
   What does a poisoned binding make of that line, and of any other read of the
   dead value the language allows?
6. **M and the claim about flow.** Is *the checker has no flow analysis* still
   true at HEAD? If any state walk exists — `lend_extent.hero`, `leasing.hero`
   — say whether M could reuse it, and price M over an `if`, a `while` and a
   `match`.
7. **Question 2: the success clause.** Panel 176's resolution item 3 says a
   transfer that happens only on success names the result that means success,
   and the obligation ends only then, the entry keeping its set on failure.
   Price it at the two placements the ergonomist is reading — on the
   parameter (`val: Json transfers json_object_put on 0`) and on the result
   (`… -> i32 when 0`) — parse, check, emit, runtime. Say what it does to a
   call with two transfers, and to a `void` result.

## Say for every claim which of these it is

Run, on which legs; or argued, and why it could not be run.
