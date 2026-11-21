# Panel 253, sitting C of M-rebel-library: Rebel's place and its tools. The brief every seat reads first

Convened by the coordinator of M-rebel-library on 2026-11-19, as the milestone page's item 33
(`docs/roadmap/milestones/M-rebel-library.md`, read at the frozen commit): *"**C, Rebel's place and its
tools**: design.md §1.11's sentence and the verb `heroes doc` (what 253 was to hold), the route of the
system's cryptography on macOS (a member written in Swift, or primitives in Heroes under a checked
constant-time discipline; decision 29), and a skipped outcome for `heroes test`."* Items 34 and 35 add two
more: *"the whole stack of calls when a program aborts, which is the runtime's, joins sitting C"*, and
*"how it is spelled (`use`, `heroes add`) and how it meets panel 242's R2 and panel 250's whole placement
is sitting C's"*. **Six questions**, numbered as the coordinator's convening message numbers them (§ The
six questions). `git ls-tree -r --name-only 181555e9a -- docs/panel | grep -c '^docs/panel/253'` prints
0: nothing of this sitting existed at the frozen commit.

**The full panel** (`.claude/skills/panel/SKILL.md:25-34`): questions 2, 4 and 6 change the command's
surface and a sentence of the spec, question 1 design.md Part 1 and the ffi-pragmatist's own charter,
question 3 the C boundary and possibly the toolchain a program's build needs, question 5 the runtime and
possibly spec § 6's sentence. **The completeness critic reads these briefs before any seat starts**, and
the reports after (`SKILL.md:269-273`); its first reading is `docs/panel/253-reports/critic-briefs.md`,
and **§ Repairs after the critic's first reading** says what each of its 17 findings changed. **§ The
seats** says who sits for which question, and **§ One sitting, six questions** records the coordinator's
decision to keep the six together and the order and stop point it gives the compiler-engineer.

