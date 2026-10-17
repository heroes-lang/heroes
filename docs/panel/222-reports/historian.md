# Panel 222, the historian: how self-hosting compilers that keep a bootstrap tell a stale stage

Written 2026-10-17 from about 13:00 by the historian seat, from the shared brief
(`docs/panel/222-briefs/00-shared.md`), `docs/roadmap/milestones/M-bounded-seed.md` and panel 219's
historian and critic reports. Panel 219's precedents are used here only where they were read again
today, and the line says so.

**What this seat ran, and what it did not.** Its instrument is the primary sources. Every page
below was read on 2026-10-17 through the session's fetch tool. That tool passes a page through a
summarising reader that limits a quotation to 125 characters, so a quotation is marked as the tool
returned it, and a count it made over a long listing is marked *approximate*. This seat built
nothing and measured nothing on the tree, and had no shell. Every Heroes number below is the
brief's or `.claude/rules/verification.md`'s as cited, *unrun here*. No paid run. *Heroes of code*
was not needed as an index for these rooms, and no claim rests on it.

**Marks.** *verified*: the named page, file or API answer was read in this session and says what
the line says. *unverified*: only a search summary or a secondary page carried it. *inference*: a
reading of verified facts that no page states.

**The Heroes failure the question is about, as the brief states it.** The compiler that judges is
the stage built by clang from `seed/heroes.c`. Under the 2026-10-14 policy the seed lags behind
`selfhost/`, so that compiler holds the seed's logic, yet its file is newer than every source. The
modification-time check of `.claude/rules/verification.md` § A suite is the last judge, layer 1,
therefore passes it. That section records what one stale judge cost on 2026-09-18: 31 minutes,
two full nets, and diagnostics that pointed away from the cause.

## verdict

**Advisory.** Each route is approved or objected to below.

| route | reading | the precedent it rests on |
|---|---|---|
| (a) the compiler carries a digest of the sources it was built from, the hook compares | **approve**, on condition the digest is of **content** rather than a commit, and covers exactly what the hook can hash | Go since 1.10 (2018-02-16): staleness *"purely based on the content of source files ... and metadata stored in the compiled packages. Modification times are no longer consulted"*; build IDs, a hash of the inputs, stored in the binary |
| (b) the seed carries the digest of what it was emitted from; stage 1 never judges, by rule and by check | **approve the check, object to the rule alone** | V's `vc/v.c` carries `V_COMMIT_HASH`, and V's version prints both hashes when they differ; GHC carries its `Stage` in the binary; no precedent found keeps stage 1 out of judging by a written rule alone (Zig, Go and GHC do it by construction) |
| (c) the gate always builds two stages, the hook checks only the stage-2 artefact | **approve as the build discipline, object as the detector** | every precedent read builds through its stages on every clean build (Go, Zig, Nim, V, OCaml, GHC, GCC, Free Pascal); none tells a stage by its path or its time |
| (d) nothing: the policy written, the hook left on modification times | **object** | the precedents that check nothing about bootstrap freshness (Zig, Gambit) never leave a stage-1 binary where their tests look for the judge; Go retired modification times outright |
| the route nobody listed: a stamp file beside the binary (Rust's `BuildStamp`, Nim's `bin/nim_csources_<hash>`) | **object for Heroes** | the stamp has to be written by the one builder, and Heroes' one-line bootstrap is a bare `clang` that writes no stamp (*inference*) |

## precedents

### Go: a content digest stored in every binary, and three toolchains on every build

