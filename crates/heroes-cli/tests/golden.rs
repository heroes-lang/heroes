//! Golden-test harness.
//!
//! design.md Part 0 calls golden tests "the single most important artifact of
//! this project and the least delegable". The tree lives at the workspace root:
//!
//!   tests/golden/check/       <name>.hero + <name>.expected  (rendered diagnostics)
//!   tests/golden/ir/          <name>.hero + <name>.expected  (the lowered IR, M-ir-lowering)
//!   tests/golden/emit/        <name>.hero + <name>.expected  (the generated C11, M-scalars-run)
//!   tests/golden/unsupported/ <name>.hero + <name>.expected  (what the backend refuses, M-scalars-run)
//!   tests/golden/run/         <name>.hero + <name>.expected  (program output at -O0 AND -O2)
//!
//! Discipline (CLAUDE.md § "Golden discipline"):
//!   - UPDATE_GOLDEN=1 may rewrite `run/` expectations after HUMAN review of the
//!     diff (quoted in the commit body). It is FORBIDDEN in `check/` and in
//!     `ir/`, and must never be used to turn a red test green without reading why
//!     it was red. `ir/` inherits the ban by the compiler-engineer's condition in
//!     panel 019: with no desugared tree to inspect, these expectations are the
//!     only evidence Part 5's sugar table was honoured, and an expectation that
//!     can be regenerated mechanically is not evidence of anything.
//!   - Each milestone's 5 adversarial cases stay marked
//!     `# UNVERIFIED — pending debrief` until the author ratifies them;
//!     assistant-written bulk regression cases are marked as such.
//!
//! M-day-zero: both directories exist and are empty; the harness passes with zero cases.
//! From M-token-stream on, each case is executed through the compiler and diffed.

use std::path::{Path, PathBuf};

mod expectation;
use expectation::{check, split_expectation};

/// A path as the goldens spell it: `/` on every platform.
///
/// The compiler echoes the path it was **given** — which is right, because a
/// reader on Windows types `\` and wants to see `\` back. So the normalisation
/// belongs here, in what the harness hands it: an expectation file records what
/// the compiler does with a path, and it must not also record which operating
/// system ran the test (2026-08-14, the third CI leg).
fn slashed(path: &Path) -> String {
    path.display().to_string().replace('\\', "/")
}

fn workspace_root() -> PathBuf {
    // crates/heroes-cli/ -> crates/ -> workspace root
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("workspace root exists")
        .to_path_buf()
}

/// Collects `.hero`/`.expected` pairs, deterministically ordered.
/// Panics on a half-pair: an unpaired file is a broken test, not a skip.
fn collect_cases(dir: &Path) -> Vec<PathBuf> {
    let mut cases = Vec::new();
    let entries = std::fs::read_dir(dir)
        .unwrap_or_else(|e| panic!("cannot read {}: {e}", dir.display()));
    for entry in entries {
        let path = entry.expect("readable dir entry").path();
        match path.extension().and_then(|e| e.to_str()) {
            Some("hero") => {
                let expected = path.with_extension("expected");
                assert!(
                    expected.exists(),
                    "golden case {} is missing its .expected file",
                    path.display()
                );
                cases.push(path);
            }
            Some("expected") => {
                assert!(
                    path.with_extension("hero").exists(),
                    "orphan expectation: {} has no .hero source",
                    path.display()
                );
            }
            _ => {} // .gitkeep, editor droppings — ignored
        }
    }
    cases.sort(); // deterministic order, always
    cases
}

/// The deepest frontend stage that exists today: `lex` at M-token-stream, `parse` at M-syntax-tree,
/// `check` from M-name-resolution. `check/` cases run through it and pin every diagnostic it
/// renders — which is why the stage moves rather than the cases: a case written
/// for the lexer must keep saying the same thing when a later stage starts
/// reading the same file.
///
/// The name stops moving here. M-checker-core–M-rich-diagnostics deepen `check` without renaming it, and
/// the move to it changed **nothing**: all twelve inherited cases hold a lexer
/// or parser mistake, and `check` declines to resolve a tree built out of
/// recovery guesses, so it prints exactly what `parse` printed (verified
/// case-by-case, and quoted in the commit that moved it).
///
/// Each move refreshes the expectations exactly once, with the diff read and
/// quoted in the commit body. `UPDATE_GOLDEN` stays forbidden here
/// (CLAUDE.md § Golden discipline).
const FRONTEND_CMD: &str = "check";

