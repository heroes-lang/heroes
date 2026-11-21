#!/usr/bin/env python3
"""The sweep: what finished lanes leave under `.claude/worktrees/`, removed.

Why this exists. The author's instruction of 2026-11-21, meant as: *put these
cleanups as a rule: every so often the system must clean the worktrees and the
agents, because I do not want to waste these things any more.* That day the
coordinator found 4 abandoned worktrees, 38 merged branches, 25 scratch folders
of finished lanes (about 23 GB with its own panel copies) and 7 background
shell loops waiting 7 to 11 hours on runs long finished, and removed them by
hand. The rule is `.claude/rules/records.md` § Working in lanes, its paragraph
on the sweep; this file performs it, and `sweep.hero` is its Heroes twin, held
to the same report by `test_twins.py`.

WHEN. On every session's start (`SessionStart`) and on a submitted prompt
(`UserPromptSubmit`) at most once an hour, the hour read from the stamp
`.claude/worktrees/.sweep-stamp`; both are `async` in `.claude/settings.json`,
so no prompt waits on a sweep and no timeout cuts a removal in half (Claude
Code enforces none on an async hook, and delivers its JSON `additionalContext`
on the session's next turn). Two sweeps never run at once: the folder
`.claude/worktrees/.sweep-lock` is made before anything is asked and removed
after, and one older than 30 minutes is a sweep that died. By hand, from any
checkout of the repository: `python3 .claude/hooks/sweep.py --dry-run` says
what it would do and does nothing; `--sweep` does it.

WHAT IT REMOVES, each only where every condition holds; a question the system
does not answer keeps the thing it was asked about:

- a WORKTREE directly under `<root>/.claude/worktrees/` (never the main
  checkout, never one elsewhere): no process has its working folder inside it
  or names it on its command line (`lsof -a -d cwd`, `ps`), nothing in it
  outside `build/` changed within 6 hours, it is not locked, `git status
  --porcelain` is empty, and its HEAD is reachable from another local branch's
  head or from `origin/main`. A lane just made is clean and holds no commit of
  its own, so the age and the processes are what keep it. Then `git worktree
  remove`, never `--force`, and its branch goes to the branches' rule.
- a FOLDER there that is no worktree (a lane's `<x>-scratch/`, a panel's
  copies): no worktree `<x>` stands, no process is inside it or names it,
  nothing in it changed within 6 hours, it holds no `.git` of its own, and no
  tracked file names it (`git grep -F <name>` over every local branch's head,
  `origin/main` and the main checkout's tracked files). Then `rm -rf`.
- a local BRANCH no worktree holds, but `main`, that did not move within 6
  hours (its reflog): `git branch -d`, never `-D`, run in a checkout whose HEAD
  holds it (or anywhere where it has an upstream, which git then asks), so git
  refuses an unmerged one itself.
- a WAIT LOOP: a shell whose script, through Claude Code's `eval '...'`, is one
  `until`/`while` loop whose body only sleeps and prints, sleeping in it now
  (its children are `sleep` alone), over 3 hours old, a child of the Claude
  Code process (its parent's command line names `claude`), working inside this
  repository, whose condition runs only readers (`[`, `test`, `grep`, `tail`
  and the rest of `READERS`) and watches no process, and whose watched paths,
  or where a path is not there yet the folder that would hold it, changed
  nothing within the hour. It gets `SIGTERM`, and so do its `sleep` children.

WHAT IT NEVER TOUCHES: the main checkout, a worktree with uncommitted work or
commits of its own (reported, with what it holds), anything outside
`.claude/worktrees/` but local branches and those loops, a remote, a compiler,
a test, a harness run, an agent. Before acting it asks the processes again,
and a thing that came into use since it was judged is kept.

WHAT IT SAYS: one short report, what it removed, what it kept and why, the disk
freed (`du -sk` before each removal); as a hook, the report is the session's
context.

Every question is asked of a program, `git`, `find`, `ps`, `lsof`, `du`, as
the twin asks it, so the two read the same answers: a time is never compared
here, `find -mmin` and `ps`'s elapsed time compare it.
"""

import json
import os
import signal
import subprocess
import sys

HOOKS = os.path.dirname(os.path.abspath(__file__))
if HOOKS not in sys.path:
    sys.path.insert(0, HOOKS)

import guard_bash  # noqa: E402
import trees  # noqa: E402

