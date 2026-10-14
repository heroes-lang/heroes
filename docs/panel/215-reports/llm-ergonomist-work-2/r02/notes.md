# Notes

## a2.hero
- `via` is an untagged union with no typedef, so there is no record type for it. The spec then says
  to leave it to `partial`: `record Tagged tag tagged partial` with only `kind`. `rest: zero` sets
  `via` to zero. Naming `via` would be refused.
- `const struct tagged *` is passed as `@t: Tagged lent`, with `t` as a `@=` cell. Leaving out `lent`
  would mean C may keep the place, and the lend would not apply.
- Result `int` is declared `i64`; a result may be wider than C's. `i32` would also work.
- Bare `extern "union.h"` with no `link`, because the functions are static inline.

## u.hero
- I declared the five fields exactly as tasks.md shows (`char[256]` → `i8[256]`), not `partial`.
  Linux's real struct has a sixth field (`domainname`) and 65-byte arrays, so clang would refuse this
  there. `partial` naming only `sysname` would survive the extra field, but not the width.
- Built with `Utsname(rest: zero)`, naming no field. Naming all five would need five 256-element
  literals.
- A non-zero `uname` result prints a message and exits 1. A bad `validated_bytes` prints `e.msg`;
  `.must()` would abort instead.

## c.hero
- `pthread_mutex_t` fits no field type, so `Counter` is `partial` with only `n`.
- `new_counter` returns `Counter?` by value. The other choice is an `@` out-parameter that initialises
  in place. Returning moves the initialised mutex; it is safe here because C keeps nothing
  (`lent`), but strict POSIX calls a copied mutex undefined.
- `doubled` takes the counter by value and lends a local copy to `counter_value`, which needs a
  pointer. Passing `@` the caller's counter would also leave it unchanged, but the signature would
  allow writes.
- Failures of init/lock/unlock become `T?` errors, and `main` handles them with `.must()` (abort).
  Ignoring the return codes is not allowed: they would be unused bindings.
- Loops use `while` with counter cells. I avoided `for _ in range(...)` because it is unclear whether
  `_` is an `ident` in `For`, and a named loop variable would be unused, which is an error.
- No `link "pthread"`: tasks.md says nothing else needs linking. Old glibc would need it.
- Expected output: 6, 4, 5.

context: yes — besides the files in this folder, I received a system prompt saying I am a Claude
agent built on the Claude Agent SDK, with a working directory path and the date (2026-10-14). I was
given only a file-reading tool, so I could not write these files myself. No repository status,
memory or other rules reached me.
