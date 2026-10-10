#!/usr/bin/env python3
"""What a commit would carry, read from the tree it runs in.

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

**And what the commit carries is judged, not the index's list** (defect 515,
2026-10-09). `git commit -- <paths>`, the commit the hard stops prescribe,
takes each named file as the working tree holds it, staged or not, so a module
over its ceiling and never staged passed unjudged (lane b15-emit, about 04:15
that day); and a commit taking the whole index, a merge's conclusion, takes
the staged version, so a broken file staged and then repaired in the working
tree alone was judged by the repair and committed broken (measured 2026-10-09).
So the files a pathspec names that differ from HEAD are judged as the working
tree holds them, and every other staged file as the index holds it.
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


def named_hero_files(where, paths):
    """The `.hero` files a commit's pathspec `paths`, read in `where`, takes
    from the working tree: each file git knows that it matches and that the
    working tree adds or changes against HEAD, staged or not, by its path
    from the root (defect 515). With no HEAD yet, every such file git knows."""
    if not paths:
        return []
    out = git(where, "diff", "-z", "--name-only", "--no-renames", "--no-relative", "--diff-filter=AM", "HEAD", "--", *paths)
    if out is None:
        out = git(where, "ls-files", "-z", "--full-name", "--", *paths)
    if out is None:
        return []
    return [rel for rel in out.split("\0") if rel.endswith(".hero")]


def unlike_their_index(top):
    """The paths whose working tree differs from what the index holds."""
    out = git(top, "diff", "-z", "--name-only", "--no-renames", "--no-relative")
    return set() if out is None else {rel for rel in out.split("\0") if rel}


def index_text(top, rel):
    """What the index holds as `rel`, or None (an unmerged path, or git
    cannot say)."""
    try:
        done = subprocess.run(["git", "show", ":" + rel], capture_output=True, timeout=30, cwd=top)
    except (OSError, subprocess.SubprocessError):
        return None
    return done.stdout if done.returncode == 0 else None


def offences(where, paths=()):
    """The `.hero` files a commit run in `where` carries that are not
    canonical or are over their ceiling, one line each: the staged ones, and
    with a pathspec `paths` the ones it names. A file a pathspec names is
    judged as the working tree holds it, as git takes it; any other staged
    file as the index holds it, its text handed to `fmt` on its standard
    input where the working tree holds another (defect 515).

    A refusal by a compiler older than its tree is told as that age rather than
    as the file's fault (defect 384's shape, here at the commit): the commit is
    still refused, since nothing could judge the file, and the line says what
    to rebuild.

    **A golden case whose marks claim diagnostics is judged by its marks, not
    by `fmt`** (defect 334, 2026-10-07): `fmt` refuses it on purpose. Every
    such case a narrowed `annotations` run judges is asked of that suite in ONE
    run (`marks.py`, which has the run's price), and a case it fails is an
    offence with the suite's words; one under the suite's run roots, whose
    marks the whole suite asks by compiling each program (a narrowed run asks a
    group only for a word naming it, defect 485), is left to the batch's gate, as the write-time hook leaves it. A marked case elsewhere, a
    `run/` or `emit/` program, keeps `fmt`'s verdict: such a program parses.
    And a run that cannot answer gives no opinion: this guard never refuses
    over its own inability to judge, so a marked case whose index holds another
    version than its working tree, which the suite would not read, is left to
    the batch's gate.

    **Whatever `fmt` answers, since 2026-10-10** (defect 581): a marked case
    of the suite's `DIRECTORIES` that parses and is not canonical on purpose,
    as defect 576's two are, was refused *not canonical* with its marks green,
    and a lane merged another lane at an older commit to get past it. Such a
    case is asked of its marks as one `fmt` refuses is (`marks.held_to_marks`);
    a case with no mark is held to `fmt` as before, and so is a marked program
    of `run/` or `emit/`, which the `canonical` suite reads.
    """
    top = toplevel(where)
    if top is None:
        return []
    compiler = trees.compiler_of(top)
    can_fmt = trees.runnable(compiler)
    found = []
    marked = []
    named = named_hero_files(where, list(paths))
    others = [rel for rel in staged_hero_files(top) if rel not in named]
    moved = unlike_their_index(top) if others else set()
    for rel in named + others:
        path = os.path.join(top, rel)
        held = index_text(top, rel) if rel in moved and rel not in named else None
        if held is None and not os.path.isfile(path):
            continue
        how = "" if held is None else " as the index holds it"
        if can_fmt:
            try:
                if held is None:
                    run = subprocess.run([compiler, "fmt", rel], capture_output=True, timeout=120, cwd=top)
                    with open(path, "rb") as handle:
                        text = handle.read()
                else:
                    run = subprocess.run([compiler, "fmt", "/dev/stdin"], input=held, capture_output=True, timeout=120, cwd=top)
                    text = held
            except (OSError, subprocess.SubprocessError):
                run, text = None, b""
            said = None
            shown = text.decode("utf-8", errors="replace")
            if run is not None and marks.held_to_marks(top, rel, shown):
                # Its marks are its judge whatever `fmt` answered (defect
                # 581): asked of the suite where it reads the case as the
                # commit carries it, left to the gate where the index holds
                # another version. One with no `.expected` beside it is a
                # half pair the harness refuses, and keeps `fmt`'s verdict.
                if held is None and marks.judged_narrowed(top, rel):
                    marked.append(rel)
                    continue
                if held is not None:
                    continue
            if run is not None and run.returncode != 0 and rel.startswith("tests/golden/") and marks.MARK.search(shown):
                if held is not None or marks.in_run_roots(top, rel):
                    continue
            # A probe fixture that parses and is not canonical on purpose,
            # under a run root the `canonical` suite does not read (defect
            # 603): no suite holds it to `fmt`, so neither does the guard.
            if run is not None and run.returncode == 0 and marks.in_run_roots(top, rel) and not marks.canonical_reads(top, rel):
                continue
            if run is not None and run.returncode != 0:
                said = rel + " does not parse" + how
            elif run is not None and run.stdout != text:
                said = rel + " is not canonical" + how + " (`heroes fmt " + rel + " --in-place`)"
            if said is not None:
                found.append(aged(said, rel, compiler, top, path))
        if held is None:
            over = ceiling.verdict(top, rel)
        else:
            over = ceiling.verdict(top, rel, text=held.decode("utf-8", errors="replace"), counted="the version the index holds")
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
