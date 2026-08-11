//! The ONE command (design.md §3.5 / the `zig` model): every capability is a
//! subcommand of `heroes`, never a second binary, script, or Makefile.
//!
//! This file does three things and nothing else: parse argv against the table in
//! `cli.rs`, dispatch on the `Tag`, and turn the result into the exit code the
//! contract promises. The dispatch is an exhaustive `match`, so a command added
//! to the table without an arm here does not compile — panel 016's condition, and
//! the reason this file is boring.

#![forbid(unsafe_code)]

mod cli;
mod commands;
mod input;

use std::process::ExitCode;

use cli::{Exit, Tag};

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let invocation = match cli::parse(&args) {
        Ok(invocation) => invocation,
        Err(message) => {
            eprintln!("error: {message}");
            return Exit::Failed.code();
        }
    };
    let file = invocation.file.clone().unwrap_or_default();
    let exit = match invocation.tag {
        Tag::Doctor => commands::doctor::run(),
        Tag::Lex => commands::lex::run(&file, &invocation),
        Tag::Measure => commands::measure::run(invocation.file.as_deref()),
        Tag::Mutate => commands::mutate::run(invocation.file.as_deref()),
        Tag::Parse => commands::parse::run(&file, &invocation),
        Tag::Check => commands::check::run(&file, &invocation),
        Tag::Build => commands::build::run(&file, &invocation),
        Tag::Run => commands::run::run(&file, &invocation),
        Tag::Test => commands::test::run(&file, &invocation),
        Tag::Fmt => commands::fmt::run(&file, &invocation),
        Tag::Version => {
            println!("heroes {}", heroes::VERSION);
            Exit::Ok
        }
        Tag::Help => {
            print!("{}", cli::help());
            Exit::Ok
        }
    };
    exit.code()
}
