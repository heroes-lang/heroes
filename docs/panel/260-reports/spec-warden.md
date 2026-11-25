# Panel 260, the spec-warden

Written 2026-11-25 from 12:40 on, as I go. My copy: the detached worktree `.claude/worktrees/260-spec-warden` at
`c3885b81b` (`git log -1`: `c3885b81b3d9c98b012cee1465c23c53282844a7`, clean). Long output in
`.claude/worktrees/260-spec-warden-scratch/`. Load: `uptime` 15.45 at 12:40, 184.66 at 12:44, 170 at 12:46.
**No paid run, no API call, no `--refresh`**: the coordinator's prompt authorises none, so **every count below is a
lower bound, vendored only**, in the words `.claude/rules/spec-shape.md` § How a change to the document is made asks.
Where I give a real figure it is an **inference** from the ledger's ratio, and it is marked so.

Every line says **ran** (a command I ran) or **read** (a file I opened).

## Status

Done about 13:30: ten texts written and measured with the base's compiler (stage 2), seven pieces of the group-mark
text priced, the `spec` suite run on the group-mark text, the `grammar` suite on four texts, predictions registered,
verdict at the end. My copy's spec restored after every run (sha256 `c915f0f2...2bee`, `git status --porcelain`
empty). No container, no paid run, no commit.

## What binds, read today

- **Read** design.md §1.6 by `grep -n` (line 257): *"must fit in 16384 tokens, measured by `claude-opus-5` through
  `POST /v1/messages/count_tokens`"*. The payment rule (lines 326 to 328): *"Every amendment owes a named removal or a
  registered prediction naming an instrument that exists today"*; a registered prediction pays only if it names the
  instrument and the milestone (lines 331 to 336). The consolidation block's 1,500 real (lines 293 to 296) binds
  *"before the thesis harness has run"*; lines 391 to 394 say metric 2 first ran on 2026-10-24, so I read that cap as
  spent and not binding here (an inference from the two sentences; no ledger row says so). §1.2 (line 195): *real cost
  = program tokens x (1 + rewrite rate)*; line 392: the rewrite rate measured 0.198 strict, a round trip's median
  **3,619 tokens** (measurement 052). §1.3 (lines 213 to 219): *"A construct is good for an LLM if the meaning of a
  line can be determined from that line plus the signature of the enclosing function"*; the category refused is
  *"constructs whose meaning lives elsewhere"*.
