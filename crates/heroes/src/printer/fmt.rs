//! `heroes fmt`: the canonical formatter (design.md §4.15).
//!
//! Mandatory and gofmt-style, for two reasons that have nothing to do with
//! taste: it removes every style decision from whoever writes the code — pure
//! spend, no meaning — and it makes any textual difference between two
//! versions *semantic*.
//!
//! Three policies, stated here because they are choices and someone will ask:
//!
//! 1. **Parentheses are minimal.** `2 + (3 * 4)` loses them, `(2 + 3) * 4`
//!    keeps them (`fmt_expr.rs`).
//! 2. **Lines break at 88 columns**, and only inside brackets — panel 007
//!    allows no other continuation. A list breaks by newline (§4.9), an
//!    argument list by comma.
//! 3. **Blank lines are content, not layout.** One blank line always
//!    separates top-level declarations; inside a body, a single blank line
//!    survives where the author put one, and runs collapse to one. The
//!    alternative — deleting them — would erase the only grouping a body has,
//!    and gofmt made the same call.
//!
//! Comments are never dropped, including the ones the tree does not keep:
//! `parse` hands back every comment span, and they are interleaved by line.
//! A blank line between a comment and the declaration below it is preserved
//! *exactly*, because that blank line is what tells documentation from an
//! ordinary remark (§4.1).

use crate::source::{Source, Span};
use crate::syntax::{Ast, Case, Decl, DeclKind, Field, Function};

use super::fmt_expr::render;
use super::types::render_type;

/// Where a line stops being one line. Not a language rule — one constant,
/// changeable, and the only thing that decides single- versus multi-line
/// literals (§4.9 leaves the choice to the formatter on purpose).
pub(super) const WIDTH: usize = 88;

/// Canonical Heroes source for a parsed file. Callers must check that the
/// file had no diagnostics first: formatting a tree built out of recovery
/// guesses would rewrite the author's file into the parser's guess.
pub fn format_file(ast: &Ast, comments: &[Span], src: &Source) -> String {
    let mut fmt = Fmt { out: String::new(), next_comment: 0, last_line: 0 };
    // The library's comments go with its declarations. Filtering only the decls
    // would leave its header block behind, trailing every formatted file.
    let kept: Vec<Span> = comments.iter().copied().filter(|c| src.is_root(c.start)).collect();
    let comments: &[Span] = &kept;
    // **The author's declarations, never the library's.** `fmt` hands a program
    // back, and with `--in-place` it writes it into their file — so a formatter
    // that walked the whole `Source` would append the library to it, once per
    // run (§1.11, `crate::library`).
    let mut items: Vec<Item> = Vec::new();
    for used in ast.uses.iter().filter(|u| src.is_root(u.span.start)) {
        items.push(Item::Use(used));
    }
    for decl in ast.decls.iter().filter(|d| src.is_root(decl_start(src, d))) {
        items.push(Item::Decl(decl));
    }
    // **Source order, not a canonical order.** A formatter that hoisted or
    // sorted `use` lines would have to move the comments attached to them, and
    // this one interleaves comments by line — so reordering is where a
    // formatter starts deleting the author's remarks. gofmt sorts imports and
    // can afford to; it has a comment model this one does not.
    items.sort_by_key(|item| item.start(src));
    let mut previous: Option<&Item> = None;
    // The `extern` group, rebuilt. The parser flattened it (§4.19, panel 036),
    // so the canonical form is **a run of consecutive members sharing a head
    // line** — which is what the author wrote, and the only rule that survives
    // `fmt(fmt(x)) == fmt(x)`: printing one head per member would round-trip a
    // two-signature group into two groups, and re-grouping non-adjacent members
    // would move declarations past each other.
    let mut open_group: Option<(&str, Option<&str>)> = None;
    for item in &items {
        let line = src.line_of(item.start(src));
        let group = match item {
            Item::Decl(decl) => extern_group(src, decl),
            Item::Use(_) => None,
        };
        let continues = group.is_some() && group == open_group;
        open_group = group;
        // A group's first member is printed **under a head line the member's own
        // span does not contain**, so the line every comment and blank-line rule
        // keys on is the head's, not the signature's. Without this the §4.1 rule
        // ("a blank line makes a comment a remark rather than documentation")
        // fired on a gap that only exists because the head is in between, and
        // `fmt` inserted a blank line the author had not written.
        let line = match item {
            Item::Decl(decl) if !continues => {
                extern_header_span(decl).map_or(line, |span| src.line_of(span.start))
            }
            _ => line,
        };
        // One blank line between top-level declarations — but not between two
        // `use` lines, which are a block the way a run of fields is.
        let after_use = matches!(previous, Some(Item::Use(_)));
        if let Some(before) = previous {
            let run = (matches!(before, Item::Use(_)) && matches!(item, Item::Use(_))) || continues;
            if !run {
                fmt.blank_line();
            }
        }
        previous = Some(item);
        fmt.comments_before(src, comments, line, if continues { 4 } else { 0 });
        match item {
            Item::Use(used) => {
                // §4.1 again, and it is the file header that needs it: a blank
                // line between a comment and the first `use` is what makes that
                // comment a remark about the file instead of documentation for
                // the line under it. Not between two `use` lines, though —
                // there the run is one block and its internal gaps close.
                if !after_use && fmt.last_line > 0 && line > fmt.last_line + 1 {
                    fmt.blank_line();
                }
                fmt.line(0, &format!("use {}", src.slice(used.name)));
                fmt.last_line = line;
                fmt.trailing_comment(src, comments, line);
            }
            Item::Decl(decl) => {
                // A blank line between the comment group and this declaration is
                // preserved exactly: it is what makes those comments ordinary
                // remarks rather than the declaration's documentation (§4.1). The
                // rule has to hold for the first declaration too — that is where a
                // file's header block lives.
                if fmt.last_line > 0 && line > fmt.last_line + 1 && !continues {
                    fmt.blank_line();
                }
                fmt.declaration(ast, src, comments, decl, continues);
            }
        }
    }
    // Comments after the last declaration still belong to the file.
    fmt.comments_before(src, comments, u32::MAX, 0);
    fmt.out
}

