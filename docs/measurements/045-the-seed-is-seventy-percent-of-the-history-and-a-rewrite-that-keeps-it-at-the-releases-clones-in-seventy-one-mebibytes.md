# 045: the seed is seventy percent of the history, and a rewrite that keeps it at the releases clones in seventy-one mebibytes

Measured 2026-10-14 by the session that re-ordered the chain, for the
author's question, meant as: *I would like to keep the repository very lean;
are we carrying five hundred megabytes for a youthful mistake, and can the
history be rewritten without losing anything?* Every number below is a
command's, run that day; what was not run is said in § 6.

## 1. What was asked, and of what

Two questions. How much of the repository is the seed, `seed/heroes.c`, the
compiler's own emitted C that a clean checkout builds with one clang line;
and whether `git filter-repo` can keep that file only at chosen commits while
every other byte of every commit, every tag and every citation survives.

## 2. The repository as it stood, at `3f77bd3a3`

| fact | number | command |
|---|---|---|
| the local `.git` | 1.0 GB, a pack of 686 MiB, 677 of it blobs and 7 commits and trees | `du -sh .git`, `git count-objects -vH`, `git verify-pack -v` |
| a fresh clone | 580 MiB | `git clone --no-local`, then `git count-objects -vH` |
| a clone of the tree alone | about 36 MiB | `git archive HEAD \| gzip -1 \| wc -c` |
| versions of the seed in the history | 1,222, 8.9 GB uncompressed | `git rev-list --objects --all -- seed/heroes.c`, `git cat-file --batch-check` |
| the seed's share of the pack | 478 of the 677 MiB of blobs, 70% | `git verify-pack -v` joined on the seed's blobs |
| commits that touch it | 290 of 3,582 | `git log --oneline -- seed/heroes.c` |
| its size at the tags | 21,008,434 bytes at `m-selfhost-fixpoint`, 30,675,263 at `m-agreed-retention`, 52,119,283 at `v0.3.0`, 58,579,877 at `HEAD` | `git cat-file -s <tag>:seed/heroes.c` |
| its `#line` directives | 24,462,654 of its 58,579,877 bytes (41.7%); 34,117,223 bytes without them | `grep -E '^#line ' seed/heroes.c \| wc -c` |
| the `#line` share of a release's diff | 52,844 of the 84,564 lines that changed between `v0.3.0` and `v0.4.0` (62%) | `git diff v0.3.0 v0.4.0 -- seed/heroes.c`, counted |
| the seed's rhythm | 33, 35, 30, 89, 27 and 35 commits in each of the six weeks to 2026-10-14 | `git log --since --until -- seed/heroes.c` |
| who would notice a rewrite | 1 star, 0 forks, 0 watchers; 1,271 clones from 177 sources and 15 views from 11 visitors in the 14 days before (the clones are almost all the CI's runners, which no command can tell apart); no Software Heritage origin | `gh repo view`, the traffic API, `archive.softwareheritage.org/api/1/origin/` |

Round 17 (`676c3941c`, the same evening) changed the seed after the
rehearsal: 51,688,146 bytes and 1,400,548 lines, of which 627,915 lines and
24,921,380 bytes are `#line` directives, 48%. The rehearsal is owed again on
the tree that will be rewritten (§ 6).

## 3. The rehearsal

On fresh clones (`git clone --no-local`; `filter-repo` refuses a clone that
is not fresh, and a `git fetch` after the clone is enough to make it refuse),
in the session's scratch folder under `.claude/worktrees/` (git-ignored,
2026-10-14), one `git filter-repo --force --prune-empty never` per variant,
with a commit callback that removes every change to `seed/heroes.c` and, at
each commit to keep, writes back that commit's ORIGINAL seed blob:

```python
commit.file_changes = [fc for fc in commit.file_changes if fc.filename != b"seed/heroes.c"]
if commit.original_id in keep:
    commit.file_changes.append(FileChange(b"M", b"seed/heroes.c", keep[commit.original_id], b"100644"))
```

`keep` maps each kept commit to the blob `<tag>:seed/heroes.c` names. Then
`git gc --prune=now --aggressive`.

