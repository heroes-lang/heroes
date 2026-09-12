#!/bin/sh
# The entry point for every hook in this directory. Usage: run.sh <script.py>
#
# WHY THE INDIRECTION, and it is not tidiness. A PreToolUse hook that exits 2
# BLOCKS the tool call, and `python3` on a file it cannot open exits 2 as well.
# So naming python directly in `.claude/settings.json` would turn a missing hook
# file, a moved checkout, or an unset CLAUDE_PROJECT_DIR into a guard that
# refuses every Bash command in the session, with an error about a path rather
# than about a rule. Measured 2026-09-07 on this machine: `python3
# /nonexistent/hook.py` exits 2, `sh /nonexistent/run.sh` exits 127. Claude Code
# treats a non-2 failure as a warning, so routing through `sh` makes every
# failure mode of this wrapper loud and harmless instead of silent and total.
#
# The rules these hooks perform are CLAUDE.md § Hard stops and § Verification.
# What each one cost to learn is docs/records/contract/case-law.md CL-041 and CL-063.

script="${CLAUDE_PROJECT_DIR:-$PWD}/.claude/hooks/$1"

# No opinion is the safe default: a hook that cannot run must never stand in the
# way of work, because the prose rule in the contract is still the backstop.
[ -r "$script" ] || exit 0
command -v python3 >/dev/null 2>&1 || exit 0

exec python3 "$script"
