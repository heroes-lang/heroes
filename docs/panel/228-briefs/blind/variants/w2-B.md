# The task

The program should print the price, 12, as an amount in euros: `12.00 EUR`.

## The program, `main.hero`

It uses a module, `lib.hero`, which you cannot see.

```
use lib

function main()
    price = 12
    print(lib.shown(???))
```

## What the compiler answered

`heroes check --json main.hero`, exit status 0.

Standard output:

```
{
  "schema": 2,
  "kind": "holes",
  "read": [
    {"file": "main.hero", "sha256": "d8be077ebe5280f3c6e3f78a73ae3e0cc42d8eced84725047b27c27ab7a6a865"},
    {"file": "lib.hero", "sha256": "ed264d74c0a896875842d7906440e89815e6fc9b8269a2e6e56abefc36d8c940"}
  ],
  "holes": [
    {
      "file": "main.hero",
      "line": 5,
      "col": 21,
      "expected": "lib.Money",
      "in_scope": [
        {"name": "price", "type": "i64", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [
        {"name": "cents", "type": "i64"},
        {"name": "currency", "type": "str"}
      ],
      "functions": [
        {"call": "lib.dollars(n: i64)", "doc": "An amount in dollars."},
        {"call": "lib.euros(n: i64)", "doc": "An amount in euros."},
        {"call": "lib.francs(n: i64)", "doc": "An amount in Swiss francs."},
        {"call": "lib.pounds(n: i64)", "doc": "An amount in pounds sterling."},
        {"call": "lib.rupees(n: i64)", "doc": "An amount in rupees."}
      ]
    }
  ]
}
```

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "main.hero", "sha256": "d8be077ebe5280f3c6e3f78a73ae3e0cc42d8eced84725047b27c27ab7a6a865"},
    {"file": "lib.hero", "sha256": "ed264d74c0a896875842d7906440e89815e6fc9b8269a2e6e56abefc36d8c940"}
  ],
  "diagnostics": []
}
```
