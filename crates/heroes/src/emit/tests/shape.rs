//! The shape of the emitted C — every rule CLAUDE.md §7 states, as a test.

use super::{c, named};

const FIRST: &str = "function main()\n    print((2 + 3) * 4)\n";

#[test]
fn the_first_program_has_the_shape_spike_01_froze() {
    let text = c(FIRST);
    // The prelude, and the stamp that makes a decoy runtime a compile error.
    assert!(text.contains("#include \"heroes_runtime.h\""));
    assert!(text.contains("_Static_assert(HERO_RUNTIME_ABI == 13"));
    // One prototype before any definition, and the mangled name.
    let prototype = text.find("void h_scratch_main(void);").expect("a prototype");
    let definition = text.find("void h_scratch_main(void) {").expect("a definition");
    assert!(prototype < definition, "the prototype must precede the definition");
    // The entry `goto`, which exists because the entry label would otherwise be
    // unused (spike 02's finding).
    assert!(text.contains("    goto bb0;"));
    assert!(text.contains("bb0:"));
    // Arithmetic through the builtins, in the source's own order.
    let add = text.find("__builtin_add_overflow").expect("the add");
    let mul = text.find("__builtin_mul_overflow").expect("the mul");
    assert!(add < mul, "(2 + 3) * 4 adds before it multiplies");
    assert!(text.contains("hero_panic_overflow();"));
    // print composed from segment printers, with the newline in the runtime.
    assert!(text.contains("hero_print_int(t"));
    assert!(text.contains("    hero_print_end();"));
    // The shim, not a mangler exception.
    //
    // It takes `argc`/`argv` and hands them to the runtime **whether or not the
    // program calls `args()`** (M-ffi-ladder): two shims chosen by whether a name
    // is reachable would be a condition that can be wrong, and the parameters cost
    // nothing. The shim also asserts the leak balance, because AddressSanitizer's
    // leak detector does not exist on this platform (panel 021 R9).
    assert!(text.contains(
        "int main(int argc, char **argv) {\n    hero_args_set(argc, argv);\n    h_scratch_main();\n    hero_runtime_check_leaks();\n    return 0;\n}"
    ), "{text}");
}

#[test]
fn every_temporary_is_declared_in_the_prologue_before_the_first_label() {
    let text = c(FIRST);
    let label = text.find("bb0:").expect("a block");
    let prologue = &text[..label];
    assert!(prologue.contains("int64_t t"), "temporaries are hoisted:\n{prologue}");
    // …and nothing is declared after a label, because `goto` may not jump over an
    // initialisation.
    let body = &text[label..];
    for line in body.lines() {
        let line = line.trim();
        assert!(
            !(line.starts_with("int64_t ") && line.ends_with(';') && !line.contains('=')),
            "a declaration after a label: {line}"
        );
    }
}

/// The unit rule, which is the first thing clang decides if the emitter does not:
/// `$t0` is `()` in every function and `void t0;` is a hard error.
#[test]
fn a_unit_typed_temporary_is_never_declared() {
    let text = c(FIRST);
    assert!(!text.contains("void t0;"), "a unit temporary was declared:\n{text}");
    assert!(!text.contains(" t0;"), "the unit value must have no name at all:\n{text}");
}

#[test]
fn a_line_directive_names_the_source_and_the_restores_name_the_generated_file() {
    let text = named("examples/gallery/00-first.hero", FIRST).c;
    assert!(text.contains("#line 2 \"examples/gallery/00-first.hero\""));
    // The stem is sanitised to alphanumerics, which is what makes `h_<module>_`
    // injective — so `00-first.hero` restores to `00first.c`.
    assert!(text.contains("\"00first.c\""), "restores name <stem>.c:\n{text}");
    // No `#line 0`: it is invalid C11.
    assert!(!text.contains("#line 0 "));
}

/// The property that makes `#line` worth having: a restore's number must be the
/// line it actually lands on in the generated file, because clang auto-increments
/// from it. Off-by-one here is what `tools/spike/01-first.c` shipped with.
#[test]
fn every_restore_directive_names_its_own_next_line() {
    let text = c(FIRST);
    for (index, line) in text.lines().enumerate() {
        let Some(rest) = line.strip_prefix("#line ") else { continue };
        let (number, file) = rest.split_once(' ').expect("a number and a file");
        if !file.contains(".c\"") {
            continue;
        }
        let claimed: usize = number.parse().expect("a line number");
        // `index` is 0-based and names the directive's own line, so the next
        // physical line is `index + 2` in 1-based counting.
        assert_eq!(claimed, index + 2, "a restore lies about where it lands:\n{text}");
    }
}

#[test]
fn a_mutable_parameter_is_a_pointer_and_copies_in_and_out() {
    let text = c("function bump(@n: i64)\n    n @ n + 1\n\nfunction main()\n    v: i64 @ 1\n    bump(@v)\n    print(v)\n");
    assert!(text.contains("void h_scratch_bump(int64_t *ph0_n);"), "{text}");
    assert!(text.contains("    h0_n = *ph0_n;"), "copy-in:\n{text}");
    assert!(text.contains("    *ph0_n = h0_n;"), "copy-out:\n{text}");
    assert!(text.contains("h_scratch_bump(&h0_v);"), "the call passes the address:\n{text}");
}

