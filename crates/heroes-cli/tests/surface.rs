//! The command surface, tested — because it had no tests at all until panel 016,
//! and 112 of its lines were duplicated argv parsing.
//!
//! What these pin is the *contract*, not the wording of one command's output:
//! the three exit codes, the two streams, strictness (an unknown flag is an
//! error that names what is accepted), and the retired spellings. Each is a
//! guess a model would otherwise have to make — the llm-ergonomist counted nine
//! of them in the old help text, four failing silently.

use std::process::{Command, Output};

fn heroes(args: &[&str]) -> Output {
    Command::new(env!("CARGO_BIN_EXE_heroes"))
        .current_dir(concat!(env!("CARGO_MANIFEST_DIR"), "/../.."))
        .args(args)
        .output()
        .expect("the heroes binary runs")
}

fn code(out: &Output) -> i32 {
    out.status.code().expect("a real exit code")
}

/// 0 clean · 1 the input has diagnostics · 2 the tool could not run. POSIX's own
/// shape (grep, diff), and javac's since JDK 1.x.
#[test]
fn the_three_exit_codes_mean_three_different_things() {
    assert_eq!(code(&heroes(&["check", "examples/gallery/01-points.hero"])), 0);
    assert_eq!(code(&heroes(&["check", "tests/golden/check/shadowing.hero"])), 1);
    assert_eq!(code(&heroes(&["check", "no/such/file.hero"])), 2);
    // A bad command line is the tool failing to run, not the program failing.
    assert_eq!(code(&heroes(&["check", "--nonsense", "examples/gallery/00-first.hero"])), 2);
    assert_eq!(code(&heroes(&["frobnicate"])), 2);
}

/// The artifact goes to stdout, the diagnostics to stderr. A wrapper that greps
/// stdout for errors would otherwise find nothing and conclude success.
#[test]
fn the_artifact_is_on_stdout_and_diagnostics_are_on_stderr() {
    let clean = heroes(&["parse", "examples/gallery/00-first.hero", "--dump-ast"]);
    assert!(!clean.stdout.is_empty(), "the tree goes to stdout");
    assert!(clean.stderr.is_empty(), "a clean file says nothing on stderr");

    let broken = heroes(&["check", "tests/golden/check/shadowing.hero"]);
    assert!(broken.stdout.is_empty(), "no artifact when the input is wrong");
    assert!(!broken.stderr.is_empty(), "diagnostics go to stderr");
}

/// Strict, and the error carries the list — so a wrong flag costs one round trip
/// instead of a guess.
#[test]
fn an_unknown_flag_names_what_the_command_accepts() {
    let out = heroes(&["check", "--dump-ast", "examples/gallery/00-first.hero"]);
    let message = String::from_utf8_lossy(&out.stderr).into_owned();
    assert!(
        message.starts_with("error: `check` does not accept `--dump-ast` — it accepts --dump-scopes"),
        "the error enumerates what is accepted: {message}"
    );
    let out = heroes(&["doctor", "--json"]);
    assert!(
        String::from_utf8_lossy(&out.stderr).contains("takes no flags"),
        "a command with no flags says so"
    );
}

/// A retired spelling names its replacement — the same treatment the *language*
/// gives a foreign keyword.
#[test]
fn a_retired_flag_names_its_replacement() {
    let out = heroes(&["fmt", "--write", "examples/gallery/00-first.hero"]);
    assert_eq!(
        String::from_utf8_lossy(&out.stderr).into_owned(),
        "error: `--write` is no longer a flag of `fmt` — use `--in-place`\n"
    );
    assert_eq!(code(&out), 2);
    // …and the file was not touched, which is the point of the rename.
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../examples/gallery/00-first.hero"
    ))
    .expect("first.hero exists");
    assert!(text.contains("print((2 + 3) * 4)"));
}

