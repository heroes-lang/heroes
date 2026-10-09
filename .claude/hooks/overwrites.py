#!/usr/bin/env python3
"""The git commands that write over what the index holds for the paths they name.

Why this exists. Defect 514's repair reads the discards of the whole tree, an
abort, a skip, a hard or merge reset, a forced checkout or switch
(`discards.py`), and none that names its paths. Defect 529, filed from lane
b16-tools's probe of 2026-10-09; measured again that day in scratch
repositories, git 2.56.0 (lane b16-misc), over a tree holding a staged new
file, a staged change, an unstaged one, a staged change changed again, a
staged new file changed again, a staged deletion and a staged change in a
directory, each command run once in a fresh copy:

- `git restore --staged --worktree`, in every spelling git reads (`-S -W`,
  `-SW`, `-WS`, `--st --w`, with `--source` or not), and `git checkout
  <commit> [--] <paths>` write the source over the index and the working
  tree of every path they name: a staged change is then nowhere, a new file
  deleted from the disk (by `restore`, and by a checkout `--no-overlay`; an
  overlay checkout leaves a file its commit lacks), a staged deletion undone;
- `git rm -f` removes the path from both, and `git mv -f` writes its source
  over a destination holding a staged change;
- `git restore --staged`, `git reset [<commit>] [--] <paths>`, a `git reset`
  of the whole index by `--mixed` or `--keep`, and `git rm --cached -f` write
  over the index alone: the staged version survives only where the working
  tree holds the same, so a path staged and then changed again loses it (a
  staged new file or change the working tree still holds stays on the disk,
  unstaged, and a staged deletion stays a deletion);
- `git restore [--worktree]`, with a `--source` or not, and `git checkout
  [-f] [--] <paths>` from the index write the working tree alone: every
  staged version stayed in the index, so they throw away unstaged work only,
  which the hard stops' *destructive operations are asked for* covers and no
  hook performs. `-m` and `--conflict` re-create the conflict of a path an
  operation brought, and no other.

So such a command is refused where a path it names holds a staged change it
would throw away, in a file no operation standing brought (`commits.brought`,
as `discards.thrown_away` reads it), and git itself is asked which paths that
is: `git diff --cached` under the command's own pathspec, read in the
directory it runs in with git's own options and environment, against its
source and, for the index alone, against the working tree. Words the text
cannot read, an expansion, a list from standard input or one the same
command line writes, or the words `xargs` adds to the command it runs, are
read as naming every path; a source it cannot read as differing from every
staged version. Where git refuses the words (an
option it does not know or cannot tell, measured from `git <verb> -h`) or
cannot say, this gives no opinion.

**And the two plumbing commands that do the same, since 2026-10-09** (defect
535, lane b16-misc's finding; measured again that day in scratch
repositories, git 2.56.0, over the same states and a staged new file deleted
from the disk). `git read-tree <tree-ish>...`, and with `--reset` or
`--empty`, writes the trees over the index alone, as a reset of the whole
index does; with `-m` and one tree it refuses a path the working tree holds
otherwise and still writes over one the disk no longer holds, so a staged
version deleted from the disk is then nowhere; with `-u` beside `-m` it writes
the working tree too, a staged new file deleted from it; and `--reset -u` puts
back every tracked file as a hard reset does (`discards.py` reads that one).
Two or three trees, `--prefix`, `--index-output` and `-n` lost nothing, and
`-u` alone git refuses. `git update-index` reads its options in order, each
path under the ones before it: `--force-remove` acts on the paths after it as
`git rm --cached -f`, `--remove` drops a path the disk no longer holds (a path
it holds it stages, as `git add` does, which is not this guard's), and
`--cacheinfo` and `--index-info` write an entry over the one staged; `--again`
lost nothing.
"""

import os
import re
import subprocess

import commits
import runs
import staged

CONTRACT = "CLAUDE.md § Hard stops"
WHOLE = ":/"


class Source:
    """A source no word of the text names as git reads it: what the refusal
    calls it, and every staged version read as unlike it."""

    def __init__(self, label):
        self.label = label