/// The goldens pin the **one-line** form, which is why `--brief` is passed: it is
/// one line per diagnostic and its shape does not move, so a case's expectation
/// is about the *message* rather than about the renderer's layout. §4.17's rich
/// form is pinned in a crate test instead — the layout is one thing, and forty
/// expectations quoting it would make every future change to it a forty-file
/// commit.
const FRONTEND_FLAG: &str = "--brief";

#[test]
fn golden_tree_is_well_formed() {
    let root = workspace_root().join("tests/golden");
    for sub in ["check", "ir", "emit", "unsupported", "run"] {
        let dir = root.join(sub);
        assert!(dir.is_dir(), "missing golden directory {}", dir.display());
        let cases = collect_cases(&dir);
        println!("golden/{sub}: {} case(s)", cases.len());
    }
}

/// Every `check/` case, through the real binary, diffed against its
/// `.expected`. Cases are invoked with a path relative to the workspace
/// root so the rendered `file:line:col` prefixes are identical on every
/// machine.
#[test]
fn golden_check_cases_render_their_diagnostics() {
    let root = workspace_root();
    for case in collect_cases(&root.join("tests/golden/check")) {
        let relative = case
            .strip_prefix(&root)
            .expect("case lives under the workspace root");
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .arg(FRONTEND_CMD)
            .arg(slashed(relative))
            .arg(FRONTEND_FLAG)
            .output()
            .expect("the heroes binary runs");
        let actual = String::from_utf8_lossy(&output.stderr).into_owned();
        let expected_path = case.with_extension("expected");
        let expected = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        assert_eq!(
            actual,
            expected,
            "\ngolden mismatch for {}\n--- expected ---\n{expected}--- actual ---\n{actual}",
            relative.display()
        );
        // Exactly 1, not merely non-zero: panel 016's contract distinguishes
        // "the input has diagnostics" from "the tool could not run", and a case
        // that started failing to *run* would otherwise pass this test.
        assert_eq!(
            output.status.code(),
            Some(1),
            "{} is a diagnostics case: the compiler must exit 1",
            relative.display()
        );
    }
}

