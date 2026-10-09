#!/usr/bin/env python3
"""PreToolUse guard on Bash: refuse the commands CLAUDE.md § Hard stops forbids.

Why this exists. Those refusals were prose from 2026-09-03 to 2026-09-07, and
measured on the day it was written, `.claude/settings.local.json` on this machine
allow-listed `git add *`, `git commit *` and `git push *`, so all three of the
categorical git refusals were pre-approved and nothing ever checked them
(docs/records/contract/case-law.md CL-041). A rule performed by nothing is not a rule.

Contract with the harness: read the tool call as JSON on stdin, look at
`tool_input.command`, and exit 2 with the reason on stderr to block. Exit 0
means no opinion, and the normal permission flow continues.

Portability. If `python3` is missing the hook cannot run, and Claude Code treats
a non-2 failure as a warning rather than a block, so the prose rule is still the
backstop on a machine without it. Nothing here touches the tree or the network.
"""

import json
import os
import re
import shlex
import sys

# The hooks' own modules are found beside this file whatever runs it:
# `python3 -I`, isolated, leaves the script's directory off `sys.path`, and
# this file then failed at its first import (measured 2026-10-07, Python 3.14).
HOOKS = os.path.dirname(os.path.abspath(__file__))
if HOOKS not in sys.path:
    sys.path.insert(0, HOOKS)

import trees

# The modules only a git command needs, imported by `git_rules` the first time
# one is read.
commits = discards = staged = unseen = None

CONTRACT = "CLAUDE.md § Hard stops"
LAST_JUDGE = "`.claude/rules/verification.md` § A suite is the last judge (CL-079)"
HARNESS = "tests/harness/main.hero"

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


def heredoc_bodies(command):
    """Every heredoc BODY of `command`, in order: what `without_heredocs` drops
    is what a commit's `-F -` or `-m "$(cat <<EOF ...)"` reads as its message
    (defect 378)."""
    out = []
    lines = command.split("\n")
    i = 0
    while i < len(lines):
        line = lines[i]
        i += 1
        for match in HEREDOC.finditer(line):
            marker = match.group(1) or match.group(2) or match.group(3)
            body = []
            while i < len(lines) and lines[i].strip() != marker:
                body.append(lines[i])
                i += 1
            i += 1
            out.append("\n".join(body))
    return out


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


# What a shell runs a command THROUGH, each with its options that take a value:
# `GIT_EDITOR=true git merge --continue`, `caffeinate -i git ...` and
# `/usr/bin/time -p git ...` are git commands, and until 2026-10-07 this guard
# read their first word and saw none (defect 403's route of 2026-10-06 began
# with an assignment).
WRAPPERS = {
    "env": frozenset({"-u", "--unset", "-C", "--chdir"}),
    "command": frozenset(),
    "builtin": frozenset(),
    "exec": frozenset({"-a"}),
    "nohup": frozenset(),
    "time": frozenset(),
    "nice": frozenset({"-n"}),
    "caffeinate": frozenset({"-w", "-t"}),
    "timeout": frozenset({"-s", "--signal", "-k", "--kill-after"}),
    "xargs": frozenset({"-P", "-n", "-I", "-L", "-s", "-E", "-d", "-a"}),
}
ASSIGNMENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*=")
# The shells whose `-c` script is a command line of its own, and `eval`.
SHELLS = frozenset({"bash", "sh", "zsh", "dash", "ksh"})


def command_words(w):
    """`w` from the command it runs: the assignments before it and the
    wrappers it runs through, with their options, left out."""
    i = 0
    while i < len(w):
        if ASSIGNMENT.match(w[i]):
            i += 1
            continue
        base = os.path.basename(w[i])
        if base not in WRAPPERS:
            break
        takes = WRAPPERS[base]
        i += 1
        while i < len(w) and w[i].startswith("-") and w[i] != "-":
            if w[i] == "--":
                i += 1
                break
            i += 2 if w[i] in takes else 1
        if base == "timeout" and i < len(w):
            i += 1  # its duration
    return w[i:]


