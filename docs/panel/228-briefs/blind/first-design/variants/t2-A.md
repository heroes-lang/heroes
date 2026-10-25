# The task

The program should print 40: the pen costs 10 in all, and the book is a book that costs 15 a copy, two copies.

## The program, `t2.hero`

```
record Item
    name: str
    price: i64
    qty: i64

function total(items: [Item]) -> i64
    sum @= 0
    for it in items
        sum @ sum + it.price * it.qty
    return sum

function main()
    pen = Item(name: "pen", price: 2, qty: 5)
    book: Item = ???
    print(total([book, pen]))
```

## What the compiler answered

`heroes check --json t2.hero`, exit status 0.

Standard output:

(nothing)

Standard error:

```
{
  "schema": 2,
  "kind": "diagnostics",
  "read": [
    {"file": "t2.hero", "sha256": "126f01d019165827cf202f2f453a6498fc8724f6765666cabe5eb3f076359ae2"}
  ],
  "diagnostics": [],
  "holes": [
    {
      "file": "t2.hero",
      "line": 14,
      "col": 18,
      "expected": "Item",
      "in_scope": [
        {"name": "pen", "type": "Item", "mutable": false}
      ],
      "more_in_scope": 0,
      "fields": [
        {"name": "name", "type": "str"},
        {"name": "price", "type": "i64"},
        {"name": "qty", "type": "i64"}
      ],
      "functions": []
    }
  ]
}
```
