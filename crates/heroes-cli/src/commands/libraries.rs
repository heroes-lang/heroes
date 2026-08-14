//! What a group's `link` and `package` become on clang's command line
//! (design.md §4.19; panels 048, 049, 050).
//!
//! Split from `toolchain.rs` by the §11 sweep at 517 lines, and the seam is a real
//! one rather than a size: the rest of that file is about **driving clang** —
//! where the runtime is, what the cache key covers, which flags a build names.
//! This is about **asking the machine a question the program cannot answer**:
//! where a library lives, what else it needs, and whether this platform's C
//! runtime already contains it.
//!
//! Two of the four things here are safety rather than plumbing, and both arrived
//! on the first day rather than after a disclosure: the **allow-list**, because a
//! `.pc` file is input this program did not write; and `in_the_c_runtime`, because
//! a universal spelling has to go on working on a platform where the library it
//! names does not exist.

use std::process::Command;

/// What a program asked to be linked against: names it knew, and names it asked
/// the machine about.
///
/// One struct rather than two more parameters, because `link` already carries a
/// `PORT-DEBT` for seven of them and CLAUDE.md §5 forbids ratcheting one.
pub struct Libraries {
    pub link: Vec<String>,
    pub packages: Vec<String>,
    /// `--include` and `--library`, in the order written (author decision
    /// 2026-08-14, panel 055's split settled).
    ///
    /// **They ride here rather than as two more parameters of `link`**, which
    /// already carries a `PORT-DEBT` for seven and which CLAUDE.md §5 forbids
    /// ratcheting up. And they belong here on meaning as well as on count: this
    /// struct is what a program says about *where things are*, and a search path
    /// is the same sentence said by the invocation instead of by the source.
    ///
    /// **No allow-list, and the reason is a falsifiable claim rather than a
    /// judgement.** Panel 050's list exists because a `.pc` hands back one string
    /// that is split on whitespace and each word read as a flag — the splitting is
    /// the vector. A search path is **one argv word** that clang consumes as a
    /// path: `Command::arg` passes argv with no shell, the parser refuses a value
    /// beginning with `-`, and clang reports `ignoring nonexistent directory` for
    /// a `-I` whose operand looks like a flag rather than obeying it. If either
    /// half of that dies — if these are ever built by string concatenation, or
    /// passed through a shell — the exemption dies with it, and
    /// `a_search_path_reaches_clang_as_one_argv_word` is the test that says so.
    pub search: Search,
}

/// Where to look, when no `package` can answer. Re-exported from `compile` so the
/// driver and the argv layer name one type.
pub use super::compile::Search;

/// Whether this platform's C runtime already contains a library, so naming it is
/// correct and passing it on would be an error.
///
/// **This is how the universal spelling is made to always work**, and the
/// precedent is Zig's `isLibCLibName`, which does exactly this per target so that
/// `-lm` means "link libc" instead of a link failure (sourced, panel 049's
/// historian). Panel 049 refused a platform axis in the *language*; this is the
/// same problem answered in the *driver*, where it is a fact about the machine
/// rather than a word in the author's program.
///
/// Windows is the case that forced it. `link "m"` is required on glibc and
/// FreeBSD, a POSIX-mandated no-op on musl and macOS — and on MSVC it is
/// `LNK1181: cannot open input file 'm.lib'`, because the maths functions are in
/// the C runtime and there is no separate library to open. Measured on the third
/// CI leg, 2026-08-14, by `tests/golden/run/ffi-constant.hero`, which had been
/// correct on two platforms for a day.
///
/// The list is deliberately short and per-platform: a name here is one this
/// driver *knows* is folded in, not one it guesses at. A library that is genuinely
/// absent still fails, and says so with `ffi_missing_link`.
pub(super) fn in_the_c_runtime(library: &str) -> bool {
    if cfg!(target_os = "windows") {
        // The POSIX names a Unix program writes, all of which the Microsoft CRT
        // either contains or does not have as a separate library at all.
        return matches!(library, "m" | "pthread" | "dl" | "rt" | "util" | "resolv");
    }
    false
}

/// **The flags a package may hand back, and nothing else.**
///
/// This is the whole security of `package`, and the precedent is not
/// hypothetical: Go shipped the same idea — a build file naming flags for the C
/// compiler — without a filter, and it became **CVE-2018-6574**. A repository
/// could ship `attack.so` beside `// #cgo CFLAGS: -fplugin=attack.so`, and
/// `go get` loaded the plugin into the host compiler. The fix, in February 2018,
/// was exactly this list. Go's own `security.go` opens with *"We must avoid flags
/// like -fplugin=, which can allow arbitrary code execution during the build. Do
/// not make changes here without carefully considering the implications."*
///
/// Heroes has the advantage of arriving second: the list is here on the first
/// day, not retrofitted after a disclosure. A `.pc` file is **input this program
/// did not write** — installed by a package manager, editable by anyone who can
/// write to a prefix — so it is treated as input and not as configuration.
///
/// `-framework` and `-F` are on the list because that is how a package answers on
/// macOS, which is the case `link` could never spell. They take a following
/// argument, which is why they are matched as a pair rather than as a prefix.
/// **`-D` and `-U` are here on Go's own precedent, which panel 050 cited for the
/// list's existence and then under-copied** (panel 055). Go documents *"only a
/// limited set of flags are allowed, notably `-D`, `-U`, `-I`, and `-l`"*, and
/// their absence was measured to shut a real door: **28 of 285 `.pc` files on one
/// machine (10%)** answer with a flag this list rejected — `sdl2`, `sdl3`,
/// `ncurses`, `readline`, `x264`, `simdjson`, all twelve `Qt6*` — and SDL2 was
/// bindable through **neither** clause, because `package` refused `-D_THREAD_SAFE`
/// and `link` could not find the header. Each diagnostic sent the author to the
/// other.
///
/// They are safe for a reason that can be stated and tested rather than assumed:
/// `-D` and `-U` define and undefine a preprocessor macro, and neither names a
/// file, loads anything, or writes anything. That is the property the list is
/// about — not the flag's popularity.
const ALLOWED: [&str; 5] = ["-D", "-U", "-I", "-L", "-l"];
const ALLOWED_WITH_ARGUMENT: [&str; 2] = ["-framework", "-F"];

