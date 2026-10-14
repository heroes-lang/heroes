# Notes

## a2.hero
- `via` is an anonymous union with no typedef, so it can't be typed as a record: I left it to
  `partial` and let `rest: zero` zero it. The only other choice was naming one union member,
  which can't be done without a typedef'd union record.
- The record is `Tagged tag tagged`, because the header writes the name after `struct`.
  Without `tag` the record would name a C type `Tagged` that doesn't exist, and clang would refuse it.
- I declared `const struct tagged *` as `@t: Tagged lent` on a cell `t @=`. Without `lent`,
  C would be taken to keep the place, and the call would be refused or unsafe.
- I declared the result as `i32`, C's own width. `i64` (wider) is also allowed and prints the same.

## u.hero
- I declared all five fields from the header given in tasks.md (this matches macOS). `partial`
  with only `sysname` would also work and would survive headers with other fields or sizes
  (glibc uses 65 bytes plus `domainname`). It would also block `==` on the record.
- `char` is `i8`, as the spec says. I built the record with `rest: zero` naming no field, instead
  of writing out five 256-element literals.
- If `uname` returns non-zero, the program prints it and calls `exit(code: 1)`. On bad text it
  prints the error code instead of aborting. Using `.must()` would abort instead.

## c.hero
- `pthread_mutex_t` isn't a type the FFI allows, so `Counter` is `partial` with only `n`.
  This makes `Stats` non-comparable, which this program never needs.
- "Sets up a new counter for its caller": I initialise the caller's counter in place
  (`@c: Counter`). Returning a `Counter?` would move an initialised mutex, which the spec
  warns about.
- To leave the counter unchanged, `doubled` takes the counter without `@` and lends C a local
  copy. Declaring `counter_value`'s parameter without `@` would go against the rule that C
  pointers are `@` parameters.
- Failures are `()?` with snake_case codes; `main` (which returns nothing) uses `.must()`.
  Propagating with `?` isn't possible from `main`.
- Loops are `for _ in range(...)`, which assumes `_` is allowed as a loop variable. A `while`
  loop with a cell is the alternative if it isn't.

context: yes — a system prompt said I am a Claude agent with a working directory and the date
2026-10-14, and gave me only a file-reading tool (no write tool), so these files could not be
written to disk.
