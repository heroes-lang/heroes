# Sample 5

## The header, `ruler.h`

```c
static inline int sqrt(int x) { return x; }
static inline int eight(void) { return 8; }
```

## The program

```
extern "ruler.h"
    function eight() -> i32

function main()
    print(eight())
```

## Message X

```
internal error: compiling the generated C failed:
In file included from build/tu-7bfe6db853bcb0ee/estd.c:45:
In file included from <toolchain>/runtime/heroes_standard.h:60:
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/math.h:467:15: error: conflicting types for 'sqrt'
  467 | extern double sqrt(double);
      |               ^
In file included from build/tu-7bfe6db853bcb0ee/estd.c:20:
./ruler.h:1:19: note: previous definition is here
    1 | static inline int sqrt(int x) { return x; }
      |                   ^
1 error generated.

the generated C is at build/tu-7bfe6db853bcb0ee/estd.c
error: clang refused the generated C
```

## Message Y

```
error[ffi_header_refused]: `ruler.h`, the header this group names, does not compile: its line 1 declares `sqrt`, which `<math.h>`, a header of C's own, declares too, after the groups' headers, and clang refuses the two: conflicting types for 'sqrt'
  at ruler.hero:1:8
    |
  1 | extern "ruler.h"
    |        ^^^^^^^^^
  note: `ruler.h` line 1 reads `static inline int sqrt(int x) { return x; }`
  note: the C library's `sqrt`, which `<math.h>` declares, is the library's in a unit that includes that header (C11 7.1.3), and this program's C includes it after its groups' headers
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
  fix (guess): rename `sqrt` in `ruler.h`, a name of C's own library, which `<math.h>` declares
```
