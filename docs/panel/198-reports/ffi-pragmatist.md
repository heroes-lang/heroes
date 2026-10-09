# Panel 198, ffi-pragmatist

Copied by the coordinator at 09:58 on 2026-10-08 (`date`) from the seat's own file
`<scratchpad>/198-ffi-pragmatist/report.md`, unchanged below the rule: the
seat's running notes, written as it went from 08:11 and finished at 09:57, with
its verdict at the end. The seat was stopped once by the session limit at about
08:25 and resumed at 09:50 on a new account. Its scratch (the copy of the tree,
the containers' logs, the Windows box's folder names, `scripts/`) stays in
`<scratchpad>/198-ffi-pragmatist/`. The seat's reply, which the coordinator also
holds, adds nothing the file below lacks except the verdict's wording:
**object** to route (F) as the shared brief states it, and a narrower
recommendation (recognise `--enable-new-dtags` by name and drop it silently,
leave `--disable-new-dtags` refused as today).

---

# Panel 198, ffi-pragmatist, running notes

- 08:11:19 brief, shared brief and critic pass 1 read.
- 08:13:17 copy made (`cp -R`, `.git` removed, `build` removed), compiler built
  from the seed in the copy (`real 95.55 user 14.05`, load ~121: waiting).
- 08:17:27 three bounded containers started (`--rm`, `sleep 9000`):
  `p198-ffi-u2404` (ubuntu:24.04 arm64, network, SDL 3.2.10 from source with
  SDL_RPATH at its default, no ldconfig yet), `p198-ffi-b14` (sdl3-b14,
  network none), `p198-ffi-x86` (heroes-linux:latest amd64 emulated, network
  none).
- Method note: the refused shapes (`--plugin`, `-T`, `--version-script`,
  `@file`, `--just-symbols`, `-dynamic-linker`) are probed only with a path
  that does not exist, reading the linker's own error to show it names a file.
  No payload is built.
- 08:18:35 `scripts/words.sh` (one `clang m.c -L. <word> -lw -lm` link per
  word, `readelf -d`/`-l` on the result, the binary run) on this Mac:
  `mac/words.tsv`. ld-27037.1, Apple clang 21.0.0: every GNU word exits 1
  `ld: unknown options: ...`, `-pthread` at a link and at a compile exit 0,
  `-isystem` at a compile exit 0. `-Wl,@args.rsp` (the file holds `-z now`)
  stops on *unknown options: -z*: ld64 OPENED and expanded the file.
- 08:18:57 the same in `sdl3-b14`: GNU ld 2.44 (`b14/words-gnu.tsv`) and LLD
  22.1.8 by `-fuse-ld=lld` (`b14/words-lld.tsv`); clang's driver calls
  `/usr/bin/ld` (`clang -###`). Both: rpath alone RUNPATH, `--enable` RUNPATH,
  `--disable` RPATH; `-z,now` BIND_NOW; `--as-needed`, `--no-as-needed`,
  `-z,relro`, `--export-dynamic`, the whole-archive pair, `--exclude-libs,ALL`,
  `-pthread` link exit 0 and run. **`--whole-archive` left open exits 1** on
  both (multiple definitions inside the `libgcc.a` clang appends). Refused
  shapes: `--version-script`, `-T` *cannot open linker script file*; GNU
  `--plugin` *error loading plugin*, LLD ignores `--plugin` silently (exit 0);
  `@args.rsp` read (BIND_NOW from inside the file); `-dynamic-linker` writes
  PT_INTERP `/nonexistent/ld.so`, the binary then does not start;
  `-Wl,-Map,m.map` WRITES a file named by a word with no slash; GNU `-R` and
  `--just-symbols` on a missing path both became RUNPATH, LLD's
  `--just-symbols` *cannot open*.
- 08:19:47 the same in `heroes-linux:latest` (Debian 13 x86-64, emulated, GNU
  ld 2.44, Debian clang 22.1.8): `x86/words-gnu.tsv`, every row as on arm64.
- 08:18:53 (container clock 06:18:53 UTC) the source build in ubuntu:24.04
  arm64 (GNU ld 2.42, Ubuntu clang 18.1.3, Ubuntu LLD 18.1.3), SDL
  `877399b2`: pkg-config answers `-I/usr/local/include -L/usr/local/lib
  -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3`
  (`u2404/pkgconfig-answer.txt`); `libSDL3.so.0` itself carries NO RPATH or
  RUNPATH (`u2404/libSDL3-dynamic.txt`). SDL's CMakeLists.txt:1868-1882 adds
  `--enable-new-dtags` only on BSDI/FreeBSD/Linux/NetBSD and only when
  `check_c_compiler_flag` with that word passes on the configuring machine;
  Solaris gets `-R${libdir}`; `:2471-2472` another branch gets `-Wl,-rpath`
  alone. So a `.pc` carrying the dtags word is written by a machine whose
  linker accepted it.
