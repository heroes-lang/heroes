//! How several files become one text (design.md §1.11, §4.1; panel 031).
//!
//! Split out of `mod.rs` by the §11 sweep. Everything here runs **once, before any
//! offset exists**; everything left in `mod.rs` answers questions *about* offsets
//! that already do. That is the seam, and it is why `InputFile` lives here: it is
//! what a caller holds before there is a `Source` to hold anything.

use super::locate::starts_of;
use super::{module_of, stem_of, FileEntry, Source, LIBRARY_FILE, LIBRARY_MODULE};

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

}

