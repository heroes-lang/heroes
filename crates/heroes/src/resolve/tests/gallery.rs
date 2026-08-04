//! `examples/gallery/` — every name in the corpus resolves.
//!
//! The other half of the gallery's contract (`printer::gallery` holds parsing
//! and canonical form). Nine programs, ~450 lines of Heroes written to be read
//! rather than to pass: they are the widest test the resolver's rules get, and
//! the one that says whether those rules fight ordinary code.
//!
//! When this file was written all nine resolved on the first attempt, which is
//! the datum worth keeping: no shadow, no unused binding, no built-in name
//! collision in code nobody wrote *for* the resolver.

use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;

#[test]
fn every_gallery_program_resolves_clean() {
    let dir = concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/gallery");
    let mut names: Vec<String> = std::fs::read_dir(dir)
        .expect("examples/gallery must exist")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero"))
        .map(|path| path.to_string_lossy().into_owned())
        .collect();
    names.sort();
    assert!(names.len() >= 9, "the gallery lost files: {}", names.len());
    for path in names {
        let text = std::fs::read_to_string(&path).expect("a readable .hero file");
        let short = path.rsplit('/').next().unwrap_or(&path).to_string();
        let src = Source::new(short.clone(), text);
        let parsed = parse(&src);
        assert!(parsed.diagnostics.is_empty(), "{short} does not parse clean");
        let out = resolve(&parsed.ast, &src);
        let rendered: Vec<String> =
            out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
        assert_eq!(rendered, Vec::<String>::new(), "{short} does not resolve clean");
    }
}
