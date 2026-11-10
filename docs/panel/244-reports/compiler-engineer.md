# Panel 244, the compiler-engineer's report

Written as it went, 2026-11-10, by the compiler-engineer seat. Every number
below comes from a command run while writing it, named beside it. Probes are
in `.claude/worktrees/p244-scratch/compiler-engineer/` (`probes/c*`), raw
outputs in `part*.txt` beside them.

## 0. The compiler judged with

- Worktree `.claude/worktrees/244-compiler-engineer`, detached at `89db84347`.
- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (real 34.27,
  user 20.11), then `./heroes build selfhost/main.hero -o heroes-next` (real
  300.18, user 160.80, sys 31.61: the machine was waiting, load 46 to 63 read
  by `uptime` during it, so no duration here is a measurement).
- `./heroes-next --version`: `heroes 0.6.0` / `sources 14e9e5ce168a2336`, the
  brief's base. Every verdict below is that binary's, on `89db84347`'s
  messages.
- The index every probe reads is the worktree's own `packages/`, written
  `index = "file://../../../../244-compiler-engineer/packages"` from a probe
  folder; `heroes fetch` placed it as `heroes.packages/core@0.2.1-7c9ea54ea66fbc80`
  (`ls c02/heroes.packages`).

## 1. The rules the brief lists, run (part 1: `internal`, `use <core>`)

Command for every row: `cd probes/<c> && heroes-next check main.hero`
(output in `part1-check.txt`, `part1-fetched.txt`).

