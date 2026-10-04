---
kind: defect
area: cli
milestone: none
filed: 2026-09-30
commit: 6d781be1b34e97d18deb2b80b2d666d9f9d3fbb4
github: none
---

# Defect 134 closed: two builds at once in one tree write private names, publish by rename, and say what failed

- [x] **134 — two builds at once in one tree fail each other with a false internal error** | on a cold `build/`, six `heroes run p.hero` started together fail five in six, 25 of 30 over five rounds, with `internal error: the runtime did not compile` or `internal error: cannot publish build/tu-<key>/p.o` then `error: clang refused the generated C`, where clang refused nothing; two DIFFERENT programs built together in a fresh tree fail one of the two in 5 rounds of 5 | `selfhost/cli/units.hero` (`tu_object`, `link_objects`) · `selfhost/cli/toolchain.hero` (`runtime_object`, `link`) · `selfhost/cli/produce.hero` (the words) · **closed 2026-09-30**

    **Origin:** lane recovery-b1's agent, 2026-09-30, reading why `canonical`
    went red in its batch gate's parallel pass: six harness processes raced
    to publish one `build/<hash>/main`. Reproduced by the coordinator at
    01:55 in a scratch tree with the trunk's compiler at `1fc77e31`, `p.hero`
    being `function main()` over `    print(1)`.

    **Measured on macOS arm64**, each round from `rm -rf build`, six
    processes started together: `heroes run p.hero`, 25 of 30 exit 2 (24
    `the runtime did not compile` then `error: no runtime object`, one
    `cannot publish build/tu-4dc4140d0e94dae5/p.o` then `clang refused the
    generated C`); the runtime object warm and the program cold, 26 of 30
    (`cannot publish` of `p.o` 25 times, of `library.o` once, each followed
    by `clang refused the generated C`); `p.hero` and `q.hero` together, one
    of the two in every round of five; `heroes build p.hero -o o-<i>`, 15 of
    18; `-o same`, 15 of 18, the published binary running; `build --emit-c`,
    10 of 18.

    **The cause, read and not yet proved by a repair.** Every artifact a
    build publishes is staged under ONE name, `object + ".tmp"` or `binary +
    ".tmp"`, and the translation unit's C is written in place, so two
    processes on one key write one file: the first `rename_over` moves that
    one staged file into place, the second finds its own gone, and a clang
    may read the C while another process truncates it. The words then
    misname it: `produce.hero` answers every failure of the link rounds as
    `clang refused the generated C`.

    **Why it is a defect.** A build that fails because another build of the
    same thing ran beside it is a false failure, and its message sends the
    reader to their program or to clang. Two builds in one tree are
    ordinary: an editor's build on save beside a terminal, `make -j`, a
    script over several programs, and the net's own parallel pass, whose
    rule *a suite red in the parallel pass is re-run alone*
    (`.claude/rules/verification.md`) is this defect's cost paid by hand.
    Defect 057's record says *the one collision left is named rather than
    hidden: two builds compiling the identical TU do share its directory,
    and their clang output is identical too*; they share the staged names
    and the C file as well.

    **Unrun, questions rather than premises**: Windows, where `MoveFileExA`
    over a binary another process is running fails where a POSIX rename
    does not; `heroes test` of one program twice at once; a warm cache's
    replay files (`warnings.txt`, the dependency listings) read by one
    process while a second rewrites them, which would lose a warning from
    every later replay, 057's class.

    **2026-09-30, lane 134, a private file has a name no other process uses,
    and a failed filesystem call says why**: repaired at `bc402e8d`, gated by
    its cases and the compiler's own tests; the net is owed at the lane's close.

    **2026-09-30, lane 134, a cached object, its replay and its record are
    published as one**: repaired at `e2f475fe`, gated by its cases and the
    compiler's own tests; the net is owed at the lane's close.

    **2026-09-30, lane 134, a binary is linked under a name of its own and
    lands where its link line says**: repaired at `3ff8af48`, gated by its
    cases and the compiler's own tests; the net is owed at the lane's close.

    **2026-09-30, lane 134, a failure that is not clang's refusal is not
    worded as one**: repaired at `c1f32307`, gated by its cases and the
    compiler's own tests; the net is owed at the lane's close.

    **2026-09-30, lane 134, the net holds it, six builds of one program
    started at once all succeed**: `bf2b29ed`, gated by the net's own tests
    and `cache`; the net is owed at the lane's close.

    **2026-09-30, lane 134, the Windows arm of a removal keeps its answer in
    the width it was given**: repaired at `ad117d2f`, found and gated on the
    Windows box; the net is owed at the lane's close.

    **2026-09-30, lane 134, publish's cases force a failed rename the one way
    every platform refuses**: `b5462c86`, gated by the compiler's own tests
    on this Mac and on the Windows box; the net is owed at the lane's close.

    **2026-09-30, lane 134, an author's -o is published as written, never
    split to name the linker's output**: repaired at `d45de7c6`, gated by its
    cases and the compiler's own tests here and on the Windows box; the net
    is owed at the lane's close.

    **2026-09-30, lane 134, three comments name what the repairs left**:
    `722bc973`, the seed byte-identical across it. The lane's gate follows
    in its closing commit, the net on this Mac, Windows and Linux x86-64.

    **The repair**, lane 134, branch `lane-134` from `c85bccb8`, merged into
    the trunk at `f37ea722` by a merge gate beside the recovery cluster's batch
    2. Every file a build writes before publishing it is written under a name
    no other process uses, `<name>.<pid>-<serial>.tmp` (`hero_run_serial` for
    the serial), published by `rename_over`, and read by anyone else only
    once published; a publish that fails where a keyed file is already there
    keeps it and throws the staged copy away, where an author's destination is
    reported instead (`selfhost/cli/publish.hero`). A failure that is not
    clang's refusal is worded by what failed, with the path and the operating
    system's own reason (`hero_fs_why`, errno or `GetLastError`, no ABI move):
    *clang refused the generated C* and *the runtime did not compile* appear
    only when clang exited non-zero, the latter now with clang's own words.
    The commits: `bc402e8d` (private names, the reason), `e2f475fe` (an
    object, its replay and the record that vouches for it published as one,
    the record last), `3ff8af48` (a binary linked in a private directory,
    landing in `build/<digest of the link line>/`), `c1f32307` (the words;
    `--emit-c -o` published whole), `bf2b29ed` (the net's case), and the
    gate's own finds, `ad117d2f` (a Windows-only line), `b5462c86` (two cases
    whose premise the Windows box falsified), `d45de7c6` (an author's `-o`
    never split to name the linker's output), `722bc973`; `c6d63959` closed
    the lane with the seed.

    **What the class was, enumerated from the code** (31 write paths in
    `selfhost/cli/`, lane 134's table): the runtime object's staging name,
    depfile, stderr file and record; per translation unit the C file, the
    staged object, the depfile, the stderr file, `warnings.txt` and the
    record, most written in place and the record published before the
    object; one `build/<hash>/` for the binary, hashed from the source alone;
    one `pkg-config` answer and complaint for the whole tree; the pointee
    verdict's files in place; `--emit-c -o` in place. **The premise the
    lane's brief gave, that every path under `build/` is keyed on the
    compiler, the flags, the text and the runtime, was false**: the binary's
    key was the source alone and an object's key held no headers; the repair
    made it true (objects named by their record's digest, the binary by its
    link line's), which is what makes keeping another build's file sound.

    **Wider than the entry, measured by the lane before the repair**: three
    `libzstd` and three `liblz4` programs built together told the author at
    exit 1, 8 times in 36, that `liblz4` answered with `td` or that `lz4.h`
    is not on the include path; a unit that warns failed 123 builds in 150,
    and of the 27 that succeeded 4 lost their warning and 3 had NUL bytes in
    their stderr; a refused `frexp(@e: i16)` gave 2 false internal errors in
    60; and three states a kill at the wrong moment leaves were served as
    good: an emptied `warnings.txt` silenced every later replay, a newer
    record beside an older object printed 7 where the header said 8 at exit
    0, an emptied pointee verdict failed every later build with `internal
    error: ` and nothing after it. After the repair every one of those reads
    0, and the three states cannot be reached.

    **The entry's shapes, before and after** (six processes started together):
    on macOS 25 of 30 failing in `run`, 25 of 30 in `build -o`, 25 of 30 in
    `--emit-c`, one of two programs in every round, all 0 after (18 of 18
    binaries printing their answer); on the Windows box 15 of 18 before in
    every shape and 0 after, the one exception 1 of 18 builds to one `-o
    same.exe` reporting `cannot publish same.exe.pdb`, the operating system's
    reason 5, which is the rule for an author's destination; a second build
    over a binary another process runs, under `build/`, exit 0 (the keyed
    file kept), and to an author's `-o`, `error: cannot publish held.exe:
    the operating system's own reason is 5`, 3 of 3.

    **The gates.** Lane 134's own, on its closing tree: the 25 suites six at a
    time and `cache` alone, each 0 failed, the parallel pass needing no re-run
    in any of three passes (it had needed one in every batch gate before the
    repair); the compiler's own tests 916 and the net's own 184; the census of
    1,353 tracked files, both arms, 0 moved; Linux x86-64 (`d45de7c6` with the
    closing seed) 916 tests and 19 suites, each 0 failed; on the Windows box,
    the 19 suites at `ad117d2f`, each 0 failed, the final compiler's own 916
    tests twice, and 7 of the 19 suites on the closing tree before the box
    stopped answering. The merge gate `f37ea722`, beside batch 2: the
    compiler's own 924, the net's own 184, the full net 3874 passed and 0
    failed with no suite re-run, the census of 1,361 files 0 moved. Linux
    x86-64 at `f37ea722`: 924 tests and 18 suites at 0 failed at first
    reading, and `probe` 21 and 1 in that leg under a load the Mac was
    measured at 47 on 8 cores, 24 and 0 re-run alone (`real 5m54s`, `user
    5m52s`). And the merge gate `77b8ca98`, the recovery cluster's batch 3
    beside it, 3907 passed and 0 failed: `emission`, `run` and `descriptors`,
    red in batch 3's own parallel pass on this defect's `cannot publish`,
    green at first reading there.

    **Owed before the push, and unrun here**: the 12 Windows suites the box
    did not run on the closing tree, and Linux arm64, both on the tree that
    is pushed; a power loss rather than a kill (a publish is not preceded by
    `fsync`, so an empty published file after a machine crash is a
    question); a kill landing inside the narrowest windows (the lane built
    those states by hand instead). Found beside the repair and filed apart:
    `fmt --in-place` and `check --apply --in-place` write the author's source
    in place (defect 136). Found and not defects, the lane's words: the cache
    keys depend on the working directory, since `clang -###` prints it;
    `--emit-c` compiles a runtime object it never links; a killed build's
    private files are never removed.

    **Windows, on the final tree, measured after the close**: at the trunk
    `6d781be1` (the code of `77b8ca98`: this defect and the recovery cluster's
    batch 3), the box answering again with 62 GB free, the compiler's 930
    tests and the 19 suites, each 0 failed, by 11:42 on 2026-09-30; `cache` 7
    and 0, the six builds started together on six threads among them. The 12
    suites the box had not run on lane 134's closing tree are in those 19.
    Linux arm64 on the trunk remains before the push.

    **Linux arm64, on the final tree, measured after the close**: at the
    trunk `ec1fd0a9`, whose `selfhost/`, `runtime/`, `seed/`, `tests/`,
    `examples/` and `spec/` are those of `77b8ca98` (`git diff --stat` between
    the two over those paths prints nothing), in the `heroes-linux-arm64`
    image with clang 22.1.8, the seed whose SHA-256 begins `4dc43f8a6410e4bf`
    built there, the compiler's 930 tests and the 19 suites, each 0 failed,
    by 14:46 on 2026-09-30. This Mac, Linux x86-64, the Windows box and Linux
    arm64 have now each run this defect's repairs on the code that is pushed.
