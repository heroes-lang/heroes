*Written out by the coordinator from the seat's final message on 2026-09-28 at
06:15, as the brief asked: the harness refuses a subagent's report file. The
text below is the critic's, unchanged.*

# Panel 182, completeness critic (soundness lane, defect 114)

I give no verdict. I worked in `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/critic/`, made with `git archive 0fc98107`, with `build/` removed. My compiler was built from the seed with CLAUDE.md § Commands' first line. Its sha256 is `b08c7c35…`, the same as both seats' binaries, and its own `--emit-c` is byte-identical to `seed/heroes.c` (`cmp`). I ran on Apple clang 21.0.0, arm64, 2026-09-28 between 05:20 and 06:10, with other processes on the machine (load 1.75 to 7.94). For that reason I make no timing claim of my own. I built and ran nothing in the trunk or in a seat's directory: I copied `variants/{amin,d,f}` into `work/v/` and built there. My instruments are under `work/`.

## 0. The brief's counts, re-derived by another method

I cut `h_keywords_keyword` out with `awk` (from its definition line to the first line that is exactly `}`), not by matching braces.

- **Confirmed:** 941 lines, 139 `= {0}`, 21 `hero_str_eq(`, and 23 distinct `h<N>_own<N>` names.
- **Off by one:** I count **181** declaration lines before `goto bb0`, not 182. They are lines 3 to 183 of the function: 139 zeroed and 42 plain.
- **Seed totals confirmed:** 102,990 and 29,366, both counted by occurrence and by line.
- **10,902 is not the number of owned slots.** It is what the brief's pattern `<type> h<N>_own<N>` matches, and that pattern has no room for a pointer type. 1,995 owned slots are `HeroArrayHeader *` (1,876) or `HeroMapHeader *` (119). The true count is **12,897**, which is the compiler-engineer's number (`work/census.py`).
- The brief was measured at `dfcac362` and the seats copied `0fc98107`. `git diff --stat dfcac362 0fc98107` touches only `docs/`, so the seed is the same file.
- Every line citation in `00-shared.md` holds: `body.hero` 333 lines and lines 156-157 and 204-205; `unread.hero` 95, 124, 232; `own.hero` 354; `layout.hero:153`; `HERO_RUNTIME_ABI 26` at `heroes_runtime.h:37`.

## 1. The dead-zeroing count, re-run by an independent instrument

**The instrument.** `work/defassign.py` is a definite-assignment check over the emitted C text. It builds a control-flow graph from `goto`, `if … goto`, `switch` and `return`. It counts a read through `&` as a read, which clang does not, and a partial write as a read. It ignores the prologue's zeroing, since that is the question.

**Negative controls, both flagged:** `release(&t152)` moved above `t152`'s write (reported as needed, through the address), and a by-value read of `t3` above its write.

**On the whole seed** (2,166 functions; the 2,167th has no tracked locals):

- values: 81,680 zeroed, **0** reached unwritten;
- synthetic slots: 12,897 of 12,897 needed;
- named slots: 7,780 needed and **633 dead**. All 633 are `@` parameters, each with its `h = *p;` copy-in (checked by script).

These are the compiler-engineer's numbers, class for class.

**Clang negative control on the trunk.** Stripping every `= {0}` and removing `#line` gives 20,928 warnings, deduplicated to **20,677** (function, variable) pairs: 7,780 named and 12,897 synthetic, 0 values. The seat's 20,928 is confirmed.

**Beyond the seed.** Of 332 programs (every `run`, `emit`, `ir` and `fixedbugs` case, plus every example `main.hero`), 300 emit. The other 32 (30 `fixedbugs`, 2 `ir`) fail, as they do for the seat. Across 1,491 functions, the 21,224 zeroed values are all dead.

## 2. Coordinator's question 1: a value whose read or release the IR does not show

**What I searched:**
- every `mangle.value(` call in `selfhost/emit/` (14 files);
- the 21 callback thunks in the corpus: none declares a zeroed local, since each initialises at its declaration;
- `callback_guard.entry`, which is printed before the prologue and so cannot name a temporary;
- the `@` out-cell: a hidden slot stored `nullptr` by an IR store, a slot and not a value;
- the panic path, whose reads are `abort_op` operands;
- the exit sweep and `@` copy-out, which touch slots only.

**No read or release of a value outside the IR's operands was found**, by the C-level analysis over 301 programs (the seed plus 300).

**What I found instead is a write that the IR shows as whole and the C performs in parts.** The map lookup (`selfhost/emit/container.hero:265-296`, seed around line 70796) writes, on the found path:

