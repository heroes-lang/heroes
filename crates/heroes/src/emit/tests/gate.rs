//! What the backend refuses, and the two things it deliberately does not.
//!
//! Every row of `gate.rs`'s table has a test that makes it fire — LLVM's
//! `test/Verifier` discipline, applied to the other direction (CLAUDE.md §9). A row
//! nobody can make fire is a row nobody can retire, so the tests move as the rows
//! die: M-strings-ownership deleted the `str` and `f64` rows, and
//! M-generics-library step 3 sent the built-in row's example from `sort` to `range`,
//! because `sort` acquired an entry point and stopped being able to demonstrate a
//! refusal. **When a test here has to be rewritten, that is the milestone working**,
//! not the test rotting.

use crate::diagnostics::Kind;

use super::emitted;

/// The refusal's shape, checked once so the other tests can be one line each.
pub(super) fn refusal(text: &str) -> (String, String) {
    let out = emitted(text);
    assert!(out.c.is_empty(), "a refused program must emit no C at all");
    let first = out.diagnostics.first().expect("a refusal");
    assert_eq!(first.kind, Kind::Unsupported, "not an error: the program is fine");
    assert!(first.fixes.is_empty(), "the gate offers no machine-applicable fix");
    // **The note is the row's, not the kind's** (panel 082 R1). It was one string
    // for every row and it was false on most of them — `[ptr]` runs the moment the
    // pointer is held in a record, `[()]` runs as `[i64]`, and `sort` in a generic
    // runs the moment the call instantiates it at an ordered type. A shared
    // assertion on a shared string is how the lie stayed uniform for so long, so
    // what is asserted here is the **invariant** rather than the strings: every row
    // says something about what to do next, and exactly one row is allowed to say
    // that nothing will help. Comparing against `what_the_author_can_do` itself
    // would be tautological and would catch nothing.
    let ends_the_loop = "no change to this file will fix this";
    assert!(
        first.notes.len() >= 2,
        "row `{}` carries no note about what to do next: {:?}",
        first.code,
        first.notes
    );
    let says_nothing_helps = first.notes.iter().any(|n| n == ends_the_loop);
    assert_eq!(
        says_nothing_helps,
        first.code == "missing",
        "only `missing` may tell the author that nothing will help, and row `{}` \
         {} — panel 082 R1: a note that denies a repair the author can make is \
         worse than no note at all. Notes: {:?}",
        first.code,
        if says_nothing_helps { "does" } else { "does not" },
        first.notes
    );
    assert!(
        first.notes.iter().any(|n| n.contains("the backend emits")),
        "the note that answers `what can I do instead` is missing"
    );
    assert!(
        !names_a_milestone(&first.message),
        "a milestone identifier reached the message: {}",
        first.message
    );
    (first.code.clone(), first.message.clone())
}

/// Does this text name a milestone? Both spellings, so the check cannot go stale.
///
/// It used to be three string literals — `"M5"`, `"M6"`, `"M7"` — which guarded the
/// milestones that happened to exist the day it was written and would have guarded
/// only retired ones after the rename to names. The rule it enforces is
/// `emit/gate.rs`'s: **the message names the capability, never the milestone.** Its
/// cost was measured — given `(M5b)`, the panel's llm-ergonomist read an internal
/// tracker id, grepped the repository for it, and told its user the toolchain was
/// broken, a sentence it recorded as false.
///
/// **The name form is the more tempting mistake**, which is why it is refused too:
/// `(M-strings-ownership)` reads plausibly out of context in a way `(M5b)` never
/// did, so it is likelier to survive a review. §4.17's standard is unchanged —
/// everything needed without opening another file — and a milestone id is a
/// pointer into `docs/ROADMAP.md`, which the reader does not have.
///
/// A boundary check, so `ARM64` and `INT64_MIN` are not milestones.
pub(crate) fn names_a_milestone(text: &str) -> bool {
    let bytes = text.as_bytes();
    bytes.iter().enumerate().any(|(i, &b)| {
        if b != b'M' {
            return false;
        }
        // The `M` must start a word: nothing alphanumeric, `_` or `-` before it.
        let opens = i == 0 || {
            let p = bytes[i - 1];
            !p.is_ascii_alphanumeric() && p != b'_' && p != b'-'
        };
        opens
            && match bytes.get(i + 1) {
                Some(&n) if n.is_ascii_digit() => true,
                Some(&b'-') => bytes.get(i + 2).is_some_and(u8::is_ascii_lowercase),
                _ => false,
            }
    })
}

