//! Lowering's tests: the shape of the IR, and the invariants that must hold.
//!
//! Every case runs the real pipeline — lex, parse, resolve, check, lower — and
//! then **verifies**, because a test that only reads the dump would pass on an IR
//! that is well-formatted and wrong.
//!
//! | file | what it holds to account |
//! |------|--------------------------|
//! | `scalars.rs` | flattening, slots, literals, the fall-through edge |
//! | `flow.rs`    | `while`, `for`, `if`, `&&`, `match` — the block shapes |
//! | `data.rs`    | records, variants, `T?`, places, `@` copy-out, `assert` |
//! | `acceptance.rs` | design.md's appendix and the whole gallery, lowered |

use crate::ir::{dump, lower, verify, Program};
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;
use crate::types::{check, Checked};

mod acceptance;
mod data;
mod flow;
mod scalars;

/// Lowers a program, asserting that the frontend had nothing to say about it — a
/// lowering test on a file with diagnostics would be testing recovery guesses.
fn lowered(text: &str) -> (Program, Checked, String) {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "parse: {:?}", parsed.diagnostics[0].message);
    let resolved = resolve(&parsed.ast, &src);
    assert!(resolved.diagnostics.is_empty(), "resolve: {:?}", resolved.diagnostics[0].message);
    let checked = check(&parsed.ast, &resolved, &src);
    assert!(checked.diagnostics.is_empty(), "check: {:?}", checked.diagnostics[0].message);
    let out = lower(&parsed.ast, &resolved, &checked, &src);
    assert!(out.diagnostics.is_empty(), "lower: {:?}", out.diagnostics[0].message);
    let problems = verify(&out.program, &checked);
    assert!(problems.is_empty(), "verify: {}", problems.join("; "));
    let text = dump(&out.program, &parsed.ast, &checked, &src);
    (out.program, checked, text)
}

/// The dump of one program, for the tests that read it.
fn text(program: &str) -> String {
    let (_, _, dumped) = lowered(program);
    dumped
}
