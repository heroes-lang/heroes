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

/// Whether the compiler said this machine does not have the library, in which case
/// the test that follows is not a verdict about the compiler.
///
/// **The rule reads the compiler's own diagnostic, never a list of test names and
/// never `cfg!(windows)`** — that is `corpus.rs`'s rule, and it is here for the
/// same reason: a skip keyed on the platform goes on skipping after the library
/// arrives, and a skip keyed on a name stops being true when the name moves. This
/// one is keyed on the fact in hand — what this invocation printed (CLAUDE.md §11).
///
/// The Windows CI leg is where it earns its keep: that image has clang and no
/// `sqlite3.h`, no `curl/curl.h`. On a machine that *has* them, nothing is skipped
/// and every assertion below still runs.
fn machine_lacks_the_library(out: &Output) -> bool {
    let said = String::from_utf8_lossy(&out.stderr);
    said.contains("error[ffi_missing_header]") || said.contains("error[ffi_package]")
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
    // The gloss on 1 widened at M-scalars-run: an unsupported form is a diagnostic about the
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

// --- M-rich-diagnostics: diagnostics as a product -----------------------------------

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
    // **The separator is the platform's, and that is the point of the line.** The
    // reader is being handed a path to type into *this* machine's shell, and
    // `cmd.exe` reads a leading `/` as a switch. So the assertion pins the shape —
    // `build/<hash>/<stem>` — and normalises the separator, rather than pinning
    // bytes that would make the message wrong for the reader it is written for.
    // Contrast a module's *name* in a diagnostic, which keeps the spelling the
    // author wrote (`modules::directory_text`): that one is matched against the
    // author's own text, this one is typed at a prompt.
    let shape = said.replace('\\', "/");
    assert!(shape.starts_with("wrote build/"), "{said}");
    assert!(shape.trim_end().trim_end_matches(".exe").ends_with("/00first"), "{said}");
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
    // worked, and no edit to the file will help. This has now named five different
    // files — `01-points.hero` until records emitted, `10-maps.hero` until `.must()`
    // did, `06-generics.hero` until monomorphisation did, `08-ffi.hero` until the
    // `#include` arrived at M-ffi-ladder — which is the assertion working rather than
    // breaking: a row dies per step and the case follows it. **No gallery program
    // is refused any more**, so it names the golden that exists to hold the last
    // two rows.
    assert_eq!(code(&heroes(&["build", "tests/golden/unsupported/three-capabilities.hero"])), 1);
    assert_eq!(code(&heroes(&["build", "no/such/file.hero"])), 2);
    assert_eq!(code(&heroes(&["build", "--dump-ast", "examples/gallery/00-first.hero"])), 2);
}

/// `run` stops being a retired spelling and becomes the dev loop (M-scalars-run). It compiles
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
/// The runtime became eleven files at M-generics-library step 3, assembled into one translation
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
    let out = heroes(&["test", "examples/calculator/whole.hero"]);
    let shown = String::from_utf8_lossy(&out.stdout).into_owned();
    // The report is the artifact, so it is on stdout (§10's contract).
    assert!(shown.contains("ok   \"precedence and parens\""), "{shown}");
    assert!(shown.trim_end().ends_with("7 tests, all passed"), "{shown}");
    assert_eq!(code(&out), 0);
}

/// **M-module-namespace's acceptance criterion**: the same program, cut into four modules, and
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
/// spellings of one program is the claim M-module-namespace makes; this is the assertion.
#[test]
fn the_split_calculator_prints_what_the_single_file_one_prints() {
    let split = heroes(&["run", "examples/calculator/main.hero"]);
    let whole = heroes(&["run", "examples/calculator/whole.hero"]);
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
        "function twice(n: i64) -> i64\n    return n * 2\n\n\
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
    let out = heroes(&["build", "examples/calculator/whole.hero", "--emit-c"]);
    let c = String::from_utf8_lossy(&out.stdout).into_owned();
    assert!(!c.contains("hero_panic_assert"), "a test block reached an ordinary build");
    assert_eq!(code(&out), 0);
}

