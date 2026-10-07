# Panel 197, compiler-engineer's report

Copied by the coordinator at about 16:50 on 2026-10-07 from the seat's final
reply, verbatim in substance (the seat's Write tool refused its report file, so
the reply is the record). `<S>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/197-compiler-engineer`,
session-specific; what must survive is this file. The seat ran from 12:36:20,
was stopped by session limits twice, resumed at 13:42:29 and about 14:48, and
last read the clock at 16:44:23.

---

## Verdict

- **verdict:** **object** to route (B), `-gline-tables-only` everywhere, which is defect 322's own route. I'd adopt **(E) + (C)** instead. Both are built and pass the shared brief's cases. No veto: neither adds a core construct, and the ceiling holds, but only in the second shape I built (see implementation_cost).
- **section:** design.md §1.1 (simplicity sets the ceiling), §1.12 (robustness: defect 219 is clang dying on a correct program), §3.1 `:653-654` (the line mapping) and Part 2 `:630-632`.
- **implementation_cost** (counted with `suite_layout.hero`'s own `code_lines` rule, so comments count):
  - **(E):** one emitter line in `selfhost/emit/typeorder.hero` (273 → 282 counted, 358 → 367 raw, the rest is its comment), plus 2 test lines. It changes 2 blessed emissions, `tests/emission/run-fixedbugs-140-records-a-thousand-deep-build.c` (1,002 lines) and `run-fixedbugs-170-variants-and-options-100-deep-build.c` (152 lines). The seed changes only in two string literals and in the `#line` numbers that comment shifts.
  - **(C):** about +6 net code lines in `selfhost/cli/`: `flags.hero` 211 → 239 (`-g` leaves `flags()`; new `debug_words(level)` and `level_words(level, sanitize)`); `compiling.hero` 203 → 201; `toolchain.hero` **300 → 299**; `link.hero` 240 → 241; tests +15 / −12 lines. My first shape put `toolchain.hero` at **305**, and `layout` read 4 passed, 1 failed. The file sits exactly at its 300 ceiling today, so (C) only fits with the shared `level_words` list.
- **needed_for_self_hosting:** no. The compiler builds at `-O0` and has no chain past 32 deep. (E) changes none of its units. Under (C) its clang lines are identical except the identity probe, which gains `-O0`.
- **argument:** (B) buys a constant factor and pays with the `-O0` debugger. On today's one-exit C, line tables cut clang's `-O2` footprint by about 57% (800 returns: 7.79–7.92 GB down to 3.26–3.38 GB), but both still grow about fourfold per doubling. And at `-O0`, `frame variable` becomes *"no variable information is available"*, which removes the half of M-typed-inspection that already works. Defect 219 is not a flag question. LLVM's DWARF writer meets the top of the type chain first, through the type of the element descriptor's casts (`retainedTypes`). Writing `in_order`'s touches as casts (one emitter line) makes 5,000 and 10,000 build under full `-g` on this Mac and on Linux. So: (E) for 219, (C) for 322, at about six net code lines.
- **prediction:** at the batch that lands E+C: `emission` reads **969 passed, 2 failed** before the re-bless (the two files above, touch lines only, line counts unchanged; heroes-ce read exactly that at 15:24); every other gate suite reads 0 failed with no re-bless, and `layout` counts `toolchain.hero` at 299; on this Mac, `heroes run` of the 800-return shape peaks at **≤ 3.5 GB** of clang footprint, against 7.79–7.92 GB today.
- **condition:** against (C): an lldb census of real programs at `-O2` under `-g` showing locals surviving, not just parameters; or either milestone committing to debug `heroes run` binaries; or the Windows `.pdb` under line tables failing to map lines. Against (E): any platform where E's unit dies at a depth where the stock unit compiles (none found at 3,000, 5,000 or 10,000 on three platforms); or the CI's Apple clang reddening defect 170's 32-deep case. For (B): an author ruling in Part 2 that `-O0` variables aren't wanted.

## Item 1: the N-return shape on today's emission

Generator `<S>/probe/v.py N b`: `s0 = x.to_str() + ":"`, then `s<i> = s<i-1> + x.to_str()`, each followed by `if x == i` / `return s<i>`. Emitted with `heroes build --emit-c`: **29,417 lines and 2,403 `hero_str_decref(`** at N = 400; **58,617 and 4,803** at 800; 7,517 and 603 at 100. Panel 190's A-star figures are 29,418 / 58,618 / 7,518 lines with the same release counts, so **today's C is A-star's C**, and the card's numbers describe a C file that nothing emits any more.

Instrument `<S>/fp.sh`: `timeout 1800 /usr/bin/time -l clang -fintegrated-cc1` with `flags()`'s sixteen words, `-g` swapped for the route's word. Files `<S>/m400.txt`, `<S>/m800.txt`. Every compile exited 0. Peak footprint in bytes, two reads each (instructions retired at `-O2` in brackets):

| words | 400, `-O0` | 400, `-O2` | 800, `-O0` | 800, `-O2` |
|---|---|---|---|---|
| `-g` | 57.2M / 56.9M | **2,046,150,600 / 2,041,923,528** (543.0e9 / 539.6e9) | 124.1M / 124.3M | **7,924,799,896 / 7,790,025,112** (2.663e12 / 2.674e12) |
| `-gline-tables-only` | 48.2M / 50.9M | **844,104,736 / 881,263,672** (507.0e9 / 505.4e9) | 113.2M / 113.4M | **3,376,008,216 / 3,261,549,568** (2.501e12 / 2.497e12) |
| `-g0` | 48.5M / 45.2M | 883,360,848 / 882,328,656 | 101.4M / 101.2M | 3,352,595,480 / 3,393,621,064 |
| `-g -fno-standalone-debug` | 56.9M / 56.8M | 2,009,040,864 / 2,043,398,088 | not run | not run |
| `-g -gno-column-info` | 55.1M / 54.9M | 2,011,547,592 / 2,041,219,040 | not run | not run |
| `-gline-tables-only -gno-column-info` | 48.6M / 48.3M | 883,082,320 / 880,002,104 | not run | not run |
| `-g -gdwarf-4` | 58.4M / 56.5M | 2,011,269,088 / 2,042,365,896 | not run | not run |

Objects at `-O2`, 400 / 800: `-g` 1,695,472 / 5,937,336; line tables 1,469,832 / 5,483,848; `-g0` 1,341,416 / 5,229,520. At `-O2`, line tables cost the same as `-g0`: all of `-g`'s excess is variable and type DWARF. `-fno-standalone-debug`, `-gno-column-info` and `-gdwarf-4` move nothing. Against the card: today's `-g` at 400 is about 2.04 GB, **+20%** over the card's 1,695,369,184, and today's line-tables figure 0.84–0.88 GB is below the card's 970,589,672, so today's cut is **−57%**, not −42.75%. Both words grow 3.7 to 4.0 times per doubling of N: (C) and (B) lower a constant factor and do not change the growth.

## Item 2: defect 219's depth

The generator `<S>/gen/variants.py` reproduces both `fixedbugs-170` cases byte for byte, comments stripped (`diff`). All builds `heroes build` at `-O0`; "E" is the copy's compiler with the cast touch.

| shape | Mac, stock | Mac, E | Linux arm64 container, stock / E (cgroup peak) | Windows box, unit alone, `-g` (CodeView), stock / E |
|---|---|---|---|---|
| v3000 | 0 | 0 | 0 / 0 | not run |
| v5000 | **2**, *Illegal instruction: 4* | **0**, prints `0 true` | 0 / 0 | 0 / 0 |
| v10000 | **2** | **0** | **2**, signal 11 / **0** (1.11 GB) | 0 / 0 |
| o5000 | **2** | **0** | **2** / **0** | 0 / 0 |
| o10000 | 124 (timed out at 1,800 s under load 100–260, no verdict) | build 124 (timed out); its unit alone exits **0** (16.47e12 instructions, 1.62 GB, 50 MB object) | **2** / **0** (2.58 GB) | **139 / 139** at `-g`, line tables and `-g0` alike |

**Which phase dies.** Mac, stock v5000 unit (`<S>/dp.sh`): `-emit-llvm -S -g` exits 0; `-c -g` exits 139 in-process (254 through the driver); `-c` exits 0 under line tables and `-g0`; `-g -fno-standalone-debug` still 139: **the backend dies**. Linux, stock v10000 unit: `-fsyntax-only` 0, `-emit-llvm -c` 0, `-c -g` **139**, `-c` with line tables 0 (`-emit-llvm -S` also 139, the textual IR printer, not the build): the backend dies here too. **Windows** survives at v10000 and o5000; at o10000 it dies in the **front end** under every debug word, stock and E alike, clang warning *"stack nearly exhausted"* at `o10000.c:332903` (`*(h_o10000_R994 *)dst = *(const h_o10000_R994 *)src;`). No debug route reaches this; it is a separate finding.

**Which declaration:** the `.ll` of the 5,000 unit has `retainedTypes = {h_v5000_R4999 *, const h_v5000_R4999 *, uint64_t}`, from the casts in `h_v5000_R4999_desc_copy` and `desc_eq`, the array's element descriptor. LLVM builds retained types before any function, so it meets the chain's top first. Linux's `.ll` carries the same kind of list under the `constructor` kind.

**(E)** is `typeorder.hero`'s touch written `(void)((T *)at + 1);` in place of `{ T *p = at; (void)(p + 1); }`. At 3,000 slightly cheaper than stock: 34.9e9 / 33.4e9 instructions against 35.9e9 / 36.6e9, a 6,019,344-byte object against 6,183,440. The E 5,000 unit compiles at `-O2 -g`, at a **5,115,318,520-byte footprint**. In lldb a 40-deep variant value looks the same under stock and E (the only differences are uninitialised temporaries).

**(A), what each platform allows:** Darwin hard stack limit 65,520 KB (`ulimit -Hs`), exactly the value already measured, so no margin; Linux container soft 8,192 KB, hard unlimited; Windows clang.exe's `SizeOfStackReserve` 10,000,000 (`llvm-readobj` on the box), fixed in its own header. Three different numbers, what panel 107 refused (`flags.hero:150-160`), and (A) cannot reach the Windows front-end death.

## Item 3: the compiler's own build

477 clang lines logged with a PATH shim: 471 units, the runtime, the link and 3 probes; the units replayed alone with `<S>/replay.py`, six at a time; results in `<S>/replay/*.tsv`. Each footprint read once.

| units replayed under | instructions, sum | object bytes, sum | largest unit footprint |
|---|---|---|---|
| `-O0 -g` (today) | 246,365,780,759 | 31,827,160 | 36,209,048 (checkwalk) |
| `-O0`, line tables | 236,701,445,845 (**−3.9%**) | 20,019,848 (**−37%**) | 28,623,232 |
| `-O0 -g0` | 211,095,368,508 | 14,214,032 | 23,888,256 |
| `-O0 -g -fno-standalone-debug` | 246,665,203,470 | 31,827,160 | 36,012,464 |
| `-O2 -g` | 621,685,981,260 | 33,890,248 | 119,488,968 (emitffi) |
| `-O2`, line tables | 520,770,808,099 (**−16%**) | 20,244,896 (**−40%**) | 52,216,216 |

## Item 4: lldb on this Mac

Program `<S>/lldb/dbg.hero` (three functions and a loop); the other words rebuilt through the compiler's own logged lines (`<S>/lldb/rebuild.py`), relinked; transcripts `<S>/lldb/st-*.txt`, `look.txt`. **Line stepping is identical** between `-g` and line tables at `-O0`; the transcripts differ only in that frames lose their argument values (`h_dbg_total(h0_limit=4)` becomes `h_dbg_total`) and `finish` prints no *Return value*. `b dbg.hero:19` resolves to `h_dbg_total + 156` under both; under `-g0` *"no locations (pending)"* and the program runs to the end. Under line tables `frame variable` says *"no variable information is available in debug info for this compile unit"* and `p h1_sum` is *undeclared*. At `-O2` under `-g`, `frame variable` shows only `h0_limit = 3`, the locals undeclared; stepping at `-O2` identical under line tables. **So what (C) gives up at `-O2` is the parameters' values.** The E+C compiler's builds give the `-g` transcript at `-O0` and the line-tables transcript at `-O2`. **Columns:** `dbg.hero:19:10` is column 10 of the C statement `    t9 = h1_sum;`, and lldb draws the caret under the `u` of `sum` in the `.hero` line, which means nothing; (K) removes the column and the caret. `step` into `square` lands on `dbg.c:34:5` under every route (not route-specific). Defect 335 was not exercised.

## Item 5: the cache key and the tests, per route

(B): the word sits in `flags()`, so `compiler_key` (`toolchain.hero:120`) moves and every build directory goes cold once; test edit `flags.hero:211`. (C), as built: `debug_words` enters `unit_words` (and through it each unit's key and every probe's words), `build_words` (the runtime key, now `-O0|-g|…`), the identity question (asked with `level_words("-O0")`), and the link line, which matters on Windows: clang 23.1.1's driver adds `-debug` (the `.pdb`) only when the link line carries a `-g`-family word (`clang -###` on the box). Tests edited: `flags.hero`'s array test (15 words, indices shifted, `debug_words` asserted), `compiling.hero` at three lines, `toolchain.hero` at two `build_words` asserts. (E): the change is in the unit text, which the key already names; no key edit. (G), (H), (D), (L): the word must enter both keys; (H) would also need a record of the death, or every warm build dies again. (I): no gain measured (probe instructions under `-g` and `-g0` within the spread: reads 186.4M vs 182.7M, ast-dump 200.7M vs 200.0M, syntax check 199.2–204.9M vs 199.1–203.1M), and it would cold every probe key once: refused.

## Item 6: what was built and run

heroes-ce (first shape): the compiler's own tests **1,357, all passed**; `lines` **373 / 0**; `run fixedbugs-170` **2 / 0**; `fixedbugs-140` **3 / 0**; the `!sanitizer:` cases over 7 filters **41 / 0**; `emission` **969 / 2** (the predicted two); `layout` **4 / 1** (`toolchain.hero` at 305). heroes-ce2 (second shape, `<S>/tree2`): own tests **1,357, all passed**; `layout` **5 / 0**; `lines` **373 / 0**; `fixedbugs-170` **2 / 0**; `dead-handle` **15 / 0**; its clang lines match heroes-ce's except the identity probe's `-O0`. An ASan report built with `run --sanitize` through the compiler's own path is identical under stock and E+C once addresses are masked (`c-frees-a-lease-and-the-runtime-names-it`). The stack guard prints `in deep.down` under both at `-O0` and `-O2`.

## The other routes

| route | what it costs | M-typed-inspection | M-vscode-extension |
|---|---|---|---|
| (B) line tables everywhere | 1 word; −37% objects; −57% `-O2` memory | its working half is gone; its falsifier fires on day one | variables pane empty |
| (C) line tables at `-O2` only | built, above | intact if its suite builds at `-O0` | intact if its launch uses `heroes build` |
| (E) cast touch | built, above | untouched | untouched |
| (A) raise clang's stack | runtime C, unbuilt and uncounted | untouched | untouched |
| (F) name the debug kind | moves nothing | — | — |
| (G) per-unit chooser | rests on a per-platform depth premise, which the death points show differ (Mac stock dies by 5,000, Linux by 10,000 or at o5000, Windows not at 10,000) | a deep program loses its variables | same |
| (H) retry on death | a failed compile first (55.3e9 instructions and 369 MB at 5,000) plus the retry, against E's 80.1e9 once | debug info depends on the machine | same |
| (I) probes at `-g0` | no gain (item 5) | — | — |
| (J) `-g0` | breakpoints don't resolve (measured) | — | — |
| (K) no column | 1 word; −5.9% object at 400 `-O0`; removes the false column | — | its DAP column changes |
| (D), (L) | a new flag or variable: not adoptable in this lane | — | — |

I'd take (K) as a separate one-word item, not part of this adoption.

## Unrun, and two rules broken

**Unrun:** the stock 5,000 unit at `-O2 -g` (stopped at 14:59:59 under the room rule); (E) beyond 10,000; lldb on Linux or Windows, and the Windows `.pdb` under line tables beyond the driver's `-debug`; the CI's clangs (18.1.3, 20.1.8, the Darwin runner's); the full net, and the seed fixpoint by `cmp` for E+C (`<S>/seed-e.c`, emitted by heroes-e, differs from the frozen seed only in typeorder's two literals and `#line` numbers); 800 returns under the other words; every footprint of item 3 read once, not twice.

**Rules broken:** the E 5,000 compile at `-O2 -g` (5.1 GB) overlapped the seat's own 800-return `-g0 -O2` read (3.35 GB), two big clangs at once against the shared brief's room rule; and `docker/phase.sh` was edited while a container read it, that one reading (exit 127) discarded and re-run (exit 0).

**Files:** `<S>/tree` (E + first shape of C), `<S>/tree2` (E + second shape), `<S>/patchC.py`, `<S>/patchC2.py`, `<S>/m400.txt`, `<S>/m800.txt`, `<S>/replay/`, `<S>/deep/`, `<S>/docker-depth.txt`, `<S>/win-depth*.txt`, `<S>/lldb/`, `<S>/suite-*.txt`, `<S>/suite2-*.txt`. On the box: `/c/w/p197-compiler-engineer-41872`, left in place.
