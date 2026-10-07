#!/usr/bin/env python3
"""What limits a commit: its pathspec, or, where git takes none, its index.

Why this exists. CLAUDE.md § Hard stops asks that a commit carry only this
conversation's files and says the pathspec is what limits it (CL-070); the
guard refused a commit with no `--` and passed every other. Defect 403, lane
b13-unit, 2026-10-06: a path array came out empty under macOS's bash 3.2,
which has no `mapfile`, so `git commit -F <file> --` took the whole index, 35
files, and missed 22 the commit meant. And measured the same evening: a
conflicted merge takes no pathspec (git refuses a partial commit during a
merge, and during a cherry-pick), so it is concluded with the whole index, and
`GIT_EDITOR=true git merge --continue` concluded `fae893f7` with nothing
asking what the index held. Measured again on 2026-10-07 in a scratch
repository, git 2.56.0: a file staged during a conflicted merge or
cherry-pick rides into the commit `--continue` makes; `git commit -- .` takes
every tracked change under the directory, staged or not, as `-a` does;
`git commit --amend --only --` amends the message and leaves the index alone;
and git refuses to START a merge over a staged change, so the conclusion is
the one moment to ask.

So a commit is refused when what follows its `--`:
- is nothing, but for `--amend --only`, which commits no path;
- is only expansions, `$...` or a backquote, which an empty list turns into
  nothing, and this guard reads the text before the shell expands it;
- names the whole tree, `.`, `:/`, `*` or the tree's own root or above.

And an operation concluded with the whole index, `git merge --continue`,
`git cherry-pick --continue`, `git revert --continue` or a `git commit` with
no `--` while that operation stands, is refused when the index holds a file
the operation did not bring: for a merge, a file the incoming side did not
change since the merge base (`git diff HEAD...MERGE_HEAD`), for a pick or a
revert, a file its commit did not change. A file the operation brought may
leave the index equal to HEAD (a resolution that kept this side), so the index
is asked to hold no OTHER file, not every one of them.
"""

import os

import staged

CONTRACT = "CLAUDE.md § Hard stops"

# The operations that conclude with the whole index, the file git keeps while
# each stands, and the files it brought.
OPERATIONS = (
    ("merge", "MERGE_HEAD"),
    ("cherry-pick", "CHERRY_PICK_HEAD"),
    ("revert", "REVERT_HEAD"),
)
WHOLE = frozenset({".", "./", ":", ":/", ":/*", "*", ":(top)", ":(top)*", ":(top)."})


def expansion(word):
    return "$" in word or "`" in word


def literal(paths):
    """The words of `paths` that are certainly a path as written: outside
    every expansion, `$x`, `"${xs[@]}"`, and the words a `$( ... )` or a
    backquoted command splits into once the quotes are read."""
    out = []
    depth = 0
    quoted = False
    for word in paths:
        inside = depth > 0 or quoted or expansion(word)
        depth = max(0, depth + word.count("$(") - word.count(")")) if (depth or "$(" in word) else 0
        if word.count("`") % 2 == 1:
            quoted = not quoted
        if not inside:
            out.append(word)
    return out


def pathspec(rest, where):
    """A refusal for the words after `git commit`, `rest`, by what follows its
    `--`, or None. A commit with no `--` is the caller's to judge."""
    if "--" not in rest:
        return None
    at = rest.index("--")
    options, paths = rest[:at], rest[at + 1:]
    if not paths:
        short = set()
        for token in options:
            if token.startswith("-") and not token.startswith("--"):
                short.update(token[1:])
        if "--amend" in options and ("--only" in options or "o" in short):
            return None
        return (
            "refused: `git commit` whose `--` names no path takes the WHOLE index, as a "
            "commit with no `--` does: an empty path list is how one commit carried 35 "
            "files and missed 22 it meant (defect 403). Name the paths after `--`. "
            + CONTRACT + " (CL-070)"
        )
    if not literal(paths):
        return (
            "refused: every path after `--` is an expansion, " + " ".join(paths) + ", and one "
            "that comes out empty leaves `git commit --`, which takes the whole index (defect "
            "403: a bash 3.2 array with no `mapfile`). Name the paths as words. "
            + CONTRACT + " (CL-070)"
        )
    wide = [p for p in literal(paths) if whole_tree(p, where)]
    if wide:
        return (
            "refused: `" + wide[0] + "` after `--` names the whole tree, so the commit takes "
            "every tracked change in it, staged or not, as `git commit -a` does. Name each "
            "path. " + CONTRACT + " (CL-070)"
        )
    return None


def whole_tree(path, where):
    """Whether the pathspec `path`, read in `where`, names the worktree's root
    or above it."""
    if path in WHOLE:
        return True
    if where is None or expansion(path) or path.startswith(":"):
        return False
    top = staged.toplevel(where)
    if top is None:
        return False
    named = os.path.normpath(os.path.join(where, path))
    return named == top or top.startswith(named.rstrip(os.sep) + os.sep)


def standing(top):
    """The operation standing in the worktree rooted at `top`, as (its name,
    the heads it brings), or None."""
    for name, head in OPERATIONS:
        found = staged.git(top, "rev-parse", "--git-path", head)
        if found is None:
            continue
        at = os.path.join(top, found.strip())
        try:
            with open(at, encoding="utf-8") as handle:
                heads = [line.strip() for line in handle if line.strip()]
        except OSError:
            continue
        if heads:
            return name, heads
    return None


def brought(top, name, heads):
    """The files the operation `name` of `heads` brings, or None when git
    cannot say."""
    files = set()
    for head in heads:
        if name == "merge":
            out = staged.git(top, "diff", "--name-only", "--no-renames", "HEAD..." + head)
        else:
            out = staged.git(top, "diff", "--name-only", "--no-renames", head + "^", head)
        if out is None:
            return None
        files.update(staged.lines(out))
    return files


def concluded(where, asked):
    """A refusal for concluding the operation that stands in `where` with an
    index holding a file it did not bring, or None. `asked` is the operation a
    `--continue` names, or None for a `git commit` with no `--`; then the
    answer is also None when no operation stands, and the caller's to give."""
    top = staged.toplevel(where)
    if top is None:
        return None
    now = standing(top)
    if now is None or (asked is not None and now[0] != asked):
        return None
    name, heads = now
    own = brought(top, name, heads)
    if own is None:
        return None
    held = staged.lines(staged.git(top, "diff", "--cached", "--name-only", "--no-renames"))
    extra = [f for f in held if f not in own]
    if not extra:
        return False
    return (
        "refused: concluding this " + name + " commits what the index holds, and it holds "
        + str(len(extra)) + " file" + ("s" if len(extra) > 1 else "") + " the " + name
        + " did not bring: " + ", ".join(extra[:12]) + ("" if len(extra) <= 12 else ", ...")
        + ". A " + name + " takes no pathspec, so the index is its only limit: unstage them "
        "(`git restore --staged -- <paths>`), conclude, and commit them apart. "
        + CONTRACT + " (CL-070, defect 403)"
    )
