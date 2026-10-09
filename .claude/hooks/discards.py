#!/usr/bin/env python3
"""The git commands that throw away what a tree holds, read before they run.

Why this exists. The commit guard read how work LEAVES the index, a commit and
the conclusion of a merge, a pick, a revert, a rebase or an am (CLAUDE.md
§ Hard stops, defects 403 and 487), and none of the ways it is thrown away.
Defect 514, lane b15-hooks, 2026-10-08; measured again on 2026-10-09 in
scratch repositories, git 2.56.0, with another session's new file and its
change to a tracked file staged beside a stopped operation: `git merge
--abort`, `git cherry-pick --abort` and `--skip`, `git revert --abort` and
`--skip`, `git rebase --abort` and `--skip` by either backend, `git am
--abort` and `--skip`, `git reset --merge` and `--hard`, and `git checkout
-f` and `git switch --discard-changes` of the whole tree each exit 0 with the
new file deleted from the disk and the change put back as HEAD has it.

Two depths, measured the same day. A rebase's abort and skip, `reset
--hard` and a forced checkout or switch put back every tracked file, an
UNSTAGED change as well; the rest
reset what is staged and leave an unstaged change on the disk, as `reset
--merge` does (an am's abort and skip of a rebase by the apply backend too).
An untracked file stays under all of them, and `--quit` leaves the index and
the working tree as they are.

So such a command is refused where the tree holds a change it would throw away
in a file the operation standing did not bring (`commits.brought`, the files
it may itself put back), a reset's with no operation standing being every such
change. Which command ends which operation, measured: a merge's `--abort`
ends a merge; a pick's and a revert's end either (`git cherry-pick --abort`
ended a revert, `git revert --abort` a pick); a rebase's end a rebase by
either backend and refuse an am; an am's end an am or a rebase by the apply
backend. Where nothing it ends stands, git refuses and this gives no opinion.

And `--autostash` is a stash (defect 514's last shape): a rebase, a merge or
a pull begun with it takes every change to a tracked file, staged or not,
whosever, off the disk while it stands (a rebase stopped on a conflict held
neither file, measured). It is in force by the flag, by `rebase.autoStash` or
`merge.autoStash` from any configuration git reads, `-c` and the environment
included, and refused where the tree holds such a change; `--no-autostash`
lifts it, and git then refuses over a change it would lose.

**And what names its paths is read too** (defect 529, `overwrites.py`): a
restore of the index, a checkout from a commit, a forced removal or move, a
reset of the index alone, by its paths or of the whole index, are refused
where a path they name holds a staged change they would throw away.
"""

import os
import re
import subprocess

import commits
import staged

# `overwrites.py`, imported by `verdict` the first time a verb of its own is
# read: every git command imports this module, and most name none of them.
overwrites = None

CONTRACT = "CLAUDE.md § Hard stops"
ASSIGNMENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*=")
# The verbs read here: the ones whose `--abort` or `--skip` ends an operation,
# the reset, the ones that may begin with an autostash, and the ones that
# write over the paths they name (defect 529).
ENDING = ("merge", "cherry-pick", "revert", "rebase", "am")
NAMING = ("restore", "rm", "mv")
VERBS = ENDING + ("reset", "checkout", "switch", "pull") + NAMING
# The ends that put back every tracked file, an unstaged change as well.
HARD = frozenset({"rebase"})
# The words with which a rebase or a merge does not begin one, and so takes
# no stash; a pull always begins one.
NOT_BEGUN = {
    "rebase": ("--continue", "--abort", "--skip", "--quit", "--edit-todo", "--show-current-patch"),
    "merge": ("--continue", "--abort", "--quit"),
    "pull": (),
}
FALSE = frozenset({"false", "no", "off", "0", ""})
# A checkout's or a switch's long options whose value may be the next word.
VALUED = frozenset({"--orphan", "--conflict", "--create", "--force-create", "--pathspec-from-file"})


