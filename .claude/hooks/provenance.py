#!/usr/bin/env python3
"""What a compiler says it was compiled from, and the tree's own answer,
mirrored (panel 222's R1 and R2, ratified 2026-10-17).

Every program carries sixteen characters beside its `main`, the key of the
modules its root reaches, the library compiled into it and the runtime it
links (`selfhost/emit/provenance.hero`), and `heroes --version` prints the
compiler's own on its second line, `sources <key>`. A compiler is the tree's
when that line is the key of the tree's sources: a modification time asks
about the file, never about what the binary was built from, and the panel
measured both of its errors (a copy built before an edit passed, a compiler
whose sources were rewritten with the same bytes was refused).

THIS FILE IS A MIRROR, AND THE COMPILER IS THE ARBITER. What the tree's key
is, is the judged compiler's own answer, `<compiler> build selfhost/main.hero
--emit-c`, whose `hero_program_sources` line holds it; this reads the same
rows the emitter reads, faster, and `test_hooks.py`'s `MirrorHeldToCompiler`
holds it to a real compiler over every spelling of a `use` line. Where the
two disagree, the mirror may be the one that is wrong, and a refusal says so.

THE ROWS, as `emit/provenance.hero` writes them, sorted, joined by a newline
with one after the last, and keyed again:
  - `module <name> <key>` for each module the root reaches through its `use`
    lines, every path resolved from the root's directory
    (`module/paths.hero`'s `uses_of`, a token `use` and a path of names
    joined by adjacent `/`), `<name>` that path and the root its stem;
  - `library library <key>` for the library text, `LINES` of
    `selfhost/library_source.hero` joined by a newline with one after the
    last: the compiler compiles in its own, so this is the tree's where the
    compiler is the tree's, and a compiler built by one older than its
    library reads one stage behind, as it is;
  - `runtime <key>` for `runtime.c` and every file it names on a line opening
    `#include "` (`cli/runtime_key.hero`), each keyed alone, the keys joined
    and keyed again: `$HEROES_RUNTIME`, else `./runtime`, as the compiler
    looks first.
A key is `cli/digest.hero`'s over the bytes: two sums, bases 131 and 137,
mod 2^31 - 1, eight lower-case hexadecimal digits each. A file that is not
UTF-8 keys otherwise in the compiler, so the mirror gives no answer for it.
"""

import os
import re
import subprocess
from operator import mul

M = 2147483647
CHUNK = 1 << 16
SOURCE_DIR = "selfhost"
ROOT = os.path.join(SOURCE_DIR, "main.hero")
LIBRARY = os.path.join(SOURCE_DIR, "library_source.hero")
USE = re.compile(r"^[ \t]*use[ \t]+([A-Za-z_][A-Za-z0-9_]*(?:/[A-Za-z_][A-Za-z0-9_]*)*)")
ESCAPES = {"n": "\n", "t": "\t", "r": "\r", "\\": "\\", '"': '"', "'": "'"}


class Unmirrored(Exception):
    """The mirror cannot answer for this tree; the compiler can."""


def _powers(base):
    """`base` to the power `CHUNK - 1 - j` for each j, mod M: a chunk's byte
    j weighs `base ** (len - 1 - j)`, the tail of this list."""
    out = [0] * CHUNK
    value = 1
    for j in range(CHUNK - 1, -1, -1):
        out[j] = value
        value = value * base % M
    return out


_P131 = _powers(131)
_P137 = _powers(137)


def digest(data):
    """`cli/digest.hero`'s `digest` of `data`'s bytes, a chunk at a time: the
    recurrence `a = (a * 131 + v) % M` over a chunk is `a * 131 ** n` plus the
    chunk's bytes weighed by the powers, which `map` and `sum` add in C."""
    a = b = 0
    view = memoryview(data)
    for at in range(0, len(view), CHUNK):
        chunk = view[at:at + CHUNK]
        n = len(chunk)
        a = (a * pow(131, n, M) + sum(map(mul, chunk, _P131[CHUNK - n:]))) % M
        b = (b * pow(137, n, M) + sum(map(mul, chunk, _P137[CHUNK - n:]))) % M
    return "%08x%08x" % (a, b)


def text_key(data, where):
    """The key of a file of UTF-8, as the compiler takes it."""
    try:
        data.decode("utf-8")
    except UnicodeDecodeError:
        raise Unmirrored(where + " is not UTF-8, which the compiler keys another way")
    return digest(data)


def read_bytes(path):
    try:
        with open(path, "rb") as handle:
            return handle.read()
    except OSError as e:
        raise Unmirrored("cannot read " + path + " (" + e.strerror + ")")


def uses_of(text):
    """The module paths a file's `use` lines name, in order."""
    return [m.group(1) for m in map(USE.match, text.splitlines()) if m]


