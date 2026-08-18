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
//! ## The predicate: ask C what the type IS
//!
//! ```c
//! _Static_assert(__builtin_classify_type(*(UDef *)0) != 13, "heroes-ffi-union UDef i f");
//! ```
//!
//! **13 is `union_type_class` and 12 is `record_type_class`.** The question is
//! answered by the same builtin `extern_record.rs` already calls on every field,
//! in an unevaluated operand, against the real header: the author declares and
//! clang refutes, which is §4.19's thesis and not a probe.
//!
//! **Panel 073 shipped `sizeof(T) >= Σ sizeof(field)` and panel 077 refuted it,
//! three hours later, on the case the whole sitting was about.** That predicate
//! asks whether the declared fields *fit*, and a union with padding has room:
//! `union { int32_t i; float f; char pad[128]; }` passes `128 >= 8` and prints
//! **1065353216** — the same false number panel 073's own commit body cites as the
//! defect it killed. `SDL_Event` is a padded union. The sum form is also **blind
//! at one declared field**, where there is nothing to sum against, and that is the
//! shape a binding to a single arm actually has.
//!
//! Two seats found this builtin independently, and the brief that told them C
//! could not answer the question was wrong: it had looked for `__is_union`, which
//! is C++ only.
//!
//! **A premise-death control ships with it** (CLAUDE.md §11). Every assertion here
//! rests on one claim about the world — that this clang still separates 12 from 13
//! — and clang answered 12 for unions before 2019. A toolchain that regresses
//! would turn every check above into one that silently passes, so two assertions
//! state the premise directly and a regression becomes a **build failure** rather
//! than a silence.
//!
//! ## What it is emitted for, and why not for every record
//!
//! Only for a record the program **constructs**, **compares** or **uses as a map
//! key** — including through a containing type, because `==` on a Heroes record
//! walks into its fields' `eq`. Emitting it for every declaration would refuse the
//! read-only bindings that work today, which is the shape panel 073 rejected.
//!
//! **The map key was reachable where `==` was refused**, which is worse than
//! refusing neither: `hash` walks the same overlapping bytes and nothing said so.

use crate::ir::{BinOp, Op, Program, Shape};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

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
    let (constructed, operated) = used(program, ast, checked);
    let mut wanted: Vec<u32> = Vec::new();
    for decl in constructed {
        // **Construction with ONE field is the sound binding and must stay**
        // (panel 073, measured against real SDL3 by two seats): a designated
        // initialiser sets the member it names and C zeroes the rest, which is
        // exactly what a binding to one arm wants. It is naming **two or more**
        // that has no answer, because C keeps the last one written.
        let DeclKind::Record { fields, .. } = &ast.decls[decl as usize].kind else { continue };
        if fields.len() >= 2 {
            wanted.push(decl);
        }
    }
    // **`==`, `hash` and a map key are refused at ANY arity.** With one declared
    // member they still read one arm's bytes out of a value that may hold
    // another, so two values holding different members compare equal whenever the
    // bytes match — the same lie, one field down.
    wanted.extend(operated);
    wanted.sort_unstable();
    wanted.dedup();
    for decl in wanted {
        let DeclKind::Record { fields, header: Some(_), .. } = &ast.decls[decl as usize].kind
        else {
            continue;
        };
        let c_type = names.of(decl).to_string();
        // **The marker carries the DECLARATION's name, never `c_type`** — panel
        // 072 rider 1, which this file broke on the day it was written, one file
        // over from where that defect was fixed. With `tag`, `c_type` is two
        // words, and `ffi_record.rs` reads token 1 of the marker as the name.
        let hero_name = src.slice(ast.decls[decl as usize].name).to_string();
        let named = fields
            .iter()
            .map(|f| src.slice(f.name).to_string())
            .collect::<Vec<_>>()
            .join(" ");
        // `#line` at the declaration's own name: unlike a field assertion, no one
        // field is at fault — what the author must change is the declaration or
        // the expression that builds it.
        let (file, at_line, _) = src.locate(ast.decls[decl as usize].name.start);
        let file = file.to_string();
        w.at_file(&file, at_line);
        w.line(&format!(
            "_Static_assert(__builtin_classify_type(*({c_type} *)0) != 13, \"{UNION_ASSERTION} {hero_name} {named}\");"
        ));
        any = true;
    }
    if any {
        w.at_generated();
        // **The control that fires when the premise dies** (CLAUDE.md §11). Every
        // assertion above rests on one claim about the world — that this clang
        // still answers 13 for a union and 12 for a struct — and clang answered 12
        // for unions before 2019. Without these two lines a toolchain that
        // regresses turns every union check into one that silently passes; with
        // them it is a build failure that names what depends on it.
        w.line(
            "_Static_assert(__builtin_classify_type(*(union { int a; float b; } *)0) == 13, \"a union must classify as 13, or every heroes-ffi-union assertion above is vacuous\");",
        );
        w.line(
            "_Static_assert(__builtin_classify_type(*(struct { int a; float b; } *)0) == 12, \"a struct must classify as 12, or every heroes-ffi-union assertion above refuses every record\");",
        );
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
fn used(program: &Program, ast: &Ast, checked: &Checked) -> (Vec<u32>, Vec<u32>) {
    let mut out: Vec<u32> = Vec::new();
    let mut ops: Vec<u32> = Vec::new();
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
                        reach(&mut ops, ast, checked, ty, 0);
                    }
                    // **A map key hashes, and `hash` walks the same overlapping
                    // bytes `eq` does** (panel 077). Refusing `==` and leaving
                    // `hash` reachable is worse than refusing neither: the wrong
                    // answer moves from a comparison the author wrote to a bucket
                    // they never see.
                    Op::Construct { shape: Shape::Map, .. } => {
                        reach(&mut ops, ast, checked, inst.ty, 0);
                    }
                    Op::MapGet { map, key } => {
                        reach(&mut ops, ast, checked, function.value_type(map), 0);
                        reach(&mut ops, ast, checked, function.value_type(key), 0);
                    }
                    _ => {}
                }
            }
        }
    }
    out.sort_unstable();
    out.dedup();
    ops.sort_unstable();
    ops.dedup();
    (out, ops)
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
