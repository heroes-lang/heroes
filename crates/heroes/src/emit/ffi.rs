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

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

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

/// What clang prints on the line of a `_Static_assert` that failed. Required
/// before a line is read as one of ours, because clang **echoes the source line
/// under its diagnostic** — and that echo contains the assertion's message text
/// verbatim, marker and all.
///
/// Without this the echo parsed as a second, malformed report of the same
/// failure: `` `sqlite3_openn` does not return `int");` ``. It was invisible
/// because the message line comes first and the duplicate was dropped by span —
/// so the good path was right by ordering rather than by matching, which is a
/// premise about clang's output order rather than a fact about the line in hand
/// (CLAUDE.md §11). Found by panel 038's ffi-pragmatist, compiling.
const FAILED: &str = "static assertion failed";

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
        if let Some(diagnostic) = wrong_type(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = not_constant(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = unknown_name(line, stderr, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = missing_link(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = missing_header(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = package_problem(stderr, line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = parameter_width(line, ast, checked, program, src) {
            push(&mut found, diagnostic);
        }
    }
    found
}

/// One diagnostic per `.hero` line: clang reports a translation unit, and one
/// mistake in it can produce several lines that all mean the same thing.
fn push(found: &mut Vec<Diagnostic>, diagnostic: Diagnostic) {
    if !found.iter().any(|d: &Diagnostic| d.span == diagnostic.span) {
        found.push(diagnostic);
    }
}

/// `_Static_assert(HERO_RET_INT(sqrt(…)), "heroes-ffi-return sqrt int")` failed:
/// the header disagrees with the declared type.
///
/// Two codes from one assertion, because the same `_Generic` asks the same
/// question of a call and of a constant, and the reader's mistake is not the
/// same: a function *returns* the wrong type, a constant **is** one.
fn wrong_type(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(ASSERTION).nth(1)?;
    // `<name> <type>` — written by the emitter, so the split is on its own
    // format rather than on a guess about clang's.
    let mut parts = rest.split_whitespace();
    let (name, declared) = (parts.next()?, parts.next()?);
    let (span, header, is_constant) = declaration(ast, src, name)?;
    let diagnostic = if is_constant {
        Diagnostic::new(
            "ffi_constant_type",
            format!(
                "`{name}` is not `{declared}` in `{header}` — clang read the header and the types disagree"
            ),
            span,
        )
        .with_note(format!(
            "an `extern constant` takes its value from the header (§4.19), so its type is the header's too: correct the declared type, or name the header that spells `{name}` as `{declared}`"
        ))
    } else {
        Diagnostic::new(
            "ffi_return_type",
            format!(
                "`{name}` does not return `{declared}` — that is what `{header}` says, and clang read it"
            ),
            span,
        )
        .with_note(format!(
            "an `extern` is checked against the real header (§4.19): correct the result type, or name the header that declares this `{name}`"
        ))
    };
    Some(diagnostic)
}

/// `_Static_assert(__builtin_constant_p(stdout), "heroes-ffi-const stdout")`
/// failed: the header has the name, and it is an **object** rather than a value.
///
/// This is the one refusal no other language makes as a rule (panel 038's
/// historian: Nim, Swift and Go all accept it, Zig refuses it by accident). The
/// reason Heroes must is §4.2's: a zero-argument accessor over `errno` returns a
/// different value on two calls, which is a mutable global arriving through the
/// back door.
fn not_constant(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(CONSTANCY).nth(1)?;
    let name = rest.split_whitespace().next()?.trim_end_matches(['"', ',', ')', '\'']);
    let (span, header, _) = declaration(ast, src, name)?;
    let diagnostic = Diagnostic::new(
        "ffi_not_constant",
        format!(
            "`{header}` has `{name}`, but not as a constant — clang cannot read its value while compiling"
        ),
        span,
    );
    Some(diagnostic.with_note(format!(
        "a `constant` names one value forever (§4.4), and `{name}` is an object or a macro that expands to one — reading it twice could give two answers, which is the mutable global §4.2 forbids. A C object is reached through an `extern function` that returns it"
    )))
}

/// The header has no such name at all. A different mistake from the one above and
/// it was being reported as that one — *"`sqlite3_openn` does not return `i64`"*,
/// whose remedy, correcting the result type, cannot fix a name that does not
/// exist (§4.17: the error carries what is needed to repair the program).
///
/// Two spellings, because C has two: a name used as a call is an undeclared
/// *function*, a name used as a value is an undeclared *identifier* — which is
/// what a misspelled `constant` produces.
fn unknown_name(line: &str, stderr: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = ["call to undeclared function '", "use of undeclared identifier '"]
        .iter()
        .find_map(|marker| line.split(marker).nth(1))?;
    let name = quoted.split('\'').next()?;
    let (span, header, _) = declaration(ast, src, name)?;
    let mut diagnostic = Diagnostic::new(
        "ffi_unknown_name",
        format!("`{header}` declares no `{name}` — clang read the header and could not find it"),
        span,
    );
    diagnostic = diagnostic.with_note(format!(
        "an `extern` names what the header already has (§4.19): check the spelling, or name the header that does declare `{name}`"
    ));
    // clang's own typo correction, when it offered one. A `Guess`: it is a
    // suggestion from a search over the header's names, not a fact about this
    // program, so `--apply` must not take it (CLAUDE.md §8).
    if let Some(meant) = did_you_mean(stderr, name) {
        let Some(at) = name_span(ast, src, name) else { return Some(diagnostic) };
        diagnostic.fixes.push(Fix {
            title: format!("the header declares `{meant}`"),
            replacement: meant,
            span: at,
            certainty: Certainty::Guess,
        });
    }
    Some(diagnostic)
}

/// The name clang suggested instead. Read from the whole of stderr rather than
/// from one line, because the note sits on a line of its own under the error.
fn did_you_mean(stderr: &str, name: &str) -> Option<String> {
    for line in stderr.lines() {
        if !line.contains(&format!("'{name}'")) && !line.contains("did you mean") {
            continue;
        }
        let Some(rest) = line.split("did you mean '").nth(1) else { continue };
        let meant = rest.split('\'').next()?;
        if meant != name {
            return Some(meant.to_string());
        }
    }
    None
}

/// The `extern` declaration with this C name, the header it was declared under,
/// and whether it is a `constant`. `None` for a name no group declares, which is
/// how a message that only looks like one of ours is refused.
fn declaration(ast: &Ast, src: &Source, name: &str) -> Option<(crate::source::Span, String, bool)> {
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
fn name_span(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
    ast.decls
        .iter()
        .find(|decl| src.slice(decl.name) == name)
        .map(|decl| decl.name)
}

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
fn missing_link(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
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
fn missing_header(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = line.split("file not found").next()?;
    let header = quoted.split('\'').nth(1)?;
    let span = group_head(ast, src, header)?;
    let mut diagnostic = Diagnostic::new(
        "ffi_missing_header",
        format!("`{header}` is not on this machine's include path — clang looked and did not find it"),
        span,
    );
    diagnostic = diagnostic.with_note(
        "the group names a header the preprocessor must be able to open: install the library's development files, or point the compiler at them".to_string(),
    );
    Some(diagnostic)
}

/// The span of the first declaration whose group names this header — which is
/// the `extern` line the author must look at.
///
/// The same gate as everywhere else in this file: a header no group in *this*
/// program names is not this program's business, and clang's line stays a
/// statement about the compiler.
fn group_head(ast: &Ast, src: &Source, header: &str) -> Option<crate::source::Span> {
    ast.decls.iter().find_map(|decl| {
        let named = match &decl.kind {
            DeclKind::Function(function) => function.header?,
            DeclKind::Constant { header, .. } => (*header)?,
            _ => return None,
        };
        (src.slice(named).trim_matches('"') == header).then_some(decl.span)
    })
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
fn package_problem(stderr: &str, line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
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

/// The span of the first declaration whose group asked about this package — the
/// same gate as everywhere else in this file, and the reason a stray line
/// mentioning a package name cannot become a diagnostic about a program that
/// never asked for it.
fn package_span(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
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
fn parameter_width(
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
    let heroes_type = heroes_spelling(c_type)?;
    let what = if kind == NARROWED { "wider than" } else { "a different sign from" };
    let (function, name) = extern_at_line(program, ast, src, at.0, at.1)?;
    let parameters = super::extern_probe::parameter_list(function, checked)?;
    let arguments = super::extern_probe::argument_names(function.params.len());
    let columns = super::extern_probe::argument_columns(&name, &parameters, &arguments);
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
    Some(
        Diagnostic::new(
            "ffi_parameter_type",
            format!(
                "{which} of `{name}` is declared {what} the header's `{c_type}` — clang read the header, and C would convert the value in silence"
            ),
            span,
        )
        .with_note(format!(
            "§4.19: a result may be wider than C's, and a parameter is declared at the header's own width and sign. Declare it `{heroes_type}`, and convert at the call where the value is known to fit"
        ))
        .with_fix(Fix {
            title: format!("declare it `{heroes_type}`"),
            replacement: heroes_type.to_string(),
            // A **guess**, and deliberately: clang names the header's C type and
            // this maps it to the Heroes type of that width, but a `long` is 64
            // bits on one platform and 32 on another (panel 047's class), so the
            // spelling that is right here is not always right elsewhere. Only a
            // `certain` fix is machine-applicable (CLAUDE.md §8).
            span: ty_span,
            certainty: Certainty::Guess,
        }),
    )
}

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

/// The Heroes type a C type of this width and signedness is declared as. The set
/// is `IntKind`'s, and a C type outside it means the narrowing is one this
/// language cannot currently express — which is a gap to report, not a fix to
/// propose, so the class stays silent rather than guessing.
fn heroes_spelling(c_type: &str) -> Option<&'static str> {
    Some(match c_type {
        "int" => "i32",
        "short" => "i16",
        "signed char" | "char" => "i8",
        "unsigned int" => "u32",
        "unsigned short" => "u16",
        "unsigned char" => "u8",
        // **`long` is deliberately absent, and so is `unsigned long`.** They are
        // 64 bits on Darwin and Linux and 32 on Windows, so there is no spelling
        // this function could return that is right on all three legs — and a
        // wrong one here is worse than silence, because it would tell the author
        // to write `u32` for a 64-bit parameter and the widening back would be
        // invisible. The first draft of this table did exactly that; the fix was
        // marked `guess` and the justification named the platform variance, which
        // is CLAUDE.md §11's failure mode written out: a premise about the world,
        // acknowledged and then shipped anyway. Panel 052's compiler-engineer
        // measured it on an `unsigned long` parameter.
        //
        // A C type absent from this table means the class **declines** — clang's
        // own verdict is still printed, and the compiler does not pretend to know
        // the repair.
        _ => return None,
    })
}
