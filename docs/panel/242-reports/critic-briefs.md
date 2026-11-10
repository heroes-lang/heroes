# Panel 242, the completeness critic's first pass, over the briefs

Written 2026-11-10, between about 11:58 and 12:15, as the checks ran. Read-only against the trunk at
`1edabb957` (`git -C /Users/joseph/Temp/heroes/heroes-lang rev-parse HEAD` printed
`1edabb9573a00db79fadda2bff695e98371a3290`, branch `main`; `git tag --points-at 1edabb957` printed
`m-package-manager`). Briefs read: `242-briefs/00-shared.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, `spec-warden.md`, `historian.md`. Scratch:
`.claude/worktrees/p242-scratch/critic/` (the 241 evidence diffs extracted to `ev/`, the outputs
named below). No verdict is given here. One slip of mine is recorded at the end (item 18).

Items run most important first. Each one carries the command and what it printed.

## A. Facts found false or stale, and what the sitting is not told

### 1. Panel 241 is ratified, and its ratified R8 asks that the blind seat's paid run be put to the author first

`00-shared.md:10` says *"decision 2415, provisional"*, and line 16 says the reversed order was
*"put to the author with 2415"*. Two commits landed on branch `p242` after the brief was written
(its mtime is 11:49):

```
$ git -C .claude/worktrees/p242 log --oneline -3
16e5038b8 verification.md: after a push that touches selfhost/print/, one strided pass ...
5bb631868 Panel 241 ratified: R1 to R8, R8's order reversed by the author's reading
1edabb957 M-package-manager: the tag ...
```

`git show 5bb631868` adds to 241's file: *"Ratified, R1 to R8, 2026-11-10, with R8's order
reversed ... What the yes settles: R1 to R7 as adopted, and R8 with that order."* GitHub has not
caught up:

```
$ gh issue view 2415 --json state,title
2415 OPEN ... :: Panel 241: ... R1 to R8, ratification pending
```

The commit body says *"decision 2415 closes with this commit"*. So the brief's "provisional" is
stale, and the record and GitHub disagree about 2415 until it is closed.

**What matters for this sitting**: R8, as ratified, ends *"then the full sitting on R4, R6 and R7,
the blind seat's paid run put to the author first"*, and 241's *Author's verdict* says *"the blind
seat's paid run of that sitting, put to the author before it runs"*
(`sed -n 258-268p` and the closing paragraph of docs/panel/241-*.md). The ratification changed only
the order. `00-shared.md:22-26` leaves the blind seat out on the coordinator's own grep and does not
say the paid run was put to the author. See item 2 for whether the substance holds. Either way, the
ratified text asks for a question to the author that the brief does not record as asked.

### 2. Leaving out the blind seat: sound on the token test, but the grep behind it missed the sentence that matters, and the sitting fits neither of `/panel`'s lanes

The brief's evidence, re-run:

```
$ grep -n -i 'standard library\|compiled into\|no package' spec/heroes-spec.md
34:  `heroes add json` writes that entry; a program that uses no package has no
```

True, but the vocabulary is narrow. A wider grep (`prelude|library|runtime|hero_os|environment|
env_|getenv|setenv|libcurl|curl|git|seed|fetch|transport|index|fingerprint|catalogue|heroes add|
read_file|validated|sha256|crypto|compiler`) finds the line every Q2 and Q3 route sits under:

```
$ sed -n 374,375p spec/heroes-spec.md
Anything beyond this document — sockets, maths, JSON, databases — comes from C
libraries; threads, processes and files from the language's `hero_os.h`. ...
```

Decision 2228 (closed) is what put `hero_os.h` there. Its *What was done* says *"spec § 13's
opening sentence names it for threads, processes and files ... its functions' stability is promised
with the rest at M-compatibility-promise"* (`gh issue view 2228 --json body`; design.md:3639-3645).
So:

- **Q3**: moving `hero_env_put` into `hero_os.h` adds a function to the one header the spec names.
  The spec never lists that header's functions, and `hero_env_shown`, the reader, is already there
  (`runtime/hero_os.h:84`). So no token is forced, but whether *"and the environment"* belongs
  beside *"threads, processes and files"* is a real spec-wording question. The spec-warden's brief
  asks it in general terms; the shared brief's grep could not have found it.
- **Q2(b)**: an `os` package over `hero_os.h` is the spec's own sentence (*"processes ... from the
  language's `hero_os.h`"*). No token moves.
- **Q1**: the spec describes a program's catalogue (lines 31-37) and says nothing of the compiler.
  No token moves under (A), (B) or (C).
- **Q2(c)/(d)** and **Q4**: the spec names no seed, transport or build line. No token moves.

**My judgment**: no route as listed moves a token of `spec/heroes-spec.md`, so the blind seat's
input (the spec alone) would not differ, which is CL-023's test (*"choosing only the seats whose
input differs"*, case-law.md:415-420). The exclusion is sound in substance on that condition, with
two caveats:

- the brief should cite 374-375, not 34, as the sentence it checked;
- the process is off. `.claude/skills/panel/SKILL.md:3-34` defines exactly two lanes, soundness
  (two seats) and full (five), and says *"When in doubt take the full panel"*. A four-seat sitting
  is neither lane, and the skill asks the synthesis to record *"which lane ran, and says what the
  lane gave up"*. Combined with item 1's ratified R8, the paid run is the author's to refuse (a paid
  run is one of the four kinds that *"stay the author's every time"*, verification.md § The batch).
  The record does not show it was put to them.

### 3. design.md:570-572's source is panel 234's R1 veto of route B5, which the brief never names, and panel 234's R13 already lets the compiler take a copy

`00-shared.md:39-43` frames the sentence by panel 097's condition 5 alone. The sentence is a
2026-11 text:

```
$ git log --format='%h %ad %s' --date=short -S'No package is compiled into the compiler' -- docs/design.md
b8f29508b 2026-11-08 M-package-manager step 14f: design.md §1.11 says how a core package reaches a program, panel 235's R15
79a42b509 2026-11-07 Panel 234's amendments to design.md, task 2301
```

Its source is panel 234's R1, ratified 2026-11-06 (234 file :256-268): *"**No core package is
shipped with the compiler or the runtime**: route B5 is vetoed (the compiler-engineer, on panel
097's condition 5 ...)"*. And B5 was a specific act:

```
$ sed -n 200,201p docs/panel/234-briefs/00-shared.md
... **B5, the canonical tree compiled into the compiler**,
panel 028's P-embed applied to `core/` (F2); ...
```

234's compiler-engineer report (:180-192) vetoes B5 as *"that reason applied to twenty packages
rather than one file"*: the prelude's embedding, packages compiled in for programs to use. Route
(A), the compiler depending on a package as any program does, is a different act from B5. So the
sitting is not told that Q1 may be a reading of a ratified veto's scope rather than an amendment of
Part 1. That decides what (A)'s amendment must say, and whether it amends anything at all.

Second, 234's R13 (ratified, :488-499) reads *"the compiler takes a copy under `selfhost/core/` only
if the admission test admits it for its 26 lines"*. That is a ratified ruling that the compiler may
take a package's copy. It is route (C)'s precedent, it is what 241's R4 means by *"panel 234's R13
already let the compiler take a checked copy"*, and it is the form 241's R3 measured not compiling
(exit 1). The shared brief does not cite it, so the seats are not told that one ratified text
already contradicts design.md:570-572's letter.

### 4. The compiler already carries `crypto/sha256`'s contract twice, so Q4's "before or after" hides the one package (A) would replace code with, and R5's fingerprint question was dropped

```
$ wc -l selfhost/cli/sha256.hero          -> 292
$ grep -rl '^use cli/sha256$' selfhost tests/harness
selfhost/module/fingerprint.hero selfhost/module/placed.hero selfhost/cli/check_json.hero
$ grep -n 'hero_text_sha256(text: text) == sha256.of' selfhost/module/fingerprint.hero
76:        assert hero_text_sha256(text: text) == sha256.of(text)
```

The compiler has a 292-line SHA-256 in Heroes (placed-folder names at `module/placed.hero:112,117`,
`check --json` at `cli/check_json.hero:132`) beside the C doors `hero_text_sha256` and
`hero_file_sha256` (fingerprints, `module/fingerprint.hero:23-25`, `fetch/index_source.hero:39-41`),
and a test asserting the two agree. Panel 241's R5 writes `crypto/sha256` in Heroes alone *"`compress`
and `hex8` copied from `selfhost/cli/sha256.hero`"*. Under R3's *"only scoped to **replace**"*, that
package is the first one whose admission into the compiler would delete code (292 lines, perhaps
the two C doors). Over `core` 0.2.0 the deletion was zero (241's R1).

- The compiler-engineer's brief counts `path`-shaped helpers for Q4 and not this.
- R5's last sentence, *"whether its own fingerprint moves to (d) goes with R4"* (241:229-233), sent
  that question to this sitting. `00-shared.md` § The four questions does not carry it.

### 5. A Q3 route nobody listed: the environment written for the child alone, not the process

Every caller of `hero_env_put` writes a variable just before it starts a child:

```
$ grep -rn 'hero_env_put(' selfhost tests/harness --include='*.hero' | grep -v function
selfhost/cli/symbolized.hero:57   ASAN_OPTIONS, before the sanitized program starts
selfhost/fetch/made_repo.hero:68-69   GIT_AUTHOR_DATE, GIT_COMMITTER_DATE, before git
selfhost/fetch/transport.hero:160-161 GIT_TERMINAL_PROMPT=0, GCM_INTERACTIVE=never, before git
tests/harness/suite_fetch.hero:86     IDENTITY, before git
tests/harness/case_setting.hero:214,223  a case's PATH / GIT_CEILING_DIRECTORIES, around shell.run
```

The spawn takes no environment:

- POSIX, `runtime/parts/run.c:1083`: `execvp(program, hero_run_words)`, inheriting `environ`;
- Windows, `run.c:1359`: `CreateProcessA(NULL, ...)`, with no block of its own. The comment above
  `hero_env_put` (`runtime/parts/os.c:900-905`) says it relies on that.

A `hero_run_env(name, value)` staged like `hero_run_arg` (`envp` on POSIX, an environment block on
Windows) would remove the process-wide write, the CRT-copy question and the empty-value question
together. It would also move `hero_compiler.h`'s `hero_env_put` off the table rather than into
`hero_os.h`. The harness already does the POSIX half by hand: `case_setting.hero:60-63`, *"The one
program that starts a case's compiler with variables of its own ... POSIX's `env`"*. Unrun, unbuilt,
cost unknown (the Windows arm would need `CreateProcessW` with `CREATE_UNICODE_ENVIRONMENT` or an
ANSI block, which is a question, not a measurement). A second, narrower route is also unread: git's
own `-c` settings, which `transport.hero:150-151` already passes (`transfer.fsckObjects=true`), for
`GCM_INTERACTIVE`. Whether git takes both variables as `-c` is unread.

### 6. The empty-value caller the ffi-pragmatist is pointed at sets only variables whose empty value reads as unset

`ffi-pragmatist.md:25-26` and 241's R7 cite `tests/harness/case_setting.hero:212-226` as the caller
that passes `""`. Read whole:

```
$ sed -n 50,53p tests/harness/case_setting.hero
# The variables a case's `environment` may set in this process, put back after its run.
constant SETTABLE: [str]
    ["PATH", "GIT_CEILING_DIRECTORIES"]
