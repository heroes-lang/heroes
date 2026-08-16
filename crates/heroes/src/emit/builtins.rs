//! Which built-ins reach C, and the entry point each one becomes.
//!
//! One file for one question, because the list and the table are the same
//! knowledge and drifting apart is the failure this project keeps meeting: the
//! gate refuses by *name*, `ops.rs` emits by *name and operand type*, and a name
//! in one and not the other is either a refusal nobody can lift or generated C
//! that clang rejects with the compiler blaming itself (panel 025 found exactly
//! that for `slice`).
//!
//! **The runtime is monomorphic and the emitter composes it** (panel 006, for
//! `print`). So one Heroes name can stand over several C functions — `len` over
//! three, `to_str` over four, `slice` over two, `sort` over one that dispatches
//! internally — and the choice is always made from the type of the first
//! argument, never from anything the runtime inspects at run time.
//!
//! Design: design.md §4.20 (the inventory), §1.11 (the two tiers), CLAUDE.md §7.

use std::collections::BTreeSet;

use crate::ir::{Arg, Args, Callee, Function, Op, Program};
use crate::source::Source;
use crate::types::{Checked, Ty};
use crate::types::FloatKind;

/// The built-ins the backend emits. The gate reads this list, the note that
/// tells a reader what *is* supported reads it, and `entry` below is exhaustive
/// over it — so the three cannot disagree. CLAUDE.md §10's own pattern ("one
/// argv table parses and prints the help"), applied to the backend.
///
/// Tier 2 (§1.11) is absent by construction: `map`, `filter`, `fold`, `find`,
/// `any`, `all` and `range` are written in Heroes, so they have no entry point
/// to name here — they arrive as source, in the prelude.
pub const EMITTED: [&str; 20] = [
    "chars", "join", "keys", "len", "print", "push", "repeat", "slice", "sort", "to_f32", "to_f64", "to_i64",
    "to_str",
    // The width conversions. They are on this list and NOT in `entry` below,
    // because `entry` answers with the name of a C entry point and a `to_<width>`
    // has none: it builds a `T?`, a union generated for that result type.
    // `ops::call` writes it inline, the way it writes `print`. `to_i64` is in the
    // row above because it was there before the family took the `to_` scheme.
    "to_i8", "to_i16", "to_i32", "to_u8", "to_u16", "to_u32", "to_u64",
];

/// The type of a call's first argument, where there is one and it is a value.
/// `None` covers a zero-argument call and an `@` argument — neither of which any
/// built-in in the list above has, and both of which fall to the default arm
/// rather than to a panic, because the gate has already refused anything else.
fn first_type(function: &Function, checked: &Checked, args: Args) -> Option<Ty> {
    match function.args_of(args).first() {
        Some(Arg::Value(value)) => Some(checked.types.get(function.value_type(*value))),
        _ => None,
    }
}

/// The C function this call becomes.
///
/// Exhaustive over [`EMITTED`] minus `print`, which is not a call at all but a
/// sequence of one printer per argument (see `ops::print`). A name that reaches
/// here without a row is a gate that let something through, so the fallback is
/// `hero_unreachable` rather than a guess.
pub(super) fn entry(
    name: &str,
    function: &Function,
    checked: &Checked,
    args: Args,
) -> &'static str {
    let first = first_type(function, checked, args);
    match name {
        // Three containers, three counts, one Heroes name.
        "len" => match first {
            Some(Ty::Map(_, _)) => "hero_map_len",
            Some(Ty::Array(_)) => "hero_array_len",
            _ => "hero_str_len",
        },
        // `keys(m)` hands back a fresh array whose keys are copied through the key
        // descriptor, so it owns them and may outlive the map.
        "keys" => "hero_map_keys",
        // The two halves that did not land together, and whose gap became a filed
        // defect: `slice` on an array type-checked and emitted `hero_str_slice`,
        // which clang rejected as exit 2 (panel 025). Both halves are here now.
        "slice" => match first {
            Some(Ty::Array(_)) => "hero_array_slice",
            _ => "hero_str_slice",
        },
        // `push` hands back a NEW array, always: `xs = [1,2,3]` leaves `xs`
        // observable, its slot holds one reference, so a refcount of 1 means "only
        // the slot has it" and appending in place would change what the slot sees.
        // Value semantics has no reading in which the argument is consumed.
        // §4.10's declared bill, in its smallest form — and `sort` is the same
        // shape for the same reason.
        "push" => "hero_array_push",
        // One entry point for all three orderable element types: the comparison is
        // chosen inside `runtime.c` by pointer identity against the three static
        // descriptors. That indirection stays out of the ABI deliberately — panel
        // 027 vetoed a `cmp` in `HeroDesc`, because C11 zero-fills a short
        // initialiser list and every descriptor that forgot the field would carry
        // a NULL and SEGV with no type name.
        "sort" => "hero_array_sort",
        "chars" => "hero_str_chars",
        "join" => "hero_str_join",
        "repeat" => "hero_str_repeat",
        "to_i64" => "hero_f64_to_int",
        // **Dispatched on the source, like `to_str` below**, because one Heroes
        // name covers two C conversions once there are two float widths. An `f32`
        // source reaching `hero_int_to_f64` would be C converting a float to an
        // `int64_t` and back — the silent wrong answer this dispatch exists to
        // stop.
        "to_f64" => match first {
            Some(Ty::Float(_)) => "hero_f32_to_f64",
            _ => "hero_int_to_f64",
        },
        "to_f32" => match first {
            Some(Ty::Float(_)) => "hero_f64_to_f32",
            _ => "hero_int_to_f32",
        },
        // `to_str` is one Heroes name over five C entry points, chosen by the
        // argument's type — the same shape as `print`, for the same reason.
        //
        // **`cstr` is the fifth, and it is the boundary's return path** (§4.19,
        // M-ffi-ladder). `.cstr()` lends a `str` to C; this copies one back, through
        // the `hero_str_from_*` primitives §4.20 says exist for exactly this —
        // "without them no `extern function` may return `str` and §4.19's ladder is
        // unwritable at step 3, read a result, because every C library returns
        // strings as borrowed pointers". Predicted by panel 036's ffi-pragmatist
        // before the ladder reached a library that returns one.
        "to_str" => match first {
            Some(Ty::Cstr) => "hero_str_from_cstr",
            Some(ty) => to_str_entry(&ty).unwrap_or("hero_unreachable"),
            None => "hero_unreachable",
        },
        _ => "hero_unreachable",
    }
}

