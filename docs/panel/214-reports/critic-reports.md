# Panel 214, the completeness critic, pass 2: the reports

Written 2026-10-14 from 04:55 in the critic's copy `.claude/worktrees/scratch-panel-214/critic2`
(detached at `8c8783ac7`, compiler built from the seed with `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`). The compiler-engineer's appendix was extracted with `awk` and
applied with `git apply`; `git diff --stat` printed its +296/-11 in four files; `./heroes build
selfhost/main.hero -o build/heroes-oc` exited 0. Every finding names the command that settled it
and what it printed, or says *unrun*. Nothing was timed. The copy was removed at the end; its
`build/` paths are reproduced from the sources quoted below.

## F1. O+C's last probe gives a header less than the unit gives it, and blames a correct header (cgo 50710, reproduced in the prototype)

The historian warned about this shape: a probe that gives the header less context than the unit
gives it (cgo issue 50710). No seat ran it against the prototype. `whose.alone_refused` compiles
**each group's header alone** after the prefix. § 13 says *one that needs another's names comes
after it*, so a correct header that needs an earlier group's names is refused when compiled alone.

Run in `build/ab/`: `a.h` holds `typedef int a_t;` and `one()`; `b.h` holds `static inline a_t
two(void)`, which needs `a.h`, correctly; `c.h` holds `#pragma GCC poison int64_t` and `three()`.
`abc.hero` binds them in the order a, b, c. `ac.hero` binds a and c only.

| program | today (`./heroes`) | O+C (`build/heroes-oc`) |
|---|---|---|
| `ac` | exit 2, internal error | exit 1, `` `c.h`, the header this group names, does not compile ... `` (right) |
| `abc` | exit 2, internal error | **exit 1, `` `b.h`, the header this group names, does not compile ...``, at `abc.hero:4:8`** |

`b.h` is correct and `c.h` is the culprit. The prototype turns an honest exit 2 into a false message
at exit 1 that names the wrong group, which is the direction the brief weighs most. The census
could not see this, because no tracked program has the shape. A repair is to compile the headers
cumulatively, in group order, and name the first prefix that is refused (unrun). Until something
like it lands, the compiler-engineer's claim *"O must never preempt a class that already exists"*
holds, but *"0 of 47 author mistakes told as the compiler's, no compiler bug told as the author's"*
does not hold for the probe's own fallback.

## F2. Route C, as built, needs no spec sentence and does not misfire on the 26 headers. The veto and the adoption are compatible at 0 tokens

The compiler-engineer says C *"needs a spec sentence"*. The spec-warden vetoes every sentence that
reserves the prefixes, because 26 tracked headers outside `unsupported/` use them for names of
their own. **The C that was built is not a reservation.** `declared_after.kept` fires only after
clang has refused, on a first error in the program's C at a name of the compiler's forms, with the
first note in a group's header at that same name. A header clang accepts never reaches it.

