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

/// **fixedbugs, panel 032 D1, 2026-08-12.** Symptom: `heroes build` exit 2,
/// `internal error: compiling the generated C failed: error: redefinition of
/// 'h_print_inst_value_name'`, on a program with no mistake in it. Cause: the
/// emitter mangled with `FileEntry.module`, the raw `use` name, so module
/// `print` with `inst_value_name` and module `print_inst` with `value_name`
/// produced one C symbol. `module_of` had always sanitised — the emitter simply
/// was not asking it. Fix: `FileEntry.component`, taken by the mangler and by
/// nothing else.
///
/// The pair is not invented: it is the one `emit/mangle.rs`'s own doc names as
/// the reason the component is sanitised, and they are this compiler's own file
/// names. Panel 031 R10 built a diagnostic for the neighbouring case and this
/// one walked past it, because R10 compares module *names* and the collision is
/// in the concatenation.
#[test]
fn fixedbugs_two_modules_whose_concatenations_collide_but_whose_components_do_not() {
    let said = graph(
        "fixedbugs-concat",
        &[
            ("main.hero", "use print_inst\n\nfunction inst_value_name() -> int\n    return 1\n\nfunction main()\n    print(inst_value_name() + print_inst.value_name())\n"),
            ("print_inst.hero", "function value_name() -> int\n    return 2\n"),
        ],
    );
    assert!(said.is_empty(), "there is nothing wrong with this program: {said:?}");

    let src = load_root(
        "fixedbugs-concat",
        &[
            ("main.hero", "use print_inst\n\nfunction inst_value_name() -> int\n    return 1\n\nfunction main()\n    print(inst_value_name() + print_inst.value_name())\n"),
            ("print_inst.hero", "function value_name() -> int\n    return 2\n"),
        ],
    );
    // The two components differ, which is the whole repair: `print` and
    // `printinst`, so the symbols are `h_print_inst_value_name` and
    // `h_printinst_value_name`.
    let components: Vec<&str> = src.files().iter().map(|f| f.component.as_str()).collect();
    assert_eq!(components, vec!["main", "printinst", "library"]);
}

/// The neighbouring case, still refused: two module names that sanitise to one
/// component. R10's own case, and the one this file must keep firing.
#[test]
fn two_modules_one_component_is_still_refused() {
    let said = graph(
        "fixedbugs-component",
        &[
            ("main.hero", "use print_inst\nuse printinst\n\nfunction main()\n    print(print_inst.a() + printinst.b())\n"),
            ("print_inst.hero", "function a() -> int\n    return 1\n"),
            ("printinst.hero", "function b() -> int\n    return 2\n"),
        ],
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_names_collide"), "{said:?}");
}

/// The whole frontend over a set of real files, as a real invocation runs it:
/// every diagnostic rendered one to a line, plus §4.16's hole report.
///
/// `graph` above answers only "is this set of modules well formed"; these two
/// cases are about what the *resolver* and the *checker* say once it is, which
/// is a different question and needs the rest of the pipeline.
fn frontend(test: &str, files: &[(&str, &str)], root: &str) -> (Vec<String>, String) {
    let dir = write(test, files);
    let src = load(&dir.join(root).display().to_string()).expect("the root file reads");
    let parsed = crate::syntax::parse(&src);
    assert!(parsed.diagnostics.is_empty(), "the test's own input must parse");
    let resolved = crate::resolve::resolve(&parsed.ast, &src);
    let checked = crate::types::check(&parsed.ast, &resolved, &src);
    // The one-line form **plus its notes**: §4.17 makes the note the half that
    // carries the other end of the mistake, and a helper that dropped it could
    // not see the class of defect this file keeps finding.
    let said = resolved
        .diagnostics
        .iter()
        .chain(checked.diagnostics.iter())
        .map(|d| {
            let mut line = d.render_line(&src);
            for note in &d.notes {
                line.push_str(&format!("\n  note: {note}"));
            }
            line
        })
        .collect();
    (said, crate::types::report_holes(&parsed.ast, &resolved, &checked, &src))
}

/// **fixedbugs, panel 033 D1, 2026-08-12.** Symptom: an unfinished `geom.hero`
/// silenced §4.4's unused-binding rule in a `main.hero` nobody was editing —
/// `heroes check main.hero` exit 0 with the hole, exit 1 with the same
/// `main.hero` once the hole was filled. Cause: `Resolved::has_hole` was one
/// `bool` over a flat scan of `ast.exprs`, and since M8a one `Ast` spans every
/// module, so "the file contains a `???`" became "the program does". Fix:
/// `Resolved::holes_in`, a set of modules, asked per binding through
/// `hole_covers`.
///
/// design.md §4.16 is normative and was never ambiguous — *"The suppression is
/// file-wide and lifts the moment the last hole is filled"* — so this is
/// CLAUDE.md §12 exactly: the document was right and the compiler had the bug.
/// It is also the reason panel 033 refused tier D's own thesis argument twice
/// over: a top-level dead-code rule would have inherited this, and one
/// unfinished module would have silenced it across a whole program.
#[test]
fn fixedbugs_a_hole_in_one_module_does_not_silence_the_unused_rule_in_another() {
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    unused_here = 42\n    print(geom.area(w: 2, h: 3))\n"),
        ("geom.hero", "function area(w: int, h: int) -> int\n    return ???\n"),
    ];
    let (said, _) = frontend("fixedbugs-hole-scope", files, "main.hero");
    assert!(
        said.iter().any(|d| d.contains("unused_binding") && d.contains("unused_here")),
        "the hole is in `geom`, so `main`'s unused binding is still an error: {said:?}"
    );

    // The other direction, which is the rule itself and must keep working: a
    // hole in the binding's **own** module suspends it.
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    unused_here = 42\n    print(geom.area(w: 2, h: 3))\n    print(???)\n"),
        ("geom.hero", "function area(w: int, h: int) -> int\n    return w * h\n"),
    ];
    let (said, _) = frontend("fixedbugs-hole-scope-own", files, "main.hero");
    assert!(
        !said.iter().any(|d| d.contains("unused_binding")),
        "§4.16's exemption still applies inside the module that holds the hole: {said:?}"
    );
}

