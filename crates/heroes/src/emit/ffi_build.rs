//! The classes where **the machine does not have what the group named** (design.md
//! §4.19; panels 048, 049, 050).
//!
//! Split from `ffi.rs` by the §11 sweep. These three are not about the author's
//! types at all — the declaration may be perfect and the header still absent, the
//! symbol still unresolved, the package still unknown to `pkg-config`. Two of them
//! read a **linker**'s output rather than a compiler's, and one reads this
//! compiler's own `PACKAGE` marker, so the file that groups them is the one that
//! knows a build has more tools in it than clang.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::Ast;

use super::ffi::{PACKAGE, declaration, group_head, package_span};

/// **A symbol the linker could not find, which this program declared `extern`**
/// — the author forgot `link`, and until panel 048 the compiler answered
/// `internal error: …` and **exit 2**, which by CLAUDE.md §10's contract says
/// *the tool could not run*.
///
/// It was the same shape §4.19's return assertion had before panel 036, and this
/// file's own header already carried the verdict on it: left as an internal
/// error it prints C the author never wrote, names a file under `build/<hash>/`,
/// and blames the compiler for a mistake in a `.hero` file.
///
/// **The gate is `declaration()` and it is doing real work here.** An undefined
/// symbol that no `extern` group declares is a symbol the *emitter* failed to
/// define — a generated `hash` prototyped and never written is a recorded
/// instance — and that stays exit 2, where it belongs. So the split is a fact
/// about the value: did *this program* promise this name to C?
///
/// **No fix is offered, and that is deliberate.** The repair is `link "<name>"`,
/// and the compiler does not know the name: the header does not carry it, and a
/// table mapping `math.h` to `m` would be a premise about the world with an entry
/// for every library anyone ever binds. The note names the form and the group;
/// the author knows the library. (Panel 048 declined a spec clause for the same
/// reason, on four judges' evidence.)
pub(super) fn missing_link(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let name = undefined_symbol(line)?;
    let (span, header, _) = declaration(ast, src, &name)?;
    let mut diagnostic = Diagnostic::new(
        "ffi_missing_link",
        format!(
            "the linker cannot find `{name}`, and no group in this program says which library has it"
        ),
        span,
    );
    diagnostic = diagnostic.with_note(format!(
        "`{header}` declares `{name}`, and a header is not a library: write `extern \"{header}\" link \"<library>\"` — `math.h` needs `link \"m\"` on Linux and the BSDs, where it is a separate library"
    ));
    Some(diagnostic)
}

/// A library the group **did** name and the linker cannot find (panel 055).
///
/// `ld: library 'sdl2' not found` was `internal error:` at **exit 2** — the
/// compiler blaming itself for a library missing from the author's machine, which
/// is exactly what panel 049 repaired on the header side and what panel 048
/// repaired for a library nobody named. This is the third face of one mistake and
/// the last of them: the group is right, the machine is short.
///
/// **The narrowing is `link`'s own list**, which is the analogue of
/// `declaration()`: a library name this program wrote is the author's business,
/// and one it did not write is the driver's own `-l` and stays exit 2. Two of
/// panel 055's judges found this independently while measuring something else.
pub(super) fn missing_library(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    // `ld: library 'X' not found` (ld64) and `cannot find -lX` (GNU ld / lld).
    let name = if let Some(rest) = line.split("library '").nth(1) {
        rest.split('\'').next()?.to_string()
    } else if let Some(rest) = line.split("cannot find -l").nth(1) {
        rest.split_whitespace().next()?.trim_matches(':').to_string()
    } else {
        return None;
    };
    let span = link_head(ast, src, &name)?;
    Some(
        Diagnostic::new(
            "ffi_missing_library",
            format!(
                "this machine has no library called `{name}` — the group names it, and the linker looked"
            ),
            span,
        )
        .with_note(format!(
            "install its development files, or pass `--library <dir>`; where the library ships a `.pc`, `package \"{name}\"` asks the machine for both its headers and its libraries"
        )),
    )
}