/// **fixedbugs, 2026-08-12.** Symptom: the commonest possible mistake in the
/// language — writing a file with no `main` and asking to build it — answered
/// `internal error: a diagnostic landed inside the Heroes library, at its line
/// 86: [no_entry_point] …` at **exit 2**, the code that means *the compiler is
/// wrong*. Cause: the diagnostic's span was `src.text.len()-1 .. len`, and since
/// M-generics-library appended the library the whole text ends **inside it**, so
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
/// renderers had been fixed at M-module-namespace and this one was not, because the sweep went
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
/// the command line rather than the files the holes are in — since M-module-namespace not the
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

/// **fixedbugs, sweep 001 N4, 2026-08-12.** Symptom: `assertion failed:
/// self.is_char_boundary(n)`, **exit 101** — a code CLAUDE.md §10 does not have
/// — on any multi-module program whose certain fix is outside the root. Cause: a
/// fix's span is an offset into the whole compilation and `--apply` writes back
/// the root file alone, so `replace_range` indexed past the end of the string.
///
/// It matters more than an ordinary panic: `--apply` is what CI uses to assert
/// that a `.fixed` golden checks clean, so the one flag whose job is to prove
/// fixes work could not be pointed at a program with more than one file.
///
/// The repair says out loud what it skipped. Dropping those fixes silently would
/// be worse than the panic — the flag's contract is *every* certain fix.
#[test]
fn fixedbugs_apply_does_not_panic_on_a_fix_in_another_module() {
    let out = heroes(&["check", "tests/golden/surface-fixtures/applyx/main.hero", "--apply"]);
    let said = String::from_utf8_lossy(&out.stderr);
    let program = String::from_utf8_lossy(&out.stdout);
    assert_eq!(code(&out), 0, "{said}");
    assert!(said.contains("another module"), "it says what it skipped: {said}");
    assert!(said.contains("geom.hero"), "and names it: {said}");
    assert!(program.contains("use geom"), "the root file comes back whole: {program}");
    assert!(!program.contains("function area"), "and only the root file: {program}");
}

/// **fixedbugs, sweep 001 N6, 2026-08-12.** `lex --dump-tokens` dumped the whole
/// compilation — an eight-line program printed 470 lines, the appended library's
/// tokens included, at concatenated line numbers with no file marker. Its two
/// siblings filter correctly, and `printer/scopes.rs` states the rule in words:
/// a `--dump-<stage>` answers "what does the compiler know about **the file I
/// named**" (CLAUDE.md §10).
#[test]
fn fixedbugs_dump_tokens_shows_the_file_that_was_named() {
    let out = heroes(&["lex", "examples/gallery/00-first.hero", "--dump-tokens"]);
    let shown = String::from_utf8_lossy(&out.stdout);
    let lines = shown.lines().count();
    assert!(lines < 40, "an 8-line program should not print {lines} token lines");
    assert!(!shown.contains("range"), "the library is not this file: {lines} lines");
    // Its own lines, not the concatenated text's.
    assert!(shown.starts_with("1:1 "), "{shown}");
    assert_eq!(code(&out), 0);
}

/// **A clang failure that is the author's, not the compiler's** — the one named
/// exception to CLAUDE.md §7's rule that a clang failure exits 2 and blames the
/// compiler (§4.19, panel 036 rider 3).
///
/// The return-type assertion is generated code whose whole purpose is to fail
/// when the *author's* declaration disagrees with the real header. Left as an
/// internal error it printed C the author never wrote, named a file under
/// `build/<hash>/`, and blamed the compiler — so this pins all three of the
/// things that make it a diagnostic instead: exit 1, the `.hero` line, and no
/// generated C anywhere in the message.
#[test]
fn a_wrong_extern_return_type_is_the_authors_error_not_the_compilers() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-return-type.hero"]);
    assert_eq!(code(&out), 1, "exit 1: the input has diagnostics");
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert!(stderr.contains("error[ffi_return_type]"), "{stderr}");
    assert!(stderr.contains("does not return `i64`"), "{stderr}");
    assert!(stderr.contains("math.h"), "{stderr}");
    assert!(!stderr.contains("internal error"), "it blamed the compiler:\n{stderr}");
    assert!(!stderr.contains("_Static_assert"), "it showed generated C:\n{stderr}");
    assert!(!stderr.contains("_Generic"), "it showed generated C:\n{stderr}");
}

