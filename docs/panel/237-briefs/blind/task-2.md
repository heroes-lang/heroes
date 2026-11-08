# Task

A program's folder holds three files. `main.hero`:

```
extern "money.h"
    function money_cents(amount: i64) -> i64

function main()
    print(money_cents(amount: 1250))
```

`money.h`:

```
#include <stdint.h>

static inline int64_t money_cents(int64_t amount) { return amount * 100; }
```

and `stdint.h`:

```
#include_next <stdint.h>

typedef int64_t money_t;
```

`heroes build main.hero`, run in that folder, prints this and exits 1:

```
error[shadowed_header]: `stdint.h` stands in for the C library's `<stdint.h>`, which the C this compiler writes reads: a folder this build names is searched before the system's, so its file is read in the library's place
  at main.hero:1:1
    |
  1 | extern "money.h"
    | ^
  note: rename the file, or move it out of `.`: the folder is this program's own, one `--include` names, or one a package's `pkg-config` answers, and every `#include <stdint.h>` of the build reads what it holds
```

Say what you change so the program builds and prints `125000`, and why, in at most five sentences,
and write every file you change, whole. Use only what is in front of you.
