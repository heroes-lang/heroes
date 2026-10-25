#!/usr/bin/env python3
"""A push that can carry a milestone tag, judged by `records/tagged` with the tag placed.

Why this exists. Defect 2198: `m-thesis-harness` was placed on 2026-10-24 over
defects 2139 and 2143, each `adjacent` and open at the tag before it, so that
its own placing aged them to count as `blocking`. The close had asked `records`
before placing the tag, when `tagged` still judged the tag before it, and could
not see what the new tag would stand over; the tag was pushed before the next
reading, and a pushed tag is not moved. A local tag can be removed and placed
again until it is pushed, so the moment to ask is the push: the author's choice
of 2026-10-25, among the close checklist alone and the check judging a tag not
yet placed, which would speak all through a milestone where it is built to
speak once.

WHAT IT ASKS. A `git push` that can carry an `m-*` tag runs, in the tree the
command stands in, `<tree>/heroes run tests/harness/main.hero -- <tree>/heroes
records records/tagged`: `tagged` alone, which judges the newest `m-*` tag the
tree holds, with GitHub asked once (`tests/harness/suite_records.hero`,
`TAG_CHECK`). The push goes through on `records: 1 passed, 0 failed` at exit 0
and on nothing else: a red, a skip (GitHub not asked), an exit 2, a compiler
the tree has not built, and a run past `marks.LIMIT` each refuse it, the guard
erring toward refusing, since what it guards cannot be taken back. A push in a
directory that is no tree of this project is not judged.

WHICH PUSHES. `--tags`, `--mirror` and `--follow-tags`, the last carrying only
annotated tags where the milestone tags are lightweight (`git for-each-ref
--format='%(taggerdate)'` empty for `m-witnessed-seed` and `m-thesis-harness`,
2026-10-25), read all the same since a tag may yet be annotated; and a refspec
whose source or destination is a tag `m-...` or `refs/tags/m-...`, or `tag
m-...`. A push that names neither is a branch's and is not judged. A push made
by configuration alone (`remote.<name>.push`, `push.followTags`) is not seen:
this is a tripwire, not a sandbox.

ITS COST, measured 2026-10-25 in the lane `rechecked-issues` with its harness
built: 34.61 s real and 9.84 s of user and system time for the narrowed run,
the rest GitHub's listing of the issues, under `marks.LIMIT` (100 s) and the
120 s the settings give this hook.
"""

import os

import marks
import trees

HARNESS = os.path.join("tests", "harness", "main.hero")
# The word that narrows `records` to `tagged`, held to
# `tests/harness/suite_records.hero`'s `TAG_CHECK` by the hooks' own tests.
TAG_CHECK = "records/tagged"
GREEN = "records: 1 passed, 0 failed"
TAG_OPTIONS = frozenset({"--tags", "--mirror", "--follow-tags"})
# The options of `git push` that take the next word as their value.
VALUED = frozenset({"-o", "--push-option", "--receive-pack", "--exec", "--repo"})
RULE = "CLAUDE.md § Verification, a milestone tagged only over a clean list; defect 2198"


def is_milestone_ref(word):
    """Whether one side of a refspec names a milestone tag."""
    name = word[len("refs/tags/"):] if word.startswith("refs/tags/") else word
    return name.startswith("m-")


def carries_tag(rest):
    """Whether `git push <rest>` can carry an `m-*` tag. The first word that is
    not an option is the repository, unless `--repo` named it; every word after
    it is a refspec, `tag <name>` among them."""
    words = []
    named_repo = False
    i = 0
    while i < len(rest):
        word = rest[i]
        if word == "--":
            words.extend(rest[i + 1:])
            break
        if word in TAG_OPTIONS:
            return True
        if word in VALUED:
            named_repo = named_repo or word == "--repo"
            i += 2
            continue
        if word.startswith("--repo="):
            named_repo = True
        elif not word.startswith("-"):
            words.append(word)
        i += 1
    refspecs = words if named_repo else words[1:]
    j = 0
    while j < len(refspecs):
        spec = refspecs[j]
        if spec == "tag" and j + 1 < len(refspecs):
            if refspecs[j + 1].startswith("m-"):
                return True
            j += 2
            continue
        sides = spec.lstrip("+").split(":")
        if any(is_milestone_ref(side) for side in sides if side):
            return True
        j += 1
    return False


def verdict(where):
    """The refusal of a push from `where` that can carry an `m-*` tag, or None
    when `tagged` passes, or when `where` is no tree of this project."""
    tree = trees.tree_of(where) if where is not None else None
    if tree is None:
        return None
    compiler = trees.compiler_of(tree)
    if not trees.runnable(compiler):
        return (
            "refused: this push can carry a milestone tag, and `records/tagged` is asked of it "
            "with the tag placed, by the tree's own compiler, which " + tree + " has not built. "
            + trees.rebuild_hint(tree) + "\n" + RULE
        )
    done = marks.bounded([compiler, "run", HARNESS, "--", compiler, "records", TAG_CHECK], cwd=tree)
    if done is None:
        return (
            "refused: this push can carry a milestone tag, and `records/tagged` did not answer "
            "within " + str(int(marks.LIMIT)) + " s. Ask it yourself, `./heroes run " + HARNESS
            + " -- ./heroes records " + TAG_CHECK + "`, and push again once it passes.\n" + RULE
        )
    out = done.stdout.decode("utf-8", errors="replace")
    err = done.stderr.decode("utf-8", errors="replace")
    if done.returncode == 0 and any(line.strip() == GREEN for line in out.splitlines()):
        return None
    said = (out + err).strip().splitlines()[:40]
    return (
        "refused: this push can carry a milestone tag, and `records/tagged`, asked with the tag "
        "placed, did not pass (exit " + str(done.returncode) + "). A local tag can be removed "
        "(`git tag -d <tag>`) and placed again once the list is clean; a pushed one cannot.\n    "
        + "\n    ".join(said) + "\n" + RULE
    )