**The paid-run bound: 10 USD for the sitting** (the author's, given 2026-11-19, as the coordinator's
`brief-writer-common.md` relays it; the author's words are not in the tree): **the blind seat's 31
sessions, the 30 graded capped at 0.40 USD each and the verdict at 0.60, the runner stopping before 9.50
in all, after a preflight session whose key the API refuses, which costs nothing; and the spec-warden's
`heroes measure --refresh` calls, at most seven (`spec-warden.md`), counted at 0 USD inside the bound** (panel 251's
decision on the same question, `docs/panel/251-briefs/spec-warden.md:63-75`: `count_tokens` is free by
Anthropic's page, carried there and carried here). **No other paid run, by anyone**: no `claude -p`, no
other API call, no cloud run; one a seat finds worth running goes in its report with its size, for the
coordinator (`SKILL.md:288-295`).

**Where this brief is wrong, say so first.** Written by the coordinator's brief-writer on 2026-11-19,
its first clock reading 14:21 (`date`, `uptime` read load 11.36). Every number, path and count below
carries the command that produced it, run while writing; every output is under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-briefwriter/` (called `bw/`
below), **read-only to every seat**. A figure from an earlier sitting, a report, a ledger or an issue
says *carried* with its file and line, and is yours to re-run before you lean on it. Paths are from the
repository's root, `/Users/joseph/Temp/heroes/heroes-lang`; a seat reads the frozen tree through its own
`git archive` copy, never through a working tree.

## The frozen commit

**`181555e9a32d0b18878f9f10e48d2eadd939c285`** (`git -C .claude/worktrees/rebel-round rev-parse HEAD`
at 14:21; `log -1`: *181555e9a 2026-11-19 13:52:09 +0100 Panel 251: its verdict names decision 2590,
ratified as proposed*), on branch `rebel-round`. **It is not `main`**: `origin/main` is `0becec5b6`
(*"Batch 39 round closed ..."*), the merge base `f6c49d84c` (`git merge-base origin/main HEAD`), and
`git log --oneline HEAD..origin/main | wc -l` prints 3. **The branch moved while this was written**:
at 14:41 its HEAD read `f4e3594d5` (*"Panel 251's evidence: the sitting's folders packed ..."*), whose
`git diff --name-only 181555e9a f4e3594d5` names `docs/panel/251-reports/evidence.md` and
`evidence.tar.xz` alone. **You judge `181555e9a`, never a working tree**: your copy is made by `git
archive` of it.

**Since panel 251's frozen commit `3a5ee2691`** three commits (`git log --oneline 3a5ee2691..181555e9a`:
`5c5e6f811`, Rebel's step 53, and panel 251's two), and `git diff --stat 3a5ee2691 181555e9a --
selfhost runtime seed spec docs/design.md tests` prints nothing: the compiler, the runtime, the seed, the
spec and design.md are 251's.

**The two stages**, built by the writer in `bw/tree/` (a `git archive` of `181555e9a`, 20,037 files by
`find tree -type f | wc -l` and by the archive's own `tar -t | grep -v '/$' | wc -l`): the seed by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 10.07` (`/usr/bin/time -p`, load
11.4), **`sources 5aae00294f7cd6fa`**; stage 2 by `./heroes build selfhost/main.hero -o heroes-next`,
exit 0, `real 136.05`, `user 100.40`, `sys 24.64` (load 10.5 to 12.4), **`sources 23c7d2a626b29fd5`**
(`./heroes-next --version`, `bw/stage2.log`), the same key panel 251's seats built. **A seat whose stage
2 prints another `sources` line is not on the frozen commit and says so first.** Apple clang 21.0.0
(`./heroes-next doctor`), `swiftc` 6.4.2 (`swiftc --version`: *Apple Swift version 6.4.2
(swiftlang-6.4.2.1.9 clang-2100.4.1.1)*, `xcrun --find swiftc` under Xcode's toolchain), macOS 27.2.

**The spec at the frozen commit**: 492 lines, 29,407 bytes, sha256 `1d532d514831aa70...` (`wc -l -c`,
`shasum -a 256`), unmoved since 2026-11-15. `./heroes-next measure spec/heroes-spec.md`
(`bw/measure-before.txt`): `claude-legacy 8551`, `cl100k_base 8702`, maximum **8702**, and the pinned
`real 11384` (*claude-opus-5, 2026-11-15*, carried by the command from `selfhost/measure/pinned.hero`, no
refresh run), headroom 5000 against 16384.

**The author's word already given on this sitting** (carried, `docs/panel/251-*.md`, § The author's
verdict): *"Ratified, 2026-11-19, R1 to R3 as proposed ... the author's word given at about 13:10, before
this synthesis was written, meant as OK to the ratifications as proposed, of sittings A, B and C of
M-rebel-library, recorded as given."* So **what this sitting proposes is what lands**, by the author's
prior word as that file records it: a seat that would object to a resolution objects in its report, where
the synthesis must carry it; there is no later reading to rely on.

## The six questions

Each with its record, what was measured for this brief, and the robust and conservative resolutions put
to the seats; **none is adopted**. Every spec arm below is written from the frozen spec by
`bw/tools/make_arms.py` (each edit asserted to match once) into `bw/arms/spec-<arm>.md`, its unified diff
beside it, and counted from the copy's root by `./heroes-next measure ../arms/<file>`
(`bw/measure-arms.txt`): the vendored maximum, **a lower bound until the spec-warden's refreshes**;
*"the two instruments disagree by a third"* (`.claude/rules/spec-shape.md:161`), and panel 251's R read
`+5` vendored against `+9` real (carried, its brief's table and `docs/panel/251-reports/critic-reports.md:170`).

| arm | what it is | sha256, first 16 | lines | legacy | cl100k | maximum | delta |
|---|---|---|---|---|---|---|---|
| `spec-now.md` | the frozen spec | `1d532d514831aa70` | 492 | 8551 | 8702 | 8702 | 0 |
| `spec-doc-r.md` | Q2 robust: the doc comment's shape and `heroes doc`, § 1 | `656b8364f33abaeb` | 498 | 8657 | 8814 | 8814 | **+112** |
| `spec-skip-r.md` | Q4 robust: `skip(reason:)`, § 12 | `a47a36c1f157cd29` | 495 | 8609 | 8761 | 8761 | **+59** |
| `spec-umb-r.md` | Q6 robust: an umbrella's `packages`, § 1 and its example | `07acf83d2ec062f9` | 500 | 8682 | 8836 | 8836 | **+134** |
| `spec-stack-r.md` | Q5, where the spec is read to owe it: § 6's abort sentence | `5f64ac703c5f7978` | 492 | 8564 | 8715 | 8715 | **+13** |
| `spec-all-r.md` | Q2, Q4 and Q6 robust together | `48d807010b254507` | 509 | 8846 | 9007 | 9007 | +305 |
| `spec-all-r-stack.md` | and Q5's sentence | `cb648afa1ec30da0` | 509 | 8859 | 9020 | 9020 | +318 |

Questions 1 and 3 add no spec token in either resolution as written (§ Question 1, § Question 3); the
spec-warden says whether either owes one. design.md is under no ceiling (`./heroes-next measure
docs/design.md`: *"No ceiling judges `docs/design.md` (panel 123 R5), so this is a count and not a
verdict"*); its arms are counted the same way: `design-now.md` 113,142, `design-r.md` 113,475
(**+333**), `design-c.md` 113,230 (**+88**).

## Question 1: design.md §1.11's sentence, and what it invalidates

**The author's decisions** (`M-rebel-library.md`, decisions of 2026-11-15): 2, *"Original, from scratch
... with coverage equivalent to"* Go's, Rust's and Python's; 6, *"The compiler does not use Rebel to write
itself"*; 8, *"`core` no longer exists: every trace removed"*; 9, *"Heroes imposes no standard library and
offers one that is optional. The Zen's law 18 becomes 'There is no standard library to obey; there is a
Rebel to call, or not.'"*; 10, *"Superseded decisions are declared invalidated in one register; living
texts are rewritten clean; sittings, journals and closed issues stay, each with its note"*. And
2026-11-16's 24: *"What reads data from outside is written in Heroes, never bound from a C library."*

**What §1.11 says today** (`grep -n '^### 1.11' docs/design.md`: line 482, its rule box 490-492, read
whole at 482-620): the title *"The founding constraint: there is no standard library"*; the box *"Heroes
ships a minimal runtime and nothing else. Anything a real program needs — maths beyond arithmetic, files,
sockets, regex, JSON, crypto, compression, databases, HTTP — comes from C libraries through the FFI.
Writing a broad standard library is out of scope, permanently."*; *"That is the whole standard
library"* (544) over a table of needs and the C library each comes from (547-560); the paragraph *"The
core packages do not change this: a package is code a program carries"* (562-586), which holds panel
242's ruler as panel 250's R2 wrote it (*"it takes one only where its own tree, every line of every
placed file counted, is smaller after the step than before"*, 578-579). **Part 2** (`grep -n 'convenience
functions' docs/design.md`: 668): *"No standard library beyond a minimal runtime. See 1.11 — this is the
founding constraint, not a consequence. Do not accumulate convenience functions; every one you add is a
permanent maintenance obligation and a competitor to the FFI."* `grep -n -i 'standard library'
docs/design.md` prints 10 lines (482, 486, 492, 544, 574, 667, 910, 3172, 3604, 3706).

**What the spec says**: nothing of a standard library (`grep -n -i 'standard library'
spec/heroes-spec.md` prints nothing); § 13's first sentence, *"Anything beyond this document comes from
packages or C libraries"* (377), already holds for Rebel as a package, and § 1 says what a package is
(lines 30-40). **So neither resolution below adds a spec token**: Rebel is a package, and a spec that
named it would make it the language's, which decision 9 refuses. The spec-warden confirms or refutes.

**Outside design.md, measured** (`git grep` at `181555e9a`):

- **CLAUDE.md § 13**, *"Where not to go ... A standard library."* (CLAUDE.md:278-279): the contract
  forbids what the milestone builds. CLAUDE.md is amended by the author, not by a sitting (CLAUDE.md § 4);
  the sitting names the line and the coordinator puts it to the author.
- **The ffi-pragmatist's own charter**: `.claude/agents/ffi-pragmatist.md:3`, *"Panel judge for the
  founding constraint (design.md §1.11, §4.19 — there is no standard library; everything comes from C)"*,
  and its first paragraph quotes the old box. The seat judges a change to its own mandate; the
  definition is the process's, amended by the author.
- **The Zen's law 18**, *"There is no standard library; C is the man who sold the world."*: printed by
  `heroes this` from `selfhost/cli/doctor.hero:254` (its test at `:287`) and `seed/heroes.c:365`, and in
  `site/src/html/project.html:95` and `site/src/html/it/project.html:95` (`grep -n`). Not design.md, and
  not the sitting's text to write, but the same decision's.
- **The site**: `why/faq.html:70-71` (*"There is no standard library, on purpose"*), `why/not.html:25`,
  `why.html:140`, `examples/packages.html:3` (*"A core package ..."*), in both editions.

**What decision 10's register would hold, enumerated from the milestone decision 8 empties** (repaired
after the critic's finding 11; the first list came from words): `gh issue list --milestone M-core-packages
--state all --limit 400` prints **231 issues, 192 closed and 39 open** (25 defects, 10 features, 4 tasks),
**19 of them decisions** (`bw/q1/core-milestone-issues.tsv`, sha256 `c09d85c6ac5817d9`): 2304 (panel 234),
2403, 2404, 2415 (panel 241), 2423 (panel 242), 2449 (243), 2461 (244), 2475 (245), 2497 and 2498 (246),
2517 (247), 2527 (248), **2528** (*"Until the core packages reach 1.0.0, every program moves to the newest
version of core"*), 2560 (*"Whether the package manager's second version (feature 2315) meets panel 242's
R2 by placing only the modules"*), 2561 (the hooks' lane over core), 2562, 2563 (249), **2564** (panel
250, which question 6 turns on) and 2590 (251). Its 14 open features and tasks: 2551, 2460, 2399, 2358,
2313, 2298, 2297, 2296, 2082, 1103, 1072, 1067, 925, 927 (`awk` over the file); its 25 open defects
2522 to 2587 (the file). **And 17 programs of `examples/`** hold `[dependencies.core] version = "0.6.0"`,
`index = "file://../../packages"` (`grep -l 'dependencies.core' examples/*/heroes.toml`: 17 of 17
catalogues, `bw/q1/core-catalogues.txt`), so decision 8 moves them, and the `compat` suite reads them at
each release tag. Which of these the register declares invalidated, which stand and which move to another
milestone is the spec-warden's to propose and the coordinator's to file. Beside the milestone, the word
search of the first writing (`grep` of `docs/panel/*.md` for *standard library*: 9 sittings, `038 041 080
097 100 127 162 234 241`; `gh issue list --state all --search` for *standard library*, *invalidated*,
*register of the decisions*, *Zen law 18*) adds the rulings it names: **panel 234** R1's admission
test (*"a function enters when a command's count shows a program already needs it ... or a named
acceptance program calls it"*, `docs/panel/234-*.md:275-281`), R2 (*"Go's tree under `core/`"*), R3 (the
conventions page under `examples/packages/`); **panel 235**'s R15 (`use <core>/strings`, cited in §1.11 at
567); **decision 2404** (*"The first batch's `path` admits no function, since no program of
`examples/` writes one by hand"*, closed 2026-11-10); **decisions 819 and 853** (*"small packages organised
as Go's tree"*, *"the level is Go's and Echo's"*, both 2026-09-03, `gh issue view`); the open feature
**1067** (*"`strconv`, because six programs write the same digit loop by hand"*, milestone M-core-packages)
and **2082** (*"the repository's own hooks rewritten in Heroes over the core packages"*), and feature
**2315** (*"The package manager rewritten over the core packages"*, `kind:feature`, milestone
M-package-index); open task **2313**
(*"Every issue on dependencies, the core packages and `use` paths searched and given an outcome"*). **Not
invalidated, by this writer's reading, and the seats' to confirm**: panel 097's condition 5 and panel
234's B5 veto (no package source in `selfhost/library_source.hero`), which decision 6 keeps; panel 242's
R1 to R4, whose ruler binds the compiler taking a package, which decision 6 makes moot for Rebel without
falsifying; panel 241's *"the compiler takes no package yet"*. **The milestone page names none of these by
number** (`grep -n -E '234|235|2404|242|250|register' docs/roadmap/milestones/M-rebel-library.md`: line
15, line 79, *"register"*, and line 308's *"panel 242's R2 and panel 250's whole placement"*): the
coordinator's message listed panel 234, panel 235 and decision 2404 as *"others the page lists"*, and the
page lists none; the register does not exist at the frozen commit (`grep -rln -i 'invalidat'` over the
tree's `.md` files outside `docs/panel` and the journal finds `docs/design.md`,
`.claude/rules/verification.md`, one question of `docs/learn/2026-08/13/` and the milestone page, none a
register).

**Robust** (`bw/arms/design-r.diff`, 113 lines, **+333** vendored on design.md): every sentence of design.md
the decisions falsify, rewritten: §1.11's title *"no standard library is imposed"*; the box (*"Heroes
ships a minimal runtime and nothing else, and imposes no standard library ... comes from a package it
carries or from a C library through the FFI. Rebel, the broad library this project writes, is a package
like any other: optional, fetched through the program's own catalogue, never shipped with the compiler or
the runtime, and never used by the compiler to write itself. There is no standard library to obey; there
is a Rebel to call, or not."*) with an `AMENDED by panel 253` note quoting the old words; the leverage
paragraph (*"nothing in the compiler or the runtime grows with Rebel"*); *"That is the whole of what the
language provides"* and the table gaining a column, Rebel's package for each row (each checked to exist:
`[ -f <pkg>.hero ]` over `packages/rebel/0.1.0/files` for the 18 named, every one present; audio has
none); the core paragraph's opening and its `use <core>/strings` example; Part 2's bullet (*"Do not
accumulate convenience functions in the language, the compiler or the runtime ... A convenience a program
may take or leave belongs to a package, Rebel's or its own"*); 910's *"with no standard library imposed"*.
**Beside it, outside the sitting's text**: CLAUDE.md:279, the ffi-pragmatist's definition, the Zen, the
site, each named for the author or the milestone; and the register, one file, its rows the list above
with what each ruled and what replaces it.

**Conservative** (`bw/arms/design-c.diff`, 27 lines, **+88**): the box's last sentence alone, *"Shipping a
standard library with the compiler or the runtime is out of scope, permanently; a broad library a program
may take or leave, as Rebel is, is a package"*, its note, and Part 2's bullet narrowed to *"in the compiler
or the runtime"*; the table, *"That is the whole standard library"* and the core paragraph left standing,
each a sentence a reader of design.md then finds false (the table names PCRE2 for regular expressions
where Rebel's `regex` stands, `[ -f regex.hero ]`).

## Question 2: the doc comment's shape, and the verb `heroes doc`

**The author's decisions**: 13 (2026-11-15), *"The documentation is decided first: the shape of the
comment above a function, a record and a variant, and the verb `heroes doc`, both in this step"*; 22
(2026-11-16), *"each parameter in a markdown list; one documented example for every public function, under
`## Examples`; the reference pages in English in both editions of the site (`examples-rebel/README.md`
§ A7)"*. **The shape as Rebel wrote it**, `examples-rebel/README.md:308-396` (§ A7, read whole): a first
sentence that stands alone; a line holding `#` alone, then `` - `name`: `` per parameter in the
signature's order; the answer's edges; `Fails` with codes; the cost; `See also`; a comment above every
field and case; **an example is a `test` block under `## Examples` with a comment directly above it, its
title opening with the function's name and a colon**; *"What reads it: `heroes doc <face>` prints the
package's reference, the same bytes every time: each declaration's signature, its comment, the call
derived from its signature with its labels, and its examples."*

**What the spec says today**: *"`#` comments to end of line, markdown inside. A comment directly above a
declaration documents it; `##` is a section heading."* (spec lines 52-53). Nothing of parameters or
examples.

**The record**: design.md Part 6's literate-source row, *"`heroes doc` generates the document instead —
one direction only"* (`grep -n 'heroes doc' docs/design.md`: 3597); panel 136 refused the doctest and
homed *"the documented test block, Go's and D's shape"* at M-doc-generator (design.md:2601-2604, 3908);
**the stopping rule** (`.claude/rules/cli-surface.md:18-25`: *"A capability enters the surface only if
the fixpoint invocation, the golden harness or the design.md Part 11 harness must type it, or it has a
measured Part 11 effect"*), which refused `outline` and `explain` (`docs/measurements/003-closure-list-audit.md:82-100`)
and which issue **808** (`OPEN`, milestone M-doc-generator, *"the stopping rule is the opening's first
question"*) says `heroes doc` fails; **the two verbs that entered by the author's decision since**, `probe`
(panel 179, `cli-surface.md:31-38`) and `new` (panel 244's R2, issue 2441, `:39-43`), each with its shape ruled by
the rule's first sentence (*a subcommand if it answers a different question, meaning a different artifact
class*). **The card writer that exists**: `tests/harness/package_card.hero` (452 lines, `wc -l`), panel
234's R5, *"A core package's card, written from its face ... each function's signature with the call a
program writes beside it"*, its call derived by the checker's label rule and built to prove it compiles
(`package_judge.hero`); it reads `<core>` faces through `module_docs.hero` (212 lines), a reader of
comment lines, not of the compiler's parse.

**Measured today**: `./heroes-next doc packages/rebel/0.1.0/files/strings.hero` prints *"error: unknown
command `doc` — the commands are lex, parse, check, build, run, test, fmt, add, fetch, new, doctor,
mutate, probe, measure, grammar, this"*, exit 2; `grep -c 'Command(' selfhost/cli/table.hero` prints 16,
so `doc` would be the seventeenth. The site counts them: *"One command, sixteen verbs"*
(`site/src/html/start.html:131`; `it/start.html:134` *"sedici verbi"*; `log.html:14`, `it/log.html:14`),
and the site's claims check reads the `Command(` rows (`site/src/lib/claims.ts:264`), so the verb moves
four pages in two editions and the site's build. **Rebel's faces against § A7**, by `bw/tools/doc_census.py`
(a reader of lines, `bw/q2/doc-census.txt`): 149 faces; 7,718 top-level declarations, **every one with a
comment directly above**; 5,210 functions, of which **4,568** carry parameter lines naming exactly their
parameters in order (4,548 in the first writing: the reader took the `>` of a function type's `->` for a
closing bracket, the critic's finding 12, repaired in the script and re-run), **626** have parameters and
no parameter line at all; **4,305** examples, covering **4,281** distinct functions. So `heroes doc` over
Rebel today would meet 642 functions whose parameter lines disagree with their signature by that reader's
rule, and 929 with no example. **And no check reads Rebel's faces today**: § A7 says *"The check of the
net holds every rule above that a machine can"* (`examples-rebel/README.md:375-379`), but no file outside
`packages/` and `docs/` names `packages/rebel` except `examples-rebel/heroes.toml` and the README itself
(`grep -rl packages/rebel`), and `.github/workflows/` holds `ci.yml`, `codeql.yml`, `deploy-site.yml`,
`release-site.yml` and no workflow of Rebel's (`ls`): the check is one to write, under either route.

**Robust**: `heroes doc <module>` the seventeenth verb, a subcommand by the rule's first sentence (its
artifact a reference document, which no verb prints), entered by the author's decision 13 as `probe` and
`new` entered; it reads the module **through the compiler's own parse and check**, so a signature is the
checker's and the call is derived by the label rule the checker applies, never a second reader of lines;
it prints, the same bytes on every run and platform, each declaration's signature, its comment, the
derived call, and its examples (the documented `test` blocks whose title opens with its name), the module
doc first; **it refuses, exit 1 with a diagnostic each, what a machine can hold**: a parameter line naming
no parameter, two out of order, an example title naming no function of the module (a new diagnostic
class, the spec-warden's to price); a package operand (`heroes doc <rebel>/json`) reads the face through
the program's catalogue; **a golden form `doc`** under `tests/golden/doc/`, each case a module and its
`.expected` bytes, judged by `heroes run tests/harness/main.hero -- <compiler> doc`, and a determinism
case (two runs, `cmp`); the spec sentence `spec-doc-r.md` (**+112**: the shape, the example rule and the
verb's promise in § 1); the site's four pages and its claims; issue 808 closed by the author's decision
and this sitting's shape.

**Conservative**: no verb; the reference is printed by a harness program grown from `package_card.hero`
(`heroes run tests/harness/<program>.hero -- <face>`), the site reading its output; the shape stays in
Rebel's README, judged by a harness check that does not exist yet (above); spec 0 tokens. What it gives up: a program's author outside
this repository cannot print a package's reference with the one command (CLAUDE.md § 10), and the
reference is made by a second reader of the module beside the compiler.

**Between them, for the seats to price**: the verb with no refusal (prints whatever is there, exit 0), and
no spec sentence (the shape left to the verb's own help and the packages' README); and where the
example's rule lives if the spec does not say it (a reader of the spec writes a doc fence, the form panel
136 refused, measured by the blind seat's task 2).

## Question 3: the system's cryptography on macOS

**The author's decision 29** (2026-11-17, `M-rebel-library.md:188-212`): *"The system's cryptography leaves
a gap on macOS, kept in 0.1, and a sitting follows ... The author's choice among three: the gap now and a
sitting after, on writing those primitives in Heroes under a constant-time discipline the tools check, as
Go writes its own; refused were loading OpenSSL where a Mac happens to have it (a premise about the
machine) and the gap with no sitting."* Then, the same evening, CryptoKit reached through Swift's
`@_cdecl` (*"a member written in Swift needs `swiftc` on every Mac that builds a program using it, a change
of the compiler's surface the sitting decides beside the constant-time route"*). And on the fly
(2026-11-16): *"a TLS engine written in Heroes is refused, since its cryptography would carry no promise
of constant time"*; *"The system's cryptography is measured before it is bound"*.

**What the member does on each platform today**, its own table (`runtime/platform/hero_crypto.c:17-55`,
3,545 lines, its header `runtime/hero_crypto.h` 263, `wc -l`): on **macOS 27.2** no AES-GCM and no
ChaCha20-Poly1305 (*"CommonCrypto's public modes are ECB, CBC, CFB, CTR, OFB, RC4 and CFB8 ... its GCM is
in a private header the SDK does not ship. CryptoKit has both ciphers and is Swift's alone"*), no Ed25519
and no X25519 (*"`SecKey` takes RSA and the NIST curves ... no Curve25519 key"*); ECDSA, RSA and ECDH on
`SecKey`. On **Linux**, OpenSSL 3 by `dlopen` at the first call, every row. On **Windows**, CNG, **no
Ed25519** (*"no identifier names Ed25519 or EdDSA"*) and X25519 *"unmeasured"*, every Windows line
*"unrun"* (the box was off). Its measurements (*"Wycheproof's files (3,173 applicable cases) and NIST
CAVP's AES-GCM (1,500): Linux arm64 0 failed ... macOS 0 failed of every case of a kind it offers"*) are
**carried**, and their probe folder, `rebel-scratch/crypto-system/probe/`, no longer exists (`ls`: no
such file).

**The gap as Rebel's ledgers say it**, every capability block naming *PENDING* or *unsupported* within
80 characters of *macOS* (`bw/tools/macgap.py`, `bw/q3/macgap.txt`, 15 rows in 11 ledgers): the
primitives, `crypto/seal.ledger` **seal/aes-gcm** and **seal/chacha20-poly1305**,
`crypto/keypairs.ledger` **keypairs/ed25519**, **keypairs/x25519** and keypairs/pkcs8 (an EC key without
its public point), `crypto.ledger` crypto/ciphers; **the packages that inherit it**: `jose` jws/eddsa,
`nats` nats/auth-nkey, `keyring` keyring/protect (pending), `web` cookie/sealed, `webauthn`
webauthn/cose-key, `x509` x509/check-signature; and **a different gap of the same platform**, TLS over a
stream the program holds, `tls` tls/client-over and tls/server-over, `http` client/proxy (Network.framework
runs TLS only over its own connection), which no cryptographic primitive closes.

**Measured again by this writer, since the page's probe is gone** (`rebel-scratch/swiftprobe/` does not
exist, `ls`): one Swift function over CryptoKit, `swp_ed25519_public`, exported by `@_cdecl`
(`bw/q3/swift/swp.swift`, sha256 `fef9066740861a2c`), built by `swiftc -parse-as-library -emit-library
-static -module-name swp -O` in `real 2.31` into `libswp.a` (6,472 bytes, `nm` shows `T
_swp_ed25519_public`), bound by a Heroes `extern "swp.h" link "swp"` group with two `@... [u8] counted_by
32 lent` parameters (`bw/q3/swift/main.hero`) and built by stage 2 with `--include . --library .`: exit 0,
it prints `status 0` and **`d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a`, RFC 8032's
first vector's public key**. `otool -L` of the binary names CryptoKit, Foundation and eight
`/usr/lib/swift/libswift*.dylib` beside libSystem; 286,456 bytes. Plain build only; `--sanitize`, a
seal, a signature and the vectors beyond one are unrun by this writer.

**How the layer is built** (`selfhost/cli/layer.hero:1-41`): *"The capabilities whose C differs by
platform ... are C files of the project's own beside the runtime ... This file compiles them under the
runtime's own words, archives them as ONE static library, and puts the archive on every link"*, keyed
by clang's answer and the members' bytes, **compiled on the machine that builds the program**; so a
member written in Swift is a `swiftc` every Mac that builds a program needs, a key that holds `swiftc`'s
answer, a `heroes doctor` row, and the CI's `macos-26` leg (`.github/workflows/ci.yml:368`, `:1434`).
**The Command Line Tools carry `swiftc`** (the critic's finding 10, settled on this Mac):
`/Library/Developer/CommandLineTools/usr/bin/swiftc` exists (`ls -la`: a link to `swift-frontend`) and the
CLT's `MacOSX.sdk` holds `CryptoKit.framework` (`ls ... | grep -c CryptoKit`: 1, run by this writer); the
critic built the writer's `swp.swift` under the CLT alone, `swiftc` called bare failing *"unable to load
standard library for target 'arm64-apple-macosx27.2.0'"* and **`xcrun swiftc -sdk "$(xcrun
--show-sdk-path)"`** writing the same `libswp.a` (carried, `critic-briefs.md` finding 10): a member's build
reaches it through `xcrun` or names the SDK. A Mac with the CLT **alone** is an inference from
`DEVELOPER_DIR`'s selection, this Mac having Xcode too. And design.md already names *"Swift `@_cdecl`"*
among the producers of the C ABI Heroes targets (§1.11, `docs/design.md:499`), the record route (S) rests
on.

**Robust, as the brief poses it, the seats to correct**: **the primitives in Heroes, every platform the
same code**, AES-GCM, ChaCha20-Poly1305, Ed25519 and X25519 written in Rebel (decision 24's *"what reads
data from outside is written in Heroes"*), closing macOS's gap and Windows' Ed25519 with one
implementation, **under a constant-time discipline a tool checks on the emitted binary of every platform
the CI builds**, not on the Heroes source alone, since clang compiles the emitted C at `-O2` and the
source's absence of a secret branch is no promise about the machine code (the ffi-pragmatist's and the
historian's to source: what checks exist, dudect's statistics, valgrind's ctgrind-style taint, a
binary's branch census, and on which platform each runs). Each primitive held to Wycheproof and RFC
vectors on three platforms. **Conservative**: the gap kept (`unsupported` on macOS, Ed25519 on Windows),
written in each ledger, as 0.1 has it. **Between them, each a route a seat prices**: (S) a platform-layer
member in Swift over CryptoKit on macOS alone, the toolchain surface it changes, Windows' Ed25519 left
open; (C) the cipher modes over what CommonCrypto offers (AES-CTR there) with GHASH or Poly1305 written
beside it, the constant-time question then in the half written here; (V) **a route nobody listed in the
page**: a small constant-time C library vendored into the platform layer as C of the project's own
(the member is already C; decision 24 speaks of reading data, and whether a cipher is that is a
question), its licence and its audit the historian's.

**What would have to be true** for the robust route to hold: a check that tells a secret-dependent branch
or memory index in the emitted binary, run on each CI platform, and a compiler that keeps constant-time
source constant-time at `-O2` (or a lowering the compiler owns for it). Neither is measured.

**Route (H) already has a subject in the frozen tree** (the critic's finding 9, re-read by this writer):
Rebel's `crypto.hero:15-21`, *"There is no encryption here, no cipher and no signature: a cipher must
compute in a time that its key and its text do not decide, and nothing in this wave proves that of the C
the compiler makes. A digest needs no such proof; the comparison `equals_constant_time` is written with no
branch on the bytes, and says how far that goes."*; `crypto/internal/compare.hero:1-20`, its loop
`difference @ difference | to_i64(a[at]).must() ^ to_i64(b[at]).must()` with *"What the C compiler makes of
it is not proven here"*; `equals_constant_time` at `crypto.hero:567`, named in 10 files (`grep -rln`). Its
emitted C and machine code (a bounds check per index, a `.must()` per byte) are the first and cheapest
measurement of whether Heroes as emitted can hold the discipline, and a secret-dependent branch found
there is a defect of the frozen tree. **Two spec facts a primitive in Heroes must live with**: *"bitwise
& | ^ ~ << >> (i64 only; shift count 0..63 or it aborts)"* (spec line 210) and *"Overflow aborts at every
width; `<<` drops the bits it shifts out, and `>>` copies the sign"* (line 220), so a 32-bit add modulo
2^32 or a limb product is written inside `i64` with masks, and no wrapping operator exists. Rebel already
holds in Heroes `argon2`, `bcrypt`, `scrypt`, `blowfish`, `sha256`, `sha512`, `keccak`, `blake2`, `blake3`
(`ls crypto/internal`), the password hashes among them code whose timing an attacker may also read.

## Question 4: a skipped outcome for `heroes test`

**What a test can end as today**, measured with stage 2 on `bw/q4/t.hero` (three tests: one passes, one
asserts falsely, one prints `SKIP c: the network is down` and ends): `ok   "a: passes"`, the assert's
report and `FAIL "b: fails"`, then `SKIP c: the network is down` and **`ok   "c: would skip"`**, `3 tests,
1 failed`, exit 1. **A test that cannot run reads `ok`**: `heroes test` runs each test as a process and
reads only its exit (`selfhost/cli/verbs.hero:175-207`: `if one.started && one.code == 0` prints `ok`,
else `FAIL`, 376 lines). Spec § 12 says *"An `assert` failure shows the source expression, and both sides
of a comparison"* (369-370) and names no third outcome.

**What Rebel's tests did instead, counted** (`bw/tools/skips.py`, `skips_literal.py` and `live.py` over
`packages/rebel/0.1.0/files`, `bw/q4/`): **14,003 `test` blocks**; **55** print a line holding *skip* and
end as `ok` where what they need is absent: 54 found by a `print(` and *skip* on one line, in 17 files of
13 top-level packages (acme 19, mongodb 5, sql 5, amqp 4, nats 4, kafka 3, mqtt 3, ldap 2, redis 2, s3 2,
llm 2, jobs 2, http 1), and one more, `dns.hero:2133`, whose `print(` spans three lines, found by a second
pass over string literals holding *skip* (11 blocks, the other 10 data: a GraphQL `@skip`, a `skip_white`,
a PostgreSQL notice, read one by one, `bw/q4/rebel-skips-literal.txt`); most on a variable naming a server
(`REBEL_AMQP_URL`, `ACME_DIRECTORY`, ...), two on the network (`dns`, `http`); and **51 more, every one in
`mongodb.hero`, do nothing at all** where `REBEL_MONGODB_URL` is unset (`.err _ => _ = 0`,
`mongodb.hero:2379-2385` read), no line printed: **106 tests that read `ok` without running their
subject** on a machine with no service and no network. A block that skips by a `return` with no line
and no `.err` arm of that shape is not counted by either tool: the count is a floor. 42 distinct
`REBEL_*` variables name those services (`live.py`'s list). The harness's own outcome for the same situation is a named third one: *"A skip is a
third outcome, and it is NAMED (defect 246, 2026-10-04) ... printed, never counted as a pass. Whoever skips
also floors its skips ... so a machine that can judge nothing is a failure rather than a list of names"*
(`tests/harness/report.hero:15-21`, `:49-53`).

**The record**: no ruling on a test's outcomes beyond § 12's sentence found (`grep -rln -i` over
`docs/panel/*.md` and design.md for *skip outcome*, *t.Skip*, *#[ignore]*, *pytest.skip*, *test.{0,20}skip*:
panel 020's `FnKind::Test` *skipped* row is about the emitter and panel 226's *"no test skipped"* about the
thesis root; `gh issue list --search` for *skip test*, *test skip outcome*, *a test that cannot run*: the
harness defects 2069, 2530, 2559, none on `heroes test`).

**Robust** (`spec-skip-r.md`, **+59**): `skip(reason: "...")` ends a test there as skipped, never passed:
`heroes test` prints its title and reason, counts it apart (*"3 tests, 1 failed, 1 skipped"*), and fails
no run for it; only a `test` block calls it (refused elsewhere, a diagnostic). The test process tells the
runner by an outcome it cannot give by accident (an exit code reserved for it, or a word on a channel the
runner reads; the compiler-engineer's to choose and price, automake's exit status 77 a precedent for the
historian to source). **The name `skip` is taken by a built-in everywhere** (`error[builtin_name_taken]`,
panel 251's finding): `grep -rn -E '^ *function skip\('` over the tree, `heroes.packages` left out, prints
**7 declarations** in 7 files (`selfhost/parse/braces.hero`, `examples/markdown/inline.hero`, Rebel's
`io.hero`, `compress/lz4/internal/frames.hero`, `archive/internal/source.hero`,
`image/netpbm/internal/parse.hero`, `protobuf/internal/wire.hero`), and a rough grep of bindings named
`skip` prints 112 lines, unread one by one: renames CLAUDE.md § 2 allows before `1.0.0`, a cost the
compiler-engineer prices against another spelling (`skip_test`: `grep -rn -E '^ *function skip_test\('`
prints nothing). And whether a run where every test skipped is green (the harness's floor says not) is the
seats' to rule. **Conservative**: no form; a test that cannot run prints a skip line by convention and
reads `ok`, the convention written in Rebel's README and the 51 silent `mongodb` tests made to print, 0
spec tokens; what it keeps is 106 tests that pass where they tested nothing.

## Question 5: the whole stack of calls when a program aborts

**Item 34's words**: *"the whole stack of calls when a program aborts, which is the runtime's, joins
sitting C"*. **What the runtime prints today, measured** with stage 2 (`bw/q5/`, each binary under
`timeout -s KILL 20`):

- `deep.hero`, an index out of range in `pick`, called by `middle`, called by `outer`, called by `main`:
  `before`, then `panic: array index out of range` and `  at deep.hero:3:12, in deep.pick`, exit 134,
  the same built by `heroes build` (`-O0`) and run by `heroes run` (`-O2`);
- `deepmust.hero`, a `.must()` on an error two calls down: `panic: .must() on an error: bad: not a
  number: x` and `  at deepmust.hero:6:12, in deepmust.read_it`, exit 134, both levels;
- **`bw/q5/rebel/main.hero`, an abort inside Rebel** (`strings.replace_patterns_all` handed one
  replacement for two patterns, through the program's `swap_words`, built over a `path` entry): `assert
  failed: replacements.len() == patterns.texts.len()`, **`at <rebel>/strings.hero:921:12, in
  strings.replace_patterns_all`**, `left: 1`, `right: 2`, exit 134. **The program's own line, the call
  at `main.hero:6`, is named nowhere.**

So an abort names one place, the innermost, by the place the compiler passes (panel 230's R1, carried:
*"the library's own Heroes ... receive their caller's place, so an abort in them names the program's
call into the library ... `#[track_caller]`'s shape, for the library alone"*, `docs/panel/230-*.md:120-123`),
and a package is not *"the library"*: Rebel's place is Rebel's line. **The runtime already walks frames
for one stop**: the stack guard names the function a stack ran out in and a recursion's cycle
(`runtime/parts/stack.c:862-889`, `hero_stack_names`, frame records read up the chain, each name by
`hero_stack_symbol`, `:585-593`: Linux's own symbol table first, `dladdr` after), **POSIX only**: *"Windows
has no walk"* (`stack.c:858`), *"`SymFromAddr` needs dbghelp initialised before the fault and a PDB
beside the binary"* (`:1231`, `:1300-1301`). And the signal guard reads frame records, `HERO_SIGNAL_WALK
8` (`runtime/parts/sigguard.c:163`). The compile flags carry `-fno-optimize-sibling-calls`
(`selfhost/cli/flags.hero:150`) and **no `-fno-omit-frame-pointer`** (`flags.hero:133-153` read whole):
**the frame records at `-O2`, by this Mac's clang driver** (the critic's finding 8, re-run by this
writer: `clang -### -target <t> -O2 -c f.c`, `bw/q5/fp/`): `arm64-apple-macos` and `aarch64-linux-gnu`
**`-mframe-pointer=non-leaf`**, `x86_64-linux-gnu` and `x86_64-pc-windows-msvc` **`-mframe-pointer=none`**;
so a walk of frame records holds on both arm64 targets by the driver's default and finds none on x86-64
without the flag (its cost the compiler-engineer's to count). The images' and the CI's clang are other
builds, their defaults **unrun**. **Line tables are on every build already** (the critic's finding 7):
`debug_words` (`selfhost/cli/flags.hero:172-175`) gives `-O0` `-g` and every other level
`-gline-tables-only`, on every compile and link. **But on Darwin the binary holds none of them**:
`bw/q5/deep-O0`, built by stage 2, has no `__debug` section (`otool -l | grep -c __debug`: 0), a debug
map of two `N_OSO` entries naming the build's objects (`dsymutil -s | grep -c N_OSO`: 2), and no `.dSYM`
beside it (`ls`), so a reader of lines at run time on Darwin needs the build folder or a `dsymutil` step
the link does not take; on Linux the ELF carries them (by the flags; unrun here); on Windows the PDB the
link writes where its line carries a `-g` word (`flags.hero:210-213`).

**The spec**: *"An abort ends the program at once, saying why; no `T?` carries one."* (§ 6, line 183):
it does not promise where, and today's runtime says where.

**Robust, as posed**: every abort prints, beneath its place, the calls of the program that led there,
innermost first, each **with its `.hero` file and line**, on the three platforms (Windows through
dbghelp or a table of the compiler's own, never *"the failure without the function"*), at both levels,
from a fixed buffer and allocating nothing (panel 230's finding 6, carried), and the cost on a program's
happy path counted and reported (CLAUDE.md § Verification); the spec sentence `spec-stack-r.md`
(**+13**) if the spec-warden reads § 6 as owing it. The routes the seats price: **(W) a frame walk**
named by symbol, as the guard's, its lines from a table the compiler writes per call site or from the
line tables every build already carries (on Darwin in the build's objects, above); **(A) a route nobody
listed, the critic's**: the runtime writes the frames' return addresses alone, allocating nothing, and
`heroes run` and `heroes test`, which are the program's parent, resolve them against the binary and its
build's line tables, a binary run alone showing addresses; **(P) the caller's place passed to every call that can reach an abort**, panel
230's library route made general, which needs no walk and no symbols and costs every call; **(B) the
place of the program's call into a package** alone (the boundary, one frame), no walk. **Conservative**:
(B), or nothing, today's one place. What (B) gives up: an abort reached through two layers of Rebel names
the first Rebel call the program made, not the path inside.

## Question 6: one umbrella, one version, placed package by package

**The author's choice** (item 35, 2026-11-17): *"Rebel keeps ONE version, tested and published as one,
and a program places only the packages it uses with their Rebel dependencies; independent versions per
package are refused (the diamond conflicts of crates and npm, and the one vocabulary of codes and one
stream the library keeps). How it is spelled (`use`, `heroes add`) and how it meets panel 242's R2 and
panel 250's whole placement is sitting C's, with these measurements."*

**What stands**: spec § 1 (30-40): *"`use <json>/parse` reaches the module `parse` of the package `json`,
code others publish, which the program's catalogue `heroes.toml` ... names with its source, exact
version and `sum`, or by a local `path` alone, which nothing verifies. `heroes add json` writes that
entry ... a program holds one version of each package, every one listed in its catalogue"*. **Panel
250's R1** (`docs/panel/250-*.md:296-331`): *"the placement unit: W stays, every version placed whole"*,
M (the reached modules), G (by Go package, a face with its `internal/`) and H not adopted, on: no
placement meets panel 242's R2 ruler for the compiler (*"+120 code lines"*), M's cost *"+294 code lines
and +389 lines of compiler in 12 files"*, *"a correct `use` refused, 17 of 17 programs, where W refuses 0
of 17"*, *"placement a function of the order the verbs ran"*, and the ffi-pragmatist's **veto on placing
a package's C by a computed closure** (wrong constants at exit 0). **Panel 242's R2**: the compiler takes
a package only where its own tree shrinks or a measured defect closes. **`.claude/rules/cli-surface.md`'s
falsifier**: *"deleting the catalogue, deleting one of its entries, or mistyping any field of one never
changes which module compiles, only whether the program builds"*, and *"a second per-project file ...
a lock, a workspace or a configuration, is a fifth class"*, refused.

**Facts 250 did not have, each measured here**: (1) **Rebel ships no C**: `find
packages/rebel/0.1.0/files -type f ! -name '*.hero' ! -name '*.ledger'` prints nothing (1,639 files:
1,495 `.hero`, 144 `.ledger`); its platform C is the runtime's layer (`runtime/platform/`, 16 `.c` members by
`ls runtime/platform/*.c | wc -l`), so 250's veto on computing a package's C closure reaches no file of Rebel's; its `extern` groups (`grep -rh '^extern '`, 23 files) name the runtime's and the platform layer's headers and four of the system's, `math.h`, `sqlite3.h`, `string.h` and `stdlib.h`, none placed with the package. (2) **The compiler
never takes Rebel** (decision 6), so R2's ruler, which bound the compiler's taking, is not what Rebel's
placement must meet. (3) **Rebel is not in the index**: `packages/rebel/` holds `0.1.0/files` alone, no
`versions` and no `list` (`ls`), where `packages/core/` holds `versions` and `0.6.0/list`; Rebel's
examples reach it by a `path` entry (`examples-rebel/heroes.toml`: `path =
"../packages/rebel/0.1.0/files"`), which spec § 1 says *"nothing verifies"*.

**Item 35's measurements, re-run** (`bw/tools/closure.py`, `bw/q6/closures.txt`; a package is a face, a
`.hero` not under `internal/` and not `hero_*`, owning its ledger and every file under its folder no deeper
face owns; `containers/internal/` has no face and is counted as its own, the one such folder; lines are
`.hero` newlines, bytes every placed file). **Whole**: 149 faces, 1,639 files, **859,150 lines of
Heroes, 38,309,694 bytes** (item 35 read 313,990 lines in 841 files of 76 faced packages on 2026-11-17,
carried: Rebel has grown 2.7 times since). **The four programs item 35 measured are not recorded**
(`rebel-scratch/coordinator-todo.md:575-579` names them, *"JSON CLI 15% (45,537), grep-like 16%,
package manager 26% (82,716), web service 39% (120,906)"*, carried; `rebel-scratch/tools/closure2.py`
takes the package sets as arguments, and no file keeps the ones given), so these are this writer's:

| program | packages named | PACKAGE grain | MODULE grain |
|---|---|---|---|
| `examples-rebel/jq.hero`, a real program | its seven `use <rebel>/` lines | 14 packages, 221 files, 76,583 lines, **8.9%**, 3.6 MB | 208 modules, 76,583 lines |
| a CLI reading JSON | `json flags os io` | 8, 107 files, 36,646 lines, **4.3%**, 1.5 MB | 99 modules, 36,646 |
| a tool like grep | `regex fs flags os io strings` | 11, 167 files, 53,842 lines, **6.3%** | 156, 53,842 |
| the package manager | `http json toml semver hash archive compress fs os process flags crypto` | 24, 351 files, 114,010 lines, **13.3%**, 5.4 MB | 325, 113,359 |
| a web service on PostgreSQL | `web sql sql/postgres sql/migrate json log config template time` | 36, 467 files, 156,395 lines, **18.2%**, 7.2 MB | 431, 156,229 |

Each package alone, its own closure: smallest `math/libm` 315 lines, median 42,405 (4.9%), largest `mcp`
187,420 (40 packages, 21.8%) (`--per-package`, `bw/q6/perpkg.txt`). **The package grain and the module
grain differ by under 1% in lines in every row, and by 9.5 to 15.0% in bytes** (the ledgers and the files
no module reaches; the critic's finding 17, reproduced by `python3 -I` over `bw/q6/closures.txt`).

**Real programs, not invented sets** (the critic's finding 14): the 161 programs of `examples-rebel/`, each
measured by its own `use <rebel>/` lines (`closure.py --programs=tree/examples-rebel`,
`bw/q6/examples-rebel.txt`, one row per program in `examples-rebel-programs.txt`): **packages placed min
1, median 7, max 40; files min 5, median 104, max 513; lines of Heroes median 39,736, max 174,162; bytes
median 1,707,683 (4.5% of the version's 38,309,694), max 8,244,160 (21.5%)**; the module grain's bytes
median 1,107,095. Every one of them shares one `path` entry today (`examples-rebel/heroes.toml`).

**Robust, as posed** (`spec-umb-r.md`, **+134**): the umbrella is **one catalogue entry with one version
and a list of the packages it places**: *"An entry publishing several packages under one version, as
`rebel` does, places only those its `packages` names, with each one they use: `heroes add rebel/csv` adds
one, and a `use <rebel>/flags` it does not name is refused"*, the example gaining `[dependencies.rebel]`
with `packages = ["csv", "json"]`; `heroes fetch` places the listed packages and their Rebel closure, each
file held to the version's listing (250's F1, the listing's own `sum`); the refusal a diagnostic with a
`certain` fix naming the `heroes add`; the index publishes `rebel` once, with each package's files and
its Rebel dependencies in the version's listing. **What it must not break**: the falsifier (deleting a
name from `packages` changes only whether the program builds), *"no fifth input class"* (the list is a
field of the catalogue's entry, not a file), 250's *"a correct `use` refused"* (here the refusal is of a
package not listed, told with its fix, never of one listed), and 250's *"placement a function of the order
the verbs ran"* (here the list decides, not the order). **What the arm does not say, asked of the
compiler-engineer and the spec-warden** (the critic's finding 14): whether `packages` binds a **`path`
entry** too (then `examples-rebel/heroes.toml`, one `path` entry for 161 programs whose `use <rebel>/` lines
name 163 distinct paths by `grep -h -o | sort -u | wc -l`, lists every face, and the refusal checks nothing
for them) or a `path` entry is exempt (then the refusal is a property of a fetched entry alone, and every
build this brief ran was a `path` build). **Conservative**: W stands; Rebel is one entry
placed whole, `heroes add rebel`, `use <rebel>/<package>`, 0 spec tokens, every program carrying 38.3 MB
and 1,639 files. **Between them, each a route a seat prices**: (M') the placement computed from the
`use` lines at `add` and `fetch`, no list (250's M at the package grain; its 17-of-17 and order findings
are the price to re-measure); (E) each Rebel package its own index entry, all at one version the index
enforces (a spelling question: `use <rebel>/csv` names the entry `rebel` in today's grammar, `Use = "use"
[ "<" ident ">" "/" ] ...`, spec line 61).

## The seats, and why each sits

| seat | questions | why |
|---|---|---|
| compiler-engineer | 2, 4, 5, 6; 1 for the Zen's two homes and `library_source.hero` | the verb, its goldens and the checker's reading (Q2); the test runner's third outcome and the name (Q4); a walk or a passed place in the runtime and the emitter, and its cost (Q5); `add`, `fetch`, the catalogue's field and the refusal (Q6). Builds each route it would adopt (`SKILL.md:274-277`) |
| ffi-pragmatist | 1, 3, 5 | §1.11 is its charter, rewritten (Q1); the C boundary of the cryptography: the constant-time function already in the tree first, the Swift member, the C route, Windows' CNG (Q3); frame records and the frame walk's Linux arm64 rows, line tables, dbghelp (Q5) |
| spec-warden | 1, 2, 4, 5, 6 | every spec sentence and its payment, the spec's silence on a standard library (Q1), three new diagnostic classes (Q2's refusals, Q4's `skip` outside a test, Q6's unlisted package), the register of predictions before the blind run |
| historian (advisory) | all six | optional libraries beside a language (Q1), doc tools and examples as tests (Q2), constant-time code and its checkers, CryptoKit and swift-crypto (Q3), skipped tests (Q4), panic traces (Q5), umbrellas at one version (Q6) |
| llm-ergonomist (blind) | 2, 4, 6 | where a reader's prediction under a sentence can be measured: the doc shape (task t2), the skip outcome (t4), the umbrella's refusal (t6); `llm-ergonomist.md` |

**Not blind-read, and why**: Q1 changes design.md and no spec sentence, and the blind seat is never given
design.md (`.claude/agents/llm-ergonomist.md`); Q3 changes no sentence a reader sees; Q5's report is the
runtime's words, and a reader-fix task would hit a ceiling unless the program hides which call aborted
(the seat may design one and name its size, the coordinator decides).

## One sitting, six questions: the coordinator's decision

**The sitting stays whole** (the coordinator's decision (a) of 2026-11-19, after the critic's first
reading): the author ratified three sittings of this milestone, A, B and C (`docs/panel/251-*.md`, § The
author's verdict, carried), and with five agents at most on this Mac a fourth sitting means more seats run
in line, not fewer. The first writing of this brief recommended a split; it is withdrawn, and no seat
plans against one.

**What it costs, and how the clock is held**: panel 251 asked three questions and its compiler-engineer ran
*"about three hours, asked by the coordinator at 13:02 to close"* (`docs/panel/251-*.md`, § How the sitting
went, carried); here it is asked four that each change `selfhost/` or `runtime/`, each change a stage 2 of
130 to 240 s before its cases run (the writer's 136.05 s, the critic's 129.99, panel 251's critic 237.88,
carried). So the compiler-engineer works **in this order, each question's block written whole in its
report before the next begins**, so a seat stopped by its clock leaves whole blocks:

1. **the spec-facing halves first**, which the spec-warden's payment and the blind seat's rows wait on:
   question 4 (the name `skip`, its channel, a run of nothing but skips), question 2's refusals and its
   golden form, question 6's refusal and the `path` question;
2. then the rest of questions 2, 6 and 1's two homes;
3. **question 5 last**.

**And the stop point, cut in this order when the clock runs short** (the critic's order, `critic-briefs.md`
§ The split): (a) **question 5's frame walk (W) built on this Mac only**, its Linux arm64 rows left to the
ffi-pragmatist's question 5.1, which builds `deep.hero` in both images already, and (P) priced by
`--emit-c | wc -c` and one program's instructions retired, not by the compiler's own build; (b) **question
6's index publication and `fetch` costs**, after the refusal is built over a `path` entry (M' and E
priced, not built); (c) **question 2's widening to `heroes check`**, counted only if the verb is built.
Question 4 is the smallest and stays whole. The ffi-pragmatist's question 3 is the sitting's other long
pole; its first measurement is the constant-time function already in the tree (§ Question 3), which may
decide route (H) before any primitive is written.

## Where each seat works, and the rules every seat obeys

**Your folder is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-<seat>/`**
(inside the repository's root, which CLAUDE.md § Hard stops requires; ignored by git through
`.gitignore`'s `.claude/worktrees/`). **Make it yourself, from the frozen commit, and build nothing in the
tree**:

```
F=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-<seat>
mkdir -p $F/tmp && cd $F && export TMPDIR=$F/tmp
git -C /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-round archive 181555e9a32d0b18878f9f10e48d2eadd939c285 | tar -x -C $F
find $F -type f -not -path "$F/tmp/*" | wc -l         # 20,037 at 181555e9a; say what yours reads
printf 'gitdir: /nonexistent-253-<seat>\n' > .git    # so the hooks judge your files in your copy
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes          # 10.07 s for the writer
./heroes build selfhost/main.hero -o heroes-next                    # 136 s for the writer at load 11
./heroes-next --version                                             # sources 23c7d2a626b29fd5
```

`./heroes-next measure <file>` reads its tables from the copy's root, so run it from there with a path
to the file (`error: cannot read vendor/tokenizers/claude-legacy.json` otherwise, read). The `.git` file
makes some of the compiler's own tests fail in a copy (carried, `docs/panel/249-*.md:413-419`); **no seat
runs `heroes test` on the compiler or a whole package**. Rules, each from `SKILL.md` § 3 and 3c or
CLAUDE.md:

- **Your folder and your report, nothing else.** Your report is
  `.claude/worktrees/rebel-round/docs/panel/253-reports/<seat>.md`, **written as you go**, in your
  definition's output structure (`.claude/agents/<seat>.md`), one block per question; make the folder if
  it is absent. Never the tree's other files, another seat's folder, a lane, `bw/` (read-only), the blind
  area `253-llm-ergonomist/` (read-only, and never run in).
- **The Mac is shared** (the coordinator's rule for this sitting, `brief-writer-common.md`): at most one
  build or test run of yours at a time, never a whole package's `heroes test`; load read 11 to 18 while
  this was written and 5.5 to 14.5 while it was repaired (`uptime`).
- **No wall clock** (the coordinator's decision (b), on the critic's finding 13): no seat takes a seconds
  measurement while five agents share the Mac, since CLAUDE.md § Verification says *"while a clock runs the
  machine stays still"* and a `real` taken under this load is discarded by its own ratio rule. Every cost
  is asked in **instructions retired and bytes**: `/usr/bin/time -l <command>` prints `instructions
  retired` on this macOS (`/usr/bin/time -l /usr/bin/true`: 7,875,664 for the writer, 9,503,315 for the
  critic, a count the load moves far less than a clock), best of N stated, the first discarded; `ls -l`,
  `wc -c` and `du -k` for bytes. A seconds figure a seat wants is named **owed to a still machine**. The
  stage 2 times above are the writer's and the critic's, given to plan a clock, never as a measurement of a
  route. **The Windows box is busy and not yours**: a Windows fact a seat needs is
  named **owed** in its report.
- **No file outside the repository's root**: no `/tmp`, nothing under the home directory. `swiftc`,
  `go` and `lldb` only with `HOME`, `TMPDIR` (and Go's `GOPATH`, `GOCACHE`, `GOMODCACHE`, `GOTMPDIR`)
  inside your folder, exported before every command (the writer's `swiftc` ran so).
- **No paid run but the spec-warden's named refreshes and the coordinator's `run.sh`.** **Never print a
  credential**: `gh auth status` at most, never `gh auth token`. No GitHub write, no commit, no push, no
  tag; `gh issue view`, `gh run view` and a `gh api` GET are reads.
- **A program that may not end** is built with `heroes build` and its **binary** run under `timeout -s
  KILL <seconds>`, never `timeout heroes run`; its output to `/dev/null` or through `head -c 65536` into
  a file; `pgrep -fl <your folder>` read after, anything left stopped (`.claude/rules/verification.md`
  § A run that may not end). Bounds: a case 20 s, a program 60 s, a stage 2 1800 s.
- **Docker**: `heroes-linux-arm64:latest` (native) and `heroes-linux:latest` (amd64, emulated, not the
  CI's leg), fed the tree by `git archive` on stdin, only a folder of yours mounted, one container of
  yours at a time, named `<prefix>-253-<n>` (`ce`, `fp`, `sw`), under an outer `timeout -s KILL`, then
  `docker ps --filter name=<prefix>-253-<n>` read and stopped where it prints one.
- **The route you would adopt, you build in your folder and run on the cases you name, before the
  synthesis** (`SKILL.md:274-277`); a route that does not build is reported as not built.
- **Every number with its command; every negative sentence run, or posed as a question**; *only*,
  *never*, *provably* with what was searched and where (CL-077, `/panel` step 2). A claim about another
  language from memory is a question for the historian.
- **The machine**: `pmset -g assertions` read `PreventUserIdleSystemSleep 1` at 14:21, held by
  Amphetamine's *"Single-Use - System"* (pid 750); a run past a minute needs nothing more, and
  `caffeinate -i -w <pid>` beside it costs nothing.

## The blind seat, in one paragraph

Three tasks, two arms each, five replicates: **30 graded sessions and one verdict session**, run by the
coordinator alone by `run.sh` in `.claude/worktrees/rebel-scratch/253-llm-ergonomist/`, **only after the
spec-warden's report holds its registered predictions under the heading `## Predictions registered before
the blind run`, with at least one non-empty line under it**: the runner refuses to start otherwise, copies
the report as it stands to `warden-at-start.md` and logs the copy's hash. Four repairs on every session
(standard input `/dev/null`, an empty git repository of the area's own, `HOME` inside the area, `TMPDIR`
and `CLAUDE_CODE_TMPDIR` inside the area), a free preflight session whose key the API refuses before any
spend, and `tools/scan.py` reading only what reached a session. Its design, cells, grading, decision
rules and cost are `llm-ergonomist.md`.

## Repairs after the critic's first reading

The critic's first reading (`docs/panel/253-reports/critic-briefs.md`, 426 lines, 14:55 to 15:13) found 17
things; every number it re-ran beside them held (its § What held). **The coordinator's decisions of
2026-11-19 on it, written in as decisions**: **(a)** the sitting stays whole, six questions (§ One sitting,
six questions), the split sentences struck, the compiler-engineer given an order and a stop point by the
critic's cut order; **(b)** no seat takes a wall-clock measurement while five agents share the Mac:
instructions retired and bytes (§ Where each seat works); **(c)** the runner's temporary directory, a free
preflight that lists `/private/tmp/claude-<uid>/` before and after, `scan.py` reading only what reached a
session with its control run on panel 251's transcripts, and a predictions gate that copies, hashes and
refuses an empty heading; **(d)** t6 redrawn over packages the arm's example does not name, t4 worded
neutrally, the labels drawn again; **(e)** this section. The writer's stop by the usage limit cut nothing
already written: every file was re-read and every count below re-run after it (15:39). One line per
finding:

1. **t6 was the `umb-r` arm's own example**: the task now names `yaml` and `template` at version `0.4.0`
   and adds `time` with a call (`tools/task-t6.md`, sha256 `9c2f39400c2fad28`), none of which the arm's
   example or sentence names (`grep -c -E 'csv|flags|0\.1\.0'` of the task: 0); the arm is unchanged, so
   unmeasured again; its reference rebuilt over a `path` entry (`bw/ref/t6b/`: builds, prints *"Ziggy sang
   on Low"*, and builds again with `use <rebel>/time` and `time.now()`); folders remade and re-registered.
2. **t4 forbade the hazard its row counted**: the task now says only that where no server is named *"the
   test cannot check the connection"* (`task-t4.md`, `3200048e07a92841`); `now` is right where its
   prediction matches its own run, the silent passes are the hazard's count, and a third rule asks whether
   `skip-r` lowers that count by at least 3 (`llm-ergonomist.md`).
3. **t2's `needed` was decided by its own words**: the task now asks for what the reader sees (what it
   answers, each parameter, an example) and names no form (`task-t2.md`, `2fcb237358b4530c`), so `now`
   measures a reader's default and `needed` becomes a measurement.
4. **`scan.py` voided sessions for their own answers**: it skips `assistant` entries, request blobs of role
   `assistant` and `toolUseResult`; **its control on panel 251's 51 transcripts flags 51 `prompt-alone=NO`,
   51 `git-status=LEAK` and 0 `claude-md-or-memory=YES`** (the old reading's two, `t1-x-4` and `t3-c-2`,
   reproduced and gone; `bw/scan-control-251.txt`); the coordinator's probe reads `NO` (its prompt *"Say
   hi."*), `reader, no commit`, `no`; a clean transcript of the real prompt is the preflight's to make.
5. **The sessions could write to `/private/tmp/claude-501/`**: every session takes `TMPDIR` and
   `CLAUDE_CODE_TMPDIR` set to the area's `tmp/`; **whether they move the `cache-break-state` file is
   unrun**; the runner's free preflight (a key the API refuses) lists the folder before and after and stops
   on a file naming its session, and the runner stops after any graded session that leaves one;
   `check_area.py` asks the two variables and `tmp/`.
6. **The gate hashed a growing file and read a heading**: the runner copies the spec-warden's report to
   `warden-at-start.md`, logs the copy's sha256, refuses a heading with nothing under it, and the rules
   score only the copy.
7. **Q5's line tables**: every build carries them (`flags.hero:172-175`); on Darwin they stay in the
   build's objects (re-run: no `__debug` section, two `N_OSO`, no `.dSYM`); § Question 5, the
   compiler-engineer's 5.1 and the ffi-pragmatist's 5.2 say so, and the critic's route (A), addresses
   resolved by the parent, is priced beside W, P and B.
8. **Darwin's frame records from memory**: replaced by the driver's answer, re-run (`non-leaf` on both
   arm64 targets, `none` on x86-64 Linux and Windows); the images' and the CI's clang stay unrun.
9. **Q3's route H had a subject in the tree**: `crypto.equals_constant_time` and `crypto.hero:15-21`, and
   the spec's `i64`-only bitwise operators and aborting overflow, in § Question 3; the ffi-pragmatist's 3.2
   reads that function first.
10. **`swiftc` in the Command Line Tools**: settled on this Mac (the critic's build, carried; the writer's
    `ls`), the member's build through `xcrun` or a named SDK; struck from the questions; the historian asked
    for Apple's page.
11. **The register was a word search**: enumerated from milestone M-core-packages (231 issues, 19
    decisions, 39 open) and the 17 `examples/` catalogues naming `core`, both handed to the spec-warden as
    files (`bw/q1/`).
12. **§ A7's net check and the 662**: no file performs the check (said in § Question 2, the conservative's
    *"harness check"* now one to write); the census script repaired for `->` and re-run: 4,568 right, **642**
    mismatched, 626 of them with no parameter line.
13. **Clocks under five agents**: decision (b), in § Where each seat works and in each brief that asked a
    clock (the compiler-engineer's 2.2, 5.2 and 6.3); a timing-based constant-time test is owed to a still
    machine (`ffi-pragmatist.md`).
14. **`packages` and a `path` entry**: asked of the compiler-engineer (its question 6.6) and the
    spec-warden (its question 5); the 161 real programs of `examples-rebel/` measured (median 7 packages,
    1.7 MB, 4.5% of the version; max 40, 8.2 MB, 21.5%).
15. **The verdict's cap**: 0.60 for the verdict alone, the stop rule taking each folder's own cap.
16. **10 USD holds**: recorded, 5.24 USD at panel 251's median and 6.71 at its maximum; the grading reads
    `result.md`, since panel 251's 51 sessions wrote no `report.md`.
17. **Smaller facts**: t6's other verbs recorded and not graded, t2's first sentence a rule (a period on
    the comment's first line), t4's given lines byte for byte; the grains' bytes differ 9.5 to 15.0%;
    2315 a feature; `hero_panic_where`'s body at `place.c:100`; the split sentences struck (decision (a));
    the count_tokens premise unchanged, the critic finding it consistent; the ffi-pragmatist's
    `layer.hero` premise put as a question with design.md:499's *"Swift `@_cdecl`"*; the historian's
    counts re-run (Deno in 12 reports, Phobos at 212 and 227).

## Questions no one has run, posed as questions

- Does `heroes doc` over Rebel's 149 faces print the same bytes twice, on two platforms, and how long does
  it take on the largest face?
- Do the 642 functions whose parameter lines disagree with their signature by a reader of lines disagree
  by the compiler's parse too, or is the reader wrong?
- Does the Swift member build inside the platform layer's archive and link into a program the linker takes
  it from, under `--sanitize`, at both levels, and on `macos-26`?
- Is there a tool that checks constant time on the binary clang emits from Heroes, on macOS, Linux and
  Windows, and what does it cost per primitive?
- Does Windows' CNG answer X25519, and in which byte order?
- Which exit code or channel can a test process use for a skip that no assert, abort, signal or exit of
  the program can produce?
- Does the Linux images' and the CI's clang keep frame records at `-O2` as this Mac's driver says for
  `aarch64-linux-gnu` and not for `x86_64-linux-gnu`, and what does an inlined call show?
- What does passing a caller's place to every call cost a program's happy path, in instructions retired?
- Does `CLAUDE_CODE_TMPDIR` keep the CLI's `cache-break-state` file inside the blind area (the runner's
  preflight asks it)?
- Under the umbrella's `packages`, what does `heroes fetch` place against W for the 161 programs of
  `examples-rebel/` (files, bytes, instructions), and does the placed subset hold to the version's listing
  at every build? Does `packages` bind a `path` entry?
- Does a `use <rebel>/x` of an unlisted package reach any module today through a `path` entry, so that the
  refusal would change what a `path` build accepts?
- What do Go, Rust, Python, Zig, Swift, Deno and Boost do at each question (the historian's brief)?
- Whether every blind session, run with the three repairs, has a transcript `tools/scan.py` reads clean.

Written by the coordinator's brief-writer, its first clock reading 14:21 and its last 14:54 CET on
2026-11-19 (`date`); repaired after the critic's first reading from 15:14, stopped by the usage limit and
resumed at 15:39, finished at 15:49 CET (`date`). No seat had started, and no paid call was made.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
