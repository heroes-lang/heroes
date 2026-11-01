# Panel 233, the completeness critic's second pass: the reports

Written as it went, 2026-11-01, in `.claude/worktrees/233-critic/` (detached at `0ee14c9be`). The
seats' worktrees were read and their files copied, never built in or edited. Instruments: instructions
retired and peak memory footprint (`/usr/bin/time -l clang -fintegrated-cc1 ...`), no wall clock; one
depth-18-sized compile at a time; every helper file under this worktree's `critic/`.

## 0. The route, rebuilt here

The seat's 18 changed and 2 new files under `selfhost/` and `runtime/` (`git -C
.claude/worktrees/233-compiler-engineer diff --name-only`, plus `runtime/parts/frame.c` and
`selfhost/emit/large_value.hero`) copied over `git archive 0ee14c9be -- seed runtime selfhost` in
`critic/route/` (`cmp` of every copied file: no difference); stage 1 from the frozen seed, stage 2
`critic/route/heroes-route`: **`sources 588572956d7a94d0`**, the seat's final key (§ 8 of its report;
`ce/heroes-route --version` prints the same). The frozen compiler is this worktree's stage 2,
`2764fc8525370b80`. A compiler finds its runtime beside its binary: the route's builds read
`critic/route/runtime`, the frozen one's the worktree's `runtime` (read from each build's
`build/words/*.rsp`).

## 1. Veto (a): one C signature for a function's value and its thunk, and the check

- **`ffi/bind/bigabi.hero` and `extval.hero`, built by the route** (copied with `bigabi.h`,
  `bigabi.c`, `ref.c`, `extval_ref.c` to `critic/ffi/bind/`; `libbigabi.a` rebuilt there; a `.pc`
  of mine under `PKG_CONFIG_PATH`): both build at `-O0` and `-O2` under both compilers and print
  exactly what the C references print (string comparison of the outputs). **The route's units differ
  from the frozen units at one line each, `hero_program_sources`** (`diff`; four units per program,
  two contents). So bigabi **tests nothing of the route**: `Big` is a group record, which the
  threshold leaves out (`large_value.hero:154`, `byte_copy.own_struct`), and `Holder`, which holds
  it, is counted at 16 bytes (§ 2).
- **No converted function can be a thunk, by the checker.** `critic/split/cbsplit.hero`, an extern
  `apply(cb: (function(E4) -> i64), ...)` with `E4` a 32 KiB program record, and `cbsmall.hero`, the
  same with an 8-byte `E0`: both compilers refuse both, `error[ffi_type]: (function(E4) -> i64)
  cannot cross the FFI boundary, and it is an extern's parameter, the callback may stand there, but
  E4 inside it cannot (§4.19)`. Only a group record can stand in a callback's type, and the route
  converts no group record, so no thunk ever forwards to a converted function. Veto (a) is met **by
  exclusion, not by a check**.
- **The equivalent of veto (a)'s shape, built**: `critic/split/valsplit.hero`, `give`/`give2`
  returning `E4` through `pick(n) -> (function(i64) -> E4)`, `first(e: E4)` held as `h: (function(E4)
  -> i64)`. Both compilers, both levels: `10 2010 14`, exit 0. The route's unit keeps C's by-value
  convention in both typedefs (`typedef h_valsplit_E4 (*h_0fn_26ba6a6)(int64_t);`, `typedef int64_t
  (*h_0fn_503123d5)(h_valsplit_E4);`) and in the twins (`h_valsplit_E4
  hero_value_h_valsplit_give(int64_t h0_n)`), and the direct functions alone take `hero_out` /
  `hero_in_`: R7b's shape. The route's thunk is also by value: `callback_thunk.header` spells each
  parameter with `signature.one_param` (by value unless `@`) and its result with `ctype.c_result`,
  and calls `signature.as_value`, the twin.
- **A per-thunk `_Static_assert`: none** (`grep` of the route's units for
  `_Static_assert(__builtin_types_compatible_p(__typeof__(&`: nothing). **An equivalent check exists
  for VALUES, measured**: every use of a twin is a typed assignment, `t5 =
  hero_value_h_valsplit_give2;` into an `h_0fn_26ba6a6`; with that twin's prototype and definition
  edited by hand to the direct convention (`critic/split/valsplit-route-mismatch.c`), clang refuses
  the unit under the build's words, `valsplit.hero:68:8: error: incompatible function pointer types
  assigning to 'h_0fn_26ba6a6' ... from 'void (h_valsplit_E4 *, int64_t)'`; the unedited unit gives
  no diagnostic. **For THUNKS no check exists**: the lookup table compares and returns through
  `(void (*)(void))` casts (`callback_thunk.hero:185-188`), so a thunk printer that drifted would be
  silent, exactly the ffi seat's R7c. It cannot drift today only because (i) the thunk's printer and
  the typedef's printer both spell by value, and (ii) no converted function is ever thunked.
- **A comment the route made false**: `callback_thunk.hero:121-122` (route) still says the thunk's
  signature is *the function's own result and parameter list, spelled by the same printer the
  function's signature uses*. For a converted function the function's own signature is now `void
  f(T *hero_out, ...)` and the thunk's is not; the ffi seat cited this comment as the rule R7c
  follows.
- **What lifts or keeps the veto, checkable**: the veto's first condition (*one C signature for a
  function, its value and its thunk*) is not met literally, the direct function differs; its second
  (*the emitter derives each thunk from the very typedef the probe checks*) is not met either, the
  thunk and the typedef come from two printers that agree. The route stands outside the veto only
  while the threshold excludes group records; **the ffi seat's own widening (group records counted)
  would put converted functions into callbacks, and then the per-thunk assertion is the only guard**
  (§ 2).

## 2. Group records: what each seat's choice does to a program that holds or returns a large one

**The route as built (group records left out) changes nothing for them, measured.**

- `critic/grp/grpwrap.hero`: a PROGRAM record `Wrap(tag: i64, inner: Held)`, `Held` defect 2253's
  128 MiB `partial` header struct, built by `make_wrap` and read in `go`; `critic/grp/grpret.hero`:
  a Heroes function `make(n) -> Held` and `go` calling it. Both compilers, both levels, `timeout 60`:

  | program | `-O0` | `-O2` |
  |---|---|---|
  | grpwrap, frozen and route | `before`, `panic: stack exhausted in grpwrap.go`, 134 | `before`, `13`, 0 |
  | grpret, frozen and route | `before`, `panic: stack exhausted in grpret.go`, 134 | `panic: stack exhausted in grpret.main`, 134 |

  The route's four units hold no `hero_frame_take` and no `hero_out` (`grep -c`: 0 each).
- **The route's own claim is false for `Wrap`.** Its § 7 says *no value of the program's own past
  4,096 bytes is ever a local of a C frame*. `Wrap` is the program's own (`own_struct` true), its C
  size 128 MiB plus 8, and it stays on the frame, because `bytes_of` reads a group record through
  `sizes.decls`, which `fields_bytes` fills from the fields a record NAMES (`large_value.hero:93-130`):
  a `partial` record counts as its named fields alone, `Held` as 8 bytes, `Wrap` as 16. The route
  says its count is *a lower bound of `sizeof`, which only moves which side of `LARGE` a value falls
  on* (`:38-39`); for a `partial` field the bound can be off by any factor.
- **The compile cost of a chain of group records is unchanged.** `critic/grp/chain/{gn12,ga12}`, the
  ffi seat's `gen2.sh` copied (the reproducer's chain over group records, named halves and array
  halves, 64 KiB): both compilers' units differ at `hero_program_sources` alone (`diff`). So the ffi
  seat's frozen readings stand under the route: 12.18 and 9.94 billion at 64 KiB, and **446.9 billion
  instructions and 6.98 GB for the array chain at 1 MiB**, worse than the program-record reproducer at
  4 MiB. **Defect 2278 stays open for group records under the route as built**, and the route's
  report does not say so; its "every shape" tables hold program records only.

**The ffi seat's choice (bytes from `sizeof`, group records counted) is unbuilt; what can be measured
of it:**

- **A by-value `extern` argument always costs the caller a full-size frame copy**, C's ABI, whatever
  holds the value: `critic/grp/byval.c` at `-O2 -fstack-usage`, `huge_sum(*p)` from heap memory,
  **16,777,248** bytes of frame for a 16 MiB struct; the ffi seat's own `r3-box.c`, `arg_from_box`,
  **262,176**. So counting `Held` would move 2253's local off the frame and the call
  `heroes_held_first(h: held)` would put 128 MiB back (inference from the two readings; 2253's case
  itself unrun under such a route).
- **An `extern`'s large result reaches a room only by an optimiser heuristic.** `*p = huge_make(s)`
  through a parameter: 16,777,248 (`byval.c`); through a pointer from an allocator declared plain,
  `malloc`, or `malloc, alloc_size(1)`: 16,777,264 each (`room.c`, `room2.c`); from libc `malloc`,
  returning the pointer, 256 KiB or 16 MiB: 262,192 / 16,777,264 (`room3.c`). **The ffi seat's
  32-byte `result_into_box` holds only with its store before the call**: `room4.c`, the same box with
  `b->rc = 1;` before `b->v = mid_make(s)`: **32**; after it: **262,192**. So the seats' two probes do
  not contradict each other (the compiler-engineer's *`*p = make(n)` keeps the hidden temporary even
  at `-O2`* is the general case), and the route's `hero_frame_take` is declared with no attribute
  (`critic/route/runtime/heroes_runtime.h:874`).
- **A `sizeof` line makes the emitted C depend on the target.** `critic/grp/target.c`,
  `struct longs { long a[600]; }` with `_Static_assert(sizeof > 4096)`: passes on
  `arm64-apple-darwin`, `x86_64-pc-linux-gnu`, `aarch64-unknown-linux-gnu`, fails on
  `x86_64-pc-windows-msvc` (LLP64, 2,400 bytes). The net's `emission` suite keeps one blessed file per
  program (no per-target file among the 728) and skips Darwin-only programs by name on the other legs
  (`tests/harness/suite_emission.hero:206-221`); a program holding such a struct would emit differently
  on Windows. The route's own rule was chosen against exactly this (*the same number on every target
  ..., so a unit's C does not depend on the machine*, `large_value.hero:35-38`).
- **Counting group records turns callbacks into converted functions**: a Heroes function taking or
  returning a large group record would get a twin, and its thunk would forward to it. That is
  veto (a)'s ground, and the route has no per-thunk check (§ 1).

**What neither seat measured and the sitting needs**: the rest of defect 2278 for group records (the
1 MiB chain at 447 billion stays as it is) is either a second route (a count for group records that
does not depend on the target, named fields AND `sizeof`'s lower bound, or `partial` records refused a
place on the frame), or a stated carve-out filed as its own defect. And `Wrap`, a program record that
holds a `partial` group record, is the place where the two seats' choices meet: the compiler-engineer's
count misses it, the ffi seat's would catch it at the cost above.

## 3. The room's gibibyte, per thread, and spec § 9

- **The bound is per thread, read and measured.** `runtime/parts/frame.c:49-53` (route):
  `HERO_FRAME_ROOM ((size_t)1 << 30)` against `static _Thread_local size_t hero_frame_held`; nothing
  bounds the process. A spawned thread asks at least its spawner's stack (`spawn.c:348-362`), 8 MiB
  here, so the room's bound fires before the stack's. Probes `critic/threads/{one,three,four_bounded}.hero`
  (the seat's `down(n, e: E4)`, 32 KiB copied at every level, reached through `run(n)`; threads by
  `hero_thread_spawn`), both compilers at `-O2`, `timeout 120`, `/usr/bin/time -l`:

  | program | frozen | route |
  |---|---|---|
  | one: `main` alone, 100,000 deep | `panic: stack exhausted in one.down`, max resident 10,223,616 | `panic: stack exhausted in one.down`, max resident **1,098,629,120** |
  | three: two spawned and `main`, each 100,000 deep | the same panic, 27,066,368 | the same panic, **3,289,038,848** |
  | four_bounded: three spawned and `main`, each 6,000 deep | the same panic | **`6001 6001 6001 6001`, exit 0, 2,402,844,672** |

  So the route's rationale for the bound, *on Linux the OOM killer's SIGKILL with nothing said*
  (`frame.c:42-48`), holds for one thread: three runaway threads held 3.29 GB before the first bound
  fired, and N threads ask N GiB (multiplication from the reading of three; 16 threads is 16 GiB on a
  machine of 24). Correction the sitting owes: a bound for the process (an atomic sum across threads),
  or the number stated per thread with what a program of N threads may hold.
- **Spec § 9's *running out of stack aborts*** (`spec/heroes-spec.md:289-290`) **stays true of the
  stack, and the route moves which resource runs out.** In `one` the route aborted at 1.10 GB resident,
  its stack far from its 8 MiB, with **the stack's words**, `panic: stack exhausted in one.down`
  (`frame.c:58-73`, `hero_frame_exhausted` prints the guard's text). The message names a resource that
  did not run out. The project counts *a false message* as `blocking`
  (`.claude/rules/verification.md`, Bounded discovery); whether a room exhausted is "stack" in the
  spec's sense is a question for the sitting, with that reading in front of it. And `four_bounded`, a
  program that aborts today at every level, runs under the route holding 2.4 GB of heap: the depth a
  recursion of large values reaches moves from about 8 MiB of frames to 1 GiB of room a thread.
- **Beside the defect, in today's runtime**: concurrent stack exhaustions write garbled messages. The
  frozen `four_bounded`'s stderr, twice: `panic: stack exhausted in panic: stack exhausted in
  four_bounded.downpanic: stack exhausted in panic: stack exhausted in
  four_bounded.downfour_bounded.downfour_bounded.down` and `panic: stack exhaustedpanic: stack
  exhausted in  in four_boundedfour_bounded.down...`. The guard writes its message in pieces
  (`hero_stack_say`) from each thread at once, no newline between them. Not the route's; the route's
  room abort calls the same pieces, so N threads exhausting their rooms together would interleave the
  same way (inference: the route's `three` printed one message, the process ended first). A filing of
  its own, `adjacent` or `blocking` by the class of a garbled message.

## 4. The route's run cost: the seat's numbers reproduce, and the comparison hides the per-use price

- **The readings reproduce.** `critic/bf16-O2` (frozen) and `critic/runcost/bf16-route-O2` (route),
  three runs each under `/usr/bin/time -l`: frozen 18,180,624 / 17,085,970 / 16,999,854 instructions,
  1,785,856 resident; route 24,945,311 / 24,322,930 / 24,309,625, 8,208,384 resident. The seat's
  17,852,466 and 24,442,073 sit inside those spreads. **The spread is about 7% between runs of one
  binary**, which the seat's table does not state; most of either number is the runtime's start
  (about 17 million in every binary measured here).
- **What the comparison hides**: today's `-O2` binary of the reproducer builds nothing (clang folded
  the record to the one leaf read), so "6.6 million more" is the price of building 1 MiB once. A program
  that builds a record past the line repeatedly, and that today's clang compiles easily, pays it per
  build. `critic/loop/loop8k.hero` (an 8 KiB record, two `E3`, built 2,000 times, two leaves read) and
  `loop32k.hero` (a 32 KiB `E4`, the same), both compilers at `-O2`:

  | program | frozen run | route run | route minus frozen | frozen compile | route compile |
  |---|---:|---:|---:|---:|---:|
  | loop8k | 17,709,463 | 25,303,561 | **7.6 million** | 8,360,188,568 / 75.7 MB | 5,631,828,203 / 65.1 MB |
  | loop32k | 17,867,344 | 71,980,871 | **54.1 million** | 8,492,158,406 / 77.2 MB | 5,635,156,121 / 64.9 MB |

  Both print the same totals (`4000000`, `3998000`). Today the loop's work is about 0.1 million
  instructions over the start; under the route it is about 3,800 instructions per 8 KiB build and
  27,000 per 32 KiB build, proportional to the bytes. **These programs are ones the route does not need
  to help** (8.4 billion to compile today, and they run), and they pay at run time. The seat's answer to
  "what the route costs a program it does not need to help" covers only programs with no value past
  4,096 bytes (byte for byte the same, true by its census); between 4,096 bytes and the size where
  today's clang chokes (the compiler-engineer's baseline, its § 1: 3.1 billion at 64 KiB, 66 billion at 1 MiB), the price is
  per construction and the sitting should see it beside the compile curve, since a higher line would
  keep these programs as they are and cost more compile (the seat's 73 to 103 billion at a 64 KiB line).

## 5. Shapes under the line: none measured grows past the `i64` bound, with one cost beside it

Every reading the route's `-O2` compile of the program's unit, the build's words (`critic/scratch/mw.sh`):

| probe (`critic/below/`, one module each) | largest value on the frame | instructions | peak |
|---|---|---:|---:|
| `onefn_e3`: `i64` leaves, eight-way, built in one function, `E3` | 4,096 B, 512 leaves | 270,767,366 | 11.4 MB |
| `onefn_u4`: **`u8` leaves**, the same, `U4` | 4,096 B, **4,096 leaves** | 720,126,626 | 26.0 MB |
| `levels8_e3`: eight calls a level, `E3` | 4,096 B, 512 leaves | 828,651,247 | 16.8 MB |
| `levels8_u4`: the same, `U4` | 4,096 B, 4,096 leaves | 692,531,507 | 16.8 MB |
| `onefn_e6` (2 MiB, past the line) | `E3` | 5,303,235,266 | 63.3 MB |
| `onefn_u6` (256 KiB, past the line) | `U4` | 3,112,424,645 | 73.8 MB |
| `levels8_e5` (256 KiB) | `E3` | 4,975,368,989 | 66.9 MB |
| `levels8_u5` (32 KiB) | `U4` | 4,796,091,822 | 66.9 MB |
| `flat/flat4096`: ONE record of 4,096 `u8` fields, copied eight times into a record past the line | 4,096 B, 4,096 leaves | 2,062,666,112 (frozen: 2,058,487,616) | 36.8 MB |

- **The seat's named worst case does not materialise.** One-byte leaves cost 2.7 times the `i64`
  shape at the line in one function, the same or less in eight calls a level, and less past the line;
  the flat 4,096-field record costs 2.06 billion, under the seat's 8-billion prediction. So bytes holds
  as the threshold's unit for clang's work; the seat's "a count of leaves beside the bytes would bound
  that too" is not needed by these readings. (Its § 8 condition (4) stands unfalsified.)
- **A field of type `()` cannot make a zero-byte leaf**: `record Z0` with `u: ()` passes `check` and
  the backend refuses it, `unsupported[unit_field]: () as the type of Z0.u is not emitted yet`
  (`critic/below/unitfield.hero`), so `bytes_of`'s `.unit => 0` never meets C.
- **The counted shape peaks below and at the line, not with the record.** The seat's counted record,
  generated at three sizes (`critic/counted/s{3,4,5}`, its two modules, `S0` a `str` and an `i64`):
  the `held` unit at `-O2` costs **7,182,256,771** instructions at `S3` (12 KiB; the seat's 7,180,918,378
  reproduced) and **326,048,038** at `S4` (96 KiB); all three run `leaf`, `new!`, `14` at `-O0`.
  Strings alone, four-way (`q4`, 256 strings at the line; `q5`, past it): 253,437,303 and 273,209,521.
  So the cost depends on the inliner's view of the per-type walks just under the line, not on the
  record's size. **But `S3`'s peak is 105,972,168 bytes, past the seat's 100 MB prediction line, and its
  7.18 billion is within 10% of the 8-billion line.** The prediction names the filing's shapes and the
  reproducer only, so it is not falsified; the counted case the seat's own § 8 says the landing owes
  would cross it.
- **Beside the defect, the compiler's own cost grows toward quadratic in a construction's fields.**
  The frozen compiler's own instructions (`/usr/bin/time -l heroes-next build -O0`, the compiler's
  process alone) for one flat record of N `u8` fields built once: **1,956,310,637; 3,745,036,634;
  8,898,719,901; 25,657,362,858** at N = 1,024, 2,048, 4,096, 8,192 (x1.91, x2.38, x2.88 a doubling);
  the route's at 4,096: 8,923,192,038. `sample` of the 8,192-field build (`critic/below/flat/c8192s/
  sample.txt`): 734 of 1,137 samples under `clirounds.round`, 315 in `emitprologue.written`, 156 of them in
  `emitnodebug.hidden` copying strings through `hero_array_push`, and `irquestions.given_fields` 82 at
  the top of the stack. The same family as the ffi seat's § 3c (group records' named members); not
  this defect and no route of this sitting reaches it; a filing of its own.

## 6. The frame face is not closed for two shapes the route's claim covers

The route's § 7 says *no value of the program's own past 4,096 bytes is ever a local of a C frame*.
Two shapes falsify it, measured with the route compiler:

- **A program record holding a `partial` group record** (§ 2): `grpwrap`, `Wrap` counted 16 bytes,
  `stack exhausted in grpwrap.go` at `-O0`, as today.
- **A large record returned through a function value**: `critic/split/direct7.hero` and
  `viaval7.hero`, `give(n) -> E7` (16 MiB), read by `first(e: E7)`, called directly in one and through
  `pick() -> (function(i64) -> E7)` in the other. Direct: `before`, `5` at `-O0` and `-O2`.
  **Through the value: `before`, `panic: stack exhausted in viaval7.go`, at both levels.** The twin
  keeps C's by-value return, so the indirect call's result is a 16 MiB object in the caller's frame;
  clang's work stays bounded (4,317,866,020 direct, 4,490,944,951 through the value). The ffi seat
  predicted this in words for R7b (*a call through a function value pays today's by-value return*);
  the route's report does not mention it.

## 7. Routes nobody listed

1. **Function types that mention a large program record take the direct convention, with no twin.**
   The route keeps C's convention for every function value because *one function type's typedef is
   also what an extern's callback parameter is spelled with* (`large_value.hero:29-33`). For a type that
   mentions a program record that premise is false: the checker refuses any program record inside an
   extern's callback type, large or small (`cbsplit`, `cbsmall`, § 1), so such a typedef never reaches C.
   Spelled with `hero_out` / `hero_in_`, those values would take the room like direct calls, closing
   `viaval7`'s abort (§ 6) and removing the twins. What must hold: `hero_callback_of` only ever receives
   a value whose type an extern may name. Unbuilt.
2. **A target-independent rule for a `partial` group record held by a program record**: count such a
   program record as large whatever its named fields sum to (its size being unknown to the count),
   which closes `Wrap` (§ 2) without the `sizeof` line's dependence on the target (`target.c`, § 2).
   Unbuilt.
3. **R6 for functions that build or return a large group record**, beside the route for program
   records: the ffi seat measured `noinline` on the group-record array chain at 1 MiB at 485,283,331
   instructions and 12.8 MB, against 446.9 billion without it (its § 5 and § 3b). It would answer the
   group-record half of the defect's compile cost, with R6's own risk at the frame (k = 18 aborts at
   `-O2`, both seats), so it needs the frame rule beside it. Unbuilt on group records past 1 MiB.

## 8. Contradictions between the seats, each with what checks it

- **Group records in the threshold** (the compiler-engineer: out; the ffi seat: in, by `sizeof`):
  measured in § 2. Out leaves the defect open for group records (447 billion at 1 MiB) and `Wrap` on the
  frame; in makes emissions depend on the target, cannot remove a by-value `extern` crossing's copy, and
  turns callbacks into converted functions, where the missing per-thunk check matters (§ 1).
- **"A C result can go straight into the box"** (ffi § 4, R3) against **"`*p = make(n)` keeps the
  hidden temporary even at `-O2`"** (compiler-engineer § 3): both true, shape by shape. `room4.c`: the
  same `malloc`ed box takes the result in a 32-byte frame with a store before the call, and keeps
  262,192 bytes without it; every other destination measured keeps the full temporary (§ 2).
- **What the ffi seat approves is not what the compiler-engineer built.** The approval: R7 in R7b's
  shape, one `_Static_assert` per thunk, paired with R6, the threshold from `sizeof` and over group
  records. The route: a room, the caller's slot, the caller's copy, construction in place; group records
  out; no R6; no per-thunk check. Checkable: veto (a) is met by exclusion (§ 1); R6 adds nothing beside
  the room (the route's k = 18, 438,942,805 against the ffi seat's R7 with R6, 539,822,425); the
  missing assertion matters only if group records are counted. The synthesis must say which of the
  ffi seat's conditions it waives and why.

## 9. Claims asserted and not settled by their command

- Route § 7, *no value of the program's own past 4,096 bytes is ever a local of a C frame*: false for
  `Wrap` and for a call through a function value (§ 6), measured.
- Route § 7, the room *bounded at one gibibyte a thread*: true per thread and unbounded per process
  (§ 3), measured; the rationale given for the bound (the OOM killer) needs the process bound.
- Route § 8, Q1, *the room's bound keeps* spec § 9's promise: the abort is kept, its words name the
  stack when the room ran out (§ 3).
- Route § 5, *the cost no longer depends on the record's size*: true of every shape measured here
  past the line, program records only; group records are not covered (§ 2).
- Route § 6, *a program with no value past 4,096 bytes ... pays nothing*: true, by its census; the
  programs past the line that today's clang compiles easily pay per construction (§ 4), unstated.
- Route § 8, Q3, the tree's nearest record *about 98 bytes*: an estimate by the route's summing rule
  with variants left out, not a `sizeof`; the brief asked for the program on each side and the
  answer's lower side is an estimate.
- The ffi seat, § 2, the routes' lowering on x86-64 and Windows: said as an inference, correctly.

## 10. The questions the sitting should have asked

1. Does the route close the defect for every shape it names, group records included? (No, § 2.)
2. What does a program with a record between the line and today's choke point pay at run time, and is
   4,096 the line that trades that against the compile bound? (§ 4.)
3. Is the room bounded per process? (No, § 3.)
4. Is "stack exhausted" a true message when the room ran out? (§ 3.)
5. Do calls through function values keep a large record off the frame? (No, § 6.)
6. What does the route do to a program record holding a `partial` group record? (Nothing; § 2.)

## 11. The short list

1. Veto (a): met by exclusion, not by a check. No converted function can be a thunk (the checker
   refuses any program record in a callback type); twins and thunks keep C's by-value convention;
   function values are type-checked at their assignment (a drifted twin is refused,
   `valsplit-route-mismatch.c`); thunks have no check, and `bigabi.hero` tests nothing of the route
   (units identical but the key).
2. Group records: the route changes nothing for them (447 billion at 1 MiB stays; `Wrap`, a program
   record holding a `partial` group record, is counted 16 bytes and aborts at `-O0`); counting them by
   `sizeof` makes emission depend on the target and cannot remove a by-value `extern` copy.
3. The room's gibibyte is per thread: three runaway threads held 3.29 GB; four threads at 6,000 deep run
   holding 2.40 GB where today they abort; the room's abort prints the stack's words.
4. The run-cost numbers reproduce (about 7% spread), and hide a per-construction price for records
   past the line that today's clang compiles easily: 7.6 and 54.1 million more for 2,000 builds of 8
   and 32 KiB.
5. No shape under the line grows past the `i64` bound: `u8` leaves cost the same or less past the line
   and 2.06 billion for the seat's 4,096 one-byte fields. The counted record peaks at 7.18 billion and
   106 MB at `S3`, past the seat's 100 MB line.
6. A large record returned through a function value still aborts `stack exhausted` (16 MiB, both levels).
7. Beside the defect: concurrent stack exhaustions print garbled messages (today's runtime); the
   compiler's own cost grows toward quadratic in a construction's fields (25.66 billion at 8,192).
