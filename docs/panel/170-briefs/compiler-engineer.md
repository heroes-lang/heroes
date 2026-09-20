# Panel 170 — brief for the compiler-engineer

Read `docs/panel/170-briefs/00-shared.md` first.

## Your axis

design.md §1.1, §1.7, Part 5: what the mark costs in the live compiler, and
whether it can be built at all.

## Where the machinery is, read rather than remembered

- `selfhost/parse/members.hero` — where `counted_by`, `owned`, `consumes`,
  `acquires` and `borrows` are parsed. **Re-measure it**: panel 167 put it at 298
  of a 300 ceiling and vetoed a route on that, and panel 169's engineer measured
  a DIFFERENT set of files at zero headroom a day later. Neither number may be
  carried.
- `selfhost/parse/group.hero` — where a mark on the group HEAD line would land
  instead, which panel 169 priced at 235 of 300.
- `selfhost/check/marks.hero` (115 lines) — `marks_are_read`, `refuse_unread`,
  `unread_mark`: what refuses a mark on a type that reaches no handle.
- `selfhost/check/acquiring.hero` (278) — the pass that makes `acquires` and
  `consumes` work.
- `selfhost/check/lending.hero`, `lend_types.hero`, `lend_decls.hero` — where a
  refusal of a lend at a marked parameter would live.
- `selfhost/ast.hero` and `selfhost/print/fmt.hero` — panel 169 measured them at
  524 of 525 and 1174 of 1175. **Re-measure.**
- `runtime/parts/alloc.c:368-440` — the handle live set, and
  `hero_handle_report_stray`, which as of today raises at the moment of detection
  rather than at exit.

## The questions

1. **Price the mark in each of its candidate spellings** — on the parameter, on
   the group head, as a mirror of `owned`. A new contextual word touches the
   lexer, the parser, the AST, the formatter and every exhaustive match over
   them; say which, with counts, and price a split where a ceiling is in the way
   rather than refusing the route on it.
2. **What refuses the lend?** Build it far enough to price it, in your copy, and
   say at which STAGE it fires and why that stage. Then run the golden trees and
   say how many programs it refuses.
3. **Defect 072 — does one mechanism serve both?** A handle needs to carry which
   producer made it. Say whether that is the same fact as retention or a second
   one, and price the cheapest thing that closes 072 alone.
4. **Which layer?** Panel 169 moved any structural rule to the IR because the
   AST's write set kept growing. A refusal about a DECLARATION may not have that
   problem. Say whether it does.
5. **Register a falsifiable prediction** with an instrument that exists today.

Report to `docs/panel/170-reports/compiler-engineer.md`. Veto on soundness.
