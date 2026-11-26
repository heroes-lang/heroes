# Panel 260: a group that binds the platform layer says `layer`; a program none of whose groups says it links nothing of the layer; and there is no fallback

**Resolution adopted: provisional — author ratification pending.** Full panel: the compiler-engineer, the ffi-pragmatist,
the spec-warden, the historian, the completeness critic twice (on the briefs, then on the reports); the llm-ergonomist
registered and not run, its paid run authorised by the author (*ok a*, relayed at 10:37 on 2026-11-25) with a ceiling
of 20.00 USD (`260-briefs/llm-ergonomist.md`) and waiting, by its own registration, for a landed build to key it.
Convened 2026-11-25 at about 10:05 as a soundness-lane question, *how does a build choose and ask the platform layer's
members, so it costs what the program reaches, never refuses a correct program and never serves a stale archive?*, and
made a full panel before 10:33 by the author's instruction, which put the proposal below in its place. Briefs:
`docs/panel/260-briefs/` (written at `80e2e39ad`, repaired from the critic's first reading and re-frozen at
`c3885b81b`); reports: `docs/panel/260-reports/`, the seats' programs beside them under `programs/` (the
compiler-engineer's copied by the coordinator; the ffi-pragmatist's 57 source files copied from its finished copy by
this synthesis, `diff -r` printing nothing; the critic's sources, scripts and the result texts cited here, copied from
`.claude/worktrees/260-critic-scratch/r2/`). **Every cell below is at `c3885b81b`** unless it names `main`. The
synthesis's own runs are this Mac's, 2026-11-26 from 00:08, at load 107 to 172 (`uptime`), with the critic's two `-O2`
compilers copied into `.claude/worktrees/panel260-scratch/synth/bin/`: the base, `sources 41f32a8f4b745c0c` (its
runtime `diff -r` equal to the base's), and the route, `sources 2ac05d2a50ea1fb7`, the compiler-engineer's
`programs/compiler-engineer/route.patch` (sha256 `e0d5011b4b27feeaa517699dea3a23df0231db9940bdd61e095321b0d034f5e3`,
104 file diffs, 2,558 lines) applied to the base (`synth/probes.txt`, `synth/arms/`).

## The proposal, as put to the seats

**The author's principle, 2026-11-25** (`00-shared.md` § 1), meant as: *a library declares that it is native where it is
DEFINED, not where it is imported; a library that uses the layer and does not declare it is an error, never a fallback
that compiles everything; paying more when you use libraries that need the layer is fine, paying when you do not use it
is not.* Today panel 234's R6 puts the layer's archive on every program's link line: every linking build asks one
`clang -M` over the 43 C members (1.400 s of a warm `print(1)`'s 1.786 s in clang, `00-shared.md` § 4.1), so a warm
`print(1)` costs 5.1 to 6.5 times `17c50eba6`'s (`old`) and a cold one 11.7 to 13.6 times (§ 4.2, § 4.5). **The
question**: where the declaration lives and how it is spelled; what *binds the layer* means and what the check reads; the
diagnostic; what a declaration that binds nothing is; whether it travels; what a declared build pays; what the landing
owes the tree; and the spec's sentence.

## What the sitting found before judging, and beside it

- **The readings of *binds the layer* disagree, and no text reading agrees with the link.** Seven readings were listed
  (`00-shared.md` § 6.2); the ffi-pragmatist built 23 door programs and replayed each link with the linker's own map and
  without the archive, on this Mac and in Linux arm64: **only the link sees every door that needs a member, and nothing
  else**. The header name misses own headers, macros, function pointers and `platform/` headers and sees `constonly`;
  the walk's rule sees `ifnever` (a member's header under a `#if` no build takes) and `constonly`; the bound name misses
  macros, own headers and arms. And **what a link takes is what a reached module binds, never what the program calls**
  (`emit/extern_refs.hero`'s `used` table): `testonly` takes `hero_time` though `main` never reads the clock, and `face`'s
  `time.mark()` alone takes seven families (the critic's probes, `00-shared.md` § 6.2); design.md §1.11 line 505's
  *"only where the program calls them"* is, measured, *where a module the program reaches binds them*.
- **The link reading is right about which programs need a member and wrong about which bindings are checked.**
  `owndecl` (a header of the program's own declaring `hero_time_monotonic_ns` and including nothing of the layer) is
  checked by clang against the program's declaration, never the layer's: `owndeclwrong`, the same shape declaring the
  clock `double`, **builds under R6 and prints `6.3659874347e-314` at exit 0**, three runs alike (the ffi-pragmatist,
  this Mac; `1.26e-321` in Linux arm64, the critic). Door 7 (`owndecl`'s shape over a member that calls a Swift member)
  is **exit 2, *internal error: linking failed***, on this Mac under R6.
- **R6 refuses correct programs on GNU ld today.** A program's own C library whose object calls the layer (door 3,
  `extern "libnow.h" link "now"`) is refused in the Linux arm64 image with `ffi_missing_link`, *"the library `now` needs
  `hero_time_monotonic_ns`, and nothing on the link line defines it"*, **false**: the archive stands one word before
  `-Wl,--start-group -lnow -Wl,--end-group`, where GNU ld 2.44 has passed it when `libnow.a(now.o)` asks (the
  ffi-pragmatist, by the compiler and in plain C on four linkers: ld64, ld.lld and lld-link link that order, GNU ld does
  not). And at the base every program reaching `hero_audio` or `hero_ui` was *internal error: linking failed* at `-O0`
  and `-O2` on Linux, the members calling `sin` and `nextafter` with no `-lm` (both seats). **That half is repaired on
  `main` since the base**: step 189b (`1e712c573`, 17:24 the same day, merged `6211633e1`) makes the members compute
  their own maths in `runtime/platform/hero_layer_maths.h`, so *"no member needs a name the default libraries lack on
  either architecture at any level"*, libm put on no link line (its commit body, carried; nothing of this sitting ran
  over it).
- **`heroes check` reads no header** (the compiler-engineer, read and run; the critic, `grep -rln '^use
  cli/include_stands$' selfhost` naming `shadowed.hero` alone, and `shadowed` used by `produce.hero` and
  `package_stand_in.hero` only): `00-shared.md` § 4.5's carried *"that `heroes check` walks too"* is false at the base.
  Step 182c's walk is `build`'s, `run`'s, `test`'s and `--emit-c`'s, and it keeps one `seen` set per program
  (`include_stands.hero:81`, `entered` at 225), so it says what a program reaches, never what a group reaches.
- **Eleven `run` goldens bind the layer through a header of their own, not ten.** The critic's `run` form over the
  route found `the-platform-layer-never-takes-a-connect-that-failed-between-its-two-looks-for-one-made`, whose header
  writes `#include "platform/hero_net.c"` at its line 95: a member's C source, which every count of member *headers*
  missed (`own_deep.py`, the compiler-engineer's `land.py`).
- **Travel has a price.** Following `use` lines, 78 of Rebel's 115 faces and 735 of its 1,914 modules reach a member
  floor (the critic's `reach.py`, re-run by the repair); a program that only parses TOML text
  (`programs/compiler-engineer/toml/`) links **142 of the layer's functions in seven families** (`nm`, `T` symbols) it
  never calls, since `toml` uses `time` (the compiler-engineer).
- **R6 stops `print(1)` for a member's sake.** A `cups.h` planted where `hero_printing.c` probes for it makes `print` and
  `clock` exit 2, *internal error: the platform layer did not compile*, in Linux arm64 (the ffi-pragmatist, the lane's
  draft `d-lane-layer-speed-1` reproduced); defect 2788 (closed) was the same class on Windows.
- **An undeclared build's whole cache is keyed on every member's bytes.** One comment appended to
  `runtime/platform/hero_audio.c`, a member `print` never links, makes the next warm `print` 9 clang calls instead of 6
  and recompile `runtime.c` and both units: 3.22 s against 1.22 s here, 2.93 s against 0.43 s in Linux arm64 (the
  critic): `runtime_key.each_file` keys every file `runtime.c` includes by a quoted name, the never-read block's 67 among
  them. It does the same today.

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve with conditions**, built | design.md §1.1, §1.7 and Part 5 (the word is neither core nor sugar, a fact about the link like `link`), §1.12, §1.3 | about 400 lines of code: `check/layer_header.hero` 192 lines (91 of code), `cli/layer_bound.hero` 365 (188), +124 / −36 code lines in 22 files; emitter, IR, ownership, descriptors and lexer 0; a fifth top-level kind would touch 180 files | `latency.hero -- compare <base> <route> --clock build` on a still machine reads the route faster on the compiler's own warm build by a median of at least 0.5 s (its first run, at load 5 to 8: **no verdict**, exit 2) | to an objection: a blind measurement that readers do not write the word where it belongs, or a landing that lets `check` and `build` read *binds* two ways; to a veto: a declaration kind or an input class |
| ffi-pragmatist | **approve, on six conditions**; **veto** (V1) of `link "<the layer>"` while a group has one library string, (V2) of a demand read by the text with *binds nothing* read by the link | design.md §1.11, §3.5, §4.19 | none at the ABI: `HERO_RUNTIME_ABI` 32, every `hero_*.h`, the archive's name unmoved; a group's emitted C with and without a library word differs only in `hero_program_sources` | `examples/sqlite/main.hero` unchanged builds and prints `rows: 3`, `longest: 6` on both arm64 platforms, its `--emit-c` equal to the base's but `hero_program_sources`, its link words the base's without the archive; with conditions 3 and 4 met, door 3, `audio` and `ui` declared build at `-O0` in Linux arm64 | to a veto: V1 or V2 adopted; to an approval without conditions: the landing's runs showing door 3, `audio` and `ui` linking on GNU ld, `owndeclwrong` refused before any link, and `print(1)` asking the layer nothing on the CI's platforms |
| spec-warden | **approve `spec-w1-group-mark`, provisional**; **object** to the catalogue key (w3), the manifest (w4), the face line (w2f), the file name (w6) and the `link` word (w5) as homes | design.md §1.6, §1.3, §1.2; CLAUDE.md § Precedence as amended 2026-11-22 | `w1` +76 vendored (10,054 to 10,130), a lower bound, vendored only; real +95 to +103, an inference from the ledger's ratio; every home +66 to +114 | the landed `w1` reads 13,280 to 13,288 real on `measure --refresh`; under it D1 at least 4 of 5 on W1 and on W3, D1c at least 4 of 5 on W4 | D1 or D1c below 4 of 5; a check that cannot read *binds* and *binds nothing* by one rule; a shape the group's header never shows that must declare (a `link` library calling the layer, `dlsym`) |
| historian (advisory) | **approve** | precedent | none | *binds nothing* read at a grain other than the declaration's refuses correct packages, as Rust's `unused_crate_dependencies` would; checkable over Rebel's 115 faces | a tool that refused an unused native declaration and retracted it; Zig withdrawing its libc error; Go's pure-Go fallbacks chosen over an error for a reason that applies here; a JEP page weighing a library-side grant |

The completeness critic gives no verdict; its second reading (`critic-reports.md`) ran the compiler-engineer's patch,
whose key is none the seat reports (its Mac timings ran `4daaa8e64a9aab05`, its 13:00 snapshot, its Mac shapes a stage 2
of 13:02, and its Linux shapes and timings `5b8289a15e4c1b6e`, 13:23, all before `ffi_layer_redeclared`), and decides
where the built route is short (below). The llm-ergonomist's reading is registered (`260-briefs/blind/registration.md`), not run.

## The resolution adopted (provisional)

**R1, the home and its spelling: the word `layer` on the group head**, right after the header's string,
`extern "hero_time.h" layer`, before `link` or `pkgconfig`, which it does not replace (`extern "libnow.h" layer link
"now"`). It is the compiler-engineer's built form (`keywords.WORD_LAYER`, one constant; a contextual word, no lexer
change), the spec-warden's approved home and the ffi-pragmatist's condition 1 (a mark beside the library string, never
the library string itself), and **the only home an existing instrument holds**: the `grammar` suite's
`grammar/extern-marks` fails on `w1`'s production until the parser reads `WORD_LAYER` (the warden, run; *"`layer`:
quoted by the spec's `Extern`, and selfhost/parse/group_head.hero reads no `keywords.WORD_LAYER`"*), and nothing holds
`w2`'s or `w7`'s word (12 passed, 0 failed each, the warden). **Declared where it is defined**: on the group that binds,
in a floor, a program's own module or a root file alike. **It travels by the module graph, with no rule of its own**:
a build links the layer where a module it compiles holds a group saying `layer`, so a program importing a declared
package declares nothing and links the layer, and a package importing one declares nothing (the compiler-engineer's
`face`, `rebel/reach_*` and `toml`, run). `hero_os.h` and `hero_compiler.h` are the runtime's and owe nothing; the
compiler's own groups name only those two, so `selfhost/` never writes the word and **the seed owes nothing** (the
seed's stage 1 emitted the route's `selfhost/` at exit 0, 13:13, the compiler-engineer). **Needed for self-hosting: no.**

**R2, one reading of *binds the layer*, read on both sides, and what each verb tells.** A group binds the layer where its
header **is** a header of the layer (at `check`, by the group's string, before any header is read) or **reaches** a file of the layer at any
depth (at `build`, `run`, `test` and `--emit-c`, by step 182c's walk before the first clang, every `#if` but `#if 0`
read as taken, with the per-group edges the compiler-engineer added, +40 lines of `include_stands.hero`). Undeclared and
binding: `ffi_layer_undeclared`. Declared and binding nothing: `ffi_layer_unreached`. A name a layer header declares,
bound through a header that neither is nor includes it: refused in both spellings, `ffi_layer_redeclared` declared and
`ffi_layer_undeclared` at the link undeclared (the ffi-pragmatist's condition 2; `cli/runtime_bound.hero`'s precedent).
**Built and run**: the three codes (`grep -oE 'ffi_layer_[a-z_]+' route.patch | sort -u` prints three; the
coordinator's brief to this synthesis said four, and the fourth is R4's, unbuilt), five annotated cases (one `check`
with its `.fixed`, four `unsupported`; the harness's `check` 1, `unsupported` 4, `annotations` 5, `fixes` 1 passed, 0
failed, 17:01), and the compiler's own tests whole, **2,297 passed** (17:01 to 17:06), every guard of `00-shared.md`
§ 5 by its titles unchanged (the compiler-engineer). Under the patch, **`owndeclwrong` never prints its wrong value
and door 7 is never exit 2**: each is refused, exit 1, in both spellings (the critic, this Mac; `ffi_layer_redeclared`
in Linux arm64 too). That closes the ffi-pragmatist's two defects found beside (the landing carries them as cases).
`constonly` declared builds, prints `1` and takes no member, so no door is refused declared by a reading other than the
one that demanded the word: **V2 is not triggered** (the critic, `ifnever`, `ifplatform`, `dlsymboth1`). The price both
seats accepted for one reading: `constonly`, the tracked `fixedbugs-2696-...` (its `hero_poll_*` calls stand under
`#if defined(_WIN32)`), `ifnever` and, on this Mac, `ifplatform` say the word and take no member, a declared build
paying the archive for them.

**And, adopted unbuilt, where the critic's second reading found the built reading short** (each a false message, a
correct program refused or a `certain` fix that may be wrong, `blocking` by `.claude/rules/verification.md` § Bounded
discovery): **the layer's files are the runtime's own, never a name.** The reading is the list the runtime itself keeps,
`runtime_key.layer_members` and the headers `runtime.c`'s never-read block names before each member, found at those
paths in the runtime's folder (which `runtime_places.located` finds without clang, `cli/toolchain.hero:111`, read by the
compiler-engineer; whether `heroes check` asks it today is unread): a
member's header, an arm's header, a header or source under the runtime's `platform/`. So **a header of the program's
own named `hero_*.h` is not the layer's** (the critic's `ownnamed`, `hero_util.h` beside the program: refused under the
patch at `check` with *"`hero_util.h` is the platform layer's header"*, false, and a `certain` fix), and **a file under
the runtime's `vendor/` is not the layer's** (`vendorinc`, a header of the program's own including
`vendor/monocypher/monocypher.h` for a type's size: refused under the patch at `build`); both build undeclared as under
R6, and declared each is `ffi_layer_unreached`, truly. At `check` the name half tells a group only where its string
names one of the runtime's layer headers and no file of that name stands where clang would find it first; where one
might, the walk tells it at `build`, naming the file it found; **a fix is `certain` only where the file found is the
runtime's**, else `guess`.

**R3, door 3 builds when declared** (adopted unbuilt). A group whose `link` or `pkgconfig` library calls the layer binds
it: `extern "libnow.h" layer link "now"` builds, **and on a target whose linker reads an archive once (GNU ld) the
layer's archive stands inside the one library group** (`link.hero`'s `-Wl,--start-group`, lines 180 to 196 at the
base), where it resolves (plain C, GNU ld 2.44: exit 0 inside the group, exit 1 before it; the ffi-pragmatist). Under
the patch door 3 is **refused in both spellings of its own group** (undeclared at the link, declared
`ffi_layer_unreached`), its undeclared sentence *"this group's header puts it in the program's own code through a header
that includes none of the layer's"* is false (the name is referred to by `libnow.a(now.o)`), and its `guess` fix,
applied, is `ffi_unknown_name`, *"`hero_time.h` declares no `lib_now`"* (the critic, `extra/linklibfix`). So: an
undeclared door 3 is told at the link, at the group naming the library the linker says refers (`link_report.references`
already names `libnow.a[2](now.o)`), in words that say the library needs the layer, with the fix ` layer` before `link`
(`certain` where one group names that library); declared, it is never `ffi_layer_unreached`. The library's need is read
where the build reads a named library's objects before the link, so that **door 3 beside an unrelated declared group**
(`linkmixed`: built at exit 0 on this Mac only because another group says `layer`, and refused in Linux arm64 under the
patch with `library_needs`' false *"nothing on the link line defines it"*, the critic, 20:58 to 21:14 UTC) is told on
both platforms alike. Door 3' (the library's header including `hero_time.h`) already builds with `layer link "now"` under
the patch (the critic). This meets the spec-warden's third condition, *"a `link` library's object calling the layer ...
which a package-level home could say and a group mark cannot"*: the mark stands beside the library it is about.

**R4, a declared program whose link misses a layer name is told the truth** (adopted unbuilt), never
`cli/header_needs.hero`'s *"no group in this program says which library has it"*, nor `library_needs`'
*"nothing on the link line defines it"*. Under the patch `told_at_link` answers only an undeclared program
(`layer_bound.hero:212`, its first `if`), so the critic's `swiftentry` declared (`extern "platform/hero_ui_appkit.h"
layer`, a Swift member's C entry) falls in Linux arm64 to `header_needs`' false sentence (`ffi_missing_link`, *"the linker
cannot find `hero_ui_appkit_here`, and no group in this program says which library has it"*, plain and `-O2`). The new
reader is asked first for every program, declared or not; for a declared one it names the member that defines the name
and why this link holds none (a member this platform does not build, here). Its code is the landing's, the fourth beside
R2's three. The ffi-pragmatist's condition 3 is met by R3 and R4 together.

**R5, what each build pays: a declared build pays R6 whole; an undeclared one asks nothing of the layer.** Under the
patch an undeclared build makes no `-M` over the members, builds no archive and puts nothing of the layer on its link,
at every level: across the compiler-engineer's 78 undeclared builds `find -name 'layer-*.a'` reads 0; a warm `print` is
6 clang calls, none naming `platform/` (`old` 5, the base 7), 0.314 s in clang against the base's 1.513 s on this Mac
(the compiler-engineer, load about 90) and 0.559 s against 2.982 s in Linux arm64 (the critic). **Measured against
`old`, never 1**: warm, **1.11 to 1.38** of `old` by `real` on this Mac (`-O0`, `-O2`, `--sanitize`; the base 5.8 to
6.4) and **1.33 to 1.64** in Linux arm64 (the base 5.6 to 7.7), the compiler-engineer's at `4daaa8e64a9aab05` and
`5b8289a15e4c1b6e`; the critic's at the patch's key, **1.35** here by `user`+`sys` medians at load 58 to 116 and **0.95
(`real`) and 1.00 (`user`+`sys`)** in Linux arm64. Cold, **1.36 to 1.44** here and 1.39 to 1.54 there (the
compiler-engineer), **1.33** here and **1.85 to 1.93** there (the critic). What stands above `old` warm is the walk,
+0.06 s of `user`+`sys` carried from step 182c's body and never resolved at a still load (`00-shared.md` § 4.5: 8 of 12
pairs, a one-sided tail of 0.19); cold it is the runtime's own object, grown since `17c50eba6` (`runtime.c` 0.48 s
against 0.38 s, the compiler-engineer's `calls-cold.sh`). Taking the carried walk out gives 1.04 (`-O0`) and 0.93
(`-O2`) of `old` warm here by `user`+`sys`, an arithmetic on numbers of two sittings, an inference. A **declared** build pays R6 row for
row: the same members as the base for every shape (the compiler-engineer's table against
`ground/repair-members-taken.txt`; the critic, the ffi-pragmatist's 31 family programs declared under the patch taking
the R6 table's families), at **0.88 to 1.07** of the base by `real` here and 0.88 to 1.03 in Linux arm64 for `clock` (the
compiler-engineer), 0.90 to 1.04 (the critic). Every guard of `00-shared.md` § 5 stands on a declared build unchanged
(no assert of `layer.hero`, `reached_each.hero` or `opening.hero` moved; `layer.hero`'s one change is R6's own test's
title and doc, `route.patch` read); R6's own test (`layer.hero:330`, 332 in the patch) keeps its asserts, its *"every link carries the layer"* moving to a new test beside `layer_reach.for_build` (an
undeclared program's `for_build` gives no word and makes no `build/` file; a declared one's gives the archive).

**R6, the spec's sentence, rewritten from the warden's `w1` to say what R2 to R4 refuse**
(`260-reports/synthesis-adopted.txt` and `.diff`; § 13, line 401, merged into the paragraph and taking out its
*"linked where a group names one"*, which the principle falsifies; and the grammar, line 508):

> Anything beyond this document comes from packages or C libraries; processes and files from the language's
> `hero_os.h`, and sockets, clocks and the rest from its platform layer: `hero_net.h`, `hero_time.h` and every other
> `hero_` header it provides but the compiler's `hero_compiler.h`. A group whose header is one of these or includes a
> file of the layer at any depth, or whose library calls the layer, must say `layer` after its header, and no other
> group may; a name one of these declares is bound only by a group whose header is or includes it, and a build links
> the layer only where a module it compiles holds a group saying `layer`.

    Extern = "extern" string [ "layer" ] [ ( "link" | "pkgconfig" ) string ] NEWLINE

**Its count: claude-legacy 9,978, cl100k_base 10,155, a vendored delta of +101 (base 10,054): a lower bound, vendored
only** (`heroes measure` of the applied copy with the base's compiler from the scratch tree's root, 00:12, no
`--refresh`, none authorised; the base read 9,877, 10,054 and real 13,185 the same way, as the brief says). `w1` read
+76 the same way (its count reproduced, 10,130). **Real, about +126 to +137, an inference** from the ledger's ratio of
1.25 to 1.36 the warden read. Past `DELTA_GATE` (50): the landing commit's body says what paid, which is the registered
prediction below (the warden's payment rule, design.md §1.6, the named removal being *"linked where a group names one"*
alone, netted inside the count). Line 428 stays byte-equal (`diff` of the line). **Against the check, clause by clause**
(the critic's § 5 table, carried forward to the adopted text):

| clause | the resolution's check | the patch as built |
|---|---|---|
| *"every other `hero_` header it provides but the compiler's `hero_compiler.h`"* | the runtime's own list, found in its folder (R2) | the name `hero_<word>.h` wherever found: `ownnamed` refused; amended by R2 |
| *"includes a file of the layer at any depth"* | the walk, per group, every `#if` but `#if 0`; a member's source counts (the connect-order golden's `platform/hero_net.c`); `vendor/` never | the same, and `vendor/` the layer's: `vendorinc` refused; amended by R2 |
| *"or whose library calls the layer"* | R3 | door 3 refused in both spellings; amended by R3 |
| *"must say `layer` ... and no other group may"* | `ffi_layer_undeclared`, `ffi_layer_unreached` | built |
| *"a name one of these declares is bound only by a group whose header is or includes it"* | `ffi_layer_redeclared` declared, `ffi_layer_undeclared` at the link undeclared | built for a bound name; **not seen through a macro or a `static inline` body** (`mixedown`, filed below) |
| *"a build links the layer only where a module it compiles holds a group saying `layer`"* | `layer_header.declared` over the build; `Toolchain.layered` for the value probe | built (`face`, `toml`, `valueprobe`, the compiler-engineer) |

So the sentence says what R2 to R4 refuse, and one shape it forbids is refused by no check yet (`mixedown`); it says
nothing a reader needs about the verb at which each half is told (the name at `check`, the walk at `build`), which is
the diagnostics' to say. **Unrun on this text**: the `spec` and `grammar` suites (the warden ran both on `w1`, the
same production: `spec` 32 passed and 4 failed, the four the budget's records a landing moves; `grammar` 11 passed, 1
failed, `grammar/extern-marks` as above), and a real count. design.md §1.11 (lines 503 to 506, *"whose archive stands
on every program's link line"* and line 505's measured clause) moves with it; its words are the landing's.

