# Panel 177 — compiler-engineer

HEAD `521c5e02`, my directory only, the trunk untouched. Every number below is
from a command run in this directory and named in the evidence (E1-E13);
where a sentence is argued rather than run it says so. Legs: Darwin arm64 for
everything; Linux aarch64 (`heroes-linux-arm64`) and Linux x86_64
(`heroes-linux`) for the five reproducers under base, P and R (E4). Windows:
unrun, the box is off.

A fact the coordinator can use at once: **rebuilding the compiler from an
edited `selfhost/` takes 67-72 s here, not ~20 minutes**, when the seed is
built at `-O2` first (`clang -O2 -I runtime seed/heroes.c runtime/runtime.c`,
18.85 s) — the seed at no optimisation level is what makes it slow. Five
compilers were built this way for this report (P, R, M, M-must, B).

## verdict

**Question 1 — the life after it ends.** No veto: nothing I approve adds a
core construct (§1.7) and nothing I object to was measured to breach the
ceiling.

| route | verdict | the reason, measured |
|---|---|---|
| **P** — poison the binding at run time | **approve**, with four conditions below | catches all five reproducers, 134 before C, on all three legs (E3, E4), including both 077 shapes, because it asks the BINDING and not the address; moves 0 of 510 `check` verdicts and 2 of 138 `run` goldens, both only in their message (E9); +0.5 ns per handle-taking call (E11) |
| **M-must** — the route nobody listed: M with an intersection where paths meet | **approve**, as the compile-time half beside P | `check` 1 on all five reproducers, 0 false refusals on every shape run (E10), the same 2 of 510 verdicts moved as M (E9), checker only, no ABI, no emitted-C change, +1-2 % check time (E12). **Its price is not only lines**: it is the analysis family design.md §4.4 counts as deleted — *"that is dataflow analysis (the same family as move checking). With mandatory initialisation the problem does not exist"* (design.md:1077-1081) — so it enters only with that sentence amended in the same sitting, at its measured 327 code lines |
| **M** — may-moved, as briefed | **object** | refuses two correct programs whose correctness is a value (`if_correlated`, `match_correlated`, E7), and cannot follow the success clause panel 176 adopted: it refuses the correct failure path (E7's last row) |
| **R** — a liveness probe at every handle-taking call | **object** | **does not catch 088 on any leg**: `calloc` hands `c` the address `b` just lost, so the dead `b` is live again when R asks (E3, E4, `rac_eq`); refuses a correct shipped golden, `ffi-borrows-owes-nothing` (E9), and json-c's borrowed child (E5); +3.9 ns per call against P's +0.5, about eight times the added cost (E11). It is the only route that catches a copy made before the call (E13), and only while the address is not reused |
| **S** — a serial carried by the value | **object** (argued, unbuilt) | changes the C representation of every handle (ABI, and every emission golden holding one: 31 of the 510 files carry a `record X tag y` line, by `grep -lE`, an upper bound on the handle-declaring ones), at every FFI boundary site in about ten emitter modules, and a handle inside a group record is C's struct and cannot carry the number, so 077 survives there |
| **G** | refuse | the stale and the live handle are the same bits (the brief's reason; not re-run) |
| **A** | stays refused | Part 6's row falsifier is *"an alias rule … priced and compiled"*; nothing here refuses a copy, so the row is not reached (argued) |
| **D** | not alone | §1.12; it remains the sentence for the copy made before the call, which P and M-must both miss (E13) |

**P's four conditions**, each from a measurement:
1. **`==` needs a ruling** (E6): a poisoned binding no longer holds its
   address, so two dead handles become equal (false → true), a dead one differs
   from its own earlier copy (true → false), and `u1_static` prints `false`.
   Either the spec says so, or `==` on a dead handle aborts like every other read
   (my preference: one rule, *a dead value may not be read*, which is what M-must
   refuses at check anyway — it already refuses `u1_static`'s `a == b` line).
2. **An `@` in/out handle parameter that is not `acquires` must be asked too**;
   the prototype asks only non-`@` arguments (E6).
3. **The dead value should be a page of the runtime's own**, not one
   `max_align_t`, so a C write through a dead handle on an unchecked path lands
   in junk rather than in the runtime's neighbouring statics (argued).
4. **The set's end of a conditional life goes on the IR's success edge**, beside
   P's poison, rather than a second comparison in the emitter (found by building
   the clause, E8).

**Question 2 — the success clause.**

| form | verdict | why |
|---|---|---|
| **on the result, `-> i64 when 0`** (the ergonomist's β) | **approve** | built on P (E8): +133 / -13 code lines in 13 `selfhost/` files and +10 in `runtime/`; correct failure path 0, leaking one 134 at exit, success-then-read 134 before C, a `when` with nothing to end and a `when true` on an integer both `check` 1; two ends in one call are one `if` with two lines; a `void` result cannot carry it (refused, `success_value_type`) |
| **on the parameter, `val: Json transfers json_object_put on 0`** (α) | **object** (argued from β's build) | the same runtime and emission, stored per parameter, which admits two states that no library named in panels 176 and 177 exhibits (not searched further) — two ends of one call succeeding on different values, and `on` on a parameter of a function that returns nothing — and each is one more refusal to write; the `Param` record has five constructors to update exactly as `FunctionDecl` does |
| the clause's reach | **must cover `consumes`, not only `transfers`** | the spec's own example `sqlite3_close(db: Db consumes) -> i64` ends the life only on `SQLITE_OK`: *"sqlite3_close() will leave the database connection open and return [SQLITE_BUSY]"* (`MacOSX.sdk/usr/include/sqlite3.h:341-344`, SQLite 3.54.0, read here) |

## argument

R was briefed as catching 088, and does not: the reproducer's own `calloc`
reuses the dead address on all three legs, so the probe passes it, while R
refuses the shipped `borrows` golden. P asks the binding, not the address, so
reuse cannot fool it: five of five, three legs, 0 of 510 check verdicts moved,
half a nanosecond a call. M as briefed
refuses correct programs decided by a value and cannot follow the
clause; the same walk joined by intersection refuses only reads dead on every
path, and hands the rest to P. The clause belongs on the result: one field,
one comparison, fewer illegal states. Nothing closes the copy made before the
call; wart 20 stays.

## section

design.md §1.7 (core plus elaboration: P adds an IR literal and a runtime
entry and no surface construct; M-must is erased before the IR; S changes a
type's representation across the backend), §1.1 (the ceiling), Part 5 (the
core list and the two core passes), §1.12 (robustness: why D is not enough
alone, and why a false refusal of a correct program is a cost), Part 8 wart 20
and Part 6's borrow-checker row (line 2663) for the class none of the approved
routes closes. **§4.4 (design.md:1077-1081) is the section M stands against**:
mandatory initialisation *"deletes an entire analysis from the compiler … that
is dataflow analysis (the same family as move checking)"*. That sentence is
about initialisation and stays true of it; M-must is the family's second
member, priced here, and its landing owes §4.4 a sentence saying so. Beyond
that sentence and §4.8's *"no dataflow, no borrow checker"* (design.md:1342,
about `@` aliasing), design.md has no sentence about a check-time analysis of
handle states; and none about a success clause
(`grep -niE 'on success|only on success|success value|when it succeeds'` on
design.md: no hit).

## cost / delta

Counted as non-blank, non-comment lines of the unified diff against HEAD
(`grep '^+' | awk 'NF && $1 !~ /^#/'`), in my prototypes; a landing adds its
tests, goldens and the header comments this repository writes.

| route | `selfhost/` | `runtime/` | ABI | emitted C of one marked call | files |
|---|---|---|---|---|---|
| **P** | +96 −9 | +12 | 22 → 23 | non-consuming: `hero_handle_alive(t8);` before the call. Consuming: `hero_handle_alive(t2); hero_handle_consumed(t2); (void)cJSON_Delete(t2); t3 = HERO_HANDLE_DEAD; h0_b = t3;` | `ir.hero` + six match sites, new `ir/dead_handles.hero` (99 lines), `ir/flatten.hero` (3 call sites), `emit/handle_traffic.hero`, `emit/literal.hero`, `emit/decls.hero` (the stamp) |
| **R** | +60 −2 | +14 | 22 → 23 | `hero_handle_held(t8);` before a non-consuming call | `emit/handle_traffic.hero` only (a consumed-type walk it has to repeat, because `check/acquiring.hero:84`'s asks a `Checker` and the emitter holds a `Checked`) |
| **M** / **M-must** | +329 (new `check/moved.hero`, 410 lines / 327 code, + 4 in `checker.hero`) | 0 | none | unchanged | the checker's first flow analysis |
| **B** (β on P) | +133 −13 over P | +10 over P | (same move as P) | shown in E8 | 13 files |
| **S** | argued ~250-450 in ~10 emitter modules | argued ~40-60 (a serial per entry, a compare per call) | every handle's representation | every FFI argument unwrapped, every result and `@` cell wrapped, handle `==` / descriptors rewritten, `nullptr` of a handle respelled | `emit/ctype.hero` (511), `ffi_call` (156), `ffi_mutable` (174), `extern_field` (373), `extern_record` (274), `callback_guard` (162), `descriptors` (203), `operator` (341), `literal` (292), `handle_traffic` (143) — current line counts |

**An ABI move is not free in the goldens**: the stamp is printed into every
emitted unit (`emit/decls.hero:79`, `_Static_assert(HERO_RUNTIME_ABI == 22 …)`),
so P, R and S each re-bless every emission golden at their landing, as every
stamp move has (argued; the `emission` suite was not run against a prototype).
M-must moves no byte of emitted C.

**Where each lands against the ceiling.** `selfhost/` is 65 061 lines in 229
files today (`wc -l`), so Pascal-P4's ~4000 was passed long ago and the
question is the kind of machinery, not the total. P extends a mechanism that
exists (the handle traffic and the set, `emit/handle_traffic.hero` 143 lines,
`runtime/parts/alloc.c` 610) with one IR constant. M-must is a new KIND: the
checker's first dataflow. **Is *"the checker has no flow analysis"* still
true at HEAD?** Of `check/`, yes: the three headers that say it
(`check/consuming.hero:22`, `check/leasing.hero:29`, `check/acquiring.hero:25`)
are accurate, `check/lend_extent.hero` judges declarations and
`check/join.hero` joins types; `grep -rniE 'flow analysis|dataflow|data flow|liveness|definitely|worklist|use.before|uninitiali|unassigned'`
over `selfhost/` hits, in `check/`, only those three headers and
`check/freer.hero:12,24`, which say the same thing. **The compiler does carry
one**: `ir/values.hero:120` `dominators`, a textbook iterative fixpoint over
the CFG (~45 lines), in the IR verifier. M cannot reuse it, because `heroes
check` never lowers (`cli/check.hero`: parse, modules, resolve, checker, and
stop), so an IR-level M would make `check` say 0 where `build` says 1. What M
reuses is the SHAPE of the resolver's ordered walk (`resolve/walk.hero:56-195`,
about 140 lines), rebuilt with a state beside it. If M-must lands, those three
headers and Part 8 wart 20 become false in part and must be corrected in the
same commit.

**Needed for self-hosting: no** (Principle 0). The compiler's own source
declares no `consumes` and no `acquires` binding (`grep -rnE` over
`selfhost/` for an `extern` member carrying either: none). These routes enter
on §1.12, not on compiler need.

## prediction

1. **At the landing of this resolution, if it is P + M-must + the result-side
   clause**: `selfhost/` grows by **+500 to +700 non-blank, non-comment lines
   outside `test` blocks** (my three prototypes sum to 558), `runtime/` by **+20
   to +40**; `heroes check` over the 510 files changes verdict on **exactly
   two**, `run/abort-handle-given-back-twice.hero` and
   `run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero`, both 0
   → 1; and no program in `examples/` changes its `run` verdict (`corpus` stays
   55 / 0). Falsified by a third moved verdict, a corpus program that aborts, or
   a count outside the ranges.
2. **If R lands instead**, `run/ffi-borrows-owes-nothing` goes red at the
   landing, 134, and `read_after_consume.hero` still exits 0 on all three
   POSIX legs. Falsified by either being otherwise.

## condition

- **P → object** if a correct program in `examples/` or `tests/golden/` compares
  a handle with `==` after a `consumes` and needs the address answer, or if a
  landing cannot close the `@` in/out and callback paths (condition 2) without
  flow analysis.
- **M-must → object** on one correct shipped program it refuses (none of 510,
  and none of the seven shapes in `work/flow/`, is one today), or if the
  sitting keeps §4.4's *"deletes an entire analysis"* as a refusal of the
  family rather than a fact about initialisation; P then stands alone and
  every one of the five reproducers is still refused, at run time.
- **R → approve** only beside a record of borrowed handles whose END is
  declared and checked (an owner relation on `borrows`, which is a lifetime and
  Part 6's territory), priced and compiled; and even then not for 088, which is
  address reuse.
- **S → approve** only with a representation that carries the serial through a
  group record's field, which C's struct layout forbids today.
- **α over β** if a library is found whose two ends in one call succeed on
  different values; **both placements need an operator** if the first library
  a binding needs signals success by a range (`!= 0`, `> 0`) rather than one
  value — a question, unrun: no header was read here for it.

## evidence

The work files named below are all under this directory: `work/P/`, `work/R/`,
`work/M/`, `work/Mm/`, `work/B/` (each an edited `selfhost/` and `runtime/`
with its compiler), their diffs `work/*.diff`, the programs in `work/`,
`work/borrows/`, `work/eq/`, `work/flow/`, `work/success/`, `work/timing/`,
the Linux script `work/linux-legs.sh` and its outputs, the sweep lists
`work/check-*.txt`, `work/brief-*.txt`, `work/suite-*-*.txt`.

### Setup

HEAD 521c5e02, my directory only. Compiler built from the seed inside it
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 3.35 s) and an
`-O2` seed build for speed (`heroes-o2`, 18.85 s real). `heroes-o2 build
selfhost/main.hero --emit-c` reproduces `seed/heroes.c` byte for byte (cmp,
run) in 37.21 s real / 31.73 user / 4.43 sys.

### E1. Baseline, the five reproducers, Darwin arm64, seed-built compiler, -O0 build, three runs each (RUN)

| program | check | run x3 | stdout / stderr bytes |
|---|---|---|---|
| read_after_consume | 0 | 0 0 0 | 28 / 0 |
| use_after_consume | 0 | 134 134 134 | 0 / 396 |
| helper_consume_then_use | 0 | 134 134 134 | 0 / 396 |
| u1_static | 0 | 0 0 0 | 42 / 0 (`true`) |
| reuse_malloc | 0 | 0 0 0 | 66 / 0 (`same address: true`) |

Matches the shared brief's table (Darwin stderr is 396 B; the brief's 399 B is Linux).

### E2. Two prototypes built (RUN, Darwin arm64)

Both built by the `-O2` seed from edited copies of `selfhost/` and `runtime/`
(`work/P/`, `work/R/`), each compiler 67 s real to build. Diffs:
`work/P.selfhost.diff`, `work/P.runtime.diff`, `work/R.selfhost.diff`,
`work/R.runtime.diff`.

- **P**: an IR literal `dead_handle` (ir.hero + six exhaustive match sites), a
  lowering module `ir/dead_handles.hero` (99 lines) hooked after the three
  named-callee call sites in `ir/flatten.hero`, which stores the literal into
  the argument's place when the parameter is `consumes` and the argument is a
  local or a field path of one; `emit/handle_traffic.hero` emits
  `hero_handle_alive(x)` for every handle a non-`@` argument reaches; runtime
  `hero_handle_dead_cell` (a `max_align_t`), `HERO_HANDLE_DEAD`, and
  `hero_handle_alive` (a compare, no lock). ABI 22 -> 23.
- **R**: `emit/handle_traffic.hero` emits `hero_handle_held(x)` for every
  handle of a consumed type that a non-`@`, non-`consumes` argument reaches;
  runtime `hero_handle_held` (lock, one probe, abort if absent). ABI 22 -> 23.
  (First build named the entry `hero_handle_live`, which collides with the
  set's own counter `static size_t hero_handle_live` at `alloc.c:164`; renamed.)

### E3. The five reproducers under P and R (RUN, Darwin arm64, -O0, three of three)

| program | base check/run | P check/run | R check/run |
|---|---|---|---|
| read_after_consume (088) | 0 / 0 | 0 / **134** before C | 0 / **0** — not caught |
| use_after_consume | 0 / 134 | 0 / 134 (P's message) | 0 / 134 (the set's stray message) |
| helper_consume_then_use | 0 / 134 | 0 / 134 (P's message, through the `@` cell's copy-out) | 0 / 134 (stray) |
| u1_static (077) | 0 / 0 | 0 / **134**, prints `false` first | 0 / 0 |
| reuse_malloc (077) | 0 / 0 | 0 / **134**, prints `same address: false` first | 0 / 0 |

**R does not catch 088 on Darwin, and the reason is 077.** `rac_eq.hero` (the
088 program with its last line replaced by `print("same address: ", b == c)`)
prints `same address: true` three of three at -O0 and once at -O2: the
`calloc` for `c` returns the address `cJSON_Delete(item: b)` just freed, so
the dead `b` is live again when R asks, and R's check passes. The 088
reproducer is itself an instance of address reuse; a liveness check cannot tell
it from a correct call. (The brief's route list says R "catches the 088 shape";
on this leg that is false as measured.)

### E4. The same table on both Linux legs (RUN, `work/linux-legs.sh`)

Each leg builds the base compiler from `seed/heroes.c` and the P and R
compilers from their emitted C (`work/P/compiler-P.c`, `work/R/compiler-R.c`,
emitted on Darwin by the `-O2` seed from the edited `selfhost/`), then builds
every program at `-O0` and at `-O2` and runs it three times. Output:
`work/linux-arm64.txt` (`heroes-linux-arm64`, aarch64) and `work/linux-x86.txt`
(`heroes-linux`, x86_64 under emulation). **The two legs agree line for line
with each other and with Darwin**, for every program at both levels:

- base: 088 0; use 134 (399 B); helper 134 (399 B); u1 0 `true`; malloc 0 `same address: true`; `rac_eq` 0 `same address: true`.
- P: all six 134 before C, 181 B on stderr; u1 prints `false`, malloc prints `same address: false`, first.
- R: identical to base on all six. `rac_eq` prints `same address: true` on both Linux legs at both levels, so glibc reuses the freed `cJSON` block for the next `calloc` exactly as Darwin's allocator does.

The Linux `-O0` rows for `u1_static` and `reuse_malloc`, unrun in the shared brief, are run here: 0 0 0 on both legs under base.

### E5. R and a `borrows` handle (RUN, Darwin arm64, three of three; `work/borrows/`)

Two shapes, each a correct program:

- `next_stmt.hero` + `sq.h` — `sqlite3_next_stmt` in miniature: the database
  lends back statements, `stmt_next(d: Db, prev: Stmt) -> Stmt borrows`, and
  the loop reads each with the non-consuming `stmt_id`. base 0 `2 1`; P 0;
  **R 0** — every statement it lends is one the program prepared, so its
  address IS in the set.
- `borrowed_child.hero` + `jc.h` — json-c's `json_object_object_get` in
  miniature: `j_get(o: J) -> J borrows` hands back a child the parent owns,
  which nothing marked `acquires` ever began. base 0 `42`; P 0 `42`; **R 134
  three of three**, *"the set of live handles did not hold it"*. **R refuses a
  correct program.**

So the brief's *"R as stated refuses every correct call that hands on a
borrowed handle"* is half right as measured: it refuses a borrowed handle the
program never acquired (json-c's children, every `get0`), and admits one the
library merely lends back from the program's own (SQLite's statement list).

**What it would cost to make R correct there (argued, unbuilt).** Two ways,
and neither is correct:

- (a) **record a borrowed address too**: `hero_handle_borrowed(result)` after
  every `borrows` result or cell (about 3 emitter lines in `for_call`) and a
  second table in `alloc.c` (about 20 lines). But a borrowed life has no END
  event anywhere in the language, so the entry is never removed: R then admits
  the borrowed child after its parent's `j_put` has freed it, for the rest of
  the process, and admits any later handle that happens to reuse that address.
  It turns a false refusal into a silent use-after-free, which §1.12 ranks
  worse.
- (b) **declare whom it is borrowed from** — `-> J borrows o`, the life of the
  result bounded by the parameter's — with an owner edge in the set and a
  cascade when the owner is consumed: a new surface form (parse, AST, check,
  both printers), an edge table and a cascade in the runtime, a spec sentence;
  on the order of 120-150 lines by the sizes of E8's clause, which touches the
  same files. That is a lifetime annotation, the cost §4.10's table says the
  refused borrow checker avoids (*"lifetimes in the surface syntax, in the
  spec, and in every error message"*, design.md:1597), and it is still wrong
  whenever C frees the child through the parent by a call no mark relates to
  the child.

P has no such cost: it never asks the set, so a borrowed handle is exactly as
legal under P as today (E5: `borrowed_child` 0 under P).

### E6. P and `==` (RUN, Darwin arm64; `work/eq/dead_eq.hero`)

`a`, `b` two live nodes, `keep = a` a copy made BEFORE the call, then
`node_free(n: a)`, `node_free(n: b)`:

| line | base | P | R |
|---|---|---|---|
| `a == b` (two dead) | false | **true** | false |
| `a == nullptr` | false | false | false |
| `a == a` | true | true | true |
| `a == keep` (dead vs its earlier copy) | true | **false** | true |
| `[keep, a]`, `held[0] == held[1]` | true | **false** | true |

All three exit 0; check 0. And in the reproducers P turns `u1_static`'s
`print(a == b)` from `true` to `false` and `reuse_malloc`'s from
`same address: true` to `same address: false`, on all three legs (E3, E4).
So a poisoned binding **no longer holds its address**: spec § 13's *"`==`
compares the address"* stops being true of a dead handle, every dead handle
equals every other, and a dead one differs from the copy made before its
death. P needs one of two spec rulings: (i) *a handle whose life a
`consumes` ended equals no live handle and every other ended one*, or (ii)
`==` on a dead handle aborts like every other read of it. (ii) costs one
compare in the handle `==` the emitter writes inline and in the `_eq`
descriptor (`h_<mod>_<T>_eq` in the emitted C, `emit/descriptors.hero`),
unbuilt here.

What else reads a dead binding under P (argued from the prototype, and the
three I ran are marked): a copy made AFTER the death carries the dead value and
is caught when it reaches C (`held` above holds it; passing `held[1]` to C
aborts — argued); a return of it hands the dead value to the caller, caught at
C; `print`, a map key and `sort` cannot take a handle at all (spec § 13, § 11).
**What reaches C unchecked in the prototype**: an `@` in/out handle parameter
that is not `acquires` (the prototype asks only non-`@` arguments; the landing
must ask the cell too), and a handle a callback hands back to C. Where a dead
value does reach C, it points at `hero_handle_dead_cell`, one `max_align_t`,
so a C write through it corrupts the runtime's neighbouring statics; a landing
should make the cell a page of its own (argued, unbuilt).

### E7. M, built too (RUN, Darwin arm64) — because *cheapest* was the wrong axis to skip it on

`work/M/selfhost/check/moved.hero`, **410 lines, 327 of them code**
(non-blank, non-comment; `awk 'NF && $1 !~ /^#/'`), plus 4 lines in
`checker.hero`; zero lines in the IR, the emitter or the runtime; ABI
unchanged; the emitted C of every program unchanged. Diff: `work/M.selfhost.diff`.
What it is: an ordered walk over every body carrying the set of locals a
consuming call may have ended, copied at each `if`/`match` branch and joined
as a union, `&&`/`||` as a branch, `return`/`break`/`continue` ending a path
into the function's or loop's exit state, a loop walked twice (the second pass
from the entry joined with one turn's end — a union of gen/kill functions is
idempotent, so two passes are the fixpoint), a new declaration or a whole-name
`@` reviving a local, and a **program-wide summary** of which `@` parameters a
Heroes function may leave consumed, iterated to a fixpoint (≤ 8 rounds), so
`finish(@b)` ends `b` in its caller. One new diagnostic, `used_after_consumed`.

**The five reproducers under M: `check` 1 on all five** (`work/runM/*.chk`):
088 at 11:39, use at 11:68, helper at 14:68 *"`b` was handed to `finish`"*
(the summary), u1 at 10:11 and 11:16, malloc at 10:29 and 11:18 — M refuses
the `a == b` line as a read, before the second release.

**Over the 510 files (`work/check-M.txt`, `work/brief-M.txt` against
`work/brief-base.txt`): two verdicts move, 0 -> 1, and nothing else changes
in any file's diagnostics** — `run/abort-handle-given-back-twice.hero:29:16`
and `run/fixedbugs-a-real-deallocator-given-the-same-handle-twice.hero:52:18`,
both deliberate double releases through one binding. Both are right
refusals; both goldens would move from `run` to `check`. The compiler's own
source (`selfhost/main.hero`, 229 modules) checks clean under M.

**M over `if`, `while`, `match` (`work/flow/`, three runs each where it builds):**

| shape | correct? | base check/run | M check | P check/run |
|---|---|---|---|---|
| `while_renewed` — cell freed and renewed each turn | yes | 0 / 0 | 0 | 0 / 0 |
| `while_twice` — `=` binding freed inside the loop | no | 0 / 134 | **1** | 0 / 134 |
| `if_early_return` — free then `return` in one branch | yes | 0 / 0 | 0 | 0 / 0 |
| `for_break` — free then `break` | yes | 0 / 0 | 0 | 0 / 0 |
| `if_correlated` — free under `flag`, then use under `!done` | **yes** | 0 / 0 | **1 (two errors)** | 0 / 0 |
| `match_correlated` — free in one arm, use under the arm's `bool` | **yes** | 0 / 0 | **1 (two errors)** | 0 / 0 |
| `transfer_failure` — a `consumes` that C performs only on success, and the failure path frees the child | yes in C | 0 / 134 | **1** | 0 / 134 |

**Priced per construct** (code lines per function of `moved.hero`, by
`awk`): an `if` is `branches`, 14; a `while` and a `for` share `looped` and
`pass`, 9 + 11, plus 6 lines of `statement` for `break` and `continue`; a
`match` is `matched`, 18; `&&`/`||` 6 lines inside `expr`. The rest is the
walk itself (`statement` 33, `expr` 41, `call`/`method`/`arguments` 26), the
consumption and its summary (`consumed` 19, `ended` 11, `argument_at` 9,
`param_ends` 9, the driver 37), the state (`meet` 10, `is_moved` 5, `dead` 3,
`revive` 4, `written` 13), the records (26) and the diagnostic (10). **The
branches and loops are about 60 lines; the walk they need to stand in is
about 200.**

M refuses two correct programs whose correctness is a fact about VALUES
(a flag, an arm's result), which no path-insensitive walk can see; P runs
both. And the last row is Question 2's shape: once the success clause lands,
the run-time routes can end the life only when the result says so and run
the correct failure path; M cannot, because *"the transfer happened"* is
`rc == 0`, a value, and M would have to become path-sensitive over values
to follow it. **M and the success clause panel 176 adopted (its item 3)
cannot both be right about the failure path.**

### E8. The success clause, built at the RESULT placement (β, `-> i64 when 0`), on top of P (RUN, Darwin arm64)

`work/B/` = P plus the clause, on `consumes` (the landed word; `transfers` is
not in this tree, and at run time a transfer's end of obligation and a
release's are the same event in the set). Over P: **+133 code lines, -13 in
`selfhost/`, +10 in `runtime/`** (`work/B-over-P.selfhost.diff`,
`work/B-over-P.runtime.diff`), in 13 files: `ast.hero` (one field) and **five
`FunctionDecl` constructors** (`ast.hero`, `resolve/types.hero`,
`resolve/state.hero`, `resolve/qualified.hero` twice) which must each name the
new field; `parse/tails.hero` + `parse/members.hero` (`success_marker`, 22
lines); `print/dump.hero` + `print/fmt.hero` (CL-036's walk); a checker rule
`check/success.hero` (46 lines, two diagnostics: `success_without_end`,
`success_value_type`); `emit/handle_traffic.hero` (before the call the set is
asked and left alone, `hero_handle_held`; after it, `if (result == 0) {
hero_handle_consumed(x); }`); `ir/dead_handles.hero` + `ir/flatten.hero` (P's
poison moves onto the success edge: an `eq`, a `branch`, two blocks); runtime
`hero_handle_held`. Compiler build 71.62 s.

Emitted C of the one marked call, `_ = node_adopt(parent: parent, child: child, broken: 1)`
(`--emit-c`, `#line` lines removed):

```c
    hero_handle_alive(t3);
    hero_handle_alive(t4);
    hero_handle_held(t4);
    t6 = node_adopt(t3, t4, t5);
    if (t6 == 0) {
    hero_handle_consumed(t4);
    }
    t7 = INT64_C(0);
    t8 = t6 == t7;
    if (t8) goto bb1; else goto bb2;
bb1:
    t9 = HERO_HANDLE_DEAD;
    h1_child = t9;
    goto bb2;
```

| program (`work/success/`) | check | run x3 |
|---|---|---|
| `fail_path_correct` — add fails, the failure path frees the child | 0 | **0 0 0** |
| `fail_path_leaks` — add fails, nobody frees the child | 0 | **134 134 134** at exit, *"1 C handle(s) never given back"* |
| `success_path` — add succeeds | 0 | 0 0 0 |
| `success_then_read` — add succeeds, child read after | 0 | **134** before C (P's message) |
| `when_without_end` — `when` on a function that ends nothing | **1** `success_without_end` | — |
| `when_wrong_type` — `-> i64 when true` | **1** `success_value_type` | — |

Against today's compiler the first of these (`work/flow/transfer_failure.hero`,
the same program with no clause) is 134 on base and on P and `check` 1 on M
(E7): the correct failure path is refused by every route until the clause
exists, and the clause is what makes the run-time routes right about it.

Two more, same compiler (`work/success/two.h`):

- `two_transfers.hero` — `node_pair(parent: Node, a: Node consumes, b: Node
  consumes, broken: i64) -> i64 when 0`, add fails, both freed on the failure
  path: check 0, run **0 0 0**; the emitted C is one condition for both ends:
  `t8 = node_pair(t4, t5, t6, t7); if (t8 == 0) { hero_handle_consumed(t5);
  hero_handle_consumed(t6); }`.
- `void_when.hero` — `-> () when 0`: **check 1**, `success_value_type`. With
  no `->` at all the clause cannot be written, since `when` is read only after
  a result type (`parse/tails.hero`).

One thing the build found that neither placement's text says: the comparison is
written TWICE, once in the emitter for the set and once in the IR for the
poison, because handle traffic lives in the emitter (`handle_traffic.hero`)
and P's poison lives in the lowering. A landing should put the set's end on
the IR's success edge too, which is the panel-116 precedent
(`ir/owned_release.hero`'s header: *"a `free()` the dump cannot show is the
larger lie"*) applied to the set.

### E9. Which verdicts move over the 510 files (RUN, Darwin arm64)

`find tests/golden examples -name '*.hero'` reads **510** here too
(`work/files.txt`). `check` exit per file for each compiler
(`work/check-{base,P,R,M}.txt`), and `check --brief` text per file for base,
M and M-must (`work/brief-*.txt`); the `run` suite (138 programs, each built
and run and compared with its `.expected`) and the `corpus` suite (55
programs from `examples/`) through the net's own binary (`work/harness`, built
by the base compiler) naming each compiler (`work/suite-*-*.txt`).

| | `check` verdicts moved (of 510) | `run` suite | `corpus` |
|---|---|---|---|
| base | — (351 at 0, 159 at 1) | 138 / 0 | 55 / 0 |
| **P** | **0** | **136 / 2** | 55 / 0 |
| **R** | **0** | **136 / 2** | 55 / 0 |
| **M** (may-moved) | **2** (0 -> 1), no other diagnostic changes | not run (check-only) | — |
| **M-must** | **2**, the same two | — | — |

Why each moved:

- **P, `run`**: `abort-handle-given-back-twice` and
  `fixedbugs-a-real-deallocator-given-the-same-handle-twice` still abort 134
  before C, but with P's sentence instead of the set's stray sentence, which is
  what the golden asserts. Both are double releases through one binding. Same
  verdict class, different message: the goldens' asserted substring moves.
- **R, `run`**: `ffi-borrows-owes-nothing` — **a correct program, the shipped
  golden for `borrows`**, which prints `103` and says *"the borrowed one was not
  given back"* — aborts 134 under R, because `slot_peek() -> Slot borrows`
  hands back a handle nothing acquired and R asks the set about it at
  `slot_value(s: theirs)`. And `abort-handle-borrows-that-gives-away` aborts at
  that same kind of call, before printing the `7` it is expected to print. **R
  refuses a correct program in the repository's own suite**, which E5 predicted
  from a miniature.
- **M and M-must, `check`**: the same two double-release goldens become compile
  errors (`:29:16`, `:52:18`); right refusals, and both files would move from
  `run/` to `check/`.

### E10. The route nobody listed: M-must + P (RUN, Darwin arm64)

`work/Mm/` is M with one function changed: where paths meet, a local is ended
only if it is ended on EVERY path (intersection instead of union; 9 lines of
`meet`). Same walk, same summary, same diagnostic, same line count to within
four.

| | M (may) | **M-must** | P at run |
|---|---|---|---|
| the five reproducers | check 1, all five | **check 1, all five** | 134 before C, all five |
| `while_twice` (wrong) | check 1 | check 0 | 134 |
| `if_correlated` (right) | **check 1 — false refusal** | **check 0** | 0 |
| `match_correlated` (right) | **check 1 — false refusal** | **check 0** | 0 |
| `if_early_return`, `for_break`, `while_renewed` (right) | 0 | 0 | 0 |
| 510 files | 2 moved | **the same 2 moved** | 0 moved |

M-must refuses only a read that is dead on every path — a fact about the
program's SHAPE, never about a value — so it has no false refusal on any
shape run here, and everything it leaves (a loop's second turn, a branch)
reaches P's check before C. With the success clause, a call carrying `when`
is not a definite end, so M-must would treat it as no move (one condition in
`consumed`; argued, not built) and P poisons on the success edge (built, E8):
the two halves then never disagree with the clause. This is the combination
my verdict below rests on.

### E11. R's price per call, and P's (RUN, Darwin arm64, `-O2`; `work/timing/`)

`loop_<N>.hero`: one handle acquired, `N` calls of a non-consuming
`node_peek(n: Node) -> i64` in a `while`, one release. First build had
`node_peek` read-only, and clang hoisted the base loop's call out of the loop
(100M calls in 0.02 s), which would have charged R and P for an optimisation
their opaque runtime call merely prevents; rebuilt with a `volatile` write in
`node_peek` so the call stays in every variant (`loop.h`). Six interleaved
rounds of the three binaries, `/usr/bin/time -p` (`times2.txt`). **Round 1
discarded**: `real` 0.43/0.48/0.82 against `user` 0.11/0.16/0.50, the first
run of a new binary waiting on the system. Rounds 2-6, `real` = `user` + `sys`
to the hundredth (nothing waiting; the machine was not idle — another user
application held ~170 % CPU on other cores — and none of my processes ran):

| N | base | P | R |
|---|---|---|---|
| 1 000 000 | 0.00 s ×5 | 0.00 s ×5 | 0.00 s ×5 (below the tool's 10 ms resolution) |
| 100 000 000 | 0.08 s ×5 | 0.13-0.14 s | 0.47-0.48 s |

So, **per call: base ≈ 0.8 ns, P ≈ +0.5 ns (one compare, no lock), R ≈ +3.9
ns (one uncontended `pthread_mutex` lock and unlock and one probe of a set
holding one address)** — 1M calls cost R about 4 ms (the 100M figure divided
by 100; the 1M runs are below resolution, so that is arithmetic on a
measurement, not a measurement). R's probe cost grows with the set's
occupancy and its lock with contention; neither was measured.

### E12. M's price at check time (RUN, Darwin arm64)

`heroes check selfhost/main.hero` (229 modules, 65 061 lines by `wc -l`),
three interleaved rounds: base 5.11 / 5.07 / 5.04 s real; M 5.11 / 5.20 /
5.26; M-must 5.08 / 5.16 / 5.11 — about +0.1 s user, ~1-2 %, for three walks
of every body (two summary rounds and the verdict pass).

### E13. The copy made before the call — the class itself (RUN, Darwin arm64; `work/flow/copy_before.hero`)

`a = node_new()`, `keep = a`, `node_free(n: a)`, `print(node_peek(n: keep))`:

| | check | run x3 | `--sanitize` |
|---|---|---|---|
| base | 0 | 0 0 0 (prints a freed word) | 134 `heap-use-after-free` |
| P | 0 | 0 0 0 | 134 `heap-use-after-free` |
| M-must | 0 | 0 0 0 | 134 `heap-use-after-free` |
| R | 0 | **134 134 134** | 134 (R's abort) |

P and M poison or track the BINDING, so neither sees `keep`. R asks the set
about the address and catches it — here, because nothing was allocated
between the free and the read. This is wart 20's class exactly as design.md
states it, and it is what no route but R (reuse-limited) and S
(field-limited) reaches.