| probe | shape | printed | exit |
|---|---|---|---|
| c04a | `main` writes `use lib/internal/strs` | `error[private_module]: `lib/internal/strs` is private to `lib`: a module under an `internal` folder is named only by the module that folder belongs to and by the modules under it; name what `lib` offers instead` | 1 |
| c04b | `main` uses `lib`, `lib.hero` uses `lib/internal/strs` | nothing | 0 |
| c04c | `lib/other.hero` (under the owner) uses `lib/internal/strs` | nothing | 0 |
| c04d | `libx.hero` (the owner's name as a prefix) uses `lib/internal/strs` | `private_module`, at `libx.hero:1:5` | 1 |
| c05 | root `internal/x.hero`, from `main` and from `other/o.hero` | nothing | 0 |
| c06 | a FILE `lib/internal.hero`, `use lib/internal` from `main` | `private_module`, "a module under an `internal` folder", though it is a file under no such folder | 1 |
| c06b | the same file named from its owner `lib.hero` | nothing | 0 |
| c06c | a root FILE `internal.hero`, `use internal` | nothing | 0 |
| c06d | control, `a/internals/b` | nothing | 0 |
| c03 | `use <core>/strconv/internal/shown` from a program (core placed) | `private_module`: private to `<core>/strconv` | 1 |
| c03 | the same, before `heroes fetch` | `package_unread` AND `private_module`, two errors for the line | 1 |
| c01 | `use <core>` alone, catalogue present, core placed | `error[package_wants_a_module]: a package is reached through one of its modules — `use <core>/parse`, `parse` being a file of the package `core`` | 1 |
| c01n | `use <core>` alone, no catalogue anywhere | the same, word for word | 1 |
| c02 | `use <core>/nothere`, core placed | `error[unknown_package_module]: the package `core` has no module `nothere`: `use <core>/nothere` reads `heroes.packages/core@0.2.1-7c9ea54ea66fbc80/nothere.hero`, and that file is not there` | 1 |
| c02 | the same, before `heroes fetch` | `package_unread`: "in the catalogue and not placed: `heroes fetch` places it at ..." | 1 |

**What the compiler holds** (`selfhost/placed_use.hero:148`, `internal_owner`;
`selfhost/module/package_told.hero:259`, `private_reaches`): the owner is the
text before the LAST `/internal` that is followed by `/` or the end. So the rule
is Go's element rule, a path part `internal` after the first, for files as well
as folders (c06), and a root `internal/` or `internal.hero` has owner `""` and
is private to nobody (c05, c06c). For a program that coincides with Go's
(Go's root `internal/` is the whole module's, and nothing outside the program
can name a program's module), so c05 is not a defect; it is a rule nobody
reading the spec can predict, since the spec says nothing of `internal`
(`grep -c -i internal spec/heroes-spec.md` read 0 on the worktree).

**Can a spec reader predict it?** No row of the table above: `internal` is in
no sentence of the spec. **Does the message carry the rule?** Yes for c04a,
c04d, c03: it names the owner, says who may name the module, and says what to
do. Not quite for c06: "under an `internal` folder" is false of a file, so the
message describes a narrower rule than the one applied (the critic's `p6`,
reproduced here; filed since as defect 2439, `adjacent`, and repaired in the
round, § 9). `use <core>` alone: the
grammar already states it (`Use = "use" [ "<" ident ">" "/" ] ident ...`, the
spec's line 58 production), the message carries the rule, but offers a module
`parse` that core 0.2.1 does not have (`ls heroes.packages/core@*/` holds
`strconv`, `strings`, `encoding`): that invented `parse` is what lane b30-pkg's
defect 2438 is moving (its uncommitted
`fixedbugs-2438-a-package-named-with-no-module-is-offered-its-modules.case/`
in `git status` of `.claude/worktrees/b30-pkg`, committed in the round since,
§ 9), so it is not mine to file (`gh issue view 2438`: "Defect 2438: `use <core>` alone is told `package_wants_a_module` with `use <core>/parse` offered ...", OPEN, class:blocking).

## 2. A placed tree with one byte changed (`probes/c07`, `part2-*.txt`)

`heroes fetch` placed core, `run main.hero` printed `42`, exit 0. Then ONE byte
of a comment of `heroes.packages/core@0.2.1-7c9ea54ea66fbc80/strconv.hero` was
changed (`cmp -l` printed `3 116 130`, an `N` become `X`; the file still
parses):

| verb | printed | exit |
|---|---|---|
| `check` | `error[package_unread]: `<core>` cannot be read: the tree placed at `heroes.packages/core@0.2.1-7c9ea54ea66fbc80/` is `sha256-b27c5c15...`, and the catalogue's `sum` for it is `sha256-6cf4554f...`: a file of it was changed, added or taken out since it was placed — `heroes fetch` places it again` | 1 |
| `build` | the same | 1 |
| `run` | the same | 1 |
| `test` | the same | 1 |
| `parse` | nothing (`parse` reads no package) | 0 |
| `fetch` | `placed core 0.2.1 ...`; `cmp` of the original and the re-placed `strconv.hero` printed nothing | 0 |

**Two shapes beside it.** A `.DS_Store` written into the placed tree's root,
and then only into `strings/internal/`, refused `check` the same way (sums
`sha256-0cbd51a6...` and `sha256-0ea21fcd...`), and `heroes fetch` removed it
(`ls -a` of the tree after it holds no `.DS_Store`), after which `check` exit 0.
The cause is `selfhost/module/fingerprint.hero`'s `listing`: every regular file
under the root is hashed, Go's `h1`. The message is true and its repair works,
but it cannot say WHICH file was added, since the compiler holds the sum and not
the listing. That Finder writes `.DS_Store` into a folder it opens is a platform
fact I did not run (I wrote the file by hand); see § 7, defect candidate D1.

**Can a spec reader predict it?** Partly: § 1 says the catalogue names the
package "with its source, exact version and fingerprint", and nothing says the
tree is held to it at every build, nor that `heroes fetch` exists (`grep -c -i
'heroes fetch' spec/heroes-spec.md` read 0). **Does the message carry the
rule?** Yes: it says what was compared, that a file changed, and which verb
repairs it.

## 3. `heroes add` and `heroes fetch` in their three forms (`probes/c30` to `c33`, `part3-*.txt`)

| form | command (in the program's folder) | wrote | `run` |
|---|---|---|---|
| index over `file://` | `heroes add core --index file://../../../../244-compiler-engineer/packages` | `added core 0.2.1` / `wrote heroes.toml`; the `index` line, `version`, `sum`; placed `core@0.2.1-7c9ea54ea66fbc80` | `42`, 0 |
| git over a `file://` repository | `heroes add file://<abs>/gitpkg --tag v1.0.0` | `added gitpkg v1.0.0`; `git`, `tag`, `commit = "d2a2853d..."`, `sum`; placed `gitpkg@v1.0.0-b6e6db7cbcb4bbb8` | `2`, 0 |
| folder | `heroes add --path ../mypkg` | `added mypkg ../mypkg`; `path = "../mypkg"` ALONE, no version, no sum; nothing placed | `note: the package `mypkg` is read from the folder `../mypkg/`, a `path` entry of the catalogue: unverified, no fingerprint holds it`, then `2`, 0 |

Then each program's `heroes.packages/` taken away:

- index and git: `check` exit 1, `package_unread`, "in the catalogue and not
  placed: `heroes fetch` places it at `heroes.packages/<name>@<v>-<k>/`";
  `heroes fetch` printed `placed ...`, exit 0; `heroes fetch -C <dir>` from the
  parent printed `kept ...`, exit 0; `run` then printed the same answer.
- path: `check` exit 0 with the `unverified` note; `heroes fetch` printed
  `path mypkg ../mypkg, read where it stands and unverified`, exit 0.

Beside them: a git address with no `--tag` is `error[bad_usage]: a git source
is added at a tag: `heroes add <address> --tag v1.0.0``, exit 2 (c33). The tag
moved to another commit after the add (`git tag -f`), the placed tree still
there: `check` exit 0 (the tree still matches its sum, so nothing is wrong
with what is built); tree removed, `heroes fetch` printed
`heroes.toml:3: error[tag_moved]: the tag `v1.0.0` of `file://...` names the
commit 453dcedf..., and the entry `gitpkg` was written when it named d2a2853d...:
a tag that moved is refused, never followed, and `heroes add` writes the entry
again once the new commit is the one meant`, exit 1.

**Can a spec reader predict them?** `heroes add json` is the one verb § 1
names (`grep -c -i 'heroes add'` read 1). The `--tag` and `--path` forms, the
`heroes.packages/` folder and `heroes fetch` are in no sentence (`grep -c -i
'heroes.packages'` 0, `'heroes fetch'` 0). **§ 1's sentence is false for the
third row**: it says the catalogue names a package "with its source, exact
version and fingerprint", and a `path` entry has neither of the last two, by
design (the note says so). **Do the messages carry the rules?** Every refusal
above names its repair command; `heroes help` (`table.hero:183-184`'s
summaries) names all three forms and says `build, check and run never fetch`.

## 4. The critic's shapes, run again in my own folder (`probes/c08` to `c22`, `part4-*.txt`)

`heroes-next check main.hero` in each folder unless named.

| probe (critic's) | shape | printed (head) | exit | the message carries the rule? |
|---|---|---|---|---|
| c08 (p8) | entry right, nothing placed, `build` and `run` | `package_unread`, "in the catalogue and not placed: `heroes fetch` places it at ..."; `ls` afterwards shows no `heroes.packages/` | 1, 1 | yes: they never fetch |
| c09 (p9) | `version = 0.2.1` unquoted | `catalogue_refused: ... heroes.toml:4: a value is a string between double quotes: `version = "1.4.2"`` | 1 | yes |
| c10 (p10) | `sum` one digit wrong, the right tree placed | `package_unread`, "not placed: `heroes fetch` places it at `heroes.packages/core@0.2.1-257d0609f146ec16/`" (another key: the key hashes the sum); `heroes fetch` then: `heroes.toml:5: error[sum_mismatch]: the listing of core 0.2.1 at the index ... sums to `sha256-6cf4...`, and the catalogue's `sum` for it is `sha256-7cf4...`` | 1, 1 | no: the first message points at `fetch`, the rule is told by the second verb |
| c11 (p11) | `version = "0.2.0"` with 0.2.1's sum | the same two steps, key `core@0.2.0-11b72902102c65dd`, then `sum_mismatch` naming 0.2.0's sum | 1, 1 | no, as c10 |
| c12b (p12b) | `path` and `version` | `catalogue_refused: ... the entry `mypkg` names one source: a `version` from the index, a `git` address with its `tag` and `commit`, or a local `path`` | 1 | yes |
| c12d (new) | `path` and `sum` | `catalogue_refused: ... is a local `path`, which holds no `sum`, `tag` or `commit`: a folder of the author's own is read as it stands, and told as unverified` | 1 | yes |
| c12c (p12c) | `path` to a missing folder | `package_unread: ... the folder `../mypkgx/` its `path` names is not there` | 1 | yes |
| c13 (p13) | `[dependencies.match]`, `use <match>/strconv` | `error[package_wants_a_name]: expected the package's name after `<`, found `match`` at the `<` | 1 | half: told at the `use`, not at the entry; lane b30-pkg's d7ff76523 (defect 2432, "an entry no use can write is refused where it is named") moves it |
| c14 (p14) | entries `core` and `core2`, both index 0.2.1 | `check`: both not placed; `heroes fetch`: `placed core 0.2.1 ...` then `heroes.toml:7: error[absent]: the index ... serves no core2 0.2.1` | 1 | yes: an index entry's name IS the index's name |
| c14b (new) | `heroes add core --index ... --as core2` | `error[bad_usage]: a package from the index keeps the index's name and has no tag` | 2 | yes |
| c14c (p14c) | two `path` entries, one folder | `catalogue_refused: ... the entries `mypkg` and `mp2` name one package, and a program holds one version of each package under one name` | 1 | yes |
| c15 (p15) | `use core/strconv`, no brackets | `error[package_unmarked]: ... write `use <core>/strconv``, `fix (certain)` | 1 | yes |
| c16 (p16) | `use heroes.packages/core` | `package_unmarked: `heroes.packages/core/` is the folder a fetch placed a package's tree in, which a `use` never names ... `use <core>/parse`` (the invented `parse` again, defect 2438's family) | 1 | yes, but for the module offered |
| c17 (p17) | `use <json>/parse`, no catalogue | `error[unknown_entry]: `<json>` names no entry: there is no catalogue `heroes.toml` beside `main.hero` ... `heroes add json` writes one` | 1 | yes |
| c19 (p19) | a `[package]` table | `... heroes.toml:1: a table is `[dependencies.<name>]`, one per entry, found `[package]`; the catalogue holds the program's dependencies and nothing else` | 1 | yes (and "nothing else" omits the `index` line it accepts) |
| c20 (p20) | `[dependencies]` / `core = "0.2.1"` | the same, `found `[dependencies]`` | 1 | yes |
| c21 (p21) | `version = "^0.2"` | `... has the `version` `^0.2`, and a version is exact, three numbers with no leading zero: `1.4.2`` | 1 | yes |
| c22 (new) | no `index` line | `package_unread`, not placed, key `core@0.2.1-edd6313d5521e3f8` (the default index's); `heroes fetch` NOT run, it would reach `https://index.heroes-lang.org` | 1 | yes |

**The critic's item 11 holds as they wrote it**, on my binary: a sum or version
mistyped by one character is told *not placed* and `heroes fetch` is what tells
`sum_mismatch`. The cause is `module/placed.hero`'s key, which hashes the entry,
the sum included, so the mistyped entry looks for a folder nobody placed.

**The critic's `p18`, on my own hand-made index** (`probes/idx/cpkg`, its
`list` and `versions` sums equal to the critic's byte for byte: `1.0.0
sha256-180127d3...`): `heroes add cpkg --index file://../idx` then `heroes run
main.hero` printed `error[ffi_constant_type]: `HALF` in `c.h` cannot be read
without running its header's code, which a fetched package's header never does
at build: it is a float, which no enumerator holds`, at
`heroes.packages/cpkg@1.0.0-a2554f96d3f86ca5/lib.hero:2:5`, with a note citing
panel 235's R9 C5, exit 1; the same two files in the program's own tree
(`probes/own18`) printed `2` and `0.5`, exit 0 (`part11-c18.txt`). The message
carries its rule (it names C5 and what a package's header may hold); spec § 13
does not let a reader predict it. The ffi-pragmatist's ground; reproduced only.

## 5. The additions the brief asked for (`probes/c06*`, `c12*`, `c14*`, `c42*`, `c44*`, `git/`, `part5-*.txt`, `part6-*.txt`)

- **`internal` as an element**: c06 (a file `lib/internal.hero`) refused, c06c
  (a root `internal.hero`) and c05 (a root `internal/`) accepted; § 1 above.
- **A `path` entry without and with a version**: c32 (`add --path`, no version,
  runs with the `unverified` note), c12b (with `version`, refused), c12d (with
  `sum`, refused). § 1's sentence ("source, exact version and fingerprint") is
  false for the accepted one.
- **Two entries naming one package**: index (c14, impossible by construction,
  an index entry's name is the index's; `--as` refused, c14b), path (c14c,
  refused, names both entries). Git: not run as two hand entries; the R3 walk
  below shows the verb never writes two.
- **R3's two aliases for two packages, run** (the critic left it unrun). Four
  throwaway git repositories under `probes/git/`: `u1` and `u2`, two different
  packages each with `u.hero`'s `which()` answering 1 and 2; `pa` requiring `u1`
  as `util`, `pb` requiring `u2` as `util` (their catalogues written by `heroes
  add ... --as util` from a folder outside them, since inside `pa`'s own
  repository a `file://` source outside it is `source_refused`, R9's C4, which
  I ran first). In `c44`, `heroes add <pa> --tag v1.0.0` printed `added pa
  v1.0.0` / `added util v1.0.0 for pa`, and `heroes add <pb>` printed `added pb
  v1.0.0` / `added util_2 v1.0.0 for pb`; the catalogue holds `pa`, `util`
  (u1), `pb`, `util_2` (u2). `run` of `a.a() + b.b()` printed **`21`**, exit 0:
  each package's `<util>` reached its own. Beside it: the program naming the
  transitive entries itself, `use <util>/u` and `use <util_2>/u as u2` (c44b),
  printed `2101`, exit 0, so a program may write a name `heroes add` invented
  (`util_2`) and never shown it but on its output line; the program's own
  `util` naming u2 BEFORE `pa` is added (c44c) gave `added util_2 v1.0.0 for
  pa` and `201`, exit 0. All right; none is predictable from the spec, whose
  words are "a program holds one version of each package, every one listed in
  its catalogue".
- **A package's own catalogue holding a `path`** (c42a, c42b, c42c): the verb
  and the compiler disagree.

  | probe | catalogue | verb / printed | exit |
  |---|---|---|---|
  | c42b | `heroes add <gp> --tag v1.0.0`, gp's own catalogue holds `path = "../../mypkg"` | `error[requirement_refused]: the catalogue of `gp` names the local folder `../../mypkg` for `mypkg`, and only the program's own catalogue holds a `path`, while a package is developed`; no `heroes.toml` written, and `heroes.packages/gp@v1.0.0-52fd1feec15c53b8` LEFT placed | 1 |
  | c42a | `heroes add --path ../git/pp`, pp's own catalogue holds `path = "../../mypkg"` (which exists) | the same `requirement_refused` | 1 |
  | c42a | hand-written `[dependencies.pp] path = "../git/pp"` alone, `check` | `package_unread: `<pp>` cannot be read: its catalogue names `mypkg`, the folder `../../mypkg`, and the program's catalogue lists no entry for that package: the program's catalogue holds every package of the build — `heroes add` writes it whole` | 1 |
  | c42a | the same, `heroes fetch` | `path pp ../git/pp, read where it stands and unverified` | **0** |
  | c42c | hand-written `pp` AND `mypkg` by `path`, `check`, `run` | two `unverified` notes, then `2` | **0** |
  | c42c | then `heroes add --path ../mypkg` (any add) | the same `requirement_refused` | 1 |

  So the compiler (`selfhost/module/packages.hero:171-183`, `mapped`, matching
  a package's own entry to the program's by `catalogue.source_of`, a `path`
  included) builds what the verb (`selfhost/fetch/build_list.hero:99-104`,
  `required_id`, refusing every `path` in a package's catalogue) refuses to
  write; the compiler's message names `heroes add` as the repair, which refuses;
  `heroes fetch` says exit 0 over a catalogue `check` refuses; and once the
  hand-written catalogue builds, `heroes add --path ../mypkg` in that program
  was refused (one run; that any add would be is an inference from the walk,
  which visits every entry, `build_list.hero`'s `chosen`). Defect candidate D2, § 7. It matters to Q-stub (§ 6): two packages
  developed side by side, one using the other, have no route the verb accepts.

## 6. Q-stub: the verb, priced as a prototype built and run

**Shape prototyped** (the name is one string in the table; `make` costs the
same lines): `heroes new program <name>` and `heroes new package <name>`, with
`-C <folder>` as `add` and `fetch` have it, writing into a NEW folder `<name>`
of the current one (or of `-C`'s). The diff is kept at
`.claude/worktrees/p244-scratch/compiler-engineer/qstub/qstub.diff` (431
lines); it was never committed.

**Where it lives** (each file read before editing; counts by `git diff --stat`
and by an awk mirror of `tests/harness/suite_layout.hero:1028`'s `code_lines`,
every non-blank line outside a `test` block, comments included):

| file | what changed | `git diff --stat` | code_lines before → after |
|---|---|---|---|
| `selfhost/cli/stub.hero` (new) | the verb: `planned` (the files or the refusal), `folder_name`, `joined`, `written`, the four texts, two tests | +209 −0 | 0 → 150 |
| `selfhost/cli/table.hero` (296 lines before, `wc -l`) | `Operand.stub_operand`, `Tag.new_cmd`, `new_flags` (`-C`), the `Command` row, the pinned count `15 → 16` with its comment | +10 −1 | 229 → 237 |
| `selfhost/cli/argv.hero` (522 before) | the second positional word, the missing-word refusal, the operand's usage and empty-word texts, two exhaustive arms, the two tests that pin the command list (`argv.hero:375`, `:447`) | +29 −7 | **284 → 303** |
| `selfhost/main.hero` (323 before) | `use cli/stub`, the dispatch arm, two exhaustive arms | +4 −2 | 289 → 291 |
| `selfhost/cli/help.hero` | the operand's usage text | +1 −0 | 109 → 110 |
| `selfhost/cli/unverified.hero` | one exhaustive arm | +1 −1 | 45 → 45 |
| **total** | 6 files, 1 new | **+254 −11** (`git diff --numstat`) | **+180** |

Five exhaustive matches on `Tag` (`main.hero` three times, `argv.hero:122`,
`unverified.hero:52`) and six on `Operand` (`argv.hero` five times, one of
them a test, and `help.hero:58`), counted by `grep -c` of `.fetch_cmd` and
`.package_operand` in each file at `HEAD`, refuse to compile without the new arms, which is the table's
design (`table.hero:1-9`, panel 016): the checker found every one, no grep was
needed.

**The ceiling is breached by the operand, not by the verb.** `argv.hero`
stands at 303 code lines against `CEILING` 300 (`suite_layout.hero:48`) and has
no `DECIDED` row (`grep -n argv tests/harness/suite_layout.hero` printed
nothing). The cause is the two-word operand: `Invocation` holds one operand,
`file: str?` (`argv.hero:32-36`), so the prototype carries the name in
`program_args`, which only `run` fills; the clean form is a field of its own,
and `Invocation(` is constructed at **26 sites in 9 files** (`grep -rn
'Invocation(' selfhost | wc -l`), each owing the new field. So the production
shape costs either that field (26 sites) plus moving `operand_said` and
`empty_operand`, pure functions of the operand, out of `argv.hero` to get under
300, or a `DECIDED` row, which is a decision. A one-word operand (`heroes new
<name>` and `heroes new <name> --package`, or the kind as a flag) costs none
of that: the argv parser already takes one operand. Measured as an inference
from the mirror; § 6's run of the `layout` suite narrowed to the file is below.

**What moves with the count of verbs** (each found by a command run here):

- `selfhost/cli/table.hero:263`, `assert table.len() == 15`, moved in the
  prototype to 16.
- `selfhost/cli/argv.hero:375` and `:447`, two tests that pin the whole list
  `lex, parse, ..., grammar, this` (`grep -n 'the commands are'`); my first
  build would have failed both, and the second edit adds `new`.
- `site/src/lib/claims.ts:265`'s `verbs()` counts the `Command(tag:` rows of
  the table and holds `site/src/html/start.html:121` (`One command, fifteen
  verbs`) and `site/src/html/it/start.html:124` (`Un comando, quindici verbi`)
  to the count (`:536-540`), and `NAME_EVERY_VERB` (`:594`) requires both pages
  to name `<code>new</code>`: two pages, two edits each, and the site's build
  before the push, which publishes (CLAUDE.md § Hard stops).
- `docs/design.md` §3.5 (`:900`) names `heroes add`/`heroes fetch` as the
  verbs to come; `.claude/rules/cli-surface.md` records how `heroes probe`
  entered (by the author's decision, 2026-09-26) and would record this one.
- The CLI goldens: `tests/golden/fetch/` holds 19 cases (`ls | wc -l`) of `add`
  and `fetch`, run from the repository's root with `-C <work>/app`
  (`tests/harness/suite_fetch.hero:197`); a `new` case would be one of that
  shape, which is why the prototype takes `-C` (without it, a case would write
  into the root). `suite_fetch.hero`'s `LEAST` floor (19) moves with each case.
- The spec names no verb but `heroes add` (`grep -n 'heroes probe\|heroes
  doctor\|heroes grammar' spec/heroes-spec.md` printed nothing), so the count
  moves nothing there.

**Built and run.** `./heroes-next build selfhost/main.hero -o heroes-q` wrote
`heroes 0.6.0` / `sources 13416e071b39c609` (the edited tree; a first build of
an earlier edit was stopped by me before it finished, once the second edit had
superseded it). `heroes-q check selfhost/main.hero` exit 0. The runs, by
`qstub/qrun.sh` in a fresh `qstub/w/` (whole transcript `part8-qstub.txt`):

| command | printed | exit |
|---|---|---|
| `heroes help` | `  new program|package <name>` and the summary row | 0 |
| `heroes new program hello` | `wrote hello/main.hero` / `next: heroes run hello/main.hero`; one file: `main()`, a `greeting()` and one `test` | 0 |
| `heroes run hello/main.hero` | `hello` | 0 |
| `heroes test hello/main.hero` | `1 test, all passed` | 0 |
| `heroes new package greet` | `wrote greet/greet.hero`, `wrote greet/greet/internal/words.hero`, `wrote greet/greet.md`, then `next: heroes test greet/greet.hero; a program beside it reaches it by heroes add --path ../greet` | 0 |
| `heroes test greet/greet.hero`, and `heroes test greet.hero` inside `greet/` | `2 tests, all passed` (the face's and the private module's) | 0, 0 |
| `heroes fmt <file> \| cmp - <file>` on the three `.hero` written | nothing: each canonical | 0 |
| `heroes new program app`, `main.hero` rewritten to `use <greet>/greet`, then in `app/` `heroes add --path ../greet` | `added greet ../greet` / `wrote heroes.toml`: `[dependencies.greet]` / `path = "../greet"` | 0 |
| `heroes run main.hero` in `app/` | the `unverified` note, `hello, ana` | 0 |
| `heroes test main.hero` in `app/` | the note, `2 tests, all passed` (the package's, reached through the program) | 0 |
| `heroes new program inner -C deep` | `wrote deep/inner/main.hero` | 0 |

**What it refuses**, each exit 2 with nothing written (`find . -maxdepth 1`
afterwards lists only what the accepted commands wrote):

| command | printed |
|---|---|
| `new program hello` (a folder there) | `error[bad_usage]: `hello` is already there, and `heroes new` writes only into a folder it makes: choose another name, or move `hello` away` |
| `new program afile` (a FILE there) | the same, for `afile` |
| `new package Greet` (with `greet` there) | the same: **this volume ignores case**, so the refusal is APFS's answer and not the verb's rule; on a case-sensitive volume `Greet` would be written beside `greet` (unrun: no such volume here) |
| `new package match`, `new package fn` (a keyword, a foreign word) | `error[bad_usage]: `match` is no package's name: a program reaches it as `use <match>/match`, and a name there is letters, digits and `_`, not opening with a digit, and no word of the language` (the parser's own rule, `module_text.is_use_path`) |
| `new package my-lib`, `new package 2d` | the same text |
| `new program a/b`, `.`, `..` | `error[bad_usage]: `a/b` is no folder's name: `heroes new` makes one folder of the current one, named with no `/`, `\` or control character` |
| `new program x -C nothere` | `error[bad_usage]: `-C nothere` names no folder on this machine` |
| `new app foo` | `error[bad_usage]: `heroes new` writes a `program` or a `package`, and was asked for `app`` |
| `new program ""` | `error: `new` was given an empty word, which names nothing: ...` |
| `new`, `new program` | `error: `new` needs what it writes and its name: `heroes new program <name>` or `heroes new package <name>`` |
| `new program --x` | `error: `new` does not accept `--x` — it accepts -C` |
| `new program a b` | `error: `new` takes one file, and two were given`: **false words** (argv's generic text; the prototype owes this verb its own, about two lines) |

A program's name is not held to the `use` rule (no `use` ever names a
program's folder) and is in no line of `main.hero`, so a name no string or
comment of Heroes can hold (a `"`, a newline) cannot make the stub fail to
parse; a package's name IS in its files, and the `use` rule makes it safe to
write there. The `layout` suite narrowed to the file, `./heroes-q run
tests/harness/main.hero -- ./heroes-q layout argv`, printed `layout (only
argv): 3 passed, 1 failed` / `selfhost/cli/argv.hero: 303 lines of code, past
§11's 300` (`part9-layout.txt`): the ceiling reading above is the instrument's,
not only my mirror's.

**The compiler's own tests the prototype touches**, each `./heroes-q test
selfhost/main.hero --only "<title>"` (never the whole run; `part10-tests.txt`):
the stub's two tests, `table.hero`'s (*the table is the pipeline's order ...*,
which pins 16), `argv.hero`'s *strict against the table ...* and *an argument
that is not UTF-8 ...* (the two that pin the command list, `new` added), *an
empty operand is refused ...* (every operand-taking verb, `new` among them,
refuses an empty word) and `help.hero`'s *the help is the table spoken ...*:
**7 runs, each `1 test, all passed`, exit 0.** The compiler's whole test run,
the net and the site's build were not run (the brief's bounds), so what they
would say of the prototype is unrun.

**Three things the prototype found that the sitting must rule, all measured:**

1. **The card.** `examples/packages/README.md` § 8.2 says *a card is written
   from its face module by a program of the net, never by hand* (task 2294),
   and that program is no verb. So a stub's `<name>.md` is a hand-written card
   from birth, which drifts the first time the face changes. Either the verb
   writes no card (the README's § 8 binds the core packages; a user's package
   is held by nothing today), or the card's writer becomes part of the verb,
   which is a second capability priced apart.
2. **Developing two packages together has no route the verb accepts** (D2,
   § 5 and § 7): `new package a`, `new package b`, `a` using `b` by `path`
   while both are developed, is refused by `heroes add` and built by the
   compiler. The stub's own development route, `heroes add --path`, stops at the
   second package.
3. **The two-word operand is the expensive part.** Of the 180 code lines, 19
   are `argv.hero`'s and they breach its ceiling; the production form adds a
   field to `Invocation` at 26 sites. `heroes new <name>` for a program and
   `heroes new <name> --package`, or the kind as a flag, leaves `argv.hero`
   untouched but for the exhaustive arms. The project's vocabulary
   (`program`/`package`, never the author's `app`/`lib`, spec § 13's *library*
   being a C library) holds in either form.

**Not in the prototype, owed by a production lane** (each a count, not run):
the verb's own text for a second name; writing into a temporary folder beside
the target and renaming it (`process.rename_over` exists) so a failure halfway
leaves no partial folder that a re-run then refuses as *already there*; golden
cases in `tests/golden/fetch/`'s shape, one per row of the two tables above
(about 20), and its `LEAST` floor raised; the site's two start pages; design.md
§3.5 and `.claude/rules/cli-surface.md`, which record how a verb entered.

## 7. Rules the compiler holds that `00-shared.md` does not list, and what I found and did not repair

**Rules the compiler holds, run here, that the shared brief does not name**
(each a row of §§ 1 to 5 above):

1. A package's catalogue is the `heroes.toml` at its tree's root
   (`fetch/placing.hero`, `own_catalogue`), and a program's catalogue holds its
   dependencies' dependencies too, under a name the verb invents where one is
   taken (`util_2`, c44), which the program may then `use` itself (c44b,
   `2101`).
2. Inside a repository, a `file://` source outside it is `source_refused`
   (R9's C4): a package developed in its own git repository cannot `heroes add`
   a sibling repository by `file://` (part5-r3-setup).
3. A git address needs `--tag` (`bad_usage`, c33); a tag that moved is
   `tag_moved` at `heroes fetch` while the tree already placed still builds
   (c31).
4. An index entry keeps the index's name; `--as` is refused for it (c14b).
5. Every regular file under a placed root is in its sum, a file the operating
   system writes included, and `heroes fetch` removes what the listing does not
   hold (c07).
6. The placed key hashes the entry, its sum included, so a mistyped sum looks
   for another folder (c10; defect 2440, filed, open).
7. `heroes parse` reads no package: a tampered tree parses at exit 0 (c07).
8. A catalogue with no `index` line reads the default index; its key is
   computed without asking it (c22).
9. A refused `heroes add` leaves the tree it placed (c42b); pinned by
   `tests/golden/fetch/add-a-package-whose-catalogue-names-a-folder.case/expected`
   and filed as defect 2434 (open).

**Defect candidates I found and did not repair or file** (no GitHub write is
mine; each searched first, `gh issue list --state all --search ...` with the
words given, nothing matching):

- **D1: a placed tree is refused for a file the operating system adds, and the
  message cannot say which.** Reproducer, `probes/c07`: `heroes fetch`, then
  `printf '\0\0\0\1Bud1' > heroes.packages/core@0.2.1-*/.DS_Store`, then
  `heroes check main.hero`: `package_unread: ... is `sha256-0cbd51a6...` ... a
  file of it was changed, added or taken out since it was placed — `heroes
  fetch` places it again`, exit 1; `heroes fetch` repairs it. The rule is R7's
  and the message is true; what is missing is the file's name, which the
  compiler cannot give since it holds the sum and not the listing. That Finder
  writes `.DS_Store` into a folder it shows is a platform fact unrun here.
  Searched: `DS_Store`, `Finder OR ".DS_Store" OR "Thumbs.db"`, `placed tree
  "changed, added or taken out"`. Proposed class **adjacent** (a true message
  less exact than it could be), area `module`; its repair is a question for a
  sitting, not a lane: keep the listing beside the placed tree (a second file
  per package) or exempt named operating-system files from the listing (a
  change to R6's fingerprint, which the index's `list` must then share).
- **D2: a `path` in a package's own catalogue is refused by the verb and built
  by the compiler, and the compiler's message names the verb as its repair.**
  Reproducer, `probes/c42a`, `c42c`, `git/pp`: `pp/heroes.toml` holds
  `[dependencies.mypkg] path = "../../mypkg"`; `heroes add --path ../git/pp` is
  `requirement_refused`, exit 1; a hand-written `[dependencies.pp] path =
  "../git/pp"` alone is told at `check` *"... the program's catalogue lists no
  entry for that package: the program's catalogue holds every package of the
  build — `heroes add` writes it whole"*, exit 1, a repair that is refused;
  `heroes fetch` on that catalogue exits 0; with `mypkg` added by hand as well,
  `check` and `run` exit 0 and print `2`, and a later `heroes add --path
  ../mypkg` in that program was refused (one run; any add, an inference). The two readers are `selfhost/module/packages.hero:171-183`
  (`mapped`) and `selfhost/fetch/build_list.hero:99-104` (`required_id`). No
  case under `tests/` holds the compiler's text (`grep -rln 'lists no entry for
  that package' tests` printed nothing). Searched: `"only the program's own
  catalogue holds a"`, `requirement_refused path in:body`, `"lists no entry for
  that package"`, `"path" "own catalogue" requirement`. Proposed class
  **blocking**: a message whose `heroes add` repair cannot work (the class
  list's *a false message*) and two readers of one catalogue that disagree on
  whether it is legal; area `module` and `fetch`.
- **D3, the prototype's own** (not a defect of the tree, since the verb does not
  exist): `heroes new program a b` is told *takes one file, and two were
  given*; a production lane owes the verb its own words.

## 8. The seat's verdict

- **verdict**: **object** (no veto: nothing here adds a core construct, and
  nothing breaches the compiler's ceiling; one file's layout ceiling is
  breached by one operand shape, which is a choice, not a necessity).
- **section**: design.md §1.1 (*implementation simplicity ... sets the
  ceiling*) and §1.7 (*anything in the core must be implemented in the type
  checker and the lowering and the backend*), read against CLAUDE.md § 2
  (Principle 0) for the verb and design.md §1.6 for the spec's sentences.
  design.md says nothing of a verb that writes a project (`grep -i -E
  'scaffold|heroes new|heroes init|cargo new|heroes make' docs/design.md`: 3
  hits, every one *scaffolding* in the sense of the bootstrap compiler), so the
  verb stands on `.claude/rules/cli-surface.md`'s author's-decision route, as
  `heroes probe` did, and on nothing in design.md; I say so rather than invent
  a section for it.
- **implementation_cost**:
  - *The specification's half*: **zero compiler lines**. Every rule a
    candidate sentence would state is already held by `selfhost/module/`
    (3,600 lines by `cat selfhost/module/*.hero | wc -l`) and `selfhost/fetch/`
    (4,615), with `selfhost/placed_use.hero` (204) and
    `selfhost/package_mark.hero` (355); each sentence is sugar for the reader,
    erased before the compiler, and costs tokens only (the spec-warden's
    pricing, not mine).
  - *Q-stub*: **180 code lines** in the layout suite's unit, all in
    `selfhost/cli/` and `selfhost/main.hero`, `+254 −11` by `git diff
    --numstat` over 6 files, one new (`cli/stub.hero`, 150); no checker, IR,
    emitter or runtime line. Production adds, uncounted until written: its own
    second-word text, a staged write, about 20 golden cases. The two-word
    operand puts `argv.hero` at 303 of 300 (the `layout` suite's verdict),
    unless `Invocation` gains a field (26 sites in 9 files) and
    `operand_said`/`empty_operand` move out; a one-word operand avoids both.
- **needed_for_self_hosting**: **no**. The compiler never runs either; Part 11
  has no measured effect for either. The spec's sentences enter by the
  sitting's own test (a reader would otherwise write a wrong program); the
  verb by the author's decision.
- **argument** (≤120 words): The spec half is free in the compiler, and
  costly in the promise: a sentence promises its rule's substance
  (panel 232), and two of the rules it would state are not yet one rule. `path`
  in a package's catalogue is refused by `heroes add` and built by `check`
  (D2), and `internal` as a file is refused under a message describing
  folders (2439, open, repairable either way). Write the `internal` sentence as
  the element rule only with 2439 repaired in the message; write no `path`
  sentence until D2 is ruled. The verb is one module, but its two-word
  operand is its one costly choice, its card a hand card the README forbids,
  and its development route stops at the second package.
- **prediction**: the production `heroes new` lands, at its lane's commit,
  with `git diff --numstat -- selfhost` between 150 and 350 added lines and
  no line under `selfhost/check/`, `selfhost/ir/`, `selfhost/emit/` or
  `runtime/`; and `argv.hero` stays at or under 300 code lines by `heroes run
  tests/harness/main.hero -- <compiler> layout argv` if and only if the
  operand is one word (the kind a flag or the program the default). Checkable
  at that lane's gate.
- **condition**: I approve the spec half when D2 has a ruling (either the
  compiler refuses a `path` in a package's catalogue as the verb does, or the
  verb accepts what the compiler builds) and 2439's repair keeps the element
  rule (it does, in the round at `af9e95c6c`, § 9: met once it reaches `main`); I approve the verb's shape when its operand leaves `argv.hero` under
  its ceiling without a `DECIDED` row, and the sitting rules the card (written
  by the verb's own generator, or not written).

## 9. Where batch 30's round moves what I quoted (read in `.claude/worktrees/b30-round`, `git log --oneline 89db84347..HEAD`, never built there)

- **2439**, `af9e95c6c`: the `private_module` text for c06 now states the
  element rule (*"a module whose path holds `internal` as a part after its
  first, a folder's name or its own, is named only by the module the path
  before the last such part names ..."*, `placed_use.PRIVATE_RULE`), the rule
  unchanged and *"panel 244 rules on how the specification states it"* in its
  body. So the `internal` half of my condition is met once the round's gate
  passes and it reaches `main`; the spec sentence should be that rule's words.
- **2438**, `52d07264c`: `use <core>` alone (c01, c01n) and c16 stop offering
  the invented `parse`.
- **2434**, `677bc250e`: a refused `heroes add` takes back the trees it placed
  (c42b's leftover, § 7 item 9, and the golden that pinned it move).
- **2432**, `d7ff76523` on lane b30-pkg (per the session's git status): c13's
  `match` entry is refused where it is named.
- Not in the round's log as read: 2440 (c10, c11), 2442 (c19's *nothing
  else*), 2443, and neither of my candidates D1 and D2.

## 10. What I leave

- The prototype stays uncommitted in `.claude/worktrees/244-compiler-engineer`
  (6 files, `selfhost/cli/stub.hero` marked intent-to-add in that worktree's
  own index by `git add -N`, so `git diff` shows it), with its binary
  `heroes-q`; a copy is `p244-scratch/compiler-engineer/qstub/qstub.diff` (431
  lines) and `qstub/stub.final.hero`.
- Probes `p244-scratch/compiler-engineer/probes/` (throwaway git repositories
  under `probes/git/` and `probes/gitpkg/`), raw outputs `part1` to `part11`.
- No file outside the repository's root, no paid run, no GitHub write, no
  network read but `gh` and `file://`; `heroes fetch` was not run on c22, whose
  default index would have been the network.
