//! Monomorphisation: one copy of a generic function per concrete type tuple
//! (design.md §4.12, panel 029).
//!
//! **This is the only sugar row erased by a pass rather than by lowering.** Part
//! 5's table says so itself — "monomorphisation on the IR, after type checking" —
//! and the table's closing sentence used to claim otherwise until panel 029 had it
//! amended. So `Ty::Generic` is present in the IR, `--dump-ir` shows it, and this
//! is what removes it.
//!
//! **It runs before `own.rs`, and the reason is a defect rather than a
//! preference.** `is_refcounted` answers `false` for `Ty::Generic`
//! (`types/counted.rs`), which is right for `T = int` and a leak for `T = str` —
//! so an ownership pass running first does not fail to decide, it **decides
//! wrongly**, and `ir/phases.rs` cannot catch it because `released_on_return`
//! asks the same predicate. Measured on a real body before this file existed.
//!
//! **What it does not do: recompute the substitution.** The checker binds the type
//! parameters to type the call, and `Checked::instantiations` records them keyed
//! by the call's span. Re-deriving them here would be a second answer to one
//! question — the shape `types/counted.rs` names as "a refcount bug that
//! reproduces once a week". The panel costed a `TyArgs` pool on `Op::Call` for
//! this; it turned out unnecessary, because `Inst` already carries the span.
//!
//! This file decides **which instances exist**: it walks out from the non-generic
//! roots, queues what each body calls, and stops. The §11 sweep took the two
//! questions that are not that one:
//!
//! - `mono_subst.rs` — what one instance *is*, once it is known to exist.
//! - `mono_recursion.rs` — why the walk terminates. Polymorphic recursion is
//!   refused structurally there, with the literature that says why a depth limit
//!   is the wrong answer.

use std::collections::{BTreeMap, BTreeSet};

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{Checked, TyId};

use super::inst::{Callee, Op};
use super::mono_recursion::recursive;
use super::mono_subst::{apply, instantiate};
use super::{Function, Phase, Program};

/// One instantiation: which declaration, at which type arguments.
pub(super) type Instance = (u32, Vec<TyId>);

/// Replace every generic function with its instances, and delete the originals.
///
/// Returns the diagnostics it found. A program with none is monomorphic
/// afterwards, which `Phase::Mono`'s verifier check asserts.
pub fn run(
    program: &mut Program,
    checked: &mut Checked,
    ast: &Ast,
    src: &Source,
) -> Vec<Diagnostic> {
    let mut diagnostics = Vec::new();
    let generic: BTreeSet<u32> =
        program.functions.iter().filter(|f| !f.generics.is_empty()).map(|f| f.decl).collect();
    if generic.is_empty() {
        program.advance_to(Phase::Mono);
        return diagnostics;
    }

    // Every instance, with the instance it was reached from — the parent chain is
    // what makes polymorphic recursion visible.
    let mut queue: Vec<(Instance, Option<usize>, Span)> = Vec::new();
    let mut seen: BTreeSet<Instance> = BTreeSet::new();
    let roots: Vec<usize> = program
        .functions
        .iter()
        .enumerate()
        .filter(|(_, f)| f.generics.is_empty())
        .map(|(index, _)| index)
        .collect();
    for index in roots {
        let calls = calls_in(&program.functions[index], &generic, checked);
        enqueue(calls, None, &mut queue, &mut seen);
    }

    let mut instances: Vec<Function> = Vec::new();
    let mut at = 0;
    while at < queue.len() {
        let ((decl, args), _, span) = queue[at].clone();
        if let Some(problem) = recursive(&queue, at, decl, &args, checked, ast, src, span) {
            diagnostics.push(problem);
            at += 1;
            continue;
        }
        let Some(template) = program.functions.iter().find(|f| f.decl == decl) else {
            at += 1;
            continue;
        };
        let instance = instantiate(template, &args, checked);
        // **The callee's arguments are the caller's, substituted.** A generic call
        // inside a generic body records `T` — the template's own parameter — so
        // instantiating from it verbatim would carry `Ty::Generic` into the copy.
        // That was this pass's first defect, and the verifier caught it: "an
        // instruction still has a type parameter in it".
        let calls = calls_in(&instance, &generic, checked);
        let calls: Vec<(Instance, Span)> = calls
            .into_iter()
            .map(|((decl, callee_args), span)| {
                let applied =
                    callee_args.into_iter().map(|t| apply(checked, t, &args)).collect();
                ((decl, applied), span)
            })
            .collect();
        enqueue(calls, Some(at), &mut queue, &mut seen);
        instances.push(instance);
        at += 1;
    }

    // The templates go: they have no C, and leaving one would emit a body with
    // `Ty::Generic` in it.
    program.functions.retain(|f| f.generics.is_empty());
    program.functions.extend(instances);
    // **`counted` is dense over the interner and was built last**, on the stated
    // ground that nothing may intern a type after it (`types/mod.rs`). This pass
    // interns `[str]` where only `[A]` existed, so a `TyId` past the end would read
    // as uncounted — "a leak that no test can see", in that comment's own words.
    crate::types::extend_counted(checked, ast);
    program.advance_to(Phase::Mono);
    diagnostics
}

/// Every generic call in this function, as it was recorded.
///
/// The arguments come back **as recorded**, which means a call written inside a
/// generic body still mentions the template's own `T`. Substituting them is the
/// caller's job and is done at the one site that knows the enclosing instance —
/// stated here because this function took a `subst` parameter it never read for
/// as long as it existed, with a doc line saying it did.
fn calls_in(
    function: &Function,
    generic: &BTreeSet<u32>,
    checked: &Checked,
) -> Vec<(Instance, Span)> {
    let mut found = Vec::new();
    for block in &function.blocks {
        for inst in &block.insts {
            let Op::Call { callee: Callee::Heroes(decl), .. } = inst.op else { continue };
            if !generic.contains(&decl) {
                continue;
            }
            let Some(args) = checked.instantiations.get(&inst.span.start) else { continue };
            found.push(((decl, args.clone()), inst.span));
        }
    }
    found
}

/// Add what has not been seen, remembering which instance reached it.
fn enqueue(
    calls: Vec<(Instance, Span)>,
    parent: Option<usize>,
    queue: &mut Vec<(Instance, Option<usize>, Span)>,
    seen: &mut BTreeSet<Instance>,
) {
    for (instance, span) in calls {
        if seen.insert(instance.clone()) {
            queue.push((instance, parent, span));
        }
    }
}

/// The instances of one declaration, for the emitter's mangler and for
/// `--dump-ir`'s instance table.
pub fn instances_of(program: &Program, decl: u32) -> Vec<Vec<TyId>> {
    program
        .functions
        .iter()
        .filter(|f| f.decl == decl && !f.instance.is_empty())
        .map(|f| f.instance.clone())
        .collect()
}

/// Which instance a call reaches, so the emitter names the same function the pass
/// created. Read from the same table the pass read: one answer, one place.
pub fn instance_at(checked: &Checked, span: Span) -> Option<&Vec<TyId>> {
    checked.instantiations.get(&span.start)
}

/// Every declaration that had type parameters, so the emitter knows a call needs a
/// suffix. Derived from the AST rather than the program, because the pass has
/// already deleted the templates by the time the emitter runs.
pub fn generic_decls(ast: &Ast) -> BTreeMap<u32, usize> {
    let mut found = BTreeMap::new();
    for (index, decl) in ast.decls.iter().enumerate() {
        if let crate::syntax::DeclKind::Function(function) = &decl.kind {
            if !function.generics.is_empty() {
                found.insert(index as u32, function.generics.len());
            }
        }
    }
    found
}