- **Read** my agent file of the copy (`.claude/agents/spec-warden.md:45-49`): no veto holds against the most robust
  resolution (CLAUDE.md § Precedence, the author's amendment of 2026-11-22); I report its cost.
- **Read** `tests/harness/suite_spec.hero`: `SPEC_TOKENS` 10054 (line 115), `CEILING` 16384 (162), `REAL_TOKENS` 13185
  (176), `DELTA_GATE` 50 vendored per commit (217), `FFI_FLOOR` 60 (302).
- **Read** the spec whole at the base (516 lines). **Read** design.md §3.5 (lines 934 to 936): *"a dependency is a link
  flag declared in the source next to the `extern` that needs it, or a package the program's catalogue names"*; §1.11
  line 505, R6 (*"whose members the linker takes into a program only where the program calls them"*), which
  `00-shared.md` § 6.2 measured false (`testonly`, `face`); `.claude/rules/cli-surface.md:73-76`; panel 234 at lines
  416 to 420 (*"found and linked by the compiler with no `link` word or flag in a package"*).

## The counts at the base (ran)

My compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 33.09`, `user 21.51`; `--version`
`sources 05186f2b38e77a97`), then `./heroes build selfhost/main.hero -o heroes-next` (`real 482.23`, `user 235.57`,
`sys 58.48`; `--version` **`sources 41f32a8f4b745c0c`**, the key the brief names for the base's compiler;
`260-spec-warden-scratch/build.log`).

`./heroes-next measure spec/heroes-spec.md` from my copy's root (`measure-base-stage2.txt`): claude-legacy **9,877**,
cl100k_base **10,054**, real **13,185** (`claude-opus-5, 2026-11-23`, no `STALE`), headroom **3,199** against
**16,384**, **13,245** with the FFI floor. File: 516 lines, 34,437 bytes, sha256 `c915f0f2...2bee` (`wc -l -c`,
`shasum -a 256`). **Every number of the brief held.** Stage 1 (the seed's compiler) reads the same two vendored rows
and prints `STALE: the recorded count is for 4dced6767460e590` (`measure-base-stage1.txt`): the seed carries an older
pin, the shape `.claude/rules/verification.md` § The compiler that judges is a build artifact describes; stage 2 is
what I measured with, and stage 1 and stage 2 read the same vendored count on every copy below
(`measure-texts-stage1.txt`, `measure-all-stage2.txt`).

## 1. The sentence each home owes, written and measured (ran)

**How**: `260-spec-warden-scratch/make_texts.py` applies each text to a copy of the base's spec (every anchor asserted
to match exactly once), writes the copy to `260-spec-warden-scratch/copies/<text>/spec/heroes-spec.md`, the
replacement text to `docs/panel/260-reports/<text>.txt` and `diff -u` against the base to `<text>.diff` (labels
`a/spec/heroes-spec.md`, `b/spec/heroes-spec.md`); `./heroes-next measure <copy>` from my copy's root. **Every text
merges into line 401** and takes out its *"linked where a group names one"*, which the principle falsifies for an
undeclared group (`spec-shape.md:67-69`, *merging beats appending*). Every text shares one clause, written once and
identically so the homes are compared on what differs:

> *processes and files from the language's `hero_os.h`, and sockets, clocks and the rest from its platform layer:
> `hero_net.h`, `hero_time.h` and every other `hero_` header it provides but the compiler's `hero_compiler.h`. A group
> whose header is one of these or includes one at any depth, or that binds a name one declares, ...*

The real column is **an inference, not a measurement**: the ratio of real to vendored delta over the four newest
ledger rows that carry both (`docs/measurements/010-spec-budget-ledger.md` rows 9826 to 10054, read: +65 to +88,
+141 to +177, +206 to +265, +87 to +111, so 1.25 to 1.36), applied to the vendored delta.

| text | home | what the reader writes | legacy | cl100k (max) | real, inferred |
|---|---|---|---|---|---|
| `spec-w1-group-mark` | a mark on the group, § 13 | `extern "hero_time.h" layer` | +76 | **+76** | +95 to +103 |
| `spec-w1n-group-mark-native` | the same, the author's word | `extern "hero_time.h" native` | +76 | +76 | +95 to +103 |
| `spec-w1x-group-mark-no-compiler-h` | w1 without the `hero_compiler.h` exception (**false** of the tree: priced only) | as w1 | +66 | +66 | +83 to +90 |
| `spec-w2-module-line` | a new kind of top-level line (the seventh: § Where the brief was false or silent), per module, § 4's production, the rule in § 13 | a line `layer` in the module holding the group | +81 | +83 | +104 to +113 |
| `spec-w2f-face-line` | the same line in the package's face module (the brief's literal home) | a line `layer` in `ticks.hero`, a program's in the file compiled | +112 | +114 | +143 to +155 |
| `spec-w3-catalogue-key` | a key of the package's own catalogue, § 1 amended | `layer = true` atop `ticks/heroes.toml`; a program's own `heroes.toml` | +98 | +99 | +124 to +135 |
| `spec-w4-manifest` | a manifest file per package | an empty `heroes.layer` in the package's root, beside a program's file | +91 | +91 | +114 to +124 |
| `spec-w5-link-word` | the `link` word the group has | `extern "hero_time.h" link "layer"` | +70 | +70 | +88 to +95 |
| `spec-w6-file-name` | the floor's own file name | the group moved into a module named `hero_<name>.hero` | +74 | +74 | +93 to +101 |
| `spec-w7-use-layer` | a `use` of the layer in the module that binds, § 1's production, the rule in § 13 | `use <layer>` | +86 | +85 | +107 to +116 |

**The pieces of w1, priced one at a time** (`price_pieces.py`, `pieces/`, cl100k): the header rule alone, *"linked
where a group names one"* taken out, **+16**; the production's `[ "layer" ]` **+5**; the declaration over a header
that names the layer, with the link clause, **+40**; *"or includes one at any depth"* **+6**; *"or that binds a name
one declares"* **+9**; the link clause (*"a build links the layer only where a module it compiles holds such a
group"*) **+17**; the `hero_compiler.h` exception **+10** (w1 against w1x). Two denser wordings of w1's same claims:
one sentence fused with the examples kept, **+73**; fused, with `hero_net.h`, `hero_time.h` and *"it provides"*
taken out, **+56** (`p6-w1-smallest`), which leaves `hero_time.h` named nowhere in the spec and a header of the
program's own named `hero_*.h` undecided by the text. I keep w1 as written (+76): 20 vendored tokens against a
reader who must map *clocks* to a header the document no longer names.

**What this says about the homes**: the clause every home shares (the header rule +16, what reaches the layer +6 and
+9, the link clause +17, the pieces above) is **about +48** of w1's +76, the principle's own price, which every home
pays: a reader cannot predict a refusal without being told which headers are the layer's, what reaches one, and that
an undeclared build links nothing of it. The homes differ by **44 tokens** between the cheapest true text (+70, w5)
and the costliest (+114, w2f). **The spec's cost does not choose the home; locality, the instrument that holds it and
what each home falsifies elsewhere do** (§ 3, § 4).

**Ran: the net's `spec` suite on `spec-w1` applied in my copy** (the base's spec copied aside with its sha256,
`spec-w1`'s copy put in place, `./heroes-next run tests/harness/main.hero -- ./heroes-next spec`, then restored and the
sha256 compared equal, `git status --porcelain` empty; `suite-spec-w1.txt`, `real 229.23`, `user 114.33`): **32
passed, 4 failed**, and the four are the budget's record checks a landing moves by hand (`spec/budget` *"the spec
measures 10130 and the record says 10054, a delta of 76"*, `spec/spendable`, `spec/real`, `spec/ledger`). Every check
of the text's form passed (`shape`, `anchors`, `named`, `rejected`, `inventory`, `offered`), the new terminal
`"layer"` included: no `spec` check holds a production's quoted words to the parser (`named` holds the reserved
built-ins, `suite_spec.hero:762`).

**Ran: the net's `grammar` suite on the same text, the same way** (`suite-grammar-w1.txt`, `real 42.67`, restored and
the sha256 equal, `git status --porcelain` empty): **11 passed, 1 failed, 1 floor outgrown**, and the failure is the
instrument working: `FAIL grammar/extern-marks`, *"`layer`: quoted by the spec's `Extern`, and
selfhost/parse/group_head.hero reads no `keywords.WORD_LAYER`"*, with `FLOOR grammar/extern-marks` *"3 against a floor
of 2"*. That check is *"the one production held to the parser"* (`suite_grammar.hero:524-535`, panel 240, both
directions). So **w1's word is held to the compiler by an instrument that exists today**. **Ran the same for w2, w7
and w5** (`suite-grammar-spec-w2-module-line.txt`, `-w7-use-layer.txt`, `-w5-link-word.txt`; restored after each, the
sha256 equal, `git status --porcelain` empty): **12 passed, 0 failed** each, with no parser that reads their word: w2's
`"layer"` in `Declaration` and w7's in `Use` are held to the compiler by no check, and w5's `"layer"` is a string no
check reads. A landing of w1 raises `LEAST_MARKS` to 3 in the same commit, as the floor's
message asks.

**+76 vendored is past `DELTA_GATE` (50)**, so the landing commit's body says what paid (CLAUDE.md, the contract's
opening).

## 2. One home for the rule

The declaration's form and the refusal that holds it govern **a group** (what it binds, what it links), so their home
is **spec § 13 FFI**, the section of that operation (`spec-shape.md` § Where a rule lives). Under **w1** and **w5**
the form, the rule and the production all stand in § 13: **no pointer is owed**, and line 401, where a reader of § 13
starts, is where the rule is. Under **w2** the form's production stands in § 4 and the rule in § 13, the pointer
*"(section 4)"* priced inside +83. Under **w7** the form is § 1's `use` production and the rule § 13's; § 1 says
*"`use geom` binds `geom`"*, and a `use` that binds no name is a second rule of § 1 a reader meets without its
reason: a pointer from § 1 is owed and **not priced** (about +5, an inference). Under **w3** § 1's catalogue sentence
must change (priced inside +99, *"has no catalogue unless it binds the platform layer (section 13)"*), and the
catalogue's rules outside the spec move too (below). Under **w4** and **w6** § 13 alone, but each makes a file or a
file's name carry a rule of § 1's subject (files and modules) that § 1 never states.

## 3. What a reader must be able to predict (read: the text against `00-shared.md` § 6.2's shapes)

**The rule, not a list.** The text gives a rule on the header's name with two named exceptions: every `hero_` header
the language provides is the layer's but `hero_os.h` (the language's) and `hero_compiler.h` (the compiler's). Ran:
`ls runtime/hero_*.h` 43 files, of which every one but `hero_os.h` and `hero_compiler.h` is a member's header (the
loop of `runtime/<b>.h` against `runtime/platform/<b>.c`; the 41 named, the arms' own among them); no tracked header
outside `runtime/` is named `hero_*.h` (`git ls-files '*.h'`, 0); every tracked group naming a `hero_` header names
`hero_os.h` (150 group lines), `hero_compiler.h` (12) or a member's header (`git ls-files -z '*.hero' | xargs -0 grep
-hoE '^extern "[^"]*"'`). So the rule holds today with no exception beyond the two it names, and **W4's
`hero_activation.h`, which the spec names nowhere, falls under it**. A new member named `hero_<x>.h` falls under it
too, so no member falsifies it; **what would** is a new runtime header named `hero_` that no member defines, and that
is what the instrument of § 4 holds.

**Why `hero_compiler.h` must be named, or renamed** (read: `runtime/hero_compiler.h:7-8`, *"the doors the compiler
binds for itself, which no program needs"*): without the exception (`spec-w1x`, 10 tokens cheaper) the rule says the
compiler's own groups bind the layer, so the 12 tracked groups naming it, the compiler's own modules among them, would
owe a declaration and link the layer, which the author's premise (*"we built the whole compiler ... without a standard
library"*) and every build of the compiler contradict. **Two repairs**: the text names it (+10 vendored, a reader told
of a header no program needs), or the tree renames it out of the `hero_` prefix (as the runtime's own `heroes_*.h`
are named), which costs the spec nothing and the seed a regeneration (`runtime/hero_compiler.h:27-31` says the
committed seed still reads it). The rename is the compiler-engineer's to price; the text stands either way.

What each shape gets under **w1**, from the text alone (each home answers the same way unless its row says otherwise):

| shape | the text says | undeclared | declared |
|---|---|---|---|
| a group naming a member's header (`clock`, `audio`, `ui`, `twofam`, `usemod`'s module, the 32 floors, W1, W3) | must say `layer` | refused at that group, the package's own file | builds, the layer linked |
| a header of its own including one at any depth (`ownhdr`, the ten `run` goldens, the tenth two deep) | must | refused at that group | builds |
| a header of its own declaring a layer name and including none (`owndecl`) | must (*"binds a name one declares"*) | refused | builds |
| `extern "platform/hero_audio_arm.h"` (`platformhdr`) | must, **if the check reads the header's file name**, its last part (a condition) | refused | builds |
| a group naming `hero_net.h` that binds only a constant (`constonly`) | must (its header is one of the layer's) | refused | **builds**: the text reads *binds* and *binds nothing* by the one header rule, never by the link |
| a used module whose binding only a `test` calls (`testonly`) | must | refused | builds, and the program links the layer though `main` never reads the clock |
| a face over a declared floor (`face`, W1's `main.hero`) | holds no such group, writes nothing | | builds, the layer linked through the floor |
| `hero_os.h` alone (W2; Rebel's five `hero_os.h` floors and fixtures) | writes nothing | builds, nothing of the layer | `layer` there refused (*"no other group may"*) |
| `layer` on a group over `sqlite3.h` | | builds | refused |
| a program importing a declared package and binding nothing itself | writes nothing | | builds, the layer linked |
| a header of the program's own named `hero_mine.h` | not the layer's (*"it provides"*) | | **a question, unrun**: the check must say where it found the header, not only its name |

**Two conditions the text puts on the check, both robustness (CLAUDE.md § Precedence rank 3)**: (i) *binds* and
*binds nothing* are read by **one** reading, the header's, or `constonly` is refused declared and undeclared alike
(`00-shared.md` § 6.2: a correct program refused is `blocking`); (ii) *"at any depth"* is step 182c's walk
(`00-shared.md` § 4.5), which reads every `#if` but `#if 0` as true, so a header including a member's header under a
never-taken `#if` must declare: the loud direction, and consistent with the text, because a reader reading the C sees
the `#include` too. And one condition on design.md: §1.11 line 505's *"only where the program calls them"* is measured
false (`testonly`, `face`), and **no text of mine says it**: each says *"where a module it compiles holds such a
group"*, which is what the link does.

**Per home, where they differ**:
- **w5, the `link` word**: the cheapest explicit form (+70), but `Extern` takes **one** string after the header (`[ (
  "link" | "pkgconfig" ) string ]`), so a group whose header of its own binds both a C library and the layer cannot
  say both: `link "layer"` displaces the `link "sqlite3"` it needs. Under w1 the production's `[ "layer" ] [ ( "link"
  | "pkgconfig" ) string ]` says both. And `link "layer"` reads to a reader as `-llayer`, a library of that name,
  which a system may hold: one word with two meanings, the shape §1.4's *different but valid* warns of. And panel
  234's condition, *"no `link` word or flag in a package"*, is the ffi-pragmatist's to lift.
- **w3, the catalogue key**: the meaning of the `extern` line, whether the build links the layer, lives in another
  file: design.md §1.3's refused category, *"constructs whose meaning lives elsewhere"*. It falsifies § 1's *"a program
  that uses no package has no catalogue"* (W3 and W4 must now write one: amended inside +99), and outside the spec
  `.claude/rules/cli-surface.md:69-76`'s *"and nothing else"*, *"A program with no `use <name>/…` has no catalogue
  and reads none"* and *"a library or package of spec § 13 ... a mode ... is refused by the reader, never ignored"*:
  `layer = true` names a § 13 library (line 401 calls the layer where § 13's headers come from) and switches the
  build's link, a mode; the clause must admit one key by name, and design.md §3.5's *"and nothing else"* with it.
  Its falsifier survives (deleting the key changes whether the program builds, never which module compiles). And
  Rebel's 21 packages have no catalogue today (`00-shared.md` § 3.2), while `rebel/examples/reach/heroes.toml` names
  `time/hero_time` as a package of its own, so *the package* that declares is not one folder.
- **w4, the manifest**: a second per-project file beside the catalogue, **a fifth input class**
  (`.claude/rules/cli-surface.md:84-87`), refused on panel 056's conditions; and §1.3's objection as w3.
- **w2f, the face line**: the spec has no *face*. § 1 reaches *"the module `parse` of the package `json`"*, any module
  of it; *"its module named as the package"* (my text's best definition) names a file W1's task does not have
  (`ticks/` holds `clock`), and Rebel's floors are packages of their own. Costliest (+114) and ill-defined.
- **w2, the module line**: well defined, +83, the declaration in the file that binds but not on the line; § 4 gains a
  kind (*"Every top-level line starts with its kind"* holds).
- **w6, the file name**: no syntax, but W3's and W4's *"write a program `w3/main.hero`"* can then hold no binding (the
  group must move into a module named `hero_<name>.hero` that `main` uses), Rebel's three `hero_os.hero` files
  (binding `hero_os.h` alone) and `math/hero_math.hero` (binding none) would be refused (*"each so named must hold
  one"*; `00-shared.md` § 3.1, read), and the 20 tracked `run` goldens binding a member split in two. The rule is in a name, not in a word a reader
  sees as a declaration.
- **w7, `use <layer>`**: a `use` that binds no name against § 1's *"`use geom` binds `geom`"* and § 5's *"An unused
  binding ... is a compile error"* read beside it; the pointer from § 1 unpriced (§ 2).

## 4. The instrument (read; none built)

design.md §1.6's payment rule asks a named removal or a prediction naming an instrument that exists. **The named
removal**: *"linked where a group names one"* (netted inside every delta above); nothing else comes out, and **that
is a problem I state rather than solve**: the cheapest real removal I found is the `hero_compiler.h` exception, 10
vendored tokens, bought by a rename in the tree. **The payment is therefore the registered prediction below**, scored
by an instrument that exists today: the blind run's runner (panel 256's, `docs/panel/256-briefs/blind/`, adapted as
`blind/registration.md` says), on this sitting's registered tasks t1 and t2, at the run the coordinator starts after
the synthesis (authorised by the author, ceiling 20 USD), before ratification.

**What must hold the sentence's words to what the compiler refuses**, as `tests/harness/spec_refusals.hero` holds the
signal sentence's two claims to `hero_signal_refuse`'s arms: a module of the `spec` suite, `spec_layer.hero`, reading
(a) the sentence's backticked `hero_*.h` exceptions and holding them to the runtime: every `runtime/hero_*.h` and
`runtime/platform/hero_*.h` is a member's (`runtime_key.layer_members`), an arm's or under `platform/`, or is one of
the sentence's exceptions, and each exception is no member's, so a new runtime header named `hero_` that no member
defines turns it red; (b) the contextual word the sentence and the production write against `keywords.hero`'s
constant, which `grammar/extern-marks` already holds for w1 (§ 1, run) and nothing holds for w2 or w7; (c) one annotated `check/` golden per refusal the sentence makes (a member's header undeclared, an own
header two deep undeclared, a bound layer name undeclared, `layer` over `hero_os.h`, `layer` over a header of the
program's own reaching none) and `run` goldens for `constonly` declared and for an undeclared program whose binary
holds no `_hero_<member>_` symbol (`nm`, as `00-shared.md` § 6.2 reads members taken); and (d) the replacement of
`layer.hero:330`, R6's *"every link carries the layer"*, which the principle falsifies.

## 5. The importer's side

Spec § 1, lines 35 to 36: *"a program that uses no package has no catalogue"*. A program that binds the layer itself,
with no package (W3, W4):
- **w1, w5**: `layer` (or `link "layer"`) on its own group. No catalogue; § 1's sentence stays true.
- **w2, w7**: the line `layer` or `use <layer>` in its own module. No catalogue.
- **w2f**: the line in the file compiled.
- **w3**: a `heroes.toml` with `layer = true` and no dependency; § 1 and `cli-surface.md:72-73` amended.
- **w4**: an empty `heroes.layer` beside the file compiled; a fifth input class.
- **w6**: its group moved into a module named `hero_<name>.hero`, which `main` uses.

A program importing a declared package writes nothing under every home: the layer travels by `use`, the brief's
question 5, and **78 of Rebel's 115 faces reach a floor** (`00-shared.md` § 3.1, read, not re-run): the text says
*where a module it compiles holds such a group*, so a TOML reader pays the layer, which is the author's word to apply.

## Predictions registered before the blind run

Registered 2026-11-25 at about 13:20, before any text is adopted and before any session; no session of the run has
started (`blind/registration.md`: the coordinator starts it after the synthesis). Each prediction is a number and a
range, for each of my texts, under that text as `adopted` (t2's programs then in its spelling) and under `now`. **How
each number reads**: **D1** the readers of 5 declared-right on W1, then on W3; **D1c** declared-right on W4; **D2**
the readers of 5 who declare anything in W2; **D3** first the number of t2's six programs on which at least 4 of 5
readers are build-right (a refusal's file named too), then the readers who name the importer's file for (2) or (5);
**D4** the median, over a t1 arm's five readers, of the spec's sections a reader's `choice_points` cite for the
declaration. A range is inclusive. `spec-w1n` and `spec-w1x` carry `spec-w1`'s numbers (another word, and an exception
no job meets).

**Under `adopted`**:

| text | D1 W1 | D1 W3 | D1c W4 | D2 | D3 programs | D3 importer | D4 |
|---|---|---|---|---|---|---|---|
| `spec-w1-group-mark` (and `w1n`, `w1x`) | 5 [4-5] | 5 [4-5] | 5 [4-5] | 0 [0-1] | 5 [4-6] | 0 [0-1] | 1 [1-2] |
| `spec-w2-module-line` | 4 [3-5] | 5 [4-5] | 5 [4-5] | 0 [0-1] | 5 [4-6] | 0 [0-1] | 2 [1-2] |
| `spec-w2f-face-line` | 2 [0-4] | 4 [3-5] | 4 [3-5] | 0 [0-1] | 3 [1-5] | 1 [0-2] | 2 [1-2] |
| `spec-w3-catalogue-key` | 3 [1-5] | 4 [2-5] | 4 [2-5] | 0 [0-1] | 4 [2-6] | 1 [0-2] | 2 [1-3] |
| `spec-w4-manifest` | 4 [2-5] | 4 [2-5] | 4 [2-5] | 0 [0-1] | 4 [3-6] | 0 [0-2] | 1 [1-2] |
| `spec-w5-link-word` | 5 [4-5] | 5 [4-5] | 5 [4-5] | 0 [0-1] | 5 [4-6] | 0 [0-1] | 1 [1-2] |
| `spec-w6-file-name` | 3 [1-5] | 3 [1-5] | 3 [1-5] | 0 [0-2] | 4 [2-6] | 0 [0-1] | 1 [1-2] |
| `spec-w7-use-layer` | 4 [3-5] | 5 [4-5] | 5 [4-5] | 0 [0-1] | 5 [4-6] | 1 [0-1] | 2 [1-2] |

**Under `now`** (the base's spec, the same for every text but D3, whose programs are written in the adopted text's
spelling): D1 **0 [0-0]** on W1 and **0 [0-0]** on W3, D1c **0 [0-0]** (by construction, `blind/registration.md`);
D2 **0 [0-1]**; D4 **0 [0-1]**. D3 programs: **1 [0-2]** for `w1`, `w1n`, `w1x`, `w2`, `w2f`, `w7` (a word `now`'s
grammar does not read: a reader predicts the declared floor, the face and the importer refused, and only (6) right);
**3 [2-3]** for `w5` (`link "layer"` is legal under `now`, so (1), (3) and (4) are predicted to build) and for `w4` and
`w6` (an empty file and a module's name `now` reads as nothing); **2 [0-3]** for `w3`. D3 importer: **0 [0-1]** for
every text.

**Why these and not others** (each an inference from the texts, none run): the group mark (w1) and the `link` word
(w5) put the declaration on the line a reader already writes for W1, W3 and W4, and the rule names the header by a
pattern W4's `hero_activation.h` meets, so D1 and D1c are near ceiling; a home in another file (w3, w4) or in a module
the task does not name (w2f's face, w6's `hero_<name>.hero`) makes a reader choose a file, and the grader's table
counts any other file as wrong; the module line (w2) and `use <layer>` (w7) let a reader also write it in `main.hero`
of W1, thinking the need travels by `use`. D2 is near 0 for every text, since each says `hero_os.h` is the
language's.

**The spend**: the 31 sessions **6.8 USD [4.5-10.0]** (panel 257's measured 6.30 for 31, `llm-ergonomist.md`; t1 now
asks four programs written).

**Which I expect**: **`spec-w1-group-mark` adopted** (in the author's word `native` if the synthesis prefers it,
`spec-w1n`, the same count), and **`spec-w3-catalogue-key` the rival**, the home that reads *"a library declares"* at
the package's level; if the rival is kept inside the source, `spec-w2-module-line`.

## Principle 0 and §1.2, read against the principle

- **The compiler does not need the form**: its groups name `hero_os.h` 23 times, `hero_compiler.h` 10 and
  `stdlib.h` once, and no member's header (ran: `git ls-files -z 'selfhost/*.hero' | xargs -0 grep -hoE '^extern
  "[^"]*"' | sort | uniq -c`), so under every text that names the `hero_compiler.h` exception the compiler's own
  modules write nothing.
