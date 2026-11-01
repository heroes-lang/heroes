# Panel 233, the ffi-pragmatist's report: defect 2278 at the C boundary

Written as it went, 2026-11-01, in `.claude/worktrees/233-ffi-pragmatist/` (a detached checkout of
`0ee14c9be`). Every file this seat made is under that worktree's `ffi/` (units, binding headers,
probes). No wall clock was read: clang's work is counted in instructions retired and peak memory,
`/usr/bin/time -l clang -fintegrated-cc1 ...`, one depth-18-sized compile at a time. Every number
carries the command that produced it; a negative sentence is either run or written as a question.

## 0. Setup

- Stage 1, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `sources 1831f7e156474293`.
  Stage 2, `./heroes build selfhost/main.hero -o heroes-next`: `sources 2764fc8525370b80`, the frozen
  compiler; every emission below is stage 2's.
- The build's own `-O2` words, read from the response file a stage 2 `-O2` build wrote
  (`build/words/words-7acece285dab960a.rsp`): `-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS
  -Wall`, the eight `-Werror=` words, `-fno-strict-aliasing -fno-delete-null-pointer-checks
  -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion
  -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O2 -gline-tables-only -I runtime -I
  <dir>`. Every count below is `/usr/bin/time -l clang -fintegrated-cc1 <those words> -c <unit>.c -o
  /dev/null` (`ffi/scratch/m.sh`), so it includes `-gline-tables-only`, which the brief's command
  omits; Linux's `-fstack-clash-protection` is absent (Darwin only here).
- Calibration: defect 2253's main case unit at `-O2`: 194,721,111 and 193,324,404 instructions, peak
  footprint 9,634,200 and 9,601,408 bytes (the critic, brief's words: 191,127,614 and 9,568,664).

## 1. What crosses the boundary today, measured

Probes in `ffi/bind/probe/` (`q.h` and four programs), each `./heroes-next check <file>`:

- **A Heroes record cannot cross at all**: `extern ... function q_takes_mine(p: Mine)` with `record
  Mine` a program record is `error[ffi_type]: Mine cannot cross the FFI boundary ... a C header can
  declare a number, bool, str, ptr, cstr, a function type as a parameter, and a record declared in
  this same group (§4.19)`. So a route that touches only the program's own records can change an
  extern call's C in exactly two ways: through a **callback** (a Heroes function C calls, whose
  parameters and result may be group records), and through a **group record held inside a program
  record** (read out, or lent with `@` or `lent`).
- Accepted by `check`, exit 0, each: a callback taking a group record by value, `cb: (function(QBig)
  -> i64)`; a callback **returning** one, `cb: (function(i64) -> QBig)`; a program record holding a
  group record, `record Holder` with `inner: QBig`; that field lent to C, `q_fill(out: @h2.inner, ...)`
  and read-only, `q_peek(b: h2.inner)` against `const qbig *`.

**The binding** (`ffi/bind/`): `bigabi.h` declares `struct big`, 256 KiB (`int64_t first; int64_t
mid[32766]; int64_t last;`), and seven functions defined OUT OF LINE in `libbigabi.a`, so every call
crosses the platform ABI: returned by value (`big_make`), passed by value (`big_sum`), both
(`big_bump`), a callback taking it (`big_apply(int64_t (*)(big), ...)`), a callback returning it
(`big_apply_ret(big (*)(int64_t), ...)`), an out-parameter (`big_fill(big *, ...)`) and a read-only
lend (`big_peek(const big *)`). `bigabi.hero` binds all seven (`record Big tag big partial`, through
`package "bigabi"` and a `.pc` in the worktree), holds a `Big` inside `record Holder`, lends
`@h.inner` and passes two Heroes functions as callbacks. `ref.c` is the same program in C alone.

- Built by stage 2 at `-O2` and at `-O0` and run under `timeout 60`: both print exactly what `ref.c`
  prints (`diff`, six lines).
- The emitted unit (`ffi/units/bigabi-today.c`) keeps C's own type at every crossing:
  `typedef int64_t (*h_0fn_6218963)(struct big);` and `typedef struct big (*h_0fn_3ae0baa8)(int64_t);`
  for the two function types; `int64_t h_bigabi_on_big(struct big h0_b)` and `struct big
  h_bigabi_give_big(int64_t)` with C's by-value signatures; thunks `h_0cb_bigabi_on_big` and
  `h_0cb_bigabi_give_big` of the same signatures; and a probe per extern, **the callbacks included**:
  `hero_ffi_probe_h_bigabi_big_apply_ret(h_0fn_3ae0baa8 a0, int64_t a1) { (void)(big_apply_ret)(a0,
  a1); }`, compiled under `-Werror=incompatible-function-pointer-types` (a pragma of the unit). That
  probe is the importc check extended to a callback: a Heroes function type whose C spelling is not
  the header's parameter type is a compile error.
