//! One declaration, printed (design.md §4.2, §4.15; panel 018).
//!
//! Split out of `fmt.rs` by the §11 sweep. `fmt.rs` decides the *order* of the
//! file's items and where the blank lines between them go; this decides what one
//! item looks like once it is its turn.
//!
//! The five kinds share one shape, and it is worth seeing at a glance because a
//! sixth kind will have to join it: print the header line, record its source line
//! so the comment rules have something to key on, take any trailing comment, then
//! descend into whatever the kind carries — a body, fields, cases.

use crate::source::{Source, Span};
use crate::syntax::{Ast, Case, Decl, DeclKind, Field, Function};

use super::fmt::Fmt;
use super::types::render_type;

impl Fmt {
    pub(super) fn declaration(
        &mut self,
        ast: &Ast,
        src: &Source,
        comments: &[Span],
        decl: &Decl,
        continues: bool,
    ) {
        let name = src.slice(decl.name);
        let line = src.line_of(decl.name.start);
        match &decl.kind {
            DeclKind::Constant { ty, body, header, .. } => {
                let indent = self.extern_head_once(src, comments, decl, *header, continues);
                self.line(indent, &format!("constant {name}: {}", render_type(ast, *ty, src)));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                if let Some(body) = body {
                    self.block(ast, src, comments, body, 4);
                }
            }
            DeclKind::Function(function) => {
                let indent = self.extern_head_once(src, comments, decl, function.header, continues);
                self.line(indent, &signature(ast, src, name, function));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                if let Some(body) = &function.body {
                    self.block(ast, src, comments, body, 4);
                }
            }
            // A `record` prints exactly like the other two group members: the head
            // line once, then the declaration at the group's indent, then the
            // fields one level deeper than *that*. Printing it at 0 unconditionally
            // is what hoisted it out of its group (panel 060's condition 1).
            DeclKind::Record { fields, header, .. } => {
                let indent = self.extern_head_once(src, comments, decl, *header, continues);
                self.line(indent, &format!("record {name}"));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                for field in fields {
                    self.field(ast, src, comments, field, indent + 4);
                }
            }
            DeclKind::Variant { cases } => {
                self.line(0, &format!("variant {name}"));
                self.last_line = line;
                for case in cases {
                    self.case(ast, src, comments, case);
                }
            }
            DeclKind::Test { body } => {
                // The name is the string literal, quotes included.
                self.line(0, &format!("test {name}"));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                self.block(ast, src, comments, body, 4);
            }
        }
    }

    fn field(&mut self, ast: &Ast, src: &Source, comments: &[Span], field: &Field, indent: usize) {
        let line = src.line_of(field.name.start);
        self.comments_before(src, comments, line, indent);
        self.line(
            indent,
            &format!("{}: {}", src.slice(field.name), render_type(ast, field.ty, src)),
        );
        self.last_line = line;
        self.trailing_comment(src, comments, line);
    }

    fn case(&mut self, ast: &Ast, src: &Source, comments: &[Span], case: &Case) {
        let line = src.line_of(case.name.start);
        self.comments_before(src, comments, line, 4);
        self.line(4, src.slice(case.name));
        self.last_line = line;
        self.trailing_comment(src, comments, line);
        for field in &case.fields {
            self.field(ast, src, comments, field, 8);
        }
    }
}

/// `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`.
///
/// `-> ()` is never printed: a function that returns nothing writes no arrow
/// (§4.2's `function main()`), so the canonical form has one spelling.
pub(super) fn signature(ast: &Ast, src: &Source, name: &str, function: &Function) -> String {
    let mut out = String::new();
    out.push_str("function ");
    out.push_str(name);
    if !function.generics.is_empty() {
        let names: Vec<&str> = function.generics.iter().map(|span| src.slice(*span)).collect();
        out.push_str(&format!("<{}>", names.join(", ")));
    }
    out.push('(');
    let params: Vec<String> = function
        .params
        .iter()
        .map(|param| {
            let marker = if param.mutable { "@" } else { "" };
            format!("{marker}{}: {}", src.slice(param.name), render_type(ast, param.ty, src))
        })
        .collect();
    out.push_str(&params.join(", "));
    out.push(')');
    let result = render_type(ast, function.result, src);
    if result != "()" {
        out.push_str(&format!(" -> {result}"));
    }
    out
}
