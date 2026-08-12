//! What a `constant`'s body may be: not circular, and not a computation that
//! reaches the world (design.md §4.2, §4.16; panel 039).
//!
//! **The file keeps the name `cycles.rs`** although it now owns two checks, for the
//! reason `docs/debrief/QUEUE.md` keeps its own name: dated records cite this path
//! — DESIGN-LOG 2026-08-12, `docs/panel/039`, and a golden's header comment — and
//! CLAUDE.md §14 does not rewrite a record to make a rename tidy. Both checks ask
//! one question, *is this body a definition*, and answering it in one place is what
//! keeps them from disagreeing.
//!
//! ## Why a body may contain no call
//!
//! Because without that rule a `constant` is not constant, measured 2026-08-12.
//! `constant ARGC: int` with body `len(args())` printed `3` under `heroes run f.hero -- a b c` and `0` under `heroes run f.hero`
//! — the same binary, two values — and a body calling a printing function printed
//! once per read, because `FnKind::Constant` lowers to a zero-argument function and
//! a read to a call. Panel 039's llm-ergonomist vetoed exactly this shape from the
//! spec alone, without knowing it shipped: a construct whose evaluation time cannot
//! be read off its own line.
//!
//! The rule is **syntactic on purpose**. A purity analysis would admit `len("abc")`
//! and refuse `len(args())`, which is a better rule and a whole new pass; nothing in
//! this tree has one. The syntactic rule buys the property outright: a call-free body
//! reads only literals and other constants, and the cycle check below proves those
//! are not circular, so its value is a deterministic function of the program text.
//! Re-evaluation per read then costs time and nothing else — and `ir/mod.rs` already
//! reserves folding as the emitter's optimisation.
//!
//! **A variant case is allowed and a record construction is not**, which is an
//! asymmetry this file inherits rather than invents: §4.9 says record construction
//! *is* a call, while `.plus` is a case whose type comes from context. It is panel
//! 035's finding in miniature, and the author's v2 decision (Part 7 item 16) fixes
//! both at once.
//!
//! ## Definitional cycles: a `constant` whose value is its own
//!
//! `constant A: int` with body `B` and `constant B: int` with body `A` used to
//! pass every gate this compiler has. `heroes check` exited 0, the emitted C was
//! two mutually recursive zero-argument accessors, and the program died by
//! **SIGSEGV at `-O0`** and **hung at `-O2`** — one program with two failure modes,
//! chosen by an optimisation flag nobody typed. Measured 2026-08-12; the thesis
//! says every plausible mistake is a compile error, and this one type-checked and
//! linked (author decision: fix it in the resolver, with its own diagnostic).
//!
//! **The edge is "this body reads that declaration", and it is not restricted to
//! constants.** A cycle can leave the constants and come back: `constant A` with
//! body `f()`, where `function f() -> int` returns `A`, is the same defect wearing
//! a function, and it was accepted too. So the graph is every top-level
//! declaration, and the *filter* is on the cycle rather than on the edges:
//!
//! - a cycle of **functions alone is legal** — §4.2 grants mutual recursion with
//!   no forward declarations, and a recursive function has arguments and a base
//!   case;
//! - a cycle containing a **`constant` has neither**. Its accessor takes nothing,
//!   so nothing about the second call can differ from the first.
//!
//! The check is **conservative in the loud direction**: it asks whether the call
//! graph has the cycle, never whether the branch holding it can be taken. That is
//! the same answer `sized.rs` gives about types, for the same reason — an analysis
//! that tried to be exact here would have to decide reachability, and CLAUDE.md
//! §11's rule is that a fallback belongs on the loud side.
//!
//! **Iterative, like `sized.rs`'s walk and not by coincidence.** The check that
//! prevents a hang is the one that historically hangs: rustc #84611 is a stack
//! overflow inside exactly this kind of check, Nim #13715 a SIGSEGV, Zig #21436 an
//! undetected cycle plus a crash.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind, ExprKind};

