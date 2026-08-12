//! The operator table — the *data* half of `heroes mutate` (design.md Part 11).
//!
//! `mod.rs`'s first rule is that the operators are data, not code: they live in
//! `harness/mutations/operators.md` with the plausible-mistake class each
//! imitates and the §-rule that should kill it. This file is that list plus the
//! dispatch that turns an id into an edit; `edits.rs` is the code.
//!
//! One consequence is worth stating, because it looks like an omission. An
//! operator whose row names **no** rule that kills it is legal here.
//! `typo-code` is the first: it exists to measure a hole rather than to confirm
//! a defence, and an instrument that could only report success would not be one.

use crate::source::Source;
use crate::syntax::parse;

use super::edits;

pub struct Operator {
    pub id: &'static str,
    /// The plausible-mistake class, from the operators table.
    pub imitates: &'static str,
}

pub const OPERATORS: [Operator; 12] = [
    Operator { id: "swap-args", imitates: "classic LLM argument inversion" },
    Operator { id: "drop-case", imitates: "a forgotten variant case" },
    Operator { id: "forget-at-decl", imitates: "mutability confusion" },
    Operator { id: "mutate-undeclared", imitates: "a silent new variable in other languages" },
    Operator { id: "typo-ident", imitates: "a one-character edit" },
    Operator { id: "typo-code", imitates: "a one-character edit inside an error code" },
    Operator { id: "wildcard-variant", imitates: "a lazy catch-all" },
    Operator { id: "positional-named", imitates: "style transfer from Python" },
    Operator { id: "mix-int-float", imitates: "an implicit-conversion prior" },
    Operator { id: "shadow", imitates: "an inner-scope habit" },
    Operator { id: "drop-question", imitates: "forgotten error propagation" },
    Operator { id: "typo-digit", imitates: "a one-digit slip in a number copied from elsewhere" },
];

/// Every mutant one operator makes from one source.
pub fn apply(id: &str, name: &str, text: &str) -> Vec<String> {
    let src = Source::new(name.to_string(), text.to_string());
    let parsed = parse(&src);
    if !parsed.diagnostics.is_empty() {
        return Vec::new();
    }
    let ast = &parsed.ast;
    match id {
        "swap-args" => edits::swap_args(ast, &src),
        "drop-case" => edits::drop_case(ast, &src),
        "forget-at-decl" => edits::forget_at_decl(ast, &src),
        "mutate-undeclared" => edits::mutate_undeclared(ast, &src),
        "typo-ident" => edits::typo_ident(ast, &src),
        "typo-code" => edits::typo_code(ast, &src),
        "wildcard-variant" => edits::wildcard_variant(ast, &src),
        "positional-named" => edits::positional_named(ast, &src),
        "mix-int-float" => edits::mix_int_float(ast, &src),
        "shadow" => edits::shadow(ast, &src),
        "drop-question" => edits::drop_question(ast, &src),
        "typo-digit" => edits::typo_digit(ast, &src),
        _ => Vec::new(),
    }
}
