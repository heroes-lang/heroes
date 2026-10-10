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

**Every question is asked and every answer said, since 2026-10-08** (defect
493). Until that day the hook stopped at its first refusal. A module written
not canonical was told so and nothing else, and the `heroes fmt --in-place`
that answers it is a shell command, which runs no write hook, so a module that
crossed its ceiling in the same write was first told so by the commit guard or
by `layout`. Measured on the base that day: a `selfhost/` module of 303 lines
of code written with `x+1` for `x + 1` was told only *not canonical*, and the
same module written canonical was told its count. A check that failed, or a
compiler older than its tree, hid the ceiling the same way, and the growth and
marks questions behind them. (The four modules the defect names crossed the
ceiling in batch 14 under the trunk's hook of before defect 254, which read a
lane's module as `.claude/worktrees/...` and asked it no ceiling: that hook,
run on the same 303 lines in a tree below the session's directory, exits 0
and says nothing.) So the ceiling is counted on every write, on the canonical
form where the file parses (what `fmt --in-place` will write) and on the text
as written where it does not; the whole compiler's check, the growth run and
a case's marks are asked of a file that parses, canonical or not; and every
answer goes into one message. The slow questions share the hook's budget,
`marks.LIMIT`: the check is bounded by it as the growth run already was, so
the hook ends and speaks before the 120 s the settings give it. What Claude
Code shows of a hook it ends at its timeout is a question rather than a
premise, unmeasured; this hook prints once, at its end, so it is not asked to.

