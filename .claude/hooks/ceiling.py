#!/usr/bin/env python3
"""The per-file line ceiling, asked at the moment of writing.

Why this exists. CLAUDE.md §11's ceiling (~300 lines of code per module) is
judged by the `layout` suite, and until 2026-09-29 the first thing that told a
session a file had crossed it was that suite, minutes after the write. The
author's rule of that day (CL-079, `.claude/rules/verification.md` § A suite is
the last judge): what a hook can see on the touched file never waits for a
suite. So this asks the question at every write of a `selfhost/` module.

**This is a NOTICE by a mirror, and the suite stays the judge.** `code_lines`
below counts the way `tests/harness/suite_layout.hero`'s `code_lines` counts,
non-blank lines outside `test` blocks, and the per-file numbers come from that
file's `DECIDED` table, read from disk so they cannot drift from it. Calling the
suite itself here was measured at 24 s a write with the cache warm on
2026-09-29, which is a suite and not a hook. If the two counts ever disagree,
the suite's is the one that binds, and this message says so in its last line.
"""

import os
import re

CEILING = 300
LAYOUT = os.path.join("tests", "harness", "suite_layout.hero")
DECIDED_ROW = re.compile(r'^\s*"(selfhost/\S+\.hero) (\d+)"\s*$')


def code_lines(text):
    """Mirror of suite_layout.hero's `code_lines`: lines a reader holds."""
    count = 0
    inside = False
    for row in text.split("\n"):
        if row.startswith('test "'):
            inside = True
            continue
        if inside and row and row[0] not in " \t" and not row.startswith("#"):
            inside = False
        if not inside and row.strip():
            count += 1
    return count


def decided(root):
    """The `DECIDED` table of the layout suite: path -> the ceiling it may not pass."""
    table = {}
    try:
        with open(os.path.join(root, LAYOUT), encoding="utf-8") as handle:
            lines = handle.read().split("\n")
    except OSError:
        return table
    inside = False
    for row in lines:
        if row.startswith("constant DECIDED"):
            inside = True
            continue
        if inside:
            if row.strip() == "]":
                break
            match = DECIDED_ROW.match(row)
            if match:
                table[match.group(1)] = int(match.group(2))
    return table


def verdict(root, rel):
    """A refusal for a `selfhost/` module over its ceiling, or None."""
    if not rel.startswith("selfhost/") or not rel.endswith(".hero"):
        return None
    try:
        with open(os.path.join(root, rel), encoding="utf-8", errors="replace") as handle:
            text = handle.read()
    except OSError:
        return None
    lines = code_lines(text)
    limit = decided(root).get(rel, CEILING)
    if lines <= limit:
        return None
    if limit == CEILING:
        what = "past CLAUDE.md §11's " + str(CEILING) + "; a module crossing the ceiling splits at a seam (`.claude/rules/module-shape.md`)"
    else:
        what = "past the " + str(limit) + " the `layout` suite's DECIDED table holds it to; a file already over the ceiling may not grow"
    return (
        rel + ": " + str(lines) + " lines of code, " + what + ".\n"
        "Counted here the way `tests/harness/suite_layout.hero`'s `code_lines` counts; "
        "that suite is the judge: `heroes run tests/harness/main.hero -- <compiler> layout " + rel + "` "
        "(`.claude/rules/verification.md` § A suite is the last judge, CL-079)."
    )
