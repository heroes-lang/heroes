# Panel 195, completeness critic, second pass (after the seats)

Started 13:10:58 (`date`). No verdict. Read: both seats' reports, the three
`p195-ce-*.diff`, `static_constant.hero` at the path the coordinator named.
Copy: `<scratchpad>/195-critic/ce/`, `git archive 96f3a588 | tar -x`, the seed
over it; the compiler-engineer's eight files reproduced there by `diff -u`
against its copy (read only) and `patch` (`diff -rq` against its copy: none
left). `96f3a588` and `23eb1169` differ in no file under `runtime/`,
`selfhost/` or `tests/harness/` (`git diff --name-only ... | wc -l` = 0), so
`195-critic/heroes` (built from `23eb1169` at 11:52) is the branch compiler.
Instruction counts only, no timing.

## Written 13:16:37 — the route rebuilt here, and (3) a release too many on a constant, three routes side by side

`ce/heroes-stage1` built 13:12:38-13:14:27 by `195-critic/heroes` with the
header held at 27 (the compiler-engineer's own procedure), then 28 restored;
`diff -rq` of `ce/runtime` against its copy: none. Its `--emit-c` of `k.hero`:
`_Static_assert(HERO_RUNTIME_ABI == 28 ...)`, `HERO_ARRAY_STATIC(hero_constant_h_k_K_33,
int64_t, &hero_desc_int, 32, INT64_C(3), ...)`, `return HERO_ARRAY_LIT(...)`.

**One release too many on a constant's read, the same sequence under three
routes** (`195-critic/second/{landed,static,onceheld}_over.c`, hand-written: reader 1 reads
`K[0]`, releases once too many, reader 2 binds `K` and writes `xs[0] @ 99`
through `hero_array_set` (the emitted place store), reader 3 reads `K[0]`; the
`onceheld` cell is the ffi seat's `x/gen.py` text in shape; `flags()` -O0):

| route | plain | `-fsanitize=address,undefined` |
|---|---|---|
| landed (`f7576a01`), runtime 27 | `panic: array with no element descriptor — a compiler bug`, exit 134 | `heap-use-after-free ... array.c:84 in hero_array_decref`, exit 134 |
| static block (the compiler-engineer's), runtime 28 | `r3 K[0] = 3`, leak check passed, **exit 0** | the same, **exit 0** |
| `onceheld` (the ffi seat's), runtime 27 | **`r3 K[0] = 99   (the constant says 3)`**, leak check passed, **exit 0** | prints **99** first, then `heap-use-after-free ... array.c:84`, exit 134 |

So the answer to the coordinator's question is yes: under `onceheld` a release
too many takes the shared block's count to 1 while another reader holds it, so
that reader's `cow.c` test reads 1 and writes **the process-wide constant in
place**. Every later reader, on any thread, sees the changed value. The plain
build exits 0 and passes the leak check. ASan names it only at the next release,
after the wrong value has already been printed. The landed route confines the
same bug to one read's copy, and it is loud plain (with a misleading message:
the freed block was reused). The static route makes it harmless and invisible.
**No route either seat built makes a release too many on a constant both
harmless and loud.** That is the point the ffi seat's "approve only if a release
too many stays loud" turns on.

## Written 13:24:23 — (1) the compiler-engineer's route against the ffi seat's stated veto conditions

The ffi seat's veto row (ir12's block "as written") says: *lifted to object if
the block is written through a runtime macro whose members are `h`/`b`, its
name added to `NAMES`, and the stamp moved 27 → 28 because a declaration was
added; still object, not approve, while the release-too-many stays invisible*.
Checked against the route as rebuilt here:

| condition | the compiler-engineer's route | evidence (run here) |
|---|---|---|
| through a runtime macro | yes: `HERO_ARRAY_STATIC` / `_EMPTY` / `HERO_ARRAY_LIT` in `heroes_runtime.h` | `--emit-c` of `k.hero`: `HERO_ARRAY_STATIC(hero_constant_h_k_K_33, int64_t, &hero_desc_int, 32, ...)` |
| members `h`/`b` | yes (`HeroArrayHeader h; type b[n];`) | header diff; ffi's `hostile.h` (`#define e 3`, `b 4`, `h 5`) built by `ce/heroes-stage1`: `4843751`, exit 0 |
| name in `NAMES` and the guard files | yes, four words | `guarded_names.hero:22` and `:116`; `grep -c 'push_macro("<w>")'` / `pop_macro` = 1/1 for `HERO_ARRAY_STATIC`, `HERO_ARRAY_STATIC_EMPTY`, `HERO_ARRAY_LIT`, `type`, `h`, `b` |
| stamp 27 → 28 | yes | skew below, refused both ways on four platforms |
| a release too many stays invisible | **yes, still invisible** | `static_over.c`: exit 0 and the right value, plain and under ASan, on Darwin, Linux arm64, Linux x86-64; plain on Windows |

So by the ffi seat's own words, the route the compiler-engineer built is
**object**, not veto and not approve. Every lifting condition is met and the
last one is not. The ffi seat's veto *reasons* (a silent SIGBUS on skew, a
member `e` a header can break) are both closed by the built route on every
platform run (below).

## Written 13:24:23 — (2) the route on the platforms (`195-critic/second/leg/`)

Mac-emitted C (`ce/heroes-stage1 --emit-c`), compiled by hand under `flags()`
with the runtime beside it (`leg.sh`, `win.sh`, sequential). Outputs are in
`out-darwin/`, `out-linux-arm64/`, `out-linux-x86/` and, on the box, `/c/w/p195-critic-32165/out-win/`.

| | Darwin arm64 (Apple clang 21) | Linux arm64 (Debian clang 22.1.8) | Linux x86-64 (`heroes-linux`, emulated, clang 22.1.8) | Windows x86-64 (clang 23.1.1) |
|---|---|---|---|---|
| `k`, `ks`, `spawnk` route, -O0 | 4843750 / 5093750 / 1453125, exit 0 | same | same | `k` 4843750, exit 0 (only `k`, light) |
| `k` -O2; ASan+UBSan on all three | exit 0, clean | exit 0, clean | exit 0, clean | unrun (light) |
| TSan, `spawnk` | (the compiler-engineer: 0 warnings) | exit 0, **0 warnings** | **unrun**: `FATAL: ThreadSanitizer: memory layout is incompatible, even though ASLR is disabled` (emulation; also with `setarch -R`, seccomp unconfined) | unrun |
| `[Variant]` holding `str` and `[i64]` payloads, `[[i64]]` with `[]` (`laidv.hero`) | 8499 / 4 / 5, ASan clean | same, ASan clean | same, ASan clean | same, exit 0 |
| where the block lands | (ffi: `__DATA_CONST,__const`) | `.data.rel.ro` (`objdump -t`) | `.data.rel.ro` | (ffi: `.rdata`) |
| new C vs old runtime | `'27 == 28'` error | same | same | same |
| old C vs new runtime | `'28 == 27'` error | same | same | same |
| new C, stamp edited to 27, vs old runtime | `unexpected type name 'int64_t'` (the macro is absent) | same | same | unrun |
| the compiler itself, `HEROES_RUNTIME=runtime.orig ce/heroes-stage1 build k.hero` | `internal error: compiling the generated C failed: ... '27 == 28'`, no binary | — | — | — |
| `ro.c` (count -1, `static const`), old runtime | SIGBUS 138, empty | SIGSEGV 139, empty | SIGSEGV 139, empty | (ffi: AV 139) |
| `ro.c`, new runtime | exit 0 both entry points | same | same | same |
| **`dbl.c` 32 elements**, new runtime | **`panic: an array released after its last reference`**, 134 | **exit 0, silent**: "second decref returned", leak check passed | **exit 0, silent** | runtime message `exception 0xC0000374, the heap manager's report`, 127, **after** "leak check passed" |
| **`dbl.c` 4 elements**, new runtime | exit 0, silent | exit 0, silent | exit 0, silent | 0xC0000374, 127, after the leak check |
| `dbl.c`, old runtime | 133 (SIGTRAP) at 32, silent at 4 | silent at both | silent at both | unrun |

**So the guard names a heap double release on one platform, at one size.** The
freed word is a non-negative pointer under glibc's tcache (`0xaaaaf4fac`,
`0x555555572`) and under the Windows heap (`0x1c73875cb80`), so it passes
`seen < 1` and the guard is never reached. The compiler-engineer's *"a heap
double release becomes a named panic where malloc trapped"* is true where it
was measured (Darwin). Its *"a double release is louder than today, not
quieter"* (its verdict's §1.12 line) holds on Darwin at 32 elements and is
**unchanged** elsewhere: no platform gets quieter, and only one gets louder.