/// **A pointer return was the one case that could not reach its own diagnostic**
/// (panel 042, 2026-08-12). `extern function getenv(name: cstr) -> i64` is an
/// ordinary mistake — `getenv` returns `char *` — and it answered `internal
/// error … invalid argument type 'char *' to unary expression` at exit 2, which
/// CLAUDE.md §7 makes a claim that the *compiler* is wrong.
///
/// The assertion was written `_Generic(+(c), …)`, and `+` on a pointer is a hard
/// clang error rather than a failed assertion, so the marker `emit/ffi.rs` looks
/// for was never produced and §7's named exception could not fire. This pins the
/// repair from both ends: the diagnostic appears, and the words that mean the
/// compiler blamed itself do not.
#[test]
fn a_wrong_pointer_return_is_a_diagnostic_not_an_internal_error() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-pointer-return.hero"]);
    assert_eq!(code(&out), 1, "exit 1: the input has diagnostics");
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert!(stderr.contains("error[ffi_return_type]"), "{stderr}");
    assert!(stderr.contains("getenv"), "{stderr}");
    assert!(stderr.contains("stdlib.h"), "{stderr}");
    assert!(!stderr.contains("internal error"), "it blamed the compiler:\n{stderr}");
    assert!(!stderr.contains("unary expression"), "clang's error leaked:\n{stderr}");
    assert!(!stderr.contains("_Generic"), "it showed generated C:\n{stderr}");
}

/// **The premise that put the `+` there, as a claim that can die loudly.**
///
/// `emit/externs.rs` recorded that without a unary `+` the return assertion
/// *"refused every enum-returning C function in existence"*. Panel 042 measured
/// that this is false: the accepted set was widened in the same commit to include
/// `unsigned int` and its narrower siblings, and C11 6.5.1.1 selects an enum's
/// **compatible integer type**, which that set now contains. The `+` was carrying
/// a justification for work it does not do, while making a pointer return
/// unreachable by its own diagnostic.
///
/// CLAUDE.md §11 owes a premise a test that fires when it dies, whose failure
/// message names what depends on it. This is that test: `examples/curl` binds
/// `curl_easy_setopt` and `curl_easy_perform`, both of which return `CURLcode`,
/// an enum. If a future clang stops selecting a compatible integer type for an
/// enum, this goes red **here** rather than as a mystery in the ladder.
#[test]
fn an_enum_returning_extern_needs_no_unary_plus() {
    let out = heroes(&["build", "examples/curl/main.hero"]);
    if machine_lacks_the_library(&out) {
        return;
    }
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert_eq!(
        code(&out),
        0,
        "an enum-returning `extern` must pass HERO_RET_INT with no unary `+`. \
         If this is red, C11 6.5.1.1's compatible-integer-type selection no longer \
         covers this enum, and `emit/externs.rs`'s widened set is what depends on \
         it — `tests/golden/fixedbugs/ffi-pointer-return.hero` is why the `+` \
         cannot simply come back.\n{stderr}"
    );
}

