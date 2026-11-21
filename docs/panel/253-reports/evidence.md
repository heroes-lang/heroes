# Panel 253's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's folders under
`.claude/worktrees/rebel-scratch/`, the six seats' (`253-briefwriter/`, `253-compiler-engineer/`,
`253-critic/`, `253-ffi-pragmatist/`, `253-llm-ergonomist/`, `253-spec-warden/`) and the
coordinator's `coord/253-grade/`, `coord/253-critic1-prompt.md`, `coord/253-critic2-prompt.md`,
`coord/253-resolutions.md` and `coord/253-synth/`, as they stood on 2026-11-19 at 18:48 CET, 26
minutes after the newest of them was written (`coord/253-synth/refused_check.py`, 18:22). Each kept
file is under 2 MiB: **960 files and 9,148,381 bytes**, plus the packer's own 34 files, 393,155
bytes (its four programs, its ten lists and reports, twelve checks, the 7 diffs below and a
manifest); 994 members and 9,541,536 bytes in all, 1,420,732 bytes after `xz -9e` (`sha256`
`817d77cc2b267a40...`). Every path in it is relative to `rebel-scratch/`. By folder, kept files and
bytes: `253-compiler-engineer/` 514 and 5,910,165, `253-briefwriter/` 76 and 1,651,742,
`253-ffi-pragmatist/` 147 and 551,956, `253-llm-ergonomist/` 108 and 498,730, `253-spec-warden/` 17
and 273,744, `coord/` 48 and 188,929, `253-critic/` 50 and 73,115.

Packed by `253-pack/pack.py`, panel 251's `251-pack/pack.py` with one rule more, which is in the
archive with the rest of `253-pack/`. Its rules are 251's: every regular text file under 2 MiB, no
`build/`, `tmp`, zig cache, Go home or `.git` folder, no binary, no symbolic link, one copy of each
content, every path left out written to `left-out.tsv` with its reason, a copy of the repository
kept as a diff, and a folder that is one of the frozen commit's evidence archives unpacked whole
named in `named.txt`, not packed. The rule more is the blind areas' `home/` (below), listed in
`253-pack/homes.txt`. `detect.py`, `diffs.py` and `scan.py` are 251's with their names changed.
`253-pack/checks/` holds what was run to check the packing: `closest.py` (each copy against the
commits around the frozen one, by blob hash), `detect-low.py` (251's `work/detect-low.py`,
unchanged), `members-index.py` and `members-search.py` (the evidence archives' members), `kinds.py`
(what was left out as binary or large), `scan-values.py` (the scan's matches against the public
tree, below), `rebuild-test.sh`, and each one's output. The manifest, `253-pack/MANIFEST.tsv`, gives
every other member's path, bytes and sha256.

**The blind areas' `home/`, named, not packed.** `253-llm-ergonomist/home/` is the HOME the blind
sessions ran under: 45 files, 5,020,574 bytes, 32 of them the sessions' transcripts (`.jsonl`, one
in each of the 32 session folders under `.claude/projects/`, 4,576,945 bytes). The transcripts carry
the specification and the answers the reports already give. They stay on disk, outside the archive.
Three more folders named `home/` stand in the critic's folder and are left out and named the same
way: `253-critic/home/` and `253-critic/work/area/home/`, both empty, and
`253-critic/work/scan251/home/`, which holds no file and 51 symbolic links, each to a folder of
`251-llm-ergonomist/transcripts/`, panel 251's transcripts.

