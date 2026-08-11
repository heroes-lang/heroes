//! The acceptance program, resolved.
//!
//! design.md's appendix is the M6 target and has one source — the appendix
//! itself — so this test reads design.md and extracts the fence, exactly as
//! `syntax::tests::acceptance` does for parsing. Two stages, one witness.
//!
//! What it pins is small and total: every name in a 317-line program resolves,
//! with **zero** diagnostics. It has already earned its place twice — the
//! program's two library functions were named `map` and `fold`, which panel
//! 015's one-tier rule makes unusable, and `.str()` turned out to be on
//! §4.20's built-in inventory and missing from the spec's list. Both were found
//! by running this.

use crate::resolve::resolve;
use crate::syntax::parse;

/// The appendix's code fence, straight out of design.md.
fn acceptance_program() -> String {
    let design =
        std::fs::read_to_string(concat!(env!("CARGO_MANIFEST_DIR"), "/../../design.md"))
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
fn the_acceptance_program_resolves_with_no_diagnostics() {
    let src = crate::library::attach("design.md appendix".to_string(), acceptance_program());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "the appendix must parse clean first");
    let out = resolve(&parsed.ast, &src);
    let rendered: Vec<String> = out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(rendered, Vec::<String>::new(), "every name must resolve");
    // Sanity on the shape: a resolver that silently walked nothing would also
    // report nothing.
    assert!(out.top.len() > 30, "expected the whole program, found {}", out.top.len());
    assert!(out.locals.len() > 50, "expected every binding, found {}", out.locals.len());
    let resolved_uses = out.uses.iter().filter(|r| **r != super::super::Ref::Unresolved).count();
    assert!(resolved_uses > 200, "expected every name, found {resolved_uses}");
}

/// The program contains one `???` (`simplify`), so the unused rule is suspended
/// for the whole file — which is what makes its one unread parameter legal.
#[test]
fn the_acceptance_program_is_held_open_by_its_one_hole() {
    let src = crate::library::attach("design.md appendix".to_string(), acceptance_program());
    let parsed = parse(&src);
    let out = resolve(&parsed.ast, &src);
    assert!(!out.holes_in.is_empty(), "the appendix has its one hole");
    let unread = out.locals.iter().filter(|l| l.reads == 0).count();
    assert_eq!(unread, 1, "only `simplify`'s parameter is unread");
}
