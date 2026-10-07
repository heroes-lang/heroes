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


def conflicted(tree, op, extra=None):
    """`tree` stopped in a conflicted `op`, `merge` or `cherry-pick`, of a
    branch that changed `a.txt` and `b.txt`, both resolved and staged; and
    `extra`, a file the operation never touched, staged beside them."""
    write(os.path.join(tree, "a.txt"), "a\n")
    write(os.path.join(tree, "b.txt"), "b\n")
    git(tree, "add", "--", "a.txt", "b.txt")
    git(tree, "commit", "-q", "-m", "ab", "--", "a.txt", "b.txt")
    git(tree, "checkout", "-q", "-b", "other")
    write(os.path.join(tree, "a.txt"), "a other\n")
    write(os.path.join(tree, "b.txt"), "b other\n")
    git(tree, "commit", "-q", "-m", "other", "--", "a.txt", "b.txt")
    git(tree, "checkout", "-q", "main")
    write(os.path.join(tree, "a.txt"), "a main\n")
    git(tree, "commit", "-q", "-m", "main", "--", "a.txt")
    stopped = git(tree, op, "other", check=False)
    assert stopped.returncode != 0, stopped.stdout + stopped.stderr
    staged(tree, "a.txt", "a resolved\n")
    git(tree, "add", "--", "b.txt")
    if extra is not None:
        staged(tree, extra, "not the operation's\n")


