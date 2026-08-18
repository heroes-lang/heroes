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
//! 1. **its `test` blocks pass in every configuration this platform can run** —
//!    `-O0`, `-O2` and `--sanitize`, except on Windows, whose clang ships no
//!    sanitiser runtime (`configurations()` says so once, and why). One corpus in
//!    more than one configuration is the cheapest
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
    slashed(path.strip_prefix(workspace_root()).expect("under the workspace root"))
}

/// Whether this machine simply does not have what a program binds.
///
/// **A fact about the machine, asked of the compiler rather than assumed from a
/// list of program names.** `ffi_package` and `ffi_missing_header` are the two
/// diagnostics that mean *the library you named is not installed here* — they are
/// the author's problem on their own machine and nobody's problem on a CI runner
/// that was never given raylib. Any other failure is a real one.
///
/// The count is floored below, so a machine that starts skipping the whole corpus
/// says so instead of passing.
fn machine_lacks_the_library(out: &std::process::Output) -> bool {
    let said = String::from_utf8_lossy(&out.stderr);
    said.contains("error[ffi_package]") || said.contains("error[ffi_missing_header]")
}

/// Whether this platform's clang ships the sanitiser runtimes.
///
/// **Windows does not**, and the failure is silent in the worst way: the binary
/// links, then exits **53** with nothing on either stream, because the process
/// cannot start without `clang_rt.asan_dynamic-x86_64.dll` — and UBSan has no
/// MSVC-target runtime at all. Measured on the third CI leg, 2026-08-14.
///
/// So `--sanitize` is not run there, and this function is where that is said
/// once. It is a fact about the toolchain rather than about the programs, and it
/// is the reason `every_program_directory_passes_its_tests_in_three_configurations`
/// runs **two** configurations on Windows and three everywhere else — which the
/// name would otherwise quietly stop being true of.
fn configurations() -> &'static [&'static str] {
    if cfg!(target_os = "windows") { &["-O0", "-O2"] } else { &["-O0", "-O2", "--sanitize"] }
}

/// How long a corpus program may take before the harness stops waiting.
///
/// **A corpus program can hang, and a hang is worse than a failure here**
/// (2026-08-14, found by the author looking at their own screen). An
/// `examples/sdl/` binding linked SDL2 — whose `SDL2main` replaces the program's
/// `main` with a Cocoa one, and whose `sdl2-compat` shim opens a **modal dialog**
/// when it cannot load SDL3 — so two processes sat at 0% CPU for ninety minutes
/// waiting for a click nobody could give them. This harness runs unattended, on
/// three platforms, in three configurations each: a program that never returns
/// burns the whole job rather than one case.
///
/// Two minutes is chosen against the slowest thing the corpus actually does — a
/// `--sanitize` build of the largest example, seconds rather than minutes — so it
/// is not a performance budget in disguise (CLAUDE.md §13). It is the line between
/// *slow* and *never*, and a program that crosses it has stopped, not slowed.
const PATIENCE: std::time::Duration = std::time::Duration::from_secs(120);

