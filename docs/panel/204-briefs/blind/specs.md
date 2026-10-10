# Panel 204's blind seat: the spec each folder read

Every folder read the trunk's `spec/heroes-spec.md` at `635e8f67`: `h1-*`, `h2-*`, `h3-*` unchanged; `j1-*` and `k1-*` with the spec-warden's F7; `j2-*` and `k2-*` with F7 and G1m. The changed lines, as `diff` prints them against `635e8f67`:

## F7

```
113,114c113,114
< Declaration order never matters; mutual recursion needs no forward
< declarations. There are no mutable globals. Constants use SCREAMING_CASE, and
---
> Declaration order never changes what a program means; mutual recursion needs
> no forward declarations. There are no mutable globals. Constants use SCREAMING_CASE, and
```

## F7 and G1m

```
113,114c113,114
< Declaration order never matters; mutual recursion needs no forward
< declarations. There are no mutable globals. Constants use SCREAMING_CASE, and
---
> Declaration order never changes what a program means; mutual recursion needs
> no forward declarations. There are no mutable globals. Constants use SCREAMING_CASE, and
348c348,350
< libraries. A group names its header, and `link` a library when the symbols need one. clang
---
> libraries. A group names its header, and `link` a library when the symbols need one; C
> reads a module's headers in the order its groups are written, so one that needs
> another's names comes after it. clang
```