- **The thesis effect is unmeasured** until the blind run, and **under §1.2 alone the form does not pay**: today a
  program binding the layer needs no word, so the word adds a mistake class (forgetting it), each costing one round trip
  (median 3,619 tokens, design.md §1.6 line 393), while the text costs about +100 real tokens in every prompt
  (inferred). The diagnostic's fix can be `certain` under w1 (insert ` layer` after the group's string), so the round
  trip is mechanical, and D1 bounds how often it is paid.
- **So Principle 0's burden is met only by rank 3**: the author's principle is the robust resolution (an undeclared
  build links nothing of the layer, so a member that does not compile, defect 2788's class, or a planted header,
  `d-lane-layer-speed-1`, stops only declared programs), and since the author's amendment of 2026-11-22 Principle 0
  and the budget yield to it. **Neither of my vetoes holds**; I report the cost.
- **What conservative would have been**: the coordinator's inferred switch (S), no word: the text would still owe the
  header rule and the link clause (+16 and +17 of the pieces, about +33 vendored), and no refusal. The author refused
  it (`00-shared.md` § 7.6).

## Where the brief was false or silent

- `00-shared.md` § 7.1 and this brief's item 1 speak of *"a fifth kind of top-level declaration in § 4"*, after
  `spec-shape.md`'s table (*"the four kinds"*): § 4's `Declaration` has **six** alternatives (`constant`, `function`,
  `record`, `variant`, `test`, `Extern`; read), so a new line is the seventh. No sentence of the spec says *four*.