RULE = ".claude/rules/records.md § Working in lanes, the sweep"
FOLDER = os.path.join(".claude", "worktrees")
STAMP = ".sweep-stamp"
LOCK = ".sweep-lock"
TRUNK = "main"
ORIGIN_MAIN = "refs/remotes/origin/main"
# Minutes: a worktree, a folder or a branch quiet this long may go; a loop's
# watched path quiet this long no longer keeps it; the stamp's hour; a lock
# this old belongs to a sweep that died.
QUIET = 360
WATCHED = 60
HOURLY = 60
STALE_LOCK = 30
# Seconds: a wait loop younger than this is left alone.
LOOP_AGE = 3 * 3600
# Seconds any one question may take before it counts as unanswered.
LIMIT = 120
# What a wait loop's body may run; one of them must be `sleep`.
WAITERS = frozenset({"sleep", "true", ":", "echo", "printf", "date"})
# What a wait loop's condition may run, each a reader of files.
READERS = frozenset({"[", "[[", "test", "true", "false", ":", "grep", "egrep", "fgrep",
                     "tail", "head", "cat", "ls", "stat", "wc", "find"})
# A condition running one of these watches a process, which is alive while the
# loop runs, so the loop is kept.
WATCHERS = frozenset({"kill", "pgrep", "ps", "pidof", "jobs", "wait", "lsof", "fuser"})
# Words of a condition that are no path.
OPERATORS = frozenset({"]", "]]", "!", "=", "==", "!=", "<", ">", "(", ")", "&&", "||"})
GREPS = frozenset({"grep", "egrep", "fgrep"})
# The longest loop text a report quotes, in bytes.
QUOTED = 160


# ---------------------------------------------------------------- asking


class Asked:
    """What a program answered: whether it started, its exit, its streams."""

    def __init__(self, started, code, out, err):
        self.started = started
        self.code = code
        self.out = out
        self.err = err

    def ok(self):
        return self.started and self.code == 0


def ask(words):
    """`words` run, the first the program found on the PATH. A stream that is
    not UTF-8 is an answer not given, so a `git status` naming a path that
    is not UTF-8 keeps its worktree rather than reading as clean."""
    try:
        done = subprocess.run(words, capture_output=True, timeout=LIMIT)
        return Asked(True, done.returncode, done.stdout.decode("utf-8"), done.stderr.decode("utf-8"))
    except (OSError, subprocess.SubprocessError, UnicodeDecodeError) as why:
        return Asked(False, -1, "", str(why))


def git(where, *args):
    return ask(["git", "--no-optional-locks", "-C", where] + list(args))


def lines(text):
    """The lines of `text`, an empty last one dropped: split at `\\n` alone,
    as the twin splits."""
    out = text.split("\n")
    if out and out[-1] == "":
        out.pop()
    return out


def first_line(text):
    for line in lines(text):
        if line.strip(" \t"):
            return line.strip(" \t")
    return "no reason given"


def number(text):
    """`text` as a count when it is ASCII digits alone, else -1."""
    if not text or len(text) > 18 or any(c < "0" or c > "9" for c in text):
        return -1
    return int(text)


def changed_within(path, minutes, skip_build=False, shallow=False):
    """True where something at or under `path` changed within `minutes`
    (`find -mmin`, which `build/` is left out of where `skip_build`), False
    where nothing did, None where `find` could not say."""
    words = ["find", path]
    if shallow:
        words += ["-maxdepth", "1"]
    if skip_build:
        words += ["-path", path + "/build", "-prune", "-o"]
    words += ["-mmin", "-" + str(minutes), "-print", "-quit"]
    asked = ask(words)
    if not asked.started:
        return None
    if asked.out != "":
        return True
    if asked.code == 0:
        return False
    return None


def kilobytes(path):
    asked = ask(["du", "-sk", path])
    if not asked.ok():
        return 0
    head = asked.out.split("\t")[0].strip(" \n")
    found = number(head)
    return found if found >= 0 else 0


# ---------------------------------------------------------------- the repository


def read(path):
    try:
        with open(path, encoding="utf-8") as handle:
            return handle.read()
    except (OSError, ValueError):
        return None


def repository_root(start):
    """The main checkout of the repository whose tree `start` stands in,
    resolved, where it is a tree of this project; None elsewhere. Read from
    the files git keeps, a worktree's `.git` naming its folder under the
    main checkout's `.git/worktrees/` and that folder's `commondir` the way
    back, so no program is asked before the sweep holds its lock."""
    tree = trees.tree_of(start)
    if tree is None:
        return None
    dot = os.path.join(tree, ".git")
    if os.path.isdir(dot):
        common = dot
    else:
        text = read(dot)
        if text is None or not text.startswith("gitdir: "):
            return None
        gitdir = text[len("gitdir: "):].rstrip("\n")
        if not os.path.isabs(gitdir):
            gitdir = os.path.join(tree, gitdir)
        back = read(os.path.join(gitdir, "commondir"))
        if back is None:
            return None
        back = back.rstrip("\n")
        common = back if os.path.isabs(back) else os.path.join(gitdir, back)
    common = os.path.normpath(common)
    if os.path.basename(common) != ".git" or not os.path.isdir(common):
        return None
    root = os.path.realpath(os.path.dirname(common))
    if trees.tree_of(root) != root:
        return None
    return root


class Worktree:
    def __init__(self, path):
        self.path = path
        self.head = ""
        self.branch = ""
        self.locked = False
        self.prunable = False


