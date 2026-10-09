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

**A rebase and an am are concluded the same way, since 2026-10-07** (defect
487). Measured that day in scratch repositories, git 2.56.0: a file staged
while a rebase or an am stands rides into the commit `git rebase --continue`
or `git am --continue` makes, by either rebase backend, at a conflicted pick,
a conflicted fixup and an `edit` stop alike (the last two amend the commit
before them with it). A stopped rebase keeps `REBASE_HEAD`, the commit being
picked, and no `CHERRY_PICK_HEAD`, so this guard read it as nothing standing;
an am keeps the patch it is applying, `rebase-apply/patch`, and no commit at
all. So a rebase brings what its pick's commit changed, and a rebase redoing
a merge what that merge brings (it keeps `MERGE_HEAD` beside `REBASE_HEAD`);
an am, and a rebase by the apply backend, what the patch names, old and new
names, read by `git apply --numstat` itself both ways. git takes a pathspec
while a rebase or an am stands (it refuses one only during a merge or a
cherry-pick), so a bare `git commit` then keeps the refusal of a commit with
no `--`; `--continue` takes none, so the index is its limit. And git reads
`--con`, `--cont` and up to `--continue` as `--continue` for a rebase, a merge
and an am (`git merge --cont` concluded a merge this guard did not watch), and
`--resolved` and `-r` as an am's `--continue`: a conclusion is read by every
spelling that can be one (`continues`).
"""

import os
import shlex

import staged

CONTRACT = "CLAUDE.md § Hard stops"

# The operations a single commit stands for, the file git keeps while each
# stands, and the files it brought. A bare commit while one stands is judged
# by the index (defect 403): git refuses a pathspec during a merge or a
# cherry-pick, and takes one during a revert (measured 2026-10-08).
OPERATIONS = (
    ("merge", "MERGE_HEAD"),
    ("cherry-pick", "CHERRY_PICK_HEAD"),
    ("revert", "REVERT_HEAD"),
)
# The sequences that stand in a directory of their own, concluded by
# `--continue` alone; a bare commit while one stands keeps its refusal.
SEQUENCES = ("rebase", "am")
# Every verb whose `--continue` concludes with the whole index.
CONCLUDED_BY = tuple(name for name, _head in OPERATIONS) + SEQUENCES
# An am's short options that take the rest of their word as a value, so an
# `r` after one of them is that value and not `-r`.
AM_VALUED = frozenset("CpS")
# The options of `git am` that name the paths its patch reaches: the rest of
# what `rebase-apply/apply-opt` holds is not asked of `git apply --numstat`.
AM_PATH_OPTIONS = ("-p", "--directory", "--include", "--exclude")
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


def paths_of(rest):
    """The paths the words after `git commit`, `rest`, name after their `--`,
    as written: the files a pathspec commit takes from the working tree, which
    the staged-file check judges too (defect 515); an expansion is the shell's
    and is left out. None of them where `rest` is None or holds no `--`."""
    if rest is None or "--" not in rest:
        return []
    return literal(rest[rest.index("--") + 1:])


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


def continues(verb, words):
    """Whether the words after `git <verb>` conclude it: any spelling git reads
    as `--continue`, a prefix of it from `--c` (git takes `--con` and longer
    for a rebase, a merge and an am, and refuses a shorter one as ambiguous,
    so reading it too refuses nothing git would run), and for an am
    `--resolved` and `-r`, alone or in a cluster of flags."""
    for word in words:
        if word == "--":
            return False
        if len(word) >= 3 and "--continue".startswith(word):
            return True
        if verb != "am":
            continue
        if len(word) >= 3 and "--resolved".startswith(word):
            return True
        if word.startswith("-") and not word.startswith("--"):
            for letter in word[1:]:
                if letter == "r":
                    return True
                if letter in AM_VALUED:
                    break
    return False


def kept(top, name):
    """The path of what git keeps as `name` in the worktree rooted at `top`
    (a file of its `.git`, or a directory), or None."""
    found = staged.git(top, "rev-parse", "--git-path", name)
    if found is None or not found.strip():
        return None
    return os.path.join(top, found.strip())


def heads_in(top, head):
    """The commits the file `head` holds, one a line, or []."""
    at = kept(top, head)
    if at is None:
        return []
    try:
        with open(at, encoding="utf-8") as handle:
            return [line.strip() for line in handle if line.strip()]
    except OSError:
        return []


def sequence(top):
    """The sequence standing in the worktree rooted at `top`, as (its name,
    the verbs whose `--continue` concludes it, the directory git keeps it
    in), or None. Measured on git 2.56.0: an am keeps `rebase-apply` holding
    `applying`, a rebase by the apply backend `rebase-apply` holding
    `rebasing`, one by the merge backend `rebase-merge`; and `git am
    --continue` concludes a rebase by the apply backend too, the index and a
    file staged beside its resolution with it (2026-10-08)."""
    merge = kept(top, "rebase-merge")
    if merge is not None and os.path.isdir(merge):
        return "rebase", ("rebase",), merge
    apply = kept(top, "rebase-apply")
    if apply is not None and os.path.isdir(apply):
        if os.path.exists(os.path.join(apply, "applying")):
            return "am", ("am",), apply
        return "rebase", ("rebase", "am"), apply
    return None


def standing(top):
    """Every operation standing in the worktree rooted at `top`, as (its
    name, the verbs that conclude it, what it brings: its heads for a merge,
    a pick or a revert, its directory for a sequence)."""
    out = []
    for name, head in OPERATIONS:
        heads = heads_in(top, head)
        if heads:
            out.append((name, (name,), heads))
    found = sequence(top)
    if found is not None:
        out.append(found)
    return out


def changed_by(top, head):
    """The files the commit `head` changed against its first parent, or
    against nothing for a root commit; None when git cannot say."""
    parent = staged.git(top, "rev-parse", "--verify", "--quiet", head + "^1")
    if parent is not None and parent.strip():
        out = staged.git(top, "diff", "--name-only", "--no-renames", parent.strip(), head)
    else:
        out = staged.git(top, "diff-tree", "-r", "--root", "--no-commit-id", "--name-only", "--no-renames", head)
    return None if out is None else set(staged.lines(out))


def patched(top, directory):
    """The paths the patch an am is applying names, old and new, by `git apply
    --numstat` itself, forward for the new names and reversed for the old (it
    prints one name a file), with the options of the am that move a path; None
    when git cannot say."""
    patch = os.path.join(directory, "patch")
    if not os.path.isfile(patch):
        return None
    options = []
    try:
        with open(os.path.join(directory, "apply-opt"), encoding="utf-8") as handle:
            words = shlex.split(handle.read())
    except (OSError, ValueError):
        words = []
    for at, word in enumerate(words):
        for option in AM_PATH_OPTIONS:
            if word == option and at + 1 < len(words):
                options += [word, words[at + 1]]
            elif word.startswith(option) and word != option and (option == "-p" or word.startswith(option + "=")):
                options.append(word)
    names = set()
    for reverse in ([], ["-R"]):
        out = staged.git(top, "apply", "--numstat", "-z", *options, *reverse, patch)
        if out is None:
            return None
        for row in out.split("\0"):
            parts = row.split("\t", 2)
            if len(parts) == 3 and parts[2]:
                names.add(parts[2])
    return names


def brought(top, name, what):
    """The files the operation `name` brings, `what` being what `standing`
    found for it, or None when git cannot say."""
    if name in SEQUENCES:
        if os.path.basename(what) == "rebase-apply":
            return patched(top, what)
        pick = heads_in(top, "REBASE_HEAD")
        # No pick in flight, a `break` or an `exec` stop: the rebase brings
        # nothing, and git refuses to continue over a staged change there
        # (`error: you have staged changes in your working tree`, exit 1,
        # measured at a `break` stop on 2026-10-08, git 2.56.0; an `exec` stop
        # is unmeasured).
        return changed_by(top, pick[0]) if pick else set()
    files = set()
    for head in what:
        if name == "merge":
            out = staged.git(top, "diff", "--name-only", "--no-renames", "HEAD..." + head)
            got = None if out is None else set(staged.lines(out))
        else:
            got = changed_by(top, head)
        if got is None:
            return None
        files.update(got)
    return files


def concluded(where, asked):
    """A refusal for concluding what stands in `where` with an index holding a
    file it did not bring, or None. `asked` is the verb a `--continue` names,
    or None for a `git commit` with no `--`; then the answer is also None
    when no merge, pick or revert stands, and the caller's to give.

    Every operation standing is asked what it brings, and the index may hold
    any of it: a rebase redoing a merge stands as both, and a pick made by
    hand at a rebase's `edit` stop as a pick inside a rebase."""
    top = staged.toplevel(where)
    if top is None:
        return None
    now = standing(top)
    if asked is None:
        named = [name for name, _verbs, _what in now if name not in SEQUENCES]
    else:
        named = [name for name, verbs, _what in now if asked in verbs]
    if not named:
        return None
    own = set()
    for name, _verbs, what in now:
        got = brought(top, name, what)
        if got is None:
            return None
        own |= got
    held = staged.lines(staged.git(top, "diff", "--cached", "--name-only", "--no-renames"))
    extra = [f for f in held if f not in own]
    if not extra:
        return False
    name = named[0]
    how = (
        "`--continue` takes no pathspec" if name in SEQUENCES
        else "A " + name + " takes no pathspec"
    )
    apart = (
        "; or commit them first by their paths, which git takes while a " + name + " stands "
        "(`git commit -- <paths>`, `--amend` to fold them into the commit an `edit` stopped at)"
        if name in SEQUENCES else ""
    )
    return (
        "refused: concluding this " + name + " commits what the index holds, and it holds "
        + str(len(extra)) + " file" + ("s" if len(extra) > 1 else "") + " the " + name
        + " did not bring: " + ", ".join(extra[:12]) + ("" if len(extra) <= 12 else ", ...")
        + ". " + how + ", so the index is its only limit: unstage them "
        "(`git restore --staged -- <paths>`), conclude, and commit them apart" + apart + ". "
        + CONTRACT + " (CL-070, defects 403 and 487)"
    )
