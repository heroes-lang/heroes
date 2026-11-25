# Panel 260, the compiler-engineer's report

Written 2026-11-25 from 12:39 on, as I go. My copy: the detached worktree
`.claude/worktrees/260-compiler-engineer` at `c3885b81b` (`git log --oneline -1`). Long output in
`.claude/worktrees/260-compiler-engineer-scratch/`. Programs and scripts I write are kept under
`docs/panel/260-reports/programs/compiler-engineer/` of my copy. Every line says **ran** (a command I ran) or **read** (a
file I opened).

## Verdict

- **verdict**: **approve with conditions** (no veto: no core construct, no ceiling breached).
- **section**: design.md §1.1 (simplicity is the ceiling), §1.7 and Part 5 (core versus sugar: the word is neither, a
  fact about the link like `link`), §1.12 (robustness: the one refusal, a layer function declared by the program),
  §1.3 (locality: what the word buys over an inferred switch).
- **implementation_cost**, built and run: about **400 lines of code**: `selfhost/check/layer_header.hero` 192 lines
  (91 of code: the name rule at `check`, three codes, the certain fix) and `selfhost/cli/layer_bound.hero` 365 (188 of
  code: the layer's files, the walk's judgment per group, the link's reader, the redeclared rule), and **+124 / -36
  code lines in 22 changed files**: parser (`keywords` +8, `parse/group_head` +13, `group`, `tails`, `records`), AST
  (`ast.hero` +9, 3 fields; 16 construction sites edited), printers (`print/fmt`, `print/dump`), checker (1 call), the walk
  (`include_stands` +40, edges and roots), the gate (`layer_reach` +3 of code), `Toolchain.layered`, `produce`,
  `value_run`, `whose` (+14). Emitter, IR, ownership, descriptors: **0**. Lexer: 0.
- **needed_for_self_hosting**: **no**. The compiler's groups name `hero_os.h` and `hero_compiler.h` only; the seed's
  compiler built the route unchanged (ran, 13:13), and the route links the compiler with no layer at all.
- **argument**: Not core: no type, no IR, no C a unit holds changes; the word lands where `link` lands. Its check
  reads *binds* one way on both sides (the header's name at `check`, the walk at `build`, the link's missing names
  last), so no correct program is refused both ways but one binding the layer through a declaration the layer did not
  write. Built and run: every shape builds declared as under R6 (same members, 0.88 to 1.07 of the base) and is told
  undeclared, never *internal error*; an undeclared `print(1)` falls from 5.6 to 11.7 times `old`'s cost to 1.1 to 1.6,
  on both platforms. A fifth declaration kind would touch 180 files; this touches 24.
- **prediction**: at the landing, `heroes run tests/harness/latency.hero -- compare <base> <route> --clock build` on a
  still machine reads the route **faster** on the compiler's own warm build, the instrument's verdict in its favour, by a
  median of **at least 0.5 s** on this Mac: `selfhost/` declares nothing, so its link no longer runs the members' `-M`,
  **0.788 s** of clang's wall for a warm `print` at load 5 here (`calls.sh`, 17:13; the base 0.996 s in clang, the
  route 0.201). Falsified by a median under 0.5 s or a verdict against the route. *First written 17:00 with **1.0 s**,
  from the `-M`'s 1.4 s at load 80 to 130 (§ 4.1); its first reading, 17:07 to 17:12 at load 5 to 8, gave **no
  verdict** (exit 2, every run waiting more than 0.1 s past its work), and the five kept pairs read by hand put the route
  ahead in 3, a median of 0.47 s of `user`+`sys` inside a spread of −1.06 to +1.59 s: the 1.0 s figure was the load's,
  not the `-M`'s, and is withdrawn here, with its reading kept.*
- **condition** (what would change my verdict to an objection): a blind measurement that readers do not write the word
  where it belongs (then the inferred switch over the same reading is the cheaper home, below); or a landing that lets
  `check` and `build` read *binds* two ways. To a veto: a home that adds a declaration kind or an input class.

**The conditions of my approval**: (1) the home is the word on the group head, judged by one reading on both sides, as
built; (2) a declared build pays R6 whole now, the narrowing routes each a sitting of their own; (3) the landing
carries the 81 heads in 64 files, the harness's four compiled texts, the fetched case's `sum`, the five cases, the
colourers, the `grammar` suite and the spec's sentence and grammar line, and runs the whole net, the `run` form over
the 30 declared goldens and the compiler's own tests after its last edit; (4) the residue (a layer function declared
by a header of the program's own and called only from a `static inline` body, in a program where another group says
`layer`) is filed by its class or closed by the bodies' reading before the link; (5) the audio members' `sin` with no
`-lm` on Linux is filed beside the drafted `nextafter`.

**What a veto would have compelled**: a written answer from the author on a home that is a fifth top-level kind
(337 lines of exhaustive matches in 180 files learn it), or a per-package file (a fifth input class,
`.claude/rules/cli-surface.md`), or a check whose *must declare* and *must not* read *binds* two ways (which refuses
`constonly` and the tracked `2696` both ways). None of the three is what I built or adopt.

## Status

