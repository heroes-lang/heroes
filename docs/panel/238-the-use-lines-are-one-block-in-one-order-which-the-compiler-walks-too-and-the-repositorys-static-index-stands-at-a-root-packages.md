# Panel 238: the `use` lines are one block in one order, which the compiler walks too, and the repository's static index stands at a root `packages/`

2026-11-08, convened by the coordinator on the author's instructions of that day, about 16:15,
under M-package-manager, whose panel gate was asked once at panel 235. **`retro-record`**: the
decision is the author's; the sitting recorded the real objections, measured the costs, settled what
the author left open, and says where each rule lands. Nothing landed in the compiler. **Seats**: the
compiler-engineer, the ffi-pragmatist, the spec-warden and the historian. A completeness critic read
the briefs before any seat started (29 findings, nine of which changed a brief and two of which
seated the ffi-pragmatist and the spec-warden; repaired at `27829fbba`) and the reports after (15
findings, 10 of which changed the resolution), and three seats ran again on its second pass (the
compiler-engineer's heading refinement, the ffi-pragmatist's group words, the spec-warden's reading
of the link table). **No paid run** of any kind, so no llm-ergonomist (process notes).
**Provisional, author ratification pending**, asked by the decision issue drafted at the sitting
(`.claude/worktrees/p238-scratch/decision-238.md`, not yet filed). Every `238-<seat>/build/238/` path
below is a file of `238-reports/evidence.tar.xz` (`238-reports/evidence.md`).

## The proposal, verbatim (`238-briefs/proposal.md`)

1. *"Add that the formatter should group the `use` lines by kind, perhaps putting first those in
   angle brackets and then those from the file system, similar to Go's formatter."*
2. *"The order does not matter anyway, since two equal names cannot be used; it is only something
   nice to look at; then, within a group, go in alphabetical order."*
3. *"Then use as the index the folder at the project's root, something like `packages`."*

The coordinator's reading of 3, told to the author, who may correct it: the repository's static
index moves from `examples/index/` (panel 235's R15) to a folder `packages/` at the repository's
root.

## What the sitting found first: the author's premise is false in this compiler today

**The order a file's `use` lines are written in is the module walk's order**
(`selfhost/modules.hero:79-128`, `load_edited`, depth first over the `use` lines as written; its own
comment, *"Both orders are output, not bookkeeping"*), and every output below is read off that walk.
Each row was measured by the seat named, its program under `build/238/` of its worktree, and every
row the critic's second pass re-ran reproduced:

| what the written order moves today | measured on | who |
|---|---|---|
| which of two clashing lines is blamed, the message, the diagnostics after it, and whether a `Fix` is offered | `clash_a.hero` / `clash_b.hero` | critic (first pass) 5, compiler-engineer 2a |
| the order of diagnostics across modules, and of the holes in `check --json` | `d12`/`d21`, `g12`/`g21` | critic 22, compiler-engineer 2a |
| the order `heroes test` runs the modules' tests in | `t1`/`t2` roots | critic (second pass) 12 |
| the one-file emission's bytes (headers, probes, prototypes), and a blessed emission | `ab`/`ba` (36 lines, the root's name normalised), `examples/shapes` (50) | critic 21 and 23, compiler-engineer 2a |
| **whether `build --emit-c` accepts the program** | `c453`: exit 0 written, exit 1 sorted, `ffi_one_file` | critic 21, compiler-engineer 2a |
| the order of libraries on the link line | `"-lm" "-lc"` against `"-lc" "-lm"` | critic 21, ffi-pragmatist 3 |
| **whether a program links on GNU ld** (an archive that needs another) | `dep_written` links, `dep_sorted` `internal error: linking failed`, exit 2, Linux arm64 | ffi-pragmatist 3 |
| **which C definition a name two libraries define binds**, silently | `dup_written` prints `1`, `dup_sorted` `2`, both linkers, static and shared | ffi-pragmatist 3, critic (second pass) 11 |
| **whether the program runs or dies by a memory fault** (that name with another signature) | `sig_written` `1 30`, `sig_sorted` Bus error 138 (Mac), SIGSEGV 139 (Linux arm64) | ffi-pragmatist 3, critic (second pass) 2 |
| **whether a per-module unit accepts a correct program**, with a false message whose fix leads nowhere | `two_others`: `rs_first` runs `9`, `pw_first` refused `ffi_field_type` | ffi-pragmatist 2, critic (second pass) 2 |

