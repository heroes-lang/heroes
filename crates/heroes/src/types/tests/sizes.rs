//! Which types have a size, and the order the backend gets them in (panel 023).
//!
//! Two things live here that no golden case can hold. The first is the **accepted**
//! side of the rule: `tests/golden/check/` asserts exit 1 on every case it owns, so
//! a legal program belongs in a unit test. The second is `Checked::type_order`,
//! which is not a diagnostic at all — it is the answer the emitter consumes, and
//! the whole reason the cycle check and the topological sort are one walk.
//!
//! The accepted cases land in the same commit as the check because panel 023's
//! historian predicted the first defect would be a **false positive**: a walk that
//! marks types "visited" rather than treating `[T]` as an edge it does not traverse
//! rejects `A`/`B` below, a cycle that passes through an array exactly once. Go's
//! own type-checker test data has `A5 [10]A6 / A6 *A5` legal for the same reason,
//! and every over-eagerness bug the historian found in rustc (#68748, #144617,
//! #31299) is a false positive rather than a missed cycle.

use super::{checked, diagnostics};

/// The predicted false positive. One array on the path is enough.
#[test]
fn a_cycle_through_an_array_has_a_size() {
    let text = "\
record A
    xs: [B]

record B
    a: A

function main()
    print(1)
";
    assert_eq!(diagnostics(text), "");
}

/// design.md §4.10's own example, and the reason `[T]` exists at all: an array is
/// a pointer, so `Expr` is finite and needs no `Box`.
#[test]
fn the_recursive_variant_from_the_design_has_a_size() {
    let text = "\
variant Expr
    num
        v: int
    sum
        children: [Expr]

function main()
    print(1)
";
    assert_eq!(diagnostics(text), "");
}

/// A map and a function value are pointers too, and the rule admits them without
/// naming them — because it is the by-value *property*, not a list of permitted
/// indirections. The panel's proposal listed `[T]` and `{K: V}` and got both of
/// these wrong: it rejected the function field, which compiles and runs.
#[test]
fn the_other_indirections_are_admitted_without_being_named() {
    let text = "\
record Scope
    vars: {str: Scope}

record Handler
    on_next: (function(Handler) -> int)

function main()
    print(1)
";
    assert_eq!(diagnostics(text), "");
}

/// A diamond is not a cycle. Two fields reaching one type is the shape a
/// `visited` flag handles correctly and a walk without colours does not.
#[test]
fn two_fields_of_one_type_are_not_a_cycle() {
    let text = "\
record Point
    x: int
    y: int

record Rect
    a: Point
    b: Point

function main()
    print(1)
";
    assert_eq!(diagnostics(text), "");
}

/// A `T?` is transparent, so the cycle is real — but only *through* the `T?`. Put
/// an array anywhere on the path and the same declarations are legal.
#[test]
fn a_fallible_field_breaks_no_cycle_but_an_array_inside_one_does() {
    let bad = "\
record Node
    next: Node?

function main()
    print(1)
";
    assert_eq!(
        diagnostics(bad),
        "test.hero:2:11: error[no_size]: `record Node` contains itself, so it has no size\n"
    );
    let good = "\
record Node
    kids: [Node?]

function main()
    print(1)
";
    assert_eq!(diagnostics(good), "");
}

/// One diagnostic for one cycle, however many back edges close it. `Node` holds
/// two of itself; that is one mistake, and `types/lower.rs`'s rule applies.
#[test]
fn two_self_fields_are_one_diagnostic() {
    let text = "\
record Node
    left: Node
    right: Node

function main()
    print(1)
";
    assert_eq!(
        diagnostics(text).lines().count(),
        1,
        "a cycle is one mistake, so it gets one message"
    );
}

// --- the order, which is the other half of the walk -----------------------

/// Dependencies before their users, whatever order the file declares them in.
///
/// This is what C needs and Heroes does not have: the top level is order-free with
/// free mutual recursion (§4.2), and a C struct must be complete before it is used
/// by value. Measured by panel 023's ffi-pragmatist: a forward `typedef struct T
/// T;` does **not** rescue a by-value field, so the sort is mandatory rather than
/// tidy.
#[test]
fn the_order_puts_a_dependency_before_its_user() {
    let text = "\
record Outer
    middle: Middle

record Middle
    inner: Inner

record Inner
    v: int

function main()
    print(1)
";
    let (out, _) = checked(text);
    // Declaration indices: Outer 0, Middle 1, Inner 2. C needs the reverse.
    assert_eq!(out.type_order, vec![2, 1, 0]);
}

/// An indirection imposes no ordering constraint, so a type reached only through
/// one may appear either side of its user — but every aggregate is still in the
/// list exactly once, because the descriptor pass filters this order and cannot
/// invent a row that is missing from it.
#[test]
fn every_aggregate_appears_exactly_once() {
    let text = "\
record A
    xs: [B]

record B
    a: A

record Loose
    v: int

function main()
    print(1)
";
    let (out, _) = checked(text);
    let mut seen = out.type_order.clone();
    seen.sort();
    assert_eq!(seen, vec![0, 1, 2]);
}

