# Panel 238, the spec-warden's report

Seat: spec-warden (design.md §1.6, §1.2, Principle 0's burden of proof; veto on the measured
ceiling or an unmet burden). Worktree `.claude/worktrees/238-spec-warden/`, detached at `27829fbba`.
Written as I go, 2026-11-08 from 20:00 (`uptime`: load averages 4.09 4.43 7.00). Evidence under
`build/238/` in this worktree. **Vendored tables only**: no `--refresh`, no `claude -p`, no API call,
so every *real* figure below is the pinned one and a draft's real cost is **unrun**.

## 0. What I read, and §1.6 as it stands today

- My brief, `00-shared.md`, `proposal.md`, the critic's findings 17, 24 and 28 and its repairs list;
  the compiler-engineer's report (read only, `.claude/worktrees/238-compiler-engineer/docs/panel/
  238-reports/compiler-engineer.md`), whose rule (its item 1) and route (its item 2c) I assume below.
- `spec/heroes-spec.md` in full (491 lines, `wc -l`).
- design.md §1.6 by grep (`grep -n '^### 1\.6' docs/design.md`: line 253): **the ceiling is 16384,
  measured by `claude-opus-5` through `POST /v1/messages/count_tokens`**, with *"the payment rule is
  unconditional at every level"* and the consolidation block's 1,500 real tokens before the thesis
  harness has run. The number in my agent file (16384) agrees with the document today.
- design.md §1.2 (line 190): *real cost = program tokens × (1 + rewrite rate)*; §1.6's last
  paragraphs record metric 2's first run (measurement 052): rewrite rate 0.198 strict, a round-trip's
  median 3,619 tokens.

The compilers: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`/usr/bin/time -l`:
exit 0, `10.59 real 10.10 user`, `build/238/stage1.log`), `./heroes --version` second line `sources
1a1e340778db789a`; stage 2 in its own command (below).

Stage 2: `./heroes build selfhost/main.hero -o heroes-next` under `/usr/bin/time -l`, exit 0,
`130.72 real 98.19 user 23.91 sys`, **746,842,012,700 instructions retired**
(`build/238/stage2.log`); `./heroes-next --version` second line **`sources 62dba14251e6eafb`**, the
briefs' key.

**The spec today**, `./heroes-next measure spec/heroes-spec.md` (`build/238/measure-base.txt`):
`claude-legacy 8188`, `cl100k_base 8335`, spread 147, **`real 10915` (*claude-opus-5,
2026-11-08*, the pinned figure)**, headroom 5469 against 16384, the FFI floor mortgaging 60 of it.
The brief's figures reproduce. No draft below comes near the ceiling, so **the ceiling's veto is
not reached by anything in this sitting**; what binds is the payment rule and Principle 0.

## 1. Does the rule need a line of the specification at all?

### 1a. Where the order could reach a model, run

- **The compiler refuses no order.** In `build/238/probe1/` (the critic's programs, `.hero` files
  copied), `main.hero` is `use zeta` / `use lib/beta` / `use alpha`: `heroes-next build` exit 0,
  the binary prints `6`; `heroes-next check` exit 0; `heroes-next fmt` returns it unchanged; the
  compiler-engineer's `heroes-fix` (its rule repaired, `sources 5db2de271ee28872`, copied into my
  `build/238/` and run only there) prints `use alpha` / `use lib/beta` / `use zeta`, exit 0.
- **No verb refuses a non-canonical file.** `./heroes-next help` lists `fmt`'s one flag,
  `--in-place`; `fmt` has no `--check`, and `build`, `check`, `run`, `test` take no flag about form
  (`selfhost/cli/table.hero:133, 176-182`). Canonical form is held only by this repository's hooks
  and `canonical` suite (F10).
- **§1.2's audited instrument never asks for canonical form.** `git grep -n -i '"fmt"\|canonical'
  -- 'tests/harness/thesis*.hero'` prints nothing over the 23 files (`git ls-files
  'tests/harness/thesis*.hero' | wc -l`). Metric 2's rewrite rate (design.md §1.6: 0.198 strict,
  measurement 052) counts correction turns after a diagnostic; an unsorted block draws none.
- **The spec states no rule that only `fmt` enforces.** The brief's `grep -n -i
  'fmt\|format\|canonical\|sorted\|order' spec/heroes-spec.md` prints **8** lines (131, 213, 216,
  279, 286, 333, 341, 376), none about `fmt` or a `use` order (the critic's count, reproduced);
  `grep -n -i 'blank\|spaces\|emit\|heroes [a-z]'` prints line 51 (*exactly 4 spaces per level; a
  tab is a compile error*, a compile rule) and line 34 (`heroes add`). By my reading of the 491
  lines, every layout rule the spec states is one the parser enforces; that the spec holds **no**
  formatter-only rule is a reading, so a question, not a measurement.
- **The spec holds no `use` block to re-sort**: `grep -c '^ *use ' spec/heroes-spec.md` prints 0;
  its `use` examples are single code spans. No example of the document becomes non-canonical.
- **The base rate of writers given no rule** (`build/238/baserate.txt`, a `python3 -I` pass over
  `git ls-files '*.hero' | grep -v '^docs/'`, 3,564 files, the rule's key: bracketed first, then
  the path with `<` `>` removed by bytes, then the alias): **834 files hold two or more column-0
  `use` lines; 635 (76.1%) are not in the rule's order**; by tree, `selfhost` 585 of 651,
  `tests/harness` 25 of 118, `examples` 4 of 31, the golden trees 21 of 34. One line the pattern
  could not read, a `check` golden's `use` malformed on purpose (`use  #~ expected_module_name`,
  `tests/golden/check/fixedbugs-130-a-block-below-a-line-that-failed-past-its-declaration.hero`). The compiler-engineer's own census by the formatter
  (its item 3a) says the rule moves 638 files; I did not re-run it.

