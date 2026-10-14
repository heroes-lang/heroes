# Notes

## a2.hero
- `via` is an anonymous union with no typedef, so the spec's only option is to leave it to
  `partial`. The record names only `kind`. `rest: zero` zeroes `via`, which shares no byte with `kind`.
- `const struct tagged *` is passed as `@t` (the place itself), since a record has no other way
  to reach a pointer parameter. Marked `lent` because `tg_kind` does not keep it. Without `lent`,
  the parameter is taken to keep the place: that is allowed, but promises more than is true.
- The result is declared `i32`, C's own width. `i64` (wider) is also allowed and prints the same `2`.

## u.hero
- `partial` with only `sysname`, not all five fields. Listing all five would match the header shown,
  but a real header with extra fields (Linux's domainname) would be refused. `partial` keeps C's size.
- `char[256]` is declared `i8[256]`, following "i8 where it says char". `u8` would be refused as a
  sign mismatch. I assumed `validated_bytes()` accepts an `i8` field as "a field of bytes".
- The width 256 comes from the header given. On a system whose header says 65 (Linux), clang refuses it.
- `uname` gets `@u lent`. A non-zero return or a non-text name prints a message and exits 1.
  The other option was `.must()`, which aborts.

## c.hero
- `pthread_mutex_t` is not a record of the group, so `Counter` is `partial` with only `n`.
  A consequence: `Counter`, and `Stats` because it holds one, can never be compared or used as a map key.
- "Sets up a new counter for its caller" is `setup(@c: Counter)`, which initialises the caller's
  own place. Returning a `Counter` instead would move the initialised mutex (a return moves it),
  which POSIX leaves undefined.
- `doubled` takes the counter by value and copies it into a local cell for `counter_value(@view)`.
  That guarantees the caller's counter is unchanged, but copies the mutex bytes; `counter_value`
  never touches the lock. The other option, `doubled(@c: Counter)`, copies nothing but marks the
  counter as writable at every call site.
- Failure from C (non-zero) prints and exits 1. The other options were `assert rc == 0` (abort
  showing the expression) or functions returning `()?`.
- The five increments are written out. A `for _ in range(...)` loop depends on `_` being accepted
  as a loop variable, which the grammar (`for ident`) leaves unclear.
- No `link` clause: the functions are `static inline`, and pthread is assumed to be in libc.

context: yes — an agent system prompt (identity as a Claude agent, the working directory path,
the date 2026-10-14) and a tool set that could read files but not write them. No other
instruction, rule, memory or repository status reached me.
