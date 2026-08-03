# Panel 004 — The tool is one command (RETRO-RECORD)

Status: **decided by the author, 2026-08-03.**

## Decision

One executable, `heroes`, with subcommands — the `zig`/`cargo` model. Never a
second binary, never a script, never a Makefile. The formatter, the test
runner, the LSP, the measurement harness, the future package operations are
all subcommands. Inspection is flags (`--dump-ir`, `--emit-c`, `--pipeline`),
not subcommands.

## Grounds

- Direct corollary of §3.3 ("the compiler must be a library with a thin CLI on
  top"): one library, one CLI, many clients of the library.
- The author's explicit requirement: "voglio un comando unico per far tutto
  come in zig" (recorded here in the original Italian; artifacts are English).
- Anti-precedent: Nim's `nimble` as a separate binary is the fragmentation to
  avoid.

## Declared exception, with expiry

`cargo build` / `cargo test` build *the compiler* until the self-compilation
fixpoint (M8c); after that `heroes` is the only command for compiler
development too.

## Consequences applied

design.md gains §3.5 ("The tool is one command"). CLAUDE.md rule 10 enforces
it on every future capability.
