//! One call as C — a Heroes function, a built-in, a function value, a C function —
//! and `print` (design.md §4.14, §4.19, §4.20; panels 006, 020, 029).
//!
//! Split from `inst.rs` when M-value-aggregates step 3 pushed that file to 450 lines, well past
//! CLAUDE.md §11's ceiling. The line between them is a real one rather than a
//! line count: `inst.rs` answers **which** operation an instruction is and where its
//! result goes, and this answers **what** one operation is in C. The dispatch is a
//! table; these are the entries.
//!
//! The §11 sweep took three concerns out of this file, each to a name of its own,
//! and what stayed is the one question the others are not about — *who* is being
//! called and under which name:
//!
//! - `literal.rs` — a constant value as C text.
//! - `operator.rs` — an operator, and the guard that keeps it out of UB.
//! - `convert.rs` — `to_<width>`, the built-in that builds its own `T?` inline.
//!
//! One rule lives here and it has a measured reason: **`print` is monomorphic**,
//! one runtime entry point per type (panel 006), which is what lets clang
//! type-check every one of them.

use crate::ir::{Arg, Program, SlotKind};
use crate::resolve::BUILTINS;
use crate::types::{Checked, IntKind, Ty};

use super::aggregate;
use super::mangle;
use super::writer::Writer;

#[allow(clippy::too_many_arguments)] // PORT-DEBT: one emitter call site, six facts
pub(super) fn call(
    w: &mut Writer,
    program: &Program,
    function: &crate::ir::Function,
    types: &aggregate::Types,
    callee: crate::ir::Callee,
    args: crate::ir::Args,
    arguments: &[String],
    target: Option<String>,
    instance: Vec<crate::types::TyId>,
) {
    let (checked, ast, src) = (types.checked, types.ast, types.src);
    use crate::ir::Callee;
    let assign = match &target {
        Some(name) => format!("{name} = "),
        None => String::new(),
    };
    match callee {
        Callee::Heroes(decl) => {
            // **The instance, not the template.** Monomorphisation deleted the
            // generic function and left one copy per type tuple, so a call has to
            // name the copy — and it reads which one from the same table the pass
            // read (`Checked::instantiations`, keyed by this call's span). One
            // answer, one place: re-deriving it here is the shape panel 029 R2
            // refused.
            let name = program
                .functions
                .iter()
                .find(|f| f.decl == decl && f.instance == instance)
                .map(|f| super::signature::instance_name(f, ast, checked, src))
                .unwrap_or_else(|| "hero_unreachable".to_string());
            w.line(&format!("    {assign}{name}({});", arguments.join(", ")));
        }
        Callee::Builtin(index) if BUILTINS[index as usize].name == "print" => {
            print(w, function, checked, args, arguments);
        }
        // `to_<width>` is not a call, which is why `convert.rs` writes it inline and
        // `builtins::entry` does not name it: that function answers with the NAME of
        // a C entry point, and a conversion has to build a `T?` — a tagged union
        // generated for this result type, which no runtime function can return.
        Callee::Builtin(index)
            if BUILTINS[index as usize].name.starts_with("to_")
                && crate::types::INT_KINDS
                    .iter()
                    .any(|k| k.name() == &BUILTINS[index as usize].name[3..])
                && target.is_some() =>
        {
            let into = target.expect("just matched");
            super::convert::width(
                w,
                types,
                function,
                BUILTINS[index as usize].name,
                args,
                arguments,
                &into,
            );
        }
        Callee::Builtin(index) if super::EMITTED_BUILTINS.contains(&BUILTINS[index as usize].name) => {
            let entry =
                super::builtins::entry(BUILTINS[index as usize].name, function, checked, args);
            // `push`'s second argument is a *place*, not a value: the runtime copies
            // through the element descriptor, which is the only way one function can
            // append an `i64` and a `Point`.
            let written: Vec<String> = if matches!(BUILTINS[index as usize].name, "push") {
                arguments
                    .iter()
                    .enumerate()
                    .map(|(at, text)| if at == 1 { format!("&{text}") } else { text.clone() })
                    .collect()
            } else {
                arguments.to_vec()
            };
            w.line(&format!("    {assign}{entry}({});", written.join(", ")));
        }
        // A call through a function value. The temporary already holds the
        // pointer, so this is just C's own indirect call — no dereference, no
        // cast, and clang type-checks the arguments against the typedef's
        // parameter list, which is the property `(void)` on a zero-parameter
        // typedef exists to keep (see `types::functions`).
        Callee::Indirect(value) => {
            w.line(&format!("    {assign}{}({});", mangle::value(value.0), arguments.join(", ")));
        }
        // **Unmangled, by design** (CLAUDE.md §7). The name in the `.hero` file is
        // the C function's own name, and the `#include` in the prelude is what
        // declares it — so this call is checked against the real header rather
        // than against a prototype this compiler invented. That is the whole of
        // §4.19's guarantee on the argument side, and `decls::extern_assertions`
        // is the other half, on the return side.
        Callee::Extern(decl) => {
            let name = src.slice(ast.decls[decl as usize].name);
            let guarded = guard_cstr_arguments(program, decl, checked, arguments);
            w.line(&format!("    {assign}{name}({});", guarded.join(", ")));
        }
        // Refused by the gate. The arm exists so that adding a callee kind to the
        // IR breaks this file.
        Callee::Builtin(_) => {
            w.line("    hero_unreachable(); /* the gate refuses this callee */");
        }
    }
}

