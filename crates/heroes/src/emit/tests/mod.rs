//! The emitter's tests: the C's shape, and what the gate refuses.
//!
//! Every case runs the whole pipeline into real C text. The **compilable** half of
//! the claim is not tested here — a crate test cannot run clang and stay a unit
//! test — it is tested by `tests/golden/run/`, which compiles every case at `-O0`
//! and `-O2` and executes it. These tests are about the text: the shapes CLAUDE.md
//! §7 mandates, and the ten things panel 020 measured clang deciding if the emitter
//! does not.
//!
//! | file | what it holds to account |
//! |------|--------------------------|
//! | `shape.rs` | the prologue, the labels, the shim, `#line`, the guards |
//! | `gate.rs`  | one refusal per capability, and the two that are *not* refusals |
//! | `mutants.rs` | the gate and the emitter agree, over a corpus nobody wrote |

use crate::emit::{emit, Emitted};
use crate::ir::lower;
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;
use crate::types::check;

mod gate;
mod mutants;
mod shape;

/// Emits a program whose frontend said nothing, under a fixed file name so the
/// `#line` directives and the module prefix are the same on every machine.
fn emitted(text: &str) -> Emitted {
    named("scratch.hero", text)
}

fn named(name: &str, text: &str) -> Emitted {
    let src = Source::new(name.to_string(), text.to_string());
    let parsed = parse(&src);
    assert!(parsed.diagnostics.is_empty(), "parse: {}", parsed.diagnostics[0].message);
    let resolved = resolve(&parsed.ast, &src);
    assert!(resolved.diagnostics.is_empty(), "resolve: {}", resolved.diagnostics[0].message);
    let checked = check(&parsed.ast, &resolved, &src);
    assert!(checked.diagnostics.is_empty(), "check: {}", checked.diagnostics[0].message);
    let out = lower(&parsed.ast, &resolved, &checked, &src);
    assert!(out.diagnostics.is_empty(), "lower: {}", out.diagnostics[0].message);
    emit(&out.program, &parsed.ast, &resolved, &checked, &src)
}

/// The C, asserting the gate had no objection. Most tests want this.
fn c(text: &str) -> String {
    let out = emitted(text);
    assert!(
        out.diagnostics.is_empty(),
        "the gate refused: {}",
        out.diagnostics[0].message
    );
    out.c
}
