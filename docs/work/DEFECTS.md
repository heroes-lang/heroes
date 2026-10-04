# DEFECTS — the compiler defects that are still open

Every defect is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. A repair is owed at the
class and not at the witness, with a `tests/golden/fixedbugs/` case per shape.

**Each open defect is a file of its own in `docs/work/defects/`** since
2026-10-04, by the author's *5a*: two lanes write two files and never one line.
This page is the list's rules and holds **no item, no banner and no count**; the
open count is the number of files there, which `records/counts` holds the
ROADMAP's line to, and the directory's `.gitkeep` keeps it in every clone on the
day it is empty, which is the goal.

**A defect file** is `NNN-<slug>.md`, `NNN` its number in three digits or more,
padded to three and never past (the thousandth is `1000`; amended 2026-10-04,
defect 280), and `<slug>` its title's words lowercased, every run of other
characters one `-`, cut at a word boundary to at most 60 characters. It holds
one item in the shape of `.claude/rules/records.md` § The lists, its line first
and its body indented under it; `records/lists` and `records/defects` are the
executors.

**Filing** one is a new file with the number the coordinator issues, one above
the highest in `docs/work/defects/` and `docs/records/done/`, read and never
remembered; who issued which number since 2026-09-08, and why 014 exists twice,
is `docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.
**Closing** one is a `git mv` into `docs/records/done/` as
`YYYY-MM-DD-HHMM-defect-NNN-<slug>.md`, its line ticked and its *The repair*
section added with the measurements that prove it, so its history follows it.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look> · **class: <name>**`
