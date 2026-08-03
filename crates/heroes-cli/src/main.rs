//! The ONE command (plan §3.5 / the `zig` model): every capability is a
//! subcommand of `heroes`, never a second binary, script, or Makefile.

#![forbid(unsafe_code)]

mod commands;

use std::process::ExitCode;

const USAGE: &str = "\
heroes — the Heroes compiler

usage: heroes <command> [args]

commands:
  doctor      check the toolchain (clang, CLT, arch, cache)
  --version   print the compiler version

More subcommands arrive with each milestone:
  lex parse (M1–M2) · fmt (M2) · check (M3) · build run (M5) · test (M6)
  lsp outline explain (M6+) · cc doc (M7) · measure (M3)";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match args.first().map(String::as_str) {
        Some("doctor") => commands::doctor::run(),
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
