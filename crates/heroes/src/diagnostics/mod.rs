//! Diagnostics are a deliverable, not plumbing (design.md §4.17): an error
//! carries everything needed to fix the program without opening another
//! file. Every `Fix` is tagged — only `Certain` fixes may ever be
//! machine-applied (CLAUDE.md § Error discipline).
//!
//! Two renderings, and the difference is the deliverable. `render_line` is the
//! one-line form the golden tests pin and a terminal scans; `render` (in
//! `render.rs`) is §4.17's — the line as written, the span underlined, the notes
//! that carry the other end of the mistake, and the fixes with their tags. A
//! third rendering, JSON, lives in the CLI, because a schema is a surface.

use crate::source::{Source, Span};

mod render;

pub use render::{render, render_all};

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
    /// The other end of the mistake, in prose: where the declaration is, what
    /// the signature says, which cases are missing. §4.17's rule is that the
    /// error carries what is needed to fix the program *without opening another
    /// file*, and this is that file's contents, quoted.
    pub notes: Vec<String>,
}

impl Diagnostic {
    pub fn new(code: &str, message: String, span: Span) -> Diagnostic {
        Diagnostic {
            code: code.to_string(),
            message,
            span,
            fixes: Vec::new(),
            notes: Vec::new(),
        }
    }

    /// Adds a note, and hands the diagnostic back so a constructor can chain.
    pub fn with_note(mut self, note: String) -> Diagnostic {
        self.notes.push(note);
        self
    }

    /// Whether this code is one of the rules the *thesis* adds, as opposed to a
    /// rule without which the program has no meaning.
    ///
    /// `check --permissive` drops exactly these, which is what makes it the
    /// control arm design.md Part 11 needs: the same compiler with §1's
    /// argument switched off. Every code here traces to a panel — the unused
    /// and shadowing rules (§4.4), the reserved built-in names (015), the
    /// statement-position and binding-position `()` rules (003 and 017 C),
    /// `_`'s ban on variants (§4.7), the `@` marker at the call site (§4.8),
    /// and a declaration in an arm (017 D).
    pub fn is_thesis_rule(&self) -> bool {
        matches!(
            self.code.as_str(),
            "unused_binding"
                | "shadowed_binding"
                | "builtin_name_taken"
                | "discarded_value"
                | "bound_unit"
                | "wildcard_on_variant"
                | "marker_mismatch"
                | "ufcs_on_mutable"
                | "declaration_in_arm"
                | "missing_label"
                | "needs_label"
                | "wrong_label"
                | "declared_twice"
        )
    }

    /// `file:line:col: error[code]: message`
    pub fn render_line(&self, src: &Source) -> String {
        let (line, col) = src.line_col(self.span.start);
        format!("{}:{}:{}: error[{}]: {}", src.name, line, col, self.code, self.message)
    }
}