- 08:20:42 the default tag on Ubuntu 24.04 (`u2404/dtags.txt`): arm64 clang
  and `ld` alone RUNPATH, `--disable-new-dtags` RPATH; **x86-64: Ubuntu's own
  GNU ld 2.42 for that target, `binutils-x86-64-linux-gnu 2.42-4ubuntu2.10`
  (the cross build of the same source package on this arm64 host), `ld -shared
  -rpath` alone writes RUNPATH, `--disable-new-dtags` RPATH**. The native amd64
  build of that package on the CI's runner is still unrun; clang's driver line
  for that target calls `x86_64-linux-gnu-ld` with no dtags word of its own.
- 08:21:34 WHY (`u2404/binutils-src.txt`, `binutils-dtags-patch.txt`, from
  `apt-get source binutils` 2.42-4ubuntu2.10): **upstream GNU ld defaults to
  RPATH** (`ld/configure.ac:176`, `:534-539`, `ac_default_new_dtags=0` unless
  configured `--enable-new-dtags`); Debian's packaging passes
  `--enable-new-dtags` in `BINUTILS_BASECONF` (`debian/rules.defs:52`, every
  target), changelog *Configure with --enable-new-dtags* in
  2.28.51.20170620-1 and *enable new dtags by default for linux/gnu targets.
  Closes: #835859* in 2.27.51.20161116-2. So every project Linux toolchain
  (Debian 13, Ubuntu 24.04, both arches) writes RUNPATH; **a GNU ld built
  with upstream's default writes RPATH**, and on it route (F) would hand a
  package that asked RUNPATH the opposite. Which distributions ship that is
  not measured here (a question, not searched).
- GNU ld `--help` (`linker-help-gnu.txt`): `--export-dynamic-symbol-list FILE`
  begins with `--export-dynamic` and reads a file; `-R FILE, --just-symbols
  FILE ... (if directory, same as --rpath)`; `-Map FILE/DIR` writes;
  `-z` takes many keywords. Any admission is by WHOLE WORD, never by prefix.
- 08:22:51 **Q5, run** (`scripts/q5-dlopen.sh`; `u2404/q5-dlopen.txt`,
  `b14/q5-dlopen.txt`, identical): a stand-in `libstub.so` (no RUNPATH of its
  own, as `libSDL3.so.0` measured) `dlopen`s `libback.so.1` by name; copies in
  A and B; the program links libstub from A with `-Wl,-rpath,A`. (A first run
  was contaminated, the program's own `dlopen` loading A's copy before the
  library asked; split into two programs.)

  | word | tag | the LIBRARY's dlopen, plain / `LD_LIBRARY_PATH=B` | the PROGRAM's own dlopen |
  |---|---|---|---|
  | none | RUNPATH | none / B | A / B |
  | `--enable-new-dtags` | RUNPATH | none / B | A / B |
  | `--disable-new-dtags` | RPATH | A / A | A / A |

  So yes, the tag changes what a dlopening library finds: under RPATH the
  program's rpath reaches its libraries' `dlopen`s and `LD_LIBRARY_PATH`
  cannot move them; under RUNPATH neither holds. `--enable-new-dtags` is
  identical to no word on both images. Consequence: **dropping
  `--disable-new-dtags` (route F) changes what a correct program loads at run
  time (A becomes none)**, the libpsx shape of "links, then misbehaves".
- 08:24:11 **Windows box** (`ssh win`, MINGW64 on Windows 10.0.26100, clang
  23.1.1 `x86_64-pc-windows-msvc`; my folder `/c/w/p198-ffi-pragmatist-1069`;
  `scripts/words-win.sh`, `win-words.tsv`; an `scp` to `/c/w/` failed and left
  nothing, checked with `ls`). `clang -###` drives `lld-link`. With the
  compiler's own `-Wl,/STACK:67108864 -Wl,/INCREMENTAL:NO`:
  `--enable-new-dtags`, `--disable-new-dtags`, `--as-needed`,
  `--export-dynamic`, the whole-archive pair: *lld-link: warning: ignoring
  unknown argument*, exit 0; `-Wl,-z,now`: warning on `-z`, then *error: could
  not open 'now'*, exit 1; `-pthread` at a link and at a compile and
  `-isystem` at a compile: exit 0, no word.
