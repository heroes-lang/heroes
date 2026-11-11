# Panel 245, the completeness critic's second pass, over the reports

Written 2026-11-11 from 00:39 to about 01:35, as the work went. No verdict is given here. Read
whole first: every file of `245-briefs/` and `245-reports/`, my first pass included.

**The compiler.** The base `7fb6868b2` was archived into
`.claude/worktrees/p245-scratch/critic/second/tree`. Then:
- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 12.69`;
- `./heroes build selfhost/main.hero -o heroes-next`, `real 178.50`, `user 115.00`.

Batch 30's gate B was running `run` four wide at the time, so neither figure is a duration.
`heroes-next --version` prints `heroes 0.6.0` and `sources 0fe313ba58ba197c`, the key every seat's
compiler printed.

**Where things are.** Every probe is under `p245-scratch/critic/second/` (`q1/`, `q1curl/`,
`beside/`, `q2/`, `q3/`). The seats' scratch folders were read and never run in or built in.

**Rules kept.** No paid run, no GitHub write, no credential. One lapse: a redirect of mine wrote
`/tmp/claude-501-wall.txt` outside the repository's root. I removed it at once (`ls` then said
*No such file*). Every platform sentence below is this Mac's: Apple clang 21.0.0, with Homebrew's
`llvm@22` 22.1.8 used only for `diagtool`.

Each item gives the fact, the command, what it printed, and the route it bears on.

## A. Q1, defect 2464

1. **Route E fails open: a default error of plain clang, and seven words raw at the base, become
   silent at exit 0.**
   - Command: `q1/run.sh <region> <shape> <level>`. It writes the unit's region as each route writes
     it, includes one header and compiles with `cli/flags.hero`'s words and `-c`. Four regions were
     run: the base; E as `routes/E.diff` writes it (`ignored "-Weverything"`, then `KEPT`,
     `checks_raised.checked()` but `-Wsign-conversion`, `-Wimplicit-int`, its eight `UNDEFINED`
     words and the two relayed); the ffi-pragmatist's prototype as `prototype.diff` writes it; and
     route (d). There were 17 shapes at `-O0` and `-O2`, and the output is `q1/table.txt`.
   - Printed, plain `clang -std=gnu11 -c`: `atomic_access` (`_Atomic` struct member written) exits 1,
     `-Watomic-access`, a default error. C calls it undefined behaviour.
   - Printed, base region: the same shape exits 1. Raw at exit 0: `-Winteger-overflow`,
     `-Wshift-negative-value`, `-Wshift-count-negative`, `-Wvarargs` (`va_start` on a parameter
     that is not the last), `-Wfree-nonheap-object` (`free` of a local), `-Wstrncat-size` and
     `-Wincompatible-library-redeclaration`.
   - Printed, E: all eight exit 0 with nothing printed.
   - The compiler-engineer's own condition for E was *"a default error clang gives outside E's list,
     so that a header plain clang refuses builds under E"*, which *"would move me to (b) with the
     relay"*. It is met.
   - Their *"I could not enumerate default errors on this Mac (`xcrun -f diagtool` fails)"* has an
     instrument: `/opt/homebrew/Cellar/llvm@22/22.1.8/bin/diagtool show-enabled` lists 57
     default-error flags. Its C ones outside E are `-Watomic-access`, `-Winvalid-gnu-asm-cast`,
     `-Wargument-outside-range`, `-Winitializer-overrides`' union case,
     `-Wunderlying-atomic-qualifier-ignored` and `-Wattribute-statement`. That is clang 22's list,
     not Apple clang 21's.
   - Bears on: Q1, E against (b) and (d).

2. **An enumerated list leaves the class open, the ffi-pragmatist's own prototype included.**
   - Same table, the prototype's region at `-O0`.
   - Printed: six shapes raw at exit 0 that no seat listed: `-Wfree-nonheap-object`, `-Wvarargs`,
     `-Wshift-count-negative`, `-Wstrncat-size`, `-Wincompatible-library-redeclaration` and
     `-Winitializer-overrides`.
   - This is the ffi-pragmatist's own argument, *"any deny list rots with clang"*, measured on the
     list they built.
   - Bears on: Q1, (b) and the prototype against (d).

3. **Route (d) refuses everything above, with four exceptions.**
   - Same table, region (d): the base region with the two deprecation words quieted, and `-Werror`
     on the unit's line.
   - Printed: every shape exits 1 except four.
     - The deprecated name builds silent, by design.
     - `#pragma message` stays a raw warning at exit 0. This confirms the ffi-pragmatist's *"clang
       never upgrades it"*.
     - `diagnose_if` is silent (item 4).
     - The `warning`-attributed call is refused at `-O0` and builds at `-O2`, where it is optimised
       away (item 9).
   - `#warning` is refused under (d). So defect 248's relay needs the allow-list word the
     ffi-pragmatist names.
   - Bears on: Q1, (d).