```c
t3.tag = INT64_C(0);
(&hero_desc_str)->copy(&t3.as.ok, found);
```

The inactive union member and the padding keep whatever the prologue left.
- There are **132 such sites in the seed** and **80 in the corpus**.
- Under (a-min) those bytes become indeterminate: for `int?`, 24 of the union's 32 bytes.
- No instrument sees it. The verifier sees one definition. Clang does not warn about a partly written aggregate. ASan does not track uninitialised memory. My own first analyser also missed it: it read the lookup's intra-instruction `if/else` as straight-line code. I found the shape by grepping for partial writes.
- Nothing reads those bytes, by reading the code: the generated release, eq and hash all branch on the tag first (`h_0opt_f87774a_release`, seed line 972447).
- MemorySanitizer is also silent on it, on the paths the runs below execute (§ 6).

(a-min)'s own comment in `body.hero` says *"its one definition precedes every read on every path, which the verifier proves"*. At these 132 sites the verifier proves that of an IR definition the C does not carry out whole. A one-line repair would make the definition whole before the payload copy: `t = (opt){.tag = 0};` then the copy. This is unrun.

**(a-min) returns fewer values to clang's check than the seat says.** Of the 81,680 zeroed values, **10,078** are read only through an address, and 18,038 have their address taken somewhere (textual count). So (a-min) returns about **71,602** values to clang's `-Werror=uninitialized`, not 81,680. The other 10,078 are guarded by the verifier's dominance and the new in-block order check alone.

## 3. Question 2: route (f)'s initialising stores

**By construction.** In `variants/f/ir/own.hero`, the "initialising" branch emits the store and at most an incref. A wrong yes can drop a release, never add one.

**Fault injection** (`work/v/fx`): every whole-slot store is called an initialisation, and no block is called cyclic.

- **`run`: 190 passed, 20 failed.** All 20 fail by `panic: N heap blocks still live at exit`, from 1 to 180,000 blocks, all at `-O0`. For one case (`adversarial-str-self-assign`, 98 blocks) I checked by hand that the gate fires in all three legs: `-O0`, `-O2` and `--sanitize`.
- **Own tests: 789 run, 494 failed**, every one by the leak gate.
- No crash, no sanitiser report, no wrong output anywhere.

So on this corpus, a wrong yes is a leak.

**The leak counter is not on in every leg that would see it.**

- **The compiler itself never reaches the gate.** Every path out of `selfhost/main.hero`'s `main` ends in `exit(code:)`, and `hero_exit` bypasses the gate by design (`runtime/hero_os.h:105`). I measured it with a 12-line destructor probe (`work/probe_live.c`): the trunk compiler exits 0 with 17 live blocks after `check selfhost/keywords.hero`.
- **The fault-injected compiler reaches its fixpoint undisturbed.** It compiled itself to a byte-identical fixpoint (`cmp`), at exit 0, with no diagnostic. Its peak RSS was **3,109,797,888** bytes, against 270,827,520 for the stage-1 compiler emitting the same file. On `check selfhost/main.hero` it gave the same stdout at exit 0, with **1,334,909** live blocks at exit against the trunk's 827.
- **What that means for the other legs.** The fixpoint, `emission`, `determinism`, `ir`, `emit` and `canonical` cannot see this class. The compiler's 12,897 synthetic slots are covered only through its unit tests.
- **In `run`, the gate is crossed by about 153 of 210 goldens.** 56 end in an expected panic that is not the gate, and `exit-status` calls `exit`. This comes from counting `.expected` endings, so it is an inference about which path each takes. Under the maximal fault, only 20 of them failed: `run` is a thin instrument for this class.
- **A cheap differential probe works today.** Trunk against (f) on five commands, live blocks at exit: 17/16, 827/826, 13/12, 828/827, 830/829. So (f) leaks nothing more on those paths. The probe is one-sided, because the frames alive at `exit()` differ between the two.

**(f) takes 3,914 slots out of clang's sight, and nobody priced it.**

| seed | fixpoint | slot zeroings needed | seen by clang if stripped | reached unwritten only through `&` |
|---|---|---:|---:|---:|
| trunk | yes | 20,677 | 20,677 | 0 |
| (d) | own seed, 17,071 zeroings, 905,835 lines (both match the seat) | 17,071 | 17,071 | 0 |
| (f) | reproduced (`cmp`), 17,116 zeroings, 841,223 lines (both match the seat) | 12,461 | 8,547 (3,404 named, 5,143 synthetic) | **3,914** |

