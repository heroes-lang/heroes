//! Source text and byte-offset spans.
//!
//! One `Source` owns **every file of one compilation**, concatenated into one
//! text; everything downstream points into it with byte offsets (indices, never
//! references — the Cyclone rule), so tokens and diagnostics stay `Copy` and the
//! text stays in one place.
//!
//! **Why one text rather than a file id in every `Span`** (panel 031 R7,
//! measured rather than preferred): the compiler has 39 `Span` construction
//! sites across 14 files and 271 functions taking `&Source`. Concatenation
//! changes **none** of them. A file id inside `Span` changes all 39, pushes
//! `Span` past 8 bytes, and forces `Span::to` to assert that both ends come from
//! the same file — a check with nothing useful to do when it fails.
//!
//! The shape was already here. M-strings-ownership appended the library to the user's file and
//! carried one boundary offset; M-module-namespace keeps exactly that arrangement and lets the
//! table have more than two rows. The library becomes the last row and stops
//! being a special case in the data — though not in the rules, since a
//! diagnostic pointing into it is still a compiler bug (`files.rs`).
//!
//! **The root file is `files[0]`, and every offset the author can see is what it
//! would have been if their file were alone.** That is why files are appended in
//! load order rather than sorted, and why nothing is ever prepended.

mod files;

pub use files::{module_of, stem_of, FileEntry, LIBRARY_FILE, LIBRARY_MODULE};

/// Every file of one compilation, in one text.
pub struct Source {
    /// The root file's name — what `--dump-<stage>` heads its output with and
    /// what the reader thinks they are compiling.
    pub name: String,
    /// Every file, joined by one blank line each.
    pub text: String,
    /// Byte offset of the first byte of each line; line_starts[0] == 0.
    line_starts: Vec<u32>,
    /// In load order, never empty, `files[0]` the root. Sorted by `start` by
    /// construction, which is what lets `file_of` binary-search it.
    files: Vec<FileEntry>,
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
    /// goes through here, so there is one binary search and one answer.
    pub fn file_of(&self, offset: u32) -> usize {
        match self.files.binary_search_by_key(&offset, |f| f.start) {
            Ok(i) => i,
            Err(i) => i.saturating_sub(1),
        }
    }

    pub fn file(&self, offset: u32) -> &FileEntry {
        &self.files[self.file_of(offset)]
    }

    /// Every file in the compilation, in load order. The root is first.
    pub fn files(&self) -> &[FileEntry] {
        &self.files
    }

    /// Is this offset in the library rather than in something the author wrote?
    ///
    /// Every caller is answering one of three questions: may a diagnostic point
    /// here (no — it is a compiler bug), may a `#line` claim the author's file
    /// for it (no — clang would blame a line they cannot see), and which module
    /// does the mangler put it in (`library`).
    pub fn is_library(&self, offset: u32) -> bool {
        self.file(offset).is_library
    }

    /// Is this offset in the file the reader actually named?
    ///
    /// The distinction `is_library` used to be enough for, and is not any more.
    /// A `--dump-<stage>` answers "what does the compiler know about **the file
    /// I named**" (CLAUDE.md §10), and so does `fmt`, which writes one file
    /// back — those ask this. Emission and `heroes test` are about the whole
    /// program and ask `is_library` instead: a module's `test` blocks are its
    /// own, and a compilation that ran only the root's tests would drop the rest
    /// in silence, which is the class of loss this language exists to refuse.
    pub fn is_root(&self, offset: u32) -> bool {
        self.file_of(offset) == 0
    }

    /// How to name **the other end of a mistake**, from where the caret is.
    ///
    /// `line 12` when both ends are in one file, `geom.hero:12` when they are
    /// not. Every "already declared at …", "the cycle is …" and "declared at …:
    /// <signature>" goes through here.
    ///
    /// It exists because `locate` could not protect these: a location formatted
    /// **into a message** is a `String` by the time any renderer sees it, so it
    /// is outside every guard this compiler has. Ten sites built one by hand from
    /// `line_col` and named no file at all — correct while `lines_before` was
    /// zero for everything, and after M-module-namespace pointing at a line that is not there.
    /// One of them was internally consistent and wrong in both halves: a
    /// `shadowed_binding` whose note said "line 8" while its own caret sat on
    /// `geom.hero:8` (2026-08-12).
    ///
    /// The short form is kept for the same-file case deliberately. §4.17 asks the
    /// note to carry *the file the model would otherwise open*, and when that is
    /// the file already on screen, naming it is noise.
    pub fn elsewhere(&self, here: u32, there: u32) -> String {
        let (file, line, _) = self.locate(there);
        if self.file_of(here) == self.file_of(there) {
            return format!("line {line}");
        }
        format!("{file}:{line}")
    }

    /// The last offset **inside the root file** — where a diagnostic about the
    /// file as a whole puts its caret.
    ///
    /// `text.len()` is not that offset and has not been since the library was
    /// appended: it lands in whichever file happens to be last, which is how
    /// `no_entry_point` came out as `internal error: a diagnostic landed inside
    /// the Heroes library` at exit 2, for the commonest mistake in the language
    /// (2026-08-12). A whole-file diagnostic needs the root's extent, and this
    /// is the one place that computes it.
    pub fn root_end(&self) -> u32 {
        match self.files.get(1) {
            // One before the next file's start: the blank line that joins them.
            Some(next) => next.start.saturating_sub(1),
            None => self.text.len() as u32,
        }
    }

    /// The C identifier component for an offset's module — what
    /// `h_<component>_<name>` uses. Never `module_at`: see [`FileEntry`].
    pub fn component_at(&self, offset: u32) -> &str {
        &self.file(offset).component
    }

    /// The module an offset belongs to — what a qualified name writes, and what
    /// a qualified name resolves against.
    pub fn module_at(&self, offset: u32) -> &str {
        &self.file(offset).module
    }

    /// Exactly what the author wrote in the file they named, with everything
    /// appended to it removed.
    ///
    /// Anything that hands a program *back* — `check --apply`, `fmt` — must go
    /// through this, or it writes the whole compilation into one of its files.
    pub fn user_text(&self) -> &str {
        self.text_of(0)
    }

    /// One file's own text, without the blank line that joins it to the next.
    pub fn text_of(&self, index: usize) -> &str {
        let start = self.files[index].start as usize;
        let end = match self.files.get(index + 1) {
            // One before the next file's start: the blank line between them.
            Some(next) => (next.start as usize).saturating_sub(1),
            None => self.text.len(),
        };
        &self.text[start..end]
    }
}


mod build;
mod locate;

pub use build::InputFile;

#[cfg(test)]
mod tests;
