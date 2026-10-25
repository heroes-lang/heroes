# The task

The program should print each name followed by an exclamation mark, one per line: `ada!` then `bob!`.

## The program, `t3.hero`

```
function shout(name: str) -> str
    return name + "!"

function main()
    names = ["ada", "bob"]
    for n in names
        ???
```

## What the compiler answered

`heroes check --json t3.hero`, exit status 0.

Standard output:

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "t3.hero", "sha256": "51f9bd2a53ec6b020dab3888cde65a9cd2ed643c24c655818531f7d09c33b219"}
  ],
  "diagnostics": [],
  "holes": [
    {
      "file": "t3.hero",
      "line": 7,
      "col": 9,
      "position": "statement",
      "in_scope": [
        {"name": "names", "type": "[str]", "mutable": false},
        {"name": "n", "type": "str", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [],
      "functions": []
    }
  ]
}
```