/// M-strings-ownership deleted the `str` and `f64` rows, which is the gate's whole
/// design: a row dies per milestone. The test that used to assert they were refused now
/// asserts they are **not** — a row nobody can prove is gone is a row that comes back.
#[test]
fn text_and_floating_point_are_no_longer_refused() {
    let text = super::c("function main()\n    print(\"hi\")\n    x = 1.5\n    print(x)\n");
    assert!(text.contains("hero_print_str"), "{text}");
    assert!(text.contains("hero_print_f64"), "{text}");
    // …and the note now says every type in the language, because at M-optional-map that
    // is the truth and enumerating them would be a list that only ever gets re-checked
    // when a row dies. What the note still enumerates is the *built-ins*, which is
    // where the remaining rows are.
    assert!(crate::emit::subset().contains("every type in the language"));
    assert!(crate::emit::subset().contains("`keys`"));
}

/// `s[i]` and `xs[i]` are the same instruction, and `len` is one built-in over both:
/// the row splits by operand type rather than by op, which is why the gate walks
/// types as well as operations.
#[test]
fn both_halves_of_a_shared_operation_now_emit_to_their_own_entry_point() {
    let text = super::c("function main()\n    s = \"ab\"\n    print(s[0])\n    print(len(s))\n");
    assert!(text.contains("hero_str_byte"), "{text}");
    assert!(text.contains("hero_str_len"), "{text}");
    // `s[i]` and `xs[i]` are one op, and `len` one built-in: the split is by operand
    // type, and at M-value-aggregates step 5 both sides have somewhere to go. Each
    // keeps its own bounds check in the runtime.
    let array = super::c("function main()\n    xs = [1, 2]\n    print(xs[0])\n    print(len(xs))\n");
    assert!(array.contains("hero_array_at"), "{array}");
    assert!(array.contains("hero_array_len"), "{array}");
    assert!(!array.contains("hero_str_len"), "the wrong half was chosen:\n{array}");
}

/// **The row the ffi-pragmatist vetoed at panel 020, retired at M-ffi-ladder step 5.**
///
/// What replaced it is not a promise but a mechanism: the group's header reaches
/// the prelude as `#include <stdlib.h>`, so the name this call writes is declared
/// by the real header — and the emitter writes no prototype of its own, because
/// one built from Heroes' `int64_t` is `conflicting types` against every C entry
/// point that returns `i64`.
#[test]
fn an_extern_call_is_emitted_against_the_real_header() {
    let out = super::emitted("extern \"stdlib.h\"\n    function labs(x: i64) -> i64\n\nfunction main()\n    print(labs(0 - 3))\n");
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("#include <stdlib.h>"), "{}", out.c);
    // Unmangled: the C name is what the author wrote (CLAUDE.md §7).
    assert!(out.c.contains("labs("), "{}", out.c);
    // **The call, not a substring of the file — and this line has now expired
    // twice, which is the lesson rather than the bug.** It read
    // `!out.c.contains("_labs")`, which died when `extern_probe.rs` began writing
    // `hero_ffi_probe_labs`; then `!out.c.contains("h_scratch_labs")`, which died
    // when that probe's name went through the mangler (panel 053). Both spellings
    // asked *does this text appear anywhere*, and the claim is not about the file:
    // it is about the **call site**, which must name the C function as the author
    // wrote it because the header is the declaration.
    //
    // So the assertion is now the call's own shape. `= labs(` is what the emitter
    // writes for the body; `= h_scratch_labs(` is what it would write if the
    // callee had gone through the mangler. Neither can be produced by a generated
    // name that merely contains the word, and a third generated name will not
    // expire this one (CLAUDE.md §11).
    assert!(out.c.contains("= labs("), "the call must name the C function:\n{}", out.c);
    assert!(!out.c.contains("= h_scratch_labs("), "an extern must not be mangled:\n{}", out.c);
    // And no invented prototype — the header is the declaration.
    assert!(!out.c.contains("int64_t labs"), "the emitter re-declared it:\n{}", out.c);
}

