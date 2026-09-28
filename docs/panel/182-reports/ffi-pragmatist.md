*Written out by the coordinator from the seat's final message on 2026-09-28 at
01:59: the harness refused the seat's own Write. The text below is the
seat's, unchanged.*

# Panel 182, ffi-pragmatist report

**The report file was not written.** The harness refused the `Write` to `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/ffi-pragmatist/report.md`, saying a subagent returns its findings as text. So nothing reached `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/182-reports/ffi-pragmatist.md` either. The report is below; the coordinator can place it there. I wrote nothing in the trunk. All three of my background jobs have exited.

All work is in `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/ffi-pragmatist/`, extracted from `0fc98107`, with the compiler built from its seed. Clang is Apple clang 21.0.0 on Darwin arm64. I used no seat's prototype: the routes are C I derived by script from today's emission, as the brief asks.

## Verdict block

- **verdict**:
  - **Approve** route (a) limited to values, plus routes (b) and (d), on the FFI side.
  - **Object** to route (c) as stated, and to route (a) reaching slots without an IR check.
  - **No veto.** No route changes the ABI, `heroes_runtime.h`, what C sees at a call, or what a binding has to say.
- **section**: design.md §1.11, §4.19, §1.12 and §4.20. The controlling decision is panel 021 R3, which is outside design.md. design.md does not cover one half of my objection: a release bug the compiler introduces, in a program with an `extern`, shows up looking like a binding bug. §1.12 is the closest text and it does not say this.
- **experiment**:
  - I wrote the C each route implies for `h_keywords_keyword` and compiled it at `-O0` and `-O2`. Flag sets: the brief's, the project's sixteen, `-Weverything`, and ASan+UBSan.
  - The harness made 22 million calls per build.
  - Clang accepted all eight routes with **0 diagnostics**. ASan and UBSan were silent on all 16 sanitizer builds, the leak gate passed, and all 16 gave one checksum (`b9f9bbaea55e4800`).
- **argument** (117 words): No route changes what C sees, measured: across 119 FFI goldens, 436 extern calls and 25 out-cell addresses, no zeroed local reaches C, and the one cell C may leave unwritten is nulled by an IR store, not by `= {0}`. None touches `heroes_runtime.h`. The real cost is the instrument. Under the project's own flags clang refuses an unzeroed value read by value, and says nothing when an aggregate is released through its address. That is the exit sweep's shape, and 273 slots in the seed are touched only that way. When that path fires, the panic names the program's `extern` marks first. Route (c) as stated is exactly that, the second time an arm runs in a loop. Values are safe: the verifier already proves them.
- **prediction**: at M-agreed-retention's close, whichever of (a), (b) or (d) has landed:
  1. `git diff 0fc98107 <close> -- runtime/heroes_runtime.h` changes no declaration, and `HERO_RUNTIME_ABI` still reads 26.
  2. None of the 119 FFI `run` goldens' `.expected` files is modified, and nothing under `examples/sqlite/` is modified.
  3. `run` is green on all 119.

  SQLite, rung 3 of §4.19's ladder, needs no shim and no binding edit. Any one of the three failing falsifies the prediction. If no route lands, it is void.
- **condition**:
  - **Veto** if a route's emission makes an extern argument, an address handed to C, or a thunk local depend on the prologue's zeroing. Re-running `work/boundary.py` on the route's emission would show a count above 0. Also veto any changed declaration in the runtime header.
  - **Approve (c)** if the release at the join is followed by a re-zero, or the arm's next store is lowered as an initialisation with no read of the old value, and a loop-with-`match` golden lent to C is green under `--sanitize`.
  - **Approve (a) on slots** once the verifier checks panel 021 R3's rule, *"every slot is stored before it is loaded on every path"*, extended to slot releases, with a negative golden that shows the check firing.
  - **Withdraw approval of (a) on values** if `c-frees-a-lease-through-a-callback` hangs on the route's real emission and not on today's.

## What I measured

**Timing.** Minimum ns per call over five interleaved rounds. The load average rose from 2.07 to 7.60 during the runs because other seats share the machine.

| route | `-O0` | `-O2` |
|---|---|---|
| today | 414.4 | 68.1 |
| a | 199.7 | 34.6 |
| b1 | 332.3 | 52.7 |
| b2 | 233.3 | 26.4 |
| c | 407.9 | 64.0 |
| d2 | 86.2 | 25.2 |

- `-O2` does not remove the dead zeroing. The 111 `memset` calls disappear, but 86 vector zero stores remain.
- Route (c) buys 1 to 7 percent.

**The boundary**, over 119 goldens:

| what | count | zeroed |
|---|---|---|
| extern call sites | 436 | |
| local arguments at them | 420 | 0 |
| addresses handed to C | 25 | 0 (all written before the call) |
| callback thunks | 8 | 0 zeroed locals |

- The `owned` out-cell that C may leave unwritten is set to NULL by an explicit store just before the call, not by the prologue.
- My copy of the runtime header is byte-identical to the frozen commit's.

**Where clang goes blind.**
- Under the project's own flags, clang refuses 4 of the 5 unwritten-value shapes I probed. It is silent on `release(&aggregate)`, even under `-Weverything`.
- On the real function, dropping `h25_own25`'s zeroing is a hard error at `keywords.hero:170`.
- In the address-only version of that omission, clang says nothing. At run time it freed the caller's input string, then panicked with a message that names C and the `extern` marks first (exit 134). ASan reported a use-after-free.

**Why values are safe.** The verifier runs after the ownership pass and counts increfs and decrefs of a value as uses (`ir/uses.hero:36-41`), so a value's zeroing is provably dead. Panel 021 R3's own rider also says zeroing a value turns a compile error into a silent `""`.

**Slots: an open question.** I did not find the IR check panel 021 R3 asked for. I searched `selfhost/` and `runtime/` for its wording and found only the runtime's NULL-slot panics. Whether it exists under another name is a question, not a premise.

**Corpus differential.** Route (a) on values, applied to all 119 FFI goldens and built with one command line:
- 884 of 1,118 zeroings removed; clang refused 0 cases.
- 118 were identical.
- 1 hang is unattributed: 1 of 61 runs on the route, 0 of 61 on today's. It happened under high load, and the other 60 runs had identical stderr.

**The ladder.**
- SQLite runs identically to today and matches `main.expected`.
- curl, raylib and SDL compile and link with 0 diagnostics. I did not run them, because they need a network or a window.

**Route (c) in a loop.** A `match` in a `while`, with each arm's value lent to `puts`:
- Clang reports 0 diagnostics.
- The fourth iteration is a `heap-use-after-free` (exit 134), and the panic points first at the `lent` mark.
- `cz` (re-zero after the release) is clean.

**Windows.** A run of the FFI `run` cases is owed for whichever route lands. I ran nothing there; that the build-time check behaves the same on that clang is an inference.

## Files

All in `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/ffi-pragmatist/work/`:
- `routes.py`: generates each route's C
- `fn_*.c`, `tu_*.c`: the routes
- `bench.c`: the harness
- `time.out`, `asan.out`: raw timing and sanitizer results
- `probe/uninit.c`, `fn_wrong_copy.c`, `fn_wrong_addr.c`, `alt.c`: the negative controls
- `boundary.py`: the boundary scan
- `ffidiff.sh`, `ffidiff.out`: the corpus differential
- `loop/`: the route (c) loop test
- `sq/`, `rungs/`: the ladder
- `project.flags`: the project's sixteen flags
