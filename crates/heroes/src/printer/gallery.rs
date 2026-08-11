//! `examples/gallery/` — the corpus, checked.
//!
//! The gallery exists to be *read*: nine programs that show what the language
//! looks like when it is used rather than tested (`examples/README.md` says what
//! each one is for). Because they are real files and nobody runs them by hand,
//! they would rot in a week — so every one of them is held to three properties
//! here, and the corpus doubles as the widest regression surface the frontend
//! has:
//!
//! 1. **it parses with zero diagnostics** — the shapes a reader is being shown
//!    are shapes the compiler accepts;
//! 2. **it is in canonical form**, byte for byte, so what the reader sees is
//!    what `heroes fmt` produces and no example teaches a layout the tool would
//!    undo;
//! 3. **formatting preserves its tree**, which is the property that makes (2)
//!    safe to assert at all.
//!
//! Resolution is checked next door in `resolve::tests::gallery`: it needs the
//! resolver, and this module is the printer's.

use crate::syntax::parse;

use super::tests::{assert_canonical, format};

/// Every `.hero` file in the gallery, in sorted order.
pub(crate) fn gallery_files() -> Vec<(String, String)> {
    let dir = concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/gallery");
    let mut files: Vec<(String, String)> = std::fs::read_dir(dir)
        .expect("examples/gallery must exist")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero"))
        .map(|path| {
            let name = path
                .file_name()
                .expect("a file name")
                .to_string_lossy()
                .into_owned();
            let text = std::fs::read_to_string(&path).expect("a readable .hero file");
            (name, text)
        })
        .collect();
    files.sort();
    assert!(files.len() >= 12, "the gallery lost files: {}", files.len());
    files
}

#[test]
fn every_gallery_program_parses_clean() {
    for (name, text) in gallery_files() {
        let src = crate::library::attach(name.clone(), text);
        let out = parse(&src);
        let rendered: Vec<String> =
            out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
        assert_eq!(rendered, Vec::<String>::new(), "{name} does not parse clean");
    }
}

#[test]
fn every_gallery_program_is_canonical() {
    for (name, text) in gallery_files() {
        assert_canonical(&text);
        assert_eq!(format(&text), text, "{name} is not in canonical form");
    }
}