#[test]
fn the_map_is_emitted_and_compares_without_regard_to_order() {
    let out = super::emitted(concat!(
        "function main()\n",
        "    m = {\"a\": 1, \"b\": 2}\n",
        "    print(len(m), !m[\"a\"].is_err())\n",
        "    print(m == {\"b\": 2, \"a\": 1})\n",
    ));
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_map_new(&hero_desc_str, &hero_desc_int"), "{}", out.c);
    assert!(out.c.contains("hero_map_put"), "{}", out.c);
    assert!(out.c.contains("hero_map_find"), "{}", out.c);
    // `hero_map_eq` is order-independent by construction — a pairwise walk of two entry
    // arrays would have made these two literals unequal.
    assert!(out.c.contains("hero_map_eq"), "{}", out.c);
}

/// `m[k]` is **not** `xs[i]`: it yields a `V?` and cannot abort (§4.9). The runtime
/// hands back an address or NULL, because it cannot build the option — that struct is
/// generated per payload type, so the wrapping is the emitter's.
#[test]
fn a_map_lookup_wraps_the_value_in_a_fallible() {
    let out = super::emitted(concat!(
        "function main()\n",
        "    m = {\"a\": 1}\n",
        "    print(m[\"a\"].default(0))\n",
        "    print(m[\"z\"].default(-1))\n",
    ));
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_map_find"), "{}", out.c);
    assert!(out.c.contains("hero_failure_missing_key()"), "{}", out.c);
    // The found value is COPIED through its descriptor, not assigned: the map keeps its
    // own, and the `V?` needs one of its own.
    assert!(out.c.contains("->copy(&"), "{}", out.c);
}

/// The `array_write` row lived for one step and retired with copy-on-write. Stated as
/// the claim it became, because the only way to state it is to emit one.
#[test]
fn writing_one_element_unshares_once_per_array_step() {
    let out = super::emitted(
        "function main()\n    xs: [i64] @ [1, 2]\n    xs[0] @ 7\n    print(xs[0])\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_array_set(&(h0_xs)"), "{}", out.c);
    // One step, so no separate unshare: `set` does it, and doing it twice would be a
    // refcount test run twice rather than a bug.
    assert_eq!(out.c.matches("hero_array_unshare").count(), 0, "{}", out.c);

    // Two array steps, and the outer one is unshared before the inner is reached —
    // which is the whole of panel 022's veto: unsharing level 1 copies its elements,
    // whose `copy` increfs level 2, so level 2 is shared exactly when level 1 was
    // copied.
    let nested = concat!(
        "record Row\n",
        "    cells: [i64]\n",
        "\n",
        "record Grid\n",
        "    rows: [Row]\n",
        "\n",
        "function main()\n",
        "    g: Grid @ Grid(rows: [Row(cells: [0])])\n",
        "    g.rows[0].cells[0] @ 7\n",
        "    print(g.rows[0].cells[0])\n",
    );
    let deep = super::emitted(nested);
    assert!(deep.diagnostics.is_empty(), "{:?}", deep.diagnostics);
    assert_eq!(
        deep.c.matches("hero_array_unshare").count(),
        1,
        "one unshare per array step that is not the last:\n{}",
        deep.c
    );
    assert!(deep.c.contains("hero_array_at_mut"), "{}", deep.c);
    assert!(deep.c.contains("hero_array_set"), "{}", deep.c);
}

