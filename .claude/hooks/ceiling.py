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

**The growths and the appends are asked of the suite itself, since 2026-10-07**
(defect 215): a text grown by `+` in a loop of `selfhost/cli/compiling.hero`
was first seen by the whole `layout` at a gate, since this hook did not ask it
and `layout` narrowed to a file could not. What `growth.hero` reads is a walk
of a module's loops and declarations, too much to mirror without a second copy
that can disagree; so `might_grow` is only a NECESSARY condition, a line where
a place is stored back into itself `+` something or `.push(`, the place a
`str` the module declares or lends, or a field (with a `\n` added, for a
`+`), and where it holds, `layout` narrowed to the module is run, bounded as
`marks.py` bounds a harness run; an append is matched by the suite's own
`SLOW_APPENDS`, read from it. Measured that day: 29 of the 470 modules hold
such a line, where 336 hold a place stored back into itself at all (`at @ at +
1` is everywhere), so a write elsewhere costs a scan and no run.
"""

import os
import re

import marks

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


GROWS = re.compile(r"([A-Za-z_][A-Za-z0-9_.]*) @ \1( \+ |\.push\()")


def slow_appends(root):
    """The `SLOW_APPENDS` rows of the layout suite in `root`, read from it."""
    try:
        with open(os.path.join(root, LAYOUT), encoding="utf-8") as handle:
            lines = handle.read().split("\n")
    except OSError:
        return []
    out, inside = [], False
    for row in lines:
        if row.startswith("constant SLOW_APPENDS"):
            inside = True
            continue
        if inside:
            if row.strip() == "]":
                break
            found = re.match(r'^\s*"(.+)"\s*$', row)
            if found:
                out.append(found.group(1))
    return out


def might_grow(text, appends=()):
    """Whether a module may hold a growth `layout/concat` or an append
    `layout/appends` refuses: a superset of `tests/harness/growth.hero`'s
    sites and of `suite_layout.hero`'s slow appends, `appends` being its
    `SLOW_APPENDS` and any list of reports grown through a field, each of
    which stores a place back into itself on one line, and of
    `layout/streams` (defect 483, `tests/harness/stream_writes.hero`): a
    store or a lend whose place runs through a `tokens` field, `STREAM_WRITE`.

    The names the module declares or lends as a `str` are read once, where a
    search of the whole text for each stored line made it quadratic: about
    0.6 billion instructions of the write hook's own on a module of 300 such
    lines, measured 2026-10-08, which defect 493 asks of more writes."""
    texts = None
    for line in text.split("\n"):
        if any(needle in line for needle in appends):
            return True
        if STREAM_WRITE.search(line):
            return True
        for found in GROWS.finditer(line):
            name, how = found.group(1), found.group(2)
            if how == ".push(":
                if "." in name and name.endswith("diagnostics"):
                    return True
            elif "." in name:
                if "\\n" in line[found.start():]:
                    return True
            elif how == " + ":
                if texts is None:
                    texts = set(DECLARED_STR.findall(text))
                if name in texts:
                    return True
    return False


# A line that may write a stream's `tokens` (`layout/streams`, defect 483): a
# `.tokens` in the place of a store, before its ` @ `, or a lend, `@` and a
# place that runs through `.tokens`. It cannot tell a lexer's own state, which
# the suite exempts by the declaration in hand, from a parsed stream's, so a
# lend of `@l.tokens` wakes it too; a line that only READS the field, with an
# `@` elsewhere on it, does not.
STREAM_WRITE = re.compile(r"\.tokens\b[^@]*\s@\s|@[^\s,()][^,()]*\.tokens\b")


# A name declared or lent as a `str`, `name: str` or `@name: str`, after the
# start of the text, a space, a bracket or a comma.
DECLARED_STR = re.compile(r"(?:^|[\s(,])@?([A-Za-z_][A-Za-z0-9_]*): str\b")


def layout_rows(compiler, root, rel, limit):
    """The `layout` suite narrowed to `rel`, run in `root`: the rows of each
    of its failures, [] when it passes, None when it could not answer within
    `limit` seconds."""
    done = marks.bounded([compiler, "run", "tests/harness/main.hero", "--", "./heroes", "layout", rel], root, limit)
    if done is None or done.returncode not in (0, 1):
        return None
    rows, inside = [], False
    for row in done.stdout.decode("utf-8", errors="replace").split("\n"):
        if row.startswith("FAIL layout"):
            inside = True
            rows.append(row)
        elif inside and row.startswith("  ") and not row.startswith("  layout"):
            rows.append(row)
        else:
            inside = False
    return rows


def verdict(root, rel, text=None, counted=None):
    """A refusal for a `selfhost/` module over its ceiling, or None: `text`
    counted where it is given, the file on disk where it is not, and
    `counted`, where given, naming what was counted (defect 493: the write
    hook counts a module's canonical form, which `heroes fmt --in-place` will
    write, since that command runs no hook)."""
    if not rel.startswith("selfhost/") or not rel.endswith(".hero"):
        return None
    if text is None:
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
        rel + ": " + str(lines) + " lines of code" + (" in " + counted if counted else "") + ", " + what + ".\n"
        "Counted here the way `tests/harness/suite_layout.hero`'s `code_lines` counts; "
        "that suite is the judge: `heroes run tests/harness/main.hero -- <compiler> layout " + rel + "` "
        "(`.claude/rules/verification.md` § A suite is the last judge, CL-079)."
    )
