//! Crate-internal tests for the checker, grouped by the question they answer.
//!
//! | file | what it pins |
//! |------|--------------|
//! | `scalars.rs`   | literals, operators, `if`, and where a value is not allowed |
//! | `calls.rs`     | calls, arity, built-ins, UFCS, `@` markers, generics |
//! | `data.rs`      | records, variants, patterns, exhaustiveness, `_`'s ban |
//! | `fallible.rs`  | `T?`, `ok`/`fail` in ⇐ mode, `?`, and the error's two fields |
//! | `flow.rs`      | panel 017 A: a jump has no type, and the one join rule |
//! | `holes.rs`     | §4.16: what the compiler knew and used to throw away |
//! | `acceptance.rs`| design.md's appendix, type-checked; and the gallery |
//!
//! `diagnostics` renders what the compiler *says*; `types_of` reads what it
//! *knows*. Both, wherever the message and the bookkeeping can disagree.

mod acceptance;
mod calls;
mod data;
mod fallible;
mod flow;
mod holes;
mod scalars;

use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;

use super::{check, Checked};

/// Every diagnostic the *checker* produces, one per line. Panics if an earlier
/// stage complained: the checker only ever runs on a resolved tree, so a test
/// whose input does not resolve is not testing what it thinks.
fn diagnostics(text: &str) -> String {
    let (out, src) = checked(text);
    out.diagnostics
        .iter()
        .map(|d| format!("{}\n", d.render_line(&src)))
        .collect()
}

fn checked(text: &str) -> (Checked, Source) {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let rendered: Vec<String> =
        parsed.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert!(rendered.is_empty(), "the input must parse clean, got: {rendered:?}");
    let resolved = resolve(&parsed.ast, &src);
    let rendered: Vec<String> =
        resolved.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert!(rendered.is_empty(), "the input must resolve clean, got: {rendered:?}");
    let out = check(&parsed.ast, &resolved, &src);
    (out, src)
}

/// A program that must check with nothing to say.
fn assert_clean(text: &str) {
    assert_eq!(diagnostics(text), "", "expected no diagnostics");
}

/// The type of the last expression in the file, as the author would write it.
fn type_of_last(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let resolved = resolve(&parsed.ast, &src);
    let out = check(&parsed.ast, &resolved, &src);
    let last = crate::syntax::ExprId(parsed.ast.exprs.len() as u32 - 1);
    super::render_ty(&out.types, &parsed.ast, &src, out.expr_types[last.0 as usize], &[])
}