The `--dump-*` printers are order-free (`--dump-ir` and `--dump-scopes` 0 lines, `--dump-ast` the
root's own two lines; critic, second pass, 12). So the premise holds for every program the tree
holds (of 763 roots in `examples/` and `tests/golden/run/`, 12 reach two modules with `extern`
groups, all 12 run the same bytes in every condition, and the one `--emit-c` verdict that moves goes
from refused to accepted; the 7 `examples/` programs the rule reformats build and print byte for byte
the same; no release program from `v0.2.0` to `v0.6.0` has two modules with groups: the
ffi-pragmatist's items 4 and 5) and **fails for programs the language accepts and the tree does not
yet hold**. A sorting formatter alone hands `heroes fmt --in-place` the power to turn
`sig_written`, which runs, into `sig_sorted`, byte for byte (`cmp`, critic 2), which builds at exit 0
and dies by a memory fault (design.md §1.12); no guard, probe or suite of the formatter can see it,
since each compares one file's text and tree.

## The resolution, provisional

**R1. The rule `heroes fmt` writes, as the compiler-engineer built it and measured it**
(`selfhost/print/use_order.hero`; `238-compiler-engineer/build/238/prototype.diff`):

1. Every `use` line of the file joins **one block** above the declarations: the bracketed group
   (`use <name>/...`) first, then the program's own, **a blank line between the two groups**, none
   inside a group.
2. Inside a group the key is **the path with a package's `<` `>` taken out, compared by bytes**,
   which is the order part by part, since `/` (0x2F) sorts below every character a part may hold
   (the critic's second pass tried `$`, `-`, `.` and a non-ASCII letter in a part: each refused by
   the parser): `<core>/y` before `<core2>/x`, `a/b` before `a0` before `a_b` before `ab`. **Case is
   not folded** (`use Zeta` before `use alpha`, gofmt's byte-wise order); three `use` lines in the
   tree open a part with an uppercase letter or `_`, all fixtures. **The alias breaks a tie, then
   the written order.** Not the whole line by bytes (it puts `<core2>` before `<core>`, `>` being
   0x3E; the two keys differ on 2 of the tree's 834 files with two or more `use` lines), not
   case-folded (it needs a tie-break and reads `Zeta` and `zeta` as one), not the bound name (it
   scatters `use emit/decls as emit_decls` away from `emit/`).
3. **Comments, by design.md §4.1's reading of a doc** (`doc_comments.take_docs`): a run of column-0
   comments glued above a `use`, stopped by a `##` heading, is that line's and moves with it; a
   comment on the line itself moves with it (rustfmt 3720's shape handled, critic 9); **a run that
   opens the file and touches its first `use` is the file's header and stays on top** (123 of the 125
   files whose first `use` has a comment glued above it are that shape); every other comment between
   the block's first and last line rises above the block, in its order. A moved run gets a blank line
   above it unless it opens its group, and **never lands glued under another comment** (the
   compiler-engineer's own test found the prototype writing one where the next reading takes it for
   the header, and no judge could see it; repaired, its item 3f). **Known cost, recorded**: a comment glued above the first of
   two or more `use` lines, other than the file's header, travels with that one line, which is what
   rustfmt's issue 6241 reports as a bug (the critic's second pass, finding 9); the tree holds no such
   heading (3 comment runs glued above a later `use`, none heading a run of two or more). **The
   spelling that keeps a heading over several lines is §4.1's remark, a blank line under it**, which
   R1 already raises above the block with the other loose comments (a fixpoint under the rule as
   built). The refinement that would raise such a heading by itself was built and priced (the
   compiler-engineer's section 8, `build/238/heroes-head`, `sources 0daf40043c1364eb`): it needed three
   companion rules before its own output was a fixpoint, grows `use_order.hero` by 40 lines, and
   **gives up one text per layout** (the 241 permutation files that gave one text give two, 144 and
   97, by whether the written order puts another line under the commented one), so it is not taken.
4. **A `use` after a declaration joins the block**, its run with it, and leaves a blank line where it
   stood, so a comment above it never becomes the documentation of the declaration below. Spec line
   164, *"a `use` binds its name for the whole file"*, is why the hoist changes no program;
   `selfhost/cli/response_file.hero`'s `## Tests` loses its two lines, the one thing a reader loses.
5. **The reorder is a pass over the text, before the printer**, judged by its own judge inside
   `print/guard.hero`'s `checked` (every line written is read, as many times, a blank aside;
   `dump(text)` against `dump(order)` with the `use` rows read as a set, every declaration and its
   doc unchanged), the guard's fixpoint check asking it to leave its output unchanged. **The printer
   keeps source order**: F2's policy (*"reordering is where a formatter starts deleting the author's
   remarks"*) stands for the walk that has the one-way comment stream it was written about, and the
   one reorder `fmt` makes is a separate, separately judged step.

**Measured** (the compiler-engineer, items 1 and 3): over 3,564 tracked `.hero` files outside
`docs/`, the exit codes of the rule's `fmt` equal today's on every file (3,076 exit 0, 488 refused by
the parser, all under `tests/golden/`), no exit 2, and **all 827 files it changes are a fixpoint**;
the rule moves **638 files**, and 628 of them keep every declaration on its line. One text per
layout: five entries in all 120 orders and three layouts (361 files) give one text for 241 files and
two for the glued layout, 96 and 24, the 24 being the files where a doc glued to the first entry is
also glued to the header, which the text cannot settle and the rule reads one way, losing nothing;
the alternative, a blank line under every glued header, adds a line to 123 files, 113 of them goldens
whose `.expected` cite lines, priced and not taken. The probe over 15 unsorted or split seeds, family
`single`: **5,055 variants, 5,055 held**, 0 refused by the guard or the reader. **Unmeasured, owed by
the landing**: the compiler's own tests with the repaired rule (the prototype read 1,910 tests and 1
failed, the failure its own test of the glued header, repaired and checked by one `cmp`), and the
`layout` suite's growth rules (`use_order.hero` is 324 lines by `layout`'s count, 24 over the
threshold, so its judge moves out).

**R2. The compiler follows the same order: the route, in the same push as R1.** One key,
`selfhost/use_key.hero` (new, 90 lines by `layout`'s count), read by the module walk
(`modules.hero`'s `load_edited`), the cycle walk (`modules.hero`'s `cycles`) and the binding of names
(`resolve/top.hero`'s `read_uses`); `print/use_order.hero` reads its key from it, so the formatter
and the compiler cannot disagree. Built (`heroes-walk`, `sources 7d731d2212cf5abf` in place,
`01116def3a2e944c` rebuilt by the ffi-pragmatist and by the critic from the same diff): the clash,
the diagnostics, the holes, `heroes test`'s order, the link words and the emission are the same in
both written orders, the emission differing only by the root file's own name. So the order an author
writes reaches no output but that file's own line numbers.

**What R2 changes, said plainly** (the critic's second pass, findings 2 and 11, which the record must
carry): the key decides where the written order decided, and it is as blind to libraries as the
written order was. At the landing, with no message, **three shapes of working program change what
they do**: `dup_written` calls `libb`'s `pick` where it called `liba`'s (`1` becomes `2`),
`sig_written`, which prints `1 30`, dies by a memory fault, and `rs_first`, which prints `9`, is
refused; each with no `use` order left that escapes, the lever having moved to the modules' names.
No tracked program and no release's program has any of the three shapes. **R3 closes them before R2
lands**, and the decision issue and the release's notes say so to a reader (CLAUDE.md § 2: before
`1.0.0` a change is told).

**Robust over conservative**: the conservative route, the rule alone, is **vetoed**, not an option
(the ffi-pragmatist, on design.md §1.12: one `fmt --in-place` turns `sig_written` into a program that
builds silently and crashes); the precedent is Go's (the historian: Go 1.21 fixed the order packages
are initialized in by sorting them by import path).

**R3. The C side: the library words are grouped on GNU ld, and the classes the written order hid are
closed by the compiler, in the round that lands R1 and R2.** The ffi-pragmatist's reading (its item
6): the header side is defended today (wherever headers read together would mean something else, the
compiler refuses), and the library side is not: §4.19's verification asks clang about the header, and
nothing asks which library supplied the definition a verified prototype calls. *"The document does
not cover the libraries' order"*, and no sitting has ruled it (its item 3's search of the issues and
of `docs/panel/`).

