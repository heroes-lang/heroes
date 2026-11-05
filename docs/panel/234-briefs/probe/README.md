# Panel 234's probes, run 2026-11-05 by the coordinator

Each is run from the repository's root with the tree's compiler, `heroes 0.6.0`,
`sources 1a1e340778db789a` (the tree at f0c4c99d2). The output is the compiler's,
whole, with its blank lines and excerpt bars left out (the critic re-ran all
seventeen with its own compiler and each reproduced, and none had been cut,
F35 of `docs/panel/234-reports/critic-briefs.md`). Re-run any of them.

## 1. Where `use` starts (panel 099 R1, R4)

```
$ ./heroes run docs/panel/234-briefs/probe/use-root/main.hero
true
(exit 0)
```

```
$ ./heroes test docs/panel/234-briefs/probe/use-root/strings.hero
ok   "contains"
1 test, all passed
(exit 0)
```

```
$ ./heroes test docs/panel/234-briefs/probe/use-root/net/http.hero
error[unknown_module]: there is no module `strings` — `use strings` reads `docs/panel/234-briefs/probe/use-root/net/strings.hero`, from the directory of `docs/panel/234-briefs/probe/use-root/net/http.hero`, the file you compile, and that file is not there
  at docs/panel/234-briefs/probe/use-root/net/http.hero:1:5
  1 | use strings
(exit 1)
```

```
$ ./heroes run docs/panel/234-briefs/probe/use-root/apps/wc.hero
error[unknown_module]: there is no module `strings` — `use strings` reads `docs/panel/234-briefs/probe/use-root/apps/strings.hero`, from the directory of `docs/panel/234-briefs/probe/use-root/apps/wc.hero`, the file you compile, and that file is not there
  at docs/panel/234-briefs/probe/use-root/apps/wc.hero:1:5
  1 | use strings
(exit 1)
```

## 2. A `core/` directory under the program's root

```
$ ./heroes run docs/panel/234-briefs/probe/core-prefix/main.hero
true
true
(exit 0)
```

```
$ ./heroes test docs/panel/234-briefs/probe/core-prefix/main.hero
ok   "is_get"
ok   "contains"
2 tests, all passed
(exit 0)
```

```
$ ./heroes check docs/panel/234-briefs/probe/core-prefix/collision.hero
error[module_binding_taken]: this file already calls `core/strings` by the name `strings`, so `strings` cannot have it too — a file may not bind one name twice
  at docs/panel/234-briefs/probe/core-prefix/collision.hero:2:5
  2 | use strings
  note: the two modules may both exist and both be used — `as` says what THIS file calls each one, and two files may name the same module differently
(exit 1)
```

## 3. Names: a module named like a built-in, a function named like one, an extern across modules

```
$ ./heroes run docs/panel/234-briefs/probe/names/sort_beside_builtin.hero
1
1
(exit 0)
```

```
$ ./heroes check docs/panel/234-briefs/probe/names/builtin_named_function.hero
error[unknown_in_module]: module `strings` declares nothing called `find`
  at docs/panel/234-briefs/probe/names/builtin_named_function.hero:4:19
  4 |     print(strings.find(s: "abc", part: "b").must())
error[builtin_name_taken]: `find` is a built-in of the language, so the name is taken everywhere — pick another name
  at docs/panel/234-briefs/probe/names/strings.hero:2:10
  2 | function find(s: str, part: str) -> i64?
(exit 1)
```

```
$ ./heroes check docs/panel/234-briefs/probe/names/extern_across.hero
error[extern_across_modules]: `abs` is an `extern` of module `raw` — an extern is never reachable across a module boundary, because only `raw` carries the header that checks it
  at docs/panel/234-briefs/probe/names/extern_across.hero:4:15
  4 |     print(raw.abs(-3))
  note: §4.19: write a Heroes function beside the `extern` in `raw` and call that — the wrapper is compiled in the module whose header clang checks
(exit 1)
```

## 4. The near-name suggestion on a qualified name, and Go's contract on a `T?`

The package declares `index_of`. The files are named apart because this Mac's file system does not tell `call_index.hero` from `call_Index.hero`, which the first run of this probe found by overwriting one with the other.

```
$ ./heroes check docs/panel/234-briefs/probe/near-names/call_go_lower.hero
error[unknown_in_module]: module `core/strings` declares nothing called `index`
  at docs/panel/234-briefs/probe/near-names/call_go_lower.hero:4:19
  4 |     print(strings.index(s: "abc", part: "b").must())
(exit 1)
```

```
$ ./heroes check docs/panel/234-briefs/probe/near-names/call_go_capital.hero
error[unknown_in_module]: module `core/strings` declares nothing called `Index`
  at docs/panel/234-briefs/probe/near-names/call_go_capital.hero:4:19
  4 |     print(strings.Index(s: "abc", part: "b").must())
(exit 1)
```

```
$ ./heroes check docs/panel/234-briefs/probe/near-names/call_camel.hero
error[unknown_in_module]: module `core/strings` declares nothing called `indexOf` — it has `index_of`
  at docs/panel/234-briefs/probe/near-names/call_camel.hero:4:19
  4 |     print(strings.indexOf(s: "abc", part: "b").must())
  fix (guess): write `strings.index_of`
(exit 1)
```

```
$ ./heroes check docs/panel/234-briefs/probe/near-names/go_contract.hero
error[bad_operand]: `<` takes any integer or a float, found `i64?`
  at docs/panel/234-briefs/probe/near-names/go_contract.hero:4:11
  4 |     print(strings.index_of(s: "abc", part: "b") < 0)
  fix (guess): `.must()` — abort on the error case, giving `i64`
(exit 1)
```

## 5. UFCS and labels across a module

```
$ ./heroes run docs/panel/234-briefs/probe/ufcs/qualified.hero
true
(exit 0)
```

```
$ ./heroes check docs/panel/234-briefs/probe/ufcs/receiver.hero
error[needs_qualifying]: `has_prefix` is declared in module `core/strings`, and a name from another module is always written qualified — `strings.has_prefix`
  at docs/panel/234-briefs/probe/ufcs/receiver.hero:5:16
  5 |     print(line.has_prefix(prefix: "GET "))
  fix (guess): write `strings.has_prefix`
(exit 1)
```

```
$ ./heroes check docs/panel/234-briefs/probe/ufcs/unlabelled.hero
error[needs_label]: two of `has_prefix`'s parameters are `str`, so every one of them is named at the call site — this one is `s`
  at docs/panel/234-briefs/probe/ufcs/unlabelled.hero:5:30
  5 |     print(strings.has_prefix(line, prefix: "GET "))
  fix (certain): write `s: `
(exit 1)
```
