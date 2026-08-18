//! `[T]` and `{K: V}` as C: built through the runtime, read through a descriptor,
//! written with one unshare per step (design.md §4.9, §4.10; panel 022).
//!
//! Together in one file because they are the same mechanism wearing two names. Neither
//! is a C aggregate: both are a pointer to a runtime header, so every operation goes
//! through `runtime/` and every element or pair crosses that boundary **by address**,
//! copied through its `HeroDesc` rather than assigned. That is what lets one runtime
//! function insert a `str` key and a `Point` value.
//!
//! The two rules worth reading before changing anything here:
//!
//! **Copy-on-write is one unshare per step of a mutated place** (panel 022, measured).
//! Not one at the primitive — that lets a nested store alias, and every instrument in
//! this project reported success while it did.
//!
//! **A map step ends a place walk and an array step does not.** `m[k] @ v` inserts, so
//! there is no element to descend into; `xs[i] @ v` has one, and descending means
//! unsharing this level first.
//!
//! What is *not* here: `hero_array_at`'s bounds check and `hero_map_find`'s NULL are
//! the runtime's, and the abort they raise is spec line 126's. This file only spells
//! the calls.

use crate::ir::{is_refcounted, Function, Place, Step, ValueId};
use crate::types::{Ty, TyId};

use super::aggregate::Types;
use super::mangle;

// --- arrays ---------------------------------------------------------------

/// `t4 = hero_array_new(&hero_desc_int, 3);` plus one push per element.
///
/// Built by pushing rather than by writing the elements directly, because `push` is
/// where the element's `copy` runs — and that copy is what takes the array's own
/// reference to a counted element. Writing the bytes would share the caller's
/// reference without counting it.
///
/// Each push hands back a NEW array and the previous one is released, so a literal of
/// n elements allocates n+1 blocks and frees n. That is §4.10's declared bill in its
/// smallest form; performance is a non-goal (Part 2), and the alternative — a `push`
/// that appends in place — has no reading under value semantics, because a refcount of
/// 1 means "the slot has it" and appending would change what the slot sees.
pub(super) fn build_array(
    types: &Types,
    result: TyId,
    arguments: &[String],
    into: &str,
) -> Option<Vec<String>> {
    let element = match types.checked.types.get(result) {
        Ty::Array(element) => element,
        _ => return None,
    };
    let desc = super::descriptors::pointer(types.checked, types.names, element)?;
    let mut lines = vec![format!("{into} = hero_array_new({desc}, {});", arguments.len().max(1))];
    for value in arguments {
        // The temporary the push returns replaces the one it grew from, and the old one
        // is released — one line each, and no temporary left owning anything.
        lines.push(format!("{{ HeroArrayHeader *grown = hero_array_push({into}, &{value});"));
        lines.push(format!("  hero_array_decref({into}); {into} = grown; }}"));
    }
    Some(lines)
}

/// `t9 = *(const int64_t *)hero_array_at(t7, t8);`
///
/// The read goes through the runtime because that is where the bounds check lives:
/// spec line 126 says an out-of-bounds index aborts, and §4.9 says it never reads
/// arbitrary memory. The cast is on the *result*, so the element type is stated at
/// every read and clang checks the assignment.
pub(super) fn read_element(
    types: &Types,
    function: &Function,
    base: ValueId,
    index: ValueId,
) -> Option<String> {
    // **A fixed array is subscripted directly, and guarded here** (panel 062).
    //
    // Two things are load-bearing and both were measured. The subscript stays on
    // an **array-typed lvalue** — `v.params[i]`, never `*((T *)&v.params + i)` —
    // because that is the only form `-fsanitize=undefined`'s `array-bounds` can
    // see; ASan is documented blind to an intra-object overflow in a **C** struct
    // (`-fsanitize-address-field-padding` is C++-only), so UBSan is `--sanitize`'s
    // only arm on this. And the **guard is the compiler's**, because
    // `-Warray-bounds` was measured not to fire on `int64_t i = 7; arr[i]` at
    // `-O0` or `-O2` — and this emitter always writes a temporary, so clang can
    // never see a constant here. The cost was measured at a 0.94–0.95 ratio over
    // four million opaque indices: under the noise floor.
    //
    // The comparison is unsigned so one test catches both ends: a negative index
    // becomes huge and fails the same `>=`.
    if let Ty::Fixed(element, n) = types.checked.types.get(function.value_type(base)) {
        let _ = element;
        let base_text = super::storageless::fixed_text(types, function, base)
            .unwrap_or_else(|| mangle::value(base.0));
        let i = mangle::value(index.0);
        return Some(format!(
            "{base_text}[((uint64_t)({i}) >= UINT64_C({n}) ? (hero_panic(\"index out of range for a fixed array\"), (int64_t)0) : ({i}))]"
        ));
    }
    let element = match types.checked.types.get(function.value_type(base)) {
        Ty::Array(element) => element,
        _ => return None,
    };
    let spelling = super::ctype::c_type(types.names, types.checked, element)?;
    // `{spelling} const *`, not `const {spelling} *`: the two differ exactly when the
    // element is itself a pointer. For `[[i64]]` the element spelling is
    // `HeroArrayHeader *`, and the prefix form reads as pointer-to-pointer-to-const,
    // whose dereference is a `const HeroArrayHeader *` — assigning that to the
    // temporary is `-Wincompatible-pointer-types-discards-qualifiers`. The suffix form
    // says what is meant: a const pointer to the element, whatever the element is.
    Some(format!(
        "*({spelling} const *)hero_array_at({}, {})",
        mangle::value(base.0),
        mangle::value(index.0)
    ))
}

