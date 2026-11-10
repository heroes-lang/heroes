#!/usr/bin/env python3
"""What may run ahead of a commit on its command line: nothing that writes.

Why this exists. The commit guard judges what a commit carries by reading the
working tree and the index (`staged.offences`), and it reads them when the
hook runs, before the shell has run any part of the line. Defect 2380, lane
b26-misc, 2026-11-09: with an untracked folder case holding a file that does
not parse, `git add -- <dir> && git commit -F m -- <dir>` was let through and
committed it, while the same two commands typed apart were refused (*does not
parse*): at the moment the guard asked, the file was neither staged nor known
to git, so `git diff HEAD -- <dir>` did not list it.

**The cause is the moment, not `git add`.** Measured 2026-11-10 in scratch
repositories, git 2.56.0: every command ahead of the commit that writes the
tree or the index puts its file into the commit unjudged, in the same way. A
file added and its directory committed; `git add -N` (intent to add), which
the guard sees when it is a command of its own; a `git add` joined by `;`, or
by `||` where `--ignore-errors` stages what it can read and exits 1 on a file
it cannot; `git stage`, git's other name for it; a `git add` of two files
and a commit of one, the other staged unjudged where the two-command form
judges every staged file; `printf ... > <file>`, `cp` and `git checkout
<commit> -- <path>` writing a file the pathspec then takes. A copy of what
`git add` would stage could never be complete, since any program can write a
file; so the guard does not simulate the line, it refuses the commit whose
line runs, before it, anything that is not known to write neither a file nor
an index, and the message says to run that command on its own first. The
guard's older rule of the same shape is the gate and the commit that never
share a line (CL-079).

WHAT COUNTS AS INERT, and why each: the moves `placed` already follows (`cd`,
`pushd`, `popd`); the commands that print or read (`true`, `:`, `pwd`,
`echo`, `printf`, `test`, `[`, `[[`, `ls`, `cat`, `head`, `tail`, `wc`,
`grep`); and git's verbs that read (`READS`), `--output`, which writes the
answer to a file, aside. Any of them with an output redirection to a file is a
writer (`redirected`); one to `/dev/null` or to a descriptor is not. A git
alias is not read as inert even where it stands for a reading verb, and
neither is `set`, `export` or a shell's `-c`: the guard errs toward refusing,
and the refusal costs one command typed apart.
"""

import os

import runs

# The commands that change neither a file nor an index whatever their words,
# an output redirection aside (`redirected`).
INERT = frozenset({
    "cd", "pushd", "popd", "true", ":", "pwd", "echo", "printf", "test", "[", "[[",
    "ls", "cat", "head", "tail", "wc", "grep",
})
# git's verbs that read the repository and write neither the working tree nor
# the index (`git status` may rewrite the index's record of file times, never
# what it holds).
READS = frozenset({
    "status", "diff", "log", "show", "rev-parse", "ls-files", "ls-tree", "cat-file",
    "describe", "rev-list", "merge-base", "for-each-ref", "check-ignore", "blame",
})
# Where a redirection may point and write no file.
NOWHERE = "/dev/null"
# The characters that end a redirection's target outside quotes.
ENDS = frozenset(" \t\n;&|<>()")


def word_at(text, i):
    """The word of `text` from `i`, its quotes read as the shell reads them,
    to the first blank or operator outside them."""
    out, quote = [], None
    while i < len(text):
        c = text[i]
        if quote:
            if c == quote:
                quote = None
            elif c == "\\" and quote == '"' and i + 1 < len(text):
                i += 1
                out.append(text[i])
            else:
                out.append(c)
            i += 1
            continue
        if c in ENDS:
            break
        if c in "'\"":
            quote = c
        elif c == "\\" and i + 1 < len(text):
            i += 1
            out.append(text[i])
        else:
            out.append(c)
        i += 1
    return "".join(out)


def redirected(segment):
    """The file an output redirection of `segment` writes, outside quotes:
    `>`, `>>`, `>|`, `<>`, `&>` and a numbered one alike, or `>(` for a
    process substitution; None where every one points at `/dev/null` or at a
    descriptor (`2>&1`, `>&2`), or where there is none. `$(( ... ))` is
    arithmetic and is skipped."""
    i, quote = 0, None
    while i < len(segment):
        c = segment[i]
        if quote == "'":
            quote = None if c == "'" else quote
            i += 1
            continue
        if c == "\\":
            i += 2
            continue
        if c == '"':
            quote = None if quote == '"' else '"'
            i += 1
            continue
        if quote is None and c == "'":
            quote = "'"
            i += 1
            continue
        if quote is None and segment.startswith("$((", i):
            end = segment.find("))", i + 3)
            i = len(segment) if end < 0 else end + 2
            continue
        if quote is None and c == ">":
            j = i + 1
            if j < len(segment) and segment[j] in ">|":
                j += 1
            if j < len(segment) and segment[j] == "(":
                return ">("
            duplicated = j < len(segment) and segment[j] == "&"
            if duplicated:
                j += 1
            while j < len(segment) and segment[j] in " \t":
                j += 1
            target = word_at(segment, j)
            if duplicated and (target.isdigit() or target == "-"):
                i = j + len(target)
                continue
            if target != NOWHERE:
                return target or ">"
            i = j + len(target)
            continue
        i += 1
    return None


def inert(w, segment):
    """Whether the command `w`, its wrappers left out, inside the segment
    `segment`, writes neither a file nor an index (`INERT`, `READS`). A
    segment of assignments alone is inert; what a substitution in it runs is
    the caller's to read."""
    if redirected(segment) is not None:
        return False
    if not w:
        return True
    base = os.path.basename(w[0])
    if base in INERT:
        return True
    if base != "git":
        return False
    verb, at = runs.verb_of(w)
    if verb not in READS:
        return False
    return not any(word == "--output" or word.startswith("--output=") for word in w[at + 1:])


def shown(segment, limit=80):
    """`segment` on one line, cut at `limit` characters."""
    flat = " ".join(segment.split())
    return flat if len(flat) <= limit else flat[: limit - 3] + "..."


def refusal(writer, committer):
    """The refusal of a commit, `committer`, behind a command, `writer`, that
    is not known to leave the tree and the index alone."""
    return (
        "refused: `" + shown(writer) + "` runs before `" + shown(committer) + "` on one command line, "
        "and the guard judges what a commit carries by the files and the index as they stand "
        "before the line runs, so a file that command stages or writes would be committed "
        "unjudged (defect 2380, `.claude/hooks/ahead.py`). Run it as a command of its own, "
        "then the commit. "
    )
