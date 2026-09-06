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

**Nothing is open as of 2026-09-06.** Defect **015** was filed and repaired the
same day and is in `docs/work/DONE.md`: a `certain` fix that machine-applied into
a program which does not compile, found by a panel seat outside its own remit,
verified within the hour and repaired at the class rather than at the witness —
three of the ten foreign-word swaps were broken in call position, not one.

**This paragraph was rewritten twice in one day and that is the point.** It said
*nothing is open* until 015 was filed, then *one defect is open* until 015 was
repaired. Both edits happened in the commit that made the line false, which is
this file's own rule; the failure it exists to prevent is the entry moving out
while the sentence announcing it stays, and that is recorded two paragraphs down
as something this file has already done once.

**Defect 014 was filed and repaired the same day** and is in `docs/work/DONE.md`:
a `[T]` or a `{K: V}` crossed the FFI boundary inside a callback signature at
exit 0, and `crosses_the_boundary` now recurses.

**Nothing was open as of 2026-09-05, at M-c-callbacks' close.** This line said *one
defect is open* for part of that day — defect 013, filed by panel 112 and put to
the author with it, because both of its candidate repairs cost something a
measurement could not choose between. **It was repaired the same evening, by a
route neither priced option named**, and it is in `docs/work/DONE.md`: both of
the sitting's routes were refusals, and the third was three lines above them in
the same file, written by panel 104 for a problem of the same family. The
question moved from *how is it prevented* to *how does the program say what
happened*, and the witness is the faulting **PC** rather than the faulting
address, because a call through a null function pointer does not touch a bad
address, it goes to execute at one. **The sentence outlived the defect by a day**,
which is this file's own rule failing in the direction nobody watches: the rule
says a repaired defect moves out, and the entry did move — the preamble
announcing it did not. A list that describes an item it no longer holds reads
exactly like a list that lost one.

The paragraph below was written when the list was previously empty and is kept:
it is the record of what closed before that.

**Nothing was open here earlier on 2026-09-05.** Defect 011 (two `@` arguments of one
place compiling and running) and defect 012 (`function f(_: ())` reaching clang
as `void h0_`, exit 2) were both repaired that night and are in
`docs/work/DONE.md`. This line is a date and not a boast: an empty list is worth
exactly as much as the last search that filled it, and the searches that found
these two were a record audit and a panel seat measuring something else.

