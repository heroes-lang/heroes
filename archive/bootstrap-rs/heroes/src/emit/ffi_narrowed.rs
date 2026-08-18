//! The class where **a parameter narrows in silence** (design.md §4.19; panels
//! 051, 052).
//!
//! Split from `ffi.rs` by the §11 sweep, and kept apart from `ffi_declared.rs`
//! because the mechanism is different in the way that matters: those classes read
//! a message this emitter wrote, and this one reads a message **clang** wrote,
//! about a probe this emitter placed. There is no marker to match and no C name in
//! the text — the gate is the *location*, and `emit/extern_probe.rs` is what makes
//! a location mean something by putting one `#line` per probe at the author's own
//! `extern` line.
//!
//! Panel 052's compiler-engineer predicted this file's parent would pass 440 lines
//! if this class landed in it. It landed and the parent reached 573; the prediction
//! held, and this file is the answer to it.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::Ast;

use super::extern_probe;
use super::c_spellings::spelling;
use super::ffi_mutable as mutable;


/// What clang calls a silent narrowing, and what it calls a silent change of
/// sign. Two strings, each matched whole.
///
/// **Both are the same defect** — a parameter declared as something the header
/// does not say — and they are one class because the repair is one sentence:
/// declare it at the header's own width *and* signedness. Measured on
/// `examples/curl/`, where the two arrived a day apart from the same cause:
/// `CURLcode`'s compatible integer type is **unsigned**, so `i32` is wrong for the
/// parameter exactly as `i32` was wrong for the result.
const NARROWED: &str = "implicit conversion loses integer precision";

const RESIGNED: &str = "implicit conversion changes signedness";

/// The **class** mismatch, which is a wider family than the two above and was
/// falling through to exit 2 until panel 071.
///
/// Measured, seven shapes, all under this project's own flags: a `ptr`, a `cstr`,
/// a `str`, an `@` parameter or a record declared against a header that says
/// something else. clang phrases them two ways —
/// `incompatible pointer to integer conversion passing 'void *' to parameter of
/// type 'int32_t'` and `passing 'HeroStr' to parameter of incompatible type
/// 'int32_t'` — and **both contain `" to parameter of "`**, which is the whole
/// matcher. The two width strings above do not, so the families do not overlap.
///
/// **The gate is unchanged and that is the point.** `extern_at_line` still asks
/// whether an `extern` is declared at exactly this file and line, which is what
/// panel 052 made the class safe with; only the phrase set widens. A gate reading
/// *"the path ends in `.hero`"* would be a premise about the world, and this
/// emitter lowers an author's call site under that author's own `#line`.
const MISMATCHED: &str = " to parameter of ";

