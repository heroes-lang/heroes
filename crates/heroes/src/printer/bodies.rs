//! Blocks, statements and expressions, written back out for `--dump-ast`.
//!
//! One decision shapes this file: **every binary and unary expression is
//! printed with its parentheses**, whether the source had them or not.
//! The source `2 + 3 * 4` comes out as `(2 + (3 * 4))`. That is not pretty —
//! `heroes fmt` will print it the way it was written — but it makes the dump
//! *prove* the precedence table instead of describing it, which is the whole
//! reason a tree dump exists.
//!
//! Statements are one per line, each opening with what it is (`bind`,
//! `declare`, `mutate`, `return`…). The exception is a statement that *is* an
//! `if` or a `match`: those keep their keyword and carry their blocks
//! indented below, because that is how they are read.

use crate::source::Source;
use crate::syntax::{
    Arg, ArmBody, Ast, BinaryOp, Block, ExprId, ExprKind, StmtKind, UnaryOp,
};

use super::types::render_type;

pub fn write_block(ast: &Ast, src: &Source, block: &Block, indent: usize, out: &mut String) {
    for id in &block.stmts {
        write_stmt(ast, src, *id, indent, out);
    }
}

fn line(out: &mut String, indent: usize, text: &str) {
    out.push_str(&" ".repeat(indent));
    out.push_str(text);
    out.push('\n');
}

fn write_stmt(
    ast: &Ast,
    src: &Source,
    id: crate::syntax::StmtId,
    indent: usize,
    out: &mut String,
) {
    match &ast.stmts[id.0 as usize].kind {
        StmtKind::Bind { name, ty, value } => {
            let annotation = match ty {
                Some(ty) => format!(": {}", render_type(ast, *ty, src)),
                None => String::new(),
            };
            let head = format!("bind {}{annotation} = ", src.slice(*name));
            write_valued(ast, src, &head, *value, indent, out);
        }
        StmtKind::Declare { name, ty, value } => {
            let head = format!(
                "declare {}: {} @ ",
                src.slice(*name),
                render_type(ast, *ty, src)
            );
            write_valued(ast, src, &head, *value, indent, out);
        }
        StmtKind::Mutate { place, value } => {
            let head = format!("mutate {} @ ", render_expr(ast, *place, src));
            write_valued(ast, src, &head, *value, indent, out);
        }
        StmtKind::Return(Some(value)) => {
            write_valued(ast, src, "return ", *value, indent, out);
        }
        StmtKind::Return(None) => line(out, indent, "return"),
        StmtKind::Break => line(out, indent, "break"),
        StmtKind::Continue => line(out, indent, "continue"),
        StmtKind::Assert(value) => write_valued(ast, src, "assert ", *value, indent, out),
        StmtKind::While { cond, block } => {
            line(out, indent, &format!("for {}", render_expr(ast, *cond, src)));
            write_block(ast, src, block, indent + 2, out);
        }
        StmtKind::ForIn { name, iterable, block } => {
            let head = format!(
                "for {} in {}",
                src.slice(*name),
                render_expr(ast, *iterable, src)
            );
            line(out, indent, &head);
            write_block(ast, src, block, indent + 2, out);
        }
        // An `if` or a `match` in statement position keeps its own shape; any
        // other expression is labelled, because a bare line of expression
        // reads like a statement kind that does not exist.
        StmtKind::Expr(value) => match &ast.exprs[value.0 as usize].kind {
            ExprKind::If { .. } | ExprKind::Match { .. } => {
                write_valued(ast, src, "", *value, indent, out)
            }
            _ => write_valued(ast, src, "expr ", *value, indent, out),
        },
        StmtKind::Error => line(out, indent, "error"),
    }
}

/// A statement's head plus its value. Control-flow values carry blocks, so
/// they continue on the lines below; everything else fits on this one.
fn write_valued(
    ast: &Ast,
    src: &Source,
    head: &str,
    value: ExprId,
    indent: usize,
    out: &mut String,
) {
    match &ast.exprs[value.0 as usize].kind {
        ExprKind::If { branches, otherwise } => {
            for (i, branch) in branches.iter().enumerate() {
                let keyword = if i == 0 { "if" } else { "else if" };
                let prefix = if i == 0 { head } else { "" };
                let cond = render_expr(ast, branch.cond, src);
                line(out, indent, &format!("{prefix}{keyword} {cond}"));
                write_block(ast, src, &branch.block, indent + 2, out);
            }
            if let Some(block) = otherwise {
                line(out, indent, "else");
                write_block(ast, src, block, indent + 2, out);
            }
        }
        ExprKind::Match { scrutinee, arms } => {
            let head = format!("{head}match {}", render_expr(ast, *scrutinee, src));
            line(out, indent, &head);
            for arm in arms {
                let patterns: Vec<String> = arm
                    .patterns
                    .iter()
                    .map(|pattern| render_pattern_public(ast, src, pattern))
                    .collect();
                let left = patterns.join(" | ");
                match &arm.body {
                    ArmBody::Expr(value) => {
                        let text = format!("{left} => {}", render_expr(ast, *value, src));
                        line(out, indent + 2, &text);
                    }
                    ArmBody::Block(block) => {
                        line(out, indent + 2, &format!("{left} =>"));
                        write_block(ast, src, block, indent + 4, out);
                    }
                }
            }
        }
        _ => line(out, indent, &format!("{head}{}", render_expr(ast, value, src))),
    }
}

