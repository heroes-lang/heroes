//! `match` — the only destructuring construct, and it lowers two ways
//! (design.md §4.7, §4.6's `.ok`/`.err`, panel 017 A on a diverging arm).
//!
//! **There is no `match` in the IR.** There is a `switch` on a variant's tag,
//! which is exhaustive by the time it gets here (M-data-declarations proved it) and therefore
//! needs no default edge; and there is a chain of two-way branches for `i64` and
//! `str` arms, where exhaustiveness is impossible and `_` is the only way to close
//! the match (§4.7). Two shapes, because C has exactly the same two: a `switch` on
//! an integer tag, and `if`-chains for everything else.
//!
//! A payload binding is a `payload` instruction, not a copy of the layout: `.num
//! n` binds `n` to the case's small record (§4.2), whose type is the one the
//! checker gave the binding. A `T?` is the same shape with two fixed cases — `ok`
//! is 0 and `err` is 1 — because §4.6 says a `T?` *is* a built-in variant, and
//! nothing here needs to know it was built in.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Arm, ArmBody, Ast, DeclKind, ExprId, PatternKind};
use crate::types::{Checked, Ty, TyId};

use super::build::Lowering;
use super::control::{seal_join, store_arm};
use super::inst::{BinOp, BlockId, Op, SlotId, Term, ValueId};
use super::{exprs, stmts, SlotKind};

/// The two cases of a `T?`, in tag order (§4.6).
const FALLIBLE_CASES: [&str; 2] = ["ok", "err"];

#[allow(clippy::too_many_arguments)]
pub(super) fn lower(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    scrutinee: ExprId,
    arms: &[Arm],
    ty: TyId,
    span: Span,
) -> ValueId {
    let subject_ty = checked.expr_types[scrutinee.0 as usize];
    let value = exprs::expr(b, ast, resolved, checked, src, scrutinee);
    // The subject goes in a slot: every arm that binds a payload reads it again,
    // and a `match` on a call must not call twice.
    let subject = b.synthetic("s", subject_ty);
    b.store(subject, value, span);

    let wants_value = ty != b.unit_ty() && ty != checked.types.error();
    let result = if wants_value { Some(b.synthetic("r", ty)) } else { None };
    let join = b.block("match: join");

    let plan = Plan { subject, result, join, wants_value, span };
    match cases_of(ast, checked, src, subject_ty) {
        Some(names) => tagged(b, ast, resolved, checked, src, &plan, &names, arms),
        None => literals(b, ast, resolved, checked, src, &plan, arms),
    }

    b.switch_to(join);
    seal_join(b, join, result, ty, span)
}

/// What both shapes need to know: where the subject is, where an arm's value goes,
/// and where control lands afterwards. Grouped because the alternative is an
/// eleven-parameter function twice over.
struct Plan {
    subject: SlotId,
    result: Option<SlotId>,
    join: BlockId,
    wants_value: bool,
    span: Span,
}

/// The case names of whatever this `match` destructures, in tag order — or `None`
/// where the subject is an `i64` or a `str` and there are no cases to switch on.
fn cases_of(ast: &Ast, checked: &Checked, src: &Source, subject: TyId) -> Option<Vec<String>> {
    match checked.types.get(subject) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => {
                Some(cases.iter().map(|case| src.slice(case.name).to_string()).collect())
            }
            _ => None,
        },
        Ty::Fallible(_) => Some(FALLIBLE_CASES.iter().map(|name| name.to_string()).collect()),
        _ => None,
    }
}

