# Panel 198, completeness critic, first pass: the briefs against the frozen tree

Copied by the coordinator on 2026-10-08 from the critic's reply (a subagent's
Write of a report file is refused). The critic read the clock at 01:44:36,
01:49:07, 01:51:59, 01:52:14, 01:52:42, 01:53:04, 01:55:29 and 01:57:11. `T` is
`.claude/worktrees/lane-panel-198` at `56def9b4`; `S` the session scratchpad;
its probe outputs in `S/198-critic/`. Read-only on the tree, the trunk and the
worktrees; probes in throwaway `--rm` containers, on this Mac in `S/198-critic/`,
and two read-only `ssh` attempts. No paid run.

## 1. The lane: the full panel is owed

- Every listed route changes the `ffi_package` note's text: (A) by design;
  (B) to (E) because the note lists the accepted set (`libraries.hero:199-201`,
  *"The list in it is the function above, word for word"*).
- That note is now the rule's only statement a reader sees: `grep -n 'package'
  spec/heroes-spec.md` returns `:445` and `:448` only, neither states the list;
  the spec's sentence (*"A package answering with anything this compiler does
  not pass on is refused, naming what it said"*) was removed on 2026-10-05 by
  panel 192 R8 item r1 (`git log -S'does not pass on' -- spec/heroes-spec.md`
  gives `d0409cc5`), on the ground that the message carries the rule
  (`192-reports/spec-warden.md:527-538`).
- The skill's wording: `SKILL.md:18-23` allows the lane only for *"no surface,
  no diagnostic and no spec token"*; `:29` *"When in doubt take the full
  panel."*; `:33` *"A question that looks internal but has a sentence in the
  spec behind it is a full panel."*
- A word the list admits changes which programs `build` accepts: semantics
  under CLAUDE.md §4.
- Routes (D) and (E) contradict design.md §4.19 (`docs/design.md:2639-2642`:
  the list holds against the `@file` door *"because it names what is
  permitted rather than what is forbidden"*).
- The precedent for widening this list sat full (`055-where-a-header-is.md:6`,
  *"Lane: full, five judges"*; it admitted `-D`, `-U`, `-Wl,-framework`, `:173`).
- What the lane gives up: Go's list, the precedent `libraries.hero:6-8` cites
  (Go 1.27.1, `security.go`), admits both words by name at `:208`
  (`-Wl,--disable-new-dtags`) and `:210` (`-Wl,--enable-new-dtags`), and lets
  the person building widen it at `:367-383` (`CGO_LDFLAGS_ALLOW` /
  `_DISALLOW`); the historian would bring this; the llm-ergonomist would judge
  route (A)'s note, the only fix text a reader is given.

## 2. Each framing fact

Shared brief: tree `56def9b4` verified (`git log --oneline -6`; trunk since at
`02256c1e`, nothing in `cli/`); written 23:42 (`stat`); 444 `systemic`, its
path, verified; **"with default options" partly false**: the run's cmake sets
seven options (`-DCMAKE_BUILD_TYPE=Release`, `-DSDL_TESTS=OFF`,
`-DSDL_EXAMPLES=OFF`, `-DSDL_STATIC=OFF`, `-DSDL_UNIX_CONSOLE_BUILD=ON`,
`-DSDL_X11=OFF`, `-DSDL_WAYLAND=OFF`, `S/batch14/box/ci/ci-step2.sh`) with
`SDL_RPATH` at its default ON (`ci-step2-out.txt:505`), the answer at `:891`:
`-I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib
-Wl,--enable-new-dtags -lSDL3`; the refusal verified at `:895` and `:920` (one
per program), note at `:900`; arm64 consistent (the local `ubuntu:24.04`
reports `aarch64`); 213's `-DSDL_RPATH=OFF` at `ci.yml:347` with its reason at
`:331-337`; `libraries.hero:5-15` quotes verified, omitting `:13-15` (*What a
library does AT RUN TIME the list cannot see*); the note at `:202-205`
verified, omitting its second line (*name the library directly with `link`*);
panel 050 verified, omitting `:65-66` (*The list is named in the spec, because
the list is the safety*) and its prediction 3 at `:128`; (B)'s *choose
DT_RUNPATH or DT_RPATH* true but incomplete (M3); the copy rule harmless but
`T/.git` is a file pointing at the frozen tree's gitdir, so `git` in a copy
reads and refreshes the frozen index; a dozen lanes approximately right (16
worktrees); Docker images verified free at 01:49; **the Windows box false at
01:52** (ssh timed out twice); report files refused, verified from the record
(it departs from `SKILL.md:278-283`).

Compiler-engineer: Homebrew `pkg-config` 3.0.7, 505 entries, 499 resolve;
`ci.yml:316` installs `clang lldb pkg-config libsqlite3-dev
libcurl4-openssl-dev`; **`allowed_prefixed` and `refused` point to the wrong
place**: whole `-Wl,` words are judged in `filter_words` (`:84-145`) through
`linker_value` (`:229-241`), `clean_tail` (`:260`) refuses a comma, and they
reach the link through `link_words` (`:174-197`), which passes every `-Wl,`
word on every platform; the note's text is pinned by the tests at `:333` and
`:372` (asserts `:355`, `:477`); platform checks `process.exe_suffix()`,
`cli/doctor.hero:152`; `absence.hero`'s stand-in `pkg-config` at
`:1285-1292`, `:1558-1571`; **"`surface` narrowed to the package rows" false**:
no package row in `suite_surface.hero`; 26 case or example files hold
`package "`, among them `run/ffi-a-construction-polls-an-sdl3-event` and
`examples/sdl/main.hero`.

