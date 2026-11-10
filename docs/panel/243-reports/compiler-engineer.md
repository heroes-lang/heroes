# Panel 243, compiler-engineer

Worktree `.claude/worktrees/243-compiler-engineer`, detached at `89db84347`. Stage 1 from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 12.15 s), stage 2
(`./heroes build selfhost/main.hero -o heroes-next`, real 328.22 against user 146.86 + sys 28.10:
three seats building at once, load 205, so not a duration), `--version` printing `sources
14e9e5ce168a2336`, as the brief says. Kept unmodified as `heroes-base`. Each route below is one
patch, applied alone, built as its own compiler from `heroes-base`, then reverted; the patches and
every probe are under `.claude/worktrees/p243-scratch/compiler-engineer/` (`q1/r0.diff`,
`q2/route_a.diff` with `q2/library_folders.hero`, `q2/route_b.diff`, `q3/route_L.diff`,
`q3/route_LS.diff`). Nothing is committed; no GitHub write; no paid run.

The scale I judge against, measured: `selfhost/` is 792 `.hero` modules and 207,031 lines by
`cat | wc -l` (tests included); the FFI's emit and pointee modules alone (`emit/ffi*.hero`,
`cli/pointee*.hero`, `cli/header_types.hero`) are 11,988. Pascal-P4's ~4,000 lines was passed long
ago, so the ceiling question here is the marginal one: what each route adds, where, and whether it
adds a second implementation of something clang already answers.

## Q1, defect 2421: a by-value argument past `...`

### Where a parameter past `...` is decided today (read, with lines)

- **What is asked**: `cli/pointee_wants.hero` `pointees` asks, per extern parameter, one kind per
  lend (`LENT`, `AT`, `UNIT`, `PLACE`, `READ`) and, for every extern with a parameter, one `ARITY`
  ask (the lines after `index @ index + 1`). A parameter handed by value, neither `@`, `lent` nor
  counted, asks nothing.
- **Where `...` is known**: `emit/ffi_past_prototype.hero` `variadic_of` (99-111) and `variadic`
  (116-127) read `"variadic": true` from the dump of `extern __typeof__(*name) hero_ty_name;`,
  called from `cli/pointee.hero:157` (cached) and `:229` (fresh). Because the `ARITY` ask exists for
  every extern with a parameter, the fixed count is already computed for every variadic binding:
  route (R0) needs no new clang run.
- **What is held**: `lines` (132-157) writes one failing `_Static_assert` per lend kind at an index
  past the fixed count; `cli/header_types.hero:145` appends them to the check unit; `refused`
  (163-244) reads them back, called from `emit/ffi_unit.hero:57`. `header_types.answered`
  (153-185) drops every by-value ask (its own comment: *a by-value parameter is left out*).
