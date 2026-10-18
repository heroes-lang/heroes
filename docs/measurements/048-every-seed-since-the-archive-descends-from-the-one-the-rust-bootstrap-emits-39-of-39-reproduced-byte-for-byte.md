# 048: every seed since the archive descends from the one the Rust bootstrap emits, 39 of 39 reproduced byte for byte

Measured 2026-10-18 by the session that opened M-witnessed-seed, for its third
item, in the words of its page (`docs/roadmap/milestones/M-witnessed-seed.md`):
*the chain walked from that root to today's seed through the sources of the
milestones, in two stages where a link does not build alone*. Every number is
a command's, run that day on this Mac (Apple clang 21, `rustc 1.90.0`), the
machine loaded beside the walk (a load between 10 and 21 on four performance
and four efficiency cores), so a duration below is what a step took that day,
never what a compiler costs.

## 1. The root

- **The Rust bootstrap emits the archive's seed.** At `m-bootstrap-archive`
  (the commit `027f69d72`; the tag is annotated, its object `21934957b`), the
  bootstrap rebuilt in measurement 047 emits `selfhost/` as the 22,025,792
  bytes of the tag's `seed/heroes.c`, byte for byte, in `real 0.79`, and the
  compiler clang makes of that seed emits the same bytes again.
- **The walk starts from the Rust bootstrap, not from the seed's compiler.**
  The first stage the walk needed was `e4a475cdd`. The seed's compiler, asked
  to build it, was still in its emitter after 7 h 26 min and was stopped,
  1,877 of 2,087 samples of its stack in `memmove`: the quadratic push
  measurement 011 found, which measurement 045 § 7 met at three tags of
  August. The Rust bootstrap emitted the same sources in `real 0.83` and built
  them into a compiler in `real 9.19`. The two implement one language at the
  tag, so which of them crosses the first link changes nothing a verdict below
  rests on, and the second implementation is the witness this milestone is
  named for (diverse double-compiling, Wheeler, ACSAC 2005).

## 2. The links

`git rev-list --topo-order --reverse m-bootstrap-archive..85fea83af --
seed/heroes.c`: 39 commits, holding 39 different seeds, 31 of them carrying a
milestone tag (`0dea184c3` carries four). Every one of the 44 `m-*` tags since
the archive holds one of these 39 seeds or the archive's, and today's tip,
`85fea83af`, holds link 39's: a seed witnessed below is the seed of every
milestone that carries it.

**A link.** Link *k* starts from the compiler clang makes of link *k − 1*'s
seed (link 1 from the Rust bootstrap), builds link *k*'s `selfhost/` with it,
and asks the compiler that makes to emit that `selfhost/` as C; the C must be
link *k*'s seed, byte for byte. A link that reproduces its seed makes the next
link's start exactly the compiler the chain itself would have built (one C,
one clang, one binary), so the links ran side by side, and the chain holds by
induction from the root.

**Each compiler builds against its own runtime's ABI.** A compiler emits C for
the runtime ABI of the sources it was built from, 14 at the archive and 30
today, moved by 16 commits between; so a build takes the target's own runtime
where its ABI is the compiler's, as the repository's two-stage build takes
`./runtime`, and else the newest runtime between the two that still has the
compiler's, named through `HEROES_RUNTIME`, which every compiler of the chain
reads, the archive's included. The runtime is C written by hand and compiled
by clang, so which version a stage linked changes nothing a seed's verdict
rests on. The build cache was emptied before every command.

## 3. Where a link did not build alone, and what crossed it

**28 of the 39 links** (3 to 5, 9 to 13, 15 to 19, 21 to 28 and 32 to 38)
built and reproduced their seed from the previous seed in one stage, the two
seeds asserting one runtime ABI. **Four** (6, 7, 8 and 14) cross one step of
the ABI between their two seeds, 6, 8 and 14 built against the newest runtime
of the previous ABI and 7 by the number put back (3.3 below). **Five** needed
stages (1, 2, 29, 30 and 31), link 39 the scaffold, and link 20's seed is an
earlier commit's. The ABI the seeds assert moves at links 1 (14 to 15), 2 (15
to 18), 6, 7, 8 and 14 (one step each), 30 (22 to 26) and 31 (26 to 30). What
crossed the others, each a procedure the repository wrote down before this
walk or a commit of its history as written:

1. **A stage.** The latest commit between the two links that the current
   compiler builds, found by halving, built twice (the second generation's code
   is its own emitter's), and the link tried again from it. Links 1, 2, 29, 30
   and 31, link 31 through eight, across two weeks of lanes merged side by side
   with no seed between them. The halving runs along the trunk's first
   parents, where each commit holds every one before it, and past every stage
   already taken: over the whole topological order, link 31's first walk took
   a stage of one lane, then of the other, and back.
2. **A lane's own commit.** Where the trunk's first commit past a stage is a
   merge whose lane teaches a form and uses it one commit later, the lane's
   own line is searched. Link 31's `18d8e18b8`, the merge of
   `lane-b12-str192`, brings `6934f73fc`, which teaches a string `\u{...}`, and
   `ad377db86`, which uses it eighteen minutes later
   (`selfhost/cli/compile.hero`, `"\u{1}I"`); `6934f73fc`, as written, was the
   stage.
3. **The ABI number put back**, one step at a time, the procedure `bfeab994f`
   wrote down (*put the number back, build the new source with the old
   compiler to get one that emits the new number, then raise the number and
   build again with that*), on a copy of the runtime outside every tree: link
   2's stages `74bc5f14d` (15 over 16) and `4810345f8` (16 over 17), and link
   7's own commit, `bed7ecebd` (19 over 20). Never across two steps, since a
   step that moved a layout cannot be crossed by a number.
4. **The definition before the use**, in passes, where one commit teaches the
   compiler something and its own sources use it at once, so no compiler built
   before it can read it:
   - link 1, `ff7f9d57a`: `validated` defined in the library every compiler
     carries and called in `selfhost/cli_toolchain.hero`. The commit with that
     one file as its parent had it, then the commit as written by the compiler
     that made, then by its own;
   - link 31, `fcec76d8d`, M-inferred-cell's step 2: the parser taught `@=` and
     the whole tree rewritten to it by the compiler's own certain fix, `v: T @
     e` become `v: T @= e` in 6,152 lines of `selfhost/`. The commit with that
     rewrite undone on the declaration lines alone (a line opening with a name,
     `:` and a type holding no `=`, `"` or `#`), in 563 files, never a string, a
     comment or the library's source held in strings; then the commit as
     written, then by its own compiler.

   The order is `seed/README.md`'s since 2026-10-17, *a new form enters
   `selfhost/` only after the refresh whose seed knows it*; both commits are
   older, and their originals carried their own seeds, which the rewrite of
   2026-10-16 left out.
5. **The scaffold of panel 222's R3**, for link 39: `d1f95674b` makes the
   emitter write `hero_program_sources` beside every program's `main` and the
   compiler read it for `--version` (`selfhost/cli/help.hero`, reached from
   `selfhost/main.hero`), and `seed/README.md` names the route: *a refresh in
   that state is emitted by a scaffold, a compiler of the tree with the reader
   taken out*. The commit with those two files as its parent `12db9b318` had
   them, then as written, then by its own compiler. The compiler it made says
   `sources 5f45d6514b22d650`, the key the CI's gcc leg printed for the seed of
   `85fea83af` that morning (task 2096).
6. **The builder linked with the next runtime**, for link 30: the runtime's
   cap on a command line, 256 arguments until `a3a04d4ee` raised it to 4,096
   (`runtime/parts/run.c`), when the compiler's 234 modules already passed it.
   Link 29's seed's compiler panics there, `a command line reached 256
   arguments`, exit 134, and so does every compiler built before
   `a3a04d4ee`, the commit that raises it included. The repository's two-stage
   build compiles the old seed with the tree's runtime; here, `a8234a98f`'s
   compiler emitted its own sources (26,457,556 bytes), that C was compiled with
   `a3a04d4ee`'s runtime whose ABI line was put back from 24 to 23, and the
   compiler that made emitted the same sources as the same bytes before it was
   trusted; it built `a3a04d4ee`, which built itself against its own runtime.

**And a seed whose sources are an earlier commit's.** Link 20's seed,
`m-stated-grammar`'s, is not what `e8b2a5ae9`'s own sources emit: those emit
24,091,256 bytes, a fixpoint over three generations, which is link 21's seed.
It was regenerated at `8776cbdcd` (*the seed is regenerated because the path
was cooked into it*), and measurement 045 § 5 says why it stands seven commits
of `selfhost/` later: the rewrite writes *the tag's own blob at the tagged
commit*, and *the seed is usually regenerated one commit before the one a tag
names*. It is what `476a2692e`'s sources emit, byte for byte (*the seed does
not move with it*), found by building the commits of `selfhost/` between the two
links newest first, the sixth tried, and what `8776cbdcd`'s emit, asked apart
once its message was read. The walk meant to ask the commits whose message
says the seed was regenerated first, and did not: its test joined a message's
characters rather than its words, so it never matched, and the order was the
history's alone.

## 4. The links one by one

