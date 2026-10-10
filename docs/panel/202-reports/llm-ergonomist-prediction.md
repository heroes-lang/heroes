# Panel 202, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 23:30 on 2026-10-09 (`date`), before any of the
sessions starts. Folders `<scratchpad>/readings-202/<label>`, outside the
repository and outside any git tree (the author's exception of 2026-10-09;
the budget the author's yes of about 22:47, 6 USD shared with panel 203). The
spec in every folder is the trunk's at `65b78f2e` (N1f in § 9). The labels'
mapping, never in a folder:

- **d1-a, d1-b**: predict `check`, `build` and run, and `test` for the
  two-header program `fp` (`a.h` `static inline long twice(long)`, `b.h`
  `static inline int twice(int)`, one module each). Today: check 0, build 0
  printing 6 and 8, test 1 (defect 538's message).
- **d2-a, d2-b**: the same for `clash` (`a.h` declares `long twice(long)`,
  `b.h` `double twice(double)`, `c.h` defines `long twice`). Today: check 0,
  build 0 printing `6`, `4.0`, `10` (the wrong value), test 1.
- **d3-a, d3-b**: repair `fp` given `heroes test`'s output (defect 538's
  message, its note ending *bind a C name they share from one header, or name
  for one group a header of your own*), keeping what the program prints.
- **d4-a, d4-b**: repair `onedef` (one header `c.h` defining a non-static
  `int twice(int)`, named by two modules) given `build`'s exit 2 (*internal
  error: linking failed: duplicate symbol*).

**What I expect.**

1. d1: both predict check 0 and build 0 printing 6 and 8; at least one
   predicts `test` exits 0 too (nothing in the spec says the tools differ on
   units).
2. d2: neither reader predicts the wrong value; both predict `8.0` for the
   middle line, or a refusal.
3. d3: both follow the note by giving one group a header of its own (a small
   wrapper) or merging the binding; at least one edits `b.h` itself.
4. d4: both make the definition `static inline` or move it to a `.c` file and
   a `link`; at least one reads the exit 2 as the compiler's fault in its
   `choice_points`.
5. Every `context` names only the folder and the harness's environment.

**What it would falsify**: a d2 reader predicting `4.0`; a d1 reader
predicting `test` refuses.
