# Panel 182, compiler-engineer: which zeroing a release needs

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5), soundness lane, defect 114.
Tree: `git archive 0fc98107` into
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/compiler-engineer/`,
`rm -rf build`, compiler built from `seed/heroes.c` with CLAUDE.md § Commands'
first line; its own `--emit-c` of `selfhost/main.hero` is byte-identical to the
seed (`cmp`). Every number below comes from a command run in that directory on
2026-09-28 between 01:35 and 05:25; a sentence that is an inference says so.
Apple clang 21.0.0 (clang-2100.3.34.2), arm64, 8 cores (4 performance). Other
seats ran throughout: every timing records the load average and the ratio of
real to user plus sys, and runs whose ratio shows waiting are discarded.

## The verdict

- `verdict`: **object**, to the repair as `docs/work/DEFECTS.md:36-49` frames
  it: that the zeroing is the release's precondition, and that removing it
  without changing the lowering "would trade a robustness guarantee for
  speed". Approve **(a-min)** now; approve **(f)** as the architecture step,
  on the conditions below; refuse **(e)**'s analysed slot zeroing as it stands;
  **veto (c)**.
- `section`: design.md §1.7 (*"does it move something from the core to the
  sugar, or remove a special case from the compiler?"*), §1.12 (*"where two
  admissible forms disagree and one of them can be made to crash, the other
  wins"*), Part 2 (*"Performance is not a goal"*), and panel 021 R3, which
  licensed zeroing for refcounted **slots** only.
- `implementation_cost`: (a-min) **+75 −7** in `selfhost/emit/body.hero`
  (333 → 333 lines) and `selfhost/ir/values.hero` (256 → 324, 40 of them the
  test). (b) **+60 −6** in `selfhost/ir/own.hero` (354 → 408). (f) = (a-min)
  + (b) + initialising stores: **+398 −23** in five files, one new
  (`selfhost/ir/assigned.hero`, 226; `own.hero` 354 → 430; `emit/unread.hero`
  355 → 360). (e) = (f)'s ownership half plus an emitter dataflow
  (`selfhost/emit/zeroing.hero`, new, 248): **+575 −18**. (a) as briefed:
  **+252 −2**. (c) not prototyped; a reset op alone would join **57
  non-comment lines naming `.decref_slot`, in 24 files**. Everything lands in the ownership pass,
  the verifier and the emitter's prologue; nothing in the lexer, parser,
  checker or descriptors. The compiler is 74,955 lines in 272 files, so (f)
  is 0.5% of it.
- `needed_for_self_hosting`: **no**. The compiler self-hosts at `0fc98107`
  with every zeroing in place; this is a defect repair, not a form.
- `argument`: The defect's note says removing zeroing trades robustness for
  speed. For 81,680 of 102,990 zeroings that is false: no value is read or
  released before its one definition, and four instruments agree. Zeroing
  values is the special case §1.7 asks us to remove: `emit/body.hero`'s own
  header documents no such thing, the Rust port carried it unargued, and it
  costs `-Werror=uninitialized` coverage, panel 021 R3's stated price. (a-min)
  removes it in 75 lines and adds the verifier's missing order check. The slot
  zeroings are owed because rule 3 loads every slot before its first store;
  (f) drops that load where nothing was written, and fails as a leak. (e)'s
  analysed slot zeroing fails as corruption no build-time instrument sees.
  Veto (c).
- `prediction`: whichever of (a-min), (d) or (f) has landed by the
  **M-agreed-retention close**: (1) `grep -c '= {0};' seed/heroes.c` is at
  most **22,000** (102,990 at `0fc98107`; measured 20,696 for (a-min), 17,116
  for (f), each on its own source); (2) `clang -fsyntax-only -Wuninitialized
  -Wsometimes-uninitialized -Wconditional-uninitialized -I runtime
  seed/heroes.c` reports **0** warnings; (3) `heroes fmt` on sixteen
  concatenated copies of `selfhost/check/walk.hero`, run by a seed built with
  the plain `clang` line, reads **user ≤ 0.85** of the `0fc98107` seed's on the
  same machine in interleaved runs (measured 0.78 for (a-min), 0.70 for (f)).
  Any one of the three failing falsifies it.
- `condition`: on (a-min), a program whose C with values unzeroed clang
  refuses, or that crashes under `-ftrivial-auto-var-init=pattern`, or on which
  the new order check fires: then value zeroing is not dead in general. On (e),
  an instrument on every build or on a CI leg that sees a release through `&`
  of an unzeroed slot, with its negative control shown to fire, **and** the
  clang flag list in the unit cache key (`selfhost/cli/units.hero:82-84`), and
  a measured gain over (f); I measured none. On (c), a prototype with no new IR
  op and no change to `phases.released_on_return`. On (f), if its gain over
  (a-min) measured within noise, Part 2 would say keep rule 5 as it is; I
  measured 11% to 14% at `-O0` and 10% at `-O2`.

## 1. Which zeroing is dead, over the whole compiler

Census of `seed/heroes.c` at `0fc98107` (identical to the compiler's own
emission), each `<type> <name> = {0};` classified by its name:

| class | zeroed | needed | dead |
|---|---:|---:|---:|
| values (`t<N>`) | 81,680 | **0** | **81,680** |
| synthetic slots (`h<N>_own<N>`, rule 5) | 12,897 | 12,897 | 0 |
| named slots (locals, loop slots, `@` parameters) | 8,413 | 7,780 | **633** |
| total | 102,990 | 20,677 | 82,313 |

The 633 dead named slots are **all** mutable `@` parameters: the prologue
writes each from its pointer (`h2_owned = *ph2_owned;`) before `goto bb0`.

**How it was decided, two instruments that agree class for class.**

- **C level.** Every `= {0}` stripped from the seed, `#line` removed so each
  warning maps to a C line, then `clang -fsyntax-only -Wuninitialized
  -Wsometimes-uninitialized -Wconditional-uninitialized`: 20,928 warnings,
  deduplicated to (function, variable) pairs: **0 values**, 12,897 of 12,897
  synthetic slots, 7,780 of 8,413 named slots. Every unflagged named slot has a
  copy-in line in its function (633 of 633).