4. **The region's `-Wall` ignore reaches two words that no route raises again.**
   - Command: `diagtool tree -Wall` (`q1/wall-tree.txt`).
   - Printed: `-Wall` holds `-Wimplicit-int`, a default error, and `-Wuser-defined-warnings`, the
     word of `__attribute__((diagnose_if(..., "warning")))`, under `-Wmost`.
   - In the table, `diagnose_if` is silent at exit 0 at the base and under the prototype and (d).
     E silences it with everything else.
   - Live instances on this Mac, by `grep -rln diagnose_if` over the SDK's `usr/include` and
     Homebrew's include trees, at the `"warning"` level: none in C. libc++'s `__config` and
     Abseil's `macros.h` are C++. Ruby 4.0.9's `scan_args.h:392-393` checks `rb_scan_args`'s format
     against its variadic count with `diagnose_if` at `"error"`, which no pragma quiets.
   - Whether glibc's or Bionic's clang fortify speaks through `diagnose_if` is a question: glibc's
     `cdefs.h`, read 2026-11-11 (item 5), holds no `diagnose_if`.
   - Bears on: every Q1 route. A fail-closed region must raise `-Wuser-defined-warnings` again after
     the `-Wall` ignore, and the `-Wimplicit-int` half is item 10.

5. **glibc's fortified `fgets` warning does not reach a clang build, as quoted.**
   - Commands: `clang -dM -E -x c /dev/null` (Apple clang 21 and Homebrew's 22), then glibc's own
     `__GNUC_PREREQ (4,3)` preprocessed by clang.
   - Printed: `__GNUC__ 4` and `__GNUC_MINOR__ 2` from both, and `int gnuc43 = 0;`.
   - glibc's `misc/sys/cdefs.h` on codebrowser.dev was read 2026-11-11; the page names no glibc
     version. It defines `__warnattr(msg)` as `__attribute__((__warning__ (msg)))` under `#if
     __GNUC_PREREQ (4,3)`, and as nothing under its `#else`. The page has no `diagnose_if` and no
     clang fortify macro.
   - So the historian's row 1.19, and the ffi-pragmatist's use of it, describe a GCC build. Under
     clang that attribute is empty. The historian's own row marked the clang half unread; it is now
     run. Linux is unrun.
   - Bears on: the `-Wattribute-warning` contradiction (item 6).

6. **The live `warning` attribute, curl's checker, marks a wrong result at exit 0, and the result
   moves with the level.**
   - Command: `q1curl/t.c`, compiled against keg curl 8.23.0. It has two `noinline` doors around
     `curl_easy_setopt(h, CURLOPT_MAXREDIRS, v)`, one passing an `int32_t` and one a `long`, each run
     with `-2`, `-5` and `7`.
   - Printed at `-O0`, with `-Wattribute-warning` printed at build: `int32 -2 -> 0, long -2 -> 43`
     and `int32 -5 -> 0, long -5 -> 43`. curl accepts the `int` and refuses the `long`
     (`CURLE_BAD_FUNCTION_ARGUMENT`). It read another value past `...`.
   - Printed at `-O2`: `43` both ways.
   - The contradiction the brief names: the lane and E quiet `-Wattribute-warning`, while the
     ffi-pragmatist and the historian raise it. This command supports raising it on robustness. The
     curl half is a measured wrong value and the glibc half is item 5's.
   - Bears on: Q1.

7. **Raising `-Wattribute-warning` gives a false order note, in the ffi-pragmatist's prototype too.**
   - The ffi-pragmatist did not report this.
   - The order trial is `clang -fsyntax-only` (`cli/order_trial.hero:106`).
   - Command: an `error` pragma on each word, the same unit compiled with `-fsyntax-only` and with
     `-c`.
   - Printed for `-Wattribute-warning`: 0 errors at `-fsyntax-only` and 1 at `-c`, for the toy at
     `-O0` and for curl's checker. `-Wshift-count-overflow`, `-Winteger-overflow`, `-Wstrncat-size`,
     `-Wfree-nonheap-object` and `-Wvarargs` print 1 and 1.
   - The prototype's own saved refusal of curl's checker,
     `ffi-pragmatist/proto/curl-checked/e2-heroes-proto-O0.txt`, ends with *"`curl_bound.h` compiles
     after `stdio.h`, a header of C's own it reads itself ... write a group naming `stdio.h` above
     this one"*. That is the note the compiler-engineer measured as false under *told*. I read it;
     I did not rerun it.
   - Bears on: Q1. Any route raising that word owes a code-generating trial for it.

8. **Route (d)'s mechanism reaches no probe or trial.**
   - Command: `grep -n 'function without_diagnostics'` in `cli/compiling.hero`.
   - Printed: `:207-214` keeps every word that does not start with `-W`. `probe_words`, which the
     probes and order trials compile under (`:160-161`, `cli/dump_meaning.hero:29`), is
     `without_diagnostics(unit_words(c))`.
   - So a `-Werror` on the unit's line never reaches them. Every word only `-Werror` makes an error
     would refuse in the unit while the trials find an order that "compiles": item 7's note,
     multiplied. That comes from reading the source, and (d) is unbuilt in the compiler.
   - Bears on: Q1, the price of (d), which no seat named.

9. **A refusal that moves with the level.**
   - Table, the `warning`-marked `static inline` call: refused at `-O0` and built at `-O2` under the
     prototype and (d). The ffi-pragmatist measured the same.
   - `grep -n -i` of `docs/design.md` for *"at every level"*, *"same verdict"*, *"level.*verdict"*
     and *"-O2.*refus"* finds no rule that a program refused at one level is refused at every level.
     That rests on my vocabulary, so it is a question.
   - Bears on: Q1, raising `-Wattribute-warning` (item 6). Neither the reports nor the brief asked
     this.

10. **The compiler-engineer's found-beside false note reproduces, and leads into a loop. The
    ffi-pragmatist's "no new defect" is about another shape.**
    - Command: `beside/ce-ii` (their `own.h`: `static implicit_x = 3;`), built with my compiler.
    - Printed: exit 1, `ffi_header_refused`, note *"`own.h` compiles after `stdio.h`, a header of
      C's own it uses and does not include: write a group naming `stdio.h` above this one"*.
      `own.h` uses nothing of `<stdio.h>`.
    - Followed in `beside/ce-ii-followed`, with a `stdio.h` group above: exit 1 again, and a second
      note, *"these compile when `own.h` is included before `stdio.h`, which the group at line 1
      names ... write this group above that one"*. The two notes send the author back and forth.
    - The ffi-pragmatist's `beside/krint` is a K&R parameter, `plus_one(x)`. My compiler prints
      their `ffi_parameter_type` for it, so it does not test this shape.
    - Unfiled. A false message is `blocking` by `.claude/rules/verification.md` § Bounded discovery.
      Its cause is item 4's `-Wimplicit-int` inside `-Wall`.

11. **Defect 158's carriers: both seats are right.**
    - `grep -n deprecated tests/golden/unsupported/fixedbugs-158-noted-*.h`.
    - `noted-outer.h:5` declares `fixedbugs_158_old __attribute__((deprecated))`, and `noted-mid.h`
      is its use.
    - A route that quiets deprecation in the region blinds
      `fixedbugs-158-a-note-in-another-header-between` through that pair. The compiler-engineer's
      one-line repair of the carrier is unrun as a case.

## B. Q2, defect 2465

12. **The compiler-engineer's *"the door its note names ... builds at exit 0 with the very fault"*
    is contradicted by the note's own text and by a run.**
    - Their `var` note (`routes/var.diff:43`, `q2cases/out-var/open_ro.err`) says to write a door
      *"taking `flags` with every argument `open` reads written there"*, and *"where `open` reads a
      format, write the format inside that function as a literal"*.
    - Their two doors go against it: `open_c(p, fl) { return open(p, fl); }` hands no mode, and
      `mp(f)` hands `sqlite3_mprintf` a run-time format.
    - Built with my compiler: their door made `made2.txt` `--wx------`. The door the note describes
      (`q2/note-door`: `open(p, fl, (mode_t)mode)` with `420`, and `sqlite3_mprintf("n=%d", n)`)
      printed `3 7` and made the file `-rw-r--r--`.
    - The ffi-pragmatist's note says the same (*"taking every value `open` will read"*), and their
      `open3` and `sq_number` measured the same.
    - So D's residue is C written against D's note: the C contract inside a door, which no route
      reaches. The other half of the objection stands: `open(p, O_RDONLY)` is correct and D refuses
      it, and the repair is a door.
    - Bears on: Q2, D against the C contract.

13. **D's notes cite spec § 13 for a clause § 13 does not hold, and so does the base's.**
    - Command: `sed -n '373,$p' spec/heroes-spec.md | grep -c 'header declares no type\|declared
      variadic\|never bound'`. Printed `0`.
    - The base's `emit/ffi_past_prototype.hero:220` and `:230` cite *"spec § 13: ... past a
      header's `...` the header declares no type"*. That reproduces the spec-warden's found-beside
      item 1 by grep.
    - The prototype's D note cites *"spec § 13: ... a function its header declares variadic is never
      bound"*. The compiler-engineer's `var` note cites *"spec § 13: past a header's `...` nothing a
      call hands is held to a type"*.
    - The contradiction: the spec-warden approves D at +0 and objects to W0 landing unpaid, while D
      as both seats built it needs W0 (+27 vendored, a lower bound) for its note to be true, or a
      note that cites no clause.
    - The base's two citations are a false message today, unfiled.
    - Bears on: Q2's spec cost, and a defect to file.

14. **`-Wformat-security` raised in the region: no seat contradicts it.**
    - The three Bash seats measured the same narrowed run, 2036's `printf(m)` case refused, 3 passed
      and 1 failed.
    - The ffi-pragmatist's census let that word speak: 0 of 1,066 compiling Homebrew headers.
    - The compiler-engineer's prediction, *"the whole `run` form fails on exactly one case"*, is
      unrun whole. Glibc's own fortified inline code under it is unrun (Linux).

15. **An inference written as a measurement.**
    - The ffi-pragmatist: *"`ioctl(0, FIONREAD)` returns `0`, so the kernel wrote an `int` through
      an address read from the `va_list`"*. What was measured is the `0`. The *so* is CLAUDE.md
      § RUN IT's tell. I did not rerun it.
    - `sqlite3_config` and `sqlite3_mprintf("n=%s")` dying of SIGSEGV are measured faults and carry
      the class alone.

## C. Q3, defect 2448

16. **2448's harm is a measured memory fault, and it reproduces with my compiler.**
    - Command: the ffi-pragmatist's `hist.h` and `main.hero` copied to `q3/rl`, built four ways, each
      run under `timeout 10`.
    - Printed, in the order *size, header, library, no-data is null, first byte, own record back*:

      | build | prints | linked |
      |---|---|---|
      | plain | `16 1026 1026 1 -1 42` | libedit |
      | `--library $R/lib` | `16 1026 2051 0 0 0` | `libreadline.8.dylib` |
      | `LIBRARY_PATH=$R/lib` | `16 1026 2051 0 0 0` | `libreadline.8.dylib` |
      | `pkgconfig "readline"` | `24 2051 2051 1 -1 42` | `libreadline.8.dylib` |

    - Every build exited 0.
    - `--library` with `--sanitize`: exit 134, *"AddressSanitizer: heap-buffer-overflow ... READ of
      size 8 ... 0 bytes after 1-byte region"* at `hist.h:39`, the region *"allocated by ...
      replace_history_entry+0x80 (libreadline.8.3.dylib)"*.
    - So the `blocking` class rests on a measured fault, and § Bounded discovery's list names it.
    - The fault's direction is the reverse one (library named, header not), which nothing refuses.
      The spec-warden found no wrong value because their candidate, ncurses, was the forward
      direction, refused `shadowed_header`.
    - Bears on: the class line, and on P1.

17. **The ffi-pragmatist's P1 column was written by hand, not judged. Computed, it agrees.**
    - `tools/pair.py` sets `p1 = 'pass' if named_h == named_l else 'refuse'`, with `named_h` and
      `named_l` passed as literals per row. Only P2's prefix is computed from the measured paths.
    - Command: `q3/p1.py` over their `pairing.tsv`. It takes the header clang read and the library
      the linker loaded, both measured, and asks whether each lies under a folder the row's own
      flags, environment or `pkg-config` answer name.
    - Printed: 11 rows, `agree` on all 11.
    - So *"both readings right on 11 of 11 builds"* holds, and P1 had no outside judge until this run.
    - Bears on: Q3, P1.

18. **The ordinary curl binding, `pkgconfig "libcurl"` with no `PKG_CONFIG_PATH`, was not among the
    11. It pairs.**
    - The compiler asks `pkg-config --cflags --libs` (`cli/libraries.hero:291`), and Homebrew's shim
      answers `-lcurl` alone.
    - Built (`q3/shim`): `header 526081`, `library libcurl/8.7.1`. The unit's deps name the Xcode
      SDK's `curl.h`, and the link takes the same SDK's `libcurl.tbd`. P1 and P2 both pass.
    - The ffi-pragmatist's question about *"two SDKs in one unit"* does not reproduce.

19. **On this Mac the environment names a library folder in every build, an empty entry
    included.**
    - Command: `printenv LIBRARY_PATH` printed `:/opt/homebrew/lib`, set by the author's shell
      profile. `printenv CPATH` and `printenv SDKROOT` printed nothing, so they are unset.
    - So every seat's "plain" row ran with it. No report says so.
    - `clang -Wl,-v` lists `.` first among the library search paths, from the empty first entry.
    - Measured in `q3/dot`: a `libcurl.dylib` I compiled into the build's folder was linked in place
      of the system's, at exit 0, printing *"library a stray libcurl in the working folder"*, with
      `otool -L` showing `@rpath/libcurl.dylib`. That is 2448's reverse direction, reached by
      nobody's word.
    - The base reads no `LIBRARY_PATH` (the compiler-engineer's grep). A P1 that reads the link
      job's `-L` folders would see it.
    - Bears on: Q3. The rule must say whether the environment's folders, `.` among them, count as
      named.

20. **A third instance of 2448's class: a library the SDK ships without its header.**
    - `comm` of `/opt/homebrew/lib` and the SDK's `usr/lib` prints `lzma pcre2-8 pcre2-posix`. The
      SDK has no `lzma.h` or `pcre2.h`, and `/opt/homebrew/include` has both.
    - Command: `q3/lzma`, `extern "lzma.h" link "lzma"`, built with `--include
      /opt/homebrew/include` under `env -u LIBRARY_PATH`.
    - Printed: exit 0, `header 50080042` (xz 5.8.4), `library 50040032`, linked
      `/usr/lib/liblzma.5.dylib`.
    - With the profile's variable set, the same build pairs `50080042` with `50080042` by accident.
    - P1 and P2 both refuse the unpaired build. No wrong value was measured from it.

21. **P2 against P1: no divergence on this Mac.**
    - Across 13 builds (11, the shim, and lzma), P1 and P2 give the same verdict.
    - P2's premise fails off this Mac: Debian multiarch, by the ffi-pragmatist's own prediction, and
      Nix's split outputs, by the historian's row 3.9. Both are unrun.
    - That is the compiler-engineer's and the spec-warden's objection, *"asks the world"*, still
      standing.

22. **P1 judged per supplied name: a question no seat priced.**
    - The ffi-pragmatist's limit, *"judge the header that declares a name the library supplies,
      never the group's own header"*, is what keeps `examples/curl` with `curl_bound.h` under
      `--include vendor` passing.
    - The compiler-engineer's 200 to 250 lines thread the header's provenance from
      `shadowed.checked`, not each name's declaring file.
    - Whether the dump already gives that file (`cli/header_types.hero:9`, `-ast-dump=json`) is
      unmeasured.

23. **A path in the shared brief.**
    - `ls selfhost/cli/head_names.hero` fails, and `selfhost/head_names.hero:222` holds the note.
    - `00-shared.md:144` says `cli/`. The compiler-engineer and the spec-warden are right. My first
      pass's item 18 named the file without its folder.

## D. Across the sitting

24. **Predictions no instrument here can score.**
    - The spec-warden's W0 (*"at most 3 of 10 blind readers"*) needs a paid run of panel 217's
      launcher.
    - The ffi-pragmatist's multiarch prediction and the historian's Q1 condition (glibc's `fgets`)
      need Linux, which means the CI's legs after a push.
    - The ffi-pragmatist's (d) prediction needs (d) in the compiler.
    - The compiler-engineer's Q1 prediction (465 emissions) is scoreable at a gate, but the condition
      beside it is met (item 1).

25. **Questions the sitting should have asked.**
    - May a refusal move with the optimisation level (item 9)?
    - Must the trials that write a refusal's notes compile under the unit's words, code generation
      included for a code-generation word (items 7 and 8)?
    - Does the region's `-Wall` ignore keep off two words that are not advice (item 4)?
    - Does D owe W0 for its own note, and is the base's citation a defect to file now (item 13)?
    - Do the environment's library folders, the empty entry `.` among them, count as named (item
      19)?

## The three routes

**Q1.**
- *Most robust and complete:* a fail-closed region, route (d). In a header's own code every warning
  clang gives is a refusal, except an allow-list of advice: the two deprecation words, `#pragma
  message`, and `#warning` relayed as defect 248 pins.
- *What it owes:*
  - `-Wuser-defined-warnings` and `-Wimplicit-int` raised again after the `-Wall` ignore (items 4
    and 10);
  - the probes and order trials compiled under the same words (item 8);
  - a code-generating trial for `-Wattribute-warning`, and a ruling on the level (items 7 and 9);
  - defect 158's carrier line (item 11).
- *Evidence that decides it:*
  - as emulated, (d) refuses every one of the 17 shapes at `-O0` except the deprecated name,
    `#pragma message` and `diagnose_if`, `#warning` included until it is allow-listed (item 3);
  - E silences a default error and seven words that were raw at the base, at exit 0 (item 1);
  - the enumerated prototype leaves six raw (item 2);
  - curl's checker marks a measured wrong result (item 6);
  - (c) loses every kept error, measured by three seats.
- *What it rests on:* it is built at the C level only, by the ffi-pragmatist and here, and is
  unbuilt in the compiler.
- *Conservative alternative:* the ffi-pragmatist's prototype, built and run in the compiler. That is
  the deprecation words quieted, a named list of undefined-behaviour words made errors with
  `-Wattribute-warning` among them, and `FORMAT_DEFINED` passed over again. It leaves the six words
  of item 2 raw, a test is owed for when clang's list moves, and it still owes items 7 and 9.
- *Not either:* E, refuted by its own condition (item 1), and the lane's three words, which quiet
  curl's measured wrong value (item 6).

**Q2.**
- *Most robust and complete:* `-Wformat-security` raised in the region, and no variadic bound
  directly whatever stands past its `...` (S and D). The ffi-pragmatist built both in one compiler.
- *Evidence that decides it:*
  - the faults at exit 0 measured by three seats for unmarked direct bindings (`open` with
    `O_CREAT`, `sqlite3_mprintf`, `sqlite3_config`, `ioctl`);
  - 0 tracked programs refused (the compiler-engineer's grep, the ffi-pragmatist's enumeration);
  - the door the note describes measured correct (item 12).
- *What it owes:* the notes' § 13 citation made true by W0, queued for the author, or cut, and the
  base's own two citations repaired either way (item 13); defect 2036's `printf(m)` case rewritten
  as `printf("%s", m)` or pinned as a known cost.
- *Its cost:* `open(p, O_RDONLY)` is refused and needs a one-function door. A door that disobeys its
  note keeps C's contract, and no route reaches that.
- *Conservative alternative:* S alone, with the C contract for unmarked callees stated in design.md
  Part 7 row 10 (the compiler-engineer's). It keeps a direct `open(path, flags)` creating
  `-r--r-x---` and `sqlite3_mprintf` reading garbage, each at exit 0.

**Q3.**
- *Most robust and complete:* P1, a refusal by provenance judged per supplied name, beside P4's note
  naming both halves.
  - The header that declares each name the library supplies, and the library file the linker loaded
    for it, must both come from folders the build names, or neither.
  - The folders the environment names count: `LIBRARY_PATH` read from clang's link job, its empty
    entry `.` among them (item 19).
  - It is a refusal and not a warning, by design.md:4336's *"exit 1 or nothing"*.
- *Evidence that decides it:*
  - the reverse direction faults (item 16);
  - P1 computed from measured paths agrees on 11 builds, and passes the shim and the paired lzma
    builds (items 17, 18 and 20);
  - P2 rests on where packagers put files (item 21);
  - P3 measured null on the reproducer (the compiler-engineer and the ffi-pragmatist);
  - P4 was built with 3 goldens and is unanimous.
- *Its price:* curl's supported old header with its newer library is refused, with a one-flag
  repair, and so is a header-only folder named beside a platform library.
- *What it rests on:* it is unbuilt in the compiler. The compiler-engineer estimates 200 to 250
  lines, and the per-name reading is unpriced (item 22). The loader stays past it: the paired build
  run under `DYLD_LIBRARY_PATH=/usr/lib` prints 8.23.0's header beside 8.7.1, at exit 0.
- *Conservative alternative:* P4, plus P1 in the measured fault's direction only. It would refuse a
  library a build names while that library's names are declared by a header it does not name,
  leaving the forward direction to the stand-in rule. That keeps lzma's forward pair (item 20) and
  curl's direct-binding forward pair at exit 0.
