# Panel 196, compiler-engineer

Read `00-shared.md` first, then defect 396's issue file, panel 194's sitting
(`docs/panel/194-*.md`, its route C for defect 092 above all) and
`.claude/rules/c-boundary.md`, in your copy.

Your seat judges the ceiling (design.md §1.1, §1.7, Part 5): implementation
cost, core against sugar, and what each route does to the compiler's own code.
You hold a veto on soundness.

1. **Where an `@` lend is read today**: the modules that check an `extern`
   parameter's pointee against the header (the pointee probes under
   `selfhost/cli/` and `selfhost/emit/`), the ones that lower an `@` argument
   (a local, a field, an element), and route C's `counted_by` check as lane
   unit landed it (`selfhost/emit/ffi_unit.hero`, `lend_count.hero`). Cite
   files and lines; count lines with the unit `tests/harness/suite_layout.hero`
   counts.
2. **Build the routes S1 and S2 in your copy**, far enough that `check`, `build`
   and `run` judge `docs/panel/196-evidence/fixed_param.hero` (S1) and a
   `counted_by 32` variant of `field_ptr.hero` (S2): what each costs in lines,
   which diagnostics it adds or changes, and whether the emitted C passes the
   field's address and nothing else. A route that does not build is reported
   as such; it is not adopted.
3. **S4 and S5**: say what each would cost in the emitter and the header
   probes, and measure what you can (S5: does the header probe see an array
   parameter's declared length through clang today?).
4. **The compiler's own seven lends** (`docs/panel/196-evidence/scalar-lends.txt`):
   does any route change them? Each must still build the compiler; a route
   that changes `selfhost/` says how the seed's fixpoint is kept.

5. **S7 and S0, added after the critic's first pass**: prototype S7 (`@md:
   [u8]` with a stated extent, passed through a C temporary of exactly N
   bytes, copied in and out by the emitter) far enough that `SHA256_Final`
   runs from one `.hero` file with no header of your own, and say what S0 (a
   local fixed array, or a `[u8]` lent through `.ptr()` with a run-time length
   check) costs. Then: what does each route do with `pipe`, `gethostname` and
   `EVP_DigestFinal_ex`'s out-count (`docs/panel/196-evidence/critic/`)?
6. **The default when a binding says nothing**: what refusing an unmarked
   one-cell `@` lend to a typed pointer would cost (the 34 live lends, 12 of
   them the compiler's own), and what the marked spelling of *one element*
   would be.

Verdict per route: approve, object or veto, with its section, its cost, a
falsifiable prediction and its condition. Report:
`<scratchpad>/p196/reports/compiler-engineer.md`, written as you go.
