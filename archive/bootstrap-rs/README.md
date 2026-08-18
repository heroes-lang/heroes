# The bootstrap compiler — archived 2026-08-19

This is the Heroes compiler written in Rust: the one that existed before the
language could compile itself. It moved here from `crates/` at
**M-bootstrap-archive**, which is the milestone whose whole job was to make one
sentence in design.md true —

> the final picture is Heroes → C → native binary, with **no third language
> anywhere**.

It is kept, not deleted, for the reason every self-hosting project in panel 085's
sample kept theirs: it is the only independent implementation of this language
that has ever existed, and the record of five milestones is written against its
files. **It is not maintained.** Nothing in CI builds it, no test here is run any
more, and a change to `selfhost/` will not be reflected here.

## If you want to build it anyway

It is a normal Cargo workspace, self-contained apart from the repository around
it:

```sh
cd archive/bootstrap-rs
cargo build
```

**`cargo test` is a different matter, and the arithmetic is why.** Every test here
finds the repository as `env!("CARGO_MANIFEST_DIR")/../..`, which was the root when
this crate lived in `crates/heroes-cli/`. The archive is one directory deeper, so
two up is now `archive/` — the tests look for `spec/`, `docs/` and `tests/golden/`
inside it and do not find them. Nothing here is patched to fix that: the tests are
frozen with the compiler they test, and a frozen tree that gets edited is not
frozen. If you want them, the mechanical repair is `nth(2)` → `nth(3)` in each
`repo()`/`workspace_root()` helper, in a scratch copy.

The second thing to know is about what they would tell you even then:
`heroes-cli/tests/` compared this compiler against `selfhost/` **as it was on
2026-08-19**, so the differential and the goldens speak about that day's source
rather than today's.

## Where its paths went

Comments across `selfhost/` and `tests/harness/` say things like *"Port of
`crates/heroes/src/mutate/edits.rs`"*. Those are provenance — they name the
ancestor a file was ported from, and they are still accurate about what was read
when it was written. **Read every `crates/...` in this repository as
`archive/bootstrap-rs/...`**; that sentence is here so the same map does not have
to be written into four hundred comments (which is what a mechanical rewrite of
them would have been — a large diff carrying no new information).

The things that did *not* stay behind, because the archive would have made them
unobservable, each landed in the live tree first:

| what it was | where it is now |
|---|---|
| `heroes-cli/tests/differential.rs` — two compilers compared | `tests/emission/`, 142 blessed emissions + `tests/harness/suite_emission.hero` |
| `heroes/src/measure/` — the token counter | `selfhost/measure_{pieces,bpe}.hero`, `selfhost/cli_measure.hero` |
| `heroes/src/measure/gate.rs` — the §1.6 budget's gate | `tests/harness/suite_spec.hero` |
| `heroes/src/measure/gate.rs` — the SPEC_TOKENS **ledger** | `docs/measurements/010-spec-budget-ledger.md` (panel 086) |
| `heroes/src/measure/spec.rs` — spec vs compiler | `tests/harness/suite_spec.hero` |
| `heroes/src/mutate/` — metric 3 | `selfhost/mutate_*.hero`, `selfhost/cli_mutate.hero` |
| `heroes-cli/tests/{golden,corpus,surface,layout,milestones}.rs` | `tests/harness/` (M-harness-port) |
| `cargo build` for a newcomer | `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` |

## The one thing this directory is still evidence for

`seed/heroes.c` is what this compiler's successor emits for its own source, and
the two agreed byte for byte over 142 programs on the day of the move
(`tests/emission/`). That agreement is the only reason anybody can trust the seed
without reading 22 MB of C: it was checked against a second implementation while
one still ran.