- **A detail the briefs do not carry**: every binding of a group record is followed by
  `hero_pad_fill(&<binding>, sizeof(struct big), ...)`, and every lend by the same on the field
  (panel 215's R1, padding written zero), so a group record's address always escapes to the runtime,
  an identity clang cannot see into. The 2253 case has it too (`h0_held`), which is a fourth
  difference between that case and the reproducer, beside the critic's three.
- `HERO_COPY` and `HERO_COPY_HELD` never touch a group record today: `own_struct` answers false for a
  record with a header (`selfhost/emit/byte_copy.hero`, `own_declaration`: `rec.header.is_err()`).

**The instrument for a route's ABI** (`ffi/scratch/abi.sh`): the unit compiled to LLVM IR at `-O2`
with the build's words, printing every extern's `declare`, every call to one and every thunk's
`define`, with value names, metadata, operands and optimiser-inferred attributes (`nonnull`,
`readonly`, `captures(...)`, ...) stripped: two outputs differ only where a type, a calling
convention or an `sret` slot differs. Today's boundary on arm64:

```
declare void @big_make(ptr sret(%v) align 8, i64)           ; result through the caller's slot (x8)
declare i64 @big_sum(ptr)                                   ; a pointer to the caller's copy (x0)
declare void @big_bump(ptr sret(%v) align 8, ptr, i64)
declare i64 @big_apply(ptr, i64)
declare i64 @big_apply_ret(ptr, i64)
declare void @big_fill(ptr, i64)
declare i64 @big_peek(ptr)
define i64 @thunk(ptr %v)                                   ; on_big's thunk, as C calls it
define void @thunk(ptr sret(%v) align 8 %v, i64 %v)         ; give_big's thunk
```

**The instrument for a route's behaviour** (`ffi/scratch/run.sh`): the unit compiled with the
build's words, linked against the same objects the build linked (its library unit, its runtime,
`libbigabi.a`), run under `timeout 60`, output diffed against `ref.c`'s. Today's unit: SAME at `-O2`
and `-O0`.

## 2. Each route's binding C, compiled against the real header and run

Each variant is today's unit (`ffi/units/bigabi-today.c`) rewritten the way the route would emit
it, by `perl` over the named statements and nothing else, then judged by both instruments.

| route | the C it implies (file) | boundary ABI (`abi.sh` against today) | run (`run.sh`, `-O2` / `-O0`) |
|---|---|---|---|
| R1, copies through memory clang cannot split, keyed on size, **group records included** | the nine place-to-place copies of `struct big` and `h_bigabi_Holder` (`h0_a = t2;`, `t3 = h0_a;`, the `Holder` compound literal, `t1 = h0_b;` in `on_big`, ...) written `HERO_COPY_HELD(dst, (src));`, the macro byte for byte `byte_copy.hero`'s `DEFINITION_HELD`; call results stay assignments (`bigabi-R1.c`) | identical | SAME / SAME |
| R2, `optnone` on a function whose frame holds such a record | `__attribute__((optnone, noinline))` on `main`, `on_big`, `give_big` and both thunks (`bigabi-R2.c`) | identical | SAME / SAME |
| R5, a unit flag | today's unit under `-fno-inline-functions`, and under `-mllvm -inline-threshold=0` | identical, both | SAME at `-O2`, both |
| R6, `noinline` on a function that builds, returns or holds such a record | `__attribute__((noinline))` on `main`, `on_big`, `give_big` (`bigabi-R6.c`) | identical | SAME / SAME |

Before the operands and the optimiser-inferred attributes were stripped, the raw IR diffs of R2 and
R6 differed only in `nonnull`, `readonly captures(none)`, `writeonly`, folded constants and the
number of call sites after inlining (`big_sum` 5 to 4, `big_make` 4 to 3): no parameter type, no
`sret`, no register class moved. **None of R1, R2, R5, R6 changes any extern call's C or ABI**, the
callbacks' included, on this Mac (arm64, Apple clang 21.0.0). On x86-64 and on Windows the same C
is lowered by those targets' conventions, which this seat did not run: a route that changes no C
type and no attribute of an extern declaration cannot change the lowering, which is an inference.