/// Run `heroes`, and refuse to wait forever.
///
/// The wait is a poll rather than a platform timeout, because the three CI legs
/// have three different ways of spelling one, and this file must read the same on
/// all of them.
fn heroes(root: &Path, args: &[String]) -> std::process::Output {
    let mut child = std::process::Command::new(env!("CARGO_BIN_EXE_heroes"))
        .current_dir(root)
        .args(args)
        .stdout(std::process::Stdio::piped())
        .stderr(std::process::Stdio::piped())
        .spawn()
        .expect("the heroes binary runs");
    let began = std::time::Instant::now();
    loop {
        match child.try_wait().expect("the child can be waited on") {
            Some(_) => break,
            None if began.elapsed() > PATIENCE => {
                let _ = child.kill();
                let _ = child.wait();
                panic!(
                    "`heroes {}` did not finish in {} seconds — it is hung, not slow, \
                     and a corpus program that never returns burns the whole job. \
                     If the program binds a C library, suspect the library: it may be \
                     waiting on a window, a display or a dialog this machine cannot show.",
                    args.join(" "),
                    PATIENCE.as_secs()
                );
            }
            None => std::thread::sleep(std::time::Duration::from_millis(50)),
        }
    }
    child.wait_with_output().expect("the child's output")
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
    let mut skipped = 0;
    let total = program_directories().len();
    for dir in program_directories() {
        let main = relative(&dir.join("main.hero"));
        for level in configurations() {
            let out = heroes(&root, &["test".to_string(), main.clone(), level.to_string()]);
            if machine_lacks_the_library(&out) {
                skipped += 1;
                break;
            }
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
            if *level == "--sanitize" {
                assert!(
                    !noise.contains("AddressSanitizer") && !noise.contains("runtime error:"),
                    "{main} tripped a sanitiser:\n{noise}"
                );
            }
        }
    }
    // **A machine may be missing a library; it may not be missing most of them.**
    // Without this, a runner with no C libraries at all would report the corpus
    // green by testing none of it — which is the failure mode a skip always
    // invites and the reason the count is asserted rather than printed.
    assert!(
        skipped * 3 <= total,
        "{skipped} of {total} programs were skipped for want of a library — this machine is not testing the corpus"
    );
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
        if machine_lacks_the_library(&built) {
            continue;
        }
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
        for level in configurations().iter().filter(|l| **l != "-O0") {
            let mut invocation = vec!["run".to_string(), main.clone(), level.to_string()];
            if !args.is_empty() {
                invocation.push("--".to_string());
                invocation.extend(args.iter().cloned());
            }
            let out = heroes(&root, &invocation);
            if *level == "--sanitize" {
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
            if machine_lacks_the_library(&out) {
                break;
            }
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

/// **Panel 058's refused option (iii) has a standing reopen condition, and this is
/// what makes it fire.**
///
/// `malloc` + `ptr` is the route the sitting adopted for every C function that
/// writes into a caller's buffer, and it works — `getcwd`, `strtok`, `putenv` and
/// ncurses all run with zero shims. **What it gives back is no length.** So a C
/// function whose contract is *"I wrote N bytes, here is N"* has no correct
/// spelling in this language: the program learns N and cannot turn N bytes of a
/// `ptr` into a `str`. A mutable buffer type returns to the panel the day a
/// binding the closure list or §4.19's ladder needs requires that, and **not one
/// day sooner** — the refusal is conditional, not permanent.
///
/// It sat on `DECIDE.md` for a day as a note that said *"something must fire when
/// it comes true"*, and nothing could: it was prose, and it was waiting for a
/// person to re-read a list. CLAUDE.md §11's rule is that a premise which can
/// expire is owed **a test that fires when it dies, whose failure message names
/// what depends on it** — so here it is, written as the falsifiable claim it
/// always was: *no program in this repository binds a C function that reports a
/// written length.*
///
/// The list is the named contract rather than a guess at one, and it is
/// deliberately a **proxy**: it fires on the binding rather than on the need, so
/// it can fire early. Early is the correct direction — a sitting convened one
/// binding too soon costs an hour, and one convened too late costs a shim that
/// outlives the milestone.
#[test]
fn no_binding_needs_a_length_back_from_c() {
    // Each of these returns a count of bytes it wrote (or would have written) and
    // hands back no terminator to find it by. `read`/`recv` are here for the same
    // reason as `readlink`: the count arrives, and the bytes have no spelling.
    const REPORTS_A_LENGTH: [&str; 10] = [
        "snprintf", "vsnprintf", "readlink", "readlinkat", "strlcpy", "strlcat", "mbstowcs",
        "wcstombs", "recv", "pread",
    ];
    let mut found: Vec<String> = Vec::new();
    let mut stack = vec![workspace_root()];
    while let Some(at) = stack.pop() {
        let Ok(entries) = std::fs::read_dir(&at) else { continue };
        for entry in entries.flatten() {
            let path = entry.path();
            let name = path.file_name().unwrap_or_default().to_string_lossy().into_owned();
            if path.is_dir() {
                if !matches!(name.as_str(), "target" | "build" | ".git" | "archive") {
                    stack.push(path);
                }
                continue;
            }
            if path.extension().and_then(|e| e.to_str()) != Some("hero") {
                continue;
            }
            let Ok(text) = std::fs::read_to_string(&path) else { continue };
            for line in text.lines() {
                let code = line.split('#').next().unwrap_or("");
                let Some(rest) = code.trim().strip_prefix("function ") else { continue };
                let called = rest.split('(').next().unwrap_or("").trim();
                if REPORTS_A_LENGTH.contains(&called) {
                    found.push(format!("  {}: {}", path.display(), code.trim()));
                }
            }
        }
    }
    assert!(
        found.is_empty(),
        "a binding now reports a written length, and this language has no way to \
         use it — `malloc` + `ptr` gives the bytes and never the count, so the \
         program learns N and cannot turn N bytes into a `str`.\n\n{}\n\n\
         This is panel 058's refused option (iii) coming true. Its refusal was \
         CONDITIONAL on exactly this, so the mutable buffer type returns to the \
         panel now — see `docs/panel/058` § Findings and design.md Part 7 item 10. \
         If the binding below does not actually need the count back, add it to \
         `REPORTS_A_LENGTH`'s exceptions with the reason, which is a decision and \
         belongs in the record.",
        found.join("\n")
    );
}
