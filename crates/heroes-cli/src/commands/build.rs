//! `heroes build <file.hero> [--dump-ir]` — the middle end (M4).
//!
//! **A `build` that exits 0 in silence was vetoed**, and by the judge whose job is
//! the burden of proof. Panel 016's watch list had already named the failure —
//! "`check` is not `build` … a model runs `heroes check` and reports success for a
//! program that has no backend yet" — and a `build` that produced nothing without
//! saying so would have promoted that misreading into the tool's behaviour, one
//! milestone before the fix was scheduled. So the no-flag form says what it did and
//! what does not exist yet, on **stderr**, leaving stdout for the artifact (panel
//! 016's stream contract).
//!
//! Four stages, each running only on the one before it having said nothing — the
//! rule `check` established, for the same reason: after a type error the checker's
//! answers are guesses, and lowering reads them as fact.
//!
//! The fourth stage is the **verifier**, and its failures are not diagnostics.
//! A violation says *the compiler* is wrong, so it exits 2 and speaks in the IR's
//! vocabulary — which is the only place, besides `--dump-ir`, that panel 019's R1
//! rule permits IR words at all.

use heroes::diagnostics::{render, Diagnostic};
use heroes::ir::{dump, lower, verify};
use heroes::resolve::resolve;
use heroes::source::Source;
use heroes::syntax::parse;
use heroes::types::{check, report_holes};

use crate::cli::{Exit, Invocation};
use crate::input;

pub fn run(path: &str, args: &Invocation) -> Exit {
    let src = match input::read(path) {
        Ok(src) => src,
        Err((message, exit)) => {
            eprintln!("error: {message}");
            return exit;
        }
    };
    let parsed = parse(&src);
    if let Some(exit) = report(&parsed.diagnostics, &src) {
        return exit;
    }
    let resolved = resolve(&parsed.ast, &src);
    if let Some(exit) = report(&resolved.diagnostics, &src) {
        return exit;
    }
    let checked = check(&parsed.ast, &resolved, &src);
    if let Some(exit) = report(&checked.diagnostics, &src) {
        return exit;
    }
    let lowered = lower(&parsed.ast, &resolved, &checked, &src);
    if let Some(exit) = report(&lowered.diagnostics, &src) {
        return exit;
    }
    // The IR verifier: GHC's Core Lint, on every function, every time. It is not
    // gated behind a flag because the cost is linear in the program and the thing
    // it catches is a wrong compiler.
    let problems = verify(&lowered.program, &checked);
    if !problems.is_empty() {
        eprintln!("internal error: the lowered program is not well formed");
        for problem in &problems {
            eprintln!("  {problem}");
        }
        return Exit::Failed;
    }
    if args.has("--dump-ir") {
        print!("{}", dump(&lowered.program, &parsed.ast, &checked, &src));
    }
    // §4.16: a hole is not an error. The program type-checks and lowers, and the
    // compiler says what belongs in the gaps — but no binary can come of it, which
    // is the sentence the stderr line below has to carry.
    let holes = !checked.holes.is_empty();
    if holes {
        print!("{}", report_holes(&parsed.ast, &resolved, &checked, &src));
    }
    if !args.has("--dump-ir") {
        eprintln!("{}", said(&lowered.program, holes));
    }
    Exit::Ok
}

/// What happened, in one line. It names the milestone rather than saying "not
/// implemented", because a model that reads "code generation lands at M5a" knows
/// the tool worked and the compiler is unfinished — and those are different facts.
fn said(program: &heroes::ir::Program, holes: bool) -> String {
    let functions = program.functions.len();
    let blocks: usize = program.functions.iter().map(|f| f.blocks.len()).sum();
    let unit = if functions == 1 { "function" } else { "functions" };
    let each = if blocks == 1 { "basic block" } else { "basic blocks" };
    let mut line =
        format!("lowered {functions} {unit}, {blocks} {each} — inspect with --dump-ir");
    if holes {
        line.push_str("; no binary while the file has holes");
    } else {
        line.push_str("; code generation lands at M5a");
    }
    line
}

/// The §4.17 form, on stderr, and `Exit::Diagnostics`. `build` takes no
/// diagnostic-shaping flags: `check` already answers every question about how to
/// print them, and two commands with two renderings of one diagnostic is the kind
/// of surface panel 016 exists to prevent.
fn report(diagnostics: &[Diagnostic], src: &Source) -> Option<Exit> {
    if diagnostics.is_empty() {
        return None;
    }
    let rendered: Vec<String> = diagnostics.iter().map(|d| render(d, src)).collect();
    eprint!("{}", rendered.join("\n"));
    Some(Exit::Diagnostics)
}
