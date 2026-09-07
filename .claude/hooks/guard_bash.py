#!/usr/bin/env python3
"""PreToolUse guard on Bash: refuse the commands CLAUDE.md § Hard stops forbids.

Why this exists. Those refusals were prose from 2026-09-03 to 2026-09-07, and
measured on the day it was written, `.claude/settings.local.json` on this machine
allow-listed `git add *`, `git commit *` and `git push *`, so all three of the
categorical git refusals were pre-approved and nothing ever checked them
(docs/contract/case-law.md CL-041). A rule performed by nothing is not a rule.

Contract with the harness: read the tool call as JSON on stdin, look at
`tool_input.command`, and exit 2 with the reason on stderr to block. Exit 0
means no opinion, and the normal permission flow continues.

Portability. If `python3` is missing the hook cannot run, and Claude Code treats
a non-2 failure as a warning rather than a block, so the prose rule is still the
backstop on a machine without it. Nothing here touches the tree or the network.
"""

import json
import re
import shlex
import sys

CONTRACT = "CLAUDE.md § Hard stops"


def segments(command):
    """Split a command line into the pieces a shell would run separately.

    A guard that only looks at the whole string is blind to
    `cd x && git add -A`, which is how these commands actually get typed.
    """
    return [s.strip() for s in re.split(r"&&|\|\||[;\n|]", command) if s.strip()]


def words(segment):
    try:
        return shlex.split(segment)
    except ValueError:
        # An unbalanced quote is not our business to fix; fall back to a coarse
        # split so a malformed line cannot slip a forbidden verb past us.
        return segment.split()


def git_args(w, *verbs):
    """The arguments AFTER `git <verb>`, or None when this is not that command.

    It returns the tail rather than a boolean because a global flag can carry a
    value that looks like an argument: in `git -C . add CLAUDE.md` the dot
    belongs to `-C`, and reading the whole word list saw `git add .` and refused
    a legitimate command. Measured against that case on 2026-09-07.
    """
    if not w or w[0] != "git":
        return None
    i = 1
    while i < len(w) and w[i].startswith("-"):
        if w[i] in ("-C", "-c", "--git-dir", "--work-tree", "--namespace"):
            i += 2
        else:
            i += 1
    if i < len(w) and w[i] in verbs:
        return w[i + 1:]
    return None


def short_flags(w):
    """Every letter of every clustered short flag, so `-am` yields a and m."""
    out = set()
    for token in w:
        if token.startswith("-") and not token.startswith("--"):
            out.update(token[1:])
    return out


def verdict(command):
    """Return a refusal string, or None to stay out of the way."""
    for segment in segments(command):
        w = words(segment)
        if not w:
            continue

        rest = git_args(w, "add")
        if rest is not None:
            if {"-A", "--all", "-u", "--update"} & set(rest):
                return (
                    "refused: `git add` with -A, --all or -u stages every "
                    "session's work in this shared checkout. Name each file "
                    "this conversation touched. " + CONTRACT
                )
            if "." in rest:
                return (
                    "refused: `git add .` stages every session's work in this "
                    "shared checkout. Name each file this conversation "
                    "touched. " + CONTRACT
                )

        rest = git_args(w, "commit")
        if rest is not None and ("a" in short_flags(rest) or "--all" in rest):
            return (
                "refused: `git commit -a` stages files this conversation may "
                "not have read. Stage by name, then commit. " + CONTRACT
            )

        rest = git_args(w, "stash")
        if rest is not None:
            # Reading the stash is harmless; taking it is what breaks a peer.
            if not ({"list", "show"} & set(rest)):
                return (
                    "refused: `git stash` takes every session's changes, not "
                    "this one's. " + CONTRACT
                )

        rest = git_args(w, "push")
        if rest is not None and ({"-f", "--force"} & set(rest)):
            return (
                "refused: a force push rewrites what other sessions and CI "
                "have already read. " + CONTRACT
            )

        # An ASSIGNMENT, not the bare word: `grep -rn UPDATE_GOLDEN .` is how
        # somebody checks the rule still holds, and refusing that would be a
        # guard that punishes reading.
        if any(t.startswith("UPDATE_GOLDEN=") for t in w):
            return (
                "refused: UPDATE_GOLDEN does not exist and must not; a red "
                "golden is read and repaired, never regenerated. " + CONTRACT
            )

        # Only when it is being PASSED to the compiler, for the same reason.
        if "--no-line" in w and "heroes" in w[0]:
            return (
                "refused: `--no-line` is not on the tool surface; emitter "
                "debugging is the test helper's job. "
                "`.claude/rules/cli-surface.md`"
            )

    return None


def main():
    try:
        payload = json.load(sys.stdin)
    except (json.JSONDecodeError, ValueError):
        return 0

    command = (payload.get("tool_input") or {}).get("command")
    if not isinstance(command, str):
        return 0

    reason = verdict(command)
    if reason is None:
        return 0

    print(reason, file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
