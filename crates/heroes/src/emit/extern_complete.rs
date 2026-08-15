//! The probe that asks whether a group `record` named **every** field the header's
//! struct has (design.md §4.19; panel 061, and panel 062 for the array).
//!
//! Split from `extern_record.rs` on 2026-08-15, and the seam is that this asks a
//! different question with a different mechanism. Its sibling asks, of a field the
//! program **names**, whether the header agrees — a `_Generic` this emitter wrote,
//! whose failure carries a marker. This asks about the fields the program did
//! **not** name, and the only thing that can answer is clang's own
//! `-Wmissing-field-initializers` on a positional list.
//!
//! Keeping them apart is what makes the difference visible: one is a contract, the
//! other is a borrowed diagnostic, and the second cost this project a silently
//! wrong colour before it existed.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind, Field};
use crate::types::{Checked, Ty};

use super::typedefs::Names;
use super::writer::Writer;

use super::extern_record::COMPLETE_PROBE;

/// One never-called function per group `record`, whose whole body is a
/// **positional** initialiser with one zero per field the author named.
///
/// ```c
/// __attribute__((unused)) static void hero_ffi_complete_h_m_Color(void)
/// { Color v = {0,0,0}; (void)v; }
/// /* -> error: missing field 'a' initializer */
/// ```
///
/// **This is the check the field assertions cannot make.** They ask, of every field
/// the program *names*, whether the header agrees — and say nothing at all about a
/// field the program left out. `record Color` with three of raylib's four members
/// therefore compiled with every assertion green, zero warnings, **exit 0**, and
/// returned `0xFF000000` where C returns `0xFF0000FF`: a transparent colour where
/// the program asked for an opaque one (panel 061, reproduced by two judges).
///
/// **Positional, and that is the entire mechanism.** The designated form this
/// emitter writes everywhere else — `(Color){.r = …, .g = …}` — warns about
/// nothing, in any clang, even under `-Wextra`, which is precisely why the defect
/// was reachable. A positional list is the one form C obliges the compiler to count.
///
/// **Local pragmas rather than a flag in `FLAGS`**, and the reason is measured: the
/// emitter writes `= {0}` for every refcounted slot (panel 021's zero-initialiser,
/// which is what makes cleanup unconditional), and a global
/// `-Wmissing-field-initializers` fires on all of them. The warning has to be armed
/// where it is wanted and disarmed immediately.
///
/// `-Wmissing-braces` is silenced inside the region for the same reason it is not a
/// defect: a nested record's `0` is C's brace elision, which is legal and says
/// nothing about completeness.
///
/// **A union gives no signal, correctly.** Naming one member of a union *is* naming
/// all of it, and `{0}` initialises the first member with no warning — so
/// `SDL_Event` with one member declared passes, which is the right answer rather
/// than a hole.
pub(super) fn completeness_probes(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let records: Vec<(usize, &Vec<Field>)> = ast
        .decls
        .iter()
        .enumerate()
        .filter_map(|(index, decl)| match &decl.kind {
            // **`partial` is exactly the absence of this probe**, and that is the
            // whole of what the word buys. Everything else it does is a *refusal*
            // — `==`, `hash`, a map key — so if it did not also switch this off it
            // would be a word that costs and gives nothing.
            DeclKind::Record { fields, header: Some(_), partial: false, .. }
                if !fields.is_empty() =>
            {
                Some((index, fields))
            }
            _ => None,
        })
        .collect();
    if records.is_empty() {
        return;
    }
    w.at_generated();
    w.line("#pragma clang diagnostic push");
    w.line("#pragma clang diagnostic error \"-Wmissing-field-initializers\"");
    w.line("#pragma clang diagnostic ignored \"-Wmissing-braces\"");
    for (index, fields) in records {
        let decl = &ast.decls[index];
        let c_type = names.of(index as u32);
        let probe = format!("{COMPLETE_PROBE}{}", names.satellite(index as u32));
        // **A nested record's slot is `{0}`, not `0`, and the difference is not
        // cosmetic** (found 2026-08-15 by re-measuring, hours after this probe
        // shipped). C's **brace elision** lets a flat zero list spill into an inner
        // struct's members, so `Camera2D v = {0,0,0,0}` fills `offset.x`,
        // `offset.y`, `target.x` and `target.y` — and clang then reports
        // `rotation` missing on a record that names **all four** of its fields.
        //
        // There was no `.hero` text that satisfied it, and the diagnostic's own
        // note said *"add the field"*. A message naming a repair that does not
        // exist is Nim issue #19040's shape — which panel 061 cited **against**
        // option E, and which this shipped four hours later. Ten of raylib's
        // thirty-five structs were unbindable for this reason alone.
        //
        // `{0}` is the universal zero initialiser and clang exempts it from
        // `-Wmissing-field-initializers`, so the outer count is what gets counted.
        // Chosen over expanding the inner record recursively because that would
        // break the moment an inner record is itself `partial`, and because it is
        // one token.
        let zeros: Vec<&str> = fields
            .iter()
            .map(|field| {
                // **A record OR a fixed array**, and the array joined this rule
                // the day it existed rather than after a second measurement: both
                // are aggregates, and C's brace elision spends a flat zero list
                // into either of them. `Matrix projection[2]` is both at once.
                let aggregate = checked.written_type(field.ty).is_some_and(|ty| {
                    matches!(checked.types.get(ty), Ty::Named(_) | Ty::Fixed(_, _))
                });
                if aggregate { "{0}" } else { "0" }
            })
            .collect();
        let zeros = zeros.join(",");
        // The declaration's own line, so the verdict lands where the author can act
        // on it — the same contract `extern_probe.rs` keeps for parameters.
        let (file, line, _) = src.locate(decl.name.start);
        let file = file.to_string();
        w.at_file(&file, line);
        w.line(&format!(
            "__attribute__((unused)) static void {probe}(void) {{ {c_type} v = {{{zeros}}}; (void)v; }}"
        ));
    }
    w.at_generated();
    w.line("#pragma clang diagnostic pop");
    w.blank();
}

