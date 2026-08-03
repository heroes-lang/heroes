//! `heroes lex <file.hero> [--json]` — dump the token stream (M1).
//!
//! Plain mode mirrors the snapshot format (`line:col kind [text]`); `--json`
//! emits one object per token. Both orders are the token order — already
//! deterministic, nothing to sort. Diagnostics render to stderr; any
//! diagnostic means a failing exit code.

use std::process::ExitCode;

use heroes::lexer::{kind_name, lex, TokenKind};
use heroes::source::Source;

const USAGE: &str = "usage: heroes lex <file.hero> [--json]";

pub fn run(args: &[String]) -> ExitCode {
    let mut file: Option<&String> = None;
    let mut json = false;
    for a in args {
        match a.as_str() {
            "--json" => json = true,
            _ if !a.starts_with('-') && file.is_none() => file = Some(a),
            _ => {
                eprintln!("error: unexpected argument `{a}`\n{USAGE}");
                return ExitCode::FAILURE;
            }
        }
    }
    let Some(path) = file else {
        eprintln!("{USAGE}");
        return ExitCode::FAILURE;
    };
    let text = match std::fs::read_to_string(path) {
        Ok(t) => t,
        Err(e) => {
            eprintln!("error: cannot read `{path}`: {e}");
            return ExitCode::FAILURE;
        }
    };
    let src = Source::new(path.clone(), text);
    let out = lex(&src);

    if json {
        println!("[");
        let last = out.tokens.len().saturating_sub(1);
        for (i, t) in out.tokens.iter().enumerate() {
            let (line, col) = src.line_col(t.span.start);
            let comma = if i == last { "" } else { "," };
            println!(
                "  {{\"kind\": \"{}\", \"text\": \"{}\", \"line\": {line}, \"col\": {col}}}{comma}",
                kind_name(t.kind),
                json_escape(src.slice(t.span)),
            );
        }
        println!("]");
    } else {
        for t in &out.tokens {
            let (line, col) = src.line_col(t.span.start);
            let layout = matches!(
                t.kind,
                TokenKind::Terminator | TokenKind::Indent | TokenKind::Dedent | TokenKind::Eof
            );
            if layout || t.span.start == t.span.end {
                println!("{line}:{col} {}", kind_name(t.kind));
            } else {
                println!("{line}:{col} {} {}", kind_name(t.kind), src.slice(t.span));
            }
        }
    }

    for d in &out.diagnostics {
        eprintln!("{}", d.render_line(&src));
    }
    if out.diagnostics.is_empty() {
        ExitCode::SUCCESS
    } else {
        ExitCode::FAILURE
    }
}

fn json_escape(s: &str) -> String {
    let mut out = String::with_capacity(s.len());
    for c in s.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out
}
