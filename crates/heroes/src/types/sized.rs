//! Which types have a size, and in what order C can be told about them
//! (design.md §3.1's declaration ordering, §4.10, panel 023).
//!
//! Heroes has no `Box`, no `ref` and no pointer type, so a record is a C struct
//! **by value** and a variant a tagged union by value. A type is therefore finite
//! exactly when its containment graph is acyclic, and one walk answers both
//! questions the backend has:
//!
//! - **an order**, because C needs every struct complete before it is used by
//!   value, and Heroes' top level is order-free with free mutual recursion;
//! - **the cycles**, which are `no_size` errors.
//!
//! They are the same walk on purpose (panel 023 R3). Two walks would be two
//! answers to one question, and `ir/layout.rs` records what that costs: "two
//! answers that disagree … is a refcount bug that reproduces once a week."
//!
//! **An edge is a field C lays out by value** — the property, never a list of
//! permitted indirections. Panel 023 measured the list being wrong in both
//! directions at once: `record Scope { vars: {str: Scope} }` compiles at 8 bytes
//! because a map is a pointer, and `record R { next: R? }` is `field has
//! incomplete type` because a `T?` is a by-value struct and therefore
//! **transparent** — it propagates the edge into `T` rather than breaking it. The
//! property gets every row right without naming one, and it cannot drift when M6
//! adds function values.
//!
//! **The walk is iterative**, and that is not a style preference. The check that
//! prevents a hang is the one that historically hangs: rustc #84611 is a stack
//! overflow *inside* this very check, Nim #13715 a SIGSEGV, Zig #21436 an
//! undetected cycle plus a crash. A recursive `fn visit` here would be the
//! documented failure mode of four compilers.

use crate::diagnostics::Diagnostic;
use crate::resolve::{Resolved, TypeRef};
use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind, TypeId, TypeKind};

use super::errors;

/// One by-value containment edge: `from`'s field lands a whole `to` inside it.
///
/// `field` is the span the caret points at — the field's *type* as written, so
/// that `child: Node` underlines `Node` and a `next: R?` underlines `R?`, which
/// is where the repair goes. `name` is the field's identifier, for the path a
/// note prints.
struct Edge {
    from: u32,
    to: u32,
    name: Span,
    field: Span,
}

/// What the walk publishes: aggregate declarations in an order where every
/// by-value dependency precedes its user.
///
/// Empty when the file has a cycle, because there is no order then and the
/// emitter must never run on a program that has diagnostics anyway.
pub fn order_and_cycles(
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
) -> (Vec<u32>, Vec<Diagnostic>) {
    let edges = collect(ast, resolved);
    walk(ast, src, &edges)
}

/// The by-value edges, for the invariant asserted over `heroes mutate`'s corpus.
///
/// It hands back what `collect` produced — the **same** function the walk uses. An
/// invariant checked against a re-derived edge set would only be asserting that two
/// implementations agree, which is the thing this file exists to have one of.
#[cfg(test)]
pub(in crate::types) fn by_value_edges(ast: &Ast, resolved: &Resolved) -> Vec<(u32, u32)> {
    collect(ast, resolved).into_iter().map(|edge| (edge.from, edge.to)).collect()
}

/// Every by-value edge in the file, in declaration order — which is what makes
/// the reported cycle stable, and therefore makes CLAUDE.md §9's `#~ no_size`
/// annotation land on the same line every run.
fn collect(ast: &Ast, resolved: &Resolved) -> Vec<Edge> {
    let mut edges = Vec::new();
    for (index, decl) in ast.decls.iter().enumerate() {
        let from = index as u32;
        match &decl.kind {
            DeclKind::Record { fields } => {
                for field in fields {
                    push_edge(&mut edges, ast, resolved, from, field.name, field.ty);
                }
            }
            // A case with fields *is* a small record (§4.2), so its payload
            // carries edges exactly as a record's fields do. A payload-free case
            // has none — it contributes no bytes to the union at all.
            DeclKind::Variant { cases } => {
                for case in cases {
                    for field in &case.fields {
                        push_edge(&mut edges, ast, resolved, from, field.name, field.ty);
                    }
                }
            }
            DeclKind::Constant { .. } | DeclKind::Function(_) | DeclKind::Test { .. } => {}
        }
    }
    edges
}

fn push_edge(
    edges: &mut Vec<Edge>,
    ast: &Ast,
    resolved: &Resolved,
    from: u32,
    name: Span,
    ty: TypeId,
) {
    if let Some(to) = by_value_target(ast, resolved, ty) {
        edges.push(Edge { from, to, name, field: ast.types[ty.0 as usize].span });
    }
}

