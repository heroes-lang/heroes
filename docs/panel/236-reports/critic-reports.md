# Panel 236, the completeness critic's second pass: the reports

Measured 2026-11-08 in `.claude/worktrees/236-critic`: the base (`sources 2fa3d81374af3684`), the
compiler-engineer's R7b route rebuilt here from `route-final.patch` (`sources 1699bb340bef9aeb`, the
seat's key) and its R-P from `route-rp-Bip.patch` (`17a3862c60d14c24`, the seat's key); the worktree
restored to the base after. Outputs in `.claude/worktrees/236-critic-scratch/r2/`. Runs `timeout 60`,
`head -c 65536`; frames with `flags()`'s words; cost in instructions.

## 0. First: the proposal's route did not build

The ffi seat's route, P with typed crossings, exists only as hand rewrites of one-unit programs: its
worktree holds no tracked change (`git status`), as its § 8 says. The emitter's R-P refuses
`threemod` here as for the seat, exit 1, `ffi_unknown_name` blaming `holder.hero:5`, a false message:
R-P writes its `static` adapter into every unit, `library.c` included, which reads no program header.
R7b with rule B builds and runs (§ 2). Under frame 5 the synthesis can adopt R7b, or wait for P built.

## 1. The typed crossing does not close `threemod`; one adapter per program does

R-P's own four units of `threemod`, edited by hand (`r2/tm/variants.sh`, each anchor asserted once):

| variant, both levels | run |
|---|---|
| V1: adapters only in units that read the header, R-P's untyped table | `panic: a null pointer was read through, at offset 0x5, called from caller.hand`, 134 |
| V2: V1 with the ffi seat's typed crossing in `caller.c` | the crossing's panic, 134 |
| V3: one external adapter in `holder.c`, declared in `caller.c`, the crossing against it | `6`, `6`, 0 |

V1 is the compiler-engineer's "unrun" inference, run: `caller.c` compares the value with its own
`static` copy, never `holder.c`'s. The crossing turns that into a loud stop; only the single
definition makes the program run. `library.c`'s failure is placement, not typing: each of the four
`library.c` read holds the table and no program typedef (`grep`: 1 and 0), so a crossing or assertion
naming `h_0fn_`/`h_0fc_` stands only in a unit making the crossing. Nobody built either; and
`held_make`, `static` in the header, cannot be named from a unit reading another header.

## 2. R7b with rule B: the witnesses, re-run

At both levels, `grpnoext`, `grpgen`, `grpfix2`, `grpdisc`, `grpbyval`, `grpwrap`, `big8k`, `gn12`,
`ga12`, `ga16`, `cbcall`, `twomod`, `threemod`, `thkbig`, `thkres`, `padhero`, `padwrap`, `extval` and
the three `cb` probes print the seat's outputs; five, sampled, clean under `--sanitize`; C's view of the
four probes is the base's byte for byte (17, 15, 15, 15 lines); the base refuses `thkbig` and
`padwrap` at exit 2, as found. The listed aborts abort: `grpopt` (the room), `grpret`, `grpextonly`,
`grpval`. Not listed: `grpvalparam` (§ 4).

## 3. Part (a): the array chain closes, the named chain does not

| unit, clang `-O2` | base | route |
|---|---|---|
| `ga16` | 460,606,411,427 / 6,995,531,848 B | 831,827,701 / 16,433,560 B |
| `gn12` | 12,475,836,488 (first pass) | 3,188,874,133 / 46,219,744 B |
| `gn16` | not run: `gn14` alone 85,127,564,746 / 1,669,515,136 B | **51,025,955,600 / 631,735,256 B** |

