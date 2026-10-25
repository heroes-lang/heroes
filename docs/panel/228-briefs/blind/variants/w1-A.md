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

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "main.hero", "sha256": "4f87520b6dadfbe910b1d386e1936ced17e4d3d093987ed3649fc8a0efd8d47d"},
    {"file": "lib.hero", "sha256": "64af68ff4ba124a39cae5fe9f5aa40897543d16876b85019eda32cccb431e8f7"}
  ],
  "diagnostics": [],
  "holes": [
    {
      "file": "main.hero",
      "line": 5,
      "col": 22,
      "expected": "lib.Box",
      "in_scope": [
        {"name": "edge", "type": "i64", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [
        {"name": "width", "type": "i64"},
        {"name": "height", "type": "i64"},
        {"name": "depth", "type": "i64"}
      ],
      "functions": [
        {"call": "lib.cube(side: i64)", "doc": "Builds a cube whose edges all measure `side`."},
        {"call": "lib.tile(width: i64, height: i64)", "doc": "Builds a flat box, one unit deep."}
      ]
    }
  ]
}
```
