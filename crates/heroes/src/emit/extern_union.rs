//! The half of §4.19 that asks whether a header's type is a **union**, and stops
//! the two operations that would answer wrongly (panel 073).
//!
//! A C `union` gives every member the same address. Bound as a group's `record`,
//! three things go wrong and all three are silent:
//!
//! - **construction** writes every field into one place and C's last initialiser
//!   wins — `UDef(i: 1, f: 1.0)` printed **1065353216**, the bits of `1.0f`, at
//!   exit 0;
//! - the generated **`eq`** walks fields, so it reads the same bytes twice under
//!   two types: two values written *differently* compared `true`, and a record
//!   holding a float's NaN bit pattern was not equal to itself;
//! - the generated **`hash`** does the same, so agreement with `eq` is accidental.
//!
//! **Reading is not one of them, and this file must not touch it.** Panel 073's
//! ffi-pragmatist compiled three SDL3 programs that read a union's members and
//! ran them: all three are correct today, and the four-arm event loop is what a
//! binding to `SDL_Event` actually looks like. The historian found the same answer
//! in five independent designs — Rust's *"it must specify exactly one field"*, D's
//! *"only one member initializer is allowed"*, Zig, Swift, and Nim's C++ backend,
//! which catches this exact bug where its C backend does not — while the two
//! languages that refused unions at the **type** level, Go and Fortran, had users
//! rebuild them by hand, and Go's rebuild defeated its own runtime pointer check.
//!
//! ## The predicate, and why it is a sum rather than a pair
//!
//! ```c
//! _Static_assert(sizeof(UDef) >= sizeof(((UDef *)0)->i) + sizeof(((UDef *)0)->f),
//!     "heroes-ffi-union UDef i f");
//! ```
//!
//! Distinct members of a **struct** occupy distinct storage (C11 6.7.2.1p15), so
//! their sizes sum to no more than the struct's; every member of a **union**
//! starts at offset 0, so two or more of them cannot fit. The assertion is exactly
//! *this record's declared fields do not overlap*, checked by clang against the
//! real header — the author declares and clang refutes, which is §4.19's own
//! thesis and not a probe.
//!
//! **The all-pairs `offsetof` form was measured and is worse on both axes.** It
//! calls a struct of two GNU zero-sized members a union — `offsetof(S5, e1) ==
//! offsetof(S5, e2)` with no overlap at all, a false positive this form passes —
//! and it is O(n²): a sixteen-field record emits 120 conjuncts against 16.
//!
//! ## What it is emitted for, and why not for every record
//!
//! Only for a record the program **constructs** or **compares** — including
//! through a containing type, because `==` on a Heroes record walks into its
//! fields' `eq`. Emitting it for every declaration would refuse the read-only
//! bindings that work today, which is the shape panel 073 rejected. A record with
//! fewer than two declared fields is never emitted for either: one member of a
//! union is sound, and it is what a binding to one arm looks like.

use crate::ir::{BinOp, Op, Program, Shape};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::mangle;
use super::typedefs::Names;
use super::writer::Writer;

/// The marker the assertion carries, so `emit/ffi.rs` recognises one of ours in
/// clang's output without matching prose. **One writer, one reader**, the contract
/// `FIELD_ASSERTION` already keeps.
pub(crate) const UNION_ASSERTION: &str = "heroes-ffi-union";

/// One assertion per `extern` record the program constructs or compares.
pub(super) fn union_assertions(
    w: &mut Writer,
    program: &Program,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let mut any = false;
    for decl in used(program, ast, checked) {
        let DeclKind::Record { fields, header: Some(_), .. } = &ast.decls[decl as usize].kind
        else {
            continue;
        };
        if fields.len() < 2 {
            continue;
        }
        let c_type = names.of(decl).to_string();
        let members: Vec<String> = fields
            .iter()
            .map(|f| mangle::field_of(true, src.slice(f.name)))
            .collect();
        let sum = members
            .iter()
            .map(|m| format!("sizeof((({c_type} *)0)->{m})"))
            .collect::<Vec<_>>()
            .join(" + ");
        let named = members.join(" ");
        // `#line` at the declaration's own name: unlike a field assertion, no one
        // field is at fault — what the author must change is the declaration or
        // the expression that builds it.
        let (file, at_line, _) = src.locate(ast.decls[decl as usize].name.start);
        let file = file.to_string();
        w.at_file(&file, at_line);
        w.line(&format!(
            "_Static_assert(sizeof({c_type}) >= {sum}, \"{UNION_ASSERTION} {c_type} {named}\");"
        ));
        any = true;
    }
    if any {
        w.at_generated();
        w.blank();
    }
}

/// Every `extern` record declaration the program constructs or compares, directly
/// or through a type that contains one.
///
/// **The reach matters and it is not decoration**: `==` on a Heroes record calls
/// its generated `eq`, which calls its fields', so a union one level down is
/// compared by the same wrong walk. Panel 061 established the same transitivity
/// for `partial`, and this is that rule one type over.
fn used(program: &Program, ast: &Ast, checked: &Checked) -> Vec<u32> {
    let mut out: Vec<u32> = Vec::new();
    for function in &program.functions {
        for block in &function.blocks {
            for inst in &block.insts {
                match inst.op {
                    // Construction is the direct case: the fields are written into
                    // one place and C keeps the last.
                    Op::Construct { shape: Shape::Record(decl), .. } => push(&mut out, decl),
                    // Equality reaches through fields, so the type in hand may only
                    // contain the union.
                    Op::Binary { op: BinOp::Eq | BinOp::Ne, left, .. } => {
                        let ty = function.value_type(left);
                        reach(&mut out, ast, checked, ty, 0);
                    }
                    _ => {}
                }
            }
        }
    }
    out.sort_unstable();
    out.dedup();
    out
}

fn push(out: &mut Vec<u32>, decl: u32) {
    if !out.contains(&decl) {
        out.push(decl);
    }
}

/// The extern records a type reaches by value, bounded like `partial::reaches` and
/// for its reason: a generic type in the arena can still be cyclic through a type
/// parameter, and a depth-16 nest of extern records does not exist.
fn reach(out: &mut Vec<u32>, ast: &Ast, checked: &Checked, ty: TyId, depth: u32) {
    if depth > 16 {
        return;
    }
    match checked.types.get(ty) {
        Ty::Named(decl) => {
            push(out, decl);
            let fields = match &ast.decls[decl as usize].kind {
                DeclKind::Record { fields, .. } => fields,
                _ => return,
            };
            for field in fields {
                if let Some(inner) = checked.written_type(field.ty) {
                    reach(out, ast, checked, inner, depth + 1);
                }
            }
        }
        Ty::Array(inner) | Ty::Fixed(inner, _) | Ty::Fallible(inner) => {
            reach(out, ast, checked, inner, depth + 1)
        }
        Ty::Map(key, value) => {
            reach(out, ast, checked, key, depth + 1);
            reach(out, ast, checked, value, depth + 1);
        }
        _ => {}
    }
}
