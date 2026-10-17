# Panel 218: a header constant's initialiser is read from clang's dump and run under the probe's checks at -O0, and every probe asks the C standard's meaning

**Convened 2026-10-17 from 02:38, synthesised the same day from 10:45**, on defect 2109
(https://github.com/heroes-lang/heroes/issues/2109, `systemic`), by the author's answer of that
night accepting the coordinator's recommendation, *a sitting*; defect 2114
(https://github.com/heroes-lang/heroes/issues/2114, `blocking`), found by this sitting's
ffi-pragmatist, put to its synthesis. **Lane: soundness**: compiler-engineer, ffi-pragmatist, then
a completeness critic, the seats of panel 216. No route adopted adds surface, a diagnostic class or
a spec token, so the sitting was not re-convened full (the brief's rule). **Resolution:
provisional, author ratification pending.** Brief: `docs/panel/218-briefs/00-shared.md`. Reports:
`docs/panel/218-reports/` (`compiler-engineer.md`, `ffi-pragmatist.md`, `critic.md`). Every number
below is a seat's, with its command in that seat's report.

## The proposal, verbatim (the shared brief's question)

> A group's integer `constant` that names an enumerator, a `static const` object, or a brace-list
> record, whose header computes its value in an initialiser that C leaves undefined, builds at exit
> 0 and prints a stable value, where the same expression spelt as a `#define` is refused today.
> What does the compiler do with it?

Routes A, B and C were the coordinator's; C2 (references followed, the probe at `-O0`) the
compiler-engineer's repair of C; the ffi-pragmatist's *C as built* (rooted at the accessor, the
closure, the predecessor rule, records leaf by leaf) the same repair reached apart; B6, B7, Bf, Bp
and K-init the ffi-pragmatist's; K23 (C23's `constexpr`), the value-agreement guard and the
references spliced at their type the critic's.

## What was measured

- **The class is wider than the brief's three shapes.** 16 undefined shapes of the
  compiler-engineer's 35 and 28 of the ffi-pragmatist's control build at exit 0 with a stable
  value on clang 18.1.3, 21 and 22.1.8; the same holds of every undefined leaf of every record
  constant, which clang folds in its front end with 0 traps under R1's checks at `-O0` and `-O2`
  (the ffi-pragmatist, `recs-O0.ll`, `recs-O2.ll`); and of shapes reached through a reference, 10
  of 10 (the compiler-engineer's attacks: `EB = EA + 1`, a macro naming a `static const`, a brace
  list naming an enumerator).
- **A real instance in the C library**: glibc's `<sys/mount.h>` line 124, `MS_NOUSER = 1 << 31`,
  builds and prints `-2147483648` on Debian clang 22.1.8 and Ubuntu clang 18.1.3. Of 33,043 real
  enumerators, const objects and brace macros on three clangs at both levels, it is the one C
  leaves undefined (the ffi-pragmatist's population, 110 to 135 libraries per leg). No real header
  of those libraries defines a `static const` scalar or record object; Windows' SDK defines five,
  all defined values.
- **B, clang's own diagnostics at the initialiser, sees too little**: 6 of the 16 never seen on
  any clang (every object and record holding `INT_MIN / -1` or a float cast), the floor's clang
  18 narrower than 21 and 22, and its strongest form `Bp` refuses `EPOLLET = 1u << 31`, a value C
  defines, on Debian clang 22.
- **C2 / C as built**: 16 of 16, 10 of 10 and 28 of 28 undefined refused, 18 of 18, 4 of 4 and 28
  of 28 defined kept, at `-O0` and `-O2`, on Apple clang 21.0.0, Debian clang 22.1.8 and Ubuntu
  clang 18.1.3 alike; the corpus 0 trapped on all three (57 roots, 120 to 124 constants asked);
  the real headers' one refusal `MS_NOUSER`.
- **The level** (the seats' conflict, settled by the critic on the ffi-pragmatist's own
  prototype): an initialiser is computed by clang's constant evaluator at any level, a probe at
  `-O2` by the optimiser, and they part on `__builtin_constant_p`: at the program's `-O2` the probe
  refuses `S_BCPV` (C defines 5) and keeps `S_BCPM` (undefined). Compiled at `-O0` both are right
  at both levels. A macro's value is the other way round: the program computes it at its own
  level, and R1 at the program's level is right on it (`M_BCPT` prints 5 at `-O0` and 9 at `-O2`,
  so the program at `-O2` takes the undefined branch).
- **The enumerator whose type moves when its enum completes** (the critic): `enum T { TA =
  2147483647, TB = TA + 1, TBIG = 0x100000000LL }`, `TB` undefined inside the body where `TA` is
  `int`, kept in silence by both prototypes, which re-ask `TA + 1` after the `}` where `TA` is
  `long`. A value-agreement guard (a probe that does not trap must equal the declaration's folded
  value) catches `TB`, `T3B`, `T4B` and the `-O2` drift, with 0 false alarms in 40; the reference
  spliced at the type the dump gives it, `((int)(TA))`, traps.
- **R1 as the lane built it** (lane `lane-land215`, `b4ae85f63`, not yet landed), attacked by the
  critic:
  - a probe that does not link passes in silence: a header constructor calling `zlibVersion` keeps
    the probe from linking with the runtime alone, and `K_BAD` (`INT_MIN / -1`) builds at exit 0
    printing a new value on each run, defect 2075's own symptom;
  - its `-E -dD` text is compiled again as `cpp-output`, which expands the kept `#define`s a second
    time: `#define K_W (K_W + 1)` over `enum { K_W = 2147483646 }` is refused as undefined where C
    gives 2147483647, and the repository's own `run/fixedbugs-361-...-own-c` probe does not compile
    and is counted judged;
  - a record accessor is never asked: `R_CALL` (`{ g_divmin(), 2 }`) builds, `-2147483648 2` at
    `-O0`, `0 2` at `-O2`;
  - an undefined operation inside `__builtin_constant_p`'s argument traps (`M_BCPO`, C defines 7):
    clang computes the argument under the checks at `-O0` too. 0 of panel 216's 41,114, 43,827 and
    42,086 bound real macros holds it.
- **`-fwrapv` on Windows** (defect 2114): the box's clang 23.1.1 adds `-fwrapv` for the MSVC target,
  where `1 << 31` is defined and today's region and every route keep it; clang 21 and 22 add none for
  that target (the critic, `-###`). Under `-fwrapv` R1's four checks still trap `INT_MAX + 1`,
  `-INT_MIN` and `65536 * 65536` and keep `1 << 31` alone; under `-fno-wrapv` the box agrees with
  the other three legs on every control, attack and real member measured. Four Windows SDK values
  build today and would be refused under `-fno-wrapv`: `VRL_ENABLE_KERNEL_BREAKS` (`winnt.h:23723`),
  `MI_FLAG_ADOPT` (`mi.h:607`), `D3DSTREAMSOURCE_INSTANCEDATA` (`d3d9types.h:800`), three macros,
  and `eLEXTYPE_PRIVATE20` (`sapi.h:6016`), an enumerator.

## Verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **adopt C2**; object A, B, C as framed, C2 at the program's level, C2 with `optnone`; **veto** an evaluator of the compiler's own | §1.1, §1.7, Part 5; §1.12 | about 300 to 450 code lines in `cli/` and `emit/`, unbuilt; cold only, one dump per asked unit behind a pre-filter (0 for the compiler's own 22 constants) | C2's verdicts unchanged on the CI's legs, its census traps nothing, the compiler's own cold build runs 0 dumps, at most 450 code lines | references followed; the probe at `-O0`; one dump per unit, ids matched within it; a two-sided canary; the message names the declaration that traps; lands with R1 |
| ffi-pragmatist | **approve C as built**; object A, B6, B7, Bf, K-init, C as written; **veto** Bp (§1.11, `EPOLLET`) | §1.11, §1.12, §4.19 | one syntax-sized clang run per unit holding such a constant, cold; `examples/curl` +10 % cold | `MS_NOUSER` refused on Debian 22 and Ubuntu 18 and nothing else of the 33,043; the control 28 and 28; on the box the control's 28 refused and `sapi.h:6016` named | inside R1's probe; rooted at the accessor, closure, predecessor rule, records leaf by leaf; a two-sided canary of the dump step; the member and its line named; **the constant question asked under `-fno-wrapv` on every target**, the program's compile untouched |
| completeness critic | the seats' choice held; R1 as built has four holes, two silent | — | — | — | C's probe at `-O0`, R1's at the level; the value-agreement guard; a probe that does not run refuses; plain `-E`; records asked; `-fno-wrapv` in the probe's words only; the reader tells a declaration from a `referencedDecl` stub |

## Disagreements, stated plainly

- **The level of the initialiser's probe.** The compiler-engineer compiles it at `-O0` always, the
  ffi-pragmatist at the program's level. **Measurement decides it**: the critic ran the
  ffi-pragmatist's own prototype at the program's `-O2` and it refused `S_BCPV` (defined) and kept
  `S_BCPM` (undefined). The initialiser's probe is compiled at `-O0`; R1's stays at the program's
  level, each right for the evaluator that computes its value.
- **`-fwrapv`.** The ffi-pragmatist asks the question under `-fno-wrapv` everywhere and names the
  turning point, an author's ruling that `-fwrapv` is Windows' meaning. The critic measured that
  R1's checks cannot tell that reading even if it were ruled (they trap three shapes `-fwrapv`
  defines), and that the reading belongs to a clang version, not to the target. The synthesis
  adopts `-fno-wrapv` in the probe's words and puts the reading to the author (R3).

## Resolution (provisional: author ratification pending)

**R1, the route for an initialiser's undefined value: C2, inside panel 216's R1.** For every
asked accessor of the program's own headers whose value, as the program's preprocessor expanded
it, holds an identifier or a brace list: one unfiltered `-ast-dump=json` of the probe's text per
asked unit, its `id`s matched within that one dump and a `referencedDecl` stub never read as a
declaration; from the accessor's own body, every `DeclRefExpr` to an `EnumConstantDecl` or an
initialised file-scope `VarDecl` followed transitively, an enumerator with no initialiser asking
its predecessor in its place (C11 § 6.7.2.2 p3), a record leaf by leaf at the leaf's converted type
(the accessor's own brace list and a const record object alike); each initialiser's text sliced
from the probe's text by the dump's offsets, every reference inside it spliced at the type the dump
gives the reference, and appended to the probe as one function per text, run by R1's resuming
probe. **Compiled and linked at `-O0` whatever the program's level**: under a `-O0` build it rides
R1's own probe, under `-O2` a second. **Every entry that does not trap must equal the
declaration's folded value byte for byte**; one that differs is refused with words saying its
initialiser computes another value outside its declaration, never *C leaves it undefined*. **A
two-sided canary of the dump step** rides every probe that asks one (an undefined enumerator
reached through a second must trap, a defined one must not), and a failing canary stops the build
as the compiler's own failure. A trapped entry is told `ffi_constant_type` on the constant's line,
naming the member whose initialiser traps and quoting its header line (the dump's
`presumedFile:presumedLine`). It never parses C: it slices clang's offsets and lets clang evaluate.
It lands with panel 216's R1, which it rides.

**R2, panel 216's R1 repaired before it lands** (the critic's four holes; R1 has not landed, so
they are its own repair, not defects of the trunk):

1. **A probe that cannot be read refuses the build.** A preprocess or a compile of the probe's text
   that fails, where the unit itself compiled, is the compiler's failure, exit 2 naming the step
   and clang's first error. A link that fails is told on every constant the probe asked,
   `ffi_constant_type`, naming the undefined symbol and that the header's own code reaches it
   before `main`. Nothing unread is counted judged.
2. **The probe compiles the plain `-E` text.** The `#define` line a refusal quotes is read from a
   second `-E -dD` run, made only when a refusal is told.
3. **Record accessors are asked**, as panel 216's R1 names records: a call leaf runs under the
   checks; a constant leaf is R1's (this sitting's) leaf by leaf.