- Reproduced the spec-warden's population: `git ls-files '*.h' | grep -v -E
  '^(runtime|seed|archive|docs)/'` gives 447. The headers holding a `hero_`/`Hero`/`HERO_`/`h_`
  name that `grep -r -w` does not find under `runtime/` number 34 by my criterion; the 26 outside
  `unsupported/` match exactly (12 `run`, 12 `check`, 2 `surface-fixtures`). The 29 programs naming
  those 26 were each built with `./heroes` and with `build/heroes-oc` (`build/cmp.sh`, digests and
  the `-o` name normalised). **29 compared, 0 moved**: 14 exit 0 with both (12 `run`, 2 fixtures),
  15 exit 1 with both (the 12 `check` cases are refused by the checker before clang, and 3
  fixtures).
- So C's home is a narrowing in `.claude/rules/c-boundary.md`, a thirteenth member beside the
  `declaration()` rule at line 133, plus a §4.17 message. The spec-warden's 0-token ruling and the
  compiler-engineer's C fit together. The one sentence to drop is the compiler-engineer's W9,
  *"the spec owes one sentence for C"*.
- **The sentence nobody priced.** The spec-warden priced prefix reservations (C1 to C3), B1, E1 and
  F1. The ergonomist's condition offers a different kind: a sentence that states the fact without
  reserving a prefix. Drafted after *one defining `_GNU_SOURCE` first.* by a script that asserts
  its anchor occurs once: *Its unit also holds the runtime's C names and, after it, `<stdint.h>`
  and `<math.h>`: a header declaring one of their names is refused, both declarations named.*
  Measured offline with `env -u ANTHROPIC_API_KEY ./heroes measure build/drafts/k1.md`: **cl100k
  7695 (+40), legacy 7562 (+43)**. Its real cost is unrun (a paid `--refresh`). It leaves the 26
  legal, since they declare no name the runtime holds. Whether it pays under §1.2 is the
  spec-warden's break-even, which is unmeasured either way.
- **C's list of forms is not the emitter's whole namespace.** The emitter also writes `f_<field>`
  for every record field and `t<N>` for temporaries (`./heroes build build/mac2/st4.hero --emit-c`:
  `int64_t f_errno;`, `t3 = (h_st4_Status){.f_errno = t1, ...}`). The compiler-engineer's
  measurement (*"every top-level name in the 605 emissions is in these forms"*) counted top-level
  names only. Run: a header `#define f_code 0` beside a program record with a field `code`
  (`build/mac2/fm.hero`) gives **exit 2, internal error, with today's compiler and with O+C alike**
  (`fm.c:79:13: error: expected member name`, note *expanded from macro 'f_code'* at `fm.h:1`).
  The shape is only planted: `grep -rhoE '^[[:space:]]*#[[:space:]]*define[[:space:]]+(f_[a-z0-9_]+|t[0-9]+)'`
  finds no such macro in the macOS SDK, in `/opt/homebrew/include` or in the Linux image's
  `/usr/include` (the same grep does find `h_addr`, so it works). The guard's 539 `push_macro`
  lines (`grep -c`) cover the runtime's words, C's names and `main`, but not `f_*` or `t<N>`.

## F3. O and I are not the same question, and each one misfires where the other does not

- **O** asks whether the refusal reproduces in the unit's opening without the program's C. It sees
  y15 (`#pragma GCC poison`) and the runtime and library collisions. It is blind to anything that
  needs the program's C: x1-style collisions (C covers those by form) and a header macro that
  rewrites a name the emitter writes (F2's `fm`: exit 2 under O+C).
- **I** asks whether the refusal disappears without the header. It would tell `fm` and x1 as the
  header's. But the ffi-pragmatist writes its stand-ins *"each spelled as the C the emitter already
  writes for it"*. A defect in how the emitter renders a binding, the compiler-engineer's NC2 shape,
  would then sit in both the stand-in and the program's C, so I would read *"needs the header"* and
  tell the header. **Whether I's stand-ins are independent of the code under suspicion is unrun.**
  NC2's verdict under I depends entirely on it. Under I, NC1 also reads *"needs the header"*.
- **Which one gives the header exactly the unit's context** (the historian's condition): O's
  opening is the unit's own prefix, region, guard and words (`order_trial.opening` under
  `compiling.unit_words`, per the appendix), so its *with* side is exact. Its weak side is F1's
  per-header fallback. I's *with* side is the unit as written. I's risk is not the header's context
  but how faithful the stand-ins are.
- **A contradiction one side has measured**: the compiler-engineer says *"stand-ins cannot exist for
  a record held by value or a constant whose value only the header knows"*. The ffi-pragmatist ran
  hand-written stand-ins in four tracked emissions: raylib's by-value records, a fixed-array field,
  SDL3's union written as a struct, and curl. Each exited 0 under `-fsyntax-only`, and all 8 cases
  exited 0 on three platforms (`ffi-pragmatist-work/route-i/`, `platforms/`). For a syntax-only probe
  the ffi-pragmatist's side is the measured one. Both cost figures (300 to 600 lines) are unrun.
