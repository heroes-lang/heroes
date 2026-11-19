# Panel 252, sitting B of M-rebel-library: the language as Rebel met it. The brief every seat reads first

Convened by the coordinator of M-rebel-library on 2026-11-19, as the milestone page's item 33
(`docs/roadmap/milestones/M-rebel-library.md:257-276`, read at the frozen commit): *"**B, the language as
Rebel met it**: a private module that cannot use its own face (more than ten packages' witnesses), a type
parameter not taken from a map literal's key (spec § 9), records with no type parameter (`containers`), a
byte in `f"..."` written as its number with no word from the compiler, `print` writing `1e-06` where a
literal with an exponent is refused."* **The number**: sittings 251 and 252 of 2026-11-15 wrote briefs no
seat read, about threads and signals, and those questions sat today as panel 251; their files live in the
worktrees `p251`, `p252` and `p252-scratch` and are **not this sitting's input**. 252 is sitting B's
number: `git -C .claude/worktrees/rebel-round ls-tree -r --name-only 181555e9a docs/panel | grep -c
'panel/252'` prints 0.

**The full panel** (`.claude/skills/panel/SKILL.md:25-34`): every question can change a refusal, a
diagnostic class or a sentence of the specification (§ The five questions). **The seats**: the
compiler-engineer, the spec-warden, the historian (advisory) and the llm-ergonomist (the blind seat: 50
graded sessions and one verdict session the coordinator runs by `run.sh`, `llm-ergonomist.md`). **The
ffi-pragmatist does not sit**: no question moves what crosses the C boundary. Searched, not assumed: a
group's record is C's struct and takes no type parameter under Q3's robust arm (`extern` records keep
spec § 13's `Member` production, untouched by `bw/arms/q3-r.diff`); Q5's literal reaches C as today,
emitted round-trip-exact by `%a` (`docs/design.md:810`); Q4 and Q2 are the checker's; Q1's ring of modules
reads each module's headers as § 13 says, which the compiler-engineer runs (§ Question 1). If a seat
finds a C question, it says so and the coordinator decides. **No question is answered already** (a
refusal that exists and reads right is named and not convened): Q3's refusal of `record Stack<T>` reads
right, and Q3 is about whether the form enters; Q1's `module_cycle` reads right and its note's route ends
in a second refusal; Q5's `exponent_literal` refuses with *"there are no exponents"* where `print` writes
them; Q2's message is false where a key says the letter; Q4 has no refusal at all (§ The five
questions). **The completeness critic reads these briefs
before any seat starts** and the reports after (`SKILL.md:269-273`).

**The paid-run bound: 10 USD for the sitting** (the author's, 2026-11-19, relayed by the coordinator's
`brief-writer-common.md`; the author's words are not in the tree): **the blind seat's 51 sessions, each
capped at 0.40 USD and the verdict's at 0.60, its runner holding a CEILING of 9.90 USD** (the coordinator's
decision (a) of 2026-11-19, after the critic's finding 4: *"The 9.50 was the coordinator's margin, not
the author's word"*; a session starts only while the spend plus its cap plus a 0.10 margin is at most
9.90); **and the spec-warden's `heroes measure --refresh` calls, at most thirteen, one per arm, at 0
USD**: *"Token counting is **free to use** but subject to requests per minute rate limits based on your
usage tier"* (`https://platform.claude.com/docs/en/build-with-claude/token-counting.md`, § *Pricing and
rate limits*, read 2026-11-19 by the writer, the coordinator's decision (d); the Start tier's limit is
5,000 requests a minute, the same page). **No other paid run, by anyone**: no `claude -p`, no other
API call, no cloud run; one a seat finds worth running goes in its report with its size, for the
coordinator (`SKILL.md:288-295`). **No credential printed**: `gh auth status` at most, never `gh auth
token`.

