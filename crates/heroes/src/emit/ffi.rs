//! Turning clang's verdict on an `extern` back into a Heroes diagnostic
//! (design.md §4.19, panel 036 rider 3).
//!
//! **This is the one place a clang failure is not the compiler's fault.**
//! CLAUDE.md §7 says a clang failure exits 2 and says the *compiler* is wrong,
//! and that was true while every line of the translation unit came from this
//! emitter. The return-type assertion breaks that: it is generated code whose
//! whole purpose is to fail when the **author's** declaration disagrees with the
//! real header. Left as an internal error it would print C the author never
//! wrote, name a file under `build/<hash>/`, and blame the compiler for a
//! mistake in a `.hero` file — three of the four things §4.17 exists to prevent.
//!
//! **The mapping is deliberately narrow, and the narrowing is `declaration()`.**
//! Every class here recovers a name from the tool's output and then asks *this
//! program* whether it declared that name in an `extern` group. A name no group
//! declares is refused, and clang's line stays what it was: a statement about
//! this compiler.
//!
//! The doc used to say the narrowing was "only the assertion messages this
//! emitter itself writes, matched by their exact prefix". That was **already
//! false when it was written** — `unknown_name` matches clang's own *"call to
//! undeclared function '"* and always did — and the sentence went on reading as
//! correct because the argument around it was still valid (CLAUDE.md §11's class,
//! found by panel 048's compiler-engineer). The rule that actually holds is the
//! stronger one: a fact about the value, not about whose text it is.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::ffi_build as build;
use super::ffi_declared as declared;
use super::ffi_narrowed as narrowed;

/// The prefix every return-type assertion's message carries, and the contract
/// between `decls::extern_assertions` and this file. One string, two readers.
pub const ASSERTION: &str = "heroes-ffi-return ";

/// The same contract for the constancy assertion an `extern constant` carries
/// (§4.19, panel 038). A separate marker because the repair is a different one:
/// the type was right and the header does not hold a *value*.
pub const CONSTANCY: &str = "heroes-ffi-const ";

/// The marker the driver puts on a verdict about a **package** — one this
/// compiler wrote itself, unlike the clang and linker lines the other classes
/// read. It is here rather than in the driver so that the one file that decides
/// *whose mistake this is* holds every marker it matches.
pub const PACKAGE: &str = "heroes-ffi-package ";

/// Every `extern` a clang failure blames the author for, as diagnostics pointing
/// at the `.hero` line rather than at generated C. Two classes, both narrow:
/// a declared result type the header refutes, and a name the header does not
/// have.
///
/// Empty when clang failed for any other reason — which is the common case and
/// still means the compiler is wrong.
pub fn explain(
    stderr: &str,
    ast: &Ast,
    checked: &crate::types::Checked,
    program: &crate::ir::Program,
    src: &Source,
) -> Vec<Diagnostic> {
    let mut found: Vec<Diagnostic> = Vec::new();
    for line in stderr.lines() {
        if let Some(diagnostic) = declared::wrong_type(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = declared::not_constant(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = declared::unknown_name(line, stderr, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = build::missing_link(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = build::missing_library(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = build::missing_header(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = build::package_problem(stderr, line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = narrowed::parameter_width(line, ast, checked, program, src) {
            push(&mut found, diagnostic);
        }
    }
    found
}

/// One diagnostic per `.hero` line: clang reports a translation unit, and one
/// mistake in it can produce several lines that all mean the same thing.
pub(super) fn push(found: &mut Vec<Diagnostic>, diagnostic: Diagnostic) {
    if !found.iter().any(|d: &Diagnostic| d.span == diagnostic.span) {
        found.push(diagnostic);
    }
}

/// The `extern` declaration with this C name, the header it was declared under,
/// and whether it is a `constant`. `None` for a name no group declares, which is
/// how a message that only looks like one of ours is refused.
pub(super) fn declaration(ast: &Ast, src: &Source, name: &str) -> Option<(crate::source::Span, String, bool)> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != name {
            return None;
        }
        let (header, is_constant) = match &decl.kind {
            DeclKind::Function(function) => (function.header?, false),
            DeclKind::Constant { header, .. } => ((*header)?, true),
            _ => return None,
        };
        Some((decl.span, src.slice(header).trim_matches('"').to_string(), is_constant))
    })
}

/// The span of the *name* alone, which is what a fix replaces — the declaration's
/// own span covers the whole signature.
pub(super) fn name_span(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
    ast.decls
        .iter()
        .find(|decl| src.slice(decl.name) == name)
        .map(|decl| decl.name)
}

/// The span of the first declaration whose group names this header — which is
/// the `extern` line the author must look at.
///
/// The same gate as everywhere else in this file: a header no group in *this*
/// program names is not this program's business, and clang's line stays a
/// statement about the compiler.
pub(super) fn group_head(ast: &Ast, src: &Source, header: &str) -> Option<crate::source::Span> {
    ast.decls.iter().find_map(|decl| {
        let named = match &decl.kind {
            DeclKind::Function(function) => function.header?,
            DeclKind::Constant { header, .. } => (*header)?,
            _ => return None,
        };
        (src.slice(named).trim_matches('"') == header).then_some(decl.span)
    })
}

/// The span of the first declaration whose group asked about this package — the
/// same gate as everywhere else in this file, and the reason a stray line
/// mentioning a package name cannot become a diagnostic about a program that
/// never asked for it.
pub(super) fn package_span(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
    ast.decls.iter().find_map(|decl| {
        let library = match &decl.kind {
            DeclKind::Function(function) => function.library?,
            DeclKind::Constant { library, .. } => (*library)?,
            _ => return None,
        };
        match library {
            crate::syntax::Library::Package(span) if src.slice(span).trim_matches('"') == name => {
                Some(decl.span)
            }
            _ => None,
        }
    })
}