def worktrees(root):
    """Every worktree git records, the main checkout first, or None."""
    asked = git(root, "worktree", "list", "--porcelain")
    if not asked.ok():
        return None
    out = []
    current = None
    for field in asked.out.split("\n"):
        if field == "":
            if current is not None:
                out.append(current)
                current = None
            continue
        if field.startswith("worktree "):
            if current is not None:
                out.append(current)
            current = Worktree(field[len("worktree "):])
        elif current is None:
            continue
        elif field.startswith("HEAD "):
            current.head = field[len("HEAD "):]
        elif field.startswith("branch refs/heads/"):
            current.branch = field[len("branch refs/heads/"):]
        elif field == "locked" or field.startswith("locked "):
            current.locked = True
        elif field == "prunable" or field.startswith("prunable "):
            current.prunable = True
    if current is not None:
        out.append(current)
    return out


def resolved(path):
    return os.path.realpath(path) if os.path.lexists(path) else path


def origin_main(root):
    asked = git(root, "for-each-ref", "--format=%(objectname)", ORIGIN_MAIN)
    return asked.out.rstrip("\n") if asked.ok() else ""


def entries(folder):
    """The folders directly in `folder`, links left out, by name, sorted; a
    name opening with `.` is the sweep's own or the system's. None where
    `find` cannot list it."""
    asked = ask(["find", folder, "-mindepth", "1", "-maxdepth", "1", "-type", "d"])
    if not asked.ok():
        return None
    names = []
    for line in lines(asked.out):
        name = line[len(folder) + 1:] if line.startswith(folder + "/") else ""
        if name and not name.startswith(".") and "/" not in name:
            names.append(name)
    return sorted(names)


# ---------------------------------------------------------------- the processes


class Process:
    def __init__(self, pid, ppid, age, command):
        self.pid = pid
        self.ppid = ppid
        self.age = age
        self.command = command


def elapsed(text):
    """`ps`'s elapsed time, `[[dd-]hh:]mm:ss`, in seconds, or -1."""
    days = 0
    rest = text
    if "-" in rest:
        head, _dash, rest = rest.partition("-")
        days = number(head)
        if days < 0:
            return -1
    parts = rest.split(":")
    if len(parts) < 2 or len(parts) > 3:
        return -1
    seconds = 0
    for part in parts:
        value = number(part)
        if value < 0:
            return -1
        seconds = seconds * 60 + value
    return days * 86400 + seconds


def ps_line(line):
    """A line of `ps -o pid=,ppid=,etime=,command=`, or None."""
    fields = []
    at = 0
    for _ in range(3):
        while at < len(line) and line[at] == " ":
            at += 1
        start = at
        while at < len(line) and line[at] != " ":
            at += 1
        if start == at:
            return None
        fields.append(line[start:at])
    if at < len(line):
        at += 1
    pid, ppid, age = number(fields[0]), number(fields[1]), elapsed(fields[2])
    if pid < 0 or ppid < 0 or age < 0:
        return None
    return Process(pid, ppid, age, line[at:])


def processes():
    asked = ask(["ps", "-axww", "-o", "pid=,ppid=,etime=,command="])
    if not asked.ok():
        return None
    out = []
    for line in lines(asked.out):
        found = ps_line(line)
        if found is not None:
            out.append(found)
    return out


def working_folders():
    """Each process's working folder, by its pid, as `lsof` names it; None
    where `lsof` gives nothing."""
    asked = ask(["lsof", "-a", "-d", "cwd", "-F", "pn"])
    if not asked.started or asked.code not in (0, 1) or asked.out == "":
        return None
    out = {}
    pid = -1
    for line in lines(asked.out):
        if line.startswith("p"):
            pid = number(line[1:])
        elif line.startswith("n") and pid >= 0:
            out[pid] = line[1:]
    return out


class Snapshot:
    def __init__(self, table, folders):
        self.table = table
        self.folders = folders

    def complete(self):
        return self.table is not None and self.folders is not None


def snapshot():
    return Snapshot(processes(), working_folders())


def under(path, folder):
    return path == folder or path.startswith(folder + "/")


def name_char(c):
    return c.isascii() and (c.isalnum() or c in "._-")


def names(command, name):
    """Whether `command` names a path whose component is `name`: `/<name>`
    followed by an end, a `/` or a byte no name holds."""
    piece = "/" + name
    at = command.find(piece)
    while at >= 0:
        end = at + len(piece)
        if end == len(command) or not name_char(command[end]):
            return True
        at = command.find(piece, at + 1)
    return False


def in_use(shot, path, name):
    """Whether a process works inside `path` or names it."""
    for pid in shot.folders:
        if under(shot.folders[pid], path):
            return True
    for one in shot.table:
        if names(one.command, name):
            return True
    return False


# ---------------------------------------------------------------- wait loops


