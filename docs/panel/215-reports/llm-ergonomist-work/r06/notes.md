# Notes

## a1
- Record name: `SockPath tag sock_path`, because the C name only appears after `struct`. Without the tag, the record would have to be called `sock_path`.
- `char path[108]` becomes `i8[108]`, as the spec says for char; `u8` would be refused.
- Building the path: start with `rest: zero` and write the bytes in a loop. The alternative is a literal with exactly 108 elements (16 characters plus 92 zeros): same bytes, much longer.
- Passing `const struct sock_path *`: an `@ ... lent` parameter, the only way to pass a struct by pointer. Since `p` must then be a cell, the `@` argument counts as the write it needs. Without `lent`, C is taken to keep the place, and lending to it is refused.
- Result: `i32`, as C has it. `i64` is also allowed (a result may be wider) and prints the same 16.

## a2
- Union members `fd` and `handle` are written as plain fields of the record. The grammar has no way to write `via.fd`, and the spec says "named or anonymous". The other choice was a `partial` record naming only `kind`, which avoids the union entirely but leaves unclear whether `rest: zero` covers the unnamed bytes.
- Built naming no union member, ending with `rest: zero`, which the spec allows. Naming `fd: 0` gives the same bytes on 32-bit only; on 64-bit the upper half of `handle` would not be guaranteed zero.
- `@t: Tagged lent` and the `i32` result: same reasoning as a1. Prints 2.

## b
- `pthread_mutex_t` has no Heroes type, so the record is `partial` and names only `n: i64`; its size stays C's. Listing every field is impossible.
- Built with `n: 0, rest: zero`, then `counter_init`.
- Every C call is an `@ ... lent` parameter: none of them keeps the pointer. Without `lent`, each lend would be refused.
- Return codes are checked: `increment` returns `()?` with `lock_failed` or `unlock_failed`, and `main` calls `.must()`. The other choice, `_ = counter_lock(@c)`, is shorter but ignores errors.
- Risk: an `@` parameter is copy-in/copy-out, so `increment` locks and unlocks a copy of the mutex, which then moves back. Lock and unlock hit the same copy and nothing keeps its address, so this works in practice. But POSIX does not promise that a copied mutex is valid. Putting the lock calls inline in `main` would avoid the move, but the task asks for a separate function.
- Expected output: 3.

context: yes — my environment told me the working directory (a worktree path), the date (2026-10-14), that I am a Claude agent built on the Agent SDK, and how to call tools. Only a file-reading tool was available, so I could not write files. No other instructions, rules, memories or repository status reached me.
