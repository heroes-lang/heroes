# Panel 244, the completeness critic's second pass, over the reports

Written as it went, 2026-11-10, by the completeness critic. No verdict. Compiler:
`.claude/worktrees/b29-round/heroes-next` (`heroes 0.6.0` / `sources 14e9e5ce168a2336`), probes in
`.claude/worktrees/p244-scratch/critic/second/`. Read whole: the ten files of `244-briefs/`, the
five of `244-reports/` (the first pass included). Batch 30's round (`b30-round`, at `3b97ebf93`)
read with `git show` and `sed`, never built. Item 4 and resolution 3 answer the coordinator's
message of the same evening on defect 2446. One heavy command at a time; no paid run, no
credential, no GitHub write (`gh issue view` only), no network read beyond `gh`.


## Findings

### The contradiction the brief names: is C5's refusal by design, or the compiler's error?

1. **The ruling is ratified, refusal included, so the command supports the spec-warden's reading,
   that it is by design.**
   - Command: `sed -n 685,688p` and `sed -n 960,962p` of `docs/panel/235-*.md`.
   - R9's item 5 reads *until then a fetched header that needs the probe is refused, never run*.
     The ratification reads **RATIFIED, 2026-11-07** ... *C5 by clang's dump, a fetched header
     that needs the probe refused*.
   - Feature 2358 is filed as a `feature`, not a `defect` (`gh issue view 2358`: `kind:feature`,
     OPEN). Its origin says it refuses floats, records and enumerator readings *as R9's ruling
     says*.
   - Defect 2357's repair (`0ba4e45bd`) rules only that a `path` entry keeps panel 216's probe
     *as the author's own code*. It does not rule the fetched refusal.
   - **The precedent the ffi-pragmatist cites cuts the other way.** Panel 237's R7
     (`docs/panel/237-*.md:119-129`) met the same shape: a ratified ruling (panel 171's `lent`)
     refusing what spec § 13 admitted. It **amended the specification to take in the ruling** and
     kept the compiler's refusal, adding only a `certain` fix. It did not repair the compiler.
   - Defect 2347's label is still `class:adjacent`, though R7's text says *re-classed `blocking`*
     (`gh issue view 2347`).
   - So *by CLAUDE.md § 12 the compiler is wrong* contradicts a ratified resolution. Lifting the
     refusal changes R9's C5, a semantics and diagnostic-class change (CLAUDE.md § 4). Whether
     2358 lands inside C5's *provisional until measured on the four CI legs* or needs its own
     sitting is a question neither seat asked.
   - Bears on: the ffi-pragmatist's verdict (*object* to any § 13 sentence, 2358 re-classed
     `blocking` before the tag) against the spec-warden's d07f.

2. **The spec-warden's approved d07f is false on two shapes the ffi-pragmatist measured, and I ran
   them again.**
   - d07f reads: *A fetched package's header is read and never run, so its group's `constant` is an
     integer or `bool` the header writes as a constant expression, never an enumerator.*
   - Probe `second/k/`: one package `kpkg`, one module per shape, on a hand-made index, each built
     fetched (`app/`) and in the program's own tree (`own/`). Command: `heroes-next build main.hero
     -o bin`, then `timeout 10 ./bin`. Output in `second/k/shapes.txt`.

     | shape (`k.h`) | fetched | own tree |
     |---|---|---|
     | `#define K_S "abc"` as `cstr` | **`abc`, exit 0** | `abc` |
     | `#define K_P ((void *)0)` as `ptr` | **`ptr`, exit 0** | `ptr` |
     | `(1 << 3)`, `((long)sizeof(int))`, `7L`, `'A'`, `18446744073709551615ULL` as `u64`, `((_Bool)1)` as `bool` | 8, 4, 7, 65, the u64, `true`: exit 0 | the same |
     | `static const long K_K = 3;` (an object) | `ffi_constant_type`: *not an integer constant expression ... a GNU extension* | 3 |
     | `enum { K_E = 5 }`, and `#define K_M K_E` | `ffi_constant_type`: *read through `K_E`, an enumerator or object* | 5, 5 |
     | `0.5f` as `f32`, `0.5` as `f64` | `ffi_constant_type`: *it is a float, which no enumerator holds* | 0.5, 0.5 |
     | `((Pt){1, 2})` as a group record `Pt` (`second/k/r/`) | `ffi_constant_type`: *it is a record, which no enumerator holds* | 1 |

   - So d07f's *is an integer or `bool`* is false for `cstr` and `ptr`. That is the flaw the
     ffi-pragmatist found in its own draft B (*no: `cstr` and `ptr` constants build fetched*).
   - Its draft C (*never a float, a record or one read through an enumerator or object*) is true
     on every row above.
   - The cause, read in `selfhost/cli/value_fetched.hero:53-61`: only a constant whose value must
     be read (`kind == "integer" || kind == "unsigned"`) is asked of the dump, and every other
     valued kind is refused. A `cstr` or `ptr` constant is emitted by its C name.
   - The compiler's own NOTE (`:128`, *an integer or `bool` whose expression is an integer constant
     expression*) is d07f's source and carries the same narrowing. It is true of the constants it
     values, and false as a sentence about every constant.
   - Bears on: S1f must not land with d07f as worded. If a § 13 sentence lands, draft C's list of
     exclusions is the one the runs support.

