# Panel 251's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's folders under
`.claude/worktrees/rebel-scratch/`, the six seats' (`251-briefwriter/`, `251-compiler-engineer/`,
`251-critic/`, `251-ffi-pragmatist/`, `251-llm-ergonomist/`, `251-spec-warden/`) and the
coordinator's `coord/251-grade/`, `coord/251-critic2-prompt.md`, `coord/251-synth-head.md` and
`coord/251-synth-body.md`, as they stood on 2026-11-19 at 14:18 CET, 33 minutes after the newest of
them was written (`coord/251-synth-body.md`, 13:45). Each kept file is under 2 MiB: **1,284 files
and 8,940,187 bytes**, plus the packer's own 65 files, 1,758,385 bytes (its four programs, its nine
lists and reports, the 51 diffs below and a manifest); 1,349 members and 10,698,572 bytes in all,
793,112 bytes after `xz -9e` (`sha256` `9cdcdd653828f4a7...`). Every path in it is relative to
`rebel-scratch/`. By folder, kept files and bytes: `251-ffi-pragmatist/` 438 and 838,033,
`251-compiler-engineer/` 264 and 4,366,178, `251-critic/` 197 and 653,386, `251-llm-ergonomist/`
171 and 754,161, `251-briefwriter/` 159 and 2,040,131, `coord/` 32 and 75,862, `251-spec-warden/`
23 and 212,436.

Packed by `251-pack/pack.py`, panel 246's `synth/pack.py` adapted to several roots, which is in the
archive with the rest of `251-pack/`. Its rules are unchanged: every regular text file under 2 MiB,
no `build/`, `tmp`, zig cache or `.git` folder, no binary, no symbolic link, one copy of each
content. What differs is said in its header: the roots named on its command line instead of one
scratch root; its outputs in `251-pack/`, so `DUPLICATES.tsv` and the manifest stand there in the
archive rather than at its root; `copies.txt`'s two kinds of line (below); two more cache names,
`zigcache` and `gohome`; `left-out.tsv`, every path it left out with its reason; and a rule of its
own, below. The manifest, `251-pack/MANIFEST.tsv`, gives every other member's path, bytes and
sha256.

