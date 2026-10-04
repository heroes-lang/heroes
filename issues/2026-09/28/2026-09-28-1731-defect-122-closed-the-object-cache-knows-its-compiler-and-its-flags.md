# Defect 122 closed: a build's object cache knows which C compiler and which flags built an object

2026-09-28, M-agreed-retention step 29, in lane 182 (`81532acc`, the trunk
merged into it at `6233d4a5`, design.md and the verification map at
`44a1196b`), merged `c8ed80bc`, on panel 182's provisional resolution
(`docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md`).

- [x] **122 — a build's object cache ignores which C compiler and which flags built an object, so a changed compiler or flag reuses stale objects** | the unit key (`selfhost/cli/units.hero:82-84`) is the fingerprint, level, module, text, runtime and search paths, the fingerprint being `VERSION`, and the runtime object's (`selfhost/cli/toolchain.hero:154`) likewise: a program printing `__clang_major__` built under Apple clang 21 prints 21, built warm with Homebrew clang 22.1.8 first on the `PATH` still prints 21, and built cold prints 22; a compiler with one more flag in its list and the same `VERSION` reused every object (the critic) | `selfhost/cli/units.hero` · `selfhost/cli/toolchain.hero` · `selfhost/cli/clang_floor.hero` (which already writes `clang --version` on every build) · **closed 2026-09-28**

    **Origin:** panel 182's compiler-engineer, 2026-09-28, reading the key
    while probing a pattern-initialised build; run by the sitting's critic
    (clang 21 against 22 on a warm directory, and a flag added with the version
    unchanged), and reproduced by the coordinator the same morning in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect122/`.

    **Why it is a defect.** A build that silently links an object compiled by
    another compiler, or without a flag the compiler now passes, is a program
    other than the one its source and its compiler say, at exit 0; and it is
    the precondition for every new instrument leg the sitting names.

## The repair

Four caches were keyed without the C compiler, not the two the entry named:
the unit object, the runtime object, the fused build directory and the pointee
check's cache. All four now key on clang's `-###` answer for the flag list,
whose first lines are its `--version` banner and whose rest is what the flags
resolve to on this machine (SDK, sysroot, target), and on the flag list itself;
the floor check reads the same answer, so a build starts no extra clang
process. Measured by hand, the trunk's compiler against the lane's: the
coordinator's reproducer (Apple clang 21, Homebrew clang 22.1.8 warm, Apple,
22) printed 21, 21, 21, 21 and now 21, 22, 21, 22; a compiler with one more flag
and the same `VERSION` built warm printed the stale 2 and now 1; a program over
raylib, the clang switched under `run` and under `build` at `-O0` and `-O2`, was
served stale every time and is rebuilt every time, and under `run --sanitize`
clang 22 had refused it outright. Every existing `build/` rebuilds cold once
under the new keys (inferred from the code, unrun).

## The gate

In the lane, one suite at a time, the compiler built from the regenerated seed,
the fixpoint by `cmp`: the compiler's 803 tests, the net's own 178; run 210,
determinism 240, ir 24, emit 8 (four goldens edited by hand, 86 lines, each a
dropped value zero), canonical 2, warnings 271, lines 211, corpus 55, check
153, annotations 199, surface 325, layout 4, order 3, records 24, probe 24, and
wholes, descriptors, cache and units, each 0 failed; `emission` blessed by its
own procedure, 259 files and 21,946 lines each way, every line a dropped value
zero (21,770), a dropped `@` parameter's slot zero (93, each with its copy-in)
or a tag write made whole (83), then 636 and 0. After the merge of the trunk at
`5fdd0edd`: the whole net, 3,120 passed and 0 failed; the compiler's 820 tests
and the net's own 178.

**Robustness**, the sitting's instruments on the landed seed: 21,566 `= {0}`
(106,734 before; 21,652 after the merge); clang's `-Wuninitialized
-Wsometimes-uninitialized -Wconditional-uninitialized` on the seed 0, and with
every remaining zero stripped 21,566 of 21,566 variables flagged, all slots, no
value; the critic's `defassign2.py` 0 values reached unwritten over the seed,
the blessed corpus and the 119 FFI emissions; a compiler built with
`-ftrivial-auto-var-init=pattern` compiles itself to identical bytes, and `run`
under the pattern on an emptied `build/` is 210 and 0; the ffi-pragmatist's
`boundary.py` 0 zeroed locals among 420 extern arguments, 25 addresses and 8
thunks.

**The time**, user seconds, three interleaved rounds on a still machine (load
1.60 to 2.31, real within 0.14 s of user plus sys): `fmt` on 36,688 lines 3.05
against 2.39 with the compiler built plainly (0.78) and 0.71 against 0.66 at
`-O2` (0.93); `check selfhost/main.hero` 4.29 against 3.34 (0.78) and 0.96
against 0.87 (0.91); `build --emit-c` 35.05 against 32.18 (0.92) and at `-O2`
within noise.

Linux arm64 on the lane's tree (`44a1196b`, the `heroes-linux-arm64` image):
the compiler's 820 tests and surface 332, canonical 2, annotations 210, check
161, fixes 30, layout 4, order 3, lines 207, run 206, emission 624,
determinism 236, wholes 302, descriptors 302, cache 6, grammar 9, spec 20 and
probe 24, each 0 failed. The Windows box on the same tree, one archive whose
sha256 matched on both sides (`c458cac54c35feba`), seed `c75c72eca1e93e88`:
the compiler's 820 tests and surface 325, canonical 2, annotations 210, check
161, fixes 30, layout 4, order 3, lines 205, run 204, emission 602,
determinism 234, wholes 302, descriptors 302, cache 6, grammar 9, spec 20 and
probe 24, each 0 failed; the cache key read from clang's `-###` answer holds
under Windows' clang. On the trunk after the merge (`c8ed80bc`): the fixpoint
by `cmp`, the compiler's 820 tests, the net's own 178, records 24, surface
332, spec 20, probe 24, wholes 302 and cache 6.
