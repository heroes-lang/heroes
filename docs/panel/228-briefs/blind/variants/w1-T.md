# The task

The program should print the volume of a cube whose edges all measure `edge`: `27`.

## The program, `main.hero`

It uses a module, `lib.hero`, which you cannot see.

```
use lib

function main()
    edge = 3
    print(lib.volume(???))
```

## What the compiler answered

`heroes check --json main.hero`, exit status 0.

Standard output:

```
hole at main.hero:5:22
  expected type: lib.Box
  in scope:
    edge: i64
  fields of lib.Box: width: i64, height: i64, depth: i64
  functions that return it:
    lib.cube(side: i64)
      # Builds a cube whose edges all measure `side`.
    lib.tile(width: i64, height: i64)
      # Builds a flat box, one unit deep.
```

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "main.hero", "sha256": "4f87520b6dadfbe910b1d386e1936ced17e4d3d093987ed3649fc8a0efd8d47d"},
    {"file": "lib.hero", "sha256": "64af68ff4ba124a39cae5fe9f5aa40897543d16876b85019eda32cccb431e8f7"}
  ],
  "diagnostics": []
}
```
