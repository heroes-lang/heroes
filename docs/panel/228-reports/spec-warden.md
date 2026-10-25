# Panel 228, the spec-warden's report

Written as the sitting goes, 2026-10-25, in my own worktree
`.claude/worktrees/228-spec-warden`, detached at `9a38f1b26`. Every number below carries the
command that produced it, run in that worktree that day.

## 0. The ceiling, reached by grep today

`grep -n '^### 1.6' docs/design.md` gives line 253, *The spec budget*. Read there today
(`docs/design.md:255-257`): **16384 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`**; and (`:291-297`) the consolidation block, the rows from
M-buildable-structs to M-compatibility-promise, *spend at most 1,500 real tokens before the thesis
harness has run*. The payment rule is unconditional at every level (`:323-326`). §4.16 is
`docs/design.md:2215`.

## 1. The coordinator's reading: no route needs a word of the specification

Commands, in the worktree:

- `grep -n -- '--json' spec/heroes-spec.md`: prints nothing, exit 1.
- `grep -n json spec/heroes-spec.md`: line 448 alone, `retains json_object_put`.
- `grep -n -i -E 'capped|\bcap\b|five|at most|suggest|nearby|deterministic|heroes check|stderr|stdout|document' spec/heroes-spec.md`:
  lines 3, 32, 353 and 456; the only `five` is § 13's *one of the five words*, an FFI fact. **No
  sentence of the spec states a cap, an order, a stream, a flag or a document.**
- § 12's holes paragraph, `sed -n '348,350p'`: *`???` is a valid expression anywhere. It is not an
  error: the compiler reports what belongs there (the expected type, what is in scope). A program
  with holes type-checks everything else but produces no binary.*

**The coordinator's reading is confirmed**: no sentence of the spec says where, in what rendering
or under which flag the hole report is written, so no route's move of the report between streams,
renderings or documents changes a word the spec says, and none owes one. § 12 binds the CONTENT
(*the expected type, what is in scope*), and that is where two of the routes' real questions live
(§§ 5 and 6 below).

History, by `git log --oneline -S '<word>' -- spec/heroes-spec.md`, run for `suggestion`,
`deterministic order`, `nearby`, `capped`, `at most 5`, `at most five`, `SUGGESTIONS`: no commit for
any; `five` finds `a3a04d4ee` alone (the FFI's five words). `git log --diff-filter=A` gives
`d10fbec74` as the file's first commit, and `git show d10fbec74:spec/heroes-spec.md` holds the holes
paragraph word for word as today (its lines 155-157). The critic's finding 19 reproduces.

## 2. My compiler and the baseline

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 8.07`; `./heroes build
  selfhost/main.hero -o heroes-next`: `real 88.69`, `user 66.50`; `./heroes-next --version`:
  `heroes 0.4.0`, `sources 451802aa66692eb3`, the shared brief's key.
- `./heroes-next measure spec/heroes-spec.md`: `claude-legacy 7717`, `cl100k_base 7854`, `real
  10297 claude-opus-5, 2026-10-24`, headroom 6087 against 16384, FFI floor 60. The real row is
  printed only when the file's digest matches `SPEC_DIGEST` (`selfhost/measure/pinned.hero:44-47`,
  `:63-64`), so 10297 is the count of these exact bytes; `--refresh` writes only under the ignored
  `build/measure-refresh` (`selfhost/cli/refresh.hero:40-44`).
- The consolidation block's budget: ROADMAP § Where we are, cell *The spec*, says the block spent
  251 of its 1,500 by 2026-10-21 and the opening line gave 2 back on 2026-10-24; that cell is the
  ledger's reading, not mine, and nothing here spends from it unless the cap sentence lands.

## 3. Streams, measured (`build/sw228/main.hero`, a `d: i64 = ???` beside two `Point`s and `dist2`)

| invocation | exit | stdout | stderr |
|---|---|---|---|
| `check main.hero` | 0 | the report, 9 lines | empty |
| `check main.hero --json` | 0 | the report, 9 lines | `{"schema": 2, "kind": "diagnostics", "read": [...], "diagnostics": []}` |
| `check main.hero --brief` | 0 | the report | empty |
| `build main.hero -o m` | 1 | the report | `no binary: 1 hole in main.hero` |

