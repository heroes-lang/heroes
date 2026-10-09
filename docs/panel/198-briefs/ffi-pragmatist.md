# Panel 198, ffi-pragmatist

Read `00-shared.md` first, whole. Your directory
`<scratchpad>/198-ffi-pragmatist/`: your copy in `tree/` (`cp -R`, then
`rm -f tree/.git`), your running notes in `report.md`. **Amended on
2026-10-08 after the critic's first pass**
(`docs/panel/198-reports/completeness-critic-pass1.md` § 6, repairs 11, 12
and 13, and its Q5 and Q6): the first version named `sdl3-b14` as the
reproducer (Debian's `.pc` answers no `-Wl,` word), misspelled two words,
assumed which linker the CI's x86-64 leg drives, asked the run-time
question of the word that restates the default rather than of the one that
moves, and gave you a veto clause a ruling of 2026-08-15 had already put
outside the list's test.

Your seat judges the founding constraint (design.md §1.11, §4.19) and holds a
veto on ABI breakage and on a word that names a file, loads anything or
writes anything at build time.

1. **What each word does on each linker the compiler drives**: does it name
   a file, load anything (a plugin, a script), or write anything at build
   time? Cite each linker's own documentation and run each word on a
   hand-made link. The linkers, as read on 2026-10-08:
   - ld64 `ld-27037.1` on this Mac (`ld -v`);
   - GNU ld 2.44 in `heroes-linux-arm64:sdl3-b14` (Debian 13) and in
     `heroes-linux:latest` (Debian 13, x86-64, emulated), and Ubuntu 24.04's
     GNU ld 2.42 *(critic)*; the compiler passes no `-fuse-ld` (`grep -rn
     'fuse-ld' selfhost runtime`: 0), so LLD 22.1.8 in `sdl3-b14` is driven
     only where clang's default linker is lld: say whether any platform the
     project builds on is one. Which linker the CI's x86-64 leg (Ubuntu
     24.04, `ubuntu-latest`) drives through clang is **unrun**;
   - lld-link 23.1.1 on the Windows box, under clang 23.1.1 targeting
     `x86_64-pc-windows-msvc` (`ssh win`, 07:50).

   The words, as a package spells them: `-Wl,--enable-new-dtags`,
   `-Wl,--disable-new-dtags`, `-Wl,--as-needed`, `-Wl,--no-as-needed`,
   `-Wl,-z,relro`, `-Wl,-z,now`, `-Wl,--export-dynamic` (gmodule; every POSIX
   link already carries `-rdynamic`, which Debian 13's clang hands the linker
   as `-export-dynamic`), `-Wl,--whole-archive` and `-Wl,--no-whole-archive`
   (libpsx), `-Wl,--exclude-libs`; and, as the shapes the list exists to
   refuse (Q4), `-Wl,--version-script` (names a file), `-Wl,--plugin`
   (loads), `-Wl,-T` (names a file), `-Wl,@file`, `-Xlinker`,
   `-Wl,-dynamic-linker`, `-Wl,-R`, `-Wl,--just-symbols`. And Q1's words that
   are no `-Wl,`: `-pthread` at a compile and at a link, `-isystem <dir>`.
   M1 and M2 (shared brief) are the dtags words on ld64 and lld-link
   already: build on them.