3. **No header the author writes rescues a fetched `f64`. The ffi-pragmatist read this; I ran it.**
   - Probe `second/k/f/`: `#define K_I 1` bound as `constant K_I: f64`, fetched.
   - Printed: `error[ffi_constant_type]: K_I is int in f.h, not f64 — clang read the header and the
     types disagree`.
   - A float literal is refused by kind (item 2). So the note's *Write the value so in the
     package's header* has no header form for a float or a record.
   - Its other branch, *compute it in Heroes*, gives up the header check. The ffi-pragmatist's
     shim route (`18` at exit 0 for a narrowing) gives up the width check.
   - Bears on: the ffi-pragmatist's found-item 1 (the note speaks to an author who cannot follow
     it), now run for the float shape.

### Defect 2446 (the compiler-engineer's D2), the coordinator's added question

4. **The lane's three claims hold, read by grep, and the shape its recommendation closes builds
   today.**
   - **Claim 1, holds.** The exact words *only the program's own catalogue holds* appear 0 times in
     `docs/design.md`, 0 in `docs/panel/235-*.md`, and in no file of `docs/`
     (`grep -c -i` / `grep -rl -i`). Their one home is `selfhost/fetch/build_list.hero:103`, which
     came in with `3d51f8958` (2026-11-08, *M-package-manager step 12: the verbs*;
     `git show 3d51f8958 | grep requirement_refused`).
   - That is a search of words, not of rulings. Panel 235's own lines pairing `path` with a refusal
     (`grep -n -i -E 'path.{0,80}(publish|refus)'`) give R6 and the restated W4 (`:304`), and
     neither states the rule.
   - **Claim 2, holds.** R6 (`docs/panel/235-*.md:612-632`) reads: *a local folder, `path`, the
     author's own source while a package is developed* ... **`path` entries are kept** ... *every
     build that uses one says so on stderr ... and the verb that publishes or records a release
     refuses a catalogue holding one.* No such verb exists. `heroes` has 15 verbs, none publishes.
   - **Claim 3, holds.** R3 (`:567-577`) reads: *The program's catalogue is the build list ... a
     dependency's entry must match one of the program's by identity.*
   - **The shape nobody had run, a package from the index whose catalogue holds a `path`**
     (probe `second/d2/`):
     - `gp` 1.0.0 is published to a hand-made index. Its `heroes.toml` is
       `[dependencies.mypkg] path = "../../mypkg"`, and `g.hero` uses `<mypkg>/lib`.
     - `heroes add gp --index file://../idx` printed `requirement_refused`, exit 1, and left
       `heroes.packages/gp@1.0.0-14de6d58f845abff` placed (defect 2434's shape on this base).
     - I then wrote the catalogue by hand: the `index` line, `gp` by `version` and `sum`, and
       `mypkg` by `path = "mypkg"`, the folder `../../mypkg` resolves to from the placed tree.
     - `check` exit 0 and `build` wrote `bin`, each after the note *`mypkg` ... unverified*. `./bin`
       printed `2`. `heroes fetch` printed `kept gp 1.0.0 ...` / `path mypkg mypkg, read where it
       stands and unverified`, exit 0.
     - I then edited `mypkg/lib.hero` to return 3 and rebuilt: `./bin` printed **`3`**. `gp`'s
       sum was unchanged and nothing refused.
   - So a fingerprinted, fetched package's meaning hangs on a folder no sum holds. What contains
     it is R3: the program must name that same folder itself, and the build notes it as
     unverified. It is declared, not silent. But it is exactly what R6 put the publishing refusal
     there to keep out of a published package.
   - Resolution 3 below weighs the routes.

### The spec half: prices, wordings, payments

5. **The spec-warden's figures reproduce to the token, and the true wording is 20 cheaper than
   d07f.**
   - Command: each set applied by the spec-warden's own anchors (and mine) to a copy in
     `second/price/`, then `heroes-next measure <copy>` run from `b29-round`'s root. That run
     wrote nothing: `find -newer` printed nothing, and `git status` was unchanged. The null copy
     read 8167 / 8314. Output in `second/price/prices.txt`.
   - Every row is vendored, claude-legacy / cl100k, **a lower bound, in those words**:

     | set | delta | note |
     |---|---|---|
     | S1f = d03 + d07f + d08d | +46 / +47 | the spec-warden's figure, reproduced; d07f false (item 2) |
     | d03 | +15 / +16 | reproduced |
     | d08d | −9 / −8 | reproduced |
     | d07f | +40 / +39 | reproduced |
     | C, the ffi-pragmatist's merged draft (*in a fetched package never a float, a record or one read through an enumerator or object*) | +20 / +19 | its figure reproduced; true on every shape of item 2 |
     | C3, *in a package from the index or git never ...* | +24 / +23 | |
     | **C2, *in a package named by `version` or `git` never ...*** | **+28 / +27** | |
     | S1c = d03 + C + d08d | +26 / +27 | |
     | **S1c2 = d03 + C2 + d08d** | **+34 / +35** | |
     | d05b, `internal` as priced | +38 / +39 | reproduced |
     | **I2**, `internal` in 2439's words (*A path with `internal` as a part after its first is named only by the module the path before the last such part names and by the modules under it.*) | **+35 / +37** | |
     | I1, I2 with an example | +58 / +60 | |
     | K1, d08d naming `core`'s `strings`, `strconv` and `encoding/json` | +16 / +16 | |

   - **Why C2 and not C.** The spec never uses the word *fetch* (`grep -c -i fetch` 0) and never
     names the package index: `index` stands on 3 lines, all of an array's index (`:329`,
     `:334`).
   - So *a fetched package* and *a package from the index* name nothing a reader of the spec can
     map to a route. Nothing tells them that a `path` entry, which builds the same constant (the
     ffi-pragmatist's `e2/app`), is not *fetched*.
   - C2 uses the two keys § 1's example shows, `version` and `git`, for 8 more tokens than C.
   - Bears on: the words of the § 13 sentence, if one lands.

6. **`internal`: the spec-warden's decisive reason has moved, the sentence it priced misstates
   nesting, and no seat named a payment for it.**
   - **The decisive reason has moved.** The spec-warden objected because *the rule's shape is
     undecided* (its § 4 item 4, b4). In the round, defect 2439's `af9e95c6c` keeps the element
     rule and makes the message state it.
     - `b30-round/selfhost/placed_use.hero:152-153`, `PRIVATE_RULE`: *a module whose path holds
       `internal` as a part after its first, a folder's name or its own, is named only by the
       module the path before the last such part names and by the modules under it.*
     - So the shape is decided once 2439 reaches `main`. The compiler-engineer's condition on
       2439 is met there too (its § 9).
   - **d05b misstates nesting.** Probe `second/int1/`: `a.hero` writing `use
     a/internal/b/internal/c` printed `error[private_module]: ... is private to a/internal/b`,
     exit 1.
     - d05b (*`a/internal/b` is named only by `a` and the modules under `a/`*) lets a reader
       expect `a` to reach it. 2439's *before the last such part* does not.
     - I2 states it, and costs 3 / 2 vendored less than d05b.
   - **No seat named a payment for `internal`.** The historian approves stating it, naming none.
     S1f's one removal, d08d, is spent on S1f.
     - design.md:324-332 makes the rule unconditional: *Every amendment owes a named removal or a
       registered prediction naming an instrument that exists today*.
     - So `internal` enters only beside a payment of its own. The spec-warden's § 7 names the one
       available: a blind reading, a paid run and the author's.
   - **The two remaining arguments rest on an unmeasured reader behaviour.** The spec-warden says
     a reader meets `internal` only after opening a package's tree. The historian puts above-file
     visibility in the language document (Swift, Kotlin, C#, Java).
     - But the rule binds the program's own tree too. `second/b3/`: `util/internal/bits` named
       from `main` printed `private_module`, exit 1.
     - How often a reader holding only the spec writes such a folder is unrun by every seat. It
       is the blind seat's question.
   - **The closest precedent is Go's, and it sits in the tool's documentation.** The historian's
     four language-document precedents are keywords of the source. A path element, Heroes' shape,
     is Go's, and Go keeps it in the go command's documentation (its § A, `go.dev/doc/go1.4`).
     That is the historian's own table read against its verdict.
   - Bears on: the question's `internal` candidate, and the blind seat.

7. **W is registered as absolute figures that panel 243's landing can falsify without S1f costing a
   token more. Whether a W-bound pays at all is a question no seat asked.**
   - The spec-warden registers *10,940 to 10,975 real* (its § 5) and says in its § 8 item 5 that
     the landing re-prices on 243's text if 243 lands first.
   - Read as a delta over 10,898 the band is **+42 to +77 real**. Only a delta stays scorable
     whichever sitting lands first. It must also be re-registered for whatever wording replaces
     d07f (items 2, 5).
   - **Does a W-bound pay?** design.md:329-334 admits a prediction scored by `heroes measure`.
     But a prediction of a sentence's own cost pays for any sentence the same way. Read so, the
     payment rule binds nothing.
     - `.claude/rules/spec-shape.md:146-180` names the real count as the pricing's last step, never
       as a payment.
     - P2 is a golden guard on the compiler's side, written by the landing itself.
     - So whether S1f is paid by more than d08d's −8 is the synthesis's to rule. Neither W nor P2
       predicts what the sentences do for a reader.
   - Bears on: S1f's payment line.

8. **The compiler-engineer's *no `path` sentence until D2 is ruled* is wider than D2.** d03 speaks
   only of the program's catalogue (*the program's catalogue ... names with its source, exact
   version and `sum`, or by a local `path` alone*). Defect 2446 is about a package's own catalogue,
   and no ruling of it changes d03's truth. The run that d03 corrects is the compiler-engineer's
   own c32 and c12b.

### What depends on 2442 and 2443, and the reader's routes to a package

9. **The spec-warden's objection to the `index` line (d09) depends on 2442 and 2443, both open on
   the round's lanes.**
   - It rests on *the repair belongs to the message, not the prompt* (its § 4 item 6).
   - On `89db84347` the two messages that would carry the rule are a false clause and a fix that
     fails:
     - Defect 2442 (`gh issue view 2442`, `class:blocking`, OPEN): *the catalogue holds nothing
       but the program's dependencies*, beside the `index` line.
     - Defect 2443 (`class:blocking`, OPEN): its body records the coordinator running `heroes
       fetch` where the line was deleted. It printed *`curl` exited 6 reading
       `https://index.heroes-lang.org/core/0.2.1/list`: curl: (6) Could not resolve host*, exit 2.
   - So d09's objection stands only once those repairs make the messages state the line. Until
     then the rule is told by no message that is true.
   - The same run answers my first pass's item 13, unrun then: the default index's host does not
     resolve. I made no network read; this is the coordinator's measurement, as 2443 records it.

10. **§ 1's example names a package that no index serves, and d08d removes the spec's last word on
    where JSON comes from. No seat asked about either.**
    - `ls packages` printed `core` alone. `heroes add json --index file://<repo>/packages`
      printed `error[absent]: ... lists no package json`, exit 1. Against the default index the
      address does not resolve (item 9).
    - `grep -c -w -i core spec/heroes-spec.md` printed 0, while `core` 0.2.1 holds
      `encoding/json.hero`.
    - So a reader who copies § 1's example (`use <json>/parse`, `heroes add json`) reaches no
      package by any route measured. After d08d, § 13 says only *packages or C libraries*.
    - Naming `core`'s modules in d08d's sentence (K1) costs +16 / +16 vendored, +25 / +24 over
      d08d, a lower bound. It would promise `strings`, `strconv` and `encoding/json` for 1.x in
      substance (panel 232: *the functions it names*), while `core` is at 0.2.x.
    - The panel 235 critic's reason to keep the example (236 of 240 readers copied it) is a
      reason its package should exist.
    - Bears on: task 2402's purpose (*saying what a program gets beyond the document*), which
      d08d meets in general words only. This is a question for the synthesis, not a candidate any
      seat priced.

11. **Every seat's probe ran inside this repository, where R9's C4 boundary is the whole tree. In a
    program's own repository the `file://` routes close.**
    - Probe `second/ws/`: `git init` in `ws/app`, which moved `git rev-parse --show-toplevel` from
      the heroes-lang root to `ws/app`.
    - `heroes add --path ../greet` printed `added greet ../greet` / `wrote heroes.toml`, and `run`
      printed the `unverified` note and `hi`, exit 0. **`--path` crosses repositories.**
    - `heroes add kpkg --index file://../idx` printed `error[source_refused]: ... outside the
      program's own repository`, exit 1.
    - A program in no repository has *no `file://` source* at all (`fetch/transport.hero:202-238`,
      read, with its own test at `:343`; unrun by me, since every folder I may write is inside
      this repository).
    - And a git package is the whole repository at its commit (`fetch/git_source.hero:87`,
      `ls-tree -r --full-tree`, read, unrun). So `core`, a subfolder here, has no git route.
    - So a program outside this repository reaches `core` by no route measured today. My first
      pass's *cheap route* (item 17) works only inside it.
    - Bears on: Q-stub's development route (`--path`, which works) and what its guide may teach
      for `core` (nothing that works outside the repository until ROADMAP row 78, `scheduled`).

### Named defects, reproduced or not

12. **The state of each defect a seat found:**

    | defect | reproduced here | state |
    |---|---|---|
    | 2439, `internal` as a file told *under a folder* (p6, b4, c06) | by all three seats with Bash | repaired in the round at `af9e95c6c`, gate owed |
    | 2438, the invented `parse` | by the compiler-engineer | repaired in the round at `52d07264c` |
    | 2440, a mistyped sum or version told *not placed* | c10, c11 | open |
    | 2442, *nothing else* beside the `index` line | b5, c19 | open on a lane |
    | 2443, `index` line deleted, a fix that fails | b10; the failing fix by the coordinator | open on a lane |
    | 2446, D2 | yes, and its index shape (item 4) | open, `blocking` |
    | **D1**, a file the system adds to a placed tree is refused, the file unnamed | **yes**, `second/k/app`: `.DS_Store` written into `kpkg@1.0.0-...` gave `package_unread`, exit 1; `heroes fetch` printed `placed kpkg`, the file gone (`grep -c DS_Store` 0), `check` exit 0 | not filed; that Finder writes it is unrun (a GUI fact) |
    | **b3**, *name what `util` offers instead* where no `util` module exists | **yes**, `second/b3/`, exit 1, `ls util` holding only `internal` | not filed |
    | the ffi-pragmatist's found item 2, `package_reads_outside`'s NOTE (`cli/package_units.hero:297-298`: *searches the runtime's directory, the folders its own `pkgconfig` groups' answer names and its own root, and reads nothing else*) | **yes, by my own run**: `kpkg`'s `k.h` includes the toolchain's `<stdbool.h>`, and every fetched row of item 2 built. That is a read the NOTE says does not happen, and R9's C2 allows it (*the toolchain's own directories*) | not filed |
    | the spec-warden's item 5, `selfhost/module/private.hero:1` citing *spec § 1* for a rule § 1 does not state | **yes**, in `b30-round` after 2439 (line 1 still reads *(spec § 1; panel 235's R16*), its comment still in folder words | not filed |