In (f), 2,781 of those 3,914 are option releases (`h_0opt_*_release(&slot)`), and 4,655 of its zeroings are dead. My analyser and clang agree on (f) pair for pair. So under (f), the unconditional slot zeroing is the only guard on 3,914 slots, where the trunk and (d) have clang behind it. By the seat's own table, what (f) adds over (d) is 2 to 5% (`fmt -O0` 2.23 to 2.165, `check` 3.02 to 2.95, `--emit-c` 29.6 to 28.0).

**The (b) half of (f) could not be fault-injected.** I tried calling a temporary used twice "consumed" (`work/v/fy`). It changed **0 of 264** programs and the compiler's own emission (`cmp`), so its `run` 210/210 measures nothing. (b)'s failure mode, use-after-free or double release, rests on the seat's reasoning alone.

## 4. Question 3: the unit cache key (`selfhost/cli/units.hero:82-84`), run rather than inferred

**Clang changed, flags the same.** In a warm build directory, switching `PATH` from Apple clang 21 to Homebrew clang 22.1.8 (both on this Mac) **rebuilt no object**. The runtime, library and program objects kept their mtimes, and the new binary printed `clang major: 21` from a header macro. A cold directory under clang 22 printed `22`.

**Flag list changed, VERSION the same.** I built compiler B from my tree with one extra flag (`-DCRITIC_FLAG_B=1`); both report `heroes 0.2.0`. On the warm directory B reused every object and printed `flag B seen: 1`; on a cold directory it printed `2`. The runtime object's key (`toolchain.hero:154`) has the same hole, and its comment's *"because the key covers everything"* is false by this run.

**The three `run` legs are keyed apart:** measured 6 unit directories and 3 runtime objects.

**Normal use: yes.**
- `clang_floor.hero:75` already writes `clang --version` into `build/clang-version.txt` on every build, and the key does not use it.
- The trunk keeps a warm `build/`: 319 unit directories and 6 runtime objects, the oldest entry from 01:06 today.
- CI caches nothing (no cache step in `ci.yml`), so CI is not exposed.
- In the history, VERSION has been 0.2.0 since `b21c5b0d` (2026-09-07). `3379df9d` (2026-09-18, `-fsigned-char`) changed only `flags.hero` and the seed. The runtime next changed the same day (`b7c47aab`), so a warm tree between the two served pre-flag objects. On Darwin the flag agrees with the platform default, so those objects would have behaved the same (an inference, unrun).

**Why it matters for defect 114.** Any pattern or MSan leg proposed as an instrument would test unpatterned objects in a warm tree unless the flags are in the key.

## 5. Contradictions between the seats, each checked

- **What `-O2` does to the zeroing.** The engineer says the optimiser removes the zeroing calls; the ffi-pragmatist says 86 vector zero stores remain. Both are true of what each counted. Disassembled at `-O2`, the trunk's `h_keywords_keyword` has 0 `memset` calls and **243 zero-store instructions before its first call**, with a 4,272-byte stack adjustment. (a-min)'s has 66 zero stores and 2,256 bytes. At `-O2` the zeroing is inlined, not removed.
- **"273 slots in the seed are touched only [through the address]" (ffi-pragmatist).** I measured **0** such slots in the trunk seed, and clang flags 20,677 of 20,677. The figure 273 appears in no output in the seat's `work/`. It is true of (f)-shaped emission (3,914), not of the trunk.
- **"Values are safe: the verifier already proves them" (ffi-pragmatist).** This is false for the trunk. `ir/values.hero:84-115` compares blocks only, which is the engineer's finding.
- **The ffi-pragmatist's single hang.** I ran `c-frees-a-lease-through-a-callback` 400 times: trunk and the real (a-min) emission, plain and `--sanitize`, 100 runs each. **0 hangs.** Beside the question: the plain builds exit 133 in 81 and 79 runs of 100 and 134 in the rest, with identical output, in the trunk and (a-min) alike. That is a pre-existing nondeterministic exit status, not pursued.

## 6. Routes and instruments nobody listed

**MemorySanitizer on the Linux leg: measured working.** I used the `heroes-linux-arm64` image already on this machine (Debian clang 22.1.8).
- It catches the address-only uninitialised release at `-O0`, `-O1` and `-O2`, with or without inlining.
- I built the whole compiler under MSan at `-O0 -g0`; at `-O1 -g` the 7.8 GB container killed clang. The trunk, (a-min) and (f) compilers give **0 reports** on `check` (twice), `fmt` and `--dump-ir`. A control that zeroes nothing reports at its first command, in `h_main_main`, at exit 1.
- On **206 of 210** `run` goldens, trunk and (a-min) emission give identical exit code, report count and stdout. One golden (`fixedbugs-c-aborts-with-no-lease-live-and-the-runtime-says-it-did-not`) draws one report under both, so it is not caused by the route. Four do not build in the container under either.

