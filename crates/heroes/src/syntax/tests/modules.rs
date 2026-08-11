//! `use geom` — the line that names a module (panel 031, spec "Files and
//! layout").
//!
//! Three properties, and the last two are the panel's whole argument for a bare
//! identifier over a quoted path: the shape a model on autopilot writes gets a
//! machine-applicable repair, and the design space the identifier forecloses is
//! refused at the point of the attempt rather than in a spec sentence.

use super::dump;

#[test]
fn a_use_names_a_module() {
    assert_eq!(
        dump("use geom\n\nfunction main()\n    print(0)\n"),
        "\
file test.hero
  use geom
  function main() -> ()
    expr print(0)
"
    );
}

/// In source order, and each on its own line. Two `use`s are two lines, not a
/// list — there is no separator syntax anywhere in this language's top level.
#[test]
fn two_uses_are_two_lines_in_source_order() {
    assert_eq!(
        dump("use lex\nuse parse\n\nfunction main()\n    print(0)\n"),
        "\
file test.hero
  use lex
  use parse
  function main() -> ()
    expr print(0)
"
    );
}

/// The quoted form is what Go, C, Python's `__import__` and this project's own
/// design.md draft all wrote, so it is the likeliest first attempt. It is a
/// diagnostic carrying a `Certain` fix: the repair is deleting two characters.
#[test]
fn a_quoted_module_is_refused_with_the_repair_written_out() {
    assert_eq!(
        dump("use \"geom\"\n\nfunction main()\n    print(0)\n"),
        "\
file test.hero
  function main() -> ()
    expr print(0)
DIAG test.hero:1:5: error[use_wants_a_name]: a module is named, not quoted — write `use geom`, and the file `geom.hero` beside this one is what it reads
"
    );
}

/// …but only when what is inside the quotes is a name this language could have
/// accepted. `use \"shapes/geom\"` is the case variant B forecloses, and
/// offering a fix that produces a second syntax error would be worse than
/// offering none (§4.17: a `Certain` fix is machine-applicable, so it must
/// leave a program that parses).
#[test]
fn a_quoted_path_is_refused_without_a_fix() {
    let text = "use \"shapes/geom\"\n\nfunction main()\n    print(0)\n";
    let src = crate::source::Source::new("test.hero".to_string(), text.to_string());
    let out = crate::syntax::parse(&src);
    assert_eq!(out.diagnostics.len(), 1, "{:?}", out.diagnostics);
    assert_eq!(out.diagnostics[0].code, "use_wants_a_name");
    assert!(out.diagnostics[0].fixes.is_empty(), "a path is not a name with quotes around it");
}

/// There is no hierarchy, and the second token is where the author finds out.
/// Both spellings a reader might reach for get the same message.
#[test]
fn a_dotted_module_name_says_there_is_no_nesting() {
    assert_eq!(
        dump("use geom.shapes\n\nfunction main()\n    print(0)\n"),
        "\
file test.hero
  function main() -> ()
    expr print(0)
DIAG test.hero:1:9: error[module_path_has_no_parts]: a module name is one word — `use geom`. There is no nesting: every `.hero` a program reads sits beside the file that names it
"
    );
}

#[test]
fn a_slashed_module_name_says_the_same_thing() {
    let text = "use shapes/geom\n\nfunction main()\n    print(0)\n";
    let src = crate::source::Source::new("test.hero".to_string(), text.to_string());
    let out = crate::syntax::parse(&src);
    assert_eq!(out.diagnostics.len(), 1, "{:?}", out.diagnostics);
    assert_eq!(out.diagnostics[0].code, "module_path_has_no_parts");
}

/// `use` with nothing after it. The message names the shape rather than the
/// token class, because "expected an identifier" is true and useless.
#[test]
fn a_use_with_no_name_says_what_the_line_looks_like() {
    assert_eq!(
        dump("use\n\nfunction main()\n    print(0)\n"),
        "\
file test.hero
  function main() -> ()
    expr print(0)
DIAG test.hero:1:1: error[expected_module_name]: `use` names one module and this line names none — `use geom`, which reads `geom.hero` beside this file
"
    );
}

/// `import` and `include` kept the foreign-word treatment when `use` left it,
/// and they gained what they never had: the repair is now a pure word-for-word
/// swap, so it is `Certain`. Before M8a the message said "modules do not exist
/// yet" and carried no fix, because there was nothing to point at.
#[test]
fn import_is_repaired_into_use() {
    let text = "import geom\n\nfunction main()\n    print(0)\n";
    let src = crate::source::Source::new("test.hero".to_string(), text.to_string());
    let out = crate::syntax::parse(&src);
    let first = &out.diagnostics[0];
    assert_eq!(first.code, "reserved_word", "{:?}", out.diagnostics);
    assert!(first.message.contains("`use geom`"), "{}", first.message);
    assert_eq!(first.fixes.len(), 1, "{:?}", first.fixes);
    assert_eq!(first.fixes[0].replacement, "use");
}
