# selfhost/ — the Heroes compiler, in Heroes

Empty until M8b. It exists from day one as the visible reminder of where this
project is going (design.md Part 0, §1.0).

## The acceptance test (M8c): the bootstrap fixpoint

```
stage0  crates/heroes (Rust)  --builds-->  A
        A  compiles selfhost/heroes.hero  --emit-c-->  B.c  --clang-->  B
        B  compiles selfhost/heroes.hero  --emit-c-->  C.c

PASS  ⇔  diff B.c C.c is empty (byte-identical generated C;
          clang version pinned and recorded in the run artifact)
```

The diff is on generated C, not Mach-O binaries — binary identity additionally
depends on clang/ld noise (`LC_UUID`, DWARF paths, mtimes), which is not
signal. Determinism is not discovered here: the double-emit diff is a CI
invariant from M5a onward.

When the fixpoint holds, `crates/heroes` moves to `archive/bootstrap-rs/` and
is never maintained again. From that day the picture is Heroes → C → binary,
no third language anywhere — and `heroes` is the only command for compiler
development too.
