## catalogue

```hero
name: "app"

dependency roman
    version: "2.1.0"
    sum: "sha256-79151d65cd1d4c2caf5095b19dd286eded992640e674dc3e4ca71650b287fbe9"
```

## program

```hero
use <roman>/numeral

function main()
    for n in [4, 1987, 2026]
        print(f"{n} = {numeral.to_roman(n).must()}")
```

## context

none
