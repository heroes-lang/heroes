## catalogue

```toml
[dependencies.ada_csv]
git = "https://git.example.org/ada/csv"
tag = "v1.2.0"
sum = "sha256-34d7dc508706d83daba5e85482dc993822ee0fc41c9c33c00e62ee5d71a1c176"

[dependencies.ben_csv]
git = "https://git.example.org/ben/csv.git"
tag = "1.10"
sum = "sha256-8f8ec08f37590a2ff129f489fdd22e48c2221472e13b22ee04f3892fb40bdb75"
```

## program

```hero
use <ada_csv>/fields
use <ben_csv>/quote

function main()
    out: [str] @= []
    for f in fields.split_line("red,green,blue")
        out @ out.push(quote.quote_field(f))
    print(join(out, ";"))
```

## context

none