1. **The library words in one group on GNU ld**: `-Wl,--start-group` before the package words and
   the `link` names together and `-Wl,--end-group` after them, on the Linux link line
   (`cli/link.hero:151-168`), never on macOS. Measured in C: GNU ld 2.44 links the dependency in both
   orders inside one group and accepts a package's own group nested in it, while two groups side by
   side do not resolve across (the critic, second pass, 5); lld links both orders with or without the
   words; ld64 refuses the words and links both orders bare. **Built as a compiler change** by the
   ffi-pragmatist after the critic's second pass (`238-ffi-pragmatist/build/238/group/group.diff`:
   `cli/link_flags.hero`'s `groups_libraries(target)`, true where the target triple names `-linux`,
   and `cli/link.hero`'s `link_line`, six and two lines of code, with two tests), on the base and on
   the route (`sources 02ffeda04877ee3b` and `50edfd92476704cb`, each built on both platforms, the keys
   equal): on Linux arm64 all six library-order programs (`dep_*`, `pp_*`, `lp_*`) link and print `41
   2` in both written orders under both, where without the words the route fails all six at exit 2
   and the base four; on this Mac every `link-words.rsp` of 13 builds is byte-identical to the base's
   once object paths are normalised. The two new tests pass on both platforms; the compiler's other
   1,910 tests were not run with it, and are owed by the landing. Windows (`lld-link`, whose line the
   words do not touch) and the CI's x86-64 leg unrun.