- The brief offers `tests/harness/spec_refusals.hero` as the model instrument and is **silent on
  `grammar/extern-marks`**, which already holds a word of the group's head to the parser both ways (run above): it is
  the one existing instrument that discriminates between the homes.
- `00-shared.md` § 7.1's *"a line in the package's face module"*: the spec has no face (§ 3, w2f), and the brief does
  not say how a reader finds it.
- design.md §1.6's 1,500-token consolidation cap: the brief is silent; I read it as spent (an inference, § What binds).
- Every count of the brief's § 3.3 and of this brief's *The counts* held (ran, above).

## Verdict

- `verdict`: **approve `spec-w1-group-mark`, provisional** (every count a lower bound, vendored only; no real count
  authorised), with the conditions below; **object** to the catalogue key (w3), the manifest (w4), the face line
  (w2f), the file name (w6) and the `link` word (w5) as homes.
- `section`: design.md §1.6 (the payment rule, lines 326 to 328 and 331 to 336), §1.3 (locality, lines 213 to 219),
  §1.2 (line 195); CLAUDE.md § Precedence as amended 2026-11-22.
- `spec_token_delta`: cl100k_base 10,054 to **10,130 (+76)**, claude-legacy 9,877 to 9,953 (+76), **a lower bound,
  vendored only**; real 13,185 to about 13,280 to 13,288 (+95 to +103), **an inference** from the ledger's ratio of
  1.25 to 1.36. Every other home: § 1's table, +66 to +114.