**Named, not packed: none.** No folder of the sitting is an evidence archive of the frozen commit
unpacked whole, so `253-pack/named.txt` holds its header alone. The check, run by command
(`checks/members-search.py`, its output `checks/members-search.txt`): every kept file and every
duplicate, 1,427 files, compared with the 152,895 members of the 61 archives the frozen commit
holds, as `git archive` wrote them (its 49 `evidence.tar.xz`, three other `*evidence.tar.xz`, two
more archives under `docs/panel/` and seven under `docs/measurements/`), and with the 1,349 members
of panel 251's own archive, which the frozen commit does not hold (it entered at `f4e3594d5`). 194
files equal some member's bytes, every one at another path: no file stands at a member's path below
any folder, so no folder is an archive unpacked, and no folder holds only files equal to members
(the rule as worded, which 251's note left to the coordinator, names nothing here). 86 of the 194
are under 100 bytes, 76 of them empty.

**One copy of each content.** 467 more files, 7,378,128 bytes, are byte copies of a file the archive
keeps (163 of them in `253-compiler-engineer/`, 110 in `253-llm-ergonomist/`, 106 in `253-critic/`,
51 in `253-ffi-pragmatist/`, 34 in `coord/`, 2 in `253-briefwriter/`, 1 in `253-spec-warden/`).
Each is named in `253-pack/DUPLICATES.tsv` beside the kept path that holds the same bytes, so the
folder can be rebuilt whole.

**Left out**, counted by `pack.py` (`253-pack/summary.json`; each path in `253-pack/left-out.tsv`):
- **the copies of the repository and of its folders**: 5 whole trees, 1 copy of `runtime/` and 1 of
  `packages/rebel/0.1.0/files/` (`253-pack/copies.txt`), 104,333 files and 1,814,527,591 bytes.
  Each is kept instead as a unified diff against the frozen commit,
  `181555e9a32d0b18878f9f10e48d2eadd939c285`, or against the frozen commit's folder for a folder
  copy, in `253-pack/diffs/`: 7 diffs, 84,563 bytes, written by `253-pack/diffs.py` as 251's were
  (`diff -ruN` from `rebel-scratch/`, the commit extracted by `git archive` at
  `253-pack/work/frozen`; `build/`, `tmp`, `.git`, the zig and Go caches, objects and `.dSYM`
  folders left out of each; a binary file named by `diff`'s one line, of which there is none).
  `copies.txt` gives each diff's size and how many files it touches.
  - **Found by content, not by name** (`253-pack/detect.py`, its output `detect.txt`): the five
    trees, each holding `seed/heroes.c`; `253-compiler-engineer/q5/x86/runtime`, all 79 files of
    `runtime/`; and `253-ffi-pragmatist/q1/rebel-mut`, 1,635 of the 1,639 files of
    `packages/rebel/0.1.0/files/`, the 4 others being under `sql/build/`, which the detector does
    not walk and which the copy holds (below). 251's lower threshold (`checks/detect-low.py`: 30
    files and a tenth of a folder) found no other copy (`checks/detect-low.txt`).
  - **Three seats' folders are themselves whole trees**: `253-compiler-engineer/`,
    `253-ffi-pragmatist/` and `253-spec-warden/`. The other two are `253-briefwriter/tree/` and
    `253-critic/tree/`. Each of the five holds a `.git` file naming a `gitdir` under
    `/nonexistent-`, kept. A whole tree's copy is the 27 names the frozen commit holds at its top;
    every other entry at the tree's top is the seat's own work, packed file by file or left out by
    the rules above: the compiler engineer's `q2/`, `q4/`, `q5/`, `q6/` and its compilers; the FFI
    pragmatist's `lx/`, `q1/`, `q3/`, `q5mac/` and its compilers; the spec warden's `sw-drafts/`,
    `sw-tools/`, its compilers, four lists and a log; beside the brief writer's and the critic's
    trees' 27 names stand only their `.git` file, compilers and `build/`.
  - **Every tree is the frozen commit's.** By blob hash (`checks/closest.txt`, over the 19,750 files
    the frozen commit holds outside folders named `build`), `253-briefwriter/tree/`,
    `253-critic/tree/`, `253-ffi-pragmatist/` and `253-spec-warden/` hold every one identical, none
    differing, none extra, none missing; against the frozen commit's parent, `b40bad9f7`, one file
    differs (panel 251's record), and against its child, `f4e3594d5`, two are missing (251's
    `evidence.md` and `evidence.tar.xz`). Their diffs are empty. `253-compiler-engineer/`'s diff,
    74,151 bytes over 34 files, is the routes': 7 files new
    (`packages/rebel/0.1.0/files/zz_every_face.hero`, and in `selfhost/` `check/test_only.hero`,
    `cli/doc.hero`, `cli/doc_shape.hero`, `cli/doc_text.hero`, `fetch/add_package.hero`,
    `module/unlisted.hero`) and 27 changed, 2 in `packages/rebel/0.1.0/files/` (`acme.hero`,
    `mongodb.hero`), 6 in `runtime/` (`hero_os.h`, `heroes_guard_close.h`, `heroes_guard_open.h`,
    `parts/os.c`, `parts/panic.c`, `parts/stack.c`) and 19 in `selfhost/`. None of the five diffs
    touches a file the commits after the frozen one changed.
  - **The `runtime/` copy**, `253-compiler-engineer/q5/x86/runtime`: a diff of 9,245 bytes over the
    same 6 files, each byte-identical to the compiler engineer's tree's. None misses a file of the
    folder. By blob hash it is closest to the frozen commit's folder, tied with every commit from
    `ec4252e22` (step 41, the newest commit touching `runtime/`) to `b7a43bf18` (the worktree's HEAD
    when packed), 6 files differing from each: `runtime/` did not change in that span, so the copy
    cannot be told apart from those commits, and it is not shown to be of another one.
  - **The `packages/rebel/0.1.0/files/` copy**, `253-ffi-pragmatist/q1/rebel-mut`: a diff of 1,167
    bytes, one line changed in `sql/sqlite/hero_sqlite3.hero`, a probe's change. By blob hash 1,634
    of its 1,635 files compared are identical to the frozen commit's, none extra, none missing, tied
    with `5c5e6f811` (step 53), `b40bad9f7` and `f4e3594d5`, in which that folder holds the same
    bytes; `3a5ee2691` (step 52) differs in 23 files and `08ae50aca` (step 56b, the first commit
    after the frozen one to touch the folder) in 4. Its 4 files under `sql/build/` are
    byte-identical to the frozen commit's.
  - **What a diff cannot carry**: nothing. `253-pack/empty-in-copies.txt` holds its header alone:
    no empty file new in a copy, no empty file of the frozen commit absent from one, no empty folder;