### 1b. The drafts, priced on the vendored tables

Each a copy of the spec with one change (`build/238/drafts/make.py`, every substitution asserted
to match exactly once), measured from the worktree's root by `./heroes-next measure
build/238/drafts/<draft>.md` (`build/238/drafts/measured.txt`). The real row is **unrun**: a
draft's real cost needs `--refresh`, an API call this sitting does not make. The figure in
parentheses is an **inference** by the document's own ratio, 10915 / 8335 = 1.31, never a count.

| draft | the change | claude-legacy | cl100k_base | real |
|---|---|---|---|---|
| base | none | 8188 | 8335 | 10915 (pinned) |
| **D1**, the order stated | § 1, after *No wildcard.*: *`heroes fmt` writes the `use` lines above the declarations, those with angle brackets first, each group sorted by path.* | **+28** | **+30** | unrun (about +39) |
| **D2**, line 131 widened | *The order of declarations and `use` lines never matters but a group's (section 13)* | +7 | +7 | unrun (about +9) |
| **D2b**, line 131, shorter | *Neither declaration nor `use` order matters but a group's (section 13)* | +4 | +4 | unrun (about +5) |
| **D3**, line 164 | *a `use` binds its name for the whole file, wherever it is written, so ...* | +5 | +5 | unrun (about +7) |
| **D4**, line 28, the author's reason | *A file may not bind one name twice, so the order of its `use` lines never matters;* | +13 | +14 | unrun (about +18) |

All under `DELTA_GATE` (50 vendored, `tests/harness/suite_spec.hero:214`), none near the ceiling;
the payment rule binds each all the same (design.md §1.6, *unconditional at every level*).

### 1c. The verdict on a line

**D1 fails Principle 0 and the payment rule, and I would veto it as drafted.** The compiler does not
need it (the compiler compiles itself in either order, §2's closure list unaffected), and no thesis
effect is measurable for it: §1.2's real cost is tokens × (1 + rewrite rate), and an unsorted
block raises the rewrite rate by exactly nothing, because no verb refuses it (1a) and the
instrument that audits the rate never asks for form. Its +30 tokens would be read by every model
that writes Heroes, every prompt, to buy a style decision that design.md §4.15 says the formatter
exists to remove from the model (*"it removes all style decisions from the model (pure spend, no
meaning)"*). A spec sentence telling the model the order is that spend, put back.

**The cost the critic's finding 28 names is real and lands elsewhere.** In this repository, a
multi-`use` file written unsorted is refused by `.claude/hooks/fmt_check.py:349`, whose message
names its own remedy (*"is not canonical: run `heroes fmt <path> --in-place`"*): one mechanical
call, never a rewrite of the program. And this repository already tells its writer to pay it at the
moment of writing (CLAUDE.md § Verification, *"Format at the moment of writing, `heroes fmt <file>
--in-place`"*). Outside this repository no tool refuses the order at all. The reader-facing home of
the rule, if one is wanted, is `heroes fmt`'s help line (the compiler-engineer's draft, item 5:
zero spec tokens, read at the moment `fmt` is used), and its rule's home is design.md §4.15.

**The one route that would make D1 owed**: a `heroes build` (or a mode of it) that refuses a
non-canonical file. Then the order is part of which programs are accepted, so it is language, and
the spec must say it. Nobody proposes that; the compiler-engineer names the order as a checker rule
as refused by Principle 0, and I agree.

## 2. The comment meanings (spec § 1, lines 49-50)

**Assumed**: the compiler-engineer's rule as its report states it (item 1, point 3; item 7) and as
its `heroes-fix` binary writes it, run in my worktree.

The parser's reading is `selfhost/doc_comments.hero:13-20`, `take_docs`: a declaration's doc is the
run of comments on the lines immediately above it, in its column, a `##` ending the run. **A comment
above a `use` documents nothing**, since a `use` is not a declaration (spec line 57, `File = {
NEWLINE } { Use | Declaration } .`, and line 135's `Declaration` lists no `use`). That reading
reaches an output: a hole's report prints each function of the expected type with its doc's first
line (`selfhost/check/hole_report.hero:221-274`).

So the test is by output, in `build/238/docs/`: eight shapes, each with an `i64` hole and
documented functions, run `heroes-next check` on the file as written, `heroes-fix fmt` on it,
`heroes-next check` on the result, and the two hole reports compared with the position line taken
out; then `heroes-fix fmt` again on its output, compared by `cmp`.

| shape | what `fmt` wrote | `fmt` exit | fixpoint | docs the compiler reads |
|---|---|---|---|---|
| s1, F8: `# what helper does` / late `use alpha` / `function helper`, all glued | the comment travels with `use alpha` to the top, glued above it | 0 | yes | **same** (helper had none before) |
| s2, a late `use` glued above a doc glued to `helper` | `use alpha` joins the block; the doc stays `helper`'s | 0 | yes | same |
| s3, `## Tests` / blank / late `use alpha` / blank / doc / `helper` | `use alpha` leaves the section; the heading now heads `helper` alone | 0 | yes | same |
| s4, `use zeta` / `## The rest` / `use alpha` | the heading **rises to the top of the file**, above the whole block | 0 | yes | same |
| s5, `# header` / `# why beta` glued above `use lib/beta`, `use alpha` | the two comments stay on top as the header; `# why beta` now sits glued above `use alpha` | 0 | yes | same |
| s6, a doc glued under the block's last line | stays the declaration's | 0 | yes | same |
| s7, no `use` before the first declaration, its doc on line 1, a late `use` at the end | the block above the doc, a blank line between | 0 | yes | same |
| s8, `# why alpha` glued above the second `use` | travels with it to the top, where the next pass reads it as the header | 0 | yes | same |

**No sentence of the specification becomes false.** *"A comment directly above a declaration
documents it"*: true on every shape before and after, and the compiler's own reading of docs is
byte-identical in all eight. *"`##` is a section heading"*: still a heading in s3 and s4. What
moves is **what a section holds** (s3, s4) and **which line a remark sits next to** (s5, s8), and
the spec defines neither: it never says what a `##` section contains, and it gives a comment above a
`use` no meaning. Neither can mislead a reader about a program, because a `use` binds its name for
the whole file (line 164), so a `use` inside a section was never scoped by it. The compiler-engineer
reports 0 files in the tree holding s4's shape (its `comment-census.txt`, column 6), which I did not
re-run.

**The smallest repair is none.** A sentence giving a comment above a `use` a meaning (*it moves with
its line*) would describe the formatter, not the language, and fails 1c's test for the same reason.
s5's reading (a remark about one line ending above another) is the one place a person reading the
file loses something; that is a formatter quality question for design.md §4.15 and the compiler-
engineer's priced alternative (a blank line under every glued header, 123 files), not the spec's.

## 3. The premise in the specification

**What the spec says or implies about `use` order today**, every line by grep (above):

- line 57, `File = { NEWLINE } { Use | Declaration } .`: a `use` may stand anywhere among the
  declarations, in any order;
- line 28, *"A file may not bind one name twice"*: no order can decide which of two bindings wins,
  which is the author's own reason (*"two equal names cannot be used"*);
- line 164, *"a `use` binds its name for the whole file"*: where a `use` is written is not
  information;
- line 131, *"Declaration order never matters but a group's (section 13)"*: about declarations, and
  a `use` is not one by the grammar's vocabulary (lines 57 and 135), though line 109, *"Every
  top-level line starts with its kind"*, lets a reader take a `use` line for one;
- line 376, *"C reads a module's headers in the order its groups are written"*: inside one module.

So the spec **implies** that the order of `use` lines carries no meaning, and **says nothing** of
the outputs where the compiler-engineer measured it carrying something (its item 2a: the order of
diagnostics and holes, which clash line is blamed, the one-file emission's bytes and verdict, the
link words). The spec names none of those: `grep -n -i 'emit\|heroes [a-z]'` finds no `--emit-c`, and
`grep -n -w -i 'build\|run\|check\|emit-c'` finds no verb (line 366's *ordinary builds* is the
word, not the verb); the one verb the document names is `heroes add` (line 34). **The silence is not a ruling either way**: `grep -rn -i` over `docs/panel/` for the
order of `use` lines finds it only as the C side, panels 200 to 204 (204's R1 rules the order of a
module's own groups, within one module, and refuses a byte sort of them); `gh issue list --search
'use order' --state all` and five more searches (`"use lines" order`, `fmt sort use`, `formatter
order`, `module walk order`, `"written order"`) return no issue that rules the order of `use` lines
in a file, the nearest being defect 563 (groups) and defects 559 and 572 (one module's headers or
flags reaching another's unit).

**Under the route** (the compiler-engineer's item 2c: one key read by the module walk, the binding
loop and the cycle walk), the written order reaches no output but the file's own line numbers. That
makes the compiler match what lines 28, 57 and 164 already imply: CLAUDE.md § 12's *spec beats
compiler*, applied before anybody filed it. **No sentence is owed for it**: the fact is derivable
from three sentences already paid for, and a restatement (D2, D2b, D3 or D4, +4 to +14 vendored)
closes no class of wrong program a reader could write, since every order is accepted with one
meaning. The one reader it would serve is the reader who wonders whether `fmt` may reorder safely,
and that reader is design.md's, where the compiler-engineer's §4.15 draft already says it.

**Without the route**, no spec sentence becomes false either, by the spec's own vocabulary. But the
implication above stays false in `--emit-c`'s verdict (c453: exit 0 written, exit 1 sorted, the
compiler-engineer's item 2a), so a later `fmt --in-place` could change what a verb says about a
program the spec calls one program. That is the compiler-engineer's objection on design.md §4.15,
and from this seat I second it: **the route keeps the spec's implication true at no spec cost; the
rule without it leaves the implication true only in what the spec names.**

**The one case that would owe the spec a sentence**: a correct program whose `build` or `run`
result, not `--emit-c` and not a message's order, depends on the order of its `use` lines (the link
line on a GNU ld leg, F20, is the ffi-pragmatist's question; its report had 15 lines when I read it
at about 20:05 and does not answer it yet). Without the route, such a program falsifies lines 28,
57 and 164's implication for what a program means, and the repair is the route (a robustness
repair, §1.12), not a sentence. With the route, its outcome is fixed by the modules' names; if such
a program could still fail to link, the spec would owe the reader how to fix it, and that is a
sentence of § 13 to price then.

## 4. Principle 0 and §1.2, together

- **Compiler-need**: none. The seed's compiler built `selfhost/` as written, in its own order (stage
  2 above); the compiler-engineer's `heroes-ref` and `heroes-ref3` built the reformatted tree to a
  fixpoint (its item 3d, not re-run here). Neither needs a spec sentence.
- **Thesis**: no Part 11 effect is measured or measurable for any draft without a paid run, and the
  run the critic sized (the spec and a task naming three modules, N samples, the share written
  sorted) would measure style, not the rewrite rate, since the compiler accepts every order. Even a
  result of 100% written sorted with D1, against the 23.9% of the tree's own writers given no rule
  (199 of 834, 1a), moves §1.2's formula by nothing. **I do not recommend that run.**
- **Payment**: the spec delta is 0, so none is owed. Any draft the synthesis takes instead owes a
  named removal or a registered prediction naming an instrument that exists today (design.md §1.6,
  panel 046), and I found no removal the rule makes possible: it adds no fact to the language and
  retires none.

## Verdict

- `verdict`: **approve**: the specification does not move, **with the GNU ld group words in the same
  push as the route** (section 5). **Object** to the route landing without them: line 375 (*`link` a
  library when the symbols need one*) is then false on GNU ld for `dep_*` and `pp_*` in every order,
  and no sentence repairs it truthfully (5c). **Veto held** on D1 (the order stated in
  the spec) if it is proposed without payment, on Principle 0's burden. **Object** to D2, D2b, D3
  and D4 as unpaid restatements of lines 28, 57 and 164, to L1a and L1b as false today (5a), and to
  L1c and N1 as defects written down as rules (panel 204's G1e). **Object** to the rule without the
  route, seconding the ffi-pragmatist's veto (section 5, `condition`). Not provisional: every count is on the
  vendored tables, measured; the real row of a draft is unrun and no verdict rests on it.
- `section`: design.md §1.6 (the payment rule, unconditional; the budget's discipline surviving the
  raise), §1.2 (real cost = tokens × (1 + rewrite rate)), §4.15 (*"it removes all style decisions
  from the model (pure spend, no meaning)"*), and Principle 0 (CLAUDE.md § 2). The comment shapes
  rest on §4.1's adjacency rule as `doc_comments.take_docs` implements it.
- `spec_token_delta`: **0** (`claude-legacy 8188`, `cl100k_base 8335`, `real 10915` pinned,
  2026-11-08, before and after). Priced and not taken: D1 +28 / +30, D2 +7 / +7, D2b +4 / +4,
  D3 +5 / +5, D4 +13 / +14 vendored; after the link table, L1a +17 / +17, L1b +31 / +31, L1c
  +31 / +31, N1 +17 / +17, N2 +10 / +10 (for the refusal's own sitting); every real delta unrun.
- `removal`: none owed, since nothing is added. Had D1 been taken: nothing in the spec is displaced
  by it and no removal was found, and that would be a problem.
- `needed_for_self_hosting`: **no**.
- `argument`: No verb refuses a `use` order, and the thesis instrument never asks for canonical
  form, so stating the order buys §1.2 nothing and spends tokens on the style decision §4.15
  removes from the model. Across eight comment shapes the compiler reads every doc as before. The
  link table does not change the answer: line 375 asks a reader only to link the libraries
  the symbols need, and one GNU ld group around them makes that suffice in every order; a sentence
  instead cannot be true today (a record-only group's library leaves the written order, run here)
  without writing a defect down as a rule. A name two libraries define is older, unspecified, and a
  refusal's sitting. The spec stays.
- `prediction`: (1) **token**: at the commit that lands panel 238's rules, `git diff 27829fbba
  <landing> -- spec/heroes-spec.md` holds no hunk from this sitting and `heroes measure` reads
  `claude-legacy` and `cl100k_base` moved only by other sittings' rows of
  `docs/measurements/010-spec-budget-ledger.md`; if the synthesis takes a draft instead, its
  vendored delta there is the one above to within 1 token, and for D1 its real delta by the
  landing's `--refresh` lies between +30 and +50 (`cl100k_base` +30, the document's ratio 1.31). (2) **rewrite rate**: on the
  first metric-2 run of the thesis instrument after the landing (`tests/harness/thesis.hero`), **0**
  correction turns in either arm are caused by the order of `use` lines (no turn whose triggering
  diagnostic is fixed by reordering `use` lines alone), with or without a spec sentence.
  (3) **link**: once the compiler carries the group words, `dep_*`, `pp_*` and `lp_*` link in
  both written orders on GNU ld (the container's 2.44 and the CI's x86-64 leg), and line 375 stays
  without a sentence; scored by the landing lane's run at M-package-manager's push. And if the
  refusal's sitting states its refusal in § 13, its sentence costs within 5 vendored tokens of N2's
  +10.
- `condition`: I move to **object** to "the spec does not move" (and price a § 13 sentence then) if,
  under the landing compiler with the route and the group words, a correct program that links every
  library its symbols need still fails to link, or its `build` or `run` result depends on its `use`
  order. **Met, so stated**: I **object to the rule without the route**, since the ffi-pragmatist's
  `sig_written` and `dep_written` are programs whose `build` or `run` result depends on their `use`
  order and `fmt` would choose it; its veto on design.md §1.12 is its seat's, which mine does not
  hold, and I second it. I hold the **objection to
  the route without the group words** until a compiler carrying them has run `dep_*`, `pp_*` and
  `lp_*` on GNU ld, and withdraw it if the author rules the library order a reader's duty, in which
  case L1b (+31 / +31) is owed once item 7.5 is repaired, unpaid, and I object to it on the payment
  rule; and I **approve D1** only if `heroes build` is ruled to refuse non-canonical source, or a run
  shows readers of the spec making a compile error, not a style difference, for want of the order.

## 5. After the link table (the critic's second pass)

Asked by the coordinator after the critic's second pass (finding 4: *"the spec-warden's own trigger
fires on the ffi-pragmatist's table, and R7 rests on a report written before it"*). Read, read
only: the ffi-pragmatist's report (416 lines, `wc -l`; items 3, 6, 7 and its Verdict) and the
critic's second pass (344 lines; findings 4, 5, 6 and 11). Same rules: my worktree, vendored tables,
no paid run.

**The critic is right that my trigger fired.** Section 3 said: *"With the route, its outcome is
fixed by the modules' names; if such a program could still fail to link, the spec would owe the
reader how to fix it, and that is a sentence of § 13 to price then."* The ffi-pragmatist measured
exactly that: under the route, `dep_written` and `pp_written` exit 2 (`internal error: linking
failed`) on GNU ld 2.44 in both orders, and link on ld64 (its item 3). My condition as written in
the Verdict (*a result that depends on its `use` order after the route*) did not fire, since under
the route those results are the same in both orders; the sentence in section 3 did. I price it here.

### 5a. One more fact, run here: a module's libraries do not follow its groups' written order

The critic's `onemod` run shows, with two function groups, the libraries in the groups' written
order. The ffi-pragmatist's item 7.5 read in the code, unrun, that a **record-only** group's library
goes after every function group's. Run on this Mac with `heroes-next` (base, `sources
62dba14251e6eafb`), `build/238/onemod/` (two archives built by clang: `libtop.a`'s `top_value` calls
`libbase.a`'s `base_helper`), `heroes-next build <p>.hero --include . --library lib`, then
`grep '"-l' build/*/link-words.rsp`:

| program | groups as written | `-l` words | run |
|---|---|---|---|
| `fn_first.hero` | `base.h` (a function) then `top.h` (a function) | `"-lbase" "-ltop"` | `43` |
| `rec_first.hero` | `base.h` (**a record only**) then `top.h` (a function) | **`"-ltop" "-lbase"`** | `42` |

So item 7.5 reproduces: **inside one module, the libraries follow the written order only among
function groups**. The route touches no line of `emit/externs.hero` (the compiler-engineer's item
2c names three call sites, none there), so I expect the same under it; unrun with `heroes-walk` here.

### 5b. With the group words: is a sentence owed by this sitting?

**Link order: no.** Line 375 already says what a reader owes the link line: *"`link` a library when
the symbols need one."* Measured in C by the ffi-pragmatist and the critic (finding 5), a library
that needs another links in both orders on ld64 bare, on lld bare, and on GNU ld 2.44 inside **one**
group around the package words and the `link` names together (two groups side by side fail; a
package's own group nested inside is accepted). With that, a reader who links every library the
symbols need gets a program that links whatever order anything is written in, which is what line
375 promises and lines 28, 57 and 164 imply. **The group words do not add a sentence: they make an
existing one true.** Without them, line 375 is false on GNU ld today for `lp` (a `link` beside the
`package` it needs, both orders) and `dep_sorted`, and under the route for `dep_written` as well.

This rests on a compiler nobody has built: the critic (finding 5) says the words are *"unmeasured as
a compiler change"*, and `lld-link` on Windows and the CI's x86-64 GNU ld are unrun. My "no" holds
only once that compiler links `dep_*`, `pp_*` and `lp_*` in both orders on GNU ld.

**One C name two libraries define (`dup`, `sig`): no sentence owed by this sitting**, for four
reasons:

1. **The class is older than the proposal.** The ffi-pragmatist's item 6: one name two libraries
   define *"binds whichever comes first, unrefused"*, on every linker measured; a group or lld
   changes nothing. The base compiler already binds it by `use` order (`dup_sorted` prints `2`,
   `sig_sorted` faults).
2. **The spec never gave the reader a lever over it.** The written order that decides it today is
   written nowhere (the ffi-pragmatist: *"it is written nowhere"*). The route moves the choice from
   one unwritten order to another, the modules' names. No sentence of the spec is falsified by
   either: § 13's *"clang checks every result type, constant and record field against that
   header"* is true in `sig`, where the header verifies and the linker takes another library's
   definition; the spec promises the check, not which library supplies the name.
3. **Stating either choice writes the defect down as a rule.** That is the shape panel 204 refused
   (its R4, *"G1e (it writes today's reordering down as a rule)"*; its spec-warden on G1e: *"True
   today on everything, but it writes defect 1 down as a rule"*). `sig` is a memory fault behind a
   verified header: design.md §1.12's class, which the synthesis files `blocking`.
4. **The repair is a refusal, a new diagnostic class, and its own sitting** (the ffi-pragmatist's
   item 6, third route). That sitting owes and prices its own sentence, after it rules the open
   question (a library that overrides a name on purpose, an allocator). A sentence now would rule
   that question in advance.

**What the landing owes instead, and where**: the critic's finding 11 (`dup_written` calls `liba`'s
`pick` today and `libb`'s after the landing, with no message) is a change a reader must be told
(CLAUDE.md § 2: *a change that refuses a `0.x` program is told*; this one does not even refuse).
The place is the decision issue and the release's notes, not the spec: the spec never stated the
binding, so no sentence of it changes. A reading, not run: by `.claude/rules/records.md` § Release
tags, an unchanged spec makes that release a `Z`, and that is right, since the behaviour was never
specified; the telling is what carries it.

### 5c. If the group words were not to land: the sentence that would be owed, priced

Same method as 1b: one change to a copy of the spec, every substitution asserted to match exactly
once (`build/238/drafts/make2.py`), measured by `./heroes-next measure build/238/drafts/<d>.md`
from the worktree's root (`build/238/drafts/measured2.txt`); the real row unrun.

| draft | where, and the text | claude-legacy | cl100k_base | true today? |
|---|---|---|---|---|
| **L1a** | § 13, after *one defining `_GNU_SOURCE` first.*: *Their libraries are linked in that order, one that needs another's before it.* | +17 | +17 | **no**: 5a's `rec_first` |
| **L1b** | L1a, then *so a library that needs another module's belongs in that module.* | +31 | +31 | no, for 5a's reason |
| **L1c** | L1a, then *and two modules' in the order of the modules' names.* | +31 | +31 | true under the route; **the key written down as a rule**, G1e's shape |
| **N1** | § 13, after *wider than C's.*: *A C name two linked libraries define is bound from one of them, unchecked.* | +17 | +17 | true; **a `blocking` defect written into the language** |
| **N2** | the same place: *A function two linked libraries define is refused.* | +10 | +10 | **false until the refusal's sitting lands it**; priced for that sitting |

**What would be owed without the group words**: the reader needs to know how to order libraries on
GNU ld, and **no sentence above is both true and useful today**:

- across modules, under the route, the only lever is the modules' names (L1c, +31), which is G1e;
- inside one module the written order holds only among function groups (5a), so L1a and L1b (+17,
  +31) would state a falsehood, and a true version has to write the record-only exception down,
  which is G1e again;
- and inside one module the header rule of line 376 (*"one that needs another's names comes after
  it"*) and GNU ld's library rule (a library that needs another comes **before** it, the
  ffi-pragmatist's C table, `-ltop -lbase` links and `-lbase -ltop` does not) point opposite ways. A
  group whose header and library both depend on another's has no written order that satisfies both
  on GNU ld. That last is an **inference** from those two measured facts; no single program was run
  for it here.

So without the group words, my recommendation is **not a sentence**: line 375 stands as the rule,
and a correct program that links every library its symbols need and fails on GNU ld is the
compiler's defect against it (spec beats compiler, CLAUDE.md § 12), filed `blocking` as the
ffi-pragmatist classes its items 7.3 and 7.4 for the coordinator to file. If the author rules instead that the order is the
reader's duty, the cheapest owed text is **L1b at +31 / +31 vendored**, and only after item 7.5 is
repaired so that it is true. It would be paid by no removal I can find and by no instrument that
exists today, so it would be **unpaid**: I would object to it on the payment rule, not veto it, since
the ceiling is far.
