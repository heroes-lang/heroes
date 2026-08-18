//! Telling the author which cycle, and on which field (design.md §4.3; panel 023).
//!
//! Split out of `sized.rs` by the §11 sweep. `sized.rs` finds a cycle; this decides
//! what the reader is shown about it, and the two are separable because the second
//! is bound by rules the first is not:
//!
//! - **the caret lands on the FIELD**, not the declaration — Swift's shape, chosen
//!   by the panel over rustc's — because the field is where the repair goes.
//! - **the cycle is rotated to start at the lowest declaration index**, so the
//!   reader gets the same message whichever declaration the walk happened to enter
//!   from. Without it, reordering two unrelated declarations in a file rewrites a
//!   golden's `.expected`.
//! - **the fix is a `guess`**, never `certain`: `[T]` and `{K: V}` both break the
//!   cycle and the compiler cannot know which the author meant. rustc's equivalent
//!   is `HasPlaceholders`; Swift emits none at all.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::sized::Edge;
use super::errors;

/// The cycle as a list of edge indices, rotated to start at the lowest
/// declaration index.
///
/// Rotated because the reader must get the same message whichever declaration
/// the walk happened to enter from — otherwise reordering two unrelated
/// declarations in a file rewrites a golden's `.expected`.
pub(super) fn cycle_from(edges: &[Edge], path: &[usize], closing: usize, target: u32) -> Vec<usize> {
    let start = path.iter().position(|&edge| edges[edge].from == target);
    let mut cycle: Vec<usize> = match start {
        Some(at) => path[at..].to_vec(),
        // A self-edge: the path holds nothing, the closing edge is the whole
        // cycle.
        None => Vec::new(),
    };
    cycle.push(closing);
    let lowest = cycle
        .iter()
        .enumerate()
        .min_by_key(|(_, &edge)| edges[edge].from)
        .map(|(at, _)| at)
        .unwrap_or(0);
    cycle.rotate_left(lowest);
    cycle
}

/// One diagnostic per cycle, at the field that closes it, with the path in
/// notes — Swift's shape (panel 023 R5).
///
/// Swift is the only precedent in Heroes' position, a value type with no escape
/// hatch, and its own Sema tests put the error on the **field** rather than the
/// type header. Go printed only `invalid recursive type T2` for a mutual pair and
/// that was filed as a bug; both edges of `A → B → A` are legal repair sites, so
/// naming one silently would be arbitrary advice dressed as help.
pub(super) fn report(ast: &Ast, src: &Source, edges: &[Edge], cycle: &[usize]) -> Diagnostic {
    let last = *cycle.last().expect("a cycle has at least one edge");
    let name_of = |decl: u32| src.slice(ast.decls[decl as usize].name).to_string();
    let kind_of = |decl: u32| match ast.decls[decl as usize].kind {
        DeclKind::Variant { .. } => "variant",
        _ => "record",
    };
    let steps: Vec<String> = cycle
        .iter()
        .map(|&edge| {
            // `A.b: B (line 5)` rather than prose: an article before a type name
            // is a grammar problem the compiler cannot solve (`a A`), and the
            // colon form is the one the author wrote.
            //
            // The step is located from **the diagnostic's own caret**, so a cycle
            // that crosses a module boundary names the file for the steps that are
            // elsewhere and stays terse for the ones that are not.
            let at = src.elsewhere(edges[last].field.start, edges[edge].field.start);
            format!(
                "{}.{}: {} ({at})",
                name_of(edges[edge].from),
                src.slice(edges[edge].name),
                name_of(edges[edge].to),
            )
        })
        .collect();
    errors::no_size(
        kind_of(edges[last].from),
        &name_of(edges[last].from),
        &name_of(edges[last].to),
        cycle.len() == 1,
        &steps,
        edges[last].field,
    )
}