/// The row that retired at step 5, stated as the claim it became.
#[test]
fn an_array_is_emitted_with_a_descriptor_for_its_element() {
    let out = emitted("function main()\n    xs = [1, 2, 3]\n    print(len(xs), xs[0])\n");
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_array_new(&hero_desc_int"), "{}", out.c);
    assert!(out.c.contains("hero_array_push"), "{}", out.c);
    // A scalar element uses the runtime's descriptor, so nothing is generated for it.
    assert!(!out.c.contains("static const HeroDesc"), "an unused descriptor:\n{}", out.c);
}

/// An array of records DOES need a generated descriptor, and exactly one.
#[test]
fn an_aggregate_element_gets_one_generated_descriptor() {
    let out = emitted(
        "record P\n    x: i64\n\nfunction main()\n    ps = [P(x: 1), P(x: 2)]\n    print(ps[1].x)\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert_eq!(
        out.c.matches("static const HeroDesc h_scratch_P_desc").count(),
        1,
        "{}",
        out.c
    );
    // And it is emitted before the function that names it, or the C does not compile.
    let desc = out.c.find("h_scratch_P_desc =").expect("the descriptor");
    let use_site = out.c.find("hero_array_new(&h_scratch_P_desc").expect("the use");
    assert!(desc < use_site, "the descriptor must precede its use");
}

/// The row that retired at step 4. A variant is a tagged union by value, and `match`
/// on one is a C `switch` — so the whole capability emits, including the case with no
/// payload, which leaves the union entirely rather than becoming an empty struct.
#[test]
fn a_variant_and_a_match_on_it_are_emitted() {
    let text = concat!(
        "variant Token\n",
        "    num\n",
        "        v: i64\n",
        "    word\n",
        "        text: str\n",
        "    end\n",
        "\n",
        "function size(t: Token) -> i64\n",
        "    return match t\n",
        "        .num n  => n.v\n",
        "        .word w => len(w.text)\n",
        "        .end    => 0\n",
        "\n",
        "function main()\n",
        "    print(size(.num(v: 7)))\n",
    );
    let out = super::emitted(text);
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("typedef enum h_scratch_Token_tag"), "no tag enum:\n{}", out.c);
    assert!(out.c.contains("h_scratch_Token_c_word"), "no payload type:\n{}", out.c);
    // The payload-free case is omitted from the union: C11 has no empty struct.
    assert!(!out.c.contains("c_end;"), "a payload-free case reached the union:\n{}", out.c);
    assert!(out.c.contains("switch ("), "no switch:\n{}", out.c);
    // Only the counted case is touched by the variant's release.
    assert!(out.c.contains("h_scratch_Token_c_word_release"), "{}", out.c);
}

/// When **every** case is payload-free the union goes away entirely. An empty
/// `union { } as;` compiles at `-Wall` and is `error: empty union is a GNU extension`
/// under `-pedantic-errors`, which §4.19 schedules — the landmine panel 023's
/// ffi-pragmatist found while compiling something else.
#[test]
fn a_variant_with_no_payloads_emits_no_union() {
    let text = concat!(
        "variant Color\n",
        "    red\n",
        "    green\n",
        "\n",
        "function pick(c: Color) -> i64\n",
        "    return match c\n",
        "        .red   => 0\n",
        "        .green => 1\n",
        "\n",
        "function main()\n",
        "    print(pick(.red))\n",
    );
    let out = super::emitted(text);
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(!out.c.contains("union"), "an empty union was emitted:\n{}", out.c);
    assert!(out.c.contains("h_scratch_Color_tag tag;"), "{}", out.c);
}

/// The row that retired at this step. Kept as a test rather than deleted, because "a
/// record is emitted" is the claim, and the only way to state it is to run one.
#[test]
fn a_record_is_emitted() {
    let out = emitted("record P\n    x: i64\n\nfunction main()\n    p = P(x: 1)\n    print(p.x)\n");
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("typedef struct h_scratch_P"), "no typedef:\n{}", out.c);
    assert!(out.c.contains("h_scratch_P_eq"), "no generated eq:\n{}", out.c);
    // Uncounted, so it needs no retain at all — and generating one anyway would be a
    // function nothing calls.
    assert!(!out.c.contains("h_scratch_P_retain"), "an uncounted record got a retain");
}

