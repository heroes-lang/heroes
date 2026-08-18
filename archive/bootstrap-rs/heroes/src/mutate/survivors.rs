//! What a surviving mutant *was* — the site, not the whole program.
//!
//! `mod.rs` measures; this file makes one number attributable. Measurement 002
//! recorded two per-operator rates that moved between runs (`swap-args` 30 → 44,
//! `wildcard-variant` 13 → 39) and **declined to explain either**, because
//! attributing a delta to a property of the new programs needs the mutants behind
//! it and `heroes mutate` printed rates alone. CLAUDE.md §12 says measurement beats
//! opinion, and a number nobody can attribute is an opinion with a decimal point.
//!
//! **The site is derived from the two texts, not carried from the edit.** An
//! operator hands back a whole mutated program, so the site is the first line where
//! the two differ — a fact about the pair in hand, which is what CLAUDE.md §11 asks
//! a narrowing to rest on. The alternative was threading a span out of all twelve
//! edits in `edits.rs`, which buys nothing here: an edit that deletes a line
//! (`drop-case`) still differs first at the line it deleted.

/// One mutant the compiler accepted, and where the mistake was made.
pub struct Survivor {
    pub operator: String,
    pub file: String,
    /// 1-based, in the *original* program — the line a reader would open.
    pub line: usize,
    /// The original line, or `None` where the mutant added one.
    pub before: Option<String>,
    /// The mutated line, or `None` where the mutant deleted one.
    pub after: Option<String>,
}

impl Survivor {
    /// Where the two texts first disagree.
    ///
    /// Returns `None` only if they are identical, which an operator is not supposed
    /// to produce — and if one ever does, a survivor with no site is more honest
    /// than a site invented for it.
    pub fn locate(operator: &str, file: &str, original: &str, mutant: &str) -> Option<Survivor> {
        let before: Vec<&str> = original.lines().collect();
        let after: Vec<&str> = mutant.lines().collect();
        let at = (0..before.len().max(after.len()))
            .find(|i| before.get(*i) != after.get(*i))?;
        Some(Survivor {
            operator: operator.to_string(),
            file: file.to_string(),
            line: at + 1,
            before: before.get(at).map(|l| l.to_string()),
            after: after.get(at).map(|l| l.to_string()),
        })
    }
}

/// The survivors as text, grouped by operator — written to be pasted into a
/// `docs/measurements/NNN.md` beside the table that raised the question.
///
/// Empty is a result and says so: an operator with no survivors is the outcome the
/// thesis predicts, and a silent blank would read as a broken flag.
pub fn report(survivors: &[Survivor]) -> String {
    if survivors.is_empty() {
        return "No surviving mutants: every mistake in this corpus is a compile error.\n"
            .to_string();
    }
    let mut out = String::new();
    let mut operator = String::new();
    for survivor in survivors {
        if survivor.operator != operator {
            operator = survivor.operator.clone();
            out.push_str(&format!("\n{operator}\n"));
        }
        out.push_str(&format!("  {}:{}\n", survivor.file, survivor.line));
        if let Some(before) = &survivor.before {
            out.push_str(&format!("    - {}\n", before.trim_end()));
        }
        if let Some(after) = &survivor.after {
            out.push_str(&format!("    + {}\n", after.trim_end()));
        }
    }
    out.push_str(
        "\nEach `+` line is a program this compiler accepts and should not: one\nplausible mistake, made once, that no diagnostic saw (design.md Part 11).\n",
    );
    out
}
