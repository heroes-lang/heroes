  `match` too, z02); a65 refused (`no_value` at the checked `if`).
- MEASURED: the checked `if`'s rule cannot land before the statement
  `match`'s: alone it refused `typed_statements_before_values` (a70's shape),
  legal on the trunk.
- Depth (deepest nesting `check` survives; depth.sh, depth_call.sh):
  trunk if 192, match 161, value 123, call 95; 201b99af 192, 161, 123, 97;
  statement half 195, 163, 123, 96. A first draft lost 6 and 7 levels
  (frames: arms +1152 bytes, expression_statement +1200, clang
  -fstack-usage on the emitted C); the arm rule moved out of `arms` and the
  statement `if` kept its own test.
- Not this lane's files: z03 (`constant M: i64` over `assert true`) costs two
  `no_value` at 2:5, the second false (*this `i64` produces no value, every
  branch jumps*): `selfhost/check/decls.hero:54` reads an absent value as a
