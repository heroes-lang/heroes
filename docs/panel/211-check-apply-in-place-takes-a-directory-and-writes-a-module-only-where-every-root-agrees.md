# Panel 211: `check --apply --in-place` takes a directory and writes a module only where every root agrees

Convened 2026-10-11 at about 02:10 by the coordinator (session heroes-lang-98)
for defect 607 (`check --apply --in-place` writes the root module only), on
the author's choice at about 02:07 in the question widget (*Seduta 211 e
atterra*). **A lean sitting**: the compiler-engineer, the spec-warden and the
historian, one completeness critic pass after the reports, no blind seat and
no paid run. The tree frozen at **`1dd890751`** (batch 20's round), each seat
in its own copy under `.claude/worktrees/scratch-b15/211-<seat>/`. The
historian's report written by about 02:2x (no shell, no clock), the
spec-warden's by 02:24 by its `date`, the compiler-engineer's first pass by
about 02:50; the critic from about 02:44 to 02:54; the compiler-engineer,
resumed at 02:58 on the critic's unlisted route, built it by 03:21; this
synthesis from 03:23.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | first **object** W as briefed, approve it under four conditions; then, on the critic's route, **approve C**, recommended; W objected if C is taken; N the conservative record | W built (+307/-47); a certain fix is certain only for the root it was computed from (`probe/p2`: writing `lib/m.hero` for `lib/b.hero` breaks `a.hero`); C built (`routeC.diff`, +456/-48 over 11 files, 377 code lines): every gate green (the compiler's own tests 1,610, `surface` 408/0 with every fixture unchanged by `shasum`, `fixes` 978/0, `layout` 6/0), a file operand answered byte for byte as ratified; over the whole tree 773 files rewritten, 0 disputes in 447 shared modules, 581 of the writes under `docs/` and 152 in golden check cases |
| spec-warden | **object** W (`--apply` would print one set of fixes and `--in-place` write another, breaking panel 193's ratified R4); **approve** N on four conditions; L objected (nothing to guard); the spec moves 0 tokens on every route | `heroes measure`: 7,636 / 7,502 / 10,024 real; design.md §4.17 states twice that `--apply` never writes another module's fix (`:2235`, `:2246`), panel 193 ratified 2026-10-06; 2,527 of 2,528 roots reached by no other root |
| historian (advisory) | **approve** W bounded by L | `cargo fix` and `rustfmt` write the closure a root reaches; their failures were writes past a boundary (cargo#9857, rust#88514), each repaired by a write boundary; Go writes the packages a pattern names |
| critic (one pass) | no verdict | the built W changes plain `check --json` under schema 2 (`elsewhere` becomes `written`, every JSON consumer would write modules); p2 is panel 099's ratified leaf-test hazard (`tests/golden/surface-fixtures/nested/decoy/`, `suite_surface.hero:674`), uncited; L has nothing to apply to (`use` cannot climb out, 0 symlinks tracked); an unlisted route, **C, consensus**: a directory operand, a module's fix written only where every root reading it computes the same text; neither W nor N delivers 607's title |

## What the sitting measured

- **A certain fix is a fact about the program it was computed from**, not
  about a file: one module read by two roots can mean two things (p2, panel
  099's hazard), and writing it for one breaks the other. Today's compiler
  does the same when the leaf is named directly.
- **C writes nothing a root disagrees on**: p2 under its common directory is
  refused and named; a broken root freezes the modules it reads; a file
  operand keeps the ratified answer (`not_applied`, `first_round:
  elsewhere`, no `modules` key) byte for byte.
- **C is sound only within the directory named**: a root above it is never
  asked (`p2` with operand `d/lib` writes, and `a.hero` breaks). The operand
  is the author's statement of which programs count.
- **The whole repository is not a migration's directory**: over it C rewrote
  581 files under `docs/` (panel probes, records not rewritten) and 152
  golden check cases (wrong on purpose). Teaching the compiler this project's
  layout would be a premise about the world inside a general tool
  (`.claude/rules/module-shape.md`); the author names the trees a migration
  means, and the help and design.md say so.

## Disagreements, stated plainly

- **W, N or C.** The spec-warden's objection to W (print one set, write
  another) and the critic's (schema 2's meaning moves) both fall on W and on
  neither N nor C; N leaves p2 to whoever opts in; C answers the prior
  question the critic named (*is a certain fix a fact about a file or a
  program?*) with the program, refuses p2, keeps the ratified file answer, and
  alone delivers 607's title. The resolution takes C.
- **L.** The historian's boundary rests on a shape the language refuses; C's
  refusal of a symlink and of a file outside the operand is what remains of it.

## The resolution, provisional (author reading pending)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, C lands**: `heroes check <directory> --apply --in-place` checks every
   root under the directory (a `.hero` file declaring `function main`, hidden
   directories and `build/` skipped) and writes a file's certain fixes only
   where every root that reads it computes the same text, rounds repeated in
   memory until nothing new is agreed, every write planned and checked before
   the first byte, each file whole or not at all; a symlink, a hard link, an
   unwritable file or a file outside the operand refused before any write; a
   disputed module not written and its roots named. The compiler-engineer's
   `routeC.diff`, with its cases.
2. **R2, a file operand keeps panel 193's ratified answer byte for byte**,
   schema 2 unchanged in meaning; a directory with anything but `--apply
   --in-place` keeps defect 295's refusal.
3. **R3, the words**: design.md §4.17's two *never writes* sentences say a
   file operand never writes another module and a directory operand writes a
   module where every root under it agrees; the stale *provisional* beside
   panel 193 (`design.md:2229`, `cli/apply_answer.hero:2`) corrected, the
   sitting ratified 2026-10-06; `cli/table.hero`'s help line and
   `site/src/llms.txt:63-66` say what a directory writes and that the author
   names the trees (a directory holding records or wrong-on-purpose cases is
   not one); no spec sentence (0 tokens, the spec names no `heroes` verb).
4. **R4, defect 607 closes with R1 to R3.**
5. **R5, refused**: W (it changes schema 2's meaning and writes p2); N (an
   opt-in to p2, no title); L (nothing to guard); S (leaves the next
   checker-stage migration to a script, CLAUDE.md § 10); a compiler that knows
   this project's `docs/` and `tests/golden/` (a premise about the world).

**The conservative alternative, the author's to choose instead**: N, a new
flag beside `--in-place` writing what the root reaches, opt-in, with the
spec-warden's four conditions; or S, the root alone with the message naming
the modules.

## Process notes

- The historian has no shell and could not read the clock or grep the
  records.
- The spec-warden first wrote four helper files into the session scratchpad
  outside the repository and moved them into its directory; its report says
  so.
- The compiler-engineer restored a tracked fixture its W run had rewritten
  through `suite_surface.hero:684`, in its own copy; C leaves every fixture
  unchanged.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at C's landing batch: `check selfhost/main.hero` within 0.5% instructions; `fixes` and `surface` the base's counts but C's own cases; `check <tree> --apply --in-place` over `selfhost`, `tests/harness` and `examples` reports 0 disputes | the landing |
| spec-warden | the spec reads 7,636 / 7,502 with its real pin 10,024 unchanged at the landing | the landing |
| historian | a route writing what a root reaches without a boundary brings a defect within two batches about a write outside the root's own tree | two batches |

## Author's verdict

(pending)
