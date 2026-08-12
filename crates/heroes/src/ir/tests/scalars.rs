//! Flattening, slots, literals, and the edges out of a function.

use super::{lowered, text};

/// `x = 2 + 3 * 4` is a tree in the syntax and four instructions here, in the
/// order the precedence table decided at M-syntax-tree. Nothing is nested: that is the
/// invariant panel 019 restated, and it is what makes the emitter a printer.
#[test]
fn an_expression_becomes_a_line_per_operation() {
    let dumped = text("function f() -> int\n    x = 2 + 3 * 4\n    return x\n");
    assert!(dumped.contains("$t1: int = const 2"), "{dumped}");
    assert!(dumped.contains("$t2: int = const 3"), "{dumped}");
    assert!(dumped.contains("$t3: int = const 4"), "{dumped}");
    // `3 * 4` first, then the addition — the multiplication binds tighter.
    assert!(dumped.contains("$t4: int = mul! $t2, $t3"), "{dumped}");
    assert!(dumped.contains("$t5: int = add! $t1, $t4"), "{dumped}");
}

/// The `!` is not decoration: it marks an instruction that can stop the program.
/// `int` arithmetic aborts on overflow (§4.3) and `f64` arithmetic cannot, so the
/// mark is a type distinction as well as a warning — and a reader who does not see
/// it will write a pass that assumes one exit per block.
#[test]
fn only_the_operations_that_can_abort_are_marked() {
    let dumped = text(
        "function f(a: f64, b: f64) -> f64\n    return a * b\n\nfunction g(a: int, b: int) -> bool\n    return a < b\n",
    );
    assert!(dumped.contains("= mul $t"), "f64 multiplication cannot abort: {dumped}");
    assert!(!dumped.contains("mul!"), "{dumped}");
    assert!(dumped.contains("= lt $t"), "a comparison cannot abort: {dumped}");
}

/// §4.3: a character literal *is* an `int`, decoded with the same five escapes the
/// lexer applies (panel 008).
#[test]
fn a_character_literal_is_an_int() {
    let dumped = text("function f() -> int\n    return 'a'\n");
    assert!(dumped.contains("const 97"), "{dumped}");
}

/// Strings are interned per program, so the same literal twice is one entry. The
/// determinism test depends on it: an id that moved between runs would make the
/// dump unstable for a reason no reader could see.
#[test]
fn an_equal_string_literal_is_interned_once() {
    let dumped = text(
        "function f() -> str\n    a = \"hi\"\n    b = \"hi\"\n    c = \"ho\"\n    print(b, c)\n    return a\n",
    );
    let strings: Vec<&str> = dumped.lines().take_while(|l| !l.is_empty()).collect();
    assert_eq!(strings.len(), 3, "one header and two literals: {strings:?}");
    assert!(strings[1].contains("\"hi\""), "{strings:?}");
    assert!(strings[2].contains("\"ho\""), "{strings:?}");
}

/// A call to a function that returns nothing has **no destination**. This is the
/// ffi-pragmatist's hard error, kept out of the IR rather than out of the emitter:
/// `dst = call print(x)` does not compile in C, and raylib is almost entirely
/// void-returning.
#[test]
fn a_call_that_returns_nothing_has_no_destination() {
    let dumped = text("function f()\n    print(1)\n");
    assert!(dumped.contains("        call builtin print($t1) variadic"), "{dumped}");
    assert!(!dumped.contains("= call builtin print"), "{dumped}");
}

/// An `extern` contributes a signature and no blocks (§4.19). Zero blocks is how
/// the dump and the emitter tell it apart from a function whose body is empty.
#[test]
fn an_extern_has_a_signature_and_no_blocks() {
    let (program, _, dumped) = lowered(
        "extern function sqrt(x: f64) -> f64\n\nfunction f() -> f64\n    return sqrt(2.0)\n",
    );
    let external = &program.functions[0];
    assert!(external.blocks.is_empty(), "an extern must have no blocks");
    assert!(dumped.contains("extern function sqrt(x: f64) -> f64"), "{dumped}");
    assert!(dumped.contains("no body"), "{dumped}");
    // The linkage travels with the call, which is the whole point: `extern` names
    // reach C unmangled and Heroes names do not.
    assert!(dumped.contains("call extern sqrt($t1)"), "{dumped}");
}

/// A `constant` is a zero-argument function and reading its name is a call (§4.2:
/// there are no mutable globals, so there is nothing else to read).
#[test]
fn a_constant_is_a_function_and_reading_it_is_a_call() {
    let dumped = text("constant LIMIT: int\n    64\n\nfunction f() -> int\n    return LIMIT\n");
    assert!(dumped.contains("constant LIMIT: int"), "{dumped}");
    assert!(dumped.contains("call heroes LIMIT()"), "{dumped}");
}

/// The one diagnostic this pass owns. Nothing before M-ir-lowering needed an `int` literal's
/// *value* — the lexer accepted the shape and the checker gave it a type — so an
/// out-of-range literal was invisible until the pass that has to represent it.
#[test]
fn an_out_of_range_int_literal_is_reported_here() {
    let src = crate::source::Source::new(
        "test.hero".to_string(),
        "function f() -> int\n    return 99999999999999999999\n".to_string(),
    );
    let parsed = crate::syntax::parse(&src);
    let resolved = crate::resolve::resolve(&parsed.ast, &src);
    let checked = crate::types::check(&parsed.ast, &resolved, &src);
    assert!(checked.diagnostics.is_empty(), "the checker has nothing to say about it");
    let out = crate::ir::lower(&parsed.ast, &resolved, &checked, &src);
    assert_eq!(out.diagnostics.len(), 1, "lowering owns this one");
    assert!(out.diagnostics[0].message.contains("does not fit in an `int`"));
    // The message names the range rather than a wider type: `int` is the only
    // integer type (§4.3), so the repair is a different number.
    assert!(out.diagnostics[0].notes[0].contains("9223372036854775807"));
}

/// Statements after a `return` are not lowered. They cannot execute, and a dump
/// that showed them would be showing code that does not exist.
#[test]
fn nothing_after_a_return_is_lowered() {
    let dumped = text("function f() -> int\n    return 1\n    x = 2\n    print(x)\n");
    assert!(!dumped.contains("const 2"), "{dumped}");
    assert!(!dumped.contains("print"), "{dumped}");
}
