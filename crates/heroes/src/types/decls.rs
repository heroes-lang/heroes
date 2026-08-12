//! One declaration at a time: what its body is *for*, and what `return` owes it.
//!
//! Three shapes, and the difference between them is the whole reason `Want`
//! exists:
//!
//! - a **constant**'s body *is* its value, so its last statement is checked
//!   against the declared type (§4.2);
//! - a **function**'s body is run for effect and hands its value back through
//!   `return`, so the body wants nothing and `checker.result` carries the
//!   signature's promise;
//! - a **test**'s body is run for effect and returns nothing at all (§4.18).
//!
//! `record` and `variant` declare types, not values, so there is no body to check —
//! but their field types are **interned here**, eagerly, and that is not
//! bookkeeping. Every later question about a declaration's contents reads the
//! interner: "does this record own a counted reference" (`counted.rs`) is answered
//! over its fields' `TyId`s, and lowering them lazily made the answer depend on
//! whether the *program* happened to read the field. `variant Token` with a `str`
//! payload matched as `.word _` was uncounted, so it copied without an incref — a
//! leak whose cause was a field nobody touched.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::stmts::{block, Flow, Want};
use super::table::Ty;
use super::{errors, lower, Checker, TyId};



pub(super) fn file(checker: &mut Checker, ast: &Ast, resolved: &Resolved, src: &Source) {
    for (index, declaration) in ast.decls.iter().enumerate() {
        let decl = declaration;
        match &decl.kind {
            DeclKind::Constant { ty, body } => {
                let declared = lower::ty(checker, ast, resolved, *ty);
                checker.result = declared;
                checker.fallible = is_fallible(checker, declared);
                checker.generic_names.clear();
                // A constant's body *is* its value: its last statement produces
                // it, unlike a function, which returns.
                let (_, value) = block(checker, ast, resolved, src, body, Want::Value(declared));
                if value.is_none() {
                    let want = checker.show(ast, src, declared);
                    let diagnostic = errors::no_value(&want, body.span);
                    checker.push_diagnostic(diagnostic);
                }
            }
            DeclKind::Function(function) => {
                checker.generic_names = function
                    .generics
                    .iter()
                    .map(|span| src.slice(*span).to_string())
                    .collect();
                let result = lower::ty(checker, ast, resolved, function.result);
                // **`main` produces nothing, and that is enforced rather than
                // assumed.** `function main() -> int?` returning `fail(…)`
                // compiled, printed nothing and exited **0** — the shell told
                // that the program succeeded, with no diagnostic anywhere. It is
                // the only failure in this language with no instrument at all
                // (panel 035, spec-warden; panel 030 had reported the weaker half
                // of it). The refusal stands until M-ffi-ladder decides `exit(code)`, which
                // is what a fallible `main` would have to mean.
                if src.is_root(declaration.name.start)
                    && src.slice(declaration.name) == "main"
                    && result != checker.out.types.unit()
                {
                    let want = checker.show(ast, src, result);
                    let span = ast.types[function.result.0 as usize].span;
                    let diagnostic = errors::main_returns(&want, span);
                    checker.push_diagnostic(diagnostic);
                }
                // **What may cross the FFI boundary, checked here rather than
                // discovered by clang** (§4.19, M-ffi-ladder). An `extern`'s C
                // counterpart is declared by a header, so every type in its
                // signature must be one C can spell: `int`, `f64`, `bool`, `ptr`,
                // `cstr`, `str` (a `HeroStr` by value) and `()`. A `[int]` reached
                // clang as `call to undeclared function` plus `incompatible
                // integer to pointer conversion` — exit 2, the compiler blaming
                // itself for a mistake in a `.hero` file, which is the failure the
                // FFI's own guarantee exists to prevent.
                checker.out.results.insert(index as u32, result);
                checker.result = result;
                checker.fallible = is_fallible(checker, result);
                for param in &function.params {
                    let ty = lower::ty(checker, ast, resolved, param.ty);
                    checker.bind_local(param.name, ty);
                }
                // **After the parameters are lowered, never before**: the check
                // reads `written_type`, and that table is what `lower::ty` fills.
                // Asking first is silent — it finds nothing and passes — which is
                // how the `Point` parameter in `tests/golden/check/ffi-type.hero`
                // went unreported while the result beside it fired.
                if function.is_extern {
                    ffi_signature(checker, ast, src, function, result);
                }
                if let Some(body) = &function.body {
                    let (flow, _) = block(checker, ast, resolved, src, body, Want::Nothing);
                    // §4.7's flow promise. A body is checked with `Want::Nothing`
                    // because its value comes from `return` statements rather than
                    // from being a value block — so nothing here ever compared the
                    // *tail* with the declared result, and a function that runs off
                    // its end checked clean all the way through M-ir-lowering.
                    //
                    // The plan had been `-Werror=return-type` at M-scalars-run. Two facts
                    // killed it: modern clang calls it `-Wreturn-mismatch`, so the
                    // named net was not the real one, and what the author would see
                    // is a clang error about `h_module_sign` at a `#line`-mapped
                    // position in a file they did not write — design.md §8's wart 13
                    // happening to a *program* error.
                    if flow == Flow::Falls && result != checker.out.types.unit() {
                        let want = checker.show(ast, src, result);
                        let name = src.slice(declaration.name).to_string();
                        let span = ast.types[function.result.0 as usize].span;
                        checker
                            .missing_returns
                            .push(errors::missing_return(&name, &want, span));
                    }
                }
            }
            DeclKind::Test { body } => {
                checker.generic_names.clear();
                checker.result = checker.out.types.unit();
                checker.fallible = false;
                block(checker, ast, resolved, src, body, Want::Nothing);
            }
            // No body, and no `Want`: only the field types, interned so that
            // `counted.rs` and the descriptor pass can read them whether or not the
            // program touches the field.
            DeclKind::Record { fields } => {
                checker.generic_names.clear();
                for field in fields {
                    let _ = lower::ty(checker, ast, resolved, field.ty);
                }
            }
            DeclKind::Variant { cases } => {
                checker.generic_names.clear();
                for case in cases {
                    for field in &case.fields {
                        let _ = lower::ty(checker, ast, resolved, field.ty);
                    }
                }
            }
        }
    }
}



