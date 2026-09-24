# Panel 177 — shared brief

**HEAD `521c5e02`, `git status` clean, read by the coordinator on 2026-09-24
before the briefs went out.** Every number below was produced by a command run
while this brief was being written, and the command is named beside it. The
trunk is frozen from now until the synthesis.

Two questions, one sitting, because both are about what the language says of a
handle's life after panel 176.

---

## Question 1 — the life after it ends (defects 077 and 088)

A handle is a C pointer the program holds as a value. `consumes` on a C
parameter ends its life (spec § 13, `spec/heroes-spec.md:382-385`: *"`consumes`
after a parameter says the call ends that value's life … mark the parameter `@`
and the value does not survive the call"*), and the live set in
`runtime/parts/alloc.c` records every acquired address and refuses a give-back
of one it does not hold (`:402` onward, an open-addressed table of addresses
under one mutex, `:161-164`). What nothing checks is a READ of the value after
its life ended.

**The class is already stated in design.md, and the defects are shapes of it.**
Part 8 wart 20 (`grep -n "^20\. \*\*One copy" docs/design/design.md`, line
3592): *one copy of a value holding a `ptr` can free what every other copy
holds, at exit 0*; the half `consumes` does not reach is *a copy made BEFORE
the call still holds the freed address*, and the class is **the affine
handle**, which Part 6's borrow-checker row (line 2663) refuses on COST since
2026-09-13, naming what would move it again: *an alias rule … priced and
compiled — nobody has*. The wart admits the class *only while the cheapest
guard is being built rather than argued about*. Since then panel 150 added the
live set, which knows whether an address is live — a fact the wart could not
use. Both defect entries in `docs/work/DEFECTS.md` carry that correction, dated
today.

### Measured today

Reproducers in this directory. Darwin arm64 is `./heroes` rebuilt from the seed
at HEAD; Linux is `docker run heroes-linux` (x86-64 under Rosetta) and
`heroes-linux-arm64`, each building the compiler from the seed at HEAD and the
program with `heroes build -O2`, then running the binary.

| program | the shape | `check` | run | `--sanitize` |
|---|---|---|---|---|
| `read_after_consume.hero` (+ `cj.h`) — **088** | `cJSON_Delete(item: b)`, then `b` to the NON-consuming `object` of an add | 0 | **0**, three of three, all three legs; it prints *wrote into a deleted object* | `heap-use-after-free`: 134 Darwin, 1 Linux |
| `use_after_consume.hero` | the same dead `b`, to a CONSUMING parameter | 0 | 134 before C, 399 B, three of three, both Linux legs; 134 Darwin | 134 *never taken* |
| `helper_consume_then_use.hero` | `finish(@b)` releases it, the caller reuses `b` as `item` | 0 | 134 before C, 399 B, both Linux legs; 134 Darwin | 134 |
| `u1_static.hero` (+ `shapes.h`) — **077** | a C `f_open` returning one static cell: `f_close(x: a)`, `b = f_open()`, `f_close(x: a)` | 0 | **0, zero bytes, ten of ten at `-O0` and ten of ten at `-O2`**, Darwin; three of three both Linux legs; prints `true` | **0** on all three — ASan does not track a static cell |
| `reuse_malloc.hero` (+ `node.h`) — **077** | the same over `malloc`'d same-size nodes | 0 | **0, ten of ten at both levels**, Darwin; 0 both Linux legs | 134 *never taken* — ASan's quarantine gives a fresh address, so the stale one is a stray |

So the dead handle is stopped before C exactly where it reaches a CONSUMING
parameter and its address is not live again. Everywhere else it reaches C.

### Routes, and where each came from

Enumerated from panel 175's reports (the spec-warden's *route nobody listed*,
its report line 111), the coordinator's draft of 2026-09-23, and this morning's
reading of `selfhost/check/` and `runtime/parts/alloc.c`. **The list is a
measurement too**: say what would have to be true for a route that is not here.

- **M — a moved binding, at check time.** A binding handed to a consuming
  parameter may not be read again in that function. `check/consuming.hero:22`
  and `check/leasing.hero:29` both state *the checker has no flow analysis*;
  `check/join.hero` joins branch TYPES, not binding states (read at its head).
  So branches and loops are the question, and so are copies made before the
  call, which M cannot see and which is the affine handle itself.
