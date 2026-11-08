# Panel 236, the ffi-pragmatist's report: a group record off the frame, at the C boundary

Written as it went, 2026-11-08, in `.claude/worktrees/236-ffi-pragmatist/` (detached at `5bd4cf3e4`),
every file this seat made under `.claude/worktrees/236-ffi-pragmatist-scratch/` (`$S` below). Cost
is counted in instructions retired (`/usr/bin/time -l`), never seconds. Every number carries the
command that produced it; a sentence that was not run says so. The seat was stopped once by the
account's limit and resumed at 03:10 from this file.

## 0. Setup

- Stage 1, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `sources 1a1e340778db789a`.
  Stage 2, `./heroes build selfhost/main.hero -o heroes-next`: **`sources 2fa3d81374af3684`**, the
  brief's base key. Apple clang 21.0.0, arm64, this Mac.
- The build's words, read from what the build itself wrote (`build/words/*.rsp`,
  `build/<key>/link-words.rsp`): a unit is `clang <flags()'s nineteen words> -O0 -g | -O2
  -gline-tables-only -I <header dir> -I runtime -I <program dir> -c <unit>.c`, and the link is
  `clang <the same words and level> -rdynamic -Wl,-reproducible runtime.o <unit>.o library.o
  -L<dir> -l<lib>`. The program's unit is byte-identical at both levels (`cmp`).
- **A binding whose C calls back.** The critic's headers define `apply_held`, `apply_ret` and
  `apply_named` as `(void)cb; return n;`, so no callback is ever called and no byte crosses. This
  seat wrote `$S/bind/cbabi.h` and `cbabi.c` (out of line, `libcbabi.a`, so every call crosses the
  platform ABI): the same `heroes_held` (`partial`, 256 KiB by default so C itself can make the
  call on every leg; `-DCB_WORDS='(1<<24)'` is 2253's 128 MiB) and `big_named` (4,800 named
  bytes); `apply_held` and `apply_named` build one, pass it by value, and return `-1` unless the
  callback's hash of what it received equals the hash of what C sent; `apply_ret` hashes every
  byte the callback returned; `held_make` is an extern returning the record by value. The four
  programs are the critic's shapes over it, `$S/progs/x{cbparam,cbresult,cbnamed,extval}.hero`,
  and `$S/bind/ref.c` is each in C alone; its outputs are `$S/out/<prog>.ref`.
- **Instruments.** `$S/tools/run.sh <prog> <level> <unit.c>`: the unit compiled with the build's
  words, linked against the very `runtime.o` and `library.o` the build linked for that program and
  level, run under `timeout 60` through `head -c 65536`, compared with the C reference.
  `$S/tools/frames.sh`: the same compile with `-fstack-usage`. `$S/tools/abi.sh`: panel 233's
  instrument, the unit at `-O2` as LLVM IR, every extern's `declare`, every call to one and every
  thunk's `define`, names and inferred attributes stripped. Calibration: the base's own units
  through `run.sh`, all four programs, **SAME as C at `-O0` and `-O2`**, as `heroes build`'s own
  binaries are.

## 1. Veto (a) for group records: the C of each route, compiled and run against C that calls back

Each variant is the build's own unit rewritten by anchored rules (`$S/tools/variants.py` over
`rw.py`, every anchor asserted to match exactly as often as intended), so a route's C differs from
the base's only where the route says. `P` is **the proposal as written**: a group record of the set
takes `hero_out` by name and through a value, the function type gets a second typedef for Heroes
values (`h_0fv_*`, `void (*)(struct heroes_held *, int64_t)`) beside the base's (`h_0fn_*`, kept
byte for byte for the probe and the crossing), an extern used as a value is wrapped in a `hero_out`
adapter (`h_0xv_held_make`) which `hero_callback_of` maps back to `held_make`, and a Heroes function
reaching C goes through a C-convention adapter (its thunk). `R7b`: one typedef, C's; values and
thunks keep C's convention; a value adapter (`h_0cv_*`) per address-taken function; only a call by
name takes `hero_out`. `R7c`: the thunk keeps the function's own converted signature (the negative).
`-a`: one `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk), h_0fn_*))` per thunk.
`Pconst`: P with the parameter spelled `const T *`, as `large_value.hero` spells a program record's.
`Pmissing`: P with the table's line for the extern's adapter left out. `Ptyped`: P where the generic
`hero_callback_of` is replaced, for this function type, by a typed crossing
`static h_0fn_X hero_cross_X(h_0fv_X f)` returning each twin and ending in `hero_panic`.

