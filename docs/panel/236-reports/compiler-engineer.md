# Panel 236, the compiler-engineer's report: a group record off the frame, between Heroes functions and handed to C

Written as it went, 2026-11-08, in `.claude/worktrees/236-compiler-engineer/` (detached at
`5bd4cf3e4ee1eead81577817e95dc25b3304df20`), scratch `.claude/worktrees/236-compiler-engineer-scratch/`
(the programs under `probe/` are the critic's, copied; mine are under `mine/`; every output under
`out/`). Cost in instructions retired (`/usr/bin/time -l`), never seconds. Every number carries its
command.

## Summary, for the synthesis

- **Recommended**: R7b, rule B, construction in place for group records, built and measured:
  `route-final.patch` in the scratch (16 files under `selfhost/emit/`, +279 lines by `wc -l`),
  `heroes-route3`, `sources 1699bb340bef9aeb`. Witnesses `grpnoext`, `grpgen`, `grpfix2` (aborts at both levels
  on the base), `grpdisc`, `grpbyval`, defect 2334's `big8k` (aborts at `-O0`) and the chains all run at
  both levels; the 1 MiB array
  chain 456.7 billion instructions and 7.0 GB at `-O2` become 0.83 billion and 16.5 MB; C's view of
  every function type byte for byte the base's; the per-thunk assertion falsified both ways.
- **The proposal as written (R-P)** built far enough to run the four probes and `grpval`, and
  objected to: a second typedef, a converting thunk, adapters that must be one function per program
  (unbuildable `threemod` in the measurement build), an assertion that cannot stand in every unit.
- **Rule A and rule B move the same 16 corpus programs**; rule B alone reaches part (a), defect 2334
  and `cbnamed`.
- **Found on the base**, two defects of panel 233's route, each a correct program refused at build,
  exit 2, each repaired by the route: a program binding any callback that takes the address of a
  function with a large program-record parameter or result (§ 2); a field of a record taken in place
  lent to C (§ 13), which panel 215's R1 zeroes through a `const` pointer. **Found in the route by the corpus**: two `fixedbugs-094` regressions (§ 7), repaired.
