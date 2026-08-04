//! Crate-internal tests for the resolver, grouped by the question they answer.
//!
//! | file            | what it pins |
//! |-----------------|--------------|
//! | `names.rs`      | what a name resolves to, and what happens when nothing does |
//! | `scopes.rs`     | shadowing, sibling scopes, the order-free top level |
//! | `unused.rs`     | §4.4's unused rule and §4.16's file-wide exemption |
//! | `types.rs`      | written types against primitives, declarations, generics |
//! | `mutation.rs`   | what may be written, and what a write counts as |
//! | `acceptance.rs` | design.md's whole appendix program, resolved clean |
//! | `gallery.rs`    | `examples/gallery/`, nine programs written to be read |
//!
//! Two helpers, and the difference between them is the point: `diagnostics`
//! renders what the compiler *says* (the deliverable, §4.17), `scopes` renders
//! what it *knows* (`--dump-scopes`). A rule is pinned by both wherever the
//! message and the bookkeeping can disagree.

mod acceptance;
mod gallery;
mod mutation;
mod names;
mod scopes;
mod types;
mod unused;

use crate::printer::dump_scopes;
use crate::source::Source;
use crate::syntax::parse;

use super::{resolve, Resolved};

/// Every diagnostic the resolver produces, rendered one per line. Panics if the
/// *parser* complained: the resolver only ever runs on a clean tree, so a test
/// whose input does not parse is testing nothing it thinks it is.
fn diagnostics(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let rendered: Vec<String> =
        parsed.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert!(rendered.is_empty(), "the input must parse clean, got: {rendered:?}");
    let out = resolve(&parsed.ast, &src);
    out.diagnostics
        .iter()
        .map(|d| format!("{}\n", d.render_line(&src)))
        .collect()
}

fn scopes(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let out = resolve(&parsed.ast, &src);
    dump_scopes(&parsed.ast, &out, &src)
}

fn resolved(text: &str) -> (Resolved, Source) {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let out = resolve(&parsed.ast, &src);
    (out, src)
}

/// A file that must resolve with nothing to say.
fn assert_clean(text: &str) {
    assert_eq!(diagnostics(text), "", "expected no diagnostics");
}
