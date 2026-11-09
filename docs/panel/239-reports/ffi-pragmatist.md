# Panel 239, the ffi-pragmatist's report

Written as it went, in the seat's worktree `.claude/worktrees/239-ffi-pragmatist/` (detached at
`1c4bc5197`), every file under `build/239/`. Every number below has its command beside it or in the
script named.

**Reading order.** Sections were written as the runs came, so their numbers are by topic, not by
place: 1 the linkers in C, 2 the routes, 3 and 3a the build (v1 to v5), 4 the programs on this Mac,
5 the cost, 6 Linux arm64 (three parts), 7 the repairs, 8 the reach, 9 2364 in C, 10 found beside,
11 Windows, 12 landing, cases and text, 13 2364's routes, then the verdict. **The route recommended
is v5** (`build/239/route/selfhost/`, `sources 0143800228018d3a`); the stop at about 01:07 cut v4's
first build, which was rerun.

## 0. The compilers

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0
  (`build/239/stage1.log`).
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next` exit 0, **725,905,816,201
  instructions retired** (`/usr/bin/time -l`, `build/239/stage2.log`); `./heroes-next --version`:
  `heroes 0.6.0`, **`sources 3d44313d62fb67a3`**, the brief's key.

## 1. What each linker says of which library supplied a name (question 1, in C)

The binding C is `build/239/c/src/` (the critic's and the coordinator's sources, plus `myalloc.c`, an
allocator linked on purpose, `alloc_main.c`, `math_main.c`, `thr_main.c`), its libraries built by
`build/239/c/src/libs.sh`, every case linked by clang and run by `build/239/c/probe.sh`, on this Mac
(ld64 `ld-27106.1`, Apple clang 21.0.0, `probe-darwin.txt`), in `heroes-linux-arm64:latest` with
GNU ld 2.44 (`probe-linux.txt`) and with `-fuse-ld=lld`, Debian LLD 22.1.8 (`probe-linux-lld.txt`).
**clang accepted every compile**; every link but those marked linked.

| case | ld64 (Mac) | GNU ld 2.44 | lld 22.1.8 |
|---|---|---|---|
| `-la -lb` / `-lb -la`, separate members | `pick=1` / `pick=2` | `1` / `2` | `1` / `2` |
| shared, `-la -lb` / `-lb -la` | `1` / `2` | `1` / `2` | `1` / `2` |
| `liba.a` then shared `libb` / the reverse | `1` / `2` | `1` / `2` | `1` / `2` |
| one member defines `pick` and the needed `b_only`, `-la -lb2` / `-lb2 -la` | `2` / `2` (the name order decides) | **link refused**, `multiple definition of 'pick'` / `2` | **link refused**, `duplicate symbol: pick` / `2` |
| a weak `pick` in that member, `-la -lbw` / `-lbw -la` | `3` / `3` (the weak one, both orders) | `1` / `3` | `1` / `3` |
| inside `--start-group` (R3's words) | n/a | the same as bare, `1` / `2`, `-la -lb2` refused | the same |
| `puts` redefined in `libq.a`, no `-l` for `puts` | **Bus error 138** | **SIGSEGV 139** | **SIGSEGV 139** |
| `printf` redefined in `libr.a` | prints nothing, exit 0 | the same | the same |
| an allocator (`malloc` and three more) in `libmyalloc`, static and shared | `served=1`, exit 0 | the same | the same |
| `ldexp` with `-lm`, with `-lc -lm` | `4 16` | `4 16` | `4 16` |
| `pthread_create` with `-lpthread`, without | `7` | `7` | `7` |

**What each linker can name, in the same link** (the same runs):

- **GNU ld**, `-Wl,--trace-symbol=<name>`, on stderr: one `<file>: definition of <name>` line for
  the definition it took (`liba.a(pick_a.o)`, `/lib/aarch64-linux-gnu/libm.so.6`); **a shared
  library after the first that defines the name is printed as `<file>: reference to <name>`**
  (`so -la -lb`: `liba.so: definition of pick`, `libb.so: reference to pick`), a word it also uses
  for a library that only calls the name, so the line alone cannot say which; **an archive not
  loaded is never named** (`liba.a then libb shared` names no `libb` member, `libb shared then
  liba.a` no `liba` member). `-lm` gives `libm.so.6: definition of ldexp`, `-lc -lm` gives
  `libc.so.6: definition of ldexp`; `-lpthread` gives `libc.so.6: definition of pthread_create`.
  `-Map` lists each archive member loaded and the reference that pulled it (`probe-linux.txt`'s
  `map:` lines).
- **lld**, the same option, on stdout (the critic's reading): `definition of` for an object or an
  archive member, **`shared definition of` for every shared library that defines the name, taken
  or not** (`so -la -lb`: both `liba.so` and `libb.so`; `-lm`: `libm.so.6` and `libc.so.6` both,
  for `ldexp`); no `lazy definition` line for an archive member it did not load (`-la -lb`: one
  line).
- **ld64** has no trace of a symbol (`-Wl,-y,_main`: `ld: unknown options: -y`). **Its map names
  the file of every symbol it places or imports**: `[  2] _pick` for an archive's member, and for a
  dylib the stub and the GOT entry, `0x...  [  2] _pick.stub` with `[  2] .../libb.dylib` in its
  object list (`build/239/c/out-Darwin/map-so.txt`), `_ldexp.stub [  5] .../system/libsystem_m.tbd`
  for `-lm`, `_pthread_create.stub` from `system/libsystem_pthread.tbd` for `-lpthread`: the
  re-exported library that holds the name, never `libm.tbd` or `libSystem.tbd`. `-why_load` names
  each archive member it loaded and the symbol that did (`'_pick' caused load of liba.a[2](pick_a.o)`).
  **`-t` lists members it did not take**: in `-la -lb` it lists `libb.a(pick_b.o)` beside
  `liba.a(pick_a.o)` though `-why_load` and the map say `pick_a.o` alone was loaded, in an order that
  moved between runs; ld-prime parses candidates in parallel. Undocumented (`man ld`: *"Logs each
  file ... the linker loads"*), so nothing below rests on it.
- **No linker names an archive member it did not load**, as the brief says; I found no option that
  does on any of the three (`man ld` on this Mac; `ld --help` and `ld.lld --help` in the container,
  grepped for `trace`, `why`, `lazy`, `map`). What reads the definitions an archive did not load is
  another link that loads them: route (D), or the same link with the libraries in the other order.

**Route (D), the whole-archive probe** (`build/239/c/repair.sh`, section D, both platforms): it
refuses `dup` in both orders, naming both members (ld64 `duplicate symbol '_pick' in:`, GNU ld
`multiple definition of 'pick'; ... first defined here`), and **links `interpose` (and then dies,
SIGSEGV on Linux) and `liba.a` beside a shared `libb`**: it sees no shared library and no C
library name, as the critic said. It did not refuse Debian's whole `libsqlite3.a` or `libz.a`
linked alone (both linked and ran). So (D) closes the archive half only.

## 2. The routes, read against those runs (question 1)

What each route does with each shape, read from the C runs above: routes (A) to (E) are not built,
and (G) is (section 3). "Refuses" is the robust answer for
every shape but `remath`, `pthread`, the allocator and `dup` with one library, which are correct
programs.

| shape | (A) taken, against the group's library | (B) every definer on the line | (C) order the line | (D) whole archives | (E) group probe, then (A) | **(G) mine** |
|---|---|---|---|---|---|---|
| `dup_written` / `dup_sorted` | accepts / refuses: **the order decides** | refuses both | `-la` first; both build `1` | refuses both (archives) | as (A) | refuses both |
| `dup` shared | as above | refuses both (lld names both; GNU ld `reference to`, ambiguous; ld64 names one) | as above | **accepts both** | as (A) | refuses both |
| `sig_*` | as `dup` | refuses | builds both | refuses (archives) | as (A) | refuses both |
| `one_*` (ld64 decides by name) | refuses both on ld64, GNU ld refuses one link | refuses | **no order exists on ld64** | refuses | as (A) | refuses both |
| `weak_*` (ld64 takes the weak one both ways) | refuses both on ld64 | refuses | **no order on ld64** | **accepts both** (`-all_load`: the strong one, `pick=1`, where the real ld64 link takes the weak one, `3`; this Mac) | as (A) | refuses both |
| `interpose` (`puts`, group with no `link`) | refuses only if "the C library" is defined | refuses only with the C library read | needs `-lc` placed first; unrun | **accepts** (the C library is shared) | refuses | refuses |
| `remath` on this Mac (`link "m"`, taken from `libsystem_m`) | **refuses** unless re-exports are resolved | accepts if the C library is not read | accepts | accepts | accepts | accepts |
| `pthread_create`, `link "pthread"`, glibc (taken from `libc.so.6`, `libpthread.a` empty) | **refuses** | accepts if the C library is not read | accepts | accepts | accepts | accepts |
| `ldexp`, `link "m"`, glibc (both define it) | accepts | **refuses if the C library is read** | accepts | accepts | accepts | accepts unless the program names `-lc` |
| the allocator, its group binding `myalloc_served` | accepts | accepts | accepts | accepts | accepts | accepts |

**(A) alone** is ruled out by the criterion, as the brief says, and worse where *the group's library*
is read as the file `-l<name>` opens: on this Mac ld64's map gives `libsystem_m.tbd` for `-lm`'s
names, never `libm.tbd`, so every `link "m"` binding the linker resolves would be refused (7 of the
25 tracked roots bind `math.h` with `link "m"`, `build/239/roots.txt` grepped), and a `link
"pthread"` binding on both platforms (`libc.so.6`, `libsystem_pthread.tbd`), all correct programs.
The tree's 4 `link "pthread"` lines are in panel 178's work files, refused by the checker before any
link (`old_cell_symbol`, base and route alike, `hero-runs/pthread/`).
**(B)** needs a symbol reader the compiler does not start (`nm`, a third tool on Windows) and the
`-l` search re-done by hand for archives; reading the C library makes it refuse glibc's six pairs.
**(C)** cannot exist on ld64 for `one_*` and `weak_*` (both orders take `libb2`'s, `libbw`'s), and
needs (B)'s knowledge to choose. **(D)** sees archives alone; it closes neither `interpose` nor a
shared duplicate, the two that crash or mislead in the field. **(E)** inherits (A)'s order.

**(G), built** (section 3): (A)'s question asked of the linker, joined to (C)'s other order as a
probe rather than a choice, with the class narrowed to the libraries the program names:

1. **Who supplied each bound name** is asked of a probe link of the real line's words: GNU ld and
   lld by `-Wl,--trace-symbol=<name>` for each bound C function (the first regular definition,
   else the first shared one), ld64 by `-Wl,-map,<file>` (the file index of `_<name>`,
   `_<name>.stub` or `_<name>.got`).
2. **A name is refused** where its supplier is a library the program names (a `-l` word of a group
   or a package) that the name's own group does not name, and where a group with no library (the
   C library's) gets it from any library the program names. A file the program names nowhere,
   the platform's own, is never judged: that is why `remath`, `pthread_create` and glibc's pairs
   pass, with no list of the platform's libraries and no reading of re-exports.
3. **The order**: where the program names two or more libraries, a second probe with their `-l`
   words reversed in place (every other word where it stood, R3's group words included). Any name
   two of the libraries define reaches the other one in one of the two orders, so the verdict is
   the same for every order of the `use` lines; and a probe the linker refuses on a duplicate of a
   bound name (GNU ld and lld on `one_*`) is that order's answer (route F).
4. The real link is untouched: same words, same `binary_home` key, nothing of the probes printed.

## 3. The refusal, built (item 3)

**The change**, as v5 stands (`build/239/route/diff/route.diff`, `diff -ruN selfhost build/239/route/selfhost`,
835 lines; every file `heroes-next fmt --in-place`d and `heroes-next check
build/239/route/selfhost/main.hero` silent):

- `selfhost/cli/supplied.hero`, new, 766 lines (`wc -l`), **479 code lines outside its tests** (the
  `layout` suite's unit, read by `awk`, not by the suite): past §11's ~300, so a landing splits it
  at its seam, the probe and its reading (`bindings`, `probe`, `taken`, `mapped`, `judged`,
  `refusal`, `platform_judged`) against the words (`told`, `diagnostic`, `bound_by`, `linked_by`,
  `the_rule`). Five tests (v5; four in v4, all passed, `route/test.log`) (the library file `-l<name>` reads, each linker's words read, the judgement, the reversed
  line and the duplicates read).
- `selfhost/cli/assemble.hero`, +14 lines: `linked_round` asks `supplied.refusal` over
  `link.link_line(...)`'s words before `link.link_objects`, and a refusal is the round's verdict,
  so a refused build publishes no binary.
- `selfhost/cli/whose.hero`, +7 lines: a refused link (`clang_link`) is read by `supplied.told`
  first, on the binding's line, exit 1.

**The code** proposed, `ffi_defined_elsewhere` (a new code: `ffi_defined_twice` names one C name
two of the program's own units define, before any link, and its notes say so; this one is a
library's definition reaching a binding, and its repair is in the library). The message, as v4
prints it (section 4 has every program's):

```
error[ffi_defined_elsewhere]: `pick`, bound here from `a.h` with `link "a"`, is defined by two
libraries this program links, and the order of the libraries decides which one a call reaches:
`liba.a(pick_a.o)` in one order, `libb.a(pick_b.o)` in the other
  at hero/picker.hero:2:5
  note: the linker found a definition in `<path>/liba.a(pick_a.o)`
  note: the linker found a definition in `<path>/libb.a(pick_b.o)`
  note: `-lb` is linked for the group of `b.h`, `link "b"`
  note: a linked program holds one definition of `pick`, taken from the first library on the link
        line that defines it, and the order of that line is not the program's to write: link one
        library that defines `pick`, or make the other's local where that library is built (over an
        archive `objcopy --localize-symbol=pick`, or `nmedit -R` on macOS; for a shared library
        `-fvisibility=hidden` or a version script)
