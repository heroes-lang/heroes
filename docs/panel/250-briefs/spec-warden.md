# Panel 250, spec-warden

Read `00-shared.md` first, whole: the base (**rebased to `48f53b343`**, `core` 0.5.0 alone), the measured
fact that a `use` written after a fetch is refused under a placement of the reached set, R2's ruler as
panel 242 measured it, the rulings quoted from the record (panel 244's R1 and panel 246's R2 among them),
the routes (W, M, G, H; F1 to F4; R2's ruler moved; `core` split; trees not committed; the nine nobody
listed), the rules (**the two compilers keep the names `heroes` and `heroes-next`; every other binary you
build is named `sw-...`**; a suite's output goes into a file, never through `head` or `tail`) and the
platforms. You judge what the specification, `.claude/rules/cli-surface.md`, design.md and the documents
a reader opens must and must not say under each route, and Principle 0's burden for any sentence, with a
veto on budget breach and on an unmet burden. Your folder is
`.claude/worktrees/p250-scratch/250-spec-warden/`, your tree `tree/` in it, a `git archive` of
**`48f53b343`**, your compiler built there from the seed in two stages (`sources 5bac791363384945`). Your
report is `docs/panel/250-reports/spec-warden.md` in `.claude/worktrees/p250`, in your definition's output
structure (`.claude/agents/spec-warden.md`), written as you go. Reach design.md by grep before you cite
it: `grep -n '^### 1.0' docs/design.md` 112 (Principle zero), `'^### 1.2'` 190 (the cost formula),
`'^### 1.6'` 253 (the spec budget), `'^### 1.7'` 425, `'^### 1.11'` 482 (no standard library; the
packages' paragraph and R2's ruler at 562-581), `'^### 1.12'` 620, `'^### 3.5'` 900 (the catalogue's
paragraph at 912-925); `grep -n 'payment' docs/design.md`: 282, 301, 324, 328, 330 and 4053 (design.md is
unchanged between the first base and this one, `git diff --stat 722ae76bf 48f53b343 -- docs/design.md`
printing nothing).

## The count before, measured at the base

`./heroes-next measure spec/heroes-spec.md`, run by the coordinator in
`.claude/worktrees/p250-scratch/briefs/base48/tree`, a `git archive` of `48f53b343` (its spec unchanged
from the first base: `shasum -a 256` begins `7dc8bdaa3692af17`; 492 lines and 29,216 bytes, `wc -l -c`),
exit 0 (`briefs/base48/measure-spec.txt`):

| row | count |
|---|---|
| claude-legacy | 8483 |
| cl100k_base | 8634 |
| maximum (vendored) | 8634, *"a lower bound, not the reader's tokeniser"* |
| spread | 151 between the two vendored tables |
| real | **11303**, *"claude-opus-5, 2026-11-14 — the binding number"*, the instrument's pin, no `STALE` |
| headroom | 5081 against 16384; *"the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 11363"* |

The ceiling is 16384 (`tests/harness/suite_spec.hero:162-163`), `DELTA_GATE` 50 (`:217-218`).
`./heroes-next measure CLAUDE.md` in the same tree, exit 0: claude-legacy 6236, cl100k_base 6347, real
8359 (*"claude-opus-5, 2026-10-29"*), against `CONTRACT_CEILING` 12288 (`suite_spec.hero:296-297`)
(`briefs/base48/measure-claude.txt`). Run both again in your own tree before you use them; a different
figure is a finding.

**No real delta is measured in this sitting.** `heroes measure --refresh` asks
`https://api.anthropic.com/v1/messages/count_tokens` with the trunk's key, an API call, which this
project's rules count among paid runs (`.claude/skills/panel/SKILL.md:288-295`), and none is named
here. Every verdict of yours on a draft is `provisional`, and every vendored delta **a lower bound**, said
in those words; a real count that would decide your verdict is owed to the coordinator, who asks the
author.

## What the documents say today, and where

- **spec § 1** (`spec/heroes-spec.md:22-55`): *"names with its source, exact version and `sum`, or by
  a local `path` alone, which nothing verifies"* (33-34); *"`heroes add json` writes that entry"* (35);
  *"a program holds one version of each package, every one listed in its catalogue"* (40-41); the
  example, one `sum` per entry. `grep -c '<core>' spec/heroes-spec.md`: 0; `grep -n -w core` and `grep -n
  'heroes.packages'`: nothing. **That silence is a ruling**: panel 244's R1 (`244-*.md:80-100`, ratified
  2026-11-11 at its line 216) admitted three package sentences and no other, *"Out: `heroes fetch` and
  `heroes.packages/`, the stand-in and outside-reading clauses"* (95-96), by the bar *"a sentence enters
  where a reader holding only the specification would otherwise write a wrong program or be unable to
  predict a refusal, measured, never because it is true"* (22-25).
- **`.claude/rules/cli-surface.md`** (87 lines): § Four input classes (52-81), the falsifier (68-74),
  the fifth class refused (76-78), the shape of a capability (22-25). CLAUDE.md § 10 cites it (its lines
  239-240, *"the fourth input class, the catalogue, and the refusal of a fifth"*).