```

Line 85's refusal calls these *"the variables whose empty value reads as unset"*. And
`shell.env` answers `ok("")` for a variable that is not set (`tests/harness/shell.hero:938-945`), so
the restore at :223 writes `""` for a variable that was unset. On POSIX it leaves that variable set
and empty; a Windows removal would be the more faithful restore. So whether
`_wputenv_s(name, L"")` removes the variable is a question about `hero_env_put`'s promise, and it is
worth running. But this caller is not at risk, by its own design, and the brief presents it as the
risk.

### 7. Route (c)'s "the seed's one clang line gaining libcurl" reaches far past the seed: costs nobody listed

```
$ git grep -c 'seed/heroes.c runtime/runtime.c' 1edabb957 | wc -l    -> 336 files, 382 lines
```

Most are dated records, which are not rewritten. The living ones:

- the contract and readme: `CLAUDE.md` § Commands, `README.md` (2), `seed/README.md` (4, the
  Windows line among them);
- the workflows: `ci.yml:588`, `ci.yml:646` and the gcc line `ci.yml:1018`, `codeql.yml:137`,
  `deploy-site.yml:143`;
- the site: ten files, with `site/src/lib/claims.ts:325,934`, whose `STAGE_ONE` the site's build
  holds to the line, so the change is a site publish (CLAUDE.md § Hard stops);
- three hooks that build compilers with it: `fmt_check.py`, `guard_bash.py`, `trees.py`;
- `tests/harness/suite_latency.hero` (3), whose clock "clang on the seed" has a level of 10.3 s
  (verification.md § The latency ceiling).

The platforms:

- `ci.yml:517`: *"Windows carries clang with Visual Studio; sqlite, curl and raylib are not there"*;
- `otool -L ./heroes` prints `/usr/lib/libSystem.B.dylib` alone today, so a libcurl linked into
  `heroes` is loaded by every verb, not only `fetch`;
- ROADMAP row 83, M-standalone-compiler, is scheduled with the author's *"everything, as Zig does"*
  (its page's opening), and panel 235's R10 leaned on `build`, `check` and `run` needing neither
  program.

None of this is in either brief.

### 8. Under every Q2 route `git` stays a program; route (c) can only move the index's HTTP read

Panel 235's R19 (:788-796) fixes *"the catalogue's file, format and fields (`version`, `git`,
`tag`, `commit`, ...)"* and *"a git source that is a local repository"* among the goldens. Its R10
(:693-700) is *"never libgit2"*. So no route (a) to (d) replaces `git`. Route (c) (libcurl) and the
compiler-engineer's *"what the compiler's index reading gains over running curl"* concern
`index_source.hero`'s HTTP read alone. The shared brief lists (c) and (d) as if they were
alternatives for the whole transport.

### 9. The libcurl binding the ffi-pragmatist is asked to write is the form defect 2229 measured refused, and none of its three machines can show it

`ffi-pragmatist.md:12-13` asks for *"the binding a package would carry (`pkgconfig "libcurl"`)"*.
The tree's libcurl program does not bind `curl/curl.h`:

```
$ sed -n 40p examples/curl/main.hero
extern "curl_bound.h" link "curl"
$ grep -v '^\s*$' examples/curl/curl_bound.h | tail -2
#include <curl/curl.h>
#undef curl_easy_setopt
$ gh issue view 2229 --json title,state
2229 CLOSED :: ... `examples/curl` does not build against libcurl 8.18, whose `curl_easy_setopt` is a macro holding a `_Pragma`
$ pkg-config --modversion libcurl                                         -> 8.7.1 (this Mac)
$ docker run --rm heroes-linux-arm64:latest sh -c 'pkg-config --modversion libcurl; . /etc/os-release; echo $PRETTY_NAME'
8.14.1
Debian GNU/Linux 13 (trixie)
```

Libcurl 8.18 is the CI x86-64 leg's (Ubuntu 26.04). The Mac's 8.7.1 and the container's 8.14.1 are
both below it, and 241 recorded *no `pkg-config`* on the box. So a plain `pkgconfig "libcurl"`
binding can pass on all three and still be refused on the CI leg. A package that carries
`curl_bound.h` ships C of its own, which meets design.md:572 (*"none ships platform C of its own"*)
and 241's R5 (*"(b) is not core's route"*). Whether a two-line `#undef` header is allowed there is a
question the sitting does not ask.

