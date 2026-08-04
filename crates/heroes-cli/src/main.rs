//! The ONE command (plan §3.5 / the `zig` model): every capability is a
//! subcommand of `heroes`, never a second binary, script, or Makefile.

#![forbid(unsafe_code)]

mod commands;

use std::process::ExitCode;

const USAGE: &str = "\
heroes — the Heroes compiler

usage: heroes <command> [args]

commands:
  doctor                    check the toolchain (clang, CLT, arch, cache)
  lex <file.hero> [--json]  dump the token stream
  measure [file]            count the spec against §1.6's ceiling
  parse <file.hero> [--dump-ast]
                            parse the file; with the flag, print the tree
  --version                 print the compiler version

More subcommands arrive with each milestone:
  fmt (M2) · check (M3) · build run (M5) · test (M6)
  lsp outline explain (M6+) · cc doc (M7)";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match args.first().map(String::as_str) {
        Some("doctor") => commands::doctor::run(),
        Some("lex") => commands::lex::run(&args[1..]),
        Some("measure") => commands::measure::run(&args[1..]),
        Some("parse") => commands::parse::run(&args[1..]),
        Some("--version") | Some("-V") => {
            println!("heroes {}", heroes::VERSION);
            ExitCode::SUCCESS
        }
        Some(cmd) => {
            eprintln!("error: unknown command `{cmd}`\n\n{USAGE}");
            ExitCode::FAILURE
        }
        None => {
            eprintln!("{USAGE}");
            ExitCode::FAILURE
        }
    }
}