/// Every stage prints only when asked. Before panel 016, `lex` printed its
/// tokens by default while `parse` and `check` did not, so "which stages print"
/// was a table to memorise.
#[test]
fn a_stage_prints_only_when_asked() {
    let quiet = heroes(&["lex", "examples/gallery/00-first.hero"]);
    assert!(quiet.stdout.is_empty(), "`lex` alone is a lexical check");
    assert_eq!(code(&quiet), 0);

    let text = heroes(&["lex", "examples/gallery/00-first.hero", "--dump-tokens"]);
    assert!(String::from_utf8_lossy(&text.stdout).contains("ident main"));

    let json = heroes(&["lex", "examples/gallery/00-first.hero", "--dump-tokens", "--json"]);
    let shown = String::from_utf8_lossy(&json.stdout);
    assert!(shown.starts_with("[\n"), "--json changes how, not what: {shown}");
    assert!(shown.contains("\"kind\""));
}

/// `--json` on its own prints nothing: it says *how*, and `--dump-tokens` says
/// *what*. Two flags because they are two questions.
#[test]
fn json_alone_selects_no_output() {
    let out = heroes(&["lex", "examples/gallery/00-first.hero", "--json"]);
    assert!(out.stdout.is_empty());
    assert_eq!(code(&out), 0);
}

#[test]
fn a_missing_file_operand_is_named_with_the_shape_that_works() {
    let out = heroes(&["check"]);
    assert_eq!(
        String::from_utf8_lossy(&out.stderr).into_owned(),
        "error: `check` needs a file: `heroes check <file.hero>`\n"
    );
    assert_eq!(code(&out), 2);
    let out = heroes(&["check", "a.hero", "b.hero"]);
    assert!(String::from_utf8_lossy(&out.stderr).contains("takes one file"));
}

/// The help text carries the contract, because a script has nowhere else to read
/// it — and `--help` exits 0, per the GNU convention.
#[test]
fn the_help_text_states_the_exit_codes_and_the_streams() {
    let out = heroes(&["--help"]);
    let shown = String::from_utf8_lossy(&out.stdout);
    // The gloss on 1 widened at M5a: an unsupported form is a diagnostic about the
    // *compiler* and still exits 1, because 2 sends a reader to reinstall the
    // toolchain (panel 020, measured).
    assert!(shown.contains("1 diagnostics were reported, no artifact was produced"));
    assert!(shown.contains("2 the tool could not run."));
    assert!(shown.contains("HEROES_RUNTIME"), "the one input channel outside the table");
    assert!(shown.contains("streams: the artifact on stdout"));
    assert_eq!(code(&out), 0);
    // No arguments prints the same thing, and that is not an error either.
    let bare = heroes(&[]);
    assert_eq!(String::from_utf8_lossy(&bare.stdout), shown);
}

/// Every flag in the table appears in the help text: the table is the single
/// source, so this is what keeps the documentation from drifting from the parser.
#[test]
fn every_flag_in_the_table_is_documented() {
    let shown = String::from_utf8_lossy(&heroes(&["--help"]).stdout).into_owned();
    for flag in ["--dump-tokens", "--json", "--dump-ast", "--dump-scopes", "--in-place"] {
        assert!(shown.contains(flag), "{flag} is missing from --help");
    }
}

/// `measure`'s optional operand has a documented default, so the bracket says
/// "defaults to something sensible" rather than "I do not know what this does".
#[test]
fn measure_states_its_default() {
    let shown = String::from_utf8_lossy(&heroes(&["--help"]).stdout).into_owned();
    assert!(shown.contains("(default: spec/heroes-spec.md)"));
    assert_eq!(code(&heroes(&["measure"])), 0);
}

// --- M3d: diagnostics as a product -----------------------------------

/// §4.17's whole argument, executable: the message, the line as written, the span
/// underlined, the note that carries the other end of the mistake, and the fixes
/// with their tags. A human has the project open; a model has this.
#[test]
fn the_rich_form_carries_the_line_the_caret_and_the_other_end() {
    let out = heroes(&["check", "tests/golden/check/certain-labels.hero"]);
    let shown = String::from_utf8_lossy(&out.stderr).into_owned();
    assert!(shown.starts_with("error[wrong_label]: `Point` has nothing called `z` at this position — it is `y`\n"), "{shown}");
    assert!(shown.contains("  at tests/golden/check/certain-labels.hero:10:21"), "{shown}");
    assert!(shown.contains("    a = Point(x: 1, z: 2)"), "the line as written: {shown}");
    assert!(shown.contains("^"), "the span, underlined: {shown}");
    assert!(shown.contains("fix (certain): write `y:`"), "the fix, tagged: {shown}");
    assert_eq!(code(&out), 1);
}

