//! The dump: `heroes build <file> --dump-ir` (design.md §3.5; panel 019 point 7).
//!
//! **This is an artifact, not debug output** — and panel 019 also fixed what that
//! does and does not promise. It is **deterministic and golden-tested**; it is
//! explicitly **not version-stable**, which is LLVM's own stance on `.ll` ("the
//! textual format is not backwards compatible … there are no specific promises")
//! and rustc's on MIR dumps ("intended for human consumers only and is subject to
//! change without notice"). Determinism is not compatibility.
//!
//! Five notation rules, and each one exists because a reader got it wrong or could
//! have (the llm-ergonomist's session, on label-stripped samples):
//!
//! 1. **The destination is always left of `=` or `<-`.** LLVM's `store` is
//!    value-first, so a dest-first `store total, $t0` is a coin flip for any
//!    reader who has seen LLVM — and `t0: int @ 0` is legal Heroes, so the
//!    namespace luck that made the sample readable does not hold in general.
//! 2. **`$` on every temporary and every synthetic slot.** The character is not in
//!    the language, so a name here can collide with nothing the author wrote — and
//!    it makes R1 greppable: no `$` may appear in a diagnostic.
//! 3. **Polarity in the text**: `branch $t -> bb2 else bb4`. An unlabelled
//!    two-target branch inverts silently.
//! 4. **`preds` on every block.** Without it, "what is this block's role" costs a
//!    pass over every block in the function.
//! 5. **`!` on every instruction that can abort** (§4.3's overflow, §4.9's bounds,
//!    division by zero). Three blocks of the sample had exits the reader did not
//!    see; `add!` also distinguishes `int` arithmetic from `f64`, which cannot
//!    abort.
//!
//! Indentation is four spaces per level, like the language itself (§4.1).

use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{render_ty, Checked};

use super::inst::{BlockId, SlotId, Term};
use super::print_inst::{instruction, value_name};
use super::{FnKind, Function, Program, SlotKind};

/// The whole program, in source order.
pub fn dump(program: &Program, ast: &Ast, checked: &Checked, src: &Source) -> String {
    let mut out = String::new();
    if !program.strings.is_empty() {
        out.push_str("strings\n");
        for (index, text) in program.strings.iter().enumerate() {
            out.push_str(&format!("    {index}  \"{}\"\n", escape(text)));
        }
        out.push('\n');
    }
    // The library is part of every compilation and part of no dump: a
    // `--dump-<stage>` answers "what does the compiler know about the file I
    // named" (CLAUDE.md §10), and the library is identical in every program, so
    // it carries no information about this one. The emitted C is the exception,
    // and necessarily — the binary needs the definitions.
    let shown: Vec<&Function> =
        program.functions.iter().filter(|f| !src.is_library(f.span.start)).collect();
    for (index, function) in shown.into_iter().enumerate() {
        if index > 0 {
            out.push('\n');
        }
        out.push_str(&one(function, ast, checked, src));
    }
    out
}

fn one(function: &Function, ast: &Ast, checked: &Checked, src: &Source) -> String {
    let mut out = header(function, ast, checked, src);
    if function.kind == FnKind::Extern {
        // No blocks at all: the implementation is C's, and clang type-checks the
        // call against the real header (§4.19).
        out.push_str("    no body — C provides it\n");
        return out;
    }
    let locals = slots(function, ast, checked, src);
    if !locals.is_empty() {
        out.push_str(&format!("    slots  {locals}\n"));
    }
    for (index, block) in function.blocks.iter().enumerate() {
        out.push_str(&block_header(BlockId(index as u32), &block.note, &block.preds));
        for inst in &block.insts {
            out.push_str(&format!("        {}\n", instruction(function, inst, ast, checked, src)));
        }
        out.push_str(&format!("        {}\n", terminator(&block.term)));
    }
    out
}

/// The signature, spelled the way the author wrote it — the same shape
/// `heroes fmt` prints, so the two artifacts agree about what a function is.
fn header(function: &Function, ast: &Ast, checked: &Checked, src: &Source) -> String {
    let show = |ty| render_ty(&checked.types, ast, src, ty, &function.generics);
    match function.kind {
        FnKind::Constant => format!("constant {}: {}\n", function.name, show(function.result)),
        FnKind::Test => format!("test {}\n", function.name),
        FnKind::Function | FnKind::Extern => {
            let generics = if function.generics.is_empty() {
                String::new()
            } else {
                format!("<{}>", function.generics.join(", "))
            };
            let params: Vec<String> = function
                .params
                .iter()
                .map(|slot| {
                    let held = &function.slots[slot.0 as usize];
                    let marker = if is_mutable(function, *slot) { "@" } else { "" };
                    format!("{marker}{}: {}", held.name, show(held.ty))
                })
                .collect();
            let result = if function.result == checked.types.unit() {
                String::new()
            } else {
                format!(" -> {}", show(function.result))
            };
            let word = if function.kind == FnKind::Extern { "extern function" } else { "function" };
            format!("{word} {}{generics}({}){result}\n", function.name, params.join(", "))
        }
    }
}

/// Every slot that is not a parameter — the parameters are in the signature, and
/// repeating them would make the line that matters harder to find.
fn slots(function: &Function, ast: &Ast, checked: &Checked, src: &Source) -> String {
    let rendered: Vec<String> = function
        .slots
        .iter()
        .enumerate()
        .filter(|(index, _)| !function.params.contains(&SlotId(*index as u32)))
        .map(|(_, slot)| {
            let ty = render_ty(&checked.types, ast, src, slot.ty, &function.generics);
            format!("{}: {ty}", slot.name)
        })
        .collect();
    rendered.join(" · ")
}

fn is_mutable(function: &Function, slot: SlotId) -> bool {
    matches!(function.slots[slot.0 as usize].kind, SlotKind::Param { mutable: true })
}

/// `bb1  while: test    preds bb0, bb3`
fn block_header(id: BlockId, note: &str, preds: &[BlockId]) -> String {
    let mut line = format!("    bb{}  {note}", id.0);
    if !preds.is_empty() {
        let names: Vec<String> = preds.iter().map(|p| format!("bb{}", p.0)).collect();
        line.push_str(&format!("    preds {}", names.join(", ")));
    }
    line.push('\n');
    line
}

fn terminator(term: &Term) -> String {
    match term {
        Term::Jump(target) => format!("jump bb{}", target.0),
        Term::Branch { cond, then, otherwise } => format!(
            "branch {} -> bb{} else bb{}",
            value_name(*cond),
            then.0,
            otherwise.0
        ),
        Term::Switch { tag, cases } => {
            let edges: Vec<String> = cases
                .iter()
                .enumerate()
                .map(|(case, target)| format!("case {case} -> bb{}", target.0))
                .collect();
            format!("switch {} {}", value_name(*tag), edges.join(", "))
        }
        Term::Return(Some(value)) => format!("return {}", value_name(*value)),
        Term::Return(None) => "return".to_string(),
        Term::Unreachable => "unreachable".to_string(),
        // `verify.rs` rejects this, so it can only appear in a dump taken of a
        // function that is still being built — which is to say, never.
        Term::Open => "OPEN".to_string(),
    }
}

/// A string literal, back in the source's own notation: the five escapes of panel
/// 008 and no others, so a dumped literal is one line however it was written.
fn escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for c in text.chars() {
        match c {
            '\n' => out.push_str("\\n"),
            '\t' => out.push_str("\\t"),
            '\\' => out.push_str("\\\\"),
            '"' => out.push_str("\\\""),
            _ => out.push(c),
        }
    }
    out
}