**Other routes nobody listed:**
- A standing `-ftrivial-auto-var-init=pattern` leg, which needs § 4's key repair first.
- The live-at-exit differential probe, as the compiler's missing leak gate.
- **Coalescing the synthetic slots of mutually exclusive arms**: one slot per type instead of one per arm, keeping rules 3 and 5 and adding no IR op. Its premise, that no path defines two of them, loops included, is **unrun**.

**`body.hero:5`'s premise is C++'s rule, not C's.** It says *"goto may not jump over an initialisation"*. C accepts it: `work/gotoinit.c` compiles under the project's 16 flags plus `-Wextra -pedantic`, exits 0 and prints 42, while `clang++` refuses it. So declaring a temporary at its definition is a route. It has its own premise (block text order against dominance) and, by inference, no run-time gain over (a-min).

## 7. Asserted and not measured

**Now measured by me.** (a-min) had not been run on `warnings`, `lines` or `corpus`, which is the `tests/golden/run/**` and `examples/**` rows of `.claude/rules/verification.md`. I ran them with the (a-min) stage-1 compiler: `warnings` 271/0, `lines` 211/0, `corpus` 55/0, `units` 3/0, `cache` 6/0. My (a-min) self-emission equals the seat's except for the source path in `#line`.

**Still unrun:**
- `layout`, `order` and `records` for any variant. (f) takes `ir/own.hero` from 354 to 430 lines against module-shape's roughly 300.
- Unit tests for (b) and (f), which the seat itself notes.
- (c): the engineer did not build it. The ffi-pragmatist measured its failure in a loop (heap-use-after-free on the fourth iteration).
- The Heroes-runtime reproduction of a stale live pointer released through `&`.
- Windows, and Linux x86-64 (I ran arm64 only).
- Every timing, by me.
- How many examples cross the leak gate: 11 of 54 example directories call `exit(` somewhere, and I did not measure which runs take that path.

## 8. Framing facts in the briefs

- **The owned-slot count:** 10,902 undercounts them; there are 12,897 (§ 0).
- **The declaration count:** 182 should be 181 (§ 0).
- **The verifier:** 00-shared says it checks that a value's definition dominates its uses. It does so between blocks only; within a block it checks nothing.
- **ASan as the instrument:** 00-shared names ASan in the `run` suite as an instrument that would catch an uninitialised read. It is blind to that class (the engineer measured it). MSan is the one that sees it, and only on Linux (§ 6).
- **Route (b) as briefed does not exist as a separate route.** The lowering already writes every arm into `h2_r0`; the per-arm slots are rule 5's synthetic slots. The seat's (b) is a different mechanism, the consumed store.
- **design.md Part 5 already states rule 5 in its refuted form** (lines 2615-2620): *"decrefed at the end of its defining block, made safe by a checked invariant"*. That is the draft `own.hero:29-31` records as refuted within the hour. (b) and (f) change rule 5 again, so they owe a design.md amendment to a sentence that is already false today.
- **The engineer's suite list omitted `warnings`, `lines` and `corpus`**, although every route changes the C of every program in those rows.

## 9. The lane, and the questions the sitting did not ask

**Reader-facing halves:**
1. The ffi-pragmatist found that a compiler release bug in a program with an `extern` panics naming the program's C marks first. That is a message's attribution. 00-shared says the sitting widens when a seat finds a reader-facing half, and the synthesis should say whether it did.
2. design.md Part 5's text, in a public repository.
3. Under (a-min), a lowering bug reaches the author as `internal error:` followed by clang's words (`selfhost/cli/produce.hero:223-234`), where today it is a silent zero. Nobody showed what the author reads.
4. `--emit-c` and lldb show uninitialised temporaries rather than zeros (unmeasured).

**Questions not asked:**
- Should the compiler itself cross a leak gate? It is the one 12,897-slot program no gate sees, and (b) and (f) grow exactly the class that gate is the only instrument for.
- Is (f)'s initialising-store half worth losing clang's sight of 3,914 slots for 2 to 5%?
- Should the cache key be filed as its own defect? It is not defect 114, and it is the precondition for any new instrument leg.

