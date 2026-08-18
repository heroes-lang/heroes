//! Making clang say whether the author's `extern` declaration matches the real
//! header (design.md §4.19; CLAUDE.md §7's one named exception; panels 036, 038,
//! 042).
//!
//! Split out of `externs.rs` by the §11 sweep. `externs.rs` reads what the group's
//! head line says — which header, which library. This is the half that **checks**
//! it, and the two separate on more than size: this file is the only place in the
//! backend where a clang failure is the **author's** mistake rather than the
//! compiler's, which is exit 1 and a `ffi_return_type` diagnostic on the `.hero`
//! line instead of exit 2 and "the compiler is wrong".
//!
//! **The mechanism is that a `_Generic`'s controlling expression is not evaluated**
//! (C11 6.5.1.1p3) but *is* type-checked, so a call with zero arguments of the
//! declared types costs nothing at runtime and asks clang what the real header
//! returns. Eleven of eleven correct ladder bindings pass; `strlen` declared
//! `-> i64` fires, because `size_t` is unsigned and the widening set admits only
//! signed C integers.
//!
//! One thing here is a repaired defect rather than a design: there is **no unary
//! `+`** on the controlling expression. It was added to make enum-returning C
//! functions pass and was measured as unnecessary — `_Generic` on an enum already
//! selects its compatible integer type — while a unary `+` on a POINTER is a hard
//! clang error, so `extern function getenv(...) -> int` produced *"internal error:
//! compiling the generated C failed"* on exactly the return type most likely to be
//! declared wrong. It also promotes away every distinction below `int`, which the
//! narrow widths need kept.

use crate::ir::{Function, Program, SlotKind};
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::Checked;

use super::externs::{extern_spans, is_extern_constant};
use super::writer::Writer;