Run: `$S/tools/run.sh`, both levels, 256 KiB (`$S/out/runs-arm64.txt`):

| program | P, P-a | R7b, R7b-a | R7c | R7c-a | Pconst | Pmissing (and -a) | Ptyped | R7c + typed crossing |
|---|---|---|---|---|---|---|---|---|
| `xcbresult` (callback returns it) | SAME / SAME | SAME / SAME | **exit 134** both levels, clang silent | **refused**: `static assertion failed ... '__builtin_types_compatible_p(void (*)(struct heroes_held *, long long), struct heroes_held (*)(long long))'` | | | | |
| `xcbparam` (callback takes it) | SAME / SAME | SAME / SAME | **exit 0, SAME as C**, clang silent | **refused** | **refused**: `passing 'const struct heroes_held *' to parameter of type 'void *' discards qualifiers` | | | |
| `xcbnamed` (4,800 named bytes) | SAME / SAME | SAME / SAME | **exit 0, SAME as C** | **refused** | **refused**, the same message | | | |
| `xextval` (extern and Heroes function in one type) | SAME / SAME | SAME / SAME | **exit 134** both levels | **refused** | | **exit 134** both levels, **every assertion passes** | SAME / SAME | **refused**: `incompatible function pointer types returning 'void (struct heroes_held *, int64_t)' ... from a function with result type 'h_0fn_2e1c08ea'` |

Read row by row:

