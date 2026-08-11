//! The whole corpus, type-checked: design.md's appendix and `examples/gallery/`.
//!
//! This is the milestone's real acceptance criterion, and it has already paid
//! for itself: checking the appendix found a **twelfth** `T`-where-`T?` site that
//! panels 002 and 006 had missed (`tokenize`'s own `return out`), after M3a's
//! resolver had found the `map`/`fold` collision and M2's parser had found
//! `.var` and `=> assert false`. Four milestones, four defects, one program that
//! has never been run.

use crate::resolve::resolve;
use crate::syntax::parse;
use crate::types::check;

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

fn check_text(name: &str, text: &str) -> Vec<String> {
    let src = crate::library::attach(name.to_string(), text.to_string());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "{name} must parse clean");
    let resolved = resolve(&parsed.ast, &src);
    assert!(resolved.diagnostics.is_empty(), "{name} must resolve clean");
    let out = check(&parsed.ast, &resolved, &src);
    out.diagnostics.iter().map(|d| d.render_line(&src)).collect()
}

#[test]
fn the_acceptance_program_type_checks() {
    let rendered = check_text("design.md appendix", &acceptance_program());
    assert_eq!(rendered, Vec::<String>::new(), "every expression must type-check");
}

/// Every expression that *produces a value* gets a type — which is the invariant
/// M4 depends on, since lowering reads `expr_types` rather than the tree. The
/// exceptions are stated rather than counted: a `???` has no type by definition
/// (§4.16), and the name of a built-in or a record used as a callee is not a
/// value at all (there are no built-in function values, and a record name is a
/// constructor).
#[test]
fn every_value_producing_expression_is_typed() {
    let text = acceptance_program();
    let src = crate::library::attach("design.md appendix".to_string(), text);
    let parsed = parse(&src);
    let resolved = resolve(&parsed.ast, &src);
    let out = check(&parsed.ast, &resolved, &src);
    let mut untyped: Vec<String> = Vec::new();
    for (index, ty) in out.expr_types.iter().enumerate() {
        if *ty != out.types.error() {
            continue;
        }
        let expr = &parsed.ast.exprs[index];
        let produces_value = matches!(
            expr.kind,
            crate::syntax::ExprKind::Int
                | crate::syntax::ExprKind::Float
                | crate::syntax::ExprKind::Str
                | crate::syntax::ExprKind::Char
                | crate::syntax::ExprKind::Bool
                | crate::syntax::ExprKind::Unary { .. }
                | crate::syntax::ExprKind::Binary { .. }
                | crate::syntax::ExprKind::Field { .. }
                | crate::syntax::ExprKind::Index { .. }
                | crate::syntax::ExprKind::Method { .. }
                | crate::syntax::ExprKind::Array(_)
                | crate::syntax::ExprKind::Map(_)
                | crate::syntax::ExprKind::Try(_)
        );
        if produces_value {
            untyped.push(format!("{}: {:?}", src.line_col(expr.span.start).0, src.slice(expr.span)));
        }
    }
    assert_eq!(untyped, Vec::<String>::new(), "these value expressions were skipped");
    assert_eq!(out.holes.len(), 1, "the appendix has exactly one hole");
    assert!(out.types.len() > 20, "the type table looks empty: {}", out.types.len());
}

#[test]
fn every_gallery_program_type_checks() {
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
        let rendered = check_text(&short, &text);
        assert_eq!(rendered, Vec::<String>::new(), "{short} must type-check");
    }
}
