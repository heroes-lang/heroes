# Panel 244, spec-warden

Written as the sitting ran, 2026-11-10. Worktree `.claude/worktrees/244-spec-warden`, detached at
`89db84347`. Probes and drafts in `.claude/worktrees/p244-scratch/spec-warden/` (`drafts/` holds
every draft as anchor/replacement JSON, `apply.py`, `price.sh`, `prices-alone.txt`,
`prices-sets.txt`, `S1f.diff`). No paid run, no credential loaded, no GitHub write; two read-only
`gh issue view` (2402, 2357/2358).

## 0. The verdict, in the seat's shape

- `verdict`: **approve S1f** (d03 + d07f + d08d below), **provisional**: every number is vendored,
  the real row was not refreshed. **Object** to stating `internal` (d05), `heroes fetch` (d06),
  the `index` line (d09), a `path` entry in the example (d04s), a package's own catalogue holding
  no `path` (d10), the stand-in and outside-reading clauses (d07b, d07c), and to the spec naming
  Q-stub's verb (§ 9). **No veto**: nothing comes near the ceiling, and nothing approved adds a form.
- `section`: design.md §1.6 (ceiling 16384, the payment rule), §1.2 (real cost = tokens × (1 +
  rewrite rate)), CLAUDE.md § 2 (Principle 0), § 12 (the spec and a ratified rule disagree: the
  spec has the error, panel 232's "an error in the specification itself").
- `spec_token_delta`: base 8167 legacy / 8314 cl100k / 10898 real. S1f **8213 / 8361, +46 / +47
  vendored, lower bounds**. Real: unrun. Inferred from the ledger's ratio (+48 to +60, +143 to
  +178, +308 to +390, about x1.25) at about **+59, 10,957**. Row 8314 (−6 vendored, +4 real) shows
  the sign can turn at this size.
- `removal`: **task 2402's rewrite of § 13's opening, d08d, −9 legacy / −8 cl100k**, measured in
  place and counted inside S1f. It does not cover S1f alone, so the rest is paid by the
  predictions in § 5.
- `needed_for_self_hosting`: **no**. The compiler uses no package.
- `argument`: Six of the candidates' refusals already state the rule and the way out to the
  person who can act (b1, b2, b5, b8, b9, e1). Writing them into the prompt charges every task,
  package or not, to save one round trip in a few, which fails §1.2. Three sentences pass. § 1's
  catalogue sentence is false for `path`, a source R6 ratified, so a reader who obeys it is
  refused (c2). § 13's "JSON from C" has been false since `encoding/json` shipped, and rewriting it
  gives 8 back. A fetched package's enumerator or float constant is refused only at a consumer,
  while the author's `path` and own-tree builds pass (d2, f2, f3): no message ever reaches the
  writer.
- `prediction`: **W**: S1f as worded reads exactly 8213 / 8361 vendored, and **10,940 to 10,975
  real** on the landing's `--refresh`. Above 10,975 falsifies it, and the landing then takes rb1
  (−4 / −4, measured). **P2**: the goldens in § 5 hold every S1f sentence at M-core-packages'
  close.
- `condition`: § 8.

## 1. The instrument and the budget, run today

