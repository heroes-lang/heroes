//! Every arm of a `match`: what its patterns bind, and what value it produces.
//!
//! Split from `stmts.rs` because it is the one statement-level walk that answers
//! a *type* question as well as a control-flow one, and because it is where three
//! separate rules meet: the pattern binds (`patterns.rs`), the body produces a
//! value or jumps (`stmts.rs`), and the set of arms must be exhaustive
//! (`patterns::exhaustive`).

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, PatternKind};

use super::join::Branch;
use super::stmts::{block, statement, Flow, Want};
use super::{Checker, TyId};


/// Every arm of a `match`: what its patterns bind, whether the set is exhaustive,
/// and what value it produces.
pub(super) fn arms(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    match_arms: &[crate::syntax::Arm],
    subject: TyId,
    want: Want,
    span: Span,
) -> Vec<Branch> {
    let mut branches: Vec<Branch> = Vec::new();
    let mut covered: Vec<u32> = Vec::new();
    let mut wildcard = false;
    for arm in match_arms {
        for pattern in &arm.patterns {
            match &pattern.kind {
                PatternKind::Case { name, binding } => {
                    let bound = super::patterns::case_pattern(
                        checker, ast, src, subject, *name, pattern.span, &mut covered,
                    );
                    if let Some(binding) = binding {
                        checker.bind_local(*binding, bound);
                    }
                }
                PatternKind::Wildcard => {
                    wildcard = true;
                    super::patterns::wildcard(checker, ast, src, subject, pattern.span);
                }
                PatternKind::Literal(literal) => {
                    super::patterns::literal(
                        checker, ast, resolved, src, subject, *literal, pattern.span,
                    );
                }
                PatternKind::Error => {}
            }
        }
        let value = match &arm.body {
            crate::syntax::ArmBody::Stmt(id) => {
                let (flow, produced) = statement(checker, ast, resolved, src, *id, want);
                if flow == Flow::Jumps {
                    None
                } else {
                    produced
                }
            }
            crate::syntax::ArmBody::Block(body) => {
                block(checker, ast, resolved, src, body, want).1
            }
        };
        branches.push(Branch { value, span: arm.span });
    }
    super::patterns::exhaustive(checker, ast, src, subject, &covered, wildcard, span);
    branches
}
