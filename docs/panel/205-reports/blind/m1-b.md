## program

One file, `main.hero`:

```
extern "sched.h"
    function sched_getcpu() -> i32

function main()
    cpu = sched_getcpu()
    print(cpu >= 0)
```

No other file was written besides this report.

## choice_points

1. **Result type of `sched_getcpu`.** C declares `int sched_getcpu(void)`. Section 13 says a result may be wider than C's, so `i32` and `i64` (and other wider signed types) are both accepted. I chose `i32`, the header's own width. With `i64`, the program would behave the same; the literal `0` would then default to `i64` instead of taking `i32` from context, and the output would not change.
2. **`link` or no `link`.** Section 13 says a group names a library "when the symbols need one". `sched_getcpu` is in glibc's libc, which is linked anyway, so I wrote no `link` clause. Writing `link "c"` should also work, but it adds nothing. A `package` clause would be wrong because there is no pkg-config package for libc.
3. **Binding the result or not.** I bound the result to `cpu` and then compared it. Writing `print(sched_getcpu() >= 0)` directly would produce the same output. Binding it makes "call it once" easy to see.
4. **Test for success.** The brief says to print `true` when the call returned a CPU number (zero or more). glibc returns -1 and sets errno on failure, so `cpu >= 0` is the test. Writing `cpu != -1` would give the same result on glibc. It would only differ if some other negative value ever came back, and in that case it would print `true` when it should print `false`.
5. **Ownership and lifetime words.** The call takes no parameters and returns a plain integer, so none of `owned`, `acquires`, `consumes`, `lent` and so on applies. Section 13 also forbids those words on values that do not reach a handle.

## confidence

I am fairly confident that it builds and prints `true` on an ordinary Linux machine. The syntax follows the grammar: an `Extern` with one `Member`, a parameter list that is allowed to be empty, a `main` that takes nothing and returns nothing, an immutable binding that gets used, and a `print` of a `bool`.

What I am least sure of is the feature-test macro. glibc's `sched.h` declares `sched_getcpu` only when `_GNU_SOURCE` (`__USE_GNU`) is defined. The specification has no way to define a macro before the header is read. clang checks every declared function against the header, so if the compiler does not define `_GNU_SOURCE` itself, the check could fail with an undeclared-function error. Nothing in the specification lets me remove that risk. I am assuming the toolchain compiles in a GNU mode, as clang's default `gnu` C dialects together with typical glibc setups often do. That assumption is not stated anywhere in the spec.

I am less worried about whether `i32` as a result type and the literal `0` taking `i32` from the comparison are accepted. Section 2 and section 13 both seem to allow them.

## context

Nothing outside this directory's files reached my context except the harness's own system context. That context included the working environment details and an automatically attached user email address, which I did not use. I read only `brief.md` and `spec.md`, and wrote only `main.hero` and `report.md`. My general knowledge of C and glibc (the signature of `sched_getcpu`, its -1 failure return, and the `_GNU_SOURCE` requirement) came from training, not from files.
