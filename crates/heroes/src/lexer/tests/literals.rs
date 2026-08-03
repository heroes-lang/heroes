//! Words that denote values: numbers, operators, strings, char literals —
//! including what does NOT exist yet (escapes) and what fails loudly.

use super::dump;

#[test]
fn operators_multichar_and_floats() {
    assert_eq!(
        dump("a == 1.5 != <= >= && || ! -> => ??? ? . |\n"),
        "\
1:1 ident a
1:3 eq_eq ==
1:6 float 1.5
1:10 bang_eq !=
1:13 le <=
1:16 ge >=
1:19 and_and &&
1:22 or_or ||
1:25 bang !
1:27 arrow ->
1:30 fat_arrow =>
1:33 hole ???
1:37 question ?
1:39 dot .
1:41 pipe |
2:1 eof
"
    );
}

#[test]
fn strings_and_chars() {
    assert_eq!(
        dump("greeting = \"hello, world\"\nc = 'a'\nsp = ' '\n"),
        "\
1:1 ident greeting
1:10 eq =
1:12 str \"hello, world\"
1:26 terminator
2:1 ident c
2:3 eq =
2:5 char 'a'
2:8 terminator
3:1 ident sp
3:4 eq =
3:6 char ' '
3:9 terminator
4:1 eof
"
    );
}

#[test]
fn backslash_is_an_ordinary_byte() {
    // No escape sequences exist (design.md is silent; gap on record in
    // OPEN-QUESTIONS): "a\nb" is FOUR characters, backslash included.
    assert_eq!(
        dump("s = \"a\\nb\"\n"),
        "\
1:1 ident s
1:3 eq =
1:5 str \"a\\nb\"
1:11 terminator
2:1 eof
"
    );
}

#[test]
fn unterminated_string_is_loud() {
    assert_eq!(
        dump("s = \"oops\nx = 1\n"),
        "\
1:1 ident s
1:3 eq =
1:5 error \"oops
2:1 ident x
2:3 eq =
2:5 int 1
2:6 terminator
3:1 eof
DIAG test.hero:1:5: error[unterminated_string]: this string never closes — strings are single-line, `\"` to `\"`
"
    );
}

#[test]
fn char_literal_is_one_ascii_character() {
    assert_eq!(
        dump("a = ''\nb = 'ab'\nc = 'é'\n"),
        "\
1:1 ident a
1:3 eq =
1:5 error ''
2:1 ident b
2:3 eq =
2:5 error 'ab'
3:1 ident c
3:3 eq =
3:5 error 'é'
4:1 eof
DIAG test.hero:1:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `''` does not
DIAG test.hero:2:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `'ab'` does not
DIAG test.hero:3:5: error[char_literal]: a character literal holds exactly one ASCII character ('a', '0', ' ') — `'é'` does not
"
    );
}