/// `print` is a compiler form, not a function value (§4.20, panel 006): the runtime
/// exposes one monomorphic printer per type and the emitter composes them.
///
/// The contract is **no separator, exactly one trailing newline** — Pascal's
/// `WriteLn`, which the ISO standard defines as `write(f,e1); write(f,e2,…)` with
/// nothing inserted between. `hero_print_end` owns the newline rather than the
/// emitter, and that is measured rather than chosen: the emitter owning it needs
/// `putchar`, which is `error: call to undeclared function 'putchar'` without
/// `#include <stdio.h>` in every generated unit — a header-collision surface on
/// every FFI program, against CLAUDE.md §7.
pub(super) fn print(
    w: &mut Writer,
    function: &crate::ir::Function,
    checked: &Checked,
    args: crate::ir::Args,
    arguments: &[String],
) {
    // The order is the IR's, which is the source's. The printer per argument comes
    // from the *value's* type, because `print` has no signature to check against —
    // it is the one call clang verifies nothing about (§4.19's variadic note).
    for (arg, name) in function.args_of(args).into_iter().zip(arguments) {
        let printer = match arg {
            Arg::Value(value) => match checked.types.get(function.value_type(value)) {
                Ty::Bool => "hero_print_bool",
                Ty::Str => "hero_print_str",
                Ty::F64 => "hero_print_f64",
                // **`u64` is the one width that needs its own printer.** The
                // other seven widen into an `int64_t` without losing a value;
                // 18446744073709551615 does not, and read as signed it is `-1` —
                // which is precisely what `print(SIZE_MAX)` produced when panel
                // 042 measured it, and what this arm exists to stop.
                Ty::Int(IntKind::U64) => "hero_print_uint",
                _ => "hero_print_int",
            },
            // `print(@x)` cannot be written: §4.8's marker is for parameters
            // declared `@`, and `print` declares none.
            Arg::InOut(_) => "hero_print_int",
        };
        w.line(&format!("    {printer}({name});"));
    }
    w.line("    hero_print_end();");
}

/// Every `cstr` argument of an `extern` call, wrapped in the runtime's null check
/// (panel 053; CLAUDE.md §12's robustness rule, §7's own precedent).
///
/// **This is not a language change and it is not a type rule.** It is a backend
/// obligation of exactly the class §7 already states: *"Arithmetic aborts via
/// `__builtin_*_overflow` — never C UB — and `%` is guarded like `/`."*
/// `hero_cstr_nonnull` is to a null `cstr` what `__builtin_mul_overflow` is to
/// signed overflow — one branch, a named abort, and C's undefined behaviour never
/// reached.
///
/// The hole it closes was measured before it existed. `to_str` on a null `cstr`
/// was already safe: `runtime/parts/str.c` guards it and panics with its own name.
/// What nothing guarded was a null `cstr` handed **straight back to C** —
/// `strstr(haystack: getenv(UNSET), needle: "y")` — where `to_str` is never
/// called and the pointer reaches libsystem unexamined:
/// `AddressSanitizer: SEGV on unknown address 0x0`. Three of panel 053's four
/// options were structurally blind to that path.
///
/// **The narrowing is the callee's declared parameter type**, which is a fact
/// about this declaration (CLAUDE.md §11). It is deliberately *not* provenance —
/// *"this value came from an extern"* — because that is a premise about where a
/// value has been rather than about the value in hand, and this compiler has no
/// flow-sensitive narrowing to rest one on. The cost of asking the cheaper
/// question is that a `cstr` from `s.cstr()`, which cannot be null, is checked
/// too. One predictable branch, against §12's rule that safety outranks speed.
///
/// Panel 052's standing veto is untouched: nothing here re-declares a signature.
/// Only the argument expression changes, so the call still goes through the
/// header's own prototype — variadics included.
fn guard_cstr_arguments(
    program: &Program,
    decl: u32,
    checked: &crate::types::Checked,
    arguments: &[String],
) -> Vec<String> {
    let Some(callee) = program.functions.iter().find(|f| f.decl == decl) else {
        return arguments.to_vec();
    };
    arguments
        .iter()
        .enumerate()
        .map(|(index, argument)| {
            // A variadic's extra arguments have no declared parameter, and a
            // parameter whose type is not `cstr` has nothing to check.
            //
            // **An `@cstr` is excluded, and the guard was vacuous on one** (author
            // decision 2026-08-15, found by panel 058's compiler-engineer while
            // measuring something else). §4.8 makes an `@` parameter a *pointer*
            // parameter, so the argument this emitter writes is `&h0_tail` — the
            // address of a local, which is **never null**. The guard fired, cost a
            // call, and proved nothing; worse, a guard that cannot fail is read by
            // the next person as protection that is there.
            //
            // It is CLAUDE.md §11 in miniature: the narrowing asked the declared
            // *type* and the fact it needed was about the *slot*. What panel 053
            // guards is a `cstr` **value** crossing into C, and an `@cstr` does not
            // carry one — it carries somewhere for C to put one.
            let declared = callee.params.get(index).map(|slot| &callee.slots[slot.0 as usize]);
            let is_out_parameter =
                matches!(declared.map(|slot| slot.kind), Some(SlotKind::Param { mutable: true }));
            let is_cstr = matches!(
                declared.map(|slot| checked.types.get(slot.ty)),
                Some(crate::types::Ty::Cstr)
            );
            if is_cstr && !is_out_parameter {
                format!("hero_cstr_nonnull({argument})")
            } else {
                argument.clone()
            }
        })
        .collect()
}