/// `x.fixed` — where a case has one, applying every `certain` fix to `x.hero`
/// must produce it *and* the result must check clean.
///
/// This is CLAUDE.md §8's promise made executable: "only `certain` is
/// machine-applicable" is a claim about every fix the compiler tags, and a fix
/// that produces a program the compiler then rejects would falsify it. The
/// harness applies them through the real binary (`check --apply`), so what is
/// tested is the shipped path and not a test helper's imitation of it.
#[test]
fn applying_certain_fixes_produces_the_fixed_file_and_it_checks_clean() {
    let root = workspace_root();
    let mut seen = 0;
    for case in collect_cases(&root.join("tests/golden/check")) {
        let fixed_path = case.with_extension("fixed");
        if !fixed_path.exists() {
            continue;
        }
        seen += 1;
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let applied = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args([FRONTEND_CMD, &slashed(relative), "--apply"])
            .output()
            .expect("the heroes binary runs");
        let got = String::from_utf8_lossy(&applied.stdout).into_owned();
        let expected = std::fs::read_to_string(&fixed_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", fixed_path.display()));
        assert_eq!(
            got,
            expected,
            "\napplied fixes differ for {}\n--- expected ---\n{expected}--- actual ---\n{got}",
            relative.display()
        );
        // …and the repaired program is accepted. A `certain` fix that leaves the
        // file broken is a `guess` that lied.
        let fixed_relative = fixed_path.strip_prefix(&root).expect("under the root");
        let rechecked = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args([FRONTEND_CMD, &fixed_relative.display().to_string(), FRONTEND_FLAG])
            .output()
            .expect("the heroes binary runs");
        assert_eq!(
            rechecked.status.code(),
            Some(0),
            "{} does not check clean after its fixes:\n{}",
            fixed_relative.display(),
            String::from_utf8_lossy(&rechecked.stderr)
        );
    }
    assert!(seen >= 3, "expected at least three `.fixed` cases, found {seen}");
}

/// Every `ir/` case, through the real binary: `build --dump-ir` on stdout, diffed
/// against its `.expected`.
///
/// One case per live row of Part 5's sugar table, which is panel 019's condition
/// for having no desugared tree — the erasure has to be *inspectable* somewhere,
/// and this is where. Plus the five adversarial cases, each of which pins a shape
/// whose failure mode is a program that compiles and does the wrong thing.
#[test]
fn golden_ir_cases_lower_to_their_expected_form() {
    let root = workspace_root();
    let cases = collect_cases(&root.join("tests/golden/ir"));
    assert!(cases.len() >= 20, "the IR goldens lost files: {}", cases.len());
    for case in cases {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["build", &slashed(relative), "--dump-ir"])
            .output()
            .expect("the heroes binary runs");
        let actual = String::from_utf8_lossy(&output.stdout).into_owned();
        let expected_path = case.with_extension("expected");
        let expected = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        assert_eq!(
            actual,
            expected,
            "\nIR golden mismatch for {}\n--- expected ---\n{expected}--- actual ---\n{actual}",
            relative.display()
        );
        // 0, not merely non-2: an `ir/` case is a program that lowers, so a
        // diagnostic in one is a broken case rather than a passing test.
        assert_eq!(
            output.status.code(),
            Some(0),
            "{} must lower clean:\n{}",
            relative.display(),
            String::from_utf8_lossy(&output.stderr)
        );
    }
}

/// The dump is deterministic: two runs of the same input produce the same bytes.
///
/// This is the cheap analogue of CLAUDE.md §7's double-emit test, one milestone
/// before there is any C to emit. It exists because determinism is the *only*
/// promise panel 019 made about this artifact — it is explicitly **not**
/// version-stable, which is LLVM's own stance on `.ll` — and a promise nothing
/// checks is not a promise.
#[test]
fn the_dump_is_byte_identical_on_a_second_run() {
    let root = workspace_root();
    for case in collect_cases(&root.join("tests/golden/ir")) {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let run = || {
            let output = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
                .current_dir(&root)
                .args(["build", &slashed(relative), "--dump-ir"])
                .output()
                .expect("the heroes binary runs");
            String::from_utf8_lossy(&output.stdout).into_owned()
        };
        assert_eq!(run(), run(), "{} dumps differently twice", relative.display());
    }
}

/// **R1, enforced rather than intended** (panel 019): a diagnostic never speaks in
/// the IR's vocabulary, and `$` is the one character that proves it. Every
/// temporary and every synthetic slot carries it, and it cannot appear in a Heroes
/// program — so a `$` in a rendered diagnostic means an IR word escaped into a
/// message the author is supposed to be able to act on.
///
/// The llm-ergonomist made this a condition of accepting R1 at all, with the
/// reason stated: without a mechanical check, R1 decays to R2 one diagnostic at a
/// time.
#[test]
fn no_diagnostic_speaks_in_the_ir_s_vocabulary() {
    let root = workspace_root();
    for case in collect_cases(&root.join("tests/golden/check")) {
        let expected = case.with_extension("expected");
        let text = std::fs::read_to_string(&expected)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected.display()));
        assert!(
            !text.contains('$'),
            "{} contains `$`, which only the IR uses: an IR word reached a diagnostic",
            expected.display()
        );
    }
    // The runtime's abort strings are the other place a `$` could reach a user.
    let runtime = std::fs::read_to_string(root.join("runtime/runtime.c"))
        .expect("the runtime must exist");
    assert!(!runtime.contains('$'), "runtime/runtime.c prints a `$`");
}

