# Panel 241, shared brief: how the compiler carries `core`, where `crypto/sha256`'s C comes from, and how much the second version replaces

Written 2026-11-10 by the coordinator, **rewritten the same morning on the completeness critic's
first pass** (`../241-reports/critic-briefs.md`, 153 lines), which found the first version's third
fact false and three framings wrong; what changed is listed at the end. Every number and path below
was produced by a command run while writing or by the critic's first pass, named beside it; a fact
carried from another document says so and is a claim for the seats to re-run.

## Why the sitting sits

The author's answer of 2026-11-10, about 07:35, put with the coordinator's recommendation: *first a
reduced sitting* on M-core-packages' step 6, feature 2315 (title, by `gh issue view 2315`: *"The
package manager rewritten over the core packages, every golden of its interface unchanged"*). The
soundness lane by the author's word: two seats, the completeness critic before and after; no blind
seat, no historian, no paid run by anyone. **The lane holds only where the adopted resolution moves
no spec token**: panel 234's R6 prices the per-capability archive's spec sentence at +37 real tokens
in the commit that ships its first header (the critic, item 3), so a route that needs it is recorded
as owing the full panel, not adopted here.

## The tree

`main` at `87d77dec2a08ad87b1593207e1015dee090bd5bb`, pushed 2026-11-10 at 06:15:18 (the critic, by
`git log`). Each seat has its own detached worktree at that commit and builds its own compiler there
from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build
selfhost/main.hero -o heroes-next`; the critic measured 109.6 s cold on a busy machine, a planning
figure only). No seat reads, builds or runs in another seat's worktree or in the trunk.

## The three questions

1. **The compiler carries `core` as a catalogue entry** (`selfhost/heroes.toml` and a placed tree
   `selfhost/heroes.packages/core@<version>-<key>/`, written by `heroes add core --index
   file://../packages -C selfhost`, reached by `use <core>/...`). **This is a new ruling, not a
   reading of an old one**: panel 234's R13 is about a stable sort in `core/slices`, a package in no
   index, admitted to the compiler *"only if the admission test admits it for its 26 lines"*
   (`sed -n 488,499p docs/panel/234-*.md`), and panel 235's R15 names *"the 13 copies, plus the
   canonical tree"*, never the compiler (line 744 onward). So the compiler's admission count is owed:
   which functions of `core` 0.2.0 the compiler would call, and the hand copies they replace.