def script_of(w):
    """The command line a shell's `-c` or `eval` runs, or None. Lanes are told
    to run a list of paths through `bash -c` under zsh, and every rule here was
    blind inside one until 2026-10-07 (defect 403)."""
    if not w:
        return None
    base = os.path.basename(w[0])
    if base == "eval":
        return " ".join(w[1:])
    if base not in SHELLS:
        return None
    i = 1
    while i < len(w) and w[i][:1] in ("-", "+") and w[i] != "--":
        if w[i] in ("-o", "+o", "-O", "+O"):
            i += 2
            continue
        if not w[i].startswith("--") and "c" in w[i][1:]:
            return w[i + 1] if i + 1 < len(w) else None
        i += 1
    return None


def words(segment):
    try:
        return shlex.split(segment)
    except ValueError:
        # An unbalanced quote is not our business to fix; fall back to a coarse
        # split so a malformed line cannot slip a forbidden verb past us.
        return segment.split()


def git_rules():
    """Import `commits`, `discards`, `staged` and `unseen`, which only the git
    rules use. This guard runs before every Bash call and most are not git:
    imported at the top, the three of 2026-10-07 and what they import took an
    `ls` from 235.6 to 251.8 million instructions (measured that day, the
    guard's own process)."""
    global commits, discards, staged, unseen
    if commits is None:
        if HOOKS not in sys.path:
            sys.path.insert(0, HOOKS)
        import commits as commits_module
        import discards as discards_module
        import staged as staged_module
        import unseen as unseen_module

        commits, discards, staged, unseen = commits_module, discards_module, staged_module, unseen_module


def git_dir(w, here):
    """The directory a `git` command runs in: `here`, moved by each `-C` in
    turn as git moves it; None where the text cannot tell. Defect 287: the
    staged files of a `git -C <lane> commit` are the lane's."""
    if not w or os.path.basename(w[0]) != "git":
        return here
    i = 1
    while i < len(w) and w[i].startswith("-"):
        if w[i] == "-C" and i + 1 < len(w):
            target = w[i + 1]
            if "$" in target or "`" in target:
                here = None
            elif os.path.isabs(target):
                here = os.path.normpath(target)
            elif target and here is not None:
                here = os.path.normpath(os.path.join(here, target))
            i += 2
        elif w[i] in ("-c", "--git-dir", "--work-tree", "--namespace"):
            i += 2
        else:
            i += 1
    return here


def git_args(w, *verbs):
    """The arguments AFTER `git <verb>`, or None when this is not that command.

    It returns the tail rather than a boolean because a global flag can carry a
    value that looks like an argument: in `git -C . add CLAUDE.md` the dot
    belongs to `-C`, and reading the whole word list saw `git add .` and refused
    a legitimate command. Measured against that case on 2026-09-07.
    """
    if not w or os.path.basename(w[0]) != "git":
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


def is_harness_run(w):
    """`heroes run tests/harness/main.hero -- <compiler> ...`, whatever the
    binary is called and whether the harness is named from the tree's root or
    by its whole path (defect 348's shape beside it, 2026-10-07)."""
    named = any(t == HARNESS or t.endswith("/" + HARNESS) for t in w)
    return named and "run" in w


def is_gate(w):
    """A harness run, or `heroes test <file>`: the commands whose exit is a verdict."""
    if is_harness_run(w):
        return True
    return len(w) > 2 and w[1] == "test" and os.path.basename(w[0]).startswith("heroes")


def bare(w):
    """`w` without the brackets of a subshell around it, `(cd x` read as `cd x`."""
    out = list(w)
    if out and out[0].startswith("("):
        out[0] = out[0][1:]
        if not out[0]:
            out = out[1:]
    if out and out[-1].endswith(")"):
        out[-1] = out[-1][:-1]
        if not out[-1]:
            out = out[:-1]
    return out


def moved_to(w, here):
    """Where the directory stands after the segment `w`, run in `here`: a
    `cd` or `pushd` moves it, anything else leaves it. None when the text
    cannot tell, a variable this hook does not hold or `cd -`, so a question
    about the tree is not answered with the wrong one."""
    w = bare(w)
    if not w or w[0] not in ("cd", "pushd", "popd"):
        return here
    if w[0] == "popd":
        return None
    named = [a for a in w[1:] if not (a.startswith("-") and a != "-")]
    target = named[0] if named else "~"
    if target == "-":
        return None
    target = os.path.expandvars(os.path.expanduser(target))
    if "$" in target or "`" in target:
        return None
    if os.path.isabs(target):
        return os.path.normpath(target)
    if here is None:
        return None
    return os.path.normpath(os.path.join(here, target))