**Instruments:** `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/critic/work/`: `defassign.py` and `defassign2.py` (analysers), `clangpairs.py`, `census.py`, `probe_live.c`, `v/fx` and `v/fy` (the injections, with `fx-run.out`, `fx-owntests.out`, `fx-self.log`), `q3/` (cache probe), `msan/` (with `runs.out`), `kw-O2.s` and `kw-amin-O2.s`, `gotoinit.c`, `hang/`. No process or container of mine is still running.


---

*The critic's second final message, 2026-09-28 at 06:40, after the coordinator's message asking it to start nothing between 06:12 and 06:17 resumed it: written out verbatim beneath the first, which stands above as it was.*

# Panel 182, completeness critic (soundness lane, defect 114)

The report file was not written: the harness refuses a subagent's report file, so this message is the report. The coordinator writes it to `docs/panel/182-reports/completeness-critic.md`. I give no verdict.

**Setup.** Tree: `git archive 0fc98107` into `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/critic/`, then `rm -rf build`. The compiler was built from `seed/heroes.c` with CLAUDE.md § Commands' first line. It is byte-identical to both seats' binaries (sha256 `b08c7c35…`), and its own `--emit-c` of `selfhost/main.hero` is byte-identical to the seed (`cmp`).

**Method.** Every number below comes from a command I ran on 2026-09-28 between 05:20 and 06:40; an inference is labelled as one. I copied the compiler-engineer's `variants/{f,amin,d,e}` into `work/v/` and rebuilt each from my own compiler. I built and ran nothing in the trunk or in either seat's directory; I only read them and made copies. I timed nothing: the machine was shared throughout (load 2 to 8). As the coordinator asked, I started no build, suite or benchmark between 06:12 and 06:18.

## The three questions

### (1) Is there a value read or released that the IR does not show as an operand?

**None found, over the whole seed and 300 programs, with three instruments.**

- **An independent C-level definite-assignment analyser** (`work/defassign.py`). It builds a CFG from the emitted labels, gotos, switches and returns. It counts every textual use as a read, **including a read through `&`**, which is exactly where clang is blind. It counts `x.f = …` as a read, not a write.
  - Negative controls, both flagged: an inserted `release(&t152)` above `t152`'s assignment in `h_keywords_keyword` (as `addr`), and an inserted `t2 = t3;` above `t3`'s write (as `value`).
  - **Seed:** 2,166 functions carrying all 102,990 zeroings. Values: **0 of 81,680 needed**. Synthetic slots: 12,897 of 12,897 needed. Named slots: 7,780 needed; the 633 dead ones are all `@` parameters with a copy-in line (633 of 633).
  - **Corpus:** 300 of 332 programs emit (the other 32 exit 1: 30 `fixedbugs` refused by the checker, 2 `ir` goldens whose headers do not exist). 1,491 functions. Values: **0 of 21,224 needed**. The 85 dead named slots are all `@` copy-ins.
  - It agrees class for class with the compiler-engineer's clang census, which I also re-ran: 20,928 warnings, 20,677 (function, variable) pairs, 0 values.
- **MemorySanitizer, which does exist on this Mac.** It is refused for the Darwin target, but the project's own `heroes-linux-arm64` image (Debian clang 22.1.8) has it.
  - Negative controls fire. A struct uninitialised on one path and released only through `&` is reported at `-O0`, `-O1` and `-O2`, noinline or inlinable. The trunk seed with every `= {0}` stripped, built under MSan, reports at its first use (`h_main_main`) on every command and exits 1.
  - The trunk, (a-min), (e) and (f) compilers under MSan give **0 reports** on `check selfhost/keywords.hero`, `check selfhost/main.hero`, `fmt selfhost/check/walk.hero` and `build selfhost/main.hero --dump-ir`.
  - All 210 `run` goldens, built from the trunk's, (a-min)'s and (e)'s real emission under MSan: 4 do not build with my flag line (the same 4 in each), and **every one of the other 206 gives the same exit, report count and output across the three**.
  - The one report common to all three is in the runtime's crash-blame stack walk (`runtime/parts/stack.c:372`, reached from `hero_lease_crash` in a signal handler). It is not related to zeroing.
  - Cost: the MSan build of the seed was OOM-killed at `-O1 -g` in the 7.8 GB container, and built at `-O0 -g0`.
- **The shapes the coordinator named, one by one:**
  - **Thunks.** 21 thunks in the corpus; their only local is `hero_result`, initialised at its declaration.
  - **Out-cells.** The `@x: cstr owned free` cell is an IR store of `nullptr`, not a prologue zeroing.
  - **Panic paths.** `hero_panic*` is noreturn and runs no sweep.
  - **The exit sweep** reads slots only (`decref_slot`), never a value.
  - **`link`.** There is no `link` parameter: `link` is a group's library name (spec line 419). The FFI brief's "a `link` parameter" names a shape that does not exist.