/// **fixedbugs, panel 033 D2, 2026-08-12.** Symptom: `heroes check main.hero` on
/// a program whose hole is in `geom.hero` answered `hole at main.hero:8:12` —
/// the wrong file, and a line number past the end of the file it named
/// (`main.hero` is five lines long). Cause: `types/holes.rs` assembled the
/// location triple itself from `line_col` and `src.name`, which was the same
/// answer while a `Source` held one file and a false one the moment it held
/// several. Fix: `Source::locate`, the function `source/mod.rs` documents as
/// mandatory for exactly this — *"there is one function and no caller assembles
/// the triple itself."*
///
/// It survived M8a's sweep because the hole report is **not** a `Diagnostic`,
/// and the sweep went through the diagnostics. That is the whole lesson: the
/// invariant was enforced by convention over one type, and the one caller
/// outside that type kept the defect.
#[test]
fn fixedbugs_a_hole_report_names_its_own_file_and_its_own_line() {
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    print(geom.area(w: 2, h: 3))\n"),
        ("geom.hero", "function area(w: int, h: int) -> int\n    return ???\n"),
    ];
    let (_, holes) = frontend("fixedbugs-hole-location", files, "main.hero");
    // The path is the temp directory's, so the assertion is on the tail: the
    // file that holds the hole, and **its own** line 2 — not a line counted
    // through the concatenated text, which is what the defect reported.
    let first = holes.lines().next().unwrap_or_default();
    assert!(first.ends_with("geom.hero:2:12"), "the report reads: {first}");
    assert!(!first.contains("main.hero"), "the root file is not where the hole is: {first}");
}

/// **fixedbugs, sweep 001 N1, 2026-08-12.** D1's twin, in the pass D1's fix did
/// not reach: the checker held `missing_return` back while `checked.holes` was
/// non-empty, and since M8a one `Checked` spans every module — so an unfinished
/// `geom.hero` suppressed the error in a `main.hero` nobody was editing.
/// design.md §4.16 says *"the suppression is file-wide"* and the code's own
/// comment said so too, three lines above the test that asked about the program.
#[test]
fn fixedbugs_a_hole_in_one_module_does_not_hold_back_missing_return_in_another() {
    let files = &[
        ("main.hero", "use geom\n\nfunction pick(f: bool) -> int\n    if f\n        return 1\n\nfunction main()\n    print(pick(true) + geom.f())\n"),
        ("geom.hero", "function f() -> int\n    return ???\n"),
    ];
    let (said, _) = frontend("fixedbugs-missing-return-scope", files, "main.hero");
    assert!(
        said.iter().any(|d| d.contains("missing_return")),
        "the hole is in `geom`, so `main`'s missing return still fires: {said:?}"
    );

    // And the rule itself, which must keep working: a hole in the same module.
    let files = &[
        ("main.hero", "function pick(f: bool) -> int\n    if f\n        return 1\n\nfunction main()\n    print(pick(true))\n    print(???)\n"),
    ];
    let (said, _) = frontend("fixedbugs-missing-return-own", files, "main.hero");
    assert!(!said.iter().any(|d| d.contains("missing_return")), "{said:?}");
}

