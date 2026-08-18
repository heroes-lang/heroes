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
//! **This file owns the file as a whole**: which items there are, what order
//! they come in, and where the blank lines between them go. What one item
//! *looks like* is `fmt_decl.rs`; what a statement looks like is `fmt_stmt.rs`;
//! comments and their blank lines are `fmt_comments.rs`; and rebuilding a
//! flattened `extern` group is `fmt_extern.rs`. `Fmt` is the state all four
//! share, so it lives here with the two primitives everything is written in.

use crate::source::{Source, Span};
use crate::syntax::{Ast, Decl};

use super::fmt_expr::render;
use super::fmt_extern::{extern_group, extern_header_span};

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
    // `fmt(fmt(x)) == fmt(x)`. The reconstruction itself is `fmt_extern.rs`.
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

/// The formatter's whole state, and it is deliberately three fields: the text so
/// far, how far into the comment stream we are, and the source line of the last
/// thing printed — which is how "was there a blank line here?" is answered
/// without a second pass.
pub(super) struct Fmt {
    /// `pub(super)` because the four sibling files implement their half on this
    /// same struct. Nothing outside `printer/` writes to it.
    pub(super) out: String,
    pub(super) next_comment: usize,
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
}

/// A one-line rendering, for the width test. Kept here so the breaking
/// decision and the printing agree on what a line costs.
pub(super) fn one_line(ast: &Ast, src: &Source, head: &str, value: crate::syntax::ExprId) -> String {
    format!("{head}{}", render(ast, src, value))
}
