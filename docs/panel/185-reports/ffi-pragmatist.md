# Panel 185, the ffi-pragmatist's report

Seat: ffi-pragmatist. Tree: `03e70520`, copied to
`<scratchpad>/185-ffi-pragmatist/`, compiler built there from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 4.13 s).
Written as I go. Every `p185/...` path below is under that directory; Q1's
verdicts follow its experiments. This seat judges Q1, defect 145 and Q4's C
habit, as its brief asks, and not Q2, Q3 or Q5.

## Re-measured on my compiler before anything else (2026-10-02, 01:08)
- `probes/q1/macro.hero`: `check` exit 0; `build` exit 1,
  `error[ffi_unknown_name]: `sys/wait.h` declares no `WEXITSTATUS``.
- `probes/q1/shim/shim.hero` (`extern "wait_shim.h"`, a `static inline`
  wrapper): `heroes run` prints `1`, exit 0.
- `probes/q1/shim/shim-wrong.hero` (`status: i64`): exit 1,
  `error[ffi_parameter_type]`, *declared wider than the header's `int`*.
- The emitted C of `shim.hero` names the C function in three places: the
  result assertion `_Static_assert(HERO_RET_INT(hero_wexitstatus((int32_t)0)), ...)`,
  the probe `(void)(hero_wexitstatus)(a0);` inside the error pragmas, and the
  call `t2 = hero_wexitstatus(t1);`.

## Q1, the experiments

### Experiment 1: `WEXITSTATUS`, macOS SDK (Apple clang 21.0.0, arm64), the compiler's flags
Generator `p185/c/gen.sh` in my directory writes, per route, the C that route
emits: the result assertion, the probe inside the four error pragmas, and the
call site. `run.sh` compiles it `-fsyntax-only` with `selfhost/cli/flags.hero:93-108`.
`(1d)`'s shim is `probes/q1/shim/wait_shim.h`'s.