## Written 13:24:46 — (3) continued: can a release too many on a static constant be loud at all? Built: yes

Neither seat built a route that does it. One exists, and a probe of it was built
here, **not as a proposal**. `leg/rt-bal/`: the compiler-engineer's runtime plus
a `_Thread_local int64_t hero_static_held`. A count of -1 at `incref` adds one;
at `decref` it panics if the count is already 0, otherwise it subtracts one.
The constant's function increfs the static block it returns (`bal/*-bal.c`,
one `sed` on the route's C), so every read is balanced by its caller's release.
Values never cross threads (`spawn.c`'s header), so each thread's count covers
its own reads. On this Mac:

- `k-bal`, `ks-bal`, `spawnk-bal`: 4843750, 5093750, 1453125, exit 0 (no false alarm).
- `static_over_bal` (the release too many): `panic: a constant's array released after its last reference — a compiler bug, please report it`, **exit 134, plain**.
- Cost, instructions, `k`: **225,421,856** at -O0 and **100,952,876** at -O2,
  against the route's 148,565,163 and 55,364,897 (+52% / +82%; 1.94x / 2.61x
  the floor). That is one runtime call per read plus a thread-local access,
  which is a call on Darwin.

Limits, stated: the count is per thread, not per block, so one release too many
and one missing release on two different constants cancel. Nothing checks the
count at thread exit or at the leak check. Unbuilt: the form that would cost a
plain build nothing, the count compiled only into `--sanitize` builds, where
ASan already pays far more.

