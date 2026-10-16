# Notes

- **`doubled` takes its counter by place.** The compiler refused every copy of a
  `Counter`, because it holds a `pthread_mutex_t`. I followed its "fix (guess)":
  `doubled(@c: Counter)`, called as `doubled(@s.first)`. It hands `@c` straight to
  `counter_value`, and that `@` argument is what counts as using the `@` parameter
  (spec section 5). The other choice was reading `c.n` directly instead of calling
  `counter_value`. That also compiles, but it breaks the task's rule to "use
  `counter_value`". Any by-value parameter or local copy (`view @= c`) gets the same
  `ffi_lock_copied` error again.
- **`counter_value` is still declared `@c: Counter lent`**, even though C takes a
  `const` pointer. Declaring it `c: Counter lent` would pass by value, which copies
  the lock and is refused.
- **`@` with no write behind it.** The `@` in `doubled` only lends the place; nothing
  writes to it. The spec has no read-only place lend, so `@` was the only option.
- **The counter is set up in the caller's place** (`init_counter(@s.first)`), not
  returned. Returning a `Counter` would move a mutex that is already initialised,
  and the compiler would see that as a copy.
- **Partial record and how it's built.** `Counter` is declared `partial` because a
  `pthread_mutex_t` field has no Heroes type. It is built with `Counter(rest: zero)`,
  then `counter_init` sets it up. The compiler didn't flag either one before.
- **What C returns.** C's return codes come back as `T?` failures and `main` prints
  them. The other choice was `.must()`, which aborts and prints nothing of mine.
- **Output order:** first counter doubled (6), second doubled (4), events (5), one
  per line.

context: yes — a system prompt said I am a Claude agent on the Claude Agent SDK and
gave a working directory and a date. The user's message told me to read `brief.md`,
use only these files and answer in English. My only tool could read files but not
write them. No repository status or memory reached me.