def git_options(w):
    """git's own options before its verb in the words `w`, with `-C` and its
    directory left out, the guard's `git_dir` having read those into the
    directory the command runs in: a `-c` asked of `git config` reads as the
    command would (`-c rebase.autoStash=true`). Here and not in the guard,
    whose own text Python compiles before every Bash call (measured
    2026-10-09: two functions there cost an `ls` 2.1 million instructions)."""
    out = []
    i = 1
    while i < len(w) and w[i].startswith("-"):
        if w[i] == "-C" and i + 1 < len(w):
            i += 2
        elif w[i] in ("-c", "--git-dir", "--work-tree", "--namespace") and i + 1 < len(w):
            out += [w[i], w[i + 1]]
            i += 2
        else:
            out.append(w[i])
            i += 1
    return out


def environment(words):
    """The environment a segment's command runs with: this process's, and the
    assignments its words hold before `git`, `env`'s among them, one whose
    value the text cannot tell left out (`GIT_CONFIG_COUNT=1 ... git rebase`
    takes an autostash from there, measured 2026-10-09)."""
    env = dict(os.environ)
    for word in words:
        if os.path.basename(word) == "git":
            break
        if ASSIGNMENT.match(word):
            name, value = word.split("=", 1)
            if "$" not in value and "`" not in value:
                env[name] = value
    return env


def prefix_of(word, option, shortest):
    """Whether `word` is `option` or an abbreviation of it git may read, at
    least `shortest` characters long."""
    return len(word) >= shortest and option.startswith(word)


def ends(verb, words):
    """`--abort` or `--skip` where the words after `git <verb>` end what
    stands, or None. git takes `--ab` and `--sk` and longer for a merge, a
    rebase and an am, and no abbreviation for a pick or a revert; `--a` and
    `--s` are ambiguous for every verb measured (2026-10-09), so reading every
    prefix from three characters refuses nothing git would run. A merge has
    no `--skip`."""
    for word in options_of(words):
        if prefix_of(word, "--abort", 3):
            return "--abort"
        if verb != "merge" and prefix_of(word, "--skip", 3):
            return "--skip"
    return None


def options_of(words):
    """The words before a `--`."""
    out = []
    for word in words:
        if word == "--":
            break
        out.append(word)
    return out


def reset_mode(words):
    """The mode of `git reset <words>`, the last one named, as git reads it:
    `--h` and longer for `--hard`, `--me` and longer for `--merge` (`--m` is
    ambiguous with `--mixed`), measured 2026-10-09."""
    mode = None
    for word in options_of(words):
        if prefix_of(word, "--hard", 3):
            mode = "--hard"
        elif prefix_of(word, "--merge", 4):
            mode = "--merge"
        elif prefix_of(word, "--keep", 3) or prefix_of(word, "--soft", 4) or prefix_of(word, "--mixed", 4):
            mode = word
    return mode


def forced(verb, words, where):
    """Whether `git checkout` or `git switch` with `words`, run in `where`,
    puts back every tracked file as a commit has it: a checkout by
    `-f` (`--f` and longer, or an `f` in a cluster of flags) that names no
    path, at most one commit before any `--` and nothing after it; a switch
    by `-f` or `--discard-changes` (`--di` and longer, `--f` being
    `--force-create` too). Measured 2026-10-09: `git checkout -f`, `-f
    <branch>`, `-f HEAD` and `-f <branch> --`, and `git switch -f` and
    `--discard-changes`, each threw away a staged new file, a staged change
    and an unstaged one; `git checkout -f <path>` touched that path alone."""
    options, after, plain, short = [], [], [], ""
    seen_dashes = False
    skip = False
    for word in words:
        if seen_dashes:
            after.append(word)
        elif word == "--":
            seen_dashes = True
        elif skip:
            skip = False
        elif word.startswith("--"):
            options.append(word)
            skip = word in VALUED
        elif word.startswith("-") and word != "-":
            # A cluster's letters up to one that takes a value, the rest of
            # the word or the next one: `-bfix` names a branch and forces
            # nothing.
            for at, letter in enumerate(word[1:]):
                short += letter
                if letter in "bBcC":
                    skip = at == len(word) - 2
                    break
        else:
            plain.append(word)
    if "p" in short or any(w.startswith("--pathspec-from-file") or prefix_of(w, "--patch", 4) for w in options):
        return False
    if verb == "switch":
        return "f" in short or any(prefix_of(w, "--force", 5) or prefix_of(w, "--discard-changes", 4) for w in options)
    if not ("f" in short or any(prefix_of(w, "--force", 3) for w in options)):
        return False
    if after or len(plain) > 1:
        return False
    if not plain:
        return True
    named = staged.git(where, "rev-parse", "--verify", "-q", plain[0] + "^{commit}")
    return named is not None and bool(named.strip())


