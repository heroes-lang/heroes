//! What the backend refuses, and the two things it deliberately does not.
//!
//! Every row of `gate.rs`'s table has a test that makes it fire — LLVM's
//! `test/Verifier` discipline, applied to the other direction (CLAUDE.md §9). A row
//! nobody can make fire is a row nobody can retire, so the tests move as the rows
//! die: M5b deleted the `str` and `f64` rows, and M6 step 3 sent the built-in row's
//! example from `sort` to `range`, because `sort` acquired an entry point and stopped
//! being able to demonstrate a refusal. **When a test here has to be rewritten, that
//! is the milestone working**, not the test rotting.

use crate::diagnostics::Kind;

use super::emitted;

/// The refusal's shape, checked once so the other tests can be one line each.
fn refusal(text: &str) -> (String, String) {
    let out = emitted(text);
    assert!(out.c.is_empty(), "a refused program must emit no C at all");
    let first = out.diagnostics.first().expect("a refusal");
    assert_eq!(first.kind, Kind::Unsupported, "not an error: the program is fine");
    assert!(first.fixes.is_empty(), "no edit to the file will fix this");
    assert!(
        first.notes.iter().any(|n| n == "no change to this file will fix this"),
        "the note that ends the reader's fix loop is missing"
    );
    assert!(
        first.notes.iter().any(|n| n.contains("the backend emits")),
        "the note that answers `what can I do instead` is missing"
    );
    assert!(
        !first.message.contains("M5") && !first.message.contains("M6") && !first.message.contains("M7"),
        "a milestone identifier reached the message: {}",
        first.message
    );
    (first.code.clone(), first.message.clone())
}

/// M5b deleted the `str` and `f64` rows, which is the gate's whole design: a row dies
/// per milestone. The test that used to assert they were refused now asserts they are
/// **not** — a row nobody can prove is gone is a row that comes back.
#[test]
fn text_and_floating_point_are_no_longer_refused() {
    let text = super::c("function main()\n    print(\"hi\")\n    x = 1.5\n    print(x)\n");
    assert!(text.contains("hero_print_str"), "{text}");
    assert!(text.contains("hero_print_f64"), "{text}");
    // …and the note now says every type in the language, because at M5d that is the
    // truth and enumerating them would be a list that only ever gets re-checked when a
    // row dies. What the note still enumerates is the *built-ins*, which is where the
    // remaining rows are.
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
    // type, and at M5c step 5 both sides have somewhere to go. Each keeps its own
    // bounds check in the runtime.
    let array = super::c("function main()\n    xs = [1, 2]\n    print(xs[0])\n    print(len(xs))\n");
    assert!(array.contains("hero_array_at"), "{array}");
    assert!(array.contains("hero_array_len"), "{array}");
    assert!(!array.contains("hero_str_len"), "the wrong half was chosen:\n{array}");
}