UNREAD = Source("a commit the text does not hold")
# What stands for the words `xargs` adds to the command it runs, which the
# text does not hold.
XARGS = "the words `xargs` adds"
# And the paths a `find` gives the command it runs (`runs.py`, defect 534),
# read as `XARGS` is: each a word the text does not hold.
FOUND = runs.FOUND
GIVEN = (XARGS, FOUND)
# What `git read-tree` writes from with `--empty`, and with several trees, and
# what `git update-index` writes from with `--cacheinfo` or `--index-info`.
EMPTY = Source("an empty index")
SEVERAL = Source("the trees it names")
ENTRY = Source("the entries it names")
# A brace expansion the shell makes before git reads the word: `{a,e}.txt`.
BRACES = re.compile(r"\{[^{}]*(,|\.\.)[^{}]*\}")

# Each verb's options as `git <verb> -h` lists them on git 2.56.0: the long
# name ("" for a letter alone), the letter ("" for a long name alone), how it
# takes a value ("" none, "=" one, "?" only joined to it), and whether
# `--no-` negates it (`--[no-]`).
RESTORE = (
    ("source", "s", "=", True), ("staged", "S", "", True), ("worktree", "W", "", True),
    ("ignore-unmerged", "", "", True), ("overlay", "", "", True), ("quiet", "q", "", True),
    ("recurse-submodules", "", "?", True), ("progress", "", "", True), ("merge", "m", "", True),
    ("conflict", "", "=", True), ("ours", "2", "", False), ("theirs", "3", "", False),
    ("patch", "p", "", True), ("unified", "U", "=", False), ("inter-hunk-context", "", "=", False),
    ("ignore-skip-worktree-bits", "", "", True), ("pathspec-from-file", "", "=", True),
    ("pathspec-file-nul", "", "", True),
)
CHECKOUT = (
    ("", "b", "=", False), ("", "B", "=", False), ("", "l", "", False), ("guess", "", "", True),
    ("overlay", "", "", True), ("auto-advance", "", "", True), ("quiet", "q", "", True),
    ("recurse-submodules", "", "?", True), ("progress", "", "", True), ("merge", "m", "", True),
    ("conflict", "", "=", True), ("detach", "d", "", True), ("track", "t", "?", True),
    ("force", "f", "", True), ("orphan", "", "=", True), ("overwrite-ignore", "", "", True),
    ("ignore-other-worktrees", "", "", True), ("ours", "2", "", False), ("theirs", "3", "", False),
    ("patch", "p", "", True), ("unified", "U", "=", False), ("inter-hunk-context", "", "=", False),
    ("ignore-skip-worktree-bits", "", "", True), ("pathspec-from-file", "", "=", True),
    ("pathspec-file-nul", "", "", True),
)
RM = (
    ("dry-run", "n", "", True), ("quiet", "q", "", True), ("cached", "", "", True),
    ("force", "f", "", True), ("", "r", "", False), ("ignore-unmatch", "", "", True),
    ("sparse", "", "", True), ("pathspec-from-file", "", "=", True), ("pathspec-file-nul", "", "", True),
)
RESET = (
    ("quiet", "q", "", True), ("no-refresh", "", "", False), ("refresh", "", "", False),
    ("mixed", "", "", False), ("soft", "", "", False), ("hard", "", "", False), ("merge", "", "", False),
    ("keep", "", "", False), ("recurse-submodules", "", "?", True), ("patch", "p", "", True),
    ("auto-advance", "", "", True), ("unified", "U", "=", False), ("inter-hunk-context", "", "=", False),
    ("intent-to-add", "N", "", True), ("pathspec-from-file", "", "=", True), ("pathspec-file-nul", "", "", True),
)
MV = (
    ("verbose", "v", "", True), ("dry-run", "n", "", True), ("force", "f", "", True),
    ("", "k", "", False), ("sparse", "", "", True),
)
READ_TREE = (
    ("index-output", "", "=", False), ("empty", "", "", True), ("verbose", "v", "", True), ("", "m", "", False),
    ("trivial", "", "", True), ("aggressive", "", "", True), ("reset", "", "", True), ("prefix", "", "=", False),
    ("", "u", "", False), ("exclude-per-directory", "", "=", False), ("", "i", "", False), ("dry-run", "n", "", True),
    ("no-sparse-checkout", "", "", False), ("sparse-checkout", "", "", False), ("debug-unpack", "", "", True),
    ("recurse-submodules", "", "?", True), ("quiet", "q", "", True),
)
UPDATE_INDEX = (
    ("", "q", "", False), ("ignore-submodules", "", "", True), ("add", "", "", True), ("replace", "", "", True),
    ("remove", "", "", True), ("unmerged", "", "", True), ("refresh", "", "", False), ("really-refresh", "", "", False),
    ("cacheinfo", "", "=", False), ("chmod", "", "=", False), ("assume-unchanged", "", "", False),
    ("no-assume-unchanged", "", "", False), ("skip-worktree", "", "", False), ("no-skip-worktree", "", "", False),
    ("ignore-skip-worktree-entries", "", "", True), ("info-only", "", "", True), ("force-remove", "", "", True),
    ("", "z", "", False), ("stdin", "", "", False), ("index-info", "", "", False), ("unresolve", "", "", False),
    ("again", "g", "", False), ("ignore-missing", "", "", True), ("verbose", "", "", True),
    ("clear-resolve-undo", "", "", False), ("index-version", "", "=", True), ("show-index-version", "", "", True),
    ("split-index", "", "", True), ("untracked-cache", "", "", True), ("test-untracked-cache", "", "", True),
    ("force-untracked-cache", "", "", True), ("force-write-index", "", "", True), ("fsmonitor", "", "", True),
    ("fsmonitor-valid", "", "", False), ("no-fsmonitor-valid", "", "", False),
)
# The plumbing read since defect 535, as a refusal names it.
PLUMBING = ("git read-tree", "git update-index")
TABLES = {"restore": RESTORE, "checkout": CHECKOUT, "rm": RM, "reset": RESET, "mv": MV, "read-tree": READ_TREE,
          "update-index": UPDATE_INDEX}
