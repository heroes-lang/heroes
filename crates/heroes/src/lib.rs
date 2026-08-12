//! The Heroes compiler, as a library.
//!
//! design.md §3.3: "Write the compiler as a library with a thin CLI on top."
//! The `heroes` binary in `heroes-cli` is one client among several; a future
//! `heroes lsp` is another. Nothing in this crate reads argv or prints to a
//! terminal on its own initiative.
//!
//! Written in the "Heroes subset of Rust" — the Cyclone rule (see CLAUDE.md,
//! enforced by clippy.toml): references only as function parameters, owned data
//! everywhere else, `BTreeMap` only, no `Box`/`Rc`/`RefCell`, no stored closures.
//! Every necessary violation carries a `// PORT-DEBT:` marker.

#![forbid(unsafe_code)]

pub const VERSION: &str = env!("CARGO_PKG_VERSION");

pub mod diagnostics;
pub mod emit;
pub mod ir;
pub mod lexer;
pub mod library;
pub mod measure;
pub mod modules;
pub mod own;
pub mod mutate;
pub mod printer;
pub mod resolve;
pub mod source;
pub mod syntax;
pub mod types;

// Pipeline modules land one milestone at a time (see docs/ROADMAP.md):
//   source/ diagnostics/          M-token-stream (landed, step 1)
//   lexer/                        M-token-stream (landed, step 1)
//   syntax/ printer/              M-syntax-tree (landed)
//   resolve/                      M-name-resolution (landing)
//   types/                        M-checker-core (landing), M-data-declarations–M-rich-diagnostics
//   desugar/ ir/                  M-ir-lowering
//   own/                          M-strings-ownership (landing) · descriptors/  M-value-aggregates
//   emit/                         M-scalars-run (landing)
