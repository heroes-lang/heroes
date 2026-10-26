# Panel 230, the coordinator's clock: every timed run the seats prepared, in one still window

**The window**, 2026-10-26 13:29:30 to 14:08:11, run by `window.sh`
(`.claude/worktrees/panic-location-scratch/timing/`, each run's `/usr/bin/time -lp` in its own log
there), the seats' commands of `compiler-engineer.md` § 8 and `ffi-pragmatist.md` § 15 unchanged
but for the two removals written `"${VAR:?}"` (a safety check refused the bare form). **The machine
was held**: the two other sessions on this Mac each answered *ok, holding* before the first run
and were released after the last; this session ran nothing beside it. `pmset -g assertions` read
`PreventUserIdleSystemSleep 1`; the load average read 3.06, 3.97 and 3.77 at the start, the middle
and the end (`uptime`), the machine's background, and **no run waited**: `real / (user + sys)` is at
most 1.10 over every kept run, 1.01 over the emissions. The first run of each group is discarded and
the best of the rest is reported, with every run's `real` beside it.

## The noise floor, measured first

| run | reals (s) | kept spread |
|---|---|---|
| `heroes-before build selfhost/main.hero --emit-c -o /dev/null`, six times | 46.91, 45.90, 45.71, 45.68, 45.87, 45.68 | 45.68 to 45.90, **0.22 s, 0.48%** |

## The compiler's path that does not abort

| run | before | after | the difference |
|---|---|---|---|
| **the compiler-engineer's**, emission alternated, 6 each: `heroes-before` against `heroes-after` (L1 for kind 1, L1a for the slice, M1; the compiler writes the places) | best **45.46** (45.48, 45.70, 45.63, 45.66, 45.46, 45.52); instructions 584,480,325,368 | best **46.03** (46.70, 46.23, 46.21, 46.09, 46.03, 46.07); instructions 591,299,477,124 | **+0.57 s, +1.25%**, the kept ranges apart (45.70 below 46.03), 2.6 times the floor's spread; instructions **×1.0117** |
| **the ffi-pragmatist's**, three compilers of one source emitting the same bytes, today's runtime against ABI 31 with the places in the compiler's own aborts, 4 each | `before` best 45.42 (46.53, 47.15, 46.35, 45.42); instructions 583,774,072,886 | `L1` best 45.47 (45.43, 48.73, 45.47, 45.65), 583,911,568,833; `L1a` best 45.63 (46.56, 47.42, 45.96, 45.63), 585,894,421,298 | `L1` **+0.02%** of instructions, `L1a` **+0.36%**; the reals within this group's own spread |

**So**: the places a program carries cost its path that does not abort next to nothing (`L1`, the
compiler as the program, +0.02% of instructions); passing one to every call of a kind-2 entry point
costs 0.36% on the compiler; and the compiler is 1.25% slower because it writes 3.3% more C, the
work the feature is. **The compiler-engineer's prediction**, *the after compiler retires between
1.00 and 1.04 times the before compiler's instructions*, reads **1.0117: a hit** for the emission.

## The whole build, cold (the cache emptied before each)

| | best real (s) | runs | instructions |
|---|---|---|---|
| before | **86.93** (user 66.35, sys 14.80) | 86.57, 86.93, 87.51 | 606,800,062,965 |
| after | **89.72** (user 68.58, sys 15.28) | 90.17, 90.87, 89.72 | 622,047,412,151 |

