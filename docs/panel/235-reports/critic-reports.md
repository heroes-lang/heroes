# Panel 235, the completeness critic's second pass: the reports

2026-11-07. Read: the five reports, `score.md`, the sealed key, ledger and
grades, my first pass, and the briefs at `328524be8`. My worktree moved to
`3b071e4f1` and its compiler rebuilt from the seed. `manifest.sha256` re-checked:
144 files, none differing, the runner pinned at `b6e3fbeb…6587`. The
compiler-engineer's prototype was rebuilt in my worktree from its twelve
changed files; its `--version` matches theirs (`sources 0b1e13e910551b34`).
Scripts and outputs are in `235-critic-scratch/pass2/`. No paid session and
no API call. No verdict.

## The ceiling, and what each choice rests on instead

1. **The test measured copying an example, not the forms.** None of the 240
   first turns failed on its catalogue or its `use` lines. The four misses
   are program faults: three `silent` (`print(…, "\n")`) and one
   `cannot_infer` (grades 048, 135, 235 and 141). Every answer copied its
   section's example: 120 of 120 TOML catalogues use `[dependencies.<name>]`
   tables, 31 of 31 YAML catalogues indent 2 and 4 spaces, 30 of 30 F-hero
   catalogues indent 4. The `use` lines are 242 `use <x>/x`, 120 `x/x`, 40
   `pkg/x/x`, and not one has the whole path inside the brackets. One repair
   turn ran in all, so no arm's messages were tested. The habit count read 0
   in every row.
2. **What each choice rests on:**
   - **U**: the author's position and question 17, measured (the spec-warden's
     placement table, the compiler-engineer's mark rule). At 30/30/29,
     `score` could print nothing but *the author's form stands*.
   - **The blind seat's veto on U-bare**: its own condition is met. It reads
     *lower it to object if variant 3 matches or beats both … and no measured
     failure comes from the collision rule*, and U-bare scored 30/30 with T3
     at 10/10. So U-bare is kept out by robustness (question 17), not by
     locality.
   - **F**: tied under the registered rule. It rests on the reader sizes in
     lines (JSON 54, reusing the RFC 8259 reader the compiler already has; the
     line format 60; Heroes' syntax 59; the TOML subset 100; the YAML subset
     188), on tokens (a 33-token spread, below anything the test resolves),
     and on hazards. YAML's `1.10` trap fired in 0 of 10 sessions: all ten
     wrote `"1.10"` quoted, and 8 left `v1.2.0` plain. The prototype's TOML
     subset built all 60 angle-TOML first turns, and each printed the
     expected output (`proto_over_answers.py`, 60 of 60).
   - **Q0**: the test never varies the base, and the historian found no page
     that measures it. Go rests on the compiler-engineer's MVS (54 lines) and
     the ffi-pragmatist's `h1` (one sum whether a package comes by git, zip or
     index, measured). The author's criterion, *works best for language
     models*, is unmeasured for Q0, and the record should say so.
   - **Section 14's text**: the ceiling was reached with an example the
     answers copied, so it says nothing for `merged-min`, which drops it.

## Contradictions between seats, each checked

3. **Panel 056's second condition: the compiler-engineer against the
   spec-warden.** The compiler-engineer turns the 13 copies into `path`
   entries (§ 11). The spec-warden's falsifier says *mistyping any field never
   changes which module compiles*, and its § 6 says a local folder has no
   fingerprint. Measured on the prototype:
   - `path = "../decoy/core"`, a folder with the same module names and one
     function changed, builds at exit 0 and prints `attacker.example` where
     the right path prints `localhost`;
   - a mistyped sum, a mistyped version and an edited placed tree each pass
     `check` at exit 0;
   - the deletions hold: an entry deleted with its copy left, or the
     catalogue deleted, gives `unknown_entry`.

   The author's page also lists *a local folder while a package is developed*
   as a source, so the falsifier as drafted refuses the author's own source.
   Both seats are right. The synthesis must name which sources the falsifier
   covers.
4. **The ffi-pragmatist's C1 to C5 against the prototype.** The
   compiler-engineer writes *nothing downstream can tell* a fetched module
   from the program's own, with 0 lines in checker to runtime. Measured on
   the prototype:
   - a placed package that ships `vecmath.h`: `check` exits 0, `build` exits
     1 with `ffi_missing_header`; only `--include heroes.packages/vec` builds
     it, which is defect 2324's route;
   - a placed header that binds a `constant`, built that way, ran its
     constructor during the build: `RAN-AT-BUILD.txt`, two lines, with the
     program never run.

   C1, C2 and C5 need the build to tell fetched units apart
   (`cli/compiling.hero`, `cli/value_run.hero`), and the compiler-engineer's
   own § 5 puts a SHA-256 door in `runtime/`. So the 475 lines and *runtime 0*
   are the prototype's cost, not the first version's. Its scored prediction
   (0 lines under `check/` and `ir/`) still stands.
