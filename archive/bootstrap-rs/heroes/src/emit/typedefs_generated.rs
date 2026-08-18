//! The names the **emitter invents**, as opposed to the ones the author declared
//! (design.md §3.1; panel 031 R10).
//!
//! Split out of `typedefs.rs` on 2026-08-15, along the seam that file\'s own module
//! doc already draws: a declared type is named after something in the source, and
//! a `T?` or a function type is named after nothing at all. The **leading digit**
//! exists to keep the two apart — `h_<module>_<N>opt<N>` carries one and a Heroes
//! identifier cannot — so this is where that rule lives and where a new invented
//! name has to come to get one.
//!
//! The two builders differ in where they look, and the difference is a fixed
//! defect rather than a preference. `with_options` walks the **interned arena**,
//! because a `T?` reached only through a declared record\'s field is invisible to a
//! walk over the emitted functions — and a missing name is a hard error found by
//! the mutant corpus rather than by any case somebody wrote. `with_functions`
//! walks the **program**, because the checker interns a `Ty::Func` for every
//! top-level declaration and an arena walk would emit a typedef per function in
//! the file, almost all of them named by nothing and none of them able to fall out
//! of `--emit-c`.

use crate::types::{Checked, Ty, TyId};

use super::ctype::{collect, mentions_generic};
use super::typedefs::{Names, SYNTHETIC};

impl Names {
    pub(super) fn with_options(mut self, module: &str, checked: &Checked) -> Names {
        for index in 0..checked.types.len() {
            let id = TyId(index as u32);
            // **A type that still mentions a parameter has no C declaration**, and
            // the arena still holds the templates': monomorphisation deletes the
            // generic *functions*, not the `A?` their signatures interned. Emitting
            // one gave `HeroValue ok;` — `error: unknown type name 'HeroValue'` —
            // because `c_type`'s catch-all is the only arm a `Ty::Generic` reaches.
            if mentions_generic(checked, id) {
                continue;
            }
            if matches!(checked.types.get(id), Ty::Fallible(_)) {
                let at = self.options.len();
                self.options.insert(id.0, format!("h_{module}_{SYNTHETIC}opt{at}"));
            }
        }
        self
    }

    /// Assigns a C typedef name to every function type the program **uses as a
    /// type** — a slot, a temporary, a parameter.
    ///
    /// Derived from the program, not walked over the interned arena, and for the
    /// same reason `descriptors.rs` derives its set: the checker interns a
    /// `Ty::Func` for *every* top-level declaration, so an arena walk emits a
    /// typedef per function in the file, almost all of them named by nothing. An
    /// unused typedef is not a warning the way an unused `static const` is, which
    /// is exactly why it has to be deliberate — it would sit in `--emit-c`
    /// forever with nothing to make it fall out.
    pub(super) fn with_functions(
        mut self,
        module: &str,
        checked: &Checked,
        program: &crate::ir::Program,
    ) -> Names {
        let mut used: std::collections::BTreeSet<u32> = std::collections::BTreeSet::new();
        for function in &program.functions {
            for slot in &function.slots {
                collect(checked, slot.ty, &mut used);
            }
            for value in &function.values {
                collect(checked, *value, &mut used);
            }
        }
        // ORDER: ascending TyId — the `fn{at}` suffix is the walk position, so
        // this order names symbols file-wide; the Heroes port owes an explicit
        // sort (design.md §4.9). Pinned by the emit golden
        // `order-options-and-fn-typedefs`: the double-emit test pins stability,
        // not order — one binary emits both files (panel 065).
        for id in &used {
            let at = self.funcs.len();
            self.funcs.insert(*id, format!("h_{module}_{SYNTHETIC}fn{at}"));
        }
        self
    }
}
