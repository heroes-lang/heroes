# Panel 245, the ffi-pragmatist

Written 2026-11-10 from 22:55 to 2026-11-11 about 00:40 (a rate-limit stop between). Worktree `.claude/worktrees/245-ffi-pragmatist`,
detached at `7fb6868b20dfaecbdc7cf501b75b79aef7b14cf3` (`git rev-parse HEAD`). Compiler built
there in two stages: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 11.46`),
then `./heroes build selfhost/main.hero -o heroes-next` (`real 184.53`, `user 124.10`; load 3.7 at
the start, so not a duration). `heroes-next --version`: `heroes 0.6.0`, `sources 0fe313ba58ba197c`.
`clang --version`: Apple clang 21.0.0 (clang-2100.4.1.1), arm64-apple-darwin27.2.0. Probes under
`.claude/worktrees/p245-scratch/ffi-pragmatist/`. Every platform sentence is this Mac's.


## Q1, defect 2464: what a door's call of a warned function prints

- `verdict`: **object** to the lane's route as written (the three words quiet in the region),
  **approve** its two deprecation words, **veto** routes (a) as the critic wrote it and (c).
- `section`: design.md §1.11 (a door is now the only route to a variadic, so a door's C is
  binding C), §1.12 (robustness), §4.19's ladder rung 4 (libcurl, `curl_easy_setopt` through a
  door); panel 208's R1 (design.md:2676-2677) for the outcome. design.md does not say what a
  door's body is held to beyond the region's lists; that silence is the question.
- `experiment`: probes under `p245-scratch/ffi-pragmatist/q1/`, each built by `heroes-next` at the
  base, then its emitted unit rewritten per route by `tools/route.py` and compiled, linked and run
  with the build's own words (`tools/try.sh`) at `-O0` and `-O2`; outputs `q1/routes-q1.txt` and
  `q1/routes-q1-d.txt` (98 + 16 rows). Real headers: the SDK's `sprintf` with fortification off,
  `gets` and `tmpnam` (`sdk0.h`); OpenSSL 4.1.0's deprecated `SHA256_Init` through `pkgconfig
  "libcrypto"` (`ossl.h`); libcurl 8.23.0's own type checker through `pkgconfig "libcurl"`
  (`kegcurl.h`: `curl_easy_setopt(h, CURLOPT_TIMEOUT, (int32_t)seconds)`). Clang accepted every
  unit the table says compiled.

  What the base prints, every build at exit 0, both levels: `-Wdeprecated-declarations` (3 in
  `sdk0`, 3 in `ossl`), `#warning`, `#pragma message`, `-Wattribute-warning` (curl's checker: 1 at
  `-O0`, 2 at `-O2`), and nine undefined-behaviour words outside `-Wall`: `-Wshift-count-overflow`,
  `-Warray-bounds`, `-Wdivision-by-zero`, `-Wnull-dereference`, `-Wreturn-stack-address`,
  `-Winteger-overflow`, `-Wshift-negative-value`, with `-Wconstant-conversion` and `-Wpointer-sign`
  beside them. The UB doors' values move by level: `x / 0` prints `0` at `-O0` and `10` at `-O2`,
  a returned stack address `8` and `1`, and `1 << 40` prints `2` at `-O0` and at `-O2` a new number
  each run (`6163161996`, `6169518972`, `6097101692`, `6137209724`, `6095954828`).

  | route | UB doors | deprecated (`sprintf`, `gets`, `SHA256_Init`) | curl's checker | the region's kept errors |
  |---|---|---|---|---|
  | base | raw, exit 0 | raw, exit 0 | raw, exit 0 | all refused |
  | lane (three words quiet) | raw, exit 0 | silent, exit 0 | **silent, exit 0** | all refused |
  | (a0) `-Weverything` ignored, region's errors after | silent, exit 0 | silent | silent | **lost**: `-Wint-conversion`, `-Wincompatible-function-pointer-types`, `-Wincompatible-pointer-types`, `-Wshorten-64-to-32` compile |
  | (a) the same, build's and clang's default errors raised again | silent, exit 0 | silent | silent | all refused, `-Wimplicit-int` too |
  | (b) nine UB words errors, advice quiet | refused | silent | silent (b) / refused (b-aw) | all refused |
  | (c) the include read as a system header (line marker, flag 3) | silent, exit 0 | silent | raw | **lost: every one** (missing return, uninitialised read, `%d` of a `char *`, `-Wformat-nonliteral`, int to pointer, pointer types, narrowing; only the program's own undeclared calls stay) |
  | (d) **deny by default**: `-Werror` on the unit, deprecation words quiet in the region | refused, both levels | silent, exit 0 | **refused, both levels** | all refused, `-Wdeprecated-non-prototype` too |

  `-Wattribute-warning` by level, alone (`q1/p-aw`): a `warning`-marked `static inline` function
  and a `noinline` one are told at `-O0` and silent at `-O2` (the call is optimised away); curl's
  checker, whose body is `__asm__("")`, is told at both. `#pragma message` stays a warning under
  `-Werror` (clang never upgrades it).

  Census, re-run counting every word (`census/one.sh`, the base region with `-Wformat-security`
  let back as a warning, each header included by its name under `-I/opt/homebrew/include` rather
  than by absolute path, `-fsyntax-only`, three wide, `real 40.56`): of the lane's 2,400 Homebrew
  headers, 1,066 compile, 1,327 are refused and 7 passed the 200,000-byte output bound (exit 141).
  **Of the 1,066, none prints any warning of any word**; the critic's 1,432
  `-Winclude-next-absolute-path` lines came from the absolute path. Control (`census/ctl.tsv`): the
  same script sees the UB words, the deprecation, `#warning` and `-Wformat-security` in my headers,
  and never `-Wattribute-warning`, which clang says only while generating code. `grep` over all
  139 keg include folders (`diag/keg-includes.txt`) finds one live `warning` attribute: curl
  8.23.0's `typecheck-gcc.h:273` (libpng's `pngconf.h:372` stands under `#if 0`). Of the 761
  tracked headers outside `docs/`, `seed/`, `vendor/`, `runtime/`, 679 compile; their words are
  `-Wpragma-pack` 10 and `-Wignored-pragmas` 1 (my shape's artefact, the real unit quiets both),
  `#warning` 2 (`surface-fixtures/warning248/lib.h`, `unsupported/fixedbugs-158-sibling-a.h`),
  `-Wformat-security` 1 (defect 2036's own case), `-Wdeprecated-non-prototype` 1 (`426`) and
  `-Wincompatible-library-redeclaration` 1 (`2049`), the last two refused today for other reasons.

  clang 22's `diagtool show-enabled` (Homebrew's LLVM 22, not Apple clang 21, so a list of the
  other compiler): 595 default-on warning flags stand outside `-Wall`'s tree, and 57 flags are
  errors by default, four of them inside it (`-Wimplicit-int` among them, which the region's
  `-Wall` ignore turns off inside the region; the build still refuses such a header, below).
- `argument`: Since panel 243's R1 a door is mandatory binding C, so what clang tells a door is
  the boundary's check. The lane's route silences the one live `warning` attribute among 139 kegs,
  libcurl's checker of a value handed past `curl_easy_setopt`'s `...`: defect 2421's class, at exit
  0. Deprecation is advice and 208's outcome binds: approve those two words. The class is not three
  words: 595 default-on flags lie outside `-Wall`, so any deny list rots with clang. Route (d) fails
  closed, refuses all nine UB doors and curl's mismatch at both levels, keeps every kept error, and
  costs 0 of 1,066 real headers. (a0) and (c) drop C-ABI errors: veto.
- `prediction`: under (d), `examples/curl` with its `#undef` removed and
  `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig` still builds (its one door is
  well-typed), and a door handing CURLOPT_TIMEOUT an `int32_t` is refused at `-O0` and `-O2`.
- `condition`: a real library header, or a door a ladder rung needs, that (d) refuses for a
  construct C defines and the allow-list cannot name; or a measured case where `-Werror` on the
  unit refuses a line the compiler wrote.

### Q1, the prototype built and run

`prototype.diff` in my scratch (uncommitted, worktree only; edits to two files,
`selfhost/emit/header_region.hero` and `selfhost/emit/ffi_past_prototype.hero`), built by
`heroes-next` as `heroes-proto` (`sources 65d119f73cee7298`). The region gains, after its
lists: `ignored` for `-Wdeprecated-declarations` and `-Wdeprecated-pragma`; `error` for
`-Wshift-count-overflow`, `-Warray-bounds`, `-Wdivision-by-zero`, `-Wnull-dereference`,
`-Wreturn-stack-address`, `-Winteger-overflow`, `-Wshift-negative-value`, `-Wfortify-source`,
`-Wbuiltin-memcpy-chk-size` and `-Wattribute-warning`; then `FORMAT_DEFINED` passed over again.
**The last line is not decoration**: without it, `-Wfortify-source` (which holds
`-Wformat-overflow` and `-Wformat-truncation`, `diagtool tree`) took back defect 2036's
`snprintf`-cut case under `--sanitize`, measured red, then green once added: an enumerated list
interacts with clang's groups, which is route (b)'s standing cost. Measured with `heroes-proto`
at `-O0` and `-O2` (`proto/final.txt`): every UB door and the critic's `own.h` refused
(`ffi_header_refused`, clang's line quoted); `sdk0` (`sprintf`, `gets`, `tmpnam`) and `ossl`
build silent and run as before (`2`, `186`); curl's checker refused at both levels; the toy
`warning` function refused at `-O0` and built at `-O2`; `#warning` and `#pragma message` still
raw (left out on purpose, below). The tree's own cases, each form narrowed with the harness's
third word: `run fixedbugs-2036` 3 passed, 1 failed (the `printf(m)` case, Q2's trade);
`run fixedbugs-584` 7 and 0; `unsupported fixedbugs-158` 14 and 0, **but** the deprecated-name
carrier (`fixedbugs-158-noted-mid.h`) compiled in the prototype's region prints no warning and no
note before its error, where the base's prints one of each (`proto/c158/r-*.err`): the case passes
and tests nothing, as the critic predicted. All 18 examples with a `main.hero` and an `extern`
build at exit 0 with no warning under both compilers (`proto/examples.txt`). `examples/curl`
with its `#undef` removed and `pkgconfig "libcurl"` over the keg builds under both; the same
program with a called `net_set_timeout(CURL *, int32_t)` door is refused by `heroes-proto` at
both levels and builds at exit 0 with curl's raw warning under `heroes-next` (1 line at `-O0`, 2
at `-O2`). Uncalled, that door is told by neither: curl's checker is a code-generation warning.

What I did not build, and say so: route (d)'s mechanism in the compiler. `-Werror` on the main
unit's own compile line, or the compiler refusing a `warning:` its own reading of clang locates
in a group header, is the compiler-engineer's ground (`cli/units.hero`, `cli/kept.hero`); route
(d) stands measured at the C level only. Its allow-list would hold the two deprecation words,
`#warning`, `#pragma message` (which clang never upgrades), `-Wpointer-sign` and
`-Wconstant-conversion` (defined constructs; `sqlite3_column_text` returned as `const char *`
from a door is `-Wpointer-sign`, unrun as a door here) and every one of them moves
`produce.hero:463`'s own test and the 158 carriers, which the compiler-engineer prices.

## Q2, defect 2465: may a variadic be bound directly, and what prints a run-time string

- `verdict`: **approve** "no variadic is bound directly, whatever stands past its `...`; a door
  calls it", and `-Wformat-security` raised in the region; **object** to any rule keyed to
  clang's `format` attribute alone.
- `section`: design.md §1.11 and §4.19's ladder rungs 1 and 3 (`printf`, SQLite), §1.12, and
  Part 7 row 10 as corrected at the base (design.md:3932-3950). design.md does not say whether a
  variadic with nothing past its `...` may be bound; panel 243's R1 refuses what passes, not what
  is named.
- `experiment`: `q2/d-*` (one program each, the base `heroes-next`), `q2/w-doors`, then
  `proto/q2-*` with `heroes-proto`; every run under `timeout 10`, output bounded, `stdin` a
  regular file. Direct, nothing past `...`: `printf`, `fprintf`, `snprintf`, `syslog`,
  `curl_mprintf` are refused at the base (`ffi_call_shape`, `-Wformat-security` on the binding's
  probe). `sqlite3_mprintf`, `open`, `fcntl`, `ioctl`, `execl`, `sqlite3_config` build at exit 0,
  and run, twice each (`q2/direct-run.txt`): `sqlite3_mprintf("n=%d")` prints `n=-1198300245`,
  `"n=%s"` dies of SIGSEGV (exit 139, not the runtime's panic); `sqlite3_config(GETMALLOC)` dies of
  SIGSEGV; `open(O_CREAT|O_WRONLY)` makes `made.txt` with mode `0450` (`-r--r-x---`);
  `ioctl(0, FIONREAD)` returns `0`, so the kernel wrote an `int` through an address read from the
  `va_list`; `fcntl(0, F_GETLK)` and `execl("/bin/echo", "echo")` return `-1` (the SDK's `execl`,
  `unistd.h:453`, carries no `sentinel` attribute). Through a door, the base (`q2/w-doors`, plain
  and `--sanitize`): `say(f) { return printf(f); }` with `n=%ld` prints `n=4337173216`, and
  `n=4311795424` under `--sanitize`, exit 0, ASan and UBSan silent; eight `%s` print the stack's
  bytes, the format's own among them, at exit 0 plain, and ASan's SEGV under `--sanitize`; `%n`
  dies of SIGABRT both ways; the door into `sqlite3_mprintf` reads `%d` as `8138016010`. A
  run-time string: `puts(s)` and `fputs` bound directly (fixed prototypes) and the door
  `printf("%s", s)` print `n=%s %d %n` verbatim, base and prototype alike.

  Under `heroes-proto`: all seven unattributed or attributed direct bindings refused at the
  binding's name, exit 1, *"`fcntl.h` declares `open` variadic, 2 parameters and then `...`, and
  this binding names it with nothing past the `...`"* (the `ARITY` entry of the asks had to be
  excluded, a bug of my first build); the forwarding doors refused at `fwd.h:4`
  (`-Wformat-security`); the correct doors (`open3`, `nread`, `getmalloc`, `say_n`, `sq_number`
  with literal formats) build and print what the base prints, mode `-rw-r--r--`, `n=4294967338`.
  **The residue, under every route**: `sqsay(f) { sqlite3_mprintf(f) }` (no `format` attribute)
  and `open2(p, fl) { open(p, fl) }` build and still read `n=6945248010` and write mode
  `-r--r-----`. No rule keyed to a binding or to the attribute reaches C the program wrote.

  The cost, enumerated (`q2/enum/`): the 44 system and library headers tracked groups name that
  this Mac has declare 95 variadic functions (clang's AST, `, ...)'`); 7 tracked bindings name
  one, none a program meant to build with nothing past `...` (two pass a mode past it in
  `docs/panel/178-*`, refused since panel 243's R1; five are annotated refusals); the 20 variadics
  declared in tracked headers are bound only by annotated refusals; `selfhost/` and `examples/`
  bind none. Raising `-Wformat-security` in the region: 0 of the 1,066 compiling Homebrew
  headers (census above), 1 tracked case, defect 2036's `printf(m)`.
- `argument`: A binding with nothing past `...` lets the callee read past it by run-time data:
  SIGSEGV from `sqlite3_config` and `sqlite3_mprintf`, a kernel write from `ioctl`, a mode from
  `open`, all at exit 0. The callee, not the format attribute, decides; a `format`-keyed rule
  misses four of six. Refusing every variadic binding costs no tracked program and leaves one
  route, the door, where the arguments are C's and visible. `-Wformat-security` in the region
  then refuses the forwarding door, costing 2036's `printf(m)`, whose fix is `printf("%s", m)`:
  a correct program refused against a memory read at exit 0, and §Precedence ranks the fault.
- `prediction`: under the rule, `examples/sqlite` (§4.19's step 3) builds unchanged (measured:
  it binds no variadic), and the first program a model writes binding `open(path, flags)`
  directly is refused at the binding with the door in its note.
- `condition`: a ladder rung or real binding that needs a variadic bound directly and cannot be
  reached through a door; or a real header's own code that `-Wformat-security` refuses (none in
  1,066 here; Linux's glibc headers unrun).

## Q3, defect 2448: a header from one release, a library from another

- `verdict`: **object** to the base (every mismatch builds at exit 0, both directions); the
  class is **`blocking` by measurement now, not by inference**; **approve** (P1) and (P2) as the
  rule's two readings, judged per bound name, and (P4) whatever the ruling; **object** to (P3) as
  a general check.
- `section`: design.md §4.19 (`docs/design.md:3186`, *"The environment is a named folder"*, panel
  055 at `:3125` naming `CPATH`, `LIBRARY_PATH` and `PKG_CONFIG_PATH`), §1.12, §1.11. design.md
  does not say that a header and the library its group links must come from one place; that is
  the silence 2448 lives in.
- `experiment`: `q3/` with the base `heroes-next`. **The curl table reproduces** row for row
  (`q3/curl-table.txt`): plain `526081` beside `libcurl/8.7.1`; `CPATH`, `C_INCLUDE_PATH`,
  `--include` alone `530176` beside 8.7.1 (`/usr/lib/libcurl.4.dylib`); `--include` with
  `--library`, `CPATH` with `LIBRARY_PATH` `530176` beside 8.23.0; `--library` alone and
  `LIBRARY_PATH` alone `526081` beside 8.23.0; every build exit 0. The compiler's
  `machine_locked_path` note says *"set `CPATH` to the directory holding it"*; followed, readline
  builds at exit 0, `header 2051`, linked to `/usr/lib/libedit.3.dylib` (`q3/locked`).

  **One measured wrong value and one memory fault** (`q3/rl/hist.h`, a door correct against the
  header it is compiled with; GNU readline 8.3's `HIST_ENTRY` is `{line, timestamp, data}`, 24
  bytes, libedit's `{line, data}`, 16). The program adds an entry, stores a pointer to its own
  record `{42}` with `replace_history_entry`, and reads it back (`q3/rl-table.txt`, two runs each):

  | build | `sizeof` | header | library | `data` of an entry with none is null | its own record read back |
  |---|---|---|---|---|---|
  | plain (libedit with libedit) | 16 | 1026 | 1026 | 1 | **42** |
  | `pkgconfig "readline"`, keg's `.pc` (GNU with GNU) | 24 | 2051 | 2051 | 1 | **42** |
  | `--library $R/lib` alone, or `LIBRARY_PATH` alone (libedit header, GNU library) | 16 | 1026 | 2051 | **0** | **0** |

  Exit 0 every time. Under `--sanitize` the mismatched build dies at exit 134: *"AddressSanitizer:
  heap-buffer-overflow ... READ of size 8 ... 0 bytes after 1-byte region"*, at `hist.h:39`, the
  region *"allocated by ... replace_history_entry+0x80 (libreadline.8.3.dylib)"*: the program's
  `data` at offset 8 is GNU's `timestamp` copy. `CPATH` and `--include` refuse this door
  (`shadowed_header`), so the forward direction reaches only direct bindings; the reverse
  direction, which nothing refuses, faults.

  **What pairs the halves elsewhere**, each read on this Mac: pkg-config by construction, one
  `.pc` giving `-I` and `-L` (the keg's `libcurl.pc`: `prefix=/opt/homebrew/Cellar/curl/8.23.0`,
  `libdir` and `includedir` under it); Homebrew by the keg's prefix, its keg-only caveat naming
  `-L<opt_lib>` and `-I<opt_include>` together (`Library/Homebrew/caveats.rb:94-101`) and its build
  environment adding each keg-only dependency's `opt_include` and `opt_lib` (`extend/ENV/super.rb:261`,
  `:287`); clang only through the sysroot, `-isysroot X` reaching the compile as `-isysroot X` and
  ld as `-syslibroot X` (`clang -###`), while `CPATH` adds `-I` to the compile and nothing to the
  link and `LIBRARY_PATH` adds a link search path and nothing to the compile (`clang -v`,
  `-Wl,-v`).

  **The rules judged on the builds** (`tools/pair.py`, `q3/pairing.tsv`): for 11 builds, the
  header clang read (the unit's `main.deps.txt`) and the library file ld64 loaded (`clang -Wl,-t`
  with the build's search words). (P1), both halves from folders the build names or neither, and
  (P2), one prefix after `realpath` (`<p>/include`, `<p>/lib`), give the same verdict on all 11:
  pass the three right pairings (the SDK's halves, `$SDK/usr` both; both keg halves named;
  `pkgconfig`), refuse the eight mismatches, curl's and readline's, both directions. Three
  limits, each measured: (P2) must read the file the linker loaded, `$SDK/usr/lib/libcurl.tbd`,
  since the install name `otool -L` shows, `/usr/lib/libcurl.4.dylib`, has another prefix; both
  must judge **the header that declares a name the library supplies**, never the group's own
  header, since `examples/curl` with `curl_bound.h` moved to `vendor/` and `--include vendor`
  is a right pairing (8.7.1 with the SDK's `curl.h`) that a group-header reading refuses; and both
  refuse curl's SDK header with the keg's library, which curl's ABI promise covers and readline's
  same shape does not. (P3) fails as a general check here: the default `libcurl.pc` is Homebrew's
  shim for the OS (`os/mac/pkgconfig/27/libcurl.pc`), naming the CommandLineTools SDK 27.2's
  `include` and `/usr/lib`, a third folder from the Xcode SDK clang read (its `curlver.h`
  byte-identical), the keg's `.pc` is off the default path, and readline has none on it. The
  loader is past every build rule: the paired `--include`+`--library` build run with
  `DYLD_LIBRARY_PATH=/usr/lib` prints `530176` beside `libcurl/8.7.1`, exit 0.
- `argument`: A header and a library carry no shared identity in C, so no rule knows a release;
  what the build can know is where each half came from. On this Mac both readings that need no
  library knowledge, provenance and prefix, separate every right pairing from every mismatch, and
  a mismatch faults: readline's record read 8 bytes past a 1-byte allocation at exit 0. Their
  price is curl's supported old-header pairing, refused with a remedy of one flag; robustness
  ranks the fault first. The rule holds at the link only; the loader stays outside it.
- `prediction`: with (P1) per name, the `--library /opt/homebrew/opt/readline/lib` build of
  `q3/rl` is refused before it links, and the same program under `pkgconfig "readline"` builds and
  prints `42`; on Linux arm64 a Debian `/usr/include` with `/usr/lib/aarch64-linux-gnu` still
  passes (P2) only if the rule reads `<p>/lib/<triplet>` as `<p>/lib` (unrun).
- `condition`: a right pairing, on a platform this project builds on, whose declaring header and
  supplying library come from different provenance or prefix by design (Nix's split outputs,
  Windows Kits' `Include\` and `Lib\`, both unrun); or a measured mismatch neither reading sees.
  The rule's judgement is built and run as an outside judge over 11 builds; its mechanism in the
  compiler (the declaring file of each supplied name, beside panel 239's link trace) is unbuilt.

## Found beside, not repaired

- **No new defect filed by this seat.** One shape looked like one and is not: inside the region
  the `-Wall` ignore turns off clang's default error `-Wimplicit-int` (route trial, `kept` row),
  but the build refuses such a header all the same (`beside/krint`: `ffi_header_refused`,
  *"parameter 'x' was not declared, defaults to 'int'"*, from a check compiled outside the
  region; bound at `i64` it is `ffi_parameter_type`). Beside that refusal clang's raw
  `-Wdeprecated-non-prototype` is printed, which is Q1's class (2464), not a new one.
- A question for the coordinator, not a premise: a `pkgconfig "libcurl"` group with no
  `PKG_CONFIG_PATH` takes `-I` from the CommandLineTools SDK while clang's sysroot is Xcode's, two
  SDKs in one unit; identical `curlver.h` here, so no wrong value measured.

## Summary of verdicts

| question | verdict | the measurement it rests on |
|---|---|---|
| Q1 | object to the lane's route; approve its two deprecation words; veto (a0) and (c); recommend the region fail closed, built as the prototype's list and measured as (d) | curl 8.23.0's checker, the one live `warning` attribute in 139 kegs, silenced by the lane's route; nine UB doors at exit 0; (c) loses every kept error; 0 of 1,066 Homebrew headers print any word |
| Q2 | approve: no variadic bound directly, `-Wformat-security` raised in the region; object to a `format`-keyed rule | SIGSEGV from `sqlite3_config`/`sqlite3_mprintf`, a kernel write from `ioctl`, mode `0450` from `open`; 0 tracked programs refused; 1 tracked case (2036's `printf(m)`) moves |
| Q3 | object to the base; `blocking` by measurement; approve (P1)+(P2) per declaring header, and (P4); object to (P3) | ASan heap-buffer-overflow and `0` for `42` at exit 0 under `--library` alone; both readings right on 11 of 11 builds |
