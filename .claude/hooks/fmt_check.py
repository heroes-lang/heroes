#!/usr/bin/env python3
"""PostToolUse on Write and Edit: what a hook can see on the touched `.hero` file.

Why this exists. Measured over one session on 2026-09-06, the full net ran eight
times and four runs found something; two of those four were a file written and
not passed through `heroes fmt`, which costs nothing to prevent and thirteen
minutes to discover (docs/records/contract/case-law.md CL-063). CLAUDE.md
§ Verification says to format at the moment of writing, and this is what
performs it.

**Widened on 2026-09-29 by author instruction (CL-079): the suite is the last
judge, never the first finder.** Until that day this hook said only whether a
`.hero` file was canonical, and a file that did not parse was left to "the
compiler's message on the next build", which by then was a suite. Measured that
day on the trunk's compiler: `heroes fmt` on a module 0.02 s, `heroes check` on
a root module 0.18 to 0.48 s, `heroes check selfhost/main.hero` 5.8 s. So the
hook now says, on the one file written and nothing else:

- a `.hero` that does not PARSE, with the compiler's own message (exit 2);
- a `.hero` that is not canonical, as before (exit 2);
- for a `selfhost/` module, whether the WHOLE compiler still checks, names and
  types, `heroes check selfhost/main.hero`: a nested module cannot be checked
  alone (its `use` lines resolve from its own directory; 251 of 308 modules
  are nested) and a root module checked alone does not see its callers;
- for a `tests/harness/` module, `heroes check <file>`, since that package is
  flat and a module checks alone;
- for a `selfhost/` module, its line ceiling by the mirror in `ceiling.py`,
  the `layout` suite staying the judge;
- for a `tests/golden/` case, its `#~` marks against its `.expected`, asked of
  the `annotations` suite itself narrowed to the case (defect 286, 2026-10-05),
  on a write of either file. `.claude/rules/verification.md` listed this check
  under layer 0 from 2026-09-29 and no hook performed it, so a case whose marks
  and expectation disagreed waited for the suite. It asks the judge rather than
  mirroring it: the comparison is `mark_readers.hero`'s, and a second reader in
  Python would be a copy that can disagree with it in silence. Measured that
  day, beside three lanes' work so only a size: 24.83 s with the harness built
  cold, 6.82 and 7.02 s warm. The suite decides which directories it sweeps: a
  case it does not select (exit 2, *no case ... matches*) gets no opinion, nor
  does a case whose `.hero` or `.expected` is not written yet.

It NOTICES and does not rewrite. A suite reading the tree owns the tree until it
exits (CL-025), and a hook that edited a file under a running suite would be the
same defect this project already paid for once. Exit 2 puts the reason in front
of the assistant, which then repairs the file itself.

**The file is judged in the tree it stands in, since 2026-10-07** (defect 254,
`trees.py`): the nearest directory above it holding `seed/heroes.c` and a
`.git` entry, so a lane under `.claude/worktrees/<lane>/` is judged by its own
compiler with its paths read from its own root, whatever the session's
directory. Until that day the session's directory was the tree: a lane's module
written by a session on the trunk was formatted by the trunk's compiler and
read as `.claude/worktrees/<lane>/selfhost/...`, which is not `selfhost/`, so
neither the whole compiler's check nor the ceiling was asked of it. A file
outside every tree is still a program: the session's tree's compiler says
whether it parses and is canonical, and nothing a tree owns is asked of it.

Contract with the harness: the tool call arrives as JSON on stdin and the path
is `tool_input.file_path`. A missing file, or anything unexpected, means exit 0
and no opinion: this hook never blocks work over its own inability to run. A
tree with no compiler built is the one absence it names (exit 2, which after a
write is a notice and blocks nothing): its file was not judged, and silence
would read as a pass.
"""

import json
import os
import re
import subprocess
import sys

import ceiling
import trees

LAYER = "(layer 0, `.claude/rules/verification.md` § A suite is the last judge, CL-079)"


class Place:
    """Where a written file stands: its tree, the compiler that judges it, its
    path as that tree reads it, and its name as the session should read it."""

    def __init__(self, path, session):
        self.path = path
        self.tree = trees.tree_of(path)
        session_tree = trees.tree_of(session)
        self.home = self.tree or session_tree or os.path.abspath(session)
        self.compiler = trees.compiler_of(self.home)
        # None for a file outside every tree, so no question a tree owns
        # (`selfhost/`, `tests/harness/`, `tests/golden/`) is asked of it.
        self.rel = os.path.relpath(path, self.tree) if self.tree else None
        # Short where the tree is the session's own, whole where it is not, so
        # a lane's file is never read as the trunk's.
        self.shown = self.rel if self.tree is not None and self.tree == session_tree else path

    def under(self, prefix):
        return self.rel is not None and self.rel.startswith(prefix)


def run(compiler, args, root):
    try:
        return subprocess.run([compiler] + args, capture_output=True, timeout=120, cwd=root)
    except (OSError, subprocess.SubprocessError):
        return None


def head(text, lines=40):
    rows = text.decode("utf-8", errors="replace").rstrip("\n").split("\n")
    if len(rows) > lines:
        rows = rows[:lines] + ["... (" + str(len(rows) - lines) + " more lines)"]
    return "\n".join(rows)


