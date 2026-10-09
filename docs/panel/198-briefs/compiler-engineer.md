# Panel 198, compiler-engineer

Read `00-shared.md` first, whole. Your directory
`<scratchpad>/198-compiler-engineer/`: your copy in `tree/` (`cp -R`, then
`rm -f tree/.git`, as the shared brief says), your running notes in
`report.md`. **Amended on 2026-10-08 after the critic's first pass**
(`docs/panel/198-reports/completeness-critic-pass1.md` § 6, repairs 9 and 10,
and its § 2 pointers for this seat): the first version pointed at
`allowed_prefixed` and `refused` as the place to build, asked for a `surface`
row that does not exist, and weighed `-Wl,` words alone.

Your seat judges the ceiling (design.md §1.1, §1.7, Part 5) and holds a veto
on soundness.

1. **The census of what packages answer, every refused word with equal
   weight** (Q1). Start from the critic's tables, M4's paths, and widen them:
   - this Mac (pkg-config 3.0.7, `pkg-config --list-all` 505 lines; the
     census holds 499 packages);
   - `heroes-linux-arm64:sdl3-b14` (Debian 13, pkgconf 1.8.1, `--list-all`
     155 lines against the census's 124 packages): say which are missing
     and why;
   - `ubuntu:24.04` arm64 with the CI's install list (`ci.yml:316`, `clang
     lldb pkg-config libsqlite3-dev libcurl4-openssl-dev`) **and SDL 3.2.10
     built from its source with `SDL_RPATH` at its default (ON)**:
     `<scratchpad>/batch14/box/ci/ci-step2.sh` is that build (it needs the
     network to clone SDL; copy it into your directory, never run it from
     the lane's folder).

   For each package, `pkg-config --cflags --libs -- <name>`, then every word
   today's list refuses, with its package, **by class**: `-Wl,` words;
   `-pthread`; `-isystem <dir>`; `-W...` warnings; anything else (uvwasi's
   one word holding `\;`). Count each class, and name the **position-dependent
   pairs** (`--whole-archive` and `--no-whole-archive`, `--as-needed` and
   `--no-as-needed`, `--start-group` and `--end-group`, `--push-state` and
   `--pop-state`) with their packages; and every `-Xlinker` spelling (refused
   today, `libraries.hero:400`).

2. **Build the route you adopt** in your copy, and run its cases there. The
   places, read in the frozen tree:
   - whole words are judged in `filter_words` (`selfhost/cli/libraries.hero:84-133`),
     the `-Wl,` shapes through `linker_value` (`:229-241`) with `clean_tail`
     (`:260-268`), which refuses a comma (Q3); `allowed_prefixed`
     (`:212-221`) holds only the joined valued flags, and `refused`
     (`:199-206`) only writes the message;
   - a word reaches the link through `link_words` (`:174-197`), which passes
     every `-Wl,` word on every platform, and `link_line`
     (`selfhost/cli/link.hero:133-151`), which puts a package's words after
     the objects and before the `link` names (Q2); a compile takes
     `compile_flags` (`libraries.hero:148-155`, `-I -D -U -F` only), read by
     `selfhost/cli/compiling.hero:86`, so a compile word such as `-isystem` or
     `-pthread` needs a place there too;
   - the platform is asked once, `process.exe_suffix()` (`libraries.hero:59-63`,
     `selfhost/cli/flags.hero:162-179`); a route keyed on ELF against Mach-O
     (B-ELF) adds a question the compiler does not ask today (Darwin is told
     apart only by `uname -s` in `selfhost/cli/doctor.hero:150-152`): price it;
   - the cases, through a stand-in `pkg-config` on `PATH` as
     `tests/harness/absence.hero` does (`stand_in`, `:1575-1592`;
     `built_said`, `:1557-1573`): a package answering each admitted word,
     each word still refused, the comma-tunnelled
     `-Wl,--enable-new-dtags,--plugin,x.so`, and libpsx's whole answer.

   What judges the prototype, each run in your copy with your compiler:
   - the compiler's own tests, `heroes test selfhost/main.hero`, among them
     the tests that pin the note's text and the list
     (`libraries.hero:333`, `:372`, `:443`, `:467`, `:488`, `:524`; needles at
     `:355`, `:390`, `:475-477`, `:496`) and `selfhost/emit/ffi_build.hero:399-402`;
   - `heroes run tests/harness/main.hero -- ./heroes run sdl3` (the `run`
     form narrowed by its third word to its one SDL3 case; zero matches exit
     2) and `-- ./heroes unsupported package` for the three package cases of
     `tests/golden/unsupported/` (`ls | grep package`: six files);
   - `examples/sdl`: `corpus` takes no case filter
     (`tests/harness/main.hero:515-523`, `takes_cases`), so run `corpus`
     whole or build and run `examples/sdl/main.hero` against
     `examples/sdl/main.expected` by hand, and say which;
   - there is **no package row in `surface`** (`grep -c package
     tests/harness/suite_surface.hero`: 0): say whether the route owes one,
     and if so what it would pin.

   Build the program under your route in the reproducer that refuses today,
   the stand-in `sdl3.pc` in `sdl3-b14` (the ffi-pragmatist's brief gives its
   text; measured to refuse at exit 1 between 07:56:51 and 07:57:12), not on
   Debian's own `.pc`,
   which answers no `-Wl,` word.

3. **What the route costs the reader.** The note's text before (the shared
   brief quotes it) and after, as your prototype prints it for three answers:
   the source-built `sdl3` (`-I/usr/local/include -L/usr/local/lib
   -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3`), Debian 13's
   `libcurl` (`-I/usr/include/aarch64-linux-gnu -isystem
   /usr/include/mit-krb5 -I/usr/include/p11-kit-1 -lcurl`) and `glib-2.0`
   (`-I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
   -I/usr/include/sysprof-6 -pthread -lglib-2.0`), each read in `sdl3-b14`
   at 07:53 or from `ci-step2-out.txt:891`. **Write each message whole into
   `report.md` under a heading `notes for the blind seat` as soon as it
   exists**: the blind seat's variant B reads them. Where your route admits
   the word and the program builds, say so; where it still refuses, the
   message is the variant. Whether a spec sentence moves (`grep -n 'package'
   spec/heroes-spec.md`: `:445`, `:448`) is the spec-warden's to price; say
   what the route needs the reader to know before the message fires.

4. **The rpath's reasoning** (`eac3e5b5`'s comment, absent from the port,
   shared brief): does your route restore the argument beside the list, and
   does it change how `-Wl,-rpath,<dir>` is judged?

Your verdict names the route, its cost in lines and modules with file
citations, and every other route's cost from (A) to (J), each priced or
marked *not priced*. Veto on soundness only.
