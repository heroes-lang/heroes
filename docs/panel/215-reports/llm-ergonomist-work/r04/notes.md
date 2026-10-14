# Notes

## All three
- **Name and tag:** each header names its struct only after the word `struct`, so I used a
  capitalised record name with `tag` (e.g. `SockPath tag sock_path`). A plain `record sock_path`
  with no `tag` is the other reading.
- **Struct passed by pointer:** each C function takes `struct X *`, so the parameter is
  `@p: X lent` and the program holds an `@=` cell. A by-value parameter (`p: X`) wouldn't match
  the header's pointer, and clang would likely refuse it. I added `lent` because C doesn't keep
  the pointer; without it, the call is taken to keep what it is handed.
- **Result width:** results are `i32`, the header's `int`. The spec allows a wider result
  (`i64`); the printed number would be the same.

## A1
- **How the path is filled:** I build with `rest: zero`, which zeroes `path`, then write the 16
  bytes in a loop through `to_i8().must()`. The other choice is a 108-element literal
  (`'/', 't', …, 0, 0, …`), which the spec's "as many elements as the type says" allows. Both
  give the same bytes.
- **`char` field:** declared `i8[108]`, because the spec says `i8` where C says char.
- Expected output: `16`.

## A2
- **Union fields:** I put the union's members (`fd: i32`, `handle: ptr`) flat in the record,
  reading "a record names one or more of each union" that way. Naming only one of them is also
  allowed; the result is the same. A separate record for `via` isn't possible, because the
  union has no tag.
- **Zeroing the union:** `rest: zero` with only `kind` named leaves the union zero. Naming
  `fd: 0` or `handle: nullptr` instead would also zero it, and fully only with `handle`, since
  `handle` is as wide as the union.
- Expected output: `2`.

## B
- **`partial` record:** `pthread_mutex_t` isn't a legal field type, so `Counter` is `partial`
  with only `n` declared. Its size stays C's.
- **Construction:** `Counter(n: 0, rest: zero)`. `Counter(rest: zero)` alone is the
  alternative.
- **Copy semantics:** `bump(@c: Counter)` is copy-in/copy-out. Lock and unlock happen on the
  same place inside `bump`, and the mutex is unlocked again before the copy goes back. Moving
  an unlocked default mutex works on glibc but isn't guaranteed by POSIX.
- **Return codes:** I discard the lock/unlock codes with `_ =` and check only `counter_init`.
  Checking them all would mean making `bump` return `()?` and propagating with `?`.
- Expected output: `3`.

context: yes. The harness's system prompt reached me: that I am a Claude agent on the Agent
SDK, the working directory (a `.claude/worktrees/readers/r04` path), the date 2026-10-14, and
the tool-use rules. Only a Read tool was available, so I could not write these files myself.
No memory or repository status reached me.
