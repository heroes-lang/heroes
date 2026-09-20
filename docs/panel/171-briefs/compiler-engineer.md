# Panel 171 — brief for the compiler-engineer

Read `docs/panel/171-briefs/00-shared.md` first. The direction is the author's;
you judge the word's cost, the rule's mechanism, and above all **the landing**.

## Your axis

design.md §1.1, §1.7, Part 5. And this sitting's own hazard: **the flip reaches
the compiler's own bindings**, nine extern functions at twelve call sites, so
the seed must parse the word before `selfhost/` can write it.

## Where the machinery is

- `selfhost/parse/members.hero` — where `counted_by`, `owned`, `consumes`,
  `acquires`, `borrows` are parsed. Panel 170's engineer measured it at 307 → 210
  after moving the marker block to a new `parse/marks.hero`; **re-measure**.
- `selfhost/ast.hero:653` and `selfhost/parse/members.hero:222` — the two
  `Param(...)` construction sites, both one-liners.
- `selfhost/check/lending.hero` — `lent_only_into_c` at `:194` is the sweep that
  would refuse a lend at an unmarked parameter; `argument_of` at `:118`.
- `selfhost/check/lend_extent.hero` — where panel 170's engineer put its
  `keeps` clause at 241 → 255, no split.
- `selfhost/check/marks.hero:64-84` — the sweep the word must NOT land inside.
- `selfhost/cli/process.hero:42-53` and `selfhost/emit/literal.hero:42` — the
  nine bindings the compiler itself must mark.

## The questions

1. **Build it far enough to price it, in your copy**: the word in its own slot,
   the refusal of a lend at an unmarked `cstr`/`ptr` parameter, at `check`. Lines
   per file in `suite_layout`'s own unit, ceilings re-measured.
2. **THE LANDING, and this is where to spend your time.** Write the two commits
   as a plan and then TEST the plan: with the word parsed but the rule not yet
   flipped, does `selfhost/` still compile and the fixpoint hold? Then, with the
   rule flipped and the nine bindings marked, does it? What happens if somebody
   lands them in one commit — say exactly where the seed breaks.
3. **The rule's edges.** An `@` out-parameter: C writes into the program's cell
   and could, in principle, keep the cell's address — is a lend even possible
   there, and does the rule reach it? A handle parameter: it has `consumes` and
   `acquires`; does the flip touch it, and should it? `nullptr` at an unmarked
   parameter: admitted, surely; say so with the run.
4. **Which layer.** Panel 169 moved any structural rule to the IR because the
   AST's write set kept growing. This rule is about a DECLARATION's mark and a
   CALL's argument kind, both AST facts that do not grow. Say whether it belongs
   at `check` on the AST, and why the open-set worry does not apply.
5. **Register a falsifiable prediction** with an instrument that exists today.

Copy named for your seat, `git log -1` at `b6e26fcc` or later. Never rebuild
from `selfhost/`. Report to `docs/panel/171-reports/compiler-engineer.md`. Veto
on soundness.