Ffi-pragmatist: ld64 `ld-27037.1`; GNU ld 2.44 and Debian LLD 22.1.8 in
`sdl3-b14`; lld-link on the box from the record; the compiler passes no
`-fuse-ld`, so ELF lld is driven only where clang's default linker is lld,
which no project platform measured is; *the CI's x86-64 runs GNU ld through
clang* an inference (Debian clang 22's driver calls `/usr/bin/ld`); `-z,relro`
and `-z,now` misspelled (a package answers `-Wl,-z,relro`, `-Wl,-z,now`);
RUNPATH after `LD_LIBRARY_PATH`, RPATH before, **verified by a run**
(`S/198-critic/dtags-order.txt`: a RUNPATH binary loads B's library under
`LD_LIBRARY_PATH=B`, an RPATH binary keeps A's); RUNPATH reaches only direct
`DT_NEEDED` entries (ld.so(8), not run); **`sdl3-b14` false as a reproducer**:
Debian's `sdl3.pc` answers no `-Wl,` word (`docker-b14.txt`); only a source
build with `SDL_RPATH` on reproduces the refusal (`ci-step2.sh`, network
needed); the refused programs are `examples/sdl/main.hero` and
`tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero`.

## 3. Measurements the briefs should hand the seats

- **M1, ld64**: `clang m.c -Wl,--enable-new-dtags` stops *ld: unknown options:
  --enable-new-dtags*, exit 1; `--disable-new-dtags` the same
  (`mac-dtags*.txt`). Admitted everywhere, (B) turns a Heroes diagnostic into
  a raw linker failure on macOS.
- **M2, lld-link 22 (Docker)**: *lld-link: warning: ignoring unknown argument
  '--enable-new-dtags'* (`lld-link.txt`): a warning on a correct program is
  `blocking`.
- **M3, the default tag**: with only `-Wl,-rpath`, Debian GNU ld 2.44 through
  clang and LLD 22 write RUNPATH; Ubuntu 24.04's GNU ld 2.42 (aarch64) writes
  RUNPATH by default and RPATH under `--disable-new-dtags`
  (`ubuntu2404-dtags.txt`). So `--enable-new-dtags` restates the default;
  `--disable-new-dtags` moves the package's rpath ahead of `LD_LIBRARY_PATH`.
  x86-64 default not run.
- **M4, the census**: on this Mac 499 packages answer no refused `-Wl,` word,
  221 refused anyway (219 `absl_*` for `-Wno-*`, `libmpdec++` for `-pthread`,
  `uvwasi`); in `sdl3-b14`, 23 of 124 refused: 13 for `-pthread` (`glib-2.0`,
  `gio-2.0`, `gobject-2.0`, three `gmodule`, `gthread-2.0`, `libpulse` ×3,
  `ibus-1.0`, `sysprof-capture-4`), 9 for `-isystem /usr/include/mit-krb5`
  (`libcurl` and the krb5 family), `gmodule-2.0` and `gmodule-export-2.0` also
  for `-Wl,--export-dynamic`, `libpsx` for `-Wl,--no-as-needed
  -Wl,--whole-archive -lpsx -Wl,--no-whole-archive -Wl,--as-needed`
  (`S/198-critic/*census*.tsv`, `*refused.tsv`); no filed defect for these.

## 4. Routes the list missed, and questions not asked

Routes: **(F)** the compiler owns the tag: recognises both words by name,
passes neither, keeps the toolchain's default (RUNPATH on both Linux
toolchains measured); never reaches ld64 or lld-link. **(B-ELF)** (B) on ELF
only (M1, M2). **(B-asym)** admit `--enable-new-dtags` only (it never gives the
package more than the admitted `-Wl,-rpath`; `--disable-new-dtags` stays
refused); needs the x86-64 default. **(G)** the person building admits a word
(argv flag or environment; Go's `CGO_LDFLAGS_ALLOW`); a CLI surface. **(H)**
Go's vetted list filtered by Heroes' own test (Go also admits
`--just-symbols` `:221`, `-R` `:220`, direct `.so` inputs `:236`). **(J)** the
`link` way out the note names: does `extern "SDL3/SDL.h" link "SDL3"` build on
the source-build machine (not run).

Questions: **Q1** the whole class or only `-Wl,`: `package "glib-2.0"` and
`package "libcurl"` are correct programs refused on Debian 13 (M4; CL-078);
panel 188's argument rests on `-isystem` refused
(`188-reports/compiler-engineer.md:538`). **Q2** words whose position matters:
under (D), dropping `--whole-archive` from libpsx's answer links and then
misbehaves; does (C) keep each word's position relative to its `-l`. **Q3**
comma tunnelling: `-Wl,--enable-new-dtags,--plugin,x.so` must stay refused (Go
`:195-198`; `clean_tail` `:260`). **Q4** shapes for (E): `-Wl,@file`,
`-Xlinker`, `-dynamic-linker`, `-R`, `--just-symbols`; cite
`design.md:2639-2642`. **Q5** RUNPATH reaches only direct dependencies; SDL3
loads its backends with `dlopen`: does the tag change what SDL3 finds. **Q6**
the veto clause *moves the library choice to someone else* is a run-time
property; the 2026-08-15 answer (c) ruled run-time properties outside the
list's test (*The allow-list gets no second half… a guard is not widened to
cover a class it cannot see*, `issues/2026-08/17/2026-08-17-0221-…`), and the
admitted `-Wl,-rpath` fails the same clause (`issues/2026-08/14/2026-08-14-0023-…`,
`eac3e5b5`, admitted with no sitting): cite or reopen. **Q7** Windows: open
defect 447, `package` never ran there. **Q8** the CI's `-DSDL_RPATH=OFF` after
the ruling.

## 5. Precedents to cite

`eac3e5b5` and its decision of 2026-08-14; the answer (c) of 2026-08-15; panel
055 (full); panel 192 R8 item r1 (`d0409cc5`); `design.md:2639-2642`; panel
188's compiler-engineer at `:538`; Go 1.27.1 `security.go` `:201`, `:208`,
`:210`, `:234`, `:367-383`; defect 447.

## 6. Repairs to the briefs

1. Convene the full panel. 2. *with default options* → *with `SDL_RPATH` at its
default (ON), seven other options set*. 3. Add `libraries.hero:13-15`, the
note's `link` line, § 5's precedents. 4. Routes: add (F), (B-ELF), (B-asym),
(G), (H), (J); amend (B) with M1 to M3; give (D) libpsx's answer; give (E) Q4's
shapes and `design.md:2639-2642`. 5. Hand M1 to M4 with their paths. 6. After
`cp -R`, `rm -f <copy>/.git`. 7. The box unreachable at 01:52: its reading owed
or stated unrun. 8. Each seat appends running notes to `S/198-<seat>/report.md`.
9. Compiler-engineer item 2: the compiler's own tests (`libraries.hero` `:333`,
`:372`, `:443`, `:467`, `:488`, `:524`), `run` narrowed to `sdl3`, `corpus` for
`examples/sdl`; C3's pointers. 10. Item 1: refusals other than `-Wl,` with
equal weight, position-dependent pairs, `-Xlinker`; `ubuntu:24.04` with SDL
from source and `SDL_RPATH` on. 11. Ffi item 3: drop `sdl3-b14` as the
reproducer; `ci-step2.sh` or a stand-in `.pc`; name the two programs. 12. Ffi
item 2: the default is RUNPATH, `--disable-new-dtags` is the word that moves;
Q5; cite or reopen the 2026-08-15 ruling. 13. Ffi item 1: `-Wl,-z,relro`,
`-Wl,-z,now`; no `-fuse-ld`. 14. Add Q7 and Q8.