def ended_by(verb, name, verbs):
    """Whether `git <verb> --abort` or `--skip` ends the operation `name`,
    which `verbs` conclude (`commits.standing`)."""
    if verb in ("cherry-pick", "revert"):
        return name in ("cherry-pick", "revert")
    return verb in verbs


def thrown_away(where, said, hard, verb=None):
    """A refusal for `said`, which throws away what the index holds, and with
    `hard` an unstaged change too, where the tree `where` stands in holds such
    a change in a file no operation standing brought; None otherwise, or when
    git cannot say. `verb`, for an abort or a skip, is what must stand."""
    top = staged.toplevel(where)
    if top is None:
        return None
    now = commits.standing(top)
    if verb is not None and not any(ended_by(verb, name, verbs) for name, verbs, _what in now):
        return None
    own = set()
    for name, _verbs, what in now:
        got = commits.brought(top, name, what)
        if got is None:
            return None
        own |= got
    held = staged.git(top, "diff", "--cached", "--name-only", "--no-renames")
    if held is None:
        return None
    changed = staged.lines(held)
    if hard:
        loose = staged.git(top, "diff", "--name-only", "--no-renames")
        if loose is None:
            return None
        changed += [f for f in staged.lines(loose) if f not in changed]
    extra = [f for f in changed if f not in own]
    if not extra:
        return None
    what = "every change to a tracked file, staged or not" if hard else "every staged change"
    named = [name for name, _verbs, _what in now]
    beside = (" the " + " and the ".join(named) + " standing did not bring") if named else ""
    advice = (
        "Unstaging does not keep them here, since it puts back an unstaged change too: commit "
        "yours by their paths first (`git commit -- <paths>`), and leave another session's to it."
        if hard else
        "Unstage them first (`git restore --staged -- <paths>`), which keeps their content on the "
        "disk, since it leaves an unstaged change and an untracked file alone; or commit yours by "
        "their paths."
    )
    return (
        "refused: `" + said + "` throws away " + what + ", and this tree holds "
        + str(len(extra)) + beside + ": " + ", ".join(extra[:12])
        + ("" if len(extra) <= 12 else ", ...") + ". A new file is deleted from the disk and a "
        "changed one put back as the commit has it, whosever they are (measured, git 2.56.0). "
        + advice + " " + CONTRACT + " (CL-041, defect 514)"
    )


def flagged(words):
    """True or False where the words name `--autostash` or `--no-autostash`,
    the last of them, by every abbreviation from `--au` and `--no-au`; None
    where they name neither. git reads `--au` and longer for a merge and a
    pull and `--autost` and longer for a rebase (`--autos` is also
    `--autosquash` there), measured 2026-10-09."""
    said = None
    for word in options_of(words):
        if prefix_of(word, "--autostash", 4):
            said = True
        elif prefix_of(word, "--no-autostash", 7):
            said = False
    return said


def config(top, options, env, key):
    """`git <options> config --get <key>` in `top` with `env`, lowercased, or
    None where nothing sets it or git cannot say."""
    try:
        done = subprocess.run(
            ["git"] + list(options) + ["config", "--get", key],
            capture_output=True, timeout=30, cwd=top, text=True, env=env,
        )
    except (OSError, subprocess.SubprocessError):
        return None
    if done.returncode != 0:
        return None
    return done.stdout.strip().lower()


def truthy(value):
    return value is not None and value not in FALSE


