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

use crate::ir::{Function, Program, SlotKind};
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{Checked, Ty, TyId};

use super::writer::Writer;

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
    seen
}

/// Every library a group named, deduplicated and in declaration order. Reached
/// from `Emitted`, so the one walk that knows about `link` lives beside the one
/// that knows about `header`.
pub(super) fn libraries(program: &Program, ast: &Ast, src: &Source) -> Vec<String> {
    let mut seen: Vec<String> = Vec::new();
    for function in &program.functions {
        let (_, link) = extern_spans(ast, function);
        let Some(link) = link else { continue };
        let library = src.slice(link).trim_matches('"').to_string();
        if !seen.contains(&library) {
            seen.push(library);
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
fn extern_spans(ast: &Ast, function: &Function) -> (Option<crate::source::Span>, Option<crate::source::Span>) {
    match &ast.decls[function.decl as usize].kind {
        crate::syntax::DeclKind::Function(declared) => (declared.header, declared.link),
        crate::syntax::DeclKind::Constant { header, link, .. } => (*header, *link),
        _ => (None, None),
    }
}

/// **The half of §4.19's guarantee that clang does not give for free.**
///
/// The emitter does not re-declare an `extern`'s signature — re-declaring is
/// `conflicting types` five times out of five on SQLite, because Heroes' `int` is
/// `int64_t` and every C entry point returns `int`. So the header declares the
/// function and clang checks the *call*: the arguments, and nothing else. Panel
/// 036 measured what that leaves open — four wrong bindings out of six compile
/// clean, and `extern function sqrt(x: f64) -> int` exits 0 printing `1`.
///
/// One `_Static_assert` per `extern` closes it. The controlling expression of a
/// `_Generic` is **not evaluated** (C11 6.5.1.1p3) but is type-checked, so a call
/// with zero arguments of the declared types costs nothing at runtime and asks
/// clang what the real header returns. Eleven of eleven correct ladder bindings
/// pass; `strlen` declared `-> int` fires, because `size_t` is unsigned and the
/// widening set admits only signed C integers.
pub(super) fn extern_assertions(
    w: &mut Writer,
    program: &Program,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) {
    // Everything a header owns, whichever kind it lowered to — a signature is
    // `FnKind::Extern`, a constant is `FnKind::Constant` with no blocks, and the
    // question both answer is *does a header declare this* (see `extern_spans`).
    let externs: Vec<&Function> =
        program.functions.iter().filter(|f| extern_spans(ast, f).0.is_some()).collect();
    if externs.is_empty() {
        return;
    }
    // Defined here rather than in `heroes_runtime.h` so the generated unit stays
    // self-contained and the runtime's ABI stamp does not move for a macro.
    // **`+(c)` and the unsigned narrows, both found by binding libcurl** (author
    // instruction, ladder rung 4). `CURLcode` is an `enum`, and `_Generic` selects
    // on the enum's own type rather than on `int`, so the first version of this
    // macro refused **every enum-returning C function in existence** — which is
    // most of libcurl, OpenSSL and raylib, and was invisible against SQLite
    // because SQLite returns plain `int`.
    //
    // **The repair was two changes and only one of them was needed** (panel 042,
    // 2026-08-12). M-ffi-ladder added a unary `+` *and* widened the accepted set
    // to `unsigned int` and its narrower siblings, on the reading that `+` applies
    // the integer promotions and is what turns an enum into a number. Measured
    // since, on Apple clang 21, against a synthetic enum and against the real
    // `CURLcode`: a **bare** `_Generic` accepts both, because C11 6.5.1.1 selects
    // an enum's *compatible integer type* and the widened set now contains it. The
    // second change alone was sufficient; the first was carrying a justification
    // for work it does not do.
    //
    // And it was doing damage while it did so. `+` on a pointer is a **hard clang
    // error**, so `extern function getenv(name: cstr) -> int` — an ordinary
    // mistake, since `getenv` returns `char *` — produced `invalid argument type
    // 'char *' to unary expression` at exit 2, which CLAUDE.md §7 makes a claim
    // that *the compiler* is wrong. The marker string below never reached
    // `emit/ffi.rs`, so §7's named exception could not fire on exactly the return
    // type most likely to be declared wrong. Without `+` the same program gets
    // exit 1 and an `ffi_return_type` on the `.hero` line, which is what panel 036
    // built this mechanism to do.
    //
    // `tests/golden/fixedbugs/ffi-pointer-return.hero` is the case, and
    // `an_enum_returning_extern_needs_no_unary_plus` is the test that fires if the
    // enum premise ever comes back (CLAUDE.md §11: a premise owes a falsifiable
    // claim and a test for its death).
    //
    // The accepted set is every signed integer plus every unsigned integer
    // narrower than 64 bits. **That is a fact about `i64`'s range and about
    // nothing else** — it is not a rule about "the integer type", and a second
    // width must never reuse this macro (panel 042; `M-sized-integers` owes its
    // own row per width).
    w.line("#define HERO_RET_INT(c) _Generic((c), signed char:1, short:1, int:1, long:1, long long:1, unsigned char:1, unsigned short:1, unsigned int:1, default:0)");
    w.line("#define HERO_RET_F64(c) _Generic((c), float:1, double:1, long double:1, default:0)");
    w.line("#define HERO_RET_BOOL(c) _Generic((c), _Bool:1, default:0)");
    w.line("#define HERO_RET_STR(c) _Generic((c), HeroStr:1, default:0)");
    w.line("#define HERO_RET_UNIT(c) _Generic((c), void:1, default:0)");
    // A pointer return is checked by its **negative** set: `_Generic` cannot say
    // "any pointer", and `default:1` alone would check nothing. Listing what a
    // pointer is not still catches the case that matters — a function returning an
    // integer or a float declared as `ptr`.
    w.line("#define HERO_RET_PTR(c) _Generic((c), signed char:0, short:0, int:0, long:0, long long:0, unsigned char:0, unsigned short:0, unsigned int:0, unsigned long:0, unsigned long long:0, float:0, double:0, long double:0, HeroStr:0, default:1)");
    for function in externs {
        let name = src.slice(ast.decls[function.decl as usize].name);
        let Some(check) = return_check(checked, function.result) else { continue };
        let declared_type = crate::types::render_ty(&checked.types, ast, src, function.result, &[]);
        // **A constant is a token, not a call.** The same `_Generic` asks the same
        // question of it — *what type does the header give this?* — with no
        // argument list to build, and one more assertion nothing else needs: that
        // the header gives it a **value** at all. `stdout` and `errno` are
        // objects, and a zero-argument accessor over one would return a different
        // value on two calls, which is the mutable global §4.2 forbids arriving
        // through the back door.
        if is_extern_constant(ast, function) {
            w.line(&format!(
                "_Static_assert({check}({name}), \"{}{name} {declared_type}\");",
                super::ffi::ASSERTION
            ));
            // `__builtin_constant_p` is itself a constant expression even when its
            // argument is not, so the failure stays a `_Static_assert` carrying
            // *our* message. A `static const T probe = X;` would fail with clang's
            // own words instead, and mapping those back to a `.hero` line would
            // widen CLAUDE.md §7's named exception from a message to a generated
            // line (panel 038, measured).
            w.line(&format!(
                "_Static_assert(__builtin_constant_p({name}), \"{}{name}\");",
                super::ffi::CONSTANCY
            ));
            continue;
        }
        let zeros: Vec<String> = function
            .params
            .iter()
            .map(|slot| {
                let declared = &function.slots[slot.0 as usize];
                // **An `@` parameter is a pointer parameter** (§4.8, CLAUDE.md §7),
                // so its zero is a null of that pointer type. Writing the value's
                // own zero instead was `-Wint-conversion` — a warning rather than
                // an error under C11, which is exactly how it survived a green
                // test run until the goldens were read.
                let mutable = matches!(declared.kind, SlotKind::Param { mutable: true });
                zero_of(checked, declared.ty, mutable)
            })
            .collect();
        // The message is a **contract with `ffi::explain`**, not prose: it carries
        // the marker, the C name and the declared Heroes type, so a failure can be
        // mapped back to the author's line instead of printing generated C.
        w.line(&format!(
            "_Static_assert({check}({name}({})), \"{}{name} {declared_type}\");",
            zeros.join(", "),
            super::ffi::ASSERTION
        ));
    }
    w.line("");
}

/// A `constant` whose value a header holds (§4.19, panel 038) — as opposed to an
/// `extern function`, which is called, or an ordinary `constant`, which has a body.
pub(super) fn is_extern_constant(ast: &Ast, function: &Function) -> bool {
    matches!(
        &ast.decls[function.decl as usize].kind,
        crate::syntax::DeclKind::Constant { body: None, header: Some(_), .. }
    )
}

/// Which assertion a declared result type asks for.
fn return_check(checked: &Checked, ty: TyId) -> Option<&'static str> {
    match checked.types.get(ty) {
        Ty::Int => Some("HERO_RET_INT"),
        Ty::F64 => Some("HERO_RET_F64"),
        Ty::Bool => Some("HERO_RET_BOOL"),
        Ty::Str => Some("HERO_RET_STR"),
        Ty::Unit => Some("HERO_RET_UNIT"),
        Ty::Ptr | Ty::Cstr => Some("HERO_RET_PTR"),
        // No other type crosses the boundary: `ffi_type` refuses them in the
        // checker, so this arm is where a new FFI type would have to declare
        // what its assertion is rather than silently getting none.
        _ => None,
    }
}

/// A zero of the declared parameter type, cast so the call type-checks. It is
/// never evaluated — it exists only to make the call expression well-formed.
fn zero_of(checked: &Checked, ty: TyId, mutable: bool) -> String {
    let value = match checked.types.get(ty) {
        Ty::Int => "int64_t",
        Ty::F64 => "double",
        Ty::Bool => "bool",
        Ty::Str => "HeroStr",
        Ty::Cstr => "const char *",
        _ => "void *",
    };
    if mutable {
        return format!("({value} *)0");
    }
    match checked.types.get(ty) {
        Ty::Str => "(HeroStr){0}".to_string(),
        _ => format!("({value})0"),
    }
}
