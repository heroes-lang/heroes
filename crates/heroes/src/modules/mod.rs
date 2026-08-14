//! Discovery: which files a compilation is made of, and in what order.
//!
//! `use geom` names `geom.hero` beside the file that names it (spec, "Files and
//! layout"). This module starts at the root file, finds its `use` lines, reads
//! what they name, and repeats — producing the ordered list `Source::of` turns
//! into one text. **It emits one whole-program `.c` from that**: separate
//! compilation is M-separate-compilation, and panel 030 R1 cut it out of M-module-namespace deliberately.
//!
//! **Discovery re-uses the real lexer** (panel 031 R8). The alternative was a
//! prefix scanner, licensed by a spec rule that `use` lines come first — and it
//! was refused, because a second grammar for the same text is two grammars that
//! can disagree, which is the failure CLAUDE.md §7 is written against. Lexing a
//! file twice is the price, and CLAUDE.md §13 forbids the compile-time argument
//! that would have bought the rule.
//!
//! **Discovery does no diagnosis.** A module that does not exist and a cycle
//! between modules are both reported by `graph.rs`, against the *finished*
//! `Source`, because that is the only text whose offsets a `Span` may point
//! into. Discovery's own job is to terminate and to be deterministic; it skips
//! what it cannot read and lets the graph pass say so, with a caret.
//!
//! | file | idea |
//! |---|---|
//! | `mod.rs` | reading the files, in load order |
//! | `graph.rs` | what is wrong with the set once it is one text |

mod graph;

pub use graph::errors;

use std::path::{Path, PathBuf};

use crate::lexer::{lex, TokenKind};
use crate::source::{stem_of, InputFile, Source};

/// The module names a file's `use` lines name, in source order.
///
/// Scanned from the token stream rather than from the tree: discovery runs
/// before there is a `Source` to parse against, and a `use` line is two tokens
/// whose shape the lexer already settles. A malformed `use` is skipped here and
/// diagnosed by the parser later, once — this function reports nothing.
pub fn uses_of(name: &str, text: &str) -> Vec<String> {
    let src = Source::new(name.to_string(), text.to_string());
    let out = lex(&src);
    let mut found = Vec::new();
    let mut previous_was_use = false;
    for token in &out.tokens {
        match token.kind {
            TokenKind::Comment => continue,
            TokenKind::KwUse => previous_was_use = true,
            TokenKind::Ident if previous_was_use => {
                found.push(src.slice(token.span).to_string());
                previous_was_use = false;
            }
            _ => previous_was_use = false,
        }
    }
    found
}

/// Every file of the compilation rooted at `path`, in load order, with the
/// library last.
///
/// **Depth-first in source order, each module loaded once.** The order is part
/// of the compiler's output — it decides line numbers in every diagnostic and
/// the order declarations reach the emitter — so it is fixed by the source text
/// and never by a filesystem listing or a hash iteration.
///
/// The `seen` set is what makes this terminate on a cycle; it is not the cycle
/// *check*, which needs spans and lives in `graph.rs`.
pub fn load(path: &str) -> Result<Source, String> {
    let root_text = read(path)?;
    Ok(load_text(path, root_text))
}

/// The root's own directory text — everything up to and including the last
/// separator the **author wrote**, or `""` for a bare file name.
///
/// A sibling module's name is built with this rather than through `Path::join`,
/// and the difference only shows on Windows: `join` inserts the platform's
/// preferred separator, so a root spelled `examples/calculator/main.hero` on the
/// command line produced sibling names spelled `examples\calculator\lex.hero`.
/// One output then named one directory in two ways — the root as the author typed
/// it, the modules as `Path` prefers them — and a diagnostic is read by tools as
/// well as people (§4.17, CLAUDE.md §8). Found by the third CI leg, 2026-08-14.
///
/// The filesystem is still asked through a real `Path`, which accepts either
/// separator on every platform this compiler builds for. This function decides
/// only what the *name* looks like, and it decides it from the value in hand:
/// this path's own text (CLAUDE.md §11).
fn directory_text(path: &str) -> &str {
    match path.rfind(['/', '\\']) {
        Some(at) => &path[..=at],
        None => "",
    }
}

/// The same, with the root's text supplied rather than read.
///
/// It exists for `heroes mutate`, which changes one byte of a file and asks the
/// frontend what it thinks — and has to ask about the *whole compilation*, or a
/// module file measures as "every mutant caught" by one unrelated diagnostic
/// about a `use` line nobody loaded.
pub fn load_text(path: &str, root_text: String) -> Source {
    let directory = PathBuf::from(path).parent().map(Path::to_path_buf).unwrap_or_default();
    // The RAW stem, the same rule `InputFile::user` follows and the same rule a
    // `use` line spells. Seeding this with the sanitised name made `use ab` in
    // `a_b.hero` look already-seen, so `ab.hero` was never loaded and the
    // compiler reported a cycle of one file with itself (sweep 001 audit L2).
    let root_module = stem_of(path);

    let mut files = vec![InputFile::user(path.to_string(), root_text)];
    let mut seen = vec![root_module];
    // The frontier, as (module, text) already loaded and not yet scanned. An
    // explicit stack rather than recursion: the port has no recursion limit to
    // reason about, and a 5–8k-line compiler's module graph is deep enough that
    // it matters more here than it looks.
    let mut pending = vec![0usize];
    while let Some(index) = pending.pop() {
        let named = uses_of(&files[index].name, &files[index].text);
        let mut added = Vec::new();
        for module in named {
            if seen.contains(&module) {
                continue;
            }
            let candidate = directory.join(format!("{module}.hero"));
            let Ok(text) = std::fs::read_to_string(&candidate) else {
                // Not an error here: `graph::errors` says so with the caret on
                // the `use` line, which is the file the author has open.
                seen.push(module);
                continue;
            };
            seen.push(module.clone());
            files.push(InputFile {
                name: format!("{}{module}.hero", directory_text(path)),
                module,
                text,
                is_library: false,
            });
            added.push(files.len() - 1);
        }
        // `files` is in source order; `pending` is reversed so that popping it
        // scans in source order too. Both orders are output, not bookkeeping.
        for new in added.into_iter().rev() {
            pending.push(new);
        }
    }

    files.push(InputFile {
        name: crate::source::LIBRARY_FILE.to_string(),
        module: crate::source::LIBRARY_MODULE.to_string(),
        text: crate::library::SOURCE.to_string(),
        is_library: true,
    });
    Source::of(files)
}

fn read(path: &str) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("cannot read `{path}`: {e}"))
}

#[cfg(test)]
mod tests;
