//! The module diagnostics (M8a, panel 031).
//!
//! Four of them exist only because **`use` binds**: naming a module twice,
//! naming one the file also declares, never reading one, and writing a module
//! where a value belongs. The other three are the qualification rules
//! themselves — and two of those are what the spec bought instead of a sentence
//! (panel 031 R5), so the wording carries the whole rule and the fix carries the
//! repair.
//!
//! The line between `Certain` and `Guess` is `errors.rs`'s, applied once more:
//! `geom.f` is certain because it repairs the line and leaves a program that
//! checks clean, and `f` from a module with no `use` is a **guess** because
//! applying it leaves a file that still needs the `use`.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;

/// One module named twice in one file. Not a shadowing error and not a
/// duplicate declaration — it is a line that does nothing, and saying which of
/// the three it is costs nothing here and a guess to the reader otherwise.
pub(crate) fn used_twice(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "used_twice",
        format!("`{name}` is already named by a `use` in this file — one `use` per module is all there is to say"),
        span,
    )
}

/// `use geom` in a file that also declares `geom`. Spec line 76's rule, reached
/// because `use` *binds* (panel 031 R3): the module name and the declaration
/// are one name meaning two things.
pub(crate) fn use_shadows_a_declaration(name: &str, decl_line: u32, span: Span) -> Diagnostic {
    Diagnostic::new(
        "shadowed_binding",
        format!(
            "`use {name}` binds `{name}`, and this file already declares `{name}` at line {decl_line} — shadowing is an error here, so one of the two has to be renamed"
        ),
        span,
    )
}

/// A `use` nothing read. Spec line 74's rule, reached the same way.
pub(crate) fn unused_use(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "unused_binding",
        format!("`use {name}` is never used — nothing in this file names anything from `{name}`"),
        span,
    )
    .with_note("delete the line: an import that is not read is a dependency that is not real".to_string())
}

/// `geom.f` where `geom` is a module this file `use`s and `f` is not in it.
pub(crate) fn not_in_module(module: &str, name: &str, near: &[String], span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "unknown_in_module",
        format!("module `{module}` declares nothing called `{name}`"),
        span,
    );
    if let [only] = near {
        diagnostic.message = format!("module `{module}` declares nothing called `{name}` — it has `{only}`");
        diagnostic.fixes.push(Fix {
            title: format!("write `{module}.{only}`"),
            replacement: only.to_string(),
            span,
            certainty: Certainty::Certain,
        });
    } else if !near.is_empty() {
        diagnostic.message =
            format!("module `{module}` declares nothing called `{name}` — it has {}", list(near));
    }
    diagnostic
}

/// The one panel 031 R5 bought instead of a spec sentence: an unqualified name
/// that exists, in a module this file already `use`s.
///
/// It carries a **`Certain`** fix, and that is the whole trade — the spec says
/// nothing about UFCS across a module boundary because the error says
/// everything, at the call site, with the repair pre-written (CLAUDE.md §8).
pub(crate) fn needs_qualifying(name: &str, module: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "needs_qualifying",
        format!(
            "`{name}` is declared in module `{module}`, and a name from another module is always written qualified — `{module}.{name}`"
        ),
        span,
    )
    .with_fix(Fix {
        title: format!("write `{module}.{name}`"),
        replacement: format!("{module}.{name}"),
        span,
        certainty: Certainty::Certain,
    })
}

/// The same, for a module the file has not named yet. The fix is **not**
/// certain: it repairs this line and leaves the file needing a `use`, and a
/// machine-applicable fix has to leave a program that checks clean.
pub(crate) fn needs_a_use(name: &str, module: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "needs_qualifying",
        format!(
            "`{name}` is declared in module `{module}`, which this file does not name — add `use {module}` at the top and write `{module}.{name}`"
        ),
        span,
    )
    .with_fix(Fix {
        title: format!("write `{module}.{name}` (and add `use {module}`)"),
        replacement: format!("{module}.{name}"),
        span,
        certainty: Certainty::Guess,
    })
}

/// A module name standing on its own. It is not a value in any position — only
/// a dot may follow it — and "unknown name" would be a lie about a name the
/// file's own `use` line put there.
pub(crate) fn module_is_not_a_value(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "module_is_not_a_value",
        format!(
            "`{name}` names a module, not a value — a module can only be followed by a dot, as in `{name}.something`"
        ),
        span,
    )
}

/// `geom.f` where `geom` is not a module this file names.
pub(crate) fn not_a_module(name: &str, known: &[String], span: Span) -> Diagnostic {
    let mut message =
        format!("`{name}` is not a module this file names, and not a value either");
    if !known.is_empty() {
        message.push_str(&format!(" — it names {}", list(known)));
    }
    Diagnostic::new("not_a_module", message, span)
}

/// `a, b and c`, which is how every list in this file's messages is written.
pub(crate) fn list(items: &[String]) -> String {
    match items {
        [] => String::new(),
        [only] => format!("`{only}`"),
        [rest @ .., last] => {
            let head: Vec<String> = rest.iter().map(|n| format!("`{n}`")).collect();
            format!("{} and `{last}`", head.join(", "))
        }
    }
}
