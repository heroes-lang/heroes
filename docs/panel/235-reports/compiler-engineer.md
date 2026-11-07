# Panel 235, the compiler-engineer's report

2026-11-07. Worktree `.claude/worktrees/235-compiler-engineer/`, detached at
`235-base`: `git log -1 --format='%H %P %T'` printed `3b071e4f12f2d3357ca2bd3bc338464dc54aa655
328524be86e7b667611610c183cde217063e86d8 9791d4787d4e63e5b7ded2c7c7cf1cf66347b2f7
f4dbb2b8db0c072de8a01cc6558307af4a4b89e1`. Compiler built there from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then
`./heroes build selfhost/main.hero -o heroes-next`, exit 0; both print
`heroes 0.6.0` / `sources 1a1e340778db789a`). Every number below is the
command beside it, run in that worktree with `TMPDIR` at the seat's scratch.
**The machine was not still**: other seats and a batch gate ran on it
throughout (`uptime` read load averages from 2.66 to 13.91 during the
sitting), so no duration here is a measurement; cost is in instructions
retired (`/usr/bin/time -l`), which the load does not move (five repeated
`check`s of one compiler spread 0.05%). Line counts are in the `layout`
suite's unit (`tests/harness/suite_layout.hero:1015`, `code_lines`:
non-blank lines outside `test` blocks, comments counted) by an `awk` mirror
of it (`235-compiler-engineer-scratch/bin/lines.sh`), with a second column,
the same lines less comment lines, where the difference matters.

Sections are in the order the questions were taken; the verdict row closes.

## First: what the briefs state that the tree does not

1. **"14 copies of `core/`"** (`00-shared.md`, *What the tree says today*;
   `probe/README.md`'s last block; the brief's question 11). The command
   counts 14 `core/` trees, and one of them is the canonical original:
   `ls -d examples/*/core | grep -v '^examples/packages/core' | wc -l`
   prints `13`. The 19 modules under `internal/` are 2 in the canonical
   tree (`examples/packages/core/strconv/internal/shown.hero`,
   `examples/packages/core/strings/internal/scan.hero`) and 17 in the copies
   (`find examples -path '*internal*' -name '*.hero' | grep -v
   '^examples/packages/' | wc -l` prints `17`). The `packages` suite's own
   header says *thirteen* (`tests/harness/suite_packages.hero:14`).
2. **"SHA-256 is written in Heroes over a `str`, so a file that is not UTF-8
   has no fingerprint today"** (`00-shared.md`). True of SHA-256, and it
   reads as if the compiler could not reach a file's bytes. It can: every
   module is read through `hero_file_read_shown`, which returns a file that
   is not UTF-8 whole, its bad bytes listed (`selfhost/module/reading.hero:52`,
   `source_text`), and the build cache already keys a file's raw bytes in C
   through the compiler's own door `hero_file_key`
   (`runtime/parts/key.c`, 141 lines; `runtime/hero_compiler.h:38`). A
   fingerprint over bytes is one more door of that shape, not a new problem
   (§ 5 below).
3. The milestone page's *`process.run` ... already runs `pkg-config`,
   `uname` and `curl`* holds: `cli/package_answer.hero:144` (`process.run`
   of `pkg-config`), `cli/doctor.hero:110` and `:151` (`capture` of
   `uname`), `cli/refresh.hero:81` (`program: "curl"`); a `grep` of
   `selfhost/` for `"git"` finds it only in this seat's prototype.

