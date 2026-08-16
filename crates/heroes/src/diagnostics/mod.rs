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

pub(crate) mod render;

#[cfg(test)]
mod tests;

pub use render::{render, render_all};

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Certainty {
    Certain,
    Guess,
}

/// What kind of thing is being reported. Two, and the second one is **not** a
/// claim about the program (panel 020).
///
/// GCC has had this exact distinction since version 2.5.8: `sorry` sits in
/// `kinds.def` beside `error`, `warning` and `ice`, `sorry_at` takes a
/// `location_t` exactly as `error_at` does, and `if (sorrycount) exit
/// (FATAL_EXIT_CODE)` — the *same* status as an error. GHC's `Sorry` carries the
/// reasoning in a comment: "The user tickled something that's known not to work
/// yet, but we're not counting it as a bug." Both kept it inside the ordinary
/// diagnostic path, and the panel's historian could find no compiler that built a
/// parallel channel instead.
///
/// The local reason is mechanical rather than stylistic: CLAUDE.md §9's `#~
/// <code>` invariant keys off diagnostic *codes*, so a report that lives outside
/// this type is a test class outside the invariant — and §4.17's renderer, the
/// JSON schema and multi-diagnostic ordering would each need a second
/// implementation.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Kind {
    /// The program is wrong.
    Error,
    /// The program is fine and **this compiler is unfinished**. Carries no fixes
    /// — no edit to the file will help — is never dropped by `--permissive`, and
    /// never applied by `--apply`.
    Unsupported,
}

impl Kind {
    /// The word that goes where `error` goes. It is the first thing read, so it
    /// is the thing that has to be true.
    pub fn word(self) -> &'static str {
        match self {
            Kind::Error => "error",
            Kind::Unsupported => "unsupported",
        }
    }
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
    pub kind: Kind,
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
            kind: Kind::Error,
            code: code.to_string(),
            message,
            span,
            fixes: Vec::new(),
            notes: Vec::new(),
        }
    }

    /// A form the backend cannot emit yet. The message names the **capability**
    /// and never the milestone: `(M-strings-ownership)` resolves only in `docs/ROADMAP.md`, a
    /// file the reader does not have, and §4.17's standard is everything needed
    /// without opening another file. The panel's llm-ergonomist read `(M-strings-ownership)` as
    /// an internal tracker id, grepped the repository for it, and then told its
    /// user the toolchain was broken.
    /// The `code` names the *capability*, so a harness can count which one blocked
    /// what; the `kind` is what a fix loop keys on, because it is the fact that
    /// terminates the loop.
    ///
    /// **The constructor no longer attaches *"no change to this file will fix
    /// this"*, and that is panel 082's R1.** It did, unconditionally, for every row
    /// this kind can carry — and it was **false on three of the five**, measured:
    /// `[ptr]` compiles the moment the pointer is held in a one-field record (exit
    /// 0 against real SQLite 3.51.0), `[()]` compiles as `[i64]`, and `sort` inside
    /// a generic compiles the moment the call instantiates it at an ordered type.
    /// A note that denies a repair the author can make is worse than no note: it
    /// does not merely fail §4.17's *everything needed to fix the program*, it
    /// tells them to stop looking. GCC's own taxonomy draws exactly this line —
    /// `DIAGNOSTIC_LEVEL_SORRY` is *"a problem where the input is **valid**, but
    /// the tool isn't able to handle it"* — and a row keyed on **this program's
    /// types** is not that.
    ///
    /// So the note is the **row's** decision, made in `emit/gate.rs` where the code
    /// is known, and this constructor stays silent about it.
    pub fn unsupported(code: &str, message: String, span: Span) -> Diagnostic {
        Diagnostic {
            kind: Kind::Unsupported,
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

    /// The same, for a fix. Only `Certain` fixes are machine-applicable
    /// (CLAUDE.md §8), and the constructor tagging one is the constructor that
    /// knows whether applying it leaves a program that checks clean.
    pub fn with_fix(mut self, fix: Fix) -> Diagnostic {
        self.fixes.push(fix);
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
                // **A path in a group head is refused, and C would take it**
                // (panel 055). That makes it a thesis rule by the same test as
                // the rest of this list: nothing about the *types* is wrong, and
                // the program would build — on exactly one machine. `--permissive`
                // drops it, so metric 3 does not count a refusal the language
                // chose against a mistake the language catches.
                | "machine_locked_path"
        )
    }

    /// `file:line:col: <kind>[code]: message` — the file and line being the
    /// ones the reader can open, which is `Source::locate`'s whole job.
    pub fn render_line(&self, src: &Source) -> String {
        let (file, line, col) = src.locate(self.span.start);
        format!(
            "{}:{}:{}: {}[{}]: {}",
            file,
            line,
            col,
            self.kind.word(),
            self.code,
            self.message
        )
    }
}
