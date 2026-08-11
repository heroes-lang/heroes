//! Source text and byte-offset spans.
//!
//! One `Source` owns one file's text; everything downstream points into it
//! with byte offsets (indices, never references — the Cyclone rule), so
//! tokens and diagnostics stay `Copy` and the text stays in one place.

/// A single input file — plus, when one is attached, the library appended to it.
pub struct Source {
    pub name: String,
    pub text: String,
    /// Byte offset of the first byte of each line; line_starts[0] == 0.
    line_starts: Vec<u32>,
    /// Where the user's text ends and the library's begins (§1.11's Tier 2,
    /// `crate::library`). `u32::MAX` when no library is attached, so every
    /// offset is a user offset and `is_library` is uniformly false.
    ///
    /// The library is APPENDED so that this boundary is the only thing anyone
    /// has to know: every user span keeps the offset and the line number it
    /// would have had alone.
    library_at: u32,
    /// The library's line offset: the number of lines before it, so a library
    /// span can be reported against the library's own numbering.
    library_line: u32,
}

/// Half-open byte range into a `Source`'s text.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct Span {
    pub start: u32,
    pub end: u32,
}

impl Span {
    /// The span covering `self` through `end` — how a node built out of
    /// several tokens (a type, a declaration) states its own extent.
    pub fn to(self, end: Span) -> Span {
        Span { start: self.start, end: end.end }
    }
}

impl Source {
    /// A file on its own, with no library attached.
    ///
    /// `library_at` is `u32::MAX` rather than `text.len()`, and the difference is
    /// one measured bug: EOF's span starts exactly at `text.len()`, so the
    /// cheaper sentinel made every end-of-file diagnostic answer "that is in the
    /// library" and moved its line.
    pub fn new(name: String, text: String) -> Source {
        let line_starts = starts_of(&text);
        Source { name, text, line_starts, library_at: u32::MAX, library_line: u32::MAX }
    }

    /// The user's program with the library appended (§1.11, `crate::library`).
    ///
    /// One blank line joins them, and it is load-bearing twice: a declaration
    /// butted onto the user's last line would be one line to the parser, and the
    /// boundary offset has to fall on a line start for `is_library` to agree
    /// with the line numbering.
    pub fn with_library(name: String, user: String, library: String) -> Source {
        let user = if user.ends_with('\n') { user } else { format!("{user}\n") };
        let library_at = user.len() as u32 + 1;
        let library_line = starts_of(&user).len() as u32 + 1;
        let text = format!("{user}\n{library}");
        let line_starts = starts_of(&text);
        Source { name, text, line_starts, library_at, library_line }
    }

    /// Is this offset in the library rather than in what the author wrote?
    ///
    /// Every caller of this is answering one of three questions: may a
    /// diagnostic point here (no — it is a compiler bug), may a `#line` claim
    /// the author's file for it (no — clang would blame a line they cannot
    /// see), and which module does the mangler put it in (`library`).
    pub fn is_library(&self, offset: u32) -> bool {
        offset >= self.library_at
    }

    /// Exactly what the author wrote, with the library and its joining blank
    /// line removed.
    ///
    /// Anything that hands a program *back* — `check --apply`, `fmt` — must go
    /// through this, or it writes the library into the author's file.
    pub fn user_text(&self) -> &str {
        if self.library_at == u32::MAX {
            return &self.text;
        }
        // One before the boundary: the blank line that joins the two.
        &self.text[..(self.library_at as usize).saturating_sub(1)]
    }

    /// The 1-based line **within the library's own text**, for a `#line` that
    /// names the library rather than the author's file.
    pub fn library_line_of(&self, offset: u32) -> u32 {
        let (line, _) = self.line_col(offset);
        line.saturating_sub(self.library_line).max(1)
    }

    pub fn slice(&self, span: Span) -> &str {
        &self.text[span.start as usize..span.end as usize]
    }

    /// 1-based (line, column); the column counts bytes, matching how the
    /// generated C's `#line` and clang both report positions.
    pub fn line_col(&self, offset: u32) -> (u32, u32) {
        let line = match self.line_starts.binary_search(&offset) {
            Ok(i) => i,
            Err(i) => i - 1,
        };
        (line as u32 + 1, offset - self.line_starts[line] + 1)
    }
}

/// Byte offset of the first byte of each line. `starts[0] == 0`, and the length
/// is the number of lines, which is what makes it double as a line count.
fn starts_of(text: &str) -> Vec<u32> {
    let mut starts = vec![0u32];
    for (i, b) in text.bytes().enumerate() {
        if b == b'\n' {
            starts.push((i + 1) as u32);
        }
    }
    starts
}