/// **fixedbugs, sweep 001 N2, 2026-08-12.** `Resolver::suggested` was a set of
/// bare names over the whole program, so a did-you-mean offered in one module
/// exempted that name from the unused sweep in another — dropping a spec-line-77
/// error outright, with nothing to show it had happened.
#[test]
fn fixedbugs_a_repair_offered_in_one_module_does_not_excuse_another() {
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    total = 1\n    print(geom.f())\n"),
        ("geom.hero", "function f() -> int\n    total = 3\n    return totl\n"),
    ];
    let (said, _) = frontend("fixedbugs-suggested-scope", files, "main.hero");
    assert!(
        said.iter().any(|d| d.contains("unused_binding") && d.contains("main.hero")),
        "`main`'s unused `total` is still an error: {said:?}"
    );
}

/// **fixedbugs, sweep 001 N9, 2026-08-12.** `module_declaring` was a `find` over
/// a `BTreeMap` keyed by `(module, name)`, so it answered with the
/// **alphabetically first** module of however many declare that name. It had
/// exactly one possible answer while a program was one file.
///
/// Three failures at once, all reproduced: it named a module the file cannot
/// see, attached a `guess` fix that gives `wrong_arity` if followed, and
/// cascaded a false `unused_binding` telling the author to delete the `use geom`
/// line that was the real fix. Renaming `alpha.hero` to `zeta.hero` — a file
/// nobody mentions — produced the right answer with a `certain` fix, which is
/// what made it a sort-order artifact rather than a lookup bug.
#[test]
fn fixedbugs_the_module_a_name_is_in_is_one_this_file_can_see() {
    let files = &[
        ("main.hero", "use geom\nuse beta\n\nfunction main()\n    print(scale(2) + beta.b())\n"),
        ("geom.hero", "function scale(n: int) -> int\n    return n * 10\n"),
        ("beta.hero", "use alpha\n\nfunction b() -> int\n    return alpha.scale(a: 1, b: 2)\n"),
        ("alpha.hero", "function scale(a: int, b: int) -> int\n    return a * b\n"),
    ];
    let (said, _) = frontend("fixedbugs-module-declaring", files, "main.hero");
    let named: Vec<&String> =
        said.iter().filter(|d| d.contains("needs_qualifying") || d.contains("needs_a_use")).collect();
    assert_eq!(named.len(), 1, "{said:?}");
    assert!(named[0].contains("`geom`"), "the module this file can see: {said:?}");
    assert!(
        !said.iter().any(|d| d.contains("unused_binding")),
        "and no cascade telling the author to delete the fix: {said:?}"
    );
}