- **Modification times retired.** The Go 1.10 release notes, § Build & Install: *"The `go build`
  command now detects out-of-date packages purely based on the content of source files, specified
  build flags, and metadata stored in the compiled packages. Modification times are no longer
  consulted or relevant."* They add that the old advice to force a rebuild with `-a`, needed
  *"in cases where the modification times were misleading"*, *"is no longer necessary"*
  (https://go.dev/doc/go1.10). Go 1.10 was released on 2018-02-16
  (https://go.dev/doc/devel/release). **verified**
- **What the binary carries.** `cmd/go/internal/work/buildid.go`, as returned: the action ID is
  *"a hash of the inputs to the action that produced the packages or binary"*. The content IDs are
  *"hashes of the export and object files"*. Both are *"truncated when stored in packages and
  binaries"*, and the action ID *"allows installed packages and binaries to serve as one-element
  cache entries"* (https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/work/buildid.go).
  **verified**
- **The bootstrap goes through stages, and a check follows.** `cmd/dist/build.go`'s
  `cmdbootstrap` prints *"Building Go toolchain2 using go_bootstrap and Go toolchain1."*, then
  *"Building Go toolchain3 and commands using go_bootstrap and Go toolchain2."*, then
  *"Checking command staleness for %s/%s."*, and it calls
  `checkNotStale(toolenv(), goBootstrap, toolsToInstall...)`. The comment on toolchain1, as
  returned: *"The toolchain1 we built earlier is built from the new sources,"* *"but because it
  was built using cmd/go it has no build IDs."* `checkNotStale` runs `go list` with a
  `{{if .Stale}}` template and ends in `fatalf` *"unexpected stale targets reported by %s list"*
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/dist/build.go). **verified**. The
  tool found no code comparing toolchain2 with toolchain3 byte for byte, only a comment saying that
  toolchain3 *"has converged"*. **verified**
- **The revision and a dirty flag.** Go 1.18 (released 2022-03-15): *"The `go` command now embeds
  version control information in binaries. It includes the currently checked-out revision, commit
  time, and a flag indicating whether edited or untracked files are present"*, read with
  `go version -m` (https://go.dev/doc/go1.18). The keys are `vcs.revision` and `vcs.modified`,
  *"true or false indicating whether the source tree had local modifications"*
  (https://pkg.go.dev/runtime/debug). **verified**
- **How it fails when it fails.** The content check gives false reds when the environment differs
  between the build and the check. Issue 46347, opened 2021-05-24 and closed 2021-05-27: a
  `CGO_CFLAGS` set during `./make.bash` made `net` read *"unexpectedly stale"*
  (https://api.github.com/repos/golang/go/issues/46347). Issue 53865, *"cmd/dist: toolchain3 fails
  in external linking mode due to stale targets"*, was opened 2022-07-14 and closed 2023-11-13
  (https://api.github.com/repos/golang/go/issues/53865). **verified**. A search summary names
  further issues of the same error on single builders (35749, 49691, 52009, 65486); **unverified**,
  since they were not opened.
- **Refresh rhythm.** None is committed. The bootstrap is a downloaded Go: *"Go version 1.N will
  require a Go 1.M compiler, where M is N-2 rounded down to an even number"*, set by
  `GOROOT_BOOTSTRAP` or the `go` on `$PATH`. The page describes no check of that toolchain's
  version (https://go.dev/doc/install/source). **verified**

### Rust: stamps compared by content, stage 2 shipped, `cfg(bootstrap)` for what stage 0 lacks

- **Stage0 is a pin, verified by SHA-256.** `src/stage0` is *"generated by `./x.py run
  src/tools/bump-stage0`"*; today it reads `compiler_date=2026-09-29` and `compiler_version=beta`,
  followed by about 590 lines mapping each artefact to a 64-hex checksum
  (https://raw.githubusercontent.com/rust-lang/rust/master/src/stage0). In `download.rs`, *"For the
  stage0 compiler, put special effort into ensuring the checksums are valid"*. A mismatch panics
  *"failed to verify"*, and verification is skipped for CI downloads
  (https://raw.githubusercontent.com/rust-lang/rust/master/src/bootstrap/src/core/download.rs).
  **verified**
- **Staleness by a stamp's content.** The downloaded stage0 is kept under a stamp keyed on that
  date: `BuildStamp::new(&bin_root).with_prefix("rustc").add_stamp(stamp_key)`. If the stamp is not
  up to date, the sysroot is removed and downloaded again (download.rs above). `is_up_to_date`:
  *"It is considered up-to-date if file content matches with the stamp string"*. The other helper,
  `clear_if_dirty`, *"Clear out `dir` if `input` is newer"*, does use modification times
  (https://raw.githubusercontent.com/rust-lang/rust/master/src/bootstrap/src/utils/build_stamp.rs).
  **verified**
- **Which stage is the product.** *"The `stage2` compiler is the one distributed with `rustup` and
  all other install methods"*. The build sets `--cfg bootstrap` when building with stage0, so new
  features sit behind `cfg(not(bootstrap))`
  (https://rustc-dev-guide.rust-lang.org/building/bootstrapping/what-bootstrapping-does.html).
  For everyday work the guide recommends stage 1; of a stage-2 build it says *"You almost never
  need to do this"* (https://rustc-dev-guide.rust-lang.org/building/how-to-build-and-run.html).
  **verified**
- **The explicit opt-out, and its warning.** `--keep-stage-std` bypasses *"bootstrap's cache
  invalidation"*. It *"may produce compilers that don't work"*, and the fix is to remove the flag
  and rebuild (https://rustc-dev-guide.rust-lang.org/building/suggested.html). **verified**
- **Cargo is moving off modification times.** `-Z checksum-freshness` *"will replace the use of
  file mtimes in cargo's fingerprints with a file checksum value. This is most useful on systems
  with a poor mtime implementation, or in CI/CD"*. It is still unstable, tracked by cargo#14136
  (https://doc.rust-lang.org/cargo/reference/unstable.html). **verified**
- **Refresh rhythm.** Once per release, by the release process: on Tuesday, *"Run this to update
  the bootstrap compiler to the beta you created yesterday"*, and on Friday a PR to the beta branch
  updating stage0 (https://forge.rust-lang.org/release/process.html). *"Rust releases every six
  weeks, like clockwork"* (https://doc.rust-lang.org/book/appendix-07-nightly-rust.html).
  **verified**

### GHC: modification time and digest together; the binary names its own stage

- **Stages, and the tests run on stage 2.** Hadrian's stage1 compiler sits in `_build/stage0/bin/ghc`
  and stage2 in `_build/stage1/bin/ghc`. `--freeze1` means *"do not rebuild it even if some of its
  source files are out-of-date"*, and it *"may lead to incorrect build results"*
  (https://raw.githubusercontent.com/ghc/ghc/master/hadrian/README.md, the GitHub mirror; the
  GitLab page was refused by its bot wall). The testsuite's `--test-compiler` is *"Default=stage2"*
  (https://raw.githubusercontent.com/ghc/ghc/master/hadrian/doc/testsuite.md). **verified**
- **The rebuild rule.** Hadrian sets `shakeChange = ChangeModtimeAndDigest`
  (https://raw.githubusercontent.com/ghc/ghc/master/hadrian/src/Main.hs). Shake documents it as
  *"A file is rebuilt if both its modification time and digest have changed"*: the time is checked
  first, then the digest. Shake's own default is `ChangeModtime`
  (https://raw.githubusercontent.com/ndmitchell/shake/master/src/Development/Shake/Internal/Options.hs).
  **verified**
- **The binary carries its stage and its commit.** In the GHC API's `Config` module,
  `cStage = show (STAGE :: Int)` and `cProjectGitCommitId` is a 40-hex string
  (https://hackage-origin.haskell.org/package/ghc-lib-parser-0.20190703/docs/src/Config.html).
  An installed 9.2.5 prints `("Stage","2")` and `("Project Git commit id","74ca6191…")` in
  `ghc --info` (https://discourse.haskell.org/t/5365, 2022-11-27). Herbert Valerio Riedel wrote on
  2015-07-04: *"since 7.10 we embed the Git commit id into the source-tarball as well as into the
  generated ghc binary"* (https://mail.haskell.org/pipermail/ghc-devs/2015-July/009346.html).
  **verified**
- **Stage 1 is kept from some work by construction.** Ömer Sinan Ağacan, ghc-devs, 2019-10-18:
  *"Stage 1 compiler doesn't have interpreter, and doesn't run plugins or TH"*
  (https://mail.haskell.org/pipermail/ghc-devs/2019-October/018251.html). **verified** as a
  developer's statement, not as a project rule.
- **Bootstrap gate and rhythm.** `configure.ac` sets `MinBootGhcVersion="9.10"` and fails with
  *"GHC version $MinBootGhcVersion or later is required to compile GHC."*
  (https://raw.githubusercontent.com/ghc/ghc/master/configure.ac). **verified**

### Zig: no freshness check on `zig1.wasm`; the CI builds through it and through a fresh one

- **What forces a refresh.** Andrew Kelley, 2022-12-07: *"The wasm binary only needs to be updated
  when a breaking change or new feature affects the compiler when building itself"*, by
  `zig build update-zig1`. The chain is zig1, then zig2 (*"zig1 translates the Zig compiler source
  into zig2.c"*), then `zig2 build` (https://ziglang.org/news/goodbye-cpp/). **verified**
- **zig1 cannot judge, by construction.** `src/dev.zig`'s `bootstrap` env is *"zig1 features:
  `-ofmt=c` only"*, with six features supported and every other refused
  (https://raw.githubusercontent.com/ziglang/zig/master/src/dev.zig). **verified**
- **No check on the bootstrap's age.** Neither `CMakeLists.txt` nor `build.zig` compares
  `stage1/zig1.wasm` with the sources. `zig2.c` is rebuilt by CMake on `DEPENDS zig1
  "${ZIG_STAGE2_SOURCES}"`, a list *"Needed because we use cmake, not the zig build system, to
  build zig2.o."*
  (https://raw.githubusercontent.com/ziglang/zig/master/CMakeLists.txt,
  https://raw.githubusercontent.com/ziglang/zig/master/build.zig). **verified**
- **The version is a commit.** `zig version` reads `git describe` and formats
  `{version}-dev.{commit_height}+{commit_id}` (build.zig above; CMake appends
  `-dev.${GIT_COMMITS_AFTER_TAG}+${GIT_COMMIT}`). **verified**
- **Its build cache uses stat as a fast path and the digest as the truth.** In
  `lib/std/Build/Cache.zig`, a file whose size, mtime and inode all match is taken as unchanged.
  Otherwise, or on a *"problematic"* timestamp (*"The actual file has an unreliable timestamp,
  force it to be hashed"*), it is hashed, and only a digest mismatch counts as a change
  (https://raw.githubusercontent.com/ziglang/zig/master/lib/std/Build/Cache.zig). **verified**
- **The CI exercises both directions on every run of its release leg**
  (https://raw.githubusercontent.com/ziglang/zig/master/ci/x86_64-linux-release.sh), in this order:
  1. `cc -o bootstrap bootstrap.c; ./bootstrap; ./zig2 build -Dno-lib`, the committed `zig1.wasm`
     to zig2 (`bootstrap.c` runs `./zig-wasm2c stage1/zig1.wasm zig1.c`,
     https://raw.githubusercontent.com/ziglang/zig/master/bootstrap.c);
  2. *"Ensure that stage3 and stage4 are byte-for-byte identical."* and
     `diff stage3-release/bin/zig stage4-release/bin/zig`;
  3. *"Ensure that updating the wasm binary from this commit will result in a viable build."*,
     `zig build update-zig1`, then the build again through it.

  **verified**
- **Refresh rhythm.** The commits touching `stage1/zig1.wasm`, read in two parts, number about 54,
  from 2022-12-10 to 2025-11-22: 5 in December 2022, about 24 or 25 in 2023, 10 in 2024 and 14 in
  2025, with one entry cut at the join
  (https://api.github.com/repos/ziglang/zig/commits?path=stage1/zig1.wasm&per_page=100).
  **verified, count approximate**. Panel 219 read the same listing as 53 dated, with 9 in 2024
  and 25 in 2023, so the two readings differ by one in each of those years. Most titles say only
  *"update zig1.wasm"*. Those that give a reason name a language change the compiler's own build
  used, or a C compiler's compatibility: *"update zig1.wasm to `@call` changes"*, *"to
  builtin.Type field changes"*, *"to overflow arithmetic builtin changes"*, *"after renaming
  "@XtoY" to "YfromX""*, *"for MSVC compatibility"*. **verified**

### OCaml: stale bootstrap detected by failure and by magic numbers; a version once forced refreshes

- **Detected by failure.** In the root `Makefile`, above `bootstrap:`: *"The compilation of ocaml
  will fail if the runtime has changed."* and *"Never mind, just do make bootstrap to reach
  fixpoint again."* The `compare` target prints *"Fixpoint reached, bootstrap succeeded."* or
  *"Fixpoint not reached, try one more bootstrapping cycle."*
  (https://raw.githubusercontent.com/ocaml/ocaml/trunk/Makefile). **verified**
- **Every compiled file carries a magic number.** On a mismatch, `utils/misc.ml`'s
  `explain_unexpected_error` says *"This seems to be a %s (%s) for %s version of OCaml."*, the slot
  being *"an older"* or *"a newer"*
  (https://raw.githubusercontent.com/ocaml/ocaml/trunk/utils/misc.ml). **verified**
- **The procedure and the CI.** A bootstrap is required *"when something changes in the runtime
  system"*. The bootstrap is committed *"separately, after that commit"*. Inria's CI runs a script
  (https://github.com/ocaml/ocaml/blob/trunk/BOOTSTRAP.adoc) whose header says it exists *"to make
  sure it is possible to bootstrap the compiler"*: it bumps the magic numbers, removes the `sinh`
  primitive, then runs `world`, `coreall` and `bootstrap`
  (https://raw.githubusercontent.com/ocaml/ocaml/trunk/tools/ci/inria/bootstrap/script). The
  GitHub workflows `build.yml` and `hygiene.yml` name no bootstrap step
  (https://raw.githubusercontent.com/ocaml/ocaml/trunk/.github/workflows/build.yml, `…/hygiene.yml`).
  **verified**
- **A coupling OCaml undid: the lesson for route (b).** Commit `a92116dc`, David Allsopp,
  2026-03-23, *"Allow version bump without bootstrap"*. As returned, the release number had been
  used *"to derive the bytecode runtime ID by boot/ocamlc"*. This was *"logically coherent"*, but
  *"it adds a requirement for two bootstraps at each tag, which is unacceptable"*. The commit
  writes the whole runtime ID to `runtime-launch-info` instead
  (https://api.github.com/repos/ocaml/ocaml/commits/a92116dcbc7be348a489567134bef7b6bd0b6b60).
  **verified**. Two months earlier, on 2026-01-26, *"first commit after bumping version to 5.6"*
  (15:39 UTC) was followed by *"CI fix: update bootstrapped compiler"* (17:05 UTC, `56ea08ce`,
  touching `boot/` alone) (https://api.github.com/repos/ocaml/ocaml/commits/56ea08ce121c9052cd7b246587047f2ebdd9d37d).
  **verified**. That the second commit was the coupling's cost is an **inference**: the commit
  does not say so.
- **Refresh rhythm.** 15 commits touched `boot/ocamlc` from 2025-10-17 to 2026-10-17, most titled
  *Bootstrap*
  (https://api.github.com/repos/ocaml/ocaml/commits?path=boot/ocamlc&since=2025-10-17T00:00:00Z).
  **verified**. Panel 219 found the same 15.

### Nim: the bootstrap's binary named by its pin; `koch boot` rebuilt to a fixpoint every time

- **Pinned by hash.** `config/build_config.txt` reads `nim_csourcesDir=csources_v3` and
  `nim_csourcesHash=eeab3ac46e93f10efda8e58c4db02b9438319d71`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/config/build_config.txt). **verified**
- **The stage-1 binary carries the pin in its name.** `ci/funs.sh` derives
  `nim_csources=bin/nim_csources_$nim_csourcesHash`. `nimBuildCsourcesIfNeeded` reuses that file
  if it exists, and otherwise clones, runs `git checkout $nim_csourcesHash` and builds. It then
  runs `rm -f bin/nim` and copies the cached binary there
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/ci/funs.sh). **verified**
- **Never the result.** `build_all.sh` then builds `koch` and runs `./koch boot -d:release`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/build_all.sh). `koch boot` loops up to
  three iterations (`let times = 2 - ord(skipIntegrityCheck)`) and compares each build's executable
  with the previous one by `sameFileContent`, printing *"executables are equal: SUCCESS!"*
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/koch.nim). **verified**
- **The version is a commit.** `nim -v` prints `"git hash: " & gitHash`, where `gitHash` is
  `gorge("git log -n 1 --format=%H")` at compile time
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/commands.nim). **verified**
- **Refresh rhythm, and how the tree admits new forms first.** The pin file has 9 commits from
  2021-05-01 to 2025-11-11: two hash updates in 2021, four in December 2022 and January 2023 for
  `csources_v2`, and *"uses csources_v3"* on 2025-11-11
  (https://api.github.com/repos/nim-lang/Nim/commits?path=config/build_config.txt&per_page=100).
  `compiler/condsyms.nim` defines about 70 `nimHas*` symbols under *"for bootstrapping purposes
  and old code:"* (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/condsyms.nim).
  **verified, symbol count approximate**

### V: the seed carries the commit it was emitted from, and the version prints both

- `cmd/tools/gen_vc.v` *"regenerates V's bootstrap .c files"* *"every time the V master branch is
  updated"*. It prepends `#define V_COMMIT_HASH "${last_commit_hash_v_short}"` to `v.c` with
  `sed -i '1s/^/…/'`, and decides whether to regenerate by comparing commit timestamps
  (`if t_unix_vc >= t_unix_v`) (https://raw.githubusercontent.com/vlang/v/master/cmd/tools/gen_vc.v).
  **verified**
- `vlib/v/util/version/version.v`, `full_hash()`: `build_hash := vhash()`,
  `current_hash := vcurrent_hash()`. It returns `build_hash` when they agree and
  `'${build_hash}.${current_hash}'` when they differ. The comment above it says
  *"`C.V_COMMIT_HASH` is only injected into the bootstrap `vc/v.c` by `gen_vc.v`"*
  (https://raw.githubusercontent.com/vlang/v/master/vlib/v/util/version/version.v; `vhash()` in
  `…/version.c.v`). **verified**. Where `vcurrent_hash()` takes its value: **unverified**, its
  definition was not found in the files read.
- The `GNUmakefile` builds `v1` from `vc/v.c`, `v2` with `v1` from `cmd/v`, and `v` with `v2`.
  Its `latest_vc` target pulls `vc` and compares no hash
  (https://raw.githubusercontent.com/vlang/v/master/GNUmakefile). **verified**

### Gambit: the emitted C carries the runtime's version, and the header refuses a mismatch

- `lib/_kernel.c` opens *"produced by Gambit v4.9.8"* and, after the linker block,
  `#define ___VERSION 409008` (https://raw.githubusercontent.com/gambit/gambit/master/lib/_kernel.c).
  `include/gambit.h.in` reads `#if ___VERSION != 409008` / `#include "gambit-not409008.h"`
  (https://raw.githubusercontent.com/gambit/gambit/master/include/gambit.h.in). **verified**.
  That clang stops there is an **inference**: no file of that name was looked for. This is the
  shape of the Heroes seed's `_Static_assert` on `HERO_RUNTIME_ABI`, which the brief cites.
- `INSTALL.txt`: the clone *"contains some stale .c files"*. *"The makefiles call up gsc-boot to
  compile the Scheme source files in the system if they are modified."* `make from-scratch` *"may
  fail if the Gambit compiler itself has evolved in an incompatible way since the latest release"*
  (https://raw.githubusercontent.com/gambit/gambit/master/INSTALL.txt). **verified**
- **Refresh rhythm.** About 35 commits touched `gsc/_front.c` from 2012-04-16 (`ad8d249a`, *"Make
  repo contain the generated .c files"*) to 2026-08-09. Almost all are titled *"[RUNTIME CHANGES
  NEEDED FOR v4.x.y] Changed version of runtime using misc/changev"*, so the refresh is the
  release, and the release moves `___VERSION`
  (https://api.github.com/repos/gambit/gambit/commits?path=gsc/_front.c&per_page=100, read in two
  parts). **verified, count approximate**

### Free Pascal and GCC: a version gate on the bootstrap, a cycle to a fixpoint

- Free Pascal's top-level `Makefile.fpc` sets `REQUIREDVERSION=3.2.2` and `REQUIREDVERSION2=3.2.0`.
  Under `ifndef OVERRIDEVERSIONCHECK` it stops with *"The only supported starting compiler version
  is $(REQUIREDVERSION). You are trying to build with $(FPC_VERSION)."*
  (https://gitlab.com/freepascal.org/fpc/source/-/raw/main/Makefile.fpc). Its `compiler/Makefile.fpc`
  cycle compares successive compilers with `$(DIFF)` (`cmp -i218` on win32, win64 and aix) until
  they match (https://gitlab.com/freepascal.org/fpc/source/-/raw/main/compiler/Makefile.fpc).
  **verified**
- GCC: *"Perform a 3-stage bootstrap of the compiler."* *"Perform a comparison test of the stage2
  and stage3 compilers."* If the comparison fails, this *"normally indicates that the stage2
  compiler has compiled GCC incorrectly"* (https://gcc.gnu.org/install/build.html). **verified**

## The reading, route by route

**(a) A digest the compiler carries, compared by the hook: approve.** Go is the direct precedent.
It retired modification times in 2018 (*"no longer consulted or relevant"*), and it decides
staleness from a hash of the inputs stored in the artefact itself (*"metadata stored in the
compiled packages"*). It has kept that design for eight years, and the `buildid.go` read today
still describes it. Three conditions come from how it fails:
1. **Content, not a commit.** GHC, Nim, Zig and V all carry a commit hash, which cannot see an
   uncommitted edit. Go's own commit stamp needs a separate `vcs.modified` boolean for that, and
   the boolean does not say which edits. A Heroes lane works on uncommitted files, which is why the
   hook judges by modification time today (`.claude/rules/verification.md`, layer 1).
2. **Exactly the inputs the hook can hash.** Go's false reds (46347, 53865) came from a flag or an
   environment that entered the digest at build time and differed at check time.
3. **Printed by a verb that exists.** Go reads it with `go version -m`, GHC with `--info`, V with
   its version. The surface word is the compiler-engineer's call.

**(b) The seed carries what it was emitted from: approve the check, object to the rule alone.**
V does exactly this: `#define V_COMMIT_HASH` is written into the bootstrap C, and the version
prints `build.current` whenever the two differ. Read beside (a), the seed carrying a digest is not
a separate mechanism. If every emission of the compiler carries the digest of the sources it
compiled, then the seed, being one such emission, carries it, and stage 1 prints it (*inference*).

OCaml's commit `a92116dc` is the warning. Something the bootstrap embeds that has to agree with
the tree turns every change of that thing into a bootstrap (*"two bootstraps at each tag, which
is unacceptable"*). The digest must therefore be printed and compared by the hook. It must never
be asserted by clang or by the build, which is what Heroes' `_Static_assert` on the ABI and
Gambit's `___VERSION` already do for the runtime's interface.

On *"stage 1 never judges"*: none of the precedents read keeps stage 1 out of judging by a
written rule. Zig's zig1 has six features and no test runner. Go's toolchain1 *"has no build
IDs"*, so the go command can never take it as fresh. GHC's stage1 lacks the interpreter, and its
testsuite defaults to stage2. Each keeps stage 1 out by construction or by a mark in the binary.
Heroes' stage 1 is the whole compiler (panel 219's s4 is *"for every program"*), so a written rule
alone would leave open exactly the 2026-09-18 class.

**(c) Always two stages, the hook on the stage-2 artefact: approve the build, object as the
detector.** Every precedent read builds through its stages from a clean build: Go's three
toolchains, Zig's zig1, zig2 and stage3, Nim's `koch boot`, V's v1, v2 and v, OCaml's `world`,
GHC's stages, GCC's three stages and Free Pascal's cycle. None tells which stage a binary is by
its path or its time. Go tells by content and GHC by a `Stage` field. A stage-1 binary written to
the stage-2 path passes a hook that reads only the path and the time (*inference*).

**(d) Nothing: object.** Zig and Gambit do check nothing about their bootstrap's freshness. But in
both, the build system runs every later stage on every clean build, and Zig's CI builds through the
committed `zig1.wasm` and through a freshly regenerated one on every release-leg run. Neither leaves
a bootstrap-built binary where its tests look for the judge. Under the new rhythm Heroes would do
exactly that, and its own record prices the result at 31 minutes for one bit.

**The route nobody listed: a stamp beside the binary (Rust, Nim): object for Heroes.** Rust's
stamp works because `x.py` is the only builder. Nim's name-carries-the-pin works because `funs.sh`
is the only builder. Heroes' bootstrap is one `clang` line printed on 34 lines of 24 files (the
brief), and a hand-typed `clang` writes no stamp. A digest inside the binary (Go) survives being
built by hand; a stamp beside it does not (*inference*).

**Rhythm, for the record.** Zig refreshed about 54 times in three years. The titles that give a
reason name a language change its own build used, or a C compiler's compatibility. OCaml refreshed
15 times last year, forced by the runtime and, until March 2026, by the version number. Rust
refreshes every six weeks, at the release. Gambit refreshes at each release, which moves
`___VERSION`. Nim's pin file changed in 9 commits over four and a half years, and Nim lets new
forms into the tree ahead of its bootstrap through about 70 `nimHas*` symbols. Rust does the same
through `cfg(bootstrap)`. Heroes' *a new form enters `selfhost/` only after the refresh whose seed
knows it* is therefore Zig's discipline, not Rust's or Nim's. Whether Heroes has a conditional the
seed could read as false is the compiler-engineer's to say; *unrun here*.

## argument

Go is the precedent that answers the question as asked. In 2018 it stopped consulting modification
times, and it has judged staleness ever since from a hash of the inputs stored in the binary.
Every other compiler read keeps a stage-1 binary from judging by construction: Zig's zig1 has
six features, Go's toolchain1 has no build IDs, GHC's stage1 lacks the interpreter and its binary
names its stage. V shows that a seed can carry what it was emitted from at the cost of one line.
OCaml shows that what a seed carries must be compared, never required, or every change becomes a
refresh. So (a) holds, carried by the seed as (b) says, with (c) as the build discipline. (d)
leaves open the class the 2026-09-18 record already paid for.

## condition

My reading would change on any of four findings:
1. **A content-based check made more trouble than it saved.** A project that replaced
   modification-time staleness with a content digest in the binary and then returned to
   modification times, or that reports false reds outweighing the stale judges caught. Go's
   46347 and 53865 are the two read, and neither led Go back.
2. **A rule alone held up.** A self-hosting compiler that let its full stage-1 binary judge and
   kept it out by a written rule alone, without a mark in the binary, and reports no stale-judge
   incident. That would weaken the objection to (b)'s rule alone.
3. **A digest that cannot be computed reliably.** Evidence that the digest of `selfhost/` and
   `runtime/` cannot be made a pure function of file contents, for instance because the build
   reads inputs the hook cannot see. That would be Go's 46347 shape and would turn (a) into a
   false-red machine. The compiler-engineer can settle this by running it; this seat cannot.
4. **The coupling OCaml undid.** Evidence that the seed's digest would have to be asserted rather
   than printed. That is the shape `a92116dc` removed.
