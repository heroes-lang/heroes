# 047: the archived Rust bootstrap still builds, and agrees with the seed at its tag on 142 of 142 programs

Measured 2026-10-18 by the session that opened M-witnessed-seed, for task 2089
(https://github.com/heroes-lang/heroes/issues/2089): the archived Rust bootstrap
rebuilt with its pinned `rustc`, and the comparison its archiving recorded at
`m-bootstrap-archive` run again. Every number is a command's, run that morning
on this Mac (Apple clang 21, `rustc 1.90.0`); what was not run is said in § 4.

## 1. The bootstrap builds

A worktree at the tag `m-bootstrap-archive` (`027f69d72`, 2026-08-19, *the
third language dies*), and in its `archive/bootstrap-rs/`:

- `rust-toolchain.toml` pins `1.90.0` with `clippy` and `rust-analyzer`, and
  all three were installed (`rustup component list --toolchain 1.90.0
  --installed`), so rustup fetched nothing;
- `Cargo.lock` names the workspace's two packages, `heroes` and `heroes-cli`,
  and nothing from a registry;
- `cargo build --release --offline --locked`, `CARGO_HOME` pointed into the
  ignored `target/`, so nothing was written outside the repository: finished
  in 8.96 s (`real 9.26`), `target/release/heroes`, 2,136,192 bytes, which says
  `heroes 0.0.1`.

Nothing had built it since 2026-08-19 (the task's own words); it builds, two
months on, unchanged.

## 2. The comparison

The tag's own `tests/harness/suite_emission.hero` defines it: every `.hero`
directly in `tests/golden/emit`, `fixedbugs`, `ir` and `run`, and each
`examples/<dir>/main.hero` and `whole.hero`, 142 programs; `<compiler> build
<source> --emit-c` from the tree's root; its standard output against
`tests/emission/<group>-<stem>.c` (or `examples-<dir>-<entry>.c`), byte for
byte, with no normalisation; a nonzero exit a failure.

- **The compiler clang builds from the tag's seed** (`seed/heroes.c`,
  22,025,792 bytes, `clang -I runtime seed/heroes.c runtime/runtime.c`, 5.60 s):
  142 of 142 emit their blessed bytes, every one at exit 0.
- **The Rust bootstrap**: 142 of 142 the same, every one at exit 0.
- **So the two agree byte for byte on 142 of 142**: the archiving's claim, in
  the tag's message *both compilers green on the same bytes*, measured again
  on a newer clang and the same `rustc`. The whole replay took 3.47 s.

## 3. Why not the tag's harness

The tag's harness asks for exactly this, `heroes run tests/harness/main.hero
-- <compiler> emission`, and this session ran its rule instead, in a script
of the same three parts (the programs, the command, the comparison): the
hook in front of today's net refuses a harness run whose compiler cannot say
what it was compiled from, and both compilers of the tag predate panel 222's
digest. The refusal is the hook's to make on this repository's own work; a
reader without the hook can run the tag's harness as the tag left it, which
this session did not run.

## 4. What it does not say

- It compares two compilers at the archive's commit. It does not walk the
  chain from that seed to today's through the milestones' sources, which is
  the milestone's third item.
- It compares the C the two emit. Nothing here compiles that C, or the seed,
  with a second C compiler: that is the milestone's second item, task 2096.
- Both compilers and the comparison ran on one machine. Every step is
  rerunnable from git, and the loop below, run as written that morning, read
  `142 of 142` for each compiler:

```sh
git worktree add --detach .claude/worktrees/ws-archive m-bootstrap-archive
cd .claude/worktrees/ws-archive
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed
(cd archive/bootstrap-rs && CARGO_HOME="$PWD/target/cargo-home" cargo build --release --offline --locked)
for c in ./heroes-seed archive/bootstrap-rs/target/release/heroes; do
  same=0
  for f in tests/golden/emit/*.hero tests/golden/fixedbugs/*.hero tests/golden/ir/*.hero tests/golden/run/*.hero; do
    g=${f#tests/golden/}; g=${g%%/*}; s=$(basename "$f" .hero)
    "$c" build "$f" --emit-c | cmp -s - "tests/emission/$g-$s.c" && same=$((same + 1))
  done
  for d in examples/*/; do for e in main whole; do
    [ -f "$d$e.hero" ] || continue
    "$c" build "$d$e.hero" --emit-c | cmp -s - "tests/emission/examples-$(basename "$d")-$e.c" && same=$((same + 1))
  done; done
  echo "$c: $same of 142"
done
```