/// **Every diagnostic is annotated in the source that provokes it**, and every
/// annotation is matched by a diagnostic.
///
/// This is rustc's rule, adopted verbatim including its reason. `tests/ui/` keeps a
/// full `.stderr` snapshot *and* requires inline `//~ ERROR` annotations, and the
/// dev guide says why: "This redundancy helps avoid mistakes since the `.stderr`
/// files are usually auto-generated. It also helps to directly see where the error
/// spans are expected to point to by looking at one file instead of having to
/// compare the `.stderr` file with the source. Finally, they ensure that no
/// additional unexpected errors are generated."
///
/// Four implementations converged on in-file expectations: rustc's `//~ ERROR`, Go's
/// `// ERROR "regexp"` under an `// errorcheck` header, Zig's trailing manifest of
/// `:line:col: error:` lines, and LLVM's `; CHECK:` beside the offending construct.
/// QBE goes furthest and puts the driver *and* the expected output in the input.
///
/// What it buys here is mechanical rather than stylistic. CLAUDE.md §9 forbids
/// `UPDATE_GOLDEN=1` in `check/` and relies on a human reading the diff; a
/// regenerator can rewrite `x.expected`, but it **cannot invent an annotation in
/// `x.hero`**. The prohibition stops being a convention and becomes an invariant.
///
/// Two forms, and the second exists because the annotation cannot always sit on the
/// offending line (rustc has `//~^` and `//~v` for the same reason):
///
/// - `#~ <code>…` — a diagnostic with each code is expected **on this line**;
/// - `#~v <code>…` — on the **next** line. `unterminated.hero` needs it because a
///   trailing comment would land inside the unterminated string, and
///   `missing-body.hero` because one of its diagnostics points at the position just
///   past the last line of the file.
///
/// The retrofit's own evidence: annotating all twenty-eight cases changed **no**
/// `.expected` file by a single byte.
///
/// One consequence to expect rather than be surprised by: a `.fixed` file carries the
/// annotations through, so a repaired program contains an annotation for a diagnostic
/// it no longer has. That is harmless — a comment provokes nothing, and the
/// `.fixed` file still has to check clean — and this test deliberately does not look
/// at `.fixed` files, only at the `.hero` cases that produce diagnostics.
#[test]
fn every_diagnostic_is_annotated_in_the_source_that_provokes_it() {
    let root = workspace_root();
    let mut annotated = 0;
    let mut cases = collect_cases(&root.join("tests/golden/check"));
    // The `unsupported` kind is inside the diagnostic system precisely so that it
    // inherits this invariant (panel 020, the historian's decisive row): a report
    // that lived outside `Diagnostic` would be the only test class in the repo
    // exempt from CLAUDE.md §9.
    cases.extend(collect_cases(&root.join("tests/golden/unsupported")));
    for case in cases {
        let hero = std::fs::read_to_string(&case).expect("a readable case");
        let expected_path = case.with_extension("expected");
        let expected = std::fs::read_to_string(&expected_path).expect("a readable expectation");
        let name = case.file_name().expect("a file name").to_string_lossy().into_owned();

        let mut claimed = annotations(&hero);
        let mut reported = diagnostics(&expected);
        annotated += reported.len();
        claimed.sort();
        reported.sort();
        assert_eq!(
            claimed, reported,
            "\n{name}: the annotations and the diagnostics disagree.\n  \
             annotated: {claimed:?}\n  reported:  {reported:?}\n  \
             `#~ <code>` marks this line, `#~v <code>` the next one."
        );
    }
    assert!(annotated >= 60, "only {annotated} annotated diagnostics");
}

/// `(line, code)` for every annotation in a case, resolving `#~v` to the next line.
fn annotations(text: &str) -> Vec<(u32, String)> {
    let mut found = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let Some((_, marks)) = line.split_once("#~") else { continue };
        // One comment may carry both forms: `#~ empty_record #~v empty_variant`.
        for (offset, chunk) in marks.split("#~").enumerate() {
            let chunk = chunk.trim();
            let (delta, codes) = match chunk.strip_prefix('v') {
                Some(rest) => (1, rest),
                None if offset == 0 => (0, chunk),
                None => (0, chunk),
            };
            for code in codes.split_whitespace() {
                found.push((index as u32 + 1 + delta, code.to_string()));
            }
        }
    }
    found
}