**One C-level shape the IR does not show as separate instructions: a write, not a read.** `emit/container.hero:265-296` (`map_get`) builds a lookup's `V?` in two branches, with `tN.tag = 0;` and `(desc)->copy(&tN.as.ok, found);` on the found branch. That is 132 sites in the seed and 80 in the corpus. Once values are unzeroed, which (a-min) does, the union's inactive bytes on that branch are indeterminate.

Every consumer I found reads the tag first: the generated `_release`, `_retain`, `_eq` and `_hash` (seed around line 972447). MSan saw no use of those bytes. So it is harmless as far as measured. It would stop being harmless if anything ever compared, hashed or wrote a whole option value byte-wise.

### (2) Can a wrong "initialising" answer in (f) release something or corrupt, rather than leak?

**Fault injection on (f)'s initialising half** (`work/v/fx`): every whole-slot store is called an initialisation, and no block is ever in a cycle.
- It compiles itself to a fixpoint.
- `run`: **190 passed, 20 failed**. All 20 fail on the leak gate (`N heap blocks still live at exit`), each reported at its `-O0` leg. There was no crash and no sanitizer report.
- The compiler's own tests: **494 of 789 failed, every one on the leak gate.**
- On one injected case (98 blocks) I confirmed the gate fires in all three legs: `-O0`, `-O2` and `--sanitize`.
- I found no shape where a wrong yes releases anything: an initialising store emits no load and no decref at all.

**Where the leak counter is off, measured:**
- **The compiler binary itself.** Every path out of `selfhost/main.hero`'s `main` ends in `exit(code:)`, which bypasses the gate (`runtime/hero_os.h:105`).
  - The injected compiler exits 0, with stdout identical to the trunk's on `check` and `fmt`, and its fixpoint holds.
  - I measured live blocks at exit with a 13-line destructor probe (`work/probe_live.c`, which reads `hero_runtime_live()`): **1,334,909 against the trunk's 827** on `check selfhost/main.hero`, 9,458 against 17 on `keywords.hero`, 164,151 against 13 on `fmt`. Max RSS went from 100 MB to 512 MB.
  - So a wrong answer that bites only compiler-shaped code passes the fixpoint and every suite that merely drives the compiler. Only the compiler's own tests see it.
- **Goldens that never reach the gate**, counted with grep and not run: 60 `run` goldens end in `!panic:` and 4 in `!exit:`. 4 of the 60 panics are the gate's own later checks, so the heap-block count is read at exit in about 150 of 210.
- **Static literals** (`HERO_STR_LIT`) are not heap blocks, so leaking one is invisible, and harmless.

**The consumed-store half of (f) is not a leak-only analysis.** (f) also carries (b)'s `consumed_by_store`, whose wrong answer is a use-after-free: the compiler-engineer's own (b) bullet says so. So "the only new analysis decides initialising stores" is not accurate for (f).

I tried to provoke that half twice, by consuming a temporary with up to 2 uses (`fy`) and with any number of uses (`fz`). **Both injections were vacuous**: byte-identical emission on 264 programs and on the compiler, and `run` 210/210. The single-use guard never decides anything on this tree. Its safety rests on the lowering never reading a stored temporary again, and no run can falsify a guard nothing exercises. A landing owes a unit test that makes it fire.

**(f) moves slots into clang's blind spot.** In (f)'s own seed (17,116 zeroings, fixpoint re-derived: 841,223 lines), **3,914 zeroed slots are read before they are written only through `&`** (1,965 named, 1,949 synthetic). Clang's stripped-seed control on (f) flags 8,547 pairs, exactly my by-value classes (3,404 + 5,143). The trunk and (d) have **0** such slots: clang sees all 20,677 and all 17,071.

The zeroing still stands in (f), so nothing is wrong today. But the next change that drops one of those zeroings has no build-time instrument, which is the very ground the compiler-engineer gives for refusing (e). The seats did not state this difference between (d) and (f).

### (3) Is a stale object built with other flags ever reused in normal use?

**Yes; I ran it** (`work/q3/`). The probe program binds `CRITIC_WHICH` (1, or 2 under `-DCRITIC_FLAG_B`) and `__clang_major__` from a local header.

