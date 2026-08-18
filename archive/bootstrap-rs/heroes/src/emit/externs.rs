//! What a `extern` group puts in the translation unit (design.md §4.19, panels
//! 036 and 038).
//!
//! Split out of `decls.rs` on 2026-08-12 by author decision, at 768 lines against
//! CLAUDE.md §11's ~300 — and predicted there: panel 038's compiler-engineer said
//! the file would end over 750 unsplit, and it ended at 768. It is the same seam
//! `syntax/externs.rs` already cut on the parser's side, so the two halves of one
//! feature are now named the same thing in the two passes that own it.
//!
//! Four walks over the same declarations, and they are here together because each
//! one answers a question the group's head line asked: which **headers** the unit
//! includes, which **libraries** the link line names, what the header **says** a
//! name is, and the one assertion clang cannot make on its own.
//!
//! **The walks ask the declaration, never `FnKind`** — see `extern_spans`. That is
//! the premise M-header-constants falsified: an `extern constant` lowers to
//! `FnKind::Constant`, so a filter written as `kind == Extern` drops its
//! `#include` and clang then blames the compiler for a name it was never given.

use crate::ir::{Function, Program};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};


/// Every header a group named, without its quotes, deduplicated and in
/// declaration order — which is deterministic, so the double-emit test holds.
pub(super) fn headers(program: &Program, ast: &Ast, src: &Source) -> Vec<String> {
    // Two seeds, both unconditional. `math.h` for `HUGE_VAL`, which is how an
    // infinite `f64` literal is spelled; `hero_os.h` because the generated `main`
    // calls `hero_args_set` whether or not the program ever asks for `args()`.
    // Seeded here rather than printed above so that a program which also *binds*
    // one of them does not include it twice.
    let mut seen: Vec<String> = vec!["math.h".to_string(), "hero_os.h".to_string()];
    for function in &program.functions {
        let Some(header) = extern_header(ast, src, function) else { continue };
        if !seen.contains(&header) {
            seen.push(header);
        }
    }
    // **And every group that declares a `record`, whether or not it declares a
    // function** (panel 061). This walk was over `program.functions` alone, which
    // was true for as long as a group's only members were functions and constants —
    // and `record` became the third on 2026-08-15. A group of records only then
    // produced no `#include`, so its own field assertions referenced a type nothing
    // had declared: `use of undeclared identifier 'Color'`, exit 2, on a program
    // whose every line is correct.
    //
    // Walked over the **declarations** rather than over the IR, because that is
    // where the answer is: a `record` lowers to no function, so no walk of the
    // emitted program can see one. The premise that died here was not written down
    // anywhere — it was the shape of the loop (CLAUDE.md §11).
    for decl in &ast.decls {
        let DeclKind::Record { header: Some(header), .. } = &decl.kind else { continue };
        let header = src.slice(*header).trim_matches('"').to_string();
        if !seen.contains(&header) {
            seen.push(header);
        }
    }
    seen
}

/// Every library a group named directly, deduplicated and in declaration order.
/// Reached from `Emitted`, so the one walk that knows about `link` lives beside
/// the one that knows about `header`.
pub(super) fn libraries(program: &Program, ast: &Ast, src: &Source) -> Vec<String> {
    named(program, ast, src, false)
}

/// Every **package** a group asked for, same order and same deduplication.
///
/// Kept apart from `libraries` all the way to the toolchain rather than merged
/// here, because the two are answered by different things: a library name is
/// handed to the linker as written, and a package name is a *question* whose
/// answer this program does not know and must validate when it arrives.
pub(super) fn packages(program: &Program, ast: &Ast, src: &Source) -> Vec<String> {
    named(program, ast, src, true)
}

fn named(program: &Program, ast: &Ast, src: &Source, want_package: bool) -> Vec<String> {
    let mut seen: Vec<String> = Vec::new();
    for function in &program.functions {
        let (_, library) = extern_spans(ast, function);
        let span = match library {
            Some(crate::syntax::Library::Package(span)) if want_package => span,
            Some(crate::syntax::Library::Link(span)) if !want_package => span,
            _ => continue,
        };
        let name = src.slice(span).trim_matches('"').to_string();
        if !seen.contains(&name) {
            seen.push(name);
        }
    }
    // The `record`-only group again (panel 061), one level further on: `headers`
    // gained its arm and this did not, so the `#include` was emitted and the `-I`
    // that finds the header was not — `fatal error: 'raylib.h' file not found`.
    // One defect, two walks, and the second was invisible until the first was
    // fixed.
    for decl in &ast.decls {
        let DeclKind::Record { library, .. } = &decl.kind else { continue };
        let span = match library {
            Some(crate::syntax::Library::Package(span)) if want_package => *span,
            Some(crate::syntax::Library::Link(span)) if !want_package => *span,
            _ => continue,
        };
        let name = src.slice(span).trim_matches('"').to_string();
        if !seen.contains(&name) {
            seen.push(name);
        }
    }
    seen
}

/// The header a signature was declared under, quotes stripped.
fn extern_header(ast: &Ast, src: &Source, function: &Function) -> Option<String> {
    let (header, _) = extern_spans(ast, function);
    Some(src.slice(header?).trim_matches('"').to_string())
}

/// The header and library spans a declaration carries, whatever kind it is
/// (§4.19, panel 038).
///
/// **The walks below ask this, never `FnKind`.** An `extern function` lowers to
/// `FnKind::Extern` and an `extern constant` to `FnKind::Constant`, so a filter
/// written as `kind == Extern` encodes a premise — *"only a function comes from a
/// header"* — that this milestone falsifies, and its failure is silent: the
/// `#include` disappears and clang blames the compiler for a name it was never
/// given (CLAUDE.md §11). Asking the declaration is a fact about the value.
pub(super) fn extern_spans(ast: &Ast, function: &Function) -> (Option<crate::source::Span>, Option<crate::syntax::Library>) {
    match &ast.decls[function.decl as usize].kind {
        crate::syntax::DeclKind::Function(declared) => (declared.header, declared.library),
        crate::syntax::DeclKind::Constant { header, library, .. } => (*header, *library),
        _ => (None, None),
    }
}

/// A `constant` whose value a header holds (§4.19, panel 038) — as opposed to an
/// `extern function`, which is called, or an ordinary `constant`, which has a body.
pub(super) fn is_extern_constant(ast: &Ast, function: &Function) -> bool {
    matches!(
        &ast.decls[function.decl as usize].kind,
        crate::syntax::DeclKind::Constant { body: None, header: Some(_), .. }
    )
}
