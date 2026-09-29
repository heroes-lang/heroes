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
  the `layout` suite staying the judge.

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
    if not isinstance(path, str) or not path.endswith(".hero"):
        return 0

    root = payload.get("cwd") or os.getcwd()
    compiler = os.path.join(root, "heroes")
    if not (os.path.isfile(compiler) and os.access(compiler, os.X_OK)):
        return 0
    if not os.path.isfile(path):
        return 0
    rel = os.path.relpath(os.path.abspath(path), root)

    # 1. The file parses, and it is canonical. `heroes fmt <file>` prints the
    #    canonical form and writes nothing; a file that does not parse makes it
    #    exit non-zero with the diagnostic on stderr.
    fmt = run(compiler, ["fmt", path], root)
    if fmt is None:
        return 0
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

    return 0


if __name__ == "__main__":
    sys.exit(main())
