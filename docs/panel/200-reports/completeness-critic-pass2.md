# Panel 200, completeness critic, second pass

Copied by the coordinator at 20:54 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

I'm the second-pass completeness critic for panel 200, and I give no verdict. I worked only in `.claude/worktrees/scratch-b15/200-critic2/` (a copy of the frozen tree at `36be56d0`, my compiler built from its seed, TMPDIR inside it). Work ran from 20:37:55 to 20:52:16 by `date`, and nothing is left running (`pgrep` empty). Where I give instruction counts they come from `/usr/bin/time -l`.

## Findings

### Q1 (453)

1. **`heroes test`'s `ffi_header_refused` message is false today, and route B would make `--emit-c` say the same false thing.** I ran it on the ffi-pragmatist's two-header case (copied into `200-critic2/fp/`).
   - `check` exits 0. `build` exits 0 and the binary prints 6 and 8. `--emit-c` exits 0.
   - `test` exits 1 with: *"`b.h`, the header this group names, does not compile: clang refuses it at line 1: conflicting types for 'twice'"*. Its note says *"repair the header, or name the one that declares this group's C"*.
   - Each header compiles on its own: `clang -std=gnu11 -Wall -fsyntax-only -x c a.h` and `b.h` both exit 0.
   - **Which header gets blamed depends only on the order of the `use` lines.** In `fp2/` I swapped them to `use right` / `use left`, and `test` then says *"`a.h` … does not compile"* at `left.hero:1:8`.

   So the class is "a false message", which is `blocking` under verification.md § Bounded discovery. The two seats disagree here, and this run settles it:
   - The compiler-engineer says "the contract allows exit 1 here".
   - The ffi-pragmatist says the case fits none of the six classes and needs a new diagnostic class, which means a full sitting under CLAUDE.md § 4.
   - `cli-surface.md:41` reads *"exit 1 the input has diagnostics"*. `check` and `build` say this input has none, and the one diagnostic `--emit-c` would print is the false one above. The ffi-pragmatist's side holds.
   - The spec says nothing about two groups binding one C name. `grep -n -i -E 'same (c )?name|two (extern )?groups|binds? the same'` matched only `:373`, which is about record fields.

2. **Route B's "cheaper" claim compares numbers in different units.**
   - `self-b-cold.log` reads 255,409,013,315 instructions; `self-b-warm.log` reads 254,883,657,783. Cold and warm are equal, so the fused clang compile is not in that count: `time -l` counts heroes alone, as the ffi-pragmatist showed (130 M instructions against 6.20 s of user time).
   - Route A's +6.0% is clang counted with `-fintegrated-cc1`. Route B's 442→255e9 is heroes' own work with clang left out.
   - What route B adds when the cache is cold shows only in durations: 45.10 s user cold against 24.15 s warm. Those were taken at a load between 3 and 88, so the project's rule discards them.
   - So: warm, B is cheaper in counts. Cold, it is unmeasured. The command that settles it is `time -l clang -fintegrated-cc1 <build words> -c` on `q1/self-b.c`, set against the per-module rounds of the old route.
   - The byte-identity claim holds: `cmp self-b.c self-new-oldbin.c` reports them identical.

3. **The fused compile already runs on every CI leg.** `.github/workflows/ci.yml:708` runs `test selfhost/main.hero` on the matrix, which includes `windows-latest` (`:283`). The compiler-engineer's "Windows: unrun" therefore applies only to the `--emit-c` unit, which differs from the test unit only by the test blocks. That is an inference.

### Q3 (470)

4. **Route C with the ABI left at 29 breaks the stamp's one job, catching version skew.**
   - I compiled the seat's route-C emission (`q3/c-200.c`) against the frozen ABI-29 `runtime/heroes_runtime.h`. clang exits 1 with `r200.c:12950:5: error: call to undeclared function 'hero_str_release_at'`, 2 errors. The stamp passes, so instead of *"heroes_runtime.h is from another compiler"* the author would get clang's error. I infer it would come out as the internal-error exit 2, because the error sits in the generated file; I did not run that through `heroes`.
   - The header's own rule (`heroes_runtime.h:19`) says the ABI *"moves whenever the declarations change shape"*. Precedent is an additive bump each time: `4cafbb05` went 12→13 for `hero_str_repeat` alone, and `bcdbf694` went 28→29 for three `hero_lend_*` functions.
   - **The seat's own notes contradict its report.** `notes.txt` at 20:06 says *"ABI left at 29 (additive; bump owed at landing)"*; the report says *"ABI left at 29 (the additions only add)"*.
   - A bump to 30 moves `selfhost/emit/decls.hero:103` and `:326` and every blessed emission (the `_Static_assert` line), not only the 287. That ABI-30 window is what broke 5 of the seat's 6 `emission` failures.

