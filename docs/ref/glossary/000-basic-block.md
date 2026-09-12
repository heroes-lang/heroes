# Basic block — the stretch of road without intersections

**Origin**: prediction 000 — the predicted loop shape was the rotated form,
the spike's is the naive one — and the author's question: *"why are `i = 0`
and `total = 0` a single block?"* — 2026-08-03, `docs/journal/000-setup.md`
§3, spike `tools/spike/02-loop.c`.

## The definition

A basic block is **the longest possible stretch of road without
intersections**: a run of instructions that, once entered, always execute
all of them, in order, with no way to enter or leave in the middle. The
instructions are the houses along the street: the street does not change
name at every house, only at intersections. Forks (jumps) exist only
**between** blocks, never inside one.

## The two rules of forced cuts

1. **A fork on the way out** — where execution can continue to two different
   places (a test), the block *must end*: "what comes next" is no longer
   unique.
2. **A merge on the way in** — where execution can *arrive* from more than
   one place (the target of a jump), a block *must begin*: a jump always
   lands at the top of a block, never in the middle.

Everything else glues together. That is why `i = 0` and `total = 0` share
one block — no rule forces a cut between them — and likewise
`print(total)` + `return`. **The number of blocks is not chosen: it is
deduced.** This is also why the M-ir-lowering station (lowering to IR) can be written
without aesthetic taste: there are no decisions to make, only rules to
apply.

## The canonical example (spike 02)

```
function main()
    i: int @ 0
    total: int @ 0
    while i < 5
        total @ total + i
        i @ i + 1
    print(total)
```

```
        goto bb0
           ↓
   bb0:  i = 0, total = 0
           ↓
   bb1:  i < 5 ?  ←───────────┐
       yes ↓          no ↘    │
   bb2:  total = total+i      │      bb3:  print(total)
         i = i+1  ────────────┘            return
```

Four blocks, all deduced: bb0 ends because bb1 must begin (rule 2: the
back-edge from the body lands on the test); bb1 ends because the test is a
fork (rule 1); bb2 ends with the jump back; bb3 collects everything after
the loop.

## The counterexample that lights it up

If bb0 and bb1 were fused into one block, the arrow coming back from the
body — which always lands at the *top* of a block — would re-execute
`i = 0` and `total = 0` on every iteration: **infinite loop**. The cut
between bb0 and bb1 is forced; the one between `i = 0` and `total = 0` is
not — and where a cut is not forced, you do not cut.

## The if/else diamond

Every `if/else` produces the same picture: one fork, two sides, one merge —
**three** new cuts, not one.

```
   bbA:  …then:  x > 2 ?
       yes ↓        ↘ no
   bbB: then-side   bbC: else-side
          ↘         ↙
   bbD: whatever follows the if
```

The cut almost everyone misses is the merge (bbD): at that point the source
says nothing — the `if` closes silently, no keyword announces "here the
branches rejoin". **Forks are written; merges are deduced.**

## Incoming arrows

Two arrows entering a block = a meeting point. If both come from above it is
a merge (the bottom of the diamond); if one comes from below, that arrow
closes a ring in the graph — and **a loop is nothing but a ring in the
block graph**. In the spike, the two arrows entering bb1 *are* the loop.

Historicized note from prediction 000: the "test at the bottom of the body"
shape predicted there really exists — it is *loop rotation* (do-while
form + a guard block), which optimising compilers produce on their own.
Heroes' M-ir-lowering station deliberately emits the naive shape — the test in its own
block, two incoming arrows — because it is uniform to emit and easy to
verify; clang rotates it by itself at `-O2`.

## Check question

Add an `if x > 2` with two branches inside the spike's loop body: how many
blocks in total? *(Answer: 7 — the 4 from before, with the body exploded
into the diamond: the head with the fork, the two sides, and the merge that
closes with `i @ i + 1` and jumps back to the test.)*
