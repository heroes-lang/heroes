# The task

The program should print the squared distance between the origin, `p`, and the point at x 3 and y 4.

## The program, `t1.hero`

```
record Point
    x: i64
    y: i64

function dist2(a: Point, b: Point) -> i64
    dx = a.x - b.x
    dy = a.y - b.y
    return dx*dx + dy*dy

function main()
    p = Point(x: 0, y: 0)
    q: Point = ???
    d: i64 = ???
    print(d)
```

## What the compiler answered

`heroes check --json t1.hero`, exit status 0.

Standard output:

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "t1.hero", "sha256": "637f215728e645ba6882e17a834e78aba9ddde9064807b57dfb3e445b989bccc"}
  ],
  "diagnostics": [],
  "holes": [
    {
      "file": "t1.hero",
      "line": 12,
      "col": 16,
      "expected": "Point",
      "in_scope": [
        {"name": "p", "type": "Point", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [
        {"name": "x", "type": "i64"},
        {"name": "y", "type": "i64"}
      ],
      "functions": []
    },
    {
      "file": "t1.hero",
      "line": 13,
      "col": 14,
      "expected": "i64",
      "in_scope": [
        {"name": "p", "type": "Point", "mutable": false},
        {"name": "q", "type": "Point", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [],
      "functions": ["dist2(a: Point, b: Point)"]
    }
  ]
}
```