- **Stays**: `grpret`, `grpextonly` (the `extern`'s by-value copy), `grpval` (R7b's price), `grpopt`
  (the room's 1 GiB: 9 regions of 128 MiB in one prologue).

## 0. Setup

- Stage 1, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: exit 0.
- Stage 2, `./heroes build selfhost/main.hero -o heroes-next`: exit 0, `heroes 0.6.0`, `sources
  2fa3d81374af3684`, the brief's key; copied to the scratch as `heroes-base`.
- Instruments (`bin/` in the scratch): `frames.sh <compiler> <prog> <tag>` emits the C with
  `--emit-c` and compiles it alone at `-O0 -g` and `-O2 -gline-tables-only` with the nineteen words
  of `flags()` (`bin/flags.rsp`, read from `selfhost/cli/flags.hero:133-154`),
  `-Wframe-larger-than=65536`, `-I runtime -I <program's folder>`, printing each `stack frame`
  warning; `run.sh <compiler> <prog> <tag> [--sanitize]` builds at `-O0` and `-O2` and runs the binary
  under `timeout 60`, its output through `head -c 65536` into a file.

## 1. The base, measured (question 2's left column)

`grpfix` does not build on the base: `error[fixed_outside_a_group]: Held[1] is a C array member, so
it stands only as a field of a record inside an extern group` (a fixed array is never a result). Its
shape is written as `mine/grpfix2.hero`: a group record `HeldArr tag held_arr` holding `hs: Held[1]`
(`fix2.h`), returned by a Heroes function. Three more of mine, each `check` exit 0 on the base:
`grpdisc` (`_ = make(n: 7)`, the result discarded), `grpbyval` (`first(h: Held) -> i64` taking the
record by value), `grpval` (`go(f: (function(i64) -> Held))` calling `f(7)`, the call through a
value).

| program | frames over 64 KiB, `-O0` | `-O2` | run `-O0` | run `-O2` |
|---|---|---|---|---|
| `grpnoext` | `go` 134,217,840 | `go` 134,217,792 | `stack exhausted in grpnoext.go`, 134 | the same |
| `grpgen` | `go` 268,435,600 | `go` 134,217,792 | `stack exhausted`, 134 | the same |
| `grpfix2` | `make` 134,217,872, `go` 134,217,888 | `go` 134,217,824 | `stack exhausted`, 134 | the same |
| `grpopt` | none | none | `the room large values take off the stack is full, 1 GiB`, 134 | the same |
| `grpwrap` | none | none | `before`, `13`, 0 | the same |
| `grpret` | `go` 268,435,584 | `go` 134,217,808 | `stack exhausted`, 134 | the same |
| `grpextonly` | `go` 134,217,856 | `go` 134,217,776 | `stack exhausted`, 134 | the same |
| `grpdisc` | `go` 134,217,792 | none | `stack exhausted`, 134 | `7`, `8`, 0 |
| `grpbyval` | `go` 134,217,856 | none | `stack exhausted`, 134 | `7`, 0 |
| `grpval` | `go` 134,217,856 | `go` 134,217,776, `main` 134,217,792 | `stack exhausted`, 134 | `stack exhausted in grpval.main`, 134 |

The four rows the critic measured (`grpret`, `grpnoext`, `grpextonly`, `grpgen`) reproduce byte for
byte. `grpopt`'s `go` takes nine regions of `h_0opt_6aba43e8` (128 MiB and 40 bytes) in its
prologue, and its `make` nine more (`out/base-grpopt.c`, `grep hero_frame_take`).

## 2. Found beside the sitting, on the base: panel 233's R3 premise is false, a defect to file

Panel 233's R3 says *C never sees such a type, because the checker refuses a program record inside
a callback type*, and `emit/callback_thunk.hero:121-127` says a function whose direct signature
takes a large value in place *is never thunked*. But the thunk is written for every function whose
address is taken anywhere, once the program declares any `extern` taking a callback
(`emit/callback_guard.hero:23-37`, *deliberately a superset*), and its body forwards C's by-value
arguments to the function's direct signature. Two programs of 50 lines, `check` exit 0 each
(`mine/thk/thkbig.hero`, `mine/thk/thkres.hero`, header `cbany.h`: `apply(int64_t (*cb)(int64_t),
int64_t)`):

- `weigh(e: E4) -> i64`, `E4` a program record of 8,192 counted bytes, `f = weigh` in `main` beside
  `apply(cb: one, n: 1)`: `heroes build` exit 2, `internal error: compiling the generated C failed:
  ... error: passing 'h_thkbig_E4' ... to parameter of incompatible type 'const h_thkbig_E4 *'`, at
  the thunk's `int64_t hero_result = h_thkbig_weigh(h0_e);`.
- `build_one(n: i64) -> E4`, `f = build_one`: exit 2, `too few arguments to function call, expected
  2, have 1`, at `h_thkres_E4 hero_result = h_thkres_build_one(h0_n);`.

A correct program refused with exit 2 (`.claude/rules/verification.md` § Bounded discovery:
`blocking`). It is this sitting's ground: every route below writes a thunk for a function whose
direct signature takes `hero_out`, and the same body breaks there. The repair either route needs:
a thunk forwards through the function's VALUE convention, or a function whose value convention is
not C's (a large program record, which no callback type can name) gets no thunk and no table row.
Its case: `run/fixedbugs-2286-a-large-record-function-whose-address-is-taken-beside-a-callback`.
**Repaired by the route** (`large_value.crosses`: a function whose value takes a large program
record in place gets no thunk and no table row): on `heroes-route` both programs build and print
`2`, exit 0, at both levels.

## 3. Defect 2334, asked by the coordinator mid-sitting (questions 1 and 2)

`d2334/big8k.hero` and `big.h`, copied from lane b24-misc's scratch: `record Big tag big` over
`typedef struct big { uint8_t f0[8192]; } big;`, not `partial`, 8,192 named bytes, and `total(b:
Big) -> i64` reading `b.f0[0]` 1,024 times. On the base, `frames.sh`: `total`'s frame **8,956,096**
bytes at `-O0` and 245,888 at `-O2`; `run.sh`: `panic: stack exhausted in big8k.total`, 134, at
`-O0`; `0`, exit 0, at `-O2`. The emitted `total` declares **1,025** temporaries of `struct big`
(`grep -o 'HERO_NODEBUG struct big [^;]*;' | tr ',' '\n' | wc -l`), each read written `t3 = h0_b;
... t6 = t3.f0[...]`: the lowering loads the parameter's slot into a temporary before each element
read, and the emitter gives every temporary its own storage.

What each set does to it, read from `large_size.hero:156-169` before any build (the measured
readings follow in § 5): **today's set** (a group record that is or holds a `partial` one) does not
reach `Big`, so nothing moves and the abort stands under either route; **named fields past
`LARGE`** reaches it (8,192 counted bytes), so `total` takes `const struct big *` and each of the
1,025 temporaries becomes a region of the room: 8.4 MiB of the room per call instead of the frame,
and 1,025 copies of 8 KiB per call. The routes of question 3 do not differ here (no function value).
What would remove the copies themselves is question 5's rule read the other way: a temporary that is
only a load of a slot, read before the slot is written again, read in place from the slot.

## 4. The route built, in R7b's shape, rule B (`heroes-r7b-B`, `sources 14756db97db09fdd`)

**What it is**, `route-r7b-B.patch` in the scratch (11 files under `selfhost/emit/`, before part
(a)'s in-place): a group record in the set takes `true` in `ctype.Names.large`
(`large_size.large_types`, rule B: it is or holds a `partial` record, or its named fields pass
`LARGE`), so a call **by name** takes it in place (`void make(struct heroes_held *hero_out, ...)`,
`const struct heroes_held *hero_in_h0_h`); `ctype.Names.by_value` (`large_size.by_value_types`) lists
the group types among them, which a **value** keeps by C's convention (`large_value.value_large`):
the typedef (`types.function_type`), a call through a value (`large_value.called_in_place`) and the
thunk. A function whose two conventions differ (`large_value.converts`) stands as a value through
its **twin**, `hero_value_<name>` (`signature.value_twin`, panel 230's twin widened: C's convention,
calling the function in place into a local `hero_result`), which `func_ref` names and the thunk
calls; one `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk), __typeof__(&twin)))`
follows each such thunk (`callback_thunk.definition`). A function whose value takes a large
**program** record (which no callback type can name) gets no thunk and no table row
(`large_value.crosses`), which also repairs § 2's defect.

**Question 2 on the route** (`out/r7bB-frames.txt`, `out/r7bB-runs.txt`):

| program | frames over 64 KiB, `-O0` / `-O2` | run `-O0` / `-O2` |
|---|---|---|
| `grpnoext` | none / none | `before`, `7`, 0 / the same |
| `grpgen` | none / none | `7`, 0 / the same |
| `grpdisc` | none / none | `7`, `8`, 0 / the same |
| `grpbyval` | none / none | `7`, 0 / the same |
| `grpwrap` | none / none | `13`, 0 / the same |
| `grpfix2` | `make` 134,217,856 / none | `stack exhausted in grpfix2.make`, 134 / `7`, 0 |
| `grpopt` | none / none | the room full, 134 / the same |
| `grpret` | `go` 134,217,840 / 134,217,792 | `stack exhausted`, 134 / the same |
| `grpextonly` | `go` 134,217,856 / 134,217,776 | `stack exhausted`, 134 / the same |
| `grpval` | `go` 134,217,856 / `go` 134,217,776, `main` 134,217,776 | `stack exhausted`, 134 / the same |

`grpfix2`'s `-O0` frame is the construction `HeldArr(hs: [Held(...)])`, a compound literal of the
fixed array (part (a)'s in-place, § 6). `grpret` and `grpextonly` keep the `extern` call's by-value
copy, C's ABI, as defect 2253's cases expect. `grpval` is R7b's price: a call through a value keeps
C's by-value return. `grpopt` is question 5's (§ 7).

**Question 3 on R7b.** For `extval`, `cbparam`, `cbresult`, `cbnamed`, every function typedef, every
`extern`'s probe and every return assertion C reads (`grep -E '^typedef [^{]*\(\*h_0fn|hero_ffi_probe_h_[a-z]*_(apply|held_make)|^_Static_assert\(HERO_RET'`
over each `--emit-c`) is **byte for byte the base's** (17, 15, 15, 15 lines, `cmp`). `extval`'s
`t4 = held_make;` is unchanged; `make` is `void h_extval_make(struct heroes_held *hero_out, int64_t)`;
`pick` returns `hero_value_h_extval_make`; the table maps the twin to its thunk. The critic's four
never call the callback back (`(void)cb`), so `mine/cbcall/cbcall.hero` does: C calls a Heroes
function taking a 256 KiB `partial` record and one returning it, an `extern` and a Heroes function
stored in one function type are handed to C and called through in Heroes. Base and R7b both print
C's own reference (`ref.c`, the same program in C), `101|1002|4|1004|5|1006|1107`, exit 0, at both
levels.

**The assertion, falsified both ways** (the route's own `--emit-c` of `cbcall`, rewritten by
`perl`-like anchored replacements, compiled with the build's words and linked with
`runtime/runtime.c`): the thunk given the function's own signature (panel 233's R7c) is refused,
`static assertion failed due to requirement '__builtin_types_compatible_p(void (*)(struct cb_held *,
long long), struct cb_held (*)(long long))': heroes-callback cbcall.make: the thunk C is handed has
its value's type`; the same unit with the assertion removed builds at clang exit 0 and ends `101`,
`panic: a null pointer was read through, at offset 0x2, called from cbcall.make`, **exit 134**; and
twin and thunk both given the function's own signature (so the assertion holds) are refused where
the address is taken, `incompatible function pointer types assigning to 'h_0fn_2a75a378' (aka
'struct cb_held (*)(long long)') from 'void (struct cb_held *, int64_t)'`. So the chain is closed
link by link: the probe ties the typedef to the header, clang ties every value to the typedef at its
assignment, the assertion ties the thunk to the value. The assertion names the value's type, not
the typedef as panel 233's seat wrote it, because a unit writes only the typedefs its own types reach
(`emit/typeorder.hero`, `synth.wants_type`) and the thunk's unit need not reach its function's type.

## 5. Part (a): the chains, and construction in place for group records

**Construction in place for group records** (`route-r7b-Bip.patch`, `heroes-r7b-Bip`, `sources
0e0207f87b9115ad`): `construct.hero:137`'s condition loses `byte_copy.own_declaration`, so every
large record is built field by field into its destination (`large_value.in_place`), a group's record
by the header's member names, a `cstr` through `aggregate.written_as`, and a fixed-array field built
from a list element by element (`storageless.element_stores`, `t8.h[0] = t2; t8.h[1] = t6;`) instead
of `__builtin_memcpy(&m, &(__typeof__(m)){...}, sizeof m)`, whose compound literal is an object of the
frame as large as the field. `grpfix2` then has no frame over 64 KiB and prints `7` at both levels
(it aborted in `make` at `-O0` without it, § 4).

**clang at `-O2` on the 64 KiB chains** (`bin/cc.sh`: `--emit-c`, then `/usr/bin/time -l clang
-fintegrated-cc1 @flags.rsp -O2 -gline-tables-only -I runtime -I <program's folder> -c`, each compile
alone; `out/chains64.txt`):

| unit | base | route, rule B, no group in-place | route, rule B, group in-place |
|---|---:|---:|---:|
| `gn12` (named halves) | 12,462,777,589 / 166,658,576 B | 3,230,527,267 / 45,810,096 B | **3,210,935,443 / 45,777,352 B** |
| `ga12` (array halves) | 10,216,415,277 / 159,400,488 B | 700,266,157 / 15,286,728 B | **700,757,161 / 14,893,464 B** |

The base reproduces `00-shared.md`'s readings within 1 % (12,427,643,548 and 10,115,706,929). The
route puts both under panel 233's bound, *each unit under 8 billion instructions and 100 MB*. What
remains of `gn12` is clang's front end over the unit's 4.25 MB of C, the padding table's one
`offsetof` per named member at every depth (panel 233's ffi seat § 3c: front end alone 2.66 billion
on its build): not a cost the route can move. **Rule A moves neither chain** (its records are fully
named and none is `partial`, `large_size.hero:165` on the base), so part (a) needs rule B.

**The 1 MiB array chain** (`f2/ga16`, `gen2.sh 16`'s array halves; this seat alone, one compile at a
time, base first, `out/chain16-*.txt`):

| unit | clang `-O2` instructions | peak footprint | run `-O0` | run `-O2` |
|---|---:|---:|---|---|
| base | **456,654,769,868** | **6,994,860,104 B** | `stack exhausted in chain.make16`, 134 | (panel 233: runs) |
| route, rule B, no group in-place | 840,069,063 | 16,630,192 B | | |
| route, rule B, group in-place | **833,788,494** | **16,482,712 B** | `before`, `1`, 0 | `before`, `1`, 0 |

The base reproduces panel 233's ffi seat (446.9 billion, 6.98 GB) and lane b23-emit (451.6 billion,
6.99 GB) within 2.2 %; `/usr/bin/time` read `real 209.15 user 129.14` beside four lanes, so no
duration is read from it. The route divides clang's work by 548 and its memory by 424, and `gn12`,
`ga12` and `ga16` print `before`, `1` at both levels. **What group in-place adds is the `-O0` frame,
not clang's work**: the chains cost the same without it (the compound literal of two halves is cheap
for the optimiser), while `grpfix2`'s `make` keeps a 128 MiB `-O0` frame without it.

**Defect 2334 on the route** (rule B): `total` takes `const struct big *hero_in_h0_b`, its 1,025
temporaries become 1,025 regions of 8 KiB (`grep -o 'hero_frame_take(sizeof(struct big))' | wc
-l`), its `-O0` frame falls from 8,956,096 to 583,856 bytes (the 1,024 `i64?` temporaries remain),
and it prints `0`, exit 0, at both levels. Under rule A it is unchanged (the base's reading). It
pays 8.4 MiB of the room and 1,025 copies of 8 KiB per call: the frame's cost moved, not removed.

## 6. Question 3: the proposal as written (R-P), built on the same base, and the two compared

**R-P** (`rp_patch.py` applied on the R7b route, `route-rp-Bip.patch`, `heroes-rp-Bip`, `sources
17a3862c60d14c24`; 16 files, +588/−97 against the base): a value takes a group's large record in
place (`value_large` is `is_large`), so the program's typedef `h_0fn_<hash>` takes `hero_out`; a
second typedef by C's convention, `h_0fc_<hash>`, is written beside it where a group's record makes
them differ (`types.c_view`), and the probe and the crossing cast name it (`probe_params.c_type_of`,
`callback_thunk.crossings`); the thunk keeps C's convention and calls the function in place
(`&hero_result`, `&h0_h`); an `extern` stored as a value of such a type is named through an adapter
of the program's convention, `hero_xv_<name>` (`if (hero_out != NULL) *hero_out = held_make(h0_n);
else (void)held_make(h0_n);`), which the table maps back to the `extern` (`callback_guard.externs_taken`,
`callback_thunk.adapters`); and the assertion names C's typedef, `_Static_assert(__builtin_types_compatible_p(__typeof__(&thunk),
h_0fc_<hash>))`.

**What R-P met on the way, each measured:**

- **The assertion in the form panel 233's seat wrote it cannot stand in every unit**: placed in the
  table, which every unit writes, the first build of `extval` stopped at `library.c:71:77: error:
  unknown type name 'h_0fc_5ee616fc'`, the library's unit writing no typedef of the program's types
  (`emit/typeorder.hero`, `synth.wants_type`). It has to stand beside the thunk, in its function's
  unit, which needs the function's own type and so the program threaded to `callback_thunk.definition`
  (`body.hero`), and a scan for a `func_ref` of the function (`value_type_of`): the checker records no
  type per function declaration. R7b's assertion names two functions both declared in that unit.
- **An adapter must be one function in the whole program.** The measurement build writes each
  adapter `static` in every unit; `mine/threemod/` (module `holder` binds `held_make` and returns it
  as a value, module `caller` binds `call_ret` and hands it to C, `main` carries it) then stops at
  build, `error[ffi_unknown_name]: cbcall.h declares held_make read alone, and not where this unit
  reads it`, at both levels: a unit that does not read the header cannot define the adapter. Written
  only in the units that read the header, two units would each hold their own `static` adapter, and
  `caller`'s table, comparing against its own, would return `holder`'s unchanged, of the program's
  convention, to C, which calls it by C's: panel 233's R7c, through `void (*)(void)` where no
  assertion sees it (an inference from the table's code, unrun). The adapter owes one definition with
  external linkage in the `extern`'s own unit, a name unique to its module, and a prototype in every
  unit whose table names it: unbuilt here.
- C's view: the type C reads is byte for byte the base's, under a new name (`h_0fc_` for `h_0fn_`),
  and the probe checks that one; the program's `h_0fn_` changes (`typedef void
  (*h_0fn_5ee616fc)(struct heroes_held *, int64_t);`). Four lines differ from the base's in each of
  the four probes (`out/cview-diff-rp-*.txt`).

**Runs** (`out/rp-runs.txt`), both levels each: `extval` `2`, `3`; `cbparam`, `cbresult`, `cbnamed`
`1`; `cbcall` C's own reference, `101|1002|4|1004|5|1006|1107`; `twomod` `1101|1002|103|1004`;
**`grpval` `before`, `7`, exit 0**, where R7b and the base abort; `threemod` refused at build (above),
where the base and R7b print `6|6`.

| | R7b (recommended) | R-P (the proposal as written) |
|---|---|---|
| typedefs per function type | one, C's, unchanged | two where a group's record is named |
| C's view | byte for byte the base's, names included | the base's types under new names |
| thunk | unchanged, calls the twin | converts into `hero_out` |
| an `extern` stored as a value | itself, unchanged | an adapter, owing one definition program-wide (unbuilt) |
| the per-thunk assertion | thunk against value, beside the thunk, holds in every unit | thunk against `h_0fc_`, only where the unit writes it |
| a call through a value of a group's large record | C's by-value return: `grpval` aborts at 128 MiB | in place: `grpval` runs |
| lines against the base (R7b's twin code included in both) | 13 files, +348/−89 | 16 files, +588/−97, and the adapters' linkage still owed |
| `threemod` | `6|6` both levels | refused at build |

**I would adopt R7b.** R-P buys one shape, a call through a function value returning a group record
past the stack, and pays for it a second typedef per such type, a converting thunk, an adapter per
stored `extern` whose single definition across units is not yet written, and an assertion whose
placement depends on which unit writes which typedef. R7b keeps every name and type C sees, needs no
adapter, and its assertion stands in the thunk's own unit by construction. `grpval` stays an abort,
which spec § 9's *running out of stack aborts* already describes and which a later route can take
without re-opening the boundary (a function type no `extern` names could take `hero_out`, decided per
type over the whole program: unbuilt, said in § 9).

## 7. What the corpus found in the route, and its repairs (question 7)

The first `emission` run on the route (`heroes-r7b-Bip`, rule B; `./heroes run tests/harness/main.hero
-- ./heroes-next emission`, `out/emission-r7bBip.txt`) read **2,280 passed, 16 failed**: 14 moved
emissions and **2 programs the route broke**, both `run/fixedbugs-094-*`, an `extern constant` of a
`partial` record (Darwin's `PTHREAD_MUTEX_INITIALIZER` and its sibling): `exit 2`, `void function
'..._BIG_INIT' should not return a value`. Two writers take a function's signature from
`signature.signature`, which now writes `hero_out` for such a record, and write their own body:

- **`emit/record_constant.hero`**, the accessor of a record's `extern constant`, ended `return
  hero_constant_value;`. It now writes `if (hero_out != NULL) *hero_out = hero_constant_value;
  return;` where the result is large (+10 lines).
- **`emit/value_probe.hero`**, the probe of header constants' values, called each accessor as
  `{ volatile __typeof__(name()) hero_v = name(); ... }`: `too few arguments to function call,
  single argument 'hero_out' was not specified`. Its `Accessor` now carries `in_place`, asked of
  `large_size.large_types(c, decls)` once per unit holding a record's constant (the probe's callers
  in `cli/` hold no `ctype.Names`), and such an accessor is called `name((void *)0)`: the probe is
  appended to the unit after preprocessing, so `NULL` was `use of undeclared identifier` on the
  first try (+21 lines).

Both cases then print their `.expected` at both levels (`850045863`, `0`; `7`, `0`, `0`). The third
writer of that shape, `emit/static_constant.hero:114`, is unreachable by a record: a program's
`constant` may not build one (`error[constant_body]`, measured on `mine/konst/kgroup.hero` and
`kconst.hero`, base and route alike). **These two are what the brief's question 7 is for**: no
seat's reading of the route found them, the corpus did.

## 8. Questions 4 and 5: the `extern`'s own copy, and the room

**Question 4.** `grpextonly` on the route reads what it reads on the base, `go` 134,217,856 bytes at
`-O0` and 134,217,776 at `-O2`, and `panic: stack exhausted in grpextonly.go`, 134, at both levels
(§ 4's table): the copy a by-value `extern` argument needs is made by clang on the caller's frame,
which no route of this lane can move (on x86-64 the struct itself travels in the stack's argument
area; on arm64 and Windows the caller's copy is passed by pointer; either way it is the caller's
frame, panel 233's critic's `byval.c`). What the program meets is spec § 9's abort with the guard's
true words. A build-time message for an `extern` taking such a record by value would need the
record's `sizeof` (a `partial` record's size is not the program's to count) and is a new
diagnostic: **it leaves this lane**, `grpextonly.hero` its program, for the full panel if anyone
asks for it. I do not: the message would fire on defect 2253's own three cases, whose abort is their
expectation.

**Question 5, the room** (regions counted per function in each `--emit-c`, `grep -o
'hero_frame_take'` per body; § 1 and above): the route takes **the same regions the base takes** for
every witness. `grpnoext`: `make` 1, `go` 2, **384 MiB at its peak** (the three are live together
while `make` runs), well under the 1 GiB of panel 233's R5. `grpdisc` 1. `grpbyval` and `grpgen`'s
`go` 3. `grpopt`: `go` 9 regions of `h_0opt_6aba43e8` (128 MiB and 40 bytes) and `make` 9 more;
`go`'s nine alone are 1,152 MiB, so the run stops in `go`'s prologue on base and route alike, before
`make` is called. So **a 128 MiB witness fits under the 1 GiB only where at most seven regions of it
are live at once**; 128 MiB is the smallest power of two past every leg's stack (Windows links 64
MiB, `fixedbugs-2253-held.h`), and the landing's cases are the shapes of at most three.

*Whether a temporary copied straight into a slot could live in the slot's region* is **unbuilt**;
what the emitted C shows (a reading, not a run): in `grpopt`'s `go`, `t3`, `t7` and `t9` are
whole copies of the slot `h0_f0` made only to read its tag or its payload, and `t10` the payload of
`t9`: four regions that a rule reading a large value in place (`ir/in_place.hero`'s rule, which
refuses only a fixed array's value today, widened to a whole large value read before the slot is
written again) would remove. That leaves `h0_f0`, `h1_own1`, `t2` and the two saved old values
`t12` and `t13` in `go`, and `make`'s nine: fourteen regions, still past the 1 GiB at 128 MiB. So no
such rule alone brings `grpopt` under the room; the saved copies of the ownership's own slots are
the rest of it, an IR question this sitting does not reach. The same widened rule is what would
remove defect 2334's 1,025 copies (§ 3, § 5), and it is the lowering's, not the convention's.

## 9. Question 1: the set, read on the corpus, the targets and the union

**The emission corpus** (route `heroes-r7b-Bip4`, `sources 44342c1333e8fbd3`, rule B with the two
repairs; `out/emission-r7bBip4.txt`): **2,282 passed, 16 failed, every failure a moved emission**.
Each moved file emitted again by rule A's compiler (`heroes-r7b-Aip4`, `sources 0c85617b6b9b6409`,
the same route with `large_size.group_large` reading `holds_partial` alone) and compared with
`cmp` outside the key line: **all 16 move identically under rule A and rule B**. Rule A's set is
inside rule B's, so rule A moves nothing rule B leaves; **the corpus holds no group record past
`LARGE` that is not `partial`**, and the two rules differ only on programs outside it: the chains
(part (a)), defect 2334, `cbnamed`. Every moved program binds a `partial` record:

| moved (`tests/emission/...`) | what moved, read in its diff |
|---|---|
| `run-fixedbugs-2316-*` (4) | `make` takes `hero_out`; its region loses `__cleanup__(hero_frame_give_at)` and goes back after the write; the caller passes `&t2`; `make`'s twin |
| `run-fixedbugs-094-*` (2) | the constant's accessor writes through `hero_out` (§ 7) |
| `examples-ctime-main` | five functions taking `struct tm` take `const struct tm *`, each with an unused twin |
| `run-ffi-a-filled-record-is-asked-for`, `run-ffi-rest-zero-a-partial-record` | the same, for their `partial` records |
| `fixedbugs-a-header-field-that-is-not-const`, `run-fixedbugs-156-*`, `run-fixedbugs-173-*`, `run-fixedbugs-396-*` (3), `run-fixedbugs-a-tag-names-a-handle-and-a-record` | in-place construction of a small `partial` record already off the frame on the base: `(BF){.kind = t3}` becomes `t4 = *(BF *)__builtin_memset(&t4, 0, sizeof t4); t4.kind = t3;` (2 or 4 lines each) |

A twin is written for every function whose two conventions differ, its address taken or not (the
library's twins are written the same way); writing one only where an address is taken would need
the program's taken set (`callback_guard.addresses_taken`, one walk): unbuilt, said.

**The four CI targets.** Both rules count named fields by their Heroes widths, never `sizeof`, so
the set is the same number on every target (panel 233's reason for its count); no rule here
introduces a per-target emission. Unrun on the legs themselves (this Mac only).

**The union's double count** (`probe/union/unioncount.hero`, `ub` `sizeof` **2,400**, measured):
base and rule A take 3 regions of the holder `W` (counted 4,808) and leave `Ub` on the frame; rule
B also counts `Ub` 4,800 and takes 2 regions of `struct ub` and passes it in place; all three print
`3` at both levels. So rule B moves a 2,400-byte record off the frame: a cost of the room and of a
copy, not a fault. The count's comment *a lower bound of `sizeof`* (`large_size.hero:11`) is false
for a group record naming two members of one union, under either rule and on the base already for
the holder; counting a union's members once would need the union's membership the emitter reads
for `extern_union.hero`: owed as a correction of that comment at least, unbuilt.

**I recommend rule B.** It is rule A on the whole corpus, and outside it it is the only rule that
reaches part (a) (548 times less clang work on the 1 MiB chain) and defect 2334 (an abort at the
default level becomes a run).

**Sanitizer**, every shape at both levels on the route (`out/sanitize-route.txt`): `grpnoext`,
`grpgen`, `grpfix2`, `grpdisc`, `grpbyval`, `grpwrap`, `cbcall`, `twomod`, `threemod`, `gn12`,
`ga12`, `big8k`, `extval`, `cbparam`, `cbresult`, `cbnamed` run clean with their answers; `grpopt`
stops at the room's own words, as on the base; `grpval` reports ASan's `stack-overflow`, as the base
does for it and for `grpnoext` (`base-san`). **The compiler's own tests on the route: 1,800 tests,
all passed** (`./heroes test selfhost/main.hero`, `out/selftest-route.txt`).

## 10. The cases the landing owes

Each a `run` case at both levels unless said, its witness named, the 128 MiB record defect 2253's
header (`fixedbugs-2253-held.h`), so it passes every leg's stack and no frame of the route holds it:

1. `run/fixedbugs-2286-a-partial-record-returned-by-a-function-is-written-into-its-callers-room` (`grpnoext`: `before`, `7`).
2. `run/fixedbugs-2286-a-generic-instance-returns-a-partial-record-in-place` (`grpgen`).
3. `run/fixedbugs-2286-a-discarded-partial-result-is-written-nowhere` (`grpdisc`: `before`, `7`, `8`).
4. `run/fixedbugs-2286-a-partial-record-taken-by-value-is-handed-as-a-pointer` (`grpbyval`).
5. `run/fixedbugs-2286-a-group-record-holding-a-fixed-array-of-partial-records-is-built-in-place` (`grpfix2`, which needs part (a)'s in-place).
6. `run/fixedbugs-2286-a-chain-of-group-records-nested-by-copy-builds-and-runs` (`ga16`, the 1 MiB array chain: 0.83 billion instructions at `-O2` on the route, so the `run` suite can hold it; `before`, `1`).
7. `run/fixedbugs-2286-c-calls-back-a-function-taking-and-returning-a-large-group-record` (`cbcall`, 256 KiB so C's own frame holds its by-value copies; C's reference output).
8. `run/fixedbugs-2286-an-extern-and-a-function-in-one-function-type-cross-to-c-from-another-module` (`threemod`, three modules: `6`, `6`).
9. `run/fixedbugs-2286-a-large-record-function-whose-address-is-taken-beside-a-callback` (§ 2's defect, `thkbig` and `thkres`; today exit 2 at build).
10. `run/fixedbugs-2334-each-element-read-of-an-8-kib-group-record-runs-at-the-default-level` (`big8k`, defect 2334's own, closed by rule B: `0`).
11. `emit/fixedbugs-2286-the-thunk-of-a-converted-function-is-asserted-against-its-value`: an `emit`
    golden pinning the twin, the thunk calling it and the `_Static_assert` line (a property of the C,
    which no `run` case sees), and a test in `large_value.hero` beside panel 233's: *a group's record
    is taken in place by a call by name and by C's convention through a value* (`is_large` true,
    `value_large` false for `Held`).

12. `run/fixedbugs-2286-a-padded-partial-record-crosses-hero-out-and-a-pointer-and-c-reads-its-padding-zero` (`padhero`: `0`, `0`, `8`), and `run/fixedbugs-2286-a-field-of-a-record-taken-in-place-is-lent-to-c` (`padwrap`, § 13's base defect: `0`).

What stays an abort, said in the cases' headers rather than filed: `grpret` and `grpextonly` (the
`extern`'s by-value copy, question 4), `grpval` (R7b's price), `grpopt` (the room, question 5).

## 11. The cost, and the ceiling (question 7; my seat's own questions)

**Core or sugar: neither.** The route adds no construct, no word, no diagnostic and no checker
rule; the type checker and the lowering are untouched (design.md §1.7: *anything in the core must be
implemented in the type checker and the lowering and the backend*; this is the backend alone, every
file under `selfhost/emit/`). R7b needs no spec text: spec § 9's *running out of stack aborts* stays
true of what it leaves (`grpret`, `grpextonly`, `grpval`, `grpopt`).

**The route I recommend, as built and restructured** (`heroes-route3`, `sources 1699bb340bef9aeb`;
R7b, rule B, group in-place, § 2's thunk repair, § 7's two corpus repairs, § 13's padding repair;
`restructure.py` in the scratch for the restructure), lines by `wc -l` against the base:

| file | base | route | |
|---|---:|---:|---:|
| `emit/large_value.hero` (`writable` of § 13 included) | 351 | 232 | −119 |
| `emit/large_construct.hero` (new: construction in place, moved out of `large_value`) | 0 | 205 | +205 |
| `emit/storageless.hero` (`element_stores`) | 185 | 244 | +59 |
| `emit/signature.hero` (the twin widened, `params_as`, `has_twin`) | 306 | 350 | +44 |
| `emit/value_probe.hero` (§ 7) | 368 | 392 | +24 |
| `emit/padding.hero` (§ 13, one call) | 346 | 348 | +2 |
| `emit/callback_thunk.hero` (crossing filter, twin, assertion) | 278 | 296 | +18 |
| `emit/construct.hero` (`element_lists`, group in-place) | 347 | 363 | +16 |
| `emit/inst.hero` (one call re-flowed by the formatter) | 465 | 476 | +11 |
| `emit/record_constant.hero` (§ 7) | 170 | 179 | +9 |
| `emit/large_size.hero` (rule B) | 203 | 211 | +8 |
| `emit/func_ref.hero`, `emit/unit.hero`, `emit/body.hero`, `emit.hero`, `emit/prologue.hero` | | | +2, +1, −1, 0, 0 |
| **total, 16 files** | **4,473** | **4,752** | **+279** |

The restructure keeps the emitted C byte for byte (below) and keeps `ctype.Names` as it was: a type
present in `names.large` is off the frame and taken in place by a call by name, `true` where a
function value takes it in place too, `false` for a group's record, which a value takes by C's
convention (the base already wrote `false` for a group record off the frame); `types.hero`,
`ctype.hero` and `members.hero` are untouched. And it deletes the path a group record off the frame
was returned by, out of a region by C's convention under `__cleanup__` (defect 2316's repair), every
such result being written through `hero_out` now: `runtime/parts/frame.c`'s `hero_frame_give_at`
then has no emitter left that calls it (left in the runtime, whose key every build reads; the landing
may remove it).

**`layout`** on the restructured route: **6 passed, 0 failed** (`out/layout-restr.txt`); before the
restructure it read 5 and 1, `ctype.hero` 410 lines of code past its decided 404 and
`large_value.hero` 359 past § 11's 300 (`out/layout-route.txt`). **The compiler's own tests**: 1,800
passed, before and after the restructure.

**R-P's cost, for the comparison**: +588/−97 in 16 files as built (§ 6), before the two corpus
repairs (+41) and before its adapters' single definition across units, which it owes and this seat
did not build; about 1.7 times R7b's lines, and the part left unbuilt is the part with the soundness
hole.

**Fits one person**: 279 lines in one directory, each part with its witness program.

## 12. The suites the route owes, run on it (question 7)

In my worktree only, uncommitted: the route's emissions blessed deliberately (`UPDATE_EMISSION=1
./heroes run tests/harness/main.hero -- ./heroes-next emission`, the 16 files of § 9) and the seed
regenerated by the route (`./heroes-next build selfhost/main.hero --emit-c -o seed/heroes.c`;
29,043,650 → 29,183,585 bytes, 8,536 lines outside `#line` differ, the emitter's own new code: the
seed holds **no twin**, no function of the compiler taking a converted record). Then, the suites in
separate harness processes beside each other (pass or fail, nothing timed):

| suite | result | build |
|---|---|---|
| `wholes` | 754 passed, 0 failed (the route's blessed emissions and its seed) | `5193df5e7652f080` |
| `descriptors` | 754 passed, 0 failed | `5193df5e7652f080` |
| `emit` | 13 passed, 0 failed | `5193df5e7652f080` |
| `lldb` | 17 passed, 0 failed | `5193df5e7652f080` |
| `emission` | 2,294 passed, 2 failed: the restructure's own slip, below | `5193df5e7652f080` |
| `determinism` | 695 passed, 2 failed: the same two | `5193df5e7652f080` |
| `emission` | **2,298 passed, 0 failed** | `68e3132a577e42ae` (slip repaired) |
| `determinism` | **697 passed, 0 failed** | `68e3132a577e42ae` |
| `layout` | 5 passed, 1 failed (`padding.hero` 308 lines of code) | `46ed0af01fcab112` (§ 13's first form) |
| `layout` | **6 passed, 0 failed** | `1699bb340bef9aeb` (final) |
| `emission` | **2,298 passed, 0 failed** | `1699bb340bef9aeb` (final) |
| `determinism` | **697 passed, 0 failed** | `1699bb340bef9aeb` (final) |
| the compiler's own tests | **1,800 passed** | `1699bb340bef9aeb` (final) |
| `wholes`, `descriptors` | **754 passed, 0 failed** each (the route's blessed emissions, the seed regenerated by this build) | `1699bb340bef9aeb` (final) |
| `emit`, `lldb` | **13 and 17 passed, 0 failed** | `1699bb340bef9aeb` (final) |
| `run`, narrowed to every moved case and to defects 2253's and 2278's | 2316: 6, 094: 8, 396: 15, 173: 2, 2253: 3, 2278: 9; 156, `ffi-a-filled-record`, `ffi-rest-zero`, `a-tag-names-a-handle`: 1 each; **0 failed** | `1699bb340bef9aeb` (final) |
| the compiler's own tests | 1,800 passed | `14756db97db09fdd` and `5193df5e7652f080` |

**The restructure's slip, found by `emission`**: `value_probe.hero` read the large set with
`.default(false)`, which after the restructure (a group record written `false`) answered *not in
place* for a `partial` record's constant, and the two `fixedbugs-094` cases failed again. It now
asks presence, as `large_value.is_large` does (`!large[f.result].is_err()`), with the comment saying
so. The lesson is the project's own: the probe asks the set apart from `large_value`, two homes for
one answer, and the landing should give the probe a reader of `large_value`'s instead (the probe's
callers in `cli/` hold no `ctype.Names`, which is why it asks `large_size`).

**Self-build** (each compiler building its own sources in a scratch copy, `/usr/bin/time -l
<compiler> build selfhost/main.hero`, cold then warm, instructions retired): base 686,915,956,137
cold and **527,896,080,513** warm; route 695,339,939,315 cold and **528,856,779,096** warm: **+0.18 %
warm**, +1.2 % cold. No clock was read (four lanes share the machine); the latency instrument's
`compare` is owed at the landing, on a still machine.

## 13. Panel 215's R1 on the route, and a second defect of panel 233's route found by it

`mine/pad/padrec.h`: a 64 KiB `partial` record with 14 padding bytes (`int8_t a; int64_t b; int8_t
c; ...`) and `padding_dirty(const padrec *)`, which counts the padding bytes that are not zero, beside
`dirty_stack()`, which fills 200,000 bytes of stack with `0xAB` first.

- **`padhero.hero`**: `make(n) -> Pad` (through `hero_out` on the route), its result lent to C
  read-only, then handed to `inspect(p: Pad)` (a `const struct padrec *` on the route), which lends
  it to C from there. **On the route as first built it was refused at build**, exit 2:
  `passing 'const struct padrec *' to parameter of type 'void *' discards qualifiers`, at
  `hero_pad_fill(&h0_p, ...)`: R1 zeroes the padding before each lend, and the place is the caller's
  copy behind the `const` pointer. A regression of the route against the base, which reads `0`,
  `0`, `8`.
- **`padwrap.hero`**, the same shape one level down: a program record `Wrap` holding the `partial`
  record, taken by a Heroes function (panel 233 hands it as `const h_Wrap *`) and its field lent to
  C: **refused at build, exit 2, on the base already**, the same message. A second defect of panel
  233's route, to file (`blocking`: a correct program refused with exit 2), its case
  `run/fixedbugs-2286-a-field-of-a-record-taken-in-place-is-lent-to-c`.

**Repair**: a lent place rooted in a parameter received in place is spelled through the parameter's
pointer cast to the record's own type, `hero_pad_fill(&(*(h_padwrap_Wrap *)hero_in_h0_w).f_inner,
...)` (`large_value.writable`, +18 lines; `padding.hero` one call, +2): the address stays typed, so an
array's element count still reads it; the padding is no part of the value and the copy is the
caller's temporary. (A first form cast the address to `(void *)` inside `padding.hero` and took it to
308 lines of code, past § 11's 300, `out/r2-layout.txt`; moved.) On `heroes-route3` (`sources
1699bb340bef9aeb`): `padhero` prints `0`, `0`, `8` and `padwrap` `0`, at both levels and under
`--sanitize`: **C reads zero padding after the record crossed `hero_out` and
after it crossed a pointer.** The repair changes C only where a lent place is rooted in such a
parameter, which the base and the route before it could not compile.

## 14. Verdict

- **verdict**: **object** to the proposal as written (its clause *an indirect call through a
  function value does the same*); **adopt with conditions** the rest of it in R7b's shape: a group
  record off the frame, set by rule B, takes `hero_out` and the pointer parameter in a call by name,
  a function value, its typedef and its thunk keep C's convention through a twin, one assertion per
  converted thunk, construction in place for group records. No veto: no construct is added and the
  ceiling holds.
- **section**: design.md §1.7 (*anything in the core must be implemented in the type checker and the
  lowering and the backend*: this is the backend alone), §1.1 (simplicity sets the ceiling: R-P's
  second typedef, adapters and their linkage), §1.12 (robustness; *a boundary that is complete and
  defended*). design.md says nothing of thunks (`grep -i thunk docs/design.md`: none), so the
  assertion's rule stands on panel 233's veto (a), not on the document.
- **implementation_cost**: +279 lines (4,473 → 4,752 by `wc -l`), 16 files, all `selfhost/emit/`:
  `large_value.hero` 351 → 232, new `large_construct.hero` 205, `storageless.hero` +59,
  `signature.hero` +44, `value_probe.hero` +24, `callback_thunk.hero` +18, `construct.hero` +16,
  `inst.hero` +11, `record_constant.hero` +9, `large_size.hero` +8, six files ±2; `layout` 6/0 on
  the final tree. R-P as built: +491 net in 16 files before +41 of repairs and its unbuilt adapter linkage.
- **needed_for_self_hosting**: **no**. The compiler binds no record the route converts: its own seed,
  regenerated by the route, holds no twin. Compiler-need of another kind (CLAUDE.md § 13): a correct
  program over a large C struct aborts at the default level today (`grpnoext`, defect 2334).
- **argument**: The route is backend-only and closes what it claims: `grpnoext`, `grpgen`,
  `grpdisc`, `grpbyval`, `grpfix2` and defect 2334 run at both levels; the 1 MiB array chain falls
  from 456.7 billion instructions and 7.0 GB to 0.83 billion and 16.5 MB; C's view stays byte for
  byte. The proposal's value clause buys one shape (`grpval`) for a second typedef per type, a
  converting thunk, an adapter per stored `extern` that must be one function program-wide (the
  measurement build's static adapters made `threemod` unbuildable; per-unit copies would, unrun, hand C the
  wrong convention), and an assertion that cannot stand in every unit
  (`library.c: unknown type name 'h_0fc_5ee616fc'`). R7b keeps one typedef and asserts each thunk
  in its own unit.
- **prediction**: at the landing, the `emission` suite run by the landing's compiler on this base's
  corpus **before** its re-bless reads **exactly 16 failed**, the 16 files of § 9 and no other, and
  clang `-O2` on `f2/ga16` (the 1 MiB array chain, `gen2.sh 16`) reads **under 1.0 billion
  instructions and 20 MB** peak (`/usr/bin/time -l clang -fintegrated-cc1` with `flags()`'s words and
  `-gline-tables-only`, Apple clang 21; 0.834 billion and 16.5 MB here, 456.7 billion and 7.0 GB on
  the base). Checkable at the landing's gate.
- **condition**: I turn to a **veto** if the landing needs a checker rule, a diagnostic or spec text
  to hold R7b, or lands with `layout` red (no seam split). I would **approve R-P instead** on two
  measurements: its adapters built with one definition program-wide and `threemod` printing `6`, `6`
  at both levels, and a program the project needs that calls a function value returning a group
  record past the stack (today none: no tracked program hands C a group record past `LARGE` through
  a callback, the critic's census).

## 15. What this seat did not run

- **Any leg but this Mac** (arm64, Apple clang 21): Linux arm64 and x86-64 and the Windows target are
  the CI's legs after a push; the R7b twin's and thunk's lowering there is unrun (no C type changes,
  so an inference that it lowers as the base's does).
- **The seed's fixpoint over two generations** (gate A) on the route: the seed was regenerated once,
  for `wholes` and `descriptors`, not rebuilt from.
- **R-P's adapters with one definition across units**, and so the silent hazard of § 6 (per-unit
  copies handing C the program's convention): unbuilt, an inference from the table's code.
- **A rule reading a large value in place** (question 5) and **twins only where an address is
  taken**: unbuilt, each said where it bears.
- **Counting a union's members once**: unbuilt; the double count's effect is measured (§ 9).
- **The latency instrument's `compare`**: no clock was read, four lanes sharing the machine; the
  instruction counts of the self-build stand in for it here and do not discharge it.
- **The whole `run` suite** on the route: narrowed to the moved cases and their neighbours
  (`out/r3-run-*.txt`).

