#!/usr/bin/env python3
"""The hooks' own tests.

Run from the repository's root:

    python3 -I -m unittest discover -s .claude/hooks -t .claude/hooks

Why a file of its own. Until 2026-10-07 no test read a hook: a hook was judged
by the session it guarded, which is the trunk's copy, so a hook edited in a lane
was tested by nothing until it reached the trunk. These tests run the hooks as
the settings do, `fmt_check.py` as a process given its event on stdin and
`guard_bash.verdict` on a command line, over trees built for the test.

A TREE here is what `trees.py` recognises: `seed/heroes.c`, a `.git` entry,
`selfhost/main.hero`, `runtime/runtime.c`, and a stand-in compiler, `heroes`, a
shell script that answers `fmt`, `check` and the harness's `run` as the test
arranges and writes every call it receives into `calls.log` beside it, so a
test asks WHICH compiler judged and with what. Its words: a file holding
`BROKEN` does not parse, one holding `UNCANONICAL` is not canonical, a
`selfhost/` holding `UNCHECKED` does not check; `run ... -- <c> <suite> <pick>`
prints `answer-<suite>` and exits with `answer-<suite>.code`, or exits 2 when
the test wrote none, as the harness does for a filter that selects nothing.

Nothing is removed afterwards: the batch's rule is never to `rm`, in a script
either, and every directory here is a few kilobytes under the system's
temporary folder.
"""

import json
import os
import subprocess
import sys
import tempfile
import unittest

HOOKS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HOOKS)

import guard_bash  # noqa: E402

FAKE = r"""#!/bin/sh
here=$(cd "$(dirname "$0")" && pwd)
echo "$PWD|$*" >> "$here/calls.log"
case "$1" in
fmt)
    if grep -q BROKEN "$2"; then echo "error[broken]: $2 holds BROKEN" >&2; exit 1; fi
    if grep -q UNCANONICAL "$2"; then sed 's/UNCANONICAL/canonical/' "$2"; exit 0; fi
    cat "$2"; exit 0 ;;
check)
    if [ "$2" = selfhost/main.hero ]; then
        if grep -rq UNCHECKED selfhost; then echo "error[unchecked]: selfhost holds UNCHECKED" >&2; exit 1; fi
    elif grep -q UNCHECKED "$2"; then echo "error[unchecked]: $2 holds UNCHECKED" >&2; exit 1; fi
    exit 0 ;;
run)
    suite=$5
    if [ -f "$here/answer-$suite" ]; then cat "$here/answer-$suite"; exit "$(cat "$here/answer-$suite.code")"; fi
    echo "harness: no case matches"; exit 2 ;;
esac
exit 0
"""

GIT_ENV = dict(
    os.environ,
    GIT_CONFIG_GLOBAL=os.devnull,
    GIT_CONFIG_NOSYSTEM="1",
    GIT_AUTHOR_NAME="t",
    GIT_AUTHOR_EMAIL="t@t",
    GIT_COMMITTER_NAME="t",
    GIT_COMMITTER_EMAIL="t@t",
)

# A day the sources are dated to, so a test sets a compiler before or after it.
T0 = 1_800_000_000


def write(path, text, at=None):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as handle:
        handle.write(text)
    if at is not None:
        os.utime(path, (at, at))
    return path


def date(path, at):
    os.utime(path, (at, at))


def git(cwd, *args, check=True):
    return subprocess.run(["git"] + list(args), cwd=cwd, env=GIT_ENV, capture_output=True, text=True, check=check)


def make_tree(root, compiler=True, built=T0 + 100, repo=False):
    """A tree at `root`: its sources dated `T0`, its compiler built at `built`.
    `repo` makes `.git` a repository with the sources committed; otherwise it
    is a worktree's `.git` file, which is all a tree's recognition reads."""
    write(os.path.join(root, "seed", "heroes.c"), "/* seed */\n", T0)
    write(os.path.join(root, "selfhost", "main.hero"), "function main()\n    return\n", T0)
    write(os.path.join(root, "runtime", "runtime.c"), "/* runtime */\n", T0)
    if repo:
        git(root, "init", "-q", "-b", "main", ".")
        git(root, "add", "seed", "selfhost", "runtime")
        git(root, "commit", "-q", "-m", "base", "--", "seed", "selfhost", "runtime")
    else:
        write(os.path.join(root, ".git"), "gitdir: /nowhere\n")
    if compiler:
        heroes = write(os.path.join(root, "heroes"), FAKE)
        os.chmod(heroes, 0o755)
        date(heroes, built)
    return root


def fresh(prefix):
    """A new directory for one test, by its real path: the stand-in reports
    the directory it runs in as the system resolves it, `/private/var/...` on
    macOS for a `/var/...` it was given."""
    return os.path.realpath(tempfile.mkdtemp(prefix=prefix))


