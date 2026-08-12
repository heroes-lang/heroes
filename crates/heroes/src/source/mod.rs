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

/// One file on its way into a `Source`: what the caller has before the offsets
/// exist. `module` is separate from `name` because a `use`d module's name is
/// declared, not derived — [`module_of`] is only the default for a root file.
pub struct InputFile {
    pub name: String,
    pub module: String,
    pub text: String,
    pub is_library: bool,
}

impl InputFile {
    /// An ordinary Heroes file, its module taken from its path — the **raw**
    /// stem, which is what `FileEntry.module` promises and what a `use` line
    /// spells. The C component is derived from it later, by `module_of`, which
    /// is the other namespace.
    pub fn user(name: String, text: String) -> InputFile {
        let module = stem_of(&name);
        InputFile { name, module, text, is_library: false }
    }
}

impl Source {
    /// One file on its own, with nothing else attached.
    pub fn new(name: String, text: String) -> Source {
        Source::of(vec![InputFile::user(name, text)])
    }

    /// The general constructor: N files, concatenated in the order given.
    ///
    /// **One blank line joins each pair**, and it is load-bearing twice: two
    /// declarations butted together would be one line to the parser, and the
    /// boundary offset has to fall on a line start for the file table to agree
    /// with the line numbering.
    ///
    /// Each file's text is normalised to end in a newline first, for the same
    /// reason — a file whose last line has no terminator would otherwise fuse
    /// with the next file's first line.
    pub fn of(inputs: Vec<InputFile>) -> Source {
        assert!(!inputs.is_empty(), "a Source has at least a root file");
        let name = inputs[0].name.clone();
        let mut text = String::new();
        let mut table: Vec<FileEntry> = Vec::new();
        for input in inputs {
            if !table.is_empty() {
                // Close the previous file's last line if it had no newline, then
                // the blank line that joins. Done *here* rather than after each
                // file so that the LAST file is left exactly as written: a
                // trailing newline the author did not type would move the EOF
                // token's position, which two lexer tests pin and a diagnostic
                // at end of file would then report one line too far.
                if !text.ends_with('\n') {
                    text.push('\n');
                }
                text.push('\n');
            }
            let start = text.len() as u32;
            let lines_before = starts_of(&text).len() as u32 - 1;
            table.push(FileEntry {
                name: input.name,
                component: module_of(&input.module),
                module: input.module,
                start,
                lines_before,
                is_library: input.is_library,
            });
            text.push_str(&input.text);
        }
        let line_starts = starts_of(&text);
        Source { name, text, line_starts, files: table }
    }

    /// The user's program with the library appended (§1.11, `crate::library`).
    pub fn with_library(name: String, user: String, library: String) -> Source {
        Source::of(vec![
            InputFile::user(name, user),
            InputFile {
                name: LIBRARY_FILE.to_string(),
                module: LIBRARY_MODULE.to_string(),
                text: library,
                is_library: true,
            },
        ])
    }

    /// Index into the file table. Every question about "which file is this"
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
fn starts_of(text: &str) -> Vec<u32> {
    let mut starts = vec![0u32];
    for (i, b) in text.bytes().enumerate() {
        if b == b'\n' {
            starts.push((i + 1) as u32);
        }
    }
    starts
}

#[cfg(test)]
mod tests;
