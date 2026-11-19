# Panel 252: modules may form a ring, a literal's key types its letter, a generic record waits, a byte stays its number, and `strtod` reads a printed float back

Sitting B of M-rebel-library, *"the language as Rebel met it"*, convened 2026-11-19 by its coordinator
(the milestone page's item 33, `docs/roadmap/milestones/M-rebel-library.md:257-276` at the frozen
commit). **The seats**: the compiler-engineer, the spec-warden, the historian (advisory) and the
llm-ergonomist (the blind seat: 50 graded paid sessions and one verdict session, 7.081615 USD of the 10
the author approved for the sitting, `run.log`). **The ffi-pragmatist did not sit**: no question moves
what crosses the C boundary (`252-briefs/00-shared.md:17-23`, with what was searched). The completeness
critic read the briefs before any seat (14:54 to 15:14) and the reports after (17:34 to 18:08). The
frozen commit: `181555e9a32d0b18878f9f10e48d2eadd939c285` of `rebel-round`, its stage 2 `sources
23c7d2a626b29fd5`. Briefs: `docs/panel/252-briefs/`; reports: `docs/panel/252-reports/`
(`critic-briefs.md`, `spec-warden.md`, `compiler-engineer.md`, `historian.md`, `llm-ergonomist.md`,
`critic-reports.md`).

**Status: ratified as proposed.** The author's word, given at about 13:10 on 2026-11-19, meant as: *OK
to the ratifications as proposed*, of sittings A, B and C of M-rebel-library. It was given before this
sitting's briefs were written (the writer's first clock reading 14:21, `00-shared.md:648`), so this
sitting's resolutions are ratified in the words this file proposes them, by that advance word, recorded
as given (CL-058), and its decision issue is opened and closed together (`.claude/rules/records.md` § The
issues). The resolutions are the coordinator's, written below as decided.

## The proposal, verbatim

The five questions, each with its robust and conservative arms, as `docs/panel/252-briefs/00-shared.md:89-461`
put them to the seats, copied by script, unedited:

## The five questions

The arms are written from the frozen spec by `bw/tools/make_arms.py` (every edit asserted to match
exactly once) into `bw/arms/`, each diff beside it, `spec-now.md` `cmp`-equal to the frozen spec, and
counted by `./heroes-next measure ../arms/spec-<arm>.md` run from `bw/tree/` (`bw/measure-arms.txt`):
**the vendored maximum, a lower bound until the spec-warden's refreshes**. The frozen spec reads
`claude-legacy 8551`, `cl100k_base 8702`, `maximum 8702`, and the pin `real 11384` (*claude-opus-5,
2026-11-15*, carried: `measure` prints it from `selfhost/measure/pinned.hero`, no refresh ran).

| arm | what it is | sha256, first 16 | lines | maximum | delta |
|---|---|---|---|---|---|
| `now` | the frozen spec | `1d532d514831aa70` | 492 | 8702 | 0 |
| `q1-c` | Q1's conservative sentence (§ 1) | `8448edd80708be52` | 494 | 8728 | +26 |
| `q1-a` | Q1's alias route: panel 135's form and one sentence (§ 4), **added** after the critic's finding 7 | `f82f959ec30b2479` | 495 | 8756 | +54 |
| `q2-c` | Q2's conservative sentence (§ 9), **repaired** after the critic's finding 1 | `4313f5f3bd0efd53` | 492 | 8716 | +14 |
| `q3-r` | Q3's robust: records and variants take type parameters, its example `Pair<A, B>` (§ 9, § 3, § 4 productions), **repaired** after the critic's finding 2 | `dfad79aa38933f85` | 495 | 8765 | +63 |
| `q4-r` | Q4's robust: a `u8` refused in a hole and in `print` (§ 2, § 11) | `1fddf579d07fd044` | 493 | 8758 | +56 |
| `q4-r2` | Q4's narrower robust: an index into a `str` refused in a hole or `print` (§ 2), **added** after the critic's finding 10 | `13337dd1fed04b0b` | 493 | 8754 | +52 |
| `q4-c` | Q4's conservative sentence (§ 2) | `aad99aafd0d1a2ca` | 492 | 8719 | +17 |
| `q5-r` | Q5's robust: a float literal may take an exponent (§ 2), **repaired** after the critic's finding 3 | `7beb56374de59fc2` | 494 | 8743 | +41 |
| `q5-c` | Q5's conservative: no exponent said, and § 11's reader named (§ 2, § 11) | `7dab16286fad68ad` | 493 | 8737 | +35 |
| `q5-p` | Q5's other robust direction: `print` writes no exponent (§ 11) | `d2e79efdb7999586` | 492 | 8703 | +1 |
| `q5-u` | a float's `_` stated (§ 2), **added** after the critic's finding 9 | `2027d86f05837b83` | 493 | 8716 | +14 |
| `proposal` | `q3-r`, `q4-r` and `q5-r` together (Q1's and Q2's robust arms are 0 tokens) | `c7885b36d17a3bd0` | 498 | 8862 | +160 |

The first arms are kept in `bw/arms.v1/` and the script that made them in `bw/tools/make_arms.v1.py`.

### Question 1: a private module that cannot use its own face