/// The declaration a field lands *by value*, if any.
///
/// Every arm is listed rather than folded into a catch-all (panel 023's second
/// condition, and `ir/layout.rs:83`'s reason for the same shape): a type added to
/// the language must not become silently sizeless because a `_` arm answered for
/// it.
fn by_value_target(ast: &Ast, resolved: &Resolved, ty: TypeId) -> Option<u32> {
    match &ast.types[ty.0 as usize].kind {
        // The only edge: a bare name that resolves to a `record` or `variant`.
        // A primitive, a generic parameter and an unresolved name are all leaves.
        TypeKind::Named => match resolved.type_uses[ty.0 as usize] {
            TypeRef::Top(decl) => Some(decl),
            TypeRef::Prim(_) | TypeRef::Generic(_) | TypeRef::Unresolved => None,
        },
        // Transparent: a `T?` is a by-value struct, so it carries `T` inside it.
        // This is the row the proposal got wrong and two judges found
        // independently (panel 023).
        TypeKind::Fallible(inner) => by_value_target(ast, resolved, *inner),
        // The indirections. Both are one pointer wide whatever they hold, so
        // neither lands its contents here and neither constrains C's ordering —
        // measured: three records mutually recursive through arrays compile in
        // the worst order with zero forward declarations.
        TypeKind::Array(_) | TypeKind::Map(_, _) => None,
        // A function value is a C function pointer: `record R { f:
        // (function(R) -> int) }` compiles and runs, measured. M6 owns the
        // emission; the edge set already has the right answer.
        TypeKind::Func { .. } => None,
        // No bytes, and a type the parser could not read — the diagnostic for
        // which is already written.
        TypeKind::Unit | TypeKind::Error => None,
    }
}

/// Iterative depth-first search: a post-order for the acyclic part, one
/// diagnostic per distinct cycle.
///
/// Colours rather than a `visited` flag, because the difference *is* the
/// algorithm: grey means "on the current path", which is what makes a back edge
/// a cycle instead of a diamond. A `visited` flag would reject `record A { xs:
/// [B] } / record B { a: A }` — a legal shape, since the cycle passes through
/// `[T]` — and panel 023's historian predicted that exact false positive as the
/// first defect this file would have.
fn walk(ast: &Ast, src: &Source, edges: &[Edge]) -> (Vec<u32>, Vec<Diagnostic>) {
    const WHITE: u8 = 0;
    const GREY: u8 = 1;
    const BLACK: u8 = 2;

    let count = ast.decls.len();
    let mut colour = vec![WHITE; count];
    let mut order: Vec<u32> = Vec::new();
    let mut reported: Vec<Vec<u32>> = Vec::new();
    let mut diagnostics = Vec::new();
    // The current path, as edge indices — a cycle is a suffix of it, so the
    // reconstruction needs no second search.
    let mut path: Vec<usize> = Vec::new();

    for root in 0..count {
        if colour[root] != WHITE || !is_aggregate(ast, root as u32) {
            continue;
        }
        // Each frame is a node plus how many of its out-edges are done, which is
        // what an explicit stack needs in place of the return address a
        // recursive walk would use.
        let mut stack: Vec<(u32, usize)> = vec![(root as u32, 0)];
        colour[root] = GREY;
        while let Some((node, step)) = stack.pop() {
            let out = out_edges(edges, node);
            match out.get(step) {
                None => {
                    colour[node as usize] = BLACK;
                    order.push(node);
                    path.pop();
                }
                Some(&edge) => {
                    stack.push((node, step + 1));
                    let target = edges[edge].to;
                    match colour[target as usize] {
                        GREY => {
                            // A back edge: the cycle is this edge plus the
                            // suffix of the path that starts at `target`.
                            let cycle = cycle_from(edges, &path, edge, target);
                            let nodes: Vec<u32> = cycle.iter().map(|&e| edges[e].from).collect();
                            if !reported.contains(&nodes) {
                                reported.push(nodes);
                                diagnostics.push(report(ast, src, edges, &cycle));
                            }
                        }
                        WHITE => {
                            colour[target as usize] = GREY;
                            path.push(edge);
                            stack.push((target, 0));
                        }
                        _ => {}
                    }
                }
            }
        }
    }
    // A cyclic file publishes **no** order, not a partial one. The DFS finishes
    // every node it entered, including the ones on a cycle, so `order` at this
    // point is a sequence that looks like an answer and is not one — there is no
    // topological order of a graph with a cycle. Emptying it buys the invariant
    // panel 023's compiler-engineer asked for: whenever the emitter runs,
    // `type_order` is a topological order *by construction*, because a file with a
    // cycle exits 1 at `check`. Found by the test that asserted it.
    if !diagnostics.is_empty() {
        order.clear();
    }
    (order, diagnostics)
}

fn is_aggregate(ast: &Ast, decl: u32) -> bool {
    matches!(
        ast.decls[decl as usize].kind,
        DeclKind::Record { .. } | DeclKind::Variant { .. }
    )
}

fn out_edges(edges: &[Edge], node: u32) -> Vec<usize> {
    edges
        .iter()
        .enumerate()
        .filter(|(_, edge)| edge.from == node)
        .map(|(index, _)| index)
        .collect()
}

/// The cycle as a list of edge indices, rotated to start at the lowest
/// declaration index.
///
/// Rotated because the reader must get the same message whichever declaration
/// the walk happened to enter from — otherwise reordering two unrelated
/// declarations in a file rewrites a golden's `.expected`.
fn cycle_from(edges: &[Edge], path: &[usize], closing: usize, target: u32) -> Vec<usize> {
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
fn report(ast: &Ast, src: &Source, edges: &[Edge], cycle: &[usize]) -> Diagnostic {
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
            let (line, _) = src.line_col(edges[edge].field.start);
            format!(
                "{}.{}: {} (line {line})",
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
