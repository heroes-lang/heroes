//! The built-ins' type rules (design.md §4.20's inventory, §1.11's two tiers).
//!
//! Hand-written rather than driven by a signature table, and that is the honest
//! shape: `print` takes any number of arguments of four kinds, `len` works on
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

use super::fallible_ops;
use super::ordering;
use super::table::Ty;
use crate::types::IntKind;
use super::{conversions, errors, Checker, TyId};

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
        // `repeat`'s count is a `u64`, so the literal in `repeat("-", 40)` takes
        // that width from here rather than defaulting to `i64` (panel 054).
        ("repeat", _, 1) => checker.out.types.lookup(Ty::Int(IntKind::U64)),
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
    // **The conversions are answered first, by `conversions.rs`** — the seam this
    // file was split along when `f32` pushed it past §11's ceiling. They are one
    // concern (what `to_<type>` means, a paragraph of the spec in its own right)
    // and the only built-ins whose *result type* is a function of the name rather
    // than of the argument, which is what makes them a family and the rest a list.
    if let Some(answer) = conversions::call(checker, ast, src, name, args, span) {
        return answer;
    }
    let result = match (name, args) {
        // Panel 006's contract: any number of arguments, four types, canonical
        // rendering, no separator, one trailing newline.
        ("print", args) => {
            for (index, arg) in args.iter().enumerate() {
                if !matches!(checker.out.types.get(*arg), Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Str) {
                    let got = checker.show(ast, src, args[index]);
                    // **The message names the set the arm above actually tests.**
                    // It said "`i64`, `f64`, `bool` or `str`" while `Ty::Int(_)`
                    // accepts all eight widths — measured at panel 043, in the
                    // milestone that added them: `u8`, `i16` and `u64` all print
                    // and none was listed. A message enumerating a set the code
                    // does not have is worse than no enumeration, because a
                    // reader who believes it converts for nothing.
                    let diagnostic = errors::bad_operand(
                        "print",
                        "any integer, a float, `bool` or `str`",
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
        // **`repeat(s: str, n: u64) -> str`, and the `u64` is the decision**
        // (panel 054). A negative count cannot be written, so the class does not
        // arise: Rust shipped `str::repeat`'s unchecked `len * count` as
        // CVE-2018-1000810, and Go's `strings.Repeat` panics on a negative count
        // in a shipped crash — oh-my-posh #4135 — from a renderer's padding
        // subtraction, which is exactly what `repeat(" ", col)` is.
        //
        // A computed count is `repeat(" ", col.to_u64().must())`, which aborts
        // with `does_not_fit` **at the subtraction that went negative** rather
        // than inside `repeat`. That is design.md §1.12's test: a defensive check
        // must surface a defect, not hide it — which is why returning `""` for a
        // negative count loses, despite `range(from: 0, to: -1)` doing exactly
        // that.
        ("repeat", [text, count]) => {
            let str_ty = checker.out.types.str();
            let u64_ty = checker.out.types.intern(Ty::Int(IntKind::U64));
            if *text != str_ty || *count != u64_ty {
                return arg_error(checker, ast, src, "repeat", "`str` and `u64`", *text, span);
            }
            str_ty
        }
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
        // **The element rule is here rather than in the emitter** (panel 068 R2,
        // ratified 2026-08-16). It used to be `emit/builtins.rs`'s, so `heroes
        // check` passed a program `heroes build` then refused — with a note saying
        // *"no change to this file will fix this"*, which is true of an unsupported
        // form and false of this one: changing the element type fixes it exactly.
        // `ordering.rs` holds what may be ordered and why.
        ("sort", [one]) => match checker.out.types.get(*one) {
            Ty::Array(element) if ordering::is_refusable(checker, element) => {
                let shown = checker.show(ast, src, element);
                let inside = ordering::why_unordered(checker, ast, src, element);
                let diagnostic = errors::unordered_element(&shown, inside, span);
                checker.push_diagnostic(diagnostic);
                return Some(checker.error_ty());
            }
            Ty::Array(_) => *one,
            _ => return arg_error(checker, ast, src, "sort", "`[T]`", *one, span),
        },
        // §4.6's three readers of a fallible value.
        ("must" | "default" | "is_err", _) => {
            return fallible_ops::builtin(checker, ast, src, name, args, span)
        }
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

pub(super) fn arg_error(
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
