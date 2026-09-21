# Panel 174 — the two files the whole net shares

**Lane: SOUNDNESS** (`compiler-engineer`, `ffi-pragmatist`), plus the
completeness critic. Convened 2026-09-21 on defect 074, which had reproduced in
CI twice in three pushes.

**Resolution: `provisional — author ratification pending`.** Queued in
`docs/work/DECIDE.md` as `panel 174`.

---

## The proposal, verbatim as it went out

> `runtime/parts/run.c` opens the harness's two redirect files
> `FILE_SHARE_READ` and hands them to every child inheritable, and
> `tests/harness/shell.hero:206-207` gives every one of the net's spawns the
> **same two paths**. So one lingering process holding either handle makes every
> later spawn fail with `ERROR_SHARING_VIOLATION`. Choose the resolution.

Five routes were put: **A** widen the share mode, **B** one path per spawn,
**C** kill the process tree, **D** `PROC_THREAD_ATTRIBUTE_HANDLE_LIST`,
**E** bounded retry. The seats added **F** and **G**; the critic added **H**.

## What was settled before the seats sat

`ERROR_SHARING_VIOLATION` is not a hypothesis. The defect reproduced during
this sitting — GitHub run **35651518558**, Windows job **106504808513**, commit
`497a048f` — and the instrument repaired hours earlier printed the number:

```
./heroes-from-seed.exe could not be started, so it has no exit code;
the operating system's own reason is 32
```

**120 of them, one number, no other**
(`grep -ao "operating system's own reason is [0-9]*" | sort | uniq -c`).

## Verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to A alone; adopt **B + C**, B load-bearing. No veto | design.md §1.1's fourth force, on §1.12 | run.c 572→648; B ~18 code lines over 4 files, C 31 code lines in 1 | three consecutive Windows legs read 0 lines carrying `reason is 32`, and `wc -l runtime/parts/run.c` ≥ 640 | C's Windows half is **uncompiled** here; falsified by building it on the box |
| ffi-pragmatist | **object** to A as stated; **veto** its `FILE_SHARE_WRITE` bit; adopt **C + B + the `DELETE` bit alone** | design.md §1.12 :576-580, and *design.md does not cover process lifetime at all* | patch +69 / -10 code lines | no file under `selfhost/emit/` or `examples/` changes; a Windows leg reads 0 lines carrying `reason is 32` | the veto lifts if the orphan's write interval is bounded above the capture window |
| completeness critic | no verdict. **Falsified the compiler-engineer's C on POSIX**, settled the A-bit contradiction for the ffi seat, added route H, found five false brief premises | — | — | — | — |

## The disagreements, stated rather than smoothed

**1. The `FILE_SHARE_DELETE` bit. The ffi seat is right, and it is checkable.**
The compiler-engineer wrote *"once each file has exactly one writer, the share
mode stops mattering — I would not spend the three lines."* The critic re-ran
`p174-sharebits.exe` and measured what B does not cover:
`tests/harness/main.hero:404`'s `remove_tree(scratch)` still fails on a held
file, **`RemoveDirectory` refused, error 145**, and the harness then `exit(2)`s
*after* printing a green line. That is the 2026-08 failure recorded at
`tests/harness/shell.hero:414-417` — *"Windows will not delete a file a handle
holds, so the next run could not clear its build directory"* — and route B does
not reach it. **The DELETE bit is adopted. The WRITE bit stays vetoed.**

**2. The two seats' "route C" are different patches, and nobody said so.** The
ffi seat's Windows job is unconditional and its POSIX arm sweeps the group on
*ordinary* exit; the compiler-engineer's gates both on the watchdog and sweeps
only on *timeout*. "Adopt C" was ambiguous in a way that decides three platform
legs.

**3. And the critic falsified the compiler-engineer's C where it matters.**
Measured against that seat's own patched tree, through the real `hero_run_go`,
watchdog armed at 120 s as `shell.hero:214` arms it, child exiting **normally**:

```
head  case B's capture is 275 bytes (17 owed), NULs 83, another case's bytes: YES
ce    case B's capture is 300 bytes (17 owed), NULs 83, another case's bytes: YES
ffi   case B's capture is  17 bytes (17 owed), NULs  0, another case's bytes: no
```

Both of that seat's own probes arm a 2 s watchdog and make the child hang, so
**its instrument could not see the gap it left**.