def make_lane(trunk, name, built=T0 + 100):
    """A worktree of the repository `trunk` under `.claude/worktrees/<name>`,
    as a lane is made, with a compiler of its own built at `built`."""
    lane = os.path.join(trunk, ".claude", "worktrees", name)
    git(trunk, "worktree", "add", "-q", "-b", "lane-" + name, lane)
    for part in ("seed/heroes.c", "selfhost/main.hero", "runtime/runtime.c"):
        date(os.path.join(lane, part), T0)
    heroes = write(os.path.join(lane, "heroes"), FAKE)
    os.chmod(heroes, 0o755)
    date(heroes, built)
    return lane


def staged(tree, rel, text, at=None):
    """`rel` written in `tree` and staged in that tree's index."""
    path = write(os.path.join(tree, rel), text, at)
    git(tree, "add", "--", rel)
    return path


def calls(tree):
    try:
        with open(os.path.join(tree, "calls.log"), encoding="utf-8") as handle:
            return handle.read().splitlines()
    except OSError:
        return []


def written(session, path):
    """`fmt_check.py` run as the settings run it, on a write of `path` by a
    session whose directory is `session`: (exit, stderr)."""
    event = json.dumps({"cwd": session, "tool_input": {"file_path": path}})
    done = subprocess.run(
        [sys.executable, os.path.join(HOOKS, "fmt_check.py")],
        input=event, capture_output=True, text=True, timeout=60,
    )
    return done.returncode, done.stderr


class Place(unittest.TestCase):
    """Defect 254: a written file is judged in the tree it stands in."""

    def setUp(self):
        self.top = fresh("hooks-254-")
        self.trunk = make_tree(os.path.join(self.top, "trunk"))
        self.lane = make_tree(os.path.join(self.trunk, ".claude", "worktrees", "lane"))

    def test_a_lane_file_written_by_a_trunk_session_is_judged_by_the_lane(self):
        module = write(os.path.join(self.lane, "selfhost", "parse", "x.hero"), "UNCHECKED\n")
        code, said = written(self.trunk, module)
        self.assertEqual(code, 2, said)
        self.assertIn("does not check", said)
        self.assertIn(self.lane + "|check selfhost/main.hero", calls(self.lane))
        self.assertEqual(calls(self.trunk), [])

    def test_a_lane_file_over_its_ceiling_is_told_so(self):
        module = write(os.path.join(self.lane, "selfhost", "big.hero"), "x\n" * 301)
        code, said = written(self.trunk, module)
        self.assertEqual(code, 2, said)
        self.assertIn("selfhost/big.hero: 301 lines of code", said)

    def test_a_trunk_file_written_by_a_lane_session_is_judged_by_the_trunk(self):
        module = write(os.path.join(self.trunk, "selfhost", "y.hero"), "UNCHECKED\n")
        code, said = written(self.lane, module)
        self.assertEqual(code, 2, said)
        self.assertIn(self.trunk + "|check selfhost/main.hero", calls(self.trunk))
        self.assertEqual(calls(self.lane), [])

    def test_a_trunk_file_written_by_a_trunk_session_is_named_as_the_tree_reads_it(self):
        module = write(os.path.join(self.trunk, "selfhost", "z.hero"), "BROKEN\n")
        code, said = written(self.trunk, module)
        self.assertEqual(code, 2, said)
        self.assertTrue(said.startswith("selfhost/z.hero does not parse"), said)

    def test_a_file_outside_every_tree_is_formatted_by_the_session_tree(self):
        loose = write(os.path.join(self.top, "scratch", "probe.hero"), "BROKEN\n")
        code, said = written(self.trunk, loose)
        self.assertEqual(code, 2, said)
        self.assertIn("does not parse", said)
        self.assertEqual([c.split("|")[1] for c in calls(self.trunk)], ["fmt " + loose])

    def test_a_lane_with_no_compiler_says_the_file_was_not_judged(self):
        bare = make_tree(os.path.join(self.trunk, ".claude", "worktrees", "bare"), compiler=False)
        module = write(os.path.join(bare, "selfhost", "w.hero"), "BROKEN\n")
        code, said = written(self.trunk, module)
        self.assertEqual(code, 2, said)
        self.assertIn("not judged", said)
        self.assertIn(bare, said)
        self.assertEqual(calls(self.trunk), [])

    def test_a_directory_with_a_seed_and_no_git_is_not_a_tree(self):
        loose = os.path.join(self.top, "copy")
        write(os.path.join(loose, "seed", "heroes.c"), "/* a copy */\n")
        module = write(os.path.join(loose, "selfhost", "v.hero"), "UNCHECKED\n")
        code, said = written(self.trunk, module)
        self.assertEqual(code, 0, said)
        self.assertEqual([c.split("|")[1] for c in calls(self.trunk)], ["fmt " + module])