/// A correct program with an unreachable join block. Emitting its label would cost
/// `-Wunused-label` on every build, and switching that warning off would take the
/// emitter's own bugs with it.
#[test]
fn a_block_nothing_jumps_to_is_omitted_entirely() {
    let text = c("function sign(x: i64) -> i64\n    if x > 0\n        return 1\n    else\n        return 0\n\nfunction main()\n    print(sign(3))\n");
    let labels = text.matches("bb").filter(|_| true).count();
    assert!(labels > 0);
    // The join after two returning arms has no predecessors, so it is not emitted:
    // every label that appears must be the target of a goto.
    for line in text.lines() {
        let Some(label) = line.strip_suffix(':') else { continue };
        if !label.starts_with("bb") {
            continue;
        }
        assert!(
            text.contains(&format!("goto {label};")),
            "label {label} is emitted but nothing jumps to it:\n{text}"
        );
    }
}

#[test]
fn division_carries_both_guards_and_the_remainder_carries_them_too() {
    let text = c("function main()\n    print(7 / 2)\n    print(7 % 2)\n");
    assert_eq!(text.matches("hero_panic(\"division by zero\")").count(), 2);
    // `INT64_MIN % -1` is undefined as surely as the division is, and on arm64 it
    // does not trap — it returns a wrong answer at exit 0.
    assert_eq!(text.matches("== INT64_MIN &&").count(), 2, "{text}");
}

#[test]
fn the_smallest_integer_is_emitted_as_a_macro_c_can_parse() {
    let text = c("function main()\n    x: i64 @ 0 - 9223372036854775807\n    x @ x - 1\n    print(x)\n");
    assert!(text.contains("INT64_C(9223372036854775807)"), "{text}");
}

#[test]
fn a_bool_prints_through_its_own_segment_printer() {
    let text = c("function main()\n    print(1 < 2)\n");
    assert!(text.contains("hero_print_bool(t"), "{text}");
}

#[test]
fn a_branch_writes_both_edges_so_adjacency_carries_no_meaning() {
    let text = c("function main()\n    n: i64 @ 0\n    while n < 3\n        n @ n + 1\n    print(n)\n");
    assert!(text.contains("else goto bb"), "both edges are written:\n{text}");
}

/// Determinism, at the cheapest level there is: the same input twice.
#[test]
fn the_same_program_emits_the_same_bytes_twice() {
    assert_eq!(c(FIRST), c(FIRST));
}

#[test]
fn the_module_prefix_comes_from_the_stem_and_not_from_the_path() {
    let a = named("a/b/first.hero", FIRST).c;
    let b = named("./first.hero", FIRST).c;
    assert!(a.contains("h_first_main"));
    // Only the `#line` path and the header comment may differ — the symbols may
    // not, because the fixpoint compares generated C.
    let strip = |text: String| -> String {
        text.lines()
            .filter(|l| !l.starts_with("#line ") && !l.starts_with("/* Generated"))
            .collect::<Vec<_>>()
            .join("\n")
    };
    assert_eq!(strip(a), strip(b));
}

/// A literal only a `test` block mentions is **not** emitted.
///
/// §4.18 says ordinary builds ignore a `test`, so its `assert` messages are in
/// `program.strings` and read by nothing. Emitted anyway they are
/// `-Wunused-const-variable` — eight of them on one gallery program, on every build,
/// which is how a real diagnostic gets lost. The index is still the literal's own, so
/// adding a `test` to a file cannot renumber the C.
#[test]
fn a_literal_only_a_test_mentions_is_not_emitted() {
    let out = super::emitted(
        "function main()\n    print(\"live\")\n\ntest \"t\"\n    assert 1 + 1 == 2\n",
    );
    assert!(out.diagnostics.is_empty(), "{:?}", out.diagnostics);
    assert!(out.c.contains("\"live\""), "the reachable literal is missing:\n{}", out.c);
    assert!(
        !out.c.contains("1 + 1 == 2"),
        "an assert message from a skipped `test` reached the C:\n{}",
        out.c
    );
}

/// **The typehash is computable in Heroes**, which is what makes the port able to
/// reproduce a name the fixpoint compares byte for byte.
///
/// The premise this pins used to be false. `mangle::instance` was FNV-1a and its
/// own comment said the port must reproduce it exactly, while every ingredient was
/// inexpressible: an unsigned offset basis (`int_out_of_range`), a wrapping
/// multiply (`panic: integer overflow`), and `^` (`reserved_operator`). Found by
/// panel 039's verification, on a closure-list row that does not exist — which is
/// why nothing mechanical could have caught it.
///
/// The other half of the pair is `tests/golden/run/premise-mangler-hash-in-heroes.hero`,
/// which computes these same four values **in Heroes** and prints them in decimal.
/// Either side moving alone is a red test.
#[test]
fn the_typehash_is_computable_in_heroes() {
    // Rendering → the decimal the `.hero` golden prints. Hex is what the mangler
    // writes into a name; decimal is what `print` can produce, so the test carries
    // both and the conversion is the assertion.
    for (rendered, decimal) in [
        ("map<i64, str>", 452_710_609_u64),
        ("fold<i64, i64>", 947_353_345),
        ("pair<i64, str_x>", 1_680_613_593),
        ("pair<i64_str, x>", 323_221_308),
    ] {
        assert_eq!(
            super::super::mangle::instance(rendered),
            format!("{decimal:x}"),
            "the mangler and the Heroes golden must agree on `{rendered}`"
        );
    }

    // Every intermediate must fit an `i64`, with the margin the golden's comment
    // claims: (M - 1) * B + 255 against i64::MAX.
    let worst = (2_147_483_646_i64) * 131 + 255;
    assert!(worst < i64::MAX / 1_000_000, "the margin is three orders of magnitude, not one");

    // A readable suffix would collide on these two; a hash must not.
    assert_ne!(
        super::super::mangle::instance("pair<i64, str_x>"),
        super::super::mangle::instance("pair<i64_str, x>"),
        "panel 029 R5's non-injectivity case"
    );
}
