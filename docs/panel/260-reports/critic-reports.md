# Panel 260, the completeness critic: second reading (the seats' reports, before the synthesis)

Written 2026-11-25 from 17:30 on, as I go, by a fresh agent. My copy: the detached worktree
`.claude/worktrees/260-critic` at **`c3885b81b`** (`git log --oneline -1`, recreated 17:29, clean). Long output in
`.claude/worktrees/260-critic-scratch/r2/`. Load at the start: `uptime` 7.60, 5.01, 8.49 (17:30); no container running
(`docker ps`). No route is judged here; every line says **ran** (a command I ran, with it) or **read** (a file I
opened). Read first: `260-briefs/critic.md`, `00-shared.md` as repaired, the five seat briefs, `blind/registration.md`,
`blind/task-t1.md`, my first reading (`critic-briefs.md`), and the four reports (`compiler-engineer.md`,
`ffi-pragmatist.md`, `spec-warden.md`, `historian.md`) whole.

## Status

**Done about 22:20.** No route judged. Ran on this Mac and in both Linux images, one container at a time, each `--rm`,
named `p260-critic` (the x86-64 one stopped by me, § 12); none left (`docker ps`). No paid run, no commit, no push, no
GitHub write, no credential, no download, no file outside the repository's root, no `pkill` (two processes of mine
stopped by their PIDs, § 11 and § 12). Every script and output is under `.claude/worktrees/260-critic-scratch/r2/`.
This Mac's load rose from 7 at the start to 190 at 21:47 (other sessions), which is why the x86-64 run and one
emission were cut short.

## 1. The repair of the briefs, item by item against `critic-briefs.md`'s last section (read, and ran where it says)

| item | what I asked | repaired? | what the repair carried, checked |
|---|---|---|---|
| 1, the base moved | name the base; hand § A to every seat; list 182c's walk as a reading | **yes**: base `c3885b81b`, § 4.5, § 6.2's seventh reading, the CE's target counting the walk | § 4.5 carried *"that `heroes check` walks too is read only in the commit body (*carried*)"*. **It is false at the base** (the CE found it; ran by me: `grep -rln '^use cli/include_stands$' selfhost` names only `shadowed.hero`, and `^use cli/shadowed$` only `package_stand_in.hero` and `produce.hero`; `cli/check.hero` uses neither). So `heroes check` reads no header and the walk's half of any check is `build`'s, never `check`'s. § 4.5's own pairs did not resolve the walk's cost (8 of 12, tail 0.19), so the +0.06 s every seat subtracts is the commit body's, **carried**, never re-measured at a still load |
| 2, nine is ten | the tenth golden in § 3.1, § 6.2, the CE's item 4, the FFI's item 1, `shapes/` | **yes**, all five places; `shapes/` holds it with its two headers | **the repair carried my count, and my count was short**: an eleventh golden binds the layer through a header of its own that includes a member's **`.c`** (`#include "platform/hero_net.c"`), which my `own_deep.py` (member headers only) and the CE's `land.py` (the same reading) both missed (§ 8, § 11, run) |
| 3, the blind W1 | no state in W1; split the grader's table by file; a W4 over an unnamed header; register the runner's diff | **yes**: W1 is `mark()`/`elapsed_ns(since:)`; W4 over `hero_activation.h`; the table is per file; the three changes to `brief-session.md` written word for word | `task-t1.md` 2,849 bytes and its sha256 registered. The per-file table's cells under `adopted` and `rival` are the synthesis's, unwritten (registration § The grader's reading) |
| 4, the disagreeing shapes | `constonly`, `platformhdr`, `testonly`, `face`, `twofam` in `shapes/`; design.md line 505 measured | **yes** | held; § 6.2's sentence on line 505 is in |
| 5, (f) | correct the *"no door"* sentence | **yes**, § 7.5 (f) | held |
| 6, travel's count | 78 of 115, 735 of 1,914, 30 of 43 | **yes**, § 2 question 5, § 3.1 | re-run by the repair and equal (`ground/repair-reach*.txt`) |
| 7, what the principle reverses | panel 234's `link` condition; `cli-surface.md:73-76` | **yes**, § 3.2 | the FFI seat answered it (its report § *My seat's own condition*) |
| 8, the bodies' reading after a failed link | say it; ask the new diagnostic first | **yes**, § 5, § 7.2, § 7.3 | **silent on `cli/library_needs.hero`**, the second post-link reader, which tells door 3 *"nothing on the link line defines it"* (the FFI's finding 6) |
| 9, the historian's brief | pkg-config, libtool, Go file names, `import "C"` | **yes**, questions 10 and 11 | answered, each `verified` with quotes (§ 6 below) |
| 10, defects 2788, 2687 | name them | **yes**, § 8 | held |
| 11, unlisted readings and routes | `nm` before the link; (j); (k) | **yes**, § 6.2, § 7.5 | none built by any seat |
| 12, small corrections | `supplied*` span; `closure.py`'s `#if`; UTC; the range; § 4.4 carried; the predictions' rule | **yes**, each | `271` `"build/selftest-` literals at the base (ran: `grep -rhoE '"build/selftest-[^"]*' selfhost \| sort -u \| wc -l`), as § 4.4 says |

**Repairs that are themselves wrong**: two, § 4.5's carried *"`heroes check` walks too"* (above), and *"the ten"*, my
own first reading's number, which the repair copied into five places (eleven at least, § 8). And one the repair could not
have known: § 6.2 says the walk *"would see the ten and `ownhdr`"*; of a **program**, yes, of a **group**, no
(`include_stands.hero:81`, `Walk.seen` one set per program, and `entered` at 225 skips a file already seen; read by
me, `sed -n 78,84p` and `222,228p`), which the CE found and repaired with +40 lines of edges.

## 2. My compilers (ran)

- **The base**, in my copy: stage 1 by clang on the seed 17:30:36 to 17:30:56 (`real 20.50`), stage 2
  `./heroes build selfhost/main.hero -o heroes-next` 17:30:56 to 17:36:09 (`real 313.00`, `user 204.28`), `heroes
  --version`: `sources 41f32a8f4b745c0c`, **the base's key** (`r2/build-base.log`).
