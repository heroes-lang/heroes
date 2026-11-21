#!/usr/bin/env python3
"""The differential test's own shapes: the command lines the guard judges on
their text, which need no tree, held identical between the Python guard and
the Heroes one (`twins.py` says how and why; feature 2082).

The hooks' own tests twin every shape they build (`test_hooks.py` installs
`twins`); these are the shapes their trees do not reach, one or more for each
branch of the guard's reading of a line and of each rule on text: heredocs,
quotes and backslashes, the segments, the wrappers, a shell's `-c`, a
substitution, a `find`, the dump, the Anthropic endpoint, the secrets,
`.env`, the held-out folders, `gh`, `--no-line`, `git add` and `git stage`,
`git commit -a`, a commit's pathspec where git is asked nothing (a directory
that is not there), `git stash`, a force push. Run from the repository's root:

    python3 -I -m unittest discover -s .claude/hooks -t .claude/hooks
"""

import json
import os
import subprocess
import sys
import tempfile
import unittest

HOOKS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HOOKS)

import twins  # noqa: E402

# A directory that is not there: a commit judged in it asks git nothing in
# either guard, so its pathspec rules are judged on text alone.
NOWHERE = "/heroes-hooks-twins-never-there"

# Each shape, and the directory it is judged in (None: the test's own).
SHAPES = [
    ("ls -la", None),
    ("git status", None),
    ("git add -A", None),
    ("git add --all x", None),
    ("git stage -u", NOWHERE),
    ("git -C . add .", None),
    ("git add a.hero b.hero", None),
    ("cd x && git add -A", None),
    ("echo \"cd x && git add -A\"", None),
    ("echo 'git add -A' ; ls", None),
    ("python3 - <<'EOF'\ngit add .\nEOF\nls", None),
    ("cat <<EOF | wc\ngit stash\n  EOF  \ngit stash", None),
    ("cat <<<word\nword\ngit stash", None),
    ("echo $(git add -A)", None),
    ("echo `git stash`", None),
    ("echo \"$(git stash)\"", None),
    ("echo '$(git stash)'", None),
    ("echo $(( 1 + 2 ))", None),
    ("bash -c 'git stash'", None),
    ("sh -ec \"git add -A\"", None),
    ("eval git stash", None),
    ("zsh script.sh", None),
    ("GIT_EDITOR=true git stash", None),
    ("caffeinate -i git add -A", None),
    ("/usr/bin/time -p git stash", None),
    ("timeout -s KILL 60 git stash", None),
    ("env -u X git stash", None),
    ("nice -n 5 git add .", None),
    ("xargs -J % git add -A %", None),
    ("if true; then git stash; fi", None),
    ("f() { git stash; }", None),
    ("case x in a) git stash ;; esac", None),
    ("(cd x && git stash)", None),
    ("{ git stash; }", None),
    ("find . -name x -exec git add -A {} \\;", None),
    ("find . -execdir git stash +", None),
    ("find . -name '*.hero'", None),
    ("git commit -a -m x", None),
    ("git commit -am x", None),
    ("git commit --all", None),
    ("git commit -m x", NOWHERE),
    ("git commit -F m --", NOWHERE),
    ("git commit -F m -- \"${xs[@]}\"", NOWHERE),
    ("git commit -F m -- .", NOWHERE),
    ("git commit -F m -- :/", NOWHERE),
    ("git commit --amend --only --", NOWHERE),
    ("git commit -F m -- a.hero", NOWHERE),
    ("git commit \\\n-- a.hero", NOWHERE),
    ("git add a && git commit -F m -- a", NOWHERE),
    ("git status && git commit -F m -- a", NOWHERE),
    ("echo x > f && git commit -F m -- a", NOWHERE),
    ("ls 2>&1 && git commit -F m -- a", NOWHERE),
    ("git commit -F m -- $(git add -A; echo a)", NOWHERE),
    ("git commit -m \"$(cat <<'EOF'\nsubject\nEOF\n)\" -- a", NOWHERE),
    ("./heroes test x.hero && git commit -F m -- a", NOWHERE),
    ("./heroes run tests/harness/main.hero -- ./heroes x | tail -3", NOWHERE),
    ("./heroes run tests/harness/main.hero -- ./heroes x > out", NOWHERE),
    ("git stash", None),
    ("git stash list", None),
    ("git stash show -p", None),
    ("git push --force origin main", None),
    ("git push -f", None),
    ("git push origin main", NOWHERE),
    ("git push origin m-thesis", NOWHERE),
    ("UPDATE_GOLDEN=1 ./heroes run x", None),
    ("grep -rn UPDATE_GOLDEN .", None),
    ("env", None),
    ("printenv", None),
    ("env FOO=1 ls", None),
    ("printenv PATH", None),
    ("echo $ANTHROPIC_API_KEY", None),
    ("echo ${#ANTHROPIC_API_KEY}", None),
    ("printf %s ${CLOUDFLARE_API_TOKEN}", None),
    ("cat .env", None),
    ("head -3 x/.env", None),
    ("cat .env.example", None),
    ("curl https://api.anthropic.com/v1/messages/count_tokens -d @x", None),
    ("curl https://api.anthropic.com/v1/messages -d @x", None),
    ("curl https://API.ANTHROPIC.COM/v1/messages/count_tokens?beta=1", None),
    ("curl api.anthropic.com:443/v1/messages/count_tokens", None),
    ("grep -rn api.anthropic.com .claude", None),
    ("ls held-out-draft/", None),
    ("cp x --into=./build/opened", None),
    ("git commit -m 'about build/opened' -- a", NOWHERE),
    ("./heroes run tests/harness/thesis.hero -- dry --mute build/opened", None),
    ("./heroes run tests/harness/thesis.hero -- dry build/opened", None),
    ("./heroes run tests/harness/thesis.hero -- lint held-out-draft", None),
    ("gh issue view 2082 --repo heroes-lang/heroes --comments", None),
    ("gh issue close 3", None),
    ("gh pr comment 4 --body x", None),
    ("gh auth token", None),
    ("gh auth status", None),
    ("gh auth login", None),
    ("gh api repos/heroes-lang/heroes/issues", None),
    ("gh api repos/x/issues -f title=t", None),
    ("gh api -X GET search/issues -f q=x", None),
    ("gh api --method=PATCH repos/x", None),
    ("gh api graphql -f query='{ viewer { login } }'", None),
    ("gh api graphql -f query='mutation { x }'", None),
    ("gh api /markdown -f text=x", None),
    ("gh --version", None),
    ("./heroes build x.hero --no-line", None),
    ("grep -- --no-line x", None),
    ("echo 'unbalanced", None),
    ("git add 'unbalanced -A", None),
    ("ls \\", None),
    ("git add -A", None),
    ("echo é && git stash", None),
    ("git add -A　", None),
    ("ls\u0000x", None),
    ("git/ add -A", None),
    ("  ;; ", None),
    ("", None),
]


