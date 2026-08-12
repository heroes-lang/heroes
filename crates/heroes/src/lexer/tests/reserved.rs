//! Foreign reserved words (spec/reserved-words.md): the thesis, executable.
//! Each fails with the registry's prescribed message; pure word-for-word
//! swaps carry a Certain, machine-applicable fix (design.md §4.17).

use super::{dump, lex};
use crate::diagnostics::Certainty;
use crate::source::Source;

#[test]
fn foreign_keywords_fail_with_the_fix_prewritten() {
    assert_eq!(
        dump("struct Point\nlet x = 5\n"),
        "\
1:1 error struct
1:8 ident Point
1:13 terminator
2:1 error let
2:5 ident x
2:7 eq =
2:9 int 5
2:10 terminator
3:1 eof
DIAG test.hero:1:1: error[reserved_word]: `struct` is not a word in this language — use `record`: `record Point`
DIAG test.hero:2:1: error[reserved_word]: `let` is not a word in this language — bind with `=`: `x = 5`
"
    );
}

#[test]
fn null_family_and_exceptions_share_their_messages() {
    assert_eq!(
        dump("x = None\ntry\n"),
        "\
1:1 ident x
1:3 eq =
1:5 error None
2:1 error try
3:1 eof
DIAG test.hero:1:5: error[reserved_word]: there is no null in this language — absence is a fallible type: `int?`, and a C pointer's zero is `nullptr`
DIAG test.hero:2:1: error[reserved_word]: there are no exceptions in this language — errors are values: `fail(code, msg)`, propagate with `?`
"
    );
}

#[test]
fn certain_fix_travels_with_the_diagnostic() {
    // `switch` → `match` is a pure word swap, so the Fix is Certain and
    // machine-applicable.
    let src = Source::new("test.hero".to_string(), "switch x\n".to_string());
    let out = lex(&src);
    let d = &out.diagnostics[0];
    assert_eq!(d.code, "reserved_word");
    assert_eq!(d.fixes.len(), 1);
    assert_eq!(d.fixes[0].replacement, "match");
    assert!(matches!(d.fixes[0].certainty, Certainty::Certain));
    // `let` has guidance but no mechanical repair: no fix attached.
    let src2 = Source::new("test.hero".to_string(), "let x = 5\n".to_string());
    let out2 = lex(&src2);
    assert_eq!(out2.diagnostics[0].fixes.len(), 0);
}

/// The registry's one departure: `while` used to be the trap row (`while` →
/// `for`), and panel 018 made it the condition loop's own keyword. It lexes
/// clean — the trap is now the other way around, and it lives in the parser
/// (`for` over a condition gets a `while` fix).
#[test]
fn while_is_a_keyword_now_not_a_foreign_word() {
    assert_eq!(
        dump("while x > 0\n"),
        "\
1:1 kw_while while
1:7 ident x
1:9 gt >
1:11 int 0
1:12 terminator
2:1 eof
"
    );
}