pub(super) fn render_pattern_public(ast: &Ast, src: &Source, pattern: &crate::syntax::Pattern) -> String {
    match &pattern.kind {
        crate::syntax::PatternKind::Case { name, binding } => match binding {
            Some(binding) => format!(".{} {}", src.slice(*name), src.slice(*binding)),
            None => format!(".{}", src.slice(*name)),
        },
        crate::syntax::PatternKind::Wildcard => "_".to_string(),
        crate::syntax::PatternKind::Literal(value) => render_expr(ast, *value, src),
        crate::syntax::PatternKind::Error => "<?>".to_string(),
    }
}

pub fn render_expr(ast: &Ast, id: ExprId, src: &Source) -> String {
    let node = &ast.exprs[id.0 as usize];
    match &node.kind {
        // Literals and names are their own text: the source is the only place
        // that stores it (`Source` owns every byte).
        ExprKind::Int
        | ExprKind::Float
        | ExprKind::Str
        | ExprKind::Char
        | ExprKind::Bool
        | ExprKind::Name => src.slice(node.span).to_string(),
        ExprKind::Hole => "???".to_string(),
        ExprKind::Unary { op, operand } => {
            let op = match op {
                UnaryOp::Neg => "-",
                UnaryOp::Not => "!",
            };
            format!("({op}{})", render_expr(ast, *operand, src))
        }
        ExprKind::Binary { op, left, right } => format!(
            "({} {} {})",
            render_expr(ast, *left, src),
            binary_op(*op),
            render_expr(ast, *right, src)
        ),
        ExprKind::Field { base, name } => {
            format!("{}.{}", render_expr(ast, *base, src), src.slice(*name))
        }
        ExprKind::Index { base, index } => format!(
            "{}[{}]",
            render_expr(ast, *base, src),
            render_expr(ast, *index, src)
        ),
        ExprKind::Call { callee, args } => {
            format!("{}({})", render_expr(ast, *callee, src), render_args(ast, src, args))
        }
        ExprKind::Method { receiver, name, args } => format!(
            "{}.{}({})",
            render_expr(ast, *receiver, src),
            src.slice(*name),
            render_args(ast, src, args)
        ),
        ExprKind::Case { name, args } => {
            if args.is_empty() {
                format!(".{}", src.slice(*name))
            } else {
                format!(".{}({})", src.slice(*name), render_args(ast, src, args))
            }
        }
        ExprKind::Array(items) => {
            let rendered: Vec<String> =
                items.iter().map(|item| render_expr(ast, *item, src)).collect();
            format!("[{}]", rendered.join(", "))
        }
        ExprKind::Map(entries) => {
            let rendered: Vec<String> = entries
                .iter()
                .map(|entry| {
                    format!(
                        "{}: {}",
                        render_expr(ast, entry.key, src),
                        render_expr(ast, entry.value, src)
                    )
                })
                .collect();
            format!("{{{}}}", rendered.join(", "))
        }
        ExprKind::Try(inner) => format!("{}?", render_expr(ast, *inner, src)),
        // Reached only when a control form is nested inside a larger
        // expression; a statement-level one is written by `write_valued`.
        ExprKind::If { .. } => "if …".to_string(),
        ExprKind::Match { .. } => "match …".to_string(),
        ExprKind::Error => "<?>".to_string(),
    }
}

fn render_args(ast: &Ast, src: &Source, args: &[Arg]) -> String {
    let rendered: Vec<String> = args
        .iter()
        .map(|arg| {
            let name = match arg.name {
                Some(name) => format!("{}: ", src.slice(name)),
                None => String::new(),
            };
            let marker = if arg.mutable { "@" } else { "" };
            format!("{name}{marker}{}", render_expr(ast, arg.value, src))
        })
        .collect();
    rendered.join(", ")
}

fn binary_op(op: BinaryOp) -> &'static str {
    match op {
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