/// **fixedbugs, sweep 001 N5, 2026-08-12.** Ten sites formatted a line number
/// **into a message** — `note: declared at line 6`, `the cycle is: A.b: B
/// (line 7)`, `already declared at line 6` — from `line_col`, naming no file.
/// Correct while `lines_before` was zero for everything; after M8a, a line that
/// is not in the file the reader is looking at. One of them was internally
/// consistent and wrong in both halves: a `shadowed_binding` whose note said
/// "line 8" while its own caret sat on `geom.hero:8`.
///
/// The reason M8a's sweep walked past all ten is the finding, not the fix: they
/// build real `Diagnostic`s, and the sweep fixed where a diagnostic's **span**
/// is rendered. A location formatted into a `String` note is outside every guard
/// this compiler has, so `Source::locate` could not protect it — which is why
/// the repair is a second function, `Source::elsewhere`, rather than another
/// pass of the same discipline.
///
/// Both directions are asserted. The short form is deliberate: §4.17 asks the
/// note to carry the file the model would otherwise open, and naming the file
/// already on screen is noise.
#[test]
fn fixedbugs_the_other_end_of_a_mistake_names_its_file_when_it_is_elsewhere() {
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    print(geom.area(3, 4))\n"),
        ("geom.hero", "function area(side: int) -> int\n    return side * side\n"),
    ];
    let (said, _) = frontend("fixedbugs-other-end-cross", files, "main.hero");
    let arity: Vec<&String> = said.iter().filter(|d| d.contains("wrong_arity")).collect();
    assert_eq!(arity.len(), 1, "{said:?}");
    assert!(arity[0].contains("geom.hero:1"), "the note names the other file: {said:?}");
    assert!(!arity[0].contains("at line "), "and not a bare line number: {said:?}");

    // Same file, same rule, terse — the case that must not become noisier.
    let files = &[(
        "main.hero",
        "function f() -> int\n    return 1\n\nfunction f() -> int\n    return 2\n\nfunction main()\n    print(f())\n",
    )];
    let (said, _) = frontend("fixedbugs-other-end-same", files, "main.hero");
    let twice: Vec<&String> = said.iter().filter(|d| d.contains("declared_twice")).collect();
    assert_eq!(twice.len(), 1, "{said:?}");
    assert!(twice[0].contains("at line 1"), "one file, no file name: {said:?}");
    assert!(!twice[0].contains("main.hero:1 —"), "{said:?}");
}

/// **fixedbugs, sweep 001 N10, 2026-08-12.** §4.16's hole report offered
/// functions from modules the hole's own file cannot name: a hole in `geom.hero`
/// was handed `main.tally(x: int)`, and `geom` cannot `use main` without a
/// module cycle — so writing the suggestion is three errors.
///
/// The code stated the right principle two lines above and applied it halfway:
/// it *qualified* a cross-module name, which is necessary, and never asked
/// whether the file could reach it, which is the other half. §4.16's whole
/// promise is that you are handed the answer rather than made to guess.
#[test]
fn fixedbugs_a_hole_is_only_offered_what_its_own_file_can_name() {
    let files = &[
        ("main.hero", "use geom\n\nfunction tally(x: int) -> int\n    return x\n\nfunction main()\n    print(geom.f())\n"),
        ("geom.hero", "function f() -> int\n    return ???\n"),
    ];
    let (_, holes) = frontend("fixedbugs-hole-reach", files, "main.hero");
    assert!(holes.contains("hole at"), "{holes}");
    assert!(
        !holes.contains("main.tally"),
        "`geom` cannot `use main` without a cycle, so it is not an answer: {holes}"
    );
}

/// **fixedbugs, sweep 001 audit S2, 2026-08-12.** A user file named
/// `library.hero` had its declarations resolvable **unqualified** from every
/// module: `square(3)` compiled and ran, while the same program with the file
/// renamed `util.hero` was correctly refused with `needs_qualifying`. Rename a
/// file, change the answer — N9's shape, reached through a different door.
///
/// Cause: four sites identify the library **by module name** rather than by the
/// `is_library` flag — `resolve/mod.rs`'s unqualified fallback, its type twin,
/// `qualified.rs`, and `types/holes.rs`'s suggestion filter — and the collision
/// check next door skips a pair whose modules are *equal*, which leaves a hole
/// exactly at identity. Identity is reachable, because the library is appended
/// to every compilation under that name.
///
/// Refused once here rather than filtered at all four: one refusal keeps them
/// honest, four filters are four places to forget.
#[test]
fn fixedbugs_a_user_module_may_not_be_called_library() {
    let said = graph(
        "fixedbugs-library-name",
        &[
            ("main.hero", "use library\n\nfunction main()\n    print(square(3))\n"),
            ("library.hero", "function square(n: int) -> int\n    return n * n\n"),
        ],
    );
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_name_reserved"), "{said:?}");
    assert!(said[0].contains("library.hero"), "it names the file to rename: {said:?}");

    // The control, and the reason this is a defect rather than a preference: the
    // identical program under any other stem is refused for the right reason.
    let files = &[
        ("main.hero", "use util\n\nfunction main()\n    print(square(3))\n"),
        ("util.hero", "function square(n: int) -> int\n    return n * n\n"),
    ];
    let (said, _) = frontend("fixedbugs-library-name-control", files, "main.hero");
    assert!(
        said.iter().any(|d| d.contains("needs_qualifying")),
        "an unqualified cross-module name is always an error: {said:?}"
    );
}

