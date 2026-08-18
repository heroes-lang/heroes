//! Types, written back out in surface syntax.
//!
//! There is exactly one correct spelling of every type (§4.15's canonical
//! form), so this renderer has no options and no choices to make. It is used
//! by `--dump-ast` today, by diagnostics as soon as they name a type, and by
//! `heroes fmt` when the formatter lands — written once, used everywhere.

use crate::source::Source;
use crate::syntax::{Ast, TypeId, TypeKind};

pub fn render_type(ast: &Ast, id: TypeId, src: &Source) -> String {
    let mut out = String::new();
    write_type(ast, id, src, &mut out);
    out
}

fn write_type(ast: &Ast, id: TypeId, src: &Source, out: &mut String) {
    let node = &ast.types[id.0 as usize];
    match &node.kind {
        TypeKind::Named => out.push_str(src.slice(node.span)),
        TypeKind::Unit => out.push_str("()"),
        // `i32[4]` — the length is part of the type, so it is part of the text.
        TypeKind::Fixed(elem, n) => {
            write_type(ast, *elem, src, out);
            out.push_str(&format!("[{n}]"));
        }
        TypeKind::Array(elem) => {
            out.push('[');
            write_type(ast, *elem, src, out);
            out.push(']');
        }
        TypeKind::Map(key, value) => {
            out.push('{');
            write_type(ast, *key, src, out);
            out.push_str(": ");
            write_type(ast, *value, src, out);
            out.push('}');
        }
        TypeKind::Fallible(inner) => {
            write_type(ast, *inner, src, out);
            out.push('?');
        }
        TypeKind::Func { params, result } => {
            out.push_str("(function(");
            for (i, param) in params.iter().enumerate() {
                if i > 0 {
                    out.push_str(", ");
                }
                write_type(ast, *param, src, out);
            }
            out.push_str(") -> ");
            write_type(ast, *result, src, out);
            out.push(')');
        }
        // A type the parser could not read. The diagnostic says why; the
        // dump says *where*, which is what makes a broken file still
        // readable.
        TypeKind::Error => out.push_str("<?>"),
    }
}
