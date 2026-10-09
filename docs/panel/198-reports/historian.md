# Panel 198, historian

Copied by the coordinator at 09:57 on 2026-10-08 (`date`) from the seat's reply,
verbatim apart from this header and the HTML entities of the notification turned
back into their characters: the harness refused the seat's Write of its report
(*subagents should return findings as text*), so it wrote none, and the whole
report is its reply. The seat was stopped once by the session limit at about
08:25 and resumed at 09:50 on a new account. It has no shell: the repository
files it read it read with `Read`, and every web source passed through the
fetch tool's summarising model, which the seat says where it matters. Re-read
any load-bearing quotation at source before it lands in a record.

---

**verdict**: object (advisory), to routes D and E and to the refusal note's appeal to Go. Admitting words by exact name has precedent. The report file does not exist: the harness refused my Write ("Subagents should return findings as text"), so the whole report is below. All web reads are dated 2026-10-08. Where I fetched through a summariser and not the raw page, I say so.

**precedents**

**1. Go, the list itself**

- The first fix is commit `1dcb5836ad`, by Russ Cox, author date 2018-02-01, "Fixes #23672, CVE-2018-6574" (verified, `api.github.com/repos/golang/go/commits/1dcb5836ad`).
  - Its message says "The new restrictions can be adjusted by the environment variables CGO_CFLAGS_ALLOW, CGO_CFLAGS_DISALLOW, and so on."
  - Issue #23672 gives the fixed releases as Go 1.8.7, 1.9.4 and 1.10rc2, and says "The same restrictions are applied to compiler flags obtained from pkg-config" (verified, `api.github.com/repos/golang/go/issues/23672`).
  - It also says env-var flags are unrestricted.
  - The same issue says the split pair `-Wl,-rpath -Wl,$ORIGIN` must become the one-word form.
- The first list, read raw at `raw.githubusercontent.com/golang/go/1dcb5836ad2c/src/cmd/go/internal/work/security.go` (verified), held:
  - 21 compiler regexps and 4 next-word flags (`-D -I -framework -x`);
  - 12 linker regexps and 4 next-word flags;
  - `-pthread` in both lists, and `-Wl,-rpath,<dir>` one-word only;
  - none of `-isystem`, `--as-needed`, either dtags word, `--export-dynamic`, or `-z`.
- Today's file, which I Read locally (Go 1.27.1), holds 94 compiler regexps (`:45-138`), 12 next-word compiler flags, 68 linker regexps (`:163-237`) and 13 next-word linker flags. These are counts by line arithmetic from the Read output, not by a tool. `--whole-archive` appears nowhere in it, so Go refuses libpsx's answer too (read, not grepped).
- When each word entered:

