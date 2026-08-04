//! The whole corpus, lowered: design.md's appendix and `examples/gallery/`.
//!
//! The appendix is this milestone's real acceptance criterion, and it has now paid
//! four times: M2's parser found `.var` and `=> assert false`, M3a's resolver found
//! the `map`/`fold` collision, M3b's checker found a twelfth `T`-where-`T?` site.
//! M4's turn produced no new defect in the program — every one of its 320 lines
//! lowers and verifies — which is itself the datum: the frontend's answers were
//! complete enough for a pass that reads them rather than the tree. It did correct
//! one *count*: panel 019's record says the appendix holds six `test` blocks, and
//! it holds seven.

use crate::ir::{dump, lower, verify};
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;
use crate::types::check;

fn acceptance_program() -> String {
    let design = std::fs::read_to_string(concat!(env!("CARGO_MANIFEST_DIR"), "/../../design.md"))
        .expect("design.md must exist");
    let appendix = design
        .find("## Appendix — A complete example program")
        .expect("the appendix must exist");
    let tail = &design[appendix..];
    let open = tail.find("```\n").expect("the appendix has a code fence") + 4;
    let close = tail[open..].find("\n```").expect("the fence closes") + open;
    tail[open..=close].to_string()
}

/// Lowers, verifies, and hands back the dump. The verifier is not optional here:
/// a test that only checked for the absence of diagnostics would pass on an IR
/// whose blocks had two exits.
fn lower_text(name: &str, text: &str) -> String {
    let src = Source::new(name.to_string(), text.to_string());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "{name} must parse clean");
    let resolved = resolve(&parsed.ast, &src);
    assert!(resolved.diagnostics.is_empty(), "{name} must resolve clean");
    let checked = check(&parsed.ast, &resolved, &src);
    assert!(checked.diagnostics.is_empty(), "{name} must type-check clean");
    let out = lower(&parsed.ast, &resolved, &checked, &src);
    let said: Vec<String> = out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(said, Vec::<String>::new(), "{name} must lower clean");
    let problems = verify(&out.program, &checked);
    assert_eq!(problems, Vec::<String>::new(), "{name}'s IR must be well formed");
    dump(&out.program, &parsed.ast, &checked, &src)
}

#[test]
fn the_acceptance_program_lowers_and_verifies() {
    let dumped = lower_text("design.md appendix", &acceptance_program());
    // The `record`s and `variant`s produce no function; the `test` blocks do
    // (panel 019 point 6).
    let functions = dumped.matches("\nfunction ").count() + usize::from(dumped.contains("function "));
    assert!(functions >= 25, "the appendix lost functions: {functions}");
    assert_eq!(dumped.matches("\ntest \"").count(), 7, "seven test blocks");
    // Nothing the lowering could not handle reached the dump. `verify` rejects it
    // too; this is the version a reader can check by eye.
    assert!(!dumped.contains("MISSING"), "a form was not lowered");
    assert!(!dumped.contains("OPEN"), "a block was left unterminated");
}

/// The compiler-engineer's prediction from panel 019, scored: **under 12 IR lines
/// per source line**, or "the load/store verbosity has defeated the deliberate
/// artifact claim and the dump needs an elided rendering".
///
/// The number is asserted rather than printed so that a future change which
/// doubles the dump's size fails here instead of being noticed a milestone later.
#[test]
fn the_dump_stays_under_twelve_lines_per_source_line() {
    let program = acceptance_program();
    let source_lines = program.lines().count();
    let dumped = lower_text("design.md appendix", &program);
    let ir_lines = dumped.lines().count();
    let ratio = ir_lines as f64 / source_lines as f64;
    assert!(
        ratio < 12.0,
        "{ir_lines} IR lines for {source_lines} source lines = {ratio:.2} per line"
    );
}

/// A `$` cannot appear in a Heroes program, so a `$` in the dump is always the
/// compiler's own vocabulary — and a source name can never be mistaken for a
/// temporary. This is the llm-ergonomist's first prediction, inverted: it said the
/// dump *would* be ambiguous unless the sigil shipped.
#[test]
fn a_source_name_can_never_look_like_a_temporary() {
    // `t0` and `t1` are legal Heroes bindings, which is what made the sigil a
    // condition rather than a preference.
    let dumped = lower_text(
        "probe",
        "function scale(n: int) -> int\n    t0: int @ 0\n    t1: int @ 1\n    t0 @ t0 + t1 * n\n    return t0\n",
    );
    for line in dumped.lines() {
        for word in line.split(|c: char| !(c.is_alphanumeric() || c == '$' || c == '_')) {
            if word == "t0" || word == "t1" {
                // A bare `t0` is always the slot; the temporary is `$t0`.
                assert!(
                    line.contains("store t0")
                        || line.contains("store t1")
                        || line.contains("load t0")
                        || line.contains("load t1")
                        || line.contains("slots"),
                    "an unsigiled name in an operand position: {line}"
                );
            }
        }
    }
    assert!(dumped.contains("$t1: int = const 0"), "{dumped}");
}

#[test]
fn every_gallery_program_lowers_and_verifies() {
    let dir = concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/gallery");
    let mut paths: Vec<String> = std::fs::read_dir(dir)
        .expect("examples/gallery must exist")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero"))
        .map(|path| path.to_string_lossy().into_owned())
        .collect();
    paths.sort();
    assert!(paths.len() >= 12, "the gallery lost files: {}", paths.len());
    for path in paths {
        let text = std::fs::read_to_string(&path).expect("a readable .hero file");
        let short = path.rsplit('/').next().unwrap_or(&path).to_string();
        let dumped = lower_text(&short, &text);
        assert!(!dumped.contains("MISSING"), "{short} has a form the lowering skipped");
    }
}