`gn16`, the named chain at 1 MiB, ran on neither seat's route: six times panel 233's bound, its
front end 47,218,886,381 (92.5 %) over 82 MB of C, panel 215's padding table; the ffi seat's
prediction holds, and the compiler-engineer's part (a) and case 6 rest on the array chain alone. The
base's `gn16` would need about 17 GB at `gn14`'s growth (inference). **And at run time**:
`gn16`'s `-O0` binary spends 637,528,212,805 instructions on the route and 637,378,953,468 on the
base (which then aborts in `chain.make14`), `-O2` 241,445,014,882, while the only work the unit adds
before `main` is a constructor calling `hero_pad_gaps` over 131,072 spans, which rescans every span
per position (`runtime/parts/lend.c`; unprofiled). Found beside, unfiled.

## 4. Shapes of 2286 neither seat built

- **`grpvalparam`**, the record passed by value through a function value: on R7b `go`'s frame is
  134,217,872 bytes at `-O0`, `stack exhausted`, 134; `-O2` runs only because clang folds the call
  (the unit's own `go` reads 134,217,792). R-P runs it at both. R7b's price has two directions.
- **`grpthread`**, `make` called on a spawned thread: base `stack exhausted in grpthread.worker` at
  both levels; R7b and R-P run it at both, R7b clean under `--sanitize`. A case owed.
- **`gn16`**, compiled and run (§ 3).

## 5. `grpval` and `xvalret`: the same shape

A call through a function value returning the 128 MiB record, read in Heroes; the value is a
parameter in one, a local in the other. Same verdict on every compiler: base and R7b abort at both
levels, R-P runs both at both. They differ at `-O2` only in where: `grpval` inlines `go` into `main`
and never prints `before` (`stack exhausted in grpval.main`), the ffi seat's adjacent finding in a
witness; `xvalret` prints it.

## 6. Contradictions, each settled by a command

- **The thunk's copy.** The ffi seat: *both routes remove today's thunk copy of a by-value
  parameter*. On the built R7b, `-fstack-usage` on `cbcall`: the thunk `h_0cb_ro_value_h_cbcall_first`
  is 262,208 bytes at `-O0`, the base's own figure, and 32 at `-O2`. True of the hand rewrite only.
- **The assertion's form.** The ffi seat's per-thunk assertion names the typedef; `library.c` holds
  the table and no program typedef (§ 1), so that form cannot stand in the table. The
  compiler-engineer's twin form, beside the thunk, compiles (two assertions in `cbcall`).
- **Two "proposals as written".** The ffi seat's P keeps `h_0fn_` as C's typedef and adds `h_0fv_`;
  the compiler-engineer's R-P renames C's to `h_0fc_` and gives `h_0fn_` the program's convention,
  so its probe lines differ from the base's. "C's view byte for byte" holds for the first spelling
  only; neither is the other seat's route.

## 7. Claims not settled by their command

- The ffi seat's *no C spelling lets an extern write its result outside its caller's frame* rests on
  four spellings; panel 233's `room4.c`, its one 32-byte case, reads 262,192 bytes under `flags()` at
  `-O2`: holds for five, still a question for the rest.
- The run cost the route moves (defect 2334's *1,025 copies of 8 KiB per call*): 1,000 calls of
  `total` at `-O2` read 76,929,897 instructions on the base and 1,775,060,970 on the route (medians of
  three), about 30 times per call, peak 1.4 MB against 11.1 MB. No seat measured a run.
- Every x86-64 and Windows statement in both reports is `-target` reading or unrun.

## 8. What the sitting should have asked

1. The multi-unit questions a one-unit rewrite cannot meet: one adapter per program, where a
   crossing stands, and what the per-module cache keys on.
2. The third route the compiler-engineer names and nobody built: `hero_out` for function types no
   `extern` names, C's for the rest, closing `grpval` and `grpvalparam` without touching the
   boundary; true only if the emitter knows that set for the whole program and each unit's key
   covers it.
3. The run cost at `-O2` of every program the route converts (§ 7; the sixteen moved corpus programs
   unmeasured).
4. Part (a)'s named chain at 1 MiB and its run before `main` (§ 3), with the bound that closes it.
5. Both directions of a call through a value (§ 4).
