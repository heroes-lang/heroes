# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `shipping.hero`

It should print:

```
order 2: Porto, 610 cents
order 3: Graz, 520 cents
order 1: Lyon, 450 cents
```

It printed nothing on standard output.

It wrote on standard error:

```
panic: .must() on an error: missing_key: no such key
```

It exited with status 134.

The program:

```hero
# Prints, for each order, the city its customer lives in and the rate to ship
# there. An order names its customer by number; the customers table keys each
# one by "c" and that number, and the rates table keys each city by its name.

function customers() -> {str: str}
    c: {str: str} @= {}
    c["c1"] @ "Lyon"
    c["c2"] @ "Porto"
    c["c3"] @ "Graz"
    return c

function rates() -> {str: i64}
    r: {str: i64} @= {}
    r["Lyon"] @ 450
    r["Porto"] @ 610
    r["Graz"] @ 520
    return r

function key_of(id: i64) -> str
    return "c-" + id.to_str()

function main()
    cities = customers()
    cost = rates()
    orders = [2, 3, 1]

    for id in orders
        city = cities[key_of(id)].must()
        print("order ", id, ": ", city, ", ", cost[city].must(), " cents")
```
