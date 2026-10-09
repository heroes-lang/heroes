# Panel 198, the shared brief: which words a package's answer may carry

Rewritten on 2026-10-08 from 08:02 to 08:10 by the clock, by the coordinator's hand, on
the tree frozen at `56def9b4` (worktree `lane-panel-198`: batch 14 as pushed,
`ddeed6d0`, plus defects 488 to 502 filed). The first version (2026-10-07,
from 23:42) was read by the completeness critic before any seat started
(`docs/panel/198-reports/completeness-critic-pass1.md`, its clock 01:44:36 to
01:57:11) and **all fourteen repairs of its § 6 are applied here** and in the
seats' briefs. Every fact below names the command or file it comes from:
those marked *(critic)* are the critic's, with its probe file under
`<scratchpad>/198-critic/`; the rest were run between 07:47:06 and 08:01:23
by the clock, read-only on every tree, their outputs in
`<scratchpad>/198-briefs-work/` (a copy of the frozen tree with its own
compiler built from the seed, and probes beside it). The trunk stood at
`ee95a6f0` at 07:55, and `git diff --stat 56def9b4 HEAD -- selfhost/cli spec`
printed nothing.

**Lane: full.** The author answered on 2026-10-08, between 01:57 and 07:37 by
the clocks read before and after, to *convene it full, the blind seat
included, 6 USD in all*. The seats: compiler-engineer, ffi-pragmatist,
spec-warden, historian, and the llm-ergonomist run by the coordinator as fresh
sessions (its brief), the completeness critic's second pass over the reports.
Why the soundness lane did not hold, the critic's § 1: every route changes the
`ffi_package` note's text (`selfhost/cli/libraries.hero:199-206`, *"The list
in it is the function above, word for word"*); that note is now the rule's
only statement a reader sees (the spec's sentence was removed, below); a word
the list admits changes which programs `build` accepts, semantics under
CLAUDE.md § 4; `SKILL.md:18-23`, `:29` and `:33`. Convened for defect 444
(`systemic`, *a ruling no rule reaches*), which blocks the milestone's tag:
`issues/2026-10/07/2026-10-07-1644-defect-444-sdl3-s-own-sdl3-pc-from-a-source-build-with-default-options-is-refused.md`.

## The question

**Which words may a package's `pkg-config` answer carry, and how is one
admitted?** The whole class of refused words, not only `-Wl,` ones (Q1):

- defect 444's: SDL 3.2.10 built from its source with `SDL_RPATH` at its
  default (ON) and seven other options set answers `-Wl,--enable-new-dtags`;
- the census (M4): `-pthread` and `-isystem <dir>` refuse correct programs on
  Debian 13 (`package "glib-2.0"`, `package "libcurl"`), `-Wno-*` refuses 219
  `absl_*` packages on this Mac, and libpsx's and gmodule's answers carry
  `-Wl,` words the list refuses.

Is each admitted, refused, or dropped, by what test, and what does the
refusal's note tell the person whose correct program it stops?

## What the list is, read in the tree

- `selfhost/cli/libraries.hero:5-15`: *THE ALLOW-LIST IS THE WHOLE SECURITY OF
  package* (Go's CVE-2018-6574, `-fplugin=attack.so` through `#cgo`); *A .pc
  file is INPUT THIS PROGRAM DID NOT WRITE*; *The property the list asks of a
  word: does it name a file, load anything, or write anything at build time?*;
  and `:13-15`, *What a library does AT RUN TIME the list cannot see
  (-lSDL2main replaces main and opens a window; the corpus timeout is the
  instrument for that class ...)*. The rulings behind it are panels 048, 049, 050
  and 055 (`:2`).
- **Where a word is judged**: `filter_words` (`:84-133`) takes the valued
  flags `-D -U -I -L -l -F` and `-framework`, joined or as the next word
  (`valued`, `:209-210`; `allowed_prefixed`, `:212-221`), then two whole
  `-Wl,` shapes through `linker_value` (`:229-241`): `-Wl,-rpath,<dir>` and
  `-Wl,-framework,<name>`, one word or split; `clean_tail` (`:260-268`)
  refuses a tail that is empty, holds a comma, or begins with `-`. Anything
  else is `refused` (`:199-206`). The tests assert `-isystem /sys/a` and
  `-Xlinker -rpath -Xlinker /rp/c` refused (`:399`, `:400`) and
  `-Wl,-e,evil_entry` refused (`:358`).
- **Where a word goes**: `link_words` (`:174-197`) passes every `-L`, `-l` and
  `-Wl,` word of the filtered answer, in the package's order, on every
  platform; `link_line` (`selfhost/cli/link.hero:133-151`) puts them after the
  objects and before the `link` groups' `-l` names (`:145-149`).
- **The note**, both lines, as the frozen compiler prints them (below):
  *only `-D`, `-U`, `-I`, `-L`, `-l` and `-F`, each with its value joined or
  as the next word, `-framework <name>`, and `-Wl,-framework,<name>` and
  `-Wl,-rpath,<dir>`, in one word or split as `-Wl,-rpath -Wl,<dir>`, are
  accepted: everything else is a flag a package file could use to run code
  during the build (Go's CVE-2018-6574)* and *name the library directly with
  `link` if you need it*. Its text is pinned by the compiler's own tests at
  `libraries.hero:355`, `:390`, `:475-477`, `:496`, and its headline's shape
  by `selfhost/emit/ffi_build.hero:399-402` (`grep -n 'needle: "'` and
  `grep -rn 'does not pass on'`).
- **The spec says nothing of the list**: `grep -n 'package'
  spec/heroes-spec.md` returns `:445` (*A group may name a **package**
  instead of a library: ... asks the system where its headers and libraries
  are and what else it needs.*) and `:448` (the `Extern` production). The
  sentence *A package answering with anything this compiler does not pass on
  is refused, naming what it said.* was removed on 2026-10-05 by panel 192 R8
  item r1 (`git log -S'does not pass on' -- spec/heroes-spec.md` gives
  `d0409cc5`, its body *r1, removed: § 13's sentence on a package's answer.
  -22*), on the ground that the message states the rule
  (`docs/panel/192-reports/spec-warden.md:527-538`). Panel 050 had said *The
  list is named in the spec, because the list is the safety*
  (`docs/panel/050-the-package-clause.md:65-66`), and its prediction 3
  (`:128`): *No `.pc` on any of the three runners answers with a flag outside
  the allow-list*. The spec names no `CPATH`, `LIBRARY_PATH` or `pkg-config`
  (`grep -n -E 'CPATH|LIBRARY_PATH|PKG_CONFIG|pkg-config'`, no line).
- `docs/design.md:2636-2642` (§4.19): a `-` check cannot see clang's `@file`;
  *Panel 050's **allow-list** holds against it ... because it names what is
  permitted rather than what is forbidden; a rule phrased as "not beginning
  with `-`" does not.*
- **One platform question**: the compiler asks the machine one thing,
  `process.exe_suffix()` (`libraries.hero:56-63`, `selfhost/cli/flags.hero:162-167`:
  *`selfhost/` gains no platform axis*); Darwin is told apart only by
  `uname -s` in `selfhost/cli/doctor.hero:150-152`. It passes no `-fuse-ld`
  (`grep -rn 'fuse-ld' selfhost runtime`: 0), so ELF lld is driven only where
  clang's default linker is lld.
- **Every POSIX link already carries `-rdynamic`** (`flags.hero:169-179`,
  panel 104), and Debian 13's clang 22.1.8 hands the linker `-export-dynamic`
  for it (`clang -###` in `heroes-linux-arm64:sdl3-b14`, between 07:54:49
  and 07:55:42 by the clocks read around it, `b14-rdynamic.txt`), so
  gmodule's `-Wl,--export-dynamic` would restate it there: an inference from
  the driver's line, not run as a link with both.
- **The rpath's admission is not beside the code any more**: `-Wl,-rpath,<dir>`
  was admitted on 2026-08-14 in `eac3e5b5`, with no sitting *(critic)*, its escalation
  written as a comment in `crates/heroes-cli/src/commands/libraries.rs` (*a
  hostile `.pc` saying `-L/tmp/x -lfoo` already chooses the library ... a
  library can be **swapped after the build***, `git show eac3e5b5`); the
  port carries none of it (`grep -rn -i hostile selfhost`: one hit, in
  `selfhost/mutate/json_read.hero`).
- `CGO_LDFLAGS_ALLOW` appears in no file of `docs/`, `issues/`, `selfhost/` or
  `.claude/` but the critic's report (`grep -rln 'CGO_LDFLAGS_ALLOW\|CGO_CFLAGS_ALLOW'`):
  whether an earlier sitting weighed a person-building allowance under other
  words is a question, not searched.

## The fault, reproduced on the frozen compiler

- **Lane b14-box's run** (`<scratchpad>/batch14/box/ci/`): `ci-step2.sh:16-17`
  configures SDL 3.2.10 with `-DCMAKE_BUILD_TYPE=Release -DSDL_TESTS=OFF
  -DSDL_EXAMPLES=OFF -DSDL_STATIC=OFF -DSDL_UNIX_CONSOLE_BUILD=ON
  -DSDL_X11=OFF -DSDL_WAYLAND=OFF` and leaves `SDL_RPATH` at its default,
  `ci-step2-out.txt:505` *SDL_RPATH (Wanted: ON): ON*, on *Ubuntu 24.04.5
  LTS, ... Ubuntu clang version 18.1.3* (`:2`). The answer, `:891`:
  `-I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib
  -Wl,--enable-new-dtags -lSDL3`; the refusal `:895` and `:920`, one per
  program, the note `:900`.
- **The same text from the frozen compiler** (07:52 to 08:01): built from the
  seed in the copy, with a stand-in `pkg-config` on `PATH` answering that
  line, `heroes build` exits 1 with the headline *the package `sdl3`
  answered with `-Wl,--enable-new-dtags`, which this compiler does not pass
  on* and the two notes above (`probe/variant-A-t1.txt`). In Docker
  `heroes-linux-arm64:sdl3-b14` with a stand-in `sdl3.pc` (its text is in
  the ffi-pragmatist's brief), real pkgconf answers
  `-Wl,-rpath,/usr/lib/aarch64-linux-gnu -Wl,--enable-new-dtags -lSDL3` and
  the build exits 1 the same way; the same program with `link "SDL3"` in
  place of `package "sdl3"` builds and prints its `.expected` (`true true
  true 42 true`) (`b14-standin.txt`, 07:56 to 07:57).
- **Debian's own `sdl3.pc` answers no `-Wl,` word** (`pkg-config --cflags
  --libs -- sdl3` in `sdl3-b14`, 07:53, `docker-b14-answers.txt`; *(critic)*
  `docker-b14.txt`), so that image is not a reproducer alone.
- **The two refused programs**: `examples/sdl/main.hero` and
  `tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero`. 26 case or
  example files hold `package "` (`git ls-files '*.hero' | xargs grep -l
  'package "'` under `tests/` and `examples/`, `tests/harness/absence.hero`
  aside).
- **The CI**: `.github/workflows/ci.yml:316` installs `clang lldb pkg-config
  libsqlite3-dev libcurl4-openssl-dev`; `:331-338` say why
  `-DSDL_RPATH=OFF`; `:347` sets it; `:338` *The x86-64 leg is unrun with
  it.*

## Measurements handed to every seat

- **M1, ld64** (`ld-27037.1`, `ld -v`): `clang m.c -Wl,--enable-new-dtags`
  stops *ld: unknown options: --enable-new-dtags*, exit 1, and
  `--disable-new-dtags` the same (*(critic)* `mac-dtags.txt`,
  `mac-dtags2.txt`; re-run between 07:54:02 and 07:54:21,
  `probe/mac---enable-new-dtags.txt` and `probe/mac---disable-new-dtags.txt`). A
  route admitting either word everywhere turns a Heroes diagnostic into a raw
  linker failure on macOS.
- **M2, lld-link**: *lld-link: warning: ignoring unknown argument
  '--enable-new-dtags'* *(critic, Docker lld-link 22, `lld-link.txt`)*; on
  the Windows box at 07:54, LLD 23.1.1, the same warning for both words
  (lld-link run alone with the word, no input; not through clang). A warning
  on a correct program is `blocking` (the critic's reading of
  `.claude/rules/verification.md` § Bounded discovery, *a clang warning on a
  correct program*).
- **M3, the default tag** with `-Wl,-rpath` alone: RUNPATH from Debian GNU ld
  2.44 through clang and from LLD 22.1.8, arm64 *(critic, `docker-b14.txt`)*;
  RUNPATH from Ubuntu 24.04's GNU ld 2.42, aarch64, and RPATH under
  `--disable-new-dtags` *(critic, `ubuntu2404-dtags.txt`)*; RUNPATH from
  Debian 13's GNU ld 2.44 through Debian clang 22.1.8 on x86-64 (emulated,
  image `heroes-linux:latest`, 07:54, `x86-dtags.txt`), RPATH under
  `--disable-new-dtags`, RUNPATH under `--enable-new-dtags`. **Ubuntu 24.04
  on x86-64, the CI's leg, is unrun.** So on every toolchain measured
  `--enable-new-dtags` restates the default and `--disable-new-dtags` is the
  word that moves: an RPATH is searched before `LD_LIBRARY_PATH`, a RUNPATH
  after it (*(critic)*, by a run: `dtags-order.txt`, a RUNPATH binary loads
  B's library under `LD_LIBRARY_PATH=B`, an RPATH binary keeps A's).
- **M4, the census** *(critic)*, `<scratchpad>/198-critic/mac-census.tsv`,
  `refused.tsv`, `linux-b14-census.tsv`, `linux-refused.tsv`; counted again
  between 07:47 and 07:50 with `cut -f1 | sort -u`:
  - this Mac, pkg-config 3.0.7, `--list-all` 505 lines (between 07:54:02
    and 07:54:21), 499 packages in the census: 221 refused, none for a
    `-Wl,` word: 219 `absl_*` for
    `-Wno-*` words, `libmpdec++` for `-pthread`, `uvwasi` for one word
    holding `-fvisibility=hidden\;--std=gnu89\;-Wall...`;
  - `sdl3-b14`, Debian 13 (trixie) aarch64, pkgconf 1.8.1, `--list-all` 155
    lines (07:53) against 124 packages in the census (the gap is
    unexplained): 23 refused. 13 for `-pthread` (`gio-2.0`, `gio-unix-2.0`,
    `glib-2.0`, `gmodule-2.0`, `gmodule-export-2.0`, `gmodule-no-export-2.0`,
    `gobject-2.0`, `gthread-2.0`, `ibus-1.0`, `libpulse`,
    `libpulse-mainloop-glib`, `libpulse-simple`, `sysprof-capture-4`); 9 for
    `-isystem /usr/include/mit-krb5` (`gssrpc`, `kadm-client`,
    `kadm-server`, `kdb`, `krb5`, `krb5-gssapi`, `libcurl`, `mit-krb5`,
    `mit-krb5-gssapi`); `gmodule-2.0` and `gmodule-export-2.0` also for
    `-Wl,--export-dynamic`; `libpsx` for `-Wl,--no-as-needed
    -Wl,--whole-archive -lpsx -Wl,--no-whole-archive -Wl,--as-needed
    -lpthread` (its whole answer, 07:53). No defect is filed for these
    *(critic)*; `grep -rl -E '\-pthread|-isystem|mit-krb5|libpsx|export-dynamic'
    issues/` finds one file, a `task` of 2026-09-19.
  - `package "libcurl"` on Debian 13 is refused for `-isystem` by the frozen
    compiler, and the same program with `link "curl"` builds and prints
    `libcurl/8.14.1 OpenSSL/3.5.7 ...` (`b14-curl.txt`, 07:57).

## The routes, a list to be widened

None is adopted; a route nobody listed is any seat's, and the critic's second
pass names what is still missing.

- **(A)** refuse as today, the note naming the source build's way out.
- **(B)** admit `-Wl,--enable-new-dtags` and `-Wl,--disable-new-dtags` by
  name, on every platform. M1: on macOS a raw linker failure; M2: a warning
  from lld-link; M3: the first restates the default, the second moves the
  package's rpath ahead of `LD_LIBRARY_PATH`.
- **(B-ELF)** (B) on ELF only, which needs a platform question the compiler
  does not ask today (one axis, `exe_suffix()`).
- **(B-asym)** admit `--enable-new-dtags` only: it never gives a package more
  than the admitted `-Wl,-rpath`; `--disable-new-dtags` stays refused. Needs
  the x86-64 default, measured RUNPATH on Debian 13 (emulated) and unrun on
  Ubuntu 24.04.
- **(C)** admit a vetted list of words, each shown to name no file, load
  nothing and write nothing at build time on each platform's linker.
- **(D)** drop an unknown word with a message and pass the rest. libpsx's
  answer is the case: dropping `--whole-archive` from it links and then
  misbehaves *(critic, not run)*. The critic reads (D) and (E) as
  contradicting `design.md:2636-2642` (*names what is permitted rather than
  what is forbidden*).
- **(E)** admit any `-Wl,` word that names no path. The shapes it must
  survive (Q4): `-Wl,@file`, `-Xlinker`, `-Wl,-dynamic-linker`, `-Wl,-R`,
  `-Wl,--just-symbols`, `-Wl,--plugin`, `-Wl,-T`, `-Wl,--version-script`;
  and `design.md:2636-2642` again: say whether any naming test can be
  complete.
- **(F)** the compiler owns the tag: it recognises both words by name, passes
  neither, keeps the toolchain's default (RUNPATH on every Linux toolchain
  measured, M3); never reaches ld64 or lld-link.
- **(G)** the person building admits a word, by argv or environment (Go's
  `CGO_LDFLAGS_ALLOW`); a CLI surface (`.claude/rules/cli-surface.md` § There
  is no fourth slot: argv and the environment are input classes, a
  per-project file is not).
- **(H)** Go's vetted list, filtered by Heroes' own test. Go 1.27.1
  (`go version`, Homebrew,
  `/opt/homebrew/Cellar/go/1.27.1/libexec/src/cmd/go/internal/work/security.go`)
  admits both dtags words (`:208`, `:210`), `-Wl,-z,relro|now|execstack`
  (`:234`), `-Wl,--(no-)?as-needed` (`:201`), `-Wl,--(no-)?export-dynamic`
  (`:212`), `-pthread` (`:131`, `:187`), `-isystem` with its next word
  (`:150`) and `-W...` at a compile (`:52`); it also admits words Heroes'
  test refuses: `-Wl,-e,<symbol>` (`:209`, refused at `libraries.hero:358`),
  `-Wl,--just-symbols` (`:221`, names a file), `-Wl,-R` (`:220`), direct
  `.so` and `.a` inputs (`:236`). It checks `pkg-config --cflags` and
  `--libs` answers by those lists (`exec.go:1841`, `:1857`), with the
  person's `CGO_<X>_ALLOW` and `_DISALLOW` applied (`security.go:366-385`).
- **(J)** the `link` way out the note names. Measured on Debian's install
  (above): `link "SDL3"` and `link "curl"` build and run. On the source-build
  machine, SDL in `/usr/local`, it is unrun.

## The questions every seat may answer

- **Q1** the whole class or only `-Wl,`: `package "glib-2.0"` and `package
  "libcurl"` are correct programs refused on Debian 13 (M4; CL-078: the shapes
  beside a defect are where what it is becomes visible). Panel 188's
  compiler-engineer rested an argument on `-isystem` being refused
  (`docs/panel/188-reports/compiler-engineer.md:538`: *a package's `-isystem`
  is refused ..., so a header with `#endif FOO` anywhere an author can bind it
  is never in a system directory*).
- **Q2** words whose position matters: `--whole-archive` and
  `--no-whole-archive` around `-lpsx`; `--as-needed` left set at the end of
  libpsx's answer reaches every `link` name `link_line` puts after it (read
  in the code, not run). Does a route keep each word's position relative to
  its `-l`?
- **Q3** comma tunnelling: `-Wl,--enable-new-dtags,--plugin,x.so` must stay
  refused (Go's comment, `security.go:195-198`; `clean_tail`, `:260-268`).
- **Q4** the shapes for (E), above.
- **Q5** RUNPATH reaches only a binary's direct `DT_NEEDED` entries (ld.so(8),
  not run); SDL3 loads its backends with `dlopen`: does the tag change what
  SDL3 finds?
- **Q6** a veto on *moving the library choice to someone else* is a run-time
  property. The answer (c) of 2026-08-15 put run-time properties outside the
  list's test (*The allow-list gets no second half ... a guard is not widened
  to cover a class it cannot see*,
  `issues/2026-08/17/2026-08-17-0221-answered-2026-08-15-and-written-as-a-refusal-with.md`),
  and the admitted `-Wl,-rpath` fails the same clause
  (`issues/2026-08/14/2026-08-14-0023-the-corpus-gets-a-timeout-joins-the-allow-list-and.md`,
  `eac3e5b5`, no sitting): cite them, or reopen them by name.
- **Q7** Windows: defect 447 is open
  (`issues/2026-10/07/2026-10-07-1644-defect-447-the-ci-s-windows-leg-carries-no-sdl3-so-the-sdl3-event-case-runs-on.md`,
  `improvement`); `package` has never run there. The box at 07:50: clang
  23.1.1, target `x86_64-pc-windows-msvc`, LLD 23.1.1, and no `pkg-config` or
  `pkgconf` on `PATH` (`command -v`), so a `package` build there would be
  told `no_pkg_config` (`libraries.hero:312-313`, and the test's comment at
  `:536-537` naming the box; not run there).
- **Q8** the CI's `-DSDL_RPATH=OFF` (`ci.yml:347`) after the ruling: kept,
  dropped, or changed?

## Precedents to cite

`eac3e5b5` and its decision of 2026-08-14 (path in Q6); the answer (c) of
2026-08-15 (path in Q6); panel 055, which sat full
(`docs/panel/055-where-a-header-is.md:6`, *Lane: full, five judges*), admitted
`-D` and `-U` (`:173`), and whose commit `4333dab9` landed
`-Wl,-framework,<name>` *in that exact form* (its body; the sitting's file
names no `-Wl,` word, `grep -c 'Wl,'` 0); panel 192 R8 item r1 (`d0409cc5`);
`design.md:2636-2642`; panel 188's compiler-engineer at `:538`; Go 1.27.1
`security.go` `:131`, `:150`, `:195-198`, `:201`, `:208`, `:209`, `:210`,
`:212`, `:220`, `:221`, `:234`, `:236`, `:366-385`; defect 447.

## The rules every seat works under

- **Your own copy**: `cp -R
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-198
  <scratchpad>/198-<seat>/tree`, then **`rm -f <scratchpad>/198-<seat>/tree/.git`**:
  the frozen tree's `.git` is a file reading `gitdir:
  /Users/joseph/Temp/heroes/heroes-lang/.git/worktrees/lane-panel-198`, so a
  `git` command in a copy that keeps it reads and refreshes the frozen
  index. Then `rm -rf build` inside the copy only (the frozen tree has none),
  and your compiler from its seed: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`. Never build, run or write in the frozen
  tree, the trunk or any `lane-*` worktree, and never in another seat's
  directory. Panel 199 sits beside this one (`lane-panel-199`,
  `<scratchpad>/199-*`): never read or write there.
- **Counts only, never durations**: `git worktree list` printed 16 entries
  at 07:55, ten of them `lane-b15-*` and one `lane-round-b15`, and `uptime`
  read a load average of 81 at 07:50.
- **A run that may not end is bounded**
  (`.claude/rules/verification.md` § A run that may not end): build the
  program and run its binary under `timeout`, never `timeout heroes run`;
  its output to `/dev/null` or through `head -c <bytes>` into a file; after
  a timeout, `pgrep -f` for the binary and stop it.
- **Running notes**: append to `<scratchpad>/198-<seat>/report.md` as you
  go, not only at the end, so a stalled seat leaves what it had; your final
  reply carries the report whole, and the coordinator copies it into
  `docs/panel/198-reports/<seat>.md`.
- **Docker**: `heroes-linux-arm64:sdl3-b14`, `heroes-linux-arm64:latest`,
  `ubuntu:24.04` and `ubuntu:26.04` (arm64) and `heroes-linux:latest`
  (amd64, emulated) are local (`docker images`, 07:50), and no container was
  running (`docker ps`). Run with `--rm` and `--pull never`: a `--platform`
  flag made Docker try to pull `heroes-linux` at 07:54 and fail, so leave it
  out. `--network none` unless the run needs the network (`ci-step2.sh`
  clones SDL). Never retag or pull an image another lane uses.
- **The Windows box answers again**: `ssh win` connected at 07:50 and
  07:54 on 2026-10-08 (it had timed out twice at 01:52, *(critic)*). It is
  shared: one clang at a time, your own folder `/c/w/p198-<seat>-<pid>`, and
  never remove anything there.
- **No paid run.** No `claude -p` session, no API call, no `heroes measure
  --refresh`; the only paid runs of this sitting are the blind seat's
  sessions, which the coordinator runs within 6 USD. A run you find worth
  paying for goes in your report with its size, and the coordinator decides.
- **Times only from `date`.** Hold the machine awake with `caffeinate -i`
  for anything longer than a minute.

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