R5's flag reaches the header's own functions too. **Measured** (`ffi/scratch/inl.c`, `clang -O2
[-fno-inline-functions] -S -emit-llvm`): without the flag both a `static inline` and a plain `static`
function are inlined into their caller; with it **both stay calls** (`tail call fastcc i64 @marked`,
`... @plain`), so a header made of `static inline` functions (raylib's `raymath.h`, 2253's
`heroes_held_first`) loses its inlining in a unit that carries the flag. Speed, never ABI: the two
functions are internal, and clang moved them to `fastcc`, a convention it may give only a function
no other object calls. (The first draft of this paragraph said the opposite, from memory; the run
falsified it within the minute, and the paragraph says what the run says.)

### R7, a large record returned through the caller's slot: the one route that moves a C signature

R7 changes the C signature of a program function returning a record past the threshold, `struct big
give_big(int64_t)` becoming `void give_big(struct big *out, int64_t)`. That function is also a
**callback** here (`big_apply_ret(cb: give_big, ...)`), and one more fact decides the shape R7 may
take, measured with `ffi/bind/extval.hero`: **an extern returning a group record is a function value
of the same type as a program function**. `pick(n) -> (function(i64) -> Big)` returns `big_make`
(the extern) or `give_big`, Heroes calls through the value (`t5 = t3(t4)`), and C receives either as
the callback; built by stage 2 at `-O2` it prints what `extval_ref.c` prints. So one function type
holds a function of C's convention and one of the program's. Three variants of each unit, written by
`ffi/scratch/r7.pl` and `r7x.pl` (anchored rewrites, each anchor asserted to match exactly as often
as intended); in all of them the `Holder` is also constructed in place (`h2_h.f_inner =
big_make(t21); h2_h.f_tag = t20;`):

| variant | what it does | clang | boundary ABI | run |
|---|---|---|---|---|
| R7a | the function type's C spelling follows the new convention, `typedef void (*h_0fn_3ae0baa8)(struct big *, int64_t)` | **refused**: `incompatible function pointer types passing 'h_0fn_3ae0baa8' (aka 'void (*)(struct big *, long long)') to parameter of type 'big (*)(int64_t)'`, at the probe and at the call; on `extval`, four errors (the probe, `return big_make` into the new type, two indirect calls *too few arguments*) | n/a | n/a |
| R7b | **function values and thunks keep C's convention**: a value adapter `struct big h_0cv_..._give_big(int64_t)` is the function's address, the thunk keeps `struct big (int64_t)`, and only a direct call by name uses the out-parameter | accepted | identical | SAME / SAME, `bigabi` and `extval` |
| R7c | the probe and the cast keep C's spelling, but the thunk keeps **the function's own signature**, which is `callback_thunk.hero`'s documented rule (*"a second C function with the function's own signature"*) | **accepted, silent** | the thunk's `define` moves from `(ptr sret(%v) align 8, i64)` to `(ptr, i64)` | **exit 134 at both levels**: `panic: a null pointer was read through, at offset 0x5, called from bigabi.give_big` |

R7c is the finding. C calls the thunk as `big (*)(int64_t)`, the result slot in `x8` and the seed in
`x0`; the thunk reads `x0`, the integer 5, as the slot and the write lands at address `0x5`. The
runtime's fault handler named it because 5 is near null; an argument that happened to be a writable
address would have taken the 256 KiB write in silence (an inference, unrun). Clang saw nothing
because the argument reaches C through `hero_callback_of`'s cast to `void (*)(void)` and back: **the
probe checks the function type's typedef against the header, and nothing checks the thunk against the
typedef**. Today that gap is closed by construction, the function, its value and its thunk having one
signature; R7 is the first route that would split them.

