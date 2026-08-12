//! Expressions in canonical form: the *minimum* parentheses that preserve
//! the tree (design.md §4.14, §4.15).
//!
//! This is the half of `heroes fmt` that has a right answer. `2 + (3 * 4)`
//! loses its parentheses because the precedence table already says that;
//! `(2 + 3) * 4` keeps them because without them it is a different program.
//! One consequence worth stating: **a formatted file is not a re-typed file,
//! it is the same tree printed once** — which is what makes any textual
//! difference between two versions semantic (§4.15).
//!
//! `if` and `match` are expressions but their bodies are blocks, so they are
//! printed by `fmt.rs`, which owns indentation. Here they render as their
//! header only.

use crate::source::Source;
use crate::syntax::{Arg, Ast, BinaryOp, ExprId, ExprKind, UnaryOp};

/// Binding power, on the same scale as the parser's table: higher binds
/// tighter. Atoms and postfix chains are above every operator.
fn power(ast: &Ast, id: ExprId) -> u8 {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Binary { op, .. } => match op {
            BinaryOp::Or => 1,
            BinaryOp::And => 2,
            BinaryOp::Eq
            | BinaryOp::Ne
            | BinaryOp::Lt
            | BinaryOp::Le
            | BinaryOp::Gt
            | BinaryOp::Ge => 3,
            // The same numbers as `syntax/expr.rs`'s table, and they have to be:
            // this decides where `heroes fmt` prints a parenthesis, so a scale that
            // disagreed with the parser's would print a program that parses
            // differently from the one it read.
            BinaryOp::BitOr => 4,
            BinaryOp::BitXor => 5,
            BinaryOp::BitAnd => 6,
            BinaryOp::Shl | BinaryOp::Shr => 7,
            BinaryOp::Add | BinaryOp::Sub => 8,
            BinaryOp::Mul | BinaryOp::Div | BinaryOp::Rem => 9,
        },
        ExprKind::Unary { .. } => 10,
        // A control form spans lines; treating it as the loosest thing there
        // is keeps a parenthesis from ever being printed around it.
        ExprKind::If { .. } | ExprKind::Match { .. } => 0,
        _ => 11,
    }
}

pub(super) fn render(ast: &Ast, src: &Source, id: ExprId) -> String {
    let node = &ast.exprs[id.0 as usize];
    match &node.kind {
        // The one literal `fmt` rewrites, and only in its digits' case: a mask
        // copied from a C header arrives as `0xFF` and the lexer accepts it,
        // so §4.15's one-spelling rule is settled here rather than by refusing
        // the paste (`lexer/number.rs::canonical_int`).
        ExprKind::Int => crate::lexer::canonical_int(src.slice(node.span)),
        ExprKind::Float
        | ExprKind::Str
        | ExprKind::Char
        | ExprKind::Bool
        | ExprKind::NullPtr
        | ExprKind::Name => src.slice(node.span).to_string(),
        ExprKind::Hole => "???".to_string(),
        ExprKind::Unary { op, operand } => {
            let op = match op {
                UnaryOp::Neg => "-",
                UnaryOp::Not => "!",
                UnaryOp::BitNot => "~",
            };
            format!("{op}{}", wrapped(ast, src, *operand, 10))
        }
        ExprKind::Binary { op, left, right } => {
            let mine = power(ast, id);
            // Left-associative everywhere, so an equally-binding operator on
            // the right *does* need its parentheses: `a - (b - c)`.
            format!(
                "{} {} {}",
                wrapped(ast, src, *left, mine),
                binary_op(*op),
                wrapped(ast, src, *right, mine + 1)
            )
        }
        ExprKind::Field { base, name } => {
            format!("{}.{}", wrapped(ast, src, *base, 7), src.slice(*name))
        }
        ExprKind::Index { base, index } => {
            format!("{}[{}]", wrapped(ast, src, *base, 7), render(ast, src, *index))
        }
        ExprKind::Call { callee, args } => {
            format!("{}({})", wrapped(ast, src, *callee, 7), args_line(ast, src, args))
        }
        ExprKind::Method { receiver, name, args } => format!(
            "{}.{}({})",
            wrapped(ast, src, *receiver, 7),
            src.slice(*name),
            args_line(ast, src, args)
        ),
        ExprKind::Case { name, args } => {
            if args.is_empty() {
                format!(".{}", src.slice(*name))
            } else {
                format!(".{}({})", src.slice(*name), args_line(ast, src, args))
            }
        }
        ExprKind::Array(items) => {
            let rendered: Vec<String> =
                items.iter().map(|item| render(ast, src, *item)).collect();
            format!("[{}]", rendered.join(", "))
        }
        ExprKind::Map(entries) => {
            let rendered: Vec<String> = entries
                .iter()
                .map(|entry| {
                    format!(
                        "{}: {}",
                        render(ast, src, entry.key),
                        render(ast, src, entry.value)
                    )
                })
                .collect();
            format!("{{{}}}", rendered.join(", "))
        }
        ExprKind::Try(inner) => format!("{}?", wrapped(ast, src, *inner, 7)),
        // Headers only: `fmt.rs` prints the blocks under them.
        ExprKind::If { branches, .. } => match branches.first() {
            Some(branch) => format!("if {}", render(ast, src, branch.cond)),
            None => "if".to_string(),
        },
        ExprKind::Match { scrutinee, .. } => {
            format!("match {}", render(ast, src, *scrutinee))
        }
        // A file with an unreadable expression is not formatted at all (the
        // command refuses on any diagnostic), so this is unreachable in
        // practice and printed rather than panicked on.
        ExprKind::Error => "???".to_string(),
    }
}

/// `id` rendered, in parentheses only if it binds looser than `needed`.
fn wrapped(ast: &Ast, src: &Source, id: ExprId, needed: u8) -> String {
    let text = render(ast, src, id);
    if power(ast, id) < needed && !matches!(ast.exprs[id.0 as usize].kind, ExprKind::If { .. } | ExprKind::Match { .. })
    {
        return format!("({text})");
    }
    text
}

pub(super) fn args_line(ast: &Ast, src: &Source, args: &[Arg]) -> String {
    let rendered: Vec<String> = args.iter().map(|arg| arg_text(ast, src, arg)).collect();
    rendered.join(", ")
}

pub(super) fn arg_text(ast: &Ast, src: &Source, arg: &Arg) -> String {
    let name = match arg.name {
        Some(name) => format!("{}: ", src.slice(name)),
        None => String::new(),
    };
    let marker = if arg.mutable { "@" } else { "" };
    format!("{name}{marker}{}", render(ast, src, arg.value))
}

fn binary_op(op: BinaryOp) -> &'static str {
    match op {
        BinaryOp::BitAnd => "&",
        BinaryOp::BitOr => "|",
        BinaryOp::BitXor => "^",
        BinaryOp::Shl => "<<",
        BinaryOp::Shr => ">>",
        BinaryOp::Add => "+",
        BinaryOp::Sub => "-",
        BinaryOp::Mul => "*",
        BinaryOp::Div => "/",
        BinaryOp::Rem => "%",
        BinaryOp::Eq => "==",
        BinaryOp::Ne => "!=",
        BinaryOp::Lt => "<",
        BinaryOp::Le => "<=",
        BinaryOp::Gt => ">",
        BinaryOp::Ge => ">=",
        BinaryOp::And => "&&",
        BinaryOp::Or => "||",
    }
}
