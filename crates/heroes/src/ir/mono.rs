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
//! **Termination is not an accident and not a depth limit.** Polymorphic
//! recursion — `f<T>` calling `f<[T]>` — instantiates forever, type-checks clean
//! today, and design.md does not mention it. MLton's whole-program monomorphiser
//! has been total for twenty-five years *because* SML bans it; inference for it is
//! undecidable (Henglein 1993; Kfoury–Tiuryn–Urzyczyn 1993). Rust is the warning:
//! it accepts at type-check and blows up at codegen with `reached the recursion
//! limit while instantiating`, no error code, late and unattributable. So this
//! pass refuses it **structurally** — instantiating `f` at `S` from within `f` at
//! `T` where `S` properly contains `T` is unbounded — and reports it as a program
//! diagnostic, exit 1, naming both instantiations.

use std::collections::{BTreeMap, BTreeSet};

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{render_ty, Checked, Ty, TyId};

use super::inst::{Callee, Op};
use super::{Function, Phase, Program};

/// One instantiation: which declaration, at which type arguments.
type Instance = (u32, Vec<TyId>);

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
        let calls = calls_in(&program.functions[index], &generic, checked, &[]);
        enqueue(calls, None, &mut queue, &mut seen);
    }

    let mut instances: Vec<Function> = Vec::new();
    let mut at = 0;
    while at < queue.len() {
        let ((decl, args), parent, span) = queue[at].clone();
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
        let calls = calls_in(&instance, &generic, checked, &args);
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
        let _ = parent;
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
/// `subst` is the enclosing instance's own type arguments, so a call written
/// inside a generic body comes back already substituted where the caller can
/// apply it.
fn calls_in(
    function: &Function,
    generic: &BTreeSet<u32>,
    checked: &Checked,
    subst: &[TyId],
) -> Vec<(Instance, Span)> {
    let _ = subst;
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

/// Is this instance a *growing* repeat of one on its own parent chain?
///
/// The test is containment, not equality: `f<[T]>` reached from `f<T>` is
/// unbounded, while `f<int>` reached from `f<int>` is the ordinary recursion that
/// every compiler has and that terminates because the instance already exists.
#[allow(clippy::too_many_arguments)]
fn recursive(
    queue: &[(Instance, Option<usize>, Span)],
    at: usize,
    decl: u32,
    args: &[TyId],
    checked: &Checked,
    ast: &Ast,
    src: &Source,
    span: Span,
) -> Option<Diagnostic> {
    let mut walk = queue[at].1;
    while let Some(index) = walk {
        let ((ancestor_decl, ancestor_args), parent, _) = &queue[index];
        if *ancestor_decl == decl
            && ancestor_args.len() == args.len()
            && ancestor_args
                .iter()
                .zip(args)
                .all(|(small, big)| contains(checked, *big, *small))
            && ancestor_args.iter().zip(args).any(|(small, big)| small != big)
        {
            let show = |list: &[TyId]| {
                list.iter()
                    .map(|t| render_ty(&checked.types, ast, src, *t, &[]))
                    .collect::<Vec<String>>()
                    .join(", ")
            };
            return Some(polymorphic_recursion(&show(ancestor_args), &show(args), span));
        }
        walk = *parent;
    }
    None
}

/// §4.12 has no shape for this, so the message carries the whole explanation: what
/// was instantiated, what it reached, and the one edit that ends it.
///
/// A **program** diagnostic, exit 1 — not exit 2. The program is legal under
/// §4.12 as written, and "the compiler could not run" would be a lie about whose
/// mistake it is.
fn polymorphic_recursion(from: &str, to: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "polymorphic_recursion",
        format!(
            "this call instantiates the function that contains it at a LARGER type — \
             `<{from}>` reaches `<{to}>`, which reaches a larger one again, without end"
        ),
        span,
    )
    .with_note(
        "a generic function is compiled once per type it is used at, so a chain that \
         never repeats a type never ends"
            .to_string(),
    )
    .with_note(
        "pass the value along unchanged, or take the recursive step in a \
         non-generic helper"
            .to_string(),
    )
}

/// Is `small` `big`, or a part of it? `[i64]` contains `i64`; `i64` does not
/// contain `[i64]`.
fn contains(checked: &Checked, big: TyId, small: TyId) -> bool {
    if big == small {
        return true;
    }
    match checked.types.get(big) {
        Ty::Array(element) => contains(checked, element, small),
        Ty::Fallible(inner) => contains(checked, inner, small),
        Ty::Map(key, value) => {
            contains(checked, key, small) || contains(checked, value, small)
        }
        Ty::Func { params, result } => {
            checked.types.params_of(params).iter().any(|p| contains(checked, *p, small))
                || contains(checked, result, small)
        }
        _ => false,
    }
}

/// One copy of `template`, with every `Ty::Generic(i)` replaced by `args[i]`.
///
/// Every type in the body goes through `apply`, which interns as it substitutes —
/// so the descriptor the emitter later asks for comes from the **substituted**
/// `TyId` through `descriptors::pointer`, and is never carried across as a name. A
/// wrong descriptor here is invisible to clang, ASan, UBSan and the leak counter
/// alike: `[-0.0] == [0.0]` would print `false` at exit 0 (panel 029 R4c).
fn instantiate(template: &Function, args: &[TyId], checked: &mut Checked) -> Function {
    let slots = template
        .slots
        .iter()
        .map(|slot| super::Slot {
            name: slot.name.clone(),
            ty: apply(checked, slot.ty, args),
            kind: slot.kind,
        })
        .collect();
    let values = template.values.iter().map(|v| apply(checked, *v, args)).collect();
    let blocks = template
        .blocks
        .iter()
        .map(|block| super::Block {
            preds: block.preds.clone(),
            insts: block
                .insts
                .iter()
                .map(|inst| super::Inst {
                    dest: inst.dest,
                    op: inst.op,
                    ty: apply(checked, inst.ty, args),
                    span: inst.span,
                })
                .collect(),
            term: clone_term(&block.term),
            note: block.note.clone(),
        })
        .collect();
    Function {
        name: template.name.clone(),
        decl: template.decl,
        kind: template.kind,
        generics: Vec::new(),
        params: template.params.clone(),
        result: apply(checked, template.result, args),
        slots,
        blocks,
        values,
        args: template.args.clone(),
        steps: template.steps.clone(),
        instance: args.to_vec(),
        span: template.span,
    }
}

/// `Term` is not `Copy` — it carries a `switch`'s arm table — so the clone is
/// written out rather than derived, which keeps a new terminator kind a compile
/// error here rather than a silently dropped arm.
fn clone_term(term: &super::Term) -> super::Term {
    match term {
        super::Term::Return(value) => super::Term::Return(*value),
        super::Term::Jump(block) => super::Term::Jump(*block),
        super::Term::Branch { cond, then, otherwise } => {
            super::Term::Branch { cond: *cond, then: *then, otherwise: *otherwise }
        }
        super::Term::Switch { tag, cases } => {
            super::Term::Switch { tag: *tag, cases: cases.clone() }
        }
        super::Term::Unreachable => super::Term::Unreachable,
        super::Term::Open => super::Term::Open,
    }
}

/// `ty` with the type parameters substituted, interning whatever is new.
fn apply(checked: &mut Checked, ty: TyId, args: &[TyId]) -> TyId {
    match checked.types.get(ty) {
        Ty::Generic(position) => match args.get(position as usize) {
            Some(bound) => *bound,
            // The checker reported an uninferable parameter; this keeps the shape.
            None => ty,
        },
        Ty::Array(element) => {
            let element = apply(checked, element, args);
            checked.types.intern(Ty::Array(element))
        }
        Ty::Fallible(inner) => {
            let inner = apply(checked, inner, args);
            checked.types.intern(Ty::Fallible(inner))
        }
        Ty::Map(key, value) => {
            let key = apply(checked, key, args);
            let value = apply(checked, value, args);
            checked.types.intern(Ty::Map(key, value))
        }
        Ty::Func { params, result } => {
            let spelled: Vec<TyId> = checked
                .types
                .params_of(params)
                .into_iter()
                .map(|p| apply(checked, p, args))
                .collect();
            let result = apply(checked, result, args);
            checked.types.func(&spelled, result)
        }
        _ => ty,
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
