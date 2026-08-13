//! `examples/` — the corpus, run.
//!
//! M-program-corpus is the one milestone whose deliverable is **programs rather
//! than compiler**, and this file is what makes that deliverable a test rather
//! than a folder. Every instrument this project owns reports on the programs it
//! is given: the M-generics-library audit found two live defects with *one*
//! program, each reachable for three milestones with no test, "because nobody had
//! written the program that meets them".
//!
//! What a program directory is, asked of the directory and not of a list: **any
//! directory under `examples/` that holds a `main.hero`**. `gallery/` therefore
//! excludes itself — it is a one-idea-per-file series, each file its own program,
//! held to its six properties by `printer::gallery` and its neighbours — and a new
//! program joins this harness by existing, which is the only arrangement where
//! adding one cannot be forgotten.
//!
//! Three properties per directory, and each is one thing that has gone wrong here
//! before:
//!
//! 1. **its `test` blocks pass in three configurations** — `-O0`, `-O2`,
//!    `--sanitize`. One corpus in more than one configuration is the cheapest
//!    multiplier in the record, and the leak gate rides along: every generated
//!    `main` ends in `hero_runtime_check_leaks()`, which on Darwin arm64 is the
//!    only leak instrument there is (ASan's does not exist there — measured,
//!    panel 021).
//! 2. **it prints what its `main.expected` says**, in the same three
//!    configurations, with `!exit:`/`!panic:` spelling how it ends. Arguments come
//!    from `main.args`, one per line, because §1.11's `args()` is only tested by a
//!    program that is given some.
//! 3. **it is named in `examples/README.md`**, which is the ROADMAP's "a README
//!    line saying what it demonstrates" made mechanical.
//!
//! Every path is relative to the workspace root and every process runs from it, so
//! a program that reads a file reads the same file in all three configurations —
//! `tests/golden/run/edges-file-args-exit.hero` learned that one the hard way.

use std::path::{Path, PathBuf};

mod expectation;
use expectation::{check, split_expectation};

fn workspace_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("workspace root exists")
        .to_path_buf()
}

/// Every program directory under `examples/`, sorted: the ones holding a
/// `main.hero`.
fn program_directories() -> Vec<PathBuf> {
    let examples = workspace_root().join("examples");
    let mut found: Vec<PathBuf> = std::fs::read_dir(&examples)
        .expect("examples/ must exist")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.join("main.hero").is_file())
        .collect();
    found.sort();
    assert!(
        found.len() >= 3,
        "the corpus lost program directories: {} under {}",
        found.len(),
        examples.display()
    );
    found
}

/// The path a command is given: relative to the workspace root, so the emitted
/// `#line` values and any diagnostic read the same on every machine.
fn relative(path: &Path) -> String {
    path.strip_prefix(workspace_root())
        .expect("under the workspace root")
        .display()
        .to_string()
}

fn heroes(root: &Path, args: &[String]) -> std::process::Output {
    std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
        .current_dir(root)
        .args(args)
        .output()
        .expect("the heroes binary runs")
}

/// `main.args`, one argument per line — or nothing, for a program that takes none.
fn arguments(dir: &Path) -> Vec<String> {
    let path = dir.join("main.args");
    let Ok(text) = std::fs::read_to_string(&path) else { return Vec::new() };
    text.lines().filter(|line| !line.is_empty()).map(|line| line.to_string()).collect()
}

/// **The acceptance criterion**: `heroes test` green over every directory in
/// `examples/`, in all three configurations.
///
/// A program with no `test` block is not a failure — `heroes test` says "no tests
/// in <path>" and exits 0, which is what the two FFI programs do. What stops that
/// from quietly becoming true of the whole corpus is the floor two tests below.
#[test]
fn every_program_directory_passes_its_tests_in_three_configurations() {
    let root = workspace_root();
    for dir in program_directories() {
        let main = relative(&dir.join("main.hero"));
        for level in ["-O0", "-O2", "--sanitize"] {
            let out = heroes(&root, &["test".to_string(), main.clone(), level.to_string()]);
            let said = String::from_utf8_lossy(&out.stdout);
            let noise = String::from_utf8_lossy(&out.stderr);
            assert_eq!(
                out.status.code(),
                Some(0),
                "{main} at {level}:\n{said}\n{noise}"
            );
            assert!(
                !said.contains("FAIL "),
                "{main} at {level} reports a failing test:\n{said}"
            );
            if level == "--sanitize" {
                assert!(
                    !noise.contains("AddressSanitizer") && !noise.contains("runtime error:"),
                    "{main} tripped a sanitiser:\n{noise}"
                );
            }
        }
    }
}