/// And the counted case, which is the one with a runtime obligation.
#[test]
fn a_record_holding_a_str_gets_a_retain_and_a_release() {
    let out = emitted(
        "record H\n    name: str\n\nfunction main()\n    h = H(name: \"a\")\n    print(h.name)\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("void h_scratch_H_retain(const h_scratch_H *v)"), "{}", out.c);
    assert!(out.c.contains("hero_str_incref(v->f_name);"), "{}", out.c);
    assert!(out.c.contains("hero_str_decref(v->f_name);"), "{}", out.c);
}

#[test]
fn a_fallible_value_is_emitted_as_a_tagged_union_by_value() {
    let out = super::emitted(
        "function half(n: i64) -> i64?\n    if n % 2 == 0\n        return ok(n / 2)\n    return fail(\"odd\", \"not even\")\n\nfunction main()\n    print(half(4).default(0))\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    // One generated struct per distinct `T?`, named by index because `int?` and
    // `[i64]?` sanitise to the same identifier — and by a **leading digit**,
    // which a Heroes identifier cannot carry, so no `record opt0` can spell it
    // (2026-08-12; before that it was `h_scratch_opt0` and a user type of that
    // name gave `redefinition`, exit 2, on a legal program).
    assert!(out.c.contains("typedef struct h_scratch_0opt0"), "{}", out.c);
    assert!(out.c.contains("int64_t tag;"), "{}", out.c);
    assert!(out.c.contains("HeroFailure err;"), "{}", out.c);
    // Every `T?` is counted whatever `T` is, because the error side is two `str`s.
    assert!(out.c.contains("h_scratch_0opt0_release"), "{}", out.c);
    assert!(out.c.contains("hero_failure_release"), "{}", out.c);
}

/// `.must()` emits from M-generics-library step 1, and the row that mattered is the
/// ARGUMENT: the failure travels with the abort, so the panic names the code and msg
/// the author wrote rather than only that a `.must()` failed.
#[test]
fn must_carries_its_failure_into_the_abort() {
    let out = super::emitted(
        "function half(n: i64) -> i64?\n    return ok(n / 2)\n\nfunction main()\n    print(half(4).must())\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_panic_must("), "{}", out.c);
    // Not a bare call: something is passed, and it is the failure read in the abort
    // block itself — so nothing counted crosses a block edge.
    assert!(!out.c.contains("hero_panic_must();"), "the failure was dropped:\n{}", out.c);
    assert!(out.c.contains(".as.err"), "{}", out.c);
}


/// The `generics` row retired at M-generics-library step 6, and the test that made it
/// fire now asserts the opposite — a row nobody can prove is gone is a row that comes
/// back.
///
/// **This test cannot use `super::emitted`**, and the reason is the step's own
/// resolution: monomorphisation is a *pass*, so a generic program is only
/// emittable after it has run, and the helper stops at lowering. The end-to-end
/// evidence is `tests/golden/run/generics.hero`; what is asserted here is the
/// narrower claim that the gate no longer has anything to say about a type
/// parameter.
#[test]
fn generics_are_no_longer_refused() {
    assert!(!crate::emit::subset().contains("generic"));
}

/// The other half of §4.19's guarantee, which clang does not give for free: one
/// `_Static_assert` per `extern`, over a `_Generic` whose controlling call C11
/// 6.5.1.1p3 does not evaluate. Without it, `extern function sqrt(x: f64) -> i64`
/// compiles clean and prints `1` — measured at panel 036.
#[test]
fn every_extern_carries_a_return_type_assertion() {
    let out = super::emitted("extern \"stdlib.h\"\n    function labs(x: i64) -> i64\n\nfunction main()\n    print(labs(0 - 3))\n");
    assert!(out.c.contains("_Static_assert(HERO_RET_INT(labs((int64_t)0))"), "{}", out.c);
    // The message is the contract with `emit::ffi::explain`: marker, C name,
    // declared type — so a failure names the author's line, not generated C.
    assert!(out.c.contains("heroes-ffi-return labs i64"), "{}", out.c);
}

/// `xs.len()` lowers to `call builtin len` while `for` lowers to `Op::Len`. Gating
/// the op alone would leave an undefined symbol at link time — a linker error is
/// the failure class the gate exists to prevent.
/// **The by-name row can no longer fire, and the invariant replaces the example.**
///
/// Every one of the 23 reserved names is now accounted for: eleven have a C entry
/// point, seven are declarations in `library/source.hero`, and five are lowered to
/// something other than a call (`ok`, `fail`, `must`, `default`, `is_err`). So
/// there is no program that reaches `callee_note`'s row.
///
/// The row stays — unlike `cow_check`, which was struck at M-strings-ownership for the
/// same symptom — because it is the net under a *mismatch*: a name added to `BUILTINS`
/// with no entry point and no library body would otherwise be an undefined symbol at
/// link. What replaces the example is this partition, asserted, which is a stronger
/// check than any one program: an example proves one name is refused, and this proves
/// none can be.
#[test]
fn every_reserved_name_is_emitted_lowered_or_written_in_heroes() {
    use crate::resolve::BUILTINS;
    // Lowered to something that is not a call: `ok`/`fail` are constructions,
    // `must` is an abort, `default` and `is_err` are branches on a tag (§4.6),
    // and `cstr` is an `Op::Cast` — §4.3's no-implicit-conversions rule would be
    // invisible in the dump if the conversion hid inside a call (M-ffi-ladder).
    const LOWERED: [&str; 6] = ["cstr", "default", "fail", "is_err", "must", "ok"];
    let library = crate::library::SOURCE;
    let mut unaccounted: Vec<&str> = Vec::new();
    for builtin in BUILTINS {
        let emitted = super::super::EMITTED_BUILTINS.contains(&builtin.name);
        let lowered = LOWERED.contains(&builtin.name);
        let written = library.contains(&format!("function {}", builtin.name))
            || library.contains(&format!("function {}<", builtin.name));
        if !emitted && !lowered && !written {
            unaccounted.push(builtin.name);
        }
    }
    assert!(
        unaccounted.is_empty(),
        "these reserved names have no entry point, no library body and no lowering: \
         {unaccounted:?} — each would be an undefined symbol at link"
    );
}

/// A program the **checker accepts** and this backend cannot emit — which after
/// panel 068 is one shape and no longer two.
///
/// `sort([Point])` used to be the example and is now `unordered_element` at exit
/// 1, in `types/ordering.rs`, on the author's own line. What is left here is the
/// route through a **generic**: inside `first<A>(xs: [A])` the element is not yet
/// a type, `first([3, 1, 2])` runs and prints, and only monomorphisation — which
/// is in the IR, which `heroes check` never reaches — can see that this call
/// instantiated `A` at a record.
///
/// So this test is the last thing between that program and `hero_cmp_for`
/// returning `NULL`, where the runtime's own message says *compiler bug*. If it
/// goes green by accident — someone deletes the arm believing the checker covers
/// it — a correct-looking program starts aborting at 134 with no line.
#[test]
fn a_builtin_whose_operand_type_has_no_order_is_refused_by_operand() {
    let (code, message) = refusal(concat!(
        "record Point\n    x: i64\n    y: i64\n\n",
        "function firstof<A>(xs: [A]) -> A\n    ys = sort(xs)\n    return ys[0]\n\n",
        "function main()\n    print(firstof([Point(x: 1, y: 2)]).x)\n",
    ));
    assert_eq!(code, "builtin");
    assert!(message.contains("`sort`"), "the message must name it: {message}");
    assert!(
        message.contains("generic"),
        "and it must name the route, because the element type on that line is spelled `A` \
         and telling the reader to change it sends them nowhere: {message}"
    );
    assert!(
        message.contains("numbers, `str` and `bool`"),
        "and what does work, or the reader has to guess: {message}"
    );
}

/// The refusal that **left** this file, asserted from the other side (panel 068 R2).
///
/// `sort` on a written-down record type is the checker's now, so the gate must
/// never see it. This is the pair to the test above: together they pin that
/// exactly one of the two answers fires for each shape, which is what stops the
/// class of defect where a rule is moved and the old copy is left behind to
/// double-report.
#[test]
fn a_sort_on_a_written_element_type_never_reaches_this_gate() {
    let src = crate::source::Source::new(
        "scratch.hero".to_string(),
        "record Point\n    x: i64\n    y: i64\n\nfunction main()\n    ps = [Point(x: 1, y: 2)]\n    \
         print(len(sort(ps)))\n"
            .to_string(),
    );
    let parsed = crate::syntax::parse(&src);
    let resolved = crate::resolve::resolve(&parsed.ast, &src);
    let checked = crate::types::check(&parsed.ast, &resolved, &src);
    let codes: Vec<&str> = checked.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert_eq!(
        codes,
        ["unordered_element"],
        "`sort` on `[Point]` is the CHECKER's since panel 068 R2, at exit 1 on the author's \
         line. If this list is empty the program reaches `emit/builtins.rs`'s backstop and the \
         author is told `heroes check` was happy; if it has two entries, the rule was moved and \
         the old copy left behind, and one mistake is reported twice."
    );
}

#[test]
fn every_unsupported_capability_is_reported_not_only_the_first() {
    let out = emitted(concat!(
        "record Point\n",
        "    x: i64\n",
        "    y: i64\n",
        "\n",
        // The generic route, since panel 068 moved the direct one to the checker.
        "function firstof<A>(xs: [A]) -> A\n",
        "    ys = sort(xs)\n",
        "    return ys[0]\n",
        "\n",
        "function main()\n",
        "    print(firstof([Point(x: 1, y: 2)]).x)\n",
        "    us: [()] @ []\n",
        "    print(len(us))\n",
    ));
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert!(codes.contains(&"builtin"), "{codes:?}");
    // Two capabilities the backend lacks — one keyed to a name, one to an operand
    // type — and both are named, so the list arrives at once.
    let messages: Vec<&str> = out.diagnostics.iter().map(|d| d.message.as_str()).collect();
    assert!(messages.iter().any(|m| m.contains("`sort`")), "{messages:?}");
    assert!(messages.iter().any(|m| m.contains("()")), "{messages:?}");
    // Sorted by span: three invocations to learn three facts is what the message
    // carrying the list exists to prevent.
    let spans: Vec<u32> = out.diagnostics.iter().map(|d| d.span.start).collect();
    let mut sorted = spans.clone();
    sorted.sort_unstable();
    assert_eq!(spans, sorted);
}

#[test]
fn one_capability_is_one_diagnostic_however_many_times_it_appears() {
    let out = emitted(concat!(
        "record Point\n",
        "    x: i64\n",
        "\n",
        "function thrice<A>(xs: [A]) -> i64\n",
        "    return len(sort(xs)) + len(sort(xs)) + len(sort(xs))\n",
        "\n",
        "function main()\n",
        "    print(thrice([Point(x: 1)]))\n",
    ));
    // Three calls, and `sort` on a record is named ONCE.
    let messages: Vec<&str> = out.diagnostics.iter().map(|d| d.message.as_str()).collect();
    assert_eq!(
        messages.iter().filter(|m| m.contains("`sort`")).count(),
        1,
        "a program that calls one unsupported built-in three times has one problem: {messages:?}"
    );
    assert_eq!(
        messages.len(),
        messages.iter().collect::<std::collections::BTreeSet<_>>().len(),
        "every capability appears exactly once: {messages:?}"
    );
}

/// A `test` block is **skipped**, not refused: `ir/mod.rs` says ordinary builds
/// ignore it, and refusing it would make a file unbuildable at M-scalars-run and still
/// unbuildable after M-strings-ownership and M-value-aggregates.
#[test]
fn a_test_block_does_not_stop_the_build_and_does_not_reach_the_c() {
    let out = emitted("function double(n: i64) -> i64\n    return n * 2\n\ntest \"doubling\"\n    assert double(2) == 4\n\nfunction main()\n    print(double(3))\n");
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics[0].message);
    assert!(!out.c.contains("doubling"), "a test block reached the translation unit");
    assert!(out.c.contains("h_scratch_double"));
}