The report lists `d: i64` in scope at the hole of `d`'s own initialiser: defect 2203, reproduced.

**What a model in the Part 11 instrument is told on a hole**: `thesis_grade.hero:166-170` grades
`build`'s refusal `hole` with `message: built.err`, and `thesis_run.hero:676-679` fills the frozen
prompt's `refused` block (`docs/metrics/first-try.md:32-38`, *The compiler refused the program:*)
with that message. By the stream measured above, `built.err` is `no binary: 1 hole in <path>` and
holds none of the report. Read from the code with its stream measured; the end-to-end feed is
unrun (no paid generation is this seat's). **So no route of this sitting changes a byte a model in
the thesis instrument reads**, and none can claim a measured Part 11 effect through it.

## 4. Route C against § 12

Every Heroes diagnostic is `error_kind` or `unsupported_kind` (`selfhost/diag.hero:22-34`, rendered
`error` or `unsupported`), and `check_json.hero` writes no severity. A hole as an entry of
`"diagnostics"` is therefore an `error[...]`, or a third kind nobody has proposed; at exit 1 the
compiler says of a program what § 12 says it is not (*It is not an error*), and at exit 0 it breaks
`.claude/rules/cli-surface.md`'s exit contract. Spec beats compiler (CLAUDE.md § 12), so C ships a
compiler bug by construction unless § 12 is amended, and that amendment owes §1.0's burden.

## 5. Question 2: the cap's sentence, priced and ruled

**The draft priced (S1)**, merged into § 12's parenthesis rather than appended (panel 122), and true
of today's compiler for the two lists it caps (`build/sw228/apply_s1.py`):

```
`???` is a valid expression anywhere. It is not an error: the compiler
reports what belongs there (the expected type, at most five names in scope and
five functions that return it). A program with holes type-checks everything
else but produces no binary.
```

- vendored, `./heroes-next measure spec/heroes-spec.md` on the draft: `claude-legacy 7726`,
  `cl100k_base 7863`, +9 on both, *not a price* (`.claude/rules/spec-shape.md`);
- **real**, `. .env` (key length 108, `${#ANTHROPIC_API_KEY}`), then `./heroes-next measure
  spec/heroes-spec.md --refresh` on the draft, 2026-10-25: **`SPEC_REAL_TOKENS 10307`**, digest
  `00ecc968072de2ea`; against the pinned **10297** of the unchanged bytes (digest
  `5bb452355391e3b6`, taken 2026-10-24, not re-asked today, one paid call per draft): **+10 real**.
  The spec was restored from a copy under a trap and `git diff --quiet -- spec/heroes-spec.md` held.

**What the cap does on a program of real size** (one hole, `probe: str = ???`, put after
`selfhost/check/holes.hero:37` in my worktree, `./heroes-next check selfhost/main.hero`, exit 0,
then restored, `git diff --quiet -- selfhost`): in scope shows the five parameters and `… and 4
more`; functions that return it show five and **say nothing**. A throwaway compiler with
`SUGGESTIONS` 100000 (`build/sw228/heroes-uncapped`, 41.23 s, source restored) on the same hole:
**9 names in scope**, the four cut being `rows`, `none`, `g`, the three nearest the hole, and
`probe` itself, false by defect 2203; **14 functions**, of which the report hides 9 in silence,
`render.render_ty` among them, which the function holding the hole calls at `holes.hero:75`. On the
tree's own 16 holed programs (`build/sw228/caps.sh`): 5 holes reported, no list cut, none at five.

**Ruling: §4.16's sentence is corrected, the spec states nothing, and the report counts every list
it cuts.** Four reasons:

1. **The count at the point of use dominates the sentence.** S1 tells a model that a list *may* be
   cut; it cannot say whether *this* one was, and the function list today does not. `… and 9 more`
   says it exactly, where the model is looking, and design.md §4.17's standard for a message, all
   that is needed without opening another file, applies to a report as well.
2. **A machine never reads the spec.** Every route but C puts the report in a document; its reader
   learns the cap from the document's own counts or not at all. S1 serves only a model that recalls
   § 12 at the moment it reads a report.
3. **A second home for `SUGGESTIONS` with no check.** design.md §1.6 (panel 086): *a number may live
   in a second place only if it is checked or dated*. No instrument holds a number of the spec to
   `holes.hero:28-29`; S1 would need one built.
4. **It could not be paid by a prediction.** The payment rule admits a prediction only on an
   instrument that exists and can score it; the one instrument that puts a model in front of the
   compiler never shows it the report (§ 3), so S1's +10 would owe a named removal, and the
   robust route spends none.

**What a model loses or gains.** Spec route: +10 real tokens in every prompt, holed or not, for a
warning it cannot apply to the list in front of it. §4.16 route: nothing before it writes a hole,
and at the hole the exact count, in both renderings, pinned by the compiler's own test. **The
correction must also fix §4.16's other inexactness**: suggestions are not *ranked by
type-relevance*; they are the functions whose result is the expected type and which the file can
call, in a fixed order (`holes.hero:181-183`), and the names in scope are the declaration's first
five in the resolver's order (`:93-104`), which on the probe kept the parameters and cut the three
nearest locals. Whether nearest-first serves a model better is the sitting's question, not a spec
one, and costs no spec token either way.

## 6. Question 1 in every route: holes beside errors, read against § 12

§ 12's sentence carries no condition: *the compiler reports what belongs there*. Measured
(`build/sw228/both.hero`, `apart.hero`, `name.hero`): a hole beside a type error, even one in
another function, is reported by neither rendering, exit 1, stdout empty; five golden programs hold
a `???` expression beside an error and report none (`discarded-failure.hero:69`,
`fixedbugs-608-...:11`, `hole-suspends-only-unused.hero:12`, `surface-fixtures/certain137-types/main.hero:7`,
and `fixedbugs-363-...:33`, whose hole is itself refused). No sitting conditioning the report was
found by `grep -rn -i -E` over `docs/panel`, `docs/design.md` and `check.hero` for the phrasings
*only when*, *after every stage was quiet*, *no other errors*, *beside an error*, and six issue
searches found no filing of the shape; both are questions, not absences.

- **The spec-neutral repair**: the compiler reports every hole the type stage reached, beside the
  errors, in both renderings: 0 spec tokens, § 12 kept as written. The alternative, a qualifier in
  § 12 (*when nothing else is wrong*), costs tokens (unpriced; I recommend against drafting it) and
  narrows the skeleton-first promise §4.16 makes.
- **What this means for a document**: where the type stage did not run (a parse or name error,
  `name.hero`), the holes are unknown, so `"holes"` must then be **absent, never `[]`**. Route A as
  drafted writes `"holes": []` for *a program with no hole*; written for a program whose stages did
  not reach its hole, `[]` is a false answer to a machine, the `blocking` class.

## 7. Question 3: Principle 0, the stopping rule, and each route's answer

The stopping rule admits a **capability** (a subcommand or a flag) only if the fixpoint, the golden
harness or the Part 11 harness must type it, or on a measured Part 11 effect. **None must**: the
fixpoint checks no holed file, the grader finds a hole by `build`'s `no binary:` (2158's own words,
`thesis_grade.hero:166-170`), and § 3 above shows the instrument never shows a model the report, so
no Part 11 effect is measurable through it. **So a route enters only as the completion of what
`--json` already promises**, *how to print, never what* (`.claude/rules/cli-surface.md` § One
command), and 2091's *this needs no new verb* is right on that reading and on no other. The test it
sets is parity: the document renders exactly the answer the text gives, every item, order, cap and
count, nothing more and nothing less. And the contract's first clause stands beside it: *the
artifact on stdout*, which `check.hero:84-87` already names the report (`io.print_artifact`).

