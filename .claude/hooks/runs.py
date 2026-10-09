#!/usr/bin/env python3
"""What a command runs that its first word does not say.

Why this exists. Every rule of `guard_bash.py` reads a command by its first
word, the wrappers before it left out (`env`, `timeout`, `xargs` and the rest,
defect 403), so two routes to a git command went unjudged, whatever they
discarded or committed. Defect 534, lane b16-misc, 2026-10-09; measured again
that day in scratch repositories, git 2.56.0 and the BSD `find` of macOS:

- `find ... -exec git <verb> ... \\;` and `+`, `-execdir`, `-ok` and `-okdir`
  run a command per file, or per list of files, the `{}` in a word standing
  for a path the text does not hold (anywhere in a word, for BSD's `find` and
  GNU's alike); `-exec` and `-ok` run in the directory `find` runs in,
  `-execdir` and `-okdir` in each file's own, under the paths `find` starts
  from;
- a git alias, `alias.<name>` in any configuration git reads, its `-c` and
  the environment included, runs `git <its words> <the command's own words>`,
  or, where its value begins with `!`, the shell command after the `!` with
  the command's own words appended, from the top of the work tree. git uses
  an alias only where no command of that name exists (a builtin, a command in
  its exec path or a `git-<name>` on the PATH), and an alias may name another.

So the guard asks this module for the commands a `find` runs and for what an
alias stands for, and judges each as it judges the command line it was given
(`guard_bash.commands_in`). Which alias a name is, and which names are git's
own, is asked of git itself (`git config --get-regexp ^alias\\.`, `git
--list-cmds`), in the directory the command runs in, with its options and its
environment. The verbs the guard reads, and the commands sessions here run
most, are git's own builtins, which an alias cannot replace, so they are not
asked about (`BUILTIN`, which a test holds to `git --list-cmds=builtins`).

And the command lines a substitution runs, `$( ... )` and backquotes, which the
guard judges as it judges the line around them (`substitutions`), here rather
than in the guard, whose own text Python compiles before every Bash call.
"""

import os
import shlex

# What stands for the paths `find` gives a command it runs, which the text does
# not hold: read by `overwrites.py` as every path, as it reads what `xargs`
# adds.
FOUND = "the words `find` puts for `{}`"
# The actions of `find` that run a command, and those that run it in each
# file's own directory.
ACTIONS = frozenset({"-exec", "-execdir", "-ok", "-okdir"})
IN_ITS_DIRECTORY = frozenset({"-execdir", "-okdir"})
# `find`'s options before its paths, BSD's and GNU's, and those that take a
# value.
FIND_OPTIONS = frozenset({"-H", "-L", "-P", "-E", "-X", "-d", "-s", "-x"})
FIND_VALUED = frozenset({"-f", "-D"})
# git's own options before its verb that take the next word as their value.
GIT_VALUED = frozenset({"-C", "-c", "--git-dir", "--work-tree", "--namespace", "--config-env", "--super-prefix"})
# Git's builtins the guard reads or sessions here run most: an alias of one of
# these names is never used, so none is asked about. Each is listed by `git
# --list-cmds=builtins` on git 2.56.0, which a test asks on the machine it runs
# on (a premise about the world, given the test that fires when it dies).
BUILTIN = frozenset({
    "add", "am", "branch", "cat-file", "check-ignore", "checkout", "cherry-pick", "commit", "config",
    "describe", "diff", "fetch", "for-each-ref", "grep", "log", "ls-files", "ls-tree", "merge",
    "merge-base", "mv", "pull", "push", "read-tree", "rebase", "remote", "reset", "restore", "rev-list",
    "rev-parse", "revert", "rm", "show", "stash", "status", "switch", "symbolic-ref", "tag",
    "update-index", "worktree",
})


def answered(where, options, env, *args):
    """`git <options> <args>` in `where` with `env`: (its exit, its stdout),
    or None where it did not run. `subprocess` is imported here: a
    substitution, read on every line that holds one, asks git nothing."""
    import subprocess

    try:
        done = subprocess.run(["git"] + list(options) + list(args), capture_output=True, timeout=30,
                              cwd=where, text=True, env=env)
    except (OSError, subprocess.SubprocessError):
        return None
    return done.returncode, done.stdout


def run(where, options, env, *args):
    """`git <options> <args>` in `where` with `env`: its stdout, or None."""
    done = answered(where, options, env, *args)
    return done[1] if done is not None and done[0] == 0 else None


def starts(w, here):
    """The directory a `find <w[1:]>` starts from: its first path, where the
    text holds it and it is a directory, else `here`."""
    i = 1
    while i < len(w) and (w[i] in FIND_OPTIONS or w[i] in FIND_VALUED or w[i].startswith("-O")):
        i += 2 if w[i] in FIND_VALUED else 1
    if i >= len(w) or w[i].startswith("-") or w[i] in ("(", "!") or here is None:
        return here
    first = w[i]
    if "$" in first or "`" in first:
        return here
    path = first if os.path.isabs(first) else os.path.join(here, first)
    return os.path.normpath(path) if os.path.isdir(path) else here