/// Ask the machine about each package, and let nothing through that is not on the
/// list.
///
/// The failure is deliberately *not* silent and deliberately *not* a fallback: a
/// package that cannot be resolved must stop the build, because the alternative
/// is linking a program that is missing exactly the library the author asked for
/// and finding out at a link error naming C symbols.
pub(super) fn resolve_packages(packages: &[String]) -> Result<Vec<String>, String> {
    let mut flags: Vec<String> = Vec::new();
    for package in packages {
        // The name reaches `pkg-config` as one argument and never as a shell
        // string, so a package name cannot become a second command.
        let mut ask = Command::new("pkg-config");
        ask.arg("--cflags").arg("--libs").arg(package);
        let answered = match ask.output() {
            Ok(answered) => answered,
            Err(e) => {
                return Err(format!(
                    "heroes-ffi-package `{package}`: {e}\n  \
                     `package \"{package}\"` needs `pkg-config` on this machine — install it, or name the library directly with `link`"
                ))
            }
        };
        if !answered.status.success() {
            let said = String::from_utf8_lossy(&answered.stderr);
            return Err(format!(
                "heroes-ffi-package `{package}` is not installed on this machine\n  \
                 pkg-config said: {}\n  \
                 install its development files, or name the library directly with `link`",
                said.trim()
            ));
        }
        let said = String::from_utf8_lossy(&answered.stdout).into_owned();
        let mut words = said.split_whitespace();
        while let Some(word) = words.next() {
            if ALLOWED_WITH_ARGUMENT.contains(&word) {
                let Some(value) = words.next() else {
                    return Err(format!(
                        "heroes-ffi-package `{package}` answered with `{word}` and nothing after it"
                    ));
                };
                flags.push(word.to_string());
                flags.push(value.to_string());
                continue;
            }
            if ALLOWED.iter().any(|allowed| word.starts_with(allowed) && word.len() > allowed.len()) {
                flags.push(word.to_string());
                continue;
            }
            // **`-Wl,-framework,<name>` is a spelling of something already on the
            // list, and only that spelling** (found after panel 055 closed, when
            // `-D` alone left SDL2 one flag short of bindable).
            //
            // SDL2's own `.pc` answers `-Wl,-framework,Cocoa`, which is
            // `-framework Cocoa` handed to the linker the long way. Four packages
            // of 285 on this machine need it. A blanket `-Wl,` would **not** be
            // safe and is not given: `-Wl,` passes anything to the linker, and the
            // property this list is about is that a word names no file, loads
            // nothing and writes nothing. This form is checked whole — three
            // comma-separated parts, the middle one `-framework` — which is a fact
            // about the value rather than a premise about what packages tend to
            // send (CLAUDE.md §11).
            // **`-Wl,-rpath,<dir>` — a directory the *loader* searches, and the
            // second thing in one hour to show that this list's property test is
            // about build time** (2026-08-14, after panel 055 closed). SDL3's own
            // `.pc` answers `-Wl,-rpath,/opt/homebrew/lib -lSDL3`, and without it
            // `package "sdl3"` is refused for a flag that names a directory.
            //
            // The list asks: *does this word name a file, load anything, or write
            // anything?* An rpath does none of the three at build time, and it
            // **does** change what is loaded at run time. What it costs, written
            // down rather than waved past: a hostile `.pc` saying
            // `-L/tmp/x -lfoo` already picks the library at build time, so the
            // rpath is not a new capability — but it defers the choice, and a
            // library can be **swapped after the build** by anyone who can write
            // to that directory, where the build-time route needs the build itself
            // compromised. That escalation is real and is queued in `DECIDE.md`
            // together with the `SDL2main` finding, because the two are one
            // question: this list cannot see runtime.
            //
            // The form is checked whole: three comma-separated parts, the middle
            // one `-rpath`, and the last a path that is not itself a flag.
            if let Some(rest) = word.strip_prefix("-Wl,-rpath,") {
                if !rest.is_empty() && !rest.contains(',') && !rest.starts_with('-') {
                    flags.push("-Wl,-rpath,".to_string() + rest);
                    continue;
                }
            }
            if let Some(rest) = word.strip_prefix("-Wl,-framework,") {
                if !rest.is_empty() && !rest.contains(',') && !rest.starts_with('-') {
                    flags.push("-framework".to_string());
                    flags.push(rest.to_string());
                    continue;
                }
            }
            return Err(format!(
                "heroes-ffi-package `{package}` answered with `{word}`, which this compiler does not pass on\n  \
                 only `-D`, `-U`, `-I`, `-L`, `-l`, `-F`, `-framework`, `-Wl,-framework,<name>` and `-Wl,-rpath,<dir>` are accepted: everything else is a flag a package file could use to run code during the build (Go's CVE-2018-6574)\n  \
                 name the library directly with `link` if you need it"
            ));
        }
    }
    Ok(flags)
}