/// A built-in whose *name* emits but whose **operand type** has no entry point.
///
/// The class was found by a judge pricing a spec sentence rather than by any
/// test: `slice(xs, from: 1, to: 3)` on an array passed `check` at exit 0 and
/// then emitted `hero_str_slice` on a `HeroArrayHeader *`, which clang rejects —
/// reported as exit 2, the compiler blaming itself for a program the author is
/// entitled to write (panel 025). `slice`'s array half has landed, so the row now
/// carries `sort`, and it is the same shape for a different reason.
///
/// **`sort`'s element rule left this file at panel 068** (R2, ratified
/// 2026-08-16). It lives in `types/ordering.rs` now, and what stays here is a
/// backstop for the one shape the checker cannot see.
///
/// The old reasoning — recorded because it was overturned rather than forgotten —
/// was that a checker rule would make the spec's own map-walking idiom a compile
/// error, so the gate should say *"this backend does not emit it"* instead. Panel
/// 068 found the premise inverted: `spec:85` **was the sentence with the bug**, it
/// was repaired in that sitting, and meanwhile `heroes check` was passing programs
/// `heroes build` refused, under a note reading *"no change to this file will fix
/// this"* — which is true of an unsupported form and false of this one.
///
/// **What is left, and why it has not moved.** Inside `function first<A>(xs: [A])`,
/// `sort(xs)` has an element type that is not yet a type; `first([3, 1, 2])` runs
/// and prints today, so refusing the generic body would delete a working program.
/// So this arm is the only thing standing between that program and `hero_cmp_for`
/// returning `NULL`, and it is kept deliberately.
///
/// **"Visible only after monomorphisation" stood here and was false** (panel 082
/// R4): `types/apply.rs:139` records instantiations inside `types::check`, keyed by
/// the **call-site** span, so a checker pass is possible (~90–110 lines) and points
/// where the author can edit. Unbuilt: `selfhost/` declares zero generics.
///
/// Its message is *not* the checker's, and must not be: this really is a form the
/// backend does not emit, reached through a generic the author may not have
/// written. Making the two identical would tell a reader to change an element type
/// that is spelled `A`.
///
/// The queued question is whether that shape gets a diagnostic of its own — it is
/// architecture (does `check` run the IR?), so it is a panel path and is on
/// `DECIDE.md` rather than decided here.
pub(super) fn unsupported_operand(
    name: &str,
    function: &Function,
    checked: &Checked,
    args: Args,
) -> Option<String> {
    if name != "sort" {
        return None;
    }
    match first_type(function, checked, args) {
        // Exactly the rows `hero_cmp_for` dispatches on — every integer width, both
        // floats, `str`, and `bool` since panel 068. The old list here said "`i64`,
        // `f64` or `str`" and had been stale since panel 042 added the seven widths,
        // which is why the set is now stated once, in the message, from the same
        // reading of the runtime that `types/ordering.rs` states it from.
        Some(Ty::Array(element)) => match checked.types.get(element) {
            Ty::Int(_) | Ty::Float(_) | Ty::Str | Ty::Bool => None,
            // A noun phrase, because `gate.rs` completes it with "is not emitted
            // yet". It names the *route* rather than the type: the author wrote
            // `[A]`, and telling them their element type is unordered would send
            // them to a line where the type is spelled with a letter.
            _ => Some(
                "the built-in `sort` on a generic element the call instantiated at a type \
                 with no order (it orders numbers, `str` and `bool`)"
                    .to_string(),
            ),
        },
        // Not an array at all: the checker reported that, and one mistake gets one
        // message.
        _ => None,
    }
}