class Pathspec(unittest.TestCase):
    """Defect 403: a commit whose `--` limits nothing, and an operation
    concluded with the whole index, are refused; the routes that limit it are
    not."""

    def setUp(self):
        self.top = fresh("hooks-403-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)

    def refused(self, command):
        said = guard_bash.verdict(command, self.tree)
        self.assertIsNotNone(said, command)
        return said

    def passed(self, command):
        said = guard_bash.verdict(command, self.tree)
        self.assertIsNone(said, command)

    def test_a_dash_dash_with_no_path_is_refused(self):
        said = self.refused("git commit -F m --")
        self.assertIn("names no path", said)

    def test_a_path_list_of_expansions_alone_is_refused(self):
        said = self.refused('git commit -F m -- "${paths[@]}"')
        self.assertIn("expansion", said)
        self.refused("git commit -F m -- $(cat list.txt)")

    def test_a_path_list_with_a_word_in_it_passes(self):
        self.passed("git commit -F m -- examples/a.hero $more")

    def test_amending_the_message_alone_passes(self):
        self.passed("git commit --amend --only -m better --")

    def test_a_pathspec_naming_the_whole_tree_is_refused(self):
        self.assertIn("whole tree", self.refused("git commit -m x -- ."))
        self.refused("git commit -m x -- :/")
        self.refused("git commit -m x -- " + self.tree)
        self.refused("cd selfhost && git commit -m x -- ..")
        self.passed("git commit -m x -- selfhost/")

    def test_a_merge_concluded_with_a_file_it_did_not_bring_is_refused(self):
        conflicted(self.tree, "merge", extra="c.txt")
        said = self.refused("GIT_EDITOR=true git merge --continue")
        self.assertIn("c.txt", said)
        self.assertNotIn("a.txt", said)
        self.refused("git commit --no-edit")

    def test_a_merge_concluded_with_its_own_files_passes(self):
        conflicted(self.tree, "merge")
        self.passed("GIT_EDITOR=true git merge --continue")
        self.passed("git commit --no-edit")

    def test_a_bare_commit_outside_a_merge_is_still_refused(self):
        self.refused("git commit -m x")

    def test_a_cherry_pick_concluded_with_a_file_it_did_not_bring_is_refused(self):
        conflicted(self.tree, "cherry-pick", extra="c.txt")
        said = self.refused("git cherry-pick --continue")
        self.assertIn("c.txt", said)

    def test_a_commit_inside_a_shell_script_is_read(self):
        self.refused("bash -c 'git commit -F m --'")
        self.refused("sh -c \"cd x && git add -A\"")

    def test_a_command_behind_an_assignment_or_a_wrapper_is_read(self):
        self.refused("env X=1 git add -A")
        self.refused("caffeinate -i git add .")
        self.refused("/usr/bin/time -p git commit -m x")
        self.refused("timeout 30 git stash")

    def test_the_rules_read_on_the_words_as_written_still_fire(self):
        self.refused("UPDATE_GOLDEN=1 ./heroes run tests/harness/main.hero -- ./heroes check")
        self.refused("env")
        self.passed("env FOO=1 ls")
        self.passed("git -C . add CLAUDE.md")


ANNOTATIONS = '''constant DIRECTORIES: [str]
    [
        "tests/golden/check"
        "tests/golden/unsupported"
        "tests/golden/permissive"
        "tests/golden/full"
    ]

constant RUN_ROOTS: [str]
    [
        "tests/golden/fixedbugs"
        "tests/golden/surface-fixtures"
    ]
'''


def answer(tree, suite, code, text):
    """What the stand-in's harness says when `suite` is run in `tree`."""
    write(os.path.join(tree, "answer-" + suite), text)
    write(os.path.join(tree, "answer-" + suite + ".code"), str(code) + "\n")


def runs(tree):
    """The harness runs the stand-in of `tree` was asked for, as `<suite> <word>`."""
    out = []
    for call in calls(tree):
        said = call.split("|", 1)[1].split()
        if said and said[0] == "run":
            out.append(" ".join(said[4:]))
    return out


class Marks(unittest.TestCase):
    """Defect 334: a staged golden case holding a diagnostic on purpose is
    judged by its marks, asked of the `annotations` suite once, not by `fmt`."""

    def setUp(self):
        self.top = fresh("hooks-334-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        write(os.path.join(self.tree, "tests", "harness", "suite_annotations.hero"), ANNOTATIONS)

    def case(self, rel, marked=True, expected=True):
        staged(self.tree, rel, "BROKEN on purpose" + ("  #~ unknown_escape" if marked else "") + "\n")
        if expected:
            staged(self.tree, rel[: -len(".hero")] + ".expected", "x.hero:1:1: error[unknown_escape]: ...\n")

    def commit(self, *rels):
        return guard_bash.verdict("git commit -F m -- " + " ".join(rels), self.tree)

    def test_a_marked_case_its_suite_passes_is_committed(self):
        answer(self.tree, "annotations", 0, "  annotations (only fixedbugs-900-a): 1 passed, 0 failed\n")
        self.case("tests/golden/check/fixedbugs-900-a.hero")
        self.assertIsNone(self.commit("tests/golden/check/fixedbugs-900-a.hero"))
        self.assertEqual(runs(self.tree), ["annotations fixedbugs-900-a"])

    def test_a_marked_case_its_suite_fails_is_refused_with_the_suite_words(self):
        answer(
            self.tree, "annotations", 1,
            "FAIL annotations/fixedbugs-900-a\n  the annotations and the diagnostics disagree\n"
            "  annotations (only fixedbugs-900-a): 0 passed, 1 failed\n",
        )
        self.case("tests/golden/check/fixedbugs-900-a.hero")
        said = self.commit("tests/golden/check/fixedbugs-900-a.hero")
        self.assertIsNotNone(said)
        self.assertIn("marks and its expectation disagree", said)
        self.assertIn("the annotations and the diagnostics disagree", said)
        self.assertNotIn("does not parse", said)

    def test_several_marked_cases_are_asked_in_one_run(self):
        answer(self.tree, "annotations", 0, "  annotations (only fixedbugs-90): 2 passed, 0 failed\n")
        self.case("tests/golden/check/fixedbugs-900-a.hero")
        self.case("tests/golden/unsupported/fixedbugs-901-b.hero")
        self.assertIsNone(self.commit("tests/golden"))
        self.assertEqual(runs(self.tree), ["annotations fixedbugs-90"])

    def test_a_marked_case_in_the_run_roots_is_left_to_the_whole_suite(self):
        self.case("tests/golden/surface-fixtures/brackets/a.hero", expected=False)
        self.assertIsNone(self.commit("tests/golden/surface-fixtures/brackets/a.hero"))
        self.assertEqual(runs(self.tree), [])

    def test_an_unmarked_case_fmt_refuses_does_not_parse(self):
        self.case("tests/golden/check/plain.hero", marked=False)
        self.assertIn("tests/golden/check/plain.hero does not parse", self.commit("tests/golden/check/plain.hero"))

    def test_a_marked_program_that_must_run_keeps_the_fmt_verdict(self):
        self.case("tests/golden/run/prog.hero")
        self.assertIn("tests/golden/run/prog.hero does not parse", self.commit("tests/golden/run/prog.hero"))
        self.assertEqual(runs(self.tree), [])

    def test_a_marked_case_with_no_expectation_keeps_the_fmt_verdict(self):
        self.case("tests/golden/check/lonely.hero", expected=False)
        self.assertIn("does not parse", self.commit("tests/golden/check/lonely.hero"))

    def test_a_suite_that_cannot_answer_gives_no_opinion(self):
        self.case("tests/golden/check/fixedbugs-900-a.hero")
        self.assertIsNone(self.commit("tests/golden/check/fixedbugs-900-a.hero"))

    def test_a_run_past_its_bound_is_ended_with_its_children(self):
        import marks

        pids = os.path.join(self.top, "pids")
        script = "sleep 30 & echo $! > " + pids + "; sleep 30"
        self.assertIsNone(marks.bounded(["sh", "-c", script], self.top, limit=1.0))
        with open(pids, encoding="utf-8") as handle:
            orphan = int(handle.read().strip())
        # Ended, and reaped by whoever inherits it: a moment is allowed for that.
        import time

        for _ in range(30):
            try:
                os.kill(orphan, 0)
            except ProcessLookupError:
                break
            time.sleep(0.1)
        with self.assertRaises(ProcessLookupError):
            os.kill(orphan, 0)

    def test_the_write_hook_asks_no_run_of_a_case_the_narrowed_suite_does_not_judge(self):
        prog = write(os.path.join(self.tree, "tests", "golden", "run", "prog.hero"), "fine\n")
        write(os.path.join(self.tree, "tests", "golden", "run", "prog.expected"), "42\n")
        code, said = written(self.tree, prog)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), [])

    def test_the_write_hook_tells_a_marked_case_its_suite_fails(self):
        answer(self.tree, "annotations", 1, "FAIL annotations/w\n  disagree\n")
        write(os.path.join(self.tree, "tests", "golden", "check", "w.expected"), "x\n")
        case = write(os.path.join(self.tree, "tests", "golden", "check", "w.hero"), "BROKEN  #~ unknown_escape\n")
        code, said = written(self.tree, case)
        self.assertEqual(code, 2, said)
        self.assertIn("marks and its expectation disagree", said)
        self.assertEqual(runs(self.tree), ["annotations w"])


if __name__ == "__main__":
    unittest.main()