```

No `Fix`: the repair is a library rebuilt or a `link` removed, which no edit of the binding's line
writes (panel 237's `shadowed_header` precedent). Told on the binding's line (`picker.hero:2:5`),
the one the author wrote and the one whose meaning changed.

**The compilers**: v1 `795cecee3f7fcc1c` (first message), v2 `2ed0fc295e542aea` (the message true
of each order), v3 `0e126475511797f6` (ld64's map read a byte at a time), v4 (the message the same
in both orders, a probe refused on a duplicate read as that order's answer), each built by this
seat's stage 2 (`build/239/route/build*.log`, `/usr/bin/time -l`: v1 735,657,534,773, v2
587,443,307,013, v3 585,397,101,678, v4 584,713,703,562 instructions retired; v4 `sources
3b0593b72037dfef`, the same key built in the container from the same sources). What moved the
verdict: v3 asks no probe on Windows (`env` is not there, and the route is unrun); v4 reads a probe
the linker refuses on a duplicate of a bound name as that order's answer, where v1 to v3 let the real
link fail (GNU ld's `one_written`). On this Mac v1 and v4 refuse and accept the same programs.

### 3a. v5, after Linux's false refusal (section 6)

**A binding whose group names no library is refused only where the platform alone defines its name
too, or where the order decides.** Where such a binding is refused, a third probe links the
program's objects with no `-l` word and no framework of the program's and unresolved names allowed
(`-Wl,--unresolved-symbols=ignore-all` on GNU ld and lld, `-Wl,-undefined,dynamic_lookup` on ld64,
measured in C: `puts` from `libc.so.6` or `libsystem_c`, `fabs` and `q_only` from nothing, ld64's
map giving `[  0] linker synthesized` for the unresolved one). Where the platform alone gives
nothing and both orders took the same file, the program reaches one definition and is accepted
(`fabs` in `libm.so.6`); where the platform gives one, the library's is a second, interposed, and
the refusal stands (`puts`). The probe runs only on that path, so it costs a program with no such
binding nothing. `cli/supplied.hero` gains `platform_judged`, `platform_only`, `without_decl` and a
fifth test (*"the platform's line keeps no library of the program's"*).

## 5. Cost (instructions retired, `/usr/bin/time -l`, warm, this Mac; `build/239/cost-darwin.txt`)

**The instrument counts the compiler's own process and not the processes it starts**, measured:
`/usr/bin/time -l clang -c` read 76,718,423, and three such clangs under one `sh` read 27,909,242.
So the probes' links are counted apart, on the linker itself.

| program (libraries it names) | base | route v1, v2 (map read by characters) | **route v3** | the probes' links, apart |
|---|---|---|---|---|
| `hello` (none) | 682.5M, 676.2M, 677.5M | 677.3M, 676.9M, 677.9M | **676.8M, 675.5M, 675.4M** (no probe) | none |
| `alloc_main` (`myalloc`, one) | 795.4M, 797.6M, 798.0M | 1,226.7M, 1,229.9M, 1,230.4M | **840.2M, 840.3M, 837.7M** (+43M, +5.4%) | one |
| `dep_written` (`top`, `base`, two, accepted) | 1,683.0M, 1,687.3M, 1,681.9M | 2,551.3M, 2,554.8M, 2,564.3M | **1,771.9M, 1,768.0M, 1,769.1M** (+86M, +5.1%) | two |
| `sig_written` (`a`, `w`, refused) | 1,681.8M, 1,684.3M, 1,679.9M (builds) | | **1,755.7M, 1,751.9M, 1,753.4M** (+72M, +4.2%; no real link runs) | two |
| **v5**, the same four (`route5` rows of `cost-darwin.txt`) | | | `hello` 676.1M, 677.4M, 676.7M; `alloc_main` 839.6M, 839.5M, 840.7M; `dep_written` 1,771.1M, 1,769.5M, 1,771.2M; `sig_written` 1,761.6M, 1,757.7M, 1,756.7M | the platform probe runs on none of them |

**One probe link of `dep`, measured on its own** (its `link-words.rsp` run by `clang -###`'s ld
command under `/usr/bin/time -l`): ld64 364,362,686 to 370,746,633 without the map, 368,708,662
to 372,846,949 with it; the clang driver around it 78,503,218 to 79,400,127. So **a probe costs
about 0.45G instructions on this Mac, the same as the link every warm build already runs**: a
program naming one library pays one more link, two or more libraries two more, none nothing. The
cheaper shape, unbuilt: the forward probe folded into the real link (its trace read and taken out
of what `run_link` prints, the map written outside the stage that is published), one extra link
instead of two. Linux arm64's cost in instructions: unmeasured (no counter there; see section 6).

