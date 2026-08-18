//! **The two compilers, on the same programs, compared byte for byte.**
//!
//! This file exists because the instrument it holds did not, and its absence hid
//! three defects at once (panel 085 R6, 2026-08-18).
//!
//! M-selfhost-port's own differential compared **streams** — five commands over
//! 257 corpus inputs, stdout and stderr, 2,570 byte-exact matches. It is a strong
//! instrument and it has a hole exactly the shape of this milestone: `heroes build
//! x.hero --emit-c -o out.c` writes an **artifact**, not a stream, so the one
//! output the finish line is *about* was compared for exactly one input —
//! `selfhost/main.hero` itself. Everything the compiler's own source does not
//! happen to contain was therefore uncompared, and the port has no fixed-array
//! field, no hyphen in a module name and no float literal.
//!
//! What that cost, measured the day this file was written: **26 of 127 programs
//! byte-identical**. Three classes, and every one of them was invisible to every
//! other instrument in the repository —
//!
//! 1. **81 programs** disagreed on the `#line` restore file name, where the port
//!    wrote a name nothing creates (`while-loop.c` against `whileloop.c`);
//! 2. **19 programs** disagreed on how a float literal is spelled, hex against
//!    decimal — and the goldens could not see it because the emitted C of a
//!    `run/` case is not snapshotted, while `corpus.rs` compares a program's
//!    *output*, where the two spellings give the same number;
//! 3. **3 programs** got C from the port that does not compile at all: an
//!    undeclared identifier, `heroes build` exit 2, against exit 0 from the
//!    bootstrap.
//!
//! **Why this cannot be a golden.** A golden pins one compiler against a stored
//! file, and both compilers passing every golden is what the port already did.
//! The question here is *do these two programs agree*, and no stored artifact
//! answers it — the oracle is the other compiler. That makes this the one test in
//! the repository whose expectation is a **second implementation**, which is also
//! why it is the only one that can go dark: it dies with the bootstrap, and what
//! replaces it before that day is the archive milestone's first row.
//!
//! Cost, measured: ~8 s to build the port (content-addressed, so a rerun is a
//! cache hit) and **2.8 s for 86 programs** — the cheapest instrument per defect
//! this project has.

use std::path::{Path, PathBuf};

/// How long a single emission may take before it is hung rather than slow. The
/// port emits a `run/` case in well under a second; this is three orders of
/// magnitude of headroom, because a hung child burns the whole job.
const PATIENCE: std::time::Duration = std::time::Duration::from_secs(300);

fn workspace_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("workspace root exists")
        .to_path_buf()
}

/// A path as every artifact in this repository spells it: `/` on every platform.
fn slashed(path: &Path) -> String {
    path.display().to_string().replace('\\', "/")
}

fn run(program: &Path, root: &Path, args: &[String]) -> std::process::Output {
    let mut child = std::process::Command::new(program)
        .current_dir(root)
        .args(args)
        .stdout(std::process::Stdio::piped())
        .stderr(std::process::Stdio::piped())
        .spawn()
        .unwrap_or_else(|e| panic!("{} runs: {e}", program.display()));
    let began = std::time::Instant::now();
    loop {
        match child.try_wait().expect("the child can be waited on") {
            Some(_) => break,
            None if began.elapsed() > PATIENCE => {
                let _ = child.kill();
                let _ = child.wait();
                panic!("{} did not finish in {PATIENCE:?} on {args:?}", program.display());
            }
            None => std::thread::sleep(std::time::Duration::from_millis(20)),
        }
    }
    child.wait_with_output().expect("the child's output")
}

/// Build the self-hosted compiler with the bootstrap, and hand back its path.
///
/// **Built rather than found.** A binary lying around from an earlier session is
/// a binary from earlier `selfhost/` source, and a differential against a stale
/// port is the one failure mode that reports success — the same shape as the
/// stale runtime object that cost two debugging detours before the cache key grew
/// to cover the whole configuration (`commands/toolchain.rs`).
fn build_the_port(root: &Path) -> Option<PathBuf> {
    let out = root.join("build").join("differential-port");
    let built = run(
        Path::new(env!("CARGO_BIN_EXE_heroes")),
        root,
        &[
            "build".to_string(),
            "selfhost/main.hero".to_string(),
            "-o".to_string(),
            slashed(&out),
        ],
    );
    // **A platform with no POSIX headers has no second compiler to compare
    // against**, and the reason is `selfhost/cli_io.hero`'s `extern "unistd.h"`
    // rather than anything here. Asked of the machine, not of a `cfg!` list, so the
    // day the port stops binding it this starts running instead of staying skipped
    // for a reason nobody rechecks (`corpus.rs`'s rule for the same class).
    let complaint = String::from_utf8_lossy(&built.stderr).into_owned();
    if !built.status.success() && complaint.contains("unistd.h") && complaint.contains("not found")
    {
        eprintln!(
            "SKIPPED: this platform has no <unistd.h>, which selfhost/cli_io.hero \
             binds, so there is no self-hosted compiler here to differ from:\n{complaint}"
        );
        return None;
    }
    assert!(
        built.status.success(),
        "the bootstrap could not build selfhost/main.hero — every other assertion \
         in this file is about the two compilers agreeing, and there is no second \
         compiler until this succeeds\nstderr:\n{complaint}"
    );
    Some(out)
}

