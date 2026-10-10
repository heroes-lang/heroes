# Panel 207, ffi-pragmatist

Copied by the coordinator at 12:15 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, ffi-pragmatist

**verdict**: object. This is not a veto: route C leaves the C ABI alone and makes no binding harder.

**section**:
- design.md Part 6, the compile-time evaluation paragraph (`:3165-3170`): *a structural termination argument ..., never a quota*.
- design.md §4.19's constant paragraph: *its value is the header's*.
- §1.11.
- design.md does not cover a build that cannot run what it builds. Its only word on that is `:788`, *`zig cc` remains interesting for future cross-compilation*.

**experiment**: I worked 12:03 to 12:15 (`date`) in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-ffi-pragmatist/`. The compiler there was built from the seed, then from `selfhost/`, exit 0.

How the probe was built (`probe.sh`):
- Run `--emit-c`.
- Compile the unit again with `-Dmain=hero_program_main -g` and the 18 flags of `flags()`.
- Add a probe file that calls each accessor `h_<mod>_<NAME>(void)`, with a SIGABRT handler (`backtrace` and `dladdr`).
- Link it with the runtime object, run it under `timeout 2`, and cut the output at `head -c 4096`.

clang accepted every unit, on the Mac and on Linux arm64 (clang 22.1.8). On glibc the probe's own C needed `#define _GNU_SOURCE`.

| case | Mac | Linux arm64 |
|---|---|---|
| `loop` | 134; `atos` maps the frame to `loop.hero:5`, the step | 134; `addr2line` gives `loop.hero:5` only when linked `-rdynamic` |
| s06, s07, s15, s36, s02, s31, s29 (`INT64_MAX + 1`) | 134, with the runtime's own text | 134 |
| s20 (never read) | 134 | not run |
| g01 `PATH_MAX * 600000` | 0 (PATH_MAX is 1024) | **134 at line 4** (PATH_MAX is 4096), and the built program also aborts there |
| g03 (`while i < PATH_MAX`) | 0 | 0 |
| s24 | 124; says nothing about which constant | 124 |
| s16 (10^7 steps) | 0, 0.04 s | 0 |

Every refusal names the constant's line. Only the symbolized runs name the step's line.

**Cost**:
- Toy program: recompile 0.02 s, probe file 0.03 s, link 0.02 s, run 0.00 s, against a warm build of 0.13 s.
- The compiler's own 403 accessors, reusing its 588 objects: 0.04 + 0.04 + 0.15 s, then 0.42 s on the first run and 0.00 s on three repeats. Its cold build is 79.5 s.

**The bound flips its own verdict**: the 4×10^8-step probe was killed at 2 s on its first run. Five runs after that finished in 1.60 to 1.77 s, exit 0.

**Memory**: on this Mac, `ulimit -v` and `ulimit -d` both fail with *Invalid argument*. On Linux, `ulimit -v` makes `malloc` refuse.

**A build refusal is a compile error already**: a wrong `sqrt` binding gets exit 0 from `check` and `ffi_parameter_type` from `build`.

**argument**: Route C changes nothing that crosses the boundary: same objects, same accessors, no marshalling, no shim. It is also the only exact route for a group's constant: g01 is refused at its line on Linux and passes on the Mac, just as the two binaries behave. But its bound is a wall clock. One probe binary on one Mac was refused once and accepted five times, so the bound is a quota that cannot even repeat its answer, which is worse than the quota `:3165-3170` refuses. macOS gives no memory bound. Finally, a probe run on the host reads the host's header (1024 against 4096), so any future cross-build gets a wrong verdict, not just a missing one.

**prediction**:
- Under route C with a 2 s wall bound:
  - `examples/sqlite` and `examples/curl` pay no probe and need no shim: they have 0 written constants.
  - Of `examples/`' 21 files with an `extern` group, none has a written constant reading a group's constant (matched by name within each file).
  - g01 is refused at `g01-pathmax.hero:4` on Linux arm64 and builds on macOS.
  - s16b (4×10^8 steps) gets two different verdicts on this Mac, depending on whether its probe binary is a fresh one.

**condition**: I would approve if all four of these hold:
1. The bound does not depend on the clock.
2. Reaching the bound refuses nothing, and C4b's sentence says so.
3. C4b says a refusal over a group's constant is the target's.
4. The probe's value is never emitted back into the program (route G).

I would **veto** route G if it bakes the host's value of a group's constant into a build for another target.

**What I did not run**:
- Windows.
- A cross-target probe; panel 206's `_Static_assert` route under cross-compilation.
- `-O2`.
- A SIGALRM inside the probe to name a hung constant.
- An allocation cap in the runtime.
- The doubling-string shape.
- s16b on Linux.
- The probe implemented inside the compiler.
- Any suite, any census, any paid run.

No probe binary and no container was left running (`pgrep`, `docker ps`).

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-ffi-pragmatist/`:
- `notes.txt`
- `probe.sh`
- `selfprobe.sh`
- `cases/`
- `linux/` (`run.log`)