/// The same diagnostics, one line each — what a terminal scans and what the
/// goldens pin, so the layout of the rich form can change without touching
/// twenty expectations.
#[test]
fn brief_is_one_line_per_diagnostic() {
    let out = heroes(&["check", "tests/golden/check/certain-labels.hero", "--brief"]);
    let shown = String::from_utf8_lossy(&out.stderr).into_owned();
    assert_eq!(shown.lines().count(), 3);
    for line in shown.lines() {
        assert!(line.starts_with("tests/golden/check/certain-labels.hero:"), "{line}");
    }
}

/// Schema 1, and the version is first because a consumer that reads it can refuse
/// a version it does not know.
#[test]
fn json_is_versioned_and_carries_the_fixes() {
    let out = heroes(&["check", "tests/golden/check/certain-labels.hero", "--json"]);
    let shown = String::from_utf8_lossy(&out.stderr).into_owned();
    assert!(shown.starts_with("{\n  \"schema\": 1,\n"), "{shown}");
    assert!(shown.contains("\"code\": \"wrong_label\""));
    assert!(shown.contains("\"certainty\": \"certain\""));
    assert!(shown.contains("\"line\": 10"));
    assert_eq!(code(&out), 1);
}

/// Part 11's control arm: the same compiler with §1's argument switched off.
/// Without it the thesis has no falsifiable claim — a language that rejects
/// everything maximises catch rate.
#[test]
fn permissive_drops_the_thesis_rules_and_keeps_the_rest() {
    // Every diagnostic in this case is a thesis rule, so permissive accepts it.
    let strict = heroes(&["check", "tests/golden/check/unused-bindings.hero", "--brief"]);
    assert_eq!(code(&strict), 1);
    assert_eq!(String::from_utf8_lossy(&strict.stderr).lines().count(), 4);
    let permissive =
        heroes(&["check", "tests/golden/check/unused-bindings.hero", "--brief", "--permissive"]);
    assert_eq!(code(&permissive), 0, "every rule in that case is a thesis rule");
    assert!(permissive.stderr.is_empty());

    // …and a rule the program's *meaning* depends on survives it.
    let both = heroes(&["check", "tests/golden/check/unknown-type.hero", "--brief", "--permissive"]);
    assert_eq!(code(&both), 1, "an unknown type is not a thesis rule");
}

/// CLAUDE.md §8: only `certain` is machine-applicable. `--apply` is that promise
/// with a command attached, and the `.fixed` goldens are the assertion that the
/// repaired program compiles.
#[test]
fn apply_repairs_the_program_with_certain_fixes_only() {
    let out = heroes(&["check", "tests/golden/check/certain-fixes.hero", "--apply"]);
    let repaired = String::from_utf8_lossy(&out.stdout).into_owned();
    assert!(repaired.contains("print(total)"), "the rename was applied: {repaired}");
    assert!(repaired.contains("for _ in range(from: 0, to: 3)"), "the loop variable: {repaired}");
    // The library is appended to every compilation and belongs to none of them:
    // `--apply` with `--in-place` writes this text into the author's file.
    assert!(!repaired.contains("The Heroes library"), "the library leaked into the repair");
    assert!(repaired.contains(".num _ => 1"), "the payload: {repaired}");
    assert_eq!(code(&out), 0, "a repair is not a failure");
    // A `guess` is never applied: this case's only fix is one, and the file comes
    // back unchanged apart from nothing.
    let untouched = heroes(&["check", "tests/golden/check/not-mutable.hero", "--apply"]);
    let text = String::from_utf8_lossy(&untouched.stdout).into_owned();
    assert!(text.contains("    x @ 2"), "a guess is prose, not an edit");
}