/// One top-level line, of the two kinds the file has. `use` lines live outside
/// `Ast::decls` (they lower to nothing), and the formatter is one of the three
/// places that has to put the two streams back into the order the author reads.
enum Item<'a> {
    Use(&'a crate::syntax::Use),
    Decl(&'a Decl),
}

impl Item<'_> {
    fn start(&self, src: &Source) -> u32 {
        match self {
            Item::Use(used) => used.span.start,
            Item::Decl(decl) => decl_start(src, decl),
        }
    }
}

/// The line a declaration starts on. Since panel 018 every declaration
/// begins at its kind keyword, which sits before the name the tree carries.
fn decl_start(src: &Source, decl: &Decl) -> u32 {
    let _ = src;
    decl.span.start.min(decl.name.start)
}

pub(super) struct Fmt {
    /// `pub(super)` because `fmt_stmt.rs` implements the statement half on
    /// this same struct. Nothing else writes to it.
    pub(super) out: String,
    pub(super) next_comment: usize,
    /// Source line of the last thing printed — how "was there a blank line
    /// here?" is answered without a second pass.
    pub(super) last_line: u32,
}

impl Fmt {
    pub(super) fn line(&mut self, indent: usize, text: &str) {
        if !text.is_empty() {
            self.out.push_str(&" ".repeat(indent));
            self.out.push_str(text);
        }
        self.out.push('\n');
    }

    pub(super) fn blank_line(&mut self) {
        if !self.out.is_empty() && !self.out.ends_with("\n\n") {
            self.out.push('\n');
        }
    }

    /// Print every comment that sits above source line `line`, at `indent`,
    /// preserving a single blank line wherever the source had one.
    pub(super) fn comments_before(
        &mut self,
        src: &Source,
        comments: &[Span],
        line: u32,
        indent: usize,
    ) {
        while self.next_comment < comments.len() {
            let span = comments[self.next_comment];
            let comment_line = src.line_of(span.start);
            if comment_line >= line {
                return;
            }
            if self.last_line > 0 && comment_line > self.last_line + 1 {
                self.blank_line();
            }
            self.line(indent, src.slice(span).trim_end());
            self.last_line = comment_line;
            self.next_comment += 1;
        }
    }