**Where this brief is wrong, say so first.** Written by the coordinator's brief-writer on 2026-11-19,
its first clock reading `uptime`'s 14:21 and its last at the foot (`date`). Every number, path and count
below carries the command that produced it, run while writing; every output is under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-briefwriter/` (called `bw/`
below), **read-only to every seat**. A figure from an earlier sitting, a ledger, the coordinator's notes or
an issue says *carried* with its file and line, and is yours to re-run before you lean on it. Paths are
from the repository's root, `/Users/joseph/Temp/heroes/heroes-lang`.

## The frozen commit

**`181555e9a32d0b18878f9f10e48d2eadd939c285`** (`git -C .claude/worktrees/rebel-round log -1
--format='%h %ad %s' --date=iso`: *181555e9a 2026-11-19 13:52:09 +0100 Panel 251: its verdict names
decision 2590, ratified as proposed*), the `rebel-round` branch's HEAD when the writer started. **It is
not `main`**: `origin/main` is `0becec5b6` (2026-11-15 23:30:55), the merge base `f6c49d84c`, and
`origin/main` holds 3 commits the branch does not (`git log --oneline HEAD..origin/main | wc -l`). **The
branch moved after the freeze**: `f4e3594d5` (14:23:04, *"Panel 251's evidence"*) touches
`docs/panel/251-reports/evidence.md` and `evidence.tar.xz` alone (`git diff --name-only 181555e9a
f4e3594d5`). **You judge the frozen commit, never the working tree**: your copy is a `git archive` of
`181555e9a`.

**Since panel 251's commit `3a5ee2691`**: 42 files, 11,786 insertions, 444 deletions (`git diff --stat
3a5ee2691 181555e9a`), every one under `docs/panel/` (14), `packages/rebel/` (27) or
`examples-rebel/README.md` (1); **`spec/heroes-spec.md`, `seed/heroes.c`, `selfhost/` and `runtime/` did
not move** (`git diff --quiet 3a5ee2691 181555e9a -- spec/heroes-spec.md seed/heroes.c selfhost runtime`,
exit 0). The spec: 492 lines, 29,407 bytes, sha256 `1d532d514831aa70...` (`wc -l -c`, `shasum -a 256`).

**The two stages**, built by the writer in `bw/tree/` (a `git archive` of `181555e9a`, **20,037 files** by
`find bw/tree -type f | wc -l` and by the archive's own `tar -t | grep -v '/$' | wc -l`): the seed by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, `real 9.94` (`/usr/bin/time -p`),
**`sources 5aae00294f7cd6fa`**; stage 2 by `./heroes build selfhost/main.hero -o heroes-next`, exit 0,
`real 134.12`, `user 99.04`, `sys 24.71` (load 12 to 16, other sessions building), **`sources
23c7d2a626b29fd5`** (`bw/stage2.log`), the key panel 251's stage 2 printed, as the unmoved `selfhost/`
says. **A seat whose stage 2 prints another `sources` line is not on the frozen commit and says so
first.** Apple clang 21.0.0 (clang-2100.4.1.1), macOS 27.2, Darwin 27.2.0 (`clang --version`,
`sw_vers -productVersion`, `uname -r`).

**One probe compiler, built by the writer for question 1 and read-only**: `bw/q1/heroes-nocycle`,
**`sources 7023dd011002b5d5`**, stage 2 rebuilt from `bw/tree/` with `selfhost/modules.hero`'s `cycles`
answering at its first line (`if s.files.len() >= 0` / `return found`, two lines inserted after line 280,
`bw/q1/modules.hero.orig` holding the original, `bw/q1/modules.hero.patched` the patched file and
`bw/q1/nocycle.diff` the patch, written after the critic's finding 11 found no patch on disk), built in
139.59 s, the file then restored and `cmp`-equal to the frozen one (`bw/q1/nocycle-build.log`); the critic
rebuilt it from these words and read the same key. **It is a probe, not a route**: no test ran under it.

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

## The blind seat, in one paragraph

Five tasks, two arms each, five replicates: 50 graded sessions and one verdict session, in
`.claude/worktrees/rebel-scratch/252-llm-ergonomist/` (`llm-ergonomist.md` holds the design, the grading
and the counted decision rules, registered before any run). Task 1 writes the face and its private module
(`now`, `q1-c`), graded by building with the frozen stage 2 and again with `bw/q1/heroes-nocycle`; task 2
predicts the map-literal program (`now`, `q2-c`, its arm repaired); task 3 writes a ring for any element
type (`now`, `q3-r`, its example no longer the answer, both arms graded by one written rubric, `now`'s
build recorded beside, decision (c)); task 4 prints each word's first letter (`now`, `q4-r`); task 5
writes a tolerance of `1e-9` (`now`, `q5-c`, Q5-R read by a registered count). Every task's input holds
the shape it measures, each run by the writer's reference (`bw/ref/`). **The runner** holds a CEILING of
9.90 USD (decision (a)); retries a failed session once and records a second failure, grades a capped one
on what it wrote (decision (b), registered before any run; an API error reads `subtype success`, so the
classifier reads `is_error` and `terminal_reason` too, measured on the coordinator's probes); takes panel
251's three repairs and 253's three (`TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area; a preflight and a
scan that read only what the session did not write and fail on silence; a gate that copies the
spec-warden's report, hashes the copy and refuses an empty heading, decision (e)). **Its controls, at no
cost**: the scan fails all 51 of panel 251's leaked transcripts and flags no session's own words; the
preflight holds on the coordinator's probe4 and fails on panel 251's `t4-f-2` and on silence
(`llm-ergonomist.md` § The controls).

