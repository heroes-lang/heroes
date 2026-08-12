//! Qualified names, and the four rules `use` gets for free by binding.
//!
//! Every test here builds a real multi-file `Source` — the concatenation is the
//! thing under test as much as the resolver is, because "which module is this
//! name in" is answered by the file table and by nothing else.

use crate::source::{InputFile, Source, LIBRARY_MODULE};

/// Two user modules plus the library, exactly as a real invocation assembles
/// them. `main` is first, so its line numbers are its own.
fn program(main: &str, geom: &str) -> Vec<String> {
    let src = Source::of(vec![
        InputFile::user("main.hero".to_string(), main.to_string()),
        InputFile::user("geom.hero".to_string(), geom.to_string()),
        InputFile {
            name: "<heroes library>".to_string(),
            module: LIBRARY_MODULE.to_string(),
            text: crate::library::SOURCE.to_string(),
            is_library: true,
        },
    ]);
    let parsed = crate::syntax::parse(&src);
    assert!(parsed.diagnostics.is_empty(), "{:?}", parsed.diagnostics);
    let resolved = crate::resolve::resolve(&parsed.ast, &src);
    resolved.diagnostics.iter().map(|d| d.render_line(&src)).collect()
}

const GEOM: &str = "\
record Point
    x: i64
    y: i64

function dist2(a: Point, b: Point) -> i64
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy
";

#[test]
fn a_qualified_call_and_a_qualified_construction_resolve() {
    let said = program(
        "use geom\n\nfunction main()\n    p = geom.Point(x: 3, y: 4)\n    print(geom.dist2(a: p, b: p))\n",
        GEOM,
    );
    assert!(said.is_empty(), "{said:?}");
}

#[test]
fn a_qualified_type_resolves_in_a_signature_and_in_an_annotation() {
    let said = program(
        "use geom\n\nfunction go(p: geom.Point) -> i64\n    q: geom.Point = p\n    return geom.dist2(a: p, b: q)\n\nfunction main()\n    print(go(p: geom.Point(x: 1, y: 1)))\n",
        GEOM,
    );
    assert!(said.is_empty(), "{said:?}");
}

/// **The rule the milestone exists for.** An unqualified name from another
/// module is refused, and the error carries the repair — which is what panel
/// 031 R5 bought instead of eleven spec tokens.
#[test]
fn an_unqualified_name_from_another_module_is_refused_with_a_certain_fix() {
    let said = program(
        "use geom\n\nfunction main()\n    print(dist2(a: 1, b: 2))\n",
        GEOM,
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("needs_qualifying"), "{said:?}");
    assert!(said[0].contains("`geom.dist2`"), "{said:?}");
}

/// The same name, in a module the file has NOT named. The fix is a `Guess`,
/// because applying it leaves a file that still needs the `use` line.
#[test]
fn a_name_from_a_module_with_no_use_says_to_add_one() {
    let src = Source::of(vec![
        InputFile::user(
            "main.hero".to_string(),
            "function main()\n    print(dist2(a: 1, b: 2))\n".to_string(),
        ),
        InputFile::user("geom.hero".to_string(), GEOM.to_string()),
    ]);
    let parsed = crate::syntax::parse(&src);
    let resolved = crate::resolve::resolve(&parsed.ast, &src);
    assert_eq!(resolved.diagnostics.len(), 1, "{:?}", resolved.diagnostics);
    let first = &resolved.diagnostics[0];
    assert_eq!(first.code, "needs_qualifying");
    assert!(first.message.contains("add `use geom`"), "{}", first.message);
    assert_eq!(first.fixes.len(), 1);
    assert_eq!(
        first.fixes[0].certainty,
        crate::diagnostics::Certainty::Guess,
        "applying it leaves a file that still needs the `use`"
    );
}

/// **`use` binds**, so spec line 74's unused rule covers it with no new text.
#[test]
fn a_use_nothing_reads_is_the_unused_binding_error() {
    let said = program("use geom\n\nfunction main()\n    print(0)\n", GEOM);
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("unused_binding"), "{said:?}");
    assert!(said[0].contains("`use geom`"), "{said:?}");
}

/// …and spec line 76's shadowing rule, the same way.
#[test]
fn a_use_that_collides_with_a_declaration_is_the_shadowing_error() {
    let said = program(
        "use geom\n\nfunction geom() -> i64\n    return 1\n\nfunction main()\n    print(geom())\n",
        GEOM,
    );
    assert!(said.iter().any(|d| d.contains("shadowed_binding")), "{said:?}");
}

/// A module named twice does nothing the first one did not.
#[test]
fn one_module_named_twice_is_reported_once() {
    let said = program(
        "use geom\nuse geom\n\nfunction main()\n    print(geom.dist2(a: geom.Point(x: 0, y: 0), b: geom.Point(x: 1, y: 1)))\n",
        GEOM,
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("used_twice"), "{said:?}");
}

/// A module is not a value in any position, and saying so beats "unknown name"
/// about a name the file's own `use` line put there.
#[test]
fn a_module_name_alone_is_not_a_value() {
    let said = program("use geom\n\nfunction main()\n    print(geom)\n", GEOM);
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_is_not_a_value"), "{said:?}");
}

/// `geom.nope` — the module is right and the name is not in it. With one near
/// candidate the fix is `Certain`, because renaming preserves meaning and the
/// compiler is the one that knows the candidate is unique.
#[test]
fn a_name_that_is_not_in_the_module_names_what_is() {
    let said = program(
        "use geom\n\nfunction main()\n    print(geom.dist(a: 1, b: 2))\n",
        GEOM,
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("unknown_in_module"), "{said:?}");
    assert!(said[0].contains("`dist2`"), "{said:?}");
}

/// **Two modules may each declare `Point`.** They are two types, and the
/// duplicate rule is per module — which is the single change that makes the
/// top-level table's key a pair.
#[test]
fn two_modules_may_declare_the_same_name() {
    let said = program(
        "use geom\n\nrecord Point\n    label: str\n\nfunction main()\n    mine = Point(label: \"here\")\n    theirs = geom.Point(x: 1, y: 2)\n    print(mine.label, geom.dist2(a: theirs, b: theirs))\n",
        GEOM,
    );
    assert!(said.is_empty(), "{said:?}");
}

/// The library is the one module every file sees without naming it: its names
/// are reserved built-ins, spent by the language rather than imported.
#[test]
fn the_librarys_names_stay_unqualified_and_need_no_use() {
    let said = program(
        "use geom\n\nfunction main()\n    for i in range(from: 0, to: 2)\n        print(geom.dist2(a: geom.Point(x: i, y: 0), b: geom.Point(x: 0, y: 0)))\n",
        GEOM,
    );
    assert!(said.is_empty(), "{said:?}");
}