/// `(line, code)` for every diagnostic in an expectation.
///
/// Two forms, because two commands print diagnostics: `check --brief`'s one-liner
/// `<file>:<line>:<col>: <kind>[<code>]: <message>`, and §4.17's rich form, whose
/// code and location are on consecutive lines (`<kind>[<code>]: …` then `  at
/// <file>:<line>:<col>`). The **kind** is read rather than assumed: `error` was the
/// only one until M-scalars-run added `unsupported`, and hard-coding it would have quietly
/// exempted the new kind from the invariant.
fn diagnostics(expected: &str) -> Vec<(u32, String)> {
    let mut found = Vec::new();
    let mut pending: Option<String> = None;
    for line in expected.lines() {
        // The rich form's location line, which closes a pending code.
        if let Some(rest) = line.strip_prefix("  at ") {
            if let Some(code) = pending.take() {
                if let Some(number) = rest.split(':').nth(1).and_then(|n| n.parse::<u32>().ok()) {
                    found.push((number, code));
                }
            }
            continue;
        }
        let Some(code) = kind_and_code(line) else { continue };
        // The one-line form carries its own location; the rich form's headline does
        // not, so it waits for the `at` line.
        let mut parts = line.split(':');
        let _file = parts.next();
        match parts.next().and_then(|n| n.trim().parse::<u32>().ok()) {
            Some(number) => found.push((number, code)),
            None => pending = Some(code),
        }
    }
    found
}

/// The `<code>` out of `…<kind>[<code>]:…`, for any kind.
fn kind_and_code(line: &str) -> Option<String> {
    let open = line.find('[')?;
    let close = line[open..].find(']')? + open;
    let before = &line[..open];
    let word = before.rsplit([' ', ':']).next()?;
    if word.is_empty() || !word.chars().all(|c| c.is_ascii_lowercase()) {
        return None;
    }
    let code = &line[open + 1..close];
    // Digits belong: `indentation_not_multiple_of_4` is a code.
    if code.is_empty()
        || !code.chars().all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
    {
        return None;
    }
    Some(code.to_string())
}

/// Every `unsupported/` case, through the real binary: `build` refuses it, says why
/// in §4.17's form on stderr, and exits **1**.
///
/// Exit 1 rather than 2, and that is the milestone's most consequential single
/// character. GCC has printed `sorry, unimplemented:` since version 2.5.8 and exits
/// `FATAL_EXIT_CODE`, which is `EXIT_FAILURE` — 1. The panel's llm-ergonomist
/// measured the other choice: given exit 2 its first action was `heroes --version &&
/// heroes doctor`, its second `grep -rn "M-strings-ownership" .`, and its third a message to the
/// user saying "The Heroes toolchain looks broken" — a sentence it reported verbatim
/// as false.
#[test]
fn golden_unsupported_cases_are_refused_with_a_reason_and_exit_one() {
    let root = workspace_root();
    let cases = collect_cases(&root.join("tests/golden/unsupported"));
    assert!(cases.len() >= 3, "the unsupported goldens lost files: {}", cases.len());
    for case in cases {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["build", &slashed(relative)])
            .output()
            .expect("the heroes binary runs");
        let actual = String::from_utf8_lossy(&output.stderr).into_owned();
        let expected_path = case.with_extension("expected");
        let expected = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        assert_eq!(
            actual, expected,
            "\nrefusal mismatch for {}\n--- expected ---\n{expected}--- actual ---\n{actual}",
            relative.display()
        );
        assert_eq!(
            output.status.code(),
            Some(1),
            "{}: an unsupported form exits 1 — 2 says the tool could not run, and sends a reader to reinstall it",
            relative.display()
        );
        // Nothing on stdout: there is no artifact.
        assert!(output.stdout.is_empty(), "{} wrote an artifact", relative.display());
    }
}

