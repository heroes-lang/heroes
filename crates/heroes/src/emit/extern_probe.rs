//! The half of §4.19's guarantee that asks what an `extern` **accepts**
//! (design.md §4.19; CLAUDE.md §7's named exception; panels 036, 042, 051, 052).
//!
//! `extern_assert.rs` asks what a header *returns*, with a `_Generic` whose
//! controlling expression C11 6.5.1.1p3 does not evaluate. That mechanism is
//! exactly why it cannot ask about parameters: **an unevaluated expression gets no
//! conversion diagnostics.** Measured — `_Generic((narrow((int64_t)0)), …)` is
//! silent under `-Wshorten-64-to-32` in both the folding and the non-folding form,
//! so the check cannot be had by turning a flag on. It has to be built.
//!
//! What is built is one **never-called function per `extern` that has a
//! parameter**, whose parameters are the *declared* types and whose body is the
//! call:
//!
//! ```c
//! #line 37 "examples/sqlite/main.hero"
//! __attribute__((unused)) static void hero_ffi_probe_sqlite3_prepare_v2(
//!     void * a0, const char * a1, int64_t a2, void * * a3, void * * a4)
//! { (void)sqlite3_prepare_v2(a0, a1, a2, a3, a4); }
//! ```
//!
//! Three properties, and each is the reason for one word of it:
//!
//! - **a parameter, not a constant.** `f((int64_t)0)` folds — the zero fits an
//!   `int` and clang says nothing. A parameter cannot be folded, so the conversion
//!   is the one the real call would make.
//! - **`#line` at the declaration.** The diagnostic then lands on the author's own
//!   `extern` line, which is where the mistake is: the *call* site is correct C in
//!   a program whose declaration is wrong. It is also what lets `ffi.rs` decide
//!   whose mistake it is from the file clang names, rather than from message text.
//! - **`__attribute__((unused)) static`.** Never called, so it costs nothing in the
//!   binary; the attribute is what keeps `-Wunused-function` quiet, and the corpus
//!   harness permits zero warnings.
//!
//! The probe is silent on a correct binding — measured on all eleven ladder
//! bindings — and it is silent on `f64`, `cstr`, `ptr` and `str`, which convert
//! exactly or not at all. It speaks only where C would narrow in silence.

use crate::ir::{Function, Program, SlotKind};
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{Checked, Ty};

use super::externs::extern_spans;
use super::writer::Writer;

/// The name a probe carries, spelled after the C function so a reader of the
/// emitted unit can see what a failure is about.
const PREFIX: &str = "hero_ffi_probe_";

/// One probe per `extern` that has at least one parameter.
///
/// **The narrowing is "has a parameter"**, which is a fact about the declaration
/// in hand: a signature with none has nothing to check, and a `constant` is not a
/// call. Nothing here asks what *kind* of extern it is — that premise is the one
/// M-header-constants falsified for the sibling walks (see `externs::extern_spans`).
pub(super) fn extern_probes(
    w: &mut Writer,
    program: &Program,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) {
    let probes: Vec<&Function> = program
        .functions
        .iter()
        .filter(|f| extern_spans(ast, f).0.is_some() && !f.params.is_empty())
        .collect();
    if probes.is_empty() {
        return;
    }
    for function in probes {
        let name = src.slice(ast.decls[function.decl as usize].name);
        let Some(parameters) = parameter_list(function, checked) else { continue };
        let arguments = argument_names(function.params.len());
        // The declaration's own file and line, so clang's verdict lands where the
        // author can act on it. `at_file` is the same entry point every module's
        // code uses, so a probe for a library's `extern` claims the library.
        let at = ast.decls[function.decl as usize].name.start;
        let (file, line, _) = src.locate(at);
        let file = file.to_string();
        w.at_file(&file, line);
        w.line(&probe_line(name, &parameters, &arguments));
    }
    w.at_generated();
    w.blank();
}

/// The probe, as one line of C. **One formatter, two readers** — the same
/// contract `ffi::ASSERTION` keeps between the emitter and the diagnostic layer,
/// and for the same reason: `emit/ffi.rs` rebuilds this string from the
/// declaration to find **which** argument clang's column points at. Rebuilding is
/// exact because the line is a pure function of the declaration; reading clang's
/// echoed source line instead is the trap panel 038 paid for.
pub(crate) fn probe_line(name: &str, parameters: &[String], arguments: &[String]) -> String {
    format!(
        "__attribute__((unused)) static void {PREFIX}{name}({}) {{ (void){name}({}); }}",
        parameters.join(", "),
        arguments.join(", ")
    )
}

/// Where each argument begins in `probe_line`'s output, in the 1-based column
/// numbering clang reports. The caret of a conversion diagnostic sits on the
/// argument token, so this is what turns a column back into a parameter index.
pub(crate) fn argument_columns(
    name: &str,
    parameters: &[String],
    arguments: &[String],
) -> Vec<usize> {
    let line = probe_line(name, parameters, arguments);
    let opens = format!("(void){name}(");
    let Some(at) = line.find(&opens) else { return Vec::new() };
    let mut at = at + opens.len();
    let mut out = Vec::new();
    for argument in arguments {
        out.push(at + 1);
        at += argument.len() + ", ".len();
    }
    out
}

/// The names a probe gives its arguments: positional, so nothing about the
/// author's own parameter names can change the generated line.
pub(crate) fn argument_names(count: usize) -> Vec<String> {
    (0..count).map(|i| format!("a{i}")).collect()
}

/// The declared parameter types as C, positionally named `a0`, `a1`, … — or
/// `None` for a type this backend has no spelling for, which the checker's
/// `ffi_type` has already refused and which therefore cannot reach a binary.
pub(crate) fn parameter_list(function: &Function, checked: &Checked) -> Option<Vec<String>> {
    function
        .params
        .iter()
        .enumerate()
        .map(|(index, slot)| {
            let declared = &function.slots[slot.0 as usize];
            let c_type = c_type_of(checked, declared.ty)?;
            // **An `@` parameter is a pointer parameter** (§4.8, CLAUDE.md §7),
            // and the probe must declare it the same way the call site passes it
            // or the probe itself would be the thing that does not compile.
            let mutable = matches!(declared.kind, SlotKind::Param { mutable: true });
            Some(if mutable {
                format!("{c_type} * a{index}")
            } else {
                format!("{c_type} a{index}")
            })
        })
        .collect()
}

/// A declared FFI parameter type as C. The set is `ffi_type`'s own — anything
/// else is refused in the checker, so this returning `None` means the emitter met
/// a type the frontend should have stopped.
fn c_type_of(checked: &Checked, ty: crate::types::TyId) -> Option<String> {
    Some(match checked.types.get(ty) {
        Ty::Int(kind) => kind.c_type().to_string(),
        Ty::F64 => "double".to_string(),
        Ty::Bool => "bool".to_string(),
        Ty::Str => "HeroStr".to_string(),
        Ty::Cstr => "const char *".to_string(),
        Ty::Ptr => "void *".to_string(),
        _ => return None,
    })
}
