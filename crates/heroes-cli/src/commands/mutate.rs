//! `heroes mutate [dir]` — metric 3, offline (design.md Part 11).
//!
//! Takes every program in the corpus that checks clean, makes **one plausible
//! mistake per site**, and counts how many the compiler catches — in two arms, so
//! the number means something. A language that rejects everything would score
//! 100% and prove nothing, which is why the control arm (`--permissive`, the same
//! compiler with the thesis rules dropped) is printed beside it.
//!
//! The corpus is the *clean* programs: `examples/gallery/` and `examples/`.
//! Mutating a program that is already wrong measures nothing, because the
//! compiler would reject the mutant for the mistake that was already there. The
//! acceptance program is included by the caller passing `design.md`'s fence — the
//! harness does that; this command's default is the directory.

use heroes::mutate::{report, run as run_operators};

use crate::cli::Exit;

fn corpus(dir: &str) -> Result<Vec<(String, String)>, String> {
    let mut sources: Vec<(String, String)> = Vec::new();
    collect(std::path::Path::new(dir), &mut sources)?;
    sources.sort();
    Ok(sources)
}

fn collect(
    dir: &std::path::Path,
    sources: &mut Vec<(String, String)>,
) -> Result<(), String> {
    let entries = std::fs::read_dir(dir).map_err(|e| format!("cannot read {}: {e}", dir.display()))?;
    for entry in entries {
        let path = entry.map_err(|e| format!("{e}"))?.path();
        if path.is_dir() {
            collect(&path, sources)?;
            continue;
        }
        if path.extension().and_then(|e| e.to_str()) != Some("hero") {
            continue;
        }
        let text = std::fs::read_to_string(&path)
            .map_err(|e| format!("cannot read {}: {e}", path.display()))?;
        sources.push((path.display().to_string(), text));
    }
    Ok(())
}

pub fn run(dir: Option<&str>) -> Exit {
    let dir = dir.unwrap_or("examples");
    let sources = match corpus(dir) {
        Ok(sources) => sources,
        Err(message) => {
            eprintln!("error: {message}");
            return Exit::Failed;
        }
    };
    if sources.is_empty() {
        eprintln!("error: no `.hero` files under {dir}");
        return Exit::Failed;
    }
    println!("corpus: {} programs under {dir}\n", sources.len());
    print!("{}", report(&run_operators(&sources)));
    Exit::Ok
}
