//! `T?` as C: two alternatives, a tag, and never a guess about which one is live
//! (design.md §4.6, §4.8).
//!
//! Its own file rather than a corner of `aggregate.rs`, because a `T?` is the one
//! aggregate whose members are **alternatives**: a record's fields all exist at once, and
//! reading the wrong one of these is reading a union member that was never written. Every
//! function here writes the tag explicitly — C would zero it for `ok` — so that a reader
//! of the generated C sees which side this is without knowing C's initialiser rules.
//!
//! `?` propagates a failure UNCHANGED (§4.6): nothing re-reads its code or msg on the
//! way out, which is why `Shape::Err` is one struct assignment and not a rebuild.

use crate::ir::ValueId;
use crate::types::TyId;

use super::aggregate::Types;
use super::mangle;

// --- `T?` -----------------------------------------------------------------

/// `ok(x)`, `fail(code, msg)` and the `err` that `?` produces (design.md §4.6).
///
/// One compound literal each, and the tag is written explicitly even though C would
/// zero it for `ok`: the two sides of a `T?` are the whole point of the type, and a
/// reader of the generated C should not have to know C's initialiser rules to see
/// which one this is.
pub(super) fn construct_option(
    types: &Types,
    result: TyId,
    shape: crate::ir::Shape,
    arguments: &[String],
) -> Option<String> {
    use crate::ir::Shape;
    let name = types.names.option_of(result);
    match shape {
        Shape::Ok => {
            let value = arguments.first();
            match value {
                Some(text) => Some(format!("({name}){{.tag = INT64_C(0), .as.ok = {text}}}")),
                // `ok(())` — a unit payload has no member at all, so the tag is the
                // whole value.
                None => Some(format!("({name}){{.tag = INT64_C(0)}}")),
            }
        }
        Shape::Fail => {
            let code = arguments.first()?;
            let msg = arguments.get(1)?;
            Some(format!(
                "({name}){{.tag = INT64_C(1), .as.err = {{.code = {code}, .msg = {msg}}}}}"
            ))
        }
        // `?` propagates the failure UNCHANGED into the caller's `T?`: nothing re-reads
        // its code and msg on the way (§4.6), so this is one struct assignment.
        Shape::Err => {
            let failure = arguments.first()?;
            Some(format!("({name}){{.tag = INT64_C(1), .as.err = {failure}}}"))
        }
        _ => None,
    }
}

/// `t16 = t15.tag;` for a `T?`, the same member a variant uses.
pub(super) fn option_tag(base: ValueId) -> String {
    format!("{}.tag", mangle::value(base.0))
}

/// `.as.ok` for case 0 and `.as.err` for case 1 — `ir/inst.rs`'s own numbering.
pub(super) fn option_payload(base: ValueId, case: u32) -> String {
    let member = if case == 0 { "ok" } else { "err" };
    format!("{}.as.{member}", mangle::value(base.0))
}