- **What the probe sees**: `emit/extern_probe.hero` writes
  `hero_ffi_probe_h_main_printf(const char * a0, int64_t a1) { (void)(printf)(a0, a1); }`
  (`--emit-c` of the critic's `fixedarity`, line 139 of its C); past `...` the call converts by the
  default promotions only, so none of `PROBE_ERRORS`' four flags and none of
  `emit/ffi_narrowed.hero`'s needles can fire. **A by-value argument past `...` is held to
  nothing.**
- **`ffi_build.hero` holds no rule about variadics** (`grep -n 'variadic\|\.\.\.'` printed
  nothing). The only call-shape reader that touches one is `emit/ffi_call.hero:110-150`: arity,
  and clang's `-Wformat-security` words for a format bound alone.
- **The format crosses as a run-time pointer**: `(void)printf(hero_cstr_nonnull_at(t2,
  &hero_here), t3)` with `t3 = INT64_C(42)` (same `--emit-c`, line 169), confirming the critic's
  item 5: no conversion reader exists and clang never sees a literal.

### The population, enumerated from the tree

1,228 tracked `.hero` files hold an `extern` group in `selfhost/`, `tests/`, `examples/`,
`packages/` (`git ls-files '*.hero' | xargs grep -l '^\s*extern "'`, then those four roots; the
other 865 are panel evidence). Every function they bind with two or more parameters, 203 names,
was asked of clang against 28 system headers, curl's and sqlite's included, by the compiler's own
`__typeof__` dump: three are variadic, `curl_easy_setopt`, `printf`, `sprintf`. The tree's own
headers declare eight more (`grep -E '\.\.\.\s*\)'` over tracked `.h`/`.c`: `va_scribble`,
`va_length`, `va_scrub`, `va_fill`, `va_wide`, `va_pair`, `tally_bump`, and glibc's `prctl` named in
a comment). **`selfhost/` binds no variadic at all.** The bindings with a parameter past `...`: 15
in 10 programs (`q1/variadic_bindings.txt`); 7 hand an argument by value there:

| program | binding past `...` | what C reads | correct? |
|---|---|---|---|
| `examples/curl` | `value: cstr` (lease) | `char *` (`CURLOPT_URL`) | yes |
| `run/ffi-variadic-at-a-fixed-arity` | `value: i64`, `"%lld"` | `long long` | yes by width everywhere; by C11 7.21.6.1p9's letter undefined on Linux, where `int64_t` is `long` |
| `run/fixedbugs-584-…` | `n: i64` to `sprintf`, `"%lld"` | `long long` | the same |
| `run/fixedbugs-2102-…lease…` | `s: cstr` ×2 | `char *`, `const char *` | yes |
| `surface-fixtures/twoarity/as_int` | `value: i64`, `"n = %d\n"` | `int` | **no**: right by luck at 42; the critic's `4294967338` prints `42` |
| `surface-fixtures/twoarity/as_text` | `value: cstr` (lease), `"%s"` | `char *` | yes |
| `surface-fixtures/twoarity-lent/as_int` | as `as_int` | `int` | **no** |

My grep reads single-line declarations; a binding written across lines would be missed (none
found by eye in the 16 hits' files).

### Route (R0), built and run

Every parameter handed by value past `...` is refused `ffi_parameter_type` on the parameter, the
note naming the wrapper route. **48 lines added, 2 changed, four files**: `cli/pointee_ask.hero`
+9 (a `VALUE` ask), `cli/pointee_wants.hero` +14 (`by_value`, asked of every by-value parameter),
`cli/header_types.hero` 1 changed (`answered` passes `VALUE` by), `emit/ffi_past_prototype.hero`
+24 (one arm of `lines` and `refused`). No new mechanism, no new diagnostic code, no new clang run:
one more kind of an assertion that already exists. Its compiler `heroes-r0` (`sources
56c6c6165512f927`) built in 59 s on a quiet machine.

Run, base against (R0), each built and its binary run:

| shape | base | (R0) |
|---|---|---|
| 2421's `slot_read(count, value: i32)`, `va_arg(ap, long)` | exit 0, prints `4294967295` | exit 1 `ffi_parameter_type` on `value` |
| its wrapper, `slot_own.h`: `static inline long slot_read_long(int count, long value)` | | exit 0, prints `-1` |
| `printf("%d\n", i32 -1)` | `-1` | refused |
| `printf("%ld\n", i32 -1)` | **`4294967295`** | refused |
| `printf("%ld\n", i64 -1)` | `-1` | refused |
| `printf("%lld\n", i64 -1)` | `-1` | refused |
| `printf("%zu\n", u64 max)` | `18446744073709551615` | refused |
| `printf("%d\n", i64 4294967338)` | **`42`** | refused |
| `open(path, flags: i32, mode: i32)`, mode 420 | file made `644` | refused |

`heroes check` does not reach it (exit 0 on 2421): like every clang-asked FFI check it runs in a
build or `--emit-c`. Census by `--emit-c` over the 11 programs binding a variadic
(`q1/census_r0.txt`): (R0) refuses **5 that build today**, `examples/curl`,
`run/ffi-variadic-at-a-fixed-arity`, `run/fixedbugs-584`, `run/fixedbugs-2102-…lease…` (2
bindings), `surface-fixtures/twoarity` (2), and adds a second error to `twoarity-lent`; the six
`unsupported/` and `fixedbugs/` variadic cases keep their codes. Of the 7 by-value bindings, 2 are
wrong programs, 2 are right by width and undefined by the letter on Linux, 3 are correct pointer
lends. Restricted to numbers (the conservative scope, R0-n), it refuses the 4 numeric bindings and
leaves the 3 pointer ones open.

**What (R0) over every by-value argument means for the code**: with it, nothing a binding declares
past `...` is held to a type, a lend (2102, 2103, 2106, 217's R3(c)) or a value (2421) alike. The
five arms of `ffi_past_prototype.hero` become one sentence, *a binding declares no parameter past
its header's `...`*, and the fixed-arity route of panels 052 and 094 is retired for the wrapper.
That is a reduction of the compiler, not a growth: the five kinds' `lines`/`refused` arms (about
150 of its 462 lines) could collapse to one, unbuilt here.

It reverses one ratified sentence: panel 217's R3(c) left a lease past `...` open (`a cstr without
lent takes only a lease, and past ... it stays open`, the module's doc, 14-18). That ruling was
about whether C may WRITE the bytes, and stays true; whether C READS them as a pointer was never
held. The synthesis says so if it adopts the whole scope.

### Route (R), the lane's: not built, and why

(R) is (R0) plus an exception: a format function with a literal format, whose conversion for each
argument names its type. What it needs, each measured or read:

1. **Which function is a format function and which parameter is its format.** The dump the
   compiler already makes carries no `FormatAttr` (0 hits for `printf` and `open` through
   `__typeof__`, `q1/c/dump_ty.json`): the attribute is on the declaration, not its type. A dump
   filtered on the real name has it, but the JSON node holds **no arguments** (`{'kind':
   'FormatAttr'}`, no archetype, no format index); only the text dump prints `printf 1 2`. So it is
   a second clang run per variadic name and a reader of a second dump format (`header_types.hero`
   says the JSON's shape is the contract). `__builtin_has_attribute` is unknown to Apple clang 21
   (measured), so C cannot be asked. A name list is a premise about the world, and misses a
   library's own (`curl_mprintf`, `sqlite3_mprintf`). The one reading that rests on clang:
   `-Wformat-nonliteral` on the probe, whose call already hands a non-literal format, fires at the
   format's column exactly when the callee is a format function (measured, `q1/c/nonlit.c`).
2. **Which call passes a literal.** Per call, not per declaration: the refusal lives in the check
   unit at the declaration, the literal at a call two instructions earlier (`t1 =
   HERO_STR_LIT(…)`, `t2 = hero_str_lend_at(t1, …)`, `emit/inst.hero:355`). A data-flow walk of
   each function's IR; `twoarity`'s format arrives through `say(format: str)` and is no literal,
   which is right (that program is wrong).
3. **The grammar**: flags, `*` width and `.*` precision each eating an `int` (`%*d` with an `i64`
   width is a `-Wformat` error, measured), `hh h l ll j z t L`, `c s p n d i o u x X f F e E g G a
   A %`, POSIX `%1$d`, glibc's `%m`, C23's `%b` and `w32`. `%n` writes through its pointer: refused
   whatever its type.
4. **The yardstick**: `-Wformat` judges by type, C11 7.21.6.1p9 and 7.16.1.1p2's letter (measured
   on this Mac: `%ld` with `int64_t` refused, `%jd` with `int64_t` refused because `intmax_t` is
   `long` here, `%zu` with `uint64_t` refused, `%lld` with `long long` accepted). A width reader
   would accept `%ld` with `i64` on this Mac and Linux and print garbage on Windows, `long` 32 bits
   there (panel 242's ffi-pragmatist § 1.4). To be per platform it must ask clang `sizeof` or
   `__builtin_types_compatible_p` of each conversion's C type in the check unit, since
   `header_types.hero`'s own rule is *never a width table this compiler carries*.

Estimate, an inference and not a measurement: 350 to 500 lines over `cli/header_types.hero` (400
today), `cli/pointee.hero` (615), `cli/pointee_wants.hero` (314), `emit/ffi_past_prototype.hero`
(462) and a new format reader, plus its cases. It is **a second implementation of clang's format
checker inside the compiler**, to admit a call the closure list never makes.

### Route (E), clang reads the literal: not built in Heroes, measured in C

(E) puts the literal into the C and lets `-Werror=format` (already on every unit's line,
`cli/flags.hero:141`, and kept in header code, `emit/header_region.hero` `KEPT`) judge it. The
critic's cost holds: it compares types, so with the emitter's `int64_t` the tree's own
`%lld` case is refused on Linux (the lane's measurement). A variant nobody listed, **(E')**: past
`...` the emitter casts each integer to the C type its Heroes type names exactly, `i64` to `long
long`, `u64` to `unsigned long long`; then `%lld`/`%llu`/`%d`/`%f`/`%s`/`%p` are right by the
letter on every C11 implementation, and `l`, `z`, `j`, `t`, which Heroes has no spelling of, are
refused where their typedef differs (measured on this Mac, `q1/c/fmt.c`: every pair above as
stated; Linux and Windows unrun). `-Wformat` fires inside `sizeof` (measured, line 21), so a probe
per call could judge without touching the real call. Still owed: item 1 above (which function clang
judged; a callee with no `format`, `curl_easy_setopt` measured, keeps (R0)'s refusal), item 2, the
cast in `emit/guarded_args.hero` (86 lines), and a reader of `-Wformat`'s words onto the call: a
fifteenth member of `.claude/rules/c-boundary.md`'s list. Estimate 250 to 350 lines; unbuilt.

### Route (C)

Zero lines of code: a sentence under design.md Part 7 row 10 (3900-3910) that `i32` spells a C
`long` through a prototype only, and a known-cost case. It leaves `4294967295` and `42` at exit 0
for every program written as the document says, and `verification.md` § Bounded discovery says a
pinned row may never break §4.17's promise and a `blocking` class is never deferred. From where I
sit (C) is only the record of what the conservative route would have been; it is not cheaper than
(R0) in anything but the sentence, since (R0) is 48 lines.

### Q1 and Q2 are one program (item 17)

Under (R0) a libcurl package writes, in a header of its own, `static inline CURLcode
net_setopt_long(CURL *h, CURLoption o, long v)`, `…_text(…, const char *v)`, and for
`curl_easy_getinfo` a door that converts, `long v = 0; rc = curl_easy_getinfo(h, i, &v); *out = v;`
taking `int64_t *out`, since `long *` has no `@` spelling every leg shares. That header includes
`<curl/curl.h>`, which over the keg is 2428 itself, so Q2 must land with Q1.

### The notes that draft a binding past `...` (item 22), rewritten under (R0)

- `emit/ffi_call.hero:139` (arity): *… for a function the header declares variadic (`...`), the
  fixed shape this program calls it with* becomes: *Declare exactly the parameters the header has,
  at its widths; a function it declares variadic (`...`) takes nothing a binding can type past it,
  so a header of your own defines a `static inline` function of a new name with a prototype that
  calls it, and the group binds that.*
- `emit/ffi_call.hero:148` (format security): *bind the fixed shape the program calls —
  `function NAME(format: cstr, value: i64) -> i32`* drafts a binding (R0) refuses, and the very
  mistake 2421 is. It becomes: *a header of your own defines a `static inline` function of a new
  name that calls `NAME` with a literal format and typed parameters, `static inline int
  print_count(long long n) { return NAME("%lld\n", n); }`, and binds that; clang then reads the
  literal against each argument.*
- `cli/pragma_ask.hero:298` (`ffi_macro_name`): its header with `#undef NAME` is right, and for a
  variadic it ends at a binding past `...`: add *where the header declares `NAME` with `...`, that
  header also defines a `static inline` function of a new name taking each argument at the type
  `NAME` reads, and the group binds it.*
- `emit/ffi_past_prototype.hero:205` and `:233` (and (R0)'s new note): *declares a `static inline`
  function* becomes *defines a `static inline` function of a new name*, the critic's item 4: never
  a re-declaration of the variadic, which panel 052 refused.
- The module's doc still says panel 217 is *provisional, author ratification pending* (line 1-3);
  it was ratified 2026-10-17 (the critic's item 11).

## Q2, defect 2428: a library's header in a named folder, included by a header of the program's own

### Where a `pkg-config` folder is told apart from a platform folder (read)

It is not, in the verdict. `cli/shadowed.hero` `told` builds `named` (`named_folders.named_folders`
plus the environment's) and `others` (the runtime's, then clang's own and the platform's); a
`pkg-config` folder is in `named` exactly as the program's own folder is. Which word named each
folder is computed (`named_folders.of_compiler`, 50-73, kind `PKGCONFIG` and the library's name)
and used **only for the note** (`shadowed.hero`, the `origin` lookup in `told`). The verdict is
`shadowed_tree.hero` `refused` (161-187 at the base): depth 1 is a group's own header; a file whose
includer is no named folder's is refused (the first arm); a file whose includer is in a named
folder is refused where a platform folder holds its name (`twin`, the second arm), unless it is the
includer's quoted sibling. 2428 is that second arm: `net/curl_own.h` (named, the program's)
including the keg's `curl/curl.h` (named, `pkg-config`'s), the SDK holding `curl/curl.h`.

Reproduced with `heroes-base`: system exit 0; `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`
exit 1 `shadowed_header`, its note *rename the file, or move it out of
`/opt/homebrew/Cellar/curl/8.23.0/include`*, a remedy no author can take for a library's own header
(a false-remedy note; the synthesis may file it with the class it reads).

### Routes built and run

**(a) group-scoped, built** (`q2/route_a.diff`, compiler `sources 7e9664a16870782d`): a file in a
folder that a group's own `pkgconfig` answer names is that library's when its includer is that
group's header or lies in one of the same folders. A new module `cli/library_folders.hero` (73
lines with its doc, `by_header`: group header to its answer's `-I`/`-F` folders), `library_own` and
`among` in `shadowed_tree.hero` (+40, 3 changed), `told`'s new parameter in `shadowed.hero` (+9, 3
changed) and `package_stand_in.hero` (+4, 1 changed): **about 120 lines**.

**(b) consistency, built** (`q2/route_b.diff`, `sources 3505b45302b0869c`): a file a group of the
build names is the program's choice wherever it is included: `of_group(path: one.path, groups:
groups)` before `twin`. **6 lines.**

| probe | base | (a) | (b) |
|---|---|---|---|
| 2428, keg | 1 `shadowed_header` | **0**, links `/opt/homebrew/opt/curl/lib/libcurl.4.dylib` | **0**, the keg's dylib (because `handle.hero` names `curl/curl.h` directly) |
| 2428, system | 0 | 0, `/usr/lib/libcurl.4.dylib` | |
| 2428's wrapper, keg folder through `CPATH` | 1 | 1 | |
| the same through `--include` | 1 | 1 | |
| critic's `sqlitewrap`, keg | 1 | **0**, prints `3.54.1` (system `3.54.0`) | **1** |
| critic's `curldirect`, keg | 0, `libcurl/8.23.0` | 0, the same | 0 |
| overlay: `p243ov.pc` naming a folder whose `sys/socket.h` redefines `SOMAXCONN 7`, included by the group's `own.h` | 1 | **0, prints `7`** (SDK: `128`) | 1 |
| the same overlay named directly, `extern "sys/socket.h" pkgconfig "p243ov"` | **0, prints `7`** | 0, `7` | |

Panel 237's 20 `.case` directories holding `shadowed_header` (`git grep -l shadowed_header --
tests/golden`, cut to the `.case`): under both (a) and (b), `unsupported` narrowed to
`fixedbugs-23` 63 passed 0 failed (17 of the 20 among them), `unsupported fixedbugs-2420` 1 and 0,
`run fixedbugs-2420` 2 and 0. Panel 237's own `.pc` probes no longer exist (searched
`.claude/worktrees` for `*.pc` under `*237*` and for `pcmain`/`pc` folders: none), so the overlay
above stands for them.

What the overlay rows say: (a) admits a library's folder overlaying a platform header through the
group's header, and the landed rule **already admits the same overlay named directly** by a group
(base prints `7`): (a) moves the program's choice one include further and admits no class the
direct form does not. (b) is the narrowest that lets 2428's reproducer build, and only because the
reproducer also names `curl/curl.h` in another group; it refuses `sqlitewrap`, a correct program,
and every wrapper the compiler's own notes draft unless the author adds a second group naming the
library's header.

**(c)** (narrow the second arm for every folder a `pkg-config` answer names, whatever group's) and
**(d)** (pkg-config only, `CPATH` and `--include` refused) **are not built**. (a) is already (d)'s
shape: measured, `CPATH` and `--include` stay refused, and the reason is real: without
`PKG_CONFIG_PATH` the answer is `-lcurl` with no `-L` (`pkg-config --libs libcurl`), so the keg's
8.23.0 header would compile against `/usr/lib`'s 8.7.1 (the link words read, the link unrun since
the build is refused). (c) differs from (a) only where a group with no `pkgconfig` includes
another group's library header; unmeasured.

### The form that holds Q2's cases

`tests/harness/case_setting.hero` gives a case `PATH`, `GIT_CEILING_DIRECTORIES` (`SETTABLE`) and
`CPATH`, `C_INCLUDE_PATH`, `CCC_OVERRIDE_OPTIONS` (`CLANG_READS`, through `/usr/bin/env` to the
case's compiler alone). `PKG_CONFIG_PATH` is read by `pkg-config`, which the compiler starts, so it
belongs in the second list with `PKG_CONFIG_LIBDIR`, never in this process's environment. Measured:
a case can carry its own library with no absolute path, `prefix=${pcfiledir}/..` and `Cflags:
-I${prefix}/inc` read `-Ipc/../inc` under `PKG_CONFIG_PATH=pc PKG_CONFIG_LIBDIR=pc`, and that
`LIBDIR` hides the machine's (`libcurl` not found), so the case is hermetic; with `directory` the
relative value names the case's folder. Its platform twin: `sys/timeb.h`, which the lane measured
the Darwin SDK, glibc and UCRT hold and the runtime never reads. A `tests/harness/` change, owing
the net's own tests; unbuilt.

## Q3, defect 2420's two rows: a quoted include found from the includer's own folder

### Routes built and run

**(L) the lane's reading** (`q3/route_L.diff`, `sources 380855f72ed6f272`, 7 added 2 changed in
`shadowed_tree.hero`): own where a quoted spelling, read from the includer's own folder, names the
file (`quoted(…, name: "")`, the existing function's second arm alone), or where the landed sibling
arm holds (`beside && quoted(…, name)`), which is what keeps shape `e`: its `"sys/timeb.h"` from
`sys/clock.h` misses `sys/sys/timeb.h` and is found through `-I .`, not from the includer's folder,
so the lane's sentence taken alone would refuse a shape that builds today.

**(L+S)** (`q3/route_LS.diff`, `sources 6ee622861f80ff73`, 21 added 4 changed): (L), plus
`-fshow-skipped-includes` in the listing's `WORDS`, plus every includer of a file judged and the
file told once. Why both halves: measured with clang on the critic's `live`, `-H` lists
`./sys/types.h` once, at its first open beneath `sys/clock.h`; with the word it is listed again as
`.. ./sys/types.h` beneath the SDK's `sys/socket.h`, the shape `entries()` already reads. But
`shadows` set `seen[path]` at the first entry whatever its verdict, so even listed, the second
includer was never asked: the order dependence is that judge-once line as much as the guard (base
builds the critic's unguarded `live2` clock-first too).

| shape | base | (L) | (L+S) |
|---|---|---|---|
| `d`: `lib/clock.h` writing `"../sys/timeb.h"` | 1 | 0, `42` | 0, `42` |
| `e`: `sys/clock.h` writing `"sys/timeb.h"`, through `-I .` | 0, `42` | 0, `42` | 0, `42` |
| `g`: root `clock.h` writing `"sys/timeb.h"` | 1 | 0, `42` | 0, `42` |
| `live` (guarded sibling `sys/types.h`), clock first | **0** | **0** | 1 |
| `live`, socket first | 1 | 1 | 1 |
| `live2` (unguarded), clock first | **0** | **0** | 1 |
| `live2`, socket first | 1 | 1 | 1 |
| `dd`: `lib/clock.h` writing `"../sys/types.h"` + `<sys/socket.h>`, clock first | 1 | **0** | 1 |
| `dd`, socket first | 1 | 1 | 1 |
| `gg`: root `clock.h` writing `"sys/types.h"` + `<sys/socket.h>`, clock first | 1 | **0** | 1 |
| `gg`, socket first | 1 | 1 | 1 |

(L+S)'s refusal is the same words in both orders: *`./sys/types.h` stands in for the C library's
`<sys/types.h>`, which the C library's `<sys/socket.h>` includes*. (L) alone builds `dd` and `gg`
clock-first: **the lane's widening, alone, widens the danger with the rows it repairs.** Panel
237's 20 cases under (L+S): `unsupported fixedbugs-23` 63 and 0, `unsupported fixedbugs-2420` 1
and 0, `run fixedbugs-2420` 2 and 0. `-fshow-skipped-includes` on Linux's clang 18 and 22 and on
the Windows box: unrun (Docker off).

### The whole `run` form under (L+S)

Started 19:32 (`heroes-q3LS run tests/harness/main.hero -- ./heroes-q3LS run`, pid 64367, output
to `q3/run-whole-LS.txt`, which the harness writes only when it exits). At 20:14 it was building
case 433 of the form's 752 by name (`fixedbugs-396-posix-fills-its-buffers`), the Mac at load 17
to 62 beside another session's build. **It ended at exit 0: `run: 750 passed, 0 failed`**
(`q3/run-whole-LS.txt`); `real 4396.56` against user 1841.02 and sys 766.09, so the run waited
and its duration is no measurement. (L+S) refuses no `run` case. The 750 against the 752 stems my
`ls` of `tests/golden/run` printed is the harness's own count; I did not read which two it does not
take. My worktree still holds (L+S) applied and uncommitted, the compiler that run judged with; the
other patches are reverted, and every patch is in my scratch. No process of mine is running.

## What I did not build or run, said once

- (R) and (E) in Heroes: estimates above, from the modules' sizes, not measurements.
- Q2's (c) and (d), and the harness change that would hold a `.pc` case.
- The combined compiler, (R0) with Q2's (a), Q3's (L+S) and item 22's notes, whose script is
  `combined.py` in my scratch: the `run` form owned the worktree until it ended, and the
  coordinator asked for no further whole suite, so **the compiler's own tests are unrun on every
  route**, and so is `layout`. Each route was built alone and run on the cases named. The
  `layout` suite's own count (`code_lines`, read and reproduced in Python on the base files):
  `cli/shadowed.hero` 294 against the ceiling 300 (route (a) adds about 6), `shadowed_tree.hero`
  237 (about 50 more across (L+S) and (a)), `emit/ffi_past_prototype.hero` 244, `pointee_wants`
  213; only `selfhost/parse/` has a directory budget.
- Instructions retired for `heroes check selfhost/main.hero` and a build of `examples/curl`:
  unrun, the machine never still. What is measured: `heroes check` never reaches (R0)'s code
  (exit 0 on 2421 under `heroes-r0`), so what (R0) adds there is the checking of its own 48 lines,
  an inference; at build time it adds one `VALUE` ask per by-value extern parameter to the pointee
  check's key and, for a variadic only, one assertion line per parameter past `...`, no new clang
  run. (L+S) lengthens the `-H` listing: on the critic's `live`, 45 entries to 58 clock first and
  96 to 156 socket first (`grep -c '^\.'` of `q3/live/order1.c` and `order2.c`); its cost in a
  real build is unmeasured.
- Linux and Windows: every verdict above is this Mac's (Apple clang 21.0.0); Docker is off.

## Verdict

- `verdict`: **object** (no veto: no route adds a core construct; each lives in the clang-question
  passes, `cli/` and `emit/ffi*`, and Part 5's core is untouched)
- `section`: design.md §1.1 (simplicity sets the ceiling), §1.7 (*does it remove a special case
  from the compiler?*), §1.12 (robustness), §4.19's acceptance ladder (rungs 1 and 4 bind
  `printf` and `curl_easy_setopt` past `...` and must be reworded under (R0)); CLAUDE.md § 2
  (Principle 0)
- `implementation_cost`: Q1 (R0) 48 added and 2 changed lines in `cli/pointee_ask.hero` (95 lines
  at the base), `cli/pointee_wants.hero` (314), `cli/header_types.hero` (400),
  `emit/ffi_past_prototype.hero` (462), built and run; over every by-value argument it can shrink
  that module, its five past-`...` arms becoming one rule (unbuilt). Q1 (R) 350 to 500 lines over
  the same four plus `cli/pointee.hero` (615) and a new format reader, and (E') 250 to 350 plus
  `emit/guarded_args.hero` (86) and a fifteenth clang-failure member, both unbuilt estimates.
  Q2 (a) about 120 lines, a new `cli/library_folders.hero` (73) and `cli/shadowed_tree.hero`
  (373), `cli/shadowed.hero` (486), `cli/package_stand_in.hero`; (b) 6 lines; both built. Q3 (L)
  7 and 2, (L+S) 21 and 4 lines in `cli/shadowed_tree.hero`, built.
- `needed_for_self_hosting`: **no**. `selfhost/` binds no variadic and no `pkgconfig` group (24
  groups, all `hero_os.h`, `hero_compiler.h`, `stdlib.h`); all three are robustness fixes (rank 3).
- `argument`: The lane's (R) puts a second copy of clang's format checker inside the compiler: a
  grammar, a second dump format (the JSON `FormatAttr` carries no index, measured), a per-call
  literal tracer and a per-platform type question, for a call `selfhost/` never makes. Principle 0
  says it waits. (R0) closes 2421 in 48 lines with no new mechanism, and over every by-value
  argument it is one rule where today there are five. Q2's (a) builds 2428 and the sqlite wrapper
  and admits only what a group naming the header directly already admits; (b) is narrower and
  refuses a correct program. Q3's (L) alone widens the danger it repairs (`dd`, `gg` build
  clock-first, measured); (L+S) refuses it in both orders.
- `prediction`: if (R0) over every by-value argument lands in batch 30, its landing commit's
  `git diff --numstat -- selfhost/` adds under 150 lines including its tests, and the round's gate
  moves exactly the five programs measured here out of the accepted set (`examples/curl`,
  `run/ffi-variadic-at-a-fixed-arity`, `run/fixedbugs-584`, `run/fixedbugs-2102-…lease…`,
  `surface-fixtures/twoarity`); if (R) lands instead, the same diff adds over 350 lines across at
  least four modules. Checkable at batch 30's closing commit.
- `condition`: a measured Part 11 effect, the thesis harness showing first-attempt compiles lost to
  (R0)'s wrapper on tasks that call `printf`, would make a format exception earn its ticket, and
  then as (E'), clang the reader, not (R); a correct program route (a) refuses, or an overlay it
  admits that the direct group does not, would move Q2 to (b) or (c); a correct program (L+S) refuses would move Q3; the whole
  `run` form, 750 cases, found none.

## Recommended, for the synthesis

- **Q1: (R0) over every by-value argument past `...`**, said as *a binding declares no parameter
  past its header's `...`; a header of the program's own defines a `static inline` function of a
  new name with a prototype that calls it*. Its conservative half, (R0) over numbers only, leaves
  a pointer read as an integer open; it refuses 4 tracked bindings instead of 7. It reverses panel
  217's R3(c) carve-out on its second axis (what C reads), and rewords §4.19's rungs 1 and 4 and
  the four notes of item 22 (20 pinned expectation files: 5, 12 and 3 by `git grep -l -F` of each
  note's words). (C) is the record of the conservative route only.
- **Q2: (a)**, `pkg-config` folders of the includer's own group; `CPATH` and `--include` stay
  refused (header and library from two releases). The false-remedy note (*move it out of the
  Cellar*) is worth filing. The case form: `PKG_CONFIG_PATH` and `PKG_CONFIG_LIBDIR` beside
  `CLANG_READS`, a case carrying its own `.pc` with `${pcfiledir}`.
- **Q3: (L+S)**, never (L) alone. The two rows of 2420 and the order-dependent stand-in (the
  critic's item 16) are one repair.