def decoded(command):
    """`ps`'s text with its octal escapes of control bytes, `\\012` for a new
    line, read back."""
    out = []
    at = 0
    while at < len(command):
        if command[at] == "\\" and at + 4 <= len(command) and all("0" <= c <= "7" for c in command[at + 1:at + 4]):
            value = int(command[at + 1:at + 4], 8)
            if value < 32 or value == 127:
                out.append(chr(value))
                at += 4
                continue
        out.append(command[at])
        at += 1
    return "".join(out)


def shell_script(command):
    """The script a shell's `-c` runs, read off `ps`'s line, or None."""
    words = command.split(" ")
    if os.path.basename(words[0]) not in guard_bash.SHELLS:
        return None
    at = 1
    while at < len(words) and (words[at].startswith("-") or words[at].startswith("+")) and words[at] != "--":
        word = words[at]
        if word in ("-o", "+o", "-O", "+O"):
            at += 2
            continue
        if not word.startswith("--") and "c" in word[1:]:
            return decoded(" ".join(words[at + 1:]))
        at += 1
    return None


def flat(script, depth=0):
    """The segments `script` runs, each (its text, its words), an `eval`'s
    script read in its place."""
    out = []
    for segment in guard_bash.segments(script):
        words = guard_bash.bare(guard_bash.words(segment))
        if words and os.path.basename(words[0]) == "eval" and depth < 4:
            out.extend(flat(" ".join(words[1:]), depth + 1))
        elif words:
            out.append((segment, words))
    return out


class Loop:
    def __init__(self, condition, body, text, assigned):
        self.condition = condition
        self.body = body
        self.text = text
        self.assigned = assigned


def assignment(word):
    eq = word.find("=")
    if eq <= 0:
        return False
    head = word[:eq]
    return (head[0].isascii() and (head[0].isalpha() or head[0] == "_")) and all(
        c.isascii() and (c.isalnum() or c == "_") for c in head)


def wait_loop(script):
    """The one `while`/`until` loop of `script` whose body only sleeps and
    prints, or None."""
    segments = flat(script)
    starts = [at for at, (_text, words) in enumerate(segments) if words[0] in ("while", "until", "for")]
    if len(starts) != 1 or segments[starts[0]][1][0] == "for":
        return None
    begin = starts[0]
    condition = [segments[begin][1][1:]]
    at = begin + 1
    while at < len(segments) and segments[at][1][0] != "do":
        condition.append(segments[at][1])
        at += 1
    if at == len(segments):
        return None
    body = [segments[at][1][1:]] if len(segments[at][1]) > 1 else []
    at += 1
    while at < len(segments) and segments[at][1][0] != "done":
        body.append(segments[at][1])
        at += 1
    if at == len(segments):
        return None
    slept = False
    for words in body:
        command = os.path.basename(words[0])
        if command not in WAITERS:
            return None
        slept = slept or command == "sleep"
    if not slept:
        return None
    assigned = []
    for _text, words in segments[:begin]:
        if all(assignment(word) for word in words):
            for word in words:
                eq = word.find("=")
                assigned.append((word[:eq], word[eq + 1:]))
    text = "; ".join(segment for segment, _words in segments[begin:at + 1])
    return Loop(condition, body, quoted(text), assigned)


def quoted(text):
    raw = text.encode("utf-8")
    if len(raw) <= QUOTED:
        return text
    cut = QUOTED
    while cut > 0 and raw[cut] & 0xC0 == 0x80:
        cut -= 1
    return raw[:cut].decode("utf-8") + "..."


def redirection(word):
    """0 where `word` is no redirection, 1 where it carries its target, 2
    where its target is the next word."""
    at = 0
    while at < len(word) and "0" <= word[at] <= "9":
        at += 1
    for op in ("&>>", "&>", ">>", ">&", "<&", ">|", "<<", ">", "<"):
        if word.startswith(op, at):
            return 1 if len(word) > at + len(op) else 2
    return 0


def operands(words):
    """The words of one condition command that may be paths."""
    command = os.path.basename(words[0])
    out = []
    pattern_given = False
    pattern_skipped = False
    at = 1
    while at < len(words):
        word = words[at]
        kind = redirection(word)
        if kind:
            at += kind
            continue
        if command in GREPS and word in ("-e", "--regexp"):
            pattern_given = True
            at += 2
            continue
        if command in GREPS and word in ("-f", "--file"):
            pattern_given = True
            if at + 1 < len(words):
                out.append(words[at + 1])
            at += 2
            continue
        if word in OPERATORS or word == "" or number(word) >= 0 or (word.startswith("-") and len(word) > 1):
            at += 1
            continue
        if command in GREPS and not pattern_given and not pattern_skipped:
            pattern_skipped = True
            at += 1
            continue
        out.append(word)
        at += 1
    return out


def known_values(assigned, home):
    """The script's assignments before its loop, each value read with the
    ones before it; None for a value that cannot be read."""
    known = {}
    for name, value in assigned:
        known[name] = expanded(value, known, home)
    return known