**R7, what the landing owes the tree, counted** (CLAUDE.md § 2: nothing kept for an older spelling): **82 group heads at
least**, the compiler-engineer's 81 in 64 files (32 Rebel floors, `examples/server/net.hero`, 30 `run` goldens,
`tests/harness/rebel_apps_run.hero`) and the connect-order golden's (the critic: the patch leaves the `run` form red on
it, *"`the-platform-layer-connect-order.h` reaches the platform layer's `platform/hero_net.c`"*); the fetched case's
`sum` and folder key (`the-platform-layer-is-reached-from-a-fetched-packages-group.case/`, red under the patch,
`package_unread`); the four texts the harness compiles (`absence.hero:2778` and `:2779`, `thesis_cards.hero:269`,
`thesis_grade.hero:512`, which the patch does not touch, so the net's own tests would go red, read); the cases of R2 to
R4 (`ownnamed`, `vendorinc`, door 3 both spellings, `linkmixed`, `swiftentry`, `owndeclwrong`, door 7, and the critic's
`casefold`, whose base refusal `ffi_header_refused` is true and whose patch refusal `ffi_layer_redeclared` reads
*"binds it through `Hero_Time.h`, which includes none of the layer's headers"*, so the base's comes first); every tool
that re-prints a program (the formatter and `--dump-ast` built and run byte-identical by the compiler-engineer; owed:
the two colourers, the `grammar` suite's contextual words and `LEAST_MARKS` 3, the spec's grammar line, `heroes
mutate`'s view of a head, and a message naming the order for `layer` written after `link`, today
`expected_end_of_line`); then the whole `run` form, the net's own tests, the full net, Rebel's pipeline and the
latency instrument's `compare` on a still machine. As patched, the `run` form over `the-platform-layer` read **16
passed, 2 failed** (the critic, 17:55 to 18:14), and its `--emit-c` sweep of the 1,051 top-level `run` goldens and 54
`.case/` programs told the layer's class on exactly one, the connect-order golden.

## Not adopted

- **The other homes** (`00-shared.md` § 7.1; the warden's texts, every count vendored only): **`w5`, `link "layer"`**
  (+70): the ffi-pragmatist's veto V1, measured: a group has one library string, so door 3' (a program's own library
  whose header and object both use the layer) has no spelling but re-declaring the library's function in a header of its
  own, the unverified binding `owndeclwrong` measures; and `-llayer` reads as a library of that name. **`w3`, a key of
  the package's catalogue** (+99): the build's meaning moves to another file (design.md §1.3's refused category), it
  falsifies spec § 1's *"a program that uses no package has no catalogue"* and `.claude/rules/cli-surface.md:73-76`'s
  refusal of *"a library or package of spec § 13 ... a mode"*, and Rebel's packages carry no catalogue. **`w4`, a
  manifest per package** (+91): a fifth input class (`cli-surface.md`), refused on panel 056's conditions. **`w2f`, a
  line in the face module** (+114): the spec has no face. **`w2`, a line `layer` in the module that binds** (+83): well
  defined, but off the binding's line and held by no instrument (`grammar` 12 passed with no parser reading it). **`w6`,
  the floor's file name** (+74): `w3/main.hero` could hold no binding, Rebel's three `hero_os.hero` files and
  `math/hero_math.hero` would be refused, and a file's name is a declaration nobody reads as one; Go 1.11 asked people
  to rename `*_js.go` and `*_wasm.go` files when those names became constraints (the historian, the release notes).
  **`w7`, `use <layer>`** (+85): a `use` that binds no name against § 1, its pointer unpriced. **The importer's
  catalogue**: the opposite of *"where it is DEFINED, not where it is imported"*.
- **The inferred switch (S)**, the coordinator's refinement of the author's first proposal, *the mark read from the
  code*: the author's instruction before 10:33 replaced it. The compiler-engineer's strongest reason against the word,
  that a word held to one reading on both sides is an echo of what an inferred switch over that reading would compute
  (the same programs accepted and refused, about 100 lines less), is recorded; what the word buys over it is locality
  (design.md §1.3: `extern "myclock.h" layer` says on its line that every program reaching it links the layer) and a
  contract a runtime edit cannot move in silence (a runtime change making `hero_os.h` include a member's header would
  make every group over `hero_os.h` link the layer unsaid under the switch, and be refused at its next build under the
  word: the compiler-engineer's inference from the code, unrun).
- **The compiler-engineer's route exactly as built**: short in the four places R2's last paragraph, R3 and R4 name, and
  the conservative half of R2 (*"no correct program is refused both ways but one binding the layer through a
  declaration the layer did not write"*) is false as worded (door 3, the critic).
- **The narrowing routes for a declared build, each a sitting of its own**: **(a)**, the archive of what the program's
  headers reach: measured short, 9 member-to-member calls outside its closure on this Mac and 107 on Linux, the same
  from the archives real builds wrote at `-O0`, `-O2` and `--sanitize` and in the x86-64 image (the ffi-pragmatist), and
  a program's own header closure wrong for 8 of the 30 family headers here and 7 there; its lane's half that retries
  the link with the whole archive is the fallback the author refuses. **The reach cache**: **(b)**, the reach answered
  from the records of the files read, measured unsound (a `cups.h` written after a cold build makes the next build exit
  2, the lane's container run, carried); **(f)**, the reach re-validated by the search directories' times, rests on
  every file system moving a directory's time on every entry, unmeasured on APFS, ext4, overlayfs and NTFS. **(d)**, R6's
  archive served on its record and the link asked which members it took: serves an archive on a record and asks
  afterwards, the shape defect 2289 closed (an inference from the code, unrun). **(P2)**, one archive per machine:
  writes outside the folder a build starts in, which no build does today, and reopens defects 134's and 443's ground
  (read, unbuilt). **(P1)**, the author's per-platform archive shipped: unbuilt; what a member compiled against the CI's
  headers does where `cups.h`, GTK or glibc differ is unmeasured, and *"the CI is the instrument, and no binary is
  uploaded"* (`.claude/rules/records.md` § Release tags). **(j)** a declaration naming its families, **(k)** a shared
  layer per machine, **(g)** to **(i)**: unbuilt questions. Under the principle these routes price the declared minority
  only; what an undeclared build pays is R5's.
- **A declaration that binds nothing as a warning, or allowed**: one reading on both sides makes `ffi_layer_unreached`
  an error that refuses no correct program (V2 untriggered, above); the historian found no tool that refuses an unused
  native declaration and one that tried at the wrong grain (Rust's lint, off by default), and here the declaration and
  *binds nothing* are both a group's (the critic: the historian's prediction met by the built check's grain).
- **`w1` as worded**: the rival (below), false against the check on three clauses (the name read wherever found under
  the patch, `vendor/`, and `owndecl` declared, which `w1` says builds and the check refuses).

## Disagreements, stated plainly

1. **Which file is the layer's.** The patch: a name, wherever found, and anything under the runtime's `platform/` or
   `vendor/`. `w1`: *"it provides"*, no `vendor/`. **The synthesis takes the runtime's own list, by its paths** (R2),
   the side on which no correct program is refused and no message is false (the critic's item 1).
2. **Door 3.** The compiler-engineer read its refusal as the one intended (*"a declaration the layer did not write"*);
   the critic ran it as a correct, verified program refused in both spellings with a false sentence and a `guess` fix
   that does not compile; the spec-warden made a shape like it a condition of its approval. **R3 makes it build
   declared**, the robust side; conservative would have been the compiler-engineer's route with door 3's words and fix
   corrected and door 3' as its one spelling.
3. **Linux's links.** The ffi-pragmatist made the archive inside GNU ld's group and `-lm` conditions; the
   compiler-engineer would file the libm half beside the drafted `nextafter`. **`-lm` is moot on `main`** (step 189b:
   no member needs libm); **the archive's place is adopted** (R3).
4. **The author's *"seen by its writer and never by a program that imports it"***: built, an undeclared floor is told to
   its importer too, pointing into the floor (the compiler-engineer, `rebel/reach_*` and `face` undeclared, run), as any
   error inside a package is; a rule hiding it would be a fallback (the compiler-engineer). **The synthesis keeps the
   built behaviour** and reads the sentence as *seen by its writer first*. The author's word.
5. **Travel's price**: a program parsing TOML links R6 and 142 functions it never calls. The compiler's answer is that it
   uses a library that needs the layer, by what it links; whether the author's *"use"* means that is **the author's
   word**, with this number in hand (the decision's draft puts it).
6. **Paying for nothing**: `constonly`, `2696`, `ifnever` and `ifplatform` here pay the archive declared. Accepted by
   both seats as the price of one reading; the ffi-pragmatist's V2 bars the alternative.
7. **Whether the form pays under §1.2**: the warden, *"under §1.2 alone the form does not pay"* (a new mistake class,
   forgetting the word, each a round trip of about 3,619 tokens, its fix `certain`); rank 3, robustness, admits it, and
   no veto of the warden's holds against the most robust resolution (CLAUDE.md § Precedence, 2026-11-22).

## What a veto would compel

**No veto is exercised against the resolution.** The ffi-pragmatist's **V1** compels a mark beside the library string,
never `link "<the layer>"`: R1 is that mark. Its **V2** compels *binds nothing* judged by the same reading as the
demand: R2 is one reading, and the critic measured no door refused by the other. The compiler-engineer's veto would
fall on a fifth declaration kind (180 files of exhaustive matches) or a per-package file (a fifth input class), neither
adopted. The spec-warden holds no veto against the most robust resolution and reports its cost (+101 here, vendored).

## The conservative resolution

**R6 as it stands, no declaration** (the ffi-pragmatist's first conservative): every program pays the layer's `-M`
and cold archive, five to thirteen times `old`'s `print(1)`; a member that does not compile (defect 2788's class) or a
planted `cups.h` stops `print(1)`; `owndeclwrong` prints a wrong value at exit 0 and door 7 is exit 2 here; door 3 is
refused on GNU ld with a false sentence. **Or the principle with the header's name as the only reading** (its second,
and the compiler-engineer's, about 85 lines): every own-header program left to a refused link and told there, after
clang compiled every unit. **Or (S), no word** (the warden's, about +33 vendored): the author refused it. **Robustness
takes none of them**: the first keeps a wrong value at exit 0 and a false message the resolution closes; the second
tells the eleven goldens' shape late and leaves the readers' disagreement to the link; the third is what the author's
instruction replaced. What conservative would have been against R3 is in disagreement 2.

## The critic's list of what must not be asserted, held line by line

- *"No correct program is refused both ways"*: not asserted; door 3 is, under the patch (R3 adopts the repair, unbuilt).
- *"The text says what the check refuses"*: asserted only of the adopted text against R2 to R4, with `mixedown` named as
  the shape it forbids that no check refuses; `w1` and the patch disagree on three clauses (the table above).
- *"Every tracked program is declared"*, *"81 heads"*: not asserted; 82 at least, plus a `sum`, a folder key and four
  harness texts, and the `run` form red on two cases as patched (R7).
- *"An undeclared program pays nothing of the layer"*: asserted with its qualifiers only (R5): warm, the walk and no
  member; cold, a grown runtime object, 1.33 to 1.93 of `old` over four runs; and its cache cold after any member's edit.
  *"At `old`'s cost"* is an inference after a carried subtraction, warm only.
- *"The latency instrument shows the compiler faster"*: not asserted; it gave no verdict.
- *"Declared builds pay exactly R6 and never end at exit 2 on Linux"*: not asserted; at the base `audio`, `ui`, `zall`,
  `armhdr` and `platformhdr` end at *internal error* on Linux (repaired on `main` by step 189b, carried, not re-run over
  the route), and `swiftentry` declared is told `header_needs`' false sentence (R4).
- *"`owndeclwrong`'s wrong value is closed"*: asserted with its residue (`mixedown`, filed below).
- *"Seen by its writer and never by a program that imports it"*: not asserted of the built behaviour (disagreement 4).
- *"`heroes check` tells a header of the program's own that reaches the layer"*: not asserted; it tells the name half.
- *"The compiler-engineer's Linux table is of the patch"*: not asserted; it is of `5b8289a15e4c1b6e`.
- *Windows, MSVC, `link.exe`*: nothing asserted (below).
- *A prior-art line from the historian's commentary*: none; the decision's lines quote the pages, Zig as `Sema.zig` at
  0.14.0 states it, Java not cited.
- *The blind run's spend, or a D-rule outcome*: not asserted; registered only.

## Found beside, for the coordinator

- **`mixedown`, `blocking`**: a layer function a header of the program's own declares wrongly, reached through a macro
  or a `static inline` body, links whenever another group brings the layer and prints `6.3659874347e-314` at exit 0
  (this Mac, the base and the patch declared, 3 runs of 3 each, the synthesis, `synth/probes.txt`; Linux arm64
  `9.532824124368238e-130` and `2.902536355108114e-57`, plain and `-O2`, the critic). **Not this sitting's
  resolution**: R2 sees a bound name, not a body. The same cause, second symptom under the patch (run by the synthesis):
  door 1b `macrodecl` declared is `ffi_layer_unreached`, *"so nothing it binds is the layer's"*, false, with a `certain`
  fix that leaves it refused at the link. Draft: `.claude/worktrees/rebel-scratch/coord/issues/d-p260-mixedown.md`
  (`heroes run tests/harness/issues.hero -- draft` exit 0, with the base's compiler from the scratch tree,
  `synth/draft-mixedown.txt`).
- **Door 3's false sentence on GNU ld under R6** (the ffi-pragmatist; reproduced by the critic): a correct program
  refused on `main` today; R3 closes it at the landing. Unfiled; the coordinator's to file or to carry as the landing's
  case.
- **`owndeclwrong` and door 7 under R6**: closed by R2 at the landing (the critic re-ran both under the patch); they
  stand on `main` until then.
- **The cache key** (R5's last measurement): whether an undeclared program's runtime key should stop folding every
  member's bytes is a question, not a defect (the critic).
- **Seven member headers say no program includes them** (`hero_audio_coreaudio.h:12` and six more, the ffi-pragmatist);
  nothing refuses one, and under R2 each is the layer's, so `armhdr` declared builds. A question for the runtime.

## What is open and unrun

- **The landing itself**: R2's runtime list, R3, R4, the spec's text and every debt of R7; the full net, the net's own
  tests, Rebel's pipeline and the 43 `rebel/examples/` programs (3 emitted by the critic), the check over Rebel's 115
  faces (the historian's measurement).
- **Windows and MSVC**: every cell (no box). Defect 2788's class bounded to declared programs is an inference.
- **Linux x86-64**: the critic's every undeclared row (25 of 26 exit 1, none exit 2) and 10 of 26 declared rows, plain
  only, stopped at load 190; the rest unrun.
- **The latency instrument's verdict** on the compiler's own build: none (the compiler-engineer's run, exit 2, *"the
  machine was not still"*); and the walk's own cost at a still load.
- **Real token counts**: none; every count here is vendored.
- **What a declared program pays at run time** (defect 2687: `crypto` alone loads CryptoKit, Foundation, Security and
  nine Swift libraries at every start, `otool -L`, the ffi-pragmatist): unchanged by the principle; nobody timed a start.
- **The blind reading**: registered with its two arms (`registration.md` § What the synthesis added); it **waits**, by
  that file's own rule, for t2's six programs (the clerk's, unwritten) and for the landed build that keys t1's *built*
  grade and t2 on both graded platforms. The patch is not that build.
- **`mixedown`'s class**: the bodies' reading before the link, or the objects' undefined names by `nm`: unbuilt.

## Predictions to score

The seats' (table above), at the landing: the compiler-engineer's `compare` at least 0.5 s faster on the compiler's own
build on a still machine; the ffi-pragmatist's SQLite (held on this Mac under the patch by the critic: `rows: 3`,
`longest: 6`, `--emit-c` equal but `hero_program_sources`, link words equal but the archive) and door 3, `audio` and
`ui` declared at `-O0` in Linux arm64; the warden's real count, read for the adopted text by the registration's rule
(its `w1`, of the same home, the lines the adopted diff adds named), and its D1, D1c and D2. **The synthesis's own**:
the adopted text reads **13,311 to 13,322** real on `measure --refresh` (the base's 13,185 plus the inferred +126 to
+137); and **R2 to R4 are withdrawn by one correct program among `print`, `constonly` declared, `ownnamed`,
`vendorinc`, door 3 declared, door 3', `linkmixed`, the eleven goldens declared and the 31 family programs declared
refused by the landed build on this Mac or in Linux arm64, or by one undeclared program whose binary holds a
`_hero_<member>_` symbol** (`nm`).

## The blind reading, run 2026-11-26

Added the same afternoon, after this sitting's ratification (`f0b59c152`): the registered reading ran on the
author's authorisation of 2026-11-25 (*ok a*, ceiling 20.00 USD), its report `260-reports/llm-ergonomist-blind.md`,
the sessions' answers `260-reports/llm-ergonomist-blind-answers.tar.xz`, t2's task, the runner as it ran and the
registration's appended section (*Written 2026-11-26, after the landing*, 91 lines, none of the registered ones
changed) in `260-briefs/blind/`, and t2's six programs in `260-reports/evidence-blind.tar.xz` (paths kept). **31
sessions counted, 7.108556 USD of the 20.00** (the 31 cost 7.018539, one attempt a network outage cut and the runner
ran again 0.090017); none voided. **D1 holds** (W1 and W3 right for 5 of 5 `adopted` readers) and **D1c** (W4, 5 of 5);
**D2 fails**: 5 of 5 `adopted` readers write `extern "hero_os.h" layer` in W2, where at most 1 may; **D3 fails on one
program of six**, t2's (4), 0 of 5 right, for the same cause. `rival` reads exactly as `adopted` on every grade, so
nothing moves the resolution to `rival`. **By the registration's own rules the resolution stays on `adopted` and its
text owes a narrowing**: the sentence naming *"processes and files from the language's `hero_os.h`, and sockets,
clocks and the rest from its platform layer: ... every other `hero_` header it provides but the compiler's
`hero_compiler.h`"* is read by every reader as putting `hero_os.h` among the layer's headers, the one exception it
names being `hero_compiler.h`. **And the landed build does not tell them**: `extern "hero_os.h" layer` builds and runs
at exit 0 and `heroes check` passes it, on this Mac and in Linux arm64, while the text says *"no other group may"*: a
plausible mistake the build accepts, a defect found beside (class blocking). The warden's predictions hold but D2
(predicted 0 [0-1], measured 5 under both arms); the spend prediction (6.8 [4.5-10.0]) held. **What is owed, and is
this sitting's amendment, provisional, the author's reading pending**: a narrowed sentence, and the build's refusal of
a `layer` on a header that is not the layer's; neither is written yet. **The run's own limits, in its report**: t2's
task, programs and key did not exist before the run and were written first, under the registration's append-only rule;
no session could write `report.md`, each answer is the brief's fallback; the verdict session approved on a premise
the landed build disproves (that the `hero_os.h` doubt can only lead to a refused build).

## Author's verdict

**Pending.** The decision's draft is `docs/panel/260-reports/decision-draft.md`, filed with this sitting as `panel 260`.

**Ratified by the author's reading, 2026-11-26, at about 14:06**, in conversation (the answer *2a* to the
coordinator's recommendation, meant as: *ratify*). Landed in steps 197a to 197h (`1c90675b9` to `e322918ed`), on
`main` and pushed at `a60dfb77f`: 96 group heads in 77 files declare `layer`, and the critic's shapes `ownnamed`,
`vendorinc`, door 3, `linkmixed`, `swiftentry` and `casefold` read as the sitting adopted them; `mixedown` is not
closed by the sitting's reading and is defect 2856, open. **What the landing measured, with the machine loaded and so
a count and not a duration** (CL-025): a warm `print(1)` build 0.36 s before Rebel (`17c50eba6`) and 0.60 s now, cold
0.81 s and 1.24 s; the latency instrument's `compare` on a still machine is owed. The blind reading, authorised up to
20.00 USD, runs on the landed build and is scored beneath with its date.
