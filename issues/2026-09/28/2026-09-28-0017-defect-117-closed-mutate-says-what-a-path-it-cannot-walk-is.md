---
kind: defect
area: examples
milestone: none
filed: 2026-09-27
commit: dfc1e221bf2103d247dc82278e9e9e14ff852a41
github: none
---

# Defect 117 closed: `heroes mutate` says what a path it cannot walk is

2026-09-27, M-agreed-retention step 24, in lane 117 (`97b4bb24`), merged
`c90fd658`. Found by lane A's sweep of every verb on degenerate files;
repaired by the coordinator.

- [x] **117 — `heroes mutate <file>` says it cannot read a file it reads** | `heroes mutate examples/adventure/main.hero` answers `error: cannot read examples/adventure/main.hero` at exit 2: `mutate` takes a directory of programs (`no_corpus`), and the message names the wrong cause | `selfhost/cli/mutate.hero:86,99` · **closed 2026-09-27**

    **Origin:** lane A's sweep of every verb on degenerate files, 2026-09-27,
    where `mutate` on each of the 15 files read the same before and after the
    repair of defect 109; re-run by the coordinator on a real program on the
    trunk at `f08b192d`.

    **Why it is a defect.** A diagnostic carries what is needed to fix the
    program without opening another file (design.md §4.17), and this one sends
    the reader to look for a permission or a missing file that is not there.

## The repair

`mutate.unread_corpus` names what the path is when the corpus walk fails: a
file, with what to give instead, the directory that holds it; a path that is
not there; or a directory the walk could not read. The operand keeps its
meaning and a file stays refused at exit 2, the command's surface; only the
sentence moves (design.md §4.17):

    error: examples/adventure/main.hero is a file, and `heroes mutate` reads a directory of programs: name the directory that holds it
    error: no file or directory is named examples/no-such-program

## The pins

A compiler test over a missing path, a file of the repository and a
directory the walk reads; two `surface` rows, a file and a missing path,
each at exit 2 with its sentence.

## The gate

In the lane: seed regenerated, fixpoint by `cmp` (`5ec8b857aad0cd87`); the
compiler's 783 tests, the net's own 172, `surface` 305. On the trunk after
the merge: the surface suite's count joined with lane B's as 133; the seed
emitted again, fixpoint (`63286fa6e637b373`); the compiler's 787 tests, the
net's own 172; the full net, with every lane merged and the split below, **2754 passed and 0 failed** (`d5d78666`, 1271.47 s real, 746.20 user). Lane C's repair had taken three files past design.md §11's ~300 lines, and the combined trunk's `layout` refused it on Linux arm64 and the Windows box (the coordinator had not run `layout` on the lane): split along the repair's own seams in `97247b0f`, `name_rows.hero`, `check/freer_marks.hero` and `check/holes_gather.hero`, the three at 290, 280 and 234 code lines; and `order` asked an ORDER mark on `resolved.modules_declaring` and the walks floor moved 22 to 21, the repair having replaced two walks by one. Linux arm64 on that trunk: the compiler's 787 tests and surface 315, canonical 2, annotations 199, check 153, fixes 26, layout 4, order 3, lines 207, run 206, grammar 9, spec 20, each 0 failed; the Windows box: the compiler's 787 tests and surface 308, canonical 2, annotations 199,
check 153, fixes 26, layout 4, order 3, lines 205, run 204, grammar 9 and spec
20, each 0 failed.
