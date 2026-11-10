# Panel 244, the completeness critic's first pass, over the briefs

The critic's final message, saved by the coordinator (a subagent may not write a report file), 2026-11-10.

The briefs need repairs before any seat starts. Five of the framing facts are false or rest on a search too narrow, and the scope leaves out four rule classes I measured as reader-facing.

I ran everything with `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b29-round/heroes-next`. My probes are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p244-scratch/critic/` (`p1` to `p21`, `idx/`, `mypkg/`, `root/`). I wrote nothing in the repository and nothing on GitHub, made no paid call, and read the network only through `gh` (plus one local file:// index and an unreachable loopback address in item 13).

**These hold as written (no repair):**
- **The compiler:** `heroes-next --version` printed `heroes 0.6.0 / sources 14e9e5ce168a2336`. `cmp heroes-next heroes-next2` printed IDENTICAL, so stage 2 and stage 3 match. Both were built at 17:17, after the last change to `selfhost/`, `runtime/` and `seed/` (af67ef596, 16:51). `b29-round` is at 89db84347.
- **The spec counts:** `grep -n '^## 1\.'` gave line 21. The eight `grep -c -i` counts match: internal 0, heroes.packages 0, `path =` 0, heroes fetch 0, vendor 0, private_module 0, fingerprint 1, heroes add 1.
- **Size and ceiling:** `heroes measure spec/heroes-spec.md` printed `real 10898 claude-opus-5, 2026-11-09`. `suite_spec.hero:161-162` holds CEILING 16384.
- **Line citations:** `docs/panel/235-*.md` lines 491-494 and 758, `package_told.hero:259`, `placed_use.hero:148` and `table.hero:183-184` are all as cited.
- **Landing order:** R16 landed at b17a032da (00:58), after R13 at 301ce606c (00:45), both 2026-11-08. `git log -L29,37:spec/heroes-spec.md` shows § 1 last touched by 301ce606c, so it is R13's text; panel 240 changed § 13 only.
- **Rules run:**
  - A placed tree with one line added is refused at check, build and run: `package_unread`, exit 1 each, naming `heroes fetch` (`p7`).
  - Check, build and run never fetch (`p8`, `p8b`).
  - `use <core>/nothere` gives `unknown_package_module` (`p2`).
- **The machine:** `pmset -g assertions` shows Amphetamine holding it awake.

**Repairs and missing items:**

1. **`use <entry>` alone is already in the spec.** The production `"<" ident ">" "/" ident` requires a module after the mark.
   - Command: `heroes parse nocat/main.hero` (no catalogue anywhere).
   - Printed: `error[package_wants_a_module]`, exit 1. It is a refusal at parse time and does not depend on the catalogue.
   - The message offers `use <core>/parse`, "`parse` being a file of the package `core`". Core 0.2.1 has no `parse`; its root holds `strconv`, `strings` and `encoding/json`.
   - Repair: drop it from "what the compiler holds that the spec does not say". The only open point is the message's invented `parse`.

2. **"No record found" about `internal/` was searched only in the synthesis.**
   - `docs/panel/235-reports/spec-warden.md:163-166` lists `internal/` (question 12) and a local folder as a source (question 5) among what "the landed text needs that no section carries".
   - `235-reports/critic-briefs.md:125-127` (item 20) records Q12 as "Unassigned".
   - R13 (`:729-736`) covers neither.
   - Repair: cite these lines. The fact becomes "named as needed, never assigned, R13 silent".

3. **§ 1's present sentence is false for `path` entries.** It says the catalogue "names with its source, exact version and fingerprint".
   - `heroes add --path ../mypkg -C p12` wrote `path = "../mypkg"` alone, with no version and no sum.
   - `run` printed `note: ... a path entry of the catalogue: unverified, no fingerprint holds it`.
   - Adding `version = "1.0.0"` (`p12b`) gives `catalogue_refused: ... names one source: a version from the index, a git address with its tag and commit, or a local path`.
   - A package's own catalogue may not hold a `path` at all (`requirement_refused`, `fetch/build_list.hero:100-103`).
   - Repair: frame this candidate as a correction of the spec, not only an addition.

4. **`internal/` has shapes the brief does not name.**

   | Probe | What it does | Result |
   |---|---|---|
   | `p4` | `main` names `lib/internal/strs` | refused `private_module` |
   | `p4` | the same module reached through `lib.hero` | exit 0 |
   | `p5` | the program's root `internal/x.hero`, named from `main` and from `other/o.hero` | exit 0 both; the owner is empty because `internal_owner` looks for `/internal` |
   | `p6` | a file `lib/internal.hero`, named as `use lib/internal` | refused, with the message "a module under an `internal` folder…", though that file is under no `internal` folder |
   | `p6b` | a root file `internal.hero` | exit 0 |

   So the compiler's rule is "a path part `internal` after the first", Go's element rule. R16's folder wording does not describe it.
   - Repair: put these shapes in the compiler-engineer's brief, and have the drafted sentence state the rule the compiler applies.
   - `p6` is a defect (class adjacent, the message), not filed; I searched with `gh` and found no issue. The coordinator files it.

5. **The C boundary of packages is missing from the scope.** This covers R9's C1 to C5, panel 237's R2 and R3, and defect 2357.
   - I made a local index with package `cpkg` 1.0.0 (`lib.hero` binding `constant HALF: f64` from `c.h`, `#define HALF 0.5`).
   - `heroes run p18/main.hero` printed `error[ffi_constant_type]: HALF in c.h cannot be read without running its header's code, which a fetched package's header never does at build: it is a float`, exit 1.
   - The same two files in the program's own tree (`own18`) printed `2`, `0.5`, exit 0.
   - Spec § 13:428 ("A group's `constant` has no body: the header holds the value.") cannot predict this refusal. Feature 2358 (open) would lift part of it.
   - The same class holds `package_reads_outside` (`cli/package_units.hero:271-289`) and the package-folder stand-in refusals (`cli/package_stand_in.hero:6-20`).
   - Repair: either bring it into scope, with § 13 as its home and the ffi-pragmatist seated, or exclude it with a stated reason. Name panel 243 as the overlap: it is sitting now on the same base, and its Q2 is a `path` package's header.

