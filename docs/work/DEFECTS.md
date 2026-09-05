# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"I do not like the
defect directory … if anything is still open in defect at the end, make one
single file called DEFECTS.md inside work, so that everything is tidy"*.
Seven files under the old defect directory became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

**One defect is open as of 2026-09-05**, filed by panel 112 and put to the author with it, because both of its candidate repairs cost something a measurement cannot choose between. The paragraph below was written when the list was empty and is kept: it is the record of what closed before it.

**Nothing was open here earlier on 2026-09-05.** Defect 011 (two `@` arguments of one
place compiling and running) and defect 012 (`function f(_: ())` reaching clang
as `void h0_`, exit 2) were both repaired that night and are in
`docs/work/DONE.md`. This line is a date and not a boast: an empty list is worth
exactly as much as the last search that filled it, and the searches that found
these two were a record audit and a panel seat measuring something else.
