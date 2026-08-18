//! `heroes measure` — counting the spec against design.md §1.6's ceiling.
//!
//! §1.6 sets a **measured** budget, and Part 10 has instructed from the
//! first draft: *"count it with a real tokeniser, not by estimation — the
//! BPE vocabulary contains arbitrary choices nobody predicts."* For two
//! days the repo defended the budget with a words × 1.33 heuristic instead,
//! because the count was assumed to need an API key. It does not (panel
//! 011): two published BPE tables, vendored and offline, do the job — and
//! the first real count found the spec 33% larger than the heuristic
//! claimed.
//!
//! | file | idea |
//! |---|---|
//! | `pretokenize.rs` | cutting text into the pieces BPE merges within |
//! | `bpe.rs` | the rank tables and the merge loop |
//! | `mod.rs` | loading the vendored instruments, and the report |
//! | `gate.rs` | the measured size, pinned, so a spec change is a red test |
//!
//! **Two instruments, and the maximum binds.** Neither is the tokeniser of
//! the model that actually reads the spec — that one is unpublished. Two
//! tables disagreeing is the honest error bar (59 tokens, ~3%, on spec v0),
//! so the report prints both and the budget verdict uses the larger.

mod bpe;
/// The gate is a test and nothing else calls it, so it compiles only under
/// `cfg(test)` — the recorded number is a record for a reader and an assertion
/// for the harness, never an input to the tool.
#[cfg(test)]
mod gate;
mod pretokenize;
mod spec;

use std::path::{Path, PathBuf};

use bpe::Ranks;

/// One instrument's reading.
pub struct Reading {
    pub instrument: String,
    /// Ranks in the table, read from the vendored file rather than
    /// asserted in a label.
    pub vocabulary: usize,
    pub tokens: usize,
}

/// What a measurement run found. `max` is what the budget is judged on.
pub struct Measurement {
    pub readings: Vec<Reading>,
}

impl Measurement {
    pub fn max(&self) -> usize {
        self.readings.iter().map(|r| r.tokens).max().unwrap_or(0)
    }

    /// The disagreement between instruments — the error bar, published
    /// rather than assumed away (panel 011).
    pub fn spread(&self) -> usize {
        let hi = self.max();
        let lo = self.readings.iter().map(|r| r.tokens).min().unwrap_or(0);
        hi - lo
    }
}

/// Count `text` with every vendored instrument found under `vendor_dir`.
pub fn measure(text: &str, vendor_dir: &Path) -> Result<Measurement, String> {
    let pieces = pretokenize::pretokenize(text);
    let mut readings = Vec::new();
    for (file, instrument, is_json) in [
        ("claude-legacy.json", "claude-legacy", true),
        ("cl100k_base.tiktoken", "cl100k_base", false),
    ] {
        let path = vendor_dir.join(file);
        let raw = std::fs::read_to_string(&path)
            .map_err(|e| format!("cannot read {}: {e}", path.display()))?;
        let ranks = if is_json {
            Ranks::from_claude_json(&raw)
        } else {
            Ranks::from_tiktoken(&raw)
        };
        if ranks.is_empty() {
            return Err(format!("{} parsed to an empty table", path.display()));
        }
        let tokens = pieces.iter().map(|p| ranks.count_piece(p.as_bytes())).sum();
        readings.push(Reading {
            instrument: instrument.to_string(),
            vocabulary: ranks.len(),
            tokens,
        });
    }
    Ok(Measurement { readings })
}

/// The repo root, resolved at compile time so the command works from any
/// directory during development. Declared exception with the same expiry
/// as `cargo` itself (CLAUDE.md § One command): at M-selfhost-fixpoint the self-hosted
/// `measure` reads its tables through Heroes' own file I/O.
pub fn workspace_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| PathBuf::from("."))
}

/// Where the vendored instruments live.
pub fn vendor_dir() -> PathBuf {
    workspace_root().join("vendor/tokenizers")
}

/// The file the §1.6 ceiling is actually about.
pub fn spec_path() -> PathBuf {
    workspace_root().join("spec/heroes-spec.md")
}

#[cfg(test)]
mod tests {
    use super::*;

    fn ranks() -> Ranks {
        let raw = std::fs::read_to_string(vendor_dir().join("claude-legacy.json"))
            .expect("the vendored claude table is present");
        Ranks::from_claude_json(&raw)
    }

    fn count(text: &str) -> usize {
        let r = ranks();
        pretokenize::pretokenize(text)
            .iter()
            .map(|p| r.count_piece(p.as_bytes()))
            .sum()
    }

    #[test]
    fn the_table_loads() {
        assert!(ranks().len() > 60_000, "claude-legacy should hold ~65k ranks");
    }

    #[test]
    fn published_vectors() {
        // Independently checked against the reference decoder before this
        // implementation existed; they pin the merge loop, not our opinion.
        assert_eq!(count("hello world"), 2);
        assert_eq!(count("tiktoken is great!"), 6);
        assert_eq!(count("antidisestablishmentarianism"), 7);
    }

    #[test]
    fn pretokenisation_is_lossless() {
        let text = "function main()\n    print((2 + 3) * 4)\n";
        assert_eq!(pretokenize::pretokenize(text).concat(), text);
    }

    #[test]
    fn the_two_instruments_agree_within_their_error_bar() {
        // Panel 011 measured spec **v0** at 1989 (claude-legacy) / 2048 (cl100k),
        // and that pair is what retired the 1.33-tokens-per-word heuristic: the
        // estimate had said 1496. Those numbers are history — v1 landed at M-rich-diagnostics —
        // so what is asserted here is the property that survives an amendment:
        // the instruments disagree by a small, published margin, and one of them
        // is the binding number.
        let spec = std::fs::read_to_string(spec_path())
            .expect("the spec must exist");
        let measured = measure(&spec, &vendor_dir()).expect("the tables load");
        assert_eq!(measured.readings.len(), 2, "two instruments, both vendored");
        assert!(
            measured.spread() * 20 < measured.max(),
            "the instruments disagree by {} on {} — that is more than 5% and one of them is wrong",
            measured.spread(),
            measured.max()
        );
    }
}
