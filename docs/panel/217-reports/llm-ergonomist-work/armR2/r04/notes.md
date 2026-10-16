# Notes

- **How `doubled` receives the counter.** The compiler refused a by-value `Counter`
  (`ffi_lock_copied`) and suggested taking it by place. I chose `doubled(@c: Counter)`,
  called as `doubled(@s.first)`. Another option was to read `c.n` directly, which the
  compiler's note also offers. That would avoid the lend, but the task says to use
  `counter_value`, so I didn't.
- **Does `@c` in `doubled` count as written?** The spec makes an `@` parameter that
  nothing writes a compile error, but says an `@` argument counts. Passing `@c` to
  `counter_value` is one, so `doubled` is accepted. A by-value parameter would bring the
  copy error back.
- **`counter_value`'s `const struct counter *`.** I declared it `@c: Counter lent`.
  Nothing in the spec expresses const. A non-`@` `Counter` parameter would mean a copy,
  which is refused.
- **The local copy `view @= c`.** I removed it, because it was a second copy the compiler
  refused.
- **Building `Stats` with `Counter(rest: zero)` before `counter_init`.** I kept this. The
  compiler did not flag line 38, so I take building a zeroed, uninitialised record into
  the field as allowed. The alternative, building the counters separately and moving them
  into `Stats`, would copy them after init and be refused.
- **Failure handling.** I kept the `()?` results and `.must()` in `main`. Ignoring the
  return codes would mean dropping values, which the spec forbids for an `i32` anyway.

context: no. Nothing reached me besides the files in this folder and the brief's
instruction.
