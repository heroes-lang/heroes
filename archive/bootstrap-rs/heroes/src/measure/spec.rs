//! The spec against the compiler: the two must name the same language.
//!
//! `spec/heroes-spec.md` is not documentation, it is **the prompt** (§1.6) — the
//! whole context a model writing Heroes ever has. So a rule the compiler enforces
//! and the spec omits is not terseness, it is a wrong briefing, and the project
//! shipped one for four milestones: panel 013 ratified `(function(A) -> B)` while
//! the spec still said `(fn(A) -> B)`, which is a *reserved-word error* in every
//! position. §4.20's inventory listed `sort` and the int-to-string conversion; the
//! spec listed neither, so a `function sort(...)` was spec-legal and
//! resolver-rejected.
//!
//! These tests are the mechanism that makes that class of defect loud. They do
//! not check prose — they check the two places where the spec makes a *claim the
//! compiler can be asked about*: the built-in names it promises, and the spellings
//! it uses for forms the lexer has an opinion about.

#[cfg(test)]
mod tests {
    use crate::measure::spec_path;
    use crate::resolve::BUILTINS;

    fn spec() -> String {
        std::fs::read_to_string(spec_path()).expect("the spec must exist")
    }

    /// Every identifier written inside a code span — a backticked fragment or a
    /// fenced block. That is where the spec makes claims a compiler can be asked
    /// about; its prose is not searched, because `all` and `has` are English words.
    fn identifiers_in_code() -> std::collections::BTreeSet<String> {
        let text = spec();
        let mut code = String::new();
        for (index, part) in text.split("```").enumerate() {
            if index % 2 == 1 {
                code.push_str(part);
                code.push('\n');
                continue;
            }
            for (inner, span) in part.split('`').enumerate() {
                if inner % 2 == 1 {
                    code.push_str(span);
                    code.push('\n');
                }
            }
        }
        code.split(|c: char| !c.is_alphanumeric() && c != '_')
            .filter(|word| !word.is_empty())
            .map(|word| word.to_string())
            .collect()
    }

    /// Every name the compiler reserves must appear in the spec, because a name a
    /// model cannot see is a name it will redeclare — and one it cannot find is a
    /// name it will write by hand.
    #[test]
    fn the_spec_names_every_builtin_the_compiler_reserves() {
        let mentioned = identifiers_in_code();
        let missing: Vec<&str> = BUILTINS
            .iter()
            .map(|b| b.name)
            .filter(|name| !mentioned.contains(*name))
            .collect();
        assert_eq!(
            missing,
            Vec::<&str>::new(),
            "the compiler reserves these and the spec never mentions them"
        );
    }

    /// …and the reverse: a name the spec offers must exist, or a model will call
    /// something that is not there.
    #[test]
    fn every_name_the_spec_offers_as_a_builtin_exists() {
        let text = spec();
        let start = text.find("Built-ins:").expect("the built-in list must exist");
        // **The list ends where its own sentence ends** — the first `.` outside a
        // code span. It used to end where the *next* sentence began, keyed on the
        // literal text "None of these names", with a blank line as the fallback and
        // a comment saying the fallback was already known to be wrong (it swallows
        // the prose after it, which names a *type* in a code span).
        //
        // That is CLAUDE.md §11's shape exactly, and it expired the way §11 says a
        // premise expires — **in silence, with the comment still reading as
        // correct**. Panel 081 spent that very sentence as clause A's named removal,
        // measured at −10 and verified loud on four shapes; deleting it dropped this
        // parser onto the fallback its own comment condemned, and it read `inf` out
        // of the float-printing sentence and called it a missing built-in. Nothing
        // about the language changed. A sentence somewhere else did.
        //
        // So the question is asked of the value now: this sentence's own full stop.
        // The scan skips code spans because `print(...)` and `slice(from:, to:)`
        // carry dots and colons of their own, and the list is one sentence by
        // construction — every entry is separated by `·` and the last is followed by
        // the period this finds.
        let end = start + end_of_sentence(&text[start..]);
        // Parenthesised prose *outside* a code span is not part of the list — the
        // `slice(from:, to:)` entry gained "(`to` excluded)" at M-strings-ownership (panel 021), and
        // `to` is a word about the built-in rather than the name of one. Code spans
        // are kept whole, so `print(...)` survives.
        let paragraph = without_prose_parentheses(&text[start..end]);
        let listed: Vec<String> = paragraph
            .split('`')
            .skip(1)
            .step_by(2)
            .map(|name| name.trim_end_matches("(...)").trim().to_string())
            .filter(|name| !name.is_empty() && !name.contains(' '))
            .collect();
        assert!(listed.len() > 10, "the list looks truncated: {listed:?}");
        // **The delimiter is pinned at both ends, and this is the half that was
        // missing.** `len() > 10` passes just as happily on a sentence that ends
        // early, so a delimiter that quietly shortened the list would shrink this
        // test's coverage and stay green — which is how the previous delimiter's
        // fallback would have failed if the panel's removal had not made it fail
        // loudly instead. `range` is the last entry by construction.
        assert_eq!(
            listed.last().map(String::as_str),
            Some("range"),
            "the built-in list no longer ends at `range` — the sentence delimiter \
             found the wrong full stop, and every name after it is now unchecked: {listed:?}"
        );
        for name in listed {
            let bare = name.split('(').next().unwrap_or(&name).to_string();
            assert!(
                BUILTINS.iter().any(|b| b.name == bare),
                "the spec offers `{bare}` and the compiler has no such built-in"
            );
        }
    }

