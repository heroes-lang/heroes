//! Two invariants about milestone identifiers, asserted over the repository itself.
//!
//! CLAUDE.md §14 says milestones are named rather than numbered, and that the record
//! keeps the numbers it was written with. Both halves of that are checkable, and
//! neither was, which is why the rename to names could have decayed the day after it
//! landed: nothing stopped the next commit writing `M9`.
//!
//! Reading the repository from a test is an existing convention here, not a new one —
//! `resolve/tests/spec.rs` reads `spec/heroes-spec.md`, and four acceptance tests read
//! `design.md` (CLAUDE.md §9: an invariant that must hold on every accepted input is
//! asserted over the corpus rather than over cases somebody thought of).

use std::path::{Path, PathBuf};

fn repo() -> PathBuf {
    Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../..")).canonicalize().expect("the repo root")
}

/// Directories and files that are **dated records**: a panel, a journal, a
/// measurement, a defect report, a story beat, a golden fixture, the design log.
/// They keep the identifiers they were written with, and appending to one uses that
/// file's vocabulary (CLAUDE.md §14). `vendor/` holds third-party byte tables whose
/// BPE bigrams contain `M6`-shaped pairs, and `editors/vscode/icons/` holds SVG path
/// data where `M14` is a moveto command.
const RECORD: &[&str] = &[
    "docs/panel",
    "docs/journal",
    "docs/measurements",
    "docs/defects",
    "docs/book",
    "tests/golden",
    "DESIGN-LOG.md",
    "vendor",
    "editors/vscode/icons",
    "site/index.html",
    "target",
    "build",
    ".git",
];

/// Files whose milestone identifiers are **quotations of the retired spelling**, kept
/// on purpose so a test can prove the legacy form is still recognised.
const QUOTES_THE_OLD_SPELLING: &[&str] = &[
    "crates/heroes/src/emit/tests/gate.rs",
    "crates/heroes-cli/tests/milestones.rs",
    // Its module doc quotes the footer it deleted, which is the only way to record
    // what was wrong with it.
    "crates/heroes-cli/src/cli/help.rs",
    "docs/debrief/QUEUE.md",
    "docs/ROADMAP.md",
    "CLAUDE.md",
];

fn walk(dir: &Path, root: &Path, out: &mut Vec<PathBuf>) {
    let entries = std::fs::read_dir(dir).expect("a readable directory");
    for entry in entries.flatten() {
        let path = entry.path();
        let rel = path.strip_prefix(root).expect("inside the repo").to_string_lossy().to_string();
        if RECORD.iter().any(|r| rel == *r || rel.starts_with(&format!("{r}/"))) {
            continue;
        }
        if path.is_dir() {
            walk(&path, root, out);
        } else if QUOTES_THE_OLD_SPELLING.iter().all(|q| rel != *q) {
            out.push(path);
        }
    }
}

/// Is there an `M` followed by a digit at a word boundary? The same boundary the
/// rename used: nothing alphanumeric, `_` or `-` before the `M`, so `ARM64` is not a
/// milestone and neither is `STAC-M3`, kdb+'s benchmark suite — the one genuine
/// false positive a `\b`-anchored search hits, and the reason this is a lookbehind.
fn names_a_numbered_milestone(text: &str) -> Option<String> {
    let bytes = text.as_bytes();
    for (i, &b) in bytes.iter().enumerate() {
        if b != b'M' {
            continue;
        }
        let opens = i == 0 || {
            let p = bytes[i - 1];
            !p.is_ascii_alphanumeric() && p != b'_' && p != b'-'
        };
        if opens && bytes.get(i + 1).is_some_and(u8::is_ascii_digit) {
            let end = (i + 4).min(bytes.len());
            return Some(String::from_utf8_lossy(&bytes[i..end]).to_string());
        }
    }
    None
}

/// No living file names a numbered milestone. This is the invariant the rename bought,
/// and without it the rename is a one-time cleanup that rots on the next commit.
///
/// If this fires on a file you just wrote: the identifier is a name now — the map is
/// `docs/ROADMAP.md` § The names and the algorithm is CLAUDE.md §14. If the file is a
/// dated record, add it to `RECORD` and say why in the commit.
#[test]
fn no_living_file_names_a_numbered_milestone() {
    let root = repo();
    let mut files = Vec::new();
    walk(&root, &root, &mut files);
    assert!(files.len() > 200, "the walk found only {} files — it is not walking", files.len());

    let mut offenders = Vec::new();
    for file in &files {
        let Ok(text) = std::fs::read_to_string(file) else { continue };
        if let Some(found) = names_a_numbered_milestone(&text) {
            let rel = file.strip_prefix(&root).expect("inside the repo");
            offenders.push(format!("{}: {found}", rel.display()));
        }
    }
    assert!(offenders.is_empty(), "a living file names a numbered milestone:\n{}", offenders.join("\n"));
}

/// No golden's expected output names a milestone in **either** spelling. 41 files of
/// byte-pinned diagnostic text is where a leak from any diagnostic would surface —
/// not only from the emit gate, which is the only one with a guard of its own
/// (`emit/tests/gate.rs`). §4.17's standard: everything needed without opening another
/// file, and a milestone identifier is a pointer into a file the reader does not have.
#[test]
fn no_expected_diagnostic_names_a_milestone() {
    let root = repo();
    let dir = root.join("tests/golden");
    let mut expected = Vec::new();
    walk_all(&dir, &mut expected);
    assert!(expected.len() > 30, "only {} .expected files found", expected.len());

    let mut offenders = Vec::new();
    for file in &expected {
        let text = std::fs::read_to_string(file).expect("a readable expectation");
        let named = names_a_numbered_milestone(&text).is_some() || text.contains("(M-");
        if named {
            offenders.push(file.strip_prefix(&root).expect("inside").display().to_string());
        }
    }
    assert!(offenders.is_empty(), "a diagnostic names a milestone:\n{}", offenders.join("\n"));
}

fn walk_all(dir: &Path, out: &mut Vec<PathBuf>) {
    let Ok(entries) = std::fs::read_dir(dir) else { return };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            walk_all(&path, out);
        } else if path.extension().is_some_and(|e| e == "expected") {
            out.push(path);
        }
    }
}

/// The boundary rule has a test that makes it fire, in both directions (CLAUDE.md §9).
/// The negatives are the ones that matter: they are strings this repository really
/// contains, and a check that flagged them would be deleted by whoever it blocked.
#[test]
fn the_boundary_rule_catches_the_numbers_and_nothing_else() {
    for named in ["scheduled for M8c", "at M5b the increfs", "M0 was day zero", "(M6+)"] {
        assert!(names_a_numbered_milestone(named).is_some(), "not caught: {named}");
    }
    for clean in [
        "STAC-M3 Antuco benchmarks",
        "INT64_MIN % -1 does not trap on ARM64",
        "the RM64 encoding",
        "M-selfhost-fixpoint is v1",
        "hero_desc_str",
    ] {
        assert!(names_a_numbered_milestone(clean).is_none(), "false positive: {clean}");
    }
}
