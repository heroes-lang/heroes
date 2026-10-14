# Sample 2

## The header, `counts.h`

```c
typedef long int64_t;
static inline int64_t seven(void) { return 7; }
```

## The program

```
extern "counts.h"
    function seven() -> i64

function main()
    print(seven())
```

## Message X

```
error[ffi_header_refused]: `counts.h`, the header this group names, does not compile: clang refuses it at line 1: typedef redefinition with different types ('long' vs 'long long')
  at counts.hero:1:8
    |
  1 | extern "counts.h"
    |        ^^^^^^^
  note: `counts.h` line 1 reads `typedef long int64_t;`
  note: clang adds, at C's own `int64_t`, which every unit of this program declares as `<stdint.h>` does, before any group's header: previous definition is here
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
```

## Message Y

```
internal error: compiling the generated C failed:
In file included from build/tu-399aa10154acbe42/counts.c:20:
./counts.h:1:14: error: typedef redefinition with different types ('long' vs 'long long')
    1 | typedef long int64_t;
      |              ^
In file included from build/tu-399aa10154acbe42/counts.c:2:
In file included from <toolchain>/runtime/heroes_unit.h:26:
<toolchain>/runtime/heroes_runtime.h:78:24: note: previous definition is here
   78 | typedef __INT64_TYPE__ int64_t;
      |                        ^
1 error generated.

the generated C is at build/tu-399aa10154acbe42/counts.c
error: clang refused the generated C
```
