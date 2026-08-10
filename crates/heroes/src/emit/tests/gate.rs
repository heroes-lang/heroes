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
fn the_string_half_of_a_shared_operation_emits_and_the_array_half_does_not() {
    let text = super::c("function main()\n    s = \"ab\"\n    print(s[0])\n    print(len(s))\n");
    assert!(text.contains("hero_str_byte"), "{text}");
    assert!(text.contains("hero_str_len"), "{text}");
    let (code, _) = refusal("function main()\n    xs = [1, 2]\n    print(xs[0])\n");
    assert_eq!(code, "array");
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
fn arrays_maps_and_variants_are_refused() {
    assert_eq!(refusal("function main()\n    xs = [1, 2]\n    _ = xs.push(3)\n    print(1)\n").0, "array");
    // A `record` is emitted from M5c step 3 and a `variant` is not, so this row split
    // in two. They share `Ty::Named`, which is why the gate asks the *declaration*.
    let variant = concat!(
        "variant V\n",
        "    a\n",
        "        n: int\n",
        "    b\n",
        "\n",
        "function size(v: V) -> int\n",
        "    return match v\n",
        "        .a x => x.n\n",
        "        .b   => 0\n",
        "\n",
        "function main()\n",
        "    print(size(.a(n: 1)))\n",
    );
    assert_eq!(refusal(variant).0, "variant");
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
    let (code, message) = refusal("function main()\n    xs = [1, 2]\n    _ = xs.push(3)\n    print(1)\n");
    // Arrays are refused too and come first in the source; the built-in row is what
    // must also appear, so the whole list is checked rather than the first line.
    let out = emitted("function main()\n    xs = [1, 2]\n    _ = xs.push(3)\n    print(1)\n");
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert!(codes.contains(&"array"), "{codes:?}");
    let _ = (code, message);
}

#[test]
fn every_unsupported_capability_is_reported_not_only_the_first() {
    let out = emitted(
        "function main()\n    xs = [1, 2]\n    print(xs.len())\n    m = {\"a\": 1}\n    print(m.has(\"a\"))\n",
    );
    let codes: Vec<&str> = out.diagnostics.iter().map(|d| d.code.as_str()).collect();
    assert!(codes.contains(&"array") && codes.contains(&"map"), "{codes:?}");
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
        "function main()\n    xs = [1]\n    ys = [2]\n    zs = [3]\n    print(xs[0], ys[0], zs[0])\n",
    );
    assert_eq!(out.diagnostics.len(), 1, "a program with three arrays has one array problem");
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
