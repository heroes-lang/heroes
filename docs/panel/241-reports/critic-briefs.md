# Panel 241, the completeness critic's first pass: the three briefs

Read 2026-11-10 at `87d77dec2` (`git log -1 --format=%H`, the trunk). Every program was built in a
detached worktree of that commit, `.claude/worktrees/p241-critic` (removed when done): stage 1 by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`--version`: `sources
5aae00294f7cd6fa`), stage 2 by `./heroes build selfhost/main.hero -o heroes-next` (`sources
110fad075b8d3bca`, the brief's key). Probes and logs are in
`.claude/worktrees/p241-scratch/critic/`. No paid run, no write to GitHub, no credential printed.

## A. Facts false, misframed or unverified, most important first

1. **False: "So no program reads a file's bytes today"** (shared fact 3, proposal item 2's premise).
   `critic/bytes/main.hero` binds `stdio.h` alone with spec § 13's fill form,
   `function fread(@buf: [u8] counted_by n lent, size: u64, n: u64, stream: File) -> u64` and
   `fopen(..., mode: "rb")`, built by stage 2, run on the coordinator's `blob.bin`: prints `5`, then
   `0 1 2 255 254`, exit 0. A tracked program already reads every byte another way:
   `tests/golden/run/fixedbugs-227-the-shown-read-names-every-byte.hero` binds `hero_os.h`'s
   `hero_file_read_shown`, whose marks name each byte that is not UTF-8 (`runtime/hero_os.h:65-77`).
   Panel 234's own R7 says the fill form "answers `read`, `recv`, `fread`"
   (`sed -n 430,440p docs/panel/234-*.md`). What is true is narrower: `read_file` cannot
   (`critic/sha/probe` re-ran the coordinator's probe: `not_text`). The origin of the false sentence
   is the scoping's comment on feature 2315 (*"no door reads a file's bytes"*,
   `gh issue view 2315 --json comments`).

2. **Misframed: item 3 moves SHA-256 and the environment's write "to a header a program may
   bind", and a program binds `hero_compiler.h` today.** `critic/sha/main.hero` declares
   `extern "hero_compiler.h"` with `hero_file_sha256` and `hero_text_sha256`; it builds and prints
   `0 aa5cd9ac...e9f60a` for `blob.bin` and `ba7816bf...15ad` for `"abc"`, both equal to
   `shasum -a 256`. Tracked programs bind it too (`git grep -ln 'extern "hero_compiler.h"' -- tests
   examples`: `tests/golden/run/fixedbugs-484-...hero`, `tests/harness/case_setting.hero`,
   `tests/harness/suite_fetch.hero`). Searched for a refusal and found none:
   `git grep -n hero_compiler -- selfhost/check selfhost/module selfhost/resolve` names only the
   compiler's own groups; only `heroes_runtime.h` is refused (`selfhost/check/runtime_header.hero`).
   What separates the headers is a comment (`runtime/hero_compiler.h:7-17`) and decision 2228,
   which promises `hero_os.h`'s functions at M-compatibility-promise (`docs/design.md:3641-3645`).
   So item 3 is a question of promise and of design.md §1.11, not of reach; the briefs should say so.

3. **Unverified premise of the lane: "the proposal moves no spec token".** It holds for the
   `hero_os.h` home only. Panel 234's R6 (`sed -n 400,428p docs/panel/234-*.md`): *"Spec § 13 gains
   the spec-warden's F3 sentence, priced +37 real, in the commit that ships the first header and not
   before."* If the sitting rules the per-capability archive the home of these doors and they are its
   first header, the spec moves.

4. **Unasked: item 3 may move design.md Part 1, which the soundness lane does not cover** (CLAUDE.md
   § 4). `docs/design.md:557`: *"Hashing, crypto | OpenSSL, libsodium"*; `:571-575`: *"No package
   is compiled into the compiler or the runtime ... So the language still ships its runtime and
   nothing a program could carry instead."* A SHA-256 door every program may bind is the runtime
   shipping a hash a program can carry (in Heroes, `selfhost/cli/sha256.hero`, 292 lines; or over
   OpenSSL). The milestone page plans the other route: `crypto/sha256`, `crypto/hmac` over
   `pkgconfig "libcrypto"` (`docs/roadmap/milestones/M-core-packages.md:32`).

5. **Misframed: R13 is not a ruling on the compiler carrying `core`.** `sed -n 488,499p
   docs/panel/234-*.md`: R13 is *"a stable sort by a caller's order is a function of the package
   `core/slices`"*, and its last sentence is conditional, *"only if the admission test admits it for
   its 26 lines"* (two comparator sorts in `selfhost/mutate/`). `core/slices` is in no index
   (`git ls-files packages`: `strings`, `strconv`, `encoding/json`). R15 names *"The 13 copies, plus
   the canonical tree"* (`sed -n 744,757p docs/panel/235-*.md`), not the compiler. So item 1
   (`strings` in `fetch/`) is a new ruling, with its own admission count, and feature 2315's body
   still says *"packages copied into the compiler's tree under `selfhost/core/`"*
   (`gh issue view 2315`), which the sitting amends or the feature keeps a false route.

6. **The compiler-engineer cannot follow item 1's provenance instruction: a patch in the worktree
   does not reach the guard.** Measured: the hook runs `$CLAUDE_PROJECT_DIR/.claude/hooks/run.sh
   guard_bash.py` (`.claude/settings.json:69`), the trunk's copy, which imports the trunk's
   `provenance.py` (`.claude/hooks/guard_bash.py:511`). In `p241-critic` I appended a `tree_sources`
   answering the stage-1 key to the worktree's `provenance.py` and ran `./heroes run
   tests/harness/main.hero -- ./heroes nosuchsuite`: refused with the same words as before the
   patch, *"keys 110fad075b8d3bca by the hook's mirror"*. File restored by `git checkout`.

7. **Unasked: no door hashes a `[u8]`, which is what item 2's `sha256.Sum256` needs.** The bytes
   core is `static void hero_sha256_bytes` (`runtime/parts/sha256.c:83`); the public doors take a
   path or a `HeroStr` (`grep -rn sha256 runtime/*.h`: `hero_compiler.h:52,56` only). And panel 234's
   R7: *"the read-only lend of a `[u8]` holds no ticket and is not adopted"*, so the bytes reach C
   through `@` with `counted_by`. A third door, and its shape, are owed and neither item names them.

8. **Carried number missing its condition**: *"SHA-256 in Heroes took 0.52 s on 4 MB against
   0.06 s"* drops the scoping's *"at `-O0`"* (the comment of 05:49 on 2315;
   `v2-scope-scratch/t_heroes.txt`, `t_c.txt`). `selfhost/cli/sha256.hero:12-20` says the compiler is
   built at `-O0`, so it prices the compiler's case, not a program's.

9. **Small corrections.** The push: `git reflog --date=iso origin/main` reads 06:15:18, not 06:16.
   Stage 2 cold: `real 109.60 user 83.15 sys 17.35` here, not "about a minute" (the machine was not
   still, so a planning figure only). 2315's title is *"every golden of its interface unchanged"*;
   *"passing"* is the milestone page's (`M-core-packages.md:415`). The compiler-engineer's item 5
   omits `selfhost/library_source.hero`, where `read_file` is written over `hero_os.h`
   (`git grep -n 'function read_file' -- selfhost`: `library_source.hero:206`).

10. **Verified as written**: the commit and the two seat worktrees detached at it
    (`git worktree list`); spec lines 356, 375, 395, 432 and no `hero_` function named in the spec
    (`grep -n 'hero_[a-z]' spec/heroes-spec.md`: line 375 alone); `hero_compiler.h:28,52,56`;
    `heroes_runtime.h:108` ABI 32; 2295 open, `M-core-packages` (`gh issue view 2295`); R15 at line
    744, its quotation and its amendment of R2; `--index` and `-C` exist
    (`selfhost/cli/table.hero:145-146`); every suite the compiler-engineer names exists
    (`grep -oE '"[a-z_]+"' tests/harness/main.hero`); `seed/README.md:142-169` (the rename rule of
    2026-11-10, `one-stage`, `abi-put-back`); `provenance.py:51`'s `USE` cannot match `use <`; the
    compiler's own tests at 87d77dec2: `1992 tests, all passed` (`critic/selftest.log`); `ssh win`
    answers.

## B. Routes the proposal does not list

1. **A package binds `stdio.h` itself** (A1, measured): no runtime change, no ABI, no wait on 2295.
   The milestone page's own `io`, `bufio` row plans it, *"`stdio.h` (`FILE*`)"*
   (`M-core-packages.md:27`). On Windows the runtime's manifest makes every narrow call read UTF-8,
   *"a bound C library's own included"* (`runtime/parts/codepage.c:11-14`); unrun on the box.
2. **Bind `hero_file_read_shown` and decode its marks**, as `fixedbugs-227` does: lossless today.
3. **Bind `hero_compiler.h` where it stands** (A2, measured): nothing moves, no guard header, no ABI;
   the sitting rules only whether a package may and whether it is promised.
4. **`crypto/sha256` ships its C floor in its own header.** A package's header is found from its own
   root and compiled (`tests/golden/run/fixedbugs-2351-a-fetched-header-is-found-from-its-own-root-and-runs-nothing-at-build.case/`,
   `pk.h` with `static inline` bodies). design.md:572 refuses *"platform C of its own"*; SHA-256 has
   no platform arm. Whether that line covers portable C is the sitting's to rule.
5. **`crypto/sha256` over `libcrypto`** by `pkgconfig` (A4's page row and design.md:557); Windows
   unmeasured.
6. **The package manager keeps its C doors and helpers, and only `core/strings` enters** (the
   scoping's step 1). Panel 235's R19 ends *"The second version replaces the transport alone"*
   (`sed -n 788,796p docs/panel/235-*.md`). What *rewritten* covers decides whether items 2 and 3
   are owed at all; the scoping asked it and the briefs do not carry it.
7. **`read_file` widened, or a bytes built-in**: closed for this milestone by its page, *"panel 097
   condition 5 keeps `selfhost/library_source.hero` closed"* (`M-core-packages.md:4-7`). The sitting
   should list it as refused, with that reason, so it is not proposed again.

## C. Questions the sitting should ask and does not

1. **Is this the soundness lane** once A3 and A4 are read? If a seat finds design.md §1.11 or § 13
   moves, the record says what the lane gave up (the spec-warden's price, the historian's precedent).
2. **Which program needs a file's bytes, by a command's count?** The fingerprint hashes files that
   are not UTF-8 (`selfhost/module/fingerprint.hero:11-14`); is that the named acceptance program,
   and does a runtime door answer the admission test or decision 2228's promise?
3. **What route 1 costs the latency ceiling.** Every build reading a placed tree checks its
   fingerprint (`fingerprint.hero:16-18`, `module/packages.hero`), so every `heroes check
   selfhost/main.hero`, every warm build and every layer-0 hook check of a `selfhost/` write would
   hash core's tree. The level is 7.50 s and 41.00 s (`.claude/rules/verification.md` § The latency
   ceiling); no brief asks for `heroes run tests/harness/latency.hero -- compare`.
4. **The instruments item 1 reaches beyond the compiler-engineer's list**: `seed` (the map's
   `selfhost/**` row, omitted); `packages` walks `"examples"` alone (`suite_packages.hero:82-93`), so
   nothing in the net holds `selfhost/`'s placed tree to the index; the site's `sizes.ts:26,62`
   counts every `.hero` under `selfhost/` as the compiler's (a core 0.2.0 tree is 10 modules and
   2,334 lines: `find examples/json/heroes.packages -name '*.hero'`), and so does the ROADMAP's
   `find selfhost -name '*.hero'`; the seed's `#line`s name the placed folder (99 of them in
   `examples/json`'s `--emit-c`, `grep -c '^#line.*heroes.packages'`), and the folder's key moves
   with the index's spelling (the scoping's `core@0.2.0-4029f4f2486e6ca7` against `examples/`'s
   `8c07af1fca61d171`), so each `core` version is a seed refresh by the rule of 2026-11-10.
5. **What moving names into `hero_os.h` costs every unit**: `hero_compiler.h:10-14` says it is a
   header apart because every unit reads `hero_os.h` and saves each word around its groups' headers
   (`runtime/heroes_guard_open.h`, held to `selfhost/emit/guarded_names.hero:72` by `macro_guard`'s
   tests). The ffi-pragmatist's cost list omits it.
6. **Who makes the provenance mirror follow `use <name>/...`, and when** (A6): a change to
   `.claude/hooks/**` owes the hooks' own tests and is the trunk's; until it lands the `fetch`
   goldens under route 1 stay unrun, as the scoping found.
7. **Does item 1 stand on R1's veto line?** R1 (`sed -n 256,286p docs/panel/234-*.md`): *"No core
   package is shipped with the compiler or the runtime"*, panel 097's condition 5. The compiler
   carrying `core` for its own use is not shipping it to programs; the sitting should say so in its
   own words rather than leave it read.