class Age(unittest.TestCase):
    """Defect 384: a compiler older than its tree is told as that, never as the
    file's fault, and only where it refused something."""

    def setUp(self):
        self.top = fresh("hooks-384-")

    def stale(self, newer="selfhost/lexer.hero"):
        """A tree whose compiler was built before `newer` was written."""
        tree = make_tree(os.path.join(self.top, "tree"))
        write(os.path.join(tree, newer), "# moved\n", T0 + 200)
        return tree

    def test_an_older_compiler_refusing_a_file_is_told_as_its_age(self):
        tree = self.stale()
        case = write(os.path.join(tree, "tests", "harness", "suite_x.hero"), "BROKEN\n")
        code, said = written(tree, case)
        self.assertEqual(code, 2, said)
        self.assertNotIn("does not parse", said)
        self.assertIn("older than its tree", said)
        self.assertIn("selfhost/lexer.hero", said)
        self.assertIn("./heroes build selfhost/main.hero -o heroes", said)
        self.assertIn("holds BROKEN", said)

    def test_a_seed_newer_than_the_compiler_names_the_seed_and_the_build_from_it(self):
        tree = self.stale(newer="seed/heroes.c")
        case = write(os.path.join(tree, "examples", "e.hero"), "BROKEN\n")
        code, said = written(tree, case)
        self.assertEqual(code, 2, said)
        self.assertIn("seed/heroes.c", said)
        self.assertIn("clang -I runtime seed/heroes.c runtime/runtime.c -o heroes", said)

    def test_an_older_compiler_finding_a_file_not_canonical_is_told_as_its_age(self):
        tree = self.stale()
        case = write(os.path.join(tree, "examples", "e.hero"), "UNCANONICAL\n")
        code, said = written(tree, case)
        self.assertEqual(code, 2, said)
        self.assertNotIn("is not canonical", said)
        self.assertIn("older than its tree", said)

    def test_an_older_compiler_failing_the_whole_check_is_told_as_its_age(self):
        tree = self.stale()
        module = write(os.path.join(tree, "selfhost", "y.hero"), "UNCHECKED\n")
        code, said = written(tree, module)
        self.assertEqual(code, 2, said)
        self.assertNotIn("does not check", said)
        self.assertIn("older than its tree", said)

    def test_the_file_written_is_not_the_age_it_is_the_subject(self):
        tree = make_tree(os.path.join(self.top, "tree"))
        module = write(os.path.join(tree, "selfhost", "y.hero"), "BROKEN\n", T0 + 200)
        code, said = written(tree, module)
        self.assertEqual(code, 2, said)
        self.assertTrue(said.startswith("selfhost/y.hero does not parse"), said)

    def test_a_compiler_newer_than_its_tree_is_read_as_its_verdict(self):
        tree = make_tree(os.path.join(self.top, "tree"), built=T0 + 900)
        write(os.path.join(tree, "selfhost", "lexer.hero"), "# moved\n", T0 + 200)
        case = write(os.path.join(tree, "examples", "e.hero"), "BROKEN\n")
        code, said = written(tree, case)
        self.assertEqual(code, 2, said)
        self.assertIn("does not parse", said)
        self.assertNotIn("older than its tree", said)

    def test_an_older_compiler_accepting_a_file_says_nothing(self):
        tree = self.stale()
        case = write(os.path.join(tree, "examples", "e.hero"), "fine\n")
        code, said = written(tree, case)
        self.assertEqual(code, 0, said)
        self.assertEqual(said, "")


