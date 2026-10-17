# Panel 222: a compiler says the digest of the sources it was built from, and a gate compares it with the tree's

**Convened 2026-10-17 from 12:40, synthesised the same day**, on task 2087
(https://github.com/heroes-lang/heroes/issues/2087), M-bounded-seed's part 2, the rhythm, whose
own text says it designs its answer first. **Seats**: compiler-engineer, historian, then a
completeness critic. **Resolution: provisional, author ratification pending.** Brief:
`docs/panel/222-briefs/00-shared.md`. Reports: `docs/panel/222-reports/compiler-engineer.md`,
`historian.md`, `critic.md`. Every number below is a seat's, with its command in its report.

## The proposal, verbatim (the shared brief's question)

> What tells a gate, a hook and the CI that a compiler was built from sources older than the
> tree's, when its file is newer than every source? And what moves with it.

Routes (a) to (d) were the coordinator's; the net checking as well as the hook the
compiler-engineer's; a stamp file beside the binary the historian's.

## What was measured

- **The hole, run** (compiler-engineer): with `selfhost/main.hero` edited after the seed, the hook's
  own verdict passed clang's compiler on the unchanged seed, passed a copy of a compiler built
  before the edit, and refused a compiler whose sources were byte-identical to the tree's after
  the file was rewritten with the same bytes: a modification time asks about the file, never about
  what the binary was built from. The CI runs the net with the stage-1 binary today
  (`ci.yml:756`).
- **(a), built** (compiler-engineer): every program carries a digest of the sources it was built
  from and `--version` prints it; `selfhost/emit/provenance.hero` 31 code lines, about 50 in all,
  no front-end line, no surface word, one C line per program. It refused the three stale cases
  above and passed the stage 2 built after the edit; the compiler's 1,690 tests passed.
- **(a), attacked** (critic): one digest, `6710054c3edc42a5`, on this Mac, Linux arm64, emulated
  Linux x86-64 and the Windows box under clang 21, 22 and 23; CRLF, a missing final newline,
  non-ASCII bytes and every spelling of the root path digest alike; Windows checks out LF under
  `core.autocrlf=true` because of `.gitattributes`. Four claims fell: the Python mirror of the
  digest misreads a legal `use sub/helper  # comment` and refuses a correct compiler, loudly; a
  compiler copied from before a `runtime/` change passes, the digest covering `.hero` files alone
  (31 of the last 30 days' 76 runtime commits touched none); *two commits with a seed refresh
  between* is not required, one commit with a seed a writer-only compiler emitted builds from a
  clean clone, and what binds is a rule: no commit may hold the reader while `seed/heroes.c` lacks
  the writer; and every comment edit now moves the emitted C by its digest line, which empties the
  author's probe exemption of 2026-10-12 (*byte-identical*) and turns `emission` red after a
  comment edit in a golden case until it is blessed again. Also found: stage 1 can tell its own
  lag with no mirror, its `--version` giving the seed's digest and its own `--emit-c` of the tree
  the tree's; `ci.yml:553`'s release test reads the first line of `--version` alone.
- **Precedent** (historian, verified pages): Go decides staleness by content since 1.10 (*purely
  based on the content of source files ... Modification times are no longer consulted*), every
  binary storing a hash of its inputs; V writes its bootstrap's commit into its `v.c` and prints
  both hashes when they differ; GHC binaries carry their stage; no project keeps a full stage-1
  compiler from judging by a written rule alone, and OCaml removed a release number from its
  bootstrap because it *adds a requirement for two bootstraps at each tag* (`a92116dc`,
  2026-03-23).

## Verdicts

| seat | verdict | condition |
|---|---|---|
| compiler-engineer | **approve (a)**, and the net checks as well as the hook; object (b) beyond (a), (c) alone, (d) | the reader in a module `main.hero` already uses (it is at 300); the emissions blessed once with s4's; `ci.yml:553` reads the first line; a whole-build timing on a still machine; to object if the cost passes 1% or two CI legs print two digests for one commit |
| historian (advisory) | **approve (a)** with the digest of file content, never a commit hash, covering exactly what the check can hash; **approve (b)'s check, object to its rule alone**; approve (c) as discipline, object as the detector; object (d); object a stamp file | the digest printed and compared, never asserted inside the seed or the build |
| completeness critic | (a) holds where it matters; four claims falsified above | a check that no commit holds the reader with a seed lacking the writer; decide the coverage before landing; the compiler's own `--emit-c` the arbiter, the Python mirror a fast path held to it by a test; the CI's stage 2 judging every step; the probe exemption restated; every refusal naming both digests |

## Resolution (provisional: author ratification pending)

**R1, (a)**: every program carries a digest of its sources' content, `--version` prints it on a
second line, and the gate compares it with the tree's. **The digest covers the `.hero` sources a
program's root reaches, the runtime it is built with (`runtime/`, the files clang reads), and the
library modules compiled into it**, the critic's robust choice: a compiler copied from before a
runtime change is then refused, as Go's digest of every input refuses it.

**R2, the arbiter is the compiler, the mirror a fast path**: what the tree's digest is, is the
judged compiler's own answer; a Python reading of it in the hook is held to the compiler's by a
test over every `use` spelling the formatter accepts, and where the two disagree the refusal says
the mirror may be the one that is wrong. Every refusal names both digests and the two-stage build.

**R3, the rule that replaces "two commits"**: no commit holds the reader of the digest while
`seed/heroes.c` lacks its writer, checked by the net, and a lane's merge keeps the seed the batch
regenerates, never a lane's own older one.

**R4, what moves with it**: the CI's legs build stage 2 and judge every step with it, `:553` reads
the first line, the seed's check `cmp`s where the digests agree and otherwise checks that stage 2
reproduces itself one stage later (both branches run by the critic, 40.5 s and 89 s); the probe
exemption of 2026-10-12 reads *identical except the digest line*; `seed/README.md`, `README.md` and
the defect form say what the second line is; the policy of 2026-10-14, the seed refreshed when the
two-stage build goes red and at each release, in `.claude/rules/verification.md` § The batch.

**R5, refused**: (b)'s rule alone, (c) as the detector, (d), and a stamp file beside the binary
(the one-line clang bootstrap writes none).

### The question to the author

**Should the digest also cover the runtime and the library modules, not only the `.hero` sources?**
**Recommended: yes**, because a compiler copied from before a runtime change otherwise passes, and
31 of the last 30 days' 76 runtime commits touched no `.hero` file; the cost is that a runtime edit
moves every program's digest line, as a comment edit does. The other answer covers the `.hero`
sources alone and writes the runtime down as uncovered.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | (a)'s commits touch no front-end file and every blessed emission moves by exactly one line | `git diff --stat`; the emission's moved lines | its landing |

## Process notes

- **The historian sat**, and R1 rests on a precedent the historian sourced and marked verified (Go's
  content-based staleness, go.dev/doc/go1.10), so the ratification carries a `## Prior art`
  section, its line re-read before the issue is filed.

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2124, naming the sitting as
`panel 222`, with its one question and the recommendation.

**RATIFIED, 2026-10-17**, R1 to R5 as written above, the author answering in conversation that
afternoon on the coordinator's question, which put the coverage with its measurement and the
other answer, meant as *yes, the runtime and the libraries too*, and, asked in the same breath
whether to build it then or in a new session, *yes, now*. So **the question to the author is
answered: the digest covers the `.hero` sources a program's root reaches, its runtime and its
library modules**; the other answer is not taken. Recorded as a reading (CLAUDE.md § 4). The
author may overturn it.
