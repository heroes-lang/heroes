//! `heroes build <file.hero> [--dump-ir | --emit-c] [-o <path>]` — the verb that
//! produces a binary (M-scalars-run).
//!
//! At M-ir-lowering this command lowered and said what it could not do yet; from M-scalars-run it
//! compiles. What it says on success is candidate (ii) of the panel's experiment,
//! and it was chosen by measurement rather than taste: given `wrote build/<hash>/f`
//! the llm-ergonomist wrote `heroes build f.hero && ./build/<hash>/f` correctly on
//! the first try, and given **silence** it wrote `./f` — guaranteed wrong, because
//! the artifact lives under a hash it cannot predict. The function count a third
//! candidate carried ("compiled 1 function to …") was dropped: it is a fact the
//! reader cannot verify, and it made the reader stop and re-count.
//!
//! The line goes to **stderr** — the artifact is the binary, and stdout belongs to
//! `--emit-c`'s C (panel 016's stream contract).

use crate::cli::{Exit, Invocation};

use heroes::emit::Target;

use super::compile::{compile, level_from, Options};

pub fn run(path: &str, args: &Invocation) -> Exit {
    if args.has("--dump-ir") && args.has("--emit-c") {
        // Two stops in one invocation. Refusing is the strict reading, and it is
        // the one that cannot quietly answer a different question than the one
        // asked.
        eprintln!(
            "error: `--dump-ir` and `--emit-c` are both stops — ask for one artifact at a time"
        );
        return Exit::Failed;
    }
    // `-O0` is `build`'s default: the dev loop's compile time, and the level at
    // which `-Werror=uninitialized` sees the emitter's own output most clearly.
    // `-O2` is a flag away since 2026-08-12, which is what closed panel 021's
    // asymmetry — the level and the sanitisers are both flags now.
    let level = match level_from(args, "-O0") {
        Ok(level) => level,
        Err(exit) => return exit,
    };
    let options = Options {
        level,
        dump_ir: args.has("--dump-ir"),
        emit_c: args.has("--emit-c"),
        output: args.value_of("-o"),
        sanitize: args.has("--sanitize"),
        target: Target::Program,
    };
    match compile(path, &options) {
        Err(exit) => exit,
        Ok(None) => Exit::Ok,
        Ok(Some(binary)) => {
            eprintln!("wrote {}", binary.display());
            Exit::Ok
        }
    }
}
