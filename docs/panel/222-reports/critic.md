# Panel 222, completeness critic: route (a) as built, attacked with runs

Written 2026-10-17 from 13:25 as the work ran, in a detached worktree at `f728f420e`,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/222-critic` (removed at the end), `rm -rf
build` first, every compiler built from its own seed. The prototype attacked is the
compiler-engineer's, `scratchpad/222-ce/route-a-tracked.diff`, `route-a-provenance.hero`,
`tree_digest.py`, `ask_guard.py`, applied unchanged unless a line says otherwise. Scratch files:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/222-critic/`.
Every number names its command; what was not run says *unrun*. Machine: this Mac, other sessions
present (load 2.7 to 6.6 at 13:26 by `uptime`), so no duration below is a still machine's.

## 0. What the digest covers, read and counted

- **The `.hero` modules the root reaches, the embedded library left out.**
  `selfhost/emit/provenance.hero`'s `sources_of` keys every `s.files` entry with `!f.is_library`,
  each by the bytes `source.from_files` joined (one `\n` between files, belonging to neither,
  `selfhost/source.hero:136`), so a module's key is its file's bytes exactly. The byte is read
  unsigned (`runtime/parts/key.c:52`, `const unsigned char *u`), so `char`'s sign on a platform
  cannot move it.
