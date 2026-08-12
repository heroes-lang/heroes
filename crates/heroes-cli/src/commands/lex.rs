//! `heroes lex <file.hero> [--dump-tokens] [--json]` — the token stream (M1).
//!
//! Without a flag it is a lexical check: silent on success, diagnostics on
//! stderr. `--dump-tokens` prints the stream (design.md §3.5's own spelling), and
//! `--json` says how — one object per token instead of `line:col kind [text]`.
//!
//! The two flags are deliberately not one: `--dump-tokens` chooses *what* to
//! print, `--json` chooses *how*, and before panel 016 the surface had them in the
//! same bracket looking alike.

use heroes::lexer::{kind_name, lex, TokenKind};

use crate::cli::{Exit, Invocation};
use crate::input;

pub fn run(path: &str, args: &Invocation) -> Exit {
    let src = match input::read(path) {
        Ok(src) => src,
        Err((message, exit)) => {
            eprintln!("error: {message}");
            return exit;
        }
    };
    let out = lex(&src);
    if args.has("--dump-tokens") {
        if args.has("--json") {
            print!("{}", json(&out.tokens, &src));
        } else {
            print!("{}", text(&out.tokens, &src));
        }
    }
    for diagnostic in &out.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if out.diagnostics.is_empty() {
        Exit::Ok
    } else {
        Exit::Diagnostics
    }
}

/// **The file the reader named, at that file's own lines.**
///
/// A `--dump-<stage>` answers "what does the compiler know about the file I
/// named" (CLAUDE.md §10, and `printer/scopes.rs` says it in those words) —
/// `--dump-ast` and `--dump-scopes` both filter, and this one did not: an
/// eight-line program printed 470 lines, the library's included, at
/// concatenated line numbers with no file marker (2026-08-12).
fn json(tokens: &[heroes::lexer::Token], src: &heroes::source::Source) -> String {
    let mut out = String::from("[\n");
    let shown_tokens: Vec<&heroes::lexer::Token> =
        tokens.iter().filter(|t| src.is_root(t.span.start)).collect();
    let last = shown_tokens.len().saturating_sub(1);
    for (i, token) in shown_tokens.iter().enumerate() {
        let (_, line, col) = src.locate(token.span.start);
        let comma = if i == last { "" } else { "," };
        out.push_str(&format!(
            "  {{\"kind\": \"{}\", \"text\": \"{}\", \"line\": {line}, \"col\": {col}}}{comma}\n",
            kind_name(token.kind),
            escape(shown(token.kind, src.slice(token.span)))
        ));
    }
    out.push_str("]\n");
    out
}

fn text(tokens: &[heroes::lexer::Token], src: &heroes::source::Source) -> String {
    let mut out = String::new();
    for token in tokens.iter().filter(|t| src.is_root(t.span.start)) {
        let (_, line, col) = src.locate(token.span.start);
        let shown = shown(token.kind, src.slice(token.span));
        if shown.is_empty() {
            out.push_str(&format!("{line}:{col} {}\n", kind_name(token.kind)));
        } else {
            out.push_str(&format!("{line}:{col} {} {shown}\n", kind_name(token.kind)));
        }
    }
    out
}

/// Layout tokens carry no text, so printing their (empty) slice would be noise.
fn shown(kind: TokenKind, text: &str) -> &str {
    match kind {
        TokenKind::Indent | TokenKind::Dedent | TokenKind::Terminator | TokenKind::Eof => "",
        _ => text,
    }
}

fn escape(text: &str) -> String {
    text.replace('\\', "\\\\").replace('"', "\\\"").replace('\n', "\\n")
}
