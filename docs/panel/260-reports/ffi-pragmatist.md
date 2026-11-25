# Panel 260, the ffi-pragmatist's report

Written 2026-11-25 from 12:40 on, as I go. My copy: the detached worktree `.claude/worktrees/260-ffi-pragmatist` at
`c3885b81b` (`git log --oneline -1`, clean). Long output: `.claude/worktrees/260-ffi-pragmatist-scratch/`. Door programs
(sources only): `.claude/worktrees/260-ffi-pragmatist/docs/panel/260-reports/programs/ffi-pragmatist/`. Load at the start
15.5 (`uptime`, 12:40); three containers of other sessions running (`docker ps`).

Every line says **ran** (a command I ran) or **read** (a file I opened).

## Status

**Done 13:25.** No container of mine left (`docker ps`: none named `p260-ffi-pragmatist`); no paid run, no commit,
no GitHub write (`gh` read only). Verdict at the end. **My compilers** (ran): this Mac, stage 1 by clang on the seed 12:40:14 to 12:40:45, stage 2 (`./heroes build selfhost/main.hero -o heroes-next`) exit 0 at 12:48:58, load 187; `heroes-next --version`: `heroes 0.6.0`, `sources 41f32a8f4b745c0c`, the base's key (`00-shared.md` § 3.3). Copied beside the base's runtime into `260-ffi-pragmatist-scratch/base/`. Linux arm64: the same two stages in the container `p260-ffi-pragmatist` (`--rm`), stage 1 exit 0 at 11:50:11 UTC, stage 2 exit 0 at 12:00:52 UTC (`real 10m40.311s`, `user 7m7.014s`, container load 5.4), `sources 41f32a8f4b745c0c`, the same key (the image has no `/usr/bin/time`, so a first attempt exited 127 and the stages are timed by bash's `time`; the container's clock is UTC, an hour behind this Mac's).

## First C, by hand (ran, 12:47:36 to 12:48:14 by the files' times, before the compilers were ready)

**Door 3's link order** (a C library the program names with `link`, whose object calls a layer function), in plain C:
`main.c` calls `lib_now`, `libnow.a(now.o)` calls `hero_time_monotonic_ns`, `liblayer.a` holds `hero_time.o` compiled
from the base's `runtime/platform/hero_time.c`; the link in R6's order, read in `selfhost/cli/link.hero:161-198`
(`link_line`: the objects, the layer's archive among them, then `-Wl,--start-group`, the `-l` words, `-Wl,--end-group`
where the target's linker reads an archive once). Scratch: `260-ffi-pragmatist-scratch/order/`.

| linker | R6's order | the layer inside the group | no layer at all (an undeclared build) |
|---|---|---|---|
| ld64 (Apple clang 21, this Mac) | **exit 0**, prints `1`; `-why_load`: `'_hero_time_monotonic_ns' caused load of liblayer.a[2](hero_time.o)` | (not needed) | exit 1: `Undefined symbols for architecture arm64:` / `"_hero_time_monotonic_ns", referenced from:` / `_lib_now in libnow.a[2](now.o)` |
| GNU ld 2.44 (`heroes-linux-arm64`, Debian clang 22.1.8) | **exit 1**: `/usr/bin/ld: ./libnow.a(now.o): in function 'lib_now':` / `now.c:(.text+0x8): undefined reference to 'hero_time_monotonic_ns'` | **exit 0**, prints `1` | exit 1, the same two lines |
| ld.lld 22.1.8 (same image, `-fuse-ld=lld`) | **exit 0**, prints `1` | (not needed) | exit 1: `ld.lld: error: undefined symbol: hero_time_monotonic_ns` / `>>> referenced by now.c` / `>>>               now.o:(lib_now) in archive ./libnow.a` |

So on GNU ld the layer's archive standing before the `-l` group is a place where R6 itself refuses a correct program
(checked with the real compiler below). It matters to the sitting because a `link` word naming the layer would put the
archive **inside** that one group (`link.hero:180-196`), where GNU ld resolves it.

**Who can put libm on a link** (`d-lane-layer-speed-2`), in plain C in the same image: `part.c` with `#pragma comment(lib,
"m")` calling `nextafter`, archived, linked with no `-lm`. clang writes the request into a `.deplibs` section
(`readelf -S`: `[ 4] .deplibs LLVM_DEPENDENT_`); **GNU ld 2.44 ignores it**, exit 1, `libpart.a(part.o): in function
'part_next': ... undefined reference to 'nextafter'`; **ld.lld 22.1.8 honours it**, exit 0, `1.0000000000000002`; GNU
ld with `-lm`, exit 0. So on the CI's default Linux linker a member cannot carry its own library need: the compiler puts
it on the link (scratch `/x/libm` in the container, gone with it).

**The fourth linker** (ran, 12:52:15 by its object's time, the arm64 image, clang `--target=aarch64-pc-windows-msvc -fuse-ld=lld -nostdlib`,
`/usr/bin/lld-link`): `main.obj` calling `hero_time_monotonic_ns` and `now.lib(now.obj)` calling it too, nothing
defining it: `lld-link: error: undefined symbol: hero_time_monotonic_ns` / `>>> referenced by main.obj:(mainCRTStartup)`
/ `>>> referenced by now.lib(now.obj):(lib_now)`. MSVC's `link.exe` is **unrun** (no box); `link_report.hero:31-45`
carries its `LNK2019`/`LNK2001` form from the box, 2026-11-10.

**The layer's archive loaded whole** (ran, 12:52:31 by its output's time, this Mac, the base's compiler): the logged link of `clock`
(`build/<key>/link-words.rsp`: the runtime's object, the two units, `build/layer-<key>.a`) replayed with the archive as
`-Wl,-force_load,<archive>`: **exit 1, 104 `referenced from` lines**, the first `"_hero_crypto_cryptokit_open"`,
`"_hero_crypto_cryptokit_seal"`, `"_hero_ui_appkit_accessible_name"`: the Swift members, compiled only where a
program's headers reach them (`layer_reach.hero`, defect 2625). So **the archive cannot be loaded whole on this Mac**,
and §4.19's probes load *"every library file of objects loaded whole"* for each library a program names
(`docs/design.md`, §4.19, the `ffi_defined_elsewhere` paragraph). A home that spells the layer with the existing
`link` word must therefore keep the layer out of those probes (it is judged by defect 2489's member path already,
`supplied_judged`), which makes `link "<the layer>"` a reserved name with its own meaning, not an ordinary library.

## My seat's own condition on panel 234's P2, which the principle reverses (read)

Panel 234's R6 says every condition this seat set on P2 holds, among them *"found and linked by the compiler with no
`link` word or flag in a package"* (`docs/panel/234-*.md:418-419`). **What the condition was for**, in the seat's own
report (`docs/panel/234-reports/ffi-pragmatist.md`): condition (4), lines 265-267, *"the compiler finds its headers on
its own include path and links its object when a group names one of them, so no package spells `link` or a flag for
it"*. Its evidence is F.1 and F.3: P2 was built **by hand** with `link "hero_platform"` **and** `--include platform
--library platform/<mac|linux>` on every build (lines 105-112, *"The `link` word and both flags are the by-hand
part"*), and F.3 measured what a placement-dependent spelling costs: `extern "hero_time.h"` needed `--include
core/time`, *"which every program using the package, and every program using a package that uses it, must pass"*
(line 195). So the condition's reason was **a path or a flag an importer must repeat**, and a header found by
placement; the `link` word rode along with the flags as convenience.

**Whether it still binds**: its reason binds, its trigger does not. A declaration that is one fixed word, naming no
path and no flag, written once where the binding is and carried by `use` like any group's `link` today, makes no
importer repeat anything; the header still stands on the runtime's include path. What replaced the trigger (*"links
its object when a group names one of them"*) is the author's principle: the layer is linked where the program
declares it, because paying for it where nothing declared it is what the author refuses. **The `link` word the group
already has is the C-natural home** by design.md §3.5's own sentence (*"a dependency is a link flag declared in the
source next to the `extern` that needs it"*) and spec § 13 line 401 (*"`link` a library when the symbols need one"*),
and it is the spelling this seat built by hand in 234 (`link "hero_platform"`). Its two costs are measured above: the
archive cannot be an ordinary named library (loaded whole it does not link on this Mac), so the name is reserved and
kept out of §4.19's probes; and on GNU ld it moves the archive inside the one library group, which is a gain (door 3).

## 2. What the linkers say when a layer name is missing (ran)

An undeclared build's link is today's link with `build/layer-<key>.a` taken out of its response file
(`build/<key>/link-words.rsp`, which the compiler hands clang as `@file`, logged by a clang wrapper; `doors.sh` in my
scratch). This Mac, the base's compiler (`sources 41f32a8f4b745c0c`), from 12:50:59 (`doors-b-O0.txt`):

```
clock (the group names hero_time.h)            linklib (door 3: libnow.a's object calls the layer)
Undefined symbols for architecture arm64:      Undefined symbols for architecture arm64:
  "_hero_time_monotonic_ns", referenced from:    "_hero_time_monotonic_ns", referenced from:
      _hero_extern_refs in main-4f26...ec.o          _lib_now in libnow.a[2](now.o)
      _h_main_main in main-4f26...ec.o           ld: symbol(s) not found for architecture arm64
ld: symbol(s) not found for architecture arm64
```

The program's own object refers to the name twice, from `main` and from `_hero_extern_refs`, the `used` table
`emit/extern_refs.hero` keeps for every function a unit binds (so a binding called only in a `test` block, or never,
is a reference all the same: `testonly`, `face`). GNU ld 2.44, ld.lld 22.1.8 and lld-link: above (plain C) and below
(the compiler's own links in the arm64 image). MSVC's `link.exe`: unrun.

**What `cli/link_report.hero` reads of these** (read, its lines 7-45): every form above, GNU ld's `in function` and
`undefined reference to`, ld64's `referenced from:` and the line under it, lld's and lld-link's `>>> referenced by`,
link.exe's `LNK2019`/`LNK2001`. It answers *who refers*: the program's own object (`main-*.o`) or a library's member
(`libnow.a[2](now.o)`). Today two readers act on that answer, and **both say something false of a layer name**: a
reference from a library the program names goes to `cli/library_needs.hero`, told `ffi_missing_link` at the `link
"now"` group as *a name a library the program links needs and nothing on the link line defines* (the layer defines
it); a reference from the program's own object, explained by a group's header text, goes to
`cli/header_needs.hero`, *"no group in this program says which library has it"* (§ 5 of the shared brief; route (a)'s
prototype printed it for `ownhdr`, `owndecl`, `platformhdr`). So the new diagnostic is asked **first, of every
undefined name, whoever refers to it**, and a name is the layer's when a layer header declares it (a text read on the
failure path alone, so an undeclared program that links pays nothing for it).

## 1. The doors, built under the base (ran) and read by each reading

Sources: `programs/ffi-pragmatist/` of my copy (each door's `main.hero` says what it is), with the shared brief's
`owndecl`, `ownhdr`, `platformhdr`, `constonly`, `twofam`, the tenth golden and `programs/` copied beside them in my
scratch (`progs/`). **Ran on this Mac, 12:50:59 to 12:57:48, load 125 to 185, `-O0`, each door in its own folder**
(`doors.sh`, output `mac/doors-b-O0.txt`): the build's exit; the members the link took, read by **replaying the logged
link with Apple's `-Wl,-map`** (the linker's own list, not a symbol count); the names the program's **own objects**
(`build/tu-*/*.o`) leave undefined that the archive defines, by `nm` **before any link**; and the link **replayed
without the archive** (the undeclared build), its exit. The three text readings (a group names a member's header; the
**rule** of step 182c's walk, every quoted include followed to any depth and every `#if` but `#if 0` read as true; a
bound name a member's header declares) are `readings.py`'s, **a script applying each rule to the text, not the
compiler's code run as a check**. The bodies' reading (`cli/header_needs.hero`) is **read from its doc, not run**.

| door | correct? | header name | walk's rule | bound name | own objects by `nm` | link without the archive | base, this Mac: exit, members the link took, run |
|---|---|---|---|---|---|---|---|
| `clock` (group names `hero_time.h`) | yes | yes | yes | yes | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `the clock reads: true` |
| `constonly` (binds only `HERO_NET_INVALID`) | yes | **yes** | **yes** | no | none | **exit 0** | 0; **none**; `1` |
| `ownhdr` (own header includes `hero_time.h`) | yes | no | yes | yes | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| `owndecl` (own header declares it, includes nothing of the layer) | **builds, unverified** (see `owndeclwrong`) | no | **no** | yes | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| 1 `macro` (own header: `#define now_ns hero_time_monotonic_ns` over `hero_time.h`) | yes | no | yes | **no** | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| 1b `macrodecl` (the same macro over its own declaration) | **builds, unverified** | no | **no** | **no** | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| 2 `fnptr` (glad's and GLEW's shape: the bound name a macro for a `static volatile` function-pointer variable initialised to the layer function) | yes | no | yes | **no** | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| 2b `fnvalue` (the bound layer function passed as a Heroes function value and called through it) | yes | yes | yes | yes | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true` |
| 3 `linklib` (`extern "libnow.h" link "now"`; `libnow.a(now.o)` calls `hero_time_monotonic_ns`) | yes | **no** | **no** | **no** | **none** (the library's member refers, not the program) | exit 1 (`_lib_now in libnow.a[2](now.o)`) | 0; `hero_time`; `true` |
| 4 `dlsym` (own header's `static inline` asks `dlsym(RTLD_DEFAULT, "hero_time_monotonic_ns")`, -2 where absent) | yes | no | no | no | none | exit 0 | 0; none; **`-2`**: R6 takes no member either |
| 4b `dlsymboth` (door 4 beside a group naming `hero_time.h`) | yes | yes | yes | yes | `hero_time_monotonic_ns` | exit 1 | 0; `hero_time`; `true`, `1` (found, `-rdynamic`) |
| 5 `ifnever` (own header includes `hero_net.h` under `#if defined(MAYBE_WITH_NET)`, never defined) | yes | no | **yes** | no | none | **exit 0** | 0; **none**; `42` |
| 5b `ifplatform` (own header includes `hero_time.h` under `#if defined(__linux__)`, `clock()` otherwise) | yes | no | **yes** | no | none here | exit 0 here | 0; none here; `true` (Linux below) |
| 6 `armhdr` (`extern "hero_audio_coreaudio.h"`, an arm's own header, binding `hero_audio_arm_permission`) | yes | yes, **if** an arm's header at `runtime/` counts as a member's | yes | no (declared under `platform/`) | `hero_audio_arm_permission` | exit 1 | 0; `hero_audio`, `hero_audio_coreaudio`, `hero_net`, `hero_stream`; `linked` |
| `platformhdr` (`extern "platform/hero_audio_arm.h"`) | yes | no | yes | no | `hero_audio_arm_permission` | exit 1 | 0; the same four; `linked` |
| 7 `owndeclswift` (own header declares `hero_crypto_last_error`, includes nothing of the layer) | **unverified** | no | **no** | yes | `hero_crypto_last_error` | exit 1 | **2, `internal error: linking failed`**: `"_hero_crypto_cryptokit_open"` and `_seal`, referenced from `hero_crypto.o`; then *the linker refused the objects clang compiled* |
| the tenth golden (own header two includes deep, `static` bodies call `hero_net_*`) | yes | no | yes | no | 11 `hero_net_*` names | exit 1 (`"_hero_net_accept"` first) | 0; `hero_net`; its own lines, `... meet their deadline: 1` |
| `twofam`, `audio`, `ui`, `print` | yes | as their groups | | | | `print` exit 0, others exit 1 | 0; `twofam` `hero_net hero_time`; `audio` the four above; `ui` `hero_net hero_poll hero_stream hero_stream_net hero_ui` and the five `hero_ui_appkit*` Swift members; `print` **none** |

| `owndeclwrong` (`owndecl` with the header saying `double hero_time_monotonic_ns(void);`, bound `-> f64`) | **no**: a wrong binding | no | no | no | `hero_time_monotonic_ns` | exit 1 | **0; `hero_time`; prints `6.3659874347e-314`**, three runs alike (13:09:27, `doors-b-wrong.txt`): a wrong value at exit 0 |

**What the table says** (each a reading of the rows above):
- **Only the link sees every door that needs a member, and nothing else.** The program's own objects by `nm` see every
  door whose reference lands in the program (1, 1b, 2, 2b, 6, 7, the ten, `owndecl`, `ownhdr`, `platformhdr`) and miss
  door 3, whose reference is in a library the program names; the link without the archive fails on exactly the doors
  whose link took a member under R6, and links `constonly`, `ifnever`, `dlsym` and `print`, which took none. **No text
  reading agrees with it**: the header name misses 1, 1b, 2, 3, 7, `owndecl`, `ownhdr`, `platformhdr` and the ten, and
  sees `constonly`; the walk's rule misses 1b, 3, 7 and `owndecl`, and **sees `ifnever`** (a member's header under a
  condition no build takes) and `constonly`; the bound name misses 1, 1b, 2, 3, 6, the ten, and `platformhdr`.
- **Door 4 (`dlsym`) is seen by nothing, and need not be**: R6 itself takes no member for it (`-2`), so a program that
  finds the layer by name at run time gets under the principle what it gets today. Door 4b finds it (`1`) only because
  a group binds the same member.
- **`owndecl`, 1b and 7 bind a layer function through a declaration the program wrote, so clang checks the binding
  against that declaration and never against the layer's** (§4.19: *the C compiler sees the real header*): `owndeclwrong`
  builds under R6 and prints a wrong value at exit 0. *(Corrected 13:10: this table first marked the three `yes`.)*
  So their refusal, which the compiler-engineer builds on `cli/runtime_bound.hero`'s precedent, guards the boundary's
  thesis; the fix both its messages name, the layer's own header, makes them `ownhdr` and door 1, which verify.
- **Door 7 is refused by R6 today**, at exit 2 as the compiler's internal error: `owndecl`'s shape over
  a member that calls a Swift member on this Mac. `cli/layer_reach.hero`'s own doc names the shape (*"What it cannot
  see: a program binding a function of the layer through a header of its own that declares it ... That link leaves
  the Swift member's names undefined and does not link, so no program is ever built with a member left out"*): the
  sentence is true, and the program is refused at exit 2. **Found beside, not filed** (`gh issue list --search` for
  `cryptokit`, `Swift member`, `header of its own`: defects 2625, 2626, 2687 and others, none this). A declaration that
  names what the group needs gives the compiler what the header text does not, so a declared door 7 can link.

## 7. ABI and what a binding sees (read, and the runs named)

- **`HERO_RUNTIME_ABI`** (`runtime/heroes_runtime.h:108`, 32) moves *"whenever the declarations change shape"* (its
  lines 19-27). The declaration changes no header, and **a group's `link` word moves no byte of the emitted C**: door 3 emitted
  with `--emit-c` as written and with ` link "now"` taken out, each as `main.hero` in its own folder, differ only in
  the line `hero_program_sources`, the sources' key (`diff` exit 0 with that line taken out; ran 12:59 to 13:00, scratch
  `emitc/`). A declaration spelled on the group is therefore a link-line fact, and the stamp does not move. **What a binding compiles against does not move**: every `hero_*.h` keeps its declarations, its
  one guard and its place on the runtime's include path; clang still checks every bound signature against the real
  header (§4.19's thesis at the boundary, untouched). `hero_os.h` and `hero_compiler.h` are the runtime's, not the
  layer's, so a program binding `hero_os.h` (decision 2228's door, `docs/design.md:3794-3798`) declares nothing.
- **The archive's name and suffix**: `build/layer-<key>-<seal>.a`, `.lib` where `link_flags.msvc` (`archiver.hero:191-196`).
  A declaration changes neither; a route that archives per family adds the families to `<key>`, as defect 2625 added
  the Swift members.
- **No value crosses the boundary differently**: no marshalling, no boxing, no change to `str`'s NUL, a record's layout
  or a handle's refcount. The principle is a link-line rule.
- **Each member's own libraries**: ld64 reads a member's `.linker_option` (`hero_audio_coreaudio.c:62-65`,
  `hero_keyring.c:71-72`), lld-link and link.exe its `#pragma comment(lib, ...)` (20 members, `grep -c`), lld its
  `.deplibs`; **GNU ld reads none** (run above). So on the CI's default Linux linker only the compiler can put a taken
  member's library on a declared link: libm for `hero_ui_parts_gtk.c:1260`'s `nextafter` (`d-lane-layer-speed-2`).
- **Door 3's order on each linker**, plain C (ran): ld64 exit 0, lld exit 0, **lld-link exit 0** (`main2.obj
  layer.lib now.lib`, `--target=aarch64-pc-windows-msvc -fuse-ld=lld`, the arm64 image), **GNU ld exit 1**. On an MSVC
  target the `.lib` archive behaves as lld's does in that order; **link.exe unrun** (no box).
- **The C toolchains that make a library declare what it links** (pkg-config's `Libs.private`, libtool's `.la`
  `dependency_libs`) are the historian's sources, not mine (my brief, item 7; the critic's § E says the historian's
  brief lacked the item): I read its report if it exists before I finish, and assert neither.

### What an undeclared program built through each door must be told, and on which line

Read from the replayed links above (who the linker says refers) and from the two readers' docs (`cli/header_needs.hero`,
`cli/library_needs.hero`); **the diagnostic is unbuilt**, so this is what it owes, not what it does.

| door | who the link says refers | the line a fix belongs on | which reader can place it today | fix |
|---|---|---|---|---|
| a group naming a member's header and binding its function (`clock`, 2b, `twofam`, `audio`, `ui`) | `main-*.o` (`_h_main_main`, `_hero_extern_refs`) | that group's string | the group's own text, **at `check`**, before any link | `certain`: the group gains the declaration |
| `owndecl`, door 7 (the bound name IS the layer's name, declared by the program's own header) | `main-*.o` | the binding (its group) | the bound name against the layer headers' declarations | `certain` |
| `ownhdr`, 1 `macro`, 2 `fnptr`, `platformhdr`, 6 `armhdr`, the ten | `main-*.o` | the group whose header's includes reach a layer header | 1 and the ten by `header_needs` (a macro for it, a `static inline` body); **2 by neither** (the name is a variable's initialiser), only by following the group's includes | `certain` where one group's header reaches it, `guess` where two do |
| 1b `macrodecl` | `main-*.o` | the group whose header's macro names it | `header_needs` (*"a bound name the header makes a macro for it"*) | `certain` |
| 3 `linklib` | `libnow.a[2](now.o)`, **not the program** | the group with `link "now"`: the library needs the layer | `library_needs`, which today says *"nothing on the link line defines"* it (false: the layer does) | `certain` |
| 4 `dlsym`, 5 `ifnever`, `constonly`, 5b here | nobody: the link without the archive succeeds | nothing to tell | | |

So **two readings, each right about a different thing** (corrected 13:11, after `owndeclwrong`; this paragraph
first said the link's reading alone was right): **the link** is right about *which programs need a member*, on
every door and platform measured; **the group's header and its includes** (the walk's reach) are right about *which
bindings clang checked against the layer's own declarations*, and a binding of a layer name outside that reach is
unverified (`owndecl`, 1b, 7; `owndeclwrong` prints a wrong value at exit 0 under R6). A check built on both, the
word demanded by the header name at `check` and by the walk before the first clang, a layer name bound through a
declaration of the program's own refused as `cli/runtime_bound.hero` refuses the runtime's, and the link asked first
where it still fails (door 3, whose library verified itself against `hero_time.h` when it was compiled), refuses no
verified program in every spelling: door 3's spelling is its header including the layer's (3'). Its cost is the
walk's loud direction: `ifnever`, door 5b on this Mac and `constonly` must say the word and take nothing, so what
a declared build pays for nothing is the archive route's question (below).

### Linux arm64: a second member needing libm (ran, 13:01, found beside, not filed)

Door 6 (`armhdr`) under the base in `heroes-linux-arm64` (Debian clang 22.1.8, GNU ld 2.44, `-O0`): **exit 2,
`internal error: linking failed:`** / `build/layer-23769fa2656bc853-7ee86660507d7361.a(hero_audio_pulse.o): in function
'hero_audio_pulse_tone':` / `/w/runtime/platform/hero_audio_pulse.c:573:(.text+0x1460): undefined reference to 'sin'`
/ the same for `hero_audio.o`, `hero_audio_null_run`, `hero_audio.c:641` / *error: the linker refused the objects clang
compiled*. So `d-lane-layer-speed-2` (`hero_ui_parts_gtk.c:1260`, `nextafter`) is **a class, not one call**: the audio
family's members call `sin` too, and nothing puts `-lm` on the link. `gh issue list --state all --search` for
`undefined reference to sin`, `libm`, `hero_audio Linux`, `-lm`: no such defect (2395, 445, 533 and 424 are other
questions). The family table below says which levels it stops.

### Door 3': one group, two libraries its symbols need (ran, 13:02:54 to 13:03:13, this Mac)

`programs/ffi-pragmatist/linklibhdr/`: `extern "libnow.h" link "now"` binds `lib_now` (the program's own library, whose
object calls `hero_time_monotonic_ns`) and `header_now`, a `static inline` of `libnow.h`, which includes `hero_time.h`
and calls the clock. **Base: exit 0, takes `hero_time`, prints `true`, `true`; without the archive, exit 1.** It is the
plainest way a C programmer wraps a library of their own that uses the layer's sockets or clock.

**What it does to the spellings** (read in the spec-warden's drafts, `260-reports/spec-w*.diff`, 12:46): the grammar
gives a group **one** library string (`Extern = "extern" string [ ( "link" | "pkgconfig" ) string ]`, spec § 13), and a
group needs at least one binding (ran: `extern "hero_time.h"` with none is `error[expected_extern_block]`). So:
- **`w5`, `link "layer"`** (*"A group whose header is one of these or includes one at any depth, or that binds a name
  one declares, must say `link "layer"`, and no other group may"*): door 3''s group must say `link "layer"` because
  `libnow.h` includes `hero_time.h`, and must say `link "now"` because its symbols need `libnow.a`; it can say one.
  Splitting it into two groups over the same header does not help, since each group naming `libnow.h` owes the word.
  **Refused in every spelling** of that program as written. Door 3 alone (`libnow.h` reaching nothing of the layer) owes
  nothing by `w5`'s rule, so its link fails with no fix on its own group, and the only declaration left is a second
  group binding a layer function the program never calls. The same holds for any `pkgconfig` group.
- **`w1`, a mark beside the library** (`Extern = "extern" string [ "layer" ] [ ( "link" | "pkgconfig" ) string ]`):
  `extern "libnow.h" layer link "now"` declares door 3' and door 3 on their own group.

So the existing `link` word **cannot be the home** while a group has one library string: that is a refusal of a
correct program in every spelling, which my seat's veto binds (below).

### Door 3 on Linux arm64, by the compiler (ran, its build.err written 12:04:15 UTC, 13:04 here; found beside, not filed)

The base's own build of `linklib` in `heroes-linux-arm64`: **exit 1**, `error[ffi_missing_link]: the library 'now' needs
'hero_time_monotonic_ns', and nothing on the link line defines it`, at `extern "libnow.h" link "now"`, its note
*"give this program a group for the library that defines it as well"*. Its response file:
`build/layer-20711caa560aa4a0-7ee86660507d7361.a -Wl,--start-group -lnow -Wl,--end-group`. **The sentence is false**:
the layer's archive is on the line, one word before the group; GNU ld 2.44 had passed it when `libnow.a(now.o)` asked
(the plain-C table above). The same program builds and runs on this Mac (ld64). So under R6 today **a correct program
is refused on the CI's Linux linker with a false message**; the class is `blocking` by the project's own list (*a
false message, a correct program refused*). `gh issue list --state all --search` for `nothing on the link line
defines`, `library needs layer`, `layer archive order`: defects 2372, 2501, 562, 2489 and others, none this. **A
declared link that puts the archive inside the one library group** (`link.hero:192-196` already writes that group)
links it (plain C: exit 0).

**Door 5b (`ifplatform`) on Linux**: exit 0, the program's own object leaves `hero_time_monotonic_ns` undefined (`nm`),
and the link without the archive fails (`plat.h:6: undefined reference to 'hero_time_monotonic_ns'`): the same source
**needs the layer on Linux and not on this Mac** (where it took nothing and linked without the archive). So *binds
the layer*, read by the only reading that is right on every door, **is a fact of a platform**; a package holding this
header is declared rightly on Linux and *"binds nothing"* on this Mac.

### The spec-warden's two group spellings, held to the doors (read its drafts; each door's row measured above)

`w1` (12:46): *"A group whose header is one of these or includes one at any depth, or that binds a name one declares,
must say `layer` after its header, **and no other group may**; a build links the layer only where a module it compiles
holds such a group."* `w5` is the same rule spelled `link "layer"`.

| door | what the link needs (measured) | `w1`'s rule demands | `w1`'s rule allows the honest declaration? |
|---|---|---|---|
| `clock`, 2b, `ownhdr`, `owndecl`, 1, 2, 6, `platformhdr`, 7, the ten | the layer | `layer` | yes |
| 1b `macrodecl` (bound `now_ns`, a macro for `hero_time_monotonic_ns`, which the header declares itself) | the layer | nothing (no layer header included, no bound name a layer header declares) | no, **rightly**: an own declaration, unverified (`owndeclwrong`); including `hero_time.h` makes it door 1 |
| 3 `linklib` (the program's own library calls the layer) | the layer (GNU ld today: refused, false message) | nothing | **no** |
| 3' `linklibhdr` | the layer | `layer` (header includes `hero_time.h`) | yes under `w1` (`layer link "now"`); **no under `w5`** (one library string) |
| 5 `ifnever`, `constonly` | **nothing** (no member, links without the archive) | `layer` (header includes one; header is one) | demands a declaration the link never uses: the program pays the layer for nothing |
| 5b `ifplatform` | the layer on Linux, nothing on this Mac | `layer` everywhere | right on Linux, paid for nothing on this Mac |
| 4 `dlsym` | nothing | nothing | yes |

So **`w1` as written refuses door 3 on its own group** (the program's library needs the layer, and its header reaches
none, so no group of the program may say so); door 1b's refusal is right (an unverified declaration). Door 3 has a
spelling that verifies: the library's header, or a header of the program's own around it, includes `hero_time.h`,
which is door 3' and builds on both linkers. **`w5` has none for door 3'**: one library string per group, so the
group needs `link "now"` and `link "layer"` at once, and the only way out is to declare `lib_now` again in a header
of the program's own, which is the unverified binding `owndeclwrong` measures. The rule also demands of doors 5,
5b and `constonly` a declaration their link never uses, paying the layer's build for nothing (*"paying when you do
not use it is not"* fine): a cost of reading *binds* by the text, not a refusal. *(Corrected 13:10: this paragraph
first counted door 1b among the refused correct programs.)*

## 6. The defects of the same class, under the principle (ran where it says)

- **Defect 2687** (open), run-time side, this Mac, `otool -L` of the family binaries built at `-O0` (after that run ended, 13:03:16): the
  program that calls **only `hero_crypto_last_error()`** loads `CoreFoundation CryptoKit Foundation Security` and nine
  `libswift*` libraries at every start; `ui` (only `hero_ui_native_here()`) loads 27, AppKit, WebKit and the Swift
  runtime among them; `audio` `AudioToolbox CoreAudio CoreFoundation libobjc`; `keyring` `CoreFoundation Security`;
  `time` and `print` **`libSystem.B.dylib` alone**. A declaration changes none of this for a declared program: what a
  program loads is decided by which member objects its link takes, and the crypto member is one object. The principle
  removes the cost from undeclared programs (they take no member, which R6 already gives them at run time); only
  splitting the member would remove it from a program that digests. Not a cost of the declaration, and not repaired
  by it.

## 3. The link line, today and declared (ran for today's; the declared one is written from the runs above)

**R6 whole, today** (the base's response file for `clock`, the compile's warning and `-f`/`-D` words left out; the
same shape on both platforms):

```
this Mac:    -O0 -g -rdynamic build/runtime-<k>.o build/tu-<k>/main-<s>.o build/tu-<k>/library-<s>.o build/layer-20e935c16b67edb7-4c06ceee57b2a396.a
Linux arm64: -O0 -g -rdynamic build/runtime-<k>.o build/tu-<k>/main-<s>.o build/tu-<k>/library-<s>.o build/layer-230debeb1fccc22b-7ee86660507d7361.a
with a `link` library (door 3, Linux): ... build/layer-<k>.a -Wl,--start-group -lnow -Wl,--end-group      <- GNU ld: refused
```

**Declared, what the C facts above require of it**, whichever route the compiler-engineer prices for the archive:

```
GNU ld targets: ... build/tu-*.o -Wl,--start-group build/layer-<k>.a -l<each link> [-lm where a taken member calls libm] -Wl,--end-group
ld64, lld, lld-link: ... build/tu-*.o build/layer-<k>.a -l<each link>   (order-free, measured; each member's .linker_option or #pragma comment(lib) carries its own frameworks and libraries)
undeclared:     ... build/tu-*.o -l<each link>                           (no archive; print, constonly, ifnever, dlsym and door 5b on this Mac link so, measured)
```

**Which members a declared link takes** (the linker's own words: Apple's `-map` on this Mac, GNU ld's `-Map` in the
container): the door table above; the family table below reads the families whose functions the **binary** holds, by
`nm`, global `T` symbols only. One measured fact for the route: what a link takes **moves with the platform** (door
5b; `armhdr` takes `hero_audio_coreaudio` here and `hero_audio_pulse` there, by each linker's map), so a closure the
compiler computes from the program's text before the link is a closure for no one platform; the link's own answer
is. *(Corrected before 13:09: this paragraph first said what a link takes moves with the level too, from the `acl`
program's binary holding `hero_stream_*` at `-O0` alone; read again, those were the runtime's own local functions,
`t _hero_stream_flushed_held`, which my pattern counted. Level dependence is unmeasured.)*

### The doors on Linux arm64 (ran, 12:00:57 to 12:07:11 UTC, 13:00 to 13:07 here; door 3' 12:07:36 to 12:07:54)

The same programs under the base built in the container (Debian clang 22.1.8, GNU ld 2.44, `-O0`; `doors.sh`, output
`lnx/doors-b-O0.txt`), members by **replaying each link with `-Wl,-Map`** (GNU ld 2.44's single `-t` names the archive
and not its members; `lnx/members.sh`, output `lnx/members-b-O0.txt`):

| door | base, Linux arm64: exit | members the link took (`-Map`) | link without the archive |
|---|---|---|---|
| `clock`, 1 `macro`, 1b `macrodecl`, 2 `fnptr`, 2b `fnvalue`, 4b, `owndecl`, `ownhdr` | 0 | `hero_time` | exit 1 |
| 5b `ifplatform` | 0 | **`hero_time`** (none on this Mac) | exit 1 (none on this Mac) |
| 7 `owndeclswift` | **0** (no Swift member on Linux; exit 2 on this Mac) | `hero_crypto` | exit 1 |
| `constonly`, 4 `dlsym` (`-2`), 5 `ifnever` (`42`), `print` | 0 | none | exit 0 |
| `twofam`; the tenth | 0 | `hero_net hero_time`; `hero_net` | exit 1 |
| 3 `linklib` | **1**, `ffi_missing_link`, *"nothing on the link line defines it"* (false) | none | exit 1 |
| 3' `linklibhdr` | 0, `true`, `true` (the program's own object asks first) | `hero_time` | exit 1 |
| `audio`, 6 `armhdr`, `platformhdr` | **2**, `internal error: linking failed`, `undefined reference to 'sin'` | `hero_audio hero_audio_pulse hero_net hero_stream` | exit 1 |
| `ui` | **2**, `undefined reference to 'nextafter'` (`d-lane-layer-speed-2`) | `hero_net hero_poll hero_printing hero_stream hero_stream_net hero_ui hero_ui_canvas_gtk hero_ui_gtk hero_ui_parts_gtk hero_ui_print_gtk hero_ui_web_gtk` | exit 1 |

GNU ld's words for an undeclared link (the archive taken out), the program's own object referring
(`gnuld-words.txt`): `/x/run/w-b-O0/clock/main.c:159:(.text+0x4): undefined reference to 'hero_time_monotonic_ns'`;
for door 2 the reference is data, `main-*.o:(.data+0x0): undefined reference to 'hero_time_monotonic_ns'`; for door 5b
`plat.h:6:(.text+0x24): undefined reference to ...`, the header's own line. **So on the CI's Linux linker today, R6
refuses at `-O0` every program reaching `hero_audio` or `hero_ui` (libm) and door 3 (order), each a correct program.**

ld.lld 22.1.8's words for the same undeclared links (the response file without the archive, `-fuse-ld=lld`;
`lnx/lld-words.txt`): `ld.lld: error: undefined symbol: hero_time_monotonic_ns` / `>>> referenced by main.c:159
(/x/run/w-b-O0/clock/main.c:159)` / `>>>               build/tu-.../main-*.o:(hero_extern_refs)`; door 3: `>>>
referenced by now.c` / `>>>               now.o:(lib_now) in archive lib/libnow.a`. Every form is one
`cli/link_report.hero` already reads (its lines 7-23).

## Where the shared brief (and the briefs' sources) was false or silent

1. **§ 6.2 and § 5 treat `owndecl` as a correct shape the readings must see.** Measured: its binding is checked by
   clang against the program's own declaration, never the layer's, and the same shape with a wrong return type builds
   under R6 and prints `6.3659874347e-314` at exit 0 (`owndeclwrong`). It is an unverified binding, and refusing it
   is §4.19's thesis, not a cost.
2. **§ 4.1 says the archive stands *"after every object"* (`assemble.hero:263`)** and is silent that on GNU ld it then
   stands *before* the one `-l` group (`link.hero:192-196`): a library the program names whose object calls the layer
   is refused today on Linux (door 3), told `ffi_missing_link`, *"nothing on the link line defines it"*, false.
3. **§ 8 names one libm call** (`hero_ui_parts_gtk.c:1260`, `nextafter`); `hero_audio.c:641` and
   `hero_audio_pulse.c:573` call `sin` the same way, so every program reaching `hero_audio` fails to link on Linux
   arm64 at `-O0` too (`audio`, `armhdr`, `platformhdr`, the family run).
4. **`cli/layer_reach.hero`'s doc** (*"so no program is ever built with a member left out"*), which § 5's defect 2625
   rests on, is silent that the program it names is refused at exit 2 as an internal error on this Mac (door 7).
5. **§ 3.2 and § 7.1 (the critic's § C): *"the `link` word the group already has ... no new grammar"*** is false for a
   group that already names a library: the grammar gives a group one library string, so door 3' cannot be declared
   with `link "<the layer>"` without a second string (new grammar) or an unverified re-declaration.
6. **§ 5 sends the new diagnostic before `cli/header_needs.hero`**; it is silent on `cli/library_needs.hero`, which
   answers a library's reference first and says *"nothing on the link line defines it"* of a layer name (door 3).
7. **§ 3.1 counts 41 member headers at `runtime/<member>.h`** and is silent that seven of them say no program includes them (`grep -liE 'no program includes|no heroes group
   names|no group names'` over the 41, 13:11: the three audio arms', `hero_ui_canvas_win32.h`, `hero_ui_gtk.h`,
   `hero_ui_parts_win32.h`, `hero_ui_win32.h`; `hero_audio_coreaudio.h:12`: *"No program includes it"*), which nothing refuses: `armhdr` builds
   on this Mac through the CoreAudio arm's header and, on Linux, takes the PulseAudio arm through the same header name.

## 4. The member-to-member calls, from the archives real builds wrote (ran)

`calls.sh <archive>`: every member of a `build/layer-*.a` a build of the all-families program wrote (`zall.hero`, the
last one built at each level, so its archive holds every Swift member this Mac compiles) extracted by `ar x`, read by
`nm -g`; a member's undefined name another member defines is a call, grouped by the header that declares the callee
(the defining member's own header first); then route (a)'s rule (a member's header and its `.c`, every quoted include
followed from the includer's folder, no `#if` read) says which calls fall outside its closure.

| platform, level | members with symbols | member-to-member calls | outside route (a)'s closure | by the callee's header |
|---|---|---|---|---|
| macOS arm64, `-O0` (13:03, `archive-b-O0.a`, 5,132,488 bytes) | 41 | **217** | **9**, all `hero_audio` to `hero_audio_coreaudio` | `platform/hero_ui_appkit.h` 58, `hero_stream.h` 54, `hero_net.h` 27, `platform/hero_ui_appkit_windows.h` 20, `platform/hero_audio_arm.h` 16, `platform/hero_ui_appkit_system.h` 12, `hero_poll.h` 9, `platform/hero_ui_appkit_dialogs.h` 7, `hero_ipc.h` 5, `platform/hero_ui_appkit_print.h` 5, `platform/hero_crypto_cryptokit.h` 2, `hero_stream_net.h` 1, `hero_time.h` 1 |
| macOS arm64, `-O2` (13:11, `archive-b-O2.a`, 4,127,864 bytes) | 41 | **217**, the same pairs | **9** | the same |
| Linux arm64, `-O0` (12:09:50 UTC, the container's `archive-b-O0.a`) | 38 | **283** | **107**: `hero_audio` to `hero_audio_pulse` 9, `hero_ui` to `hero_ui_gtk` 98 | `platform/hero_ui_arm.h` 127, `hero_stream.h` 59, `hero_net.h` 37, `platform/hero_audio_arm.h` 16, `hero_poll.h` 9, `hero_ui_parts_gtk.h` 8, `hero_ui_web_gtk.h` 6, `hero_ipc.h` 5, `hero_ui_canvas_gtk.h` 5, `platform/hero_ui_print_gtk.h` 5, `hero_stream_net.h` 4, `hero_time.h` 1, `hero_printing.h` 1 |
| Linux arm64, `-O2` (12:12:51 UTC) | 38 | **283** | **107**, the same | the same |
| macOS arm64, `--sanitize` (13:17:52) | 49 | **217** | **9** | the same as `-O0` |
| Linux arm64, `--sanitize` (12:16:01 UTC) | 43 | **283** | **107** | the same as `-O0` |
| Linux x86-64 (`heroes-linux`, emulated; Debian clang 22.1.8; the 43 C members compiled **alone** under the base's `-O0` words, as `ground/linux-nm.sh` does, all 43 compiled, 12:21:40 UTC) | 38 | **283** | **107**, the same pairs | the same as Linux arm64 |
| Linux x86-64, `-O2`, the same way (12:24:53 UTC) | 38 | **283** | **107** | the same |

So § 6.1's 9 and 107 (members compiled alone) are the same counts from the archives real builds wrote, at both levels;
the 104 calls into the five AppKit members and the 2 into CryptoKit are inside (a)'s closure, since `hero_ui.c` and
`hero_crypto.c` include their `platform/*.h`. **A program's own header closure is a different, wider miss**: by the
same calls, a program binding `hero_activation.h`, `hero_ipc.h` or `hero_serial.h` needs `hero_net`, one binding
`hero_poll.h` needs `hero_stream_net`, `hero_tls.h` `hero_net` and `hero_time`, `hero_crypto.h` its CryptoKit member,
`hero_ui.h` nine members, none of which its header's includes reach, and `hero_audio.h` its CoreAudio arm
(`calls/mac-O0-all.txt`, *beyond*, counted: **8** headers, `activation`, `audio`, `crypto`, `ipc`, `poll`, `serial`,
`tls`, `ui`). So a family table written from headers alone is wrong for 8 of the 30 family headers on this Mac. On Linux arm64 (`lnx/calls-O0.txt`) it is wrong for **7**: `activation`, `audio` (its PulseAudio arm), `ipc`, `poll`, `serial`, `tls` (`hero_net`, `hero_stream_net`, `hero_time`) and `ui`, which needs ten members **`hero_printing`** among them, the member that probes `cups.h`: so `d-lane-layer-speed-1`'s planted header stops a UI program on Linux under an exact closure too, and a family table must come from the archive's own calls on each platform, not from headers.

**Calls `nm` cannot see, looked for** (read, `grep` over `runtime/`): no member finds another by a string (`dlsym`,
`GetProcAddress` of a `"hero_*"` name: none); no weak reference in a member (`weak_import` once, `parts/calls.c:148`,
the runtime's); one constructor in a member (`hero_ui.c:665`), which runs only where the link took `hero_ui.o`; a
function pointer to another member is a data relocation `nm` lists as `U` (door 2's own object shows it:
`main-*.o:(.data+0x0): undefined reference`); the Swift members' C entries are `@c(...)` names (102 attributes) that
`nm` lists once the Swift object is in the archive (the 104 + 2 above). The members' own run-time loading
(`dlopen`/`dlsym` of GTK, CUPS, libsecret, OpenSSL: task 2593's slots) is a member calling the platform, not a member.

## Verdict (drafted 13:14, held to the runs that end after it)

- **verdict**: **approve, on six conditions; the veto binds one spelling and one pairing of readings.**
- **section**: design.md §1.11 (the founding constraint; the layer *"whose archive stands on every program's link
  line"*, which the principle amends), §3.5 (*"a dependency is a link flag declared in the source next to the `extern`
  that needs it"*: the declaration's natural home is the group), §4.19 (clang checks a binding against the real header;
  the probes load a named library whole; defect 2489's members judged as the runtime). The document does not cover a
  group needing two libraries; spec § 13's grammar gives it one string.
- **what the principle does to the boundary, measured**: nothing a value crosses moves. The emitted C of a group with
  and without a library word differs only in `hero_program_sources`; headers, `HERO_RUNTIME_ABI`, the archive's name
  and clang's check of every binding are untouched. It is a rule of the link line, and the link line is where R6 is
  wrong today on the CI's Linux linker (three correct shapes refused, below).
- **the conditions**:
  1. **The home is a word on the group beside its library string** (the spec-warden's `w1`, the compiler-engineer's
     built `layer`), never the library string itself.
  2. **The word is demanded and judged by one reading on both sides** (the group's header name at `check`, the walk's
     reach before the first clang, as the compiler-engineer built), and **a layer name bound through a declaration the
     program wrote is refused** as `cli/runtime_bound.hero` refuses the runtime's (`owndeclwrong`: a wrong value at
     exit 0 under R6).
  3. **A link that still fails on a name a layer header declares is told by the new diagnostic before
     `cli/library_needs.hero` and `cli/header_needs.hero`**, each of which says something false of a layer name
     (*"nothing on the link line defines it"*, door 3 on Linux; *"no group in this program says which library has
     it"*), the group placed by those readers and its fix naming the layer's header.
  4. **A declared link on a GNU ld target puts the archive inside the one library group, and `-lm` where a member it
     takes calls libm.** R6 refuses door 3 on GNU ld today with a false message, and every program reaching
     `hero_audio` (`sin`) or `hero_ui` (`nextafter`) at `-O0` in `heroes-linux-arm64`; GNU ld reads no member's
     `.deplibs`, so only the compiler can.
  5. **A declared build's archive holds the Swift members its link needs**, not only those its headers' text reaches,
     or condition 2's refusal tells door 7 rightly: today it is `internal error` at exit 2 on this Mac.
  6. **Nothing of the C ABI moves**: `HERO_RUNTIME_ABI`, every `hero_*.h`, the archive's name and suffix; `hero_os.h`
     stays the runtime's (decision 2228) and owes no word.
- **the veto** (a refusal, not a price): (V1) **`link "<the layer>"` as the home while a group has one library
  string** (`w5`): door 3' (a program's own C library whose header and object both use the layer) then has no spelling
  but re-declaring the library's function in a header of the program's own, the unverified shape `owndeclwrong`
  measures; that makes a binding unsafe. (V2) **the demand read by the text and *"binds nothing"* read by the link**
  (or by one platform's link): `constonly` and `ifnever` (correct, verified, taking no member) are then refused
  undeclared by the text and declared by the link, and door 5b is refused on this Mac whichever it says.
- **what the veto compels**: for V1, a mark beside the library string (no grammar is saved by `link`, since a second
  string is grammar too); for V2, *"binds nothing"* judged by the same text reading as the demand, or not refused.
- **what the conservative resolution would have been**: R6 as it stands, no declaration (every program pays the
  layer's `-M` and cold archive; the three Linux refusals and door 7 stay as they are), or the principle with the
  group's header name as the only reading and every other shape left to the link's failure, told as today.

## 5. A program per family, declared (R6 whole) and undeclared (ran)

`programs/ffi-pragmatist/families/`: one program per family header a group names in the tree (30; `grep -rhoE
'^extern "hero_[a-z_]+\.h"'` over `rebel/library`, `tests/golden/run`, `examples`, `tests/harness`, less `hero_os.h`
and `hero_compiler.h`), each binding one function and printing its answer, and `zall.hero` naming all 30. **Built under
the base**, all in one folder per platform (one shared `build/`), at `-O0`, `-O2` and `--sanitize` (`families.sh`):
this Mac 12:57:53 to 13:17:52 at load 99 to 145; Linux arm64 12:07:11 to 12:16:01 UTC at container load about 5.
**Declared** here is R6 whole, the base's build (no declaration exists to build); **undeclared** is each program's
logged `-O0` link replayed without the archive (`famnolayer.sh`): **31 of 31 exit 1 on both platforms**, each naming
its bound function (`"_hero_acl_last_error", referenced from:`; `undefined reference to 'hero_acl_last_error'`), since
every program binds a member's function. The last column is the families whose **global** functions each binary
holds (`nm`, `T` only; `took.sh`; `stream` counted without `stream_net`'s).

| family | platform | `-O0` | `-O2` | `--sanitize` | `-O0` printed | families whose global functions each binary holds (`nm`, `T`) |
|---|---|---|---|---|---|---|
| acl | macOS arm64 | 0 | 0 | 0 | `acl: 0` | O0: acl, O2: acl, sanitize: acl |
| acl | Linux arm64 | 0 | 0 | 0 | `acl: 0` | O0: acl, O2: acl, sanitize: acl |
| activation | macOS arm64 | 0 | 0 | 0 | `activation: 0` | O0: activation ipc net stream, O2: activation ipc net stream, sanitize: activation ipc net stream |
| activation | Linux arm64 | 0 | 0 | 0 | `activation: 0` | O0: activation ipc net stream, O2: activation ipc net stream, sanitize: activation ipc net stream |
| audio | macOS arm64 | 0 | 0 | 0 | `audio: 0` | O0: audio net stream, O2: audio net stream, sanitize: audio net stream |
| audio | Linux arm64 | **2**, `internal error: linking failed:` | **2**, `internal error: linking failed:` | 0 | `` | O0: (no binary), O2: (no binary), sanitize: audio net stream |
| crypto | macOS arm64 | 0 | 0 | 0 | `crypto: 0` | O0: crypto, O2: crypto, sanitize: crypto |
| crypto | Linux arm64 | 0 | 0 | 0 | `crypto: 0` | O0: crypto, O2: crypto, sanitize: crypto |
| disk | macOS arm64 | 0 | 0 | 0 | `disk: 0` | O0: disk stream, O2: disk stream, sanitize: disk stream |
| disk | Linux arm64 | 0 | 0 | 0 | `disk: 0` | O0: disk stream, O2: disk stream, sanitize: disk stream |
| ipc | macOS arm64 | 0 | 0 | 0 | `ipc: 0` | O0: ipc net stream, O2: ipc net stream, sanitize: ipc net stream |
| ipc | Linux arm64 | 0 | 0 | 0 | `ipc: 0` | O0: ipc net stream, O2: ipc net stream, sanitize: ipc net stream |
| keyring | macOS arm64 | 0 | 0 | 0 | `keyring: 0` | O0: keyring, O2: keyring, sanitize: keyring |
| keyring | Linux arm64 | 0 | 0 | 0 | `keyring: 0` | O0: keyring, O2: keyring, sanitize: keyring |
| locale | macOS arm64 | 0 | 0 | 0 | `locale: 12884901888` | O0: locale, O2: locale, sanitize: locale |
| locale | Linux arm64 | 0 | 0 | 0 | `locale: 4294967296` | O0: locale, O2: locale, sanitize: locale |
| net | macOS arm64 | 0 | 0 | 0 | `net: 0` | O0: net, O2: net, sanitize: net |
| net | Linux arm64 | 0 | 0 | 0 | `net: 0` | O0: net, O2: net, sanitize: net |
| odbc | macOS arm64 | 0 | 0 | 0 | `odbc: 0` | O0: odbc, O2: odbc, sanitize: odbc |
| odbc | Linux arm64 | 0 | 0 | 0 | `odbc: 0` | O0: odbc, O2: odbc, sanitize: odbc |
| poll | macOS arm64 | 0 | 0 | 0 | `poll: 0` | O0: net poll stream stream_net, O2: net poll stream stream_net, sanitize: net poll stream stream_net |
| poll | Linux arm64 | 0 | 0 | 0 | `poll: 0` | O0: net poll stream stream_net, O2: net poll stream stream_net, sanitize: net poll stream stream_net |
| printing | macOS arm64 | 0 | 0 | 0 | `printing: 0` | O0: printing, O2: printing, sanitize: printing |
| printing | Linux arm64 | 0 | 0 | 0 | `printing: 0` | O0: printing, O2: printing, sanitize: printing |
| proc | macOS arm64 | 0 | 0 | 0 | `proc: 0` | O0: proc stream, O2: proc stream, sanitize: proc stream |
| proc | Linux arm64 | 0 | 0 | 0 | `proc: 0` | O0: proc stream, O2: proc stream, sanitize: proc stream |
| procs | macOS arm64 | 0 | 0 | 0 | `procs: 0` | O0: procs, O2: procs, sanitize: procs |
| procs | Linux arm64 | 0 | 0 | 0 | `procs: 0` | O0: procs, O2: procs, sanitize: procs |
| proxy | macOS arm64 | 0 | 0 | 0 | `proxy: 12884901888` | O0: proxy, O2: proxy, sanitize: proxy |
| proxy | Linux arm64 | 0 | 0 | 0 | `proxy: 4294967296` | O0: proxy, O2: proxy, sanitize: proxy |
| scm | macOS arm64 | 0 | 0 | 0 | `scm: 0` | O0: scm, O2: scm, sanitize: scm |
| scm | Linux arm64 | 0 | 0 | 0 | `scm: 0` | O0: scm, O2: scm, sanitize: scm |
| serial | macOS arm64 | 0 | 0 | 0 | `serial: 0` | O0: net serial stream, O2: net serial stream, sanitize: net serial stream |
| serial | Linux arm64 | 0 | 0 | 0 | `serial: 0` | O0: net serial stream, O2: net serial stream, sanitize: net serial stream |
| service | macOS arm64 | 0 | 0 | 0 | `service: 0` | O0: service, O2: service, sanitize: service |
| service | Linux arm64 | 0 | 0 | 0 | `service: 0` | O0: service, O2: service, sanitize: service |
| stream | macOS arm64 | 0 | 0 | 0 | `stream: 0` | O0: stream, O2: stream, sanitize: stream |
| stream | Linux arm64 | 0 | 0 | 0 | `stream: 0` | O0: stream, O2: stream, sanitize: stream |
| stream_net | macOS arm64 | 0 | 0 | 0 | `stream_net: linked` | O0: net stream stream_net, O2: net stream stream_net, sanitize: net stream stream_net |
| stream_net | Linux arm64 | 0 | 0 | 0 | `stream_net: linked` | O0: net stream stream_net, O2: net stream stream_net, sanitize: net stream stream_net |
| sys | macOS arm64 | 0 | 0 | 0 | `sys: 29344` | O0: sys, O2: sys, sanitize: sys |
| sys | Linux arm64 | 0 | 0 | 0 | `sys: 11238` | O0: sys, O2: sys, sanitize: sys |
| syslog | macOS arm64 | 0 | 0 | 0 | `syslog: 0` | O0: syslog, O2: syslog, sanitize: syslog |
| syslog | Linux arm64 | 0 | 0 | 0 | `syslog: 0` | O0: syslog, O2: syslog, sanitize: syslog |
| term | macOS arm64 | 0 | 0 | 0 | `term: 0` | O0: stream term, O2: stream term, sanitize: stream term |
| term | Linux arm64 | 0 | 0 | 0 | `term: 0` | O0: stream term, O2: stream term, sanitize: stream term |
| time | macOS arm64 | 0 | 0 | 0 | `time: 260699079366458` | O0: time, O2: time, sanitize: time |
| time | Linux arm64 | 0 | 0 | 0 | `time: 23253498889136` | O0: time, O2: time, sanitize: time |
| tls | macOS arm64 | 0 | 0 | 0 | `tls: 0` | O0: net stream time tls, O2: net stream time tls, sanitize: net stream time tls |
| tls | Linux arm64 | 0 | 0 | 0 | `tls: 0` | O0: net stream stream_net time tls, O2: net stream stream_net time tls, sanitize: net stream stream_net time tls |
| ui | macOS arm64 | 0 | 0 | 0 | `ui: 1` | O0: net poll stream stream_net ui, O2: net poll stream stream_net ui, sanitize: net poll stream stream_net ui |
| ui | Linux arm64 | **2**, `internal error: linking failed:` | **2**, `internal error: linking failed:` | 0 | `` | O0: (no binary), O2: (no binary), sanitize: net poll printing stream stream_net ui |
| users | macOS arm64 | 0 | 0 | 0 | `users: 0` | O0: users, O2: users, sanitize: users |
| users | Linux arm64 | 0 | 0 | 0 | `users: 0` | O0: users, O2: users, sanitize: users |
| volumes | macOS arm64 | 0 | 0 | 0 | `volumes: 0` | O0: volumes, O2: volumes, sanitize: volumes |
| volumes | Linux arm64 | 0 | 0 | 0 | `volumes: 0` | O0: volumes, O2: volumes, sanitize: volumes |
| watch | macOS arm64 | 0 | 0 | 0 | `watch: 0` | O0: net stream watch, O2: net stream watch, sanitize: net stream watch |
| watch | Linux arm64 | 0 | 0 | 0 | `watch: 0` | O0: net stream watch, O2: net stream watch, sanitize: net stream watch |
| xattr | macOS arm64 | 0 | 0 | 0 | `xattr: 0` | O0: xattr, O2: xattr, sanitize: xattr |
| xattr | Linux arm64 | 0 | 0 | 0 | `xattr: 0` | O0: xattr, O2: xattr, sanitize: xattr |
| zall | macOS arm64 | 0 | 0 | 0 | `acl: 0 / activation: 0 / audio: 0 / crypto: 0 / disk: 0 / ip` | O0: acl activation audio crypto disk ipc keyring locale net odbc poll printing proc procs proxy scm serial service stream stream_net sys syslog term time tls ui users volumes watch xattr, O2: acl activation audio crypto disk ipc keyring locale net odbc poll printing proc procs proxy scm serial service stream stream_net sys syslog term time tls ui users volumes watch xattr, sanitize: acl activation audio crypto disk ipc keyring locale net odbc poll printing proc procs proxy scm serial service stream stream_net sys syslog term time tls ui users volumes watch xattr |
| zall | Linux arm64 | **2**, `internal error: linking failed:` | **2**, `internal error: linking failed:` | 0 | `` | O0: (no binary), O2: (no binary), sanitize: acl activation audio crypto disk ipc keyring locale net odbc poll printing proc procs proxy scm serial service stream stream_net sys syslog term time tls ui users volumes watch xattr |

**What it says**: the seats' built resolution was not built by me, so the *undeclared* column is the link the
principle leaves (no archive), and every family program meets it as a failed link, the new diagnostic's case. Of the
declared builds, **this Mac builds and runs all 31 at all three levels; Linux arm64 refuses `audio`, `ui` and `zall`
at `-O0` and `-O2` (libm: `sin`, `nextafter`) and builds them under `--sanitize`**, whose runtime brings libm; that
is R6's own refusal of correct programs, which a declared link that puts `-lm` beside the archive removes. What a
binary holds is the family's closure by calls, never the header's: `activation` holds `ipc`, `net` and `stream`
functions; `tls` `net`, `stream` and `time`; `ui` on this Mac `net`, `poll`, `stream`, `stream_net`.

### 6, continued: the planted header and defect 2788 (ran in the arm64 image; 2788 read)

- **`d-lane-layer-speed-1` under the base** (12:16:14 UTC; `run.sh`'s last step: `/usr/include/cups/cups.h` holding
  `#error a cups.h installed after the build`, `print` and `clock` built in a fresh folder, the file removed after):
  **`print` exit 2** and **`clock` exit 2**, each `internal error: the platform layer did not compile` /
  `/usr/include/cups/cups.h:1:2: error: a cups.h installed after the build`. So today a header one member probes
  stops `print(1)`. **Under the principle an undeclared `print` compiles no member** (it is undeclared, and an
  undeclared build asks the layer nothing), so the header stops it no more: an inference from the principle's
  definition, unrun, since no build of the resolution is mine. A declared `clock` under R6 whole is still stopped;
  under an archive of what the link needs it is not (the clock's closure is `hero_time` alone on every platform
  read); a declared **`ui` on Linux is stopped under either**, since its closure holds `hero_printing` (§ 4).
- **Who puts libm on a declared link**: `supplied_kept.alone_needs` records a member's undefined names when it is
  linked alone (`-nostdlib`), so `nextafter` and `sin` stand in `hero_ui_parts_gtk`'s and `hero_audio`'s needs (read,
  `supplied_kept.hero:219-226`), but the record judges collisions and puts nothing on a line; GNU ld reads no member's
  `.deplibs` (ran). So the compiler, on a GNU ld target, puts `-lm` on a declared link whose members need it; an
  undeclared link takes no member and needs none.
- **Defect 2788** (closed, read: *"On Windows every program fails to build: `hero_ui_parts_gtk.c` has no platform
  guard"*): R6's class, one member that does not compile stopping every program on a platform. Under the principle it
  stops declared programs only, and under an archive of what the link needs only those whose closure holds the
  member. **Unrun**: no Windows box in this seat's hands.

## Prediction, and what would change the verdict

- **prediction** (falsifiable at the landing, by `cmp` and the clang wrapper on this Mac and in
  `heroes-linux-arm64`): **an ordinary C binding owes the principle nothing and loses only the archive.**
  `examples/sqlite/main.hero` (`extern "sqlite3.h" link "sqlite3"`, no layer header), unchanged, builds and prints
  `rows: 3`, `longest: 6` on both, as it does under the base today (ran, 13:13:32 here, 12:15:15 UTC there); its
  `--emit-c` equals the base's but the line `hero_program_sources`; and its `link-words.rsp` equals the base's with
  `build/layer-<key>.a` taken out (today: `build/layer-206a1f4d6ce632b6-4c06ceee57b2a396.a -lsqlite3` here,
  `build/layer-33166f5a59d7b9a8-7ee86660507d7361.a -Wl,--start-group -lsqlite3 -Wl,--end-group` there), with no clang
  call naming `platform/`. And **with conditions 3 and 4 met**, door 3, `audio` and `ui`, each with the word, build at
  exit 0 at `-O0` in `heroes-linux-arm64`, where the base refuses all three today. Wrong if any of these differs.
- **what would change my verdict**: to **veto**, the sitting adopting `link "<the layer>"` with one library string per
  group, or reading *binds nothing* by the link (or by one platform's link) against a demand read by the text; to
  **approve without conditions**, the landing's own runs showing door 3, `audio` and `ui` linking on GNU ld,
  `owndeclwrong` refused before any link, and `print(1)`'s build asking the layer nothing on the three platforms the
  CI holds.

## What I ran and what I read, apart

**Ran** (this Mac, Apple clang 21, ld64; the arm64 image, Debian clang 22.1.8, GNU ld 2.44, ld.lld 22.1.8, lld-link;
the x86-64 image for the member calls alone): the base's compiler in two stages on this Mac and in the arm64 image
(`sources 41f32a8f4b745c0c` both); 23 door programs under the base on this Mac and 22 in the arm64 image, each link
replayed with the linker's map and without the archive; plain C for door 3's order on four linkers, `.deplibs` on two,
the archive loaded whole, an emission with and without a `link` word; 31 family programs at three levels on both arm64
platforms with their undeclared links; the member calls of every archive those builds wrote, and of the members
compiled alone in the x86-64 image; `otool -L` of the family binaries; the planted `cups.h`; SQLite on both. Every
script and output is in `.claude/worktrees/260-ffi-pragmatist-scratch/` (`doors.sh`, `families.sh`, `calls.sh`,
`famnolayer.sh`, `took.sh`, `readings.py`, `famtable.py`, `lnx/`, `x86/`, `mac/`, `calls/`).

**Read, not run**: step 182c's walk as code (its rule applied by `readings.py`, never the compiler's walk run as a
check); `cli/header_needs.hero` and `cli/library_needs.hero` beyond what door 3's Linux build printed; the spec-warden's
`w1` and `w5` drafts and the compiler-engineer's built route, from their reports (my door rows hold them to the
measured links; their binaries I did not run); the historian's items 12 and 13 on pkg-config and libtool (cited, not
asserted); defects 2788 and 2687's texts; panel 234's report and R6. **Unrun**: Windows and MSVC's `link.exe`
(no box), the Swift members' objects on any platform but this Mac, the x86-64 image's builds (members compiled alone
only), and any build of a resolution.
