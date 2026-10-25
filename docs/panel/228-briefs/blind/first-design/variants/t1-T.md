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

```
hole at t1.hero:12:16
  expected type: Point
  in scope:
    p: Point
  fields of Point: x: i64, y: i64
hole at t1.hero:13:14
  expected type: i64
  in scope:
    p: Point
    q: Point
  functions that return it:
    dist2(a: Point, b: Point)
```

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "t1.hero", "sha256": "637f215728e645ba6882e17a834e78aba9ddde9064807b57dfb3e445b989bccc"}
  ],
  "diagnostics": []
}
```