/// Every program asserts its own behaviour, unless what it does belongs to a C
/// library rather than to it.
///
/// The exemption is read off the program and not off a list of names: a directory
/// whose modules declare an `extern` group is bound to a live library, whose
/// version decides what the program prints, and `examples/curl/` prints this
/// machine's libcurl. Everything else owes `test` blocks — which is the ROADMAP's
/// "asserts its own behaviour in `test` blocks", asked of each directory.
#[test]
fn the_corpus_tests_itself() {
    let untested: Vec<String> = program_directories()
        .into_iter()
        .filter(|dir| {
            let modules = module_texts(dir);
            let tested = modules.iter().any(|text| declares(text, "test \""));
            let binds_c = modules.iter().any(|text| declares(text, "extern \""));
            !tested && !binds_c
        })
        .map(|dir| relative(&dir))
        .collect();
    assert!(
        untested.is_empty(),
        "these programs neither test themselves nor bind a C library: {untested:?}"
    );
}

/// Whether a module has a top-level declaration with this opening — a line that
/// starts at column 0, so the word inside a comment or a string does not count.
fn declares(text: &str, opening: &str) -> bool {
    text.lines().any(|line| line.starts_with(opening))
}

/// Every `.hero` file in a program directory — a program is its modules, and a
/// `test` block may live in any of them (`heroes test` runs them all).
fn module_texts(dir: &Path) -> Vec<String> {
    let mut texts: Vec<String> = std::fs::read_dir(dir)
        .expect("a readable program directory")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero"))
        .map(|path| std::fs::read_to_string(&path).expect("a readable module"))
        .collect();
    texts.sort();
    texts
}

/// Every program that carries a `main.expected` prints it, in the same three
/// configurations — and enough of them do that a lost expectation is visible.
#[test]
fn every_program_prints_what_it_promises_in_three_configurations() {
    let root = workspace_root();
    let scratch = root.join("build/corpus");
    std::fs::create_dir_all(&scratch).expect("a scratch directory");
    let mut checked = 0;
    for dir in program_directories() {
        let expected_path = dir.join("main.expected");
        if !expected_path.is_file() {
            continue;
        }
        checked += 1;
        let text = std::fs::read_to_string(&expected_path)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", expected_path.display()));
        let (expected, ending) = split_expectation(&text);
        let main = relative(&dir.join("main.hero"));
        let case = PathBuf::from(&main);
        let args = arguments(&dir);
        let name = dir.file_name().expect("a directory name").to_string_lossy().into_owned();

        // -O0: `build` writes a binary and the harness runs it, from the root.
        let binary = scratch.join(&name);
        let mut build = vec!["build".to_string(), main.clone(), "-o".to_string()];
        build.push(binary.display().to_string());
        let built = heroes(&root, &build);
        assert_eq!(
            built.status.code(),
            Some(0),
            "{main} did not build:\n{}",
            String::from_utf8_lossy(&built.stderr)
        );
        let ran = std::process::Command::new(&binary)
            .current_dir(&root)
            .args(&args)
            .output()
            .expect("the program runs");
        check(&case, "-O0", &ran, &expected, &ending);

        // -O2 and --sanitize: `run` compiles and executes, forwarding the status.
        // `--` separates the compiler's arguments from the program's.
        for level in ["-O2", "--sanitize"] {
            let mut invocation = vec!["run".to_string(), main.clone(), level.to_string()];
            if !args.is_empty() {
                invocation.push("--".to_string());
                invocation.extend(args.iter().cloned());
            }
            let out = heroes(&root, &invocation);
            if level == "--sanitize" {
                let noise = String::from_utf8_lossy(&out.stderr);
                assert!(
                    !noise.contains("AddressSanitizer") && !noise.contains("runtime error:"),
                    "{main} tripped a sanitiser:\n{noise}"
                );
            }
            check(&case, level, &out, &expected, &ending);
        }
    }
    assert!(checked >= 1, "no program directory carries a `main.expected`");
}

/// **At least one corpus program does not end in `exit(code)`** (author
/// instruction 2026-08-12), and this is that instruction, executable.
///
/// `exit` bypasses `hero_runtime_check_leaks()` by design — a program asking to
/// stop now is not asking for an audit, and running the gate there would report a
/// leak for every live value in every frame the exit unwinds past. The exemption
/// stands; what this asserts is that the corpus keeps a path that still **crosses**
/// the gate, because on Darwin arm64 that counter is the only leak instrument
/// there is.
#[test]
fn at_least_one_program_reaches_the_end_of_main_rather_than_exiting() {
    let crossing: Vec<String> = program_directories()
        .into_iter()
        .filter(|dir| !module_texts(dir).iter().any(|text| text.contains("exit(")))
        .map(|dir| relative(&dir))
        .collect();
    assert!(
        !crossing.is_empty(),
        "every corpus program calls `exit`, so nothing crosses the leak gate"
    );
}

