# Heroes

A small compiled language designed so that **every plausible mistake an LLM
makes is a compile error**.

Not a language that tolerates a machine writing it — one whose surface is chosen
so that the wrong program does not compile, and the diagnostic arrives with the
repair already written. The bet is stated as a cost formula and measured rather
than asserted; where the measurement is missing, this file says so.

```
function main()
    scores: {str: int} = {"ziggy": 12, "aladdin": 9}
    for name in sort(keys(scores))
        print(name, " scored ", scores[name].must())
```

## Status: pre-v1

The language is finished and compiles itself only in the sense that the
*acceptance program* runs — a calculator with seven passing tests. It does not
yet compile its own compiler; that is the v1 finish line.

| working today | not yet |
|---|---|
| lexer, parser, formatter, resolver, bidirectional type checker | modules |
| three-address IR with basic blocks, ownership pass, C11 emission | the FFI (`extern` is refused by the backend until the header verifies it) |
| value semantics with copy-on-write, refcounted `str`/`[T]`/`{K: V}` | file I/O, `args()`, `exit(code)` |
| generics by monomorphisation, function values, `T?`, `test`/`assert` | self-hosting, and the fixpoint that defines v1 |

The chain from here is `docs/ROADMAP.md`. The identifiers in it are **not
chronological** — read the order from the table at the top of *What is left*.

## Build and try it

Requires a recent Rust toolchain and clang. Tested on macOS arm64; Linux is
expected to work and is **not yet verified by anything** (see the roadmap's CI
entry).

```sh
cargo build && cargo test        # build the compiler, run its tests
./target/debug/heroes doctor     # check the toolchain it needs
./target/debug/heroes run examples/gallery/00-first.hero
./target/debug/heroes test examples/calculator.hero
```

Every capability is a subcommand or a flag of the one binary — never a second
binary, never a script, never a Makefile. `heroes --help` prints the current
surface; `cargo` builds *the compiler* until the fixpoint, and that exception has
an expiry date written into it.

## The thesis, and how much of it is measured

The design rule is a cost formula: a construct's cost is its token count times
one plus the rate at which a model rewrites it wrongly. Two of its three
instruments have run — the spec's measured size (2363 tokens of a hard 4096
ceiling, counted by two vendored BPE tables so that neither can hide its own
drift) and a mutation-based check over the compiler's own corpus. **The third,
the rewrite rate, has not run**, so the formula remains the design rule it always
was and is not yet an audited one. This is written here rather than discovered by
a reader, because measurement beats opinion in this project — including the
author's.

## Where things are written down

| file | what it is |
|---|---|
| `design.md` | the source of truth for the language, and the reasoning behind it |
| `spec/heroes-spec.md` | the contract, budgeted at 4096 tokens — a control instrument, never a tutorial |
| `docs/ROADMAP.md` | the milestone chain, in execution order |
| `DESIGN-LOG.md` | every decision, dated, one line, with its reason |
| `docs/panel/` | the design reviews: five judges with differentiated inputs, their vetoes, and what lifted them |
| `docs/journal/` | one entry per milestone: what was built, what broke, and why |

Where two of them disagree: the spec beats the compiler (the compiler has the
bug), and measurement beats opinion.

## License

Apache-2.0 (`LICENSE`), with the **Heroes runtime exception**
(`LICENSE-RUNTIME-EXCEPTION`): a program you compile with Heroes contains part of
the C runtime and owes nothing for it — no copy of the license, no notice to
reproduce. Attribution is owed by whoever redistributes Heroes itself.

## Contributing

This is one person learning compilers, on a deliberately unusual set of rules —
so issues and questions are welcome, and pull requests are not being accepted
yet. The rules are not decoration: they are in `CLAUDE.md`, and the reason each
exists is in `design.md` or in a panel session.
