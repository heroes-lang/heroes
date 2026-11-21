#!/usr/bin/env python3
"""The differential test of the Heroes hooks against the Python ones.

Why this exists. The author's answer `4a` of 2026-11-12 (feature 2082): the
hooks are written again in Heroes BESIDE the Python ones under
`.claude/hooks/`, and a differential test feeds both the same inputs, every
tool-call shape each hook judges today, and holds their answers identical:
the decision, the message and the exit code. The switch itself,
`.claude/settings.json` pointing at the Heroes hooks and the Python ones
removed, is the author's, made present, since a hook that fails open turns a
guard off in silence.

HOW. The shapes each hook judges today are the ones its own tests build:
`test_hooks.py` constructs trees, lanes, conflicted merges and stand-in
compilers, then asks `guard_bash.verdict` of a command line. `install`, which
`test_hooks.py` calls once at its top, wraps that function, so every verdict a
test asks at its top is also asked of the two hooks AS PROCESSES, the way the settings run
them: the same event on stdin, `{"cwd": ..., "tool_input": {"command": ...}}`,
in the same directory, with the same environment (a test's `mock.patch.dict`
of `os.environ` included), against the same trees. The test's own assertion
reads the Python answer as before. `test_twins.py` adds the shapes that need
no tree.

WHAT IS HELD. Where the Heroes guard answers, its exit code and every byte of
its stderr are the Python hook's, or the test fails naming the command. Where
it answers that it does not judge the command yet (its message opens
`UNWRITTEN`: the rules that ask git or a compiler are not written in Heroes
so far), it must still block, exit 2, and the shape is counted apart; the
count of each kind is printed at the end of the run, and `test_twins.py`
holds a floor under the identical ones, so a twin that stopped comparing
would be red rather than green and blind.

THE HEROES GUARD is built once per run, `<tree>/heroes build
.claude/hooks/guard_bash.hero`, into the system's temporary folder (the
hooks' tests' own exception to CLAUDE.md § Hard stops, 2026-10-09). Where the
tree has no compiler built, nothing is twinned and the floor test says so as
a skip. The write hook, `fmt_check.py`, has no Heroes twin yet.
"""

import atexit
import json
import os
import subprocess
import sys
import tempfile
import threading

HOOKS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HOOKS))
PYTHON_GUARD = os.path.join(HOOKS, "guard_bash.py")
HEROES_GUARD_SOURCE = os.path.join(".claude", "hooks", "guard_bash.hero")
# How the Heroes guard says it does not judge a command yet (`answer.hero`).
UNWRITTEN = "refused: the Heroes guard does not judge this command yet: "

TALLY = {"identical": 0, "unwritten": 0}
_BUILT = {"path": None, "why": None, "done": False}
_LOCK = threading.Lock()


def compiler():
    """The tree's own compiler, `<tree>/heroes`, or None."""
    path = os.path.join(ROOT, "heroes")
    return path if os.path.isfile(path) and os.access(path, os.X_OK) else None


def guard():
    """The Heroes guard's binary, built once, or None with `why_not()` set."""
    with _LOCK:
        if _BUILT["done"]:
            return _BUILT["path"]
        _BUILT["done"] = True
        heroes = compiler()
        if heroes is None:
            _BUILT["why"] = "the tree has no compiler built (" + os.path.join(ROOT, "heroes") + ")"
            return None
        out = os.path.join(tempfile.mkdtemp(prefix="hooks-twins-"), "guard_bash")
        done = subprocess.run([heroes, "build", HEROES_GUARD_SOURCE, "-o", out], cwd=ROOT,
                              capture_output=True, text=True, timeout=600)
        if done.returncode != 0 or not os.path.isfile(out):
            raise AssertionError("the Heroes guard does not build:\n" + done.stdout + done.stderr)
        _BUILT["path"] = out
        return out


def why_not():
    return _BUILT["why"]


