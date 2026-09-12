#!/usr/bin/env python3
"""PostToolUse notice on Write and Edit: say when a `.hero` file is not canonical.

Why this exists. Measured over one session on 2026-09-06, the full net ran eight
times and four runs found something; two of those four were a file written and
not passed through `heroes fmt`, which costs nothing to prevent and thirteen
minutes to discover (docs/records/contract/case-law.md CL-063). CLAUDE.md § Verification
says to format at the moment of writing, and this is what performs it.

It NOTICES and does not rewrite. A suite reading the tree owns the tree until it
exits (CL-025), and a hook that edited a file under a running suite would be the
same defect this project already paid for once. Exit 2 puts the reason in front
of the assistant, which then runs `heroes fmt <file> --in-place` itself.

Contract with the harness: the tool call arrives as JSON on stdin and the path
is `tool_input.file_path`. Missing compiler, missing file, or anything
unexpected means exit 0 and no opinion: this hook never blocks work over its own
inability to run.
"""

import json
import os
import subprocess
import sys


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

    try:
        # `heroes fmt <file>` prints the canonical form and writes nothing.
        run = subprocess.run(
            [compiler, "fmt", path],
            capture_output=True,
            timeout=120,
            cwd=root,
        )
    except (OSError, subprocess.SubprocessError):
        return 0

    if run.returncode != 0:
        # The file does not parse. That is the compiler's message to give, on
        # the next build, with a span. Not this hook's business.
        return 0

    try:
        with open(path, "rb") as handle:
            on_disk = handle.read()
    except OSError:
        return 0

    if run.stdout == on_disk:
        return 0

    print(
        path + " is not canonical: run `heroes fmt " + path + " --in-place`.\n"
        "The `canonical` suite fails on it otherwise, and design.md §4.15 "
        "rests on a textual difference meaning a semantic one "
        "(CLAUDE.md § Verification).",
        file=sys.stderr,
    )
    return 2


if __name__ == "__main__":
    sys.exit(main())