- **So the option nobody listed is O and I together, decided by agreement.** The header's when both
  say so. The compiler's when O says *"not the opening"* and C does not claim the name. A
  disagreement stays at exit 2, quoting both probes. Unrun as a build.

## F4. The false blame every route shares (NC1) can be closed, at test time and at build time

The compiler-engineer: *"Every route shares this residual, and only the namespace test closes it."*
Two more instruments, both run:

- **A hygiene probe of the runtime against the C library**: the program unit's prefix and region
  (lines 1 to 48 of `tests/emission/examples-sqlite-main.c`), with 67 C11 and POSIX headers in the
  group's place (`assert` to `wctype`, `unistd`, `pthread`, `netdb`, `sys/socket`, `dlfcn` and the
  rest; `uchar.h` dropped, since the macOS SDK lacks it), compiled with the unit's `-W` words.
  **This Mac (Apple clang 21): exit 0, 0 warnings. The Linux arm64 image (Debian clang 22.1.8):
  exit 0, 0 warnings.** The same probe with NC1's runtime (a copy of `runtime/*.h` with `int
  abs(long x);` added after `heroes_runtime.h:278`) gives exit 1, `conflicting types for 'abs'`.
  It also covers what the namespace test exempts by design, the ten `<stdint.h>` typedefs the
  runtime declares from clang's macros. On a platform where they disagree with the libc, 2044's
  reader would tell the runtime's mismatch as the header's (an inference, unrun on such a
  platform). Windows: unrun.
- **At build time, a third probe in O's family**: the prefix plus only the C-library header that the
  first error lies in, with no group's header. With NC1's runtime it exits 1 (`_stdlib.h:148:6:
  error: conflicting types for 'abs'`); with the real runtime it exits 0 (`build/nc1/run.sh`).
  Refused with no group's header means the compiler's. The header bare with the C library and no
  runtime exits 0 either way, so E alone cannot place NC1. NC3, a defect in the region's own
  policy, stays the policy's question. Its witnesses are the `run` cases that must build.

## F5. `#line` already gives the compiler's synthetic C a name of its own; the program's lowered code cannot have one