- Stage 1 `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 14.61 s; stage 2
  `./heroes build selfhost/main.hero -o heroes-next`, real 179.60 s; `--version`: `heroes 0.6.0` /
  `sources 14e9e5ce168a2336`, the base's key. Every probe below ran with that `heroes-next`.
- design.md §1.6, grepped this sitting (line 253): ceiling **16384** on `claude-opus-5` through
  `count_tokens`; the payment rule unconditional; a registered prediction pays only if its
  instrument exists the day it is registered. The 1,500-token cap binds the consolidation block
  "before the thesis harness has run". M-thesis-harness closed 2026-10-24 (ROADMAP row 70), and
  M-core-packages is row 76, past the block's last row (74). **The cap does not reach this
  sitting**; the ceiling and the payment rule do.
- `tests/harness/suite_spec.hero`: `CEILING` 16384, `DELTA_GATE` 50 vendored per commit.
- Baseline, `./heroes-next measure spec/heroes-spec.md` from the worktree's root: legacy **8167**,
  cl100k **8314**, real **10898** (pinned 2026-11-09), FFI floor 60, so 10958 is judged against
  16384: **5426 free**. No candidate set below comes within 5,200 tokens of the ceiling. **My veto
  ground is not the ceiling.** It is Principle 0 and the payment rule.
- **How every draft was priced** (item 9): the draft applied in place to the worktree's
  `spec/heroes-spec.md` by `drafts/apply.py` (each anchor asserted to occur exactly once),
  `measure` run from the worktree's root, then the original copied back and checked by `cmp` and
  `git diff --quiet` (`price.sh` prints `restored`; every row below printed it). A null draft read
  8167 / 8314, the baseline. **Every delta below is vendored and a lower bound on the binding
  number, in those words.** No `--refresh` ran (a paid call, the author's).

## 2. What each mistake gets from the compiler today (89db84347's messages, run)

The question each row answers: does the refusal reach **the person who can fix it**, and does it
tell them the rule?

| probe | the mistake | what `heroes-next` said | carries the rule? reaches the writer? |
|---|---|---|---|
| `a0` | `heroes add core --index file://../../../../../packages` from nothing | `added core 0.2.1`; wrote `index`, `version`, `sum`; **placed** `heroes.packages/core@0.2.1-a317653cd03ec276`; `run` printed `hi`, exit 0 | the spec's route works without `fetch` |
| `b1` | a catalogue with its tree absent (a cloned program) | `package_unread`: "not placed: `heroes fetch` places it at `…/core@0.2.1-…/`", exit 1 | yes, and the exact command; `heroes` with no argument also prints "build, check and run never fetch" |
| `b5` | `fingerprint =` for `sum =` (the spec's own prose word) | `catalogue_refused`: "`fingerprint` is not a key an entry has: an entry holds `version` and `sum` for the index, …", exit 1 | yes; but its "the catalogue holds nothing but the program's dependencies" is **false** beside the `index` line `heroes add` writes (defect candidate) |
| `b2` | `use <core>/strings/internal/scan` | `private_module`: "…a module under an `internal` folder is named only by the module that folder belongs to and by the modules under it; name what `<core>/strings` offers instead" | yes, word for word, with the way out |
| `b3` | the program's own `util/internal/bits` named from `main` | `private_module`, same words | yes (it points at a `util` module that does not exist: minor) |
| `c3` | a `path` package's root `internal/help` named by a consumer | `private_module`, private to `<mypkg>` | yes |
| `b4` | a file `lib/internal.hero`, under **no** `internal` folder, as `use lib/internal` | `private_module`: "a module under an `internal` folder…", exit 1 | **false message**: the critic's `p6`, reproduced. R16's words and the compiler disagree |
| `c1` | `heroes add --path ../mypkg` | wrote `path = "../mypkg"` alone; `run`: note "unverified, no fingerprint holds it", `2`, exit 0 | the spec's § 1 sentence is **false** for this ratified source (panel 235's R6: "`path` entries are kept") |
| `c2` | a `path` entry with `version` and `sum`, as § 1 reads | `catalogue_refused`: "names one source: a `version` from the index, a `git` address with its `tag` and `commit`, or a local `path`" | yes, but a reader **obeying the spec** gets it |
| `e1` | a `path` package whose own catalogue holds a `path` | `requirement_refused` at the author's own `heroes add --path`: "only the program's own catalogue holds a `path`" | yes, **on the author's route** |
| `b8` | `use <json>/parse`, no catalogue | `unknown_entry`: "there is no catalogue `b8/heroes.toml` beside `b8/main.hero` … `heroes add json` writes one" | yes |
| `b9` | `use <core>/nothere` | `unknown_package_module`: "has no module `nothere` … that file is not there" | yes |
| `b10` | the `index` line deleted, tree placed | `package_unread` "not placed … `core@0.2.1-edd6313d5521e3f8/`" while `…-a317653cd03ec276/` stands | **no**: the `index` line is part of a package's key, and the message names the wrong cause (defect candidate, the critic's item 11's class) |
| `d1` | a **fetched** package binding `#define HALF 0.5` as `constant HALF: f64` | `ffi_constant_type`: "cannot be read without running its header's code, which a fetched package's header never does at build: it is a float", note: "Write the value so in the package's header, or compute it in Heroes" | it carries the rule, but it reaches **the consumer**, who cannot edit the package |
| `d2` | **the same package through `path`** (R6's development route) | runs, prints `2` and `0.5`, exit 0 | **the author is told nothing** |
| `f1` | a fetched package binding an **enumerator**, `enum colour { RED = 1, GREEN = 2 }`, as `constant GREEN: i64` (hand-made index `idx/epkg`) | `ffi_constant_type`: "its value is read through `GREEN`, an enumerator or object…", exit 1 | the consumer again |
| `f2`, `f3` | the same package through `path`, and in the program's own tree | both run, `2` and `1048576`, exit 0 | **the author is told nothing** |

`d2` and `f2` are not a defect: defect 2357's repair (closed, `0ba4e45bd`) keeps a `path` entry on
"the program's route as the author's own code (panel 235's R6)", and feature 2358 (open) is filed
to lift the enumerator and float refusals. So it is structural: **the author of a package over C
whose constants are enumerators, curl's and libuv's shape, writes a package that builds wherever
they test it and that every consumer is refused**. Of the candidates, it is the only refusal I
measured that never reaches the person who can repair it, and the only one a reader holding the
spec cannot predict from § 13, whose "A group's `constant` has no body: the header holds the
value." it contradicts for fetched code.

## 3. The drafts, each priced alone in place (vendored, lower bounds)

| id | home | the change | legacy | cl100k |
|---|---|---|---|---|
| d01 | § 1 | "exact version and fingerprint" becomes "exact version and `sum`" | +1 | +1 |
| d02 | § 1 | the correction, prose: "…and fingerprint, or by a local `path` alone, which nothing verifies." | +14 | +15 |
| **d03** | § 1 | d02 with `sum` for fingerprint: "names with its source, exact version and `sum`, or by a local `path` alone, which nothing verifies." | **+15** | **+16** |
| a03 | § 1 | d03's content **appended** as its own sentence ("An entry may instead name a local folder, `path = "../mine"`, alone: no version and no `sum`, and nothing verifies it.") | +32 | +33 |
| d04 | § 1 | d02 plus a third fence entry `[dependencies.mine]` / `path = "../mine"` | +27 | +28 |
| d04s | § 1 | d03 plus that entry | +28 | +29 |
| d05a | § 1 | `internal`, R16's folder words, with `a/internal/b` | +43 | +45 |
| d05b | § 1 | `internal`, the element rule the compiler applies: "A path with `internal` as a part after its first is private: `a/internal/b` is named only by `a` and the modules under `a/`." | +38 | +39 |
| d06 | § 1 | "`heroes add json` writes that entry and places the package, `heroes fetch` places every entry's, and a build only reads what is placed" | +23 | +24 |
| d09 | § 1 | "its `index` line names where versions come from, the Heroes index if none" | +18 | +19 |
| d10 | § 1 | "the package's own catalogue, which holds no `path`" | +7 | +8 |
| d07a | § 13 | after the `constant` sentence: "A package's header is read and never run, so its group's `constant` is an integer or `bool` the header writes as a constant expression." | +34 | +34 |
| d07b | § 13 | d07a plus "and it reads no file outside the package, the runtime and the toolchain" | +51 | +51 |
| d07c | § 13 | d07b plus "and no file of the package may be named like a header of either" | +65 | +65 |
| d07e | § 13 | "A fetched package's header is read and never run, so its group's `constant` is an integer or `bool` the header writes as an expression of literals." | +36 | +36 |
| **d07f** | § 13 | "A fetched package's header is read and never run, so its group's `constant` is an integer or `bool` the header writes as a constant expression, never an enumerator." | **+40** | **+39** |
| a07 | § 13 | d07e's content appended after the `pkgconfig` sentence | +37 | +37 |
| d08a | § 13 | task 2402: "Anything beyond this document comes from packages (section 1) or C libraries; threads, …" | −4 | −2 |
| d08b | § 13 | task 2402, keeping "— sockets, maths, databases —" | +4 | +5 |
| d08c | § 13 | task 2402: "…comes from a package (section 1) or from C; threads, …" | −3 | −1 |
| **d08d** | § 13 | task 2402: "Anything beyond this document comes from packages or C libraries; threads, …" | **−9** | **−8** |

**Merged against appended (item 9)**: the `path` correction merged (d03, +16) costs **17 cl100k
less** than appended (a03, +33), panel 122's rule holding again. The § 13 clause is within 1 either
way (d07e +36, a07 +37), and its home is the `constant` sentence it qualifies, so it merges.

**Sets, priced whole in place** (`prices-sets.txt`):

| set | contents | legacy | cl100k |
|---|---|---|---|
| S0 | d03 + d08d (the correction and 2402 alone) | +6 | +8 |
| S1 | d03 + d07e + d08d | +42 | +44 |
| **S1f** | **d03 + d07f + d08d (recommended)** | **+46** | **+47** |
| S2 | d04s + d07e + d08d (the example entry) | +55 | +57 |
| S3 | d03 + d05b + d07e + d08d (with `internal`) | +80 | +83 |

Everything drafted together (d04s + d05b + d06 + d07c + d09 + d10 + d08d) sums to about **+176
cl100k** (sum of the parts alone, an inference: that set was not applied whole).


The `spec` suite with S1f in place (`./heroes-next run tests/harness/main.hero -- ./heroes-next
spec`, output `drafts/S1f-spec-suite.txt`): **31 passed, 4 failed**. The four are exactly the pins
a landing rewrites in its own commit: `budget` (8361 against 8314, delta 47), `spendable`, `real`
and `ledger`. Every word, anchor, shape and inventory check passed with the new code spans
(`sum`, `path`, `bool`, `constant`). The spec was then restored, and `cmp` and `git diff --quiet`
confirmed it.

## 4. Each candidate, ruled

The test the shared brief sets: a sentence enters where a reader holding only the specification
would otherwise write a wrong program or be unable to predict a refusal. I add §1.2's question to
it: **does the refusal reach the person who can act, and does it tell them the rule?** Where it
does, a sentence buys at most one round trip, and only in a package task that makes that
mistake. Meanwhile its tokens are paid in every prompt.

1. **The `path` correction, d03 (+15 / +16): approve, merged.** § 1's "names with its source,
   exact version and fingerprint" is false for a ratified source (panel 235's R6, "`path` entries
   are kept"). It is the one place where a reader who **obeys** the spec is refused (`c2`). The
   Q-stub verb makes this more urgent, because the first catalogue its package stub implies is a
   `path` entry (§ 9). Merged costs 17 cl100k less than appended (a03). "`sum`" for
   "fingerprint" costs +1 and makes the prose name the key every message names (`b5`).
   **Against the example entry (d04s, +13 more)**, on robustness rather than tokens: 236 of 240 of
   panel 235's readers copied that fence, and a `path` is the one source no fingerprint holds (a
   mistyped `path` to a folder with the same module names compiles other code, measured by panel
   235's critic). Putting it in the most-copied block promotes the unverified route. The prose
   states it together with its cost.
2. **Task 2402, d08d (−9 / −8): approve. It is the named removal.** "Anything beyond this
   document — sockets, maths, JSON, databases — comes from C libraries" has been false for JSON
   since core 0.2.x ships `encoding/json` (`packages/core/0.2.1/files/encoding/json.hero`). The
   pointer "(section 1)" costs 6 (d08a −2 against d08d −8). In a document read whole, after panel
   240 made *package* mean one thing, the pointer buys nothing. design.md's JSON row (`:556`,
   2402's own pointer) is outside this budget and is the landing's.
3. **A fetched package's constants, d07f (+40 / +39): approve, in § 13 beside the sentence it
   qualifies.** It is measured as the one refusal that never reaches its writer (`d1`, `f1`
   refused at the consumer; `d2`, `f2`, `f3` building for the author). § 13's present "the header
   holds the value" says the opposite for fetched code. The enumerator shape is not rare: it is
   how curl, libuv and the key codes of graphics libraries spell their constants (feature 2358's
   own words: "a library's key codes among them"). **d07f over d07e (+3)**: in C a `const` object
   is already no constant expression, but an enumerator is one, so "never an enumerator" is the
   one exclusion C's own term does not carry. d07e's "an expression of literals" would wrongly
   exclude `sizeof`. The clause lives exactly as long as feature 2358 leaves the refusal standing:
   **2358's landing commit owes its removal**, and P2's golden is what makes that loud.
4. **`internal`, d05a / d05b (+43 / +45, +38 / +39): object.** Every shape I ran (`b2`, `b3`, `c3`)
   is told the rule word for word, with "name what `<owner>` offers instead". And the mistake
   needs a reader who has opened a package's tree and seen the folder `internal`, Go's convention,
   which predicts the refusal. The decisive point is that **the rule's shape is undecided**:
   `b4`, a file `lib/internal.hero` under no `internal` folder, is refused with a message saying
   it is under one. R16's ratified words say folder; the compiler applies Go's element rule. A
   sentence now writes one side of an open defect into the document promised in substance. Left
   to `private_module` it is promised by its code alone, and b4 can be repaired either way without
   a spec change. Whether Go's language specification states `internal` or only the go command's
   documentation does is the historian's question; I did not read either this sitting.
5. **`heroes fetch` and where trees stand, d06 (+23 / +24): object.** `heroes add` places the
   package as it writes the entry (`a0`), so the spec's route needs no `fetch`. The clone case is
   told the exact command and folder (`b1`), and `heroes` with no argument prints "build, check
   and run never fetch". The folder name `heroes.packages/` is a tool fact. Writing it into the
   spec would promise it for 1.x and buy nothing a program needs.
6. **The `index` line, d09 (+18 / +19): object, with a defect beside it.** A reader never writes
   the line: `heroes add --index` does, and a hand-written entry has no `sum` to write anyway
   (panel 235's spec-warden, its § 2). But `b10` shows the line is part of a package's key, and
   deleting it yields "not placed" beside a placed tree of the same version and sum. That is a
   message naming the wrong cause, the class of the critic's item 11. The repair belongs to the
   message, not the prompt. Whether § 1 promises a verb whose default source answers nobody
   (item 13) is unrun: I made no network read. The default is ROADMAP row 78's to serve, and the
   spec describes the language at 1.0.
7. **A package's catalogue holding no `path`, d10 (+7 / +8): object.** It is refused at the
   author's own `heroes add --path` with the rule (`e1`), so it reaches the writer.
8. **The outside-reading and stand-in clauses, d07b / d07c (+17 / +31 over d07a): object,
   pending the ffi-pragmatist.** `package_stand_in.hero:6-20` says a `path` entry's units are held
   to the stand-in search, so the author does see that refusal. I did not run whether
   `package_reads_outside` binds a `path` entry. If it does not, it has d07f's shape and the
   ffi-pragmatist's measurement should bring it to the same ruling.
9. **`use <entry>` alone and `unknown_package_module`: no sentence.** The first is in the `Use`
   production (critic item 1). The second (`b9`) names the file it looked for.

**What the guide takes instead.** Everything ruled out above is true and worth a reader's time:
the layout, `internal/`, `fetch`, the `index` line, the reasons. Lane b30-docs' guide holds it at
no prompt cost. That is how I read the author's instruction of 2026-11-10: "put what is missing
into the specification" means what a program's writer cannot learn otherwise, and the rest goes to
the guide.

## 5. The payments (design.md §1.6, panel 046's standard)

- **Named removal**: d08d, −9 legacy / −8 cl100k, measured in place, landing in the same commit.
- **W, registered now**: S1f as worded reads exactly **8213 / 8361** vendored and **10,940 to
  10,975 real** on the landing's `--refresh` (the instrument exists; the call is paid and is the
  author's). Above 10,975 falsifies W, and the landing then takes **rb1**: d03 loses ", which
  nothing verifies", **−4 / −4** measured (S1f-rb1 8209 / 8357). The build's note says "unverified"
  at every use of a `path` (`c1`), so rb1 removes the one S1f clause that a message already carries.
- **P2, registered now, scored at M-core-packages' close by the golden forms that exist today**
  (`tests/golden/fetch/` and the `check`/`run` forms): the landing pins, beside the text, (i) a
  `path` entry with a `version` refused `catalogue_refused` and a `path` entry alone building
  with the unverified note (`c2`, `c1`); (ii) a fetched package's enumerator and float constants
  refused `ffi_constant_type` (`f1`, `d1`); (iii) the same package through `path` building
  (`f2`, `d2`). **Falsified if any case flips while its sentence stands.** In particular,
  feature 2358 landing without removing d07f's clause falsifies P2 at that commit.
- **P3, an observation unless the author asks for it** (a paid blind run, whose instrument exists,
  so it is admissible only if run): with S1f landed, at least 9 of 12 blind sessions asked to write
  a package over a header whose constants are enumerators bind none of them as a fetched
  `constant`; with today's spec, at most 3 of 12.

## 6. The 1.0.0 promise, per candidate (panel 232, `docs/panel/232-*.md:264-276`)

- **d03** brings `path` entries into the promise: a source with no version and no `sum`,
  unverified, through 1.x. Today's sentence leaves them out by accident of wording, not by ruling.
  R6's refusal at a publishing verb stays compatible, because it is a tool rule, not a build rule.
- **d07f** promises a **refusal**. Lifting it (feature 2358) compiles more programs, which the
  promise allows, and the clause is then corrected as "an error in the specification itself" by
  2358's commit.
- **d08d** promises nothing new and withdraws a false claim.
- **Left out**: `internal`, `fetch`, the `index` line and Q-stub's verb are promised only by their
  diagnostic codes (`private_module`, `package_unread`) and exit codes. Their substance, b4
  included, stays repairable in 1.x without a spec change.

## 7. Should the blind seat run before ratification?

**Not as a condition for S1f.** The truth of each S1f sentence rests on a run, not on a reader:
d03 corrects a measured falsehood, d08d is a measured removal, and d07f states a refusal measured
to reach no author. The blind reading would score only P3, which pays nothing the removal, W and
P2 do not already pay. It is worth running if the author wants P3 scored: 2 arms x 6 sessions on
one model. Panel 237's 72 sessions cost 4.5208 USD, so about 0.75 USD by proportion, an inference.
**If the sitting adopts any of the sentences I object to** (d05, d06, d09, d04s), the blind
reading should run **before** ratification. Those sentences have no value except a reader's
behaviour, and the messages already carry them. Without a reading they would be paid by nothing.

## 8. What would change this verdict

1. Feature 2358 landing before this landing: d07f drops, and S1f becomes S0 (+6 / +8).
2. The sitting holding a `path` package to the fetched constant rule, or giving the `path` build a
   note naming what a fetched build will refuse. The author would then be told, and I would object
   to d07f as the message's job.
3. A ruling on b4 does not move `internal` into the spec by itself. A blind reading in which a
   measurable share of package tasks name a private module would (d05b).
4. A `--refresh` above 10,975 after rb1.
5. Panel 243 landing first. My anchors are § 13's opening sentence (spec line 374) and the
   `constant` sentence (line 428). 243's drafts anchor after "and what a `ptr` points at." (line
   383), so the anchors do not overlap, and the landing re-prices S1f on 243's text (item 18).

## 9. Q-stub: does the specification name the stub verb or its layout?

**No, on the same test.** Measured: the spec names exactly one verb, `heroes add` (line 34;
`grep -n -o -E 'heroes [a-z]+'`), and never names `build`, `run`, `test` or `fetch`. It says
"the file you compile". Without a stub verb a reader writes no wrong program: § 1 already says
what a program is (one file holding `function main()`), and a package's layout beyond its modules
is a convention of `examples/packages/README.md`, whose rules it says are held "by the compiler,
the net's `packages` suite, or a reader's review". Nothing under `selfhost/fetch` or
`selfhost/module` requires a card (grepped), and my `cpkg`, `epkg` and `mypkg` probes had none and
fetched or built. The verb is found where the other fourteen are: `heroes` with no argument lists
every verb with its summary.

**What naming it would cost** (in place, vendored lower bounds, `drafts/prices-qstub.txt`):

| id | clause after "has no catalogue" | legacy | cl100k |
|---|---|---|---|
| q1 | "…catalogue, and `heroes make program app` writes `app/main.hero` and none." | +19 | +20 |
| q3 | q1 with `new` for `make` | +19 | +20 |
| q2 | both forms with the package layout: `json.hero`, its card `json.md`, its `internal/`, its tests, reached through a `path` entry while developed | +67 | +70 |

q2 would also write into the spec the `internal/` convention I object to in § 4 item 4. It would
also promise the verb's name, operands and layout for 1.x, where left out they are promised by its
exit codes alone (panel 232). Whether a verb the spec names falls inside "the language the
specification describes" is a question, not a premise. If it does, naming the stub verb would
freeze its first shape.

**The operand words, against the spec's own vocabulary** (`grep -c -E -i` on `spec/heroes-spec.md`):
`program` stands on **13** lines and is the spec's word for what you compile; `package` stands on
**5** and since panel 240 means code others publish; `library` stands on **2**, both C's (§ 13: "C
libraries", "`link` a library", "headers and libraries"); `project`, `app` and `lib` stand on
**0**. So the operands that fit are **`program` and `package`**. `lib` would bring back the
collision panel 240 removed when it respelled the C mark `pkgconfig` so that *package* means one
thing: here *library* would mean two. `app` is a word the spec never uses for a thing it already
names `program`. The verb's own name is not a spec word whichever way it goes. I note only that
`make` is also the build tool CLAUDE.md § 10 refuses ("never a Makefile"); that and the rest of the
shape belong to the compiler-engineer and the historian.

## 10. Defect candidates for the coordinator (filed by nobody here: I write nothing on GitHub)

1. **`b4`** (the critic's `p6`): `lib/internal.hero`, under no `internal` folder, is refused
   `private_module` with "a module under an `internal` folder…". A false message, or a correct
   program refused under R16's words.
2. **`b5`**: `catalogue_refused`'s "the catalogue holds nothing but the program's dependencies" is
   false beside the `index` line `heroes add` writes (the critic's `p19` read the same
   contradiction).
3. **`b10`**: deleting the `index` line moves the placed tree's key (`a317653cd03ec276` to
   `edd6313d5521e3f8`), and the message says "not placed" beside a placed tree of the same
   version and sum. The class of the critic's item 11.
4. **`b3`**: "name what `util` offers instead" where no module `util` exists. Minor.
5. **Lane b30-pkg's `selfhost/module/private.hero:1`** cites "spec § 1" for the `internal` rule,
   which § 1 does not state. `anchors` cannot see it, because the section exists. Under this
   verdict the citation is corrected to panel 235's R16.
6. **The author's blind spot, `d2`/`f2` against `d1`/`f1`**: not a defect by defect 2357's ruling,
   but a feature or improvement beside 2358. A `path` build could note each constant a fetched
   build will refuse, so the refusal reaches the author on the author's own route.

## 11. What moves under the sitting (item 10)

Lane b30-pkg (`970a650ff`, `d7ff76523`) moves the `internal` rule into
`selfhost/module/private.hero` unchanged (its header: "one rule in one place … It stood in
`package_told.hero`"), so § 4 item 4 stands. Lane b30-fetch (`677bc250e`, defect 2434, a refused
`add` taking back its trees; uncommitted `tests/harness/shell.hero`) moves no message this
verdict reads. Every verdict above is judged on `89db84347`'s messages, run with
`sources 14e9e5ce168a2336`.
