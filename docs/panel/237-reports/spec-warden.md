# Panel 237, the spec-warden's report

Written 2026-11-08 in worktree `.claude/worktrees/237-spec-warden/`, detached at `163912031`, as I
went. Every number below carries the command that produced it, run in this worktree this session.

## The ceiling, read today

`grep -n '^### 1.6' docs/design.md` prints line 253; lines 255-256 read: *the entire language
specification ... must fit in 16384 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*. Lines 290-293 add the consolidation block's budget, *at most 1,500 real
tokens before the thesis harness has run*. That is the number this report judges against; my agent
file's copy (16384, 2026-10-16) agrees with it.

## The drafts

`build/sw237/heroes-spec-{A,B,C,D}.md` (`build/` is `.gitignore:7`), each the specification with
lines 392-393 replaced by the fence lines of `237-briefs/blind/variant-{2,3,4,1}.md` (the arms A, B,
C, D by `variants.tsv`). `diff spec/heroes-spec.md` prints nothing for A, one changed line for B,
two for C, two changed and one added for D. `shasum -a 256`: A `fea65798...` (= the specification),
B `a2cdf90b...`, C `3cabe593...`, D `ab724fa5...`; `cmp` against the critic's
`build/critic/fences/heroes-spec-*.md` prints nothing for all four.


## The vendored counts, re-made

`./heroes measure build/sw237/heroes-spec-<arm>.md` (stage 1, from the seed; offline, free):

| arm | claude-legacy | cl100k_base | delta (legacy / cl100k) |
|---|---|---|---|
| A | 8181 | 8327 | 0 / 0 |
| B | 8183 | 8330 | +2 / +3 |
| C | 8187 | 8333 | +6 / +6 |
| D | 8210 | 8357 | +29 / +30 |