| Word | Commit and date | Why |
|---|---|---|
| `-Wl,--(no-)?as-needed`, both dtags words, `-isystem`, `-Wl,-framework` pairs | `f7c2a71632`, author date 2018-02-14, "Fixes #23749" | User reports on #23749 (`-isystem` and dtags below); the message gives no per-word reasoning |
| `-Wl,--export-dynamic` | `ceb7745cc8`, 2018-09-07, "Fixes #27496" | The bimg package refused (`api.github.com/repos/golang/go/commits/ceb7745cc8`, `.../issues/27496`) |
| `-Wl,-z,relro` and `-Wl,-z,(no)?execstack` | `7e34ac1f4c`, 2018-03-28, "Fixes #23937" | Not stated |
| `-Wl,-z,now`, merged into `-Wl(,-z,(relro|now|(no)?execstack))+` | `88480fadcc`, 2024-03-13 | Linker `-bindnow` work (#45681) |
| `-pthread` | First commit, 2018-02-01 | Not stated |

  - The `f7c2a71632` message (`api.github.com/repos/golang/go/commits/f7c2a71632`) reads "Also permit passing flags to pkg-config, as we used to."
  - On #23749 (`api.github.com/repos/golang/go/issues/23749/comments`), qeedquan wrote on 2018-02-13 "please add these flags to the whitelist, -Wl,--enable-new-dtags". The commit followed about 25 hours later.
  - The same thread, 2018-02-09, has steeve unable to whitelist `-isystem` because the next word, a path, was denied.
  - On 2018-02-08 andlabs asked whether `-as-needed` is safe since it is positional. Ian Lance Taylor gave a `--as-needed ... --no-as-needed` example.
  - The summariser excerpted these quotes, so the wording is approximate.
- **Position words (Q2).** Go checks no ordering. Go 1.27.1 handles `--push-state` only as a composite prefix (`security.go:406-430`). That arrived in `4a0d5d601e` (2024-10-25, Fixes #70023), then `c901d93fcd` (2024-11-15), by the commit listing (verified, truncated list).
- **`CGO_*_ALLOW` and `_DISALLOW`.** They arrived in the first commit, in releases 1.8.7, 1.9.4 and 1.10rc2 (sources above).
  - `checkOverrides := true` for compiler and linker flags, `false` for pkg-config's own flags (`security.go:308-321`, read).
  - The pkg-config answers go through `checkCompilerFlags` and `checkLinkerFlags` (`exec.go:1841`, `:1857`, read).
  - The documentation says env-var flags are not restricted (`pkg.go.dev/cmd/cgo`, verified).
- **How the overrides are used in the record.**
  - #23749 has 138 comments (opened 2018-02-08, closed 2018-05-30).
  - Users set `CGO_LDFLAGS_ALLOW=".*\\.a"` (glycerine, 2018-02-09) and `"-I.*"` (AlexRouSg, 2018-05-07).
  - jeddenlea pointed `PKG_CONFIG` at a wrapper script (2018-02-08).
  - Package READMEs now tell users to export `CGO_CFLAGS_ALLOW="-Xpreprocessor"` (govips, `pkg.go.dev/github.com/davidbyttow/govips/v2`, verified).
  - #77387 (2026-01-31) reports the workaround `CGO_CFLAGS_ALLOW='.*define.*variable.*'`.
  - GitHub issue search returned 320 hits for `CGO_LDFLAGS_ALLOW` and 256 for `CGO_CFLAGS_ALLOW` (`api.github.com/search/issues`). These are hits across repositories, noisy, and not a count of distinct workarounds.
  - A Dockerfile line `ENV CGO_LDFLAGS_ALLOW='.*'` appeared only in a search-engine summary, so it is **unverified**.
- **Counts of refusals of correct input.**
  - The tracker searches returned 45 issues for "invalid flag in #cgo" and 15 for "invalid flag in pkg-config", with noise such as `TestScript` failures. I did not narrow them to correct-package refusals.
  - The 15 include #23737, #24505, #25493, #27496, #34710, #35262, #39988, #41199, #44263, #70023, #77387 and #79330.
  - I have no total count of commits touching `security.go`. The API listing I read had two chunks and omitted at least one commit I verified separately (`bbeb55f5fa`), so any total from it is **unverified**.
- **#23737** (2018-02-08, closed the same day): `-Wl,--export-dynamic` from `pkg-config --libs` was judged against the compiler list. The fix was CL 92755, per Ian Lance Taylor's comment on #23749. The same word that refuses `gmodule-2.0` here went unadmitted for seven months, until #27496.
- **#23749's own summary** says the whitelist made "several different people" report packages that "fail to build by default".

**2. Advisories after CVE-2018-6574 that touched the list (all verified via `pkg.go.dev/vuln` and the commit API)**

- **CVE-2020-28367** (issue #42558, 2020-11-12; fixed in 1.14.12 and 1.15.5; GO-2022-0476). Commit `da7aa86917` restricted `-D`/`-U` to C identifiers and banned commas in `-Wp,-D` values.
- **CVE-2023-29404** (GO-2023-1841, issue #60305, published 2023-06-08; fixed in 1.19.10 and 1.20.5). Commit `bbeb55f5fa`: "-Wl,-O -Wl,-R,-bad-flag" is read as "-O=-R -bad-flag", so a flag with a non-optional argument can smuggle the next word. This is the shape of the `-isystem <dir>` and `-Wl,-rpath -Wl,<dir>` handling Heroes has.
- **CVE-2024-24787** (GO-2024-2825, published 2024-05-08; fixed in 1.21.10 and 1.22.3). Commit `348b23830d` (2024-04-25) added `invalidLinkerFlags`. The message says `-lto_library` "wasn't caught by our 'safe linker flags' check because it was covered by the -lx flag". The comment is "On macOS this means the linker loads and executes the next argument." A prefix wildcard let it through.
- **CVE-2025-22867.** Commit `e3cd55e9d2` (2024-12-27, Fixes #40559) allowed `@` in some Darwin `-Wl,` words. Commit `51bf2cf7cf` (2025-01-29) reverted it: "@ flags are first resolved as files by the darwin linker, before their meaning as flags, allowing the flag filtering logic to be entirely bypassed." Issue #71476 says only go1.24rc2 was affected. #40559, the correct program that prompted the widening, is still open. **A widening made for a correct program was itself the CVE.**
- **CVE-2025-61731**, announced 2026-01-15 on `groups.google.com/g/golang-announce/c/Vd2tYVM8eUc` (NVD publication is said to be 2026-01-28 by a search summary, so **unverified**). Commit `5e1ad12db9` sanitises the flags passed to `pkg-config` itself. It produced two regressions: #77387 (fixed by `28fbdf7acb`, 2026-02-03) and #79330 (fixed by `921529fd83`, 2026-07-28).
- **Open on 2026-10-08:** #78750 (2026-04-14, labelled Security and NeedsFix, opened by Neal Patel). It says `security.go:225`, the `-Wl,-sectcreate` entry, lets a module make the Darwin linker read an arbitrary file. A proposal, #80491 (2026-07-21), would reject it by default with the override as the way out. So Go's list has admitted a file-reading word since 2018-03-28 (`7e34ac1f4c`). Go labels it a hardening measure.
- No advisory I found names `--enable-new-dtags`, `--disable-new-dtags`, `--as-needed`, `--export-dynamic`, `-pthread` or `-isystem`. I did not search the GitHub Advisory Database directly, so "none exists" is **unverified**.

**3. Rust**

- The `pkg-config` crate, read raw from `master` (verified, through a summariser): `-L`, `-F`, `-l`, `-D` and `-framework` are parsed. `-isystem`, `-iquote` and `-idirafter` are parsed too. Every `-Wl,` word is split on commas and emitted as `cargo:rustc-link-arg=-Wl,...` and stored in `ld_args`. A word matching nothing is ignored, with a warning only if it is a file. Neither `-pthread` nor `-Wl,-rpath` is mentioned in the code shown.
- `CHANGELOG.md` (verified): `-isystem` family added in 0.3.19 (2020-10-13), `-Wl` parsing in 0.3.25 (2022-03-31), `-Wl,-u` in 0.3.29 (2024-01-17).
- PR #131 (`github.com/rust-lang/pkg-config-rs/pull/131`, verified): the first proposal added rpath alone. The reviewer sdroege replied "instead of supporting specific linker arguments it would probably make more sense to pass through all -Wl arguments as-is". The merged form passes them through.
- No refusal for safety, found by search. I searched pkg-config-rs for advisories and found none; the hits were pkgconf CVEs. Rust's stated model is that build scripts do anything "by design", relying on trust (`goals.rust-lang.org/2024h2/sandboxed-build-script.html`, verified; goal status Accepted).
- `rustc-link-arg` is documented for binaries, `cdylib` crates, tests, benchmarks and examples only (`doc.rust-lang.org/cargo/reference/build-scripts.html`, verified). Whether a dependency's `rustc-link-arg` reaches the final binary I did not find, so it is **unverified**.

**4. Meson, CMake, Zig, Swift, Nix**

- Meson (`mesonbuild/dependencies/pkgconfig.py`, `_search_libs`, verified). `-L` and `-l` are resolved to full paths. Every other word, `-Wl,`, `-pthread` and the rest, is appended to `link_args` unchanged. `_set_cargs` passes the cflags as `compile_args`. No filter. I found no `new-dtags` in `mesonbuild/linkers/linkers.py`; the fetch reported no truncation.
- CMake (`cmake.org/cmake/help/latest/module/FindPkgConfig.html`, verified). `<XXX>_LDFLAGS_OTHER` holds "All other linker flags". `IMPORTED_TARGET` puts non-library linker options into `INTERFACE_LINK_OPTIONS` (3.15). From 3.18 it puts `-isystem` directories into `INTERFACE_INCLUDE_DIRECTORIES`. No refusal is documented. What it does with `--enable-new-dtags` specifically: not found.
- Zig, `runPkgConfig` in `lib/std/Build/Step/Compile.zig` (verified through a summariser). It handles `-I -L -l -D` and turns `-Wl,-rpath,` into `-rpath`. Any other word is silently dropped unless `b.debug_pkg_config` is set, which makes it an error. That is route D. A Zig issue about partial pkg-config application showed up in search snippets only, so what happened to Zig users is **unverified**.
- Swift Package Manager (`forums.swift.org/t/confused-by-unsafe-flags-being-disallowed-in-dependencies/27359` and `.../pitch-disable-checks-for-unsafe-flags-in-swiftpm/80698`, verified). A versioned dependency may not carry `unsafeFlags`. The check was planned to lift for local and branch dependencies in 2019. A pitch to disable the check was approved in July 2025, with the Ecosystem Steering Group endorsing it on 2025-07-08. SwiftPM's pkg-config parser showed no per-flag filter (summariser, so **unverified**). This is the one tool besides Go that restricted who may widen, and it reversed its restriction.
- Nix, `ld-wrapper.sh` and `utils.bash` (verified, summariser).
  - Under `NIX_ENFORCE_PURITY` it silently skips `-L`, `-rpath` and `-dynamic-linker` with a path outside the store or a temporary directory. The `skip` message prints only at `NIX_DEBUG>=1`. This is a path test for reproducibility, not for safety.
  - The `pkg-config-wrapper.sh` does not touch the output.
  - nixpkgs' `binutils/default.nix` sets `--enable-new-dtags`, commented "RUNPATH can be overridden using LD_LIBRARY_PATH at runtime".
  - Commit `18f517fbd6` (2018-07-14) says `patchelf` converts any RPATH to RUNPATH unless `--force-rpath` is given.
  - I found no `dtags` mention in `ld-wrapper.sh`.

**5. The dtags**

- gABI (`sco.com/developers/gabi/latest/ch5.dynamic.html`, verified; page undated): DT_RPATH is superseded by DT_RUNPATH. "All LD_LIBRARY_PATH directories are searched before those from DT_RUNPATH." If both are present, only RUNPATH is processed.
- ld.so(8) (`man7.org/linux/man-pages/man8/ld.so.8.html`, verified): RPATH is searched first if no RUNPATH exists, then `LD_LIBRARY_PATH`, then RUNPATH. RUNPATH "do not apply to those objects' children". dlopen(3) (`man7.org/linux/man-pages/man3/dlopen.3.html`, verified) lists the same order.
  - Q5: RUNPATH reaches the binary's direct `DT_NEEDED` entries only. SDL3's `dlopen`ed backends fall under dlopen's calling-object rule and not the binary's. I did not run this.
- Earliest dated evidence I hold is H.J. Lu's mail of 2000-07-20 (`www.sourceware.org/ml/binutils/2000-07/msg00322.html`, verified, summariser). It concerns the new gABI tags, glibc 2.1 rejecting unknown tags, "the same binaries run fine under glibc 2.2", and the proposed name `--enable-new-dynamic-tags`.
  - A 2013 patch says the options "have been around for 14+ years" (Mike Frysinger, `sourceware.org/ml/binutils/2013-01/msg00296.html`, verified).
  - The statement "RUNPATH was added since 1999" is from a blog (`blog.tremily.us/posts/rpath/`) and **unverified**.
  - The gABI edition that introduced DT_RUNPATH: **not found**.
- Upstream default (verified, with caveats):
  - The sourceware manual for GNU Binutils 2.44.50 says "By default, the new dynamic tags are not created."
  - `ld/configure.ac` has `ac_default_new_dtags` unset become 0 (read from the gitlab mirror `gitlab.com/gnutools/binutils-gdb`, not sourceware, so mirror-grade).
  - A 2013 patch to make new dtags the default for Linux drew H.J. Lu's reply "This breaks -rpath on Linux" (`sourceware.org/ml/binutils/2013-02/msg00022.html`, verified).
- Distributions that configure GNU ld to write RUNPATH:
  - Debian since `binutils/2.27.51.20161116-2`, from bug #835859 (reported 2016-08-28, fixed 2016-11-17, changelog "ld: enable new dtags by default for linux/gnu targets"; `bugs.debian.org/cgi-bin/bugreport.cgi?bug=835859`, verified). That bug says Gentoo already did.
  - nixpkgs (above).
  - Yocto, by Khem Raj's patch of 2023-02-22 (`patchwork.yoctoproject.org/comment/9019`, verified), which says "this is now default on major linux distributions already".
  - Linux From Scratch's binutils 2.45 build passes `--enable-new-dtags` (`linuxfromscratch.org/lfs/view/stable/chapter08/binutils.html`, verified).
  - Arch is said to have switched at 2.39-4 (search snippet only; the page was blocked, so **unverified**).
  - A FreeBSD ports bug titled "devel/binutils: configure with --enable-new-dtags" (#259446) exists; the page was blocked, so the contents are **unverified**.
  - I found no `dtags` patch in Debian binutils 2.44-3's `debian/rules` or `patches/series`, so how Debian 13 gets RUNPATH is **unverified** by me. The shared brief measured it.
- Why SDL writes the word (verified):
  - Commit `dc5f05bb99` (2016-01-07, Sam Lantinga): "Use --enable-new-dtags to set RUNPATH rather than RPATH so that LD_LIBRARY_PATH is not overridden by the application." It touched `CMakeLists.txt`, `configure` and `configure.in`.
  - Commit `757e994eaa` the same day: "Fixed --enable-new-dtags check with cmake".
  - At `release-3.2.10`, `SDL_RPATH` defaults to ON for `UNIX AND NOT ANDROID AND NOT RISCOS AND NOT SDL_FRAMEWORK` (raw `CMakeLists.txt`).
  - The Linux, BSD and NetBSD block probes the word with `check_c_compiler_flag("" HAVE_ENABLE_NEW_DTAGS)` and writes `-Wl,-rpath,${libdir} -Wl,--enable-new-dtags` only if it passes.
  - The Apple block writes `-Wl,-rpath,${libdir}` alone.
  - `cmake/sdl3.pc.in` has `Libs: -L${libdir} @SDL_RLD_FLAGS@ ...`.
  - SDL itself gates the word by platform and by a probe of the build machine's linker, and its macOS branch omits it. The probe runs where the library is built, not where it is consumed.

**6. Faults of this shape**

- Exploited: CVE-2018-6574 (cited), CVE-2020-28367, CVE-2023-29404, CVE-2024-24787 and CVE-2025-22867 (section 2). All are Go. I found no exploit of this shape in Cargo, Meson, CMake or Nix.
- A list so narrow users routed around it: Go, section 1. The override sat in the first commit and became routine.
- A different attacker model: in Go the module can ship its own `.pc`. rgburke, on #23749 on 2018-02-08, quoted `#cgo !windows pkg-config: --static ${SRCDIR}/vendor/libgit2/build/libgit2.pc`. In Heroes the search path is the machine's own `PKG_CONFIG_PATH` (`docs/design.md:2642-2645`, read), so the attacker needs a `.pc` installed on the machine.
- The note's wording: `selfhost/cli/libraries.hero:205` says everything else is a flag "a package file could use to run code during the build (Go's CVE-2018-6574)". Go admits `-pthread` from day one and `-isystem` from 2018-02-14. I found no reasoned departure in `libraries.hero:1-280`.
- No platform precedent in Go: its list is platform-blind, so on macOS `-Wl,--enable-new-dtags` would pass Go's check and reach ld64 (M1). SDL's probe and branch are the platform precedent.

**Searched, nothing found:**
- gABI edition for DT_RUNPATH.
- Upstream `ld/NEWS`: 404 or Anubis-blocked.
- Any advisory on the dtags class.
- Cargo propagation of `rustc-link-arg`.
- Zig user outcomes.
- Meson dtags emission beyond `linkers.py`.
- Debian 13's RUNPATH source.

**argument** (112 words)

Go is the precedent, and it supports admitting by exact name. Both dtags words, `--as-needed` and `-isystem` entered Go's list on 2018-02-14, thirteen days after the first list, on tracker reports (#23749); `-pthread` was there from day one. So the note's appeal to CVE-2018-6574 for refusing them is contradicted by Go's own list. Go's later advisories were never about those words: they were a wildcard (`-lto_library` under `-l`), next-word smuggling (CVE-2023-29404), `@` indirection (CVE-2025-22867, a widening that was itself the CVE) and a file-reading word (`-sectcreate`, open #78750). That tells against routes D and E, whose test is "names no path". Go also shipped the person's override in its first commit, and it became routine (320 issue hits).

**condition**

- An advisory or exploit in any build tool naming `--enable-new-dtags`, `--disable-new-dtags`, `--as-needed`, `--export-dynamic`, `-pthread` or `-isystem` (I did not search the GitHub Advisory Database) would reverse my reading on admitting those by name.
- A tool that ran for years on an "any word that names no path" rule with no bypass would soften my objection to E. I found none: Nix's path test is for purity and drops silently, and Go's own `-sectcreate` case is a file-naming word that stayed on the list unnoticed for eight years.
- How Go resolves #78750 and #80491 would settle whether "names a file" is the criterion even Go applies.
- A reasoned Heroes ruling for refusing `-pthread` and `-isystem` that I did not find would change my reading of the note's attribution.

Relevant local files: `/opt/homebrew/Cellar/go/1.27.1/libexec/src/cmd/go/internal/work/security.go`, `/opt/homebrew/Cellar/go/1.27.1/libexec/src/cmd/go/internal/work/exec.go`, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-198/selfhost/cli/libraries.hero`, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-198/docs/design.md`, `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-198/.github/workflows/ci.yml`.

Sources:
- https://github.com/golang/go/issues/23672
- https://github.com/golang/go/issues/23749
- https://github.com/golang/go/issues/23737
- https://github.com/golang/go/issues/27496
- https://github.com/golang/go/issues/78750
- https://github.com/golang/go/issues/80491
- https://github.com/golang/go/issues/71476
- https://go.dev/wiki/InvalidFlag
- https://pkg.go.dev/cmd/cgo
- https://pkg.go.dev/vuln/GO-2022-0476
- https://pkg.go.dev/vuln/GO-2023-1841
- https://pkg.go.dev/vuln/GO-2024-2825
- https://groups.google.com/g/golang-announce/c/Vd2tYVM8eUc
- https://pkg.go.dev/github.com/davidbyttow/govips/v2
- https://raw.githubusercontent.com/golang/go/1dcb5836ad2c/src/cmd/go/internal/work/security.go
- https://raw.githubusercontent.com/rust-lang/pkg-config-rs/master/src/lib.rs
- https://raw.githubusercontent.com/rust-lang/pkg-config-rs/master/CHANGELOG.md
- https://github.com/rust-lang/pkg-config-rs/pull/131
- https://goals.rust-lang.org/2024h2/sandboxed-build-script.html
- https://raw.githubusercontent.com/mesonbuild/meson/master/mesonbuild/dependencies/pkgconfig.py
- https://cmake.org/cmake/help/latest/module/FindPkgConfig.html
- https://raw.githubusercontent.com/ziglang/zig/master/lib/std/Build/Step/Compile.zig
- https://forums.swift.org/t/confused-by-unsafe-flags-being-disallowed-in-dependencies/27359
- https://forums.swift.org/t/pitch-disable-checks-for-unsafe-flags-in-swiftpm/80698
- https://raw.githubusercontent.com/NixOS/nixpkgs/master/pkgs/build-support/bintools-wrapper/ld-wrapper.sh
- https://raw.githubusercontent.com/NixOS/nixpkgs/master/pkgs/build-support/wrapper-common/utils.bash
- https://raw.githubusercontent.com/NixOS/nixpkgs/master/pkgs/build-support/pkg-config-wrapper/pkg-config-wrapper.sh
- https://raw.githubusercontent.com/NixOS/nixpkgs/master/pkgs/development/tools/misc/binutils/default.nix
- https://www.sco.com/developers/gabi/latest/ch5.dynamic.html
- https://man7.org/linux/man-pages/man8/ld.so.8.html
- https://man7.org/linux/man-pages/man3/dlopen.3.html
- https://www.sourceware.org/ml/binutils/2000-07/msg00322.html
- https://www.sourceware.org/ml/binutils/2013-01/msg00296.html
- https://sourceware.org/ml/binutils/2013-02/msg00022.html
- https://sourceware.org/binutils/docs/ld/Options.html
- https://gitlab.com/gnutools/binutils-gdb/-/raw/master/ld/configure.ac
- https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=835859
- https://patchwork.yoctoproject.org/comment/9019
- https://www.linuxfromscratch.org/lfs/view/stable/chapter08/binutils.html
- https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/CMakeLists.txt
- https://raw.githubusercontent.com/libsdl-org/SDL/release-3.2.10/cmake/sdl3.pc.in
- https://api.github.com/repos/libsdl-org/SDL/commits/dc5f05bb99b9618db11db23a74605c7a98213b1d