def placed(command, cwd):
    """Each segment's words with the directory it runs in, `None` where the
    text cannot tell. Until 2026-10-07 every segment was read in the session's
    directory, so `cd <lane> && ./heroes run ...` was judged in the trunk
    (defect 348)."""
    here = os.path.abspath(cwd) if cwd else None
    out = []
    for segment in segments(command):
        w = words(segment)
        if not w:
            continue
        out.append((w, here, segment))
        here = moved_to(w, here)
    return out


def stale_compiler(w, here):
    """The refusal when a harness run names a compiler older than the tree it
    runs in, else None.

    **Added 2026-09-29 (CL-079).** On 2026-09-18 the full net was judged by a
    binary built at 02:08 against a HEAD of 18:17: 1862 passed, 23 failed, none
    of the 23 saying *your compiler is old*, two full nets for one bit of
    information (`.claude/rules/verification.md` § The compiler that judges is a
    build artifact). Rebuilding costs seconds; this asks for it before the gate.
    `heroes test <file>` is not touched: it compiles the source it is given.

    **The tree is the one the run stands in, since 2026-10-07** (defect 348,
    `trees.py`): the nearest directory above the segment's own directory, its
    `cd` read, holding `seed/heroes.c` and a `.git` entry, or above the
    compiler's own path where the directory cannot be told. The harness reads
    the tree from its working directory, so that is the tree being judged.
    Until that day the session's directory was asked, and a lane's run from a
    session on the trunk was refused for the trunk's age while the lane's
    compiler was newer than every file of the lane.
    """
    if not is_harness_run(w) or "--" not in w:
        return None
    at = w.index("--")
    if at + 1 >= len(w):
        return None
    named = w[at + 1]
    if os.path.isabs(named):
        compiler = named
    elif here is not None:
        compiler = os.path.join(here, named)
    else:
        return None
    if not os.path.isfile(compiler):
        return None
    tree = trees.tree_of(here) if here is not None else None
    if tree is None:
        tree = trees.tree_of(compiler)
    if tree is None:
        return None
    old = trees.older_than_tree(compiler, tree)
    if old is None:
        return None
    where, built, wrote = old
    return (
        "refused: " + named + " was built at " + trees.stamp(built) + ", before " + where
        + " was written at " + trees.stamp(wrote) + " in the tree this run judges, " + tree
        + ", so the net would judge with a compiler older than the tree. Build it first, "
        "there: " + trees.rebuild_hint(where) + ". " + LAST_JUDGE
    )