## 4. Every program, both orders, under the route (item 3)

The 2363 programs at the Heroes level, `build/239/hero/` (the brief's `link/hero/`, `s2363/hero/`,
the critic's `s2363x/hero/`, and three of mine: `weak_written`/`weak_sorted`, `picker` beside
`extraw.hero` linking `libbw.a`, whose `b_only` member holds a weak `pick`; `alloc_main`, a group
binding only `myalloc_served` from `libmyalloc`, which replaces `malloc`, `calloc`, `realloc` and
`free` on purpose; `alloc_bound`, the same group binding `malloc` too), run by `build/239/hero.sh`
against `build/239/c/out-<platform>/`. *Written* is the brief's order, *sorted* the swap.

**This Mac** (base `hero-base-darwin.txt`, route v4 `hero-route4-darwin.txt`):

| program | base | route v4 |
|---|---|---|
| `dup_written` / `dup_sorted`, static | `1 10 20` / **`2 10 20`** | **exit 1 / exit 1**, the same message byte for byte (`cmp`) |
| the same, shared | `1 10 20` / **`2 10 20`** | **exit 1 / exit 1**, byte-identical |
| `sig_written` / `sig_sorted` | `1 30` / **Bus error 138** | **exit 1 / exit 1**, byte-identical |
| `one_written` / `one_sorted` | **`2 20` / `2 20`** (`libb2`'s, by the names) | **exit 1 / exit 1**, byte-identical, *"reaches `libb2.a(b2.o)` in either order"* |
| `weak_written` / `weak_sorted` | **`3 20` / `3 20`** (the weak one) | **exit 1 / exit 1**, byte-identical |
| `interpose` | **Bus error 138** | **exit 1**, on `printer.hero:2:5`, *"`puts`, bound here from `stdio.h` with no `link`, so the C library's, reaches `libq.a(puts_q.o)`"* |
| `remath` (`link "m"`, `sqrt`) | `4.0` | `4.0` |
| `dep_written` / `dep_sorted` | `41 2` / `41 2` | `41 2` / `41 2` |
| `only_top` | exit 2, `internal error` (2365) | the same (not this class) |
| `runtime_name` (`printf` in `libr`) | two empty lines | **the same: not judged** (section 7) |
| `alloc_main`, static and shared; `alloc_bound` | `false`, exit 0 | the same: accepted |

**The 25 tracked roots that reach a `link` or `package` group** (`build/239/roots.txt`: every file
of `examples/` and `tests/golden/run/` with an `extern ... link|package` line and a `main`, plus
`examples/ledger/main.hero`, whose `db/sqlite.hero` binds SQLite; SQLite, libm, libcurl, OpenSSL's
libcrypto, zlib, libuv, raylib, SDL3, libjpeg through `package`): **every one builds and runs under
v1 and v4 as under base**, `diff` of the verdict lines silent (`runs-base-darwin.txt`,
`runs-route-darwin.txt`, `runs-route4-darwin.txt`), the whole outputs identical by `cmp` but one,
a heap address in a panic line (`fixedbugs-2291-a-refused-close-nobody-repeats-is-owed-at-exit`,
`0x10105f0a0` against `0x1049df0a0`, ASLR). **Zero false refusals of tracked programs on this Mac.**

## 7. What a program that needs both libraries does (item 4, question 2)

`build/239/c/repair.sh`, both platforms, `repair-darwin.txt` and `repair-linux.txt`; the author
needs `liba`'s `pick` and `libb`'s `b_only`:

| repair, where the other library is built | this Mac | Linux arm64 |
|---|---|---|
| `libb`'s `pick` made local in the archive: `nmedit -R` (cctools, `/usr/bin/nmedit`) on this Mac, `objcopy --localize-symbol=pick` on Linux | `pick=1 10 20` in both orders | the same |
| the same where `pick` shares its member with the needed `b_only` (`libb2`) | `pick=1 20` both orders | the same |
| `libb` shared, its `pick` built `__attribute__((visibility("hidden")))` | `pick=1 10 20` both orders | the same |
| a wrapper of the program's own: none can rename a symbol inside another library in a static link; one name, one definition | n/a | n/a |

`objcopy` is not on this Mac (`command -v objcopy llvm-objcopy`, `xcrun -f`: none) and `nmedit` is not
in the Linux image (`command -v nmedit`: none; `objcopy` and `llvm-objcopy` are), so the note names both. **A Heroes author reaches none of these
through the language**: they are steps where the library is built, which §1.11's thin shim (a `.c`
file of the program's own, design.md:599) cannot replace, since the shim links into the same one
namespace. After R2 no `use` order is a lever, and the route makes the order irrelevant anyway: the
note says the line's order is not the program's to write. The one route inside Heroes is not to link
the second library, which is what *"link one library that defines `pick`"* says first.

## 8. 2363's reach (question 3)

- **A name no group binds that two linked libraries define and a third needs** (`intern_written`,
  `intern_sorted`: `libtop`'s `top_value` calls `base_helper`, which `libbase` defines as 40 and a new
  `libother` as 99; `build/239/hero/`): **this Mac prints `100 2 7` in both orders**, ld64 taking
  `libother`'s silently, under base and the route alike. **Not reached by my build**: the probe
  traces bound names only. Reachable on GNU ld and lld by comparing the archive members the two
  probes load (`-Map`'s *Archive member included to satisfy reference by file (symbol)*); on ld64
  both orders agree, so it needs every library's definitions, route (B)'s reader. A question for
  later, said why: no group of the program names `base_helper`, so there is no line to tell it on
  but the library's, and no measured program of the tree has it.
- **A name the runtime calls that a linked library defines** (`runtime_name`, `printf` in `libr`):
  measured silent on both platforms under base, and **not judged by my build**. Reachable by the
  same probe: ld64's map already names every symbol's file (`[  3] _printf` from
  `libr.a(printf_r.o)`, `probe-darwin.txt`), GNU ld's `-Map` names the referrer of an archive
  member (`printf_main-*.o (printf)` against `libr.a(printf_r.o)`, `probe-linux.txt`), and a shared
  redefinition needs the runtime's names traced: **142 undefined names** in this Mac's runtime
  object (`nm -u`, `build/239/runtime-undefined-darwin.txt`), a list the compiler would hold as a
  premise with a test that fires when it dies (`.claude/rules/module-shape.md`).
- **How it tells an allocator linked on purpose from `printf_r`**: by the author's own word, with no
  new form. `alloc_main` (a group binding only `myalloc_served`) and `alloc_bound` (binding `malloc`
  too) both build and run under the route (`false` on this Mac, `true` on Linux arm64 where the
  runtime's `malloc` reaches the counter; on this Mac the binary defines `_malloc` itself, `nm -m`,
  and the counter still read 0: why is unmeasured). The rule I would land with the runtime's names:
  a runtime name a library of the program supplies is refused unless a group binds that name with
  that library, so `alloc_bound`'s `function malloc(n: u64) -> ptr` under `link "myalloc"` is the
  author saying it, clang holding the prototype to the allocator's header, and `runtime_name`, whose
  `loud` binds no `printf`, is refused.
- **A name the runtime's object defines, which a library's loaded member defines too**
  (`hero_name`: `libhh.a`'s one member defines `h_only` and `hero_print_end`): **exit 2,
  `internal error: linking failed:`, ld64 `duplicate symbol '_hero_print_end'`, under base and the
  route alike** (this Mac; Linux in section 6). Every one of the runtime's 244 defined names
  carries the `hero_` prefix (`nm -g --defined-only`), so only a library that spells that prefix
  meets it. Found beside, section 10.
- **A name a library defines and the program's own unit defines too** (a header's external
  definition): `ffi_defined_twice` covers two units; a unit's definition against a library's is
  read by the route like any supplier: the unit's object is not a library the program names, so it
  is never refused, and the linker takes the object's over an archive's (unrun here as a program).
- **A `.so` whose own dependency defines the name** (`build/239/c/dep_so.sh`, Linux arm64:
  `libc2.so` needs `libdep.so`, which defines `pick`): `pick=1 6` in both orders and on both GNU ld
  and lld; GNU ld names `libdep.so` only as `reference to pick`, lld not at all, so the probe reads
  `liba.so` and accepts. Correct.
- **A library two modules both name** (the tree: `link "sqlite3"` in `examples/ledger/db/sqlite.hero`
  and others): its name is the group's own in each, never another group's; the 25 roots include
  SQLite and zlib programs and none moved.

## 9. 2364 in C (item 5)

- **`pack` as two C units** (`build/239/c2364/maker.c`, `reader.c`: `maker.c` reads `pk.h` then
  `rp.h` and returns an `Rp`; `reader.c` reads `rp.h` alone): `clang -std=c11 -Wall -Wextra
  -pedantic` **silent**, prints `4 8 0 sizes 5 8` at exit 0; the same under `-flto` and under
  `-fsanitize=address,undefined`. C11 6.2.7p1 makes the two `struct Rp` compatible (same tag, same
  members, same types, same order); `#pragma pack` is an extension the standard's rule never sees,
  so **nothing in C, the compiler, the linker or the sanitizers, compares a record's layout across
  units**. Only the Heroes compiler can, which is question 4's (B) or (D).
- **A real pair where the order search would choose a meaning** (`build/239/c2364/jpeg/run.sh`,
  this Mac, jpeg-turbo 3.2.1 by `pkg-config --modversion libjpeg`): another module's header defining
  `HAVE_BOOLEAN` and `typedef unsigned char boolean;`, as Windows' `rpcndr.h` does. **Only one order
  compiles**: that header first builds, `sizeof(struct jpeg_decompress_struct)` **624** where the
  library was built at **656**, and runs to `JPEG parameter struct mismatch: library thinks size is
  656, caller expects 624`, exit 1, libjpeg's own guard and no compiler's; `jpeglib.h` first is
  refused by clang, *typedef redefinition with different types ('unsigned char' vs 'int')*. So
  **question 4's route (A), the order search, chooses the wrong ABI here**: the order that compiles
  is the one that changes what every bound libjpeg function reads; a library without libjpeg's
  `structsize` check would read past the caller's struct. GLFW 3.5.1 is installed and unrun; readline
  is not installed (`pkg-config`); Linux arm64 has no libjpeg (unrun there).

## 11. Windows, `lld-link` (unrun)

The box is off and only the author starts it (`.claude/rules/platforms.md:98-101`); nothing here ran
there, and v3 onward asks no probe on Windows (`process.exe_suffix() == ".exe"`, or an MSVC triple):
the class stays open on that platform under this build. What it would need, as questions: ELF lld,
`lld-link`'s sibling, names every shared definer (`shared definition of`) and no unloaded archive
member (section 1), so the reversed probe is owed there as everywhere. `lld-link` has no
`--trace-symbol` that I know of (a question, not searched on a Windows machine); its `/VERBOSE`
prints which archive member it loaded for which symbol, and an MSVC-style `/MAP` lists each public
symbol with its `lib:object`, an import as `__imp_<name>` from the import library: either would be
read as ld64's map is. Whether `lld-link` refuses or takes the first of two import libraries that
export one name is unmeasured.

## 12. The order of landing, the cases, the text (questions 6, 7, 8)

- **Order of landing.** The route depends on no other change: its verdict is the same for both
  orders of the libraries, so it stands before R2's key, after it, or without it, and the key
  changes nothing it decides (on ld64 too: `one_*` and `weak_*` are refused in both orders because
  the supplier is another group's in both). With R3's group words (`group.diff`, applied to a copy
  of v4, `build/239/routeg/`) the reversal moves the `-l` words inside the group and leaves the group
  words where they stood (the test *"the reversed line moves the -l words alone"*); in C a group
  changes no binding (section 1: `group -la -lb` gives `1`, `group -lb -la` gives `2`, `group -la
  -lb2` is refused as bare). **Measured with the built compiler**, v4 with `group.diff` (`build/239/routeg/`,
  `sources 589cef5c7a0ff7c8`, Linux arm64, `hero-routeg-linux.txt`): every member of the class refused
  at exit 1 in both orders, the four pairs' messages byte-identical, a line carrying the group words
  (`link-words.rsp`), and `dep_sorted`, exit 2 under base and v4, **linking at `41 2`**: the two
  repairs meet without either moving the other's verdict. v5 with the words: section 6.
- **The cases.** Built: four `test` blocks in `cli/supplied.hero` (each linker's words read, the
  library file `-l<name>` reads, the judgement, the reversal and the duplicates). Owed, and not
  buildable as a golden case today, since no golden case builds a C library: one harness check that
  builds two shared libraries with clang alone (`-dynamiclib`, `-shared`; no `ar`, which Windows
  lacks as a word) defining one name, a third with a `puts`, and builds `dup_written`,
  `dup_sorted` and `interpose`, asserting exit 1, `ffi_defined_elsewhere` on the binding's line and
  the two orders' messages equal byte for byte; it runs on this Mac and the CI's Linux legs, and on
  Windows where a DLL is built (`tests/harness/shell.hero:2332` builds one already). The archive
  shapes (`one_*`, `weak_*`, `sig_*`) need `ar` and are Mac and Linux only.
- **The text.** A spec sentence is owed, and § 13 is where: *"a C name a group binds is taken from
  its group's library, or from no library the program names where the group names none; a library
  of the program that defines it again is refused, whatever order the program's modules stand in"*.
  Its size is the spec-warden's to measure (`heroes measure`, vendored tables only); unrun here.
  design.md §4.19 gains the rule beside its link paragraphs, and `.claude/rules/c-boundary.md`'s
  member list gains `ffi_defined_elsewhere`.

## 6. Linux arm64 (`heroes-linux-arm64:latest`, GNU ld 2.44, Debian clang 22.1.8)

Built in the container from `git ls-files seed runtime selfhost examples tests/golden/run`
(`build/239/linux/tree.tar`, `COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs`, 2,872 files,
none under `.claude`), `build/239/` mounted alone, `TMPDIR` under it: base `sources
3d44313d62fb67a3` (`linux/build.out`), route v4 `3b0593b72037dfef`, the same keys as this Mac's.

**The 2363 programs** (`hero-base-linux.txt`, `hero-route4-linux.txt`):

| program | base | route v4 |
|---|---|---|
| `dup_written` / `dup_sorted`, static and shared | `1 10 20` / **`2 10 20`** | **exit 1 / exit 1**, messages byte-identical (`cmp`) |
| `sig_written` / `sig_sorted` | `1 30` / **SIGSEGV 139** | **exit 1 / exit 1**, byte-identical |
| `one_written` / `one_sorted` | **exit 2 `internal error`** (`multiple definition`) / **`2 20`** | **exit 1 / exit 1**, byte-identical: *"reaches `libb2.a(b2.o)` in one order of the libraries, and in the other the linker refuses two definitions of it"* |
| `weak_written` / `weak_sorted` | `1 20` / **`3 20`** | **exit 1 / exit 1**, byte-identical |
| `interpose` | **SIGSEGV 139** | **exit 1** |
| `remath`, `alloc_main` (static, shared), `alloc_bound` | `4.0`, `true` | the same |
| `dep_written` / `dep_sorted` | `41 2` / exit 2 (2365's, the group words' to close) | the same |
| `hero_name` | exit 2 `internal error` | the same (section 10) |
| `intern_written` / `intern_sorted` | **exit 2 `internal error`** / **`100 2 7`** | the same (section 8) |

**The 25 roots** (`runs-base-linux.txt`, `runs-route4-linux.txt`): base builds 17 and refuses 8 for
packages the image lacks (`raylib` 5, `sdl3` 2, `libuv` 1: `error[ffi_package] ... not installed`).
**v4 refused one of the 17, falsely**: `tests/golden/run/fixedbugs-2056-a-header-s-own-macro-of-a-name-it-binds-stands.hero`,
*"`fabs`, bound here from `fixedbugs-2056-own.h` with no `link`, so the C library's, reaches
`libm.so.6`, a library this program links for ..."*. On this Mac the same program passes (its
`fabs` comes from `libsystem_m`, which no group names). On glibc `fabs` is defined by `libm.so.6`
alone (`nm -D --defined-only`: `libc.so.6` 0, `libm.so.6` 1), so the program reaches one
definition; my rule for a group with no library read `libm`, which another group of the program
links, as another group's library, where on glibc it is the C library's second file. **This is
v5's repair** (section 3a), measured below.

## 10. Found beside, for the coordinator to file (this seat files nothing)

Each with its reproducer under `build/239/`, each older than this sitting (base compiler, `sources
3d44313d62fb67a3`), each run on both platforms:

1. **A library whose loaded member defines a name the runtime defines is told `internal error`,
   exit 2.** `hero/hero_name.hero` (`use hh_lib`; `hh_lib.hero` links `"hh"` for `h_only`), with
   `libhh.a` from `c/src/libs.sh` (`c/src/hh.c`: one member defining `h_only` and `void
   hero_print_end(void)`): `BUILD exit 2: internal error: linking failed:`, ld64 `duplicate symbol
   '_hero_print_end'` (this Mac), GNU ld the same exit 2 (Linux arm64), under base and my route.
   The author's library collides with the compiler's runtime and the compiler blames itself. Class
   by `.claude/rules/verification.md` § Bounded discovery: **`blocking`** (an exit 2 where the
   author can be told). Its repair is a reading of the refused link on the group that links the
   library, as `supplied.told` reads a bound name's duplicate.
2. **A name no group binds that two linked libraries define and a third needs.**
   `hero/intern_written.hero` / `intern_sorted.hero` (`top_lib`, `base_lib`, and `other_lib` linking
   `"other"`, whose one member defines `other_only` and `base_helper` returning 99): this Mac
   **`100 2 7` in both orders**, `libtop` reaching `libother`'s `base_helper`, silent; Linux arm64
   **exit 2 `internal error`** (`multiple definition`) in the written order and **`100 2 7`** in the
   other. The exit 2 is 2365's failure path met by a duplicate (**`blocking`**, or a widening of
   2365); the silent `100` is 2363's reach (section 8), a question for later, since no line of the
   program says which `base_helper` `libtop` wants.
3. **Question 4's order search chooses the wrong ABI on a real library** (section 9, libjpeg's
   `boolean`): not a defect of the tree, a finding for this sitting's 2364 route.

