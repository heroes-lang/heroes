//! The pipeline from a `.hero` file to a native binary, shared by `build` and `run`
//! (design.md §3.1, panel 020).
//!
//! Every stage runs only on the one before it having said nothing — the rule `check`
//! established, for the same reason: after a type error the checker's answers are
//! guesses, and lowering reads them as fact.
//!
//! **Two stages are stops.** `--dump-ir` prints the IR and returns; `--emit-c`
//! prints the C and returns. Neither invokes clang, and neither needs an entry
//! point: a file with no `main` is a perfectly good translation unit, and a dump is
//! inspection (design.md §3.5). Only the two invocations that promise a *binary* ask
//! for `main`, and they ask before the linker does — `ld: undefined symbol _main`
//! names a symbol the author never wrote.
//!
//! **A holed file exits 1.** §4.16 says a hole is not an error and that such a file
//! "produces no binary"; both halves are kept by exiting 1 with the holes reported
//! and `no binary: N hole(s)` as the last line on stderr. Exit 0 was measured: the
//! panel's llm-ergonomist wrote `heroes build f.hero && ./build/<hash>/f`, which
//! reports success and then executes **yesterday's binary**, attributing its output
//! to today's source.
//!
//! **A clang failure is exit 2**, never 1. The generated C is not the author's text,
//! so a diagnostic about it is a statement that this compiler is wrong.

use std::path::PathBuf;

use heroes::diagnostics::Diagnostic;
use heroes::emit::{emit_for, entry_point, module_of, Target};
use heroes::ir::{dump, lower, verify};
use heroes::ir::mono;
use heroes::own;
use heroes::resolve::resolve;
use heroes::source::Span;
use heroes::syntax::parse;
use heroes::types::{check, report_holes};

use super::contract::{report, write};

use crate::cli::Exit;
use crate::input;

use super::toolchain::Toolchain;

pub struct Options {
    /// The clang optimisation level. Part of the cache key, because two levels
    /// sharing one directory is one configuration pretending to be two.
    ///
    /// Chosen by the **verb's default and a flag that overrides it** — see
    /// `level_from`. Until 2026-08-12 the verb was the only way to say it, which
    /// panel 021 recorded as an asymmetry visible in `--help`: sanitising was a
    /// flag and the level was a subcommand, so `build` could not be asked for the
    /// level the golden harness runs at.
    pub level: &'static str,
    pub dump_ir: bool,
    pub emit_c: bool,
    pub output: Option<String>,
    /// `-fsanitize=address,undefined`. Part of the cache key: a sanitised build
    /// sharing a directory with a plain one is the same silent staleness the level
    /// key exists to prevent.
    pub sanitize: bool,
    /// Which translation unit to build: the program, or the one that runs the
    /// file's `test` blocks (§4.18). Part of the cache key, for the reason the two
    /// above are — a test binary sharing a directory with a program binary is one
    /// configuration pretending to be two.
    pub target: Target,
    /// `--include` and `--library`, in the order written, which is the order clang
    /// searches (author decision 2026-08-14, settling panel 055's split).
    ///
    /// **They are in the cache key**, and that is a measured condition rather than
    /// a precaution: the judge who built this found its own prototype serving one
    /// artifact to two configurations, because `dir_for` hashed the source, the
    /// level, the sanitiser and the runtime and nothing else — so `--include /old`
    /// and `--include /new` on unchanged source hashed identically and the second
    /// build silently reused the first. That is the staleness the level key exists
    /// to prevent, arriving through a new door.
    pub search: Search,
}

/// Where to look for a header and for a library, when no `package` can say.
#[derive(Clone, Default)]
pub struct Search {
    pub include: Vec<String>,
    pub library: Vec<String>,
}

impl Search {
    /// The part of the cache key these contribute. Empty when neither was given,
    /// so every existing build directory keeps its name.
    pub fn key(&self) -> String {
        if self.include.is_empty() && self.library.is_empty() {
            return String::new();
        }
        format!("\u{1}I{}\u{1}L{}", self.include.join(":"), self.library.join(":"))
    }
}

/// `Ok(None)` — it stopped at a dump. `Ok(Some(path))` — that binary exists now.
pub fn compile(path: &str, options: &Options) -> Result<Option<PathBuf>, Exit> {
    compile_with_tests(path, options).map(|pair| pair.map(|(binary, _)| binary))
}