## Where each seat works, and the rules every seat obeys

**Your folder is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-<seat>/`**
(inside the repository's root, CLAUDE.md § Hard stops; ignored by git, `.gitignore`'s
`.claude/worktrees/`). **Make it yourself, from the frozen commit, and build nothing in the tree**:

```
F=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-<seat>
mkdir -p $F/tmp && cd $F && export TMPDIR=$F/tmp
git -C /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-round archive 181555e9a32d0b18878f9f10e48d2eadd939c285 | tar -x -C $F
find $F -type f -not -path "$F/tmp/*" | wc -l           # 20,037 at 181555e9a; say what yours reads
printf 'gitdir: /nonexistent-252-<seat>\n' > .git      # so the hooks judge your files in your copy
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes          # 9.94 s for the writer
./heroes build selfhost/main.hero -o heroes-next                    # 134 s for the writer
./heroes-next --version                                             # sources 23c7d2a626b29fd5
```

`measure` reads its vendored tables from the copy's root: run it from there, `./heroes-next measure
<path>` (from `bw/`, it exits with *"cannot read `vendor/tokenizers/claude-legacy.json`"*, run). **No seat
runs `heroes test` over the compiler or a package** (the Mac is shared; a test of one small program of
your own is allowed). Rules, each from `SKILL.md` § 3 and 3c or CLAUDE.md:

- **Your folder and your report, nothing else.** Your report is
  `.claude/worktrees/rebel-round/docs/panel/252-reports/<seat>.md`, **written as you go**, in your
  definition's output structure (`.claude/agents/<seat>.md`), one block per question; make the folder if it
  is absent. Never the tree's other files, another seat's folder, a lane, `bw/` (read-only), the blind
  area, or `p251`, `p252`, `p252-scratch`.
- **No file outside the repository's root**: `TMPDIR`, and `HOME`, `GOCACHE` and their kin for any tool
  that writes under them, inside your folder.
- **A program that may not end** is built and its **binary** run under `timeout -s KILL <seconds>`, never
  `timeout heroes run`; its output to `/dev/null` or through `head -c 65536` into a file; `pgrep -fl <your
  folder>` read after (`.claude/rules/verification.md` § A run that may not end). Bounds: a case 20 s, a
  program 60 s, a stage 2 1800 s.
- **One build or test run of yours at a time** (the Mac is shared; load read 12 to 16 while the writer
  built). **The machine is held awake** for a run past a minute: `pmset -g assertions` read
  `PreventUserIdleSystemSleep 1` at 15:51, held by Amphetamine's *"Single-Use - System"* assertion (217
  hours) beside powerd's display hold; by `.claude/rules/verification.md` § A long run holds the machine
  awake, Amphetamine's is the hold that counts, and `caffeinate -i -w <pid>` beside a long run costs
  nothing should it end.
- **The route you would adopt, you build in your folder and run on the cases you name, before the
  synthesis** (`SKILL.md:274-277`); a route that does not build is reported as not built.
- **Every number with its command; every negative sentence run, or posed as a question**; *only*,
  *never*, *provably* with what was searched and where (CL-077). A claim about another language from memory
  is a question for the historian.
- **No paid run, no credential printed, no GitHub write, no commit, no push, no tag.** `gh issue view`,
  `gh issue list` and a `gh api` GET are reads.

**Platforms**: Darwin arm64, this Mac. No question rests on a platform fact the writer found; a seat that
finds one runs it in `heroes-linux-arm64:latest` (native) under its own outer `timeout -s KILL`, one
container at a time, named `<prefix>-252-<n>` and stopped after (panel 251's rules, `251-briefs/00-shared.md:521-526`),
or names it **owed** to the CI. Windows: no seat.

## What panel 251's critics found, and where this brief answers it

- **The blind runner leaked three ways** (`docs/panel/251-reports/critic-reports.md:45-97`, finding 2,
  defect 2589): the labels by inherited standard input, the folder's path, the main checkout's git status;
  `context: no` saw none. Answered in `run.sh`: `< /dev/null`; the empty repository `s/.git` (with `.gitignore`
  `*`, user `reader`, no commit) above every session folder, the verdict's included (it stood beside `s/`
  in 251); `HOME` inside the area; and a free preflight session whose transcript `tools/preflight.py`
  reads before any spend. **The path still names the area** (`rebel-scratch`, `252-llm-ergonomist`),
  as 251's did; `context` asks, and the synthesis does not read `context: no` as proof (finding 2's last
  paragraph).
- **The predictions were registered 13 seconds after the runner started** (finding 4): `run.sh` copies
  the spec-warden's report into the area before the first paid session, hashes the copy, writes the hash
  and the time into `run.log`, and refuses unless the copy holds `## Registered predictions` with a
  non-empty line under it (panel 253's critic's finding 6: a hash of a file that keeps growing proves
  nothing later, and a bare heading proves nothing at all); `spec-warden.md` says so.