- 12:39 started; load 15.6 (`uptime`), three containers of other sessions (`docker ps`).
- 12:40 the base's compiler building from the seed in `scratch/base-src` (`git archive c3885b81b selfhost runtime
  seed`, so my edits to the worktree never reach it): stage 1 by clang 12:40:16 to 12:40:47; stage 1's `--emit-c`
  38,004,504 bytes at 12:49:40 (the repair's figure); `clang -O2` of it done 12:53:38 (`real 236.90`, `user
  125.97`, load 183), `sources 41f32a8f4b745c0c`, the repair's key.
- 12:56 to 13:01 the route's stage 2 (the base's `-O2` compiler building my `selfhost/`), exit 0, `real 302.72`,
  `user 135.23`; warm rebuilds after each edit, 91 to 120 s.
- 13:00:52 to 13:22:33 the route's `-O2` compiler as the base's was built: **the seed's stage 1 emitted the route's
  `selfhost/` at exit 0**, 38,125,837 bytes; `clang -O2` of it, `real 555.67`, `user 144.29` (load 118 to 129),
  `sources 4daaa8e64a9aab05`. It is the 13:00 snapshot of my copy; what moved after it is wording, tests and the
  redeclared message (`diff -r`, 13:19), none of which a timing reads.
- 13:23 to 13:29 the route's final sources emitted by that compiler for Linux, 40,551,369 bytes; 13:28 the container
  `p260-compiler-engineer` started (`--rm`, image `338a1044ac90`, its own load 2.4).
- 13:04 to 13:40 the shapes on this Mac; 13:23 to 13:35 the compiler's own tests whole (2,297, 2 failed, both
  repaired); 13:28 to 13:59 Linux's builds and shapes; 13:41 to 16:53 this Mac's timing table, **of which about 14:16 to
  16:53 one block (`ui --sanitize`) waited on a paging machine**, its runs dropped and the last stopped by its PIDs;
  16:53 to 17:03 Linux's timing; 16:56 to 17:00 the residue found and closed; 17:01 the harness's forms over the five
  cases; 17:01 to 17:12 the emitted C compared; 17:01 to 17:06 the compiler's own tests whole again, **all 2,297
  passed**. No container of mine left (`docker ps`, 17:06); no commit, no push, no GitHub write, no paid run.

## What I read first (no route judged yet)

- **A fifth declaration kind costs 180 files**: `grep -rln '\.test_decl' selfhost --include='*.hero' | wc -l` reads
  180 files and 337 lines naming the last case of `DeclKind`, each an exhaustive match a new top-level kind must
  learn. A word on the group head touches the head's parser, the AST's three group-carrying kinds and their 18
  construction sites (`grep -rn 'function_decl(\|\.constant_decl(ty\|\.record_decl(' selfhost | wc -l`), and the two
  printers that rebuild a head from `header` and `library` (`print/fmt.hero:394` `extern_head`,
  `print/dump.hero:157` `extern_prefix`). The probe's reader skips a head line whole (`probe/reader.hero:296`).
- **`heroes check` is the frontend alone**: `cli/check.hero` (464 lines) runs parse, the module graph, resolution and
  the checker, and locates no toolchain; `check/runtime_header.hero` judges a group by its string alone. The runtime
  is found without clang by `runtime_places.located` (`cli/toolchain.hero:111`).
- **`runtime/hero_*.h` is exactly the 41 members' headers plus `hero_os.h` and `hero_compiler.h`** (`ls
  runtime/*.h`, compared by eye with `ls runtime/platform/*.c`): a fact about the runtime a test can hold.
- **Step 182c's walk attributes a file to the first group that reached it**: `include_stands.hero`'s `Walk.seen`
  (line 81) is one set for the program and `entered` (225) skips a file already seen, so a second group whose header
  includes a header already read is never told it reaches it. A per-group reading of *binds the layer* resting on the
  walk needs the walk's edges, not its labels.

## The route I built (13:00): a word on the group head, judged by the group's string at `check` and by the walk at `build`

**The home**: `layer`, a contextual word right after the header's string, `extern "hero_time.h" layer` (before
`link` or `pkgconfig`, which it does not replace: a header of the program's own may reach the layer and need a
library of its own). The spelling is one constant, `keywords.WORD_LAYER`; changing it is one line. Why the group and
not a module line, a catalogue key or a manifest (read, not run): a fifth top-level kind is 180 files of
exhaustive matches (above); a catalogue key meets `.claude/rules/cli-surface.md:73-76` and needs a package-side
catalogue Rebel does not have (`find rebel/library -name heroes.toml` prints nothing); the word on the group is where
the binding to the layer is written, so *"declared where it is DEFINED"* holds for a floor, for a program's own
module and for a root file alike, and travel needs no rule: a build sees every group of every module it compiles.

**Built, in my copy** (`git diff --stat -- selfhost`, `wc -l`, 13:00, the first build; the final counts after the
repairs are the verdict's): 21 files changed, 173 lines in, 38 out, and two
new modules, `selfhost/check/layer_header.hero` (181 lines, 85 of code) and `selfhost/cli/layer_bound.hero` (291
lines, 141 of code). Where it lands:
- **lexer**: nothing (the word is contextual, matched by text as `link` is).
- **parser**: `keywords.hero` +8 (the constant, `contextual()`), `parse/group_head.hero` +13 (`layer_word`),
  `parse/group.hero` +2, `parse/tails.hero` +9 (`Linkage.from_c` carries it, `linkage_layer`), `parse/records.hero` 1.
- **AST**: `ast.hero` +9: `layer: token.Span?` on `FunctionDecl`, `constant_decl` and `record_decl`, beside
  `library`; the other construction sites, `resolve/state.hero`, `resolve/qualified.hero` (2), `resolve/types.hero`
  (2), `print/signature.hero`, `handles.hero` (5, tests), take an absent one.
- **printers**: `print/fmt.hero` (`extern_head`, `ExternSpans`, `extern_head_once`'s three callers) and
  `print/dump.hero` (`extern_prefix`). The probe's reader skips a head line whole and owes nothing.
- **checker**: `check/layer_header.hero`, called from `checker.hero` beside `runtime_header` (+4): a group whose
  string names a layer header without the word is `ffi_layer_undeclared` at the string, a **certain** fix
  inserting ` layer`. It reads no file: the rule is the name's, `hero_<word>.h` but `hero_os.h` and
  `hero_compiler.h`, or `platform/hero_<word>.h`, and `cli/layer_bound.hero`'s test holds it to `runtime.c`'s
  never-read block (every name there obeys it; every runtime header obeying it is named there but the five
  `platform/hero_layer_*.h` the runtime's own unit includes, which the rule calls the layer's in the loud direction).
- **build, before the first clang**: step 182c's walk now records its edges and each group's root
  (`include_stands.hero` +40), so a group is judged by what its own header reaches whichever group read a file first;
  `layer_bound.told` refuses an undeclared group whose header reaches a layer file (`ffi_layer_undeclared`, certain
  fix) and a declared one whose header is not the layer's by name and reaches none (`ffi_layer_unreached`, at the
  word, certain fix removing it). **One reading on both sides** (name, or the walk's reach), so no program is refused
  both with the word and without it but one that binds a layer function through a declaration of its own (below).
- **link**: `layer_reach.for_build` returns no words unless a group says `layer` (+3 of code): no `-M`, no archive,
  nothing on the line. `Toolchain` gains `layered` (+7), set once by `produce.hero` (+11) and read by the value
  probe's link (`value_run.hero` +6), so every link a build makes carries the layer exactly where the program says.
- **a refused link**: `whose.hero` (+14) asks `layer_bound.told_at_link` before every other reader: in an undeclared
  program, a name the link misses that a layer header declares in the runtime's own forms is `ffi_layer_undeclared`
  at the binding of that name, or at the first group whose header is the program's own, a `guess` fix naming the
  layer's header. Never `internal error`, never `header_needs`'s *"no group in this program says which library has
  it"*.
- **emitter, IR, ownership, descriptors**: nothing. The word changes no C a unit holds.

**Core or sugar (design.md Part 5)**: neither; it is a fact about the link, like `link`. No IR construct, no type, no
lowering, no backend line. It is a new surface form and a new diagnostic class, so it owes every tool that re-prints a
program (`.claude/rules/diagnostics-and-goldens.md` § A new surface form) and a panel, which this is.

**The landing over the tree, run** (`docs/panel/260-reports/programs/compiler-engineer/land.py`, applied to my copy,
13:00): **81 group heads in 64 files**: 32 Rebel floors, 1 `examples/server/net.hero`, 29 `run` goldens at the top
and 1 under a `.case/` (20 naming a member's header, the ten through a header of their own, the 13 tracked headers
that reach a member at any depth recomputed by the script and equal to the critic's list), and
`tests/harness/rebel_apps_run.hero`. **One owes more than a word**: the `.case/` golden
`the-platform-layer-is-reached-from-a-fetched-packages-group` holds a fetched package whose catalogue pins a `sum`
and whose folder is named by its key, so its word moves both (unrun).

## The diagnostic (§ 7.3), built and run

Three codes beside the C boundary's, each annotated in the case that provokes it and snapshotted from a run (my
copy's `tests/golden/`, written 13:04 and 17:00, `.expected` made by the route's stage 2 from the repository's root, as
the forms run; **run through the harness's `check`, `unsupported`, `annotations` and `fixes` forms at 17:01**, below):
- **`ffi_layer_undeclared`**, three places, one mistake (the layer reached, the word not said):
  - at `check`, on the group's string: *"`hero_time.h` is the platform layer's header, and this group does not say
    `layer`: write `extern "hero_time.h" layer`"*, note *"the platform layer's members are compiled and linked only
    for a program whose groups say `layer`; a program none of whose groups says it links nothing of the layer, so its
    build asks nothing of it"*, fix **certain**, ` layer` inserted after the string
    (`check/layer-a-group-naming-the-layers-header-says-layer.hero`, three annotated groups, `.fixed` written by
    `check --apply`; the applied program checks clean, **ran**: exit 0);
  - at `build`, before the first clang, on the group's string: *"`<own>.h` reaches the platform layer's
    `hero_time.h`, and this group does not say `layer`: ..."*, fix certain
    (`unsupported/layer-a-header-of-the-programs-own-reaching-the-layer-says-layer.hero`, two includes deep);
  - at a refused link, before every other reader, on the binding's group: *"`hero_time_monotonic_ns` is the platform
    layer's, declared by its `hero_time.h`, and this group binds it through a header that includes none of the
    layer's, so no group of this program says `layer` and its link carries none of the layer: name `hero_time.h` in
    this group, or include it in the group's header, and say `layer` after the header"*, fix **guess**
    (`unsupported/layer-a-name-of-the-layers-declared-by-a-header-of-the-programs-own.hero`, `owndecl`'s shape).
- **`ffi_layer_unreached`**, at the word: *"this group says `layer`, and `<own>.h` is none of the platform layer's
  headers and includes none of them, so nothing it binds is the layer's: take the word out, or name the layer's
  header that declares what this group binds"*, fix certain, the word taken out
  (`unsupported/layer-a-group-saying-layer-whose-header-reaches-none-of-it.hero`).
- **`ffi_layer_redeclared`** (since 17:00), at the binding: *"`hero_time_monotonic_ns` is the platform layer's, declared
  by its `hero_time.h`, and this group binds it through `<own>.h`, which includes none of the layer's headers: name
  `hero_time.h` in the group, or include it in `<own>.h`"*, note why, no fix
  (`unsupported/layer-a-layer-function-declared-by-the-programs-own-header-beside-a-group-saying-layer.hero`).

Each exits 1 (**ran**, 13:04 and 17:00). `owndecl` declared is `ffi_layer_redeclared` and undeclared `ffi_layer_undeclared`:
**refused both ways**, by design, and the fix that builds it is the one both messages name, the layer's own header.
A program that declares a layer function in a header of its own binds it against a declaration the layer did not
write, which §4.19's mechanism (clang checks the binding against the real declaration) exists to refuse; this is
`cli/runtime_bound.hero`'s precedent applied to the layer, and it is the one shape the route refuses outright.

## What *binds the layer* means, at which verb (ran on this Mac, 13:04 to 13:15)

**One reading, two halves, and a third for the link**: a group binds the layer where its header's **name** obeys
the rule (`check`, no file read) **or** the **walk** from its header reaches a file of the layer at any depth (`build`,
before the first clang, every `#if` but `#if 0` read as true). Undeclared and binding: `ffi_layer_undeclared`;
declared and binding nothing: `ffi_layer_unreached`. **A name the link misses** in an undeclared program, that a layer
header declares in the runtime's own forms, is the third: `ffi_layer_undeclared` before every other reader. And a
declared group whose walk reaches nothing but which binds a name a layer header declares (`owndecl` declared) is told
truly at that binding, `ffi_layer_redeclared` (`ffi_layer_unreached` until 17:00), *"`hero_time_monotonic_ns` is the platform layer's, declared by its
`hero_time.h`, and this group binds it through `myclock.h`, which includes none of the layer's headers"*, with the
note why (clang checks a binding against the header its group names). **My first build told it *"nothing it binds is
the layer's"*, false, found on the declared run at 13:08 and repaired at 13:09** (the layer's headers read only on
that path: a declared group whose header reaches none of them).

**Read, not run**: `heroes check` runs no walk. `include_stands.hero` is used by `shadowed.hero` alone, and `shadowed`
by `produce.hero` and its own family; `cli/check.hero` uses neither (`grep -rln '^use cli/include_stands'`,
`'^use cli/shadowed'`). § 4.5's *"that `heroes check` walks too is read only in the commit body"* is therefore not so
at the base: `check` reads no header. **Ran**: `heroes check` of `ownhdr` and of the ten undeclared exits 0; `build`
and `--emit-c` of the same refuse them.

**At each verb, under the route's stage 2** (`programs/compiler-engineer/shapes.sh s2 <route> undeclared|declared`,
`scratch/shapes-s2-*.txt`): `check` tells the name half; `build`, `run`, `test` and `--emit-c` go through `produce`
and tell the name and the walk halves before any clang (the checker runs inside `compile.to_unit` first, so a
named header is told at type checking and the walk sees the rest); the link half is `build`'s, `run`'s and `test`'s
only, since `--emit-c` links nothing: **`owndecl` undeclared emits C at exit 0** (ran), as every link-time refusal of
the C boundary's class does today. The value probe's link follows the program's word (`Toolchain.layered`): ran on
`programs/compiler-engineer/valueprobe/main.hero`, a `hero_os.h` constant read by the probe, the route's build exit
0, prints `2`, its `build/` holding a `value-*` answer and **no `layer-*` file**; the base's the same program, **4**
`layer-*` files (the archive, its seal, its record, its needs).

## Whether the declaration travels (question 5)

**By the module graph, with no rule of its own**: a build's declarations are every module's it compiles, its
packages' among them (`compile.to_unit` parses the one `Source` that discovery joined, `source.hero:1-28`), and
`layer_header.declared` asks them all. So a face whose floor declares travels to every program that reaches the
face by `use`; a program importing a declared package declares nothing and links the layer; a package importing a
declared package declares nothing. **What it costs, read with the critic's count**: 78 of Rebel's 115 faces reach a
member floor, so a program reading TOML pays R6 whole, which is what it pays today; a program reaching none of them
(`json`, `strings`, `bytes`, `numbers`, `unicode`, `path` among Rebel's) pays nothing. What the link takes is still
what a reached module's groups bind (`emit/extern_refs.hero`'s `used` references), so the word travelling by `use`
is exact for the link: a declared module the build compiles is a module whose bound functions the link references.
The one program that pays for nothing is the one that reaches a declared group binding only constants or records
(`constonly`), which is a program the word's writer chose.

**The second half of the author's sentence, *"seen by its writer and never by a program that imports it"***, holds
only as *"seen by its writer first"*: an undeclared group inside a package is told to an importer too
(`rebel/reach_hero_*` and `face` undeclared, ran: the refusal points into the base library's floor), as any error
inside a package is. A rule that hid it from the importer would be a fallback. Nothing in the language today keeps a
package's error from its importer, and I would not add one.

## The conservative resolution, and the robust one

**Robust (what I built and adopt)**: one reading of *binds* on both sides (name or walk), the link's reader for what
the walk cannot see, `owndecl` refused both ways with its reason, a declared build paying R6 whole. **Conservative**
would have been: the name half alone (`check` only, ~85 lines), every own-header program left to a refused link and
told there by the link reader; it costs the walk's edges (+40 lines in `include_stands.hero`) less, and tells the ten
goldens' shape after clang compiled every unit rather than before.

## The strongest reason the proposal is wrong, and why I do not stand on it

**The word is, by construction, an echo of what the compiler computes.** For a check that holds the word to the code
and never refuses a program both ways, the reading that decides *must declare* must also decide *must not*, so the
word can never add a shape the reading misses nor drop one it sees: what the build links is decided by the reading,
exactly as in (S), the inferred switch the author set aside (§ 7.6). The same programs are accepted, the same are
refused (`owndecl` both ways in each), the same members are linked. An inferred switch over the same reading costs
less: no parser, AST, printer, editor-grammar, spec-grammar or `ffi_layer_unreached` lines, and no CL-036 walk
(about 100 of the lines above).

**Why I do not stand on it**, two reasons the documents give, each about what is not computed today:
- **Locality (design.md §1.3)**: *"a construct is good for an LLM if the meaning of a line can be determined from that
  line"*. `extern "myclock.h" layer` says on its line that this group makes every program reaching it compile and link
  the layer; under the switch nothing on the line says so, and `myclock.h` must be opened. The registered
  llm-ergonomist measures whether a reader writes the word; the argument is theirs to price, not mine.
- **A contract a runtime change cannot move in silence**: under the switch, a runtime edit that made `hero_os.h`
  include a member's header would make every program that binds `hero_os.h` (80 of the run goldens and the compiler's 33
  groups, the brief's counts, carried) compile and link the layer with nothing said; under the word, each of those groups is refused at its next
  build, the loud direction (`.claude/rules/module-shape.md` § A narrowing asks the value). That is robustness, rank 3,
  and it is what the word buys that the switch cannot. (An inference from the code; no such runtime edit was run.)

**What would make me stand on it**: a measurement that the word's spelling is not written unprompted where it belongs
(the llm-ergonomist's blind run), or a landing that cannot keep the two readings one, would make the switch the
better home; neither is in my hands today.

## Where the brief was false or silent

1. **§ 4.5, *"that `heroes check` walks too is read only in the commit body"***: at the base it does not. Read:
   `include_stands` is reached only through `shadowed`, which only `produce` uses; `cli/check.hero` uses neither.
   Ran: `heroes check` of `ownhdr` and of the ten (undeclared) exits 0 under the route, where `build` refuses them.
2. **§ 6.2, *"step 182c's walk ... would see the ten and `ownhdr`"***: of the program, yes; of a group, no. The walk's
   `seen` is one set for the program (`include_stands.hero:81`, `entered` at 225), so a file is attributed to the
   first group whose walk reached it. A reading of *binds* per group needs the walk's edges, which I added (+40 lines).
3. **§ 5 and item 3, *"R6's own test ... is the one the principle rewrites"***: its mechanism stands. The test links
   C units beside `layer.archive`'s words through `link.link_objects` (`layer.hero:206` to 239) and never asks
   `for_build`, so no word of the principle falsifies its asserts; its first sentence, *"every link carries the
   layer"*, is what moves. I reworded its title and doc, and the half it no longer says is a new test beside
   `for_build` (`layer_reach.hero`, *"a program none of whose groups says layer asks the platform layer nothing, and
   one that says it links the archive"*).
4. **§ 3.1's 54 files count group lines, not program texts**: the harness holds seven more texts naming a member's
   header (`grep -rnoE 'extern \\?"(platform/)?hero_[a-z_]+\.h\\?"' tests/harness`), four of which compile them and
   would be refused without the word: `absence.hero:2778` and `:2779` (defect 2489's case), `thesis_cards.hero:269`,
   `thesis_grade.hero:512`. The other three only read a head's header (`package_face.quoted_in` takes the text
   between the first two quotes, which ` layer` after them does not move).
5. **Silent: the fetched case** `tests/golden/run/the-platform-layer-is-reached-from-a-fetched-packages-group.case/`
   holds its package under a `sum` and a folder named by its key; its word moves both.
6. **Silent: a tracked program binds the layer on one platform alone.**
   `tests/golden/run/fixedbugs-2696-a-strangers-datagram-never-stops-a-wait.hero` reaches `hero_poll.h`
   unconditionally, and its calls of `hero_poll_*` stand under `#if defined(_WIN32)` (its header's lines 73 to 96):
   declared, it builds and runs on this Mac and **takes no member** (ran, `nm`). Every reading that the brief lists by
   name says it binds; the link on Darwin says it binds nothing. It is `constonly` in the tree, and the word, written
   once for every platform, makes Darwin and Linux pay an archive their link does not take.
7. **§ 6.2 and § 7.4, `constonly` *"refused declared and undeclared alike"***: only by a check whose two refusals read
   *binds* two ways. With one reading on both sides, `constonly` declared builds, prints `1` and takes no member (ran).

## What the landing owes the tree, counted (item 4)

- **81 group heads in 64 tracked `.hero` files** (`land.py`, ran): 32 Rebel floors, `examples/server/net.hero`, 30 `run`
  goldens (19 at the top naming a member's header, the ten through a header of their own, one under the fetched
  `.case/`), `tests/harness/rebel_apps_run.hero`. Nothing kept for the old spelling (CLAUDE.md § 2).
- **4 more texts the harness compiles** (`absence.hero:2778`, `:2779`, `thesis_cards.hero:269`,
  `thesis_grade.hero:512`), and the fetched case's `sum` and folder key.
- **The compiler's own tests that build the layer**: `layer.hero`'s six (332, 424, 467, 533, 567, 578) call
  `layer.archive` directly and stand; R6's own (332) reworded; one new beside `for_build`; `layer_bound.hero`'s two
  hold the name rule to `runtime.c`'s never-read block and the declared names to their headers;
  `check/layer_header.hero`'s two hold the rule and `declared`. No other compiler test compiles a text naming a layer
  header (`grep -rnoE 'extern \\"(platform/)?hero_[a-z_]+\.h\\"' selfhost`, two, both `layer_reach`'s).
- **Five golden cases** (above), one `check` with its `.fixed`, four `unsupported`, the harness's `check`, `fixes`,
  `annotations` and `unsupported` forms run over them and green (17:01); owed: the `run` form over the 30 declared
  goldens, `emission`, and the full net.
- **Every tool that re-prints a program** (`.claude/rules/diagnostics-and-goldens.md` § A new surface form): the
  formatter and `--dump-ast` built, the round trip ran byte-identical over the word alone, beside `link`, beside
  `pkgconfig` and absent (13:15); `heroes probe`'s reader skips a head line whole (`probe/reader.hero:296`), owes
  nothing, unrun; **owed, unbuilt**: the two colourers (`editors/vscode/syntaxes/heroes.tmLanguage.json`,
  `site/src/lib/highlight.ts`), the `grammar` suite's contextual-word list (`keywords.contextual()` grew by one), the
  spec's grammar line (§ 13, line 508), and `heroes mutate`'s view of a head.
- **The word written after `link`** (`extern "x.h" link "m" layer`) is told `expected_end_of_line` at the word (ran);
  a message naming the order would be better, and is owed.

## The shapes, this Mac (ran 13:04 to 13:40, load 84 to 130)

The route's stage 2 (`sources` of 13:02, before the redeclared repair and the `platform/` file repair below, neither
of which moves a verdict here), `programs/compiler-engineer/shapes.sh s2 <route> undeclared|declared`: undeclared is
every shape as the briefs hold it, over the base's Rebel library (`git archive c3885b81b rebel/library`); declared adds
` layer` after every group's header and points the catalogues at my copy's library, which `land.py` declared. One
shared `build/` per folder; each binary run under `timeout 20`; members read by `nm`. Exit codes at plain / `-O2` /
`--sanitize`; the code is the first error's:

| shape | undeclared plain / -O2 / --sanitize | check, --emit-c | declared plain / -O2 / --sanitize | members taken (declared, plain) | check, --emit-c |
|---|---|---|---|---|---|
| `print` | 0 / 0 / 0 | 0, 0 | 0 / 0 / 0 | none | 0, 0 |
| `clock` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | time 5 | 0, 0 |
| `audio` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | audio 39, net 36, stream 23 | 0, 0 |
| `ui` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | net 36, poll 17, stream 29, ui 301 | 0, 0 |
| `usemod/` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | time 5 | 0, 0 |
| `ownhdr/` | 1 ffi_layer_undeclared (each, the walk) | **0**, 1 | 0 / 0 / 0 | time 5 | 0, 0 |
| `owndecl/` | 1 ffi_layer_undeclared (each, the link) | **0, 0** | 1 ffi_layer_unreached (each) | none | 0, 1 |
| `rebel/reach_hero_time` | 1 ffi_layer_undeclared (each, in the floor) | 1, 1 | 0 / 0 / 0 | time 5 | 0, 0 |
| `rebel/reach_hero_audio` | 1 (each, in the floor) | 1, 1 | 0 / 0 / 0 | audio 39, net 36, stream 23 | 0, 0 |
| `rebel/reach_hero_net` | 1 (each, in the floor) | 1, 1 | 0 / 0 / 0 | net 36 | 0, 0 |
| `constonly/` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0, prints `1` | **none** | 0, 0 |
| `platformhdr/` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | audio 39, net 36, stream 23 | 0, 0 |
| `twofam/` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | net 36, time 5 | 0, 0 |
| `testonly/` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0, prints `42` | time 5 | 0, 0 |
| `face/` | 1 ffi_layer_undeclared (each, in the floor) | 1, 1 | 0 / 0 / 0 | disk 35, net 36, procs 3, stream 23, sys 32, time 5, users 8 | 0, 0 |
| the ten (own headers; two at depth two) | 1 ffi_layer_undeclared (each, the walk) | **0**, 1 | 0 / 0 / 0, each prints its lines | net 36 and kin; **2696: none** | 0, 0 |
| the control naming `hero_net.h` | 1 ffi_layer_undeclared (each) | 1, 1 | 0 / 0 / 0 | net 36 | 0, 0 |

- **Undeclared, no layer anywhere**: across the 78 undeclared builds, `find <variant> -name 'layer-*.a'` reads **0**;
  `print` builds, runs and holds no member's symbol at every level. Not one undeclared build ends at exit 2, *internal
  error* or `header_needs`'s sentence.
- **Declared, the base's members exactly**: every row's families equal `ground/repair-members-taken.txt`'s under the
  base (`clock` 5, `audio` 39/36/23, `ui` 301/36/29/17, `face` the same seven), so a declared build links what R6 links
  today. 34 archives in the declared variant's folders, one per folder and key (`ui`'s Swift members a key of their
  own).
- `owndecl` declared reads, under this table's stage 2, the sentence repaired at 13:09; the repaired compiler tells it
  at the binding (ran by hand, 13:16, quoted above).

## How a declared build pays (item 2): R6 whole, now; the narrowing routes left for later

**Adopt R6 whole for a declared build**: every member asked by the one `-M` and archived, the linker choosing, exactly
today's route, its tests and its guards unchanged (`layer.hero`, `reached_each.hero`, `opening.hero` untouched by the
route). Measured above: a declared build takes the base's members row for row. **(a) guarded, (d), (P2), (f): left
for later, each a sitting of its own**, for reasons read in the files and not priced by me on `clock`, `audio` and
`ui`: (a) without the lane's fallback must close § 6.1's 9 (macOS) and 107 (Linux) calls by a closure the compiler
derives and a test that fails on every leg when one is missed, and what `nm` cannot see (task 2593's 151 function
slots, a Swift member's C entry) stays a hole; (d) serves an archive on a record and asks the link afterwards, the
shape defect 2289 closed (§ 7.5's inference, unrun); (P2) writes outside the folder a build starts in, which no build
does today, and re-opens defects 134 and 443's ground; (f) rests on every file system moving a directory's time on
every entry, unmeasured on APFS, ext4, overlayfs and NTFS. **What the principle already buys is the large part**: the
undeclared majority pays nothing (below), so the narrowing routes price the minority, and a declared program pays
what every program pays at the base.

## The spec sentence (item 7), for the spec-warden to price (unmeasured by me)

§ 13, line 401, the clause *"and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked
where a group names one"* becomes, as a reader must be told it: *"and sockets and clocks from its platform layer's
`hero_net.h` and `hero_time.h`; every `hero_<word>.h` but those two of the language's, and `platform/hero_<word>.h`,
is the layer's. A group whose header is the layer's or includes one says so after it, `extern "hero_time.h" layer`,
and only a program one of whose groups says `layer` links the layer. A function of the layer is bound through the
layer's header."* And the grammar, line 508: `Extern = "extern" string [ "layer" ] [ ( "link" | "pkgconfig" ) string ]
NEWLINE`. What a reader needs and nothing else: the rule a header's name obeys (so the 39 headers the spec does not
name are covered), where the word goes, what it costs, and the one refusal (a layer function declared elsewhere).

## The guards, by their tests (item 3; ran 13:23 to 13:35)

`heroes test selfhost/main.hero` over my copy, whole (the base's `-O2` compiler compiling the route's sources into
the test binary): **2,297 tests, 2 failed**, `real 748.81`, `user 289.31` at load 118 to 137
(`scratch/selftest.txt`). By the guards' own titles (`grep -F` of each name over the verdicts): defect 2289, 4 ok;
2625, 1 ok; 134, 6 ok; 443, 1 ok; 2489, 4 ok; step 182c's walk (`d-lane-blocking-small-2`), 4 ok; feature 2295 (R6's
own among them, reworded), 9 ok. **Nothing moved in a guard**: the route touches the archive's door
(`layer_reach.for_build`) and the walk's records (edges and roots), never what a guard asserts.

**The two failures were mine, and each was the test doing its job**:
- `keywords.hero`'s *"a contextual word is never a keyword, and each is listed once"* pins the count at 18; the word is
  the nineteenth. Its pin moves in the commit that adds the word (repaired, ran alone: ok).
- `layer_bound.hero`'s *"the rule names the layer's files exactly as the runtime's never-read block does"* found that
  my `layer_path` called only `platform/hero_<word>.h` the layer's, where the block names the members' `.c` and
  `.swift` files there too: a file anywhere under the runtime's `platform/` is the layer's now (the group-string rule
  stays headers-only). Repaired; the panel's six tests ran alone: 6 passed.

**Run again whole after every repair, 17:01 to 17:06** (the redeclared code and its case in): **2,297 tests, all
passed**, `real 328.75`, `user 210.76` at load 10 to 24 (`scratch/selftest2.txt`); the guards' counts the same, the
panel's six ok.

**What replaces R6's own test** (`layer.hero:330`, now 332): its asserts stand (the archive links, the linker takes
only what a program calls); its claim *"every link carries the layer"* becomes the new test beside `for_build`, which
runs the gate: an undeclared program's `for_build` gives no word and makes no `build/`, a declared one's gives the
archive. **No guard is lost.**

## The shapes, Linux arm64 (ran 13:28 to 13:59, container `p260-compiler-engineer`, image `338a1044ac90`, Debian clang 22.1.8)

Each compiler built inside from its C by `clang -O2 ... -lm -lpthread -ldl` (`programs/compiler-engineer/run-linux.sh`):
`old` from `perf/old/seed/heroes.c`, `real 274.21`; the base, `real 322.69`, `sources 41f32a8f4b745c0c` (this Mac's key);
the route from its final sources as its own `-O2` compiler emitted them (13:23), `real 233.07`, `sources
5b8289a15e4c1b6e`. Then every shape under the route, undeclared and declared, plain, `-O2`, `--sanitize`:

| shape | undeclared plain / -O2 / --sanitize | declared plain / -O2 / --sanitize | members (declared, plain) |
|---|---|---|---|
| `print` | 0 / 0 / 0 | 0 / 0 / 0 | none |
| `clock`, `usemod/`, `ownhdr/`, `testonly/`, `rebel/reach_hero_time` | 1 ffi_layer_undeclared (each) | 0 / 0 / 0 | time 5 |
| `owndecl/` | 1 ffi_layer_undeclared (each, the link) | 1 ffi_layer_unreached (each, at the binding) | none |
| `constonly/`, the tracked `2696` | 1 ffi_layer_undeclared (each) | 0 / 0 / 0 | **none** |
| `twofam/` | 1 (each) | 0 / 0 / 0 | net 36, time 5 |
| `rebel/reach_hero_net`, the control and seven of the ten | 1 (each) | 0 / 0 / 0 | net 36 (and kin) |
| `audio`, `platformhdr/` | 1 ffi_layer_undeclared (each) | **2 internal / 2 internal / 0** | none |
| `ui` | 1 ffi_layer_undeclared (each) | **2 internal / 2 internal / 0** | none |
| `face/`, `rebel/reach_hero_audio` | 1 ffi_layer_undeclared (each, in the floor) | **1 ffi_unpaired_release (each)** | none |

**Every declared failure here is the base's own** (`programs/compiler-engineer/base-check.sh`, ran 13:59, the base
over the same programs as written): `audio` and `platformhdr` end at *internal error: linking failed* on an undefined
`sin` (`runtime/platform/hero_audio.c:641`, `hero_audio_pulse.c:573`), `ui` on `nextafter`
(`hero_ui_parts_gtk.c:1260`, the lane's draft defect `d-lane-layer-speed-2`), and `face` and `reach_hero_audio` are
told `ffi_unpaired_release` on `nextafterf`; the route's declared builds read the same names. **Found beside, not
filed**: the audio members' `sin` is the same class as the drafted `nextafter` (a member calling the maths library
with no `-lm` on Linux) and no draft names it. Under the principle both stop only a declared program; under R6 they
stop every program that binds those headers, as they do today.

**Clang's calls for a warm `print`**: the route's undeclared build makes **9 calls on Linux and none names
`platform/`**, its `build/` holding no archive (`calls-mine-print.log`). The image has no `bc`, so the log's seconds
column is empty, and with the wrapper on `PATH` the base's and the declared route's counted builds compiled their
archives again (97 and 102 calls, 44 over `platform/`), so their warm counts there are cold ones: unread, the Mac's
log stands for them.

## The timing table, this Mac (ran 13:41 to 16:53)

`programs/compiler-engineer/table-mac.sh` (`time.sh` per program and level): `heroes build <program> -o bin`, a prime,
then four rounds each a warm build in the compiler's own folder and a cold one in a fresh folder, the compilers
interleaved inside each round, **round 1 discarded**, rounds 2 to 4 kept; **a run whose `real` stands above twice its
`user`+`sys` plus one second is dropped as waiting** and counted (`read_times.py`). `old` is
`rebel-scratch/perf/old/heroes` (read only), the base the `-O2` build of `c3885b81b`, the route the `-O2` build of my
13:00 snapshot; each beside its own runtime. `print` as written for all three; `clock`, `audio` and `ui` as written
for the base and declared for the route (`old` refuses them, § 4.5). **Load**: 76 to 86 at `-O0`'s `print`, my own
container compiling three compilers beside it until 13:59; 14 to 72 after. Best `real` (its `user`+`sys`) and the
ratios, every exit 0:

| program | level | mode | compiler | best real (its user+sys) | median user+sys | kept (dropped) | exits | load | real / old | u+s / old | real / base | u+s / base |
| audio | O0 | cold | base | 10.25 (7.60) | 7.77 | 3 (0) | 0 | 38-47 |  |  |  |  |
| audio | O0 | cold | route | 10.96 (7.86) | 7.85 | 3 (0) | 0 | 38-45 |  |  | 1.07 | 1.03 |
| audio | O0 | warm | base | 2.42 (1.85) | 1.85 | 3 (0) | 0 | 41-47 |  |  |  |  |
| audio | O0 | warm | route | 2.43 (1.90) | 1.89 | 3 (0) | 0 | 41-47 |  |  | 1.00 | 1.03 |
| audio | O2 | cold | base | 16.30 (13.61) | 13.80 | 3 (0) | 0 | 19-21 |  |  |  |  |
| audio | O2 | cold | route | 14.32 (12.63) | 13.91 | 3 (0) | 0 | 18-20 |  |  | 0.88 | 0.93 |
| audio | O2 | warm | base | 2.20 (1.84) | 1.84 | 3 (0) | 0 | 18-20 |  |  |  |  |
| audio | O2 | warm | route | 2.31 (1.92) | 1.94 | 3 (0) | 0 | 18-20 |  |  | 1.05 | 1.04 |
| audio | sanitize | cold | base | 8.18 (7.82) | 8.38 | 3 (0) | 0 | 15-17 |  |  |  |  |
| audio | sanitize | cold | route | 7.97 (7.79) | 8.56 | 3 (0) | 0 | 14-17 |  |  | 0.97 | 1.00 |
| audio | sanitize | warm | base | 1.60 (1.51) | 1.80 | 3 (0) | 0 | 15-18 |  |  |  |  |
| audio | sanitize | warm | route | 1.52 (1.49) | 1.72 | 3 (0) | 0 | 15-18 |  |  | 0.95 | 0.99 |
| clock | O0 | cold | base | 9.36 (7.83) | 7.74 | 3 (0) | 0 | 48-72 |  |  |  |  |
| clock | O0 | cold | route | 9.34 (7.62) | 7.62 | 3 (0) | 0 | 46-68 |  |  | 1.00 | 0.97 |
| clock | O0 | warm | base | 2.13 (1.88) | 1.81 | 3 (0) | 0 | 54-72 |  |  |  |  |
| clock | O0 | warm | route | 2.16 (1.82) | 1.82 | 3 (0) | 0 | 52-72 |  |  | 1.01 | 0.97 |
| clock | O2 | cold | base | 16.54 (13.73) | 13.91 | 3 (0) | 0 | 33-64 |  |  |  |  |
| clock | O2 | cold | route | 17.07 (13.72) | 13.87 | 3 (0) | 0 | 30-57 |  |  | 1.03 | 1.00 |
| clock | O2 | warm | base | 2.36 (1.88) | 1.88 | 3 (0) | 0 | 39-72 |  |  |  |  |
| clock | O2 | warm | route | 2.23 (1.85) | 1.85 | 3 (0) | 0 | 38-72 |  |  | 0.94 | 0.98 |
| clock | sanitize | cold | base | 10.33 (9.22) | 9.56 | 3 (0) | 0 | 16-19 |  |  |  |  |
| clock | sanitize | cold | route | 10.76 (9.31) | 9.62 | 3 (0) | 0 | 15-18 |  |  | 1.04 | 1.01 |
| clock | sanitize | warm | base | 2.18 (1.81) | 1.85 | 3 (0) | 0 | 14-18 |  |  |  |  |
| clock | sanitize | warm | route | 2.05 (1.83) | 1.88 | 3 (0) | 0 | 14-18 |  |  | 0.94 | 1.01 |
| print | O0 | cold | old | 0.85 (0.68) | 0.71 | 3 (0) | 0 | 79-84 |  |  | 0.09 | 0.09 |
| print | O0 | cold | base | 9.91 (7.72) | 7.68 | 3 (0) | 0 | 76-83 | 11.66 | 11.35 |  |  |
| print | O0 | cold | route | 1.21 (0.91) | 0.94 | 3 (0) | 0 | 76-83 | 1.42 | 1.34 | 0.12 | 0.12 |
| print | O0 | warm | old | 0.36 (0.28) | 0.29 | 3 (0) | 0 | 79-86 |  |  | 0.16 | 0.16 |
| print | O0 | warm | base | 2.29 (1.75) | 1.79 | 3 (0) | 0 | 79-84 | 6.36 | 6.25 |  |  |
| print | O0 | warm | route | 0.48 (0.35) | 0.35 | 3 (0) | 0 | 79-84 | 1.33 | 1.25 | 0.21 | 0.20 |
| print | O2 | cold | old | 2.04 (1.68) | 1.68 | 3 (0) | 0 | 66-68 |  |  | 0.12 | 0.12 |
| print | O2 | cold | base | 17.42 (13.77) | 13.74 | 3 (0) | 0 | 62-71 | 8.54 | 8.20 |  |  |
| print | O2 | cold | route | 2.94 (2.36) | 2.36 | 3 (0) | 0 | 62-72 | 1.44 | 1.40 | 0.17 | 0.17 |
| print | O2 | warm | old | 0.38 (0.28) | 0.28 | 3 (0) | 0 | 62-68 |  |  | 0.17 | 0.16 |
| print | O2 | warm | base | 2.22 (1.73) | 1.79 | 3 (0) | 0 | 62-68 | 5.84 | 6.18 |  |  |
| print | O2 | warm | route | 0.42 (0.32) | 0.36 | 3 (0) | 0 | 62-68 | 1.11 | 1.14 | 0.19 | 0.18 |
| print | sanitize | cold | old | 1.21 (1.06) | 1.14 | 3 (0) | 0 | 15-17 |  |  | 0.11 | 0.12 |
| print | sanitize | cold | base | 10.63 (9.20) | 9.76 | 3 (0) | 0 | 15-15 | 8.79 | 8.68 |  |  |
| print | sanitize | cold | route | 1.65 (1.45) | 1.50 | 3 (0) | 0 | 15-15 | 1.36 | 1.37 | 0.16 | 0.16 |
| print | sanitize | warm | old | 0.34 (0.27) | 0.29 | 3 (0) | 0 | 14-15 |  |  | 0.17 | 0.16 |
| print | sanitize | warm | base | 2.00 (1.72) | 1.76 | 3 (0) | 0 | 15-17 | 5.88 | 6.37 |  |  |
| print | sanitize | warm | route | 0.47 (0.39) | 0.39 | 3 (0) | 0 | 15-17 | 1.38 | 1.44 | 0.23 | 0.23 |
| ui | O0 | cold | base | 29.52 (25.36) | 25.36 | 3 (0) | 0 | 42-54 |  |  |  |  |
| ui | O0 | cold | route | 28.27 (24.73) | 25.84 | 3 (0) | 0 | 47-50 |  |  | 0.96 | 0.98 |
| ui | O0 | warm | base | 3.05 (2.73) | 2.71 | 3 (0) | 0 | 39-47 |  |  |  |  |
| ui | O0 | warm | route | 3.17 (2.70) | 2.73 | 3 (0) | 0 | 39-46 |  |  | 1.04 | 0.99 |
| ui | O2 | cold | base | 46.00 (41.97) | 42.93 | 3 (0) | 0 | 17-27 |  |  |  |  |
| ui | O2 | cold | route | 45.30 (42.47) | 43.41 | 3 (0) | 0 | 15-21 |  |  | 0.98 | 1.01 |
| ui | O2 | warm | base | 2.62 (2.63) | 2.72 | 3 (0) | 0 | 20-20 |  |  |  |  |
| ui | O2 | warm | route | 2.77 (2.72) | 2.72 | 3 (0) | 0 | 20-20 |  |  | 1.06 | 1.03 |
| ui | sanitize | warm | base | 2.18 (2.18) | 2.18 | 1 (1) | 0 | 73-73 |  |  |  |  |
| ui | sanitize | warm | route | 2.10 (2.17) | 2.17 | 1 (1) | 0 | 50-50 |  |  | 0.96 | 1.00 |

**`ui --sanitize`**: from its first cold run, about 14:16 (load 83), nearly every run waited (`real` 39.9 to 2,607 s against `user`+`sys` 2.2 to 28.8;
`sysctl vm.swapusage` read 3,732 of 5,120 MB used at 16:53, the machine paging, Amphetamine holding the wake
assertion), so its cold rows are all dropped and one warm run each stands; the table's last run was stopped by its
PIDs at 16:53.

**What it says**:
- **An undeclared `print`**: warm, **1.33 / 1.11 / 1.38** of `old` by `real` at `-O0` / `-O2` / `--sanitize`
  (**1.25 / 1.14 / 1.44** by `user`+`sys`), against the base's **6.4 / 5.8 / 5.9**; cold **1.42 / 1.44 / 1.36**
  (1.34 / 1.40 / 1.37), against **11.7 / 8.5 / 8.8**. The route is 0.12 to 0.23 of the base's cost on `print` at every
  level and mode.
- **With the walk's share taken out**: clang's own calls for a warm `print` (`calls.sh`, 17:03, load about 90):
  `old` 5 calls, 0.285 s in clang; **the route 6, 0.314 s**, the sixth the walk's `clang -E -v`; the base 7, 1.513 s,
  its `-M` over the 43 members among them; none of the route's names `platform/`, its `build/` holds no archive.
  Taking out the walk's carried +0.060 s of `user`+`sys` (§ 4.5's pairs), warm `print` stands at **1.04** of `old`
  at `-O0` and **0.93** at `-O2` by `user`+`sys` (an arithmetic on a median of one sitting and a best of another, an
  inference): **at `old`'s cost**. Cold (`calls-cold.sh`): `old` 8 calls 0.70 s, the route 9 calls 0.81 s, of which
  `runtime.c` 0.48 s against 0.38 s: what stands above `old` cold beside the walk is **the runtime's own object, grown
  since 17c50eba6**, compiled once per cold folder; not the layer.
- **A declared program pays R6, exactly**: `clock`, `audio` and `ui` under the route read **0.88 to 1.07** of the base by
  `real` and **0.93 to 1.04** by `user`+`sys` at every level and mode kept: today's price, no more.

## Travel, priced (ran 17:04)

`programs/compiler-engineer/toml/main.hero`, a program that writes `use <rebel>/toml` and calls `toml.parse` on a
string, declaring nothing: **over the declared library it builds (cold, `real 66.28`, `user 42.51`, the whole of
`toml`'s reach compiled with the layer's archive), prints `true`, and links seven of the layer's families**: time 5,
net 36, disk 35, stream 23, procs 3, sys 32, users 8, the same as `face`'s `time.mark()`, because `toml` uses `time`
and every module reached binds its floor's functions (`emit/extern_refs.hero`). **Over the base's library it is refused
in the floor** (`ffi_layer_undeclared` at `time/hero_time.hero`'s group). So the price of question 5 is exact: a
program reading TOML text pays R6 and links 142 of the layer's functions (the `T` symbols `nm` reads) it never calls; that is the link's
`used` references, which the principle does not move. Whether it *uses a library that needs the layer* is the author's
word; the compiler's answer is yes, by what it links.

## The residue I found in my own route, and closed (ran 16:56 to 17:00)

`programs/compiler-engineer/mixed/main.hero`: one group says `layer` (the sockets' constant) and another, saying nothing,
binds the clock through a header of the program's own that declares it (`owndecl`'s header). **Before the repair it
built at exit 0 and linked the clock's 5 symbols**: the layer was on the line for the first group, so the second's
redeclaration linked unseen. Repaired: where the program declares, a group whose walk reaches nothing and binds a name
a layer header declares is told at that binding, **`ffi_layer_redeclared`**, its own code since 17:00 (`owndecl`
declared reads it too). Case: `tests/golden/unsupported/layer-a-layer-function-declared-by-the-programs-own-header-beside-a-group-saying-layer.hero`.
**What stays open, said**: a header of the program's own that declares a layer function itself and calls it only from
a `static inline` body, with no group binding that name, in a program where another group says `layer`: the walk does
not see it, no bound name names it, and the link succeeds. Seeing it needs the bodies' reading before the link
(`cli/header_bodies.hero`, today asked only after a link fails) or the objects' undefined names by `nm`; an inference
from the code, unrun. It binds the layer through a declaration the layer did not write, which is the robustness
argument for refusing it; it does not corrupt anything the route links that the base did not.

## The harness's own forms over the new cases (ran 17:01)

`heroes run tests/harness/main.hero -- <route> <form> layer-` with the route's stage 2: `check` 1 passed, `unsupported`
4 passed, `annotations` 5 passed, `fixes` 1 passed, each 0 failed. **Unrun**: the `run` form over the 30 declared
goldens, `emission` and the full net.

## The timing table, Linux arm64 (ran 16:53 to 17:03 local, the container's own load 1.7 to 4)

`run-linux.sh` with `PHASE=time PROGS=clock` (the compilers built at 13:28 and kept on the mount; every build folder in
the container's own `/x`): the same rounds and rule as this Mac's. `audio` and `ui` are not timed here, since on Linux
they end at *internal error* at plain and `-O2` under the base and the route alike (above): a timing of a failure.
Every exit 0, nothing dropped:

| program | level | mode | compiler | best real (its user+sys) | median user+sys | kept (dropped) | exits | load | real / old | u+s / old | real / base | u+s / base |
| clock | O0 | cold | base | 5.37 (5.19) | 5.39 | 3 (0) | 0 | 2-3 |  |  |  |  |
| clock | O0 | cold | mine | 5.16 (4.98) | 5.40 | 3 (0) | 0 | 2-2 |  |  | 0.96 | 0.96 |
| clock | O0 | warm | base | 1.25 (1.15) | 1.16 | 3 (0) | 0 | 2-2 |  |  |  |  |
| clock | O0 | warm | mine | 1.27 (1.18) | 1.18 | 3 (0) | 0 | 2-2 |  |  | 1.02 | 1.03 |
| clock | O2 | cold | base | 14.39 (14.22) | 14.28 | 3 (0) | 0 | 3-3 |  |  |  |  |
| clock | O2 | cold | mine | 13.47 (13.30) | 13.42 | 3 (0) | 0 | 3-3 |  |  | 0.94 | 0.94 |
| clock | O2 | warm | base | 1.22 (1.13) | 1.13 | 3 (0) | 0 | 3-4 |  |  |  |  |
| clock | O2 | warm | mine | 1.26 (1.16) | 1.16 | 3 (0) | 0 | 3-4 |  |  | 1.03 | 1.03 |
| clock | sanitize | cold | base | 11.05 (10.88) | 10.93 | 3 (0) | 0 | 2-3 |  |  |  |  |
| clock | sanitize | cold | mine | 9.73 (9.58) | 10.92 | 3 (0) | 0 | 2-3 |  |  | 0.88 | 0.88 |
| clock | sanitize | warm | base | 1.63 (1.54) | 1.76 | 3 (0) | 0 | 2-3 |  |  |  |  |
| clock | sanitize | warm | mine | 1.68 (1.55) | 1.62 | 3 (0) | 0 | 2-3 |  |  | 1.03 | 1.01 |
| print | O0 | cold | old | 0.53 (0.50) | 0.50 | 3 (0) | 0 | 2-2 |  |  | 0.11 | 0.11 |
| print | O0 | cold | base | 4.76 (4.58) | 4.94 | 3 (0) | 0 | 2-2 | 8.98 | 9.16 |  |  |
| print | O0 | cold | mine | 0.80 (0.74) | 0.88 | 3 (0) | 0 | 2-2 | 1.51 | 1.48 | 0.17 | 0.16 |
| print | O0 | warm | old | 0.14 (0.13) | 0.16 | 3 (0) | 0 | 2-2 |  |  | 0.14 | 0.14 |
| print | O0 | warm | base | 0.99 (0.90) | 1.19 | 3 (0) | 0 | 2-2 | 7.07 | 6.92 |  |  |
| print | O0 | warm | mine | 0.21 (0.18) | 0.20 | 3 (0) | 0 | 2-2 | 1.50 | 1.38 | 0.21 | 0.20 |
| print | O2 | cold | old | 1.52 (1.49) | 1.64 | 3 (0) | 0 | 3-3 |  |  | 0.11 | 0.11 |
| print | O2 | cold | base | 13.67 (13.48) | 13.73 | 3 (0) | 0 | 3-3 | 8.99 | 9.05 |  |  |
| print | O2 | cold | mine | 2.34 (2.27) | 2.37 | 3 (0) | 0 | 3-3 | 1.54 | 1.52 | 0.17 | 0.17 |
| print | O2 | warm | old | 0.14 (0.13) | 0.14 | 3 (0) | 0 | 3-3 |  |  | 0.13 | 0.13 |
| print | O2 | warm | base | 1.08 (1.01) | 1.13 | 3 (0) | 0 | 3-3 | 7.71 | 7.77 |  |  |
| print | O2 | warm | mine | 0.23 (0.20) | 0.20 | 3 (0) | 0 | 3-3 | 1.64 | 1.54 | 0.21 | 0.20 |
| print | sanitize | cold | old | 1.17 (1.13) | 1.24 | 3 (0) | 0 | 3-3 |  |  | 0.13 | 0.13 |
| print | sanitize | cold | base | 8.71 (8.59) | 9.06 | 3 (0) | 0 | 3-3 | 7.44 | 7.60 |  |  |
| print | sanitize | cold | mine | 1.63 (1.56) | 1.75 | 3 (0) | 0 | 3-3 | 1.39 | 1.38 | 0.19 | 0.18 |
| print | sanitize | warm | old | 0.24 (0.21) | 0.23 | 3 (0) | 0 | 3-3 |  |  | 0.18 | 0.17 |
| print | sanitize | warm | base | 1.34 (1.26) | 1.42 | 3 (0) | 0 | 3-3 | 5.58 | 6.00 |  |  |
| print | sanitize | warm | mine | 0.32 (0.29) | 0.34 | 3 (0) | 0 | 3-3 | 1.33 | 1.38 | 0.24 | 0.23 |

- **An undeclared `print`**: warm **1.50 / 1.64 / 1.33** of `old` by `real` at `-O0` / `-O2` / `--sanitize` (**1.38 /
  1.54 / 1.38** by `user`+`sys`), the base **7.1 / 7.7 / 5.6**; cold **1.51 / 1.54 / 1.39**, the base **9.0 / 9.0 / 7.4**.
  On Linux the absolute times are small (`old`'s warm `print` 0.13 s of `user`+`sys`), so the route's +0.05 to 0.07 s
  reads as a larger ratio than on this Mac; the walk's share there is unmeasured (this Mac's +0.06 s, carried, would
  leave 0.92 of `old` at `-O0`, an inference).
- **A declared `clock`**: **0.88 to 1.03** of the base by `real` and `user`+`sys` alike: R6's price.

## What the emitted C and the seed owe (item 6; ran 17:01 to 17:12)

`programs/compiler-engineer/cmp.sh`: the base's compiler and the route's each emit **the base's own `selfhost/`** with
`--emit-c` (exit 0 both, **40,396,174 bytes each**), and `cmp` of the two with the line `hero_program_sources` taken out
of each prints nothing: **identical**, and so does `cmp` of the whole files: **byte-identical**. And `print`, `clock`, `audio`, `ui` as written for the base against the same
programs with `layer` for the route, each emitted from a folder of its own under one name: **identical but that line**
(11,200 to 19,974 bytes). The word moves no C a unit holds; only the source key, which every byte of a module moves.
**The seed owes nothing**: the compiler binds `hero_os.h` and `hero_compiler.h` alone, which the rule does not call the
layer's, so `selfhost/` never writes the word, and the seed's own compiler built the route at exit 0 (13:13, 38,125,837
bytes of C). The seed needs to learn `layer` only if `selfhost/` ever binds a member's header, which § 1.11 and
`.claude/rules/rebel.md` keep it from doing.

## What I ran and what I read, apart

**Ran** (each with its command beside its number above): the base's compiler and the route's from the seed and at
`-O2`; the route's stage 2; `heroes check` of the whole compiler after each edit; every shape of `shapes/` and
`programs/` undeclared and declared at plain, `-O2` and `--sanitize`, with `check` and `--emit-c`, on this Mac and in
Linux arm64; the base on the shapes the route's declared builds refused on Linux; the formatter's round trip and
`parse --dump-ast`; `check --apply` and the applied program checked; the five cases and four harness forms over them;
the compiler's own tests whole, twice; the value probe's program; `heroes test` and `heroes run` of a floor's program;
the TOML travel program over both libraries; the mixed residue before and after its repair; clang's calls warm and
cold; the timing tables on both platforms; `cmp` of the emitted C; `land.py` over my copy; `latency.hero compare` (below).

**Read, not run**: that a fifth declaration kind touches 180 files (a `grep` count of `.test_decl`, no kind added);
that `heroes check` runs no walk (the `use` graph; the run agrees); that the inferred switch would accept and refuse
the same programs (an argument from the one-reading construction, no switch built); what (a), (d), (P2) and (f) would
cost a declared build (§ 7.5's carried tables and the code, none built); the static-inline residue (the code); that a
runtime edit making `hero_os.h` include a member's header would be refused under the word and silent under the
switch (the code); the `--sanitize` link on Linux bringing the maths library (an inference from `audio`'s exit 0 there).

**Carried, not re-run by me**: step 182c's walk's warm share, +0.060 s of `user`+`sys` (§ 4.5's pairs), used once to
take the walk out of the warm `print` ratio; the 78 of 115 faces and 735 of 1,914 modules (§ 3.1, the critic's).

**Where it is**: the route as one patch, `docs/panel/260-reports/programs/compiler-engineer/route.patch` (104 file
diffs, 2,558 lines: the compiler, the five cases, the landing over Rebel, `examples/`, the goldens and the harness),
beside every script and program named here, in my copy and copied beside this report; long output in
`.claude/worktrees/260-compiler-engineer-scratch/` (`shapes-s2-*.txt`, `time-mac.txt`, `table-mac.md`,
`linux/results.txt`, `table-linux.md`, `selftest.txt`, `selftest2.txt`, `cmp.txt`, `calls-*.log`, `cold-*.log`,
`latency-compare.txt`). My copy holds the route uncommitted; nothing is committed, pushed or filed.

## The latency instrument (item 5's last ask; ran 17:07 to 17:12)

`heroes run tests/harness/latency.hero -- compare <base> <route> --clock build --runs 6`, the route's stage 2 running
the harness, from my copy's root: **exit 2, no verdict**, *"0 pairs kept, fewer than 5: no verdict, the machine was not
still"*: every run waited more than 0.1 s past its own work (load 8.17 before, 5.19 after; other sessions). The runs
themselves (`scratch/latency-compare.txt`): the route's first build was cold (`real 101.25`, its own cache key) and is
the discarded pair; then the base 19.37 to 21.97 s and the route 19.93 to 21.76 s of `real`. **Whether the check moves
the compiler's own speed**: `heroes check selfhost/main.hero` gains one rule that reads a string per group (the
compiler's 33 groups), and its clock was not compared; the build clock is where the route moves, by the members' `-M`
(0.788 s at load 5), which the instrument could not resolve at this load. Owed: the comparison on a still machine,
which is my prediction's measurement.
