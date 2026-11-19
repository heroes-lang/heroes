# Panel 253: design.md is rewritten for Rebel, `heroes doc` enters, a test can skip, an umbrella places what it lists, and macOS's cryptography and the stack at an abort wait

Sitting C of M-rebel-library, convened 2026-11-19 by its coordinator (the milestone page's item 33, items 34
and 35 joined to it). **The full panel**, six questions in one sitting (the coordinator's decision (a),
`docs/panel/253-briefs/00-shared.md` § One sitting, six questions): the compiler-engineer (Q2, Q4, Q5, Q6
and Q1's compiler part), the ffi-pragmatist (Q1, Q3, Q5), the spec-warden (Q1, Q2, Q4, Q5, Q6, and Q3's
token count), the historian (advisory, all six) and the llm-ergonomist (the blind seat, Q2, Q4 and Q6: 30
graded paid sessions and one verdict session, 5.335998 USD of the 10 the author approved), with the
completeness critic reading the briefs before any seat and the reports after. The frozen commit:
`181555e9a32d0b18878f9f10e48d2eadd939c285` of `rebel-round`. Briefs: `docs/panel/253-briefs/`
(`00-shared.md` and the five seats'); reports: `docs/panel/253-reports/` (`critic-briefs.md`, the five
seats, `critic-reports.md`).

**Status: ratified as proposed.** The author's word, given at about 13:10 on 2026-11-19, before this
sitting's briefs were written, meant as: *OK to the ratifications as proposed*, of sittings A, B and C of
M-rebel-library. So this sitting's resolutions are ratified in the words this file proposes them, by that
advance word, and its decision issue is opened and closed together (`.claude/rules/records.md` § The
issues). It is recorded as given: before the synthesis existed (CL-058). One question stays open by R3's
own words, whether decision 24 admits a vendored C library for cryptography, and goes to the author as an
open decision of its own with the coordinator's recommendation.

## The proposal, verbatim

The six questions as `docs/panel/253-briefs/00-shared.md:82-593` put them to the seats, each with its
record, what was measured for the brief, and its robust and conservative resolutions, copied by script,
unedited:

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

## The verdicts

Each from its seat's report, the critic's re-runs beside it where it ran one; *did not sit* where the brief
gave the seat no such question (`00-shared.md` § The seats, and why each sits).

