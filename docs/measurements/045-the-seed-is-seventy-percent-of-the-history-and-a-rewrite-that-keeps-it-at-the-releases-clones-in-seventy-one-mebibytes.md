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