- `removal`: *"linked where a group names one"* (netted). Nothing else, **and that is a problem**: the one real
  removal found is the `hero_compiler.h` exception (10 tokens), bought by renaming the header in the tree. The
  payment is the registered prediction, scored by the blind run that exists (panel 256's runner, this sitting's
  registration) before ratification.
- `needed_for_self_hosting`: **no**.
- `argument`: Every home must tell a reader which headers are the layer's, what reaches one and what an undeclared
  build links: about +48 of +76 is the principle's own price. What separates homes is locality and instruments. The
  group mark is the only home whose meaning stays on the line that binds (§1.3), whose word `grammar/extern-marks`
  already holds to the parser, that combines with `link`, and that leaves § 1's *"no package, no catalogue"* true.
  The catalogue key moves meaning to another file and breaks the catalogue's *"nothing else"*; the manifest is a
  fifth input class; the `link` word takes one string. Under §1.2 alone the word costs; rank 3 admits it.
- `prediction`: the landed `spec-w1` reads **13,280 to 13,288** real on `heroes measure spec/heroes-spec.md
  --refresh`; under it, the blind run reads D1 at least 4 of 5 on W1 and on W3, and D1c at least 4 of 5 on W4.
- `condition`: I move off w1 if, under it, D1 or D1c reads below 4 of 5 (the text does not teach the form); if the
  compiler-engineer shows the check cannot read *binds* and *binds nothing* by the one header rule (`constonly` then
  refused both ways, and the text must change); or if a shape the group's header never shows (a `link` library's
  object calling the layer, a `dlsym` door) must declare too, which a package-level home could say and a group mark
  cannot.

