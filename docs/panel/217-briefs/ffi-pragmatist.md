# Panel 217, the ffi-pragmatist's brief

Read `docs/panel/217-briefs/00-shared.md` first, whole.

## Your seat's question

**Does a read-only lend bind the C that takes a place it only reads?** Write the bindings F1 implies
and compile their C: `counter.h`'s `counter_value(const struct counter *)` as `counter_value(c:
Counter lent)`; and from the headers this project binds (`git grep -l -E '^[[:space:]]*extern '
f57af53da -- '*.hero' ':!docs'` lists the files), the extern parameters today declared `@x: T` or
`@x: T lent` whose header parameter is a pointer to `const`: count them, and say which F1 would
let drop the `@` and which it would not (a `const` pointer C still keeps, a `const` pointer to an
array). On this Mac and in `heroes-linux-arm64`; the Windows box (`ssh win`) if it answers, *unrun*
if not. What clang checks of such a binding today (the pointee check, `selfhost/cli/pointee.hero`),
and what it must check under F1 so that a header taking `struct counter *` (non-`const`) is not
bound read-only: a C function that writes through a pointer the program declared read-only is a
wrong value at exit 0.

## What to return

`verdict` per form, `section` (design.md §1.11, §4.19), `experiment` (each command and what it
printed), `prediction`, `condition` (what turns it, and what would be a veto: a correct binding
made unbuildable, or a C write through a read-only lend accepted).