    /// A comment that sits on the same line as what was just printed goes at
    /// the end of that line, two spaces out.
    pub(super) fn trailing_comment(&mut self, src: &Source, comments: &[Span], line: u32) {
        if self.next_comment >= comments.len() {
            return;
        }
        let span = comments[self.next_comment];
        let comment_line = src.line_of(span.start);
        if comment_line != line {
            return;
        }
        let text = src.slice(span).trim_end();
        // Splice it before the newline the statement already wrote.
        if self.out.ends_with('\n') {
            self.out.pop();
        }
        self.out.push_str("  ");
        self.out.push_str(text);
        self.out.push('\n');
        self.next_comment += 1;
    }

    fn declaration(
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
            DeclKind::Constant { ty, body } => {
                self.line(0, &format!("constant {name}: {}", render_type(ast, *ty, src)));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                self.block(ast, src, comments, body, 4);
            }
            DeclKind::Function(function) => {
                let indent = match function.header {
                    Some(header) => {
                        if !continues {
                            self.line(0, &extern_head(src, function));
                            self.last_line = src.line_of(header.start);
                        }
                        4
                    }
                    None => 0,
                };
                self.line(indent, &signature(ast, src, name, function));
                self.last_line = line;
                self.trailing_comment(src, comments, line);
                if let Some(body) = &function.body {
                    self.block(ast, src, comments, body, 4);
                }
            }
            DeclKind::Record { fields } => {
                self.line(0, &format!("record {name}"));
                self.last_line = line;
                for field in fields {
                    self.field(ast, src, comments, field, 4);
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
/// `extern "sqlite3.h" link "sqlite3"` — the group's head line, rebuilt from any
/// one of its members, since every member carries both spans (§4.19).
fn extern_head(src: &Source, function: &Function) -> String {
    let mut out = String::from("extern ");
    if let Some(header) = function.header {
        out.push_str(src.slice(header)); // quotes included
    }
    if let Some(link) = function.link {
        out.push_str(" link ");
        out.push_str(src.slice(link));
    }
    out
}

/// The header and library a declaration belongs to, as text — `None` for
/// anything that is not an `extern`. Compared by **text, not by span**: two
/// members of one group have different spans and the same words.
fn extern_group<'a>(src: &'a Source, decl: &Decl) -> Option<(&'a str, Option<&'a str>)> {
    let header = extern_header_span(decl)?;
    let DeclKind::Function(function) = &decl.kind else { return None };
    Some((src.slice(header), function.link.map(|span| src.slice(span))))
}

/// The head line's header string, as a span — where the group *begins*, which is
/// earlier than any member's own span.
fn extern_header_span(decl: &Decl) -> Option<Span> {
    let DeclKind::Function(function) = &decl.kind else { return None };
    function.header
}

fn signature(ast: &Ast, src: &Source, name: &str, function: &Function) -> String {
    let mut out = String::new();
    out.push_str("function ");
    out.push_str(name);
    if !function.generics.is_empty() {
        let names: Vec<&str> =
            function.generics.iter().map(|span| src.slice(*span)).collect();
        out.push_str(&format!("<{}>", names.join(", ")));
    }
    out.push('(');
    let params: Vec<String> = function
        .params
        .iter()
        .map(|param| {
            let marker = if param.mutable { "@" } else { "" };
            format!(
                "{marker}{}: {}",
                src.slice(param.name),
                render_type(ast, param.ty, src)
            )
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

/// A one-line rendering, for the width test. Kept here so the breaking
/// decision and the printing agree on what a line costs.
pub(super) fn one_line(ast: &Ast, src: &Source, head: &str, value: crate::syntax::ExprId) -> String {
    format!("{head}{}", render(ast, src, value))
}
