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
            DeclKind::Constant { ty, body, header, .. } => {
                let declared = lower::ty(checker, ast, resolved, *ty);
                checker.result = declared;
                checker.fallible = is_fallible(checker, declared);
                checker.generic_names.clear();
                match body {
                    // A constant's body *is* its value: its last statement
                    // produces it, unlike a function, which returns.
                    Some(body) => {
                        let (_, value) =
                            block(checker, ast, resolved, src, body, Want::Value(declared));
                        if value.is_none() {
                            let want = checker.show(ast, src, declared);
                            let diagnostic = errors::no_value(&want, body.span);
                            checker.push_diagnostic(diagnostic);
                        }
                    }
                    // No body: the value is the header's, so there is nothing here
                    // to check it against and the type is the declaration's alone.
                    // What *is* checkable here is whether a header could produce
                    // this type at all; the rest is clang's, per constant.
                    None if header.is_some() => {
                        super::ffi_decls::ffi_constant(checker, ast, src, declared, ast.types[ty.0 as usize].span);
                        // The result type reaches lowering through this table
                        // rather than through the body, which no longer exists.
                        checker.out.results.insert(index as u32, declared);
                    }
                    None => {}
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
                // assumed.** `function main() -> i64?` returning `fail(…)`
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
                // signature must be one C can spell: `i64`, `f64`, `bool`, `ptr`,
                // `cstr`, `str` (a `HeroStr` by value) and `()`. A `[i64]` reached
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
                    super::ffi_decls::ffi_signature(checker, ast, src, function, result, decl.name);
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
            DeclKind::Record { fields, header, .. } => {
                checker.generic_names.clear();
                for field in fields {
                    let ty = lower::ty(checker, ast, resolved, field.ty);
                    // **A group's `record` is the header's struct, so its fields
                    // are what a header can hold** (§4.19, panel 060). Refused
                    // here rather than left to the field assertion, because a
                    // `str` field has no C spelling at all: the assertion could
                    // not be written, so the failure would arrive as a clang error
                    // about generated C — exit 2, the compiler blaming itself.
                    if header.is_some() {
                        super::ffi_decls::ffi_field(checker, ast, src, ty, ast.types[field.ty.0 as usize].span);
                    } else {
                        // **A fixed array exists at the C boundary and nowhere
                        // else** (panel 062; all three judges who looked at Q3
                        // reached it from different directions). It exists because
                        // a C compiler laid out a struct and put the elements
                        // inline; an ordinary `record` has no header to inherit a
                        // layout from, so `[T]` is the form and this one would be a
                        // second array type in every program for no one's benefit.
                        //
                        // Refused **here** rather than left inert: it parses and
                        // type-checks today, and a type with no constructor and no
                        // operation is worse than an error — the reader gets no
                        // signal until the emitter meets it.
                        super::ffi_decls::fixed_outside_a_group(
                            checker,
                            ast,
                            src,
                            ty,
                            ast.types[field.ty.0 as usize].span,
                        );
                    }
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
