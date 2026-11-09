# Panel 239, the spec-warden's report

Seat: spec-warden. Worktree `.claude/worktrees/239-spec-warden/`, detached at `1c4bc5197`
(`git log --oneline -3`). Written as I go; every number has its command beside it.

## 0. The budget, read today

- `grep -n '16384' docs/design.md`: design.md §1.6 line 255 reads *"must fit in 16384 tokens,
  measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*; line 286 *"Raised again
  to 16384 by author decision 2026-10-14"*, with the consolidation block's 1,500 real tokens before
  the thesis harness has run. The ceiling binding this sitting is **16384 real**, read today.
- The payment rule (§1.6, the paragraph after *The soft line was 2000*): every amendment owes a
  named removal or a registered prediction naming an instrument that exists today; unconditional.
- Pricing: vendored tables only (no `--refresh`, unallowed). `.claude/rules/spec-shape.md` § How a
  change to the document is made: a vendored delta is *a real measurement of the wrong thing*, so
  every delta below is written as **a lower bound, in those words**, of the real one.

## 1. Compiler (stage 1, stage 2)

- Stage 1: `/usr/bin/time -l clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0,
  **85,029,716 instructions retired** (`build/239/stage1.log`).
- Stage 2: `/usr/bin/time -l ./heroes build selfhost/main.hero -o heroes-next`, exit 0,
  **724,696,254,395 instructions retired**; `real 301.77` against `user 147.92` + `sys 27.90`, so
  the seconds are discarded as waiting (load 37 to 91 by `uptime`) (`build/239/stage2.log`).
- `./heroes-next --version`: `heroes 0.6.0`, **`sources 3d44313d62fb67a3`**, the brief's key.
- `./heroes-next measure spec/heroes-spec.md`: **`claude-legacy 8178`, `cl100k_base 8325`, `real
  10900`** (`claude-opus-5`, 2026-11-08, pinned), headroom **5484**, the FFI floor mortgaging 60
  (panel 030 R3). The brief's figures reproduce.

## 2. Prediction, registered before reading any other seat's report

Registered 2026-11-09, before any `239-reports/` file but this one was opened (none was).

**P1. The new refusals this sitting lands, 2363's and 2364's, whatever their codes, refuse 0
tracked programs outside the cases their repairs add.** Scored at the gate of the round that lands
them: the full net (every golden form, `corpus`, `compat` over the newest release's corpus,
`examples/`) shows no program moving from built to refused, or from exit 0 to exit 1, because of
them, its counts in the closing commit. **What it decides here**: the class is then absent from
every program this project has, and a spec sentence's §1.2 break-even (section 5 below: about 8 to 30
real tokens on every prompt, an inference from the vendored deltas, against one round trip of 500
to 2,000, so the class would have to be written in roughly 0.4% to 6% of programs, and the sentence
would have to avert it) is not approached. (These two ranges were corrected after the drafts were
priced in section 4; the prediction itself, the 0, is as registered.) **If P1 fails**, a tracked program is refused, the class is not rare, and I re-price
the sentence rather than refuse it.

Measured beside it, not predicted: `tests/thesis/constructs.txt` and
`tests/thesis/held-out/constructs.txt` (the thesis instrument's 10 and 60 tasks) list the classes
`map`, `record`, `variant`, `propagation`, `at-parameter`, `function-value` and no FFI class
(`awk` over both files, `sort | uniq -c`), and `git grep -l -E '^extern ' -- tests/thesis` finds 2
files, both spec copies used as contexts (`contexts/spec-with-one-worked-diagnostic.md`,
`contexts/spec-without-productions.md`). **So a prediction naming metric 2 for any sentence of this
sitting is scored by no task, and pays nothing under §1.6** (it would be an observation).

## 3. What the specification says today about each defect (brief item 1)

### 3a. Line 131 and § 1's `use` lines

`sed -n '28,29p;57,58p;109p;131,132p;135p;140p;164p' spec/heroes-spec.md`:

- line 131, *"Declaration order never matters but a group's (section 13); mutual recursion needs no
  forward declarations."*
- line 57, `File = { NEWLINE } { Use | Declaration } .`, and line 135 to 140, `Declaration =
  "constant" ... | Extern .`: **a `Use` is not a `Declaration` in the grammar's own vocabulary**, so
  line 131 does not, by its letter, cover the order of `use` lines. Line 109, *"Every top-level line
  starts with its kind"*, lets a reader take a `use` line for one; panel 238's spec-warden read it
  the same way (`238-reports/spec-warden.md` § 3).
- **But the document already promises the `use` order carries no meaning, by three other lines**,
  and a sitting has ruled so: lines 28 (*"A file may not bind one name twice"*), 57 (a `use` may
  stand anywhere) and 164 (*"a `use` binds its name for the whole file"*). Panel 238's R7, ratified
  2026-11-08 by the author's reading, refused D2 to D4 *"as unpaid restatements of lines 28, 57 and
  164, which already imply that the order carries no meaning"*
  (`docs/panel/238-the-use-lines-...md`, `grep -n 'R7'`, line 231). So the question the brief asks
  (is a `use` line a declaration in line 131's sense?) does not decide the matter: **the document
  promises it either way**, and `pw_first`, `dup_sorted` and `sig_sorted` break that promise today.
- **The one order line 131 lets matter has nothing to order in these programs.** *"But a
  group's"* points to § 13, whose only order is *"C reads a module's headers in the order its groups
  are written"* (line 376), inside one module. `grep -c '^extern'` over every module of the
  reproducers (my copy, `build/239/repro/`): `picker`, `extra`, `aux`, `pw`, `rs`, `pk`, `rp`, `pa`,
  `rb`, `m`, `printer`, `quiet`, `extra2` hold **one** group each, the roots `dup_sorted`,
  `pw_first`, `pk_first` **zero**. Only `prefix/rs3.hero` holds two (`cfg.h` above `rs.h`), and
  there § 13's order is exactly what fixes `Rs`'s meaning: wide, as `rs3`'s module reads it. So
  **every order these defects turn on is one the document says never matters**: the walk's today,
  the modules' key after panel 238's R2.
- **After R2 the lever moves to the modules' names**, and no line promises a module's file name
  carries no meaning; but panel 238's R3 already ruled the repair *"closes `dup` and `sig` whatever
  the order, R2's key included"* (line 187 of that file), and R2's own paragraph calls the change of
  meaning a cost R3 must close. So a name deciding a meaning is ruled a defect, not a gap.

### 3b. § 13, defect by defect

The reader's whole model of a group is three sentences: *"A group names its header, and `link` a
library when the symbols need one"* (line 375), *"C reads a module's headers in the order its groups
are written"* (376) and *"A group's `record` is the header's struct"* (404), with the sqlite fence
(389 to 394) as the one example. Re-run here with `./heroes-next` (`sources 3d44313d62fb67a3`) on
this Mac, from my copy `build/239/repro/`:

| program | today (this Mac, my run) | what a reader of the spec predicts | does the document call it correct? |
|---|---|---|---|
| `dup_written` | built, `1 10 20` | `1 10 20`: `pick` is its group's library's, `liba` | yes, and its meaning is realised |
| `dup_sorted` | built, **`2 10 20`** | `1 10 20`: nothing in the document lets another library answer `pick` | **yes**; the value is the compiler's defect against line 375 |
| `sig_written` | built, `1 30` | `1 30` | yes |
| `sig_sorted` | built, **run exit 138** (128 + 10, SIGBUS on this Mac) | `1 30`; a crash is unpredictable from any sentence | **yes**; a memory fault on a correct program (design.md §1.12) |
| `one_written`, `one_sorted` (critic) | built, **`2 20`** both | `1 20` | yes by every sentence; **no link on ld64 can realise it** (`liba`'s `pick` reached in neither order, my run) |
| `runtime_name` (critic) | built, **two empty lines** | `30` and `7`: `print`'s meaning is § 11's (*"writes its values ... and exactly one trailing newline"*) | yes; § 11 broken by a library no group binds |
| `rs_first` | `9` | `9` | yes |
| `pw_first` | **exit 1**, ``error[ffi_field_type]: `Rs.a` is `long long` in `rs.h` ... not `i32` `` | `9`: `rs.h` read as `rs`'s module reads it says `int` | **yes**; the refusal is false of the header as § 13 defines its reading |
| `pack/rp_first` | `4 1 1000` | `4 1 1000` | yes |
| `pack/pk_first` | **exit 0, `4 1 -402653184`** | `4 1 1000`: `Rp` is the header's struct as `rp`'s module reads it, padded | **yes**; a wrong value at exit 0 |
| `both/pa_first`, `rb_first` | **exit 1** both, each on the other record | each record as its own module reads it | yes by every sentence; whether any build can realise it is the compiler-engineer's question |
| `own/main` | **exit 1**, the same false `Rs.a` message | `Rs` narrow, as `rs`'s module reads it | yes |

So **a reader of the specification predicts none of the four outcomes the brief names**:
`sig_sorted`'s crash, `dup_sorted`'s `2`, `pw_first`'s refusal and `pk_first`'s value; and **the
document's own words call every one of these programs correct, each with one meaning** (the
column above). Both defects are therefore CLAUDE.md § 12's *"spec beats compiler: the compiler has
the bug"*, and a repair that makes the compiler give those meanings changes no sentence.

### 3c. The silence, searched before it is called a gap

- `grep -rln "Declaration order never matters" docs/` finds the sentence's history: panel 202's
  spec-warden (*"§ 4's 'Declaration order never matters' is falsified by `build` at exit 0"*), panel
  204's F2, adopted by its R1 (ratified 2026-10-10, `1eed23d94`), and panel 238's reading. Panel
  204's R1 rules the order of **a module's own** groups; **its R4 refused per-group units and a
  byte-sorted order, and no sitting has ruled the order of other modules' headers in a unit**
  (`grep -n 'another module\|other module' docs/panel/204-c-reads*.md` prints nothing).
- design.md §4.1 (lines 933 to 936, panel 202's R1): *"each unit reads the headers of its own module's
  groups and of the group records it spells, never another module's"*; §4.19 (line 2675): *"after
  them, the header of each record of another module's group the unit spells (§4.1)"*. **Neither
  says in what order, nor that the record must read as its owner reads it.** That is the design's
  gap 2364 lives in; the specification never mentions a unit (`grep -ci unit` prints 0).
- Panel 238's R3 (*"The document does not cover the libraries' order"*, its ffi-pragmatist) and R7
  (*"With R3's group words, no link sentence is owed: spec line 375 already gives the reader the
  whole duty"*). I read the same: line 375 states the duty and the meaning; the libraries' order is
  the compiler's to make true.

## 4. The drafts, priced on the vendored tables (brief item 2)

Each a copy of the spec with one change, every anchor asserted to match exactly once
(`build/239/drafts/make.py`), measured from the worktree's root by `./heroes-next measure
build/239/drafts/<d>.md` (`build/239/drafts/measured.txt`), which prints the two vendored rows and
*"No ceiling judges ... this is a count and not a verdict"*. **The real row is unrun** (no
`--refresh`); the figure in parentheses is **an inference** by the document's own ratio, 10900 /
8325 = 1.31, never a count, and every vendored delta is **a lower bound, in those words**, of what
the reader pays (`.claude/rules/spec-shape.md` § How a change to the document is made).

| draft | where, and the text | claude-legacy | cl100k_base | real | true under |
|---|---|---|---|---|---|
| **S0** | nothing | 0 | 0 | 0 | every mechanism |
| **N1** (238's, re-priced) | § 13 after *wider than C's.*: *A C name two linked libraries define is bound from one of them, unchecked.* | +16 | +16 | unrun (about +21) | today only; **writes the crash class into the language** |
| **N2** (238's R7, re-priced) | the same place: *A function two linked libraries define is refused.* | **+9** | **+9** | unrun (about +12) | (B), or (D) widened to shared libraries; **false under (A)+(C)**, where `dup_written` and `dup_sorted` both build and print `1`; silent on `interpose` and `runtime_name`, where one of the two definers is the C library, which no program `link`s |
| **M3b** | the same place: *A group's function is its own library's, C's without a `link`, or the program is refused.* | +23 | +23 | unrun (about +30) | every mechanism that closes the class: it is the brief's criterion written as a meaning |
| **M3c** | the same place: *Another library never answers a group's function: that program is refused.* | +14 | +14 | unrun (about +18) | every closing mechanism; *that program* is ambiguous |
| **M3d** | merged into line 375: *... and `link` a library when the symbols need one, the one that answers them or the program is refused;* | +11 | +11 | unrun (about +14) | every closing mechanism, for a group with a `link`; silent on one without (`interpose`) |
| **R1** | line 404: *A group's `record` is the header's struct as its own module reads it:* | **+6** | **+6** | unrun (about +8) | any 2364 repair that realises the owner's reading or refuses; **false today** (`pk_first`) |
| **R2** | before *A group's `constant` has no body*: *Two groups' records whose headers change each other may not both be spelled in one module.* | +18 | +18 | unrun (about +24) | only if `both` is refused; false if the compiler builds it |
| **R3** | line 404: R1 and *..., wherever it is spelled, or the program is refused:* | +17 | +17 | unrun (about +22) | as R1 |
| **B1** | after *one defining `_GNU_SOURCE` first.*: *What a group binds means what its own module reads and links, in every module, or the program is refused.* | +23 | +23 | unrun (about +30) | every mechanism that closes both classes |

**Panel 238's N2 re-prices at +9 / +9 here** (its own count was +10 / +10 on its own tree, where the
spec read 8188 / 8335, `238-reports/spec-warden.md` §§ 1b and 5c). Every draft is under `DELTA_GATE` and far from the
ceiling: the largest, +23 vendored and about +30 real, against **5484** of headroom. **No veto of
mine rests on the ceiling.**

## 5. What each draft buys (§1.2) and what pays for it (§1.6)

**§1.2's arithmetic, best case for the sentence.** A sentence costs its real tokens on every prompt
(about 8 for R1 to about 30 for M3b and B1, inferred above); one correction round trip costs 500 to
2,000 (design.md §1.2). Even if the sentence averted every occurrence, it pays only where the class
is written in at least **0.4% (R1 against 2,000) to 6% (B1 against 500)** of the programs a model
writes. Two measurements say the class is nowhere near that, and a third says why no sentence can
avert it:

1. **The instrument that scores §1.2 has no FFI task.** Section 2: the thesis tasks' construct
   classes are `map`, `record`, `variant`, `propagation`, `at-parameter`, `function-value`, 10 public
   and 60 held-out (`grep -c '^[0-9]'` over both `constructs.txt`), and no tracked `.hero` of
   `tests/thesis/` holds an `extern` (`git grep -l -E '^extern ' -- tests/thesis` names two spec
   copies only). Metric 2 cannot see any sentence of this sitting move.
2. **The tracked tree.** `git grep -o -E 'link "[^"]*"' -- '*.hero' | sort -u` grouped by file:
   **10 files** name two or more distinct libraries, by their names seven golden fixtures of refused
   library *names* (`fixedbugs-216`, `222`, `234`, `252`, `253`, and `446` twice), two compiler modules
   (`selfhost/emit/ffi_build.hero`, `selfhost/head_names.hero`) and one panel brief. Whether any
   program **across** modules links two libraries defining one bound name is P1's subject (section
   2), scored at the landing.
3. **No reader can act on such a sentence before the refusal.** What decides `dup`, `sig`, `one`,
   `interpose` and `runtime_name` is which names a C library's files define, and what decides `pw`,
   `pack`, `own`, `prefix`, `both`, `pragma` and `partial` is what one C header does to another's
   struct; neither is in the program text or in the specification, so P(avert) is about 0 and the
   sentence's value is only to explain a refusal **after** it, which design.md §4.17 puts in the
   diagnostic (*"The model fixes it in one turn, without opening anything"*).

**Payment** (design.md §1.6, *unconditional at every level*): I find **no named removal** in
§ 13 that any draft could replace; and **no registered prediction can pay**, since the instrument
that would score a rewrite-rate claim, metric 2, has no stratum for it (item 1), and a blind
reader's prediction needs a paid `claude -p` seat the author has not allowed (`00-shared.md`), so it
would be an observation. **Every draft above is unpaid today.**

**And the precedent agrees.** Every refusal of a C-world conflict the compiler already makes is
spec-silent: `grep -n -i` over the spec for *two ways*, *twice* (only line 28, about `use`),
*unavailable*, *redefine*, *together*, *duplicate*, *emit* prints nothing, so `ffi_declared_two_ways`,
`ffi_defined_twice`, `ffi_one_file`, `ffi_unavailable`, the two headers refused together (panel 202)
and the macro two headers define two ways (panel 204's R2) have no sentence, each told by its
diagnostic. Panel 204 added a sentence (G1m, line 376) only where the reader holds **a lever**, the
written order of their own groups.

## 6. The document's meaning, held against the mechanisms (the strongest reasons the proposal is wrong)

My seat judges text, but the text decides here which mechanisms need any: **a mechanism that gives
every program of § 3b the meaning the document already gives it needs no sentence; one that does
not either leaves the spec false or has to buy a sentence that pays nothing (§ 5).**

**2363.**

- **(A) alone**: the brief rules it out and I agree from the text. `dup_written` and `dup_sorted`
  are one program by the document (§ 3a); (A) builds one and refuses the other, so after R2 a
  module's file name decides whether a program the document calls correct builds.
- **(B) refuses `dup_written`**, a program the document calls correct and whose meaning the link
  realises today (my run: `1 10 20`). So (B) either refuses a correct program, which
  `.claude/rules/verification.md` § Bounded discovery names a `blocking` outcome (*"a correct
  program refused"*), or buys the right to by writing N2 into § 13 (+9 vendored, unpaid, § 5). **I
  object to (B) as the closing mechanism on that ground**; as a probe whose answer feeds (C) it costs
  the text nothing.
- **(C), or any route that makes the group's library answer in every order**: the document already
  says that meaning (line 375 and its fence), so **0 tokens**; it refuses only where no link can
  realise it, `one_*` on ld64 (my run: `2 20` in both orders, `liba`'s `pick` reached by neither),
  a conflict between two libraries the author chose, of the kind the compiler already refuses with
  no sentence (§ 5's precedent).
- **`runtime_name` and the allocator.** `print`'s meaning is § 11's, and no sentence lets a library
  change a built-in, so a library replacing `printf` breaks the document (my run: two empty lines).
  A library replacing `malloc` on purpose is spoken of nowhere; **a word to allow it would be a new
  form**, owing Principle 0's ticket: the compiler does not need it (`git grep -h -oE 'link
  "[^"]*"' -- '*.hero' | grep -i -c -E 'malloc|jemalloc|tcmalloc|mimalloc'` prints **0**), and no
  thesis effect can be measured (§ 5, item 1). **So it waits**, and if the robust reading refuses
  both, the refusal is told in the decision issue and the release's notes, never in a sentence
  (CLAUDE.md § 2: *a change that refuses a `0.x` program is told*).