4. **`-fno-wrapv` in the probe's words on every target**, never in the program's own compile (R3).

**R3, the meaning the probe asks is the C standard's, on every platform.** Defect 2114 closes by
R2's fourth item once R1 lands: `#define D_SHIFT (1 << 31)` is refused on Windows as on the three
other legs, and today's region, which IS the program's compile and cannot take the word, is
superseded there by R1 as everywhere. The reason: one program, one verdict (§1.12), and the
`-fwrapv` reading is clang 23's for the MSVC target and not clang 21's or 22's, so a verdict resting
on it would move with the compiler a machine has. The cost, measured: four Windows SDK values that
build today are refused, named above.

**R4, the questions the critic left, ruled provisionally.**

- **An undefined operation inside `__builtin_constant_p`'s argument** (`M_BCPO`; the region's older
  twin `M_BCPZ`): refused, as clang computes it under the checks; none of the populations holds it.
  The told words must be true of it: a trapped value is told as an expression that holds an
  operation C leaves undefined for the operands it is given, which a trap shows, and never as a
  claim that the value itself is undefined.
- **A value that names an undefined enumerator it never reads** (`sizeof(EA)`, `0 ? EA : 5`, with
  `EA = 1 << 31`): refused, through `EA`'s own trap, the message naming `EA` and its line; the
  header holds a constraint violation for its whole unit (the critic's reading, C11 § 6.6 p4, not
  a run).
