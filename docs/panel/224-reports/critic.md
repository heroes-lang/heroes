# Panel 224, completeness critic, second pass: what is missing or contradicted

2026-10-17, from 18:56, after both seats reported (`compiler-engineer.md`, complete at 18:56;
`ffi-pragmatist.md`, 18:22) and both approved route E. I give no verdict. My copy is `$C` =
`.claude/worktrees/224-critic/` (`git -C <lane> archive 3c478500b | tar -x`, an empty `git init`),
with `.claude/worktrees/224-compiler-engineer/out/route-e.patch` applied by `patch -p1` (clean;
`shasum -a 256` begins `8b1a5844beebe443`, the copy read kept as `$C/out/route-e.patch.as-read`).
On the box I worked in `/c/w/224-critic/tree`, my first pass's tree with the same patch applied.

The compilers: on this Mac, the base is stage 1 from the seed (`sources 5f45d6514b22d650`) and E
was built by it (`./heroes build selfhost/main.hero -o heroes-e`, exit 0, `sources
789ddedd621bdb7b`, the compiler-engineer's key). On the box, the base is my first pass's
`heroes-next.exe` and E was built by it (exit 0, `sources 789ddedd621bdb7b`). Outputs are in
`$C/out/`. One capture, `box-prog.txt`, was first written to the session's scratchpad and moved
into `$C/out/` at once. No paid run, no credential printed. Nothing was written in the lane but
this file.

## 1. The ffi-pragmatist's four conditions on E, against the patch

| condition | what the patch does (`$C/selfhost/cli/units.hero`, `front_check.hero`) | checked by | holds? |
|---|---|---|---|
| (1) diagnostics only, the object under the target's own words | the pass is `-fsyntax-only` (`FRONT`); `line` gains `-c ... -o` only after it (`units.hero:199-212`) | reading | **holds** |
| (2) `-fno-wrapv` last, after the package words | `FRONT` follows the response file that carries every package word | reading | **holds by construction**, but **the threat it guards does not exist**: a package cannot carry `-fwrapv` (§ 3, first row) |
| (3) only the region's six read, every other diagnostic left to the unit's own compile | `front_check.refused` returns **any** clang failure of the pass, and `tu_object` returns it as the unit's refusal (`units.hero:199-205`) | reading, and § 2's run on the box | **does not hold**. Note also that a filter by warning **name** would not satisfy (3): the refusal in § 2 is `-Wshift-negative-value`, one of the six, at a header's own line. (3) needs the region's **lines** |
| (4) only units holding a region, keyed with the object | `key_text` is `""` for a unit without the region (the compiler-engineer measured 0 new `tu-` keys for `none.hero`); the pass runs after the cache check returns a served object (`deps.vouched` at `:149`, `return ok(object)` at `:170`, the pass at `:199`), so a warm build runs no pass | reading | **holds**. One premise is held by nothing: `holds()` looks for the region's first line by a spelling written again in `front_check.hero`, and no test reads `value_check.opened` (`grep -rn 'value_check.opened' selfhost/`: none). If the region is spelt otherwise, E switches off with every suite green on Mac, Linux and the CI's Windows clang 20, where the pass is redundant anyway |

The compiler-engineer's line that a pass refusal "is returned exactly as a compile's refusal is"
**holds in substance**. The patch calls `failure_code` and `failure_note` where the compile calls
`said_code` and `failure_text`, but both pairs return `e.code` and `e.msg`
(`units.hero:286-308`); they differ only in the type they take.

## 2. Can E's pass refuse a unit that compiles? Yes, measured, and one of its notes is false

**At the clang level** (`$C/out/div/`, ten shapes of header code, `flags()`'s words as written in
`flags.hero:130-151`):
- Only two warnings move with `-fwrapv`: `-Wshift-negative-value` (on by default) and
  `-Wshift-sign-overflow` (off by default). Under `flags()` alone, no shape gives an error on one
  side and not the other, on either clang. The shapes covered a missing return or an uninitialised
  read behind an overflow-dependent constant condition, a `switch`, a loop, a sign conversion, a
  file-scope array and a `_Static_assert`.
- **With `-Werror` among the words**, the used function `int f(void) { if ((-1 << 2) < 0) return
  1; }` gives no error under the box's compile, `[-Werror,-Wshift-negative-value]` under E's pass,
  and the same error under this Mac's own compile.
- No predefined macro differs under `-fno-wrapv` on the box (`clang -dM -E` both ways, 399 lines,
  `diff` empty).

**A package cannot bring `-Werror` or `-fwrapv`.** A package's compile words are limited to `-D`,
`-U`, `-I`, `-F`, `-isystem` and `-pthread` (`selfhost/cli/libraries.hero:162`, its message at
`:218`). So the only way to such a refusal is a header that makes one of the two warnings an error
itself. **End to end** (`$C/out/prog/`): `hdr.h` holds `#pragma clang diagnostic error
"-Wshift-negative-value"` and `static inline int hdr_twice(int x) { return x + (-1 << 2); }`.
`main.hero` binds `K_OK` (an integer, so its unit holds a region). `fl.hero` binds `K_F`, a float
of the same header code (no region). `control.hero` binds `K_OK` of a header with no code.

| program | this Mac, base | this Mac, E | box, base | box, E |
|---|---|---|---|---|
| `main.hero` | exit 1, `ffi_header_refused` | exit 1, `ffi_header_refused` | **exit 0, prints `7`** | **exit 1, `ffi_header_refused`** |
| `fl.hero` | exit 1, `ffi_header_refused` | exit 1, `ffi_header_refused` | exit 0, prints `1.5` | exit 0, prints `1.5` |
| `control.hero` | exit 0 | exit 0 | exit 0, `7` | exit 0, `7` |

**What E says on the box** (`$C/out/box-prog.txt`): the class, the line and the first two notes are
the Mac's. The third note is **false**: "*`hdr.h` compiles after `stdio.h`, a header of C's own it
uses and does not include: write a group naming `stdio.h` above this one ...*". `hdr.h` uses
nothing of `stdio.h`, and the Mac's message carries no such note (`grep -c stdio`: 0 in the Mac's
three outputs). The mechanism, **by reading, not run**: the note comes from
`header_order.hero:160-165`, whose trial `standard(...)` compiles the header after C's own headers
under `compiling.unit_words(under)` (`:272`). That is the target's words, so on the box the trial
passes because `-fwrapv` silences the warning, while the pass gave its verdict under `-fno-wrapv`.
The two stages ask under two meanings, and the reader takes the difference for a missing include.
A false message is `blocking` by `.claude/rules/verification.md` § Bounded discovery's class list.

