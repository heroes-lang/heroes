//! What can be wrong with a set of modules, once it is one text.
//!
//! Three checks, and they run here rather than in discovery for one reason: a
//! `Span` may only point into the finished `Source`, and discovery works on
//! files that have no offsets in it yet. So discovery skips what it cannot read
//! and this pass says so, with the caret on the `use` line the author has open.
//!
//! - **a module that is not there.** Discovery tried `<name>.hero` beside the
//!   importing file and found nothing.
//! - **a cycle.** Refused (panel 031 R6), and refused with no sentence in the
//!   spec: the error is loud and it names the ring, which is more than a
//!   sentence could carry. Refusing is the reversible direction — relaxing later
//!   breaks nothing, while tightening at M9, where the per-module cache needs a
//!   topological order, would break the port.
//! - **two modules that mangle to one C name.** `module_of` sanitises to
//!   `[A-Za-z0-9]`, so `geo_m` and `geom` are one component and clang answers
//!   `redefinition of 'h_geom_Point'` — exit 2, *the compiler is wrong*, on a
//!   legal program. Panel 031 R10 makes it a Heroes diagnostic naming both.

use crate::diagnostics::Diagnostic;
use crate::source::{module_of, Source};
use crate::syntax::Ast;

/// Every diagnostic the module *graph* produces. Ordered by the position of the
/// `use` line that provokes it, so the output reads down the file.
pub fn errors(ast: &Ast, src: &Source) -> Vec<Diagnostic> {
    let mut out = Vec::new();
    unknown_modules(ast, src, &mut out);
    collisions(src, &mut out);
    if out.is_empty() {
        // Only when every name resolves: a cycle report that names a module
        // which does not exist would be a second diagnostic for one mistake.
        cycles(ast, src, &mut out);
    }
    out.sort_by_key(|d| d.span.start);
    out
}

/// `use geom` with no `geom.hero` beside the file that says it.
fn unknown_modules(ast: &Ast, src: &Source, out: &mut Vec<Diagnostic>) {
    for used in &ast.uses {
        if src.is_library(used.span.start) {
            continue;
        }
        let name = src.slice(used.name);
        if src.files().iter().any(|f| f.module == name) {
            continue;
        }
        let importer = src.file(used.span.start);
        out.push(Diagnostic::new(
            "unknown_module",
            format!(
                "there is no module `{name}` — `use {name}` reads `{name}.hero`, beside `{}`, and that file is not there",
                importer.name
            ),
            used.name,
        ));
    }
}

/// Two modules whose names differ and whose C components do not.
fn collisions(src: &Source, out: &mut Vec<Diagnostic>) {
    for (index, file) in src.files().iter().enumerate() {
        for earlier in &src.files()[..index] {
            if earlier.module == file.module || module_of(&earlier.module) != module_of(&file.module)
            {
                continue;
            }
            out.push(
                Diagnostic::new(
                    "module_names_collide",
                    format!(
                        "modules `{}` and `{}` are one name to the compiler — every name reaching C is `h_<module>_<name>` with the module reduced to letters and digits, so both become `h_{}_…`",
                        earlier.module,
                        file.module,
                        module_of(&file.module)
                    ),
                    crate::source::Span { start: file.start, end: file.start },
                )
                .with_note(format!(
                    "rename one of them: `{}` and `{}` differ only where the rule cannot see",
                    earlier.name, file.name
                )),
            );
        }
    }
}

/// A ring of `use` lines. Reported once, on the edge that closes it, with the
/// whole ring in the note — an error naming one edge of a cycle is an error the
/// reader has to reconstruct.
fn cycles(ast: &Ast, src: &Source, out: &mut Vec<Diagnostic>) {
    let modules: Vec<&str> = src.files().iter().map(|f| f.module.as_str()).collect();
    // Edges as (from, to, the span of the `use` that makes it), in source order.
    let mut edges: Vec<(usize, usize, crate::source::Span)> = Vec::new();
    for used in &ast.uses {
        let from = src.file_of(used.span.start);
        let name = src.slice(used.name);
        if let Some(to) = modules.iter().position(|m| *m == name) {
            edges.push((from, to, used.name));
        }
    }

    let mut state = vec![Visit::New; modules.len()];
    let mut stack: Vec<usize> = Vec::new();
    for start in 0..modules.len() {
        if state[start] == Visit::New && walk(start, &edges, &mut state, &mut stack, src, out) {
            return;
        }
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Visit {
    New,
    Open,
    Done,
}

/// Depth-first, reporting the first back edge. Returns whether it reported.
///
/// Recursive, and deliberately: the recursion is over the module graph, which a
/// cycle bounds and `Open` marks. The port reads this as the same walk.
fn walk(
    node: usize,
    edges: &[(usize, usize, crate::source::Span)],
    state: &mut Vec<Visit>,
    stack: &mut Vec<usize>,
    src: &Source,
    out: &mut Vec<Diagnostic>,
) -> bool {
    state[node] = Visit::Open;
    stack.push(node);
    for (from, to, span) in edges {
        if *from != node {
            continue;
        }
        if state[*to] == Visit::Open {
            let modules: Vec<&str> = src.files().iter().map(|f| f.module.as_str()).collect();
            let at = stack.iter().position(|n| n == to).unwrap_or(0);
            let mut ring: Vec<&str> = stack[at..].iter().map(|n| modules[*n]).collect();
            ring.push(modules[*to]);
            out.push(
                Diagnostic::new(
                    "module_cycle",
                    format!(
                        "modules may not form a cycle, and this `use` closes one: {}",
                        ring.join(" uses ")
                    ),
                    *span,
                )
                .with_note(
                    "move what both modules need into a third one that neither uses"
                        .to_string(),
                ),
            );
            stack.pop();
            return true;
        }
        if state[*to] == Visit::New && walk(*to, edges, state, stack, src, out) {
            stack.pop();
            return true;
        }
    }
    stack.pop();
    state[node] = Visit::Done;
    false
}