- **A record constant whose leaf is a call builds** (`R_CALL_OK`, `{ g_ok(), 2 }`) where a scalar
  call is refused `ffi_not_constant`: the asserted constness of panel 216's R2 has no record twin.
  Filed apart as a defect, found beside this sitting.

**R5, the routes refused**, each with its measured reason: A (the defect); B6, B7, Bf (miss every
object, record and float cast on every clang; Bf narrower on clang 18); Bp (vetoed, refuses
`EPOLLET`); C as the brief framed it (misses 5 of 10 reached through a reference, folds a record);
C2 at the program's level and with `optnone` (refuse `S_BCPV`, keep `S_BCPM` at `-O2`); K-init
(keeps `MS_NOUSER` and `1 << 31` in every spelling, refuses `0.0f / 0.0f`); K23, C23's `constexpr`
(unknown to clang 18.1.3, the floor, and misses `1 << 31`); an evaluator of the compiler's own
(vetoed, §1.1, §1.11).

### The question to the author

**On Windows, is `1 << 31` the C standard's undefined value, or the defined one clang 23's MSVC
target gives it with `-fwrapv`?** The recommendation is **the standard's, on every platform**:
then one header value has one verdict wherever the program is built, and the verdict does not move
with the clang version a machine has (21 and 22 add no `-fwrapv` for that target, 23 does). It
refuses four SDK values that build on Windows today. The other answer is below.

