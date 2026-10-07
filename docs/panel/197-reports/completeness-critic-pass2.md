# Panel 197, completeness critic, second pass

Copied by the coordinator at 17:21 on 2026-10-07 from the critic's reply,
verbatim in substance (a subagent's Write of a report file is refused). The
critic read the clock at 16:48:00 and 17:19:41. Its files are under
`<scratchpad>/197-critic/pass2/` (`shapes/results.txt`, `c18/result.txt`,
`ocost/res.txt`, `k/`, `gen/`), session-specific.

---

It read SKILL.md § 3b and § 3c, the three briefs, its first pass and both seat reports in full on the frozen tree `dad2da47`, and ran nothing on the trees or worktrees beyond `git` reads. Everything it ran is in `pass2/`: `tree/`, a copy of the frozen tree with `heroes-stock` built from its seed at `-O2`; `treeE/`, the same copy with the compiler-engineer's one `typeorder.hero` line copied in and `heroes-e` built from that copy's `selfhost/`; one throwaway `ubuntu:24.04` container running Ubuntu's clang-18. No clang went over 4 GB, no paid run, nothing still running (`pgrep`, `docker ps`). Counts only: the machine at load 13 to 26 throughout.

## 1. The seats' numbers, checked against their raw files

| Claim | Status | Command / what is true |
|---|---|---|
| CE item 1, the whole table | **verified** | Every cell matches `m400.txt` and `m800.txt`. The cut at `-O2` is 56.8 to 58.7% (400 returns) and 57.4 to 58.1% (800). |
| CE item 3, the sums | **verified exactly** | `awk` over `replay/*.tsv`: 471 units each, all exit 0. Instructions 246,365,780,759 / 236,701,445,845 / 211,095,368,508 / 621,685,981,260 / 520,770,808,099; the object sums and the largest units also match. |
| CE line counts | **toolchain 300→299, flags 211→239, link 240→241, typeorder 273→282: verified. compiling.hero 203→201: false.** | `suite_layout.hero:826-841`'s `code_lines` re-implemented in `gen/code_lines.py`: compiling.hero is **203→203** (the 2 lines removed at `:70-72` come back as the 2 comment lines added at `:134-135`). **"About +6 net code lines" uses another unit**: +6 is without comments (`gen/pure_code.py`: flags +11, compiling −2, toolchain −1, link −2); the seat's own per-file code_lines figures sum to +26. |
| CE `emission` 969/2 | **verified, on shape 1 only** (`suite-emission.txt`) | The emitter is the same in shape 2 (`cmp` of `typeorder.hero`). Only two blessed files carry the touch (`git grep -c 'p = at; (void)(p + 1)'`). |
| The brief's cases on the adopted shape 2 | **partly run** | Shape 2 ran own tests 1,357; layout 5/0; lines 373/0; `fixedbugs-170` 2/0; `dead-handle` 15/0. **Not run on shape 2:** `fixedbugs-140` (3 cases), the other six `!sanitizer:` filters (26 of the 41 cases shape 1 ran), and `emission`. The seat's transfer argument holds for its one probe: `diff eq-ce-O0/lines.txt eq-ce2-O0/lines.txt` differs in one line, the identity probe's `-O0`. |
| CE lldb at `-O2` | **verified, but on the second probe** | The first probe never stopped (`lldb/st-g-O2.txt`, `st-glt-O2.txt`): `total(limit: 4)` was folded to a constant. The claim rests on `lldb/st2-g.txt` (`limit: args().len() + 3`: 2 locations, `[opt] [inlined]`, `h0_limit=3`), one program. |
| CE: room rule broken | **held by the letter** | `m800.txt`: the overlapping `-g0 -O2` read peaked at 3,352,595,480 / 3,393,621,064 bytes, under 4 GB; only the 5.1 GB clang was over the line. Reporting the 8.5 GB overlap beside 13 lanes was still right. |
| CE: Windows o10000 dies "in the front end" | **the death is measured, the phase is inferred** | `win/one.sh` has only `c` and `S` modes, so the phase was never split. The evidence is `-Wstack-exhausted` at `o10000.c:332903` plus the death under `-g0` (`win-depth3.txt`, `win-depth4.txt`). |
| FFI sanitizer table | **verified** | Mac (`out/san-*.err`): UBSan `bind.h:22:21` under every word at both levels; ASan `#0 vec2_uaf bind.h:31` under `-g` and line tables, `vec2_uaf+0x118 (bin…)` under `-g0`; at `-O2` under `-g0` the header frame is gone. Linux 400-return numbers verified (`linux-400-mem.txt`). |
| FFI: ASan identical under (K) "on the Mac and on Linux" | **the Linux half is unrun** | Docker had only `heroes`, `heroes-g0` and `heroes-glt`; `linux-san.txt` holds `g`, `glt` and `g0` sections only. Linux is where ASan prints a column (`uaf.hero:5:10` under `-g`), so it is where (K) would change a report's text. |
| **FFI's (E) is not the adopted (E)** | **a different mechanism** | `e/e_ffi.c:9-15` and `e/step-e.c` add `__attribute__((used)) static struct vec2 *const hero_dbg_0 = 0;`, the critic's first-pass file-scope declaration applied to extern records. The adopted (E) is a cast inside `hero_tu_type_order`, emitted only past 32 deep (`typeorder.hero:114`) and never for a header's record (`:145`). FFI's condition, *Heroes types only*, holds by construction; its "3,168 → 7,120 bytes" and "behaves exactly like `-g`" describe a mechanism nobody proposes to land. For a binding program 32 deep or less the adopted (E) changes nothing in the emitted C. |
| FFI box run | **`-O0` only** (`box/run.sh:11-13`) | Under (C), Windows meets `-gline-tables-only` only at `-O2`; the `.pdb`'s lines at `-O2` with line tables are unrun. |

## 2. Measured in this pass

**The shapes next to (E)** (Mac, `heroes build` at `-O0`; stock is the frozen compiler, E the copy with the one line):

| Shape | Stock | E |
|---|---|---|
| v5000 + `--sanitize` | not run | 0, `0 true` |
| param-5000: a constant of the top type (emitted as a function, `h_k_TOP(void)`), a by-value parameter, `==`, no array | **2**, *Illegal instruction: 4* | 0 |
| mod-5000: the chain in `chain.hero`, used from `main.hero` (at depth 40 the types and the touch land in `main.c`; `chain.c` has 0 typedefs) | **2** | 0 |
| r-15000: records by value, `[R]` and `{R: i64}` | **2** | 0, `0 true 0` |
| ext-15000: the `fixedbugs-140` extern-records shape | **2** | **2** |
| v5000, control | — | 0 |

The extern-records death is not a debug-info death: the unit alone exits 254 under `-g`, `-gline-tables-only`, `-g0`, and at `-g -emit-llvm -S`. No route of this sitting reaches it, and E rightly leaves header records alone. No E build printed a clang warning (`b-e.log`, `warnings-*.txt`).

**Clang 18.1.3**, the version the CI's Linux legs install with `apt-get install clang` (`ci.yml:316`), run as arm64 in `ubuntu:24.04` on this Mac, not on the CI; `-g` is the `constructor` kind, stack 8,192 KB: v5000 `-O0 -g -c` stock 0, E 0; **v10000 stock 139 (SIGSEGV), E 0**.

**The mechanism (E) relies on holds on two more clangs.** `-g -emit-llvm -S` of the 40-deep units, read by `gen/retained.py`: stock keeps 3 retained types, `{R39*, const R39*, uint64_t}` (the top first); E keeps 42, opening `R0*, R1*, R2*` (the bottom first). The same on Ubuntu clang 18.1.3 and Apple clang 21.0.0; the compiler-engineer saw it on Debian clang 22.1.8. Windows clang 20.1.8 and the CI runners themselves unrun.

**New finding: the options shape costs clang super-quadratically in its front end, not in its debug information.** The compiler-engineer's `o5000-E-unit.c`, read once each: `-g` 2,170,997,429,944 instructions, 794,838,120 bytes; `-g0` 2,148,675,259,804, 538,968,856; `-g0 -fsyntax-only` 2,023,561,487,326, 200,032,808. Debug info is 1.0% of the instructions; the front end is 94% of the `-g0` compile. Against the seat's o10000 (16,474,318,665,016), cost grows ×7.59 per doubling (exponent 2.92) while the C grows linearly (597,570 → 1,195,070 lines, `wc -l`); o5000 costs 27× v5000. "o10000 builds on the Mac under E" means roughly an hour of clang; the Windows o10000 death, on a struct copy with `-Wstack-exhausted`, is probably the same front end. None of (A) to (L) reaches it. `issues/` searched for `options.*deep|through an option|nested option|option.*nest`: nothing filed.

**Searched, and turned from a question into a negative**: `clang -mllvm --help-hidden` prints 2,255 lines, its DWARF options none that limits depth; `clang -cc1 --help` none either; `clang -cc1 --help-hidden` prints nothing on Apple clang 21.

**(K) on a hand C file under `#line`** (`k/k.c`): UBSan prints `k.hero:5:59` under `-g`, `-g -gno-column-info`, line tables, line tables with no columns, and `-g0` (column 59 is the C column of `+`), so UBSan's false column comes from the front end and (K) does not remove it. ASan on Darwin prints no column under any word.

## 3. Contradictions between the reports

1. **(I), the probes at `-g0`**: the compiler-engineer refuses it, the ffi-pragmatist approves it as neutral; the compiler-engineer's measurement settles it (probe instructions within the spread): record (I) as not adopted for want of a gain, not because of a harm.
2. **(E)**: two different mechanisms (§ 1); the ffi-pragmatist's (E) numbers must not be cited for the adopted route.
3. **(K) on Linux**: the ffi-pragmatist says ASan is unchanged there; it was not run.
4. **The room rule**: § 1.

## 4. What (E)+(C) owes before it lands

1. **Batch 14 is editing the same files** (read with `git diff dad2da47` and `git status` in each `lane-b14-*`): `lane-b14-cli` has committed changes to `compiling.hero`, `link.hero` and `toolchain.hero` (74 lines in, 36 out) and uncommitted `toolchain.hero` work at **313** code_lines and a new `served.hero`; `lane-b14-emit` an uncommitted rewrite of `typeorder.hero`'s walk (`graph_of`, `deepest()`); the trunk is now `036b0baf`. The 299 fit, every line number above, and both shapes were measured on `dad2da47` and are out of date: (E)+(C) is rebuilt and re-counted on the trunk after batch 14.
2. The seed regenerated and the fixpoint by `cmp`: unrun for E+C.
3. On shape 2: `fixedbugs-140`, every `!sanitizer:` filter, and the re-bless of the two emission files.
4. The suites the `selfhost/emit/**` row names (`emission`, `emit`, `determinism`, `wholes`, `descriptors`, plus `canonical`, `layout`, `order`, `records`); **`cache` alone**, because both keys move (`toolchain.hero:120`, `:135`); `warnings`; then the full net at the batch.
5. **A test that goes red if (E) is reverted; today there is none.** The 32- and 100-deep cases build without (E) on every clang measured; `run` cannot hold the death depth (it builds every case at `-O0`, `-O2` and `--sanitize`; the 170 case's header says 1,000 deep took 62 s and 3,000 took 175 s at `-O2`, against the harness's 120 s). Option a: a mechanism check, `-g -emit-llvm -S` of a unit more than 32 deep, the first retained type the chain's bottom (under a second, run on two clangs above). Option b: an `-O0`-only build at 5,000 (v5000 under E took 23 s wall here; the CI's cost unrun).
6. **A test that goes red if (C) is wrong; today there is none.** If `debug_words` returned `-g0` nothing in the net would turn red. The cheapest guard: one ASan case whose needle names a `.hero` file and line (ASan prints `uaf.hero:5` on the Mac and `uaf.hero:5:10` on Linux under line tables; an ASan case, since UBSan keeps its location under `-g0`). For the variables at `-O0` the tools exist (lldb on the Mac; `lldb-22` and `/usr/bin/llvm-dwarfdump-22` in the Docker image; `dwarfdump` and `xcrun llvm-dwarfdump` on the Mac; lldb on the CI's Linux leg): M-typed-inspection's step 1, pulled forward, or the synthesis says the `-O0` half is pinned only by `flags.hero`'s text and one hand transcript.
7. **Platforms**: the CI matrix has **four** legs (`ci.yml:279-290`: Linux x86-64, Linux arm64, macOS 26, Windows). Owed: Windows at `-O2` with line tables (the `.pdb`'s lines, and the 400-return shape's memory on the 11 GB box, where panel 190's R9 recorded the 800 shape dying of memory); an lldb run in the Docker image.
8. **What the cards can say when they close.** 219, row by row: Mac, v up to 10,000 and o5000 build, o10000 about an hour of front end; Linux, both shapes up to 10,000; Windows, v10000 and o5000 build, stock too, **o10000 dies under every word**. The Windows death and the options front-end cost are each a new defect, beyond R6's floor, so `improvement`, with the box's phase split owed. 322: (C) cuts 57 to 59%, both words still grow ×3.85 per doubling; `issues/` searched for `GVN`, `fourfold`, `per doubling`: no open item on that growth, so if 322 closes the growth is recorded nowhere.
9. **Documents that move** (`grep` over `docs/design.md`, 7 lines, and `git grep` over the living files): design.md `:630-632` (Part 2) and `:3682` (Part 8, item 12) move; `:653-654`, `:682`, `:750`, `:756`, `:83` stay true (line tables kept at every level); `M-typed-inspection.md:20` (*`-g` is on every build*) and `:55` (falsifier *`-g` deleted*) move; `M-vscode-extension.md:16-29` moves (the debug launch builds at `-O0`); **`tests/golden/run/fixedbugs-170-variants-and-options-100-deep-build.hero:19-20`** says *at 5,000 clang's debug information still dies*, which (E) makes false (append a dated correction at the end, shifting no line); `.claude/rules/generated-c.md` § Flags gains one sentence (the debug word no longer in `flags()`); outward-facing files none (`git grep` over `site/`, `examples/`, `spec/`, `editors/` empty; `claims.ts` reads no file the change touches).
10. **Defect 335**: neither seat exercised it; any lldb check the landing adds must not bless 335's wrong line.
11. **Unfiled**: `step` into a Heroes function lands on the generated C (`dbg.c:34:5`) under every word; design.md `:682`'s intended attribution, but an author sees C; `issues/` searched for `step into|stepping into|lands on .*\.c:|prologue.*lldb`: nothing. File it or record it as `:682`'s consequence.

## 5. Does the soundness lane still hold for (C)?

**Yes, with two conditions.** No surface moves: (C) rides the existing `-O0`/`-O2` words (`compile.hero:64-75`, `table.hero:105-121`, accepted by `build`, `run`, `test`); `heroes run -O0` keeps the full variables. The spec is untouched. The reader is told by lldb itself: at `-O2` *"no variable information is available in debug info for this compile unit"* (`look.txt`) and *"compiled with optimization - … variables may not be available"* (`st2-g.txt`). **Condition 1**: `-O2`'s help line, *compile at -O2*, stays as it is (changing `table.hero` touches a surface `claims.ts:48` reads, and for that sentence the lane would not hold). **Condition 2**: the Part 2 and Part 8 amendments land in their own commit, citing this sitting. **What the lane gave up**: the historian, so whether other toolchains tie variable debug info to the optimisation level by default stays an open question (no search run).

## 6. (K): its own item, not this landing

Measured: objects −5.95% at 400 returns, `-O0` (463,240 → 435,688 bytes); memory does not move (Linux −0.21%); the false caret in lldb goes away. Unrun: Linux ASan's `:col` (UBSan's false column stays under (K), so on Linux the two reports of one run would disagree), the column VS Code highlights through the debug adapter, whether CodeView carries columns on the box. It solves neither 219 nor 322; landing it with (C) puts two behaviours behind one cold cache and one bisect.

## 7. Repairs to the synthesis

1. compiling.hero 203→203; "+6 code lines without comments, +26 by `code_lines`".
2. Do not cite the ffi-pragmatist's (E) numbers for the adopted route; its condition holds by construction (`typeorder.hero:114`, `:145`).
3. Mark (K) on Linux, and Windows `-O2` line tables, as unrun.
4. Rebuild and re-count E+C on the trunk after batch 14's `cli` and `emit` lanes merge.
5. A mechanism test (or an `-O0` depth case) for (E) and a location needle for (C) as conditions of the landing, or write that neither is pinned.
6. 219's closure as § 4.8's rows; file the Windows o10000 death and the options front-end cost as new defects.
7. Keep 322 open on its growth, or file the growth.
8. Name § 4.9's design.md, milestone, golden and rule sentences as owed.
9. (I) not adopted for want of a gain.
10. File (K) as an `improvement`.
11. Record that the CI has four legs.
12. Record that Ubuntu clang 18.1.3 holds (E) in a container (arm64, not the CI).
