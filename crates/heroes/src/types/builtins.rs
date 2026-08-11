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
//! that can be relaxed (`to_int` on a `str` is not offered, because parsing can
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
                if !matches!(checker.out.types.get(*arg), Ty::Int | Ty::F64 | Ty::Bool | Ty::Str) {
                    let got = checker.show(ast, src, args[index]);
                    let diagnostic = errors::bad_operand(
                        "print",
                        "`int`, `f64`, `bool` or `str`",
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
                return arg_error_named(checker, "slice", "`int` bounds", &got, span);
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
        ("to_int", [one]) if checker.out.types.get(*one) == Ty::F64 => checker.out.types.int(),
        ("to_f64", [one]) if checker.out.types.get(*one) == Ty::Int => checker.out.types.f64(),
        // §4.20's inventory calls this one `.str()`; panel 017 renames it
        // `to_str`, so the three conversions share one scheme and a model can
        // derive the third from the two the spec already lists.
        ("to_str", [one]) => match checker.out.types.get(*one) {
            Ty::Int | Ty::F64 | Ty::Bool | Ty::Str => checker.out.types.str(),
            _ => return arg_error(checker, ast, src, "to_str", "`int`, `f64`, `bool` or `str`", *one, span),
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
        // Tier 2 (§1.11): written in Heroes. `range` used to have a rule here and
        // no longer does — it is a real declaration in `library/source.hero` from
        // M6 step 4, and `resolve/exprs.rs` consults the top-level table before
        // the built-in one, so no `Callee::Builtin(range)` is ever produced. The
        // six below still have rules because they need generics and function
        // values, which land later in this milestone.
        ("map", [items, f]) => return higher_order(checker, ast, src, "map", *items, *f, span, Shape::Map),
        ("filter", [items, f]) => {
            return higher_order(checker, ast, src, "filter", *items, *f, span, Shape::Keep)
        }
        ("find", [items, f]) => {
            return higher_order(checker, ast, src, "find", *items, *f, span, Shape::Find)
        }
        ("any", [items, f]) => {
            return higher_order(checker, ast, src, "any", *items, *f, span, Shape::Test)
        }
        ("all", [items, f]) => {
            return higher_order(checker, ast, src, "all", *items, *f, span, Shape::Test)
        }
        ("fold", [items, start, f]) => {
            return fold(checker, ast, src, *items, *start, *f, span)
        }
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

enum Shape {
    /// `map([A], (function(A) -> B)) -> [B]`
    Map,
    /// `filter([A], (function(A) -> bool)) -> [A]`
    Keep,
    /// `find([A], (function(A) -> bool)) -> A?`
    Find,
    /// `any`/`all`
    Test,
}

fn higher_order(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    items: TyId,
    f: TyId,
    span: Span,
    shape: Shape,
) -> Option<TyId> {
    let Ty::Array(element) = checker.out.types.get(items) else {
        return arg_error(checker, ast, src, name, "`[T]`", items, span);
    };
    let Ty::Func { params, result } = checker.out.types.get(f) else {
        return arg_error(checker, ast, src, name, "a function", f, span);
    };
    let params = checker.out.types.params_of(params);
    if params.len() != 1 || params[0] != element {
        return arg_error(checker, ast, src, name, "a function of the element type", f, span);
    }
    let bool_ty = checker.out.types.bool();
    let result = match shape {
        Shape::Map => checker.out.types.intern(Ty::Array(result)),
        Shape::Keep | Shape::Find | Shape::Test => {
            if result != bool_ty {
                return arg_error(checker, ast, src, name, "a function returning `bool`", f, span);
            }
            match shape {
                Shape::Keep => items,
                Shape::Find => checker.out.types.intern(Ty::Fallible(element)),
                _ => bool_ty,
            }
        }
    };
    Some(result)
}

/// `fold([A], B, (function(B, A) -> B)) -> B`
fn fold(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    items: TyId,
    start: TyId,
    f: TyId,
    span: Span,
) -> Option<TyId> {
    let Ty::Array(element) = checker.out.types.get(items) else {
        return arg_error(checker, ast, src, "fold", "`[T]`", items, span);
    };
    let Ty::Func { params, result } = checker.out.types.get(f) else {
        return arg_error(checker, ast, src, "fold", "a function", f, span);
    };
    let params = checker.out.types.params_of(params);
    if params.len() != 2 || params[0] != start || params[1] != element || result != start {
        return arg_error(
            checker,
            ast,
            src,
            "fold",
            "a function `(function(B, A) -> B)` matching the accumulator",
            f,
            span,
        );
    }
    Some(start)
}
