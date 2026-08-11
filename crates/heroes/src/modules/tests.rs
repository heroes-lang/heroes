//! Discovery, and the three ways a set of modules can be wrong.
//!
//! These tests write real files, because discovery's job *is* the filesystem —
//! a fake reader would test the walk and not the thing the walk is for. Each
//! test gets its own directory under the system temp dir, named after itself, so
//! two tests can never read each other's modules.

use super::*;

/// A directory holding the files a test names, removed and rebuilt on every run
/// so a previous failure leaves nothing behind.
fn write(test: &str, files: &[(&str, &str)]) -> PathBuf {
    let dir = std::env::temp_dir().join(format!("heroes-modules-{test}"));
    let _ = std::fs::remove_dir_all(&dir);
    std::fs::create_dir_all(&dir).expect("a temp directory");
    for (name, text) in files {
        std::fs::write(dir.join(name), text).expect("writing a module");
    }
    dir
}

fn load_root(test: &str, files: &[(&str, &str)]) -> Source {
    let dir = write(test, files);
    load(&dir.join("main.hero").display().to_string()).expect("the root file reads")
}

/// Every diagnostic the graph produces for a set of files, rendered one to a
/// line — the whole pipeline a real invocation runs, minus the binary.
fn graph(test: &str, files: &[(&str, &str)]) -> Vec<String> {
    let src = load_root(test, files);
    let parsed = crate::syntax::parse(&src);
    errors(&parsed.ast, &src).iter().map(|d| d.render_line(&src)).collect()
}

#[test]
fn a_use_line_names_the_module_and_the_scan_finds_it() {
    assert_eq!(uses_of("main.hero", "use geom\n\nfunction main()\n"), vec!["geom"]);
    assert_eq!(uses_of("main.hero", "use lex\nuse parse\n"), vec!["lex", "parse"]);
    assert_eq!(uses_of("main.hero", "function main()\n"), Vec::<String>::new());
}

/// A `use` inside a comment or a string is not a `use`. This is the whole
/// argument for lexing rather than scanning lines with a regular expression, and
/// it is why panel 031 R8 refused a second grammar.
#[test]
fn a_use_the_lexer_does_not_see_is_not_a_use() {
    assert_eq!(uses_of("main.hero", "# use geom\nfunction main()\n"), Vec::<String>::new());
    assert_eq!(
        uses_of("main.hero", "function main()\n    print(\"use geom\")\n"),
        Vec::<String>::new()
    );
}

/// Load order is source order, depth first, and the library is last. It decides
/// every line number in every diagnostic, so it is pinned rather than left to
/// whatever the walk happens to do.
#[test]
fn load_order_is_source_order_depth_first_with_the_library_last() {
    let src = load_root(
        "order",
        &[
            ("main.hero", "use lex\nuse parse\n\nfunction main()\n    print(0)\n"),
            ("lex.hero", "function tokens() -> int\n    return 1\n"),
            ("parse.hero", "use tree\n\nfunction go() -> int\n    return 2\n"),
            ("tree.hero", "function node() -> int\n    return 3\n"),
        ],
    );
    let modules: Vec<&str> = src.files().iter().map(|f| f.module.as_str()).collect();
    assert_eq!(modules, vec!["main", "lex", "parse", "tree", "library"]);
}

/// A module named twice is loaded once — and the diamond is the shape that
/// proves it, because `tree` is reached from both sides.
#[test]
fn a_module_reached_twice_is_loaded_once() {
    let src = load_root(
        "diamond",
        &[
            ("main.hero", "use lex\nuse parse\n\nfunction main()\n    print(0)\n"),
            ("lex.hero", "use tree\n\nfunction tokens() -> int\n    return 1\n"),
            ("parse.hero", "use tree\n\nfunction go() -> int\n    return 2\n"),
            ("tree.hero", "function node() -> int\n    return 3\n"),
        ],
    );
    let trees = src.files().iter().filter(|f| f.module == "tree").count();
    assert_eq!(trees, 1, "one file per module, however many name it");
}

/// A cycle terminates discovery — the `seen` set is what does it — and is then
/// refused with the ring named.
#[test]
fn a_cycle_terminates_discovery_and_is_refused_by_name() {
    let said = graph(
        "cycle",
        &[
            ("main.hero", "use a\n\nfunction main()\n    print(0)\n"),
            ("a.hero", "use b\n\nfunction f() -> int\n    return 1\n"),
            ("b.hero", "use a\n\nfunction g() -> int\n    return 2\n"),
        ],
    );
    assert_eq!(said.len(), 1, "one cycle, one diagnostic: {said:?}");
    assert!(said[0].contains("module_cycle"), "{said:?}");
    assert!(said[0].contains("a uses b uses a"), "the ring, not one edge: {said:?}");
}

/// A module that uses itself is the shortest cycle there is.
#[test]
fn a_module_that_uses_itself_is_a_cycle() {
    let said = graph(
        "self",
        &[("main.hero", "use main\n\nfunction main()\n    print(0)\n")],
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_cycle"), "{said:?}");
}

/// The file is not there. Discovery skips it silently and the message arrives
/// with the caret on the `use` line, naming the file it looked for.
#[test]
fn a_module_with_no_file_is_named_along_with_the_file_it_wanted() {
    let said = graph(
        "missing",
        &[("main.hero", "use geom\n\nfunction main()\n    print(0)\n")],
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("unknown_module"), "{said:?}");
    assert!(said[0].contains("`geom.hero`"), "it names what it looked for: {said:?}");
}

/// Panel 031 R10, and the case `mangle.rs` claimed M8a would make impossible.
/// Two legal module names, one C component, and clang's version of this message
/// is exit 2 saying the compiler has a bug.
#[test]
fn two_modules_that_mangle_to_one_name_are_refused_here_rather_than_by_clang() {
    let said = graph(
        "collide",
        &[
            ("main.hero", "use geom\nuse geo_m\n\nfunction main()\n    print(0)\n"),
            ("geom.hero", "function a() -> int\n    return 1\n"),
            ("geo_m.hero", "function b() -> int\n    return 2\n"),
        ],
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_names_collide"), "{said:?}");
    assert!(said[0].contains("h_geom_"), "{said:?}");
}

/// The ordinary case produces nothing at all, which is the assertion the other
/// tests are only interesting against.
#[test]
fn a_well_formed_program_produces_no_graph_diagnostic() {
    let said = graph(
        "clean",
        &[
            ("main.hero", "use geom\n\nfunction main()\n    print(0)\n"),
            ("geom.hero", "function dist() -> int\n    return 1\n"),
        ],
    );
    assert!(said.is_empty(), "{said:?}");
}
