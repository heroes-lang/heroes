//! The built-ins' type rules (design.md §4.20's inventory, §1.11's two tiers).
//!
//! Hand-written rather than driven by a signature table, and that is the honest
//! shape: `print` takes any number of arguments of four types, `len` works on
//! three, `must` is generic over one. A signature table would need a type
//! language richer than Heroes has — which is precisely why these are
//! **built-ins** and not library declarations.
//!
//! Each rule states the shape it accepts and nothing else. Where a rule is
//! narrower than design.md's inventory promises, the narrowing is a rejection
//! that can be relaxed (`to_i64` on a `str` is not offered, because parsing can
//! fail and the spec does not say what it returns).
//!
//! `ok` and `fail` are absent: they are ⇐-only (panel 002) and live in
//! `exprs.rs`, where the expected type is in hand.

use crate::source::{Source, Span};
use crate::syntax::Ast;

use super::table::Ty;
use super::{errors, Checker, TyId};

/// What a built-in expects of its `index`-th argument, given the type of its
/// first — `None` where the rule has nothing to say and the argument must be
/// synthesised instead.
///
/// This exists so that the ⇐-only forms are writable inside a built-in call:
/// `out.push(.num(v: 1))` is the shape design.md's own appendix uses six times,
/// and without an expectation to hand down, `.num` has nothing to be checked
/// against.
pub(super) fn expectation(
    checker: &Checker,
    name: &str,
    first: TyId,
    index: usize,
) -> Option<TyId> {
    if index == 0 {
        return None;
    }
    match (name, checker.out.types.get(first), index) {
        ("push", Ty::Array(element), 1) => Some(element),
        ("default", Ty::Fallible(inner), 1) => Some(inner),
        ("join", _, 1) => Some(checker.out.types.str()),
        ("slice", _, 1 | 2) => Some(checker.out.types.int()),
        _ => None,
    }
}