MODES = ("mixed", "soft", "hard", "merge", "keep")


def long_row(rows, arg):
    """The row `--<arg>` names and whether it negates it, as git's
    parse-options reads it: an exact name first, `no-` and the name, or the
    one name it abbreviates (`--sta` is `--staged`, `--no-overl` is
    `--no-overlay`); None where git refuses it, unknown or ambiguous (`--s`
    for a restore, measured 2026-10-09)."""
    name = arg.split("=", 1)[0]
    found = []
    for row in rows:
        long_name, _letter, _value, negatable = row
        if not long_name:
            continue
        unset = False
        if not name.startswith("no-") and negatable and long_name.startswith("no-"):
            long_name, unset = long_name[3:], True
        if name == long_name:
            return row, unset
        if long_name.startswith(name):
            found.append((row, unset))
            continue
        if not negatable:
            continue
        if "no-".startswith(name):
            found.append((row, not unset))
            continue
        if not name.startswith("no-"):
            continue
        if name[3:] == long_name:
            return row, not unset
        if long_name.startswith(name[3:]):
            found.append((row, not unset))
    return found[0] if len(found) == 1 else None


def key_of(row):
    return row[0] or row[1]


def options(rows, words):
    """`words`, the arguments after `git <verb>`, read as git reads them:
    (the options set, each by its long name or its letter, to True, False or
    its value; the other words before a `--`; the words after it, or None
    where there is none). None where git refuses the words."""
    letters = {row[1]: row for row in rows if row[1]}
    got, plain = {}, []
    i = 0
    while i < len(words):
        word = words[i]
        if word == "--":
            return got, plain, words[i + 1:]
        if word.startswith("--"):
            found = long_row(rows, word[2:])
            if found is None:
                return None
            row, unset = found
            joined = word.split("=", 1)[1] if "=" in word else None
            if unset or row[2] == "":
                if joined is not None:
                    return None
                got[key_of(row)] = not unset
            elif row[2] == "?":
                got[key_of(row)] = joined if joined is not None else True
            elif joined is not None:
                got[key_of(row)] = joined
            elif i + 1 < len(words):
                i += 1
                got[key_of(row)] = words[i]
            else:
                return None
        elif word.startswith("-") and word != "-":
            at = 1
            while at < len(word):
                row = letters.get(word[at])
                if row is None:
                    return None
                rest = word[at + 1:]
                if row[2] == "=":
                    if rest:
                        got[key_of(row)] = rest
                    elif i + 1 < len(words):
                        i += 1
                        got[key_of(row)] = words[i]
                    else:
                        return None
                    break
                if row[2] == "?":
                    got[key_of(row)] = rest or True
                    break
                got[key_of(row)] = True
                at += 1
        else:
            plain.append(word)
        i += 1
    return got, plain, None