- **Other flags.** Compiler B is the same VERSION `0.2.0` with `-DCRITIC_FLAG_B=1` added to `flags.hero`. Run on a build directory the trunk compiler had warmed, **no object was rebuilt** (identical mtimes) and the program printed `flag B seen: 1`. On a cold directory it printed `2`.
- **Another clang.** Homebrew clang 22.1.8 first on PATH, same compiler, warm directory: no object rebuilt, and the program printed `clang major: 21`. Cold, it printed `22`.
- **The runtime object** (`runtime-*.o`, keyed on fingerprint, level, sanitizer and runtime text) was reused the same way.
- **Level and sanitizer are in the key:** 6 TU directories and 3 runtime objects for `-O0`, `-O2` and `--sanitize`. So the `run` legs do not contaminate each other.
- **Normal-use paths:**
  - A clang change: both clangs are on this Mac, and an Xcode update does the same. `clang_floor` writes `build/clang-version.txt` on every build, but that answer does not enter the key.
  - A `flags.hero` edit without a VERSION move. VERSION last moved at `b21c5b0d` (2026-09-07). The list grew twice since (`115c4dd1`, `-fno-delete-null-pointer-checks`, 09-15; `3379df9d`, `-fsigned-char`, 09-18), and nothing in `selfhost/` or `tests/harness/` empties `build/`. That warm trees reused pre-flag objects then is an inference. The trunk's `build/` today starts at 01:07, so it holds none.
  - The Linux container procedure excludes `build/` from its copy (`LINUX-MACHINE.md`), so cross-platform reuse is not a path.
- **Consequence:** any pattern-init or MSan leg needs its own build directory, or the flags in the key.

## Counts re-derived (`00-shared.md` and the compiler-engineer)

- **`h_keywords_keyword`:** 941 lines, 139 `= {0}`, 21 `hero_str_eq`, 23 owned variables. Confirmed by a line-based extraction of my own.
  - **Declarations: 181, not 182.** The 139 zeroed plus 42 plain sit between the `#line` and `goto bb0`.
  - The exit sweep is as described. One of the 23 is `HeroStr h24_own24`, not an option.
- **Seed:** 102,990 and 29,366 confirmed. **10,902 is not the owned slots**: that pattern misses 1,876 `HeroArrayHeader *` and 119 `HeroMapHeader *` synthetic slots. There are **12,897**, the compiler-engineer's number.
  - The brief measured at `dfcac362`; `git diff dfcac362 0fc98107` touches only three docs files, so the seed is identical.
  - Every line citation in the brief resolves: `body.hero` 156-157 and 204-205, `layout.hero:153`, `unread.hero` 95/124/232, ABI 26 at `heroes_runtime.h:37`.
- **Compiler-engineer:**
  - Census 81,680 / 12,897 / 8,413 (633 dead): confirmed.
  - Line counts: (a-min) +75 −7 in 2 files, (d) +135 −13 in 3, (f) +398 −23 in 5 (`git diff --numstat`): confirmed.
  - Zeroing counts: (d) 17,071, (e) 12,467, (f) 17,116, each on its own seed: confirmed.
  - My (a-min) emission equals the seat's seed once the source path is normalised.
- **(a-min) on suites the seat did not run:** `warnings` 271/0, `lines` 211/0, `units` 3/0, `cache` 6/0, `corpus` 55/0.

## Contradictions between the seats, and how to check them

- **What `-O2` does with the zeroing.**
  - The compiler-engineer says "at -O2 the optimiser removes the zeroing calls whatever the route". The ffi-pragmatist says "-O2 does not remove the dead zeroing … vector zero stores remain".
  - Both are literally true. `objdump` of the trunk seed at `-O2`: `h_keywords_keyword` has 0 `memset`/`bzero` calls, and **243 zero-store instructions** (`str`/`stp`/`stur` of `xzr` or `q0`) before its first call. (a-min) has **66**.
  - So the zeroing survives `-O2` as inline stores; the compiler-engineer's explanation of the 7% `-O2` gain rests on the call count alone.
- **The ffi-pragmatist's "273 slots in the seed are touched only that way"** (released through `&`): my count on the trunk seed is **0 of 21,310** zeroed slots. Every one has a by-value read, the old-value load. I found no 273 in the seat's `work/` other than a timing. The number is unsourced as stated.
- **(a-min)'s `@`-parameter clause against the ffi-pragmatist's objection to "(a) reaching slots without an IR check".**
  - (a-min) removes 633 slot zeroings. They are written by the prologue copy-in that `body.hero:250-264` prints for every mutable parameter before `goto bb0`. That is a structural fact of the emitter, not an analysis; measured 633/633 and 85/85.
  - The synthesis must say whether the objection covers this clause.
