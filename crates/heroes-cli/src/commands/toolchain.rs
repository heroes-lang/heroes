//! Everything between the generated C and a running binary: where the runtime is,
//! what clang is invoked with, and what is cached where (design.md §3.1, panel
//! 020).
//!
//! **Finding the runtime is a search, and the search is why the stamp exists.**
//! An installed compiler has no repository above it, so there are three places to
//! look — and the second one, `runtime/` under the working directory, is a hazard:
//! `runtime` is one of the commonest directory names in a source tree. The panel's
//! ffi-pragmatist put a decoy there and the generated C compiled with **zero
//! warnings under `-Weverything`** and printed `0x1e` where `30` was expected. Every
//! generated unit therefore `_Static_assert`s `HERO_RUNTIME_ABI`, which turns that
//! silence into `error: use of undeclared identifier`. The search is kept, and the
//! silence is not.
//!
//! **The cache key is the whole configuration**, not the source. Measured: with
//! `hero_print_int` changed and `runtime.o` not recompiled, the relink succeeded
//! silently and printed the old bytes. And without the optimisation level in the
//! key, the two-configuration golden harness would be one configuration run twice —
//! measuring nothing while looking thorough. So the key covers the source, the
//! compiler's version, the level, and the contents of both runtime files.
//!
//! One `.o` is correct across `-O0`, `-O2`, `-flto` and the sanitisers (measured, all
//! four link and run), so the object is keyed on the runtime's own configuration and
//! shared between programs rather than copied per build directory.

use std::path::{Path, PathBuf};
use std::process::Command;

/// CLAUDE.md §7's set, in one place. `-Werror=uninitialized` is the net under the
/// emitter's hoisted prologue: a slot read on a path that never wrote it is a
/// *lowering* bug, and this is what makes it a compile error instead of a wrong
/// answer.
///
/// **`-Wconditional-uninitialized` is `-Werror=` because the corpus said so.** Panel
/// 020 adopted it with a falsifiable disposition — zero fires across the whole
/// `run/`+`emit/` corpus at both levels promotes it; one fire is a lowering bug that
/// gets a case named after it (CLAUDE.md §9) — and the measurement came back zero on
/// the first run, with clang's warnings forwarded rather than swallowed so that they
/// could. It is the strictest warning in the set and the one most likely to catch a
/// join slot the lowering forgot to write on one arm.
pub const FLAGS: [&str; 7] = [
    "-std=c11",
    "-Wall",
    "-Werror=return-type",
    "-Werror=uninitialized",
    "-Werror=format",
    "-Werror=conditional-uninitialized",
    "-fno-strict-aliasing",
];

pub struct Toolchain {
    /// The directory holding `heroes_runtime.h` and `runtime.c`.
    pub runtime: PathBuf,
    /// Where derived files go. Everything under it is reproducible from the inputs.
    pub build: PathBuf,
}

impl Toolchain {
    /// Locate the runtime, or say all three places it looked. A message that names
    /// only the failure would make the fix a source-reading exercise.
    pub fn find() -> Result<Toolchain, String> {
        let mut candidates: Vec<PathBuf> = Vec::new();
        if let Ok(from_env) = std::env::var("HEROES_RUNTIME") {
            candidates.push(PathBuf::from(from_env));
        }
        candidates.push(PathBuf::from("runtime"));
        if let Ok(exe) = std::env::current_exe() {
            for ancestor in exe.ancestors() {
                candidates.push(ancestor.join("runtime"));
            }
        }
        for candidate in &candidates {
            if candidate.join("heroes_runtime.h").is_file() && candidate.join("runtime.c").is_file()
            {
                return Ok(Toolchain {
                    runtime: candidate.clone(),
                    build: PathBuf::from("build"),
                });
            }
        }
        Err(format!(
            "cannot find the Heroes runtime (heroes_runtime.h and runtime.c).\n  \
             looked in: $HEROES_RUNTIME, ./runtime, and runtime/ beside the compiler\n  \
             set HEROES_RUNTIME=<dir> to say where it is (searched {} places)",
            candidates.len()
        ))
    }

    /// The runtime's whole contents, which are half of every cache key.
    ///
    /// **Every file, not the two entry points.** `runtime.c` is one translation
    /// unit assembled from `runtime/parts/`, so hashing only the file clang is
    /// handed would let a part be edited with the cached object still used — and
    /// a stale relink is silent, which is the one thing standing between a golden
    /// test and passing for the wrong reason.
    ///
    /// Sorted, so the key is a function of the contents and not of readdir order.
    fn runtime_text(&self) -> String {
        let mut files: Vec<PathBuf> = Vec::new();
        collect_sources(&self.runtime, &mut files);
        files.sort();
        let mut text = String::new();
        for file in &files {
            text.push_str(&std::fs::read_to_string(file).unwrap_or_default());
            text.push('\u{1}');
        }
        text
    }