def unreadable(word):
    """Whether the shell makes `word` into something else before git reads
    it: an expansion, a brace expansion, a home directory; or `xargs` gives
    it."""
    return commits.expansion(word) or bool(BRACES.search(word)) or word.startswith("~") or any(given in word for given in GIVEN)


def readable(words):
    """(the words of `words` git reads as written, the ones it does not),
    the words a `$( ... )` or a backquoted command splits into read with
    it (`commits.literal`)."""
    kept = commits.literal(words)
    out = [w for w in kept if not unreadable(w)]
    unread = [w for w in words if w not in out]
    return out, unread


def listed(where, name, nul, line):
    """The pathspecs the file `name` holds, read in `where`, or None where
    this cannot read them: standard input, an expansion, a file it cannot
    open, one the command line `line` names again (a list it writes before
    git reads it), or an element git would unquote."""
    if not isinstance(name, str) or name == "-" or unreadable(name):
        return None
    if line is not None and len(re.findall(r"(?<![\w./-])" + re.escape(name) + r"(?![\w./-])", line)) > 1:
        return None
    path = name if os.path.isabs(name) else os.path.join(where, name)
    try:
        with open(path, "rb") as handle:
            text = handle.read().decode("utf-8", errors="replace")
    except OSError:
        return None
    if nul:
        return [part for part in text.split("\0") if part]
    out = []
    for part in text.split("\n"):
        part = part[:-1] if part.endswith("\r") else part
        if part.startswith('"'):
            return None
        if part:
            out.append(part)
    return out


def run(where, opts, env, *args):
    """`git <opts> <args>` in `where` with `env`, its stdout, or None."""
    try:
        done = subprocess.run(["git"] + list(opts) + list(args), capture_output=True, timeout=30, cwd=where, text=True, env=env)
    except (OSError, subprocess.SubprocessError):
        return None
    return done.stdout if done.returncode == 0 else None


def names(where, opts, env, args, spec):
    """The paths, from the root, that `git diff --name-only <args>` lists
    under the pathspec `spec` (None for every path), or None."""
    tail = ["--"] + list(spec) if spec is not None else []
    out = run(where, opts, env, "diff", "--name-only", "-z", "--no-renames", "--no-relative", "--no-ext-diff", *args, *tail)
    return None if out is None else [rel for rel in out.split("\0") if rel]


def tree_ish(where, opts, env, word):
    """Whether git reads `word` as a commit or a tree here."""
    if word == "-":
        word = "@{-1}"
    out = run(where, opts, env, "rev-parse", "--verify", "-q", word + "^{tree}")
    return bool(out and out.strip())


def lost(where, opts, env, spec, source, index_alone, overlay=False, removing=False, missing=False):
    """The staged paths under `spec` whose staged version a command would
    leave nowhere: one writing `source` (None for HEAD, a `Source` for one no
    word names, UNREAD a commit the text cannot tell) over the index, and with
    `index_alone` False over the working tree too; `overlay` keeps a path its
    source lacks, and `removing` names a removal, which a staged deletion
    escapes; `missing`, that it writes over a path the working tree no longer
    holds and refuses or stages one it holds. None where git cannot say."""
    exempt = ["--diff-filter=d"] if (index_alone or removing) else (["--diff-filter=a"] if overlay and source is None else [])
    held = names(where, opts, env, ["--cached"] + exempt, spec)
    if not held:
        return held
    if source is not None and not isinstance(source, Source) and not removing:
        unlike = names(where, opts, env, ["--cached"] + (["--diff-filter=a"] if overlay else []) + [source], spec)
        if unlike is None:
            return None
        held = [rel for rel in held if rel in unlike]
    if index_alone and held:
        moved = names(where, opts, env, ["--diff-filter=D"] if missing else [], spec)
        if moved is None:
            return None
        held = [rel for rel in held if rel in moved]
    return held