5. **Fix certainty.** The spec-warden's W3 registers a `certain` fix for
   brackets added and for a domain written in the `use`. Re-run on the
   prototype, the compiler-engineer's rule prints `fix (guess)` for `use
   <geom>/area` and no fix for `use github.com/acme/roman/numeral`. W3 as
   registered is falsified by the rule the sitting should keep; restate it.
6. **The `core/` copies.** The shared brief's *14 copies* is false: `ls -d
   examples/*/core | grep -v '^examples/packages/core$' | wc -l` prints 13,
   and the fourteenth is the canonical tree. Of the 19 `internal/` modules, 2
   are canonical. The compiler-engineer is right. **My first pass's finding 15
   called *14 example programs carry `core/`* a fact that holds**: it holds
   for programs with a `core/` folder, not for copies. Corrected here.
7. **What a placed tree is keyed by.** The compiler-engineer keys it, and the
   C names, by the program's alias; the ffi-pragmatist (§ 7) keys C1's root
   by the package, *never the alias*. They differ where two catalogues use
   two aliases for one package, which is the compiler-engineer's § 16, and
   that is unbuilt.

## Claims asserted and not measured

8. The spec-warden's *U-angle meets* question 17 *on three conditions*: the
   third, every source checked against its fingerprint, holds in no compiler
   that exists (finding 3). It is a condition, not a result.
9. The spec-warden's *33 tokens are worth 0.9 points*: a ratio taken from
   measurement 052, read and not re-run, against a test that measured no
   difference at all.
10. The blind seat said it had no write tool despite `--tools Read,Write`
    (why is unrun), and its verdict line misdescribes variant-3's spelling;
    quote the veto by `variants.tsv`.

## Routes nobody listed

11. **C5 without running fetched code.** A fetched header's integer constant
    can be read from clang's front end. Measured: `enum : long long { v =
    (PKG_LIMIT) }` under `clang -std=c2x -fsyntax-only -Xclang
    -ast-dump=json` gave 1099511627779 for `((1LL << 40) + 3)`, and the
    header's constructor wrote nothing (Apple clang 21; the other platforms
    unrun). Reading fetched headers from the dump alone, and refusing what
    needs the run, keeps Go's goal without extending panel 216's yes. Panel
    218 already reads initialisers from the dump.
12. **The 13 copies as fingerprinted entries from a local static index**
    (`file://`, which the ffi-pragmatist measured with `curl` and `h1`), in
    place of `path` entries. Every entry then stays under the falsifier. The
    compiler-engineer proposed a folder index for the goldens only.

## The question the sitting should have asked

13. **Who writes the catalogue?** The spec-warden finds the fingerprint makes
    the verb necessary; the compiler-engineer's catalogue lists every
    transitive package with MVS's version and its sum. So in use, `heroes
    add` writes the file, and people and models read it, review it and merge
    it. The test measured a hand copying a given sum from an example. A
    format's case rests on reading, diffs and merges, which is what Cargo's
    lock v2 and Bun's text lockfile were changed for (historian § 3), and
    nobody measured that. Tasks that could tell the forms apart: an entry
    without its sum, a dependency with dependencies, two pins of one package,
    a catalogue to edit, and a reading question (*which file does line N
    reach, and what changes if entry X goes?*). About 19 USD remains under
    the cap (ledger: readers 5.4654, the seat 0.2419, the cap probe 0.0005;
    the warden's count reckoned at 0.40).

## What the synthesis must not adopt without a further measurement

1. A format chosen *for language models*: the tie shows only that a model
   copies a given example.
2. `merged-min`'s removal of the format example, on the strength of 236 of
   240.
3. Go over Cargo *for language models*: unmeasured.
4. `path` entries under the falsifier as drafted (they fail it, measured), or
   the falsifier without naming the sources it exempts.
5. Question 17 *met* by U-angle before a fingerprint check passes W4's cases:
   sum, version, tag and path mistyped, and the placed tree edited.
6. The 475 lines as the first version's cost: C1, C2, C5 and the C
   fingerprint door are outside it.
7. W3 as registered.
8. The U-bare veto as a veto: its own condition lowered it.
9. C5 by silence: the prototype runs a fetched header at build. That needs
   the author's word, or the dump-only route of finding 11 measured on the
   four CI legs.
