---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 4070b5eb5768ac68eb3fe560bae7a5e88e5f4422
github: none
---

- [ ] **534 — the guard reads a segment's first word, so git inside `find -exec` or an alias goes unjudged** | every rule of `.claude/hooks/guard_bash.py` reads a command segment by its first word, so a git command run by `find ... -exec git ...` and a git alias that expands to a refused command are never judged, whatever they discard or commit (lane b16-misc) | `.claude/hooks/guard_bash.py`, `.claude/hooks/discards.py` · defects 514 and 529 · **class: adjacent**

    **Origin:** filed by the coordinator at 15:09 on 2026-10-09 from lane b16-misc's final report (its notes `.claude/worktrees/scratch-b15/misc/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a peer's work in the shared checkout the hard stops name.

    Repaired at `4070b5eb`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `guard_bash.commands_in` gives every command a segment runs and judges each as the line it was given: a git command a `find` runs by `-exec`, `-execdir`, `-ok` or `-okdir` (`runs.executed`, a word holding `{}` read as every path), and what a git alias stands for, asked of git in the command's directory with its options and environment (a shell alias's line judged from the top of the work tree), never for a name git runs a command of its own for (`runs.BUILTIN`, held to `git --list-cmds=builtins`); beside it, with the same cause, `\;` and a backslash-newline read as the shell reads them, the reserved words, a function's header and a `case` pattern before a command, substitutions, and xargs's options that take a value. 10 tests, 9 red on the base; the hooks' own tests 135, all passed; an `ls` 202.1 to 209.0 M instructions in the guard's process.
