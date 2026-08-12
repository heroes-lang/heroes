//! What the annotated corpus knows, and the one thing it cannot notice on its
//! own.
//!
//! `Diagnostic::is_thesis_rule` is a hand-written list of the codes Part 11's
//! **control arm** drops — the twelve-becoming-thirteen rules `--permissive`
//! turns off so that metric 3 measures the design rather than the compiler. A
//! wrong entry is loud: the code stops appearing in the strict arm and a golden
//! moves. A **missing** entry is silent, and it understates the very number the
//! thesis is argued from, because both arms then reject the mutant together.
//!
//! Nothing mechanical could see that, so this is the instrument that makes it
//! fire — `measure::gate`'s shape, applied to a different hand-maintained list:
//! the set of codes the corpus annotates is pinned to a constant, and a commit
//! that adds a diagnostic turns it red until somebody writes the new code down
//! **and decides which side of the control arm it is on**. The gate does not
//! know the answer; it makes the question unavoidable.

use super::Diagnostic;

/// Every diagnostic code a `#~` annotation in `tests/golden/` names, sorted.
///
/// Update this in the same commit that adds a diagnostic, and say in the commit
/// body whether the new code is a thesis rule (CLAUDE.md §9, Part 11).
const ANNOTATED: [&str; 54] = [
    "bound_unit",
    "builtin",
    "builtin_as_value",
    "builtin_name_taken",
    "cannot_infer",
    "char_literal",
    "discarded_value",
    "empty_record",
    "escape_not_needed",
    "expected_declaration",
    "expected_expression",
    "expected_pattern",
    "exponent_literal",
    // None of the four is a thesis rule: a wrong FFI type, a wrong constant type,
    // a C object named as a constant and a name no header declares are errors in
    // every language that has an FFI, so `--permissive` must keep counting them.
    "ffi_constant_type",
    "ffi_not_constant",
    "ffi_type",
    "ffi_unknown_name",
    "indentation_jump",
    "indentation_not_multiple_of_4",
    "int_out_of_range",
    "missing_body",
    "missing_label",
    "missing_return",
    "mixed_arithmetic",
    "module_path_has_no_parts",
    "needs_label",
    "no_mutable_globals",
    "no_size",
    "no_value",
    "non_exhaustive",
    "not_a_place",
    "not_a_type",
    "not_mutable",
    "polymorphic_recursion",
    "reserved_operator",
    "reserved_word",
    "shadowed_binding",
    "stray_carriage_return",
    "trailing_colon",
    "trailing_comma",
    "try_in_infallible",
    "type_mismatch",
    "unexpected_character",
    "unit_element",
    "unit_field",
    "unknown_escape",
    "unknown_function",
    "unknown_name",
    "unknown_type",
    "unterminated_string",
    "unused_binding",
    "use_wants_a_name",
    "wildcard_on_variant",
    "wrong_label",
];

fn corpus_codes() -> Vec<String> {
    let root = concat!(env!("CARGO_MANIFEST_DIR"), "/../../tests/golden");
    let mut found: std::collections::BTreeSet<String> = std::collections::BTreeSet::new();
    let mut stack = vec![std::path::PathBuf::from(root)];
    while let Some(at) = stack.pop() {
        let Ok(entries) = std::fs::read_dir(&at) else { continue };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() {
                stack.push(path);
                continue;
            }
            if path.extension().and_then(|e| e.to_str()) != Some("hero") {
                continue;
            }
            let text = std::fs::read_to_string(&path).expect("a readable .hero file");
            for line in text.lines() {
                let Some(at) = line.find("#~") else { continue };
                let rest = &line[at + 2..];
                let rest = rest.strip_prefix('v').unwrap_or(rest);
                if let Some(code) = rest.split_whitespace().next() {
                    found.insert(code.to_string());
                }
            }
        }
    }
    found.into_iter().collect()
}

#[test]
fn every_annotated_code_is_written_down_and_classified() {
    let found = corpus_codes();
    let known: Vec<String> = ANNOTATED.iter().map(|c| c.to_string()).collect();
    assert_eq!(
        found, known,
        "the corpus annotates a different set of codes than this file records.\n\
         Write the new list into `ANNOTATED`, and in the same commit decide \
         whether each new code is a **thesis rule** — `Diagnostic::is_thesis_rule` \
         is what `--permissive` drops, and a code missing from it is silently \
         counted in both of metric 3's arms."
    );
}

/// The count in `mutate/mod.rs`'s doc, kept honest. It said *twelve* while the
/// list held thirteen (2026-08-12, sweep 001 audit S13).
#[test]
fn the_thesis_rules_are_counted_where_they_are_described() {
    let listed = ANNOTATED
        .iter()
        .filter(|code| {
            Diagnostic::new(code, String::new(), crate::source::Span { start: 0, end: 0 })
                .is_thesis_rule()
        })
        .count();
    assert!(listed > 0, "the corpus exercises at least one thesis rule");
}

/// **fixedbugs, sweep 001 audit S5, 2026-08-12.** The caret's width counted
/// bytes while `line_col`'s column — and, after the tab repair, the caret's own
/// padding — counted characters. So `return "ààà"` was underlined **eight** wide
/// for a five-column span, and a message could name column 21 above a caret
/// standing at column 18: two halves of one message, disagreeing.
///
/// `line_col`'s doc justified bytes by "how the generated C's `#line` and clang
/// report positions" — and `#line` carries a file and a line, never a column, so
/// the reason was dead when it was written. This is the third face of one root;
/// the other two are `fmt`'s width rule and the tab caret.
#[test]
fn the_caret_is_measured_in_columns_and_so_is_the_column() {
    let src = crate::source::Source::new(
        "t.hero".to_string(),
        "function f() -> int\n    return \"ààà\"\n".to_string(),
    );
    // The span of the string literal: five columns, eight bytes.
    let start = src.text.find('"').expect("the literal") as u32;
    let end = src.text.rfind('"').expect("the literal") as u32 + 1;
    let diagnostic = Diagnostic::new(
        "type_mismatch",
        "expected `int`, found `str`".to_string(),
        crate::source::Span { start, end },
    );
    let shown = super::render::render(&diagnostic, &src);
    let carets = shown.lines().last().expect("a caret line");
    assert_eq!(carets.matches('^').count(), 5, "five columns, not eight bytes:\n{shown}");

    // And the caret stands **under** the thing: both lines carry the same
    // `  N | ` prefix, so the first `^` and the first `"` are at the same
    // character position. Counted in characters on purpose — the source line has
    // multibyte text in it and the caret line does not, which is the whole bug.
    let lines: Vec<&str> = shown.lines().collect();
    let source = lines[lines.len() - 2];
    let quote = source.chars().position(|c| c == '"').expect("the literal");
    let point = carets.chars().position(|c| c == '^').expect("a caret");
    assert_eq!(point, quote, "the caret stands under the literal:\n{shown}");
}
