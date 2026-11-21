# Panel 252's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's folders under
`.claude/worktrees/rebel-scratch/`, the five seats' (`252-briefwriter/`, `252-compiler-engineer/`,
`252-critic/`, `252-llm-ergonomist/`, `252-spec-warden/`) and the coordinator's `coord/252-grade/`,
`coord/252-critic1-prompt.md`, `coord/252-critic2-prompt.md`, `coord/252-resolutions.md`,
`coord/252-synth-head.md` and `coord/252-synth-body.md`, as they stood on 2026-11-19 at 18:44 CET,
16 minutes after the newest of them was written (`coord/252-synth-body.md`, 18:27). Each kept file
is under 2 MiB: **2,052 files and 5,282,458 bytes**, plus the packer's own 34 files, 1,709,171
bytes (its four programs, its ten lists and reports, ten checks, the 9 diffs below and a
manifest); 2,086 members and 6,991,629 bytes in all, 529,000 bytes after `xz -9e` (`sha256`
`78f312ab55661160...`). Every path in it is relative to `rebel-scratch/`. By folder, kept files and
bytes: `252-compiler-engineer/` 1,426 and 3,066,675, `252-llm-ergonomist/` 184 and 635,186,
`252-briefwriter/` 179 and 961,838, `252-critic/` 154 and 263,908, `252-spec-warden/` 56 and
265,359, `coord/` 53 and 89,492.

Packed by `252-pack/pack.py`, panel 251's `251-pack/pack.py` with one rule more, which is in the
archive with the rest of `252-pack/`. Its rules are 251's: every regular text file under 2 MiB, no
`build/`, `tmp`, zig cache, Go home or `.git` folder, no binary, no symbolic link, one copy of each
content, every path left out written to `left-out.tsv` with its reason, a copy of the repository
kept as a diff, and a folder that is one of the frozen commit's evidence archives unpacked whole
named in `named.txt`, not packed. The rule more is the blind areas' `home/` (below), listed in
`252-pack/homes.txt`. `detect.py`, `diffs.py` and `scan.py` are 251's with their names changed.
`252-pack/checks/` holds what was run to check the packing: `closest.py` (each copy against the
commits around the frozen one, by blob hash), `detect-low.py` (251's `work/detect-low.py`,
unchanged), `members-index.py` and `members-search.py` (the evidence archives' members), `kinds.py`
(what was left out as binary or large), `rebuild-test.sh`, and each one's output. The manifest,
`252-pack/MANIFEST.tsv`, gives every other member's path, bytes and sha256.

**The blind area's `home/`, named, not packed.** `252-llm-ergonomist/home/` is the HOME the blind
sessions ran under: 65 files, 7,411,146 bytes, 52 of them the sessions' transcripts (`.jsonl`, one
in each of the 52 session folders under `.claude/projects/`, 6,967,655 bytes). The transcripts carry
the specification and the answers the reports already give. They stay on disk, outside the archive.