- 08:24:29 **THE ADMITTED `-Wl,-rpath,<value>` NAMES A FILE ON lld-link**
  (`win-rpath-input.txt`): `-Wl,-rpath,C:/opt/a` warns on `-rpath`, then
  *could not open 'C:/opt/a'*, exit 1; and with `h.o` (my own object holding
  `helper`) given ONLY as the rpath's value, `clang m.c ... -Wl,-rpath,h.o`
  links, exit 0, and prints `7 2.0`; the split spelling `-Wl,-rpath -Wl,h.o`
  the same. Without it: *undefined symbol: helper*. So on Windows the list's
  own property (*does it name a file at build time*) fails for a word it
  admits today, unreached only because the box has no `pkg-config`.
- 08:26:25 **`-isystem` and header verification** (my standing concern; `isys/`,
  `run1.txt`, `run2.txt`, this Mac, the frozen compiler): a header of mine,
  `int64_t w_count(int64_t *out, int64_t n)` plus a macro spelling, bound five
  ways, the header found once through `CPATH` (an `-I` directory to clang) and
  once through `C_INCLUDE_PATH` (which clang reads as a SYSTEM directory, the
  stand-in for a package's `-isystem` until a prototype passes the word
  itself): the right binding builds and prints `21 40` both ways;
  `ffi_parameter_type` (width), `ffi_parameter_type` (sign), `ffi_return_type`
  and `ffi_macro_name` fire IDENTICALLY both ways. The checks are the
  compiler's own questions to clang about the header, not warnings at a
  location, so a system directory does not silence them here.
- 08:26:40 **`-pthread`** (`scripts/pthread-macros.sh`, `-std=gnu11` as the
  compiler compiles): at a compile it adds `#define _REENTRANT 1` on this Mac,
  Debian 13 and Ubuntu 24.04, and `_MT 1` on the Windows box; at a link it
  adds `-lpthread` on both Linux images and nothing on Darwin or Windows. So it
  is a driver spelling of a `-D` and an `-l` the list already admits: names no
  file the list does not already let a package name, loads nothing, writes
  nothing. At a compile alone (`-c`) no platform warned (`words*.tsv` rows).
- 09:51 RESUMED after the session limit (08:25) and the Mac's sleep (to 09:40),
  on the coordinator's message. State read from this file: the three
  containers still up, my copy and its compiler intact. The Q5 split into two
  programs was DONE at 08:22:51 (table above); re-run below as the
  coordinator asked.
- 09:54:56 **SDL3 end to end, the frozen compiler, built from the seed inside
  each container** (`/tmp/fz/heroes`, built 06:27 UTC = 08:27; scripts
  `scripts/sdl-e2e.sh`; outputs `b14/sdl-e2e-frozen.txt`,
  `u2404/sdl-e2e-frozen-*.txt`):
  - `sdl3-b14` + my stand-in `sdl3.pc` (`standin/sdl3.pc`, the brief's text),
    `PKG_CONFIG_PATH=/w/standin`: pkgconf answers `-Wl,-rpath,/usr/lib/aarch64-linux-gnu
    -Wl,--enable-new-dtags -lSDL3`; the event case and `examples/sdl/main.hero`
    both exit 1 `ffi_package ... answered with -Wl,--enable-new-dtags`; the
    event case with `link "SDL3"` builds, exit 0, prints its `.expected`.
  - `ubuntu:24.04` arm64, SDL 3.2.10 (`877399b2`) built from source by
    `scripts/u2404-setup.sh` (the CI step's cmake lines, `SDL_RPATH` at its
    default), the REAL pkg-config answer `-I/usr/local/include -L/usr/local/lib
    -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3`: both programs
    exit 1, same refusal.
  - **(J) on the source build, answered**: `extern "SDL3/SDL.h" link "SDL3"`
    BUILDS with no flag (clang finds `/usr/local/include` by default and GNU ld
    searches `/usr/local/lib`; exit 0), and the binary has no RUNPATH. Run
    BEFORE `ldconfig`: exit 127, *error while loading shared libraries:
    libSDL3.so.0: cannot open shared object file* (`ldconfig -p | grep -c SDL3`
    = 0). After `ldconfig` (as the CI step runs it): prints `true true true 42
    true`, `.expected`. With `LD_LIBRARY_PATH=/usr/local/lib`, the same. So
    what a reader must know: the way out `link` carries no rpath, so the
    library must be in the loader's cache or `LD_LIBRARY_PATH`; `package`
    carried the rpath and needed neither.
- 09:55:30 **Q2, verified by a run, not only read**: `whole-archive-unclosed`
  (`mac/words.tsv` has no GNU ld; `b14/words-gnu.tsv` and `words-lld.tsv` row
  `whole-archive-unclosed`) — `-lm -Wl,--whole-archive -lw` with no closing
  `--no-whole-archive` exits 1, *multiple definition* from `libgcc.a`'s
  objects pulled in whole. So a route that refuses `--whole-archive` and
  `--no-whole-archive` by name but passes `-lpsx` through (route D's own
  example) does not merely drop a want-to-have: the link can refuse to
  produce a binary, or produce one whose archive is pulled in whole with
  nothing to re-hide it. And `filter_words`/`link_words` are each a SINGLE
  linear pass over the package's words that only ever pushes (never
  reorders or buckets by kind), so any route built by widening `valued`,
  `allowed_prefixed` or `linker_value` keeps each admitted word's position
  relative to every other, `-l` names included, automatically — confirmed by
  reading `filter_words` (`libraries.hero:84-133`) and `link_words`
  (`:174-197`) again against this run. A route that instead re-buckets by
  flag kind (all `-l` first, then all `-Wl,`) would be the one that breaks
  this, and none of (B) through (J) does that.
