#!/usr/bin/env python3
"""A golden case's `#~` marks, judged by the `annotations` suite itself.

Why this exists. A golden case whose marks claim diagnostics is refused by
`heroes fmt` on purpose: its diagnostics are its subject. The write-time hook
learned on 2026-10-05 to judge such a case by its marks rather than call it a
parse error (defects 272 and 286); the commit guard did not, and refused the
eight cases of defects 324 and 325, each holding a lexer error on purpose, with
*does not parse* when batch 10's merge was concluded with `git commit -- <paths>`
(defect 334, 2026-10-04). Both hooks now ask the one judge through this
module, so there is one way to ask it and one reading of its answer.

THE JUDGE, NOT A MIRROR. The comparison is `tests/harness/mark_readers.hero`'s;
a second reader in Python would be a copy that can disagree with it in silence.
A narrowed `annotations` run compares the marks of each case of its
`DIRECTORIES` with that case's `.expected` and asks the run roots nothing, so
the directories it judges narrowed are read from that constant, in the tree
judged, never written down here.

ITS COST, measured 2026-10-07 on this Mac beside twelve other lanes: 80.1
billion instructions of `heroes run` and 13.0 s of user time for one case or
for five, 39.8 s for the whole suite's 846; the harness's own build and start
are the price, and a case adds a text comparison. So the commit guard asks once
for every such case it holds, narrowed to a word all their names hold. The
same runs read 73 to 79 s of wall time beside that load, and a fresh tree
building its harness cold passed 120 s.

SO THE RUN IS BOUNDED, and ended as a shell ends a job. `LIMIT` keeps a hook's
answer its own: Claude Code ends a hook that runs past its timeout, and the
default this repository's settings leave in force was documented as a minute,
which is a question rather than a premise, unmeasured on the installed
version. Past it the run is asked to end with SIGTERM, which `heroes run`
passes to every child it started (defect 425's repair), then SIGKILL after
`GRACE`; until 2026-10-07 the write hook's run was killed outright at 120 s,
and SIGKILL is the one signal `heroes` cannot pass on, so the clang processes
building the harness ran on with no parent (seen that day, in a scratch tree).
A run that did not end in time gives no opinion.

RAISED 2026-10-07, 45 s to 100 s, with both hooks' `timeout` set to 120 in
`.claude/settings.json` (lane b14-hooks' recommendation, taken by the batch's
rule): under thirteen lanes' load a warm narrowed harness run took 45 to 79 s
of wall time, quiet about 15 s, so at 45 s the questions of defects 334 and
215 gave no opinion under exactly the load lanes work in. The settings' 120
is written rather than left to the installed version's default.
"""

import os
import re
import signal
import subprocess

SUITE = os.path.join("tests", "harness", "suite_annotations.hero")
MARK = re.compile(r"#~v? [a-z_]+")
ROW = re.compile(r'^\s*"(tests/golden/[^"\s]+)"\s*$')
LIMIT = 100.0
GRACE = 5.0


def has_marks(path):
    """Whether the case claims a diagnostic with a `#~` or `#~v` mark. A mark
    quoted in prose reads as one here, and costs only the marks' question being
    asked of the suite, which reads marks exactly (`mark_readers.hero`)."""
    try:
        with open(path, "rb") as handle:
            text = handle.read().decode("utf-8", errors="replace")
    except OSError:
        return False
    return MARK.search(text) is not None


def constant(tree, name):
    """The rows of the `[str]` constant `name` of the annotations suite in
    `tree`, or None when the suite or the constant is not there."""
    try:
        with open(os.path.join(tree, SUITE), encoding="utf-8") as handle:
            rows = handle.read().split("\n")
    except OSError:
        return None
    out, inside = [], False
    for row in rows:
        if row.startswith("constant " + name + ":"):
            inside = True
            continue
        if inside:
            if row.strip() == "]":
                return out
            found = ROW.match(row)
            if found:
                out.append(found.group(1))
    return None


def judged_narrowed(tree, rel):
    """Whether a narrowed `annotations` run judges the case `rel`: a `.hero`
    in one of the suite's `DIRECTORIES`, with its `.expected` beside it."""
    directories = constant(tree, "DIRECTORIES")
    if not directories or not rel.endswith(".hero"):
        return False
    if os.path.dirname(rel) not in directories:
        return False
    return os.path.isfile(os.path.join(tree, rel[: -len(".hero")] + ".expected"))


def in_run_roots(tree, rel):
    """Whether `rel` is under the suite's `RUN_ROOTS`, whose marks only the
    whole suite asks, each program compiled."""
    roots = constant(tree, "RUN_ROOTS") or []
    return any(rel.startswith(root + "/") for root in roots)


def word_for(names):
    """A word every one of `names` holds, the longest, for the harness's third
    word: their common prefix where they have one."""
    if not names:
        return ""
    prefix = os.path.commonprefix(names)
    if prefix:
        return prefix
    shortest = min(names, key=len)
    for size in range(len(shortest), 0, -1):
        for at in range(0, len(shortest) - size + 1):
            piece = shortest[at:at + size]
            if all(piece in name for name in names):
                return piece
    return ""


def bounded(args, cwd, limit=LIMIT):
    """`args` run in `cwd` in a session of its own, as a CompletedProcess, or
    None when it could not start or did not end within `limit` seconds, in
    which case its whole group was asked to end, then made to."""
    try:
        child = subprocess.Popen(
            args, cwd=cwd, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, start_new_session=True,
        )
    except OSError:
        return None
    try:
        out, err = child.communicate(timeout=limit)
    except subprocess.TimeoutExpired:
        for sig, wait in ((signal.SIGTERM, GRACE), (signal.SIGKILL, None)):
            try:
                os.killpg(child.pid, sig)
            except OSError:
                pass
            try:
                child.communicate(timeout=wait)
                break
            except subprocess.TimeoutExpired:
                continue
        return None
    return subprocess.CompletedProcess(args, child.returncode, out, err)


def disagreements(compiler, tree, names, limit=LIMIT):
    """The `annotations` suite narrowed to a word all `names` hold, run in
    `tree` with `compiler`: {name: the suite's rows} for each case of `names`
    it fails, {} when it fails none, None when it could not answer (no word,
    a run that could not be made, or one that selected nothing)."""
    word = word_for(names)
    if not word:
        return None
    done = bounded([compiler, "run", "tests/harness/main.hero", "--", "./heroes", "annotations", word], tree, limit)
    if done is None or done.returncode not in (0, 1):
        return None
    said = done.stdout.decode("utf-8", errors="replace").split("\n")
    out = {}
    for name in names:
        failed = "FAIL annotations/" + name
        if failed not in said:
            continue
        at = said.index(failed)
        rows = [failed]
        for row in said[at + 1:]:
            if row.startswith("FAIL ") or row.startswith("harness:") or not row.startswith("  "):
                break
            rows.append(row)
        out[name] = rows
    return out
