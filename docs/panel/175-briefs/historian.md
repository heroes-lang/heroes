# Panel 175 — historian

Read `00-shared.md` first. Your seat is advisory, holds no veto, and **must
verify every date, name and claim by web search**; an unsourced precedent is
inadmissible. You have no file write, so the coordinator writes your report to
`docs/panel/175-reports/historian.md` from what you return, verbatim.

## What to find, with sources

1. **Recording the deallocator per allocation and refusing the wrong one at run
   time**, which is route A's shape. Candidates to verify rather than assume:
   AddressSanitizer's `alloc-dealloc-mismatch`; Valgrind Memcheck's
   *Mismatched free() / delete / delete []*; Microsoft's Application Verifier
   and page heap; the MSVC debug heap's block types. For each: when it shipped,
   what it records, whether it aborts or reports, and whether it has a
   documented false-positive class — the interchangeable-releaser case.
2. **Refusing the wrong one at compile time**: the Clang Static Analyzer's
   `unix.MismatchedDeallocator`, GCC's `-Wmismatched-dealloc` and the
   `malloc (deallocator)` attribute (GCC 11), and what each can and cannot see
   across a function boundary.
3. **Putting the releaser in the TYPE**, which is route C's shape: C++'s
   `std::unique_ptr<T, Deleter>`, Rust's allocator parameter on `Box`, and any
   FFI binding generator that gives one C type two wrapper types by how it was
   acquired. What did it cost the functions that take the C type either way?
4. **A crash handler that speaks when the C allocator kills the process**,
   which is route E: what glibc, macOS libmalloc and the Windows heap print on
   a double free by default, and whether any language runtime adds its own line
   there with no bookkeeping of its own to consult.

Say, for each, what it predicts for Heroes, and one falsifiable prediction with
the milestone at which it is checkable.