2. **Four defects older than the proposal, to be filed `blocking`** (drafts in
   `.claude/worktrees/p238-scratch/defects/`, each read by `issues.hero -- draft`, not yet filed): **7.1** one C name two linked libraries define binds
   whichever stands first, silently, and with another signature the program builds at exit 0 and
   dies by a memory fault; **7.2** two other modules' records read in a third's unit follow the walk's
   order, and a correct program is refused with a false message whose fix leads nowhere; **7.3** an
   undefined reference inside a linked library is told as the compiler's own `internal error`, exit 2
   (`only_top`, which no group word reaches); **7.4** a `link` library needing a `package`'s fails on
   GNU ld in every order (closed by item 1). One more, `adjacent`: the clash keeps its better message
   in one order only (R6).
3. **How 7.1 and 7.2 close.** 7.1: **a refusal of a name a group binds that the link took from
   another library than its group's**, which both linkers can report in the same link (GNU ld
   `--trace-symbol`, ld64 `-map`, `nm -m` of the output for shared libraries, measured in both
   orders); it closes `dup` and `sig` whatever the order, R2's key included. A new diagnostic class:
   it lands at once on robustness and its sitting follows (CLAUDE.md § Precedence, ranks 3 and 4,
   panel 215), as panel 237 sat on batch 24's refusals. 7.2: **the compiler decides the order of the
   other modules' headers a unit reads**, panel 204's order search applied to them (the critic's route,
   second pass, 7: `rs.h` before `pw.h` makes both written orders build and print `9`), one clang
   probe per order tried, its cost measured in instructions, and a true refusal kept where no order
   works. Robust over the true refusal alone, which still refuses a correct program `run` accepts
   today. Panel 204's family, so a sitting's.
4. **The order of landing: R1 and R2 land in the round that repairs 7.1 and 7.2**, so that no pushed
   compiler turns a program that runs today into a crash or a refusal. **The conservative
   alternative** is the ffi-pragmatist's own condition: R1, R2 and item 1 in one push, 7.1 and 7.2
   filed `blocking` and repaired after, never deferred (`.claude/rules/verification.md` § Bounded
   discovery). Either way **the four `blocking` defects hold M-package-manager's tag**
   (`records/tagged`, CLAUDE.md § Verification) until they close (the critic, second pass, 6).
5. **The group words are judged before the push, on the arm64 container**: the landing compiler's
   `run` form and `dep_*`, `pp_*`, `lp_*` linked there. An exception to
   `.claude/rules/verification.md` § The optimistic chain's *no platform leg before the push*, said
   as one, because the change alters every Linux link line and the CI would otherwise be its first
   judge (the critic, second pass, 15); robustness (rank 3) over process (rank 5). The conservative
   alternative is the chain as written, a red Linux leg reverted.

**R4. The goldens that pin the written order** are renamed so the order each pins is the sorted
one, the only route that survives R2 (an exempt case written unsorted is walked sorted all the same):
defect 560's default case (`cap_ten` to `then_cap_ten`; measured, a compiler from before the repair,
`55d20a362`, fails the renamed case, and the sorted unrenamed case passes on it, so it stops
guarding), `unsupported`'s 555 and 2136, `surface-fixtures/onefile453type` and `onefile453macro`;
2143's `.expected` takes the walk's order (its subject, *every unit is told*, holds); the 563 case's
comment is made true of the sorted text; `mark235` is rewritten in the canonical form. The other two
order goldens (560 *two ways*, 563) still catch their defect in either order. **Each renamed case
says in its header that its modules' names are chosen for the key, and `use_key.hero`'s test names
them**, since a later change of the key would reverse them in silence as the reformat would have
(the critic, second pass, 14).

**R5. `check --apply` does not sort.** Its one `certain` fix that writes into a `use` line
(`selfhost/parse/use_line.hero:273`, `use "zeta"` to `use zeta`) may leave a block `fmt` would
reorder; `--apply` repairs meaning and `fmt` repairs form, and putting the formatter inside the fix
writer for one fix is refused on cost. A file written by `--apply` in this repository is refused by
the hook until `fmt --in-place` runs, as any unsorted write.

**R6. The clash keeps its better message in every order** (`adjacent`, drafted): under R2 the pair
always takes `clash_b`'s outcome, two errors and no fix; the order-free repair blames the aliased
line of the two wherever it stands, since its alias is what the author can change
(`resolve/top.hero`, unbuilt).