| variant | seeds kept | pack | commits | tags |
|---|---|---|---|---|
| none (the clone as it was) | 1,222 | 580 MiB | 3,582 | 69 |
| A: at every milestone tag and at `HEAD` (45 commits) | 39 distinct | 123 MiB | 3,582 | 69 |
| B: at the four `v*` tags and at `HEAD` (5 commits) | 5 | 71 MiB | 3,582 | 69 |
| C: as A, rewriting only the commits after the seed's arrival, through `--refs <base>..main` and one range per tag | failed: the tags were rewritten and `main` was not | | | |

## 4. What was verified, and how

On A and B, each against the original repository:

- the seed at every kept commit is the original blob, byte for byte: 45 of
  45 in A, 5 of 5 in B; at `HEAD` identical in both;
- 3,582 commits and 69 tags in both, and none pruned (`commit-map` lines
  mapped to zero: 0);
- every commit a living file or a record cites, read as the 7- to 40-digit
  hexadecimal tokens of `docs/`, `CLAUDE.md`, `README.md`, `.claude/rules/`,
  `.claude/skills/`, `tests/harness/`, `selfhost/` and `site/src/` that name a
  commit of the original (525), is a prefix of an old id in the `commit-map`,
  so it can be remapped, but one: `2067ec875`, a commit of a lane never
  merged, which GitHub does not hold (its commits API answered 422) and which
  `selfhost/emit/header_region.hero` cites;
- the compiler built from the rewritten copy's seed and `heroes --version`
  read `heroes 0.4.0`.

**Remap and re-run, on A.** The records suite on the rewritten copy read 25
passed and 2 failed, `records/citations` and `records/redirects`: the
instruments pin commits by hash in their code, `ISSUES_ARCHIVED_AT` among
them. Remapping through the `commit-map` the 56 hashes that `tests/harness/`,
`.claude/rules/` and `selfhost/cli/` pin, in 27 files of the copy, the suite
read 26 passed and 0 failed. The suite is the instrument that says nothing
was missed, and it found exactly what was.

## 5. What the rehearsal taught the run that counts

- **`--prune-empty never`.** Without it, fifteen commits that touched the
  seed alone (*the seed regenerated...*) become empty and are pruned, and six
  of them are cited by records.
- **The first attempt kept 13 seeds of 39**, because it kept the seed's
  change only at the tagged commit, and the seed is usually regenerated one
  commit before the one a tag names. Writing the tag's own blob at the tagged
  commit is what keeps them all.
- **Five commits of 2026-08-10 carry SSH signatures** (`%G?` reads `N` only
  because no allowed-signers file is configured). `filter-repo` drops them,
  and because a changed commit changes every descendant, in A and B the 448
  commits between those five and the seed's arrival change hash as well;
  227 commits stay identical. All of them are in the `commit-map`, so no
  citation is lost; the five signatures are.
- **Variant C does not work as written**: ranges given to `--refs` left
  `main` unrewritten, so the old history stayed referenced and the
  aggressive collection repacked all of it for over an hour before it was
  stopped.

## 6. What this does not settle, and is owed before the run that counts

- whether GitHub accepts and serves refs under `refs/archive/*`, so that an
  old `Record:` permalink of an issue still opens, and what a release does
  when its tag is re-pointed: to be proved on a throwaway repository first;
- whether `git bisect` across the rewritten history stays usable, on a
  sample of intermediate commits built in two stages;
- whether `git fast-export --signed-commits` (git 2.56 lists it) can carry
  the five signatures through `filter-repo`, which exposes no option for it;
- the rehearsal itself on the tree that will be rewritten, since round 17
  moved the seed after it, with the full net on the rewritten copy.

## 7. The run on the tree that will be rewritten, 2026-10-14 evening

