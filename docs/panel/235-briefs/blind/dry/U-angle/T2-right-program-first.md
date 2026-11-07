Here is the program:

```hero
use <csv_ada>/fields
use <csv_ben>/quote

function main()
    quoted: [str] @= []
    for field in fields.split_line("red,green,blue")
        quoted @ quoted.push(quote.quote_field(field))
    print(join(quoted, ";"))
```

And the catalogue beside it:

```toml
[dependencies.csv_ada]
git = "https://git.example.org/ada/csv"
tag = "v1.2.0"
sum = "sha256-34d7dc508706d83daba5e85482dc993822ee0fc41c9c33c00e62ee5d71a1c176"

[dependencies.csv_ben]
git = "https://git.example.org/ben/csv"
tag = "1.10"
sum = "sha256-8f8ec08f37590a2ff129f489fdd22e48c2221472e13b22ee04f3892fb40bdb75"
```