**One line per question**:
1. **The sentence**: `spec-w1-group-mark`, **+76 vendored, a lower bound, vendored only** (real +95 to +103,
   inferred); what would change my mind: D1 or D1c below 4 of 5 under it in the blind run.
2. **One home**: § 13, the production and the rule together, no pointer owed (+0); what would change my mind: an
   adopted home outside § 13, which then owes a priced pointer.
3. **What a reader predicts**: the rule on the header's name, every `hero_` header the language provides but
   `hero_os.h` and `hero_compiler.h`, never a list (+16 of the +76); what would change my mind: a runtime header named
   `hero_` that no member defines, or a check that cannot read `platform/hero_audio_arm.h` by its file name.
4. **The instrument**: `grammar/extern-marks` (exists, run: it fails until the parser reads `WORD_LAYER`) plus a
   `spec_layer.hero` holding the two exceptions to the runtime and one annotated golden per refusal; the payment the
   registered prediction above; what would change my mind: a landing whose `grammar` passes with the parser
   unchanged.
5. **The importer's side**: under w1 a program binding the layer itself writes `layer` on its own group and has no
   catalogue; one importing a declared package writes nothing and pays the layer; what would change my mind: the
   author reading *"uses a library that needs the layer"* as excluding the 78 faces that reach a floor only by `use`,
   which the text's link clause would then have to say.

