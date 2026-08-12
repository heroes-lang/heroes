//! One instruction, as C (design.md §3.1, §4.14; panel 020).
//!
//! The sibling of `ir/print_inst.rs`, and deliberately shaped like it: the same
//! `match` over the same `Op`, one arm each, so a new instruction cannot be added
//! to the IR without both files failing to compile.
//!
//! **UB is not a diagnostic.** Every arithmetic operation that §4.3 says aborts is
//! emitted with its check: `__builtin_*_overflow` for `+ - *` and negation, an
//! explicit pair of guards for `/` and `%`. The pair matters — `INT64_MIN % -1` is
//! undefined too, and measured on arm64 it does not trap: it returns 0 at exit 0,
//! while on x86 the same program raises SIGFPE. Same source, two behaviours, no
//! diagnostic anywhere. (The spec says overflow and division by zero abort and is
//! silent on this one; the silence is recorded as a gap rather than read as
//! permission.)
//!
//! One invariant to carry forward, because it will not be visible when it starts
//! mattering: **`__builtin_*_overflow` decides overflow against the destination
//! type, not the operands.** `sub_overflow(false, true)` into a `bool` reports
//! overflow; into an `int64_t` it does not. Heroes never does `bool` arithmetic, so
//! M-scalars-run is safe — but §4.19's `c_int` (Part 7) will introduce a 32-bit destination
//! and move the abort threshold from 2⁶³ to 2³¹ with nothing changing at the call
//! site.

use crate::ir::{Arg, Inst, Op, Place, Program};
use crate::types::Ty;

use super::aggregate;
use super::ops;
use super::ctype::is_unit;
use super::mangle;
use super::writer::Writer;

