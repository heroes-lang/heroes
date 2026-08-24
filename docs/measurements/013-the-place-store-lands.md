# 013 — The place store lands: the commonest line stops being quadratic

Date: 2026-08-24. Panel 037 adopted the shape (2026-08-12), panel 088
prototyped it and left it waiting on the one trigger that was the author's
alone; the author pulled it this morning — *"approvo panel 037"* — and this
file is the landing's record. **Everything below was run in the session that
writes it**, serially, under `caffeinate`, `real ≈ user` checked on every
timed row (measurement 012's discard rule).

## What landed

- **Runtime** (`runtime/parts/cow.c`): `hero_array_push_owned(HeroArrayHeader
  **slot, const void *value)` — `**` per panel 088 R4 condition 1, uniqueness
  from the place, never from a count. Refcount 1 with room appends in place;
  anything else copies into a geometrically doubled block (Rust/libc++/Swift's
  factor; folly's 1.5 exists to reuse freed memory, which a refcounted block
  regains on the copying path anyway). Never realloc: the value may point into
  the old block. The guard the two panel prototypes disagreed on is condition
  2, kept: the copy may incref THIS array (`n.kids @ n.kids.push(n)`), so the
  refcount is re-read across it and a moved count undoes and snapshots.
  `HERO_RUNTIME_ABI` 14 → 15 — panel 037's bump honoured, not overturned.
- **IR** (`selfhost/ir.hero`): one op, `push_owned(place, value)`, and every
  exhaustive match in the tree names it — most on an existing no-payload arm
  line, which is what kept eleven frozen-ceiling files at their exact size.
- **The pass** (`selfhost/ir_place_store.hero`, new): between mono and own,
  `store p ← call push(load p, v)` with a BARE place, load and call read
  exactly once, all in one block with no write to p's root in between,
  becomes the one growing instruction. On the IR every safety clause is a
  fact about the value in hand (CLAUDE.md §11): an `@` escape in the pushed
  value (`qs @ qs.push(bump(@qs))`) is a write between load and store, so the
  pattern refuses itself and the classic order stays observable.
- **Why before own**: rule 5 gives a call's result a synthetic owning slot, so
  after own every accumulator push reads refcount 2 — panel 037 measured the
  old gate firing on 0 of 100,000 pushes for exactly this reason. Before own,
  the count the runtime sees is the true one.
- **Spec**: the cost sentence was rewritten in the same commit (panel 088 R3,
  pre-registered): `xs @ xs.push(4)` *grows in place while nothing else holds
  `xs`*; the quadratic warning stays only where it stays true, on `str`
  concatenation. `heroes measure`: **3512 → 3506** (the warden predicted 3508,
  falsifier ≥3512; the landed wording is 6 under the falsifier). Ledger row
  added, count re-pinned at 39.

## The numbers

| batch | 2026-08-23 (before 012) | after 012 | place store landed | vs 012 | cumulative |
|---|---|---|---|---|---|
| `build selfhost/main.hero --emit-c` | 944.76 s | 424.67 s | **188.51 s** | **2.25×** | **5.01×** |
| `test selfhost/main.hero` | 20m35s | 446.08 s | **198.72 s** (492 green) | 2.24× | **6.2×** |

The intermediate rungs, each measured at its own step: the transition binary
(classic code compiling the new source) emitted in 433.42 s; the same source
compiled BY a place-store binary but still carrying classic code inside
itself took 429.78 s; the first fully-landed binary took 184.75 s — the drop
lives in the compiler's own loops, exactly where measurement 012's profile
said 78% of the time was sitting. The committed compiler, with the ownership
repair below included, builds itself in **188.51 s**, fixpoint byte-identical
(D2 == D3, three-link chain because the repair changes the IR the previous
binary did not emit).

## The defect the honest counts exposed — found by this landing's own test

`place-store-c5.hero` case 5 (`qs @ qs.push(bump(@qs))`) died at
*"array with no element descriptor"* under the landed compiler and ran clean
under yesterday's — and yesterday's cleanliness was an ACCIDENT. A borrowed
load of a slot that is then passed `@` to a call was never given a reference:
classically it survived because rule 5's synthetic slot held every
accumulator at refcount 2, so the header under the borrow could not die.
`hero_array_push_owned` makes the count honest — that is its whole point —
and the borrow was suddenly a use-after-free. ASan on yesterday's compiler:
clean (the accident, verified, not inferred). The repair is in the ownership
pass where it belongs: a refcounted load whose value outlives a write to its
own root (an `@` call, a `push_owned`, a store — `load_survives_write`,
one pass, fast path free) takes an incref and rule 5's own slot machinery.
Cost on the build: inside run-to-run spread (185.42 → 188.51 across chain
links). `place-store-c5` case 5 is the test that fires without it, and the
sanitizers agree: micro-case and full c5 both ASan+UBSan clean.

## What was re-verified

492 tests green (the suite grew by this work's own: the pass's four, source's
extent split earlier today). The net green after three instrument updates,
each the guard doing its job: 6 `emit/` goldens re-read and refreshed — the
diff is ONE line each, the ABI stamp 14 → 15; 291 blessed emissions
regenerated with `UPDATE_EMISSION=1` (every unit now asserts ABI 15 and the
rewritten pushes); the spec ledger gained its row and its pinned count moved
38 → 39. `place-store-c4`/`c5` are panel 037 item 2's named cases, present at
last (panel 088 R6 found them missing), `# UNVERIFIED — pending debrief`.

## Panel 088 predictions, scored at the milestone that lands it

- **#2 (engineer)**: the diff exceeds 200 lines over `selfhost/` + `tests/` —
  **confirmed**, see the commit stat; panel 037's ≤200 condition was breached
  exactly as predicted.
- **#3 (warden)**: landed wording at 3508, falsified ≥3512 — **3506
  measured**: under the falsifier, 2 under the point estimate.
- **#9 (historian)**: the first thing the repair breaks borrows a buffer
  across a *user function value* (`sort`/`map`/`fold`) — **half right,
  measured**: the first break was a borrow across a *call*, but through an
  `@` parameter, not a function value; the primitives named never fired
  (c4/c5 exercise `fold`-shaped reads and stay green).
- **#1 (engineer)**: `build tests/harness/main.hero --emit-c` stays above
  9.8 s against its 12.29 s baseline (speedup under 1.30×) — **falsified
  downward: 7.96 s measured** with the landed compiler.
- **#4 (warden)**: `heroes lex selfhost/ir_lower.hero` from 84.16 s to under
  10 s, falsified above 40 s — **confirmed: 5.12 s** (the loader repair of
  measurement 012 had already taken a share; the store took the rest).
