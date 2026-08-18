//! Comments, and the blank lines that give them their meaning (design.md §4.1,
//! §4.15).
//!
//! Split out of `fmt.rs` by the §11 sweep, and it is the formatter's most
//! dangerous concern: **a comment is the one thing in the file the tree does not
//! keep.** `parse` hands back every comment span and they are interleaved by line
//! here, so a mistake in this file does not produce bad output — it produces
//! *missing* output, and the round-trip test that guards everything else cannot see
//! what was never printed.
//!
//! **A blank line is content, not layout.** §4.1 makes a blank line between a
//! comment and the declaration below it the difference between documentation and an
//! ordinary remark, so it is preserved *exactly* rather than normalised. That is
//! the whole reason `last_line` exists on `Fmt`: it answers "was there a gap here?"
//! without a second pass over the source.
//!
//! `fmt_stmt.rs` and `fmt_break.rs` call both entry points, and both have paid for
//! passing the wrong line — the comments in those files record which.

use crate::source::{Source, Span};

use super::fmt::Fmt;

impl Fmt {
    /// Print every comment that sits above source line `line`, at `indent`,
    /// preserving a single blank line wherever the source had one.
    pub(super) fn comments_before(
        &mut self,
        src: &Source,
        comments: &[Span],
        line: u32,
        indent: usize,
    ) {
        while self.next_comment < comments.len() {
            let span = comments[self.next_comment];
            let comment_line = src.line_of(span.start);
            if comment_line >= line {
                return;
            }
            if self.last_line > 0 && comment_line > self.last_line + 1 {
                self.blank_line();
            }
            self.line(indent, src.slice(span).trim_end());
            self.last_line = comment_line;
            self.next_comment += 1;
        }
    }

    /// A comment that sits on the same line as what was just printed goes at
    /// the end of that line, two spaces out.
    pub(super) fn trailing_comment(&mut self, src: &Source, comments: &[Span], line: u32) {
        if self.next_comment >= comments.len() {
            return;
        }
        let span = comments[self.next_comment];
        let comment_line = src.line_of(span.start);
        if comment_line != line {
            return;
        }
        let text = src.slice(span).trim_end();
        // Splice it before the newline the statement already wrote.
        if self.out.ends_with('\n') {
            self.out.pop();
        }
        self.out.push_str("  ");
        self.out.push_str(text);
        self.out.push('\n');
        self.next_comment += 1;
    }
}