/// A file with no `main` is a perfectly good translation unit. Refusing it here
/// would make `--emit-c` useless for a library, and the binary's need for an entry
/// point belongs to the driver.
#[test]
fn a_file_with_no_main_emits_a_unit_with_no_shim() {
    let out = emitted("function double(n: i64) -> i64\n    return n * 2\n");
    assert!(out.diagnostics.is_empty());
    assert!(out.c.contains("h_scratch_double"));
    assert!(!out.c.contains("int main(void)"));
    assert!(crate::emit::entry_point(&crate::emit::tests::gate::program_of("function double(n: i64) -> i64\n    return n * 2\n")).is_none());
}

/// The helper the test above needs: `entry_point` answers over a `Program`, and the
/// question is worth asking through the real lowering rather than a stub.
pub(super) fn program_of(text: &str) -> crate::ir::Program {
    use crate::ir::lower;
    use crate::resolve::resolve;
    use crate::source::Source;
    use crate::syntax::parse;
    use crate::types::check;
    let src = Source::new("scratch.hero".to_string(), text.to_string());
    let parsed = parse(&src);
    let resolved = resolve(&parsed.ast, &src);
    let checked = check(&parsed.ast, &resolved, &src);
    lower(&parsed.ast, &resolved, &checked, &src).program
}