**R7. The specification does not move** (the spec-warden, read again after the link table and the
critic's finding 4; 0 tokens; `claude-legacy 8188`, `cl100k_base 8335`, `real 10915` pinned). No verb
refuses an unsorted program and the thesis instrument never asks for form, so a sentence stating the
order buys §1.2 nothing and spends tokens on the style decision §4.15 removes from the model: **D1**
(*`heroes fmt` writes the `use` lines above the declarations, those with angle brackets first, each
group sorted by path*, +28 / +30 vendored) refused without payment, D2 to D4 refused as unpaid
restatements of lines 28, 57 and 164, which already imply that the order carries no meaning; across
eight comment shapes the compiler's reading of docs is byte-identical before and after the reorder.
**With R3's group words, no link sentence is owed**: spec line 375 already gives the reader the whole
duty (*`link` a library when the symbols need one*), and the words make it true on GNU ld. Without
them no true sentence exists today: inside one module a record-only group's library is linked after
the function groups' whatever the written order (the spec-warden's new run, `onemod/rec_first.hero`),
so *"a module's libraries are linked in its groups' written order"* (+17 / +17) is false, and writing
the key's order down as a rule (+31 / +31) is what panel 204 refused as G1e; a correct program that
fails on GNU ld is the compiler's defect against line 375. 7.1's sitting prices its own sentence
(the spec-warden's draft stating the refusal: +10 / +10 vendored). Every real delta unrun.

**R8. Where the rule is written.** design.md §4.15, a sentence appended to the bullet at line 2252,
the compiler-engineer's draft (its item 5) with one correction (the critic, second pass, 13): the
reorder *"reaches no output but that file's own line numbers"*, never *"no output can see"*, since the
blank line between the groups and the hoist move the line numbers of 10 files, which `#line`s,
positions and five blessed emissions see. `selfhost/print/fmt.hero:109-114`'s policy comment replaced
by its draft (the printer keeps source order; the one reorder is `print/use_order.hero`'s). `heroes
fmt`'s help (`selfhost/cli/table.hero:182`, *"print the canonical form"*) unchanged, still true; no
site page's words move (`start.html:110` already says *"fixed rules for indentation, line breaks and
order"*), and R9 keeps *byte for byte* true at every pushed commit.

**R9. The landing order, one lane and one push, as a plan the landing lane settles under the hooks**
(the critic, second pass, 10): commit 1, the rule and the route with their tests, the files it edits
formatted in the same commit (six of them are unsorted today, the seventh the critic counted being
`cli/link.hero`, which R3's group words edit, and `staged.py` refuses a commit of one once the tree's
compiler knows the rule), and the goldens and emissions they move (R4, five
emissions re-blessed); commit 2, the tree formatted by commit 1's compiler (586 `selfhost`, 25
`tests/harness`, 10 `examples`, 4 more `tests/golden/run`; five emissions whose line numbers move
re-blessed); commit 3, the index (R10); commit 4, the records. R3's group words and 7.1 and 7.2's
repairs travel in the same round. The trees `canonical` does not walk (`check`, `unsupported`,
`permissive`, `full`, `surface-fixtures`) stay as written, their text being the fixture. **The
landing regenerates the seed** (robust): the hooks judge with `<tree>/heroes`, a seed-built compiler
cannot format a file with a bracketed `use` at all (defect 2352, open, `blocking`, which owes the
refresh anyway) and a stage 2 from before the landing removes the blank line the rule writes between
the groups, so a fresh tree would be told *not canonical* of what `canonical` requires; and every open
tree rebuilds its `heroes` after the merge (`./heroes build selfhost/main.hero -o heroes`, the hook's
own hint). The conservative alternative: no seed refresh, the rebuild named in every tree. A lane
opened before the landing never resolves a `use` block by hand: it takes its own side, rebuilds its
compiler from the merged tree and runs `heroes fmt --in-place` on the conflicted files. **The gate
waits on batch 25's blessing** of the runtime's key (at the briefs' commit `emission` read 771 failed
on `hero_program_sources` alone, the runtime having moved at `82f9d0a5a` after the last blessing at
`a789691f1`). After the push: the formatter's probe by hand, the census of `check`, panel 187's R2,
the CI's legs. The list, file by file: `.claude/worktrees/p238-scratch/landing.md`, carried into
the landing lane.

**R10. The index moves to a root `packages/`**, the coordinator's reading, measured sound: `git mv
examples/index packages`, the 14 catalogues' line `index = "file://../../packages"`, which the
`file://` rule accepts (`heroes fetch` and `heroes add` against it, exit 0, the programs printing what
they print today), and **every placed folder renamed** (the key holds the `index` line as written,
`core@0.1.0-db7f072b4678b0b1` to `core@0.1.0-2e3e1bba9c8acea1`, 84 files under 14 folders, the old
ones `git rm`ed since `fetch` leaves them). It owes three lines the instruction did not name: the
site's `checkIndex()` called directly (with `INDEX` changed and nothing else the site's build is green
and blind) and `packages/**` in `deploy-site.yml`'s `paths:`, so the index becomes a site path; and
`suite_packages.hero:91`'s `INDEX_LINE`. The walks that reached the index through `"examples"`
(`canonical`, `probe`, `compat`, `cards`' `AREAS`) are told to name it. A plain `use` still cannot
reach the index's sources (`use packages/core/0.1.0/files/strings` is refused at the version's dot,
the critic's first pass, 15), by the index's layout, not by the folder's name.

**R11. The name `packages/`, the author's word, with its measured collision repaired by rewording,
never by an exemption.** `records/citations` reads the `packages` suite's 13 check labels
(`packages/placed` and twelve more, `suite_packages.hero:25-56`) as paths under the new root and
refuses them; teaching the check otherwise is the exemption list its own message says it does not
have (the critic, second pass, 8), so the 13 comment lines are reworded, and every later living
sentence naming a check of that suite avoids the `packages/<check>` shape, which `records` tells at
once if it does not. **The alternative name, `index/`, is measured clean**: `records` with a root
`index/` names only the two `examples/index/` lines any move rewrites (the critic, second pass, 8).
The historian's one condition against `packages/`, a planned workspace feature that would want the
name, is answered by panel 235's R2 (Go's modules *"without workspaces"*) and its line 724, which
keeps *"a workspace or a configuration beside the catalogue"* refused. The author chooses; the sitting
adopts the author's word.