**2364.**

- **(A), panel 238's direction as worded** (*"one clang probe per order tried"*, *"a true refusal
  kept where no order works"*): **the order `pk_first` is read in today compiles and passes every
  field assertion** (my run: exit 0, `4 1 -402653184`), so a search asked where a unit fails never
  runs there; and a search run everywhere, taking the first order that compiles, may take either
  reading of `Rp`, since both orders compile (`rp_first` and `pk_first`, my runs), so which it takes
  is the search's order, not the document's meaning. The
  document fixes `Rp`, `Rs` and `Rx` as their **owners** read them (line 404 with line 376;
  `prefix/rs3.hero` shows the owner's own order is part of that meaning), one reading, not any of
  several that compile. **(A) alone leaves the class's worst member, a wrong value at exit 0, and
  has the compiler choose between meanings the document already chose between. I object to it as
  the closing mechanism.** It also cannot reach `own` as worded, since there the header that
  configures `Rs` is the spelling module's own, which §4.19 (line 2674) reads first, before any
  other module's: a reading of design.md, not a run.
- **(B) as the judge**, each unit's layout of each group record it spells equal to its owner's
  unit's (size, alignment, every member's offset and type): the only listed route that sees `pack`,
  `pragma` and `partial`, and the one that states line 404's *"is the header's struct"* as § 13 reads
  a header. **(C)** is what `prefix` needs, since line 376 makes `rs3`'s order part of `Rs`'s
  meaning; **(D)** is one way to make (B) hold by construction.
