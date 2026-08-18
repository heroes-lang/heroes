/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0 */

//! What to declare a parameter as, given the C type clang reported (§4.19).
//!
//! Split out of `ffi_narrowed.rs` on 2026-08-16, and the seam is a different
//! question rather than a line count. That file asks **which clang message is
//! this** — three phrase families, one location gate, and a rule about which
//! class owns which string. This one answers **what is this C type called here**,
//! which is a lookup and knows nothing about diagnostics.
//!
//! **It is not a list of every C type and must not become one.** A row exists
//! because a real header used it and somebody bound it; the table's own comments
//! record which. `size_t` and `ssize_t` need none, because clang reports the
//! **canonical** type and `malloc`'s parameter arrives as `unsigned long`.
//!
//! **A missing row is not a failure any more** (panel 071). It used to make the
//! whole diagnostic decline, which is right for a width — a gap in this table is
//! a gap to report — and catastrophic for a class mismatch, where the types with
//! no Heroes name are precisely the interesting ones: a `Vec2` parameter's repair
//! is *declare `record Vec2` in the group*, a sentence rather than a type name.
//! So the caller reports either way and only the `Fix` asks here.

/// A Heroes type to declare a parameter as, and what the reader must know about
/// how this answer was reached.
pub(super) struct Spelling {
    pub(super) heroes: String,
    /// Present when the C type's width is the **platform's** rather than a width
    /// the header chose, so the note must say which target this answer is for.
    pub(super) caveat: Option<String>,
}

/// The Heroes type a C type of this width and signedness is declared as. The set
/// is `IntKind`'s, and a C type outside it means the narrowing is one this
/// language cannot currently express — which is a gap to report, not a fix to
/// propose, so the class stays silent rather than guessing.
///
/// **`long` and `unsigned long` used to be absent, and their absence was the
/// defect** (author decision 2026-08-15, `/decide`). They are 64 bits on Darwin
/// and Linux and 32 on Windows, so no *tabled* spelling is right on three legs —
/// and the first draft tabled one anyway, marked the fix `guess`, and named the
/// platform variance in the justification, which is CLAUDE.md §11's failure mode
/// written out. Declining was the right correction to that and the wrong answer to
/// the question: it left `function malloc(size: i64) -> ptr` exiting **2** with raw
/// clang output, blaming the compiler for a declaration the author wrote — while
/// `u64` binds `malloc` correctly, so the language could express it and only the
/// diagnostic could not.
///
/// The answer is that a width is not a constant to be tabled. It is a question
/// about the target, and `word_width_bits` asks it.
pub(super) fn spelling(c_type: &str) -> Option<Spelling> {
    let fixed = |heroes: &str| {
        Some(Spelling { heroes: heroes.to_string(), caveat: None })
    };
    match c_type {
        // **The two pointer rows, owed by the class family** (panel 071). A width
        // mismatch never reports one of these; a class mismatch reports them
        // constantly, because `ptr` and `cstr` are what an author reaches for
        // first at the boundary.
        "void *" => fixed("ptr"),
        "char *" | "const char *" => fixed("cstr"),
        "int" => fixed("i32"),
        "short" | "short int" => fixed("i16"),
        "signed char" | "char" => fixed("i8"),
        "unsigned int" => fixed("u32"),
        "unsigned short" | "unsigned short int" => fixed("u16"),
        "unsigned char" => fixed("u8"),
        // **`long long` is the one C integer that is 64 bits everywhere**, by
        // C99's own floor (`LLONG_MIN` ≤ −(2^63−1)) and by every ABI that has
        // shipped since. It carries no caveat because there is nothing about the
        // target left to say. It was missing from the table for the same reason
        // `long` was: nobody had bound a function that takes one.
        "long long" | "long long int" => fixed("i64"),
        "unsigned long long" | "unsigned long long int" => fixed("u64"),
        // `size_t` and `ssize_t` need no rows: clang reports the **canonical**
        // type, so `malloc`'s parameter arrives here as `unsigned long` and not
        // as `size_t`. Verified on the message this class was built from.
        "long" | "long int" => Some(word_width('i')),
        "unsigned long" | "unsigned long int" => Some(word_width('u')),
        // A C type absent from this table means the class **declines** — clang's
        // own verdict is still printed, and the compiler does not pretend to know
        // the repair. `float` is the live example (§4.19: `f32` does not exist).
        _ => None,
    }
}

/// The spelling for a C type whose width is the machine's word, with the note
/// that says so.
fn word_width(sign: char) -> Spelling {
    let bits = word_width_bits();
    // **The name of this machine is asked of the machine, not inferred from the
    // width** (panel 062's audit, 2026-08-15). This read `if bits == 64 { "Darwin
    // and Linux" } else { "Windows" }` — a premise about which platforms exist,
    // written inside the function whose own doc comment two paragraphs down
    // explains that it does not have one. It is falsifiable **today** and without a
    // `--target`: on `armv7-unknown-linux-gnueabihf` a C `long` is 32 bits, and the
    // note would have told a Linux reader they were on Windows.
    //
    // `std::env::consts::OS` is a fact about the value in hand — the machine this
    // compiler is running on — which is the distinction CLAUDE.md §11 turns on.
    let here = match std::env::consts::OS {
        "macos" => "macOS",
        "linux" => "Linux",
        "windows" => "Windows",
        other => other,
    };
    let other = if bits == 64 { 32 } else { 64 };
    Spelling {
        heroes: format!("{sign}{bits}"),
        caveat: Some(format!(
            "C's `long` is the platform's word, not a width the header chose: {bits} bits on this machine ({here}), and {other} elsewhere — Windows is {other} where the Unixes are {bits}, or the reverse. `{sign}{bits}` is the answer for this target; a program that must build on both declares the width it means there and converts at the call"
        )),
    }
}

/// How wide C's `long` is in the C this compiler is about to emit, in bits.
///
/// **Derived, never tabled, and the derivation is the whole point.** `c_ulong` is
/// Rust's name for the same C type clang is about to compile, and both run on this
/// machine for this target: `heroes` has no `--target` and cross-compilation is not
/// on the command surface (CLAUDE.md §10 — one argv table, and it has no such
/// flag). So this is a fact about the value in hand rather than a premise about
/// which platforms exist, which is the distinction CLAUDE.md §11 turns on — a fact
/// about the value cannot expire, and the tabled version expired the moment a third
/// CI leg arrived.
///
/// **The claim, written so it can die loudly**: `size_of::<c_ulong>()` is
/// `sizeof(unsigned long)` in the emitted C. `the_word_width_this_compiler_claims_is_the_one_clang_uses`
/// in `crates/heroes-cli/tests/surface.rs` builds a binding at this width through
/// the real toolchain and fails the day the two disagree — which is the day a
/// `--target` flag lands, and it will fail *in the milestone that adds it* rather
/// than in a diagnostic somebody reads six months later.
fn word_width_bits() -> usize {
    std::mem::size_of::<std::os::raw::c_ulong>() * 8
}