def judged(verb, words, where, opts, env, line, appended=False):
    """What `git <verb> <words>` would write over, read in `where`: (how it
    is said, its source, whether it writes the index alone, overlay, removing,
    whether it reads the paths the disk no longer holds, its pathspec or None
    for every path, the words it cannot read), or
    None where it writes over no staged version or git refuses it.
    `appended`: `xargs` runs it, adding words the text does not hold."""
    read = options(TABLES[verb], list(words) + ([XARGS] if appended else []))
    if read is None:
        return None
    got, plain, after = read
    if got.get("patch") is True or got.get("dry-run") is True:
        return None
    named = plain + (after or []) if verb in ("restore", "rm") else None
    source = None
    if verb == "restore":
        if got.get("staged") is not True:
            return None
        source = got.get("source") if isinstance(got.get("source"), str) else None
        if source is not None and unreadable(source):
            source = UNREAD
        said = "git restore --staged" + (" --worktree" if got.get("worktree") is True else "")
        shape = (said, source, got.get("worktree") is not True, got.get("overlay") is True, False)
    elif verb == "rm":
        if got.get("force") is not True:
            return None
        cached = got.get("cached") is True
        shape = ("git rm --cached -f" if cached else "git rm -f", None, cached, False, True)
    elif verb == "mv":
        if got.get("force") is not True or len(plain + (after or [])) < 2:
            return None
        *sources, dest = plain + (after or [])
        into = os.path.isdir(os.path.join(where, dest)) or len(sources) > 1
        named = [":(literal)" + (os.path.join(dest, os.path.basename(s.rstrip("/"))) if into else dest) for s in sources]
        shape = ("git mv -f", None, False, False, True)
    else:
        head, named = tree_and_paths(verb, got, plain, after, where, opts, env)
        if head is False:
            return None
        source = UNREAD if isinstance(head, str) and unreadable(head) else head
        if verb == "checkout":
            if head is None:
                return None
            shape = ("git checkout " + ("<commit>" if head in GIVEN else head) + " --", source, False, got.get("overlay") is not False, False)
        else:
            mode = [m for m in MODES if got.get(m) is True]
            if mode and mode[-1] in ("soft", "hard", "merge"):
                return None
            shape = ("git reset" + (" --keep" if "keep" in mode else "") + (" " + head if head and head not in GIVEN else ""), source, True, False, False)
    from_file = got.get("pathspec-from-file")
    unread = []
    if isinstance(from_file, str):
        more = listed(where, from_file, got.get("pathspec-file-nul") is True, line)
        if more is None:
            return shape + (False, None, ["--pathspec-from-file=" + from_file])
        named = (named or []) + more
    elif verb in ("restore", "rm", "mv") and not named:
        return None
    if named is not None:
        named, unread = readable(named)
        if unread:
            named = None
    return shape + (False, named, unread)


def tree_and_paths(verb, got, plain, after, where, opts, env):
    """A checkout's or a reset's source and pathspec: (the word naming the
    commit or tree it writes from, None for the index for a checkout and HEAD
    for a reset; its paths, None for every path), or (False, None) where git
    refuses the words or writes no path from a commit (a branch switched to,
    a branch made). A first word the shell expands, before more words, is
    read as a commit, the reading that throws away more."""
    if verb == "checkout" and any(isinstance(got.get(k), str) for k in ("b", "B", "orphan")):
        return False, None
    listing = isinstance(got.get("pathspec-from-file"), str)
    if after is not None:
        if len(plain) > 1:
            return False, None
        head = plain[0] if plain else None
        paths = after
    elif plain and (len(plain) > 1 or listing) and (unreadable(plain[0]) or tree_ish(where, opts, env, plain[0])):
        head, paths = plain[0], plain[1:]
    elif plain and plain[0] in GIVEN:
        head, paths = plain[0], plain
    elif verb == "reset" and plain and tree_ish(where, opts, env, plain[0]):
        head, paths = plain[0], []
    else:
        head, paths = None, plain
    if verb == "checkout" and not paths and not listing:
        return False, None
    if verb == "reset" and not paths and not listing:
        paths = None
    return head, paths