/// Every `emit/` case, through the real binary: `build --emit-c` on stdout, diffed
/// against its `.expected`.
///
/// This is where the C's *shape* is pinned — the hoisted prologue, one label per
/// block, `__builtin_*_overflow`, the `#line` directives, the `@` pointer ABI, the
/// shim. `UPDATE_GOLDEN` is as forbidden here as in `check/` and `ir/`: these
/// expectations are the only place the generated C is read by anything other than
/// clang, and an expectation a script can regenerate is not evidence of anything.
#[test]
fn golden_emit_cases_produce_their_c() {
    let root = workspace_root();
    let cases = collect_cases(&root.join("tests/golden/emit"));
    assert!(cases.len() >= 3, "the emit goldens lost files: {}", cases.len());
    for case in cases {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let output = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["build", &slashed(relative), "--emit-c"])
            .output()
            .expect("the heroes binary runs");
        let actual = String::from_utf8_lossy(&output.stdout).into_owned();
        let expected_path = case.with_extension("expected");
        let expected = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        assert_eq!(
            actual, expected,
            "\nC mismatch for {}\n--- expected ---\n{expected}--- actual ---\n{actual}",
            relative.display()
        );
        assert_eq!(output.status.code(), Some(0), "{} must emit clean", relative.display());
    }
}

/// **The double-emit determinism test** (CLAUDE.md §7): same input, byte-identical
/// C. It stays green at all times, over every case in the two directories that have
/// emittable programs.
///
/// The ancestor is named rather than invented: GCC's bootstrap compares stage2 and
/// stage3 objects, and its manual says a mismatch "normally indicates that the
/// stage2 compiler has compiled GCC incorrectly" — the M-selfhost-fixpoint fixpoint, thirty years
/// earlier. The stronger property is checked too: the C is identical whether it goes
/// to stdout or through `-o`, because the emitted text never mentions the output
/// path.
#[test]
fn the_emitted_c_is_byte_identical_twice_and_through_o() {
    let root = workspace_root();
    let scratch = root.join("build/determinism");
    std::fs::create_dir_all(&scratch).expect("a scratch directory");
    for case in collect_cases(&root.join("tests/golden/emit"))
        .into_iter()
        .chain(collect_cases(&root.join("tests/golden/run")))
    {
        let _ = &scratch;
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let emit = |extra: Vec<String>| -> String {
            let mut command = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"));
            command
                .current_dir(&root)
                .args(["build", &slashed(relative), "--emit-c"])
                .args(extra);
            let output = command.output().expect("the heroes binary runs");
            assert_eq!(
                output.status.code(),
                Some(0),
                "{} must emit clean:\n{}",
                relative.display(),
                String::from_utf8_lossy(&output.stderr)
            );
            String::from_utf8_lossy(&output.stdout).into_owned()
        };
        let first = emit(Vec::new());
        let second = emit(Vec::new());
        assert_eq!(first, second, "{} emits differently twice", relative.display());
        let name = case.file_stem().expect("a stem").to_string_lossy().into_owned();
        let target = scratch.join(format!("{name}.c"));
        emit(vec!["-o".to_string(), target.display().to_string()]);
        let written = std::fs::read_to_string(&target).expect("the -o file");
        assert_eq!(
            first,
            written,
            "{} differs between stdout and -o: the C mentions its own output path",
            relative.display()
        );
    }
}

