# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `basket.hero`

It should print:

```
total 1280
```

It printed nothing on standard output.

It wrote on standard error:

```
panic: .must() on an error: missing_key: no such key
```

It exited with status 134.

The program:

```hero
# Prices a basket: each item's unit price comes from the catalogue, and the
# coupon's discount from the coupons table. Prices are in cents.

function catalogue() -> {str: i64}
    c: {str: i64} @= {}
    c["apple"] @ 30
    c["bread"] @ 250
    c["cheese"] @ 480
    return c

function coupons() -> {str: i64}
    c: {str: i64} @= {}
    c["WELCOME"] @ 100
    c["SPRING"] @ 50
    return c

function line_total(prices: {str: i64}, item: str, count: i64) -> i64
    return prices[item].must() * count

function main()
    prices = catalogue()
    off = coupons()
    total @= 0
    total @ total + line_total(prices, item: "apple", count: 4)
    total @ total + line_total(prices, item: "bread", count: 1)
    total @ total + line_total(prices, item: "chese", count: 2)
    total @ total - off["SPRING"].must()
    print("total ", total)
```