**Named, not packed: none.** No folder of the sitting is an evidence archive of the frozen commit
unpacked whole, so `252-pack/named.txt` holds its header alone. The check, run by command
(`checks/members-search.py`, its output `checks/members-search.txt`): every kept file and every
duplicate, 7,254 files, compared with the 152,895 members of the 61 archives the frozen commit
holds, as `git archive` wrote them (its 49 `evidence.tar.xz`, three other `*evidence.tar.xz`, two
more archives under `docs/panel/` and seven under `docs/measurements/`), and with the 1,349 members
of panel 251's own archive, which the frozen commit does not hold (it entered at `f4e3594d5`). 3,018
files equal some member's bytes, every one at another path: no file stands at a member's path below
any folder, so no folder is an archive unpacked, and no folder holds only files equal to members
(the rule as worded, which 251's note left to the coordinator, names nothing here). 2,614 of the
3,018 are the compiler engineer's census outputs (`out/census*`), and 2,933 are under 100 bytes, 293
of them empty.

**One copy of each content.** 5,202 more files, 7,205,640 bytes, are byte copies of a file the
archive keeps (4,824 of them in `252-compiler-engineer/`, 4,685 of these in its `out/census*`; 166
in `252-llm-ergonomist/`, 118 in `252-critic/`, 47 in `252-spec-warden/`, 29 in `coord/`, 18 in
`252-briefwriter/`). Each is named in `252-pack/DUPLICATES.tsv` beside the kept path that holds the
same bytes, so the folder can be rebuilt whole.

**Left out**, counted by `pack.py` (`252-pack/summary.json`; each path in `252-pack/left-out.tsv`):
- **the copies of the repository and of its folders**: 5 whole trees and 4 copies of `selfhost/`
  (`252-pack/copies.txt`), 104,348 files and 1,679,145,093 bytes. Each is kept instead as a unified
  diff against the frozen commit, `181555e9a32d0b18878f9f10e48d2eadd939c285`, or against the frozen
  commit's folder for a folder copy, in `252-pack/diffs/`: 9 diffs, 146,927 bytes, written by
  `252-pack/diffs.py` as 251's were (`diff -ruN` from `rebel-scratch/`, the commit extracted by
  `git archive` at `252-pack/work/frozen`; `build/`, `tmp`, `.git`, the zig and Go caches, objects
  and `.dSYM` folders left out of each; a binary file named by `diff`'s one line, of which there is
  none). `copies.txt` gives each diff's size and how many files it touches.
  - **Found by content, not by name** (`252-pack/detect.py`, its output `detect.txt`): the five
    trees, each holding `seed/heroes.c`, and the four `selfhost/` copies,
    `252-compiler-engineer/exp/q1a/selfhost`, `exp/q3/selfhost`, and the critic's
    `work2/exp-q1a/selfhost` and `work2/exp-q3/selfhost`. 251's lower threshold
    (`checks/detect-low.py`: 30 files and a tenth of a folder) found no other copy
    (`checks/detect-low.txt`).
  - **Two seats' folders are themselves whole trees**: `252-compiler-engineer/` and
    `252-spec-warden/`. The other three are `252-briefwriter/tree/`, `252-critic/tree/` and
    `252-critic/tree-all/`. Each of the five holds a `.git` file naming a `gitdir` under
    `/nonexistent-`, kept. A whole tree's copy is the 27 names the frozen commit holds at its top;
    every other entry at the tree's top is the seat's own work, packed file by file or left out by
    the rules above: the compiler engineer's `cases/`, `exp/`, `out/`, `routes/`, `tools/`, its
    compilers, lists and logs; the spec warden's `arms/`, `myarms/`, `probes/`, `suites/`,
    `sw-tools/`, its compilers, harness and logs; beside the brief writer's and the critic's trees'
    27 names stand only their `.git` file, compilers and `build/`.
  - **Every tree is the frozen commit's.** By blob hash (`checks/closest.txt`, over the 19,750 files
    the frozen commit holds outside folders named `build`), `252-critic/tree/` and
    `252-spec-warden/` hold every one identical, none differing, none extra, none missing; against
    the frozen commit's parent, `b40bad9f7`, one file differs (panel 251's record), and against its
    child, `f4e3594d5`, two are missing (251's `evidence.md` and `evidence.tar.xz`). Their diffs are
    empty. `252-briefwriter/tree/`'s diff, 1,784 bytes, holds 2 files new,
    `examples-rebel/bw_deque_long.hero` and `bw_deque_short.hero`. `252-compiler-engineer/`'s, 41,571
    bytes over 16 files, holds 7 examples new in `examples-rebel/` (`ce_*.hero`), and in `selfhost/`
    2 files new (`check/byte_text.hero`, `check/waiting_parts.hero`) and 7 changed
    (`check/interp.hero`, `check/settling.hero`, `check/waiting_order.hero`, `check/walk.hero`,
    `modules.hero`, `number.hero`, `use_key.hero`): the routes'. `252-critic/tree-all/`'s, 33,016
    bytes over 9 files, is the same 9 `selfhost/` files, and its `selfhost/` is byte for byte the
    compiler engineer's (`diff -r`, `build/` aside, empty). None of the five diffs touches a file
    the commits after the frozen one changed.
  - **The 4 `selfhost/` copies**: the compiler engineer's `exp/q1a` and `exp/q3`, diffs of 34,421
    and 34,663 bytes over 10 files each, the same 9 paths as its tree's diff and one more each,
    `ast.hero` and `check/table.hero`; the critic's `work2/exp-q1a` and `work2/exp-q3`, 606 and 866
    bytes, that one file alone, byte-identical to the compiler engineer's. None misses a file of the
    folder. By blob hash each is closest to the frozen commit's folder, tied with every commit from
    `44b26c2d1` (2026-11-15, the newest commit touching `selfhost/`) to `b7a43bf18` (the worktree's
    HEAD when packed): `selfhost/` did not change in that span, so these copies cannot be told apart
    from those commits, and none is shown to be of another one.
  - **What a diff cannot carry**: nothing. `252-pack/empty-in-copies.txt` holds its header alone:
    no empty file new in a copy, no empty file of the frozen commit absent from one, no empty folder;
- **259 binary files**, 73,458,933 bytes (a NUL byte, or an executable, archive or PDF header): 258
  Mach-O files and one Python bytecode file, `252-llm-ergonomist/tools/__pycache__/
  transcript_checks.cpython-315.pyc` (10,501 bytes); 139 of them in `252-compiler-engineer/out/`,
  44 in `coord/252-grade/` and 26 in `252-critic/work2/` (`checks/kinds.txt`);
- **18 files of 2 MiB or more**, 387,663,248 bytes, all Mach-O, none text. Seventeen are compilers,
  380,189,656 bytes, each holding the banner the compiler writes at the head of the C it emits:
  `heroes` and `heroes-next` at the top of `252-briefwriter/tree/` and `252-critic/tree/`, `heroes`
  and `cr-all` at the top of `252-critic/tree-all/`, `252-critic/tree/crit-nocycle`,
  `252-briefwriter/q1/heroes-nocycle`, the compiler engineer's `heroes`, `ce-all`, `ce-all2`,
  `ce-frozen`, `ce-q1r`, `ce-q2r` and `ce-q4r`, and the spec warden's `sw-heroes` and
  `sw-heroes-next`. One is the spec warden's `sw-harness`, 7,473,592 bytes;
- 100 `build/` folders, 5 `tmp` folders, and one `.git` folder, `252-llm-ergonomist/s/.git/` (18
  files, 27,305 bytes).

**A blind spot of every diff.** The frozen commit holds 287 files under folders named `build`, which
each diff leaves out on both sides, as 251's did; four of them are real sources,
`packages/rebel/0.1.0/files/sql/build/{checks,clauses,statements,tokens}.hero`, so a change a seat
made there would not show. Measured when packed, each tree's 287 are byte-identical to the frozen
commit's (the `selfhost/` copies hold none). Three trees also hold build output of their own in
`examples-rebel/build/` (90 files in `252-briefwriter/tree/`, 352 in `252-compiler-engineer/`, 284
in `252-critic/tree/`), left out with the copy.

**Read back and scanned.** `tar -tJf` lists 2,086 members, the 2,052 of `252-pack/list.txt` and the
packer's 34, in the order they were packed, with no folder entry, no `._` file and nothing under a
`home/`. 89 members, those whose path does not fit a `ustar` header, carry a pax header of their
path and time, and no member carries an extended attribute. Each member read back from the archive is
byte-identical to its source and to its line of the manifest. The folder was then rebuilt from the
archive and the frozen commit, into an empty folder, by the steps below
(`252-pack/checks/rebuild-test.sh`): the 2,052 kept files and the 5,202 duplicates came back
identical, each of the 9 copies identical to its source with no line of difference, and
`252-llm-ergonomist/home/` absent, 110,910 files rebuilt in all. Every file the archive carries but
the scan's report and the manifest, 2,084 files, was scanned (`252-pack/scan.py`, its report
`252-pack/scan.txt`: a path, a line, the pattern and the shape of what follows, never a value) for
an `ANTHROPIC_API_KEY` and what follows it, `sk-ant-`, `ghp_`, `github_pat_`, `gho_`, a private
key's `BEGIN` line and an `Authorization:` header, then a wider sweep for `x-api-key`, `Bearer`,
`AKIA` and `sk-` shapes. There were 12 matches and no credential: 11 in `252-pack/scan.py`, its own
patterns with nothing after them, and `252-llm-ergonomist/run.sh:91`, which passes the variable on
by reference. No file was removed.

