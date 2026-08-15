//! A value the C type system gives **no storage** — today, a fixed array
//! (design.md §3.1; panel 062).
//!
//! Split from `aggregate.rs` on 2026-08-15, and it is a concern rather than an
//! overflow: everything else in that file names a place or builds a value that has
//! one. This names the values that have none.
//!
//! **It is `ctype.rs`'s unit rule, second instance.** `()` is never declared
//! because `void t0;` is a hard error; a `T[N]` is never declared because C has no
//! assignable array — `float t[4]; t = …;` compiles at no optimisation level. Both
//! are real values with no storage, and both are rendered where they are used
//! rather than assigned. A third would want this file rather than another arm.

use crate::types::Ty;

use super::aggregate::Types;
use super::mangle;

/// The **text** of a value whose C type has no storage — today, a fixed array.
///
/// **The second instance of `ctype.rs`'s unit rule, and the reason it needs a
/// function rather than a temporary** (panel 062). C has no assignable array:
/// `float t[4]; t = …;` does not compile at any optimisation level. So a
/// `Ty::Fixed` value is never declared, never assigned, and its instruction emits
/// nothing — what stands in its place is the expression it came from, rendered
/// where it is used.
///
/// Two producers, and both must be here rather than at their use sites: a literal
/// (`{a, b, c}`) and a field read (`v.params`). A third would be an index into an
/// array of arrays, which the checker refuses today.
///
/// **The field read keeps the subscript on an array-typed lvalue**, and that is
/// load-bearing rather than incidental: panel 062's historian established that
/// ASan cannot see an intra-object overflow in a **C struct** at all
/// (`-fsanitize-address-field-padding` is documented for C++ non-standard-layout
/// classes with a destructor, never for C), so `--sanitize`'s only arm on
/// `v.params[i]` is UBSan's `array-bounds` — which sees it **only** while the
/// subscript sits on something whose C type is still `T[N]`. Lower it to
/// `*((T *)&v.params + i)` and the sanitiser reports clean while the next field is
/// corrupted.
pub(super) fn fixed_text(
    types: &Types,
    function: &crate::ir::Function,
    value: crate::ir::ValueId,
) -> Option<String> {
    if !matches!(types.checked.types.get(function.values[value.0 as usize]), Ty::Fixed(_, _)) {
        return None;
    }
    let inst = function
        .blocks
        .iter()
        .flat_map(|block| &block.insts)
        .find(|inst| inst.dest == Some(value))?;
    match &inst.op {
        crate::ir::Op::Construct { shape: crate::ir::Shape::Array, args } => {
            let parts: Vec<String> = function
                .args_of(*args)
                .into_iter()
                .filter_map(|arg| match arg {
                    crate::ir::Arg::Value(v) => {
                        Some(fixed_text(types, function, v).unwrap_or_else(|| mangle::value(v.0)))
                    }
                    crate::ir::Arg::InOut(_) => None,
                })
                .collect();
            Some(format!("{{{}}}", parts.join(", ")))
        }
        crate::ir::Op::Field { base, index } => {
            let owner = function.values[base.0 as usize];
            let (member, _) = types.field(owner, *index)?;
            let base_text =
                fixed_text(types, function, *base).unwrap_or_else(|| mangle::value(base.0));
            Some(format!("{base_text}.{member}"))
        }
        _ => None,
    }
}