# Shapes judged in a folder that is there, where the Python guard's overwrite
# rules read the words and answer nothing before asking git anything.
IN_A_FOLDER = [
    "git rm x",
    "git rm -r --cached-not x",
    "git rm -f -n x",
    "git restore x",
    "git restore --staged",
    "git restore --no-staged x",
    "git mv a b",
    "git mv -f a",
    "git checkout main",
    "git checkout -b fix",
    "git checkout -bfix main",
    "git checkout --orphan new",
    "git checkout -- f",
    "git checkout --bogus x",
    "git switch main",
    "git reset --soft -- x",
    "git reset --merge-not",
    "git cherry-pick abc",
    "git revert abc",
    "git am patch",
    "git merge --quit",
    "git rebase --edit-todo",
    "xargs git rm x",
    "git -C . rm x",
]


class Corpus(unittest.TestCase):
    """Every shape above, held identical by the two guards, none of them a
    command the Heroes guard leaves unwritten."""

    def test_every_shape_of_text_is_held_identical(self):
        if twins.guard() is None:
            self.skipTest("nothing twinned: " + twins.why_not())
        before = dict(twins.TALLY)
        for command, cwd in SHAPES:
            with self.subTest(command=command):
                twins.held(command, cwd)
        self.assertEqual(twins.TALLY["unwritten"], before["unwritten"], "a shape of text left unwritten")
        self.assertEqual(twins.TALLY["identical"] - before["identical"], len(SHAPES))

    def test_a_difference_is_seen(self):
        """The instrument is not blind: an event the two hooks answer apart
        fails, here the Heroes guard asked as if it were in another tree."""
        if twins.guard() is None:
            self.skipTest("nothing twinned: " + twins.why_not())
        python, heroes = twins.answers("git add -A", None)
        self.assertEqual(python, heroes)
        self.assertNotEqual(python, twins.answers("ls", None)[1])
        with self.assertRaises(AssertionError):
            original = twins.answers
            twins.answers = lambda command, cwd: (original(command, cwd)[0], (0, ""))
            try:
                twins.held("git add -A")
            finally:
                twins.answers = original

    def test_what_names_its_paths_is_held_identical_where_git_is_asked_nothing(self):
        if twins.guard() is None:
            self.skipTest("nothing twinned: " + twins.why_not())
        folder = os.path.realpath(tempfile.mkdtemp(prefix="hooks-twins-"))
        before = dict(twins.TALLY)
        for command in IN_A_FOLDER:
            with self.subTest(command=command):
                twins.held(command, folder)
        self.assertEqual(twins.TALLY["unwritten"], before["unwritten"], "a shape git is asked nothing of left unwritten")
        self.assertEqual(twins.TALLY["identical"] - before["identical"], len(IN_A_FOLDER))

    def test_a_folder_that_is_a_tree_twins_its_text_rules(self):
        if twins.guard() is None:
            self.skipTest("nothing twinned: " + twins.why_not())
        tree = os.path.realpath(tempfile.mkdtemp(prefix="hooks-twins-"))
        for command in ("git add -A", "gh issue close 1", "cat .env", "ls"):
            with self.subTest(command=command):
                twins.held(command, tree)