5. **Run-time cost on a real program, which I measured.** I built the compiler from the B+C source twice: once emitted the old way (the seed compiler) and once emitted with route C (by heroes-c). Then I ran `X check selfhost/main.hero` (`200-critic2/runc.out`, `runc2.out`).

   | level | old emission | route C | change |
   |---|---|---|---|
   | `-O0` (`build`'s default) | 72.591 / 72.543e9 | 73.347 / 73.316e9 | +1.0 to +1.1% |
   | `-O2` | 21.063 / 21.063e9 | 21.237 / 21.232e9 | +0.8% |

   All four checks exit 0. That is below the bench's +2.2 to 3.2%. Two gaps remain: records are not covered, because the record extension is unbuilt, and an array- or map-heavy hot loop is unmeasured.

6. **Route C works only because the runtime is a separate, opaque C unit.** The record case proves it. `T_release(&slot)` already takes the address, but it is inlined, so the escape disappears (47.9e9 at 200 returns; linear once marked `noinline` by hand). `grep -rln flto selfhost runtime` finds nothing today. If LTO or a unity build ever lands, route C silently stops working. That premise should be written down where the landing records route C.

### Q4 (472)

7. **Route D as built grows the seed by 28%.**
   - I took the frozen tree plus the seat's `body.hero` (D alone), built it, and ran `--emit-c` on `selfhost/main.hero`: 1,838,399 lines and 59,014,593 bytes, against the seed's 1,513,554 lines and 46,097,621 bytes. `#line` lines go from 531,526 to 856,357.
   - The reason is one `#line 1 "<file>"` per prologue line. On the small case, `dbg.c` has 307 lines and 57 `#line`; `dbg-d.c` has 334 and 84.
   - 59,014,593 bytes is 56.3 MiB. From memory of GitHub's documentation (I did not check it here), GitHub warns on a pushed file above 50 MiB.
   - The seat's prediction mentions prologue `#line` lines but not their size.
8. **Neither run of the `lines` suite tests where the prologue maps.** It reads 422/0 with heroes-d, and green on the trunk (the trunk's gate, which I did not re-run). So a landing needs a case that pins the new mapping.
9. **Q4's sanitizer worry.** In the measured case the prologue is declarations and `= {0}` stores only (`dbg-d.c:94-110`). The one fault that can land there is a stack overflow on the first touch of the frame, which would then name the recursing function's own `function` line. That is an inference, unrun. Two things are also unrun: `-O2` (whether sunk stores give lldb line-1 rows in the middle of a function, which is defect 335's shape) and the Windows debugger.

### Q5 (523)

10. **The deep-record exit 2, reproduced and bounded.**
    - The seat's `deepstr.hero` with my compiler: exit 2, *"clang died … (Segmentation fault: 11)"*, plus the note *"where that stack ends is the C compiler's limit, not this language's"*.
    - I generated the same chain at several depths (`200-critic2/q5/d<n>.hero`). Depths 33, 64, 128, 250, 500 and 750 all build and print `x`. Depths 875 and 1000 exit 2 with Segmentation fault: 11.
    - The same chain with an `i64` at the bottom builds at 7,000 and dies at 8,000 (`fixedbugs-140-records-a-thousand-deep-build.hero:11-13`). A single `str` field drops the survivable depth from about 7,000 to between 750 and 875.
    - With `memset` it builds at 1,000 (the seat's measurement; I did not re-run it).
    - Defect 170's exit-2 ruling was for a real C-compiler limit. Here the emitter's spelling sets the limit, so the note is false in substance. A correct program refused at exit 2 when the emitter can avoid it reads as `blocking`; that is my reading, not a ruling.
    - `suite_wholes.hero` does not match the `{0}` text (grep empty), so a `memset` spelling should not blind it textually. Unrun.

### Q2 (465)

11. **Killing route A's sentinel kills the runner.**
    - I copied the seat's runner and `rt-A`, started `runner-a 200000`, and after 0.5 s sent `kill -9` to its sentinel. The runner exited **141** (SIGPIPE).
    - Cause: no file in `runtime/` handles SIGPIPE (`grep SIGPIPE` is empty). The runner's next HOLD/FREE write goes to a pipe nobody reads, and the forked child's HOLD write before exec would die the same way.
    - The sentinel shows in `ps` under the runner's own name (`comm` read `./runner-a`), so a kill by name or of "a process's children" hits it. The seat itself killed one this way by mistake.
    - This is a soundness question the seats did not ask (§1.12: a guarantee that ends quietly is not one). The obvious repair, `fcntl(F_SETNOSIGPIPE)` on the write end with EPIPE restarting a sentinel, is unbuilt.
12. **The other owed items, none a veto as far as I can see.**
    - **Pid reuse:** covered on the ordinary path. FREE is written before `waitpid` (`run.c.A:724-726`), the pipe keeps order, and the sentinel drains the pipe before killing. There is one hole: when more than 1024 children are live (`:1271-1276`), the child is reaped with no FREE and then `hero_panic`, so the sentinel later SIGKILLs a stale pid and its group.
    - **Darwin guard:** the guard is `!defined(__linux__)` (diff lines 431, 541, 1232). On an unsupported BSD it is a compile error, not a silent one.
    - **`proc_pidinfo`:** it uses a static buffer and no malloc. That it is a plain syscall wrapper is from memory of xnu's libproc, not checked here.
13. **Copy-on-write memory held by the sentinel: unmeasured, and my instrument failed.** A runner with 512 MB written, then one launch, then the 512 MB rewritten: `footprint` gives the sentinel 752 KB, and `vmmap` shows its 512 MB `MALLOC_LARGE` at 0K resident. That per-task view cannot show pages the sentinel holds only through copy-on-write. A system-wide `vm_stat` read with 2 GB gave +308, +1882, +2320 and −483 MB across four runs, which is noise, so I discard it. Whether the sentinel pins the runner's pre-fork heap is still a question.

## Routes and questions nobody covered

- **Q1:** what `build` and `test` should do with the two-header program. Three options:
  - (a) `test` compiles per module as `build` does;
  - (b) the checker refuses two groups binding one C name with different signatures, which is a new class and needs a full sitting;
  - (c) `--emit-c` refuses with a true message naming both groups.

  The soundness lane cannot adopt (b) or a new message. Also unmeasured: route B's cold counts with clang included, and the ffi-pragmatist's Windows `raylib.h` plus `windows.h` prediction.
- **Q3:** a `noinline` (or out-of-unit) release for records, and its run-time cost on the compiler at `-O2`, since records are its commonest counted value. The ABI 30 bump and its effect on emissions. `layout` (`inst.hero` at 365 against 350) and `guarded_names`.
- **Q4:** keep D's effect but write the whole prologue on one physical line under a single `#line`. That costs about 2 lines per function (my regex counts 7,938 function definitions in the seed) instead of 324,845 lines. Unbuilt. Still unrun: `-O2` stepping and a `tests/golden/run/` ASan frame in housekeeping.
- **Q5:** where the `memset` version itself dies, and whether `memset` should apply to every counted slot or only past a depth (every slot moves every emission). Also, crash depths on the CI's clang 18 and the Windows clang.
- **Q2:**
  - a sentinel that is not a bare fork: `posix_spawn` with Darwin's `POSIX_SPAWN_CLOEXEC_DEFAULT`, which closes every descriptor and sidesteps fork-safety, at the cost of re-entering the runner's binary in a sentinel mode;
  - what the runner does when its sentinel is gone (restart, or a message);
  - FREE before the over-1024 reap.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/200-critic2/`:
- `fp/` and `fp2/`: the two-header case, in both `use` orders
- `c-200.c`: the route-C emission used for the ABI-29 test
- `q5/`: the deep-record cases (`d<n>.hero`, `o<n>.txt`)
- `q2/`: `runner-a`, `cow.c`
- `seed-d.c`: the seed emitted with route D alone
- `runc.out`, `runc2.out`: the `-O0` and `-O2` run-time counts
- `treec/`, `treed/`: the B+C and D-only trees