def expanded(word, known, home):
    """`word` with a leading `~` and each `$name` or `${name}` read from
    `known`, `$HOME` being `home` where the script set none; None where a `$`
    names what is not known."""
    if word == "~" or word.startswith("~/"):
        word = home + word[1:]
    out = []
    at = 0
    while at < len(word):
        if word[at] != "$":
            out.append(word[at])
            at += 1
            continue
        if word.startswith("${", at):
            end = word.find("}", at + 2)
            if end < 0:
                return None
            name, after = word[at + 2:end], end + 1
        else:
            end = at + 1
            while end < len(word) and word[end].isascii() and (word[end].isalnum() or word[end] == "_"):
                end += 1
            name, after = word[at + 1:end], end
        if name in known:
            value = known[name]
        elif name == "HOME":
            value = home
        else:
            value = None
        if name == "" or value is None:
            return None
        out.append(value)
        at = after
    return "".join(out)


def watch(loop, cwd, home):
    """Why the loop is kept on its condition, or "" where nothing keeps it:
    a process watched, a command that is no reader, a path read through a
    variable, a relative path where the loop's folder is unknown, or a watched
    path, or the folder that would hold it, changed within the hour."""
    known = known_values(loop.assigned, home)
    for words in loop.condition:
        rest = [word for word in words]
        while rest and rest[0] == "!":
            rest = rest[1:]
        if not rest:
            continue
        command = os.path.basename(rest[0])
        if command in WATCHERS:
            return "its condition watches a process (`" + command + "`)"
        if command not in READERS:
            return "its condition runs `" + command + "`"
        for word in operands(rest):
            path = expanded(word, known, home)
            if path is None:
                return "its condition names a path through a variable (`" + word + "`)"
            if not path.startswith("/"):
                if cwd == "":
                    return "its working folder is unknown"
                path = os.path.join(cwd, path)
            path = os.path.normpath(path)
            seen = path
            while not os.path.lexists(seen) and seen != "/":
                seen = os.path.dirname(seen)
            fresh = changed_within(seen, WATCHED, shallow=True)
            if fresh is None:
                return "`" + seen + "` could not be read"
            if fresh:
                if seen == path:
                    return "`" + path + "` changed within the hour"
                return "`" + seen + "`, which would hold `" + path + "`, changed within the hour"
    return ""


def names_claude(command):
    words = command.split(" ")
    if os.path.basename(words[0]) == "claude":
        return True
    return os.path.basename(words[0]) in ("node", "bun") and len(words) > 1 and "claude" in words[1]


