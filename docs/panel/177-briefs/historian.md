# Panel 177 — historian

Read `00-shared.md` first. You judge precedent, advisory, no veto. **Every
date, version, line count and quotation is verified by web search and cited
with its URL**; an unsourced precedent is inadmissible, and *I did not find it*
is a finding. You have no write tool: return your report as your final
message, and the coordinator writes it to `docs/panel/177-reports/historian.md`
unchanged.

## Question 1 — a handle used after its life ended

For each, what the system does to a value used after the operation that ended
it, and whether that check is static, dynamic or absent:

- **Moved-from values**: Rust's move semantics across an FFI boundary; C++'s
  use-after-move diagnostics (clang-tidy `bugprone-use-after-move`, Clang's
  `-Wconsumed` / `consumable` typestate attributes); Swift's `consume` operator
  and `~Copyable` types.
- **Generation-counted handles**: slot maps / generational indices; Vale's
  generational references; Windows kernel handle tables and whether a closed
  handle's value is reused; Vulkan's non-dispatchable handles.
- **Run-time invalidation of the name**: any system that overwrites or poisons
  the variable that held a freed pointer (the coordinator's route P) —
  debug allocators, `FREE_AND_NULL`-style macros, anything else you find. Say
  what each gives up.
- Design.md Part 6 refuses a borrow checker on cost since 2026-09-13. Is there a
  system that took **one** of these routes for foreign handles only, without a
  borrow checker for its own values?

## Question 2 — ownership that transfers only on success, and added references

- **Conditional transfer**: GObject-Introspection's `(transfer full)` and what
  it says of an error return; Clang's `ownership_takes` / `ownership_holds`
  attributes; CoreFoundation's `CF_CONSUMED` and whether it consumes on
  failure; OpenSSL's `add0` / `set0` convention and what its pages say of
  failure; the Linux kernel's rule for functions that take a reference.
- **A reference added through a result or a parameter**: `CF_RETURNS_RETAINED`,
  `ns_returns_retained` and `ns_consumed`; GObject's `g_object_ref` and its
  `(transfer full)` return; whether any system annotates a PARAMETER as
  gaining a reference, as OpenSSL's `X509_up_ref(X509 *a)` does.
- For both: where the success condition lives in the annotation — on the
  parameter, on the result, or nowhere — in each system that has one.

## What to return

A verdict on each route and form from precedent alone, with the condition under
which precedent would change it, and one falsifiable prediction.