def verdict(command, cwd=None):
    """Return a refusal string, or None to stay out of the way."""
    cwd = cwd or os.getcwd()
    parsed = [words(s) for s in segments(command)]
    parsed = [w for w in parsed if w]

    # **A gate and a commit never share a command line** (2026-09-29, CL-079).
    # Twice in one week a commit went past a red `records` because the chain
    # read the exit status of the pipe's last command (journal 061, `f22c8baf`,
    # `efaf5564`). The gate's line is read, then the commit is its own command.
    if any(is_gate(w) for w in parsed) and any(git_args(command_words(bare(w)), "commit") is not None for w in parsed):
        return (
            "refused: a gate and a `git commit` on one command line, so the commit "
            "cannot wait for the gate's line to be read. Run the gate, read its "
            "counts, then commit as its own command. " + LAST_JUDGE
        )

    # **A gate's output is never cut** (2026-09-29, CL-079). Seven `emit`
    # goldens stayed red for two steps behind an output cut at forty lines
    # (journal 061). Redirect it to a file and read the file whole.
    if any(is_harness_run(w) for w in parsed) and any(w[0] in ("head", "tail") for w in parsed):
        return (
            "refused: the net's output piped into `" + next(w[0] for w in parsed if w[0] in ("head", "tail")) + "` "
            "cuts the lines that carry the verdict. Send it to a file and read the "
            "whole file, or read the terminal as it is. " + LAST_JUDGE
        )

    steps = placed(command, cwd)
    for w, here, _segment in steps:
        stale = stale_compiler(w, here)
        if stale is not None:
            return stale

    for raw, here, segment in steps:
        # **A bare dump** is read before the wrappers are left out, `env`
        # being one of them (its rule is below, with its story).
        if raw[0] in DUMPERS and len(raw) == 1:
            return (
                "refused: a bare `" + raw[0] + "` prints every variable in the "
                "environment, the live keys included. Name the one you want. "
                "`.env.example`"
            )
        w = command_words(bare(raw))
        if not w:
            continue

        script = script_of(w)
        if script is not None:
            inner = verdict(script, here or cwd)
            if inner is not None:
                return inner
            continue
        where = git_dir(w, here)
        is_git = bool(w) and os.path.basename(w[0]) == "git"
        if is_git:
            git_rules()

        # **A message is read for what its reader cannot see** (defect 378): the
        # list is the `unseen` suite's, read from the tree the command stands in.
        for verb in unseen.VERBS if is_git else ():
            more = git_args(w, verb)
            if more is not None:
                tree = trees.tree_of(where) if where is not None else None
                said = unseen.verdict(more, verb, where, heredoc_bodies(command), unseen.runs(tree or trees.tree_of(cwd)))
                if said is not None:
                    return said

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
        # (docs/records/contract/case-law.md CL-070). The rule above it, CL-041, asks
        # for exactly what this prevents and its own procedure could not deliver
        # it. The pathspec is the only thing that limits a commit, so this guard
        # asks for the pathspec rather than for care.
        #
        # **One route takes no pathspec, and it is checked by its index**
        # (defect 403, 2026-10-07): a merge, a cherry-pick or a revert that
        # stopped on a conflict is concluded with the whole index, git refusing
        # a partial commit while it stands, so a bare commit then is allowed
        # when the index holds no file the operation did not bring
        # (`commits.concluded`). A rebase or an am takes a pathspec, so a bare
        # commit while one stands keeps this refusal (defect 487).
        if rest is not None and "--" not in rest:
            sequence = commits.concluded(where, None)
            if sequence:
                return sequence
            if sequence is None:
                return (
                    "refused: `git commit` with no `--` takes the WHOLE index, "
                    "including whatever a parallel session has staged in this "
                    "shared checkout. Naming the paths to `git add` does not limit "
                    "the commit; only the pathspec does. Write "
                    "`git commit -- <paths>`. " + CONTRACT + " (CL-070)"
                )
        if rest is not None:
            said = commits.pathspec(rest, where)
            if said is not None:
                return said
        # A rebase and an am are concluded the same way, and `--cont` is
        # `--continue` to git (defect 487, `commits.continues`).
        for name in commits.CONCLUDED_BY if is_git else ():
            more = git_args(w, name)
            if more is not None and commits.continues(name, more):
                said = commits.concluded(where, name)
                if said:
                    return said
                rest = more
        if rest is not None:
            offences = staged.offences(where, commits.paths_of(git_args(w, "commit")))
            if offences:
                return (
                    "refused: a `.hero` file this commit carries would reach the suites with "
                    "what a hook can see now:\n    " + "\n    ".join(offences) + "\n" + LAST_JUDGE
                )

        rest = git_args(w, "stash")
        if rest is not None:
            # Reading the stash is harmless; taking it is what breaks a peer.
            if not ({"list", "show"} & set(rest)):
                return (
                    "refused: `git stash` takes every session's changes, not "
                    "this one's. " + CONTRACT
                )

        # What throws work away is read as what commits it (defect 514,
        # `discards.py`): an abort, a skip, a hard or merge reset, an autostash.
        for verb in discards.VERBS if is_git else ():
            more = git_args(w, verb)
            if more is not None:
                said = discards.verdict(verb, more, where, discards.git_options(w), discards.environment(bare(raw)))
                if said is not None:
                    return said

        rest = git_args(w, "push")
        if rest is not None and ({"-f", "--force"} & set(rest)):
            return (
                "refused: a force push rewrites what other sessions and CI "
                "have already read. " + CONTRACT
            )

        # An ASSIGNMENT, not the bare word: `grep -rn UPDATE_GOLDEN .` is how
        # somebody checks the rule still holds, and refusing that would be a
        # guard that punishes reading.
        if any(t.startswith("UPDATE_GOLDEN=") for t in raw):
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
        # Performed at the top of this loop, on the words as written, since
        # `env` is also a wrapper the words above are read through.
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

    reason = verdict(command, payload.get("cwd") or os.getcwd())
    if reason is None:
        return 0

    print(reason, file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
