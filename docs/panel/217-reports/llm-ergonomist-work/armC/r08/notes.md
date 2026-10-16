# Notes

- **`struct counter` as `record Counter tag counter partial`, naming only `n: i64`.** `pthread_mutex_t` can't be written as a field type, so `partial` leaves it out and C's size is kept. Listing every field instead would be refused, because the lock field has no type here. Leaving `n` out too would work for `counter_value`, but then `increment` couldn't write `c.n`.
- **Setting up "for its caller" is done in place: `new_counter(@c: Counter) -> ()?`.** Returning a `Counter` instead would move it (the spec says a return moves a value), which copies an initialised `pthread_mutex_t`. POSIX says that's undefined, though it often works in practice.
- **Every C pointer parameter is declared `lent`.** None of these functions keeps the pointer. Without `lent`, the parameter counts as keeping what it's given, and a lend can't reach it.
- **`counter_value` takes `c: Counter lent` without `@`.** This is C's `const struct counter *`. Using `@` would make it a writable pointer that doesn't match the `const` in the header.
- **Results are `i32`, matching C's `int`.** `i64` is also allowed, since a result may be wider. The behaviour would be the same.
- **Non-zero `int` results become `fail(...)`, and `main` calls `.must()`.** Ignoring them isn't allowed (a value has to be used). Matching on them instead would only change how an error is reported.
- **`link "pthread"`.** On glibc 2.34 or later and on macOS, the pthread functions are already in libc, so leaving it out would probably still link. On older glibc it would fail to link.
- **Constructed with `Counter(n: 0, rest: zero)` before `counter_init`.** The mutex bytes start as zero, then C initialises them. `n: 0` could be left out, because `rest: zero` covers it.
- **Loops use `while` with counters `i` and `j`.** The spec doesn't say whether `_` can be the loop variable of a `for`, and an unused named one is an error. A `for _ in range(from: 0, to: 3)` loop would be shorter if `_` is accepted there.
- **Events are counted in `main`, not inside `increment`.** Counting inside would mean `increment` takes the whole `@s: Stats` and a choice of which counter. The output would be the same.
- **The doubling overflow isn't checked.** `* 2` aborts on overflow, which can't happen with these values.

context: yes — besides the files in this folder, I got a system prompt saying I'm a Claude agent built on the Claude Agent SDK, with a working directory and the date (2026-10-16). It also gave me only a file-reading tool, so I couldn't write the files myself. No repository status or memories reached me.
