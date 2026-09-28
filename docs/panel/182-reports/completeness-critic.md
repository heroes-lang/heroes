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