### What the conservative resolution would have been, so the author can choose it

The probe under the target's own words, `-fwrapv` on the box: `1 << 31` and its three SDK macros and
one enumerator keep building on Windows and are refused elsewhere, a platform split recorded as
known in § 13's words; R1's checks would still refuse `INT_MAX + 1`, `-INT_MIN` and `65536 * 65536`
there, which `-fwrapv` defines, so the split would be partial and its words owed.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | C2's verdicts of `c4-O0.txt` and `c4-O2.txt` (35, 14 and 4 shapes) unchanged on the CI's legs; the census over 57 roots traps nothing; the compiler's own cold build runs 0 dumps; at most 450 code lines | the seat's scripts against the built compiler; a logging `clang` first on `PATH`; `layout`'s `code_lines` | R1 and C2's landing |
| ffi-pragmatist | `MS_NOUSER` refused on Debian 22 and Ubuntu 18 and nothing else of the 33,043; the control 28 refused and 28 built; on the box the control's 28 refused and `sapi.h:6016` named; no corpus program that builds today stops building | `pop.py`'s lists through `uxcorpus.py`'s generator; `ctlprog/`, `atk/`, `linux/ms/`; `run`, `emission` | R1 and C2's landing |
| completeness critic | with R2's repairs, `K_BAD` under its constructor refused, `K_W` builds and prints 2147483647, `run/fixedbugs-361` judged, `R_CALL` refused at both levels | the critic's `unread/`, `dd/` and `atk/` reproducers | R1's landing |

## Process notes

- **R1 was attacked before it landed, and that is where its holes were found**: the lane's gate had
  read `run 531/0` and counted an unread probe judged. The critic's reproducers become R2's cases.
- **Found beside the sitting**: the record call leaf (R4, filed); the fourth SDK name and clang 21
  and 22 adding no `-fwrapv` for the MSVC target (R3's cost and reason).
- **No historian sat**, so this file and its ratification carry no `## Prior art`
  (`.claude/rules/records.md` § And the record says whose idea it was).

## Author's verdict

Pending.