2. **The run-time effect of the dtags words, the right way round.** With
   `-Wl,-rpath` alone, every Linux toolchain measured writes RUNPATH (M3), so
   `--enable-new-dtags` restates the default and **`--disable-new-dtags` is
   the word that moves**: it writes RPATH, searched before `LD_LIBRARY_PATH`
   (the critic's run, `dtags-order.txt`). Measure what is still open:
   - the default on Ubuntu 24.04 x86-64, the CI's leg (unrun; Debian 13
     x86-64 emulated wrote RUNPATH at 07:54);
   - whether any toolchain the project supports writes RPATH by default (a
     question: upstream GNU ld's own default is unverified here), because
     under (F) such a toolchain would give a package that asked for RUNPATH
     the opposite;
   - **Q5**: RUNPATH reaches only a binary's direct `DT_NEEDED` entries
     (ld.so(8), not run), and SDL3 loads its backends with `dlopen` (the
     critic's statement, not run here). With two copies of one `dlopen`ed
     library, does the tag the
     program carries change what SDL3 (or a stand-in that `dlopen`s) finds?

   **Your veto, and Q6.** You veto a word that names a file, loads anything
   or writes anything at build time (the list's property,
   `selfhost/cli/libraries.hero:11-13`). Whether a word that moves who
   chooses the library at run time is also a veto is not handed to you as a
   premise: the answer (c) of 2026-08-15 put run-time properties outside the
   list's test (*a guard is not widened to cover a class it cannot see*,
   `issues/2026-08/17/2026-08-17-0221-answered-2026-08-15-and-written-as-a-refusal-with.md`),
   and `-Wl,-rpath,<dir>` was admitted on 2026-08-14 with that same escalation
   written down and no sitting
   (`issues/2026-08/14/2026-08-14-0023-the-corpus-gets-a-timeout-joins-the-allow-list-and.md`,
   `eac3e5b5`). If you would veto on it, cite both and say why they no longer
   hold: that is a reopening, and the author's to decide.

3. **SDL3 end to end, on a reproducer that refuses.** Not `sdl3-b14` alone.
   Two ways, the second the real one:
   - **a stand-in `sdl3.pc`** over Debian's install in `sdl3-b14`, measured
     between 07:56:51 and 07:57:12 to make real pkgconf answer
     `-Wl,-rpath,/usr/lib/aarch64-linux-gnu -Wl,--enable-new-dtags -lSDL3`
     and the frozen compiler refuse at exit 1; put it in a directory of yours
     and name it in `PKG_CONFIG_PATH`:

     ```
     prefix=/usr
     libdir=/usr/lib/aarch64-linux-gnu
     includedir=/usr/include
     Name: sdl3
     Description: a stand-in for SDL 3.2.10 built from its source with SDL_RPATH at its default, Debian's install beneath it
     Version: 3.2.10
     Libs: -L${libdir} -Wl,-rpath,${libdir} -Wl,--enable-new-dtags -lSDL3
     Cflags: -I${includedir}
     ```

   - **the source build itself**, `<scratchpad>/batch14/box/ci/ci-step2.sh`
     (copied into your directory; it clones SDL, so it needs the network), in
     `ubuntu:24.04` arm64, which installs into `/usr/local` with `SDL_RPATH`
     at its default and answers `-I/usr/local/include -L/usr/local/lib
     -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3`
     (`ci-step2-out.txt:891`).

   The refused programs are `examples/sdl/main.hero` and
   `tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero` (its
   `.expected` is `true true true 42 true`, one per line). Build each under
   the route you would adopt, in your copy (the compiler-engineer's
   prototype is its own, in its directory: build yours or read its
   report), and run the event case. And **(J)**: the event case with
   `extern "SDL3/SDL.h" link "SDL3"` built and printed its `.expected` over
   Debian's install at 07:57; run it on the source build in `/usr/local`,
   where it is unrun, and say what a reader must know for it to build there.

4. **Q7, Windows.** Defect 447 is open; `package` has never run on Windows,
   and the box has no `pkg-config` or `pkgconf` on `PATH` (07:50). What does
   each route do there: a route that admits the dtags words on every
   platform hands lld-link a word it warns about (M2). One clang at a time,
   your own folder `/c/w/p198-ffi-pragmatist-<pid>`, never remove anything.

5. **Q2 and Q8.** Does a route keep each word's position relative to its
   `-l` (libpsx's `--whole-archive -lpsx --no-whole-archive`, and the
   `--as-needed` its answer leaves set before every `link` name
   `selfhost/cli/link.hero:145-149` puts after it)? And the CI's
   `-DSDL_RPATH=OFF` (`.github/workflows/ci.yml:347`) after the ruling: kept,
   dropped, or changed, and what the Linux legs would then measure.

Your verdict names the route from the boundary's side, with the C and the
link lines you ran.
