## 14. Packages
A program may use packages: modules that others publish, fetched for it. Its
catalogue, the file `heroes.mod` beside the file you compile, has one entry per
package; a program that uses none has none. Each entry is a line of words:
`require`, the entry's name, `index` and the version or `git`, the address and
the tag, then the sum; `#` begins a comment line:
```
require json index 1.4.2 sha256-14a856d668dbbf1a29152de3db55b276e1c29cd5599823335de861c4b97da124
require yaml git https://git.example.org/ana/yaml v0.3.1 sha256-cdde804c15d59f1c85f133e063479ca5ecaa27304bbfadbff981caa339274ee9
```
- An entry from `index` comes from the Heroes index, which knows the package
  by the entry's name; the version is exact, three numbers. An entry from
  `git` comes from that address, at that tag, and its name is the program's
  choice.
- The sum is the fingerprint its publisher gives, `sha256-` then 64 lowercase
  hex digits; a package whose fetched files do not match it is refused.
- An entry's name is lowercase letters, digits and `_`, starting with a letter.
- `use <json>/parse` reaches the module `parse` of the entry `json` and binds
  `parse`; `as` renames it, as in section 1. A `use` without angle brackets
  reaches the program's own module, as in section 1: `use text/lines`. `Use`
  becomes:

    Use = "use" [ "<" ident ">" "/" ] ident { "/" ident } [ "as" ident ] NEWLINE .
