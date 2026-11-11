# Panel 245, the build of Q3's P1 with P4's note: the compiler-engineer

Written 2026-11-11 from 01:04, as the work went, on `docs/panel/245-briefs/build-q3.md`. Every
number below is from a command run in this session, named beside it; a sentence with no command
is marked *unrun* or *a question*. The machine was loaded the whole time (`uptime` read 33.64 at
01:04, batch 30's suites and another seat beside), so no figure here is a duration anyone should
quote; the one comparison timed, § 9's, is an indication and says so.

## The base

- Worktree `.claude/worktrees/245-compiler-engineer`, `git log -1` = `7fb6868b20df`, `git status
  --short` empty at 01:04.
- The base compiler is the one my first report built there: `heroes-base --version` prints
  `heroes 0.6.0`, `sources 0fe313ba58ba197c`, the key every seat's compiler printed.
- The reproducers were copied first, sources only, into
  `p245-scratch/compiler-engineer/build/repro/` (the critic's `q3/rl`, `shim`, `lzma`, `dot`, its
  `p1.py`; the ffi-pragmatist's `curl/main.hero`, `rl/`, `t.c` and `pairing.tsv`). The stray
  `libcurl.dylib` of `q3/dot` was rebuilt there from the critic's `fake.c` with the install name
  its own carries (`otool -D` of theirs: `@rpath/libcurl.dylib`).
- The environment: `printenv LIBRARY_PATH` prints `:/opt/homebrew/lib`, the author's profile's.
- Rules kept: no paid run, no GitHub write, no credential, no commit; builds and runs only in my
  worktree and `p245-scratch/compiler-engineer/build/`; one heavy command at a time. **One lapse**:
  I drafted two notes for this report (`owes.md`, `verdict.md`) in the session's scratchpad under
  `/private/tmp/`, outside the repository's root. I removed both at 02:20 (`ls` then said *No such
  file*); their text is §§ 8 and 10 here. And in my own worktree I restored `assemble.hero` once
  with `git checkout --` to re-apply a smaller edit of my own (§ 3); it discarded nothing but
  that edit.

## 1. What the link job reads from `LIBRARY_PATH` (the library half's question)

- Command, in `build/lp/`: `clang -### t.o -L/x/named -lm -o a.out` with the profile's variable,
  then under `env -u LIBRARY_PATH`, then `LIBRARY_PATH=/a::/b`.
- Printed: with the profile's, the ld job carries `"-L/x/named" "-L." "-L/opt/homebrew/lib"
  "-L/usr/local/lib"`; unset, `"-L/x/named" "-L/usr/local/lib"`; with `/a::/b`, `"-L/a" "-L."
  "-L/b" "-L/usr/local/lib"`. So clang writes each element of the variable as an `-L` after the
  line's own, an empty element as `-L.`, and adds `-L/usr/local/lib` on its own whether or not the
  variable is set. `clang -Wl,-v` lists `.`, `/opt/homebrew/lib`, then the SDK's `usr/lib` and
  `usr/lib/swift` as ld64's search paths.
- Command: a two-line C program linked by hand with `-Wl,-map`, once against the stray
  `libcurl.dylib` through the profile's `-L.`, once with `-L/opt/homebrew/opt/readline/lib`.
- Printed: the map names the files `./libcurl.dylib` and
  `/opt/homebrew/opt/readline/lib/libreadline.dylib`, the `-L` folder as written and the file's
  name, and gives `_history_get.stub` and `_history_base.got` that file's index. So the folder
  that held a library file is its path's folder, read lexically, and a data name is traced as a
  function is (`supplied_read.mapped` already reads `.got`).

## 2. Where each name's declaring header comes from (the critic's item 22, its question)

- Command, in `build/repro/rl/`: `clang -fsyntax-only -Xclang -ast-dump -I . hist.h > dump.txt`.
- Printed: 257,145 bytes, 2,792 lines, `real 0.04`. `history_get` is a top-level `FunctionDecl`
  row with a relative location (`<line:153:1, col:28>`), and the last absolute location above it
  is the SDK's `usr/include/readline/readline.h`.
- So each name's declaring file is not on its own row: it is the file of the last absolute
  location the dump printed above it, which `cli/label_dump.hero`'s `located` already reads in the
  dump's order, and `cli/header_bodies.hero`'s `of_dump` already reads off the same dump which
  names each body calls and reads (defect 2372's reader, on the link's failure path until now).
  `cli/header_types.hero:9`'s `-ast-dump=json` was not needed; the text dump that
  `defined_twice.dump_of` already writes answers it.
- **The critic's item 22 holds more strongly than it says.** The names a group BINDS are not
  enough: in `q3/rl` every bound name is a `static inline` door in the program's own `hist.h`,
  supplied by no library, and the fault is in the names those doors call (`history_get`,
  `replace_history_entry`, the data name `history_base`). A P1 that judged bound names alone,
  or `supplied.hero`'s traced names as they stand (bound names and the runtime's), would pass
  2448's own measured fault. So the build judges every name a bound name reaches through the
  bodies its header list defines, followed breadth first.

## 3. What was built

In `.claude/worktrees/245-compiler-engineer`, every file formatted by `heroes fmt --in-place`
when written, `heroes-next check selfhost/main.hero` exit 0 after each change, and stage 2 rebuilt
from the seed's stage 1 (`./heroes build selfhost/main.hero -o heroes-next`) after each: five
times, `real 61.08`, `66.06`, and three not timed, each `exit 0`. The last prints `heroes 0.6.0`,
`sources 125475c31f2da42a`; it is `build/heroes-p1`. The tables of § 4 were first run with the
fourth (`244ce7a0279fa241`), and every row was run again with the fifth, which moved only where
`assemble.hero` asks the header half (below): each verdict and each printed line the same
(`build/final-all-p1.txt`).

- **`selfhost/cli/supplied_paired.hero`, new: the judge.**
  - *The header half* (`of_probings`): for each probed header list, one `defined_twice.dump_of`
    (the text dump), each top-level function's and object's declaring files in the dump's order
    (`declaring`, through `label_dump.located`), the names each bound name reaches
    (`reached`, through `header_bodies.of_dump`), and for each name its first declaring file that
    the program's own folder did not find, with the named folder that found it
    (`stand_in.finder` over the folders panel 237 counts: the program's, `--include`'s, a
    package's `-I`, and the environment's, `CPATH`, `C_INCLUDE_PATH`, `CCC_OVERRIDE_OPTIONS`; the
    runtime's, the platform's and clang's own configuration's, `/usr/local/include`, are not).
    Asked only where the link line names a library.
  - *A ruling the build had to make, which the critic's wording does not cover*: a name only the
    program's own folder declares is not judged. A prototype the program writes itself is its own
    word, of no release. That without this rule a program whose own header declares `cos` and
    links `m` would be refused is an inference from the code, unrun. Whether the rule is right is
    the sitting's question.
  - *The library half* (`searched`, `judged`): the line's own `-L` words, `--library`'s and a
    package's, are named; clang's `-###` of the link line is asked once, where some judged name
    was supplied by a library the program names, and each `-L` of its ld job that an element of
    `LIBRARY_PATH` names (`clang_added.element_names`, an empty element naming `.`) is named too;
    every other `-L` of the job is clang's own. A library file is named where its folder is a
    named one and not one of clang's own. On Windows the job is not asked (lld-link reads no
    `LIBRARY_PATH`; unrun).
  - *The verdict*: for each name whose supplier the real link names is a library the program
    names (`supplied_names.naming`), a `paired` marker where exactly one of the two halves is
    named.
