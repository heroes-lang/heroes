- [x] **008 — On Windows the documented way in builds a compiler that cannot compile a medium program** | Date: 2026-09-04, found on the Windows box while measuring M-corpus-depth's two FFI programs on the three platforms | REPAIRED 2026-09-04, by panel 106's repair rather than by a change of its own | CLAUDE.md § Commands and `seed/README.md` both give the way in as one clang line; `.github/workflows/ci.yml:334-335` gives a different one on Windows | **Severity: the documented instruction is wrong on one of the three platforms, and the instrument that would have caught it is the thing working around it.**

  **The reproducer, on `apponfly-vps` at `ed88c02a`, clang 22.1.8, Windows Server 2025.** Build the compiler exactly as the contract says:

  ```
  clang -I runtime seed/heroes.c runtime/runtime.c -o heroes.exe
  ./heroes.exe check examples/query/main.hero
  ```

  answers `panic: stack exhausted` at **exit 127**. The same command on the same machine, with the flag CI passes:

  ```
  clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o stacked.exe
  ./stacked.exe check examples/query/main.hero
  ```

  is **exit 0**. Measured the same session: `heroes test selfhost/main.hero` is `panic: stack exhausted` with the first binary and the compiler's own tests with the second.

  **What it is not.** It is not the emitted program's stack — panel 058 ratified `/STACK:67108864` for what the compiler LINKS, and that is in `selfhost/cli/flags.hero` and works. It is the compiler's OWN process: `heroes.exe` built by the documented line gets Windows' default 1 MB thread stack, and this compiler's frames do not fit in it once a program is large enough to recurse a few dozen levels through the parser. Small and medium programs are fine — measured with the plain binary: `examples/gallery/00-first.hero`, `examples/roman/main.hero`, `examples/csv/main.hero` and `examples/json/main.hero` all check clean. `examples/query/main.hero` (1,487 lines, six modules) is where it stops.

  **It is Windows-only, measured on the other two the same day**: the identical command on the Mac and in the Linux image (`docs/environment/linux/Dockerfile`, x86-64) is exit 0 on `examples/query/main.hero`.

  **Why nothing caught it.** CI passes `-Wl,/STACK:67108864` at `ci.yml:334-335` — a line that exists nowhere in the contract, the seed's README or the ROADMAP's verify block. So the Windows leg is green **because it does not run the documented instruction**, and the documented instruction has been wrong on that platform for as long as the flag has been in CI. panel 104's stack guard is what turned it from a silent crash into a sentence, which is the only reason it is legible at all.

  **What is owed, and the order matters.** (1) The one clang line in CLAUDE.md § Commands and in `seed/README.md` gains the Windows arm, so the contract and CI say the same thing — that is the repair, and it is documentation because the defect is in an instruction. (2) A check that fires when they diverge again: the flag CI passes and the flag the contract prints are one string, read from one place, the way `selfhost/cli/flags.hero` is the single home of the compile flags. (3) **And panel 106's repair changes the numbers here** — it takes the interpreter's frames down 52% at `-O0`, so the size at which 1 MB stops being enough moves a long way out. Whoever lands `decref_place` re-measures this defect on the box afterwards and says whether the plain line still fails, because the answer decides whether (1) is a permanent arm or a note about a version.

  **The repair, and it is somebody else's.** Nothing in this entry's own list of what was owed was done. `docs/panel/106`'s adopted resolution — `decref_slot`, which deletes the exit sweep's load temporary — took the compiler's own frames from **6,563,376 to 4,227,600 bytes, −35.6%**, and the seed from 846,804 to **719,955 lines**. That moved the size at which Windows' default 1 MB thread stack stops being enough far enough out that the defect no longer reproduces.

  **Measured on the box at `dc677afc`, the SAME reproducer that failed at `ed88c02a`**, built with the documented line and nothing else — `clang -I runtime seed/heroes.c runtime/runtime.c -o plain.exe`:

  ```
  ./plain.exe check examples/query/main.hero        exit 0   (was 127, panic: stack exhausted)
  ./plain.exe check examples/interpreter/main.hero  exit 0
  ./plain.exe test selfhost/main.hero               560 tests, all passed  (was a panic)
  ./plain.exe test examples/nbody/main.hero         6 tests, all passed
  ./plain.exe test examples/spectral/main.hero      8 tests, all passed
  ```

  The compiler compiling **its own source** with the plain binary is the strongest margin available without inventing a bigger program: `selfhost/` is 178 modules and the deepest thing this repository can hand a parser.

  **This entry is closed and its question is NOT.** The defect is gone; the *divergence* that hid it is not. CI's Windows leg still passes `-Wl,/STACK:67108864` at `ci.yml:334-335`, a flag no document mentions, so the leg still does not run the instruction the contract prints — and the day the compiler's frames grow back, the contract's line breaks again and CI stays green again. That is filed in `docs/work/SCHEDULED.md` as its own item rather than left inside a closed defect, because a closed defect is not a place anybody looks.

  **And the honest reading of the timing**: this was found because the author started the box to measure two FFI programs, and repaired hours later by a sitting convened about frame size for a different reason. Neither half was aimed at the other. The thing that made the connection possible is that the defect entry named panel 106's repair as the thing that would move its numbers, and said to re-measure rather than assume — which is the only instruction in it that was followed.
