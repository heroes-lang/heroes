//! Shape: indentation, terminator insertion, and the panel-007 behaviour
//! (completed ender list, brackets-only continuation, unclosed opener).

use super::dump;

#[test]
fn terminator_only_after_enders() {
    // Line 1 ends with `+` (no terminator: the expression continues);
    // line 2 ends with `)` (terminator). design.md §4.15, Go's rule.
    assert_eq!(
        dump("b = (2 +\n3)\n"),
        "\
1:1 ident b
1:3 eq =
1:5 lparen (
1:6 int 2
1:8 plus +
2:1 int 3
2:2 rparen )
2:3 terminator
3:1 eof
"
    );
}

#[test]
fn nesting_indents_and_dedents() {
    assert_eq!(
        dump("f = function: ()\n    if x\n        g()\n    h()\n"),
        "\
1:1 ident f
1:3 eq =
1:5 kw_function function
1:13 colon :
1:15 lparen (
1:16 rparen )
1:17 terminator
2:1 indent
2:5 kw_if if
2:8 ident x
2:9 terminator
3:1 indent
3:9 ident g
3:10 lparen (
3:11 rparen )
3:12 terminator
4:1 dedent
4:5 ident h
4:6 lparen (
4:7 rparen )
4:8 terminator
5:1 dedent
5:1 eof
"
    );
}

#[test]
fn comments_are_retained_and_layout_neutral() {
    // The doc comment gets no terminator; the trailing comment does not
    // block the terminator earned by `1`.
    assert_eq!(
        dump("# doc\nx = 1 # five\n"),
        "\
1:1 comment # doc
2:1 ident x
2:3 eq =
2:5 int 1
2:7 comment # five
2:13 terminator
3:1 eof
"
    );
}

// --- panel 007 -------------------------------------------------------------

#[test]
fn enders_break_continue_hole_question() {
    // panel 007: break/continue/???/postfix ? now terminate their line.
    assert_eq!(
        dump("for x in xs\n    break\ny = f(s)?\nz = ???\n"),
        "\
1:1 kw_for for
1:5 ident x
1:7 kw_in in
1:10 ident xs
1:12 terminator
2:1 indent
2:5 kw_break break
2:10 terminator
3:1 dedent
3:1 ident y
3:3 eq =
3:5 ident f
3:6 lparen (
3:7 ident s
3:8 rparen )
3:9 question ?
3:10 terminator
4:1 ident z
4:3 eq =
4:5 hole ???
4:8 terminator
5:1 eof
"
    );
}

#[test]
fn brackets_suspend_indentation() {
    // panel 007: inside [ ] the element lines produce no Indent/Dedent;
    // terminators still separate the elements (§4.9 multi-line literals).
    assert_eq!(
        dump("fixed = [\n    3\n    1\n]\n"),
        "\
1:1 ident fixed
1:7 eq =
1:9 lbracket [
2:5 int 3
2:6 terminator
3:5 int 1
3:6 terminator
4:1 rbracket ]
4:2 terminator
5:1 eof
"
    );
}

#[test]
fn unclosed_opener_is_reported_at_eof() {
    // panel 007 (engineer's condition): without this diagnostic, one
    // missing `)` silently swallows the rest of the file's layout.
    assert_eq!(
        dump("x = (2 +\n3\n"),
        "\
1:1 ident x
1:3 eq =
1:5 lparen (
1:6 int 2
1:8 plus +
2:1 int 3
2:2 terminator
3:1 eof
DIAG test.hero:1:5: error[unclosed_bracket]: `(` opened here is never closed
"
    );
}

#[test]
fn stray_closer_does_not_corrupt_layout() {
    // Saturating pop: the `)` is the parser's problem; the next line's
    // indentation still works.
    assert_eq!(
        dump("x = 1)\n    y\n"),
        "\
1:1 ident x
1:3 eq =
1:5 int 1
1:6 rparen )
1:7 terminator
2:1 indent
2:5 ident y
2:6 terminator
3:1 dedent
3:1 eof
"
    );
}
