//! `heroes mutate`'s own tests: that an operator fires, and — for the one that
//! measures a hole rather than a defence — that the hole is still open.
//!
//! CLAUDE.md §9 asks for a test that makes every check fire, and a mutation
//! operator needs it more than most: one that silently matches nothing removes
//! its own sites from the denominator and **raises** the reported rate. That is
//! the single failure mode a mutation harness cannot see from its own number.

use super::{mutants, run};

/// Both ends of §4.6's error-code contract in one file: the `fail` that builds
/// the code, and the comparison that reads it back.
const CODES: &str = r#"function classify(c: i64) -> i64?
    if c < '0' || c > '9'
        return fail("unknown_char", "that byte is not a digit")
    return ok(c - '0')

function main()
    match classify('x')
        .ok v  => print(v)
        .err e => print(e.code == "unknown_char")
"#;

/// The same comparison written the other way round. A reader who writes the
/// literal first is writing the same program, and the operator has to see it.
const REVERSED: &str = r#"function classify(c: i64) -> i64?
    if c < '0' || c > '9'
        return fail("unknown_char", "that byte is not a digit")
    return ok(c - '0')

function main()
    match classify('x')
        .ok v  => print(v)
        .err e => print("unknown_char" == e.code)
"#;

/// A code carrying an escape. Dropping the middle character of `\n` measures the
/// lexer, which `mod.rs`'s rule 2 excludes — so the site is skipped outright
/// rather than counted and then excluded.
const ESCAPED: &str = r#"function classify(c: i64) -> i64?
    if c < '0' || c > '9'
        return fail("a\nb", "an escape, not a code")
    return ok(c - '0')

function main()
    print(classify('x').default(0))
"#;

/// Two constants a C header owns and one the program owns. The operator cannot
/// tell them apart — nothing in the file says which number has an authority
/// behind it — and that inability is exactly what the row measures.
const NUMBERS: &str = r#"constant SQLITE_ROW: i64
    100

constant MAX_DEPTH: i64
    64

function deep(n: i64) -> bool
    return n > MAX_DEPTH || n == SQLITE_ROW

function main()
    print(deep(9))
"#;

/// A constant whose body is not one literal. There is no single digit to move, so
/// the site is skipped rather than mutated into something arbitrary.
const COMPUTED: &str = r#"constant LIMIT: i64
    base = 100
    base * 2

function main()
    print(LIMIT)
"#;