### 10. `selfhost/cli/process.hero:37-56`, the `os` row's citation, is stale, and the group it means carries open defect 2413's shape

The milestone page's row (line 26) was written 2026-09-12 (`git log -L26,26:...` -> `efb769424`).
At 1edabb957:

- lines 37-38 are `use cli/strings` and `use not_text`;
- the `extern "hero_os.h"` group runs 40-64 (21 functions, 2 constants);
- lines 66-69 bind `hero_dir_scan`, `hero_dir_at` and `hero_dir_release` under `extern "stdlib.h"`,
  although `runtime/hero_os.h:342-352` declares them. `module/spelling.hero:22-24` does the same.

```
$ grep -n '^extern' selfhost/cli/process.hero          -> 40:extern "hero_os.h"   66:extern "stdlib.h"
$ gh issue view 2413 --json state,title   -> OPEN :: Defect 2413: A group may name any header for a function the runtime's `hero_os.h` declares: it builds ...
```

An `os` package lifted from `cli/process` (route (b)) inherits 2413's shape unless the lift repairs
it. 00-shared.md:68 carries the citation unchecked.

### 11. Programs that could admit a package without touching design.md:570-572: a Q1 and Q4 route nobody listed

Decision 2404 (closed) names *"the compiler and the net"* as the programs that write `path` by hand.
The milestone page names two more acceptance programs:

