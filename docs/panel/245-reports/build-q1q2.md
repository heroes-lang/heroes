# Panel 245, the build of Q1's route (d) and Q2's S and D: the ffi-pragmatist

Written 2026-11-11 from about 01:05, as the work went, in answer to
`docs/panel/245-briefs/build-q1q2.md`. Worktree `.claude/worktrees/245-ffi-pragmatist`, detached at
`7fb6868b2`, holding my prototype uncommitted; scratch
`.claude/worktrees/p245-scratch/ffi-pragmatist/build/`. Apple clang 21.0.0 (clang-2100.4.1.1),
arm64-apple-darwin27.2.0; every platform sentence is this Mac's unless it says otherwise. Load at
the start: `uptime` printed `load averages: 34.13 29.71 22.27` (batch 30's suites), and `pmset -g
assertions` showed `PreventUserIdleSystemSleep 1` held by Amphetamine. So no duration below is a
measurement of speed.

## 0. What the reading established before any code

Each item is a fact read in the tree at `7fb6868b2` or a command run, with what it printed.

0.1. **The trials that write a *compiles* note already compile under the unit's words, not
`probe_words`.** `grep -n 'order_trial.compiled\|probe_words' selfhost/cli/*.hero`:
`header_order.hero:224`, `:261`, `:272` (the move, the module's own order, the header of C's own),
`name_order.hero:44`, `header_reads.hero:34`, `opening_told.hero:57`, `macro_twice.hero:156` and
`one_file.hero:81` all take `compiling.unit_words(under)`. `probe_words` is taken by the type and
meaning questions: `header_ask`, `switch_ask`, `unswitch_ask`, `declared_ask`, `header_alone`, the
`-M` reads, `dump_meaning` (`-ast-dump`), `header_meaning` (`-E`) and `bound_macro`. So the critic's
item 8, read from `compiling.hero:160-161` and `dump_meaning.hero:29`, holds for the meaning probes
and not for the order trials. A `-Werror` placed in `compiling.unit_words` reaches every trial whose
verdict becomes a *compiles* note, and `without_diagnostics` drops it from every type probe, which is
what defect 163 asked of them (*a probe never fails on a header's warning*).

0.2. **Item 7's false note does not come from the words. It comes from `-fsyntax-only`.**
`order_trial.compiled_at` runs `clang -fsyntax-only` (`cli/order_trial.hero:106` at the base).

0.3. **The `warnings` suite already holds the trees `-Werror` would reach.**
`tests/harness/suite_warnings.hero:1-2`: *"The generated C compiles with NO warnings at all — over
the goldens and over the corpus"*. Its scope is `tests/golden/run`, `tests/golden/emit`, every
`examples/*/main.hero` and the gallery, at `-O0` and `-O2`, and *"THERE IS NO EXEMPTION"* (`:24`).
`cli/clang_told.hero`'s `fresh` drops only a warning already told, never a word. So `-Werror` on a
unit's line turns an existing bar into an exit code for those trees. It does not create a new bar.

0.4. **A package's answer cannot put a `-W` word on a unit's line.** `cli/libraries.hero:159-166`
(`compile_flags`) keeps `-pthread`, `-I`, `-D`, `-U` and `-F` alone.

0.5. **The base already quiets the three words after the region.** `emit/deprecation.hero:27-38`
(panel 208's R1): `-Wdeprecated-declarations`, `-Wdeprecated-pragma` and `-Wattribute-warning` are
`ignored` from the groups' close on. Its comment says of the third: *"which clang says at -O0 and not
at -O2, so it can carry no verdict (panel 208)"*. That is a prior ruling on item 9's level question,
written for the compiler's own lines.

0.6. **What § 13 holds.** `sed -n '373,$p' spec/heroes-spec.md | grep -n -i '\.\.\.\|variadic\|door\|printf\|format'`
printed nothing. § 13 does hold the first clause of each of the four notes the brief names:

| base line | clause cited to § 13 | in § 13 | clause not in § 13 |
|---|---|---|---|
| 220 | a parameter declared at the header's own width and sign | yes: *"declared at the header's own width and sign ... one that disagrees is refused"* | *past a header's `...` the header declares no type* |
| 230 | a record lent read-only is a place C reads and never writes | yes: *"declared `lent` without `@`, is a pointer to const, the place C reads and never writes"* | *past a header's `...` the header declares no type for it* |
| 250 | what an `@` parameter points at is held to the header's own type | yes: *"What it points at is ONE element, held to the same width and sign"* | *past a header's `...` the header gives none* |
| 260 | a counted lend hands C the program's own bytes | yes: *"`counted_by N lent` hands C `N` elements ... `N` the sibling that tells C how many"* | *past a header's `...` none of them can be held to a count* |

The fact each second clause needs is C's own: C11 6.5.2.2p7, *"The ellipsis notation in a function
prototype declarator causes argument type conversion to stop after the last declared parameter. The
default argument promotions are performed on trailing arguments."* `header_region.hero` already
cites C11 by paragraph (`6.5.1p2`, `7.21.6.1p2`), so the repair cites that paragraph for the clause
§ 13 does not hold.

0.7. **Four clang facts the build rests on.** Each was run in `build/facts/` with Apple clang 21:

- `clang -std=gnu11 -Werror -Wno-error=#warnings -fsyntax-only w.c`, where `w.c` holds a `#warning`,
  a `#pragma message` and `1 << 40`, printed `warning: "kept a warning" [-W#warnings]`, `warning: a
  message [-W#pragma-messages]` and `error: shift count >= width of type
  [-Werror,-Wshift-count-overflow]`, exit 1. So the spelling is accepted, `#warning` stays relayed,
  and `#pragma message` is never upgraded.
- **A header's own `warning` pragma is exempt from `-Werror`.** `pw.c`
  (`#pragma clang diagnostic warning "-Wdeprecated-declarations"` and then a use of a deprecated
  name) printed `warning: 'old' is deprecated` with its `note: ... explicitly marked deprecated
  here`, exit 0. That is the mechanism of the compiler-engineer's one-line carrier for defect 158
  (item 4 below).
- **`-femit-all-decls` makes `-c` see an unused door's `warning` call.** `aw.c` holds a
  `warning`-marked `static inline` and a door calling it, and nothing calls the door.
  - `-O0 -c`: exit 0, nothing printed.
  - `-O0 -femit-all-decls -c`: `error: call to 'warned' declared with 'warning' attribute
    [-Werror,-Wattribute-warning]`, exit 1.
  - `-O2 -femit-all-decls -c`: exit 0, which is the unit's own `-O2` answer.
  - `-fsyntax-only -femit-all-decls`: exit 0.
- **No driver warning from a package's compile words under `-Werror`.** `clang --target=<t>
  -std=gnu11 -Werror <w> -c e.c` exited 0 with nothing printed for `-F/nowhere`, `-pthread` and
  `-D_X=1` on each of `aarch64-linux-gnu`, `x86_64-linux-gnu`, `x86_64-pc-windows-msvc` and
  `arm64-apple-macos`. These are Apple clang's driver for each target; Debian clang and the Windows
  box are unrun.

## 1. What was built

The prototype's two modules stay; the route adds four modules and one carrier. Everything is in
the worktree, uncommitted. The diff is in § 9.

- **`emit/header_region.hero`.**
  - The prototype's enumerated `UNDEFINED` list and its second pass over `FORMAT_DEFINED` are gone.
  - `LINE_WORDS` (`-Werror`, `-Wno-error=#warnings`) is what each unit's line carries.
  - `ADVICE` (`-Wdeprecated-declarations`, `-Wdeprecated-pragma`) is `ignored` in the region.
  - `HELD` (`-Wimplicit-int`, `-Wuser-defined-warnings`) is `error` after the `-Wall` ignore
    (item 1).
  - S is in: `-Wformat-security` is out of `FORMAT_DEFINED`, so `KEPT`'s `-Wformat` raises it.
  - The `-Werror` note now says what is passed over (`-Wall`'s warnings, sign conversions,
    deprecations), what is told and never refused (`#warning`, `#pragma message`), and *"every other
    warning clang gives there is an error"*.
  - New `never_for` phrases for the two `HELD` words; the module's tests updated, and one new test.
- **`cli/compiling.hero`.** `unit_words` is the flags, the level and the search, then `LINE_WORDS`.
  `probe_words` drops them, as it drops every `-W` word (0.1). Two tests updated.
- **`cli/order_trial.hero`.**
  - `compiled_at` takes its clang mode.
  - New `generated`: `clang -c -femit-all-decls -o <private object>` under the same words, the
    object discarded after.
  - The opening the test pins is updated.
- **`cli/header_order.hero`.**
  - Every refusal first asks `sees` of the order as written. If the syntax pass refuses it, the
    searches are syntax trials. If the syntax pass compiles it and a generating compile refuses
    it, every search is a generating trial. If both compile it, no order note is written, and the
    refusal keeps clang's line and its own notes.
  - `search`, `above` and `standard` take `generating`.
  - `cli/name_order.hero` passes `false`: an unknown name is a syntax question.
- **`emit/ffi_past_prototype.hero`.**
  - D, from the prototype: a variadic bound with nothing past its `...` is refused at the binding,
    `ffi_call_shape`.
  - The five notes' citations are repaired (§ 5).
- **`tests/golden/unsupported/fixedbugs-158-noted-outer.h`.** The compiler-engineer's carrier line,
  `#pragma clang diagnostic warning "-Wdeprecated-declarations"`, at line 3. The comment above it was
  folded from three lines to two, so `line 6 includes`, which the case's `.expected` pins, is still
  line 6.

**Type check and build.**
- `./heroes check selfhost/main.hero` (stage 1, from the seed): exit 0, `real 10.37`.
- `heroes fmt --in-place` on the six modules: `order_trial`, `header_order` and `name_order`
  reformatted, line breaks only (`diff` read).
- `./heroes build selfhost/main.hero -o heroes-next`: `wrote heroes-next`, `real 60.16`, `user
  57.57`, exit 0. `heroes-next --version`: `sources 2a61801315e47adc`.
- The base stage 2 is kept as `heroes-base` (`sources 0fe313ba58ba197c`).

## 2. The critic's 17 shapes, through the compiler

The critic's `run.sh` writes each region by hand and compiles the C directly. To judge the region
the compiler now writes, each of the 17 headers of `critic/second/q1/h/` was copied into
`build/shapes/<shape>/` beside a Heroes program binding `door(x: i64) -> i64` and printing
`door(1)`. Each was built by a compiler and run where it built (`build/shapes.sh <compiler> <tag>
<level>`, `timeout 20`, output through `head -c 200000`). Outputs: `build/shapes-{d,base}-O{0,2}.txt`.

| shape | base `-O0` | base `-O2` | (d) `-O0` | (d) `-O2` |
|---|---|---|---|---|
| asm_cast | 1 `ffi_header_refused` | 1 | 1 `-Winvalid-gnu-asm-cast` | 1 |
| atomic_access | 1 | 1 | 1 `-Watomic-access` | 1 |
| attr_warning | 0, raw, prints `1` | 0, silent, `1` | **1** `-Werror,-Wattribute-warning` | 0, silent, `1` |
| deprecated | 0, raw, `1` | 0, raw, `1` | 0, **silent**, `1` | 0, silent, `1` |
| diagnose_if | 0, silent, `0` | 0, silent, `0` | **1** `-Werror,-Wuser-defined-warnings` | **1** |
| free_nonheap | 0, raw, **dies of SIGABRT** | 0, raw, SIGABRT | **1** `-Wfree-nonheap-object` | **1** |
| hash_warning | 0, raw `#warning`, `1` | 0, raw, `1` | 0, **raw `#warning`**, `1` | 0, raw, `1` |
| init_union | 0, raw (2), `1` | 0, raw, `1` | **1** `-Winitializer-overrides` | **1** |
| int_overflow | 0, raw, `-2147483647` | 0, raw, `-2147483647` | **1** `-Winteger-overflow` | **1** |
| kept_return | 1 `-Werror,-Wreturn-type` | 1 | 1 | 1 |
| lib_redecl | 0, raw, **a null read, panic** | 0, raw, null read | **1** `-Wincompatible-library-redeclaration` | **1** |
| pragma_message | 0, raw, `1` | 0, raw, `1` | 0, **raw**, `1` | 0, raw, `1` |
| shift_count | 0, raw, `2` | 0, raw, **`0`** | **1** `-Wshift-count-overflow` | **1** |
| shift_neg_count | 0, raw, `2` | 0, raw, `0` | **1** `-Wshift-count-negative` | **1** |
| shift_neg_value | 0, raw, `-7` | 0, raw, `-7` | **1** `-Wshift-negative-value` | **1** |
| strncat_size | 0, raw, **dies of SIGTRAP** | 0, raw, SIGTRAP | **1** `-Wstrncat-size` | **1** |
| varargs | 0, raw, `3` | 0, raw, `3` | **1** `-Wvarargs` | **1** |

In the table, "raw" means clang's warning printed at exit 0, and "1" means exit 1. Every (d) refusal
is `error[ffi_header_refused]` at exit 1, and none is an exit 2.

What (d) does with the 17:
- It refuses 14 at `-O0` and 13 at `-O2`. Of those, 11 and 10 are new; the base already refuses `asm_cast`, `atomic_access` and `kept_return`.
- It lets the deprecated name through silent.
- It relays `#warning` and `#pragma message` at exit 0.
- The one shape that moves with the level is the `warning`-marked `static inline` call (§ 3.3).
- `diagnose_if`, silent at the base and under the critic's emulated (d), is refused by `HELD`.

The base's own run of the same programs shows why, each after a warning at exit 0:
- three die at run time (`free_nonheap`, `strncat_size`, `lib_redecl`);
- five print a value from a construct C11 leaves undefined (`int_overflow`, the three shifts,
  `varargs`), and `shift_count` and `shift_neg_count` print `2` at `-O0` and `0` at `-O2`.

**One of (d)'s 14 refusals is of a construct C does not call undefined.** `init_union` overrides
union members by designated initializers, which C11 6.7.9p19 orders (the base printed `1`). (d)
refuses it because it refuses every default warning outside `-Wall`, `-Winitializer-overrides`
among them. That is (d)'s standing cost class: a default warning outside `-Wall` about a construct C
defines. It met no tracked program here (§ 7). In my first report's census it met none of the
1,066 Homebrew headers that compile, a census at `-fsyntax-only` that printed no word at all. My
first report's condition was *a real library header ... that (d) refuses for a construct C defines
and the allow-list cannot name*. A toy header does not meet it.

## 3. Q1, route (d): the four items it owes

### 3.1. `-Wuser-defined-warnings` and `-Wimplicit-int` raised again (item 1)

`HELD` writes both as `error` after the `-Wall` ignore.
- `diagnose_if` is refused at both levels (§ 2's table), where the base and the critic's emulation
  build it silent and print `0`.
- **The loop of item 10 is closed.** I copied the two programs of `b30-scratch/repro/loop/` to
  `build/loop/{a,b}` and built each with both compilers (`e-{base,next}.txt`).

| program | base | (d) |
|---|---|---|
| `a` (`own.h` alone) | exit 1, note *"`own.h` compiles after `stdio.h`, a header of C's own it uses and does not include: write a group naming `stdio.h` above this one"* | exit 1, note *"... repair the header, or name the one that declares this group's C"* (`REPAIR`) |
| `b` (a `stdio.h` group above) | exit 1, note *"these compile when `own.h` is included before `stdio.h` ... write this group above that one"* | exit 1, the same `REPAIR` note |

`own.h` is beside the program and its line 2, `static implicit_x = 3;`, is the fault. So under (d)
the note is true, and following it ends the loop.

### 3.2. The trials under the unit's words (item 2)

- **Built.** `LINE_WORDS` sits in `compiling.unit_words`. Every trial whose verdict becomes a
  *compiles* note already takes that list (0.1), so each one now compiles under `-Werror` as the unit
  does.
- **Left without, on purpose.** The type and meaning probes take `probe_words`, which drops every
  `-W` word, so a header's warning never turns into a wrong answer about a type (defect 163's
  reason).
- **Shown.**
  - The loop rows above: the order trial now refuses `own.h` in every order, as the unit does, so no
    order is told.
  - § 2's rows: no (d) refusal carries an order note. The command and its `0` are in § 3.5.

### 3.3. A code-generating trial for `-Wattribute-warning`, and the level (items 3, 7, 9)

**Built: `order_trial.generated`, and `header_order.sees` before every search.**
- `sees` compiles the order as written.
  - If clang's syntax pass refuses it, the searches are syntax trials, as before.
  - If the syntax pass compiles it, the unit was refused for something only code generation says.
    The written order is then compiled again with `-c -femit-all-decls` at the unit's own level. If
    that refuses it, every search compiles that way.
  - If that compiles too, no trial reproduces the refusal, and no order note is written.
- `-femit-all-decls` is what makes a trial of an opening see a door nobody in it calls (0.7: exit 0
  without it, exit 1 with it, at `-O0`).

**curl's checker, through the compiler.**
- `build/curl/` is my earlier probe: `curl_bound.h` with `#include <curl/curl.h>` and no `#undef`,
  and the door `net_set_timeout(CURL *h, int32_t s)` handing `s` to `CURLOPT_TIMEOUT`. It was built
  with `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig` (keg curl 8.23.0).
  - **Base:** exit 0 at both levels, raw `-Wattribute-warning` (1 line at `-O0`, 2 at `-O2`), and
    the program runs.
  - **(d):** exit 1 at both levels, `ffi_header_refused` at `curl_bound.h` line 34, `[-Werror,-Wattribute-warning]`.
    The notes are clang's line, the `-Werror` note and `REPAIR`.
  - The prototype's saved refusal of the same program (`proto/curl-checked/e2-heroes-proto-O0.txt`)
    ended in *"`curl_bound.h` compiles after `stdio.h`, a header of C's own it reads itself"*. That
    note is gone at both levels.
- `build/q1curlp/` is the critic's `q1curl/t.c` as Heroes programs: `redirs.h` holds
  `set_int(CURL *, int32_t)` and `set_long(CURL *, long)`, each handing `v` to `CURLOPT_MAXREDIRS`;
  `both` binds the two and prints each with `-2`, and `long` binds `set_long` alone.

| program | base `-O0` | base `-O2` | (d) `-O0` | (d) `-O2` |
|---|---|---|---|---|
| `both` | exit 0, 1 raw warning, prints `0 43` | exit 0, 2 raw, `0 43` | exit 1 `ffi_header_refused` | exit 1 |
| `long` | exit 0, silent, `43` | exit 0, `43` | exit 0, silent, `43` | exit 0, `43` |

`43` is `CURLE_BAD_FUNCTION_ARGUMENT`, libcurl refusing `-2`. The `int32_t` door's `0` is libcurl
accepting a value other than the one handed to it, at exit 0 under the base, at both levels.

**The level (item 9).** Under (d), the `warning`-marked `static inline` call is refused at `-O0` and
builds silent at `-O2` (§ 2, `attr_warning`), because clang inlines the call away. curl's checker is
refused at both levels: its function is `noinline` with an `asm` body, so the call survives. What
(d) does is clang's answer at the unit's level. The generating trial runs at that level too, so the
note never contradicts the unit. The answers, and what each costs:

1. **Keep clang's answer per level** (what is built). It costs nothing on a build that compiles. A
   program can be refused by `heroes build` (`-O0`) and built by `heroes run` (`-O2` by default).
   This happens only where the optimiser deletes the warned call, and a warned call that `-O2`
   inlines still runs the callee's body. Panel 208's R1 already ruled that this word *"can carry no
   verdict"* for the compiler's own lines (`emit/deprecation.hero:30-33`), and (d) puts a verdict on
   it in a header's code.
2. **Judge the region's code at `-O0` whatever the level.** One more `-c -femit-all-decls -O0`
   compile per unit that binds a group's header at `-O2`. Its cost was measured only on one opening
   (§ 3.5), and a whole unit's is unmeasured. It is paid on every build at `-O2`, on success too, and it would refuse an unused `static inline`
   function of a library header that calls a `warning`-marked one, which the unit never compiles. I
   did not run that refusal's census (unrun).
3. **Quiet `-Wattribute-warning` in the region**, the lane's route. It costs curl's checker: the
   `int32_t` door above prints `0` for `-2` at exit 0.

The ruling is the sitting's. Answer 1 is the only one built.

### 3.4. Defect 158's carrier (item 4)

The case's run is in § 4. It is built as the compiler-engineer wrote it: one line,
`#pragma clang diagnostic warning "-Wdeprecated-declarations"`, in `fixedbugs-158-noted-outer.h`.
A header's own pragma mapping to `warning` is exempt from `-Werror` (0.7), so the deprecated name
in `noted-mid.h` is still a warning under (d), with its note pointing back into `noted-outer.h`.

### 3.5. Two small measurements promised above

- **No order note in any (d) shape build.** `grep -l 'compiles after\|these compile when\|compiles
  when' build/shapes/*/out-d-O*/build.err | wc -l` printed `0`, over the 34 builds. The same grep
  over the base's 34 printed `0`.
- **What a generating trial costs on one opening.** The opening is `stdio.h`, `stdlib.h`,
  `string.h` and `curl_bound.h` (`build/facts/open.c`), with keg curl's `-I`. `/usr/bin/time -p
  clang ... -fsyntax-only` read `user 0.01` in each of 6 runs. `... -c -femit-all-decls -O0 -o
  gen.o` read `user 0.00` in each of 6, exit 0, with 58 functions in the object (`nm`). Both are at
  the instrument's 0.01 s resolution, at load 5, so I can only say: below 0.01 s of user time on this
  opening. A trial runs on a refusal's path alone. A whole unit's `-O0` compile (answer 2 of § 3.3)
  is unmeasured.

## 4. The named cases, narrowed by the harness's third word

Each was run as `./heroes-next run tests/harness/main.hero -- ./heroes-next <form> <substring>` from
the worktree's root, with the compiler of § 1. Outputs: `build/h-*.txt`.

| form, substring | first run | what moved | after |
|---|---|---|---|
| `unsupported fixedbugs-158` | **14 passed, 0 failed** | nothing | — |
| `run fixedbugs-2036` | **3 passed, 1 failed** | `a-header-s-own-printf-of-a-format-held-in-a-variable-builds`, refused at its line 4, `[-Werror,-Wformat-security]`: S's one cost (§ 5) | left failing, the sitting's to rule |
| `unsupported fixedbugs-2036` | 0 passed, 5 failed | the `-Werror` note's text, one line per case (below) | **5 passed, 0 failed** |
| `run fixedbugs-2421` | **2 passed, 0 failed** | nothing | — |
| `unsupported fixedbugs-2421` | 1 passed, 6 failed | the repaired `...` notes, one line per refused binding (`build/blocks.py` printed every differing line; nothing else differed) | **7 passed, 0 failed** |
| `unsupported fixedbugs-2102` | **1 passed, 0 failed** | — | — |
| `unsupported fixedbugs-2103` | 1 passed, 0 failed | (after the note's re-blessing) | — |
| `unsupported fixedbugs-2106` | 1 passed, 0 failed | (after it) | — |
| `run fixedbugs-2106` | **1 passed, 0 failed** | — | — |
| `unsupported fixedbugs-569` | 4 passed, 0 failed | (the note, re-blessed) | — |
| `unsupported fixedbugs-2034` | 2 passed, 0 failed | (the note) | — |
| `unsupported fixedbugs-360` | 6 passed, 0 failed | (the note) | — |
| `unsupported past-a-variadic` | 10 passed, 0 failed | (the `...` notes) | — |

**The re-blessing, by hand and never by a regenerator.** Two notes moved by design, and every
expectation that pins either was rewritten by an exact substitution of the old text by the new, on
the note's own line.
- **The `-Werror` note.** Its old text was false under (d): *"passed over for `-Wall`'s warnings and
  sign conversions alone"*, when deprecations are quiet in the region too. `grep -rlF` found it in
  15 files, every one under `tests/golden/unsupported/`, one line each.
- **The four `...` notes.** 9 files hold them (2421 ×6, 2103, 2106,
  `ffi-a-record-lent-read-only-past-a-variadic-prototype`). The new text names the function, so
  the name was taken from the refusal's own message above each note (`` `param` of `name` ``).
- The two 2053 cases and 2280 are judged on Windows alone. Their rewritten note is unrun.

**Defect 158's carrier tests what it says (item 4).** The case's 14 passing is not that proof, so I
went further.
- I copied the case and its two headers to `build/c158/{with,without}` and took the pragma line
  out of the copy in `without`.
- I built both with my compiler. The compiler prints only its own `ffi_missing_header` in both.
- I compiled the program's unit by hand, with its own words (`build/tu-*/fixedbugs158*.c`,
  `@build/words/<the .rsp holding "-Werror">`).
  - **With the line**: `noted-mid.h:3:52: warning: 'fixedbugs_158_old' is deprecated`, then
    `noted-outer.h:5:45: note: ... explicitly marked deprecated here`, then
    `noted-mid.h:4:10: fatal error`.
  - **Without it**: the fatal error alone. That is a case which passes and tests nothing.

## 5. Q2: S and D in one compiler, with notes that cite no false clause

**S.** `-Wformat-security` is kept in the region with `-Wformat`.
- `build/q2/q2-fwd` is my forwarding door `say(f) { return printf(f); }`, called with a run-time
  string. The base builds it at exit 0. (d)+S: exit 1, `ffi_header_refused` at `fwd.h`,
  `[-Werror,-Wformat-security]`.
- Its one cost in the tree is `run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds`
  (§ 4), refused at `printf(m)` with clang's own fix-it note *"treat the string as an argument to
  avoid this"*. The sitting has two repairs to choose from:
  - rewrite the header's line 4 as `printf("%s", m)`, and the case still builds and prints `hi`;
  - or move it to `unsupported/` as `...-is-a-known-cost`, as defect 2036's other five were.

  I built neither.

**D.** A variadic bound with nothing past its `...` is refused at the binding, `ffi_call_shape`.
`build/q2/`, my earlier probes copied:

| program | base | (d)+S+D |
|---|---|---|
| `open(path, flags)` direct, `fcntl.h` | exit 0 | exit 1 `ffi_call_shape` |
| `sqlite3_mprintf(f)` direct | exit 0 | exit 1 `ffi_call_shape` |
| `ioctl(fd, req)` direct | exit 0 | exit 1 `ffi_call_shape` |
| `printf(f)` direct | exit 1 `ffi_call_shape` (the base's `format` path) | exit 1 `ffi_call_shape` |
| `say(f) { printf(f); }`, a door | exit 0 | exit 1 `ffi_header_refused`, `-Wformat-security` |

**The notes, as built (the first of the brief's two).**
- **D's rule note** cites C, not § 13:

  > C11 6.5.2.2p7: a call converts nothing past a prototype's `...`, and a binding hands nothing
  > there; C11 7.16.1.1p2: a `va_arg` with no next argument to read is undefined, so what
  > `sqlite3_mprintf` reads past it is whatever the machine left there. A function its header
  > declares variadic is called from a header of the program's own, where the call is C's, its
  > arguments are the ones written there, and clang reads a format there against them

  The second sentence is this compiler's rule, stated without a citation. `grep -c 'spec § 13'`
  over the four D refusals' output printed `0` for each. The note was first built citing only
  6.5.2.2p7, with *"whatever the registers and the stack held"*; I tightened it to cite 7.16.1.1p2
  and rebuilt stage 2 (`real 53.10`, `sources 3f20e3710e524f6f`). That compiler is the one every
  later row used.
- **Lines 220, 230, 250 and 260 at the base** (`ffi_parameter_type` by value,
  `ffi_writable_parameter` for a read-only record, `ffi_parameter_type` for an `@` cell, and for a
  counted lend). Each now cites § 13 for the clause § 13 holds (0.6, quoted there) and C11
  6.5.2.2p7 for what C does past the `...`, in a sentence of its own. For example, line 220 is now:

  > spec § 13: a parameter is declared at the header's own width and sign, and one that disagrees
  > is refused. C11 6.5.2.2p7: past a prototype's `...` a call converts nothing beyond the default
  > argument promotions, so `slot_flag` reads the bytes as the type its `va_arg` names: ...

  Three of the four old notes named no function. The new ones name the one the binding names.
- **`told`'s comment** said *"the rule spec § 13 states"*. It now says a rule cites § 13 for what
  § 13 holds, and C11 6.5.2.2p7 for what C does past a prototype's `...`.

**The second, not built: the note under W0.** If the spec-warden's W0 lands, *"A function its header
declares with `...` is never bound: bind instead a function of a header of your own that calls it"*
(+27 vendored tokens, their table), D's rule note would read:

> spec § 13: a function its header declares with `...` is never bound: bind instead a function of a
> header of your own that calls it. C11 6.5.2.2p7: a call converts nothing past a prototype's
> `...`, and a binding hands nothing there, so what `open` reads past it is undefined (C11
> 7.16.1.1p2)

The four `...` notes would not change: each already cites § 13 only for what § 13 holds today.

**Found beside, not repaired: line 240 at the base is the same class.** `READ`'s note says *"spec
§ 13: `s.cstr()` lends the string's own bytes, and they may be shared with other values — a C write
through them changes values this program never passed to C, and on a literal it aborts"*. `grep -n -i
'shared\|literal.*abort\|abort.*literal\|own bytes' spec/heroes-spec.md` finds none of those clauses
in the spec (one hit, § 4:133, is about constant folding). § 13 holds only *"`s.cstr()` lends a
`str` to C"*. I left its wording as it is: whether a `cstr` lend's bytes are shared is a fact of
the runtime I have not measured, so I cannot write its true replacement. This is a false message
by `.claude/rules/verification.md` § Bounded discovery, so `blocking`. It is unfiled: this brief
allows no GitHub write.

## 6. What the `-Wall` ignore still takes under (d)

Item 1 names two words, and the brief asked for them. To see whether any other default error sits
in the same blind spot, I asked Homebrew's LLVM 22 `diagtool`. This is clang 22's list, not Apple
clang 21's.
- `diagtool show-enabled -x c /dev/null` printed 1,406 enabled diagnostics. With `-Wno-all`
  added it printed 1,259.
- `comm` of the two gives the 147 that only `-Wall`'s group holds (`build/facts/en-wall-only.txt`).
  Of those, 10 are errors by default:

| diagnostic | group | under (d) |
|---|---|---|
| `ext_implicit_function_decl_c99`, `ext_implicit_lib_function_decl_c99`, `warn_builtin_unknown` | `-Wimplicit-function-declaration` | raised again (`KEPT`, defect 2052) |
| `ext_missing_type_specifier`, `ext_param_not_declared` | `-Wimplicit-int` | raised again (`HELD`, item 1) |
| `ext_return_has_expr`, `ext_return_missing_expr`, `warn_return_missing_expr` | `-Wreturn-mismatch` | raised again: `diagtool tree -Wreturn-type` lists `-Wreturn-mismatch` under it, and `KEPT` holds `-Wreturn-type` |
| `warn_comparison_in_fold_expression` | `-Wparentheses` | passed over; a C++ fold expression, no C construct |
| `warn_consecutive_comparison` | `-Wparentheses` | passed over |

`warn_consecutive_comparison` was run on `int f(int a, int b, int c) { return a < b < c; }`
(`build/facts/chain.c`, `-std=gnu11 -fsyntax-only`). Apple clang 21 and Homebrew's clang 22 both
printed `error: chained comparison 'X < Y < Z' does not behave the same as a mathematical expression
[-Wparentheses]`, exit 1. C11 6.5.8 defines that expression (the 0 or 1 of `a < b`, compared with
`c`), so passing it over in a header's code is the region's own rule: what C defines, a library's
style may hold. So no default error but C-defined ones stays turned off by `-Wall`'s ignore under
(d), on clang 22's list. Apple clang 21's own list is unrun: `xcrun -f diagtool` fails here, as the
critic found.

The 137 default **warnings** in that list are what the note calls *"`-Wall`'s warnings"*, passed
over. Among them are `-Wunused-value`, `-Wparentheses`, `-Wswitch`, the `-Wtautological-*` words,
`-Wsizeof-array-argument`, `-Wstring-plus-int` and `-Wstatic-self-init`. The format words are
`-Wformat`'s and kept. I did not judge the remaining ones one by one against C; that is the same
open question the region has carried since panel 205.

## 7. The census: what (d)+S+D refuses that the base builds

**The set.** `git ls-files 'examples/*.hero' 'tests/golden/run/*.hero' | xargs grep -l '^extern "'`
gave 510 files, 21 under `examples/` and 489 under `tests/golden/run/`. Each was mapped to the
program that holds it: a `.case/` folder's `main.hero`, an example's `main.hero`, a gallery file
itself. That gave 505.
- The base refused 29 of the 505 with `no_entry_point` or `unknown_module`: they are modules under a
  plain folder of `tests/golden/run/`.
- I replaced them with the 25 roots that `use` them (`grep -lE "^use <dir>/<name>"`).
- That makes **496 roots** (`build/census-roots2.txt`).

**The run.** `build/census.sh <tag> <compiler>` builds each root once with `heroes build <root> -o
<scratch>` at `build`'s default level, from the worktree's root, one build at a time.
- The base took `real 345.72` for the first 505 and then 20 more. Mine took `real 417.87` for the
  496.
- These are not timings: the machine was not still, and the two runs started from different
  caches.
- Tables: `build/census/{base,route}/table.tsv`. Every stderr: `build/census/*/err/`. The
  comparison: `build/census-compare.py`.

| | built | refused |
|---|---|---|
| base | 492 | 4 |
| (d)+S+D | 491 | 5 |

**Moved: one program.**
`tests/golden/run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds.hero`,
exit 0 at the base, exit 1 `ffi_header_refused` `[-Werror,-Wformat-security]` under the route. That
is S, and it is the same case as § 4.

**Refused under both, for their environment, not the route.**
- The two `fixedbugs-2428-*.case`: `ffi_pkgconfig`. They need the `PKG_CONFIG_PATH` their case sets.
- `fixedbugs-437` and `fixedbugs-589`: `ffi_missing_header`. They bind Linux headers.

**Warnings.** `grep -l ': warning:'` over the 496 stderrs of each run printed 0 files for both
compilers. So at `-O0`, no tracked program that binds a header made clang say anything that (d)
could newly refuse, which is the `warnings` suite's standing bar seen from the other side. `-O2` was
not part of this census. The `warnings` suite builds at both levels and is the gate's.

## 7b. `surface`, whole, once at the end

`./heroes-next run tests/harness/main.hero -- ./heroes-next surface` from the worktree's root, with
the final compiler (`sources 3f20e3710e524f6f`):
- It printed **`surface: 452 passed, 0 failed`**, exit 0 (`build/h-surface.txt`).
- That holds defect 248's check (*"a bound header's clang warning is told once, by the program's
  unit"*): `#warning` is still relayed under `-Wno-error=#warnings`.
- It also holds the two `twoarity-lent` rows on `printf`'s `...`, which pin the
  `ffi_parameter_type` and `ffi_writable_parameter` messages; those messages did not move, only
  the notes did.
- `real 99.83` against `user 57.80`: the run waited, so this is not a timing.

## 8. What (d) and S+D still owe

Each item is unrun unless it says otherwise.

**Before a landing.**
1. **The gate.** Two kinds of output move at once:
   - (d) changes what is refused, so every golden tree must judge it (`.claude/rules/verification.md`,
     *a new checker rule*): `check`, `run`, `emission`, `determinism`, `corpus`, `compat`, `warnings`.
   - The region's text and every unit's words both change, so `emit` and `emission` move. The
     compiler-engineer counted 465 of `tests/emission/` and 2 of `tests/golden/emit/` for E's text.
     Not counted here.
   - `cache` alone: every unit's key names its words, so every build directory goes cold once.
2. **The compiler's own tests**, `heroes test selfhost/main.hero`. Five test blocks changed or
   were added (`header_region` ×3, `compiling` ×2, `order_trial`'s pinned opening). They type-check
   (`heroes check`, exit 0) and were never run.
3. **Three splits.** `layout`, narrowed to each touched module (`./heroes-next run
   tests/harness/main.hero -- ./heroes-next layout <name>`, `build/h-layout-*.txt`), read 3 passed
   and 1 failed for `header_region`, `order` and `ffi_past_prototype`, and 4 passed for
   `compiling`:
   - `selfhost/emit/header_region.hero` has 338 lines of code. The seam I'd cut is the note
     (`werror_notes`, `never_for`, `never_words`, `index_of`) away from the lists and `opening`.
   - `selfhost/cli/header_order.hero` has 336. The seam is `sees` and `compiles`, the trial's mode,
     into a module of their own.
   - `selfhost/emit/ffi_past_prototype.hero` has 305. The seam is D's refusal, away from the
     `...`-lend refusals.

   Each is past § 11's 300.
4. **Defect 2036's `printf(m)` case**: rewritten as `printf("%s", m)`, or moved to `unsupported/`
   as a known cost (§ 5).
5. **Line 240's false § 13 citation**, `blocking`, unfiled (§ 5).

**Off this Mac.**
6. **Linux and Windows.**
   - glibc's own inline code in a group's header under `-Werror` is unrun: the 85 Linux arm64
     headers of panel 204's census and glibc's fortified `printf` family under S.
   - The Windows box's `-Werror` with the MSVC target's headers is unrun.
   - The driver's words under Debian clang 18 and 22 are unrun: § 0.7's driver check was Apple
     clang's for four targets.
7. **Two paths that compile with the unit's words**, and so now under `-Werror`, are unrun as
   cases: `front_check`'s second read of a unit holding the value region (`cli/units.hero:233`),
   and `one_file`'s whole-program trial (`cli/one_file.hero:81`).

**For the sitting to rule.**
8. **The level (§ 3.3).** What is built is clang's answer per level. Ruling otherwise has a cost,
   measured only on one opening.

**Cases and checks still missing.**
9. **No tracked case pins any of this yet.** Each needs a golden case, annotated in its source:
   - the (d) shapes of § 2 (one refusal per kind at least: a shift, `diagnose_if`, a `warning` call
     at `-O0`);
   - the loop of § 3.1;
   - a code-generating refusal whose note is no order. curl's probe needs keg curl 8.23.0, which
     no CI leg is known to hold, so the toy `warning` header is the portable carrier;
   - D's direct `open` and `sqlite3_mprintf` (§ 5).
10. **The generating trial is one-sided by argument, not by measurement.** `-femit-all-decls` makes
    a trial generate functions the unit never calls. On a refusal's path that can only make fewer
    orders compile, so fewer notes, and never a false *compiles*. That is an inference from the
    mechanism. It was run on curl's door, the toy `warning` header and the loop, and on nothing
    else.

## 9. The diff

`git -C .claude/worktrees/245-ffi-pragmatist diff 7fb6868b2 >
.claude/worktrees/p245-scratch/ffi-pragmatist/build/route.diff`, written after the last edit and
after every run above. It is 993 lines, sha256 `337e8be259b44ce7…`. `git diff --stat`: **31 files
changed, 306 insertions, 89 deletions.**

| part | files | + | − |
|---|---|---|---|
| `selfhost/` | 6 | 271 | 54 |
| `tests/golden/unsupported/*.expected` (15 `-Werror` notes, 9 `...` notes) | 24 | 32 | 32 |
| defect 158's carrier, `fixedbugs-158-noted-outer.h` | 1 | 3 | 3 |

By module (`--numstat`):

| module | + | − |
|---|---|---|
| `emit/header_region.hero` | 103 | 14 |
| `cli/header_order.hero` | 72 | 24 |
| `emit/ffi_past_prototype.hero` | 43 | 6 |
| `cli/order_trial.hero` | 33 | 4 |
| `cli/compiling.hero` | 10 | 4 |
| `cli/name_order.hero` | 10 | 2 |

The prototype's own share is in it: D's marker and refusal in `ffi_past_prototype`, and
`-Wformat-security` out of `FORMAT_DEFINED`. Untracked in the worktree and not in the diff: the
binaries `heroes-base` (the base's stage 2, kept for the census) and `heroes-proto` (the
prototype's). Nothing was committed.

## 10. Rules kept, and one lapse

- No paid run, no `claude -p`, no API call, no credential printed, no GitHub write.
- Every file written is under `.claude/worktrees/245-ffi-pragmatist`, `.claude/worktrees/p245-scratch/ffi-pragmatist/build/`,
  or this report. The critic's `q1/` and `q1curl/`, my own earlier `proto/` probes and the loop's
  two programs were copied into `build/` before any use.
- The only run of a whole form was `surface`, once, at the end. Each binary that might not end ran
  under `timeout 20`, with output through `head -c 200000`. No harness run shared a line with
  `head` or `tail`; the hook refused one attempt, which I then split.
- **One lapse.** A heredoc of mine was unquoted, so the shell ran the words in backticks inside my
  Python text before Python started. It tried `layout` (*command not found*) and
  `emit/header_region.hero` (*no such file*, in the briefs folder), and then hung on `` `wc` ``
  reading its input. It wrote nothing: the draft it was to edit was unchanged (`grep`), and `git
  status` of the p245 worktree showed only this report and another seat's `build-q3.md`. I stopped
  the shell and its `wc`, pids 24474 and 24477, both mine, in my briefs folder, and redid the edit
  with a quoted heredoc.
- **Another process, not mine, left alone.** While that happened, a process whose folder is
  `.claude/worktrees/245-compiler-engineer` (`./heroes-next run ... check fixedbugs-225-`) was
  running. I did not touch it. It overlapped my `surface` run, so `surface`'s `real 99.83` is no
  timing.

## 11. The level, built

Asked by the coordinator after § 10 was written: answer 2 of § 3.3 built as a variant, so that a
program has one verdict at every level. Same worktree, on top of § 1's (d)+S+D.

**What was built.**
- **A new module, `selfhost/cli/region_level.hero`.**
  - `owed`: a unit owes a second compile where its text holds the groups' region
    (`macro_guard.OPEN_LINE`) and the build's level is not `-O0`.
  - `judged`: that compile is a plain `clang @<the unit's own words at -O0> -c -fno-crash-diagnostics
    <unit>.c -o <private object>`, with no `-femit-all-decls`. The object and the stderr are
    discarded. A refusal is handed back as `toolchain.asked_of_clang` hands back the unit's own.
  - `key_text`: what the unit's key names of that compile (below).
  - One test.
- **`cli/units.hero`'s `tu_object`.**
  - After a unit's compile succeeds, it asks `region_level.judged`. A refusal there returns through
    the same path as the unit's own refusal, so every reader of clang's words tells it the same way.
  - The object is published only where both compiles pass, so a warm hit carries both verdicts.
- **The key.** My first build of this variant (`sources 1eac99d8794f6283`) built the `attr_warning`
  shape at `-O2`, exit 0, because the build was served the object the (d) compiler had kept under
  the same key. A unit's key names its text, its words, clang and the Heroes `VERSION`. It names
  neither the compiler's own sources nor the second compile.
  - So the key now names the `-O0` words wherever that compile is owed (`region_level.key_text`,
    as `front_check.key_text` names the front end's), and nothing elsewhere, so no other unit's key
    moves.
  - Rebuilt: `heroes check` exit 0, stage 2 `real 70.36`, **`sources 753fd8fb31f6f8e9`**. Every row
    below is this compiler, beside the (d) compiler kept as `heroes-route` (`sources
    3f20e3710e524f6f`).
- **Found beside, a question rather than a filing.** The base's unit key names `VERSION`, not the
  compiler's sources (panel 222's key). So two builds of one `VERSION` that differ only in what they
  refuse share every warm object, and the later one is served the earlier one's verdict. A change
  that moves no unit's text or words, as this one did before its key line, is invisible to a warm
  cache. I did not look for a tracked change that hit this.

### 11.1. `attr_warning` and curl's checker under `heroes build` and `heroes run`, `-O0` and `-O2`

Each program was copied fresh into `build/level/`, so no cache of mine was reused. The rows are
`heroes-<c> build main.hero <level> -o p` and `heroes-<c> run main.hero <level>`
(`build/level/{a,c}-*.txt`).

| program | compiler | `build -O0` | `build -O2` | `run -O0` | `run -O2` |
|---|---|---|---|---|---|
| `attr_warning` shape | (d) | 1 `ffi_header_refused` | **0** | 1 | **0, prints `1`** |
| `attr_warning` shape | **variant** | 1 | **1** | 1 | **1** |
| curl's checker (`net_set_timeout`, `int32_t`) | (d) | 1 | 1 | 1 | 1 |
| curl's checker | **variant** | 1 | 1 | 1 | 1 |

- **One verdict both ways: yes.** The variant refuses the shape at all four, where (d) alone gives
  two verdicts. Every refusal is `[-Werror,-Wattribute-warning]` at the header's line 2.
- The variant's `-O2` refusal text is the `-O0` one: clang's line, the `-Werror` note and `REPAIR`.
- No refusal carries an order note: `grep -c 'compiles after\|these compile when\|compiles when'`
  printed 0 for each of curl's four.
- **Owed:** nothing in the `-O2` refusal says the `-O0` compile is what refused it. Compiled at
  `-O2` by hand, that header draws no word from clang, so a note naming the level is owed
  (design.md §4.17). Unbuilt.

### 11.2. A header with an unused `static inline` calling a `warning`-marked function

`build/level/unused-a/lib.h` holds a `warning`-marked `warned`, `never_called(x) { return
warned(x); }`, and `door(x) { return x + 1; }`.
- `unused-a` binds `door` alone.
- `unused-b` binds `door` and `never_called`, and calls only `door`.
- The control `long` is the curl door that hands a `long` (§ 3.3's `q1curlp/long`).

| program | (d), four ways | variant, four ways |
|---|---|---|
| `unused-a` | exit 0 at `build -O0`, `build -O2`, `run -O0`, `run -O2`, silent, prints `2` | the same |
| `unused-b` (bound, never called) | the same | the same |
| `long` | exit 0 four ways, prints `43` | the same |

**Built at both levels: yes**, silent. The second compile generates only what the unit
generates, as `-O0` does, so `never_called`, bound and not called, drew nothing. The measurement is
the exit 0. Why the compiler's own references to a binding generate no call is my inference,
unread. A second compile with `-femit-all-decls` would have generated `never_called`: § 0.7's
`aw.c`, the same shape, exits 1 with it at `-O0`.

### 11.3. The cost, in instructions retired

`/usr/bin/time -l` on `clang` counts Apple's launcher, `/usr/bin/clang`, and not the compile. Six
compiles from a 6 KB unit to a 4.2 MB one all read 76.8 to 80.3 million, while user time ran from
0.02 s to 2.03 s (`build/cost/instr2.txt`). Calling the toolchain's clang (`xcrun -f clang`)
directly also counted only the driver: 59 million for 0.27 s of user time. So each compile was run
as clang's in-process job:

```
/usr/bin/time -l sh -c "exec $(xcrun -f clang) -fintegrated-cc1 -isysroot $(xcrun --show-sdk-path) \
  @<the unit's words> -c -fno-crash-diagnostics <unit>.c -o x.o"
```

That is the unit's `.c` and word files, exactly as the variant's `-O2` build wrote them, run from
the program's folder. The `-isysroot` is what the launcher adds: without it, *'math.h' file not
found*. Every run exited 0; there were four of each, the first discarded (`build/cost/instr3.txt`).

| unit | size | the unit's `-O2` compile (the base) | the `-O0` second compile | added |
|---|---|---|---|---|
| `unused-a`'s main | 6,371 B | 175.7 M, 175.8 M, 176.9 M | 167.0 M, 167.3 M, 167.6 M | **+95 %** |
| `examples/ledger`'s main | 113,876 B | 2,367.2 M, 2,369.8 M, 2,370.6 M | 716.8 M, 716.9 M, 717.0 M | **+30 %** |
| `fixedbugs-2320`'s main | 4,237,573 B | 19,289 M, 19,316 M, 19,341 M | 4,583 M, 4,600 M, 4,629 M | **+24 %** |

User time at `-O2` and `-O0`: 0.01 s and 0.01 s, 0.24 s and 0.07 s, about 1.6 to 1.8 s and 0.4 to
0.5 s.
- The added share is the `-O0` count over the `-O2` one, best of three each.
- It is paid once per **cold** compile of each unit holding a region, at a level above `-O0`, on
  success as on refusal. `examples/ledger` has two such units (its `main` and `dbsqlite`, 98 KB).
- A warm build pays nothing.
- A unit with no group's header pays nothing.
- A build at `-O0`, `heroes build`'s default, pays nothing.


### 11.4. The census of the 496 roots at `-O2`

**The first attempt is discarded.** The data volume filled at about 02:25 (the coordinator's word).
My first `-O2` censuses ran from 02:10 to past that time, and they read:
- eight of defect 582's cases failing at the base's `-O2`: seven at exit 1 with their stderr file
  missing, one at exit 2 with an empty one;
- 114 lines of *No space left on device* in the variant's log.

The eight, rebuilt by the same script at 02:21, built at exit 0. That rerun overlapped the variant's
census for about a minute: a lapse of *one heavy command at a time*.

After the coordinator freed space, I removed my own folders' build products:
- the worktree's `build/` (828 MB);
- the probe folders' `build/` caches;
- the discarded census outputs.

Every census below then wrote into a clean output folder. The worktree's `build/` (171 MB after
each) was deleted once its table was written. Free space read 22 to 23 GB throughout.

The command each time: `LEVEL=<level> ROOTS=build/census-roots2.txt build/census.sh <tag>
<compiler>`, one build at a time, from the worktree's root. Each table has 496 rows and 496 stderr
files.

| census | compiler | level | built | refused | stderrs with a `warning:` | `real` |
|---|---|---|---|---|---|---|
| § 7 | (d) `3f20e371` | `-O0` | 491 | 5 | 0 | 417.87 |
| `base-O2` | base `0fe313ba` | `-O2` | 492 | 4 | 0 | 379.93 |
| `level-O2` | variant `753fd8fb` | `-O2` | 491 | 5 | 0 | 400.59 |
| `level-O0` | variant `753fd8fb` | `-O0` | 491 | 5 | 0 | 421.99 |

What moved (`build/census-compare.py`, tables in `build/census/{route,base-O2,level-O2,level-O0}/`):
- **The variant at `-O2` against the base at `-O2`: one program,**
  `run/fixedbugs-2036-a-header-s-own-printf-of-a-format-held-in-a-variable-builds`. That is S, as at
  `-O0` (§ 7). The second compile moved no other root.
- **The variant at `-O0` against the variant at `-O2`: none.** Each of the 496 roots has one
  verdict at both levels: 491 built and the same 5 refused.
- **The variant at `-O0` against (d) at `-O0` (§ 7): none.** The second compile does not run at
  `-O0`, and the two tables say so.
- **Refused at every level by every compiler, for their environment:** the two `fixedbugs-2428`
  cases (`ffi_pkgconfig`), `fixedbugs-437` and `fixedbugs-589` (`ffi_missing_header`).
- **No clang warning in any of the three runs' 1,488 stderrs.** The only `-W` word anywhere is the
  `-Wformat-security` of S's refusal.

None of the `real` figures is a timing: the machine was not still. They do not price the second
compile either. On this tree the census pays that compile only for the units that hold a region,
and the warm build pays none.

### 11.5. What the variant still owes

1. **A note naming the level** (§ 11.1): an `-O2` build refused by the `-O0` compile says nothing
   of `-O0`.
2. **Its cases.** The `attr_warning` shape refused at `-O2`, and `unused-b` built at `-O2`, each as
   a golden case (§ 8's item 9).
3. **`layout`.** `./heroes-next run tests/harness/main.hero -- ./heroes-next layout units` read 3
   passed and 1 failed: `selfhost/cli/units.hero` now has 318 lines of code. `layout region_level`
   read 4 passed. That is a fourth split beside § 8's three. The call site in `units.hero` is 11
   lines with its comment. Moving the call and the key line into `region_level` should take `units`
   back near where it stood, which is an inference, unrun. Unmeasured at the base, since the hook refuses a harness run by a
   compiler built from other sources than the tree.
4. **The compiler's own tests**, `region_level`'s new test among them, unrun as in § 8.
5. **The key question** of § 11's found-beside item: a warm object of another compiler build of
   the same `VERSION` is served.
6. **The platforms.** An `-O0` compile of each region unit at `-O2` on Linux, where glibc's
   headers read `__OPTIMIZE__` and so say other things at `-O0` than at `-O2`. Unrun.

### 11.6. The diff

`build/route-level.diff` is `git diff 7fb6868b2` of the worktree, followed by `git diff --no-index
/dev/null selfhost/cli/region_level.hero` (the new module is untracked, and I did not touch the
index). It was written after the last edit, and regenerated and checked by `cmp` after every run
above.
- 1,148 lines, sha256 `d4f67f1070275ed4…`. **33 files, 425 insertions, 92 deletions.**
- `selfhost/` holds 8 of them: the 7 tracked modules (+294, −57) and `cli/region_level.hero` (96
  lines, new).
- The other 25 are the 24 expectations and the carrier of § 9, unchanged since.
- Over `route.diff` (§ 9) it adds `cli/units.hero` (+23, −3) and `cli/region_level.hero` (+96).

`heroes-route`, the (d) compiler kept beside the variant, is one more untracked binary in the
worktree. Nothing was committed.