pub(super) fn emit(
    w: &mut Writer,
    program: &Program,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    inst: &Inst,
    live_values: &std::collections::BTreeSet<u32>,
) {
    // Unpacked once: the four references travel together everywhere in the backend
    // (they are what "what is this type called in C" needs), so they arrive as one
    // bundle and are spread here rather than in every signature.
    let (checked, src) = (types.checked, types.src);
    match inst.op {
        // §4.8's second half, and the only instruction that is *about* the calling
        // convention rather than about the program. It is housekeeping, so it is
        // pointed at the generated file: lldb must not blame the author's `return`
        // line for a write the author never wrote.
        Op::CopyOut { param } => {
            w.at_generated();
            let slot = &function.slots[param.0 as usize];
            w.line(&format!(
                "    *{} = {};",
                mangle::out_param(param.0, &slot.name),
                mangle::slot(param.0, &slot.name)
            ));
            return;
        }
        _ => super::writer::at_span(w, src, inst.span.start),
    }
    // **A call whose result nobody reads gets no temporary**, and no other op is
    // treated this way. `_ = f(x)` is §4.4's discard and the ordinary way to
    // ignore a C status code, and it emitted `int64_t t2; t2 = f(x);` —
    // `-Wunused-but-set-variable` on every FFI program that did the right thing.
    // Restricted to `Op::Call` on purpose: a pure op with an unused result may
    // vanish, but an *aborting* one (an index, a division) must still run, and
    // dropping its assignment would drop its check.
    let discarded = matches!(inst.op, Op::Call { .. })
        && inst.dest.is_some_and(|d| !live_values.contains(&d.0));
    let dest = inst.dest.filter(|_| !is_unit(checked, inst.ty) && !discarded);
    let target = dest.map(|d| mangle::value(d.0));
    match inst.op {
        Op::Const(value) => {
            if let Some(name) = target {
                w.line(&format!("    {name} = {};", ops::constant(value)));
            }
        }
        Op::Load(place) => {
            if let Some(name) = target {
                w.line(&format!("    {name} = {};", read(types, function, place)));
            }
        }
        // A store whose place ends in an index is copy-on-write, and the runtime owns
        // the whole replacement: unshare, release what was there, move the value in.
        Op::Store { place, value }
            if function
                .steps_of(place.path)
                .iter()
                .any(|step| matches!(step, crate::ir::Step::Index(_))) =>
        {
            match aggregate::write_element(types, function, place, value) {
                Some(lines) => {
                    for line in lines {
                        w.line(&format!("    {line}"));
                    }
                }
                None => w.line("    hero_unreachable(); /* not an element write */"),
            }
        }
        Op::Store { place, value } => {
            if !is_unit(checked, function.value_type(value)) {
                w.line(&format!(
                    "    {} = {};",
                    read(types, function, place),
                    mangle::value(value.0)
                ));
            }
        }
        // Housekeeping the author did not write, so it points at the generated file:
        // lldb must not attribute a refcount to a user line.
        //
        // **Dispatched by type**, and the two shapes are different in kind. A `str`
        // owns one block, so the runtime counts it. An aggregate owns nothing of its
        // own and *contains* what is counted, so its generated function walks its
        // fields — which is why this is `retain`/`release` and not `copy`: no bytes
        // move, the references inside them just become owned.
        Op::Incref(value) | Op::Decref(value) => {
            w.at_generated();
            let keep = matches!(inst.op, Op::Incref(_));
            let ty = function.value_type(value);
            let name = mangle::value(value.0);
            let line = match checked.types.get(ty) {
                Ty::Str if keep => format!("    hero_str_incref({name});"),
                Ty::Str => format!("    hero_str_decref({name});"),
                Ty::Array(_) if keep => format!("    hero_array_incref({name});"),
                Ty::Array(_) => format!("    hero_array_decref({name});"),
                Ty::Map(_, _) if keep => format!("    hero_map_incref({name});"),
                Ty::Map(_, _) => format!("    hero_map_decref({name});"),
                // A `T?` and a `Failure` are by value, so they are reached by address
                // and their own function decides which side of the union is live.
                Ty::Failure if keep => format!("    hero_failure_retain(&{name});"),
                Ty::Failure => format!("    hero_failure_release(&{name});"),
                _ => match aggregate::retain(types, ty, value, keep) {
                    Some(call) => format!("    {call};"),
                    None => "    hero_unreachable(); /* the gate refuses this type */".to_string(),
                },
            };
            w.line(&line);
        }
        Op::Unary { op, operand } => ops::unary(w, function, checked, op, operand, target),
        Op::Binary { op, left, right } => {
            ops::binary(w, types, function, checked, op, left, right, target)
        }
        Op::Call { callee, args, .. } => {
            let arguments: Vec<String> = function
                .args_of(args)
                .into_iter()
                .map(|arg| match arg {
                    Arg::Value(value) => mangle::value(value.0),
                    // A place, not a value: the callee gets its address (§4.8).
                    Arg::InOut(place) => format!("&{}", read(types, function, place)),
                })
                .collect();
            // Which instance this call reaches, read from the table the pass read.
            let instance = crate::ir::mono::instance_at(checked, inst.span)
                .cloned()
                .unwrap_or_default();
            ops::call(
                w, program, function, types, callee, args, &arguments, target, instance,
            );
        }
        // One built-in, two runtime entry points: `len` on a `str` counts bytes and on
        // an array counts elements. The op is the same op, so the split is by operand
        // type — the same shape the gate uses to ask about it.
        Op::Len(value) => {
            if let Some(name) = target {
                let counter = match checked.types.get(function.value_type(value)) {
                    Ty::Str => "hero_str_len",
                    Ty::Map(_, _) => "hero_map_len",
                    _ => "hero_array_len",
                };
                w.line(&format!("    {name} = {counter}({});", mangle::value(value.0)));
            }
        }
        // `s[i]`: a byte as an `int`, aborting out of range (spec line 142). The
        // array case is the same op and waits for M-value-aggregates.
        Op::Index { base, index } if checked.types.get(function.value_type(base)) == Ty::Str => {
            if let Some(name) = target {
                w.line(&format!(
                    "    {name} = hero_str_byte({}, {});",
                    mangle::value(base.0),
                    mangle::value(index.0)
                ));
            }
        }
        // Every remaining form is refused by `gate.rs` at this milestone. The arm is
        // here rather than in a catch-all so that M-value-aggregates and M-generics-library are compile errors
        // until they are written, not silent omissions.
        // `Point(x: 1, y: 2)` and `.num(v: 7)`. The container shapes and `T?` are
        // still refused, and each stays a named arm so that landing one is a compile
        // error here rather than a silent omission.
        Op::Construct { shape: crate::ir::Shape::Record(decl), args }
        | Op::Construct { shape: crate::ir::Shape::Case(decl, _), args } => {
            if let Some(name) = target {
                let arguments: Vec<String> = function
                    .args_of(args)
                    .into_iter()
                    .map(|arg| match arg {
                        Arg::Value(value) => mangle::value(value.0),
                        Arg::InOut(place) => format!("&{}", read(types, function, place)),
                    })
                    .collect();
                let literal = match inst.op {
                    Op::Construct { shape: crate::ir::Shape::Case(decl, case), .. } => {
                        aggregate::construct_case(types, decl, case, &arguments)
                    }
                    _ => aggregate::construct(types, decl, &arguments),
                };
                match literal {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* not an aggregate */"),
                }
            }
        }
        // An array literal is several statements rather than one expression, because
        // each push has to release the array it grew from.
        Op::Construct { shape: crate::ir::Shape::Array, args } => {
            if let Some(name) = target {
                let arguments: Vec<String> = function
                    .args_of(args)
                    .into_iter()
                    .filter_map(|arg| match arg {
                        Arg::Value(value) => Some(mangle::value(value.0)),
                        Arg::InOut(_) => None,
                    })
                    .collect();
                match aggregate::build_array(types, inst.ty, &arguments, &name) {
                    Some(lines) => {
                        for line in lines {
                            w.line(&format!("    {line}"));
                        }
                    }
                    None => w.line("    hero_unreachable(); /* not an array */"),
                }
            }
        }
        // `ok(x)`, `fail(c, m)`, and the `err` that `?` produces.
        Op::Construct { shape: shape @ (crate::ir::Shape::Ok | crate::ir::Shape::Fail | crate::ir::Shape::Err), args } => {
            if let Some(name) = target {
                let arguments: Vec<String> = function
                    .args_of(args)
                    .into_iter()
                    .filter_map(|arg| match arg {
                        Arg::Value(value) => Some(mangle::value(value.0)),
                        Arg::InOut(_) => None,
                    })
                    .collect();
                match aggregate::construct_option(types, inst.ty, shape, &arguments) {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* not a T? */"),
                }
            }
        }
        // A tag and a payload read the same two members on a variant and on a `T?`.
        // §4.6's `ok`/`err` *is* a variant by the time it reaches here, and the only
        // difference is where the member names come from.
        Op::Construct { shape: crate::ir::Shape::Map, args } => {
            if let Some(name) = target {
                let arguments: Vec<String> = function
                    .args_of(args)
                    .into_iter()
                    .filter_map(|arg| match arg {
                        Arg::Value(value) => Some(mangle::value(value.0)),
                        Arg::InOut(_) => None,
                    })
                    .collect();
                match aggregate::build_map(types, inst.ty, &arguments, &name) {
                    Some(lines) => {
                        for line in lines {
                            w.line(&format!("    {line}"));
                        }
                    }
                    None => w.line("    hero_unreachable(); /* not a map */"),
                }
            }
        }
        Op::MapGet { map, key } => {
            if let Some(name) = target {
                match aggregate::map_get(types, function, map, key, inst.ty, &name) {
                    Some(lines) => {
                        for line in lines {
                            w.line(&format!("    {line}"));
                        }
                    }
                    None => w.line("    hero_unreachable(); /* not a map */"),
                }
            }
        }
        // `.must()` on an error. The failure travels with the abort (`ir/fallible.rs`),
        // so the panic names the `code` and `msg` the author wrote rather than only that
        // a `.must()` failed. `assert` keeps its own row until `heroes test` runs one.
        Op::Abort { reason: crate::ir::Abort::Must, args } => {
            w.at_generated();
            match function.args_of(args).first() {
                Some(Arg::Value(value)) => {
                    w.line(&format!("    hero_panic_must({});", mangle::value(value.0)))
                }
                _ => w.line("    hero_unreachable(); /* a must with no failure */"),
            }
        }
        // `assert` (§4.18, spec line 163: "shows the source expression and both
        // sides"). The IR carries three operands where the asserted expression is
        // a comparison and one where it is not (`ir/asserts.rs`), and each side is
        // rendered through the same `to_str` entry point `print` uses — so a side
        // with no rendering (a record, an array) falls back to the text alone
        // rather than inventing one.
        Op::Abort { reason: crate::ir::Abort::Assert, args } => {
            w.at_generated();
            let operands = function.args_of(args);
            let rendered: Vec<String> = operands
                .iter()
                .skip(1)
                .filter_map(|arg| match arg {
                    Arg::Value(value) => {
                        let ty = checked.types.get(function.value_type(*value));
                        let entry = match ty {
                            Ty::Int => "hero_int_to_str",
                            Ty::F64 => "hero_f64_to_str",
                            Ty::Bool => "hero_bool_to_str",
                            Ty::Str => "hero_str_identity",
                            _ => return None,
                        };
                        Some(format!("{entry}({})", mangle::value(value.0)))
                    }
                    Arg::InOut(_) => None,
                })
                .collect();
            let text = match operands.first() {
                Some(Arg::Value(value)) => mangle::value(value.0),
                _ => {
                    w.line("    hero_unreachable(); /* an assert with no text */");
                    return;
                }
            };
            if rendered.len() == 2 {
                w.line(&format!(
                    "    hero_panic_assert_sides({text}, {}, {});",
                    rendered[0], rendered[1]
                ));
            } else {
                // A side with no rendering — a record, an array — is still
                // computed: the lowering does not know what C can print, and
                // asking it to would put a backend question in the IR. Discarding
                // it explicitly is what keeps `-Wunused-but-set-variable` at zero,
                // and the cast says "deliberately" where silence would say
                // "forgotten".
                for arg in operands.iter().skip(1) {
                    if let Arg::Value(value) = arg {
                        w.line(&format!("    (void){};", mangle::value(value.0)));
                    }
                }
                w.line(&format!("    hero_panic_assert({text});"));
            }
        }
        Op::Tag(base) => {
            if let Some(name) = target {
                let text = match checked.types.get(function.value_type(base)) {
                    Ty::Fallible(_) => aggregate::option_tag(base),
                    _ => aggregate::tag(base),
                };
                w.line(&format!("    {name} = {text};"));
            }
        }
        Op::Payload { base, case } => {
            if let Some(name) = target {
                let text = match checked.types.get(function.value_type(base)) {
                    Ty::Fallible(_) => Some(aggregate::option_payload(base, case)),
                    _ => aggregate::payload(types, function, base, case),
                };
                match text {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* not a variant */"),
                }
            }
        }
        Op::Field { base, index } => {
            if let Some(name) = target {
                match aggregate::read_field(types, function, base, index) {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* the gate refuses this base */"),
                }
            }
        }
        Op::Index { base, index } => {
            if let Some(name) = target {
                match aggregate::read_element(types, function, base, index) {
                    Some(text) => w.line(&format!("    {name} = {text};")),
                    None => w.line("    hero_unreachable(); /* not an array */"),
                }
            }
        }
        // A function used as a value is the C function designator, which decays to
        // a pointer on its own — `&` would be legal and redundant, and writing it
        // would make the emitted C say something the language does not: that a
        // function value is an address taken of something, rather than the
        // function itself (§1.11's boundary: this eight-byte value IS a C
        // callback).
        Op::FuncRef(crate::ir::Callee::Heroes(decl)) => {
            if let Some(name) = target {
                let callee = program
                    .functions
                    .iter()
                    .find(|f| f.decl == decl)
                    // The callee's own module, which is the point of M-module-namespace: a
                    // function value taken across a module boundary is the same
                    // symbol the definition emitted.
                    .map(|f| mangle::function(src.component_at(f.span.start), &f.name))
                    .unwrap_or_else(|| "hero_unreachable".to_string());
                w.line(&format!("    {name} = {callee};"));
            }
        }
        // **`s.cstr()` is free, and that is §4.20's highest-return decision paying
        // out.** Every Heroes string allocates `len+1` and is NUL-terminated, so
        // handing C the byte pointer is a field read rather than a copy — Zig's
        // `[:0]u8` trick. The `str` itself is still owned by the caller and still
        // decrefed at the end of the statement, which is why the borrow is safe
        // only *for the duration of the call* (§4.19).
        Op::Cast { kind: crate::ir::CastKind::StrToCstr, operand } => {
            let Some(name) = target else { return };
            w.line(&format!("    {name} = hero_str_cstr({});", mangle::value(operand.0)));
        }
        // An `extern` used as a value is the C callback case §4.19's ladder needs,
        // and it is `ptr` until a C-width type vocabulary exists (panel 013). A
        // built-in or an indirection here is a lowering bug: neither has an
        // address to take.
        Op::FuncRef(_)
        | Op::Hole
        | Op::Missing => {
            w.line("    hero_unreachable(); /* the gate refuses this form */");
        }
        Op::CopyOut { .. } => unreachable!("handled above"),
    }
}

/// A place, as a C lvalue. At M-scalars-run a place is its root: a path means a field or an
/// element, and `gate.rs` refuses both until the descriptor pass exists.
fn read(types: &aggregate::Types, function: &crate::ir::Function, place: Place) -> String {
    aggregate::place(types, function, place)
}