fn is_fallible(checker: &Checker, ty: TyId) -> bool {
    matches!(checker.out.types.get(ty), Ty::Fallible(_))
}

/// Every type in an `extern`'s signature must be one a C header can declare
/// (§4.19). The list is `ctype.rs`'s scalars plus §4.19's two opaque types, plus
/// `()` for a function that returns nothing — and `str`, which is a `HeroStr` by
/// value and reaches C only from a function that builds one (§4.20).
///
/// **The refusal is the loud direction** (CLAUDE.md §11). A container or a record
/// in an `extern` has no header counterpart at all, so leaving it to clang costs
/// an internal error naming generated C; refusing it costs one message naming the
/// parameter.
fn ffi_signature(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    function: &crate::syntax::Function,
    result: TyId,
) {
    // Collected first, then checked: a closure that borrows the checker mutably
    // cannot also read it (and CLAUDE.md §5 keeps stored closures out anyway).
    let mut wanted: Vec<(TyId, crate::source::Span, &str)> =
        vec![(result, ast.types[function.result.0 as usize].span, "an `extern`'s result")];
    for param in &function.params {
        if let Some(declared) = checker.out.written_type(param.ty) {
            wanted.push((declared, ast.types[param.ty.0 as usize].span, "an `extern`'s parameter"));
        }
    }
    for (ty, span, what) in wanted {
        if crosses_the_boundary(checker, ty) {
            continue;
        }
        let name = checker.show(ast, src, ty);
        let diagnostic = errors::ffi_type(&name, what, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// The seven types a C header can spell.
fn crosses_the_boundary(checker: &Checker, ty: TyId) -> bool {
    matches!(
        checker.out.types.get(ty),
        Ty::Int | Ty::F64 | Ty::Bool | Ty::Str | Ty::Ptr | Ty::Cstr | Ty::Unit | Ty::Error
    )
}