use super::{Ref, Resolved};

/// Both checks, in the order a reader wants them: what the body *is*, then whether
/// the bodies are circular.
pub fn report(out: &mut Resolved, ast: &Ast, src: &Source) {
    let extent = extents(ast);
    let refused = bodies(out, ast, src, &extent);
    let edges = edges(out, ast, &extent);
    for cycle in walk(ast, &edges) {
        // **A cycle through a body that was already refused is a consequence, not a
        // finding.** `constant THROUGH` / `depends()`, where `depends` returns
        // `THROUGH`, is one mistake — a call in a body — and the cycle exists only
        // because that call does. Reporting both is the two-diagnostics-for-one-
        // mistake defect this compiler has fixed three times over. The body rule is
        // the primary one: it names the line the author wrote.
        if cycle.iter().any(|decl| refused.contains(decl)) {
            continue;
        }
        out.diagnostics.push(diagnose(ast, src, &cycle));
    }
}

/// Refuse the four expression kinds that would make a `constant` a computation
/// rather than a definition.
///
/// Every arm is listed rather than folded into a catch-all — `sized.rs`'s rule and
/// `ir/layout.rs`'s: an expression kind added to the language must not become
/// silently legal here because a `_` arm answered for it.
fn bodies(
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

/// Every declaration's extent, sorted by where it starts.
///
/// Sorted here rather than assumed: `Ast::decls` is in the order the parser
/// produced, and while that is source order today, the ownership question is
/// answered from the spans themselves — a fact about the values in hand, which is
/// what CLAUDE.md §11 asks a lookup to rest on. Declarations do not nest, so a
/// containing extent is unique.
fn extents(ast: &Ast) -> Vec<(u32, u32, u32)> {
    let mut extent: Vec<(u32, u32, u32)> = ast
        .decls
        .iter()
        .enumerate()
        .map(|(index, decl)| (decl.span.start, decl.span.end, index as u32))
        .collect();
    extent.sort_unstable();
    extent
}

/// Which declaration an offset falls inside, if any.
fn owner(extent: &[(u32, u32, u32)], offset: u32) -> Option<u32> {
    let at = extent.partition_point(|(start, _, _)| *start <= offset);
    let (_, end, index) = extent.get(at.checked_sub(1)?)?;
    if offset < *end { Some(*index) } else { None }
}

/// Every "the body of `from` reads `to`" edge, deduplicated.
///
/// A `BTreeSet` because CLAUDE.md §5 allows no other, and because the dedup is
/// what keeps a constant read fifty times from being fifty edges — the walk below
/// would give the same answer either way, and the smaller graph is the one a
/// reader can follow.
fn edges(out: &Resolved, ast: &Ast, extent: &[(u32, u32, u32)]) -> Vec<Vec<u32>> {
    let mut pairs = std::collections::BTreeSet::new();
    for (index, expr) in ast.exprs.iter().enumerate() {
        let Ref::Top(to) = out.uses[index] else { continue };
        // A self-edge here is a body reading its own name, which is the direct
        // cycle: `Ast::exprs` holds no headers, so a declaration's own name never
        // produces one.
        if let Some(from) = owner(extent, expr.span.start) {
            pairs.insert((from, to));
        }
    }
    let mut adjacent = vec![Vec::new(); ast.decls.len()];
    for (from, to) in pairs {
        adjacent[from as usize].push(to);
    }
    adjacent
}

/// Is this declaration a `constant` with a body — the thing that cannot recur?
fn is_valued_constant(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, DeclKind::Constant { body: Some(_), .. })
}