def module_rows(root):
    """`module <name> <key>` for the root and every module it reaches."""
    base = os.path.dirname(root)
    stem = os.path.splitext(os.path.basename(root))[0]
    seen = {}
    todo = [(stem, root)]
    while todo:
        name, path = todo.pop()
        if name in seen:
            continue
        data = read_bytes(path)
        seen[name] = text_key(data, path)
        for used in uses_of(data.decode("utf-8")):
            todo.append((used, os.path.join(base, used + ".hero")))
    return ["module " + name + " " + key for name, key in seen.items()]


def library_text(path):
    """`library_source.text()`: its `LINES`, each a string literal on a line
    of its own, decoded, joined by a newline with one after the last."""
    text = read_bytes(path).decode("utf-8")
    at = text.find("constant LINES: [str]\n    [\n")
    end = text.find("\n    ]\n", at)
    if at < 0 or end < 0:
        raise Unmirrored(path + " holds no `constant LINES: [str]` list in the shape this reads")
    lines = []
    for row in text[at:end].splitlines()[2:]:
        literal = row.strip()
        if len(literal) < 2 or literal[0] != '"' or literal[-1] != '"':
            raise Unmirrored(path + " holds a row of LINES that is not one string literal: " + row)
        lines.append(unescaped(literal[1:-1], path))
    return "\n".join(lines) + "\n"


def unescaped(body, where):
    """A string literal's text, its seven escapes read as the spec reads
    them (`spec/heroes-spec.md` § 2)."""
    out = []
    i = 0
    while i < len(body):
        c = body[i]
        if c != "\\":
            out.append(c)
            i += 1
            continue
        nxt = body[i + 1] if i + 1 < len(body) else ""
        if nxt in ESCAPES:
            out.append(ESCAPES[nxt])
            i += 2
        elif nxt == "u" and body[i + 2:i + 3] == "{":
            shut = body.find("}", i + 3)
            if shut < 0:
                raise Unmirrored(where + " holds a `\\u{` with no `}`")
            out.append(chr(int(body[i + 3:shut], 16)))
            i = shut + 1
        else:
            raise Unmirrored(where + " holds an escape this does not read: \\" + nxt)
    return "".join(out)


def included_names(data):
    """`runtime_key.included_names`: every line opening `#include "`, the
    name up to the next quote on that line."""
    head = b'#include "'
    out = []
    for line in data.split(b"\n"):
        if line.startswith(head):
            shut = line.find(b'"', len(head))
            if shut >= 0:
                out.append(line[len(head):shut].decode("utf-8"))
    return out


def runtime_key(runtime):
    """`Toolchain.runtime_key`: `runtime.c`'s key and each included file's,
    joined, keyed again."""
    entry = os.path.join(runtime, "runtime.c")
    data = read_bytes(entry)
    keys = [text_key(data, entry)]
    for name in included_names(data):
        path = os.path.join(runtime, name)
        keys.append(text_key(read_bytes(path), path))
    return digest("".join(keys).encode("ascii"))


def runtime_of(tree):
    """Where the compiler run in `tree` looks first: `$HEROES_RUNTIME`, else
    `./runtime`."""
    hint = os.environ.get("HEROES_RUNTIME")
    if hint:
        return hint if os.path.isabs(hint) else os.path.join(tree, hint)
    return os.path.join(tree, "runtime")


def sources_key(root, library, runtime):
    """The sixteen characters a program built from `root`, with the library
    `library` (a `library_source.hero`) and the runtime directory `runtime`,
    carries."""
    rows = module_rows(root)
    rows.append("library library " + digest(library_text(library).encode("utf-8")))
    rows.append("runtime " + runtime_key(runtime))
    return digest(("\n".join(sorted(rows)) + "\n").encode("utf-8"))


def tree_sources(tree):
    """The key of the compiler `tree` builds from its own sources."""
    return sources_key(os.path.join(tree, ROOT), os.path.join(tree, LIBRARY), runtime_of(tree))


def compiler_said(compiler, cwd):
    """What `<compiler> --version` says on its `sources` line: (the key or
    None, why None)."""
    try:
        done = subprocess.run([compiler, "--version"], cwd=cwd, capture_output=True, text=True, timeout=20)
    except (OSError, subprocess.TimeoutExpired) as e:
        return None, "`" + compiler + " --version` did not answer (" + str(e) + ")"
    if done.returncode != 0:
        return None, "`" + compiler + " --version` exited " + str(done.returncode)
    for line in done.stdout.splitlines():
        if line.startswith("sources "):
            return line[len("sources "):].strip(), None
    return None, "`" + compiler + " --version` prints no `sources` line, so it was built before panel 222's digest"
