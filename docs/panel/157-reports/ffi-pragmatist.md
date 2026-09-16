# Panel 157 — ffi-pragmatist report

- `verdict`: **object** on R1 as framed (the framing is too weak, and the
  measurement below shows why); **approve** R2, R3, R4 with the amendments here.
- `section`: design.md **§1.12** (robustness) first, then §1.11 and §4.19;
  CLAUDE.md § Precedence rank 3.
- `veto`: **NOT cast.** Nothing on the table breaks the C ABI. It is held in
  reserve against exactly one option, named in R2.

All measurements this session, Apple clang 21.0.0, arm64-apple-darwin25.6.0,
from a scratchpad copy of the tree; the repository working tree was not touched
except for this file.

## The question only this seat could settle: shape A is REAL and BINDABLE

Four header shapes, each compiled twice under the real flag list from
`selfhost/cli/flags.hero`:

| header | `struct node *` | `node *` |
|---|---|---|
| A `typedef struct { int64_t v; } node;` (ANONYMOUS) | **error** | ok |
| B `typedef struct node node;` | ok | ok |
| C `struct node { int64_t v; };` (tag only, no typedef) | ok | **error** |
| D `typedef struct node_s { int64_t v; } node;` | **error** | ok |

Shape A under `struct`, verbatim:

```
error: incompatible pointer types initializing 'struct node *' with an
expression of type 'node *' [-Werror,-Wincompatible-pointer-types]
```

Shape C under the bare word, verbatim:

```
error: must use 'struct' tag to refer to type 'node'
```

**And shape A binds through this compiler today.** I wrote `shapea.h` with an
anonymous-struct typedef and `shapea.hero` with `record Anode tag anode`:
`heroes build` exit 0, the program prints `7`; `--emit-c` exit 0 and its
artifact compiles at **exit 0, 0 errors** and prints `7`. Shape B likewise.

**Therefore no fixed spelling can be correct and the tag-learning round is
unavoidable.** The option set is NOT larger than the sitting thinks.

Two corrections the sitting should carry:

1. **Declaration proves nothing; use does.** Under default flags shape A accepts
   `struct node *p;` at exit 0 — it declares a *new* incomplete type. Panel 152's
   *`struct nosuchtype *p;` is legal C* is right and is about declarations. The
   round survives it because the round compiles USES, and only because
   `-Werror=incompatible-pointer-types` is in `flags.hero`'s list.
2. **The defect's scope is shape C alone.** A, B and D emit correct C on the
   first pass and their `--emit-c` artifacts compile clean, measured. The round
   therefore **does not fire needlessly on `typedef struct node node;`**: clang
   accepts `node *` there, so there is no refusal to read.

## R1 — what `--emit-c` owes: MORE than compilable C

### The loud half, reproduced from the tree

```
heroes build tests/golden/surface-fixtures/structtag/main.hero -o st.bin  # exit 0, prints 7
heroes build … --emit-c -o st.c                                            # exit 0
clang -c <flags.hero's list> -I runtime -I …/structtag st.c                # exit 1, 20 errors
```

First error **verbatim**:

```
st.c:30:56: error: must use 'struct' tag to refer to type 'probe'
   30 | _Static_assert(HERO_RET_RECORD(probe_open((int64_t)0), probe *), "heroes-ffi-return probe_open Probe");
      |                                                        ^
      |                                                        struct
```

20 errors, not the shared brief's 15; different clang or a per-module split. Same
class.

### The silent half — NOT in the brief, and it is the finding

`cli/pointee.hero` has **exactly one call site**: `cli/assemble.hero:62`, inside
the round (`grep -rn 'pointee\.check' selfhost/`, one hit). `cli/compile.hero:266`
returns before any round runs. So `--emit-c` never asks the header what an `@`
parameter points at.

```hero
extern "widen.h"                 # static inline void fill(uint64_t *n);
    function fill(@n: i32)       # WRONG: the header writes 8 bytes into a 4-byte slot

function main()
    guard: i64 @ 123456
    n: i32 @ 0
    fill(@n)
    print(n.to_str())
    print(guard)
```

| command | result |
|---|---|
| `heroes build widen.hero -o widen.bin` | **exit 1**, `error[ffi_parameter_type]`, fix (guess) `@n: u64` |
| `heroes build widen.hero --emit-c -o widen.c` | **exit 0** |
| `clang <flags.hero's list> widen.c runtime.c -o widen-by-hand` | **exit 0, 0 errors, 0 warnings** |
| `./widen-by-hand` | prints `-1431655766`, then **`2863311530`** |

`guard` was `123456`. It printed `2863311530` = `0xAAAAAAAA`, the bytes `fill`
wrote past the end of `n`. **The adjacent stack slot was overwritten.** design.md
§1.12: *a Heroes program must not segfault and must not corrupt memory.*

**Why clang cannot catch it, and this is the load-bearing sentence.**
`cli/pointee.hero`'s own module doc: *"It does not cast: the emitter passes
`(void *)&x` for the argument it has checked here … which is what makes a
correct binding compile clean under `-Werror=incompatible-pointer-types`."* The
emitted line is `fill((void *)&h0_n);`. That cast is sound **as a consequence of
a check that did not run**; under `--emit-c` it launders a wrong binding past
the one flag that would have seen it.

**This is not hypothetical and it is already in the repository.** Of the 32
cases in `tests/golden/fixedbugs/`, **25** emit at exit 0 and are refused by
`build` at exit 1. I compiled six of their artifacts:

