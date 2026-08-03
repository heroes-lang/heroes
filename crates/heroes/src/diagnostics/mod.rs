//! Diagnostics are a deliverable, not plumbing (design.md §4.17): an error
//! carries everything needed to fix the program without opening another
//! file. Every `Fix` is tagged — only `Certain` fixes may ever be
//! machine-applied (CLAUDE.md § Error discipline).
//!
//! The rich renderer is an M3d deliverable; until then `render_line` is the
//! one-line format the golden tests pin.

use crate::source::{Source, Span};

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Certainty {
    Certain,
    Guess,
}

#[derive(Debug)]
pub struct Fix {
    pub title: String,
    pub replacement: String,
    pub span: Span,
    pub certainty: Certainty,
}

#[derive(Debug)]
pub struct Diagnostic {
    /// Stable snake_case code (the same convention as `fail` codes).
    pub code: String,
    pub message: String,
    pub span: Span,
    pub fixes: Vec<Fix>,
}

impl Diagnostic {
    pub fn new(code: &str, message: String, span: Span) -> Diagnostic {
        Diagnostic { code: code.to_string(), message, span, fixes: Vec::new() }
    }

    /// `file:line:col: error[code]: message`
    pub fn render_line(&self, src: &Source) -> String {
        let (line, col) = src.line_col(self.span.start);
        format!("{}:{}:{}: error[{}]: {}", src.name, line, col, self.code, self.message)
    }
}
