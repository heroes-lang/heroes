# Sample 4

## The header, `tally.h`

```c
static inline int eight(void) { return (int)hero_str_le(8); }
```

## The program

```
extern "tally.h"
    function eight() -> i32

function main()
    print(eight())
```

## Message X

```
error[ffi_header_refused]: `tally.h`, the header this group names, does not compile: clang refuses it at line 1: call to undeclared function 'hero_str_le'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  at tally.hero:1:8
    |
  1 | extern "tally.h"
    |        ^^^^^^^
  note: `tally.h` line 1 reads `static inline int eight(void) { return (int)hero_str_le(8); }`
  note: clang adds, at `tally.h` line 1: did you mean 'hero_str_len'?
  note: clang adds, at `hero_str_len`, a name of the Heroes runtime's own, which every unit of this program declares before any group's header, and clang suggests in the refused line: 'hero_str_len' declared here
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
```

## Message Y

```
internal error: linking failed:
Undefined symbols for architecture arm64:
  "_hero_str_le", referenced from:
      _eight in tally-3bb19d0d6c2b9777.o
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)

error: the linker refused the objects clang compiled
```