## What the sitting changed its mind about, and the measurement that did it

**The watchdog did not create the orphan on the failing run, and it could not
have.** From job 106504808513's own timestamps:

```
unsupported   20:33:49
run           20:34:54      65 s for all 136 cases
annotations   20:34:54      the same second — 19 groups refused instantly
determinism   20:37:55
```

`WATCHDOG_SECONDS` is **120**, per process. The whole `run` suite took 65 s.
**No process can have reached the limit.** The shared brief's § 5 — the
watchdog terminating a `heroes.exe` and orphaning its `clang.exe` — is not
merely unrun for that run, it is **excluded**.

So route C does **not** close defect 074. It closes a different and real
defect, measured on Darwin at HEAD: `TerminateProcess`/`kill` answers 124 while
the descendant it was meant to stop is `STILL ALIVE`. Both are worth having and
they are not the same repair, and this sitting says so rather than letting one
wear the other's justification.

## The resolution adopted

**The most robust and complete, per CLAUDE.md § 4. Where robust and
conservative disagree it takes robust, and the conservative reading is recorded
below so the author can choose it.**

1. **B — one redirect path per spawn**, removed once its streams are read.
   **Load-bearing**: it closes 074 *whoever* the holder turns out to be, which
   matters precisely because the holder is still unidentified on the runner.
   Measured price: `hero_fs_remove` is **34.83 µs**, **0.037%** of one real
   `heroes build`; the longest path reaches **52 of 260** on Windows.

2. **F — `selfhost/cli/toolchain.hero:79` stops passing `out: ""` to clang.**
   That empty string means *inherit* (`runtime/parts/run.c:131-133`), so clang
   receives the harness's own capture handle. It is the mechanism behind the
   only holder this project has ever named. One word, at the source.

3. **A's `FILE_SHARE_DELETE` bit alone**, on the three redirect opens. The
   `FILE_SHARE_WRITE` bit is **vetoed**: it is the one that opens the
   interleaving, measured identically on Windows, Darwin and Linux — a capture
   of 150 bytes where 17 were owed, holding another case's output.

4. **C — kill the tree, in the ffi seat's form**: the Windows Job Object
   unconditional and verified *nested inside an existing job*, the POSIX group
   swept on ordinary exit and not only on timeout. **Recorded as closing the
   watchdog defect and NOT defect 074**, on the timestamps above. The POSIX
   `setpgid` stays guarded, because the naive form stops a terminal-reading
   child with SIGTTIN on both Darwin and Linux.

5. **G — `selfhost/cli/process.hero`'s `Ran` gains `why`**, so
   `toolchain.hero:99-104` stops printing *"clang is not on this machine's
   PATH"* for a sharing violation. It is the same false sentence this project
   repaired one layer up in `497a048f`, and not looking for its siblings is the
   shape CLAUDE.md § RUN IT names.

6. **D is refused with a measurement**, not an opinion: the handle reaches the
   child with `HANDLE_FLAG_INHERIT` set and travels on to the grandchild, and
   excluding it produces **0-byte captures silently** rather than failing loud.

7. **E is refused.** It tolerates rather than removes, and its policy rests on
   a holder nobody has identified.

**What conservative would have been**: B and F alone, leaving the DELETE bit
and route C to the milestone that can measure them on the runner. It is
narrower and it leaves `remove_tree` failing after a green line.

## Route H, considered and deferred with its price

The critic's: capture through a **pipe the parent owns**, so no child ever
opens a file. Measured on Darwin to remove the sharing violation, the
interleaving, and — free, through SIGPIPE — the orphan itself. **Deferred**
because it deadlocks above the pipe buffer: 32 KB passes, 1 MB hangs, and the
largest expectation already in the tree is **38,145 bytes**. It needs a
concurrent drain, which is a design this sitting did not price. Recorded here
so it is chosen rather than forgotten.

## The question the sitting should have asked and did not

**Should `HERO_OS_FAILED` produce a case verdict at all?** The failing run
emitted 130 verdicts about 130 programs that never ran. A machine that cannot
start a process has not told us anything about the corpus, and a suite that
reports `0 failed` in that state would be lying in the other direction. Filed
rather than answered.

## Five brief premises the critic checked, and what survived

The coordinator wrote the shared brief and nothing checks a brief against the
world. Four of the five stand as errors; **one accusation is itself wrong** and
is corrected here rather than accepted:

- **"the 10 groups that suite reports on"** — the critic measured 19 and called
  the brief wrong. **The brief is right and the critic counted the wrong
  population.** Re-measured by the coordinator: 19 directories are walked, and
  ten carry `#~` markers (`fixedbugs` 33, `applyx` 1, `calledforeign` 3,
  `cross` 1, `externroute` 2, `externsignature` 2, `handlearming` 1,
  `localtakesamodule` 1, `qualifyptr` 2, `qualifytypo` 1). `deep` and
  `deepthread` carry **zero**, so they report nothing and cannot fail. The five
  failures are positions **1 to 5 of the ten that report**, the five passes are
  6 to 10, and 164 + 10 = 174 closes the arithmetic. **§ 2's contiguous prefix
  stands.**
- **"eighteen `abort-*` cases"** — it is **14**. Decorative, and wrong.
- **"the same HEAD" on the author's box** — the box's git HEAD was `e6202a6e`
  with `497a048f`'s files applied uncommitted. Substantively true, literally
  false, and the literal form is what a later reader would trust.
- **"the only named holder anybody has measured"** — true, and the
  configuration it was measured in **no longer exists**: 2026-08-31, no Windows
  watchdog, and a path that stopped existing on 2026-09-03.
- **the instrument the coordinator suggested for counting the liars**
  (`grep -c '\.code'`) measures the wrong thing since `497a048f`, because
  `insisted()` now refuses instead of returning `started: false`. The real
  count is **457 `is_err()` branches, 19 of 269 `keep_failure` sites carrying
  an error value** — a milestone of its own, owed separately.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | three consecutive Windows legs read **0** lines carrying `reason is 32`, and `harness:` reads `0 failed` | the close of the milestone landing B |
| compiler-engineer | the `orphan` probe prints `gone` on Darwin, where it prints `STILL ALIVE` at `497a048f` | the same close |
| compiler-engineer | `wc -l runtime/parts/run.c` ≥ **640** | the same close |
| ffi-pragmatist | `git diff --stat` on the landing commit names **no** file under `selfhost/emit/` or `examples/` | the landing commit |
| ffi-pragmatist | with C and the DELETE bit, a Windows leg reads **0** lines carrying `reason is 32` | the first Windows leg after landing |

## Author's verdict

**RATIFIED 2026-09-21**, in the author's own words: *I have read the panel, so
I decide to ratify.* **Recorded as a reading of this file**, which is CLAUDE.md
§ 4's default and which the author has now stated three times; not `by
delegation`.

**The robust resolution stands as adopted.** The author named no preference for
the conservative reading, and CLAUDE.md § 4 requires the most robust and
complete one where the two disagree, so all six clauses land: B, F, the
`FILE_SHARE_DELETE` bit alone, C in the ffi seat's form, G, and the refusals of
D and E.

**The ffi seat's veto stands with it**: `FILE_SHARE_WRITE` does not land in any
form, and a ratification cannot lift a veto — only the measurement named under
*What a veto would compel* can.

**What a yes settles:** that the net stops sharing two redirect files between
every process it starts (B), that clang stops inheriting the harness's own
capture handle (F), that `FILE_SHARE_DELETE` lands and `FILE_SHARE_WRITE` never
does, that the process tree is killed in the ffi seat's form (C), and that
`Ran` gains a `why` (G).

**What a yes does NOT settle**, and each is named rather than bundled:

- **What holds the handle on the GitHub runner.** Route B closes the defect
  without knowing, which is why it is load-bearing — but the holder stays
  unidentified, and the cheapest instrument that would name it is a
  `Get-CimInstance Win32_Process` dump taken at the first error 32.
- **Route H**, the pipe the parent owns, which is deferred with its price and
  not refused.
- **The 457 `is_err()` branches** that discard an error value, of which only 19
  of 269 `keep_failure` sites carry one. A milestone of its own.
- **Whether a machine that cannot start a process should produce a case verdict
  at all.** The failing run emitted 130 verdicts about 130 programs that never
  ran.

**If the author prefers conservative**, the narrower reading is in the
resolution above: B and F alone. Saying so overturns nothing already landed,
because nothing has landed yet.

## What a veto would compel

The ffi seat's veto is narrow and it binds: the `FILE_SHARE_WRITE` bit does not
land in any form. It lifts only on a measurement bounding the orphan's write
interval above the capture window, which nobody has taken.