- **The ffi-pragmatist's report (02:03) predates (e), (f) and the compiler-built (d).** Neither route had an FFI reading until I re-ran its `boundary.py` with my (f) compiler on the 119 `grep -l extern` goldens.
  - Result: trunk and (f) identical. 436 calls; 0 zeroed argument locals; 25 addresses handed to C, 0 zeroed; 8 thunks, 0 zeroed locals.
  - The seat's veto condition is not met for (f).
- **The ffi-pragmatist's withdrawal condition** (the callback golden hangs on the route's real emission): I ran 100 runs each of the trunk and (a-min) builds, plain and `--sanitize`: **0 hangs**.
  - Found beside it: the plain `-O0` build's exit status varies with byte-identical output, 133 (SIGTRAP) 81 or 79 times and 134 (SIGABRT) 19 or 21 times, in both trunk and (a-min). That is pre-existing and not route-related.

## Framing facts nobody checked

- **`emit/body.hero:5`'s premise is false for C.** It says "the prologue exists because goto may not jump over an initialisation". C11 allows it: a file doing so compiled clean under the project's sixteen flags plus `-Wextra -pedantic` and ran; `clang++` refuses it. That is the premise behind declaring every value at the top.
- **design.md Part 5 (lines 2612-2626), the reader-facing half.**
  - It states rule 5 as "decrefed at the end of its defining block", which `own.hero:29-31` records as refuted.
  - It describes "a cleanup-label chain per function so every exit edge (…, panic, …) releases live locals". The emitted C has one sweep at the return block and none on panic.
  - It says only slots are zeroed; (a-min) makes the emitter match that sentence.
  - (b), (d) and (f) change rule 5 again, and (e) falsifies the zeroing sentence.
  - The spec is untouched: grep finds no zeroing in it. But design.md Parts 1 to 11 are within CLAUDE.md § 4's panel scope, so a landing owes a Part 5 edit and its log entry.
- **Default levels:** `heroes build` and `heroes test` default to `-O0`, `heroes run` to `-O2` (`cli/verbs.hero:31,56,105`). So the unoptimised cost is what every `build` pays by default, and brief item 4 names only the compiler's own build line.
- **The verifier's dominance is block-level** (`ir/values.hero:84-115`), as the compiler-engineer found; it runs after `own` (`cli/compile.hero:199-203`), and nothing checks the emitter's own C except clang.

## Routes nobody listed

- **(g) Declare each value where it is defined**, which C permits over a goto. It removes the value prologue outright. Its gain over (a-min) is unrun.
- **(h) An MSan leg on Linux arm64.** This is an instrument, not a repair, but it meets the text of the compiler-engineer's condition on (e): an instrument on a CI leg that sees a release through `&` of an unzeroed slot, with a negative control shown to fire. It covers executed paths only, like the pattern, so "on the rest, nothing" still stands. Not on CI; unrun there.
- **(i) A live-block baseline at the compiler's exit**, the destructor probe against the trunk's count, as the instrument for (b) and (f) in the one binary the gate never reaches.

## Asserted and not measured

- **(f) has one fewer live block at exit than the trunk on every command** (17→16, 827→826, 13→12, 828→827, 830→829). My guess is a synthetic slot in `main` holding an extra reference in the trunk until `exit()`, but that is unverified. Settle it by diffing `hero_runtime_live()` per function exit.
- **Windows and Linux x86-64** are not run for any route. The MSan results are Linux arm64 only.

## Questions the sitting should have asked

- Should the compiler binary reach the leak gate? It never does, and the fixpoint cannot see leaks.
- Should the unit cache key carry the clang flags and clang's identity? Now measured.
- Should MSan be a CI leg?
- **Which lane.** Soundness is right for the spec, but design.md Part 5 is a reader-facing text that routes (b) to (f) change. And the ffi-pragmatist never read (f), so (f) has had one seat.

## Files

All under `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/critic/work/`:
- **Analysers:** `defassign.py` and `defassign2.py` (the analyser), `census.py`, `clangpairs.py`.
- **Probes:** `probe_live.c`, `q3/` (cache probe), `hang/` (callback runs).
- **Injections and emissions:** `v/fx`, `v/fy`, `v/fz` (the injections) with their `*-run.out` and `fx-owntests.out`; `v/{f,d,e,amin}.seed.c`; `emitc/` (300 emissions and `analysis.txt`); `fyd/` (the (f), `fy` and `fz` emission comparisons).
- **MSan and boundary:** `msan/` (all MSan builds and `runs.out`, `runs_e.out`); `bnd/` (the boundary scans).

No process or container of mine is still running.