/// The same pipeline, handing back the `test` block titles as well.
///
/// `heroes test` needs them before it runs anything: the binary takes an index,
/// and the titles stay in the compiler where they are already exact rather than
/// being marshalled through C (see `decls::test_shim`).
pub fn compile_with_tests(
    path: &str,
    options: &Options,
) -> Result<Option<(PathBuf, Vec<String>)>, Exit> {
    let src = match input::read(path) {
        Ok(src) => src,
        Err((message, exit)) => {
            eprintln!("error: {message}");
            return Err(exit);
        }
    };
    let parsed = parse(&src);
    report(&parsed.diagnostics, &src)?;
    // Same stage, same reason, as `check`: the cause once beats the consequence
    // nine times. A module that is not there is not a name error.
    report(&heroes::modules::errors(&parsed.ast, &src), &src)?;
    let resolved = resolve(&parsed.ast, &src);
    report(&resolved.diagnostics, &src)?;
    let checked = check(&parsed.ast, &resolved, &src);
    report(&checked.diagnostics, &src)?;
    let lowered = lower(&parsed.ast, &resolved, &checked, &src);
    report(&lowered.diagnostics, &src)?;
    // The IR verifier: GHC's Core Lint, on every function, every time. Not gated
    // behind a flag because the cost is linear in the program and the thing it
    // catches is a wrong compiler.
    let problems = verify(&lowered.program, &checked, &parsed.ast);
    if !problems.is_empty() {
        eprintln!("internal error: the lowered program is not well formed");
        for problem in &problems {
            eprintln!("  {problem}");
        }
        return Err(Exit::Failed);
    }
    // **Monomorphisation first, and the order is forced** (panel 029 R1). The
    // ownership pass asks `is_refcounted`, which answers `false` for a type
    // parameter — right for `T = int` and a leak for `T = str` — so running it
    // first does not decline to decide, it decides wrongly, and the verifier
    // cannot catch it because `released_on_return` asks the same predicate.
    let mut lowered = lowered;
    let mut checked = checked;
    let problems = mono::run(&mut lowered.program, &mut checked, &parsed.ast, &src);
    report(&problems, &src)?;
    let problems = verify(&lowered.program, &checked, &parsed.ast);
    if !problems.is_empty() {
        eprintln!("internal error: monomorphisation produced an ill-formed program");
        for problem in &problems {
            eprintln!("  {problem}");
        }
        return Err(Exit::Failed);
    }
    // The ownership pass, and `--dump-ir` shows its result because that is what the
    // emitter sees (panel 021 R8). The verifier runs again afterwards, at phase
    // `Owned`, where the invariants are different ones.
    own::run(&mut lowered.program, &checked);
    let problems = verify(&lowered.program, &checked, &parsed.ast);
    if !problems.is_empty() {
        eprintln!("internal error: the ownership pass produced an ill-formed program");
        for problem in &problems {
            eprintln!("  {problem}");
        }
        return Err(Exit::Failed);
    }
    if options.dump_ir {
        print!("{}", dump(&lowered.program, &parsed.ast, &checked, &src));
        if !checked.holes.is_empty() {
            print!("{}", report_holes(&parsed.ast, &resolved, &checked, &src));
        }
        return Ok(None);
    }
    // §4.16: the hole is not an error, and there is still no binary. Both halves.
    if !checked.holes.is_empty() {
        print!("{}", report_holes(&parsed.ast, &resolved, &checked, &src));
        let count = checked.holes.len();
        let unit = if count == 1 { "hole" } else { "holes" };
        // The files the holes are actually in, not the file that was named on
        // the command line: since M-module-namespace they are not the same thing, and a count
        // attached to the wrong file sends the reader to the wrong file.
        let mut files: Vec<&str> = Vec::new();
        for hole in &checked.holes {
            let (file, _, _) = src.locate(hole.span.start);
            if !files.contains(&file) {
                files.push(file);
            }
        }
        eprintln!("no binary: {count} {unit} in {}", files.join(", "));
        return Err(Exit::Diagnostics);
    }
    let emitted =
        emit_for(options.target, &lowered.program, &parsed.ast, &resolved, &checked, &src);
    report(&emitted.diagnostics, &src)?;
    if options.emit_c {
        match &options.output {
            Some(target) => write(target, &emitted.c)?,
            None => print!("{}", emitted.c),
        }
        return Ok(None);
    }
    // A test build needs no `main`: the `test` blocks are the program. A file with
    // tests and no `main` is a legitimate thing to write, and `heroes test` is
    // where it runs.
    if options.target == Target::Program && entry_point(&lowered.program).is_none() {
        // The root file's end, never the whole text's: since the library is
        // appended, `text.len()` lands inside it and `library::misplaced` turns
        // the user's own mistake into `internal error … exit 2` — the message
        // panel 020 measured sending an agent to reinstall its toolchain.
        let end = src.root_end();
        let diagnostic = Diagnostic::new(
            "no_entry_point",
            "this file declares no `function main()`, so it cannot become a binary".to_string(),
            Span { start: end.saturating_sub(1), end },
        )
        .with_note(
            "add `function main()`, or use `--emit-c` to produce C for another program to compile"
                .to_string(),
        );
        report(std::slice::from_ref(&diagnostic), &src)?;
    }
    let toolchain = match Toolchain::find() {
        Ok(found) => found,
        Err(message) => {
            eprintln!("error: {message}");
            return Err(Exit::Failed);
        }
    };
    let module = module_of(&src.name);
    let level_key = match options.target {
        Target::Program => options.level.to_string(),
        Target::Tests => format!("{}+tests", options.level),
    };
    let dir = match toolchain.dir_for(&src.name, &src.text, &level_key, options.sanitize, &options.search.key()) {
        Ok(dir) => dir,
        Err(message) => {
            eprintln!("error: {message}");
            return Err(Exit::Failed);
        }
    };
    let c_file = dir.join(format!("{module}.c"));
    write(&c_file.display().to_string(), &emitted.c)?;
    let object = match toolchain.runtime_object(options.level, options.sanitize) {
        Ok(object) => object,
        Err(message) => {
            eprintln!("internal error: {message}");
            return Err(Exit::Failed);
        }
    };
    let binary = match &options.output {
        Some(target) => PathBuf::from(target),
        None => dir.join(&module),
    };
    // The source's own directory, so `extern "mylib.h"` finds a header beside the
    // `.hero` file. Passed explicitly because the unit lives under `build/<hash>/`
    // and an angled include never searches the source's directory (panel 036).
    // **An empty parent is the current directory, not the absence of one** — the
    // 2026-08-16 defect, pinned by `surface.rs`'s `a_header_beside_the_program…`.
    let include = Some(match std::path::Path::new(&src.name).parent() {
        Some(parent) if !parent.as_os_str().is_empty() => parent.to_path_buf(),
        _ => std::path::PathBuf::from("."),
    });
    if let Err(message) = toolchain.link(
        &c_file,
        &object,
        &binary,
        options.level,
        options.sanitize,
        include.as_deref(),
        &super::libraries::Libraries {
            link: emitted.link.clone(),
            packages: emitted.packages.clone(),
            search: options.search.clone(),
        },
    ) {
        // **Some clang failures are the author's**, and `ffi.rs` is the only place
        // that decides which: the assertions and probes §4.19 generates per
        // `extern` exist to fail when a declaration disagrees with the real
        // header. Reported as an internal error they would print C the author
        // never wrote and blame the compiler for a mistake in a `.hero` file
        // (panel 036 rider 3). The lowered program and its types go with the AST
        // because the parameter class rebuilds a probe's own line to find which
        // argument clang's column names (panel 052).
        let theirs =
            heroes::emit::ffi::explain(&message, &parsed.ast, &checked, &lowered.program, &src);
        if !theirs.is_empty() {
            report(&theirs, &src)?;
            return Err(Exit::Diagnostics);
        }
        // Every other verdict on generated C is a statement about this compiler, so
        // it speaks in the compiler's own vocabulary and exits 2 (panel 019's R1
        // rule reserves that for exactly here and for `--dump-ir`).
        eprintln!("internal error: {message}");
        eprintln!("the generated C is at {}", c_file.display());
        return Err(Exit::Failed);
    }
    Ok(Some((binary, heroes::emit::tests_of(&lowered.program))))
}
