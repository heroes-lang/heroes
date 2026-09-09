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

# The one endpoint the Anthropic key is in `.env` for, and the clients that can
# actually reach it. A python one-liner is a client; `grep` is a reader.
COUNT_TOKENS = "/v1/messages/count_tokens"
CLIENTS = frozenset({"curl", "wget", "http", "https", "xh", "httpie",
                     "python", "python3", "node", "deno"})
PRINTERS = frozenset({"echo", "printf", "print", "printenv", "env", "set"})
# The ones that print the WHOLE environment when given nothing to print.
DUMPERS = frozenset({"env", "printenv", "set", "export", "declare"})
READERS = frozenset({"cat", "less", "more", "head", "tail", "bat", "open",
                     "nl", "strings"})
SECRETS = ("$ANTHROPIC_API_KEY", "${ANTHROPIC_API_KEY}",
           "$CLOUDFLARE_API_TOKEN", "${CLOUDFLARE_API_TOKEN}")


HEREDOC = re.compile(r"<<-?\s*(?:'([^']+)'|\"([^\"]+)\"|([A-Za-z_][A-Za-z0-9_]*))")


def without_heredocs(command):
    """Drop every heredoc BODY, keeping the commands around it.

    **A false positive is the worst failure this guard has, and it had one on
    its first day.** A `python3 - <<'EOF'` writing a record entry was refused as
    `git add .`, because the entry's own prose quoted that command while
    explaining why it is forbidden. The guard was reading DATA as if it were a
    command line, and it blocked twenty minutes of legitimate work with a
    message about a rule the text was documenting.

    A heredoc body is fed to a program's stdin. It is never executed by the
    shell, so it is never this guard's business. Everything else stays: a
    command after the body is still scanned, which the crude fix of truncating
    at the first `<<` would have lost.
    """
    out = []
    lines = command.split("\n")
    i = 0

    while i < len(lines):
        line = lines[i]
        out.append(line)
        i += 1
        # Several heredocs can open on one line (`a <<X b <<Y`); their bodies
        # then arrive in order, so each delimiter is consumed in turn.
        for match in HEREDOC.finditer(line):
            marker = match.group(1) or match.group(2) or match.group(3)
            while i < len(lines) and lines[i].strip() != marker:
                i += 1
            i += 1  # the delimiter line itself

    return "\n".join(out)


def segments(command):
    """Split a command line into the pieces a shell would run separately.

    A guard that only looks at the whole string is blind to
    `cd x && git add -A`, which is how these commands actually get typed.

    **The split is quote-aware, since 2026-09-08.** It was a plain `re.split`
    over the raw text, which cut inside quotes as happily as outside them, so
    `echo "cd x && git commit -m y"` produced a segment reading
    `git commit -m y"` and the guard read a QUOTED MENTION as a command. It cost
    nothing while the rules only watched `git add -A`, a string nobody writes in
    passing, and it fired the day a rule started watching `git commit`, which
    documentation and test scripts write all the time. Same lesson as
    `without_heredocs` above, one level up: data is not a command line.
    """
    text = without_heredocs(command)
    out, buf, quote = [], [], None
    i = 0
    while i < len(text):
        c = text[i]
        if quote:
            buf.append(c)
            # Inside double quotes a backslash escapes the next character;
            # inside single quotes it does not, which is why `'\''` closes.
            if c == "\\" and quote == '"' and i + 1 < len(text):
                i += 1
                buf.append(text[i])
            elif c == quote:
                quote = None
            i += 1
            continue
        if c in "'\"":
            quote = c
            buf.append(c)
            i += 1
            continue
        if text.startswith("&&", i) or text.startswith("||", i):
            out.append("".join(buf))
            buf = []
            i += 2
            continue
        if c in ";\n|":
            out.append("".join(buf))
            buf = []
            i += 1
            continue
        buf.append(c)
        i += 1
    out.append("".join(buf))
    return [s.strip() for s in out if s.strip()]


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

        # **A bare `git commit` takes the whole index, and naming the paths to
        # `git add` limits nothing.** Measured on 2026-09-08, by doing it: a
        # commit that named fourteen paths carried sixteen, the two extra being
        # files a parallel session had staged in this shared checkout, and the
        # commit body said in so many words that they were not in it
        # (docs/contract/case-law.md CL-070). The rule above it, CL-041, asks
        # for exactly what this prevents and its own procedure could not deliver
        # it. The pathspec is the only thing that limits a commit, so this guard
        # asks for the pathspec rather than for care.
        if rest is not None and "--" not in rest:
            return (
                "refused: `git commit` with no `--` takes the WHOLE index, "
                "including whatever a parallel session has staged in this "
                "shared checkout. Naming the paths to `git add` does not limit "
                "the commit; only the pathspec does. Write "
                "`git commit -- <paths>`. " + CONTRACT + " (CL-070)"
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

        # **The Anthropic key counts tokens and does nothing else** (author
        # instruction 2026-09-09, the hour the key was put in `.env`). It was
        # added for one job: `spec/heroes-spec.md` is budgeted against two
        # VENDORED tokenisers and one of them, `cl100k_base`, is OpenAI's, so the
        # binding number was never the one the reader's tokeniser gives. Counting
        # is a read. Inference is spending somebody's money from a shell that
        # also has the deploy token, and it is not why the key is here.
        #
        # It fires only when the segment's own COMMAND is a client that can make
        # the request. `grep -rn api.anthropic.com .claude/` is how somebody
        # checks this rule still holds, and a guard that refuses reading is the
        # failure `without_heredocs` above was written for.
        if w[0] in CLIENTS and "api.anthropic.com" in segment:
            if COUNT_TOKENS not in segment:
                return (
                    "refused: the Anthropic key is for `" + COUNT_TOKENS + "` "
                    "and nothing else. Inference, agents and the organisation "
                    "endpoints are not why it is in `.env`. "
                    "`.env.example` § Anthropic"
                )

        # **A secret in a transcript is a leaked secret**, and the transcript is
        # written whether anybody rereads it or not. `${#VAR}` is a length and
        # `source .env` is how the key is loaded, so neither is touched; what is
        # refused is expanding the VALUE into output, and reading `.env` itself.
        # `.env.example` stays legal because it carries no value, which is the
        # whole reason that file exists.
        if w[0] in PRINTERS:
            for token in w:
                if any(secret in token for secret in SECRETS):
                    return (
                        "refused: that expands a secret into the transcript. "
                        "Test it without printing it, or print `${#VAR}`. "
                        "`.env.example`"
                    )

        # **A bare dump names no secret and prints every one of them**, which is
        # the hole the rule above had on its first hour: it read the words for a
        # secret's name, and `env` on its own has no words. Found by the author
        # asking what the guard actually costs, 2026-09-09. A dump with
        # ARGUMENTS is somebody's legitimate `env FOO=1 cmd` prefix or
        # `printenv PATH`, so only the bare form goes.
        if w[0] in DUMPERS and len(w) == 1:
            return (
                "refused: a bare `" + w[0] + "` prints every variable in the "
                "environment, the live keys included. Name the one you want. "
                "`.env.example`"
            )
        if w[0] in READERS and any(t == ".env" or t.endswith("/.env") for t in w):
            return (
                "refused: `.env` holds the live keys. `.env.example` is the "
                "shape and carries no value. `.env.example`"
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
