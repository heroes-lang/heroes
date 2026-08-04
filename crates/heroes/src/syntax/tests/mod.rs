//! Crate-internal snapshot tests for the parser, grouped like the parser
//! itself. Goldens (`tests/golden/`) pin *rendered diagnostics* through the
//! real binary; tree shape is pinned here.
//!
//! | file           | what it pins |
//! |----------------|--------------|
//! | `entities.rs`  | the four entities, `extern`, `test`, generics, `@` params |
//! | `bodies.rs`    | statements, the precedence table, control flow |
//! | `types.rs`     | the type grammar, including the panel-013 function type |
//! | `docs.rs`      | §4.1's adjacency rule: which comment is documentation |
//! | `recovery.rs`  | broken files: one diagnostic per mistake, parsing continues |
//! | `acceptance.rs`| design.md's whole appendix program, parsed clean |
//!
//! Dump format is `printer::dump_ast` (2-space indent, node kinds named),
//! with diagnostics appended as `DIAG <rendered line>`. Both are output
//! surface: changing a word churns every snapshot here.
//!
//! Every expectation starts at column 0 and therefore opens with the
//! `file test.hero` header line. That is not decoration: a `"\` string
//! continuation swallows the next line's leading whitespace, so an
//! expectation whose first line is indented silently loses it — which is
//! exactly how the first version of these tests failed.

mod acceptance;
mod bodies;
mod docs;
mod entities;
mod recovery;
mod types;

use crate::printer::dump_ast;
use crate::source::Source;

use super::parse;

fn dump(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = parse(&src);
    let mut s = dump_ast(&out.ast, &src);
    for diagnostic in &out.diagnostics {
        s.push_str(&format!("DIAG {}\n", diagnostic.render_line(&src)));
    }
    s
}
