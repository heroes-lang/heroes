//! What this backend cannot emit yet, said in a way that ends the reader's loop
//! (panel 020, R1 and R2).
//!
//! The gate walks the **IR**, not the surface. Part 5's sugar is erased on the way
//! in, so the IR is the smaller closed vocabulary: a table over ops and types
//! cannot drift when a sugar row moves, and one row here covers every spelling
//! that lowers to it. The rows die one milestone at a time — that is the file's
//! whole design, and the reason the milestone names live in the comments here
//! rather than in the message.
//!
//! **The message names the capability, never the milestone.** `(M5b)` resolves
//! only in `docs/ROADMAP.md`, which the reader does not have; the panel's
//! llm-ergonomist read it as an internal tracker id, grepped the repository for
//! it, and then reported to its user that the toolchain was broken — a sentence it
//! recorded as false. §4.17's standard is everything needed without opening another
//! file.
//!
//! Four rows were missing from the proposal and are here because a judge found
//! them:
//!
//! - **built-ins by name.** `xs.len()` lowers to `call builtin len` while `for`
//!   lowers to `Op::Len`. Gating the op alone leaves an undefined symbol at link
//!   time — the exact class the gate exists to prevent.
//! - **`extern`** (the ffi-pragmatist's veto). No header attachment exists yet, so
//!   the emitter would have to invent a C prototype from the Heroes signature — and
//!   an invented prototype is self-consistent by construction, so clang verifies
//!   nothing. Measured: `abs(-2147483649)` returned `2147483647` at exit 0 with one
//!   non-fatal warning, and `sqlite3_open` was accepted in total silence under
//!   `-Weverything -pedantic`; with the real header both are `error: conflicting
//!   types`. §4.19's mechanism is the `#include`, not the declaration.
//! - **`test` functions are skipped, not refused.** `ir/mod.rs` already says
//!   ordinary builds ignore them; refusing them would make a file unbuildable at
//!   M5a *and* after M5b and M5c, for a block nothing emits.
//! - **one diagnostic per capability**, at its first occurrence, sorted by span.
//!   Three unsupported forms should not be three invocations — `cli.rs`'s own rule
//!   about carrying the list, applied to the backend.

use crate::diagnostics::Diagnostic;
use crate::ir::{Abort, Callee, FnKind, Function, Op, Program};
use crate::resolve::{Resolved, BUILTINS};
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{render_ty, Checked, Ty, TyId};

/// The built-ins the backend emits. The gate reads this list and so does the note,
/// so they cannot disagree — panel 021 R10, and CLAUDE.md §10's own pattern ("one
/// argv table parses and prints the help"). A hand-written enumeration beside a
/// machine-readable table is a sentence that becomes a lie one milestone later.
pub const EMITTED_BUILTINS: [&str; 6] = ["keys", "len", "print", "push", "slice", "to_str"];

/// What the backend does emit. Derived, not maintained: the type list is the arms of
/// `check_type` that return without a note, and the built-in list is the constant
/// above.
///
/// The second clause is the llm-ergonomist's, and it is load-bearing: without it, the
/// rewrite the message *invites* ("use a byte loop instead of `chars()`") is itself a
/// coin flip, because a list that mixes types and forms never says it is exhaustive
/// over built-ins.
pub fn subset() -> String {
    format!(
        "the backend emits every type in the language, `if`, `while`, `for`, `match`, \
         functions, and the built-ins {} — no other built-in is emitted yet",
        EMITTED_BUILTINS
            .iter()
            .map(|name| format!("`{name}`"))
            .collect::<Vec<String>>()
            .join(", ")
    )
}