**R12. The other readings of the third instruction, priced and not adopted.** A catalogue with no
`index` line defaulting to a project-root `packages/`: it changes panel 235's R5 default (the Heroes
index) into a path no catalogue wrote, a surface change of `fetch` and a sitting's, and a stranger's
program has no `packages/` at its root. `heroes.packages/` renamed `packages/` (the critic's third
reading): R7's reason survives by the version folder's `@`, but it renames 101 tracked files in 23
folders, and the author said *index*. **What would decide**: the author naming which folder he meant.

**R13. A question for the author, not a ruling: should the next move of the index rename nothing?**
A placed key without the `index` line (the compiler-engineer's 6c) is sound: the `sum` alone fixes
the bytes a build reads, and a mistyped `index` line would go unseen only until the next `fetch` or
`add`. It is panel 235's R7 to restate, so a sitting's, not this one's.

## The verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**, no veto; approves when the rule, the route and 7.1's refusal land together | §4.15 (line 2252), §1.12, §1.1, §1.7 | rule +361 lines, route +92 (`layout`'s count); 638 files moved; the compiler's C 403,084 lines once; 9 emissions re-blessed; 8 fixtures; the heading refinement +40, not recommended | at the landing's gate, after batch 25's blessing, `emission` reads red on exactly the nine programs before their re-blessing, and on no other | its first condition (a tracked program changed by the route) restated after re-running the critic's finding 2: met outside the tree and by the rule alone, so it separates landing with or without 7.1; recommends the heading refinement only if a tracked heading over a run is found |
| ffi-pragmatist | **veto** on the rule without the route; with the route and the group words, now built, in the same push, **approve**, 7.1 to 7.3 filed `blocking` whatever that does to the tag | §1.12, §4.19 (design.md:606), §1.11; *the document does not cover the libraries' order* | 763 roots, 12 with two modules' groups, 0 `run` outputs moved, 1 `--emit-c` verdict moved (563, refused to accepted); 7 `examples/` programs identical; 0 release programs; the compiler links no library | under route and groups (`50edfd92476704cb`) on the CI's Linux x86-64 leg, `dep_*`, `pp_*`, `lp_*` link and print `41 2` in both orders, `sig_written` still dies by SIGSEGV and `only_top` still exits 2; on `macos-26` every `link-words.rsp` equals the base's | the veto lifts when rule and route travel together with one result per program in both orders; returns to object if x86-64 refuses the words or a nested group, or if 7.1 to 7.3 are classed below what their reproducers support |
| spec-warden | **approve**: the spec does not move, **conditional** on the group words landing with the route; objects to the route without them; seconds the ffi-pragmatist's veto of the rule alone | §1.6, §1.2, §4.15, Principle 0 | 0 tokens; priced and refused D1 +28/+30, D2 +7/+7, D2b +4/+4, D3 +5/+5, D4 +13/+14, L1a +17/+17, L1b and L1c +31/+31, N1 +17/+17 (vendored) | the landing leaves the spec without a hunk of this sitting; 0 correction turns caused by `use` order on the first metric-2 run; with the group words, `dep_*`, `pp_*`, `lp_*` link in both orders on GNU ld | objects if a correct program's `build` or `run` depends on its `use` order after the route; approves a link sentence only if the author rules library order the reader's duty |
| historian (advisory) | **approve** the rule and the route in one push | §4.15, §4.1 | none on the tree; pages read | the probe after the push refuses no variant where a comment written on a `use` line ends on another `use` line (rustfmt 3720's shape) | a language that sorts imports while the written order reaches codegen or the link line with no breakage over years |

## Disagreements

1. **The compiler-engineer's *the route breaks nothing* against the ffi-pragmatist's *the route
   flips `sig` and `dep`*.** Both literal claims reproduce (the critic rebuilt the route and ran
   both). The difference was scope: the compiler-engineer's claim and its withdrawal condition covered
   the tree and the releases, which hold none of the shapes; the ffi-pragmatist's covered the programs
   the language accepts. The compiler-engineer re-ran the critic's finding 2 and restated its
   condition to the second scope; the resolution takes it (R2, R3): a working program's behaviour
   changed at a landing is the compiler's to close, whether or not the tree holds one. **Settled.**
2. **The rule alone**: offered by the compiler-engineer as the author's conservative choice, vetoed
   by the ffi-pragmatist (`fmt` writes `sig_sorted` from `sig_written`, `cmp`), seconded by the
   spec-warden. A veto is a refusal, not a price (CLAUDE.md § 4), so it stands as vetoed.
3. **When R1 and R2 may land**: the ffi-pragmatist approves the route and the group words in one
   push, 7.1 to 7.3 filed `blocking`; the resolution lands them only with 7.1 and 7.2 repaired, since
   filed is not closed and R2 changes `dup_written`, `sig_written` and `rs_first` at the landing
   (the group words leave `sig_written` crashing in both orders under the route, the ffi-pragmatist's
   section 8). Recorded as robust against conservative (R3.4).
4. **One block or each run sorted alone**: the historian found gofmt sorting each run of
   consecutive imports and the Rust style guide's *"Groups of imports must not be merged or
   re-ordered"*; the resolution merges every `use` into one block, as the author's *group by kind*
   asks and goimports' accepted proposal 20818 would, since two layouts of one set of lines are two
   canonical texts (design.md:2252).
5. **`packages/` or `index/`**: the author's word against a name measured with no collision (R11).
6. **The heading comment** (rustfmt 6241's shape): the critic found R1 carrying a run's heading with
   the run's first line; the compiler-engineer built the refinement that raises it and recommends
   against it, since it loses one text per layout for 241 of the 361 permutation files; the
   resolution keeps the rule and records the cost and the spelling that avoids it (R1.3).

## Prior art

The precedents R1, R2 and R3 rest on, each `verified` by the historian (`238-reports/historian.md`)
and re-read by the coordinator on 2026-11-08: goimports puts a blank line between groups of one
block (`x/tools/internal/imports/imports.go`), and its accepted, open proposal 20818 asks it to
re-group and re-order the imports; `go/ast`'s `SortImports` sorts by import path, then name; Go's
specification initializes packages *"sorted by import path"* and Go 1.21's notes say so; swift-format's
`OrderedImports` moves a comment above an import with its line; PEP 8 puts a blank line between
groups; and the failures on record of a formatter sorting C's headers, Chromium's 2017 thread
(*"shellapi.h only compiles if windows.h was included first"*) and libc++'s commit (*"the
clang-format change broke the build by reordering the headers"*), which F20 reached on its own. The
decision issue's `## Prior art` carries them, each in the page's own words. **rustfmt 6241 is not
prior art the resolution rests on**: R1 does on purpose what that page reports as a bug, a recorded
cost (R1.3), so the historian's candidate line is not used (the critic, second pass, 9).

## Predictions to score

- **compiler-engineer**: at the landing commit's gate, after batch 25 has blessed the runtime's key,
  `emission` with the rule and the route reads red on exactly the nine programs of its item 3b and on
  no other. Scored at the landing's gate, not before the blessing.
- **ffi-pragmatist** (changed to the built compiler): under route and groups (`50edfd92476704cb`) on
  the CI's Linux x86-64 leg, `build/238/link/`'s `dep_*`, `pp_*` and `lp_*` link and print `41 2` in
  both written orders, as on arm64; `sig_written` still dies by SIGSEGV and `only_top` still exits 2;
  on `macos-26` every `link-words.rsp` equals the base's. Scored by the landing lane's run of those
  programs on the CI's legs at M-package-manager's push; an x86-64 GNU ld that refuses a grouped
  `dep_sorted`, or a `sig_written` that does not crash there, falsifies it.
- **spec-warden**: (1) the landing leaves `spec/heroes-spec.md` without a hunk of this sitting; (2) the
  first metric-2 run of the thesis instrument after the landing shows 0 correction turns caused by
  the order of `use` lines, in either arm; (3) with the group words, `dep_*`, `pp_*` and `lp_*` link
  in both orders on GNU ld.
- **historian**: the formatter's probe after the landing push refuses no variant in which a comment
  written on a `use` line ends on another `use` line. Scored by that run.

## What the conservative resolution would have been, so the author can choose it

- **The rule alone (R1 without R2)**: **vetoed** by the ffi-pragmatist on design.md §1.12, so not an
  option; recorded because the compiler-engineer offered it.
- **R1 and R2 before 7.1 and 7.2 are repaired** (R3.4): the ffi-pragmatist's own condition, the
  group words in the same push and the two defects filed `blocking`.
- **7.2 closed by a true refusal** instead of the order search (R3.3).
- **The group words judged by the CI after the push** (R3.5), a red Linux leg reverted.
- **No seed refresh at the landing** (R9), the rebuild of `heroes` named in every tree.
- **Each run of `use` lines sorted on its own**, nothing merged (gofmt's and Rust's style guide's
  reading): two texts for one set of lines, against *exactly one correct way*.
- **The index left at `examples/index/`**: nothing renamed, against the author's third instruction.

## Author's verdict

Pending.

**Ratified, 2026-11-08, about 22:39**, by the author's reading of the sitting's results as the coordinator set
them out in the conversation (the premise measured false, R1 to R13, the four defects older than the
proposal, the batch that lands R1 and R2 with the repairs of 2363 and 2364), given in Italian and meant
as: *OK, I ratify*. R1 to R12 stand as adopted. R13's question is, by its own text, panel 235's R7
to restate, so a later sitting's: until one rules, the next move of the index renames as R10 does.

## Process notes

1. **The critic's first pass changed the seating before any seat started.** The briefs said *the
   change touches no C boundary* and seated neither the ffi-pragmatist nor the spec-warden; the
   critic ran it and found the one-file emission's headers and the link words following the written
   order, and a program whose `--emit-c` exits 0 as written and 1 sorted (finding 21). That negative
   sentence was written by a coordinator with a shell who did not run it, the shape the `/panel`
   skill's *a brief's negative sentences are run* names; the ffi-pragmatist seated on it produced the
   sitting's largest finding, a formatter able to turn a running program into a memory fault. Nine
   of the 29 findings changed a brief; the repairs are `27829fbba`.
2. **The critic's second pass changed the resolution ten times**, and three seats ran again on it
   before the synthesis: the compiler-engineer built the heading refinement (not taken) and re-ran
   finding 2 (its condition restated), the ffi-pragmatist built the group words (taken), and the
   spec-warden read the link table it had not seen (its first report was written while the
   ffi-pragmatist's had 15 lines) and made its approval conditional on the words.
3. **The seats were not launched together.** The Agent tool returned only when the compiler-engineer
   finished (from about 17:00 to 19:59), so the other three started at 19:59 instead of beside it;
   the four should have gone in one message. At about 20:20 the account's five-hour limit stopped the
   ffi-pragmatist and the historian mid-work; both were resumed at 21:41 from their own transcripts
   and report files, not from another seat's findings.
4. **The machine was loaded** (`uptime` 43 to 59 at the compiler-engineer's start; its stage 2 read
   `6751.70 real 179.34 user`), so every cost in the reports is instructions retired or a count.
5. **The `emission` suite was red at the briefs' commit** on every program (771 failed), the
   runtime having moved at `82f9d0a5a` (batch 25, defect 2350) after the last blessing at
   `a789691f1`; not this sitting's finding, and no seat blessed anything. Batch 25's close owes the
   blessing, and R9's gate waits for it.
6. **No llm-ergonomist and no paid run**, by the sitting's brief. What it gives up: a measurement of
   what a model writes (the reader-facing half). The spec-warden's reading makes it unneeded for the
   specification (no verb refuses the order; `fmt` writes it). A run that would measure it, if the
   author wants one: N `claude -p` sessions given the specification and a task naming three modules,
   the share writing the canonical order, at the `/panel` skill's 3 USD cap per session; not
   recommended, since nothing a reader writes is refused for the order.
7. **The worktrees stood under `.claude/worktrees/`**, as at panels 235 to 237 (CLAUDE.md forbids a
   file outside the repository's root). The ffi-pragmatist reports that its first container runs let
   clang write temporary objects to the container's own `/tmp`, removed with the container, never the
   host's; later runs kept them under the mount. The compiler-engineer reports one command writing a
   163-byte `fmt` output to the host's `/tmp/x`, outside the repository, removed at once (its section
   8). The critic ran `--version` of three seats' `heroes-next` in place, read only.
8. **The sitting's own commits touch `docs/panel/**`, a path of the site's deploy**, so the push that
   carries them publishes the site; that push is the author's to ask for.
9. **Two of the coordinator's brief facts were corrected by the critic and stand as written in the
   first commit**: F4 (*`fmt` formats only a file with no diagnostic*: the parser's alone) and F9 (the
   fix `--apply` writes is `use_line.hero:273`'s, not `missing_use`'s guess); F2's quotation put a
   `;` where the file has a dash. And the synthesis's first draft wrote 833 files with two or more
   `use` lines, the compiler-engineer's count; the critic and the spec-warden count 834.
10. **Evidence**: `238-reports/evidence.tar.xz` and `evidence.md` beside it.
