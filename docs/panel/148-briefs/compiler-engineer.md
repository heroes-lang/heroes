# Panel 148 — compiler-engineer

Read `00-shared.md` first. You judge the ceiling (design.md §1.1, §1.7, Part 5):
implementation cost, and core-versus-sugar. **Veto power.**

## What only you are asked

1. **C's hidden cost, which A does not have.** Option C reads the obligation off
   the group's own `consumes` function — so the compiler must, at every call
   handing back a handle of type `T`, know whether the GROUP declares a consumer
   for `T`. Is that a lookup the emitter already has, or a new table? Read
   `selfhost/check/consuming.hero`'s `consuming_positions`, which reads ONE
   declaration, and say what asking *about a type across a whole group* costs on
   top. **This is the hinge and nobody else can answer it.**
2. **C's correctness, not its cost.** Under C the obligation is inferred from
   the presence of a consumer. `examples/sqlite/main.hero` declares
   `sqlite3_finalize` but its `Stmt` also comes back from nothing else — fine.
   Now find, or fail to find, a shipped or plausible binding where a group
   declares a consumer for `T` and ALSO has a function handing back a **borrowed**
   `T`. Under C that borrow silently acquires an obligation nobody owes.
   `sqlite3_db_handle` and `sqlite3_next_stmt` are the shapes; neither is bound
   in this tree today.
3. **The counter.** Both options need the same one: an increment at the
   acquiring call, a decrement at the `consumes` call, a fourth exit check in
   `runtime/parts/alloc.c` beside the three there. Read `hero_live_held` and the
   `lease` pair, and say whether the emitter can place both ends **without flow
   analysis** — `check/leasing.hero:29` and `check/consuming.hero:22` both state
   it has none.
4. **§1.7's subtraction** for each option, and core or sugar.

## Prototype discipline

In a copy, `rm -rf target build` after. Prototype the one whose cost you are
least sure of, report the number, throw it away.