**And a shape neither seat saw: 2120's class lives in the header's own compile too.**
`fl.hero` is refused on this Mac and builds on the box under the base **and under E**. The
header's own compile (`ffi_header_refused`) runs under the target's words, and E asks only units
that hold a region. So under E, on the box, the verdict on `hdr.h`'s code depends on whether the
program binds an integer constant. Its prevalence among real headers is unmeasured: it needs a
header that makes one of the two warnings an error itself.

## 3. Contradictions between the seats, and which side is checkable

- **"A package carrying `-fwrapv`"**: the ffi-pragmatist's § 3 ("*a package carrying one would undo
  A for its units in silence*") and the compiler-engineer's § 4 (the cheaper variant is "*not taken:
  a `-fwrapv` in a package's own words ... is not in that answer*") rest on it. **Checkable, and
  false**: a `.pc` of my own with `Cflags: -fwrapv` (`$C/out/pkg/`, `PKG_CONFIG_PATH`, `pkg-config
  --cflags fw` printing `-fwrapv`) is refused by base and E alike, exit 1, `error[ffi_package]: the
  package fw answered with -fwrapv, which this compiler does not pass on`. **So the cheaper variant
  is open again**: the pass only where the build's `clang -###` answer holds `"-fwrapv"`. On this
  Mac, Linux and the CI's Windows clang 20.1.8, E as built runs the compile's own front end under
  the same meaning a second time, since `-fno-wrapv` is already their default. Its cost and
  robustness against E as built are unmeasured; I give no reading.
