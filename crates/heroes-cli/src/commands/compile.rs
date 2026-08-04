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

use heroes::diagnostics::{render, Diagnostic};
use heroes::emit::{emit, entry_point, module_of};
use heroes::ir::{dump, lower, verify};
use heroes::own;
use heroes::resolve::resolve;
use heroes::source::{Source, Span};
use heroes::syntax::parse;
use heroes::types::{check, report_holes};

use crate::cli::Exit;
use crate::input;

use super::toolchain::Toolchain;

pub struct Options {
    /// `-O0` for `build`, `-O2` for `run`. Part of the cache key, because two
    /// levels sharing one directory is one configuration pretending to be two.
    pub level: &'static str,
    pub dump_ir: bool,
    pub emit_c: bool,
    pub output: Option<String>,
    /// `-fsanitize=address,undefined`. Part of the cache key: a sanitised build
    /// sharing a directory with a plain one is the same silent staleness the level
    /// key exists to prevent.
    pub sanitize: bool,
}

/// `Ok(None)` — it stopped at a dump. `Ok(Some(path))` — that binary exists now.
pub fn compile(path: &str, options: &Options) -> Result<Option<PathBuf>, Exit> {
    let src = match input::read(path) {
        Ok(src) => src,
        Err((message, exit)) => {
            eprintln!("error: {message}");
            return Err(exit);
        }
    };
    let parsed = parse(&src);
    report(&parsed.diagnostics, &src)?;
    let resolved = resolve(&parsed.ast, &src);
    report(&resolved.diagnostics, &src)?;
    let checked = check(&parsed.ast, &resolved, &src);
    report(&checked.diagnostics, &src)?;
    let lowered = lower(&parsed.ast, &resolved, &checked, &src);
    report(&lowered.diagnostics, &src)?;
    // The IR verifier: GHC's Core Lint, on every function, every time. Not gated
    // behind a flag because the cost is linear in the program and the thing it
    // catches is a wrong compiler.
    let problems = verify(&lowered.program, &checked);
    if !problems.is_empty() {
        eprintln!("internal error: the lowered program is not well formed");
        for problem in &problems {
            eprintln!("  {problem}");
        }
        return Err(Exit::Failed);
    }
    // The ownership pass: the first IR→IR pass, and `--dump-ir` shows its result
    // because that is what the emitter sees (panel 021 R8). The verifier runs again
    // afterwards, at phase `Owned`, where the invariants are different ones.
    let mut lowered = lowered;
    own::run(&mut lowered.program, &checked);
    let problems = verify(&lowered.program, &checked);
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
        eprintln!("no binary: {count} {unit} in {}", src.name);
        return Err(Exit::Diagnostics);
    }
    let emitted = emit(&lowered.program, &parsed.ast, &resolved, &checked, &src);
    report(&emitted.diagnostics, &src)?;
    if options.emit_c {
        match &options.output {
            Some(target) => write(target, &emitted.c)?,
            None => print!("{}", emitted.c),
        }
        return Ok(None);
    }
    if entry_point(&lowered.program).is_none() {
        let end = src.text.len() as u32;
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
    let dir = match toolchain.dir_for(&src.name, &src.text, options.level, options.sanitize) {
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
    if let Err(message) = toolchain.link(&c_file, &object, &binary, options.level, options.sanitize) {
        // clang's verdict on generated C is a statement about this compiler, so it
        // speaks in the compiler's own vocabulary and exits 2 (panel 019's R1 rule
        // reserves that for exactly here and for `--dump-ir`).
        eprintln!("internal error: {message}");
        eprintln!("the generated C is at {}", c_file.display());
        return Err(Exit::Failed);
    }
    Ok(Some(binary))
}

fn write(path: &str, text: &str) -> Result<(), Exit> {
    if let Some(parent) = std::path::Path::new(path).parent() {
        if !parent.as_os_str().is_empty() {
            let _ = std::fs::create_dir_all(parent);
        }
    }
    std::fs::write(path, text).map_err(|e| {
        eprintln!("error: cannot write {path}: {e}");
        Exit::Failed
    })
}

/// §4.17's rich form, on stderr, and `Exit::Diagnostics`.
///
/// `build` and `run` take no diagnostic-shaping flags: `check` already answers every
/// question about how to print them, and two commands with two renderings of one
/// diagnostic is the kind of surface panel 016 exists to prevent.
fn report(diagnostics: &[Diagnostic], src: &Source) -> Result<(), Exit> {
    if diagnostics.is_empty() {
        return Ok(());
    }
    let rendered: Vec<String> = diagnostics.iter().map(|d| render(d, src)).collect();
    eprint!("{}", rendered.join("\n"));
    Err(Exit::Diagnostics)
}
