# Panel 170 — brief for the ffi-pragmatist

Read `docs/panel/170-briefs/00-shared.md` first.

## Your axis, and your record in this milestone

design.md §1.11, and the veto on ABI breakage. You supplied panel 167's ground
for a trailing header and **withdrew it at panel 168 on your own measurement**;
you registered R5 at panel 169 and it is now measured to work with no new form
at all. This sitting rests on your reframe, so it is yours to test again.

## The part only you do

**Write the bindings the mark implies, against real headers.**

1. **`sqlite3_bind_text` / `sqlite3_bind_blob`.** The retention is decided by
   the FIFTH argument at run time — `SQLITE_STATIC` keeps, `SQLITE_TRANSIENT`
   copies, a real destructor disposes. `examples/ledger/db/sqlite.hero:20-23`
   says in a comment that the program passes `SQLITE_TRANSIENT` *so there is no
   lifetime to reason about*, which is prose doing a compile error's job in the
   project whose thesis is that it never should. **Can a mark on the parameter be
   honest about an API whose retention is an argument?** Panel 167 measured that
   a run-time `bool` picks it. Say what a mark can truthfully claim there.
2. **Count the shape in real headers.** How many parameters across `sqlite3.h`,
   `raylib.h` and `curl.h` retain unconditionally, how many decide by an
   argument, and how many take an explicit disposer. Name your ruler.
3. **Does the mark make a binding you would write easier or harder?** Compare
   against R5, which needs no mark: the author allocates and names the disposer.
4. **Defect 072.** Two allocator families on one `tag void`. Is that a real
   binding shape or an artificial one? Name a library pair that forces it.
5. **Does anything here break ABI?**
6. **Register a falsifiable prediction** with an instrument that exists today.

Build in a copy; never rebuild from `selfhost/`. Report to
`docs/panel/170-reports/ffi-pragmatist.md`. Veto on ABI breakage.
