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
    assert_eq!(code(&heroes(&["check", "--nonsense", "examples/first.hero"])), 2);
    assert_eq!(code(&heroes(&["frobnicate"])), 2);
}

/// The artifact goes to stdout, the diagnostics to stderr. A wrapper that greps
/// stdout for errors would otherwise find nothing and conclude success.
#[test]
fn the_artifact_is_on_stdout_and_diagnostics_are_on_stderr() {
    let clean = heroes(&["parse", "examples/first.hero", "--dump-ast"]);
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
    let out = heroes(&["check", "--dump-ast", "examples/first.hero"]);
    let message = String::from_utf8_lossy(&out.stderr).into_owned();
    assert_eq!(
        message,
        "error: `check` does not accept `--dump-ast` — it accepts --dump-scopes\n"
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
    let out = heroes(&["fmt", "--write", "examples/first.hero"]);
    assert_eq!(
        String::from_utf8_lossy(&out.stderr).into_owned(),
        "error: `--write` is no longer a flag of `fmt` — use `--in-place`\n"
    );
    assert_eq!(code(&out), 2);
    // …and the file was not touched, which is the point of the rename.
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../examples/first.hero"
    ))
    .expect("first.hero exists");
    assert!(text.contains("print((2 + 3) * 4)"));
}

/// Every stage prints only when asked. Before panel 016, `lex` printed its
/// tokens by default while `parse` and `check` did not, so "which stages print"
/// was a table to memorise.
#[test]
fn a_stage_prints_only_when_asked() {
    let quiet = heroes(&["lex", "examples/first.hero"]);
    assert!(quiet.stdout.is_empty(), "`lex` alone is a lexical check");
    assert_eq!(code(&quiet), 0);

    let text = heroes(&["lex", "examples/first.hero", "--dump-tokens"]);
    assert!(String::from_utf8_lossy(&text.stdout).contains("ident main"));

    let json = heroes(&["lex", "examples/first.hero", "--dump-tokens", "--json"]);
    let shown = String::from_utf8_lossy(&json.stdout);
    assert!(shown.starts_with("[\n"), "--json changes how, not what: {shown}");
    assert!(shown.contains("\"kind\""));
}

/// `--json` on its own prints nothing: it says *how*, and `--dump-tokens` says
/// *what*. Two flags because they are two questions.
#[test]
fn json_alone_selects_no_output() {
    let out = heroes(&["lex", "examples/first.hero", "--json"]);
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
    assert!(shown.contains("exit:    0 nothing to report · 1 the input has diagnostics · 2 the tool could not run."));
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