## 13. 2364's routes, from the ABI side (question 4)

- **(A), the order search, alone: I veto it.** The libjpeg pair (section 9) is a real library where
  the one order of two headers that compiles is the wrong ABI: `jpeg_decompress_struct` read at 624
  bytes against the library's 656. Asked of another module's header and libjpeg's, the search would
  choose that order and build; the owner's module, reading `jpeglib.h` alone, reads 656; the record
  crossing between the two units is `pack/pk_first`'s silent wrong value, chosen by the compiler.
  design.md §4.19 says of the header list that *"a change to it changes what programs mean"*, and
  this is that change on a library the tree binds (`package "libjpeg"`, 16 lines).
- **(B), each group record a unit spells compared with its owner's unit's layout** (size,
  alignment, every member's offset and type): the one route that sees `pack`, `pragma`, `partial`
  and the libjpeg pair, since C itself compares nothing across units (section 9: `-Wall -Wextra
  -pedantic`, LTO and ASan+UBSan all silent on two layouts of one `struct`). Its cost and its build
  are the compiler-engineer's; from this seat, (A) is admissible only joined to (B) in one push, so
  any order it picks is held to the owner's layout.
- **(D), a unit that reads another module's record with no header the owner's unit does not read**:
  ABI-safe by construction (the record is read as its owner reads it), and its cost to bindings is
  the question: a header that needs another module's header to compile (`jpeglib.h` after
  `stdio.h`, design.md §4.19) must find it in the owner's own groups, which is where §4.19 already
  sends it. Unbuilt here.

