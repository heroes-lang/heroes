# Sample 1

## The header, `notes.h`

```c
#include <stdint.h>
static inline int hero_str_len(int x) { return x; }
static inline int eight(void) { return hero_str_len(8); }
```

## The program

```
extern "notes.h"
    function eight() -> i32

function main()
    print(eight())
```

## Message X

```
internal error: compiling the generated C failed:
In file included from build/tu-545f84e922beb2b1/notes.c:20:
./notes.h:2:19: error: static declaration of 'hero_str_len' follows non-static declaration
    2 | static inline int hero_str_len(int x) { return x; }
      |                   ^
In file included from build/tu-545f84e922beb2b1/notes.c:2:
In file included from <toolchain>/runtime/heroes_unit.h:26:
<toolchain>/runtime/heroes_runtime.h:278:9: note: previous declaration is here
  278 | int64_t hero_str_len(HeroStr s);
      |         ^
In file included from build/tu-545f84e922beb2b1/notes.c:20:
./notes.h:3:53: error: passing 'int' to parameter of incompatible type 'HeroStr'
    3 | static inline int eight(void) { return hero_str_len(8); }
      |                                                     ^
In file included from build/tu-545f84e922beb2b1/notes.c:2:
In file included from <toolchain>/runtime/heroes_unit.h:26:
<toolchain>/runtime/heroes_runtime.h:278:30: note: passing argument to parameter 's' here
  278 | int64_t hero_str_len(HeroStr s);
      |                              ^
2 errors generated.

the generated C is at build/tu-545f84e922beb2b1/notes.c
error: clang refused the generated C
```

## Message Y

```
error[ffi_header_refused]: `notes.h`, the header this group names, does not compile: clang refuses it at line 2: static declaration of 'hero_str_len' follows non-static declaration
  at notes.hero:1:8
    |
  1 | extern "notes.h"
    |        ^^^^^^
  note: `notes.h` line 2 reads `static inline int hero_str_len(int x) { return x; }`
  note: clang adds, at the Heroes runtime's own `hero_str_len`, which every unit of this program declares before any group's header: previous declaration is here
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
  fix (guess): rename `hero_str_len` in `notes.h`, a name the Heroes runtime declares first in every unit
```