def pull_rebases(words, top, options, env):
    """Whether `git pull <words>` rebases: the last of `--rebase` (`--reb` and
    longer, `-r`, `--rebase=<how>`) and `--no-rebase` (`--no-reb` and
    longer), else `branch.<the branch>.rebase`, else `pull.rebase`; every
    reading measured 2026-10-09."""
    said = None
    for word in options_of(words):
        if word == "-r" or prefix_of(word, "--rebase", 5):
            said = True
        elif word.startswith("--rebase="):
            said = word[len("--rebase="):].lower() not in FALSE
        elif prefix_of(word, "--no-rebase", 8):
            said = False
    if said is not None:
        return said
    branch = staged.git(top, "symbolic-ref", "--short", "-q", "HEAD")
    if branch and branch.strip():
        value = config(top, options, env, "branch." + branch.strip() + ".rebase")
        if value is not None:
            return value not in FALSE
    value = config(top, options, env, "pull.rebase")
    return value is not None and value not in FALSE


def autostash(verb, words, where, options, env):
    """A refusal for `git <options> <verb> <words>` where it begins with an
    autostash and the tree holds a change to a tracked file it would take, or
    None."""
    if verb not in NOT_BEGUN:
        return None
    if any(prefix_of(word, stop, 4) for word in options_of(words) for stop in NOT_BEGUN[verb]):
        return None
    top = staged.toplevel(where)
    if top is None:
        return None
    on = flagged(words)
    whence = "`--autostash`"
    if on is None:
        if verb == "pull":
            verb_of = "rebase" if pull_rebases(words, top, options, env) else "merge"
        else:
            verb_of = verb
        key = verb_of + ".autoStash"
        on = truthy(config(top, options, env, key))
        whence = "`" + key + "` in the configuration git reads here"
    if not on:
        return None
    held = staged.git(top, "status", "--porcelain", "--untracked-files=no")
    if held is None:
        return None
    changed = [line[3:] for line in held.split("\n") if len(line) > 3]
    if not changed:
        return None
    return (
        "refused: `git " + verb + "` with " + whence + " stashes the " + str(len(changed))
        + " change" + ("s" if len(changed) > 1 else "") + " this tree holds to a tracked file, "
        "staged or not, whosever: " + ", ".join(changed[:12]) + ("" if len(changed) <= 12 else ", ...")
        + ". They are off the disk while it stands, and a rebase stopped on a conflict holds neither "
        "(measured, git 2.56.0): `git stash` by another name. Add `--no-autostash`, and git refuses "
        "over a change it would lose and names it. " + CONTRACT + " (CL-041, defect 514)"
    )


def verdict(verb, words, where, options=(), env=None, line=None, appended=False):
    """A refusal for `git <options> <verb> <words>` run in `where`, or None.
    `options` are git's own before the verb, `-C` left out, `where` having
    read it; `env` the environment the command runs with; `line` the whole
    command line, which a list of paths it writes is read against;
    `appended`, that `xargs` runs it and adds words the text does not hold."""
    global overwrites
    if where is None:
        return None
    env = dict(os.environ) if env is None else env
    if overwrites is None and verb in ("reset", "checkout") + NAMING:
        import overwrites as overwrites_module

        overwrites = overwrites_module
    if verb == "reset":
        mode = reset_mode(words)
        if mode in ("--hard", "--merge"):
            return thrown_away(where, "git reset " + mode, hard=mode == "--hard")
        return overwrites.verdict(verb, words, where, options, env, line, appended)
    if verb in ("checkout", "switch"):
        if forced(verb, words, where):
            return thrown_away(where, "git " + verb + " " + ("--discard-changes" if verb == "switch" else "-f"), hard=True)
        return overwrites.verdict(verb, words, where, options, env, line, appended) if verb == "checkout" else None
    if verb in NAMING:
        return overwrites.verdict(verb, words, where, options, env, line, appended)
    if verb in ENDING:
        how = ends(verb, words)
        if how is not None:
            return thrown_away(where, "git " + verb + " " + how, hard=verb in HARD, verb=verb)
    return autostash(verb, words, where, options, env)
