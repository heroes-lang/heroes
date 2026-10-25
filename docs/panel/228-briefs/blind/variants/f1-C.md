# The task

A program, `main.hero`, was checked by the language's compiler. You cannot see the program; what the compiler answered is below.

## What the compiler answered

`heroes check --json main.hero`, exit status 1.

Standard output:

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "main.hero", "sha256": "637f215728e645ba6882e17a834e78aba9ddde9064807b57dfb3e445b989bccc"}
  ],
  "diagnostics": [
    {
      "code": "hole",
      "message": "a hole: expected `Point`",
      "file": "main.hero",
      "line": 12,
      "col": 16,
      "notes": ["in scope: p: Point", "fields of Point: x: i64, y: i64"],
      "fixes": []
    },
    {
      "code": "hole",
      "message": "a hole: expected `i64`",
      "file": "main.hero",
      "line": 13,
      "col": 14,
      "notes": ["in scope: p: Point, q: Point", "functions that return it: dist2(a: Point, b: Point)"],
      "fixes": []
    }
  ]
}
```