2. **Where `crypto/sha256`'s C comes from**, four routes, none adopted: (a) `pkgconfig "libcrypto"`,
   which M-core-packages' own page plans (`sed -n 32p docs/roadmap/milestones/M-core-packages.md`:
   *"`crypto/sha256`, `crypto/hmac` | `pkgconfig "libcrypto"` | writable, Mac and Linux"*) and
   design.md's table answers (*"Hashing, crypto | OpenSSL, libsodium"*, line 557); (b) the package
   ships its own C header (the critic, item 8: `fixedbugs-2351` shows a package's header works);
   (c) the runtime's `runtime/parts/sha256.c` through a header a program binds, `hero_compiler.h`
   today or `hero_os.h`, whose every function decision 2228 promises stable (design.md:3641, the
   critic), with a third door, since `hero_sha256_bytes` is `static` (`sha256.c:83`) and the two
   public doors take a path or a string; (d) Heroes alone (carried: 0.52 s on 4 MB against 0.06 s
   for the C door, **at `-O0`, the compiler's own level**, the 2315 comment).
3. **How much the second version replaces**: panel 235's R19 ends *"The second version replaces the
   transport alone"* (`grep -n 'replaces the transport alone' docs/panel/235-*.md`, line 796), and
   feature 2315 and M-core-packages' page say *rewritten over the core packages*. Routes: only
   `core/strings` enters and the package manager keeps its C doors; the transport moves to an
   `os/exec` package over `hero_os.h`; the whole closure the scoping listed.

## The facts, each with its command

1. **A program reads a file's bytes today**: the critic's probe
   (`.claude/worktrees/p241-scratch/critic/bytes/main.hero`) binds `stdio.h`'s `fopen("rb")` and
   `fread` with § 13's fill form (`@buf: [u8] counted_by n lent`, spec line 395) and printed `5 0 1 2
   255 254` for a 5-byte file; `tests/golden/run/fixedbugs-227-the-shown-read-names-every-byte.hero`
   reads every byte through `hero_file_read_shown`. Only `read_file` refuses such a file (`not_text`,
   the coordinator's probe, `main`'s compiler `sources 110fad075b8d3bca`); it is written at
   `selfhost/library_source.hero:206` (the critic). M-core-packages' page plans `io` over `stdio.h`
   (line 27).
2. **A program can bind `hero_compiler.h` today**: the critic's probe
   (`.claude/worktrees/p241-scratch/critic/sha/main.hero`) calls `hero_file_sha256` and
   `hero_text_sha256`, digests equal to `shasum -a 256`; three tracked programs bind that header (the
   critic, item 2); no rule refusing it was found (searched by the critic, item 2). It declares
   `hero_env_put` (line 28), `hero_file_sha256` (52), `hero_text_sha256` (56) (`grep -n`); the
   header's own comment (lines 10 to 14) says why it is apart from `hero_os.h`, and moving a name
   between them moves `heroes_guard_open.h` and `selfhost/emit/guarded_names.hero` (the critic, item 7).
   `HERO_RUNTIME_ABI` is 32 (`runtime/heroes_runtime.h:108`).
3. **What the guard runs**: `.claude/settings.json:69` runs the trunk's `guard_bash.py`, which asks
   the trunk's `provenance.py`; a copy patched in a worktree changes nothing (the critic patched and
   reran: the same refusal). So route 1's `fetch` goldens run only once the trunk's hook learns
   `use <name>/...`, a change that owes the hooks' own tests.
4. **Carried, to be re-run** (the scoping on feature 2315, its scratch in
   `.claude/worktrees/v2-scope-scratch/`): route 1 built green over two stages, stage 2 and 3
   emitting identical C, the compiler's own tests 2,006 against 1,992 (1,992 confirmed at 87d77dec2
   by the critic); R13's literal copy at `selfhost/core/` refused at exit 1; `order` 2 passed and 1
   failed on core's `encoding/json.hero:186`; the package manager's 33 calls whose contract is
   `core/strings`' (`has_prefix` 22, `has_suffix` 6, `contains` 5).

## The costs every route of question 1 owes (the critic, item 7)

The latency of `heroes check selfhost/main.hero` and of the warm build, since every check reads the
placed tree's fingerprint (`tests/harness/latency.hero compare`; the level is 8.9 s and 49.1 s since
decision 2396); the instruments that walk `selfhost/` (`order`, `probe`, `spec`'s anchors, `layout`,
`canonical`, `seed`), and `packages`, which walks `examples` only, so nothing would hold
`selfhost/`'s placed tree to the index; the site's sizes and the ROADMAP's line count, which would
count `core`'s modules as the compiler's; the seed, whose `#line`s name the placed folder, so every
`core` version the compiler takes is a seed refresh (`seed/README.md` § When this file must be
regenerated).

## What every seat owes

- Its report written as it goes, to
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p241/docs/panel/241-reports/<seat>.md`.
- A verdict per question (approve a route, object, veto), its cost in lines and seconds where it
  builds something, a falsifiable prediction with the milestone where it becomes checkable, and the
  condition that would change its verdict.
- Every number from a command it ran, named beside it; a negative claim written as what it searched
  and where.
- No paid run of any kind; never print a credential (`gh auth status` if it must, never `gh auth
  token`); no file written outside `/Users/joseph/Temp/heroes/heroes-lang/`; no `git push`, no write
  to GitHub, no commit in the trunk, no edit of the trunk's hooks. A program that might not end is
  built and its binary run under `timeout`, its output bounded.

## What changed from the first version, on the critic's first pass

Fact 3 said no program reads a file's bytes: false (fact 1 above), so the first proposal's item 2,
a new byte door, lost its premise. Item 3 was framed as reach: a program binds `hero_compiler.h`
already (fact 2), so the question is where hashing's C belongs (question 2). Item 1 was framed as a
reading of R13: R13 is about `core/slices` (question 1). The provenance patch in a worktree cannot
work (fact 3). The lane's premise now has its exception named (above). The push time, the build's
duration and 2315's title are corrected.