/// **The spec's own FFI example, run.** Nothing in this project compiled the
/// specification's examples, and that is how the most-copied FFI line in the
/// document came to contradict the rule four lines below it: it declared
/// `sqlite3_open(path: cstr, out: ptr)` while the prose said *"a C out-parameter is
/// an `@` parameter"*.
///
/// The consequence was not a crash. Copied verbatim, that example compiled clean,
/// linked, ran and **exited 0** printing `rc: 21` — `SQLITE_MISUSE`: the address
/// of the handle was never passed, so no database was opened and the program
/// reported success to the shell. A silent error inside the specification.
///
/// Found by panel 038's llm-ergonomist, which is given the document and nothing
/// else, while it was being asked about constants. This is the instrument that
/// would have found it without a judge: the example is read **out of the real spec
/// file at test time**, so it cannot drift from what a reader copies.
#[test]
fn the_specs_own_ffi_example_opens_a_database() {
    let spec = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../spec/heroes-spec.md"
    ))
    .expect("the spec is in the repo");
    // The fenced block under `## FFI`, which is the one a reader copies.
    let ffi = spec.split("## FFI").nth(1).expect("the spec has an FFI section");
    let block = ffi.split("```").nth(1).expect("the FFI section shows a group");
    assert!(block.contains("extern \"sqlite3.h\""), "the example moved: {block}");

    // The example plus the smallest program that exercises what it declares. If
    // the out-parameter is not an `@`, `db` stays `nullptr`, sqlite3_open returns
    // SQLITE_MISUSE and this prints the wrong number — which is the whole point.
    let program = format!(
        "{block}\nfunction main()\n    db: ptr @ nullptr\n    print(sqlite3_open(\":memory:\".cstr(), @db))\n    _ = sqlite3_close(db)\n"
    );
    let dir = std::env::temp_dir().join("heroes-spec-ffi");
    std::fs::create_dir_all(&dir).expect("a writable temp dir");
    let path = dir.join("spec-ffi.hero");
    std::fs::write(&path, program).expect("the program is written");

    let out = heroes(&["run", &path.display().to_string()]);
    if machine_lacks_the_library(&out) {
        return;
    }
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert_eq!(code(&out), 0, "the spec's own example must compile and run:\n{stderr}");
    // `0` is `SQLITE_OK`. `21` is `SQLITE_MISUSE`, which is what a missing `@`
    // produces: it opens nothing and says so only through a number.
    assert_eq!(String::from_utf8_lossy(&out.stdout), "0\n", "stderr:\n{stderr}");
}

/// An `extern constant` whose declared type the header refutes. The same
/// `_Generic` as a signature's, asked of a token instead of a call — and a
/// different code, because a function *returns* the wrong type and a constant
/// **is** one (panel 038).
///
/// The silent route this closes was measured before the form existed: `int64_t
/// f(void) { return M_PI; }` compiles clean under the project's flags and yields
/// **3**.
#[test]
fn a_wrong_extern_constant_type_is_the_authors_error_not_the_compilers() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-constant-type.hero"]);
    assert_eq!(code(&out), 1, "exit 1: the input has diagnostics");
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert!(stderr.contains("error[ffi_constant_type]"), "{stderr}");
    assert!(stderr.contains("`M_PI` is not `i64`"), "{stderr}");
    assert!(stderr.contains("math.h"), "{stderr}");
    // Not the function's diagnostic: a constant does not "return" anything.
    assert!(!stderr.contains("does not return"), "{stderr}");
    assert!(!stderr.contains("internal error"), "it blamed the compiler:\n{stderr}");
    assert!(!stderr.contains("_Static_assert"), "it showed generated C:\n{stderr}");
}

/// A C **object** named as a constant. The refusal that no other language makes as
/// a rule, and the one the historian predicted Heroes would need or else compile
/// `stdout` at exit 0 in silence (panel 038).
#[test]
fn a_c_object_is_not_a_constant() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-not-constant.hero"]);
    assert_eq!(code(&out), 1, "exit 1: the input has diagnostics");
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert!(stderr.contains("error[ffi_not_constant]"), "{stderr}");
    assert!(stderr.contains("has `stdout`, but not as a constant"), "{stderr}");
    // The note must carry *why* it is refused, not just that it is: the rule being
    // enforced is §4.2's ban on mutable globals (§4.17).
    assert!(stderr.contains("reading it twice could give two answers"), "{stderr}");
    assert!(!stderr.contains("internal error"), "it blamed the compiler:\n{stderr}");
    assert!(!stderr.contains("__builtin_constant_p"), "it showed generated C:\n{stderr}");
}

