# Sample 6

## The header, `gadget.h`

```c
static inline int eight(void) { HeroSt s; (void)s; return 8; }
```

## The program

```
extern "gadget.h"
    function eight() -> i32

function main()
    print(eight())
```

## Message X

```
error[ffi_header_refused]: `gadget.h`, the header this group names, does not compile: clang refuses it at line 1: unknown type name 'HeroSt'; did you mean 'HeroStr'?
  at gadget.hero:1:8
    |
  1 | extern "gadget.h"
    |        ^^^^^^^^^^
  note: `gadget.h` line 1 reads `static inline int eight(void) { HeroSt s; (void)s; return 8; }`
  note: clang adds, at `HeroStr`, a name of the Heroes runtime's own, which every unit of this program declares before any group's header, and clang suggests in the refused line: 'HeroStr' declared here
  note: a group's header is compiled as this program's own C includes it, before any declaration of the group is checked against it: repair the header, or name the one that declares this group's C
```

## Message Y

```
internal error: compiling the generated C failed:
In file included from build/tu-118158886bef8d88/jtypo.c:20:
./gadget.h:1:33: error: unknown type name 'HeroSt'; did you mean 'HeroStr'?
    1 | static inline int eight(void) { HeroSt s; (void)s; return 8; }
      |                                 ^~~~~~
      |                                 HeroStr
In file included from build/tu-118158886bef8d88/jtypo.c:2:
In file included from <toolchain>/runtime/heroes_unit.h:26:
<toolchain>/runtime/heroes_runtime.h:147:3: note: 'HeroStr' declared here
  147 | } HeroStr;
      |   ^
1 error generated.

the generated C is at build/tu-118158886bef8d88/jtypo.c
error: clang refused the generated C
```
