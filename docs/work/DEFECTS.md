# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item here is a **measured** failure of
the compiler on a program — a crash, a wrong answer at exit 0, a silence where
a message is owed — with its reproducer, its cause where known, and what is
owed. It exists because the author said so on 2026-09-03: *"I do not like the
defect directory … if anything is still open in defect at the end, make one
single file called DEFECTS.md inside work, so that everything is tidy"*.
Seven files under `docs/defects/` became this list and six entries in
`docs/work/DONE.md` that evening; the directory is gone.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md` — the record — as every other list in this directory does
(CLAUDE.md §3). A defect that stays here after its fix is the shape §3 was
amended to prevent.

**One notation: `- [ ]`.** A finding written as a bare bullet is invisible to
every count in this project. The body of an entry is indented under its line;
it may be long, because a defect's reproducer, cause and measurements are the
entry, not decoration.

Format: `- [ ] **NNN — <title>** | <date, found by> | <status> | <where it came from> | <severity>`, then the body.

- [ ] **008 — On Windows the documented way in builds a compiler that cannot compile a medium program** | Date: 2026-09-04, found on the Windows box while measuring M-corpus-depth's two FFI programs on the three platforms | OPEN, cause measured, repair not written | CLAUDE.md § Commands and `seed/README.md` both give the way in as one clang line; `.github/workflows/ci.yml:334-335` gives a different one on Windows | **Severity: the documented instruction is wrong on one of the three platforms, and the instrument that would have caught it is the thing working around it.**

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
