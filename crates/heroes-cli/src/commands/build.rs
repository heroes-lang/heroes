//! `heroes build <file.hero> [--dump-ir | --emit-c] [-o <path>]` — the verb that
//! produces a binary (M5a).
//!
//! At M4 this command lowered and said what it could not do yet; from M5a it
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

use super::compile::{compile, Options};

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
    let options = Options {
        // `-O0` for `build`: the dev loop's compile time, and the level at which
        // `-Werror=uninitialized` sees the emitter's own output most clearly. `run`
        // is where `-O2` lives.
        level: "-O0",
        dump_ir: args.has("--dump-ir"),
        emit_c: args.has("--emit-c"),
        output: args.value_of("-o"),
        sanitize: args.has("--sanitize"),
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
