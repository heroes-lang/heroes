---
kind: defect
area: process
milestone: M-issue-files
filed: 2026-10-11
commit: ca3d3532a4d7b96cc11683d7de7c3b22318eca1b
github: https://github.com/heroes-lang/heroes/issues/2016
---

- [x] **671 — a GitHub write from the assistant's shell asks nothing: the local permissions allow `gh api` on the repository and `guard_bash.py` does not name `gh`** | `.claude/settings.local.json` (the author's, git-ignored) allows `Bash(gh api repos/heroes-lang/heroes/*)`, `Bash(gh api --allow-escape-sequences repos/heroes-lang/heroes/*)` and `Bash(gh run *)` and has no `ask` and no `deny`; `gh api` with any `-f` or `-F` field switches to POST (`gh api --help`), so `gh api repos/heroes-lang/heroes/issues -f title=x` is a write that passes; the tracked `.claude/settings.json` asks only for `git push*` and `guard_bash.py` has no rule about `gh` (`grep -c -E '\bgh\b' .claude/hooks/guard_bash.py` is 0); CLAUDE.md § Hard stops says an outward-facing act is asked for every time and that the settings and hooks enforce it | `.claude/hooks/guard_bash.py` · `.claude/settings.json` · `.claude/hooks/test_hooks.py` · panel 212 R7 · **class: blocking**

    **Origin:** panel 212's spec-warden asked whether a tool that pushes would lose a confirmation a hand-run `gh` call gives; the completeness critic's second pass read both settings files and the hook and found the confirmation is not there today, 2026-10-11. Nothing was written to GitHub by any seat (only GET reads and one GraphQL query, read-only in effect); the defect is the missing refusal.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a hard stop of the contract that the machine does not enforce, on the eve of 2,014 GitHub writes; the transcription cannot begin over it.

    **What a repair owes**: a refusal in `guard_bash.py`, tested in `test_hooks.py`, of every GitHub write from the assistant's shell (`gh api` with a method other than GET, `-X`, `-f`, `-F`, `--input`, except the one read-only POST to `/markdown`; the write subcommands of `gh issue`, `gh label`, `gh repo`, `gh release` and `gh milestone`; `gh api graphql` carrying `mutation`), through the wrappers the guard already reads (`env`, `time`, `caffeinate`, `timeout`, a shell's `-c`); and an `ask` entry in the tracked `settings.json` for the transcription program's `push`. The author's `settings.local.json` is not edited by the assistant.

    **2026-10-11, lane panel-212**: Repaired at `ca3d3532`, gated by its cases (nine in `test_hooks.py`, six red on the previous guard) and the hooks' own tests (171 OK); the net is owed at the batch's close. `guard_bash.py` reads `gh` through an allow-list of reads and refuses every other verb, `gh api` as anything but a GET, a GraphQL `mutation` and `gh auth` but `status`, through the wrappers it already reads. Left open on purpose: the tracked `settings.json`'s `ask` entry for the transcription program's `push`, which waits for the program.

## The repair

Two halves, as panel 212's R7 asked. The guard at `ca3d3532`: `.claude/hooks/guard_bash.py` reads `gh` through an allow-list of reads and refuses every GitHub write in a shell the hooks judge, the assistant's and a seat's (defect 672 measured the second on 2026-10-12), with nine cases in `.claude/hooks/test_hooks.py`. And the transcription program's writing words ask the author, whatever the local permissions allow: `.claude/settings.json` holds `ask` entries for `push`, `prepare` and `finish` of `tests/harness/issues.hero`, each spelled four ways (with and without `./`, with and without `caffeinate -i`), held by the hooks' `TrackedAsks` tests, landed with that program in M-issue-files step 14. What no line here can see stands said in the guard's comment: a program that spawns `gh` inside, which is why the program's words carry their own `--yes` and their own `ask`.

**Closed 2026-10-12** in M-issue-files step 14, the commit that writes this section.
