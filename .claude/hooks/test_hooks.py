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
test asks WHICH compiler judged and with what; `fmt /dev/stdin` reads its text
as the real one does, kept beside it as `stdin.<pid>`. Its words: a file holding
`BROKEN` does not parse, one holding `UNCANONICAL` is not canonical, one
holding `SPLIT` is not canonical and its canonical form holds that line twice,
a `selfhost/` holding `UNCHECKED` does not check; `run ... -- <c> <suite> <pick>`
prints `answer-<suite>` and exits with `answer-<suite>.code`, or exits 2 when
the test wrote none, as the harness does for a filter that selects nothing.

Nothing is removed afterwards: the batch's rule is never to `rm`, in a script
either, and every directory here is a few kilobytes under the system's
temporary folder.
"""

import json
import os
import re
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

HOOKS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HOOKS)

import guard_bash  # noqa: E402
import marks  # noqa: E402
import provenance  # noqa: E402
import runs as what_runs  # noqa: E402
import tagged  # noqa: E402

FAKE = r"""#!/bin/sh
here=$(cd "$(dirname "$0")" && pwd)
echo "$PWD|$*" >> "$here/calls.log"
case "$1" in
--version)
    echo "heroes 0.0.0"
    if [ -f "$here/sources" ]; then echo "sources $(cat "$here/sources")"; fi
    exit 0 ;;
fmt)
    src=$2
    if [ "$src" = /dev/stdin ]; then src="$here/stdin.$$"; cat > "$src"; fi
    if grep -q BROKEN "$src"; then echo "error[broken]: $2 holds BROKEN" >&2; exit 1; fi
    if grep -q UNCANONICAL "$src"; then sed 's/UNCANONICAL/canonical/' "$src"; exit 0; fi
    if grep -q SPLIT "$src"; then awk '/SPLIT/ { print "split"; print "split"; next } { print }' "$src"; exit 0; fi
    cat "$src"; exit 0 ;;
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

# A tree's library, as `selfhost/library_source.hero` holds it: the shape the
# digest's mirror reads.
LIBRARY = 'constant LINES: [str]\n    [\n        "# the library"\n        "function x()"\n    ]\n'


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
    write(os.path.join(root, "selfhost", "library_source.hero"), LIBRARY, T0)
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
        built_from(root)
    return root


def built_from(tree):
    """The stand-in in `tree` made to say it was compiled from the tree's
    sources as they stand (panel 222's digest, `provenance.py`)."""
    write(os.path.join(tree, "sources"), provenance.tree_sources(tree) + "\n")


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
    built_from(lane)
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


class Isolated(unittest.TestCase):
    """The hooks run under `python3 -I` too, which leaves their directory off
    `sys.path`: both failed at their first import of a module beside them."""

    def test_the_guard_refuses_under_an_isolated_python(self):
        event = json.dumps({"cwd": HOOKS, "tool_input": {"command": "git add -A"}})
        done = subprocess.run(
            [sys.executable, "-I", os.path.join(HOOKS, "guard_bash.py")],
            input=event, capture_output=True, text=True, timeout=60,
        )
        self.assertEqual(done.returncode, 2, done.stderr)
        self.assertIn("git add", done.stderr)

    def test_the_write_hook_runs_under_an_isolated_python(self):
        event = json.dumps({"cwd": HOOKS, "tool_input": {"file_path": os.path.join(HOOKS, "absent.hero")}})
        done = subprocess.run(
            [sys.executable, "-I", os.path.join(HOOKS, "fmt_check.py")],
            input=event, capture_output=True, text=True, timeout=60,
        )
        self.assertEqual(done.returncode, 0, done.stderr)
        self.assertEqual(done.stderr, "")


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

    def age(self, tree):
        """`tree`'s compiler made one built from other sources than the tree's:
        its root moved after it was built (panel 222's digest)."""
        write(os.path.join(tree, "selfhost", "main.hero"), "function main()\n    return\n# moved\n", T0 + 200)

    def test_a_lane_run_from_a_trunk_session_is_judged_by_the_lane(self):
        self.age(self.trunk)
        said = guard_bash.verdict("cd " + self.lane + " && ./heroes run tests/harness/main.hero -- ./heroes records", self.trunk)
        self.assertIsNone(said)

    def test_an_old_lane_compiler_is_refused_from_a_trunk_session(self):
        self.age(self.lane)
        said = guard_bash.verdict("cd " + self.lane + " && ./heroes run tests/harness/main.hero -- ./heroes records", self.trunk)
        self.assertIsNotNone(said)
        self.assertIn(provenance.tree_sources(self.lane), said)
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


class Provenance(unittest.TestCase):
    """Panel 222: a harness run's compiler is judged by what it says it was
    compiled from, against the tree's key, never by its file's date. Each test
    is one of the sitting's measured cases, or a coverage it ratified."""

    def setUp(self):
        self.tree = make_tree(os.path.join(fresh("hooks-222-"), "tree"))
        self.run = "./heroes run tests/harness/main.hero -- ./heroes records"

    def test_a_copy_built_before_an_edit_is_refused_whatever_its_date(self):
        write(os.path.join(self.tree, "selfhost", "main.hero"), "function main()\n    print(1)\n", T0 + 50)
        date(os.path.join(self.tree, "heroes"), T0 + 10_000)
        said = guard_bash.verdict(self.run, self.tree)
        self.assertIsNotNone(said)
        self.assertIn("keys " + provenance.tree_sources(self.tree), said)

    def test_sources_rewritten_with_the_same_bytes_pass(self):
        main = os.path.join(self.tree, "selfhost", "main.hero")
        with open(main, encoding="utf-8") as handle:
            same = handle.read()
        write(main, same, T0 + 10_000)
        self.assertIsNone(guard_bash.verdict(self.run, self.tree))

    def test_a_compiler_that_says_no_sources_is_refused(self):
        os.remove(os.path.join(self.tree, "sources"))
        said = guard_bash.verdict(self.run, self.tree)
        self.assertIsNotNone(said)
        self.assertIn("prints no `sources` line", said)

    def test_a_runtime_edit_is_refused(self):
        write(os.path.join(self.tree, "runtime", "runtime.c"), "/* runtime, moved */\n")
        self.assertIsNotNone(guard_bash.verdict(self.run, self.tree))

    def test_a_runtime_part_edit_is_refused(self):
        write(os.path.join(self.tree, "runtime", "runtime.c"), '#include "parts/a.c"\n')
        write(os.path.join(self.tree, "runtime", "parts", "a.c"), "int a;\n")
        built_from(self.tree)
        self.assertIsNone(guard_bash.verdict(self.run, self.tree))
        write(os.path.join(self.tree, "runtime", "parts", "a.c"), "int b;\n")
        self.assertIsNotNone(guard_bash.verdict(self.run, self.tree))

    def test_a_library_edit_is_refused(self):
        write(os.path.join(self.tree, "selfhost", "library_source.hero"), LIBRARY.replace("function x()", "function y()"))
        self.assertIsNotNone(guard_bash.verdict(self.run, self.tree))

    def test_a_module_the_root_reaches_is_covered_and_one_it_does_not_is_not(self):
        write(os.path.join(self.tree, "selfhost", "unused.hero"), "function u()\n    return\n")
        self.assertIsNone(guard_bash.verdict(self.run, self.tree))
        write(os.path.join(self.tree, "selfhost", "main.hero"), "use sub/used  # the one it reaches\n\nfunction main()\n    return\n")
        write(os.path.join(self.tree, "selfhost", "sub", "used.hero"), "function v()\n    return\n")
        built_from(self.tree)
        self.assertIsNone(guard_bash.verdict(self.run, self.tree))
        write(os.path.join(self.tree, "selfhost", "sub", "used.hero"), "function w()\n    return\n")
        self.assertIsNotNone(guard_bash.verdict(self.run, self.tree))

    def test_the_refusal_names_both_keys_the_stages_and_the_arbiter(self):
        said_before = provenance.compiler_said(os.path.join(self.tree, "heroes"), self.tree)[0]
        write(os.path.join(self.tree, "selfhost", "main.hero"), "function main()\n    print(2)\n")
        said = guard_bash.verdict(self.run, self.tree)
        self.assertIn("compiled from sources " + said_before, said)
        self.assertIn("keys " + provenance.tree_sources(self.tree), said)
        self.assertIn("./heroes build selfhost/main.hero -o heroes-next", said)
        self.assertIn("build selfhost/main.hero --emit-c", said)
        self.assertIn("the mirror is the one that is wrong", said)

    def test_a_tree_the_mirror_cannot_key_is_refused_with_the_arbiter(self):
        with open(os.path.join(self.tree, "selfhost", "main.hero"), "wb") as handle:
            handle.write(b"function main()\n    print(\"\xff\")\n")
        said = guard_bash.verdict(self.run, self.tree)
        self.assertIn("is not UTF-8", said)
        self.assertIn("build selfhost/main.hero --emit-c", said)


class Digest(unittest.TestCase):
    """The mirror's key is `cli/digest.hero`'s, computed a chunk at a time:
    held here to the recurrence the compiler runs byte by byte."""

    @staticmethod
    def by_byte(data):
        a = b = 0
        for v in data:
            a = (a * 131 + v) % provenance.M
            b = (b * 137 + v) % provenance.M
        return "%08x%08x" % (a, b)

    def test_every_length_about_a_chunk_keys_as_the_recurrence_does(self):
        n = provenance.CHUNK
        for size in (0, 1, 2, n - 1, n, n + 1, 3 * n + 7):
            data = bytes((i * 97 + 13) % 256 for i in range(size))
            self.assertEqual(provenance.digest(data), self.by_byte(data), size)

    def test_a_use_line_is_read_as_the_compiler_reads_its_tokens(self):
        text = "use a\nuse sub/helper  # a comment\nuse x/y as z\n  use indented\nuser = 1\n# use not\nuse a / b\nuse\n"
        self.assertEqual(provenance.uses_of(text), ["a", "sub/helper", "x/y", "indented", "a"])

    def test_a_string_literal_reads_the_seven_escapes(self):
        self.assertEqual(provenance.unescaped('a\\n\\t\\r\\\\\\"\\\'\\u{e9}', "x"), "a\n\t\r\\\"'\u00e9")
        with self.assertRaises(provenance.Unmirrored):
            provenance.unescaped("\\q", "x")


class MirrorHeldToCompiler(unittest.TestCase):
    """Panel 222's R2: the mirror is held to a real compiler, this tree's own,
    over a program whose `use` lines take every spelling the language writes:
    a name, a path, a path behind a comment, a path under another name, and a
    nested module's own `use`, resolved from the root's directory."""

    PROGRAM = {
        "main.hero": "use a\nuse sub/helper  # the helper\nuse sub/x as y\n\nfunction main()\n    print(a.one() + helper.two() + y.three())\n",
        "a.hero": "function one() -> i64\n    return 1\n",
        "sub/helper.hero": "use sub/deep\n\nfunction two() -> i64\n    return deep.four() - 2\n",
        "sub/deep.hero": "function four() -> i64\n    return 4\n",
        "sub/x.hero": "function three() -> i64\n    return 3\n",
    }

    def test_the_mirror_keys_a_program_as_the_compiler_writes_it(self):
        tree = os.path.dirname(os.path.dirname(HOOKS))
        compiler = os.path.join(tree, "heroes")
        if not os.access(compiler, os.X_OK) or provenance.compiler_said(compiler, tree)[0] is None:
            self.skipTest("no compiler of this tree that prints its sources: build it to hold the mirror to it")
        top = fresh("hooks-222-held-")
        for rel, text in self.PROGRAM.items():
            write(os.path.join(top, rel), text)
        env = dict(os.environ, HEROES_RUNTIME=os.path.join(tree, "runtime"))
        done = subprocess.run([compiler, "build", "main.hero", "--emit-c"], cwd=top, env=env, capture_output=True, text=True, timeout=300)
        self.assertEqual(done.returncode, 0, done.stderr)
        head = 'hero_program_sources(void) { return hero_str_from_bytes("'
        at = done.stdout.find(head)
        self.assertGreaterEqual(at, 0, "the program's C carries no key")
        written = done.stdout[at + len(head):at + len(head) + 16]
        mirrored = provenance.sources_key(
            os.path.join(top, "main.hero"),
            os.path.join(tree, "selfhost", "library_source.hero"),
            os.path.join(tree, "runtime"),
        )
        self.assertEqual(
            mirrored, written,
            "the mirror and " + compiler + " key one program apart; the compiler is the arbiter, unless it "
            "was built from another library than the tree's (`--version`'s line against its own `--emit-c`)",
        )


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