class SweepTwins(unittest.TestCase):
    """The Heroes sweep beside the Python one (`sweep.hero`, `sweep.py`): run
    as processes on one repository holding every shape the sweep judges
    (`test_hooks.sweep_scenario`), their reports are held identical byte for
    byte, a dry run's on one repository, a real sweep's on two built alike,
    which must also leave the same worktrees, branches and folders. A wait
    loop is judged on its age, which a test cannot make three hours, so the
    loops' reading is held by each twin's own tests, `SweepLoops` and
    `sweep_loops.hero`'s, over the same shapes."""

    def setUp(self):
        self.binary = twins.sweeper()
        if self.binary is None:
            self.skipTest("nothing twinned: " + twins.why_not_sweeper())
        import test_hooks

        self.hooks = test_hooks

    def both(self, cwd_python, cwd_heroes, words, given=None):
        python = subprocess.run([sys.executable, "-I", twins.PYTHON_SWEEP] + words, cwd=cwd_python, input=given,
                                capture_output=True, text=True, timeout=600)
        heroes = subprocess.run([self.binary] + words, cwd=cwd_heroes, input=given,
                                capture_output=True, text=True, timeout=600)
        return (python.returncode, python.stdout, python.stderr), (heroes.returncode, heroes.stdout, heroes.stderr)

    def state(self, root, folder):
        listed = self.hooks.git(root, "worktree", "list", "--porcelain").stdout
        trees = sorted(line[len("worktree " + folder) + 1:] for line in listed.split("\n")
                       if line.startswith("worktree " + folder + "/"))
        folders = sorted(name for name in os.listdir(folder) if not name.startswith("."))
        return trees, sorted(self.hooks.branch_names(root)), folders

    def test_a_dry_run_is_held_identical_on_every_shape(self):
        root, _folder = self.hooks.sweep_scenario("hooks-twins-sweep-")
        python, heroes = self.both(root, root, ["--dry-run"])
        self.assertEqual(python, heroes)
        self.assertEqual(python[0], 0)
        for said in ("would remove worktree lane-done", "would remove folder old-scratch", "would delete branch merged-one",
                     "worktree lane-dirty: uncommitted work", "worktree lane-ahead: 1 commit",
                     "folder p999-scratch: a tracked file names it", "branch unmerged-one: no checkout's HEAD",
                     "worktree record lane-gone", "worktree lane-locked: it is locked",
                     "folder clone-scratch: it holds a git repository", "branch just-made, fresh-scratch, lane-new"):
            self.assertIn(said, python[1])

    def test_a_sweep_leaves_the_same_tree_and_says_the_same(self):
        root_a, folder_a = self.hooks.sweep_scenario("hooks-twins-sweep-a-")
        root_b, folder_b = self.hooks.sweep_scenario("hooks-twins-sweep-b-")
        python, heroes = self.both(root_a, root_b, ["--sweep"])
        self.assertEqual(python[0], heroes[0])
        self.assertEqual(python[1].replace(root_a, "<root>"), heroes[1].replace(root_b, "<root>"))
        self.assertEqual(python[2], heroes[2])
        self.assertIn("removed worktree lane-done", python[1])
        self.assertEqual(self.state(root_a, folder_a), self.state(root_b, folder_b))
        trees, names, folders = self.state(root_b, folder_b)
        self.assertNotIn("lane-done", trees)
        self.assertNotIn("merged-one", names)
        self.assertNotIn("old-scratch", folders)
        self.assertIn("lane-dirty", trees)
        self.assertIn("unmerged-one", names)
        self.assertIn("p999-scratch", folders)

    def test_the_hook_answers_identically_and_a_prompt_within_the_hour_says_nothing(self):
        root_a, _folder_a = self.hooks.sweep_scenario("hooks-twins-hook-a-")
        root_b, _folder_b = self.hooks.sweep_scenario("hooks-twins-hook-b-")
        start_a = json.dumps({"hook_event_name": "SessionStart", "cwd": root_a, "session_id": "s"})
        start_b = json.dumps({"hook_event_name": "SessionStart", "cwd": root_b, "session_id": "s"})
        python = subprocess.run([sys.executable, "-I", twins.PYTHON_SWEEP], input=start_a, capture_output=True,
                                text=True, timeout=600)
        heroes = subprocess.run([self.binary], input=start_b, capture_output=True, text=True, timeout=600)
        self.assertEqual((python.returncode, python.stderr), (heroes.returncode, heroes.stderr))
        self.assertEqual(python.stdout.replace(root_a, "<root>"), heroes.stdout.replace(root_b, "<root>"))
        self.assertIn("removed worktree lane-done", json.loads(heroes.stdout)["hookSpecificOutput"]["additionalContext"])
        for root in (root_a, root_b):
            prompt = json.dumps({"hook_event_name": "UserPromptSubmit", "cwd": root, "prompt": "x"})
            python, heroes = self.both(root, root, [], given=prompt)
            self.assertEqual(python, heroes)
            self.assertEqual(python, (0, "", ""))

    def test_nothing_to_judge_a_held_lock_and_a_wrong_word_are_said_identically(self):
        elsewhere = os.path.realpath(tempfile.mkdtemp(prefix="hooks-twins-sweep-none-"))
        python, heroes = self.both(elsewhere, elsewhere, ["--dry-run"])
        self.assertEqual(python, heroes)
        self.assertIn("nothing was judged", python[1])
        root, folder = self.hooks.sweep_repository("hooks-twins-sweep-lock-")
        os.mkdir(os.path.join(folder, ".sweep-lock"))
        python, heroes = self.both(root, root, ["--dry-run"])
        self.assertEqual(python, heroes)
        self.assertIn("another sweep holds", python[1])
        python, heroes = self.both(root, root, ["--bogus"])
        self.assertEqual(python, heroes)
        self.assertEqual(python[0], 2)
        for event in ("not json", "[1]", json.dumps({"hook_event_name": "Stop", "cwd": root})):
            python, heroes = self.both(root, root, [], given=event)
            self.assertEqual(python, heroes)
            self.assertEqual(python, (0, "", ""))

    def test_a_difference_is_seen(self):
        """The instrument is not blind: two repositories that differ give
        reports that differ."""
        root, _folder = self.hooks.sweep_scenario("hooks-twins-sweep-blind-")
        plain, _plain_folder = self.hooks.sweep_repository("hooks-twins-sweep-plain-")
        python, heroes = self.both(root, plain, ["--dry-run"])
        self.assertNotEqual(python[1].replace(root, "<root>"), heroes[1].replace(plain, "<root>"))


if __name__ == "__main__":
    unittest.main()
