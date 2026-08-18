//! From a byte offset to a place a person can read (design.md §4.17, §3.1).
//!
//! Split out of `mod.rs` by the §11 sweep. `mod.rs` answers *which file* an offset
//! belongs to; this answers *where in it* — line, column, and the text a span
//! covers. Every diagnostic's caret and every `#line` directive ends up here.
//!
//! The line table is built once and binary-searched, which is what keeps a caret
//! from costing a scan of the whole program per diagnostic.

use super::{Source, Span};

impl Source {
    /// The 1-based line **within its own file**, for a diagnostic or a `#line`
    /// that names that file rather than the root.
    pub fn file_line_of(&self, offset: u32) -> u32 {
        let (line, _) = self.line_col(offset);
        line.saturating_sub(self.file(offset).lines_before).max(1)
    }

    /// Where an offset is, **as a reader has to be told it**: the file that
    /// contains it, the line within that file, and the column.
    ///
    /// Every diagnostic goes through here. Before M-module-namespace the two renderers each
    /// printed `src.name` and `line_col` directly, which was the same answer
    /// while there was one file and became a *false* one the moment there were
    /// several — a diagnostic in `geom.hero` naming `main.hero` at a line number
    /// from the concatenated text. Well-formed and wrong is the failure mode
    /// this table exists to prevent, so there is one function and no caller
    /// assembles the triple itself.
    pub fn locate(&self, offset: u32) -> (&str, u32, u32) {
        let (_, col) = self.line_col(offset);
        (&self.file(offset).name, self.file_line_of(offset), col)
    }

    pub fn slice(&self, span: Span) -> &str {
        &self.text[span.start as usize..span.end as usize]
    }

    /// Byte offset of the first byte of the line containing `offset`. The one
    /// caller is the caret, which pads with the line's own leading whitespace.
    pub fn line_start_of(&self, offset: u32) -> u32 {
        let line = match self.line_starts.binary_search(&offset) {
            Ok(i) => i,
            Err(i) => i - 1,
        };
        self.line_starts[line]
    }

    /// The 1-based line in the **whole text**, which is what a printer's relative
    /// arithmetic needs and what almost every caller of `line_col` was after.
    ///
    /// It exists so that nobody reaches for a column by accident: sweep 001 found
    /// eight callers building a user-visible location by hand under a doc comment
    /// that has said *"there is one function and no caller assembles the triple
    /// itself"* since M-module-namespace. `locate` answers the reader's question; this answers
    /// the printer's; `line_col` is `pub(crate)` and answers neither on its own.
    pub fn line_of(&self, offset: u32) -> u32 {
        self.line_col(offset).0
    }

    /// 1-based (line, column) in the whole text; **the column counts characters**.
    ///
    /// It counted bytes until 2026-08-12, justified by a consumer that does not
    /// consume columns: `#line` carries a file and a line and never a column, so
    /// the reason written here was already dead when it was written. What does
    /// consume the column is the reader — `locate`'s triple, and the caret drawn
    /// under the source line — and for both of those a byte is not a column. The
    /// caret's padding was converted to characters by the tab repair, so the two
    /// halves of one message disagreed: `at f.hero:2:21` under a caret standing
    /// at column 18. Every other caller wants the line and discards this.
    pub(crate) fn line_col(&self, offset: u32) -> (u32, u32) {
        let line = match self.line_starts.binary_search(&offset) {
            Ok(i) => i,
            Err(i) => i - 1,
        };
        let start = self.line_starts[line] as usize;
        let column = self.text[start..offset as usize].chars().count() as u32 + 1;
        (line as u32 + 1, column)
    }
}

/// Byte offset of the first byte of each line. `starts[0] == 0`, and the length
/// is the number of lines, which is what makes it double as a line count.
pub(super) fn starts_of(text: &str) -> Vec<u32> {
    let mut starts = vec![0u32];
    for (i, b) in text.bytes().enumerate() {
        if b == b'\n' {
            starts.push((i + 1) as u32);
        }
    }
    starts
}