/// The C a corpus program generates compiles with **no warnings at all** — with
/// one exemption that is read off the program and written to fire when it ends.
///
/// `tests/golden/run/` has been held to this since M-scalars-run and the corpus
/// was not, which is how a dead temporary reached a real program: `_ = f(x)?` on
/// a fallible left an `int64_t t72;` that nothing read, and clang said so on
/// every build of `examples/maze/` while every golden stayed silent.
///
/// **The exemption is `ptr`, and it is not a list of file names.** A program that
/// hands C an `@` parameter of type `ptr` emits `void **` where the header wants
/// `sqlite3 **`, and C converts `void *` to any object pointer implicitly but
/// does not convert `void **`. That warning is *true* and is forwarded rather
/// than silenced by an M-ffi-ladder decision recorded in
/// `examples/sqlite/main.hero`'s own header: a cast the emitter inserted would
/// compile clean and hide a real limit of `ptr`, and the repair is the C-width
/// type vocabulary §4.19 defers to Part 7 item 10.
///
/// So the assertion runs **both ways**. A program with no `ptr` out-parameter
/// must produce no warning at all; one that has them must produce warnings, and
/// every one of them must be that warning. When Part 7 item 10 lands and `ptr`
/// stops erasing the type, this test fails on the second half and the exemption
/// is deleted by whoever made it obsolete — which is what a premise with a test
/// under it looks like.
#[test]
fn the_corpus_generates_c_that_compiles_without_a_single_warning() {
    let root = workspace_root();
    for dir in program_directories() {
        let main = relative(&dir.join("main.hero"));
        let erases_a_type = module_texts(&dir).iter().any(|text| lends_a_ptr(text));
        // `build` is -O0 and `run` is -O2, so the two verbs are the two
        // configurations. clang's warnings reach stderr even on success.
        for verb in ["build", "run"] {
            let out = heroes(&root, &[verb.to_string(), main.clone()]);
            let noise = String::from_utf8_lossy(&out.stderr);
            let warnings: Vec<&str> = noise.lines().filter(|l| l.contains("warning:")).collect();
            if !erases_a_type {
                assert!(
                    warnings.is_empty(),
                    "{main} under `{verb}` produced clang warnings:\n{}",
                    warnings.join("\n")
                );
                continue;
            }
            assert!(
                !warnings.is_empty(),
                "{main} lends a `ptr` and no longer warns — if `ptr` stopped erasing \
                 the type, delete this exemption instead of widening it"
            );
            for one in &warnings {
                assert!(
                    one.contains("incompatible pointer types"),
                    "{main} under `{verb}` warns about something other than `ptr`:\n{one}"
                );
            }
        }
    }
}

/// Whether a module hands C the address of a `ptr` — an `@` parameter of that
/// type, which is what becomes `void **`.
fn lends_a_ptr(text: &str) -> bool {
    text.lines().any(|line| line.contains('@') && line.contains(": ptr"))
}

/// The corpus runs on **both** platforms, and the workflow says which two.
///
/// M-program-corpus's acceptance is the corpus green "in all three
/// configurations **and on both platforms**", and the three configurations are
/// asserted three tests above. The platforms cannot be asserted from inside one
/// of them — a test running on this machine can only ever report this machine —
/// so what is checked is the thing that decides: the workflow's matrix.
///
/// It is a weaker check than the others and is written down as such. It proves
/// the *intent* is still in the tree, not that the run was green; the run being
/// green is what the badge is for. What it catches is the failure this project
/// has already had once in another form — a rule that quietly stopped covering
/// something because a line was deleted while everything else stayed true.
#[test]
fn the_ci_covers_both_platforms() {
    let workflow = std::fs::read_to_string(workspace_root().join(".github/workflows/ci.yml"))
        .expect("the CI workflow must exist");
    for runner in ["ubuntu-latest", "macos-14"] {
        assert!(workflow.contains(runner), "the CI matrix lost `{runner}`");
    }
    assert!(
        workflow.contains("fail-fast: false"),
        "with `fail-fast`, one platform's failure hides the other's — which is the \
         one thing having two platforms was for"
    );
}

/// Every program directory is named in `examples/README.md`.
///
/// The ROADMAP asks each program for "a `README` line saying what it
/// demonstrates", and a documentation duty nothing checks is a documentation duty
/// that lasts one milestone.
#[test]
fn every_program_directory_is_documented() {
    let readme = std::fs::read_to_string(workspace_root().join("examples/README.md"))
        .expect("examples/README.md must exist");
    for dir in program_directories() {
        let name = dir.file_name().expect("a directory name").to_string_lossy().into_owned();
        assert!(
            readme.contains(&format!("{name}/")),
            "examples/README.md does not mention `{name}/`"
        );
    }
}