/// The other half of §7's named exception: a name the header does not have is
/// **a different mistake** from a result type it refutes, and was being reported
/// as that one (panel 038, rider A1).
///
/// This pins what the old message got wrong: the code, the fact that no text
/// from clang's echo of the source line reaches the message, and the repair —
/// clang's own typo correction, carried as a `guess` so `--apply` leaves it
/// alone (CLAUDE.md §8).
#[test]
fn a_misspelled_extern_name_says_the_header_has_no_such_name() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-unknown-name.hero"]);
    if machine_lacks_the_library(&out) {
        return;
    }
    assert_eq!(code(&out), 1, "exit 1: the input has diagnostics");
    let stderr = String::from_utf8_lossy(&out.stderr);
    assert!(stderr.contains("error[ffi_unknown_name]"), "{stderr}");
    assert!(stderr.contains("declares no `sqlite3_openn`"), "{stderr}");
    assert!(stderr.contains("sqlite3.h"), "{stderr}");
    // clang knows what was meant, and the message says so without promising it.
    assert!(stderr.contains("fix (guess)"), "{stderr}");
    assert!(stderr.contains("`sqlite3_open`"), "{stderr}");
    // The defect itself: no fragment of the echoed source line may appear.
    assert!(!stderr.contains("ffi_return_type"), "the wrong diagnosis is back:\n{stderr}");
    assert!(!stderr.contains("int\");"), "clang's echo reached the message:\n{stderr}");
    assert!(!stderr.contains("internal error"), "it blamed the compiler:\n{stderr}");
    assert!(!stderr.contains("_Static_assert"), "it showed generated C:\n{stderr}");
}

/// **The milestone's acceptance test, run** (design.md §4.19's ladder, rung 3):
/// *if this works without you having written a standard library, the
/// architecture holds*.
///
/// Open a database, create a table, insert, prepare, step, read a column,
/// finalize, close — from Heroes, against the SDK's own `sqlite3.h`, with **no
/// shim**. It is a surface test rather than a `run/` golden because it needs a
/// third-party library on the link line, which is the one thing the golden
/// harness cannot assume on a machine it has not met.
#[test]
fn sqlite_opens_queries_and_closes_with_no_shim() {
    let out = heroes(&["run", "examples/sqlite/main.hero"]);
    if machine_lacks_the_library(&out) {
        return;
    }
    assert_eq!(code(&out), 0, "{}", String::from_utf8_lossy(&out.stderr));
    let stdout = String::from_utf8_lossy(&out.stdout);
    assert_eq!(stdout, "rows: 3\nlongest: 6\n", "{stdout}");
}

/// **The defect the ROADMAP called "the second page of any tour", closed.**
///
/// A Heroes program could not tell the shell it had failed: `main` may not
/// declare a result — the checker refuses it, because a fallible `main` returning
/// `fail(…)` printed nothing and exited **0** — so every program said it had
/// succeeded. `exit(code)` is the answer, and it is written in Heroes over a
/// `_Noreturn` C function (§1.11 Tier 2, M-ffi-ladder).
///
/// The `run/` golden asserts the output and stops there, because that harness
/// compares stdout. This asserts the half a shell actually reads.
#[test]
fn exit_forwards_the_programs_own_status() {
    let out = heroes(&["run", "tests/golden/run/exit-status.hero"]);
    assert_eq!(code(&out), 3, "{}", String::from_utf8_lossy(&out.stderr));
    assert_eq!(String::from_utf8_lossy(&out.stdout), "before\n");
}

/// **The ladder's fourth rung** (author instruction, 2026-08-12): a variadic and
/// an enum return, which SQLite has neither of.
///
/// It asserts a prefix rather than the whole line, because `curl_version()`
/// prints this machine's libcurl and that is not the compiler's business. What
/// it pins is that the variadic call reached the library and the library
/// answered — `curl_easy_strerror` maps a `CURLcode` the program never provoked
/// to a message no table here contains.
#[test]
fn libcurl_takes_a_variadic_and_returns_an_enum() {
    let out = heroes(&["run", "examples/curl/main.hero"]);
    if machine_lacks_the_library(&out) {
        return;
    }
    assert_eq!(code(&out), 0, "{}", String::from_utf8_lossy(&out.stderr));
    let stdout = String::from_utf8_lossy(&out.stdout);
    assert!(stdout.starts_with("libcurl: libcurl/"), "{stdout}");
    assert!(stdout.contains("the url was accepted"), "{stdout}");
    assert!(stdout.contains("code 1 means: Unsupported protocol"), "{stdout}");
}

