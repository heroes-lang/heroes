//! The two `int main` shims: the program's, and the test runner's (design.md
//! §3.1, §4.18).
//!
//! Split out of `decls.rs` on 2026-08-12 by author decision — the same cut panel
//! 038's compiler-engineer named when it predicted the file would pass 750 lines
//! unsplit.
//!
//! **A shim, not a mangler exception.** Heroes' `main` becomes `h_<module>_main`
//! like every other function, and a separate `int main` calls it. Vala does
//! exactly this (`ccode.add_return (main_call)`), Nim's generated `main` is a shim
//! with `NimMain()` behind it, and Zig's C backend once emitted `void main(void)`,
//! which clang rejects. These are also the only two functions in a generated unit
//! whose name C chose, which is why the runtime's own bookkeeping lives here:
//! `hero_args_set` on the way in, `hero_runtime_check_leaks` on the way out.

use crate::ir::{Function, Program};
use crate::source::Source;

use super::mangle;
use super::writer::Writer;

/// `int main(int argc, char **argv)` for a test build: run the test whose index
/// is `argv[1]`, and nothing else.
///
/// **One process per test**, which is what the argument is for. An `assert` is a
/// panic (§4.18 and `runtime/parts/failure.c`), so a runner that called them in
/// sequence would stop at the first failure and hide every test after it — the
/// one thing a test runner must not do. `heroes test` therefore compiles once and
/// executes the binary once per test.
///
/// No test title reaches the C: the index is the whole protocol, and the titles
/// stay in the compiler where they are already exact.
pub(super) fn test_shim(w: &mut Writer, program: &Program, src: &Source) {
    let tests: Vec<&Function> = program
        .functions
        .iter()
        .filter(|f| f.kind == crate::ir::FnKind::Test && !src.is_library(f.span.start))
        .collect();
    w.blank();
    w.at_generated();
    w.line("int main(int argc, char **argv) {");
    // The same first call the program's `main` makes. A test build does not want
    // the arguments — its one argument is the test's index — but it wants the
    // streams put in the state the language promises, which is where `\n` is one
    // byte on every platform (2026-08-14, the third CI leg's second run).
    w.line("    hero_args_set(argc, argv);");
    w.line("    int64_t which = hero_test_index(argc, argv);");
    w.line("    switch (which) {");
    // The index is the test's POSITION, and the C name is its DECLARATION index:
    // the first is the protocol `heroes test` counts in, the second is what makes
    // the name unique in a file that also has functions. Keeping them apart here
    // is what stops the two from drifting — the switch is the one place they meet.
    for (position, function) in tests.iter().enumerate() {
        w.line(&format!("    case {position}:"));
        w.line(&format!(
            "        {}();",
            mangle::test(src.component_at(function.span.start), function.decl as usize)
        ));
        w.line("        break;");
    }
    w.line("    default:");
    w.line("        hero_panic(\"no test with that index — this is a compiler bug\");");
    w.line("    }");
    w.line("    hero_runtime_check_leaks();");
    w.line("    return 0;");
    w.line("}");
}

/// The shim. `main` itself is mangled, so this is the only function in the unit
/// whose name C chose.
pub(super) fn shim(w: &mut Writer, function: &Function, src: &Source) {
    w.blank();
    w.at_generated();
    // **`argc`/`argv`, always, whether or not the program calls `args()`.** The
    // alternative — two shims chosen by whether a name is reachable — is a
    // condition that can be wrong, and the cost of the parameters is nothing.
    // `hero_args_set` is called before the program's own `main` so that the
    // arguments are there for the first statement (M-ffi-ladder, `hero_os.h`).
    w.line("int main(int argc, char **argv) {");
    w.line("    hero_args_set(argc, argv);");
    w.line(&format!(
        "    {}();",
        mangle::function(src.component_at(function.span.start), &function.name)
    ));
    // The leak gate, and it is here because AddressSanitizer is **not** one on this
    // platform: `detect_leaks is not supported`, measured, with a 999-block leak
    // exiting 0 in silence. A live-block counter asserted at exit names a count
    // instead of a stack, works everywhere, and is deterministic (panel 021 R9).
    w.line("    hero_runtime_check_leaks();");
    w.line("    return 0;");
    w.line("}");
}