- **R — a liveness check at every handle-taking call, at run time.** A handle
  of a type any `extern` consumes, reaching a NON-consuming parameter, must be
  in the live set, or the call aborts before C. Catches the 088 shape; cannot
  catch 077, whose stale address is live again. **The cost nobody has priced**:
  a `borrows` handle is never in the set (`examples/` has 0 `borrows`, 5
  `consumes`, 5 `acquires`; `tests/golden/` 7, 32, 26 — lines, comments
  excluded, `grep -rE "\b<word>\b" --include='*.hero'`), so R as stated
  refuses every correct call that hands on a borrowed handle; and one lock and
  one probe per call.
- **P — the binding is poisoned at run time.** After a consuming call whose
  argument is a plain binding (or an `@` cell), the emitted C overwrites that
  binding with a value no allocation returns, and a handle-taking call handed
  that value aborts before C. The coordinator's route, unbuilt: it reaches
  branches and loops without flow analysis and an `@` helper through its cell,
  and misses a copy made before the call. What it does to `==` on a dead
  handle (`u1_static` prints `a == b`) is a question.
- **S — a serial per acquisition, carried by the value** (panel 175's
  spec-warden). The value becomes C's pointer plus a number the set issued; a
  handle whose number is not the live entry's is refused before C runs. Closes
  077 as well as 088. Costs the ABI of every handle, the emitted C of every
  marked call, `==`, and a handle stored in a group record's field, which is
  C's struct and cannot carry the number.
- **G — a generation in the set only.** Cannot work alone: the stale handle and
  the live one are the same bits. Listed so it is refused with its reason.
- **A — the affine handle**: copies refused. Core by §1.7, Part 6's row's cost.
  Listed so the row's falsifier is judged, not assumed.
- **D — the documented limit**, which is what spec § 13 says since `a747e5a2`:
  *"Giving one back twice aborts, unless C has since reused its address"*
  (`:387-388`). §1.12 is why it cannot be the answer alone.

---

## Question 2 — the reader test panel 176 put off (its item 9)

Panel 176 (`docs/panel/176-a-transfer-names-where-the-life-goes-and-a-reference-joins-the-life-it-finds.md`,
provisional) adopted three forms whose spellings no reader had read:

- **a transfer names the releaser it hands the life towards**, checked against
  the handle's set before C runs: `val: Json transfers json_object_put`;
- **a transfer that happens only on success says so**, naming the result that
  means success — the libraries disagree: json-c 0, OpenSSL's `add0`/`set0` 1,
  cJSON `true`, `SSL_set0_rbio` `void` and unconditional (panel 176's critic,
  its § 2; the critic's emulated run: a correct failure path 0, a leaking one
  134 at exit);
- **a reference is its own mark on a result or a parameter**, `retains
  <releasers>`, joining the live entry and keeping its set: json-c's
  `json_object_get` returns the object it was given; OpenSSL's 27 `_up_ref`
  take one and return a status (panel 176's ffi-pragmatist).

**The library facts in these three bullets are CARRIED** from panel 176's
reports — the success values from its critic's § 2, the 27 `_up_ref` from its
ffi-pragmatist — and were not re-run for this brief; a seat that leans on one
re-runs it.

The llm-ergonomist reads them blind; the spec-warden prices them on the real
instrument; the compiler-engineer prices the success clause, which nobody has.

---

## What is measured on the spec now

`./heroes measure spec/heroes-spec.md` at HEAD, with `.env` loaded: **real 8361**
(claude-opus-5, 2026-09-23), maximum vendored 6282, headroom **1879** against
the 10240 ceiling, the FFI floor mortgaging 60 of it. The author's word of
2026-09-23 — *pay all the tokens, without economising* — is recorded in panel
176's synthesis and its log entry; it does not lift design.md §1.6's payment
rule.

## Unrun, said here rather than found later

- **Windows**, for every row above: the box was off all morning
  (`ssh win` timed out at 09:00, 09:50 and 10:03).
- The Linux numbers for `u1_static` and `reuse_malloc` at `-O0`.
- Any route above, built.

## Where each seat works

`<scratchpad>` is `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad`.
Each seat's own directory is `<scratchpad>/177-<seat>/`, a `git archive` of HEAD
`521c5e02` that the coordinator made before launch. **Build your own compiler
inside it from the seed** (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, three seconds) and set `HEROES_RUNTIME` to its `runtime/`; never use
the trunk's `./heroes` or another seat's; never read, build or run inside
another seat's directory. The trunk is read-only for every seat. Write your
report to `<scratchpad>/177-<seat>/REPORT.md`; the coordinator copies it to
`docs/panel/177-reports/<seat>.md` unchanged.
