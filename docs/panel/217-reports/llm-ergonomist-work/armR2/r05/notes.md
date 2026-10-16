# Notes

- **`doubled` takes the counter by place (`@c`), not by value.** The compiler offered this fix,
  and the task requires `counter_value`, which in C takes a pointer. The other choice was to read
  `c.n` in place, which the compiler's note also suggests. That would skip `counter_value`, which
  the task names. A by-value parameter, or a local copy (`local @= c`), is refused
  (`ffi_lock_copied`). The `@` does not change the counter: `counter_value` takes a
  `const struct counter *`. The spec says an `@` argument counts as a use, so `@c` is not an
  unused `@` parameter.
- **`setup_counter` no longer assigns `c @ Counter(rest: zero)`.** That line was refused as a
  copy. `counter_init` already sets `n = 0` and initialises the mutex in the caller's place. The
  zeroed starting value comes from building `Stats` with `Counter(rest: zero)`, a line the
  compiler did not flag. If that line were also treated as a copy, the other choice would be a
  different way of building `Stats`. Neither the spec nor the compiler offers one.
- **Set up in the caller's place, not returned.** "Sets up a new counter for its caller" could
  also mean a function that returns a `Counter`. A return moves and copies the value, which a lock
  forbids, so it takes `@c` instead.
- **`counter_value` is still declared `@c: Counter lent`.** Declaring it by value would be a copy.
  Leaving out `lent` would mean C may keep the place it is given.
- **Errors are fallible results (`()?`), and `main` uses `.must()`.** The other choice was to
  abort inside the helpers with `assert`. The output on success is the same either way: `6`, `4`,
  `5`.
- **No mutex destroy.** The header provides no destroy function, so the mutexes are never torn
  down.

context: no. Only brief.md, heroes-spec.md, task.md, counter.h, previous.hero and compiler.txt
reached me. Apart from the system prompt's statement of the working directory and date, no
outside instruction, rule, memory or repository status did.