/// A `switch` on the tag: one edge per case, dense, no default.
fn tagged(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    plan: &Plan,
    names: &[String],
    arms: &[Arm],
) {
    let int = checked.types.int();
    let held = b.load(plan.subject, plan.span);
    let tag = b.emit(Op::Tag(held), int, plan.span);

    // One block per arm; a `|`-joined arm is several tags pointing at one block.
    let mut edges: Vec<Option<BlockId>> = vec![None; names.len()];
    let mut bodies: Vec<(BlockId, usize, Option<u32>)> = Vec::new();
    for (index, arm) in arms.iter().enumerate() {
        let block = b.block("match: arm");
        let mut bound_case = None;
        for pattern in &arm.patterns {
            match &pattern.kind {
                PatternKind::Case { name, binding } => {
                    if let Some(case) = index_of(names, src, *name) {
                        edges[case as usize] = Some(block);
                        // `.num _` binds **nothing** (§4.7), so there is no payload
                        // to read and no slot to make. Reading it anyway would put a
                        // slot named `_` in the dump and, worse, ask the checker for
                        // the type of a binding it never made.
                        if binding.is_some_and(|b| src.slice(b) != "_") {
                            bound_case = Some(case);
                        }
                    }
                }
                // `_` on a variant is forbidden (§4.7), so this only fires where it
                // is legal — and it fills every case no arm claimed.
                PatternKind::Wildcard => {
                    for edge in edges.iter_mut() {
                        if edge.is_none() {
                            *edge = Some(block);
                        }
                    }
                }
                PatternKind::Literal(_) | PatternKind::Error => {}
            }
        }
        bodies.push((block, index, bound_case));
    }

    // M-data-declarations proved exhaustiveness, so an unclaimed case cannot happen — and saying so
    // with `unreachable` is CLAUDE.md §7's rule rather than a safety net.
    let unclaimed = edges.iter().any(|edge| edge.is_none());
    let orphan = if unclaimed { Some(b.block("match: unclaimed")) } else { None };
    let cases: Vec<BlockId> =
        edges.iter().map(|edge| edge.or(orphan).unwrap_or(plan.join)).collect();
    b.terminate(Term::Switch { tag, cases });
    if let Some(block) = orphan {
        b.switch_to(block);
        b.terminate(Term::Unreachable);
    }

    for (block, index, bound_case) in bodies {
        b.switch_to(block);
        let arm = &arms[index];
        if let (Some(case), Some(name)) = (bound_case, binding_of(arm)) {
            bind_payload(b, checked, src, plan, case, name);
        }
        let value = body(b, ast, resolved, checked, src, arm, plan.wants_value);
        store_arm(b, plan.result, value, plan.span);
        b.terminate(Term::Jump(plan.join));
    }
}

/// `.num n` — the payload of a known case, into a slot the body can read.
fn bind_payload(
    b: &mut Lowering,
    checked: &Checked,
    src: &Source,
    plan: &Plan,
    case: u32,
    name: Span,
) {
    let held = b.load(plan.subject, name);
    let ty = match b.local_at(name) {
        Some(local) => checked.local_type(local as usize),
        None => checked.types.error(),
    };
    let payload = b.emit(Op::Payload { base: held, case }, ty, name);
    let slot = b.slot(src.slice(name).to_string(), ty, SlotKind::Local);
    if let Some(local) = b.local_at(name) {
        b.bind_local(local, slot);
    }
    b.store(slot, payload, name);
}

/// A chain of equality tests: `i64` and `str` arms, where exhaustiveness is
/// impossible and `_` closes the match (§4.7).
fn literals(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    plan: &Plan,
    arms: &[Arm],
) {
    let bool_ty = checked.types.bool();
    let mut pending: Vec<(BlockId, usize)> = Vec::new();
    for (index, arm) in arms.iter().enumerate() {
        let block = b.block("match: arm");
        pending.push((block, index));
        if arm.patterns.iter().any(|p| matches!(p.kind, PatternKind::Wildcard)) {
            b.terminate(Term::Jump(block));
            continue;
        }
        for pattern in &arm.patterns {
            if let PatternKind::Literal(literal) = &pattern.kind {
                let held = b.load(plan.subject, plan.span);
                let expected = exprs::expr(b, ast, resolved, checked, src, *literal);
                let same = b.emit(
                    Op::Binary { op: BinOp::Eq, left: held, right: expected },
                    bool_ty,
                    plan.span,
                );
                let next = b.block("match: test");
                b.terminate(Term::Branch { cond: same, then: block, otherwise: next });
                b.switch_to(next);
            }
        }
    }
    // Falling past every arm cannot happen: the checker required `_`.
    b.terminate(Term::Unreachable);

    for (block, index) in pending {
        b.switch_to(block);
        let value = body(b, ast, resolved, checked, src, &arms[index], plan.wants_value);
        store_arm(b, plan.result, value, plan.span);
        b.terminate(Term::Jump(plan.join));
    }
}

/// An arm's body: one statement inline, or a block whose value is its last
/// expression (§4.7, panel 014 — one form, seen two ways).
fn body(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    arm: &Arm,
    wants_value: bool,
) -> Option<ValueId> {
    match &arm.body {
        ArmBody::Stmt(id) => {
            let produced = stmts::statement(b, ast, resolved, checked, src, *id);
            if wants_value {
                produced
            } else {
                None
            }
        }
        ArmBody::Block(block) => stmts::block(b, ast, resolved, checked, src, block, wants_value),
    }
}

fn index_of(names: &[String], src: &Source, name: Span) -> Option<u32> {
    let wanted = src.slice(name);
    names.iter().position(|candidate| candidate == wanted).map(|index| index as u32)
}

/// The payload name a single-pattern arm binds, if it binds one. A `|`-joined arm
/// cannot: two cases have two payload types, so there is nothing one name could be
/// (§4.7).
fn binding_of(arm: &Arm) -> Option<Span> {
    if arm.patterns.len() != 1 {
        return None;
    }
    match &arm.patterns[0].kind {
        PatternKind::Case { binding, .. } => *binding,
        _ => None,
    }
}