/// Every `run/` case compiled and **executed**, at `-O0` and at `-O2`, both
/// required to produce the expected bytes.
///
/// One corpus in more than one configuration is the cheapest multiplier in the
/// record — GHC's testsuite runs in over 30 "ways" and Zig documents four build
/// modes, so two is conservative. The class is real (GCC PR 115492: "fails at -O2
/// but passes at -O0/-O1"), and for a C *emitter* an `-O0`/`-O2` divergence is
/// usually our own undefined behaviour rather than clang's bug. One caveat on the
/// record: `-fno-strict-aliasing` deliberately switches off the largest single
/// source of `-O2` divergence, so the signal here will come from signed overflow and
/// uninitialised reads — which is what the guards and `-Werror=uninitialized` are
/// for.
#[test]
fn golden_run_cases_produce_their_output_at_both_optimisation_levels() {
    let root = workspace_root();
    let scratch = root.join("build/golden-run");
    std::fs::create_dir_all(&scratch).expect("a scratch directory");
    let cases = collect_cases(&root.join("tests/golden/run"));
    assert!(cases.len() >= 10, "the run goldens lost files: {}", cases.len());
    for case in cases {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        let expected_path = case.with_extension("expected");
        let text = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        let (expected, ending) = split_expectation(&text);
        let name = case.file_stem().expect("a stem").to_string_lossy().into_owned();

        // -O0: `build` writes a binary, and the harness runs it itself.
        let binary = scratch.join(&name);
        let built = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["build", &slashed(relative), "-o", &binary.display().to_string()])
            .output()
            .expect("the heroes binary runs");
        assert_eq!(
            built.status.code(),
            Some(0),
            "{} did not build:\n{}",
            relative.display(),
            String::from_utf8_lossy(&built.stderr)
        );
        // **From the workspace root, like the other two legs.** `run` executes the
        // binary as a child of `heroes`, which the harness invokes from the root;
        // this leg used to execute it from the crate's own directory, so a
        // program opening a relative path opened a *different file* at `-O0` than
        // at `-O2`. The case that met it worked around it with an absolute
        // `/tmp` path, which then had to be undone for Windows — so the
        // workaround outlived its reason and the disagreement it hid was still
        // there (2026-08-14).
        let ran = std::process::Command::new(&binary)
            .current_dir(&root)
            .output()
            .expect("the program runs");
        check(relative, "-O0", &ran, &expected, &ending);

        // -O2: `run` compiles and executes, and forwards the program's own status.
        let at_o2 = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["run", &slashed(relative)])
            .output()
            .expect("the heroes binary runs");
        check(relative, "-O2", &at_o2, &expected, &ending);

        // The third configuration: `-fsanitize=address,undefined`. **Not** a leak
        // gate — AddressSanitizer's is missing on Darwin arm64, and a 999-block leak
        // exits 0 in silence under it (measured, panel 021). Leaks are caught by
        // `hero_runtime_check_leaks()` in every generated `main`, which runs in all
        // three configurations. What this one adds is use-after-free and double-free,
        // which is what reference counting gets wrong.
        // Windows clang ships no sanitiser runtime — the binary links and then
        // exits 53 before `main`, because the ASan DLL is not there and UBSan has
        // no MSVC-target runtime at all (measured, 2026-08-14). The other two
        // configurations run everywhere.
        if cfg!(target_os = "windows") {
            continue;
        }
        let sanitised = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
            .current_dir(&root)
            .args(["run", &slashed(relative), "--sanitize"])
            .output()
            .expect("the heroes binary runs");
        let noise = String::from_utf8_lossy(&sanitised.stderr);
        assert!(
            !noise.contains("AddressSanitizer") && !noise.contains("runtime error:"),
            "{} tripped a sanitiser:\n{noise}",
            relative.display()
        );
        check(relative, "--sanitize", &sanitised, &expected, &ending);
    }
}

/// The generated C compiles with **no warnings at all**, at both levels.
///
/// The flag set has `-Werror=` on four warnings and plain `-W` on
/// `-Wconditional-uninitialized`, which panel 020 adopted with a falsifiable
/// disposition: **zero fires across the corpus promotes it to `-Werror=`; one fire is
/// a lowering bug and gets a case named after it**. This test is that measurement,
/// standing. If it ever fails, read the warning before touching it — clang has found
/// something about the emitter, at the one milestone where the emitter is small
/// enough to fix cheaply.
#[test]
fn the_generated_c_compiles_without_a_single_warning() {
    let root = workspace_root();
    for case in collect_cases(&root.join("tests/golden/run"))
        .into_iter()
        .chain(collect_cases(&root.join("tests/golden/emit")))
    {
        let relative = case.strip_prefix(&root).expect("under the workspace root");
        // `build` is -O0 and `run` is -O2, so the two verbs are the two
        // configurations. clang's warnings reach stderr because `toolchain::run`
        // forwards them even on success — a warning nobody sees cannot falsify
        // anything.
        for verb in ["build", "run"] {
            let built = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
                .current_dir(&root)
                .args([verb, &slashed(relative)])
                .output()
                .expect("the heroes binary runs");
            let noise = String::from_utf8_lossy(&built.stderr);
            let warnings: Vec<&str> = noise.lines().filter(|l| l.contains("warning:")).collect();
            assert!(
                warnings.is_empty(),
                "{} under `{verb}` produced clang warnings:\n{}",
                relative.display(),
                warnings.join("\n")
            );
        }
    }
}