- **`selfhost/cli/supplied_paired_words.hero`, new: the telling.** `ffi_unpaired_release`, one
  diagnostic per library and pair of folders on the first name's binding, every name in a note,
  the declaring header and the defining file each in a note, then the remedy. No `Fix`.
- **`selfhost/cli/supplied.hero`**: `Judging` carries the declared names; `traced` traces them
  too, so the real link's own map or trace says who supplied each; `after_link` judges them from
  the same answer and refuses beside the existing findings, the paired markers appended after
  `own_lines` so no `own` line is written for them. No new link is run.
- **`selfhost/cli/supplied_words.hero`**: the bound-name loop skips `paired` markers; their
  diagnostics are appended.
- **`selfhost/cli/assemble.hero`**: `linked_round` takes a `supplied_paired.Asking` (the
  program's declarations, its source and its probings) and hands `supplied_paired.asked_of` to
  the judging, which asks the header half only where the line holds an `-l` word. +5/−3. The first
  build put the record and the guard here and took the file to 313 lines of code, which
  `layout assemble` refused (*"313 lines of code, past §11's 300"*); moved into the new module, it
  is 295 and `layout assemble` reads 4 passed, 0 failed.
- **`selfhost/head_names.hero:222`**: P4's note, my route's file copied over, byte for byte the
  `routes/P4` text.
- **A refusal, not a warning**: the finding goes out as the link's own refusal (`clang_link`,
  read by `whose.hero`'s link branch), exit 1, and nothing the link wrote is published.

Tests, each by `heroes-next test selfhost/main.hero --only <title>`:
- `--only "(panel 245)"`: 3 tests, all passed (the dump's declaring files and the reached names;
  the program's own folder passed over; one refusal per library and pair, the remedies' exact
  words).
- `--only "asks its link nothing (panel 239)"`, the existing test whose call I changed: 1 test,
  passed.

**Four repairs the runs forced, each before the final compiler** (§ 4 holds the runs):
1. The first build told one diagnostic per name, seven for `q3/rl` (62 lines), a second message
   for one mistake. Grouped by library and folder pair: one.
2. The first remedy for a platform header beside a named library, *"name the header's folder
   too, with `--include <dir>` or `CPATH`"*, is **false**: followed on `q3/rl` it is refused
   `shadowed_header`, by the base as well (`rl-followed`, `rl-cpath-lib`: *"`.../readline.h`
   stands in for the system's `<readline/readline.h>`, which `./hist.h` includes"*). Panel 237's
   rule refuses any named folder holding a header the platform ships, reached through another
   header. Removed.
3. The remedy *"or name the library with `pkgconfig`"* is **false** where `LIBRARY_PATH` named the
   library's folder: in the stray's folder `pkgconfig "libcurl"` still linked `./libcurl.dylib`
   (`dot-pc`: the base builds it, P1 refuses it again), since the SDK's shim answers `-lcurl` alone
   (`pkg-config --libs --cflags libcurl` printed `-lcurl`) and `-L.` is searched first. Removed for
   the variable's case.
4. *"take `.` out of `LIBRARY_PATH`"* named a `.` the variable does not hold (`:/opt/homebrew/lib`).
   It now says *"take the empty element out of `LIBRARY_PATH` for this build, which names `.`, the
   folder the build runs in"*.

## 4. The reproducers, base and P1

Driver: `build/case.sh <compiler> <case> <src> <main> [VAR=v | -VAR] -- [flags]` copies the
sources into `build/runs/<compiler>/<case>/`, runs `heroes-<compiler> build <main> <flags> -o p`
under the environment given (`-VAR` unsets one), and, where it built, runs `timeout 20 ./p 2>&1 |
head -c 200000 > r.txt` and reads `otool -L p`. Every row is one run of it; the tables are
`build/rl-table.txt`, `build/other-table.txt` (base and an earlier P1) and
`build/final-critic-p1.txt` (the final P1). `R=/opt/homebrew/opt/readline`,
`H=/opt/homebrew/include`, `K=/opt/homebrew/opt/curl`; "plain" keeps the profile's
`LIBRARY_PATH=:/opt/homebrew/lib`.

### The critic's `q3/rl`, readline (prints: size, header, library, no-data null, first byte, own record back)

| build | base | P1 |
|---|---|---|
| plain | 0, `16 1026 1026 1 -1 42`, libedit | 0, the same |
| `--library $R/lib` | 0, `16 1026 2051 0 0 0`, `libreadline.8.dylib` | **1** `ffi_unpaired_release` |
| `LIBRARY_PATH=$R/lib` | 0, `16 1026 2051 0 0 0`, `libreadline.8.dylib` | **1** `ffi_unpaired_release` |
| `pkgconfig "readline"`, `PKG_CONFIG_PATH=$R/lib/pkgconfig` | 0, `24 2051 2051 1 -1 42` | 0, the same |
| `--library $R/lib --sanitize` | build 0, run **134**, *AddressSanitizer: heap-buffer-overflow ... READ of size 8* in `own_data_back` | **1** `ffi_unpaired_release` |

The base's rows reproduce the critic's item 16 with my compiler, the fault included. P1's
refusal of `--library $R/lib`, whole:

```
error[ffi_unpaired_release]: `using_history` and 6 more names this group's bindings reach are declared by a header no folder this build names holds, and defined by a library of `/opt/homebrew/opt/readline/lib`, a folder this build names: they may be two releases of library `readline`, and nothing holds one to the other
  at main.hero:2:5
  note: the names: `using_history`, `add_history`, `history_get`, `history_base`, `rl_readline_version`, `where_history`, `replace_history_entry`
  note: the declaration clang read is in `/Applications/Xcode.app/.../MacOSX.sdk/usr/include/readline/readline.h`
  note: the definition the link took is in `/opt/homebrew/opt/readline/lib/libreadline.dylib`
  note: a library's header and its definitions come both from folders the build names, or neither: take `/opt/homebrew/opt/readline/lib` out of `--library` or a package's answer for this build, so the platform's library is linked with the platform's header, or name the library with `pkgconfig` and let `pkg-config` answer both
```

Both remedies were followed and build: the plain row (the folder taken out) and the `pkgconfig`
row. Under `LIBRARY_PATH` the note offers the first alone (§ 3, repair 3). No binary is left in a
refused build's folder (`ls runs/p1/rl-lib`: `b.txt build hist.h main-pc.hero main.hero`).

**The remedy P1's first note named, the header's folder named too, is refused on this
reproducer by the base already**: `--include $R/include --library $R/lib` and `CPATH=$R/include
--library $R/lib` are each `shadowed_header` at exit 1 under both compilers (*"stands in for the
system's `<readline/readline.h>`, which `./hist.h` includes"*, note *"rename the file, or move it
out of ..."*). P4's note is not this one: it is told for a group naming a header by its path, a
group's own header, which panel 237 does not judge as a stand-in (my first report followed it to
exit 0). Panel 237's rule and P1 together leave a program whose own header reaches a library
header the SDK also ships two routes to the keg's release: a package, or a group naming the
library's header itself.

### The critic's `q3/shim`, `q3/lzma`, `q3/dot`

| build | base | P1 |
|---|---|---|
| shim, `pkgconfig "libcurl"`, no `PKG_CONFIG_PATH` | 0, `header 526081`, `libcurl/8.7.1`, `/usr/lib/libcurl.4.dylib` | 0, the same |
| lzma plain | 1 `ffi_missing_header` | 1, the same |
| lzma `--include $H` (profile's `LIBRARY_PATH`) | 0, `50080042` and `50080042` | 0, the same |
| lzma `--include $H`, `LIBRARY_PATH` unset | 0, `header 50080042 library 50040032`, `/usr/lib/liblzma.5.dylib` | **1** `ffi_unpaired_release` |
| lzma `CPATH=$H` (profile's) | 0, `50080042` and `50080042` | 0, the same |
| lzma `CPATH=$H`, `LIBRARY_PATH` unset | 0, `50080042` and `50040032` | **1** `ffi_unpaired_release` |
| lzma `--include $H --library /opt/homebrew/lib`, unset | 0, `50080042` and `50080042` | 0, the same |
| lzma `pkgconfig "liblzma"`, unset | 0, `50080042` and `50080042`, the keg's | 0, the same |
| dot, the stray `libcurl.dylib` in the build's folder | build 0, run 134: *"dyld: Library not loaded: @rpath/libcurl.dylib ... no LC_RPATH's found"* | **1** `ffi_unpaired_release` |
| dot, `LIBRARY_PATH` unset | 0, `header 526081`, `libcurl/8.7.1` | 0, the same |
| dot, `pkgconfig "libcurl"` (the shim) | build 0, links `@rpath/libcurl.dylib`, run 134 the same | **1** `ffi_unpaired_release` |
| dot, `--include $K/include` (profile's) | build 0, links `@rpath/libcurl.dylib` | **0**, links `@rpath/libcurl.dylib` |

- The stray's run: with no loader variable my binary dies at start; `DYLD_LIBRARY_PATH=. ./p`
  prints `header 526081` and *"library a stray libcurl in the working folder"*, the critic's
  line. `printenv DYLD_LIBRARY_PATH` and `printenv DYLD_FALLBACK_LIBRARY_PATH` print nothing here,
  and the critic's binary carries no `LC_RPATH` either (`otool -l` of theirs), so how theirs ran
  is unread.
- lzma's refusal's note: *"pass `--library <dir>` with the folder holding the library of
  `/opt/homebrew/include`'s release, or name the library with `pkgconfig` and let `pkg-config`
  answer both"*; both were followed and build (the last two lzma rows). Its declaration is read
  in `/opt/homebrew/include/lzma/version.h` and its definition in the SDK's `usr/lib/liblzma.tbd`.
- The dot refusal's note: *"take the empty element out of `LIBRARY_PATH` for this build, which
  names `.`, the folder the build runs in, so the platform's library is linked with the platform's
  header"*; followed, it is the `LIBRARY_PATH` unset row.
- **P1's residue, measured**: the last dot row. The keg's 8.23.0 header named by `--include` and
  the stray library named by `LIBRARY_PATH`'s empty element are both named, so P1 builds it at
  exit 0, and under `DYLD_LIBRARY_PATH=.` it prints `header 530176` beside the stray's string.
  Two halves named by unrelated words is a pair P1 cannot judge; only a rule about which folder
  belongs to which release could (P2), and the seat's objection to P2 stands.

### The ffi-pragmatist's eleven rows (`build/ffi-rows.txt`)

Their `pair.py`'s words and environment for each row, their `curl/main.hero` (`extern
"curl/curl.h" link "curl"`) and `rl/main-link.hero` and `main-pc.hero`.

| row | base | P1 | their P1 column |
|---|---|---|---|
| curl-plain | 0, `526081`, `8.7.1` | 0 | pass |
| curl-cpath | 0, `530176`, `8.7.1` | **1** | refuse |
| curl-inc | 0, `530176`, `8.7.1` | **1** | refuse |
| curl-inclib | 0, `530176`, `8.23.0` | 0 | pass |
| curl-cpathlp | 0, `530176`, `8.23.0` | 0 | pass |
| curl-lib | 0, `526081`, `8.23.0` | **1** | refuse |
| curl-lp | 0, `526081`, `8.23.0` | **1** | refuse |
| rl-plain | 0, `16 1026 1026 1 -1 42` | 0 | pass |
| rl-lib | 0, `16 1026 2051 0 0 0` | **1** | refuse |
| rl-lp | 0, `16 1026 2051 0 0 0` | **1** | refuse |
| rl-pc | 0, `24 2051 2051 1 -1 42` | 0 | pass |

**The compiler's P1 agrees with the hand column on 11 of 11**, and with the critic's computed one
(item 17). `curl-lib` and `curl-lp`, the SDK's 8.7.1 header with the keg's 8.23.0 library, are the
price the critic named: curl's supported old header with a newer library, refused, its repair one
flag (`--include $K/include`, the `curl-inclib` row) or the folder taken out.

## 5. The forms, narrowed by the harness's third word

Each run is `./heroes-next run tests/harness/main.hero -- ./heroes-next <form> <word>` from the
worktree's root, the final P1 compiler judging and running the harness, each to its own file
under `build/final-forms/`. The base was not run through the forms: the trunk at `7fb6868b2`
passed them at batch 30's gate, and the hook in front of the net refuses a compiler whose
`sources` key is not the tree's.

- **Which cases P1 can move.** P1 runs after a link that succeeded, so `check`, `permissive`,
  `full`, `ir`, `emit` and `emission` (no link) cannot move under it, and an `unsupported` case
  only where its refusal comes from the link's own judging. The cases that name a library, by
  `git ls-files tests/golden/<form> | grep '\.hero$' | xargs grep -lE '^extern .*(link|pkgconfig)
  "'`: `run` 22 files, `unsupported` 14, `check` 22, `permissive` 3, `fixedbugs` 5, `full`,
  `control`, `emit` and `ir` 0.
- **`run`**, fifteen selections covering the 22 files (`ffi-libm`, `ffi-a-c-array-member`,
  `ffi-a-construction-polls`, `ffi-constant`, `fixedbugs-2056-a-header-s-own-macro`,
  `fixedbugs-2291`, `-2395`, `-2428`, `-2429`, `-361`, `-396`, `-413`, and the three
  `fixedbugs-a-...` cases by their whole names): **42 passed, 0 failed**, each selection exit 0.
  The selections hold more cases than the 22 (`fixedbugs-396` selects 15). A first pass with the
  fourth compiler read the same, 42 and 0; in it the prefix `fixedbugs-a-` selected 103 cases, a
  near-whole form, which I stopped after its first minutes (`pkill`, then `pgrep` found no
  process left) and replaced by the three names.
- **`unsupported`**, eight selections covering the 14 files (`ffi-incomplete-record`,
  `ffi-records-only-group-cannot-claim`, `fixedbugs-2056`, `-2059`, `-226-`, `-237-`, `-2428`,
  `-2429`): **20 passed, 0 failed**. `fixedbugs-2059`'s two, refused by panel 239's link judging
  beside which P1's markers are written, read 2 and 0.
- **`check`**, P4's three: `fixedbugs-216-` 5 passed, 1 failed; `fixedbugs-225-` 0 and 1;
  `fixedbugs-250-` 0 and 1. The three failures are
  `check/fixedbugs-216-a-backslash-in-a-header-name`,
  `check/fixedbugs-225-a-climb-to-the-root-is-a-known-cost` and
  `check/fixedbugs-250-an-escape-in-a-header-name-is-told-once-by-the-head`, each differing only in
  the note P4 rewrites (`--- expected ---` *"set `CPATH` to the directory holding it, or name its
  library with `pkgconfig` and let `pkg-config` answer"*, `--- actual ---` P4's words). **P4 moves
  exactly the three goldens that pin its old words**, which my first report predicted; their
  `.expected` files are not rewritten here.
- Unrun: every form whole, `emission`, `warnings`, `corpus`, `compat`, `surface`, and the
  compiler's own tests whole.

## 6. The tracked programs that name a library

- The list: `git ls-files 'examples/*.hero' 'tests/golden/run/*.hero' 'tests/golden/run/**/*.hero'
  | xargs grep -lE '^extern .*(link|pkgconfig) "'` printed 30 files: 8 under `examples/` (the
  `ledger` one a module of `examples/ledger/main.hero`), 22 under `tests/golden/run/`. It rests on
  the group head being one line, which every group in the tree is; a lower bound otherwise.
- **`examples/`**, by `build/examples.sh <compiler>` from the worktree's root, each binary into
  `build/ex/<compiler>/`, the six that open no window run under `timeout 20 ... | head -c 200000`:

| example | base | P1 |
|---|---|---|
| `curl` | 0, run 0, 139 bytes, md5 `07c1644b...` | the same |
| `gallery/08-ffi` | 0, run 0, 60 bytes, `f7f1d847...` | the same |
| `ledger` | 0, run 0, 1,003 bytes, `67d355d4...` | the same |
| `nbody` | 0, run 0, 26 bytes, `5b8f3d2f...` | the same |
| `spectral` | 0, run 0, 12 bytes, `eff6e24f...` | the same |
| `sqlite` | 0, run 0, 19 bytes, `94afa3bd...` | the same |
| `raylib` (`pkgconfig "raylib"`) | 0, not run (a window) | 0 |
| `sdl` (`pkgconfig "sdl3"`) | 0, not run (a window) | 0 |

  `diff` of the two tables, the compiler's name taken off, printed nothing: **no example moves.**
- **That `examples/curl` passes because the judging ran, not because nothing was judged**: a copy
  of it (`build/repro/excurl/`) built with `--library /opt/homebrew/opt/curl/lib` builds at the
  base (prints `libcurl/8.23.0 ...` beside the SDK's 8.7.1 header) and is refused by P1,
  *"`curl_version` and 4 more names this group's bindings reach ..."*, the names note reading
  `curl_version`, `curl_easy_init`, `curl_easy_setopt`, `curl_easy_strerror`, `curl_easy_cleanup`.
  `curl_easy_setopt` is called only inside `curl_bound.h`'s door `net_set_url`, so the per-name
  reading reached it through the door's body. Its group's own header, `curl_bound.h`, stands in
  the program's own folder, which panel 237 counts as named (`named_folders.of_compiler`'s
  `PROGRAM`): judged by its group's header alone, the plain build would pair a named header with
  the SDK's library and be refused. That last sentence is an inference from the code; the
  per-group reading was not built.
- The same copy with `--include /opt/homebrew/opt/curl/include --library .../lib` is
  `shadowed_header` under both compilers (*"`.../curl/curl.h` stands in for the system's
  `<curl/curl.h>`, which `./curl_bound.h` includes"*), so this example has no `--include` route to
  the keg's release either, as § 4 found for readline.
- **`tests/golden/run/`**: § 5.

## 7. The lines, in `code_lines`' unit

`code_lines` (`tests/harness/suite_layout.hero:1028`) counts every non-blank line outside a `test`
block, comments included. Counted with that rule by `awk` for each touched file, the base's from
`git show 7fb6868b2:<file>`, and the instrument asked for its verdict on the same files
(`heroes-next run tests/harness/main.hero -- ./heroes-next layout <word>`):

| file | base | P1 | delta | `layout` |
|---|---|---|---|---|
| `selfhost/cli/supplied_paired.hero` (new) | 0 | 276 | +276 | `supplied`: 4 passed, 0 failed |
| `selfhost/cli/supplied_paired_words.hero` (new) | 0 | 125 | +125 | (the same selection) |
| `selfhost/cli/supplied.hero` | 252 | 274 | +22 | (the same selection) |
| `selfhost/cli/supplied_words.hero` | 288 | 291 | +3 | (the same selection) |
| `selfhost/cli/assemble.hero` | 293 | 295 | +2 | `assemble`: 4 passed, 0 failed |
| `selfhost/head_names.hero` | 292 | 292 | 0 | `head_names`: 4 passed, 0 failed |
| **total** | | | **+428** | |

- Without comments, the two new modules hold 209 and 96 lines of code before their `## Tests`
  (`grep -cvE '^\s*(#|$)'`).
- `git diff 7fb6868b2 --numstat`: +601 and −11 lines over six files, tests and comments included.
- Where they land: the CLI's link stage alone (`cli/`), and one note's words in
  `selfhost/head_names.hero`. No lexer, checker, IR, lowering or emitter line moves, so P1 is no
  core construct (design.md §1.7); it adds one diagnostic class.
- **My first report's estimate was 200 to 250 lines over five modules. Built, it is 428 in the
  instrument's unit, 328 without blank lines, comments or tests (`awk '/^## Tests/{exit}' | grep
  -cvE '^\s*(#|$)'`, base against P1, per file: +209, +96, +18, +3, +2, 0), over six files.** The estimate missed what this
  build found: judging per name needs the names the doors' bodies call, and each refusal's remedy
  had to be measured and cut to the ones that build.

## 8. What P1 still owes

Built and run here is the rule and its telling, on this Mac. Not built, or built and unrun:

1. **Its words in the specification and design.md.** A new diagnostic class,
   `ffi_unpaired_release`: a design.md §4.19 paragraph, a spec § 13 sentence, and the fifteenth
   member of `.claude/rules/c-boundary.md`'s list (it moves the line between accepted and refused,
   on robustness's rank). None is written; the class's name is mine and provisional.
2. **The ruling on the program's own declarations** (§ 3): a name only the program's own folder
   declares is not judged. The critic's wording does not cover it, and the sitting has not ruled.
3. **The tie on the library side.** A folder the line names that clang's own job also adds is
   counted named here, where the header side gives such a tie to the platform
   (`stand_in.finder`). On Linux, `--library /usr/lib/<triple>` names a folder clang's driver adds
   as `-L` on its own; this build would count it the program's and refuse a platform header beside
   it. That is an inference from the code, unbuilt and unrun on Linux.
4. **Linux and Windows.** The names a door's body calls are traced by the same `traced` list
   (`--trace-symbol` on GNU ld and lld, `/MAP` on Windows) and read by the same
   `supplied_read.taken`; unrun on both. On Windows no `-###` is asked (lld-link reads no
   `LIBRARY_PATH`, design.md:3128-3133), so the line's own `-L` words alone are named; unrun.
5. **Its residue, measured, which no route in the sitting closes**: two halves each named, by
   unrelated words, build at exit 0 (§ 4, `dot` with `--include`); and the loader at run time
   (the critic's `DYLD_LIBRARY_PATH=/usr/lib` over a paired build).
6. **The remedies it can offer are bounded by panel 237's stand-in rule**, measured: where the
   header is the platform's and another header reaches it, naming the keg's include folder is
   `shadowed_header` (readline, `examples/curl`), so the note offers the library's folder taken
   out, or `pkgconfig` where the build's own words named that folder. Whether 237's rule should
   let a folder stand in for a header that the same build's `--library` pairs with is a question
   for the sitting, unasked by any seat.
7. **Its cases as goldens.** `q3/rl`, `lzma` with `LIBRARY_PATH` unset and the stray in `.` need
   the Homebrew kegs or a stray library, so tracked cases need a library the case builds itself
   (as `fixedbugs-2428`'s `.case` folders carry their own `pc/`) and an `environment` sidecar.
   None is written.
8. **Its cost on a build that names a library** (§ 9): one text dump per probed header list and
   one `clang -###` per link where a named library supplied a judged name.

## 9. What it costs a build, and a warm build judged again

- **Which builds pay**: one that names a library (`asked_of` asks the header half only where the
  link line holds an `-l` word). The compiler's own build names none:
  `grep -rn '^extern .* \(link\|pkgconfig\) ' selfhost` prints nothing. So the latency
  instrument's three clocks (`heroes check`, clang on the seed, the warm self-build) never run P1;
  that they do not move is an inference from that grep, unrun.
- **A warm `examples/sqlite` build** (`link "sqlite3"`), base and P1 alternated, seven pairs, the
  first discarded, `uptime` reading 10.97 before and 10.25 after (other sessions' work: the
  machine was not still, so this is an indication and no duration to quote): `user+sys` per run,
  base 0.43, 0.39, 0.31, 0.30, 0.31, 0.34, P1 0.51, 0.37, 0.42, 0.38, 0.45, 0.50; best 0.30
  against 0.37; P1 the slower in 5 of the 6 pairs. What P1 adds there is one text dump of the
  list holding `sqlite3.h` and one `clang -###` of the link line, on every build. **Owed and
  unbuilt**: both answers kept by the keys the build already holds (the probe's digests for the
  dump, the line and the environment for the `-###`), as `cli/supplied_kept.hero` keeps the
  probes', which would take a warm build's cost back toward the base's. The comparison the rule
  admits (`latency.hero compare`) times the self-build, which P1 does not touch; a clock for a
  library-naming build is not among its three.
- **A warm build is judged again.** In one folder (`build/runs/p1/warm/`), `q3/rl` built plain:
  exit 0, `p` links `/usr/lib/libedit.3.dylib`; then with `LIBRARY_PATH=$R/lib`, nothing else
  changed: exit 1 `ffi_unpaired_release`, and `p` still links libedit (the refused build
  published nothing over it); then plain again: exit 0. The real link runs on each build and the
  judging reads its answer, so a change in the environment alone is seen.

## 10. The seat's verdict on P1, now that it is built

- `verdict`: **approve** P1 with P4's note, as built here, under the conditions below. No veto:
  it adds no core construct and breaches no ceiling.
- `section`: design.md §1.12 (a heap overflow at exit 0, § 4); §4.17 (a note names a remedy that
  builds: three first remedies were cut for failing that, § 3); §4.19 at :3125-3133 (the machine's
  `CPATH`, `LIBRARY_PATH`, and Windows); Part 8 at :4336 (*a diagnostic is exit 1 or nothing*);
  §1.7 (link-stage logic, no core construct).
- `implementation_cost`: +428 lines in `code_lines`' unit, 328 without comments: two new modules,
  `selfhost/cli/supplied_paired.hero` (276) and `selfhost/cli/supplied_paired_words.hero` (125),
  and +22, +3, +2 in `cli/supplied.hero` (274), `cli/supplied_words.hero` (291),
  `cli/assemble.hero` (295); P4 is ±1 in `selfhost/head_names.hero` and three `check` goldens.
  No lexer, checker, IR, lowering or emitter line. `layout` passes every touched file.
- `needed_for_self_hosting`: no.
- `argument`: Built, P1 refuses every measured mismatch: readline's heap overflow under
  `--library` and `LIBRARY_PATH`, lzma's `50080042` header over the SDK's `50040032` library, the stray
  `libcurl` in `.`, and the eleven rows exactly as the hand column said. It refuses no tracked
  program: 42 `run` cases, 20 `unsupported` and 8 examples unchanged. It reads two answers the
  build already asks, the dump and the link's own map. Its weak points: it judges folders, not
  releases, so two named halves of two releases build (measured); judged per bound name it would
  miss 2448's own fault, which lives in a door's callees; and three of its first remedies were
  false until run.
- `prediction`: at the gate that lands P1 (M-core-packages, after the sitting's synthesis), the
  full net's `run`, `corpus`, `compat`, `warnings` and `unsupported` forms move by zero cases and
  print `ffi_unpaired_release` nowhere, and `check` moves by exactly `fixedbugs-216`, `-225` and
  `-250`, whose `.expected` files take P4's words.
- `condition`: I move to **object** if a Linux leg refuses a correct program because clang's own
  `-L` folder is also one the line names (§ 8, item 3), until the tie is built; or if a program
  whose header and library come from one release, both named or both the platform's, is refused
  anywhere; or if the warm cost, measured on a still machine, stands above the latency rule's
  0.5 % floor of a build that names a library and the keeping of § 9 does not take it back.

## 11. The diff

`git diff 7fb6868b2` in `.claude/worktrees/245-compiler-engineer`, saved as
`.claude/worktrees/p245-scratch/compiler-engineer/build/route.diff`. The two new files are marked
intent-to-add (`git add -N`) in that worktree's own index so the diff carries them; nothing is
staged with content and nothing is committed.

- `wc -l route.diff`: 767 lines.
- `git diff 7fb6868b2 --numstat`, added and removed per file:

| file | + | − |
|---|---|---|
| `selfhost/cli/supplied_paired.hero` (new) | 373 | 0 |
| `selfhost/cli/supplied_paired_words.hero` (new) | 186 | 0 |
| `selfhost/cli/supplied.hero` | 31 | 6 |
| `selfhost/cli/assemble.hero` | 5 | 3 |
| `selfhost/cli/supplied_words.hero` | 5 | 1 |
| `selfhost/head_names.hero` | 1 | 1 |
| **total** | **601** | **11** |

The `code_lines` counts are § 7's: +428 in the instrument's unit, 328 without comments.

## Linux arm64, the tie, measured

Asked by the coordinator at 02:17 (§ 8, item 3 was an inference). Every row below is from a
command run in this section; the machine was not still (other seats, a lane's Docker leg), and no
figure here is a duration anyone should quote.

**A first run, 02:18 to 02:26, is void.** The Mac's data volume filled at about 02:25; that run's
folder held a table in the same shape as run 1's below, but it was written inside the window the
coordinator named and Docker reported `read-only file system` at 02:26:44, so I deleted the whole
folder (`linux/`, 76 MB) at 02:37 and started again from an empty one. Nothing below reads it.

### How it was run

- Image `heroes-linux-arm64:latest` (`docker images`: `338a1044ac90`), Docker 29.9.2, `--rm`.
- The tree: `git -C .claude/worktrees/245-compiler-engineer ls-files` (16,406 paths, the two new
  modules among them through `git add -N`), sent as `COPYFILE_DISABLE=1 tar --no-mac-metadata
  --no-xattrs -cf - -T files.txt | docker run -i --rm -v <scratch>/linux:/out
  heroes-linux-arm64:latest bash /out/inner.sh ...`. Inside: `mktemp -d`, the tar extracted
  (`find . -name '._*'` counts 0), stage 1 `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes`, stage 2 of each compiler from its tree. Only text is kept under `/out`: the log, the
  table and each build's `b.txt`; no compiler and no binary leaves the container.
- The base's tree is the same tar with the route's changed files taken back from `git show
  7fb6868b2:<path>` and its two new files removed. The image: `uname -m` `aarch64`, `Debian clang
  version 22.1.8 (1~deb13u4)`, `LIBRARY_PATH` unset, `/lib -> /usr/lib` (`readlink -f /lib`),
  `zlib1g-dev 1:1.3.dfsg+really1.3.1-1+b1`, `/usr/include/zlib.h`, `/usr/lib/aarch64-linux-gnu/
  libz.so -> libz.so.1.3.1`, `libz.a`.
- The program, `linux/zprog/main.hero`:

```
extern "zlib.h" link "z"
    constant ZLIB_VERNUM: i64
    function zlibVersion() -> cstr

function main()
    print("header ", ZLIB_VERNUM)
    print("library ", validated(c: zlibVersion()).default("?"))
```

  A paired build prints `header 4880` (`0x1310`, zlib 1.3.1) and `library 1.3.1`. Each build is
  `heroes-<c> build main.hero <words> -o p` in a fresh `mktemp -d`; where it builds, `timeout 20
  ./p 2>&1 | head -c 200000 > r.txt` and `ldd p` (run 1) or `readelf -d p` (run 2).
- The ways: the coordinator's three, `plain`, `library` (`--library /usr/lib/aarch64-linux-gnu`),
  `lp` (`LIBRARY_PATH=/usr/lib/aarch64-linux-gnu`); and the shapes beside them: `library-lib` and
  `lp-lib`, the same through `/lib/aarch64-linux-gnu`, the merged-`/usr` spelling clang also adds;
  `stray`, `--library` of a `mktemp -d` holding a copy of the system's `libz.so`, a folder clang
  does not add: the control the tie must not reach. Its library is the system's bytes, so its
  refusal is P1's own price (a header of the platform's beside a library of a named folder is
  judged by where each was found, never by its bytes), the price § 4's `curl-lib` already shows,
  and no case of § 10's condition (its two halves are neither both named nor both the platform's).

### Run 1, 02:38:20 to 02:43:01: the base and P1 (`linux/table-r1.txt`, `linux/leg-r1.log`)

`stage1 exit 0`; `stage2 base exit 0`, `sources 0fe313ba58ba197c` (the key every seat's base
printed); `stage2 p1 exit 0`, `sources 125475c31f2da42a` (this Mac's `build/heroes-p1` prints the
same key).

| way | base | P1 |
|---|---|---|
| plain | 0, run 0, `header 4880 library 1.3.1`, `ldd`: `/lib/aarch64-linux-gnu/libz.so.1` | 0, the same |
| `--library /usr/lib/aarch64-linux-gnu` | 0, the same | **1** `ffi_unpaired_release` |
| `LIBRARY_PATH=/usr/lib/aarch64-linux-gnu` | 0, the same | 0, the same |
| `--library /lib/aarch64-linux-gnu` | 0, the same | **1** `ffi_unpaired_release` |
| `LIBRARY_PATH=/lib/aarch64-linux-gnu` | 0, the same | **1** `ffi_unpaired_release` |
| stray (`--library` of a copy) | 0, the same | **1** `ffi_unpaired_release` |

P1's refusal of the coordinator's second way, whole (`linux/runs/r1-p1-library.txt`):

```
error[ffi_unpaired_release]: `zlibVersion`, bound here, is declared by a header no folder this build names holds, and defined by a library of `/usr/lib/aarch64-linux-gnu`, a folder this build names: they may be two releases of library `z`, and nothing holds one to the other
  at main.hero:5:5
    |
  5 |     function zlibVersion() -> cstr
    |     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  note: the declaration clang read is in `/usr/include/zlib.h`
  note: the definition the link took is in `/usr/lib/aarch64-linux-gnu/libz.so`
  note: a library's header and its definitions come both from folders the build names, or neither: take `/usr/lib/aarch64-linux-gnu` out of `--library` or a package's answer for this build, so the platform's library is linked with the platform's header, or name the library with `pkgconfig` and let `pkg-config` answer both
```

- **§ 8's item 3 was right, and it is worse than it said**: P1 refuses a correct program three
  ways of the five, a `blocking` class (a correct program refused). The header is zlib 1.3.1's,
  the library zlib 1.3.1's, and the base prints both.
- **The `LIBRARY_PATH=/usr/lib/<triple>` row passes by an accident of Debian's merged `/usr`, not
  by the rule**: clang's `-###` for that way (below) puts its own `-L/lib/aarch64-linux-gnu` first,
  so GNU ld took `/lib/aarch64-linux-gnu/libz.so`, a folder nothing named, and P1 counted it the
  platform's. Named by that spelling (`lp-lib`), the same variable is refused.
- **What clang's link job carries**, `clang -### /tmp/t.o <words> -lz`, its `-L` words in order:

| way | the job's `-L` |
|---|---|
| plain | `/usr/lib/gcc/aarch64-linux-gnu/14` `/lib/aarch64-linux-gnu` `/usr/lib/aarch64-linux-gnu` `/usr/lib/llvm-22/bin/../lib` `/lib` `/usr/lib` |
| `-L /usr/lib/aarch64-linux-gnu` | **`/usr/lib/aarch64-linux-gnu`**, then the six above |
| `LIBRARY_PATH=/usr/lib/aarch64-linux-gnu` | the six above, then **`/usr/lib/aarch64-linux-gnu`** |
| `-L /lib/aarch64-linux-gnu` | **`/lib/aarch64-linux-gnu`**, then the six |
| `LIBRARY_PATH=/lib/aarch64-linux-gnu` | the six, then **`/lib/aarch64-linux-gnu`** |
| stray | **the copy's folder**, then the six |

  Debian clang 22.1.8 merges no `-L`: a folder named and also its own is written twice, and a
  `LIBRARY_PATH` element goes after its own folders. Apple clang 21 on this Mac does the same with
  its one folder, `/usr/local/lib` (in `build/lp/`: `env -u LIBRARY_PATH clang -### t.o
  -L/usr/local/lib -lz` prints `"-L/usr/local/lib" "-L/usr/local/lib"`; `LIBRARY_PATH=/usr/local/lib`
  the same), except that its `LIBRARY_PATH` elements go before it (§ 1). **And an empty value is not
  an unset one**: `LIBRARY_PATH=` adds `-L.` under Apple clang (`"-L." "-L/usr/local/lib"`), so a
  second `-###` with the variable given empty cannot ask clang which folders are its own; the
  count of the job's words can.

### The tie rule, built (v1)

The coordinator's wording: a folder clang adds on its own counts as the system's whichever way it is
also named, as `stand_in.finder` gives a header's tie to the platform.

- `selfhost/cli/supplied_names.hero`, `clang_own(job, named, elements)`: each folder the job holds
  more often than the line's `-L` words and `LIBRARY_PATH`'s elements name it, all read lexically.
  No second process: it reads the `-###` P1 already asks. Were clang ever to merge two `-L`, the
  folder reads as named and is judged: the loud direction, P1's behaviour.
- `selfhost/cli/clang_added.hero`, `elements_of(name)`: the variable's elements, read lexically, an
  empty one `.`; `element_names` now reads through it.
- `selfhost/cli/supplied_paired.hero`, `searched`: the job's `-L` words gathered first, clang's own
  (ties among them) put in `own`, whatever is left of the job and not the line's in `named` and
  `environment`; `judged`'s test `named && !own` was already there and now sees the tie.
- A test of `clang_own` over the measured jobs above (Debian's plain, `-L`, `LIBRARY_PATH` and
  stray; Apple's `/a::/b` and its doubled `/usr/local/lib`).
- This Mac: `heroes check selfhost/main.hero` exit 0; stage 2 `heroes-tie`, `sources
  79fb6cbc7f68d713`; `heroes-tie test selfhost/main.hero --only "panel 245"`: 4 passed, 0 failed;
  `--only "a variable's elements"`: 1 passed.

### Run 2, 02:50:12 to 02:58:01: base, P1 and the tie (`linux/table-r2.txt`)

The same tar with the tie, three compilers: `base` (0fe313ba58ba197c), `p1` (P1's own
`supplied_paired.hero` recovered from `build/route.diff`, `clang_added` and `supplied_names` the
base's: `sources 125475c31f2da42a`, run 1's key, so the same P1) and `tie` (79fb6cbc7f68d713,
this Mac's). Two shapes more: `both`, `--include /usr/include --library /usr/lib/aarch64-linux-gnu`;
and `planted`, after every other row: a `libz.so` built from one line, `zlibVersion` returning
`9.9.9-planted`, soname `libzplanted.so`, put in `/usr/lib/llvm-22/lib` (which held no `libz`
before: `ls` said so), a folder clang adds on its own, and `--library /usr/lib/llvm-22/lib`.
It stands in for a second release in a toolchain folder of clang's: this image holds one,
`libc++.so`, a file of its own in `/usr/lib/aarch64-linux-gnu` and a link to llvm-19's
`libc++.so.1.0` in `/usr/lib/llvm-22/lib` (`linux/reorder-probe.txt`), which no C header binds.

| way | base | P1 | tie |
|---|---|---|---|
| plain | 0, `4880`, `1.3.1` | 0, the same | 0, the same |
| `--library /usr/lib/<triple>` | 0, the same | **1** | **0**, the same |
| `LIBRARY_PATH=/usr/lib/<triple>` | 0, the same | 0, the same | 0, the same |
| `--library /lib/<triple>` | 0, the same | **1** | **0**, the same |
| `LIBRARY_PATH=/lib/<triple>` | 0, the same | **1** | **0**, the same |
| stray | 0, the same | **1** | **1**, the same refusal |
| both | 0, the same | **1** | **0**, the same |
| planted | 0, `NEEDED libzplanted.so` | **1** | **0**, `NEEDED libzplanted.so` |

(`readelf -d p`'s `NEEDED` is `libz.so.1` in every row that built but `planted`.)

- **The tie answers the coordinator's question**: every paired build passes, and the stray is
  still refused.
- **And it opens a hole P1 did not have, measured**: `planted` built at exit 0 under the tie with
  the link taking the second release (`NEEDED libzplanted.so`) beside the platform's 1.3.1 header,
  where P1 refused it. Naming a folder clang also adds moves it before clang's other folders, and
  those folders are not one release. (That row's run died at 127, *libzplanted.so: cannot open
  shared object file*: I named the planted file `libz.so` and no file `libzplanted.so`; run 3
  adds it.)

### The second-release guard, built (v2)

- `selfhost/cli/supplied_names.hero`, `tied(own, dir, file)`: a folder the build names is clang's
  own for the file the link took only where it is one of `clang_own`'s and **no other of them holds
  a file of that name with other bytes** (`file_reading.key_of`, as `shown` already compares). The
  same bytes, as `/lib/<triple>` holds through the merged `/usr`, are one release. It asks the
  files, never the order of clang's folders, which differs by platform (§ 1, above).
- `selfhost/cli/supplied_paired.hero`, `judged`: `library_named = named && !tied(...)`, one line.
- A test making three folders under `build/`: the same bytes in two of clang's folders is a tie;
  a folder clang does not add is none; a second release in a third of clang's folders takes the
  tie away.
- This Mac: `heroes check selfhost/main.hero` exit 0; stage 2 `heroes-tie2`, `sources
  3511671d755a6689`; `--only "panel 245"`: 5 passed, 0 failed.
- **What it does not see, said**: a second release under another name, a `libz.a` in an earlier
  folder of clang's beside the `libz.so` the named folder gave, which GNU ld would take first from
  that earlier folder unnamed. Seeing it needs the linker's own rule of which file a `-l` reads in
  a folder, a premise about the world this build does not write; unbuilt.

### Run 3, 03:01:17 to 03:05:23: the tie (v1) and the guard (v2) (`linux/table-r3.txt`)

`tie` from v1's two files kept aside (`linux/tiev1/`): `sources 79fb6cbc7f68d713`, run 2's and
this Mac's key, so the same v1. `tie2`, the tree as sent: `sources 3511671d755a6689`, this Mac's.
Two planted shapes after the seven ways, each in `/usr/lib/llvm-22/lib` (no `libz` there before):
`planted-same`, a copy of the system's `libz.so` (`cmp` said the same bytes); `planted`, the
one-line second release, now with a file `libzplanted.so` beside it so the program can run under
`LD_LIBRARY_PATH=/usr/lib/llvm-22/lib`.

| way | tie (v1) | tie2 (v2) |
|---|---|---|
| plain | 0, `4880`, `1.3.1` | 0, the same |
| `--library /usr/lib/<triple>` | 0, the same | 0, the same |
| `LIBRARY_PATH=/usr/lib/<triple>` | 0, the same | 0, the same |
| `--library /lib/<triple>` | 0, the same | 0, the same |
| `LIBRARY_PATH=/lib/<triple>` | 0, the same | 0, the same |
| stray | **1** `ffi_unpaired_release` | **1**, the same |
| both | 0, the same | 0, the same |
| planted-same | 0, the same, `NEEDED libz.so.1` | 0, the same |
| planted | see run 4 | see run 4 |

**My script's own fault, said**: in run 3 the `planted` arm read `build ...; ldp=$P`, so `be=$?` was
the assignment's exit, 0, under both compilers. The guard's build printed its refusal whole
(`linux/runs/r3-tie2-planted.txt`, naming `/usr/lib/llvm-22/lib` and the note to take it out of
`--library`), and `readelf` found no `p` (`docker3.out`: *readelf: Error: 'p': No such file*); its
exit was unread, so run 4 read it.

### Run 4, 03:05:52 to 03:09:47: the `planted` row's exits (`linux/table-r4.txt`)

The same two compilers (`79fb6cbc7f68d713`, `3511671d755a6689`), the second release planted as in
run 3, the build's exit read on the next line:

| compiler | build | run, under `LD_LIBRARY_PATH=/usr/lib/llvm-22/lib` |
|---|---|---|
| tie (v1) | **0**, `NEEDED libzplanted.so` | **0**, `header 4880 library 9.9.9-planted` |
| tie2 (v2) | **1** `ffi_unpaired_release`, no `p` (`ls`: *No such file*) | none |

**The tie as worded builds a mismatched pair at exit 0 and runs it; the guard refuses it, and
builds every paired row of the eight.**

### This Mac, the guard beside P1 (`build/macrows-p1b.txt`, `build/macrows-tie2.txt`)

`build/macrows.sh <compiler>` runs § 4's 28 rows through `build/case.sh`, its words written from
§ 4's descriptions (`R`, `H`, `K` as there, the profile's `LIBRARY_PATH=:/opt/homebrew/lib`, `-VAR`
unsetting one). `p1b` is `build/heroes-p1` (`sources 125475c31f2da42a`) under another name so its
folders do not overwrite § 4's; `tie2` is the guarded compiler.

- **28 of 28 rows give the same verdict, 27 the same line to the character.** The 28th is
  `dot-inc`, P1's measured residue: build 0, run 134, *Library not loaded:
  @rpath/libcurl.dylib*, under both, the binary's UUID in dyld's words the one difference.
- Every refusal of § 4 stands under the guard: `rl-lib`, `rl-lp`, `rl-lib-san`, `lzma-inc-nolp`,
  `lzma-cpath-nolp`, `dot`, `dot-pc`, `curl-cpath`, `curl-inc`, `curl-lib`, `curl-lp`, `rlp-lib`,
  `rlp-lp`; and every pass.
- That this Mac cannot move is measured, not argued: Apple clang's link job adds one folder of its
  own, `/usr/local/lib` (§ 1), and `ls /usr/local/lib` says *No such file or directory* here, so no
  library a build takes can come from a folder clang adds itself. On an Intel Mac, where Homebrew's
  prefix is `/usr/local`, the tie would reach every Homebrew library named through
  `/usr/local/lib`, and the guard would give it to the platform only where no other folder of
  clang's holds a second file of that name; unrun, no such machine here.
- `layout`, narrowed by the harness's third word, judged by the guarded compiler:
  `supplied_names`, `supplied_paired`, `clang_added`: 4 passed, 0 failed each
  (`build/h-layout-tie2-*.txt`).

### What it costs, and where it lands

`code_lines`' unit, base (7fb6868b2) against the guarded tie, and without comments or tests
(`awk '/^## Tests/{exit}' | grep -cvE '^\s*(#|$)'`):

| file | base | now | Δ | Δ without comments |
|---|---|---|---|---|
| `selfhost/cli/supplied_paired.hero` (new) | 0 | 277 | +277 | +208 |
| `selfhost/cli/supplied_paired_words.hero` (new) | 0 | 125 | +125 | +96 |
| `selfhost/cli/supplied.hero` | 252 | 274 | +22 | +18 |
| `selfhost/cli/supplied_words.hero` | 288 | 291 | +3 | +3 |
| `selfhost/cli/assemble.hero` | 293 | 295 | +2 | +2 |
| `selfhost/head_names.hero` | 292 | 292 | 0 | 0 |
| `selfhost/cli/clang_added.hero` | 280 | 287 | **+7** | +5 |
| `selfhost/cli/supplied_names.hero` | 216 | 256 | **+40** | +23 |
| **total** | | | **+476** | **+355** |

The tie and the guard are +48 of it (+27 without comments), in the link stage's `cli/` alone: no
lexer, checker, IR, lowering or emitter line, no new diagnostic class (the refusal stays
`ffi_unpaired_release`), and **no new process**: the tie reads the `-###` P1 already asks, and the
guard opens the library files of clang's own folders by name only where a named folder supplied
a judged name and is one of them.

`build/route.diff` is regenerated, `git diff 7fb6868b2` in the worktree: **915 lines** (§ 11's 767
are P1's alone, kept as `build/route-p1.diff`); `--numstat` +706 and −19 over eight files:
`supplied_paired.hero` 375/0, `supplied_paired_words.hero` 186/0, `supplied_names.hero` 84/0,
`supplied.hero` 31/6, `clang_added.hero` 19/8, `assemble.hero` 5/3, `supplied_words.hero` 5/1,
`head_names.hero` 1/1. Nothing is staged with content and nothing is committed.

### What it does to § 10

- **§ 10's first condition fired**: a Linux leg refused a correct program because a folder clang
  adds on its own was also named (run 1, three ways of five; run 2's `both` a fourth), so P1 as
  § 7 built it is
  **objected to** by its own seat until the tie is in it.
- **The tie the coordinator worded is not enough, measured**: it builds and runs a second release
  at exit 0 (run 4), a pair P1 refused. **What I put forward is P1 with the tie and the guard**,
  measured: every paired row on Linux arm64 builds (7 of 7: plain, both spellings by `--library`
  and by `LIBRARY_PATH`, `both`, `planted-same`), the stray and the second release are refused,
  and this Mac's 28 rows keep their verdicts.
- What stays unrun: the tie and the guard on Linux x86-64 and on Windows (read from `searched`'s
  code, not run: an MSVC target returns before any `-###`, so no folder is clang's own there and
  the tie cannot move a verdict); a second release under another name (the `.a` beside a `.so`);
  and a `-Wl,-L<dir>` word on the line, which `dirs_of` does not read as a named folder, so its
  folder counts as clang's own, as it did under P1 (read from the code, not run).