- **What the document asks for, then**: every unit reads each group record as its owner's unit
  does, by (C) and (D) or any route that makes it so, with (B) the check that it is so, refused only
  where no unit can. **0 tokens.** Whether `both` can be built is the compiler-engineer's to measure;
  § 11's condition 3 says what changes if it cannot.

## 7. design.md: the sentences each repair owes the design (brief item 3)

design.md cites the specification by section (`.claude/rules/spec-shape.md` § How to cite the
specification: *"`spec § 7`, never `spec:NNN`"*), so the drafts do. The bracketed parts are the
synthesis's to fill from the mechanism it adopts.

**(a) §4.19, a new paragraph after line 2721** (the end of *A module's headers are included in the
order its groups are written*), before *A `constant` is the group's second kind of member*:

> **What a group binds means, in every module, what its own module reads and links** (panel 239,
> provisional, author ratification pending; defects 2363 and 2364). A function a group binds is
> called from the library its `link` or `package` names, or from the C library where it names
> neither, whatever order the modules are walked in and whatever they are named: [mechanism]. Where
> no link can make it so, as where another library the program links defines the name in a file the
> link needs for another name (`one_*` on ld64), the build is refused on the binding, both libraries
> and the modules linking them named [code]. A record a group binds is laid out, in every unit that
> spells it, as its own module's unit lays it out, that module's earlier headers read before the
> record's in their written order: its size, its alignment, and each member's offset and type
> [mechanism and check]. Where no unit can, the build is refused naming both modules and what each
> reads before the record's header [code], and never as `ffi_field_type` against a header read
> alone, since there the field is right. **The specification gains no sentence** (the sitting's
> spec-warden): spec § 13's *`link` a library when the symbols need one* and *A group's `record` is
> the header's struct* already give both meanings, and what is refused is what no order and no name
> can make mean them, as with the other refusals this section names.