- 09:55:12 edge note, ld64 (not asked by the brief, found beside Q4): the
  admitted `-Wl,-rpath,<dir>` already has a macOS-only wrinkle — a value
  itself beginning `@` is not a response file there (`-Wl,@args.rsp` alone
  is refused by ld64's own parsing, *unknown options*, so ld64's `@file` door
  the brief and design.md §4.19 worry about on ELF does not reparse inside a
  `-Wl,` value on this platform); but `@loader_path/...` IS a live ld64 macro
  and an already-admitted `-Wl,-rpath,@loader_path/../lib` links clean with
  an `LC_RPATH` of exactly that string, which is the same run-time-relocatable
  shape Q6 is about, not a new one.
- 09:58 **My recommended route, derived from the measurements above**: not
  (F) as the shared brief states it (name both words, pass neither, "keep
  the toolchain's default") — my own Q5 run shows that silently NOT writing
  `--disable-new-dtags` when a package sends it would hand the package the
  RUNPATH default instead of the RPATH it asked for, and Q5 proves RPATH vs
  RUNPATH changes what a `dlopen`ing library finds. That is a real run-time
  change, exactly the shape Q6 is about, and I will not wave it past the
  veto question that is the author's to re-open.
  **Narrower than (F): recognise `--enable-new-dtags` by name and DROP it
  silently (never hand it to the linker at all), leave `--disable-new-dtags`
  refused exactly as today.** Grounds, all measured above: `--enable-new-dtags`
  is byte-for-byte a no-op next to no word at all on every Linux toolchain this
  project ships on (M3, and my own `words.tsv` rows `rpath-alone` and
  `enable-new-dtags` are IDENTICAL `readelf -d` output on ld-27037.1's refusal
  aside, Debian GNU ld 2.44, Debian LLD 22.1.8, Ubuntu GNU ld 2.42, the
  emulated x86-64 Debian GNU ld 2.44); it is a raw linker failure on ld64
  (M1) and a warning on lld-link (M2) if ever forwarded, so it must be
  dropped, never passed on, on every platform; and because it is dropped
  rather than admitted-and-forwarded, this never reaches ld64 or lld-link and
  reopens no run-time question — the binary's tag is left to come from the
  toolchain's own default, which M3 measured as RUNPATH everywhere already,
  so the resulting binary is bit-for-bit what it would have been had the
  package never said the word. This is **(B-asym) done as a drop, not a
  pass-through**: it needs no x86-64-native confirmation to be safe (the
  change is a no-op by construction, not a forwarded flag whose effect
  depends on the toolchain), though the CI's native x86-64 leg remains unrun
  here and should still be watched at the push gate.
  **Q8**: once this lands, `package "sdl3"` with `SDL_RPATH` ON builds (the
  companion `-Wl,-rpath,/usr/local/lib` is already on the list and is passed
  through unchanged), and the resulting binary's tag is the toolchain default
  (RUNPATH, measured), identical to today's `-DSDL_RPATH=OFF` + `ldconfig`
  binaries in every way that matters to the loader. So `-DSDL_RPATH=OFF` in
  `ci.yml:347` stops being NECESSARY for the build to succeed; dropping it or
  keeping it is then a test-environment choice with no linker-semantics
  reason to force either way — not mine to answer which, only that it is no
  longer forced.
- What this route explicitly does NOT do: it does not touch `--disable-new-dtags`,
  `-Wl,-z,relro`, `-Wl,-z,now`, `--as-needed`/`--no-as-needed`,
  `--export-dynamic`, `--whole-archive`/`--no-whole-archive`, `--exclude-libs`,
  `-pthread` as a `-Wl,` word (it already works as a driver flag the list
  does not need to see specially), or any of (C), (D), (E), (G), (H) — those
  stay open questions for the full panel, not settled by this seat alone.