- **116 binary files**, 33,478,872 bytes (a NUL byte, or an executable, archive or PDF header): 71
  Mach-O files (35 of them in `253-ffi-pragmatist/q3/`), 38 ELF files (32 in
  `253-ffi-pragmatist/lx/`) and 7 `ar` archives (`checks/kinds.txt`);
- **37 files of 2 MiB or more**, 487,356,917 bytes. Seventeen are compilers, 380,673,624 bytes, each
  holding the banner the compiler writes at the head of the C it emits: `heroes` and `heroes-next`
  at the top of `253-briefwriter/tree/`, `253-critic/tree/`, `253-compiler-engineer/` and
  `253-ffi-pragmatist/`, the compiler engineer's `ce-base`, `ce-doc`, `ce-q26`, `ce-q4`, `ce-q6` and
  `ce-skip`, the critic's `r2/doc/ce-q26`, and the spec warden's `sw-heroes` and `sw-heroes-next`.
  Five are other Mach-O programs, 23,601,112 bytes: `253-briefwriter/ref/t6/table2`,
  `ref/t6b/page` and `ref/t6b/page_time`, and the FFI pragmatist's `q1/good/good` and
  `q3/conservative/cons`. Eight are the FFI pragmatist's ELF programs
  `lx/{chacha,chacha2,hashes,taint}/prog-msan-O0` and `-O2`, 34,240,856 bytes. Seven are text,
  48,841,325 bytes, all the FFI pragmatist's: three MemorySanitizer reports, each opening on a
  `use-of-uninitialized-value` warning, `lx/chacha/run-O0.err` (3,321,451 bytes, 41,704 lines),
  `lx/chacha/run-O2.err` (3,065,487 bytes, 38,500 lines) and `lx/hashes/run-O2.err` (30,721,715
  bytes, 275,003 lines); three C files the compiler emitted, opening on its banner,
  `lx/hashes/prog.c` (3,078,028 bytes), `lx/taint/prog.c` (3,079,067) and `q3/ct/ct.c` (3,066,048);
  and one disassembly, `q3/ct/eq-O2.s` (2,509,529 bytes);
- 60 `build/` folders, 7 `tmp` folders, and two `.git` folders, `253-llm-ergonomist/.git/` and
  `253-critic/work/area/.git/` (18 files and 27,305 bytes each).

**A blind spot of every diff.** The frozen commit holds 287 files under folders named `build`, which
each diff leaves out on both sides, as 251's did; four of them are real sources,
`packages/rebel/0.1.0/files/sql/build/{checks,clauses,statements,tokens}.hero`, so a change a seat
made there would not show. Measured when packed, each tree's 287 and `rebel-mut`'s 4 are
byte-identical to the frozen commit's (the `runtime/` copy holds none). `253-ffi-pragmatist/` also
holds build output of its own in `examples-rebel/build/`, 2,423 files, left out with the copy.

**Read back and scanned.** `tar -tJf` lists 994 members, the 960 of `253-pack/list.txt` and the
packer's 34, in the order they were packed, with no folder entry, no `._` file and nothing under a
`home/`; no member carries a pax header or an extended attribute. Each member read back from the
archive is byte-identical to its source and to its line of the manifest. The folder was then rebuilt
from the archive and the frozen commit, into an empty folder, by the steps below
(`253-pack/checks/rebuild-test.sh`): the 960 kept files and the 467 duplicates came back identical,
each of the 7 copies identical to its source with no line of difference, and the four `home/`
folders absent, 103,371 files rebuilt in all.

