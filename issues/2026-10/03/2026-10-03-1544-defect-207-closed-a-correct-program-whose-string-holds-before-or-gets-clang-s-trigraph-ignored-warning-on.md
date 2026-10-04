# Defect 207 closed: a correct program whose string holds `??` before `)`, `(`, `<`, `>`, `=`, `/`, `'`, `!` or `-` gets clang's *trigraph ignored* warning on the emitted C

- [x] **207 — a correct program whose string holds `??` before `)`, `(`, `<`, `>`, `=`, `/`, `'`, `!` or `-` gets clang's *trigraph ignored* warning on the emitted C** | `print("what???)")`: `check` exit 0, `run` exit 0 and prints `what???)`, and clang prints `build/tu-<key>/tri.c:10:42: warning: trigraph ignored [-Wtrigraphs]`; the net's own tests' build prints it twice for the harness's own `print(???)` | the emitter's string literals (a `?` that would begin a trigraph is written `?\?` in C) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane round1003b's gate, 2026-10-03, seeing it in the net's own tests' build and in the last gate's (`scratchpad/lane-round1003b/progress.md`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` before 08:00 by `date` (`scratchpad/file-r5/tri.hero`, 2026-10-03).

    **2026-10-03, lane warn, every text the emitter writes into C is spelled
    for where C reads it (`selfhost/emit/c_text.hero`): a literal or a
    `#line` name writes a `?` that would end a trigraph as `\?`, a header
    name is split by a line splice, and the comment opening a unit neither
    closes nor opens one; the shapes beside it with its cause included, the
    four `#line` writers that escaped nothing (a directory `a\q` or `a"b`), a
    line end in a path and a directory `a*` (both exit 2), a header named
    `h??).h`**: repaired at `ed776fb2`, gated by its own cases on this Mac;
    the rest is owed at the round's gate, and the platform legs before the
    push. On Linux arm64 since (Debian clang 22.1.8, a copy of the tree with
    206's repair beside it), the case prints its `.expected` with an empty
    stderr and the compiler tests of the modules it touched read 31 and 0;
    the two spellings were measured there under 18.1.8 too; unrun on Windows.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

    **Closed 2026-10-03** after the push of `e339ece9`. Repaired at `ed776fb2` in lane warn, it entered the trunk at the round of 2026-10-03's sixth gate, `da3e29af` (lanes warn, ffimsg, depth, twin156 and win214): the seed regenerated once, 39,690,755 bytes, SHA-256 beginning `2c809845ed0f7ba8`, its fixpoint by `cmp`; the compiler's own tests 1,099 and the net's own 200, all passed; the full net, 26 suites, 5,033 passed and 0 failed; the censuses of `check --brief` over 1,905 files and of `--emit-c` over 595, every move attributed to its lane. Its cases read one by one at `da3e29af`, whose compiler `e339ece9` carries unchanged (no file under `selfhost/`, `seed/`, `runtime/` or `tests/` moved between them): on this Mac (Apple clang 21), in the Linux arm64 container under Debian clang 22.1.8 and again under 18.1.8, and on the Windows box (clang 23.1.1): `run/fixedbugs-207-*` 1 of 1 on each, none skipped, and its five compiler tests `ok` on each. The push's legs: Linux arm64, its suites four at a time, the compiler's own tests 1,099 all passed and 20 suites at 0 failed under each clang; the Windows box, the compiler's own tests 1,099 all passed and nine of its 20 suites at 0 failed before it went offline at about 14:47 (Tailscale, read at 15:41: last seen 54 minutes before), every case above already read there; and the CI's run 37124159403 on `e339ece9`: Linux x86-64 (Ubuntu clang 18.1.3), the compiler's own tests 1,099 all passed and 26 suites, 4,993 passed and 0 failed; Windows x86-64 (clang 20.1.8), 1,099 and 26 suites, 4,932 passed and 0 failed, the box's eleven unrun suites among them; Linux arm64 and Darwin arm64 green. Which golden cases those jobs ran their logs do not say, so the readings above are the measurement of them, and x86-64's an inference from Linux arm64 under the same clang major. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