**The rule that refuses it**: **`error[module_cycle]`**, `selfhost/modules.hero:280-334` (`cycles`, the
message at 324-325, its note at 328), stated in **design.md §4.1** (*"modules may not form a cycle
(corrected 2026-11-06, panel 234's R11)"*, `docs/design.md:942-943`) and **in no sentence of the spec**
(`grep -n -i cycle spec/heroes-spec.md` prints nothing). Measured in `bw/q1/cycle/` (a face `geom.hero`
declaring `record Point` and using `geom/internal/calc`, whose `square_sum(p: geom.Point)` uses `geom`):

```
error[module_cycle]: modules may not form a cycle, and this `use` closes one: geom uses geom/internal/calc uses geom
  at geom/internal/calc.hero:1:5
  note: move what both modules need into a third one that neither uses
```

**The note's route, followed for a face, meets a second refusal** (`bw/q1/third/`): the type moved into
`geom/internal/types.hero`, the face and `calc` both using it, builds and runs (`main.hero` holds the value
by inference and prints `3` and `25`), but a program that names the type in a signature,
`use geom/internal/types`, is **`error[private_module]`** (*"`geom/internal/types` is private to `geom`
... name what `geom` offers instead"*, `main2.hero`, exit 1). **A route nobody in Rebel took, measured**
(`bw/q1/public/`): the type in a public `shapes/types.hero` beside the face, used by the face, by its
private module and by the program (`use shapes/types`, `types.Rect`): builds, prints `26`, exit 0; it
breaks Rebel's own rule that *"every name a program writes is declared in"* the face
(`examples-rebel/README.md:189-191`), not the language's.

**The record.** Panel 031's **R6**: *"module cycles are refused, with no spec sentence ... It is the
reversible direction: relaxing later breaks nothing, while tightening at M9 — where the per-module cache
needs a topological order — breaks the port"* (`docs/panel/031-modules-the-namespace.md:193-198`; decision
210, closed 2026-08-11). **Decision 523** (2026-08-16): CLAUDE.md §11's ~300 lines yield where the
language forbids the seam, *"The language rule is the one that stands"*; the compiler's own knots are one
module each (`selfhost/ir/flatten.hero` 1,414 lines, `selfhost/grammar_expr.hero` 1,767, `wc -l`; the
comments resting on the refusal, the eight `grep -rn module_cycle selfhost` prints: `selfhost/cli/digest.hero:6`,
`parse/decl.hero:22-30`, `ir/flatten.hero:14-19`, `parse/use_line.hero:21`, `emit/probe_reply.hero:17`,
`grammar_expr.hero:8`, `mutate/typo.hero:30`, `mutate/sites.hero:14`; the first brief named five here and
the other three in `compiler-engineer.md`, the critic's finding 11). **Decision 2403** (2026-11-10, *"A face whose type is recursive walks it with loops, since a
walker on the face would be a line of its card"*, `gh issue view 2403`): it names this very rule
(*"a module under `internal/` cannot name the face's type without using the face, a cycle the compiler
refuses"*), was taken with its recommendation *"keep the rule and the loops"* and **ratified by the
author's reading the same day**, over `core` 0.2.0's one package. **Panel 234's R3 item 2**: *"a face
module declares only what a program calls"* (`docs/panel/234-*.md:316-319`). Panel 141 refused
declaration visibility (decision 1166). **The facts those rulings did not have**: Rebel's witnesses below
(thirty-odd packages, against 2403's one), and the reading that the per-module key is *"the TU's own
emitted text"* (`selfhost/cli/units.hero:7-9`), not a composition in topological order, which may have
retired R6's reason (a reading, not a measurement: the compiler-engineer's).

**The witnesses, by command** (`python3 -I bw/tools/q1_census.py bw/tree/packages/rebel/0.1.0/files`,
the patterns in the script, `bw/q1-census2.txt`): **34 packages, 47 lines**. Of them, `websocket`
(`internal/fixture.hero:3`) is about threads and not this question, and `time` (`internal/zone_files.hero:6`,
`time.ledger:500`, *"It reads no file through `fs`, whose package uses `time`: the compiler refuses the
cycle"*) is the shape beside it, a cycle between two packages. **The other 32**, each with what it did
instead (read at the cited line):

| what the package did instead | packages, file:line |
|---|---|
| a mirror of the face's types in a private module, **field for field**, converted at the face's edge | `amqp`, `http2`, `kafka`, `mqtt`, `nats` (each `internal/model.hero:1-2`), `dns/internal/model.hero:2`, `http/internal/form_parts.hero:19`, `:34`, `multipart.hero:8`, `wire.hero:20`, `jose/internal/jwk.hero:5`, `:12` (keys passed as their JSON), `url/internal/parts.hero:1-3`, `json/patch/internal/ops.hero:3`, `json/schema/internal/model.hero:4`, `qr/internal/bits.hero:6`, `image/internal/layout.hero:2`, `chunks/internal/meter.hero:3`, `log/internal/piece.hero:3`, `html/sanitize/internal/rules.hero:4`, `fs/watch/internal/taking.hero:3`, `math/internal/complex_arithmetic.hero:7` (a complex number as a `Pair`), `asn1/internal/header.hero:4` (a class as a number) |
| **the codes written twice**, once in a private module and again in the face | `archive`, `compress`, `pdf` (each `internal/codes.hero:2`), `keyring/internal/failure.hero:4` (*"a test holds the two to one spelling"*), `unicode/encodings/internal/codes.hero:2`, `search/internal/codes.hero:2`, `vectors/internal/codes.hero:2`, `math/internal/limits.hero:2-3` |
| a list of events the face turns into its value and back | `json`, `cbor`, `msgpack` (each `internal/event.hero:5-7`, *"a module the face uses may not use the face"*) |
| private modules that name no type of the face | `graphql/internal/model.hero:5`, `jsonrpc/internal/wire.hero:4`, `os/internal/counters.hero:5` |

The census's vocabulary is its claim: a package that met the rule and said it in other words is not in
it. Two more mirrors the patterns miss, read: `xlsx/internal/model.hero:1-4` (*"so that reading and
writing need nothing of the face"*) and `scanner/internal/config.hero:1-3`. **Of the 144 ledgers**
(`find -name '*.ledger' | wc -l`), only `time.ledger:500` names a cycle; the rule lives in the sources'
comments. **Carried from the coordinator's notes** (`.claude/worktrees/rebel-scratch/coordinator-todo.md`
lines 103, 120, 220, 263, 387-388, 413, 481, 623-624, 635, 690): the same question named as each package
landed, and two sizes laid on it: `sql/build`'s face 1,629 lines and `math/linear`'s 2,322 (by `wc -l`
today **1,629** and **2,330**; that the cycle is why is the notes' claim, unrun). Every package agent's
brief carried it as *"a known limit, under question"* (46 files of `queue/*.md`, `grep -l -F 'A
private module cannot `use` its own face'`, among them `queue/80-repairs-6.md:4`), pointing at README § A2.

**Shapes beside it**: the cycle between two packages (`time` and `fs`); a face's helper over its own
type, which would be a line of the card (decision 2403); the compiler's own knots (decision 523); and the
note's route ending in `private_module` (above).

**What the probe compiler says, unrun as a route**: with `cycles` answering at once, `bw/q1/cycle/`'s
ring checks (exit 0), builds by units and prints `25`; `--emit-c` writes one 11,830-byte `.c`, exit 0; an
edit to `calc.hero` rebuilt warm prints `26`; a `test` across the ring passes (`1 test, all passed`). **Not
run under it**: the compiler's own tests, the net, a ring holding generics, constants, a record held by
value across two modules (`selfhost/check/sized.hero`'s cycle), `extern` groups in two modules of a ring,
`heroes doc`, the cards, a package's placement walk (panel 250's), the formatter.

**Robust, Q1-R: modules may form a cycle.** `error[module_cycle]` leaves the language; **0 spec tokens**
(the spec never stated the rule); design.md §4.1's clause and decision 2403's loops go; Rebel's mirrors,
twice-written codes and event lists become a choice, not a constraint. It compels: the compiler-engineer's
census of what rests on acyclicity, the cache's key under a ring, the order of a ring's `extern` headers,
the compiler's own knots allowed to split. **A route nobody had listed, Q1-A (the critic's finding 7): the
face names its private module's type as its own**, so `geom.Point` is written by the program while the
record lives in `geom/internal/types` and no module uses one that uses it: a re-export, or a transparent
alias in the face. Arm `q1-a`, **+54** (panel 135's settled form, `"alias" ident "=" Type NEWLINE` in § 4's
`Declaration`, with one sentence: *"`alias Point = types.Point` gives a type a second name, one type
everywhere, its construction and `::` included, so a face may name its private module's type as its
own."*). Its record: **panel 031's R4**, *"Every module you name needs its own `use`" ... "There is no
re-export"* (`docs/panel/031-modules-the-namespace.md:178-182`); **panel 135** deferred design.md Part 7
item 5's `alias` again, ratified 2026-09-13, with the form's settled shape (*"transparent, and *everywhere*
reaches the constructor and `::`"*, `:144`) and three return conditions, (b) being *"a commit in `selfhost/` whose diff
changes one structural type spelling at **8 or more sites**"* (`docs/panel/135-*.md:124-165`, `:156-165`,
`:217-221`), and its account of Go: *"1.9's warrant was refactoring, not brevity"* (`:136`). A form, so
Principle 0's ticket; whether Rebel's mirrors meet a return condition of 135 is **unrun**. **A narrower
robust, Q1-R2**: a ring allowed only through a
module and the modules under its own `internal/` (a face and its private modules), every other cycle still
`module_cycle`; it would owe a spec sentence, since the narrowing is a rule a reader meets, unpriced.
**Conservative, Q1-C: the refusal stands** (decision 2403 kept), and § 1 states it, arm `q1-c`, **+26**:
*"Modules may not form a cycle, so a module under `internal` cannot use the module that names it."*;
and the note stops sending a face into `private_module` (a message's words, no class). **Status quo with
no sentence** is panel 031's R6 as ruled, and the blind seat's `now` cell reads it.

### Question 2: a type parameter not taken from a map literal's key (spec § 9)

**The spec's sentence** (§ 9, lines 305-308): *"A type parameter takes its type from the arguments that
have one of their own, else from the type the context asks for, else from a generic call or a literal
among them (section 2); `ok(...)`, `[]` and a case name give none, and a call that says none of these is
an error."* **design.md §4.12** (`docs/design.md:1976-1996`, panels 105 and 203): an argument with no type
of its own, *"an array or map literal holding one"*, waits, then *"the waiting argument whose type owes
least to a literal's default"*.

**The witness**, carried from `coordinator-todo.md:494-496` (*"`count({"a": []})` with
`count<K>(graph: {K: [K]})` refused cannot_infer"*; the program it names,
`rebel-scratch/defects/infer-map-key-literal.hero`, **does not exist**, `ls`), reproduced and widened in
`bw/q2/` (`heroes-next check`, each `.out` beside its `.hero`):

| program | parameter | argument | verdict |
|---|---|---|---|
| `a-witness` | `{K: [K]}` | `{"a": []}` | **refused** |
| `k-emptymap-value` | `{K: {str: i64}}` | `{"a": {}}` | **refused**, `K` standing only in the key |
| `o-ok-value` | `{K: K?}` | `{"a": ok("b")}` | **refused** |
| `g-intkey` | `{K: [K]}` | `{1: []}` | **refused** |
| `i-value-empty-map` | `{K: [V]}` | `{"a": []}` | refused, naming `K` and `V`; `V` is truly unsaid |
| `l-nested-array` | `[{K: [K]}]` | `[{"a": []}]` | **refused** |
| `e-array` | `[[K]]` | `[[]]` | refused, rightly: nothing says `K` |
| `b-keyvalue`, `c-keyonly`, `d-keyvalue-same`, `n-two-entries`, `j-record-arg` | `{K: V}`, `{K: i64}`, `{K: [K]}`, `{K: [K]}`, `{K: bool}` | `{"a": 1}`, `{"a": 1}`, `{"a": ["b"]}`, `{"a": ["b"], "b": []}`, `{"a": true}` | accepted (`b`, `d` run: `1`) |
| `m-second-arg` | `{K: [K]}` and `extra: K` | `{"a": []}`, `"b"` | accepted |
| `h-annotated` | `{K: [K]}` | a bound `{str: [str]}` | accepted, runs `1` |
| `f-array-lit` | `[K]` | `["a"]` | accepted |
| **`p-reversed`** | `{K: [K]}` | `{"a": [], "b": ["c"]}` | **refused**, where `n`'s `{"a": ["b"], "b": []}` is accepted |
| **`q-array-mixed`** | `[[K]]` | `[[], ["a"]]` | **refused**, where `s`'s `[["a"], []]` is accepted |
| `r-keyonly-typedvalue`, `s-array-mixed-first`, `t-two-typed` | `{K: [str]}`, `[[K]]`, `{K: i64}` | `{"a": ["x"]}`, `[["a"], []]`, `{"a": 1, "b": 2}` | accepted |

**20 programs** (`ls bw/q2/*.hero | wc -l`; the first brief said *sixteen* and held 15, the critic's count),
every verdict by `heroes-next check`, its exit captured (`bw/q2/*.out`): 11 accepted, 9 refused. **The rule
the compiler follows, read from them**: a container literal is typed from its **first** element or value;
when that one gives no type, nothing of the literal is read, its keys included. So the refusal **depends on
order inside one literal** (`p` against `n`, `q` against `s`), a shape panel 203 ruled out across a call's
arguments (*"a pair's verdict is the same in either order"*, `docs/design.md:1976-1996`) and never measured
inside one literal.

The message, whole (`a-witness.out`): *"`[]` takes its type from the type its position asks for, and
`graph` of `count` asks for `{K: [K]}`: **no other argument and no context says what `K` is**, and the
literal written there takes its type from this value"*, `fix (guess): bind the argument first, `v: <type> =
{"a": []}`, and pass `v``. **The key `"a"` says what `K` is**, so the sentence is false of `a`, `k`, `o`,
`l`, and of `p` and `q`, where another entry says it too. The rule: `selfhost/check/generic_argument.hero:285` (the message), `check/waiting.hero` (236 lines,
panel 203's waiting arguments), `check/walk.hero`.

**The record.** Panel 203's critic measured order across two arguments (`first(a: [1], b: [])`,
`docs/panel/203-*.md:52-56`) and nothing names a key inside one literal (`grep` of `203-*.md`,
`203-reports/`, `203-briefs/`, `105-*.md`, `201-*.md` for a map's key and a letter). Issues searched
(`gh issue list --state all --search` for *cannot_infer map*, *map literal key*, *literal among them*,
*cannot_infer*, *type parameter key literal*, `bw/gh/q2-search.txt`): the neighbours are defects 415 (a map
literal's entries never take its annotation's type), 488, 552 and 578, all closed; **none names this
shape**. **What binds**: CLAUDE.md § 12, *"Spec beats compiler: the compiler has the bug"*, unless the
sitting amends the sentence.

**Robust, Q2-R: the compiler has the bug.** A waiting literal's parts that have a type of their own (a
key, any entry, in any order) settle its letters before the forms that refuse themselves are read, so `a`,
`g`, `k`, `l`, `o`, `p` and `q` are accepted, the verdict the same in either order, and `i` is told for `V`
alone; **0 spec tokens**; filed as a `blocking` defect (a correct program refused, a false message, an
order-dependent verdict), repaired in the checker. **Conservative, Q2-C: the spec follows the compiler**,
arm `q2-c`, **+14**, **repaired** (the critic's finding 1: its first wording, *"nor does a literal holding
one"*, was false of task 2's own line 6, which holds a `[]` and is accepted): *"`ok(...)`, `[]`, `{}` and a
case name give none, nor does a literal whose first element or value gives none,"*, true of all 20
programs; and the message stops saying *"no other argument ... says"* where a key does. Q2-C states an
order-dependence in the specification, which the spec-warden judges. **Not convened beyond its seats if the blind reading settles it**: a reading where readers of `now`
predict the program accepted (task 2) is the spec reading as Q2-R says, and the question becomes a
defect with no sentence.

### Question 3: records with no type parameter (`containers`)

**The rule**: spec § 9, *"Generics: on functions only"* (line 303); design.md §4.12, *"Generics on
functions only, not on types. `T?`, `[T]` and `{K:V}` stay built-in"* (`docs/design.md:2020`), the same
section calling *"`Stack<T>` ... perfectly comprehensible to an LLM"* (`:1972-1973`). The refusal exists
and reads right (`bw/q3/r-record.out`): *"error[expected_end_of_line]: expected the end of the line after
`record Stack`, found `<` — type parameters are written on a function only; its fields go on the lines
below it, one level deeper"*, from `selfhost/parse/members_below.hero:223`; a variant likewise
(`r-variant.out`). **So the question is whether the form enters**, not whether the message is right.
**The record**: no sitting ruled on it by `grep -l -i -E 'generic records?|records? (with|taking) (a
)?type parameters?|Stack<T>|parametric'` over `docs/panel/` (panel 128 cites §4.12 as the rule,
`docs/panel/128-*.md:97-102`); **no Part 6 row** (Part 6 is `docs/design.md:3555-3722` by `grep -n '^##
Part'`; `sed -n 3555,3722p docs/design.md | grep -i generic` prints four lines, the traits row, C's
`_Generic`, Zig's history and §1.7's test, none a generic type; the first brief ran the command over Part
7's range alone and called it Part 6, the critic's finding 8) and no Part 7 item (`sed -n 3723,4284p |
grep -i generic`); no issue by `gh issue list --search` for *generic record*, *record type parameter*,
*generics on types*, *Stack<T>*, *generic types* (`bw/gh/q3-search.txt`). **The record Q3-R extends**, found
by the critic's wider search (12 files, none ruling on a record's type parameter, carried): **panel 029,
*generics by monomorphisation***, whose **R4** refuses polymorphic recursion in the pass (*"`f<T>` calling
`f<[T]>` instantiates forever"*, MLton the precedent, `docs/panel/029-generics-by-monomorphisation.md:110`)
and whose **R5** names an instance by its canonical rendered type (`:180`). A silence on records: a
question, not a ruling.

**What Rebel wrote instead** (`grep -rn -i -E 'generic record|record takes no type parameter|records?
(with|holding) (a |two )?type parameters?|no generic record|generics are on functions only'`, 20 lines,
`bw/q3-census.txt`): five of `containers`' eight subpackages (`ls containers/*.hero`) say in those words that each is **a
shape record of the package beside the program's own list, lent with `@` to every call**
(`containers/deque.hero:10-14`, and `cache`, `indexed`, `ordered`, `trie`; `heap` takes the caller's order
at each call, `heap.ledger:10`); `graphs` hands out node numbers (`graphs.ledger:35`); `math/linear` writes a record per
dimension (`math/linear.ledger:25`); **six capabilities refused** for it, `lists`' zip, enumerate,
partition and split-in-two (`lists.ledger:589`, `:596`, `:603`, `:610`), `maps`' entries (`maps.ledger:79`)
and `sets`' take-one (`sets.ledger:191`).

**What the split costs, measured** (`bw/tree/examples-rebel/bw_deque_long.hero` and `bw_deque_short.hero`,
built by `heroes-next` through the catalogue's `path` entry, run 3 times each under `timeout -s KILL 20`):
two queues, and one call handing the jobs' shape the urgent list:
`deque.pop_front(@jobs_queue, @urgent)`. **Both accepted, both exit 0, 3 of 3 each, printing `taken 10`**
where the jobs' front is `1`, stderr empty, the shorter list too (it had grown past the ring's count).
The face promises otherwise: *"A call aborts where a deque is given an item list that is not its own"*
(`containers/deque.hero:17-18`); the ring checks only that the list is not shorter than it says
(`deque/internal/ring.hero:35-37`). **A false sentence of a package's face, measured**: a defect for the
coordinator to file (the writer files nothing). The same promise stands in `cache.hero:109`,
`indexed.hero:20`, `:75`, `ordered.hero:35`, unrun. A `Deque<T>` holding its items is a program in which
that call cannot be written.

**Robust, Q3-R: records and variants take type parameters**, inferred at a construction as a call's are,
written in a type as `Pair<i64, str>`, monomorphised as functions are; arm `q3-r`, **+63**, **its example
repaired** (decision (c), the critic's finding 2: the first arm's example, *"`record Ring<T>` holding
`items: [T]`"*, was task 3's own answer; the arm now reads *"`record Pair<A, B>` holding `first: A` and
`second: B` is written `Pair<i64, str>` in a type"*, a word neither task 3 nor its reference holds, `grep
-c -i pair`: 0) (§ 9's sentence; § 3's `Prefix` gains `[ "<" Type { "," Type } ">" ]`; § 4's
`Declaration` gains `[ Generics ]` on `record` and `variant`). **The shapes beside it** (the critic's finding
8): a record holding itself at another instance, `record Nest<T>` with `inner: [Nest<[T]>]`, instantiates
without end as panel 029's `f<[T]>` did; and a recursive generic variant, `variant List<T>` with a case
holding a `List<T>`. Q3-R's price owes both, each refused at its declaration or built, never a hang or a C
stack overflow (design.md §1.12). It compels: the parser, the type table, inference at construction and in `match`, the
monomorphiser over types, the mangler, the formatter, `heroes grammar`, the cards and `heroes doc`; panel
251's measure of one new type form, *"288 `match` statements incomplete in 109 files"* for `(thread T)`
(`docs/panel/251-*.md:143`, carried), is the nearest price. **Conservative, Q3-C: functions only**, 0
tokens, and `containers`' false promise repaired in the package; **by what mechanism is a question,
unrun** (the critic's finding 8): a `Deque` record and the program's `[T]` share no identity, values being
copies (spec § 3, lines 97-98), so whether any check can make `deque.pop_front(@jobs_queue, @urgent)` abort
is open; if none can, Q3-C's repair is to strike the promise and leave the silent wrong program standing. Principle 0 binds (CLAUDE.md § 2): the
compiler-engineer answers whether the compiler needs it, the blind seat's task 3 whether a reader does.

### Question 4: a byte in `f"..."` written as its number, with no word from the compiler

**The spec** (§ 2, lines 73-74): *"An `f` before a literal's opening quote makes `{e}` write that value as
`to_str` does"*; § 10, *"`s[i]` yields a `u8`"* (line 318); § 2, *"A character literal is an integer"*
(line 68). The behaviour follows the text; the question is whether a plausible mistake is allowed to be
silent. **Measured** (`bw/q4/bytes.hero`, `heroes-next check` exit 0 with 0 bytes of output, built, run
exit 0, stderr 0 bytes):

```
f"[{s[0]}]"         [117]        s = "use"
print(s[0])         117
to_str(s[0])        117
f"[{b}]"            [65]         b: u8 = 65
f"[{c}]"            [97]         c = 'a', an i64
f"[{s.slice(from: 0, to: 1)}]"   [u]
```

**The witness**: `examples-rebel/README.md:491-493`, *"A byte in an `f"..."` writes its number ... `f"{s[0]}"`
writes `117` (run 2026-11-16)"*, and the coordinator's notes, which record it as a
model's trap, *"`f"{c}"` with c: u8 writes the number (117), a silent trap"* (`coordinator-todo.md:233`,
`:649`, carried). Which Rebel lines still hold a `u8` in a hole or a `print` is
**unrun** (a grep cannot type a hole: the census of `compiler-engineer.md` § The census each question may need).
**The rule**: `selfhost/check/interp.hero:26-31` admits `.int_ty | .float_ty | .bool | .str` (panel 121's
R1 and R4); `print`'s at `selfhost/check/builtins.hero:176-190`. **The record**: no sitting names a byte in
a hole (`grep` of `docs/panel/` and its reports: two hits, both array indices), and **panel 061 measured the
same printing** and ruled on another sentence: *"`b: u8 @ 'a'` prints 97, `c: i32 @ 'z'` prints 122"*, its
ruling the spec's false *"Character literals are `i64`"*, repaired at 0 tokens, never the printing
(`docs/panel/061-the-fields-nobody-named.md:279-284`, the critic's finding 10; decision 512 followed);
issues searched for
*byte interpolation*, *u8 f-string*, *writes its number*, *character literal integer*, *to_str u8*,
*interpolation hole* (`bw/gh/q4-search.txt`): decision 512 put *"A character literal is an integer"* into
the spec at 0 tokens; none names this trap.

**Robust, Q4-R: a `u8` is refused where a value becomes text implicitly**, an `f"..."` hole and a `print`
argument, a new diagnostic class, its fixes guessing the two meanings: `s.slice(from: i, to: i + 1)` for
the character, `to_str(b)` for the number; arm `q4-r`, **+56** (§ 2 and § 11). It compels: the census of
correct programs that print a `u8` as a number (each gains `to_str`), the class and its goldens. **A narrower robust arm nobody had listed, Q4-R2** (the critic's
finding 10): refuse a hole or a `print` argument whose expression is itself an index into a `str`
(`s[i]`, `word[0]`), the trap's own form, so a `u8` bound by name (`b: u8 = 65`, `[65]` above) still prints
its number and no correct program pays a `to_str`; it misses `c = s[0]` then `f"{c}"`, a cost to state beside
Q4-R's census; arm `q4-r2`, **+52**, one sentence in § 2: *"A hole or a `print` argument that indexes a
`str` is refused: `s.slice(from: i, to: i + 1)` writes its character, `to_str(s[i])` its number."*. **The
shape beside it, not in the arm**: `c = 'a'` is an `i64` and `f"{c}"` writes `97`; a character literal
defaulting to `u8` would bring it under Q4-R (a § 2 sentence, unpriced). **Conservative, Q4-C**: the
compiler stays; § 2 says *"A `u8`, `s[i]` included, writes its number."*, arm `q4-c`, **+17**.

### Question 5: `print` writing `1e-06` where a literal with an exponent is refused

**Measured** (`bw/q5/prints.hero`, built, run exit 0): `print(0.000001)` writes `1e-06`; `0.0001`
writes `0.0001`; `0.00001` writes `1e-05`; `100000000000000000000.0` writes `1e+20`; `123456789012345678.0`
writes `1.2345678901234568e+17`; `-0.0000025` writes `-2.5e-06`; an `f32` of `0.000001` writes `1e-06`;
`f"{0.000001}"` and `to_str(0.000001)` write `1e-06`. **And every exponent literal is refused**
(`lit-*.out`): `1e-06`, `1.0e-6`, `1e21`, `2.5E3` each
*"error[exponent_literal]: `1e-06` is not a number in this language — there are no exponents. Write the
digits out, or compute it: `1 * pow(base: 10.0, exponent: …)` through the FFI (spec § 13)"*, from
`selfhost/number.hero:146-172`.

**The spec's sentences**: § 11, *"A float prints a point or exponent (`1.0`, `1e-06`), or `inf`, `-inf`,
`nan`, and reads back as the same value"* (lines 350-351); **§ 2 says nothing of a float literal's form**
(no sentence of § 2 names a float, `sed -n 63,79p`). *"Reads back"* is C's `strtod`, by design.md §4.9's
*"`snprintf("%.*g")` at increasing precision until `strtod` reads back the same bits"*
(`docs/design.md:1752-1755`, panel 021; `runtime/parts/f64.c:26-27`), which the spec's sentence does not
name. **The record**: panel 035's **R6**, *"exponent literals are refused (+23), and the refusal is made
safe. Principle 0's burden is unmet: a new syntactic form for a type not on the closure list, with no Part
11 evidence"* (`docs/panel/035-the-specs-accumulated-silences.md:175-179`); decision 259 (2026-08-12) made
the refusal *"honest"*; the spec gained no sentence for it (`git log -S'exponent' -- spec/heroes-spec.md`
prints one commit, `d06b22669`, panel 021's). **The sentence's own record**: *"reads back as the same
value"* entered by panel 121's R6, *"the printed float's round-trip sentence (+9)"*
(`docs/panel/121-the-brace-was-already-taken.md:291-293`; decision 858, closed 2026-09-09, *"what a
printed float guarantees, which the spec does not say"*), over panel 081's print sentence (decision 595);
neither line weighs the literal refusal (read). Beside it: decision 618 decided nothing for `5.` (*"a shape nobody
writes"*), which is refused today as a line ending in `.` (`bw/q5/five-dot.hero`,
`error[continuation_outside_brackets]`); defect 2391 (closed) met `exponent_literal` inside a `use` path; **defect 2276** (filed
2026-10-29, closed 2026-10-30) is a written-out `f32` literal, `0.00000000000000000000000007038531`, reaching C as the wrong
binary32 (`gh issue view`, `bw/gh/q5-issues.txt`; issues searched for *exponent literal*,
*exponent_literal*, *exponent*, *1e-06*, *scientific notation*, *float literal*, `bw/gh/q5-search.txt`).
**The fact panel 035's R6 did not have**: Rebel's float literals written
out (`grep -rn -o -E '(^|[^0-9A-Za-z_."])(0\.0{5,}[0-9]+|[1-9][0-9]{15,}\.[0-9]+)' --include='*.hero'`,
`url/idna`'s string data left out by hand, `bw/q5-longlits3.txt`): **429 literals in 88 files**, **96 over
100 characters, 81 over 300** (`math/internal/complex_hyperbolic_cases.hero:82`, 311 and 312 characters),
each one where a dropped zero is a different number the compiler accepts (by construction: any run of
digits with a point is a literal). In `selfhost/`, `examples/` and
`tests/`, the unrefined pattern prints 719 lines (unread one by one). **And the hazard has a mitigation
the first brief did not weigh** (the critic's finding 9): **a float takes `_` today**, though spec § 2
grants it to *"One `i64`"* alone (line 66): `tol = 0.000_000_001` and `big = 1_000_000.5`, built and run,
print `1e-09` and `1000000.5`, exit 0 (`bw/q5/float-sep.hero`); and `.5` and `5.` are refused
(`bw/q5/dot-five.out`, `error[expected_case_name]`; `five-dot.out`, `error[continuation_outside_brackets]`).

**Robust, Q5-R: a float literal may take an exponent**, so what `print` writes reads back in the
language; arm `q5-r`, **+41**, **repaired** (the critic's finding 3: its first sentence, *"A float has
digits on both sides of its point and may end with an exponent: `2.5`, `1e-06`, `2.5E3`"*, gave as a
float `1e-06`, which has no point): § 2 gains *"A float is digits, a point and digits, or digits with an
exponent, or both: `2.5`, `1e-06`, `2.5E3`."*, which also states a float's form for the first time; the
compiler-engineer's build says what `1e21` and `1e400` become. It compels:
the lexer (`number.hero`'s refusal becomes a reader), the formatter's canonical spelling of an exponent,
`heroes grammar`, `exponent_literal` leaving (a class), the emitter unchanged (`%a`). **The other robust
direction, Q5-P**: `print` writes no exponent, arm `q5-p`, **+1**; it moves every program that prints a
small or large float and writes 311 characters for the largest `f64` and 326 for the smallest subnormal positionally (Python's
`Decimal` of each shortest round-trip form, `python3 -I -c`; what Heroes' ladder would write is unrun),
and leaves the 429 literals as they are; **no blind cell reads it**: it changes the one sentence that
shows a reader an exponent, § 11's *"(`1.0`, `1e-06`)"*, and fits the bound only in place of another
cell (about 0.92 USD for five sessions at panel 251's writing median, the critic's finding 9), a route
priced for the coordinator. **A float's `_` stated, Q5-U** (the critic's route): § 2's *"with `_` between
any two digits"* widened to *"and a number of any type takes `_` between two digits: `0.000_001`"*, arm
`q5-u`, **+14**, beside Q5-R's or Q5-C's form sentence; whether `heroes fmt` keeps a float's `_` is the
compiler-engineer's. **Conservative, Q5-C**: both behaviours stay; § 2 says a float
literal has no exponent and § 11 names `strtod` as what reads the output back, arm `q5-c`, **+35**; and
`exponent_literal`'s *"there are no exponents"* says *"a literal has no exponent"*.

## The verdicts

Each from its seat's report; the blind seat's counts by the rows registered before any run
(`252-briefs/llm-ergonomist.md` § The grading); the critic's re-runs beside them where it ran one. A
*real* token delta is the spec-warden's refresh (`claude-opus-5`, 16:11 to 16:12); a *vendored* one is a
lower bound.

| seat | Q1, a private module and its face | Q2, a map literal's key | Q3, records with type parameters | Q4, a byte in `f"..."` or `print` | Q5, an exponent printed and refused |
|---|---|---|---|---|---|
| compiler-engineer | **approve Q1-R**, built as a deletion (`modules.hero` 450 to 331 lines, `sources 2ea57f890c143853`): of twelve cases the frozen compiler refuses as `module_cycle`, ten build and run and two stay refused by their own checks (`constant_cycle`, `no_size`); a warm rebuild after an edit to each ring member right; nothing reads the module graph in an order. **Veto Q1-A** (a new declaration kind: 322 incomplete matches in 172 files). **Object** to Q1-R2 (about +35 lines and a sentence) and Q1-C | **approve Q2-R**, built, +202 −13 lines in `selfhost/check/`: the brief's 20 programs read 18 accepted and 2 refused rightly; plain literals refused for their order build; two `check` goldens move, both pinning the order; `[1, b]` and `{name(): []}` left refused, priced. **Object** to Q2-C | **object** to Q3-R (one type case: 290 incomplete matches in 111 files by its count, 288 in 109 on the frozen tree by the critic's; over 1,500 lines, an estimate; not built). **Approve Q3-C**: all four `containers` promises false, `cache`, `indexed` and `ordered` writing a key into the other map, exit 0, 3 of 3 | built Q4-R (`byte_as_text`, +103 lines); census: 226 sites in 98 files, 4 of Rebel's 69 meaning the character. **Object** to Q4-R and Q4-R2 (it catches 1 of the 4). **Approve Q4-C**, the 4 filed against the package | **approve Q5-R**, built, +29 −17 in `number.hero` (`1e21` a float; `E` refused by a new `exponent_case` with a `certain` fix; `1e400` prints `inf` in silence), with a range refusal (about +20, unbuilt) as part of it. **Object** to Q5-P and Q5-C; Q5-U a sentence at no compiler cost |
| spec-warden | **approve Q1-R** (0 tokens) on the compiler-engineer's census, a condition met after it was written (critic finding 12). **Object** to Q1-C: false (its `q1-sibling` has a module under `internal` use the face, and it builds); +33 real. **Veto Q1-A** (Principle 0; +71 real). **Object** to Q1-R2 | **approve Q2-R** (0), its repair widened to every container literal. **Veto Q2-C** (+21 real: an order-dependent refusal no program needs) | **veto Q3-R** (+86 real) unless task 3's rule fires or a compiler need is measured. **Approve Q3-C**, the package's promise repaired at once | **approve the refusal of every `u8`** in a hole and in `print`, as robustness (rank 3); **object** to `q4-r`'s text (+62 real, one rule in two homes; `w4-min`, +14 vendored, in § 11 instead), to Q4-R2 (+60 real) and to Q4-C (+19 real) | **approve naming `strtod`** in § 11 in every route (`w5-strtod`, +7 vendored); approve a sentence giving a float's form; **Q5-R if W9 fires** (+49 real), else Q5-C's form with `_` (`w5-c-u`, +45 vendored); **object** to Q5-P (0 real) |
| historian (advisory) | **approve Q1-R over Q1-C**, the departure from Go named: Pike's paper calls entanglement *"more important"* than incremental builds, and Go declined Q1-R (issue 30247) and Q1-R2's exact shape (issue 31833); Q1-A has precedent (Rust's `pub use`, Go's proposal 16339) and a documentation cost Go recorded (issue 44905) | **approve Q2-R, object to Q2-C**: rustc typed an array's letter from its first element and removed it as *"accidental"* (PR 140283); C#, C++ and TypeScript read every element | **approve Q3-R as the direction** (Go's 43651, *"we also want generic types"*; Dart 1.21; Go 1.27's generic methods); Go refuses the nested shape at its declaration; no position on Principle 0's ticket | **approve Q4-C; object to Q4-R and Q4-R2 on precedent**: no compiler read refuses a byte where it becomes text; Rust and Swift close the trap at the index | **approve Q5-R with Java's range refusal** (JLS § 3.10.2) and Q5-U; **object** to Q5-P and to Q5-C's *"no exponent"* half |
| llm-ergonomist (blind) | task 1: `now` 5 of 5 under the frozen stage 2 and 5 of 5 under the probe; `q1-c` 5 of 5; **no reader formed the ring** (0 of 10): neither rule fires | task 2: `now` 0 of 5 by the row, **all five predicting the build Q2-R gives**; `q2-c` 5 of 5 by the row's quote (2 of 5 by the line number alone): both rules fire | task 3: `now` 5, `q3-r` 5 by the rubric: the rule does not fire; **5 of 5 `now` answers accept a mismatched pair** (recorded, not a gate) | task 4: `now` 5, `q4-r` 5; silent wrong 0 of 5: neither rule fires | task 5: `now` 4 of 5 under the frozen stage 2, 5 regraded; `q5-c` 5; 1 of 5 writes an exponent: no rule fires |

**No budget veto**: the largest arm, `proposal`, reads 11,581 real, and every arm together 11,842 (11,902
with the FFI floor), against design.md §1.6's 16,384 (the spec-warden's refreshes). **The verdict
session**, over task 3 under the `proposal` arm, said *"approve"*, recorded and grading nothing.

## The critic's second reading, what it changed

Twelve findings (`critic-reports.md`), each settled by a command where one could settle it:

1. **The cells grade as reported.** Task 2's `now` regrade is a built fact: the critic rebuilt the
   compiler-engineer's routes (`sources 679e4c547fb98958`, its `ce-all2`'s key) and task 2 prints `2`, `1`,
   as the five `now` readers predicted. `q2-c` reads 5 of 5 by the registered row, which admits the quote,
   and 2 of 5 by the line number alone (three answers counted `task.md`'s lines). **The grader's ring test
   can never fire** (a raw string's `\\b`); the count stands on a `grep` (no `area.hero` holds a `use` line)
   and on the frozen builds, 10 of 10 printing `26`.
2. **The three ceilings.** Task 1 asked for no private function over `Rect`, so all ten readers wrote two
   integers and never met the refusal; task 4 printed its expected first line, `u 3`, and all five `now`
   readers named `word[0]` as a `u8` and turned away (the trap met 5 of 5, and turned by reading); task 5's
   checks pass any tolerance between `5.55e-17` and `1e-3`, so a dropped zero is graded right. **A rule that
   read zero measured the readers' route around a hazard, not the hazard's absence**: W1, W6, W8, W9 and
   W10 may not be cited as evidence that a hazard is absent, and none is below.
3. **Task 3's hazard, run.** The `q3-r` answers, monomorphised by hand, build 10 of 10 halves and print
   `3 4 5` and `green blue`. One mismatched call added to each `now` answer: **5 of 5 compile, 4 of 5 exit 0
   printing a ring of two holding three, one aborts** (`252-critic/work2/t3mix/`). The rule measured
   whether readers can write a working ring under each arm, not whether the program they write can be
   misused.
4. **Q1, one shape further.** A ring between two packages whose catalogues name each other, under the
   compiler-engineer's Q1-R: `check` and `build` exit 0, `41`; an edit rebuilt warm, `61`; restored, `41`. A
   ring through packages fetched by `version` or `git` is unrun. Panel 031's R6 recorded Go's lesser reason:
   the paper's source names incremental builds, then *"More important"*, entanglement (read through `gh`;
   issues 31833 and 30247 read too). No census reaches that reason. Before `1.0.0`, R6's *"reversible
   direction"* buys nothing, CLAUDE.md § 2 making tightening as legal as relaxing.
5. **Q2.** Each arm's readers are right against their own compiler, so neither count ranks Q2-C above
   Q2-R. The built Q2-R covers the plain literals refused for their order (the spec-warden's premise 3 is
   true of the brief's words, false of the built route) and leaves `x07` and `x14`. **Spec § 10's *"an empty
   one needs an annotation"* is the price Q2-R's 0 tokens leaves out**: all five `now` readers weighed it and
   read it as Q2-R needs.
6. **Q3's price on the frozen tree is 288 in 109**, not 290 in 111 (the compiler-engineer's copies carried
   its other routes). `selfhost/` holds one generic function, so the compiler cannot show a generic
   container's need either way; Principle 0's ticket for Q3-R can come only from Part 11. W5 may not be
   cited as the form buying nothing.
7. **Q4.** design.md §1.12 reaches a segfault and a corrupted memory, and a byte's number is neither: the
   compiler-engineer's side is the checkable one, so the refusal rests on Principle 0's ticket, never on
   rank 3. Re-read, Rebel's 69 sites hold **6** meaning the character by the compiler-engineer's own
   criterion (`redis/internal/resp.hero:229` and `qr/internal/bits.hero:94` beside its four) and **up to 15**
   on a wider reading. Reusing `hole_not_renderable` or `bad_operand` needs their words changed, since both
   are false of a `u8`.
8. **Q5.** Spec § 11, `spec/heroes-spec.md:356-357`, says *"Nothing fails to fit a float: too large is
   `inf`"*, so the range refusal is a ruling with a sentence to price, not a defect *"owed either way"*; and
   a literal of `0.`, 400 zeros and a `1` prints `0.0` today, the silent underflow. The spec-warden's
   condition for Q5-R rests on W9, which read a ceiling.
9. **The defects the seats name**, each reproduced in the critic's copy, each the frozen tree's; none
   filed.
10. **Fourteen premises a seat called false, judged**: ten right, one a definition (the census's 1,852
    against 1,924 roots), one half right (the spec-warden's premise 3), one right as a measurement with its
    classification a reading (the silent `inf`), one wrong as stated (the spec-warden's *"no new class is
    owed"*).
11. **A task's reference holding the shape does not show that the task leads a reader into it** (§ How
    the sitting went).
12. **Smaller points**: the compiler-engineer's second census moved 146 roots, not 149; the spec-warden's
    removal list is 25 comment lines, not eight; no blind session had `Write` (43 of 51 said so, and the
    fallback caught all 51); the historian's PDF outside the root is gone; design.md §4.12's Go note is not
    what Go's FAQ says, a correction owed only where a ruling leans on it, and R3 does not.

## Where the seats disagree, stated plainly

- **Q1, Go's reason.** Every judging seat approves Q1-R. The compiler-engineer and the spec-warden rest
  it on compiler facts; the historian holds that Go's main reason, entanglement, is a judgement no census
  answers, and the critic confirms the quotes at their source. **Q1-R2**: the historian finds the densest
  precedent on its line, and Go declined that exact shape; the critic names it the route nearest the line
  Rust, Java and Swift draw, serving 32 of the 34 witnesses; the compiler-engineer and the spec-warden
  object on a sentence's price. Nobody measured it against a reader. **Q1-A**: vetoed by both, with
  precedent by the historian.
- **Q2.** No seat approves Q2-C. The spec-warden registered that Q2-C's sentence would carry (W4) and it
  did, and argued it pays for nothing; the critic finds each arm right on its own compiler.
- **Q3.** The compiler-engineer objects on cost and need, the spec-warden vetoes on Principle 0 unless
  task 3's rule fires, the historian approves the form as the direction precedent points. The blind count,
  5 against 5, is a ceiling; the hazard it recorded is four silent wrong programs of five. **Whether
  design.md Part 11 counts a reader who writes an API whose misuse compiles and runs wrong is a question
  the sitting did not ask** (critic finding 3).
- **Q4, rank.** The spec-warden approves the refusal as robustness, rank 3; the compiler-engineer says
  §1.12 does not reach a byte's number, and the critic's reading of §1.12 and panel 215 sides with it.
  **Q4-C**: the compiler-engineer and the historian approve it, the spec-warden objects (it describes a
  trap the thesis says must be an error). **The class**: the spec-warden's reuse of two classes against the
  compiler-engineer's new one, the critic finding reuse a reword, not free. **The share**: the
  compiler-engineer reads 4 of Rebel's 69 sites as character-meant, the critic 6 to 15, so the
  compiler-engineer's own condition (a share over 10%) crosses or not by corpus and reader.
- **Q5.** The compiler-engineer and the historian approve Q5-R; the spec-warden approves it only if W9
  fires, and W9 read no on a task that could not fire it. **The range**: the compiler-engineer calls the
  silent `inf` a `blocking` defect owed either way; the critic reads § 11:356 as making it a ruling. **Q5-C**:
  the spec-warden takes its form if W9 fails, the compiler-engineer objects, the historian objects to its
  *"no exponent"* half.

## The resolutions

The coordinator's, as decided; robustness (design.md §1.12) decides THAT a measured class of wrong
programs closes, Principle 0 HOW a new form enters, and a sentence of the spec owes a named removal or a
registered prediction (design.md §1.6). **A rule that read zero measured the readers' route around a
hazard, not the hazard's absence** (critic findings 2 and 11), and no resolution cites W1, W6, W8, W9 or
W10 as evidence that a hazard is absent.

### R1, Q1: modules may form a ring; the refusal leaves the language

**Adopted: Q1-R, the refusal lifted**, built by the compiler-engineer as a deletion: `selfhost/modules.hero`
from 450 lines to 331, no line added elsewhere (`ce/routes/q1/q1r.diff`). Ten cases the frozen compiler
refuses as a cycle build and run under it (a face and its private module, a ring of three, mutual
recursion, a generic called across the ring, constants in a chain, records held through a list, a variant,
an `extern` group in each of two modules, a package placed through a catalogue's `path`, a `test` across
the ring); constants and records held by value stay refused by their own checks, `constant_cycle` and
`no_size`; a warm rebuild right after an edit to each member prints the edited value. A ring between two
packages whose catalogues name each other builds and rebuilds right (critic finding 4). Nothing in the
compiler reads the module graph in an order, so **panel 031's R6 reason, incremental builds, no longer
holds**. **Its other reason, entanglement, which Pike's paper calls *"More important"* and which Go's team
gave in declining cycles (issue 30247) and the narrower ring inside one module (issue 31833), is recorded
as weighed and not answered**: the language now lets a program write a ring, and whether rings stay
readable is the author's to watch. **0 spec tokens**: no sentence of the spec states the refusal (panel
031's R6).

**Not adopted.** **Q1-A** (an `alias` in the face) is vetoed by the spec-warden on Principle 0, a re-export
panel 031's R4 refused, and by the compiler-engineer on its price, 322 incomplete matches in 172 files.
**Q1-C's sentence is false** (a module under `internal` may use the face; only a ring is refused, the
spec-warden's probe), and is not adopted; it was the conservative route. **Q1-R2**, a ring only through a
face and its `internal/`, is not adopted, Q1-R allowing every ring it would allow.

**Owed**: the deletion with its goldens and the compiler's tests, in one lane; the `module_cycle` note
that led a face into `private_module` goes with the refusal; a ring between fetched packages, unrun.
**What conservative would have been**: the refusal kept and stated in § 1 by a true sentence (the
spec-warden's `w1-true`, +21 vendored, since Q1-C's is false).

### R2, Q2: the compiler has the bug; a literal's typed parts settle its letter

**Adopted: Q2-R as the compiler-engineer built it**: +202 −13 lines in `selfhost/check/` (a new module,
`waiting_parts.hero`, 122 lines; `waiting_order.hero` +48 −5, `walk.hero` +20 −8, `settling.hero` +12). A
literal's lead is its first element, key or value that does not refuse itself, and a letter nothing
settled takes the type of a part that needs no synthesis, by form and never by position, never reading
inside `ok(…)`. Two `check` goldens move, both pinning the order-dependence (`fixedbugs-402-*:57`,
`fixedbugs-541-*:88`); the census's nine wrongly lifted refusals, inside `ok(…)`, were caught by the
seat's own census and fixed before its last build. **A `blocking` defect**: a correct program refused with
a false message, and every reader of today's spec predicts the build Q2-R gives (5 of 5).

**Its residue stays refused and is filed in the defect**: `[1, b]` with `b: u8` refused where `[b, 1]`
builds, and `{name(): []}` refused with the same false message, each priced by the compiler-engineer and
unbuilt. **Spec § 10's *"an empty one needs an annotation"*** (`spec/heroes-spec.md:320-321`) is checked
against Q2-R by the implementing lane, and amended with its count if Q2-R makes it false (critic finding
5: a cost the arm's 0 tokens left out).

**Not adopted: Q2-C**, vetoed by the spec-warden (an order-dependent refusal no program needs, against
design.md §4.12's order-free rule); its blind count does not rank it above Q2-R, each arm's readers being
right about their own compiler. **What conservative would have been**: Q2-C, the spec following the
compiler (+21 real).

### R3, Q3: a record with a type parameter waits; `containers` keeps its promises

**Q3-R is not adopted**: Principle 0 is unpaid (no registered reading shows the effect: task 3 read 5
against 5, a ceiling, critic findings 3 and 6), the compiler does not need it (two families of records,
seven records), and its cost is 288 incomplete matches in 109 files (the critic's re-count of the
compiler-engineer's 290 in 111, on the frozen `selfhost/` alone). **Recorded as the measured argument for a
reading registered first**: one mismatched call added, the five `now` rings all compile, four print a ring
of two holding three and exit 0, one aborts (`252-critic/work2/t3mix/`); the `q3-r` answers cannot be
written that way, their `add` taking no list.

**Q3-C is adopted**: the four `containers` whose faces promise an abort they do not give (`deque`,
`cache`, `indexed`, `ordered`: a list of another shape handed in runs on, exit 0, 3 of 3 each) are repaired
in the package so each promise holds, a `blocking` defect. **Q3-C fixes `containers` and none of the rings a
program writes itself.** **What robust would have been**: Q3-R (+86 real), refused on Principle 0's burden,
the spec-warden's veto standing on that and not on W5.

### R4, Q4: nothing enters the language; Rebel's own traps are a defect

**Nothing enters the language.** Q4-R and Q4-R2 are not robustness (design.md §1.12; critic finding 7), and
Principle 0 is unpaid: task 4 printed the expected `u 3` in its own text, and all five `now` readers named
`word[0]` as a `u8` and avoided it, a ceiling. The census: 226 places where a byte becomes text, the great
majority meaning the number.

**Rebel's own traps are a defect**, class `adjacent` (a true message less exact than it could be): at
least 6 of Rebel's 69 sites print a byte's number where the character is meant (the Postgres message kinds,
`sql/postgres/internal/backend.hero:155` and `session.hero:168`; the TZif version byte; Punycode;
`redis/internal/resp.hero:229`; `qr/internal/bits.hero:94`), up to 15 on a wider reading (the critic's
re-read), each in error or description text. **Q4-C** (a sentence that a byte prints as its number, +17
vendored, +19 real) has no blind cell and so no payment; it waits with the question, which a task that
leads readers into the shape would answer. **What robust would have been**: Q4-R (+62 real), the
compiler-engineer's `byte_as_text`, 222 correct sites each gaining a `to_str`; **conservative**: Q4-C's
sentence. Neither is adopted.

### R5, Q5: § 11 names `strtod`; exponent literals wait

**Adopted: § 11 names C's `strtod` as the reader that reads a printed float back** (the spec-warden's
`w5-strtod`, +7 vendored, in every route), because the sentence today promises a read-back the language's
own lexer refuses: a truth repair of the spec. The arm, word for word:

```
-exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`, and reads back as the same value; a
+exponent (`1.0`, `1e-06`), or `inf`, `-inf`, `nan`, and C's `strtod` reads it back as the same value; a
```

**Q5-R (exponent literals) is not adopted now**: built by the compiler-engineer (+29 −17 in the lexer),
but Principle 0 is unpaid (task 5 read 1 exponent of 5, a ceiling: its checks pass any tolerance between
about 6e-17 and 1e-3, so a dropped zero is graded right, critic finding 2), and the spec-warden's
condition rests on W9, which read that ceiling. **Recorded as the argument for a reading registered
first**: Rebel holds 429 written-out float literals, 81 of them over 300 characters, where a dropped digit
compiles silently. **Q5-U** (a float takes `_`, true today) and **Q5-P**: no sentence. **The range refusal is
a language ruling, not a defect**: § 11 line 356 says too large is `inf`, and underflow to `0.0` is silent
too (critic finding 8); it is put as a question with Q5-R. **What robust would have been**: Q5-R with the
range refusal; **conservative**: Q5-C whole (+41 real), whose § 11 half this resolution takes.

## The prior art, per resolution

Re-read on 2026-11-19 by this file's drafter, each page fetched again and its own words asked for
verbatim. Of the historian's candidate lines for these resolutions, four were narrowed to what their pages
state (Rust's, to *"wait until LUB coercion is completed"*, not every element's; C#'s and C++'s, to the
conditions their rules carry; Python's, to *"on most systems"*), Nim's was completed with the page's own
*"An import cycle without a cyclic import is deprecated"*, Java's and Haskell's stand as drafted, and
TypeScript's, a precedent the historian marked verified and drafted no line for, was added.

- **R1**: Java's specification, § 7.3 (classes in different compilation units may refer to each other
  circularly); the Haskell 2010 Report, chapter 5 (modules may be mutually recursive); Nim's manual,
  version 2.4.2, § Cyclic imports (cycles allowed, a cycle group checked phase by phase, the undeclared
  cycle deprecated). Go's specification, Pike's paper and Go's issues 30247 and 31833 are what R1 weighs
  against, written in its text; a declined proposal is no prior art. These go into the decision issue.
- **R2**: rustc's PR 140283 (an array element's type variable no longer resolved to the first element's
  type), C# 12's collection expressions (an inference from each element), cppreference's template argument
  deduction (every element of a braced list, a conflict failing) and TypeScript's handbook (the best
  common type considers each element). These go into the decision issue.
- **R3**: none; it adopts no form, and `containers`' repair rests on no precedent the historian read.
- **R4**: none; it adopts nothing.
- **R5**, its `strtod` sentence alone: Python's tutorial, *Floating-Point Arithmetic: Issues and Limitations* (a printed float's
  decimal string chosen to preserve `eval(repr(x)) == x`). The historian found no language that names C's
  `strtod`; Python names its own reader. Go's literal grammar, Java's § 3.10.2 and Go 1.13's letter serve
  Q5-R and the range refusal, which wait. This goes into the decision issue.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the batch that lands Q1-R, `selfhost/modules.hero` reads at most 335 lines, the compiler's own tests change in no assertion but `modules.hero`'s ring case, and the full net reads the same failed count as the frozen tree's | the batch that lands R1 |
| compiler-engineer | at the batch that repairs Q2, the 20 programs of `bw/q2/` read 18 accepted and 2 refused (`e` and `i`), and the census moves no root from accepted to refused by `cannot_infer` or `type_mismatch` | the batch that lands R2 |
| compiler-engineer | if Q3-R lands, its first commit that builds `record Pair<A, B>` end to end touches at least 111 files of `selfhost/` (the frozen tree's count is 109, critic finding 6) | a reopened Q3 |
| compiler-engineer | under Q4-R as built, a census of the frozen tree's roots refuses at least 130 roots that build today, and at most 5% of its distinct sites write a character's number. **This sitting's census**: 132 roots (90 drivers, 42 `run` goldens); the character-meant share 4 of 226 (1.8%) by the seat, 6 (2.7%) by its criterion re-read, up to 15 (6.6%) on the critic's wider reading | a batch that tries Q4-R; none is owed |
| compiler-engineer | Q5-R lands with `selfhost/number.hero` changing by at most +60 lines with its range refusal, no line of `selfhost/emit/`, `selfhost/ir/` or `runtime/` changing, and its census moving the 3 goldens that hold `exponent_literal` and nothing else | a reopened Q5 |
| spec-warden | W1 to W12, registered before the runner started (§ How the sitting went); **scored here**: W2, W3, W4, W5 and W7 hold (W4 by the registered row's quote; by the line number alone it reads no); **W1, W6, W8, W9 and W10 read no**, each on a task that could not fire it as worded (critic finding 2), so not one is evidence that its hazard is absent; W11 (`now` refreshes to 11384) and W12 (`proposal` +197 real, no arm above 11,700) held | scored here |

The historian registers conditions, not predictions; the blind seat's counted rules are the instrument,
not a forecast.

## How the sitting went, for the next

- **The blind seat's leaks, measured shut.** The free preflight held (`run.log`: one session id; the
  attachments `date` and `session_context` alone; git `reader`, clean, no commit; two system blocks; the
  runner's prompt alone; no other folder named; 10 files of the system's temporary folder newer than the
  marker, 0 naming the session or the area). After the run the scan read **51 transcripts, 0 failing a
  check** (`scan exit 0`), and 129 files modified in the temporary folder since the marker, 0 naming a
  session or the area; every `context` answer is no in substance. Panel 251's three leaks (defect 2589) are
  shut on this run. What still reached each session is its folder's path, which the contract keeps.
- **The spec-warden's predictions were registered before the runner.** The gate copied the report at
  16:08:22, before any paid session (35 non-empty lines under the heading, the copy's sha256
  `9621cd1f5c6d32e0...`), and the copy's `## Registered predictions` section is byte-identical to the
  report's today (compared again for this file, `python3 -I`). Panel 251's departure was not needed.
- **The clock rule.** No duration was taken as a measurement while five agents shared the Mac: the
  compiler-engineer priced in instructions retired (`/usr/bin/time -l`), the spec-warden and the critic
  took no clock.
- **The freeze was kept.** `git log --oneline 181555e9a..HEAD -- packages selfhost runtime spec seed` prints
  nothing (run at 18:27); the three commits after the freeze touch `docs/panel/` alone: `f4e3594d5` (14:23,
  panel 251's evidence), and, while this file was written, `6e1158d65` (18:25, panel 251's verdict heading)
  and `f6cdbce44` (18:25, panel 253's briefs, reports and synthesis).
- **A task must lead its readers into the shape** (critic finding 11). Every task's reference held its
  shape, and three of five tasks still let the shortest correct program avoid it: task 1 asked for no
  function over `Rect`, task 4 printed its expected first line, task 5's checks passed any tolerance within
  thirteen decades. **The check that would have caught it before 7.08 USD was spent, at no cost**: write the
  shortest program the task's text admits, and ask whether it holds the shape and whether the grading tells
  a wrong value of it from a right one; run by reading the five task files, it flags tasks 1, 4 and 5. A
  line for `SKILL.md` § 2's blind-seat bullet, the coordinator's to propose.
- **Two instruments of the run are wrong and stay as they are**: the grader's ring test (finding 1), cited
  never, its count resting on a `grep` and the builds; and no session had `Write` (43 of 51 said so), the
  brief's fallback catching all 51, why unmeasured and a paid session to find.
- **The number 252 is held twice**: `git -C .claude/worktrees/p252 status --short` prints
  `docs/panel/252-briefs/` and `docs/panel/252-reports/` untracked, the 2026-11-15 sitting's (run
  2026-11-19 at 18:15), so a commit from that worktree would meet this sitting's.

## What this sitting files and owes

- **Defects filed with this sitting** (the coordinator files them): Q2's refusal, `count({"a": {}})` and
  the order-dependent literals refused, the message false, with the rows `[1, b]` and `{name(): []}`,
  `blocking`, repaired by R2's lane; `containers`' four false abort promises (`deque`, `cache`, `indexed`,
  `ordered`), `blocking`, repaired by R3's Q3-C; Rebel's byte-as-number sites in error and description
  text (6 named, up to 15), `adjacent`.
- **Owed, one lane after the sittings**: R1's deletion; R2's repair, with § 10's check; R5's `strtod`
  sentence, the spec and its pins moved in one change; R3's `containers` repair and the byte sites, Rebel's
  files.
- **With R1's deletion, the sentences that state the refusal**: the 22 comment lines in 20 files of
  `selfhost/` and three in `tests/harness/suite_layout.hero` (the compiler-engineer's premise 2, judged
  right by the critic); design.md §4.1's clause (`docs/design.md:942-943`), corrected beneath with its date;
  and, by `git grep` at the frozen commit outside `selfhost/`, `.claude/rules/module-shape.md:35-37`,
  `examples/interpreter/run/expr.hero:12-16`, `examples/interpreter/run/value.hero:13`,
  `examples/packages/README.md:107`, `tests/harness/mark_readers.hero:9` and
  `tests/harness/package_codes.hero:57` (which cites a spec § 1 sentence that does not exist); the dated
  records that state it (`docs/measurements/009-selfhost-readiness.md:306`,
  `docs/records/contract/case-law.md:375`) corrected beneath, never rewritten; and decisions 2403 (*"keep the
  rule and the loops"*) and 523 (the ~300 lines yielding where the language forbids the seam), closed,
  each gaining a comment that R1 lifted the rule it rests on.
- **Unrun, named**: a ring between fetched packages; Q2-R's two residue rows; the range refusal, a
  question with Q5-R; Q4-C, waiting with its question; the leading tasks the critic names for Q1, Q3, Q4
  and Q5, each a paid run and the coordinator's.
- **The sitting's evidence**, `252-reports/evidence.tar.xz` and `evidence.md`, by `SKILL.md` § 3c, as
  panel 251's (`f4e3594d5`).

## Author's verdict

**Ratified, 2026-11-19**, R1 to R5 as proposed: the author's word given at about 13:10, before this
sitting's briefs were written, meant as *OK to the ratifications as proposed*, of sittings A, B and C of
M-rebel-library, recorded as given. Its decision is https://github.com/heroes-lang/heroes/issues/2599,
opened and closed with this file's commit, its `## Prior art` the lines re-read that day. Filed with it:
defects 2596 (a container literal typed from its first part alone, `blocking`), 2597 (four containers
promising an abort they do not give, `blocking`) and 2598 (Rebel's messages writing a byte's number,
`adjacent`).
