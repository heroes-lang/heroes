//! The spec and the built-in table, held to saying the same thing.
//!
//! This file exists because of a defect it would have caught. M6 step 2 struck
//! `has` (panel 026 R7): the resolver lost the name, `design.md` recorded the
//! removal, the built-in *inventory* line in the spec lost its entry — and the
//! sentence that taught it, three sections earlier, stayed. A program written
//! from the spec answered `error[unknown_name]: nothing named `has` is in
//! scope`, and nothing in this project noticed, because **no test reads the
//! spec**. Every other artifact here is checked against the compiler; the one
//! document CLAUDE.md §1 calls the source of truth was checked against nothing.
//!
//! Two directions, and both are one-line failures rather than a diff to read:
//! a built-in the spec never names is unreachable for the only reader that
//! matters (§1.1), and a name the spec still teaches after the compiler dropped
//! it is a promise the compiler breaks.

use crate::resolve::BUILTINS;

/// The spec, read from the repo rather than from a fixture: a copy would age
/// out and pin nothing.
fn spec() -> String {
    let path = concat!(env!("CARGO_MANIFEST_DIR"), "/../../spec/heroes-spec.md");
    std::fs::read_to_string(path).expect("spec/heroes-spec.md is where CLAUDE.md §1 says it is")
}

/// The three ways the spec writes a built-in's name, and no fourth: as an
/// inventory entry (`` `join` ``), as a call (`range(a, b)`), or as the UFCS
/// spelling of a reader (`` `.must()` ``, `` `.ok x` ``).
///
/// Matching these forms rather than the bare word is what lets the struck-name
/// direction below stay honest: `has` the English verb appears in the type
/// section ("`child: Node` has no size") and must not count as a mention.
fn mentions(text: &str, name: &str) -> bool {
    text.contains(&format!("`{name}`"))
        || text.contains(&format!("{name}("))
        || text.contains(&format!("`.{name}"))
}

/// §1.11's inventory is reserved everywhere, in both tiers — so a name in the
/// table that the spec never writes is a name no reader can use and no reader
/// can avoid declaring.
#[test]
fn every_reserved_builtin_is_named_in_the_spec() {
    let spec = spec();
    let unnamed: Vec<&str> =
        BUILTINS.iter().map(|b| b.name).filter(|name| !mentions(&spec, name)).collect();
    assert!(
        unnamed.is_empty(),
        "reserved but never written in the spec: {unnamed:?} — \
         a reader cannot use the name and cannot declare it either"
    );
}

/// Names this project decided to remove. One row per struck built-in, carrying
/// the panel that struck it, because the failure message has to say *why* the
/// name is forbidden rather than just that it is.
///
/// A row is added when a removal lands and is never deleted: the point is that
/// the spec cannot drift back into promising it.
const STRUCK: [(&str, &str); 1] = [(
    "has",
    "struck by panel 026 R7 — map access returns `V?` always, so `has(m, k)` \
     and `!m[k].is_err()` were two spellings of one predicate",
)];

/// The direction that failed. Struck at −17 measured, of which only −4 shipped:
/// the inventory line was edited and the prose was not.
#[test]
fn a_struck_builtin_is_unfindable_in_the_spec() {
    let spec = spec();
    for (name, why) in STRUCK {
        assert!(
            !mentions(&spec, name),
            "the spec still teaches `{name}`, which the compiler does not have: {why}"
        );
        assert!(
            !BUILTINS.iter().any(|b| b.name == name),
            "`{name}` is back in the built-in table: {why}"
        );
    }
}