# The Heroes sweep (`sweep.hero`), the twin of `sweep.py`, built once per run
# as the guard is; `test_twins.py`'s `SweepTwins` runs both on the same
# repositories and holds their reports and what they leave identical.
PYTHON_SWEEP = os.path.join(HOOKS, "sweep.py")
HEROES_SWEEP_SOURCE = os.path.join(".claude", "hooks", "sweep.hero")
_SWEEPER = {"path": None, "why": None, "done": False}


def sweeper():
    """The Heroes sweep's binary, built once, or None with `why_not_sweeper()`
    set."""
    with _LOCK:
        if _SWEEPER["done"]:
            return _SWEEPER["path"]
        _SWEEPER["done"] = True
        heroes = compiler()
        if heroes is None:
            _SWEEPER["why"] = "the tree has no compiler built (" + os.path.join(ROOT, "heroes") + ")"
            return None
        out = os.path.join(tempfile.mkdtemp(prefix="hooks-twins-"), "sweep")
        done = subprocess.run([heroes, "build", HEROES_SWEEP_SOURCE, "-o", out], cwd=ROOT,
                              capture_output=True, text=True, timeout=600)
        if done.returncode != 0 or not os.path.isfile(out):
            raise AssertionError("the Heroes sweep does not build:\n" + done.stdout + done.stderr)
        _SWEEPER["path"] = out
        return out


def why_not_sweeper():
    return _SWEEPER["why"]


def event(command, cwd):
    """The event the settings hand the guard for `command` run in `cwd`."""
    payload = {"tool_input": {"command": command}}
    if cwd is not None:
        payload["cwd"] = cwd
    return json.dumps(payload)


def answers(command, cwd):
    """(the Python hook's (exit, stderr), the Heroes hook's), both run as the
    settings run them, or None where the Heroes guard could not be built."""
    binary = guard()
    if binary is None:
        return None
    given = event(command, cwd)
    # The test's own environment, and one variable the hooks read nowhere:
    # the stand-in compiler of `test_hooks.py` leaves its call log alone for
    # it, so a test counting calls counts the test's own.
    env = dict(os.environ, HEROES_HOOKS_TWIN="1")
    python = subprocess.run([sys.executable, "-I", PYTHON_GUARD], input=given, capture_output=True,
                            text=True, timeout=120, env=env)
    heroes = subprocess.run([binary], input=given, capture_output=True, text=True, timeout=120, env=env)
    return (python.returncode, python.stderr), (heroes.returncode, heroes.stderr)


def held(command, cwd=None):
    """Asks both hooks of `command` and holds their answers as the module's
    docstring says, counting each shape; a difference is an AssertionError,
    which fails the test that asked."""
    got = answers(command, cwd)
    if got is None:
        return
    python, heroes = got
    if heroes[1].startswith(UNWRITTEN):
        if heroes[0] != 2:
            raise AssertionError("the Heroes guard must block what it does not judge: " + repr(command))
        TALLY["unwritten"] += 1
        return
    if heroes != python:
        raise AssertionError(
            "the two guards answer " + repr(command) + " in " + repr(cwd) + " differently:\n"
            + "  python: " + repr(python) + "\n  heroes: " + repr(heroes)
        )
    TALLY["identical"] += 1


def install(module):
    """Wraps `module.verdict`, the Python guard's, so each verdict a test asks
    at its top is also held by `held`; the guard's own recursive calls, a
    substitution's or a shell's `-c`, are not, being part of one."""
    original = module.verdict
    depth = [0]

    def twinned(command, cwd=None, seen=frozenset()):
        depth[0] += 1
        try:
            said = original(command, cwd, seen)
        finally:
            depth[0] -= 1
        if depth[0] == 0 and not seen:
            held(command, cwd)
        return said

    module.verdict = twinned


def report():
    if TALLY["identical"] or TALLY["unwritten"]:
        sys.stderr.write("twins: " + str(TALLY["identical"]) + " shapes held identical by the two guards, "
                         + str(TALLY["unwritten"]) + " not judged by the Heroes guard yet\n")


atexit.register(report)