def read_tree(words, where, opts, env, appended=False):
    """The shapes of `git read-tree <words>` that write over a staged version
    (defect 535), each as `judged` gives one; none where it writes over none,
    git refuses it, or it is a hard reset, `discards.py`'s (`read_tree_hard`)."""
    read = options(READ_TREE, list(words) + ([XARGS] if appended else []))
    if read is None:
        return []
    got, plain, after = read
    trees = plain + (after or [])
    merging, resetting, updating, empty = (got.get(k) is True for k in ("m", "reset", "u", "empty"))
    if got.get("dry-run") is True or isinstance(got.get("index-output"), str) or isinstance(got.get("prefix"), str):
        return []
    if (merging and resetting) or (updating and not (merging or resetting)) or (resetting and updating):
        return []
    # No tree at all empties the index as `--empty` does (git warns it is
    # deprecated, and does it); with `-m` or `--reset`, git refuses.
    if (empty and trees) or ((merging or resetting) and len(trees) != 1):
        return []
    if not trees:
        source = EMPTY
    elif any(unreadable(tree) for tree in trees):
        source = UNREAD
    else:
        source = trees[0] if len(trees) == 1 else SEVERAL
    said = "git read-tree" + (" -m" if merging else "") + (" --reset" if resetting else "") + (" -u" if updating else "")
    said += (" --empty" if empty else "") + "".join(" " + ("<commit>" if tree in GIVEN else tree) for tree in trees)
    if merging and updating:
        return [(said, source, False, False, False, False, None, [])]
    return [(said, source, True, False, False, merging, None, [])]


def read_tree_hard(words):
    """Whether `git read-tree <words>` puts back every tracked file as a hard
    reset does: `--reset -u` and one tree or `--empty`, measured 2026-10-09."""
    read = options(READ_TREE, list(words))
    if read is None:
        return False
    got, plain, after = read
    if got.get("dry-run") is True or isinstance(got.get("index-output"), str) or got.get("m") is True:
        return False
    return got.get("reset") is True and got.get("u") is True and len(plain + (after or [])) + (got.get("empty") is True) == 1


def index_updates(words, where, opts, env, appended=False):
    """The shapes of `git update-index <words>` that write over a staged
    version (defect 535), each as `judged` gives one, read in order as git
    reads them: `--force-remove` and `--remove` bind the paths after them,
    `--stdin` every path under the ones before it, `--cacheinfo` its own path
    and `--index-info` every path. Where git refuses the words, none."""
    words = list(words) + ([XARGS] if appended else [])
    letters = {row[1]: row for row in UPDATE_INDEX if row[1]}
    force = remove = dashed = False
    bound = {"force-remove": [], "remove": [], "cacheinfo": []}
    every = []
    i = 0
    while i < len(words):
        word = words[i]
        if not dashed and word == "--":
            dashed = True
        elif not dashed and word.startswith("--"):
            found = long_row(UPDATE_INDEX, word[2:])
            if found is None:
                return []
            row, unset = found
            name = key_of(row)
            joined = word.split("=", 1)[1] if "=" in word else None
            if name == "cacheinfo":
                value = joined if joined is not None else (words[i + 1] if i + 1 < len(words) else None)
                i += 0 if joined is not None else 1
                parts = (value or "").split(",", 2)
                if len(parts) == 3:
                    bound["cacheinfo"].append(parts[2])
                elif i + 2 < len(words):
                    bound["cacheinfo"].append(words[i + 2])
                    i += 2
                else:
                    return []
            elif name in ("force-remove", "remove"):
                force = not unset if name == "force-remove" else force
                remove = not unset if name == "remove" else remove
            elif name == "stdin" and (force or remove):
                every.append(("force-remove" if force else "remove", "--stdin"))
            elif name == "index-info":
                every.append(("cacheinfo", "--index-info"))
            elif row[2] == "=" and joined is None and not unset:
                i += 1
        elif not dashed and word.startswith("-") and word != "-":
            if any(letter not in letters for letter in word[1:]):
                return []
        elif force or remove:
            bound["force-remove" if force else "remove"].append(word)
        i += 1
    shapes = []
    for name, paths in bound.items():
        if paths:
            named, unread = readable(paths)
            spec = None if unread else [":(literal)" + path for path in named]
            shapes.append(index_shape(name, spec, unread))
    for name, how in every:
        shapes.append(index_shape(name, None, [how]))
    return shapes


def index_shape(name, spec, unread):
    """A shape of `git update-index`: what `--<name>` writes over the paths
    `spec` names, None for every path."""
    said = "git update-index --" + name
    if name == "cacheinfo":
        return (said, ENTRY, True, False, False, False, spec, unread)
    return (said, None, True, False, True, name == "remove", spec, unread)