- **design.md** §1.11's packages paragraph and R2's ruler (562-581, *"placed files counted"* at 578) and
  §3.5's catalogue paragraph (912-925, *"deleting or mistyping an entry never changes which module
  compiles"*). No suite holds design.md to a token ceiling, as far as the coordinator searched: `grep -n
  -E '^constant [A-Z_]+: i64' tests/harness/suite_spec.hero` names `CEILING` (the spec's) and
  `CONTRACT_CEILING` (CLAUDE.md's), and `grep -rn -l -i 'design\.md.*ceiling\|DESIGN_CEILING'
  tests/harness/*.hero` found one comment, `suite_layout.hero:536`, on §1.1, no constant (panel 242's
  spec-warden wrote the same of its wordings, *"on design.md, which no ceiling judges"*,
  `242-*.md:105-106`, carried).
- **`examples/packages/README.md`** (369 lines at the base): the opening (16-24, *"places the version's
  files under the program's own `heroes.packages/`, the index's files byte for byte"*), § 1 rule 4
  (44-53), § 9, § 10 (225-353).
- **The verbs' help**, which a reader reads (cli-surface.md:49-50, *"One argv table parses the arguments
  and prints the help"*): `selfhost/cli/table.hero:194-195`, `add` *"writes every package of the build
  with its sum, and places each"*, `fetch` *"place every package heroes.toml names under
  heroes.packages/, each checked against its sum"*; no golden pins them (`git grep -l 'place every
  package heroes.toml names'`: `selfhost/cli/table.hero` and `seed/heroes.c` alone).
- **The two messages a reader of a pruned tree meets today** (00-shared.md § Whole placement today):
  `package_unread`'s and `sum_mismatch`'s advice, *"move it out of `heroes.packages/` to keep the change,
  or remove it, and `heroes fetch` places it again"*.
- **The site** (outward-facing, `site/CLAUDE.md` governs its copy), its packages chapter re-cut from
  0.5.0 by the round (`git diff --stat 722ae76bf 48f53b343 -- site/src/html/docs/packages.html`: 73
  lines changed, and its Italian twin's too): `site/src/html/docs/packages.html` and `site/src/html/it/docs/packages.html` (in the base, line
  112, *"`heroes add` placed the package in a folder of the program called `heroes.packages/`"*, and 183,
  *"The compiler checks the copy in `heroes.packages/` against the sum at every build"*),
  `site/src/html/examples/_fetched.html` and its twin, `site/src/lib/examples-model.ts`.
- **The records**: panel 235's R2 (with its proposal at `235-*.md:60`), R6, R7, R12, R15, R19 and R20 as
  amended; panel 234's R2 and R13; panel 238's R10; panels 241 and 242; panel 244's R1; panel 246's R2
  and defect 2493; decision 2528 (00-shared.md quotes each with its lines).

## The candidates, to be priced by you

Drafts the coordinator wrote to be priced, **never a ruling**, spelled by this brief alone (`grep -c -F`
of each anchor in the base's spec printed 1). A seat that builds another spelling changes the draft, and
you price the built one. Rebuild each in your folder from these words. **Each faces panel 244's R1**,
whose *"Out"* names `heroes fetch` and `heroes.packages/` and whose bar asks a measured reader's wrong
program or unpredictable refusal: say for each whether that bar is met, by what measurement, and whether
admitting it re-opens 244's R1, which a sitting rules and these briefs do not.

| draft | route | where | the change |
|---|---|---|---|
| x | | | the base, no change |
| p0 | M or H, F1 | | no sentence: the spec says nothing of placement today, as 244's R1 ruled |
| p1 | M or H, F1 | line 35 | *"`heroes add json` writes that entry;"* becomes *"`heroes add json` writes that entry, and `heroes fetch` places the modules the program's `use` lines reach, each file held to the listing the `sum` names;"* (names `heroes fetch`, which 244's R1 ruled out) |
| p2 | F2 | line 33 | *"names with its source, exact version and `sum`"* becomes *"names with its source, exact version and a `sum` for each module it uses"*, and the example's entries gain what you would write for it (R5's subset refuses arrays, `235-*.md:589-592`, so say the shape) |

The refusal a route of placement adds (00-shared.md's measured edit: a `use <core>/strconv` written
after the fetch, `check` exit 1 `package_unread`, where today it builds) is the shape of 244's bar, *"be
unable to predict a refusal"*: whether a reader of p0 can predict it, and whether p1 is what would let
them, is the measurement your verdict on p1 owes or names as owed.

And for the routes outside the spec: **a falsifier for M**, drafted by you for
`.claude/rules/cli-surface.md:68-74` (which edits it must now name: a `use` line added or removed after
the fetch, a placed file removed or added, the listing edited or moved), and **a ruler for the
alternative**, drafted by you for design.md:575-581 (*the compiler counted by the modules it reaches*),
each priced by `heroes measure` on its file (vendored, a lower bound; no ceiling judges either, so the
price is information, not a gate).

## The questions

1. **Which sentence each route makes false**, in every document above, quoted with its line. The
   coordinator's reading, **a premise for you to confirm or refute, never to inherit**: W changes nothing;
   M, G and H change what README's opening and § 1 rule 4, the guide's chapter (both editions), design.md,
   the verbs' help and the two messages say of what is placed; F2 and F3 change what spec § 1 says the
   catalogue holds; F4 changes R7's *"checked ... at every build"* and the guide's line 183; the split
   changes R15, the 18 catalogues and every `use <core>/…` line, and no sentence of the spec names `core`
   (0, above), though whether one of its sentences reads otherwise under the split is yours; a ruler
   counted by modules changes design.md:578 and panel 242's R2.
2. **Reader-facing or not, route by route**: say for each route whether a program, its catalogue, a
   `use` line, a message a reader acts on, the verbs' help, or a sentence of the spec changes. **Where one
   does, say so in those words.** The measured edit above is one for M, G and H; the critic's reading
   adds F3 (the catalogue's `sum` moves with the `use` lines), p1 and trees not committed (its § K):
   confirm or refute each.
3. **Panel 235's R2**, in a sitting whose proposal said *"The semantics copy one widely used system whole,
   Go's modules or Cargo"* (`235-*.md:60`) and *"The sitting chooses ... where fetched code lives"*
   (61-62): does placing a subset depart from R2 as written (*"the program's catalogue lists every package
   in the build ... each with its fingerprint"*; *"at build it checks ... each placed tree's
   fingerprint"*), or only from R6's and R7's wording? The historian's report says what Go itself places;
   read it before you rule, or say your reading is ahead of it.
4. **Panel 234's R2**, *"it carries under its own root the packages it reaches"*: the examples held only
   the reached Go packages from `43aae88d8` to `31c34d962`, `packages/copies` holding each copy
   (00-shared.md § Before whole placement). Does M read as that sentence's intent, which 235's R15 amended
   for placement and kept for Go's tree? **Why did whole placement replace it**, and did any record weigh
   a subset (the critic's search printed nothing, carried): read panel 235's briefs, reports and
   `31c34d962`'s body. And R13 with panel 241's R3, whose withdrawn `selfhost/core/` form feature 2315's
   notes still name.
5. **The fifth input class** (`cli-surface.md:76-78`): is a listing placed inside the tree (F1), a
   record of which modules were placed, or the verb reading the program's `use` lines to decide what to
   place, a fifth class, or none? Rule by the paragraph's letter and by panel 056's three conditions it
   cites, and say which reading you take.
6. **R19** (`235-*.md:788-796`): the placement folder, the fingerprint's algorithm and the verbs'
   output are fixed as goldens *"from the first version to the second"*, and it ends *"The second version
   replaces the transport alone"*, where panel 242's R5 says *"replaces no transport"* (`242-*.md:169`):
   does M change R19's interface? Read which goldens a route moves by their folder, knowing that the
   `fetch` form prints placed folders' names alone (`tests/harness/suite_fetch.hero:452-456`), so no golden
   pins which files are placed; **where your reading and the compiler-engineer's run differ, the run
   stands**.
7. **Decision 2528 under the split**: does *"every program moves to the newest version of core and the
   older versions are removed"* become one rule per entry, and does the split need the author's word on
   that decision as well as a sitting?
8. **R2's words**: which **unit** design.md:575-581 and panel 242's R2 mean by *"its own tree, placed
   files counted, is smaller"* (panel 242 counted lines of Heroes; the author was given every file's
   lines; 00-shared.md § R2's ruler), and whether cards, a placed listing and the compiler's catalogue
   count; **what *"the compiler's tree"* is** (`selfhost/`, `selfhost/` and `runtime/`, or the repository
   whose `packages/` holds every version, which route 9 turns on); and **which *package*** *"every live
   copy of that package's contract"* means, the entry or the Go package reached.
9. **Panel 246's R2 and defect 2493**: under M a `fetch` after a `use` change writes into a placed tree;
   under F1 an author's removal of an unreached file is not an edit at all. Read both against *"never
   discarded in silence"* and *"never replaces an edited placed tree"*, and say what they require of the
   route.
10. **Principle 0 and the burden**: no route adds a form of the language; say which rank of CLAUDE.md
    § Precedence each route's case rests on (robustness, Principle 0, process), what each sentence you
    would admit is paid with (a named removal or a pre-registered falsifiable prediction with its
    instrument, design.md:324-330), and for each route you would refuse, the program or fact that would
    make the refusal wrong (CLAUDE.md § 12).
11. **What a reader meets**, built in your tree (every binary `sw-...`, bounded as 00-shared.md says):
    re-run the measured edit (a copy of `examples/ini` gaining `use <core>/strconv`, its tree whole and
    pruned), and say what message each route owes that reader, and whether today's advice (*"remove it,
    and `heroes fetch` places it again"*) would be true under M.