| case | clang on the artifact |
|---|---|
| `ffi-pointee-opaque` | **exit 0, 0 errors** |
| `ffi-pointee-sign` | **exit 0, 0 errors** |
| `ffi-pointee-void` | **exit 0, 0 errors** |
| `ffi-pointee-width` | **exit 0, 0 errors** |
| `ffi-parameter-width` | exit 1, 2 errors |
| `fixedbugs-a-misspelled-handle-tag-stays-refused` | exit 1, 10 errors |

I then compiled **all 25**. The split is **20 loud / 5 silent**. The five whose
artifact clang accepts at exit 0 while `heroes build` refuses the program:

```
ffi-missing-link  ffi-pointee-opaque  ffi-pointee-sign  ffi-pointee-void  ffi-pointee-width
```

**Five cases the repository already carries produce `--emit-c` output that clang
accepts without a murmur and that the build refuses as unsound.** Four are the
pointee check; `ffi-missing-link` is the same class through the linker rather
than the compiler, which is why `-c` alone would not see it either.

**R1 answer.** `--emit-c` owes **the artifact of a build that passed**, which is
strictly more than compilable C — because the five rows above satisfy
*compilable* and are still wrong. `.claude/rules/cli-surface.md` (*"an output,
not a dump"*) and design.md:717 are right; neither sentence changes. Add one:
*an output is what the build would have used, so every check the build performs
has run before it is written.* If instead the sitting answers *the first
emission*, then `.claude/rules/cli-surface.md`'s sentence is the one that must
change to *`--emit-c` is a dump* — and I object to that, because §1.11 makes
`--emit-c` the route by which a C programmer audits a binding.

## R2 — the repair

**Run the round inside `--emit-c` and write the C the round accepted.** The
alternatives fail for named reasons:

- **A fixed `struct X *`**: shapes A and D are unbindable under it, measured.
  `typedef struct { float x, y; } Vector2;` (raylib) is shape A; glibc's `FILE`
  is D. **I would veto this option on design.md §1.11.**
- **Refuse when a tagged binding is present**: leaves all five silent
  cases open, and takes `--emit-c` away from exactly the programs §1.11 exists
  for.
- **Learn the qualifier without clang**: a C parser in the compiler, and it does
  nothing for the pointee half, which needs `-ast-dump=json` regardless —
  already implemented in `cli/header_types.hero`.

Cost note in this seat's favour: the round runs ONCE for shapes A, B and D, so
the common binding pays one clang invocation it was going to pay at build time
anyway.

## R3 — `suite_emission.hero`'s fourteen

**First, the number is stale.** The suite's premise says *"all fourteen emit —
measured"*. Measured today: `tests/golden/fixedbugs/` holds **32** `.hero` cases
and **25** of them emit-while-build-refuses. The suite's own floor comment
already warns about exactly this drift class.

**Second, the premise dies under the repair, and it should.** It is the same
premise that let `widen.hero` emit corrupting C at exit 0.

What replaces them, losing nothing they were written for: the 25 become
**build-refusal cases judged by their annotated diagnostic**. That is the shape
`.claude/rules/verification.md` already ratified for this exact situation —
*"The repair was to move the cases, not to loosen the suite"* — when four
checker-refused cases entered the same directory on 2026-09-09. What they
actually test (the emitter does not crash on a wrong binding, and the
diagnostic is right) is preserved by `--dump-ir`, which is a dump and owes
nothing, plus the existing annotation.

## R4 — the instrument gap

**Yes, and the five clean rows above prove the obvious instrument is not
enough.** Two assertions are needed, and the second is the one that matters:

1. **Compile the artifact.** Every `tests/golden/emit/` artifact through clang
   with `flags.hero`'s own list, exit 0 required. This catches the 20 errors.
   **It catches 20 of the 25 and NOT the five silent ones.**
2. **Agree with the build.** For the same program, `heroes build -o bin` and
   `heroes build --emit-c` + compile-by-hand must produce the **same exit code**
   and the same stdout. This is the only check that catches the silent five, and
   it is the assertion the repository is missing.

## Prediction (falsifiable)

**SQLite.** `typedef struct sqlite3 sqlite3;` is shape B. Under today's rule
`record Db tag sqlite3` emits `sqlite3 *`, clang accepts it on the FIRST
emission, **the round runs once and `struct_tags` stays empty**. So step 3 of
§4.19's ladder needs no shim and no second clang invocation — and would need one
under a fixed-`struct` rule. Falsified by any shape-B build that records a
second round.

**raylib.** `typedef struct { float x; float y; } Vector2;` is shape A, which I
built and ran through this compiler as `shapea.hero`. A fixed-`struct` rule
makes it **unbindable**. Falsified by compiling `struct Vector2 *v = …;` against
`raylib.h` at exit 0 with `flags.hero`'s flags.

## Condition

- I withdraw the objection on R1 if the sitting shows a `cli/pointee.hero` call
  site on the `--emit-c` path that I missed. One hit, session-measured.
- I move to **veto** if the resolution is *always write `struct X *`*, on §1.11:
  shapes A and D are not bindable under it, measured.
- I would accept *the first emission* as R1's answer only if `--emit-c` gained a
  loud refusal-or-warning whenever a check it skipped would have fired. Silence
  is what makes the five rows above a §1.12 defect rather than a documentation
  gap.

## UNRUN

- The `emission` suite under the repair. `./heroes run tests/harness/main.hero
  -- ./heroes emission`. Over the 60 s budget; the brief forbids the net.
- A real libm/SQLite/raylib link. Shapes A-D are the reduction; those headers
  are not on this box.
- Whether 20 vs the brief's 15 errors is a clang-version or module-split
  difference. `clang --version` on the box that produced 15 would settle it.
- Windows: not needed; the box was not touched.