13. **The historian's departure 3 is wrong in fact on this base.** It reads *Heroes refuses at the
    consumer's fetch, the critic says*. The compiler-engineer's c42 rows and my item 4 show that
    `heroes add` refuses, while `check`, `build`, `run` and `heroes fetch` accept, exit 0. That
    disagreement is defect 2446, and the departure to record waits on its ruling (resolution 3).

### Predictions and the instruments that would score them

14. **Three of the seats' predictions cannot be scored as registered:**
    - **The compiler-engineer's** *`argv.hero` stays at or under 300 ... if and only if the
      operand is one word*. A lane builds one shape, so the other half of the *if and only if*
      is never observed. What can be scored is its 150-to-350-line band, and the ceiling of the
      shape built.
    - **The ffi-pragmatist's** prediction names `p244-scratch/ffi-pragmatist/e4/idx`.
      - That folder is under `.claude/worktrees/`, which `.gitignore` names, so no commit holds
        it.
      - Its `hray` needs Homebrew's `raylib.h` and its `hcurl` the SDK's `curl/curl.h`, both
        this Mac's.
      - Its clang-18 half is unrun by its own words.
      - So it is scorable on this Mac while the folder stands. It is scorable on the four legs
        only if 2358's lane copies the cases into a tracked form.
    - **The spec-warden's W** is absolute (item 7). Its P3 is a paid reading, registered as an
      observation by its own words.
    - The historian registered none (advisory).