- **The route**: `git archive c3885b81b selfhost runtime seed rebel examples tests spec` into `r2/route/`, then the
  compiler-engineer's `route.patch` (sha256 `e0d5011b...f5e3`) applied with `git apply` (its `--check` passed; a first
  `git apply` run inside the repository's tree applied nothing in silence, since git ignores paths outside the folder
  it runs in, so it ran again with `GIT_CEILING_DIRECTORIES` set: `layer_header.hero` 192 lines and `layer_bound.hero`
  365, the CE's counts); stage 2 by the base's stage 2, 17:36:17 to 17:42:53 (`real 395.70`), `sources
  2ac05d2a50ea1fb7` (`r2/build-route.log`). **This key is none the CE reports**: its Mac timings ran `4daaa8e64a9aab05`
  (its 13:00 snapshot) and its Linux shapes and timings `5b8289a15e4c1b6e` (13:23 sources), both before the 17:00
  `ffi_layer_redeclared` change. So the CE's Linux table, and its Mac shapes table (13:02), are of a compiler that
  is not the patch; what I ran below is the patch.

## 3. Four shapes of my own beside § 6.2, undeclared and declared, base and route (ran, this Mac, plain)

Sources in `r2/mine/` (as written) and `r2/mine-d/` (declared), `r2/run.sh` (each built in its folder, run under
`timeout 20`, families read by `nm`, then `check` and `--emit-c`); outputs `r2/res-base-mine*-asis.txt`,
`r2/res-route-mine-asis.txt`, `r2/res-route-mined-asis.txt`; 17:36 to 17:45, load 7 to 73.