| route | spec tokens | Principle 0 / stopping rule |
|---|---|---|
| A | 0 | completes `--json` for the report, but **empties stdout**, moving the artifact off the stream the contract gives it and changing what every stdout reader of `check --json` on a holed file reads (`--json --dump-scopes` included); that is a *what*, which A' shows is not needed |
| A' | 0 | the cleanest completion: one answer, two renderings, no reader moved; passes if parity holds |
| B | 0 | the contract's own reading (artifact on stdout, as JSON), but it leaves 2158 standing and puts two documents, or text then a document, on one stdout (critic, finding 14) |
| C | owes an amendment of § 12 | a hole becomes `error[...]` or a kind nobody proposed; *It is not an error* is contradicted at exit 1, the exit contract at exit 0; the amendment has no compiler need and a measured harm to the instrument (`hole` moved into `compile_error`): **Principle 0 unmet** |
| D | 0 | as A on parity and stdout; the schema number is panel 193's question, not mine; the only route whose page edit the site's build enforces (`claims.ts:539-542`, both editions red until they say 3) |
| E | 0 | a lossy rendering (a count, or places), so not parity and not 2091; but the one route that cannot put a false name, position or spelling (2203, 2204, 2205) in a machine's hands before their repairs |

## 8. Question 4: the pages a model meets, as they stand

