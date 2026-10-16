# Notes

- **`Counter` is `partial`, naming only `n`.** The header's `pthread_mutex_t lock` can't be written as a field type. Listing every field instead: refused, since `lock` can't be typed.
- **`tag counter`.** The header names the struct only after the word `struct`. Without `tag`, the compiler would look for a type called `Counter` and not find it.
- **Construction uses `rest: zero`.** This zeroes the mutex bytes the record leaves out. Without it, building a `partial` record may be refused, or the bytes not named are left undefined.
- **C parameters are `@c: Counter lent`.** C works through the pointer during the call and doesn't keep it. Without `lent`, C is assumed to keep it, and lending a field (`@stats.first`) would be refused.
- **`counter_value` takes `c: Counter lent`.** Per §13, this is a pointer to const, which matches `const struct counter *`. Without `lent`, the record would be passed by value, which the header refuses.
- **Results declared `i32`.** This is C's `int`. `i64` would also be accepted, because a result may be wider than C's.
- **"Sets up a new counter for its caller" means initialising in place** (`init_counter(@c)`). The other choice is to return a `Counter`. That moves the struct after `pthread_mutex_init`, and the spec says a return moves the place. It would probably work for a fresh unlocked mutex, but POSIX doesn't promise it.
- **Nonzero results from `counter_init`/`lock`/`unlock` become `fail`, and `main` calls `.must()`.** The other choice is `_ = counter_lock(@c)`, which ignores the error silently.
- **Loops are `while` with an index cell.** `for _ in range(...)` depends on whether `_` counts as an `ident` in `For`, which the spec doesn't settle. `for i in range` with an unused `i` is a compile error.
- **`link "pthread"`.** It's harmless on systems where pthread is part of libc. Leaving it out could fail to link on older glibc.
- **The mutexes are never destroyed.** The header has no destroy function. Adding a `pthread_mutex_destroy` member would need `pthread.h` and the `lock` field, which isn't named.

context: yes — besides the folder's files, I received a system prompt saying I am a Claude agent on the Agent SDK, with a working directory, a date (2026-10-16), and the instruction to batch independent tool calls. The working-directory path names a scratchpad for "llm-ergonomist/armC/r05" in a "heroes-lang" repo. No memory or repository status reached me. My only tool was Read, so these files are given in my reply rather than written to disk.
