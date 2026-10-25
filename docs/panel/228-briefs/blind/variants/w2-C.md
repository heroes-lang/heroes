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

`heroes check --json main.hero`, exit status 1.

Standard output:

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "main.hero", "sha256": "d8be077ebe5280f3c6e3f78a73ae3e0cc42d8eced84725047b27c27ab7a6a865"},
    {"file": "lib.hero", "sha256": "ed264d74c0a896875842d7906440e89815e6fc9b8269a2e6e56abefc36d8c940"}
  ],
  "diagnostics": [
    {
      "code": "hole",
      "message": "a hole: expected `lib.Money`",
      "file": "main.hero",
      "line": 5,
      "col": 21,
      "notes": ["in scope: price: i64", "fields of lib.Money: cents: i64, currency: str", "functions that return it: lib.dollars(n: i64) (An amount in dollars.); lib.euros(n: i64) (An amount in euros.); lib.francs(n: i64) (An amount in Swiss francs.); lib.pounds(n: i64) (An amount in pounds sterling.); lib.rupees(n: i64) (An amount in rupees.)"],
      "fixes": []
    }
  ]
}
```
