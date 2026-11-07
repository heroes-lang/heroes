## catalogue

```toml
[dependencies.csv]
git = "https://git.example.org/ada/csv"
tag = "v1.2.0"
sum = "sha256-34d7dc508706d83daba5e85482dc993822ee0fc41c9c33c00e62ee5d71a1c176"

[dependencies.csv]
git = "https://git.example.org/ben/csv"
tag = "v0.9.4"
sum = "sha256-6a858c25dddf189645befd02e67b98a1c8cd38ca3e2393474af73c67351d87d6"
```

## program

```hero
use <csv>/fields
use <csv>/quote

function main()
    quoted: [str] @= []
    for field in fields.split_line("red,green,blue")
        quoted @ quoted.push(quote.quote_field(field))
    print(join(quoted, ";"))
```

## context

none