/// Which library functions this program actually reaches.
///
/// The library is compiled with every program (§1.11, `crate::library`), but a
/// program that never calls `range` has no business carrying its definition:
/// `--emit-c` is an artifact the author reads (CLAUDE.md §10), and seven unused
/// definitions in every file would be seven pieces of noise about a library they
/// cannot change. So the emitter walks the call graph from the author's own
/// functions and emits what it finds.
///
/// A worklist rather than one pass, because a library function may call another —
/// and the day one does, a single pass would emit the caller and drop the callee,
/// which is an undefined symbol at link rather than a wrong answer, but still a
/// failure this walk makes impossible.
pub(super) fn reachable(program: &Program, src: &Source) -> BTreeSet<u32> {
    let mut seen: BTreeSet<u32> = BTreeSet::new();
    let mut work: Vec<u32> = Vec::new();
    for function in &program.functions {
        if !src.is_library(function.span.start) {
            for callee in calls_of(function) {
                work.push(callee);
            }
        }
    }
    while let Some(decl) = work.pop() {
        if !seen.insert(decl) {
            continue;
        }
        let Some(function) = program.functions.iter().find(|f| f.decl == decl) else { continue };
        for callee in calls_of(function) {
            work.push(callee);
        }
    }
    seen
}

/// Every Heroes function this one names. Read off the instructions, never a count
/// kept in step with them.
///
/// **Names**, not calls. `Op::FuncRef` is the other way a function's symbol reaches
/// the C — §4.13's "top-level functions are values" — and until 2026-08-12 this
/// walk matched only `Op::Call`, so a library function passed as a value was
/// referenced and never defined: `use of undeclared identifier 'h_library_range'`,
/// exit 2, on a legal program. One *direct* call from any module in the program
/// put the definition back, which is what made it root-dependent as well as wrong.
fn calls_of(function: &Function) -> Vec<u32> {
    let mut found = Vec::new();
    for block in &function.blocks {
        for inst in &block.insts {
            match inst.op {
                Op::Call { callee: Callee::Heroes(decl), .. }
                | Op::FuncRef(Callee::Heroes(decl)) => found.push(decl),
                _ => {}
            }
        }
    }
    found
}

/// Which runtime entry point turns a value of this type into a `str`.
///
/// **One reader, because two disagreed.** `print(x)` reaches this through
/// `abort.rs` and `x.to_str()` through the table above, and until panel 052 the
/// second was a `_ => "hero_int_to_str"` catch-all. So `print(n)` for a `u64` of
/// 2^64−1 printed `18446744073709551615` and `print(n.to_str())` printed `-1` —
/// the exact value panel 042 records as the defect `hero_uint_to_str` was added to
/// fix, fixed on one path and not the other, for two milestones. Found by
/// `-Werror=sign-conversion`, which made the emitter's own `uint64_t` argument to
/// an `int64_t` parameter a hard error.
///
/// The match over `IntKind` is **exhaustive on purpose** (CLAUDE.md §11: put the
/// fallback in the loud direction). Six of the seven narrow widths widen into an
/// `int64_t` without losing a value and share the signed entry point; `u64` does
/// not, because 2^64−1 read as signed is −1. A new width has to answer here, and
/// answering wrongly is a compile error in Rust rather than a wrong number in C.
pub(super) fn to_str_entry(ty: &Ty) -> Option<&'static str> {
    Some(match ty {
        Ty::Int(kind) => match kind {
            crate::types::IntKind::U64 => "hero_uint_to_str",
            crate::types::IntKind::I8
            | crate::types::IntKind::I16
            | crate::types::IntKind::I32
            | crate::types::IntKind::I64
            | crate::types::IntKind::U8
            | crate::types::IntKind::U16
            | crate::types::IntKind::U32 => "hero_int_to_str",
        },
        Ty::Float(FloatKind::F32) => "hero_f32_to_str",
        Ty::Float(FloatKind::F64) => "hero_f64_to_str",
        Ty::Bool => "hero_bool_to_str",
        Ty::Str => "hero_str_identity",
        _ => return None,
    })
}