/// A parameter declared wider than the header's, which C narrows in silence
/// (§4.19; panels 051 and 052).
///
/// **The gate is the location, and it is narrower than "a `.hero` file".** Every
/// other class here recovers a C name from clang's output and asks `declaration()`
/// whether *this* program declared it. This message carries no name at all — it is
/// `implicit conversion loses integer precision: 'int64_t' to 'int'` and nothing
/// more — so the question is asked of the location instead: is an `extern`
/// **declared at exactly this file and line**? That is `extern_at_line`, and it is
/// what makes the class safe.
///
/// A gate reading only *"the path ends in `.hero`"* would be a premise about the
/// world rather than a fact about the value (CLAUDE.md §11), and it is falsifiable
/// today: this emitter lowers an author's call site under that author's own
/// `#line`, so a narrowing **the compiler itself** emitted would be reported
/// against a `.hero` path and blamed on the reader. Panel 052's historian named
/// this, and named the shipped answer: cgo does not read messages either, and
/// `cmd/cgo/gcc.go` says why — *"we used to look at specific warning or error
/// messages here, but that tied the behavior too closely to specific versions of
/// the compilers"* — gating instead on synthetic per-probe `#line` filenames. The
/// declaration line is this compiler's version of that channel: `extern_probe.rs`
/// puts one `#line` per probe, and only a probe's line can name an `extern`.
///
/// The declaration is then found by **line**, and the offending parameter by
/// **column**, rebuilt from `probe_line` — the same string the emitter wrote,
/// recomputed rather than read back out of clang's echo.
pub(super) fn parameter_width(
    line: &str,
    ast: &Ast,
    checked: &crate::types::Checked,
    program: &crate::ir::Program,
    names: &super::typedefs::Names,
    src: &Source,
) -> Option<Diagnostic> {
    // **`discards qualifiers` belongs to `ffi_mutable.rs` and this must not take
    // it** (found by the golden corpus the hour this matcher widened, which is
    // what that corpus is for). clang's text for a `cstr` handed to a `char *` is
    // `'const char *' to parameter of type 'char *'` — it contains `MISMATCHED`,
    // so the wider family would swallow panel 058's whole class and then say
    // *"declare it `cstr`"* to an author who already had. The two questions are
    // different: this one asks whether the types convert, and that one asks
    // whether C may **write** through the pointer, which is design.md §1.12 and
    // not a type mismatch at all.
    //
    // The test is the phrase clang itself uses for the qualifier, not a guess
    // about which class *ought* to win — `ffi_mutable.rs` owns that string and
    // this line names it as its own.
    if line.contains(mutable::DISCARDS) {
        return None;
    }
    let kind = [NARROWED, RESIGNED, MISMATCHED].into_iter().find(|what| line.contains(what))?;
    let (at, rest) = location(line, src)?;
    // `'int64_t' (aka 'long long') to 'int'` — the header's type is the last
    // quoted name on the line, and it is what the parameter should be declared as.
    let c_type = rest.rsplit('\'').nth(1)?;
    // **The diagnostic fires whether or not the type has a Heroes spelling, and
    // only the `Fix` consults the table** (panel 071's ffi-pragmatist made this a
    // condition of its approval). `parameter_width` declines wholesale when
    // `spelling()` misses, which is right for a width — a gap in a table is a gap
    // to report — and catastrophic for a class: the types with no Heroes name are
    // exactly the interesting ones. A `Vec2` parameter's repair is *declare
    // `record Vec2` in the group*, a sentence rather than a type name, and
    // declining would send it back to exit 2 with the compiler blamed. That is
    // **356 of 1159** entry points by panel 052's count.
    let spelled = spelling(c_type);
    let what = match kind {
        NARROWED => "wider than",
        RESIGNED => "a different sign from",
        _ => "a different kind of thing from",
    };
    let (function, name) = extern_at_line(program, ast, src, at.0, at.1)?;
    let parameters = extern_probe::parameter_list(names, function, checked)?;
    let arguments = extern_probe::argument_names(function.params.len());
    let module = src.component_at(ast.decls[function.decl as usize].span.start);
    let probe = extern_probe::probe_name(module, &name);
    let columns = extern_probe::argument_columns(&probe, &name, &parameters, &arguments);
    let index = columns.iter().position(|column| *column == at.2 as usize);
    let which = match index.and_then(|i| function.params.get(i)) {
        Some(slot) => format!("`{}`", function.slots[slot.0 as usize].name),
        // The column did not land on an argument — a clang that carets the call
        // rather than the argument, say. The rule is still the rule and the
        // declaration is still the place; only the parameter's name is lost.
        None => "a parameter".to_string(),
    };
    let span = ast.decls[function.decl as usize].span;
    // The fix rewrites the *type*, and the type's own span is not carried in the
    // IR — so the fix points at the declaration and says what to write. A `guess`
    // is not applied by machine, so a span that names the line is enough.
    let ty_span = span;
    // A width is converted **in silence**; a class mismatch is not — clang refuses
    // it outright — so the sentence that follows the type has to differ or one of
    // the two is false. This is the sentence panel 048 and 051 kept getting wrong
    // in the other direction, and it is cheaper to split it than to find a wording
    // true of both.
    let consequence = match kind {
        MISMATCHED => "and the two do not convert",
        _ => "and C would convert the value in silence",
    };
    let mut diagnostic = Diagnostic::new(
        "ffi_parameter_type",
        format!(
            "{which} of `{name}` is declared {what} the header's `{c_type}` — clang read the header, {consequence}"
        ),
        span,
    );
    let Some(spelled) = spelled else {
        // **No Heroes spelling, and the diagnostic still lands.** The note says
        // what the reader must do instead of naming a type that does not exist:
        // a C struct parameter is bound by declaring the header's own `record`
        // inside the group (panel 060), and anything else is a signature this
        // language cannot express.
        return Some(diagnostic.with_note(format!(
            "§4.19: a parameter is declared as what the header says. `{c_type}` has no name in this language — if it is a struct, declare it as a `record` inside this same `extern` group and the header owns its layout"
        )));
    };
    let heroes_type = &spelled.heroes;
    diagnostic = diagnostic.with_note(format!(
        "§4.19: a result may be wider than C's, and a parameter is declared at the header's own width and sign. Declare it `{heroes_type}`, and convert at the call where the value is known to fit"
    ));
    // A **second** note rather than a longer first one: the width rule is the
    // same for every C type, and this says something extra about *this* one —
    // that the answer above is this target's and not the header's.
    if let Some(caveat) = &spelled.caveat {
        diagnostic = diagnostic.with_note(caveat.clone());
    }
    Some(diagnostic.with_fix(Fix {
        title: format!("declare it `{heroes_type}`"),
        replacement: heroes_type.clone(),
        // A **guess**, and deliberately — for two different reasons depending on
        // the type. For a fixed-width C type the spelling is exact and the guess
        // is about *intent*: the author may have meant to convert at the call
        // rather than to change the declaration. For a word-width type it is also
        // about the target, and the note above says so. Only a `certain` fix is
        // machine-applicable (CLAUDE.md §8), and neither reading may be applied
        // for the author.
        span: ty_span,
        certainty: Certainty::Guess,
    }))
}

