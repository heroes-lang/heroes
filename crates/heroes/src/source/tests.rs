//! What the file table has to get right.
//!
//! Every property here was a one-file assumption until M8a, and each one is a
//! way for a diagnostic to point at the wrong line in the wrong file — the class
//! of defect that is invisible in a green test suite, because the message is
//! well-formed and merely false.

use super::*;

fn three() -> Source {
    Source::of(vec![
        InputFile::user("main.hero".to_string(), "a\nb\n".to_string()),
        InputFile::user("geom.hero".to_string(), "c\n".to_string()),
        InputFile {
            name: "<heroes library>".to_string(),
            module: LIBRARY_MODULE.to_string(),
            text: "d\ne\n".to_string(),
            is_library: true,
        },
    ])
}

/// The joined text, spelled out. One blank line between files, and a file
/// missing its final newline gets one — **except the last**, which is left byte
/// for byte as written, because a newline nobody typed moves the EOF token.
#[test]
fn files_are_joined_by_exactly_one_blank_line() {
    assert_eq!(three().text, "a\nb\n\nc\n\nd\ne\n");
    let unterminated = Source::of(vec![
        InputFile::user("a.hero".to_string(), "x".to_string()),
        InputFile::user("b.hero".to_string(), "y".to_string()),
    ]);
    assert_eq!(unterminated.text, "x\n\ny", "the LAST file keeps exactly what was written");
}

/// **The root file's offsets and line numbers are exactly what they would be if
/// it were alone.** This is the whole reason files are appended rather than
/// sorted or prepended, and it is what every diagnostic in the compiler assumes.
#[test]
fn the_root_files_lines_are_untouched_by_what_follows_it() {
    let alone = Source::new("main.hero".to_string(), "a\nb\n".to_string());
    let joined = three();
    for offset in 0..4u32 {
        assert_eq!(alone.line_col(offset), joined.line_col(offset), "offset {offset}");
        assert_eq!(joined.file_of(offset), 0, "offset {offset} is the root's");
    }
}

/// Each file's own numbering, which is what a `#line` for it must claim.
#[test]
fn a_line_is_reported_within_its_own_file() {
    let src = three();
    let c = src.text.find('c').unwrap() as u32;
    let e = src.text.find('e').unwrap() as u32;
    assert_eq!(src.line_col(c).0, 4, "line 4 of the whole text");
    assert_eq!(src.file_line_of(c), 1, "line 1 of geom.hero");
    assert_eq!(src.file_line_of(e), 2, "line 2 of the library");
}

#[test]
fn every_offset_knows_its_file_and_module() {
    let src = three();
    let b = src.text.find('b').unwrap() as u32;
    let c = src.text.find('c').unwrap() as u32;
    let d = src.text.find('d').unwrap() as u32;
    assert_eq!(src.module_at(b), "main");
    assert_eq!(src.module_at(c), "geom");
    assert_eq!(src.module_at(d), LIBRARY_MODULE);
    assert!(!src.is_library(b));
    assert!(!src.is_library(c));
    assert!(src.is_library(d));
}

/// A file's first byte belongs to that file and not to the one before it. The
/// binary search is `Err(i) => i - 1`, so the exact-hit case is the one that
/// would be off by one if `Ok` were folded into it.
#[test]
fn a_boundary_offset_belongs_to_the_file_that_starts_there() {
    let src = three();
    let starts: Vec<u32> = src.files().iter().map(|f| f.start).collect();
    for (index, start) in starts.iter().enumerate() {
        assert_eq!(src.file_of(*start), index, "the first byte of file {index}");
        if index > 0 {
            assert_eq!(src.file_of(start - 1), index - 1, "the blank line before file {index}");
        }
    }
}

/// **The end-of-file offset is a real offset.** `text.len()` is where an EOF
/// span starts, and the sentinel that used to answer this question said "that is
/// in the library" and moved every end-of-file diagnostic's line by one file.
#[test]
fn the_offset_one_past_the_end_still_names_the_last_file() {
    let src = three();
    let end = src.text.len() as u32;
    assert_eq!(src.file_of(end), src.files().len() - 1);
    assert!(src.is_library(end));

    let alone = Source::new("main.hero".to_string(), "a\nb\n".to_string());
    assert!(!alone.is_library(alone.text.len() as u32), "one file, and it is not the library");
}

/// What `fmt` and `check --apply` write back. Anything more than the root file
/// is somebody else's text landing in the author's file.
#[test]
fn user_text_is_the_root_file_and_nothing_else() {
    assert_eq!(three().user_text(), "a\nb\n");
    assert_eq!(three().text_of(1), "c\n");
    assert_eq!(three().text_of(2), "d\ne\n");

    let alone = Source::new("main.hero".to_string(), "a\nb\n".to_string());
    assert_eq!(alone.user_text(), alone.text);
}

/// The library arrangement M5b built, still exactly itself — now expressed as
/// two rows of the general table rather than as a boundary offset.
#[test]
fn with_library_is_two_files() {
    let src = Source::with_library(
        "main.hero".to_string(),
        "function main()\n".to_string(),
        "function range()\n".to_string(),
    );
    assert_eq!(src.files().len(), 2);
    assert_eq!(src.files()[0].module, "main");
    assert_eq!(src.files()[1].module, LIBRARY_MODULE);
    assert!(src.files()[1].is_library);
    assert_eq!(src.user_text(), "function main()\n");
}
