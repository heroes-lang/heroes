## 14. Packages
Section 1's *every `use` starts at the directory of the file you compile* now
holds for the program's own modules; a `use` reaches a package by `pkg/` and
its entry's name, `pkg/json`.

A program may use packages: modules that others publish, fetched for it. Its
catalogue, the file `heroes.toml` beside the file you compile, has one entry
per package; a program that uses none has none. The catalogue holds the
program's dependencies and nothing else: no name, version or description of
the program itself. The catalogue is TOML, each entry a table under
`dependencies`, each value a string:
```
[dependencies.json]
version = "1.4.2"
sum = "sha256-14a856d668dbbf1a29152de3db55b276e1c29cd5599823335de861c4b97da124"

[dependencies.yaml]
git = "https://git.example.org/ana/yaml"
tag = "v0.3.1"
sum = "sha256-cdde804c15d59f1c85f133e063479ca5ecaa27304bbfadbff981caa339274ee9"
```
- An entry with a `version` comes from the Heroes index, which knows the
  package by the entry's name; the version is exact, three numbers. An entry
  with a `git` address comes from there, at its `tag`, and its name is the
  program's choice.
- The sum is the fingerprint its publisher gives, `sha256-` then 64 lowercase
  hex digits; a package whose fetched files do not match it is refused.
- An entry's name is lowercase letters, digits and `_`, starting with a letter.
- `use pkg/json/parse` reaches the module `parse` of the entry `json` and
  binds `parse`; `as` renames it, as in section 1. A `use` whose first name is
  not `pkg` reaches the program's own module, as in section 1:
  `use text/lines`.
- `pkg` is reserved as the first name of a `use`: a program has no folder or
  module of its own named `pkg`. `Use` becomes:

    Use = "use" [ "pkg" "/" ident "/" ] ident { "/" ident } [ "as" ident ] NEWLINE .