/// The group head that wrote `link "<name>"` — the narrowing that keeps a
/// library the *driver* passed from being blamed on the author.
fn link_head(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
    ast.decls.iter().find_map(|decl| {
        let library = match &decl.kind {
            crate::syntax::DeclKind::Function(function) => function.library,
            crate::syntax::DeclKind::Constant { library, .. } => *library,
            _ => None,
        }?;
        let crate::syntax::Library::Link(span) = library else { return None };
        (src.slice(span).trim_matches('"') == name).then_some(span)
    })
}

/// The symbol out of a linker's complaint, in the spellings the two platforms
/// produce.
///
/// GNU `ld` and `lld` write ``undefined reference to `sqrt'``; Apple's `ld64` and
/// `ld_prime` write `"_sqrt", referenced from:` on its own line. **The leading
/// underscore is Mach-O's**, not the program's, and stripping it is what lets one
/// `declaration()` lookup serve both.
fn undefined_symbol(line: &str) -> Option<String> {
    if let Some(rest) = line.split("undefined reference to `").nth(1) {
        return Some(rest.split('\'').next()?.to_string());
    }
    if line.contains("referenced from:") {
        let quoted = line.split('"').nth(1)?;
        return Some(quoted.strip_prefix('_').unwrap_or(quoted).to_string());
    }
    None
}

/// **A header the preprocessor could not find**, named by an `extern` group in
/// this program — the fifth class, and the one that stops §4.19's ladder one
/// directory before the linker.
///
/// Measured at panel 049: `heroes build` on a real raylib binding died with
/// `internal error: … fatal error: 'raylib.h' file not found`, **exit 2**, for a
/// missing `-I` on the author's own machine. design.md:2058 had recorded the
/// diagnosis a milestone earlier — *"what the group head lacks is **search
/// paths**, not a framework keyword"* — and three judges re-measured it from
/// three directions before anyone read that line.
///
/// The class does not repair the gap; the language still has no way to *say*
/// where a header is, and what that clause should look like is queued. What it
/// repairs is the blame: a header this program named and this machine does not
/// have is the author's problem to solve, not evidence that the compiler is
/// broken.
pub(super) fn missing_header(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = line.split("file not found").next()?;
    let header = quoted.split('\'').nth(1)?;
    let span = group_head(ast, src, header)?;
    let mut diagnostic = Diagnostic::new(
        "ffi_missing_header",
        format!("`{header}` is not on this machine's include path — clang looked and did not find it"),
        span,
    );
    diagnostic = diagnostic.with_note(
        "the group names a header the preprocessor must be able to open: install the library's development files, or pass `--include <dir>` — `package` finds it for you where the library ships a `.pc`, and `CPATH` works too".to_string(),
    );
    Some(diagnostic)
}

/// **A package this program asked about, and the machine could not answer** —
/// not installed, or answering with a flag the allow-list refuses.
///
/// The sixth class, and the only one whose text this compiler wrote itself: the
/// driver marks it, and the marker is matched here so that one file holds every
/// rule about whose mistake a build failure is. The verdict is still the
/// author's — a package absent from *this machine* is a thing they install, and
/// a `.pc` answering with `-fplugin=` is their environment — so it is exit 1 on
/// the line that named the package, not exit 2 on the compiler.
pub(super) fn package_problem(stderr: &str, line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let rest = line.split(PACKAGE).nth(1)?;
    let name = rest.split('`').nth(1)?;
    let span = package_span(ast, src, name)?;
    // The driver's message is already written for a reader; the whole of it is
    // carried, minus the marker, because its second and third lines are the
    // repair and §4.17 says an error carries what is needed to fix it.
    let whole = stderr
        .lines()
        .skip_while(|l| !l.contains(PACKAGE))
        .collect::<Vec<&str>>()
        .join("\n");
    let headline = whole.lines().next().unwrap_or(rest).replace(PACKAGE, "the package ");
    let mut diagnostic = Diagnostic::new("ffi_package", headline, span);
    for note in whole.lines().skip(1).map(str::trim).filter(|l| !l.is_empty()) {
        diagnostic = diagnostic.with_note(note.to_string());
    }
    Some(diagnostic)
}