| seat | Q1, design.md §1.11 | Q2, `heroes doc` | Q3, macOS's cryptography | Q4, a skipped outcome | Q5, the stack at an abort | Q6, the umbrella |
|---|---|---|---|---|---|---|
| compiler-engineer | **approve** (its part): law 18 stands in four places, `doctor.hero:254`, its test at `:287`, `seed/heroes.c:365` and both editions' `project.html:95`; rewording it is two lines of `doctor.hero`, the seed's copy left to the next refresh, a window of two compilers printing two laws that no suite asks about; `library_source.hero` untouched by decisions 6 and 9 | **approve the verb, object to the arm's three refusals as the whole rule**: built (`ce-q26`, three modules, 511 lines, 350 of code), every stage of `check` run and the call labelled over the checker's types; over Rebel's 149 faces, 142 references and 7 refused (18 `@` lines, 1 constant's example, 0 order), 149 of 149 `cmp`-equal over two runs; the three miss the 661 functions with no parameter line | did not sit | **object to `skip`, approve `skip_test(reason:)` with an all-skipped run failing**: built (`ce-q4`), the runner's own file the channel, no exit, print, abort or assert forging it; `skip` taken refuses 45 sites, 9 of them declarations; selfhost +43 and a new module of 39, runtime +34 | **approve (W) with (A), object to (P)**: (W) built on this Mac, runtime only (`stack.c` +64, `panic.c` +2, +756 bytes of `__text`, 0 happy-path instructions by construction); (P) about four records per abort site (3,245 calls against 791 sites in `examples/interpreter`'s C); (B) redundant beside (W) | **approve the list over a `path` entry and a fetched one alike, reading (a)**; object to the arm's `certain` fix (panel 235's R5), to its silence on 16 `hero_*` modules, and to `fetch`'s closure written as small (about 150 lines, priced); built: the refusal (about 170 lines in 10 files) and `heroes add <entry>/<path>` |
| ffi-pragmatist | **approve the robust arm with four repairs, object to the conservative, veto reserved** for any wording of decision 24 that reaches a program's own binding; Rebel's `sql/sqlite` held to `sqlite3.h` (a result one width narrow refused `ffi_return_type`); *"nothing in the compiler or the runtime grows with Rebel"* false (39,110 lines of `runtime/` since step 3) | did not sit | **object to (S), object to (H) today, approve (V) as the route to build**, provisional on its licence and audit: CryptoKit's Ed25519 is not deterministic, its header is checked only when compiled as Objective-C, and a Swift member costs every Mac's cold build 9% (warm module cache) to 84% (cold) more instructions; `equals_constant_time` clean at both levels; a ChaCha20 in Heroes branches on secret sums unless masked, and `>>` branches on a secret sign at `-O0` | did not sit | **approve (W) with (A) on three repairs, object to (P)**: (W) skips the frame that aborts; `dladdr` waits on glibc 2.41 while another thread is in `dlopen`; x86-64 keeps no frame record at `-O2` without `-fno-omit-frame-pointer`; the whole inline chain is in the line tables on both POSIX platforms | did not sit |
| spec-warden | **approve `design-r` at 0 spec tokens, object to `design-c`**; the register proposed as one `decision` issue with dated comments, never a file of `docs/`; CLAUDE.md:279's words drafted (+26 vendored) | **object to the arm's refusal clause, adopt `doc-w`**, provisional on t2: `doc-r` +145 real, `doc-w` +1 vendored over it; paid by P-t2-a and P-t2-b | 0 spec tokens on every route; a wrapping operator, a narrower bitwise width or a constant-time mark would be a new form owing its own sitting | **approve the form in § 12, object to *"fails no run for it"*, adopt `skip-v2`**: `skip-r` +74 real, `skip-v2` -3 vendored against it; paid by P-t4-a to P-t4-c | **veto the sentence** (+14 real): false on landing on Windows and on x86-64 at `-O2`, and panel 230 ruled this § 6 sentence at zero tokens; approve the runtime's work at 0 tokens | **object to `umb-r`, approve `umb-w`**, provisional on t6: `umb-r` +166 real, `umb-w` -4 vendored against it, a hypothetical package `kit` and `path` bound too |
| historian (advisory) | **approve the robust arm** with the ffi-pragmatist's repairs; DMD keeps the compiler off Phobos (decision 6 reached first), PEP 594, the Haskell Platform retired, the Rust Platform refused | **approve the verb**; `go doc`, Go's examples named for their function, D's documented unittests; Javadoc errors on a bad `@param` name and warns on a missing one, so refusing a missing line departs from every tool read | **approve (S) now** (.NET's commit, Apple's DTS), **(H) only behind an enabler** (a wrapping operator, overflow checks proved away, constant-time operations the compiler owns), **object to ciphers in Heroes under today's emission**; no small library read holds all four primitives | **approve** a named third outcome, the runner's channel over an exit code, an all-skipped run failing (Catch2), the name `skip_test` (Python's `skipTest`) | **approve (W) with (A), object to (P) made general** (Rust's RFC 2091 declined it for binary size) | **approve one version and a list** (Maven's and Compose's bills of materials, Cargo's features); **object to (M')** (Go's vendoring, Boost's `bcp`) |
| llm-ergonomist (blind) | did not sit (design.md is never handed to it) | t2: `doc-r` 5 of 5, `now` 0 of 5: **suffices and needed**; every `now` reader already writes the parameter list, and every one puts its example in a code fence inside the comment | did not sit | t4: `skip-r` 5 of 5 by the arm's words; `now` 5 of 5 silent passes against `skip-r`'s 0: **suffices and reduces the hazard** | did not sit | t6: `umb-r` 5 of 5, each predicting `use <rebel>/time` refused and naming `heroes add rebel/time`; `now` 5 of 5 predict it accepted: **suffices** |

The blind seat's verdict session read the three arms together (`all-r`) and answered *"**Approve.**"*; no
seat adopts those three arms word for word (`doc-w`, `skip-v2`, `umb-w`), so it is a reading of sentences
that will not land as read (`critic-reports.md` finding 19, item 4).

## The critic's second reading, what it changed

Nineteen findings (`critic-reports.md` § Tally), thirteen of them named in its summary as changing what the
synthesis may adopt. Each was settled by a command where one could settle it:

1. **(W) as built drops the frame that aborts at `-O2`** (finding 1). On the compiler-engineer's own
   `deep-O2`, inspected only, `main` calls `outer`, into which `pick` and `middle` are inlined, and `xcrun
   atos -i` names `deep.hero:3`, `:6`, `:10` and `:14`: every call is in the line tables, and the
   compiler-engineer's *"an inlined frame at `-O2` is lost"* is the walk's own skip, as the ffi-pragmatist
   read it. **(W) is not adopted as built** (R5).
2. **`dladdr` waits on glibc 2.41** (finding 15), reproduced in the Linux arm64 image (exit 3 after 5 s),
   so the walk reads its base on Linux without it; the frozen stack guard makes the same call inside a
   signal handler, which nobody probed (R5, a task filed).
3. **(S) may not be adopted on the .NET precedent** (finding 3). CryptoKit's Ed25519, run again: two
   signatures of one message differ and neither is RFC 8032's, which makes Rebel's `keypairs.hero:96`
   false on macOS and fails its test at `:485`; .NET's member exports the two AEADs and no Ed25519; and a
   route nobody listed: the AEADs through CryptoKit, Ed25519 and X25519 through a C library (R3).
4. **Only the conservative is built for Q3** (finding 4): (S) ran as a library against vectors and never
   as a member of the layer; (V) is two libraries, Monocypher having no AES and BearSSL unsourced; and
   whether decision 24 admits it is the author's reading to give, not a seat's (R3).
5. **The `@` ruling is owed** (finding 5): 18 parameter lines of Rebel write `@r`, and no arm says whether
   that names `r`; it decides whether the compiler-engineer's 18 refusals stand (R2 rules `r`).
6. **The verb as built prints none of the five `doc-r` readers' examples** (finding 6), since it takes
   examples under `## Examples` alone, which decision 22 asks and neither measured sentence says; and it
   prints a lone comment above a module's first function twice (R2: an example wherever it stands).
7. **`doc-w`'s refusal of a missing line is unbuilt and stops 16 faces whole, 661 functions** (finding 7),
   `strings`, `bytes`, `path`, `lists` and `fs` among them; 661 is the compiler's count over 142 faces,
   642 the census's; Javadoc's line (refuse a wrong name, tell a missing one elsewhere) was the unpriced
   route (R2 refuses the missing line and records Javadoc's split).
8. **The floor's reason is false of a file with no test**, which exits 0 today (finding 10), and no Rebel
   file is all skips, so the floor turns none of them red (R4 keeps a file with no test at exit 0 until its
   census is taken).
9. **The grain of a name in `packages` was undecided, and the seats assumed two** (finding 12): the 161
   programs name 163 paths, 16 of them `hero_*` modules and 68 nested, whose first parts are 97 names; the
   spec-warden predicted first parts, the prototype lists paths (R6 rules the whole path).
10. **`design-r` is not the text to land** (finding 13): the ffi-pragmatist's three false sentences, run
    again, and a fourth, its core paragraph grounded on panel 234's R1 and R2, which the spec-warden's
    register invalidates; 30 of design.md's 31 lines naming `core` kept (R1).
11. **Decision 8 reaches 66 catalogues, not 17** (finding 17): the 17 of `examples/`,
    `tests/harness/heroes.toml`, `.claude/hooks/heroes.toml` and 47 golden cases; 116 files of
    `tests/harness/` and 3 of `.claude/hooks/` `use <core>/` (R1's register).
12. **What is built and what only priced, per question** (finding 16): the right-hand column lands only
    after its own build and run, which each resolution below names as owed.
13. **The `>>` branch at `-O0` is the frozen tree's and breaks no written promise** (finding 2):
    reproduced on a one-function program (`tbz x8, #0x3f` at `-O0`, `asr` at `-O2`), reaching SHA-384 and
    SHA-512, BLAKE2b and so Argon2, every SHA-3 and SHAKE, scrypt and `random`'s next key; filed as an
    `improvement`, the first prerequisite of route (H).

And of the rest: the blind grading holds cell by cell, P-t2-c false as scored, no session voided (finding
8); t2's *needed* was near-certain from the title rule, and the measurement it bought is the fence, five of
five `now` readers writing the example as a fence inside the comment, the form panel 136 refused; read
symmetrically, `skip-r` is 4 of 5 (`t4-f-3` does not build, `error[expected_arm_arrow]`), still sufficient,
the hazard rule unchanged. `CLAUDE_CODE_TMPDIR` moved the CLI's `cache-break-state` file inside the blind
area (finding 9). The name `skip` is declared 11 times, two of them in frozen corpus files no rename may
touch (finding 11). Every *instructions retired* figure counts the process `/usr/bin/time` starts and none
of its children (finding 14). Every premise a seat called false was judged right but the
compiler-engineer's reading of the inlined frame; the historian's five were not re-read (finding 18). And
nine smaller things (finding 19): 12 more tests that return in silence where a primitive is unavailable
(`crypto/seal` 6, `crypto/keypairs` 5, `web` 1), the spec's catalogue example pinned twice
(`selfhost/module/catalogue.hero:483`, `tests/harness/suite_spec.hero:1438`), the stopping rule owing a
sentence for a third verb entered by decision, and the ffi-pragmatist's veto reserved, not spent.

## Where the seats disagree, stated plainly

- **Q1, the text and the seed.** Every seat that read `design-r` approves its direction; the ffi-pragmatist
  and the critic find it false in four places, so the direction is unanimous and the text is not. The
  compiler-engineer would leave law 18's copy in the seed to the next refresh, two compilers printing two
  laws meanwhile; R1 refreshes the seed in the same change.
- **Q2, what the verb refuses.** The spec-warden: the arm's refusals meet none of the census's 642, and a
  missing line must be refused (`doc-w`). The compiler-engineer: the three it built are the wrong three,
  the missing line is the commonest departure, and its refusal is unbuilt. The historian: no tool read
  refuses a missing line as an error (Javadoc warns, rustdoc allows by default), so refusing it is a
  departure to state. The critic: Javadoc's split was the route nobody priced. **R2 refuses the missing
  line.** On the example's place, the verb took `## Examples` alone and the spec-warden recommended
  anywhere; R2 rules anywhere. An example whose title names no function of the module the
  compiler-engineer built as `doc_example_unknown`; the spec-warden recommended not building it, such a test
  being no example by the sentence's own definition.
- **Q3, the route.** The historian approves (S) now on .NET's precedent and Apple's DTS; the ffi-pragmatist
  objects on three measurements the historian's report does not answer (a randomised Ed25519, a header
  clang checks only as Objective-C, the cost in every Mac's build), and the critic finds .NET's member
  holds no Ed25519. On (H), both object today. On (V), the ffi-pragmatist approves, and the historian
  finds no one small library holding all four primitives.
- **Q4, the name and the floor.** The blind run measured `skip`; the compiler-engineer objects to it (45
  sites, 47 with the frozen corpus by the critic's count) and the historian prefers `skip_test`. The arm
  said *"fails no run for it"*, which the blind run graded (status 0); the spec-warden, the
  compiler-engineer and the historian each make an all-skipped run fail, which no reading measured; and a
  file with no test exits 0 today, half of Catch2's rule.
- **Q5.** No seat defends the sentence. On the walk, the compiler-engineer and the ffi-pragmatist read an
  inlined frame differently; the critic settled it for the ffi-pragmatist on the compiler-engineer's own
  binary.
- **Q6, the grain.** The spec-warden reads a name in `packages` as a first part (97 names); the
  compiler-engineer's prototype lists paths, faces and `hero_*` modules alike (163). R6 rules the whole
  path. Both bind a `path` entry, reading (a); the compiler-engineer's own condition says a catalogue
  shared by 161 programs makes the check empty for each of them. Nobody defends (M') or (E).

## The resolutions

### R1, Q1: design.md is rewritten wherever M-rebel-library's decisions make it false; not as `design-r` wrote it

**Adopted: the robust route**, design.md rewritten wherever M-rebel-library's decisions make it false, at
**0 spec tokens** (design.md §1.11, its box and Part 2's bullet among them). **Not `design-r.md` as
written**: the ffi-pragmatist's three false sentences, *"nothing in the compiler or the runtime grows with
Rebel"* among them (39,110 lines of `runtime/` added since M-rebel-library's step 3), line 573 kept against
the new box's own rule, and the table's preface, false on its own SQL row; its core paragraph resting on
panel 234's R1 and R2, which the register invalidates; and 30 of design.md's 31 lines naming `core` kept
(the critic's finding 13, its summary's item 10). **The landing writes the text fresh** with the
ffi-pragmatist's four repairs (the leverage sentence, line 573, the table's preface, and a clause in the box
naming the FFI as every package's door to the platform), and **the ffi-pragmatist's reserved veto stands**
against any wording of decision 24 that would stop a program binding a C library itself.

**The register of invalidated decisions** (decision 10) is **one `decision` issue with dated comments**,
not a file of `docs/` (the spec-warden). Decision 8 reaches **66 catalogues**: the 17 programs of
`examples/`, `tests/harness`, `.claude/hooks` and 47 golden cases, with 116 harness files and 3 hooks using
`core` 0.6.0 (the critic's finding 17, its summary's item 11); **the register names each of them before
`packages/core` goes**.

**Zen law 18's two lines** in `selfhost/cli/doctor.hero` (the compiler-engineer's reading: the string and
its test) **land in a commit of their own, without a refresh of the seed**: the seed is regenerated only
where the two-stage build goes red or a release closes (the author's rhythm of 2026-10-14,
`.claude/rules/verification.md` § The batch), and a law of the Zen printed by the old seed's compiler is no
question of robustness, so the rhythm holds; the window in which the clang-built compiler prints the old
law until the next refresh is recorded, as the compiler-engineer measured it. *Corrected by the
coordinator before the commit*: the resolutions first said the seed would be refreshed in the same change,
which went against that rhythm. The site's law-18 and *"no standard library"* lines and
`.claude/agents/ffi-pragmatist.md:3` are rewritten by the landing with design.md. **CLAUDE.md:279's
words** (the spec-warden's draft, +26 vendored on CLAUDE.md) land with the landing, under the author's
decision 14 of 2026-11-15 (new rules in `CLAUDE.md`).

**What conservative would have been**: only §1.11's last sentence and Part 2's bullet (`design-c`, +88
vendored on design.md), leaving false sentences standing; refused.

### R2, Q2: `heroes doc`, the seventeenth verb, and its sentence in spec § 1

**Adopted: the seventeenth verb**, reading the compiler's own parse and the checker's types, the same
bytes on every run (built and run twice over Rebel's 149 faces, byte-identical: the compiler-engineer), and
**its sentence in spec § 1**: `doc-r` as the blind seat read it (suffices 5 of 5, needed 5 against 0), with
the spec-warden's `doc-w` wording of the refusal (+1 vendored over `doc-r`; `doc-r` +145 real):

```
- `#` comments to end of line, markdown inside. A comment directly above a
  declaration, a field or a case documents it: a first sentence that stands
  alone, then, after a line holding `#` alone, one `` - `name`: `` line per
  parameter in the signature's order. A `test` with a comment directly above
  it and a title opening with a function's name and a colon is that
  function's example. `heroes doc` prints a module's declarations, what
  documents them and their examples, and refuses a comment whose lines do
  not name each parameter once, in order. `##` is a section heading.
```

**The verb is repaired so it prints what the sentence promises**: the documented `test` titled `<name>:`
is the function's example **wherever it stands**, not only under `## Examples` (the critic's finding 6:
the verb as built printed none of the five `doc-r` readers' examples). **The refusals**: a parameter line
naming no parameter, or naming one out of order, refused; **a missing parameter line refused too** (the
words *"a comment whose lines do not name each parameter once, in order"*), since the author's decision 22
asks for each parameter in a list. **Rebel's 661 functions without a line** (16 faces refused today, the
critic's count) **are documented before the landing**, as owed work and not as a waiver. **A parameter
line names the parameter's identifier without its mode mark** (`r`, not `@r`), and Rebel's 18 `@` lines are
rewritten (the critic's finding 5, a choice no seat ruled; recorded as the coordinator's).

**Built, and owed**: the verb, its three refusals and its two runs are built (`ce-q26`); the example taken
wherever it stands and the refusal of a missing line (about 15 lines, priced) are not, and land only after
their own build and run, beside the documenting of the 661 and the rewriting of the 18.

**What the blind seat measured**: the sentence's payment is the example (5 of 5 `now` readers already
wrote the parameter list; none titled a test `count_words:`: 3 of 5 wrote a test of the example under
another title and 2 wrote none, and all five also wrote the example as a fence in the comment). **What conservative would have been**: a
harness program, no verb, 0 tokens; refused. Javadoc's split (refuse a wrong name, warn on a missing one),
named by the critic, is what conservative-in-part would have been.

### R3, Q3: the gap stays in 0.1; the mixed route is the next to build; decision 24's reach goes to the author

**Adopted: the gap stays in 0.1** (decision 29's standing state), because no route was built into the
layer and run:

- **the Swift member (S) is refused as measured**: CryptoKit's Ed25519 signs non-deterministically, two
  signatures of one message differing and neither equal to RFC 8032's, which falsifies `keypairs.hero:96`
  and fails its test at `:485` (the ffi-pragmatist and the critic, each by a build);
- **primitives in Heroes (H) wait for an enabler**: the `>>` sign-test branch removed, overflow checks shown
  away, or constant-time operations the compiler owns;
- **a vendored C library (V) is two libraries**: Monocypher has no AES, and BearSSL was not sourced.

**Named as the next route to build and measure**: the mixed route nobody listed until the critic, the
AEADs through CryptoKit and Ed25519 and X25519 through a vendored C library.

**Put to the author**, as an open decision with the coordinator's recommendation: whether decision 24
(*"What reads data from outside is written in Heroes, never bound from a C library"*) admits a vendored C
cryptography library. **Recommended: yes, for primitives that read no outside data** (a cipher reads bytes
a program already holds), because the measured alternatives are a randomising system library or ciphers
whose emission branches on secrets today.

### R4, Q4: `skip_test(reason:)`, a third outcome told through a channel the runner owns

**Adopted: `skip_test(reason:)`**, not `skip` (11 declarations in the tree take that name, two of them in
frozen files; `skip_test` none), **a third outcome told through a channel the runner owns**: a file the
runner names, read only with exit 0, since no exit code can carry it, every code being one `exit(code:)` can
make. Built by the compiler-engineer: about 80 compiler lines and 34 runtime C lines, five of Rebel's 106
silent tests rewritten and reporting `SKIP`.

**A run where every test skipped fails** (the harness's floor rule; Catch2 by the historian's page; no
Rebel file is all skips, the critic's count). **A file with no test keeps exit 0 today**: the reason given
for the all-skipped rule does not reach it unmeasured (the critic's finding 10, its summary's item 8), so its
census is owed before it changes.

**The sentence**: `skip-r` as the blind seat read it (suffices 5 of 5, reduces the hazard 5 against 0),
its word `skip` written `skip_test`, and its clause *"fails no run for it"* replaced by the spec-warden's
`skip-v2` (*"a run where every test skipped fails"*, -3 vendored). That the renamed sentence reads as well
as the measured one is an inference, recorded. As composed, in § 12 after the `assert` sentence:

```
`skip_test(reason: "no server named")` ends a test there as skipped, never passed:
`heroes test` prints its title and reason and counts it apart, and a run
where every test skipped fails; only a `test` block calls it.
```

Measured for this synthesis, vendored and so a lower bound (`heroes-next measure`, the brief writer's
stage 2, `sources 23c7d2a626b29fd5`): 8,760 against the frozen spec's 8,702, **+58**; `skip-v2` as the
spec-warden wrote it reads 8,758, so the rename adds 2. Its real count is unrun.

**What conservative would have been**: a printing convention, 0 tokens; refused (the blind seat's 5 of 5
silent passes).

### R5, Q5: the sentence is vetoed; the walk is owed, not adopted as built

**The sentence (+14 real) is vetoed** by the spec-warden: it would be false on landing on Windows
(`stack.c:858`, no walk) and on x86-64 at `-O2` without frame pointers, and panel 230 ruled this § 6
sentence at zero tokens. **Not adopted.**

**The runtime work**, a frame walk with its lines resolved by the parent process ((W) with (A)) at 0
tokens, is approved by the compiler-engineer, the ffi-pragmatist and the historian, **and is not adopted
as built**: the walk as built skips the frame that aborts and loses what is inlined into it at `-O2` (the
critic's finding 1, by `otool` and `atos`), and it must not call `dladdr` on Linux, which waits more than
5 s while another thread is inside `dlopen` (the critic's finding 15, its summary's item 2, glibc 2.41).

**Owed, as one lane after the sittings**: (W) with (A) and its three repairs (start at the aborting
frame; resolve in the parent, never `dladdr` in the child on Linux; `-fno-omit-frame-pointer` for the
program's units on x86-64, a byte-identical binary on Linux arm64 by the ffi-pragmatist's run, Darwin
arm64 unrun), built and run on the three platforms. **And
a question filed**: the frozen stack guard already calls `dladdr` from a signal handler, and nobody has
probed it on Linux.

**What conservative would have been**: naming only the program's call into a package (B); refused as less
complete. **Route (P)**, the caller's place passed to every call: refused on cost (3,245 calls against 791
abort sites; Rust's RFC 2091).

### R6, Q6: one umbrella entry, one version, and the `packages` it places

**Adopted: an entry's `packages` list, binding a `path` entry too.** The compiler-engineer built the
refusal (about 170 lines) and `heroes add rebel/<package>` (113 new lines in `add_package.hero` and 16 in
`add.hero`, by its report); every
catalogue edit changes only whether the program builds. **The rulings the seats left open**:

- a name in `packages` is the path a program writes after `<rebel>/`, **whole** (`compress/zstd`, not
  `compress`; the critic's finding 12, its summary's item 9, the coordinator's choice);
- **every package a program `use`s directly is listed**, a platform module `hero_*` included, so the 16
  `reach_hero_*` programs list theirs, and `heroes add rebel/<path>` writes one;
- **a listed package's own `use` lines are placed with it**, its closure, and never listed by the program.

**The fix the arm promised is a note** naming `heroes add rebel/<package>`, never a `certain` fix (panel
235's R5: a fix edits only the program's files).

**The sentence**: `umb-w`, the spec-warden's, over a hypothetical package `kit`, binding `path` too, in § 1
after *"has no catalogue."*:

```
  catalogue. An entry publishing several packages under one version, or
  its `path`, places only those its `packages` names, with each one they
  use: `heroes add kit/csv` adds one, and a `use <kit>/flags` it does
  not name is refused.
```

and the example's entry `[dependencies.kit]`, `version = "0.1.0"`, `packages = ["csv", "json"]`, its `sum`.
The blind seat read `umb-r` (5 of 5 suffices), and `umb-w` changes words it did not read, so **its payment
is the argument, recorded**.

**Panel 250's R1 is amended by name** for an entry with a `packages` line; an entry without one keeps R1,
every version placed whole. **`fetch` placing the list's closure** (about 150 lines) is priced, not built:
owed with the landing.

**What conservative would have been**: Rebel placed whole, 38.3 MB and 1,639 files in every program;
refused (the median program of `examples-rebel/` needs 1.7 MB).

## The prior art, per resolution

Re-read on 2026-11-19 by the session that drafted this synthesis: each page below fetched again, its own
words checked against the line, and each line narrowed to what its page states. These lines go into the
decision issue as written here. Only precedents the historian marked `verified` and a resolution rests on
are kept.

**R1**, three:

- **Reached first by D (DMD)**: [Walter Bright on the D forum, 2023-12-19](https://forum.dlang.org/post/ultcue$1gv0$1@digitalmars.com): "In order for our ease of debugging, we keep dmd's dependence on other code at a minimum."; "There also isn't much that Phobos can add to the compiler."
- **Reached first by Go**: [Go Wiki: X-Repositories](https://go.dev/wiki/X-Repositories): "The `golang.org/x/...` repositories are part of the Go Project but outside the main Go tree."
- **Reached first by Python**: [PEP 594, Removing dead batteries from the standard library](https://peps.python.org/pep-0594): "Any additional module increases the maintenance cost for the Python core development team."

**R2**, four:

- **Reached first by Go**: [go doc, in the go command's documentation](https://pkg.go.dev/cmd/go#hdr-Show_documentation_for_package_or_symbol): "Doc prints the documentation comments associated with the item identified by its arguments".
- **Reached first by Go**: [package testing, Examples](https://pkg.go.dev/testing): "The naming convention to declare examples for the package, a function F, a type T and method M on type T are:" `func Example() { ... }`, `func ExampleF() { ... }`, `func ExampleT() { ... }`, `func ExampleT_M() { ... }`.
- **Reached first by D**: [DMD 2.063 changelog, released May 28, 2013](https://dlang.org/changelog/2.063.html): "Documented unittests which follow any symbol declarations are now used to generate example sections for the symbol"; "This allows the implementor of the function to keep their examples always up-to-date."
- **Reached first by Java (javadoc)**: [javadoc, JDK 21](https://docs.oracle.com/en/java/javase/21/docs/specs/man/javadoc.html): DocLint's `reference` group checks for issues such as "a bad name is given for `@param` or `@throws`", and "Issues are typically reported as errors because while the issue may not cause problems in the generated files, the author has likely made a mistake that will lead to incorrect or unexpected documentation."

**R3**, two:

- **Taken from Go**: [crypto/internal/constanttime](https://go.dev/src/crypto/internal/constanttime/constant_time.go): "The functions in this package are compiler intrinsics for constant-time operations."
- **Taken from SPARK (SPARKNaCl)**: [SPARKNaCl](https://github.com/rod-chapman/SPARKNaCl): a "SPARK 2014 re-implementation of the TweetNaCl crypto library", whose proof of type safety means "the code definitely contains no buffer overflow, numeric overflows, division by zero, or anything else that would normally give rise to an exception at run-time in Ada"; "The code retains the "constant time" algorithms from TweetNaCl."

**R4**, four:

- **Reached first by Go**: [Go 1.1 release notes](https://go.dev/doc/go1.1): "a new Skip method of testing.B and testing.T to simplify skipping an inappropriate test".
- **Reached first by TAP**: [TAP 14 specification](https://testanything.org/tap-version-14-specification.html): "If the directive starts with `# SKIP`, the test is counted as a skipped test"; "Harnesses must not treat failing SKIP test points as a test failure"; "Harnesses should report SKIP test points found as a list of items that were not tested".
- **Reached first by Catch2**: [Skipping, passing and failing](https://raw.githubusercontent.com/catchorg/Catch2/devel/docs/skipping-passing-failing.md): `SKIP`, "Introduced in Catch2 3.3.0"; "This test case is then reported as skipped instead of passed or failed"; "if all test cases in a run are skipped, Catch2 returns a non-zero exit code, same as it does if no test cases have run".
- **Reached first by Python**: [unittest, Skipping tests and expected failures](https://docs.python.org/3/library/unittest.html): "calling TestCase.skipTest() within a setUp() or test method, or raising SkipTest directly"; "Added in version 3.1."

**R5**, one:

- **Taken from Rust**: [RFC 2091, start date 2017-07-31](https://rust-lang.github.io/rfcs/2091-inline-semantic.html): "Enable accurate caller location reporting during panic in `{Option, Result}::{unwrap, expect}`"; of the panicking functions it rates "Nice to have" ("These functions are not commonly used, or the panicking condition is pretty rare."): "Inlining them would bloat the binary size without much benefit."; among its drawbacks, "the number of instructions per `unwrap()`/`expect()` will increase."

**R6**, three:

- **Reached first by Jetpack Compose**: [Use a Bill of Materials](https://developer.android.com/develop/ui/compose/bom): "The Compose Bill of Materials (BOM) lets you manage all of your Compose library versions by specifying only the BOM's version."; "To actually add and use Compose libraries in your app, you must declare each library as a separate dependency line"; "the BOM doesn't actually add those Compose libraries to your app."
- **Reached first by Maven**: [Introduction to the Dependency Mechanism](https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html): "The pattern below illustrates how a "bill of materials" (BOM) can be created for use by other projects."; "It defines the versions of all the artifacts that will be created in the library."
- **Reached first by Cargo**: [Features](https://doc.rust-lang.org/cargo/reference/features.html): "Features for dependencies can be enabled in the dependency declaration in `Cargo.toml`."

**Narrowed** against the historian's candidate lines: Go's X-Repositories loses *"looser compatibility
requirements"*, on which no resolution rests; Javadoc keeps the `reference` group alone, its `missing`
group's warning being the half R2 departs from (recorded in R2, not in a line); Cargo loses *"the union of
all features enabled"*, the weakness R6's per-program check answers and does not adopt; the DMD line
names no thread title, which the re-read page did not confirm, and gains the sentence before its first
quotation; Maven's quotation is the page's own pair, the BOM's creation and what it defines; SPARKNaCl's
README is quoted for its own words (*"an automated static proof"*, not *"completely automated"*); RFC
2091's line says which functions its *"bloat"* sentence is about. **Dropped**, each verified and none
rested on: DMD 2.020's druntime split (R1 rests on the ffi-pragmatist's count of 39,110 lines, not on D's
lesson); Go `vet`'s `tests` analyzer (R2 does not list the example-title refusal among its refusals);
rustdoc's doctests and RFC 1574 (the form panel 136 refused); Javadoc's `--date` and `-notimestamp`; .NET's
commit and Apple's DTS answer (R3 adopts neither (S) nor the mixed route, which it names as next); BoringSSL's
taint commit, Clangover, "Breaking Bad", "What you get is what you C", dudect, Monocypher, libsodium and
CNG's pages (R3 rests on the seats' own measurements); automake, Meson and CTest's status 77 (R4 departs
from it); Go's `GOTRACEBACK`, AddressSanitizer's offline symbolizer, Swift's backtracer, DbgHelp and the
frame-pointer pages of Fedora and Ubuntu (R5 adopts no walk yet); Go's vendoring and Boost's `bcp` (R6 does
not state (M') refused on them); Deno, the AWS SDK for Go v2 and Boost 1.56.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | P-t2-a, P-t2-b, P-t4-a, P-t4-b, P-t4-c, P-t6-a, P-t6-b, registered at 15:51 before the runner started | scored here: **all seven hold** |
| spec-warden | P-t2-c: at least 3 of 5 `now` answers write the example outside a `count_words:` test **and** at least 3 of 5 write the parameters in another form | scored here: **false** (5, and 0: every `now` reader already writes the list) |
| spec-warden | P-real: the refresh of `now` prints the pin, 11384, and each arm's real delta falls between 1.15 and 1.60 times its vendored one | scored by the seat's own refreshes at 15:54: **7 of 7 hold** |
| spec-warden | at M-rebel-library's close, `grep -c -i -E '\b(rebel\|core\|standard library)\b' spec/heroes-spec.md` prints 0, and every row of §1.11's table naming a C library for a need Rebel has a face for also names that face | M-rebel-library's close |
| spec-warden | `heroes doc` with `doc-w`'s refusal, over Rebel 0.1.0's 149 faces as frozen, refuses 642 functions, 626 of them for a missing list; with the arm's literal refusal, 0 | R2's landing, on Rebel as frozen (the critic counts 661 over 142 faces, and the 18 `@` lines count as 0 only under R2's ruling) |
| spec-warden | at R4's landing, `heroes test` over a file whose every test skips exits 1 and prints each title with its reason; one pass and one skip exits 0 | R4's landing |
| spec-warden | § 6 still reads *"An abort ends the program at once, saying why; no `T?` carries one."* at M-rebel-library's close | M-rebel-library's close |
| spec-warden | under `umb-w`, `examples-rebel/heroes.toml`'s `packages` lists exactly the distinct first parts of its programs' `use <rebel>/` lines, and 0 of the 161 programs is refused | R6's landing; its first half is false by R6's ruling of the whole path, its second stays to score |
| compiler-engineer | `builtin_name_taken` over every tracked root moves by 0 if `skip_test` lands (by 45, ±2, if `skip` did); the outcome's `selfhost/` diff stays under 120 lines with its tests | R4's landing |
| compiler-engineer | `heroes doc` over Rebel's faces refuses 0 faces for `doc_parameter_order`, and with a refusal of a missing line at least 600 functions before Rebel's comments are repaired; the verb's three modules stay under 400 lines of code | R2's landing (the 600, on its compiler over Rebel as frozen) |
| compiler-engineer | at the commit that rewords law 18, `git diff --stat` touches `doctor.hero` by exactly 2 lines and `seed/heroes.c` by 0, and the two-stage fixpoint `cmp` stays empty | R1's landing; every half stays to score (R1 lands law 18 without a refresh of the seed) |
| compiler-engineer | over the 161 programs of `examples-rebel/`, each given a catalogue made by `heroes add rebel/<path>` for its own `use` lines, `heroes check` refuses 0; a list of faces alone refuses exactly the 16 `reach_hero_*` programs | R6's landing |
| compiler-engineer | every abort of the `run` form at `-O0` names at least one `called from` line wherever its place is not in `main`, and the happy-path instructions of the form's programs move by 0.0% | R5's lane |
| ffi-pragmatist | at the landing of the design.md text, its commit names no file under `selfhost/`, `runtime/` or `seed/`; and Rebel's `sql/sqlite` with any of its 42 `sqlite3_*` results declared one integer width narrower than `sqlite3.h` says is refused `ffi_return_type`, with no shim | R1's landing (its first half stays to score: law 18 lands in a commit of its own, apart from the text) |
| ffi-pragmatist | (a) with a Swift member in the layer, `keypairs.hero:485` fails on macOS unless its assertion is weakened; (b) a cold `heroes build` of `print(1)` on a Mac then runs a `swift-frontend` retiring at least 1.4 G instructions | a Swift member's landing, which R3 does not adopt (the mixed route's, for (b)) |
| ffi-pragmatist | (c) with the emitter's `>>` written as `shr_flat`, MemorySanitizer's `-O0` reports at `word.hero:23` over SHA-512 of a secret fall from 1,943 to 0, and the two-stage fixpoint holds | the repair of the `>>` defect filed with this sitting |
| ffi-pragmatist | with (W) telling the abort's own frame, `heroes run` of `bw/q5/deep.hero` in Linux arm64, resolved by `llvm-symbolizer --inlines`, names `deep.hero:3`, `:6`, `:10` and `:14`; on Linux a program aborting while another thread is inside a blocking `dlopen` reaches exit 134 only once `dladdr` is out of the walk | R5's lane |
| historian | none (advisory; its conditions stand in its report, one per question) | |
| llm-ergonomist | its decision rules, registered in its brief before any run (`253-briefs/llm-ergonomist.md:245-251`) | scored here: t2 suffices and needed, t4 suffices and reduces the hazard, t6 suffices |

## How the sitting went, for the next

- **The blind seat's leaks were measured shut.** The free preflight and every paid session ran with
  standard input from `/dev/null`, the blind area an empty git repository of its own, and `HOME`, `TMPDIR`
  and `CLAUDE_CODE_TMPDIR` inside it; `tools/scan.py` read 32 transcripts, the preflight's and the 31
  sessions', and printed `prompt-alone=NO: 0, git-status=LEAK: 0, claude-md-or-memory=YES: 0`; no
  transcript under `~/.claude/projects/` names the area. So defect 2589's three leaks and the fourth panel
  251's critic found stayed shut, and no session is voided. `CLAUDE_CODE_TMPDIR` did move the CLI's
  `cache-break-state` file inside the area (`tmp/claude-501/`, the critic's finding 9), which answers the
  brief's open question; its listing belongs in the evidence. 31 of 31 sessions ended `success`, `is_error`
  false, `terminal_reason` `completed`; spent 5.335998 USD of 10. The verdict session says it had a file
  reader alone and wrote its report into its answer, which the grading reads from `result.md`.
- **The spec-warden's predictions were registered before the runner started**: written at 15:51
  (`date` read 15:50:49 just before), the runner started at 15:51:14 and copied the report to
  `warden-at-start.md` (sha256 `1f0ba1909c8af73e...`, the hash `run.log` wrote), whose lines 1 to 17 equal
  the report's by the critic's `diff`. Panel 251's departure, a payment admitted after the fact, was not
  needed.
- **The clock rule held**: no seat took a wall-clock measurement while five agents shared the Mac (the
  coordinator's decision (b)); every cost is in instructions retired and bytes. The ffi-pragmatist found,
  and the critic reproduced, that `/usr/bin/time -l` counts the process it starts and none of its children
  (clang's driver 75.3 M, its `-cc1` counted alone 1.23 G), so every figure of a verb that spawns clang or
  another tool is the `heroes` process's own; the next brief's rule says so. Owed to a still machine:
  `-fno-omit-frame-pointer`'s instructions on x86-64, a timing test of dudect's kind for any cipher, and
  `fetch`'s cold instructions under the list and under W.
- **The tree's freeze was kept**: `git -C .claude/worktrees/rebel-round log --oneline 181555e9a..HEAD --
  packages selfhost runtime spec seed` prints nothing. The branch's one commit since the frozen commit,
  `f4e3594d5` (panel 251's evidence, 14:23), moved two files under `docs/panel/251-reports/` while the
  brief was being written, before any seat started, and the brief says so; every seat judged its own `git
  archive` of `181555e9a`.
- **Two rules a seat broke, each said by the seat itself**: the ffi-pragmatist's Argon2i taint at `-O0`
  wrote 531,195,298 bytes into a file nothing bounded before it was killed and the file deleted, every
  later run bounded at 120 s and 65,536 bytes; and the historian's WebFetch of a PDF saved 305.6 KB outside
  the repository's root (`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/tool-results/webfetch-1795104584839-vk3usk.pdf`),
  for the coordinator or the author to remove.
- **The seats ran in line on one Mac**, as the decision to keep the six questions together foresaw: the
  spec-warden 15:50 to 16:05, the compiler-engineer from 15:55, the ffi-pragmatist 16:40 to 17:28, the
  historian's last write 17:32, the critic 17:39 to 17:52 (each report's own `date` lines and file times).
  The compiler-engineer took its three cuts in the brief's order, and every question kept a whole block.
- **The evidence** is owed: the sitting's folders packed by panel 246's rules into
  `docs/panel/253-reports/evidence.tar.xz`, as panel 251's were.

## What this sitting files and owes

- **Filed with the sitting** (drafts under `.claude/worktrees/rebel-scratch/coord/issues/`; the
  coordinator files them):
  - a defect: `>>` is lowered as a test of the sign (`selfhost/emit/operator.hero:172`), a secret-dependent
    branch at `-O0` in Rebel's `word.shift_right`, reaching SHA-512, BLAKE2b, Argon2, SHA-3 and `random`'s
    next key; **`improvement`** (the critic: no promise of the tree is broken, `crypto.hero:15-19`), the
    first prerequisite of route (H);
  - a question: the frozen stack guard's `dladdr` inside a signal handler on Linux, unprobed;
  - a question of the C boundary: about 151 function-pointer slots of the Linux layer typed by hand and
    checked against no header (the ffi-pragmatist);
  - R3's open decision: whether decision 24 admits a vendored C cryptography library, recommended yes for
    primitives that read no outside data.
- **Owed by R1**: design.md's text written fresh, with the ffi-pragmatist's four repairs and against the
  register; the register, one `decision` issue with dated comments, naming each of the 66 catalogues before
  `packages/core` goes; law 18 in `doctor.hero` with the seed's refresh in the same change; CLAUDE.md:279's
  words.
- **Owed by R2**: the verb's two repairs, each built and run (an example wherever it stands, a missing
  parameter line refused); the `@` ruling; Rebel's 661 functions documented and its 18 `@` lines rewritten
  before the landing; the sentence with the verb.
- **Owed by R3**: the mixed route built and measured; the author's answer on decision 24.
- **Owed by R4**: the landing, the sentence with it; the census of files with no test before that rule
  changes.
- **Owed by R5**: one lane after the sittings, (W) with (A) and its three repairs, built and run on the
  three platforms.
- **Owed by R6**: the landing with `umb-w` and the rulings above; `fetch` placing the list's closure;
  panel 250's R1 amended by name.
- Each landing's goldens, forms and site pages as its seat priced them (`compiler-engineer.md`, § Cuts and
  each question's `implementation_cost`).

## Author's verdict

**Ratified, 2026-11-19**, R1 to R6 as proposed: the author's word given at about 13:10, before this
sitting's briefs were written, meant as *OK to the ratifications as proposed*, of sittings A, B and C of
M-rebel-library, recorded as given. Its decision is https://github.com/heroes-lang/heroes/issues/2595,
opened and closed with this file's commit, its `## Prior art` the lines re-read that day. Filed with it:
defect 2591 (`>>` lowered as a test of the sign, `improvement`), tasks 2592 (the stack guard's `dladdr`
inside its signal handler on Linux) and 2593 (the Linux layer's hand-typed function-pointer slots), and
decision 2594, open, R3's question to the author whether decision 24 admits a vendored C cryptography
library.
