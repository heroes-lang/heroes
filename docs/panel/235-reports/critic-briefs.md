# Panel 235, the completeness critic's first pass: the briefs

2026-11-07. Briefs read at `95a995bf7` (pm-opening); commands run in
`.claude/worktrees/235-critic/`, detached at `55a687a38`, compiler from the
seed (`heroes 0.6.0`, `sources 1a1e340778db789a`). The grader was exercised
through a copy of `blind235.py` (sha256 `c40cf0db…908e`) whose one changed
line points `LANE` at this worktree, same commit: its `dry` printed *72
answers graded as expected, 0 not; 0 problems*, and `dry-run.txt` byte for
byte, paths aside (`diff`). Twenty crafted answers went through its own
`grade_one` (`235-critic-scratch/probe_grader.py`, output
`probe-grader.txt`). No paid session, no API call, no verdict.

## The question the briefs disagree on

1. **The spelling is not delegated, by the page.** Positions 3 and 7 rule
   `use <json>/parse` and *the bracket is the whole difference between mine
   and fetched*; the page's questions ask `use <name>/…` *beside* `use
   name/…`; the author's word after the survey delegates Go-or-Cargo and the
   format only. `00-shared.md`'s Frame 1 drops the brackets and says *a `use`
   with no mark stays a module of the program's own tree*, then Q2 and the
   README (*the sitting must choose (U)*) open the spelling. U-bare
   contradicts Frame 1 (*a `use` whose first name is no entry's reaches the
   program's own module*: an unmarked `use` can be fetched). U-bare and U-pkg
   are 60 of 210 first turns. Say what a U result can do: rule, or go to the
   author as a measured case against a position.

## The grader (`probe-grader.txt`)

2. **A valid TOML catalogue crashes it.** `[dependencies]` then
   `roman.version = "2.1.0"` raises `ValueError` (`toml_locate` returns two
   values at line 191, `parse_toml` unpacks three at 259); `grade` has no
   guard, so every later session stays ungraded. Four arms are TOML.
3. **Right answers failed.** A file-name subheading under `## catalogue`
   (`### heroes.toml`) ends the part (line 1122): `catalogue_missing`, whose
   text, *there is no heroes.toml beside main.hero*, then misleads the repair
   turn; `**heroes.toml**` passes. F-hero refuses `package roman  # from the
   index` (line 718, three errors), though the arm claims Heroes' own syntax
   and spec § 1 allows a comment to end a line. F-hero without headings: the
   fallback takes the first `hero` fence as the program (line 1137), and the
   arm's own right answer fences its catalogue `hero`: `catalogue_missing`,
   where F-toml's same shape passes.
4. **Rules no section states**: `[package]` (TOML), `name: app` (YAML),
   `module app` (`heroes.mod`) are refused as *nothing else*; each is the
   habit of the ecosystem whose words the arm borrows.
5. **The repair messages lean.** For one slip, the authors' name kept in the
   `use`, U-angle and U-pkg get `unknown_entry` with a per-line `fix (guess):
   write use <csv_ada>/fields`; U-bare gets the compiler's `unknown_module`,
   one trailing note, no fix (`dry-run.txt`, T2 wrong-entry-not-named): the
   `holders` lookup (line 965) is skipped on the bare path. Conversely `use
   <roman/numeral>`, C's `#include <sys/types.h>` habit (plausible,
   unmeasured), falls to today's compiler, *found `<` — `use geom`*, telling
   an angle arm the bracket is wrong; five of seven arms are angle.
6. Second turns never run (a hard stop during repairs) score as failed at
   pass@2 (line 1550), not as absent.

## The arms