/// `heroes run f.hero -- a b` — a separator, not a trailing operand (author
/// instruction 2026-08-12).
///
/// The alternative is unreadable in the case that matters: `heroes run f.hero
/// --sanitize` would have to mean the flag and `heroes run f.hero -o x` the
/// flag's value, so a program taking `-o` could never be run at all. And `--`
/// anywhere but `run` is a named refusal rather than a silent no-op, because
/// nothing else executes a program to forward to.
#[test]
fn run_passes_arguments_to_the_program_after_a_separator() {
    let out = heroes(&["run", "tests/golden/run/edges-file-args-exit.hero", "--", "alpha", "beta"]);
    assert_eq!(code(&out), 0, "{}", String::from_utf8_lossy(&out.stderr));
    let stdout = String::from_utf8_lossy(&out.stdout);
    assert!(stdout.contains("arguments: 2"), "{stdout}");

    let refused = heroes(&["check", "examples/gallery/00-first.hero", "--", "x"]);
    assert_eq!(code(&refused), 2);
    let said = String::from_utf8_lossy(&refused.stderr);
    assert!(said.contains("only `heroes run` does"), "{said}");
}

/// `heroes mutate` refuses a corpus that does not compile (author decision
/// 2026-08-12, panel 038 § The digit class).
///
/// This is metric 3's one number that can be wrong in the **flattering**
/// direction: every mutant of a program that is already refused is killed by the
/// diagnostic that was already there, so the run reports a perfect defence it
/// never mounted. The witness is the one that found it — `crates/heroes/src/library/`
/// is refused by `heroes check` because the library *is* the built-ins, and it
/// used to score 100%.
#[test]
fn mutate_refuses_a_corpus_that_does_not_compile() {
    let out = heroes(&["mutate", "crates/heroes/src/library/"]);
    assert_eq!(code(&out), 2, "a corpus it cannot use is exit 2, not a quiet exclusion");
    assert!(out.stdout.is_empty(), "no table when the corpus is refused");
    let said = String::from_utf8_lossy(&out.stderr);
    assert!(said.contains("this corpus does not compile"), "{said}");
    assert!(said.contains("library/source.hero"), "it names the file: {said}");

    // And the corpus that does compile still runs.
    assert_eq!(code(&heroes(&["mutate", "examples/gallery"])), 0);
}

/// `--survivors` prints the mutants behind a rate, and `--operator` narrows to one.
///
/// Measurement 002 recorded two per-operator rates that moved between runs and
/// declined to explain either, because attributing a delta needs the mutants and
/// the tool printed rates alone (CLAUDE.md §12: a number nobody can attribute is
/// an opinion with a decimal point).
#[test]
fn mutate_can_print_the_survivors_of_one_operator() {
    let out = heroes(&["mutate", "examples/gallery", "--operator", "swap-args", "--survivors"]);
    assert_eq!(code(&out), 0, "{}", String::from_utf8_lossy(&out.stderr));
    let stdout = String::from_utf8_lossy(&out.stdout);
    assert!(stdout.contains("| swap-args |"), "the table is still the measurement: {stdout}");
    assert!(!stdout.contains("| typo-ident |"), "--operator scores one row: {stdout}");
    // A survivor is printed as its site and the two lines, so it can be opened.
    let survivors = stdout.split("Never pool these").nth(1).expect("the survivor section");
    assert!(
        survivors.contains(".hero:") && survivors.contains("    - ") && survivors.contains("    + "),
        "a survivor names file:line and both lines: {survivors}"
    );

    // An unknown operator is exit 2 and names the twelve, like every other
    // strictness failure on this surface.
    let wrong = heroes(&["mutate", "--operator", "swapargs"]);
    assert_eq!(code(&wrong), 2);
    let said = String::from_utf8_lossy(&wrong.stderr);
    assert!(said.contains("no operator `swapargs`") && said.contains("swap-args"), "{said}");
}

