#!/usr/bin/env python3
"""Which tree a written file or a command stands in, and how old its compiler is.

Why this exists. Until 2026-10-07 both hooks took the tree to be the session's
working directory. A session whose directory is the trunk and which writes a
lane's file under `.claude/worktrees/<lane>/` had that file formatted by the
trunk's compiler and named `.claude/worktrees/<lane>/selfhost/...`, which is not
`selfhost/`, so the whole compiler's check never ran for it (defect 254); the
guard judged a lane's harness run with the trunk's compiler against the trunk's
sources, and refused it, while the lane's own compiler was newer than every file
of the lane (defect 348); and a compiler older than its tree said *does not
parse* of a file the tree's language writes, two false sentences, because
nothing asked its age before its verdict was read (defect 384).

A TREE is the nearest directory, from a path upward, that holds
`seed/heroes.c` and a `.git` entry: a directory in a clone, a file in a
worktree. So a lane under `.claude/worktrees/<lane>/` is its own tree, with its
own compiler, `<tree>/heroes`, and its own sources, and a file's path is read
relative to the tree it stands in.

The AGE is asked of the sources the compiler is built from, the seed and every
file under `selfhost/` and `runtime/` (`.claude/rules/verification.md` § The
compiler that judges is a build artifact), with one file left out where the
question is about that file: the file just written is newer than the compiler
on every write, and is the subject being judged rather than the language
judging it.
"""

import os
import time

SEED = os.path.join("seed", "heroes.c")
SOURCES = (SEED, "selfhost", "runtime")
# A build and the write of its newest source can share a second on a
# filesystem that keeps whole seconds; the guard has allowed this since
# 2026-09-29.
SLACK = 1.0


def tree_of(path):
    """The nearest directory at or above `path` holding `seed/heroes.c` and a
    `.git` entry, as an absolute path, or None when no directory does."""
    if not path:
        return None
    here = os.path.abspath(path)
    if not os.path.isdir(here):
        here = os.path.dirname(here)
    while True:
        if os.path.isfile(os.path.join(here, SEED)) and os.path.lexists(os.path.join(here, ".git")):
            return here
        parent = os.path.dirname(here)
        if parent == here:
            return None
        here = parent


def compiler_of(tree):
    """The compiler a tree builds for itself, `<tree>/heroes`."""
    return os.path.join(tree, "heroes")


def runnable(path):
    return os.path.isfile(path) and os.access(path, os.X_OK)


def newest_source(tree, besides=None):
    """The newest mtime among `tree`'s sources but `besides`, and that file's
    path relative to the tree; (0.0, None) when there is none."""
    left_out = os.path.abspath(besides) if besides else None
    newest, where = 0.0, None
    for entry in SOURCES:
        top = os.path.join(tree, entry)
        if os.path.isfile(top):
            candidates = [top]
        elif os.path.isdir(top):
            candidates = []
            for base, _dirs, files in os.walk(top):
                candidates.extend(os.path.join(base, f) for f in files)
        else:
            continue
        for path in candidates:
            if left_out is not None and os.path.abspath(path) == left_out:
                continue
            try:
                stamp = os.path.getmtime(path)
            except OSError:
                continue
            if stamp > newest:
                newest, where = stamp, os.path.relpath(path, tree)
    return newest, where


def older_than_tree(compiler, tree, besides=None):
    """None when `compiler` was built after every source of `tree` but
    `besides`; else (the newest source's path in the tree, when the compiler
    was built, when that source was written), the two as epoch seconds."""
    try:
        built = os.path.getmtime(compiler)
    except OSError:
        return None
    newest, where = newest_source(tree, besides)
    if where is None or newest <= built + SLACK:
        return None
    return where, built, newest


def stamp(seconds):
    """A moment as a reader compares two: the local day and time."""
    return time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(seconds))


def rebuild_hint(where):
    """How to rebuild a compiler older than `where`: from the seed when the
    seed or the runtime moved, from the source when only `selfhost/` did."""
    from_seed = "`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`"
    from_source = "`./heroes build selfhost/main.hero -o heroes`"
    if where.startswith("selfhost"):
        return from_source + " (the source as it stands), or " + from_seed
    return from_seed + ", then " + from_source + " if `selfhost/` moved too"