**Named, not packed.** `251-critic/work/ev250/` is panel 250's evidence unpacked, which the frozen
commit holds as `docs/panel/250-reports/evidence.tar.xz`. By the rule the coordinator added to the
packer on 2026-11-19, *a folder whose every file is byte-identical to a member of an evidence
archive the frozen commit holds is named, not packed*, it is named in `251-pack/named.txt` beside
that archive and left out: 1,632 files, 36,908,606 bytes. The check, run before it was left out:
each of its files compared with the member of the same path in the archive as
`git -C .claude/worktrees/rebel-round show 3a5ee2691:docs/panel/250-reports/evidence.tar.xz` gives
it (`sha256` `06d629442f1e4772...`, the prefix 250's note gives), **1,632 checked, 1,632
byte-identical**, none differing, none without a member, no member without a file, no symbolic
link. `pack.py` repeats the check at every run, against the archive as `git archive` wrote it into
`251-pack/work/frozen/` (the same sha256), and stops if it fails, as it did when the folder was
paired with 249's archive instead. It names a folder only where the folder is the archive unpacked
whole: every file the member at the same path, no member missing. Six small folders, 12 files,
hold only files byte-identical to members of an earlier sitting's archive and are kept: the three
`routes/c-word/selfhost/` (1 file each) and `251-compiler-engineer/q1/ship/deep/` (3) and
`q1/ship/r3/` (4), each file the member at the same path of 249's or 247's archive, and
`q1/ship/depth/` (2), whose files equal members at other paths. They are a route's or a probe's own
files that an earlier sitting archived first. The rule as worded would name them too; that choice
stays the coordinator's. The search that found them compared every kept file and every duplicate
with the members of all 49 evidence archives the frozen commit holds.

**One copy of each content.** 688 more files, 3,860,534 bytes, are byte copies of a file the
archive keeps (276 of them in `251-ffi-pragmatist/`, 165 in `251-llm-ergonomist/`, 107 in
`251-critic/`, 67 in `251-compiler-engineer/`, 45 in `coord/`, 16 in `251-spec-warden/`, 12 in
`251-briefwriter/`). Each is named in `251-pack/DUPLICATES.tsv` beside the kept path that holds the
same bytes, so the folder can be rebuilt whole.

**Left out**, counted by `pack.py` (`251-pack/summary.json`; each path in `251-pack/left-out.tsv`):
- **the copies of the repository and of its folders**: 5 whole trees, 41 copies of `runtime/`, 3
  of `selfhost/` and 2 of `packages/` (`251-pack/copies.txt`), 107,810 files and 1,827,642,459
  bytes. Each is kept instead as a unified diff against the frozen commit,
  `3a5ee269117a31780ef1f96d0305452630ac4d83` (step 52, the commit
  `251-briefwriter/tree-commit.txt` names), or against the frozen commit's folder for a folder copy,
  in `251-pack/diffs/`: 51 diffs, 1,313,810 bytes, written by `251-pack/diffs.py` as 250's were
  (`diff -ruN` from `rebel-scratch/`, the commit extracted by `git archive` at
  `251-pack/work/frozen`; `build/`, `tmp`, `.git`, the zig and Go caches, objects and `.dSYM`
  folders left out of each; a binary file named by `diff`'s one line). `copies.txt` gives each
  diff's size and how many files it touches.
  - **Found by content, not by name** (`251-pack/detect.py`, which aligns every file with the
    frozen tree's files on their common path, its output `detect.txt`): 7 of the 41 `runtime/`
    copies carry that name, the brief writer's `routes/check/*/runtime`; the others are the
    compiler engineer's 13 `rt-*` folders, the FFI pragmatist's 15 under `rt/` and `q2/linux/rt/`,
    and the critic's 6 under `work2/q1/rt/`, `work2/q2/rt/` and `work2/q3/rt-Q3/`. The route folders
    holding only a route's own 3 or 4 files (`251-briefwriter/routes/<route>/`,
    `251-compiler-engineer/routes/` and `q1/ship/routes/`) are not copies and are kept file by
    file, as 250 kept its partial trees.
  - **Three seats' folders are themselves whole trees**: `251-compiler-engineer/`,
    `251-ffi-pragmatist/` and `251-spec-warden/`, each holding `seed/heroes.c` and a `.git` file
    naming a `gitdir` that does not exist (kept, as `251-critic/tree/.git` is). A whole tree's copy
    is the 27 names the frozen commit holds at its top; every other entry at the tree's top (`q1/`
    to `q3/`, `routes/`, `rt-*`, `logs/`, `probes/`, `repro/`, the seat's compilers) is the seat's
    own work, packed file by file. The other two trees are `251-briefwriter/tree/` and
    `251-critic/tree/`.
  - **Every tree is the frozen commit's.** Four of the five diffs hold only files new under
    `examples-rebel/` and change no file the commit holds: the brief writer's 2 (1,743 bytes), the
    compiler engineer's same 2 (1,747), the critic's 16, 4 of them binary (8,414), the FFI
    pragmatist's 11, 6 binary (3,856). `251-spec-warden/`'s diff is empty. Their `packages/` tell
    the frozen commit from its neighbours: step 52 changed 11 of its files and step 53 changed 27,
    and no tree's diff touches it.
  - **The 41 `runtime/` copies**: 2 identical to the frozen folder, empty diffs
    (`251-briefwriter/routes/check/base/runtime`, `251-ffi-pragmatist/rt/base`), and 39 with diffs
    of 697 to 29,843 bytes touching 1 to 6 files each, the routes' changes, most often
    `runtime.c` (in 22), `parts/sigguard.c` (21), `parts/alloc.c` and `parts/spawn.c` (15 each)
    and `parts/spawn_str.c` (8), and the new `parts/rebind.c` (11), `parts/siginstall.c` (9),
    `parts/sigjump.c` (3), `parts/mailbox.c` and `parts/spawn_value.c` (2 each). The 3 `selfhost/`
    copies' diffs are 804, 814 and 2,541 bytes, over `ast.hero`, `check/table.hero` and
    `parse/type.hero`. None misses a file of its folder. By blob hash against the twelve newest
    commits touching `runtime/` or `selfhost/`, each is closest to the frozen commit's folder, tied
    with the commits whose folder holds the same bytes: `runtime/`, `selfhost/` and `seed/` did not
    change from step 41 (`ec4252e22`) to step 53, so these copies cannot be told apart from those
    commits, and none is shown to be of another one.
  - **One copy is of a different commit.** `251-critic/work2/c53/5c5e6f811/packages` is the
    `packages/` of `5c5e6f811` (step 53, the commit after the frozen one) without its `core/`: 1,639
    files identical to that commit's by blob hash, none differing, none extra, the 41 files of
    `core/` absent. Its diff against the frozen folder, 452,097 bytes over 68 files, carries step
    53's 27 changes and `core/`'s 41 files removed. Its pair,
    `251-critic/work2/c53/3a5ee2691/packages`, is the frozen commit's `packages/` without `core/`
    (1,638 files identical, none differing, none extra), and its diff, 340,495 bytes, is those 41
    files removed.
  - **What a diff cannot carry** is listed in `251-pack/empty-in-copies.txt`: 4 empty files new in
    `251-critic/tree/examples-rebel/` (`diff -N` reads an absent file as empty) and one empty
    folder, `251-briefwriter/tree/examples-rebel/bw251/`; no empty file of the frozen commit is
    absent from a copy;
- **608 binary files**, 160,058,104 bytes (a NUL byte, or an executable, archive or PDF header):
  570 Mach-O files, 16 ELF, 2 Windows executables with their 2 `.pdb` files
  (`251-ffi-pragmatist/q3/mailbox/`), and 18 `ar` archives; 263 of them in
  `251-ffi-pragmatist/q1/` and 111 in `251-critic/work2/`;
- **28 files of 2 MiB or more**, 427,120,869 bytes. Fourteen are compilers, 299,408,704 bytes:
  `heroes` and `heroes-next` at the top of `251-briefwriter/tree/`, `251-critic/tree/`,
  `251-compiler-engineer/` and `251-ffi-pragmatist/`, the compiler engineer's `ce-heroes`,
  `ce-heroes-next`, `q2/census/bin/ce-census-heroes` and `ce-x-heroes`, and the spec warden's
  `sw-heroes` and `sw-heroes-next`. Twelve are the FFI pragmatist's ELF programs
  `q2/linux/fp-*`, 40,086,256 bytes, and one its archive `q1/census/go/libfpgo.a`. One is text:
  `251-ffi-pragmatist/logs/cache-imports.txt`, 83,534,453 bytes and 1,345,871 lines, a listing of
  the system libraries' imports opening `/usr/lib/libobjc.A.dylib [arm64e]:` (beside it, kept,
  `logs/cache-family-counts.txt` and `logs/cache-family-importers.txt`);
- 62 `build/` folders and 5 `tmp` folders, and two caches left out by name, both the FFI
  pragmatist's: `zigcache/`, zig's cache, 2,882 files and 94,476,515 bytes, as 246's `zcache` and
  `zlocal`; and `gohome/`, a Go home, 414 files and 31,194,558 bytes, 407 of them in its `cache/`
  and 7 under `Library/`.

**A blind spot of every diff.** The frozen commit holds 287 files under folders named `build`, which
each diff leaves out on both sides, as 250's did; four of them are real sources,
`packages/rebel/0.1.0/files/sql/build/{checks,clauses,statements,tokens}.hero`, so a change a seat
made there would not show. Measured when packed, every copy's 287 (4 in each `packages/` copy) are
byte-identical to the frozen commit's.

**Read back and scanned.** `tar -tJf` lists 1,349 members, the 1,284 of `251-pack/list.txt` and the
packer's 65, in the order they were packed, with no folder entry, no `._` file and nothing under
`ev250/`. Each member read back from the archive is byte-identical to its source and to its line of
the manifest. The folder was then rebuilt from the archive and the frozen commit, into an empty
folder, by the steps below: the 1,284 kept files and the 688 duplicates came back identical,
`ev250/` came back from the committed archive with its 1,632 files and no difference, and each of
the 51 copies identical to its source but for the 10 binaries the tree diffs name. Every file the
archive carries was scanned (`251-pack/scan.py`, its report `251-pack/scan.txt`: a path, a line, the
pattern and the shape of what follows, never a value) for an `ANTHROPIC_API_KEY` and what follows
it, `sk-ant-`, `ghp_`, `github_pat_`, `gho_`, a private key's `BEGIN` line and an `Authorization:`
header, then a wider sweep for `x-api-key`, `Bearer`, `AKIA` and `sk-` shapes. There was one match,
and it is not a credential: `251-llm-ergonomist/run.sh:56`, which passes the variable on by
reference. `251-llm-ergonomist/key.txt` is the blind arms' key, their labels and the sha256 of each
session's `brief.md`, `task.md` and `heroes-spec.md`, and holds none. No file was removed.

No issue draft and no decision draft stood at any of the roots when they were packed (no
`defects/`, no `decision-*` file).

**How to rebuild the folder.** In an empty folder that stands for `rebel-scratch/`, with `F` a
folder outside it:

```sh
mkdir -p "$F" && git -C <a checkout> archive 3a5ee269117a31780ef1f96d0305452630ac4d83 | tar -x -C "$F"
tar -xJf evidence.tar.xz
TAB="$(printf '\t')"
# the duplicates
tail -n +2 251-pack/DUPLICATES.tsv | while IFS="$TAB" read -r p k; do mkdir -p "$(dirname "$p")" && cp -p "$k" "$p"; done
# the named folders, each an evidence archive of the frozen commit unpacked
grep -v '^#' 251-pack/named.txt | while IFS="$TAB" read -r d a; do mkdir -p "$d" && git -C <a checkout> show 3a5ee269117a31780ef1f96d0305452630ac4d83:"$a" | tar -xJ -C "$d"; done
# the copies: the frozen tree or folder, then its diff, its --- paths made the copy's own
grep -v '^#' 251-pack/copies.txt | while IFS="$TAB" read -r c base rest; do
  mkdir -p "$c"
  if [ "$base" = . ]; then cp -R "$F/." "$c/"; pre=251-pack/work/frozen/; else cp -R "$F/$base/." "$c/"; pre="251-pack/work/frozen/$base/"; fi
  sed "s|^--- $pre|--- $c/|" "251-pack/diffs/$(printf %s "$c" | tr / _).diff" | patch -p0 -E -s
  find "$c" -type d -empty -delete
done
# what a diff cannot carry
tail -n +2 251-pack/empty-in-copies.txt | while IFS="$TAB" read -r p why; do case "$p" in */) mkdir -p "$p" ;; *) case "$why" in new*) : > "$p" ;; *) rm -f "$p" ;; esac ;; esac; done
```

The `sed` is needed. Each diff's `---` line names the frozen commit where `diffs.py` read it,
`251-pack/work/frozen/`, and `patch` (Apple's 2.0-12u11, as run) given both names created three
files new in `251-critic/work2/c53/5c5e6f811/packages` at that shorter path instead. What the
rebuild cannot give back is what the archive left out: the binaries (the 10 the tree diffs name
among them), the files of 2 MiB or more, and the folders left out by name.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
