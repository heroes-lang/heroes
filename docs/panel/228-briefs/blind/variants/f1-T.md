# The task

A program, `main.hero`, was checked by the language's compiler. You cannot see the program; what the compiler answered is below.

## What the compiler answered

`heroes check --json main.hero`, exit status 0.

Standard output:

```
hole at main.hero:12:16
  expected type: Point
  in scope:
    p: Point
  fields of Point: x: i64, y: i64
hole at main.hero:13:14
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
    {"file": "main.hero", "sha256": "637f215728e645ba6882e17a834e78aba9ddde9064807b57dfb3e445b989bccc"}
  ],
  "diagnostics": []
}
```
