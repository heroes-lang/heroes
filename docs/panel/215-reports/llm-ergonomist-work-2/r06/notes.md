# Notes

## a2.hero
- **The union `via`.** It is a named member whose union type has no typedef, so §13 says it is
  "left to `partial`". I wrote `record Tagged tag tagged partial` naming only `kind`, and
  `rest: zero` zeroes `via`. Naming `via` would need a record for the union's typedef, and there
  isn't one. Naming `fd`/`handle` directly is wrong because C reads `t.via.fd`, not `t.fd`.
- **Name and `tag`.** The header writes `struct tagged`, so I used `tag tagged` with my own name
  `Tagged`. Without `tag` the record would have to be called `tagged`.
- **How the pointer is passed.** `tg_kind` takes `const struct tagged *`, so the parameter is `@t`
  (C gets the place) and the value is a cell. A plain `t: Tagged` would pass the struct by value,
  which doesn't match the header and would be refused.
- **`lent`.** C doesn't keep the pointer. Without `lent` the parameter counts as keeping what it is
  handed, and a lend can't reach it.
- **Result width.** I used `-> i64` (a result may be wider than C's). `-> i32` prints the same thing.

## u.hero
- **Fields.** I declared all five `char[256]` fields as `i8[256]` (char → i8), as the task gives
  them, with no `partial`. Another choice is `partial` naming only `sysname`. That still compiles
  if the real header has more fields (glibc adds a domainname field), but glibc's 65-byte arrays
  would be refused either way.
- **Construction.** `Utsname(rest: zero)` names no field and zeroes everything. Without
  `rest: zero` I would have to write five 256-element literals.
- **`@u` and `lent`.** This is a C out-parameter, so it must be an `@` cell for C's write to come
  back.
- **Text.** `validated_bytes()` reads up to the first zero and fails with `not_text`. I print the
  failure instead of using `.must()`, which would abort.
- **`uname` failing.** I print a message and `exit(code: 1)`. Ignoring `rc` would print an empty
  name.

## c.hero
- **The mutex.** `pthread_mutex_t` isn't a type a field can have, so the record is `partial` and
  names only `n: i64`. Its size stays C's.
- **"Sets up a new counter for its caller".** I did this through `@c`, so `counter_init` runs on
  the caller's own place (`s.first`) and the mutex never moves after it is initialised. The other
  choice, returning a `Counter?`, moves the initialised mutex on the return (§13), which is
  formally undefined behaviour in pthreads.
- **Doubled value without changing the counter.** It takes `c: Counter` by value, copies it into a
  cell and lends that copy to `counter_value`. The caller's counter can't change. An `@c` version
  would avoid the copy but would mark the counter as mutable.
- **Errors.** Each C call's `int` result is checked and turned into a `fail`. These go up through
  `run() -> ()?` to `main`, which prints and exits 1. `.must()` would be shorter but aborts.
- **Loops.** `for _ in range(...)`: I assumed `_` is allowed as the loop variable. A named
  variable would be unused, which is an error.
- **Linking.** No `link "pthread"`, per the task. On older glibc you'd need it.
- **Event count.** It is `s.events`, raised after each successful increment. Expected output:
  `6`, `4`, `5`.

context: yes. Besides the folder's files, I received a system prompt saying I am a Claude agent
built on the Claude Agent SDK. It gave a working directory (a worktree path ending
`readers2/r06`), the date 2026-10-14, and tool instructions. My only tool could read files, not
write them. No other rules, memories or repository status reached me.