/// Every program both compilers should agree on: the goldens whose C is real
/// (`run/`, `emit/`, `ir/`, `fixedbugs/`) and every corpus program.
///
/// `check/` and `unsupported/` are deliberately absent and the reason is not
/// laziness: those cases are refused at exit 1, so they have no emitted C to
/// compare, and their agreement is what the M-selfhost-port stream differential
/// already covers. A directory joins this list by holding programs that reach the
/// emitter.
fn programs(root: &Path) -> Vec<PathBuf> {
    let mut found = Vec::new();
    for directory in ["run", "emit", "ir", "fixedbugs"] {
        let dir = root.join("tests").join("golden").join(directory);
        let entries = std::fs::read_dir(&dir)
            .unwrap_or_else(|e| panic!("cannot read {}: {e}", dir.display()));
        for entry in entries {
            let path = entry.expect("a readable entry").path();
            if path.extension().and_then(|e| e.to_str()) == Some("hero") {
                found.push(path);
            }
        }
    }
    let examples = root.join("examples");
    for entry in std::fs::read_dir(&examples).expect("examples/ must exist") {
        let directory = entry.expect("a readable entry").path();
        for name in ["main.hero", "whole.hero"] {
            let program = directory.join(name);
            if program.is_file() {
                found.push(program);
            }
        }
    }
    found.sort();
    assert!(
        found.len() >= 100,
        "the differential lost its corpus: {} programs, and it was 127 when this \
         floor was written",
        found.len()
    );
    found
}

/// The emitted C of the bootstrap and of the self-hosted compiler are the same
/// bytes, for every program in the repository that reaches the emitter.
///
/// **This is the fixpoint's own claim, asked of more than one input.** `diff B.c
/// C.c` over `selfhost/main.hero` proves the two compilers agree on the one
/// program that happens to be the compiler; this asks the same question of every
/// shape the language has, in the proportions the corpus has them.
///
/// A failure here names the program and the first lines that differ. It is not a
/// golden and there is nothing to regenerate: one of the two compilers is wrong,
/// and which one is decided by reading, not by `UPDATE_GOLDEN=1` (CLAUDE.md §9
/// forbids that route here by construction — there is no stored file to update).
#[test]
fn the_two_compilers_emit_the_same_c() {
    let root = workspace_root();
    let Some(port) = build_the_port(&root) else {
        return;
    };
    let bootstrap = PathBuf::from(env!("CARGO_BIN_EXE_heroes"));
    let mine = root.join("build").join("differential");
    std::fs::create_dir_all(&mine).expect("the scratch directory");

    let mut compared = 0usize;
    let mut divergences: Vec<String> = Vec::new();
    for program in programs(&root) {
        let relative =
            slashed(program.strip_prefix(&root).expect("under the workspace root")).to_string();
        let from_bootstrap = mine.join("bootstrap.c");
        let from_port = mine.join("port.c");
        let emit = |compiler: &Path, into: &Path| {
            let _ = std::fs::remove_file(into);
            let out = run(
                compiler,
                &root,
                &[
                    "build".to_string(),
                    relative.clone(),
                    "--emit-c".to_string(),
                    "-o".to_string(),
                    slashed(into),
                ],
            );
            (out, std::fs::read(into).ok())
        };
        let (bootstrap_run, bootstrap_c) = emit(&bootstrap, &from_bootstrap);
        let (port_run, port_c) = emit(&port, &from_port);

        // **Exit codes first, because a divergence in them is the louder
        // finding.** The three fixed-array programs were exactly this: the
        // bootstrap at exit 0 and the port at exit 2, the compiler blaming
        // itself for C it had written. Comparing only the bytes would have
        // reported "one file missing" and buried the reason.
        if bootstrap_run.status.code() != port_run.status.code() {
            divergences.push(format!(
                "{relative}: exit {:?} from the bootstrap, {:?} from the port\n  \
                 port stderr: {}",
                bootstrap_run.status.code(),
                port_run.status.code(),
                String::from_utf8_lossy(&port_run.stderr).lines().take(3).collect::<Vec<_>>().join(" / ")
            ));
            continue;
        }
        // A program this machine cannot build for want of a C library it binds is
        // nobody's defect — and it is skipped only when **both** compilers say so,
        // which keeps a real one-sided failure above.
        let (Some(bootstrap_c), Some(port_c)) = (bootstrap_c, port_c) else {
            continue;
        };
        compared += 1;
        if bootstrap_c == port_c {
            continue;
        }
        let differing = first_differing_lines(&bootstrap_c, &port_c);
        divergences.push(format!("{relative}: the emitted C differs\n{differing}"));
    }

    assert!(
        compared >= 100,
        "only {compared} programs were actually compared, and it was 127 when this \
         floor was written — a differential that silently stops comparing is worse \
         than no differential"
    );
    assert!(
        divergences.is_empty(),
        "{} of {compared} programs disagree between the bootstrap and the \
         self-hosted compiler.\n\n{}\n\nOne of the two is wrong. There is no \
         snapshot to regenerate here: read both, decide which, and fix that one.",
        divergences.len(),
        divergences.join("\n\n")
    );
}

/// The first few differing lines, side by side, with their line numbers — enough
/// to name the class without printing 20 MB.
fn first_differing_lines(left: &[u8], right: &[u8]) -> String {
    let left = String::from_utf8_lossy(left);
    let right = String::from_utf8_lossy(right);
    let mut shown = String::new();
    let mut count = 0usize;
    for (at, (l, r)) in left.lines().zip(right.lines()).enumerate() {
        if l == r {
            continue;
        }
        shown.push_str(&format!("  line {}:\n    bootstrap: {l}\n    port:      {r}\n", at + 1));
        count += 1;
        if count == 3 {
            break;
        }
    }
    if shown.is_empty() {
        shown.push_str(&format!(
            "  no differing line — the files differ in length: {} bytes against {}\n",
            left.len(),
            right.len()
        ));
    }
    shown
}
