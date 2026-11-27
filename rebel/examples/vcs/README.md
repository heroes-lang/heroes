# vcs

vcs: a small version control system in git's model, written from git's
documented object model and behaviour.

Every file, folder and snapshot is an object named by the SHA-1 of its
kind, size and content, so the names are git's own (`git hash-object`
gives the same), stored compressed by zlib, first loose and then, after
`gc`, in a packfile whose objects are mostly deltas of each other, with
the pack's index beside it. A staging index in git's binary format says
what the next commit holds; branches, tags and `HEAD` are files under
`refs/`. `status` walks the work tree, leaving out what `.vcsignore`
files ignore; `diff` writes unified differences between the work tree,
the index and any commit; `log` draws the commit graph; `merge`
fast-forwards, or merges three ways and writes conflicts between markers
for the person to resolve; `fsck` recomputes every object's hash. The
repository is the folder `.vcs`, laid out as `.git` is; the demo asks git
only to name objects, never to read the repository.

Run for real, in the folder of a project:

    vcs init
    vcs add .
    vcs commit -m "First"
    vcs checkout -b topic
    vcs status
    vcs diff
    vcs log --graph --all
    vcs merge topic
    vcs tag -m "First release" v1.0
    vcs gc
    vcs fsck

with `VCS_AUTHOR_NAME`, `VCS_AUTHOR_EMAIL` and, for names that never
change, `VCS_AUTHOR_DATE` (`1795251600 +0100`) in the environment; and
`vcs demo build/vcs-demo` runs a whole session in a folder it makes and
removes, printing a transcript that is the same on every run
(`main.args`, `main.expected`). Its one line that may differ between
machines is the line beginning `git hash-object:`, where the demo asks
this machine's git to name one object of each kind: `main.expected`
holds it as a machine with git prints it, `agrees on 4 objects`, and a
machine without git prints `skipped: no git on this machine` there.

Rebel's packages and what for:

- `crypto`: SHA-1, the name of every object and the checksum that ends the
  index, each pack and each pack's index; `hash`: the CRC-32 a pack's
  index keeps of every packed object, and the FNV-1a fingerprints a delta
  finds its copies by; `compress`: zlib, around every loose object and
  every object of a pack.
- `bytes`: the binary formats, the index's and the pack's numbers and
  their varints, names as hexadecimal; `containers/ordered`: the index,
  an ordered map from path and stage to entry, kept in the file's order.
- `diff`: the unified differences, the counts a commit reports, and the
  edit scripts the three-way merge of a text walks.
- `graphs`: the commit graph, what a commit reaches, the order a log
  shows, and from those the merge base.
- `fs`: the repository's files, written whole and atomically, and the
  work tree walked with its ignore files; `path`: the ignore patterns
  themselves and every path built; `time`: a signature's moment and its
  zone, and the dates a log writes.
- `flags`: this command line; `os`: the working folder and the
  environment's identity; `process`: `git hash-object` run by the demo;
  `io`, `strings`, `lists`, `numbers`: what the rest stands on.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/vcs/main.hero -- demo build/vcs-demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 18 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress`](https://heroes-lang.org/rebel/compress/), [`containers/ordered`](https://heroes-lang.org/rebel/containers/ordered/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`diff`](https://heroes-lang.org/rebel/diff/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`lists`](https://heroes-lang.org/rebel/lists/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`process`](https://heroes-lang.org/rebel/process/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/).

## Its folders

12 folders, 43 files and 7,704 lines, this page aside; of them 40 modules, `.hero` files, of 7,457 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 330 | 1 | 83 |
| `cli/` | 2 | 346 | 2 | 346 |
| `commands/` | 3 | 519 | 3 | 519 |
| `demo/` | 3 | 525 | 3 | 525 |
| `history/` | 3 | 684 | 3 | 684 |
| `merging/` | 3 | 508 | 3 | 508 |
| `objects/` | 7 | 935 | 7 | 935 |
| `repo/` | 3 | 807 | 3 | 807 |
| `snapshot/` | 2 | 468 | 2 | 468 |
| `store/` | 6 | 1,372 | 6 | 1,372 |
| `upkeep/` | 3 | 347 | 3 | 347 |
| `work/` | 4 | 863 | 4 | 863 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
