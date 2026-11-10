# Panel 243, the completeness critic's second pass, over the reports

Written 2026-11-10 from about 20:20, as the runs went. Read whole: every file of `243-briefs/`
(the four seats' briefs, their `.before-critic` copies, `00-shared.md` and its copy, both critic
briefs) and of `243-reports/` (the compiler-engineer, the ffi-pragmatist, the spec-warden, the
historian, and my first pass). Probes in `.claude/worktrees/p243-scratch/critic/second/`. Compilers
run, never built in a seat's worktree: `b29-round/heroes-next` (`sources 14e9e5ce168a2336`, `main`'s),
and the seats' own route binaries run from my folder on my copies of their probes
(`243-compiler-engineer/heroes-{r0,q2a,q2b,q3L,q3LS}`, `243-ffi-pragmatist/heroes-proto`). No paid
run, no credential, no GitHub write, no file outside the repository's root; the machine read load
average 164 at 20:23 with the compiler-engineer's whole `run` form (pid 64367) and other sessions'
work on it, so nothing here is timed.

Complete at about 20:47: twenty items, then the three routes.

## Items

1. **P2 (the spec-warden) and the compiler-engineer's prediction miscount `twoarity-lent`, and (R0) turns a pinned `surface` row red that nobody named.**
   - **Ran:** `heroes-next build` and `heroes-r0 build` of copies of `surface-fixtures/twoarity/` and `twoarity-lent/` (`second/surf/`); `grep -n twoarity tests/harness/*.hero`.
   - **Printed:** `twoarity-lent` is **exit 1 today** (`ffi_writable_parameter` on `as_text.hero`), so it is not a program "from building to refused". `suite_surface.hero:688` pins it with `.err_lacks("as_int.hero")`. Under `heroes-r0` its output names `twoarity-lent/as_int.hero:12:40` (one hit, against 0 at the base), so **row 688 goes red** under every (R0) scope. `twoarity` (row 687, `out_is("n = 42\n…")`) is exit 1 under `heroes-r0`.
   - **Bears on:** P2 should read *three programs from building to refused (`run/ffi-variadic-at-a-fixed-arity`, `run/fixedbugs-584`, `surface-fixtures/twoarity`), and `surface` rows 687 and 688 rewritten*. My first pass's item 12 listed `twoarity-lent/as_int` among "the tracked by-value integers" without saying it is already refused; that was imprecise. The compiler-engineer's census says this correctly ("adds a second error"); its prediction counts five programs under the whole scope and should add the row-688 assertion.

2. **(R0)'s door closes `twoarity`'s class only if the door fixes the format, and nothing checks that it does.**
   - **Ran:** `second/fwddoor/`: `say_own.h` defining `static inline int say_any(const char *f, long long v) { return printf(f, v); }`, bound `say_any(f: cstr lent, v: i64)`, called through `say(format: "n = %d\n", value: 4294967338)`, built with `heroes-next` and with `heroes-r0`.
   - **Printed:** both **exit 0, `n = 42`**. (R0)'s new note (read whole from `twoarity`'s output) says *"defines a `static inline` function of a new name taking `value` at the C type `printf` reads there"*. It does not say to put the literal format in the door. The ffi-pragmatist's 1.7 door and the compiler-engineer's rewrite of `ffi_call.hero:148` do put it there.
   - **Route nobody listed, measured in C:** keep `-Wformat-nonliteral` as an error in the program's own header code, beside the `-Werror=format` already kept there (`cli/flags.hero:141`, `header_region.hero` `KEPT`). `clang -fsyntax-only -Wformat-nonliteral` flags `say_own.h:2:71`, *"format string is not a string literal"*. It is silent on a forwarder marked `__attribute__((format(printf, 1, 2)))` that hands on to `vprintf`.
   - **Its cost on the tree:** the tracked C that passes a non-literal format is two headers. The command was three `git grep -nE` by the format's position (`printf`/`vprintf` first, `fprintf`/`sprintf` second, `snprintf` third), seed and `docs/` out, single-line calls only. The two headers:
     - defect 2036's `run` header, `printf(m)` with no argument: clang flags it `-Wformat-security`, not `-Wformat-nonliteral`, so the case keeps building;
     - defect 2034's attribute-marked `say`: exempt by its attribute.
   - **Unbuilt in Heroes. Unrun:** Linux and the box; the runtime under the flag.
   - **Bears on:** Q1. (R0)'s note must name the literal format in the door. The flag is what makes that more than advice.