## Written 13:24:46 — (4) the seats' instruction counts against each other

Same programs: `k.hero` and `local.hero` are byte-identical in both seats' copies
and the brief's (`cmp`). The two `ks.hero` **differ** (different strings), so
the `[str]` rows are not comparable across seats.

| program | compiler-engineer | ffi-pragmatist | here |
|---|---|---|---|
| `k` landed -O0 | 4,596,876,585 (re-runs 4,596.0-4,597.1M) | 4,600,830,901 / 4,597,626,304 | 4,599,763,643; 4,596.8-4,597.0M |
| `k` landed -O2 | 2,577,431,397 | 2,578,634,156 / 2,577,642,655 | 2,579,516,710; 2,577.1-2,578.3M |
| `local` -O0 | 116,542,863 (115.86-116.74M) | 116,605,066 | 116,461,213 |
| `local` -O2 | 38,382,216 | **39,062,990** (+1.8%) | 38,388,381 / 38,624,664 |
| static block, `k` -O0 | **147.9-148.7M** (emitted, its guard) | **141,684,878** (hand C, ir12's guard) | 148,565,163 (rebuilt route) |
| static block, `k` -O2 | 54.8-55.4M | 52,449,198 | 55,364,897 |

**The 6-7 million gap between the seats' static-block rows is the guard, not
the emission.** The ffi seat's own `v/k_sconst.c` (`195-critic/second/measure/`),
compiled against ir12's guard (`x/rtg`) and against the compiler-engineer's
runtime with its stamp line edited to 27, three runs each:

    ir12 guard:     141,517,284 · 140,985,257 · 141,016,817 (-O0)   52,443,391 · 51,884,501 · 52,629,592 (-O2)
    CE guard:       148,528,905 · 147,931,274 · 147,904,549 (-O0)   55,344,796 · 54,876,544 · 54,885,179 (-O2)

So two claims in the compiler-engineer's report are falsified by its own number
reproduced on the other seat's C:

- *"the emitted route reads 23 million more at -O0: the read is still a call
  and its owned result still goes through the slot's load, store and `decref`"*.
  The ffi seat's hand C keeps that same call and slot, and reads the
  compiler-engineer's number once it is linked with the compiler-engineer's runtime.
- *"The panic for counts below 1 sits on the cold side of that one compare, so
  the louder double release costs nothing over ir12's guard."* For a
  **constant's** block the `seen < 1` side is the hot side: a second compare
  on every static incref and decref, **+6.9M at -O0 (+4.9%) and +2.6M at -O2
  (+5%)** per million reads. It does cost nothing for heap blocks.

The guard's price on the landed `k` (`k_landed.c`, the same C against three
runtimes, three runs each): original 4,596.8-4,597.0M; ir12's 4,611.4-4,622.6M
(+0.3-0.6%); the compiler-engineer's 4,630.5-4,631.5M (**+0.73%**) at -O0. At
-O2: original 2,577.1-2,578.3M; ir12's 2,583.2-2,583.5M (+0.22%); the
compiler-engineer's **2,562.0-2,562.4M (-0.6%)**. The seats' "+0.41%" (ffi,
ir12's guard, `k`) and "+2.9%" (compiler-engineer, its guard, `shares.hero`)
are different guards on different programs, not a contradiction.

The element route: the ffi seat's `v/k_elems.c` on this Mac reads 79,428,127 /
23,329,386 (0.68x / 0.61x the floor), so it reproduces. The compiler-engineer's
"the gain over this route is 1.28x -> ~1x" is **falsified** at 0.68x. But
`gen.py` shows where the gain comes from: the element route replaces
`hero_array_at(t, i)` (a runtime call) with an inline bounds-checked C index.
The floor still makes that call, so most of "below the floor" is an inlined
`hero_array_at`, which every array read could have. It is not owed to constants.

*Measured before 13:25:32 (`date` read after writing), replacing the inference in the paragraph above*: the floor
`local.hero` emitted by `195-critic/heroes`, its one `hero_array_at(t8, t11)`
replaced by an inline bounds-checked index (`measure/local_inline.c`), against
the original runtime: **85,461,725 at -O0 and 25,353,715 at -O2** (the floor
was 116.5M / 38.4M; the element route 79.4M / 23.3M). So 31M of the element
route's 37M below the floor at -O0 (84%), and 13.0M of 15.1M at -O2 (86%), is
the inlined `hero_array_at`, which any array read could have. Measured against
an inlined floor, the element route is 0.93x / 0.92x.

## Written 13:26:00 — (5) what both seats missed, contradictions, and the question not asked

**A correction to my first pass.** My route 3 said a writable static block
"turns a missed guard into a count decrement instead of SIGBUS". The ffi seat
measured this false, and I re-ran it here (`measure/rw.c`: `ro.c` with `static`
for `static const`, `(__DATA,__data)`, the original runtime): `panic: the
process is dying of SIGABRT`, exit 134. The decref reads -1, takes it as
doomed and hands it to `free()`. **That line of my first pass is withdrawn.**

**Shapes run here that neither seat listed** (`second/shapes/`, `ce/heroes-stage1`
against `195-critic/heroes`, same output on both):

- `[3, -1, 4]`, `[u64]` holding 18446744073709551615, `[f32]`, `[[str]]` with an
  inner `[]`: all laid out (`grep HERO_ARRAY_STATIC`). Output 6,
  18446744073709551615, 1.25, c on both routes.
- A **70,000-element** `[i64]` constant (one variadic macro of 70,004
  arguments): builds and prints 70993 on both routes, Darwin. Other platforms unrun.
- `[ptr]`: refused by both routes (`unsupported[pointer_element]`), not a shape.
- `[Variant]` with `str` and `[i64]` payloads plus `[[i64]]` with `[]`
  (`laidv.hero`): laid out (7 blocks), 8499 / 4 / 5 on all four platforms,
  ASan clean on the three Unix legs. The compiler-engineer ran the variant shape
  on Darwin only, and the ffi seat did not run it.

**Routes neither seat built:**

1. The static block plus a count of static references (built as a probe in (3)).
   It makes a release too many on a constant a named panic on a plain build.
   Always on it costs +52% / +82% over the static route. Compiled into
   `--sanitize` builds only (unbuilt), a plain build would pay nothing.
2. The static block in plain builds, and today's block per read under
   `--sanitize`, so ASan keeps naming a release too many on a constant. The
   cost is that a sanitized binary no longer exercises the static path. Unbuilt.
3. `hero_array_at` inlined in the header (measured on the floor in (4): 116.5M
   → 85.5M at -O0). It benefits every array read, not only constants, and as a
   declaration it would move the stamp. Outside 382's scope, named so nobody
   reads the element route's 0.68x as a constant-specific gain.

**Contradictions between the seats, and which side the runs take:**

- *compiler-engineer: approve the static block; ffi: object while a release too
  many is invisible.* Run: the release too many is invisible under the built
  route on four platforms, plain and under ASan. It is also **harmless**: right
  value, no memory error. The ffi seat's own approved alternative, `onceheld`,
  turns the same bug into **a changed constant for every reader at exit 0**
  (Darwin, Linux arm64, Linux x86-64). ASan names it only after the wrong value
  has been printed, and on Windows the heap manager does (127). The landed
  route is loud but misleading (`array with no element descriptor` on Darwin,
  `array index out of range` on Linux). No route the seats built is both
  harmless and loud; the probe in (3) is. The run cannot settle what is left:
  whether a guard that leaves the emitter's own error harmless but unnamed is
  §1.12's *"a defensive check [that] hides a defect"* (design.md:609-613). That
  is a ruling for the sitting.
- *compiler-engineer: "the guard costs nothing over ir12's"*: falsified for
  constant reads, +4.9% / +5% (4).
- *compiler-engineer: "element route ~1x"*: falsified, 0.68x; but 84-86% of that
  is an inlined `hero_array_at` (4).
- *ffi: `onceheld` 1.50x / 2.01x the floor; compiler-engineer: the static block
  1.28x / 1.43x.* Both from Darwin counts. Nothing contradicts that ordering.
  Neither seat, nor I, counted instructions off Darwin (`perf` absent in the
  image; no counter in Git Bash).

**The question the sitting did not ask**: what each route does, on each
platform, plain and sanitized, with **the emitter's own error on a constant's
value**: a release too many, and a release missing. The briefs asked whether the
*runtime's* double release stays loud. Today that is loud on one platform at one
size, and the guard does not change it anywhere else. The outcome that actually
differs between routes is the program's, in (3)'s table, and it was measured
only in this pass.

## Written 13:26:29 — a missing release, the other half of the question, run (Darwin, -O0)

`second/{landed,static,onceheld}_missing.c`: reader 1 takes `K` and never
releases it, reader 2 reads and releases, then the leak check:

| route | outcome |
|---|---|
| landed | `panic: 1 heap blocks still live at exit (a missing decref) — this is a compiler bug`, exit 134 |
| static block (compiler-engineer) | `leak check passed`, **exit 0** |
| `onceheld` (ffi) | `panic: 1 heap blocks still live at exit (a missing decref)`, exit 134 |

The compiler-engineer names this ("a missing release of a constant's value is
no longer a leak") and answers that every other call result still exercises
that path. Measured here: on the static route the leak check cannot see it,
while `onceheld` keeps it loud. The probe in (3) would see it only if it also
checked its count at exit, which it does not (unbuilt). Linux and Windows unrun
for this case.

Pass closed 13:26:29. Files: `<scratchpad>/195-critic/ce/` (the route rebuilt),
`195-critic/second/` (`*_over.c`, `*_missing.c`, `leg/` with `leg/bal/` and `leg/rt-bal/`, `measure/`, `shapes/`),
on the box `/c/w/p195-critic-32165/` (made 13:18, nothing removed,
`/c/w/b12-*` untouched).
