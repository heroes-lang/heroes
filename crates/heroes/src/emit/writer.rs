//! The output, which has to count its own lines (design.md §3.1, panel 020).
//!
//! **`#line N` anchors the *next* line, and C then auto-increments.** That one
//! sentence is why this is a type and not a `String`. "Emit `#line` on source-line
//! change" is not implementable by comparing an instruction's span with the
//! previous instruction's: a Heroes line that lowers to K lines of C drifts by
//! K−1, silently, and the further into a function you read the further off the
//! mapping is. The comparison has to be against **what clang currently believes** —
//! the effective line — which this writer tracks.
//!
//! The frozen target had the bug. `tools/spike/01-first.c` advertises
//! `00-first.hero:2` in its own header comment; clang, given a deliberate type
//! error, reports `00-first.hero:6` — a blank line — and prints the *C* text under
//! the `.hero` file name. Measured by the panel's ffi-pragmatist against the file
//! CLAUDE.md calls the shape the emitter must produce.
//!
//! The **restore** to the generated file has the same requirement from the other
//! side: `#line <n> "<stem>.c"` must name the line the *next output line* will
//! actually be, so lldb and clang land on the housekeeping code itself rather than
//! blaming a user line for it. bison has the concept (`b4_sync_start` /
//! `b4_sync_end`) and points it at its skeletons; pointing it at the emitter's own
//! output appears to be original, and is flagged as such in the DESIGN-LOG rather
//! than claimed as precedent.
//!
//! The generated name is derived from the **source** stem, never from `-o`: the
//! emitted C must not mention the output path, or the double-emit determinism diff
//! would be a property of the invocation instead of the emitter.

/// What clang believes the next output line is: which file, and which line in it.
#[derive(Clone, PartialEq, Eq, Debug)]
enum Claim {
    /// Nothing has been claimed yet — the file is still its own truth.
    None,
    At { file: String, line: u32 },
}

pub(super) struct Writer {
    out: String,
    /// Lines pushed so far. The next line's number in the generated file is
    /// `pushed + 1`.
    pushed: u32,
    claim: Claim,
    source: String,
    generated: String,
}

/// The name a `#line` gives the library's own source (§1.11's Tier 2).
///
/// It is not a path, and that is deliberate: there is no file to open. The
/// library is embedded in the compiler (`crate::library`), so a clang error
/// inside it must name something a reader can recognise as "not your program"
/// rather than a path that does not exist on their disk.
pub(super) const LIBRARY_FILE: &str = "<heroes library>";

impl Writer {
    pub(super) fn new(source_path: &str, module: &str) -> Writer {
        Writer {
            out: String::new(),
            pushed: 0,
            claim: Claim::None,
            source: source_path.to_string(),
            generated: format!("{module}.c"),
        }
    }

    /// One line of C. Everything goes through here, which is what keeps the count
    /// honest.
    pub(super) fn line(&mut self, text: &str) {
        self.out.push_str(text);
        self.out.push('\n');
        self.pushed += 1;
        if let Claim::At { line, .. } = &mut self.claim {
            *line += 1;
        }
    }

    pub(super) fn blank(&mut self) {
        self.line("");
    }

    /// Point the next line at the author's source, if it is not already pointing
    /// there.
    pub(super) fn at_source(&mut self, line: u32) {
        // `#line 0` is invalid C11, and a zero here would mean a span the source
        // map could not place. Clamp rather than emit invalid C.
        let line = line.max(1);
        if self.claim == (Claim::At { file: self.source.clone(), line }) {
            return;
        }
        let source = self.source.clone();
        self.directive(&source, line);
    }

    /// Point the next line at the library's own source.
    ///
    /// A library function must never claim the author's file: its line numbers
    /// run past the end of what they wrote, so a clang error would be reported
    /// against a line that does not exist — the compiler blaming the author for
    /// its own code, which is the failure CLAUDE.md §8 exists to prevent.
    pub(super) fn at_library(&mut self, line: u32) {
        let line = line.max(1);
        if self.claim == (Claim::At { file: LIBRARY_FILE.to_string(), line }) {
            return;
        }
        self.directive(LIBRARY_FILE, line);
    }

    /// Point the next line at the generated file — around a prologue, a copy-out,
    /// the shim: code no user line is responsible for.
    pub(super) fn at_generated(&mut self) {
        if matches!(&self.claim, Claim::At { file, .. } if *file == self.generated) {
            return;
        }
        let next = self.pushed + 2; // this directive occupies one line first
        let generated = self.generated.clone();
        self.directive(&generated, next);
    }

    fn directive(&mut self, file: &str, line: u32) {
        // Cleared first, so `line` does not increment a claim this directive is
        // about to replace.
        self.claim = Claim::None;
        let text = format!("#line {line} \"{}\"", escape(file));
        self.line(&text);
        self.claim = Claim::At { file: file.to_string(), line };
    }

    pub(super) fn finish(self) -> String {
        self.out
    }
}

/// A path inside a C string literal. A `"` or a `\` in a file name would otherwise
/// end the directive early — and on Windows-style paths the backslash is the
/// separator, so this is not a hypothetical.
fn escape(path: &str) -> String {
    let mut out = String::with_capacity(path.len());
    for c in path.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            _ => out.push(c),
        }
    }
    out
}

/// Point the next line at whichever source a span actually came from.
///
/// Every caller that has a span and a `Source` goes through here rather than
/// choosing for itself, so "is this the author's code or the library's" is
/// answered in one place (CLAUDE.md §11's rule about where a rule lives).
pub(super) fn at_span(w: &mut Writer, src: &crate::source::Source, offset: u32) {
    if src.is_library(offset) {
        w.at_library(src.library_line_of(offset));
    } else {
        let (line, _) = src.line_col(offset);
        w.at_source(line);
    }
}
