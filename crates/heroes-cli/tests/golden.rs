//! Golden-test harness.
//!
//! design.md Part 0 calls golden tests "the single most important artifact of
//! this project and the least delegable". The tree lives at the workspace root:
//!
//!   tests/golden/check/   <name>.hero + <name>.expected  (rendered diagnostics)
//!   tests/golden/run/     <name>.hero + <name>.expected  (program output, ASan-clean)
//!
//! Discipline (CLAUDE.md § "Golden discipline"):
//!   - UPDATE_GOLDEN=1 may rewrite `run/` expectations after HUMAN review of the
//!     diff (quoted in the commit body). It is FORBIDDEN in `check/`, and must
//!     never be used to turn a red test green without reading why it was red.
//!   - Each milestone's 5 adversarial cases stay marked
//!     `# UNVERIFIED — pending debrief` until the author ratifies them;
//!     assistant-written bulk regression cases are marked as such.
//!
//! M0: both directories exist and are empty; the harness passes with zero cases.
//! From M1 on, each case is executed through the compiler and diffed.

use std::path::{Path, PathBuf};

fn workspace_root() -> PathBuf {
    // crates/heroes-cli/ -> crates/ -> workspace root
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("workspace root exists")
        .to_path_buf()
}

/// Collects `.hero`/`.expected` pairs, deterministically ordered.
/// Panics on a half-pair: an unpaired file is a broken test, not a skip.
fn collect_cases(dir: &Path) -> Vec<PathBuf> {
    let mut cases = Vec::new();
    let entries = std::fs::read_dir(dir)
        .unwrap_or_else(|e| panic!("cannot read {}: {e}", dir.display()));
    for entry in entries {
        let path = entry.expect("readable dir entry").path();
        match path.extension().and_then(|e| e.to_str()) {
            Some("hero") => {
                let expected = path.with_extension("expected");
                assert!(
                    expected.exists(),
                    "golden case {} is missing its .expected file",
                    path.display()
                );
                cases.push(path);
            }
            Some("expected") => {
                assert!(
                    path.with_extension("hero").exists(),
                    "orphan expectation: {} has no .hero source",
                    path.display()
                );
            }
            _ => {} // .gitkeep, editor droppings — ignored
        }
    }
    cases.sort(); // deterministic order, always
    cases
}

#[test]
fn golden_tree_is_well_formed() {
    let root = workspace_root().join("tests/golden");
    for sub in ["check", "run"] {
        let dir = root.join(sub);
        assert!(dir.is_dir(), "missing golden directory {}", dir.display());
        let cases = collect_cases(&dir);
        println!("golden/{sub}: {} case(s)", cases.len());
        // M1+ will compile/run each case here and diff against .expected.
    }
}