#[test]
fn the_code_a_fail_builds_is_mutated() {
    let out = mutants("typo-code", "codes.hero", CODES);
    assert!(
        out.iter().any(|m| m.contains(r#"fail("unknow_char""#)),
        "no mutant slipped the constructed code: {out:#?}"
    );
}

#[test]
fn the_code_a_comparison_reads_is_mutated_on_either_side() {
    for (name, text) in [("codes.hero", CODES), ("reversed.hero", REVERSED)] {
        let out = mutants("typo-code", name, text);
        assert!(
            out.iter().any(|m| m.contains(r#""unknow_char" == e.code"#)
                || m.contains(r#"e.code == "unknow_char""#)),
            "no mutant slipped the compared code in {name}: {out:#?}"
        );
    }
}

#[test]
fn an_escape_is_not_a_code_site() {
    assert!(mutants("typo-code", "escaped.hero", ESCAPED).is_empty());
}

#[test]
fn a_constants_last_digit_is_moved() {
    let out = mutants("typo-digit", "numbers.hero", NUMBERS);
    assert!(
        out.iter().any(|m| m.contains("    101")),
        "no mutant moved SQLITE_ROW's last digit: {out:#?}"
    );
    assert!(
        out.iter().any(|m| m.contains("    65")),
        "no mutant moved MAX_DEPTH's last digit: {out:#?}"
    );
}

#[test]
fn a_computed_constant_is_not_a_digit_site() {
    assert!(mutants("typo-digit", "computed.hero", COMPUTED).is_empty());
}

/// **The second finding, pinned, and the reason this operator exists.** A number
/// copied out of a C header is checked by nothing: the mutant is a legal program
/// that calls the wrong option, compares against the wrong code, and says so
/// nowhere. `SQLITE_ROW` here is `100` because `sqlite3.h` says so, and `101`
/// type-checks exactly as well.
///
/// When §4.19's header-valued `constant` lands, the honest outcome is not that
/// this rate improves: it is that the **site disappears**, because the digit is
/// no longer in the file. This test then measures the program's own constant
/// only — which still survives, and correctly, since nothing outside the program
/// knows what `MAX_DEPTH` should be.
#[test]
fn today_nothing_catches_a_wrong_constant() {
    let sources = vec![("numbers.hero".to_string(), NUMBERS.to_string())];
    let scores = run(&sources);
    let score = scores
        .iter()
        .find(|s| s.operator == "typo-digit")
        .expect("the operator table carries typo-digit");
    assert_eq!(score.mutants, 2, "both constants are sites");
    assert_eq!(score.excluded, 0, "a moved digit still parses");
    assert_eq!(
        score.killed_strict, 0,
        "a wrong constant is now caught — update this test in the commit that did it"
    );
}

/// **The finding, pinned.** §4.6's code is a bare `str`, so a slip at one end of
/// the contract is a legal program that silently never matches. Every other
/// operator's test asserts the compiler *catches* something; this one asserts it
/// does not, which is why the assertion is written as the measurement it is.
///
/// If this test ever goes red, a diagnostic has closed the hole — and then it is
/// this test that must be rewritten, deliberately, in the commit that closed it.
#[test]
fn today_nothing_catches_a_slipped_code() {
    let sources = vec![("codes.hero".to_string(), CODES.to_string())];
    let scores = run(&sources);
    let score = scores
        .iter()
        .find(|s| s.operator == "typo-code")
        .expect("the operator table carries typo-code");
    assert!(score.mutants > 0, "the operator found no site to mutate");
    assert_eq!(score.excluded, 0, "a code slip still parses");
    assert_eq!(
        score.killed_strict, 0,
        "a slipped error code is now caught — update this test in the commit that did it"
    );
}

/// A survivor's site is derived from the two texts, not carried from the edit —
/// so it must land on the line the operator actually changed.
#[test]
fn a_survivor_names_the_line_the_operator_changed() {
    let original = "function main()\n    x = 1\n    print(x)\n";
    let mutant = "function main()\n    x = 1\n    print(y)\n";
    let found = super::Survivor::locate("typo-ident", "t.hero", original, mutant)
        .expect("the two texts differ");
    assert_eq!(found.line, 3);
    assert_eq!(found.before.as_deref(), Some("    print(x)"));
    assert_eq!(found.after.as_deref(), Some("    print(y)"));

    // A deletion still differs first at the line it deleted (`drop-case`).
    let shorter = "function main()\n    print(1)\n";
    let deleted = super::Survivor::locate("drop-case", "t.hero", original, shorter)
        .expect("a deletion is a difference");
    assert_eq!(deleted.line, 2);
    assert_eq!(deleted.after.as_deref(), Some("    print(1)"));

    // Identical texts have no site, and inventing one would be worse than none.
    assert!(super::Survivor::locate("x", "t.hero", original, original).is_none());
}

/// The base program must compile before anything is mutated, and the check is the
/// same `fate` the mutants get — two pipelines could not drift apart if they
/// wanted to.
#[test]
fn a_corpus_program_that_does_not_check_is_refused() {
    assert!(super::base_checks_clean("t.hero", "function main()\n    print(1)\n"));
    // `range` is a built-in, which is exactly what refused the library corpus.
    assert!(!super::base_checks_clean(
        "t.hero",
        "function range(from: i64, to: i64) -> [i64]\n    return []\n\nfunction main()\n    print(1)\n"
    ));
}

/// **The arm panel 040's own falsifier needed, and the instrument did not have.**
///
/// Prediction 2 of the bitwise set asked that no survivor ever be a boolean
/// operator misread as its bitwise twin. `docs/measurements/007` scored it *held*
/// over 892 survivors — vacuously, because none of the twelve operators made the
/// substitution. This test is what stops that recurring: it fires the operator and
/// asserts the mutation is the one named, so a prediction about `&` for `&&` is
/// scored against an instrument that can produce the observation (panel 046 R1 as
/// amended, author decision 2026-08-14).
#[test]
fn boolean_twin_writes_the_bitwise_spelling_of_a_boolean_operator() {
    let text = "function main()\n    a = 1 > 0\n    b = 2 > 1\n    if a && b\n        print(1)\n    if a || b\n        print(2)\n";
    let mutants = super::operators::apply("boolean-twin", "t.hero", text);
    assert_eq!(mutants.len(), 2, "one per boolean operator: {mutants:?}");
    assert!(mutants.iter().any(|m| m.contains("if a & b") && m.contains("if a || b")));
    assert!(mutants.iter().any(|m| m.contains("if a && b") && m.contains("if a | b")));
    // And nothing else moved: a mutant differs from the source by one character.
    for mutant in &mutants {
        assert_eq!(mutant.len(), text.len() - 1, "one character, no more:\n{mutant}");
    }
}

/// A program with no boolean operator produces no mutant — the narrowing is the
/// operator in hand, not a search of the line for an `&`.
#[test]
fn boolean_twin_leaves_a_bitwise_expression_alone() {
    let text = "function main()\n    x = 6 & 3\n    y = 6 | 3\n    print(x + y)\n";
    assert!(super::operators::apply("boolean-twin", "t.hero", text).is_empty());
}
