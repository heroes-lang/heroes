# Task

A program of three files, in one folder:

`main.hero`

```
use geom
use geom/internal/scan

function main()
    print(geom.area(2) + scan.twice(1))
```

`geom.hero`

```
use geom/internal/scan

function area(n: i64) -> i64
    return scan.twice(n)
```

`geom/internal/scan.hero`

```
function twice(n: i64) -> i64
    return n * 2
```

Its author runs `heroes build main.hero` in that folder and expects the program to print `6`.

1. Does the compiler accept the program as written? If it does, say what it prints. If it does not,
   say what it refuses and why, citing the specification.
2. If it does not build, write the smallest repair that keeps each function's meaning and prints
   `6`: every file you change, whole.
