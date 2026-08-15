//! The class where **C would write through a `cstr`** (design.md §1.12, §4.19;
//! panel 058, ratified 2026-08-15).
//!
//! `.cstr()` is zero-copy (§4.20): it hands C the interior pointer of a
//! **refcounted, copy-on-write** buffer. So a C function whose parameter is a
//! plain `char *` writes into values the program never handed it — measured, `b =
//! a` then `strtok(a.cstr(), …)` changes both, at exit 0 with no diagnostic. A
//! literal's `.cstr()` is read-only memory and the same write is `SIGBUS`. Both
//! are design.md §1.12's class: a Heroes program must not corrupt memory and must
//! not segfault.
//!
//! **The refusal deletes a spelling, not a function**, and that is what made it
//! admissible under §1.12's *completeness* half. 186 plain `char *` parameters
//! across 17 headers, **159 of them (85%) genuinely write**; the correct spelling —
//! `ptr` with a buffer C owns — compiles today, measured on `getcwd`, `strtok`,
//! `putenv` and ncurses `tigetnum`. The 10% that do not write pay a copy, and that
//! cost is recorded with its falsifier in `tests/golden/unsupported/`.
//!
//! Sibling of `ffi_narrowed.rs` and gated the same way: the message clang writes
//! carries no C name, so the question is asked of the **location** — is an `extern`
//! declared at exactly this file and line? That is panel 048's `declaration()`
//! narrowing reaching this class through `extern_at_line`.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::Ast;

use super::extern_probe;
use super::ffi_narrowed::{extern_at_line, location};

/// What clang calls handing a `const char *` to a `char *`. Matched whole.
const DISCARDS: &str = "discards qualifiers";

/// A parameter the header declares **writable**, which `.cstr()` cannot supply.
pub(super) fn writable_parameter(
    line: &str,
    ast: &Ast,
    checked: &crate::types::Checked,
    program: &crate::ir::Program,
    names: &super::typedefs::Names,
    src: &Source,
) -> Option<Diagnostic> {
    if !line.contains(DISCARDS) {
        return None;
    }
    let (at, rest) = location(line, src)?;
    // `'const char *' to parameter of type 'char *'` — the header's type is the
    // last quoted name, the same shape `ffi_narrowed` reads.
    let c_type = rest.rsplit('\'').nth(1)?;
    let (function, name) = extern_at_line(program, ast, src, at.0, at.1)?;
    let parameters = extern_probe::parameter_list(names, function, checked)?;
    let arguments = extern_probe::argument_names(function.params.len());
    let module = src.component_at(ast.decls[function.decl as usize].span.start);
    let probe = extern_probe::probe_name(module, &name);
    let columns = extern_probe::argument_columns(&probe, &name, &parameters, &arguments);
    let index = columns.iter().position(|column| *column == at.2 as usize);
    // **The parameter's own declared type, read from the value** rather than
    // asserted to be `cstr` (CLAUDE.md §11 — a narrowing asks the value). A future
    // Heroes type that also lowers to a pointer would otherwise be described wrong.
    let slot = index.and_then(|i| function.params.get(i));
    let which = match slot {
        Some(slot) => format!("`{}`", function.slots[slot.0 as usize].name),
        None => "a parameter".to_string(),
    };
    let declared = slot
        .map(|slot| checked.types.get(function.slots[slot.0 as usize].ty))
        .map(|ty| format!("{ty:?}").to_lowercase())
        .unwrap_or_else(|| "cstr".to_string());
    let span = ast.decls[function.decl as usize].span;
    Some(
        Diagnostic::new(
            "ffi_writable_parameter",
            format!(
                // **"does not promise to leave it alone", not "would write through
                // it"** (author decision 2026-08-15, from panel 059's findings). The
                // first wording was measured wrong on the first function anyone
                // tries: `free(void *)` does not *write* through its argument, it
                // **deallocates** it — and `strdup`'s result reaching `free` is
                // exactly the case this class now blocks. Both are the same defect
                // for this program and neither is "writing", so the message states
                // what the header actually declares and lets the note carry the
                // repair. CLAUDE.md §11's rule pointed at prose: the sentence
                // asserted a mechanism where only a permission is known.
                "{which} of `{name}` is declared `{declared}`, and the header says `{c_type}` — C does not promise to leave it alone"
            ),
            span,
        )
        .with_note(
            "§4.20: `s.cstr()` lends the string's own bytes, and they may be shared with other values — a C write through them changes values this program never passed to C, and on a literal it aborts".to_string(),
        )
        // **The repair is not "add `const`"**, which is the header's to choose and
        // not this program's. It is to stop lending and start owning.
        .with_note(format!(
            "declare it `ptr` and give `{name}` a buffer C owns, then read the result back"
        ))
        .with_fix(Fix {
            title: "declare it `ptr`".to_string(),
            replacement: "ptr".to_string(),
            span,
            certainty: Certainty::Guess,
        }),
    )
}
