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

import os
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


if __name__ == "__main__":
    unittest.main()