| declared `status:` | today | (1a) | (1a') | (1d) |
|---|---|---|---|---|
| `i32` (right) | refused, undeclared | accepted | accepted | accepted |
| `i64` | refused | **accepted** | **accepted** | refused, `-Wshorten-64-to-32` |
| `u8` | refused | **accepted** | **accepted** | accepted (C converts exactly, § 13's exception) |
| `i16` | refused | accepted | accepted | accepted (same exception) |
| `u32` | refused | **accepted** | **accepted** | refused, `-Wsign-conversion` |
| `f64`, `f32` | refused | **accepted** | **accepted** | refused, `-Wfloat-conversion` |
| `cstr`, `ptr` | refused | **accepted** | **accepted** | refused, `-Wint-conversion` |

The reason is in the header: `sys/wait.h:131`, `#define _W_INT(w) (*(int *)&(w))`.
The macro takes the ADDRESS of whatever it is handed and reads an `int` there,
so no declared type is ever converted and no conversion warning can fire.
The rvalue form today's result assertion uses, `WEXITSTATUS((int32_t)0)`, is
refused, *cannot take the address of an rvalue*; `(int32_t){0}` compiles
(the critic's finding, re-run).

**What the accepted wrong declarations do when run** (`p185/c/oob.c`,
`oob1a.c`, built with the compiler's flags plus `-fsanitize=address,undefined`,
which is `--sanitize`):
- `status: u8` under (1a')'s wrapper: `AddressSanitizer: stack-buffer-overflow
  ... READ of size 4 ... in hero_m_WEXITSTATUS`, exit 134. Under (1a)'s call
  site `t2 = WEXITSTATUS(t1)` with a `uint8_t t1`: the same overflow, exit 134.
  Without the sanitizer it reads three bytes beyond `t1`: an inference from
  the C, not run.
- `status: f64` under (1a'): exit 0, prints `0` for `256.0` where `1` is meant.

So on this Mac a declaration (1a) and (1a') accept compiles into an
out-of-bounds read: design.md §1.12's robustness goal, CLAUDE.md § Precedence
rank 3, broken at the boundary by a binding clang said nothing about.

### Experiment 2: `htonl`, macOS SDK
`sys/_endian.h:137`, `#define htonl(x) __DARWIN_OSSwapInt32(x)`, which is
`_OSByteOrder.h:70`, `(__builtin_constant_p(x) ? __DARWIN_OSSwapConstInt32(x) : _OSSwapInt32(x))`,
and `_OSSwapInt32` is an inline function taking `__uint32_t`
(`libkern/arm/_OSByteOrder.h:60`). So the macro DOES reach a typed parameter.
It does not help:

| declared `x:` | today | (1a) | (1a') | (1d), `static inline uint32_t hero_htonl(uint32_t x)` |
|---|---|---|---|---|
| `u32` (right) | refused, undeclared | accepted | accepted | accepted |
| `i32` | refused | **accepted** | **accepted** | refused, `-Wsign-conversion` |
| `i64`, `u64` | refused | **accepted** | **accepted** | refused, `-Wshorten-64-to-32` |
| `u16` | refused | accepted | accepted | accepted (exact) |
| `f64` | refused | **accepted** | **accepted** | refused, `-Wfloat-conversion` |
| `ptr` | refused, `-Wint-conversion` | refused | refused | refused |

**Why**: clang does not diagnose an implicit conversion whose context lies in
a macro expanded from a system header. Measured three ways on the `i64`
probe (`p185/c/h64.c`): zero diagnostics as emitted; zero with
`-Wsystem-headers` added; and the SAME code preprocessed first (`clang -E -P`,
then compiled) gives `-Wshorten-64-to-32` twice, the probe and the call site.
So the only parameter errors (1a) and (1a') keep for a system macro are the
ones that are errors by default (pointer to integer), never a width, a sign or
a float. That falsifies the hopeful reading of (1a), *a macro expanding into a
typed function inherits that function's checks*: on macOS it does not.

### Experiment 3: `FD_ZERO`, `FD_SET`, `FD_ISSET`, macOS SDK (run again at 01:45 after the resume)
Today the three shapes I tried for an `fd_set` stop at `FD_ZERO`,
`ffi_unknown_name` (`p185/fd/handle.hero`, `record FdSet tag fd_set`;
`ptr.hero`, `set: ptr`; `partial.hero` is refused earlier, `empty_record`).
On macOS `FD_SET` and `FD_ISSET` expand to typed inline functions
(`sys/_types/_fd_def.h:106-108`) and `FD_ZERO` to
`__builtin_bzero(p, sizeof(*(p)))` (`:115`).

| declaration | (1a) | (1a') | (1d), the program's own `static inline` |
|---|---|---|---|
| `FD_SET(fd: i32, set: FdSet)` (right) | accepted | accepted | accepted |
| `fd: i64`, `fd: u32`, `fd: f64` | **accepted** | **accepted** | refused (`-Wshorten-64-to-32`, `-Wsign-conversion`, `-Wfloat-conversion`) |
| `FD_ISSET(fd: i64, ...)` | **accepted** | **accepted** | refused, `-Wshorten-64-to-32` |
| `FD_ZERO(set: ptr)` | accepted | accepted | accepted (§ 13's named hole, what a `ptr` points at) |

The last row is accepted by all three and means different things.
`p185/c/fdzero.c`, the buffer filled with `0xff` first as a set reused between
two `select` calls is, built with the compiler's flags and `--sanitize`'s,
zero diagnostics: **(1a')'s `FD_ZERO` through a `void *` clears one byte**
(`sizeof(void)` is 1 in GNU C) and `FD_ISSET(40)` answers `256` where `0` is
meant; (1d)'s shim, whose parameter the author wrote as `fd_set *`, clears all
of it and answers `0`. Under (1a) and (1a') the macro's meaning depends on the
static type of its argument, and the only place that type is stated is the
Heroes declaration the macro cannot check.

### Experiment 4: `errno`, and a defect beside defect 143 (run again at 01:45)
`errno` is an object-like macro over a call (`sys/errno.h:81`,
`#define errno (*__error())`). No route of Q1 reaches it: (1a) and (1a')
start from an `undeclared identifier`, and `errno` is declared. Measured:

| `p185/errno/` | declaration | exit | what the author is told |
|---|---|---|---|
| `const.hero` | `constant errno: i32` | 1 | `ffi_not_constant`, true |
| `fn.hero` | `function errno() -> i32` | **2** | *internal error: compiling the generated C failed*, *called object type 'int' is not a function or function pointer* |
| `errno1.hero` | `function errno(n: i32) -> i32` | **2** | the same |
| `stdin.hero` | `function stdin() -> ptr` (`stdio.h`, a macro over `__stdinp`) | **2** | the same, `'FILE *'` |
| `optarg.hero`, `optarg1.hero` | `function optarg() -> cstr` (`unistd.h`, a plain variable, no macro) | **2** | the same, `'char *'` |
| `optargc.hero` | `constant optarg: ptr` | 1 | `ffi_not_constant`, true |
| `shim.hero` + `errno_shim.h` | `function hero_errno() -> i32` over `static inline int hero_errno(void) { return errno; }` | 0 | prints `-1` then `2` after `chdir("/no/such/dir")`: ENOENT |

**A C object declared as an extern `function` is exit 2**, the compiler
blaming itself for the author's declaration, with or without parameters and
macro or not. That is `.claude/rules/c-boundary.md` § *A clang failure that
the author's own extern caused*, whose five members do not include it; the
same names declared `constant` are already a clean exit 1. Not in
`docs/work/DEFECTS.md` (grep for *called object*, *not a function or function
pointer*, *errno* over it, `docs/records/done/`, `docs/panel/` and
`docs/records/log/`: nothing). **For the coordinator to file**; I edit nothing
but this report. It is Q1's neighbour, not Q1: whichever route the sitting
adopts, the message for a name the header has as an object should be exit 1
and should name (1d)'s shape, because (1d) is the only route that reaches
`errno`, `stdin` or `optarg` at all.

### Experiment 5: `isdigit`, `signbit`, `isnan`, macOS (run after the resume at 01:44)
- `isdigit(c: i32)` binds and runs today (`p185/lib/isdigit.hero` prints 1);
  `c: i64` is `ffi_parameter_type`. It is a function on macOS and glibc, so no
  route touches it while the first round stays first.
- `signbit` is macro-only on both platforms (`math.h:194` here, *did you
  mean `__signbit`* on glibc). On macOS it dispatches on `sizeof(x)` with
  explicit casts, so (1a) and (1a') accept `x: i64` and `x: i32` and refuse
  only a pointer; (1d)'s `static inline int hero_signbit(double x)` refuses
  `i64` (`-Wimplicit-int-float-conversion`) and `f32` (`-Wdouble-promotion`,
  panel 092's own rule). `isnan(x: i64)` under (1a') on macOS: zero
  diagnostics, and `isnan` of an `i64` holding NaN's bits prints `0`
  (`p185/c/sb.c`).

### Experiment 6: the same matrix on Linux x86-64 (`heroes-linux`, Debian clang 22.1.8, glibc 2.41; one container, `docker ps` empty before; output `p185/c/linux.out`)

| binding | macOS under (1a) | Linux under (1a) |
|---|---|---|
| `WEXITSTATUS`, `WIFEXITED`, `WTERMSIG` | accepts every type tried | accepts `i64`, `u8`, `u32`; refuses `f64`, `cstr` (*invalid operands*) |
| `htonl`, `ntohl` | macro-only: accepts `i32`, `i64`, `f64` | **a function**: first round, refuses `i32`, `i64`, `f64` today, at `-O0` and `-O2` |
| `FD_SET(fd: i64, ...)` | accepted | accepted |
| `FD_SET(..., set: ptr)` | accepted | refused, *member reference base type 'void'* |
| `FD_ZERO` | an expression | **a statement**, `do { ... } while (0)` (`bits/select.h:25-31`): (1a)'s result assertion and `(void)FD_ZERO(a0)` are *expected expression*; a wrapper with no assertion through the macro compiles (`p185/c/fdz.c`) |
| `signbit(x: i64)` | accepted | refused, *floating point classification requires ...* |
| `isnan` | macro-only | **also a function**, `int isnan(double)`: binds today |

(1d) gives the SAME verdict on both platforms in every row I ran, because the
probe meets the shim's prototype, which the author wrote once.

**So under (1a) or (1a') one `.hero` file is refused on one platform and
accepted on the other**, by measurement: `function htonl(x: i64) -> u32`
(refused on Linux by the first round, accepted on macOS by the second);
`function WEXITSTATUS(status: f64) -> i64` and `function signbit(x: i64) ->
i32` (refused on Linux, accepted on macOS). In each case the platform that
accepts it is the one the author develops on, and the platform that runs the
leak leg (`.claude/rules/c-boundary.md`, CL-055) is the one that refuses.

### The 206 that emit today: does any route move them?
`p185/q1/names.tsv`: the 206 files with exit 0 in `probes/q1/extern-emit.tsv`
bind **526** distinct `(header, function)` pairs. Each preprocessed with
`#ifdef NAME` after its header (the file's own directory and `runtime/` on the
include path, `pkg-config --cflags raylib`): 524 preprocess, 2 do not, and
**one is also a macro on this Mac, `curl/curl.h`'s `curl_easy_setopt`**, at
`-O0` and at `-O2` alike (`p185/q1/macro-names*.txt`). It binds today through
the first round, `(curl_easy_setopt)(...)`, which reaches the function.
So: a route that asks the FIRST round first moves none of the 206; that is
an inference from the route's definition, the census of the code itself
being the compiler-engineer's to run. A route that asks *is it a macro*
first (an `#ifdef` before the parenthesised probe) moves `curl_easy_setopt`
to the weaker form, and `memset` with it wherever `_FORTIFY_SOURCE` is on
(panel 092). The ordering is load-bearing, not hypothetical.

**Today's compiler on Linux x86-64** (built from the seed inside the
container, `p185/linux-build.out`): `q1/macro.hero` exit 1 `ffi_unknown_name`;
`fd/handle.hero` and `fd/ptr.hero` exit 1 `ffi_unknown_name` at `FD_ZERO`;
`errno/fn.hero` **exit 2**, *internal error ... called object type 'int'*,
as on macOS; `lib/signbit.hero` exit 1 `ffi_unknown_name`; `lib/isdigit.hero`
exit 0; **(1d)'s `shim.hero` runs and prints `1`**, exit 0.

### Experiment 7: does (1d) compose across modules? (`p185/mod/`, run after the resume at 01:44)
`main.hero` does `use os/wait`; `os/wait.hero` binds `hero_wexitstatus` from
a shim kept beside it, `os/wait_shim.h`. Written `extern "wait_shim.h"`: exit
1, `ffi_missing_header`. Written `extern "os/wait_shim.h"`: runs, prints `1`.
A header is found from the directory of the file you compile, which is spec
§ 1's rule for `use` (*every `use` starts at the directory of the file you
compile*), so a shim moves with its module exactly as the module's own `use`
lines do. (1d) adds no relocation cost a library module does not already pay.

## Defect 145: the no-evidence shape, and whose round it rides

**The critic's query is not yet the right one.** Over every name
`tests/golden/unsupported/fixedbugs-145-types.h` declares, plus one function
(`p185/c/`, the compiler's flags, run after the resume):

| name | what it is | `(void)sizeof(NAME);` | `typedef NAME hero_q;` |
|---|---|---|---|
| `anon_s`, `anon_u`, `tagged_t` | typedef of a struct or union | compiles | compiles |
| `opaque_t` | typedef of an opaque struct | *incomplete type* | compiles |
| `void_t` | typedef of `void` | compiles (GNU `sizeof(void)`) | compiles |
| `only_tag` | a real tag, no typedef | *undeclared identifier* | *must use 'struct' tag* |
| `only_tagg` | nothing (misspelled) | *undeclared identifier* | *unknown type name* |
| `use_anon_s` | a FUNCTION | **compiles** (GNU `sizeof` of a function) | *unknown type name* |

`sizeof` would tell an author who wrote a function's name after `tag` that it
is a typedef, and cannot tell a real tag from a misspelled one.
`typedef NAME hero_q;` gives three distinct answers, a type name, a tag,
neither, and none of them false on these eight. **I recommend the typedef
form**, its one answer per shape pinned by the existing 145 goldens plus a
function-name case.

**It belongs to the same run as Q1's question, IF Q1 takes (1b).** Under
(1b) Q1 needs one fact the first round does not give, *is the undeclared name
a macro?*, and only to write a message on a build that already fails. 145
needs one fact, *is the name a type?*, for the same reason. Both are a
question about a NAME asked of the group's header after round 1 has refused,
and both change only a message. Measured: one TU, `p185/c/query.c`, the
headers then one `#line 1 "hero-ask-<kind> <name>"` per question
(`#ifdef NAME` / `#warning yes` for a macro, `typedef NAME hero_q_N;` for a
type), compiled once with `-ferror-limit=0`, answers all six questions, each
diagnostic carrying its own question in the file name: `WEXITSTATUS` and
`errno` macros, `nosuchname` not, `anon_s` a type, `only_tagg` and
`use_anon_s` not. **0.02 s real, 0.01 user**, three runs, load 5.25, so the
ratio is sound. Paid only by a build that has already failed.

**Under (1a) they do not share a run.** (1a)'s second round changes the
EMISSION (a different probe and call for the macro names) and must re-run the
compile, which is `selfhost/cli/assemble.hero`'s `Round` machinery, the
struct-tag round's; 145's query changes no byte of emitted C. They would
share a trigger point and nothing else, so 145 should not wait on Q1 there.

**Correction to experiment 4's last sentence, added 01:51.** design.md §4.19
(`:2318-2325`, panel 038) refuses a C object as a `constant` *as a language
rule*, naming `errno` and `stdout`: an accessor over either *returns whatever
it holds at the time of the call*. That ruling is about the `constant` member
(`docs/panel/038-constants-from-headers.md:234-236`); neither it nor any
sentence I found rules on reaching an object through a FUNCTION, which is
what `hero_errno()` is (a C function returning state, as `time()` does).
So whether the exit-2 defect's message should offer (1d)'s shape for `errno`
is a question for the sitting, not the premise my sentence made it. The
defect itself (exit 2 where the class is the author's) stands on either
answer.

### The route nobody listed: (1e), the second round's probe compiled PREPROCESSED
Experiment 2 showed `clang -E` brings `htonl`'s width check back. Measured on
the brief's bindings, macOS, (1a)'s probe preprocessed then compiled:
`htonl(i64)` refused, `htonl(i32)` refused, `FD_SET(fd: i64)` refused; but
`WEXITSTATUS(u8)` (the overflow), `WEXITSTATUS(f64)`, `WEXITSTATUS(i64)`,
`signbit(i64)` and `FD_ZERO(set: ptr)` (one byte cleared) all **accepted**.
It recovers a check only where the macro forwards to a typed function, and
the type-blind macros, which are the unsafe ones, stay open. Not a route that
closes Q1; listed so the option set holds it (CL-057).

## Q1 verdicts

**The five refused stay refused, measured.** The names behind the five
`ffi_unknown_name` files of `probes/q1/extern-emit.tsv` (`_IO_FILE`, `_popen`,
`sqlite3_openn`, `addrinfoo`, `describe`), each `#ifdef`-tested after its
header on this Mac: none is a macro. So (1a) never gives them a second round
and (1b)'s macro question answers *no*, leaving their message as it is today.

### (1a), a second clang round with a call-form probe: **veto**
- **section**: design.md §1.12 (`:571-590`: *where two admissible forms
  disagree and one of them can be made to crash, the other wins, whatever it
  costs*); §4.19 and §3.1 (`:645`: *clang verifies every `extern` signature
  against the real header*); spec § 13 (*a parameter ... declared at the
  header's own width and sign ... one that disagrees is refused*).
- **experiment**: `p185/c/gen.sh`, the C (1a) emits (result assertion with
  `(T){0}`, `(void)NAME(a0)` probe inside the four error pragmas, the call
  site), compiled with the compiler's flags against the macOS SDK and glibc
  2.41. On macOS it accepts `WEXITSTATUS` declared `i64`, `u8`, `u32`, `f64`,
  `f32`, `cstr` and `ptr`; `htonl` declared `i32`, `i64`, `u64`, `f64`;
  `FD_SET(fd: i64)`; `signbit(x: i64)`. Run: `status: u8` is an ASan
  stack-buffer-overflow, READ of size 4, exit 134; `status: f64` prints 0 for
  1; `FD_ZERO(set: ptr)` clears one byte of 128. On glibc `FD_ZERO` is a
  statement and (1a)'s assertion and probe do not compile at all.
- **argument**: (1a) is not a weaker check, it is no check: the parameter
  verdicts it gives come from whatever the expansion happens to do, which on
  macOS is a type pun or a system-header macro whose conversions clang does
  not diagnose. It admits a declaration that compiles to an out-of-bounds read
  on the platform the author develops on, and refuses the same file on Linux.
  A spec sentence saying *unchecked* is honest about it and makes the boundary
  §1.12 calls defended a place where a model's wrong width compiles and reads
  memory it does not own. That is a soundness refusal, so a veto, not a price.
- **cost**: compiler lines unrun (the compiler-engineer's); one § 13 sentence
  on the FFI floor, priced by the spec-warden; programs moved: none of the 206,
  an inference from the route's definition (round 1 first), since only one of
  their 526 names is also a macro here and round 1 reaches it.
- **prediction**: on a (1a) build, `function WEXITSTATUS(status: u8) -> i64`
  in `extern "sys/wait.h"` builds on macOS at exit 0 and its `--sanitize`
  run aborts with `stack-buffer-overflow`; and `function htonl(x: i64) -> u32`
  in `extern "arpa/inet.h"` builds on macOS and is `ffi_parameter_type` on
  Linux x86-64. Checkable the hour the compiler-engineer's (1a) builds.
- **condition**: a (1a) that refuses every declaration (1d) refuses in
  experiments 1, 2, 3, 5 and 6, on both platforms. (1e) is the nearest I
  found and it does not.

### (1a'), the compiler writes the wrapper from the declaration: **veto**
- **section**: as (1a).
- **experiment**: the same matrix. Its column equals (1a)'s on every row the
  generator ran, macOS and Linux, because the wrapper's parameter types ARE the declaration:
  there is no second statement of the type for clang to hold it against. The
  overflow above was measured through (1a')'s own wrapper.
- **argument**: the critic's C2 is right that it checks what (1a) checks;
  that is the objection. It improves the emitted C (the macro sits in one
  static function, the call site calls a real one, a statement macro such as
  glibc's `FD_ZERO` compiles in the wrapper, `p185/c/fdz.c`), which is worth
  having and does not touch the hole. The one row where a real (1a') could
  differ is a statement macro with a `()` result: dropping the assertion
  through the macro lets glibc's `FD_ZERO` wrapper compile where (1a) cannot,
  a gain in reach and none in checking.
- **condition**: as (1a).

### (1b), refused, the message naming a repair, design.md corrected: **approve**
- **section**: design.md §4.17 (a diagnostic carries what is needed to fix
  the program), §4.19, §1.12; `.claude/rules/c-boundary.md` § *A clang failure
  that the author's own extern caused*, whose third member it sharpens.
- **experiment**: the message needs one fact, *is it a macro?*, on a build
  that has already failed: `p185/c/query.c` answers it with 145's question in
  one clang run, 0.02 s. The repair it names is (1d), measured below.
- **what it tells the author**, a draft: *`sys/wait.h` has `WEXITSTATUS` only
  as a macro, and clang checks the parameters of a function, never of a
  macro*; note: *bind a function of your own: in a header of this program's,
  `static inline <C result> hero_wexitstatus(<C type> status) { return
  WEXITSTATUS(status); }`, with the C types the macro's documentation gives,
  then `extern` that header; clang checks the declaration against your
  function (§ 13)*. **It must not draft the C types from the declaration**:
  that reproduces (1a') in the author's file, and the `u8` overflow with it.
  No `Fix`: a new file is not a span.
- **cost**: 0 spec tokens (the spec names no macro); four design.md
  sentences corrected (`:565`, `:645`, `:2265-2266`, `:2401-2402`, the last
  being *not for every macro*); programs moved: none, measured for the five.
  Compiler lines unrun. Is it a new diagnostic class (CLAUDE.md § 4)? I would
  keep the code `ffi_unknown_name` only if its first sentence stays true, and
  *declares no `WEXITSTATUS`* is false of a header that defines it: a new code
  is the honest one, which is the sitting's to name.
- **prediction**: on a (1b) build, `build --emit-c` over the 412 files of
  `probes/q1/extern-emit.tsv` gives the same 206 and 206, the five refused
  with today's message text, and C byte-identical to `03e70520`'s for the 206.
- **condition**: a binding the brief's list names that (1d) cannot reach. I
  found none: every row of experiments 1 to 6 compiles through a shim in the
  generator's C, and `WEXITSTATUS`, the `FD_*` family, `htonl` and `errno`
  run through today's compiler on both platforms (below); `signbit` and
  `isnan` were compiled, not run, through the compiler.

### (1d), a `static inline` in a header of the program's own: **approve, as (1b)'s named repair**
- **experiment**: `probes/q1/shim/` runs on macOS and on Linux x86-64
  (today's compiler built in the container) and prints 1; the probe holds the
  declaration against the shim's prototype, so every wrong width, sign, float
  and pointer in my matrix is refused, the same on both platforms; it composes
  across modules by spec § 1's `use` rule (experiment 7); it needs no
  `heroes cc`, because nothing is compiled separately, which leaves panel
  036's undecided `.c` shim question where it is.
- **what it cannot check**: the C the author writes. A shim typed
  `unsigned char status` would read out of bounds as (1a) does. The guarantee
  ends at the shim's prototype, which the author wrote from the macro's
  documentation, in C, where a C reader looks for it.

### (1c), a mark such as `macro function`: **object**
- **section**: §4.19, spec § 13.
- **argument**: in my matrix it changes no verdict; it makes visible that a
  parameter is unchecked, and a model writing `macro function
  WEXITSTATUS(status: u8)` meets nothing that refuses it. A word whose
  content is *nothing is checked here* is a surface cost for no refusal. With
  (1a) it inherits (1a)'s veto.

### The robust route and the conservative one
Here they coincide: (1b) with (1d) as its repair. What a more COMPLETE route
would look like, so the author can choose it later: the group member states
the C prototype the documentation gives (a C type spelling beside the Heroes
one), and the compiler writes (1d)'s wrapper from THAT. Its C is (1d)'s byte
for byte, so it checks what (1d) checks by construction; it is unrun, and it
costs a C-type spelling inside the Heroes surface, a sitting of its own.

**(1d) through the real compiler for the rest of the list** (`p185/net/`):
one shim header, `net_shim.h`, with `hero_fd_new` (`calloc`), `hero_fd_free`,
`hero_fd_zero`, `hero_fd_set`, `hero_fd_isset` and `hero_htonl`, bound as
`record FdSet tag fd_set` with `acquires hero_fd_free` and `consumes`. macOS:
prints `8`, `0`, `16777216`, exit 0, and the same under `--sanitize`; Linux
x86-64 (built from the seed in the container) under `--sanitize`, where
LeakSanitizer exists: `1`, `0`, `16777216`, exit 0. `fd: i64` is
`ffi_parameter_type` on both. (`FD_ISSET` answers 8 on one libc and 1 on the
other, C's *nonzero*; a program compares it with `!= 0`.) So (1d) reaches
`WEXITSTATUS`, the `FD_*` family, `htonl` and `errno` through today's
compiler on both platforms, with the handle's life checked by the marks
§ 13 already has.

## Q4, where a C or shell habit is the reading

**The counts** (run after the resume, the regexes in the commands of this
seat's session):

| corpus | a sign written against its literal | a sign set apart by a space |
|---|---|---|
| C: 18,998 `.c`/`.h` files under `/opt/homebrew` and the macOS SDK's `usr/include`, `case` labels | 17 `case -N` | **0** `case - N` |
| C, the same files, every unary position (after `=`, `(`, `,`, `return`) | 12,716 | **0** |
| shell: 134 scripts (`/opt/homebrew`, `/usr/bin`, `/usr/sbin`, `/usr/libexec`), `case` arms opening with a dash | 153 (`-h)`, `-v|--verbose)`) | **0** |
| Heroes: the 1,530 tracked `.hero` files, arms opening with a signed integer | 16 | **1**, `tests/golden/check/fixedbugs-a-match-arm-opened-by-a-spaced-minus.hero`, the defect's own case |
| Rust crates in `~/.cargo/registry` (295 files) and `archive/bootstrap-rs` | 0 | 0, too few arms to say anything |
| Markdown: the 2,770 tracked `.md` files, lines opening with a dash then a digit | 3 `-N` | **46** `- N`: 43 bullets outside a fence, 3 inside one, all three this project's own probes of the spaced-minus rule (`docs/panel/180-briefs/llm-ergonomist-second-reading.md:48`, `docs/panel/181-briefs/llm-ergonomist-second-reading.md:49`, `docs/panel/181-briefs/llm-ergonomist.md:60`) |

**What it says.** No C or shell habit I can point at writes a sign apart
from its number: 0 in 12,716 unary minuses, 0 in 17 negative case labels, 0
in 153 shell arms. So a spaced `-` opening an arm is not a C writer's sign
carried over; the shape it does match, at a line's start, is Markdown's
bullet (43 bullets against 3 attached in this repository's own prose, which is the text a model
here reads most). That is a fact about writers, and §4.15's *`certain`,
because in a pattern a `-` can only be its literal's sign* is a fact about
the grammar: both true, and a `Fix` tagged `certain` is machine-applied
(CLAUDE.md § 8), so it must be true of what the author MEANT. The counts give
the signed reading no habit to stand on.

- **(4c), the premise stands: object.** §4.17, CLAUDE.md § 8. Lane 135c's
  `s5a`, re-run on my compiler (`heroes check --apply --in-place`, then
  `run`): the certain fix writes `-1`, `-2`, `-3` and the program prints
  `many` where `two` is meant, and the counts above find no habit
  that would make that the rare case.
- **(4a) and (4b): no objection from this seat between them; the counts lean
  to (4b) for integers**, since they name no source for a spaced sign at all.
  (4a)'s string clause is the one certain fix the habits support: a `-`
  before a string signs nothing, and `s5c` with today's fix applied, re-run
  on my compiler, is refused anew, `bad_operand` twice.
- **The `|` question, a question rather than a premise**: a spaced `-` after
  a `|` cannot be a bullet, since a bullet opens a line; so in
  `- 1 | - 2 =>` it is the writer showing that they space their signs, which
  would make it evidence FOR the head's sign reading, not a bulleted
  neighbour. Unrun: no corpus here holds the shape but the probe.
- **prediction**: a count of spaced against attached negative `case` labels
  over a large C tree (the Linux kernel, SQLite's amalgamation) gives 0
  spaced or within a handful; checkable by one `grep` wherever such a tree
  is on disk. None is on this machine (`find / -maxdepth 4 -name sqlite3.c`:
  nothing).
- **condition**: a C, shell or Heroes corpus where spaced signs are a habit
  of more than a few percent, which would make the signed reading a live
  one.

## Found beside the questions, for the coordinator to file or route
1. **A C object declared as an extern `function` is exit 2, internal error**,
   on macOS and Linux x86-64 (experiment 4): `errno`, `stdin`, `optarg`, with
   and without parameters. Same names as `constant`: a clean exit 1. Not in
   `docs/work/DEFECTS.md`.
2. **glibc's `FD_ZERO` is a statement macro** (`do { ... } while (0)`): any
   route that asserts a `()` result through a macro, or probes it as
   `(void)NAME(...)`, cannot compile it (experiment 6). Today it is exit 1,
   `ffi_unknown_name`, whose first sentence (*declares no `FD_ZERO`*) is
   false of that header, as it is of `sys/wait.h` for `WEXITSTATUS`.
3. **Defect 145's query should be `typedef NAME hero_q;`, not `sizeof`**,
   which calls a function name a typedef (§ Defect 145).
4. **Platform split that exists today**: `isnan` binds on Linux (glibc
   declares `int isnan(double)`) and is `ffi_unknown_name` on macOS; `htonl`
   binds on Linux and not on macOS. Under (1b) both get the macro message on
   macOS and bind on Linux, which is today's split with a true message.

## Verdicts, in one place
| question, route | verdict | stands on |
|---|---|---|
| Q1 (1a) | **veto** | §1.12, §4.19, spec § 13; experiments 1 to 6, the ASan overflow |
| Q1 (1a') | **veto** | the same; its column equals (1a)'s |
| Q1 (1b) | **approve**, with a new code whose first sentence is true and a note naming (1d) without drafting C types | §4.17, §4.19 |
| Q1 (1c) | object | §4.19: a mark that refuses nothing |
| Q1 (1d) | **approve** as (1b)'s repair | measured on both platforms, through the compiler |
| Q1 (1e), unlisted | not a route that closes Q1 | measured: type-blind macros stay open |
| defect 145 | the `typedef` query, in (1b)'s run if Q1 takes (1b), on its own otherwise | the eight-name table |
| Q4 (4c) | object | §4.17, CLAUDE.md § 8; the counts |
| Q4 (4a), (4b) | no objection between them; the counts lean to (4b) for integers | the counts |

Q2, Q3 and Q5 are not this seat's (`00-shared.md` § The seats).
No paid run was made or is proposed by this seat.
