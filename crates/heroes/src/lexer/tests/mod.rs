//! Crate-internal snapshot tests for token dumps, grouped like the lexer
//! itself (the plan's M1 test strategy: goldens are for rendered
//! diagnostics and program output; token dumps are pinned here).
//!
//! | file             | what it pins |
//! |------------------|--------------|
//! | `end_to_end.rs`  | whole files: empty input, examples/first.hero |
//! | `layout.rs`      | indentation, terminators, panel-007 behaviour |
//! | `literals.rs`    | numbers, operators, strings, char literals |
//! | `reserved.rs`    | foreign words and their prescribed errors |
//! | `adversarial.rs` | M1's five adversarial cases (UNVERIFIED — pending debrief) |
//!
//! Dump format, one token per line: `line:col kind [text]` — layout tokens
//! (terminator/indent/dedent/eof) carry no text. Diagnostics follow, each
//! prefixed `DIAG `. Changing the format churns every snapshot: don't.

mod adversarial;
mod end_to_end;
mod layout;
mod literals;
mod reserved;

use super::{kind_name, lex, TokenKind};
use crate::source::Source;

fn dump(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = lex(&src);
    let mut s = String::new();
    for t in &out.tokens {
        let (line, col) = src.line_col(t.span.start);
        s.push_str(&format!("{line}:{col} {}", kind_name(t.kind)));
        let layout = matches!(
            t.kind,
            TokenKind::Terminator | TokenKind::Indent | TokenKind::Dedent | TokenKind::Eof
        );
        if !layout && t.span.start != t.span.end {
            s.push(' ');
            s.push_str(src.slice(t.span));
        }
        s.push('\n');
    }
    for d in &out.diagnostics {
        s.push_str(&format!("DIAG {}\n", d.render_line(&src)));
    }
    s
}
