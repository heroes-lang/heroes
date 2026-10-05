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

Contract with the harness: the tool call arrives as JSON on stdin and the path
is `tool_input.file_path`. Missing compiler, missing file, or anything
unexpected means exit 0 and no opinion: this hook never blocks work over its own
inability to run.
"""

import json
import os
import re
import subprocess
import sys

import ceiling

LAYER = "(layer 0, `.claude/rules/verification.md` § A suite is the last judge, CL-079)"


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

    root = payload.get("cwd") or os.getcwd()
    compiler = os.path.join(root, "heroes")
    if not (os.path.isfile(compiler) and os.access(compiler, os.X_OK)):
        return 0
    if not os.path.isfile(path):
        return 0
    rel = os.path.relpath(os.path.abspath(path), root)

    # An expectation written is judged against its case's marks, and nothing
    # else here reads it.
    if path.endswith(".expected"):
        return marks_against_expectation(compiler, root, rel)

    # 1. The file parses, and it is canonical. `heroes fmt <file>` prints the
    #    canonical form and writes nothing; a file that does not parse makes it
    #    exit non-zero with the diagnostic on stderr.
    fmt = run(compiler, ["fmt", path], root)
    if fmt is None:
        return 0
    if fmt.returncode != 0 and rel.startswith("tests/golden/") and has_marks(path):
        # A golden case whose `#~` marks claim diagnostics is refused by `fmt`
        # on purpose: its diagnostics are its subject (defect 272, 2026-10-05).
        # This hook said *does not parse* of every such case, a false alarm
        # that teaches its reader to pass it by; the marks are judged against
        # the expectation instead, which is the question the case asks.
        return marks_against_expectation(compiler, root, rel)
    if fmt.returncode != 0:
        print(
            rel + " does not parse; the compiler says:\n" + head(fmt.stderr) + "\n" + LAYER,
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
            rel + " is not canonical: run `heroes fmt " + path + " --in-place`.\n"
            "The `canonical` suite fails on it otherwise, and design.md §4.15 "
            "rests on a textual difference meaning a semantic one "
            "(CLAUDE.md § Verification).",
            file=sys.stderr,
        )
        return 2

    # 2. Names and types: the whole compiler for one of its modules, the one
    #    file for a harness module.
    if rel.startswith("selfhost/"):
        check = run(compiler, ["check", "selfhost/main.hero"], root)
        subject = "selfhost/main.hero, with " + rel + " as written,"
    elif rel.startswith("tests/harness/"):
        check = run(compiler, ["check", rel], root)
        subject = rel
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
    over = ceiling.verdict(root, rel)
    if over is not None:
        print(over, file=sys.stderr)
        return 2

    # 4. A golden case's marks against its expectation.
    return marks_against_expectation(compiler, root, rel)


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


def marks_against_expectation(compiler, root, rel):
    """The `annotations` suite narrowed to the case `rel` names (defect 286).

    Exit 2 with the suite's own words where it fails THIS case; no opinion
    where the case is not a golden one, has no `.hero` or no `.expected` yet,
    is not one the suite selects, or the run could not be made."""
    if not rel.startswith("tests/golden/"):
        return 0
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
        stem + "'s marks and its expectation disagree; the `annotations` suite says:\n"
        + "\n".join(rows) + "\n"
        "Change whichever side is wrong; if the `.expected` is being rewritten "
        "next, this clears when it is.\n" + LAYER,
        file=sys.stderr,
    )
    return 2


if __name__ == "__main__":
    sys.exit(main())