class HarnessTree(unittest.TestCase):
    """Defect 348: a harness run's compiler is judged against the tree the run
    stands in, found from the command's own `cd`, never the session's."""

    def setUp(self):
        self.top = fresh("hooks-348-")
        self.trunk = make_tree(os.path.join(self.top, "trunk"))
        self.lane = make_tree(os.path.join(self.trunk, ".claude", "worktrees", "lane"))

    def age(self, tree, what="selfhost/resolve/state.hero"):
        """`tree`'s compiler made older than one of its sources."""
        write(os.path.join(tree, what), "# moved\n", T0 + 200)

    def test_a_lane_run_from_a_trunk_session_is_judged_by_the_lane(self):
        self.age(self.trunk)
        said = guard_bash.verdict("cd " + self.lane + " && ./heroes run tests/harness/main.hero -- ./heroes records", self.trunk)
        self.assertIsNone(said)

    def test_an_old_lane_compiler_is_refused_from_a_trunk_session(self):
        self.age(self.lane)
        said = guard_bash.verdict("cd " + self.lane + " && ./heroes run tests/harness/main.hero -- ./heroes records", self.trunk)
        self.assertIsNotNone(said)
        self.assertIn("selfhost/resolve/state.hero", said)
        self.assertIn(self.lane, said)

    def test_a_relative_cd_moves_the_tree_too(self):
        self.age(self.lane)
        said = guard_bash.verdict("cd .claude/worktrees/lane && ./heroes run tests/harness/main.hero -- ./heroes", self.trunk)
        self.assertIsNotNone(said)

    def test_an_absolute_compiler_is_judged_against_the_tree_the_run_stands_in(self):
        self.age(self.lane)
        heroes = os.path.join(self.lane, "heroes")
        said = guard_bash.verdict("cd " + self.lane + " && " + heroes + " run tests/harness/main.hero -- " + heroes, self.trunk)
        self.assertIsNotNone(said)

    def test_a_harness_named_by_its_absolute_path_is_a_harness_run(self):
        self.age(self.lane)
        harness = os.path.join(self.lane, "tests", "harness", "main.hero")
        said = guard_bash.verdict("./heroes run " + harness + " -- ./heroes", self.lane)
        self.assertIsNotNone(said)

    def test_a_directory_the_text_cannot_tell_gives_no_opinion(self):
        self.age(self.trunk)
        said = guard_bash.verdict('cd "$NOWHERE_SET_348" && ./heroes run tests/harness/main.hero -- ./heroes', self.trunk)
        self.assertIsNone(said)

    def test_a_compiler_newer_than_its_tree_passes(self):
        said = guard_bash.verdict("cd " + self.lane + " && ./heroes run tests/harness/main.hero -- ./heroes", self.trunk)
        self.assertIsNone(said)

    def test_an_old_compiler_in_the_session_tree_is_still_refused(self):
        self.age(self.trunk)
        said = guard_bash.verdict("./heroes run tests/harness/main.hero -- ./heroes", self.trunk)
        self.assertIsNotNone(said)


class CommitTree(unittest.TestCase):
    """Defect 287: the staged-file check reads the index of the tree the commit
    runs in, `git -C` or the command's `cd`, never the session's."""

    def setUp(self):
        self.top = fresh("hooks-287-")
        self.trunk = make_tree(os.path.join(self.top, "trunk"), repo=True)
        self.lane = make_lane(self.trunk, "lane")

    def test_a_lane_commit_after_cd_reads_the_lane_index(self):
        staged(self.lane, "examples/x.hero", "BROKEN\n")
        said = guard_bash.verdict("cd " + self.lane + " && git commit -F m -- examples/x.hero", self.trunk)
        self.assertIsNotNone(said)
        self.assertIn("examples/x.hero does not parse", said)
        self.assertIn(self.lane + "|fmt examples/x.hero", calls(self.lane))
        self.assertEqual(calls(self.trunk), [])

    def test_a_lane_commit_by_git_c_reads_the_lane_index(self):
        staged(self.lane, "examples/x.hero", "UNCANONICAL\n")
        said = guard_bash.verdict("git -C " + self.lane + " commit -F m -- examples/x.hero", self.trunk)
        self.assertIsNotNone(said)
        self.assertIn("examples/x.hero is not canonical", said)

    def test_the_session_index_does_not_refuse_a_lane_commit(self):
        staged(self.trunk, "examples/t.hero", "BROKEN\n")
        said = guard_bash.verdict("cd " + self.lane + " && git commit -F m -- examples/y.hero", self.trunk)
        self.assertIsNone(said)

    def test_a_commit_from_a_subdirectory_reads_the_whole_index(self):
        staged(self.lane, "selfhost/x.hero", "BROKEN\n")
        staged(self.lane, "examples/z.hero", "BROKEN\n")
        said = guard_bash.verdict("cd " + os.path.join(self.lane, "selfhost") + " && git commit -F m -- x.hero", self.trunk)
        self.assertIsNotNone(said)
        self.assertIn("selfhost/x.hero does not parse", said)
        self.assertIn("examples/z.hero does not parse", said)

    def test_an_older_compiler_refusing_a_staged_file_is_told_as_its_age(self):
        write(os.path.join(self.lane, "selfhost", "lexer.hero"), "# moved\n", T0 + 200)
        staged(self.lane, "examples/x.hero", "BROKEN\n")
        said = guard_bash.verdict("cd " + self.lane + " && git commit -F m -- examples/x.hero", self.trunk)
        self.assertIsNotNone(said)
        self.assertNotIn("does not parse", said)
        self.assertIn("older than", said)
        self.assertIn("selfhost/lexer.hero", said)

    def test_a_canonical_staged_file_passes(self):
        staged(self.lane, "examples/x.hero", "fine\n")
        said = guard_bash.verdict("cd " + self.lane + " && git commit -F m -- examples/x.hero", self.trunk)
        self.assertIsNone(said)


if __name__ == "__main__":
    unittest.main()