- **IR level.** Route (a)'s prototype, a definite-assignment answer computed
  per function beside `unread.defs` (values by dominance plus order within the
  block; slots by a forward may-be-unassigned fixpoint over the live blocks,
  counting loads, partial stores, place stores, copy-outs, sweeps and `@`
  arguments as reads). On its own source: values owed **0 of 81,942**,
  synthetic slots 12,918 of 12,918, named slots 7,820 of 8,457 (the 637 dead
  are the `@` parameters).
- **Clang's blind spot, measured** on a six-function file: clang warns on a
  struct local read **by value** (always, or on one branch), and says nothing
  when only its **address** reaches a call (`rel(&o)`, always or sometimes
  uninitialised). The exit sweep of an aggregate slot is exactly that shape
  (`h_0opt_316a1e6d_release(&h23_own23);`), so the C-level census sees a slot
  only because of the next point.

**Why every slot is owed: rule 3's old-value load, not the arms.** Every store
into a refcounted slot is emitted as load old, store, release old, and the
load is a by-value read of the slot before its first write. In
`h_keywords_keyword` the scrutinee binding is read before anything writes it:

```c
    t111 = h1_s0;
    hero_str_incref(t1);
    h1_s0 = t1;
    hero_str_decref(t111);
```

So the coordinator's note holds for the slots, and for a reason wider than it
gives: the precondition comes from the store's uniform shape
(`selfhost/ir/own.hero:133-159` and `:188-198`), not only from one owned
variable per arm. In that function the 139 zeroings are 25 slots, all owed,
and **114 values, all dead**.

## 2. The routes, priced