/// `xs[i] @ v`, and every deeper spelling of it: **one unshare per array step of the
/// place, each writing back at its own level** (panel 022).
///
/// The walk keeps a C *lvalue* for the place reached so far rather than an address,
/// because a field step is `.member` on an lvalue and `&` is only needed where a
/// primitive takes one. For `g.rows[0].cells[0] @ 7`:
///
/// ```text
///   h0_g                                        the slot, ours already
///   h0_g.f_rows                                 a field: no unshare, we own the record
///   hero_array_unshare(&(h0_g.f_rows));         an array step: make THIS level unique
///   (*(h_m_Row *)hero_array_at_mut(h0_g.f_rows, t12))          descend into the element
///   ....f_cells                                 a field of the element
///   hero_array_unshare(&(....f_cells));         the second array step, unique too
///   hero_array_set(&(....f_cells), t13, &t14);  and the write, which unshares again
/// ```
///
/// The last step's `set` unshares a third time and that is deliberate: it is a
/// refcount test, it is idempotent, and having one entry point that cannot be reached
/// without it beats an emitter that has to remember.
pub(super) fn write_element(
    types: &Types,
    function: &Function,
    at: Place,
    value: ValueId,
) -> Option<Vec<String>> {
    let slot = &function.slots[at.root.0 as usize];
    let mut lvalue = mangle::slot(at.root.0, &slot.name);
    let mut ty = slot.ty;
    let steps = function.steps_of(at.path);
    let mut lines: Vec<String> = Vec::new();
    for (position, step) in steps.iter().enumerate() {
        let last = position + 1 == steps.len();
        match step {
            Step::Field(index) => {
                let (member, next) = types.field(ty, *index)?;
                lvalue = format!("{lvalue}.{member}");
                ty = next;
                // A field is not an indirection: the record is inside a place we
                // already own, so nothing to unshare. A trailing field store is a
                // plain assignment — done HERE when the path holds an index,
                // because the array levels above it were unshared on the way
                // down. (Until 2026-08-17 this arm returned None on `last`,
                // claiming such a store never reaches this function; that is
                // true only of an index-free path, and `xs[i].f @ v` — first
                // written by the ported resolver — emitted hero_unreachable at
                // exit 0's price: a runtime abort on a legal program.)
                if last {
                    // `hero_array_set` releases the old ELEMENT through its
                    // descriptor; this store replaces one FIELD of it, so
                    // releasing the old value is this line's own job —
                    // measured: without it, a heap `str` overwritten here was
                    // `1 heap blocks still live at exit`.
                    if is_refcounted(types.checked, ty) {
                        let release = match types.checked.types.get(ty) {
                            Ty::Str => format!("hero_str_decref({lvalue});"),
                            Ty::Array(_) => format!("hero_array_decref({lvalue});"),
                            Ty::Map(_, _) => format!("hero_map_decref({lvalue});"),
                            Ty::Failure => format!("hero_failure_release(&({lvalue}));"),
                            _ => {
                                let name = types.satellite_name(ty)?;
                                format!("{name}_release(&({lvalue}));")
                            }
                        };
                        lines.push(release);
                    }
                    lines.push(format!("{lvalue} = {};", mangle::value(value.0)));
                    return Some(lines);
                }
            }
            Step::Index(index) => {
                // **A map step ends the walk.** `m[k] @ v` inserts, so there is no
                // element to descend into — a key that is absent is created by the
                // store itself. A map step that is *not* last would mean writing
                // through a value the map may not hold, which the checker refuses.
                if let Ty::Map(_, _) = types.checked.types.get(ty) {
                    if !last {
                        return None;
                    }
                    lines.push(format!(
                        "hero_map_set(&({lvalue}), &{}, &{});",
                        mangle::value(index.0),
                        mangle::value(value.0)
                    ));
                    return Some(lines);
                }
                let element = match types.checked.types.get(ty) {
                    Ty::Array(element) => element,
                    _ => return None,
                };
                if last {
                    lines.push(format!(
                        "hero_array_set(&({lvalue}), {}, &{});",
                        mangle::value(index.0),
                        mangle::value(value.0)
                    ));
                    return Some(lines);
                }
                lines.push(format!("hero_array_unshare(&({lvalue}));"));
                let spelling = super::ctype::c_type(types.names, types.checked, element)?;
                lvalue = format!(
                    "(*({spelling} *)hero_array_at_mut({lvalue}, {}))",
                    mangle::value(index.0)
                );
                ty = element;
            }
        }
    }
    None
}
// --- `{K: V}` -------------------------------------------------------------

