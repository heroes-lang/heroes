# Panel 258, the evidence

`evidence.tar.xz` (254,648 bytes, 367 files, 10.8 MB unpacked), made 2026-11-24 by the sitting's coordinator from
the folders the reports cite, **every text file under 2 MiB** (`file` reading anything but binary, an empty file
counting as text), **every `build/` and every binary left out** (148 binary files across the folders below), and
**the copies of the repository left out**: of 10,378 text files staged, 10,011 were byte-identical to a file
tracked at `a23ea157a`, the commit the briefs froze (`git hash-object` of each staged file against `git ls-tree -r
a23ea157a`), and are not kept: 159 copies of `runtime/`, then 9,852, of which 9,744 headers, 24 `.hero` sources and
24 `.expected` files that the compiler-engineer's room runs had copied from `tests/golden/run/` into each of 24 case
folders, and 60 others. Read back byte for byte (`diff -r` of the staged folder against the archive unpacked: no
difference) and scanned for keys (`grep -rIl -E 'sk-ant-[A-Za-z0-9_-]{20,}|gh[opsu]_[A-Za-z0-9]{20,}|AKIA[0-9A-Z]{16}'`:
nothing). What each folder holds is read from its file names, not from the files' contents; the reports say what
each program showed.

| in the archive | from | what it holds |
|---|---|---|
| `evidence-258/compiler-engineer-ce/` | `.claude/worktrees/258-compiler-engineer/.ce/` | the seat's probes and outputs: `cost/` and `dcost/` (`walk.c`, `walk.hero`, the `cc1_*` compile-cost scripts), `d6/` (`d6a.hero`, `d6b.hero`, `d6.h`, `proto/`), `leak/`, `pexit/`, `proto/`, `shapes/`, `room/` (its room program, link responses and the 24 goldens' outputs; the copied headers, sources and expectations are not kept), the two stage builds' logs and four `report_*.md` fragments |
| `evidence-258/ffi-pragmatist-doors/` | `<scratchpad>/ffi258/doors/` | the C, C++ and Heroes programs of its door experiments (`door.hero`, `door_spawn.hero`, `cxdoor.hero`, `jldoor.hero`, `chk.c`, `doors.h`) |
| `evidence-258/ffi-pragmatist-png/` | `<scratchpad>/ffi258/png/` | its programs over real libpng (`pnga`, `pngb`, `pngc`, `pngmsg_lent`, `pngmsg_lease`, `pngread.h`) |
| `evidence-258/ffi-pragmatist-hazard/`, `-unwind/`, `-win/`, `-msdocs/` | `<scratchpad>/ffi258/hazard/`, `unwind/`, `win/`, `msdocs/` | `d7.c` and `sigfree.c` with their outputs and `rates_mac.txt`; the forced-unwind programs (`fu_*.c`, `fu_lib.cpp`, `mp.c`); `winjmp.c`, `winlib.c`, `bj.c` and the box runs (`boxrun.sh`, `boxrun.log`, `boxrun2.log`); two Microsoft pages (`longjmp.html`, `tryfinally.html`) |
| `evidence-258/ffi-pragmatist-misc/` | `<scratchpad>/ffi258/patch_count.py`, `rt-count/`, `rt-countA/` | the script that patched the counters in, and the two runtime copies as `diff -ru` of `runtime/` at `a23ea157a` against each (`rt-count.diff`, `rt-countA.diff`) |
| `evidence-258/coord-leak2704/` | `.claude/worktrees/rebel-scratch/leak2704/` | the coordinator's builds and timed runs of `main_1k.hero` and `main_100k.hero` for defect 2704's leak (the briefs hold the programs and `readings.md`, `258-briefs/leak/`) |
| `evidence-258/coord-f2f5/` | `.claude/worktrees/rebel-scratch/f2f5/` | the build logs and runs of the reproducers of defects 2752 to 2756, three runs where the program names them (`err_handlejump.1` to `.3`, `err_pexit.1` to `.3`, `t_lendjump.txt`, `t_lendok.txt`), `room_full.hero` and `room_one.hero` (the briefs hold the programs, `258-briefs/jumps/`) |
| `evidence-258/critic-exp/` | `<scratchpad>/exp/` | the completeness critic's outputs: `al.N.out` and `al.xN.out` (the signal-handler program), `room_order`, `room_one`, `handlejump`, `px` and `main_1k` builds and three runs each |

**Not in the archive**: the two seats' detached worktrees, `.claude/worktrees/258-compiler-engineer/` and
`258-ffi-pragmatist/`, copies of `a23ea157a` whose built compilers and `build/` directories are binaries;
`<scratchpad>/ffi258/ev257/`, a copy of panel 257's evidence the seat read, which is that sitting's own archive;
and the critic's worktree, which was no longer there when this was made, so nothing of it is kept but `exp/`:
`.claude/worktrees/258-critic` read absent on 2026-11-24, as the archive was made.
**The folders are the author's to remove** once the sitting is ratified: the two worktrees above, and
`ffi258/` and `exp/` in the session's scratchpad (a reboot empties them whatever is decided).

**What it does not carry**: a blind run. The soundness lane has no blind seat, and none was run.