def executed(w, here):
    """The commands `find <w[1:]>`, run in `here`, runs: each (its words,
    the directory it runs in), a word holding `{}` read as `FOUND`. A command
    ends at a `;` or at a `+` after a `{}`; one the words never end is read to
    their end, the reading that judges more."""
    out = []
    i = 1
    while i < len(w):
        if w[i] not in ACTIONS:
            i += 1
            continue
        action = w[i]
        j = i + 1
        inner = []
        while j < len(w) and w[j] != ";" and not (w[j] == "+" and inner and inner[-1] == "{}"):
            inner.append(w[j])
            j += 1
        there = starts(w, here) if action in IN_ITS_DIRECTORY else here
        if inner:
            out.append(([FOUND if "{}" in word else word for word in inner], there))
        i = j + 1
    return out


def verb_of(w):
    """(git's verb in `w`, its index), or (None, None) where there is none."""
    i = 1
    while i < len(w) and w[i].startswith("-"):
        i += 2 if w[i] in GIT_VALUED else 1
    if i < len(w):
        return w[i], i
    return None, None


def aliases(where, options, env):
    """Every alias git reads in `where` with `options` and `env`, by its name
    lowercased (git reads a configuration key's name so), or None where git
    cannot say."""
    done = answered(where, options, env, "config", "-z", "--get-regexp", r"^alias\.")
    if done is None or done[0] not in (0, 1):
        return None
    # git exits 1 where no key matches, which is no alias at all.
    code, out = done
    found = {}
    if code == 1:
        return found
    for entry in out.split("\0"):
        if not entry:
            continue
        key, _newline, value = entry.partition("\n")
        found[key[len("alias."):].lower()] = value
    return found


def own_command(where, options, env, name):
    """Whether git runs a command of its own for `name`, a builtin, one of its
    exec path or a `git-<name>` on the PATH, which an alias never replaces;
    None where git cannot say."""
    out = run(where, options, env, "--list-cmds=builtins,main,others")
    if out is None:
        return None
    return name in out.split()


def alias(w, where, options, env):
    """What git runs for the git command `w`, run in `where`, where its verb
    is an alias git uses: ("words", the words with the alias expanded, the
    alias's name) or ("script", the shell command line, the directory it runs
    in, the alias's name); None where it is none, or git cannot say. A name
    of `BUILTIN` is never asked about."""
    verb, at = verb_of(w)
    if verb is None or verb in BUILTIN or where is None or not os.path.isdir(where):
        return None
    known = aliases(where, options, env)
    if not known or verb.lower() not in known:
        return None
    if own_command(where, options, env, verb) is not False:
        return None
    value = known[verb.lower()]
    rest = w[at + 1:]
    if value.startswith("!"):
        top = run(where, options, env, "rev-parse", "--show-toplevel")
        there = top.strip() if top and top.strip() else where
        return ("script", value[1:] + "".join(" " + shlex.quote(word) for word in rest), there, verb)
    try:
        words = shlex.split(value)
    except ValueError:
        words = value.split()
    return ("words", w[:at] + words + rest, verb)


def substitutions(text):
    """The command lines the shell runs inside `$( ... )` and backquotes in
    `text`, before the command around them, each to be judged as one (defect
    534's shape beside it: `echo $(git add -A)` was read by its first word).
    Outside single quotes, a backslash's character left alone; `$(( ... ))`
    is arithmetic; one never closed is read to the end."""
    out = []
    i, quote = 0, None
    while i < len(text):
        c = text[i]
        if quote == "'":
            quote = None if c == "'" else quote
            i += 1
            continue
        if c == "\\":
            i += 2
            continue
        if c == "'" and quote is None:
            quote = "'"
        elif c == '"':
            quote = None if quote == '"' else '"'
        elif text.startswith("$((", i):
            i += 3
            continue
        elif text.startswith("$(", i):
            depth, j, inner = 1, i + 2, None
            while j < len(text) and depth:
                d = text[j]
                if d == "\\" and inner != "'":
                    j += 2
                    continue
                if inner:
                    inner = None if d == inner else inner
                elif d in "'\"":
                    inner = d
                elif d == "(":
                    depth += 1
                elif d == ")":
                    depth -= 1
                j += 1
            out.append(text[i + 2:j - 1] if depth == 0 else text[i + 2:])
            i = j
            continue
        elif c == "`":
            j = i + 1
            while j < len(text) and text[j] != "`":
                j += 2 if text[j] == "\\" else 1
            out.append(text[i + 1:j])
            i = j + 1
            continue
        i += 1
    return [s for s in out if s.strip()]