**(b) §4.19 line 2674 to 2675**, *"after them, the header of each record of another module's group
the unit spells (§4.1)"*, becomes *"after them, the headers of each record of another module's
group the unit spells, read as that module reads them (§4.1)"*, worded to the mechanism.

**(c) §4.1 line 935 to 936**, *"each unit reads the headers of its own module's groups and of the
group records it spells, never another module's"*, gains *"each record laid out as its own module's
unit lays it out (§4.19)"*. Until it does, §4.1 is silent on exactly what 2364 got wrong.

**(d) §4.17, after *"The model fixes it in one turn, without opening anything."* (lines 2352 to 2353)**,
the words (section 9 below says why):

> **At the C boundary, in the specification's words** (panel 239). A refusal that a group's library
> or another module's header caused names what the specification names, a group, its header, its
> `link` library, a module, a function, a record and its field, and says a module *reads* a header,
> as spec § 13 does. It leans on no word the specification does not use (*archive*, *link line*,
> *linker*, *unit*, each `grep -ci` 0 over it on 2026-11-09), nor on *member* in the linker's sense,
> which the specification spends on a struct's member and a group's. Its note cites the
> specification by its own section, *spec § 13*, as `cli/argv.hero`'s notes cite *spec § 3 Types*:
> a model that holds the specification alone reads *§4.19* as its section 4.

## 8. `.claude/rules/c-boundary.md` § *A clang failure that the author's own extern caused* (brief item 3)

Read today: the list's preamble says *"it has twelve members"*, and the bullets count twelve, the
eighth and ninth (`ffi_declared_two_ways`, `ffi_defined_twice`) already refusals made *"before the
link"* rather than clang failures; and the twelfth reuses an existing code, `ffi_missing_link`, the
fourth member's (`selfhost/emit/ffi_build.hero:46`, in the tree since `03633d86b`, 2026-08-17), as
its own member. So the list counts **mistakes told**, not codes.

- **2363's refusal is a thirteenth member**, whatever its code. No member names a library's
  definition answering a group's function: the fourth is *a symbol the linker cannot find because the
  group named no `link`*, the twelfth a label the linker cannot find. It moves both lines the preamble's history names: **the line between
  accepted and refused** (`sig_sorted` built and died at 138, `dup_sorted` built and printed `2`, on
  robustness's rank, as defects 2071 and 2072 moved it) and **who is blamed** (`one_written` on GNU
  ld, `multiple definition of 'pick'` told `internal error: linking failed` at exit 2, the brief's
  critic run, not re-run here). Draft:

  > - **a C name a group binds that a library other than its group's would answer**: added
  >   2026-11-NN (panel 239, defect 2363). Told `<code>` on the binding, its group's library and the
  >   other one named with the modules that link them, where the link took the other definition at
  >   exit 0 (`sig_sorted` built and died by a memory fault) and where the link refused the two
  >   (`multiple definition`, until then `internal error: linking failed` at exit 2); a group naming
  >   no `link` names the C library (`interpose`). [The mechanism, and what it costs a build.]

  A widened `ffi_defined_twice` reads true to a reader (*`pick` is defined twice*), so the code is
  admissible either way; the count is thirteen either way.
- **The narrowing paragraph** (*"Every class recovers a name and asks whether this program declared
  it `extern`, so a symbol nobody declared stays exit 2"*) holds for every bound name. **If the
  sitting reaches `runtime_name`** (a library replacing `printf`, which no group binds), the class
  reaches a name nobody declared, recovered through the `link` the program wrote, and the paragraph
  must say so: *"... declared it `extern`, or linked the library that defines it"*. Otherwise it
  would read as ruling `runtime_name` the compiler's fault at exit 2, where today it is silent at
  exit 0.
- **2364's refusal is a member only where a true refusal is kept.** If the repair lays every group
  record out at its owner's layout by construction, a disagreement (B) then finds is the compiler's
  own failure, exit 2, and **no member**; if a refusal is kept where no unit can (`both`, should it
  prove unbuildable), it is a fourteenth member or a widening of the eighth, *one C name two
  modules' headers declare two ways*, since a record laid out two ways in two units is that mistake
  for a type (C11 6.2.7's compatible types across translation units, the historian's to source).
- **One observation for the coordinator, not this sitting's question**: the file's `paths:` are
  `runtime/**`, `examples/**`, `selfhost/emit/ffi*` and `selfhost/parse/group.hero` (`sed -n 1,8p`),
  and the members' code it names lives in `selfhost/cli/` (`two_ways.hero`, `defined_twice.hero`,
  `one_file.hero`, `renamed.hero`), where 2363's link reading will likely live too; so the rule
  does not load when that code is written.

## 9. The words of both refusals, as a reader of the specification meets them (brief item 4)