**+2.79 s, +3.2%**; instructions ×1.0251 (inside the same prediction's 1.00 to 1.04: a hit).

## clang over the seed, with and without the 17,277 records

`cc.sh` (the flags of `flags()` at `595d92d88`, the level and its debug word), three each:

| level | `seed/heroes.c` | `seed_L1.c` |
|---|---|---|
| `-O0` | best 12.96 (12.41, 12.96, 13.65) | best 13.05 (12.47, 13.05, 13.83) |
| `-O2` | best 50.99 (46.34, 50.99, 51.08) | best 51.14 (49.32, 53.05, 51.14) |

**Inside each pair's own spread**: the records do not move clang's time a measurable amount.

## Kind 2 on the path that does not abort: 20 million calls each, three runs

| benchmark | `-O0` best | `-O2` best | `-O2` instructions |
|---|---|---|---|
| slice, before | 0.84 | **0.44** | 10,918,575,259 |
| slice, L1a (a pointer to a per-site record) | 1.05 | **0.45** | 11,618,749,750 (+6.4%) |
| slice, L1b (the check in the program's C) | 1.06 | 0.49 | 11,700,613,559 (+7.2%) |
| slice, L1c (a status the program tests) | 1.07 | 0.47 | 11,740,518,283 (+7.5%) |
| indexed store, before | 0.26 | **0.09** | 2,357,725,167 |
| indexed store, L1a | 0.29 | 0.11 | 2,677,860,224 (+13.6%) |
| indexed store, L1b | 0.28 | **0.09** | 2,437,842,089 (+3.4%) |

**The fastest form depends on the entry point, measured**: for the slice L1a (+2% at `-O2`, the
fewest instructions of the three), for the indexed store L1b (no difference at `-O2`, the form the
index read already takes). At `-O0`, `heroes build`'s default, every form costs the slice about 25%
in this loop of nothing but slices.

## The dSYM step L2 and L5 would need on Darwin

`dsymutil heroes-kept`: 0.48 and 0.36 s. Neither route is adopted (`ffi-pragmatist.md` § 16).

## Sizes (deterministic, measured after the window)

| | before | after |
|---|---|---|
| `seed/heroes.c`, the ffi-pragmatist's per-site records (`seed_L1.c`, `seed_L1a.c`) | 27,000,911 | 28,997,679 (+7.4%), 29,066,744 (+7.6%) |
| `seed/heroes.c`, the compiler-engineer's literals on the failing branch (its report § 6) | 27,000,911 | 27,889,403 (+3.29%) |
| `__TEXT`, the ffi-pragmatist's compilers (`size`) | 12,795,904 | 13,008,896 (+1.66%) |
| `__DATA_CONST`, the same (`otool -l`) | 0x8000 | 0x70000 (+416 KiB, rebased at load) |
| `__TEXT`, the compiler-engineer's compilers (`size`) | 14,581,760 | 15,106,048 (+3.6%, M1 and the slice's places included); 15,138,816 with S1 |

**The literals on the failing branch carry half the seed's growth of a per-site record and no
load-time rebase**: the ffi-pragmatist's own refinement (*one record per function, line and column
as an immediate*), which its timing condition did not trigger (+0.02%) and the compiler-engineer's
form already meets.

## Corrected 2026-10-26, on the completeness critic's second pass (`critic-reports.md`, findings 4, 5 and 16)

Written beneath rather than over.

- **The kind-2 table compared the prototypes' bodies, not the routes** (finding 4). Per call, from
  these logs' instructions (kept minimum less `before`'s, over 20,000,000): the slice's L1a costs
  35.0 extra instructions at `-O2` and 125.0 at `-O0`, L1b 39.1 and 131.0, L1c 41.1 and 157.0; the
  store's L1a 16.0 and 32.0, L1b 4.0 and 16.0. The compiler-engineer's own count for a pointer
  argument is about **two** (`compiler-engineer.md` § 9). So *the fastest form depends on the entry
  point, measured* is a reading of how each `_at` body was written, and **the route's own cost is
  unmeasured here**; by arithmetic on 17,163,158 stores a self-emission, either form of the store
  costs the compiler under 0.1% of its 584 billion instructions. The settling command is the
  critic's: `hero_str_slice_at` and the store's entry point written as the originals' bodies with the
  place added, timed against the same loops.
- **The two record forms were compared on one axis** (finding 5): seed bytes and load-time rebases
  favour the literals, while `__text` grows about twice as fast under them (+3.15% against +1.66%),
  and clang over one unit was timed only for the per-site records, the whole cold build only for the
  literals. The choice of literals is therefore **made on bytes and loader fixups alone**; the other
  two axes are owed by the landing, `cc.sh` over the compiler-engineer's literal seed beside
  `seed_L1.c` at both levels.
- **The clang pairs' "inside the spread" is "not measured at this resolution"** (finding 16): their
  `-O2` reals spread 10% over three runs (46.34 to 51.08, 49.32 to 53.05), which cannot exclude a few
  percent. Every comparison read on **instructions** stands, and so does the compiler-engineer's
  +0.57 s on the emission, well clear of its own 0.20 to 0.24 s spread.
- **"Because it writes 3.3% more C" is an inference** (finding 16): the after compiler also computes
  18,988 places and passes the slices' four literals (about 113 million instructions by the
  compiler-engineer's arithmetic, about 1.7% of the 6.8 billion measured difference), and no run
  separates the three.
