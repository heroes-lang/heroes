# Panel 252, the compiler-engineer's report

Written as I go, from 16:22 CET 2026-11-19 (`uptime`). Every command ran in my folder,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-compiler-engineer/` (called `ce/`), a
`git archive` of `181555e9a32d0b18878f9f10e48d2eadd939c285`, `TMPDIR=ce/tmp`. No clock is a measurement here
(five agents share the Mac); costs are `/usr/bin/time -l`'s instructions retired and bytes.

## Work log

- **16:22** the copy: `find ce -type f -not -path 'ce/tmp/*' | wc -l` **20,037**, the brief's. Load 5.44.
- **16:22** seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0.
- **16:23** stage 2: `./heroes build selfhost/main.hero -o ce-frozen`, exit 0, 941,430,666,034 instructions
  retired (`/usr/bin/time -l`, `ce/stage2.log`); `./ce-frozen --version`: `heroes 0.6.0`, **`sources
  23c7d2a626b29fd5`**, the writer's. I am on the frozen commit.
- Every compiler I report from, its key by `--version`: `ce-frozen` `23c7d2a626b29fd5`; `ce-q1r`
  `2ea57f890c143853` (Q1-R alone, 693,420,945,785 instructions retired to build, `ce/routes/q1/build.log`).

## Q1, a private module that cannot use its own face

**Built: Q1-R as a removal, not a probe.** `ce/routes/q1/q1r.diff`: the call at `selfhost/modules.hero:151-152`,
the records `Edge` and `Walked`, `cycles` and `walk` (`:269-380`) deleted, the test's ring at `:420-432` now
asserting no error, and `use_key.hero:4`'s comment naming the cycle walk corrected. **`modules.hero` 450 lines to
331** (`wc -l`), 125 lines out and 6 in (`grep -c '^<'`, `'^>'` of the diff). `ce-frozen fmt` writes both files
back unchanged; `ce-frozen check selfhost/main.hero` exit 0; stage 2 built, `ce-q1r`, `sources 2ea57f890c143853`.

**The cases**, each checked, built and its binary run under `timeout -s KILL 20` by `ce/tools/run_q1.sh`,
outputs in `ce/out/q1-frozen/` and `ce/out/q1-q1r/`. Under `ce-frozen` all twelve are `error[module_cycle]`,
exit 1. Under `ce-q1r`:

| case (`ce/cases/q1/`) | what it holds | `ce-q1r` |
|---|---|---|
| `face` | the writer's face and its private module (`bw/q1/cycle/`, copied) | exit 0, prints `26` |
| `ring3` | `ra` uses `rb` uses `rc` uses `ra`, `rc` building `ra`'s record | `12` |
| `mutual` | `is_even` and `is_odd` recursing into each other across two modules | `true true` |
| `generic` | a generic `pick<T>` in `ga`, called from `gb` at `ga.Box`, `[str]` and `i64` | `10` |
| `consts-cycle` | `ca.A = cb.B + 1`, `cb.B = ca.A + 1` | **`error[constant_cycle]`**: *"`constant A` is defined in terms of itself through `B`"*, exit 1 |
| `consts-chain` | `ca.A = cb.B + 1`, `cb.B = 2`, `cb.C = ca.A * 10` | `3 30` |
| `sized-cycle` | `record A` holds `sb.B`, `record B` holds `sa.A`, both by value | **`error[no_size]`**: *"`record B` contains itself through `A`, so it has no size"*, exit 1 |
| `sized-list` | the same through `items: [sa.A]`, with a function each side | `9` |
| `variant` | `variant Shape` with a case holding `vb.Bag`, `Bag` holding `[va.Shape]` | `5` |
| `extern` | an `extern` group in each of two ring modules, a C record of `xa.h` passed to `xb` | `27` |
| `pkg` | a package's face `shapes` and `shapes/internal/calc` in a ring, reached by a catalogue's `path` | `30 10` |
| `ring3-test` | a `test` block in `rb` calling round the ring, `ce-q1r test main.hero` | `1 test, all passed` |

Two of my first cases were wrong and the route was not: `va.Shape.circle(...)` is no construction in the
language (rewritten as `.circle(r: 2)` in a typed position), and an `extern` function called across modules
is **`error[extern_across_modules]`** in any graph, a rule of its own that Q1-R does not touch (rewritten so
each module calls its own header through a function of its own).

**The cache across a ring, run** (`ring3`, `ce-q1r`): a warm rebuild after an edit to each member prints the
edited value each time (`212`, `214`, `321`), two objects compiled per edit (the edited module's and the root's,
whose text carries the program's sources key), and the three files restored compiles **0** objects and prints
`12`. **`--emit-c`**, the fused path the seed and the emissions read, on `ring3`, `variant`, `extern`,
`sized-list` and `generic`: exit 0 each, 11,260 to 18,943 bytes, and `clang -std=c11 -c` on each emitted file
exit 0 with 0 bytes on stderr.

**What rests on acyclicity, read.** The per-module key is a digest of the TU's own emitted text and what clang
says its opening reached (`selfhost/cli/units.hero:7-20`, `:166-167`); a TU carries every function's prototype
and the typedefs its own members reach, ordered by `emit/typeorder.hero`'s post-order over **types**, not
modules (`selfhost/emit/unit.hero:26-49`). The type graph's own cycles are `check/sized.hero`'s (run above,
`no_size`), and constants' are `resolve/cycles.hero`'s (run above, `constant_cycle`). `components` orders link
objects by file-table order, a function of the `use` graph but not of its acyclicity (`units.hero:91-109`).
`grep -rn -i -E 'topolog|dependency order|post-?order'` over `selfhost/` prints only `typeorder.hero`,
`type_needs.hero` and `check/sized.hero`, all over the type graph. Package placement (`selfhost/module/`, 24 files,
5,414 lines) walks folders in byte order (`answering.hero:9-22`), not `use` lines. The card and doc readers
(`tests/harness/module_docs.hero`, `package_card.hero`) read each module's text and walk no graph. **Panel 031
R6's reason, *"the per-module cache needs a topological order"*, does not hold of the compiler that shipped**:
nothing I found reads the module graph in an order, and the runs above agree. A question, not a premise: a ring
**between two packages** with catalogue entries naming each other was not built.

**What rests on the refusal is comments**: `grep -rn -i -E 'module_cycle|module cycles|refuses (the )?cycle'
selfhost --include='*.hero'` prints **22 lines in 20 files** outside `modules.hero` (the brief's eight and twelve
more, among them `cli/artifact.hero:56`, `cli/runtime_key.hero:13,109`, `emit/synth.hero:187`, `ir/lower.hero:16`,
`ir/inout.hero:61`, `parse/group.hero:23`, `parse/tails.hero:17`), plus three in `tests/harness/suite_layout.hero`
(`:18`, `:75`, `:251`). Each is a seam note, a sentence that becomes false under Q1-R and is corrected, not code.

**The compiler's own knots**: under Q1-R `ir/flatten.hero` (1,414 lines) and `grammar_expr.hero` (1,767) may
be split, since mutual recursion across modules builds and runs (`mutual`); their comments say the refusal is
the only reason they are one file (`flatten.hero:14-19`, `grammar_expr.hero:5-12`). Unbuilt.

**What Rebel could delete, counted** (`wc -l` of the files the brief's table names, `ce/packages/rebel/0.1.0/files`):
the 23 mirror modules (21 in the table, `xlsx` and `scanner` beside it) **5,812 lines**; the 8 twice-written
code modules **477**; the 3 event lists **561**. An upper bound on what Q1-R frees, not what it deletes: each
file holds more than its mirror, and the conversions at the faces' edges are not counted.

**Q1-A priced, unbuilt.** A transparent `alias` is a new declaration kind: `selfhost/ast.hero:546`'s `Decl` gains
a case, and `.test_decl`, the arm every match over a declaration's kind writes, stands on **324 lines in 172
files** of `selfhost/` (`grep -rn '\.test_decl' selfhost | wc -l`, `grep -rln ... | wc -l`), and the compiler's
own count agrees: `ce/exp/q1a/`, a copy of `selfhost/` whose `ast.hero` `DeclKind` gains `alias_decl`, checked
by `ce-frozen`, prints **322 `non_exhaustive` errors in 172 files**, every match that would owe an arm; then the resolver's type names, the checker's construction and `::` through it (panel 135's
*"everywhere"*), the printer, `heroes grammar`, the cards. It removes Rebel's mirrors of types only, not the codes
written twice or the event lists, which are values and functions. Q1-R removes all three by deleting code.
**Q1-R2 priced, unbuilt**: it keeps `cycles` and adds a classification of each strongly connected set of modules
(a face and its own `internal/` or not), about 30 to 40 lines on top of today's 112, and a sentence a reader meets.

- `verdict`: **Q1-R approve. Q1-A veto. Q1-R2 object. Q1-C object.**
- `section`: design.md §1.1 (simplicity is the ceiling: Q1-R lowers it) and §1.7 with Part 5 (Q1-A is a core
  construct, a new declaration kind every pass handles); §4.1 (`docs/design.md:942-943`), whose cycle clause
  Q1-R strikes. design.md does not state why cycles were refused; panel 031 R6 does, and its reason is gone.
- `implementation_cost`: Q1-R **minus 119 lines** of `selfhost/modules.hero` (450 to 331, built,
  `ce/routes/q1/q1r.diff`), 0 lines added anywhere else, 22 comment lines in 20 files corrected. Q1-A: a new
  declaration kind, **322 incomplete matches in 172 files** by the compiler's count, plus resolver, checker,
  printer, grammar, cards; unbuilt. Q1-R2: about +35 lines in `modules.hero` over Q1-R's deletion and a spec sentence; unbuilt.
- `needed_for_self_hosting`: no (the compiler compiles itself under the refusal; Q1-R lets two knots of 1,414
  and 1,767 lines split, which nothing needs).
- `argument`: The refusal is 119 lines of checker guarding nothing the compiler needs: the cache keys a TU by
  its own text, the type order walks types, and constants' and sizes' own cycle checks hold across a ring
  (run: `constant_cycle`, `no_size`). Ten ring cases build and run under Q1-R, the cache, the fused C, a
  test and a package included. Removing a refusal is the cheapest change a compiler can take: it shrinks.
  Rebel paid for the refusal with mirrors, twice-written codes and event lists, files totalling 6,850
  lines. Q1-A answers the same need with a new declaration kind, which every pass must learn and which
  frees the type mirrors alone. Q1-R2 keeps the walk and adds a rule.
- `prediction`: at the batch that lands Q1-R, `selfhost/modules.hero` reads at most 335 lines, the compiler's own
  tests change in no assertion but `modules.hero`'s ring case, and the full net reads the same failed count
  as the frozen tree's.
- `condition`: a program that builds wrong, or a warm build that serves a stale object, with a ring and
  without `module_cycle` (the cases above are the attempt and found none); or a ring between two packages
  that fails to place or build. Q1-A's veto lifts on a measured need Q1-R cannot meet, a name a program must
  write that a ring cannot give it.

## Q2, the key that is not read

**Reproduced** (`ce/cases/q2/`, the writer's 20 copied, run by `ce/tools/run_q2.sh`, outputs `ce/out/q2-*.txt`):
under `ce-frozen` the 20 read exactly as the brief's table, 11 accepted and 9 refused (`a`, `e`, `g`, `i`, `k`,
`l`, `o`, `p`, `q`), every refusal `cannot_infer`. I added 17 programs (`x01` to `x17`), my own, not copied from
the spec-warden's folder:

- **The spec-warden's plain shapes, reproduced**: `x = [[], ["a"]]` refused, `[["a"], []]` accepted; `{"a": [],
  "b": ["c"]}` refused, `{"b": ["c"], "a": []}` accepted; `[{"a": []}, {"b": ["c"]}]` refused, each with
  *"the array literal is empty"* said of a literal holding `["a"]`.
- **Found, a second order-dependence inside one literal that no brief names**: `b: u8 = 7` then `x = [1, b]` is
  **`error[type_mismatch]`: expected `i64`, found `u8`**, and `x = [b, 1]` is accepted and runs (`x07`, `x08`).
  The cause is the same line: the literal's type is its first element's, and `1` is `i64` by default.
- Panel 203's pairs (`first(a: [1], b: [])` and its mirror, `{"k": 1}` with `{}`, `[[1]]` with `[[]]`) are one
  verdict either way under the frozen compiler (`x09` to `x12`, after my first versions tripped on an unused
  parameter).

**Why**: `check/walk.hero:237-260` synthesises an array literal's element 0, and a map literal's entry 0, and
checks the rest against it, so when element 0 refuses itself (`[]`, `{}`, `ok(…)`, a case) nothing else is read.
`check/waiting_order.hero:76-110`'s `weight` mirrors that rule by its own comment (*"an array literal takes its
first element's type"*). For a generic call, the waiting argument is synthesised alone (`walk.hero:1790-1803`),
`unsettled` is computed before anything binds, and `generic_argument.hero:276-297` then writes *"no other argument
and no context says what `K` is"*. With one waiting argument `settling.hero:86-94` goes to that synthesis at
once.

**Built: Q2-R**, in `ce-q2r` then in `ce-q4r` and `ce-all` (`ce/routes/q2/`):

1. **The literal's lead** (`waiting_order.hero` +48 −5, `walk.hero` +20 −8): `lead` is the first element, key or
   value whose `weight` is not `REFUSES`, element 0 where every one refuses; the walk synthesises the lead and
   checks the others, and `weight` follows the same rule. A literal whose element 0 does not refuse itself is
   read exactly as before, so **no program accepted today changes its type**: only a literal whose element 0
   refused moves, and every such literal was refused.
2. **The typed parts of a waiting literal** (new `check/waiting_parts.hero`, 122 lines; `settling.hero` +12):
   before the refusing forms are told, a literal is read down against its parameter and a letter nothing settled
   takes the type of a part that needs no synthesis (a string, an interpolation, a `bool`, a local), then in a
   second round a literal's default width (a number, a character, a local of `i64` or `f64`), by form and never
   by position. In the one-argument shortcut it runs only where the argument's weight is `REFUSES`, so a call
   accepted today takes the path it took. Nothing is synthesised twice.

| program | frozen | Q2-R (`ce-q4r`, `sources 2b9b2eaa546c43db`) |
|---|---|---|
| `a` `{"a": []}`, `g` `{1: []}`, `k` `{"a": {}}`, `l` `[{"a": []}]`, `o` `{"a": ok("b")}` | refused | **accepted, run, print `1`** |
| `p` `{"a": [], "b": ["c"]}`, `q` `[[], ["a"]]` | refused | **accepted, print `2`**, as `n` and `s` |
| `x01`, `x03`, `x05`, the plain literals refused for their order | refused | **accepted, print `2`** |
| `x13` `{k: []}` with `k = "a"` | refused | accepted, `1` |
| `x15` `{1: [], k8: []}`, `x16` `{k8: [], 1: []}` | both refused | **both accepted, `2`**: one verdict either order |
| `i` `{K: [V]}` with `{"a": []}` | refused naming `K` | refused, *"no other argument and no context says what `V` is"*: true |
| `e` `[[]]`, `x06` `m = {"a": []}`, `x17` `[ok(1), ok(2)]` | refused | refused, rightly |
| the other 11 of the 20, `x02`, `x04`, `x08`, panel 203's four pairs | accepted | accepted, the same output byte for byte (`diff` of the pairs' `check` output: identical) |
| **`x14` `{name(): []}`** | refused | **still refused, and the message still false**: a key written as a call needs synthesis, which this route does not do |
| **`x07` `[1, b]`** | refused | **still refused**, where `[b, 1]` is accepted |

**What a whole repair still owes, priced, unbuilt**: `x14` needs a part synthesised once and not again by the
argument's later check (the walk re-synthesises every part it checks), about 20 lines in `waiting_parts.hero` and a
guard in `check/walk.hero` against a second synthesis; `x07` needs the lead taken by `weight`, lightest first, as
panel 203 ruled across arguments, which can change the type of a program accepted today (a later element of a
narrower type than element 0's default) and so needs the census to read it. Neither is a reason for a sentence.

**The census** (below, § The census): what Q2-R moves among the 1,802 roots.

- `verdict`: **Q2-R approve; Q2-C object.**
- `section`: design.md §4.12 (panel 203: an argument's verdict *"is the same in either order"*,
  `docs/design.md:1976-1996`); §1.1 (the repair is a checker change with no new construct, inside two passes).
  design.md says nothing of order inside one literal; CLAUDE.md § 12, *"Spec beats compiler"*, decides it.
- `implementation_cost`: built, **+202 −13 lines** in `selfhost/check/`: `waiting_parts.hero` +122 (new),
  `waiting_order.hero` +48 −5 (354 lines), `walk.hero` +20 −8 (2,488 lines, a knot past its ceiling: the growth
  belongs in `waiting_order` and could move there), `settling.hero` +12 (136). No lexer, IR, emitter or runtime
  line. Q2-C: 0 compiler lines and +21 real spec tokens (the spec-warden's refresh).
- `needed_for_self_hosting`: no.
- `argument`: The spec says the key types `K`; the compiler reads only element 0. That is a bug, not a rule: it
  makes the verdict depend on order inside one literal, even with no type parameter at all (`[[], ["a"]]`
  refused, its reverse accepted), and its message says *"no other argument says"* where a key does. Q2-R
  repairs it inside the checker, touching no construct, and moves only programs refused today. Q2-C would write
  that order-dependence into the spec as a rule a reader must learn, for a behaviour no program or compiler fact
  needs. A defect, `blocking`: a correct program refused with a false message.
- `prediction`: at the batch that repairs it, the 20 programs of `bw/q2/` read 18 accepted and 2 refused (`e`
  and `i`), and the census moves no root from accepted to refused by `cannot_infer` or `type_mismatch`.
- `condition`: a program accepted today that Q2-R refuses or types differently (the census is that test); or a
  program that needs element 0 to decide a literal's type.

## Q4, a `u8` where a value becomes text

**Built: Q4-R** (`ce/routes/q4/`): a new module `selfhost/check/byte_text.hero` (95 lines), a new class
**`byte_as_text`**, called from `check/interp.hero`'s `hole` (+5 lines) and from `check/walk.hero` (+3: one call in
`builtin_dispatch` for `print`'s arguments, one before a dotted built-in for `s[1].print()`, and its `use`).
Message: *"`s[0]` is a `u8`, a byte, and a hole of an `f"…"` literal writes a byte as its number, never as a
character — write which one you mean"*, two `guess` fixes, *"write its character, `s.slice(from: 0, to: 0 + 1)`"*
offered only where the value indexes a `str`, and *"write its number, `to_str(s[0])`"*. A new class rather than
widening `hole_not_renderable` (the spec-warden's finding 4 asks): that class's words, *"`to_str` takes an
integer ... which has no text"*, would be false of a `u8`, which `to_str` takes and renders.

**The cases** (`ce/cases/q4/`, `ce/out/q4/`), frozen against Q4-R:

| case | frozen | Q4-R |
|---|---|---|
| `f"[{s[0]}]"`, `print(s[0])`, `c = s[0]` then `f"first {c}"`, `print(s[0] + 1)` | `[117]`, `117`, `first 117`, `118` | `byte_as_text` each, exit 1 |
| `s[1].print()` | `115` | `byte_as_text` (after my first build missed it: the value before the dot is typed apart, `walk.hero:1519`) |
| `to_str(s[0])`, `f"{to_str(b)}"`, `s.slice(from: 0, to: 1)` | `117 65`, `[u]` | the same, exit 0 |
| `c = 'a'` then `f"[{c}]"` | `[97]` | `[97]`: an `i64`, untouched, the shape beside |
| `u16`, `i8` in `print` and a hole | `7 -3 7-3` | the same |

**The census** (§ The census, roots 1,802): **226 distinct sites** in **98 files** refused by `byte_as_text`
(`ce/out/q4-sites-src.txt`, each site's line printed and read):

- **Rebel, 69 sites in 39 package modules and 6 drivers**, which refuse **90 of the 160 drivers**. Read one by
  one: **65 mean the number** (an error message saying *"byte {b}"*, a version, a colour channel, an SGR code, a
  palette index; `json/jq/internal/lexer.hero:340-343` even slices the character below 128 and writes the
  number above); **4 mean the character** and write its number, Q4's trap in the wild:
  `sql/postgres/internal/backend.hero:155` (*"a message of kind {head[0]}"*, Postgres kinds being letters),
  `sql/postgres/internal/session.hero:168` (the same), `time/internal/tzif.hero:100` (*"is {data[at + 4]}, none of
  0, '2', '3' or '4'"*) and `url/idna/internal/punycode.hero:284` (*"a byte that is no digit, {s[at]}"*).
- **Goldens, 157 sites**: 42 of the 811 `run` roots refused (buffers, widths, overflow, indices) and 23 sites in 11
  `check` cases; every one prints a byte's number on purpose and its `.expected` blesses the number.
- `examples/` 0 of 71; `selfhost/main.hero` not moved: the compiler prints no `u8` as text.

So Q4-R catches **4** wrong programs at the price of **222** correct sites each gaining `to_str(…)`: 1.8% of what it
refuses is the trap. **Q4-R2** (an index into a `str` only): 18 of the 226 sites, my diagnostic's character fix
marking them; 17 golden number prints and **one** of the four traps (Punycode). The other three traps read a
byte from a `[u8]`, which neither Q4-R2 nor a sentence about `str` sees. **The shape beside**, a character
literal defaulting to `u8`: 15,988 character literals by `grep -rhoE "'(\\.|[^'\\])'"` (the compiler 2,287, Rebel
10,429, `examples/` 3,193, goldens 79); what the default would move among them is unbuilt.

- `verdict`: **Q4-R object. Q4-R2 object. Q4-C approve, with the 4 Rebel traps filed as package defects.**
- `section`: design.md §1.1 and §1.7 (a new class every program that prints a byte pays for); §1.12 does not reach
  it: a byte's number is a defined value, no fault and no corruption. The thesis rule (CLAUDE.md, *"every
  plausible LLM mistake is a compile error"*) is the strongest case for Q4-R and the census weighs it: the
  mistake is plausible (4 in Rebel's 69) but the refusal is 98% correct programs.
- `implementation_cost`: built, **+103 lines** in `selfhost/check/` (`byte_text.hero` 95, `interp.hero` +5,
  `walk.hero` +3), one class, no lexer, IR, emitter or runtime line; **its price in programs**: 226 sites in 98
  files, 90 Rebel drivers and 42 `run` goldens refused until each gains `to_str`. Q4-R2: the same module with the
  test narrowed to `sliced`'s, about 80 lines.
- `needed_for_self_hosting`: no.
- `argument`: The census says what the trap costs: of 226 places where a byte becomes text, 222 mean the number
  and say so, and four mean a character. Q4-R makes all 222 write `to_str`, a second spelling of what they
  already write, to catch four; Q4-R2 misses three of the four, because the bytes come from a `[u8]`. A byte's
  number is not a robustness fault. The four are package defects, found now, and Q4-C's sentence tells the
  next reader what `print` does. If the blind seat's task 4 measures readers falling in, the refusal's price
  is still 55 correct sites per trap.
- `prediction`: under Q4-R as built, the census of the frozen tree's 1,802 roots refuses at least 130 roots that
  build today (here 90 Rebel drivers and 42 `run` goldens, 132), and at most 5% of its distinct sites write a
  character's number; checkable at the first batch that tries it.
- `condition`: the blind seat's task 4 showing at least 3 of `now`'s 5 silent wrong with no sentence able to
  prevent it (registered as W6 by the spec-warden), and a census where the character-meant share exceeds 10%;
  or a narrower test than Q4-R2 that catches the `[u8]` traps without the number prints.

## Q3, a record or a variant with a type parameter

**The silent mismatch, run on all four containers** (`ce/examples-rebel/ce_*.hero`, built by `ce-frozen` through
the catalogue's `path`, each binary 3 times under `timeout -s KILL 20`, `ce/out/q3/`):

- `deque` (the writer's program, copied): `taken 10, jobs left 3, urgent left 5`, exit 0, 3 of 3. The writer's
  measurement holds.
- `cache`, `indexed`, `ordered`, **unrun until now**: the first shape's `put` given the second shape's lists is
  accepted. Read afterwards with the first shape's own lists, the program aborts (*"the value list holds 1
  elements where its structure holds 2: it is not the list the structure was built over"*, exit 134, 3 of 3),
  one honest call too late. **Read only through the second shape, it runs on in silence**: the second map
  reports size 2 and holds a key never put into it, `"q"` worth `20` for `cache` and `indexed`, `7 → "seven"`
  for `ordered`, its own second entry overwritten, exit 0, stderr empty, 3 of 3 each (`ce_*_mix2.hero`). So
  **all four promises are false** (`deque.hero:17-18`, `cache.hero:109`, `indexed.hero:20`, `:75`,
  `ordered.hero:35`): a defect for the coordinator to file against the package, not the language.
- **Whether any check can keep the promise (Q3-C's mechanism)**: the shape and the program's list share no
  identity (spec § 3, values are copies), and the shape cannot hold a `K`, so it cannot compare keys. What it can
  hold is an `[i64]` of each slot's hash, checked at every slot a call touches: O(1) per call, and it catches a
  foreign list unless two hashes collide. Unbuilt, and it needs a hash of any `K` the package can call; a
  question for the package, not a premise.

**Q3-R priced by the compiler, not estimated** (panels 119 and 251's method): `ce/exp/q3/`, a copy of `selfhost/`
whose type table (`check/table.hero:69-99`) gains one case, `applied` with `decl` and `args`, checked by
`ce-frozen`: **290 `non_exhaustive` errors in 111 files**, every match over a type's kind that would owe an arm.
Panel 251's carried *"288 ... in 109 files"* for `(thread T)` agrees. Carrying the arguments inside `.named`
instead moves the price, not its size: `.named` stands on 510 lines in 172 files (`grep -rn '\.named' selfhost`).
On top of the arms: the parser (`parse/members_below.hero:219-230`'s refusal becomes a reader, `parse/type.hero`
660 lines gains `Name<T, U>`), inference at a construction and in a `match`, field access under substitution,
`ir/mono.hero` (548 lines) instantiating types as it does functions, one C struct per instance and its
descriptor (`emit/descriptors.hero` 221, `runtime/parts/desc.c` 207), the mangler (`emit/mangle.hero` 285), the
formatter, `heroes grammar`, the cards. **Panel 029's two rules reach a type this way**: R5 (an instance named by
its canonical rendered type) names `Pair<i64, str>` as it names `pick<i64>`; R4's structural refusal
(`ir/mono.hero:19-28`, an instance reached from an ancestor of the same declaration whose arguments contain the
ancestor's and one grew) refuses `record Nest<T>` holding `inner: [Nest<[T]>]` **at its declaration**, since the
growth is visible in the field's type with no instance at all; `variant List<T>` holding `List<T>` by value is
`no_size` as today (`check/sized.hero`), and through `[List<T>]` is finite. Neither was built: the smallest slice
that runs, `record Pair<A, B>` through parser, checker, mono and emitter, is beyond this sitting's time, and is
**not built**.

**Principle 0, counted** (`ce/tools/twin_records.py`, every `record` of `selfhost/` with two or more fields,
548, grouped by field names with every type name replaced): **two families** a type parameter would write once,
`Token`, `Stmt`, `Expr`, `Pattern`, `TypeNode` (`kind` and `span`, five records of two lines each) and `UseRows`
with `NameSets` (two); three more groups share field names by coincidence. The compiler compiles itself without it.

- `verdict`: **Q3-R object** (a core construct; the brief's veto asks a built program that dies under it, and I
  built none). **Q3-C approve**, with the four false promises filed against `containers` at once.
- `section`: design.md §1.7 and Part 5 (a type former is core: every pass handles it, nothing erases it);
  §4.12's *"Generics on functions only, not on types"* (`docs/design.md:2020`); §1.1, the ceiling.
- `implementation_cost`: **290 match arms in 111 files** before any new logic, by the compiler's own count; then
  parser, inference, substitution, monomorphisation over types, per-instance structs, descriptors, mangler,
  formatter, grammar and cards, and a declaration-time refusal of growing self-reference. My estimate, not
  measured: over 1,500 lines, a milestone of its own.
- `needed_for_self_hosting`: no (two families, seven records of two or three lines each, would shrink).
- `argument`: Q3-R adds a type former: 290 matches the compiler itself lists as incomplete, then the
  monomorphiser, the emitter and the runtime's descriptors over types. That is the core growing, the one
  thing §1.7 prices highest. What it buys is measured: four containers whose shape accepts another's lists
  and silently overwrites a map. That bug lives in a package and can be repaired there (a slot hash, or a
  shape that returns its lists); the four sentences that promise an abort are false today and should go first.
  The compiler needs no generic record. Only the blind seat's task 3 can pay for the form.
- `prediction`: if Q3-R lands, its first commit that builds `record Pair<A, B>` end to end touches at least 111
  files of `selfhost/`, the count `non_exhaustive` gives today.
- `condition`: the blind seat's task 3 counted rule firing (`q3-r` beating `now` by at least 3 by the rubric); or
  a package repair shown impossible, a mismatch no check in the package can catch at O(1) per call.

## Q5, an exponent the language prints and refuses

**Built: Q5-R** (`ce/routes/q5/q5r.diff`, in `ce-all2`, `sources 679e4c547fb98958`): `selfhost/number.hero`
**+29 −17** (590 to 602 lines), the refusal at `:146-172` replaced by a reader: an exponent after digits, with or
without a point, makes the token a `float_lit`; an `E` is a new class **`exponent_case`** with a `certain` fix
to `e`, as `0X` is `base_prefix_case` today (`number.hero:60-104`); `exponent_literal` leaves; the lexer's own
test at `:539-550` rewritten. **Nothing else**: the only readers of a float literal's value are C's `atof` and
`strtof` (`emit/literal.hero:41-47`, `:145-155`, defect 2276), which read an exponent already, so the emitter's
`%a` C is unchanged.

**The cases** (`ce/cases/q5/`, `ce/tools/run_q5.sh`, `ce/out/q5-*`): under `ce-frozen` every exponent is
`exponent_literal`, exit 1. Under Q5-R:

| case | Q5-R |
|---|---|
| `x = 0.000001`, `y = 1e-06`, `print(x)`, `print(y, " ", x == y)` | `1e-06`, `1e-06 true`: what `print` writes reads back |
| `1.0e-6`, `2.5e3`, `1e21`, `1e+2`, `1_000e-3` | `1e-06 2500.0 1e+21 100.0 1.0`: **`1e21`, with no point, is a float**, and the reader takes `_` |
| `2.5E3` | `error[exponent_case]`, *"writes its exponent with `E` — this language spells it `e`"*, `fix (certain)` |
| `small: f32 = 1e-06` | `1e-06` |
| `n: i64 = 1e3` | `error[type_mismatch]: expected i64, found f64` |
| `x = 1e-7 * 3.0` printed, then the printed text written back | `3e-07`, `true` |
| **`1e400`** | **builds, prints `inf`, exit 0, no word** |
| **`1e-400`** | **builds, prints `0.0 true`, no word** |

**Found beside it, under the frozen compiler, no exponent needed**: a float literal of 401 written digits builds
and prints **`inf`** at exit 0, and an `f32` one of 41 digits likewise (`e09`, `e10`); the emitter says so in a
comment, *"A literal can reach one — enough digits and `atof` says so"* (`emit/literal.hero:182-188`), and no check
refuses it. A literal whose value is not the number written is a wrong program accepted, today: a defect for the
coordinator to file (`blocking` by `.claude/rules/verification.md` § Bounded discovery's *"a wrong value"*). Q5-R
makes it a few characters long; **the robust Q5-R refuses a float literal that `atof` (or `strtof` at an `f32`)
reads as infinite, or as zero where a digit is not zero**, about 20 lines with an `extern "stdlib.h"` as
`literal.hero` declares, unbuilt, and that refusal closes the written-out shape too.

**`fmt`**: `ce-all2 fmt` writes `1e-06`, `1.0e-6`, `1_000e-3` and `1e+2` back unchanged, as `ce-frozen fmt` keeps
`0.000_001` (run): a number already has several spellings under `fmt`, so only `E` is refused. **The census**
moves exactly the two `check` goldens that hold `exponent_literal` (`exponent-literal.hero`: three gone, one
`exponent_case` for its `E`; `fixedbugs-2391-*`: one gone) and nothing else among 1,924 roots;
`tests/golden/full/fixedbugs-2390-*` is not a census root and is unrun.

**Q5-P priced, unbuilt**: `runtime/parts/f64.c` (341 lines) writes shortest round-trip by `%.*g` and `strtod`
(`:24-40`); a positional form changes that ladder, and **8 of 811** `run` expectations print an exponent today
(the writer's pattern, re-run: 8, and 8 across every golden `.expected`). Written positionally the largest `f64`
holds 309 digits and the smallest subnormal 326 characters (Python's `Decimal` of each shortest form, `python3 -I`;
what Heroes' own ladder would write is unrun). **Q5-U**: no compiler line; a float's `_` is read today and kept by
`fmt`, so the arm is a sentence only.

- `verdict`: **Q5-R approve, with the range refusal as part of it. Q5-P object. Q5-C object. Q5-U approve as a
  sentence (no compiler cost).**
- `section`: design.md §1.1 (a lexer change, erased before the checker: the type checker, the lowering and the
  backend see a `float_lit` as today, Part 5's sugar side); §1.12 for the range refusal, a literal that is not its
  value; §4.9 (`docs/design.md:1752-1755`, the printer's round trip).
- `implementation_cost`: built, **+29 −17 in `selfhost/number.hero`**, 0 lines in `selfhost/check/`, `ir/`,
  `emit/` or `runtime/`; one class in, one class out. The range refusal: about +20, unbuilt. Q5-P: the ladder in
  `runtime/parts/f64.c` and 8 `run` expectations.
- `needed_for_self_hosting`: no.
- `argument`: Q5-R is the cheapest change in this sitting: twelve net lines of lexer, nothing past it, because C
  already reads the form. It makes the language read what its own `print` writes, which Q5-C can only describe.
  The cost it carries is the range: `1e400` is a short way to write `inf` in silence. But that wrong program
  exists today with 401 digits, so the refusal is owed either way, and Q5-R should land with it. Q5-P changes the
  runtime and makes every extreme value hundreds of characters long, to keep a refusal.
- `prediction`: Q5-R lands with `selfhost/number.hero` changing by at most +60 lines with its range refusal, and no
  line of `selfhost/emit/`, `selfhost/ir/` or `runtime/` changing; its census moves the 3 goldens that hold
  `exponent_literal` and nothing else.
- `condition`: a float literal C reads differently from the language (a value whose `%a` disagrees with `atof`);
  or the range refusal not buildable without a second reader of decimals beside C's.

## The census

`ce-<compiler> check <root>` over every root, two at a time (`ce/tools/census.sh`, `census_dirs.sh`,
`census_one.sh`), each output whole with its exit in `ce/out/census-<label>/`, compared root by root on the exit
and the multiset of diagnostic codes (`ce/tools/census_diff.py`, `ce/out/census-diff2.txt`). **One check's cost**
before running: a Rebel driver 8,538,649,768 instructions retired, a golden case 86,901,496 (`/usr/bin/time -l`).

- **Roots, 1,924**, by command: `selfhost/main.hero`; 71 files of `examples/` holding `function main()`
  (`grep -rl`); 811 `tests/golden/run/*.hero` and 759 `tests/golden/check/*.hero` (`ls`); the 160
  `examples-rebel/reach_*.hero`; and the 122 directory cases holding a `main.hero` (26 under `run/`, 96 under
  `check/`; the 23 directories without one are module folders a top-level case uses).
- **`ce-frozen`**: 1,043 + 26 accepted, 759 + 96 refused, the refused being exactly the `check` goldens.
- **`ce-all2`** (Q1-R, Q2-R, Q4-R, Q5-R together, `sources 679e4c547fb98958`): **149 roots move**, every move
  attributed by its code: `byte_as_text` +576 diagnostics at 226 sites (Q4); `exponent_literal` −4 and
  `exponent_case` +1 in two `check` goldens (Q5); `cannot_infer` −1 and `type_mismatch` −1 in `fixedbugs-402:57`
  and `fixedbugs-541:88` (Q2: `[ident, twice]` and `[ident, half]`, whose reverses the frozen compiler accepts,
  `ce/cases/q2b/`, run); nothing from Q1 (the tree holds no ring). The 122 directory cases do not move.
- **My first all-routes compiler** (`ce-all`, `sources 45bd21c92e371ed7`) also lifted 9 refusals inside
  `ok(…)` in `fixedbugs-541`, `-544`, `-551`, `-552`: my parts walk read inside `ok(…)`, which spec § 9 says gives
  no type. The census found it; the arm was removed, `ce-all2` rebuilt and the whole census re-run.

## Premises of the briefs I found false, each with its command

1. **There is no `heroes doc`** at the frozen commit (`00-shared.md` § Question 1's *"`heroes doc`, the cards"*,
   *"Not run under it ... `heroes doc`"*; `compiler-engineer.md` § Question 1 and § Question 3): `grep -c
   'Command(' selfhost/cli/table.hero` prints 16 rows (`:187-202`), none `doc`, and `ce-frozen --help` lists none.
   The documentation and the cards are read by harness programs (`tests/harness/module_docs.hero`,
   `package_card.hero`), which walk no `use` graph.
2. **What rests on `module_cycle` is not eight comments** (`00-shared.md` § The record, *"the eight `grep -rn
   module_cycle selfhost` prints"*): `grep -rn -i -E 'module_cycle|module cycles|refuses (the )?cycle' selfhost
   --include='*.hero'` prints 22 lines in 20 files outside `modules.hero`, and `tests/harness/suite_layout.hero`
   three more (`:18`, `:75`, `:251`). All comments; none code.
3. **The census is 1,924 roots, not 1,852** (`compiler-engineer.md` § The census, carried from the critic): by
   `ls`, `tests/golden/run/*.hero` is 811 and `tests/golden/check/*.hero` 759, not 837 and 783; with the 122
   directory cases (`ls tests/golden/{run,check}/*/main.hero`) the roots are 1,924.
4. **An order-dependence inside one literal is measured, and pinned, already** (`00-shared.md` § Question 2,
   *"never measured inside one literal"*): `tests/golden/check/fixedbugs-402-*.hero:57` blesses `fs = [ident,
   twice]` as `cannot_infer` and `fixedbugs-541-*.hero:88` blesses `[ident, half]` as `type_mismatch`, while
   `[twice, ident]` and `[half, ident]` build and run (`ce/cases/q2b/`, `ce-frozen check` exit 0, `100`, `2`). And a
   third shape no brief names: `[1, b]` with `b: u8` refused, `[b, 1]` accepted (`ce/cases/q2/x07`, `x08`).
5. **Q2-R as worded would lift correct refusals** (`00-shared.md`, *"A waiting literal's parts that have a type of
   their own (a key, any entry, in any order) settle its letters"*): read literally it reaches inside `ok(…)`, and
   my first build did, lifting `held(ok(1))` and eight more refusals that spec § 9 keeps (*"`ok(...)` ... give
   none"*). The route needs the limit: a part needing no synthesis, never the inside of `ok(…)`.
6. **The promise of `cache`, `indexed` and `ordered` is false, not only unrun** (`00-shared.md` § Question 3), and
   their failure is worse than `deque`'s: the mismatched call silently writes a key into the other map
   (`ce/examples-rebel/ce_*_mix2.hero`, exit 0, 3 of 3 each).
7. **A float literal past range is a silent `inf` today** (`00-shared.md` § Question 5 asks what `1e400` becomes
   under Q5-R as a new question): 401 written digits already build and print `inf` under `ce-frozen`
   (`ce/cases/q5/e09`, `e10`), so the hazard is not Q5-R's to introduce, and is a defect now.
8. **A `u8`'s trap in Rebel is mostly not an index into a `str`** (the frame of Q4-R2, *"the trap's own form"*):
   three of the four character-meant sites read a `[u8]` (`postgres/internal/backend.hero:155`,
   `session.hero:168`, `time/internal/tzif.hero:100`); only Punycode's indexes a `str`.

## What I cut

- **Q3-R's smallest slice**, `record Pair<A, B>` through parser, checker, monomorphiser and emitter: not built;
  priced by the compiler's `non_exhaustive` count instead.
- **Q1-A and Q1-R2**: priced, not built (Q1-A by the compiler's count of 322 matches).
- **A ring between two packages** whose catalogues name each other: not built.
- **Q4-R2 as its own compiler**: its census read from Q4-R's, by the diagnostic's character fix.
- **The two Q2 shapes left** (`x14` a key written as a call; `x07` the default width): priced, not built.
- **Q5's range refusal and Q5-P**: priced, not built. `tests/golden/full/fixedbugs-2390-*` not run.
- **The character literal defaulting to `u8`**: counted (15,988 literals), its census not built.
- No `heroes test` over the compiler or a package, by the brief's rule; none of my routes' stage 2 ran its own
  tests, so `selfhost/modules.hero`'s and `number.hero`'s rewritten tests are unrun.

Report closed 17:01 CET 2026-11-19 (`date`). Nothing written outside my folder and this report, nothing
committed, no paid run, no credential read.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