/// §4.16: a hole is not an error. The file type-checks, the exit code stays 0, and
/// the compiler prints what belongs in the gap — the expected type, what is in
/// scope, and which functions return it.
#[test]
fn a_hole_is_reported_on_stdout_and_the_exit_code_stays_zero() {
    let out = heroes(&["check", "examples/gallery/09-holes.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    assert_eq!(code(&out), 0, "a program with holes is not wrong");
    assert!(shown.contains("hole at examples/gallery/09-holes.hero:"), "{shown}");
    assert!(shown.contains("this function returns: Entry"), "{shown}");
    assert!(shown.contains("expected type: str"), "{shown}");
    assert!(shown.contains("in scope:"), "{shown}");
    assert!(shown.contains("entries: [Entry]"), "{shown}");
}

/// `heroes build` with no flag **says what it did**, and this is the whole point of
/// the test rather than a nicety.
///
/// The spec-warden vetoed a silent exit-0 `build` in panel 019, quoting panel 016's
/// own watch list: "`check` is not `build` … a model runs `heroes check` and reports
/// success for a program that has no backend yet". A `build` that produced nothing
/// without saying so would have promoted that misreading into the tool's behaviour.
#[test]
fn build_says_where_it_put_the_binary() {
    let out = heroes(&["build", "examples/gallery/00-first.hero"]);
    assert_eq!(code(&out), 0);
    assert!(out.stdout.is_empty(), "the binary is the artifact, and it is a file");
    let said = String::from_utf8_lossy(&out.stderr).into_owned();
    // Candidate (ii) of the panel's experiment: given this line the reader wrote
    // `heroes build f.hero && ./build/<hash>/f` correctly first try; given silence
    // it wrote `./f`, which cannot work.
    assert!(said.starts_with("wrote build/"), "{said}");
    assert!(said.trim_end().ends_with("/00first"), "{said}");
    // And the path it names exists and runs.
    let path = std::path::Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../.."))
        .join(said.trim_start_matches("wrote ").trim_end());
    let ran = std::process::Command::new(&path).output().expect("the binary runs");
    assert_eq!(String::from_utf8_lossy(&ran.stdout), "20\n");
}

/// `--dump-ir` puts the IR on stdout and leaves stderr alone: panel 016's stream
/// contract, so a wrapper can pipe one without the other.
#[test]
fn build_dumps_the_ir_on_stdout() {
    let out = heroes(&["build", "examples/gallery/00-first.hero", "--dump-ir"]);
    assert_eq!(code(&out), 0);
    let ir = String::from_utf8_lossy(&out.stdout).into_owned();
    assert!(ir.starts_with("function main()"), "{ir}");
    assert!(ir.contains("bb0"), "{ir}");
    assert!(out.stderr.is_empty(), "the summary makes way for the artifact");
}

/// The three exit codes again, for the new verb. A program with diagnostics never
/// reaches lowering — the stages are ordered, each running only on the one before it
/// having said nothing.
#[test]
fn build_honours_the_exit_code_contract() {
    assert_eq!(code(&heroes(&["build", "examples/gallery/00-first.hero"])), 0);
    assert_eq!(code(&heroes(&["build", "tests/golden/check/shadowing.hero"])), 1);
    // A correct program the backend cannot emit yet is **1**, not 2: the tool
    // worked, and no edit to the file will help. This has now named four different
    // files — `01-points.hero` until records emitted, `10-maps.hero` until `.must()`
    // did, `06-generics.hero` until monomorphisation did — which is the assertion
    // working rather than breaking: a row dies per step and the case follows it.
    // `08-ffi.hero` is the one left, and its refusal is a veto rather than a
    // milestone (§4.19: the mechanism is the `#include`).
    assert_eq!(code(&heroes(&["build", "examples/gallery/08-ffi.hero"])), 1);
    assert_eq!(code(&heroes(&["build", "no/such/file.hero"])), 2);
    assert_eq!(code(&heroes(&["build", "--dump-ast", "examples/gallery/00-first.hero"])), 2);
}

/// `run` stops being a retired spelling and becomes the dev loop (M5a). It compiles
/// at `-O2`, executes, and forwards the program's own exit status — the program is
/// the artifact, so its verdict is the command's.
#[test]
fn run_compiles_and_executes_the_program() {
    let out = heroes(&["run", "examples/gallery/00-first.hero"]);
    assert_eq!(code(&out), 0);
    assert_eq!(String::from_utf8_lossy(&out.stdout), "20\n");
    // Nothing of the tool's own before the program's output.
    assert!(out.stderr.is_empty(), "{}", String::from_utf8_lossy(&out.stderr));
}

/// `--no-line` is not born, and the message says so rather than printing the list:
/// panel 016 refused it on CLAUDE.md §10's stopping rule and panel 020 declined to
/// implement it, so a reader who plausibly reaches for it gets the reason.
#[test]
fn the_refused_no_line_flag_names_what_to_do_instead() {
    let out = heroes(&["build", "examples/gallery/00-first.hero", "--no-line"]);
    assert_eq!(code(&out), 2);
    let message = String::from_utf8_lossy(&out.stderr).into_owned();
    assert!(message.contains("no longer a flag"), "{message}");
    assert!(message.contains("--emit-c"), "{message}");
}

/// `-o` is the surface's first value-taking flag, and it is strict: a missing path
/// is an error that shows the shape rather than a silent default.
#[test]
fn the_output_flag_takes_a_path_and_says_so_when_it_is_missing() {
    let out = heroes(&["build", "examples/gallery/00-first.hero", "-o"]);
    assert_eq!(code(&out), 2);
    assert!(String::from_utf8_lossy(&out.stderr).contains("needs a path"));
}

/// Two stops in one invocation is refused rather than silently resolved: whichever
/// one won, the reader asked for the other half of the time.
#[test]
fn two_stops_in_one_invocation_are_refused() {
    let out = heroes(&["build", "examples/gallery/00-first.hero", "--dump-ir", "--emit-c"]);
    assert_eq!(code(&out), 2);
    assert!(String::from_utf8_lossy(&out.stderr).contains("one artifact at a time"));
}

/// §4.16: a file with a hole is not wrong. It lowers, the compiler says what belongs
/// in the gaps, and the summary carries the one sentence that matters — no binary can
/// come of it. Panel 019 recorded this as decided-by-default and queued it.
#[test]
fn a_hole_reports_what_belongs_there_and_the_build_exits_one() {
    let out = heroes(&["build", "examples/gallery/09-holes.hero"]);
    // §4.16 keeps both halves: the hole is not an error *and* there is no binary.
    // Panel 019 queued this as decided-by-default; panel 020 decided it, on the
    // ergonomist's measurement — under exit 0 a `build && ./artifact` loop reports
    // success and then runs yesterday's binary.
    assert_eq!(code(&out), 1, "no artifact was produced");
    assert!(!out.stdout.is_empty(), "the hole report is the artifact that does exist");
    let said = String::from_utf8_lossy(&out.stderr).into_owned();
    assert!(said.trim_end().ends_with("holes in examples/gallery/09-holes.hero"), "{said}");
    assert!(said.starts_with("no binary: 2 holes"), "{said}");
    // `check` still exits 0 on the same file: a hole is not a program defect, and
    // the two commands answer different questions.
    assert_eq!(code(&heroes(&["check", "examples/gallery/09-holes.hero"])), 0);
    // …and so does `--dump-ir`, which is inspection rather than a promise of a
    // binary.
    assert_eq!(code(&heroes(&["build", "examples/gallery/09-holes.hero", "--dump-ir"])), 0);
}

/// **A runtime part edited alone must invalidate the cached object.**
///
/// The runtime became eleven files at M6 step 3, assembled into one translation
/// unit by `runtime.c`. The cache key used to hash the two entry points, so a part
/// could be edited and the *previous* `runtime-<key>.o` relinked — and a stale
/// relink is silent: it prints yesterday's bytes at exit 0, which is exactly how
/// panel 020 measured the hazard before the key covered the runtime at all.
///
/// The test edits a part (a comment, so behaviour cannot change), rebuilds, and
/// asserts a new object appeared. It restores the file before asserting anything,
/// so a failure cannot leave the tree dirty.
#[test]
fn editing_a_runtime_part_invalidates_the_cached_object() {
    let part = std::path::Path::new(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../runtime/parts/sort.c"
    ))
    .to_path_buf();
    let objects = || {
        let dir = std::path::Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../../build"));
        let Ok(entries) = std::fs::read_dir(dir) else { return 0 };
        entries
            .flatten()
            .filter(|e| e.file_name().to_string_lossy().starts_with("runtime-"))
            .count()
    };

    let original = std::fs::read_to_string(&part).expect("the part is where runtime.c includes it");
    assert_eq!(code(&heroes(&["build", "tests/golden/run/builtins.hero"])), 0);
    let before = objects();

    // The probe must be unique per run: `build/` persists between test runs, so a
    // fixed comment would hash to a key whose object already exists and the count
    // would not move — a test that passes for the wrong reason on the second run.
    let stamp = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .expect("a clock")
        .as_nanos();
    std::fs::write(&part, format!("{original}\n/* cache-key probe {stamp} */\n"))
        .expect("writable");
    let built = heroes(&["build", "tests/golden/run/builtins.hero"]);
    let after = objects();
    std::fs::write(&part, &original).expect("restored");

    assert_eq!(code(&built), 0, "the probe must not break the build");
    assert!(
        after > before,
        "editing runtime/parts/sort.c reused the cached object ({before} -> {after}): \
         the key hashes runtime.c and heroes_runtime.h only"
    );
}

/// `heroes test` — §4.18's "run only when asked for", with the asking.
///
/// A **subcommand** under CLAUDE.md §10's stopping rule: it answers a different
/// question about the same input and its artifact is a verdict per test, where
/// `build`'s is a binary and `run`'s is a process.
#[test]
fn test_runs_each_block_and_reports_one_line_each() {
    let out = heroes(&["test", "examples/calculator.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    // The report is the artifact, so it is on stdout (§10's contract).
    assert!(shown.contains("ok   \"precedence and parens\""), "{shown}");
    assert!(shown.trim_end().ends_with("7 tests, all passed"), "{shown}");
    assert_eq!(code(&out), 0);
}

/// **M8a's acceptance criterion**: the same program, cut into four modules, and
/// the same seven tests.
///
/// It asserts more than "green". The tests come from *three* of the four files,
/// so it is also the check that `heroes test` runs every module's blocks rather
/// than the root's — the difference between `is_library` and `is_root`, which is
/// a silent loss if taken the other way. And the four `use` lines make the
/// graph four deep, `main` -> `eval` -> `parse` -> `lex`.
#[test]
fn the_split_calculator_passes_the_same_tests_across_four_modules() {
    let out = heroes(&["test", "examples/calculator/main.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    assert!(shown.trim_end().ends_with("7 tests, all passed"), "{shown}");
    // One from each file that has any: `main`, `eval`, `lex`.
    assert!(shown.contains("ok   \"generics work across types\""), "{shown}");
    assert!(shown.contains("ok   \"precedence and parens\""), "{shown}");
    assert!(shown.contains("ok   \"tokenizes numbers, names and symbols\""), "{shown}");
    assert_eq!(code(&out), 0);
}

/// The split program runs, and prints what the single-file one prints. Two
/// spellings of one program is the claim M8a makes; this is the assertion.
#[test]
fn the_split_calculator_prints_what_the_single_file_one_prints() {
    let split = heroes(&["run", "examples/calculator/main.hero"]);
    let whole = heroes(&["run", "examples/calculator.hero"]);
    assert_eq!(
        String::from_utf8_lossy(&split.stdout),
        String::from_utf8_lossy(&whole.stdout),
        "the modules changed what the program does"
    );
    assert_eq!(code(&split), 0);
}

/// **fixedbugs, panel 032 D2, 2026-08-12.** Symptom: in the four-module
/// calculator the emitted `#line` values reached 410 while `main.hero` is 78
/// lines, and `lex.hero`, `parse.hero` and `eval.hero` appeared **zero times**
/// in the C. Cause: `at_span` chose between two entry points — the file the
/// reader named, and the library — a split that is right for two files and
/// wrong for four. Fix: one `at_file`, fed by `Source::locate`.
///
/// It is not cosmetic. §4.19's whole guarantee is that clang checks an `extern`
/// against the real header and reports it at the author's line; delivered to a
/// file that does not contain the declaration, the guarantee is noise.
#[test]
fn fixedbugs_every_module_names_itself_in_the_emitted_c() {
    let out = heroes(&["build", "examples/calculator/main.hero", "--emit-c"]);
    let c = String::from_utf8_lossy(&out.stdout).into_owned();
    for module in ["main", "lex", "parse", "eval"] {
        assert!(
            c.contains(&format!("examples/calculator/{module}.hero\"")),
            "no `#line` names {module}.hero"
        );
    }
    // And no line claims a file for a line it does not have: `main.hero` is
    // shorter than the concatenated text, which is how the defect showed.
    let main_lines = std::fs::read_to_string("../../examples/calculator/main.hero")
        .expect("the module is there")
        .lines()
        .count();
    for line in c.lines().filter(|l| l.contains("calculator/main.hero\"")) {
        let number: usize = line
            .split_whitespace()
            .nth(1)
            .and_then(|n| n.parse().ok())
            .expect("a `#line` carries a number");
        assert!(number <= main_lines, "{line} — main.hero has {main_lines} lines");
    }
    assert_eq!(code(&out), 0);
}

/// A failing test is the **program** being wrong: exit 1, the same code a
/// diagnostic gets and for the same reason — the tool worked.
///
/// And the failure carries what spec line 163 asks for: the source expression
/// *and both sides*. A bare "assert failed" loses the half that says what
/// happened.
#[test]
fn a_failing_test_shows_the_expression_and_both_sides() {
    let dir = std::path::Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../.."));
    let file = dir.join("build/surface-failing-test.hero");
    std::fs::create_dir_all(dir.join("build")).expect("build/ is writable");
    std::fs::write(
        &file,
        "function twice(n: int) -> int\n    return n * 2\n\n\
         test \"holds\"\n    assert twice(2) == 4\n\n\
         test \"fails\"\n    assert twice(2) == 5\n",
    )
    .expect("writable");
    let out = heroes(&["test", "build/surface-failing-test.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    let said = String::from_utf8_lossy(&out.stderr).into_owned();
    let _ = std::fs::remove_file(&file);

    assert!(shown.contains("ok   \"holds\""), "{shown}");
    assert!(shown.contains("FAIL \"fails\""), "{shown}");
    assert!(shown.contains("2 tests, 1 failed"), "{shown}");
    // The expression as the author wrote it, and both sides.
    assert!(said.contains("assert failed: twice(2) == 5"), "{said}");
    assert!(said.contains("left:  4"), "{said}");
    assert!(said.contains("right: 5"), "{said}");
    assert_eq!(code(&out), 1, "a failing test is the program being wrong");
}

/// One process per test, which is what makes the report complete: an `assert` is
/// a panic, so a runner that called them in sequence would stop at the first
/// failure and hide every test after it.
#[test]
fn a_failing_test_does_not_hide_the_ones_after_it() {
    let dir = std::path::Path::new(concat!(env!("CARGO_MANIFEST_DIR"), "/../.."));
    let file = dir.join("build/surface-order.hero");
    std::fs::create_dir_all(dir.join("build")).expect("build/ is writable");
    std::fs::write(
        &file,
        "test \"first, and it fails\"\n    assert 1 == 2\n\n\
         test \"second, and it must still run\"\n    assert 1 == 1\n",
    )
    .expect("writable");
    let out = heroes(&["test", "build/surface-order.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    let _ = std::fs::remove_file(&file);
    assert!(shown.contains("FAIL \"first, and it fails\""), "{shown}");
    assert!(shown.contains("ok   \"second, and it must still run\""), "{shown}");
}

/// `build` and `run` ignore `test` blocks (§4.18), and a file that is *only*
/// tests still builds — it just has no `main`, which `test` does not need.
#[test]
fn an_ordinary_build_ignores_test_blocks() {
    let out = heroes(&["build", "examples/calculator.hero", "--emit-c"]);
    let c = String::from_utf8_lossy(&out.stdout).into_owned();
    assert!(!c.contains("hero_panic_assert"), "a test block reached an ordinary build");
    assert_eq!(code(&out), 0);
}

/// **fixedbugs, 2026-08-12.** Symptom: the commonest possible mistake in the
/// language — writing a file with no `main` and asking to build it — answered
/// `internal error: a diagnostic landed inside the Heroes library, at its line
/// 86: [no_entry_point] …` at **exit 2**, the code that means *the compiler is
/// wrong*. Cause: the diagnostic's span was `src.text.len()-1 .. len`, and since
/// M6 appended the library the whole text ends **inside it**, so
/// `library::misplaced` swallowed the user's own error. Fix: `Source::root_end`.
///
/// Panel 020 settled the exit code by measurement: given 2, a judge checked the
/// compiler's version, ran `doctor`, grepped for the identifier and told its user
/// the toolchain was broken (`docs/panel/020-the-c-emitter.md`, `no_entry_point,
/// exit 1`). The row was written and then contradicted by a span, and nothing
/// tested it — `grep no_entry_point tests/` found nothing at all.
#[test]
fn fixedbugs_a_file_with_no_main_is_the_authors_error_not_the_compilers() {
    for verb in ["build", "run"] {
        let out = heroes(&[verb, "tests/golden/surface-fixtures/no-main.hero"]);
        let said = String::from_utf8_lossy(&out.stderr);
        assert_eq!(code(&out), 1, "{verb}: the input has a mistake — exit 1 (CLAUDE.md §10)\n{said}");
        assert!(said.contains("error[no_entry_point]"), "{verb}: {said}");
        assert!(said.contains("no-main.hero"), "{verb}: the caret names the author's file\n{said}");
        assert!(!said.contains("internal error"), "{verb}: the compiler must not blame itself\n{said}");
        assert!(!said.contains("library"), "{verb}: nothing here is about the library\n{said}");
    }
}

/// **fixedbugs, 2026-08-12.** Symptom: `check --json` reported a diagnostic in
/// `geom.hero` as `"file": "main.hero"` with a line number counted through the
/// concatenated text — the same defect the hole report had, in the one surface
/// whose consumer cannot notice by eye. Cause: `json()` assembled the triple from
/// `line_col` and `src.name` instead of `Source::locate`, which
/// `source/mod.rs` documents as mandatory: *"there is one function and no caller
/// assembles the triple itself."*
///
/// The generalisation, which is why this test asserts agreement rather than a
/// literal: `locate` can only protect the location it is *asked* for. The text
/// renderers had been fixed at M8a and this one was not, because the sweep went
/// through `Diagnostic`'s renderer and `--json` is a second one.
#[test]
fn fixedbugs_json_and_text_agree_about_which_file_a_diagnostic_is_in() {
    let root = "tests/golden/surface-fixtures/cross/main.hero";
    let text = heroes(&["check", root, "--brief"]);
    let json = heroes(&["check", root, "--json"]);
    let text = String::from_utf8_lossy(&text.stderr).to_string();
    // Both on stderr: `check` produces no artifact, so its diagnostics are the
    // whole output and `--json` says only *how* to print them (CLAUDE.md §10).
    let json = String::from_utf8_lossy(&json.stderr).to_string();
    assert!(text.contains("geom.hero:5:"), "the text form names the module: {text}");
    assert!(json.contains("\"file\": \"tests/golden/surface-fixtures/cross/geom.hero\"")
            || json.contains("\"file\": \"geom.hero\""),
            "the JSON form must name the same file:\n{json}");
    assert!(json.contains("\"line\": 5,"), "and the same line, counted in that file:\n{json}");
}

/// **fixedbugs, 2026-08-12.** `no binary: N holes in <file>` named the file on
/// the command line rather than the files the holes are in — since M8a not the
/// same thing, and a count attached to the wrong file sends the reader there.
#[test]
fn fixedbugs_the_hole_count_names_the_files_the_holes_are_in() {
    let out = heroes(&["build", "tests/golden/surface-fixtures/holes/main.hero"]);
    let said = String::from_utf8_lossy(&out.stderr);
    assert!(said.contains("no binary: 1 hole in "), "{said}");
    assert!(said.contains("geom.hero"), "the hole is in geom.hero: {said}");
    assert!(!said.trim_end().ends_with("main.hero"), "and not in main.hero: {said}");
    assert_eq!(code(&out), 1);
}
