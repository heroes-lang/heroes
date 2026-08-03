//! The Heroes compiler, as a library.
//!
//! design.md §3.3: "Write the compiler as a library with a thin CLI on top."
//! The `heroes` binary in `heroes-cli` is one client among several; a future
//! `heroes lsp` is another. Nothing in this crate reads argv or prints to a
//! terminal on its own initiative.
//!
//! Written in the "Heroes subset of Rust" — the Cyclone rule (CLAUDE.md rule 7,
//! enforced by clippy.toml): references only as function parameters, owned data
//! everywhere else, `BTreeMap` only, no `Box`/`Rc`/`RefCell`, no stored closures.
//! Every necessary violation carries a `// PORT-DEBT:` marker.

#![forbid(unsafe_code)]

pub const VERSION: &str = env!("CARGO_PKG_VERSION");

// Pipeline modules land one milestone at a time (see the approved plan):
//   source/ diagnostics/          M1
//   lexer/                        M1
//   syntax/ printer/              M2
//   resolve/ types/               M3a–M3d
//   desugar/ ir/                  M4
//   descriptors/ ownership/       M5b–M5c
//   backend/ driver/ api          M5a