/// Iterative depth-first search; one cycle per distinct set of declarations.
///
/// Colours rather than a `visited` flag, for `sized.rs`'s reason: grey means "on
/// the current path", and that is what tells a back edge from a diamond. Two
/// constants that both read a third are not a cycle, and a `visited` flag would
/// have to be told so.
fn walk(ast: &Ast, adjacent: &[Vec<u32>]) -> Vec<Vec<u32>> {
    const WHITE: u8 = 0;
    const GREY: u8 = 1;
    const BLACK: u8 = 2;

    let count = ast.decls.len();
    let mut colour = vec![WHITE; count];
    let mut found: Vec<Vec<u32>> = Vec::new();
    let mut seen: std::collections::BTreeSet<Vec<u32>> = std::collections::BTreeSet::new();

    for root in 0..count as u32 {
        if colour[root as usize] != WHITE {
            continue;
        }
        // The stack holds (node, how many of its edges have been taken), which is
        // what makes the path a real stack rather than a recursion the machine
        // keeps for us.
        let mut path: Vec<(u32, usize)> = vec![(root, 0)];
        colour[root as usize] = GREY;
        while let Some((node, taken)) = path.pop() {
            let Some(next) = adjacent[node as usize].get(taken).copied() else {
                colour[node as usize] = BLACK;
                continue;
            };
            path.push((node, taken + 1));
            match colour[next as usize] {
                GREY => {
                    // The cycle is the suffix of the path from `next` on — no
                    // second search, because the path *is* the reconstruction.
                    let at = path.iter().position(|(n, _)| *n == next).unwrap_or(0);
                    let cycle: Vec<u32> = path[at..].iter().map(|(n, _)| *n).collect();
                    if cycle.iter().any(|d| is_valued_constant(ast, *d)) {
                        let mut key = cycle.clone();
                        key.sort_unstable();
                        if seen.insert(key) {
                            found.push(cycle);
                        }
                    }
                }
                WHITE => {
                    colour[next as usize] = GREY;
                    path.push((next, 0));
                }
                _ => {}
            }
        }
    }
    found
}

/// The message, anchored at the constant nearest the top of the file.
///
/// Deterministic on purpose: the traversal decides which *rotation* of a cycle it
/// finds, and a diagnostic that moved between runs would move CLAUDE.md §9's `#~
/// constant_cycle` annotation with it. The set is what the traversal cannot
/// change, so the anchor is chosen from the set.
fn diagnose(ast: &Ast, src: &Source, cycle: &[u32]) -> Diagnostic {
    let name = |decl: &u32| src.slice(ast.decls[*decl as usize].name).to_string();
    let anchor = cycle
        .iter()
        .filter(|decl| is_valued_constant(ast, **decl))
        .min_by_key(|decl| ast.decls[**decl as usize].name.start)
        .copied()
        .unwrap_or(cycle[0]);
    let span = ast.decls[anchor as usize].name;
    let holder = name(&anchor);
    let direct = cycle.len() == 1;
    let message = if direct {
        format!("`constant {holder}` is defined as itself, so it has no value")
    } else {
        let through: Vec<String> = cycle.iter().filter(|d| **d != anchor).map(name).collect();
        format!(
            "`constant {holder}` is defined in terms of itself through `{}`, so it has no value",
            through.join("`, `")
        )
    };
    let mut diagnostic = Diagnostic::new("constant_cycle", message, span);
    if !direct {
        // Printed from the anchor round to the anchor, whatever rotation the walk
        // found it in, so the note reads as the reader's own file does.
        let at = cycle.iter().position(|d| *d == anchor).unwrap_or(0);
        let mut steps: Vec<String> = cycle[at..].iter().chain(cycle[..at].iter()).map(name).collect();
        steps.push(holder.clone());
        diagnostic = diagnostic.with_note(format!("the cycle is: {}", steps.join(" reads ")));
    }
    diagnostic = diagnostic
        .with_note(
            "a `constant`'s value is computed by reading it, and its accessor takes no \
             arguments — so nothing about the second call could differ from the first"
                .to_string(),
        )
        .with_note(
            "functions may recur freely (§4.2); a cycle is an error only where a `constant` \
             is on it"
                .to_string(),
        );
    diagnostic.fixes.push(Fix {
        title: format!("give `{holder}` a value that does not read the others"),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}