/// `t5 = hero_map_new(&hero_desc_str, &hero_desc_int, 2);` plus one `put` per pair.
///
/// The arguments alternate key, value (`ir/inst.rs`'s own shape for `Shape::Map`), and
/// both go in **by address**: the runtime copies them through their descriptors, which
/// is the only way one function can insert a `str` key and a `Point` value.
pub(super) fn build_map(
    types: &Types,
    result: TyId,
    arguments: &[String],
    into: &str,
) -> Option<Vec<String>> {
    let (key, value) = match types.checked.types.get(result) {
        Ty::Map(key, value) => (key, value),
        _ => return None,
    };
    let key_desc = super::descriptors::pointer(types.checked, types.names, key)?;
    let value_desc = super::descriptors::pointer(types.checked, types.names, value)?;
    let pairs = arguments.len() / 2;
    let mut lines = vec![format!("{into} = hero_map_new({key_desc}, {value_desc}, {pairs});")];
    for pair in arguments.chunks(2) {
        if pair.len() == 2 {
            lines.push(format!("hero_map_put({into}, &{}, &{});", pair[0], pair[1]));
        }
    }
    Some(lines)
}

/// `m[k]`, which is **not** `xs[i]`: it yields a `V?` and cannot abort (§4.9).
///
/// The runtime hands back the value's address or NULL, because it cannot build the
/// option — that struct is generated per payload type. So the wrapping is here, and the
/// found value is copied through its descriptor rather than assigned: the map still
/// owns its copy, and the `V?` needs one of its own.
pub(super) fn map_get(
    types: &Types,
    function: &Function,
    map: ValueId,
    key: ValueId,
    result: TyId,
    into: &str,
) -> Option<Vec<String>> {
    let value = match types.checked.types.get(function.value_type(map)) {
        Ty::Map(_, value) => value,
        _ => return None,
    };
    let option = types.names.option_of(result);
    let mut lines = vec![
        "{".to_string(),
        format!(
            "  const void *found = hero_map_find({}, &{});",
            mangle::value(map.0),
            mangle::value(key.0)
        ),
        "  if (found == NULL) {".to_string(),
        format!(
            "    {into} = ({option}){{.tag = INT64_C(1), .as.err = hero_failure_missing_key()}};"
        ),
        "  } else {".to_string(),
        format!("    {into}.tag = INT64_C(0);"),
    ];
    // A unit value type has no `ok` member at all (`ctype.rs`'s unit rule), so the tag
    // is the whole answer.
    if super::ctype::c_type(types.names, types.checked, value).is_some() {
        let desc = super::descriptors::pointer(types.checked, types.names, value)?;
        lines.push(format!("    ({desc})->copy(&{into}.as.ok, found);"));
    }
    lines.push("  }".to_string());
    lines.push("}".to_string());
    Some(lines)
}
