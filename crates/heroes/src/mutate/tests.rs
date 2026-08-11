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
const CODES: &str = r#"function classify(c: int) -> int?
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
const REVERSED: &str = r#"function classify(c: int) -> int?
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
const ESCAPED: &str = r#"function classify(c: int) -> int?
    if c < '0' || c > '9'
        return fail("a\nb", "an escape, not a code")
    return ok(c - '0')

function main()
    print(classify('x').default(0))
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
