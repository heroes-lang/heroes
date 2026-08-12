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
//! | `inst.rs`   | which operation an instruction is, and where its result goes |
//! | `ops.rs`    | what one operation is: a literal, an operator, a call, `print` |
//! | `term.rs`   | how a block ends |
//! | `types.rs`  | what a `record` and a `variant` look like in C |
//! | `perfn.rs`  | the per-type functions C cannot write for itself |
//! | `descriptors.rs` | which types need a `HeroDesc`, and what to call it |
//! | `aggregate.rs` | one record: construction, a field, a place with a path |
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

mod aggregate;
mod body;
mod builtins;
mod ctype;
mod descriptors;
mod decls;
mod externs;
pub mod ffi;
mod gate;
mod inst;
mod main;
mod mangle;
mod signature;
mod ops;
mod perfn;
mod term;
mod typeorder;
mod types;
mod writer;

#[cfg(test)]
mod tests;

pub use builtins::EMITTED as EMITTED_BUILTINS;
pub use gate::subset;
pub use crate::source::module_of;

/// The `test` blocks in this program, in source order, by their titles.
///
/// `heroes test` needs the list before it runs anything: it compiles once and
/// then runs the binary once per test, so each failure is its own process and one
/// abort does not hide the tests after it.
pub fn tests_of(program: &Program) -> Vec<String> {
    program
        .functions
        .iter()
        .filter(|f| f.kind == FnKind::Test)
        .map(|f| f.name.clone())
        .collect()
}

/// What came out. Exactly one of the two is interesting: with any diagnostic the
/// C is empty, because a translation unit missing the form that was refused would
/// compile into a program that quietly does less.
pub struct Emitted {
    pub c: String,
    pub diagnostics: Vec<Diagnostic>,
    /// The libraries every `link "…"` in the file named, deduplicated and in
    /// declaration order (§4.19, panel 036).
    ///
    /// **Declared, not called**, which is the same rule design.md states for the
    /// flag itself ("the link flag is declared next to the `extern` that needs
    /// it"). The alternative — link only what a call reaches — would make the
    /// linker's arguments depend on dead-code analysis, and a program that stops
    /// calling one function of a library would stop linking it.
    pub link: Vec<String>,
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
/// What the translation unit is *for*: an ordinary binary, or one that runs the
/// file's `test` blocks (§4.18).
///
/// It is one enum rather than a `bool` because the two differ in three places —
/// which functions are emitted, which `main` is written, and whether `main` is
/// written at all — and a `bool` at three call sites is three chances to read it
/// backwards.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Target {
    /// `heroes build` and `heroes run`: `test` blocks are ignored (§4.18).
    Program,
    /// `heroes test`: the `test` blocks are the program, and one is chosen by
    /// index at run time so each runs in its own process.
    Tests,
}

pub fn emit(
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Emitted {
    emit_for(Target::Program, program, ast, resolved, checked, src)
}

pub fn emit_for(
    target: Target,
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Emitted {
    let refused = gate::refuse(target, program, ast, resolved, checked, src);
    if !refused.is_empty() {
        return Emitted { c: String::new(), diagnostics: refused, link: Vec::new() };
    }
    // The ROOT module, and it names exactly two things now: the generated `.c`
    // in a `#line` directive, and the program-wide types the emitter invents
    // (option structs, function-pointer typedefs), which belong to no
    // declaration. Every name that comes from a declaration takes that
    // declaration's own module instead — M-module-namespace's whole change to this file.
    let module = crate::source::module_of(&src.name);
    // The author's functions, plus exactly the library functions they reach —
    // computed **before** the type walk, because the types a pruned function
    // mentions are not reachable either. Until M-ffi-ladder no library function
    // mentioned a type the program did not already have; `read_file -> str?` and
    // `write_file -> ()?` are the first, and unfiltered they put two option
    // structs and eight per-type functions into every translation unit in the
    // language, including one that only adds two integers.
    let used = builtins::reachable(program, src);
    let shown: Vec<&crate::ir::Function> = program
        .functions
        .iter()
        .filter(|f| !src.is_library(f.span.start) || used.contains(&f.decl))
        .collect();
    let mut reachable: std::collections::BTreeSet<u32> = std::collections::BTreeSet::new();
    for function in &shown {
        if function.kind == crate::ir::FnKind::Test && target != Target::Tests {
            continue;
        }
        reachable.insert(function.result.0);
        for slot in &function.slots {
            reachable.insert(slot.ty.0);
        }
        for block in &function.blocks {
            for inst in &block.insts {
                reachable.insert(inst.ty.0);
            }
        }
    }
    // **Transitively**, because a type names the types it holds: `[int?]` is one
    // slot type and two declarations, and naming only the outer one left the
    // inner `int?` unnamed under a rule that asserts every `T?` is named before
    // anything mentions one (`ctype.rs`). Found by the mutant corpus, which is
    // where CLAUDE.md §9 says an invariant belongs.
    let mut frontier: Vec<u32> = reachable.iter().copied().collect();
    while let Some(id) = frontier.pop() {
        for inner in checked.types.contained(crate::types::TyId(id)) {
            if reachable.insert(inner.0) {
                frontier.push(inner.0);
            }
        }
    }
    let names = ctype::Names::new(ast, src)
        .with_options(&module, checked)
        .with_functions(&module, checked, program);
    let mut w = writer::Writer::new(&module);
    decls::prelude(&mut w, program, target, ast, checked, src);
    // Types before anything that can mention one: **every** typedef in one
    // containment order — declared aggregates and the ones the emitter invents
    // together, because each kind can contain the other — then every per-type
    // prototype, then the ordinary function prototypes.
    typeorder::definitions(&mut w, ast, checked, &names, src);
    perfn::prototypes(&mut w, ast, checked, &names, src);
    perfn::descriptors(&mut w, ast, checked, &names, &reachable);
    // The descriptors before any definition: an array literal names its element's
    // descriptor, so the object has to exist by the time a function body mentions it.
    // Every type the functions this build will actually emit mention — the
    // descriptor worklist's filter, so a `test` block's containers do not put
    // unused objects in an ordinary program (see `descriptors::generated`).
    for function in &shown {
        signature::prototype(&mut w, function, target, ast, checked, src, &names);
    }
    for function in &shown {
        body::definition(&mut w, program, function, target, ast, checked, &names, src);
    }
    perfn::bodies(&mut w, ast, checked, &names, src);
    match target {
        Target::Program => {
            if let Some(index) = entry_point(program) {
                main::shim(&mut w, &program.functions[index], src);
            }
        }
        Target::Tests => main::test_shim(&mut w, program, src),
    }
    Emitted { c: w.finish(), diagnostics: Vec::new(), link: externs::libraries(program, ast, src) }
}