def verdict(verb, words, where, opts=(), env=None, line=None, appended=False):
    """A refusal for `git <opts> <verb> <words>` run in `where`, or None.
    `line` is the whole command line, which a pathspec file it writes is read
    against; `appended`, that `xargs` runs it and adds words of its own."""
    if where is None or verb not in TABLES or not os.path.isdir(where):
        return None
    env = dict(os.environ) if env is None else env
    if verb == "read-tree":
        shapes = read_tree(words, where, opts, env, appended)
    elif verb == "update-index":
        shapes = index_updates(words, where, opts, env, appended)
    else:
        shape = judged(verb, words, where, opts, env, line, appended)
        shapes = [] if shape is None else [shape]
    for said, source, index_alone, overlay, removing, missing, spec, unread in shapes:
        gone = lost(where, opts, env, spec if spec is not None else [WHOLE], source, index_alone, overlay, removing, missing)
        if not gone:
            continue
        top = staged.toplevel(where)
        if top is None:
            return None
        now = commits.standing(top)
        own = set()
        for name, _verbs, what in now:
            got = commits.brought(top, name, what)
            if got is None:
                return None
            own |= got
        extra = [rel for rel in gone if rel not in own]
        if extra:
            return refusal(said, extra, source, index_alone, removing, overlay, [n for n, _v, _w in now], unread, missing)
    return None


def refusal(said, extra, source, index_alone, removing, overlay, standing, unread, missing=False):
    """The refusal's words: what `said` writes over, the paths it would
    leave without their staged version, and what keeps them."""
    whence = "HEAD" if source is None else (source.label if isinstance(source, Source) else "`" + source + "`")
    beside = (" that the " + " and the ".join(standing) + " standing did not bring") if standing else ""
    shown = ", ".join(extra[:12]) + ("" if len(extra) <= 12 else ", ...")
    count = str(len(extra)) + (" paths" if len(extra) > 1 else " path") + beside + (" hold" if len(extra) > 1 else " holds")
    if missing:
        what = ("writes over the index of every path it names that the working tree no longer holds, and " + count
                + " a staged version the working tree does not: " + shown + ". That version is then nowhere")
    elif removing and index_alone:
        what = ("removes every path it names from the index, and " + count
                + " a staged version the working tree does not: " + shown + ". That version is then nowhere")
    elif removing:
        does = "writes its source over every destination it names" if said.startswith("git mv") else "removes every path it names from the index and the disk"
        what = does + ", and " + count + " a staged change: " + shown + ". Its content is then nowhere"
    elif index_alone:
        what = ("writes " + whence + " over the index of every path it names, and " + count
                + " a staged version the working tree does not: " + shown + ". That version is then nowhere")
    else:
        what = ("writes " + whence + " over the index and the working tree of every path it names, and " + count
                + " a staged change: " + shown + ". " + ("A changed file is" if overlay else "A new file is deleted from the disk, a changed one")
                + " put back as " + whence + " has it, a staged deletion undone")
    blind = ""
    if unread:
        blind = (" This guard cannot read which paths " + ", ".join(word if word in GIVEN else "`" + word + "`" for word in unread)
                 + " name" + ("" if len(unread) > 1 or unread[0] in GIVEN else "s") + ", so it read the command as naming every path.")
    if source is UNREAD:
        blind += " This guard cannot read that commit, so it read it as unlike every staged version."
    if missing:
        advice = ("If they are this conversation's own, write the staged version back to the disk first (`git checkout -- "
                  "<paths>` writes the index's), and the command then leaves it there; another session's are left to it.")
    elif index_alone:
        advice = ("If they are this conversation's own, stage the version to keep first (`git add -- <paths>` stages the "
                  "working tree's), and the command then leaves it on the disk; another session's are left to it.")
    else:
        advice = ("If they are this conversation's own, unstage them first (`git restore --staged -- <paths>`, which keeps "
                  "each on the disk where the working tree holds what the index does), and what is then thrown away is a "
                  "destructive operation, asked for; another session's are left to it.")
    return (
        "refused: `" + said + "` " + what + ", whosever " + ("they are" if len(extra) > 1 else "it is")
        + " (measured, git 2.56.0)." + blind + " " + advice
        + " " + CONTRACT + " (CL-041, defect 529" + ("; defect 535" if said.startswith(PLUMBING) else "") + ")"
    )