/// The guard above has a test that makes it fire, in both spellings and in the
/// negative direction (CLAUDE.md §9). The negatives are the ones that matter: they
/// are strings this compiler really emits, and a guard that flagged them would be
/// retired by the first person it inconvenienced.
#[test]
fn a_milestone_identifier_is_recognised_in_either_spelling() {
    for named in [
        "the backend cannot emit this yet (M5b)",
        "the backend cannot emit this yet (M-strings-ownership)",
        "scheduled for M8c",
        "scheduled for M-selfhost-fixpoint",
        "M0 did not exist",
    ] {
        assert!(names_a_milestone(named), "not caught: {named}");
    }
    for clean in [
        "the backend emits int64_t, double and bool",
        "INT64_MIN % -1 does not trap on ARM64",
        "hero_str_from_bytes rejected the input",
        "expected ')' but found a name",
        "a MODULE name may not be a keyword",
        "MAX is not a builtin",
        "h_geom_dist2 is declared twice",
    ] {
        assert!(!names_a_milestone(clean), "false positive: {clean}");
    }
}

/// **No `_Generic` association names `void`**, in any program.
///
/// A FIXED DEFECT, named after it (Go's `test/fixedbugs`), 2026-08-13.
///
/// SYMPTOM: every program that binds a C function returning nothing emitted
/// `#define HERO_RET_UNIT(c) _Generic((c), void:1, default:0)`, and every
/// program in the corpus binds one — `hero_exit` comes with the library. **It
/// compiled here and nowhere else.** The first CI run that ever put this project
/// on two machines failed on both: `error: type 'void' in generic association
/// incomplete`.
///
/// CAUSE: C11 6.5.1.1p2 — *"The type name in a generic association shall specify
/// a complete object type"* — and `void` is incomplete. It was a constraint
/// violation from the day §4.19's return assertion was written. This laptop's
/// clang accepts it and says why when asked: *"incomplete type 'void' in a
/// '_Generic' association is a **C2y extension**"*. One compiler was reading a
/// future standard and nothing else was.
///
/// FIX: `__builtin_types_compatible_p(__typeof__(c), void)`, which clang and GCC
/// both give, which takes `void` without complaint, and whose operand is
/// unevaluated exactly as `_Generic`'s is — so the call is still never made.
///
/// This test is the net rather than the fix: it reads the emitted text and looks
/// for the shape, because the thing that hid the defect for two milestones was
/// that **only a compiler could see it**, and only some compilers at that. A
/// string search sees it on every machine.
#[test]
fn fixedbugs_no_generic_association_names_void() {
    let out = emitted(
        "extern \"stdlib.h\"\n    function abort() -> ()\n\nfunction main()\n    abort()\n",
    );
    assert!(out.c.contains("HERO_RET_UNIT"), "the unit return assertion is gone: {}", out.c);
    for line in out.c.lines().filter(|line| line.contains("_Generic")) {
        assert!(
            !line.contains("void:"),
            "`void` is an incomplete type and C11 6.5.1.1 forbids it as a generic \
             association — one clang takes it as a C2y extension and the rest refuse it:\n{line}"
        );
    }
}
