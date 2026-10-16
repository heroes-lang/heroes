# Notes

- **How `struct counter` is declared:** `record Counter tag counter partial` with only `n: i64`. `pthread_mutex_t` isn't a type the language can name, so `partial` leaves it out and the record keeps C's size. Listing every field isn't possible; leaving out `partial` would be refused.
- **"Sets up a new counter for its caller":** `setup(@c: Counter)` runs `counter_init` on the caller's own place. The other option, `-> Counter` returning a value set up inside the function, would move the mutex after `pthread_mutex_init`. The spec says a return moves the place, and a moved mutex is undefined behaviour.
- **Starting value before `counter_init`:** `Counter(n: 0, rest: zero)`, because every binding must start with a value. `Counter(rest: zero)` alone might also be accepted; I named `n` to be safe.
- **`lent` on the C parameters:** none of the C functions keeps the pointer, so all are declared `lent`. Without `lent`, C may keep the pointer as long as the place doesn't move, which is also safe here since `s` is never copied.
- **`counter_value`:** takes `c: Counter lent` without `@`, which the spec defines as a pointer to const, matching the header's `const struct counter *`. Writing `@` would wrongly allow C to write to it.
- **`doubled` takes `Counter lent`:** a plain parameter would copy the counter, mutex included.
- **Result width:** `int` results are declared `i32`, exactly as C has them. `i64` is also allowed, because a result may be wider than C's.
- **Result checks:** each `int` result is checked with `assert ... == 0`. The other option, `_ = ...`, would silently ignore a lock failure.
- **Linking:** I added `link "pthread"`. It's harmless where the pthread functions are already in libc, and leaving it out could fail to link on older glibc. The header's functions are `static inline`; I assumed the compiler compiles the header, so calling them works.
- **Loops:** I used `while` with counter cells instead of `for _ in range(...)`, because the grammar's `For` takes an `ident` and it's unclear whether `_` counts as one. A named loop variable that isn't used would be a compile error.
- **Printing:** three `print` calls, one value each, to get one value per line.

context: yes. Besides the files in this folder, the system setup reached me: I'm a Claude agent built on the Agent SDK, working in this scratchpad directory, with the date given as 2026-10-16. The path names "heroes-lang" and "llm-ergonomist". It also said to answer in English and to make independent tool calls together. No other rules, memories or repository status reached me.