def main():
    try:
        payload = json.load(sys.stdin)
    except (json.JSONDecodeError, ValueError):
        return 0

    path = (payload.get("tool_input") or {}).get("file_path")
    if not isinstance(path, str) or not (path.endswith(".hero") or path.endswith(".expected")):
        return 0

    session = payload.get("cwd") or os.getcwd()
    path = os.path.abspath(os.path.join(session, path))
    if not os.path.isfile(path):
        return 0
    place = Place(path, session)
    if not trees.runnable(place.compiler):
        if place.tree is None:
            return 0
        print(
            place.shown + " was not judged: its tree, " + place.tree + ", has no compiler "
            "built, so nothing asked whether it parses, is canonical or checks. Build it "
            "there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.\n" + LAYER,
            file=sys.stderr,
        )
        return 2
    compiler = place.compiler

    # An expectation written is judged against its case's marks, and nothing
    # else here reads it.
    if path.endswith(".expected"):
        return marks_against_expectation(compiler, place)

    # 1. The file parses, and it is canonical. `heroes fmt <file>` prints the
    #    canonical form and writes nothing; a file that does not parse makes it
    #    exit non-zero with the diagnostic on stderr.
    fmt = run(compiler, ["fmt", path], place.home)
    if fmt is None:
        return 0
    if fmt.returncode != 0 and place.under("tests/golden/") and has_marks(path):
        # A golden case whose `#~` marks claim diagnostics is refused by `fmt`
        # on purpose: its diagnostics are its subject (defect 272, 2026-10-05).
        # This hook said *does not parse* of every such case, a false alarm
        # that teaches its reader to pass it by; the marks are judged against
        # the expectation instead, which is the question the case asks.
        return marks_against_expectation(compiler, place)
    if fmt.returncode != 0:
        print(
            place.shown + " does not parse; the compiler says:\n" + head(fmt.stderr) + "\n" + LAYER,
            file=sys.stderr,
        )
        return 2
    try:
        with open(path, "rb") as handle:
            on_disk = handle.read()
    except OSError:
        return 0
    if fmt.stdout != on_disk:
        print(
            place.shown + " is not canonical: run `heroes fmt " + path + " --in-place`.\n"
            "The `canonical` suite fails on it otherwise, and design.md §4.15 "
            "rests on a textual difference meaning a semantic one "
            "(CLAUDE.md § Verification).",
            file=sys.stderr,
        )
        return 2

    # 2. Names and types: the whole compiler for one of its modules, the one
    #    file for a harness module, each in the tree the file stands in.
    if place.under("selfhost/"):
        check = run(compiler, ["check", "selfhost/main.hero"], place.tree)
        subject = "selfhost/main.hero, with " + place.shown + " as written,"
    elif place.under("tests/harness/"):
        check = run(compiler, ["check", place.rel], place.tree)
        subject = place.shown
    else:
        check = None
        subject = ""
    if check is not None and check.returncode != 0:
        print(
            subject + " does not check; the compiler says:\n" + head(check.stderr) + "\n" + LAYER,
            file=sys.stderr,
        )
        return 2

    # 3. The line ceiling, by the mirror; the `layout` suite is the judge.
    if place.rel is not None:
        over = ceiling.verdict(place.tree, place.rel)
        if over is not None:
            print(over, file=sys.stderr)
            return 2

    # 4. A golden case's marks against its expectation.
    return marks_against_expectation(compiler, place)


def has_marks(path):
    """Whether the case claims a diagnostic with a `#~` or `#~v` mark. A mark
    quoted in prose reads as one here, and costs only the marks' question being
    asked of the suite, which reads marks exactly (`mark_readers.hero`)."""
    try:
        with open(path, "rb") as handle:
            text = handle.read().decode("utf-8", errors="replace")
    except OSError:
        return False
    return re.search(r"#~v? [a-z_]+", text) is not None


def marks_against_expectation(compiler, place):
    """The `annotations` suite narrowed to the case `place` names (defect 286),
    run in the case's own tree with that tree's compiler.

    Exit 2 with the suite's own words where it fails THIS case; no opinion
    where the case is not a golden one, has no `.hero` or no `.expected` yet,
    is not one the suite selects, or the run could not be made."""
    if not place.under("tests/golden/"):
        return 0
    root = place.tree
    rel = place.rel
    stem = rel[: rel.rfind(".")]
    if not (os.path.isfile(os.path.join(root, stem + ".hero")) and os.path.isfile(os.path.join(root, stem + ".expected"))):
        return 0
    name = os.path.basename(stem)
    judged = run(compiler, ["run", "tests/harness/main.hero", "--", "./heroes", "annotations", name], root)
    if judged is None or judged.returncode != 1:
        return 0
    said = judged.stdout.decode("utf-8", errors="replace").split("\n")
    failed = "FAIL annotations/" + name
    if failed not in said:
        return 0
    at = said.index(failed)
    rows = [failed]
    for row in said[at + 1:]:
        if row.startswith("FAIL ") or row.startswith("harness:") or not row.startswith("  "):
            break
        rows.append(row)
    print(
        place.shown[: place.shown.rfind(".")] + "'s marks and its expectation disagree; the `annotations` suite says:\n"
        + "\n".join(rows) + "\n"
        "Change whichever side is wrong; if the `.expected` is being rewritten "
        "next, this clears when it is.\n" + LAYER,
        file=sys.stderr,
    )
    return 2


if __name__ == "__main__":
    sys.exit(main())
