#!/usr/bin/env python3
"""A commit's message read for the characters its reader cannot see.

Why this exists. The body of `fe3f788e` holds one raw U+200B: the tool that
wrote its message file decoded a written `\\u200b`, defect 356's hazard, and the
`unseen` suite that defect added reads files, never a commit's message, which
is outward-facing on a public repository and append-only (defect 378, counted
by the coordinator on 2026-10-06). So the commit guard reads the message a
`git commit`, a `git merge` or a `git tag` is given, `-m` and `-F` in each of
their spellings, `-F -` and a `$(cat <<EOF ...)` read through the command's
heredocs, and refuses a character the list refuses, by its code point, its
line and its column.

THE LIST IS THE SUITE'S. `REFUSED` in `tests/harness/suite_unseen.hero`, the
runs of code points a document refuses, read from that file in the tree the
command stands in, as `ceiling.py` reads the layout suite's `DECIDED`: one
list, so a message and a document are refused the same characters. A tree
whose suite holds no such list gives no opinion.

What it cannot see, said so a pass is not read as more: a message an editor
writes (no `-m` or `-F`), the text of `-t <template>`, a message reused with
`-C` or `-c`, and an ANSI-C `$'...'` quote, whose escapes the shell decodes
after this guard has read the words.
"""

import os
import re

SUITE = os.path.join("tests", "harness", "suite_unseen.hero")
RUN = re.compile(r'^\s*"(\d+) (\d+)"\s*$')
# The flags that carry a message, and those that name a file holding one.
TEXT = frozenset({"-m", "--message"})
FILE = frozenset({"-F", "--file"})
VERBS = ("commit", "merge", "tag")


def runs(tree):
    """`REFUSED`'s runs in the tree, as (first, last) pairs, or None."""
    if tree is None:
        return None
    try:
        with open(os.path.join(tree, SUITE), encoding="utf-8") as handle:
            rows = handle.read().split("\n")
    except OSError:
        return None
    out, inside = [], False
    for row in rows:
        if row.startswith("constant REFUSED:"):
            inside = True
            continue
        if inside:
            if row.strip() == "]":
                return out or None
            found = RUN.match(row)
            if found:
                out.append((int(found.group(1)), int(found.group(2))))
    return None


def named(code):
    return "U+%04X" % code


def found_in(text, spans):
    """Each character of `text` the runs refuse, as (line, column, code),
    the column counted in characters from 1."""
    out = []
    for line, row in enumerate(text.split("\n"), start=1):
        for column, char in enumerate(row, start=1):
            code = ord(char)
            if any(first <= code <= last for first, last in spans):
                out.append((line, column, code))
    return out


def messages(rest, where):
    """The messages the words after `git <verb>` give, as (where it came
    from, its text, or None where it is read from stdin)."""
    out = []
    i = 0
    while i < len(rest):
        word = rest[i]
        if word == "--":
            break
        flag, value = None, None
        if word in TEXT or word in FILE:
            flag = word
            value = rest[i + 1] if i + 1 < len(rest) else None
            i += 1
        elif word.startswith("--message=") or word.startswith("--file="):
            flag, value = word.split("=", 1)
        elif len(word) > 2 and word[:2] in ("-m", "-F") and not word.startswith("--"):
            flag, value = word[:2], word[2:]
        i += 1
        if flag is None or value is None:
            continue
        if flag in TEXT or flag == "-m":
            out.append(("the `-m` message", value))
        elif value == "-":
            out.append(("the message read from stdin", None))
        else:
            path = value if os.path.isabs(value) or where is None else os.path.join(where, value)
            try:
                with open(path, "rb") as handle:
                    text = handle.read().decode("utf-8", errors="replace")
            except OSError:
                continue
            out.append(("`" + value + "`", text))
    return out


def verdict(rest, verb, where, bodies, spans):
    """A refusal for the message `git <verb>` is given in `rest`, or None.
    `bodies` are the command's heredoc bodies, which `-F -` and a command
    substitution in `-m` read."""
    if not spans:
        return None
    told = []
    for source, text in messages(rest, where):
        if text is None:
            texts = bodies
        elif "$(" in text or "`" in text:
            texts = [text] + bodies
        else:
            texts = [text]
        for one in texts:
            for line, column, code in found_in(one, spans):
                told.append(named(code) + " at line " + str(line) + ", column " + str(column) + " of " + source)
    if not told:
        return None
    return (
        "refused: the " + verb + "'s message holds a character its reader cannot see, written as "
        "itself: " + "; ".join(told[:8]) + ("" if len(told) <= 8 else "; and " + str(len(told) - 8) + " more")
        + ". A message is outward-facing and never rewritten: write the character as the text "
        "means, its name, `U+200B`, or the escape a quoted program spells it with. A tool that "
        "decodes a written escape is how they arrive (defects 356 and 378; the list is "
        "`REFUSED` in " + SUITE + ")."
    )