6. **Task 2402 (open, area:spec, M-core-packages) is omitted, and its trigger has fired.**
   - § 13:374 says "Anything beyond this document — sockets, maths, JSON, databases — comes from C libraries". § 1 meanwhile shows `use <json>/parse`.
   - `gh issue view 2402` says it lands "with encoding/json or before it, through the panel a spec change owes".
   - `packages/core/0.2.1/files/encoding/json.hero` exists; 8a660cb32 (2026-11-10 02:25) is "core 0.2.0 publishes encoding/json".
   - Repair: put it in scope. Its rewrite is the obvious named removal to pay for the additions.

7. **The payment rule is not in the briefs.**
   - `heroes measure` prints "An addition needs a named removal or a pre-registered falsifiable prediction (panel 012)".
   - design.md:322-334 makes the rule unconditional. A prediction pays only if its instrument exists today, so one naming task 2300's paired run pays nothing yet.
   - Repair: the spec-warden names a payment for each sentence.

8. **The headroom figure is short of the mortgage.**
   - The same `heroes measure` output says "Headroom: 5486 … but the FFI floor mortgages 60 of it, so what is measured against the ceiling is 10958".
   - Repair: carry 10958 and 5426, not 10898.

9. **"The real row is provisional" is not what the instrument does.**
   - A one-line draft, measured with `heroes measure ../p244-scratch/critic/s1.md`, printed 36 and 36 vendored, no `real` row, and "No ceiling judges … a count and not a verdict".
   - A spec copy with that sentence inserted read 8350 on cl100k (+36), then `STALE … No verdict is given against a stale count`, exit 1.
   - `measure` run outside a repository root exits 2: "cannot read vendor/tokenizers/claude-legacy.json".
   - Repair, per `.claude/rules/spec-shape.md` § How a change to the document is made:
     - Run `measure` from the seat's worktree root, with the draft in place.
     - Write each vendored delta "as a lower bound, in those words".
     - Price a merged draft against an appended one.
     - Register a W-bound for the landing's `--refresh`, which is a paid call needing the author's yes.

10. **The messages being judged are changing during the sitting.**
    - Lane b30-pkg holds 970a650ff (defect 2431).
    - Lane b30-fetch has uncommitted edits to `fetch/add.hero`, `fetch_all.hero` and `told.hero` (defect 2433).
    - Lane b30-msg edits `cli/produce.hero`.
    - Defects 2431, 2432, 2433 and 2434 are open. I reproduced 2432 in `p13`: `use <match>/x` gives `package_wants_a_name … found match`.
    - Repair: list them, and say the criterion is judged on 89db84347's messages.

