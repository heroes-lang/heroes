# Panel 211, shared brief: `check --apply --in-place` over a tree (defect 607)

Convened 2026-10-11 at about 02:10 by the coordinator (session heroes-lang-98)
on the author's choice at about 02:07 (the question widget, *Seduta 211 e
atterra*): a lean sitting, the compiler-engineer, the spec-warden and the
historian, one completeness critic pass after the reports, no blind seat and
no paid run; the resolution adopted provisionally and lands tonight, the
author reading it in the morning.

## The question

Defect 607 (`improvement`),
`issues/2026-10/10/2026-10-10-2355-defect-607-check-apply-in-place-writes-the-root-module-only.md`:
`check --apply --in-place` writes the root module only, so a migration over a
tree (M-inferred-cell's rewrite of every `@` declaration) is one command per
root. Lane b20-tools measured beside it (its record under 607's item,
commit `4e3541717`): no command and no pair of commands writes a certain fix
that sits in a nested module with a `use` line, since checking the module
alone reports its `use` as an unknown module and writes nothing; the tree
holds 3,473 tracked `.hero` files, 2,504 of them roots. Its recommendation:
widen `--apply --in-place` to every module the root reaches, each file
written whole or not at all, every file written named, no new flag. It noted
that this changes what `selfhost/cli/table.hero` documents (*with --apply,
rewrite the file*, read at 02:09 at `:96`) and panel 193's provisional
`not_applied` field in the `--json` document.

## The tree

Frozen at **`1dd890751`**, batch 20's round (it carries lane b20-tools'
defect 621, the apply answer's message for a root with fixes elsewhere). Each
seat in its own copy, `.claude/worktrees/scratch-b15/211-<seat>/tree/`; build
there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed && ./heroes-seed build selfhost/main.hero -o heroes`.
Never another seat's directory, the trunk or a lane. No file outside
`/Users/joseph/Temp/heroes/heroes-lang`.

## Questions every seat answers from its ground

1. `.claude/rules/cli-surface.md`'s stopping rule and shapes: is widening
   what `--in-place` writes a change of how one question is answered (a
   flag's meaning, admissible) or a new capability; does `--in-place` naming
   files the command line did not name break the invocation contract
   (artifact on stdout, diagnostics on stderr, exit codes)?
2. The routes: (W) widen `--in-place` to every module the root reaches; (N)
   a new flag for it; (S) keep the root only and say in the message which
   modules hold fixes (621 began that); (L) the library modules of a package
   left alone; and any route nobody listed.
3. Atomicity and safety: every file written whole or none, a file not
   writable, a module reached through `use` outside the root's directory
   (a package, `examples/` from `tests/`), a symlink, the same module reached
   twice, `--json`'s document.
4. Does the spec or design.md state what `--apply` writes (grep both), and
   at what cost a sentence.

## Rules every seat follows

- Every number by a command you ran, named beside it; a negative sentence
  goes out as a question naming what you searched.
- Write your report as you go to `docs/panel/211-reports/<seat>.md`.
- No paid run, no `pkill` by pattern, no commit, no push; `caffeinate -i`;
  times from `date`; English; no em dashes.
- Verdict per route, recommendation, one scorable prediction. Aim to finish
  by 03:15 by `date`.
