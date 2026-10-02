# Panel 186: the blind seat's program under L, built on today's compiler

Run by the coordinator 2026-10-02 at 11:11, on the trunk's compiler
(`ae08ed93`'s seed, the tree frozen at `779139d0`, `selfhost/` unchanged
between them), in `<scratchpad>/p186/blind-run/` with `HEROES_RUNTIME` set,
`heroes build <file> -o <bin>` then the binary. The seat could not run it.

| file | the construction | `build` | prints |
|---|---|---|---|
| the seat's L program, verbatim from its `experiment` | `SA(kind: 2, i: 0, f: 1.5, x: 4)` | exit 0, 3 clang warnings: *excess elements in struct initializer* twice at `:2:91`, *initializer overrides prior initialization of this subobject* at `:13:36` | `12`, `1.5`, `4`, `true` |
| the same, `i: 7` for `i: 0` | `SA(kind: 2, i: 7, f: 1.5, x: 4)` | exit 0 | `12`, `1.5`, `4`, `true` |
| the same, the two labels swapped | `SA(kind: 2, f: 1.5, i: 0, x: 4)` | exit 1, `wrong_label` (*`SA` has nothing called `f` at this position, it is `i`*, fix a guess) and `type_mismatch` | |

So the write order the seat found unstated is settled by the compiler: labels
are held to declaration order, and the emitted designated initialiser writes
the fields in it, so the field declared LAST in the union wins. The seat's
program prints the meant `1.5` because the header declares `f` after `i`,
and the value given to `i` is discarded in silence (`i: 7` prints the same).
Then measured, 11:13: over `sa_rev.h`, the same struct with the union's
members in the other order (`union { float f; int32_t i; }`), the record
declaring `kind`, `f`, `i`, `x` and built `SA(kind: 2, f: 1.5, i: 0, x: 4)`
(`<scratchpad>/p186/blind-run/l_rev.hero`): `build` exit 0 with the same 3
warnings, and it prints `12`, **`0.0`**, `4`, `true`, exit 0. The same
program text, the header's member order swapped, prints a wrong value at
exit 0. The line's meaning depends on the order of a union's members in the
header, which is the seat's veto on L stated as a measurement.
