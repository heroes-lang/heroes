# Panel 176 — historian

Read `00-shared.md` first. Advisory, no veto, and **every date, name and claim
is verified by web search** with its source beside it; unsourced is
inadmissible. You have no file write: reply with your full report and the
coordinator writes it out verbatim.

## What to find

1. **Marks that say a callee takes ownership, as against ending the life.**
   Candidates to verify: the Clang Static Analyzer's `ownership_takes`,
   `ownership_holds` and `ownership_returns` attributes and what distinguishes
   *takes* from *holds*; CoreFoundation's `CF_CONSUMED`, `CF_RETURNS_RETAINED`
   and `CF_RETURNS_NOT_RETAINED`; GObject Introspection's `(transfer full)`,
   `(transfer none)` and `(transfer container)`; Vala's `owned`; Swift's
   `consuming`/`__owned` parameters; Rust's `Box::into_raw`/`from_raw` as the
   FFI idiom. For each: what it marks, who writes it, and what enforces it.
2. **Reference counts at a boundary.** How the same systems spell *adds a
   reference* against *begins a life* — `CFRetain`, `g_object_ref`,
   `ns_returns_retained` — and whether any runtime counts references per address
   and what it does on a mismatch.
3. **Two declarations of one function.** C11's compatible-types rule for
   redeclarations and composite types; Clang's handling of attributes that
   disagree across redeclarations (the `noescape` revert panel 171's historian
   dated); GObject Introspection's one `.gir` per library; any binding generator
   that refuses two bindings of one symbol with different ownership.
4. **One call, a contract chosen by an argument.** SQLite's destructor argument,
   and how other language bindings spell `SQLITE_STATIC` against
   `SQLITE_TRANSIENT` — Python's `sqlite3` module, `rusqlite`, Go's
   `mattn/go-sqlite3` — and whether any exposes both through one function.

Say what each predicts for Heroes on V1, V2, V3, R1 and A2 (the llm-ergonomist's
brief names them), an advisory verdict, and one falsifiable prediction with the
milestone at which it is checkable.