## The texts and their diffs, by hash

Each `spec-w*.txt` beside this report holds the replacement text, each `spec-w*.diff` the `diff -u` against the base's
`spec/heroes-spec.md` (`c915f0f2...2bee`), labels `a/spec/heroes-spec.md` and `b/spec/heroes-spec.md` (`shasum -a 256`):

| diff | sha256 |
|---|---|
| `spec-w1-group-mark.diff` | `af4718ccae74d423f9a733ecf767bbed21a9c20de237e47cd1bc7844ed6f83c8` |
| `spec-w1n-group-mark-native.diff` | `5482399bdd6e019c8101685d951243d990f4859a0a01dc0b08def1f91a9a6e6c` |
| `spec-w1x-group-mark-no-compiler-h.diff` | `f295048f5737672814e85732378a9e22548c455dedbf9bb7ef4e13085a37d359` |
| `spec-w2-module-line.diff` | `fec571faa2f4e3c9b17b378b14475aed8fc1a075a747bd88d645016f6aed2483` |
| `spec-w2f-face-line.diff` | `6ad639c4d6fe4ef5268f2b39b32461754c00fdc5e992e123943f7cc15ff63449` |
| `spec-w3-catalogue-key.diff` | `d1f1465410f47f70a44684a073ac84fadec9e3e386614c874d6bf670eb4cec46` |
| `spec-w4-manifest.diff` | `22c6f9a1ed1cd9ec495b6dbbec542558f930edf3b91358867c280351f5543857` |
| `spec-w5-link-word.diff` | `ddc7affa1db068e60f5195a4300bc0ff3be76fb15b880e142bf685b2a8212e03` |
| `spec-w6-file-name.diff` | `8b1abd8b5e749dde3c7a352c39512b2c0060320bf1465d277b878fd5c031b6ac` |
| `spec-w7-use-layer.diff` | `0765091102f52c2b9762f83f1d0aa9666d2d596a69335adb3a548054daccc10a` |