def hours(seconds):
    return str(seconds // 3600) + "h" + str(seconds // 60 % 60).rjust(2, "0") + "m"


class Waiting:
    def __init__(self, process, loop, cwd, why):
        self.process = process
        self.loop = loop
        self.cwd = cwd
        self.why = why


def waiting(shot, root, home):
    """The wait loops over `LOOP_AGE` under Claude Code working in `root`,
    each with why it is kept, "" where it is to be stopped."""
    by_pid = {}
    for one in shot.table:
        by_pid[one.pid] = one
    out = []
    for one in shot.table:
        if one.age < LOOP_AGE:
            continue
        parent = by_pid.get(one.ppid)
        if parent is None or not names_claude(parent.command):
            continue
        cwd = shot.folders.get(one.pid, "")
        if not under(cwd, root):
            continue
        script = shell_script(one.command)
        if script is None:
            continue
        loop = wait_loop(script)
        if loop is None:
            continue
        why = ""
        for child in shot.table:
            if child.ppid == one.pid and os.path.basename(child.command.split(" ")[0]) != "sleep":
                why = "it runs `" + child.command.split(" ")[0] + "` now"
                break
        if why == "":
            why = watch(loop, cwd, home)
        out.append(Waiting(one, loop, cwd, why))
    return out


# ---------------------------------------------------------------- the sweep


class Report:
    def __init__(self, folder, dry):
        self.folder = folder
        self.dry = dry
        self.removed = []
        self.kept = []
        self.busy = []
        self.counts = {"worktree": 0, "folder": 0, "branch": 0, "loop": 0}
        self.kilobytes = 0

    def did(self, kind, line, size=0):
        self.counts[kind] += 1
        self.kilobytes += size
        self.removed.append(line)

    def text(self):
        def many(n, one, more):
            return str(n) + " " + (one if n == 1 else more)

        c = self.counts
        if self.dry:
            head = ("sweep, dry run, of " + self.folder + ": would remove "
                    + many(c["worktree"], "worktree", "worktrees") + " and "
                    + many(c["folder"], "folder", "folders") + ", delete "
                    + many(c["branch"], "branch", "branches") + ", stop "
                    + many(c["loop"], "wait loop", "wait loops") + ", free ")
        else:
            head = ("sweep of " + self.folder + ": removed "
                    + many(c["worktree"], "worktree", "worktrees") + " and "
                    + many(c["folder"], "folder", "folders") + ", deleted "
                    + many(c["branch"], "branch", "branches") + ", stopped "
                    + many(c["loop"], "wait loop", "wait loops") + ", freed ")
        out = [head + str(self.kilobytes // 1024) + " MB (" + RULE + ")."]
        out += ["- " + line for line in self.removed]
        out += ["- " + line for line in self.kept]
        if self.busy:
            out.append("- in use (a process inside it or naming it, a change within 6 hours, or its lane "
                       "standing), kept: " + ", ".join(self.busy))
        return "\n".join(out) + "\n"


def judge_worktree(shot, wt, name, origin):
    """Why the worktree `wt` stays, as (busy, reason): busy where it is in
    use, a reason where it holds something; (False, "") where it may go."""
    if in_use(shot, wt.path, name):
        return True, ""
    fresh = changed_within(wt.path, QUIET, skip_build=True)
    if fresh is None:
        return False, "`find` could not read all of it"
    if fresh:
        return True, ""
    if wt.locked:
        return False, "it is locked (`git worktree unlock` first)"
    status = git(wt.path, "status", "--porcelain")
    if not status.ok():
        return False, "`git status` failed: " + first_line(status.err)
    dirty = lines(status.out)
    if dirty:
        shown = [line[3:] for line in dirty[:3]]
        more = ", ..." if len(dirty) > 3 else ""
        return False, ("uncommitted work in " + str(len(dirty)) + (" path (" if len(dirty) == 1 else " paths (")
                       + ", ".join(shown) + more + ")")
    words = ["rev-list", "--count", wt.head, "--not"]
    if wt.branch:
        words.append("--exclude=" + wt.branch)
    words.append("--branches")
    if origin:
        words.append(origin)
    counted = git(wt.path, *words)
    own = number(counted.out.strip("\n")) if counted.ok() else -1
    if own < 0:
        return False, "`git rev-list` failed: " + first_line(counted.err)
    if own > 0:
        return False, (str(own) + (" commit" if own == 1 else " commits")
                       + " on no other local branch nor origin/main")
    return False, ""


def judge_folder(shot, folder, name, standing):
    """(busy, reason) for a folder that is no worktree, as `judge_worktree`."""
    lane = name[:-len("-scratch")] if name.endswith("-scratch") else name
    if lane in standing:
        return True, ""
    if in_use(shot, folder, name):
        return True, ""
    fresh = changed_within(folder, QUIET)
    if fresh is None:
        return False, "`find` could not read all of it"
    if fresh:
        return True, ""
    asked = ask(["find", folder, "-name", ".git", "-print", "-quit"])
    if not asked.ok():
        return False, "`find` could not read all of it"
    if asked.out != "":
        return False, "it holds a git repository of its own (" + lines(asked.out)[0][len(folder) + 1:] + ")"
    return False, ""


def citations(root, candidates, origin):
    """For each name, the tracked files that name it, or None where git could
    not be asked."""
    if not candidates:
        return {}
    heads = git(root, "for-each-ref", "--format=%(objectname)", "refs/heads")
    if not heads.ok():
        return None
    revisions = sorted(set(lines(heads.out) + ([origin] if origin else [])))
    patterns = []
    for name in candidates:
        patterns += ["-e", name]
    found = {name: [] for name in candidates}
    for revs in (revisions, []):
        asked = git(root, "grep", "-I", "-o", "-F", *patterns, *revs)
        if not asked.started or asked.code not in (0, 1):
            return None
        for line in lines(asked.out):
            for name in candidates:
                if line.endswith(":" + name):
                    start = line.find(":") + 1 if revs else 0
                    found[name].append(line[start:len(line) - len(name) - 1])
    return found


def branch_merged_sets(root, checkouts):
    """For each checkout, the local branches its HEAD holds."""
    out = []
    for wt in checkouts:
        asked = git(root, "for-each-ref", "--merged", wt.head, "--format=%(refname)", "refs/heads")
        held = set()
        if asked.ok():
            held = {ref[len("refs/heads/"):] for ref in lines(asked.out)}
        out.append((wt, held))
    return out


def recent_branches(root):
    logs = os.path.join(root, ".git", "logs", "refs", "heads")
    asked = ask(["find", logs, "-type", "f", "-mmin", "-" + str(QUIET)])
    return {line[len(logs) + 1:] for line in lines(asked.out) if line.startswith(logs + "/")}


# What `sweep` answers where another sweep holds the lock.
HELD = "held"


def take(lock):
    """Whether this sweep now holds `lock`: made by `mkdir`, which refuses a
    folder that stands, after one older than `STALE_LOCK` minutes, a sweep
    that died, is removed."""
    if ask(["mkdir", lock]).ok():
        return True
    ask(["find", lock, "-maxdepth", "0", "-mmin", "+" + str(STALE_LOCK), "-exec", "rm", "-rf", "{}", ";"])
    return ask(["mkdir", lock]).ok()


def sweep(start, dry, stamp="none"):
    """Sweeps the repository `start` stands in: the report's text; None where
    nothing here may be judged, or where `stamp` is "hourly" and the stamp is
    under an hour old; `HELD` where another sweep holds the lock. `stamp`
    "write" or "hourly" writes the stamp before the sweep."""
    root = repository_root(start)
    if root is None:
        return None
    folder = os.path.join(root, FOLDER)
    if not os.path.isdir(folder) or os.path.islink(folder):
        return None
    lock = os.path.join(folder, LOCK)
    if not take(lock):
        return HELD
    try:
        stamped = os.path.join(folder, STAMP)
        if stamp == "hourly" and changed_within(stamped, HOURLY, shallow=True):
            return None
        if stamp != "none":
            try:
                with open(stamped, "w", encoding="utf-8") as handle:
                    handle.write("sweep\n")
            except OSError:
                return None
        return swept(root, folder, dry)
    finally:
        ask(["rm", "-rf", lock])


def swept(root, folder, dry):
    report = Report(folder, dry)
    home = os.environ.get("HOME", "")
    records = worktrees(root)
    shot = snapshot()
    if records is None:
        report.kept.append("everything: `git worktree list` could not be asked")
        return report.text()
    if not shot.complete():
        report.kept.append("every worktree and folder: `ps` or `lsof` could not be asked, so no process is known")
    origin = origin_main(root)
    here = {}
    for wt in records[1:]:
        if os.path.dirname(resolved(wt.path)) == folder:
            here[os.path.basename(resolved(wt.path))] = wt
    standing = set(here)

    # Worktrees.
    going = []
    for name in sorted(here):
        wt = here[name]
        if wt.prunable:
            report.kept.append("worktree record " + name + ": its folder is gone (`git worktree prune` drops it)")
            continue
        if not shot.complete():
            continue
        wt.path = resolved(wt.path)
        busy, why = judge_worktree(shot, wt, name, origin)
        if busy:
            report.busy.append(name)
        elif why:
            report.kept.append("worktree " + name + ": " + why)
        else:
            going.append((wt, name))

    # Folders that are no worktree.
    names = entries(folder)
    if names is None:
        report.kept.append("every folder: `find` could not list " + folder)
        names = []
    candidates = []
    for name in names:
        if name in standing or not shot.complete():
            continue
        path = os.path.join(folder, name)
        busy, why = judge_folder(shot, path, name, standing)
        if busy:
            report.busy.append(name)
        elif why:
            report.kept.append("folder " + name + ": " + why)
        else:
            candidates.append(name)
    cited = citations(root, candidates, origin)
    clearing = []
    for name in candidates:
        if cited is None:
            report.kept.append("folder " + name + ": `git grep` could not be asked whether a tracked file names it")
        elif cited[name]:
            report.kept.append("folder " + name + ": a tracked file names it (" + min(cited[name]) + ")")
        else:
            clearing.append(name)

    # Wait loops.
    loops = waiting(shot, root, home) if shot.complete() else []
    stopping = []
    for one in loops:
        if one.why:
            report.kept.append("wait loop " + str(one.process.pid) + ", " + hours(one.process.age) + " old, in "
                               + one.cwd + ": " + one.why + ": " + one.loop.text)
        else:
            stopping.append(one)

    sizes = {}
    for wt, name in going:
        sizes[name] = kilobytes(wt.path)
    for name in clearing:
        sizes[name] = kilobytes(os.path.join(folder, name))

    if not dry and (going or clearing or stopping):
        fresh = snapshot()
        if not fresh.complete():
            report.kept.append("everything judged: `ps` or `lsof` could not be asked again before acting")
            going, clearing, stopping = [], [], []
        else:
            still = []
            for wt, name in going:
                if in_use(fresh, wt.path, name):
                    report.busy.append(name)
                else:
                    still.append((wt, name))
            going = still
            still = []
            for name in clearing:
                if in_use(fresh, os.path.join(folder, name), name):
                    report.busy.append(name)
                else:
                    still.append(name)
            clearing = still
            alive = {(p.pid, p.ppid, p.command) for p in fresh.table}
            stopping = [one for one in stopping
                        if (one.process.pid, one.process.ppid, one.process.command) in alive]
            shot = fresh

    removed = set()
    for wt, name in going:
        line = "worktree " + name + ", " + str(sizes[name] // 1024) + " MB"
        if dry:
            report.did("worktree", "would remove " + line, sizes[name])
            removed.add(wt.path)
            continue
        asked = git(root, "worktree", "remove", wt.path)
        if asked.ok():
            report.did("worktree", "removed " + line, sizes[name])
            removed.add(wt.path)
        else:
            report.kept.append("worktree " + name + ": `git worktree remove` refused: " + first_line(asked.err))
    for name in clearing:
        path = os.path.join(folder, name)
        line = "folder " + name + ", " + str(sizes[name] // 1024) + " MB"
        if dry:
            report.did("folder", "would remove " + line, sizes[name])
            continue
        asked = ask(["rm", "-rf", path])
        if asked.ok() and not os.path.lexists(path):
            report.did("folder", "removed " + line, sizes[name])
        else:
            report.kept.append("folder " + name + ": `rm -rf` failed: " + first_line(asked.err))

    # Branches, once the worktrees removed no longer hold theirs.
    branches(root, records, removed, dry, report)

    for one in stopping:
        line = ("wait loop " + str(one.process.pid) + ", " + hours(one.process.age) + " old, in " + one.cwd
                + ": " + one.loop.text)
        if dry:
            report.did("loop", "would stop " + line)
            continue
        stop(one.process.pid)
        for child in shot.table:
            if child.ppid == one.process.pid and os.path.basename(child.command.split(" ")[0]) == "sleep":
                stop(child.pid)
        report.did("loop", "stopped " + line)

    report.busy = sorted(set(report.busy))
    return report.text()


def stop(pid):
    try:
        os.kill(pid, signal.SIGTERM)
    except OSError:
        pass


def branches(root, records, removed, dry, report):
    listed = git(root, "for-each-ref", "--format=%(refname)%09%(objectname)%09%(upstream)", "refs/heads")
    if not listed.ok():
        report.kept.append("every branch: `git for-each-ref` could not be asked")
        return
    held = {wt.branch for wt in records if wt.branch and wt.path not in removed and resolved(wt.path) not in removed}
    checkouts = [wt for wt in records
                 if not wt.prunable and wt.head and wt.path not in removed and resolved(wt.path) not in removed]
    recent = None
    merged_sets = None
    for line in lines(listed.out):
        fields = line.split("\t")
        if len(fields) != 3 or not fields[0].startswith("refs/heads/"):
            continue
        name, sha, upstream = fields[0][len("refs/heads/"):], fields[1], fields[2]
        if name == TRUNK or name in held:
            continue
        if recent is None:
            recent = recent_branches(root)
        if name in recent:
            report.busy.append("branch " + name)
            continue
        host = ""
        why = ""
        if upstream:
            asked = git(root, "merge-base", "--is-ancestor", sha, upstream)
            if asked.started and asked.code == 0:
                host = root
            elif asked.started and asked.code == 1:
                why = "not merged into its upstream " + upstream
        if host == "" and why == "":
            if merged_sets is None:
                merged_sets = branch_merged_sets(root, checkouts)
            for wt, names_held in merged_sets:
                if name in names_held:
                    host = resolved(wt.path)
                    break
            if host == "":
                why = "no checkout's HEAD holds it, so `git branch -d` would refuse"
        if why:
            report.kept.append("branch " + name + ": " + why)
            continue
        if dry:
            report.did("branch", "would delete branch " + name)
            continue
        asked = git(host, "branch", "-d", name)
        if asked.ok():
            report.did("branch", "deleted branch " + name)
        else:
            report.kept.append("branch " + name + ": `git branch -d` refused: " + first_line(asked.err))


# ---------------------------------------------------------------- the hook


def hook_answer(event_name, text):
    return json.dumps({"hookSpecificOutput": {"hookEventName": event_name, "additionalContext": text}})


def as_hook(raw):
    """The hook's stdout for the event `raw`, "" where it says nothing."""
    try:
        payload = json.loads(raw)
    except ValueError:
        return ""
    if not isinstance(payload, dict):
        return ""
    event_name = payload.get("hook_event_name")
    if event_name not in ("SessionStart", "UserPromptSubmit"):
        return ""
    start = payload.get("cwd")
    if not isinstance(start, str) or not start:
        start = os.environ.get("CLAUDE_PROJECT_DIR") or os.getcwd()
    text = sweep(start, dry=False, stamp="hourly" if event_name == "UserPromptSubmit" else "write")
    if text is None or text == HELD:
        return ""
    return hook_answer(event_name, text) + "\n"


def by_hand(start, dry):
    """What `--dry-run` and `--sweep` print."""
    text = sweep(start, dry=dry)
    if text is None:
        return ("sweep: " + start + " is in no checkout of this project with a " + FOLDER
                + " folder, so nothing was judged.\n")
    if text == HELD:
        return "sweep: another sweep holds " + LOCK + ", so this one did nothing.\n"
    return text


def main(argv):
    if argv == ["--dry-run"] or argv == ["--sweep"]:
        sys.stdout.write(by_hand(os.getcwd(), dry=argv == ["--dry-run"]))
        return 0
    if argv:
        sys.stderr.write("usage: sweep [--dry-run | --sweep]; with no argument it is the hook, its event on stdin\n")
        return 2
    try:
        out = as_hook(sys.stdin.read())
    except Exception as why:  # noqa: BLE001 - a hook never stops a session
        out = hook_answer("SessionStart", "sweep failed: " + type(why).__name__ + ": " + str(why)) + "\n"
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
