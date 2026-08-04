//! The backend: the IR becomes one C11 translation unit (design.md §3.1, panel
//! 020).
//!
//! **A printer, plus two mechanical passes.** The passes are declaration ordering
//! (every prototype before every definition, because Heroes' top level is
//! order-free with free mutual recursion and C's is not) and the mangler. There
//! is no analysis and no optimisation: the whole optimisation pipeline is
//! clang's, which design.md calls "a welcome side effect, not a reason".
//!
//! | file | idea |
//! |------|------|
//! | `mangle.rs` | `h_<module>_<name>`, and the one thing that makes it injective |
//! | `ctype.rs`  | a `TyId` as a C type — and the unit rule, which is *no* declaration |
//! | `gate.rs`   | what this backend cannot emit yet, and what to say about it |
//! | `writer.rs` | the output, counting its own lines, because `#line` needs that |
//! | `decls.rs`  | prototypes, the prologue, the epilogue, the `main` shim |
//! | `inst.rs`   | one instruction |
//! | `term.rs`   | how a block ends |
//!
//! **What the emitter refuses, it refuses as a diagnostic** (`Kind::Unsupported`,
//! exit 1) rather than as a second channel. GCC's `sorry()` is the thirty-year
//! precedent and it is a *kind*, not a type; the local reason is that CLAUDE.md
//! §9's `#~ <code>` invariant keys off codes, so anything outside `Diagnostic` is
//! a test class outside the invariant.
//!
//! **The C never mentions the output path.** The restore directives name
//! `<stem>.c`, derived from the source, so `--emit-c` produces the same bytes on
//! stdout and in `-o a.c` — which is what makes the double-emit determinism diff
//! (CLAUDE.md §7) a property of the emitter rather than of the invocation.

use crate::diagnostics::Diagnostic;
use crate::ir::{FnKind, Program};
use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::Checked;

mod ctype;
mod decls;
mod gate;
mod inst;
mod mangle;
mod term;
mod writer;

#[cfg(test)]
mod tests;

pub use gate::SUBSET;
pub use mangle::module_of;

/// What came out. Exactly one of the two is interesting: with any diagnostic the
/// C is empty, because a translation unit missing the form that was refused would
/// compile into a program that quietly does less.
pub struct Emitted {
    pub c: String,
    pub diagnostics: Vec<Diagnostic>,
}

/// Where a program's `main` is, or nothing. A file with no entry point is a
/// perfectly good translation unit — it just cannot become a binary, and the
/// difference belongs to the caller (`--emit-c` emits it; `build` refuses it with
/// `no_entry_point` rather than letting `ld` say `undefined symbol _main`).
pub fn entry_point(program: &Program) -> Option<usize> {
    program
        .functions
        .iter()
        .position(|f| f.name == "main" && f.kind == FnKind::Function)
}

/// Emit one lowered program.
///
/// The gate runs first and to completion: every unsupported form is reported,
/// sorted by span, because a message that carries the list is one round trip and
/// three invocations is three (`cli.rs`'s own rule, applied to the backend).
pub fn emit(
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Emitted {
    let refused = gate::refuse(program, ast, resolved, checked, src);
    if !refused.is_empty() {
        return Emitted { c: String::new(), diagnostics: refused };
    }
    let module = mangle::module_of(&src.name);
    let mut w = writer::Writer::new(&src.name, &module);
    decls::prelude(&mut w, src);
    for function in &program.functions {
        decls::prototype(&mut w, function, ast, checked, &module);
    }
    for function in &program.functions {
        decls::definition(&mut w, program, function, ast, checked, src, &module);
    }
    if let Some(index) = entry_point(program) {
        decls::shim(&mut w, &program.functions[index], &module);
    }
    Emitted { c: w.finish(), diagnostics: Vec::new() }
}