class Carried(unittest.TestCase):
    """Defect 515: what a commit carries is judged, not the index's list. A
    pathspec commit takes each named file as the working tree holds it,
    staged or not; a commit of the whole index takes each file as the index
    holds it; each witnessed against what git then commits."""

    def setUp(self):
        self.top = fresh("hooks-515-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        staged(self.tree, "examples/x.hero", "fine\n")
        staged(self.tree, "selfhost/big.hero", "x\n" * 10)
        git(self.tree, "commit", "-q", "-m", "x", "--", "examples/x.hero", "selfhost/big.hero")

    def said(self, command, cwd=None):
        return guard_bash.verdict(command, cwd or self.tree)

    def test_a_file_never_staged_is_judged_when_its_path_is_committed(self):
        write(os.path.join(self.tree, "examples", "x.hero"), "BROKEN\n")
        for command in ("git commit -F m -- examples/x.hero", "git commit -F m -- examples", "git commit -F m -- 'examples/*.hero'",
                        "git commit -F m -- examples/*.hero", "git commit -F m -- examples/x.hero $more", "cd examples && git commit -F m -- x.hero",
                        "git commit -i -F m -- examples/x.hero", "git commit --amend -F m -- examples/x.hero"):
            said = self.said(command)
            self.assertIsNotNone(said, command)
            self.assertIn("examples/x.hero does not parse", said)
            self.assertNotIn("as the index holds it", said)
        self.assertIsNone(self.said("git commit -F m -- selfhost"))

    def test_a_module_over_its_ceiling_and_never_staged_is_told_so(self):
        write(os.path.join(self.tree, "selfhost", "big.hero"), "x\n" * 301)
        said = self.said("git commit -F m -- selfhost/big.hero")
        self.assertIsNotNone(said)
        self.assertIn("selfhost/big.hero: 301 lines of code", said)
        self.assertIsNotNone(self.said("cd selfhost && git commit -F m -- big.hero"))

    def test_a_path_committed_is_judged_as_the_working_tree_holds_it(self):
        staged(self.tree, "examples/x.hero", "BROKEN\n")
        write(os.path.join(self.tree, "examples", "x.hero"), "fine again\n")
        self.assertIsNone(self.said("git commit -F m -- examples/x.hero"))
        git(self.tree, "commit", "-q", "-m", "x", "--", "examples/x.hero")
        self.assertEqual(git(self.tree, "show", "HEAD:examples/x.hero").stdout, "fine again\n")

    def test_a_staged_file_beside_the_paths_is_judged_as_the_index_holds_it(self):
        staged(self.tree, "examples/y.hero", "UNCANONICAL\n")
        write(os.path.join(self.tree, "examples", "y.hero"), "fine\n")
        said = self.said("git commit -i -F m -- selfhost/big.hero")
        self.assertIsNotNone(said)
        self.assertIn("examples/y.hero is not canonical as the index holds it", said)
        staged(self.tree, "selfhost/big.hero", "x\n" * 301)
        write(os.path.join(self.tree, "selfhost", "big.hero"), "x\n")
        said = self.said("git commit -i -F m -- examples/x.hero")
        self.assertIn("selfhost/big.hero: 301 lines of code in the version the index holds", said)

    def test_a_merge_concluded_takes_the_index_and_is_judged_by_it(self):
        write(os.path.join(self.tree, "a.txt"), "a\n")
        git(self.tree, "add", "--", "a.txt")
        git(self.tree, "commit", "-q", "-m", "a", "--", "a.txt")
        git(self.tree, "checkout", "-q", "-b", "other")
        write(os.path.join(self.tree, "a.txt"), "a other\n")
        staged(self.tree, "examples/y.hero", "fine\n")
        git(self.tree, "commit", "-q", "-m", "o", "--", "a.txt", "examples/y.hero")
        git(self.tree, "checkout", "-q", "main")
        write(os.path.join(self.tree, "a.txt"), "a main\n")
        git(self.tree, "commit", "-q", "-m", "m", "--", "a.txt")
        self.assertNotEqual(git(self.tree, "merge", "other", check=False).returncode, 0)
        staged(self.tree, "a.txt", "a resolved\n")
        write(os.path.join(self.tree, "examples", "y.hero"), "BROKEN\n")
        self.assertIsNone(self.said("git merge --continue"))
        staged(self.tree, "examples/y.hero", "BROKEN\n")
        write(os.path.join(self.tree, "examples", "y.hero"), "fine\n")
        for command in ("git merge --continue", "git commit --no-edit"):
            self.assertIn("examples/y.hero does not parse as the index holds it", self.said(command), command)
        git(self.tree, "commit", "-q", "--no-edit")
        self.assertEqual(git(self.tree, "show", "HEAD:examples/y.hero").stdout, "BROKEN\n")

    def test_a_marked_case_staged_unlike_its_working_tree_is_left_to_the_gate(self):
        write(os.path.join(self.tree, "tests", "harness", "suite_annotations.hero"), ANNOTATIONS)
        staged(self.tree, "tests/golden/check/w.expected", "x\n")
        staged(self.tree, "tests/golden/check/w.hero", "BROKEN  #~ unknown_escape\n")
        write(os.path.join(self.tree, "tests", "golden", "check", "w.hero"), "BROKEN  #~ unknown_escape, edited\n")
        self.assertIsNone(self.said("git commit -F m -- examples/x.hero"))
        self.assertEqual(runs(self.tree), [])

    def test_a_path_deleted_from_the_working_tree_is_not_judged(self):
        os.remove(os.path.join(self.tree, "examples", "x.hero"))
        self.assertIsNone(self.said("git commit -F m -- examples/x.hero"))

    def test_a_first_commit_reads_what_git_knows_under_the_paths(self):
        fresh_tree = os.path.join(self.top, "first")
        for part, text in (("seed/heroes.c", "/* seed */\n"), ("selfhost/main.hero", "function main()\n    return\n"), ("runtime/runtime.c", "/* runtime */\n")):
            write(os.path.join(fresh_tree, part), text, T0)
        heroes = write(os.path.join(fresh_tree, "heroes"), FAKE)
        os.chmod(heroes, 0o755)
        date(heroes, T0 + 100)
        git(fresh_tree, "init", "-q", "-b", "main", ".")
        staged(fresh_tree, "examples/z.hero", "fine\n")
        write(os.path.join(fresh_tree, "examples", "z.hero"), "BROKEN\n")
        said = guard_bash.verdict("git commit -F m -- examples/z.hero", fresh_tree)
        self.assertIsNotNone(said)
        self.assertIn("examples/z.hero does not parse", said)


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

    def test_the_anthropic_key_reaches_the_token_count_and_nothing_else(self):
        """Defect 670: the endpoint had only to appear somewhere in the
        segment, so a request to another one carrying its words passed."""
        count = "https://api.anthropic.com/v1/messages/count_tokens"
        self.passed("curl -s " + count + " -d @body.json")
        self.passed("curl -s '" + count + "?beta=true' -d @body.json")
        self.passed("grep -rn api.anthropic.com .claude/")
        self.refused("curl -s https://api.anthropic.com/v1/messages -d @body.json")
        self.refused("curl -s https://api.anthropic.com/v1/messages -d '{\"note\": \"/v1/messages/count_tokens\"}'")
        self.refused("curl -s https://api.anthropic.com/v1/messages " + count)
        self.refused("curl -s " + count + " https://api.anthropic.com/v1/organizations")
        self.refused("curl -s https://api.anthropic.com:443/v1/messages")
        self.refused("curl -s https://api.anthropic.com.example.org/v1/messages/count_tokens")
        self.refused("curl -s https://API.ANTHROPIC.COM/v1/messages")
        self.refused("curl -s " + count + "x")
        self.refused("python3 -c \"import urllib.request; urllib.request.urlopen('https://api.anthropic.com/v1/messages')\"")


def branched(tree, side_file="a.txt"):
    """`tree` holding `a.txt` and `b.txt`, a branch `other` that changes
    `a.txt` then `b.txt` in two commits, and `main` changing `side_file`, so a
    rebase of `other` onto `main` stops at its first pick when `side_file` is
    `a.txt`. `main` is checked out."""
    write(os.path.join(tree, "a.txt"), "a\n")
    write(os.path.join(tree, "b.txt"), "b\n")
    git(tree, "add", "--", "a.txt", "b.txt")
    git(tree, "commit", "-q", "-m", "ab", "--", "a.txt", "b.txt")
    git(tree, "checkout", "-q", "-b", "other")
    write(os.path.join(tree, "a.txt"), "a other\n")
    git(tree, "commit", "-q", "-m", "other1", "--", "a.txt")
    write(os.path.join(tree, "b.txt"), "b other\n")
    git(tree, "commit", "-q", "-m", "other2", "--", "b.txt")
    git(tree, "checkout", "-q", "main")
    write(os.path.join(tree, side_file), side_file + " main\n")
    git(tree, "add", "--", side_file)
    git(tree, "commit", "-q", "-m", "main1", "--", side_file)


def stopped(tree, how, extra=None):
    """`tree` stopped in `how`, its conflict resolved and staged, and `extra`,
    a file the stop never brought, staged beside it: `rebase` and
    `rebase-apply` (each backend) stop on `other`'s first pick, `am` on the
    first of `other`'s patches, `edit` at an `edit` of that pick."""
    branched(tree)
    if how == "am":
        patches = os.path.join(os.path.dirname(tree), os.path.basename(tree) + "-patches")
        git(tree, "format-patch", "-q", "-2", "-o", patches, "other")
        names = sorted(os.listdir(patches))
        done = git(tree, "am", *[os.path.join(patches, n) for n in names], check=False)
        assert done.returncode != 0, done.stdout + done.stderr
        staged(tree, "a.txt", "a other\n")
    elif how == "edit":
        git(tree, "checkout", "-q", "other")
        env = dict(GIT_ENV, GIT_SEQUENCE_EDITOR="sed -i.bak 's/^pick \\(.* other1\\)$/edit \\1/'")
        done = subprocess.run(["git", "rebase", "-i", "HEAD~2"], cwd=tree, env=env, capture_output=True, text=True)
        assert done.returncode == 0 and os.path.isdir(os.path.join(tree, ".git", "rebase-merge")), done.stderr
    else:
        git(tree, "checkout", "-q", "other")
        backend = ["--apply"] if how == "rebase-apply" else []
        done = git(tree, "rebase", *backend, "main", check=False)
        assert done.returncode != 0, done.stdout + done.stderr
        staged(tree, "a.txt", "a resolved\n")
    if extra is not None:
        staged(tree, extra, "not the stop's\n")


class Sequences(unittest.TestCase):
    """Defect 487: a rebase or an am concluded by `--continue` commits the
    whole index, so it is refused when the index holds a file it did not
    bring; and every spelling git reads as `--continue` is read."""

    def setUp(self):
        self.top = fresh("hooks-487-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)

    def refused(self, command):
        said = guard_bash.verdict(command, self.tree)
        self.assertIsNotNone(said, command)
        return said

    def passed(self, command):
        said = guard_bash.verdict(command, self.tree)
        self.assertIsNone(said, command)

    def test_a_rebase_continued_with_a_file_it_did_not_bring_is_refused(self):
        stopped(self.tree, "rebase", extra="c.txt")
        said = self.refused("GIT_EDITOR=true git rebase --continue")
        self.assertIn("this rebase", said)
        self.assertIn("c.txt", said)
        self.assertNotIn("a.txt", said)
        self.assertIn("git commit -- <paths>", said)

    def test_a_rebase_continued_with_its_own_files_passes(self):
        stopped(self.tree, "rebase")
        self.passed("GIT_EDITOR=true git rebase --continue")

    def test_a_rebase_by_the_apply_backend_is_read_by_its_patch(self):
        stopped(self.tree, "rebase-apply", extra="c.txt")
        self.assertIn("c.txt", self.refused("git rebase --continue"))
        self.refused("git am --continue")

    def test_an_edit_stop_continued_over_a_staged_file_is_refused(self):
        stopped(self.tree, "edit", extra="c.txt")
        said = self.refused("git rebase --continue")
        self.assertIn("--amend", said)
        self.passed("git commit --amend -F m -- c.txt")

    def test_an_am_continued_with_a_file_it_did_not_bring_is_refused_by_every_spelling(self):
        stopped(self.tree, "am", extra="c.txt")
        for spelling in ("--continue", "--resolved", "-r", "-qr", "--cont"):
            said = self.refused("git am " + spelling)
            self.assertIn("this am", said)
            self.assertIn("c.txt", said)
        self.passed("git am -p1r --continue-not")

    def test_an_am_continued_with_its_own_files_passes(self):
        stopped(self.tree, "am")
        self.passed("git am --continue")
        self.passed("git am -r")

    def test_a_patch_renaming_a_file_brings_its_old_name_and_its_new(self):
        branched(self.tree, side_file="b.txt")
        git(self.tree, "checkout", "-q", "-b", "move", "other~2")
        git(self.tree, "mv", "a.txt", "moved.txt")
        write(os.path.join(self.tree, "b.txt"), "b moved\n")
        git(self.tree, "add", "--", "b.txt")
        git(self.tree, "commit", "-q", "-m", "move", "--", "a.txt", "moved.txt", "b.txt")
        patches = os.path.join(self.top, "renames")
        git(self.tree, "format-patch", "-q", "-1", "-o", patches, "HEAD")
        git(self.tree, "checkout", "-q", "main")
        done = git(self.tree, "am", *[os.path.join(patches, n) for n in os.listdir(patches)], check=False)
        self.assertNotEqual(done.returncode, 0, done.stdout)
        # Applied by hand, as an am that stopped is: the move and b.txt.
        git(self.tree, "mv", "a.txt", "moved.txt")
        staged(self.tree, "b.txt", "b moved\n")
        held = git(self.tree, "diff", "--cached", "--name-only", "--no-renames").stdout.split()
        self.assertEqual(sorted(held), ["a.txt", "b.txt", "moved.txt"])
        self.passed("git am --continue")
        staged(self.tree, "c.txt", "not the patch's\n")
        self.assertIn("c.txt", self.refused("git am --continue"))

    def test_an_abbreviated_continue_is_read(self):
        conflicted(self.tree, "merge", extra="c.txt")
        self.assertIn("c.txt", self.refused("git merge --cont"))
        self.refused("git merge --con")

    def test_a_bare_commit_while_a_rebase_stands_keeps_its_refusal(self):
        stopped(self.tree, "rebase")
        self.assertIn("no `--`", self.refused("git commit --no-edit"))
        self.passed("git commit -F m -- a.txt")

    def test_a_continue_with_nothing_standing_gives_no_opinion(self):
        branched(self.tree)
        staged(self.tree, "c.txt", "c\n")
        self.passed("git rebase --continue")
        self.passed("git am --continue")

    def test_a_lane_rebase_is_read_from_its_own_tree(self):
        stopped(self.tree, "rebase", extra="c.txt")
        said = guard_bash.verdict("git -C " + self.tree + " rebase --continue", self.top)
        self.assertIsNotNone(said)
        self.assertIsNotNone(guard_bash.verdict("bash -c 'cd " + self.tree + " && git rebase --continue'", self.top))

    def test_a_rebase_redoing_a_merge_brings_what_the_merge_brings(self):
        write(os.path.join(self.tree, "a.txt"), "a\n")
        write(os.path.join(self.tree, "b.txt"), "b\n")
        write(os.path.join(self.tree, "d.txt"), "d\n")
        git(self.tree, "add", "--", "a.txt", "b.txt", "d.txt")
        git(self.tree, "commit", "-q", "-m", "abd", "--", "a.txt", "b.txt", "d.txt")
        git(self.tree, "checkout", "-q", "-b", "side")
        write(os.path.join(self.tree, "b.txt"), "b side\n")
        git(self.tree, "commit", "-q", "-m", "side1", "--", "b.txt")
        git(self.tree, "checkout", "-q", "-b", "other", "main")
        write(os.path.join(self.tree, "b.txt"), "b other\n")
        git(self.tree, "commit", "-q", "-m", "other1", "--", "b.txt")
        git(self.tree, "merge", "-q", "--no-edit", "side", check=False)
        staged(self.tree, "b.txt", "b merged\n")
        git(self.tree, "commit", "-q", "--no-edit")
        git(self.tree, "checkout", "-q", "main")
        write(os.path.join(self.tree, "d.txt"), "d main\n")
        git(self.tree, "commit", "-q", "-m", "main1", "--", "d.txt")
        git(self.tree, "checkout", "-q", "other")
        done = git(self.tree, "rebase", "--rebase-merges", "main", check=False)
        self.assertNotEqual(done.returncode, 0, done.stdout)
        self.assertTrue(os.path.isfile(os.path.join(self.tree, ".git", "MERGE_HEAD")))
        staged(self.tree, "b.txt", "b merged again\n")
        self.passed("GIT_EDITOR=true git rebase --continue")
        self.passed("git merge --continue")
        staged(self.tree, "c.txt", "not the merge's\n")
        self.assertIn("c.txt", self.refused("git rebase --continue"))
        self.assertIn("c.txt", self.refused("git merge --continue"))

    def test_a_root_commit_picked_brings_its_files(self):
        write(os.path.join(self.tree, "a.txt"), "a\n")
        git(self.tree, "add", "--", "a.txt")
        git(self.tree, "commit", "-q", "-m", "a", "--", "a.txt")
        git(self.tree, "checkout", "-q", "--orphan", "root")
        git(self.tree, "rm", "-q", "-r", "--cached", "--", ".")
        write(os.path.join(self.tree, "a.txt"), "a root\n")
        git(self.tree, "add", "--", "a.txt")
        git(self.tree, "commit", "-q", "-m", "root", "--", "a.txt")
        git(self.tree, "checkout", "-q", "-f", "main")
        done = git(self.tree, "cherry-pick", "root", check=False)
        self.assertNotEqual(done.returncode, 0, done.stdout)
        staged(self.tree, "a.txt", "a resolved\n")
        self.passed("git cherry-pick --continue")
        staged(self.tree, "c.txt", "not the pick's\n")
        self.assertIn("c.txt", self.refused("git cherry-pick --continue"))


def peer(tree, rel, text, stage=True):
    """Another session's change to `rel` in `tree`, staged or left unstaged."""
    path = write(os.path.join(tree, rel), text)
    if stage:
        git(tree, "add", "--", rel)
    return path


def read(path):
    try:
        with open(path, encoding="utf-8") as handle:
            return handle.read()
    except OSError:
        return None


class Discards(unittest.TestCase):
    """Defect 514: what throws work away is read as what commits it. An
    abort, a skip and a reset by `--merge` or `--hard` are refused where the
    tree holds a change they would throw away that the operation standing did
    not bring, and an autostash where the tree holds a change it would take;
    each witnessed against what git does."""

    def setUp(self):
        self.top = fresh("hooks-514-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        # The guard asks git's configuration as the command would, so this
        # machine's own is kept out of what the tests read.
        isolated = mock.patch.dict(os.environ, {"GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1"})
        isolated.start()
        self.addCleanup(isolated.stop)

    def refused(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNotNone(said, command)
        return said

    def passed(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNone(said, command)

    def test_a_merge_aborted_over_a_file_it_did_not_bring_is_refused(self):
        conflicted(self.tree, "merge", extra="c.txt")
        said = self.refused("git merge --abort")
        self.assertIn("c.txt", said)
        self.assertNotIn("a.txt", said)
        self.assertIn("the merge standing did not bring", said)
        self.assertIn("git restore --staged", said)
        for command in ("git merge --ab", "GIT_EDITOR=true git merge --abort", "git reset --merge", "git reset --hard"):
            self.assertIn("c.txt", self.refused(command))
        self.refused("git -C " + self.tree + " merge --abort", cwd=self.top)
        self.refused("bash -c 'cd " + self.tree + " && git merge --abort'", cwd=self.top)

    def test_a_merge_aborted_with_its_own_files_passes(self):
        conflicted(self.tree, "merge")
        self.passed("git merge --abort")
        self.passed("git reset --merge")
        self.passed("git reset --hard")
        self.passed("git merge --quit")

    def test_an_unstaged_change_is_left_by_an_abort_and_taken_by_a_hard_reset(self):
        conflicted(self.tree, "merge")
        kept = peer(self.tree, "runtime/runtime.c", "/* a peer's */\n", stage=False)
        self.passed("git merge --abort")
        self.passed("git reset --merge")
        self.assertIn("runtime/runtime.c", self.refused("git reset --hard"))
        git(self.tree, "merge", "--abort")
        self.assertEqual(read(kept), "/* a peer's */\n")

    def test_what_the_refusal_says_keeps_the_files_does(self):
        conflicted(self.tree, "merge", extra="c.txt")
        kept = peer(self.tree, "runtime/runtime.c", "/* a peer's */\n")
        said = self.refused("git merge --abort")
        self.assertIn("runtime/runtime.c", said)
        git(self.tree, "restore", "--staged", "--", "c.txt", "runtime/runtime.c")
        self.passed("git merge --abort")
        git(self.tree, "merge", "--abort")
        self.assertEqual(read(os.path.join(self.tree, "c.txt")), "not the operation's\n")
        self.assertEqual(read(kept), "/* a peer's */\n")

    def test_the_abort_refused_is_the_one_that_loses_the_file(self):
        conflicted(self.tree, "merge", extra="c.txt")
        self.refused("git merge --abort")
        git(self.tree, "merge", "--abort")
        self.assertIsNone(read(os.path.join(self.tree, "c.txt")))

    def test_a_pick_and_a_revert_are_ended_by_either_verb(self):
        conflicted(self.tree, "cherry-pick", extra="c.txt")
        for command in ("git cherry-pick --abort", "git cherry-pick --skip", "git revert --abort", "git revert --skip"):
            self.assertIn("c.txt", self.refused(command))
        self.passed("git merge --abort")
        self.passed("git am --abort")

    def test_a_conflicted_revert_is_read(self):
        write(os.path.join(self.tree, "a.txt"), "a\n")
        git(self.tree, "add", "--", "a.txt")
        git(self.tree, "commit", "-q", "-m", "a", "--", "a.txt")
        write(os.path.join(self.tree, "a.txt"), "a1\n")
        git(self.tree, "commit", "-q", "-m", "a1", "--", "a.txt")
        write(os.path.join(self.tree, "a.txt"), "a2\n")
        git(self.tree, "commit", "-q", "-m", "a2", "--", "a.txt")
        done = git(self.tree, "revert", "--no-edit", "HEAD~1", check=False)
        self.assertNotEqual(done.returncode, 0, done.stdout)
        staged(self.tree, "a.txt", "a resolved\n")
        self.passed("git revert --abort")
        staged(self.tree, "c.txt", "not the revert's\n")
        self.assertIn("c.txt", self.refused("git revert --abort"))
        self.assertIn("the revert standing", self.refused("git cherry-pick --skip"))

    def test_a_rebase_s_abort_and_skip_take_an_unstaged_change_too(self):
        stopped(self.tree, "rebase")
        self.passed("git rebase --abort")
        self.passed("git rebase --skip")
        kept = peer(self.tree, "runtime/runtime.c", "/* a peer's */\n", stage=False)
        said = self.refused("git rebase --abort")
        self.assertIn("runtime/runtime.c", said)
        self.assertIn("staged or not", said)
        self.assertIn("Unstaging does not keep them", said)
        self.refused("git rebase --sk")
        git(self.tree, "rebase", "--abort")
        self.assertEqual(read(kept), "/* runtime */\n")

    def test_a_rebase_by_the_apply_backend_is_ended_by_a_rebase_or_an_am(self):
        stopped(self.tree, "rebase-apply")
        peer(self.tree, "runtime/runtime.c", "/* a peer's */\n", stage=False)
        self.refused("git rebase --abort")
        self.passed("git am --abort")
        staged(self.tree, "c.txt", "not the patch's\n")
        self.assertIn("c.txt", self.refused("git am --skip"))

    def test_an_am_aborted_over_a_file_it_did_not_bring_is_refused(self):
        stopped(self.tree, "am", extra="c.txt")
        for command in ("git am --abort", "git am --skip", "git am --abo"):
            self.assertIn("c.txt", self.refused(command))
        self.passed("git rebase --abort")

    def test_with_nothing_standing_an_abort_is_git_s_to_refuse_and_a_reset_is_read(self):
        branched(self.tree)
        staged(self.tree, "c.txt", "c\n")
        for command in ("git merge --abort", "git rebase --abort", "git am --skip", "git cherry-pick --abort"):
            self.passed(command)
        for command in ("git reset --hard", "git reset --h", "git reset --merge", "git reset --me", "git reset --soft --hard", "git reset -q --hard HEAD"):
            said = self.refused(command)
            self.assertIn("c.txt", said)
            self.assertNotIn("standing did not bring", said)
        for command in ("git reset --keep", "git reset", "git reset --soft", "git reset --hard --soft", "git reset --m"):
            self.passed(command)

    def test_a_clean_tree_is_reset_freely(self):
        branched(self.tree)
        self.passed("git reset --hard")
        self.passed("git reset --hard main~1")
        self.passed("git checkout -f other")
        self.passed("git switch --discard-changes other")

    def test_a_forced_checkout_or_switch_of_the_whole_tree_is_a_hard_reset(self):
        branched(self.tree)
        staged(self.tree, "c.txt", "a peer's\n")
        kept = peer(self.tree, "runtime/runtime.c", "/* a peer's */\n", stage=False)
        for command in ("git checkout -f", "git checkout -f other", "git checkout -qf other", "git checkout --for other",
                        "git checkout -f HEAD", "git checkout -f other --", "git switch -f other", "git switch --discard-changes other",
                        "git switch --di other", "git switch -qf other"):
            said = self.refused(command)
            self.assertIn("c.txt", said, command)
            self.assertIn("runtime/runtime.c", said, command)
        for command in ("git checkout other", "git switch other", "git checkout -f runtime/runtime.c", "git checkout -f other -- a.txt",
                        "git checkout -f other a.txt", "git checkout -bfix", "git checkout -p other", "git switch --f other"):
            self.passed(command)
        git(self.tree, "checkout", "-q", "-f", "other")
        self.assertIsNone(read(os.path.join(self.tree, "c.txt")))
        self.assertEqual(read(kept), "/* runtime */\n")

    def test_an_autostash_over_a_change_is_refused_in_every_spelling(self):
        branched(self.tree)
        git(self.tree, "checkout", "-q", "other")
        staged(self.tree, "c.txt", "a peer's\n")
        said = self.refused("git rebase --autostash main")
        self.assertIn("c.txt", said)
        self.assertIn("--no-autostash", said)
        self.refused("git rebase --autost main")
        self.passed("git rebase --no-autostash main")
        self.passed("git rebase --autostash --no-autostash main")
        self.refused("git -c rebase.autoStash=true rebase main")
        self.refused("GIT_CONFIG_COUNT=1 GIT_CONFIG_KEY_0=rebase.autostash GIT_CONFIG_VALUE_0=true git rebase main")
        self.passed("git -c rebase.autoStash=true rebase --no-autostash main")
        self.passed("git rebase main")
        git(self.tree, "config", "rebase.autoStash", "true")
        self.assertIn("rebase.autoStash", self.refused("git rebase main"))
        self.passed("git rebase --continue")
        self.refused("git merge --autostash main")
        self.refused("git merge --au main")
        self.passed("git merge main")
        git(self.tree, "config", "merge.autoStash", "yes")
        self.refused("git merge main")
        self.passed("git merge --quit")

    def test_an_unstaged_change_is_stashed_too_and_a_clean_tree_is_not(self):
        branched(self.tree)
        git(self.tree, "checkout", "-q", "other")
        self.passed("git rebase --autostash main")
        peer(self.tree, "runtime/runtime.c", "/* a peer's */\n", stage=False)
        self.assertIn("runtime/runtime.c", self.refused("git rebase --autostash main"))

    def test_the_autostash_refused_is_the_one_that_takes_the_file(self):
        branched(self.tree)
        git(self.tree, "checkout", "-q", "other")
        staged(self.tree, "c.txt", "a peer's\n")
        self.refused("git rebase --autostash main")
        done = git(self.tree, "rebase", "--autostash", "main", check=False)
        self.assertNotEqual(done.returncode, 0, done.stdout)
        self.assertIsNone(read(os.path.join(self.tree, "c.txt")))

    def test_a_pull_reads_the_autostash_of_the_way_it_will_go(self):
        down = os.path.join(self.top, "down")
        git(self.top, "clone", "-q", self.tree, down)
        staged(down, "c.txt", "a peer's\n")
        for command in ("git pull --autostash", "git pull --au", "git -c merge.autoStash=true pull --no-rebase",
                        "git -c rebase.autoStash=true pull --rebase", "git -c rebase.autoStash=true pull -r",
                        "git -c rebase.autoStash=true pull --reb"):
            self.refused(command, cwd=down)
        for command in ("git pull", "git -c rebase.autoStash=true pull --no-rebase", "git -c merge.autoStash=true pull --rebase",
                        "git -c rebase.autoStash=true pull --rebase=false", "git pull --autostash --no-autostash"):
            self.passed(command, cwd=down)
        git(down, "config", "pull.rebase", "true")
        self.refused("git -c rebase.autoStash=true pull", cwd=down)
        git(down, "config", "branch.main.rebase", "false")
        self.passed("git -c rebase.autoStash=true pull", cwd=down)


def every_state(tree):
    """`tree` holding a path in each state a peer can leave one in, beside a
    branch `other` that changes `a.txt`, `d.txt` and `f.txt`: `a.txt` clean,
    `c.txt` a staged new file, `d.txt` a staged change, `e.txt` an unstaged
    one, `f.txt` a staged change changed again, `g.txt` a staged new file
    changed again, `h.txt` a staged deletion, `sub/s.txt` a staged change in
    a directory, and `my file.txt` a staged change with a space in its name."""
    for rel in ("a.txt", "d.txt", "e.txt", "f.txt", "h.txt", "sub/s.txt", "my file.txt"):
        write(os.path.join(tree, rel), rel + " head\n")
    git(tree, "add", "--", "a.txt", "d.txt", "e.txt", "f.txt", "h.txt", "sub/s.txt", "my file.txt")
    git(tree, "commit", "-q", "-m", "states", "--", "a.txt", "d.txt", "e.txt", "f.txt", "h.txt", "sub/s.txt", "my file.txt")
    git(tree, "checkout", "-q", "-b", "other")
    for rel in ("a.txt", "d.txt", "f.txt"):
        write(os.path.join(tree, rel), rel + " other\n")
    git(tree, "commit", "-q", "-m", "other", "--", "a.txt", "d.txt", "f.txt")
    git(tree, "checkout", "-q", "main")
    for rel in ("c.txt", "d.txt", "f.txt", "g.txt", "sub/s.txt", "my file.txt"):
        staged(tree, rel, rel + " staged\n")
    git(tree, "rm", "-q", "--", "h.txt")
    for rel in ("e.txt", "f.txt", "g.txt"):
        write(os.path.join(tree, rel), rel + " worktree\n")


def index_holds(tree, rel):
    """What the index of `tree` holds as `rel`, or None."""
    done = git(tree, "show", ":" + rel, check=False)
    return done.stdout if done.returncode == 0 else None


class Overwrites(unittest.TestCase):
    """Defect 529: the commands that write over what the index holds for the
    paths they name are refused where a path they name holds a staged change
    they would throw away, whosever: a restore of the index, a checkout from a
    commit, a forced removal or move, a reset of the index; each witnessed
    against what git does. A restore or a checkout of the working tree alone
    reaches no staged change, and is the hard stops' *asked for*."""

    def setUp(self):
        self.top = fresh("hooks-529-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        isolated = mock.patch.dict(os.environ, {"GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1"})
        isolated.start()
        self.addCleanup(isolated.stop)
        every_state(self.tree)

    def refused(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNotNone(said, command)
        self.assertIn("defect 529", said, command)
        return said

    def passed(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNone(said, command)

    def test_a_restore_of_the_index_and_the_working_tree_is_refused_in_every_spelling(self):
        said = self.refused("git restore -S -W -- c.txt d.txt e.txt a.txt")
        self.assertIn("c.txt, d.txt", said)
        self.assertNotIn("e.txt", said)
        self.assertNotIn("a.txt", said)
        for command in ("git restore --staged --worktree -- d.txt", "git restore -SW d.txt", "git restore -WS -- d.txt",
                        "git restore -qSW -- d.txt", "git restore --st --w -- d.txt", "git restore --sta --wor d.txt",
                        "git restore --source=other -S -W -- d.txt", "git restore -s other -SW -- d.txt",
                        "git restore -sother -SW -- d.txt", "git restore -SWs other -- d.txt", "git restore --so=other --staged --worktree d.txt",
                        "git restore --no-overlay -S -W -- d.txt", "git restore -S -W -- 'my file.txt'"):
            self.refused(command)
        self.assertIn("sub/s.txt", self.refused("git restore -S -W -- sub"))
        self.assertIn("sub/s.txt", self.refused("git restore -S -W -- s.txt", cwd=os.path.join(self.tree, "sub")))
        self.assertIn("d.txt", self.refused("git restore -S -W -- ../d.txt", cwd=os.path.join(self.tree, "sub")))
        for command in ("git restore -S -W .", "git restore -S -W :/", "git restore -S -W -- '*.txt'"):
            said = self.refused(command)
            for rel in ("c.txt", "d.txt", "f.txt", "g.txt", "h.txt"):
                self.assertIn(rel, said, command)
        self.refused("git -C " + self.tree + " restore -S -W -- d.txt", cwd=self.top)
        self.refused("bash -c 'cd " + self.tree + " && git restore -S -W -- d.txt'", cwd=self.top)

    def test_a_restore_git_refuses_or_that_names_no_staged_change_passes(self):
        for command in ("git restore -S -W -- a.txt e.txt", "git restore --s --w -- d.txt", "git restore -S -W --overlay -- c.txt",
                        "git restore -p -S -W -- d.txt", "git restore -S -W", "git restore -s other -SW -- a.txt"):
            self.passed(command)

    def test_a_restore_of_the_index_alone_is_refused_where_the_working_tree_holds_another_version(self):
        said = self.refused("git restore -S -- c.txt d.txt f.txt g.txt h.txt")
        self.assertIn("f.txt, g.txt", said)
        for rel in ("c.txt", "d.txt", "h.txt"):
            self.assertNotIn(rel, said)
        self.assertIn("git add --", said)
        self.refused("git restore --staged --source=other -- f.txt")
        for command in ("git restore --staged -- c.txt d.txt h.txt", "git restore --staged -- sub", "git restore -S --source=other -- d.txt"):
            self.passed(command)

    def test_a_restore_of_the_working_tree_alone_reaches_no_staged_change(self):
        for command in ("git restore -- d.txt e.txt f.txt g.txt", "git restore -W -- d.txt f.txt", "git restore --worktree .",
                        "git restore --source=HEAD -- c.txt d.txt f.txt g.txt", "git restore -s other -- d.txt f.txt", "git restore ."):
            self.passed(command)
        git(self.tree, "restore", "--", "d.txt", "f.txt", "g.txt")
        git(self.tree, "restore", "--source=HEAD", "--", "c.txt")
        for rel in ("c.txt", "d.txt", "f.txt", "g.txt"):
            self.assertEqual(index_holds(self.tree, rel), rel + " staged\n", rel)

    def test_a_checkout_from_a_commit_is_refused_over_a_staged_change(self):
        said = self.refused("git checkout HEAD -- d.txt e.txt a.txt")
        self.assertIn("d.txt", said)
        self.assertNotIn("e.txt", said)
        for command in ("git checkout HEAD d.txt", "git checkout @ -- d.txt", "git checkout main -- f.txt", "git checkout other -- d.txt f.txt",
                        "git checkout other d.txt", "git checkout -f HEAD -- d.txt", "git checkout -q HEAD -- d.txt", "git checkout HEAD -q -- d.txt",
                        "git checkout HEAD^{tree} -- d.txt", "git checkout HEAD -- h.txt"):
            self.refused(command)
        self.assertIn("sub/s.txt", self.refused("git checkout HEAD -- s.txt", cwd=os.path.join(self.tree, "sub")))
        for command in ("git checkout HEAD -- .", "git checkout HEAD ."):
            said = self.refused(command)
            self.assertIn("d.txt", said)
            self.assertNotIn("c.txt", said)
            self.assertNotIn("g.txt", said)
        said = self.refused("git checkout --no-overlay HEAD -- .")
        self.assertIn("c.txt", said)
        self.assertIn("g.txt", said)
        self.refused("git checkout --no-overl HEAD -- d.txt")

    def test_a_checkout_from_the_index_or_of_a_branch_or_that_git_refuses_passes(self):
        for command in ("git checkout -- d.txt e.txt f.txt g.txt", "git checkout -f -- d.txt f.txt", "git checkout -- .", "git checkout .",
                        "git checkout d.txt", "git checkout other", "git checkout -b fix", "git checkout -b fix HEAD",
                        "git checkout HEAD -- a.txt e.txt", "git checkout other -- a.txt", "git checkout HEAD -- c.txt",
                        "git checkout -p HEAD -- d.txt", "git checkout --ov HEAD -- d.txt", "git checkout nowhere -- d.txt",
                        "git checkout HEAD other -- d.txt", "git checkout -m -- d.txt", "git checkout --ours -- d.txt"):
            self.passed(command)

    def test_a_forced_removal_is_refused_over_a_staged_change(self):
        said = self.refused("git rm -f -- a.txt c.txt d.txt e.txt")
        self.assertIn("c.txt, d.txt", said)
        self.assertNotIn("a.txt", said)
        for command in ("git rm -q --force c.txt", "git rm --f d.txt", "git rm --fo -- d.txt", "git rm -qf d.txt", "git rm -rf sub",
                        "git rm -fr -- sub", "git rm -r --force -- '*.txt'", "git rm -f -q -- 'my file.txt'"):
            self.refused(command)
        said = self.refused("git rm --cached -f -- c.txt d.txt f.txt g.txt")
        self.assertIn("f.txt, g.txt", said)
        self.assertNotIn("c.txt", said)
        self.refused("git rm --ca --f -- f.txt")
        for command in ("git rm -- d.txt", "git rm -f -- a.txt e.txt", "git rm --cached -- c.txt d.txt f.txt", "git rm --cached -f -- c.txt d.txt",
                        "git rm -n -f -- d.txt", "git rm -f --dry-run -- d.txt", "git rm -nf d.txt", "git rm -f --no-force -- d.txt",
                        "git rm -f -- h.txt"):
            self.passed(command)

    def test_a_reset_of_the_index_is_refused_where_the_working_tree_holds_another_version(self):
        for command in ("git reset -- f.txt", "git reset f.txt", "git reset -q HEAD -- f.txt g.txt", "git reset HEAD f.txt", "git reset --mixed -- f.txt",
                        "git reset --mi -- f.txt", "git reset other -- f.txt", "git reset -N -- f.txt", "git reset", "git reset -q", "git reset --keep",
                        "git reset --ke", "git reset HEAD", "git reset other", "git reset -- :/", "git reset -- ."):
            said = self.refused(command)
            self.assertIn("f.txt", said, command)
        said = self.refused("git reset")
        self.assertIn("f.txt, g.txt", said)
        for rel in ("c.txt", "d.txt", "h.txt", "sub/s.txt"):
            self.assertNotIn(rel, said)
        for command in ("git reset -- c.txt d.txt h.txt", "git reset other -- d.txt", "git reset --soft", "git reset -p", "git reset --m",
                        "git reset -- a.txt e.txt"):
            self.passed(command)

    def test_a_forced_move_over_a_staged_change_is_refused(self):
        said = self.refused("git mv -f a.txt d.txt")
        self.assertIn("d.txt", said)
        self.assertNotIn("a.txt", said)
        self.refused("git mv --force -v a.txt f.txt")
        write(os.path.join(self.tree, "dir", "d.txt"), "dir d\n")
        git(self.tree, "add", "--", "dir/d.txt")
        git(self.tree, "commit", "-q", "-m", "dir", "--", "dir/d.txt")
        staged(self.tree, "dir/d.txt", "dir d staged\n")
        self.assertIn("dir/d.txt", self.refused("git mv -f d.txt dir"))
        for command in ("git mv a.txt d.txt", "git mv -n -f a.txt d.txt", "git mv -f d.txt z.txt", "git mv -f a.txt sub", "git mv -f a.txt"):
            self.passed(command)

    def test_the_refused_commands_are_the_ones_that_lose_the_change(self):
        cases = (
            ("git restore -S -W -- d.txt", "d.txt"), ("git restore -S -- f.txt", "f.txt"), ("git checkout HEAD -- d.txt", "d.txt"),
            ("git rm -q -f -- c.txt", "c.txt"), ("git rm -q --cached -f -- g.txt", "g.txt"), ("git reset -q -- f.txt", "f.txt"),
            ("git reset -q", "g.txt"), ("git mv -f a.txt d.txt", "d.txt"),
        )
        for at, (command, rel) in enumerate(cases):
            tree = make_tree(os.path.join(self.top, "witness-" + str(at)), repo=True)
            every_state(tree)
            self.refused(command, cwd=tree)
            subprocess.run(command, shell=True, cwd=tree, env=GIT_ENV, capture_output=True, check=True)
            self.assertNotEqual(index_holds(tree, rel), rel + " staged\n", command)
            self.assertNotEqual(read(os.path.join(tree, rel)), rel + " staged\n", command)

    def test_what_the_refusal_says_keeps_the_files_does(self):
        said = self.refused("git restore -S -W -- d.txt")
        self.assertIn("git restore --staged --", said)
        self.passed("git restore --staged -- d.txt")
        git(self.tree, "restore", "--staged", "--", "d.txt")
        self.assertEqual(read(os.path.join(self.tree, "d.txt")), "d.txt staged\n")
        said = self.refused("git reset -- f.txt")
        self.assertIn("git add --", said)
        git(self.tree, "add", "--", "f.txt")
        self.passed("git reset -- f.txt")
        git(self.tree, "reset", "-q", "--", "f.txt")
        self.assertEqual(read(os.path.join(self.tree, "f.txt")), "f.txt worktree\n")

    def test_paths_the_text_cannot_read_are_judged_as_every_path(self):
        for command in ('git restore -S -W -- "$f"', "git checkout HEAD -- $(cat list)", "git rm -f -- `cat list`",
                        "git restore -S -W -- {a,e}.txt", "git restore -S -W --pathspec-from-file=-", "git checkout HEAD -- ~/x"):
            said = self.refused(command)
            self.assertIn("every path", said, command)
        for command in ("git checkout $rev -- d.txt", "git checkout $rev d.txt", 'git reset "$rev" -- f.txt'):
            self.assertIn("cannot read", self.refused(command))
        for command in ("git checkout $rev -- a.txt", "git checkout $rev", "git reset $rev -- d.txt"):
            self.passed(command)
        self.assertIn("cannot read that commit", self.refused('git restore -S -W --source="$rev" -- d.txt'))
        # The words `xargs` adds are not in the text either.
        for command in ("printf 'a.txt\\n' | xargs git rm -f", "xargs -0 git restore -S -W --", "xargs git checkout HEAD --",
                        "xargs git reset", "xargs -I{} git rm -f {}", "xargs git mv -f a.txt"):
            self.assertIn("the words `xargs` adds", self.refused(command), command)
        for command in ("xargs git checkout --", "xargs git rm", "xargs git restore --", "xargs git rm -n -f"):
            self.passed(command)
        write(os.path.join(self.tree, "list"), "a.txt\nd.txt\n")
        self.assertIn("d.txt", self.refused("git restore -S -W --pathspec-from-file=list"))
        self.assertIn("d.txt", self.refused("git checkout --pathspec-from-file list HEAD"))
        write(os.path.join(self.tree, "list"), "a.txt\ne.txt\n")
        self.passed("git restore -S -W --pathspec-from-file=list")
        write(os.path.join(self.tree, "nul"), "a.txt\0d.txt\0")
        self.assertIn("d.txt", self.refused("git rm -f --pathspec-from-file=nul --pathspec-file-nul"))
        self.assertIn("every path", self.refused("git rm -f --pathspec-from-file=missing"))
        # A list the same command line writes is read as every path: what it
        # holds now is not what git will read.
        write(os.path.join(self.tree, "list"), "a.txt\n")
        self.assertIn("every path", self.refused("printf 'd.txt\\n' > list && git restore -S -W --pathspec-from-file=list"))

    def test_while_an_operation_stands_its_own_files_are_its_to_resolve(self):
        tree = make_tree(os.path.join(self.top, "merging"), repo=True)
        conflicted(tree, "merge", extra="c.txt")
        for command in ("git checkout other -- a.txt", "git checkout MERGE_HEAD -- a.txt b.txt", "git restore -S -W -- a.txt",
                        "git restore --source=other -SW -- a.txt", "git checkout --theirs -- a.txt", "git checkout -m -- a.txt"):
            self.passed(command, cwd=tree)
        said = self.refused("git restore -S -W -- a.txt c.txt", cwd=tree)
        self.assertIn("c.txt", said)
        self.assertNotIn("a.txt", said)
        self.assertIn("the merge standing did not bring", said)

    def test_a_first_commit_s_staged_files_are_read_with_no_head(self):
        tree = os.path.join(self.top, "unborn")
        os.makedirs(tree)
        git(tree, "init", "-q", "-b", "main", ".")
        staged(tree, "x.txt", "x staged\n")
        self.assertIn("x.txt", self.refused("git rm -f -- x.txt", cwd=tree))
        self.passed("git reset", cwd=tree)
        write(os.path.join(tree, "x.txt"), "x worktree\n")
        self.assertIn("x.txt", self.refused("git reset", cwd=tree))


class Runs(unittest.TestCase):
    """Defect 534: what a command runs that its first word does not say is
    judged as the command line it was given is: a git command a `find` runs
    for its files, and what a git alias stands for, asked of git; and, beside
    them with the same cause, a command after a shell's reserved word, inside a
    function, a `case` arm or a substitution, behind an escaped `;`, and after
    an `xargs` option that takes a value. Each witnessed against what the
    shell and git do."""

    def setUp(self):
        self.top = fresh("hooks-534-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        isolated = mock.patch.dict(os.environ, {"GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1"})
        isolated.start()
        self.addCleanup(isolated.stop)
        every_state(self.tree)

    def refused(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNotNone(said, command)
        return said

    def passed(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNone(said, command)

    def test_a_git_command_a_find_runs_is_judged(self):
        for command in ("find . -name d.txt -exec git rm -f {} \\;", "find . -name d.txt -exec git rm -f {} ';'",
                        "find . -name '*.txt' -exec git restore -S -W -- {} +", "find . -name d.txt -execdir git rm -f {} \\;",
                        "find . -name d.txt -ok git rm -f {} \\;", "find sub -name s.txt -okdir git checkout HEAD -- {} \\;",
                        "find . -name d.txt -exec env X=1 git rm -f {} \\;", "find . -type f -name 'd*' -exec git rm -f ./{} \\; -print"):
            self.assertIn("the words `find` puts for `{}`", self.refused(command), command)
        self.assertIn("`$1`", self.refused("find . -name d.txt -exec sh -c 'git rm -f \"$1\"' _ {} \\;"))
        for command in ("find . -maxdepth 0 -exec git reset --hard \\;", "find . -maxdepth 0 -exec git stash \\;",
                        "find . -maxdepth 0 -exec git commit -m x \\;", "find . -maxdepth 0 -exec git add -A \\; -print",
                        "find . -maxdepth 0 -exec true \\; -exec git add -A \\;"):
            self.refused(command)
        for command in ("find . -name '*.txt' -exec git add -- {} +", "find . -name d.txt -exec git rm -n -f {} \\;",
                        "find . -name d.txt -exec echo git rm -f {} \\;", "find . -name '*.txt' -exec grep -l staged {} +",
                        "find . -name d.txt -print", "find . -name d.txt -exec git diff -- {} +"):
            self.passed(command)

    def test_what_a_find_runs_is_what_was_refused(self):
        self.refused("find . -name d.txt -exec git rm -q -f {} \\;")
        subprocess.run("find . -name d.txt -exec git rm -q -f {} \\;", shell=True, cwd=self.tree, env=GIT_ENV, capture_output=True, check=True)
        self.assertIsNone(index_holds(self.tree, "d.txt"))
        self.assertIsNone(read(os.path.join(self.tree, "d.txt")))

    def test_a_backslash_is_read_as_the_shell_reads_it(self):
        self.assertEqual(len(guard_bash.segments("find . -exec git rm -f {} \\; -print")), 1)
        self.passed("echo a \\; git add -A")
        self.passed("git commit -m x \\\n  -- a.txt")
        self.refused("git rm -f \\\n  -- d.txt")
        self.refused("git add \\\n  -A")
        done = subprocess.run("echo a \\; git add -A", shell=True, cwd=self.tree, env=GIT_ENV, capture_output=True, text=True, check=True)
        self.assertEqual(done.stdout, "a ; git add -A\n")

    def test_xargs_reads_the_options_that_take_a_value(self):
        for command in ("printf 'd.txt\\n' | xargs -J % git rm -f %", "xargs -R 1 -I % git rm -f %", "xargs -S 255 -I % git rm -f %",
                        "xargs --max-procs 4 git rm -f", "xargs --arg-file list git rm -f", "xargs -0 -P 4 -n 1 git rm -f"):
            self.assertIn("the words `xargs` adds", self.refused(command), command)
        self.passed("xargs -J % git rm -n -f %")

    def test_a_reserved_word_a_function_a_case_or_a_substitution_runs_its_command(self):
        for command in ("if true; then git add -A; fi", "for f in d.txt; do git rm -f \"$f\"; done", "while false; do git stash; done",
                        "until true; do git stash; done", "{ git add -A; }", "f() { git reset --hard; }; f", "f () { git stash; }",
                        "function f { git stash; }", "case x in x) git add -A;; esac", "case x in y) true;; x) git stash;; esac",
                        "! git stash", "if git stash; then echo; fi", "echo $(git add -A)", "echo \"$(git stash)\"", "echo `git add -A`",
                        "x=$(git reset --hard)", "echo $(echo $(git stash))"):
            self.refused(command)
        for command in ("echo '$(git add -A)'", "echo \"\\$(git add -A)\"", "x=$((1+2)); echo $x", "echo 'if true; then git add -A; fi'",
                        "if git diff --quiet; then echo same; fi", "echo \"$(git rev-parse HEAD)\"", "echo 'a) git add -A'"):
            self.passed(command)

    def test_a_git_alias_is_judged_as_what_it_stands_for(self):
        for name, value in (("rr", "reset --hard"), ("rmf", "rm -f"), ("st", "stash"), ("ci", "commit"), ("nuke", "!git reset --hard"),
                            ("sh", "!f() { git rm -f \"$@\"; }; f"), ("a", "b"), ("b", "reset --hard"), ("p", "-p reset --hard"),
                            ("RS", "restore -S -W"), ("lg", "log --oneline")):
            git(self.tree, "config", "alias." + name, value)
        for command in ("git rr", "git RR", "git st", "git ci -m x", "git nuke", "git sh d.txt", "git a", "git p", "git rs -- d.txt",
                        "git -c alias.zz='reset --hard' zz", "GIT_CONFIG_COUNT=1 GIT_CONFIG_KEY_0=alias.yy GIT_CONFIG_VALUE_0='rm -f' git yy d.txt",
                        "find . -maxdepth 0 -exec git rr \\;", "xargs git rmf", "bash -c 'git rr'", "if true; then git rr; fi"):
            self.refused(command)
        said = self.refused("git rmf -- d.txt e.txt")
        self.assertIn("d.txt", said)
        self.assertNotIn("e.txt", said)
        self.refused("git -C " + self.tree + " rr", cwd=self.top)
        for command in ("git ci -m x -- a.txt", "git rmf -- a.txt", "git lg", "git rs -- a.txt e.txt"):
            self.passed(command)

    def test_an_alias_git_does_not_use_is_not_read(self):
        git(self.tree, "config", "alias.status", "reset --hard")
        git(self.tree, "config", "alias.loop", "!git loop")
        git(self.tree, "config", "alias.zap", "reset --hard")
        self.passed("git status")
        self.passed("git loop")
        self.refused("git zap")
        # A `git-<name>` on the PATH is the command git runs, never the alias.
        tools = os.path.join(self.top, "bin")
        os.makedirs(tools)
        os.chmod(write(os.path.join(tools, "git-zap"), "#!/bin/sh\nexit 0\n"), 0o755)
        with mock.patch.dict(os.environ, {"PATH": tools + os.pathsep + os.environ.get("PATH", "")}):
            self.passed("git zap")
            env = dict(GIT_ENV, PATH=os.environ["PATH"])
            subprocess.run(["git", "zap"], cwd=self.tree, env=env, capture_output=True, check=True)
        git(self.tree, "status")
        self.assertEqual(index_holds(self.tree, "c.txt"), "c.txt staged\n")

    def test_the_alias_refused_is_the_one_that_loses_the_change(self):
        git(self.tree, "config", "alias.rr", "reset --hard")
        self.refused("git rr")
        git(self.tree, "rr")
        self.assertIsNone(read(os.path.join(self.tree, "c.txt")))

    def test_a_gate_beside_a_commit_an_alias_or_a_find_runs_is_refused(self):
        git(self.tree, "config", "alias.ci", "commit")
        gate = "./heroes run tests/harness/main.hero -- ./heroes check"
        self.assertIn("a gate and a `git commit`", self.refused(gate + " && git ci -m x -- a.txt"))
        self.assertIn("a gate and a `git commit`", self.refused(gate + "; find . -maxdepth 0 -exec git commit -m x -- a.txt \\;"))

    def test_the_names_not_asked_about_are_git_s_own(self):
        # `runs.BUILTIN` is a premise about git: each name a builtin, which an
        # alias cannot replace. Asked of the git this machine runs.
        listed = git(self.tree, "--list-cmds=builtins").stdout.split()
        for name in sorted(what_runs.BUILTIN):
            self.assertIn(name, listed, name)


class Plumbing(unittest.TestCase):
    """Defect 535: `git read-tree` and `git update-index` write over a staged
    version as a reset of the index and a forced removal do, and are refused
    where one would be left nowhere, as `overwrites.py` reads those; each
    witnessed against what git does."""

    def setUp(self):
        self.top = fresh("hooks-535-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        isolated = mock.patch.dict(os.environ, {"GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1"})
        isolated.start()
        self.addCleanup(isolated.stop)
        every_state(self.tree)
        # A staged new file the disk no longer holds, the state `-m` and
        # `--remove` write over.
        staged(self.tree, "x.txt", "x.txt staged\n")
        os.remove(os.path.join(self.tree, "x.txt"))
        self.blob = git(self.tree, "rev-parse", "HEAD:a.txt").stdout.strip()

    def refused(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNotNone(said, command)
        return said

    def passed(self, command, cwd=None):
        said = guard_bash.verdict(command, cwd or self.tree)
        self.assertIsNone(said, command)

    def test_a_read_tree_writes_its_trees_over_the_index(self):
        for command in ("git read-tree HEAD", "git read-tree other", "git read-tree --reset HEAD", "git read-tree --empty",
                        "git read-tree", "git read-tree HEAD other", "git read-tree -q HEAD", "git read-tree -v HEAD",
                        "git read-tree --reset other", "git -C " + self.tree + " read-tree HEAD"):
            said = self.refused(command, cwd=self.top if " -C " in command else None)
            self.assertIn("defect 535", said, command)
            for rel in ("f.txt", "g.txt", "x.txt"):
                self.assertIn(rel, said, command)
            for rel in ("c.txt", "d.txt", "h.txt"):
                self.assertNotIn(rel, said, command)

    def test_a_read_tree_merging_one_tree_writes_over_what_the_disk_no_longer_holds(self):
        said = self.refused("git read-tree -m HEAD")
        self.assertIn("x.txt", said)
        self.assertIn("git checkout --", said)
        for rel in ("f.txt", "g.txt", "c.txt"):
            self.assertNotIn(rel, said)
        said = self.refused("git read-tree -m -u HEAD")
        for rel in ("c.txt", "d.txt", "x.txt"):
            self.assertIn(rel, said)
        said = self.refused("git read-tree --reset -u HEAD")
        self.assertIn("e.txt", said)
        self.assertIn("defect 514", said)

    def test_a_read_tree_that_loses_nothing_or_that_git_refuses_passes(self):
        for command in ("git read-tree -n HEAD", "git read-tree --index-output=alt HEAD", "git read-tree --prefix=p/ HEAD",
                        "git read-tree -m HEAD other", "git read-tree -m HEAD HEAD other", "git read-tree --reset HEAD other",
                        "git read-tree -u HEAD", "git read-tree -m --reset HEAD", "git read-tree --nonsense HEAD",
                        "git read-tree --empty HEAD", "git read-tree -m", "git read-tree --reset -u"):
            self.passed(command)

    def test_an_update_index_is_read_in_order(self):
        said = self.refused("git update-index --force-remove -- c.txt d.txt f.txt g.txt x.txt")
        for rel in ("f.txt", "g.txt", "x.txt"):
            self.assertIn(rel, said)
        for rel in ("c.txt", "d.txt"):
            self.assertNotIn(rel, said)
        self.assertNotIn("f.txt", self.refused("git update-index f.txt --force-remove g.txt"))
        for command in ("git update-index --force-rem g.txt", "git update-index --add --force-remove -- g.txt",
                        "printf 'a.txt\\n' | git update-index --force-remove --stdin", "git update-index --force-remove -z --stdin",
                        "find . -name g.txt -exec git update-index --force-remove {} +", "xargs git update-index --force-remove"):
            self.refused(command)
        said = self.refused("git update-index --remove -- x.txt f.txt g.txt")
        self.assertIn("x.txt", said)
        self.assertNotIn("f.txt", said)
        for command in ("git update-index --cacheinfo 100644," + self.blob + ",f.txt", "git update-index --cacheinfo 100644 " + self.blob + " f.txt",
                        "git update-index --cacheinfo=100644," + self.blob + ",g.txt", "git update-index --index-info"):
            self.refused(command)
        for command in ("git update-index --force-remove --no-force-remove g.txt", "git update-index -- f.txt g.txt",
                        "git update-index --add -- f.txt", "git update-index --force-remove -- c.txt d.txt h.txt",
                        "git update-index --remove -- f.txt", "git update-index --again", "git update-index -g --remove",
                        "git update-index --cacheinfo 100644," + self.blob + ",d.txt", "git update-index --refresh",
                        "git update-index --chmod=+x f.txt", "git update-index --nonsense g.txt", "git update-index --force-remove",
                        "printf 'a.txt\\n' | git update-index --stdin"):
            self.passed(command)

    def test_the_refused_plumbing_is_what_loses_the_change(self):
        cases = (
            ("git read-tree HEAD", "f.txt"), ("git read-tree -m HEAD", "x.txt"), ("git read-tree --empty", "g.txt"),
            ("git update-index --force-remove g.txt", "g.txt"), ("git update-index --remove x.txt", "x.txt"),
            ("git update-index --cacheinfo 100644," + self.blob + ",f.txt", "f.txt"),
        )
        for at, (command, rel) in enumerate(cases):
            tree = make_tree(os.path.join(self.top, "witness-" + str(at)), repo=True)
            every_state(tree)
            staged(tree, "x.txt", "x.txt staged\n")
            os.remove(os.path.join(tree, "x.txt"))
            if " -m " in command:
                # git refuses `-m` over a path the working tree holds otherwise
                # (`f.txt`, `g.txt`), and writes over the one it no longer holds.
                git(tree, "restore", "--staged", "--worktree", "--", "f.txt")
                git(tree, "rm", "-q", "--cached", "-f", "--", "g.txt")
            self.refused(command, cwd=tree)
            subprocess.run(command, shell=True, cwd=tree, env=GIT_ENV, capture_output=True, check=True)
            self.assertNotEqual(index_holds(tree, rel), rel + " staged\n", command)
            self.assertNotEqual(read(os.path.join(tree, rel)), rel + " staged\n", command)


REPO = os.path.dirname(os.path.dirname(HOOKS))
ZWSP = chr(0x200B)
RLO = chr(0x202E)
ESC = chr(0x1B)
ZWJ = chr(0x200D)


class Message(unittest.TestCase):
    """Defect 378: a commit's message is read for the characters the `unseen`
    suite refuses in a document, from that suite's own list."""

    def setUp(self):
        self.top = fresh("hooks-378-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        with open(os.path.join(REPO, "tests", "harness", "suite_unseen.hero"), encoding="utf-8") as handle:
            write(os.path.join(self.tree, "tests", "harness", "suite_unseen.hero"), handle.read())

    def said(self, command):
        return guard_bash.verdict(command, self.tree)

    def test_a_message_file_holding_a_zero_width_space_is_refused_with_its_place(self):
        write(os.path.join(self.tree, "msg.txt"), "Subject\n\nA body with" + ZWSP + " one.\n")
        said = self.said("git commit -F msg.txt -- a.hero")
        self.assertIsNotNone(said)
        self.assertIn("U+200B at line 3, column 12 of `msg.txt`", said)

    def test_a_message_given_with_m_is_read(self):
        said = self.said("git commit -m 'abc" + RLO + "def' -- a.hero")
        self.assertIsNotNone(said)
        self.assertIn("U+202E", said)
        self.assertIsNotNone(self.said("git commit --message='x" + ESC + "' -- a.hero"))
        self.assertIsNotNone(self.said("git commit -Fmsg.txt -m ok -m 'y" + ZWSP + "' -- a.hero"))

    def test_a_message_read_from_stdin_or_a_substitution_is_read_in_the_heredoc(self):
        self.assertIsNotNone(self.said("git commit -F - -- a.hero <<'EOF'\nSubject" + ZWSP + "\nEOF"))
        self.assertIsNotNone(self.said("git commit -m \"$(cat <<'EOF'\nSubject\n\nbody" + ESC + "\nEOF\n)\" -- a.hero"))

    def test_a_merge_and_a_tag_message_are_read(self):
        self.assertIsNotNone(self.said("git merge -m 'm" + ZWSP + "' lane-x"))
        self.assertIsNotNone(self.said("git tag -a v0 -m 't" + ZWSP + "'"))

    def test_a_clean_message_and_what_text_spells_with_pass(self):
        write(os.path.join(self.tree, "msg.txt"), "Subject\n\n\tA tab, a joiner " + ZWJ + " and an é.\n")
        self.assertIsNone(self.said("git commit -F msg.txt -- a.hero"))

    def test_the_list_is_the_suite_s_own(self):
        write(os.path.join(self.tree, "tests", "harness", "suite_unseen.hero"), 'constant REFUSED: [str]\n    [\n        "88 88"\n    ]\n')
        self.assertIsNotNone(self.said("git commit -m 'aXb' -- a.hero"))
        self.assertIsNone(self.said("git commit -m 'a" + ZWSP + "b' -- a.hero"))


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

CANONICAL = '''constant SOURCE_DIRS: [str]
    [
        "selfhost"
        "tests/harness"
        "examples"
        "tests/golden/run"
        "tests/golden/ir"
        "tests/golden/emit"
        "tests/golden/fixedbugs"
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


class NotCanonicalOnPurpose(unittest.TestCase):
    """Defect 581: a staged golden case that parses and is not canonical on
    purpose, as defect 576's two are, is judged by its marks whatever `fmt`
    answers; a case with no mark, and a program `canonical` reads, are still
    held to `fmt`."""

    def setUp(self):
        self.top = fresh("hooks-581-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        write(os.path.join(self.tree, "tests", "harness", "suite_annotations.hero"), ANNOTATIONS)

    def case(self, rel, marked=True, expected=True):
        staged(self.tree, rel, "x: i8 @ -(-128) UNCANONICAL" + ("  #~ int_out_of_range" if marked else "") + "\n")
        if expected:
            staged(self.tree, rel[: -len(".hero")] + ".expected", "x.hero:1:9: error[int_out_of_range]: ...\n")

    def commit(self, *rels):
        return guard_bash.verdict("git commit -F m -- " + " ".join(rels), self.tree)

    def test_a_marked_case_not_canonical_its_suite_passes_is_committed(self):
        answer(self.tree, "annotations", 0, "  annotations (only fixedbugs-576-a): 1 passed, 0 failed\n")
        self.case("tests/golden/check/fixedbugs-576-a.hero")
        self.assertIsNone(self.commit("tests/golden/check/fixedbugs-576-a.hero"))
        self.assertEqual(runs(self.tree), ["annotations fixedbugs-576-a"])

    def test_a_marked_case_not_canonical_its_suite_fails_is_refused_with_the_suite_words(self):
        answer(
            self.tree, "annotations", 1,
            "FAIL annotations/fixedbugs-576-a\n  the annotations and the diagnostics disagree\n"
            "  annotations (only fixedbugs-576-a): 0 passed, 1 failed\n",
        )
        self.case("tests/golden/full/fixedbugs-576-a.hero")
        said = self.commit("tests/golden/full/fixedbugs-576-a.hero")
        self.assertIsNotNone(said)
        self.assertIn("marks and its expectation disagree", said)
        self.assertNotIn("is not canonical", said)

    def test_a_case_with_no_mark_not_canonical_is_still_refused(self):
        answer(self.tree, "annotations", 0, "  annotations (only plain): 1 passed, 0 failed\n")
        self.case("tests/golden/check/plain.hero", marked=False)
        said = self.commit("tests/golden/check/plain.hero")
        self.assertIsNotNone(said)
        self.assertIn("tests/golden/check/plain.hero is not canonical", said)
        self.assertEqual(runs(self.tree), [])

    def test_a_marked_program_not_canonical_keeps_the_fmt_verdict(self):
        for rel in ("tests/golden/run/prog.hero", "tests/golden/fixedbugs/prog.hero"):
            self.case(rel)
            self.assertIn(rel + " is not canonical", self.commit(rel))
        self.assertEqual(runs(self.tree), [])

    def test_a_marked_case_not_canonical_with_no_expectation_keeps_the_fmt_verdict(self):
        self.case("tests/golden/check/lonely.hero", expected=False)
        self.assertIn("tests/golden/check/lonely.hero is not canonical", self.commit("tests/golden/check/lonely.hero"))

    def test_a_marked_case_not_canonical_staged_unlike_its_working_tree_is_left_to_the_gate(self):
        self.case("tests/golden/check/w.hero")
        write(os.path.join(self.tree, "tests", "golden", "check", "w.hero"), "x: i8 @ -(-128) UNCANONICAL  #~ int_out_of_range, edited\n")
        staged(self.tree, "examples/x.hero", "fine\n")
        self.assertIsNone(guard_bash.verdict("git commit -i -F m -- examples/x.hero", self.tree))
        self.assertEqual(runs(self.tree), [])

    def test_a_merge_carrying_marked_cases_not_canonical_is_concluded(self):
        # The shape that was refused: lane b18-close's merge of a lane whose
        # two cases of defect 576 are not canonical on purpose.
        answer(self.tree, "annotations", 0, "  annotations (only fixedbugs-576-): 2 passed, 0 failed\n")
        write(os.path.join(self.tree, "a.txt"), "a\n")
        git(self.tree, "add", "--", "a.txt", "tests/harness/suite_annotations.hero")
        git(self.tree, "commit", "-q", "-m", "a", "--", "a.txt", "tests/harness/suite_annotations.hero")
        git(self.tree, "checkout", "-q", "-b", "other")
        self.case("tests/golden/check/fixedbugs-576-a.hero")
        self.case("tests/golden/full/fixedbugs-576-b.hero")
        git(self.tree, "commit", "-q", "-m", "o", "--", "tests/golden")
        git(self.tree, "checkout", "-q", "main")
        write(os.path.join(self.tree, "a.txt"), "a main\n")
        git(self.tree, "commit", "-q", "-m", "m", "--", "a.txt")
        self.assertEqual(git(self.tree, "merge", "--no-commit", "--no-ff", "other", check=False).returncode, 0)
        for command in ("git commit --no-edit", "git merge --continue"):
            self.assertIsNone(guard_bash.verdict(command, self.tree), command)
        self.assertEqual(runs(self.tree), ["annotations fixedbugs-576-"] * 2)
        unmarked = os.path.join(self.tree, "tests", "golden", "check", "fixedbugs-576-a.hero")
        staged(self.tree, "tests/golden/check/fixedbugs-576-a.hero", "x: i8 @ -(-128) UNCANONICAL\n")
        self.assertTrue(os.path.isfile(unmarked))
        said = guard_bash.verdict("git commit --no-edit", self.tree)
        self.assertIsNotNone(said)
        self.assertIn("tests/golden/check/fixedbugs-576-a.hero is not canonical", said)

    # The write-time hook, the same rule through `marks.held_to_marks`: it
    # told 576's case *not canonical* when it was written.
    def written_case(self, rel, marked=True):
        write(os.path.join(self.tree, rel[: -len(".hero")] + ".expected"), "x.hero:1:9: error[int_out_of_range]: ...\n")
        text = "x: i8 @ -(-128) UNCANONICAL" + ("  #~ int_out_of_range" if marked else "") + "\n"
        return written(self.tree, write(os.path.join(self.tree, rel), text))

    def test_the_write_hook_judges_a_marked_case_not_canonical_by_its_marks(self):
        answer(self.tree, "annotations", 0, "  annotations (only fixedbugs-576-a): 1 passed, 0 failed\n")
        code, said = self.written_case("tests/golden/check/fixedbugs-576-a.hero")
        self.assertEqual((code, said), (0, ""))
        self.assertEqual(runs(self.tree), ["annotations fixedbugs-576-a"])

    def test_the_write_hook_tells_a_marked_case_not_canonical_its_suite_fails(self):
        answer(self.tree, "annotations", 1, "FAIL annotations/fixedbugs-576-a\n  disagree\n")
        code, said = self.written_case("tests/golden/full/fixedbugs-576-a.hero")
        self.assertEqual(code, 2, said)
        self.assertIn("marks and its expectation disagree", said)
        self.assertNotIn("is not canonical", said)

    def test_the_write_hook_still_tells_an_unmarked_case_or_a_program_not_canonical(self):
        answer(self.tree, "annotations", 0, "  annotations (only plain): 1 passed, 0 failed\n")
        code, said = self.written_case("tests/golden/check/plain.hero", marked=False)
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        code, said = self.written_case("tests/golden/run/prog.hero")
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)


class ProbeFixtures(unittest.TestCase):
    """Defect 603: a probe fixture under a run root the `canonical` suite does
    not read (`tests/golden/surface-fixtures/`) parses and is not canonical on
    purpose, so the commit guard leaves it to the suites that read it; a
    program of a run root that suite does read (`fixedbugs/`) and a source
    file keep `fmt`'s verdict, a fixture that does not parse is still refused,
    and without the suite's file to read the fixture is held to `fmt`, the
    loud direction."""

    def setUp(self):
        self.top = fresh("hooks-603-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)
        write(os.path.join(self.tree, "tests", "harness", "suite_annotations.hero"), ANNOTATIONS)
        write(os.path.join(self.tree, "tests", "harness", "suite_canonical.hero"), CANONICAL)

    def commit(self, *rels):
        return guard_bash.verdict("git commit -F m -- " + " ".join(rels), self.tree)

    def test_an_unmarked_fixture_not_canonical_is_committed(self):
        staged(self.tree, "tests/golden/surface-fixtures/comments101/pieces.hero", "x: [i64] = a UNCANONICAL\n")
        self.assertIsNone(self.commit("tests/golden/surface-fixtures/comments101/pieces.hero"))
        self.assertEqual(runs(self.tree), [])

    def test_a_program_the_canonical_suite_reads_keeps_the_fmt_verdict(self):
        staged(self.tree, "tests/golden/fixedbugs/prog.hero", "x = 1 UNCANONICAL\n")
        self.assertIn("tests/golden/fixedbugs/prog.hero is not canonical", self.commit("tests/golden/fixedbugs/prog.hero"))
        staged(self.tree, "examples/x.hero", "UNCANONICAL\n")
        self.assertIn("examples/x.hero is not canonical", self.commit("examples/x.hero"))

    def test_a_fixture_that_does_not_parse_is_still_refused(self):
        staged(self.tree, "tests/golden/surface-fixtures/comments101/broken.hero", "BROKEN\n")
        self.assertIn("does not parse", self.commit("tests/golden/surface-fixtures/comments101/broken.hero"))

    def test_without_the_canonical_suite_a_fixture_is_held_to_fmt(self):
        os.remove(os.path.join(self.tree, "tests", "harness", "suite_canonical.hero"))
        staged(self.tree, "tests/golden/surface-fixtures/comments101/pieces.hero", "x = a UNCANONICAL\n")
        self.assertIn("is not canonical", self.commit("tests/golden/surface-fixtures/comments101/pieces.hero"))

    def test_canonical_reads_places_a_file_by_the_suites_own_list(self):
        self.assertTrue(marks.canonical_reads(self.tree, "selfhost/x.hero"))
        self.assertTrue(marks.canonical_reads(self.tree, "tests/golden/run/p.hero"))
        self.assertFalse(marks.canonical_reads(self.tree, "tests/golden/surface-fixtures/a/b.hero"))
        self.assertFalse(marks.canonical_reads(self.tree, "tests/golden/check/c.hero"))


class Growth(unittest.TestCase):
    """Defect 215: a written `selfhost/` module that might grow a text by `+`
    is asked of `layout` narrowed to it; one that cannot asks nothing."""

    GROWN = 'function f(xs: [str]) -> str\n    beside: str @ ""\n\n    for ch in xs\n        beside @ beside + ch\n\n    return beside\n'

    def setUp(self):
        self.top = fresh("hooks-215-")
        self.tree = make_tree(os.path.join(self.top, "tree"))

    def test_a_module_growing_a_text_is_refused_with_the_suite_words(self):
        answer(self.tree, "layout", 1, "FAIL layout/concat\n  a text is grown by pushing\n    selfhost/g.hero:5: beside @ beside + ch\n")
        module = write(os.path.join(self.tree, "selfhost", "g.hero"), self.GROWN)
        code, said = written(self.tree, module)
        self.assertEqual(code, 2, said)
        self.assertIn("FAIL layout/concat", said)
        self.assertIn("selfhost/g.hero:5: beside @ beside + ch", said)
        self.assertEqual(runs(self.tree), ["layout selfhost/g.hero"])

    def test_a_module_the_suite_passes_is_quiet(self):
        answer(self.tree, "layout", 0, "  layout (only selfhost/g.hero): 3 passed, 0 failed\n")
        module = write(os.path.join(self.tree, "selfhost", "g.hero"), self.GROWN)
        code, said = written(self.tree, module)
        self.assertEqual(code, 0, said)

    def test_a_module_that_cannot_grow_a_text_asks_no_run(self):
        module = write(os.path.join(self.tree, "selfhost", "n.hero"), "function f(n: i64) -> i64\n    at: i64 @ 0\n    at @ at + 1\n    return at\n")
        code, said = written(self.tree, module)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), [])

    def test_the_prefilter_holds_every_shape_the_suite_refuses(self):
        import ceiling

        appends = ["l.tokens @ l.tokens.push("]
        for text in (
            self.GROWN,
            "function f(@out: str)\n    out @ out + \"x\"\n",
            '    p.page @ p.page + "line\\n"\n',
            "    l.tokens @ l.tokens.push(t)\n",
            "    c.tokens[i] @ t\n",
            "    c.tokens[i].kind @ .newline\n",
            "    kept(@c.tokens, 1)\n",
            "    r.out.diagnostics @ r.out.diagnostics.push(d)\n",
            'function f() -> str\n    out: str @ ""\n    match k\n        .a => out @ out + "a"\n',
        ):
            self.assertTrue(ceiling.might_grow(text, appends), text)
        for text in (
            "    at @ at + 1\n",
            "    p.page @ p.page + word\n",
            "    r.items @ r.items.push(x)\n",
            "    n = c.tokens.len()\n",
            "    t = c.tokens[i]\n",
        ):
            self.assertFalse(ceiling.might_grow(text, appends), text)


class EveryAnswer(unittest.TestCase):
    """Defect 493: a write is asked every question the hook can answer, and
    told every answer; a refusal never hides the ceiling, nor the questions
    after it, since the `heroes fmt --in-place` that answers *not canonical*
    is a shell command and runs no write hook."""

    def setUp(self):
        self.top = fresh("hooks-493-")
        self.tree = make_tree(os.path.join(self.top, "tree"))

    def module(self, name, last, lines=300):
        """A `selfhost/` module of `lines` lines of code then `last`."""
        return write(os.path.join(self.tree, "selfhost", name), "x\n" * lines + last + "\n")

    def test_a_module_not_canonical_is_told_its_ceiling_too(self):
        code, said = written(self.tree, self.module("big.hero", "UNCANONICAL"))
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        self.assertIn("selfhost/big.hero: 301 lines of code in its canonical form", said)

    def test_the_canonical_form_is_counted_not_the_text_as_written(self):
        code, said = written(self.tree, self.module("split.hero", "SPLIT", lines=299))
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        self.assertIn("301 lines of code in its canonical form", said)

    def test_a_module_that_does_not_check_is_told_its_ceiling_too(self):
        code, said = written(self.tree, self.module("big.hero", "UNCHECKED"))
        self.assertEqual(code, 2, said)
        self.assertIn("does not check", said)
        self.assertIn("selfhost/big.hero: 301 lines of code", said)

    def test_a_module_that_does_not_parse_is_told_its_ceiling_as_written(self):
        code, said = written(self.tree, self.module("big.hero", "BROKEN"))
        self.assertEqual(code, 2, said)
        self.assertIn("does not parse", said)
        self.assertIn("301 lines of code in the text as written", said)
        self.assertNotIn("check selfhost/main.hero", " ".join(calls(self.tree)))

    def test_a_module_refused_by_an_older_compiler_is_told_its_ceiling_too(self):
        write(os.path.join(self.tree, "selfhost", "lexer.hero"), "# moved\n", T0 + 200)
        code, said = written(self.tree, self.module("big.hero", "UNCANONICAL"))
        self.assertEqual(code, 2, said)
        self.assertIn("older than its tree", said)
        self.assertIn("selfhost/big.hero: 301 lines of code", said)

    def test_a_module_not_canonical_is_still_checked(self):
        code, said = written(self.tree, self.module("small.hero", "UNCANONICAL UNCHECKED", lines=3))
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        self.assertIn("does not check", said)
        self.assertIn(self.tree + "|check selfhost/main.hero", calls(self.tree))

    def test_a_module_not_canonical_is_still_asked_its_growth(self):
        answer(self.tree, "layout", 1, "FAIL layout/concat\n  a text is grown by pushing\n")
        module = write(os.path.join(self.tree, "selfhost", "g.hero"), Growth.GROWN + "# UNCANONICAL\n")
        code, said = written(self.tree, module)
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        self.assertIn("FAIL layout/concat", said)

    def test_a_case_not_canonical_is_still_asked_its_marks(self):
        write(os.path.join(self.tree, "tests", "harness", "suite_annotations.hero"), ANNOTATIONS)
        answer(self.tree, "annotations", 1, "FAIL annotations/w\n  disagree\n")
        write(os.path.join(self.tree, "tests", "golden", "check", "w.expected"), "x\n")
        case = write(os.path.join(self.tree, "tests", "golden", "check", "w.hero"), "UNCANONICAL\n")
        code, said = written(self.tree, case)
        self.assertEqual(code, 2, said)
        self.assertIn("is not canonical", said)
        self.assertIn("marks and its expectation disagree", said)

    def test_a_module_under_its_ceiling_and_canonical_is_quiet(self):
        code, said = written(self.tree, self.module("fine.hero", "y", lines=299))
        self.assertEqual((code, said), (0, ""))

    def test_a_spent_budget_asks_no_slow_question(self):
        import fmt_check
        import time

        kept = fmt_check.STARTED
        try:
            fmt_check.STARTED = time.monotonic() - fmt_check.marks.LIMIT
            self.assertIsNone(fmt_check.left())
            self.assertIsNone(fmt_check.ask("/bin/sleep", ["30"], self.top))
            fmt_check.STARTED = time.monotonic() - fmt_check.marks.LIMIT + fmt_check.FLOOR + 1
            started = time.monotonic()
            self.assertIsNone(fmt_check.ask("/bin/sleep", ["30"], self.top))
            self.assertLess(time.monotonic() - started, 20)
        finally:
            fmt_check.STARTED = kept


class GithubWrites(unittest.TestCase):
    """Defects 671 and 672, panel 212: a GitHub write, and the credential
    itself, are refused in the assistant's shell, whatever the permission
    files allow; a read is not."""

    def setUp(self):
        self.top = fresh("hooks-671-")
        self.tree = make_tree(os.path.join(self.top, "tree"))

    def said(self, command):
        return guard_bash.verdict(command, self.tree)

    def test_gh_api_with_a_field_is_a_post_and_is_refused(self):
        said = self.said("gh api repos/heroes-lang/heroes/issues -f title=x")
        self.assertIsNotNone(said)
        self.assertIn("POST", said)
        self.assertIn("a field or an input switches it to POST", said)
        self.assertIsNotNone(self.said("gh api repos/heroes-lang/heroes/issues -F title=x"))
        self.assertIsNotNone(self.said("gh api repos/heroes-lang/heroes/issues --raw-field title=x"))
        self.assertIsNotNone(self.said("gh api repos/heroes-lang/heroes/issues --input body.json"))
        self.assertIsNotNone(self.said("gh api -i repos/heroes-lang/heroes/issues --input -"))

    def test_gh_api_with_a_writing_method_is_refused_in_every_spelling(self):
        for spelling in ("-X POST", "-XPOST", "--method POST", "--method=patch", "-X DELETE", "-X PUT", "-X patch"):
            self.assertIsNotNone(self.said("gh api " + spelling + " repos/heroes-lang/heroes/issues/1"), spelling)
        self.assertIsNotNone(self.said("gh api repos/heroes-lang/heroes/labels/x -X DELETE"))

    def test_gh_api_reads_pass(self):
        self.assertIsNone(self.said("gh api repos/heroes-lang/heroes"))
        self.assertIsNone(self.said("gh api -i repos/heroes-lang/heroes --jq .full_name"))
        self.assertIsNone(self.said("gh api -X GET repos/heroes-lang/heroes/issues -f per_page=1"))
        self.assertIsNone(self.said("gh api --paginate repos/heroes-lang/heroes/issues?state=all"))
        self.assertIsNone(self.said("gh api rate_limit --jq '.resources.core'"))
        self.assertIsNone(self.said("gh api -H 'Accept: application/json' orgs/heroes-lang/issue-fields"))

    def test_the_markdown_renderer_and_a_graphql_query_write_nothing_and_pass(self):
        self.assertIsNone(self.said("gh api markdown -f text='#1 and @x' -f mode=gfm -f context=heroes-lang/heroes"))
        self.assertIsNone(self.said("gh api /markdown -X POST -f text=hi"))
        self.assertIsNone(self.said("gh api graphql -f query='{ viewer { login } }'"))
        self.assertIsNone(self.said("gh api graphql -i --input query.json"))

    def test_a_graphql_mutation_is_refused_however_it_is_spelled(self):
        self.assertIsNotNone(self.said("gh api graphql -f query='mutation { addComment(input: {}) { clientMutationId } }'"))
        self.assertIsNotNone(self.said("gh api graphql -f query='MUTATION { x }'"))

    def test_the_write_subcommands_are_refused_and_the_reads_pass(self):
        for write in ("issue create -t x", "issue edit 1", "issue close 1", "issue comment 1 -b x", "issue delete 1",
                      "label create x", "label delete x --yes", "repo delete heroes-lang/heroes --yes",
                      "repo create x", "repo edit --enable-issues=false", "run rerun 1", "run cancel 1",
                      "run delete 1", "release create v1", "release delete v1", "pr merge 1", "milestone create",
                      "workflow run ci.yml", "secret set X", "gist create f"):
            self.assertIsNotNone(self.said("gh " + write), write)
        for read in ("issue list", "issue view 1", "pr list", "pr checks 1", "pr diff 1", "run view 1", "run list",
                     "run watch 1", "release view v1", "label list", "repo view heroes-lang/heroes",
                     "repo clone heroes-lang/heroes", "search issues x", "auth status", "--version", "help"):
            self.assertIsNone(self.said("gh " + read), read)

    def test_the_credential_is_never_printed_and_auth_changes_are_the_authors(self):
        said = self.said("gh auth token")
        self.assertIsNotNone(said)
        self.assertIn("leaked secret", said)
        self.assertIsNotNone(self.said("gh auth token --hostname github.com"))
        self.assertIsNotNone(self.said("gh auth refresh -s admin:org"))
        self.assertIsNotNone(self.said("gh auth login"))
        self.assertIsNotNone(self.said("gh auth logout"))
        self.assertIsNone(self.said("gh auth status"))

    def test_a_wrapper_or_a_shell_or_a_global_flag_does_not_hide_the_command(self):
        self.assertIsNotNone(self.said("caffeinate -i gh issue create -t x"))
        self.assertIsNotNone(self.said("env GH_HOST=github.com gh api -f a=b repos/heroes-lang/heroes/issues"))
        self.assertIsNotNone(self.said("/usr/bin/time -p gh issue close 1"))
        self.assertIsNotNone(self.said("sh -c 'gh issue close 1'"))
        self.assertIsNotNone(self.said("gh -R heroes-lang/heroes issue create -t x"))
        self.assertIsNotNone(self.said("gh issue create -R heroes-lang/heroes -t x"))
        self.assertIsNone(self.said("gh -R heroes-lang/heroes issue list"))
        self.assertIsNotNone(self.said("cd /tmp && gh label delete x"))
        self.assertIsNotNone(self.said("true; gh api repos/a/b -X DELETE"))

    def test_a_word_that_only_mentions_gh_is_not_a_command(self):
        self.assertIsNone(self.said("echo gh issue create"))
        self.assertIsNone(self.said("grep -rn 'gh api' .claude/"))
        self.assertIsNone(self.said("git log --oneline --grep='gh issue create'"))


class HeldOut(unittest.TestCase):
    """Issue 2156, 2026-10-21: a held-out task in clear is read by no model, so
    a command naming `held-out-draft/` or `build/opened/` is refused in the
    assistant's shell, save the thesis instrument's words that print no word
    of a task; and the file tools are denied the same folders by the
    settings."""

    def setUp(self):
        self.top = fresh("hooks-2156-")
        self.tree = make_tree(os.path.join(self.top, "tree"))

    def said(self, command):
        return guard_bash.verdict(command, self.tree)

    def test_a_reader_a_walk_or_a_move_into_either_folder_is_refused(self):
        for command in ("cat held-out-draft/suite/01/task.md", "ls held-out-draft", "ls ./held-out-draft/",
                        "cd held-out-draft && ls", "head -n 3 build/opened/03/cases/1.expected",
                        "grep -rn x held-out-draft/", "cp -R build/opened /tmp/x",
                        "find /Users/x/repo/held-out-draft -name '*.md'", "sed -n 1,9p held-out-draft/suite/07/reference.hero",
                        "python3 -I -c 'print(1)' --path=held-out-draft/suite"):
            said = self.said(command)
            self.assertIsNotNone(said, command)
            self.assertIn("held out no longer", said)

    def test_a_wrapper_or_a_shell_does_not_hide_it(self):
        self.assertIsNotNone(self.said("caffeinate -i cat held-out-draft/suite/01/task.md"))
        self.assertIsNotNone(self.said("sh -c 'cat held-out-draft/suite/01/task.md'"))
        self.assertIsNotNone(self.said("true; less build/opened/02/task.md"))

    def test_the_instruments_words_that_print_no_task_pass_and_the_others_are_refused(self):
        start = "./heroes-next run tests/harness/thesis.hero -- "
        for word in ("lint held-out-draft/suite", "tasks held-out-draft/suite --label held-out",
                     "seal held-out-draft/suite -o tests/thesis/held-out/suite.enc --key HEROES_TASKS_KEY",
                     "canary held-out-draft/suite", "scaffold held-out-draft/suite 07",
                     "unseal --sealed tests/thesis/held-out/suite.enc --into build/opened",
                     "dry held-out-draft/suite --mute", "dry build/opened --arm control --mute --compiler ./heroes-next"):
            self.assertIsNone(self.said(start + word), word)
            self.assertIsNone(self.said("caffeinate -i " + start + word), word)
        for word in ("dry held-out-draft/suite", "grade x.hero --task held-out-draft/suite/01",
                     "plan held-out-draft/suite --model claude-sonnet-5-5", "report build/opened"):
            self.assertIsNotNone(self.said(start + word), word)

    def test_a_word_that_only_mentions_a_folder_is_not_a_path_into_it(self):
        self.assertIsNone(self.said("git commit -m 'the kit: lint reads held-out-draft/suite' -- .gitignore"))
        self.assertIsNone(self.said("grep -n held-out .gitignore"))
        self.assertIsNone(self.said("ls tests/thesis/held-out"))
        self.assertIsNone(self.said("ls build/thesis-1234/opened-not"))
        self.assertIsNone(self.said("cat docs/metrics/writing-a-held-out-task.md"))

    def test_the_settings_deny_the_file_tools_both_folders(self):
        with open(os.path.join(os.path.dirname(HOOKS), "settings.json"), encoding="utf-8") as f:
            deny = json.load(f)["permissions"]["deny"]
        for tool in ("Read", "Edit"):
            for folder in ("held-out-draft", "build/opened"):
                self.assertIn(tool + "(/" + folder + "/**)", deny)
                self.assertIn(tool + "(**/" + folder + "/**)", deny)


class Credit(unittest.TestCase):
    """CL-083, 2026-10-11: a file the code cites a defect in is asked of the
    `records` suite narrowed to it. Since the author's seven answers of
    2026-10-12 the issues are GitHub's, so an issue file of `issues/` is asked
    nothing, and the narrowed run reads the defects' names from the copy the
    last full run kept, skipping, and so saying nothing, where there is none."""

    def setUp(self):
        self.top = fresh("hooks-credit-")
        self.tree = make_tree(os.path.join(self.top, "tree"))
        self.issue = os.path.join("issues", "2026-10", "11", "2026-10-11-1501-x.md")

    def test_an_issue_file_is_asked_nothing(self):
        answer(self.tree, "records", 1, "FAIL records/prior-art\n  a refusal no run may give\n")
        issue = write(os.path.join(self.tree, self.issue), "---\nkind: task\n---\n\ndefect 9\n")
        code, said = written(self.tree, issue)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), [])

    def test_a_cited_file_whose_register_copy_is_absent_says_nothing(self):
        answer(
            self.tree, "records", 0,
            "SKIP records/defects-cited\n  no copy of the defects' names at build/defect-register\n"
            "  records: 0 passed, 0 failed\nharness: 0 passed, 0 failed\n",
        )
        cites = write(os.path.join(self.tree, "runtime", "x.c"), "/* Defect 9 */\n")
        code, said = written(self.tree, cites)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), ["records runtime/x.c"])

    def test_a_document_outside_issues_is_not_asked(self):
        doc = write(os.path.join(self.tree, "docs", "x.md"), "defect 9\n")
        code, said = written(self.tree, doc)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), [])

    def test_a_c_file_citing_a_defect_is_asked_and_one_citing_none_is_not(self):
        answer(self.tree, "records", 1, "FAIL records/defects-cited\n  runtime/x.c:1: defect 9 names no defect the copy of GitHub's names at build/defect-register holds\n")
        cites = write(os.path.join(self.tree, "runtime", "x.c"), "/* Defect 9 */\n")
        code, said = written(self.tree, cites)
        self.assertEqual(code, 2, said)
        self.assertIn("defect 9 names no defect", said)
        self.assertEqual(runs(self.tree), ["records runtime/x.c"])
        quiet = write(os.path.join(self.tree, "runtime", "y.c"), "/* no number, and defect_register is a name */\n")
        code, said = written(self.tree, quiet)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), ["records runtime/x.c"])

    def test_a_harness_module_citing_a_defect_is_asked_beside_its_check(self):
        answer(self.tree, "records", 0, "  records: 1 passed, 0 failed\n")
        module = write(os.path.join(self.tree, "tests", "harness", "m.hero"), "# defects 9 and 10\n")
        code, said = written(self.tree, module)
        self.assertEqual(code, 0, said)
        self.assertEqual(runs(self.tree), ["records tests/harness/m.hero"])
        self.assertIn(self.tree + "|check tests/harness/m.hero", calls(self.tree))

    def test_a_cited_file_in_a_tree_with_no_compiler_is_told_so(self):
        bare = make_tree(os.path.join(self.top, "bare"), compiler=False)
        cites = write(os.path.join(bare, "runtime", "x.c"), "/* defect 9 */\n")
        code, said = written(bare, cites)
        self.assertEqual(code, 2, said)
        self.assertIn("not judged", said)
        self.assertIn("`records` suite", said)


class TrackedAsks(unittest.TestCase):
    """Panel 212's R7, the guard's other half, as the author's seven answers of
    2026-10-12 amended it: opening, commenting on, closing and correcting an
    issue through the transcription program is asked once, by that answer, so
    `push`, `repair` and the words of the issues on GitHub, `file`, `close`,
    `comment`, `edit` and `open`, are asked nothing; a milestone, a label and an
    interaction limit stay asked every time, so `prepare` and `finish`, which
    write them from a `gh` they spawn and no rule of the shell guard can see,
    are asked before every run, however each is spelled. A rule is read as the
    harness reads one, `*` standing for any text; that an `ask` outranks an
    `allow` is the harness's order."""

    def setUp(self):
        with open(os.path.join(os.path.dirname(HOOKS), "settings.json"), encoding="utf-8") as f:
            self.permissions = json.load(f)["permissions"]

    def matched(self, kind, command):
        import fnmatch

        return [rule for rule in self.permissions.get(kind, [])
                if rule.startswith("Bash(") and rule.endswith(")")
                and fnmatch.fnmatchcase(command, rule[len("Bash("):-1])]

    def test_the_program_asks_nothing_and_a_push_is_still_asked(self):
        # The transcription's prepare and finish, asked every time, left with
        # the folder of issue files on 2026-10-12, and so did their asks.
        self.assertEqual(self.permissions.get("ask", []), ["Bash(git push*)"])
        self.assertTrue(self.matched("ask", "git push origin HEAD:main"))

    def test_the_writes_asked_once_ask_nothing_and_none_is_denied(self):
        for word in ("file /tmp/x/2026-10-12-1800-a.md", "close 2031 /tmp/x/closing.md",
                     "comment 2031 /tmp/x/text.md", "edit 2031 /tmp/x/2026-10-12-1800-a.md",
                     "open --kind defect --json"):
            for start in ("./heroes", "caffeinate -i ./heroes"):
                command = start + " run tests/harness/issues.hero -- " + word
                self.assertFalse(self.matched("ask", command), command)
                self.assertFalse(self.matched("deny", command), command)
            self.assertTrue(self.matched("allow", "./heroes run tests/harness/issues.hero -- " + word), word)

    def test_the_reads_ask_nothing(self):
        for command in ("./heroes run tests/harness/issues.hero -- draft /tmp/x/2026-10-12-1800-a.md",
                        "./heroes run tests/harness/issues.hero -- open",
                        "./heroes run tests/harness/main.hero -- ./heroes records"):
            self.assertFalse(self.matched("ask", command), command)
            self.assertTrue(self.matched("allow", command), command)


class TaggedPush(unittest.TestCase):
    """Defect 2198: a push that can carry an `m-*` tag is judged by
    `records/tagged` with the tag placed, and goes through on a pass alone; a
    branch's push is not judged."""

    CARRY = ("git push origin main m-rechecked-issues", "git push origin --tags", "git push --follow-tags origin main",
             "git push origin refs/tags/m-x", "git push origin +m-x", "git push origin m-x:refs/tags/m-x",
             "git push origin tag m-x", "git push --mirror origin", "git push --repo=origin m-x",
             "git push --repo origin m-x", "git push -o ci.skip origin m-x", "git push origin -- m-x",
             "git -C . push origin main m-x")
    BRANCH = ("git push origin main", "git push", "git push origin", "git push m-remote main", "git push origin main:main",
              "git push origin v1.0.0", "git push -o m-x origin main", "git push --dry-run origin main",
              "git push origin tag v1.0.0", "git push origin refs/heads/m-branch-is-a-branch-only-when-named-so:refs/heads/x")

    def setUp(self):
        self.top = fresh("hooks-2198-")
        self.tree = make_tree(os.path.join(self.top, "tree"), repo=True)

    def said(self, command, cwd=None):
        return guard_bash.verdict(command, cwd or self.tree)

    def test_which_pushes_can_carry_a_milestone_tag(self):
        for command in self.CARRY:
            words = command.split()
            rest = words[words.index("push") + 1:]
            self.assertTrue(tagged.carries_tag(rest), command)
        for command in self.BRANCH:
            words = command.split()
            self.assertFalse(tagged.carries_tag(words[words.index("push") + 1:]), command)

    def test_a_branch_push_asks_nothing(self):
        for command in self.BRANCH:
            self.assertIsNone(self.said(command), command)
        self.assertEqual(runs(self.tree), [])

    def test_a_tag_push_goes_through_on_a_pass_and_asks_the_tag_check_alone(self):
        answer(self.tree, "records", 0, "  records: 1 passed, 0 failed\nharness: 1 passed, 0 failed\n")
        for command in self.CARRY:
            self.assertIsNone(self.said(command), command)
        self.assertEqual(set(runs(self.tree)), {"records records/tagged"})
        self.assertEqual(len(runs(self.tree)), len(self.CARRY))

    def test_a_red_tag_check_refuses_with_its_own_words(self):
        answer(self.tree, "records", 1, "FAIL records/tagged\n  m-x was placed over 1 open defect(s) that count against a tag:\n    2139\n  records: 0 passed, 1 failed\n")
        said = self.said("git push origin main m-x")
        self.assertIsNotNone(said)
        self.assertIn("did not pass (exit 1)", said)
        self.assertIn("m-x was placed over 1 open defect(s)", said)
        self.assertIn("git tag -d", said)

    def test_a_skip_is_not_a_pass(self):
        answer(self.tree, "records", 0, "SKIP records/tagged\n  GitHub could not be asked, so this check judged nothing: offline\n  records: 0 passed, 0 failed\n")
        said = self.said("git push origin --tags")
        self.assertIsNotNone(said)
        self.assertIn("GitHub could not be asked", said)

    def test_a_harness_that_selects_nothing_refuses(self):
        said = self.said("git push origin m-x")
        self.assertIsNotNone(said)
        self.assertIn("did not pass (exit 2)", said)

    def test_a_check_that_does_not_answer_in_time_refuses(self):
        with mock.patch.object(marks, "bounded", return_value=None):
            said = self.said("git push origin m-x")
        self.assertIsNotNone(said)
        self.assertIn("did not answer within", said)

    def test_a_tree_with_no_compiler_built_refuses(self):
        bare = make_tree(os.path.join(self.top, "bare"), compiler=False, repo=True)
        said = self.said("git push origin m-x", cwd=bare)
        self.assertIsNotNone(said)
        self.assertIn("has not built", said)

    def test_a_push_outside_every_tree_is_not_judged(self):
        elsewhere = fresh("hooks-2198-elsewhere-")
        self.assertIsNone(self.said("git push origin m-x", cwd=elsewhere))

    def test_the_word_is_the_harness_own(self):
        suite = os.path.join(HOOKS, "..", "..", "tests", "harness", "suite_records.hero")
        with open(suite, encoding="utf-8") as handle:
            held = re.search(r'^constant TAG_CHECK: str\n    "([^"]+)"$', handle.read(), re.MULTILINE)
        self.assertIsNotNone(held, "suite_records.hero no longer holds `constant TAG_CHECK: str`")
        self.assertEqual(held.group(1), tagged.TAG_CHECK)


if __name__ == "__main__":
    unittest.main()
