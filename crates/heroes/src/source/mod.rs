//! Source text and byte-offset spans.
//!
//! One `Source` owns one file's text; everything downstream points into it
//! with byte offsets (indices, never references — the Cyclone rule), so
//! tokens and diagnostics stay `Copy` and the text stays in one place.

/// A single input file.
pub struct Source {
    pub name: String,
    pub text: String,
    /// Byte offset of the first byte of each line; line_starts[0] == 0.
    line_starts: Vec<u32>,
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
    pub fn new(name: String, text: String) -> Source {
        let mut line_starts = vec![0u32];
        for (i, b) in text.bytes().enumerate() {
            if b == b'\n' {
                line_starts.push((i + 1) as u32);
            }
        }
        Source { name, text, line_starts }
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