Contract with the harness: the tool call arrives as JSON on stdin and the path
is `tool_input.file_path`. A missing file, or anything unexpected, means exit 0
and no opinion: this hook never blocks work over its own inability to run. A
tree with no compiler built is the one absence it names (exit 2, which after a
write is a notice and blocks nothing): its file was not judged, and silence
would read as a pass.
"""

import json
import os
import sys
import time

# The hooks' own modules are found beside this file whatever runs it:
# `python3 -I`, isolated, leaves the script's directory off `sys.path`, and
# this file then failed at its first import (measured 2026-10-07, Python 3.14).
HOOKS = os.path.dirname(os.path.abspath(__file__))
if HOOKS not in sys.path:
    sys.path.insert(0, HOOKS)

import ceiling
import marks
import trees

STARTED = time.monotonic()
# The least a slow question is given; under it the question is not asked.
FLOOR = 5.0
LAYER ="(layer 0, `.claude/rules/verification.md` § A suite is the last judge, CL-079)"


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


def head(text, lines=40):
    rows = text.decode("utf-8", errors="replace").rstrip("\n").split("\n")
    if len(rows) > lines:
        rows = rows[:lines] + ["... (" + str(len(rows) - lines) + " more lines)"]
    return "\n".join(rows)


def told(place, refusals):
    """The compiler's refusals of the file as one message, or None: each
    verdict with what the compiler said when it is as new as the sources of
    the tree it judges for, and its age instead when it is older (defect 384),
    `refusals` being (verdict, what the compiler said) pairs.

    The age is asked before the verdicts are read. A compiler built before a
    source of its tree was written may not know a form the tree's language
    now writes: on 2026-10-06 a file holding panel 192's `\\u{1b}` escape, under
    a tree whose compiler predated the escape, was told *does not parse* and
    *this language has no escape for one by its code*, two false sentences,
    where the round's compiler formatted it at exit 0. The file written is
    left out of the question, being the subject rather than the language; and
    a refusal alone asks it, so an older compiler that accepts the file costs
    nothing and says nothing."""
    if not refusals:
        return None
    old = trees.older_than_tree(place.compiler, place.home, besides=place.path)
    if old is None:
        return "\n".join(verdict + ("; the compiler says:\n" + said if said else "") for verdict, said in refusals)
    where, built, wrote = old
    return (
        place.shown + " was refused by a compiler older than its tree: " + place.compiler
        + " was built at " + trees.stamp(built) + ", before " + where + " was written at "
        + trees.stamp(wrote) + ", so the refusal may be that compiler's age rather than the "
        "file. Rebuild it in " + place.home + ", " + trees.rebuild_hint(where)
        + ", and write the file again. What the older compiler said:\n"
        + "\n".join(said or "(nothing; its canonical form differs from the file)" for _verdict, said in refusals)
    )


def say(parts):
    """Every answer the hook has, in front of the assistant, exit 2; exit 0
    when it has none."""
    parts = [part for part in parts if part]
    if not parts:
        return 0
    print("\n".join(parts) + "\n" + LAYER, file=sys.stderr)
    return 2


def left():
    """What the hook's budget leaves a slow question, `marks.LIMIT` less the
    time already spent, or None under `FLOOR` seconds: a question asked past
    it would carry the hook past the timeout the settings give it, and a
    question not asked gives no opinion."""
    remaining = marks.LIMIT - (time.monotonic() - STARTED)
    return remaining if remaining >= FLOOR else None


def ask(compiler, args, root):
    """`compiler args` run in `root` within what the budget leaves, or None
    when it could not answer in that time (`marks.bounded` ends it with its
    children)."""
    limit = left()
    if limit is None:
        return None
    return marks.bounded([compiler] + args, root, limit)


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
        return say([told(place, marks_against_expectation(compiler, place))])

    try:
        with open(path, "rb") as handle:
            on_disk = handle.read()
    except OSError:
        return 0

    # Every question below is asked whatever an earlier one answered, and
    # every answer is said together (defect 493): the compiler's refusals,
    # told as its age where it is older than its tree, then what the mirror
    # and the suites say.
    refusals = []
    beside = []

    # 1. The file parses, and it is canonical. `heroes fmt <file>` prints the
    #    canonical form and writes nothing; a file that does not parse makes it
    #    exit non-zero with the diagnostic on stderr.
    fmt = ask(compiler, ["fmt", path], place.home)
    if fmt is None:
        # No compiler's answer in time: the mirror needs none.
        if place.rel is None:
            return 0
        return say([ceiling.verdict(place.tree, place.rel, text=on_disk.decode("utf-8", errors="replace"))])
    if fmt.returncode != 0 and place.under("tests/golden/") and marks.has_marks(path):
        # A golden case whose `#~` marks claim diagnostics is refused by `fmt`
        # on purpose: its diagnostics are its subject (defect 272, 2026-10-05).
        # This hook said *does not parse* of every such case, a false alarm
        # that teaches its reader to pass it by; the marks are judged against
        # the expectation instead, which is the question the case asks.
        return say([told(place, marks_against_expectation(compiler, place))])
    parses = fmt.returncode == 0
    # A marked case of the annotations suite's directories that parses and is
    # not canonical on purpose, as defect 576's two are, is judged by its marks
    # alone, asked below (defect 581, 2026-10-10: this hook told it *not
    # canonical* as the commit guard refused it; `marks.held_to_marks` is the
    # rule's one statement, which the guard asks too).
    on_purpose = place.rel is not None and marks.held_to_marks(place.tree, place.rel, on_disk.decode("utf-8", errors="replace"))
    if not parses:
        refusals.append((place.shown + " does not parse", head(fmt.stderr)))
    elif fmt.stdout != on_disk and not on_purpose:
        refusals.append((
            place.shown + " is not canonical: run `heroes fmt " + path + " --in-place`.\n"
            "The `canonical` suite fails on it otherwise, and design.md §4.15 "
            "rests on a textual difference meaning a semantic one "
            "(CLAUDE.md § Verification)",
            "",
        ))
    # What the file is once formatted: the text the ceiling and the growth
    # are asked of, the file as written where it does not parse.
    shaped = (fmt.stdout if parses else on_disk).decode("utf-8", errors="replace")

    # 2. The line ceiling, by the mirror; the `layout` suite is the judge. It
    #    reads no compiler, so it is asked first and of every write.
    if place.rel is not None:
        how = None if parses and fmt.stdout == on_disk else ("its canonical form, which `heroes fmt --in-place` writes" if parses else "the text as written, which does not parse")
        beside.append(ceiling.verdict(place.tree, place.rel, text=shaped, counted=how))

    # 3. Names and types: the whole compiler for one of its modules, the one
    #    file for a harness module, each in the tree the file stands in, of a
    #    file that parses, canonical or not.
    check = None
    if parses and place.under("selfhost/"):
        check = ask(compiler, ["check", "selfhost/main.hero"], place.tree)
        subject = "selfhost/main.hero, with " + place.shown + " as written,"
    elif parses and place.under("tests/harness/"):
        check = ask(compiler, ["check", place.rel], place.tree)
        subject = place.shown
    if check is not None and check.returncode != 0:
        refusals.append((subject + " does not check", head(check.stderr)))

    # 4. A module's growths and appends, asked of `layout` narrowed to it
    #    where a line might be one (defect 215, `ceiling.might_grow`), within
    #    what the hook's budget leaves after the checks above.
    if parses and place.under("selfhost/") and place.rel.endswith(".hero") and ceiling.might_grow(shaped, ceiling.slow_appends(place.tree)):
        limit = left()
        rows = ceiling.layout_rows(compiler, place.tree, place.rel, limit) if limit is not None else None
        if rows:
            beside.append(place.shown + " is refused by the `layout` suite narrowed to it:\n" + "\n".join(rows))

    # 5. A golden case's marks against its expectation.
    if parses:
        refusals += marks_against_expectation(compiler, place)
    return say([told(place, refusals)] + beside)


def marks_against_expectation(compiler, place):
    """The `annotations` suite narrowed to the case `place` names (defect 286),
    run in the case's own tree with that tree's compiler (`marks.py`, which
    the commit guard asks through too), as a list of refusals, `told`'s pairs.

    One refusal with the suite's own words where it fails THIS case; none
    where the case is not one a narrowed run judges (a `.hero` of the suite's
    `DIRECTORIES` with its `.expected` beside it), or the run could not be
    made within the hook's budget. Until 2026-10-07 the run was made for any
    golden case with an `.expected` and its answer, *no case matches*, read as
    no opinion: the same verdict, at the price of a harness run (`marks.py`
    has the price)."""
    if not place.under("tests/golden/"):
        return []
    stem = place.rel[: place.rel.rfind(".")]
    if not marks.judged_narrowed(place.tree, stem + ".hero"):
        return []
    name = os.path.basename(stem)
    limit = left()
    found = marks.disagreements(compiler, place.tree, [name], limit) if limit is not None else None
    if not found or name not in found:
        return []
    return [(
        place.shown[: place.shown.rfind(".")] + "'s marks and its expectation disagree. "
        "Change whichever side is wrong; if the `.expected` is being rewritten "
        "next, this clears when it is",
        "\n".join(found[name]),
    )]


if __name__ == "__main__":
    sys.exit(main())
