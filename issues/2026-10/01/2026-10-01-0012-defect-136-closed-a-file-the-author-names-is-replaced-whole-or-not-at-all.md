# Defect 136 closed: a file the author names is replaced whole or not at all, and a write that fails or is killed leaves it as it was

- [x] **136 — `fmt --in-place` and `check --apply --in-place` destroy the author's source when the write fails** | on a disk nearly full, `heroes fmt prog.hero --in-place` answers `error: cannot write`, exit 2, and leaves `prog.hero` at 0 bytes, the 62,706 bytes the author wrote gone; `heroes check --apply --in-place` leaves the file cut mid-token, 57,344 of the 59,312 bytes it meant to write, ending in `retu` | `selfhost/cli/syntax_cmds.hero:66` · `selfhost/cli/check.hero:110` · `runtime/parts/os.c:635` (`hero_file_write`) · `selfhost/cli/publish.hero` (defect 134's publish by rename) · **closed 2026-10-01**

    **Origin:** lane 134's agent, 2026-09-30, reading the code while it
    enumerated every path a build writes (its report: *`fmt --in-place` and
    `check --apply --in-place` write the author's source in place*, not run).
    Reproduced by the coordinator at 06:55 and 06:56 on the trunk's compiler
    at `e5cc73eb`, and again at 09:08 at `f37ea722`, on a 2 MB HFS+ disk image
    (`hdiutil create -size 2m`), filled with `dd` to 8 KB and 4 KB free.

    **Measured.** A program of 62,706 bytes that `fmt` writes as 80,708
    (`v0=0+0*2` over 3000 lines becomes `v0 = 0 + 0 * 2`): `fmt --in-place`
    exit 2, the file 0 bytes, twice. A program of 50,312 bytes whose 1500
    certain swaps of `fn` for `function` write 59,312: `check --apply
    --in-place` exit 2, the file 57,344 bytes, refused at 4358:5 with
    `unknown_name` for `retu`; the original is in neither.

    **The cause, read and not yet proved by a repair.** `hero_file_write`
    opens the path with `fopen(path, "wb")`, which empties it before a byte is
    written, then writes; when the write fails the old text is already gone and
    the new one is not all there. Both in-place verbs call `write_file` on the
    author's own path.

    **Why it is a defect.** The tool destroys the program it was asked to
    rewrite and says only that it could not write, so an author who reads the
    message and looks at the file finds nothing; a full disk is ordinary (the
    Windows box stood at 99% on this night, `run` reading 45 false failures),
    and a killed process or a lost power mid-write is the same shape. Defect
    134's publish by rename, a private name beside the destination and a
    rename over it, is the shape that leaves the old text whole whenever the
    new one cannot be.

    **Unrun, questions rather than premises**: a kill mid-write; Windows;
    what a rename owes a file's mode, owner and links (a read-only source, a
    symlinked one: a publish by rename replaces the link, not what it points
    at); and the library's own `write_file`, which user programs call and whose
    comment promises *the text, written whole, replacing whatever was there*
    (`selfhost/library_source.hero:220`), with the same truncate-first body:
    whether that promise is kept the same way is a question about the
    library, and a change to what it does is the panel's.

    **2026-09-30, lane 136, a file the author names is replaced whole or not
    at all, and a write that fails or is killed leaves it as it was**:
    repaired at `1f173a42`, gated by its cases and the compiler's own tests;
    the net is owed at the lane's close.

    **The repair**, lane 136, branch `lane-136` from `77b8ca98`, the trunk
    merged into it once at `9cc3a295`: `1f173a42` and `9775d2f1`, closed at
    `2ca85531`. **The class**: every file whose name the author gives a verb
    (the source `fmt --in-place` and `check --apply --in-place` rewrite, and
    an `-o`) goes through `publish.rewrite` and `publish.landing_for`: the new
    text is staged under a private name beside the file its path finally lands
    in, given that file's owner, group, mode, flags, ACL and extended
    attributes, flushed, and renamed over it; a symbolic link is written
    through and stays a link; a file that could not be opened for writing, one
    with a second name, a directory and a directory that is not there are
    refused before a byte is written; a copy that cannot take the name is
    removed whatever ACL it carries; every refusal and failure says what failed
    and that the file is untouched. The C is `runtime/parts/replace.c`, its
    Heroes half `selfhost/cli/files.hero`, split out of `cli/process.hero` past
    its 300 lines.

    **The entry's shapes, before and after**: on a 2 MB disk image, `fmt` at
    4 and 8 KB free left 0 bytes and `check --apply` at 4 KB 57,344 cut bytes;
    after, the file byte for byte (reason 28). By SIGKILL inside the write, on
    this Mac: before, 13 of 44 kills left the file empty or cut; after, 0 of
    25 (21 left the original with its private name beside it, 4 the new
    text). In the Linux container: at 8 KB free 73,728 cut bytes before; after,
    the original at 8, 32 and 64 KB, with modes, links, `user.*` extended
    attributes, the POSIX ACL and the group kept, and a read-only file refused
    for a user and rewritten for root, as `fopen` did. **The price, measured
    and kept**: `fmt` at 16 to 64 KB free and `--apply` at 8 to 32 KB now
    refuse where the in-place write fitted, the new text needing room beside
    the old; the file is untouched and the message says so. **On the Windows box**
    (`9775d2f1`, the arm the header had called unverified): `fmt --in-place`
    on a 16 MB NTFS volume left the file at 0 bytes with 8 KB free before the
    repair, and byte for byte at 8, 32 and 64 KB free after it, reason 112 and
    the file said untouched; a read-only file, and one PowerShell holds open
    for reading alone, are refused untouched (5 and 32); a hidden, a system
    and a hidden system file are rewritten with their attributes, where
    `fopen` refused all three; a second name is refused; a link, to a source
    or as an `--emit-c -o`, is written through and stays a link; `-o NUL` is
    written into. No kill landed inside the write there, 13 tries per
    compiler: that window is unrun on Windows.

    **Cases**: twelve in `selfhost/cli/publish.hero` (the closing commit
    corrects its own first body's *eleven*); defect 134's two reworded to the
    one sentence; the net's `surface` pair under a 1,024-byte write ceiling
    (`hero_run_limit_writes`), 2 failed on the base compiler, 0 on this one.

    **Every place a verb writes a file the author names** (the lane's
    enumeration, from `grep write_file`, `-o`, `--in-place` and the verb
    table): `fmt --in-place` and `check --apply --in-place` (truncate first,
    now `rewrite`); `build --emit-c -o` (whole since defect 134, but it
    replaced read-only files and links, reset the mode and refused
    `/dev/null`; now `rewrite`); `build -o` and `run -o`, with a `.pdb` or
    `.dSYM` beside them (now `landing_for`, then the rename; the binary's mode
    stays the linker's and a hard link is broken, as `ld` does); `build
    --dump-ir -o` ignores `-o` and prints (measured, unchanged). Everything
    else a verb writes lives under `build/` and is not the author's.

    **Not changed, and the panel's**: the library's `write_file`, what a
    user's program calls, still empties the file first: over a 62,706-byte
    file on a full disk it left 0 bytes, 3 runs of 3 at 8, 64 and 256 KB free,
    saying only *could not write* (`scratchpad/lane-136/r2/userw/w.hero`); the
    item named it as a question about the library.

    **Unrun, questions rather than premises**: a power cut rather than a kill
    (the stage is flushed, `F_BARRIERFSYNC` on Darwin and `fsync`, and the
    directory after the rename, best effort; no power was cut); a kill
    landing inside the write on Windows (none landed in 13 tries per
    compiler, the watcher slower than the write). **Not carried, documented
    and without a case**: on Windows a file's owner, its explicit ACL entries
    and its alternate data streams (a new file there takes its directory's
    owner and inherited entries); anywhere, a file's birth time and inode.

    **The gates.** Lane 136's own, at its close `2ca85531`, with defect 137
    beside it: the seed regenerated from `selfhost/`, 33,727,341 bytes,
    SHA-256 beginning `6b094cdef4c06596`, the compiler clang builds from it
    emitting it again byte for byte (`cmp` silent), before `9775d2f1` and
    after it. **This Mac**: the 25 suites other than `cache`, six at a time,
    each 0 failed (annotations 347, canonical 2, check 295, corpus 55,
    descriptors 303, determinism 241, emission 638, emit 8, fixes 541, grammar
    9, ir 24, layout 4, lines 212, order 3, records 24, run 211, runtime 8,
    spec 20, special 10, surface 336, units 3, unsupported 15, warnings 272,
    wholes 303), `probe` 24 and 0 re-run alone (21 and 1 in the parallel
    pass, its `selfhost, multi` stopped on the harness's own timeout, exit
    124, beside five other suites), `cache` 7 and 0 alone after them; the
    compiler's own tests 947 and the net's own tests 184, all passed; after
    `9775d2f1`, `runtime` 8 and 0, `cache` 7 and 0, own tests 947. The census
    of `check --brief` and its exit over the 1,394 tracked `.hero` files, the
    trunk's compiler against the lane's, both arms: 0 moved. **Linux x86-64**,
    the container, on the lane's tree but for `9775d2f1`'s comment (tree
    `eba4fb73`, its seed the same SHA-256): the compiler's own tests 947, all
    passed, and the 19 suites each 0 failed (surface 336, canonical 2,
    annotations 347, check 295, fixes 541, layout 4, order 3, lines 208, run
    207, emission 626, determinism 237, wholes 303, descriptors 303, cache 7,
    grammar 9, spec 20, probe 24, corpus 53, warnings 266), read at 21:11 from
    `docker logs` of the container (the lane's own log had stopped at `layout`
    when its session's host process died, the container running on).
    **Windows**, the box, at `9cc3a295` with the lane's compiler built from
    its `selfhost/` by the trunk's seed: its own tests 947, all passed;
    `surface` 327, `fixes` 541 and `probe` 24, each 0 failed.

    **The integration gate**, the lane merged with lanes recovery-b4 and 135b
    at `56d0631b` (branch `integrate-0930`, from the trunk at `93375d63`): one
    interaction, and it was this lane's: defect 137's case and fixture asked
    `--apply` for labels on `copy_text("a", "b")`, which defect 135's rule
    (`5ea6b86f`) makes a guess, both parameters being `str`; the later repair
    redone at `638c73f7`, both now `copy_text("a", to: "b")`, a label the rule
    certifies, the trunk's compiler still corrupting that line, so the case
    still witnesses defect 137. Closed at `3cc3b553`: the seed regenerated
    once, SHA-256 beginning `e4723edfe15d71f7`, its fixpoint by `cmp`; the
    compiler's own tests 960 and the net's own 184, all passed; the full net
    4,023 passed and 0 failed over 26 suites, every one green at its first
    pass, `cache` alone; the census over 1,448 files, every move a lane's own
    case; Linux x86-64 on `3cc3b553`, 960 tests and 19 suites, 3,899 passed
    and 0 failed. The trunk fast-forwarded to `3cc3b553` at 00:03 on
    2026-10-01. **Speed**, on a still machine (load 1.5 to 2.4), the trunk's
    compiler before against after, interleaved: `check selfhost/main.hero`
    4.30, 4.26, 4.27 s against 4.31, 4.32 s (a third, 4.90 against 4.27 of
    `user`, waited and is discarded), +1%; a warm `build selfhost/main.hero`
    49.23, 49.36 s against 49.37, 49.60 s, +0.5%. **Owed before the push**:
    Linux arm64 and the Windows box, on the trunk.

    **Linux arm64, 2026-10-01 11:14**, on the trunk at `9d1c209d`, whose
    `selfhost/`, `runtime/`, `seed/`, `tests/`, `examples/` and `spec/` are
    those of `3cc3b553` (`git diff --stat` over those paths prints nothing),
    in the `heroes-linux-arm64` image with clang 22.1.8, the seed built there
    (its SHA-256 begins `e4723edfe15d71f7`, the integration's): the compiler's
    own tests 960, all passed, and the 19 suites each 0 failed, 3,899 passed.
    **The Windows box is owed still**: its leg refused to start at 10:51, the
    `ssh` connection to the box timing out, and nothing was sent.