/// The rule for one built-in call. `None` means "this built-in cannot be
/// checked with these arguments, and a diagnostic said so".
pub(super) fn call(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    args: &[TyId],
    span: Span,
) -> Option<TyId> {
    // Any poisoned argument means a mistake was already reported: produce a
    // poisoned result and add nothing.
    if args.iter().any(|a| checker.out.types.poisoned(*a)) {
        return Some(checker.error_ty());
    }
    let result = match (name, args) {
        // Panel 006's contract: any number of arguments, four types, canonical
        // rendering, no separator, one trailing newline.
        ("print", args) => {
            for (index, arg) in args.iter().enumerate() {
                if !matches!(checker.out.types.get(*arg), Ty::Int(_) | Ty::F64 | Ty::Bool | Ty::Str) {
                    let got = checker.show(ast, src, args[index]);
                    let diagnostic = errors::bad_operand(
                        "print",
                        "`i64`, `f64`, `bool` or `str`",
                        &got,
                        span,
                    );
                    checker.push_diagnostic(diagnostic);
                    return Some(checker.error_ty());
                }
            }
            checker.out.types.unit()
        }
        ("len", [one]) => match checker.out.types.get(*one) {
            Ty::Str | Ty::Array(_) | Ty::Map(_, _) => checker.out.types.int(),
            _ => return arg_error(checker, ast, src, "len", "`str`, `[T]` or `{K: V}`", *one, span),
        },
        ("push", [container, element]) => match checker.out.types.get(*container) {
            Ty::Array(item) if item == *element => *container,
            Ty::Array(item) => {
                let (want, got) = (checker.show(ast, src, item), checker.show(ast, src, *element));
                let diagnostic = errors::mismatch(&want, &got, span);
                checker.push_diagnostic(diagnostic);
                return Some(checker.error_ty());
            }
            _ => return arg_error(checker, ast, src, "push", "`[T]`", *container, span),
        },
        ("slice", [subject, from, to]) => {
            let int = checker.out.types.int();
            if *from != int || *to != int {
                let got = checker.show(ast, src, if *from == int { *to } else { *from });
                return arg_error_named(checker, "slice", "`i64` bounds", &got, span);
            }
            match checker.out.types.get(*subject) {
                Ty::Str | Ty::Array(_) => *subject,
                _ => return arg_error(checker, ast, src, "slice", "`str` or `[T]`", *subject, span),
            }
        }
        ("chars", [one]) if checker.out.types.get(*one) == Ty::Str => {
            let str_ty = checker.out.types.str();
            checker.out.types.intern(Ty::Array(str_ty))
        }
        // `keys(m) -> [K]`. `has` used to sit here and was struck (panel 026): map
        // access returns `V?` always, so `has(m, k)` and `!m[k].is_err()` were two
        // spellings of one predicate.
        ("keys", [map]) => match checker.out.types.get(*map) {
            Ty::Map(k, _) => checker.out.types.intern(Ty::Array(k)),
            _ => return arg_error(checker, ast, src, "keys", "`{K: V}`", *map, span),
        },
        ("join", [parts, separator]) => {
            let str_ty = checker.out.types.str();
            let of_str = checker.out.types.lookup(Ty::Array(str_ty));
            if Some(*parts) != of_str || *separator != str_ty {
                return arg_error(checker, ast, src, "join", "`[str]` and `str`", *parts, span);
            }
            str_ty
        }
        // §4.19's one conversion: a `str` lent to C for the duration of a call.
        // It is free rather than a copy, because every Heroes string allocates
        // `len+1` and is NUL-terminated (§4.20) — Zig's `[:0]u8` trick, and the
        // single highest-return decision in the string design.
        ("cstr", [one]) if checker.out.types.get(*one) == Ty::Str => {
            checker.out.types.intern(Ty::Cstr)
        }
        ("cstr", [one]) => {
            return arg_error(checker, ast, src, "cstr", "`str`", *one, span)
        }
        ("to_i64", [one]) if checker.out.types.get(*one) == Ty::F64 => checker.out.types.int(),
        // `fit_<width>(x) -> <width>?`. The source must be an integer of some
        // width — converting an `f64` is `to_i64`'s job and aborts, which is a
        // different question with a different answer (panel 042 Q2).
        (name, [one])
            if name.starts_with("fit_")
                && crate::types::INT_KINDS.iter().any(|k| k.name() == &name[4..]) =>
        {
            if !matches!(checker.out.types.get(*one), Ty::Int(_)) {
                return arg_error(checker, ast, src, name, "an integer", *one, span);
            }
            let kind = *crate::types::INT_KINDS
                .iter()
                .find(|k| k.name() == &name[4..])
                .expect("just matched");
            let source = match checker.out.types.get(*one) {
                Ty::Int(k) => k,
                _ => unreachable!("refused above"),
            };
            let target = checker.out.types.intern(Ty::Int(kind));
            // **A widening returns `T`, not `T?`** (pending author ratification;
            // the author's ruling was `T?` for every conversion, and the cost of
            // taking that literally showed up on the language's own acceptance
            // programme: `v * 10 + fit_i64(l.here() - '0').must()` in the
            // calculator's lexer, a `.must()` on something that cannot fail, at
            // the hottest line of the shape the closure list is made of).
            //
            // Panel 042's Q2 refused *one name with two result types*, and the
            // reason was unpredictability. This is not that: the result is a
            // function of the argument's width, the compiler knows it, and it is
            // fallible exactly when it can fail. The llm-ergonomist asked for this
            // and was overruled for uniformity; the measurement went its way.
            if kind.contains(source) {
                target
            } else {
                checker.out.types.intern(Ty::Fallible(target))
            }
        }
        ("to_f64", [one]) if matches!(checker.out.types.get(*one), Ty::Int(_)) => checker.out.types.f64(),
        // §4.20's inventory calls this one `.str()`; panel 017 renames it
        // `to_str`, so the three conversions share one scheme and a model can
        // derive the third from the two the spec already lists.
        // `cstr` is here and not in a conversion of its own: §4.19's boundary has
        // two directions, `.cstr()` out and this one back, and giving the return
        // path a new name would have cost spec tokens for a conversion the three
        // that exist already teach the shape of.
        ("to_str", [one]) => match checker.out.types.get(*one) {
            Ty::Int(_) | Ty::F64 | Ty::Bool | Ty::Str | Ty::Cstr => checker.out.types.str(),
            _ => {
                return arg_error(
                    checker, ast, src, "to_str", "`i64`, `f64`, `bool`, `str` or `cstr`", *one, span,
                )
            }
        },
        ("sort", [one]) => match checker.out.types.get(*one) {
            Ty::Array(_) => *one,
            _ => return arg_error(checker, ast, src, "sort", "`[T]`", *one, span),
        },
        // §4.6's three readers of a fallible value.
        ("must", [one]) => match checker.out.types.get(*one) {
            Ty::Fallible(inner) => inner,
            _ => return arg_error(checker, ast, src, "must", "a fallible value", *one, span),
        },
        ("default", [one, fallback]) => match checker.out.types.get(*one) {
            Ty::Fallible(inner) if inner == *fallback => inner,
            Ty::Fallible(inner) => {
                let (want, got) =
                    (checker.show(ast, src, inner), checker.show(ast, src, *fallback));
                let diagnostic = errors::mismatch(&want, &got, span);
                checker.push_diagnostic(diagnostic);
                return Some(checker.error_ty());
            }
            _ => return arg_error(checker, ast, src, "default", "a fallible value", *one, span),
        },
        ("is_err", [one]) => match checker.out.types.get(*one) {
            Ty::Fallible(_) => checker.out.types.bool(),
            _ => return arg_error(checker, ast, src, "is_err", "a fallible value", *one, span),
        },
        // **Tier 2 has no rules here at all now.** `range` lost its rule at M-generics-library
        // step 4 and the six higher-order ones at step 6: they are declarations in
        // `library/source.hero`, checked by the ordinary rules like anything else,
        // and `resolve/exprs.rs` consults the top-level table before this one — so
        // no `Callee::Builtin` is ever produced for them.
        //
        // That is §4.12's claim made good: without generics these were "seven
        // special cases in the type checker, each with hand-written rules", which
        // is what `Shape`, `higher_order` and `fold` were. They are gone.
        // The arity is wrong, or the built-in is one this milestone does not
        // type yet. Either way the caller reports it, so that "how many
        // arguments" is answered in one place.
        _ => return None,
    };
    Some(result)
}

fn arg_error(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    allowed: &str,
    got: TyId,
    span: Span,
) -> Option<TyId> {
    let got = checker.show(ast, src, got);
    arg_error_named(checker, name, allowed, &got, span)
}

fn arg_error_named(
    checker: &mut Checker,
    name: &str,
    allowed: &str,
    got: &str,
    span: Span,
) -> Option<TyId> {
    let diagnostic = errors::bad_operand(name, allowed, got, span);
    checker.push_diagnostic(diagnostic);
    Some(checker.error_ty())
}
