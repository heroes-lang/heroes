//! What the backend refuses, and the two things it deliberately does not.
//!
//! Every row of `gate.rs`'s table has a test that makes it fire — LLVM's
//! `test/Verifier` discipline, applied to the other direction (CLAUDE.md §9). When
//! M5b lands, the `str` and `f64` tests here are the ones that must be deleted, and
//! that is the point: a row nobody can make fire is a row nobody can retire.

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
    // …and the note now says so, derived from the same table the gate reads.
    assert!(crate::emit::subset().contains("`str`"));
    assert!(crate::emit::subset().contains("`f64`"));
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
fn maps_are_refused_and_so_is_writing_one_element_of_an_array() {
    let map = concat!(
        "function main()\n",
        "    m = {\"a\": 1}\n",
        "    print(m.has(\"a\"))\n",
    );
    assert_eq!(refusal(map).0, "map");
    // The row that arrived WITH arrays rather than dying with them: reading an element
    // emits, writing one is copy-on-write — one unshare per array step of the place,
    // each writing back at its level (panel 022). Without this row, `xs[0] @ 7` reached
    // clang as `error: incompatible integer to pointer conversion assigning to
    // 'HeroArrayHeader *'`, reported as an internal error with a path to generated C.
    let write = "function main()\n    xs: [int] @ [1, 2]\n    xs[0] @ 7\n    print(xs[0])\n";
    assert_eq!(refusal(write).0, "array_write");
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
fn a_fallible_value_is_refused() {
    let (code, _) = refusal(
        "function half(n: int) -> int?\n    if n % 2 == 0\n        return ok(n / 2)\n    return fail(\"odd\", \"not even\")\n\nfunction main()\n    print(half(4).default(0))\n",
    );
    assert_eq!(code, "fallible");
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
    // `sort` has no entry point yet, and it is refused *by name* rather than by any
    // type in the program: the row is keyed to the built-in list, so a built-in whose
    // operands all emit is still refused until the runtime has it.
    let (code, message) = refusal("function main()\n    xs = [2, 1]\n    ys = sort(xs)\n    print(ys[0])\n");
    assert_eq!(code, "builtin");
    assert!(message.contains("`sort`"), "the message must name it: {message}");
}

#[test]
fn every_unsupported_capability_is_reported_not_only_the_first() {
    let out = emitted(
        "function main()\n    m = {\"a\": 1}\n    print(m.has(\"a\"))\n    xs: [int] @ [1]\n    xs[0] @ 2\n    print(xs[0])\n",
    );
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert!(codes.contains(&"map") && codes.contains(&"array_write"), "{codes:?}");
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
        "function main()\n    m = {\"a\": 1}\n    n = {\"b\": 2}\n    o = {\"c\": 3}\n    print(has(m, \"a\"), has(n, \"b\"), has(o, \"c\"))\n",
    );
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert_eq!(
        codes.iter().filter(|c| **c == "map").count(),
        1,
        "a program with three maps has one map problem: {codes:?}"
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