**The gap closes with one line per thunk**, measured (`ffi/units/bigabi-*-asserted.c`):
`_Static_assert(__builtin_types_compatible_p(__typeof__(&h_0cb_bigabi_give_big), h_0fn_3ae0baa8),
"...")` before the table. Today's unit and R7b compile with it (exit 0); R7c is refused at compile
time, `static assertion failed due to requirement '__builtin_types_compatible_p(void (*)(struct big *,
long long), struct big (*)(long long))'`. So **R7 keeps the boundary only in R7b's shape, with that
assertion**, and its price at the boundary is a value adapter and a thunk per program function of a
large-record result whose address is taken, whose frames hold the record by value again (a call
through a function value pays today's by-value return). The `unused variable` warnings clang printed
on R7b and R7c (`t4`, `t22`, `t23`) are this seat's hand rewrite leaving declarations behind, not the
route's; an emitter would not declare them, and this build treats a clang warning on a correct
program as `blocking`, so an R7 lane owes that check.

## 3. The carried claim, separated: what the cost follows

The claim: defect 2253's group record `Held` (128 MiB, `int64_t first; int64_t rest[(1 << 24) - 1];`)
compiles at `-O2` in about 191 million instructions, *so the cost follows the fields a record names,
not its bytes*. Between that unit and the reproducer's, at least **four** things differ: the field
shape (two named members, one an array, against 2^19 named scalars), the escape (2253's value goes to
C by value and through a `volatile` pointer, **and** `hero_pad_fill(&held, ...)` hands its address to
the runtime, the fourth difference, found in § 1), the construction (2253 writes its record in two
slices, `memset` then `first`; the reproducer writes every leaf), and inlining (2253 has no chain of
calls; the reproducer's `make_k` all inline into `main`). Each was varied alone below. Every reading:
`ffi/scratch/m.sh`, the build's own words, Apple clang 21.0.0, this Mac (M3, 24 GiB).

### 3a. Hold 2253's program, vary only the header's field shape

The 2253 main case's program, byte for byte, bound to a header of the same name whose struct is (a)
2253's own shape, one named word and an array, (f) that many **named scalar fields**, or (n) the
reproducer's **nested named pairs** (`r0 {a, b}`, `r_j {r_(j-1) left, right}`, the top struct `first,
second, a0 ... a_(d-1)`), generated by `ffi/three/` shell (sizes checked by `sizeof`). Units from
stage 2 builds (`ffi/units/f1-{a,f,n}12.c`), 64 KiB, 8,192 `int64_t` leaves:

| 2253's program, header shape | `-fsyntax-only` | `-O0` | `-O2` | `-O2` peak |
|---|---:|---:|---:|---:|
| (a) array, 2253's own (also at 128 MiB: 193,797,734) | 163,278,770 | 179,417,076 | 196,276,613 | 9.8 MB |
| (f) 8,192 named scalars | 670,558,697 | 897,824,061 | 807,027,413 | 21.9 MB |
| (n) nested named pairs | 940,265,405 | 1,025,502,482 | 1,036,888,812 | 20.7 MB |

**Under 2253's construction the optimiser's work is nil for every shape**; what the named shapes add
is clang's FRONT END (`-fsyntax-only` already holds it), and it is the emitter's own padding table:
the unit lists one `__builtin_offsetof` per named member at every depth (`grep -o
__builtin_offsetof | wc -l`: 2 for (a), 8,192 for (f) and (n); 8 KB, 1.29 MB and 1.64 MB of C).

### 3b. Hold the reproducer's chain, vary the field shape and the escape

The reproducer's chain written over GROUP records (`ffi/three/gen2.sh`): named halves (`struct g_j {
g_(j-1) left, right; }`) and **array halves** (`struct g_j { g_(j-1) h[2]; }`, `g0 { int64_t v[2]; }`,
built `G_j(h: [make(n), make(n + 1)])`), beside the reproducer itself (program records), each with
and without one escape of `main`'s result (`hero_pad_fill(&h0_r, ...)`, which the emitter writes
after a group record's binding, removed by `perl`; or `hero_slot_escape(&h0_r)` added to the
reproducer). The optimiser's share is `-O2` minus `-fsyntax-only`. At 64 KiB (k = 12):

| the chain at 64 KiB | front end | `-O2`, no escape | `-O2`, one escape |
|---|---:|---:|---:|
| named halves, program records (the reproducer) | 187,893,432 | 3,138,197,352 / 3,150,744,528 | 9,563,154,854 |
| named halves, group records | 2,659,041,422 | 5,742,373,106 | 12,184,850,667 |
| **array halves**, group records | 191,421,917 | **9,937,708,871** | 9,940,468,254 |
| named halves, program records, `noinline` on every `make` | | 317,972,356 | |

At 1 MiB (k = 16), one compile at a time:

| the chain at 1 MiB | `-O2` instructions | `-O2` peak footprint |
|---|---:|---:|
| the reproducer (front end alone: 515,039,270) | 65,668,621,224 | 2,778,663,264 |
| the reproducer, **one escape of its result** (`hero_slot_escape(&h0_r)`) | **442,799,714,041** | **6,956,357,656** |
| **array halves**, group records, with the emitter's escape | **446,864,711,210** | **6,979,000,368** |
| **array halves**, group records, that escape removed | **446,461,451,704** | **6,970,054,704** |
| the reproducer, `noinline` on every `make` | 706,939,790 | 10,486,168 |

What this separates, each line a measurement above:

1. **The array is not what made 2253 cheap.** Built the reproducer's way, array halves cost the
   optimiser as much as named halves with an escape, three times the named reproducer at 64 KiB and
   6.8 times at 1 MiB, with 2.5 times its memory. The inference *the cost follows the fields a record
   names, not its bytes* is false for clang's optimiser.
2. **The escape is not what made 2253 cheap either, and it makes the chain worse**: one escape of the
   final value triples the optimiser's work at 64 KiB and multiplies it by 6.7 at 1 MiB (442.8 against
   65.7 billion), memory 2.78 to 6.96 GB. `byte_copy.hero:50-52` records the same sign on an
   `i64`-only chain (28 s plain, 48 s through the identity, the Windows box). **So group records of
   the reproducer's shape are hit harder than program records, because the emitter escapes every
   group record's binding** (panel 215's padding fill): at 64 KiB the named group chain costs 12.18
   billion against the program chain's 3.15, and removing that one line brings it to 5.74 (its
   front end 2.66 of it).
3. **What made 2253 cheap is its construction and its lack of a chain**: written in two slices
   (`memset`, one store) and never returned through inlined calls, any field shape costs the optimiser
   nothing (3a). Without inlining, the reproducer's chain costs 0.71 billion at 1 MiB (the critic's
   R6, re-measured with the build's words).
4. **The array chain dies at `-O0` too**: built by stage 2 at `-O0` at k = 16 it prints `before`
   and `panic: stack exhausted in chain.make16`, as the reproducer does (the critic).

### 3c. A cost that DOES follow named members: the compiler's own, not clang's (a finding beside the defect)

Family 3a at 1 MiB could not be measured as planned. The 2253 program bound to the nested-named header
(131,072 named leaves, `ffi/three/f1c/`) **did not finish `heroes build -O2` in 600 s**, nor in a
second run bounded at 150 s (`exit 124`, both), and no clang was running: `ps` showed `heroes-next`
alone, 1:09 of CPU in 1:10, 1.55 GB resident. `sample` of it after 20 s of CPU
(`ffi/scratch/f1c.sample.txt`): every working frame under `clirounds.round` -> `clirounds.lock_told`
-> `clilocks.found` -> `clilocks.asked_kept` -> `clidigest.digest`, the lock screen of panel 215's
R2 (`selfhost/cli/locks.hero`: *every partial record ... is read from clang's record-layout dump,
every member at every depth by its C path*). The 2253 array header of the same size built at once.

The compiler's OWN instructions (`timeout <s> /usr/bin/time -l ./heroes-next build <prog> -O2`,
`ffi/scratch/hb.sh`; `time -l` counts the direct child and not clang's `cc1`, the critic's
measurement), the same 2253 program, three header shapes, sizes in leaves:

| leaves (bytes) | array | flat named | nested named | nested peak |
|---|---:|---:|---:|---:|
| 128 (1 KiB) | 750,601,214 | 927,774,976 | 1,120,328,939 | 6.2 MB |
| 512 (4 KiB) | 750,632,070 | 1,609,294,246 | 2,481,962,808 | 14.0 MB |
| 2,048 (16 KiB) | 750,221,741 | 6,133,281,609 | 9,991,874,868 | 32.7 MB |
| 8,192 (64 KiB) | 984,175,170 | 58,425,780,110 | 80,396,699,963 | 146.5 MB |
| 131,072 (1 MiB) | built | | **past 600 s** | |

`-O0` pays the same (nested, 2,048 leaves: 12,915,722,981 at `-O0`), so it is not an optimisation
level's cost. Each quadrupling of named members multiplies the compiler's work by 8 to 9.5 at the
top of the table. The group chains of 3b show the same rule without `partial`: named halves at k =
12 cost the compiler 9,473,296,024 instructions and give a 4.24 MB unit with 16,382 `offsetof`
entries; array halves 1,066,653,679 and 42 KB with 116. **So the field shape a header names is what
the Heroes compiler's own lock screen and padding table pay for, and clang's front end after it; it
is not what clang's optimiser pays for.** A real header whose struct holds many members by value at
depth (a generated register map, a protocol state, any large named C struct bound `partial`) builds
in minutes or not at all, at every level. This is not defect 2278 and needs no route of this
sitting; it is filed by the coordinator or not, as `adjacent`, with this table as its reproducer
(the programs are `ffi/three/sc/{a,f,n}{6,8,10,12}/held.hero` and `ffi/three/f1c/held.hero`).

## 4. The other routes at the boundary

### The byte-array route: the boundary holds only with the held group record's alignment

A program record past the threshold written as one array of bytes is the program's own business
until it holds a group record that C is lent. A byte array's alignment is 1, so a byte-array record
nested in another is placed where C's rules put a char array, and a lend of the group record inside
hands C that address. Compiled (`ffi/units/al-routes.c`, `ffi/bind/alabi.h`: `struct al_state`
aligned 16 as this Mac's `struct __darwin_arm_neon_state64` is, 528 bytes, alignment 16, measured,
while `max_align_t` is 8 here; `al_init` out of line in a library built with
`-fsanitize=alignment -fno-sanitize-recover=all`), the emitter's shapes for `record Box (tag: i8, st:
AlState)` inside `record Outer (flag: i8, box: Box)` and the lend `@o.box.st`:

| variant | `st`'s offset, address mod 16 | exit |
|---|---|---|
| 1, today's structs | 32, 0 | 0 |
| 2, byte arrays at C's own offsets (`offsetof` on a shadow struct), a char array's alignment | `runtime error: member access within misaligned address 0x00016d1862d1 for type 'al_state' ... which requires 16 byte alignment`, in C's own code | **134** |
| 3, the same with `_Alignas(struct al_state)` on the bytes | 32, 0 | 0 |

So the route keeps the boundary only by writing `_Alignas(<the strictest group record it holds>)` at
every level where byte arrays nest, which C11 lets the emitter write with no layout read. Without the
sanitiser an arm64 scalar load forgives the address; an aligned vector or atomic access, or another
target's aligned move, need not (an inference, unrun on x86-64). And § 3b measured that the array
shape does not lower the optimiser's cost, so the route buys nothing for defect 2278 at this price.

### R3, the record in the heap: no ABI change, and the frame stays at every by-value crossing

`ffi/units/r3-box.c` at `-O2`: a C result can go straight into the box (`big_make` receives the heap
pointer as its `sret` slot, no stack copy); **a by-value argument from the box needs the whole record
in the caller's frame** (`alloca %struct.big`, a 262,144-byte `memcpy`, then `big_sum`), since the
callee owns its copy. So boxed group records pay a full-size copy at every by-value crossing and
remove no frame there: marshalling at the boundary, the cost §1.11 names in rejecting level 3. The
sketch of a callback boxing its parameter was inconclusive (clang elided its local `malloc`/`free`
pair). R3 is outside the lane (spec § 3, *a record holds its fields by value*).

### R4, a refusal past a size: no C to compile; a refusal that reaches a group record is a veto

R4 changes no C; it removes the program. If its threshold counted a group record, `record Held tag
heroes_held partial` (128 MiB, `tests/golden/run/fixedbugs-2253-*`, three committed cases) would be
refused, and with it any header whose struct passes the threshold: a C library made unbindable,
which design.md §1.12 calls the boundary failing (*any C library must be bindable*). The largest
structs measured on this Mac's headers: `struct statfs` 2,168 bytes, `struct utsname` 1,280, `struct
dirent` 1,048, `struct __darwin_arm_neon_state64` 528, raylib 6.2's `VrStereoConfig` 304 (passed and
returned by value by three entry points; bound with every field named and run against C,
`ffi/bind/vr.hero`, the same three values), `Model` 136. R4 is outside the lane.

### A bigger stack: no C change, and no foreign thread to reach today

A Heroes callback C runs on a thread of its own is refused at its entry, at both levels
(`ffi/bind/cthr.hero`, a C library creating a pthread and calling back): `panic: cthr.heavy ran on a
thread this program did not start ... (design.md Part 7.13)`. So a stack the runtime sizes covers
every thread a callback can run on today; the day Part 7.13 lets C's threads call back, the frame of
a callback is set by C, and only a route that shrinks the frame reaches it.

## 5. The two routes that pass the boundary, measured for the frame as well as the compile

R6 (`noinline`) and R7 (construction in place) are the routes whose C is ABI-neutral at the extern
(§ 2). Their frames, run (each unit compiled with the build's words, linked against the build's
library and runtime, `timeout 60`):

| shape | `-O2` instructions | `-O2` peak | runs at `-O2` | runs at `-O0` |
|---|---:|---:|---|---|
| the reproducer, k = 16 | 65,668,621,224 | 2.78 GB | yes | **no**, stack exhausted (the critic) |
| the reproducer, k = 18 | 297 to 307 billion (the critic, the coordinator) | 12.29 GB | yes | **no** (the critic) |
| R6, `noinline` on every `make`, k = 16 | 706,939,790 | 10.5 MB | yes | |
| **R6, k = 18** (`ffi/units/bf18-noinline.c`) | (1.86 billion, the critic) | | **no: `panic: stack exhausted in bigframe18.make18`, exit 134** | |
| R6 on the group-record array chain, k = 16 | 485,283,331 | 12.8 MB | yes | |
| R7's shape, every `make` writing its caller's slot, k = 16 (`bf16-r7.c`) | 5,945,285,843 | 225 MB | yes | **yes** |
| R7's shape, k = 18 (`bf18-r7.c`) | 22,715,898,904 | 820 MB | yes | **yes** |
| **R7's shape and R6, k = 18** (`bf18-r7-noinline.c`) | **539,822,425** | **10.3 MB** | **yes** | |

R6 alone turns today's slow but correct k = 18 run into a stack abort at `-O2`, which is the brief's
robustness question answered for R6 (design.md §1.12). Construction in place keeps every size
running at both levels; with R6 it costs clang less at k = 18 than the critic's `-O0` reading (1.97
billion). (My first k = 18 `-O0` run of R7's shape aborted in `main`, whose frame still declared my
rewrite's leftover `t3`, 4 MiB; with that declaration removed, as an emitter would not write it, it
runs. The `unused variable` warnings of the rewrites are the same leftovers.)

**So the route this sitting is most likely to need is the one that moves C signatures**, and § 2's R7
conditions are its price at the boundary: function values and thunks keep C's convention, a value
adapter per program function of a large-record result whose address is taken, and one
`_Static_assert` per thunk. Unrun and owed by an R7 lane: that a group record built in place still
has its padding written zero before C reads it (panel 215's R1; today `hero_pad_fill` runs at the
binding and at the lend, which R7 moves no further than the slot).

## 6. The threshold's unit, from the three-way

- **clang's optimiser** pays for the slices a construction writes into one aggregate once the chain
  is inlined: the same for named and array halves (§ 3b), nothing for any field shape written in two
  slices (§ 3a), multiplied by an escape (6.7 at 1 MiB) and removed by `noinline`. For a program
  record those slices are its scalar leaves, a function of the type alone, since a program record
  cannot hold a fixed array (`error[fixed_outside_a_group]`, measured) and every construction names
  every field; for the reproducer's `i64` leaves, bytes / 8.
- **the stack** pays for bytes (§ 5).
- **the Heroes compiler and clang's front end** pay for a group record's named members at every depth
  (§ 3c), superlinearly.

So the unit is **bytes** for my seat, with one constraint and one widening. The constraint: a group
record's size comes from `sizeof`, which the build's layout read already has, and **never from a walk
of its members**, which § 3c measured past 600 s at 131,072 nested members. The widening: **the
threshold applies to group records as much as to program records**, since the emitter's escape after
every group-record binding makes them the worse case (§ 3b: 12.18 against 3.15 billion at 64 KiB; the
array chain 446.9 billion at 1 MiB). A count of scalar leaves would predict the optimiser better for
program records (bytes / leaf width, within a factor of 8 between `i8` and `i64` leaves) but needs that
member walk for a group record. Nearest each side: every real header struct this seat measured is at most 2,168 bytes (`struct
statfs`), and the reproducer already costs 3.15 billion instructions at 64 KiB. **The programs of
`tests/`, `examples/` and `selfhost/` nearest the threshold were not enumerated**: no instrument
computes a record's size (the critic's search) and this seat ran no census, so where the tree's
largest record stands is a question, not a premise.

## 7. Verdict

- **verdict**: **object** to the routes as briefed, with a **veto** on two shapes and an **approve** for
  one. **Veto (a)**: R7 written so that a thunk or a function value takes the out-parameter
  convention (R7a refuses correct programs; R7c passes clang and dies at exit 134), or written
  without one `_Static_assert` per thunk tying its type to the typedef the probe checks. **Veto (b)**:
  R4 if its threshold counts a group record. **Object**: R1 as a cure (its escape multiplies the
  chain's cost, § 3b), R6 alone (k = 18 aborts at `-O2`, § 5), the byte-array route (no gain, an
  alignment obligation, § 4). **Approve**: R7 in R7b's shape with the per-thunk assertion, paired
  with R6, the threshold in bytes from `sizeof`, applied to group records as to program records.
- **section**: design.md §1.11 (level 1 of interoperability, no marshalling), §1.12 (*any C library
  must be bindable*; must not segfault), §4.19 (*Clang verifies the declared signature against the
  real header*, and its callback paragraph: *the emitter must never insert a cast from `ptr` to a
  header's function-pointer type (it compiles clean and silently calls the wrong callback)*). **The
  document does not cover thunks** (`grep -i thunk docs/design.md` finds nothing): the check R7
  needs is not written anywhere.
- **experiment**: § 1 to § 5. `bigabi.h` (a 256 KiB struct by value both ways, held, lent, two
  callbacks) and `extval.hero` (an extern as a function value), each route's C compiled by clang
  against the real header with the build's words and linked against the out-of-line library: R1,
  R2, R5, R6, R7b accepted, every extern declaration and thunk lowered identically, output equal to
  C's; R7a refused by clang; R7c accepted and exit 134; the per-thunk assertion refuses R7c and
  passes today's unit and R7b. raylib 6.2's `VrStereoConfig` bound and run against C.
- **argument**: Only group records cross, so only callbacks and held group records expose a route to
  C. Every route keeps the extern's ABI but one: R7 splits a function's convention from its value's
  and its thunk's, and `hero_callback_of`'s cast through `void (*)(void)` hides a split from clang,
  the silent-wrong-callback class §4.19 forbids; one assertion per thunk restores the check. R7 is
  also the only route measured to keep every size running at both levels, so it must land with that
  assertion. The three-way falsifies *cost follows named fields*: arrays cost clang's optimiser as
  much; named members cost the Heroes compiler itself (§ 3c).
- **prediction**: an R7 lane whose thunks follow `callback_thunk.hero`'s current rule (*the
  function's own signature*) builds `ffi/bind/bigabi.hero` with clang silent and the program exits
  134 at `big_apply_ret`; with one `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk),
  <typedef>))` per thunk the same build is refused at compile time; and in R7b's shape
  `ffi/scratch/abi.sh` prints the same lines for `bigabi` as today's, while raylib 6.2 (largest
  struct 304 bytes) is untouched by any byte threshold of 4 KiB or more (`examples/raylib`'s unit
  identical but for `hero_program_sources`). Instruments: the lane's compiler on `bigabi.hero` and
  `extval.hero`, `abi.sh`, the net's `emission` suite.
- **condition**: the veto (a) lifts if R7 keeps ONE C signature for a function, its value and its
  thunk (no split, nothing to assert), or if the emitter is shown to derive each thunk from the very
  typedef the probe checks so the two cannot diverge. The approve turns to a veto if `abi.sh` shows any
  extern's lowered declaration move on a leg this seat did not run (x86-64 Linux, Windows), or if a
  committed callback case changes output. Veto (b) lifts if R4 exempts group records. I would accept
  R6 alone if it is shown to run k = 18 at `-O2` on every leg (a bigger stack beside it).

## 8. What this seat did not run

x86-64 and Windows lowering of any unit (the Linux arm64 image has no `/usr/bin/time`, the critic);
the R7 lane's real emitter (every R7 unit here is a hand rewrite of stage 2's emission); R2 on the
reproducer's frames; the one-function shape under R7 (the critic measured R6 not to reach it); a
census of record sizes in the tree; the routes' line counts as diffs (the modules they would touch
stand today at `callback_thunk.hero` 273, `callback_guard.hero` 207, `signature.hero` 257,
`construct.hero` 278, `aggregate.hero` 351, `byte_copy.hero` 120 lines, `wc -l`; what R7b adds per
function of a large-record result whose address is taken is a 5-line value adapter and per thunk one
line of assertion, counted in `ffi/units/bigabi-R7b.c` and `bigabi-R7b-asserted.c`).
