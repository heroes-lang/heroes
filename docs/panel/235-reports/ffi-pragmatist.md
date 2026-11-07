# Panel 235, the ffi-pragmatist's report

2026-11-07. Written as it went, one section per question. Worktree
`.claude/worktrees/235-ffi-pragmatist/`, detached at `235-base`:
`git log -1 --format='%H %P'` prints `3b071e4f12f2d3357ca2bd3bc338464dc54aa655
328524be86e7b667611610c183cde217063e86d8 9791d4787d4e63e5b7ded2c7c7cf1cf66347b2f7`,
`git rev-parse HEAD^{tree}` prints `f4dbb2b8db0c072de8a01cc6558307af4a4b89e1`.
Compiler built from the seed there (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 9.26` by `/usr/bin/time -p`), `./heroes
--version` printing `heroes 0.6.0` / `sources 1a1e340778db789a`. Apple clang
21.0.0 (clang-2100.4.1.1), arm64-apple-darwin27.2.0. Every C file, program and
script named below is in `.claude/worktrees/235-ffi-pragmatist-scratch/`
(`S` below), with `TMPDIR` set there. No paid session, no API call. **Linux
x86-64, Linux arm64 and Windows x86-64 are unrun** throughout; a sentence about
them says what file it read, never what the platform does.

## 0. First: what the briefs state that I find false or incomplete, and two slips of mine

- **My brief's robustness question lists what building does with a header as
  *a header is compiled*. At `235-base` a cold build also RUNS the header's
  C** when its group binds a `constant`: panel 216's R1 (ratified
  2026-10-17, *`heroes build` may run a program's header code to judge its
  constants*) compiles the module's unit, links it with the runtime and runs
  it (`selfhost/cli/value_run.hero:1-21`). Measured in § 2: a fetched
  header's C constructor wrote two files during `heroes build`, the program
  never run. `grep -n "panel 216\|panel 218" docs/design.md
  spec/heroes-spec.md .claude/rules/*.md` finds no line in design.md, the spec
  or the rules (only a records-rule example): **the permission lives in the
  sitting's file and the compiler's comments, and design.md does not cover
  build-time execution.**
- **00-shared, *a file that is not UTF-8 has no fingerprint today*:** true of
  SHA-256, and incomplete. The compiler already keys any file's BYTES in C,
  `hero_file_key` (`runtime/parts/key.c`, `runtime/hero_compiler.h:38`,
  defect 484), over `hero_file_bytes` (`runtime/parts/os.c:753`). It is the
  cache's polynomial digest, which no tool outside the compiler computes, so it
  cannot be a published fingerprint; but *the runtime would need to read
  bytes* is already done.
- **00-shared and my brief, *today `-I` is the compiled file's directory*:**
  holds (`selfhost/cli/produce.hero:195`), and is incomplete in the way § 1
  turns on: every unit's line is `--include` directories, then a `package`'s
  `-I`, **then the runtime's**, then the compiled file's directory
  (`selfhost/cli/compiling.hero:80-94`, `search_words`, and read on a logged build, § 1a).
- **Two slips of mine, reported rather than hidden.** (1) `brew list
  --versions libgit2 curl sqlite` printed *Downloading Homebrew API data*: an
  unintended network access, and a write into Homebrew's cache outside the
  repository. No `brew` command was run after it. (2) A command of mine copied
  a scratch file to `/tmp/x`. `stat -f '%SB'` read its birth at 17:40:45, ten
  seconds before `date` (17:40:55), and `cmp` matched it to my file, so it was
  created rather than overwritten. I removed it at once (`ls /tmp/x`: *No such
  file or directory*).

## 1. A fetched package that binds C

### 1a. A package that ships its own header

`S/a1`: `main.hero` (`use deps/vec/vec`), `deps/vec/vec.hero` (`extern
"vecmath.h"`, `vm_dot3` and a Heroes wrapper), `deps/vec/vecmath.h`
(header-only C, `static inline int64_t vm_dot3(...)`, the shape §1.11 point 2
and panel 185's R1 prescribe). Each row is the command run in `S/a1` or its
copy:

| route | command | result |
|---|---|---|
| header beside the package's module, as today | `heroes build main.hero` | `check` exit 0; `build` **exit 1**, `error[ffi_missing_header]: vecmath.h is not on this machine's include path` at `deps/vec/vec.hero:2:5` |
| the consumer names the package's folder | `heroes build main.hero --include deps/vec` | exit 0, prints `32` |
| the package writes its path from the consumer's root | `extern "deps/vec/vecmath.h"` (`S/a2`) | exit 0, prints `32`, but the package now names the consumer's layout, which it cannot know |

What the compiler ran, logged by a `clang` wrapper first on `PATH`
(`S/bin/clang`, `S/a1-clang.log`, 11 lines): every unit (`main`, `depsvecvec`,
`library`) compiles under ONE response file whose search is `"-I" "deps/vec"
"-I" "<worktree>/runtime" "-I" "."`. So a fetched root on today's search path
reaches **every** unit, the consumer's own `main` included, and **comes before
the runtime's headers**.

**What that order lets a fetched package do, measured** (`S/h1`): a
`main.hero` adding `9223372036854775807` and `args().len()`; a folder holding
one file, `heroes_standard.h`, which `#include_next`s the runtime's and then
`#define __builtin_add_overflow(a, b, r) ((*(r) = (a) + (b)), 0)`:

| build | run `./<bin> x` |
|---|---|
| plain | `panic: integer overflow at main.hero:4:11`, **exit 134** |
| `--include deps/evil` | `-9223372036854775808`, **exit 0**, no diagnostic at build or run |
| the same file beside `main.hero` (the source's directory, after the runtime) | `panic: integer overflow`, exit 134 |

A file in a directory that precedes the runtime's switched off the overflow
abort in the consumer's own `main`. The unit opens with `#include
"heroes_unit.h"` and reads `#include "heroes_standard.h"` after the guard
(`build/tu-*/depsvecvec.c:2,47`), so any file named like a runtime header in
such a directory replaces it. Today that directory is the author's own flag
or the machine's `.pc`, so the order costs nothing; **a fetched root put there
is a hostile package switching off §1.12 in the program that fetched it.**
`grep` of design.md and `docs/panel/*.md` for the decoy classes finds the
runtime-directory decoy (panel 020, design.md:797-802) and the quoted-header
decoy (§4.19, design.md:2764-2771), and nothing on this search order.

**The route the first version needs, stopped at the C because it is a compiler
change I do not make.** Each unit searches the runtime first, then the root of
the tree its module belongs to, and nothing else of the program:

```
main.c        ... -I <runtime> -I .          # the consumer's root
depsvecvec.c  ... -I <runtime> -I deps/vec   # the package's root, instead of the consumer's
library.c     ... -I <runtime> -I .
```

Compiled by hand from the build's own units and words (`S/a1/scoped/`): three
`clang -c` exit 0, the link exit 0, the binary prints `32`. With the planted
`heroes_standard.h` at the package's root, `clang -M` on the package's unit
lists `deps/vec/vecmath.h` and the RUNTIME's `heroes_standard.h`. A planted
`stdint.h` at the package's root is read by the package's own unit and not by
the consumer's (`clang -M`, both units). So scoping confines what a package's
headers can shadow to the package's own unit, which is C the package already
controls. The rule, in one line, mirrors question 3: **a group's header
starts at the root of the tree its file belongs to, as its `use` lines do.**
Today `compiling.Compiling` carries one `source_dir` for the program
(`produce.hero:195`), so this is a per-unit search, and the work is the
compiler-engineer's to price.

### 1b. A package that names a system library

`S/b1`: `deps/sql/version.hero` (`extern "sqlite3.h" link "sqlite3"`,
`sqlite3_libversion_number`), `deps/ray/ready.hero` (`extern "raylib.h"
package "raylib"`, `IsWindowReady`, `TextLength(text: cstr lent) -> u32`).
`heroes build` exit 0. The program printed `3054000`, `false`, `6` and exited 0.
Logged:

- `package` ran the machine's program as `PKGCONFIG [--cflags] [--libs] [--]
  [raylib]`, never anything of the package's;
- **the link line gains** `"-L/opt/homebrew/Cellar/raylib/6.2/lib" "-lraylib"
  "-lsqlite3"` (`build/*/link-words.rsp`); `otool -L` names
  `/opt/homebrew/opt/raylib/lib/libraylib.620.dylib` and
  `/usr/lib/libsqlite3.dylib`;
- raylib's `-I/opt/homebrew/Cellar/raylib/6.2/include` reaches only the unit
  whose group names it (two response files, one with it, defect 572's rule).

A dependency's `link` is a word on the consumer's link line, so I tried six
hostile values, each a `main.hero` in `S/l1`. `-fplugin=/tmp/x.so` is refused
`option_like_name`. `:libz.a` is refused `machine_locked_path`. `../../evil`
and `/usr/lib/libz.dylib` are refused `unwritable_name`. `@words.rsp` and `z
-lz` are refused `ffi_missing_library`. Every one exits 1; `link "z"` builds
and prints `1.2.12`.

### 1c. What a fingerprint must cover for a reproducible build

- **Every file under the package's root, not the `.hero` and `.h` files
  alone.** A header reads any file by any name. In `S/e1`, `#embed` copied a
  file of the build machine into the program (below), and `clang -M` lists
  the files a unit reads, `token.txt` included.
- **A C file:** no route compiles one. §4.19 says *how it is compiled is
  undecided*, and panel 234's R6 says *none ships platform C of its own*. So a
  `.c` in a package is inert bytes, and C a package needs is header-only
  (`static inline`), covered as a file.
- **A prebuilt object:** no route links one from a package. A `link` value
  holding a path is refused (above), and `--library <dir>` is the consumer's
  flag. It is bytes the fingerprint covers as bytes, which is § 4's question.
- **The system library is never covered.** It is the machine's, as the row-83
  page says (*a library a program binds … stays the program's own to
  install*). The build is reproducible as far as the package's bytes go;
  sqlite's version is this Mac's `/usr/lib` one.

**One C name in two packages needs no Cargo `links` key** (measured in
`S/two`). Two packages' headers each define `static inline int64_t
util_version` with different bodies: the program builds and prints `1 2`, exit
0, each package keeping its own. Two headers declaring one external name two
ways: `error[ffi_declared_two_ways]` at exit 1, before the link (panel 202's
C1).

## 2. Robustness at the boundary: does fetching or building run the dependency's code?

| step | measured on this Mac | runs the package's code? |
|---|---|---|
| `git clone` | a `post-checkout` hook in the source repository writes 1 line on its own checkout, 0 during `git clone`; the clone holds 0 hooks that are not `.sample` (`S/crlf`) | no |
| git argv | `git clone "--upload-pack=touch …; git-upload-pack" file://… d1`: the marker file written, exit 0. After `--`: *fatal: repository '--upload-pack=…' does not exist*, exit 128 | **yes without `--`**: a URL a dependency's catalogue writes is the consumer's argv |
| `ext::` transport | `git clone "ext::sh -c touch% …"`: *transport 'ext' not allowed*, exit 128 (git 2.56.0's default) | no |
| `file://` from a catalogue | `git clone -- file://…/src d4`: exit 0 | no, but it reads a local repository of the consumer's machine |
| symlink in the tree | a committed `pkg.h -> S/secret/token.txt`: the clone holds the link and `cat dst/pkg.h` prints the secret; `clang -M` lists `pkg.h`, not its target (`S/link`) | no, but the build would read outside the package |
| two names one disk | `Mod.hero` and `mod.hero` in one commit: git warns *the following paths have collided*, exit 0, one file in the tree | no, but which module compiles depends on the disk |
| static index by `curl` | `S/tools/fetch_index.sh`, § 4 | no |
| `build`, header compiled | `#embed "S/secret/token.txt"` in a header: build exit 0 with one clang warning (`-Wc23-extensions`), the binary prints `BUILD-MACHINE-SECRET-4711`. `#include` of the same file: refused, and clang's message prints the file's first line | no, but it reads any file, into the binary or into a CI log |
| `build`, header **run** | a header with `#define PKG_LIMIT 3`, a `constant PKG_LIMIT: i64` bound, and an `__attribute__((constructor))` writing a file (`S/x1`) | **yes**: cold `build` 2 files, cold `--emit-c` 2, cold `test` 2; warm `build` 0, `check` 0, the same group without the `constant` 0 |
| `package "<name>"` | the machine's `pkg-config --cflags --libs -- <name>` (logged) | no: the package names a `.pc`, whose words pass `package_words.hero`'s allow-list |

The run is bounded at `LIMIT` 30 s and `CAP` 1,048,576 bytes
(`value_run.hero:53-58`). My grep of `selfhost` and `runtime` for `sandbox`,
`seccomp`, `sandbox-exec` and `unshare` finds no sandbox. Panel 216 measured
the same constructor and put the question to the author for **the program's
own** headers, and the author answered yes (its § Author's verdict). A fetched
package's header is not the program's own, and Go's base states the opposite
goal in the survey's row 2.1 probe 7: *neither fetching nor building code
will let that code execute*. **The sitting cannot inherit panel 216's yes in
silence: it either extends it to fetched code by name, Cargo's stance
(`build.rs`), or keeps Go's goal and says how the probe avoids executing
fetched bytes.** What I measured for the second:

- clang's AST names `ConstructorAttr` (3), `DestructorAttr` (1) and
  `SectionAttr … "__DATA,__mod_init_func"` (1) in a test header (`S/ast`), so
  load-time code of those shapes is visible before anything runs. That list is
  an enumeration, a premise about the world (CLAUDE.md § 11). One shape
  outside it, a header defining `malloc`, ran nothing at build or at run on
  this Mac (`S/x2`: 0 marker lines both times, `_malloc` defined in the
  binary); whether another C library name does is **a question, unrun**;
- the project already runs untrusted programs under `sandbox-exec` on macOS
  and `bwrap` on Linux, refusing on Windows where neither exists
  (`tests/harness/thesis_sandbox.hero:1-50`). Whether the probe runs there is
  **unrun**.

**What a hostile package can make the consumer's build do, today's compiler,
the package placed as § 1a places it:** run its C for 30 s per probe at a
cold build; copy any file the building user can read into the binary
(`#embed`) or one line of it into the build's error output (`#include`); and,
put on today's search path, switch off the language's runtime checks in the
consumer's own units. Fetching it runs nothing, given `--` and an allow-list
of `https://`.

**Conditions the first version owes at the boundary**, each measured above:

- **C1.** A fetched package's root is searched after the runtime's, by its own
  module's units alone, in place of the consumer's root (§ 1a).
- **C2.** Every file a fetched package's unit reads lies under that package's
  root, the runtime, or the toolchain's own directories. This is judged on the
  `clang -M` listing before any compile. That listing exits 0 and prints no
  content (measured on the `#include` of the secret), and the compiler already
  asks it (`build/reads/`) and keeps it (`build/tu-*/main.deps.txt` lists
  `token.txt` with its key).
- **C3.** At fetch, before writing: a symlink (mode 120000) and a gitlink
  (160000) are refused, and so are two names that collide under case folding
  and a name holding `..`, `/` at its start, `\` or `:`. C2 cannot catch a
  symlink, because `-M` names the link and not its target.
- **C4.** Every word a package controls goes to `git` and `curl` after `--`,
  as `pkg-config` already gets it (`libraries.hero:284`). A source is
  `https://` by allow-list, the way `package_words.hero` judges a `.pc`'s
  words. This refuses `file://` and `ext::` by construction, not by git's
  default.
- **C5.** The build-time probe's answer to fetched headers, ruled by the
  sitting or the author as above, never inherited.

## 3. Question 13: the C under the first version

| | programs run (`git`, `curl`) | libraries bound (libcurl, libgit2) |
|---|---|---|
| new C | none: `process.run` (`process.hero:109`) already runs `pkg-config` (logged, § 1b) and `curl` (`refresh.hero:80-84`) | `S/lib13/fetch_curl.c`, 30 lines (`wc -l`) against the SDK's `curl/curl.h`: `clang -std=gnu11 -Wall -Werror … -lcurl` exit 0; fetched a `file://` URL byte for byte (`cmp`); `CURLOPT_PROTOCOLS_STR "https,file"` refused `dict://` (code 1). `#include <git2.h>`: **fatal error: 'git2.h' file not found** |
| the seed's one line | unchanged: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` | gains `-lcurl`, and for libgit2 its `-I`/`-L`/`-lgit2`; the seed then includes `curl/curl.h` and `git2.h`, so every machine that builds it needs their headers |
| this Mac | `/opt/homebrew/bin/git` 2.56.0; `/usr/bin/git` is the developer tools' shim, 200,560 bytes with 78 links, resolving to `git version 2.54.0 (Apple Git-157)` with Xcode installed; `/usr/bin/curl` 8.7.1, a universal binary | libcurl in the SDK (`libcurl.tbd`, `curl.h`), linked as `/usr/lib/libcurl.4.dylib` (`otool -L`); libgit2 absent (`pkg-config`: *Package 'libgit2' not found*) |
| Linux x86-64, arm64 | **unrun**. `ci.yml:359` installs `clang lldb pkg-config libsqlite3-dev libcurl4-openssl-dev` and no `git` or `curl` package; whether the runner image carries them is unread | **unrun**. libcurl's headers are already installed there (`ci.yml:359`); libgit2's are not named in `ci.yml` |
| Windows x86-64 | **unrun** | **unrun**. `ci.yml:495` states *sqlite, curl and raylib are not there*, so a seed binding libcurl would not link on that leg as the workflow stands |
| row 83 | `build`, `check` and `run` need neither program when fetched code is present (§ 5), so row 83 holds for them; `fetch` needs `git` for a git source and `curl` for the index | the prebuilt `heroes` carries a TLS client, or links one the platform may not have (macOS: the system dylib, measured; Linux and Windows: unrun); a git or curl CVE is then fixed by a `heroes` release, not by the machine's update |

**A route nobody listed:** an index that serves each file at its own address,
with a listing of every file's SHA-256 (JSR's `<version>_meta.json` shape,
survey Table 3). The first version then needs `curl` alone for the index: no
`git`, no `tar`, no zip and no zlib. It is measured in § 4 with `/usr/bin/curl`
over `file://`. A git source still needs `git`.

**Recommendation for 13: programs, not libraries, in the first version.** It
adds no C, leaves the seed's one line and its Windows leg as they are, and
keeps the network out of the `heroes` binary. Q14's fixed interface is then
the catalogue's entry and the verified tree, never the transport. **The
second version** binding libcurl in a core package copied into the compiler's
tree meets the same seed question, which that row must answer on all four
legs. That is unrun.

## 4. Questions 5 and 8: fingerprints over which bytes, and the index

**Go's `h1`, recomputed from its own data** (`S/tools/h1.sh`): for each file,
its SHA-256 and its name, in byte order, as `<hex>  <name>\n`, then the
listing's SHA-256 in base64. Over `github.com/google/uuid v1.6.0`'s cached zip
(31 files) it gives `h1:NIvaJDMOsjHA8n1jAhLSgzrAzy1Hgr+hNrb57e+94F0=`, **equal
to Go's recorded `v1.6.0.ziphash`**. The `go.mod` line, recomputed from the
cached `.mod`, is `TIyPZe4MgqvfeYDBFedMoGGpEw/LqOeaOT+nhxU+yHo=`.

**The same fingerprint from git, the brief's one fetch:** `timeout 60 git -c
core.autocrlf=false clone --depth 1 --branch v1.6.0 --
https://github.com/google/uuid` ran at `real 0.66` and checked out commit
`0f11ee6918f41a04c201eceeadf612a377bc7fbc`. Its working tree, its `git
archive`, and its blobs (`S/tools/h1blobs.sh`, `git ls-tree -r` and `git
cat-file blob`) each give **the same `h1` as Go's proxy zip**, and the
listings are identical (`diff`). One fingerprint verifies a package whichever
way it came.

**What an archive's own bytes are a function of** (Cargo's `cksum`, *A SHA256
checksum of the `.crate` file*): one commit, five producers.

| producer | SHA-256 prefix |
|---|---|
| `git archive --format=tar.gz` | `d02a4a4865c51071` |
| `\| gzip -n -6` | `d02a4a4865c51071` |
| `\| gzip -n -9` | `2beecaeab51a32b2` |
| `git archive --format=zip` | `fa505496153aac03` |
| Go's proxy zip | `d0f02f377217f427` |

The tar.gz and the zip each unpack to the same `h1`. An archive's hash holds
only where the index keeps the uploaded bytes immutably, as crates.io does. It
verifies no git source, and a second version that changed transport would
change every catalogue's fingerprint.

**Over which bytes, for git: the committed bytes, never the checkout.**
`S/crlf`, a two-file repository cloned twice:

| clone | `main.hero` on disk | `h1` of the working tree | `h1` of `git archive` | `h1` of the blobs |
|---|---|---|---|---|
| `core.autocrlf=false` | 29 bytes | `P6kw5di…` | `P6kw5di…` | `P6kw5di…` |
| `core.autocrlf=true` | 31 bytes | `wBq+eYY…` | `wBq+eYY…` | `P6kw5di…` |

A repository's own `.gitattributes` saying `* text eol=crlf` moves the working
tree and `git archive` the same way (`attrs`: 31 bytes). So the fetch reads
`git ls-tree -r -z` and `git cat-file blob`, and C3's refusals read that same
listing.

**Over which names:** every regular file under the package's root (§ 1c),
relative to it. Whether the names carry a `name@version/` prefix, as Go's do,
is the sitting's to choose. My index test used none, so its `h1`
(`1WRZV7W…`) differs from Go's for the same 31 files.

**Can the compiler's own SHA-256 serve?** For text, yes. `S/hsum/main.hero`
uses a copy of `selfhost/cli/sha256.hero` and builds at `-O0`. Over the uuid
tree it prints
`348bda24330eb231c0f27d630212d2833ac0cf2d4782bfa136b6f9edefbde05d`, equal to
`shasum -a 256` of the same listing. For a file that is not text, no: with a
PNG header added, it prints `no fingerprint: logo.png is not_text`, exit 1,
where `shasum` gives `044dbe64b84ec81c…`. Its cost: over the compiler's 701
`.hero` files, 8,259,507 bytes, `real 1.30 user 1.28` (best of three after the
first was discarded, load average 3.51), against `shasum`'s `real 0.05`. That
is fine at fetch, and slow for every build. **What the runtime would need:**
nothing to read bytes, since `hero_file_bytes` exists. The missing part is a
SHA-256 over them. Three routes:

- (a) C beside `hero_file_key`, defect 484's precedent and its reason, speed.
  It travels in every program's runtime: `nm -m x2bin` finds `hero_file_key`
  in a user program;
- (b) Heroes over a `[u8]` filled through a binding (spec § 13's
  `counted_by` lend), unbuilt by me;
- (c) refusing non-text files in a package, which is simplest and also
  refuses binary test data a C binding may need.

I recommend (a) for robustness and speed, with its size in every runtime
measured before landing.

**Can a static index be read by the first version with `curl` and checked
with that fingerprint? Yes, measured.** `S/idx/site`: per package a
`versions` file (`1.6.0 h1:…`), per version a `list` (the `h1` listing), and
each file at `files/<path>`. `S/tools/fetch_index.sh` does the first
version's work with `/usr/bin/curl -fsS --proto '=https,file' -- <url>`. It
fetched 31 files, checked the listing against the catalogue's `h1` and each
file against the listing, and the tree equals the git clone's byte for byte
(`diff -r`). Two hostile cases:

- one byte appended to `uuid.go` on the index: `refused: uuid.go does not
  match its listing`, exit 1. **But my script had written 25 files before
  it**, so the fetch must fill a staging directory and move it into place
  whole;
- a hostile index that supplies a listing naming `../../../escaped.txt` with
  its own matching `h1`: refused before writing, and no escaped file exists.

## 5. Question 5: whose fingerprint is trusted, and what a hostile index or a moved tag does

Under each base, as the survey states it (rows 2.1, 2.2, Table 3; read, not
run):

- **Go**: the consumer computes `h1` and checks it against `sum.golang.org`,
  a transparent log separate from the proxy, so a hostile proxy alone cannot
  pass a changed tree, even at the first fetch;
- **Cargo**: the index's `cksum` is the registry's statement, so a hostile
  index can pair a changed archive with a matching sum, and nothing else
  checks it. A git source has a commit and no checksum;
- **a first fetch recorded** (Zig's hash in the manifest; the reader test's
  *the publisher's fingerprint*): whoever answers the first `heroes add` is
  trusted once, and every later fetch is checked against what was written.

Measured for a moved tag (`S/tag`). `v1.0.0` was recorded at commit
`08e5299d78dd` with `h1:A7YQH/J…`, then moved by `git tag -f` to a commit
returning `2`. Fetched again, `v1.0.0` is commit `b91a1ce7cfcc`,
`h1:E8eKl/4…`, a **mismatch**. A catalogue holding the fingerprint refuses it.
One holding the tag alone builds a different program in silence.

**For the first version:** the catalogue always holds the fingerprint, and for
git the commit as well. A first `add` from git is trust on first use, and from
the static index it trusts the index. Both are written as such in the
resolution, and the checksum log is row 78's, not the first version's.

## 6. Question 18: may `build`, `check` or `run` fetch?

Go's `go build`, with `go.sum` holding the module's line, an empty module
cache and `GOPROXY=off` (`S/gob`), printed `go: downloading
github.com/google/uuid v1.6.0`, then *module lookup disabled by GOPROXY=off*,
exit 1. **The base's build reaches the network for a pinned module.** That
matches `refresh.hero:9`'s rule, *the network is admissible only where the
answer is pre-recorded*: the catalogue's fingerprint is that answer.

**My recommendation is stricter: `build`, `check`, `run` and `test` never
fetch.** A missing or mismatched package is an error naming `heroes fetch` as
its `certain` fix. Only `heroes add` and `heroes fetch` reach the network. Two
reasons, the first an argument and not a measurement:

- a build that may fetch has a duration that depends on the network, and the
  latency instrument times `heroes build` (`.claude/rules/verification.md`
  § The latency ceiling's instrument);
- C2 and C3 refuse before writing, and a fetch inside `build` would write into
  the tree a build is reading.

This is Go 1.16's *build commands … no longer modify `go.mod` and `go.sum`*,
taken one step further (survey, row 2.1). Go's own build still downloads.

## 7. Question 15, with the historian: what row 78 must deliver, and what the first version must already respect

From the C boundary's side alone; the historian owns the precedents.

- **Immutability of a published version**: crates.io, JSR and npm, survey
  Table 3. The first version already relies on it, because the catalogue's
  fingerprint refuses any change (§ 5).
- **A withdrawn version keeps building where it is pinned**: Cargo's yank,
  Go's `retract`. The first version needs nothing for it, since an exact pin
  with its fingerprint never asks the index which versions stand.
- **A transparency log or signatures over the index**: Go's checksum database,
  Hex's RSA over the payload. This is what turns § 5's trust on first use
  into a check. The first version's catalogue format must leave room for it
  and need nothing from it.
- **Names**: the index name is the package's identity, and the alias is the
  consumer's. C1's per-package root is keyed by the package, never the alias.
- **What a package binds**: the index records each package's `link` and
  `package` words, so `add` can say what the machine must install. No
  `links` uniqueness key is owed (§ 1c).
- **C3's refusals, applied at publish time too**, so no index ever serves a
  tree a consumer's fetch refuses.

## Verdict row

| seat | verdict | sections | cost or delta | prediction, and the milestone that scores it | condition that changes it |
|---|---|---|---|---|---|
| ffi-pragmatist | **object** until the resolution carries C1 to C5. **Veto** held on one route: a fetched package's root on the program-wide search path ahead of the runtime's (§ 1a: the overflow abort switched off in the consumer's `main`, exit 0 against 134) | §1.11, §1.12, §4.19 (the header search, the probe's mechanism); design.md does not cover build-time execution (panel 216 only) | per-unit header search (`compiling.hero`, one `source_dir` per program today); a `-M` containment check over a listing the compiler already makes; fetch-time refusals; SHA-256 over bytes (route (a), C in every runtime, size unmeasured); the first version needs no new C for transport | **(P1)** Under C1, a fetched copy of `examples/ledger/db/sqlite.hero` (§4.19's rung 3) and a fetched package with a header of its own build with no `--include`, and a `heroes_standard.h` planted at the package's root appears in no unit's `deps.txt`. **(P2)** Under C5 read as Go's goal, a fetched header with a constructor that binds a `constant` writes nothing during a cold `heroes build`. Both scored at M-package-manager's first version, row 77's close, on the four CI legs | C1 to C5 adopted makes it **approve**. If the author extends panel 216's yes to fetched headers by name, C5 is met as Cargo's stance and recorded. The veto lifts only if the runtime's headers come first on every unit's line, measured |

report complete