11. **Some messages do not carry the rule, so the criterion has "no" answers.**
    - In `p10` the sum is mistyped by one digit. `check` says "the package `core` is in the catalogue and not placed: `heroes fetch` places it at `…core@0.2.1-085faebfe17ea049/`", while the right tree is in fact placed. Only `heroes fetch -C p10` then says `sum_mismatch`.
    - A mistyped version (`p11`) behaves the same way.
    - These are defect candidates, not filed.
    - Repair: add them, with item 1's invented `parse`, to both seats' lists.

12. **Catalogue rules nobody listed. I ran each one; the message carries the rule in every case.**
    - `p9`: an unquoted version gives "a value is a string between double quotes".
    - `p19`: a `[package]` table and `p20`: `[dependencies]` with `core = "0.2.1"` give "a table is `[dependencies.<name>]` … nothing else".
    - `p21`: `^0.2` gives "a version is exact".
    - The `index` line is unstated in the spec, yet every example catalogue carries it (`index = "file://../../packages"`). `p19`'s "nothing else" contradicts it.
    - The spec says "fingerprint" while the key and every message say `sum`. Merging the two words could save tokens.
    - Repair: list these so the spec-warden can rule on them explicitly.

13. **The default index behind `heroes add json` is not served yet.**
    - `fetch/index_source.hero:46-50` defaults to `https://index.heroes-lang.org`, "its address waits on" row 78, which ROADMAP:219 shows as scheduled.
    - Unrun: whether that host answers. I made no network read.
    - `--index https://127.0.0.1:1/idx` gives `unreachable`, exit 2.
    - `--index file:///private/var/empty` gives `source_refused … outside the program's own repository`.
    - Question for the sitting: does § 1 promise a verb whose default source does not exist yet?

14. **Two entries for one package (R3).**
    - Two `path` entries to one folder (`p14c`) give `catalogue_refused`: "the entries `mypkg` and `mp2` name one package".
    - For an index entry the entry name is the index's own name: `core2` gives `absent`.
    - Unrun: R3's case of two dependencies giving one alias to two packages. This goes to the compiler-engineer.

15. **The seating is not explained.**
    - The panel skill says: "anything the spec must state" is a full panel; "When in doubt take the full panel".
    - The brief gives a reason only for the blind seat. The historian is not a paid run, and the ffi-pragmatist's input is item 5.
    - Repair: seat them, or say why their input does not differ (CL-023).
    - A precedent question, unrun: does Go's language spec state `internal`, or only the go command's documentation?

16. **The 1.0.0 promise is missing from the question.**
    - `docs/panel/232-*.md:264-276`: the promise "covers the language the specification describes … its diagnostics' codes … never their words".
    - So a rule put in the spec is promised in substance; a rule left to the messages is promised only by its code. Task 2303 is open on this.
    - Repair: add this consequence to "The question".

17. **The seats' worktrees do not exist yet.**
    - `git worktree list` shows no `244-*`, and `p244-scratch` was empty before I wrote.
    - Repair: the coordinator creates them before launch with `git worktree add --detach .claude/worktrees/244-<seat> 89db84347`.
    - Give the compiler-engineer the cheap route I measured:
      - `add core --index file://../../../../../packages` from scratch printed "added core 0.2.1".
      - A hand-made index is `versions` (`<v> sha256-<sha256 of list>`), `<v>/list` (`shasum -a 256` lines sorted by name) and `<v>/files/`, as `idx/cpkg` shows.

18. **Panel 243 runs at the same time on the same base and may change § 13** (its Q1 is on lines 379-382). Repair: say which sitting lands first, or that the second re-prices against the first's landing.

19. **The author's instruction cannot be checked.** A `grep` for `batch 30|panel 244|b30-docs` in `docs/` and `ROADMAP.md` found nothing, and `gh` searches found nothing either. Repair: cite where it is recorded, or say it was given in conversation and is unrecorded.

20. **Smaller fixes to the seats' briefs:**
    - The spec-warden's "spec § 1's register" should read "its home section's register", since items 5 and 6 belong in § 13.
    - The brief cites lane b29-envfetch's report, but `ls -d .claude/worktrees/*envfetch*` finds no worktree and no path is given. Replace the citation with `p7`'s command.
    - `.claude/rules/spec-shape.md`'s § 13 row still says "packages" in the old pkg-config sense, which panel 240 retired. This is for the landing to fix, not the sitting.