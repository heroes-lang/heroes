# Notes

## a2.hero
- `via` is an anonymous union with no typedef, so no record can type it. I left it to `partial`, and `rest: zero` zeroes it. There is no other way to name it.
- Record name: I wrote `Tagged tag tagged`, reading "written after the word struct" as needing `tag`. The other choice, `record tagged` with no tag, would only be right if the name alone matched a struct tag.
- `const struct tagged *` is an `@ … lent` parameter. Passing by value would be refused (wrong type), and dropping `lent` would make the program assume C keeps the place.
- Result `i32` (C's int). `i64` is also allowed (a result may be wider) and prints the same `2`.

## u.hero
- Field widths come from the header given in tasks.md (`char[256]` → `i8[256]`, five fields). On Linux the real header uses 65-byte fields (and may add `domainname`), so clang would refuse this there.
- I built it with `Utsname(rest: zero)` rather than writing out five 256-element arrays.
- If `uname` fails, the program prints a message and runs `exit(code: 1)`. Ignoring the result is not an option (an unused binding is an error).
- On a `not_text` failure it prints `e.code`. Printing `e.msg` would be the other choice.

## c.hero
- `pthread_mutex_t` can't be a field type, so `Counter` is `partial` with only `n: i64`, and its size stays C's.
- "Sets up a new counter for its caller": I wrote `setup_counter(@c: Counter) -> ()?`, which works in the caller's place. Returning a `Counter?` instead would copy an initialised mutex when it returns and when it goes into `Stats`. POSIX says not to copy one, though it works in practice.
- `doubled` takes the counter by value and lends a local copy to `counter_value`, because C's `const *` still has to be `@`. Taking `@c` would avoid the copy but would mark the counter as changed.
- Failures come back as `()?`, and `main` calls `.must()`, which aborts. Matching and printing would be the other choice.
- Loops use `while` with counter cells, because I wasn't sure `_` is allowed as a `for` variable. A named `for` variable that is never used would be a compile error.
- Events are counted in `main` after each increment. Passing all of `Stats` to a helper would be the other choice; the output is the same.

context: yes — besides the folder's files I received a system prompt: an agent identity (Claude Agent SDK), the working directory path (a `.claude/worktrees/readers2/r05` worktree), the date (2026-10-14), and tool-use instructions. No repository status, memories or other rules.