### Q-stub, facts the seats left beside each other

15. **The verb's name, the operands and the exit codes, as measured here:**
    - **Name.**
      - `make`: CLAUDE.md:238 reads *never a script, never a Makefile*, and none of the seven
        verbs the historian read is named `make` (its § F).
      - `new` is the new-folder verb in both precedents that write one, Cargo and Gleam. `init`
        is the current-folder verb in four of seven.
      - The prototype writes a new folder.
    - **Operands.** `grep -c -E -i -w`: `program` 12 lines, `package` 5, `library` 1 (C's),
      `project`, `app`, `lib` 0.
    - **Exit codes.** The prototype answers an existing folder with `bad_usage`, exit 2.
      - Run here: `heroes build m.hero -o hasbin` onto an existing folder printed *cannot write
        hasbin: a directory is there; hasbin is untouched*, exit 2. `fetch -C nothere` printed
        `bad_usage`, exit 2. `add --path ../nothere` printed `absent`, exit 1.
      - So the prototype follows the nearest precedent, an occupied output path. No `.hero` input
        is judged, so exit 1 (*the input has diagnostics*) has no input to be about.
    - **The card.** `examples/packages/README.md:183-185`: *A card is written from its face module
      by a program of the net, never by hand ... (task 2294)*. The README's title binds it to *the
      core packages*. A stub's card is a hand card from birth (the compiler-engineer's finding 1).
    - **What a program commits.** The same README, § 1 item 4: the placed files go *under
      `heroes.packages/`, which the program commits, so `build`, `check` and `run` never fetch
      (panel 235's R8)*. `git ls-files 'examples/*/heroes.packages/*' | wc -l` printed 225. So a
      stub writes no ignore line for that folder.
    - **How a package is tested.** The same README, § 1 item 6: *A package is tested through a
      program that reaches it, the way a program fetches it.* The prototype's `next:` line names
      `heroes test greet/greet.hero` and the `--path` route. Every route an author can take
      builds what a fetch refuses (item 2: own tree; the ffi-pragmatist's `e2`: own folder and
      `path`; item 4 under resolution 3: a `path` in a package's catalogue).

### The questions the sitting should have asked and did not

16. **Where does a package's author learn what a fetched build will refuse?**
    - Three measured refusals reach only the consumer:
      - C5's (item 2, the ffi-pragmatist's § 1.1);
      - a package folder's `shadowed_header` note (its § 3);
      - and, under resolution 3's robust route, a `path` in the catalogue of a package named by
        `version` or `git`.
    - `heroes` has 15 verbs and none builds a package as fetched. The one route is a hand-made
      index, which item 11 shows closed outside this repository.
    - Q-stub's package template is where the author's first route is written. A note at every
      `path` build naming what a fetch of that package would refuse is the spec-warden's own § 8
      item 2. A check of a package as fetched is the other route.
    - No seat priced either.

17. **Whether 2358 lies inside R9's C5 or needs a sitting of its own.**
    - C5 reads *Provisional until measured on the four CI legs at the first version*.
    - The ffi-pragmatist's route for a float or a record is a C11 static initialiser compiled
      `-fsyntax-only` (its § 1.5), not *clang's AST dump*, the words of C5 and of 2358's title.
    - It runs nothing, which is C5's goal. Whether a new reading route is a change to a ratified
      diagnostic class (CLAUDE.md § 4) was asked by no seat.

18. **Whether § 1 keeps a verb line every default use of which fails until ROADMAP row 78**
    (items 9 to 11). The spec-warden answers that the spec describes the language at 1.0. The
    sitting did not weigh that a reader obeying § 1 today is refused, which is the reason it gives
    for d03.
    - Run against the repository's own index, `add json` is `absent`, exit 1 (item 10).
    - Run against the default index, `heroes fetch` is `unreachable`, exit 2, by 2443's record.
    - `heroes add json` against the default index is unrun: it would be a network read.

## The resolutions the reports support

### Resolution 1: the question (which candidates enter the specification, in what words)

**The most robust and complete.**

1. **§ 1, merged into the catalogue sentence** (d03, +15 / +16):

   > ... names with its source, exact version and `sum`, or by a local `path` alone, which nothing
   > verifies.

   - Evidence, the runs: a `path` alone builds with the *unverified* note (c1, c32). A `version`
     beside it is refused, so a reader who obeys today's sentence is refused (c12b, c2).
   - Evidence, the rulings and prices: R6 is ratified. Merging costs 17 cl100k less than
     appending (a03). `sum` is the key every message names (b5).
   - Every seat that ruled agrees. The compiler-engineer's condition reaches wider than 2446
     (item 8).
   - It brings `path` entries into the 1.0.0 promise (the spec-warden's § 6).

2. **§ 13's opening, task 2402** (d08d, −9 / −8):

   > Anything beyond this document comes from packages or C libraries; threads, ...

   - This is the named removal, measured in place and reproduced (item 5).

3. **§ 13's `constant` sentence, merged** (C2, +28 / +27):

   > A group's `constant` has no body: the header holds the value, in a package named by `version`
   > or `git` never a float, a record or one read through an enumerator or object.

   - **Why a sentence.** R9's C5 is ratified, refusal included (item 1). Panel 237's R7 is the
     project's precedent: a ratified refusal that § 13 admitted was taken into the specification.
   - **Why it matters.** It is one of the three refusals measured to reach no author (item 16),
     and the one § 13's text contradicts (the ffi-pragmatist's § 1.1, item 2's own-tree column).
   - **Why these words.** d07f is false for `cstr` and `ptr` (item 2). C's *fetched* is a word the
     spec never defines (item 5).
   - **It respects the veto.** C2 names no repair, so it does not meet the ffi-pragmatist's veto
     on bare copied digits.
   - **How long it lives.** As long as feature 2358 leaves the refusal standing. 2358's landing
     commit owes its removal, and the spec-warden's P2 golden makes a forgotten removal loud. If
     2358 lands first, the clause drops (the spec-warden's § 8 item 1).
   - **The set**, S1c2 = d03 + C2 + d08d: +34 / +35 vendored, a lower bound in those words.
   - **Unrun.** The real count, a paid `--refresh` and the author's call.
   - **W** to be re-registered as a delta over the real count of whichever base the landing
     measures on (item 7). Whether W and P2 pay at all is the synthesis's to rule (item 7).

4. **`internal`, in 2439's words** (I2, +35 / +37):

   > A path with `internal` as a part after its first is named only by the module the path before
   > the last such part names and by the modules under it.

   - **It enters only with a payment of its own**: a registered prediction scored by the blind
     reading, which is the author's paid run. That reading runs before ratification, as the
     spec-warden's § 7 asks for any sentence it objects to.
   - Two seats approve stating it: the historian, and the compiler-engineer once 2439 reaches
     `main` (met in the round).
   - The spec-warden's decisive reason moved with 2439. d05b misstates nesting. The rule binds
     the program's own folders, so a reader holding only the spec cannot predict the refusal
     (item 6).
   - Without the reading, it stays out.

**Not entering.** Every seat that ruled agrees on these, and nothing run here moved them:

- `heroes fetch` and `heroes.packages/` (d06);
- the stand-in and outside-reading clauses (d07b, d07c). The ffi-pragmatist re-ran panel 237's
  R8 premise: no tracked package ships a header;
- `use <entry>` alone, which is already in the grammar;
- `unknown_package_module`;
- the `index` line (d09), but only once 2442 and 2443 make its messages true (item 9);
- d10, which resolution 3 would make false.

**Priced by nobody until this pass.** § 1's example names `json`, which no index serves (item 10).
K1, +16 / +16, is the priced form if the synthesis takes it up. It would promise `core`'s modules
in substance.

**The conservative alternative: S0, d03 + d08d alone** (+6 / +8), with no § 13 clause and no
`internal`.

- The C5 refusal is left to `ffi_constant_type`'s message and promised only by its code
  (panel 232). Feature 2358 is the route that ends the contradiction.
- What it leaves, measured: § 13:428 and its grammar keep admitting a program the compiler refuses
  on a ratified rule. A package's author learns of it only from a consumer.

**The ffi-pragmatist's route, recorded beside it**: no clause, and 2358 re-classed `blocking`
before M-core-packages' tag. Three things weigh against taking it now:

- item 1: a ratified refusal, a feature as filed, and R7's precedent running the other way;
- item 17: whether its reading route lies inside C5;
- its own clang-18 half, unrun.

### Resolution 2: Q-stub

**The most robust and complete.**

- **The name is `new`.** It writes into a new folder, refused when the folder exists, as `cargo
  new` and `gleam new` do.
  - `make` is set aside: CLAUDE.md:238 refuses a Makefile, and none of the seven precedents uses
    the word.
  - `init` names the current-folder mode, which nobody asked for (item 15).
- **The operands.** `heroes new <name>` writes a program; `heroes new <name> --package` writes a
  package.
  - One positional word, with the kind a flag that defaults to a program: Cargo's `--bin`/`--lib`,
    SwiftPM's `--type`, Dart's `--template`.
  - It keeps `argv.hero` under its ceiling with no `DECIDED` row. The layout suite measured the
    two-word form at 303 of 300; the one-word form is an inference, scored at the lane's gate.
  - It fits the stopping rule's flag clause: one question, a project folder, answered two ways.
  - It takes `-C <folder>` as `add` and `fetch` do, so a golden case writes under its own work
    folder.
  - The nouns are the spec's, `program` and `package`, never `app` or `lib`: § 13's *library* is
    C's (item 15).
- **For a program it writes `<name>/main.hero` alone**: `main()`, one function with its doc
  comment, one `test`. That is the prototype's text, which `fmt` holds canonical.
  - No catalogue: § 1 says a program that uses no package has none.
  - No ignore file: the program commits `heroes.packages/` (README § 1 item 4, panel 235's R8).
  - No `git init`: `--path` crosses repositories, and a repository changes which `file://` sources
    `heroes add` accepts (item 11). Of the seven precedents, Cargo and Gleam make one, and the
    other five pages say nothing of one (the historian's § F), a silence rather than a measurement.
- **For a package it writes two files.**
  - `<name>/<name>.hero`, the face: a function with its doc comment and its `test` blocks
    (README § 6).
  - `<name>/<name>/internal/<x>.hero`, a private module the face uses (README § 1 item 3, R16).
  - No catalogue.
  - **No card**, until task 2294's writer becomes a verb. README § 8 item 2 forbids a hand card,
    and nothing in `fetch/` or `module/` asks for one (the spec-warden's grep).
  - Its `next:` line names the development route that works: `heroes add --path ../<name>` from a
    program beside it (the compiler-engineer's `app`; item 11's `ws/app`, in a repository of its
    own).
  - It owes the author item 16's warning, that a fetch may refuse what a `path` build accepts.
  - It does not present `heroes test <name>/<name>.hero` as the package's test, since README § 1
    item 6 tests a package through a program that reaches it.
- **It refuses before writing anything, exit 2.** The nearest precedent is `build -o` onto an
  occupied path (item 15). Refused:
  - an existing file or folder of that name;
  - a name holding `/`, `\` or a control character, and `.` and `..`;
  - for a package, a name no `use` writes (`module_text.is_use_path`: a keyword, a foreign word,
    a `-`, a leading digit), as Cargo and Gleam refuse a name their language cannot import;
  - a second name, told in the verb's own words (D3).
- **It writes into a staged folder renamed into place** (`process.rename_over`). A failure then
  leaves nothing that a second run would refuse as already there.
- **Output**: one `wrote <path>` line per file on stdout, then the `next:` line; exit 0.
- **What moves with it**:
  - `table.hero:263`, 15 to 16, and `argv.hero`'s two pinned lists;
  - `claims.ts`'s count and both start pages (*fifteen verbs*, *quindici verbi*), which the site's
    build checks before a push that publishes;
  - design.md §3.5, and `.claude/rules/cli-surface.md`'s record of how the verb entered (the
    author's decision of 2026-11-10, as `probe`'s was of 2026-09-26);
  - about 20 cases in `tests/golden/fetch/`'s shape, and its `LEAST` floor.
- **The specification does not name it.** The spec-warden ruled so, the only seat that ruled. The
  spec names one verb, `heroes add` (`grep -o 'heroes [a-z]+'`), and q1/q3 (+19 / +20) would
  promise the verb's name and shape for 1.x.
- **It depends on resolution 3.** Two packages developed side by side, a second stub's first use,
  have a route only once 2446 is ruled as resolution 3's robust route says.

**The conservative alternative.** `heroes new <name>` writes a program alone. The package template
is left to `examples/packages/README.md` and the guide until three things are ruled:

- the card, which waits on task 2294's writer;
- defect 2446, the development route stopping at the second package;
- how an author sees what a fetched build refuses (item 16).

Or the prototype's two-word shape, with a `DECIDED` row for `argv.hero`, which is a decision (the
compiler-engineer's § 6). The specification names neither.

### Resolution 3: defect 2446, the coordinator's added question

**The most robust: the lane's recommendation, with one addition.**

- **One rule in the build and the verbs.**
  - A `path` in the catalogue of a package the program names by `version` or `git` is refused at
    `heroes add`, at `heroes fetch` and at every build, as `requirement_refused`.
  - A `path` in the catalogue of a package that is itself a `path` entry of the program is
    accepted. It is matched to the program's own entry by identity (R3), and `heroes add --path`
    writes that entry, as it writes `util_2` for a git dependency (the compiler-engineer's c44).
- **The evidence:**
  - item 4's run: a fetched package's output moved from `2` to `3` with its sum unchanged, while
    `heroes fetch` said `kept`;
  - R6's words: a `path` is the author's source while a package is developed, and the refusal
    belongs to the publishing verb, not yet written;
  - item 4's grep: the narrower rule has no ruling;
  - the compiler-engineer's c42c: the compiler already builds the side-by-side case;
  - Q-stub, which needs that route.
- It reads as R6 and R3 applied rather than amended. That is a reading, not a measurement.
- **The addition.** The refusal of a fetched package's `path` reaches only the consumer, item 16's
  class. So the author's `path` build of a package whose catalogue holds a `path` notes that a
  fetch of it will be refused.
- **What follows.** `check`'s message (*`heroes add` writes it whole*) becomes true, and d10 stays
  out of the specification, since under this rule it would be false.

**The conservative alternative.** The verb's present rule is carried into the compiler: every
`path` in any package's catalogue is refused at every build, as at `heroes add`. That is one rule,
the narrower one. What it costs, measured:

- c42c is refused, and no route develops two packages side by side;
- R6's *the author's own source while a package is developed* is narrowed, with no ruling behind
  it (item 4's grep);
- Q-stub's development route stops at the second package.

**Refuted by the run**: the compiler's present acceptance everywhere, with the verb following.
Item 4 shows a fingerprinted package's meaning changing under an unchanged sum, which is what R6's
publishing refusal is there to keep out.