/// `str`→`cstr` exists for one boundary and nothing consumes it before M7, so landing
/// `str` did **not** make the cast emittable — the gate's row for it is keyed to the
/// FFI rather than to `str`.
///
/// There is no end-to-end case, and the reason is worth recording rather than hiding
/// behind a test that passes for the wrong reason: the checker refuses `puts("hi")`
/// with `expected `cstr`, found `str``, so **`Op::Cast` is unreachable from source
/// today**. The conversion arrives with M7's header attachment, and the row is here
/// waiting for it. Queued.
#[test]
fn an_extern_is_still_refused_after_str_landed() {
    let (code, _) = refusal("extern function labs(x: int) -> int\n\nfunction main()\n    print(labs(0 - 3))\n");
    assert_eq!(code, "extern");
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
        "function main()\n    xs: [int] @ [1, 2]\n    xs[0] @ 7\n    print(xs[0])\n",
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
        "    cells: [int]\n",
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
        "record P\n    x: int\n\nfunction main()\n    ps = [P(x: 1), P(x: 2)]\n    print(ps[1].x)\n",
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
        "        v: int\n",
        "    word\n",
        "        text: str\n",
        "    end\n",
        "\n",
        "function size(t: Token) -> int\n",
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
        "function pick(c: Color) -> int\n",
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
    let out = emitted("record P\n    x: int\n\nfunction main()\n    p = P(x: 1)\n    print(p.x)\n");
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
        "function half(n: int) -> int?\n    if n % 2 == 0\n        return ok(n / 2)\n    return fail(\"odd\", \"not even\")\n\nfunction main()\n    print(half(4).default(0))\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    // One generated struct per distinct `T?`, named by index because `int?` and
    // `[int]?` sanitise to the same identifier.
    assert!(out.c.contains("typedef struct h_scratch_opt0"), "{}", out.c);
    assert!(out.c.contains("int64_t tag;"), "{}", out.c);
    assert!(out.c.contains("HeroFailure err;"), "{}", out.c);
    // Every `T?` is counted whatever `T` is, because the error side is two `str`s.
    assert!(out.c.contains("h_scratch_opt0_release"), "{}", out.c);
    assert!(out.c.contains("hero_failure_release"), "{}", out.c);
}