/// `file:line:col: …` split off the head of a clang report, with the file kept
/// **only if it is one of this compilation's own** — which is what makes this the
/// author's mistake rather than the emitter's.
pub(super) fn location(line: &str, src: &Source) -> Option<((usize, u32, u32), String)> {
    let mut parts = line.splitn(4, ':');
    let file = parts.next()?;
    let at_line: u32 = parts.next()?.trim().parse().ok()?;
    let column: u32 = parts.next()?.trim().parse().ok()?;
    let rest = parts.next()?.to_string();
    // **The library is not one of these files**, even though it is a registered
    // `Source` entry. `source/files.rs` states the invariant: its name is *"not a
    // path, and deliberately unopenable: a clang error against one of its lines
    // must not look like an error in a file the author can edit"*. Without this,
    // a runtime whose header disagreed with the library's own `extern` group
    // would be reported at exit 1 against `<heroes library>` — the compiler's bug
    // dressed as the author's, pointing at a file that cannot be opened. Panel
    // 052's compiler-engineer found it in the emitted goldens.
    let index = src
        .files()
        .iter()
        .position(|entry| entry.name == file && !entry.is_library)?;
    Some(((index, at_line, column), rest))
}

/// The `extern` declared at a file and line — the one the probe's `#line` claims.
pub(super) fn extern_at_line<'a>(
    program: &'a crate::ir::Program,
    ast: &Ast,
    src: &Source,
    file: usize,
    at_line: u32,
) -> Option<(&'a crate::ir::Function, String)> {
    program.functions.iter().find_map(|function| {
        let decl = &ast.decls[function.decl as usize];
        let start = decl.name.start;
        if src.file_of(start) != file || src.file_line_of(start) != at_line {
            return None;
        }
        Some((function, src.slice(decl.name).to_string()))
    })
}



