# Notes

- **How to declare `struct counter`:** I used `record Counter tag counter partial` and named only `n: i64`.
  `pthread_mutex_t` isn't a type a field can have (number, bool, ptr, cstr, a group record, or a fixed array of one), so `lock` stays hidden under `partial` and the record keeps C's size.
  The other options were to list `lock`, which is refused, or to use a handle (`tag counter` with no fields). A handle would be a `struct counter *` that nothing in this header can allocate.
- **How the counter is set up "for its caller":** `new_counter(@c: Counter)` initialises the caller's own place.
  The other option was `-> Counter`. Returning moves the struct, and then the record construction moves it again, so an already-initialised `pthread_mutex_t` would be copied. POSIX says that is undefined.
- **`lent` on the C parameters:** I marked them `@c: Counter lent`, because C doesn't keep the pointer. Without `lent` the compiler would assume C keeps the place, and a lend could not reach it.
  For `counter_value` I used `c: Counter lent` with no `@`, which is a pointer to const and matches `const struct counter *`.
- **Result width:** `int` is declared `i32`. `i64` would also be allowed, because a result may be wider than C's.
- **Handling the `int` return codes:** a non-zero code becomes a `()?` failure, and `main` calls `.must()`, so the program aborts with the code.
  `assert rc == 0` would also abort. Ignoring the codes isn't possible, because a value must be used.
- **Initial bytes:** every `Counter(...)` construction ends with `rest: zero`, so the mutex bytes start zeroed rather than unspecified.
- **Loops:** I used `while` with counter cells `i` and `j`. `for i in range(...)` would leave `i` unused, which is a compile error. `for _ in` might work, but the `For` grammar asks for an `ident`, so I didn't rely on it. Writing the five calls out by hand would also work.
- **No `link` clause:** pthread lives in libc on macOS and on current glibc. Older glibc would need `link "pthread"`.
- **Static inline functions:** the header defines them `static inline`. I assumed the compiler, which reads the header through clang, can call them. If it needs exported symbols, they would have to be compiled into a small C file.
- **No `pthread_mutex_destroy`:** the header provides no destroy function, so the mutexes are never destroyed. This is harmless because the program exits right away.
- **Finding the header:** I assumed `"counter.h"` is found next to the source file.

context: yes. Besides the files in this folder, I received a system prompt. It said I am a Claude agent built on the Claude Agent SDK, and it gave the working directory path, the date (2026-10-16), the instruction to make independent tool calls together, and a tool list containing only Read. No repository status, rules or memories reached me.