Every file the archive carries but the scan's report and the manifest, 992 files, was scanned
(`253-pack/scan.py`, its report `253-pack/scan.txt`: a path, a line, the pattern and the shape of
what follows, never a value) for an `ANTHROPIC_API_KEY` and what follows it, `sk-ant-`, `ghp_`,
`github_pat_`, `gho_`, a private key's `BEGIN` line and an `Authorization:` header, then a wider
sweep for `x-api-key`, `Bearer`, `AKIA` and `sk-` shapes. There were 152 matches:
- 79 in the packer's own files: 11 in `scan.py` and 2 in `checks/scan-values.py`, their own
  patterns, and 66 in `checks/scan-values.txt`, which names the pattern of each match below and no
  value;
- 2 in `253-llm-ergonomist/run.sh:49`, `sk-ant-` and `sk-`: **a string in a key's shape**, 39
  characters after its prefix, which the script holds as `REFUSED_KEY` and passes to its preflight
  session. The preflight's record, `253-llm-ergonomist/preflight/run.json`, holds the service's
  answer to that session, HTTP 401, `Invalid API key`. It is not a working key, but the archive
  carries it in a key's shape; whether it stays is the coordinator's choice;
- 2 in `253-llm-ergonomist/run.sh:95` and `253-critic/work/area/run.sh:83`, which pass the variable
  on by reference;
- 69 in 13 files: the 12 pages of `253-compiler-engineer/q2/run1/` that hold one (`acme`,
  `crypto__keypairs`, `http`, `jose`, `llm__anthropic`, `llm__openai`, `mcp`, `oauth2`, `openapi`,
  `pem`, `s3`, `web`; each `.out` opens on a package's name and its documentation and quotes its
  tests) and `253-briefwriter/q4/rebel-skips-list.txt`. `checks/scan-values.py` looked for each
  value in the frozen commit's tree, the public repository (`checks/scan-values.txt`, a path, the
  pattern, the value's length and where, never the value): each of the 63 values of 3 bytes or more
  stands in a file of it, and the other 6 are 0 or 1 byte. The three `AKIA` matches, in `s3.out`,
  are the access key id AWS's own documentation uses as its example. The three `BEGIN PRIVATE KEY`
  matches are tests: two assert a block's first line, and one, in `pem.out`, is a literal of an
  encrypted key's headers whose body is 4 characters.

No credential was found, and no file was removed.

No issue draft and no decision draft stood at any of the roots when they were packed (no
`defects/`, no `decision-*` file).

**How to rebuild the folder.** In an empty folder that stands for `rebel-scratch/`, with `F` a
folder outside it:

```sh
mkdir -p "$F" && git -C <a checkout> archive 181555e9a32d0b18878f9f10e48d2eadd939c285 | tar -x -C "$F"
tar -xJf evidence.tar.xz
TAB="$(printf '\t')"
# the duplicates
tail -n +2 253-pack/DUPLICATES.tsv | while IFS="$TAB" read -r p k; do mkdir -p "$(dirname "$p")" && cp -p "$k" "$p"; done
# the named folders, each an evidence archive of the frozen commit unpacked (none for this sitting)
grep -v '^#' 253-pack/named.txt | while IFS="$TAB" read -r d a; do mkdir -p "$d" && git -C <a checkout> show 181555e9a32d0b18878f9f10e48d2eadd939c285:"$a" | tar -xJ -C "$d"; done
# the copies: the frozen tree or folder, then its diff, its --- paths made the copy's own
grep -v '^#' 253-pack/copies.txt | while IFS="$TAB" read -r c base rest; do
  mkdir -p "$c"
  if [ "$base" = . ]; then cp -R "$F/." "$c/"; pre=253-pack/work/frozen/; else cp -R "$F/$base/." "$c/"; pre="253-pack/work/frozen/$base/"; fi
  sed "s|^--- $pre|--- $c/|" "253-pack/diffs/$(printf %s "$c" | tr / _).diff" | patch -p0 -E -s
  find "$c" -type d -empty -delete
done
# what a diff cannot carry (nothing for this sitting)
tail -n +2 253-pack/empty-in-copies.txt | while IFS="$TAB" read -r p why; do case "$p" in */) mkdir -p "$p" ;; *) case "$why" in new*) : > "$p" ;; *) rm -f "$p" ;; esac ;; esac; done
```

The `sed` is 251's: each diff's `---` line names the frozen commit where `diffs.py` read it,
`253-pack/work/frozen/`, and the `sed` makes it the copy's own path, as the rebuild test ran it
(Apple's `patch` 2.0-12u11). What the rebuild cannot give back is what the archive left out: the
blind areas' `home/`, the binaries, the files of 2 MiB or more, and the folders left out by name.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
