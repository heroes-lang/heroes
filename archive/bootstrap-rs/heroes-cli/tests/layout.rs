//! CLAUDE.md §11's per-file ceiling, enforced instead of remembered.
//!
//! **Why this is a test and not a line in a checklist** (author decision
//! 2026-08-15). §11 says *"split a module before it passes ~300 lines"*, and the
//! rule's own credibility argument is that a rule twenty-two files break is a
//! preference. The 2026-08-12 sweep took the tree to zero breaches in one run — and
//! ordinary work put four files back over it within three days, unnoticed, because
//! nothing was watching. A one-off sweep measures the day it runs; this measures
//! every day.
//!
//! It is a **test** rather than a `heroes` subcommand because §10's stopping rule
//! says so in its own words: *nothing if two existing invocations already compose to
//! it*, and `find crates -name '*.rs' | xargs wc -l | sort -rn` is that composition.
//! What a subcommand would add is not the measurement, it is the *obligation* — and
//! an obligation is what a test already is. It is the same shape as
//! `emit/tests/gate.rs` asserting a milestone id never reaches user-visible output,
//! and as `clippy.toml` enforcing the Cyclone rule: this repository does not keep
//! rules that only people check.
//!
//! **The ceiling binds the compiler's code and not its tests** (§11 as amended
//! 2026-08-14): the number governs what a reader must hold in their head, and a test
//! file is read one case at a time.

use std::path::{Path, PathBuf};

/// §11's number. A threshold to think at, not a limit to round — which is why
/// `DECIDED` exists and why entries in it carry a reason rather than a number alone.
const CEILING: usize = 300;

/// Files the author has looked at and allowed past `CEILING`, each with the number
/// that was decided and **why**.
///
/// The reason is the load-bearing column. A bare exception list decays into the
/// breach list it was meant to replace; a reason can be read later and disagreed
/// with. An entry here is also a *ceiling*, not a licence — drifting past the
/// decided number is exactly the silence this test exists to break, and it is how
/// `toolchain.rs` reached 425 against a 400 settled by name.
const DECIDED: [(&str, usize, &str); 1] = [(
    "crates/heroes-cli/src/commands/toolchain.rs",
    400,
    "author decision 2026-08-14 — the cut that would take it under 300 runs through \
     the cache key that `link` and `runtime_object` share, and a file split against \
     its own seam is harder to read than a long one. (It reached 425 against that \
     400 and this test is what said so; `flags.rs` came out on a different seam \
     entirely — what clang enforces, which knows no path — and it is 360 now)",
)];

fn repository() -> PathBuf {
    Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../..")).canonicalize().expect("the repository")
}

/// Every `.rs` file under `crates/` that is compiler code rather than test code.
///
/// **Two shapes are tests and both are excluded by what they are, not by a list**:
/// anything under a `tests/` directory, and any `tests.rs` — this repository's one
/// spelling for a `#[cfg(test)] mod tests;` sibling. A name-keyed exclusion would go
/// stale the day a file moves; these are facts about where the file sits.
fn compiler_sources(dir: &Path, found: &mut Vec<PathBuf>) {
    for entry in std::fs::read_dir(dir).expect("a readable directory").flatten() {
        let path = entry.path();
        if path.is_dir() {
            if path.file_name().is_some_and(|n| n == "tests" || n == "target") {
                continue;
            }
            compiler_sources(&path, found);
        } else if path.extension().is_some_and(|e| e == "rs")
            && path.file_name().is_none_or(|n| n != "tests.rs")
        {
            found.push(path);
        }
    }
}

#[test]
fn no_compiler_file_passes_the_line_ceiling() {
    let root = repository();
    let mut files = Vec::new();
    compiler_sources(&root.join("crates"), &mut files);
    assert!(files.len() > 100, "the walk found {} files — it is not reaching the tree", files.len());

    let mut over: Vec<String> = Vec::new();
    for path in &files {
        let lines = std::fs::read_to_string(path).expect("a readable source").lines().count();
        let relative = path.strip_prefix(&root).unwrap_or(path).to_string_lossy().replace('\\', "/");
        let (limit, why) = DECIDED
            .iter()
            .find(|(name, _, _)| *name == relative)
            .map(|(_, limit, why)| (*limit, Some(*why)))
            .unwrap_or((CEILING, None));
        if lines > limit {
            over.push(match why {
                // A decided exception exceeded is worse than an ordinary breach: a
                // number the author settled by name has moved without anyone
                // deciding it should.
                Some(why) => format!(
                    "  {relative}: {lines} lines, past its DECIDED {limit}\n      \
                     the decision was: {why}\n      \
                     either bring it back under {limit} or re-decide the number in DECIDED"
                ),
                None => format!("  {relative}: {lines} lines, past §11's {CEILING}"),
            });
        }
    }

    assert!(
        over.is_empty(),
        "CLAUDE.md §11: *split a module before it passes ~300 lines*, because \"the author \
         must be able to open any file and read it without drowning\".\n\n{}\n\n\
         This is the only live watch on the compiler's size — design.md §1.6 refused a global \
         budget, so §11's per-file number is what replaced it. Split along a seam that names a \
         concern, never at a line count, and where the file is in `emit/` assert the emitted C \
         is byte-identical. If a file genuinely should stay long, add it to `DECIDED` with the \
         reason, which is a decision and belongs in the record.",
        over.join("\n")
    );
}