- **The tree was not frozen** (finding 1): the branch moved once since the freeze, `f4e3594d5`, evidence
  files alone (§ The frozen commit); every seat copies `181555e9a` by `git archive`.
- **A task whose input could not show the shape** (finding 3, the separator): every task here was run first
  as a reference holding the shape (`bw/ref/`: task 1's ring refused by the frozen stage 2 and built by the
  probe, `26`; task 2's line 7 refused; task 4's trap printing `117 3`; task 5's `1e-9` refused).
- **A paid run past the bound**: the estimate is measured from 251's own 51 `run.json` and simulated over
  this `order.txt` with the 9.90 ceiling (`llm-ergonomist.md` § The order, the runner and the bound).
- **Panel 253's critic, on the same runner** (`docs/panel/253-reports/critic-briefs.md`): its finding 4 (a
  scan reading a session's own `context` answer as a leak) is answered by reading only the entries the
  session did not write, and the scan's control on panel 251's 51 flags none of them for project text; its
  finding 5 (a panel 251 session's file in `/private/tmp/claude-501/`) by `TMPDIR` and `CLAUDE_CODE_TMPDIR`
  inside the area and a temporary-folder check in the preflight and the scan; its finding 6 by the gate
  above; its finding 15 (a verdict at 94% of its cap) by a 0.60 cap for the verdict alone.

**And the number 252 is held twice**: the 2026-11-15 sitting's `docs/panel/252-briefs/` and `252-reports/`
stand uncommitted in the worktree `p252` (`git -C .claude/worktrees/p252 status --short`, run 15:51), so a
commit from that worktree would meet this sitting's: a line the synthesis owes (the critic's finding 11, as
panel 251's critic said of 251).

## Repairs after the critic's first reading

The completeness critic read these briefs from 14:54 to 15:14 (`docs/panel/252-reports/critic-briefs.md`,
eleven findings); the coordinator decided (a) to (g) on them and on panel 253's critic's findings 4, 5 and
6 for the same runner. One line per finding, what changed or why nothing did; every repair run again by the
writer after 15:39.

1. **Q2-C false of task 2's line 6** (wrong): the arm reworded as the compiler behaves, measured on 20
   programs: *"nor does a literal whose first element or value gives none"* (+14); the five `t2-g`
   folders, `key.txt` and the grading row remade with it. **Found beside it**: the compiler's verdict
   depends on order inside one literal (`p`, `q` against `n`, `s`); Q2-R now owes an order-free verdict
   (§ Question 2). `bw/q2/` holds 20 programs, `f-array-lit` in the table.
2. **q3-r's example was task 3's answer, and the two arms were graded differently** (wrong framing): the
   example is now `record Pair<A, B>` (+63), the five `t3-f` folders, the verdict's `spec-B.md` and
   `key.txt` remade; both arms graded by one written rubric from the answer's text, `now`'s build recorded
   beside and never graded (decision (c), `llm-ergonomist.md` § The grading).
3. **Q5-R contradicted its own example** (wrong): reworded, *"A float is digits, a point and digits, or
   digits with an exponent, or both"* (+41); `proposal` (+160) and the verdict's `spec-B.md` with it.
4. **Whether the bound holds** (the coordinator's answer): CEILING 9.90 (decision (a)); no cell cut; the
   stop rule simulated again (all 51 at 251's medians, 46 at its maxima); a failed session retried once
   and a capped one graded on what it wrote, registered (decision (b)), the classifier reading
   `is_error` and `terminal_reason` because an API error records `subtype success` (measured).
5. **The preflight passed on silence and looked nowhere a fourth leak would land** (rule): it fails on no
   session id, no git status, no stored system blocks, an attachment kind beyond the two, system blocks
   other than the two; it reads the stored request's system blocks and records `output_config`; `TMPDIR`
   and `CLAUDE_CODE_TMPDIR` inside the area, and the temporary folder asked before and after (decision
   (e)); controls on probe4, panel 251's `t4-f-2` and a silenced transcript.
6. **The refreshes' "0 USD" was carried** (unrun): the page read today, quoted with its date (decision (d),
   § the paid-run bound); thirteen refreshes, one per arm, counted at 0.
7. **Q1-A, a face naming its private module's type, and its record** (route): written as arm `q1-a` (+54)
   with panel 031's R4 and panel 135; the historian, the spec-warden and the compiler-engineer pointed at
   them.
8. **Q3's record missed panel 029; `Nest<[T]>`; Q3-C's mechanism** (unrun, shape): panel 029's R4 and R5
   named; Part 6's range corrected (3555-3722) and the claim re-run on it; `record Nest<T>` with `inner:
   [Nest<[T]>]` and a recursive generic variant in Q3-R's price; Q3-C's mechanism posed as a question.
9. **A float's `_`; Q5-R's count; Q5-P's cell** (unrun, rule, route): `0.000_000_001` and `1_000_000.5`
   measured accepted, the route written as arm `q5-u` (+14); Q5-R's decision rule registered with a
   count (*"at least 3"*); Q5-P left without a cell, priced for the coordinator.
10. **Q4's narrower arm and panel 061** (route, record): arm `q4-r2` (+52) written and measured; panel
    061's measurement cited in Q4's record.
11. **Smaller points**: the probe's patch kept on disk (`bw/q1/nocycle.diff`, `modules.hero.patched`);
    the eight `module_cycle` comments in one place; the number 252 held twice (a line above); the machine
    held by Amphetamine's *"Single-Use - System"* assertion (217 hours at 15:51, `pmset -g assertions`) beside
    powerd's display hold; the folder's path kept, the area's name being the contract's and the label
    random (`llm-ergonomist.md`); the census's clock carried to the compiler-engineer (about 11 minutes a
    census at `-P 2`, the critic's, carried); the verdict speaking for Q3's form alone; `result.md` named as
    what the grading reads (panel 251's 51 wrote no `report.md`, `grep -c` over its `run.log`); the
    refreshes' *"free"* now the page's own words, read today.

**Panel 253's critic, findings 4, 5 and 6**: taken whole (§ What panel 251's critics found). Its finding 15
taken too (the verdict's cap 0.60). Its other findings were read by their titles alone: 1 to 3, 7 to 12
and 14 concern its own tasks and questions; whether 13 (*"Every clock the briefs ask for falls while five
agents load the Mac"*) or 17 (smaller facts) binds this sitting is a question, unread.

## Questions no one has run, posed as questions

- Does anything in the per-module build, the cache, the cards, `heroes doc` or a package's placement need
  modules to form no cycle, now that a unit's key is its own emitted text (Q1)?
- Is Q1-R2 (a ring only through a face and its `internal/`) a rule a reader meets, and what does it cost
  beside Q1-R?
- Do `cache`, `indexed` and `ordered` keep their promise to abort on a list that is not their own, and can
  any check keep it, values being copies (Q3-C's mechanism)?
- What does Q1-A, an alias of a private module's type in its face, cost beside Q1-R, and do Rebel's mirrors
  meet one of panel 135's return conditions?
- Does Q3-R refuse `record Nest<T>` with `inner: [Nest<[T]>]` at its declaration, or hang?
- How many `f"..."` holes and `print` arguments in `selfhost/`, `examples/`, `tests/` and Rebel hold a
  `u8`, and how many of those mean the number (Q4's census)?
- Should a character literal default to `u8` (Q4's shape beside)?
- What does Q5-P print for the largest `f64`, and how many goldens move under it?
- Do `spec`'s `named` and `rejected` checks accept `Pair<i64, str>`, `record Pair<A, B>`, `2.5E3`,
  `0.000_001`, `alias Point = types.Point` in a code span (the arms), given that `1e-06` already stands in
  one today (§ 11)?
- Does `TMPDIR` with `CLAUDE_CODE_TMPDIR` keep every file a session writes inside the area (the preflight
  asks it first, at no cost)?
- What do Go, Rust, Python, Swift, OCaml and Haskell do at each question (the historian's brief)?
- Whether every blind session, run with the three repairs, answers `context: no`, and whether its
  transcript agrees.

Written by the coordinator's brief-writer, its first clock reading 14:21 (`uptime`) and its last 14:52
CET on 2026-11-19 (`date`); repaired after the critic's first reading from 15:39 to 15:54 CET (`date`). No seat
had started, and the blind area's `run.sh` had not run.