7. **Lengths parallel**: `wc -w` 209–222 and vendored 413–439 / 430–456,
   re-measured. The F sections differ only in the format's paragraph (`git
   diff --no-index --word-diff`).
8. **U facts are not**: spec lines 29–30, *Every `use` starts at the
   directory of the file you compile*, stand in every arm and every U arm
   contradicts them; only U-angle says *`Use` becomes* and gives a production.
9. **The words are the coordinator's, not the question's.** Q4 asks *a line
   format*; the arm is `heroes.mod` and `require`, Go's file and verb, and its
   grader refuses Go's `require (` and `v2.1.0`. F-hero's `package` is also
   spec § 13's `package "raylib"` (line 462). F results will be read as
   Go-or-Cargo evidence they cannot carry.
10. **No JSON arm**, though Q4 and the page list JSON: about 0.77 USD of first
    turns at panel 234's means (its ledger re-read: 139 Haiku at 0.0044, 162
    Sonnet at 0.0468, max 0.0617).
11. **YAML's hazard cannot fire**: every version has three numbers, every tag
    opens `v`. A tag such as `1.10`, the same in every arm, would measure it.
12. **Stated in every arm, ruled by nobody**: an index entry's name is its
    index name, so an index package cannot be aliased (against the author's
    alias rule); the sum is *the fingerprint its publisher gives*.

## The cost

13. **The hard stop bounds reported cost, not spend.** `reserve` (line 1369)
    trusts every session to stay within 0.20; whether `--max-budget-usd` can
    stop inside one call is unrun. Panel 234's `modelUsage` reads
    `maxOutputTokens 128000`, and a least-squares fit of its 162 Sonnet costs
    gives about 9.92 USD per million output tokens (an inference): one runaway
    call near 1.27 USD, six running near 7.6 past 22, so past 25. The observed
    maxima (0.0617; 2,612 output tokens) make it unlikely, not impossible.
    Whether the warden's `count_tokens` calls bill is unchecked; the cut to
    four or three samples has no executor.
14. **Nothing pins the grader.** `blind235.py` is in no commit and the
    materials are read from `235-base`'s working tree, unchecked; the repairs
    will change both. No decision rule is registered: what difference, at 15
    or 30 a cell, chooses U or F, and what leaves the author's form standing.

## Framing facts, checked

15. Hold: the `Use` production (spec 40); `modules.hero:107`; the three
    refusals, and `import` refused `reserved_word`; `process.hero:109`;
    `refresh.hero:4` (a comment; a grep of `selfhost/` for `"curl"`, `"git"`,
    `http` finds only it); `sha256.hero` 292 lines, `of` at 119, `read_file`
    failing `not_text` (spec 337); `table.hero:238`; `claims.ts:263`; the use
    counts 5,482/635/130/41; 69 programs at `v0.6.0`; `vendor` in `RECORD`,
    prefix-matched from the root (line 1833); 14 example programs carrying
    `core/`; 8,019 and 10,514; panel 234 ratified 2026-11-06; `claude` 2.1.311
    lists every flag; panel 234's 301 sessions all said `context: none`;
    Amphetamine holds the machine (`pmset -g assertions`).
16. Do not hold as written: `55a687a38`'s parents are `9d894fe68` and
    `95a995bf7`, not pm-opening and core-b1; `use <json>/parse` is refused at
    1:1, on `use`; `cli-surface.md`'s `design.md:772` is stale, `heroes add`
    standing at 898 (§ 3.5), and the probe quotes it unflagged.

## Questions no brief asks

17. **Alias scope**: whose catalogue resolves a dependency's own `use <x>/…`,
    and what happens when two catalogues give one alias to two packages, or
    two aliases to one, under *one version per program*.
18. **Panel 056's second condition by its purpose**: design.md §3.5, *deleting
    the file never changes which symbols are linked*. For a catalogue:
    deleting or mistyping an entry never changes which module compiles, only
    whether it builds. Angle and pkg meet it; U-bare does not where a folder
    of the entry's name exists, or where fetched code sits in the tree, as the
    grader places it at `json/`.
19. **The locality veto**: `use <json>/parse` means what another file says;
    the llm-ergonomist's veto is on non-local constructs, and the blind seat is
    an experiment without a verdict row. Who holds it?
20. **Unassigned**: the page's simplicity measures (concepts counted, the
    catalogue's grammar in productions, files a program needs); Q1, Q9, Q11
    (the 14 copies of `core/`), Q12 (`internal/`, already in core-b1, e.g.
    `examples/ini/core/strings/internal/scan.hero`) and Q15.
21. **The network**: may `build`, `check` or `run` ever fetch?
    `refresh.hero:4–10` admits it *only where the answer is pre-recorded*.
22. **Q0 under the author's criterion**: the test never touches the base; say
    how *works best for language models* is judged there.

## The repairs, most important first

1. Settle finding 1 in `00-shared.md` and the README before predictions: the
   spelling's status, Frame 1 reworded, U-bare kept as a measured case or
   dropped.
2. Repair the grader (2, 3, 5): dotted keys, subheadings, F-hero comments and
   fallback, U-bare's `holders` fix, an arm-spelled message for `use <a/b>`;
   each a new case in `dry/`.
3. Bound the spend (13): reserve one worst call per running session, or cap
   output tokens, and state the warden's margin.
4. Pin `blind235.py`'s hash and a decision rule in the briefs' commit (14);
   re-run `dry`, re-merge `235-base`, detach the seats at the new commit.
5. Give 17 and 18 to named seats, and 19 to a seat or the author.
6. State or drop the unstated rules (4); neutralise the line format's words or
   say what F cannot rule (9); add JSON (10) and a YAML-hazard tag (11).
7. Correct 16; assign 12, 20, 21 and 22.
