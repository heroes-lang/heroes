# Panel 198, historian

Read `00-shared.md` first, whole. Your seat is advisory (no veto) and judges
precedent: **every claim carries its source (a URL and the date you read it,
or a file and its line), or it is marked *unverified***. You have no shell,
so you do not copy the tree: read the files named below with your file tool,
and the web with search and fetch. Your running notes go in
`<scratchpad>/198-historian/report.md`, appended as you go. Written on
2026-10-08 for the full panel the author convened; this seat had no brief in
the soundness lane.

The question you bring precedent to: **which words a package's
`pkg-config` answer may carry into a build, how a toolchain decides, and who
may widen it**, for defect 444's `-Wl,--enable-new-dtags` and for the whole
class (Q1: `-pthread`, `-isystem <dir>`, `-W...`, position-dependent `-Wl,`
pairs).

1. **Go, the precedent the list cites** (`selfhost/cli/libraries.hero:5-9`,
   CVE-2018-6574). Its list is on this Mac:
   `/opt/homebrew/Cellar/go/1.27.1/libexec/src/cmd/go/internal/work/security.go`
   (Go 1.27.1, `go version`, 2026-10-08), and the calls that apply it to
   pkg-config's answers are `exec.go:1826-1857` beside it. Read them, then
   find in Go's history:
   - when each word this sitting weighs entered the list, by which issue or
     change and why: `-Wl,--disable-new-dtags` (`:208`),
     `-Wl,--enable-new-dtags` (`:210`), `-Wl,--(no-)?as-needed` (`:201`),
     `-Wl,--(no-)?export-dynamic` (`:212`), `-Wl,-z,(relro|now|...)` (`:234`),
     `-pthread` (`:131`, `:187`), `-isystem` (`:150`);
   - `CGO_CFLAGS_ALLOW`, `CGO_LDFLAGS_ALLOW` and their `_DISALLOW` (`:366-385`,
     applied to pkg-config's answers, `checkOverrides := true` in
     `checkCompilerFlags` and `checkLinkerFlags`, `:308-316`): when they
     arrived, in which release, and what the record
     says of how they are used;
   - how many words were added after the first list, and how many reports of a
     correct package refused it took (the issue tracker's own count, sourced);
   - the advisories after CVE-2018-6574 that changed the list, if any:
     `invalidLinkerFlags` (`:156-160`) refuses `-lto_library` with the comment
     *On macOS this means the linker loads and executes the next argument*;
     which advisory added it, and any other.

2. **Rust**: a build script (`build.rs`) runs arbitrary code at build time,
   and `cargo:rustc-link-arg` hands the linker any word; what the
   `pkg-config` crate does with a `.pc` file's `Libs:` words (does it filter
   or pass on `-Wl,` words, `-pthread`, `-isystem`?), and whether Cargo or
   the crate ever refused a word for safety.

3. **Meson and CMake**: how Meson's `dependency()` with the pkg-config method
   passes a `.pc` file's `Libs:` and `Cflags:` words to the link and the
   compile, and whether it filters any; how CMake's `FindPkgConfig`
   (`pkg_check_modules`, `<PREFIX>_LDFLAGS`, `IMPORTED_TARGET`) does the same.
   What either does with `-Wl,--enable-new-dtags` or `-isystem`.

4. **Nix**: how nixpkgs' compiler and linker wrappers treat rpaths and the
   dtags words (`NIX_LDFLAGS`, the `ld-wrapper`, `patchelf`), and its
   `pkg-config` wrapper; whether a word from a `.pc` file is ever refused.

5. **The dtags themselves**: when `DT_RUNPATH` was introduced beside
   `DT_RPATH` and why (the gABI, glibc's loader); which distributions build
   GNU ld to write RUNPATH by default and what upstream binutils' own default
   is (the shared brief measured RUNPATH on every Linux toolchain it ran and
   calls the upstream default unverified); and why SDL writes
   `--enable-new-dtags` into `sdl3.pc` when `SDL_RPATH` is on: SDL's own
   CMake sources at the tag `release-3.2.10`, commit
   `877399b2b2cf21e67554ed9046410f268ce1d1b2` (`.github/workflows/ci.yml:343-344`),
   and when the option arrived.

6. **Faults of this shape in the record**: a build tool that passed a
   package's or a dependency's flags on and was exploited (CVE-2018-6574 is
   the one cited; any other, cited); and the opposite, a list so narrow that
   users routed around it (an override set by default, a wrapper), if the
   record has one.

Output: `verdict` (approve or object, advisory), `precedents` (each with its
source and *verified* or *unverified*), `argument` (120 words at most),
`condition`. No paid run.
