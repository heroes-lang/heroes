# Panel 205, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 03:14 on 2026-10-10 (`date`), before any of the
sessions starts. Folders `<scratchpad>/readings-205/<label>`, outside the
repository and outside any git tree (the author's exception of 2026-10-09;
the budget the author's yes of 02:36 to 02:37, 5.43 USD). The task is the
spec-warden's P2 (`docs/panel/205-reports/spec-warden.md`): write a Linux
program binding glibc's `sched_getcpu` from `sched.h`, which glibc declares
only under `_GNU_SOURCE`. The brief names neither the macro nor a header of
the program's own. The labels' mapping, never in a folder:

- **m1-a to m1-d**: the trunk's spec at `46c975c4` with panels 203 and 204's
  ratified sentences (the spec-warden's `drafts/base1.md`: V3T, § 6's clause,
  F2 and G1m).
- **m2-a to m2-d**: the same with H2f in § 13: *C reads a module's headers in
  the order its groups are written: one that needs another's names comes
  after it, and one defining `_GNU_SOURCE` first.*

**What I expect** (the spec-warden's P2): in m2 at least 3 of 4 write a header
of their own defining `_GNU_SOURCE`, named by the module's first group; in m1
at most 1 of 4; and at least 2 of 4 in m1 name `_GNU_SOURCE` in their
`choice_points` (it is in `sched_getcpu`'s manual page) and put it somewhere
the compiler's prefix makes useless or bind `sched.h` plainly. **What it would
falsify**: m1 reaching 3 of 4, by the spec-warden's own rule H2f then stays
out.
