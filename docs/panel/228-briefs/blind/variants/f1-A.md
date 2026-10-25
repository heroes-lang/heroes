# The task

A program, `main.hero`, was checked by the language's compiler. You cannot see the program; what the compiler answered is below.

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
    {"file": "main.hero", "sha256": "637f215728e645ba6882e17a834e78aba9ddde9064807b57dfb3e445b989bccc"}
  ],
  "diagnostics": [],
  "holes": [
    {
      "file": "main.hero",
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
      "file": "main.hero",
      "line": 13,
      "col": 14,
      "expected": "i64",
      "in_scope": [
        {"name": "p", "type": "Point", "mutable": false},
        {"name": "q", "type": "Point", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [],
      "functions": [
        {"call": "dist2(a: Point, b: Point)", "doc": null}
      ]
    }
  ]
}
```