3. **The two seats contradict each other on Q2, and the run sides with the ffi-pragmatist: the compiler-engineer's (a), as built, admits both shapes the ffi-pragmatist vetoed.**
   - **Ran:** `second/q2r/`, copies of the ffi-pragmatist's `pcstdio` and `pcsocket` (its `.pc` files read where they stand). Each built with four compilers, one at a time: `heroes-next` (base), `243-compiler-engineer/heroes-q2a` (`sources 7e9664a16870782d`), `heroes-q2b` (`3505b45302b0869c`) and `243-ffi-pragmatist/heroes-proto` (`3f1db37e310b5a55`).
   - **Printed:**

     | shape | base | (a) | (b) | step 3 |
     |---|---|---|---|---|
     | `pcstdio` (`own.h` writing `<stdio.h>`, a `.pc` folder holding `stdio.h`, `BUFSIZ 7`) | 1 | **0, prints `7`** (the SDK's is 1024) | 1 | 1 |
     | `pcsocket` (the same with `sys/socket.h`, `AF_INET 99`) | 1 | **0, prints `99`** (the SDK's is 2) | 1 | 1 |

   - **What it settles:** the compiler-engineer writes that (a) "admits no class the direct form does not", from its `p243ov` overlay row. The ffi-pragmatist vetoes route (c) without its exception on exactly these two programs. Its veto, by its own words, reaches (a) as built.
   - **Bears on:** Q2. (a) is admissible only with the ffi-pragmatist's exception, or with another guard on a platform name. The two seats should be told that the measurement contradicts one of their sentences.

4. **The coordinator's readline fact, from panel 244: half of it reproduces, half of it reverses.**
   - **Ran:** `second/readline/`, copies of panel 244's `e8/own` (a wrapper `rl.h` holding `<stdio.h>` and `<readline/readline.h>`, `link "readline"`) and `e8/own2` (`readline/readline.h` named directly, `link`). Also `own3` (named directly, `pkgconfig "readline"`) and my own `own4` (the wrapper, `pkgconfig "readline"`). Each built with `heroes-next`, run, and read with `otool -L`. The keg's answer: `PKG_CONFIG_PATH=/opt/homebrew/opt/readline/lib/pkgconfig pkg-config --cflags --libs readline` printed `-I/opt/homebrew/Cellar/readline/8.3.6/include -D_DARWIN_C_SOURCE -L…/lib -lreadline`. The system's `pkg-config` knows no `readline`, and the SDK holds `libreadline.tbd` beside `libedit*.tbd`.
   - **Printed:**

     | build | exit | prints | links |
     |---|---|---|---|
     | direct, no keg | 0 | `1026` | `/usr/lib/libedit.3.dylib` |
     | direct, `CPATH=/opt/homebrew/opt/readline/include` | 0 | **`2051`** | **`/usr/lib/libedit.3.dylib`** |
     | direct, the keg's `.pc` | 0 | `2051` | `/opt/homebrew/opt/readline/lib/libreadline.8.dylib` |
     | wrapper, no keg | 0 | `1026` | libedit |
     | wrapper, `CPATH` | 1 `shadowed_header` | | |
     | wrapper, the keg's `.pc` | 1 `shadowed_header` | | |

     Both refusals' notes read *"rename the file, or move it out of `/opt/homebrew/Cellar/readline/8.3.6/include`"* (the `.pc` build) or *"… out of `/opt/homebrew/opt/readline/include`"* (`CPATH`).
   - **What reproduces:** the wrapper is refused under `CPATH` and under `pkgconfig`, with the move-it-out-of-the-Cellar note (the compiler-engineer's false-remedy finding, a second library). The direct binding prints `2051`.
   - **What reverses:** "without `CPATH` it prints `1026`, libedit's header against GNU readline's library" does not reproduce. Without `CPATH`, `1026` comes from **libedit's header with libedit's library**, a consistent pair. The mismatched pair is the **direct group under `CPATH`**: GNU readline 8.3's header against libedit's library, exit 0, accepted today. Panel 244's own `own2/run.out` (`2051`) is that build. That is the ffi-pragmatist's § 6 item 2 (`--include` pairing curl 8.23.0's header with 8.7.1's library), found on a second library through a second arm, `CPATH`.
   - **Which Q2 route closes or opens it** (`second/q2r/rlpc`, `rlcpath`, `rldirect`, the four compilers of item 3):
     - the wrapper over the keg's `.pc` builds under (a) and under step 3 (prints `2051`, links the keg's `libreadline.8.dylib`), and is refused under (b) and the base;
     - the wrapper under `CPATH` is refused by all four;
     - the direct `CPATH` mispairing builds under (a) and step 3 exactly as at the base (`2051`, libedit). **No Q2 route closes it, and none opens anything new.** It needs its own filing (a `CPATH` or `--include` folder whose header's library the link does not take). The ffi-pragmatist's (d) premise is the reading that would reach it.

5. **The two veto seats contradict each other on (R0)'s scope, and a measured wrong value decides it: under the numbers-only scope, a pointer read as an integer by a format stays open.**
   - **Ran:** `second/ptrint/`: `printf(format: cstr lent, value: cstr)`, called as `examples/curl` calls its lease (`copy: cstr @= "forty-two".lease()`), with `"n = %d\n"`. Built with `heroes-next` and with `heroes-r0` (the whole by-value scope). Also `second/excurl/`, a copy of `examples/curl`, under both; `git show v0.6.0:examples/curl/main.hero`; `sed -n 2830,2845p docs/design.md`; `suite_compat.hero:497`.
   - **Printed:**
     - today: **exit 0, `n = 42259872`**, the pointer's bits read as an `int`;
     - `heroes-r0`: exit 1 `ffi_parameter_type`. A door with the literal format then gets clang's `-Wformat`, which already refuses `%d` against a `char *` in header code (defect 2034's case, `fixedbugs-2034…h:6:56`, *"format specifies type 'int' but the argument has type 'char *'"*, re-run this pass).
   - **The cost of the whole scope, measured:** `heroes-r0` refuses `examples/curl` (rung 4 of §4.19's ladder, design.md:3087-3090) at its `value: cstr`, and the base builds it. `v0.6.0` carries that binding unchanged at `main.hero:58`, and `compat` runs `build --emit-c` on a release's programs (`suite_compat.hero:497`), so it is *told*, a `0.x` program refused (CLAUDE.md § 2). design.md:2839's worked example binds the same shape.
   - **What decides it:** the ffi-pragmatist writes *"Pointers keep one representation on every leg"*. That is true, but a format reading a pointer as an integer is not a question of representation. Its own § 1.9 says a per-type door does not close the wrong-option mistake, which is right for `curl_easy_setopt`. For a format function, though, the door with a literal format does close it, by `-Wformat`.
   - **Bears on:** Q1. By § Precedence (robustness, rank 3), the whole scope is the more robust. The numbers-only scope is the conservative alternative, and it leaves `n = 42259872` at exit 0.

6. **The real population of `pkg-config` folders: (a) and step 3 agree on every installed library, so they differ only on the synthetic shapes of item 3.**
   - **Ran:** `second/pcpop/pop.py`: every `.pc` under `/opt/homebrew/{lib,share}/pkgconfig` and `/opt/homebrew/opt/*/lib/pkgconfig`, each asked `pkg-config --cflags-only-I` under its own folder. Each `-I` folder's `.h` names (depth 3) were intersected with the SDK's `usr/include` (depth 4), then the shared prefix was re-walked with links followed. Also `brew info --json=v2 tcl-tk@8`.
   - **Printed:**
     - **1,032 `.pc` files, 166 distinct `-I` folders.** **8 folders** hold a file the SDK also names: curl 8.23.0 (11), icu4c@78 and @79 (23 each), ncurses (6, and 11 in `ncursesw/`), readline (2), sqlite (2), `tcl-tk@8/include/tcl-tk` (10: `tcl.h`, `tk.h`, …). **None holds a C11 header.**
     - `sdl2`, `sdl2-compat`, `sdl3` and `uvwasi` name the shared prefix `/opt/homebrew/include`: **3,722 headers, 0 colliding** with the SDK.
     - `tcl-tk@8` is keg-only, `:versioned_formula`.
   - **Corrects my first pass:** item 15 counted *seven keg-only formulae* colliding with the SDK. Counted through the folders `pkg-config` names, `tcl-tk@8` is an eighth (its collision sits in a subfolder my first count did not reach), and `llvm@22` names no `.pc` folder.
   - **Bears on:** Q2.
     - On every installed library, (a) and the ffi-pragmatist's step 3 give the same verdict; the exception list decides only `pcstdio` and `pcsocket`.
     - Step 3's POSIX half (56 names "from memory and unsourced", its 4.1) is a premise about the world, which `.claude/rules/module-shape.md` says a narrowing must not rest on. The historian was not asked to source it (POSIX.1-2024 XBD chapter 13 is the page).
     - A shared prefix is a folder `pkg-config` names that is no library's own, and today it opens nothing. Under both routes it would excuse any file a later linked formula puts there.

7. **The cache follows both arms: no stale pairing. The mispairing through `CPATH` reproduces on curl too.**
   - **Ran:** `second/cache/`: the ffi-pragmatist's `pairing` program (printing `LIBCURL_VERSION_NUM` and `curl_version()`) rebound `pkgconfig "libcurl"`. Built four times in one `build/`, alternating no variable and the keg's `PKG_CONFIG_PATH`. `second/cache2/`: the `link "curl"` form, alternating no `CPATH` and `CPATH=/opt/homebrew/opt/curl/include`.
   - **Printed:**
     - the `.pc` arm alternated: `526081`/8.7.1/`/usr/lib`, `530176`/8.23.0/keg, `526081`, `530176`, each linking its own library;
     - the `CPATH` arm: `526081`/8.7.1, then **`530176` against `libcurl/8.7.1` at exit 0**, then `526081`.
   - **Bears on:** Q2. When a route admits the keg, the cache will not mix the two versions. The `CPATH` and `--include` mispairing (item 4, the ffi-pragmatist's § 6 item 2) is three measured instances now, and no route touches it.

8. **A Q3 route nobody built, from the historian's open question: the program's folders as `-iquote`, the groups included in quotes. It closes the danger at its root, in C, at a measured cost.**
   - **Ran:**
     - `git grep -n iquote -- selfhost`, and `cli/compiling.hero:97-109`: the program's root reaches clang as `-I <source_dir>`, an angle-form folder. Nobody answered the historian's question.
     - `second/iquote/`, `clang -fsyntax-only -H -fshow-skipped-includes` on the critic's `q3danger/live` in both group orders, `-I <P>` with angle groups against `-iquote <P>` with quoted groups. Then the lane's shapes `d`, `e`, `g` under `-iquote`, each followed by a platform-style `#include <sys/timeb.h>`.
     - A census of every angle include in the 698 tracked headers outside `runtime/`, `seed/`, `docs/`, resolved against each header's folder and those above it.
   - **Printed:**
     - **Today:** `<SDK>/usr/include/sys/socket.h` → `<P>/sys/types.h` in both orders.
     - **Under `-iquote`:** `<SDK>/usr/include/sys/socket.h` → `<SDK>/usr/include/sys/types.h` in both orders. The program's file is opened only beneath its own `sys/clock.h`, and its `#include_next` still reaches the SDK's.
     - The lane's three shapes: each reads `<P>/…/sys/timeb.h` from its own header, the platform-style include reads `<SDK>/usr/include/sys/timeb.h`, exit 0 each.
     - Census: 552 angle includes, **7 resolving to a file of the program**. Two are cases that are not refusals of this class:
       - `run/fixedbugs-563-cap_fixedbugs-563-limit.h` (`<fixedbugs-563-cap.h>`, `<fixedbugs-563-limit.h>`), a correct program this route would stop compiling;
       - `unsupported/fixedbugs-158-depth-three-mid.h`.

       The other five are 2343, 2357 and 2420 cases of the stand-in class itself.
   - **Its other cost, read:** `-iquote` folders are searched before every `-I` for a quoted include. So the order `cli/compiling.hero:95-96` promises (*"a program's header found in an `--include` directory before its own root is still found there"*) reverses, and a group's quoted `"curl/curl.h"` would prefer a copy in the program's root over `pkg-config`'s.
   - **What it removes:** the listing reader's first-open rule, the dependence on `-fshow-skipped-includes`, and the first arm for the program's own folder.
   - **Unbuilt in Heroes.** It changes how every unit includes its groups, which is architecture (CLAUDE.md § 4).
   - **Bears on:** Q3. A third route beside (L) and (L+S), more robust in kind and dearer in the correct programs it refuses: one tracked program by this census, and every C project that names its own headers in angle form.

9. **Two clang instruments, checked: one closes a route, one confirms a cost.**
   - **Ran:** `clang -Wshadow-header` (Apple clang 21.0.0, and Homebrew's clang 22.1.8) on a one-line file, then clang 22 on the danger shape (`-I <P>`, `<sys/clock.h>` and `<sys/socket.h>`) and on 2428's wrapper shape (`-I cw -I/opt/homebrew/opt/curl/include`). Then `clang -Xclang -ast-dump=json -Xclang -ast-dump-filter=printf` of `<stdio.h>`, parsed with a raw JSON decoder, and `__builtin_has_attribute(printf, format)`.
   - **Printed:**
     - Apple clang 21: *"unknown warning option '-Wshadow-header'"*. clang 22: **silent, exit 0, on both shapes.** The historian's clang 22.1.0 release-note row reads *"(excluding system paths)"*, which is why. That route, letting clang's warning judge, is closed here, measured.
     - The JSON dump: 24 documents, 20 `FormatAttr` nodes, each with keys `id`, `implicit`, `kind`, `range` and **no archetype and no index**. The text dump prints `printf 1 2`. `__builtin_has_attribute` is *"use of unknown builtin"*.
   - **Bears on:** Q1. The compiler-engineer's cost argument against (R)'s reader stands, re-run.

10. **`-fshow-skipped-includes`, on which (L+S) and the prototype's change 1 both rest, is documented at the compiler's clang floor and run on no CI leg.**
    - **Ran:** read LLVM's *Clang command line argument reference* for 18.1.0 (https://releases.llvm.org/18.1.0/tools/clang/docs/ClangCommandLineReference.html, read 2026-11-10, first 100,000 characters). Also `grep -n` of `.github/workflows/ci.yml` and `selfhost/cli/clang_floor.hero`.
    - **Printed:**
      - the 18.1.0 page lists `-fshow-skipped-includes`: *"#include files may be “skipped” due to include guard optimization or #pragma once. This flag makes -H show also such includes."*;
      - the legs are `ubuntu-26.04`, `ubuntu-26.04-arm`, `macos-26`, `windows-latest` (`ci.yml:350`), the x86-64 leg having read clang 21.1.8 (`ci.yml:332`); the floor is clang 18.
    - **Unrun:** on any Linux clang and on the box's clang 23, where `-H` prints Windows paths that the listing reader has never read with the flag. `verification.md` § The batch records the CI's x86-64 leg at clang 18.1.3 on 2026-10-01 (*"clang 22.1.8 against 18.1.3"*, the container's against the leg's). That is a dated sentence, not today's leg.
    - **Bears on:** Q3. Both seats' predictions name *"a clang on a CI leg that refuses or ignores the flag"* as their condition. The documentation says it is not refused at the floor; what the box prints is the CI's to answer after the push.

11. **Populations nobody enumerated: the site, the books and the thesis suite bind no variadic, so (R0) moves no published page. But the compiler-engineer's *condition* has no instrument today.**
    - **Ran:**
      - `git grep -lE 'function (printf|…|execl)\('` over every tracked path outside `selfhost`, `tests`, `examples`, `packages` and `docs/panel`;
      - `git grep -lE 'printf|curl_easy_setopt|\.\.\.\)' -- site docs/books`;
      - `git grep -nE '^\s*extern "' -- tests/thesis` and a count of its files.
    - **Printed:**
      - outside those trees the names occur only in `docs/design.md:2839` (item 5) and two `docs/learn/` questions;
      - `site/` has a shell `printf` in `start.html:112` and no binding;
      - `tests/thesis` holds 324 files, and its two `extern` lines are inside copies of the spec (`contexts/*.md`). No task binds a C function, and the held-out archive is encrypted and unread.
    - **Bears on:** Q1's predictions. The compiler-engineer's condition (*"the thesis harness showing first-attempt compiles lost to (R0)'s wrapper on tasks that call `printf`"*) and the spec-warden's P3 both need a task that does not exist. Each is a new task and a paid run, queued for the author's yes, or the condition can never fire.

12. **The compiler-engineer's Q3 finding reproduces: the lane's widening alone widens the danger it repairs.**
    - **Ran:** `second/q3r/{dd,gg}`, copies by `tar` of the compiler-engineer's shapes (`dd`: `lib/clock.h` writing `"../sys/types.h"`; `gg`: root `clock.h` writing `"sys/types.h"`; each with a `sys/socket.h` group), both orders (`main`, `main2`). Built with `heroes-next`, `heroes-q3L` (`380855f72ed6f272`), `heroes-q3LS` (`6ee622861f80ff73`) and `heroes-proto`.
    - **Printed:** `dd` and `gg` clock-first: base 1, **(L) 0 printing `42 2`**, (L+S) 1, prototype 1. Socket-first: 1 under all four. (A first copy with `cp --parents`, which macOS's `cp` lacks, left empty folders and sixteen exit-2 lines. Those were discarded and the copy redone; they measured nothing.)
    - **Bears on:** Q3. (L) alone is not admissible. (L+S) and the prototype's changes 1 and 2 agree on all four rows.

13. **The historian's reading of shape `e` (MSVC's includer walk would find it) does not hold for the unit's layout, and opens a Windows question nobody asked.**
    - **Ran:** `ls` of a built unit's folder and `grep -n '#include'` of its `main.c` (`second/q3r/dd/build/tu-7c1bbf704bbbe773/main.c`: `heroes_unit.h`, `<hero_os.h>`, then the groups).
    - **Printed:** the unit's own file sits in `build/tu-<key>/`. Shape `e`'s chain is `build/tu-…/main.c` → `<root>/sys/clock.h`, so a walk of the includers' folders looks in `<root>/sys/` and `build/tu-…/`, and `sys/timeb.h` is in neither. It is found only through `-I <root>`, as on clang here.
    - **The question it opens (unrun, no box this sitting):** the historian's `HeaderSearch.cpp` row says clang *in MSVC compatibility mode* searches *"all \#including files"*' folders. The box builds with clang 23.1.1 for an MSVC target (`ci.yml:480`). Under that mode, is a quoted include found from a grandparent includer's folder? If so, the lane's *"found from that header's own folder"* is not what clang does there, and a platform header's quoted include could reach a program's file through the program's own header that included it. The CI's Windows leg after the push, or the box, answers it; both Q3 routes' predictions are this Mac's alone.
    - **Bears on:** Q3, and the reading the historian put in its Q3 summary.

14. **Predictions, by whether an instrument can score them.**
    - **Scoreable at batch 30's gate:**
      - the compiler-engineer's line count (`git diff --numstat`);
      - its five programs;
      - the spec-warden's P2, both once reworded by item 1 (three programs, rows 687 and 688).
    - **Scoreable only by hand, no tracked form reaching them:**
      - the ffi-pragmatist's Q2 prediction (the libcurl package over the keg returns `43`), and every Q2 case: `tests/harness/case_setting.hero:55-61` lets a case set no `PKG_CONFIG_PATH` (my first pass, item 8). The compiler-engineer's `${pcfiledir}` design for a hermetic `.pc` case is the instrument, and it is unbuilt;
      - both Q3 predictions on `danger-live` and `danger-live2`, which live in scratch folders. Their header pair, `sys/socket.h` reading `sys/types.h`, exists on Darwin and glibc; `windows-latest`'s UCRT has no `sys/socket.h` (reasoning, unrun on the box). So a tracked case holding the order-hole repair is judged where the header is (`.claude/rules/platforms.md`), and no seat named which header pair every leg shares.
    - **Paid, queued for the author:** the spec-warden's P1 (`--refresh`) and P3 (a blind run on a task that does not exist, item 11).
    - **None:** the historian's seat registers none, being advisory.

15. **Messages found by the seats, re-read in this pass's outputs. Each is a false or unusable sentence, which §4.17 counts against the message.**
    - `shadowed_header` calls a keg's own header *"the C library's"*. This pass printed *"stands in for the C library's `<readline/readline.h>`"* (item 4); the spec-warden's §9 has it for `<curl/curl.h>` and for the quoted `"../sys/timeb.h"` told as `<sys/timeb.h>`.
    - Its note tells the consumer to move a library's own header out of Homebrew's Cellar: *"rename the file, or move it out of `/opt/homebrew/Cellar/readline/8.3.6/include`"*, a remedy no consumer can take. That is the compiler-engineer's false-remedy finding, now on a second library.
    - `heroes-r0`'s prototype message writes *"passed by value as a `i64`"*, the article wrong before a vowel sound. That is cosmetic, in an unlanded prototype.
    - **Bears on:** Q2 and Q3's landing, and the filing of 2420's rows (my first pass's item 10). Not filed here; no GitHub write.

16. **The compiler-engineer's own Q2 condition fires: (a) refuses a correct program that step 3 builds.**
    - **Ran:** `second/q2cross/`: `both.h` (`<stdio.h>`, `<curl/curl.h>`, `<readline/readline.h>`) bound `extern "both.h" pkgconfig "libcurl"`, plus a second group `extern "readline/readline.h" pkgconfig "readline"`, `PKG_CONFIG_PATH` naming both kegs. The four compilers of item 3, each binary read with `otool -L`.
    - **Printed:**

      | compiler | exit | refused on, or printed and linked |
      |---|---|---|
      | base | 1 | `shadowed_header` on `/opt/homebrew/Cellar/curl/8.23.0/include/curl/curl.h` |
      | **(a)** | **1** | `shadowed_header` on `/opt/homebrew/Cellar/readline/8.3.6/include/readline/readline.h` |
      | (b) | 1 | `shadowed_header` on the curl file |
      | **step 3** | **0** | **`530176 2051`**, linking `/opt/homebrew/opt/curl/lib/libcurl.4.dylib` and `/opt/homebrew/opt/readline/lib/libreadline.8.dylib`: each header with its own library |

    - **What it settles:** the compiler-engineer's verdict names its condition as *"a correct program route (a) refuses … would move Q2 to (b) or (c)"*, and it marked this shape *"unmeasured"*. Measured, it moves Q2 to (c), in the ffi-pragmatist's step-3 form. The spec-warden's preference for (a) rested on (a) *"naming the one fact that makes the keg's header the library's own: header and library come from one answer"*. Here header and library come from one build's answers, and they pair, by `otool`.
    - **Bears on:** Q2's route (below).

17. **Questions the sitting should have asked and did not.**
    - **Does a build pair a header with the library it describes?** Items 4 and 7 measured three mispairings at exit 0: curl through `--include` (the ffi-pragmatist), curl through `CPATH`, and readline through `CPATH`. Every one is accepted today and under every route. Q2 asked only when a folder stands in, never whether the folder's header and the link's library are one release. It is the ffi-pragmatist's (d) premise, and nobody turned it into a question.
    - **Who checks the door?** Under (R0) the door is C the program writes, and item 2 shows the door can hand the mistake back. Nobody asked what the compiler holds a door to beyond its prototype: the literal format inside it, and `-Wformat-nonliteral`.
    - **Is the program's folder in the angle-form search at all?** It is (item 8). The danger the rule exists for comes through that one choice, and no seat was asked to price the other one.
    - **On Windows:** the box's clang in MSVC mode searches the includers' folders (item 13), and `long` is 32 bits there. What does a door `long slot_read_long(int, long)` bound `value: i64` do on the box (the ffi-pragmatist's 1.3, unrun)? Neither the Q1 nor the Q3 verdicts can be scored there before the push.
    - **Where do the cases live?** No seat answered which tracked form holds Q1's door cases (`run`, with a header beside each), Q2's `.pc` cases (the unbuilt harness change, item 14), and Q3's danger cases on a header pair every leg shares.

18. **Re-run and confirmed as the seats wrote.**
    - **Ran:** the ffi-pragmatist's `q1c/curl/timeout.c` (copied to `second/curlto/`) over the keg and over the SDK.
    - **Printed:**
      - keg 8.23.0: `(long)-1 -> 43`, `(int32_t)-1 -> 0`, with libcurl's own *"curl_easy_setopt expects a long argument [-Wattribute-warning]"* at line 11;
      - SDK 8.7.1: `0` and `0`.
    - Also confirmed in items 9, 12 and 16: the compiler-engineer's `FormatAttr` finding and its (L)/(L+S) table, and the ffi-pragmatist's prototype rows I re-ran.

19. **Procedural, said once.**
    - One command of mine wrote 471 bytes of grep output to `/tmp/claude-fwd.txt`, outside the repository's root, through a `tee`. It was found by `ls` about a minute later and removed (`ls` then printed *"No such file or directory"*).
    - A removal of my own scratch folders was refused by the harness for its variable target. The folders were new, so nothing needed removing and the run went on without it.
    - No other file outside `p243-scratch/critic/second/` and this report was written.

20. **The compiler-engineer's whole `run` form under (L+S) (pid 64367) ended at about 20:45: no red case, so no correct program of the form is refused by the route.**
    - **Ran:**
      - watched without touching: `kill -0 64367` and its children, in probes of at most 170 s from 20:42, the case being built read from `ps`;
      - `cat` of `p243-scratch/compiler-engineer/q3/run-whole-LS.txt` once it held bytes (538);
      - `ls -d tests/golden/run/*.case` (11) and `ls *.hero` (741) in that worktree, to place the run.
    - **Printed:** **`run: 750 passed, 0 failed`** and **`harness: 750 passed, 0 failed`**, plus two `SKIP` lines:
      - `run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it`;
      - `run/fixedbugs-589-a-call-of-mktemp-links-silent-on-linux`;

      each told *"`sys/prctl.h`, which clang does not find on the include path the compiler searches"*, Linux-only headers judged where they are. 750 and 2 are the form's 752 cases. `real 4396.56` against `user 1841.02`, `sys 766.09`: the run was waiting on a shared machine, so the duration is not a measurement and is not quoted as one.
    - **Bears on:** Q3. The compiler-engineer's condition, *"a correct program in the `run` form that (L+S) refuses would move Q3"*, is measured and does not fire. This covers (L+S) alone, as that seat built it. The ffi-pragmatist's prototype (changes 1 to 3) and the combined compiler (R0 with a Q2 route and (L+S)) have run no whole form, and the compiler's own tests are unrun on every route.

## The three routes

**Q1, defect 2421.**
- **Most robust and complete that builds: (R0) over every by-value argument past `...`.** That is the compiler-engineer's `heroes-r0`, built and run: 2421 refused, its door printing `-1`, `printf("%d", i64 4294967338)` and `printf("%d", <a lease>)` refused (item 5). It lands with:
  - the note naming the literal format inside the door, and `-Wformat-nonliteral` kept as an error in the program's own header code, so a door cannot hand the mistake back (item 2; measured in C, unbuilt in Heroes);
  - Q2's route in the same batch, since every curl and readline door is a wrapper header (items 4 and 16; the spec-warden's § 8 condition);
  - R10 corrected (the spec-warden's 3.4 wording);
  - rungs 1 and 4 and design.md:2839 reworded, `examples/curl`, `run/ffi-variadic-at-a-fixed-arity` and `run/fixedbugs-584` rewritten as doors, `surface` rows 687 and 688 rewritten (item 1), and the item-22 notes;
  - no § 13 sentence (the spec-warden: +0; V2 is P3's to earn, a paid run).

  The compiler's own tests are unrun on `heroes-r0` (its report).
- **The evidence:**
  - item 5: the numbers-only scope leaves `n = 42259872` at exit 0;
  - the ffi-pragmatist's 1.7: a door with a literal format gets `-Wformat`;
  - item 9, re-run: (R)'s reader has no JSON to read its format index from;
  - the historian: Go, Swift and GHC's `ccall` close the class by the same refusal and wrapper.
- **Conservative: (R0) over numbers only**, the ffi-pragmatist's verdict. It keeps `examples/curl` and `v0.6.0`'s binding building, and refuses 4 bindings instead of 7. It leaves a pointer read as an integer by a format open, measured. **(C) is only the record**: the ffi-pragmatist's veto, the spec-warden's objection, and a `blocking` class pinned. **(R) and (E) are vetoed** by the spec-warden on Principle 0, and (E) does not reach 2421's non-format variadic (the ffi-pragmatist's 1.8). The compiler-engineer's (E') answers 1.8's *no literal for an `i64` on two legs* with a cast to `long long`, but it is unbuilt.

**Q2, defect 2428.**
- **Most robust and complete that builds: the ffi-pragmatist's step 3.** A file in a folder any `pkg-config` answer of the build names is that library's for the second arm. That arm still reaches a name the platform defines, and the first arm stays whole.
- **The evidence:**
  - built as `heroes-proto`: 2428, `curlwrap`, `sqlitewrap`, the readline wrapper and the cross-group wrapper all build, each header paired with its own library by `otool` (items 4 and 16);
  - `pcstdio` and `pcsocket` stay refused (item 3).
- **It lands with:**
  - the exception list **sourced**, not from memory: C11 §7.1.2's 29 names, the POSIX half from POSIX.1-2024 XBD chapter 13 by the historian, held in a module of its own under the `layout` ceiling (the prototype grew `shadowed_tree.hero` to 504 lines);
  - the false message and the false-remedy note rewritten (item 15);
  - the harness change holding a `.pc` case (`PKG_CONFIG_PATH` and `PKG_CONFIG_LIBDIR` beside `CLANG_READS`, a case's `.pc` written with `${pcfiledir}`, the compiler-engineer's design, owing the net's own tests);
  - the `CPATH`/`--include` mispairing filed apart, since no route touches it (items 4 and 7).
- **Conservative: (a), group-scoped, with the same exception.** Without it, (a) as built prints `7` and `99` (item 3). It builds 2428, `sqlitewrap` and the readline wrapper, and refuses the cross-group wrapper, a correct program (item 16). (b) refuses `sqlitewrap` and the readline wrapper and is not admissible as the resolution.

**Q3, defect 2420's two rows.**
- **Most robust and complete that builds: (L+S)**, which matches the ffi-pragmatist's changes 1 and 2 on every shape run. That is the lane's widening (a file a header includes in quotes, found from that header's own folder, is its own, the landed sibling arm kept for shape `e`), together with every open judged and `-fshow-skipped-includes` in the listing.
- **The evidence:**
  - item 12 and both seats' tables: `d` and `g` build; `e` still builds; `live`, `live2`, `dd` and `gg` are refused in both orders, where (L) alone builds `dd` and `gg` clock-first;
  - item 10: the flag is documented at the compiler's clang floor and run on no Linux clang and not on the box.
- **Measured since:** the whole `run` form under (L+S), 750 passed and 0 failed, two Linux-only skips (item 20).
- **Still owed:** the compiler's own tests beyond the shadow modules, the whole forms on the combined compiler, the Windows question of item 13, and the filing of 2420's two rows and the order hole as one item.
- **Conservative: (S) alone**, the order hole closed and `d` and `g` left refused. That is only the record, since a correct program refused is `blocking`.
- **More robust in kind, and unbuilt: `-iquote` for the program's folders (item 8).** It removes the danger at its root in C, and it refuses `run/fixedbugs-563` and every project naming its own headers in angle form. It is for the synthesis to queue as a question, not to adopt.