/// Every capability this program needs and this backend lacks, sorted by where it
/// first appears.
pub(super) fn refuse(
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Vec<Diagnostic> {
    let mut found: Vec<(String, String, Span)> = Vec::new();
    let _ = resolved;
    for function in &program.functions {
        // §4.18: `heroes test` enters these and ordinary builds ignore them.
        if function.kind == FnKind::Test {
            continue;
        }
        if function.kind == FnKind::Extern {
            note(&mut found, "extern", "an `extern` function".to_string(), function.span);
            continue;
        }
        if !function.generics.is_empty() {
            note(&mut found, "generics", "a generic function".to_string(), function.span);
        }
        for slot in &function.params {
            let ty = function.slots[slot.0 as usize].ty;
            check_type(&mut found, ast, checked, src, function, ty, function.span);
        }
        check_type(&mut found, ast, checked, src, function, function.result, function.span);
        for block in &function.blocks {
            for inst in &block.insts {
                check_type(&mut found, ast, checked, src, function, inst.ty, inst.span);
                check_op(&mut found, ast, src, inst.op, inst.span);
                check_builtin_operand(&mut found, checked, function, inst.op, inst.span);
            }

        }
    }
    found.sort_by_key(|(_, _, span)| span.start);
    found
        .into_iter()
        .map(|(code, what, span)| {
            Diagnostic::unsupported(&code, format!("{what} is not emitted yet"), span)
                .with_note(subset())
        })
        .collect()
}

/// One capability, at its earliest span. The dedup is on the *message*: a program
/// with forty strings in it has one string problem.
fn note(found: &mut Vec<(String, String, Span)>, code: &str, what: String, span: Span) {
    match found.iter_mut().find(|(_, existing, _)| *existing == what) {
        Some((_, _, at)) => {
            if span.start < at.start {
                *at = span;
            }
        }
        None => found.push((code.to_string(), what, span)),
    }
}

/// A type the runtime has no representation for yet. One row per §4.3 table entry,
/// so M5b and M5c delete rows rather than discovering cases.
fn check_type(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    function: &Function,
    ty: TyId,
    span: Span,
) {
    let (code, what) = match checked.types.get(ty) {
        // M5b landed `str` and `f64`: the two rows that used to be here are gone,
        // which is the gate's whole design — a row dies per milestone.
        Ty::Int | Ty::Bool | Ty::Unit | Ty::F64 | Ty::Str => return,
        // M5c — the descriptor pass, whose ABI spike 04 froze.
        Ty::Array(_) => return,
        Ty::Map(_, _) => return,
        // Records and variants both emit from M5c step 4. The row that split at step 3
        // is gone: two capabilities became one again, which is what a milestone
        // finishing looks like.
        Ty::Named(_) | Ty::Case(_, _) => return,
        // A `T?` is a by-value tagged union from M5d. Its *operators* are not all here
        // — `.must()` is an `Op::Abort` with its own row — but the representation is.
        Ty::Fallible(_) | Ty::Failure => return,
        // M6.
        Ty::Func { .. } => ("function_value", "a function used as a value".to_string()),
        Ty::Generic(_) => ("generics", "a generic type".to_string()),
        // M7 — §4.19's two opaque types arrive with the header that verifies them.
        Ty::Ptr | Ty::Cstr => {
            let name = render_ty(&checked.types, ast, src, ty, &function.generics);
            ("ffi_type", format!("the C type `{name}`"))
        }
        // The checker reported this already; one mistake, one message.
        Ty::Error => return,
    };
    note(found, code, what, span);
}

fn check_op(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    op: Op,
    span: Span,
) {
    match op {
        // A place with a path is a field or an element. Reading either emits; *writing*
        // through an element does not yet, because that is the copy-on-write path —
        // one unshare per array step of the place, each writing back at its level
        // (panel 022), and the mutation primitives it needs take `HeroArrayHeader **`.
        //
        // The row is here rather than left out because leaving it out is measured: with
        // arrays emitting and this refused nowhere, `xs[0] @ 7` reached clang as
        // `error: incompatible integer to pointer conversion assigning to
        // 'HeroArrayHeader *'`, which this toolchain reports as an internal error with
        // a path to generated C — the compiler blaming itself for the author's program,
        // which is the one failure the gate exists to prevent.
        // Reading and writing a place both emit now: a field is a member access, and an
        // element write is copy-on-write, one unshare per array step with write-back.
        Op::Load(_) | Op::Store { .. } => {}
        // `str`→`cstr` exists for one boundary and nothing consumes it before M7:
        // the row is keyed to the FFI rather than to `str`, which is why landing
        // `str` did not make it emittable.
        Op::Cast { .. } => note(found, "extern", "an `extern` function".to_string(), span),
        Op::Call { callee, args, .. } => {
            callee_note(found, ast, src, callee, span);
            let _ = args;
        }
        // **Every construction emits from M5d**: records, variant cases, both
        // containers, and all three sides of a `T?`. The arm holds no rows rather than
        // being deleted, so adding a shape to the IR is still a compile error here —
        // the gate's own reason for enumerating instead of defaulting.
        Op::Construct { .. } => {}
        // A tag, a payload and a field read are the same three operations on a variant
        // and on a `T?` — §4.6's `ok`/`err` *is* a variant by the time it reaches here —
        // and all of them emit now.
        Op::Field { .. } | Op::Tag(_) | Op::Payload { .. } => {}
        // Both spellings of `[i]` emit now: a byte through `hero_str_byte`, an element
        // through `hero_array_at`.
        Op::Index { .. } => {}
        Op::MapGet { .. } => {}
        // `len` counts bytes on a `str` and elements on an array; a map is the row
        // still standing, and it is refused by its own type before reaching here.
        Op::Len(_) => {}
        Op::FuncRef(_) => {
            note(found, "function_value", "a function used as a value".to_string(), span)
        }
        Op::Abort { reason, .. } => {
            let (code, what) = match reason {
                // `.must()` emits from M6 step 1.
                Abort::Must => return,
                // **This row cannot fire today, and that is worth writing down rather
                // than testing.** An `assert` lives inside a `test` (§4.18), ordinary
                // builds skip `FnKind::Test` entirely (above), and `heroes test` does not
                // exist yet — so no program reaches here. It is the same shape as
                // `cow_check`, struck at M5b for having zero call sites, and it is kept
                // only because the step that lands `heroes test` needs the row already
                // written. If that step slips, this row should be struck, not defended.
                Abort::Assert => ("assert", "`assert`".to_string()),
            };
            note(found, code, what, span);
        }
        // §4.16 says a holed file "produces no binary". The build path reports the
        // holes themselves and stops before here, so this row is the belt to that
        // braces: a hole must never be emitted as anything.
        Op::Hole => note(found, "hole", "a hole (`???`)".to_string(), span),
        // The verifier rejects this before the emitter is asked.
        Op::Missing => note(found, "missing", "a form the compiler cannot lower".to_string(), span),
        Op::Const(_)
        | Op::Incref(_)
        | Op::Decref(_)
        | Op::Unary { .. }
        | Op::Binary { .. }
        | Op::CopyOut { .. } => {}
    }
}

/// A built-in whose *name* is emitted but whose **operand type** has no entry point.
///
/// `slice` is the case, and it was found by a judge pricing a spec sentence rather than
/// by any test: `slice(xs, from: 1, to: 3)` on an array type-checks (exit 0) and then
/// emits `hero_str_slice` on a `HeroArrayHeader *`, which clang rejects — reported as
/// exit 2, the compiler blaming itself for a program the author is entitled to write.
/// Spec line 148 lists `slice` without restricting it to `str`.
///
/// So the row splits by operand, exactly as `len` does, and the array half is refused
/// until `hero_array_slice` exists. `len` needed no such row because both of its halves
/// landed together; this is the shape of a built-in whose halves did not.
fn check_builtin_operand(
    found: &mut Vec<(String, String, Span)>,
    checked: &Checked,
    function: &Function,
    op: Op,
    span: Span,
) {
    let Op::Call { callee: Callee::Builtin(index), args, .. } = op else { return };
    if BUILTINS[index as usize].name != "slice" {
        return;
    }
    let first = function.args_of(args).first().copied();
    let Some(crate::ir::Arg::Value(value)) = first else { return };
    if matches!(checked.types.get(function.value_type(value)), Ty::Array(_)) {
        note(found, "builtin", "the built-in `slice` on an array".to_string(), span);
    }
}

fn callee_note(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    src: &Source,
    callee: Callee,
    span: Span,
) {
    match callee {
        Callee::Heroes(_) => {}
        Callee::Extern(_) => {
            let _ = (ast, src);
            note(found, "extern", "an `extern` function".to_string(), span)
        }
        Callee::Builtin(index) => {
            let name = BUILTINS[index as usize].name;
            if !EMITTED_BUILTINS.contains(&name) {
                note(found, "builtin", format!("the built-in `{name}`"), span);
            }
        }
        Callee::Indirect(_) => {
            note(found, "function_value", "a function used as a value".to_string(), span)
        }
    }
}