- **The compiler-engineer's § 5**: "*Outside the region the word adds warnings and no error, and E
  refuses only on clang's error, so these shapes pass. Unrun: a package whose own words carry
  `-Werror`*". Now run: no package can carry it, a header's own pragma can, and E then refuses
  (§ 2).
- **B and F**: the ffi-pragmatist approves either "*as the order's repair beside E*" and reads them
  as reaching `both` and `mm`, unrun. The compiler-engineer objects to both as 2120's answer, and
  would approve only B's part that probes the units that compiled, filed apart for `mm`, reading
  `both` as needing a dedupe. **Neither is built**, so neither side is checkable as it stands.
- **By design or a defect**: the compiler-engineer's § 6 calls the Mac shapes "*the by-design
  order*" by an analogy he names as his own (design.md §4.17's sentence is written of `heroes
  check`'s stages). His § 8 files `mm` as an `adjacent` defect. The record's class rule reads them
  `adjacent` word for word ("*a mistake told only after the first is fixed*",
  `.claude/rules/verification.md` § Bounded discovery), and no document says a build's probe waits
  by design. Not checkable: a ruling.

## 4. The Mac shapes (`both`, `mm`, `missing`)

E moves none of them, on this Mac or the box (the compiler-engineer's tables; consistent with my
first pass). What is missing:

- **Only `mm` has a home.** The compiler-engineer files `mm` apart. `both` (the larger, in his
  words) and `missing` are filed by nobody, and the ffi-pragmatist files nothing.
- **`missing` is a third stage**: `ffi_unknown_name` is told from the header probes
  (`selfhost/cli/header_order.hero`, `name_order.hero`, `emit/ffi_reach.hero`,
  `emit/ffi_switched.hero`, `emit/ffi_cascade.hero` name it; by reading). Neither B nor F reaches it
  (the ffi-pragmatist's reading, unrun).
- **The question nobody answered**: is a build's later stage waiting for an earlier one by design,
  as §4.17 says of `heroes check`? Until a ruling, the three are `adjacent` by the class rule.

## 5. What the patch leaves for the gate

1. **No witness of E outside the box.** The CI's Windows clang 20.1.8 reads 2072 green with E or
   without (my first pass, its log). On this Mac and Linux the pass repeats the compile's front end
   under the same meaning. No package can bring `-fwrapv` (§ 3). So no CI leg can see E work or
   regress, and under the optimistic chain the box runs no leg before a push. The patch's test
   asserts its own strings (`FRONT[0]`, a hand-written region). A test that calls
   `front_check.refused` with `-fwrapv` in its line over a region's text and expects the two errors
   would witness E on every platform. None exists.
2. **`holds()`'s premise**, the region's first line, is held by no test that reads
   `value_check.opened` (§ 1, row 4).
3. **The box's suites under E, unrun**: `run`, `corpus` and the examples, `warnings`, `emission`.
   The compiler-engineer ran `unsupported` whole (328/0) and the compiler's own tests (1720) there.
   § 2 shows the pass can refuse what the box compiled; only the box's `run` and `corpus` can say
   whether a real program does.
4. **Cases**: no case holds the box's pair alone, and none holds § 2's header shape. Both read the
   same on this Mac with E or without, so neither is a falsifier off the box.
5. **Records**: the false note of § 2 if E lands as built (by the class rule, `blocking`); 2120's
   close; `mm`, `both` and `missing`; and the compiler-engineer's cheaper-variant paragraph, which
   rests on the falsified premise.
6. **Cost**: the compiler-engineer measured +0.03 s `real` per cold unit holding a region on this
   Mac, which the pass spends there with no verdict it can change (§ 3).

## The questions the sitting should have asked

- **Does the pass refuse whatever clang refuses under `-fno-wrapv`** (the patch), **or only the
  region's six at the region's lines** (the ffi-pragmatist's condition (3))? The two give different
  verdicts for `main.hero` on the box (§ 2).
- **Should the header's own compile and `header_order`'s trials ask under the pass's meaning?**
  Otherwise the false note of § 2 has nowhere else to come from, and `fl.hero` stays split between
  platforms.
- **Is the pass to run where the driver adds no `-fwrapv`?** The premise that answered *yes* is
  false (§ 3).