/// The four hand-written spikes still compile, still run, and still print what they
/// say they print.
///
/// They exist because they decided things before any compiler code did — the target
/// shape (01), loops as `goto`+labels (02), the FFI (03), the container and
/// descriptor ABI (04) — and CLAUDE.md keeps calling 01 "the shape the emitter must
/// produce". But their expected output lived in a **C comment**, so when M-scalars-run moved
/// print's newline out of `hero_print_int` and into `hero_print_end`, spike 04's
/// three readable lines silently collapsed to `10-42` and nothing failed. Now each
/// one has a checked `.expected`, and the frozen ABI is checked by compiling against
/// the current runtime rather than by remembering that it was once fine.
#[test]
fn the_spikes_still_compile_and_print_what_they_claim() {
    let root = workspace_root();
    let build = root.join("build/spikes");
    std::fs::create_dir_all(&build).expect("a scratch directory");
    let object = build.join("runtime.o");
    let clang = |args: Vec<String>| -> std::process::Output {
        std::process::Command::new("clang")
            .current_dir(&root)
            .args(args)
            .output()
            .expect("clang runs")
    };
    // `gnu11` tracks `toolchain::FLAGS` (panel 047). The duplication is
    // structural — `heroes-cli` declares only a `[[bin]]`, so a test cannot
    // import the constant — so the two are kept in step by hand and by this
    // comment. A spike compiled under a dialect the product does not use is a
    // test proving a configuration nobody ships.
    let flags: Vec<String> =
        ["-std=gnu11", "-D_USE_MATH_DEFINES", "-D_CRT_SECURE_NO_WARNINGS", "-Wall", "-Werror=return-type", "-Iruntime"]
        .iter()
        .map(|f| f.to_string())
        .collect();
    let mut compile_runtime = flags.clone();
    compile_runtime.extend([
        "-c".to_string(),
        "runtime/runtime.c".to_string(),
        "-o".to_string(),
        object.display().to_string(),
    ]);
    let out = clang(compile_runtime);
    assert!(out.status.success(), "the runtime must compile: {}", String::from_utf8_lossy(&out.stderr));

    for name in ["01-first", "02-loop", "03-ffi", "04-variant"] {
        let binary = build.join(name);
        let mut args = flags.clone();
        args.push(format!("tools/spike/{name}.c"));
        args.push(object.display().to_string());
        // `-lm` for the same reason the driver drops it on Windows: the maths
        // functions are in the C runtime there and `m.lib` does not exist.
        if name == "03-ffi" && !cfg!(target_os = "windows") {
            args.push("-lm".to_string());
        }
        args.extend(["-o".to_string(), binary.display().to_string()]);
        let compiled = clang(args);
        assert!(
            compiled.status.success(),
            "spike {name} no longer compiles against the runtime:\n{}",
            String::from_utf8_lossy(&compiled.stderr)
        );
        let ran = std::process::Command::new(&binary).output().expect("the spike runs");
        let expected_path = root.join(format!("tools/spike/{name}.expected"));
        let expected = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        // **Line endings are normalised here and nowhere else.** The spikes are
        // hand-written C that calls `printf` directly; they never reach the
        // runtime call that puts the streams in binary mode, so on Windows the C
        // runtime translates `\n` on the way out. What a spike pins is the
        // *shape* the emitter must produce — the hoisted prologue, the labels,
        // the descriptor ABI — and not a byte count. The language's own output is
        // byte-exact and is asserted by `run/` and by the corpus, both of which
        // go through `_setmode`.
        assert_eq!(
            String::from_utf8_lossy(&ran.stdout).replace("\r\n", "\n"),
            expected,
            "spike {name} prints something else now"
        );
    }
}
