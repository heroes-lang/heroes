//! `heroes --help`: the table, plus the contract (panel 016).
//!
//! The exit codes are printed because a wrapper script has nowhere else to read
//! them, and `$HEROES_RUNTIME` is printed because it is the one input channel
//! outside the argv table — a channel nobody can discover is a channel that will be
//! discovered by reading the source.

use super::{commands, Operand};

pub fn help() -> String {
    let mut out = String::from(
        "heroes — the Heroes compiler\n\nusage: heroes <command> [file] [flags]\n\n",
    );
    for command in commands() {
        let operand = match command.operand {
            Operand::None => "",
            Operand::File => " <file.hero>",
            Operand::Optional => " [file]",
        };
        out.push_str(&format!("  {}{}\n      {}\n", command.name, operand, command.summary));
        for flag in &command.flags {
            let spelling =
                if flag.value { format!("{} <path>", flag.spelling) } else { flag.spelling.clone() };
            out.push_str(&format!("      {spelling}  {}\n", flag.what));
        }
    }
    out.push_str(
        "\n  --version · --help\n\n\
         streams: the artifact on stdout, diagnostics and progress on stderr.\n\
         exit:    0 nothing to report · 1 diagnostics were reported, no artifact was produced\n\
         \x20        2 the tool could not run. `run` forwards the program's own status.\n\
         env:     HEROES_RUNTIME  where heroes_runtime.h and runtime.c live, if not\n\
         \x20        under the working directory or beside the compiler.\n\n\
         More commands arrive with each milestone:\n\
         lsp outline explain (M6+) · cc doc (M7)\n",
    );
    out
}