/// A cycle publishes no order. There is none, and the emitter never runs on a
/// program with diagnostics — but the field must not carry a half-built answer that
/// a later pass could mistake for a whole one.
#[test]
fn a_cycle_leaves_no_order_to_mistake_for_one() {
    let text = "\
record Node
    child: Node

function main()
    print(1)
";
    let (out, _) = checked(text);
    assert!(
        !out.type_order.contains(&0),
        "a declaration on a cycle is never ordered: {:?}",
        out.type_order
    );
}

// --- the invariant, over a corpus nobody wrote ----------------------------

/// **Every program the checker accepts has a `type_order` that is a real
/// topological order.** Asserted over `heroes mutate`'s corpus rather than over
/// cases somebody thought of (CLAUDE.md §9, `llvm-opt-fuzzer`'s argument with
/// `verifyModule`).
///
/// Two halves, and each catches a different way to be wrong. *Completeness*: every
/// aggregate appears exactly once, because the descriptor pass filters this list
/// and cannot invent a row missing from it — a dropped declaration would surface as
/// a C type with no `typedef`, at link time. *Order*: every by-value dependency
/// precedes its user, which is the property C needs and the only reason the list
/// exists.
///
/// The mutants are what make this worth running. `heroes mutate` makes one
/// plausible mistake per site, so it produces field types, declaration orders and
/// containment shapes that no hand-written case would contain — and every mutant
/// that still checks clean is a program this invariant must hold for.
#[test]
fn every_accepted_program_has_a_real_topological_order() {
    use crate::mutate::{corpus, mutants, OPERATORS};
    use crate::resolve::resolve;
    use crate::source::Source;
    use crate::syntax::{parse, DeclKind};

    let mut programs = 0;
    let mut ordered_aggregates = 0;
    for (name, text) in corpus() {
        let mut variants: Vec<String> = vec![text.clone()];
        for operator in OPERATORS {
            variants.extend(mutants(operator.id, &name, &text));
        }
        for variant in variants {
            let src = Source::new(name.clone(), variant);
            let parsed = parse(&src);
            if !parsed.diagnostics.is_empty() {
                continue;
            }
            let resolved = resolve(&parsed.ast, &src);
            if !resolved.diagnostics.is_empty() {
                continue;
            }
            let out = super::super::check(&parsed.ast, &resolved, &src);
            if !out.diagnostics.is_empty() {
                continue;
            }
            programs += 1;

            let aggregates: Vec<u32> = parsed
                .ast
                .decls
                .iter()
                .enumerate()
                .filter(|(_, decl)| {
                    matches!(decl.kind, DeclKind::Record { .. } | DeclKind::Variant { .. })
                })
                .map(|(index, _)| index as u32)
                .collect();
            let mut seen = out.type_order.clone();
            seen.sort();
            assert_eq!(
                seen, aggregates,
                "{name}: the order must hold every aggregate exactly once — a missing one \
                 becomes a C type with no typedef, at link time"
            );
            ordered_aggregates += out.type_order.len();

            let position = |decl: u32| {
                out.type_order
                    .iter()
                    .position(|&d| d == decl)
                    .expect("every aggregate is in the order")
            };
            for (from, to) in super::super::sized::by_value_edges(&parsed.ast, &resolved) {
                assert!(
                    position(to) < position(from),
                    "{name}: {to} is laid out inside {from} by value, so C needs it first, \
                     and the published order has it second: {:?}",
                    out.type_order
                );
            }
        }
    }
    // Both counts must be non-zero or this test asserts nothing: no accepted
    // programs would mean the corpus never got past the checker, and no ordered
    // aggregates would mean it contains no records at all.
    // Floors from the measured run, not from taste, so that a shrinking corpus or a
    // mutation operator that silently stops producing anything fails here instead of
    // making this test quietly assert nothing.
    //
    // MEASURED, and the number moved once already: at M5c step 1 this corpus held 66
    // accepted programs and only **14** ordered aggregates, because it predated
    // aggregates being emittable and almost nothing in it declared a record. The same
    // coverage hole `emit/tests/mutants.rs` wrote down when it produced zero mutants on
    // its first run — and it closed the way that one did, by the milestone's own cases
    // joining the corpus: **76 programs, 35 aggregates** today.
    //
    // The floors carry margin under those, so a shrinking corpus or a mutation operator
    // that silently stops producing anything fails here instead of letting this test
    // quietly assert nothing.
    assert!(programs >= 70, "only {programs} accepted programs — the corpus is not being read");
    assert!(
        ordered_aggregates >= 30,
        "only {ordered_aggregates} ordered aggregates: the corpus is not exercising the order"
    );
}
