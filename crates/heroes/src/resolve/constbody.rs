//! What a `constant`'s body may contain (design.md §4.2, §4.16; panel 039).
//!
//! Split from `cycles.rs` on 2026-08-12 when the two rules together passed
//! CLAUDE.md §11's ~300 lines. They remain coupled through one value — the set of
//! constants refused here — because a cycle through a body that was already refused
//! is a consequence rather than a finding, and reporting both would be two
//! diagnostics for one mistake.
//!
//! ## Why a body may contain no call
//!
//! Because without that rule a `constant` is not constant, measured 2026-08-12.
//! `constant ARGC: int` with body `len(args())` printed `3` under `heroes run
//! f.hero -- a b c` and `0` under `heroes run f.hero` — the same binary, two values
//! — and a body calling a printing function printed once per read, because
//! `FnKind::Constant` lowers to a zero-argument function and a read to a call.
//! Panel 039's llm-ergonomist vetoed exactly this shape from the spec alone,
//! without knowing it shipped: a construct whose evaluation time cannot be read off
//! its own line.
//!
//! The rule is **syntactic on purpose**. A purity analysis would admit `len("abc")`
//! and refuse `len(args())`, which is a better rule and a whole new pass; nothing in
//! this tree has one. The syntactic rule buys the property outright: a call-free
//! body reads only literals and other constants, and `cycles.rs` proves those are
//! not circular, so its value is a deterministic function of the program text.
//! Re-evaluation per read then costs time and nothing else — and `ir/mod.rs` already
//! reserves folding as the emitter's optimisation.
//!
//! **A variant case is allowed and a record construction is not**, which is an
//! asymmetry this file inherits rather than invents: §4.9 says record construction
//! *is* a call, while `.plus` is a case whose type comes from context. It is panel
//! 035's finding in miniature, and the author's v2 decision (Part 7 item 16) fixes
//! both at once.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, ExprKind};

use super::Resolved;
use super::cycles::{is_valued_constant, owner};

pub(super) fn bodies(
    out: &mut Resolved,
    ast: &Ast,
    src: &Source,
    extent: &[(u32, u32, u32)],
) -> std::collections::BTreeSet<u32> {
    let mut found: Vec<(crate::source::Span, &str, u32)> = Vec::new();
    let mut refused = std::collections::BTreeSet::new();
    for expr in ast.exprs.iter() {
        let what = match &expr.kind {
            ExprKind::Call { .. } => "a call",
            ExprKind::Method { .. } => "a call written with `.`",
            ExprKind::Try(_) => "a `?`",
            ExprKind::Hole => "a `???`",
            // Everything a definition may be: literals, the containers built from
            // them, names, operators, a field or element of one, and the two
            // constructs that choose between values.
            ExprKind::Int
            | ExprKind::Float
            | ExprKind::Str
            | ExprKind::Char
            | ExprKind::Bool
            | ExprKind::NullPtr
            | ExprKind::Name
            | ExprKind::Unary { .. }
            | ExprKind::Binary { .. }
            | ExprKind::Field { .. }
            | ExprKind::Index { .. }
            | ExprKind::Case { .. }
            | ExprKind::Array(_)
            | ExprKind::Map(_)
            | ExprKind::If { .. }
            | ExprKind::Match { .. }
            | ExprKind::Error => continue,
        };
        let Some(owner) = owner(extent, expr.span.start) else { continue };
        if !is_valued_constant(ast, owner) {
            continue;
        }
        found.push((expr.span, what, owner));
    }
    // **The outermost offender only.** `len(args())` is two calls and one mistake,
    // and two diagnostics for one mistake is a defect this project has fixed three
    // times (the lexer, the parser and the resolver each hold the invariant). An
    // expression strictly inside another offender is that one's inside.
    for (span, what, owner) in &found {
        if found.iter().any(|(outer, _, _)| outer.start <= span.start && span.end < outer.end) {
            continue;
        }
        let name = src.slice(ast.decls[*owner as usize].name);
        out.diagnostics.push(body_diagnostic(name, what, *span));
        refused.insert(*owner);
    }
    refused
}

/// The message. It names the constant, because the caret is on the call and the
/// rule is about the declaration around it.
fn body_diagnostic(name: &str, what: &str, span: crate::source::Span) -> Diagnostic {
    Diagnostic::new(
        "constant_body",
        format!(
            "`constant {name}`'s body may not contain {what} — it may only read literals \
             and other constants"
        ),
        span,
    )
    .with_note(
        "a `constant` is read by calling its accessor, so a body that can call \
         anything is re-evaluated on every read — and `len(args())` made one whose \
         value depended on the command line (panel 039)"
            .to_string(),
    )
    .with_note(
        "for a value that is computed, write a `function` and call it where the value \
         is wanted"
            .to_string(),
    )
}