| shape | what it is | base | route, undeclared | route, declared |
|---|---|---|---|---|
| `ownnamed` | a header **of the program's own** named `hero_util.h`, beside it, a `static inline` returning 42; nothing of the layer | 0, prints `42`, no member | **1 at `check`**: *"`hero_util.h` is the platform layer's header, and this group does not say `layer`"*, **false**, a `certain` fix | 0, `42`, **no member, and a layer archive built** |
| `vendorinc` | a header of the program's own including the runtime's `vendor/monocypher/monocypher.h` for a type's size; nothing bound or called of the layer | 0, `true`, no member | `check` 0; **1 at `build`/`--emit-c`**: *"`sizes.h` reaches the platform layer's `vendor/monocypher/monocypher.h`"* | 0, `true`, **no member, an archive built** |
| `mixedown` | one group over `hero_net.h` (its constant); a second over a header of the program's own that declares `double hero_time_monotonic_ns(void)` (wrong) and reaches it by a macro `now_ns` and a `static inline` | 0, prints `1`, **`6.3659874347e-314` twice**, time 5 | 1 at `check` (the `hero_net.h` group) | **0, prints `6.3659874347e-314` twice, exit 0**, time 5: the FFI seat's `owndeclwrong` value, through a macro and an inline body beside a declared group |
| `swiftentry` | `extern "platform/hero_ui_appkit.h"` binding `hero_ui_appkit_here`, a Swift member's C entry | 0, prints `1`, ui 58 | 1 at `check` (by name) | 0, `1`, ui 58 |
| (`casefold`) | `extern "Hero_Time.h"` | **1**, `ffi_header_refused` (clang's non-portable path): not a correct program, kept as a probe | the same | 1, `ffi_layer_redeclared` *"binds it through `Hero_Time.h`, which includes none of the layer's headers"* (false words; the base refuses it anyway) |

**What they say** (each from the rows):
- **The route's *binds* is not the text's.** `ownnamed` and `vendorinc` bind nothing of the layer, build under R6 with
  no member, and under the route are refused undeclared (the first with a false sentence and a `certain` fix) and,
  declared, build an archive their link never takes. The rule reads a header's **name wherever the file is found**
  (`layer_header.names_layer`, `check/layer_header.hero:112`, takes the string alone) and the walk calls **any file
  under the runtime's `vendor/`** the layer's (`layer_bound.layer_path`, its doc lines 15 to 19). The spec-warden's
  `w1` says the opposite of both: *"every other `hero_` header **it provides**"* (its own table: *"a header of the
  program's own named `hero_mine.h`: not the layer's ... a question, unrun"*), and it names no `vendor/` file. So on
  these two shapes the text and the built check disagree, and the checkable side is the build's (run above).
- **The residue the CE named is a measured wrong value, not an inference.** `mixedown` declared prints
  `6.3659874347e-314` at exit 0 under the route, as it does under the base: the CE's repair (`ffi_layer_redeclared`)
  reads only a **bound name** a layer header declares; a macro or an inline body reaching a layer function through a
  declaration the program wrote is unseen as soon as another group says `layer`. The FFI seat's condition 2, worded
  on *"a layer name bound through a declaration the program wrote"*, does not reach it either. Not a regression (R6
  prints the same), and not closed.

## 4. The FFI seat's doors under the CE's built check (ran, this Mac, plain, the patch's compiler `2ac05d2a50ea1fb7`)

The doors' sources are **not** under `panel260/docs/panel/260-reports/programs/`, where the coordinator's prompt puts the
seats' programs: only `compiler-engineer/` is there. I copied the FFI seat's `programs/ffi-pragmatist/` (sources only,
`cp -R`, read) from its finished copy into `r2/ffi/`, building nothing there; door 3's `libnow.a` is built from its
`lib/now.c` against the compiler's runtime and linked with `--library lib --include lib`, as its `doors.sh` does.
`r2/run.sh` over them: *undeclared* as written, *declared* with ` layer` after every group's header (the CE's own
`shapes.sh` rule), 17:45 to 17:53, load 26 to 40 (`r2/res-route-ffi-undeclared.txt`, `-declared.txt`); then two
spellings by hand, `r2/extra/` (`res-route-extra-asis.txt`).

| door | correct by the FFI seat | route, undeclared | route, declared | what the route tells, checked |
|---|---|---|---|---|
| `clock`-shaped, 2b `fnvalue`, the 31 family programs | yes | 1 at `check`, each | 0, each; the family members **equal the FFI seat's R6 table** (`activation` takes ipc, net, stream; `tls` net, stream, time; `ui` net, poll, stream, ui 301; `zall` all 29 families) | true |
| 1 `macro`, 2 `fnptr`, 5 `ifnever`, 5b `ifplatform`, 3' `linklibhdr`, 6 `armhdr` | yes | `check` 0; 1 at `build` and `--emit-c` (*"`<own>.h` reaches the platform layer's `hero_*.h`"*) | 0; `ifnever` and `ifplatform` take **no member** here, an archive built for nothing (the walk's loud direction, as both seats said) | true |
| 4 `dlsym` | yes | 0, prints `-2`, as R6 | 1 `ffi_layer_unreached` (rightly: its group binds nothing of the layer) | true |
| 4b `dlsymboth` | yes | 1 at `check` | `layer` on every group: 1 `ffi_layer_unreached` at `dl.h`'s; **`layer` on the `hero_time.h` group alone** (`extra/dlsymboth1`): 0, prints `true`, `1`, time 5 | true |
| `owndecl`, **`owndeclwrong`**, **7 `owndeclswift`** | unverified bindings | `check` 0, `--emit-c` 0; 1 at the link, `ffi_layer_undeclared` at the binding (*"binds it through a header that includes none of the layer's"*) | 1, `ffi_layer_redeclared` before any clang | **`owndeclwrong` never prints `6.3659874347e-314`; door 7 is never exit 2 on this Mac** (each told, exit 1, in both spellings) |
| 1b `macrodecl` | unverified | 1 at the link, the same words as door 3 below | 1 `ffi_layer_unreached` | refused both ways, rightly by the FFI seat |
| **3 `linklib`** (`extern "libnow.h" link "now"`; `libnow.a`'s object calls the clock) | **yes** | `check` 0, `--emit-c` 0; 1 at the link: *"`hero_time_monotonic_ns` is the platform layer's ... and **this group's header puts it in the program's own code** through a header that includes none of the layer's"*, note *"a function of the layer's declared by the program itself is checked against nothing the layer wrote"*, fix `guess` *"name the layer's header and say `layer`"* | 1, `ffi_layer_unreached` | **refused in both spellings of its own group, and the undeclared sentence is false**: the name is referred to by `libnow.a(now.o)`, not by the group's header or the program. **The fix, applied** (`extra/linklibfix`, `extern "hero_time.h" layer link "now"`): 1, `ffi_unknown_name`, *"`hero_time.h` declares no `lib_now`"*. Its one spelling that builds is door 3' (the library's header including `hero_time.h`) |

**What this settles for the coordinator's questions**, each from the rows:
- **The FFI seat's two defects are closed by the built check on this Mac**: `owndeclwrong` and door 7 are refused in
  both spellings with a told error, never a wrong value at exit 0 and never exit 2 (the base: `6.3659874347e-314`
  at exit 0, door 7 exit 2, the FFI seat's runs).
- **`constonly` and the one-reading rule hold** (the CE's table; `ifnever`, `ifplatform` and `dlsymboth1` here): no
  door is refused declared by a reading other than the one that demanded the word, so the FFI seat's **V2 is not
  triggered**. Its **V1** (`link "<the layer>"`) is not what was built: the word is `layer` beside the library string,
  and door 3' builds with both (`layer link "now"`, run above).
- **The CE's claim *"no correct program is refused both ways but one binding the layer through a declaration the layer
  did not write"* is false as worded**: door 3 is a correct, verified program (its library compiled against
  `hero_time.h`) refused undeclared and declared. Its condition 3 (*"the group placed by those readers and its fix
  naming the layer's header"*) is met for the placement and **not for the words or the fix**: `told_at_link`
  (`layer_bound.hero:212`) takes every reference the link reports whoever refers, and where no group binds the name it
  falls to `first_own_group` and the sentence *"this group's header puts it in the program's own code"*; the reader
  that knows a library refers (`link_report.references` already names `libnow.a[2](now.o)`) is not asked who.
- **Three of my own shapes and two doors pay the layer for nothing when declared** (`ownnamed`, `vendorinc`, `ifnever`,
  `ifplatform` here, `constonly` and `2696` in the CE's table): the price both seats accepted for one reading.

## 5. The spec-warden's `w1` against what the built check refuses, clause by clause (read `spec-w1-group-mark.txt`, its diff sha256 `af4718cc...83c8` as registered; each row's run named)

| `w1`'s clause | the built check (`layer_header.hero`, `layer_bound.hero` of the patch) | agree? |
|---|---|---|
| *"sockets, clocks and the rest from its platform layer: `hero_net.h`, `hero_time.h` and every other `hero_` header **it provides** but the compiler's `hero_compiler.h`"* | `names_layer`: **any** group string `hero_<word>.h` (no folder) but `hero_os.h` and `hero_compiler.h`, or `platform/hero_<word>.h`, a leading `./` passed over, **wherever clang finds the file**; at `build`, any file under the runtime's `platform/` or `vendor/` | **No** on two shapes, run (§ 3): `ownnamed` (the program's own `hero_util.h`) is the layer's for the build and not for the text; `vendorinc` (a header reaching `vendor/monocypher/monocypher.h`) the same. The text names no `vendor/`, and its own table calls `hero_mine.h` *"a question, unrun"*: run, the build refuses it undeclared |
| *"A group whose header is one of these or includes one at any depth"* | the walk, `build` only, every `#if` but `#if 0` true | yes (`ifnever`, `ifplatform`, the ten: both say *must*); the text says nothing of the verb, and **`heroes check` reads no include** (§ 1), so the text's *"at any depth"* is told at `build` and `--emit-c`, never at `check` |
| *"or that binds a name one declares, must say `layer`"* | undeclared: told **at the link** (`check` 0, `--emit-c` 0); **declared: refused, `ffi_layer_redeclared`** | **No**: the text says `owndecl` declared is right (its table: *"builds"*); the build refuses it in both spellings (§ 4, `owndecl`, `owndeclwrong`, door 7), which is the FFI seat's condition 2 and the CE's design. The CE's own proposed sentence carries the clause the text lacks: *"A function of the layer is bound through the layer's header"* |
| *"and no other group may"* | `ffi_layer_unreached` where the string is not the layer's by name, the walk reaches nothing and no bound name is a layer's | yes for `sqlite3.h`, `hero_os.h`, `dl.h`, door 3 (§ 4); **no** for `ownnamed`, which the text says may not and the build says must |
| *"a build links the layer only where a module it compiles holds such a group"* | `layer_header.declared(decls)` over the whole build; `Toolchain.layered` for the value probe | yes (the CE's `face`, `toml`, `valueprobe`; my `mixedown` declared) |
| grammar `[ "layer" ] [ ( "link" \| "pkgconfig" ) string ]` | the word after the string, before `link`; after it `expected_end_of_line` (CE, ran) | yes; door 3' builds with `layer link "now"` (§ 4) |

**So `w1` does not yet say what the built check refuses** on three clauses (the name read wherever found, `vendor/`,
and the binding through a declaration of the program's own). Either the text gains them (+? tokens, unpriced) or the
check moves to the text (*"it provides"*: the name rule asked where the file was found, as `layer_path` already asks
of the walk's files). Which, is the synthesis's; the blind run's `adopted` arm is the text, so its t2 key (the landed
build) and its text must agree before any session, or a reader who follows the text is graded against a build that
refuses what the text allows (registration § The key).

**The predictions against the registration's rules** (read `spec-warden.md` § Predictions, `blind/registration.md`
§ How a prediction is read, `llm-ergonomist.md` § The decision rules):
- They are numbers and ranges per rule and per text, for D1 (W1 and W3 apart), D1c, D2, D3 and D4, under `adopted`
  and under `now`, and the spend; D1b is a report and is not predicted, which the registration allows.
- **D3 is predicted in another unit than the rule's**: the registration's D3 is a gate on **each** of t2's six (at least
  4 of 5 build-right on every one, and at most 1 naming the importer's file); the seat predicts *"the number of t2's
  six programs on which at least 4 of 5 readers are build-right"*, `5 [4-6]` under `w1`, a range of which only 6
  passes the gate. Readable (the registration asks *"a number and a range per rule"*), but a reading that lands at 5 is
  *"right"* as a prediction and a failed D3 as a gate; the synthesis should say so before the run.
- **D4** is defined by the seat (*"the median, over a t1 arm's five readers, of the spec's sections a reader's
  `choice_points` cite"*); the registration leaves D4's count to the reading. One grader can count it as written.
- The registration reads an arm against **the seat's text whose diff is byte for byte the arm's**: the ten diffs'
  sha256 in the seat's table are the files' (ran: `shasum -a 256 spec-w*.diff`, all ten equal). If the synthesis adds
  any clause to `w1` (above), the arm is read against `w1` *"with every line the arm's diff has and `w1`'s does not
  named"*, not against `w1` itself.
- The spend, *"6.8 USD [4.5-10.0]"*, rests on panel 257's measured 6.30 for 31 and is an inference.

## 6. The historian: does every `verified` precedent quote its page? (read; no page re-read by me, I hold no web)

Read `historian.md` whole. Sixteen precedents marked `verified`; **each carries at least one quotation in the page's
own words with the page's URL**, and the report says every page was read 2026-11-25 (its line 3 and § search). The
sentences a page did not carry are marked `unverified` in place (Cargo's silence on an undeclared `#[link]`; Go's
*"a program importing no cgo package pays none"*; Zig's first version and issue; Dart's and Flutter's check; the
SwiftPM *"linked automatically"* mechanism; Cabal's mechanism; the reproc issue, read as a snippet). Where it is
weaker than its heading says:
- **Java** is headed *"verified through Oracle's guide; the JEP pages unread"* (403): the JEPs' own words are
  unverified, and the *why* rests on two secondary pages (nipafx, InfoQ). A `## Prior art` line may cite Oracle's
  guide, not JEP 472.
- **Zig**: the error's text is quoted from a forum post and from `Sema.zig` at tag 0.14.0 (source code, not a
  document); *"the one precedent found where an undeclared use of the platform is a compile error"* is the
  historian's reading of the function, and its scope (libc, libc++, libunwind by library name, never a symbol) is
  stated. That it *reached first* what Heroes reaches is a comparison, which `.claude/rules/records.md` § And the
  record says whose idea it was forbids a line to assert beyond the page.
- **pkg-config's guide** is dated *"2010"* in the report, read from Debian's copy; the copy's page is the source, the
  date is not quoted.
- Each *"What it says"* paragraph is the historian's commentary, labelled as such, not the page's: a prior-art line is
  written from the quotations above it, never from those paragraphs (the rule's 33-of-90 lesson).

The historian's falsifiable prediction (*binds nothing* read at a grain other than the declaration's refuses correct
packages, as Rust's `unused_crate_dependencies` would) is **met by the built check's grain**: the declaration and
*binds nothing* are both per group, and no face holds a group (the CE's `face` declared builds). **Unrun by anyone**:
the check over Rebel's 115 faces, which the historian names as its measurement; the CE's `land.py` declared the 32
floors and the CE built `face`, `rebel/reach_*` and `toml` over them, not the 115.

## 7. Does an undeclared program pay nothing of the layer, warm and cold? (ran)

**Linux arm64, one interleaved row of my own** (`heroes-linux-arm64:latest`, image `338a1044ac90`, container
`p260-critic`, `--rm`; the three compilers built inside from C by `clang -O2 ... -lm -lpthread -ldl`, in parallel:
`old` from `perf/old/seed/heroes.c` (`cmp`-equal to `git show 17c50eba6:seed/heroes.c`), `sources 5aae00294f7cd6fa`;
the base from my stage 2's `--emit-c` (40,396,174 bytes, the CE's figure), `41f32a8f4b745c0c`; the route from the
patch's stage 2's `--emit-c` (40,559,146 bytes), `2ac05d2a50ea1fb7`; `r2/lnx/out/log.txt`). The CE's
`run-linux.sh` time phase unchanged but for the compiler list (`r2/lnx/ce-run-linux.sh`, `PHASE=time LEVELS=O0
PROGS=clock`), 17:06 to 17:10 UTC, container load 4.1 to 4.6, this Mac at load about 40 (other sessions), read by
the CE's `read_times.py`; `r2/lnx/results.txt`:

| program | mode | `old` | base | route | route / `old` (`real`; `user`+`sys`) | route / base |
|---|---|---|---|---|---|---|
| `print` | warm | 0.40 (0.32) | 2.40 (2.13) | 0.38 (0.32) | **0.95; 1.00** (median `user`+`sys` 0.45 against 0.32) | 0.16; 0.15 |
| `print` | cold | 1.07 (0.99) | 12.05 (11.21) | 2.06 (1.83) | **1.93; 1.85** | 0.17; 0.16 |
| `clock` (declared for the route) | warm | refused | 2.74 (2.38) | 2.74 (2.48) | | 1.00; 1.04 |
| `clock` | cold | refused | 12.77 (11.86) | 11.43 (10.77) | | 0.90; 0.91 |

Against the CE's Linux row (warm 1.50 / 1.38, cold 1.51 / 1.48, at a container load of 2): **the warm claim holds
within a range** (0.95 to 1.50 by `real` over two runs, the walk inside it), **the cold one is wider than the CE
reports** (1.51 to 1.93), and in no run is an undeclared cold `print` at `old`'s cost: the runtime's own object, grown
since `17c50eba6` (the CE's reading, `calls-cold.sh`), is compiled per cold folder. **A declared `clock` pays R6**
(0.90 to 1.04 of the base), as the CE measured.

**What an undeclared build still carries of the layer** (this Mac, the patch's compiler, a private copy of it and of
its runtime in `r2/touch/`, clang wrapped to log each call, `r2/touch-before.log`, `touch-after.log`): a warm `print`
makes **6 clang calls, none naming `platform/`, no `-M` over the members, no archive** (`build/` holds `runtime`,
`tu`, `openings`, `reads`, `shadowed`, `words`, `package-reads`, nothing `layer-`), 1.22 s in clang at load about 40.
Then **one comment appended to `runtime/platform/hero_audio.c`**, a member `print` never links: the next build makes
**9 calls, 3.22 s, recompiling `runtime.c` (1.85 s) and both units** (`print.c`, `library.c`). So an undeclared
program's whole cache is keyed on every member's bytes: `runtime_key.each_file` keys `runtime.c` and **every file it
includes by a quoted name** (`selfhost/cli/runtime_key.hero`, its doc and `each_file`, read), the 67 of the never-read
block among them (`00-shared.md` § 4.1). **Nobody measured this**; it is *"nothing of the layer"* warm, and not cold
after any change to a member (each runtime edit, a toolchain upgrade's, makes every undeclared program in the net
recompile its runtime object and units, which it does today too). Whether the runtime's key should leave the layer's
files to the layer's own key is a question for the synthesis, not a defect I file.

## 8. The `run` form over the declared goldens, which the CE left unrun (ran, this Mac, the patch's compiler)

`./heroes-route run tests/harness/main.hero -- ./heroes-route run the-platform-layer` from the patched tree's root
(`r2/route/`, made a git tree of its own by `git init` so the provenance hook keys it: it accepted the compiler as the
tree's, `2ac05d2a50ea1fb7`), 17:55 to 18:14, load about 40, `real 1166.95`, `user 400.69` (`r2/golden-run-1.txt`):
**`run (only the-platform-layer)`: 16 passed, 2 failed.**
- **`the-platform-layer-is-reached-from-a-fetched-packages-group`** (the `.case/` the CE named): `error[package_unread]`,
  *"the tree placed at ... `stopwatch@1.0.0-d9946aad55e71455/` is `sha256-8a825e07...`, and the catalogue's `sum` for it
  is `sha256-2ed7d1c7...`"*. The patch edits the placed package's file and neither its `sum` nor its folder's key, as
  the CE said it owed (*"unrun"*): now run, red.
- **`the-platform-layer-never-takes-a-connect-that-failed-between-its-two-looks-for-one-made`**, which **no seat and no
  brief counted**: `error[ffi_layer_undeclared]`, *"`the-platform-layer-connect-order.h` reaches the platform layer's
  `platform/hero_net.c`"*. Its header (step 87b, `f29d2cb7f`) writes `#include "platform/hero_net.c"` at line 95 (read,
  `grep -n`): it compiles a **member's C source** into the program's unit. My first reading's `own_deep.py` followed
  includes to a member's **header**, and the CE's `land.py` recomputed the same list, so both missed it. So **the ten
  are eleven** (at least: § 9 counts the rest), the CE's *"81 heads in 64 files"* is short, and the landing as patched
  leaves the `run` form red on two goldens of this one filter.

## 9. Linux arm64: my shapes and the FFI seat's doors under the patch (ran, the same container, 17:10 to 17:25 UTC)

`r2/lnx/critic-linux.sh` (phases `mine`, `doors`, `calls`, `touch`), `r2/run.sh` inside; outputs `r2/lnx/out-arm64/`
(`res-*.txt`, `calls.txt`, `touch.txt`). Debian clang 22.1.8, GNU ld 2.44.
- **My shapes**: as on this Mac, `ownnamed` and `vendorinc` are refused undeclared (`check` for the first, `build`
  for the second) and build declared taking **no member**; `mixedown` declared prints **`9.532824124368238e-130` and
  `2.902536355108114e-57` at exit 0** (another wrong value per platform, plain and `-O2`), the base the same.
  `swiftentry` (the AppKit entry) is refused on Linux by the base and, declared, by the route with
  **`ffi_missing_link`, *"no group in this program says which library has it"*** (`cli/header_needs.hero`'s
  sentence): a **declared** program whose link misses a name a layer header declares falls past `told_at_link`, which
  answers only an undeclared program (`layer_bound.hero:212`, its first `if`), to the reader both seats called false.
- **The doors undeclared**: every door that needs a member is refused at exit 1, never exit 2; door 3 with the same
  false sentence as on this Mac; `dlsym` builds (`-2`).
- **The doors declared**: `fnptr`, `fnvalue`, `macro`, `linklibhdr` (3'), `ifplatform` (time 5 here, none on this
  Mac) and 27 of the 31 family programs build and take the FFI seat's members; **`armhdr`, and the `audio`, `ui` and
  `zall` family programs end at exit 2, `internal error: linking failed:`** (the members' `sin` and `nextafter` with no
  `-lm`, the FFI seat's and the CE's finding); `owndeclwrong` and door 7 are refused (`ffi_layer_redeclared`).
- **The base, same container**: `owndeclwrong` prints **`1.26e-321` at exit 0**; door 3 is
  `ffi_missing_link`, *"the library `now` needs `hero_time_monotonic_ns`, and nothing on the link line defines it"*
  (false, the FFI seat's finding, reproduced); door 7 builds (no Swift member on Linux).
- **Clang's calls for a warm `print`** (`calls.txt`): `old` 5 calls, 0.311 s; **the route 6, 0.559 s, none naming
  `platform/`, no archive, no member symbol in the binary**; the base 7, 2.982 s, one naming `platform/`.
- **The member edit** (`touch.txt`): the route's warm `print` 6 calls, 0.43 s; after a comment appended to
  `platform/hero_audio.c`, **9 calls, 2.93 s, `runtime.c` and both units compiled again**, as on this Mac.

**So on Linux arm64 the patch holds the FFI seat's conditions 1, 2 and 6 and the one-reading rule; it does not hold
condition 3 for door 3's words or for a declared program's missed name (`swiftentry`), nor condition 4 (the archive
inside GNU ld's group, `-lm`): four declared correct programs end at exit 2 as an internal error**, the base's own
failure carried (the CE's table, at its 13:23 sources, said the same), and the FFI seat's veto does not reach it (it
is a condition, not V1 or V2).

## 10. Two more probes on this Mac (ran, the patch's compiler)

- **The FFI seat's prediction** (`examples/sqlite/main.hero` unchanged): copies under the base and the route
  (`r2/sqlite/`), 18:2x, load about 40: both exit 0 and print `rows: 3`, `longest: 6`; `--emit-c` of the two equal
  **but the line `hero_program_sources`** (`diff` with it taken out, 29,444 bytes each); the link words (`build/*/
  link-words.rsp`) equal **but `build/layer-249f9b5944f1d786-4c06ceee57b2a396.a`, absent from the route's, and the
  keyed names of the runtime's and the units' objects**, which differ between any two compilers. **Held on this Mac**;
  its Linux half is unrun by me.
- **Door 3 beside a declared group** (`r2/extra2/linkmixed`: one group `extern "hero_net.h" layer` binding a constant,
  a second `extern "libnow.h" link "now"` as door 3 writes it): **exit 0, prints `1`, `true`, takes `hero_time`**.
  So door 3's group, refused in both its own spellings when alone (§ 4), builds unchanged once **another** group says
  `layer`: the word is judged per group at `check` and before clang, and per program at the link. The one-reading rule
  is a rule of each group's header; what a library the program names needs of the layer is read by neither half.
  **The same program on Linux arm64** (`r2/lnx/linkmixed.sh`, the patch's arm64 compiler, container `p260-critic`,
  20:58 to 21:14 UTC, `r2/lnx/out-linkmixed/`): **exit 1, `ffi_missing_link`, *"the library `now` needs
  `hero_time_monotonic_ns`, and nothing on the link line defines it"***, false: its link words put
  `build/layer-003065180a280819-7ee86660507d7361.a` **before** `-Wl,--start-group -lnow -Wl,--end-group`, where GNU ld
  has passed the archive when `libnow.a(now.o)` asks. So a declared program builds on this Mac and is refused on Linux
  with a false sentence: the FFI seat's condition 4 (the archive inside the group), unmet by the patch, measured on a
  declared build.

## 11. Which tracked programs the patch still leaves undeclared (ran, this Mac)

`r2/emit-one.sh` (the patch's `--emit-c` of one program from its own folder, the first `error[` line kept) over every
`tests/golden/run/*.hero` (1,051) and every `tests/golden/run/*.case/main.hero` (54), four at a time, from the
patched tree, 18:26:42 to 18:48:56, load about 100 (`r2/emit-run.txt`, `r2/emit-list-run.txt`); and over
`examples/*/main.hero` and `examples/*/*.hero` (120, `r2/emit-all.txt`, its first 123 lines):
- **The layer's class is told on exactly one: the connect-order golden** of § 8; the fetched case is
  `package_unread`. No other run golden is refused by the walk or the name rule. The other 15 non-zero exits are
  `--emit-c`'s own `ffi_one_file` (7), `ffi_pkgconfig` on libraries this Mac lacks (6) and `sys/prctl.h` (2), none of
  the layer.
- `examples/`: every program emits but `gallery/09-holes.hero` (a program with holes, no binary by design).
- **What this sweep cannot see**: a refusal told only at the link (`owndecl`'s shape), since `--emit-c` links
  nothing; `rebel/examples/`' 43 programs (3 emitted before I stopped the sweep by its PIDs: each compiles Rebel cold,
  about ten minutes apiece at that load); and the harness's own compiled texts. So **the landing owes, measured: 82
  heads, the CE's 81 and the connect-order golden's, the fetched case's `sum` and key, and the four harness texts the
  CE named** (`absence.hero:2778`, `:2779`, `thesis_cards.hero:269`, `thesis_grade.hero:512`: read, each a group over
  `hero_net.h` or `hero_time.h` with no `layer`, which the family programs show is refused at `check`); the whole
  `run` form, the net and Rebel's pipeline are what would see the rest, and none has run.

## 12. The CE's programs in the x86-64 image (ran, partly; `heroes-linux:latest`, image `6f78598a4eba`, emulated)

`docker run --rm --pull never --name p260-critic` with no `--platform` (docker warned the image is `linux/amd64` on an
arm64 host; `uname -m` `x86_64`), Debian clang 22.1.8. The base and the route built inside from the same two
`--emit-c` files at **`-O0`** (to fit the emulation: `real 218.85` and `218.81`), `sources 41f32a8f4b745c0c` and
`2ac05d2a50ea1fb7` (`r2/lnx/out/log.txt`); then the CE's `run-linux.sh` shapes phase unchanged but for its levels
(`SHAPE_LEVELS=plain`, `NOCALLS=1`, `r2/lnx/ce-run-linux.sh`), **plain only**, 17:29 UTC on (`r2/lnx/results.txt`):
- **Undeclared, all 26 rows**: `print` exit 0, no archive, no member (it prints `20`); **the other 25 exit 1, each
  `ffi_layer_undeclared`** at its group (the ten goldens, `ownhdr`, `platformhdr`, `constonly`, `face` and
  `rebel/reach_*` in their floors, `owndecl` at its binding after the link); **none exits 2**.
- **Declared, 10 of 26 rows before I stopped it**: `print` 0; `clock`, `usemod`, `ownhdr`, `rebel/reach_hero_time`
  0, time 5; `rebel/reach_hero_net` 0, net 36; **`audio` and `ui` exit 2, `internal error: linking failed:`**;
  `rebel/reach_hero_audio` exit 1, `ffi_unpaired_release` on `nextafterf`; `owndecl` exit 1, `ffi_layer_redeclared`.
  Each row equals the CE's arm64 row for the same shape (its 13:23 compiler; `owndecl` there was still `unreached`).
- **Stopped by me, `docker stop p260-critic`, at 21:48 local**, the container's own exit 137: this Mac's load had
  risen to 170 to 190 (`uptime`, other sessions), and the emulated cold archives had slowed to about one row in
  twenty minutes. **Unrun on x86-64**: the declared `constonly`, `platformhdr`, `twofam`, `testonly`, `face` and the
  eleven goldens; `-O2`, `--sanitize`; the CE's `base-check.sh`; the doors and my shapes. The CE's own `cmp` of the
  compiler's emission, which I began on this Mac at 18:55, was stopped by its PIDs at 21:48 after 2 h 52 min at 15 %
  of a core, unfinished.

## 13. One interleaved `print` row on this Mac (ran; a loaded machine, read by `user`+`sys` alone)

The CE's `time.sh` with its folder changed (`r2/time.sh`), `heroes build print.hero -o bin` at `-O0`, `old`
(`rebel-scratch/perf/old/heroes`, read only), the base and the route each built from its `--emit-c` by `clang -O2`
beside the base's runtime (`r2/o2/`, `real 442` each; `sources 41f32a8f4b745c0c` and `2ac05d2a50ea1fb7`), a prime,
four rounds, round 1 discarded; 18:49 to 18:54, **load 58 to 116** (`r2/time-mac.txt`). Every cold run but one of
`old`'s and one warm `base` run waited (`real` above twice `user`+`sys` plus 1 s, dropped by the CE's reader), so
`real` is no reading here and the CPU is:

| mode | `old` `user`+`sys`, rounds 2 to 4 | base | route | route / `old`, by the medians | route / base |
|---|---|---|---|---|---|
| warm | 0.46, 0.52, 0.51 | 2.98, 3.45, 3.50 | 0.60, 0.69, 0.73 | **1.35** (0.69 / 0.51) | 0.20 |
| cold | 1.20, 1.50, 1.52 | 13.72, 14.53, 16.50 | 2.00, 1.88, 2.35 | **1.33** (2.00 / 1.50) | 0.14 |

The CE's Mac row (warm 1.25, cold 1.34 by `user`+`sys` at `-O0`, load 76 to 86): **held** at this load, with the walk
and the grown runtime object inside it; never 1. The route is a fifth to a seventh of the base, as the CE says.

## 14. What nobody measured (the seats, the clerk, or me), each a question until it runs

1. **The full net under the patch**: unrun by every seat (the CE's condition 3 owes it). What runs of it exist: the
   `run` form over `the-platform-layer` (§ 8, 2 red), my `--emit-c` sweep of the run goldens (§ 11), the CE's four
   harness forms over its five cases, its compiler's own tests (2,297, not re-run by me).
2. **The net's own tests**: the four harness texts that compile a group over a member's header (`absence.hero:2778`,
   `:2779`, `thesis_cards.hero:269`, `thesis_grade.hero:512`) are not in `route.patch` (`grep '^diff --git'` of it:
   one harness file, `rebel_apps_run.hero`); refused by the rule they are (read, and the family programs, run), so the
   net's own tests would go red at the landing as patched. Unrun.
3. **Windows and MSVC**: no seat held the box. Defect 2788's class (a member that does not compile) under the principle,
   `lld-link`'s and `link.exe`'s words for a missing layer name through the compiler (the FFI seat ran `lld-link` in
   plain C only), the archive's `.lib`, the Win32 arms' headers that say *"no program includes them"*: none run.
4. **Linux x86-64** for the CE's programs: run by me (§ 12), plain only, every undeclared row and 10 of 26 declared rows before I stopped it; the rest, `-O2`, `--sanitize`, the doors and my shapes unrun there.
5. **The latency instrument's verdict** on the compiler's own build (the CE's prediction, ≥ 0.5 s faster): *"0 pairs
   kept ... no verdict"* in the CE's run; this Mac stood at load 40 to 190 all afternoon (`uptime`), so I did not run
   it. Unmeasured.
6. **The spec's real count** for any text (no `--refresh` authorised): every delta is vendored, a lower bound.
7. **The blind run**: registered, not started (correct); its `adopted` and `rival` arms, the per-file grading table
   and t2's six programs are the synthesis's, unwritten; **its key needs a landed build, and the patch is not one**
   until the landing's debts of § 11 are paid.
8. **Rebel**: the 115 faces under the check (the historian's measurement), the 43 `rebel/examples/` programs (3 of 43
   emitted by me), the `rebel` suite and Rebel's pipeline.
9. **Every tool that re-prints a program** past the formatter and `--dump-ast`: the two colourers, the `grammar`
   suite with the patched parser (the spec-warden ran it on `w1` with the base's parser: `grammar/extern-marks` FAIL,
   as it should be), `heroes probe`, `heroes mutate`.
10. **What a declared build pays at run time** (defect 2687): unchanged by the principle, read by the FFI seat with
    `otool -L`; nobody measured a start.
11. **The CE's `cmp` of the compiler's own emission** (base and route over the base's `selfhost/`, *"byte-identical"*):
    begun by me and stopped unfinished (§ 12); my base's emission is 40,396,174 bytes, the CE's figure.
12. **A refusal told only at the link in any tracked program**: my sweep runs no link (§ 11).

## 15. What the synthesis must not assert

- **"No correct program is refused both ways"** (the CE's argument): door 3 is (§ 4), and its undeclared sentence is
  false and its `guess` fix, applied, is `ffi_unknown_name`.
- **"The text says what the check refuses"**: `w1` and the patch disagree on a header of the program's own named
  `hero_*.h`, on `vendor/`, and on a binding through the program's own declaration (§ 5), all run.
- **"Every tracked program is declared"** or **"the landing is 81 heads"**: 82 at least, plus the fetched case's
  `sum` and key and four harness texts (§ 8, § 11); the `run` form is red on two goldens as patched.
- **"An undeclared program pays nothing of the layer"** without its qualifiers: warm it pays the walk (+0.06 s,
  carried, never resolved at a still load) and no member; cold it pays a grown runtime object (1.3 to 1.9 of `old`
  over four runs, the CE's two and my two, § 7, § 13), and **its whole cache goes cold when any member's bytes change** (§ 7, run on both
  arm64 platforms). *"At `old`'s cost"* is an inference after subtracting a carried share, and holds warm only.
- **"The latency instrument shows the compiler faster"**: it gave no verdict.
- **"Declared builds pay exactly R6 and never end at exit 2"** on Linux: `audio` and `ui` (arm64 and x86-64), `zall`
  and `armhdr` (arm64, mine) and `platformhdr` (arm64, the CE's) end at *internal error: linking failed* (the base's own `sin`/`nextafter`, carried);
  and a declared program's missed name is told by `header_needs`' false sentence (`swiftentry`, § 9).
- **"`owndeclwrong`'s wrong value is closed"** without its residue: through a macro or an inline body beside a
  declared group the same wrong value prints at exit 0 on both platforms (`mixedown`, § 3, § 9), as under R6.
- **"Seen by its writer and never by a program that imports it"**: the CE ran it told to the importer, pointing into
  the floor; that is the author's sentence, not the built behaviour.
- **"`heroes check` tells a header of the program's own that reaches the layer"**: it tells the name half only; the
  walk is `build`'s and `--emit-c`'s (§ 1, and every `check` 0 in § 3, § 4).
- **That the CE's Linux table is of the patch**: it is of `5b8289a15e4c1b6e` (13:23), before `ffi_layer_redeclared`;
  the patch's own key is `2ac05d2a50ea1fb7` (§ 2), whose arm64 and x86-64 runs are mine.
- **Anything of Windows, MSVC or `link.exe`.**
- **A prior-art line from the historian's commentary**: only from its quotations; Java from Oracle's guide, not JEP
  472's page; Zig as what `Sema.zig` at 0.14.0 states, not as *"reached first"* beyond it.
- **The blind run's spend** as a measurement (6.8 USD is an inference), or any D-rule outcome.

## What the synthesis must settle, most important first

1. **One reading of *the layer's file*, shared by the text and the check, before the blind run's arms are hashed.**
   The patch reads a group's string by name wherever the file is found and calls any runtime `vendor/` file the
   layer's; `w1` says *"it provides"* and names no `vendor/`; `w1` lets a binding through the program's own
   declaration build declared, the patch refuses it in both spellings (the FFI seat's condition 2 and its two
   defects, which the patch closes). Either the check asks where the header was found (as `layer_path` already does
   for the walk's files) or the text says the name is reserved, and `w1` gains the CE's *"a function of the layer is
   bound through the layer's header"*; then the registration's grading table and t2's key are written against that
   one text. Run on: `ownnamed`, `vendorinc`, `owndecl`, `owndeclwrong`, door 7 (§ 3 to § 5).
2. **Door 3, a correct program refused in every spelling of its own group.** Accept it (its verifying spelling is 3',
   the library's header including the layer's) and then **correct the words and the fix**, which today say the
   group's header puts the name in the program's own code (false) and offer a fix that does not compile; or read what
   a library the program names needs of the layer (`link_report.references` already says who refers). And say why
   door 3 builds unchanged on this Mac once an unrelated group says `layer`, and is refused on Linux with a false
   sentence (`linkmixed`, § 10): the word is per group before the link and per program at it.
3. **What the landing owes, recounted**: the connect-order golden (a member's `.c` included by a header of the
   program's own), the fetched case's `sum` and folder key, the harness's four compiled texts, then the whole `run`
   form, the net's own tests, the full net and Rebel's pipeline on the patched tree. The `run` form is red on two
   cases as patched (§ 8).
4. **GNU ld, which both Linux images and the CI's Linux legs use**: the FFI seat's condition 4 (the archive inside the
   one `-l` group; `-lm` for a member that calls the maths library) is not in the patch, so declared `audio`, `ui`,
   `zall`, `armhdr` (and `platformhdr`, the CE's) end at exit 2 as an internal error on arm64 and x86-64, and a
   declared program whose link misses a layer name is told by `header_needs`' false sentence (`swiftentry`), and door 3
   beside a declared group by `library_needs`' (`linkmixed`, § 10). Adopt it
   now (the FFI seat's condition) or file it beside the drafted `nextafter` defect (the CE's). The two found beside
   by both seats, `sin` with no `-lm` and door 3's false *"nothing on the link line defines it"* on GNU ld under R6,
   are unfiled.
5. **The residue, filed by its class**: a layer function declared wrongly by a header of the program's own and
   reached by a macro or an inline body, beside a declared group, prints a wrong value at exit 0 on both platforms
   under the patch and under R6 (`mixedown`). A wrong value is `blocking` by the project's list; the bodies' reading
   before the link, or the objects' undefined names, is what would see it (the CE's own note).
6. **What *pays nothing* means**: the cold ratio to `old` (1.3 to 1.9, never 1), and whether an undeclared program's
   runtime key should stop folding every member's bytes (§ 7). The principle's sentence is about the layer; the cache
   key is where an undeclared build still depends on it.
7. **Windows**: unrun by everyone; defect 2788's class is the one the principle bounds, and nothing measured shows
   it bounded there.
8. **The CE's falsifiable prediction** (the latency instrument, ≥ 0.5 s) needs a still machine; and the spec-warden's
   D3 prediction is a count of programs where the rule is a gate on each, so a *"right"* prediction can stand beside
   a failed D3: say how the two are read before the run.
9. **Travel and the importer**: the author's *"never by a program that imports it"* is not what the check does (an
   undeclared floor is told to its importer too); the 78 faces that reach a floor by `use` link the layer (the CE's
   `toml`, 142 functions never called): the author's word to apply, with the CE's number in hand.
10. **The historian's lines**: prior art only from the quotations; Java's from Oracle's guide; Zig's as the source
    states it.
