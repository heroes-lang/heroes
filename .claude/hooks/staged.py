#!/usr/bin/env python3
"""What a commit would carry, read from the index of the tree it runs in.

Why this exists. The commit guard asks, of every staged `.hero` file, what the
write-time hook would have asked of it: a file that reached the tree some other
way, a merge or a `git mv`, is seen here, the last moment before a suite would
have to find it (CL-079). Until 2026-10-07 it asked the SESSION's directory, so
a commit made in a lane's worktree was judged by the trunk's index: lane
b9-harness's `a3fb46d5` staged three cases `heroes fmt` refuses and the guard
accepted it, while the trunk's own staged files could refuse a lane's commit
that held none of them (defect 287). The index is now the one of the directory
the commit runs in, `git -C` or the command's own `cd` (`guard_bash.placed`),
its paths read from that tree's root, and judged by that tree's compiler.
"""

import os
import subprocess

import ceiling
import marks
import trees


def git(where, *args):
    """`git <args>` run in `where`, its stdout, or None when it fails."""
    try:
        done = subprocess.run(["git"] + list(args), capture_output=True, timeout=30, cwd=where, text=True)
    except (OSError, subprocess.SubprocessError):
        return None
    if done.returncode != 0:
        return None
    return done.stdout


def toplevel(where):
    """The root of the worktree `where` stands in, or None outside one."""
    if where is None or not os.path.isdir(where):
        return None
    out = git(where, "rev-parse", "--show-toplevel")
    if out is None or not out.strip():
        return None
    return out.strip()


def lines(text):
    return [line.strip() for line in (text or "").split("\n") if line.strip()]


def staged_hero_files(top):
    """The `.hero` files the index of the worktree rooted at `top` adds or
    changes, by their paths from that root, whichever directory asks."""
    return lines(git(top, "diff", "--cached", "--name-only", "--diff-filter=AM", "--", ":/*.hero"))


def offences(where):
    """The staged `.hero` files of the tree `where` stands in that are not
    canonical or are over their ceiling, one line each.

    A refusal by a compiler older than its tree is told as that age rather than
    as the file's fault (defect 384's shape, here at the commit): the commit is
    still refused, since nothing could judge the file, and the line says what
    to rebuild.

    **A golden case whose marks claim diagnostics is judged by its marks, not
    by `fmt`** (defect 334, 2026-10-07): `fmt` refuses it on purpose. Every
    such case a narrowed `annotations` run judges is asked of that suite in ONE
    run (`marks.py`, which has the run's price), and a case it fails is an
    offence with the suite's words; one under the suite's run roots, whose
    marks only the whole suite asks by compiling each program, is left to the
    batch's gate, as the write-time hook leaves it. A marked case elsewhere, a
    `run/` or `emit/` program, keeps `fmt`'s verdict: such a program parses.
    And a run that cannot answer gives no opinion: this guard never refuses
    over its own inability to judge.
    """
    top = toplevel(where)
    if top is None:
        return []
    compiler = trees.compiler_of(top)
    can_fmt = trees.runnable(compiler)
    found = []
    marked = []
    for rel in staged_hero_files(top):
        path = os.path.join(top, rel)
        if not os.path.isfile(path):
            continue
        if can_fmt:
            try:
                run = subprocess.run([compiler, "fmt", rel], capture_output=True, timeout=120, cwd=top)
                with open(path, "rb") as handle:
                    on_disk = handle.read()
            except (OSError, subprocess.SubprocessError):
                run = None
            said = None
            if run is not None and run.returncode != 0 and rel.startswith("tests/golden/") and marks.has_marks(path):
                if marks.judged_narrowed(top, rel):
                    marked.append(rel)
                    continue
                if marks.in_run_roots(top, rel):
                    continue
            if run is not None and run.returncode != 0:
                said = rel + " does not parse"
            elif run is not None and run.stdout != on_disk:
                said = rel + " is not canonical (`heroes fmt " + rel + " --in-place`)"
            if said is not None:
                found.append(aged(said, rel, compiler, top, path))
        over = ceiling.verdict(top, rel)
        if over is not None:
            found.append(over.split("\n")[0])
    if marked:
        names = {os.path.basename(rel)[: -len(".hero")]: rel for rel in marked}
        told = marks.disagreements(compiler, top, sorted(names))
        for name, rows in sorted((told or {}).items()):
            found.append(
                names[name] + "'s marks and its expectation disagree; the `annotations` suite says:\n      "
                + "\n      ".join(rows)
            )
    return found


def aged(said, rel, compiler, top, path):
    """`said`, or the compiler's age where it is older than a source of its
    tree but the file itself."""
    old = trees.older_than_tree(compiler, top, besides=path)
    if old is None:
        return said
    where, built, wrote = old
    return (
        rel + " was refused by " + compiler + ", built at " + trees.stamp(built)
        + ", older than " + where + " written at " + trees.stamp(wrote)
        + ": rebuild it (" + trees.rebuild_hint(where) + ") and commit again"
    )