- `site/src/llms.txt:61-66` takes the schema by substitution (`site/src/pages/llms.txt.ts:26`,
  `jsonSchema()`), so it can never say a stale number, and names the document's contents: *codes,
  source locations, notes and suggested fixes ... the files it read and their SHA-256*. It says
  nothing of holes in the document; `:25-27` says *The compiler reports its expected type*, no
  stream.
- `site/src/html/docs/errors.html:39` and its Italian twin are held by `claims.ts:539-542` to
  `JSON, schema N, written to stderr`; `:73` says the report is given and *the check exits with
  status zero* *if there are no other errors*.
- **What the build catches**: a schema number that moves (D) and a stream that moves for the
  diagnostics document. **What it cannot catch**: a member added (A, A', D's member, E) or a second
  document on stdout (B) that the page does not name; no claim reads the document's members.

## 9. Question 4: what the pages owe

Every route but C owes `site/src/llms.txt:61-66` a clause naming the member (A, A', D, E) or the
stdout document (B), since that sentence enumerates the document's contents and a model reads it as
complete; the errors pages' `:39` *The same messages also come out as JSON* likewise. **The build
catches none of these missing**: no claim reads the document's members. The robust order is
instruments first (`.claude/rules/spec-shape.md`'s order of work, applied to the page): a claim in
`claims.ts` reading the writer's top-level members from `check_json.hero` and asking `llms.txt` to
name each, landed with the member.

## The verdict

- `verdict`: **A object** · **A' approve**, on the condition below · **B object** · **C veto** ·
  **D approve** on budget and Principle 0, inheriting A's stdout objection unless it keeps A''s
  stdout · **E approve**, as a first landing only, since it does not answer 2091. Not provisional:
  every count is measured.
- `section`: design.md §1.6 (budget, payment rule, panel 086's second-home rule), §1.0 and CLAUDE.md
  § 2 (C), §4.16 (the cap); spec § 12; `.claude/rules/cli-surface.md` § One command.
- `spec_token_delta`: every route **10297 → 10297 real** (no sentence owed; `grep` above). The cap
  stated in the spec (S1): **10297 → 10307 real, +10** (`measure --refresh`, 2026-10-25, reverted;
  vendored +9/+9). C: the § 12 amendment it owes, unpriced, vetoed on burden and not on price.
- `removal`: none needed, since no route I approve spends a spec token; S1 would owe a named
  removal (no existing instrument can score a prediction about a report no model is shown), and I
  name none because I rule S1 out.
- `needed_for_self_hosting`: **no**.
- `argument`: The spec names no stream, flag, document or cap, so no route owes it a word, and the
  cap's sentence (+10 real) is dominated by a count printed where a list is cut: that serves the
  text reader and the machine, while the spec's *five* serves neither and is an unchecked second
  home for `SUGGESTIONS`. Measured on one real hole: 4 of 9 names and 9 of 14 functions cut, the
  functions silently. A's flaw is not tokens: it empties the artifact stream under `--json`, would
  give a schema to a report § 12 says is wrong (2203, 2204, 2205), and writes `[]` where it did not
  look. C contradicts § 12.
- `prediction`: at the landing of A', B, D or E, `git diff 9a38f1b26 <landing> --
  spec/heroes-spec.md` is empty and this sitting's ledger row reads 0; with the function list
  counted, the probe hole above reads `… and 9 more` under its functions, and `… and 3 more` under
  in scope once 2203's repair drops `probe` (4 today). Either number otherwise falsifies this report.
- `condition`: A becomes approve as A' (stdout untouched), landing after 2203, 2204 and 2205's
  repairs, every capped list counted in both renderings, holes beside errors reported as § 12 reads
  (the member absent where the type stage did not run), and §4.16 corrected instead of the spec; C's
  veto lifts only with a measured Part 11 effect that pays for amending § 12.