/// **The half of §4.19's guarantee that clang does not give for free.**
///
/// The emitter does not re-declare an `extern`'s signature — re-declaring is
/// `conflicting types` five times out of five on SQLite, because Heroes' `i64` is
/// `int64_t` and every C entry point returns `i64`. So the header declares the
/// function and clang checks the *call*: the arguments, and nothing else. Panel
/// 036 measured what that leaves open — four wrong bindings out of six compile
/// clean, and `extern function sqrt(x: f64) -> i64` exits 0 printing `1`.
///
/// One `_Static_assert` per `extern` closes it. The controlling expression of a
/// `_Generic` is **not evaluated** (C11 6.5.1.1p3) but is type-checked, so a call
/// with zero arguments of the declared types costs nothing at runtime and asks
/// clang what the real header returns. Eleven of eleven correct ladder bindings
/// pass; `strlen` declared `-> i64` fires, because `size_t` is unsigned and the
/// widening set admits only signed C integers.
pub(super) fn extern_assertions(
    w: &mut Writer,
    program: &Program,
    ast: &Ast,
    checked: &Checked,
    names: &super::typedefs::Names,
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
    // on the enum's own type rather than on `i64`, so the first version of this
    // macro refused **every enum-returning C function in existence** — which is
    // most of libcurl, OpenSSL and raylib, and was invisible against SQLite
    // because SQLite returns plain `i64`.
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
    // error**, so `extern function getenv(name: cstr) -> i64` — an ordinary
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
    // **The seven other widths, and they cannot reuse the row above** (panel 042,
    // ffi-pragmatist, compiled against real headers on five targets). Three facts
    // decide the shape:
    //
    // - **No unary `+`.** It applies the integer promotions, so `+(c)` on a
    //   `signed char` is an `int` and the narrow rows would either reject every
    //   correct binding or check nothing at all. The `+` also broke pointer
    //   returns outright, which is `tests/golden/fixedbugs/ffi-pointer-return.hero`.
    // - **Fundamental types, never typedefs.** `size_t` and `uintptr_t` in one
    //   `_Generic` is a *hard clang error* — they are the same type on this
    //   target — so the class test lists the thirteen fundamental integer types,
    //   which C guarantees are pairwise distinct.
    // - **`sizeof` is load-bearing, not belt-and-braces.** Without it, i386 and
    //   wasm32 ACCEPT a 32-bit `unsigned long` as `u64`: `_Generic` can say what
    //   kind a type is and cannot say how wide it is. Measured, both targets.
    //
    // `char`'s signedness is implementation-defined, so it is asked rather than
    // assumed: `(char)-1 > 0` is the question, answered at compile time.
    w.line("#define HERO_C_INTEGER(c) _Generic((c), _Bool:1, char:1, signed char:1, short:1, int:1, long:1, long long:1, unsigned char:1, unsigned short:1, unsigned int:1, unsigned long:1, unsigned long long:1, default:0)");
    w.line("#define HERO_C_UNSIGNED(c) _Generic((c), unsigned char:1, unsigned short:1, unsigned int:1, unsigned long:1, unsigned long long:1, _Bool:1, char:((char)-1 > 0), signed char:0, short:0, int:0, long:0, long long:0, default:0)");
    for (name, signed_test, bytes) in [
        ("HERO_RET_I8", "!HERO_C_UNSIGNED(c)", 1),
        ("HERO_RET_I16", "!HERO_C_UNSIGNED(c)", 2),
        ("HERO_RET_I32", "!HERO_C_UNSIGNED(c)", 4),
        ("HERO_RET_U8", "HERO_C_UNSIGNED(c)", 1),
        ("HERO_RET_U16", "HERO_C_UNSIGNED(c)", 2),
        ("HERO_RET_U32", "HERO_C_UNSIGNED(c)", 4),
        ("HERO_RET_U64", "HERO_C_UNSIGNED(c)", 8),
    ] {
        w.line(&format!(
            "#define {name}(c) (HERO_C_INTEGER(c) && {signed_test} && sizeof(c) == {bytes})"
        ));
    }
    // **`f32` accepts `float` and nothing else**, where `f64` below accepts all
    // three. That asymmetry is the spec's own rule — *"a result may be wider than
    // C's"* — read in the one direction it can be read: declaring `f64` for a C
    // `float` widens and is exact, declaring `f32` for a C `double` **narrows**,
    // silently, at every call. The macro is the only thing that can see the
    // difference, because both compile.
    w.line("#define HERO_RET_F32(c) _Generic((c), float:1, default:0)");
    w.line("#define HERO_RET_F64(c) _Generic((c), float:1, double:1, long double:1, default:0)");
    w.line("#define HERO_RET_BOOL(c) _Generic((c), _Bool:1, default:0)");
    w.line("#define HERO_RET_STR(c) _Generic((c), HeroStr:1, default:0)");
    // **`void` cannot be a `_Generic` association, and this line said it was.**
    // C11 6.5.1.1p2: *"The type name in a generic association shall specify a
    // complete object type"*, and `void` is incomplete — so this was a constraint
    // violation, always, in every program that binds a C function returning
    // nothing. One clang accepted it and every other rejected it: the laptop this
    // project was built on said nothing for two milestones, and the **first CI
    // run that ever compared two machines** failed on both of them
    // (M-program-corpus, `error: type 'void' in generic association incomplete`).
    //
    // `__builtin_types_compatible_p` is the answer clang and GCC both give, it
    // takes `void` without complaint, and its operand is unevaluated exactly as
    // `_Generic`'s is — so the call is still never made. It is not a new
    // dependency: CLAUDE.md §7 already commits every arithmetic operation to
    // `__builtin_*_overflow`.
    w.line("#define HERO_RET_UNIT(c) __builtin_types_compatible_p(__typeof__(c), void)");
    // A pointer return, asked **directly**. This was a `_Generic` listing what a
    // pointer is *not*, on the premise that *"`_Generic` cannot say 'any pointer',
    // and `default:1` alone would check nothing"*. The premise was true about
    // `_Generic` and false about C: `__builtin_classify_type` answers it, and its
    // codes are GCC's own `typeclass.h` — 5 is a pointer, 12 a struct — with the
    // operand unevaluated exactly as `_Generic`'s is.
    //
    // What the negative list could not see was the **struct**, which fell through
    // to `default:1` and passed. So `extern function makebox(n: i64) -> ptr`
    // against a C function returning a struct by value compiled its assertion
    // clean and then failed at the call site as *"internal error: compiling the
    // generated C failed"* — exit 2, the compiler blamed for the author's
    // declaration, which is the one outcome §4.19's whole apparatus exists to
    // prevent. That is **356 of 1159 entry points across three real headers**
    // (panel 052's ffi-pragmatist), so it is the ordinary case and not a corner.
    //
    // Verified six ways on this clang: `void *` and `const char *` pass; a struct,
    // an integer, a double and a `HeroStr` are all refused.
    w.line("#define HERO_RET_PTR(c) (__builtin_classify_type(c) == 5)");
    // **A struct result, and the macro is emitted only where one exists** (panel
    // 063, the compiler-engineer's condition 3).
    //
    // Not thrift. Every line in this preamble shifts the `#line N` restores under
    // it by one, and emitting this unconditionally renumbered **56 restore
    // directives, 112 lines across 5 files** in `tests/golden/emit/` — a directory
    // where `crates/heroes-cli/tests/golden.rs` forbids `UPDATE_GOLDEN`, so the
    // churn would have to be hand-read. Conditional, the golden tree does not move
    // at all, which is the second half of that judge's prediction.
    //
    // `__builtin_types_compatible_p` over `_Generic`, and the reason is in
    // `assert_spelling.rs::return_check`: a `_Generic` association must be a
    // complete type and this builtin has no such constraint — it is also already
    // the mechanism `HERO_RET_UNIT` uses two lines up, so the family gains no new
    // dependency. `__typeof__` gives the type of an **unevaluated** expression
    // exactly, which is why no probe is needed here where the parameter check
    // needed one: that check wanted a *conversion* diagnostic, and a conversion in
    // an unevaluated operand is silent, while this wants a *type-identity fact*.
    // Verified from the other side — `nm -u` on a unit whose only mention of the
    // function is this assertion shows no undefined symbol, so the call is never
    // emitted.
    if externs
        .iter()
        .any(|function| matches!(checked.types.get(function.result), crate::types::Ty::Named(_)))
    {
        w.line("#define HERO_RET_RECORD(c, T) __builtin_types_compatible_p(__typeof__(c), T)");
    }
    // **Conditional for panel 063's reason, which this file already paid once.**
    // Every line of this preamble shifts the `#line N` restores under it, and
    // emitting a macro unconditionally renumbered 56 restore directives across five
    // `tests/golden/emit/` files — a directory where `UPDATE_GOLDEN` is forbidden,
    // so the churn is hand-read. Emitted only where an `extern` returns a `cstr`,
    // the golden tree does not move at all.
    if externs
        .iter()
        .any(|function| checked.types.get(function.result) == crate::types::Ty::Cstr)
    {
    // **A `cstr` is a pointer to characters, and only that** (panel 083). Six
    // spellings, because C's `char` is a distinct type from both `signed char` and
    // `unsigned char` and real headers return all three — `sqlite3_column_text`
    // gives `const unsigned char *`. `const void *` is refused, which is the whole
    // repair: it used to pass, and `to_str()` then walked past the object to the
    // next NUL.
    w.line(
        "#define HERO_RET_CSTR(c) _Generic((c), \
char *:1, const char *:1, signed char *:1, const signed char *:1, \
unsigned char *:1, const unsigned char *:1, default:0)",
    );
    }
    for function in externs {
        let name = src.slice(ast.decls[function.decl as usize].name);
        let Some(check) = super::assert_spelling::return_check(checked, function.result) else { continue };
        let declared_type = crate::types::render_ty(&checked.types, ast, src, function.result, &[]);
        // **A constant is a token, not a call.** The same `_Generic` asks the same
        // question of it — *what type does the header give this?* — with no
        // argument list to build, and one more assertion nothing else needs: that
        // the header gives it a **value** at all. `stdout` and `errno` are
        // objects, and a zero-argument accessor over one would return a different
        // value on two calls, which is the mutable global §4.2 forbids arriving
        // through the back door.
        let type_argument = super::assert_spelling::return_argument(names, checked, function.result);
        if is_extern_constant(ast, function) {
            w.line(&format!(
                "_Static_assert({check}({name}{type_argument}), \"{}{name} {declared_type}\");",
                super::ffi::ASSERTION
            ));
            // **A struct constant is not asked whether it is a value** (panel 063).
            // `__builtin_constant_p` answers 0 for every struct, compound literal
            // included, so asking would newly refuse raylib's 26 `CLITERAL(Color)`
            // macros — which bind and run at exit 0 today. The hole that leaves is
            // named in `assert_spelling::asks_for_a_value` and has a test, rather
            // than being absorbed by this comment.
            if !super::assert_spelling::asks_for_a_value(checked, function.result) {
                continue;
            }
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
                super::assert_spelling::zero_of(names, checked, declared.ty, mutable)
            })
            .collect();
        // The message is a **contract with `ffi::explain`**, not prose: it carries
        // the marker, the C name and the declared Heroes type, so a failure can be
        // mapped back to the author's line instead of printing generated C.
        w.line(&format!(
            "_Static_assert({check}({name}({}){type_argument}), \"{}{name} {declared_type}\");",
            zeros.join(", "),
            super::ffi::ASSERTION
        ));
    }
    w.line("");
}
