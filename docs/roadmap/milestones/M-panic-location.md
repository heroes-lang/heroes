# M-panic-location — a panic names its line


**Scheduled by author instruction 2026-09-03**, the cheapest half of what the
author called *"a strong runtime"*, and its warrant is **§1.12**: a program that
stops must say where.

**Measured 2026-09-03.** `hero_panic` flushes stdout, prints `panic: <msg>` and
calls `abort()` (`runtime/parts/panic.c:21-25`); an out-of-range index, an
overflow, a `.must()` on an error and a slice that splits a character all funnel
through it, and none names a `.hero` file, a line or a function. The one abort
that does is the stack guard: `runtime/parts/stack.c:202-213` walks back with
`dladdr` to the first Heroes frame on POSIX, and `stack.c:292` says Windows names
the failure and not the function until dbghelp and a PDB are measured on the box.
The emitted C carries `#line` (CLAUDE.md §7), so `__FILE__` and `__LINE__` at
every runtime call that can abort already resolve to the `.hero` position — the
information is in the binary and the runtime is not told.

**What it delivers.** Every panic names the `.hero` file, the line and the Heroes
function, on the three platforms, measured on each before its commit (CLAUDE.md
§ Commands); `HERO_RUNTIME_ABI` +1 with the two-phase edit (`seed/README.md`); a
`fixedbugs` case per abort class (CLAUDE.md §9). **Soundness lane**: no surface,
no diagnostic class, no spec token. What it must not do is slow a program's happy
path or the compiler — the location is passed, never computed, and the corpus
leg's time before and after is the measurement.

**Why here, and why it may move.** It touches the runtime, which
M-isolated-threads holds until it closes; nothing else depends on it, so it may
be taken the day the threads land. Its witnesses are the corpus under
`--sanitize` and Part 11's metric 4, whose turns-to-green a panic that names its
line shortens.

**Widened and corrected 2026-10-14.** The row keeps 71 and widens to what a
failure shows: an `assert` on a record shows both sides rather than the
expression alone, as spec § 12 says today, and a panic names the file, the
line and the Heroes function, from the runtime's type descriptors and the
formatter M-typed-inspection wrote. It touches spec § 12, so it is a sitting.
Facts above that have moved: `hero_panic` is at `runtime/parts/panic.c:39-43`;
M-isolated-threads closed on 2026-09-06; and the premise that `__FILE__` and
`__LINE__` already resolve to the `.hero` line at every abort is false for
`.must()` and `assert`, which `selfhost/emit/abort.hero` points at the
generated file on purpose.