Each prototype lives in `variants/<route>/` in my directory. Each was
regenerated through a stage-1 compiler, its seed emitted from `selfhost/`,
rebuilt, emitted again and compared: **the fixpoint holds for every route**.
Zeroing counts compare the trunk's emitter and the route's on the **same**
source (the route's own compiler).

| route | lines | zeroings, trunk emitter → route | C lines of the compiler | `h_keywords_keyword`: lines · zeroings · releases |
|---|---|---:|---:|---|
| trunk | none | 102,990 | 974,557 | 941 · 139 · 71 |
| (a) dataflow | +252 −2, 2 files | 103,317 → 20,738 | 978,411 | 941 · 25 · 71 |
| (a-min) | +75 −7, 2 files | 103,080 → 20,696 | 975,385 | 941 · 25 · 71 |
| (b) destination | +60 −6, 1 file | 103,131 → 95,827 | 976,106 → 905,062 | 655 · 95 · 27 |
| (c) join release | not prototyped | not measured | | |
| (d) = (a-min)+(b) | +135 −13, 3 files | 103,221 → 17,071 | 905,835 | 655 · 3 · 27 |
| (e) = (a)+(b)+init | +575 −18, 5 files | 103,798 → 12,467 | 983,518 → 844,004 | 581 · 1 · 3 |
| (f) = (d)+init | +398 −23, 5 files | 103,561 → 17,116 | 841,223 | 581 · 3 · 3 |

**(a), zero only what a release or a read can reach unwritten.** Built as
briefed (`selfhost/emit/zeroing.hero`, 248 lines). Measured: on the compiler
and on **300 of 332** programs (the four `emission` groups and every example
`main.hero`), its C is byte-identical to (a-min)'s; the other 32 exit non-zero
under both, with the same codes. Its `emission` blessing rewrote the same 259
files and **the same 21,863 lines** as (a-min)'s. Under the current lowering the
dataflow computes a two-clause rule, *never zero a value, never zero an `@`
parameter's slot*, and costs emission time for it (below). It also carried a
defect of its own class in its first draft (section 5).

**(a-min), the two clauses themselves.** `emit/body.hero` prints `= {0}` on a
refcounted slot unless it is a parameter, and never on a value;
`ir/values.hero` gains `in_order`, the check the verifier lacked: its
dominance test is block-level (`selfhost/ir/values.hero:84-115` compares
blocks only), so a read above its definition in the same block passed it. The
check did not fire on the compiler or on any suite program; its own test makes
it fire. Removes 82,313 zeroings (80% of the seed's).

**(b), one destination for the arms.** The lowering already writes every arm
into one slot (`h2_r0`); the 23 per-arm slots are rule 5's synthetic slots,
one per owning temporary. The prototype moves an owning temporary whose
**only** use is a whole-slot store later in its own block straight into that
slot: no synthetic slot, no incref, no exit release. On the compiler it removes
3,652 of 12,906 synthetic slots and as many old-value temporaries; in the
keyword lookup 23 of its synthetic slots go, leaving one. **What rule 5
becomes:** *an owning temporary is moved into a slot in the block that defines
it: into the one slot its only use stores it to, whole, later in that block, or
else into a synthetic slot of its own.* **What it breaks:** the uniform reading
in `own.hero:32-35` (*every value that reaches a store is borrowed, so a store
always increfs*), which becomes *a store increfs unless it consumes the
temporary it stores*. The verifier's `owning_temporaries_move_into_slots`
(`selfhost/ir/phases.hero:259-291`) still holds unchanged, because the
consuming store is in the defining block; every build after it passed the
verifier. It changes 7 of the 23 `ir` goldens (147 diff lines), which may not
be regenerated (CLAUDE.md § Hard stops), so each is edited with its diff read.
The prototype carries no unit test of its own; a landing owes one (unmeasured).

**(c), an arm's slot released where the arm joins.** Not prototyped, and the
reason is its price rather than its difficulty. By itself it removes **no**
zeroing: the exit sweep still reads the slot on every path where the arm did
not run. To make that zeroing dead it needs (1) a per-slot
unassigned/held/released dataflow in the ownership pass, of the size of
`assigned.hero` (226 lines, measured in (e)); (2) releases placed on the edges
that leave the defining block's region, with a new block wherever the arm's
last block branches; (3) per-return sweep lists, which means changing
`phases.released_on_return` (`selfhost/ir/phases.hero:150-190`), since today
it requires every owed slot swept at every return, so the verifier would share
the pass's answer instead of checking it; (4) wherever a loop brings the
released slot back to a store's old-value load, a way to reset it to the null
non-value, and the IR has no zero-aggregate literal: a new op joins the 57
non-comment lines that name `.decref_slot` today, in 24 files. That is the
liveness pass panel 021 R3 refused (*"the alternative is a liveness pass"*) and
the draft rule 5 records as refuted within the hour (*"decref at end of
defining block"*, `own.hero:29-31`); its failure mode is a double release. The
most it could remove beyond (e) is the 12,467 zeroings (e) keeps. That total
is measured; the cost above is counted from the code, not built.

**(d), (a-min) and (b).** On its own source: values 0, synthetic slots 9,260,
named slots 7,811.

**(e), (a) and (b) plus initialising stores.** `ir/assigned.hero` finds each
whole-slot store onto a slot no path has written, where the pass then prints
the store alone, without the old-value load and release; a synthetic slot in a
block no loop returns to is stored the same way. `emit/zeroing.hero` then zeroes
only the slots some sweep or read can meet unwritten. It changes 15 of the 23
`ir` goldens (167 diff lines).

**(f), (d) plus initialising stores, with every non-parameter slot still
zeroed.** The route none of the four names. It keeps (e)'s ownership half,
whose wrong answer can only leak, and drops (e)'s emitter half, whose wrong
answer is an uninitialised release. It also needs the `unread.slots_read` fix
in section 5.

**The suites, one at a time in my copy**, each route's own seed installed:

| route | fixpoint | own tests | `run` | `emission` (blessings changed) | `determinism` | `ir` changed | `emit` changed | `canonical` |
|---|---|---|---|---|---|---|---|---|
| (a) | yes | 788 passed | 210/210 | 259 files, 21,863 lines | green | 0 | 4 | green |
| (a-min) | yes | 789 passed | 210/210 | 259 files, 21,863 lines | green | 0 | 4 | green |
| (b) | yes | 788 passed | 210/210 | 153 files, 74,764 lines | green | 7 | 2 | green |
| (d) | yes | 789 passed | 210/210 | 259 files, 92,866 lines | green | 7 | 4 | green |
| (e) | yes | 788 passed | 210/210 | 259 files, 103,670 lines | green | 15 | 4 | green |
| (f) | yes | 789 passed | 210/210 | 259 files, 101,705 lines | green | 15 | 4 | green |

`emission` goes red only on bytes, and was blessed by its own procedure
(`UPDATE_EMISSION=1 heroes run tests/harness/main.hero -- <compiler>
emission`), then re-run: **636 passed, 0 failed** for every route; the store was
restored from the archive afterwards. For (a) and (a-min) every rewritten line
is a dropped `= {0}`. The `emit` goldens hold whole emitted C and have no
regenerator: 2 or 4 of them are rewritten by hand, diff read. `run` builds every
case at `-O0`, at `-O2` and under ASan plus UBSan, and every generated `main`
checks for leaks (`runtime/parts/alloc.c:255`).

## 3. Robustness: where each route could read or release garbage

**Instruments, measured before they were trusted:**

- clang, in every `heroes build` (`-Werror=uninitialized` and
  `-Werror=conditional-uninitialized`, `selfhost/cli/flags.hero:99,101`): sees
  by-value reads only (the six-function file above). A control compiler that
  zeroes nothing has its C refused (`error: variable 'h3_own3' is
  uninitialized when used here [-Werror,-Wuninitialized]`).
- ASan: blind to uninitialised reads. A C program that releases an
  uninitialised struct through `&` drew 0 clang warnings; built plainly it
  printed `refcount now 512` where 2 was stored and exited 0; under ASan plus
  UBSan it exited 0 with no report. MemorySanitizer, the tool for this class,
  is refused here: `unsupported option '-fsanitize=memory' for target
  'arm64-apple-darwin25.6.0'`.
- `-ftrivial-auto-var-init=pattern`: the same program exits 139; with ASan it
  reports `SEGV ... in rel`. Negative controls: the trunk seed with every
  zeroing stripped, built with the pattern, dies at its first `check`
  (exit 138); a control compiler that leaves every aggregate slot unzeroed
  (with its two `-Werror` uninitialized flags removed so clang lets it through)
  fails **23 of 210** `run` goldens under the pattern: 19 die by SIGSEGV (18) or SIGBUS (1) where output was expected, and 4 that must abort with a named message die having said nothing.
- The runtime's own guards: the same control **without** the pattern fails
  the same 23 goldens, mostly by the runtime's aborts (exit 134, one 133:
  `a str's block has lost its mark`, the null-read handler, a SIGTRAP report). They catch garbage that is not a valid
  block. A stale pointer to a block still alive would pass them; that case is
  shown silent only in my C reproduction, and a Heroes-runtime reproduction of
  it is **unrun**.
- The verifier (every build, `selfhost/cli/compile.hero:202`): dominance
  before (a-min), dominance plus order after it. It knows nothing of zeroing.
- The leak counter: it caught a real defect in my (e) prototype (section 5).

**Route by route:**

- **(a-min).** Path: a value read or released before its definition. Across
  blocks the verifier's dominance refuses it and within a block the new order
  check does, both at exit 2 on every build. A by-value read the emitter makes
  outside the IR's operands is refused by clang on every build. A value's
  release or retain through `&` is always an `incref` or `decref` operand, so
  the verifier covers it. Measured: 0 warnings over the whole seed; a
  pattern-initialised build of the compiler compiles itself to identical
  bytes; (d) under the pattern on an emptied `build/`: `run` 210/210.
  **(a-min) gains robustness**: it returns 81,680 locals to the clang check
  that panel 021 R3 measured the zeroing takes away (*"`{0}` converts a compile
  error into a silent `""`"*).
- **(a).** The same C, but produced by an analysis that has to be right.
  Nothing is gained over (a-min).
- **(b).** Zeroing unchanged. Path: a consumed temporary read after the store
  takes it, which the single-use rule excludes; if it happened, it would be a
  use-after-free or double release, caught by ASan in `run` and by the leak
  counter. Measured: `run` 210/210.
- **(c).** Path: a slot released at the join, then swept at exit or reloaded
  by a store's old-value load in a loop, is a double release. ASan sees that
  only on paths a test executes.
- **(e).** Path: the emitter's dataflow wrongly calls a slot's zeroing dead,
  and the exit sweep releases an uninitialised aggregate through `&`. Clang
  does not see it, ASan does not, the verifier does not. On executed paths the
  pattern and the runtime's marks do; on the rest, nothing. Measured green: the
  fixed (e) under the pattern on an emptied `build/`, `run` 210/210. **Refused
  as it stands** on §1.12 and the brief's rule, and it buys nothing over (f)
  (section 4).
- **(f).** The only new analysis decides initialising stores. A wrong yes
  forgets to release a real old value: a **leak**, which
  `hero_runtime_check_leaks` reports in all three legs of `run`. It reported
  exactly that when my first draft was wrong. Slot zeroing stays unconditional.
  Measured: under the pattern on an emptied `build/`, `run` 210/210.

## 4. The time

Same inputs for every compiler: `heroes fmt` on 36,688 lines (sixteen copies
of `selfhost/check/walk.hero`, which is 2,293 lines at `0fc98107`, so one line
more per copy than the brief's 36,672), `check` and `build --emit-c` on the
trunk's `selfhost/main.hero`. User seconds, three interleaved rounds (one run
of each compiler per round), load 2.66 to 4.30, ratio 1.00 to 1.05. Excluded:
`fmt` runs with ratio 1.17, 1.49 and 1.63 (waiting), and round 1 of
`--emit-c`, whose sys was 4.2 to 4.8 s against 0.1 s in rounds 2 and 3, for
every compiler alike (cause not identified).

| compiler | `fmt`, seed at `-O0` | `fmt`, seed at `-O2` | `check`, `-O0` | `--emit-c`, `-O0` |
|---|---|---|---|---|
| trunk | 3.18 · 3.11 · 3.11 | 0.73 · 0.72 · 0.72 | 4.32 · 4.26 · 4.22 | 35.57 · 35.50 |
| (a-min) | 2.45 · 2.42 · 2.43 (0.78) | 0.67 · 0.67 · 0.67 (0.93) | 3.33 · 3.31 · 3.33 (0.78) | 32.59 · 32.50 (0.92) |
| (d) | 2.23 · 2.23 · 2.21 (0.72) | 0.63 · 0.62 · 0.63 (0.87) | 3.02 · 3.04 · 3.00 (0.71) | 29.69 · 29.53 (0.83) |
| (e) | 2.16 · 2.16 · 2.17 (0.69) | 0.61 · 0.62 (0.85) | 2.93 · 2.94 · 2.91 (0.69) | 29.46 · 28.71 (0.82) |
| (f) | 2.17 · 2.16 (0.70) | 0.60 · 0.60 (0.83) | 3.05 · 2.95 · 2.91 (0.69) | 27.94 · 28.06 (0.79) |

Ratios in parentheses are the median against the trunk's. From an earlier
matrix at load 4.2 to 12.9 (the `--emit-c` rows interleaved, the rest not): (a) `fmt -O0` 2.52 · 2.51 · 2.56,
`check` 3.48 · 3.50 · 3.49, interleaved `--emit-c` 36.48 · 36.60 · 44.06
against (a-min)'s 33.72 · 35.06 · 40.12, so the dataflow costs emission time;
(b) alone `fmt -O0` 3.30 · 3.14 · 3.12 against the trunk's 3.09 · 3.10 · 3.11,
no gain on its own.

**What `-O2` does with each route's C.** In the compiler binaries, `memset`
and `bzero` call sites: trunk 2,323 at `-O0` and **2 at `-O2`**; (a-min) 323
and 2; (b) 2,305 and 2; (d) 314 and 2; (e) 243 and 2. At `-O2` the optimiser
removes the zeroing calls whatever the route, which is why `fmt` moves only
7% there for (a-min). What (d), (e) and (f) still win at `-O2` (13% to 17%) is
the ownership traffic they no longer emit, not the zeroing. `sample` of the
same 2 s of `fmt` at `-O0`, `memset` at the top of the stack: trunk 503,
(a-min) 126, (d) 87, (e) 70, of about 1,340. Binary sizes at `-O0` and `-O2`:
trunk 7,194,376 and 5,076,040; (a-min) 6,253,160 and 4,498,088; (d) 5,906,424
and 4,085,160; (e) 5,544,424 and 3,689,912.

**(f) equals (e)** within noise in every column, so the analysed slot zeroing,
the only part of (e) that can corrupt, buys nothing measurable.

## 5. Found beside the question

1. **`unread.slots_read` does not count the exit sweep as a read**
   (`selfhost/emit/unread.hero:268` lists `.decref_slot` among the arms that
   `continue`), while `selfhost/ir/uses.hero:130-135` and
   `selfhost/emit/body.hero:168-169` say the sweep is what keeps a swept slot
   counted as read. The trunk never shows it because rule 3's old-value load
   reads every refcounted slot first. Remove that load, as (e) and (f) do, and
   5,468 zeroed slots gain a false `__attribute__((unused))`; the fix is one arm
   (in `variants/e`, `variants/f`), and the count returns to the trunk's 40.
2. **The dataflow routes' own failure, observed.** My first fixpoint in
   `assigned.hero` and `zeroing.hero` recorded a block's entry set only when
   its exit set changed. A back edge that grows the entry set of a block which
   overwrites the slot anyway leaves it stale. In `assigned.hero` that called
   a loop's store an initialisation, and two `run` goldens leaked (**399,999**
   and **2** heap blocks at exit), caught by the leak counter alone. In
   `zeroing.hero` the same bug was latent: fixed and unfixed builds zero the
   identical set on the compiler. The corruption half of that one mistake was
   seen by no instrument; it simply did not happen to change an answer.
3. **The unit cache key omits the clang flags and the compiler's identity.**
   `selfhost/cli/units.hero:82-84` keys an object on the fingerprint, level,
   module, text, runtime and search paths, and the fingerprint is `VERSION`
   (`selfhost/main.hero:192`). Two compilers of one version that emit the same
   text share an object whatever flags built it. It could have contaminated my
   first (d) pattern probe, wherever (d) emits the text (a-min) emitted in its
   earlier unpatterned `run`, so every probe was re-run on an emptied `build/`,
   with the same results, and any pattern leg proposed as an instrument needs its own build directory or
   the flags in the key. That a flag added to `flags.hero` (as `-fsigned-char`
   was) reaches no object already cached for unchanged text is an inference,
   **unrun**.
4. **Panel 021 R3's rider is half built.** The runtime half exists
   (`runtime/heroes_runtime.h:67`, the null pointer as the one non-value). The
   *"phase-Owned invariant that every slot is stored before it is loaded on every
   path"* does not exist in `selfhost/ir/` (searched: `unassigned`, `stored
   before`, `before it is loaded`), and under rule 3's old-value load it could
   not hold.
5. **The value zeroing was never argued.** `selfhost/emit/body.hero:5-13` says
   nothing is initialised except refcounted slots (*"The ONE exception (panel
   021 R3): a refcounted slot"*), while lines 204-205 zero refcounted values
   too. `archive/bootstrap-rs/heroes/src/emit/body.rs:178` did the same with no
   comment, where its slot line (134-146) carries the whole reason.

## 6. Not done, or unrun

- (c) not built; its cost is counted from the code.
- No unit tests of their own for (b), (e) or (f)'s ownership changes; a
  landing owes them, unmeasured.
- Linux and Windows not measured; every platform fact here is this Mac's.
- The `ir` and `emit` goldens were counted, not rewritten.
- A Heroes-runtime reproduction of a stale live pointer released through `&`:
  unrun.

Paths: prototypes `variants/{a,amin,b,d,e,f}/`; probes `variants/{dp,ep,fp,enp,ena}/`;
logs `probe/logs/<route>/` and `probe/logs/probes2/`; timings `probe/timing1.txt`,
`probe/emit_rounds.txt`, `probe/timing2.txt`; all under
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/compiler-engineer/`.