Measured with `grep -o '#line [0-9]* "[^"]*"' tests/emission/*.c`, file names classified:
**174,801 to a `.hero`, 9,699 to `"<heroes library>"`, 4,711 restores to the unit's own `.c`**. In
`examples-sqlite-main.c` the include region (lines 2 to 48) stands before the first `#line`
(line 115), so a header's include stack names the unit's `.c`. Cgo's labelling therefore exists
here for synthetic code (the `.c`, which `group_file.compilers` counts as the compiler's) and for
library code (`<heroes library>`). Code lowered from the program's functions carries the `.hero`
name on purpose: the `lldb` suite pins those `#line`s (`.claude/rules/verification.md`'s row), so
relabelling it to remove B's misattribution would cost the debugger. B does not need it. It needs
`.hero` and `<heroes library>` counted as the compiler's, and **neither is today**:
`group_file.compilers` (lines 36 to 46, read) knows only the compiled file and the runtime's files.
That is a precondition the historian's adopted B omits.

## F6. The ergonomist's `#define fabs my_fabs`: not silent alone, silent when another group binds the name

Run in `build/mac/`. `mf.h` holds `static double my_fabs(double x)` returning 1000.0, then `#define
fabs my_fabs`, then `eightf()` calling `fabs(-8.0)`.

- `mg.hero` binds only `eightf`: exit 0, prints `1000.0`, which is the header's own meaning. The
  guard (`push_macro("fabs")` at `heroes_guard_open.h:173`) gives `fabs` back after the header, so
  `<math.h>` and the program's C are untouched. The runtime's float behaviour does not change: the
  emitted C calls no unbound `fabs` (`grep -o '\bfabs('` over `tests/emission/*.c`: 0).
- **`mf.hero` binds `eightf` from `mf.h` and `fabs` from a second group, `extern "math.h"`: build
  exit 0, `print(fabs(-3.0))` prints `1000.0`.** The unit re-pushes a bound name *"so it keeps the
  header's meaning"* (`heroes_guard_close.h`'s comment). It keeps the meaning of whichever header
  defined the macro, not of the group that binds the name. The result-type check passes because
  `my_fabs` returns `double`.

This is a silent wrong value at exit 0, the `blocking` class's first item (§ Bounded discovery). It
lies beside the sitting's question, since clang refuses nothing and no route A to K sees it. Searched
read-only: `gh issue list --search "macro another group's header in:title,body"` printed `[]`, and
`"push_macro bound name"` printed only issue 1690 (defect 361, closed). Whether it is filed is a
question, as far as those words reach. It is for the soundness seats.

## F7. The question the sitting should have asked

Is a header that is correct C, refused only because of names the compiler put in its unit, a wrong
program, and by which sentence? The spec is silent: § 13, read, never says the runtime or
`<stdint.h>`/`<math.h>` share the header's unit. CLAUDE.md § 12 says *spec beats compiler*. The
ergonomist answered for samples 1 and 5: *"right C inside a unit it did not choose"*. The
spec-warden answered: *"a header C refuses refuses the program"*, which assumes C refuses the header
in the unit's context and not alone. Today's compiler already tells these at exit 1, so no route
changes the exit code. What the ruling changes is whether `repair the header` is a true sentence, or
`this compiler's unit takes the name` would be. And whether F2's unpriced sentence (+40 cl100k) is
owed, which no seat decided.

## Smaller, checked

- The compiler-engineer's restated test (W4) is the one place where O+C tells a different party on
  purpose: a header declaring `h_femit_eight`, a name no program function is given, is now the
  header's. That is also C's only false-blame direction: an emitter that writes a stray `h_` name
  that a header happens to declare. Unrun as a case.
- H: the compiler-engineer counts 18 of 220 headers and the ffi-pragmatist 29 of 306 emissions (12
  under the variants). These are different instruments pointing the same way, and both refuse H. Not
  a contradiction that moves the ruling.
- The compiler-engineer's census (878 compared, 0 moved) and its 47 shapes: unrun by me.

## Ordered, most consequential first

1. **F1**: the prototype's `alone_refused` names a correct header (`b.h`, which needs `a.h` as § 13
   allows) when `c.h` is the culprit, at exit 1, where today says exit 2. This is cgo 50710's shape
   inside the route adopted. Repair (unrun): cumulative prefixes in group order.
2. **F2**: route C as built is a blame rule, not a reservation. 29 programs over the 26 headers:
   0 moved. The veto and the adoption fit at 0 tokens, and the compiler-engineer's *"C needs a spec
   sentence"* is the claim to drop. An unpriced fact sentence costs +40 cl100k. C's forms miss the
   emitter's `f_` and `t<N>`: `#define f_code 0` stays exit 2 under O+C (planted only; no system
   header defines one).
3. **F3**: O and I answer different questions. I's stand-ins, if written by the emitter, share an
   emitter defect (NC2) and blame the header. The compiler-engineer's *"stand-ins cannot exist"* is
   contradicted by the ffi-pragmatist's measured runs. Unlisted route: O and I by agreement, exit 2
   on disagreement.
4. **F6**: a header's macro rewrites a name **another group** binds: `fabs(-3.0)` from `extern
   "math.h"` prints `1000.0`, build exit 0. A silent wrong value outside every route. No issue
   found by the words searched.
5. **F4**: NC1 can be closed. A hygiene probe (prefix with 67 C/POSIX headers) exits 0 on macOS and
   Linux arm64 today and catches NC1's runtime; at build time, the prefix plus the failing C header
   with no group header tells NC1 as the compiler's. Windows unrun.
6. **F5**: `#line` already names synthetic C (the `.c`, 4,711) and library C (`<heroes library>`,
   9,699). Lowered program code must keep its `.hero` name for `lldb`. B needs `.hero` and
   `<heroes library>` counted as the compiler's, and `group_file.compilers` counts neither.
7. **F7**: the unasked question, whether a header that is correct C, refused only in the compiler's
   unit, is a wrong program under § 12.