## 6, continued. v5 on Linux arm64 (`sources 0143800228018d3a`, built in the container)

- **The 2363 programs** (`hero-route5-linux.txt`): `dup` (static and shared), `sig`, `one`, `weak`
  refused at exit 1 in both orders, every pair's whole message byte-identical (`cmp`); `interpose`
  refused at exit 1 (`puts` is `libc.so.6`'s too, so the platform probe keeps the refusal);
  `remath`, `alloc_main`, `alloc_bound`, `dep_written`, `intern_sorted` as base; `dep_sorted`,
  `only_top`, `hero_name`, `intern_written` exit 2 as base (sections 8 and 10, 2365).
- **The 25 roots** (`runs-route5-linux.txt`): **identical to base** (`diff` silent); the 17 that
  build give whole outputs identical by `cmp` but `fixedbugs-2291-a-refused-close...`'s heap address
  (`0xaaaaeeb0cc78` against `0xaaaaddb2cc78`); **`fixedbugs-2056` builds and prints `1000.0 2.0`**,
  its whole output identical. **Zero false refusals of tracked programs on Linux arm64 under v5**,
  and zero on this Mac (section 4, `runs-route5-darwin.txt`).
- **The tests**: five, all passed on this Mac (`route/test5.log`, 801,258,781,441 instructions
  retired); not run in the container (the compiler's whole test build there is unmeasured).

## 6, last. v5 with R3's group words (`build/239/routeg5/`, `sources 1edadba701d9a514`, Linux arm64)

`hero-routeg5-linux.txt`, `runs-routeg5-linux.txt`: every member of the class refused at exit 1 in
both orders, the four pairs' messages byte-identical; `dep_sorted` links at `41 2` (exit 2 under
base and v5 without the words); the 25 roots identical to base. **The glibc pair program**
(`hero/glibc_written.hero`, `glibc_sorted.hero`: `pthread_self` and `pthread_equal` under `link
"pthread"`, `ldexp` and `sqrt` under `link "m"`, two libraries so the reversed probe runs;
`linux/glibc.out`): **`1 16.0` in both orders under base, v5 and v5 with the words**. On this Mac
the same pair through `pthread_getconcurrency` (`hero/platform_*.hero`) prints `0 16.0` in both
orders under base and v5; glibc declares that function only under a feature macro, refused
there as `ffi_unknown_name` under base too, my binding's mistake. **On this Mac v5 with the group
words is unbuilt**: `group.diff`'s `groups_libraries` is false for an Apple triple (its test) and
panel 238 measured the Mac's `link-words.rsp` byte-identical with and without the words, so the
Mac line does not move: an inference, said so.

## Verdict on v5 (superseded by the verdict after section 14, at the end; kept as it stood)

- **`verdict`: veto**, on both of the sitting's provisional directions as `proposal.md` words them,
  each with its condition below; **approve** 2363's refusal in the form built here (v5,
  `ffi_defined_elsewhere`).
  - **2363, (A) alone** (*"the definition the link took, compared with the library the binding's
    group names"*): its verdict follows the order (`dup_written` accepted, `dup_sorted` refused,
    as the brief says), and where *the group's library* is read as the file `-l<name>` opens it
    refuses every resolved `link "m"` binding on this Mac (ld64's map: `libsystem_m.tbd`, never
    `libm.tbd`) and `link "pthread"` on both platforms: §4.19's rung 2, libm, refused, which is a
    binding made categorically harder.
  - **2364, (A) alone** (panel 204's order search over other modules' headers): on a library the
    tree binds, libjpeg, the one order that compiles is the wrong ABI (`jpeg_decompress_struct` 624
    bytes against the library's 656), so the search would choose a layout the library does not
    have. That breaks the C ABI at the boundary §1.12 defends.
- **`section`**: design.md §1.12 (*"A Heroes program must not segfault and must not corrupt
  memory"*; the boundary *"complete and defended"*), §1.11 (*"FFI ergonomics rank alongside
  comprehension"*: a route refusing `link "m"` fails that), §4.19 at design.md:606 (*"a wrong type
  in an `extern` is a *compile* error, which is this project's thesis applied to the boundary"*),
  and §4.19's header-order paragraph (*"a change to it changes what programs mean, so it is a
  sitting's"*). **The document does not cover the library side**: nothing in §4.19 or spec § 13
  says which library a bound name is taken from, as panel 238's search found and this seat's
  re-read of §4.19 confirms (`sed -n 2582,3080p docs/design.md`, grepped for `link`, `library`,
  `duplicate`, `archive`, `symbol`: no rule on it).
- **`experiment`**: the binding C in `build/239/c/` (`liba`, `libb`, `libw`, `libb2`, `libbw`,
  `libq`, `libr`, `libmyalloc`, `libtop`, `libbase`, `libhh`, `libother`, static and shared),
  every compile accepted by clang, linked by ld64, GNU ld 2.44 and lld 22.1.8 in both orders
  (sections 1, 7); the libjpeg pair and the `pack` pair in C (section 9); and the refusal built as
  a compiler change (`cli/supplied.hero`, `assemble.hero` +14, `whose.hero` +7), five tests passing,
  run on both platforms with and without R3's group words: the class refused at exit 1 in both
  orders with byte-identical messages, the 25 tracked roots and the platform's own pairs
  unchanged, zero false refusals of tracked programs under v5 on either platform.
- **cost or delta** (this Mac, instructions retired, warm): a program naming no library, none
  (`hello` 676M under base and v5); one library, the compiler's own process +43M (+5.4%) and one
  probe link, about 0.45G (clang's driver 79M, ld64 369M with the map), the size of the link a warm
  build already runs; two or more libraries, +86M (+5.1%) and two probe links; a refused build runs
  no real link. Linux arm64's cost in instructions is unmeasured (the image has no `perf`,
  `valgrind` or `/usr/bin/time`). The cheaper shape, unbuilt: the forward probe folded into the
  real link, one probe link instead of two.
- **`argument`** (119 words, `wc -w`): One C name has one definition in a linked program, and every linker
  measured takes it by the line's order or, on ld64, by the names; clang verifies the header and
  nothing verifies the library, so `sig_sorted` dies at exit 0 behind a verified prototype. Asking
  the linker who supplied each bound name, in both orders of the libraries the program names,
  refuses the whole class on both platforms with one message whatever the order, and judges no
  library the program does not name, so `libm`, `libpthread` and glibc's pairs stand untouched.
  The order search for 2364 is the opposite move: it lets the compiler choose an order, and on
  libjpeg the order that compiles is the wrong ABI.
- **`prediction`**: **SQLite, §4.19's rung 3, needs no shim and moves nowhere under v5**:
  `examples/sqlite/main.hero` and `examples/ledger/main.hero` build and print what base prints on
  the CI's Linux x86-64 leg and on `macos-26`, as measured on arm64 and this Mac; and on that x86-64
  leg `build/239/hero/sig_sorted.hero` is refused at exit 1, its message byte-identical to
  `sig_written`'s, and `tests/golden/run/fixedbugs-2056-...` builds. Scored by the landing lane's
  run of these programs on the CI's legs at the push that carries the refusal; any of them moving,
  or a tracked program refused there, falsifies it.
- **`condition`**: the 2363 veto lifts when what lands asks the supplier in both orders of the
  program's libraries (or by another reader whose verdict is free of the order), judges only the
  libraries the program names, reads a group with no library as interposition (the platform's
  definition and a library's both present) rather than as any named library, and its cases run on
  both platforms; v5 meets that, so I approve v5 and would move to **object** if the CI's x86-64 leg
  or the Windows box finds a tracked program it refuses, or if a measured probe costs a warm build
  more than one extra link per two libraries. The 2364 veto lifts when the order search lands only
  joined to the layout comparison (B), each spelled record held to its owner's unit's size,
  alignment and offsets in the same push, or when (D) replaces it; I would withdraw it if the
  libjpeg pair, built as Heroes modules, is refused or builds with the library's own layout under
  the landing compiler.

## 14. v6, after the critic's second pass

Asked by the coordinator after the critic's second pass (`p239/docs/panel/239-reports/critic-reports.md`,
V1, I3, C1, C3, C4, W2 and its closing list). The critic's `ab_*` and `self_*` copied from
`239-critic/build/239/p2/self/` into `build/239/hero/`, `libab.a` and `libbs.a` built by my
`c/src/libs.sh` from its `ab.c`, `b_self.c`, `pick_b.c`.

### 14.1 What v6 is (`build/239/route6/selfhost/`, `sources 30f949d859f52b96` on this Mac)

v5's readings, joined to route (D), with the forward probe folded into the real link and the
runtime's names judged:

1. **The real link is the forward reading** (`cli/link.hero`): its words gain `--trace-symbol` for
   every bound name and every name the runtime needs (GNU ld, lld) or `-map` written under `build/`
   (ld64, never published, removed after reading); `cli/link_warning.hero`'s `run_link` returns
   what the link said and prints it without the trace's lines; a refusal removes the private stage
   before anything is published.
2. **Where the program names two or more libraries, the reversed probe** (v5's), and **where that
   probe read a library file of objects the program names, route (D)**: one probe with every such
   file loaded whole (`-Wl,--whole-archive` around the `-l` words, `-Wl,-all_load` on ld64,
   unresolved names allowed), whose duplicates are read with both files (`supplied.duplicates`:
   GNU ld's `multiple definition ... first defined here`, lld's `>>> ... in archive`, ld64's
   indented paths). A duplicate between two libraries the program names, of a name a group binds
   or the runtime needs, is refused. **D is built, B is not**: D sees what no order loads (`ab`),
   and it costs no tool the compiler does not start; B needs a symbol reader (`nm`) the compiler has
   never started, and on its one remaining case, a shared library, the reversed probe already
   reads a bound name (section 6) and Apple's two-level namespace binds a dylib's own calls to
   itself.
3. **The runtime's names** (the critic's C4): asked once per runtime object, by linking it alone
   with `-nostdlib` and reading back every name the link finds undefined (`ffi_build.undefined_symbol`),
   cached under `build/` beside the object: on this Mac **142 names, exactly `nm -u`'s, and `main`**
   (`v6c/rt-needs-darwin.txt` against `runtime-undefined-darwin.txt`, `diff` one line, `main`), 75M
   instructions the first time. A name the runtime needs that a library of the program supplies is
   refused unless a group of that library binds it.
4. **The words** (`cli/supplied_words.hero`, W2): the specification's vocabulary (its spec-warden's
   § 9): a library by its `link` name and the module that links it, *the C library*, a file by its
   path, no *linker*, *link line*, *archive* or *member*, and no lever of `use` order or module name.

**A duplicate of a name no group binds and the runtime does not need is not refused, measured**
(`v6c/dprobe.sh` over real static libraries, `dprobe-darwin.txt`, `dprobe-linux.txt`): every
library file of a set loaded whole, unresolved names allowed. This Mac's Homebrew: **static FFmpeg,
40 names defined in two of `libavcodec.a`, `libavformat.a`, `libavutil.a`**
(`ff_se_golomb_vlc_code` in `libavformat.a` and `libavcodec.a`, `ff_init_float2half_tables` in
`libavutil.a` and `libavcodec.a`); **static X11, `xreallocarray` in `libX11.a` and `libXext.a`**;
`libjpeg.a` with `libturbojpeg.a`, 293 (the second bundles the first); OpenSSL, brotli, each library
alone: none. Debian's (`/usr/lib/aarch64-linux-gnu`): `libcurl.a`, `libssl.a`, `libcrypto.a`,
`libz.a`, `libsqlite3.a`, `libncurses.a`, alone and together: none. FFmpeg and X11 are correct
static programs whose libraries carry identical copies on purpose; C11 6.9p5's strict reading would
refuse them, and no symbol tells an identical copy from `intern`'s two different `base_helper`s. So
**`intern_*` stays a known silent shape** under v6 (section 8): told at exit 1 where the link itself
refuses it (GNU ld's written order), silent `100 2 7` in the other, and in both after R2's walk (I3).
The ruling between that and refusing static FFmpeg is the sitting's; this seat recommends recording
it, with this measurement as its reason.

### 14.2 Every program, both orders (this Mac, v6b `11a70e0569b6f6e2` and v6 `30f949d859f52b96`, the same verdicts)

`hero-route6b-darwin.txt`; every pair's whole message byte-identical across orders (`cmp`): `dup`
(static and shared), `sig`, `one`, `weak`, `ab`, `self`.

| program | base | v6 |
|---|---|---|
| `dup_*` (static, shared), `sig_*`, `one_*`, `weak_*` | as section 4 | exit 1 both orders |
| **`ab_written` / `ab_sorted`** (critic V1) | `1 10 10` / `1 10 10` | **exit 1 / exit 1** (D: `libab.a(ab.o)` and `libbs.a(pick_b.o)`) |
| `self_written` / `self_sorted` (critic C1) | `1 10` / `2 20` | exit 1 / exit 1 |
| `interpose` | Bus error 138 | exit 1 |
| **`runtime_name`** (`printf` in `libr`) | two empty lines | **exit 1**, on `loud.hero:2:5` |
| **`alloc_main`** (binds only `myalloc_served`), static and shared | `false` | **exit 1**: *defines `calloc`, `free`, `malloc`* |
| **`alloc_bound`** (binds `malloc` too) | `false` | **exit 1**: *defines `calloc`, `free`* |
| **`alloc_all`** (its group binds `malloc`, `calloc`, `realloc`, `free`) | | **`100 true`**, accepted |
| **`hero_name`** | exit 2 `internal error` | **exit 1**, on `hh_lib.hero:2:5` |
| `intern_written` / `intern_sorted` | `100 2 7` / `100 2 7` | the same (not refused, 14.1) |
| `remath`, `dep_*`, `platform_*` | `4.0`, `41 2`, `0 16.0` | the same |
| `only_top` | exit 2 (2365) | the same |

**The 25 roots under v6 on this Mac**: identical to base (`runs-route6c-darwin.txt`, `diff` silent).

**Why the allocator read `false` on this Mac** (C4): the runtime routes every block through one
`malloc` (`runtime/parts/alloc.c:470`, `hero_malloc_raw`), and `alloc_main` reads the counter before
it has allocated; on Linux glibc's own `malloc` calls (its stdio among them) already reached the
replacement, on macOS libSystem's own calls stay inside libSystem (the two-level namespace).
Measured: `alloc_more`, which builds a hundred strings first, prints `100` and **`true`** under base
on this Mac (`hero-runs/alloc-more/`, `nm -m`: `_malloc` and `_calloc` defined in the binary).

### 14.3 Cost, every link counted (C3; this Mac, `build/239/cost6/`)

**The instrument** (`cost6/cost6.sh`): a `clang` earlier on `PATH` (`cost6/wrap/clang`) records the
words of every clang the compiler starts and runs the real one; each recorded link (an `-o` and no
`-c`) is replayed with its output redirected, its driver counted by `/usr/bin/time -l clang ...` and
its `ld` by `/usr/bin/time -l` on the command `clang -###` prints; the compiler's own process by
`/usr/bin/time -l heroes build`. Warm: one build discarded, then four, each line a whole build
(`cost6-base.txt`, `cost6-v6c.txt`). Median of runs 2 to 4, instructions retired:

| program (what it names) | base: compiler + links = total | v6 `30f949d859f52b96`: compiler + links = total | delta |
|---|---|---|---|
| `hello` (no library) | 675.5M + 1 link (78.0M + 358.8M) = **1,111.97M** | 678.0M + 1 link = **1,118.35M** | +0.6% |
| `alloc_all` (one library) | 868.7M + 1 link = **1,309.60M** | 946.2M + 1 link (77.9M + 368.6M) = **1,392.43M** | **+6.3%, no extra link** |
| `platform_written` (`m`, `pthread`, both shared) | 2,147.4M + 1 link = **2,767.12M** | 2,345.6M + **2 links** = **3,595.15M** | **+30%, one extra link** |
| `dep_written` (`top`, `base`, library files of objects) | 1,685.0M + 1 link = **2,124.04M** | 1,880.4M + **3 links** = **3,211.22M** | **+51%, two extra links** |
| `sig_written` (two library files of objects; refused under v6) | 1,688.5M + 1 link = **2,134.72M** (builds) | 1,921.6M + 3 links = **3,243.91M** (refused) | +52% |

So **v6 meets the condition I wrote for v5 where the libraries are shared** (one extra link per
program naming two or more) and **misses it where one of them is a library file of objects**: the
reversed probe and route D are two links, since one probe with the libraries reversed *and* loaded
whole would lose what the reversal is for (a definition the whole load forces in, where the normal
link would take another library's: `weak_*` on GNU ld, an archive against a dylib on ld64, both
measured in section 1). The compiler's own process grows by about 80M per map it reads on ld64
(`alloc_all` +77.5M; two maps on `platform_written`, +198M); v6's first build of each runtime object
adds the 75M needs probe, cached after.

**Whether a probe's verdict can be kept by the link's key**: not by that key alone. The binary's
home is named by the link's words (`link_output.binary_home`), and the probes' verdict also rests on
the contents of every library file the link opens, which a rebuilt library changes under the same
words; that is why the real link runs on every warm build (`cli/link.hero:22-24`). A sound key is the
words and the digest of every file the link opened (ld64's map lists them, `-t` on GNU ld and lld):
then a warm build skips both probes. Unbuilt; its cost, digesting each library file, unmeasured.

### 14.4 The words (W2), verbatim, this Mac

`sig_sorted` (and `sig_written`, byte-identical):

```
error[ffi_defined_elsewhere]: `pick`, bound here from library `a`, is also defined by library `w`, which module `aux` links, and a program holds one `pick`: this call, or that library's own calls to it, would reach the other library's
  at <b239>/hero/picker.hero:2:5
  note: a definition is in `<b239>/c/out-Darwin/lib/liba.a(pick_a.o)`
  note: a definition is in `<b239>/c/out-Darwin/lib/libw.a(pick_w.o)`
  note: a program holds one definition of `pick`: link one library that defines it, or build the other library with its `pick` kept to itself (`objcopy --localize-symbol=pick` on Linux, `nmedit -R` on macOS, `-fvisibility=hidden` for a shared library)
```

`interpose`:

```
error[ffi_defined_elsewhere]: `puts`, bound here from the C library, would be called from library `q`, which module `quiet` links, not from the C library
  at <b239>/hero/printer.hero:2:5
  note: a definition is in `<b239>/c/out-Darwin/lib/libq.a(puts_q.o)`
  note: a program holds one definition of `puts`: link one library that defines it, or build the other library with its `puts` kept to itself (`objcopy --localize-symbol=puts` on Linux, `nmedit -R` on macOS, `-fvisibility=hidden` for a shared library)
```

`dup`, `one`, `ab`, `self` read as `sig` with their libraries (`b`, `b2`, `bs`); `weak`: *"would be
called from library `bw`, which module `extraw` links, not from library `a`, whatever the order of
the libraries"*; `runtime_name`: *"library `r`, which module `loud` links, defines `printf`, of the C
library, which the program's own runtime calls: the runtime's calls would reach library `r`'s"*,
noted *"where that is meant, as an allocator replaces `malloc` on purpose, bind each of them in a
group of library `r`, which says so"*; `hero_name`: *"library `hh`, which module `hh_lib` links,
defines `hero_print_end`, a name of the program's own runtime"*, noted *"every name of the runtime
begins `hero_`: rename `hero_print_end` where library `hh` is built"*. `intern_*` is not refused
(14.1); where GNU ld refuses its written order the words are *"`base_helper` is defined by library
`base`, which module `base_lib` links, and by library `other`, which module `other_lib` links, and
the program cannot be built with both"* (section 14.5). The `<path>(<object>)` spelling stays in
the notes: it is the one exact name of where the definition is, as the critic says.

### 14.5 Linux arm64 (`heroes-linux-arm64:latest`, GNU ld 2.44)

v6b (`sources 11a70e0569b6f6e2`, the same sources and key as this Mac's v6b) and v6b with R3's group
words (`build/239/routeg6/`, `37befe7365854e55`), each built in the container by its base
(`linux/v6.out`); base's runs repeated beside them (`hero-base6-linux.txt`):

| program | base | v6b | v6b with the group words |
|---|---|---|---|
| `dup_*` (static, shared), `sig_*`, `one_*`, `weak_*`, `ab_*`, `self_*` | `ab_written` `1 10 10`, `ab_sorted` exit 2, `self_*` `1 10` / `2 20`, the rest as section 6 | **exit 1 in both orders, every pair byte-identical** | the same |
| `interpose`, `runtime_name`, `alloc_main` (static, shared), `alloc_bound`, `alloc_more` | SIGSEGV 139; two empty lines; `true` | exit 1 each | exit 1 each |
| `alloc_all` | `100 true` | **`100 true`** | `100 true` |
| `hero_name` | exit 2 `internal error` | **exit 1** | exit 1 |
| `intern_written` / `intern_sorted` | exit 2 / `100 2 7` | **exit 1** / `100 2 7` | exit 1 / `100 2 7` |
| `remath`, `dep_written` | `4.0`, `41 2` | the same | the same |
| `dep_sorted` | exit 2 (2365) | exit 2 | **`41 2`** |

**The 25 roots**: identical to base under v6b and under v6b with the group words (`diff` of
`runs-base-linux.txt` against `runs-route6-linux.txt` and `runs-routeg6-linux.txt` silent), the 17
that build with whole outputs identical by `cmp` but the heap address in
`fixedbugs-2291-a-refused-close...`'s panic line (`0xaaaaeeb0cc78` against `0xaaaac23edc78`).
**The glibc pair program** (`glibc_*`): `1 16.0` in both orders under base, v6b and v6b with the
words (`linux/glibc6.out`). **Zero false refusals of tracked programs on either platform under v6.**

**The final v6 on Linux arm64** (`30f949d859f52b96`, built in the container from the final sources,
`linux/v6c.out`): every program's verdict as v6b's (`diff` of `hero-route6-linux.txt` and
`hero-route6c-linux.txt`, outputs left out, silent), every pair's message byte-identical across
orders, every message as v6b's but `hero_name`'s path of the runtime object (another compiler,
another runtime key), and the 25 roots identical to base (`runs-route6c-linux.txt`).

### 14.6 The change, and what v6 still does not see

- **Size** (`build/239/route6/diff/route6.diff`, `diff -ruN selfhost build/239/route6/selfhost`,
  1,587 lines): `cli/supplied.hero` new, 985 lines, **610 code lines** outside its tests;
  `cli/supplied_words.hero` new, 292 lines, 215 code lines; `cli/link.hero` +41 -11,
  `cli/link_warning.hero` +40 -4, `cli/assemble.hero` +15 -3, `cli/whose.hero` +7. `supplied.hero` is
  twice §11's ~300 and splits at its seams for a landing: the probes and the runtime's names, the
  readings of each linker's words, the judgement. **Tests**: five, all passed on this Mac
  (`route6/test.log`, 960,560,935,476 instructions retired, the compiler built as a test binary).
- **V1 reproduced first**: v5 builds `ab_*` silently in both orders on this Mac, `1 10 10`
  (`hero-runs/ab-v5/`); v6 refuses both.
- **What v6 does not see**: a duplicate of a name no group binds and the runtime does not need
  (`intern_*`, 14.1, by choice and measured); a second definition in a **shared** library of such a
  name (no symbol reader; on ld64 a dylib's own calls are bound to itself, on ELF they are not); a
  **weak** definition in a library file of objects that the whole load does not report beside a
  strong one, where the bound name's own library is the strong one and the other library's own
  calls reach it (unrun as a program); and **Windows**, asked nothing.

## Verdict (after section 14: v6)

- **`verdict`: veto** on 2363's (A) alone and on 2364's order search alone, unchanged and on the
  same measurements; **approve 2363's refusal as v6** (`sources 30f949d859f52b96`), and no longer v5,
  which misses `ab_*` on ld64 (V1, reproduced here).
- **`section`**: design.md §1.12, §1.11, §4.19 (design.md:606, and its header-order paragraph);
  CLAUDE.md § Precedence for the cost (robustness, rank 3, over *never slow the compiler down*, rank
  5: *"a guard that closes a corruption class lands, with its cost measured and reported"*). The
  document still says nothing of which library supplies a bound name.
- **`experiment`**: the binding C of sections 1, 7, 9; route (D) over real static libraries on both
  machines (14.1); v6 built as a compiler change, five tests passing, run on both platforms with and
  without R3's group words: every member of the class refused at exit 1 in both orders with
  byte-identical words (`dup`, `sig`, `one`, `weak`, `ab`, `self`, static and shared), `interpose`,
  `runtime_name`, the allocators that do not bind what they replace and `hero_name` told at exit 1,
  `alloc_all` accepted, the 25 tracked roots and the glibc pair unchanged.
- **cost** (this Mac, every link counted, 14.3): no library +0.6%; one library +6.3% and no extra
  link; two shared libraries +30% and one extra link; two libraries of which one is a library file
  of objects +51% and two extra links. **This breaks the condition I set on v5** (*"more than one
  extra link per two libraries"*) where a library file of objects is linked; I withdraw that
  condition on CLAUDE.md § Precedence's ground and report the cost instead, with the cache that
  would remove both probes from a warm build (14.3) as the owed repair of it.
- **`argument`** (115 words, `wc -w`): One C name has one definition in a linked program, and which
  one a call reaches is the libraries' order, or on ld64 the names'. v5 asked who supplied each
  bound name in two orders; the critic's `ab` showed a definition neither order loads, which another
  library's own call silently reaches. Loading every library file whole sees it, on both linkers,
  with both files named. Refusing every such duplicate would refuse static FFmpeg and static X11,
  measured, so v6 refuses those of names a group binds or the runtime calls, and says so in the
  specification's words. The runtime's names close `printf` and keep the allocator, by the author's
  own binding, with no new form.
- **`prediction`**: under v6 on the CI's Linux x86-64 leg and on `macos-26`, `build/239/hero/ab_written.hero`
  and `ab_sorted.hero` are refused at exit 1 with byte-identical words, `alloc_all.hero` builds and
  prints `100 true`, `intern_sorted.hero` builds and prints `100 2 7`, and `examples/sqlite/main.hero`
  and `examples/ledger/main.hero` (SQLite, §4.19's rung 3, no shim) print what base prints. Scored by
  the landing lane's run on the CI's legs at the push that carries the refusal; any of them moving
  falsifies it.
- **`condition`**: the 2363 veto lifts with v6's shape: the supplier asked in both orders of the
  libraries the program names, every library file of objects loaded whole, only libraries the
  program names judged, a group with no library read as interposition, and the runtime's names
  held to a group that binds them. I would move to **object** if a CI leg or the Windows box
  refuses a tracked program, if the sitting rules `intern_*` refused and no route that does it spares
  static FFmpeg and X11, or if the landing ships the probes without the digest-keyed cache and a
  measured warm build of a tracked program doubles. The 2364 veto lifts, as before, when the order
  search lands only with the layout comparison (B) in one push, or (D) replaces it.