    /// The offset just past this sentence's terminating `.` — counting only full
    /// stops **outside** a code span, because `print(...)` is not the end of
    /// anything. Falls back to the whole slice if the sentence never terminates,
    /// which the caller's `listed.len() > 10` assertion then catches.
    fn end_of_sentence(text: &str) -> usize {
        let mut in_span = false;
        let mut chars = text.char_indices().peekable();
        while let Some((at, c)) = chars.next() {
            match c {
                '`' => in_span = !in_span,
                '.' if !in_span => {
                    let next_is_break =
                        chars.peek().is_none_or(|(_, n)| n.is_whitespace());
                    if next_is_break {
                        return at;
                    }
                }
                _ => {}
            }
        }
        text.len()
    }

    /// Drop every `(…)` group that is not inside a code span.
    fn without_prose_parentheses(text: &str) -> String {
        let mut out = String::with_capacity(text.len());
        let mut in_code = false;
        let mut depth = 0u32;
        for c in text.chars() {
            if c == '`' {
                in_code = !in_code;
            }
            if !in_code {
                if c == '(' {
                    depth += 1;
                    continue;
                }
                if c == ')' && depth > 0 {
                    depth -= 1;
                    continue;
                }
            }
            if depth == 0 {
                out.push(c);
            }
        }
        out
    }

    /// No word the lexer rejects may appear **in a code span**. `fn` is the case
    /// that happened: the foreign-word registry calls it an error *and* the spec
    /// used it in the function type, so every function-typed annotation a model
    /// wrote from the spec was a compile error with a certain fix attached — for
    /// four milestones.
    ///
    /// Code spans only. The prose says "Construct an error with…", and forbidding
    /// the English word "construct" because `struct` is a substring of it would be
    /// a test that fails for a reason nobody can act on.
    #[test]
    fn the_spec_never_uses_a_word_the_compiler_rejects() {
        let mentioned = identifiers_in_code();
        let forbidden: Vec<&String> = mentioned
            .iter()
            .filter(|word| crate::lexer::is_foreign_word(word))
            .collect();
        assert_eq!(
            forbidden,
            Vec::<&String>::new(),
            "these appear in the spec's code and the lexer rejects them with a prescribed fix"
        );
    }

    /// The budget, as a test rather than a habit: §1.6's ceiling is 4096 measured
    /// (panel 024; 3000 before it, 2000 before that, and an unmeasured 1500 first),
    /// and the soft line stays at 2000. Breaching the hard one fails here.
    #[test]
    fn the_spec_is_under_the_measured_ceiling() {
        let text = spec();
        let measured = crate::measure::measure(&text, &crate::measure::vendor_dir())
            .expect("the vendored tokenisers must load");
        assert!(
            measured.max() <= 4096,
            "the spec measures {} against §1.6's ceiling of 4096",
            measured.max()
        );
    }
}