/// The optimisation level is a flag as well as a verb's default (author decision
/// 2026-08-12, closing panel 021's asymmetry).
///
/// Panel 021 recorded it as visible in `--help`: sanitising was a flag and the
/// level was a subcommand, so `build` could not be asked for the level the `run/`
/// golden harness runs every case at. Two levels and no others — CLAUDE.md §10's
/// stopping rule applied to a flag: the harness types `-O0` and `-O2`, and nothing
/// types `-O1`.
#[test]
fn the_optimisation_level_is_a_flag_with_the_verbs_default() {
    // Both verbs reach both levels, and the cache key separates them: `level` is
    // in the digest, so the two binaries are two directories.
    let at_two = heroes(&["build", "examples/gallery/00-first.hero", "-O2"]);
    assert_eq!(code(&at_two), 0, "{}", String::from_utf8_lossy(&at_two.stderr));
    let at_zero = heroes(&["build", "examples/gallery/00-first.hero", "-O0"]);
    assert_eq!(code(&at_zero), 0);
    assert_ne!(
        String::from_utf8_lossy(&at_two.stderr),
        String::from_utf8_lossy(&at_zero.stderr),
        "two levels are two build directories, not one"
    );
    assert_eq!(code(&heroes(&["run", "examples/gallery/00-first.hero", "-O0"])), 0);

    // Both at once is refused rather than resolved by precedence — the reading
    // `build` already gives `--dump-ir --emit-c`.
    let both = heroes(&["build", "examples/gallery/00-first.hero", "-O0", "-O2"]);
    assert_eq!(code(&both), 2);
    let said = String::from_utf8_lossy(&both.stderr);
    assert!(said.contains("one level at a time"), "{said}");

    // A level the harness does not need is not on the surface, and the strictness
    // error names what is.
    let unknown = heroes(&["build", "examples/gallery/00-first.hero", "-O1"]);
    assert_eq!(code(&unknown), 2);
    let names = String::from_utf8_lossy(&unknown.stderr);
    assert!(names.contains("-O0, -O2"), "{names}");
}

/// **The fourth class of §7's named exception, and the first that is the
/// *linker*'s** (panel 048, 2026-08-14). A correct binding missing only its
/// `link` clause used to be `internal error:` at exit **2** — the compiler
/// blaming itself for a mistake in a `.hero` file, with no line, no span, and a
/// pointer at generated C under `build/<hash>/`.
///
/// The gate is what makes this safe and it is asserted below: the class is
/// admitted only for a symbol *this program* declared `extern`. An undefined
/// symbol nobody declared is the emitter's own bug and stays exit 2.
#[test]
fn fixedbugs_a_missing_link_is_the_authors_error_not_the_compilers() {
    let out = heroes(&["build", "tests/golden/fixedbugs/ffi-missing-link.hero"]);
    if machine_lacks_the_library(&out) {
        return;
    }
    let said = String::from_utf8_lossy(&out.stderr).into_owned();
    // Exit 1: the input has a diagnostic. 2 would say the tool could not run.
    assert_eq!(code(&out), 1, "{said}");
    assert!(said.contains("error[ffi_missing_link]"), "{said}");
    // The author's line, not the generated C's.
    assert!(said.contains("ffi-missing-link.hero:"), "{said}");
    assert!(!said.contains("internal error"), "{said}");
    assert!(!said.contains("build/"), "the message sends the reader to generated C: {said}");
    // And it says what to write, which is the whole of §4.17 here.
    assert!(said.contains("link \"<library>\""), "{said}");
    // The repair works: the same program with the library named builds and runs.
    let source = std::fs::read_to_string("../../tests/golden/fixedbugs/ffi-missing-link.hero")
        .expect("the case is there");
    let repaired =
        source.replace("extern \"sqlite3.h\"", "extern \"sqlite3.h\" link \"sqlite3\"");
    let path = std::env::temp_dir().join("heroes-ffi-missing-link-fixed.hero");
    std::fs::write(&path, repaired).expect("a writable temp file");
    let fixed = heroes(&["run", &path.display().to_string()]);
    assert_eq!(code(&fixed), 0, "{}", String::from_utf8_lossy(&fixed.stderr));
    assert!(String::from_utf8_lossy(&fixed.stdout).starts_with("sqlite "), "{}", String::from_utf8_lossy(&fixed.stdout));
}