Every row reproduced: the compiler its route made emits the link's sources
(the commit's own, or for link 20 the earlier one named) as the link's seed,
byte for byte, at the first generation. The route names the stages after the
compiler clang makes of the previous row's seed; the ABI is the one each seed
asserts; the SHA-256 is the seed's, its first sixteen hexadecimal digits.

| link | commit | date | tag | ABI | route | seed bytes | SHA-256 |
|---|---|---|---|---|---|---|---|
| 1 | `16a3dd50c` | 2026-08-26 | `m-separate-compilation` | 14 to 15 | the Rust bootstrap, `e4a475cdd`, `ff7f9d57a` two passes (§ 3.4) | 23,085,901 | `fe097404ec622d9b` |
| 2 | `056cf8218` | 2026-08-31 | `m-argv-execution` | 15 to 18 | `74bc5f14d` by the number put back (§ 3.3), `4810345f8` by the number put back (§ 3.3) | 23,236,554 | `edb01a9f81aac2e6` |
| 3 | `e5b5d84d7` | 2026-09-02 | `m-package-layout` | 18 | one stage | 23,391,790 | `2ba8177e9f4f9b94` |
| 4 | `0acbf1abf` | 2026-09-02 | `m-selfhost-nesting` | 18 | one stage | 23,432,901 | `03b8ebc2aa73da13` |
| 5 | `7ee29deb3` | 2026-09-02 | `m-corpus-coverage` | 18 | one stage | 23,432,919 | `4e28e51c6b6363fd` |
| 6 | `1c5dae2f5` | 2026-09-03 | `m-robustness-guards` | 18 to 19 | one stage, against the newest runtime of the ABI before | 24,536,719 | `3a20452ed411e223` |
| 7 | `bed7ecebd` | 2026-09-04 | `m-corpus-depth` | 19 to 20 | its own commit by the number put back (§ 3.3) | 21,828,701 | `9a36855e51f59b20` |
| 8 | `591dfab29` | 2026-09-05 | `m-c-callbacks` | 20 to 21 | one stage, against the newest runtime of the ABI before | 22,198,771 | `1fed9b6c4fb72630` |
| 9 | `5516cffa7` | 2026-09-06 | `m-isolated-threads` | 21 | one stage | 22,225,001 | `0902f6dcf0f54fa1` |
| 10 | `b36d96f40` | 2026-09-07 | `m-declared-freer` | 21 | one stage | 22,658,238 | `d67a8f7a703737e7` |
| 11 | `a4a3da585` | 2026-09-07 | none | 21 | one stage | 22,658,238 | `63a786ae823b93be` |
| 12 | `a6c0f3c40` | 2026-09-07 | none | 21 | one stage | 22,658,238 | `6fffee01549c4578` |
| 13 | `c8c4e6f34` | 2026-09-08 | `m-discard-refusal` | 21 | one stage | 22,808,182 | `e03e438ad9b0f19a` |
| 14 | `b03520a75` | 2026-09-09 | `m-held-bytes` | 21 to 22 | one stage, against the newest runtime of the ABI before | 23,317,368 | `f74da03b16ad8a0f` |
| 15 | `b7637412f` | 2026-09-09 | `m-interpolated-strings` | 22 | one stage | 23,455,906 | `7488145966043698` |
| 16 | `e2c0d82cb` | 2026-09-09 | `m-cstr-lifetime` | 22 | one stage | 23,455,906 | `ddb450413e5a89cb` |
| 17 | `0dea184c3` | 2026-09-11 | `m-anchored-spec`, `m-labelled-builtins`, `m-labelled-types`, `m-positional-values` | 22 | one stage | 23,724,113 | `10fc02615e3aad13` |
| 18 | `1e0c4ce9f` | 2026-09-11 | `m-named-callbacks` | 22 | one stage | 23,739,680 | `43cc6d0a9535e9ae` |
| 19 | `29970839f` | 2026-09-12 | `m-rotated-records` | 22 | one stage | 23,981,951 | `5648c077f499a9fe` |
| 20 | `e8b2a5ae9` | 2026-09-12 | `m-stated-grammar` | 22 | the seed of `476a2692e` (§ 3, a seed whose sources are an earlier commit's) | 23,981,959 | `18ecfed7a651992d` |
| 21 | `d65dc646a` | 2026-09-12 | `m-reflection-verdict` | 22 | one stage | 24,091,256 | `afc7d52e9b31b65c` |
| 22 | `80d81aef5` | 2026-09-13 | `m-deferral-ledger` | 22 | one stage | 24,433,607 | `974ce680b6b0b66c` |
| 23 | `d952482e3` | 2026-09-14 | `m-handle-verdict` | 22 | one stage | 24,433,458 | `00a77b3e372ac9bc` |
| 24 | `1e1bf870f` | 2026-09-14 | `m-cleanup-verdict` | 22 | one stage | 24,451,348 | `f07374368306a28e` |
| 25 | `b74009292` | 2026-09-14 | `m-marked-acquisition` | 22 | one stage | 24,604,877 | `040255314c2e25a8` |
| 26 | `b4403d60e` | 2026-09-17 | `m-check-completeness` | 22 | one stage | 25,000,736 | `aef39dc3cf1611c1` |
| 27 | `fc69a189c` | 2026-09-18 | `m-arm-platform` | 22 | one stage | 25,041,820 | `0cfad98a96eb2c67` |
| 28 | `6e2020b78` | 2026-09-18 | `m-readable-bytes` | 22 | one stage | 25,113,751 | `37d32983f22d44c3` |
| 29 | `420f929ce` | 2026-09-21 | `m-declared-extents` | 22 | `71bff89a2` | 26,212,594 | `00b8b2c9591fa35e` |
| 30 | `55d20a362` | 2026-09-28 | `m-agreed-retention` | 22 to 26 | `a8234a98f`, `a3a04d4ee` the builder linked with its runtime (§ 3.6) | 30,675,263 | `0bef1dfe91c51dcd` |
| 31 | `eeb239370` | 2026-10-11 | `m-inferred-cell` | 26 to 30 | `0650990ff`, `9066bf64f`, `6d022039e`, `1bd966b59`, `6934f73fc` a lane's own commit (§ 3.2), `b24bf79ab`, `dddea0cf3`, `fcec76d8d` its rewrite undone (§ 3.4) | 52,028,131 | `769abf23a8c7096b` |
| 32 | `cc9cc321c` | 2026-10-11 | none | 30 | one stage | 52,119,283 | `4882373884cc9fa3` |
| 33 | `461be3b94` | 2026-10-12 | `m-issue-files` | 30 | one stage | 52,126,543 | `08f92aa218fad462` |
| 34 | `6fe850445` | 2026-10-13 | `m-typed-inspection` | 30 | one stage | 57,395,081 | `0037e53be954f4ff` |
| 35 | `9b4822b2c` | 2026-10-15 | none | 30 | one stage | 51,688,146 | `744cb301066f18a7` |
| 36 | `6e2690b50` | 2026-10-16 | none | 30 | one stage | 51,688,264 | `8194b3eb4ffc4934` |
| 37 | `bf6a9c4eb` | 2026-10-17 | none | 30 | one stage | 51,514,006 | `f1456eee44adebc5` |
| 38 | `03b6cd829` | 2026-10-17 | none | 30 | one stage | 26,229,018 | `92549ab45aa4eca0` |
| 39 | `d1f95674b` | 2026-10-17 | none | 30 | `d1f95674b` the scaffold (§ 3.5) | 26,240,524 | `bc9a444fef09664c` |

## 5. What it does not say

- **One machine and one C compiler.** gcc's half is the milestone's second
  item, on the CI's Linux legs since `85fea83af` (task 2096).
- **The hybrids of § 3 are not commits**: `ff7f9d57a` with one file of its
  parent, `d1f95674b` with two, `fcec76d8d` with its rewrite undone on 6,152
  lines, and `a8234a98f`'s compiler compiled with `a3a04d4ee`'s runtime. Each
  was built once, and each worktree was checked as the commit's own tree, no
  path changed, before the next pass. A put-back changes one line of a copy of
  `runtime/heroes_runtime.h`. No tracked file was touched.
- **The walk's own tools ran outside the tree**, since the tree takes no
  script (CLAUDE.md § 10). The logs of every walk, the scripts that wrote them,
  the crossings' scripts and logs and the table's generator stand in
  `048-every-seed-since-the-archive-descends-from-the-one-the-rust-bootstrap-emits-39-of-39-reproduced-byte-for-byte.tar.xz`,
  beside this file, the repository's absolute path written `<repo>` and the
  scratch folder's `<scratch>` in them; the compilers are not kept. A link of
  the ordinary kind is three commands:

```sh
git worktree add --detach w <link k-1> && cd w
clang -I runtime seed/heroes.c runtime/runtime.c -o k0 && git checkout --detach <link k> && rm -rf build
./k0 build selfhost/main.hero -o k1 && rm -rf build && ./k1 build selfhost/main.hero --emit-c | cmp - seed/heroes.c
```

  with `HEROES_RUNTIME` naming the newest runtime of link *k − 1*'s ABI where
  link *k*'s runtime has another.
- **Today's sources were not asked for today's seed.** Under M-bounded-seed's
  rhythm the seed is refreshed only where the two-stage build goes red, so it
  lags the sources by design; the CI's fixpoint step checks that the two agree.
- **The history as it was**, under `refs/archive/` on `origin` (measurement
  045 § 7), carries the seeds the rewrite left out; this walk did not use it.
- **From the next refresh on, a link is witnessed when it is made**: the
  author's instruction of the same evening, `seed/README.md` § When this file
  must be regenerated, and the CI's job *A refresh of the seed is rebuilt by
  the seed before it*, whose script rebuilt links 38 (one stage) and 39 (the
  scaffold) byte for byte on this Mac before it entered the workflow.