No issue draft and no decision draft stood at any of the roots when they were packed (no
`defects/`, no `decision-*` file).

**How to rebuild the folder.** In an empty folder that stands for `rebel-scratch/`, with `F` a
folder outside it:

```sh
mkdir -p "$F" && git -C <a checkout> archive 181555e9a32d0b18878f9f10e48d2eadd939c285 | tar -x -C "$F"
tar -xJf evidence.tar.xz
TAB="$(printf '\t')"
# the duplicates
tail -n +2 252-pack/DUPLICATES.tsv | while IFS="$TAB" read -r p k; do mkdir -p "$(dirname "$p")" && cp -p "$k" "$p"; done
# the named folders, each an evidence archive of the frozen commit unpacked (none for this sitting)
grep -v '^#' 252-pack/named.txt | while IFS="$TAB" read -r d a; do mkdir -p "$d" && git -C <a checkout> show 181555e9a32d0b18878f9f10e48d2eadd939c285:"$a" | tar -xJ -C "$d"; done
# the copies: the frozen tree or folder, then its diff, its --- paths made the copy's own
grep -v '^#' 252-pack/copies.txt | while IFS="$TAB" read -r c base rest; do
  mkdir -p "$c"
  if [ "$base" = . ]; then cp -R "$F/." "$c/"; pre=252-pack/work/frozen/; else cp -R "$F/$base/." "$c/"; pre="252-pack/work/frozen/$base/"; fi
  sed "s|^--- $pre|--- $c/|" "252-pack/diffs/$(printf %s "$c" | tr / _).diff" | patch -p0 -E -s
  find "$c" -type d -empty -delete
done
# what a diff cannot carry (nothing for this sitting)
tail -n +2 252-pack/empty-in-copies.txt | while IFS="$TAB" read -r p why; do case "$p" in */) mkdir -p "$p" ;; *) case "$why" in new*) : > "$p" ;; *) rm -f "$p" ;; esac ;; esac; done
```

The `sed` is 251's: each diff's `---` line names the frozen commit where `diffs.py` read it,
`252-pack/work/frozen/`, and the `sed` makes it the copy's own path, as the rebuild test ran it
(Apple's `patch` 2.0-12u11). What the rebuild cannot give back is what the archive left out: the
blind area's `home/`, the binaries, the files of 2 MiB or more, and the folders left out by name.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