1. **Both routes keep the boundary when written right**: every byte C received equals what the
   Heroes side wrote (C's own hash comparison, and the hash of the returned record), both levels,
   all four shapes, the assertion passing on both.
2. **R7c's parameter direction runs correctly on this Mac.** `int64_t (*)(struct heroes_held *)`
   called as `int64_t (*)(struct heroes_held)` reads the right bytes, because arm64 passes a
   composite over 16 bytes as a pointer to the caller's copy in `x0`, exactly where the wrong thunk
   looks. Only the result direction (`x8` against `x0`) shows on a run here. The assertion refuses
   both directions at both levels. (Other ABIs: § 3.)
3. **The proposal opens a hole the per-thunk assertion does not close.** Under P an extern stored
   as a value is not its own address but its adapter's, so `hero_callback_of`'s last line, `return
   f;`, which is right for every extern today, hands C a `hero_out`-convention function the moment
   the table misses one adapter: `Pmissing` compiles with every assertion passing and dies at both
   levels (`panic: a null pointer was read through, at offset 0x2`: C's `n = 2` taken as the slot;
   an argument that happened to be a writable address would take the 256 KiB write in silence,
   an inference, unrun). R7b has no such adapter and no such line to miss.
4. **A typed crossing per converted function type closes it by construction**: each `return` is
   checked by clang against the typedef the probe checks (`R7c + typed crossing` refused at both
   levels), each comparison against the Heroes typedef, and a value with no twin panics instead of
   crossing. `Ptyped` runs SAME at both levels.
5. **`const T *` cannot carry a group record that is lent**: panel 215's padding fill takes `void *`.

**Found beside the sitting, on the base (unfiled; five searches of the issues, `hero_pad_fill
discards qualifiers`, `const pad_fill parameter lent`, `discards qualifiers`, `hero_pad_fill`,
`large parameter lent`, found nothing; issue 1334 is the fill's origin, not this).**
`$S/progs/xlendparam.hero`, a large program record holding a `partial` group record, handed by
plain parameter (panel 233's R2, `const h_xlendparam_Wrap *`), whose group field the callee lends
read-only to C (`held_hash(h: w.inner)`): **`heroes build` exits 2 at both levels**, `internal
error: compiling the generated C failed: ... passing 'const struct heroes_held *' to parameter of
type 'void *' discards qualifiers`, on `hero_pad_fill(&h0_w.f_inner, ...)`. A correct program
refused with an internal error: `blocking` by `.claude/rules/verification.md` § Bounded discovery.
The proposal widens it to every plain parameter of a group record in the set that is lent.


**C's view, byte for byte.** Under P, R7b and P-typed, on all four programs: the `h_0fn_*` typedef
and every `hero_ffi_probe_*` line are the base's `--emit-c` byte for byte (`grep` and `md5`); P adds
one `h_0fv_*` typedef per converted function type, which no probe and no crossing ever names; and
`$S/tools/abi.sh`, every extern's lowered `declare`, every call to one and every thunk's `define`,
is the base's (**SAME**, all nine pairs). The thunk's `-O0` definition is the base's to the
attribute (read on `xcbresult`); at `-O2` only optimiser-inferred attributes differ (`captures(none) initializes((0,
262144))` against `captures(address_is_null)`). The instrument's first reading said DIFFERENT for
`xcbresult` and `xextval`; that was its regex leaving a `)` of the nested `initializes((...))`
behind, repaired and re-run, and both readings are in this paragraph.

### The adapters' frames (`$S/tools/frames.sh`, `-fstack-usage`, 256 KiB, `$S/out/frames-routes-256k.txt`)

| function | base `-O0` / `-O2` | P `-O0` / `-O2` | R7b `-O0` / `-O2` |
|---|---:|---:|---:|
| `xcbresult`: thunk (C calls it, record returned) | 32 / 64 | 32 / 48 | 32 / 48 |
| `xcbresult`: R7b's value adapter | | | 32 / 48 |
| `xcbresult.main` (`h = make(n: 5)` by name) | 262,304 / 262,224 | 128 / 48 | 128 / 48 |
| `xcbparam`: thunk (record passed by value) | **262,208 / 262,176** | 32 / 32 | 32 / 32 |
| `xcbnamed`: thunk (4,800 named bytes by value) | **4,864 / 4,832** | 32 / 32 | 32 / 32 |
| `xextval.main` (two calls through a value) | 524,672 / 262,256 | **352** / 262,224 | 524,672 / 262,256 |
| `xextval`: P's adapter of the extern `held_make` | | **524,368 / 262,176** | |

- Clang gives C's `T r; f(&r, n); return r;` NRVO: every C-convention adapter, P's thunks and R7b's
  thunks and value adapters alike, is 32 to 48 bytes at both levels.
- **Today's thunk copies a by-value parameter on its own frame**, the whole record, at both levels
  (`h_0cb_xcbparam_first(h0_h)` hands `h0_h` on by value). Both routes remove that copy: the thunk
  passes its own parameter's address.
- **Through a value**, P keeps `main`'s frame at 352 bytes at `-O0` where R7b keeps the base's two
  temporaries; at `-O2` P's `main` is 262,224 again: it calls `held_make` twice directly
  (`$S/out/xextval-P.ll`), the extern's adapter and `pick` inlined into it.
- **P's adapter of an extern holds the record**: an extern's by-value result needs a slot in its
  caller's frame, and the adapter is that caller. Four spellings measured (`$S/targets/xv.c`): the
  two-branch adapter 524,368 / 262,176, one call into the slot or a room region 262,240 / 262,192,
  `restrict` on the slot 262,208 / 262,176, the initialisation `T h = f(n);` 262,192 / 262,176. **No C
  spelling lets an extern write its by-value result outside its caller's frame, at either level**,
  so P moves `extval`'s abort from `main` into the adapter and does not remove it when the value
  holds the extern.

## 2. Where the two routes part: a call through a value (part (b)'s witness, robustness)

`$S/valret/xvalret.hero`: `grpnoext`'s call made through a function value, `f = make; h = f(7)`, 2253's
128 MiB record, no extern called and no callback, so no crossing is involved and only the value's
convention decides. Base, P and R7b written as in § 1 (`$S/tools/variants_valret.py`), run through
`run.sh`, and P and R7b under `--sanitize` too, linked against the build's own sanitized runtime and
library objects (`$S/out/san-other.txt`):

| route | `go`'s frame `-O0` / `-O2` | run, both levels | `--sanitize`, both levels |
|---|---:|---|---|
| base | 134,217,856 / 134,217,792 | `before`, `panic: stack exhausted in xvalret.go`, 134 | not run |
| **P** | **96 / 48** | **`before`, `7`, exit 0, SAME as C** | exit 0, SAME |
| R7b | 134,217,856 / 134,217,776 | `before`, `panic: stack exhausted in xvalret.go`, 134 | `AddressSanitizer: stack-overflow ... in h_xvalret_go xvalret.hero:16`, 134 |

R7b's value adapter is small (32 / 48 bytes, NRVO), and its **caller** holds the slot, as for any
by-value C return. So **R7b leaves a correct program that calls a large-result function through a
value aborting at both levels**, where P runs it in the room; at the boundary both are byte for byte
C's (§ 1). That is CLAUDE.md § Precedence rank 3 deciding between two routes the boundary cannot tell
apart, provided P's crossing is the typed one (§ 1, rows 3 and 4).

## 3. Targets: which rules hold the same C on the four CI legs

All by clang's own `-target` on this Mac (`$S/targets/`), header-light so every target compiles.

**Sizes** (`sizes.c`, `sizeof` and `_Alignof` read off the IR): `heroes_held` 134,217,728,
`big_named` 4,800, the union `ub` 2,400 and its holder 2,408, and defect 2334's `big` 8,192 with
alignment 1, **the same on `arm64-apple-darwin`, `x86_64-pc-linux-gnu`, `aarch64-unknown-linux-gnu`
and `x86_64-pc-windows-msvc`**. Two shapes differ, and neither can be one Heroes text on all four:
`long a[1024]` is 8,192 on the three LP64 targets and **4,096 on Windows**, and design.md's
correction of 2026-11-06 (panel 234's R10) says a C `long` **field** has no spelling the four legs
share, so its binding is `i64[1024]` on three and `i32[1024]` on one, two programs counted 8,192 and
4,096; `char; long double` is 16 on Apple arm64 and MSVC and 32 on both Linuxes, and a `long double`
has no Heroes type, so it is left to `partial`, whose membership asks no size. **So a count of named
fields gives one program one answer on every target, and the emitted C is the same on all four.**

**Defect 2334, the coordinator's question** (its two files copied read-only into `$S/d2334/`): the
record is `uint8_t f0[8192]`, counted 8,192 by `bytes_of` and 8,192 by `sizeof` on all four targets
(above), so **a rule counting named fields moves it the same way on LLP64 as on LP64**: past `LARGE`,
into the room, the same C on every leg. What differs on Windows is the stack the frame is judged
against, not the rule: `total`'s frame is 8,956,096 bytes at `-O0` and 245,888 at `-O2` here
(`-fstack-usage`), `panic: stack exhausted in big8k.total` at `-O0` against Darwin's 8 MiB; Windows
links 64 MiB, so the defect's abort may not reproduce on that leg at all (arithmetic, unrun: the
box answered at 01:01 and not from 03:20, `connect to host apponfly-vps port 22: Operation timed
out`).

**Acceptance** (`boundary.c`, every route's boundary C for the three callback directions and the
extern stored as a value, with and without the assertions, `$S/targets/accept.txt`): **identical on
the four targets**. P, R7b and P-typed accepted with and without the per-thunk assertion; R7c
accepted in silence without it and refused with it; R7c under typed crossings refused without it.
The assertion is a type-level fact, so it answers the same on every leg.

**Lowering** (`boundary.c` at `-O2`, each thunk's `define`, C's against R7c's):

| target | result direction | parameter direction |
|---|---|---|
| `arm64-apple-darwin`, `aarch64-unknown-linux-gnu` | `sret` (`x8`) against a pointer (`x0`): differs | `ptr` against `ptr`: **identical** |
| `x86_64-pc-linux-gnu` | `sret` against `ptr`, both in `rdi` | `byval` (the stack) against `ptr` (`rdi`): differs |
| `x86_64-pc-windows-msvc` | `sret` against `ptr`, both in `rcx` | `ptr` against `ptr`: **identical** |

Read from each thunk's assembly at `-O2` (`$S/targets/b-<target>-{1,3}.s`): on both arm64 targets
C's thunk takes the slot from `x8` (`mov x19, x8`) and R7c's from `x0`; on x86-64 System V both
take it from `rdi`, C's returning it in `rax` as that ABI asks (`movq %rbx, %rax`); on Windows both
take it from `rcx` and `n` from `rdx`, C's again returning the slot in `rax` (`movq %rsi, %rax`).
And clang's own caller never reads `rax` after an `sret` call on either x86-64 convention (`caller.c`
at `-O0` and `-O2`: it reloads its own slot, `leaq -262160(%rbp), %rdi`, `movq %rbx, %rdi`, `leaq
40(%rsp), %rcx`, `movq %rsi, %rcx`). So against a clang-built C library a wrong thunk would run
correctly in the result direction on both x86-64 legs, and **on Windows in neither direction would a
run show it**: an inference from the lowering and the caller's code, run on arm64 only. The model
that settles it in one command, `$S/targets/r7c_model.c` (no runtime; `-DCC='__attribute__((sysv_abi))'`
runs System V's convention on the box's own CPU), reproduces the units here: `ok-ret 5009`, `ok-par
5007`, `r7c-ret` exit 139, `r7c-par 5007` at both levels. **Owed on the box** when the author starts
it, and on Linux by the CI: no x86-64 image of this project stands on this Mac, and running a
container would write outside the repository's root.

## 4. Panel 215's R1 where a record crosses through `hero_out` or a pointer and C reads it

`$S/pad/pad.h`: a `partial` record with fourteen bytes of padding (`int8_t tag; int64_t v; int8_t
tail; int64_t body[1000];`, 8,024 bytes); `pad_count` counts nonzero padding bytes; `apply_pad` calls
the callback twice into **one** result slot (the library built at `-O0`), the first time through a C
function that fills every byte with `0xAA`, so a callback that does not write the padding leaves
`0xAA` for C to read. **Negative control, live**: a C callback writing the fields alone reads **14**
at both levels of the caller (`$S/pad/control.c`). `$S/progs/xpad.hero` crosses three ways: returned
by name into the caller's slot and lent; returned through the callback with **no fill between** the
Heroes construction and C's read; and passed by C with its padding `0xAA`, lent back from the
callback. The routes' C: `$S/tools/variants_pad.py`.

| crossing | base `-O0` / `-O2` | P `-O0` / `-O2` | R7b `-O0` / `-O2` |
|---|---|---|---|
| by name through `hero_out`, then lent | 0 / 0 | 0 / 0 | 0 / 0 |
| through the C-convention thunk, read by C | 0 / 0 | 0 / 0 | 0 / 0 |
| from C by value (padding `0xAA`), lent back | 0 / 0 | 0 / 0 | 0 / 0 |

**R1 holds on every crossing, and by construction**: at `-O2` the callee's body is a `memset` of the
whole region (8,024 bytes), the three field stores, then one 8,024-byte `memcpy` into `hero_out`
(`$S/out/xpad-P.ll`), so the slot receives every byte, padding included, from a region written zero
whole; the third row is the fill at the lend. It rests on one fact a route must keep: **a record of
the set is copied whole, memory to memory, never through a frame temporary clang may split**, which
is what the room gives. (The unit's construction writes the region zero twice, `t4 = *(T
*)__builtin_memset(&t4, ...); __builtin_memset(&t4, ...);`, collapsed to one at `-O2`; at `-O0` it is
two passes over 128 MiB for 2253's record: a cost, not a fault, and not this lane's.)

## 5. An `extern`'s own by-value parameter or result (question 4)

`$S/q4/x2253.h`, defect 2253's 128 MiB record crossing an out-of-line extern (`libx2253.a`) by value:
`held_first_v(h)` (a parameter, the critic's `grpextonly`), `held_make_v(n)` (a result) and
`held_bump_v(h, by)` (both). Built by the base, run under `timeout 60`:

| shape | `go`'s frame `-O0` / `-O2` | run `-O0` | run `-O2` |
|---|---:|---|---|
| parameter | 134,217,856 / 134,217,776 | `before`, `panic: stack exhausted in xextparam.go`, 134 | the same |
| result | 134,217,840 / 134,217,776 | `before`, `panic: stack exhausted in xextresult.go`, 134 | **no `before`**, `panic: stack exhausted in xextresult.main`, 134 |
| both | 268,435,616 / 268,435,504 | `before`, `panic: stack exhausted in xextboth.go`, 134 | the same as `-O0` |

**The C fact every rule rests on**: a by-value aggregate's storage is its **caller's**, on all four
targets, read in § 3's lowering: arm64 and Windows pass a pointer to a copy the caller makes, x86-64
System V copies it into the caller's outgoing argument area (`byval`), and every target returns it
through an `sret` slot the caller provides. One copy per crossing, two for `both`; and § 1's four
spellings show no C form puts an extern's result anywhere but its caller's frame. So no route of this
sitting changes a byte of these three runs, and the abort is spec § 9's promise kept.

**Found beside the sitting (unfiled; searched `stack exhausted inlined`, `stack exhausted names the
caller`, `before is not printed stack`; defect 2277 is a different naming fault)**: at `-O2` clang
inlined `go` into `main` for the result shape, so `main`'s frame became 134,217,776 bytes, probed at
`main`'s entry: **the program's `before` is never printed, and the panic names `xextresult.main`**,
whose source holds nothing large. A true message less exact than it could be, and output the program
wrote before the failing call lost, at one level and not the other: `adjacent` by § Bounded discovery,
the full panel's or the runtime's, not this lane's.

**Refusal or message: this seat recommends neither from this lane.** A refusal would read `sizeof`
against a stack the build does not know (a thread's, `ulimit -s`, Windows' 64 MiB link) and would
make a group record's library unbindable past a size, which panel 233's veto (b) and design.md §1.12
(*any C library must be bindable*) refuse. A run-time message naming the copy (*a C call here copies
134,217,728 bytes onto the stack*) would be design.md §4.17's kind of improvement over `stack
exhausted in xextparam.go`; it is a diagnostic's wording, so the full panel's, with these three
programs as its witnesses.

## 6. Part (a) from the C side: the 64 KiB chains, and what the routes leave beside `noinline`

Panel 233's `f2/gn12` and `f2/ga12` (its evidence archive, `gen2.sh`), extracted to `$S/chain/`.
The route's C (`$S/tools/chain_route.py`, any depth): the levels a named-field count moves, `G9` to
`G12` (8,192 to 65,536 counted bytes; `G8` counts exactly 4,096 and stays), take `hero_out` by name,
their temporaries in the room, each construction into a region zeroed whole and then field by field,
the result copied whole into the caller's slot; for the array chain both the emitter's current
fixed-array spelling (`one_field`: a compound literal of the member's type, `memcpy`'d) and one
element at a time. Cost: `$S/tools/cost.sh`, `/usr/bin/time -l clang -fintegrated-cc1 <flags()>
-O2 -gline-tables-only -c`, one compile at a time, on the `--emit-c` unit (the coordinator's
instrument) and on the build's own unit:

| unit, `-O2` | instructions (`--emit-c`) | peak (`--emit-c`) | instructions (build's unit) | front end (`-fsyntax-only`) |
|---|---:|---:|---:|---:|
| `gn12` base | 12,482,798,284 | 166,937,128 | 12,253,510,914 | 2,643,160,269 |
| `gn12` route | **3,224,286,690** | **45,941,168** | 2,972,349,148 | 2,631,358,885 |
| `ga12` base | 10,215,479,145 | 155,304,512 | 9,923,727,519 | 192,600,309 |
| `ga12` route, element by element | **685,669,938** | **14,811,568** | 453,356,816 | 193,459,990 |
| `ga12` route, `one_field`'s literal | | | 459,369,247 | |
| `ga13` route (128 KiB) | 713,514,664 | 15,155,632 | | |
| `ga14` route (256 KiB) | 746,326,736 | 15,581,616 | | |
| `gn13` route (128 KiB) | 6,061,299,824 | 82,723,320 | | 5,311,705,454 |
| `gn13` / `gn14` base, front end alone | | | | 5,379,622,814 / **10,991,815,613** (peak 146,047,504) |

The `--emit-c` base readings are within 1.0 % of the coordinator's and the critic's; the build's own
unit reads 1.4 % (`gn12`) and 1.9 % (`ga12`) under the coordinator's. Runs (`$S/chain/runs.txt`), linked
against the build's objects: every variant prints `before`, `1` at `-O0` and `-O2`. Frames:

| function | base `-O0` / `-O2` | route `-O0` / `-O2` | route, `one_field`'s literal |
|---|---:|---:|---:|
| `make12` | 196,736 / 147,520 | **128 / 64** | **65,712 / 65,616** |
| `make11` | 98,432 / 49,216 | 128 / 64 | 32,944 / 32,848 |
| `make10` | 49,280 / 36,928 | 128 / **8,288** (`make9` inlined) | 16,560 / 28,768 |
| `make9` (its halves are `G8`, under the line) | 24,704 / 12,352 | 16,528 / 8,256 | 24,752 / 12,352 |
| `main` (`ga12`: the reads copy each level) | 328,112 / 65,760 | 8,704 / 256 | 8,704 / 256 |

1. **The routes make `noinline` unnecessary for the optimiser at these sizes**: its share (the
   compile less the front end, both read on the build's own unit) falls from 9.61 to 0.34 billion
   (`gn12`) and from 9.73 to 0.26 billion (`ga12`), and the array route grows about 30 million instructions a level (686, 714, 746
   million at 64, 128, 256 KiB), where panel 233's `noinline` on the 1 MiB array chain read 0.49
   billion. No builder above the line holds more than 128 bytes at `-O0`; at `-O2` `make10` holds 8,288,
   `make9` and its two `G8` halves inlined into it.
2. **What remains on the frame** is the levels under the line: `make9` holds its two `G8` halves and
   their return slots, 16,528 bytes at `-O0`, a constant of `LARGE` whatever the chain's size; and
   **`one_field`'s fixed-array spelling puts the whole array back on the frame** (`make12` 65,712
   bytes, the record's own size): a route that reaches group records must write a fixed-array field
   of large elements one element at a time, or its frame grows with the record.
3. **What remains beside the frame decides the named chain, and no route reaches it**: panel 215's
   padding table for the bound `r` (`hero_pad_t_12[]`, one line of 4,214,704 characters naming every
   leaf at every depth) is clang's front end, 2.64, 5.38 and 10.99 billion instructions at 64, 128
   and 256 KiB, doubling per level and identical under the route (2,631,358,885 against
   2,643,160,269). At 256 KiB it alone passes panel 233's bound of 8 billion.

## 7. Verdict

- **verdict**: **adopt with conditions**, and **veto on the proposal as written if its crossing is
  `hero_callback_of`'s untyped table**. The C that breaks: `$S/units/xextval-Pmissing-a.c`, every
  `_Static_assert` passing, exit 134 at both levels, an extern's `hero_out` adapter handed to C as
  `heroes_held (*)(int64_t)` by the table's `return f;`. The conditions: (1) for P, a typed crossing
  per converted function type, its return type the very typedef the probe checks and its fall-through
  `hero_panic`, so no value of a converted type ever reaches `return f;` (`Ptyped`: SAME at both
  levels, the R7c thunk refused on all four targets); (2) one `_Static_assert(__builtin_types_compatible_p(
  __typeof__(&thunk), <typedef>))` per thunk on either route; (3) a group record's pointer parameter
  spelled without `const` (or the fill changed), since `hero_pad_fill(void *, ...)` refuses a lent
  `const T *` (`Pconst`), and the base's own refusal of that shape repaired in the same landing; (4) a
  fixed-array field of large elements written one element at a time.
- **route I would adopt** between question 3's two: **P with typed crossings**. Both keep C's view
  byte for byte (§ 1: typedef, probes, lowered boundary SAME), both remove today's thunk copy of a
  by-value parameter, both keep R1 (§ 4); only P runs a large result called through a value (§ 2:
  `xvalret` runs where R7b aborts at both levels), which rank 3 decides. Its price at the boundary:
  two typedefs per converted function type, an adapter per extern used as a value (whose frame holds
  the extern's result, as any C caller's must, § 1), and a typed crossing per converted type. R7b
  with the per-thunk assertion is the conservative route, safe at the boundary and leaving § 2's
  witness aborting.
- **experiment**: the C of P, R7b, R7c, `Pconst`, `Pmissing` and `Ptyped` for `extval`, `cbparam`,
  `cbresult` and `cbnamed` over a library whose C calls back and hashes every byte (§ 0), compiled
  with `flags()`'s words at `-O0` and `-O2` and linked against the build's own objects: P, R7b and
  `Ptyped` accepted, SAME as C at both levels and under `--sanitize`; R7c accepted in silence, exit
  134 in the result direction and silently correct in the parameter direction on arm64; the
  per-thunk assertion refusing every R7c; `Pmissing` passing every assertion and exiting 134;
  `Pconst` refused by clang; acceptance identical on four targets by `-target` (§ 3); `xvalret`
  at 128 MiB running under P and aborting under R7b (§ 2).
- **argument** (116 words): Both routes keep C's view byte for byte: the typedef C sees, every probe, every lowered declaration and thunk are the base's, and both remove today's thunk copy of a by-value parameter. They part twice. At the boundary, the proposal as written adds an adapter per extern used as a value that only the untyped table maps back, so one missing line hands C a hero_out function with every per-thunk assertion passing; a typed crossing per converted type closes that by construction on all four targets. In the language, only the proposal runs a large result called through a value; R7b aborts it at both levels. Robustness outranks the price, so: the proposal, with typed crossings.
- **section**: design.md §4.19 (*clang verifies the declared signature against the real header*, and
  its callback paragraph: the emitter must never insert a cast to a header's function-pointer type
  that *compiles clean and silently calls the wrong callback*), §1.11 (level 1, no marshalling),
  §1.12 (*any C library must be bindable*; must not segfault). The document does not cover thunks or
  adapters (`grep -i thunk docs/design.md`: nothing, as panel 233's seat found).
- **prediction**: on panel 233's 1 MiB array chain `ga16`, the route written as § 6 writes it
  (`hero_out` from `G9` up, construction element by element, no `noinline`) compiles at `-O2` with
  `flags()`'s words in **under 1 billion instructions and under 20 MB peak** (`/usr/bin/time -l clang
  -fintegrated-cc1`, the `--emit-c` unit), against 446.9 billion and 6.98 GB on the base; and the
  named chain `gn16` stays **past 40 billion, at least 85 % of it in `-fsyntax-only`** whatever the
  route, its padding table doubling per level. And on the Windows box (`$S/targets/win.sh`, owed),
  `r7c_model.exe` prints the right answer for **both** R7c directions under Microsoft x64 at both
  levels, so no run of the Windows leg can see a wrong thunk; only the assertion can.
- **condition**: the veto lifts with the typed crossing (or with R7b). This seat's adopt turns to a
  veto if any CI leg's `abi.sh`-style reading moves an extern's lowered declaration or a thunk's
  `define` (§ 3 read four targets by `-target`, ran one), if a converted function value reaches C by
  a path the typed crossing does not cover (panel 230's library twin, `signature.takes_caller`, is
  the one this seat did not build), or if the landing's thunks are written without the assertion.

## 8. What this seat did not run, and the platform runs the landing owes

- **Not run**: any x86-64 execution (the box answered at 01:01 and not from 03:20 to 03:42, `connect
  to host apponfly-vps port 22: Operation timed out`; no x86-64 Linux image of this project stands on
  this Mac and a container would write outside the repository's root); Linux arm64 by run (its
  lowering read by `-target` only); the 1 MiB chains (the compiler-engineer's); the routes in a real
  emitter (every route here is a hand rewrite of stage 2's units, anchored and asserted); defect
  2334's program on Windows (its abort under the 64 MiB link is arithmetic).
- **Owed by the landing, per CI leg**: on all four, the four callback shapes over a library that
  calls back (§ 0's `cbabi`), at `-O0` and `-O2` and `--sanitize`, compared byte for byte with C; the
  padding probe with its live control (§ 4); `xvalret` and the extern-as-value shape at 128 MiB; the
  `Pmissing`-shaped negative as a compile-time case (the typed crossing refusing it); on x86-64 Linux
  the R7c parameter direction, the one run that can show it there; on Windows `win.sh`'s model, both
  conventions, where a run shows neither direction; on arm64 legs the R7c result direction.
- **Found beside the sitting, for the coordinator to file**: the `const` padding-fill refusal of a
  large program record's lent field (§ 1, `blocking`); the inlined frame that loses `before` and names
  `main` (§ 5, `adjacent`); the named group chain's padding table in clang's front end, 10.99 billion
  instructions at 256 KiB (§ 6), defect 2284's family on clang's side.

## 9. Process, recorded

- One command of this seat wrote a 12,694-byte LLVM file to `/tmp/claude-501/x.ll`, outside the
  repository's root, at 03:26 (the first branch of an `||` I meant as a fallback); found within the
  minute and removed. Every other file of this seat is under its scratch or this worktree.
- The environment's working directory moved to `.claude/worktrees/core-b1` during the stop; this seat
  wrote nothing there and read only defect 2334's two files in `core-b1-scratch`, as the coordinator
  asked.