- the HTTP server (page line 183);
- the hooks rewritten in Heroes (`gh issue view 2082` -> OPEN, M-core-packages, *"the repository's
  own hooks rewritten in Heroes over the core packages"*). The page says its *"314 calls of
  `os.path`"* ask `path/filepath`'s contract (446-452); `grep -rn 'os\.path' .claude/hooks/*.py | wc
  -l` prints 312 lines, a different ruler.

None of these is compiled into the compiler or the runtime, so the sentence Q1 is about does not
reach them. The route: `path/filepath` (and `os`) admitted by 2082 or by the net
(`tests/harness/`, no `heroes.toml` today), giving a measured package of the tree's own before Q1
is answered for the compiler. It pays no seed refresh. The provenance mirror's reach over a
non-compiler program is unread. Not in the brief's Q1 or Q4 routes.

## B. Facts verified, with their commands (true as written unless noted)

12. Verified:

- **Spec grep**: line 34 alone (above).
- **Paths**: the 241 file path exists; `docs/panel/097-the-platform-arm-belongs-to-the-runtime.md`
  128-130 holds condition 5, with an em dash where the brief writes `--`; `selfhost/library_source.hero`
  exists.
- **Module counts**: `grep -rl '^use cli/strings$' selfhost | wc -l` -> 125, also with `as` forms.
  It was 114 at 241's base: `git grep -l ... 87d77dec2 -- selfhost | wc -l` -> 114, R2's repairs
  having moved copies onto it, so 241's *"the `as` the 64 modules owe"* is stale and must be
  recounted. `cli/process` -> 96 at both commits; in `selfhost/fetch` -> 11; `ls
  selfhost/fetch/*.hero | wc -l` -> 21.
- **Extern groups**: `git grep -n '^extern' 1edabb957 -- selfhost/fetch
  selfhost/module/fingerprint.hero` -> exactly the six lines named. They are six groups in four
  files, not "four C groups": four `hero_compiler.h`, two `hero_os.h`. `transport.hero:46`'s group is
  `hero_env_put`, so Q3's write is the transport's own.
- **Panel 235**: R10 at 693-700 with the quoted sentence at 699-700; R19 at 788-796 ending *"The
  second version replaces the transport alone."* (796).
- **Milestone page**: `grep -n '^| `os`\|^| `net/http`' ...` -> 26, 33 (and 35, the server).
- **Issues**: 2315 OPEN, M-core-packages, title as quoted. 2263 OPEN, M-publication-gate. 2404
  CLOSED; body lines 56-57: *"on the day its first program reaches `core`, most likely the package
  manager's"*. 2406 OPEN, class `improvement`, four sites in `fetch/` and four outside it
  (`mutate/`, the harness). 2228 CLOSED. design.md:3639-3645 carries 2228's promise.
- **The b28 round deleted four copies**: `git diff 87d77dec2 1edabb957 -- selfhost | grep '^-function'`
  -> `holds`, `has`, `has`, `opens_with`.
- **241's evidence**: `evidence.tar.xz` holds `evidence/compiler-engineer/route1.diff`, the hook
  mirror and its test among its 12 files. `git apply --check -v` of it at 1edabb957 -> exit 0 on
  all 12 (`apply-check.log`).
- **The index**: `cat packages/core/versions` -> 0.1.0 and 0.2.0, and 0.2.0's files hold
  `encoding`, `strconv` and `strings` only. `selfhost/heroes.toml` does not exist; route (A) creates
  it.
- **Worktrees**: `git worktree list` -> `242-compiler-engineer` and `242-ffi-pragmatist`, both
  detached at 1edabb957, `git status --short` empty, no compiler built in either yet.
- **The box**: `timeout 25 ssh -o BatchMode=yes win 'echo reachable; where curl; where git'` ->
  reachable at about 12:00, Git Bash, curl and git present. Run in no folder.
- **The rest**: `docs/platforms/linux/LINUX-MACHINE.md` exists and `heroes-linux-arm64:latest` is in
  `docker images`; `heroes_guard_open.h`, `heroes_guard_close.h` and
  `selfhost/emit/guarded_names.hero` exist; CLAUDE.md § 13 lists *"A standard library"*
  (CLAUDE.md:278-279).

13. Small inaccuracies, none of which changes a question:

- "design.md:570-571": the quoted sentence ends on 572 (`sed -n 565,575p docs/design.md`).
- Panel 097's *"design.md §1.11:443-445"*, quoted in `00-shared.md:43`: line 443 is now `### 1.8
  Compare against the real alternative` (`grep -n '^### 1\.' docs/design.md`); §1.11 opens at 482
  and its rule box is 490-492. A seat following the citation lands in the wrong section.
- "decision 2404 names": 2404 is closed. The brief reads it correctly but does not say so.

## C. Process gaps a seat will hit

14. **No compiler of the tree is named for the spec-warden.** Its brief says *"read-only in the
trunk"* and *"the vendored count is yours"*. The trunk's compiler is older than the tree:

```
$ ls -la heroes; ./heroes --version            -> Nov  3 23:08, sources 1a1e340778db789a
$ python3 -I -c '...provenance.tree_sources(trunk)'   -> tree 45824e7205ca021e
$ .claude/worktrees/p242-scratch/heroes-seed-o2 --version -> sources 5aae00294f7cd6fa
```

Neither binary was compiled from the tree. The spec-warden may build nothing in the trunk, and its
brief names no worktree. Whether the vendored tokeniser moved since 2026-11-03 is unread, so its
count would come from a compiler that may not be the tree's.

15. **Clocks beside builds.** `00-shared.md:89-90` asks every seat for *"its cost in lines and
seconds"*, and the compiler-engineer times `heroes check selfhost/main.hero` five times alternated.
Meanwhile the ffi-pragmatist builds compilers, runs the Linux container and uses the box from this
same Mac. CLAUDE.md § Verification says *"While a clock runs the machine stays still"* (CL-025). The
instructions-retired count 241 used is robust to load; seconds are not. No brief orders the two
seats' clocks.

verification.md § The latency ceiling also makes `heroes run tests/harness/latency.hero -- compare`
*"the only comparison the rule admits as a breach"*. The compiler-engineer's brief asks for
`/usr/bin/time -l` alone, so its number can show a cost and cannot declare one.

16. **The hook mirror.** `.claude/settings.json:69,81` runs `$CLAUDE_PROJECT_DIR/.claude/hooks/...`,
so 241's `provenance.py` patch applied in a seat's worktree is never the hook that runs. The
compiler-engineer's brief already says *"the runs it blocks stay unrun"*, and that is the only
outcome available under *"no edit of the trunk's hooks"*. Correct, noted so nobody tries.

## D. The questions the sitting should ask and does not

17. In order:

- **(i)** Does a compiler taking a package as its own dependency fall inside panel 234's R1 veto of
  B5 (the canonical tree compiled in for programs, P-embed) at all? If not, Q1 is a reading of
  design.md:570-572 back to its 2026-11-07 source. If so, (A) reverses a ratified veto, not a
  sentence (item 3). And how does panel 234's R13, ratified, which already lets the compiler take a
  copy, stand beside the sentence's letter?
- **(ii)** Which package would (A) delete code for today, and is `crypto/sha256` against
  `cli/sha256.hero`'s 292 lines and the two C doors that package (item 4)? If the compiler's
  fingerprint check became a package's code, can a compiler verify the placed tree of the package
  that implements its verification? This is a soundness question about the verifier being among the
  verified, unrun, put as a question.
- **(iii)** Should a process-wide environment write exist at all, or does the write belong in the
  spawn, for the child alone (item 5)? Question 3 as written assumes the write and asks only which
  header holds it.
- **(iv)** Is libcurl in `heroes` compatible with ROADMAP row 83's *"needing nothing else
  installed"*, and with the Windows leg, which has no curl library (item 7)? Question 2's route (c)
  asks only what the seed's line becomes.

## E. My own slip

18. While checking which compiler the spec-warden could use, I ran in the trunk
`./heroes build selfhost/main.hero --emit-c`, piped into `grep`, meaning to read the emitted
`hero_program_sources` key. It writes the build cache. It ran from about 12:05:50, moved to the
background at its 120 s timeout, and I stopped it (`TaskStop btbc2pin0`).

- What it left: 688 `build/tu-*` directories created between 12:06:26 and 12:07:50, plus other
  `build/` entries in that window (`find build -maxdepth 1 -name 'tu-*' -mmin -8 -exec stat -f '%SB'
  ...`), and nothing after 12:07:50.
- What it did not do: `git status --short` in the trunk is empty, no tracked file moved, and no
  `heroes` or `clang` process remains (`ps -axo ... | grep -E 'clang|heroes'`).
- `build/` is git-ignored and keyed by content, so the entries change no verdict. I removed nothing,
  since a removal in the trunk's shared cache is a destructive operation and the coordinator's to
  decide.

The read-only route was the hooks' mirror (item 14), which I used afterwards.