- **Not the library the binary was compiled WITH.** The library is the building compiler's
  embedded constant (`selfhost/library_source.hero`, *EMBEDDED, never read from disk*), appended
  to every compilation (`selfhost/modules.hero:131`). So a stage 2 runs the SEED's library
  functions and embeds the tree's text for the programs it compiles, and prints the tree's digest.
  `ci.yml`'s own comment above *The seed is what today's source emits* names this class (*a stale
  seed builds ... and every job after it runs a compiler carrying yesterday's library*).
  `git log --oneline -- selfhost/library_source.hero`: 11 commits since 2026-08-17, 3 since
  2026-09-17.
- **Not the runtime.** The binary links `runtime/` at its build; programs it compiles take the
  runtime from `./runtime`, `HEROES_RUNTIME` or beside the compiler (`selfhost/main.hero:265-316`).
  `git log --since=2026-09-17 -- runtime`: 76 commits, **31 of them touching no file under
  `selfhost/`** (each commit's `git show --stat` grepped), so a runtime-only change is common, and
  the digest cannot see one.

## 1. The mirror against the binary, on this Mac

The binary's digest read from `heroes-s2w build <root> --emit-c` (the writer half applied, stage 2
built by the trunk seed's stage 1, `real 73.82`), the line `hero_program_sources(void) {...}`
grepped; the mirror's from `python3 -I scratchpad/222-ce/tree_digest.py <tree> <root>`. Programs
of two modules, `main.hero` using `sub/helper`, under `scratchpad/222-critic/`.

| case | binary | mirror | |
|---|---|---|---|
| plain `use sub/helper` | `50a5d6f369d264dc` | `50a5d6f369d264dc` | equal |
| **`use sub/helper  # the helper`** (canonical: `heroes fmt` prints it unchanged, `heroes check` exit 0) | `3efb0a427cb46c1b` | `341f030505aa2cce` | **differ**: the mirror's regex ends `\s*$`, so it follows no module from that line (*reached: 1*, *under / but not reached: 1*) while the compiler, reading tokens (`selfhost/module/paths.hero:45`), follows it |
| used module with no final newline | `6ab4dbe904088e89` | `6ab4dbe904088e89` | equal |
| used module in CRLF | `3fb6c4527fc7740a` | `3fb6c4527fc7740a` | equal (and unlike the LF one, as its bytes are) |
| used module with `é` and `§` | `2b8f26bd0df575ea` | `2b8f26bd0df575ea` | equal |
| used module opening with a BOM | refused, `error[unexpected_character]` | `6c85a4a97e99b6b8` | no binary exists to judge |
| root as absolute path, bare `main.hero`, `./main.hero` | `50a5d6f369d264dc` three times | | equal: the root is keyed by its stem, never its path |
| the same root copied to `other.hero` | `5c6d4f8a67976f2f` | `5c6d4f8a67976f2f` | equal, and unlike `main`: the root's stem is in the digest |

**Falsified: *the premise it rests on ... is checked by the run (an unresolved name is printed),
not assumed*.** The run checks the premise only for the use lines the regex matches; a use line it
does not match is invisible to it, and the run printed *missing: []* while it missed a module. A
second reader of the compiler's discovery is a second grammar for one rule, which is exactly what
`selfhost/module/paths.hero`'s own header warns against (*a prefix scanner would be a second
grammar for the same text*). The failure is loud, a refusal of every compiler of the tree whose
sources are the tree's, never a pass.

## 2. The landing: one commit is enough when it carries the right seed

The compiler-engineer's § 4: *So (a) lands in two commits with a refresh between*. Run here, in
order, in the worktree:

1. the writer half applied (`git apply --exclude=selfhost/main.hero route-a-tracked.diff`, the
   module copied), stage 2 built by the trunk seed's stage 1: `heroes-s2w`, one `--version` line;
2. the reader applied (`git apply --include=selfhost/main.hero`), and **`heroes-s2w build
   selfhost/main.hero --emit-c -o seed-wr.c`** (`real 41.11`): the seed of the tree WITH the
   reader, emitted by a compiler that has the writer, holding `hero_program_sources(void) { return
   hero_str_from_bytes("6710054c3edc42a5", 16); }`, the mirror's digest of that tree;
3. that file copied over `seed/heroes.c`, then the clean-clone path alone: `clang -I runtime
   seed/heroes.c runtime/runtime.c -o heroes-s1wr` (`real 7.95`), `--version` prints `sources
   6710054c3edc42a5`; `heroes-s1wr build selfhost/main.hero -o heroes-s2wr` (`real 32.06`), the same
   line; `heroes-s2wr build selfhost/main.hero --emit-c` and **`cmp` against the seed: silent**.

So **one commit carrying the writer, the reader and that seed builds from a clean clone and is its
own fixpoint**; the writer-only compiler of step 1 is scaffolding on the lander's machine, never a
commit. What the compiler-engineer measured stands and is narrower than the sentence drawn from
it: the deadlock is a tree holding the reader whose seed lacks the writer. **The condition is
therefore an invariant, not a count of commits: no commit may hold the reader with a seed that does
not define `hero_program_sources`**, and two commits with a refresh between is one way to keep it.

**What the rhythm's own recovery cannot do**, from the compiler-engineer's run and the
mechanism read here: the rhythm refreshes the seed when the two-stage build goes red, and a refresh
is emitted by the stage 2 that stage 1 builds. A tree in the forbidden state has no such stage 2
(`ffi_missing_link`), so the documented refresh cannot rescue it; only the scaffolding of step 1
can. That makes the invariant a check worth automating, not a sentence: under (a) the seed's own
line answers it (`grep -c 'hero_program_sources(void) {' seed/heroes.c` is 1 or 0).

**The same trap waits in every lane's merge.** `.claude/rules/records.md` § Working in lanes:
*`seed/heroes.c` is never resolved by hand: take one side, build, regenerate*. A lane opened before
the landing that takes ITS side of the seed in the merge holds exactly the forbidden state (its
seed has no writer, the merged tree has the reader), the state the compiler-engineer ran to
`ffi_missing_link` at `selfhost/main.hero:88`, exit 1. So the rule must name the side: the seed
that defines `hero_program_sources`, the trunk's, after the landing. An inference from the rule's
text and that run; no lane merge was run here.

## 3. The build cache refreshes the digest (holds)

`heroes-s2wr build selfhost/main.hero -o heroes-s3` (warm cache), then one byte of a comment in
`selfhost/cli/digest.hero` changed in place (`The content` to `The  content`, no line added, so
that module's emitted C cannot move), then `heroes-s2wr build ... -o heroes-s3e` (`real 31.88`):
**`sources 199f1cbd27d09707`, the mirror's digest of the edited tree**, `heroes-s3` still
`6710054c3edc42a5`, `cmp` of the two binaries differs. The root unit's C carries the digest, so it
recompiles whenever any module's bytes move, even a module whose own C did not. The file was then
restored byte for byte.

## 4. A compiler copied from before a runtime change passes both checks (falsified as a refusal)

In the worktree: `heroes-s3` built (13:31:25); `runtime/parts/array.c:182`'s panic text changed to
`array length overflow (critic 222)`; `cp heroes-s3 heroes-copy`; `heroes-s2wr build ... -o
heroes-s3r` (`real 75.07`, the runtime's object rebuilt). Then `strings <c> | grep -c 'critic
222'`, `--version`, and `python3 -I scratchpad/222-ce/ask_guard.py . heroes-s3 heroes-copy
heroes-s3r`:

| compiler | runtime inside | `sources` | the tree's digest | mtime guard | a digest check |
|---|---|---|---|---|---|
| `heroes-s3`, built before the change | old (0) | `6710054c3edc42a5` | `6710054c3edc42a5` | **refused** (*built at 13:31:25, before runtime/parts/array.c was written at 13:32:34*) | accepts |
| `heroes-copy`, a copy of it made after | old (0) | `6710054c3edc42a5` | `6710054c3edc42a5` | **passes** | **accepts** |
| `heroes-s3r`, built after | new (1) | `6710054c3edc42a5` | `6710054c3edc42a5` | passes | accepts |

So the compiler-engineer's *(a) refuses ... a copy from another tree* holds only for a tree whose
`.hero` sources differ. A copy between two trees that differ in `runtime/` alone (a lane handed the
trunk's binary while it repairs the runtime, which is the copy shape the seat itself names) passes
the digest and the mtime check together, and keeping mtime on `runtime/` does not close it. 31 of
the last 30 days' 76 runtime commits touched no `.hero` file (§ 0). The file was restored.

## 5. An edit made while the build runs: mtime passes, the digest refuses (a shape nobody listed)

`heroes-s2wr build selfhost/main.hero -o heroes-race` started in the background; when its first
unit's `.c` appeared under `build/` (13:41:06, the modules already read), one byte of a comment in
`selfhost/cli/digest.hero` changed; the build went on and wrote `heroes-race` at 13:41:45.
`heroes-race --version`: `sources 6710054c3edc42a5`, the tree before the edit; the mirror:
`199f1cbd27d09707`; `ask_guard.py`: **`older_than_tree: None`, `PASSES`**, because the file was
written after the edit it does not contain. The digest refuses it. A session that saves a file
while a build runs (a formatter, an editor, a second agent in the same tree) gets this today, and
the mtime check cannot see it. The file was restored.

## 6. Every platform reachable from here computes the same digest (holds)

The same tree (the writer, the reader and `seed-wr.c`, whose SHA-1 begins `bf521482851f` on this
Mac and on the box after `git apply`), on each platform: stage 1 by the documented line, its
`--version`, the digest the writer computes inside `--emit-c` for the five small programs and for
the whole compiler, `cmp` of that whole emission against the seed, and the Python mirror where a
Python exists. Commands: `scratchpad/222-critic/xfer/run-linux.sh` in `docker run --rm -v
<xfer>:/xfer:ro <image> bash /xfer/run-linux.sh`, and `run-win.sh` over `ssh win` in
`~/panel222-critic` (removed at the end).

| platform, clang | stage 1 `--version` | 5 programs, binary | mirror | whole compiler's emission | `cmp` with the Mac's seed | stage 2 `--version` |
|---|---|---|---|---|---|---|
| this Mac, Apple clang 21.0.0 | `6710054c3edc42a5` | as § 1 | as § 1 | `6710054c3edc42a5` | silent | `6710054c3edc42a5` |
| Linux arm64 container, Debian clang 22.1.8 | `6710054c3edc42a5` | all five equal to the Mac's | equal, the commented use line differing as on the Mac | `6710054c3edc42a5` (`real 84.136`) | **silent** | `6710054c3edc42a5` (`real 29.957`, after the emission warmed the cache) |
| Linux x86-64 container (emulated), Debian clang 22.1.8 | `6710054c3edc42a5` | all five equal | equal, the same one differing | `6710054c3edc42a5` (`real 566.496`, emulated) | **silent** | unrun |
| Windows box, clang 23.1.1 x86_64-pc-windows-msvc | `6710054c3edc42a5`, LF line ends (`od -c`) | all five equal | **unrun: the box has no Python** (`python` is the Store's stub, `py` absent), and installing one is not this seat's to do | `6710054c3edc42a5` (`real 208.638`) | **silent** | `6710054c3edc42a5` (`real 61.597`, after the emission) |

So the writer's digest does not depend on the platform, the clang, `char`'s sign (the key reads
`unsigned char`) or the separator, and the emission stays host-independent with the line in it;
the compiler-engineer's prediction *one `sources` value on all four legs* holds on the three
architectures run, for one commit. **What makes Windows agree is `.gitattributes`**: the box's
`core.autocrlf` is `true` at system level, and a clone of the repository there holds 0 CR bytes
under `selfhost/`, while a clone of a repository without the attributes, same box, holds 2 in a
two-line file. A `.hero` the compiler reaches given `-text` would split the Windows leg's digest
from the others'.

**The mirror on Windows is unrun, and read it would fail**: `tree_digest.py` names a row by
`os.path.relpath(rel, base)`, which on Windows spells `cli\digest` where the compiler spells
`cli/digest` from the use line (`selfhost/modules.hero:116`), and sorts `\` (0x5C) after `/`
(0x2F). An inference from the code; it matters only if the hook's Python runs on Windows, which
the hooks do not today, or if the net's Heroes mirror copies the shape.

**A root spelt with backslashes builds nothing on Windows**: `heroes-s1wr.exe build
'selfhost\main.hero' --emit-c` fails at once (`real 0.113`, exit 1) with `error[unknown_module]`, saying that there is no module `source`, that `use source` reads `selfhost\source.hero` from the directory of `selfhost\main.hero`, and that the file is not there, a file that is there; the small program's root spelt `progs\nocomment\main.hero` the same. The `\` premise of `directory_text` has a recorded history (decisions 469 and 498); whether this message is a defect is a question for that history, not for this panel. No binary,
so no digest to compare; it is a fact about module discovery on Windows, not about (a).

## 7. The suites that read emitted C, run with the writer and the reader (holds)

`./heroes run tests/harness/main.hero -- ./heroes <suite>` from the worktree, `./heroes` the stage
3 of § 2's tree (newer than every source), suites in separate processes beside each other (a red
would be re-run alone; none was red but `emit`):

| suite | passed | failed |
|---|---|---|
| `wholes` | 627 | 0 |
| `descriptors` | 627 | 0 |
| `lldb` | 14 | 0 |
| `lines` | 532 | 0 |
| `emit` | 1 | **12**, each by exactly one added line, the definition (`diff` of every `tests/golden/emit/*.expected` against `./heroes build <case> --emit-c`: 1 added, 0 removed, 0 added lines other than `hero_program_sources`) |

So the readers of the emitted C the compiler-engineer's condition names are green beyond the
re-bless it priced; `emission`, `run`, `warnings` and the compiler's own tests were not run here
(the compiler-engineer ran the last, 1690 passed).

## 8. Stage 2 after a library change: accepted, running yesterday's library (measured)

The library's `read_file` failure text, `selfhost/library_source.hero:217`, changed from `no file at `
to `no file (critic 222) at `; then `heroes-s1wr build selfhost/main.hero -o heroes-lib2` (`real
31.95`) and `heroes-lib2 build ... -o heroes-lib3` (`real 42.17`); `strings <c> | grep -c`:

| compiler | `sources` | old text `no file at ` | new text |
|---|---|---|---|
| `heroes-s1wr`, stage 1 | `6710054c3edc42a5` | 3 | 0 |
| `heroes-lib2`, stage 2 | **`1e8f7b9671fae03a`**, the mirror's digest of the edited tree | **3**: its compiled `read_file` is the seed's | 1: the embedded text only |
| `heroes-lib3`, stage 3 | `1e8f7b9671fae03a` | 2 | 2: compiled and embedded |

So a digest check accepts a stage 2 whose own library functions are the seed's. The compiler-engineer
put *stage 2's own body compiled by a stale stage 1* in (a)'s *lets through* column as *the
fixpoint's job*; this is that class measured, and it is the class `ci.yml`'s own comment gives as
the reason the seed's `cmp` exists (*every job after it runs a compiler carrying yesterday's
library*). The library file was restored.

## 9. Stage 1 can tell its own lag without any mirror (a route nobody listed)

The same edited tree: `heroes-s1wr --version` prints `sources 6710054c3edc42a5`, and
`heroes-s1wr build selfhost/main.hero --emit-c` (`real 54.09`) writes `1e8f7b9671fae03a` into its
emission, the tree's digest computed by the judged compiler's own rule. **Equal if and only if the
compiler is the tree's, by one rule and one reader of the `use` lines**: no second grammar, so § 1's
drift cannot happen, and a stale compiler with an older rule compares its old rule with itself.
Price: one emission, 40 to 54 s on this Mac against the mirror's 1.4 s, through a verb that exists.
That fits the net's start (CE's § 6.1, a run of many minutes) and a stranger's README; it does not fit
a hook on every command, where the mirror can stay as the fast path whose disagreement is settled
by this emission instead of refusing outright.

## 10. Every comment edit now moves an emission (measured; one rule loses its instrument)

`tests/golden/run/closure-list.hero`, one comment line's `# ` made `#  ` (no line moved):
`./heroes build <case> --emit-c` before and after, `diff`: one line, `hero_program_sources`,
`298389c54da88577` to `0b93729667f14ff3`. The trunk seed's compiler (`heroes-s1`) on the same two
texts: `cmp` silent. So under (a) no edit of a program's sources leaves its emission
byte-identical. Two consequences:

- `.claude/rules/verification.md` § The formatter's probe, the author's exemption of 2026-10-12
  (*the probe is not owed where the change to `selfhost/print/` leaves the compiler's emitted C
  byte-identical, which `cmp` ... settles: a comment that moves no line gives it*), would never
  apply again; `grep -rn byte-identical .claude/rules CLAUDE.md` names no other rule resting on it.
  It needs restating as *identical but for the `hero_program_sources` line*.
- A comment-only edit of a golden case's `.hero` (a citation rewritten as a link, as `link` and
  `records/defects-cited` do) now moves its blessed emission under `tests/emission` and turns
  `emission` red until a re-bless.

## 11. The CI legs (read at `f728f420e`, measured where it could be)

Under the policy every leg builds stage 2 and judges with it; under (a) beyond that:

| `ci.yml` | today | must become | measured here |
|---|---|---|---|
| `:537-540` | stage 1 by the documented line, `--version` | then `./heroes-from-seed build selfhost/main.hero -o heroes` | stage 2 cold: Mac `real 73.82` (load about 5); Linux arm64 container 84.1 s of emission then 30.0 s of build; Windows box 208.6 s then 61.6 s. **CI runners unrun** |
| `:548-557`, release tag | `test "$got" = "$want"` on the whole `--version` | the first line | with the reader: whole output **exit 1**, `head -1` exit 0 |
| `:577-582`, Windows plain line | stage 1 builds a module | unchanged: it tests the bootstrap line itself | |
| `:588-726`, doctor, one program, a diagnostic, one module checked, emitted, tested | stage 1 | stage 2 wherever the step judges the tree; stage 1 only where it tests the bootstrap | |
| `:741-779`, own tests, the net, the net's own tests (`HEROES_COMPILER`) | stage 1 | stage 2 | |
| `:819-822`, mutation | stage 1 | stage 2 | |
| `:858-862`, *The seed is what today's source emits* | `cmp` on Linux, red by design under the rhythm between refreshes | **conditional on the seed's own line**: `cmp` against the seed when the seed's `hero_program_sources` equals stage 2's `sources`, else the fixpoint one generation further | `scratchpad/222-critic/ci-seed-step.sh`, run both ways on this Mac: refreshed tree, `cmp` silent, `real 40.470`; a tree lagging its seed by one comment line, stage 2's emission E2, clang on E2, its emission E3, **`cmp E2 E3` silent**, `40.304 + 7.895 + 40.958` s, and the seed differs from E2 |

The conditional step keeps today's guarantee exactly where it is meaningful, at a refresh commit,
and gives the policy its fixpoint on every other push; it is also where a digest rule changed
without a refresh shows (stage 2's `--version`, written by the seed's rule, against E2's line,
written by the tree's): an inference from where each line is written, unrun.

## 12. What a stranger who types the one-line bootstrap sees

At a refresh commit: stage 1's `--version` prints the tree's digest, and `README.md`'s two
commands (*`cmp` printing nothing is the whole proof*) hold (§ 2, § 11). Between refreshes: the
same two lines with the seed's digest, which no file explains, and the README's `cmp` is not
silent (§ 11's lagging run). That is the policy's cost; (a) is what lets the README stay true:
*the seed is this tree's when `./heroes --version`'s `sources` equals the line `./heroes build
selfhost/main.hero --emit-c` writes* (§ 9, 54 s, no mirror, no new word). `README.md`,
`seed/README.md:9` and `.github/ISSUE_TEMPLATE/defect.yml:62` print or ask for `--version` and
say nothing of a second line.

## 13. The critic's answer

**Falsified, each by a run above:**

1. *The premise it rests on ... is checked by the run* (the mirror's): a canonical `use sub/helper
   # the helper` is followed by the compiler and missed by the mirror's regex, so the two digests
   differ on the Mac and both Linux containers alike, and the mirror printed `missing: []` (§ 1).
2. *(a) refuses ... a copy from another tree*: not when the trees differ in `runtime/` alone. A copy
   made from before a runtime change passes the digest and the mtime guard together, and *mtime
   stays on `runtime/`* does not close it (§ 4); 31 of the last 30 days' 76 runtime commits touched
   no `.hero`.
3. *So (a) lands in two commits with a refresh between*: one commit carrying the writer, the reader
   and a seed emitted by a writer-only scaffold builds from its own seed and is its own fixpoint
   (§ 2). What binds is an invariant, and it binds every lane's seed merge too.
4. Not a seat's claim but a premise of a rule: under (a) a comment-only edit no longer leaves the
   emitted C byte-identical, so the probe's exemption of 2026-10-12 empties (§ 10).

**Held, each by a run above:** one digest for one tree on this Mac, Linux arm64, Linux x86-64
and the Windows box, clang 21, 22 and 23, with the emission host-independent and equal to the seed
by `cmp` on all three non-Mac platforms (§ 6); CRLF, no final newline, UTF-8 and every spelling of
the root (§ 1); `.gitattributes` keeping the Windows checkout LF under `core.autocrlf=true` (§ 6);
the warm cache refreshing the digest on a comment-only edit (§ 3); the refusal of stage 1 on a
lagging seed (§ 9) and of a binary written during an edit, which mtime passes (§ 5);
`wholes` 627/0, `descriptors` 627/0, `lldb` 14/0, `lines` 532/0, and `emit` red by exactly the one
line in each of its 12 cases (§ 7); the release-tag step red on two lines and green on the first
(§ 11).

**Conditions this seat adds to the compiler-engineer's seven:**

1. **The invariant, checked by an instrument**: no commit holds the reader while `seed/heroes.c`
   does not define `hero_program_sources` (`grep -c` gives 1 or 0); the landing may be one commit
   with the scaffolded seed or two; `.claude/rules/records.md`'s *take one side* names the side, the
   seed that defines it.
2. **The coverage is decided at the landing**, since the line is written by the building compiler
   and a later change of the rule needs a seed refresh in its own commit (an inference from § 2, §
   5 and § 8, unrun as a rule change). The robust choice: the digest also covers the runtime the
   build links (33 tracked `.c` and `.h`, 688,179 bytes, about 0.12 s more in the mirror by
   proportion) and the library text the binary was compiled with, so § 4's copy and § 8's stage 2
   are refused; the conservative one, if the author prefers it: the record names both as not
   covered, with the instrument that covers each (none for the copy).
3. **No second grammar is the authority**: the judged compiler's own emission of the tree (§ 9) is
   the arbiter at the net's start, in the CI and in the README; a hook's mirror is a fast path whose
   disagreement asks that emission rather than refusing, and whose reading of `use` lines is held
   to `selfhost/module/paths.hero`'s by a test over the same texts, the trailing comment first.
4. **The CI as § 11's table**, the conditional seed step on every push, not on tags.
5. **`.claude/rules/verification.md`'s probe exemption restated** as identical but for the
   `hero_program_sources` line, in the landing's commit.
6. **Every refusal names both digests, the two-stage line, and that a mirror can be the one wrong.**
7. **`seed/README.md`, `README.md` and `defect.yml` say what `sources` is** and give § 9's
   self-check.

**Unrun here:** the Python mirror on Windows (no Python on the box); a stage 2 on the x86-64
container (its emission only); any CI runner; a whole build before and after on a still machine;
the suites `emission`, `run`, `warnings`, `determinism`; a lane merge; a change of the digest's
rule. **No veto from this seat** (it holds none); route (a) stands as built, its claims narrowed as
above.

The worktree was removed at the end (`git worktree remove --force`), and the Windows folder
`~/panel222-critic` with it (`ls` answered *No such file or directory*). Every file restored in
the worktree was restored byte for byte before the next measurement (`git diff --stat` empty for
each).
