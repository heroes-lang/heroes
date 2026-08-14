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
    src: &Source,
) -> Option<Diagnostic> {
    let kind = [NARROWED, RESIGNED].into_iter().find(|what| line.contains(what))?;
    let (at, rest) = location(line, src)?;
    // `'int64_t' (aka 'long long') to 'int'` — the header's type is the last
    // quoted name on the line, and it is what the parameter should be declared as.
    let c_type = rest.rsplit('\'').nth(1)?;
    let spelled = spelling(c_type)?;
    let heroes_type = &spelled.heroes;
    let what = if kind == NARROWED { "wider than" } else { "a different sign from" };
    let (function, name) = extern_at_line(program, ast, src, at.0, at.1)?;
    let parameters = extern_probe::parameter_list(function, checked)?;
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
    let mut diagnostic = Diagnostic::new(
        "ffi_parameter_type",
        format!(
            "{which} of `{name}` is declared {what} the header's `{c_type}` — clang read the header, and C would convert the value in silence"
        ),
        span,
    )
    .with_note(format!(
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
fn location(line: &str, src: &Source) -> Option<((usize, u32, u32), String)> {
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
fn extern_at_line<'a>(
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

/// A Heroes type to declare a parameter as, and what the reader must know about
/// how this answer was reached.
struct Spelling {
    heroes: String,
    /// Present when the C type's width is the **platform's** rather than a width
    /// the header chose, so the note must say which target this answer is for.
    caveat: Option<String>,
}

/// The Heroes type a C type of this width and signedness is declared as. The set
/// is `IntKind`'s, and a C type outside it means the narrowing is one this
/// language cannot currently express — which is a gap to report, not a fix to
/// propose, so the class stays silent rather than guessing.
///
/// **`long` and `unsigned long` used to be absent, and their absence was the
/// defect** (author decision 2026-08-15, `/decide`). They are 64 bits on Darwin
/// and Linux and 32 on Windows, so no *tabled* spelling is right on three legs —
/// and the first draft tabled one anyway, marked the fix `guess`, and named the
/// platform variance in the justification, which is CLAUDE.md §11's failure mode
/// written out. Declining was the right correction to that and the wrong answer to
/// the question: it left `function malloc(size: i64) -> ptr` exiting **2** with raw
/// clang output, blaming the compiler for a declaration the author wrote — while
/// `u64` binds `malloc` correctly, so the language could express it and only the
/// diagnostic could not.
///
/// The answer is that a width is not a constant to be tabled. It is a question
/// about the target, and `word_width_bits` asks it.
fn spelling(c_type: &str) -> Option<Spelling> {
    let fixed = |heroes: &str| {
        Some(Spelling { heroes: heroes.to_string(), caveat: None })
    };
    match c_type {
        "int" => fixed("i32"),
        "short" | "short int" => fixed("i16"),
        "signed char" | "char" => fixed("i8"),
        "unsigned int" => fixed("u32"),
        "unsigned short" | "unsigned short int" => fixed("u16"),
        "unsigned char" => fixed("u8"),
        // **`long long` is the one C integer that is 64 bits everywhere**, by
        // C99's own floor (`LLONG_MIN` ≤ −(2^63−1)) and by every ABI that has
        // shipped since. It carries no caveat because there is nothing about the
        // target left to say. It was missing from the table for the same reason
        // `long` was: nobody had bound a function that takes one.
        "long long" | "long long int" => fixed("i64"),
        "unsigned long long" | "unsigned long long int" => fixed("u64"),
        // `size_t` and `ssize_t` need no rows: clang reports the **canonical**
        // type, so `malloc`'s parameter arrives here as `unsigned long` and not
        // as `size_t`. Verified on the message this class was built from.
        "long" | "long int" => Some(word_width('i')),
        "unsigned long" | "unsigned long int" => Some(word_width('u')),
        // A C type absent from this table means the class **declines** — clang's
        // own verdict is still printed, and the compiler does not pretend to know
        // the repair. `float` is the live example (§4.19: `f32` does not exist).
        _ => None,
    }
}

/// The spelling for a C type whose width is the machine's word, with the note
/// that says so.
fn word_width(sign: char) -> Spelling {
    let bits = word_width_bits();
    let (other, here, there) = if bits == 64 {
        (32, "Darwin and Linux", "Windows")
    } else {
        (64, "Windows", "Darwin and Linux")
    };
    Spelling {
        heroes: format!("{sign}{bits}"),
        caveat: Some(format!(
            "C's `long` is the platform's word, not a width the header chose: {bits} bits here ({here}) and {other} on {there}. `{sign}{bits}` is the answer for this target — a program that must build on {there} too declares the width it means there and converts at the call"
        )),
    }
}

/// How wide C's `long` is in the C this compiler is about to emit, in bits.
///
/// **Derived, never tabled, and the derivation is the whole point.** `c_ulong` is
/// Rust's name for the same C type clang is about to compile, and both run on this
/// machine for this target: `heroes` has no `--target` and cross-compilation is not
/// on the command surface (CLAUDE.md §10 — one argv table, and it has no such
/// flag). So this is a fact about the value in hand rather than a premise about
/// which platforms exist, which is the distinction CLAUDE.md §11 turns on — a fact
/// about the value cannot expire, and the tabled version expired the moment a third
/// CI leg arrived.
///
/// **The claim, written so it can die loudly**: `size_of::<c_ulong>()` is
/// `sizeof(unsigned long)` in the emitted C. `the_word_width_this_compiler_claims_is_the_one_clang_uses`
/// in `crates/heroes-cli/tests/surface.rs` builds a binding at this width through
/// the real toolchain and fails the day the two disagree — which is the day a
/// `--target` flag lands, and it will fail *in the milestone that adds it* rather
/// than in a diagnostic somebody reads six months later.
fn word_width_bits() -> usize {
    std::mem::size_of::<std::os::raw::c_ulong>() * 8
}
