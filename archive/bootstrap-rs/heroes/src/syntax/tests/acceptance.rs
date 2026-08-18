//! The acceptance program, parsed.
//!
//! design.md's appendix ("A complete example program") is the M-generics-library acceptance
//! target: a lexer, parser and evaluator for arithmetic expressions, 317
//! lines. It has **one** source — the appendix itself (`examples/README.md`
//! records why the duplicate file was pruned) — so this test reads design.md
//! and extracts the fence rather than keeping a copy that could drift.
//!
//! What it pins is small and total: the whole program parses with **zero**
//! diagnostics. Every construct the language has appears in it, so this is
//! the one test that fails when a form the acceptance program needs stops
//! working — which is how the two gaps found at M-syntax-tree were found (a variant case
//! named `.var`, which the reserved-word registry rejects, and `=> assert …`,
//! a statement where §4.7 allows only an expression or a block).

use crate::source::Source;
use crate::syntax::parse;

/// The appendix's code fence, straight out of design.md.
fn acceptance_program() -> String {
    let design = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../design.md"
    ))
    .expect("design.md must exist");
    let appendix = design
        .find("## Appendix — A complete example program")
        .expect("the appendix must exist");
    let tail = &design[appendix..];
    let open = tail.find("```\n").expect("the appendix has a code fence") + 4;
    let close = tail[open..].find("\n```").expect("the fence closes") + open;
    tail[open..=close].to_string()
}

#[test]
fn the_acceptance_program_parses_with_no_diagnostics() {
    let text = acceptance_program();
    assert!(
        text.lines().count() > 250,
        "the appendix fence looks truncated: {} lines",
        text.lines().count()
    );
    let src = Source::new("design.md appendix".to_string(), text);
    let out = parse(&src);
    let rendered: Vec<String> =
        out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(rendered, Vec::<String>::new(), "the acceptance program must parse clean");
    // Sanity on the shape, so a parser that silently swallows declarations
    // cannot pass by reporting nothing.
    assert!(
        out.ast.decls.len() > 30,
        "expected the whole program, found {} declarations",
        out.ast.decls.len()
    );
}