Everything else the briefs state that I re-ran held: the `Use` production
(`spec/heroes-spec.md:40`); `selfhost/modules.hero:107`;
`selfhost/cli/table.hero:238`, `assert table.len() == 13`;
`selfhost/cli/sha256.hero`, 292 lines, `of` at 119; the `use` counts
5,482 / 635 / 130 / 41 (the probe's four `grep | wc -l`, re-run); 69 programs
in `v0.6.0`'s `examples/`; `"vendor"` at `tests/harness/suite_records.hero:84`.

**And one fact the briefs do not state, which decides where U-angle's cost
lands**: `selfhost/parse/` has a directory budget, `BUDGETS` in
`tests/harness/suite_layout.hero:579`, `"selfhost/parse/ 8737"`, and it
stands at **8,736** of it (`lines.sh selfhost/parse/*.hero`, summed). Any
spelling that touches the parser raises that row.

## What I built, in one paragraph, so the sections below can cite it

A prototype of the author's spelling, U-angle, in my worktree and nowhere
else: `use <json>/parse` parsed, read off the token stream by discovery,
resolved through a catalogue `heroes.toml` beside the file you compile to a
tree placed at `heroes.packages/<entry>/` (or to a local folder an entry
names), a package's own `use` lines read from the package's root, its
modules given a C name no module of the program can have, `internal/`
enforced by the compiler, and the mistakes of § 2's table told. Built in two stages
from the seed's compiler (`./heroes build selfhost/main.hero -o
heroes-proto`, exit 0), it passes the compiler's own tests
(`./heroes-proto test selfhost/main.hero`: *1797 tests, all passed*, exit
0, the prototype's own four test blocks among them),
builds and runs the demo (`235-compiler-engineer-scratch/demo1`: one index
entry placed, one program module; prints `IV` and `25`), and builds the
`ini` example with `core` as a catalogue entry instead of its copy (§ 11).
Its lines, per module, are § The prototype's cost. **A route that does not
build is not adopted** (`/panel` § 3c): this one builds.

## 0. The base, from the compiler's side: Go's selection against Cargo's

**What each costs to write.** Go is on this machine (`go version`:
`go1.27.3 darwin/arm64`, its sources under
`/opt/homebrew/Cellar/go/1.27.3/libexec/src`), so its implementation was
counted rather than recalled: `cmd/go/internal/mvs/mvs.go` 314,
`graph.go` 138, `errors.go` 69 code lines (`awk`: non-blank, non-`//`), 521
in all, and they carry more than selection (upgrade, downgrade, the
requirement graph's own errors). Minimal version selection itself I wrote in
Heroes for this sitting, `235-compiler-engineer-scratch/mvs/main.hero`:
**63 lines in the layout unit, 54 less comments**, the walk over every
requirement reachable from the program and the comparison of three numbers,
under two tests (`heroes-next test mvs/main.hero`: *2 tests, all passed*;
the first asserts that a version an older requirement brings in stays in
the list, as MVS's own definition keeps it). It asks the index nothing but
the catalogue of each version it is named. **Cargo's resolver is not on
this machine** (`~/.cargo/registry/src` holds `semver` and
`cargo-platform`, no resolver), so its size in any language is a question,
unrun. What the survey's 2.2 row 4 states is enough to place it: *Cargo
prefers the highest version currently available*, so its answer is a
function of the index on the day it runs, it needs every version of every
package (a listing row 78 must then serve), and two ranges that cannot be
unified are an error found by search.

**Whether a lock file is needed.** Under MVS the selected versions are a
function of the catalogues alone, so a second file adds nothing a build
could not recompute: Go has none, only `go.sum`'s fingerprints (Table 1A).
Under Cargo the lock is what makes the build reproducible, because the
answer moves with the index (2.2 row 4; Table 1B's `Cargo.lock`). The
author's ruling, an exact version and a fingerprint in every entry, makes
Cargo's solver degenerate in the root and leaves it whole one level down,
where a dependency's own ranges still need a lock (an inference from Table
1B: a dependency entry's `version` is a requirement, *`1.2.3 :=
>=1.2.3, <2.0.0`*; unrun).

**What each asks of the catalogue, and the rule I recommend the compiler
rest on.** Since Go 1.17 the main module's file *includes an explicit
`require` directive for each module that provides any package transitively
imported* (Table 1A, *go-gated behaviour*). Copied, that is: **the
program's catalogue lists every package of the build, transitive ones
included, each at the version MVS chose, each with its sum.** Then the
compiler reads one file, resolves every module of the build from it, never
touches the network, never solves anything, and can check the MVS invariant
(the program's version of a package is at least each dependency's) while it
reads the dependencies' catalogues it already opens for § 16. The verb that
writes the catalogue (`heroes add`, `heroes fetch`) is where MVS runs, with
the 54 lines above. Cargo's base asks the compiler for nothing more at
build, but asks the verb for a solver whose size is a question and the
index for a listing.

**What each asks of a fingerprint.** Go's `h1:` is the one a sum in the
entry needs, read in the installed source
(`cmd/vendor/golang.org/x/mod/sumdb/dirhash/hash.go`, 93 code lines): *the
base64-encoded SHA-256 hash of a summary ... a single line for each file
... the hexadecimal SHA-256 hash of the file content, two spaces, the file
name, and a newline*. It is over names and contents, so the same release has
one sum whether it came from the index, from git or from a folder. Cargo's
`cksum` is the `.crate` archive's SHA-256, and its git sources carry a
commit and no checksum, its path sources nothing (2.2 row 2): three sources,
three answers. § 5 says what h1 costs here.

**So, from the compiler's side: Go.** Selection measured at 54 lines of
Heroes and outside the compiler; one file; one rule the compiler checks; one
fingerprint for every source. The edition and toolchain lines (`go`,
`toolchain`) are not needed by anything above: the compiler reads no
version of itself from the catalogue, and § 2 of CLAUDE.md can stand.

## 2. The three spellings: what each costs the lexer, the parser and the resolver

| | lexer | parser, `selfhost/parse/` | discovery's reader, `module/paths.hero` | the four readers of a module's name | a new refusal of the program's own tree |
|---|---|---|---|---|---|
| **U-angle** `use <json>/parse` | 0: `<` and `>` are `.lt` and `.gt` already (`selfhost/token.hero:90`, `:92`); the probe's refusal is the parser's, at `use` | `parse/use_mark.hero`, new, **92** (78 less comments); `parse/use_line.hero` **+11** (263 → 274, its ceiling 310, `suite_layout.hero:527`). The directory: **8,736 → 8,839, 102 past its budget of 8,737** | `marked`, **+13** (128 → 141), and two texts added to the test that runs both readers over one text (`paths.hero`, *the two readers of one adjacency rule agree*), which passes | one function, `module/placed.identity`, called at each | none |
| **U-bare** `use json/parse` | 0 | 0 | 0, but the line no longer says what it names: discovery must ask the catalogue for every `use`'s first part | each must ask the catalogue too, so the compilation must carry the entries (`source.Source` has no field for them, `selfhost/source.hero:61`) | an entry named as a folder or module of the program's own: a file-system probe per entry per build |
| **U-pkg** `use pkg/json/parse` | 0 | 0 (`use pkg/json/parse` parses today, the probe's second block) | 0 | as U-angle, the mark being the first part | a folder `pkg/` or module `pkg.hero` of the program's own: `git ls-files \| grep -E '(^\|/)pkg(/\|\.hero$)'` prints nothing today |

**The four readers** are the places that take a `use` line's text as the
name of a module: `modules.unknown_modules` and `modules.cycles`
(`selfhost/modules.hero:193` and `:282` at `235-base`), and
`resolve/top.read_uses` and `unused_uses` (`selfhost/resolve/top.hero`, the
two `named = source.span_text(s, used.name)` lines), with discovery's loop
(`modules.hero:98-117`) as a fifth. Every spelling changes all five; U-angle
changes them by asking the text, U-bare by asking a file.

**The formatter costs nothing under any of the three**:
`selfhost/print/fmt.hero:338`, `use_line`, prints the name's span text, and
`heroes-proto fmt` printed `use <roman>/numeral` back unchanged.

**What the parser line count buys.** U-angle is the only spelling whose
cost lands in `parse/`, a directory at its budget, and the only one whose
line says, without a file, whether its module is the program's: the
budget's row (`suite_layout.hero:531-582`) is raised by a sitting's form *read
beside its neighbours, never moved elsewhere to fit a count*, which is this
case. The 103 lines include six of the mistakes below; the rest would add
to them.

**The mistakes, each run through the prototype** (`heroes-proto check
main.hero` in a copy of the demo, the `use` line replaced; every one exit
1):

| written | told | fix |
|---|---|---|
| `use <roman/numeral>` (C's `#include <a/b>`) | `package_mark_holds_a_path`: *only the package's name goes inside `<` `>`, its module after them — `use <roman>/numeral`* | **certain**, `<roman>/numeral`: an entry's name holds no `/`, so the first part is the entry |
| `use roman/numeral`, the brackets forgotten, `roman` an entry and no own `roman/` | `package_unmarked`: *there is no module `roman/numeral` of this program, and `roman` is a package of its catalogue: a package's module is written in angle brackets, `use <roman>/numeral`* | **certain**: the module is on disk at the entry's tree |
| `use <geom>/area`, brackets added to the program's own module | `unknown_entry`: *`<geom>` is no entry of `heroes.toml` — `heroes add geom` writes one from the index* | `geom/area`, because `geom/area.hero` is there: printed **certain** by the first prototype, which is wrong (below), and a **guess** since |
| `use < roman >/numeral`, `use <roman> /numeral` | `package_mark_apart`: *the mark touches its parts — `use <roman>/numeral`, with no space inside it* | **certain** |
| `use <roman>` | `package_wants_a_module`: *a package is reached through one of its modules — `use <roman>/parse`, `parse` being a file of the package* | none: which module is the author's |
| `use <romam>/numeral` | `unknown_entry` as above | none in the prototype; a near entry name would be a `guess` |
| `use <roman>/numerals` | `unknown_package_module`: *the package `roman` has no module `numerals`: `use <roman>/numerals` reads `heroes.packages/roman/numerals.hero`, and that file is not there* | none in the prototype; a near module a `guess` |
| no catalogue | `unknown_entry`: *`<roman>` is no entry of a catalogue, and there is no `heroes.toml` — `heroes add roman` writes one from the index* | none: a fix edits the compilation's text, and the catalogue is not in it |
| `version = 2.1.0` in the catalogue | `catalogue_refused`: *`use <roman>/numeral` needs the catalogue, and it does not read: heroes.toml:2: a value is a string between double quotes: `version = "1.4.2"`* | none, same reason |
| the tree not placed | `package_not_fetched`: *the package `roman` is in the catalogue and not fetched: `heroes fetch` places it at `heroes.packages/roman/`* | none |
| **told badly, owed**: `use <roman>.numeral` | `package_wants_a_module`, where *write `/`* is the certain answer | owed |
| **told badly, owed**: `use <"roman">/numeral` | `package_wants_a_name`, caret on `<` | owed, certain (unquote) |
| **told badly, owed**: `use <github.com/acme/roman>/numeral` | `package_mark_unclosed`: *`<github` wants its `>`* | owed: *an address goes in the catalogue; the `use` names its entry*, a `guess` of the address's last part |
| **told badly today and under every spelling**: `use github.com/acme/roman/numeral` | the base's `module_path_has_no_parts`: *`use github/…`* | owed, as above |

**The controls' mistakes, unbuilt, from the same lookups.** U-bare:
`use <json>/parse` (another ecosystem's or this test's angle habit) has a
certain fix where `json` is an entry, `json/parse`; an entry named as the
program's own folder has none, since which one to rename is the author's.
U-pkg: `use json/parse` with the entry `json` and no own `json/` has a
certain fix, `pkg/json/parse`, by the same reasoning as `package_unmarked`;
a folder `pkg/` of the program's own has none. Each is the lookup the
prototype already does, under another spelling.

**Panel 056's second condition, from the compiler's side** (the question
is 17, the spec-warden's; this is what the compiler shows). Under U-angle
the line's text says which module it names, so deleting or mistyping an
entry turns `use <json>/…` into an error and never into the program's own
module; under U-bare the text is the same for both, and the catalogue's
answer is what decides. **And a fix can break what the spelling keeps**,
which this seat found in its own prototype: with the catalogue deleted
beside a program that has its own `roman/numeral.hero`, `use <roman>/numeral`
was told `unknown_entry` with *fix (certain): write `use roman/numeral`*
(`235-compiler-engineer-scratch/v_deleted`, measured), so `check --apply`
would have compiled the program's module in the package's place, silently
where the two agree in their signatures. A `certain` fix that writes a
program meaning something else is a `blocking` defect by
`.claude/rules/verification.md` § Bounded discovery. **The rule: a fix that
moves a `use` across the mark, either way, is certain only where nothing
on the other side of the mark answers the same path**; `package_unmarked`
meets it (no own module was there), the bracket-removing fix does not (the
package's entry may be what is missing). Repaired to a `guess` in the
prototype (`module/package_told.hero`), and the same case then printed
*fix (guess): write `use roman/numeral`*, rebuilt and re-run.

Two classes no spelling escapes. **A mistake in the catalogue has no
machine-applicable fix**: a `Fix` carries a span into the compilation's
text (`selfhost/diag.hero:47-51`), and the catalogue is not a file of the
compilation; every F arm's slip (an unquoted value, YAML's `1.10`, JSON's
trailing comma) is told with its line and never repaired by `check
--apply`. Making it one is an architecture change: `source.InputFile` gains
a kind the lexer and parser skip, and `check --apply` writes it. Only
Heroes' own syntax could be read by the existing parser, and only with a
new top-level production, in `parse/` again. And **a catalogue that does not
read must name nothing**: the prototype's first build used the entries
read before the refused line, and the demo with `version = 2.1.0` compiled
at exit 0 (measured, then repaired in `catalogue.entry`, with a test). The
rule is all or nothing, whatever the format.

## 3. Inside a dependency: its own `use` lines from its own root

Six lines (`module/placed.hero`, `identity` and `package_of`): a file whose
module is `<roman>/numeral` names `use internal/check` as
`<roman>/internal/check`, read from the package's tree. Measured in the
demo: `heroes.packages/roman/numeral.hero`'s `use internal/check` loaded
`heroes.packages/roman/internal/check.hero` (its `#line` in the emitted C),
and the program printed `IV`. In the `ini` example (§ 11) the canonical
tree's `use strings/internal/scan` loaded
`../packages/core/strings/internal/scan.hero`. A dependency's `use <x>/…`
is § 16's.

**The C names a package's modules get.** At `235-base`,
`module/names.component_of` keeps the letters and digits of the whole
path, so `<roman>/numeral` would be `romannumeral`, the C component of the
program's own `roman/numeral`. Measured on the prototype's first build: a
program with its own `roman/numeral.hero` beside the entry `roman` was
refused, `module_names_collide`, *both become `h_romannumeral_…`*, a
correct program refused. The prototype leads a package's component with
`1` (`names.hero` +5): the same program then built and ran, printing `IV`
and `own 4`, its C holding `h_1romannumeral_to_roman` beside
`h_romannumeral_to_roman`. Two packages can still meet (`<ab>/c` and
`<a>/bc` are both `1abc`), and are refused by the same check, loudly; the
alias that repairs it is the catalogue's (§ 16).

**The premise that makes `1` safe is false today, one level down.**
`emit/ctype.hero:98-101` says *what separates a name the EMITTER invents
from a name an author could write: a leading digit*, and `emit/mangle.hero`'s
`INVENTED` rests on it. A module whose stem starts with `_` and a digit
gets a component led by that digit: `_0opt.hero` holding `function five`
emits `h_0opt_five` (`heroes-next build main.hero --emit-c | grep -c
h_0opt_five` printed 3), inside the `h_0…` namespace the emitter keeps for
its own names (`h_0opt_a8ea2` stood in the same C). No collision came of it
in that run. Under packages it matters twice: own `_1json/parse.hero`
beside `<json>/parse` is refused by `module_names_collide`, loudly, but the
emitter's `h_0…` names are no module and nothing compares them. **The
robust route is the one rule for both: no component of the program's own
begins with a digit** (a letter prefixed where the filter leaves a digit
first; `git ls-files` names no tracked module or folder beginning with
`_`, so no emitted C in the tree moves). Unbuilt; a finding for an
`adjacent` defect whatever this sitting rules.

## 4 and 20. The catalogue's formats: each reader in lines, each grammar in productions

All five readers were written for this sitting, in my worktree, each to the
shape its F arm's section states (`docs/panel/235-briefs/blind/sections/`),
each refusing whatever else its format allows with the line it stands on,
each under a test that reads the arm's own example and refuses the arm's
own trap; **all pass** (`heroes-proto test selfhost/formats_probe.hero
--only arm`: *12 tests, all passed*, the four readers' among them; the TOML
reader's two ran with the compiler's own 1796). They share what follows a name and a value: the entry's record,
its validation (one source, a `tag` with `git`, a sum's shape, the name's
shape), the duplicate check, and where a tree is: 87 lines of
`module/catalogue.hero` and 47 of `module/formats/fields.hero`. The
format's own lines are then:

| format (its file in the prototype) | the reader's own lines, layout / less comments | what it reuses | its grammar, in productions | a value that is not a string |
|---|---|---|---|---|
| line format (`formats/lines_format.hero`) | **60 / 54** | nothing | `Catalogue = { [ Entry ] NEWLINE } . Entry = "dependency" name ( "index" version sum \| "git" address tag sum \| "path" folder ) .` with `#` a comment line: **2 productions**, and words are apart by spaces | none exists: every word is text, so `1.10` is `1.10` |
| Heroes' own syntax (`formats/hero_format.hero`) | **59 / 53** | the compiler's lexer (`lexer.lex_one`), so a string, a comment and an indentation mean what spec § 1 and § 2 say | `Catalogue = { Dependency } . Dependency = "dependency" ident NEWLINE INDENT { ident ":" string NEWLINE } DEDENT .`, **2 productions** over the spec's own lexical rules | the lexer's: `tag: 1.10` is a number literal, refused (*a field is `<key>: "<value>"`*) |
| JSON (`formats/json_format.hero`) | **54 / 49** | **the RFC 8259 reader the compiler already carries**, `selfhost/mutate/json_read.hero`, 278 / 231, reached from `main.hero:44` through `cli/mutate.hero:31`, `cli/mutate_recovery.hero:13` and `mutate/recovery.hero:29`; so **no new reader** | RFC 8259's grammar, plus `Catalogue = "{" "\"dependencies\"" ":" "{" [ Entry { "," Entry } ] "}" "}" . Entry = string ":" "{" [ Field { "," Field } ] "}" . Field = string ":" string .`, **3 productions** over JSON's | `"tag": 1.10` reads as a number, refused (*`tag` is a string between double quotes*) |
| TOML subset (`module/catalogue.hero`'s reader) | **100 / 98** | nothing | `Catalogue = { [ Table \| Pair ] [ Comment ] NEWLINE } . Table = "[" "dependencies" "." name "]" . Pair = key "=" basic-string .`, **3 productions**, and every other TOML form refused: inline tables, dotted keys, literal and multi-line strings, escapes, arrays, numbers. TOML 1.0's own grammar is the subset's *ceiling*, whose size I did not count (unrun) | `version = 2.1.0` refused (*a value is a string between double quotes*) |
| YAML subset (`formats/yaml_format.hero`) | **188 / 173** | the line format's `rows` | `Catalogue = "dependencies:" NEWLINE { Entry } . Entry = indent₁ name ":" NEWLINE { Field } . Field = indent₂ key ": " Scalar NEWLINE . Scalar = quoted \| plain .`, **4 productions, plus a table that is no production**: which plain scalars YAML 1.2.2's core schema reads as a null, a bool, an integer or a float, and which YAML 1.1 reads as a bool (34 words and a numeric shape, `not_a_string`), refused where a string belongs, since which YAML a later reader runs is not the file's to say | `tag: 1.10` refused: *YAML reads `1.10` as a number, a bool or a null; write it in quotes, "1.10"*; `version: no` refused |

**Can reading it run code?** None of the five readers evaluates anything:
the Heroes reader accepts string literals and no expression (so not Zig's
`build.zig`, only its `.zon`-like data shape), and the YAML subset refuses
anchors, aliases and tags, YAML's only reach beyond data. A Heroes
*program* as the catalogue would run code and is not on this list.

**What the ceiling says between them.** By the reader's own lines, JSON and
Heroes' syntax are the cheapest because each reuses a reader the compiler
already trusts, and the line format is cheapest of the new ones; the YAML
subset costs three times any other, and most of it is the scalar table, the
hazard the reader test registers (`blind/README.md`, T2's `1.10`). No
reader moves the parser's budget: they live under `module/`. The format
is the reader test's to choose; none is above the ceiling, and none earns a
`certain` fix (§ 2's last paragraph).

## 5. The fingerprint: over which bytes, and what a file that is not text costs

**What there is.** `cli/sha256.hero`'s `of(text: str)`, 188 lines less
comments, takes a `str`. Measured as built here (`heroes build` defaults to
`-O0`, `selfhost/cli/table.hero:106`): a probe program calling it
on a file of 991,000 bytes retired 1,607,857,839 instructions against
19,098,321 on an empty file (`/usr/bin/time -l`, its answer equal to
`shasum -a 256`'s), **about 1,603 instructions a byte**; `openssl dgst
-sha256` retired 0.39 a byte on the same file. The canonical `core` tree is
21,045 bytes in 6 files (`find examples/packages/core -type f | xargs cat
| wc -c`): about 34 million instructions to hash in Heroes, against the
95.0 billion of a `check` of the compiler.

**The bytes, and a file that is not text.** The robust route is Go's `h1:`
shape (§ 0), split at the one line where text stops being enough:

- each file's SHA-256 **over its bytes, in C**, as a door beside
  `hero_file_key` in `runtime/hero_compiler.h` (that door already reads a
  file's raw bytes for the build cache, `runtime/parts/key.c`, 141 lines);
  a SHA-256 in C is a known size and I did not write it here (unrun);
- the summary, one line per file, `<hex>  <path>\n`, sorted by path bytes,
  hashed by `sha256.of` in Heroes, because the summary is text;
- a path that is not UTF-8 or holds a newline refused at fetch (h1's own
  rule refuses the newline); `hero_dir_scan(root, want, recursive)`
  (`runtime/hero_os.h:342`) lists the tree.

So no file of a fetched tree is read as text to be fingerprinted, no
`str` ever holds bytes that are not UTF-8, and a `.c`, a header or an image
in a package is covered like a module. **Every file of the placed tree is
in the sum**, `.hero` or not: a header a fetched `extern` reads is code the
program trusts.

**When it is checked.** At fetch, and at every `build`, `check` and `run`
that reads the tree: a tree inside the program is a folder anything can
edit (Go's module cache is written read-only, the survey's 2.1 row 6; ours
would not be), and a stale or edited tree compiling at exit 0 is the
outcome § Precedence's rank 3 refuses. In C the cost is per byte of the
tree, read once per build; in Heroes at `-O0` it is the 1,603 a byte above.
The build cache could spare it (a file whose `hero_file_key` has not moved
since its sum was checked), at the cost of a second key per file; unbuilt.

## 6. Versions

Exact versions in the catalogue, chosen by MVS at the verb, one per
package per program. At build the compiler checks a version's shape (three
numbers, the prototype's `catalogue.incomplete` asks only that a source is
named) and, once § 16's rule is built, that the program's version of a
package is at least each dependency's; it chooses nothing.

## 7. Where fetched code lives, and what the suites must know of it

**Inside the program, under a folder no `use` can name**: the prototype's
`heroes.packages/<entry>/`, since a path's part holds no `.` (`use geom.`
is refused at the dot, `module_path_has_no_parts`). What the compiler gets
from it, measured on the demo: every name it prints is relative and the
same on every machine (`#line "heroes.packages/roman/numeral.hero"` in the
emitted C; the diagnostics' `at` lines), so a golden can pin a fetched
module's message; no `HOME`, `XDG_CACHE_HOME` or `LOCALAPPDATA` is read, so
no platform rule enters the compiler; two programs never share a tree, so
no lock between two builds is needed beyond `build/`'s own
(`cli/publish.hero`). Against it: one copy per program, which this
repository answers with a local-folder entry (§ 11), and a tree anything
can edit, which § 5's check answers. A cache per user would cost the
compiler the platform's cache folder, absolute paths in every message,
`#line` and golden, and a cross-program lock; a virtual name
(`<roman>/numeral.hero`) would buy back the messages and lose the debugger,
whose `#line` must name a file (the `lldb` suite pins them).

**What each piece of the tree must change** (the brief's list):

- **`build/`'s per-module cache: nothing.** Its key is the compiler, the
  level, the module name, the unit's emitted text, the runtime, the search
  paths and the resolved package flags (`selfhost/cli/units.hero:7-18`): a
  fetched module is one more unit, keyed the same way.
- **`corpus`**: its check 2 reads a program's *own* modules
  (`tests/harness/suite_corpus.hero:545-548`, `own_module`: *a copy stands
  under the program's `core/`*), and it walks the file system
  (`shell.list_tree`), so a placed tree is walked whether git ignores it or
  not; `own_module` must exclude `heroes.packages/` (one line). And `heroes
  test main.hero` ran the `core` package's tests with the program's (25 in
  the `ini` example, § 11): whether a program's `test` runs its packages'
  tests is the sitting's to say; Go's and Cargo's test the main module's own.
- **`packages`**: § 11.
- **`records`** and **`unseen`**: they read `shell.project_files`, which is
  `git ls-files --cached --others --exclude-standard`
  (`tests/harness/shell.hero:694-696`), so a placed tree git ignores is
  never read, and one it tracks is read like any file, a stranger's
  invisible character included, which is right. `records` exempts
  `"vendor"` at the root only (`suite_records.hero:84`); a tracked
  `heroes.packages/` under `examples/` would want the same exemption.
- **`compat`**: nothing, so long as the compiler reads no catalogue that
  is not there: § 9.
- **`canonical`** (`heroes fmt` over the tree): a tracked placed tree is a
  stranger's code, not ours to hold canonical; it is exempt as `vendor` is,
  or ignored.

## 9. A program with no catalogue compiles as today

The prototype reads `heroes.toml` once, in discovery, from the directory of
the file you compile (`modules.load_edited`), and a read failing
`file_not_found` answers a catalogue of no entries
(`module/catalogue.hero`, `read`). Measured:

- **the release's programs**: `./heroes-next run tests/harness/main.hero --
  ./heroes-proto compat` printed *79 programs of examples/ and
  tests/thesis/, 78 compiled by 0.6.0's compiler, 1 unfinished, their holes
  alone reported by `check`: all ok* (`compat: 1 passed, 0 failed`; the
  suite reads 79 where the brief counts `examples/`'s 69, the other ten
  being `tests/thesis/`'s). That run includes the prototype's one widened
  refusal, `internal/` (§ 12), which no program of the release crosses;
- **the compiler's own tests**: *1796 tests, all passed* on the first
  prototype build, and *1797 tests, all passed* on the final one, § 12's
  test block the one more (`./heroes-proto test selfhost/main.hero`, exit 0
  both times);
- **the cost** to a build that has no catalogue: § The latency.

## 11. `core` as a catalogue entry, against the copies

Measured on a copy of `examples/ini` and of the canonical tree
(`235-compiler-engineer-scratch/ex/`): `ini/core/` deleted, `ini/main.hero`'s
`use core/strings` written `use <core>/strings`, the canonical tree's
`use core/strings/internal/scan` written `use strings/internal/scan` (its own
root, § 3), and `ini/heroes.toml` holding one entry,
`[dependencies.core]` / `path = "../packages/core"`. `heroes-proto build`
exit 0, the program's output equal to `main.expected` (`diff` silent),
`heroes-proto test main.hero` *25 tests, all passed*; the emitted C names
`../packages/core/strings.hero` and `../packages/core/strings/internal/scan.hero`.

So, for this repository: **the 13 copies become 13 entries of one line
each, naming the one tree**, and the 34 `.hero` files of the copies (38
`.hero` files under every `core/`, 4 of them canonical: `find examples
-path '*/core/*' -name '*.hero' | wc -l`) leave the tree. A path entry may
leave the program's folder, `..` and all, because it is the catalogue's,
not a `use`'s (panel 099 R4 refuses the climb in a `use`). What the
`packages` suite becomes, label by label
(`tests/harness/suite_packages.hero:17-37`): `copies` has nothing to hold,
the catalogue naming one tree; `internal` becomes the compiler's (§ 12),
and so holds every fetched package, which no harness walk of this tree can
see; `reach`, `case`, `names`, `codes`, `quiet`, `docs`, `tests` and
`cards` stay, over the canonical tree alone. Once `core` is published
(the second tempo) the entries become index or git entries and the path
entry is what a package's own author keeps while developing it.

## 12. `internal/`, and the verbs

**`internal/` inside a fetched package is the compiler's to hold**, since no
harness reads a stranger's tree. Built: `package_told.private_reaches`
and `owner_of` (in `module/package_told.hero`, with its test), the
`packages` suite's rule word for word (`tests/harness/package_rules.hero:64-100`,
`internal_owner`): a module under an `internal` folder belongs to the
module before the last `internal` part, and only that module and the
modules under it name it. Measured: `use <core>/strings/internal/scan`
written in `ini/main.hero` is refused, `private_module`, *`<core>/strings/
internal/scan` is private to `<core>/strings` ...*, exit 1, and the
canonical tree's own `use strings/internal/scan` from `strings.hero` is not.
The prototype applies it to the program's own folders as well, a widened
refusal `compat` saw no program cross; whether a program's own `internal/`
binds it is the sitting's to say (Go's rule binds every import path, the
survey's 2.1 row 9).

**The verbs.** Design.md §3.5 already names two, `heroes add` and `heroes
fetch` (`docs/design.md:898`), and nothing in § 0 needs a third: `add`
writes an entry (a bare name against the index, `--git`, `--path`), runs
MVS (§ 0) over the catalogues it fetches and writes the build list and its
sums; `fetch` places every entry's tree and checks its sum. Where each
enters, all unbuilt:

- `selfhost/cli/table.hero`: two `Command(` rows, two arms of `Tag`
  (`table.hero:18-34`), and the pin `assert table.len() == 13`
  (`table.hero:238`) moved to 15 in the same commit, as its comment asks;
- `selfhost/main.hero`: two arms where the commands that read no program
  are dispatched (`main.hero:199-206`), since neither compiles anything;
- `site/src/lib/claims.ts:263-270`, `verbs()`, reads the `Command(` rows,
  so the site's count follows by itself and every page saying *thirteen*
  must say *fifteen* before the site builds (`.claude/rules/verification.md`,
  the map's last row, and its story of `probe` costing a deploy);
- `.claude/rules/cli-surface.md`: its stopping rule, its exit-code contract
  (a fetch that fails on the network is exit 2, the tool could not run; a
  sum that does not match is exit 1, a mistake in the program's inputs),
  and § There is no fourth slot, which § 10 of the sitting rewrites.

`build`, `check` and `run` gain no flag and never fetch: § The latency and
§ 7 rest on it, and so does every golden.

## 14. The interface that stays fixed from the first version to the second

What a program and its author can observe, each pinned by a golden before
the first version lands, so the second (over the core packages) is judged by
the same files:

1. **The catalogue's grammar and the reader's refusals**: one
   `tests/golden/check/` case per refusal of § 2's table, each with its
   message (and a golden of a catalogue that reads).
2. **The placed tree**: its folder (`heroes.packages/<entry>/`), and its
   bytes equal to the published tree's.
3. **The fingerprint's definition**: the sum of a fixed tree, files of text
   and one that is not, a golden of the string.
4. **The verbs' argv, exit codes and output**, and the catalogue each
   writes, byte for byte.
5. **Every build-side diagnostic** of § 2, § 3 and § 12, which the
   prototype already prints.

**The goldens need no network, so row 78 owes one thing to this row**: an
index the verbs can read from a folder (the static index the sitting's
question 8 names as one variant), and a git source that can be a local
repository, so `tests/golden/` holds both and the net never leaves the
machine. The harness's goldens are single files today; a case that needs a
catalogue and a placed tree is a folder, which the `check` form would have
to learn (unbuilt; how much it costs is a question).

**What the second version replaces**: the transport alone, `process.run`
of `git` and `curl` (`cli/process.hero:109`, `cli/refresh.hero:81`'s
shape) by the core packages that fetch; the fingerprint's per-file SHA-256
door stays C either way, since it reads bytes no `str` holds.

## 16. Alias scope: whose catalogue, and what one package is called

**The rule I recommend, and the one the compiler can check: the program's
catalogue is the build list** (§ 0, Go 1.17's rule copied). Then:

- **a module's name in the compilation is the program's alias for its
  package**: `<json>/parse` is `json`'s module whoever names it;
- **a dependency's own `use <x>/…` is read through the dependency's own
  catalogue** to the package `x` names there (its index name, or its git
  address), and then to the program's entry for that same package: its
  alias there may differ, and its module is `<that alias>/…`;
- **two catalogues giving one alias to two packages** is no conflict:
  each alias is read in its own catalogue, and the two packages are two
  entries of the program's, under two aliases;
- **two aliases to one package** within the program's catalogue are
  refused: one package, one version, one name in the compilation. Across
  catalogues they are the expected case and meet at the program's one entry;
- **an index package may be named otherwise than its index name**, as a
  git entry may: the alias is the catalogue's, the package is the source;
  the reader test's simplification is not needed by the compiler;
- **the C names** come from the program's alias (§ 3: `1` + its letters),
  so a collision is repaired by the one file the program's author owns.

**Why not the alias's text, which is what the prototype and the grader
do.** The prototype reads a dependency's `use <x>/…` through the program's
catalogue directly (`placed.identity` keeps a marked name as written),
right only where every catalogue uses the program's aliases. Read by text,
two aliases of one package are two modules: the package compiles twice,
its record types are two types, and a value one dependency hands the
program is refused with a type mismatch between two types of one name.
That is loud, not unsound, and it breaks *one version per program* in
meaning while keeping it in files. Unbuilt: the dependency's catalogue read
once per package in discovery and matched by source, about the size of
`placed.hero` again (an estimate, unrun).

## The latency: what reading a catalogue costs a build

Every figure is instructions retired, the two compilers alternated in one
sitting, the first pair discarded (`235-compiler-engineer-scratch/check-ab*.txt`,
`build-ab.txt`, `demo-ab.txt`):

- **`check selfhost/main.hero`, a build with no catalogue**: the base
  compiler (`heroes-next`, stage 2 at `235-base`) against the prototype,
  both judging the same tree. First prototype: +38.6, +51.5, +47.6, +51.9,
  +55.1 million over about 95.04 billion, **5 of 5 slower, median +0.054%**.
  The rebuilt prototype with § 12's rule: +154.8, +20.5, +90.6, +88.8,
  +93.2 million, **5 of 5 slower, median +0.095%**; the rule's `owner_of`
  slices a string at every position of every `use` name (5,482 of them in
  `selfhost/`), which a byte comparison would not. Both are below the
  latency rule's floor of effect, *a median 0.5 %*
  (`.claude/rules/verification.md` § The latency ceiling's instrument), by
  five and nine times, and both are breaches of *never slow the compiler
  down* by its own word, owed a sentence in the landing commit.
- **a warm `build selfhost/main.hero`**, the first prototype: −372.6, −33.9, +152.6, +30.3
  million over about 524.9 billion, **2 of 4 slower**: no difference this
  instrument resolves. The check's 50 to 90 million are inside the build's
  own spread.
- **a build that has one**: the demo checked through its catalogue
  (`demo1`) against the same three modules as the program's own
  (`demo_own`): +644.6, +761.7, −42.3, +214.9, +704.2 thousand over about
  93.4 million, **4 of 5 slower, median +0.69%**: reading three lines of
  TOML and a lookup per `use`.
- **the fingerprint, if checked at every build** (§ 5): 1,603 instructions
  a byte in Heroes at `-O0`, so a tree of the canonical `core`'s size, about
  34 million; in C, the bytes read once.

## The prototype's cost, and where it lands

In the layout unit, then less comments, against `235-base` (`git show
HEAD:<file>` through the same counter):

| module | pass | lines | less comments |
|---|---|---|---|
| `parse/use_mark.hero` (new) | parser | 92 | 78 |
| `parse/use_line.hero` | parser | +11 (263 → 274; ceiling 310) | +10 |
| `module/paths.hero` | discovery | +13 (128 → 141) | +10 |
| `modules.hero` | discovery and the graph's errors | +12 (303 → **315; ceiling 320**) | +12 |
| `module/names.hero` | the C component | +5 | +2 |
| `resolve/top.hero` | resolver | +1 | +1 |
| `module/catalogue.hero` (new) | discovery: the TOML reader (100) and what every format shares (87) | 187 | 163 |
| `module/placed.hero` (new) | discovery and resolver: a module's name and file | 34 | 21 |
| `module/package_told.hero` (new) | the graph's errors: § 2's catalogue messages and § 12's rule | 120 | 97 |
| **total** | | **475** | **394** |

**Where it lands**: parser 103, discovery and the module graph 371,
resolver 1. **Type checker, lowering, descriptor pass, ownership pass,
emitter and runtime: 0.** A fetched module is a module: once discovery has
named its file, nothing downstream can tell it from the program's own, and
the one place C sees the difference is the component a module's name is
reduced to (`selfhost/source.hero:163`, one call). This is not a core
construct (design.md Part 5's seven are untouched); it is not even sugar in
§1.7's sense, since nothing reaches lowering to be erased. Its cost is the
frontend's and the command's.

**The instrument's verdict on it**: `layout` narrowed to
`parse/use_mark.hero` failed `layout/budget`, *selfhost/parse/: 8839 lines
of code, past its budget of 8737*; narrowed to `modules.hero`, it passed.
**Owed and unbuilt**, for the first version: the verbs and their transport
(§ 12), MVS (§ 0, 54 lines written outside the compiler), the fingerprint's
C door and its check at build (§ 5), the dependencies' catalogues and § 16's
rule, the four messages § 2 marks *owed*, the own-component rule of § 3,
the formats' choice (one of § 4's readers replaces the TOML one), and the
question of a fetched module's `extern` headers: a package whose module
binds C reads its header from where? The `--include` search paths are the
command line's (`cli/table.hero`'s `--include`), and nothing today adds a
package's own folder. Unasked by any brief; a question.

**Does it fit one person?** The compiler is 701 modules and 182,687 lines
(`find selfhost -name '*.hero' | wc -l`; `... | xargs cat | wc -l`), so the
Pascal-P4 scale of the seat's charter was passed long ago and the ceiling
that binds is the instruments': every file under its `DECIDED` or §11's
ceiling but `parse/`'s budget row, which the sitting raises by about 103
for U-angle or not at all for U-bare and U-pkg. The verbs, which touch the network and the C,
are unbuilt, and nothing here measures them.

## 20. Productions, and the files a program must have

The catalogue's productions per format are § 4's table (2, 2, 3 over JSON's,
3, and 4 plus a scalar table). U-angle's `Use` is one production, the
reader test's: `Use = "use" [ "<" ident ">" "/" ] ident { "/" ident } [ "as"
ident ] NEWLINE .`; U-bare's and U-pkg's leave § 1's production as it is
(U-pkg's own adds the optional `"pkg" "/" ident "/"`). **Files a program must
have**: none when it uses no package (§ 9, `compat` all ok with no
catalogue anywhere); one file it edits, the catalogue, when it does, under
Go's base (no lock, § 0), and two under Cargo's; plus the placed tree, which
`heroes fetch` writes and nobody edits.

## Verdict

| seat | verdict | design.md | cost | prediction, and the milestone that scores it | what would change it |
|---|---|---|---|---|---|
| compiler-engineer | **object** | §1.1 (simplicity sets the ceiling), §1.7 and Part 5 (no core construct), §1.12 with CLAUDE.md § Precedence rank 3 (robustness), §3.5 (`heroes add`/`heroes fetch`, one binary) | the prototype: **475 lines** (394 less comments): parser 103, discovery and module graph 371, resolver 1; checker, lowering, descriptors, ownership, emitter, runtime **0**; `selfhost/parse/` **8,839 against its budget of 8,737** (`layout/budget`, measured); `check selfhost/main.hero` **+0.095%** instructions (5 of 5 pairs), a warm build unresolved (2 of 4); the verbs, the fingerprint door and § 16's rule unbuilt | at M-package-manager's tag, the commits that land the first version change **0 lines** under `selfhost/check/` and `selfhost/ir/` (`git diff --stat` over them), and, under U-angle, **add between 95 and 165 lines** to `selfhost/parse/` in `code_lines`' unit, whatever they delete there to pay for them (0 under U-bare or U-pkg); falsified by any line in those two directories or a parser cost outside the band | **to approve**: a resolution that states § 16's identity rule (the program's catalogue is the build list, so one package compiles once), a catalogue read all or nothing, a package's C component no module of the program can have (and no component of the program's led by a digit), fixes that cross the mark certain only where nothing answers on the other side, the fingerprint over bytes in C checked at every build, and the `parse/` row raised in writing; **to veto**: a resolution in which `build`, `check` or `run` fetches, or in which a fetched tree can be reached by a `use` the program's own modules also spell (U-bare with the tree placed at `json/`, as the grader places it), since deleting the catalogue then compiles unverified code as the program's own |

- `verdict`: object
- `section`: design.md §1.1 and §1.7 with Part 5 (the ceiling and the core),
  §1.12 (robustness), §3.5 (the one command and its two named verbs)
- `implementation_cost`: 475 layout lines in the prototype that builds and
  runs: `parse/use_mark.hero` 92 and `parse/use_line.hero` +11 (parser,
  over the `parse/` budget by 102); `module/catalogue.hero` 187,
  `module/placed.hero` 34, `module/package_told.hero` 120, `module/paths.hero`
  +13, `modules.hero` +12 (315 of 320), `module/names.hero` +5 (discovery and
  the graph); `resolve/top.hero` +1; zero in `check/`, `ir/`, `emit/` and
  `runtime/`. The other formats' readers 54 to 188; MVS 63 outside the
  compiler; the verbs unbuilt
- `needed_for_self_hosting`: no. The compiler compiles itself with no
  package today, and the second tempo copies the core packages it uses into
  its own tree (the milestone page, panel 234's R13), so building it fetches
  nothing
- `argument`: Not a core construct: once discovery names a file, nothing
  downstream tells a fetched module from the program's own; the prototype
  spends zero lines in the checker, lowering and emitter. The objection is
  to what the proposal leaves unsaid and the compiler must decide first:
  whose catalogue names a dependency's `<x>` (read by text, one package
  compiles twice), what a half-read catalogue means (my prototype compiled
  one at exit 0), which C names a package gets (today's filter makes
  `<json>/parse` the program's `json/parse`, a correct program refused),
  and whether a `certain` fix may cross the mark (mine did). Each is
  measured, each has a stated repair, and the `parse/` budget's raise is
  the price of the author's spelling.
- `prediction`: at M-package-manager's tag, the first version's landing
  commits change 0 lines under `selfhost/check/` and `selfhost/ir/`, and
  under U-angle add 95 to 165 lines to `selfhost/parse/` in `code_lines`'
  unit (the prototype's 103 plus the four messages § 2 marks owed); scored
  by `git diff --stat` and `tests/harness/suite_layout.hero`'s counter over
  the landing's first and last commits
- `condition`: the approve and veto conditions in the table's last cell

report complete