Added the same evening by the same session, at `13dd2ebdd` (round 17 and the
chain's three commits in): § 6's questions run, and two numbers of § 2 found
wrong. Nothing above is rewritten; this section is what the record now says.

**Two numbers of § 2 were wrong, and the title carries one of them.**

- *1,222 versions of the seed* counted every line that
  `git rev-list --objects --all -- seed/heroes.c` printed. Run again at
  `3f77bd3a3` with the tags, the command prints 1,217 lines: 290 commits, 57
  tags, 580 trees and 290 blobs of 8.87 GB, which is the 8.9 GB of § 2. The
  versions are the blobs, and counted without the path limit
  (`git rev-list --objects 3f77bd3a3 --tags`, the lines whose path is
  `seed/heroes.c`) they were **294**, 8.99 GB. At `13dd2ebdd`: **301**,
  9.36 GB.
- *70% of the pack* read the local repository's pack, which holds objects a
  clone never receives. In the pack a clone receives the seed is **87%**:
  536.5 of 614.5 MiB in a clone from GitHub (`git clone --bare`, 616.61 MiB
  and 78,035 objects, 2026-10-14 at 22:22), and 502.5 of 578.5 MiB in a
  `--no-local` clone of 580.59 MiB, each read by `git verify-pack -v` joined
  on the 301 seed blobs. The title's *seventy percent* is this error; the
  record keeps its name.

And a clone from GitHub, which is what a stranger receives, was 616.61 MiB,
not the 580 of a local clone.

**The rehearsal again, at `13dd2ebdd`, one procedure for every variant**: a
`--no-local` clone, `git filter-repo --force --prune-empty never
--preserve-commit-hashes` with § 3's callback, every changed commit compared
with the original's, then `git reflog expire --expire=now --all` and
`git gc --prune=now --aggressive`.

| variant | seeds kept | pack |
|---|---|---|
| B: the four releases and `HEAD` | 5 | 72.95 MiB |
| D: B, and every commit whose inherited seed asserts another runtime ABI than its own runtime header | 24 | 119.25 MiB |
| A: every tagged commit that has a seed, and `HEAD` | 39 | 124.88 MiB |
| E: A and D together | 54 | 156.89 MiB |

Each variant: 3,619 commits and 69 tags; 3,392 commits changed, none with
another author, committer, date or message and none with a tree that differs
outside `seed/heroes.c`; the five signatures dropped. B was run twice and
packed to the same 72.95 MiB and 77,435 objects. **`--preserve-commit-hashes`
is owed**: without it `filter-repo` rewrites the abbreviated hashes inside
1,173 commit messages, and a message is a record.

**§ 6, whether `git bisect` stays usable from a default clone: under no
variant, and the archive is where it does.** Stage 1 of a two-stage build,
clang on the seed a commit carries, stops at the seed's
`_Static_assert(HERO_RUNTIME_ABI == N)` whenever the runtime header of the
same tree says another N, and the runtime headers of the published history
carry seventeen ABI numbers, 14 to 30. Over the 3,619 commits of `main` and
the tags, with the seed each would carry simulated by the callback's own rule
(a commit not kept carries its first parent's seed; the simulation of B
agrees with the rewritten copy at all 3,619):

| history | stage 1 refused | no seed | tagged commits that carry a seed |
|---|---|---|---|
| as it was | 141 | 674 | 44, all their own |
| B | 2,279 | 1,065 | 29, 5 their own, 20 refused at stage 1 |
| D | 141 | 674 | 44, 10 their own |
| A | 1,229 | 684 | 44, all their own |
| E | 141 | 674 | 44, all their own |

The 141 of the history as it was are its own: commits whose own seed asserts
another ABI than their own runtime header. Six milestone tags built in full
on the rewritten B: `m-typed-inspection` and `m-open-repository` build in two
stages; `m-inferred-cell`, `m-agreed-retention` and `m-anchored-spec` stop at
stage 1; `m-separate-compilation` carries no seed.

Stage 2, an older compiler reading newer sources, was built in full on D. Of
the 44 tagged commits that carry a seed, the 10 that carry their own: 9
build. Of the 34 that carry an older one: 26 build and 6 stop at stage 2,
each on a form its inherited compiler did not yet read: `m-inferred-cell` on
an `=` (`expected_expression`), `m-declared-extents` on a parameter list
(`expected_params_close`), `m-selfhost-nesting`, `m-corpus-coverage` and
`m-documentation-site` on `use cli` (`module_path_has_no_parts`), and
`m-separate-compilation` on a name (`unknown_name`). Of 10 commits drawn at
random, 8 build and 2 stop at stage 2 (`declaration_in_arm`). **Each of the
eight, built as it was with its own seed on a clone that was not rewritten,
builds**: the runtime's ABI is one interface a carried seed must still read,
and the language is the other. Three tags of 2026-08-18 and 2026-08-19,
`m-selfhost-fixpoint` (its own seed), `m-harness-port` and
`m-bootstrap-archive`, were still in the emitter after 32 minutes at full
speed, where the compiler of those days pushes every line onto an array, the
quadratic push measurement 011 found five days later; they were stopped
unjudged. The commits as they were, under `refs/archive/`, carry every seed
they carried, and there every commit builds as it did.

**And the recovery chain.** `seed/README.md` § If the seed is already broken
stands on *every milestone tag from `m-selfhost-fixpoint` on carries its own
`seed/heroes.c`*. Under B that holds at five tags of 69: the four releases,
and `m-open-repository`, which no regeneration separates from `v0.2.0`. Under
A and E it holds at every one.

**The history not rewritten, repacked the same way**, a `--no-local` clone
given the same `gc --aggressive` so that what the rewrite saves could be told
from what repacking alone saves, had not finished after 98 minutes and was
stopped, as § 5's variant C was. That weight is not known.

**§ 6, whether GitHub serves `refs/archive/*`: yes, a commit and a blob.**
`refs/archive/probe-2026-10-14`, pushed to this repository the same day: the
commit's page answered 200, a blob permalink 200, the commits API found it;
the ref was then deleted. A release on a re-pointed tag was not tried.
**Every permalink the issues hold depends on five commits**: the bodies of
the 2,096 issues hold 4,699 links to a commit of this repository, and they
name 5 distinct commits (2,020 `Record:` lines name `d67f95ae`), all under
the old `main`, so `refs/archive/main` alone keeps every one of them open.

**§ 6, the signatures**: not tried. Every variant above drops the five.

**The remap, and the defect the full net found in it.** The hashes that
living files cite were remapped through the `commit-map` (104 hashes in 78
files; the records' own left as they are, which `refs/archive/` keeps
openable). The full net on the rewritten B read 8,622 passed and 8 failed,
the 8 in `surface`: the remap had rewritten a hash inside eight rows'
expected output, and each of those rows expects back the bytes of a fixture
under `tests/golden/`, a record the remap rightly leaves alone. **A living
file's hash is a citation and follows the history; a row that copies a
record's bytes copies them as the record holds them.** The rule that says so
keeps a hash on a row whose `argv` reads a `tests/golden/` fixture holding
that same hash: it keeps exactly those eight, and the nine places that hold
a fixture's path in a variable share no hash with it. On the rewritten A,
remapped by that rule, the full net read 8,630 passed and 0 failed, the
compiler's own tests 1,677 and the net's own 405, all passed, and the
fixpoint by `cmp` was identical. On the rewritten B, remapped again by it,
the suites the correction and the announcement touch read `surface` 424 and
0, `records` 26 and 0, `unseen` 3 and 0, `wholes` 620 and 0, the net's own
tests 405, all passed, and the fixpoint was identical.

**Spot checks on the rewritten B**, drawn with the fixed seed 20261014: eight
commits beside their originals (same author, committer, date and message,
parents mapped, the tree differing in `seed/heroes.c` alone or not at all);
five tags (tagger and message as they were, each on the mapped commit); eight
remapped hashes (the map agrees, and the commit has the same date and
subject); eight closed issues' `Record:` permalinks (200 today, under the old
`main`, the same bytes at the mapped commit); and the four releases' seeds,
each the original blob, each building a compiler that says its version,
`heroes 0.1.0` to `heroes 0.4.0`.

**What this leaves to the author**: which seeds the rewrite keeps, B, D, A
or E, with the numbers above; then the force-push, the reset of the shared
checkout, and the CI's release job around the re-pushed tags, each asked for.
