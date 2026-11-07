# The task

Write a Heroes program that splits the text `red,green,blue` into its fields
at each comma, quotes each field as comma-separated values quote one, and
prints the quoted fields on one line, separated by `;`, as the expected
output shows. Print nothing else.

Two packages from git do the work, and their authors call both of them
`csv`:

- the first splits. Its address: `https://git.example.org/ada/csv`; its tag:
  `v1.2.0`; its fingerprint:
  `sha256-34d7dc508706d83daba5e85482dc993822ee0fc41c9c33c00e62ee5d71a1c176`.
  Its module `fields` declares `function split_line(line: str) -> [str]`: the
  fields of `line` between its commas, in order.
- the second quotes. Its address: `https://git.example.org/ben/csv`; its tag:
  `1.10`; its fingerprint:
  `sha256-8f8ec08f37590a2ff129f489fdd22e48c2221472e13b22ee04f3892fb40bdb75`.
  Its module `quote` declares `function quote_field(field: str) -> str`:
  `field` inside double quotes, each double quote in it doubled.

The program uses both packages.

## Expected output

```
"red";"green";"blue"
```