/// **fixedbugs, sweep 001 audit L1, 2026-08-12.** The lexer walked to
/// `src.text.len()` under a doc comment saying "one source file" — true until
/// M8a made a `Source` N files concatenated. After that the indent stack and the
/// open-bracket list crossed the boundary between files, so:
///
/// - an unclosed `(` in `main.hero` **swallowed the whole of `geom.hero`** and
///   reported itself inside the appended library, which `library::misplaced`
///   turned into `internal error … exit 2`;
/// - and so did `y =` — a half-written line, the commonest state a file is ever
///   in while somebody is editing it. The compiler answered *the compiler is
///   wrong* for the most ordinary thing there is.
///
/// Panel 007 had already fixed the rule's shape: continuation lives inside
/// brackets only, and an unclosed opener is a diagnostic rather than a silent
/// swallow. A file boundary is where that rule has to be applied, because
/// nothing in one file may continue a line in another.
#[test]
fn fixedbugs_a_bracket_opened_in_one_file_does_not_reach_the_next() {
    // Not through `frontend`: this input deliberately does not parse, which is
    // the whole case.
    let dir = write(
        "fixedbugs-bracket-boundary",
        &[
            ("main.hero", "use geom\n\nfunction main()\n    print((geom.f()\n"),
            ("geom.hero", "function f() -> int\n    return 7\n"),
        ],
    );
    let src = load(&dir.join("main.hero").display().to_string()).expect("the root file reads");
    let parsed = crate::syntax::parse(&src);
    let said: Vec<String> = parsed.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert!(
        said.iter().any(|d| d.contains("unclosed_bracket") && d.contains("main.hero")),
        "the opener is reported in the file that opened it: {said:?}"
    );
    assert!(
        !said.iter().any(|d| d.contains("<heroes library>")),
        "and nothing lands in the library: {said:?}"
    );

    // `geom.hero` is still lexed as itself — the swallow is what is fixed, not
    // the reading of the file that was being swallowed.
    let files = &[
        ("main.hero", "use geom\n\nfunction main()\n    print(geom.f())\n"),
        ("geom.hero", "function f() -> int\n    return 7\n"),
    ];
    let (said, _) = frontend("fixedbugs-bracket-boundary-ok", files, "main.hero");
    assert!(said.is_empty(), "the well-formed pair still says nothing: {said:?}");
}

/// **fixedbugs, sweep 001 audit L2, 2026-08-12.** The root file's module was
/// stored **sanitised** while every `use`d module's was stored raw — two rules
/// for one namespace, where `FileEntry.module` documents itself as *"the RAW
/// name, `_` and all, because that is what the author typed"*.
///
/// A root called `a_b.hero` was therefore the module `ab`, so `use ab` in it was
/// skipped by discovery as already-seen and answered **`modules may not form a
/// cycle: ab uses ab`** — a message about a file using itself, for a file that
/// does not, naming a module the author never wrote. `ab.hero` was never loaded.
///
/// The true answer is the one panel 031 R10 already built: the two C *components*
/// collide, and that is a different namespace from the one the author types.
#[test]
fn fixedbugs_a_root_file_keeps_the_name_the_author_typed() {
    let dir = write(
        "fixedbugs-root-stem",
        &[
            ("a_b.hero", "use ab\n\nfunction main()\n    print(ab.g())\n"),
            ("ab.hero", "function g() -> int\n    return 2\n"),
        ],
    );
    let src = load(&dir.join("a_b.hero").display().to_string()).expect("the root file reads");
    let parsed = crate::syntax::parse(&src);
    let said: Vec<String> =
        errors(&parsed.ast, &src).iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(said.len(), 1, "{said:?}");
    assert!(said[0].contains("module_names_collide"), "{said:?}");
    assert!(!said[0].contains("cycle"), "nothing here is a cycle: {said:?}");

    // And the ordinary case: a root whose stem needs sanitising, next to a module
    // whose component does not collide with it, just works.
    let files = &[
        ("a_b.hero", "use cd\n\nfunction main()\n    print(cd.g())\n"),
        ("cd.hero", "function g() -> int\n    return 2\n"),
    ];
    let (said, _) = frontend("fixedbugs-root-stem-ok", files, "a_b.hero");
    assert!(said.is_empty(), "{said:?}");
    let _ = &files;
}