/// `.must()` emits from M6 step 1, and the row that mattered is the ARGUMENT: the
/// failure travels with the abort, so the panic names the code and msg the author wrote
/// rather than only that a `.must()` failed.
#[test]
fn must_carries_its_failure_into_the_abort() {
    let out = super::emitted(
        "function half(n: int) -> int?\n    return ok(n / 2)\n\nfunction main()\n    print(half(4).must())\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("hero_panic_must("), "{}", out.c);
    // Not a bare call: something is passed, and it is the failure read in the abort
    // block itself — so nothing counted crosses a block edge.
    assert!(!out.c.contains("hero_panic_must();"), "the failure was dropped:\n{}", out.c);
    assert!(out.c.contains(".as.err"), "{}", out.c);
}


#[test]
fn a_generic_function_is_refused() {
    let (code, _) = refusal(
        "function twice<A>(x: A, f: (function(A) -> A)) -> A\n    return f(f(x))\n\nfunction inc(n: int) -> int\n    return n + 1\n\nfunction main()\n    print(twice(1, inc))\n",
    );
    // The generic function and the function value are both refused; the earliest
    // span wins, and it is the declaration.
    assert!(code == "generics" || code == "function_value", "{code}");
}

/// The ffi-pragmatist's veto, as a test. §4.19's mechanism is the `#include`, and
/// there is no header attachment yet — so an emitted prototype would be
/// self-consistent by construction and clang would verify nothing.
#[test]
fn an_extern_is_refused_because_nothing_would_check_its_signature() {
    let (code, message) = refusal("extern function labs(x: int) -> int\n\nfunction main()\n    print(labs(0 - 3))\n");
    assert_eq!(code, "extern");
    assert_eq!(message, "an `extern` function is not emitted yet");
}

/// `xs.len()` lowers to `call builtin len` while `for` lowers to `Op::Len`. Gating
/// the op alone would leave an undefined symbol at link time — a linker error is
/// the failure class the gate exists to prevent.
#[test]
fn a_builtin_with_no_runtime_entry_point_is_refused_by_name() {
    // `range` has no entry point, and it is refused *by name* rather than by any type
    // in the program: the row is keyed to the built-in list, so a built-in whose
    // operands all emit is still refused until the implementation exists.
    //
    // It used to be `sort` here, and the swap is the file's own rule working: M6 step
    // 3 gave `sort` an entry point, so the test that made its row fire had to move to
    // a name that still has none. `range` is Tier 2 (§1.11) — written in Heroes, so
    // what it waits for is the prelude rather than a runtime function.
    let (code, message) =
        refusal("function main()\n    for i in range(0, 3)\n        print(i)\n");
    assert_eq!(code, "builtin");
    assert!(message.contains("`range`"), "the message must name it: {message}");
}

/// The other half of the row, and the one added at M6 step 3: a built-in whose
/// name emits and whose **operand type** has none.
///
/// `sort` orders `int`, `f64` and `str`. A `[Point]` is refused *here* rather
/// than by the checker, because `{Point: int}` compiles and spec line 71 teaches
/// `for k in sort(keys(m))` — a compile error would contradict the spec's own
/// idiom, where a refusal only says this backend has not decided yet.
#[test]
fn a_builtin_whose_operand_type_has_no_order_is_refused_by_operand() {
    let (code, message) = refusal(
        "record Point\n    x: int\n    y: int\n\nfunction main()\n    ps = [Point(x: 1, y: 2)]\n    \
         print(len(sort(ps)))\n",
    );
    assert_eq!(code, "builtin");
    assert!(message.contains("`sort`"), "the message must name it: {message}");
    assert!(
        message.contains("`int`, `f64` or `str`"),
        "and it must name what does work, or the reader has to guess: {message}"
    );
}

#[test]
fn every_unsupported_capability_is_reported_not_only_the_first() {
    let out = emitted(
        "record Point\n    x: int\n    y: int\n\nfunction main()\n    ps = [Point(x: 1, y: 2)]\n    \
         print(len(sort(ps)))\n    for i in range(0, 3)\n        print(i)\n",
    );
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert!(codes.contains(&"builtin"), "{codes:?}");
    // Two capabilities the backend lacks — one keyed to a name, one to an operand
    // type — and both are named, so the list arrives at once.
    let messages: Vec<&str> = out.diagnostics.iter().map(|d| d.message.as_str()).collect();
    assert!(messages.iter().any(|m| m.contains("`sort`")), "{messages:?}");
    assert!(messages.iter().any(|m| m.contains("`range`")), "{messages:?}");
    // Sorted by span: three invocations to learn three facts is what the message
    // carrying the list exists to prevent.
    let spans: Vec<u32> = out.diagnostics.iter().map(|d| d.span.start).collect();
    let mut sorted = spans.clone();
    sorted.sort_unstable();
    assert_eq!(spans, sorted);
}

#[test]
fn one_capability_is_one_diagnostic_however_many_times_it_appears() {
    let out = emitted(
        "function main()\n    for i in range(0, 1)\n        print(i)\n    \
         for j in range(0, 2)\n        print(j)\n    for k in range(0, 3)\n        print(k)\n",
    );
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert_eq!(
        codes.len(),
        1,
        "a program that calls one unsupported built-in three times has one problem: {codes:?}"
    );
}

/// A `test` block is **skipped**, not refused: `ir/mod.rs` says ordinary builds
/// ignore it, and refusing it would make a file unbuildable at M5a and still
/// unbuildable after M5b and M5c.
#[test]
fn a_test_block_does_not_stop_the_build_and_does_not_reach_the_c() {
    let out = emitted("function double(n: int) -> int\n    return n * 2\n\ntest \"doubling\"\n    assert double(2) == 4\n\nfunction main()\n    print(double(3))\n");
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics[0].message);
    assert!(!out.c.contains("doubling"), "a test block reached the translation unit");
    assert!(out.c.contains("h_scratch_double"));
}

/// A file with no `main` is a perfectly good translation unit. Refusing it here
/// would make `--emit-c` useless for a library, and the binary's need for an entry
/// point belongs to the driver.
#[test]
fn a_file_with_no_main_emits_a_unit_with_no_shim() {
    let out = emitted("function double(n: int) -> int\n    return n * 2\n");
    assert!(out.diagnostics.is_empty());
    assert!(out.c.contains("h_scratch_double"));
    assert!(!out.c.contains("int main(void)"));
    assert!(crate::emit::entry_point(&crate::emit::tests::gate::program_of("function double(n: int) -> int\n    return n * 2\n")).is_none());
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