    /// Where this program's artifacts live: `build/<hash>/`.
    ///
    /// design.md §3.1's own shape. The hash is what makes two files with the same
    /// stem, or the same file at two optimisation levels, different builds.
    pub fn dir_for(
        &self,
        source: &str,
        text: &str,
        level: &str,
        sanitize: bool,
    ) -> Result<PathBuf, String> {
        let key = digest(&format!(
            "{}\u{1}{}{}\u{1}{}\u{1}{}\u{1}{}",
            heroes::VERSION,
            level,
            if sanitize { "+san" } else { "" },
            source,
            text,
            self.runtime_text()
        ));
        let dir = self.build.join(&key);
        std::fs::create_dir_all(&dir).map_err(|e| format!("cannot create {}: {e}", dir.display()))?;
        Ok(dir)
    }

    /// Compile the runtime once and cache it, keyed on its own contents and the
    /// level. A stale relink is silent, so the key is the only thing standing
    /// between a golden test and passing for the wrong reason.
    pub fn runtime_object(&self, level: &str, sanitize: bool) -> Result<PathBuf, String> {
        let key = digest(&format!(
            "{}\u{1}{level}\u{1}{sanitize}\u{1}{}",
            heroes::VERSION,
            self.runtime_text()
        ));
        std::fs::create_dir_all(&self.build)
            .map_err(|e| format!("cannot create {}: {e}", self.build.display()))?;
        let object = self.build.join(format!("runtime-{key}.o"));
        if object.is_file() {
            return Ok(object);
        }
        let mut clang = Command::new("clang");
        clang.args(FLAGS).arg(level).args(sanitizers(sanitize)).arg("-c");
        clang.arg(self.runtime.join("runtime.c"));
        clang.arg("-I").arg(&self.runtime);
        clang.arg("-o").arg(&object);
        run(clang, "compiling the runtime")?;
        Ok(object)
    }

    /// Compile one generated translation unit and link it with the runtime.
    pub fn link(
        &self,
        c_file: &Path,
        object: &Path,
        binary: &Path,
        level: &str,
        sanitize: bool,
    ) -> Result<(), String> {
        let mut clang = Command::new("clang");
        clang.args(FLAGS).arg(level).args(sanitizers(sanitize));
        clang.arg(c_file).arg(object);
        clang.arg("-I").arg(&self.runtime);
        clang.arg("-o").arg(binary);
        run(clang, "compiling the generated C")
    }
}

/// `--sanitize`'s flags, and what they are for.
///
/// **Not a leak detector.** AddressSanitizer's is missing on Darwin arm64
/// (`detect_leaks is not supported on this platform`, exit 134), and a program leaking
/// 999 blocks exits 0 in silence under it — measured twice, by two panel judges
/// independently. Leaks are caught by `hero_runtime_check_leaks()` in the generated
/// `main`. What these two *do* catch is use-after-free and double-free, and they caught
/// two real bugs in the panel's own hand-written C on their first run, which no output
/// comparison would have.
/// Every `.c` and `.h` under `dir`, one level of nesting deep — which is what
/// `runtime/` is: the entry points beside a `parts/` directory. Recursive rather
/// than hard-coded so that adding a part is not a second edit somewhere else.
fn collect_sources(dir: &Path, into: &mut Vec<PathBuf>) {
    let Ok(entries) = std::fs::read_dir(dir) else { return };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            collect_sources(&path, into);
        } else if matches!(path.extension().and_then(|e| e.to_str()), Some("c") | Some("h")) {
            into.push(path);
        }
    }
}

fn sanitizers(sanitize: bool) -> Vec<String> {
    if sanitize {
        vec!["-fsanitize=address,undefined".to_string(), "-g".to_string()]
    } else {
        Vec::new()
    }
}

/// clang's own words, kept verbatim. A failure here says **the compiler is wrong**
/// — the generated C is not the author's text — so the caller exits 2 and says so.
///
/// A *warning* is forwarded rather than swallowed, and that is the whole point of
/// the flag set: `-Wconditional-uninitialized` is in it without `-Werror` on a
/// falsifiable disposition (zero fires across the corpus promotes it; one fire is a
/// lowering bug with a case named after it), and a warning nobody ever sees cannot
/// falsify anything.
fn run(mut clang: Command, what: &str) -> Result<(), String> {
    let output = clang
        .output()
        .map_err(|e| format!("cannot run clang: {e} — try `heroes doctor`"))?;
    if output.status.success() {
        let noise = String::from_utf8_lossy(&output.stderr);
        if !noise.trim().is_empty() {
            eprint!("{noise}");
        }
        return Ok(());
    }
    Err(format!(
        "{what} failed:\n{}{}",
        String::from_utf8_lossy(&output.stderr),
        String::from_utf8_lossy(&output.stdout)
    ))
}

/// FNV-1a, 64 bits, as sixteen hex digits.
///
/// Vendored rather than depended on, like the tokenisers in `measure/`: the whole
/// pipeline has zero dependencies and a cache key is not the place to acquire the
/// first one. Deterministic across runs and machines — no seed, no `RandomState`,
/// which `BTreeMap`-only (CLAUDE.md §5) exists to guarantee for the same reason.
pub fn digest(text: &str) -> String {
    let mut hash: u64 = 0xcbf2_9ce4_8422_2325;
    for byte in text.as_bytes() {
        hash ^= *byte as u64;
        hash = hash.wrapping_mul(0x0000_0100_0000_01b3);
    }
    format!("{hash:016x}")
}
