# Panel 224: a unit holding the value region is read by the front end under C's own meaning of a signed shift before it compiles

**Convened 2026-10-17 at about 16:55, synthesised the same evening**, on defect 2120
(https://github.com/heroes-lang/heroes/issues/2120), by the author's answer that afternoon,
*convene the two sittings now*, with M-bounded-seed's tag waiting on it; the Windows box on, by the
author. **Lane: soundness** (compiler-engineer, ffi-pragmatist, a completeness critic before the
seats and after them). **Resolution: provisional, author ratification pending.** Briefs:
`docs/panel/224-briefs/`, repaired after the critic's first pass
(`docs/panel/224-reports/critic-briefs.md`, ten repairs). Reports:
`docs/panel/224-reports/compiler-engineer.md`, `ffi-pragmatist.md`, `critic.md`. Every number below is
a seat's or the critic's, with its command in its report.

## The proposal, verbatim (the shared brief's question)

> How should every undefined value a program binds be told in its first build on every target?

On the Windows box, clang 23.1.1 for `x86_64-pc-windows-msvc` adds `-fwrapv` with
`-fms-compatibility`, the region `emit/value_check.hero` opens around a header constant's accessor is
silent on `-1 << 2` and `1 << 31`, and the unit's compile stops on the region's other refusals
before panel 216's probe, which asks under `-fno-wrapv`, runs. Routes A to D were the coordinator's;
E, E', F and G the critic's first pass.

## What was measured

- **The defect at `3c478500b` on the box** (the critic, both seats): `unsupported fixedbugs-2072`
  0 and 1, the output missing exactly `SHIFT_NEGATIVE` and `SHIFT_INTO_SIGN`; 1 and 0 on this Mac.
  The CI's Windows leg runs clang 20.1.8, which adds no `-fwrapv`, and reads the case green.
- **Route E, built** (compiler-engineer, `selfhost/cli/front_check.hero`, 63 lines, and 10 in
  `selfhost/cli/units.hero`; nothing in the lexer, checker, IR or emitter): before a unit holding the
  region compiles, `-fsyntax-only -fno-wrapv` after its own words; the object compiled under the
  target's. On the box the 2072 case 0/1 to 1/0 in one build, in the `.expected`'s words, the box's
  `unsupported` 328/0 against the base's 327/1; on this Mac `unsupported` 344/0, `run` 534/0, `cache`
  7/0, `layout` 6/0, the compiler's own tests 1720; a program binding no header value keeps every
  cache key (0 new objects) and its build time, a cold build of one that does pays +0.03 s here and
  0.31 to 0.35 s of a 12 to 16 s cold `windows.h` build on the box. The ffi-pragmatist read the same
  on the box's real unit, all six in the `.expected`'s words, `windows.h` 0 warnings under the pass.
- **E as built refuses a program the base builds** (the critic, on the box): a header that makes
  `-Wshift-negative-value` an error itself and holds its own `-1 << 2`, a program binding an integer
  constant of it: the base builds it and prints `7`; E refuses it, `ffi_header_refused`, exit 1, with
  a false note from `header_order.hero`'s trial compile, which asks under the target's words. The
  pass refused on any error it saw, where the ffi-pragmatist's condition 3 reads the region's six.
- **A** (both seats): one word, but every unit's and the runtime's key moves on four platforms, it
  reverses panel 218's R3 (ratified that morning: the word is the probe's, never the program's), and
  on clang 23 it changes what signed overflow means in the runtime's C and every header's inline
  code; on the box 6 `nsw` flags across 602 UCRT and SDK inline functions, their `-O2` code identical.
- **G** (ffi-pragmatist): `-fno-ms-compatibility` defines `__STDC__ 1` and `__GNUC__ 4`, and a
  `fstat` binding that builds and runs today (`fstat 0 size 718`) is refused with three errors.
- **A package cannot carry `-fwrapv`** (the critic): a `.pc` with `Cflags: -fwrapv` is refused by the
  base and E alike, `ffi_package`, its words limited to `-D`, `-U`, `-I`, `-F`, `-isystem`,
  `-pthread` (`libraries.hero:162`); both seats had leaned on that threat.
- **The order beyond Windows** (the critic, both seats re-run): on this Mac, a region refusal beside
  a value only the probe sees (`INT_MIN / -1`, a float out of an integer's range), the same pair in
  two modules, and a name the header lacks beside such a value each tell the first only, under the
  base and under E: the probe is asked only once every unit compiles (`assemble.hero:212`).

## Verdicts

| seat | verdict | condition |
|---|---|---|
| compiler-engineer | **approve E and adopt it**; object A, B (as 2120's resolution), C, D, E', F, G; no veto | the patch under 80 lines at its gate; the Mac shapes a separate order, `mm` filed `adjacent` |
| ffi-pragmatist | **approve E**; B and F beside it for the order; object A, C, D, E'; **veto G** | E reports only, the object under the target's words; `-fno-wrapv` last; only the region's six read; only units holding a region, keyed with their object |
| critic | no verdict | E as built breaks condition 3 and refuses a program the base builds, with a false note; nothing witnesses E outside the box; no test ties `holds` to `value_check.opened`; the `-###`-gated variant's stated reason is falsified |

## Resolution (provisional: author ratification pending)

**R1, route E, as condition 3 reads it.** A unit holding the value region is read once by clang's
front end, `-fsyntax-only -fno-wrapv` after its own words, before it compiles; **its verdict counts
only when every error it reports is a region error**, in the unit's own C file and naming one of
`undefined_words.UNDEFINED`; any other error, or a mix, and the pass gives no verdict and the unit's
own compile decides, as before. The object is compiled under the target's own words, so panel 218's
R3 stands, and a unit with no region reads nothing and keeps its key. A program the base builds is
never refused by the pass, which the critic's header shape is to show on the box at the landing
(built, printing `7`, as under the base).

**R2, witnesses.** A compiler test runs the pass with `-fwrapv` in the words over a unit holding the
region with `1 << 31` and `-1 << 2` and reads them refused, and the same unit's compile under
`-fwrapv` not, so E is witnessed on every platform and not on the box alone; and a test ties
`front_check.holds` to `value_check.opened`'s own output.

**R3, the pass runs on every target**, not only where the driver adds `-fwrapv`: the critic falsified
the reason the compiler-engineer gave against the `-###`-gated variant, and the sitting keeps the
ungated form on another one, that every CI leg then exercises the pass the box needs, at +0.03 s of a
cold build of a region unit on this Mac. **What conservative would have been**: the gated variant,
the pass asked only where the toolchain's identity shows `-fwrapv`, cheaper on three legs and
witnessed on none of them; the author can choose it.

**R4, refused**: A (218's R3, every key), G (a working binding refused), C (no rule fits a skip by a
machine's clang, and an open defect counts against a tag), D (no spelling found), E' (the emitter and
two meanings in one program), B and F as 2120's resolution. **Filed apart, `adjacent`**: the order
the critic found on every platform, a value only the probe sees waiting for every earlier refusal of
the build, a region's, another module's or a missing name's, one item for its one cause,
https://github.com/heroes-lang/heroes/issues/2128.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | E reads the 2072 case green on the CI's Windows leg on its first run with clang 23 or later, where the base reads it red; the patch under 80 lines by `wc -l` at its gate | the CI's Windows leg; `wc -l` | the first CI run with clang 23; the batch gate |
| ffi-pragmatist | with E landed, a box program binding `VRL_ENABLE_KERNEL_BREAKS` beside `MB_OK` is refused in its first build in clang's words, the 2072 case reads 1 passed with its `.expected` unchanged, and the SQLite objects on this Mac are byte-identical to a build without E | the box; `cmp` of the objects | the batch gate |

## Process notes

- The critic's first pass found the defect's class wider than its issue: the order holds on every
  platform, and 2120 is its `-fwrapv` case. Its second pass found the one way E as built made a
  correct program refused, which the repair closes.
- Copies inside the repository's root, `.claude/worktrees/224-<seat>/`; the compiler-engineer wrote
  outputs to the scratchpad first and moved them, and made a folder outside his own on the box, moved
  and removed, both said in his report.
- No historian sat; no prior art is claimed.

## Amended at the landing, 2026-10-17 evening

**R1 as built reads one word more than as written**, and the compiler-engineer measured why
(`docs/panel/224-reports/compiler-engineer.md` § 10): besides the region's six, the pass reports on
the 2072 unit the three `_Static_assert` errors the unit's own compile reports too, so a verdict
counting only where *every* error is the region's never counts on that unit. So the pass runs
**after** the unit's compile, and **its verdict counts only where every error it reports that the
compile did not is a region error**, at the unit's C file or a file one of its own `#line`s names,
naming one of `UNDEFINED`; any other new error and it gives no verdict, the compile deciding as
before; an object the pass refuses after a compile that succeeded is thrown away, never cached. The
pass can only add region errors. Measured on the final build (`sources 5fa340a90f75c5eb` on both
machines): the 2072 case 1/0 on this Mac and on the box; the critic's `hdr.h` program refused on this
Mac with the base's message and built on the box printing `7`, no false note; the compiler's own
tests 1723 on both; `unsupported` 344/0 here and 328/0 on the box; `run` 534/0 here. R2's two tests
landed, and two more: a header's own error alone and beside the region give no verdict.

**Still open, said**: the header's own compile keeps each machine's meaning, so the critic's
`fl.hero`, binding a float of a header whose own code holds a shift C leaves undefined, builds on the
box and is refused on this Mac, as before this sitting; and the compiler-engineer **withdraws his
objection to the `-###`-gated variant**, its reason falsified by the critic, which leaves R3's
choice, the pass on every target, standing on the witness it gives every CI leg.

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2130, naming the sitting as `panel 224`, with its question and the
recommendation.

**RATIFIED, 2026-10-17**, R1 to R4 as written above and as amended at the landing, the author answering in conversation that evening, meant as *I ratify everything*, on the coordinator's list of the five open decisions, each with its recommendation: route
E, its verdict counting where every error the pass adds is the region's, as landed at `32e517b9d`;
the pass runs on every target, the `-###`-gated variant recorded and not taken. Recorded as a reading
(CLAUDE.md § 4). The author may overturn it.
