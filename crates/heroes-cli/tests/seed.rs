//! **The seed: a clean checkout, a C compiler, and nothing else.**
//!
//! This is the gate `docs/ROADMAP.md` puts in front of the archive, in its own
//! words: *"it is tested from a clean checkout with nothing but a C compiler, and
//! if that test is not written before the archive commit, the archive commit does
//! not happen"*. This file is that test.
//!
//! **Why it archives the tree instead of using it.** A test that ran `clang` in
//! the working directory would prove nothing about a *checkout*: the thing that
//! can go wrong is an untracked file — a generated header, a leftover object, a
//! path only this machine has — silently making the build work here and nowhere
//! else. `git archive HEAD` writes exactly what a `git clone` would give a
//! stranger, so the build below sees the repository as they will see it.
//!
//! **What it deliberately does not check.** It does not compare `seed/heroes.c`
//! against a fresh emission. That would make every change to `selfhost/` a seed
//! refresh — option A4, refused at panel 085 R1 because 152 commits is 152 seed
//! reviews and nobody performs them. The refresh rule is the other one: the seed
//! is regenerated in the commit that breaks it, and *breaks it* is what the build
//! here measures.

use std::path::{Path, PathBuf};

fn workspace_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("workspace root exists")
        .to_path_buf()
}

/// The C compiler this machine has, asked of the environment the way the
/// toolchain asks: `CC` if it is set, `clang` otherwise. A machine with neither
/// cannot run this test and says so rather than passing.
fn c_compiler() -> String {
    std::env::var("CC").unwrap_or_else(|_| "clang".to_string())
}

fn run(program: &str, dir: &Path, args: &[&str]) -> std::process::Output {
    std::process::Command::new(program)
        .current_dir(dir)
        .args(args)
        .output()
        .unwrap_or_else(|e| panic!("`{program} {}` runs: {e}", args.join(" ")))
}

/// A newcomer with a C compiler and this repository gets a working Heroes
/// compiler, and the compiler they get compiles and runs a program.
///
/// The four things this pins, and each is a way the seed has been or could be
/// worthless:
///
/// 1. **the checkout is complete** — the build runs on `git archive HEAD`, so an
///    untracked file cannot be holding it up;
/// 2. **the documented command is the command** — one line, `-I runtime` and two
///    inputs, exactly as `seed/README.md` prints it. If the real build needs a
///    flag the README does not name, this goes red;
/// 3. **the seed is not stale against the runtime** — its `_Static_assert` on
///    `HERO_RUNTIME_ABI` is the one skew the stamp exists for, and it is checked
///    here as a number rather than trusted to fire;
/// 4. **it is a compiler, not a binary that prints a version** — it builds and
///    runs a program end to end, which means it also found the runtime and drove
///    clang itself.
#[test]
fn the_seed_builds_from_a_clean_checkout() {
    let root = workspace_root();
    let seed = root.join("seed").join("heroes.c");
    assert!(
        seed.is_file(),
        "seed/heroes.c is missing. It is the only way a clean checkout can build a \
         compiler, and `docs/ROADMAP.md` makes it the gate in front of the archive"
    );

    // The checkout, as a stranger would receive it.
    let checkout = root.join("build").join("seed-checkout");
    let _ = std::fs::remove_dir_all(&checkout);
    std::fs::create_dir_all(&checkout).expect("the checkout directory");
    let archive = root.join("build").join("seed-checkout.tar");
    let wrote = run(
        "git",
        &root,
        &["archive", "--format=tar", "-o", &archive.display().to_string(), "HEAD"],
    );
    assert!(
        wrote.status.success(),
        "git archive HEAD failed — this test asks what a clone would contain, so it \
         needs one\nstderr:\n{}",
        String::from_utf8_lossy(&wrote.stderr)
    );
    let untarred = run("tar", &checkout, &["-xf", &archive.display().to_string()]);
    assert!(untarred.status.success(), "the archive unpacks");

    // (3) The stamp, as a number, before anything is compiled: a mismatch here is
    // the staleness the seed's own `_Static_assert` exists to catch, and reading
    // both sides says *which* is behind rather than only that they disagree.
    let seed_text = std::fs::read_to_string(checkout.join("seed").join("heroes.c"))
        .expect("the archived seed is readable");
    let header = std::fs::read_to_string(checkout.join("runtime").join("heroes_runtime.h"))
        .expect("the archived runtime header is readable");
    let stamped = seed_text
        .lines()
        .find_map(|line| line.strip_prefix("_Static_assert(HERO_RUNTIME_ABI == "))
        .and_then(|rest| rest.split(',').next())
        .expect("the seed carries the ABI assertion on its own line");
    let defined = header
        .lines()
        .find_map(|line| line.strip_prefix("#define HERO_RUNTIME_ABI "))
        .expect("the runtime defines HERO_RUNTIME_ABI");
    assert_eq!(
        stamped.trim(),
        defined.trim(),
        "the seed was emitted against runtime ABI {stamped} and the runtime is now \
         {defined}. Regenerate it: heroes build selfhost/main.hero --emit-c -o \
         seed/heroes.c (seed/README.md § When this file must be regenerated)"
    );

    // (2) The documented line, verbatim.
    let compiler = c_compiler();
    let built = run(
        &compiler,
        &checkout,
        &["-I", "runtime", "seed/heroes.c", "runtime/runtime.c", "-o", "heroes-seed"],
    );
    assert!(
        built.status.success(),
        "the seed did not compile with `{compiler} -I runtime seed/heroes.c \
         runtime/runtime.c`, which is the one command seed/README.md gives a \
         newcomer.\nstderr:\n{}",
        String::from_utf8_lossy(&built.stderr)
    );
    let seeded = checkout.join("heroes-seed");
    assert!(seeded.is_file(), "the seed produced a binary");

    let version = run(&seeded.display().to_string(), &checkout, &["--version"]);
    assert!(version.status.success(), "the seed-built compiler answers --version");
    let said = String::from_utf8_lossy(&version.stdout);
    assert!(said.contains("heroes"), "it says what it is: {said}");

    // (4) It is a compiler. The program is written here rather than taken from
    // `tests/golden/` on purpose: this test is about the *checkout*, and a case
    // file is one more thing that could be missing from it without the seed being
    // at fault.
    let program = checkout.join("seed-smoke.hero");
    std::fs::write(
        &program,
        "function main()\n    for i in range(from: 0, to: 3)\n        print(i * 2)\n",
    )
    .expect("the smoke program is written");
    let ran = run(&seeded.display().to_string(), &checkout, &["run", "seed-smoke.hero"]);
    let printed = String::from_utf8_lossy(&ran.stdout).into_owned();
    let complained = String::from_utf8_lossy(&ran.stderr).into_owned();
    assert!(
        ran.status.success(),
        "the seed-built compiler could not run a three-line program.\nstdout: \
         {printed}\nstderr: {complained}"
    );
    assert_eq!(
        printed, "0\n2\n4\n",
        "and it printed the wrong thing, which means the compiler is wrong rather \
         than absent:\nstderr: {complained}"
    );
    // `range` is the row that catches the defect this milestone opened on: the
    // library used to be read from `crates/` at run time, so every program using
    // it was refused **on the author's line** once that directory was not under
    // the working directory (panel 085 R5).
    assert!(
        !complained.contains("builtin_shape"),
        "the standard library did not reach the program: {complained}"
    );
}