The reader's vocabulary, counted (`grep -n -i -w` over `spec/heroes-spec.md`): **present**: group
(13 lines), header, `link`, *library* (lines 375, 479) and *C libraries* (374), module (lines 22,
26, 29, 31, 376), function, record, field, *size* (100, 410), *reads* (348, 376, 413, 414, 416,
437, 443), bytes, *symbols* (375, once). **Absent**: *archive*, *link line*, *translation unit*,
*unit*, *linker* (each 0 by `grep -ci`), *offset*, *align*, *padding*, *laid*, *definition* (0
by `grep -n -i -w`). **Taken**: *member* is a struct's or union's member (lines 414, 416) and the
group's `Member` production (483, 484), so a linker's *archive member* would read as one of those.

- **2363.** Every fact the refusal needs is sayable in those words: *`pick` would be called from
  library `b`, which module `extra` links, not from `a`, the library its group names*; *`b` defines
  `pick` too*; for a group with no `link`, *not from the C library*. What it must not say: *archive*,
  *member* (say *a file of library `b`* if it must), *link line*, *linker*. And **no fix may point at
  the order of `use` lines or at a module's name**: after R2 neither is a lever, and panel 238's R3
  rules a name deciding the meaning a defect. Cost of a word outside the document: the model must
  guess its meaning or open something, which §4.17 says it cannot.
- **2364.** Today's message is false in three places, measured here: ``error[ffi_field_type]:
  `Rs.a` is `long long` in `rs.h` `` is false of `rs.h` as § 13 defines reading it (my `pw_first`
  run); its note cites *"(§4.19)"*, design.md's numbering, which a reader of the specification reads
  as its section 4 (**49** lines of `selfhost/` end a message sentence that way, `git grep -n -E
  '"[^"]*\(§4\.[0-9]+\)[^"]*"' -- selfhost | wc -l`, test assertions among them; `cli/argv.hero`
  cites *spec § 3 Types* instead); and its `guess` walks in a circle (my `fixguess` run: with `a:
  i64` both orders are refused *"`Rs.a` is `int` in `rs.h` ... declare `a` with `i32`"*). **Another
  code is owed**: the mistake is not the field, which is right where its module reads it, and in
  `pack`, `pragma` and `partial` no field's type differs at all, so `ffi_field_type` cannot even
  name them. True words exist in the reader's vocabulary: *`Rs.a` is `long long` where module
  `main` reads `rs.h`, after `pw.h` defines `TWO_OTHERS_WIDE`, and `int` where module `rs` reads
  it*; for `pack`, *`Rp.b` is at byte 1 where module `main` reads `rp.h`, after `pk.h`, and at byte
  4 where module `rp` reads it*; for `partial`, the record's *size* in each. Brief question 5's
  *"in `rb.h`"* is made true only by naming the module that reads it, which is that other message.

## 10. What 0 tokens commits the sitting to

- **The repairs, not the document, carry the meaning.** 0 tokens is right only if every program of
  § 3b that the link and the headers can realise builds with the document's meaning, in every order
  and under every module name, and only what no order and no name can realise is refused, told in the
  reader's words (§ 9).
- **The telling goes where CLAUDE.md § 2 puts it**: a program that ran and is now refused (`one_*` on
  ld64; `runtime_name` and an allocator if the robust reading refuses them) is told in the decision
  issue and the release's notes. With the specification unmoved, `.claude/rules/records.md` §
  Release tags makes that release a `Z`, as panel 238's spec-warden read for its own landing: the
  document never specified these behaviours, so no sentence of it changes.
