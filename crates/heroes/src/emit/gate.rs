//! What this backend cannot emit yet, said in a way that ends the reader's loop
//! (panel 020, R1 and R2).
//!
//! **Two tables, and this file is neither of them.** What it owns is the LIST:
//! collecting one entry per capability, deduplicating on the message so a program
//! with forty strings in it has one string problem, and sorting by where the
//! capability first appears. The tables themselves are `gate_types.rs` (what the
//! runtime can represent) and `gate_ops.rs` (what the emitter can write).
//!
//! The gate walks the **IR**, not the surface. Part 5's sugar is erased on the way
//! in, so the IR is the smaller closed vocabulary: a table over ops and types
//! cannot drift when a sugar row moves, and one row here covers every spelling
//! that lowers to it. The rows die one milestone at a time — that is the file's
//! whole design, and the reason the milestone names live in the comments here
//! rather than in the message.
//!
//! **The message names the capability, never the milestone.** `(M-strings-ownership)` resolves
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
//!   M-scalars-run *and* after M-strings-ownership and M-value-aggregates, for a block nothing emits.
//! - **one diagnostic per capability**, at its first occurrence, sorted by span.
//!   Three unsupported forms should not be three invocations — `cli.rs`'s own rule
//!   about carrying the list, applied to the backend.

use crate::diagnostics::Diagnostic;
use crate::ir::{FnKind, Program};
use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::Checked;

use super::builtins::EMITTED as EMITTED_BUILTINS;
use super::gate_ops::{check_builtin_operand, check_fixed_flow, check_op};
use super::gate_types::{check_type, unit_fields};

/// What the backend does emit. Derived, not maintained: the type list is the arms of
/// `check_type` that return without a note, and the built-in list is
/// `builtins::EMITTED`, which is also what `builtins::entry` is exhaustive over —
/// panel 021 R10, and CLAUDE.md §10's own pattern ("one argv table parses and
/// prints the help"). A hand-written enumeration beside a machine-readable table
/// is a sentence that becomes a lie one milestone later.
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
    target: super::Target,
    program: &Program,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
) -> Vec<Diagnostic> {
    let mut found: Vec<(String, String, Span)> = Vec::new();
    let _ = resolved;
    unit_fields(&mut found, ast, checked, src);
    for function in &program.functions {
        // §4.18: `heroes test` enters these and ordinary builds ignore them. Under
        // `heroes test` they ARE the program, so the gate walks them like any
        // other function — and the `assert` row below can finally fire.
        if function.kind == FnKind::Test && target != super::Target::Tests {
            continue;
        }
        // The `extern` row died at M-ffi-ladder step 5. It was the one refusal whose
        // reason was a veto rather than a milestone: with no header attachment the
        // emitter would have had to invent a prototype, and an invented prototype
        // is self-consistent by construction, so clang verified nothing. The group
        // form supplies the header, the prelude emits `<header.h>`, and
        // `decls::extern_assertions` covers the half a call site does not.
        if function.kind == FnKind::Extern {
            continue;
        }
        // The `generics` row died at M-generics-library step 6: monomorphisation deleted every
        // template and left one concrete copy per type tuple, so a generic
        // function no longer reaches this backend at all. A survivor is a pass
        // failure, and `ir/phases.rs` asserts it at two phases rather than here.
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
                check_fixed_flow(&mut found, checked, function, inst.op, inst.span);
            }

        }
    }
    found.sort_by_key(|(_, _, span)| span.start);
    found
        .into_iter()
        .map(|(code, what, span)| {
            Diagnostic::unsupported(&code, format!("{what} is not emitted yet"), span)
                .with_note(what_the_author_can_do(&code).to_string())
                .with_note(subset())
        })
        .collect()
}

/// What this row leaves the author able to do — panel 082 R1, **per row**, because
/// the blanket note was one string for every row and false on most of them.
///
/// The test is not *how hard is this to implement*, it is **what the refusal is
/// keyed on**. A row keyed on a capability the backend lacks whatever the author
/// writes is honestly *"no change to this file will fix this"* — that is `missing`,
/// and it is the only one. Every other row is keyed on **something in this
/// program**, so something else in this program compiles, and the note says what.
///
/// **Each route below was run before it was written here**, which is what separates
/// a note from a guess (CLAUDE.md §1). They are notes rather than `Fix`es because
/// none is mechanical — wrapping a `ptr` in a record changes types at every use —
/// and CLAUDE.md §8 reserves machine-application for `certain`.
///
/// The prose also has to **complete the sentence `gate.rs` builds**, `"{what} is
/// not emitted yet"`. Keeping the reason out of `what` is what stops the message
/// reading *"…so one is written where its record is built is not emitted yet"* —
/// panel 082's ffi-pragmatist filed exactly that against `pointer_element`, where
/// the row's phrase already carried a clause of its own.
fn what_the_author_can_do(code: &str) -> &'static str {
    match code {
        // The only row the old blanket note was ever true of.
        "missing" => "no change to this file will fix this",
        // Measured against real SQLite 3.51.0: `stmts: [ptr]` is refused and
        // `record Stmt { p: ptr }` with `stmts: [Stmt]` runs at exit 0. §4.19
        // ladder step 3's own shape, so this is the row an author reaches first.
        "pointer_element" => "hold the pointer in a `record` and make an array of that instead",
        // Measured: `[()]` is refused, `[i64]` compiles. There is no reason to want
        // a container of `()`, so the note says the shape rather than a rewrite.
        "unit_element" | "unit_field" => "give it a type that carries a value",
        // Measured: writing the elements at the construction compiles; copying or
        // re-assigning the array does not, because C has no array assignment.
        "fixed_flow" | "fixed_element" => {
            "write a fixed array out where its record is built — C has no array assignment"
        }
        // Measured: the same generic called at `[2, 1]` builds and runs. Naming the
        // element type would send the reader to a line where it is spelled `A`.
        "builtin" => "call it at a type the built-in accepts",
        // A hole is filled by editing the file, which is the whole point of `???`.
        _ => "this is refused for what is on this line, so something else compiles",
    }
}

/// One capability, at its earliest span. The dedup is on the *message*: a program
/// with forty strings in it has one string problem.
pub(super) fn note(found: &mut Vec<(String, String, Span)>, code: &str, what: String, span: Span) {
    match found.iter_mut().find(|(_, existing, _)| *existing == what) {
        Some((_, _, at)) => {
            if span.start < at.start {
                *at = span;
            }
        }
        None => found.push((code.to_string(), what, span)),
    }
}
