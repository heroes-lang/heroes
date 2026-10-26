# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `checksum.hero`

It should print:

```
sum of squares 20540
mean 526
checksum 938856634
```

It printed on standard output:

```
sum of squares 20540
mean 526
```

It wrote on standard error:

```
panic: integer overflow
  at checksum.hero:11:13, in checksum.checksum
```

It exited with status 134.

The program:

```hero
# Prints the sum of the squares of 1 to 39, their mean, and a checksum of the
# numbers reported modulo 1,000,000,007.

constant MODULUS: i64
    1_000_000_007

function checksum(values: [i64]) -> i64
    h @= 7

    for v in values
        h @ h * 31 + v

    return h % MODULUS

function main()
    values = range(from: 1, to: 40)
    total @= 0

    for v in values
        total @ total + v * v

    print("sum of squares ", total)
    print("mean ", total / values.len())
    print("checksum ", checksum(values))
```