- **The design carries the rule** (§ 7's four drafts), so the next sitting does not find §4.1 and
  §4.19 silent on the order of other modules' headers, as this one did (§ 3c).

## 11. Verdict

*Moved by § 12e after the critic's second pass (2026-11-09); this section stands as written at
its time.*

- **verdict: object**, to the two directions as panel 238 worded them, on the document's own
  words: for 2363, (A) alone (the brief's criterion, agreed) and (B) as the closing mechanism, which
  refuses `dup_written`, a program the document calls correct and the link realises; for 2364, (A)
  alone, which leaves `pk_first` at exit 0 printing `4 1 -402653184` and has the compiler choose a
  meaning the document already fixed. **Approve 0 spec tokens** for both repairs made by a mechanism
  that realises the document's meaning (§ 6). **Veto N1** (it writes a crash class into the
  language, design.md §1.12, CLAUDE.md § Precedence rank 3) **and any sentence false at the commit
  it lands in** (N2 under (A)+(C); R1 before 2364's repair). **Not provisional**: the vendored
  counts are measured, the real ones unrun, and no verdict here rests on a real count or on the
  ceiling.
- **section**: design.md §1.6 (the payment rule) and §1.2 (real cost); CLAUDE.md § 12 (*spec beats
  compiler*) and § 2 (Principle 0); design.md §4.19 and §4.1 for the design text, §4.17 for the
  words.
- **spec_token_delta**: recommended **0 / 0** (`claude-legacy 8178` to 8178, `cl100k_base 8325` to
  8325, `real 10900` pinned and unchanged). The drafts, measured: **+6 to +23 vendored**, N2 **+9 /
  +9**, R1 +6 / +6, M3b and B1 +23 / +23 (§ 4); every real delta unrun, about +8 to +30 by the ratio
  1.31 (an inference).
- **removal**: nothing, and nothing is owed, since nothing is added. Had a sentence been adopted:
  nothing, and that would be a problem (§ 5: no removal in § 13, no prediction that pays).
- **needed_for_self_hosting**: **no**. Stage 2 built at exit 0 today with both defects open
  (§ 1), and no sentence is needed for the compiler to compile itself.
- **argument** (at most 120 words): The specification already gives every program of both
  defects one meaning: a group's function is its library's (line 375), a group's record is its
  header's struct as its module reads it (lines 376, 404), and neither `use` order nor a module's
  name carries meaning (lines 28, 57, 164; panel 238's R7, R3). Both defects are the compiler's
  against the document; a repair realising that meaning changes no sentence. A sentence could not
  pay: no reader sees from a program which C library defines what, the thesis instrument has no FFI
  task, no removal exists, and every existing C-world refusal is spec-silent. Mechanisms refusing
  realisable programs ((B)) or leaving wrong values ((A) alone) would need text the document cannot
  afford.
- **prediction**: **P1** (§ 2, registered before any other report was read): the new refusals of
  2363 and 2364 refuse **0** tracked programs outside their own cases, scored by the full net of the
  round that lands them (every golden form, `corpus`, `compat`, `examples/`), its counts in the
  closing commit.
- **condition** that would change it:
  1. **a run showing a sentence averts the class**: a blind reader (a paid `claude -p` seat, the
     author's to allow, its size the coordinator's to set) given N2 or M3b who avoids the
     refused shape on a task linking two libraries that define one name, or a thesis task with an
     FFI stratum whose rewrite rate the sentence moves; then a sentence can pay by prediction;
  2. **P1 failing**: a tracked program refused means the class is not rare, and I re-price;
  3. **the compiler-engineer measuring `both`, or another program of § 3b beside `one_*`,
     unrealisable by any unit while a C program in two files realises it**: then the compiler refuses
     a correct program for its own reason, and R2 (+18 vendored) is owed for the document to be true;
     I would approve it on truth (the author's instruction of 2026-09-28: *the most robust route, at
     the spec's own tokens' cost too*), paid by a registered prediction naming an instrument that
     exists (the `both` case's golden, refused with the new code at the landing);
  4. **the synthesis adopting (B) for 2363**: the document must then call `dup_written` refused,
     N2 (+9 / +9) becomes (B)'s price, true under it, and I would object to it as unpaid rather than
     veto it.

## 12. After the critic's second pass

Asked by the coordinator after the critic's second pass (`239-reports/critic-reports.md`, findings C1,
V1, I3, W1, W2 and item 7 of its closing list), with leave to read the other seats' reports (my P1
being registered in § 2). Read: `critic-reports.md`, `ffi-pragmatist.md`, `compiler-engineer.md` in
`.claude/worktrees/p239/docs/panel/239-reports/`. The ffi-pragmatist's report has no § 14 yet (its
headings end at § 13 and *Verdict*, `grep -n '^#'`), so its widening is priced in both readings.
Everything above this section stands as written at its time; where it is corrected, the correction
is here.

### 12a. C1, re-run, and what it does to my reading of `dup_written`

**Re-run with my compiler** (the critic's `p2/self/` copied to `build/239/p2/self/`, `sh
p2/self/run.sh ./heroes-next sw`): C `-la -lbs` prints `pick=1 b_only=10`, `-lbs -la` prints `pick=2
b_only=20`; **`self_written` built `1 10`, `self_sorted` built `2 20`**. Never `1 20`.

**What a symbol table says** (`nm`, this Mac): `libbs.a`'s `b_self.o` holds `U _pick`, its `b_only`
calling its own library's `pick`; the brief's `libb.a`'s `b_only.o` holds no `_pick` at all. So the
two programs differ only inside library `b`, and the compiler starts no symbol reader today
(`00-shared.md`: no `nm`, `otool`, `objdump`, `readelf`).

**Reading.** The document gives `self_*` one meaning, `1 20` (`pick` is `a`'s by its group, line 375;
`b_only` is library `bs`'s function as `bs` built it), and **no link realises it**: one name, one
definition in the program. So refusing both orders of `self_*` is the only robust verdict, and it is a
conflict between two libraries the author chose. And **`dup_written`'s meaning is realised today
only because `libb`'s `b_only.o` happens to hold no reference to `pick`**: a premise about a
library's insides, visible only to a reader the compiler does not have, and falsified in silence by
the next build of `libb`. Accepting `dup_written` while refusing `self_written` would rest on the
world, not on the program (CLAUDE.md § 11: *a narrowing asks the value, never the world*;
`.claude/rules/module-shape.md`).

**So I withdraw my objection to refusing both orders of `dup_*`** (§ 6, *"(B) refuses
`dup_written` ... I object to (B) as the closing mechanism on that ground"*, and § 11's first bullet
on the same ground). The ground was *"the link realises it"*, and C1 shows that is true of `libb`'s
present contents, not of anything the program or the document states.

**Does the refusal of both orders refuse a program the document calls correct?** By the letter,
yes: nothing in the specification refuses `dup_written`, and line 375 gives it a meaning. **But so
does every refusal of a C-world conflict already landed**: `ffi_declared_two_ways`,
`ffi_defined_twice` and panel 204's macro two headers define two ways each refuse programs no sentence
refuses, and the last refused `cfgone`, which ran and printed a value (`3 50` against `3 10`, panel
202's spec-warden). The spec stays silent on all of them (§ 5, re-run there). So *"a program the
document calls correct"*, in the sense *"nothing refuses it"*, is not what makes a sentence owed:
the specification promises nothing about two libraries defining one name, and its line 375 stays
true of every program the refusal leaves standing. **A sentence is owed only if it pays (§1.6) or if
the specification would be false without it; neither holds.**

**My condition 4 (§ 11) was wrong in one word, corrected here**: it said that under a refusal of
`dup_written` *"the document must then call `dup_written` refused"*. The precedent above says it
need not; the sentence stays a priced choice, and an unpaid one.

### 12b. The sentences true of the refusal, re-priced

Same method as § 4 (`build/239/drafts/make2.py`, each anchor asserted once; `./heroes-next measure`,
`build/239/drafts/measured2.txt`); each sentence placed after *wider than C's.* in § 13; the real
row unrun, the figure in parentheses an inference by the ratio 1.31.

**V1, re-run here** (the critic's `p2/self/run2.sh`, copied, with a copy of v5's binary,
`build/239/bin/ffi-v5`, `--version` `sources 0143800228018d3a`): C `-lab -lbs` and `-lbs -lab` both
`pick=1 a_only=10 b_only=10`; **v5 `ab_written` and `ab_sorted` built, `1 10 10` both, no word**;
my base compiler the same. `b_only` is `20` by its own library. So the column below rests on a run of
mine, not on the critic's alone.

| draft | text | legacy | cl100k | real | true of v5 as built (this Mac) | true of v5 joined to a reader of every definer, (B) or (D) |
|---|---|---|---|---|---|---|
| **N2** | *A function two linked libraries define is refused.* | +9 | +9 | unrun (about +12) | **no**: `ab_*` accepted in both orders, `libab` and `libbs` both defining `pick` (critic V1, re-run here); and, on Linux, glibc's `ldexp` under `link "m"`, defined by libc and libm, is accepted, if the C library counts as linked | yes for bound names, if *linked* is read as *named by the program* |
| **FP1**, the ffi-pragmatist's own (its § 12) | *A C name a group binds is taken from its group's library, or from no library the program names where the group names none; a library of the program that defines it again is refused, whatever order the program's modules stand in.* | **+48** | **+48** | unrun (about +63) | **no**: `libbs` defines `pick` again and `ab_*` builds (V1) | yes |
| **V1s** | *A function a group binds is refused where a library the program names, other than its group's, defines it too.* | +24 | +24 | unrun (about +31) | **no**, by V1 | yes |
| **V1t** | *A function a group binds is refused where, in either order of the program's libraries, a call would reach another library's than its group's.* | +30 | +30 | unrun (about +39) | **yes**, and that is its fault: it writes `ab_*`'s silent `1 10 10` (for `1 10 20`) into the language as allowed | yes, but then narrower than the rule |
| **W2a**, the widening: bound or not | *A C name two libraries the program names both define is refused, whether a group binds it or not.* | +21 | +21 | unrun (about +27) | **no**: `ab_*` (V1) and `intern_*` (I3, unbound, silent) | only with unbound names judged |
| **W2b**, the widening with the C library's names | *A C name that a library the program names defines, and another library or the C library defines too, is refused unless a group binds it from that library.* | +32 | +32 | unrun (about +42) | **no**: `ab_*`, `intern_*`, `runtime_name` (critic C4, ffi-pragmatist § 8: not judged) | only with the runtime's names judged; it then refuses `alloc_main` and accepts `alloc_bound`, as the critic's item 6 says the unbuilt rule would |

What each verdict of mine covers:

- **Veto**: **V1t** and **N1** (§ 4), each writing a wrong-value class into the language (design.md
  §1.12; CLAUDE.md § Precedence rank 3); and **N2, FP1, V1s, W2a and W2b if landed beside v5
  alone**, since each is false at the commit it lands in on this Mac (V1, and for W2a and W2b also
  I3 and C4).
- **Object** (unpaid, §1.6): **N2, FP1 and V1s** if v5 lands joined to a reader of every definer
  (route B) or the whole-archive probe (route D), where they become true; **W2a and W2b** if the
  widening lands with unbound names and the runtime's names judged. **FP1, at +48 vendored, is the
  dearest draft of this sitting and 2 under `DELTA_GATE`** (50, `tests/harness/suite_spec.hero:214`); the ffi-pragmatist calls it owed, and I
  read nothing in the document that owes it and nothing that pays for it: no removal in § 13, no
  thesis task with an FFI construct (§ 2), and a reader who cannot see which library defines what
  cannot act on it before the refusal (§ 5, item 3).
- **Approve**: **0 tokens**. Refusing both orders joins the family § 5 counted, refusals of a
  C-world conflict told by their diagnostic and written in no sentence; its C ground is the one
  `ffi_defined_twice` already stands on (C11 6.9p5, one external definition, cited by this sitting's
  historian, its lines 38 and 489, and by the critic's V1).

### 12c. The words, read against § 9's vocabulary (the critic's W1 and W2)

**v5's `ffi_defined_elsewhere`**, read from the critic's copies of its outputs
(`239-critic/build/239/p2/ffi/hero-runs/v5-Darwin/*.err`):

- the machinery's words in every message: *the linker found a definition in*, *the link line*,
  *over an archive*, *`-lw` is linked*;
- **a false note on ld64.** Every message ends *"a linked program holds one definition of `pick`,
  taken from the first library on the link line that defines it"*. `one_written` is linked `-la
  -lb2` (my rebuild of it, `build/<key>/link-words.rsp`) and prints `2`, `libb2`'s, though `liba`
  stands first on the line and defines `pick`; v5's own headline for it says *"reaches
  `libb2.a(b2.o)` in either order"*. A false sentence in a message is `blocking`
  (`.claude/rules/verification.md` § Bounded discovery, *"a false message"*);
- *"the order of that line is not the program's to write"* is true after R2 only; today the walk
  follows the `use` lines;
- the file inside a library (`liba.a(pick_a.o)`) is a name, and may stay one: `pick_a.o` and the
  library's path, never called a *member*.

**The compiler-engineer's `ffi_record_two_layouts`**, read in its report and its
`out-agree-mac/s2364_*.err`:

- *unit* in nearly every sentence (*"`rb.hero`'s unit"*), which the specification never uses;
- every member of both layouts listed (W1: 8,113 bytes, 186 pairs, on libjpeg's struct);
- **told at the owner's declaration** (`rs.hero:2:12`, `rb.hero:2:12`), the one line in the program
  that is right; a model edits where the message points, and today's `fixguess` circle (§ 9) is the
  measured cost of pointing at a correct field;
- in `both`, `Rb` from `pa_first` and `Pa` from `rb_first`: the walk picks the record;
- its last note's second half (*"or have `rb.hero` bind first the header that configures it"*) is
  false for `both` (the compiler-engineer's own reading, its § 6).

**The rule I would land for the words** (it is § 7(d)'s, made exact): a refusal names a group, its
header, its `link` library, a module, a function, a record, a field, a size in bytes, and what a
header defines; **the header's own C words are allowed** (`long long`, `#pragma pack`, a macro's name,
*aligned* where alignment is the only difference), since spec § 13 sends the reader to the header;
**the machinery's are not** (*unit*, *archive*, *member*, *link line*, *linker*, *object*); a file in
a library is named by its file name and the library's path; the message is told **where the author
can change something**, and names **every** record or library in a fixed order (by name), never the
walk's; it lists the first fields that differ and the sizes, not every field; and every sentence is
true in every order and on both platforms.

**The reference words**, each sentence true of the program as measured (sources and outputs: my
copies under `build/239/repro/`; sizes from the compiler-engineer's runs; the notes in each the same
in every order of the modules). `<path>` is the library's own path as the build found it.

`sig_sorted` and `sig_written`, one message:

```
error[ffi_defined_elsewhere]: `pick`, which this group binds from `a.h` with `link "a"`, is defined by library `w` too, which module `aux` links with `link "w"`, so a call of `pick` may reach `w`'s instead of `a`'s
  at picker.hero:2:5
  note: library `a` defines `pick` in its file `pick_a.o` (`<path>/liba.a`), and library `w` in its file `pick_w.o` (`<path>/libw.a`)
  note: the program's calls of `pick` reach only one of the two, and which one depends on how the two libraries are linked, so the program is refused in every order of its modules
  note: link one library that defines `pick`; or, where library `w` is built, keep its `pick` inside it (`nmedit -R` on macOS, `objcopy --localize-symbol=pick` on Linux, `-fvisibility=hidden` for a shared library)
```

`interpose`:

```
error[ffi_defined_elsewhere]: `puts`, which this group binds from `stdio.h` with no `link`, so from the C library, is defined by library `q` too, which module `quiet` links with `link "q"`, and the program's calls of `puts` reach `q`'s
  at printer.hero:2:5
  note: library `q` defines `puts` in its file `puts_q.o` (`<path>/libq.a`)
  note: link no library that defines a name the C library defines; or, where library `q` is built, keep its `puts` inside it (`nmedit -R` on macOS, `objcopy --localize-symbol=puts` on Linux, `-fvisibility=hidden` for a shared library)
```

`own` (told where `m` spells `rs.Rs`, `m.hero:8:19`, `awk` on the file):

```
error[ffi_record_two_layouts]: module `m` reads `rs.h` after its own `m.h`, which defines `TWO_OTHERS_WIDE`, so where `m` spells `rs.Rs` its field `a` is `long long` and `Rs` is 16 bytes; where module `rs` reads `rs.h` alone, `a` is `int` and `Rs` is 8 bytes
  at m.hero:8:19
  note: `Rs` is bound at `rs.hero:2:12` with `a: i32`, which is right where module `rs` reads `rs.h`: the binding is not what to change
  note: a value of `Rs` passes between `m` and `rs`, so both must read one struct, and `m` reads its own headers before any other module's, so no order puts `rs.h` before `m.h` in `m`
  note: spell `Rs` only in modules none of whose headers defines `TWO_OTHERS_WIDE`; or, if `Rs` is meant wide, have `rs.hero` name a header defining it above `rs.h` and declare `a` with `i64`, so every module reads one struct
```

`both` (one message for both written orders, both records named by name; told on the first line of
`pa_first` that spells either record, `pa_first.hero:7:11` reading `p.w`, the anchor the landing's):

```
error[ffi_record_two_layouts]: module `pa_first` spells both `pa.Pa` and `rb.Rb`, and no order of their headers reads both as their own modules do: `pa.h` defines `PA_WIDE`, which makes `Rb.a` `long long` in `rb.h` read after it, and `rb.h` defines `RB_WIDE`, which makes `Pa.w` `long long` in `pa.h` read after it
  at pa_first.hero:7:11
  note: module `pa` reads `pa.h` alone, where `Pa.w` is `int` and `Pa` is 4 bytes; module `rb` reads `rb.h` alone, where `Rb.a` is `int` and `Rb` is 4 bytes; each read after the other's header is 8 bytes
  note: a value of each passes between its own module and `pa_first`, so each must read one struct in both
  note: spell `Pa` and `Rb` in two different modules, so that neither reads the other's header
```

Checked against the sources (`cat` of `both/pa.h`, `rb.h`, `own/m.h`, `rs.h` and the `.hero` files):
every macro, field type and line named is theirs; the sizes are the compiler-engineer's runs
(`own`: 8 and 16, `both`: 4 and 8). Two clauses rest on the route, not on a run of mine: *"`m` reads
its own headers before any other module's"* (`emit/externs.hero:124-128` and the compiler-engineer's
composer, its § 13) and *"the program is refused in every order of its modules"* (v5's byte-identical
messages, the ffi-pragmatist's § 4).

### 12d. design.md §4.17 and the 49 citations

**Counted** (`git grep` over `selfhost/`, comment lines left out): **49** lines hold a string
literal ending a sentence `(§N.N)`, in **25** files, 5 of them assertions of a test (`git grep -n
-E '"[^"]*\(§[0-9]+\.[0-9]+\)[^"]*"' -- selfhost | wc -l`, the same piped through `grep -c -E
':\s*assert '`); **97** lines hold a literal with a design section anywhere in it, a leading
*"§4.19: ..."* included (`git grep -n -E '"[^"]*§[0-9]+\.[0-9]+[^"]*"' -- selfhost | grep -v -E
':[0-9]+:\s*#' | wc -l`); **7** cite the specification as `spec § N` (the same with `spec § ?[0-9]+`).

**Searched for a ruling before calling it a gap**: design.md §4.17 (`grep -n -i
'cite\|citation\|(§'` over lines 2320 to 2493: two hits, neither about a message's citation),
`docs/panel/` (`grep -rn` for a message or note citing design.md: one, panel 112, a *false* Part 8
citation), and the issues (`gh issue list --state all --search`, two searches, both `[]`). **Unruled:
a question, not a ruling either way.**

**Recommendation**:

- **§4.17's sentence is a rule for every diagnostic, not for this sitting's two codes**: what a model
  holds is the specification, whatever the code. So § 7(d)'s draft stands as written, and the two
  refusals of this round are born under it (`spec § 13`, not `§4.19`).
- **The 49 (97 by the wider count) are not this round's repair.** Their cause is not 2363's or
  2364's, so `.claude/rules/verification.md` § Bounded discovery files them apart, **`adjacent`**:
  *"a true message less exact than it could be"*. Each note's content stands alone, and the citation
  names a section the specification does not have. Its body carries both counting commands above,
  and the repair is mechanical: a citation the specification can answer becomes `spec § N`, and one
  it cannot (most are design rationale) is dropped from the message, its home staying the code's
  comment.
- **§4.17's sentence then carries one dated clause**, so the design does not state a rule 49 lines
  break in silence: *"(on 2026-11-09, 49 notes still cited this document's sections, defect
  <n>)"*, removed by that defect's repair.

### 12e. The verdict, after the second pass

*(Moves § 11's verdict as follows; § 11 stands above as written at its time.)*

- **verdict: object**, narrowed. **To 2364's (A) alone** as panel 238 worded it (unchanged; the
  compiler-engineer and the ffi-pragmatist now agree on (A) joined to (B), critic C2). **To any 2363
  sentence landed beside v5 alone**, each false of v5 on ld64 by V1 (vetoed, below); and **to v5's
  words as built**, whose last note is false on ld64 (§ 12c), a `blocking` message. **Withdrawn**:
  my objection to refusing both orders of `dup_*` (§ 12a). **Approve 0 spec tokens**, the refusal
  joining the spec-silent family (§ 12b). **Veto** N1 and V1t, and N2, FP1, V1s, W2a, W2b beside v5
  alone. Not provisional: every count vendored and measured, every real unrun, and no verdict rests
  on a real count or on the ceiling.
- **section**: design.md §1.6 and §1.2; CLAUDE.md § 12 and § 2; CLAUDE.md § 11 (*a narrowing asks
  the value, never the world*) for 12a; design.md §4.17 for the words; §4.19 and §4.1 for the design
  text (§ 7).
- **spec_token_delta**: recommended **0 / 0** (8178 / 8325, real 10900 pinned). Second-pass drafts,
  measured: N2 +9, W2a +21, V1s +24, V1t +30, W2b +32, **FP1 +48**, each the same on both vendored
  tables; every real delta unrun.
- **removal**: nothing, and nothing is owed.
- **needed_for_self_hosting**: no.
- **argument** (at most 120 words): C1 holds: `self_*` prints `1 10` or `2 20` under my compiler,
  never `1 20`, and `dup_written` reaches its meaning only because `libb`'s `b_only.o` holds no
  `U _pick`, a fact about a library's insides that nothing the compiler reads can see. So refusing
  both orders is robust, and my objection to it is withdrawn. It refuses programs no sentence
  refuses, as `ffi_defined_twice`, `ffi_declared_two_ways` and the macro refusal already do, all
  spec-silent. No sentence pays: none has a removal, the thesis instrument has no FFI task, and a
  reader cannot see definers before the refusal. Every sentence that closes the class is false of
  v5 alone on ld64 (V1), so beside v5 alone each is vetoed.
- **prediction**: **P1**, unchanged (§ 2). The seats' tracked runs support it, 25 roots unmoved under
  v5 and 21 under the compiler-engineer's route, both reproduced by the critic (T1, T2); it is still
  scored only at the landing round's full net.
- **condition** that would change it: § 11's conditions 1 to 3 stand; **condition 4 is replaced**:
  if the synthesis adds a sentence on the ground that refusing `dup_written` refuses a program the
  document calls correct, I object to it as unpaid (12a, 12b), and veto it if v5 lands alone. And
  **the words**: if the landing's messages keep a sentence false on either platform (v5's *"taken
  from the first library on the link line"*) or tell `own` and `both` at the owner's correct
  declaration, I object to the words as a `blocking` message and a pointer at a correct line.
