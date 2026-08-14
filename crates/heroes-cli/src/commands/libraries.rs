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
}

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
const ALLOWED: [&str; 3] = ["-I", "-L", "-l"];
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
            return Err(format!(
                "heroes-ffi-package `{package}` answered with `{word}`, which this compiler does not pass on\n  \
                 only `-I`, `-L`, `-l`, `-F` and `-framework` are accepted: everything else is a flag a package file could use to run code during the build (Go's CVE-2018-6574)\n  \
                 name the library directly with `link` if you need it"
            ));
        }
    }
    Ok(flags)
}
