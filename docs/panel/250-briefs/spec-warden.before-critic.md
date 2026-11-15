# Panel 250, spec-warden

Read `00-shared.md` first, whole: the routes (W, M, G, H; F1 to F4; R2's ruler moved; `core` split;
trees not committed), **why there is no blind seat and the escape that would call one**, the rulings
quoted from the base, the rules (**your binaries are named `sw-...`**) and the platforms. You judge what
the specification, `.claude/rules/cli-surface.md`, design.md and the documents a reader opens must and
must not say under each route, and Principle 0's burden for any sentence, with a veto on budget breach
and on an unmet burden. Your folder is `.claude/worktrees/p250-scratch/250-spec-warden/`, your tree
`tree/` in it, a `git archive` of **`722ae76bf`**, your compiler built there from the seed in two stages
(`sources 2ab7ca0a644692a1`). Your report is `docs/panel/250-reports/spec-warden.md` in
`.claude/worktrees/p250`, in your definition's output structure (`.claude/agents/spec-warden.md`), written
as you go. Reach design.md by grep before you cite it: `grep -n '^### 1.0' docs/design.md` 112 (Principle
zero), `'^### 1.2'` 190 (the cost formula), `'^### 1.6'` 253 (the spec budget), `'^### 1.7'` 425,
`'^### 1.11'` 482 (no standard library; the packages' paragraph and R2's ruler at 562-581), `'^### 1.12'`
620, `'^### 3.5'` 900 (the catalogue's paragraph at 912-925); `grep -n 'payment' docs/design.md`: 282,
301, 324, 328, 330 and 4053.

## The count before, measured at the base

`./heroes-next measure spec/heroes-spec.md`, run by the coordinator in
`.claude/worktrees/p250-scratch/briefs/base/tree` (its spec the worktree's byte for byte: `shasum -a 256`
of both begins `7dc8bdaa3692af17`; 492 lines and 29,216 bytes, `wc -l -c`), exit 0
(`briefs/base/measure-spec.txt`):

| row | count |
|---|---|
| claude-legacy | 8483 |
| cl100k_base | 8634 |
| maximum (vendored) | 8634, *"a lower bound, not the reader's tokeniser"* |
| spread | 151 between the two vendored tables |
| real | **11303**, *"claude-opus-5, 2026-11-14 — the binding number"*, the instrument's pin, no `STALE` |
| headroom | 5081 against 16384; *"the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 11363"* |

The ceiling is 16384 (`tests/harness/suite_spec.hero:162-163`), `DELTA_GATE` 50 (`:217-218`).
`./heroes-next measure CLAUDE.md` in the same tree: claude-legacy 6236, cl100k_base 6347, real 8359
(*"claude-opus-5, 2026-10-29"*), against `CONTRACT_CEILING` 12288 (`suite_spec.hero:296-297`). Run both
again in your own tree before you use them; a different figure is a finding.

**No real delta is measured in this sitting.** `heroes measure --refresh` asks
`https://api.anthropic.com/v1/messages/count_tokens` with the trunk's key, an API call, which this
project's rules count among paid runs (`.claude/skills/panel/SKILL.md:288-295`), and none is named
here. Every verdict of yours on a draft is `provisional`, and every vendored delta **a lower bound**, said
in those words; a real count that would decide your verdict is owed to the coordinator, who asks the
author.

## What the documents say today, and where

- **spec § 1** (`spec/heroes-spec.md:22-55`): *"names with its source, exact version and `sum`, or by
  a local `path` alone, which nothing verifies"* (33-34); *"`heroes add json` writes that entry"* (35);
  *"a program holds one version of each package, every one listed in its catalogue"*; the example, one
  `sum` per entry. `grep -c '<core>' spec/heroes-spec.md`: 0; `grep -n 'heroes.packages'
  spec/heroes-spec.md`: nothing. So the spec names neither `core` nor the placement folder.
- **`.claude/rules/cli-surface.md`** (87 lines): § Four input classes (52-81), the falsifier (68-74),
  the fifth class refused (76-78). CLAUDE.md § 10 cites it (its lines 239-240, *"the fourth input class,
  the catalogue, and the refusal of a fifth"*).
- **design.md** §1.11's packages paragraph and R2's ruler (562-581, *"placed files counted"* at 578) and
  §3.5's catalogue paragraph (912-925, *"deleting or mistyping an entry never changes which module
  compiles"*); no suite holds design.md to a token ceiling, as far as the coordinator searched: `grep -n -E
  '^constant [A-Z_]+: i64' tests/harness/suite_spec.hero` names `CEILING` (the spec's) and
  `CONTRACT_CEILING` (CLAUDE.md's), and `grep -rn -l -i 'design\.md.*ceiling\|DESIGN_CEILING'
  tests/harness/*.hero` found one comment, `suite_layout.hero:536`, on §1.1, no constant (panel 242's
  spec-warden wrote the same of its wordings, *"on design.md, which no ceiling judges"*,
  `242-*.md:105-106`, carried).
- **`examples/packages/README.md`** (367 lines): the opening (16-24, *"places the version's files under
  the program's own `heroes.packages/`, the index's files byte for byte"*), § 1 rule 4 (44-53), § 9,
  § 10 (225-350).
- **The site** (outward-facing, `site/CLAUDE.md` governs its copy): `site/src/html/docs/packages.html`
  and `site/src/html/it/docs/packages.html` (the guide's chapter; its line 109, *"`heroes add` placed
  the package in a folder of the program called `heroes.packages/`"*, and 180, *"The compiler checks the
  copy in `heroes.packages/` against the sum at every build"*), `site/src/html/examples/_fetched.html`
  and its twin, `site/src/lib/examples-model.ts`.
- **The records**: panel 235's R2, R6, R7, R12, R15, R19 and R20 as amended; panel 234's R2 and R13;
  panels 241 and 242; decision 2528 (00-shared.md quotes each with its lines).

## The candidates, to be priced by you

Drafts the coordinator wrote to be priced, **never a ruling**, spelled by this brief alone (`grep -c -F`
of each anchor in the base's spec printed 1). A seat that builds another spelling changes the draft, and
you price the built one. Rebuild each in your folder from these words:

| draft | route | where | the change |
|---|---|---|---|
| x | | | the base, no change |
| p0 | M or H, F1 | | no sentence: the spec says nothing of placement today |
| p1 | M or H, F1 | line 35 | *"`heroes add json` writes that entry;"* becomes *"`heroes add json` writes that entry, and `heroes fetch` places the modules the program's `use` lines reach, each file held to the listing the `sum` names;"* |
| p2 | F2 | line 33 | *"names with its source, exact version and `sum`"* becomes *"names with its source, exact version and a `sum` for each module it uses"*, and the example's entries gain what you would write for it (R5's subset refuses arrays, `235-*.md:589-592`, so say the shape) |

And for the routes outside the spec: **a falsifier for M**, drafted by you for
`.claude/rules/cli-surface.md:68-74` (which edits it must now name: a `use` line added or removed after
the fetch, a placed file removed or added, the listing edited), and **a ruler for the alternative**,
drafted by you for design.md:575-581 (*the compiler counted by the modules it reaches*), each priced by
`heroes measure` on its file (vendored, a lower bound; no ceiling judges either, so the price is
information, not a gate).

## The questions

1. **Which sentence each route makes false**, in every document above, quoted with its line. The
   coordinator's reading, **a premise for you to confirm or refute, never to inherit**: W changes nothing;
   M, G and H change what README's opening and § 1 rule 4, the guide's chapter (both editions) and
   design.md say of what is placed; F2 and F3 change what spec § 1 says the catalogue holds; F4 changes
   R7's *"checked ... at every build"* and the guide's line 180; the split changes R15, the 17 catalogues
   and every `use <core>/…` line (16 programs and the net's own), and no sentence of the spec names
   `core` (0, above), though whether one of its sentences reads otherwise under the split is yours; a
   ruler counted by modules changes design.md:578 and panel 242's R2.
2. **Reader-facing or not, route by route** (00-shared.md's claim that the question changes no line a
   program writes): say for each route whether a program, its catalogue, a `use` line, a message a reader
   acts on, or a sentence of the spec changes. **Where one does, say so in those words**: that is the
   trigger for the blind seat's budget.
3. **Panel 235's R2**, the sitting's *"Go's modules copied whole"*: does placing a subset depart from R2
   as written (*"the program's catalogue lists every package in the build ... each with its
   fingerprint"*; *"at build it checks ... each placed tree's fingerprint"*), or only from R6's and R7's
   wording? The historian's report says what Go itself places; read it before you rule, or say your
   reading is ahead of it.
4. **Panel 234's R2**, *"it carries under its own root the packages it reaches"*: whether M reads as
   that sentence's intent, which 235's R15 amended for placement and kept for Go's tree; and R13 with
   panel 241's R3.
5. **The fifth input class** (`cli-surface.md:76-78`): is a listing placed inside the tree (F1), a
   record of which modules were placed, or the verb reading the program's `use` lines to decide what to
   place, a fifth class, or none? Rule by the paragraph's letter and by panel 056's three conditions it
   cites, and say which reading you take.
6. **R19** (`235-*.md:788-796`): the placement folder, the fingerprint's algorithm and the verbs'
   output are fixed as goldens *"from the first version to the second"*; does M change R19's interface,
   and so the goldens 2315 must pass *"unchanged"* (task 2312, lane b37-pm, 00-shared.md § The
   interface)? Say which goldens a route moves by their folder, read, not run.
7. **Decision 2528 under the split**: does *"every program moves to the newest version of core and the
   older versions are removed"* become one rule per entry, and does the split need the author's word on
   that decision as well as a sitting?
8. **Principle 0 and the burden**: no route adds a form of the language; say which rank of CLAUDE.md
   § Precedence each route's case rests on (robustness, Principle 0, process), what each sentence you
   would admit is paid with (a named removal or a pre-registered falsifiable prediction with its
   instrument, design.md:324-330), and for each route you would refuse, the program or fact that would
   make the refusal wrong (CLAUDE.md § 12).
9. **What a reader meets today**, built in your tree (every binary `sw-...`, bounded as 00-shared.md
   says): a copy of `examples/ini` gaining `use <core>/net` with its tree whole (builds, at the base) and
   the same under a pruned tree (refused `package_unread` at the base, the message telling the reader to
   remove the tree and fetch again); say what message each route owes that reader, and whether today's
   advice would be true under M.