The brief's and the critic's rows reproduce exactly. Note: stage 1's `measure
spec/heroes-spec.md` exits 1 with *STALE: the recorded count is for 72dff50a68d58c19 and this file
is 718d0e076deb36a5*, because the seed is older than `selfhost/measure/pinned.hero`'s pin
(`SPEC_DIGEST "718d0e076deb36a5"`, `SPEC_REAL_TOKENS 10904`, taken `2026-11-08`); the verdict is
read from `heroes-next`, below.

## My prediction, registered before any real count came back

Written at 12:06 (`date`) on 2026-11-08, before the first `--refresh` of this sitting was sent.

- **A** reads **10904** exactly (the pinned digest is this file's; `count_tokens` is
  deterministic for one model id).
- **B** reads **10904 + 3**, inside **+2 to +5**.
- **C** reads **10904 + 7**, inside **+4 to +10**.
- **D** reads **10904 + 38**, inside **+30 to +46**.
- The real order is the vendored one, B < C < D, and no arm's real delta exceeds 1.6 times its
  cl100k delta plus 2 (B 6, C 11, D 50).
- No arm comes within 5,000 of the ceiling's red line (`REAL_TOKENS + FFI_FLOOR >= CEILING`, 16384
  less 60).

Falsified by any of the four counts below falling outside its range. Scored here, by the four
requests this brief names.

## Which budget rules bind this sitting, read today

- **The ceiling**: `tests/harness/suite_spec.hero` holds `CEILING 16384`, `REAL_TOKENS 10904`
  (`REAL_MODEL "claude-opus-5"`, `REAL_TAKEN "2026-11-08"`), `FFI_FLOOR 60`, and goes red at
  `REAL_TOKENS + FFI_FLOOR >= CEILING` (its line 530), so the red line is 16324 real.
- **The consolidation block's 1,500** (design.md:290-293) binds *the rows from M-buildable-structs
  to M-compatibility-promise ... before the thesis harness has run*. `grep -n` in `ROADMAP.md`:
  M-buildable-structs is row 66, M-thesis-harness row 70 *done 2026-10-24*, M-compatibility-promise
  row 74 *done 2026-10-30*, M-package-manager row 77 **OPEN**. The harness has run (measurement 052,
  the first held-out run) and this sitting is outside the block, so the 1,500 does not bind it.
- **The payment rule** (design.md:323-339): unconditional, every amendment owes a named removal or a
  registered prediction naming an instrument that exists today and the milestone that scores it.
  This binds item 3 whatever its size.
- **The delta gate**: `DELTA_GATE 50` vendored per commit (`suite_spec.hero:214`); D's +30 is under it.
- **The newest ledger row** (`docs/measurements/010-spec-budget-ledger.md:239`, 8327 vendored,
  10,904 real) is panel 235's R13 at M-package-manager, paid by registered predictions.

## Principle 0 (CLAUDE.md § 2), item by item

- **Item 1, `shadowed_header`**: a refusal of a file arrangement at the C boundary, no syntax and
  no meaning of a Heroes program added. CLAUDE.md's Precedence (ranks 3 and 4 read together, panel
  215) lets a measured class of wrong programs close at once; Principle 0 governs forms, and this is
  not one. What it owes is CLAUDE.md § 4's sitting, for the diagnostic class, which this is.
- **Item 2, a package's `-D` or `-U`**: a new case of an existing code, `ffi_package`. Not a form.
- **Item 3, the fence**: B and D use `when 0`, a form the specification already defines (§ 13, the
  paragraph opening *`when 0` after a result names the result that means the call did what it was
  handed*, and the `Member` production's `[ "when" ( integer | "true" | "false" ) ]`); C uses no
  new form either. No arm adds a form, so no arm owes § 2's ticket; each owes §1.6's payment.
- **What WOULD be a form, so the sitting can see it coming**: an escape that lets a build keep a
  shadowing file (a flag, a word in `heroes.toml`). Its ticket would be a program that needs it, and
  the brief's count (0 files in the 85 package `-I` folders, 0 tracked files outside `runtime/`) says
  none is known; re-counted below where I ran it.

## The two thesis contexts

Run in this worktree:

- `shasum -a 256 tests/thesis/contexts/*.md`: `b36c80dd...` (with one worked diagnostic) and
  `6e4979e4...` (without productions), the README's two pins.
- `git show a4e09e061:spec/heroes-spec.md | shasum -a 256` prints `4d8e5251...`, the digest the
  README and the registered block (`prediction.md:131`) name. `git diff --shortstat a4e09e061
  163912031 -- spec/heroes-spec.md` prints *41 insertions(+), 20 deletions(-)*.
- `grep -c 'function sqlite3_close(db: Db consumes) -> i64$'` prints 1 in each context.
- `git grep -n 'tests/thesis/contexts' 163912031 -- tests/harness selfhost` prints nothing; `git grep
  -l b36c80dd616737bd` and `6e4979e404d25efd` print only `tests/thesis/contexts/README.md`. No suite
  reads them; the README alone pins them.
- **They have been scored**: measurement 052 § 4 (*The variant contexts on ten held-out tasks*)
  reports both against `spec` for Haiku 5.5 and Sonnet 5.5, and scores panel 133's third prediction
  and issue 1005 on them.

**What a change to their bytes would cost**: no instrument (none reads them), two sha256 pins and
two vendored counts in the README's prose, and **comparability**: a run of a context whose bytes
moved is not the context 052 scored, and the registered block's own text says *a run of another
context writes another `contexts` field ... which `report` shows as a deviation by construction*
(`prediction.md`, the paragraph before the block). And it would buy nothing true: each is a
snapshot of the specification at `a4e09e061`, already 61 changed lines away from today's, so a
one-line patch makes a file that is neither that specification nor this one.

**My reading**: leave both untouched. Defect 2322's *where to look* names them as copies of the
specification; they are not copies, they are dated measurement inputs, and the closing text should
say so. A future run that wants variant contexts regenerates both from the specification it runs
against, by the README's recipe, with new pins: a snapshot is replaced whole, never patched.

## The four paid requests, and the real counts

Each is one `./heroes-next measure spec/heroes-spec.md --refresh` (`heroes-next --version`:
`heroes 0.6.0`, `sources 0d7a6741345899d8`), the key loaded by `. /Users/joseph/Temp/heroes/heroes-lang/.env`
and only its length printed (`key length 108`). Each invocation sends **three** POSTs to
`https://api.anthropic.com/v1/messages/count_tokens` (`selfhost/cli/refresh.hero`: the probes `x` and
`x x x x x`, then the document), model `claude-opus-5`; so the sitting's four refreshes are twelve
requests, their bodies and answers left in this worktree's ignored `build/measure-refresh/`. For B, C
and D the draft was copied over `spec/heroes-spec.md`, `git diff --stat` printed its change, the
refresh ran, and `cp build/sw237/heroes-spec-A.md spec/heroes-spec.md` restored it: after each,
`git diff --stat` printed nothing and `shasum -a 256 spec/heroes-spec.md` printed `fea65798...`, the
specification's own. No commit, no other paid call.

| request | sent at | file's sha256 | real, `claude-opus-5` | real delta | digest printed | cl100k delta |
|---|---|---|---|---|---|---|
| A | 12:10:21 | `fea65798...` | **10904** | 0 | `718d0e076deb36a5` (= the pin) | 0 |
| B | 12:10:28 | `a2cdf90b...` | **10908** | **+4** | `0c6d3d2f0d46ad87` | +3 |
| C | 12:10:34 | `3cabe593...` | **10910** | **+6** | `5c3352bd742e808b` | +6 |
| D | 12:10:40 | `ab724fa5...` | **10945** | **+41** | `0a57061608028f64` | +30 |

`./heroes-next measure build/sw237/heroes-spec-<arm>.md` printed the same vendored rows as stage 1
(8181/8327, 8183/8330, 8187/8333, 8210/8357). `./heroes-next measure spec/heroes-spec.md` before them: *Headroom: 5480 against the 16384 ceiling
... what is measured against the ceiling is 10964*. The dearest arm, D, puts `REAL_TOKENS +
FFI_FLOOR` at 11005 against the red line's 16384: **no arm is near a budget veto**.

### My prediction, scored

Every clause held: A exact; B +4 inside +2 to +5 (point +3, off by one); C +6 inside +4 to +10 (point
+7, off by one); D +41 inside +30 to +46 (point +38, off by three); the real order B < C < D is the
vendored one; each real delta is under 1.6 times its cl100k delta plus 2 (6, 11, 50 against 4, 6,
41); no arm within 5,000 of the red line (the nearest, D, 5,379 below 16324). D's real-to-cl100k ratio, 41 to
30, is 1.37, near the document's own 1.31 (10904 to 8327): a fence line costs about what prose costs.

## A second prediction, registered as B's payment, before I read any reader session

Registered at 12:13 (`date`), 2026-11-08. **What existed then, said plainly**: lane `p237`'s
`build/b237/reader/` held `run.json` in 40 of its 144 session folders (`find . -name run.json`,
counted by folder name only: R1 A 6, B 6, C 4, D 5; R2 A 3, B 8, C 2, D 6). I read none of them,
and no grade had been published. The coordinator decides whether that makes this a registration; I
hold it to the standard either way.

**The instrument**, which exists today: the reader test's runner and its behavioural grade
(`237-briefs/blind/README.md` § Grading and § The decision rule, the shim from `shim.c`, `store.h`'s
destructor), 18 sessions per task and arm pooled over three models. **Scored at**: this sitting, at
M-package-manager, when the coordinator publishes the grade.

- **P2**: on R2, the store library with no `_v2`, arm **B is preferred over A** by the registered
  rule (fewer `silent` at one-sided Fisher p < 0.05, `correct` not lower at p < 0.05), and arm **C is
  not** preferred over A there.
- **P3**: over R1 and R2 pooled, arm B's `silent` sessions number **at most 2 of 36**.

Falsified by either failing. If they fail, B's +4 real is re-argued under the removal branch
(design.md:341-344), and the candidate removal priced below is what I would put on the table.

## What each arm moves beyond tokens, run here

- **Each fence compiled as `special` builds it** (the block plus `special`'s `main`,
  `suite_special.hero:439-440`), `./heroes-next build` then the binary under `timeout 20`
  (`build/sw237/fence-*`): A, B and D build and print `0`, exit 0. **C is refused**:
  `error[unknown_name]: nothing named `sqlite3_close` is in scope`, exit 1, at the `main`'s
  `_ = sqlite3_close(db)`. So C owes `special`'s program a change, and makes the specification
  teach `_v2` where `examples/sqlite/main.hero:68` binds `sqlite3_close ... when 0` (`grep -n`).
- **Every arm but A moves `fragments`**: `suite_spec.hero:1428` pins § 13's fence byte for byte,
  and *an entry no fence holds any more is red too* (its comment, 1380-1391). The landing commit
  carries the new entry, instruments first (`.claude/rules/spec-shape.md` § How a change to the
  document is made).
- **The shape the defect is about, run**: `build/sw237/busy-{A,B}/main.hero`, the fence's bindings
  plus `examples/sqlite`'s `Stmt`, `sqlite3_prepare_v2` and `sqlite3_finalize`, a statement prepared,
  then `sqlite3_close`, then the finalize. **Under A: exit 0, printing `0`, `0`, `5`**, the close
  refused and the connection left open, in silence. **Under B: exit 134**, the same three lines and
  then `panic: 1 C handle(s) never given back ... 1 taken at main.hero:12:10, in main.main`. B moves
  this program from design.md Part 11's third class, *silent (exit 0 ...); the thesis is measured on
  the third* (design.md:4543-4544), to its second, a loud failure that names its line.

## §1.2, priced

§1.2's formula (`real cost = program tokens x (1 + rewrite rate)`) prices a correction round and has
no term for a silent program: A's binding gives this program **zero** rewrites and a wrong
outcome. B *adds* a correction round on this shape (the abort, then the repair) and that is
precisely the trade the thesis wants, a silent failure turned into a loud one. So B is not paid by
§1.2's rewrite rate, which it raises on this shape; it is paid, if at all, by Part 11's silent class,
which is why its payment below is a prediction about `silent` sessions and not about rewrites.

The per-prompt side: B costs **+4** real on every prompt that carries the specification, C **+6**,
D **+41**. design.md:389-392 reports metric 2's round-trip median at 3,619 tokens (measurement 052;
a document's figure, not re-run by me), so D's extra 37 over B pays only if D prevents one more
silent-or-loud failure in about every 98 prompts than B does, on tasks that bind a refusable close,
which the held-out suite does not contain (052 § 4: *no task of the suite binds C*). Nothing measured
says D buys that; the reader test is the one instrument that could.

## What it displaces, and the payment owed (design.md:323-339)

Headroom is 5,480 (`heroes-next measure`), so nothing is displaced by size. **The proposal names no
payment**, and the rule is unconditional, so as written item 3 cannot land. What I put on the table:

- **For B**: my P2 and P3 above, registered predictions on an instrument that exists, scored this
  sitting. If they fail, a named removal, measured on the vendored tables only (the real is unrun,
  my brief allowing four refreshes): the clause *and a call returning nothing takes no clause* in § 13's
  `when 0` paragraph, **-10** on both tables (`build/sw237/heroes-spec-B-w.md`: 8173 / 8320, so B
  with it is 7 under today's 8327). **It is weaker than it looks, and I say so**: `function
  sqlite3_close(db: Db consumes) when 0` is refused `success_needs_result` and the `Member` production
  admits no `when` without `->`, but `-> () when 0` **passes the production** and is refused by the
  checker alone, `when_on_void` (`build/sw237/{noresult,unitresult}`, both exit 1, each message
  carrying the repair, *drop the clause*). Removing the clause trades a stated refusal for one implied
  by the sentence before it (*an integer ... for an integer result, `true` or `false` for a `bool`*)
  and two diagnostics. A fallback, not my first choice.
- **For D, if it wins**: the prose example at spec line 459, `` `acquires sqlite3_close |
  sqlite3_close_v2` ``, becomes *as the example does*, since D's fence then shows the mark three
  lines from its own `sqlite3_close_v2`: **-11** vendored (`heroes-spec-D-s.md`: 8199 / 8346), real
  unrun; D with it is +19 vendored over today.
- **A paid run I would ask the coordinator for, not made**: one `--refresh` each of `B-w` and `D-s`,
  three `count_tokens` requests apiece, about 11,000 input tokens each, to replace two lower bounds
  with prices.

## Items 1 and 2 owe the specification nothing; design.md owes them text

- **The frequency, counted**: the runtime's opening reads 36 library headers on this Mac (`clang
  @build/shadowed/words-*.rsp -M build/shadowed/opening-*.c`, the runtime's own paths left out; 38
  paths, `stddef.h` and `stdint.h` each twice, 34 basenames). `git ls-files` outside `runtime/`: 849
  `.h` files, **0** bearing one of the 34 basenames. (The 85 package folders' 0 is the critic's,
  not re-run here.)
- **The price of telling every reader**: a minimal sentence after § 13's `package` sentence
  (`heroes-spec-A-item1.md`, *A file in a folder the build names, named like a header of the C library
  the compiler's C reads, `stdint.h`, refuses the build.*) is **+32** cl100k, +33 legacy, on every
  prompt, for a refusal no tracked program meets and whose message names the file, the folder, the
  library header and both repairs (§4.17's promise). It does not pay. Item 2's refusal is of a
  package's answer, which no program's text can write, so a reader could not act on a sentence about it.
- **Unrun, and a question for the Linux and Windows seats**: the opening's list is the platform's.
  On glibc it will include headers such as `features.h`, a name a program's own C shim could
  plausibly take; the count above is this Mac's.
- **The design.md text question 8 asks**, drafted at `build/sw237/design-419-items-1-2.md` (two
  paragraphs, **558** cl100k / 553 legacy) and, if the sitting rules on the environment,
  `design-419-environment.md` (**100** / 102). design.md has no token budget; the counts are what the
  brief asked. The second draft reads panel 055's ruling (design.md:2981-2984, *the search path is
  the machine's own `CPATH` ...*) as the reason the environment's folders are *named* folders: a route
  that **clears** `CPATH` would refuse the one header route that sitting sanctioned.

## design.md §4.19's copy, and why "the same" is not enough

`docs/design.md:2577-2582` built with `special`'s `main` (`build/sw237/design-copy/`): **exit 1,
`error[lend_kept]`** on `path` (no `lent`). The copy was last changed 2026-09-15 (`git log -L
2577,2582:docs/design.md`: `c2303d6ea`); the specification's fence gained `lent` on 2026-09-21
(`git log -S`: `e71d57ff9`, *the default flips*). That it has stood refused since that day is an
inference from those two commits; that it is refused today is measured. Proposal item 3's *design.md
§4.19's copy of the example the same* would add `when 0` to a fence the compiler refuses on another
line. **What I recommend, the robust route**: the copy becomes the landed fence byte for byte
(`constant SQLITE_OK: i64`, `path: cstr lent`, the arm's close), and an instrument holds it there,
`special` building design.md §4.19's first fence beside the specification's or `spec` comparing the
two, landed before the text (instruments first). The conservative route, recorded for the author:
correct the copy by hand and add no check, which is how it drifted the last time.

## The copy design.md §4.19 should carry, if B lands

Byte for byte the specification's fence under B (`build/sw237/fence-B/main.hero`'s first five
lines, which built and printed `0` with `special`'s `main`):

```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64 when 0
```

## The verdict

- **verdict**: **object** to the proposal as written; **approve arm B** under the condition below.
  Not provisional: all four counts are the reader's tokeniser's. Not a veto: no budget breach and no
  form enters.
- **section**: design.md §1.6 (the payment rule, design.md:323-339, unconditional at every level),
  §1.2 (design.md:190-203), Part 11's silent class (design.md:4543-4544), §4.19 (its copy at
  2577-2582; panel 055's `CPATH` at 2981-2984), §4.17.
- **spec_token_delta**, measured, `claude-opus-5` by `--refresh`: A **10904**, B **10908 (+4)**, C
  **10910 (+6)**, D **10945 (+41)**; cl100k +3 / +6 / +30, legacy +2 / +6 / +29. Headroom 5,480; the
  dearest arm 5,379 below the red line of 16324.
- **removal**: none named by the proposal, and that is the problem. Offered: for B, my registered P2
  and P3 (scored this sitting by the reader test), and if they fail the `when` paragraph's *and a
  call returning nothing takes no clause*, **-10** vendored, real unrun, a weak removal (it trades a
  stated refusal, `when_on_void`, for an implied one); for D, the prose example at spec line 459,
  **-11** vendored, real unrun.
- **needed_for_self_hosting**: **no**. The compiler binds no SQLite (`grep -rl sqlite3_close
  selfhost`: five files, every mention a comment, a message or a test); items 1 and 2 are
  robustness (rank 3), not compiler need.
- **argument**: The budget is not the issue: the dearest arm reads 10945 against a red line of
  16324. The proposal lands item 3 with no payment, which §1.6 forbids at every level, and its
  *design.md §4.19's copy the same* adds `when 0` to a copy the compiler already refuses
  (`lend_kept`, exit 1). B is the arm to land: cheapest real (+4, against +6 and +41), keeps
  `special` and both SQLite examples agreeing (C breaks `special`, `unknown_name`), and turns a
  measured silent exit 0 into exit 134 naming the line. Items 1 and 2 owe the specification nothing
  (0 of 849 tracked headers; +32 vendored per prompt); design.md owes them two paragraphs.
- **prediction**: (P1, tokens) B landed byte-equal to `build/sw237/heroes-spec-B.md` reads exactly
  **10908** on `claude-opus-5`, digest `0c6d3d2f0d46ad87`, at the landing commit's `--refresh`; any
  other number means the landed text is not the text priced. (P2, P3, registered at 12:13 above)
  on the reader test's R2 B is preferred over A and C is not; B's `silent` sessions over both tasks
  are at most 2 of 36. Scored this sitting, at M-package-manager.
- **condition**: I move to **approve** when the landing carries, in one commit: a payment (P2 and
  P3 accepted as registered, or the named removal measured real); `fragments`' § 13 entry moved
  with the fence; design.md §4.19's copy made the landed fence byte for byte, held by an instrument;
  and the two thesis contexts untouched. I move **from B to D** if the reader test prefers D over A
  on both tasks and B not on R2, and then D lands with the line-459 removal; **to C** only if C is
  preferred on R2, which P2 says it will not be. I would **veto** an escape that lets a build keep a
  shadowing file (a form, with no program known to need it: 0 of 849), and any landed count with
  `REAL_TOKENS + FFI_FLOOR >= 16384`.

## What I did not do

No commit. No paid call beyond the four refreshes (twelve `count_tokens` requests). No write outside
this worktree; lane `p237`, the trunk and the critic's worktree were read only (`blind/`, the
critic's `build/critic/fences/` by `cmp`, and the names, never the contents, of the reader
sessions' `run.json` files). `git diff --stat` in this worktree prints nothing; `git status --short`
shows only `docs/panel/237-reports/`.
